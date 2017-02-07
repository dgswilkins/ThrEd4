#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)

extern fPOINT*	adclp (unsigned count);
extern fPOINT*	adflt (unsigned count);
extern SATCON*	adsatk (unsigned count);
extern void		alrotmsg ();
extern void		angrct (fRECTANGLE* rectangle);
extern void		centr ();
extern void		chkcwlk ();
extern unsigned	chkMap (unsigned bit);
extern void		chkrng (fPOINT* range);
extern unsigned	chku (unsigned bit);
extern void		chkund ();
extern void		chkwlk ();
extern void		clpxadj ();
extern void		coltab ();
extern void		dasyfrm ();
extern void		delfstchs ();
extern void		delinf ();
extern void		delstchm ();
extern void		deltx ();
extern unsigned	duthrsh (double threshold);
extern void		duzrat ();
extern void		fdelstch ();
extern BOOL		filmsgs (unsigned code);
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
extern void		moveStitch (fPOINTATTR* destination, fPOINTATTR* source);
extern void		movStch ();
extern TCHAR*	mvflpnt (fPOINT* destination, fPOINT* source, unsigned count);
extern void		mvsatk (SATCON* destination, SATCON* source, unsigned count);
extern void		mvstch (unsigned destination, unsigned source);
extern void		mvstchs (unsigned destination, unsigned source, unsigned count);
extern void		numWnd ();
extern void		nuRct ();
extern void		pntmsg (unsigned count);
extern unsigned	px2stch ();
extern void		pxCor2stch (POINT point);
extern void		redclp ();
extern void		redraw (HWND window);
extern void		ritfcor (fPOINT* point);
extern void		ritmov ();
extern void		ritnum (unsigned code, unsigned value);
extern void		ritot (unsigned number);
extern void		rngadj ();
extern void		rotang1 (fPOINTATTR unrotatedPoint, fPOINT* rotatedPoint);
extern void		rotangf (fPOINT unrotatedPoint, fPOINT* rotatedPoint);
extern void		rotflt (fPOINT* point);
extern void		rotfn ();
extern void		rstAll ();
extern unsigned	rstMap (unsigned bit);
extern void		savdo ();
extern void		sCor2px (dPOINT stitchCoordinate, POINT* pixelCoordinate);
extern void		sdmsg ();
extern void		selRct (fRECTANGLE* sourceRect);
extern void		setangf (double angle);
extern void		setfchk ();
extern unsigned	setMap (unsigned bit);
extern void		setpsel ();
extern unsigned	setRmap (fPOINTATTR stitchPoint);
extern void		setxt ();
extern void		shft (fPOINT delta);
extern void		shoMsg (TCHAR* string);
extern void		shord ();
extern void		shoseln (unsigned code0, unsigned code1);
extern void		spltmsg ();
extern void		srtcol ();
extern void		stch2pxr (fPOINT stitchCoordinate);
extern void		tabmsg (unsigned code);
extern unsigned	toglMap (unsigned bit);
extern unsigned	toglu (unsigned bit);
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
extern			HCURSOR			ArrowCursor;
extern			unsigned		BitmapHeight;
extern			unsigned		BitmapWidth;
extern			unsigned		ButtonHeight;
extern			unsigned		ButtonWidth;
extern			unsigned		ButtonWidthX3;
extern			HWND			CancelButton;
extern			unsigned		Clip;
extern			fPOINTATTR		ClipBuffer[MAXFRMLINS];
extern			HGLOBAL			ClipMemory;
extern			void*			ClipPointer;
extern			fRECTANGLE		ClipRect;
extern			FLSIZ			ClipRectSize;
extern			unsigned		ClipStitchCount;
extern			CLPSTCH*		ClipStitchData;
extern			unsigned		ClosestPointIndex;
extern			HCURSOR			CrossCursor;
extern			HWND			DeleteStitchesDialog;
extern			HWND			DiscardButton;
extern			POINT			EndPointCross;
extern			HMENU			FillMenu;
extern			unsigned		FillStartsMap;
extern			HWND			FormDataSheet;
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
extern			INIFILE			IniFile;
extern			POINT			InsertLine[3];
extern			fPOINT			InterleaveSequence[MAXSEQ];
extern			unsigned		InterleaveSequenceIndex;
extern			unsigned		InterleaveSequenceIndex2;
extern			INSREC			InterleaveSequenceIndices[10];
extern			HPEN			LayerPen[5];
extern			HMENU			MainMenu;
extern			HWND			MainStitchWin;
extern			unsigned		MarkedStitchMap[RMAPSIZ];
extern			double			MinStitchLength;
extern			MSG				Msg;
extern			TCHAR			MsgBuffer[MSGSIZ];
extern			unsigned		MsgIndex;
extern			RECT			MsgRect;
extern			HWND			MsgWindow;
extern			HPEN			MultiFormPen;
extern			unsigned		NearestCount;
extern			HWND			OKButton;
extern			TCHAR*			PcdClipFormat;
extern			PCSHEADER		PCSHeader;
extern			unsigned		PreferenceIndex;
extern			HWND			PreferencesWindow;
extern			double			RotationAngle;
extern			dPOINT			RotationCenter;
extern			fRECTANGLE		RotationRect;
extern			unsigned		SearchLineIndex;
extern			HPEN			SelectAllPen;
extern			FRMHED*			SelectedForm;
extern			FORMVERTICES	SelectedFormVertices;
extern			fPOINT			SelectedFormsSize;
extern			RECT			SelectedPixelsRect;
extern			fPOINT			SelectedPoint;
extern			fRECTANGLE		SelectedVerticesRect;
extern			double			ShowStitchThreshold;
extern			HWND			SideMessageWindow;
extern			HWND			SideWindow[11];
extern			TCHAR			SideWindowEntryBuffer[11];
extern			double			SmallStitchLength;
extern			double			StitchBoxesThreshold;
extern			fPOINTATTR		StitchBuffer[MAXPCS];
extern			fRECTANGLE		StitchRangeRect;
extern			POINT			StitchCoordinatesPixels;
extern			RECT			StitchWindowClientRect;
extern			HDC				StitchWindowDC;
extern			HDC				StitchWindowMemDC;
extern			POINT			StitchWindowOrigin;
extern			POINT			StitchWindowSize;
extern			POINT			StretchBoxLine[5];
extern			TCHAR*			StringTable[STR_LEN];
extern			int				TextureIndex;
extern			TXPNT			TexturePointsBuffer[MAXSEQ];
extern			RNGCNT*			TextureSegments;
extern			HINSTANCE		ThrEdInstance;
extern			HWND			ThrEdWindow;
extern			POINT			ThredWindowOrigin;
extern			TCHAR			ThrName[_MAX_PATH];
extern			unsigned		AppliqueColor;
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
void			clRmap (unsigned len);
void			contf ();
void			deleclp (unsigned ind);
void			delmclp (unsigned ind);
void			duangs ();
void			dubfn ();
void			dufcntr (dPOINT* cntr);
void			dulast ();
void			dufxlen ();
void			duhart (unsigned nsids);
void			durpoli (unsigned nsids);
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

unsigned		FormMenuEntryCount;		//lines in the form-form
float			MaxStitchLen;			//maximum stitch length
float			UserStichLen;			//user stitch length
double*			ListSINEs;				//a list of sins of form angles
double*			ListCOSINEs;			//a list of cosins of form angles
double			AdjustedSpace;			//adjusted space
unsigned		NextStart;				//index of the endpoint of the line segment being processed
fPOINT*			ChainEndPoints;			//end points of chain stiches
unsigned		ChainSequence[] = { 0,1,2,3,0,1,4,3,0,3 };//chain stich sequence
double			Div4;					//chain space divided by four
unsigned		ChainCount;				//number of elements of the chain sequence to process
unsigned		ClosestFormToCursor;	//closest form to the cursor
unsigned		ClosestVertexToCursor;	//formOrigin closest to the cursor
double			VerticalRatio;			//vertical ratio between the zoom window
										// and the entire stitch space
double			HorizontalRatio;		//horizontal ratio between the zoom window
										// and the entire stitch space
POINT			FormLines[MAXFRMLINS];	//used in the form drawing routines
unsigned		NewFormVertexCount;		//points in the new form
FORMINFO		FormInfo;				//form info used in drawing forms
FRMHED			FormList[MAXFORMS];		//a list of pointers to forms
unsigned		FormIndex = 0;			//index into the list of forms
double			StitchSpacing = DEFSPACE*PFGRAN;//stitch spacing in stitch units
fPOINT*			CurrentFillVertices;	//pointer to the line of the polygon being filled
fPOINT*			ClipFillData;			//data for clipboard fills
fPOINT*			ClipReversedData;		//data for clipboard fills
unsigned		StitchLineCount;		//count of stitch lines
SMALPNTL*		LineEndpoints;			//pairs of fill line endpoints
unsigned		ActivePointIndex;		//pointer to the active formOrigin in the sequencing algorithm
unsigned		LineGroupIndex;			//pointer for groups of fill line segments
SMALPNTL**		SortedLines;			//sorted pointers to LineEndpoints
unsigned short	VertexCount;				//sides of the selected form to fill
unsigned		SortedLineIndex;		//for connecting fill lines
unsigned		SequenceIndex;			//sequencing pointer
BSEQPNT			BSequence[BSEQLEN];		//reverse sequence for polygon fills
fPOINT			OSequence[OSEQLEN];		//temporary storage for sequencing
double			Slope;					//slope of line in angle fills
unsigned		SatinIndex;				//pointer to next satin formOrigin to enter
fPOINT			FormMoveDelta;			//offset for moving forms
fPOINT			TempPolygon[MAXFRMLINS];	//temporary storage when user is entering a polygon;
unsigned		OutputIndex;			//output pointer for sequencing
double*			Lengths;				//array of cumulative lengths used in satin fills
fPOINT*			CurrentFormVertices;	//points in the currently selected form
SATCON*			CurrentFormGuides;	//connections in the currently selecteed form
unsigned short	CurrentFormConnectionsCount;	//number of connections in the currently selected form
unsigned short	WordParam;				//word paramater from the currently selected form
unsigned*		CheckMap;				//bitmap used by satin fill to delete illegal lines
unsigned short	StartPoint;				//starting formOrigin for a satin stitch guide-line
double			HorizontalLength2;		//horizontal length of a clipboard fill/2
double			HorizontalLength;		//horizontal length of a clipboard fill
fPOINT			LastPoint;				//last formOrigin written by line connect routine
fPOINT			OutsidePointList[MAXFRMLINS];	//list of outside outline points for satin or clipboard fills
fPOINT			InsidePointList[MAXFRMLINS];	//list of inside outline points for satin or clipboard fills
fPOINT*			OutsidePoints;			//pointer to the list of outside outline points
fPOINT*			InsidePoints;			//pointer to the list of inside outline points
fPOINT			ClipReference;			//clipboard reference formOrigin
double			BorderWidth = BRDWID;	//border width for satin borders
unsigned		SelectedFormControlVertex;	//user selected form control formOrigin
POINT			FormControlPoints[10];	//form control rectangle in pixel coordinates
double			XYratio;				//expand form aspect ratio
HWND			ValueWindow[LASTLIN];	//data handles for the form data sheet
HWND			LabelWindow[LASTLIN];	//text handles for the form data sheet
RECT			LabelWindowCoords;		//location of left windows in the form data sheet
RECT			ValueWindowCoords;		//location of right windows in the form data sheet
POINT			LabelWindowSize;		//size of the left windows in the form data sheet
POINT			ValueWindowSize;		//size of the right windows in the form data sheet
fPOINT			LowerLeftStitch;		//lower left formOrigin in a form
VRCT2*			FillVerticalRect;		//fill points for vertical satin fill
VRCT2*			UnderlayVerticalRect;	//underlay fill points for vertical satin fill
POINT			RubberBandLine[3];		//points to form points to be moved
unsigned*		Xhistogram;				//x histogram for snap together
double			SnapLength = SNPLEN*PFGRAN;		//snap together length
unsigned*		Xpoints;				//stitch indices sorted according to x values
unsigned		ColorBitmap;			//bitmap of colors in a design for sort
double			StarRatio = STARAT;			//star formOrigin to body ratio
double			SpiralWrap = SPIRWRAP;		//number of revolutions in a spiral
unsigned		Srtmsk = (1 << EDGEANGSAT) | (1 << EDGEAPPL) | (1 << EDGEPROPSAT);	 //mask for switchable fill types
fRECTANGLE		BoundingRect;			//isin rectangle
RCON*			PathMap;				//path map for sequencing
unsigned		PathMapIndex;			//number of entries in the path map
TCHAR*			VisitedRegions;			//visited character map for sequencing
unsigned		VisitedIndex;			//next unvisited region for sequencing
unsigned		RegionCount;			//number of regions to be sequenced
REGION*			RegionsList;			//a list of regions for sequencing
unsigned		DoneRegion;				//last region sequenced
double			GapToClosestRegion;		//region close enough threshold for sequencing
unsigned*		MapIndexSequence;		//pointers to sets of adjacent regions
RGSEQ*			RegionPath;				//path to a region
unsigned		GroupIndexCount;		//number of group indices
unsigned*		GroupIndexSequence;		//array of group indices for sequencing
unsigned		LastGroup;				//group of the last line written in the previous region;
FSEQ*			SequencePath;			//path of sequenced regions
RGSEQ*			TempPath;				//temporary path connections
unsigned		SequencePathIndex;		//index to path of sequenced regions
unsigned		NextGroup;				//group that connects to the next region
unsigned*		SequenceFlagBitmap;		//a bitmap of sequenced lines
REGION*			CurrentRegion;			//region currently being sequenced
fPOINT			LastRegionCorners[4];	//corners of last region sequenced
FRMHED			AngledForm;				//a temporary rotated form for angle fills
unsigned short	SelectedFormList[MAXFORMS];		//a list of selected forms
unsigned		SelectedFormCount = 0;	//number of selected forms
unsigned		PreviousFormIndex;		//previously selected form
RECT			SelectedFormsRect;		//for multiple selections;
POINT			SelectedFormsLine[9];	//line derived from the big rectangle
POINT			SelectedPointsLine[9];	//line derived from the formOrigin select rectangle
fRECTANGLE		AllItemsRect;			//rectangle enclosing all forms and stitches
double			FormAngles[MAXFRMLINS];	//angles of a form for satin border fills
fPOINT			FormVertices[MAXFRMPNTS];	//form points
unsigned		FormVertexIndex;		//next index to append form points
fPOINT			ClipPoints[MAXCLPNTS];	//main clipboard fill points for forms
unsigned		ClipPointIndex;			//next index to append main clipboard points
SATCON			SatinConnects[MAXSAC];	//satin form connects
unsigned		SatinConnectIndex;		//next index to append satin connect points
float			ButtonholeCornerLength = IBFCLEN;	//buttonhole corner length
float			PicotSpacing = IPICSPAC;	//space between border picots
unsigned		PseudoRandomValue;		//pseudo-random sequence register
dPOINT			SatinBackup[8];			//backup stitches in satin fills
unsigned		SatinBackupIndex;		//pointer for backup stitches in satin fills
double			ClipAngle;				//for clipboard border fill
dPOINT			MoveToCoords;			//moving formOrigin for clipboard fill
double			CosAngle;				//cosine for clipboard fill
double			SinAngle;				//sine for clipboard fill
fPOINT			BorderClipReference;	//reference for clipboard line border
unsigned		CurrentSide;			//active form formOrigin for line clipboard fill
dPOINT			Vector0;				//x size of the clipboard fill at the fill angle
FRMHED*			TempFormList;			//temporary form header storage for reordering forms
fPOINT*			TempFormVertices;		//temporary form vertex storage for reordering forms
SATCON*			TempSatinConnects;		//temporary satin guidline storage for reordering forms
fPOINT*			TempClipPoints;			//temporary clipboard formOrigin storage for reordering forms
unsigned		FormRelocationIndex;	//form relocator pointer
fPOINTATTR*		TempStitchBuffer;		//pointer to high stitch area for form sort
unsigned		BeanCount;				//number of stitches added by convert to bean
FRMHED*			FormForInsert;			//insert form vertex in this form
unsigned		FormVertexPrev;			//form vertex storage for form vertex insert
unsigned		FormVertexNext;			//form vertex storage for form vertex insert
SMALPNTL*		SequenceLines;			//line for vertical/horizontal/angle fills
unsigned		PathIndex;				//formOrigin to the next path element for vertical fill sequencing
fPOINT*			WorkingFormVertices;	//form points for angle fills
fPOINT			AngledFormVertices[MAXFRMLINS];	//form formOrigin data for angle fills
CLPSEG*			ClipSegments;			//clipboard segments for virtual clipboard fill
unsigned		ClipSegmentIndex;		//clipboard segment pointer
unsigned short	ClipIntersectSide;		//clipboard intersect side;
float**			SortedLengths;			//array of sorted side lengths for vertical clipboard fill
fPOINT			lineSegmentStart;		//vertical clipboard line segment start
fPOINT			LineSegmentEnd;			//vertical clipboard line segment end
double*			ClipSideLengths;		//lengths of form sides for vertical clipboard fill
CLIPSORT*		ClipIntersectData;		//intersect points for vertical clipboard fill
CLIPSORT**		ArrayOfClipIntersectData;	//pointers to line intersect points
CLIPNT*			ClipStitchPoints;		//points for vertical clipboard fills
VCLPX			RegionCrossingData[MAXFRMLINS];	//region crossing data for vertical clipboard fills
unsigned		RegionCrossingStart;	//start of region crossing data for a particular region
unsigned		RegionCrossingEnd;		//end of region crossing data for a particular region
float			ClipWidth;				//horizontal spacing for vertical clipboard fill
HWND			TimeWindow;				//progress bar
HDC				TimeDC;					//progress bar device context
double			TimeStep;				//progress bar step
double			TimePosition;			//progress bar postiion
FLOAT			FormOffset;				//form offset for clipboard fills
long			PreferenceWindowWidth;	//width of the preference window
double			EggRatio;				//ratio for shrinking eggs
unsigned		PreferenceWindowTextWidth;	//size of the text part of the preference window

TCHAR		FormOnOff[16];

MENUITEMINFO MenuInfo = {
	sizeof(MENUITEMINFO),	// Size
	MIIM_TYPE,				// Mask
	MFT_STRING,				// Type
	0,						// State
	0,						// ID
	0,						// SubMenu
	0,						// bmpChecked
	0,						// bmpUnchecked
	0,						// ItemData
	FormOnOff,				// TypeData
	16,						// cch
#if(WINVER >= 0x0500)
	0						// bmpItem
#endif /* WINVER >= 0x0500 */
};

unsigned short EdgeArray[] = {
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

unsigned char Level00 = 0;
unsigned char Level01 = 1;
unsigned char Level02[] = { 0,1 };
unsigned char Level03[] = { 1,0,2 };
unsigned char Level04[] = { 1,3,0,2 };
unsigned char Level05[] = { 2,0,3,1,4 };
unsigned char Level06[] = { 3,0,2,4,1,5 };
unsigned char Level07[] = { 3,0,4,1,6,2,5 };
unsigned char Level08[] = { 4,0,5,1,3,6,2,7 };
unsigned char Level09[] = { 4,0,5,1,6,2,7,3,8 };
unsigned char Level10[] = { 5,0,6,1,7,2,8,3,9,4 };
unsigned char Level11[] = { 5,0,6,1,7,2,8,3,9,10 };
unsigned char Level12[] = { 6,0,7,1,8,2,9,3,10,4,11 };
unsigned char Level13[] = { 6,0,1,7,2,8,3,9,4,10,5,11,6,12 };
unsigned char Level14[] = { 7,0,8,1,9,2,10,3,11,4,12,5,13,6 };
unsigned char Level15[] = { 7,0,8,1,9,2,10,3,11,4,12,5,13,6,14,7,15 };

unsigned char* Levels[] = {
&Level00,&Level01,&Level02[0],&Level03[0],&Level04[0],&Level05[0],&Level06[0],&Level07[0],&Level08[0],
&Level09[0],&Level10[0],&Level11[0],&Level12[0],&Level13[0],&Level14[0],&Level15[0]
};


void frmcpy(FRMHED* destination, FRMHED* source) {
#if	 __UseASM__
	unsigned index = sizeof(FRMHED);

	_asm {
		mov		edi, destination
		mov		ecx, index
		shr		ecx, 2
		mov		esi, source
		rep		movsd
	}
#else
	memcpy(destination, source, sizeof(FRMHED));
#endif
}

void frmclr(FRMHED* destination) {
#if	 __UseASM__
	unsigned formHeaderSize = sizeof(FRMHED);

	_asm {
		mov		edi, destination
		mov		ecx, formHeaderSize
		shr		ecx, 2
		xor eax, eax
		rep		stosd
	}
#else
	memset(destination, 0, sizeof(FRMHED));
#endif
}

void duinf(FRMHED* formHeader) {
#if	 __UseASM__
	_asm {
		mov		ebx, formHeader
		mov		eax, [ebx]
		mov		ebx, offset FormInfo
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
	FormInfo.type = formHeader->attribute & 0xf;
	FormInfo.attribute = (formHeader->attribute >> 4) & 0xf;
	FormInfo.sideCount = formHeader->vertexCount;
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
cmp1 :	sub		ecx, 8
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
cmp2 :	xor		eax, eax
		jmp		short cmpx
cmpg :	xor		eax, eax
		inc		eax
cmpx :
	}
#else
	const dPOINTLINE **Point1 = (const dPOINTLINE **)arg1, **point2 = (const dPOINTLINE **)arg2;

	if ((*point2)->y < (*Point1)->y)
		return 1;
	if ((*point2)->y > (*Point1)->y)
		return -1;

	if ((*point2)->x < (*Point1)->x)
		return 1;
	if ((*point2)->x > (*Point1)->x)
		return -1;

	return 0;
#endif
}

void getfinfo(unsigned iForm) {
	duinf(&FormList[iForm]);
}

unsigned satind(SATCON* guide) {
#if	 __UseASM__
	_asm {
		mov		eax, guide
		sub		eax, offset SatinConnects
		shr		eax, 2
	}
#else
	return guide - SatinConnects;
#endif
}

void dusqr() {
	if (chku(SQRFIL))
		SelectedForm->extendedAttribute |= AT_SQR;
	else
		SelectedForm->extendedAttribute &= (~AT_SQR);
}

void sacspac(SATCON* startGuide, unsigned guideCount) {
	int			iStartGuide;
	int			source, destination;
	unsigned	iForm;

	iStartGuide = satind(startGuide);
	source = SatinConnectIndex - 1;
	destination = SatinConnectIndex + guideCount - 1;
	while (source >= iStartGuide) {
		SatinConnects[destination].start = SatinConnects[source].start;
		SatinConnects[destination--].finish = SatinConnects[source--].finish;
	}
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			FormList[iForm].satinOrAngle.guide += guideCount;
	}
	SatinConnectIndex += guideCount;
}

SATCON* nusac(unsigned formIndex, unsigned guideCount) {
	unsigned iForm, guideIndex;

	guideIndex = 0;
	for (iForm = 0; iForm < formIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			guideIndex += FormList[iForm].satinGuideCount;
	}
	FormList[formIndex].satinOrAngle.guide = &SatinConnects[guideIndex];
	sacspac(FormList[formIndex].satinOrAngle.guide, guideCount);
	return FormList[formIndex].satinOrAngle.guide;
}

void delclps(unsigned iForm) {
	deleclp(iForm);
	delmclp(iForm);
}

unsigned findclp(unsigned formIndex) {
	int iForm;

	for (iForm = formIndex - 1; iForm >= 0; iForm--) {
		if (iseclp(iForm))
			return FormList[iForm].borderClipData - ClipPoints + FormList[iForm].clipEntries;
		if (isclp(iForm))
			return FormList[iForm].angleOrClipData.clip - ClipPoints + FormList[iForm].lengthOrCount.clipCount;
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
	unsigned iForm;

	for (iForm = fpnt + 1; iForm < FormIndex; iForm++) {
		if (isclpx(iForm))
			FormList[iForm].angleOrClipData.clip -= cnt;
		if (iseclpx(fpnt))
			FormList[iForm].borderClipData -= cnt;
	}
}

void delmclp(unsigned iForm) {
	unsigned	source, destination;

	if (ClipPointIndex) {
		if (isclp(iForm)) {
			destination = findclp(iForm);
			source = destination + FormList[iForm].lengthOrCount.clipCount;
			MoveMemory(&ClipPoints[destination], &ClipPoints[source], sizeof(fPOINT)*(ClipPointIndex - source));
			if (iseclp(iForm))
				FormList[iForm].borderClipData -= FormList[iForm].lengthOrCount.clipCount;
			clpsub(iForm, FormList[iForm].lengthOrCount.clipCount);
			if (ClipPointIndex > FormList[iForm].lengthOrCount.clipCount)
				ClipPointIndex -= FormList[iForm].lengthOrCount.clipCount;
			else
				ClipPointIndex = 0;
			FormList[iForm].lengthOrCount.clipCount = 0;
		}
	}
}

void deleclp(unsigned iForm) {
	unsigned	source, destination;

	if (ClipPointIndex) {
		if (iseclpx(iForm)) {
			destination = findclp(iForm);
			source = destination + FormList[iForm].clipEntries;
			while (source < ClipPointIndex) {
				ClipPoints[destination].x = ClipPoints[source].x;
				ClipPoints[destination++].y = ClipPoints[source++].y;
			}
			clpsub(iForm, FormList[iForm].clipEntries);
			if (ClipPointIndex > FormList[iForm].clipEntries)
				ClipPointIndex -= FormList[iForm].clipEntries;
			else
				ClipPointIndex = 0;
			FormList[iForm].clipEntries = 0;
		}
	}
}

fPOINT* nueclp(unsigned formIndex, unsigned count) {
	int			find;
	int			source, destination;
	unsigned	iform;

	find = findclp(ClosestFormToCursor);
	if (isclp(ClosestFormToCursor))
		find += FormList[ClosestFormToCursor].lengthOrCount.clipCount;
	source = ClipPointIndex - 1;
	destination = ClipPointIndex + count - 1;
	while (source >= find) {
		ClipPoints[destination].x = ClipPoints[source].x;
		ClipPoints[destination--].y = ClipPoints[source--].y;
	}
	FormList[find].borderClipData = &ClipPoints[find];
	// Todo - Should this be "iForm = 0"?
	for (iform = formIndex; iform < FormIndex; iform++) {
		if (iseclpx(iform))
			FormList[iform].borderClipData += count;
	}
	for (iform = formIndex + 1; iform < FormIndex; iform++) {
		if (isclp(iform))
			FormList[iform].angleOrClipData.clip += count;
	}
	ClipPointIndex += count;
	return &ClipPoints[find];
}

fPOINT* numclp() {
	int			find;
	int			source, destination;
	unsigned	iForm;

	find = findclp(ClosestFormToCursor);
	source = ClipPointIndex - 1;
	destination = ClipPointIndex + ClipStitchCount - 1;
	while (source >= find) {
		ClipPoints[destination].x = ClipPoints[source].x;
		ClipPoints[destination--].y = ClipPoints[source--].y;
	}
	FormList[ClosestFormToCursor].angleOrClipData.clip = &ClipPoints[find];
	if (iseclpx(ClosestFormToCursor))
		FormList[ClosestFormToCursor].borderClipData += ClipStitchCount;
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (isclpx(iForm))
			FormList[iForm].angleOrClipData.clip += ClipStitchCount;
		if (iseclpx(iForm))
			FormList[iForm].borderClipData += ClipStitchCount;
	}
	ClipPointIndex += ClipStitchCount;
	return &ClipPoints[find];
}

unsigned fltind(fPOINT* point) {
#if	 __UseASM__
	_asm {
		mov		eax, point
		sub		eax, offset FormVertices
		shr		eax, 3
	}
#else
	return point - FormVertices;
#endif
}

unsigned sacind(SATCON* guide) {
#if	 __UseASM__
	_asm {
		mov		eax, guide
		sub		eax, offset SatinConnects
		shr		eax, 2
	}
#else
	return guide - SatinConnects;
#endif
}

unsigned clpind(fPOINT* point) {
#if	 __UseASM__
	_asm {
		mov		eax, point
		sub		eax, offset ClipPoints
		shr		eax, 3
	}
#else
	return point - ClipPoints;
#endif
}

void fltspac(fPOINT* start, unsigned count) {
	int			startIndex;
	int			source, destination;
	unsigned	iForm;

	startIndex = fltind(start);
	source = FormVertexIndex - 1;
	destination = FormVertexIndex + count - 1;
	while (source >= startIndex) {
		FormVertices[destination].x = FormVertices[source].x;
		FormVertices[destination--].y = FormVertices[source--].y;
	}
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		FormList[iForm].vertices += count;
	FormVertexIndex += count;
}

void delsac(unsigned formIndex) {
	unsigned	iForm;
	unsigned	source, destination;

	if (SatinConnectIndex) {
		if (FormList[formIndex].type == SAT&&FormList[formIndex].satinGuideCount) {
			destination = satind(FormList[formIndex].satinOrAngle.guide);
			source = destination + FormList[formIndex].satinGuideCount;
			while (source < SatinConnectIndex) {
				SatinConnects[destination].start = SatinConnects[source].start;
				SatinConnects[destination++].finish = SatinConnects[source++].finish;
			}
			for (iForm = formIndex + 1; iForm < FormIndex; iForm++) {
				if (FormList[iForm].type == SAT&&FormList[iForm].satinGuideCount)
					FormList[iForm].satinOrAngle.guide -= FormList[formIndex].satinGuideCount;
			}
			SatinConnectIndex -= FormList[formIndex].satinGuideCount;
		}
	}
	FormList[formIndex].satinGuideCount = 0;
}

void delflt(unsigned formIndex) {
	unsigned	iForm;
	unsigned	source, destination;

	if (FormList[formIndex].vertexCount) {
		destination = fltind(FormList[formIndex].vertices);
		source = destination + FormList[formIndex].vertexCount;
		while (source < FormVertexIndex) {
			FormVertices[destination].x = FormVertices[source].x;
			FormVertices[destination++].y = FormVertices[source++].y;
		}
		for (iForm = formIndex + 1; iForm < FormIndex; iForm++)
			FormList[iForm].vertices -= FormList[formIndex].vertexCount;
		FormVertexIndex -= FormList[formIndex].vertexCount;
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
	unsigned iSequence;

	for (iSequence = 0; iSequence < SequenceIndex; iSequence++)
		rotflt(&OSequence[iSequence]);
}

void delfil(unsigned attribute) {
	unsigned iSource, iDestination;

	attribute &= TYPMSK | FRMSK;
	iDestination = iSource = 0;
	rstMap(WASDEL);
	while ((StitchBuffer[iSource].attribute&(TYPMSK | FRMSK)) != attribute&&iSource < PCSHeader.stitchCount)
		iSource++;
	iDestination = iSource;
	if (iSource < PCSHeader.stitchCount) {
		setMap(WASDEL);
	}
	while (iSource < PCSHeader.stitchCount) {
		if ((StitchBuffer[iSource].attribute&(TYPMSK | FRMSK)) != attribute) {
			StitchBuffer[iDestination].attribute = StitchBuffer[iSource].attribute;
			StitchBuffer[iDestination].x = StitchBuffer[iSource].x;
			StitchBuffer[iDestination++].y = StitchBuffer[iSource].y;
		}
		iSource++;
	}
	PCSHeader.stitchCount = iDestination;
}

void delmfil(unsigned color) {
	unsigned attribute, stitchAttribute;
	unsigned iSource, iDestination;

	if (istx(ClosestFormToCursor))
		deltx();
	delmclp(ClosestFormToCursor);
	// ToDo - color is not used. Remove parameter?
	color &= 0xf;
	attribute = ClosestFormToCursor << FRMSHFT;
	iDestination = 0;
	for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++)
	{
		stitchAttribute = StitchBuffer[iSource].attribute;
		if (!(stitchAttribute&NOTFRM))
		{
			if (!((stitchAttribute&FRMSK) == attribute && (stitchAttribute&(TYPFRM | FTHMSK))))
				mvstch(iDestination++, iSource);
		}
		else
			mvstch(iDestination++, iSource);
	}
	PCSHeader.stitchCount = iDestination;
}

void fsizpar() {
	SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
	SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
	SelectedForm->minFillStitchLen = MinStitchLength;
}

void chkcont() {
	unsigned iGuide, shortestGuideIndex = 0, length, minimumLength = 10000;

	delmclp(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	fsizpar();
	if (SelectedForm->fillType != CONTF) {
		if (CurrentFormConnectionsCount) {
			for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				length = SelectedForm->satinOrAngle.guide[iGuide].finish - SelectedForm->satinOrAngle.guide[iGuide].start;
				if (length < minimumLength) {
					minimumLength = length;
					shortestGuideIndex = iGuide;
				}
			}
			SelectedForm->angleOrClipData.guide.start = SelectedForm->satinOrAngle.guide[shortestGuideIndex].start;
			SelectedForm->angleOrClipData.guide.finish = SelectedForm->satinOrAngle.guide[shortestGuideIndex].finish;
			delsac(ClosestFormToCursor);
		}
		else {
			SelectedForm->angleOrClipData.guide.start = 1;
			SelectedForm->angleOrClipData.guide.finish = SelectedForm->vertexCount - 2;
		}
	}
	SelectedForm->type = FRMLINE;
	SelectedForm->fillType = CONTF;
	SelectedForm->attribute |= FRECONT;
}

unsigned find1st() {
	unsigned iStitch = 0, attribute;

	if (chk2of())
		return ClosestPointIndex;
	else {
		attribute = ClosestFormToCursor << 4;
		while (iStitch < PCSHeader.stitchCount) {
			if ((StitchBuffer[iStitch].attribute&FRMSK) == attribute) {
				if (!(StitchBuffer[iStitch].attribute&NOTFRM))
					return iStitch;
			}
			iStitch++;
		}
		return iStitch;
	}
}

void ispcdclp() {
	rstMap(WASPCDCLP);
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory)
			setMap(WASPCDCLP);
	}
	CloseClipboard();
}

void frmout(unsigned formIndex) {
	fRECTANGLE*		rectangle;
	unsigned	iVertex;
	float		offset;

	if (FormList[formIndex].vertexCount) {
		rectangle = &FormList[formIndex].rectangle;
		CurrentFormVertices = FormList[formIndex].vertices;
		rectangle->left = rectangle->right = CurrentFormVertices[0].x;
		rectangle->bottom = rectangle->top = CurrentFormVertices[0].y;
		for (iVertex = 1; iVertex < FormList[formIndex].vertexCount; iVertex++) {
			if (CurrentFormVertices[iVertex].x > rectangle->right)
				rectangle->right = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].x < rectangle->left)
				rectangle->left = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].y < rectangle->bottom)
				rectangle->bottom = CurrentFormVertices[iVertex].y;
			if (CurrentFormVertices[iVertex].y > rectangle->top)
				rectangle->top = CurrentFormVertices[iVertex].y;
		}
		if (rectangle->top - rectangle->bottom < MINRCT) {
			offset = (MINRCT - (rectangle->top - rectangle->bottom)) / 2;
			rectangle->top += offset;
			rectangle->bottom -= offset;
		}
		if (rectangle->right - rectangle->left < MINRCT) {
			offset = (MINRCT - (rectangle->right - rectangle->left)) / 2;
			rectangle->left -= offset;
			rectangle->right += offset;
		}
	}
}

void sfCor2px(fPOINT stitchPoint, POINT* screen) {
	screen->x = (stitchPoint.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	screen->y = StitchWindowClientRect.bottom - (stitchPoint.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void stCor2px(fPOINTATTR stitch, POINT* screen) {
	screen->x = (stitch.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	screen->y = StitchWindowClientRect.bottom - (stitch.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void px2stchf(POINT screen, fPOINT* stitchPoint) {
	double	factorX, factorY;

	// ToDo - is factorX calculated correctly?
	factorX = (double)(screen.x - StitchWindowClientRect.left) / StitchWindowClientRect.right;
	stitchPoint->x = factorX*(ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	factorY = (double)(StitchWindowClientRect.bottom - screen.y) / StitchWindowClientRect.bottom;
	stitchPoint->y = factorY*(ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

void frmlin(fPOINT* vertices, unsigned vertexCount) {
	unsigned iVertex;

	if (VertexCount) {
		for (iVertex = 0; iVertex < vertexCount; iVertex++) {
			FormLines[iVertex].x = (vertices[iVertex].x - ZoomRect.left)*ZoomRatio.x;
			FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[iVertex].y - ZoomRect.bottom)*ZoomRatio.y;
		}
		FormLines[iVertex].x = (vertices[0].x - ZoomRect.left)*ZoomRatio.x;
		FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom)*ZoomRatio.y;
	}
}

void dufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, FormLines, NewFormVertexCount);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unfrm() {
	if (rstMap(SHOFRM))
		dufrm();
}

void mdufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (FormList[ClosestFormToCursor].type == FRMLINE)
		Polyline(StitchWindowDC, FormLines, NewFormVertexCount - 1);
	else
		Polyline(StitchWindowDC, FormLines, NewFormVertexCount);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void munfrm() {
	if (rstMap(SHOFRM))
		mdufrm();
}

void rats() {
	if (chkMap(ZUMED)) {
		HorizontalRatio = (double)(ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
		VerticalRatio = (double)(ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	}
	else {
		HorizontalRatio = (double)UnzoomedRect.x / StitchWindowClientRect.right;
		VerticalRatio = (double)UnzoomedRect.y / StitchWindowClientRect.bottom;
	}
}

void setfrm() {
	unsigned	iVertex;
	fPOINT		point;
	fPOINT		delta;

	rats();
	ClosestFormToCursor = FormIndex;
	fvars(ClosestFormToCursor);
	px2stchf(FormLines[0], &point);
	delta.x = point.x - CurrentFormVertices[0].x;
	delta.y = point.y - CurrentFormVertices[0].y;
	SelectedForm->rectangle.left = SelectedForm->rectangle.bottom = (float)1e30;
	SelectedForm->rectangle.right = SelectedForm->rectangle.top = 0;
	for (iVertex = 0; iVertex < NewFormVertexCount - 1; iVertex++) {
		CurrentFormVertices[iVertex].x += delta.x;
		CurrentFormVertices[iVertex].y += delta.y;
		if (CurrentFormVertices[iVertex].x < SelectedForm->rectangle.left)
			SelectedForm->rectangle.left = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].x > SelectedForm->rectangle.right)
			SelectedForm->rectangle.right = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].y > SelectedForm->rectangle.top)
			SelectedForm->rectangle.top = CurrentFormVertices[iVertex].y;
		if (CurrentFormVertices[iVertex].y < SelectedForm->rectangle.bottom)
			SelectedForm->rectangle.bottom = CurrentFormVertices[iVertex].y;
	}
	FormIndex++;
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

void frmsqr(unsigned iVertex) {
	double	angle;
	double	ratio;
	float	length;
	fPOINT	offset;
	fPOINT	point;
	fPOINT	adjustedPoint;
	dPOINT	delta;
	POINT	line[4];

	stch2pxr(CurrentFormVertices[iVertex]);
	line[1].x = StitchCoordinatesPixels.x;
	line[1].y = StitchCoordinatesPixels.y;
	ratio = (double)IniFile.formVertexSizePixels / StitchWindowClientRect.right;
	length = (ZoomRect.right - ZoomRect.left)*ratio * 2;
	delta.x = CurrentFormVertices[iVertex - 1].x - CurrentFormVertices[iVertex].x;
	delta.y = CurrentFormVertices[iVertex - 1].y - CurrentFormVertices[iVertex].y;
	angle = atan2(delta.y, delta.x);
	offset.x = length*cos(angle);
	offset.y = length*sin(angle);
	point.x = CurrentFormVertices[iVertex].x + offset.x;
	point.y = CurrentFormVertices[iVertex].y + offset.y;
	angle = atan2(-delta.x, delta.y);
	length /= 2;
	offset.x = length*cos(angle);
	offset.y = length*sin(angle);
	adjustedPoint.x = point.x + offset.x;
	adjustedPoint.y = point.y + offset.y;
	stch2pxr(adjustedPoint);
	line[0].x = line[3].x = StitchCoordinatesPixels.x;
	line[0].y = line[3].y = StitchCoordinatesPixels.y;
	adjustedPoint.x = point.x - offset.x;
	adjustedPoint.y = point.y - offset.y;
	stch2pxr(adjustedPoint);
	line[2].x = StitchCoordinatesPixels.x;
	line[2].y = StitchCoordinatesPixels.y;
	Polyline(StitchWindowMemDC, line, 4);
}

// ToDo - selsqr, frmsqr0 and frmx are very similar. Can they be combined?
void selsqr(POINT controlPoint, HDC dc) {
	POINT	line[5];

	line[0].x = line[3].x = line[4].x = controlPoint.x - IniFile.formVertexSizePixels;
	line[0].y = line[1].y = controlPoint.y - IniFile.formVertexSizePixels;
	line[1].x = line[2].x = controlPoint.x + IniFile.formVertexSizePixels;
	line[2].y = line[3].y = controlPoint.y + IniFile.formVertexSizePixels;
	line[4].y = controlPoint.y - IniFile.formVertexSizePixels;
	Polyline(dc, line, 5);
}

void frmsqr0(POINT controlPoint) {
	POINT		line[5];
	unsigned	offset;

	offset = IniFile.formBoxSizePixels;
	if (offset)
	{
		line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
		line[0].y = line[1].y = controlPoint.y - offset;
		line[1].x = line[2].x = controlPoint.x + offset + 1;
		line[2].y = line[3].y = controlPoint.y + offset + 1;
		line[4].y = controlPoint.y - 1;
		Polyline(StitchWindowMemDC, line, 5);
	}
}

void frmx(POINT controlPoint, HDC dc) {
	POINT	line[2];

	SelectObject(dc, FormSelectedPen);
	line[0].x = line[1].x = controlPoint.x;
	line[0].y = controlPoint.y + 8;
	line[1].y = controlPoint.y - 8;
	Polyline(dc, line, 2);
	line[0].y = line[1].y = controlPoint.y;
	line[0].x = controlPoint.x - 8;
	line[1].x = controlPoint.x + 8;
	Polyline(dc, line, 2);
	SelectObject(dc, FormPen);
}

void ratsr() {
	if (chkMap(ZUMED)) {
		HorizontalRatio = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
		VerticalRatio = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		HorizontalRatio = (double)StitchWindowClientRect.right / UnzoomedRect.x;
		VerticalRatio = (double)StitchWindowClientRect.bottom / UnzoomedRect.y;
	}
}

float midl(float high, float low) {
	return (high - low) / 2 + low;
}

void fvars(unsigned ind) {
	SelectedForm = &FormList[ind];
	CurrentFormVertices = FormList[ind].vertices;
	VertexCount = FormList[ind].vertexCount;
	CurrentFormGuides = FormList[ind].satinOrAngle.guide;
	CurrentFormConnectionsCount = FormList[ind].satinGuideCount;
	WordParam = FormList[ind].wordParam;
}

void ritfrct(unsigned iForm, HDC dc) {
	unsigned		controlPoint;
	POINT			pixelOutline[10];
	fRECTANGLE*		rectangle;
	fPOINT			formOutline[10];

	ratsr();
	SelectObject(StitchWindowDC, FormPen);
	SetROP2(StitchWindowDC, R2_XORPEN);
	getfinfo(iForm);
	rectangle = &FormList[iForm].rectangle;
	SelectObject(dc, FormSelectedPen);
	formOutline[0].x = formOutline[6].x = formOutline[7].x = formOutline[8].x = rectangle->left;
	formOutline[1].x = formOutline[5].x = midl(rectangle->right, rectangle->left);
	formOutline[0].y = formOutline[1].y = formOutline[2].y = formOutline[8].y = rectangle->top;
	formOutline[3].y = formOutline[7].y = midl(rectangle->top, rectangle->bottom);
	formOutline[4].y = formOutline[5].y = formOutline[6].y = rectangle->bottom;
	formOutline[2].x = formOutline[3].x = formOutline[4].x = rectangle->right;
	for (controlPoint = 0; controlPoint < 8; controlPoint++)
		sfCor2px(formOutline[controlPoint], &pixelOutline[controlPoint]);
	sfCor2px(formOutline[0], &pixelOutline[controlPoint]);
	Polyline(dc, pixelOutline, 9);
	for (controlPoint = 0; controlPoint < 8; controlPoint++)
		selsqr(pixelOutline[controlPoint], dc);
	SetROP2(StitchWindowDC, R2_COPYPEN);
	if (rstMap(GRPSEL)) {
		rstMap(SELSHO);
		SearchLineIndex = 0;
		setMap(RESTCH);
	}
}

void delfrms() {
	unsigned iStitch;

	savdo();
	FormIndex = FormVertexIndex = SatinConnectIndex = ClipPointIndex = 0;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
	{
		StitchBuffer[iStitch].attribute &= NFRM_NTYP;
		StitchBuffer[iStitch].attribute |= NOTFRM;
	}
}

void fselrct(unsigned iForm) {
	FRMHED*		formHeader = &FormList[iForm];
	fPOINT		formOutline[5];
	POINT		line[6];
	unsigned	iPoint;

	formOutline[0].x = formOutline[3].x = formOutline[4].x = formHeader->rectangle.left;
	formOutline[1].x = formOutline[2].x = formHeader->rectangle.right;
	formOutline[0].y = formOutline[1].y = formOutline[4].y = formHeader->rectangle.top;
	formOutline[2].y = formOutline[3].y = formHeader->rectangle.bottom;
	for (iPoint = 0; iPoint < 5; iPoint++) {
		line[iPoint].x = (formOutline[iPoint].x - ZoomRect.left)*HorizontalRatio;
		line[iPoint].y = (ZoomRect.top - formOutline[iPoint].y)*VerticalRatio;
		if (line[iPoint].x < SelectedFormsRect.left)
			SelectedFormsRect.left = line[iPoint].x;
		if (line[iPoint].x > SelectedFormsRect.right)
			SelectedFormsRect.right = line[iPoint].x;
		if (line[iPoint].y < SelectedFormsRect.top)
			SelectedFormsRect.top = line[iPoint].y;
		if (line[iPoint].y > SelectedFormsRect.bottom)
			SelectedFormsRect.bottom = line[iPoint].y;
	}
	line[5].x = (formOutline[0].x - ZoomRect.left)*HorizontalRatio;
	line[5].y = (ZoomRect.top - formOutline[0].y)*VerticalRatio;
	if (line[5].x < SelectedFormsRect.left)
		SelectedFormsRect.left = line[5].x;
	if (line[5].x > SelectedFormsRect.right)
		SelectedFormsRect.right = line[5].x;
	if (line[5].y < SelectedFormsRect.top)
		SelectedFormsRect.top = line[5].y;
	if (line[5].y > SelectedFormsRect.bottom)
		SelectedFormsRect.bottom = line[5].y;
	Polyline(StitchWindowMemDC, line, 5);
}

void rct2sel(RECT rectangle, POINT* line) {
	line[0].x = line[6].x = line[7].x = line[8].x = rectangle.left;
	line[1].x = line[5].x = ((rectangle.right - rectangle.left) >> 1) + rectangle.left;
	line[2].x = line[3].x = line[4].x = rectangle.right;
	line[0].y = line[1].y = line[2].y = line[8].y = rectangle.top;
	line[3].y = line[7].y = ((rectangle.bottom - rectangle.top) >> 1) + rectangle.top;
	line[4].y = line[5].y = line[6].y = rectangle.bottom;
}

void dubig() {
	unsigned iPoint;

	rct2sel(SelectedFormsRect, SelectedFormsLine);
	SelectObject(StitchWindowMemDC, SelectAllPen);
	Polyline(StitchWindowMemDC, SelectedFormsLine, 9);
	for (iPoint = 0; iPoint < 8; iPoint++)
		selsqr(SelectedFormsLine[iPoint], StitchWindowMemDC);
}

void frmpoly(POINT* line, unsigned count) {
	unsigned iPoint;

	// ToDo - why iterate through the points instead of drawing as 1 polygon?
	if (count) {
		for (iPoint = 0; iPoint < count - 1; iPoint++)
			Polyline(StitchWindowMemDC, &line[iPoint], 2);
	}
}

void dupsel(HDC dc) {
	unsigned iPoint;

	SelectObject(dc, FormPen);
	SetROP2(dc, R2_XORPEN);
	Polyline(dc, SelectedPointsLine, 9);
	iPoint = SelectedFormVertices.start;
	for (iPoint = 0; iPoint < 8; iPoint++)
		selsqr(SelectedPointsLine[iPoint], dc);
	frmx(EndPointCross, dc);
}

void unpsel() {
	if (rstMap(SHOPSEL))
		dupsel(StitchWindowDC);
}

void drwfrm() {
	unsigned	iForm, iGuide, iVertex, lastPoint, layer;
	POINT		line[2];
	dPOINT		point;

	rstMap(SHOMOV);
	rstMap(SHOPSEL);
	line[0].x = line[0].y = line[1].x = line[1].y = 0;
	// ToDo - why draw the line when coords are 0?
	Polyline(StitchWindowMemDC, line, 2);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	ratsr();
	duzrat();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		fvars(iForm);
		frmlin(SelectedForm->vertices, VertexCount);
		lastPoint = 0;
		layer = ((SelectedForm->attribute&FRMLMSK) >> 1);
		if (!ActiveLayer || !layer || layer == ActiveLayer) {
			if (SelectedForm->type == SAT) {
				if (SelectedForm->attribute&FRMEND) {
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, FormLines, 2);
					lastPoint = 1;
				}
				if (SelectedForm->wordParam) {
					SelectObject(StitchWindowMemDC, FormPen);
					frmpoly(&FormLines[1], SelectedForm->wordParam);
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, &FormLines[SelectedForm->wordParam], 2);
					SelectObject(StitchWindowMemDC, LayerPen[layer]);
					lastPoint = SelectedForm->wordParam + 1;
				}
				for (iGuide = 0; iGuide < FormList[iForm].satinGuideCount; iGuide++) {
					sfCor2px(CurrentFormVertices[CurrentFormGuides[iGuide].start], &line[0]);
					sfCor2px(CurrentFormVertices[CurrentFormGuides[iGuide].finish], &line[1]);
					SelectObject(StitchWindowMemDC, FormPen);
					Polyline(StitchWindowMemDC, line, 2);
				}
			}
			SelectObject(StitchWindowMemDC, LayerPen[layer]);
			if (SelectedForm->type == FRMLINE) {
				frmpoly(FormLines, VertexCount);
				if (SelectedForm->fillType == CONTF) {
					point.x = CurrentFormVertices[SelectedForm->angleOrClipData.guide.start].x;
					point.y = CurrentFormVertices[SelectedForm->angleOrClipData.guide.start].y;
					sCor2px(point, &line[0]);
					point.x = CurrentFormVertices[SelectedForm->angleOrClipData.guide.finish].x;
					point.y = CurrentFormVertices[SelectedForm->angleOrClipData.guide.finish].y;
					sCor2px(point, &line[1]);
					Polyline(StitchWindowMemDC, line, 2);
				}
			}
			else
				frmpoly(&FormLines[lastPoint], VertexCount + 1 - lastPoint);
			if (ClosestFormToCursor == iForm&&chkMap(FRMPSEL)) {
				for (iVertex = 1; iVertex < VertexCount; iVertex++) {
					if (iVertex == ClosestVertexToCursor)
						frmx(FormLines[iVertex], StitchWindowMemDC);
					else
						frmsqr(iVertex);
				}
				if (ClosestVertexToCursor)
					frmsqr0(FormLines[0]);
				else
					frmx(FormLines[0], StitchWindowMemDC);
				ritnum(STR_NUMPNT, ClosestVertexToCursor);
			}
			else {
				for (iVertex = 1; iVertex < VertexCount; iVertex++)
					frmsqr(iVertex);
				SelectObject(StitchWindowMemDC, FormSelectedPen);
				frmsqr0(FormLines[0]);
			}
			if (chkMap(FPSEL) && ClosestFormToCursor == iForm) {
				sRct2px(SelectedVerticesRect, &SelectedPixelsRect);
				rct2sel(SelectedPixelsRect, SelectedPointsLine);
				setMap(SHOPSEL);
				dupsel(StitchWindowMemDC);
			}
		}
	}
	if (SelectedFormCount) {
		SelectObject(StitchWindowMemDC, MultiFormPen);
		ratsr();
		SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
		SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fselrct(SelectedFormList[iForm]);
		SelectedFormsSize.x = SelectedFormsRect.right - SelectedFormsRect.left;
		SelectedFormsSize.y = SelectedFormsRect.bottom - SelectedFormsRect.top;
		dubig();
	}
	else {
		if (chkMap(FORMSEL))
			ritfrct(ClosestFormToCursor, StitchWindowMemDC);
		if (chkMap(FRMPMOV)) {
			ritmov();
			RubberBandLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
			RubberBandLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
			setMap(SHOMOV);
			ritmov();
		}
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void setpoli() {
	frmclr(&FormList[FormIndex]);
	FormList[FormIndex].type = FRMFPOLY;
	//frmtyp = FRMFPOLY;
	FormList[FormIndex].attribute = ActiveLayer << 1;
	FormList[FormIndex].underlayIndent = IniFile.underlayIndent;
	setMap(SATIN);
	setMap(INIT);
}

void setlin() {
	frmclr(&FormList[FormIndex]);
	FormList[FormIndex].type = FRMLINE;
	//frmtyp = FRMLINE;
	FormList[FormIndex].attribute = ActiveLayer << 1;
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

	TCHAR		buffer[HBUFSIZ];

	switch (umsg) {
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	reinit:;
		sprintf_s(buffer, sizeof(buffer), "%d", IniFile.formSides);
		SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buffer);
		sprintf_s(buffer, sizeof(buffer), "%.3f", IniFile.tearTailLength);
		SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buffer);
		sprintf_s(buffer, sizeof(buffer), "%.3f", IniFile.tearTwistStep / PFGRAN);
		SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buffer);
		sprintf_s(buffer, sizeof(buffer), "%.3f", IniFile.tearTwistRatio);
		SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buffer);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buffer, HBUFSIZ);
			IniFile.formSides = atoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buffer, HBUFSIZ);
			IniFile.tearTailLength = atof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buffer, HBUFSIZ);
			IniFile.tearTwistStep = atof(buffer)*PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buffer, HBUFSIZ);
			IniFile.tearTwistRatio = atof(buffer);
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
	unsigned	nResult, iStep, iVertex, count, iRightVertices, iLeftVertices;
	double		middle;
	double		verticalPosition;
	double		length;
	double		step;
	double		horizontalRatio;
	double		verticalRatio;
	float		twistStep;
	fPOINT		size;

	unmsg();
	nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, (DLGPROC)tearprc);
	if (nResult > 0) {
		twistStep = IniFile.tearTwistStep;
		durpoli(IniFile.formSides);
		fvars(FormIndex);
		count = VertexCount / 4;
		middle = (CurrentFormVertices[1].x - CurrentFormVertices[0].x) / 2 + CurrentFormVertices[0].x;
		length = CurrentFormVertices[count].y - CurrentFormVertices[0].y;
		step = CurrentFormVertices[count + 1].y - CurrentFormVertices[count].y;
		verticalPosition = CurrentFormVertices[count + 1].y;
		iLeftVertices = VertexCount - count;
		iRightVertices = count + 1;
		for (iStep = 0; iStep < (unsigned)count; iStep++) {
			CurrentFormVertices[iRightVertices].y = CurrentFormVertices[iLeftVertices].y = verticalPosition;
			CurrentFormVertices[iRightVertices].x += twistStep;
			CurrentFormVertices[iLeftVertices].x += twistStep;
			twistStep *= IniFile.tearTwistRatio;
			verticalPosition -= step;
			step *= IniFile.tearTailLength;
			iRightVertices--;
			iLeftVertices++;
		}
		CurrentFormVertices[0].y = CurrentFormVertices[1].y = verticalPosition;
		CurrentFormVertices[0].x += twistStep;
		CurrentFormVertices[1].x += twistStep;
		verticalPosition -= step / 2;
		CurrentFormVertices[VertexCount].x = CurrentFormVertices[0].x;
		CurrentFormVertices[VertexCount].y = CurrentFormVertices[0].y;
		if (twistStep)
			CurrentFormVertices[0].x = CurrentFormVertices[1].x + twistStep / 4;
		else
			CurrentFormVertices[0].x = middle;
		CurrentFormVertices[0].y = verticalPosition;
		SelectedForm->vertexCount++;
		NewFormVertexCount++;
		FormVertexIndex++;
		setMap(FORMSEL);
		fvars(FormIndex);
		frmout(FormIndex);
		flipv();
		rstMap(FORMSEL);
		size.x = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		size.y = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		horizontalRatio = UnzoomedRect.x / 4 / size.x;
		if (horizontalRatio > 1)
			horizontalRatio = 1;
		verticalRatio = UnzoomedRect.y / 4 / size.y;
		if (verticalRatio < horizontalRatio)
			horizontalRatio = verticalRatio;
		if (horizontalRatio < 1) {
			for (iVertex = 0; iVertex < VertexCount; iVertex++) {
				CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x)*horizontalRatio + CurrentFormVertices[0].x;
				CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y)*horizontalRatio + CurrentFormVertices[0].y;
			}
		}
		frmout(FormIndex);
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			CurrentFormVertices[iVertex].x -= SelectedForm->rectangle.left;
			CurrentFormVertices[iVertex].y -= SelectedForm->rectangle.bottom;
		}
	}
}

void duform(unsigned formType) {
	frmclr(&FormList[FormIndex]);
	switch (formType + 1) {
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
	unsigned	iForm, iVertex, closestForm, closestVertex, layerCoded, formLayer;
	fPOINT*		vertices;
	fPOINT		point;
	POINT		screenCoordinate;
	double		length, minimumLength = 1e99;

	if (FormIndex) {
		screenCoordinate.x = Msg.pt.x - StitchWindowOrigin.x;
		screenCoordinate.y = Msg.pt.y - StitchWindowOrigin.y;
		rats();
		closestForm = closestVertex = 0;
		px2stchf(screenCoordinate, &point);
		layerCoded = ActiveLayer << 1;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			if (chkMap(FRMSAM) && iForm == ClosestFormToCursor)
				continue;

			formLayer = FormList[iForm].attribute&FRMLMSK;
			if (!ActiveLayer || !formLayer || formLayer == layerCoded) {
				getfinfo(iForm);
				vertices = FormList[iForm].vertices;
				for (iVertex = 0; iVertex < FormInfo.sideCount; iVertex++) {
					length = hypot(point.x - vertices[iVertex].x, point.y - vertices[iVertex].y);
					if (length < minimumLength&&length >= 0) {
						minimumLength = length;
						closestForm = iForm;
						closestVertex = iVertex;
					}
				}
			}
		}
		stch2pxr(FormList[closestForm].vertices[closestVertex]);
		minimumLength = hypot(StitchCoordinatesPixels.x - screenCoordinate.x, StitchCoordinatesPixels.y - screenCoordinate.y);
		if (minimumLength < CLOSENUF) {
			ClosestFormToCursor = closestForm;
			ClosestVertexToCursor = closestVertex;
			fvars(ClosestFormToCursor);
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
	unsigned iPoint, previousPoint;

	fvars(ClosestFormToCursor);
	ratsr();
	if (SelectedForm->type == FRMLINE)
		NewFormVertexCount = SelectedForm->vertexCount;
	else
		NewFormVertexCount = SelectedForm->vertexCount + 1;
	frmlin(SelectedForm->vertices, SelectedForm->vertexCount);
	previousPoint = prv(ClosestVertexToCursor);
	for (iPoint = 0; iPoint < 3; iPoint++)
	{
		RubberBandLine[iPoint].x = FormLines[previousPoint].x;
		RubberBandLine[iPoint].y = FormLines[previousPoint].y;
		previousPoint++;
	}
	ritmov();
}

void makspac(unsigned start, unsigned count) {
	if (!chkmax(PCSHeader.stitchCount, count))
	{
		MoveMemory(&StitchBuffer[start + count], &StitchBuffer[start], sizeof(fPOINTATTR)*(PCSHeader.stitchCount - start));
		PCSHeader.stitchCount += count;
	}
}

BOOL ritlin(fPOINT start, fPOINT finish)
{
	dPOINT		delta, step, point;
	double		length;
	unsigned	iStep, count;

	delta.x = finish.x - start.x;
	delta.y = finish.y - start.y;
	length = hypot(delta.x, delta.y);
	InterleaveSequence[InterleaveSequenceIndex].x = start.x;
	InterleaveSequence[InterleaveSequenceIndex++].y = start.y;
	if (length > MaxStitchLen) {
		count = ceil(length / UserStichLen);
		if (!count)
			count = 1;
		while (length / count > MaxStitchLen)
			count++;
		if (!chkmax(InterleaveSequenceIndex, count)) {
			step.x = delta.x / count;
			step.y = delta.y / count;
			point.x = start.x + step.x;
			point.y = start.y + step.y;
			for (iStep = 0; iStep < count - 1; iStep++) {
				if (InterleaveSequenceIndex&MAXMSK) {
					InterleaveSequenceIndex = MAXSEQ - 2;
					return 0;
				}
				InterleaveSequence[InterleaveSequenceIndex].x = point.x;
				InterleaveSequence[InterleaveSequenceIndex++].y = point.y;
				point.x += step.x;
				point.y += step.y;
			}
		}
		else {
			SequenceIndex = MAXSEQ - 2;
			return 0;
		}
	}
	return 1;
}

unsigned closflt(float xCoordinate, float yCoordinate)
{
	double		length, minimumLength;
	unsigned	iVertex, closestVertex = 0;

	minimumLength = 1e99;
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
	{
		length = hypot(xCoordinate - CurrentFormVertices[iVertex].x, yCoordinate - CurrentFormVertices[iVertex].y);
		if (length < minimumLength)
		{
			closestVertex = iVertex;
			minimumLength = length;
		}
	}
	return closestVertex;
}

void chkseq(BOOL border) {
#if BUGBAK

	unsigned index;

	for (index = 0; index < SequenceIndex; index++)
	{
		InterleaveSequence[index].x = OSequence[index].x;
		InterleaveSequence[index].y = OSequence[index].y;
	}
	InterleaveSequenceIndex = SequenceIndex;
#else

	double		len;
	unsigned	iSequence, destination, savedIndex;
	float		minimumStitchLength;

	savedIndex = InterleaveSequenceIndex;
	if (border) {
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EDGELCHAIN || SelectedForm->edgeType == EDGEOCHAIN)
			UserStichLen = 9 * PFGRAN;
		else
			UserStichLen = SelectedForm->edgeStitchLen;
		minimumStitchLength = SelectedForm->minBorderStitchLen;
	}
	else {
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxFillStitchLen;
		if (isclp(ClosestFormToCursor))
			UserStichLen = MaxStitchLen;
		else
			UserStichLen = SelectedForm->lengthOrCount.stitchLength;
		minimumStitchLength = SelectedForm->minFillStitchLen;
	}
	if (UserStichLen > MaxStitchLen)
		UserStichLen = MaxStitchLen;
	for (iSequence = 0; iSequence < SequenceIndex - 1; iSequence++)
	{
		if (!ritlin(OSequence[iSequence], OSequence[iSequence + 1]))
			goto seqskp;
	}
	InterleaveSequence[InterleaveSequenceIndex].x = OSequence[iSequence].x;
	InterleaveSequence[InterleaveSequenceIndex++].y = OSequence[iSequence].y;
seqskp:;
	if (!minimumStitchLength)
		return;
	destination = savedIndex + 1;
	for (iSequence = savedIndex + 1; iSequence < InterleaveSequenceIndex; iSequence++) {
		len = hypot(InterleaveSequence[iSequence].x - InterleaveSequence[iSequence - 1].x, InterleaveSequence[iSequence].y - InterleaveSequence[iSequence - 1].y);
		if (len > minimumStitchLength) {
			InterleaveSequence[destination].x = InterleaveSequence[iSequence].x;
			InterleaveSequence[destination].y = InterleaveSequence[iSequence].y;
			destination++;
		}
	}
	InterleaveSequenceIndex = destination;
#endif
}

void ritbrd() {
	if (SequenceIndex)
	{
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_BRD;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPBRD;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->borderColor&COLMSK;
		chkseq(true);
		InterleaveSequenceIndex2++;
	}
}

void ritapbrd() {
	if (SequenceIndex)
	{
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_AP;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPMSK;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->borderColor >> 4;
		chkseq(true);
		InterleaveSequenceIndex2++;
	}
}

void ritfil() {
	if (SequenceIndex)
	{
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPFRM;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->fillColor;
		chkseq(false);
		InterleaveSequenceIndex2++;
	}
}

void okcan() {
	GetClientRect(MsgWindow, &MsgRect);

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
		ThrEdInstance,
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
		ThrEdInstance,
		NULL);
}

void savdisc() {
	TCHAR	buffer[HBUFSIZ];

	sdmsg();
	rstMap(BIGBOX);

	GetClientRect(MsgWindow, &MsgRect);

	LoadString(ThrEdInstance, IDS_SAV, buffer, HBUFSIZ);
	OKButton = CreateWindow(
		"STATIC",
		buffer,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		5,
		MsgRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThrEdInstance,
		NULL);

	LoadString(ThrEdInstance, IDS_DISC, buffer, HBUFSIZ);
	DiscardButton = CreateWindow(
		"STATIC",
		buffer,
		SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3 + 15,
		MsgRect.bottom + 15,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThrEdInstance,
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
		ThrEdInstance,
		NULL);
}

BOOL lastch() {
	if (InterleaveSequenceIndex)
	{
		LastPoint.x = InterleaveSequence[InterleaveSequenceIndex - 1].x;
		LastPoint.y = InterleaveSequence[InterleaveSequenceIndex - 1].y;
		return 1;
	}
	else
		return 0;
}

unsigned getlast() {
	unsigned		iVertex;
	unsigned		closestVertex = 0;
	double			length, dx, dy, minimumLength = 1e99;

	if (SelectedForm->fillType) {
		lastch();
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			// ToDo - does this have to be a double or would a fPOINT work?
			dx = LastPoint.x - CurrentFormVertices[iVertex].x;
			dy = LastPoint.y - CurrentFormVertices[iVertex].y;
			length = hypot(dx, dy);
			if (length < minimumLength) {
				minimumLength = length;
				closestVertex = iVertex;
			}
		}
		return closestVertex;
	}
	else
		return 0;
}

void flt2dub(fPOINT inPoint, dPOINT* outPoint) {
	outPoint->x = inPoint.x;
	outPoint->y = inPoint.y;
}

void linrutb(unsigned start) {
	double		spacing;
	unsigned	iVertex;
	dPOINT		point;

	spacing = StitchSpacing;
	SelectedPoint.x = CurrentFormVertices[start].x;
	SelectedPoint.y = CurrentFormVertices[start].y;
	for (iVertex = start + 1; iVertex < VertexCount; iVertex++) {
		flt2dub(CurrentFormVertices[iVertex], &point);
		filinsb(point);
	}
	flt2dub(CurrentFormVertices[0], &point);
	filinsb(point);
	StitchSpacing = spacing;
}

void oclp(fPOINT* clip, unsigned clipEntries) {
	unsigned	iClip;

	if (!chkMap(NOCLP))
	{
		for (iClip = 0; iClip < clipEntries; iClip++) {
			ClipBuffer[iClip].x = clip[iClip].x;
			ClipBuffer[iClip].y = clip[iClip].y;
		}
		ClipRect.left = ClipRect.right = ClipBuffer[0].x;
		ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
		for (iClip = 1; iClip < (unsigned)clipEntries; iClip++) {
			if (ClipBuffer[iClip].x < ClipRect.left)
				ClipRect.left = ClipBuffer[iClip].x;
			if (ClipBuffer[iClip].x > ClipRect.right)
				ClipRect.right = ClipBuffer[iClip].x;
			if (ClipBuffer[iClip].y < ClipRect.bottom)
				ClipRect.bottom = ClipBuffer[iClip].y;
			if (ClipBuffer[iClip].y > ClipRect.top)
				ClipRect.top = ClipBuffer[iClip].y;
		}
		ClipRectSize.cx = ClipRect.right - ClipRect.left;
		ClipRectSize.cy = ClipRect.top - ClipRect.bottom;
		ClipStitchCount = clipEntries;
	}
}

float getblen() {
// ToDo - there must be a better way to do this.
//        savblen is the complementary function

#if	 __UseASM__
	float		fLength;
	unsigned	iLength;

	iLength = (FormList[ClosestFormToCursor].clipEntries << 16) | FormList[ClosestFormToCursor].picoLength;

	_asm {
		mov		eax, iLength
		mov		fLength, eax
	}
	return fLength;
#else
	union {
		float		fLength;
		unsigned	iLength;
	} x;

	x.iLength = (FormList[ClosestFormToCursor].clipEntries << 16) | FormList[ClosestFormToCursor].picoLength;
	return x.fLength;
#endif
}

void savblen(float fLength) {
#if	 __UseASM__
	unsigned short clipEntries;
	unsigned short picoLength;

	_asm {
		mov		eax, fLength
		mov		picoLength, ax
		shr		eax, 16
		mov		clipEntries, ax
	}
	FormList[ClosestFormToCursor].clipEntries = clipEntries;
	FormList[ClosestFormToCursor].picoLength = picoLength;
#else
	union {
		unsigned int i;
		float f;
	} x;

	x.f = fLength;

	FormList[ClosestFormToCursor].clipEntries = x.i >> 16;
	FormList[ClosestFormToCursor].picoLength = x.i & 0xFFFF;
#endif
}

float getplen() {
	unsigned value = SelectedForm->picoLength;

	return((float)(value >> 8) + (value & 0xff) / 256);
}

void savplen(float length) {
	unsigned	num, fr;
	double		integerPart, fractionalPart;

	if (length > 255)
		length = 255;
	fractionalPart = modf(length, &integerPart);
	fr = (unsigned)floor(fractionalPart * 256);
	num = integerPart;
	FormList[ClosestFormToCursor].picoLength = (unsigned short)(num << 8) | fr;
}

void chkbrd() {
	float	length;

	fvars(ClosestFormToCursor);
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

			HorizontalLength2 = SelectedForm->borderSize;
			sbrd();
			break;

		case EDGEPROPSAT: //Proportional Satin

			HorizontalLength2 = SelectedForm->borderSize;
			pbrd(SelectedForm->edgeSpacing);
			break;

		case EDGEAPPL: // Applique

			SequenceIndex = 0;
			apbrd();
			ritapbrd();
			HorizontalLength2 = SelectedForm->borderSize;
			sbrd();
			break;

		case EDGEBHOL: // BH Buttonhole

			HorizontalLength2 = SelectedForm->borderSize * 2;
			length = ButtonholeCornerLength;
			ButtonholeCornerLength = getblen();
			satout(20);
			bhbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = length;
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

void boldlin(unsigned start, unsigned finish, double size) {
	dPOINT		delta, step, point0, point1;
	double		length;
	unsigned	count;

	delta.x = CurrentFormVertices[finish].x - CurrentFormVertices[start].x;
	delta.y = CurrentFormVertices[finish].y - CurrentFormVertices[start].y;
	length = hypot(delta.x, delta.y);
	count = length / size;
	if (count) {
		step.x = delta.x / count;
		step.y = delta.y / count;
		point0.x = CurrentFormVertices[start].x;
		point0.y = CurrentFormVertices[start].y;
		point1.x = point0.x + step.x;
		point1.y = point0.y + step.y;
		while (count) {
			OSequence[SequenceIndex].x = point1.x;
			OSequence[SequenceIndex++].y = point1.y;
			OSequence[SequenceIndex].x = point0.x;
			OSequence[SequenceIndex++].y = point0.y;
			OSequence[SequenceIndex].x = point1.x;
			OSequence[SequenceIndex++].y = point1.y;
			point0.x += step.x;
			point0.y += step.y;
			point1.x += step.x;
			point1.y += step.y;
			count--;
		}
		OSequence[SequenceIndex].x = CurrentFormVertices[finish].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[finish].y;
	}
	else {
		OSequence[SequenceIndex].x = CurrentFormVertices[finish].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[finish].y;
		OSequence[SequenceIndex].x = CurrentFormVertices[start].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[start].y;
		OSequence[SequenceIndex].x = CurrentFormVertices[finish].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[finish].y;
	}
}

void bold(double size) {
	unsigned		iVertex, iSequence, iOutput = 0;
	unsigned short	iNextLine, iLine;
	double			length;

	iNextLine = iLine = getlast();
	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[iLine].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[iLine].y;
	for (iVertex = 0; iVertex < (unsigned)VertexCount - 1; iVertex++) {
		iNextLine = nxt(iLine);
		boldlin(iLine, iNextLine, size);
		iLine = iNextLine;
	}
	if (SelectedForm->type != FRMLINE) {
		iNextLine = nxt(iLine);
		boldlin(iLine, iNextLine, size);
	}
	for (iSequence = 0; iSequence < SequenceIndex - 1; iSequence++) {
		length = hypot(OSequence[iSequence + 1].x - OSequence[iSequence].x, OSequence[iSequence + 1].y - OSequence[iSequence].y);
		if (length > TINY) {
			OSequence[iOutput].x = OSequence[iSequence].x;
			OSequence[iOutput++].y = OSequence[iSequence].y;
		}
	}
	OSequence[iOutput].x = CurrentFormVertices[iNextLine].x;
	OSequence[iOutput++].y = CurrentFormVertices[iNextLine].y;
	SequenceIndex = iOutput;
}

void refilfn() {
	double		spacing;
	double		stitchLength = UserStitchLength;
	float		length;

	rstMap(TXFIL);
	fvars(ClosestFormToCursor);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->underlayIndent = 0;
	savdo();
	fdelstch();
	setMap(WASREFIL);
	if (SelectedForm->fillSpacing < 0.5 && !isclp(ClosestFormToCursor))
		SelectedForm->fillSpacing = 0.5;
	if (SelectedForm->edgeSpacing < 0.5)
		SelectedForm->edgeSpacing = 0.5;
	UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
	if (!chkMap(WASDO))
		savdo();
	rstMap(WASDO);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK) && SelectedForm->type == FRMLINE&&SelectedForm->fillType != CONTF)
		SelectedForm->type = FRMFPOLY;
	InterleaveSequenceIndex = InterleaveSequenceIndex2 = 0;
	rstMap(ISUND);
	switch (SelectedForm->type) {
	case FRMLINE:

		HorizontalLength2 = SelectedForm->borderSize;
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
			SequenceIndex = 0;
			clpbrd(0);
			ritbrd();
			break;

		case EDGEANGSAT:

			rstMap(SAT1);
			slbrd();
			ritbrd();
			break;

		case EDGEPROPSAT:

			if (SelectedForm->vertexCount > 2) {
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

			HorizontalLength2 = SelectedForm->borderSize * 2;
			length = ButtonholeCornerLength;
			ButtonholeCornerLength = getblen();
			satout(20);
			blbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = length;
			ritbrd();
			break;

		case EDGEPICOT:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			SequenceIndex = 0;
			length = ButtonholeCornerLength;
			ButtonholeCornerLength = getplen();
			clpic(0);
			ButtonholeCornerLength = length;
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
			spacing = StitchSpacing;
			StitchSpacing = SelectedForm->fillSpacing;
			switch ((unsigned)SelectedForm->fillType) {
			case VRTF:

				fnvrt();
				WorkingFormVertices = SelectedForm->vertices;
				break;

			case HORF:

				fnhor();
				WorkingFormVertices = AngledForm.vertices;
				break;

			case ANGF:

				RotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
				filang();
				WorkingFormVertices = AngledForm.vertices;
				break;

			case VCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				setangf(0);
				fvars(ClosestFormToCursor);
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
			StitchSpacing = spacing;
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

			spacing = StitchSpacing;
			StitchSpacing = SelectedForm->fillSpacing;
			UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
			satfil();
			StitchSpacing = spacing;
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
	UserStitchLength = stitchLength;
	intlv();
	ritot(PCSHeader.stitchCount);
	setfchk();
}

void refil() {
	unsigned trg, iStitch, attribute;

	if (!chku(WRNOF))
	{
		// ToDo - rename trg when I figure it out :)
		trg = (ClosestFormToCursor << FRMSHFT) | USMSK;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			attribute = StitchBuffer[iStitch].attribute;
			if (!(attribute&NOTFRM) && (attribute&(USMSK | FRMSK)) == trg) {
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
	POINT		screenCoordinate;
	fPOINT*		vertices;

	unfrm();
	fvars(ClosestFormToCursor);
	vertices = &SelectedForm->vertices[0];
	screenCoordinate.x = Msg.pt.x - StitchWindowOrigin.x;
	screenCoordinate.y = Msg.pt.y - StitchWindowOrigin.y;
	rats();
	px2stchf(screenCoordinate, &vertices[ClosestVertexToCursor]);
	frmout(ClosestFormToCursor);
	refil();
	setMap(WASFPNT);
	rstMap(SELBOX);
	setMap(FRMPSEL);
	ritfcor(&CurrentFormVertices[ClosestVertexToCursor]);
	setMap(RESTCH);
}

unsigned short nxt(unsigned short iVertex) {
	iVertex++;
	if (iVertex > (unsigned)VertexCount - 1)
		iVertex = 0;
	return iVertex;
}

unsigned short prv(unsigned iVertex) {
	if (iVertex)
		iVertex--;
	else
		iVertex = VertexCount - 1;
	return iVertex;
}

unsigned proj(dPOINT point, double slope, fPOINT point0, fPOINT point1, dPOINT* adjustedPoint) {
	dPOINT	delta;
	double	slopl, con, conl, xMinimum, xMaximum, yMinimum, yMaximum, swap;

	// ToDo - rename slop1, con & conl
	delta.x = point1.x - point0.x;
	delta.y = point1.y - point0.y;
	if (delta.x) {
		slopl = delta.y / delta.x;
		conl = point0.y - slopl*point0.x;
		con = point.y - slope*point.x;
		adjustedPoint->x = (conl - con) / (slope - slopl);
		adjustedPoint->y = adjustedPoint->x*slope + con;
	}
	else {
		adjustedPoint->x = point0.x;
		con = point.y - slope*point.x;
		adjustedPoint->y = adjustedPoint->x*slope + con;
	}
	xMinimum = point0.x;
	xMaximum = point1.x;
	if (xMinimum > xMaximum) {
		swap = xMinimum;
		xMinimum = xMaximum;
		xMaximum = swap;
	}
	if (delta.y) {
		yMinimum = point0.y;
		yMaximum = point1.y;
		if (yMinimum > yMaximum) {
			swap = yMinimum;
			yMinimum = yMaximum;
			yMaximum = swap;
		}
		if (adjustedPoint->x<xMinimum || adjustedPoint->x>xMaximum || adjustedPoint->y<yMinimum || adjustedPoint->y>yMaximum)
			return 0;
		else
			return 1;
	}
	else {
		if (adjustedPoint->x<xMinimum || adjustedPoint->x>xMaximum)
			return 0;
		else
			return 1;
	}
}

unsigned projv(double xCoordinate, fPOINT lowerPoint, fPOINT upperPoint, dPOINT* intersect) {
	double swap, slope, deltaX;

	intersect->x = xCoordinate;
	deltaX = upperPoint.x - lowerPoint.x;
	if (deltaX) {
		slope = (upperPoint.y - lowerPoint.y) / deltaX;
		intersect->y = (xCoordinate - lowerPoint.x)*slope + lowerPoint.y;
		if (lowerPoint.x > upperPoint.x) {
			swap = lowerPoint.x;
			lowerPoint.x = upperPoint.x;
			upperPoint.x = swap;
		}
		if (xCoordinate<lowerPoint.x || xCoordinate>upperPoint.x)
			return 0;
		else
			return 1;
	}
	else
		return 0;
}

unsigned projh(double yCoordinate, fPOINT point0, fPOINT point1, dPOINT* adjustedCoordinate) {
	double swap, slope, deltaX, deltaY;

	adjustedCoordinate->y = yCoordinate;
	deltaX = point1.x - point0.x;
	if (deltaX) {
		deltaY = point1.y - point0.y;
		if (deltaY) {
			slope = deltaY / deltaX;
			adjustedCoordinate->x = (yCoordinate - point0.y) / slope + point0.x;
		}
		else
			return 0;
	}
	else
		adjustedCoordinate->x = point0.x;
	if (point0.y > point1.y) {
		swap = point0.y;
		point0.y = point1.y;
		point1.y = swap;
	}
	if (yCoordinate<point0.y || yCoordinate>point1.y)
		return 0;
	else
		return 1;
}

void filang() {
	unsigned	iVertex;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex].x = SelectedForm->vertices[iVertex].x;
		AngledForm.vertices[iVertex].y = SelectedForm->vertices[iVertex].y;
		rotflt(&AngledForm.vertices[iVertex]);
	}
	SelectedForm = &AngledForm;
	fnvrt();
	SelectedForm = &FormList[ClosestFormToCursor];
}

void makpoli() {
	if (SelectedForm->type == SAT) {
		if (SelectedForm->satinGuideCount)
			delsac(ClosestFormToCursor);
	}
	SelectedForm->type = FRMFPOLY;
}

void filinu(float xCoordinate, float yCoordinate) {
	unsigned	count;
	double		length;
	dPOINT		point, step, delta;

	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	delta.x = xCoordinate - SelectedPoint.x;
	delta.y = yCoordinate - SelectedPoint.y;
	length = hypot(delta.x, delta.y);
	count = length / UserStitchLength;
	if (chkmax(count, SequenceIndex) || count + SequenceIndex > MAXSEQ - 3)
		return;
	if (count) {
		if (chkMap(FILDIR))
			count++;
		step.x = delta.x / count;
		step.y = delta.y / count;
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence[SequenceIndex].x = point.x;
			OSequence[SequenceIndex++].y = point.y;
			count--;
		}
	}
	else {
		OSequence[SequenceIndex].x = xCoordinate;
		OSequence[SequenceIndex++].y = yCoordinate;
	}
	SelectedPoint.x = xCoordinate;
	SelectedPoint.y = yCoordinate;
}

void filin(dPOINT currentPoint) {
	dPOINT delta, step, point;
	double length;
	int count;

	delta.x = currentPoint.x - SelectedPoint.x;
	delta.y = currentPoint.y - SelectedPoint.y;
	length = hypot(delta.x, delta.y);
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	count = length / UserStitchLength;
	if (chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXSEQ - 3)
		return;
	if (count) {
		if (chkMap(FILDIR))
			count++;
		step.x = delta.x / count;
		step.y = delta.y / count;
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence[SequenceIndex].x = point.x;
			OSequence[SequenceIndex++].y = point.y;
			count--;
		}
	}
	else {
		OSequence[SequenceIndex].x = currentPoint.x;
		OSequence[SequenceIndex++].y = currentPoint.y;
	}
	SelectedPoint.x = currentPoint.x;
	SelectedPoint.y = currentPoint.y;
}

unsigned short isclos(SMALPNTL* lineEndPoint0, SMALPNTL* lineEndPoint1) {
	float		low0, high0, low1, high1;

	high0 = lineEndPoint0[1].y + GapToClosestRegion;
	low0 = lineEndPoint0[0].y - GapToClosestRegion;
	high1 = lineEndPoint1[1].y + GapToClosestRegion;
	low1 = lineEndPoint1[0].y - GapToClosestRegion;
	if (high0 < low1)
		return 0;
	if (high1 < low0)
		return 0;
	return 1;
}

BOOL lnclos(unsigned group0, unsigned line0, unsigned group1, unsigned line1) {
	unsigned	index0, index1, count0, count1;
	SMALPNTL*	lineEndPoint0;
	SMALPNTL*	lineEndPoint1;

	if (group1 > GroupIndexCount - 2)
		return 0;
	if (!group0)
		return 0;
	count0 = (GroupIndexSequence[group0 + 1] - GroupIndexSequence[group0]) >> 1;
	index0 = 0;
	lineEndPoint0 = &LineEndpoints[GroupIndexSequence[group0]];
	while (count0&&lineEndPoint0[index0].line != line0) {
		count0--;
		index0 += 2;
	}
	if (count0) {
		count1 = (GroupIndexSequence[group1 + 1] - GroupIndexSequence[group1]) >> 1;
		index1 = 0;
		lineEndPoint1 = &LineEndpoints[GroupIndexSequence[group1]];
		while (count1&&lineEndPoint1[index1].line != line1) {
			count1--;
			index1 += 2;
		}
		if (count1) {
			if (isclos(&lineEndPoint0[index0], &lineEndPoint1[index1]))
				return 1;
			else
				return 0;
		}
	}
	return 0;
}

// ToDo - should the return type be BOOL?
unsigned short regclos(unsigned iRegion0, unsigned iRegion1) {
	SMALPNTL*	lineEndPoint0Start;
	SMALPNTL*	lineEndPoint0End;
	SMALPNTL*	lineEndPoint1Start;
	SMALPNTL*	lineEndPoint1End;
	unsigned	group0Start;
	unsigned	group0End;
	unsigned	group1Start;
	unsigned	group1End;
	unsigned	groupStart;
	unsigned	groupEnd;
	unsigned	lineStart, lineEnd;
	unsigned	prlin, polin;

	//ToDo - More renaming required
	lineEndPoint0Start = &*SortedLines[RegionsList[iRegion0].start];
	lineEndPoint1Start = &*SortedLines[RegionsList[iRegion1].start];
	group1Start = lineEndPoint1Start->group;
	group0Start = lineEndPoint0Start->group;
	if (group0Start > group1Start) {
		groupStart = group0Start;
		lineStart = lineEndPoint0Start->line;
		prlin = lineEndPoint1Start->line;
	}
	else {
		groupStart = group1Start;
		lineStart = lineEndPoint1Start->line;
		prlin = lineEndPoint0Start->line;
	}
	if (groupStart&&lnclos(groupStart - 1, prlin, groupStart, lineStart)) {
		NextGroup = groupStart;
		return 1;
	}
	else {
		lineEndPoint0End = &*SortedLines[RegionsList[iRegion0].end];
		lineEndPoint1End = &*SortedLines[RegionsList[iRegion1].end];
		group1End = lineEndPoint1End->group;
		group0End = lineEndPoint0End->group;
		if (group0End < group1End) {
			groupEnd = group0End;
			lineEnd = lineEndPoint0End->line;
			polin = lineEndPoint1End->line;
		}
		else {
			groupEnd = group1End;
			lineEnd = lineEndPoint1End->line;
			polin = lineEndPoint0End->line;
		}
		if (lnclos(groupEnd, lineEnd, groupEnd + 1, polin)) {
			NextGroup = groupEnd;
			return 1;
		}
	}
	if (((group0Start > group1Start) ? (group0Start - group1Start) : (group1Start - group0Start)) < 2) {
		if (isclos(lineEndPoint0Start, lineEndPoint1Start)) {
			NextGroup = group0Start;
			return 1;
		}
	}
	if (((group0Start > group1End) ? (group0Start - group1End) : (group1End - group0Start)) < 2) {
		if (isclos(lineEndPoint0Start, lineEndPoint1End)) {
			NextGroup = group0Start;
			return 1;
		}
	}
	if (((group0End > group1Start) ? (group0End - group1Start) : (group1Start - group0End)) < 2) {
		if (isclos(lineEndPoint0End, lineEndPoint1Start)) {
			NextGroup = group0End;
			return 1;
		}
	}
	if (((group0End > group1End) ? (group0End - group1End) : (group1End - group0End)) < 2) {
		if (isclos(lineEndPoint0End, lineEndPoint1End)) {
			NextGroup = group0End;
			return 1;
		}
	}
	return 0;
}

BOOL unvis() {
	for (VisitedIndex = 0; VisitedIndex < RegionCount; VisitedIndex++) {
		if (!VisitedRegions[VisitedIndex])
			return 1;
	}
	return 0;
}

unsigned setseq(unsigned bit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, SequenceFlagBitmap
		mov		ecx, bit
		bts[ebx], ecx
		jnc		short setseqx
		inc		eax
		setseqx :
	}
#else
	return _bittestandset((long *)SequenceFlagBitmap, bit);
#endif
}

void rspnt(float xCoordinate, float yCoordinate) {
	BSequence[OutputIndex].x = xCoordinate;
	BSequence[OutputIndex].y = yCoordinate;
	BSequence[OutputIndex++].attribute = 0;
}

void dunseq(unsigned start, unsigned finish) {
	SMALPNTL*	lineEndPoint0;
	SMALPNTL*	lineEndPoint1;
	unsigned	iLine;
	double		deltaY, minimumY;

	minimumY = 1e30;
	for (iLine = start; iLine <= finish; iLine++) {
		lineEndPoint0 = &*SortedLines[iLine];
		deltaY = lineEndPoint0[1].y - lineEndPoint0->y;
		if (deltaY < minimumY)
			minimumY = deltaY;
	}
	minimumY /= 2;
	lineEndPoint0 = &*SortedLines[start];
	lineEndPoint1 = &*SortedLines[finish];
	if (minimumY == 1e30 / 2)
		minimumY = 0;
	rspnt(lineEndPoint0->x, lineEndPoint0->y + minimumY);
	rspnt(lineEndPoint1->x, lineEndPoint1->y + minimumY);
	LastGroup = lineEndPoint1->group;
}

void movseq(unsigned ind) {
	SMALPNTL* lineEndPoint;

	lineEndPoint = &*SortedLines[ind];
	BSequence[OutputIndex].attribute = SEQBOT;
	BSequence[OutputIndex].x = lineEndPoint->x;
	BSequence[OutputIndex].y = lineEndPoint->y;
	OutputIndex++;
	lineEndPoint++;
	BSequence[OutputIndex].attribute = SEQTOP;
	BSequence[OutputIndex].x = lineEndPoint->x;
	BSequence[OutputIndex].y = lineEndPoint->y;
	OutputIndex++;
}

void duseq2(unsigned iLine) {
	SequenceLines = &*SortedLines[iLine];
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x, (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq1() {
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x, (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq(unsigned start, unsigned finish) {
	unsigned	iLine, topbak;

	//ToDo - More renaming required
	SequenceLines = nullptr;
	rstMap(SEQDUN);
	topbak = SortedLines[start][1].line;
	if (start > finish) {
		for (iLine = start; (int)iLine >= (int)finish; iLine--) {
			if (setseq(iLine)) {
				if (!setMap(SEQDUN))
					duseq2(iLine);
				else {
					if (topbak != SortedLines[iLine][1].line) {
						if (iLine)
							duseq2(iLine + 1);
						duseq2(iLine);
						topbak = SequenceLines[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN))
					duseq2(iLine + 1);
				SequenceLines = &*SortedLines[iLine];
				movseq(iLine);
			}
		}
		if (rstMap(SEQDUN))
			duseq2(iLine + 1);
		if (SequenceLines != nullptr) { LastGroup = SequenceLines->group; }
	}
	else {
		for (iLine = start; iLine <= finish; iLine++) {
			if (setseq(iLine)) {
				if (!setMap(SEQDUN))
					duseq2(iLine);
				else {
					if (topbak != SortedLines[iLine][1].line) {
						if (iLine)
							duseq2(iLine - 1);
						duseq2(iLine);
						topbak = SequenceLines[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN)) {
					if (iLine)
						duseq2(iLine - 1);
				}
				SequenceLines = &*SortedLines[iLine];
				movseq(iLine);
			}
		}
		if (rstMap(SEQDUN)) {
			if (iLine)
				duseq2(iLine - 1);
		}
		if (SequenceLines != nullptr) { LastGroup = SequenceLines->group; }
	}
}

void brkseq(unsigned start, unsigned finish) {
	//SMALPNTL* line=0;
	unsigned	iLine, bgrp = 0;

	//ToDo - More renaming required
	rstMap(SEQDUN);
	if (start > finish) {
		bgrp = SortedLines[start]->group + 1;
		for (iLine = start; (int)iLine >= (int)finish; iLine--) {
			bgrp--;
			if (SortedLines[iLine]->group != bgrp) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = &*SortedLines[iLine];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				bgrp = SequenceLines[0].group;
			}
			else
				SequenceLines = &*SortedLines[iLine];
			if (setseq(iLine)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(iLine);
		}
		LastGroup = SequenceLines->group;
	}
	else {
		bgrp = SortedLines[start]->group - 1;
		for (iLine = start; iLine <= finish; iLine++) {
			bgrp++;
			if (SortedLines[iLine]->group != bgrp) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = &*SortedLines[iLine];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				bgrp = SequenceLines[0].group;
			}
			else
				SequenceLines = &*SortedLines[iLine];
			if (setseq(iLine)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(iLine);
		}
		LastGroup = SequenceLines->group;
	}
	if (rstMap(SEQDUN))
		duseq1();
}

void brkdun(unsigned start, unsigned finish) {
	rspnt(SortedLines[start]->x, SortedLines[start]->y);
	rspnt(SortedLines[finish]->x, SortedLines[finish]->y);
	rspnt(WorkingFormVertices[SortedLines[start]->line].x, WorkingFormVertices[SortedLines[start]->line].y);
	setMap(BRKFIX);
}

void durgn(unsigned pthi) {
	unsigned	dun, gdif, mindif = 0, iVertex, ind, fdif, bdif;
	unsigned	seql, seqn;
	unsigned	sequenceStart, sequenceEnd;
	unsigned	nextGroup, groupStart, groupEnd;
	unsigned	rgind;
	SMALPNTL*	lineEndPointStart;
	SMALPNTL*	lineEndPointEnd;
	double		length, minimumLength;
	BSEQPNT*	bpnt;

	//ToDo - More renaming required
	rgind = SequencePath[pthi].node;
	CurrentRegion = &RegionsList[rgind];
	nextGroup = SequencePath[pthi].nextGroup;
	sequenceStart = CurrentRegion->start;
	sequenceEnd = CurrentRegion->end;
	if (SequencePath[pthi].skp || rstMap(BRKFIX)) {
		if (BSequence[OutputIndex - 1].attribute != SEQBOT)
			rspnt(BSequence[OutputIndex - 2].x, BSequence[OutputIndex - 2].y);
		lineEndPointStart = &*SortedLines[rgind];
		dun = SortedLines[sequenceStart]->line;
		bpnt = &BSequence[OutputIndex - 1];
		minimumLength = 1e99;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			length = hypot(bpnt->x - WorkingFormVertices[iVertex].x, bpnt->y - WorkingFormVertices[iVertex].y);
			if (length < minimumLength) {
				minimumLength = length;
				mindif = iVertex;
			}
		}
		if (minimumLength)
			rspnt(WorkingFormVertices[mindif].x, WorkingFormVertices[mindif].y);
		fdif = (VertexCount + dun - mindif) % VertexCount;
		bdif = (VertexCount - dun + mindif) % VertexCount;
		if (fdif < bdif) {
			ind = nxt(mindif);
			while (ind != dun) {
				rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
				ind = nxt(ind);
			}
			rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
		}
		else {
			ind = prv(mindif);
			while (ind != dun) {
				rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
				ind = prv(ind);
			}
			rspnt(WorkingFormVertices[ind].x, WorkingFormVertices[ind].y);
		}
	}
	if (VisitedRegions[rgind])
		dun = 1;
	else {
		dun = 0;
		VisitedRegions[rgind]++;
	}
	lineEndPointStart = &*SortedLines[CurrentRegion->start];
	lineEndPointEnd = &*SortedLines[CurrentRegion->end];
	groupStart = lineEndPointStart->group;
	groupEnd = lineEndPointEnd->group;
	if (groupEnd != groupStart)
		seql = (double)(LastGroup - groupStart) / (groupEnd - groupStart)*(sequenceEnd - sequenceStart) + sequenceStart;
	else
		seql = 0;
	if (seql > SortedLineIndex)
		seql = 0;
	length = (double)(groupEnd - groupStart)*(sequenceEnd - sequenceStart);
	if (length)
		seqn = (double)(nextGroup - groupStart) / length + sequenceStart;
	else
		seqn = sequenceEnd;
	if (seql < sequenceStart)
		seql = sequenceStart;
	if (seql > sequenceEnd)
		seql = sequenceEnd;
	if (seqn < sequenceStart)
		seqn = sequenceStart;
	if (seqn > sequenceEnd)
		seqn = sequenceEnd;
	if (SortedLines[seql]->group != LastGroup) {
		if (seql < sequenceEnd&&SortedLines[seql + 1]->group == LastGroup)
			seql++;
		else {
			if (seql > sequenceStart&&SortedLines[seql - 1]->group == LastGroup)
				seql--;
			else {
				mindif = 0xffffffff;
				for (ind = sequenceStart; ind <= sequenceEnd; ind++) {
					gdif = ((SortedLines[ind]->group > LastGroup) ? (SortedLines[ind]->group - LastGroup) : (LastGroup - SortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seql = ind;
					}
				}
			}
		}
	}
	if (SortedLines[seqn]->group != nextGroup) {
		if (seqn < sequenceEnd&&SortedLines[seqn + 1]->group == nextGroup)
			seqn++;
		else {
			if (seqn > sequenceStart&&SortedLines[seqn - 1]->group == nextGroup)
				seqn--;
			else {
				mindif = 0xffffffff;
				for (ind = sequenceStart; ind <= sequenceEnd; ind++) {
					gdif = ((SortedLines[ind]->group > nextGroup) ? (SortedLines[ind]->group - nextGroup) : (nextGroup - SortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seqn = ind;
					}
				}
			}
		}
	}
	if (CurrentRegion->breakCount) {
		if (dun) {
			brkdun(seql, seqn);
		}
		else {
			if (LastGroup >= groupEnd) {
				brkseq(sequenceEnd, sequenceStart);
				if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn)
					brkseq(sequenceStart, seqn);
			}
			else {
				if (groupStart <= nextGroup) {
					if (seql != sequenceStart)
						brkseq(seql, sequenceStart);
					brkseq(sequenceStart, sequenceEnd);
					if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn)
						brkseq(sequenceEnd, seqn);
				}
				else {
					if (seql != sequenceEnd)
						brkseq(seql, sequenceEnd);
					brkseq(sequenceEnd, sequenceStart);
					if (pthi < SequencePathIndex - 1 && sequenceStart != seqn)
						brkseq(sequenceStart, seqn);
				}
			}
		}
	}
	else {
		if (dun)
			dunseq(seql, seqn);
		else {
			if (LastGroup >= groupEnd) {
				duseq(sequenceEnd, sequenceStart);
				duseq(sequenceStart, seqn);
			}
			else {
				if (groupStart <= nextGroup) {
					if (seql != sequenceStart)
						duseq(seql, sequenceStart);
					duseq(sequenceStart, sequenceEnd);
					if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn)
						duseq(sequenceEnd, seqn);
				}
				else {
					if (seql != sequenceEnd)
						duseq(seql, sequenceEnd);
					duseq(sequenceEnd, sequenceStart);
					if (pthi < SequencePathIndex - 1 && sequenceStart != seqn)
						duseq(sequenceStart, seqn);
				}
			}
		}
	}
}

unsigned notdun(unsigned level) {
	unsigned	ind;
	int			tpiv, pivot = level - 1;

	//ToDo - More renaming required
	RegionPath = &TempPath[SequencePathIndex];
	RegionPath[0].pcon = MapIndexSequence[DoneRegion];
	RegionPath[0].count = MapIndexSequence[DoneRegion + 1] - RegionPath[0].pcon;
	for (ind = 1; ind < level; ind++) {
		RegionPath[ind].pcon = MapIndexSequence[PathMap[RegionPath[ind - 1].pcon].node];
		RegionPath[ind].count = MapIndexSequence[PathMap[RegionPath[ind - 1].pcon].node + 1] - RegionPath[ind].pcon;
	}
	while (VisitedRegions[PathMap[RegionPath[pivot].pcon].node] && pivot >= 0) {
		if (--RegionPath[pivot].count > 0)
			RegionPath[pivot].pcon++;
		else {
			tpiv = pivot;
			do {
				tpiv--;
				if (tpiv < 0)
					return 1;
				RegionPath[tpiv].count--;
				RegionPath[tpiv].pcon++;
			} while (!RegionPath[tpiv].count);
			if (tpiv < 0)
				return 1;
			tpiv++;
			while (tpiv <= pivot) {
				if (tpiv) {
					RegionPath[tpiv].pcon = MapIndexSequence[PathMap[RegionPath[tpiv - 1].pcon].node];
					RegionPath[tpiv].count = MapIndexSequence[PathMap[RegionPath[tpiv - 1].pcon].node + 1] - RegionPath[tpiv].pcon;
				}
				else {
					if (--RegionPath[0].count)
						RegionPath[0].pcon++;
					else
						return 1;
				}
				tpiv++;
			}
		}
	}
	return 0;
}

double reglen(unsigned iRegion) {
	double		length, minimumLength = 1e99;
	unsigned	iCorner, iPoint;
	SMALPNTL*	lineEndPoints[4];

	lineEndPoints[0] = SortedLines[RegionsList[iRegion].start];
	lineEndPoints[1] = &SortedLines[RegionsList[iRegion].start][1];
	lineEndPoints[2] = SortedLines[RegionsList[iRegion].end];
	lineEndPoints[3] = &SortedLines[RegionsList[iRegion].end][1];
	for (iCorner = 0; iCorner < 4; iCorner++) {
		for (iPoint = 0; iPoint < 4; iPoint++) {
			length = hypot(LastRegionCorners[iCorner].x - lineEndPoints[iPoint]->x, LastRegionCorners[iCorner].y - lineEndPoints[iPoint]->y);
			if (length < minimumLength)
				minimumLength = length;
		}
	}
	return minimumLength;
}

void nxtrgn() {
	unsigned	iRegion, iPath, newRegion;
	SMALPNTL*	lineEndPoint;
	double		length, minimumLength = 1e99;
	unsigned	pathLength;					//length of the path to the region

	pathLength = 1;
	while (notdun(pathLength)) {
		pathLength++;
		if (pathLength > 8) {
			lineEndPoint = &*SortedLines[RegionsList[DoneRegion].start];
			LastRegionCorners[0].x = lineEndPoint[0].x;
			LastRegionCorners[0].y = lineEndPoint[0].y;
			LastRegionCorners[1].x = lineEndPoint[1].x;
			LastRegionCorners[1].y = lineEndPoint[1].y;
			lineEndPoint = &*SortedLines[RegionsList[DoneRegion].end];
			LastRegionCorners[2].x = lineEndPoint[0].x;
			LastRegionCorners[2].y = lineEndPoint[0].y;
			LastRegionCorners[3].x = lineEndPoint[1].x;
			LastRegionCorners[3].y = lineEndPoint[1].y;
			newRegion = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++) {
				if (!VisitedRegions[iRegion]) {
					length = reglen(iRegion);
					if (length < minimumLength) {
						minimumLength = length;
						newRegion = iRegion;
					}
				}
			}
			TempPath[SequencePathIndex].skp = 1;
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				if (PathMap[iPath].node == newRegion) {
					TempPath[SequencePathIndex++].pcon = iPath;
					VisitedRegions[newRegion] = 1;
					DoneRegion = newRegion;
					return;
				}
			}
			TempPath[SequencePathIndex].count = VisitedIndex;
			TempPath[SequencePathIndex++].pcon = 0xffffffff;
			VisitedRegions[VisitedIndex] = 1;
			DoneRegion = VisitedIndex;
			return;
		}
	}
	for (iPath = 0; iPath < pathLength; iPath++) {
		TempPath[SequencePathIndex].skp = 0;
		TempPath[SequencePathIndex++].pcon = RegionPath[iPath].pcon;
		VisitedRegions[PathMap[RegionPath[iPath].pcon].node] = 1;
	}
	DoneRegion = PathMap[RegionPath[iPath - 1].pcon].node;
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
	SMALPNTL* lineEndPoint0;
	SMALPNTL* lineEndPoint1;

#if	 __UseASM__
	lineEndPoint0 = srtref(arg1);
	lineEndPoint1 = srtref(arg2);
#else
	lineEndPoint0 = (SMALPNTL*)arg1;
	lineEndPoint1 = (SMALPNTL*)arg2;
#endif

	if (lineEndPoint0->line == lineEndPoint1->line) {
		if (lineEndPoint0->group == lineEndPoint1->group) {
			if (lineEndPoint0->y == lineEndPoint1->y)return 0;
			else {
				if (lineEndPoint0->y > lineEndPoint1->y)
					return 1;
				else
					return -1;
			}
		}
		else {
			if (lineEndPoint0->group > lineEndPoint1->group)
				return 1;
			else
				return -1;
		}
	}
	else {
		if (lineEndPoint0->line > lineEndPoint1->line)
			return 1;
		else
			return -1;
	}
}

void nxtseq(unsigned pathIndex) {
	unsigned nextNode, iPath;

	iPath = MapIndexSequence[SequencePath[pathIndex].node];
	nextNode = SequencePath[pathIndex + 1].node;
	while (iPath < MapIndexSequence[SequencePath[pathIndex].node + 1] && PathMap[iPath].node != nextNode) {
		iPath++;
	}
	SequencePath[PathIndex++].nextGroup = PathMap[iPath].nextGroup;
}

#define BUGSEQ 0

void lcon() {
	unsigned		iPath, iLine, iRegion, iSequence, iNode, bytesInBitmap;
	unsigned		iByte, leftRegion, iOutPath, breakLine, count, startGroup;
	REGION*			regions;
	short			connected;
	RCON*			tempPathMap;
	SMALPNTL*		lineGroupPoint;
	//ToDo - Is regionStarts required?
	unsigned*		regionStarts;
	unsigned		iStartLine;

#if BUGSEQ

	unsigned		bugColor;
#endif

	if (StitchLineCount) {
		SortedLines = new SMALPNTL*[StitchLineCount >> 1];
		SortedLineIndex = 0;
		for (iLine = 0; iLine < StitchLineCount; iLine += 2)
			SortedLines[SortedLineIndex++] = &LineEndpoints[iLine];
		qsort((void*)SortedLines, SortedLineIndex, 4, sqcomp);
		RegionCount = 0;
		// ToDo - Allocate memory locally for regions
		regions = (REGION*)OSequence;
		regions[0].start = 0;
		breakLine = SortedLines[0]->line;
		for (iLine = 0; iLine < SortedLineIndex; iLine++) {
			if (breakLine != SortedLines[iLine]->line) {
				regions[RegionCount++].end = iLine - 1;
				regions[RegionCount].start = iLine;
				breakLine = SortedLines[iLine]->line;
			}
		}
		regions[RegionCount++].end = iLine - 1;
		RegionsList = new REGION[RegionCount];
		VisitedRegions = new char[RegionCount];
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			RegionsList[iRegion].start = regions[iRegion].start;
			RegionsList[iRegion].end = regions[iRegion].end;
			VisitedRegions[iRegion] = 0;
			RegionsList[iRegion].breakCount = 0;
		}
		// ToDo - Allocate memory locally for regionStarts
		regionStarts = (unsigned*)OSequence;
		iStartLine = 0;
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			count = 0;
			if ((RegionsList[iRegion].end - RegionsList[iRegion].start) > 1) {
				startGroup = SortedLines[RegionsList[iRegion].start]->group;
				for (iLine = RegionsList[iRegion].start + 1; iLine <= RegionsList[iRegion].end; iLine++) {
					startGroup++;
					if (SortedLines[iLine]->group != startGroup) {
						if (!count)
							RegionsList[iRegion].regionBreak = iStartLine;
						count++;
						startGroup = SortedLines[iLine]->group;
						regionStarts[iStartLine++] = iLine;
					}
				}
			}
			RegionsList[iRegion].breakCount = count;
		}
		// ToDo - Allocate memory locally for tempPathMap
		tempPathMap = (RCON*)BSequence;
		MapIndexSequence = new unsigned[RegionCount + 1];

#if BUGSEQ
		bugColor = 0; SequenceIndex = 0;
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			for (iLine = RegionsList[iRegion].start; iLine <= RegionsList[iRegion].end; iLine++) {
				lineGroupPoint = &*SortedLines[iLine];
				StitchBuffer[SequenceIndex].attribute = bugColor;
				StitchBuffer[SequenceIndex].x = lineGroupPoint[0].x;
				StitchBuffer[SequenceIndex++].y = lineGroupPoint[0].y;
				StitchBuffer[SequenceIndex].attribute = bugColor;
				StitchBuffer[SequenceIndex].x = lineGroupPoint[1].x;
				StitchBuffer[SequenceIndex++].y = lineGroupPoint[1].y;
			}
			bugColor++;
			bugColor &= 0xf;
		}
		PCSHeader.stitchCount = SequenceIndex;
		goto seqskip;
#endif
		OutputIndex = 0;
		if (RegionCount > 1) {
			PathMapIndex = 0;
			for (iSequence = 0; iSequence < RegionCount; iSequence++) {
				MapIndexSequence[iSequence] = PathMapIndex;
				count = 0; GapToClosestRegion = 0;
				for (iNode = 0; iNode < RegionCount; iNode++) {
					if (iSequence != iNode) {
						connected = regclos(iSequence, iNode);
						if (connected) {
							tempPathMap[PathMapIndex].con = connected;
							tempPathMap[PathMapIndex].nextGroup = NextGroup;
							tempPathMap[PathMapIndex++].node = iNode;
							count++;
						}
					}
				}
				while (!count) {
					GapToClosestRegion += StitchSpacing;
					count = 0;
					for (iNode = 0; iNode < RegionCount; iNode++) {
						if (iSequence != iNode) {
							connected = regclos(iSequence, iNode);
							if (connected) {
								tempPathMap[PathMapIndex].con = connected;
								tempPathMap[PathMapIndex].nextGroup = NextGroup;
								tempPathMap[PathMapIndex++].node = iNode;
								count++;
							}
						}
					}
				}
			}
			MapIndexSequence[iSequence] = PathMapIndex;
			PathMap = new RCON[PathMapIndex + 1];
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				PathMap[iPath].con = tempPathMap[iPath].con;
				PathMap[iPath].node = tempPathMap[iPath].node;
				PathMap[iPath].nextGroup = tempPathMap[iPath].nextGroup;
			}
			//find the leftmost region
			startGroup = 0xffffffff; leftRegion = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++) {
				lineGroupPoint = &*SortedLines[RegionsList[iRegion].start];
				if (lineGroupPoint->group < startGroup) {
					startGroup = lineGroupPoint->group;
					leftRegion = iRegion;
				}
			}
			OutputIndex = 0;
			// ToDo - Allocate memory locally for TempPath
			TempPath = (RGSEQ*)OSequence;
			//find the leftmost region in PathMap
			SequencePathIndex = 1;
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				if (PathMap[iPath].node == leftRegion)
					goto lconskip;
			}
			PathMap[PathMapIndex].node = leftRegion;
			PathMap[PathMapIndex].nextGroup = 0;
			iPath = PathMapIndex;
		lconskip:;
			//set the first entry in the temporary path to the leftmost region
			TempPath[0].pcon = iPath;
			TempPath[0].count = 1;
			TempPath[0].skp = 0;
			VisitedRegions[leftRegion] = 1;
			DoneRegion = leftRegion;
			while (unvis())
				nxtrgn();
			iOutPath = 0;
			count = 0xffffffff;
			// ToDo - Allocate memory locally for SequencePath
			SequencePath = (FSEQ*)&OSequence[OSEQLEN >> 1];
			for (iPath = 0; iPath < SequencePathIndex; iPath++) {
				SequencePath[iOutPath].skp = TempPath[iPath].skp;
				if (TempPath[iPath].pcon == 0xffffffff) {
					SequencePath[iOutPath++].node = TempPath[iPath].count;
					count = TempPath[iPath].count;
				}
				else {
					if (TempPath[iPath].pcon != count) {
						count = TempPath[iPath].pcon;
						SequencePath[iOutPath++].node = PathMap[TempPath[iPath].pcon].node;
					}
				}
			}
			// ToDo - should this be iPath or iOutPath?
			SequencePathIndex = iPath; PathIndex = 0;
			for (iPath = 0; iPath < SequencePathIndex; iPath++)
				nxtseq(iPath);
			bytesInBitmap = (SortedLineIndex >> 5) + 1;
			SequenceFlagBitmap = new unsigned[bytesInBitmap];
			for (iByte = 0; iByte < bytesInBitmap; iByte++)
				SequenceFlagBitmap[iByte] = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++)
				VisitedRegions[iRegion] = 0;
			LastGroup = 0;
			for (iPath = 0; iPath < PathIndex; iPath++) {
				//				sprintf_s(MsgBuffer, sizeof(MsgBuffer),"iterator %d,vrt %d,grpn %d\n",iterator,PathMap[iPath].vrt,PathMap[iPath].grpn);
				//				OutputDebugString(MsgBuffer);
				if (!unvis())
					break;
				durgn(iPath);
			}
		}
		else {
			PathMap = new RCON[1];
			SequencePath = new FSEQ[1];
			bytesInBitmap = (SortedLineIndex >> 5) + 1;
			SequenceFlagBitmap = new unsigned[bytesInBitmap];
			for (iByte = 0; iByte < bytesInBitmap; iByte++)
				SequenceFlagBitmap[iByte] = 0;
			LastGroup = 0;
			SequencePath[0].node = 0;
			SequencePath[0].nextGroup = SortedLines[RegionsList[0].end]->group;
			SequencePath[0].skp = 0;
			durgn(0);
			delete[] SequencePath;
		}
		//skip:;

#if BUGSEQ

		seqskip : ;
#endif
				  delete[] SortedLines;
				  delete[] LineEndpoints;
				  delete[] RegionsList;
				  delete[] MapIndexSequence;
				  delete[] VisitedRegions;
				  delete[] PathMap;
				  delete[] GroupIndexSequence;
				  delete[] SequenceFlagBitmap;
	}
}

unsigned seqtab[] =
{
	12,7,15,11,13,17,
};

#define RITSIZ 6

void bakseq() {
#if BUGBAK

	for (SequenceIndex = 0; SequenceIndex < OutputIndex; SequenceIndex++)
	{
		OSequence[SequenceIndex].x = BSequence[SequenceIndex].x;
		OSequence[SequenceIndex].y = BSequence[SequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	int			count, rcnt;
	int			iSequence, rit;
	dPOINT		delta, point, step;
	double		length, slope;
	double		UserStitchLength2 = UserStitchLength * 2;
	double		UserStitchLength9 = UserStitchLength / 9;
	double		StitchSpacing2 = StitchSpacing * 2;

	//ToDo - More renaming required
	SequenceIndex = 0;
	rstMap(FILDIR);
	iSequence = OutputIndex - 1;
	OSequence[SequenceIndex].x = BSequence[iSequence].x;
	OSequence[SequenceIndex].y = BSequence[iSequence].y;
	SequenceIndex++;
	SelectedPoint.x = BSequence[iSequence].x;
	SelectedPoint.y = BSequence[iSequence].y;
	iSequence--;
	while (iSequence > 0) {
		rcnt = iSequence%RITSIZ;
		if (SequenceIndex > MAXSEQ) {
			SequenceIndex = MAXSEQ - 1;
			return;
		}
		rit = BSequence[iSequence].x / StitchSpacing2;
		delta.x = BSequence[iSequence].x - BSequence[iSequence + 1].x;
		delta.y = BSequence[iSequence].y - BSequence[iSequence + 1].y;
		if (delta.y)
			slope = delta.x / delta.y;
		else
			slope = 1e99;
		switch (BSequence[iSequence].attribute) {
		case SEQTOP:

			if (SelectedForm->extendedAttribute&AT_SQR) {
				if (toglMap(FILDIR)) {
					OSequence[SequenceIndex].x = BSequence[iSequence - 1].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence - 1].y;
					count = ceil(BSequence[iSequence].y / UserStitchLength);
				// ToDo - replace labels with while and break
				blntop:;
					OSequence[SequenceIndex].y = count*UserStitchLength + (rit%seqtab[rcnt])*UserStitchLength9;
					if (OSequence[SequenceIndex].y > BSequence[iSequence].y)
						goto blntopx;
					delta.y = OSequence[SequenceIndex].y - BSequence[iSequence].y;
					OSequence[SequenceIndex++].x = BSequence[iSequence].x;
					count++;
					goto blntop;
				blntopx:;
					OSequence[SequenceIndex].x = BSequence[iSequence].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence].y;
				}
				else {
					OSequence[SequenceIndex].x = BSequence[iSequence].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence].y;
					count = floor(BSequence[iSequence].y / UserStitchLength);
				blntbot:;
					OSequence[SequenceIndex].y = count*UserStitchLength - ((rit + 2) % seqtab[rcnt])*UserStitchLength9;
					if (OSequence[SequenceIndex].y < BSequence[iSequence - 1].y)
						goto blntbotx;
					delta.y = OSequence[SequenceIndex].y - BSequence[iSequence - 1].y;
					OSequence[SequenceIndex++].x = BSequence[iSequence].x;
					count--;
					goto blntbot;
				blntbotx:;
					OSequence[SequenceIndex].x = BSequence[iSequence - 1].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence - 1].y;
				}
			}
			else {
				count = ceil(BSequence[iSequence + 1].y / UserStitchLength);
			toplab:;
				OSequence[SequenceIndex].y = count*UserStitchLength + (rit%seqtab[rcnt])*UserStitchLength9;
				if (OSequence[SequenceIndex].y > BSequence[iSequence].y)
					goto toplabx;
				delta.y = OSequence[SequenceIndex].y - BSequence[iSequence + 1].y;
				delta.x = slope*delta.y;
				OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
				count++;
				goto toplab;
			toplabx:;
				OSequence[SequenceIndex].x = BSequence[iSequence].x;
				OSequence[SequenceIndex++].y = BSequence[iSequence].y;
			}
			break;

		case SEQBOT:

			if (!(SelectedForm->extendedAttribute&AT_SQR)) {
				count = floor(BSequence[iSequence + 1].y / UserStitchLength);
			botlab:;
				OSequence[SequenceIndex].y = count*UserStitchLength - ((rit + 2) % seqtab[rcnt])*UserStitchLength9;
				if (OSequence[SequenceIndex].y < BSequence[iSequence].y)
					goto botlabx;
				delta.y = OSequence[SequenceIndex].y - BSequence[iSequence + 1].y;
				delta.x = slope*delta.y;
				OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
				count--;
				goto botlab;
			botlabx:;
				OSequence[SequenceIndex].x = BSequence[iSequence].x;
				OSequence[SequenceIndex++].y = BSequence[iSequence].y;
			}
			break;

		case 0:

			delta.x = BSequence[iSequence].x - BSequence[iSequence + 1].x;
			delta.y = BSequence[iSequence].y - BSequence[iSequence + 1].y;
			rstMap(FILDIR);
			length = hypot(delta.x, delta.y);
			if (length) {
				if (length > UserStitchLength2) {
					point.x = BSequence[iSequence + 1].x;
					point.y = BSequence[iSequence + 1].y;
					count = length / UserStitchLength - 1;
					if (chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXSEQ - 3)
						return;
					step.x = delta.x / count;
					step.y = delta.y / count;
					while (count) {
						point.x += step.x;
						point.y += step.y;
						OSequence[SequenceIndex].x = point.x;
						OSequence[SequenceIndex++].y = point.y;
						count--;
					}
				}
			}
			OSequence[SequenceIndex].x = BSequence[iSequence].x;
			OSequence[SequenceIndex++].y = BSequence[iSequence].y;
		}
		iSequence--;
	}
#endif
}

void fnvrt() {
	unsigned		iVertex, iStitch, ind, ine, iLine, inf, stitchCount, lineCount = 0, tind;
	int				stitchOffset;
	dPOINTLINE*		projectedPoints;
	dPOINTLINE**	projectedPointsArray;
	double			lowX, highX, currentX, step;
	dPOINT			point;
	unsigned*		groupIndex;
	unsigned		maximumLines = 0;	//maximum angle fill lines for any adjusted y cordinate

	CurrentFillVertices = SelectedForm->vertices;
	VertexCount = SelectedForm->vertexCount;
	lowX = highX = CurrentFillVertices[0].x;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		if (CurrentFillVertices[iVertex].x > highX)
			highX = CurrentFillVertices[iVertex].x;
		if (CurrentFillVertices[iVertex].x < lowX)
			lowX = CurrentFillVertices[iVertex].x;
	}

	stitchOffset = lowX / StitchSpacing;
	lowX = StitchSpacing*stitchOffset;
	stitchCount = (highX - lowX) / StitchSpacing + 1;
	projectedPoints = new dPOINTLINE[VertexCount + 2];
	projectedPointsArray = new dPOINTLINE*[VertexCount + 2];
	step = (highX - lowX) / stitchCount;
	currentX = lowX;
	for (iStitch = 0; iStitch < stitchCount; iStitch++) {
		iLine = 0;
		currentX += step;
		for (iVertex = 0; iVertex < (unsigned)VertexCount - 1; iVertex++) {
			if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[iVertex + 1], &point))
				iLine++;
		}
		if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[0], &point))
			iLine++;
		lineCount += iLine;
		if (iLine > maximumLines)
			maximumLines = iLine;
	}
	maximumLines = (maximumLines >> 1);
	LineEndpoints = new SMALPNTL[lineCount + 1];
	StitchLineCount = 0; LineGroupIndex = 0; 
	// ToDo - Allocate memory locally for groupIndex
	groupIndex = (unsigned*)BSequence;
	GroupIndexCount = 0;
	currentX = lowX;
	for (iStitch = 0; iStitch < stitchCount; iStitch++) {
		currentX += step;
		inf = 0;
		for (iVertex = 0; iVertex < (unsigned)VertexCount - 1; iVertex++) {
			if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[iVertex + 1], &point)) {
				projectedPointsArray[inf] = &projectedPoints[inf];
				projectedPoints[inf].line = iVertex;
				projectedPoints[inf].x = point.x;
				projectedPoints[inf++].y = point.y;

			}
		}
		if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[0], &point)) {
			projectedPointsArray[inf] = &projectedPoints[inf];
			projectedPoints[inf].line = iVertex;
			projectedPoints[inf].x = point.x;
			projectedPoints[inf++].y = point.y;
		}
		if (inf > 1) {
			inf &= 0xfffffffe;
			groupIndex[GroupIndexCount++] = StitchLineCount;
			// ToDo - replace 4 with sizeof(dPOINTLINE(?))
			qsort((void*)projectedPointsArray, inf, 4, comp);
			ine = 0;
			tind = StitchLineCount;
			while (ine < inf) {
				if (StitchLineCount < lineCount) {
					LineEndpoints[StitchLineCount].line = projectedPointsArray[ine]->line;
					LineEndpoints[StitchLineCount].group = LineGroupIndex;
					LineEndpoints[StitchLineCount].x = projectedPointsArray[ine]->x;
					LineEndpoints[StitchLineCount++].y = projectedPointsArray[ine++]->y;
					LineEndpoints[StitchLineCount].line = projectedPointsArray[ine]->line;
					LineEndpoints[StitchLineCount].group = LineGroupIndex;
					LineEndpoints[StitchLineCount].x = projectedPointsArray[ine]->x;
					LineEndpoints[StitchLineCount++].y = projectedPointsArray[ine++]->y;
				}
			}
			if (StitchLineCount != tind)
				LineGroupIndex++;
		}
	}
	groupIndex[GroupIndexCount++] = StitchLineCount;
	GroupIndexSequence = new unsigned[GroupIndexCount];
	for (ind = 0; ind < GroupIndexCount; ind++)
		GroupIndexSequence[ind] = groupIndex[ind];
	LineGroupIndex--;
	delete[] projectedPoints;
	delete[] projectedPointsArray;
}

void fnhor() {
	unsigned	iVertex;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	RotationAngle = PI / 2;
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex].x = SelectedForm->vertices[iVertex].x;
		AngledForm.vertices[iVertex].y = SelectedForm->vertices[iVertex].y;
		rotflt(&AngledForm.vertices[iVertex]);
	}
	SelectedForm = &AngledForm;
	fnvrt();
	SelectedForm = &FormList[ClosestFormToCursor];
}

void fsvrt() {
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = VRTF;
	SelectedForm->type = FRMFPOLY;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = StitchSpacing;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refilfn();
}

void filvrt() {
	unsigned iForm;

	if (filmsgs(FMM_VRT))
		return;
	if (SelectedFormCount) {
		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
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
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = HORF;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = StitchSpacing;
	SelectedForm->angleOrClipData.angle = (float)PI / 2;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refil();
}

void filhor() {
	unsigned iForm;

	if (filmsgs(FMM_HOR))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
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
	double savedAngle;

	savedAngle = RotationAngle;
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = ANGF;
	RotationAngle = PI / 2 - RotationAngle;
	SelectedForm->angleOrClipData.angle = (float)IniFile.fillAngle;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = StitchSpacing;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refil();
	RotationAngle = savedAngle;
}

void filangl() {
	unsigned iForm;

	if (filmsgs(FMM_ANG))
		return;
	RotationAngle = IniFile.fillAngle;
	if (SelectedFormCount) {
		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
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

void sRct2px(fRECTANGLE stitchRect, RECT* screenRect) {
	screenRect->left = (stitchRect.left - ZoomRect.left)*ZoomRatio.x + 0.5;
	screenRect->right = (stitchRect.right - ZoomRect.left)*ZoomRatio.x + 0.5;
	screenRect->top = (StitchWindowClientRect.bottom) - (stitchRect.top - ZoomRect.bottom)*ZoomRatio.y + 0.5;
	screenRect->bottom = (StitchWindowClientRect.bottom) - (stitchRect.bottom - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void setmfrm() {
	unsigned	iForm;
	POINT		point;
	POINT		offset;

	sfCor2px(FormList[ClosestFormToCursor].vertices[0], &point);
	offset.x = Msg.pt.x - StitchWindowOrigin.x - point.x + FormMoveDelta.x;
	offset.y = Msg.pt.y - StitchWindowOrigin.y - point.y + FormMoveDelta.y;
	for (iForm = 0; iForm < FormList[ClosestFormToCursor].vertexCount; iForm++) {
		sfCor2px(FormList[ClosestFormToCursor].vertices[iForm], &point);
		FormLines[iForm].x = point.x + offset.x;
		FormLines[iForm].y = point.y + offset.y;
	}
	FormLines[iForm].x = FormLines[0].x;
	FormLines[iForm].y = FormLines[0].y;
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
	POINT		point, formOrigin;
	RECT		rectangle;
	unsigned	iControl;
	double		length, minimumLength = 1e99;

	fvars(ClosestFormToCursor);
	point.x = Msg.pt.x - StitchWindowOrigin.x;
	point.y = Msg.pt.y - StitchWindowOrigin.y;
	NewFormVertexCount = SelectedForm->vertexCount + 1;
	duzrat();
	sRct2px(SelectedForm->rectangle, &rectangle);
	FormControlPoints[0].x = FormControlPoints[6].x = FormControlPoints[7].x = FormControlPoints[8].x = rectangle.left;
	FormControlPoints[1].x = FormControlPoints[5].x = midl(rectangle.right, rectangle.left);
	FormControlPoints[0].y = FormControlPoints[1].y = FormControlPoints[2].y = FormControlPoints[8].y = rectangle.top;
	FormControlPoints[3].y = FormControlPoints[7].y = midl(rectangle.top, rectangle.bottom);
	FormControlPoints[4].y = FormControlPoints[5].y = FormControlPoints[6].y = rectangle.bottom;
	FormControlPoints[2].x = FormControlPoints[3].x = FormControlPoints[4].x = rectangle.right;
	for (iControl = 0; iControl < 10; iControl++) {
		length = hypot(FormControlPoints[iControl].x - point.x, FormControlPoints[iControl].y - point.y);
		if (length < minimumLength) {
			minimumLength = length;
			SelectedFormControlVertex = iControl;
		}
		if (minimumLength < CLOSENUF) {
			ritfrct(ClosestFormToCursor, StitchWindowDC);
			for (iControl = 0; iControl < 4; iControl++) {
				StretchBoxLine[iControl].x = FormControlPoints[iControl << 1].x;
				StretchBoxLine[iControl].y = FormControlPoints[iControl << 1].y;
			}
			StretchBoxLine[4].x = StretchBoxLine[0].x;
			StretchBoxLine[4].y = StretchBoxLine[0].y;
			if (SelectedFormControlVertex & 1)
				setMap(STRTCH);
			else {
				setMap(EXPAND);
				XYratio = (double)(SelectedForm->rectangle.right - SelectedForm->rectangle.left) / (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom);
			}
			SelectedFormControlVertex >>= 1;
			setMap(SHOSTRTCH);
			strtchbox();
			return 1;
		}
	}
	if (point.x >= rectangle.left&&point.x <= rectangle.right&&
		point.y >= rectangle.top&&point.y <= rectangle.bottom) {
		sfCor2px(SelectedForm->vertices[0], &formOrigin);
		FormMoveDelta.x = formOrigin.x - point.x;
		FormMoveDelta.y = formOrigin.y - point.y;
		setMap(FRMOV);
		return 1;
	}
	else
		return 0;
}

void rstfrm() {
	fPOINT		offset;
	unsigned	iVertex, iStitch;
	POINT		point;
	unsigned	attribute = (ClosestFormToCursor << 4);

	setmfrm();
	rstMap(FRMOV);
	point.x = Msg.pt.x + FormMoveDelta.x;
	point.y = Msg.pt.y + FormMoveDelta.y;
	pxCor2stch(point);
	offset.x = SelectedPoint.x - SelectedForm->vertices[0].x;
	offset.y = SelectedPoint.y - SelectedForm->vertices[0].y;
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		SelectedForm->vertices[iVertex].x += offset.x;
		SelectedForm->vertices[iVertex].y += offset.y;
	}
	SelectedForm->rectangle.bottom += offset.y;
	SelectedForm->rectangle.top += offset.y;
	SelectedForm->rectangle.left += offset.x;
	SelectedForm->rectangle.right += offset.x;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute&FRMSK) == attribute&&StitchBuffer[iStitch].attribute&ALTYPMSK && !(StitchBuffer[iStitch].attribute&NOTFRM)) {
			StitchBuffer[iStitch].x += offset.x;
			StitchBuffer[iStitch].y += offset.y;
		}
	}
}

void clrfills() {
	unsigned iForm;

	for (iForm = 0; iForm < FormIndex; iForm++) {
		FormList[iForm].clipEntries = 0;
		FormList[iForm].lengthOrCount.clipCount = 0;
		FormList[iForm].edgeType = 0;
		FormList[iForm].fillType = 0;
		FormList[iForm].attribute &= NFRECONT;
		FormList[iForm].extendedAttribute &= !(AT_UND | AT_WALK);
	}
	ClipPointIndex = 0;
}

void dusat() {
	POINT* line = &FormLines[SatinIndex - 1];

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, line, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unsat() {
	if (rstMap(SHOSAT))
		dusat();
}

void drwsat() {
	unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	setMap(SHOSAT);
	dusat();
}

void satpnt0() {
	px2stch();
	FormLines[0].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[0].y = Msg.pt.y - StitchWindowOrigin.y;
	TempPolygon[0].x = SelectedPoint.x;
	TempPolygon[0].y = SelectedPoint.y;
	SatinIndex = 1;
	setMap(SATPNT);
}

void satpnt1() {
	unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	dusat();
	TempPolygon[SatinIndex].x = SelectedPoint.x;
	TempPolygon[SatinIndex].y = SelectedPoint.y;
	SatinIndex++;
	setMap(RESTCH);
}

void satfix() {
	unsigned iVertex;

	if (SatinIndex > 1) {
		FormList[FormIndex].vertices = adflt(SatinIndex);
		for (iVertex = 0; iVertex < SatinIndex; iVertex++) {
			FormList[FormIndex].vertices[iVertex].x = TempPolygon[iVertex].x;
			FormList[FormIndex].vertices[iVertex].y = TempPolygon[iVertex].y;
		}
		FormList[FormIndex].vertexCount = SatinIndex;
		frmout(FormIndex);
		FormList[FormIndex].satinGuideCount = 0;
		FormIndex++;
		setMap(INIT);
	}
	rstMap(SHOSAT);
	setMap(RESTCH);
}

void delcon(unsigned GuideIndex)
{
	unsigned iForm;
	unsigned iGuide;
	SATCON* guide;
	FRMHED* formHeader;

	guide = &SelectedForm->satinOrAngle.guide[GuideIndex];
	iGuide = &SelectedForm->satinOrAngle.guide[GuideIndex] - SatinConnects;
	if (SatinConnectIndex > iGuide)
		MoveMemory(guide, &guide[1], (SatinConnectIndex - iGuide + 1) * sizeof(SATCON));
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
	{
		formHeader = &FormList[iForm];
		if (formHeader->type == SAT&&formHeader->satinGuideCount)
			formHeader->satinOrAngle.guide--;
	}
	if (ClosestVertexToCursor < WordParam)
		WordParam--;
	SelectedForm->satinGuideCount--;
	SatinConnectIndex--;
	CurrentFormConnectionsCount = SelectedForm->satinGuideCount;
	if (SelectedForm->fillType == SATF)
		refil();
	coltab();
	setMap(RESTCH);
}

BOOL satselfn() {
	unsigned iForm, iVertex, layerCode;
	double		length, minimumLength = 1e99;
	double		deltaX, deltaY;

	px2stch();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		layerCode = (FormList[iForm].attribute&FRMLMSK) >> 1;
		if (!ActiveLayer || !layerCode || layerCode == ActiveLayer) {
			for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++) {
				deltaX = SelectedPoint.x - FormList[iForm].vertices[iVertex].x;
				deltaY = SelectedPoint.y - FormList[iForm].vertices[iVertex].y;
				length = hypot(deltaX, deltaY);
				if (length < minimumLength) {
					minimumLength = length;
					ClosestFormToCursor = iForm;
					ClosestVertexToCursor = iVertex;
				}
			}
		}
	}
	if (minimumLength < CLOSENUF)
		return 1;
	else
		return 0;
}

void satsel() {
	if (satselfn()) {
		fvars(ClosestFormToCursor);
		duzrat();
		StartPoint = ClosestVertexToCursor;
		sfCor2px(SelectedForm->vertices[ClosestVertexToCursor], &FormLines[0]);
		rstMap(SHOCON);
		setMap(SATCNKT);
		if (SelectedForm->type == FRMFPOLY)
			SelectedForm->type = SAT;
	}
}

void ducon() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, FormLines, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uncon() {
	if (rstMap(SHOCON))
		ducon();
}

void drwcon() {
	uncon();
	FormLines[1].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[1].y = Msg.pt.y - StitchWindowOrigin.y;
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

unsigned setchk(unsigned bit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, CheckMap
		mov		ecx, bit
		bts[ebx], ecx
		jnc		short setcx
		dec		eax
		setcx :
	}
#else
	return _bittestandset((long *)CheckMap, bit) ? 0xFFFFFFFF : 0;
#endif
}

unsigned chkchk(unsigned bit) {
	// ToDo - reverse the return value (if the bit is set,  return 0xFFFFFFFF) and adjust usage appropriately 
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, CheckMap
		mov		ecx, bit
		bt[ebx], ecx
		jc		short ccx
		dec		eax
		ccx :
	}
#else
	return _bittest((long *)CheckMap, bit) ? 0 : 0xFFFFFFFF;
#endif
}

unsigned nxtchk(unsigned bit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, bit
		shl		ebx, 2
		add		ebx, CheckMap
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
	if (CheckMap[bit] == 0)
		return 0xffffffff;

	DWORD returnBit;

	_BitScanForward(&returnBit, CheckMap[bit]);
	_bittestandreset((long *)(CheckMap + bit), returnBit);

	return returnBit;
#endif
}

unsigned prvchk(unsigned bit) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, bit
		shl		ebx, 2
		add		ebx, CheckMap
		mov		ecx, [ebx]
		bsr		eax, ecx
		jne		short prvc1
		dec		eax
		jmp		short prvcx
prvc1 :	btr		ecx, eax
		mov[ebx], ecx
prvcx :
	}
#else
	//Check translation
	DWORD returnBit;

	if (CheckMap[bit] == 0)
		return 0xffffffff;

	_BitScanReverse(&returnBit, CheckMap[bit]);
	_bittestandreset((long *)(CheckMap + bit), returnBit);

	return returnBit;
#endif
}

void satadj()
{
	unsigned	iGuide, iSource, iWord, iForm, iForward, iVertex, iReverse, iDestination, mapSize;
	SATCON*		guide;
	SATCON*		sourceGuide;
	SATCON*		destinationGuide;
	unsigned short guideCount;
	FRMHED*		formHeader;

	fvars(ClosestFormToCursor);
	guideCount = SelectedForm->satinGuideCount;
	for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++)
	{
		if (CurrentFormGuides[iGuide].finish > VertexCount - 1)
			CurrentFormGuides[iGuide].finish = VertexCount - 1;
		if (CurrentFormGuides[iGuide].start > VertexCount - 1)
			CurrentFormGuides[iGuide].start = VertexCount - 1;
	}
	// ToDo - Allocate memory locally for guide
	guide = (SATCON*)BSequence;
	mapSize = (VertexCount >> 5) + 1;
	// ToDo - Allocate memory locally for CheckMap
	CheckMap = (unsigned*)OSequence;
	iDestination = 0;
	for (iSource = 0; iSource < CurrentFormConnectionsCount; iSource++)
	{
		if (CurrentFormGuides[iSource].start != CurrentFormGuides[iSource].finish)
		{
			CurrentFormGuides[iDestination].start = CurrentFormGuides[iSource].start;
			CurrentFormGuides[iDestination].finish = CurrentFormGuides[iSource].finish;
			iDestination++;
		}
	}
	// Todo - Should this be iSource or iDestination?
	CurrentFormConnectionsCount = SelectedForm->satinGuideCount = iSource;
	if (WordParam || SelectedForm->attribute&FRMEND)
	{
		for (iWord = 0; iWord < mapSize; iWord++)
			CheckMap[iWord] = 0;
		if (SelectedForm->attribute&FRMEND)
		{
			setchk(0);
			setchk(1);
		}
		if (WordParam)
		{
			setchk(WordParam);
			setchk(WordParam + 1);
		}
		iDestination = 0;
		for (iSource = 0; iSource < CurrentFormConnectionsCount; iSource++)
		{
			if (chkchk(CurrentFormGuides[iSource].start) && chkchk(CurrentFormGuides[iSource].finish))
			{
				guide[iDestination].start = CurrentFormGuides[iSource].start;
				guide[iDestination].finish = CurrentFormGuides[iSource].finish;
				iDestination++;
			}
		}
		CurrentFormConnectionsCount = SelectedForm->satinGuideCount = iDestination;
		if (WordParam)
		{
			iDestination = 0;
			for (iSource = 0; iSource < CurrentFormConnectionsCount; iSource++)
			{
				if (CurrentFormGuides[iSource].start < WordParam)
				{
					guide[iDestination].start = CurrentFormGuides[iSource].start;
					guide[iDestination++].finish = CurrentFormGuides[iSource].finish;
				}
			}
			CurrentFormConnectionsCount = SelectedForm->satinGuideCount = iDestination;
		}
	} else
	{
		for (iGuide = 0; iGuide < CurrentFormConnectionsCount; iGuide++)
		{
			guide[iGuide].start = CurrentFormGuides[iGuide].start;
			guide[iGuide].finish = CurrentFormGuides[iGuide].finish;
		}
	}
	if (CurrentFormConnectionsCount)
	{
		for (iWord = 0; iWord < mapSize; iWord++)
			CheckMap[iWord] = 0;
		for (iGuide = 0; iGuide < CurrentFormConnectionsCount; iGuide++)
		{
			iForward = CurrentFormGuides[iGuide].start;
			if (iForward > (unsigned)WordParam - 1)
				iForward = WordParam - 1;
			if (setchk(iForward))
			{
				iReverse = iForward;
				if (iReverse)
					iReverse--;
				while (!chkchk(iForward) && iForward < (unsigned)WordParam - 1)
					iForward++;
				while (iReverse && (!chkchk(iReverse)))
					iReverse--;
				if (!chkchk(iForward) && !chkchk(iReverse))
					break;
				if (chkchk(iForward) && chkchk(iReverse))
				{
					if (iForward - CurrentFormGuides[iGuide].start > CurrentFormGuides[iGuide].start - iReverse)
						setchk(iReverse);
					else
						setchk(iForward);
				} else {
					if (chkchk(iForward))
						setchk(iReverse);
					else
						setchk(iForward);
				}

			}
		}
		iGuide = 0;
		// ToDo - Does this for loop make sense?
		for (iWord = 0; iWord < mapSize; iWord++)
		{
			do
			{
				iVertex = nxtchk(iWord);
				if (iVertex < VertexCount)
					CurrentFormGuides[iGuide++].start = iVertex + (iWord << 5);
			} while (iVertex < VertexCount);
		}
		CurrentFormConnectionsCount = SelectedForm->satinGuideCount = iGuide;
		for (iWord = 0; iWord < mapSize; iWord++)
			CheckMap[iWord] = 0;
		// Todo - are iForward and iReverse appropriate variable names below?
		for (iGuide = 0; iGuide < CurrentFormConnectionsCount; iGuide++)
		{
			iForward = iReverse = CurrentFormGuides[iGuide].finish;
			if (iForward > (unsigned)VertexCount - 1)
				iForward = VertexCount - 1;
			if (setchk(iForward))
			{
				if (iForward < (unsigned)VertexCount - 1)
					iForward++;
				if (iReverse > (unsigned)WordParam + 1)
					iReverse--;
				while (!chkchk(iForward) && iForward < (unsigned)VertexCount - 1)
					iForward++;
				while (iReverse > (unsigned)WordParam - 1 && (!chkchk(iReverse)))
					iReverse--;
				if (!chkchk(iForward) && !chkchk(iReverse))
					break;
				if (chkchk(iForward) && chkchk(iReverse))
				{
					if (iForward - CurrentFormGuides[iGuide].finish > CurrentFormGuides[iGuide].finish - iReverse)
						setchk(iReverse);
					else
						setchk(iForward);
				} else
				{
					if (chkchk(iForward))
						setchk(iForward);
					else
						setchk(iReverse);
				}
			}
		}
		iGuide = 0;
		for (iWord = mapSize; iWord != 0; iWord--)
		{
			do
			{
				iReverse = prvchk(iWord - 1);
				if (iReverse < VertexCount)
					CurrentFormGuides[iGuide++].finish = iReverse + ((iWord - 1) << 5);
			} while (iReverse < VertexCount);
		}
		if (iGuide < CurrentFormConnectionsCount)
			iGuide = CurrentFormConnectionsCount;
		CurrentFormConnectionsCount = SelectedForm->satinGuideCount = iGuide;
		if (WordParam)
		{
			if (CurrentFormConnectionsCount > VertexCount - WordParam - 2)
				CurrentFormConnectionsCount = VertexCount - WordParam - 2;
			if (CurrentFormConnectionsCount > WordParam - 2)
				CurrentFormConnectionsCount = WordParam - 2;
			SelectedForm->satinGuideCount = CurrentFormConnectionsCount;
		}
	}
	if (SelectedForm->satinGuideCount < guideCount)
	{
		iGuide = guideCount - CurrentFormConnectionsCount;
		sourceGuide = destinationGuide = SelectedForm->satinOrAngle.guide;
		destinationGuide += SelectedForm->satinGuideCount;
		sourceGuide += guideCount;
		MoveMemory(destinationGuide, sourceGuide, sizeof(SATCON)*(&SatinConnects[SatinConnectIndex] - sourceGuide + 1));
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		{
			formHeader = &FormList[iForm];
			if (formHeader->type == SAT)
				formHeader->satinOrAngle.guide -= iGuide;
		}
		SatinConnectIndex -= iGuide;
	}
}

void satclos() {
	unsigned	ind, clos, psac;
	double		dx, dy, tlen, len = 1e99;
	SATCON*		tsac;

	uninsf();
	px2stch();
	psac = SelectedForm->satinGuideCount;
	tsac = SelectedForm->satinOrAngle.guide;
	clos = StartPoint;
	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
		dx = SelectedPoint.x - SelectedForm->vertices[ind].x;
		dy = SelectedPoint.y - SelectedForm->vertices[ind].y;
		tlen = hypot(dx, dy);
		if (tlen < len) {
			len = tlen;
			ClosestVertexToCursor = ind;
		}
	}
	rstMap(SATCNKT);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->fillType = CONTF;
		clos = ClosestVertexToCursor;
		if (StartPoint > clos) {
			psac = clos;
			clos = StartPoint;
			StartPoint = psac;
		}
		if (!StartPoint)
			StartPoint++;
		if (StartPoint == SelectedForm->vertexCount - 2 && clos == (unsigned)SelectedForm->vertexCount - 1) {
			StartPoint = 1;
			clos = SelectedForm->vertexCount - 2;
		}
		if (clos >= (unsigned)SelectedForm->vertexCount - 2) {
			clos = SelectedForm->vertexCount - 2;
			if (StartPoint >= (unsigned)SelectedForm->vertexCount - 2)
				StartPoint = SelectedForm->vertexCount - 2;
		}
		if (clos - StartPoint < 2) {
			clos = StartPoint + 2;
			if (clos > (unsigned)SelectedForm->vertexCount - 2) {
				clos = clos - SelectedForm->vertexCount - 2;
				clos -= clos;
				StartPoint -= clos;
			}
		}
		SelectedForm->angleOrClipData.guide.start = StartPoint;
		SelectedForm->angleOrClipData.guide.finish = clos;
	}
	else {
		if (ClosestVertexToCursor < clos) {
			ind = ClosestVertexToCursor;
			ClosestVertexToCursor = clos;
			clos = ind;
		}
		if (clos == 0 && ClosestVertexToCursor == (unsigned)VertexCount - 1) {
			clos = VertexCount - 1;
			ClosestVertexToCursor = VertexCount;
		}
		if (clos == 1 && ClosestVertexToCursor == (unsigned)VertexCount) {
			clos = 0;
			ClosestVertexToCursor = 1;
		}
		if (ClosestVertexToCursor - clos == 1) {
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
			if (CurrentFormConnectionsCount) {
				sacspac(&SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount].start = clos;
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satadj();
			}
			else {
				SelectedForm->satinOrAngle.guide = nusac(ClosestFormToCursor, 1);
				SelectedForm->satinOrAngle.guide[psac].start = clos;
				SelectedForm->satinOrAngle.guide[psac].finish = ClosestVertexToCursor;
				SelectedForm->satinGuideCount = 1;
			}
		}
	}
}

void satknkt() {
	satclos();
	fvars(ClosestFormToCursor);
	refil();
	setMap(RESTCH);
}

void ritseq1(unsigned ind) {
	BSequence[SequenceIndex].x = CurrentFormVertices[ind].x;
	BSequence[SequenceIndex].y = CurrentFormVertices[ind].y;
	SequenceIndex++;
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
				BSequence[SequenceIndex].attribute = 0;
				ritseq1(astrt%VertexCount);
			}
			else {
				if (chkMap(BARSAT)) {
					ritseq1(astrt%VertexCount);
					ritseq1(bstrt%VertexCount);
				}
				else {
					SelectedPoint.x = CurrentFormVertices[astrt].x;
					SelectedPoint.y = CurrentFormVertices[astrt].y;
					OSequence[SequenceIndex].x = SelectedPoint.x;
					OSequence[SequenceIndex++].y = SelectedPoint.y;
				}
			}
		}
		alen = Lengths[afin] - Lengths[astrt];
		blen = Lengths[bstrt] - Lengths[bfin];
		if (fabs(alen) > fabs(blen))
			cnt = fabs(blen) / StitchSpacing;
		else
			cnt = fabs(alen) / StitchSpacing;
		asegs = ((afin > astrt) ? (afin - astrt) : (astrt - afin));
		bsegs = ((bstrt > bfin) ? (bstrt - bfin) : (bfin - bstrt));
		acnts = new unsigned[asegs];
		bcnts = new unsigned[bsegs + 1];
		ine = astrt;
		tcnt = 0;
		for (ind = 0; ind < asegs - 1; ind++) {
			inf = nxt(ine);
			acnts[ind] = ((Lengths[inf] - Lengths[ine]) / alen)*cnt + 0.5;
			tcnt += acnts[ind];
			ine++;
		}
		acnts[ind] = cnt - tcnt;
		ind = bstrt;
		ine = prv(ind);
		inf = 0;
		tcnt = 0;
		while (ine > bfin) {
			bcnts[inf] = ((Lengths[ind] - Lengths[ine]) / blen)*cnt + 0.5;
			tcnt += bcnts[inf++];
			ine = prv(--ind);
		}
		bcnts[inf] = cnt - tcnt;
		apnt.x = CurrentFormVertices[astrt].x;
		apnt.y = CurrentFormVertices[astrt].y;
		anxt = nxt(astrt);
		bprv = prv(bstrt);
		acnt = acnts[pacnt++];
		bcnt = bcnts[pbcnt++];
		acind = astrt;
		bcind = bstrt;
		adif.x = CurrentFormVertices[anxt].x - CurrentFormVertices[acind].x;
		adif.y = CurrentFormVertices[anxt].y - CurrentFormVertices[acind].y;
		if (bcind == VertexCount) {
			bdif.x = CurrentFormVertices[bprv].x - CurrentFormVertices[0].x;
			bdif.y = CurrentFormVertices[bprv].y - CurrentFormVertices[0].y;
			bpnt.x = CurrentFormVertices[0].x;
			bpnt.y = CurrentFormVertices[0].y;
		}
		else {
			bdif.x = CurrentFormVertices[bprv].x - CurrentFormVertices[bcind].x;
			bdif.y = CurrentFormVertices[bprv].y - CurrentFormVertices[bcind].y;
			bpnt.x = CurrentFormVertices[bcind].x;
			bpnt.y = CurrentFormVertices[bcind].y;
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
					BSequence[SequenceIndex].attribute = 0;
					BSequence[SequenceIndex].x = apnt.x;
					BSequence[SequenceIndex++].y = apnt.y;
				}
				else {
					BSequence[SequenceIndex].attribute = 1;
					BSequence[SequenceIndex].x = bpnt.x;
					BSequence[SequenceIndex++].y = bpnt.y;
				}
				if (SequenceIndex > MAXSEQ - 6) {
					SequenceIndex = MAXSEQ - 6;
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
						BSequence[SequenceIndex].attribute = 0;
						BSequence[SequenceIndex].x = apnt.x;
						BSequence[SequenceIndex++].y = apnt.y;
						BSequence[SequenceIndex].attribute = 1;
						BSequence[SequenceIndex].x = bpnt.x;
						BSequence[SequenceIndex++].y = bpnt.y;
					}
					else {
						BSequence[SequenceIndex].attribute = 2;
						BSequence[SequenceIndex].x = bpnt.x;
						BSequence[SequenceIndex++].y = bpnt.y;
						BSequence[SequenceIndex].attribute = 3;
						BSequence[SequenceIndex].x = apnt.x;
						BSequence[SequenceIndex++].y = apnt.y;
					}
					if (SequenceIndex > MAXSEQ - 6) {
						SequenceIndex = MAXSEQ - 6;
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
				adif.x = CurrentFormVertices[anxt].x - CurrentFormVertices[acind].x;
				adif.y = CurrentFormVertices[anxt].y - CurrentFormVertices[acind].y;
				acind = nxt(acind);
				astp.x = adif.x / acnt;
				astp.y = adif.y / acnt;
			}
			if (!bcnt) {
				bcnt = bcnts[pbcnt++];
				bprv = prv(bcind);
				bdif.x = CurrentFormVertices[bprv].x - CurrentFormVertices[bcind].x;
				bdif.y = CurrentFormVertices[bprv].y - CurrentFormVertices[bcind].y;
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
	satfn(ind, CurrentFormGuides[0].start, VertexCount, CurrentFormGuides[0].finish);
	for (ind = 0; ind < (unsigned)CurrentFormConnectionsCount - 1; ind++)
		satfn(CurrentFormGuides[ind].start, CurrentFormGuides[ind + 1].start, CurrentFormGuides[ind].finish, CurrentFormGuides[ind + 1].finish);
	if (WordParam)
		satfn(CurrentFormGuides[ind].start, WordParam, CurrentFormGuides[ind].finish, WordParam + 1);
	else {
		if (CurrentFormGuides[ind].finish - CurrentFormGuides[ind].start > 2) {
			len = (Lengths[CurrentFormGuides[ind].finish] - Lengths[CurrentFormGuides[ind].start]) / 2 + Lengths[CurrentFormGuides[ind].start];
			ine = CurrentFormGuides[ind].start;
			while (len > Lengths[ine])
				ine++;
			dx = Lengths[ine] - len;
			dy = len - Lengths[ine - 1];
			if (dy > dx)
				ine--;
			satfn(CurrentFormGuides[ind].start, ine, CurrentFormGuides[ind].finish, ine);
		}
		else
			satfn(CurrentFormGuides[ind].start, CurrentFormGuides[ind].start + 1, CurrentFormGuides[ind].finish, CurrentFormGuides[ind].start + 1);
	}
}

extern void prbug();

void satfil() {
	unsigned		ind;
	double			len, dx, dy;
	double			tspac;

	fvars(ClosestFormToCursor);
	satadj();
	tspac = StitchSpacing;
	StitchSpacing /= 2;
	SequenceIndex = 0;
	rstMap(SAT1);
	rstMap(FILDIR);
	SelectedForm->fillType = SATF;
	Lengths = new double[VertexCount + 2];
	len = 0;
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++) {
		Lengths[ind] = len;
		dx = CurrentFormVertices[ind + 1].x - CurrentFormVertices[ind].x;
		dy = CurrentFormVertices[ind + 1].y - CurrentFormVertices[ind].y;
		len += hypot(dx, dy);
	}
	Lengths[ind] = len;
	dx = CurrentFormVertices[0].x - CurrentFormVertices[ind].x;
	dy = CurrentFormVertices[0].y - CurrentFormVertices[ind].y;
	len += hypot(dx, dy);
	Lengths[ind + 1] = len;
	if (WordParam) {
		if (CurrentFormConnectionsCount) {
			satmf();
			goto satdun;
		}
		else {
			satfn(1, WordParam, VertexCount, WordParam + 1);
			goto satdun;
		}
	}
	if (SelectedForm->attribute&FRMEND) {
		if (CurrentFormConnectionsCount) {
			satmf();
			goto satdun;
		}
		else {
			if (VertexCount == 3 && FormList[ClosestFormToCursor].attribute & 1) {
				satfn(2, 3, 2, 1);
				goto satdun;
			}
			else {
				len = (len - Lengths[1]) / 2;
				ind = 1;
				if (!chkMap(BARSAT)) {
					OSequence[0].x = SelectedPoint.x = CurrentFormVertices[1].x;
					OSequence[0].y = SelectedPoint.y = CurrentFormVertices[1].y;
					SequenceIndex = 1;
				}
				while ((len > Lengths[ind]) && (ind < (unsigned int)(VertexCount + 1)))
					ind++;
				dx = Lengths[ind] - len;
				dy = len - Lengths[ind - 1];
				if (dy > dx)
					ind--;
				satfn(1, ind, VertexCount, ind);
			}
			goto satdun;
		}
	}
	if (CurrentFormConnectionsCount) {
		satmf();
		goto satdun;
	}
	len /= 2;
	ind = 0;
	if (!chkMap(BARSAT) && !chkMap(FTHR)) {
		OSequence[0].x = SelectedPoint.x = CurrentFormVertices[0].x;
		OSequence[0].y = SelectedPoint.y = CurrentFormVertices[0].y;
		SequenceIndex = 1;
	}
	while (len > Lengths[ind])
		ind++;
	dx = Lengths[ind] - len;
	dy = len - Lengths[ind - 1];
	if (dy > dx)
		ind--;
	satfn(0, ind, VertexCount, ind);
satdun:;

	delete[] Lengths;
	StitchSpacing = tspac;
}

void filsfn() {
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->type = SAT;
	fsizpar();
	SelectedForm->fillType = SATF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->fillSpacing = StitchSpacing;
	SelectedForm->type = SAT;
	refilfn();
}

void filsat() {
	unsigned ind;

	if (filmsgs(FMM_FAN))
		return;
	if (SelectedFormCount) {
		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
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
	for (ind = 0; ind < FormIndex; ind++) {
		if (!ActiveLayer || (unsigned)((FormList[ind].attribute&FRMLMSK) >> 1) == ActiveLayer || !(FormList[ind].attribute&FRMLMSK)) {
			CurrentFormVertices = FormList[ind].vertices;
			for (ine = 0; ine < FormList[ind].vertexCount; ine++) {
				dx = SelectedPoint.x - CurrentFormVertices[ine].x;
				dy = SelectedPoint.y - CurrentFormVertices[ine].y;
				tlen = hypot(dx, dy);
				if (tlen < len) {
					len = tlen;
					ClosestFormToCursor = ind;
					ClosestVertexToCursor = ine;
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
	fltspac(&FormForInsert->vertices[pnu], 1);
	FormForInsert->vertices[pnu].x = SelectedPoint.x;
	FormForInsert->vertices[pnu].y = SelectedPoint.y;
	FormForInsert->vertexCount++;
	for (ind = 0; ind < FormForInsert->satinGuideCount; ind++) {
		if (FormForInsert->satinOrAngle.guide[ind].start > pnu - 1)
			FormForInsert->satinOrAngle.guide[ind].start++;
		if (FormForInsert->satinOrAngle.guide[ind].finish > pnu - 1)
			FormForInsert->satinOrAngle.guide[ind].finish++;
	}
	if (FormForInsert->wordParam >= pnu) {
		FormForInsert->wordParam++;
		FormForInsert->wordParam %= VertexCount;
	}
	if (FormForInsert->fillType == CONTF) {
		if (FormForInsert->angleOrClipData.guide.start > pnu - 1)
			FormForInsert->angleOrClipData.guide.start++;
		if (FormForInsert->angleOrClipData.guide.finish > pnu - 1)
			FormForInsert->angleOrClipData.guide.finish++;
	}
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
}

double p2p(fPOINT pnt0, fPOINT pnt1) {
	return hypot(pnt0.x - pnt1.x, pnt0.y - pnt1.y);
}

unsigned upsat() {
	unsigned	ind;
	double		dwnlen, uplen, p2clos, p2up, p2dwn;

	VertexCount = FormForInsert->vertexCount;
	CurrentFormVertices = FormForInsert->vertices;
	p2clos = p2p(SelectedPoint, CurrentFormVertices[ClosestVertexToCursor]);
	ind = prv(ClosestVertexToCursor);
	dwnlen = p2p(CurrentFormVertices[ind], CurrentFormVertices[ClosestVertexToCursor]);
	p2dwn = p2p(CurrentFormVertices[ind], SelectedPoint);
	ind = nxt(ClosestVertexToCursor);
	uplen = p2p(CurrentFormVertices[ind], CurrentFormVertices[ClosestVertexToCursor]);
	p2up = p2p(CurrentFormVertices[ind], SelectedPoint);
	if ((p2dwn + p2clos) / dwnlen > (p2up + p2clos) / uplen)
		return 0;
	else {
		return 1;
	}
}

void insat() {
	if (closat()) {
		savdo();
		SelectedForm = &FormList[ClosestFormToCursor];
		FormForInsert = SelectedForm;
		fvars(ClosestFormToCursor);
		if (upsat()) {
			if (!ClosestVertexToCursor&&FormForInsert->type == FRMLINE)
				setMap(PRELIN);
			else
				ClosestVertexToCursor = prv(ClosestVertexToCursor);
			nufpnt(ClosestVertexToCursor);
			if (rstMap(PRELIN)) {
				SelectedPoint.x = FormForInsert->vertices[0].x;
				SelectedPoint.y = FormForInsert->vertices[0].y;
				FormForInsert->vertices[0].x = FormForInsert->vertices[1].x;
				FormForInsert->vertices[0].y = FormForInsert->vertices[1].y;
				FormForInsert->vertices[1].x = SelectedPoint.x;
				FormForInsert->vertices[1].y = SelectedPoint.y;
			}
		}
		else
			nufpnt(ClosestVertexToCursor);
		refil();
	}
	setMap(RESTCH);
}

BOOL chkdel() {
	if (SelectedForm->type == FRMLINE) {
		if (SelectedForm->vertexCount > 2)
			return 0;
		else
			return 1;
	}
	else {
		if (SelectedForm->vertexCount > 3)
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

	fvars(ClosestFormToCursor);
	if (chkdel()) {
		setMap(DELTO);
		frmdel();
		rstMap(FRMPSEL);
		coltab();
		setMap(RESTCH);
		return;
	}
	if (SelectedForm->type == SAT) {
		if (ClosestVertexToCursor < SelectedForm->wordParam)
			SelectedForm->wordParam--;
		if (SelectedForm->satinGuideCount) {
			l_sac = SelectedForm->satinOrAngle.guide;
			ind = 0;
			while (l_sac[ind].start != ClosestVertexToCursor&&l_sac[ind].finish != ClosestVertexToCursor&&ind < SelectedForm->satinGuideCount)
				ind++;
			if (ind < SelectedForm->satinGuideCount && (l_sac[ind].start == ClosestVertexToCursor || l_sac[ind].finish == ClosestVertexToCursor)) {
				while (ind < SelectedForm->satinGuideCount) {
					l_sac[ind].start = l_sac[ind + 1].start;
					l_sac[ind].finish = l_sac[ind + 1].finish;
					ind++;
				}
				SelectedForm->satinGuideCount--;
				SatinConnectIndex--;
				for (ine = ClosestFormToCursor + 1; ine < FormIndex; ine++)
				{
					fp = &FormList[ine];
					if (fp->type == SAT&&fp->satinGuideCount)
						fp->satinOrAngle.guide++;
				}
			}
			for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {
				if (l_sac[ind].start > ClosestVertexToCursor)
					l_sac[ind].start--;
				if (l_sac[ind].finish > ClosestVertexToCursor)
					l_sac[ind].finish--;
			}
		}
	}
	MoveMemory(&SelectedForm->vertices[ClosestVertexToCursor], &SelectedForm->vertices[ClosestVertexToCursor + 1], (FormVertexIndex - ClosestVertexToCursor) * sizeof(fPOINTATTR));
	SelectedForm->vertexCount--;
	FormVertexIndex--;
	fvars(ClosestFormToCursor);
	if (ClosestVertexToCursor > (unsigned)SelectedForm->vertexCount - 1)
		ClosestVertexToCursor = SelectedForm->vertexCount - 1;
	setMap(FRMPSEL);
	for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
		FormList[ind].vertices--;
	ritfcor(&CurrentFormVertices[ClosestVertexToCursor]);
	ritnum(STR_NUMPNT, ClosestVertexToCursor);
	frmout(ClosestFormToCursor);
	tflt = &SelectedForm->vertices[ClosestVertexToCursor];
	if (tflt->x<ZoomRect.left || tflt->x>ZoomRect.right || tflt->y<ZoomRect.bottom || tflt->y>ZoomRect.top)
		shft(SelectedForm->vertices[ClosestVertexToCursor]);
	refil();
}

void unfil() {
	unsigned src, dst, trg, at;
	unsigned mlen;

	if (filmsgs(FMX_UNF))
		return;
	if (SelectedFormCount) {
		mlen = (SelectedFormCount >> 5) + 1;
		for (src = 0; src < mlen; src++)
			MarkedStitchMap[src] = 0;
		for (src = 0; src < SelectedFormCount; src++) {
			SelectedForm = &FormList[SelectedFormList[src]];
			if (SelectedForm->fillType || SelectedForm->edgeType) {
				delclps(SelectedFormList[src]);
				deltx();
				setr(SelectedFormList[src]);
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
				trg = (ClosestFormToCursor << FRMSHFT) | USMSK;
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
			trg = ClosestFormToCursor << FRMSHFT;
			dst = 0;
			for (src = 0; src < PCSHeader.stitchCount; src++) {
				if ((StitchBuffer[src].attribute&FRMSK) != trg || (StitchBuffer[src].attribute&NOTFRM)) {
					StitchBuffer[dst].attribute = StitchBuffer[src].attribute;
					StitchBuffer[dst].x = StitchBuffer[src].x;
					StitchBuffer[dst++].y = StitchBuffer[src].y;
				}
			}
			delclps(ClosestFormToCursor);
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
	VertexCount = SatinIndex;
	frmlin(TempPolygon, SatinIndex);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	Polyline(StitchWindowMemDC, FormLines, SatinIndex);
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

	fvars(ClosestFormToCursor);
	flt0 = SelectedForm->vertices;
	VertexCount = SelectedForm->vertexCount;
	flt1 = new fPOINT[VertexCount];
	for (ind = 0; ind < VertexCount; ind++) {
		flt1[ind].x = flt0[ind].x;
		flt1[ind].y = flt0[ind].y;
	}
	for (ind = 0; ind < VertexCount; ind++) {
		flt0[ind].x = flt1[l_xpnt].x;
		flt0[ind].y = flt1[l_xpnt].y;
		l_xpnt = nxt(l_xpnt);
	}
	ine = 0;
	if (SelectedForm->type == SAT) {
		if (SelectedForm->wordParam)
			SelectedForm->wordParam = (SelectedForm->wordParam + SelectedForm->vertexCount
				- nu0) % SelectedForm->vertexCount;
		for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {
			if (CurrentFormGuides[ind].start != nu0&&CurrentFormGuides[ind].finish != nu0) {
				CurrentFormGuides[ine].start = (CurrentFormGuides[ind].start + VertexCount - nu0) % VertexCount;
				CurrentFormGuides[ine].finish = (CurrentFormGuides[ind].finish + VertexCount - nu0) % VertexCount;
				if (CurrentFormGuides[ine].start > CurrentFormGuides[ine].finish) {
					tlin = CurrentFormGuides[ine].start;
					CurrentFormGuides[ine].start = CurrentFormGuides[ine].finish;
					CurrentFormGuides[ind].finish = tlin;
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
		nsac[ind].start = CurrentFormGuides[ind].start;
		nsac[ind].finish = CurrentFormGuides[ind].finish;
	}
	qsort((void*)psac, ine, 4, scomp);
	for (ind = 0; ind < ine; ind++) {
		CurrentFormGuides[ind].start = psac[ind]->start;
		CurrentFormGuides[ind].finish = psac[ind]->finish;
	}
	if (SelectedForm->extendedAttribute&AT_STRT)
		SelectedForm->fillStart = (SelectedForm->fillStart + VertexCount - nu0) % VertexCount;
	if (SelectedForm->extendedAttribute&AT_END)
		SelectedForm->fillEnd = (SelectedForm->fillEnd + VertexCount - nu0) % VertexCount;
	delete[] flt1;
	delete[] nsac;
	delete[] psac;
}


void frm0() {
	if (chkMap(FRMPSEL)) {
		savdo();
		rotfrm(ClosestVertexToCursor);
		ClosestVertexToCursor = 0;
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
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
	SelectObject(StitchWindowMemDC, FormPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	if (FormVertexNext || FormForInsert->type != FRMLINE)
		Polyline(StitchWindowMemDC, &FormLines[FormVertexPrev], 2);
	InsertLine[0].x = FormLines[FormVertexPrev].x;
	InsertLine[0].y = FormLines[FormVertexPrev].y;
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	setMap(SHOINSF);
	duinsf();
}

void infrm() {
	if (closat()) {
		FormForInsert = &FormList[ClosestFormToCursor];
		fvars(ClosestFormToCursor);
		if (upsat()) {
			if (!ClosestVertexToCursor&&FormForInsert->type == FRMLINE) {
				FormVertexPrev = 0;
				setMap(PRELIN);
			}
			else {
				FormVertexPrev = prv(ClosestVertexToCursor);
				FormVertexNext = ClosestVertexToCursor;
			}
		}
		else {
			FormVertexPrev = ClosestVertexToCursor;
			FormVertexNext = nxt(ClosestVertexToCursor);
		}
		setMap(INSFRM);
		setMap(INIT);
		rinfrm();
	}
}

void setins() {
	px2stch();
	nufpnt(FormVertexPrev);
	if (chkMap(PRELIN)) {
		SelectedPoint.x = FormForInsert->vertices[0].x;
		SelectedPoint.y = FormForInsert->vertices[0].y;
		FormForInsert->vertices[0].x = FormForInsert->vertices[1].x;
		FormForInsert->vertices[0].y = FormForInsert->vertices[1].y;
		FormForInsert->vertices[1].x = SelectedPoint.x;
		FormForInsert->vertices[1].y = SelectedPoint.y;
	}
	else {
		VertexCount = FormForInsert->vertexCount;
		FormVertexPrev = nxt(FormVertexPrev);
		FormVertexNext = nxt(FormVertexPrev);
	}
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
	InsertLine[0].x = FormLines[FormVertexPrev].x;
	InsertLine[0].y = FormLines[FormVertexPrev].y;
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
	dif.x = CurrentFormVertices[fin].x - CurrentFormVertices[strt].x;
	dif.y = CurrentFormVertices[fin].y - CurrentFormVertices[strt].y;
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
		pnt.x = CurrentFormVertices[strt].x + stp.x;
		pnt.y = CurrentFormVertices[strt].y + stp.y;
		cnt--;
		while (cnt) {
			OSequence[SequenceIndex].x = pnt.x;
			OSequence[SequenceIndex++].y = pnt.y;
			pnt.x += stp.x;
			pnt.y += stp.y;
			cnt--;
		}
	}
	OSequence[SequenceIndex].x = CurrentFormVertices[fin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[fin].y;
}

void brdfil(double pd_Size) {
	unsigned		ind;
	unsigned short	nlin, tlin;

	if (SelectedForm->extendedAttribute&AT_STRT)
		tlin = SelectedForm->fillStart;
	else
		tlin = getlast();
	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++) {
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
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGELINE;
	bsizpar();
	refilfn();
}

void bord() {
	unsigned ind;

	if (filmsgs(FML_LIN))
		return;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
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

	if (chkmax(ClipStitchCount, SequenceIndex))
		return 1;
	adj.x = pnt.x - ClipReference.x;
	adj.y = pnt.y - ClipReference.y;
	for (ind = 0; ind < ClipStitchCount; ind++) {
		OSequence[SequenceIndex].x = ClipFillData[ind].x + adj.x;
		OSequence[SequenceIndex++].y = ClipFillData[ind].y + adj.y;
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
	pnt.x = CurrentFormVertices[strt].x;
	pnt.y = CurrentFormVertices[fin].y;
	dif.x = CurrentFormVertices[fin].x - CurrentFormVertices[strt].x;
	dif.y = CurrentFormVertices[fin].y - CurrentFormVertices[strt].y;
	len = hypot(dif.x, dif.y);
	RotationAngle = atan2(dif.y, dif.x);
	rotang1(rpnt, &ClipReference);
	cnt = len / ClipRectSize.cx;
	if (cnt) {
		if (cnt > 1)
			tdub = ((len - cnt*ClipRectSize.cx) / (cnt - 1) + ClipRectSize.cx) / len;
		else
			tdub = (len - ClipRectSize.cx) / 2;
		stp.x = dif.x*tdub;
		stp.y = dif.y*tdub;
		pnt.x = CurrentFormVertices[strt].x;
		pnt.y = CurrentFormVertices[strt].y;
		RotationAngle = atan2(dif.y, dif.x);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(ClipReversedData[ind], &ClipFillData[ind]);
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

	dif.x = CurrentFormVertices[CurrentSide + 1].x - SelectedPoint.x;
	dif.y = CurrentFormVertices[CurrentSide + 1].y - SelectedPoint.y;
	len = hypot(dif.x, dif.y);
	cnt = len / ClipRectSize.cx;
	if (cnt) {
		RotationAngle = ClipAngle;
		rotangf(BorderClipReference, &ClipReference);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(ClipReversedData[ind], &ClipFillData[ind]);
		for (ind = 0; ind < cnt; ind++) {
			ritclp(SelectedPoint);
			SelectedPoint.x += Vector0.x;
			SelectedPoint.y += Vector0.y;
		}
	}
}

BOOL nupnt() {
	double		len, dif;
	unsigned	ind;

	MoveToCoords.x = CurrentFormVertices[CurrentSide + 2].x;
	MoveToCoords.y = CurrentFormVertices[CurrentSide + 2].y;
	len = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
	if (len > ClipRectSize.cx) {
		for (ind = 0; ind < 10; ind++) {
			len = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
			dif = ClipRectSize.cx - len;
			MoveToCoords.x += dif*CosAngle;
			MoveToCoords.y += dif*SinAngle;
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

	SinAngle = sin(ClipAngle);
	CosAngle = cos(ClipAngle);
	if (nupnt()) {
		dif.x = MoveToCoords.x - SelectedPoint.x;
		dif.y = MoveToCoords.y - SelectedPoint.y;
		RotationAngle = atan2(dif.y, dif.x);
		rotangf(BorderClipReference, &ClipReference);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(ClipReversedData[ind], &ClipFillData[ind]);
		ritclp(SelectedPoint);
		SelectedPoint.x = MoveToCoords.x;
		SelectedPoint.y = MoveToCoords.y;
	}
}

void durev() {
	unsigned	ind;
	double		mid;

	mid = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	if (ClipBuffer[0].x > mid) {
		for (ind = 0; ind < ClipStitchCount; ind++) {
			ClipReversedData[ind].x = ClipRect.right - ClipBuffer[ind].x;
			ClipReversedData[ind].y = ClipBuffer[ind].y;
		}
	}
	else {
		for (ind = 0; ind < ClipStitchCount; ind++) {
			ClipReversedData[ind].x = ClipBuffer[ind].x;
			ClipReversedData[ind].y = ClipBuffer[ind].y;
		}
	}
}

void setvct(unsigned strt, unsigned fin) {
	ClipAngle = atan2(CurrentFormVertices[fin].y - CurrentFormVertices[strt].y, CurrentFormVertices[fin].x - CurrentFormVertices[strt].x);
	Vector0.x = ClipRectSize.cx*cos(ClipAngle);
	// ToDo - is this line below correct?
	Vector0.y = ClipRectSize.cx*sin(ClipAngle);
}

void clpbrd(unsigned short strtlin) {
	unsigned		ind, ref;
	unsigned short	nlin;

	SequenceIndex = 0;
	rstMap(CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength = ClipRectSize.cx;
	ClipFillData = new fPOINT[ClipStitchCount];
	ClipReversedData = new fPOINT[ClipStitchCount];
	RotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	ClipReference.y = RotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	ClipReference.x = ClipRect.left;
	durev();
	ref = 0;
	if (SelectedForm->type == FRMLINE) {
		SelectedPoint.x = CurrentFormVertices[0].x;
		SelectedPoint.y = CurrentFormVertices[0].y;
		setvct(0, 1);
		BorderClipReference.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
		// ToDo - is this line below correct?
		BorderClipReference.x = (ClipRect.top = ClipRect.left) / 2 + ClipRect.left;
		for (CurrentSide = 0; CurrentSide < (unsigned)VertexCount - 2; CurrentSide++) {
			linsid();
			setvct(CurrentSide + 1, CurrentSide + 2);
			lincrnr();
		}
		linsid();
	}
	else {
		clpout();
		ref = strtlin;
		for (ind = 0; ind < VertexCount; ind++) {
			nlin = prv(strtlin);
			if (clpsid(ref, nlin))
				ref = nlin;
			strtlin = nlin;
		}
	}
	delete[] ClipFillData;
	delete[] ClipReversedData;
}

void outfn(unsigned strt, unsigned fin, double satwid) {
	double		l_ang;
	double		len;
	double		xOffset, yOffset;

	if (fabs(FormAngles[strt]) < TINY && fabs(FormAngles[fin]) < TINY) {
		xOffset = 0;
		yOffset = satwid;
	} else {
#define SATHRESH 10

		l_ang = (FormAngles[fin] - FormAngles[strt]) / 2;
		len = satwid / cos(l_ang);
		if (len < -satwid*SATHRESH)
			len = -satwid*SATHRESH;
		if (len > satwid*SATHRESH)
			len = satwid*SATHRESH;
		l_ang += FormAngles[strt] + PI / 2;
		xOffset = len*cos(l_ang);
		yOffset = len*sin(l_ang);
	}
	InsidePoints[fin].x = CurrentFormVertices[fin].x - xOffset;
	InsidePoints[fin].y = CurrentFormVertices[fin].y - yOffset;
	OutsidePoints[fin].x = CurrentFormVertices[fin].x + xOffset;
	OutsidePoints[fin].y = CurrentFormVertices[fin].y + yOffset;
}

void duangs() {
	unsigned ind;

	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++)
		FormAngles[ind] = atan2(CurrentFormVertices[ind + 1].y - CurrentFormVertices[ind].y, CurrentFormVertices[ind + 1].x - CurrentFormVertices[ind].x);
	FormAngles[ind] = atan2(CurrentFormVertices[0].y - CurrentFormVertices[ind].y, CurrentFormVertices[0].x - CurrentFormVertices[ind].x);
}

void satout(double satwid) {
	unsigned	ind;
	unsigned	cnt;

	if (VertexCount) {
		duangs();
		OutsidePoints = OutsidePointList;
		InsidePoints = InsidePointList;
		for (ind = 0; ind < (unsigned)VertexCount - 1; ind++)
			outfn(ind, ind + 1, 0.1);
		cnt = 0;
		for (ind = 0; ind < VertexCount; ind++)
		{
			if (cisin(InsidePoints[ind].x, InsidePoints[ind].y))
				cnt++;
		}
		satwid /= 2;
		for (ind = 0; ind < (unsigned)VertexCount - 1; ind++)
			outfn(ind, ind + 1, satwid);
		outfn(ind, 0, satwid);
		rstMap(INDIR);
		if (cnt < (unsigned)VertexCount >> 1)
		{
			setMap(INDIR);
			OutsidePoints = InsidePointList;
			InsidePoints = OutsidePointList;
		}
	}
}

void clpout() {
	if (SelectedForm->type == FRMLINE)
		satout(HorizontalLength2);
	else {
		satout(ClipRectSize.cy);
		InsidePoints = SelectedForm->vertices;
	}
}

void fsclp() {
	unsigned	ind;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGECLIP;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = ClipRectSize.cx;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = ClipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = ClipBuffer[ind].y;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	clpout();
	refilfn();
}

void fclp() {
	unsigned ind;

	if (filmsgs(FML_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		fvars(ClosestFormToCursor);
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			savdo();
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (SelectedFormCount) {
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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
		if (chkmax(cnt, SequenceIndex))
			return;
		while (cnt) {
			SelectedPoint.x += stp.x;
			SelectedPoint.y += stp.y;
			OSequence[SequenceIndex].x = SelectedPoint.x;
			OSequence[SequenceIndex++].y = SelectedPoint.y;
			cnt--;
		}
	}
	if (SequenceIndex & 0xffff0000)
		return;
	OSequence[SequenceIndex].x = pnt.x;
	OSequence[SequenceIndex++].y = pnt.y;
	SelectedPoint.x = pnt.x;
	SelectedPoint.y = pnt.y;
}

BOOL chkbak(dPOINT pnt) {
	unsigned ind;
	double		len;

	for (ind = 0; ind < 8; ind++) {
		len = hypot(SatinBackup[ind].x - pnt.x, SatinBackup[ind].y - pnt.y);
		if (len < StitchSpacing)
			return 1;
	}
	return 0;
}

BOOL linx(fPOINT* p_flt, unsigned strt, unsigned fin, dPOINT* npnt) {
	dPOINT	dif;
	dPOINT	tdub;

	dif.x = OutsidePoints[strt].x - p_flt[strt].x;
	dif.y = OutsidePoints[strt].y - p_flt[strt].y;
	if (!dif.x && !dif.y)
		return 0;
	tdub.x = p_flt[strt].x;
	tdub.y = p_flt[strt].y;
	if (dif.x) {
		if (proj(tdub, dif.y / dif.x, OutsidePoints[fin], p_flt[fin], npnt))
			return 1;
		else
			return 0;
	}
	else {
		if (projv(tdub.x, p_flt[fin], OutsidePoints[fin], npnt))
			return 1;
		else
			return 0;
	}
}

void filinsbw(dPOINT pnt) {
	SatinBackup[SatinBackupIndex].x = pnt.x;
	SatinBackup[SatinBackupIndex++].y = pnt.y;
	SatinBackupIndex &= 0x7;
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
	odif.x = OutsidePoints[finish].x - OutsidePoints[start].x;
	odif.y = OutsidePoints[finish].y - OutsidePoints[start].y;
	ilen = hypot(idif.x, idif.y);
	olen = hypot(odif.x, odif.y);
	ipnt.x = p_flt[start].x;
	ipnt.y = p_flt[start].y;
	l_opnt.x = OutsidePoints[start].x;
	l_opnt.y = OutsidePoints[start].y;
	xflg = SatinBackupIndex = iflg = oflg = bcnt = 0;
	for (ind = 0; ind < 8; ind++) {
		SatinBackup[ind].x = (float)1e12;
		SatinBackup[ind].y = (float)1e12;
	}
	if (olen > ilen) {
		cnt = olen / StitchSpacing;
		iflg = 1;
		if (linx(p_flt, start, finish, &npnt)) {
			xflg = 1;
			idif.x = idif.y = ilen = 0;
			ipnt.x = npnt.x;
			ipnt.y = npnt.y;
		}
	}
	else {
		cnt = ilen / StitchSpacing;
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
	if (chkmax(cnt, SequenceIndex))
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
				bcnt = blen / StitchSpacing;
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
				bcnt = blen / StitchSpacing;
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

	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
		nextLine = nxt(startLine);
		sbfn(InsidePoints, startLine, nextLine);
		startLine = nextLine;
	}
	OSequence[0].x = OSequence[SequenceIndex - 1].x;
	OSequence[0].y = OSequence[SequenceIndex - 1].y;
	if (SequenceIndex > MAXSEQ - 2)
		SequenceIndex = MAXSEQ - 2;
}

void sbrd() {
	double			tspac;
	unsigned		strt;

	strt = getlast();
	tspac = StitchSpacing;
	rstMap(SAT1);
	rstMap(FILDIR);
	SequenceIndex = 1;
	if (SelectedForm->edgeType&EGUND) {
		StitchSpacing = USPAC;
		satout(HorizontalLength2*URAT);
		sfn(strt);
		setMap(FILDIR);
		sfn(strt);
	}
	fvars(ClosestFormToCursor);
	satout(HorizontalLength2);
	StitchSpacing = SelectedForm->edgeSpacing;
	sfn(strt);
	StitchSpacing = tspac;
}

void rfn(unsigned tlin) {
	unsigned ind, nlin;

	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
		nlin = nxt(tlin);
		sbfn(InsidePoints, tlin, nlin);
		tlin = nlin;
	}
}

void rbrd() {
	unsigned short	tlin = getlast();
	double			tspac;

	rstMap(SAT1);
	rstMap(FILDIR);
	tspac = StitchSpacing;
	StitchSpacing = USPAC;
	rfn(tlin);
	setMap(FILDIR);
	rfn(tlin);
	satout(HorizontalLength2);
	StitchSpacing = tspac;
	rfn(tlin);
}

void satends(unsigned blnt) {
	fPOINT		step;

	if (blnt&SBLNT) {
		step.x = sin(FormAngles[0])*HorizontalLength2 / 2;
		step.y = cos(FormAngles[0])*HorizontalLength2 / 2;
		if (chkMap(INDIR))
		{
			step.x = -step.x;
			step.y = -step.y;
		}
		InsidePoints[0].x = SelectedForm->vertices[0].x + step.x;
		InsidePoints[0].y = SelectedForm->vertices[0].y - step.y;
		OutsidePoints[0].x = SelectedForm->vertices[0].x - step.x;
		OutsidePoints[0].y = SelectedForm->vertices[0].y + step.y;
	}
	else {
		InsidePoints[0].x = OutsidePoints[0].x = CurrentFormVertices[0].x;
		InsidePoints[0].y = OutsidePoints[0].y = CurrentFormVertices[0].y;
	}
	if (blnt&FBLNT) {
		step.x = sin(FormAngles[VertexCount - 2])*HorizontalLength2 / 2;
		step.y = cos(FormAngles[VertexCount - 2])*HorizontalLength2 / 2;
		if (chkMap(INDIR))
		{
			step.x = -step.x;
			step.y = -step.y;
		}
		InsidePoints[VertexCount - 1].x = SelectedForm->vertices[VertexCount - 1].x + step.x;
		InsidePoints[VertexCount - 1].y = SelectedForm->vertices[VertexCount - 1].y - step.y;
		OutsidePoints[VertexCount - 1].x = SelectedForm->vertices[VertexCount - 1].x - step.x;
		OutsidePoints[VertexCount - 1].y = SelectedForm->vertices[VertexCount - 1].y + step.y;
	}
	else {
		InsidePoints[VertexCount - 1].x = OutsidePoints[VertexCount - 1].x = CurrentFormVertices[VertexCount - 1].x;
		InsidePoints[VertexCount - 1].y = OutsidePoints[VertexCount - 1].y = CurrentFormVertices[VertexCount - 1].y;
	}
}

void slbrd() {
	unsigned	ind;
	double		tspac = StitchSpacing;

	SequenceIndex = 0;
	if (SelectedForm->edgeType&EGUND) {
		HorizontalLength2 = SelectedForm->borderSize*URAT;
		satout(HorizontalLength2);
		satends(SelectedForm->attribute);
		rstMap(SAT1);
		rstMap(FILDIR);
		StitchSpacing = USPAC;
		for (ind = 0; ind < (unsigned)SelectedForm->vertexCount - 1; ind++)
			sbfn(InsidePoints, ind, ind + 1);
		toglMap(FILDIR);
		for (ind = SelectedForm->vertexCount - 1; ind; ind--)
			sbfn(InsidePoints, ind, ind - 1);
	}
	HorizontalLength2 = SelectedForm->borderSize;
	satout(HorizontalLength2);
	satends(SelectedForm->attribute);
	StitchSpacing = SelectedForm->edgeSpacing;
	rstMap(SAT1);
	for (ind = 0; ind < (unsigned)SelectedForm->vertexCount - 1; ind++)
		sbfn(InsidePoints, ind, ind + 1);
	StitchSpacing = tspac;
}

void satsbrd() {
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEANGSAT;
	if (chku(DUND))
		SelectedForm->edgeType |= EGUND;
	bsizpar();
	SelectedForm->borderSize = BorderWidth;
	SelectedForm->edgeSpacing = StitchSpacing / 2;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void satbrd() {
	unsigned ind;

	if (filmsgs(FML_ANGS))
		return;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
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
			fvars(ClosestFormToCursor);
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

	SequenceIndex = 0;
	tsiz = UserStitchLength;
	UserStitchLength = APSPAC;
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++)
		bdrlin(ind, ind + 1, APSPAC);
	for (ind = VertexCount - 1; ind; ind--)
		bdrlin(ind, ind - 1, APSPAC);
	UserStitchLength = tsiz;
}

void apbrd() {
	unsigned		ind;
	unsigned short	nlin, tlin = 0;

	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	for (ind = 0; ind < (unsigned)VertexCount << 1; ind++) {
		nlin = nxt(tlin);
		bdrlin(tlin, nlin, APSPAC);
		tlin = nlin;
	}
}

void sapliq()
{
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEAPPL;
	if (chku(DUND))
		SelectedForm->edgeType |= EGUND;
	SelectedForm->edgeSpacing = StitchSpacing / 2;
	SelectedForm->borderSize = IniFile.borderWidth;
	bsizpar();
	SelectedForm->borderColor = ActiveColor | (AppliqueColor << 4);
	if (SelectedForm->type != FRMLINE)
	{
		if (SelectedForm->fillType == SAT&&SelectedForm->satinGuideCount)
			delsac(ClosestFormToCursor);
	}
	SelectedForm->fillType = 0;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void apliq() {
	unsigned ind;

	if (filmsgs(FML_APLQ))
		return;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
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

	AppliqueColor = ActiveColor;
	LoadString(ThrEdInstance, IDS_APCOL, buf, HBUFSIZ);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, AppliqueColor + 1);
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
	PreferenceWindowTextWidth = pnt.x + 6;
}

HWND txtwin(TCHAR* str, RECT loc) {
	if (chkMap(REFCNT))
	{
		maxtsiz(str, &LabelWindowSize);
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
		ThrEdInstance,
		NULL);
}

HWND txtrwin(TCHAR* str, RECT loc) {
	if (chkMap(REFCNT))
	{
		maxtsiz(str, &ValueWindowSize);
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
		ThrEdInstance,
		NULL);
}

HWND numwin(TCHAR* str, RECT loc) {
	if (chkMap(REFCNT))
	{
		maxtsiz(str, &ValueWindowSize);
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
		ThrEdInstance,
		NULL);
}

void nxtlin() {
	FormMenuEntryCount++;
	LabelWindowCoords.top += LabelWindowSize.y;
	LabelWindowCoords.bottom += LabelWindowSize.y;
	ValueWindowCoords.top += ValueWindowSize.y;
	ValueWindowCoords.bottom += ValueWindowSize.y;;
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
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left = 3;
	LabelWindowCoords.right = 3 + LabelWindowSize.x;
	ValueWindowCoords.left = 6 + LabelWindowSize.x;
	ValueWindowCoords.right = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	LabelWindow[LFRM] = txtwin(StringTable[STR_TXT0], LabelWindowCoords);
	if (SelectedForm->type == FRMLINE)
		ValueWindow[LFRM] = txtrwin(StringTable[STR_EDG1], ValueWindowCoords);
	else
		ValueWindow[LFRM] = txtrwin(StringTable[STR_FREH], ValueWindowCoords);
	nxtlin();
	LabelWindow[LLAYR] = txtwin(StringTable[STR_TXT1], LabelWindowCoords);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", (SelectedForm->attribute&FRMLMSK) >> 1);
	ValueWindow[LLAYR] = txtrwin(MsgBuffer, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->type != FRMLINE)
	{
		LabelWindow[LCWLK] = txtwin(StringTable[STR_CWLK], LabelWindowCoords);
		if (SelectedForm->extendedAttribute&AT_CWLK)
			ValueWindow[LCWLK] = txtrwin(StringTable[STR_ON], ValueWindowCoords);
		else
			ValueWindow[LCWLK] = txtrwin(StringTable[STR_OFF], ValueWindowCoords);
		nxtlin();
		LabelWindow[LWALK] = txtwin(StringTable[STR_WALK], LabelWindowCoords);
		if (SelectedForm->extendedAttribute&AT_WALK)
			ValueWindow[LWALK] = txtrwin(StringTable[STR_ON], ValueWindowCoords);
		else
			ValueWindow[LWALK] = txtrwin(StringTable[STR_OFF], ValueWindowCoords);
		nxtlin();
		LabelWindow[LUND] = txtwin(StringTable[STR_UND], LabelWindowCoords);
		if (SelectedForm->extendedAttribute&AT_UND)
			ValueWindow[LUND] = txtrwin(StringTable[STR_ON], ValueWindowCoords);
		else
			ValueWindow[LUND] = txtrwin(StringTable[STR_OFF], ValueWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&(AT_WALK | AT_UND | AT_CWLK))
		{
			LabelWindow[LUNDCOL] = txtwin(StringTable[STR_UNDCOL], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->underlayColor + 1);
			ValueWindow[LUNDCOL] = txtrwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
			LabelWindow[LULEN] = txtwin(StringTable[STR_ULEN], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlayStitchLen / PFGRAN);
			ValueWindow[LULEN] = txtrwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		LabelWindow[LWLKIND] = txtwin(StringTable[STR_UWLKIND], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlayIndent / PFGRAN);
		ValueWindow[LWLKIND] = txtrwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&AT_UND)
		{
			LabelWindow[LUSPAC] = txtwin(StringTable[STR_FUSPAC], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlaySpacing / PFGRAN);
			ValueWindow[LUSPAC] = txtrwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
			LabelWindow[LUANG] = txtwin(StringTable[STR_FUANG], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->underlayStitchAngle * 180 / PI);
			ValueWindow[LUANG] = txtrwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
	}
	LabelWindow[LFRMFIL] = txtwin(StringTable[STR_TXT2], LabelWindowCoords);
	ValueWindow[LFRMFIL] = txtrwin(StringTable[STR_FIL0 + SelectedForm->fillType], ValueWindowCoords);
	nxtlin();
	if (SelectedForm->fillType) {
		LabelWindow[LFRMCOL] = txtwin(StringTable[STR_TXT3], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillColor + 1);
		ValueWindow[LFRMCOL] = numwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == FTHF) {
			LabelWindow[LFTHCOL] = txtwin(StringTable[STR_FTHCOL], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.color + 1);
			ValueWindow[LFTHCOL] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHTYP] = txtwin(StringTable[STR_FTHTYP], LabelWindowCoords);
			ValueWindow[LFTHTYP] = numwin(StringTable[STR_FTH0 + SelectedForm->fillInfo.feather.fillType - 1], ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHBLND] = txtwin(StringTable[STR_FTHBLND], LabelWindowCoords);
			if (SelectedForm->extendedAttribute&AT_FTHBLND)
				pchr = StringTable[STR_ON];
			else
				pchr = StringTable[STR_OFF];
			ValueWindow[LFTHBLND] = txtrwin(pchr, ValueWindowCoords);
			nxtlin();
			if (!(SelectedForm->extendedAttribute&AT_FTHBLND)) {
				// ToDo - check whether we are doing 'feather down' or 'feather both'
				// only 'feather both' exists in the string table
				LabelWindow[LFTHDWN] = txtwin(StringTable[STR_FTHBOTH], LabelWindowCoords);
				if (SelectedForm->extendedAttribute&(AT_FTHDWN))
					pchr = StringTable[STR_ON];
				else
					pchr = StringTable[STR_OFF];
				ValueWindow[LFTHDWN] = txtrwin(pchr, ValueWindowCoords);
				nxtlin();
				if (!(SelectedForm->extendedAttribute&AT_FTHDWN)) {
					LabelWindow[LFTHUP] = txtwin(StringTable[STR_FTHUP], LabelWindowCoords);
					if (SelectedForm->extendedAttribute&AT_FTHUP)
						pchr = StringTable[STR_ON];
					else
						pchr = StringTable[STR_OFF];
					ValueWindow[LFTHUP] = txtrwin(pchr, ValueWindowCoords);
					nxtlin();
				}
			}
			LabelWindow[LFTHUPCNT] = txtwin(StringTable[STR_FTHUPCNT], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.upCount);
			ValueWindow[LFTHUPCNT] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHDWNCNT] = txtwin(StringTable[STR_FTHDWNCNT], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.downCount);
			ValueWindow[LFTHDWNCNT] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHSIZ] = txtwin(StringTable[STR_FTHSIZ], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->fillInfo.feather.ratio);
			ValueWindow[LFTHSIZ] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
			if (SelectedForm->fillInfo.feather.fillType == FTHPSG) {
				LabelWindow[LFTHNUM] = txtwin(StringTable[STR_FTHNUM], LabelWindowCoords);
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillInfo.feather.count);
				ValueWindow[LFTHNUM] = numwin(MsgBuffer, ValueWindowCoords);
				nxtlin();
			}
			LabelWindow[LFTHFLR] = txtwin(StringTable[STR_FTHFLR], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->fillInfo.feather.minStitchSize / PFGRAN);
			ValueWindow[LFTHFLR] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType != CLPF) {
			LabelWindow[LFRMSPAC] = txtwin(StringTable[STR_TXT4], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->fillSpacing / PFGRAN);
			ValueWindow[LFRMSPAC] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (istx(ClosestFormToCursor))
		{
			LabelWindow[LTXOF] = txtwin(StringTable[STR_TXOF], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->txof / PFGRAN);
			ValueWindow[LTXOF] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		LabelWindow[LMAXFIL] = txtwin(StringTable[STR_TXT20], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->maxFillStitchLen / PFGRAN);
		ValueWindow[LMAXFIL] = numwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
		if (!isclp(ClosestFormToCursor) && !istx(ClosestFormToCursor)) {
			LabelWindow[LFRMLEN] = txtwin(StringTable[STR_TXT5], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->lengthOrCount.stitchLength / PFGRAN);
			ValueWindow[LFRMLEN] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		LabelWindow[LMINFIL] = txtwin(StringTable[STR_TXT21], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->minFillStitchLen / PFGRAN);
		ValueWindow[LMINFIL] = numwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == ANGF || SelectedForm->fillType == TXANGF) {
			LabelWindow[LFRMANG] = txtwin(StringTable[STR_TXT6], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->angleOrClipData.angle * 180 / PI);
			ValueWindow[LFRMANG] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == ANGCLPF) {
			LabelWindow[LSACANG] = txtwin(StringTable[STR_TXT6], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->satinOrAngle.angle * 180 / PI);
			ValueWindow[LSACANG] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VCLPF || SelectedForm->fillType == HCLPF || SelectedForm->fillType == ANGCLPF) {
			LabelWindow[LFRMFAZ] = txtwin(StringTable[STR_TXT18], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->wordParam);
			ValueWindow[LFRMFAZ] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VRTF || SelectedForm->fillType == HORF || SelectedForm->fillType == ANGF || istx(ClosestFormToCursor)) {
			LabelWindow[LBFILSQR] = txtwin(StringTable[STR_PRF2], LabelWindowCoords);
			if (SelectedForm->extendedAttribute&AT_SQR)
				strcpy_s(MsgBuffer, StringTable[STR_SQR]);
			else
				strcpy_s(MsgBuffer, StringTable[STR_PNTD]);
			ValueWindow[LBFILSQR] = txtrwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
	}
	LabelWindow[LFSTRT] = txtwin(StringTable[STR_FSTRT], LabelWindowCoords);
	if (SelectedForm->extendedAttribute&AT_STRT)
		strcpy_s(MsgBuffer, StringTable[STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_OFF]);
	ValueWindow[LFSTRT] = txtrwin(MsgBuffer, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_STRT)
	{
		LabelWindow[LDSTRT] = txtwin(StringTable[STR_FSTRT], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillStart);
		ValueWindow[LDSTRT] = numwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
	}
	LabelWindow[LFEND] = txtwin(StringTable[STR_FEND], LabelWindowCoords);
	if (SelectedForm->extendedAttribute&AT_END)
		strcpy_s(MsgBuffer, StringTable[STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_OFF]);
	ValueWindow[LFEND] = txtrwin(MsgBuffer, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_END)
	{
		LabelWindow[LDEND] = txtwin(StringTable[STR_FEND], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", SelectedForm->fillEnd);
		ValueWindow[LDEND] = numwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
	}
	LabelWindow[LBRD] = txtwin(StringTable[STR_TXT7], LabelWindowCoords);
	ValueWindow[LBRD] = txtrwin(StringTable[STR_EDG0 + cod], ValueWindowCoords);
	nxtlin();
	if (cod) {
		LabelWindow[LBRDCOL] = txtwin(StringTable[STR_TXT8], LabelWindowCoords);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", (SelectedForm->borderColor & 0xf) + 1);
		ValueWindow[LBRDCOL] = numwin(MsgBuffer, ValueWindowCoords);
		nxtlin();
		if (EdgeArray[fpnt] & BESPAC) {
			LabelWindow[LBRDSPAC] = txtwin(StringTable[STR_TXT9], LabelWindowCoords);
			if (cod == EDGEPROPSAT || cod == EDGEOCHAIN || cod == EDGELCHAIN)
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeSpacing / PFGRAN);
			else
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeSpacing / PFGRAN * 2);
			ValueWindow[LBRDSPAC] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BPICSPAC) {
			LabelWindow[LBRDPIC] = txtwin(StringTable[STR_TXT16], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeSpacing / PFGRAN);
			ValueWindow[LBRDPIC] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BEMAX) {
			LabelWindow[LMAXBRD] = txtwin(StringTable[STR_TXT22], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->maxBorderStitchLen / PFGRAN);
			ValueWindow[LMAXBRD] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BELEN) {
			LabelWindow[LBRDLEN] = txtwin(StringTable[STR_TXT10], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeStitchLen / PFGRAN);
			ValueWindow[LBRDLEN] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BEMIN) {
			LabelWindow[LMINBRD] = txtwin(StringTable[STR_TXT23], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->minBorderStitchLen / PFGRAN);
			ValueWindow[LMINBRD] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BESIZ) {
			LabelWindow[LBRDSIZ] = txtwin(StringTable[STR_TXT11], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->borderSize / PFGRAN);
			ValueWindow[LBRDSIZ] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BRDPOS) {
			LabelWindow[LBRDPOS] = txtwin(StringTable[STR_TXT18], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeStitchLen);
			ValueWindow[LBRDPOS] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & CHNPOS) {
			LabelWindow[LBRDPOS] = txtwin(StringTable[STR_TXT19], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SelectedForm->edgeStitchLen);
			ValueWindow[LBRDPOS] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (cod == EDGEAPPL) {
			LabelWindow[LAPCOL] = txtwin(StringTable[STR_TXT12], LabelWindowCoords);
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", (SelectedForm->borderColor >> 4) + 1);
			ValueWindow[LAPCOL] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (cod == EDGEANGSAT || cod == EDGEAPPL || cod == EDGEPROPSAT) {
			LabelWindow[LBRDUND] = txtwin(StringTable[STR_TXT17], LabelWindowCoords);
			if (SelectedForm->edgeType&EGUND)
				ValueWindow[LBRDUND] = numwin(StringTable[STR_ON], ValueWindowCoords);
			else
				ValueWindow[LBRDUND] = numwin(StringTable[STR_OFF], ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[fpnt] & BCNRSIZ) {
			if (SelectedForm->edgeType == EDGEBHOL)
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", getblen() / PFGRAN);
			else
				sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", getplen() / PFGRAN);
			LabelWindow[LBCSIZ] = txtwin(StringTable[STR_TXT13], LabelWindowCoords);
			ValueWindow[LBCSIZ] = numwin(MsgBuffer, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->type == FRMLINE&&EdgeArray[fpnt] & BRDEND) {
			LabelWindow[LBSTRT] = txtwin(StringTable[STR_TXT14], LabelWindowCoords);
			if (SelectedForm->attribute&SBLNT)
				ValueWindow[LBSTRT] = numwin(StringTable[STR_BLUNT], ValueWindowCoords);
			else
				ValueWindow[LBSTRT] = numwin(StringTable[STR_TAPR], ValueWindowCoords);
			nxtlin();
			LabelWindow[LBFIN] = txtwin(StringTable[STR_TXT15], LabelWindowCoords);
			if (SelectedForm->attribute&FBLNT)
				ValueWindow[LBFIN] = numwin(StringTable[STR_BLUNT], ValueWindowCoords);
			else
				ValueWindow[LBFIN] = numwin(StringTable[STR_TAPR], ValueWindowCoords);
			nxtlin();
		}
	}
}

void refrm() {
	SelectedForm = &FormList[ClosestFormToCursor];
	if (rstMap(PRFACT)) {
		DestroyWindow(PreferencesWindow);
		rstMap(WASRT);
	}
	LabelWindowSize.x = LabelWindowSize.y = ValueWindowSize.x = ValueWindowSize.y = 0;
	setMap(REFCNT);
	FormMenuEntryCount = 0;
	refrmfn();
	if (FormDataSheet) {
		while (EnumChildWindows(FormDataSheet, chenum, 0));
		MoveWindow(FormDataSheet, ButtonWidthX3 + 3, 3, LabelWindowSize.x + ValueWindowSize.x + 18, LabelWindowSize.y*FormMenuEntryCount + 12, TRUE);
		redraw(FormDataSheet);
	}
	else {
		FormDataSheet = CreateWindow(
			"STATIC",
			0,
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			ButtonWidthX3 + 3,
			3,
			LabelWindowSize.x + ValueWindowSize.x + 18,
			LabelWindowSize.y*FormMenuEntryCount + 12,
			ThrEdWindow,
			NULL,
			ThrEdInstance,
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
		MoveMemory(&trct, &SelectedPointsLine, sizeof(fRECTANGLE));
	else {
		if (SelectedFormCount || chkMap(BIGBOX))
			pxrct2stch(SelectedFormsRect, &trct);
		else {
			fvars(ClosestFormToCursor);
			px2stch();
			trct.bottom = trct.left = trct.right = trct.top = 0;
		}
	}
	switch (SelectedFormControlVertex) {
	case 0:

		if (SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.bottom;
			tlng = Msg.pt.y - StitchWindowOrigin.y;
			rat = (double)(SelectedFormsRect.bottom - tlng) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
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

		if (SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.left;
			tlng = Msg.pt.x - StitchWindowOrigin.x;
			rat = (double)(tlng - SelectedFormsRect.left) / (SelectedFormsRect.right - SelectedFormsRect.left);
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

		if (SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.top;
			tlng = Msg.pt.y - StitchWindowOrigin.y;
			rat = (double)(tlng - SelectedFormsRect.top) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
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

		if (SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
			ref = trct.right;
			tlng = Msg.pt.x - StitchWindowOrigin.x;
			rat = (double)(SelectedFormsRect.right - tlng) / (SelectedFormsRect.right - SelectedFormsRect.left);
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
	if (SelectedFormControlVertex & 1) {
		if (chkMap(FPSEL)) {
			fvars(ClosestFormToCursor);
			ine = SelectedFormVertices.start;
			for (ind = 0; ind <= SelectedFormVertices.vertexCount; ind++) {
				CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - ref)*rat + ref;
				ine = pdir(ine);
			}
			frmout(ClosestFormToCursor);
			setpsel();
			setMap(RESTCH);
			return;
		}
		if (chkMap(BIGBOX)) {
			for (ind = 0; ind < FormIndex; ind++) {
				CurrentFormVertices = FormList[ind].vertices;
				for (ine = 0; ine < FormList[ind].vertexCount; ine++)
					CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - ref)*rat + ref;
				frmout(ind);
			}
			for (ind = 0; ind < PCSHeader.stitchCount; ind++)
				StitchBuffer[ind].x = (StitchBuffer[ind].x - ref)*rat + ref;
			selal();
			return;
		}
		else {
			if (SelectedFormCount) {
				for (ind = 0; ind < SelectedFormCount; ind++) {
					CurrentFormVertices = FormList[SelectedFormList[ind]].vertices;
					for (ine = 0; ine < FormList[SelectedFormList[ind]].vertexCount; ine++)
						CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - ref)*rat + ref;
				}
			}
			else {
				if (chkMap(FORMSEL)) {
					for (ind = 0; ind < VertexCount; ind++)
						CurrentFormVertices[ind].x = (CurrentFormVertices[ind].x - ref)*rat + ref;
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
			fvars(ClosestFormToCursor);
			ine = SelectedFormVertices.start;
			for (ind = 0; ind <= SelectedFormVertices.vertexCount; ind++) {
				CurrentFormVertices[ine].y = (CurrentFormVertices[ine].y - ref)*rat + ref;
				ine = pdir(ine);
			}
			frmout(ClosestFormToCursor);
			setpsel();
			refil();
			setMap(RESTCH);
			return;
		}
		if (chkMap(BIGBOX)) {
			for (ind = 0; ind < FormIndex; ind++) {
				CurrentFormVertices = FormList[ind].vertices;
				for (ine = 0; ine < FormList[ind].vertexCount; ine++)
					CurrentFormVertices[ine].y = (CurrentFormVertices[ine].y - ref)*rat + ref;
				frmout(ind);
			}
			for (ind = 0; ind < PCSHeader.stitchCount; ind++)
				StitchBuffer[ind].y = (StitchBuffer[ind].y - ref)*rat + ref;
			selal();
			return;
		}
		else {
			if (SelectedFormCount) {
				for (ind = 0; ind < SelectedFormCount; ind++) {
					CurrentFormVertices = FormList[SelectedFormList[ind]].vertices;
					for (ine = 0; ine < FormList[SelectedFormList[ind]].vertexCount; ine++)
						CurrentFormVertices[ine].y = (CurrentFormVertices[ine].y - ref)*rat + ref;
				}
			}
			else {
				if (chkMap(FORMSEL)) {
					for (ind = 0; ind < VertexCount; ind++)
						CurrentFormVertices[ind].y = (CurrentFormVertices[ind].y - ref)*rat + ref;
				}
				else {
					for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++)
						StitchBuffer[ind].y = (StitchBuffer[ind].y - ref)*rat + ref;
				}
			}
		}
	}
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ine = SelectedFormList[ind];
			frmout(ine);
			ClosestFormToCursor = ine;
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
	if (SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
		rct.bottom = SelectedFormsRect.bottom;
		rct.left = SelectedFormsRect.left;
		rct.right = SelectedFormsRect.right;
		rct.top = SelectedFormsRect.top;
		SelectedPoint.x = Msg.pt.x - StitchWindowOrigin.x;
		SelectedPoint.y = Msg.pt.y - StitchWindowOrigin.y;
		l_siz0.y = rct.bottom - rct.top;
	}
	else {
		px2stch();
		fvars(ClosestFormToCursor);
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
	switch (SelectedFormControlVertex) {
	case 0:

		ref.x = rct.right;
		ref.y = rct.bottom;
		l_siz1.x = fabs(SelectedPoint.x - ref.x);
		l_siz1.y = fabs(SelectedPoint.y - ref.y);
		aspect = l_siz1.x / l_siz1.y;
		if (aspect < XYratio)
			l_siz1.x = l_siz1.y*XYratio;
		else
			l_siz1.y = l_siz1.x / XYratio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!SelectedFormCount&&chkMap(FORMSEL)) {
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
		if (aspect < XYratio)
			l_siz1.x = l_siz1.y*XYratio;
		else
			l_siz1.y = l_siz1.x / XYratio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!SelectedFormCount&&chkMap(FORMSEL)) {
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
		if (aspect < XYratio)
			l_siz1.x = l_siz1.y*XYratio;
		else
			l_siz1.y = l_siz1.x / XYratio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!SelectedFormCount&&chkMap(FORMSEL)) {
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
		if (aspect < XYratio)
			l_siz1.x = l_siz1.y*XYratio;
		else
			l_siz1.y = l_siz1.x / XYratio;
		rat.x = l_siz1.x / l_siz0.x;
		rat.y = l_siz1.y / l_siz0.y;
		if (!SelectedFormCount&&chkMap(FORMSEL)) {
			SelectedForm->rectangle.left = rct.right - l_siz1.x;
			SelectedForm->rectangle.bottom = rct.top - l_siz1.y;
		}
		break;
	}
	tref.x = ref.x;
	tref.y = ref.y;
	px2stchf(tref, &sref);
	if (chkMap(FPSEL)) {
		fvars(ClosestFormToCursor);
		ine = SelectedFormVertices.start;
		for (ind = 0; ind <= SelectedFormVertices.vertexCount; ind++) {
			CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - sref.x)*rat.x + sref.x;
			CurrentFormVertices[ine].y = (CurrentFormVertices[ine].y - sref.y)*rat.y + sref.y;
			ine = pdir(ine);
		}
		setpsel();
		frmout(ClosestFormToCursor);
		refil();
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		for (ind = 0; ind < FormIndex; ind++) {
			fvars(ind);
			for (ine = 0; ine < SelectedForm->vertexCount; ine++) {
				CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - sref.x)*rat.x + sref.x;
				CurrentFormVertices[ine].y = (CurrentFormVertices[ine].y - sref.y)*rat.y + sref.y;
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
		if (SelectedFormCount) {
			for (ind = 0; ind < SelectedFormCount; ind++) {
				fvars(SelectedFormList[ind]);
				for (ine = 0; ine < SelectedForm->vertexCount; ine++) {
					CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - sref.x)*rat.x + sref.x;
					CurrentFormVertices[ine].y = (CurrentFormVertices[ine].y - sref.y)*rat.y + sref.y;
				}
				frmout(SelectedFormList[ind]);
				ClosestFormToCursor = SelectedFormList[ind];
				refil();
			}
		}
		else {
			if (chkMap(FORMSEL)) {
				for (ind = 0; ind < VertexCount; ind++) {
					CurrentFormVertices[ind].x = (CurrentFormVertices[ind].x - ref.x)*rat.x + ref.x;
					CurrentFormVertices[ind].y = (CurrentFormVertices[ind].y - ref.y)*rat.y + ref.y;
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
		at = (ClosestFormToCursor << 4) | FRMFIL;
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
		at = (ClosestFormToCursor << 4) | FTHMSK;
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
	at = (ClosestFormToCursor << 4) | FRMBFIL;
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
		at = (ClosestFormToCursor << 4) | TYPMSK;
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
		ThrEdWindow,
		NULL,
		ThrEdInstance,
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
		ValueWindowSize.x,
		wrct.bottom - wrct.top + 3,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void sbold() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEBEAN;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void dubold() {
	unsigned ind;

	if (filmsgs(FML_BLD))
		return;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
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
		LabelWindowCoords.left,
		LabelWindowCoords.top,
		LabelWindowCoords.right - LabelWindowCoords.left,
		LabelWindowCoords.bottom - LabelWindowCoords.top,
		PreferencesWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

HWND prfnwin(TCHAR* str) {
	return CreateWindow(
		"STATIC",
		str,
		SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
		ValueWindowCoords.left,
		ValueWindowCoords.top,
		ValueWindowCoords.right - ValueWindowCoords.left,
		ValueWindowCoords.bottom - ValueWindowCoords.top,
		PreferencesWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void prflin(unsigned p_lin) {
	prftwin(StringTable[p_lin]);
	ValueWindow[p_lin - STR_PRF0] = prfnwin(MsgBuffer);
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
	SelectedFormCount = 0;
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	LabelWindowSize.x = LabelWindowSize.y = 0;
	ValueWindowSize.x = ValueWindowSize.y = 0;
	maxtsiz(StringTable[STR_PRF0 + 4], &LabelWindowSize);
	maxtsiz(StringTable[STR_TAPR], &ValueWindowSize);
	LabelWindowSize.x = PreferenceWindowTextWidth;
	LabelWindowSize.x += 4;
#if LANG==HNG
	LabelWindowSize.x += 10;
#endif
	DestroyWindow(PreferencesWindow);
	PreferenceWindowWidth = LabelWindowSize.x + ValueWindowSize.x + 18;
	PreferencesWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3 + 3,
		3,
		PreferenceWindowWidth,
		LabelWindowSize.y*PRFLINS + 12,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	prfdc = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &prfrct);
	FillRect(prfdc, &prfrct, (HBRUSH)(COLOR_WINDOW + 1));
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left = 3;
	LabelWindowCoords.right = 3 + LabelWindowSize.x;
	ValueWindowCoords.left = 6 + LabelWindowSize.x;
	ValueWindowCoords.right = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", AppliqueColor + 1);
	prflin(STR_PRF10);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", BorderWidth / PFGRAN);
	prflin(STR_PRF3);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", ButtonholeCornerLength / PFGRAN);
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", StitchSpacing / PFGRAN);
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", PicotSpacing / PFGRAN);
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SnapLength / PFGRAN);
	prflin(STR_PRF11);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SpiralWrap);
	prflin(STR_PRF13);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", StarRatio);
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
	ReleaseDC(ThrEdWindow, prfdc);
}
#endif

#if LANG==GRM

void prfmsg() {
	HDC		prfdc;
	RECT	prfrct;

	if (rstMap(INSRT))
		setMap(WASRT);
	rstMap(BIGBOX);
	SelectedFormCount = 0;
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	LabelWindowSize.x = LabelWindowSize.y = 0;
	ValueWindowSize.x = ValueWindowSize.y = 0;
	maxtsiz(StringTable[STR_PRF0 + 4], &LabelWindowSize);
	maxtsiz(StringTable[STR_BLUNT], &ValueWindowSize);
	LabelWindowSize.x = PreferenceWindowTextWidth;
	ValueWindowSize.x += 4;
	DestroyWindow(PreferencesWindow);
	PreferenceWindowWidth = LabelWindowSize.x + ValueWindowSize.x + 18;
	PreferencesWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3 + 3,
		3,
		PreferenceWindowWidth,
		LabelWindowSize.y*PRFLINS + 12,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	prfdc = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &prfrct);
	FillRect(prfdc, &prfrct, (HBRUSH)(COLOR_WINDOW + 1));
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left = 3;
	LabelWindowCoords.right = 3 + LabelWindowSize.x;
	ValueWindowCoords.left = 6 + LabelWindowSize.x;
	ValueWindowCoords.right = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", StitchSpacing / PFGRAN);
	prflin(STR_PRF0);
	if (chku(SQRFIL))
		strcpy_s(MsgBuffer, StringTable[STR_SQR]);
	else
		strcpy_s(MsgBuffer, StringTable[STR_PNTD]);
	prflin(STR_PRF2);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", BorderWidth / PFGRAN);
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", ButtonholeCornerLength / PFGRAN);
	prflin(STR_PRF14);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainSpace / PFGRAN);
	prflin(STR_PRF23);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.chainRatio);
	prflin(STR_PRF24);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f mm", IniFile.clipOffset / PFGRAN);
	prflin(STR_PRF21);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", IniFile.fillPhase);
	prflin(STR_PRF22);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", PicotSpacing / PFGRAN);
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SnapLength / PFGRAN);
	prflin(STR_PRF11);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", duthrsh(StitchBoxesThreshold));
	prflin(STR_PRF8);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", IniFile.EggRatio);
	prflin(STR_PRF26);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", SpiralWrap);
	prflin(STR_PRF13);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", StarRatio);
	prflin(STR_PRF12);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%d", AppliqueColor + 1);
	prflin(STR_PRF10);
	setMap(PRFACT);
	ReleaseDC(ThrEdWindow, prfdc);
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
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(nsids);
	SelectedForm->vertexCount = nsids;
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	for (ind = 0; ind < VertexCount; ind++) {
		CurrentFormVertices[ind].x = pnt.x;
		CurrentFormVertices[ind].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	SelectedForm->type = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = nsids + 1;
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
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(tsid);
	SelectedForm->vertexCount = tsid;
	SelectedForm->attribute = (ActiveLayer << 1);
	fvars(FormIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	setMap(FILDIR);
	for (ind = 0; ind < tsid; ind++) {
		CurrentFormVertices[ind].x = pnt.x;
		CurrentFormVertices[ind].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	cntr.x = (CurrentFormVertices[nsids].x - CurrentFormVertices[0].x) / 2 + CurrentFormVertices[0].x;
	cntr.y = (CurrentFormVertices[nsids].y - CurrentFormVertices[0].y) / 2 + CurrentFormVertices[0].y;
	for (ind = 1; ind < tsid; ind += 2) {
		CurrentFormVertices[ind].x = (CurrentFormVertices[ind].x - cntr.x)*StarRatio + cntr.x;
		CurrentFormVertices[ind].y = (CurrentFormVertices[ind].y - cntr.y)*StarRatio + cntr.y;
	}
	SelectedForm->type = FRMFPOLY;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = tsid + 1;
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
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	num = nsids*SpiralWrap;
	SelectedForm->vertices = adflt(num);
	tflt = new fPOINT[nsids];
	tdif = new fPOINT[nsids];
	SelectedForm->vertexCount = num;
	SelectedForm->attribute = (ActiveLayer << 1);
	fvars(FormIndex);
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
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = num + 1;
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
	SelectedForm = &FormList[FormIndex];
	frmclr(SelectedForm);
	SelectedForm->attribute = ActiveLayer << 1;
	CurrentFormVertices = &FormVertices[FormVertexIndex];
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
		CurrentFormVertices[ind].x = pnt.x;
		CurrentFormVertices[ind++].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang -= dang;
	}
	dang /= 4.5;
	bind = ind;
	while (pnt.x > CurrentFormVertices[0].x&&ind < 200) {
		CurrentFormVertices[ind].x = pnt.x;
		CurrentFormVertices[ind++].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang -= dang;
	}
	bind--;
	rat = (CurrentFormVertices[bind].x - CurrentFormVertices[0].x) / (CurrentFormVertices[bind].x - CurrentFormVertices[ind - 1].x);
	for (ine = bind + 1; ine < ind; ine++)
		CurrentFormVertices[ine].x = (CurrentFormVertices[ine].x - CurrentFormVertices[bind].x)*rat + CurrentFormVertices[bind].x;
	bind = ine;
	for (ind = bind - 2; ind; ind--) {
		CurrentFormVertices[ine].y = CurrentFormVertices[ind].y;
		CurrentFormVertices[ine].x = av + av - CurrentFormVertices[ind].x - 2 * (av - CurrentFormVertices[0].x);
		ine++;
	}
	NewFormVertexCount = ine + 1;
	SelectedForm->vertices = adflt(ine);
	SelectedForm->vertexCount = ine;
	SelectedForm->type = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
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
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	pnt.x = SelectedPoint.x;
	pnt.y = SelectedPoint.y;
	ind = 0;
	SelectedPoint.x -= (float)0.0001;
	while (pnt.x >= SelectedPoint.x) {
		CurrentFormVertices[ind].x = pnt.x;
		CurrentFormVertices[ind++].y = pnt.y;
		pnt.x += len*cos(l_ang);
		pnt.y += len*sin(l_ang);
		l_ang += dang;
	}
	bind = ind - 1;
	ine = ind;
	av = CurrentFormVertices[0].x;
	for (ind = bind; ind != 0; ind--) {
		CurrentFormVertices[ine].y = CurrentFormVertices[ind - 1].y;
		CurrentFormVertices[ine].x = av + av - CurrentFormVertices[ind - 1].x;
		ine++;
	}
	NewFormVertexCount = ine;
	SelectedForm->vertices = adflt(ine - 1);
	SelectedForm->vertexCount = ine - 1;
	SelectedForm->type = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

float shreg(float hi, float ref) {
	return (hi - ref)*EggRatio + ref;
}

void dueg(unsigned nsids) {
	double		ref, hi;
	unsigned	ind;

	if (nsids < 8)
		nsids = 8;
	fvars(FormIndex);
	durpoli(nsids);
	ref = midl(CurrentFormVertices[nsids / 2].y, CurrentFormVertices[0].y);
	hi = CurrentFormVertices[nsids >> 2].y - CurrentFormVertices[0].y;
	for (ind = 0; ind < nsids; ind++) {
		if (CurrentFormVertices[ind].y < ref)
			CurrentFormVertices[ind].y = ref - (ref - CurrentFormVertices[ind].y)*IniFile.eggRatio;
	}
	EggRatio = hi / (CurrentFormVertices[nsids >> 2].y - CurrentFormVertices[0].y);
	for (ind = 1; ind < VertexCount; ind++) {
		CurrentFormVertices[ind].x = shreg(CurrentFormVertices[ind].x, CurrentFormVertices[0].x);
		CurrentFormVertices[ind].y = shreg(CurrentFormVertices[ind].y, CurrentFormVertices[0].y);
	}
}

void duzig(unsigned nsids) {
	fPOINT		off;
	unsigned	ind;

	if (nsids < 3)
		nsids = 3;
	if (nsids > 100)
		nsids = 100;
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(nsids);
	SelectedForm->vertexCount = nsids;
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	SelectedPoint.x;
	SelectedPoint.y;
	off.x = UnzoomedRect.x / 6;
	off.y = UnzoomedRect.y / (6 * nsids);
	for (ind = 0; ind < nsids; ind++) {
		CurrentFormVertices[ind].x = SelectedPoint.x;
		CurrentFormVertices[ind].y = SelectedPoint.y;
		SelectedPoint.y -= off.y;
		if (ind & 1)
			SelectedPoint.x += off.x;
		else
			SelectedPoint.x -= off.x;
	}
	SelectedForm->type = FRMLINE;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = nsids + 1;
	setMap(POLIMOV);
	setmfrm();
	setMap(SHOFRM);
	mdufrm();
}

void fliph() {
	unsigned	ind, ine;
	float		av = 0;
	fRECTANGLE		trct;

	fvars(ClosestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		av = (SelectedVerticesRect.right - SelectedVerticesRect.left)*0.5 + SelectedVerticesRect.left;
		ine = SelectedFormVertices.start;
		for (ind = 0; ind <= SelectedFormVertices.vertexCount; ind++) {
			CurrentFormVertices[ine].x = av + av - CurrentFormVertices[ine].x;
			ine = pdir(ine);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		av = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
		for (ind = 0; ind < FormVertexIndex; ind++)
			FormVertices[ind].x = av + av - FormVertices[ind].x;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			StitchBuffer[ind].x = av + av - StitchBuffer[ind].x;
		for (ind = 0; ind < FormIndex; ind++) {
			FormList[ind].rectangle.left = av + av - FormList[ind].rectangle.left;
			FormList[ind].rectangle.right = av + av - FormList[ind].rectangle.right;
		}
		setMap(RESTCH);
		return;
	}
	if (SelectedFormCount) {
		savdo();
		clRmap((FormIndex >> 5) + 1);
		pxrct2stch(SelectedFormsRect, &trct);
		av = (trct.right - trct.left) / 2 + trct.left;
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			setr(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			for (ine = 0; ine < SelectedForm->vertexCount; ine++)
				CurrentFormVertices[ine].x = av + av - CurrentFormVertices[ine].x;
			frmout(ClosestFormToCursor);
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
			for (ind = 0; ind < VertexCount; ind++)
				CurrentFormVertices[ind].x = av + av - CurrentFormVertices[ind].x;
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor && !(StitchBuffer[ind].attribute&NOTFRM))
					StitchBuffer[ind].x = av + av - StitchBuffer[ind].x;
			}
			frmout(ClosestFormToCursor);
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

	fvars(ClosestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		av = (SelectedVerticesRect.top - SelectedVerticesRect.bottom)*0.5 + SelectedVerticesRect.bottom;
		ine = SelectedFormVertices.start;
		for (ind = 0; ind <= SelectedFormVertices.vertexCount; ind++) {
			CurrentFormVertices[ine].y = av + av - CurrentFormVertices[ine].y;
			ine = pdir(ine);
		}
		setMap(RESTCH);
		return;
	}
	if (chkMap(BIGBOX)) {
		savdo();
		av = (AllItemsRect.top - AllItemsRect.bottom) / 2 + AllItemsRect.bottom;
		for (ind = 0; ind < FormVertexIndex; ind++)
			FormVertices[ind].y = av + av - FormVertices[ind].y;
		for (ind = 0; ind < PCSHeader.stitchCount; ind++)
			StitchBuffer[ind].y = av + av - StitchBuffer[ind].y;
		for (ind = 0; ind < FormIndex; ind++) {
			FormList[ind].rectangle.bottom = av + av - FormList[ind].rectangle.bottom;
			FormList[ind].rectangle.top = av + av - FormList[ind].rectangle.top;
		}
		setMap(RESTCH);
		return;
	}
	if (SelectedFormCount) {
		savdo();
		clRmap((FormIndex >> 5) + 1);
		pxrct2stch(SelectedFormsRect, &trct);
		av = (trct.top - trct.bottom) / 2 + trct.bottom;
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			setr(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			for (ine = 0; ine < SelectedForm->vertexCount; ine++)
				CurrentFormVertices[ine].y = av + av - CurrentFormVertices[ine].y;
			frmout(ClosestFormToCursor);
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
			for (ind = 0; ind < VertexCount; ind++)
				CurrentFormVertices[ind].y = av + av - CurrentFormVertices[ind].y;
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor && !(StitchBuffer[ind].attribute&NOTFRM))
					StitchBuffer[ind].y = av + av - StitchBuffer[ind].y;
			}
			frmout(ClosestFormToCursor);
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
		ThrEdInstance,
		NULL);
}

void sprct(unsigned strt, unsigned fin) {
	dPOINT	dif, tpnt;
	VRCT2*	tvrct;

	tvrct = &FillVerticalRect[strt];
	dif.x = OutsidePoints[fin].x - OutsidePoints[strt].x;
	dif.y = OutsidePoints[fin].y - OutsidePoints[strt].y;
	if (dif.x&&dif.y) {
		Slope = -dif.x / dif.y;
		tpnt.x = CurrentFormVertices[fin].x;
		tpnt.y = CurrentFormVertices[fin].y;
		proj(tpnt, Slope, OutsidePoints[strt], OutsidePoints[fin], &tvrct->dopnt);
		proj(tpnt, Slope, InsidePoints[strt], InsidePoints[fin], &tvrct->dipnt);
		tpnt.x = CurrentFormVertices[strt].x;
		tpnt.y = CurrentFormVertices[strt].y;
		proj(tpnt, Slope, OutsidePoints[strt], OutsidePoints[fin], &tvrct->aopnt);
		proj(tpnt, Slope, InsidePoints[strt], InsidePoints[fin], &tvrct->aipnt);
		tpnt.x = InsidePoints[strt].x;
		tpnt.y = InsidePoints[strt].y;
		if (proj(tpnt, Slope, OutsidePoints[strt], OutsidePoints[fin], &tvrct->bopnt)) {
			tvrct->bipnt.x = InsidePoints[strt].x;
			tvrct->bipnt.y = InsidePoints[strt].y;
		}
		else {
			tvrct->bopnt.x = OutsidePoints[strt].x;
			tvrct->bopnt.y = OutsidePoints[strt].y;
			tpnt.x = OutsidePoints[strt].x;
			tpnt.y = OutsidePoints[strt].y;
			proj(tpnt, Slope, InsidePoints[strt], InsidePoints[fin], &tvrct->bipnt);
		}
		tpnt.x = InsidePoints[fin].x;
		tpnt.y = InsidePoints[fin].y;
		if (proj(tpnt, Slope, OutsidePoints[strt], OutsidePoints[fin], &tvrct->copnt)) {
			tvrct->cipnt.x = InsidePoints[fin].x;
			tvrct->cipnt.y = InsidePoints[fin].y;
		}
		else {
			tvrct->copnt.x = OutsidePoints[fin].x;
			tvrct->copnt.y = OutsidePoints[fin].y;
			tpnt.x = OutsidePoints[fin].x;
			tpnt.y = OutsidePoints[fin].y;
			proj(tpnt, Slope, InsidePoints[strt], InsidePoints[fin], &tvrct->cipnt);
		}
	}
	else {
		if (dif.x) {
			tpnt.x = CurrentFormVertices[fin].x;
			projv(tpnt.x, OutsidePoints[strt], OutsidePoints[fin], &tvrct->dopnt);
			projv(tpnt.x, InsidePoints[strt], InsidePoints[fin], &tvrct->dipnt);
			tpnt.x = CurrentFormVertices[strt].x;
			projv(tpnt.x, OutsidePoints[strt], OutsidePoints[fin], &tvrct->aopnt);
			projv(tpnt.x, InsidePoints[strt], InsidePoints[fin], &tvrct->aipnt);
			tpnt.x = InsidePoints[strt].x;
			if (projv(tpnt.x, OutsidePoints[strt], OutsidePoints[fin], &tvrct->bopnt)) {
				tvrct->bipnt.x = InsidePoints[strt].x;
				tvrct->bipnt.y = InsidePoints[strt].y;
			}
			else {
				tvrct->bopnt.x = OutsidePoints[strt].x;
				tvrct->bopnt.y = OutsidePoints[strt].y;
				tpnt.x = OutsidePoints[strt].x;
				projv(tpnt.x, InsidePoints[strt], InsidePoints[fin], &tvrct->bipnt);
			}
			tpnt.x = InsidePoints[fin].x;
			if (projv(tpnt.x, OutsidePoints[strt], OutsidePoints[fin], &tvrct->copnt)) {
				tvrct->cipnt.x = InsidePoints[fin].x;
				tvrct->cipnt.y = InsidePoints[fin].y;
			}
			else {
				tvrct->copnt.x = OutsidePoints[fin].x;
				tvrct->copnt.y = OutsidePoints[fin].y;
				tpnt.x = OutsidePoints[fin].x;
				projv(tpnt.x, InsidePoints[strt], InsidePoints[fin], &tvrct->cipnt);
			}
		}
		else {
			tpnt.y = CurrentFormVertices[fin].y;
			projh(tpnt.y, OutsidePoints[strt], OutsidePoints[fin], &tvrct->dopnt);
			projh(tpnt.y, InsidePoints[strt], InsidePoints[fin], &tvrct->dipnt);
			tpnt.y = CurrentFormVertices[strt].y;
			projh(tpnt.y, OutsidePoints[strt], OutsidePoints[fin], &tvrct->aopnt);
			projh(tpnt.y, InsidePoints[strt], InsidePoints[fin], &tvrct->aipnt);
			tpnt.y = InsidePoints[strt].y;
			if (projh(tpnt.y, OutsidePoints[strt], OutsidePoints[fin], &tvrct->bopnt)) {
				tvrct->bipnt.x = InsidePoints[strt].x;
				tvrct->bipnt.y = InsidePoints[strt].y;
			}
			else {
				tvrct->bopnt.x = OutsidePoints[strt].x;
				tvrct->bopnt.y = OutsidePoints[strt].y;
				tpnt.y = OutsidePoints[strt].y;
				projh(tpnt.y, InsidePoints[strt], InsidePoints[fin], &tvrct->bipnt);
			}
			tpnt.y = InsidePoints[fin].y;
			if (projh(tpnt.y, OutsidePoints[strt], OutsidePoints[fin], &tvrct->copnt)) {
				tvrct->cipnt.x = InsidePoints[fin].x;
				tvrct->cipnt.y = InsidePoints[fin].y;
			}
			else {
				tvrct->copnt.x = OutsidePoints[fin].x;
				tvrct->copnt.y = OutsidePoints[fin].y;
				tpnt.y = OutsidePoints[fin].y;
				projh(OutsidePoints[fin].y, InsidePoints[strt], InsidePoints[fin], &tvrct->cipnt);
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
	spurfn(&FillVerticalRect[ind].aipnt, &FillVerticalRect[ind].aopnt, &UnderlayVerticalRect[ind].aipnt, &UnderlayVerticalRect[ind].aopnt);
	spurfn(&FillVerticalRect[ind].bipnt, &FillVerticalRect[ind].bopnt, &UnderlayVerticalRect[ind].bipnt, &UnderlayVerticalRect[ind].bopnt);
	spurfn(&FillVerticalRect[ind].cipnt, &FillVerticalRect[ind].copnt, &UnderlayVerticalRect[ind].cipnt, &UnderlayVerticalRect[ind].copnt);
	spurfn(&FillVerticalRect[ind].dipnt, &FillVerticalRect[ind].dopnt, &UnderlayVerticalRect[ind].dipnt, &UnderlayVerticalRect[ind].dopnt);
}

unsigned psg() {
	unsigned tmp;

	if (!PseudoRandomValue)
		PseudoRandomValue = SEED;
	tmp = PseudoRandomValue & 0x48000000;
	PseudoRandomValue <<= 1;
	if (tmp == 0x40000000 || tmp == 0x8000000)
		PseudoRandomValue++;
	return PseudoRandomValue;
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
	cnt = len0 / (StitchSpacing / 2);
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

	idif.x = FillVerticalRect[fin].cipnt.x - FillVerticalRect[strt].bipnt.x;
	idif.y = FillVerticalRect[fin].cipnt.y - FillVerticalRect[strt].bipnt.y;
	odif.x = FillVerticalRect[fin].copnt.x - FillVerticalRect[strt].bopnt.x;
	odif.y = FillVerticalRect[fin].copnt.y - FillVerticalRect[strt].bopnt.y;
	ilen = hypot(idif.x, idif.y);
	olen = hypot(odif.x, odif.y);
	if (olen > ilen) {
		piv.x = FillVerticalRect[strt].cipnt.x;
		piv.y = FillVerticalRect[strt].cipnt.y;
		sdif.x = FillVerticalRect[strt].copnt.x - piv.x;
		sdif.y = FillVerticalRect[strt].copnt.y - piv.y;
		fdif.x = FillVerticalRect[fin].bopnt.x - piv.x;
		fdif.y = FillVerticalRect[fin].bopnt.y - piv.y;
	}
	else {
		piv.x = FillVerticalRect[strt].copnt.x;
		piv.y = FillVerticalRect[strt].copnt.y;
		sdif.x = FillVerticalRect[strt].cipnt.x - piv.x;
		sdif.y = FillVerticalRect[strt].cipnt.y - piv.y;
		fdif.x = FillVerticalRect[fin].bipnt.x - piv.x;
		fdif.y = FillVerticalRect[fin].bipnt.y - piv.y;
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
	cnt = arc / StitchSpacing;
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
			lvl = Levels[cnt][ind];
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
			filinsb(UnderlayVerticalRect[strt].copnt);
			filinsb(UnderlayVerticalRect[strt].cipnt);
			dif.x = UnderlayVerticalRect[fin].bipnt.x - UnderlayVerticalRect[strt].cipnt.x;
			dif.y = UnderlayVerticalRect[fin].bipnt.y - UnderlayVerticalRect[strt].cipnt.y;
			len = hypot(dif.x, dif.y);
			if (len > SelectedForm->edgeStitchLen) {
				tang = atan2(InsidePoints[fin].y - OutsidePoints[fin].y, InsidePoints[fin].x - OutsidePoints[fin].x);
				tpnt.x = UnderlayVerticalRect[fin].bopnt.x + cos(tang)*HorizontalLength2;
				tpnt.y = UnderlayVerticalRect[fin].bopnt.y + sin(tang)*HorizontalLength2;
				filinsb(tpnt);
			}
			filinsb(UnderlayVerticalRect[fin].bipnt);
			filinsb(UnderlayVerticalRect[fin].bopnt);
		}
		else {
			filinsb(UnderlayVerticalRect[strt].cipnt);
			filinsb(UnderlayVerticalRect[strt].copnt);
			dif.x = UnderlayVerticalRect[fin].bopnt.x - UnderlayVerticalRect[strt].copnt.x;
			dif.y = UnderlayVerticalRect[fin].bopnt.y - UnderlayVerticalRect[strt].copnt.y;
			len = hypot(dif.x, dif.y);
			if (len > SelectedForm->edgeStitchLen) {
				tang = atan2(OutsidePoints[fin].y - InsidePoints[fin].y, OutsidePoints[fin].x - InsidePoints[fin].x);
				tpnt.x = UnderlayVerticalRect[fin].bipnt.x + cos(tang)*HorizontalLength2;
				tpnt.y = UnderlayVerticalRect[fin].bipnt.y + sin(tang)*HorizontalLength2;
				filinsb(tpnt);
			}
			filinsb(UnderlayVerticalRect[fin].bopnt);
			filinsb(UnderlayVerticalRect[fin].bipnt);
		}
	}
	else
		spend(strt, fin);
}

void pfn(unsigned strtlin, VRCT2* vrct) {
	unsigned		ind;
	unsigned short	nlin;

	SelectedPoint.x = CurrentFormVertices[strtlin].x;
	SelectedPoint.y = CurrentFormVertices[strtlin].y;
	nlin = nxt(strtlin);
	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
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
	for (ind = 2; ind < (unsigned)VertexCount - 4; ind++) {
		duromb(prct[ind].bipnt, prct[ind].cipnt, prct[ind].bopnt, prct[ind].copnt);
		duspnd(ind, ind + 1);
	}
	duromb(prct[VertexCount - 4].bipnt, prct[VertexCount - 4].dipnt, prct[VertexCount - 4].bopnt, prct[VertexCount - 4].dopnt);
}

void prsmal() {
	unsigned	ind, ine, ref;
	double		lSize, len;
	dPOINT		dif;

	ref = 0; ine = 0;
	lSize = USPAC*0.8;
	if (lSize > HorizontalLength2)
		lSize = HorizontalLength2*0.9;
	for (ind = 1; ind < SequenceIndex; ind++) {
		dif.x = OSequence[ind].x - OSequence[ref].x;
		dif.y = OSequence[ind].y - OSequence[ref].y;
		len = hypot(dif.x, dif.y);
		if (len > lSize) {
			OSequence[ine].x = OSequence[ind].x;
			OSequence[ine++].y = OSequence[ind].y;
			ref = ind;
		}
	}
	SequenceIndex = ine;
}

void plbak(unsigned bpnt) {
	unsigned	ind = SequenceIndex - 1;
	fPOINT		tflt;

	while (ind > bpnt) {
		tflt.x = OSequence[ind].x;
		tflt.y = OSequence[ind].y;
		OSequence[ind].x = OSequence[bpnt].x;
		OSequence[ind].y = OSequence[bpnt].y;
		OSequence[bpnt].x = tflt.x;
		OSequence[bpnt].y = tflt.y;
		ind--; bpnt++;
	}
}

void plvct(unsigned pind, dPOINT* vp0, dPOINT* vp1, double len)
{
	double l_angl;
	double len2;
	fPOINT vct;

	len2 = len / 2;
	l_angl = atan2(CurrentFormVertices[pind + 1].y - CurrentFormVertices[pind].y, CurrentFormVertices[pind + 1].x - CurrentFormVertices[pind].x) + PI / 2;
	vct.x = cos(l_angl)*len2;
	vct.y = sin(l_angl)*len2;
	vp0->x = CurrentFormVertices[pind].x + vct.x;
	vp0->y = CurrentFormVertices[pind].y + vct.y;
	vp1->x = CurrentFormVertices[pind].x - vct.x;
	vp1->y = CurrentFormVertices[pind].y - vct.y;
}

void prebrd()
{
	fPOINT dif;
	double rat;

	MoveMemory(&AngledFormVertices[1], CurrentFormVertices, sizeof(fPOINT)*VertexCount);
	dif.x = CurrentFormVertices[1].x - CurrentFormVertices[0].x;
	dif.y = CurrentFormVertices[1].y - CurrentFormVertices[0].y;
	if (fabs(dif.x) > fabs(dif.y))
		rat = fabs(0.1 / dif.x);
	else
		rat = fabs(0.1 / dif.y);
	AngledFormVertices[0].x = CurrentFormVertices[0].x - dif.x*rat;
	AngledFormVertices[0].y = CurrentFormVertices[0].y - dif.y*rat;
	MoveMemory(&AngledForm, SelectedForm, sizeof(FRMHED));
	AngledForm.vertices = AngledFormVertices;
	AngledForm.vertexCount += 3;
	dif.x = CurrentFormVertices[VertexCount - 1].x - CurrentFormVertices[VertexCount - 2].x;
	dif.y = CurrentFormVertices[VertexCount - 1].y - CurrentFormVertices[VertexCount - 2].y;
	if (dif.x > dif.y)
		rat = 0.1 / dif.x;
	else
		rat = 0.1 / dif.y;
	AngledFormVertices[AngledForm.vertexCount - 1].x = CurrentFormVertices[VertexCount - 1].x + dif.x*rat;
	AngledFormVertices[AngledForm.vertexCount - 1].y = CurrentFormVertices[VertexCount - 1].y + dif.y*rat;
	SelectedForm = &AngledForm;
	VertexCount = AngledForm.vertexCount;
	CurrentFormVertices = AngledForm.vertices;
}

void plbrd(double spac) {
	double			tspac;
	unsigned short	slin;
	unsigned bpnt;
	unsigned ind;

	slin = getlast();
	prebrd();
	tspac = StitchSpacing;
	FillVerticalRect = (VRCT2*)BSequence;
	UnderlayVerticalRect = &FillVerticalRect[VertexCount];
	satout(SelectedForm->borderSize);
	InsidePoints[VertexCount].x = InsidePoints[0].x;
	InsidePoints[VertexCount].y = InsidePoints[0].y;
	OutsidePoints[VertexCount].x = OutsidePoints[0].x;
	OutsidePoints[VertexCount].y = OutsidePoints[0].y;
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++)
	{
		sprct(ind, ind + 1);
		spurct(ind);
	}
	sprct(ind, 0);
	spurct(ind);
	if (!(SelectedForm->attribute&SBLNT))
	{
		FillVerticalRect[1].aipnt.x = FillVerticalRect[1].aopnt.x = UnderlayVerticalRect[1].aipnt.x = UnderlayVerticalRect[1].aopnt.x = SelectedForm->vertices[1].x;
		FillVerticalRect[1].aipnt.y = FillVerticalRect[1].aopnt.y = UnderlayVerticalRect[1].aipnt.y = UnderlayVerticalRect[1].aopnt.y = SelectedForm->vertices[1].y;
	}
	if (!(SelectedForm->attribute&FBLNT))
	{
		FillVerticalRect[VertexCount - 4].dipnt.x = FillVerticalRect[VertexCount - 4].dopnt.x = UnderlayVerticalRect[VertexCount - 4].dipnt.x = UnderlayVerticalRect[VertexCount - 4].dopnt.x = SelectedForm->vertices[VertexCount - 1].x;
		FillVerticalRect[VertexCount - 4].dipnt.y = FillVerticalRect[VertexCount - 4].dopnt.y = UnderlayVerticalRect[VertexCount - 4].dipnt.y = UnderlayVerticalRect[VertexCount - 4].dopnt.y = SelectedForm->vertices[VertexCount - 1].y;
	}
	SequenceIndex = 0;
	SelectedPoint.x = CurrentFormVertices[0].x;
	SelectedPoint.y = CurrentFormVertices[0].y;
	if (SelectedForm->edgeType&EGUND) {
		StitchSpacing = USPAC;
		setMap(UND);
		HorizontalLength2 = SelectedForm->borderSize*URAT;
		setMap(UNDPHAS);
		rstMap(FILDIR);
		plfn(&UnderlayVerticalRect[0]);
		bpnt = SequenceIndex;
		rstMap(UNDPHAS);
		SelectedPoint.x = CurrentFormVertices[0].x;
		SelectedPoint.y = CurrentFormVertices[0].y;
		setMap(FILDIR);
		plfn(&UnderlayVerticalRect[0]);
		plbak(bpnt);
		prsmal();
		if (SequenceIndex) { //ensure that we can do a valid read from OSequence
			SelectedPoint.x = OSequence[SequenceIndex - 1].x;
			SelectedPoint.y = OSequence[SequenceIndex - 1].y;
		}
	}
	rstMap(UND);
	StitchSpacing = SelectedForm->edgeSpacing;
	plfn(&FillVerticalRect[0]);
	StitchSpacing = spac;
	fvars(ClosestFormToCursor);
}

void pbrd(double spac) {
	double			tspac;
	unsigned		ind;
	unsigned		strt;

	tspac = StitchSpacing;
	StitchSpacing = SelectedForm->edgeSpacing;
	SequenceIndex = 0;
	FillVerticalRect = (VRCT2*)BSequence;
	UnderlayVerticalRect = &FillVerticalRect[VertexCount];
	strt = getlast();
	satout(SelectedForm->borderSize);
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++) {
		sprct(ind, ind + 1);
		spurct(ind);
	}
	sprct(ind, 0);
	spurct(ind);
	if (SelectedForm->edgeType&EGUND) {
		rstMap(SAT1);
		StitchSpacing = USPAC;
		setMap(UND);
		HorizontalLength2 = SelectedForm->borderSize*URAT;
		satout(HorizontalLength2);
		setMap(UNDPHAS);
		setMap(FILDIR);
		pfn(strt, &UnderlayVerticalRect[0]);
		rstMap(UNDPHAS);
		rstMap(FILDIR);
		pfn(strt, &UnderlayVerticalRect[0]);
		StitchSpacing = spac;
		prsmal();
		HorizontalLength2 = SelectedForm->borderSize;
		rstMap(UND);
	}
	pfn(strt, &FillVerticalRect[0]);
	StitchSpacing = tspac;
}

void prpsbrd() {
	if (SelectedForm->vertexCount > 2) {
		deleclp(ClosestFormToCursor);
		SelectedForm->edgeType = EDGEPROPSAT;
		if (chku(DUND))
			SelectedForm->edgeType |= EGUND;
		bsizpar();
		SelectedForm->borderSize = BorderWidth;
		SelectedForm->edgeSpacing = StitchSpacing;
		SelectedForm->borderColor = ActiveColor;
		refilfn();
	}
}

void prpbrd(double spac) {
	double		tspac = StitchSpacing;
	unsigned	ind;

	if (filmsgs(FML_PRPS))
		return;
	StitchSpacing = spac;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
			SelectedForm->borderSize = StitchSpacing;
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
			fvars(ClosestFormToCursor);
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
	StitchSpacing = tspac;
}

void tglfrm() {
	if (rstMap(SATPNT))
		satfix();
	rstMap(HIDSTCH);
	if (toglMap(FRMOF))
		strcpy_s(FormOnOff, StringTable[STR_FRMPLUS]);
	else {
		strcpy_s(FormOnOff, StringTable[STR_FRMINUS]);
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
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, &MenuInfo);
	setMap(DUMEN);
	setMap(RESTCH);
}

void frmon() {
	unbsho();
	rstMap(FRMOF);
	strcpy_s(FormOnOff, StringTable[STR_FRMPLUS]);
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, &MenuInfo);
	setMap(DUMEN);
}

void fnord() {
	int			ind;
	fPOINTATTR	tpnt;

	fvars(ClosestFormToCursor);
	SelectedForm = &FormList[ClosestFormToCursor];
	for (ind = 0; ind < SelectedForm->vertexCount >> 1; ind++) {
		tpnt.x = SelectedForm->vertices[ind].x;
		tpnt.y = SelectedForm->vertices[ind].y;
		SelectedForm->vertices[ind].x = SelectedForm->vertices[SelectedForm->vertexCount - ind - 1].x;
		SelectedForm->vertices[ind].y = SelectedForm->vertices[SelectedForm->vertexCount - ind - 1].y;
		SelectedForm->vertices[SelectedForm->vertexCount - ind - 1].x = tpnt.x;
		SelectedForm->vertices[SelectedForm->vertexCount - ind - 1].y = tpnt.y;
	}
	refil();
}

void flpord() {
	int			ind;
	unsigned	uind, strt, fin;
	fPOINTATTR	tpnt;

	fvars(ClosestFormToCursor);
	if (chkMap(FPSEL)) {
		savdo();
		strt = SelectedFormVertices.start;
		fin = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
		for (uind = 0; uind <= SelectedFormVertices.vertexCount >> 1; uind++) {
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
	if (SelectedFormCount) {
		savdo();
		for (uind = 0; uind < SelectedFormCount; uind++) {
			ClosestFormToCursor = SelectedFormList[uind];
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
		OSequence[ActivePointIndex].x = clprat.y*mdif.x + bmid.x;
		OSequence[ActivePointIndex].y = clprat.y*mdif.y + bmid.y;
		ActivePointIndex++;
	}
}

void clpfm() {
	unsigned	ind, ine, cnt;
	double		lsiz, l_rsiz;
	dPOINT		ldif, rdif;
	dPOINT		lstp, rstp;
	dPOINT		lpnt0, rpnt0;
	dPOINT		lpnt1, rpnt1;

	ActivePointIndex = 0;
	for (ind = 0; ind < SequenceIndex - 2; ind += 2) {
		lsiz = hypot(BSequence[ind + 1].x - BSequence[ind].x, BSequence[ind + 1].y - BSequence[ind].y);
		l_rsiz = hypot(BSequence[ind + 3].x - BSequence[ind + 2].x, BSequence[ind + 3].y - BSequence[ind + 2].y);
		ldif.x = BSequence[ind + 1].x - BSequence[ind].x;
		ldif.y = BSequence[ind + 1].y - BSequence[ind].y;
		rdif.x = BSequence[ind + 2].x - BSequence[ind + 3].x;
		rdif.y = BSequence[ind + 2].y - BSequence[ind + 3].y;
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
		lpnt1.x = BSequence[ind].x;
		lpnt1.y = BSequence[ind].y;
		rpnt1.x = BSequence[ind + 3].x;
		rpnt1.y = BSequence[ind + 3].y;
		for (ine = 0; ine < cnt; ine++) {
			lpnt0.x = lpnt1.x;
			lpnt0.y = lpnt1.y;
			rpnt0.x = rpnt1.x;
			rpnt0.y = rpnt1.y;
			lpnt1.x += lstp.x;
			lpnt1.y += lstp.y;
			rpnt1.x += rstp.x;
			rpnt1.y += rstp.y;
			if (ActivePointIndex > MAXSEQ - ClipStitchCount - 1)
				return;
			trfrm(lpnt0, lpnt1, rpnt0, rpnt1);
		}
	}
}

void fmclp() {
	double		tspac = StitchSpacing;

	StitchSpacing = ClipRectSize.cx;
	setMap(BARSAT);
	satfil();
	rstMap(BARSAT);
	clpfm();
	SelectedForm->fillType = CLPF;
	SequenceIndex = ActivePointIndex;
	StitchSpacing = tspac;
}

void filsclp() {
	unsigned ind;

	deltx();
	fvars(ClosestFormToCursor);
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
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN&&ClipRectSize.cy > CLPMIN) {
				if (SelectedFormCount) {
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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

	fin = Xhistogram[xind + len];
	ind = Xhistogram[xind];
	if (fin - ind) {
		for (ind = Xhistogram[xind]; ind < Xhistogram[xind + 1]; ind++) {
			ref = Xpoints[ind];
			for (ine = ind + 1; ine < fin; ine++) {
				chk = Xpoints[ine];
				tlen = hypot(StitchBuffer[chk].x - StitchBuffer[ref].x, StitchBuffer[chk].y - StitchBuffer[ref].y);
				if (tlen < SnapLength) {
					StitchBuffer[chk].x = StitchBuffer[ref].x;
					StitchBuffer[chk].y = StitchBuffer[ref].y;
				}
			}
		}
	}
}

void nutim(double pl_Size) {
	TimeWindow = CreateWindow(
		"STATIC",
		0,
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		ButtonWidthX3,
		0,
		StitchWindowSize.x,
		ButtonHeight,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
	TimeDC = GetDC(TimeWindow);
	TimeStep = (double)StitchWindowSize.x / pl_Size;
	TimePosition = 0;
	FormLines[0].y = 0;
	FormLines[1].y = ButtonHeight;
	FormLines[0].x = FormLines[1].x = 0;
	SelectObject(TimeDC, UserPen[0]);
}

void nxtim() {
	Polyline(TimeDC, FormLines, 2);
	TimePosition += TimeStep;
	FormLines[0].x = FormLines[1].x = TimePosition;
}

void snp(unsigned strt, unsigned fin) {
	unsigned	ind, tuns, acc, chklen, at = 0;
	fPOINT		l_rsiz;
	unsigned*	txhst;

	chkrng(&l_rsiz);
	Xpoints = (unsigned*)BSequence;
	ZeroMemory(BSequence, 65536 * sizeof(unsigned));
	Xhistogram = txhst = new unsigned[static_cast<int>(l_rsiz.x) + 1];
	for (ind = 0; ind < l_rsiz.x; ind++)
		Xhistogram[ind] = 0;
	if (chkMap(FORMSEL)) {
		at = (ClosestFormToCursor << 4)&FRMSK;
		for (ind = strt; ind < fin; ind++) {
			if (!(StitchBuffer[ind].attribute&NOTFRM) && (StitchBuffer[ind].attribute&FRMSK) == at) {
				tuns = StitchBuffer[ind].x;
				Xhistogram[tuns]++;
			}
		}
	}
	else {
		for (ind = strt; ind < fin; ind++) {
			tuns = StitchBuffer[ind].x;
			Xhistogram[tuns]++;
		}
	}
	acc = 0;
	for (ind = 0; ind < l_rsiz.x; ind++) {
		tuns = Xhistogram[ind];
		Xhistogram[ind] = acc;
		acc += tuns;
	}
	Xhistogram[ind] = acc;
	if (chkMap(FORMSEL)) {
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (!(StitchBuffer[ind].attribute&NOTFRM) && (StitchBuffer[ind].attribute&FRMSK) == at) {
				tuns = StitchBuffer[ind].x;
				Xpoints[Xhistogram[tuns]++] = ind;
			}
		}
	}
	else {
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			tuns = StitchBuffer[ind].x;
			Xpoints[Xhistogram[tuns]++] = ind;
		}
	}
	Xhistogram = &Xhistogram[1];
	chklen = SnapLength * 2 + 1;
	nutim(l_rsiz.x);
	for (ind = 0; ind < l_rsiz.x - chklen; ind++) {
		snpfn(ind, chklen);
		nxtim();
	}
	DestroyWindow(TimeWindow);
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
		mov		eax, ColorBitmap
		mov		ebx, bpnt
		bts		eax, ebx
		mov		ColorBitmap, eax
	}
#else
	_bittestandset((long *)&ColorBitmap, bpnt);
#endif
}

unsigned nxtcol() {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, ColorBitmap
		mov		ecx, AppliqueColor
		bt		ebx, ecx
		jnc		short nxtcol1
		mov		eax, ecx
		jmp		short nxtcolx
		nxtcol1 : bsf		eax, ebx
				  jne		short nxtcolx
				  dec		eax
				  nxtcolx : btc		ebx, eax
							mov		ColorBitmap, ebx
	}
#else
	DWORD col;
	if (_bittest((long *)&ColorBitmap, AppliqueColor)) {
		col = AppliqueColor;
	}
	else {
		if (ColorBitmap == 0) {
			return 0xffffffff;
		}

		_BitScanForward(&col, ColorBitmap);
	}

	_bittestandcomplement((long *)&ColorBitmap, col);

	return col;
#endif
}

unsigned isrt(unsigned bpnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, offset Srtmsk
		mov		ecx, bpnt
		bt[ebx], ecx
		jnc		isrtx
		inc		eax
		isrtx :
	}
#else
	return _bittest((long *)&Srtmsk, bpnt);
#endif
}

unsigned prgflg(unsigned ind) {
	if ((StitchBuffer[ind].attribute&TYPMSK) == FRMBFIL)
		return isrt(FormList[(StitchBuffer[ind].attribute&FRMSK) >> 4].edgeType&NEGUND);
	else
		return 0;
}

void rotpar() {
	if (IniFile.rotationAngle < (PI / 180))
		tabmsg(IDS_ROTIN);
	if (chkMap(FPSEL)) {
		RotationCenter.x = midl(SelectedVerticesRect.right, SelectedVerticesRect.left);
		RotationCenter.y = midl(SelectedVerticesRect.top, SelectedVerticesRect.bottom);
		return;
	}
	if (chkMap(BIGBOX)) {
		RotationCenter.x = midl(AllItemsRect.right, AllItemsRect.left);
		RotationCenter.y = midl(AllItemsRect.top, AllItemsRect.bottom);
		return;
	}
	if (SelectedFormCount) {
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

	LoadString(ThrEdInstance, IDS_ROTA, buf, HBUFSIZ);
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
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
		RotationAngle = IniFile.rotationAngle;
		rotpar();
		rotentr();
		setMap(ENTROT);
	}
	else
		shoseln(IDS_ALLX, IDS_ROTCMD);
}

void fnagain() {
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || SelectedFormCount || chkMap(BIGBOX) || chkMap(FPSEL)) {
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
	if (chkMap(FORMSEL) || chkMap(GRPSEL) || SelectedFormCount) {
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

	SelectedForm = &FormList[fpnt];
	nupnt = &FormList[FormIndex];
	frmcpy(nupnt, SelectedForm);
	ClosestFormToCursor = FormIndex;
	nupnt->vertices = adflt(SelectedForm->vertexCount);
	mvflpnt(nupnt->vertices, SelectedForm->vertices, SelectedForm->vertexCount);
	if (nupnt->type == SAT&&nupnt->satinGuideCount) {
		nupnt->satinOrAngle.guide = adsatk(nupnt->satinGuideCount);
		mvsatk(nupnt->satinOrAngle.guide, SelectedForm->satinOrAngle.guide, SelectedForm->satinGuideCount);
	}
	if (iseclpx(FormIndex)) {
		nupnt->borderClipData = adclp(nupnt->clipEntries);
		mvflpnt(nupnt->borderClipData, SelectedForm->borderClipData, SelectedForm->clipEntries);
	}
	if (isclpx(FormIndex)) {
		nupnt->angleOrClipData.clip = adclp(nupnt->lengthOrCount.clipCount);
		mvflpnt(nupnt->angleOrClipData.clip, SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
	}
	FormIndex++;
}

void duprot() {
	adfrm(ClosestFormToCursor);
	rotfn();
	refil();
	setMap(FORMSEL);
	setMap(RESTCH);
}

void duprotfs() {
	unsigned	l_bakpnt, ind;

	l_bakpnt = FormIndex;
	rotpar();
	for (ind = 0; ind < SelectedFormCount; ind++)
		adfrm(SelectedFormList[ind]);
	FormIndex = l_bakpnt;
	for (ind = 0; ind < SelectedFormCount; ind++)
		SelectedFormList[ind] = FormIndex++;
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

	l_spnt = &FormList[fpnt];
	fvars(FormIndex);
	frmcpy(SelectedForm, l_spnt);
	fvars(FormIndex);
	SelectedForm->vertices = adflt(SelectedForm->vertexCount);
	MoveMemory(SelectedForm->vertices, l_spnt->vertices, VertexCount * sizeof(fPOINT));
	if (SelectedForm->type == SAT&&SelectedForm->satinGuideCount)
	{
		SelectedForm->satinOrAngle.guide = adsatk(SelectedForm->satinGuideCount);
		MoveMemory(SelectedForm->satinOrAngle.guide, l_spnt->satinOrAngle.guide, SelectedForm->satinGuideCount * sizeof(SATCON));
	}
	SelectedForm->clipEntries = 0;
	SelectedForm->fillType = 0;
	SelectedForm->lengthOrCount.clipCount = 0;;
	SelectedForm->edgeType = 0;
	SelectedForm->fillInfo.texture.index = 0;
	SelectedForm->attribute = FormList[FormIndex].attribute&NFRMLMSK;
	SelectedForm->attribute |= play;
	FormIndex++;
}

void cpylayr(unsigned play) {
	unsigned ind, ine, playcod;

	if (SelectedFormCount) {
		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++)
			cplayfn(SelectedFormList[ind], play);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			cplayfn(ClosestFormToCursor, play);
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
	if (SelectedFormCount) {
		savdo();
		clRmap((FormIndex >> 5) + 1);
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ine = SelectedFormList[ind];
			slay = (FormList[ine].attribute&FRMLMSK) >> 1;
			FormList[ine].attribute = (TCHAR)(FormList[ine].attribute&NFRMLMSK) | play;
			setr(ine);
		}
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (StitchBuffer[ind].attribute&ALTYPMSK) {
				ine = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
				if (chkr(ine))
					StitchBuffer[ind].attribute = StitchBuffer[ind].attribute&NLAYMSK | stchcod;
			}
		}
		SelectedFormCount = 0;
		rstMap(FORMSEL);
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			savdo();
			slay = (FormList[ClosestFormToCursor].attribute&FRMLMSK) >> 1;
			FormList[ClosestFormToCursor].attribute = (TCHAR)(FormList[ClosestFormToCursor].attribute&NFRMLMSK) | play;
			rstMap(FORMSEL);
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if (StitchBuffer[ind].attribute&ALTYPMSK && ((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor)
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

	l_bakpnt = ClosestFormToCursor;
	setMap(FRMSAM);
	if (FormIndex > 1 && chkMap(FORMSEL) && closfrm()) {
		l_sids = FormList[ClosestFormToCursor].vertexCount;
		tflt = (fPOINT*)&BSequence;
		for (ind = 0; ind < l_sids; ind++) {
			tflt[ind].x = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].x;
			tflt[ind].y = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].y;
			ClosestVertexToCursor = nxt(ClosestVertexToCursor);
		}
		setMap(DELTO);
		frmdel();
		if (l_bakpnt > ClosestFormToCursor)
			ClosestFormToCursor = l_bakpnt - 1;
		else
			ClosestFormToCursor = l_bakpnt;
		ipnt = &FormList[ClosestFormToCursor].vertices[FormList[ClosestFormToCursor].vertexCount];
		fltspac(ipnt, l_sids);
		for (ind = 0; ind < l_sids; ind++) {
			ipnt[ind].x = tflt[ind].x;
			ipnt[ind].y = tflt[ind].y;
		}
		SelectedForm = &FormList[ClosestFormToCursor];
		SelectedForm->vertexCount += l_sids;
		SelectedForm->rectangle.left = SelectedForm->rectangle.right = SelectedForm->vertices[0].x;
		SelectedForm->rectangle.top = SelectedForm->rectangle.bottom = SelectedForm->vertices[0].y;
		for (ind = 1; ind < SelectedForm->vertexCount; ind++) {
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
	l_bakpnt = ClosestFormToCursor;
	for (ClosestFormToCursor = 0; ClosestFormToCursor < FormIndex; ClosestFormToCursor++)
		refilfn();
	ClosestFormToCursor = l_bakpnt;
	fvars(ClosestFormToCursor);
	coltab();
	setMap(RESTCH);
}

BOOL notsel() {
	unsigned ind;

	for (ind = 0; ind < SelectedFormCount; ind++) {
		if ((unsigned)SelectedFormList[ind] == ClosestFormToCursor)
			return 0;
	}
	return 1;
}

void nufsel() {
	if (FormIndex) {
		rstMap(FORMSEL);
		if (rstMap(WASEL))
			SelectedFormList[SelectedFormCount++] = PreviousFormIndex;
		if (notsel())
			SelectedFormList[SelectedFormCount++] = (unsigned short)ClosestFormToCursor;
		setMap(RESTCH);
	}
}

void frmadj(unsigned find) {
	unsigned ind;

	fvars(find);
	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
		CurrentFormVertices[ind].x += FormMoveDelta.x;
		CurrentFormVertices[ind].y -= FormMoveDelta.y;
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

void clRmap(unsigned mapSize) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ecx, mapSize
		mov		edi, offset MarkedStitchMap
		rep		stosd
	}
#else
	memset(MarkedStitchMap, 0, mapSize * sizeof(*MarkedStitchMap));
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
	for (ind = 0; ind < SelectedFormCount; ind++)
		setr(SelectedFormList[ind]);
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		if (StitchBuffer[ind].attribute&ALTYPMSK&&chkr((StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT)) {
			StitchBuffer[ind].x += FormMoveDelta.x;
			StitchBuffer[ind].y -= FormMoveDelta.y;
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

	AllItemsRect.bottom = AllItemsRect.left = 1e9;
	AllItemsRect.top = AllItemsRect.right = 0;
	for (ind = 0; ind < FormIndex; ind++) {
		trct = &FormList[ind].rectangle;
		if (trct->bottom < AllItemsRect.bottom)
			AllItemsRect.bottom = trct->bottom;
		if (trct->left < AllItemsRect.left)
			AllItemsRect.left = trct->left;
		if (trct->right > AllItemsRect.right)
			AllItemsRect.right = trct->right;
		if (trct->top > AllItemsRect.top)
			AllItemsRect.top = trct->top;
	}
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		if (StitchBuffer[ind].x < AllItemsRect.left)
			AllItemsRect.left = StitchBuffer[ind].x;
		if (StitchBuffer[ind].x > AllItemsRect.right)
			AllItemsRect.right = StitchBuffer[ind].x;
		if (StitchBuffer[ind].y < AllItemsRect.bottom)
			AllItemsRect.bottom = StitchBuffer[ind].y;
		if (StitchBuffer[ind].y > AllItemsRect.top)
			AllItemsRect.top = StitchBuffer[ind].y;
	}
}

void selal() {
	rstMap(FORMSEL);
	SelectedFormCount = 0;
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
	stchrct2px(AllItemsRect, &SelectedFormsRect);
	setMap(BIGBOX);
	setMap(RESTCH);
}

void frmpnts(unsigned typ) {
	unsigned ind = 0;
	unsigned trg = (ClosestFormToCursor << 4) | typ;

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
		if (typ == FRMFIL && !FormList[ClosestFormToCursor].fillType) {
			tabmsg(IDS_FSELF);
			return;
		}
		if (typ == FRMBFIL && !FormList[ClosestFormToCursor].edgeType) {
			tabmsg(IDS_FSELB);
			return;
		}
		if (typ == FRMAPFIL && (FormList[ClosestFormToCursor].edgeType&NEGUND) != EDGEAPPL) {
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
	if (((at&FRMSK) >> FRMSHFT) == ClosestFormToCursor)
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
	if (FormList[fpnt].fillType || FormList[fpnt].edgeType) {
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

	dif.x = CurrentFormVertices[fin].x - CurrentFormVertices[strt].x;
	dif.y = CurrentFormVertices[fin].y - CurrentFormVertices[strt].y;
	RotationAngle = atan2(-dif.x, dif.y);
	ostp.x = SelectedForm->borderSize*cos(RotationAngle);
	ostp.y = SelectedForm->borderSize*sin(RotationAngle);
	len = hypot(dif.x, dif.y);
	cnt = len / spac;
	stp.x = dif.x / cnt;
	stp.y = dif.y / cnt;
	pnti.x = CurrentFormVertices[strt].x;
	pnti.y = CurrentFormVertices[strt].y;
	if (cnt) {
		for (ind = 0; ind < cnt - 1; ind++) {
			pntf.x = pnti.x + stp.x;
			pntf.y = pnti.y + stp.y;
			pnto.x = pntf.x + ostp.x;
			pnto.y = pntf.y + ostp.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			OSequence[SequenceIndex].x = pnti.x;
			OSequence[SequenceIndex++].y = pnti.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			OSequence[SequenceIndex].x = pnto.x;
			OSequence[SequenceIndex++].y = pnto.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			OSequence[SequenceIndex].x = pnto.x;
			OSequence[SequenceIndex++].y = pnto.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			pnti.x += stp.x;
			pnti.y += stp.y;
		}
		pntf.x = pnti.x + stp.x;
		pntf.y = pnti.y + stp.y;
		OSequence[SequenceIndex].x = pntf.x;
		OSequence[SequenceIndex++].y = pntf.y;
		OSequence[SequenceIndex].x = pnti.x;
		OSequence[SequenceIndex++].y = pnti.y;
	}
}

void bhcrnr(unsigned p_lin) {
	unsigned	tlin = nxt(p_lin);
	dPOINT		dif, pnt;
	double		len, rat;

	if (chkMap(INDIR))
	{
		dif.x = OutsidePoints[tlin].x - CurrentFormVertices[tlin].x;
		dif.y = OutsidePoints[tlin].y - CurrentFormVertices[tlin].y;
	}
	else
	{
		dif.x = InsidePoints[tlin].x - CurrentFormVertices[tlin].x;
		dif.y = InsidePoints[tlin].y - CurrentFormVertices[tlin].y;
	}
	len = hypot(dif.x, dif.y);
	rat = ButtonholeCornerLength / len;
	dif.x *= rat;
	dif.y *= rat;
	pnt.x = CurrentFormVertices[tlin].x + dif.x;
	pnt.y = CurrentFormVertices[tlin].y + dif.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	OSequence[SequenceIndex].x = pnt.x;
	OSequence[SequenceIndex++].y = pnt.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	OSequence[SequenceIndex].x = pnt.x;
	OSequence[SequenceIndex++].y = pnt.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
}

void bhbrd(double spac) {
	unsigned short	slin;
	unsigned short	nlin, ind;

	slin = getlast();
	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[slin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[slin].y;
	for (ind = 0; ind < VertexCount; ind++) {
		nlin = nxt(slin);
		bhfn(slin, nlin, spac);
		bhcrnr(slin);
		slin = nlin;
	}
}

void blbrd(double spac) {
	unsigned ind;

	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[0].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[0].y;
	for (ind = 0; ind < (unsigned)VertexCount - 2; ind++) {
		bhfn(ind, ind + 1, spac);
		bhcrnr(ind);
	}
	bhfn(ind, ind + 1, spac);
	OSequence[SequenceIndex].x = CurrentFormVertices[VertexCount - 1].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[VertexCount - 1].y;
}

void bholbrd() {
	deleclp(ClosestFormToCursor);
	SelectedForm->borderSize = BorderWidth;
	bsizpar();
	SelectedForm->edgeType = EDGEBHOL;
	SelectedForm->edgeSpacing = StitchSpacing;
	SelectedForm->borderColor = ActiveColor;
	savblen(ButtonholeCornerLength);
	refilfn();
}

void bhol() {
	double		tspac = StitchSpacing;
	unsigned	ind;

	if (filmsgs(FML_BHOL))
		return;
	StitchSpacing = tspac;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
			bholbrd();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fvars(ClosestFormToCursor);
			bholbrd();
			setMap(INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			setMap(RESTCH);
		}
	}
	StitchSpacing = tspac;
}

void fcntr() {
	unsigned	ind, ine, inf, tat;
	dRECTANGLE		avrct;
	dPOINT		avcntr;
	dPOINT		cntr;
	dPOINT		dif;

	avrct.bottom = avrct.left = avrct.right = avrct.top = 0;
	if (SelectedFormCount) {
		savdo();
		ine = SelectedFormList[0];
		avcntr.x = (FormList[ine].rectangle.right - FormList[ine].rectangle.left) / 2 + FormList[ine].rectangle.left;
		avcntr.y = (FormList[ine].rectangle.top - FormList[ine].rectangle.bottom) / 2 + FormList[ine].rectangle.bottom;
		for (ind = 1; ind < SelectedFormCount; ind++) {
			ine = SelectedFormList[ind];
			cntr.x = (FormList[ine].rectangle.right - FormList[ine].rectangle.left) / 2 + FormList[ine].rectangle.left;
			cntr.y = (FormList[ine].rectangle.top - FormList[ine].rectangle.bottom) / 2 + FormList[ine].rectangle.bottom;
			dif.x = avcntr.x - cntr.x;
			dif.y = avcntr.y - cntr.y;
			SelectedForm = &FormList[ine];
			for (inf = 0; inf < SelectedForm->vertexCount; inf++) {
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
		SetCapture(ThrEdWindow);
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
		dif.x = OutsidePoints[tlin].x - CurrentFormVertices[tlin].x;
		dif.y = OutsidePoints[tlin].y - CurrentFormVertices[tlin].y;
	}
	else
	{
		dif.x = InsidePoints[tlin].x - CurrentFormVertices[tlin].x;
		dif.y = InsidePoints[tlin].y - CurrentFormVertices[tlin].y;
	}
	RotationAngle = atan2(dif.y, dif.x) + PI / 2;
	rotang1(rpnt, &ClipReference);
	for (ind = 0; ind < ClipStitchCount; ind++)
		rotang1(ClipBuffer[ind], &ClipFillData[ind]);
	len = hypot(dif.x, dif.y);
	rat = getplen() / len;
	dif.x *= rat;
	dif.y *= rat;
	pnt.x = CurrentFormVertices[tlin].x + dif.x;
	pnt.y = CurrentFormVertices[tlin].y + dif.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	OSequence[SequenceIndex].x = pnt.x;
	OSequence[SequenceIndex++].y = pnt.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	OSequence[SequenceIndex].x = pnt.x;
	OSequence[SequenceIndex++].y = pnt.y;
	tpnt.x = pnt.x;
	tpnt.y = pnt.y;
	if (!ritclp(tpnt)) {
		OSequence[SequenceIndex].x = pnt.x;
		OSequence[SequenceIndex++].y = pnt.y;
		OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	}
}

void picfn(unsigned strt, unsigned fin, double spac) {
	double		len, tdub;
	dPOINT		dif, stp, ostp, pnti, pntf, pnto;
	fPOINT		tpnt;
	fPOINTATTR	rpnt;
	unsigned	cnt, ind;

	dif.x = CurrentFormVertices[fin].x - CurrentFormVertices[strt].x;
	dif.y = CurrentFormVertices[fin].y - CurrentFormVertices[strt].y;
	RotationAngle = atan2(-dif.x, dif.y);
	ostp.x = SelectedForm->borderSize*cos(RotationAngle);
	ostp.y = SelectedForm->borderSize*sin(RotationAngle);
	len = hypot(dif.x, dif.y);
	spac += ClipRectSize.cx;
	cnt = len / spac;
	rpnt.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	rpnt.y = ClipRect.top;
	RotationAngle = atan2(dif.y, dif.x);
	rotang1(rpnt, &ClipReference);
	pnti.x = CurrentFormVertices[strt].x;
	pnti.y = CurrentFormVertices[strt].y;
	if (cnt) {
		stp.x = 0;
		stp.y = 0;
		if (cnt > 1) {
			tdub = ((len - cnt*spac) / (cnt - 1) + spac) / len;
			stp.x = dif.x*tdub;
			stp.y = dif.y*tdub;
		}
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotang1(ClipBuffer[ind], &ClipFillData[ind]);
		for (ind = 0; ind < cnt - 1; ind++) {
			pntf.x = pnti.x + stp.x;
			pntf.y = pnti.y + stp.y;
			pnto.x = pntf.x + ostp.x;
			pnto.y = pntf.y + ostp.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			OSequence[SequenceIndex].x = pnti.x;
			OSequence[SequenceIndex++].y = pnti.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			OSequence[SequenceIndex].x = pnto.x;
			OSequence[SequenceIndex++].y = pnto.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			OSequence[SequenceIndex].x = pnto.x;
			OSequence[SequenceIndex++].y = pnto.y;
			tpnt.x = pnto.x;
			tpnt.y = pnto.y;
			if (ritclp(tpnt))
				goto picfnx;
			OSequence[SequenceIndex].x = pnto.x;
			OSequence[SequenceIndex++].y = pnto.y;
			OSequence[SequenceIndex].x = pntf.x;
			OSequence[SequenceIndex++].y = pntf.y;
			pnti.x += stp.x;
			pnti.y += stp.y;
		}
		OSequence[SequenceIndex].x = CurrentFormVertices[fin].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[fin].y;
		OSequence[SequenceIndex].x = pnti.x;
		OSequence[SequenceIndex++].y = pnti.y;
	}
picfnx:;
}

void clpic(unsigned short strtlin) {
	unsigned		ind;
	unsigned short	nlin;

	SequenceIndex = 0;
	rstMap(CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength = ClipRectSize.cx;
	ClipFillData = new fPOINT[ClipStitchCount];
	RotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	ClipReference.y = RotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	ClipReference.x = ClipRect.left;
	satout(20);
	if (SelectedForm->type == FRMLINE) {
		for (ind = 0; ind < (unsigned)VertexCount - 2; ind++) {
			picfn(ind, ind + 1, SelectedForm->edgeSpacing);
			clpcrnr(ind);
		}
		picfn(ind, ind + 1, SelectedForm->edgeSpacing);
	}
	else {
		if (!SelectedForm->fillType) {
			OSequence[SequenceIndex].x = CurrentFormVertices[strtlin].x;
			OSequence[SequenceIndex++].y = CurrentFormVertices[strtlin].y;
		}
		for (ind = 0; ind < VertexCount; ind++) {
			nlin = nxt(strtlin);
			picfn(strtlin, nlin, SelectedForm->edgeSpacing);
			clpcrnr(strtlin);
			strtlin = nlin;
		}
		OSequence[SequenceIndex].x = CurrentFormVertices[strtlin].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[strtlin].y;
	}
	delete[] ClipFillData;
}

void fspic() {
	unsigned	ind;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEPICOT;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = PicotSpacing;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	savplen(ButtonholeCornerLength);
	for (ind = 0; ind < ClipStitchCount; ind++) {
		SelectedForm->borderClipData[ind].x = ClipBuffer[ind].x;
		SelectedForm->borderClipData[ind].y = ClipBuffer[ind].y;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	refilfn();
}

void picot() {
	unsigned ind;

	if (filmsgs(FML_PIC))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		savdo();
		fvars(ClosestFormToCursor);
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (SelectedFormCount) {
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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

	SequenceIndex = 0;
	strt = SelectedForm->angleOrClipData.guide.start;
	fin = SelectedForm->angleOrClipData.guide.finish;
	hilins = VertexCount - strt - 1;
	lolins = strt;
	selins = fin - strt;
	sels = &CurrentFormVertices[strt];
	his = &CurrentFormVertices[fin];
	lolen = hilen = 0;

	locnts = (unsigned*)BSequence;
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
		loflts[loind].x = CurrentFormVertices[ind].x;
		loflts[loind].y = CurrentFormVertices[ind].y;
		lodifs[loind].x = CurrentFormVertices[ind - 1].x - CurrentFormVertices[ind].x;
		lodifs[loind].y = CurrentFormVertices[ind - 1].y - CurrentFormVertices[ind].y;
		lolens[loind] = hypot(lodifs[loind].x, lodifs[loind].y);
		lolen += lolens[loind];
		loind++;
	}
	selind = 0;
	for (ind = strt + 1; ind <= fin; ind++) {
		dif.x = CurrentFormVertices[ind].x - sels[0].x;
		dif.y = CurrentFormVertices[ind].y - sels[0].y;
		pols[selind].length = hypot(dif.x, dif.y);
		pols[selind].angle = atan2(dif.y, dif.x);
		selind++;
	}
	hind = 0; hilen = 0;
	for (ind = fin; ind < (unsigned)VertexCount - 1; ind++) {
		hiflts[hind].x = CurrentFormVertices[ind].x;
		hiflts[hind].y = CurrentFormVertices[ind].y;
		hidifs[hind].x = CurrentFormVertices[ind + 1].x - CurrentFormVertices[ind].x;
		hidifs[hind].y = CurrentFormVertices[ind + 1].y - CurrentFormVertices[ind].y;
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
	dif.x = CurrentFormVertices[fin].x - CurrentFormVertices[strt].x;
	dif.y = CurrentFormVertices[fin].y - CurrentFormVertices[strt].y;
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
		if (polref.length > 0.9*StitchSpacing) {
			poldif.length = polin.length / polref.length;
			if (toglMap(FILDIR)) {
				OSequence[SequenceIndex].x = lopnt.x;
				OSequence[SequenceIndex].y = lopnt.y;
				SequenceIndex++;
				for (ind = 0; ind < (selins - 1); ind++) {
					RotationAngle = pols[ind].angle + poldif.angle;
					len = pols[ind].length*poldif.length;
					OSequence[SequenceIndex].x = lopnt.x + cos(RotationAngle)*len;
					OSequence[SequenceIndex].y = lopnt.y + sin(RotationAngle)*len;
					SequenceIndex++;
				}
			}
			else {
				OSequence[SequenceIndex].x = hipnt.x;
				OSequence[SequenceIndex].y = hipnt.y;
				SequenceIndex++;
				for (ind = selins - 1; ind != 0; ind--) {
					RotationAngle = pols[ind - 1].angle + poldif.angle;
					len = pols[ind - 1].length*poldif.length;
					OSequence[SequenceIndex].x = lopnt.x + cos(RotationAngle)*len;
					OSequence[SequenceIndex].y = lopnt.y + sin(RotationAngle)*len;
					SequenceIndex++;
				}
			}
		}
		lopnt.x += lostp.x;
		lopnt.y += lostp.y;
		hipnt.x += histp.x;
		hipnt.y += histp.y;
	}
	if (chkMap(FILDIR)) {
		OSequence[SequenceIndex].x = CurrentFormVertices[0].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[0].y;
	}
	else {
		OSequence[SequenceIndex].x = CurrentFormVertices[VertexCount - 1].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[VertexCount - 1].y;
	}
	if (SelectedForm->lengthOrCount.stitchLength < MinStitchLength)
		SelectedForm->lengthOrCount.stitchLength = MinStitchLength;
}

BOOL contsf(unsigned find)
{
	ClosestFormToCursor = find;
	fvars(find);
	if (SelectedForm->vertexCount > 4)
	{
		delclps(ClosestFormToCursor);
		deltx();
		chkcont();
		SelectedForm->fillSpacing = StitchSpacing;
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
	if (SelectedFormCount) {
		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++)
			contsf(SelectedFormList[ind]);
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		fvars(ClosestFormToCursor);
		savdo();
		if (contsf(ClosestFormToCursor))
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
		fvars(ClosestFormToCursor);
		if (VertexCount > 2) {
			savdo();
			clobak = ClosestFormToCursor;
			satout(BorderWidth);

			HorizontalLength2 = BorderWidth / 2;
			tfrm = &FormList[FormIndex];
			frmclr(tfrm);
			fpnt = 0;
			tfrm->maxFillStitchLen = 9 * PFGRAN;
			tfrm->minFillStitchLen = MinStitchLength;
			MaxStitchLen = 9 * PFGRAN;
			if (SelectedForm->type == FRMLINE) {
				if (chku(BLUNT))
					ind = 0xffffffff;
				else
					ind = 0;
				satends(ind);
				tfrm->vertices = adflt(VertexCount << 1);
				tfrm->vertices[0].x = OutsidePoints[0].x;
				tfrm->vertices[fpnt++].y = OutsidePoints[0].y;
				for (ind = 0; ind < VertexCount; ind++) {
					tfrm->vertices[fpnt].x = InsidePoints[ind].x;
					tfrm->vertices[fpnt++].y = InsidePoints[ind].y;
				}
				for (ind = VertexCount - 1; ind; ind--) {
					tfrm->vertices[fpnt].x = OutsidePoints[ind].x;
					tfrm->vertices[fpnt++].y = OutsidePoints[ind].y;
				}
			}
			else {
				tfrm->vertices = adflt((VertexCount << 1) + 2);
				tfrm->vertices[0].x = OutsidePoints[0].x;
				tfrm->vertices[fpnt++].y = OutsidePoints[0].y;
				tfrm->underlayIndent = IniFile.underlayIndent;
				for (ind = 0; ind < VertexCount; ind++) {
					tfrm->vertices[fpnt].x = InsidePoints[ind].x;
					tfrm->vertices[fpnt++].y = InsidePoints[ind].y;
				}
				tfrm->vertices[fpnt].x = InsidePoints[0].x;
				tfrm->vertices[fpnt++].y = InsidePoints[0].y;
				tfrm->vertices[fpnt].x = OutsidePoints[0].x;
				tfrm->vertices[fpnt++].y = OutsidePoints[0].y;
				for (ind = VertexCount - 1; ind; ind--) {
					tfrm->vertices[fpnt].x = OutsidePoints[ind].x;
					tfrm->vertices[fpnt++].y = OutsidePoints[ind].y;
				}
			}
			tfrm->type = SAT;
			tfrm->fillColor = ActiveColor;
			tfrm->fillSpacing = StitchSpacing;
			tfrm->lengthOrCount.stitchLength = IniFile.maxStitchLength;
			tfrm->vertexCount = fpnt;
			tfrm->attribute = 1;
			tfrm->wordParam = fpnt >> 1;
			tfrm->satinGuideCount = tfrm->wordParam - 2;
			tfrm->satinOrAngle.guide = adsatk(tfrm->satinGuideCount);
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
				tfrm->satinOrAngle.guide[ind].start = ind + 2;
				tfrm->satinOrAngle.guide[ind].finish = tfrm->vertexCount - ind - 1;
			}
			FormIndex++;
			frmout(FormIndex - 1);
			ClosestFormToCursor = FormIndex - 1;
			refilfn();
			ClosestFormToCursor = clobak;
			setMap(DELTO);
			frmdel();
			ClosestFormToCursor = FormIndex - 1;
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
				if (SelectedFormCount)
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
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++) {
		dif.x = CurrentFormVertices[ind + 1].x - CurrentFormVertices[ind].x;
		dif.y = CurrentFormVertices[ind + 1].y - CurrentFormVertices[ind].y;
		len = hypot(dif.x, dif.y);
		cnt = len / ClipRectSize.cx + 0.5;
		rat = (ClipRectSize.cx*cnt + 0.004) / len;
		CurrentFormVertices[ind + 1].x = CurrentFormVertices[ind].x + dif.x*rat;
		CurrentFormVertices[ind + 1].y = CurrentFormVertices[ind].y + dif.y*rat;
	}
	dif.x = CurrentFormVertices[0].x - CurrentFormVertices[1].x;
	dif.y = CurrentFormVertices[0].y - CurrentFormVertices[1].y;
	len = hypot(dif.x, dif.y);
	RotationAngle = atan2(dif.y, dif.x);
	for (ine = 0; ine < 5; ine++) {
		dif.x = CurrentFormVertices[0].x - CurrentFormVertices[ind].x;
		dif.y = CurrentFormVertices[0].y - CurrentFormVertices[ind].y;
		dlen = hypot(dif.x, dif.y);
		cnt = dlen / ClipRectSize.cx;
		adif = dlen - cnt*ClipRectSize.cx;
		RotationAngle -= adif / len;
		dif.x = cos(RotationAngle)*len;
		dif.y = sin(RotationAngle)*len;
		CurrentFormVertices[0].x = CurrentFormVertices[1].x + dif.x;
		CurrentFormVertices[0].y = CurrentFormVertices[1].y + dif.y;
	}
	refil();
}

void shrnk() {
	fvars(ClosestFormToCursor);
	if (chkMap(FORMSEL) && SelectedForm->edgeType == EDGECLIP) {
		shrnks();
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FRMCLP, IDS_SHRNK);
}

void mvfrms(FRMHED* destination, FRMHED* source, unsigned count) {
#if	 __UseASM__
	_asm {
		mov		esi, source
		mov		edi, destination
		mov		eax, count
		mov		ecx, fsizeof
		mul		ecx
		mov		ecx, eax
		rep		movsd
	}
#else
	memcpy(destination, source, count * sizeof (FRMHED));
#endif
}

void dufdat(unsigned find) {
	FRMHED* dst;
	FRMHED* src;

	dst = &TempFormList[FormRelocationIndex++];
	src = &FormList[find];
	mvfrms(dst, src, 1);
	mvflpnt(&TempFormVertices[FormVertexIndex], dst->vertices, dst->vertexCount);
	dst->vertices = &FormVertices[FormVertexIndex];
	FormVertexIndex += dst->vertexCount;
	if (dst->satinGuideCount) {
		mvsatk(&TempSatinConnects[SatinConnectIndex], dst->satinOrAngle.guide, dst->satinGuideCount);
		dst->satinOrAngle.guide = &SatinConnects[SatinConnectIndex];
		SatinConnectIndex += dst->satinGuideCount;
	}
	if (iseclpx(find))
	{
		mvflpnt(&TempClipPoints[ClipPointIndex], dst->borderClipData, dst->clipEntries);
		dst->borderClipData = &ClipPoints[ClipPointIndex];
		ClipPointIndex += dst->clipEntries;
	}
	if (isclpx(find))
	{
		mvflpnt(&TempClipPoints[ClipPointIndex], dst->angleOrClipData.clip, dst->lengthOrCount.clipCount);
		dst->angleOrClipData.clip = &ClipPoints[ClipPointIndex];
		ClipPointIndex += dst->lengthOrCount.clipCount;
	}
}

void stchfrm(unsigned fnum, unsigned* attribute) {
#if	 __UseASM__
	_asm {
		mov		eax, fnum
		shl		eax, FRMSHFT
		mov		ebx, attribute
		mov		ecx, [ebx]
		and ecx, NFRMSK
		or ecx, eax
		mov[ebx], ecx
	}
#else
	*attribute &= NFRMSK;
	*attribute |= fnum << FRMSHFT;
#endif
}

void frmnumfn(unsigned nunum) {
	unsigned	ind, srcpnt, strt, fin, cod;

	if (nunum != ClosestFormToCursor) {
		if (ClosestFormToCursor > nunum) {
			strt = nunum;
			fin = ClosestFormToCursor;
		}
		else {
			strt = ClosestFormToCursor;
			fin = nunum;
		}
		srcpnt = FormRelocationIndex = 0;
		TempFormList = (FRMHED*)&BSequence;
		TempFormVertices = (fPOINT*)&TempFormList[FormIndex];
		TempSatinConnects = (SATCON*)&OSequence;
		TempClipPoints = (fPOINT*)&TempSatinConnects[SatinConnectIndex];
		FormVertexIndex = SatinConnectIndex = ClipPointIndex = 0;
		for (ind = 0; ind < FormIndex; ind++) {
			if (ind == nunum)
				dufdat(ClosestFormToCursor);
			else {
				if (srcpnt == ClosestFormToCursor)
					srcpnt++;
				dufdat(srcpnt++);
			}
		}
		mvfrms(FormList, TempFormList, FormIndex);
		mvflpnt(FormVertices, TempFormVertices, FormVertexIndex);
		mvsatk(SatinConnects, TempSatinConnects, SatinConnectIndex);
		mvflpnt(ClipPoints, TempClipPoints, ClipPointIndex);
		for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
			if (StitchBuffer[ind].attribute&TYPMSK) {
				cod = (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT;
				if (cod == ClosestFormToCursor)
					stchfrm(nunum, &StitchBuffer[ind].attribute);
				else {
					if (cod >= strt&&cod <= fin) {
						if (nunum < ClosestFormToCursor)
							stchfrm(cod + 1, &StitchBuffer[ind].attribute);
						else
							stchfrm(cod - 1, &StitchBuffer[ind].attribute);
					}
				}
			}
		}
		ClosestFormToCursor = nunum;
		ritnum(STR_NUMFRM, ClosestFormToCursor);
	}
}

void frmnum() {
	TCHAR	buf[HBUFSIZ];

	if (FormIndex&&chkMap(FORMSEL)) {
		LoadString(ThrEdInstance, IDS_FRML, buf, HBUFSIZ);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, FormIndex);
		shoMsg(MsgBuffer);
		setMap(NUMIN);
		setMap(ENTRFNUM);
		numWnd();
	}
	else
		shoseln(IDS_FRM1MSG, IDS_SETFRM);
}

unsigned duat(unsigned attribute) {
#if	 __UseASM__
	_asm {
		mov		eax, attribute
		mov		ebx, eax
		shr		eax, TYPSHFT
		inc		eax
		and		al, 3
		and		ebx, FRMSK
		shr		ebx, 2
		or		eax, ebx
	}
#else
	unsigned type = ((attribute >> TYPSHFT) + 1) & 3;
	unsigned frm = (attribute & FRMSK) >> 2;

	return type | frm;
#endif
}

void srtf(unsigned strt, unsigned fin) {
	unsigned*	frmhst;
	unsigned	ind, ine, tmp;

	if (strt != fin) {
		frmhst = (unsigned*)&BSequence;
		for (ind = 0; ind < FormIndex << 2; ind++)
			frmhst[ind] = 0;
		for (ind = strt; ind < fin; ind++)
			frmhst[duat(TempStitchBuffer[ind].attribute)]++;
		ine = strt;
		for (ind = 0; ind < FormIndex << 2; ind++) {
			tmp = frmhst[ind];
			frmhst[ind] = ine;
			ine += tmp;
		}
		for (ind = strt; ind < fin; ind++)
			moveStitch(&StitchBuffer[frmhst[duat(TempStitchBuffer[ind].attribute)]++], &TempStitchBuffer[ind]);
	}
}

void srtbyfrm() {
	unsigned	ind, ine, tmp;
	unsigned	colhst[16];
	unsigned	colr[16];

	if (FormIndex) {
		savdo();
		colr[AppliqueColor] = 0;
		for (ind = 0; ind < 16; ind++) {
			if (ind != AppliqueColor)
				colr[ind] = ind + 1;
		}
		TempStitchBuffer = &StitchBuffer[MAXSEQ];
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
			moveStitch(&TempStitchBuffer[colhst[colr[StitchBuffer[ind].attribute & 0xf]]++], &StitchBuffer[ind]);
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

	trct = &FormList[SelectedFormList[0]].rectangle;
	brct.left = trct->left;
	brct.right = trct->right;
	brct.top = trct->top;
	brct.bottom = trct->bottom;
	for (ind = 1; ind < SelectedFormCount; ind++) {
		trct = &FormList[SelectedFormList[ind]].rectangle;
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
	if (SelectedFormCount) {
		flg = 1;
		savdo();
		dufcntr(&selcntr);
		FormMoveDelta.x = mrkcntr.x - selcntr.x;
		FormMoveDelta.y = -mrkcntr.y + selcntr.y;
		if (chkMap(CNTRV))
			FormMoveDelta.y = 0;
		if (chkMap(CNTRH))
			FormMoveDelta.x = 0;
		for (ind = 0; ind < SelectedFormCount; ind++)
			frmadj(SelectedFormList[ind]);
		frmsadj();
	}
	else {
		if (chkMap(FORMSEL)) {
			flg = 1;
			savdo();
			trct = &FormList[ClosestFormToCursor].rectangle;
			selcntr.x = (trct->right - trct->left) / 2 + trct->left;
			selcntr.y = (trct->top - trct->bottom) / 2 + trct->bottom;
			FormMoveDelta.x = mrkcntr.x - selcntr.x;
			FormMoveDelta.y = -mrkcntr.y + selcntr.y;
			if (chkMap(CNTRV))
				FormMoveDelta.y = 0;
			if (chkMap(CNTRH))
				FormMoveDelta.x = 0;
			frmadj(ClosestFormToCursor);
			for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
				if (StitchBuffer[ind].attribute&ALTYPMSK && (StitchBuffer[ind].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor) {
					StitchBuffer[ind].x += FormMoveDelta.x;
					StitchBuffer[ind].y -= FormMoveDelta.y;
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
				FormMoveDelta.x = mrkcntr.x - selcntr.x;
				FormMoveDelta.y = -mrkcntr.y + selcntr.y;
				if (chkMap(CNTRV))
					FormMoveDelta.y = 0;
				if (chkMap(CNTRH))
					FormMoveDelta.x = 0;
				for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++) {
					StitchBuffer[ind].x += FormMoveDelta.x;
					StitchBuffer[ind].y -= FormMoveDelta.y;
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
	dcntr.x = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
	dcntr.y = (AllItemsRect.top - AllItemsRect.bottom) / 2 + AllItemsRect.bottom;
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
	for (ind = 0; ind < FormIndex; ind++)
		frmout(ind);
	setfchk();
	setMap(RESTCH);
}

void bean(unsigned strt, unsigned fin) {
	unsigned ind, ine, inf;

	ine = MAXSEQ;
	ind = strt;
	BeanCount = 0;
	mvstch(ine++, ind);
	if (StitchBuffer[ind + 2].x != StitchBuffer[ind].x || StitchBuffer[ind + 2].y != StitchBuffer[ind].y) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		BeanCount += 2;
	}
	ind++;
	mvstch(ine++, ind);
	if (StitchBuffer[ind + 2].x != StitchBuffer[ind].x || StitchBuffer[ind + 2].y != StitchBuffer[ind].y) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		BeanCount += 2;
	}
	ind++;
	while (ind < (unsigned)fin - 2) {
		mvstch(ine++, ind);
		if ((StitchBuffer[ind + 2].x != StitchBuffer[ind].x || StitchBuffer[ind + 2].y != StitchBuffer[ind].y) &&
			(StitchBuffer[ind - 2].x != StitchBuffer[ind].x || StitchBuffer[ind - 2].y != StitchBuffer[ind].y)) {
			mvstch(ine++, ind + 1);
			mvstch(ine++, ind);
			BeanCount += 2;
		}
		ind++;
	}
	mvstch(ine++, ind);
	if ((StitchBuffer[ind - 2].x != StitchBuffer[ind].x || StitchBuffer[ind - 2].y != StitchBuffer[ind].y)) {
		mvstch(ine++, ind + 1);
		mvstch(ine++, ind);
		BeanCount += 2;
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
				ClosestPointIndex += BeanCount;
			else
				GroupStitchIndex += BeanCount;
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
	BeanCount = 0;
	for (ind = strt; ind <= fin; ind++) {
		mvstch(ine++, ind);
		if (StitchBuffer[ind].x == StitchBuffer[ind + 2].x&&StitchBuffer[ind].y == StitchBuffer[ind + 2].y) {
			ind += 2;
			BeanCount += 2;
		}
	}
	if (BeanCount)
		BeanCount -= 2;;
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
			ClosestPointIndex -= BeanCount;
		else
			GroupStitchIndex -= BeanCount;
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

void mvfrmsb(FRMHED* destination, FRMHED* source, unsigned count) {
#if	 __UseASM__
	_asm {
		std
		mov		eax, fsizeof
		mul		count
		mov		ecx, eax
		mov		edi, destination
		add		edi, 64
		mov		esi, source
		add		esi, 64
		rep		movsd
		cld
	}
#else
	memmove(destination, source, count * sizeof(FRMHED));
#endif
}

void mvfltsb(fPOINT* destination, fPOINT* source, unsigned count) {
#if	 __UseASM__
	_asm {
		std
		mov		ecx, count
		shl		ecx, 1
		mov		esi, source
		add		esi, 4
		mov		edi, destination
		add		edi, 4
		rep		movsd
		cld
	}
#else
	memmove(destination, source, count * sizeof(fPOINT));
#endif
}

void clpspac(fPOINT* pins, unsigned cnt) {
	mvfltsb(&ClipPoints[ClipPointIndex + cnt - 1], &ClipPoints[ClipPointIndex - 1], ClipPointIndex - clpind(pins));
}

void stchadj() {
	unsigned ind, lo, hi;

	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {
		hi = StitchBuffer[ind].attribute&FRMSK;
		lo = hi >> FRMSHFT;
		if (lo > ClosestFormToCursor) {
			StitchBuffer[ind].attribute &= NFRMSK;
			hi += 1 << FRMSHFT;
			StitchBuffer[ind].attribute |= hi;
		}
	}
	refilfn();
	lo = ClosestFormToCursor << FRMSHFT;
	for (ind = PCSHeader.stitchCount; ind != 0; ind--) {
		hi = StitchBuffer[ind - 1].attribute&FRMSK;
		if ((StitchBuffer[ind - 1].attribute&FRMSK) == lo) {
			break;
		}
	}
	ClosestFormToCursor++;
	refilfn();
	rstMap(FRMPSEL);
}

void spltsat(SATCON tsac) {
	fPOINT*		hiflt;
	unsigned	ind, lo, hi, hibak;

	hiflt = (fPOINT*)&OSequence;
	mvfrmsb(&FormList[FormIndex], &FormList[FormIndex - 1], FormIndex - ClosestFormToCursor);
	FormIndex++;
	if (ClosestFormToCursor < (unsigned)FormIndex - 2)
		mvfltsb(&FormVertices[FormVertexIndex + 1], &FormVertices[FormVertexIndex - 1], FormVertexIndex - fltind(FormList[ClosestFormToCursor + 2].vertices));
	FormVertexIndex += 2;
	for (ind = ClosestFormToCursor + 2; ind < FormIndex; ind++)
		FormList[ind].vertices += 2;
	lo = 0;
	hibak = tsac.start + (VertexCount - tsac.finish) + 1;
	hi = hibak + 1;
	for (ind = 0; ind < VertexCount; ind++) {
		if (ind == tsac.start || ind == tsac.finish) {
			hiflt[lo].x = CurrentFormVertices[ind].x;
			hiflt[lo++].y = CurrentFormVertices[ind].y;
			if (ind == tsac.start) {
				hiflt[hi].x = CurrentFormVertices[ind].x;
				hiflt[hi++].y = CurrentFormVertices[ind].y;
			}
			else {
				hiflt[hibak].x = CurrentFormVertices[ind].x;
				hiflt[hibak].y = CurrentFormVertices[ind].y;
			}
		}
		else {
			if (ind < tsac.start) {
				hiflt[lo].x = CurrentFormVertices[ind].x;
				hiflt[lo++].y = CurrentFormVertices[ind].y;
			}
			else {
				if (ind < tsac.finish) {
					hiflt[hi].x = CurrentFormVertices[ind].x;
					hiflt[hi++].y = CurrentFormVertices[ind].y;
				}
				else {
					hiflt[lo].x = CurrentFormVertices[ind].x;
					hiflt[lo++].y = CurrentFormVertices[ind].y;
				}
			}
		}
	}
	for (ind = 0; ind < hi; ind++) {
		CurrentFormVertices[ind].x = hiflt[ind].x;
		CurrentFormVertices[ind].y = hiflt[ind].y;
	}
	SelectedForm->vertexCount = lo;
	FormList[ClosestFormToCursor + 1].vertexCount = hi - lo;
	FormList[ClosestFormToCursor + 1].vertices = &CurrentFormVertices[lo];
	frmout(ClosestFormToCursor);
	frmout(ClosestFormToCursor + 1);
	lo = tsac.start + 1 - tsac.finish;
	for (ind = 0; ind < ActivePointIndex; ind++)
		SelectedForm->satinOrAngle.guide[ind].finish += lo;
	if (SelectedForm->wordParam)
		SelectedForm->wordParam = tsac.start;
	lo = ind + 1;
	while (ind < (unsigned)SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.guide[ind].start -= (tsac.start - 1);
		SelectedForm->satinOrAngle.guide[ind].finish -= (tsac.start - 1);
		ind++;
	}
	if (FormList[ClosestFormToCursor + 1].wordParam)
		FormList[ClosestFormToCursor + 1].wordParam -= (tsac.start - 1);
	mvsatk(&SelectedForm->satinOrAngle.guide[lo - 1], &SelectedForm->satinOrAngle.guide[lo], SatinConnectIndex - sacind(&SelectedForm->satinOrAngle.guide[lo]));
	SatinConnectIndex--;
	FormList[ClosestFormToCursor + 1].satinOrAngle.guide = &SelectedForm->satinOrAngle.guide[ActivePointIndex];
	FormList[ClosestFormToCursor + 1].satinGuideCount = SelectedForm->satinGuideCount - ActivePointIndex - 1;
	SelectedForm->satinGuideCount = ActivePointIndex;
	for (ind = ClosestFormToCursor + 2; ind < FormIndex; ind++)
		FormList[ind].satinOrAngle.guide--;
	if (iseclp(ClosestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
			FormList[ind].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
}

BOOL spltlin() {
	unsigned ind;

	if (ClosestVertexToCursor < 2 || SelectedForm->vertexCount - ClosestVertexToCursor < 2)
		return 0;
	mvfrmsb(&FormList[FormIndex], &FormList[FormIndex - 1], FormIndex - ClosestFormToCursor);
	FormIndex++;
	SelectedForm->vertexCount = ClosestVertexToCursor;
	FormList[ClosestFormToCursor + 1].vertexCount -= ClosestVertexToCursor;
	FormList[ClosestFormToCursor + 1].vertices = &SelectedForm->vertices[ClosestVertexToCursor];
	frmout(ClosestFormToCursor);
	frmout(ClosestFormToCursor + 1);
	if (iseclp(ClosestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
			FormList[ind].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
	return 1;
}

void spltfrm() {
	if (chkMap(FRMPSEL)) {
		savdo();
		fvars(ClosestFormToCursor);
		if (SelectedForm->type == SAT) {
			if (SelectedForm->satinGuideCount) {
				delfstchs();
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				for (ActivePointIndex = 0; ActivePointIndex < SelectedForm->satinGuideCount; ActivePointIndex++) {
					if (SelectedForm->satinOrAngle.guide[ActivePointIndex].start == ClosestVertexToCursor || SelectedForm->satinOrAngle.guide[ActivePointIndex].finish == ClosestVertexToCursor) {
						spltsat(SelectedForm->satinOrAngle.guide[ActivePointIndex]);
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
		SelectedForm = &FormList[FormIndex];
		frmclr(SelectedForm);
		SelectedForm->type = FRMLINE;
		SelectedForm->vertexCount = len;
		SelectedForm->vertices = adflt(len);
		ine = 0;
		for (ind = GroupStartStitch; ind <= GroupEndStitch; ind++) {
			SelectedForm->vertices[ine].x = StitchBuffer[ind].x;
			SelectedForm->vertices[ine++].y = StitchBuffer[ind].y;
		}
		frmout(FormIndex);
		FormIndex++;
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

	if (ClipIntersectSide != find) {
		if ((find - ClipIntersectSide + VertexCount) % VertexCount < (unsigned)(VertexCount >> 1)) {
			ind = nxt(ClipIntersectSide);
			lim = nxt(find);
			while (ind != lim) {
				OSequence[SequenceIndex].x = CurrentFormVertices[ind].x;
				OSequence[SequenceIndex++].y = CurrentFormVertices[ind].y;
				ind = nxt(ind);
			}
		}
		else {
			ind = ClipIntersectSide;
			while (ind != find) {
				OSequence[SequenceIndex].x = CurrentFormVertices[ind].x;
				OSequence[SequenceIndex++].y = CurrentFormVertices[ind].y;
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
		ind = ClipSegments[ActivePointIndex].start;
		if (chkMap(TXFIL) && pntd)
			ind++;
		chksid(ClipSegments[ActivePointIndex].asid);
		while (ind <= ClipSegments[ActivePointIndex].finish) {
			OSequence[SequenceIndex].x = ClipStitchPoints[ind].x;
			OSequence[SequenceIndex++].y = ClipStitchPoints[ind++].y;
		}
		ClipIntersectSide = ClipSegments[ActivePointIndex].zsid;
	}
	else {
		ind = ClipSegments[ActivePointIndex].finish;
		if (chkMap(TXFIL) && pntd)
			ind--;
		chksid(ClipSegments[ActivePointIndex].zsid);
		if (ClipSegments[ActivePointIndex].start) {
			while (ind >= ClipSegments[ActivePointIndex].start) {
				OSequence[SequenceIndex].x = ClipStitchPoints[ind].x;
				OSequence[SequenceIndex++].y = ClipStitchPoints[ind--].y;
			}
		}
		else {
			while (ind < ClipSegments[ActivePointIndex].start) {
				OSequence[SequenceIndex].x = ClipStitchPoints[ind].x;
				OSequence[SequenceIndex++].y = ClipStitchPoints[ind--].y;
			}
		}
		ClipIntersectSide = ClipSegments[ActivePointIndex].asid;
	}
	ClipSegments[ActivePointIndex].dun = 1;
}

unsigned lenref(float* pflt) {
#if	 __UseASM__
	_asm {
		mov		eax, pflt
		sub		eax, ClipSegments
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
	unsigned eax = (unsigned)pflt - (unsigned)ClipSegments;

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
	indexDoubled = ClipSegmentIndex << 1;
	rstMap(FILDIR);
	while (ind < ClipSegmentIndex) {
		if (toglMap(FILDIR)) {
			OutputIndex = (sind + ind) % indexDoubled;
			if (!ClipSegments[lenref(SortedLengths[OutputIndex]) >> 1].dun)
				return 0;
			ind++;
		}
		else {
			OutputIndex = (sind + indexDoubled - ind) % indexDoubled;
			if (!ClipSegments[lenref(SortedLengths[OutputIndex]) >> 1].dun)
				return 0;
		}
	}
	return 1;
}

BOOL nucseg() {
	unsigned	ind;

	if (chkMap(FILDIR))
		ind = ClipSegments[ActivePointIndex].edgeIndex;
	else
		ind = ClipSegments[ActivePointIndex].bind;
	if (clpnxt(ind))
		return 0;
	ind = lenref(SortedLengths[OutputIndex]);
	if (ind & 1)
		rstMap(FILDIR);
	else
		setMap(FILDIR);
	ActivePointIndex = ind >> 1;
	return 1;
}

void mvpclp(unsigned destination, unsigned source) {
#if	 __UseASM__
	_asm {
		mov		edi, destination
		shl		edi, 2
		add		edi, ArrayOfClipIntersectData
		mov		edi, [edi]
		mov		esi, source
		shl		esi, 2
		add		esi, ArrayOfClipIntersectData
		mov		esi, [esi]
		xor ecx, ecx
		mov		cl, 5
		rep		movsd
	}
#else
	//ToDo - Should '20' be sizeof(CLIPSORT)'?
	memcpy(ArrayOfClipIntersectData[destination], ArrayOfClipIntersectData[source], 20);
#endif
}

float getlen(unsigned ind) {
	ClipStitchPoints[ind].side %= VertexCount;
	return	Lengths[ClipStitchPoints[ind].side] +
		hypot(CurrentFormVertices[ClipStitchPoints[ind].side].x - ClipStitchPoints[ind].x,
			CurrentFormVertices[ClipStitchPoints[ind].side].y - ClipStitchPoints[ind].y);
}

unsigned leftsid() {
	unsigned	ind, sid;
	float		minx;

	minx = 1e9;
	sid = 0;
	for (ind = 0; ind < VertexCount; ind++) {
		if (CurrentFormVertices[ind].x < minx) {
			minx = CurrentFormVertices[ind].x;
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

	dif.x = LineSegmentEnd.x - lineSegmentStart.x;
	dif.y = LineSegmentEnd.y - lineSegmentStart.y;
	tpnt.x = lineSegmentStart.x;
	tpnt.y = lineSegmentStart.y;
	flg = 0;
	if (dif.x&&dif.y)
		flg = proj(tpnt, dif.y / dif.x, CurrentFormVertices[find0], CurrentFormVertices[find1], &tipnt);
	else {
		if (dif.y)
			flg = projv(tpnt.x, CurrentFormVertices[find0], CurrentFormVertices[find1], &tipnt);
		else {
			if (dif.x)
				flg = projh(tpnt.y, CurrentFormVertices[find0], CurrentFormVertices[find1], &tipnt);
			else
				if (CurrentFormVertices[find0].y == lineSegmentStart.y&&CurrentFormVertices[find1].y == lineSegmentStart.y)
				{
					if (CurrentFormVertices[find0].x < CurrentFormVertices[find1].x)
					{
						lft = CurrentFormVertices[find0].x;
						rit = CurrentFormVertices[find1].x;
					}
					else
					{
						lft = CurrentFormVertices[find1].x;
						rit = CurrentFormVertices[find0].x;
					}
					if (lineSegmentStart.x > lft&&lineSegmentStart.x < rit)
					{
						ipnt->x = lineSegmentStart.x;
						ipnt->y = lineSegmentStart.y;
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
	*len = hypot(tipnt.x - lineSegmentStart.x, tipnt.y - lineSegmentStart.y);
	// ToDo - Why is this line here?
	//	 hypot(tipnt.x-LineSegmentEnd.x,tipnt.y-LineSegmentEnd.y);
	return flg;
}

unsigned insect() { 
	unsigned	ind, ine, cnt;
	unsigned	svrt, nvrt;
	fRECTANGLE		lrct;
	fPOINT*		ipnt;

	if (LineSegmentEnd.x > lineSegmentStart.x) {
		lrct.left = lineSegmentStart.x;
		lrct.right = LineSegmentEnd.x;
	}
	else {
		lrct.left = LineSegmentEnd.x;
		lrct.right = lineSegmentStart.x;
	}
	if (LineSegmentEnd.y > lineSegmentStart.y) {
		lrct.top = LineSegmentEnd.y;
		lrct.bottom = lineSegmentStart.y;
	}
	else {
		lrct.top = lineSegmentStart.y;
		lrct.bottom = LineSegmentEnd.y;
	}
	ine = cnt = 0;
	for (ind = RegionCrossingStart; ind < RegionCrossingEnd; ind++) {
		svrt = RegionCrossingData[ind].sid;
		nvrt = nxt(svrt);
		if (isect(svrt, nvrt, &ClipIntersectData[ine].point, &ClipIntersectData[ine].sidlen)) {
			ipnt = &ClipIntersectData[ine].point;
			if (ipnt->x >= lrct.left&&
				ipnt->x <= lrct.right&&
				ipnt->y >= lrct.bottom&&
				ipnt->y <= lrct.top) {
				ClipIntersectData[ine].seglen = hypot(ClipIntersectData[ine].point.x - lineSegmentStart.x, ClipIntersectData[ine].point.y - lineSegmentStart.y);
				ClipIntersectData[ine].lin = svrt;
				ArrayOfClipIntersectData[ine] = &ClipIntersectData[ine];
				ine++;
				cnt++;
			}
		}
	}
	if (cnt > 1) {
		qsort((void*)ArrayOfClipIntersectData, cnt, 4, lencmp);
		ine = 1;
		for (ind = 0; ind < cnt - 1; ind++) {
			if (fabs(ArrayOfClipIntersectData[ind]->seglen - ArrayOfClipIntersectData[ind + 1]->seglen) > TINY)
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

	if (pntx < BoundingRect.left)
		return 0;
	if (pntx > BoundingRect.right)
		return 0;
	if (pnty < BoundingRect.bottom)
		return 0;
	if (pnty > BoundingRect.top)
		return 0;
	acnt = 0;
	for (ind = RegionCrossingStart; ind < RegionCrossingEnd; ind++)
	{
		svrt = RegionCrossingData[ind].sid;
		nvrt = nxt(svrt);
		if (projv(pntx, CurrentFormVertices[svrt], CurrentFormVertices[nvrt], &ipnt))
		{
			if (ipnt.y > pnty)
			{
				if (CurrentFormVertices[svrt].x != pntx&&CurrentFormVertices[nvrt].x != pntx)
					acnt++;
				else
				{
					if (CurrentFormVertices[svrt].x < CurrentFormVertices[nvrt].x)
					{
						if (CurrentFormVertices[nvrt].x != pntx)
							acnt++;
					}
					else
					{
						if (CurrentFormVertices[svrt].x != pntx)
							acnt++;
					}
				}
			}
		}
	}
	return acnt & 1;
}

unsigned clpnseg(unsigned strt, unsigned fin) {
	ClipSegments[ClipSegmentIndex].start = strt;
	ClipSegments[ClipSegmentIndex].borderLength = getlen(strt);
	ClipSegments[ClipSegmentIndex].asid = ClipStitchPoints[strt].side;
	ClipSegments[ClipSegmentIndex].edgeLength = getlen(fin);
	ClipSegments[ClipSegmentIndex].zsid = ClipStitchPoints[fin].side;
	ClipSegments[ClipSegmentIndex].finish = fin;
	ClipSegments[ClipSegmentIndex++].dun = 0;
	return fin + 1;
}

unsigned vclpfor(unsigned ind) {
	while (!ClipStitchPoints[ind].flag&&ind < ActivePointIndex)
		ind++;
	return ind;
}

unsigned vclpbak(unsigned ind) {
	while (!ClipStitchPoints[ind].flag&&ind)
		ind--;
	return ind;
}

BOOL vscmp(unsigned index1, unsigned index2) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		esi, index1
		mov		edi, index2
		shl		esi, 3
		shl		edi, 3
		mov		ecx, offset OSequence
		add		esi, ecx
		add		edi, ecx
		mov		ecx, [esi]
		cmp		ecx, [edi]
		je		short vscmp1
		inc		eax
		jmp		short vscmpx
vscmp1: add		esi, 4
		add		edi, 4
		mov		ecx, [esi]
		cmp		ecx, [edi]
		je		short vscmpx
		inc		eax
vscmpx :
	}
#else
	if (OSequence[index1].x != OSequence[index2].x)
		return 1;

	return OSequence[index1].y != OSequence[index2].y ? 1 : 0;
#endif
}

void duflt() {
	unsigned	ind;
	float		leftEdge;

	leftEdge = 1e9;
	for (ind = 0; ind < VertexCount; ind++) {
		if (CurrentFormVertices[ind].x < leftEdge)
			leftEdge = CurrentFormVertices[ind].x;
	}
	if (leftEdge < ClipRectSize.cx) {
		setMap(WASNEG);
		FormOffset = ClipRectSize.cx + fabs(leftEdge) + .05;
		for (ind = 0; ind < VertexCount; ind++)
			CurrentFormVertices[ind].x += FormOffset;
		SelectedForm->rectangle.left += FormOffset;
		SelectedForm->rectangle.right += FormOffset;
	}
	else
		rstMap(WASNEG);
}

void inspnt()
{
	ClipStitchPoints[ActivePointIndex + 1].x = ClipStitchPoints[ActivePointIndex].x;
	ClipStitchPoints[ActivePointIndex + 1].y = ClipStitchPoints[ActivePointIndex].y;
	ClipStitchPoints[ActivePointIndex].x = midl(ClipStitchPoints[ActivePointIndex + 1].x, ClipStitchPoints[ActivePointIndex - 1].x);
	ClipStitchPoints[ActivePointIndex].y = midl(ClipStitchPoints[ActivePointIndex + 1].y, ClipStitchPoints[ActivePointIndex - 1].y);
	ClipStitchPoints[ActivePointIndex].flag = 1;
	ActivePointIndex++;
}

void clpcon() {
	RECT		nrct;
	unsigned	iSegment, iStitchPoint, ind, ine, inf, ing, nof, clpneg;
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
	ClipWidth = ClipRectSize.cx + SelectedForm->fillSpacing;
	if (chkMap(ISUND))
		ClipWidth = SelectedForm->underlaySpacing;
	if (SelectedForm->fillSpacing < 0)
		clpneg = 1;
	else
		clpneg = 0;
	if (ClipWidth < CLPMINAUT)
		ClipWidth = (float)CLPMINAUT;
	if (chkMap(TXFIL))
	{
		if (TextureIndex&&SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count <= TextureIndex)
			ClipWidth = SelectedForm->fillSpacing;
		else
			return;
	}
	Lengths = new double[VertexCount + 1];
	ClipSideLengths = new double[VertexCount];
	ClipIntersectData = new CLIPSORT[VertexCount];
	ArrayOfClipIntersectData = new CLIPSORT*[VertexCount + 1]();
	ine = leftsid();
	tlen = 0;
	Lengths[ine] = 0;
	ine = nxt(ine);
	for (ind = 0; ind <= VertexCount; ind++) {
		inf = nxt(ine);
		Lengths[ine] = tlen;
		ClipSideLengths[ine] = hypot(CurrentFormVertices[inf].x - CurrentFormVertices[ine].x, CurrentFormVertices[inf].y - CurrentFormVertices[ine].y);
		tlen += ClipSideLengths[ine];
		ine = inf;
	}
	ClipSegments = (CLPSEG*)&StitchBuffer[MAXSEQ];
	nrct.left = floor(SelectedForm->rectangle.left / ClipWidth);
	nrct.right = ceil(SelectedForm->rectangle.right / ClipWidth);
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
			nrct.left -= (float)ClipRectSize.cx / ClipWidth;
			nrct.right += (float)ClipRectSize.cx / ClipWidth;
		}
	}
	if (clpneg && !clpnof)
		nrct.left -= (float)ClipRectSize.cx / ClipWidth;
	if (nrct.bottom < 0) {
		nof = 1 - nrct.bottom;
		nrct.bottom += nof;
		nrct.top += nof;
		fnof = ClipRectSize.cy*nof;
		for (ind = 0; ind < VertexCount; ind++)
			CurrentFormVertices[ind].y += fnof;
	}
	ClipStitchPoints = (CLIPNT*)&BSequence;
	segxs = 0;
	for (ind = 0; ind < VertexCount; ind++) {
		strt = floor(CurrentFormVertices[ind].x / ClipWidth);
		fin = floor((CurrentFormVertices[nxt(ind)].x) / ClipWidth);
		if (strt > fin) {
			ine = strt;
			strt = fin;
			fin = ine;
		}
		if (SelectedForm->fillSpacing < 0)
			fin += ClipRectSize.cx / ClipWidth;
		if (fin > (unsigned)nrct.right)
			fin = nrct.right;
		if (clpneg)
			strt -= (float)ClipRectSize.cx / ClipWidth;
		for (ine = strt; ine <= fin; ine++) {
			RegionCrossingData[segxs].sid = ind;
			RegionCrossingData[segxs++].seg = ine;
		}
	}
	qsort((void*)RegionCrossingData, segxs, 8, clpcmp);
	iclpx = (unsigned*)&RegionCrossingData[segxs];
	ine = 1; inf = RegionCrossingData[0].seg;
	iclpx[0] = 0;
	for (ind = 1; ind < segxs; ind++) {
		if (RegionCrossingData[ind].seg != inf) {
			iclpx[ine++] = ind;
			inf = RegionCrossingData[ind].seg;
		}
	}
	iclpx[ine] = ind;
	BoundingRect.left = BoundingRect.right = CurrentFormVertices[0].x;
	BoundingRect.top = BoundingRect.bottom = CurrentFormVertices[0].y;
	for (ind = 1; ind < VertexCount; ind++)
	{
		if (CurrentFormVertices[ind].x > BoundingRect.right)
			BoundingRect.right = CurrentFormVertices[ind].x;
		if (CurrentFormVertices[ind].x < BoundingRect.left)
			BoundingRect.left = CurrentFormVertices[ind].x;
		if (CurrentFormVertices[ind].y > BoundingRect.top)
			BoundingRect.top = CurrentFormVertices[ind].y;
		if (CurrentFormVertices[ind].y < BoundingRect.bottom)
			BoundingRect.bottom = CurrentFormVertices[ind].y;
	}
	segps = ine;
	ind = RegionCrossingStart = cnt = 0;
	seg = RegionCrossingData[0].seg;
	clrnum = (nrct.top >> 5) + 1;
	// ToDo -When copy-pasting multiple forms, ActivePoint Index is not being updated correctly
	ActivePointIndex = 0;
	for (ind = 0; ind < segps; ind++) {
		RegionCrossingStart = iclpx[ind];
		RegionCrossingEnd = iclpx[ind + 1];
		ploc.x = ClipWidth*(ind + nrct.left);
		clpvof = 0;
		if (chkMap(TXFIL))
		{
			tine = (ind + nrct.left) % SelectedForm->fillInfo.texture.lines;
			ClipStitchCount = TextureSegments[tine].stitchCount;
			ptx = &TexturePointsBuffer[SelectedForm->fillInfo.texture.index + TextureSegments[tine].line];
			lineSegmentStart.x = ploc.x;
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
			lineSegmentStart.x = ploc.x + ClipBuffer[0].x;
		}
		lineSegmentStart.y = nrct.bottom*ClipRectSize.cy;
		if (clpnof)
			clpvof = (float)(ind%clpnof) / clpnof*ClipRectSize.cy;
		for (tine = nrct.bottom; tine < nrct.top; tine++) {
			ploc.y = tine*ClipRectSize.cy - clpvof;
			LineSegmentEnd.x = ploc.x + ClipBuffer[0].x;
			LineSegmentEnd.y = ploc.y + ClipBuffer[0].y;
			if (!ActivePointIndex) {
				lineSegmentStart.x = LineSegmentEnd.x;
				lineSegmentStart.y = LineSegmentEnd.y;
			}
			for (inf = 0; inf < ClipStitchCount; inf++) {
				if (chkMap(TXFIL))
				{
					if (ptx != nullptr) {
						LineSegmentEnd.x = ploc.x;
						LineSegmentEnd.y = ploc.y + ptx[inf].y;
					}
				}
				else
				{
					LineSegmentEnd.x = ploc.x + ClipBuffer[inf].x;
					LineSegmentEnd.y = ploc.y + ClipBuffer[inf].y;
				}

				ClipStitchPoints[ActivePointIndex].x = lineSegmentStart.x;
				ClipStitchPoints[ActivePointIndex].y = lineSegmentStart.y;
				if (isin(lineSegmentStart.x, lineSegmentStart.y))
				{
					if (ActivePointIndex&&ClipStitchPoints[ActivePointIndex - 1].flag == 2)
						inspnt();
					ClipStitchPoints[ActivePointIndex].flag = 0;
				}
				else
				{
					if (ActivePointIndex && !ClipStitchPoints[ActivePointIndex - 1].flag)
						inspnt();
					ClipStitchPoints[ActivePointIndex].flag = 2;
				}
				ActivePointIndex++;
				cnt = insect();
				if (cnt)
				{
					for (ing = 0; ing < cnt; ing++) {
						if (ArrayOfClipIntersectData != nullptr) {
							ClipStitchPoints[ActivePointIndex].side = ArrayOfClipIntersectData[ing]->lin;
							ClipStitchPoints[ActivePointIndex].x = ArrayOfClipIntersectData[ing]->point.x;
							ClipStitchPoints[ActivePointIndex].y = ArrayOfClipIntersectData[ing]->point.y;
							ClipStitchPoints[ActivePointIndex].flag = 1;
							ActivePointIndex++;
							if (ActivePointIndex > MAXSEQ << 2)
								goto clpskp;
						}
					}
				}
				lineSegmentStart.x = LineSegmentEnd.x;
				lineSegmentStart.y = LineSegmentEnd.y;
			}
		}
		ClipStitchPoints[ActivePointIndex - 1].flag = 2;
	}
clpskp:;

	ClipStitchPoints[ActivePointIndex].flag = 2;
	if (nof) {
		fnof = nof*ClipRectSize.cy;
		for (iStitchPoint = 0; iStitchPoint < ActivePointIndex; iStitchPoint++)
			ClipStitchPoints[iStitchPoint].y -= fnof;
		for (ind = 0; ind < VertexCount; ind++)
			CurrentFormVertices[ind].y -= fnof;
	}
#define CLPVU 0


#if CLPVU==1

	goto clp1skp;

#endif

	ClipSegmentIndex = 0;
	rstMap(FILDIR);
	ine = 0;
	if (ActivePointIndex)
	{
		for (ind = 0; ind < ActivePointIndex - 1; ind++) {
			switch (ClipStitchPoints[ind].flag)
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

			delete[] Lengths;
			delete[] ClipSideLengths;
			delete[] ClipIntersectData;
			delete[] ArrayOfClipIntersectData;

			if (ClipSegmentIndex) {
				clplim = ClipSegmentIndex >> 3;
				clplim = ClipSegmentIndex >> 1;
				if (!clplim)
					clplim = 1;
				if (clplim > 12)
					clplim = 12;
				SortedLengths = (float**)&ClipSegments[ClipSegmentIndex];
				ine = 0;
				for (iSegment = 0; iSegment < ClipSegmentIndex; iSegment++) {
					SortedLengths[ine++] = &ClipSegments[iSegment].borderLength;
					SortedLengths[ine++] = &ClipSegments[iSegment].edgeLength;
				}
				qsort((void*)SortedLengths, ine, 4, lencmp);
				ind = sizeof(CLPSEG);
				for (ind = 0; ind < ine; ind++) {
					inf = lenref(SortedLengths[ind]);
					ing = inf >> 1;
					if (inf & 1)
						ClipSegments[ing].edgeIndex = ind;
					else
						ClipSegments[ing].bind = ind;
				}

#if CLPVU==1

				for (unsigned iStitch = 0; iStitch < ActivePointIndex; iStitch++) {
					StitchBuffer[iStitch].x = ClipStitchPoints[iStitch].x;
					StitchBuffer[iStitch].y = ClipStitchPoints[iStitch].y;
					StitchBuffer[iStitch].attribute = 0;
				}
				PCSHeader.stitchCount = ActivePointIndex;
#endif

#if CLPVU==2

				inf = 0;
				for (iSegment = 0; iSegment < ClipSegmentIndex; iSegment++) {
					for (iStitchPoint = ClipSegments[iSegment].start; iStitchPoint <= ClipSegments[iSegment].finish; iStitchPoint++) {
						StitchBuffer[inf].x = ClipStitchPoints[iStitchPoint].x;
						StitchBuffer[inf].y = ClipStitchPoints[iStitchPoint].y;
						StitchBuffer[inf++].attribute = iSegment & 0xf;
					}
				}
				PCSHeader.stitchCount = inf;

#endif

				minx = 1e99;

#if CLPVU==0

				ActivePointIndex = 0;
				setMap(FILDIR);
				SequenceIndex = 0;
				ClipIntersectSide = ClipSegments[0].asid;
				ritseg();
				while (nucseg()) {
					if (SequenceIndex > MAXSEQ - 3)
						break;
					ritseg();
				}
				chksid(0);
				if (SequenceIndex > MAXSEQ - 100)
					SequenceIndex = MAXSEQ - 100;
				ine = 0; inf = 0;
				for (ind = 0; ind < SequenceIndex; ind++) {
					if (vscmp(ind, ine)) {
						ine++;
						OSequence[ine].x = OSequence[ind].x;
						OSequence[ine].y = OSequence[ind].y;
					}
					else
						inf++;
				}
				SequenceIndex = ine;
				if (chkMap(WASNEG)) {
					for (ind = 0; ind < SequenceIndex; ind++)
						OSequence[ind].x -= FormOffset;
					for (ind = 0; ind < VertexCount; ind++)
						CurrentFormVertices[ind].x -= FormOffset;
					SelectedForm->rectangle.left -= FormOffset;
					SelectedForm->rectangle.right -= FormOffset;
				}
#endif
			}
}

void vrtsclp() {
	unsigned ind;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
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
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (SelectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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
						fvars(ClosestFormToCursor);
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

	if (AngledForm.vertexCount) {
		trct = &AngledForm.rectangle;
		CurrentFormVertices = AngledForm.vertices;
		trct->left = trct->right = CurrentFormVertices[0].x;
		trct->bottom = trct->top = CurrentFormVertices[0].y;
		for (ine = 1; ine < AngledForm.vertexCount; ine++) {
			if (CurrentFormVertices[ine].x > trct->right)
				trct->right = CurrentFormVertices[ine].x;
			if (CurrentFormVertices[ine].x < trct->left)
				trct->left = CurrentFormVertices[ine].x;
			if (CurrentFormVertices[ine].y < trct->bottom)
				trct->bottom = CurrentFormVertices[ine].y;
			if (CurrentFormVertices[ine].y > trct->top)
				trct->top = CurrentFormVertices[ine].y;
		}
	}
}

void horclpfn() {
	unsigned ind;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	RotationAngle = PI / 2;
	for (ind = 0; ind < AngledForm.vertexCount; ind++) {
		AngledForm.vertices[ind].x = SelectedForm->vertices[ind].x;
		AngledForm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&AngledForm.vertices[ind]);
	}
	angout();
	SelectedForm = &AngledForm;
	CurrentFormVertices = AngledForm.vertices;
	clpcon();
	RotationAngle = -PI / 2;
	rotbak();
	fvars(ClosestFormToCursor);
}

void horsclp() {
	unsigned ind;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
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
	CurrentFormVertices = SelectedForm->vertices;
	refilfn();
}

void horclp() {
	unsigned ind;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (SelectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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
						fvars(ClosestFormToCursor);
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

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	if (chkMap(ISUND))
	{
		RotationAngle = PI / 2 - SelectedForm->underlayStitchAngle;
		tflt = insid();
		for (ind = 0; ind < AngledForm.vertexCount; ind++)
		{
			AngledFormVertices[ind].x = tflt[ind].x;
			AngledFormVertices[ind].y = tflt[ind].y;
			rotflt(&AngledFormVertices[ind]);
		}
	}
	else
	{
		if (chkMap(TXFIL))
			RotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
		else
			RotationAngle = PI / 2 - SelectedForm->satinOrAngle.angle;
		for (ind = 0; ind < AngledForm.vertexCount; ind++)
		{
			AngledFormVertices[ind].x = SelectedForm->vertices[ind].x;
			AngledFormVertices[ind].y = SelectedForm->vertices[ind].y;
			rotflt(&AngledFormVertices[ind]);
		}
	}
	angout();
	SelectedForm = &AngledForm;
	CurrentFormVertices = AngledFormVertices;
	clpcon();
	RotationAngle = -RotationAngle;
	rotbak();
	fvars(ClosestFormToCursor);
}

void angsclp() {
	unsigned ind;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
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
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cy > CLPMIN) {
				if (SelectedFormCount) {
					setMap(NOCLP);
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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
						SelectedForm = &FormList[ClosestFormToCursor];
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
	ine = SequenceIndex;
	for (ind = SequenceIndex; ind != 0; ind--) {
		OSequence[ine].x = OSequence[ind - 1].x;
		OSequence[ine++].y = OSequence[ind - 1].y;
	}
	SequenceIndex = ine;
}

void dubsfil() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEDOUBLE;
	SequenceIndex = 0;
	SelectedForm->borderColor = ActiveColor;
	dubfn();
	bsizpar();
	refilfn();
}

void dubfil() {
	unsigned ind;

	if (filmsgs(FML_LIN))
		return;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
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
	fp16 = FormIndex << 4;
	if (FormIndex) {
		fcnts = (unsigned*)&OSequence;
		bcnts = (unsigned*)&BSequence;
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
		for (ind = 0; ind < FormIndex; ind++) {
			if (FormList[ind].fillType) {
				cnt = maxcol = 0;
				for (ine = strt; ine < fin; ine++) {
					if (fcnts[ine] > cnt) {
						cnt = fcnts[ine];
						maxcol = ine;
					}
				}
				maxcol &= 0xf;
				if (FormList[ind].fillColor != maxcol) {
					chngcnt++;
					FormList[ind].fillColor = maxcol;
				}
				if (FormList[ind].fillType == FTHF&&FormList[ind].extendedAttribute&AT_FTHBLND) {
					cnt = maxcol = 0;
					for (ine = strt; ine < fin; ine++) {
						if (fthcnts[ine] > cnt) {
							cnt = fcnts[ine];
							maxcol = ine;
						}
					}
					maxcol &= 0xf;
					if (FormList[ind].fillInfo.feather.color != maxcol) {
						chngcnt++;
						FormList[ind].fillInfo.feather.color = maxcol;
					}
				}
			}
			if (FormList[ind].edgeType) {
				cnt = maxcol = 0;
				for (ine = strt; ine < fin; ine++) {
					if (bcnts[ine] > cnt) {
						cnt = bcnts[ine];
						maxcol = ine;
					}
				}
				maxcol &= 0xf;
				if (FormList[ind].borderColor != maxcol) {
					chngcnt++;
					FormList[ind].borderColor = maxcol;
				}
			}
			if (FormList[ind].extendedAttribute&(AT_WALK | AT_CWLK | AT_UND)) {
				cnt = maxcol = 0;
				for (ine = strt; ine < fin; ine++) {
					if (ucnts[ine] > cnt) {
						cnt = bcnts[ine];
						maxcol = ine;
					}
				}
				maxcol &= 0xf;
				if (FormList[ind].underlayColor != maxcol) {
					chngcnt++;
					FormList[ind].underlayColor = maxcol;
				}
			}
			strt += 16;
			fin += 16;
		}
	}
	LoadString(ThrEdInstance, IDS_NCOLCHG, buf, HBUFSIZ);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), buf, chngcnt);
	shoMsg(MsgBuffer);
}

BOOL fxpnt() {
	double		len, dif;
	unsigned	ind;

	MoveToCoords.x = CurrentFormVertices[NextStart].x;
	MoveToCoords.y = CurrentFormVertices[NextStart].y;
	len = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
	if (len > AdjustedSpace) {
		for (ind = 0; ind < 10; ind++) {
			len = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
			dif = AdjustedSpace - len;
			MoveToCoords.x += dif*ListCOSINEs[CurrentSide];
			MoveToCoords.y += dif*ListSINEs[CurrentSide];
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
		SelectedPoint.x = MoveToCoords.x;
		SelectedPoint.y = MoveToCoords.y;
		BeanCount++;
		len = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		cnt = floor(len / AdjustedSpace);
		dif.x = AdjustedSpace*ListCOSINEs[CurrentSide];
		dif.y = AdjustedSpace*ListSINEs[CurrentSide];
		SelectedPoint.x += dif.x*cnt;
		SelectedPoint.y += dif.y*cnt;
		BeanCount += cnt;
	}
}

void fxlin() {
	double		len;
	unsigned	cnt;
	unsigned	ind;
	dPOINT		dif;

	if (fxpnt()) {
		ChainEndPoints[ActivePointIndex].x = SelectedPoint.x = MoveToCoords.x;
		ChainEndPoints[ActivePointIndex].y = SelectedPoint.y = MoveToCoords.y;
		ActivePointIndex++;
		len = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		cnt = floor(len / AdjustedSpace);
		dif.x = AdjustedSpace*ListCOSINEs[CurrentSide];
		dif.y = AdjustedSpace*ListSINEs[CurrentSide];
		for (ind = 0; ind < cnt; ind++) {
			SelectedPoint.x += dif.x;
			SelectedPoint.y += dif.y;
			ChainEndPoints[ActivePointIndex].x = SelectedPoint.x;
			ChainEndPoints[ActivePointIndex].y = SelectedPoint.y;
			ActivePointIndex++;
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

	ChainEndPoints = (fPOINT*)BSequence;
	AdjustedSpace = 0;
	for (ind = 1; ind < (unsigned)VertexCount; ind++) {
		len = hypot(CurrentFormVertices[ind].x - CurrentFormVertices[0].x, CurrentFormVertices[ind].y - CurrentFormVertices[0].y);
		if (len > SelectedForm->edgeSpacing)
			goto fxlab;
		else {
			if (len > AdjustedSpace) {
				AdjustedSpace = len;
				mind = ind;
			}
		}
	}
	ChainEndPoints[0].x = CurrentFormVertices[0].x;
	ChainEndPoints[0].y = CurrentFormVertices[0].y;
	ChainEndPoints[1].x = CurrentFormVertices[1].x;
	ChainEndPoints[1].y = CurrentFormVertices[1].y;
	ActivePointIndex = 2;
	return;
fxlab:;
	AdjustedSpace = minspac = SelectedForm->edgeSpacing;
	spac2 = AdjustedSpace / 2;
	ter = minter = 1e9;
	lupcnt = inicnt = 0;
	loval = 0;
	hival = 1;
	while (lupcnt<100 && (hival - loval)>TINY) {
		BeanCount = 0;
		SelectedPoint.x = CurrentFormVertices[0].x;
		SelectedPoint.y = CurrentFormVertices[0].y;
		ActivePointIndex = 1;
		ChainEndPoints[0].x = SelectedPoint.x;
		ChainEndPoints[0].y = SelectedPoint.y;
		for (CurrentSide = 0; CurrentSide < (unsigned)VertexCount - 1; CurrentSide++) {
			NextStart = CurrentSide + 1;
			fxlit();
		}
		if (SelectedForm->type != FRMLINE) {
			NextStart = 0;
			fxlit();
		}
		else
			NextStart = VertexCount - 1;
		if (!inicnt) {
			inicnt = BeanCount;
			loval = AdjustedSpace;
			minter = ter = (double)hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
			minspac = AdjustedSpace;
			ter /= inicnt;
			AdjustedSpace += ter / 2;
			hival = loval + ter;
		}
		else {
			ter = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
			if (ter > spac2)
				ter = SelectedForm->edgeSpacing - ter;
			if (ter < minter) {
				minter = ter;
				minspac = AdjustedSpace;
			}
			if (inicnt == BeanCount)
				loval = AdjustedSpace;
			else {
				if (BeanCount > inicnt)
					hival = AdjustedSpace;
				else
					loval = AdjustedSpace;
			}
			AdjustedSpace = loval + (hival - loval) / 2;
		}
		lupcnt++;
	}
	SelectedPoint.x = CurrentFormVertices[0].x;
	SelectedPoint.y = CurrentFormVertices[0].y;
	OutputIndex = 1;
	AdjustedSpace = minspac;
	for (CurrentSide = 0; CurrentSide < (unsigned)VertexCount - 1; CurrentSide++) {
		NextStart = CurrentSide + 1;
		fxlin();
	}
	if (SelectedForm->type != FRMLINE) {
		NextStart = 0;
		fxlin();
	}
	ter = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
	if (ter < spac2)
		ActivePointIndex--;
	ChainEndPoints[ActivePointIndex].x = CurrentFormVertices[NextStart].x;
	ChainEndPoints[ActivePointIndex].y = CurrentFormVertices[NextStart].y;
	ActivePointIndex++;
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

	dif.x = ChainEndPoints[fin].x - ChainEndPoints[strt].x;
	dif.y = ChainEndPoints[fin].y - ChainEndPoints[strt].y;
	difrat.x = dif.x*SelectedForm->edgeStitchLen;
	difrat.y = dif.y*SelectedForm->edgeStitchLen;
	pnts[0].x = ChainEndPoints[strt].x;
	pnts[0].y = ChainEndPoints[strt].y;
	pnts[4].x = ChainEndPoints[fin].x;
	pnts[4].y = ChainEndPoints[fin].y;
	tang = atan2(dif.y, dif.x) + PI / 2;
	off.x = cos(tang)*SelectedForm->borderSize;
	off.y = sin(tang)*SelectedForm->borderSize;
	midx = ChainEndPoints[strt].x + difrat.x;
	midy = ChainEndPoints[strt].y + difrat.y;
	pnts[1].x = midx + off.x;
	pnts[1].y = midy + off.y;
	pnts[3].x = midx - off.x;
	pnts[3].y = midy - off.y;
	dif.x = ChainEndPoints[fin + 1].x - ChainEndPoints[fin].x;
	dif.y = ChainEndPoints[fin + 1].y - ChainEndPoints[fin].y;
	pnts[2].x = ChainEndPoints[fin].x + dif.x / 4;
	pnts[2].y = ChainEndPoints[fin].y + dif.y / 4;
	for (ine = 0; ine < ChainCount; ine++) {
		OSequence[SequenceIndex].x = pnts[ChainSequence[ine]].x;
		OSequence[SequenceIndex].y = pnts[ChainSequence[ine]].y;
		SequenceIndex++;
	}
}

void duch() {
	unsigned ind, bak;

	Div4 = AdjustedSpace / 4;
	SequenceIndex = 0;
	if (ActivePointIndex > 1) {
		for (ind = 0; ind < (unsigned)ActivePointIndex - 2; ind++)
			duchfn(ind, ind + 1);
		if (SelectedForm->type == FRMLINE) {
			duchfn(ind, ind + 1);
			bak = 8;
			if (chkMap(LINCHN))
				bak--;
			if ((SequenceIndex >= bak)) {
				OSequence[SequenceIndex - bak].x = ChainEndPoints[ind + 1].x;
				OSequence[SequenceIndex - bak].y = ChainEndPoints[ind + 1].y;
			}
			OSequence[SequenceIndex].x = ChainEndPoints[ind + 1].x;
			OSequence[SequenceIndex++].y = ChainEndPoints[ind + 1].y;
		}
		else {
			duchfn(ind, 0);
			OSequence[SequenceIndex].x = ChainEndPoints[ActivePointIndex - 1].x;
			OSequence[SequenceIndex].y = ChainEndPoints[ActivePointIndex - 1].y;
			SequenceIndex++;
		}
	}
	else
		tabmsg(IDS_CHANSMAL);
}

void dufxlen() {
	unsigned ind;

	duangs();
	// ToDo - Use locations that are not fPOINT for the list of doubles
	ListSINEs = (double*)AngledFormVertices;
	ListCOSINEs = (double*)TempPolygon;
	for (ind = 0; ind < (unsigned)VertexCount; ind++) {
		ListSINEs[ind] = sin(FormAngles[ind]);
		ListCOSINEs[ind] = cos(FormAngles[ind]);
	}
	ListSINEs[ind] = sin((FormAngles[0] > FormAngles[ind]) ? (FormAngles[0] - FormAngles[ind]) : (FormAngles[ind] - FormAngles[0]));
	fxlen();
}

void chnfn() {
	ChainCount = 10;
	if (chkMap(LINCHN))
		ChainCount--;
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	dufxlen();
	dulast();
	SequenceIndex = 0;
	duch();
}

void chan() {
	SelectedForm->borderColor = ActiveColor;
	SelectedForm->edgeSpacing = IniFile.chainSpace;
	SelectedForm->borderSize = BorderWidth;
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
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
			chan();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {
		if (chkMap(FORMSEL)) {
			fvars(ClosestFormToCursor);
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
	for (ind = 0; ind < VertexCount; ind++)
	{
		nvrt = nxt(ind);
		if (projv(pntx, CurrentFormVertices[ind], CurrentFormVertices[nvrt], &ipnt))
		{
			if (ipnt.y >= pnty)
			{
				if (CurrentFormVertices[ind].x != pntx&&CurrentFormVertices[nvrt].x != pntx)
					acnt++;
				else
				{
					if (CurrentFormVertices[ind].x < CurrentFormVertices[nvrt].x)
					{
						if (CurrentFormVertices[nvrt].x != pntx)
							acnt++;
					}
					else
					{
						if (CurrentFormVertices[ind].x != pntx)
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
		fvars(ClosestFormToCursor);
		ine = 0;
		RegionCrossingStart = 0;
		RegionCrossingEnd = VertexCount;
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
	dif.x = ChainEndPoints[fin].x - ChainEndPoints[strt].x;
	dif.y = ChainEndPoints[fin].y - ChainEndPoints[strt].y;
	len = hypot(dif.x, dif.y);
	rat = len / ClipRectSize.cx;
	RotationAngle = atan2(dif.y, dif.x);
	for (ind = 0; ind < ClipStitchCount; ind++) {
		tflt[ind].x = TempClipPoints[ind].x*rat;
		tflt[ind].y = TempClipPoints[ind].y;
		rotflt(&tflt[ind]);
		OSequence[SequenceIndex].x = ChainEndPoints[strt].x + tflt[ind].x;
		OSequence[SequenceIndex++].y = ChainEndPoints[strt].y + tflt[ind].y;
	}
}

void duxclp() {
	unsigned ind;

	duangs();
	dufxlen();
	clpxadj();
	SequenceIndex = 0;
	RotationCenter.x = RotationCenter.y = 0;
	for (ind = 1; ind < ActivePointIndex; ind++)
		xclpfn(ind - 1, ind);
	if (SelectedForm->type != FRMLINE) {
		OSequence[SequenceIndex].x = ChainEndPoints[0].x;
		OSequence[SequenceIndex++].y = ChainEndPoints[0].y;
	}
}

void dulast() {
	unsigned	ind, ine, mind;
	double		minlen;
	double		len;

	TempClipPoints = (fPOINT*)&ChainEndPoints[ActivePointIndex];
	if (lastch()) {
		minlen = 1e99;
		mind = 0;
		for (ind = 0; ind < ActivePointIndex; ind++) {
			len = hypot(LastPoint.x - ChainEndPoints[ind].x, LastPoint.y - ChainEndPoints[ind].y);
			if (len < minlen) {
				minlen = len;
				mind = ind;
			}
		}
		if (mind) {
			ine = 0;
			for (ind = mind; ind < ActivePointIndex - 1; ind++) {
				TempClipPoints[ine].x = ChainEndPoints[ind].x;
				TempClipPoints[ine++].y = ChainEndPoints[ind].y;
			}
			for (ind = 0; ind <= mind; ind++) {
				TempClipPoints[ine].x = ChainEndPoints[ind].x;
				TempClipPoints[ine++].y = ChainEndPoints[ind].y;
			}
			MoveMemory(ChainEndPoints, TempClipPoints, sizeof(fPOINT)*ine);
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
			TempClipPoints[ind].x = ClipBuffer[ind].x;
			TempClipPoints[ind].y = -ClipBuffer[ind].y + pivot;
		}
	}
	else {
		for (ind = 0; ind < ClipStitchCount; ind++) {
			TempClipPoints[ind].x = ClipBuffer[ind].x;
			TempClipPoints[ind].y = -ClipBuffer[ind].y;
		}
	}
}

void fsclpx() {
	unsigned	ind;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGECLIPX;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
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
	if (OpenClipboard(ThrEdWindow)) {
		fvars(ClosestFormToCursor);
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			savdo();
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (SelectedFormCount) {
					for (ind = 0; ind < SelectedFormCount; ind++) {
						ClosestFormToCursor = SelectedFormList[ind];
						fvars(ClosestFormToCursor);
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
	if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, (DLGPROC)wavprc)) {
		ind = FormVertexIndex;
		end = IniFile.waveEnd + 1;
		durpoli(IniFile.wavePoints);
		mdufrm();
		FormVertexIndex = ind;
		tflt = (fPOINT*)&BSequence;
		ine = 0;
		ind = IniFile.waveStart;
		while (ind != IniFile.waveEnd) {
			inf = (ind + 1) % IniFile.wavePoints;
			tflt[ine].x = -CurrentFormVertices[inf].x + CurrentFormVertices[ind].x;
			tflt[ine].y = -CurrentFormVertices[inf].y + CurrentFormVertices[ind].y;
			ine++;
			ind = inf;
		}
		cnt = ine;
		ine = 0;
		pos.x = pos.y = 0;
		for (inf = 0; inf < IniFile.waveLobes; inf++) {
			if (inf & 1) {
				for (ind = 0; ind < cnt; ind++) {
					CurrentFormVertices[ine].x = pos.x;
					CurrentFormVertices[ine].y = pos.y;
					ine++;
					pos.x += tflt[ind].x;
					pos.y += tflt[ind].y;
				}
			}
			else {
				for (ind = cnt; ind != 0; ind--) {
					CurrentFormVertices[ine].x = pos.x;
					CurrentFormVertices[ine].y = pos.y;
					ine++;
					pos.x += tflt[ind - 1].x;
					pos.y += tflt[ind - 1].y;
				}
			}
		}
		CurrentFormVertices[ine].x = pos.x;
		CurrentFormVertices[ine].y = pos.y;
		ine++;
		RotationAngle = -atan2(CurrentFormVertices[ine - 1].y - CurrentFormVertices[0].y, CurrentFormVertices[ine - 1].x - CurrentFormVertices[0].x);
		for (ind = 0; ind < ine; ind++)
			rotflt(&CurrentFormVertices[ind]);
		SelectedForm->type = FRMLINE;
		SelectedForm->vertexCount = ine;
		FormVertexIndex += ine;
		frmout(FormIndex);
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
				CurrentFormVertices[ind].x = (CurrentFormVertices[ind].x - CurrentFormVertices[0].x)*hrat + CurrentFormVertices[0].x;
				CurrentFormVertices[ind].y = (CurrentFormVertices[ind].y - CurrentFormVertices[0].y)*hrat + CurrentFormVertices[0].y;
			}
		}
		frmout(FormIndex);
		for (ind = 0; ind < ine; ind++) {
			CurrentFormVertices[ind].x -= SelectedForm->rectangle.left;
			CurrentFormVertices[ind].y -= SelectedForm->rectangle.bottom;
		}
		FormMoveDelta.x = FormMoveDelta.y = 0;
		NewFormVertexCount = ine + 1;
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

