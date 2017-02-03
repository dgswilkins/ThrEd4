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
unsigned		ClosestVertexToCursor;	//point closest to the cursor
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
unsigned		ActivePointIndex;		//pointer to the active point in the sequencing algorithm
unsigned		LineGroupIndex;			//pointer for groups of fill line segments
SMALPNTL**		SortedLines;			//sorted pointers to LineEndpoints
unsigned short	VertexCount;				//sides of the selected form to fill
unsigned		SortedLineIndex;		//for connecting fill lines
unsigned		SequenceIndex;			//sequencing pointer
BSEQPNT			BSequence[BSEQLEN];		//reverse sequence for polygon fills
fPOINT			OSequence[OSEQLEN];		//temporary storage for sequencing
double			Slope;					//slope of line in angle fills
unsigned		SatinIndex;				//pointer to next satin point to enter
fPOINT			FormMoveDelta;			//offset for moving forms
fPOINT			TempPolygon[MAXFRMLINS];	//temporary storage when user is entering a polygon;
unsigned		OutputIndex;			//output pointer for sequencing
double*			Lengths;				//array of cumulative lengths used in satin fills
fPOINT*			CurrentFormVertices;	//points in the currently selected form
SATCON*			CurrentFormConnections;	//connections in the currently selecteed form
unsigned short	CurrentFormConnectionsCount;	//number of connections in the currently selected form
unsigned short	WordParam;				//word paramater from the currently selected form
unsigned*		CheckMap;				//bitmap used by satin fill to delete illegal lines
unsigned short	StartPoint;				//starting point for a satin stitch guide-line
double			HorizontalLength2;		//horizontal length of a clipboard fill/2
double			HorizontalLength;		//horizontal length of a clipboard fill
fPOINT			LastPoint;				//last point written by line connect routine
fPOINT			OutsidePointList[MAXFRMLINS];	//list of outside outline points for satin or clipboard fills
fPOINT			InsidePointList[MAXFRMLINS];	//list of inside outline points for satin or clipboard fills
fPOINT*			OutsidePoints;			//pointer to the list of outside outline points
fPOINT*			InsidePoints;			//pointer to the list of inside outline points
fPOINT			ClipReference;			//clipboard reference point
double			BorderWidth = BRDWID;	//border width for satin borders
unsigned		SelectedFormControlVertex;	//user selected form control point
POINT			FormOutlineRect[10];	//form control rectangle in pixel coordinates
double			XYratio;				//expand form aspect ratio
HWND			ValueWindow[LASTLIN];	//data handles for the form data sheet
HWND			LabelWindow[LASTLIN];	//text handles for the form data sheet
RECT			LabelWindowCoords;		//location of left windows in the form data sheet
RECT			ValueWindowCoords;		//location of right windows in the form data sheet
POINT			LabelWindowSize;		//size of the left windows in the form data sheet
POINT			ValueWindowSize;		//size of the right windows in the form data sheet
fPOINT			LowerLeftStitch;		//lower left point in a form
VRCT2*			FillVerticalRect;		//fill points for vertical satin fill
VRCT2*			UnderlayVerticalRect;	//underlay fill points for vertical satin fill
POINT			RubberBandLine[3];		//points to form points to be moved
unsigned*		Xhistogram;				//x histogram for snap together
double			SnapLength = SNPLEN*PFGRAN;		//snap together length
unsigned*		Xpoints;				//stitch indices sorted according to x values
unsigned		ColorBitmap;			//bitmap of colors in a design for sort
double			StarRatio = STARAT;			//star point to body ratio
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
POINT			SelectedPointsLine[9];	//line derived from the point select rectangle
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
dPOINT			MoveToCoords;			//moving point for clipboard fill
double			CosAngle;				//cosine for clipboard fill
double			SinAngle;				//sine for clipboard fill
fPOINT			BorderClipReference;	//reference for clipboard line border
unsigned		CurrentSide;			//active form point for line clipboard fill
dPOINT			Vector0;				//x size of the clipboard fill at the fill angle
FRMHED*			TempFormList;			//temporary form header storage for reordering forms
fPOINT*			TempFormVertices;		//temporary form vertex storage for reordering forms
SATCON*			TempSatinConnects;		//temporary satin guidline storage for reordering forms
fPOINT*			TempClipPoints;			//temporary clipboard point storage for reordering forms
unsigned		FormRelocationIndex;	//form relocator pointer
fPOINTATTR*		TempStitchBuffer;		//pointer to high stitch area for form sort
unsigned		BeanCount;				//number of stitches added by convert to bean
FRMHED*			FormForInsert;			//insert form vertex in this form
unsigned		FormVertexPrev;			//form vertex storage for form vertex insert
unsigned		FormVertexNext;			//form vertex storage for form vertex insert
SMALPNTL*		SequenceLines;			//line for vertical/horizontal/angle fills
unsigned		PathIndex;				//point to the next path element for vertical fill sequencing
fPOINT*			WorkingFormVertices;	//form points for angle fills
fPOINT			AngledFormVertices[MAXFRMLINS];	//form point data for angle fills
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
		mov		eax, pnt
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
			FormList[iForm].satinOrAngle.sac += guideCount;
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
	FormList[formIndex].satinOrAngle.sac = &SatinConnects[guideIndex];
	sacspac(FormList[formIndex].satinOrAngle.sac, guideCount);
	return FormList[formIndex].satinOrAngle.sac;
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
		sub		eax, offset SatinConnects
		shr		eax, 2
	}
#else
	return pnt - SatinConnects;
#endif
}

unsigned clpind(fPOINT* pnt) {
#if	 __UseASM__
	_asm {
		mov		eax, pnt
		sub		eax, offset ClipPoints
		shr		eax, 3
	}
#else
	return pnt - ClipPoints;
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
	for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
		FormList[ind].vertices += cnt;
	FormVertexIndex += cnt;
}

void delsac(unsigned fpnt) {
	unsigned	ind;
	unsigned	src, dst;

	if (SatinConnectIndex) {
		if (FormList[fpnt].type == SAT&&FormList[fpnt].satinGuideCount) {
			dst = satind(FormList[fpnt].satinOrAngle.sac);
			src = dst + FormList[fpnt].satinGuideCount;
			while (src < SatinConnectIndex) {
				SatinConnects[dst].start = SatinConnects[src].start;
				SatinConnects[dst++].finish = SatinConnects[src++].finish;
			}
			for (ind = fpnt + 1; ind < FormIndex; ind++) {
				if (FormList[ind].type == SAT&&FormList[ind].satinGuideCount)
					FormList[ind].satinOrAngle.sac -= FormList[fpnt].satinGuideCount;
			}
			SatinConnectIndex -= FormList[fpnt].satinGuideCount;
		}
	}
	FormList[fpnt].satinGuideCount = 0;
}

void delflt(unsigned fpnt) {
	unsigned	ind;
	unsigned	src, dst;

	if (FormList[fpnt].vertexCount) {
		dst = fltind(FormList[fpnt].vertices);
		src = dst + FormList[fpnt].vertexCount;
		while (src < FormVertexIndex) {
			FormVertices[dst].x = FormVertices[src].x;
			FormVertices[dst++].y = FormVertices[src++].y;
		}
		for (ind = fpnt + 1; ind < FormIndex; ind++)
			FormList[ind].vertices -= FormList[fpnt].vertexCount;
		FormVertexIndex -= FormList[fpnt].vertexCount;
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

	for (ind = 0; ind < SequenceIndex; ind++)
		rotflt(&OSequence[ind]);
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

	if (istx(ClosestFormToCursor))
		deltx();
	delmclp(ClosestFormToCursor);
	col &= 0xf;
	at = ClosestFormToCursor << FRMSHFT;
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

	delmclp(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	fsizpar();
	if (SelectedForm->fillType != CONTF) {
		if (CurrentFormConnectionsCount) {
			for (ind = 0; ind < SelectedForm->satinGuideCount; ind++) {
				len = SelectedForm->satinOrAngle.sac[ind].finish - SelectedForm->satinOrAngle.sac[ind].start;
				if (len < minlen) {
					minlen = len;
					ine = ind;
				}
			}
			SelectedForm->angleOrClipData.sat.start = SelectedForm->satinOrAngle.sac[ine].start;
			SelectedForm->angleOrClipData.sat.finish = SelectedForm->satinOrAngle.sac[ine].finish;
			delsac(ClosestFormToCursor);
		}
		else {
			SelectedForm->angleOrClipData.sat.start = 1;
			SelectedForm->angleOrClipData.sat.finish = SelectedForm->vertexCount - 2;
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
		at = ClosestFormToCursor << 4;
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
	if (OpenClipboard(ThrEdWindow)) {
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

	if (FormList[ind].vertexCount) {
		trct = &FormList[ind].rectangle;
		CurrentFormVertices = FormList[ind].vertices;
		trct->left = trct->right = CurrentFormVertices[0].x;
		trct->bottom = trct->top = CurrentFormVertices[0].y;
		for (ine = 1; ine < FormList[ind].vertexCount; ine++) {
			if (CurrentFormVertices[ine].x > trct->right)
				trct->right = CurrentFormVertices[ine].x;
			if (CurrentFormVertices[ine].x < trct->left)
				trct->left = CurrentFormVertices[ine].x;
			if (CurrentFormVertices[ine].y < trct->bottom)
				trct->bottom = CurrentFormVertices[ine].y;
			if (CurrentFormVertices[ine].y > trct->top)
				trct->top = CurrentFormVertices[ine].y;
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

	if (VertexCount) {
		for (ind = 0; ind < sidz; ind++) {
			FormLines[ind].x = (scr[ind].x - ZoomRect.left)*ZoomRatio.x;
			FormLines[ind].y = StitchWindowClientRect.bottom - (scr[ind].y - ZoomRect.bottom)*ZoomRatio.y;
		}
		FormLines[ind].x = (scr[0].x - ZoomRect.left)*ZoomRatio.x;
		FormLines[ind].y = StitchWindowClientRect.bottom - (scr[0].y - ZoomRect.bottom)*ZoomRatio.y;
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
	unsigned	ind;
	fPOINT		tpnt;
	fPOINT		dif;

	rats();
	ClosestFormToCursor = FormIndex;
	fvars(ClosestFormToCursor);
	px2stchf(FormLines[0], &tpnt);
	dif.x = tpnt.x - CurrentFormVertices[0].x;
	dif.y = tpnt.y - CurrentFormVertices[0].y;
	SelectedForm->rectangle.left = SelectedForm->rectangle.bottom = (float)1e30;
	SelectedForm->rectangle.right = SelectedForm->rectangle.top = 0;
	for (ind = 0; ind < NewFormVertexCount - 1; ind++) {
		CurrentFormVertices[ind].x += dif.x;
		CurrentFormVertices[ind].y += dif.y;
		if (CurrentFormVertices[ind].x < SelectedForm->rectangle.left)
			SelectedForm->rectangle.left = CurrentFormVertices[ind].x;
		if (CurrentFormVertices[ind].x > SelectedForm->rectangle.right)
			SelectedForm->rectangle.right = CurrentFormVertices[ind].x;
		if (CurrentFormVertices[ind].y > SelectedForm->rectangle.top)
			SelectedForm->rectangle.top = CurrentFormVertices[ind].y;
		if (CurrentFormVertices[ind].y < SelectedForm->rectangle.bottom)
			SelectedForm->rectangle.bottom = CurrentFormVertices[ind].y;
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

void frmsqr(unsigned ind) {
	double	lang;
	double	rat;
	float	len;
	fPOINT	off;
	fPOINT	fpnt;
	fPOINT	l_dpnt;
	dPOINT	dif;
	POINT	sqlin[4];

	stch2pxr(CurrentFormVertices[ind]);
	sqlin[1].x = StitchCoordinatesPixels.x;
	sqlin[1].y = StitchCoordinatesPixels.y;
	rat = (double)IniFile.formVertexSizePixels / StitchWindowClientRect.right;
	len = (ZoomRect.right - ZoomRect.left)*rat * 2;
	dif.x = CurrentFormVertices[ind - 1].x - CurrentFormVertices[ind].x;
	dif.y = CurrentFormVertices[ind - 1].y - CurrentFormVertices[ind].y;
	lang = atan2(dif.y, dif.x);
	off.x = len*cos(lang);
	off.y = len*sin(lang);
	fpnt.x = CurrentFormVertices[ind].x + off.x;
	fpnt.y = CurrentFormVertices[ind].y + off.y;
	lang = atan2(-dif.x, dif.y);
	len /= 2;
	off.x = len*cos(lang);
	off.y = len*sin(lang);
	l_dpnt.x = fpnt.x + off.x;
	l_dpnt.y = fpnt.y + off.y;
	stch2pxr(l_dpnt);
	sqlin[0].x = sqlin[3].x = StitchCoordinatesPixels.x;
	sqlin[0].y = sqlin[3].y = StitchCoordinatesPixels.y;
	l_dpnt.x = fpnt.x - off.x;
	l_dpnt.y = fpnt.y - off.y;
	stch2pxr(l_dpnt);
	sqlin[2].x = StitchCoordinatesPixels.x;
	sqlin[2].y = StitchCoordinatesPixels.y;
	Polyline(StitchWindowMemDC, sqlin, 4);
}

void selsqr(POINT p_cpnt, HDC dc) {
	POINT	sqlin[5];

	sqlin[0].x = sqlin[3].x = sqlin[4].x = p_cpnt.x - IniFile.formVertexSizePixels;
	sqlin[0].y = sqlin[1].y = p_cpnt.y - IniFile.formVertexSizePixels;
	sqlin[1].x = sqlin[2].x = p_cpnt.x + IniFile.formVertexSizePixels;
	sqlin[2].y = sqlin[3].y = p_cpnt.y + IniFile.formVertexSizePixels;
	sqlin[4].y = p_cpnt.y - IniFile.formVertexSizePixels;
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
		HorizontalRatio = (double)StitchWindowClientRect.right / (ZoomRect.right - ZoomRect.left);
		VerticalRatio = (double)StitchWindowClientRect.bottom / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		HorizontalRatio = (double)StitchWindowClientRect.right / UnzoomedRect.x;
		VerticalRatio = (double)StitchWindowClientRect.bottom / UnzoomedRect.y;
	}
}

float midl(float hi, float lo) {
	return (hi - lo) / 2 + lo;
}

void fvars(unsigned ind) {
	SelectedForm = &FormList[ind];
	CurrentFormVertices = FormList[ind].vertices;
	VertexCount = FormList[ind].vertexCount;
	CurrentFormConnections = FormList[ind].satinOrAngle.sac;
	CurrentFormConnectionsCount = FormList[ind].satinGuideCount;
	WordParam = FormList[ind].wordParam;
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
	trct = &FormList[ind].rectangle;
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
	FormIndex = FormVertexIndex = SatinConnectIndex = ClipPointIndex = 0;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		StitchBuffer[ind].attribute &= NFRM_NTYP;
		StitchBuffer[ind].attribute |= NOTFRM;
	}
}

void fselrct(unsigned fpnt) {
	FRMHED*		tpnt = &FormList[fpnt];
	fPOINT		tlin[5];
	POINT		sqrlin[6];
	unsigned	ind;

	tlin[0].x = tlin[3].x = tlin[4].x = tpnt->rectangle.left;
	tlin[1].x = tlin[2].x = tpnt->rectangle.right;
	tlin[0].y = tlin[1].y = tlin[4].y = tpnt->rectangle.top;
	tlin[2].y = tlin[3].y = tpnt->rectangle.bottom;
	for (ind = 0; ind < 5; ind++) {
		sqrlin[ind].x = (tlin[ind].x - ZoomRect.left)*HorizontalRatio;
		sqrlin[ind].y = (ZoomRect.top - tlin[ind].y)*VerticalRatio;
		if (sqrlin[ind].x < SelectedFormsRect.left)
			SelectedFormsRect.left = sqrlin[ind].x;
		if (sqrlin[ind].x > SelectedFormsRect.right)
			SelectedFormsRect.right = sqrlin[ind].x;
		if (sqrlin[ind].y < SelectedFormsRect.top)
			SelectedFormsRect.top = sqrlin[ind].y;
		if (sqrlin[ind].y > SelectedFormsRect.bottom)
			SelectedFormsRect.bottom = sqrlin[ind].y;
	}
	sqrlin[5].x = (tlin[0].x - ZoomRect.left)*HorizontalRatio;
	sqrlin[5].y = (ZoomRect.top - tlin[0].y)*VerticalRatio;
	if (sqrlin[5].x < SelectedFormsRect.left)
		SelectedFormsRect.left = sqrlin[5].x;
	if (sqrlin[5].x > SelectedFormsRect.right)
		SelectedFormsRect.right = sqrlin[5].x;
	if (sqrlin[5].y < SelectedFormsRect.top)
		SelectedFormsRect.top = sqrlin[5].y;
	if (sqrlin[5].y > SelectedFormsRect.bottom)
		SelectedFormsRect.bottom = sqrlin[5].y;
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

	rct2sel(SelectedFormsRect, SelectedFormsLine);
	SelectObject(StitchWindowMemDC, SelectAllPen);
	Polyline(StitchWindowMemDC, SelectedFormsLine, 9);
	for (ind = 0; ind < 8; ind++)
		selsqr(SelectedFormsLine[ind], StitchWindowMemDC);
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
	Polyline(dc, SelectedPointsLine, 9);
	ind = SelectedFormVertices.start;
	for (ind = 0; ind < 8; ind++)
		selsqr(SelectedPointsLine[ind], dc);
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
	for (ind = 0; ind < FormIndex; ind++) {
		fvars(ind);
		frmlin(SelectedForm->vertices, VertexCount);
		inf = 0;
		layr = ((SelectedForm->attribute&FRMLMSK) >> 1);
		if (!ActiveLayer || !layr || layr == ActiveLayer) {
			if (SelectedForm->type == SAT) {
				if (SelectedForm->attribute&FRMEND) {
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, FormLines, 2);
					inf = 1;
				}
				if (SelectedForm->wordParam) {
					SelectObject(StitchWindowMemDC, FormPen);
					frmpoly(&FormLines[1], SelectedForm->wordParam);
					SelectObject(StitchWindowMemDC, FormPen3px);
					Polyline(StitchWindowMemDC, &FormLines[SelectedForm->wordParam], 2);
					SelectObject(StitchWindowMemDC, LayerPen[layr]);
					inf = SelectedForm->wordParam + 1;
				}
				for (ine = 0; ine < FormList[ind].satinGuideCount; ine++) {
					sfCor2px(CurrentFormVertices[CurrentFormConnections[ine].start], &l_lin[0]);
					sfCor2px(CurrentFormVertices[CurrentFormConnections[ine].finish], &l_lin[1]);
					SelectObject(StitchWindowMemDC, FormPen);
					Polyline(StitchWindowMemDC, l_lin, 2);
				}
			}
			SelectObject(StitchWindowMemDC, LayerPen[layr]);
			if (SelectedForm->type == FRMLINE) {
				frmpoly(FormLines, VertexCount);
				if (SelectedForm->fillType == CONTF) {
					tpnt.x = CurrentFormVertices[SelectedForm->angleOrClipData.sat.start].x;
					tpnt.y = CurrentFormVertices[SelectedForm->angleOrClipData.sat.start].y;
					sCor2px(tpnt, &l_lin[0]);
					tpnt.x = CurrentFormVertices[SelectedForm->angleOrClipData.sat.finish].x;
					tpnt.y = CurrentFormVertices[SelectedForm->angleOrClipData.sat.finish].y;
					sCor2px(tpnt, &l_lin[1]);
					Polyline(StitchWindowMemDC, l_lin, 2);
				}
			}
			else
				frmpoly(&FormLines[inf], VertexCount + 1 - inf);
			if (ClosestFormToCursor == ind&&chkMap(FRMPSEL)) {
				for (ine = 1; ine < VertexCount; ine++) {
					if (ine == ClosestVertexToCursor)
						frmx(FormLines[ine], StitchWindowMemDC);
					else
						frmsqr(ine);
				}
				if (ClosestVertexToCursor)
					frmsqr0(FormLines[0]);
				else
					frmx(FormLines[0], StitchWindowMemDC);
				ritnum(STR_NUMPNT, ClosestVertexToCursor);
			}
			else {
				for (ine = 1; ine < VertexCount; ine++)
					frmsqr(ine);
				SelectObject(StitchWindowMemDC, FormSelectedPen);
				frmsqr0(FormLines[0]);
			}
			if (chkMap(FPSEL) && ClosestFormToCursor == ind) {
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
		for (ind = 0; ind < SelectedFormCount; ind++)
			fselrct(SelectedFormList[ind]);
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
	ind = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, (DLGPROC)tearprc);
	if (ind > 0) {
		xstp = IniFile.tearTwistStep;
		durpoli(IniFile.formSides);
		fvars(FormIndex);
		cnt = VertexCount / 4;
		mid = (CurrentFormVertices[1].x - CurrentFormVertices[0].x) / 2 + CurrentFormVertices[0].x;
		len = CurrentFormVertices[cnt].y - CurrentFormVertices[0].y;
		stp = CurrentFormVertices[cnt + 1].y - CurrentFormVertices[cnt].y;
		vpos = CurrentFormVertices[cnt + 1].y;
		lind = VertexCount - cnt;
		rind = cnt + 1;
		for (ind = 0; ind < (unsigned)cnt; ind++) {
			CurrentFormVertices[rind].y = CurrentFormVertices[lind].y = vpos;
			CurrentFormVertices[rind].x += xstp;
			CurrentFormVertices[lind].x += xstp;
			xstp *= IniFile.tearTwistRatio;
			vpos -= stp;
			stp *= IniFile.tearTailLength;
			rind--;
			lind++;
		}
		CurrentFormVertices[0].y = CurrentFormVertices[1].y = vpos;
		CurrentFormVertices[0].x += xstp;
		CurrentFormVertices[1].x += xstp;
		vpos -= stp / 2;
		CurrentFormVertices[VertexCount].x = CurrentFormVertices[0].x;
		CurrentFormVertices[VertexCount].y = CurrentFormVertices[0].y;
		if (xstp)
			CurrentFormVertices[0].x = CurrentFormVertices[1].x + xstp / 4;
		else
			CurrentFormVertices[0].x = mid;
		CurrentFormVertices[0].y = vpos;
		SelectedForm->vertexCount++;
		NewFormVertexCount++;
		FormVertexIndex++;
		setMap(FORMSEL);
		fvars(FormIndex);
		frmout(FormIndex);
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
			for (ind = 0; ind < VertexCount; ind++) {
				CurrentFormVertices[ind].x = (CurrentFormVertices[ind].x - CurrentFormVertices[0].x)*hrat + CurrentFormVertices[0].x;
				CurrentFormVertices[ind].y = (CurrentFormVertices[ind].y - CurrentFormVertices[0].y)*hrat + CurrentFormVertices[0].y;
			}
		}
		frmout(FormIndex);
		for (ind = 0; ind < VertexCount; ind++) {
			CurrentFormVertices[ind].x -= SelectedForm->rectangle.left;
			CurrentFormVertices[ind].y -= SelectedForm->rectangle.bottom;
		}
	}
}

void duform(unsigned ind) {
	frmclr(&FormList[FormIndex]);
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

	if (FormIndex) {
		stchpx.x = Msg.pt.x - StitchWindowOrigin.x;
		stchpx.y = Msg.pt.y - StitchWindowOrigin.y;
		rats();
		tind = tine = 0;
		px2stchf(stchpx, &tpnt);
		playcod = ActiveLayer << 1;
		for (ind = 0; ind < FormIndex; ind++) {
			if (chkMap(FRMSAM) && ind == ClosestFormToCursor)
				continue;

			frmcod = FormList[ind].attribute&FRMLMSK;
			if (!ActiveLayer || !frmcod || frmcod == playcod) {
				getfinfo(ind);
				pdat = FormList[ind].vertices;
				for (ine = 0; ine < FormInfo.sideCount; ine++) {
					len = hypot(tpnt.x - pdat[ine].x, tpnt.y - pdat[ine].y);
					if (len < minlen&&len >= 0) {
						minlen = len;
						tind = ind;
						tine = ine;
					}
				}
			}
		}
		stch2pxr(FormList[tind].vertices[tine]);
		minlen = hypot(StitchCoordinatesPixels.x - stchpx.x, StitchCoordinatesPixels.y - stchpx.y);
		if (minlen < CLOSENUF) {
			ClosestFormToCursor = tind;
			ClosestVertexToCursor = tine;
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
	unsigned ind, ine;

	fvars(ClosestFormToCursor);
	ratsr();
	if (SelectedForm->type == FRMLINE)
		NewFormVertexCount = SelectedForm->vertexCount;
	else
		NewFormVertexCount = SelectedForm->vertexCount + 1;
	frmlin(SelectedForm->vertices, SelectedForm->vertexCount);
	ine = prv(ClosestVertexToCursor);
	for (ind = 0; ind < 3; ind++)
	{
		RubberBandLine[ind].x = FormLines[ine].x;
		RubberBandLine[ind].y = FormLines[ine].y;
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
		StitchBuffer[strt].x = InterleaveSequence[ostrt].x;
		StitchBuffer[strt++].y = InterleaveSequence[ostrt++].y;
	}
	ostrt--;
	LastPoint.x = InterleaveSequence[ostrt].x;
	LastPoint.y = InterleaveSequence[ostrt].y;
}

BOOL ritlin(fPOINT strt, fPOINT fin)
{
	dPOINT		dif, stp, pnt;
	double		len;
	unsigned	ine, cnt;

	dif.x = fin.x - strt.x;
	dif.y = fin.y - strt.y;
	len = hypot(dif.x, dif.y);
	InterleaveSequence[InterleaveSequenceIndex].x = strt.x;
	InterleaveSequence[InterleaveSequenceIndex++].y = strt.y;
	if (len > MaxStitchLen) {
		cnt = ceil(len / UserStichLen);
		if (!cnt)
			cnt = 1;
		while (len / cnt > MaxStitchLen)
			cnt++;
		if (!chkmax(InterleaveSequenceIndex, cnt)) {
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			pnt.x = strt.x + stp.x;
			pnt.y = strt.y + stp.y;
			for (ine = 0; ine < cnt - 1; ine++) {
				if (InterleaveSequenceIndex&MAXMSK) {
					InterleaveSequenceIndex = MAXSEQ - 2;
					return 0;
				}
				InterleaveSequence[InterleaveSequenceIndex].x = pnt.x;
				InterleaveSequence[InterleaveSequenceIndex++].y = pnt.y;
				pnt.x += stp.x;
				pnt.y += stp.y;
			}
		}
		else {
			SequenceIndex = MAXSEQ - 2;
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
	for (ind = 0; ind < VertexCount; ind++)
	{
		len = hypot(px - CurrentFormVertices[ind].x, py - CurrentFormVertices[ind].y);
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

	for (index = 0; index < SequenceIndex; index++)
	{
		InterleaveSequence[index].x = OSequence[index].x;
		InterleaveSequence[index].y = OSequence[index].y;
	}
	InterleaveSequenceIndex = SequenceIndex;
#else

	double		len;
	unsigned	ind, ine, bakind;
	float		mins;

	bakind = InterleaveSequenceIndex;
	if (brd) {
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EDGELCHAIN || SelectedForm->edgeType == EDGEOCHAIN)
			UserStichLen = 9 * PFGRAN;
		else
			UserStichLen = SelectedForm->edgeStitchLen;
		mins = SelectedForm->minBorderStitchLen;
	}
	else {
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxFillStitchLen;
		if (isclp(ClosestFormToCursor))
			UserStichLen = MaxStitchLen;
		else
			UserStichLen = SelectedForm->lengthOrCount.stitchLength;
		mins = SelectedForm->minFillStitchLen;
	}
	if (UserStichLen > MaxStitchLen)
		UserStichLen = MaxStitchLen;
	ine = 0;
	for (ind = 0; ind < SequenceIndex - 1; ind++)
	{
		if (!ritlin(OSequence[ind], OSequence[ind + 1]))
			goto seqskp;
	}
	InterleaveSequence[InterleaveSequenceIndex].x = OSequence[ind].x;
	InterleaveSequence[InterleaveSequenceIndex++].y = OSequence[ind].y;
seqskp:;
	if (!mins)
		return;
	ine = bakind + 1;
	for (ind = bakind + 1; ind < InterleaveSequenceIndex; ind++) {
		len = hypot(InterleaveSequence[ind].x - InterleaveSequence[ind - 1].x, InterleaveSequence[ind].y - InterleaveSequence[ind - 1].y);
		if (len > mins) {
			InterleaveSequence[ine].x = InterleaveSequence[ind].x;
			InterleaveSequence[ine].y = InterleaveSequence[ind].y;
			ine++;
		}
	}
	InterleaveSequenceIndex = ine;
#endif
}

void ritbrd() {
	if (SequenceIndex)
	{
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_BRD;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPBRD;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->borderColor&COLMSK;
		chkseq(1);
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
		chkseq(1);
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
		chkseq(0);
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
	TCHAR	buf[HBUFSIZ];

	sdmsg();
	rstMap(BIGBOX);

	GetClientRect(MsgWindow, &MsgRect);

	LoadString(ThrEdInstance, IDS_SAV, buf, HBUFSIZ);
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
		ThrEdInstance,
		NULL);

	LoadString(ThrEdInstance, IDS_DISC, buf, HBUFSIZ);
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
	unsigned		ind;
	unsigned		tclos = 0;
	double			len, dx, dy, min = 1e99;

	if (SelectedForm->fillType) {
		lastch();
		for (ind = 0; ind < VertexCount; ind++) {
			dx = LastPoint.x - CurrentFormVertices[ind].x;
			dy = LastPoint.y - CurrentFormVertices[ind].y;
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

	tspac = StitchSpacing;
	SelectedPoint.x = CurrentFormVertices[strt].x;
	SelectedPoint.y = CurrentFormVertices[strt].y;
	for (ind = strt + 1; ind < VertexCount; ind++) {
		flt2dub(CurrentFormVertices[ind], &tpnt);
		filinsb(tpnt);
	}
	flt2dub(CurrentFormVertices[0], &tpnt);
	filinsb(tpnt);
	StitchSpacing = tspac;
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

	tlen = (FormList[ClosestFormToCursor].clipEntries << 16) | FormList[ClosestFormToCursor].picoLength;

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

	x.tlen = (FormList[ClosestFormToCursor].clipEntries << 16) | FormList[ClosestFormToCursor].picoLength;
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
	FormList[ClosestFormToCursor].clipEntries = l_nclp;
	FormList[ClosestFormToCursor].picoLength = picoLength;
#else
	union {
		unsigned int i;
		float f;
	} x;

	x.f = len;

	FormList[ClosestFormToCursor].clipEntries = x.i >> 16;
	FormList[ClosestFormToCursor].picoLength = x.i & 0xFFFF;
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
	FormList[ClosestFormToCursor].picoLength = (unsigned short)(num << 8) | fr;
}

void chkbrd() {
	float	tlen;

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
			tlen = ButtonholeCornerLength;
			ButtonholeCornerLength = getblen();
			satout(20);
			bhbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = tlen;
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

	dif.x = CurrentFormVertices[fin].x - CurrentFormVertices[strt].x;
	dif.y = CurrentFormVertices[fin].y - CurrentFormVertices[strt].y;
	len = hypot(dif.x, dif.y);
	cnt = len / pd_Size;
	if (cnt) {
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		pnt0.x = CurrentFormVertices[strt].x;
		pnt0.y = CurrentFormVertices[strt].y;
		pnt1.x = pnt0.x + stp.x;
		pnt1.y = pnt0.y + stp.y;
		while (cnt) {
			OSequence[SequenceIndex].x = pnt1.x;
			OSequence[SequenceIndex++].y = pnt1.y;
			OSequence[SequenceIndex].x = pnt0.x;
			OSequence[SequenceIndex++].y = pnt0.y;
			OSequence[SequenceIndex].x = pnt1.x;
			OSequence[SequenceIndex++].y = pnt1.y;
			pnt0.x += stp.x;
			pnt0.y += stp.y;
			pnt1.x += stp.x;
			pnt1.y += stp.y;
			cnt--;
		}
		OSequence[SequenceIndex].x = CurrentFormVertices[fin].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[fin].y;
	}
	else {
		OSequence[SequenceIndex].x = CurrentFormVertices[fin].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[fin].y;
		OSequence[SequenceIndex].x = CurrentFormVertices[strt].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[strt].y;
		OSequence[SequenceIndex].x = CurrentFormVertices[fin].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[fin].y;
	}
}

void bold(double pd_Size) {
	unsigned		ind, ine = 0;
	unsigned short	nlin, tlin;
	double			len;

	nlin = tlin = getlast();
	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[tlin].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[tlin].y;
	for (ind = 0; ind < (unsigned)VertexCount - 1; ind++) {
		nlin = nxt(tlin);
		boldlin(tlin, nlin, pd_Size);
		tlin = nlin;
	}
	if (SelectedForm->type != FRMLINE) {
		nlin = nxt(tlin);
		boldlin(tlin, nlin, pd_Size);
	}
	for (ind = 0; ind < SequenceIndex - 1; ind++) {
		len = hypot(OSequence[ind + 1].x - OSequence[ind].x, OSequence[ind + 1].y - OSequence[ind].y);
		if (len > TINY) {
			OSequence[ine].x = OSequence[ind].x;
			OSequence[ine++].y = OSequence[ind].y;
		}
	}
	OSequence[ine].x = CurrentFormVertices[nlin].x;
	OSequence[ine++].y = CurrentFormVertices[nlin].y;
	SequenceIndex = ine;
}

void refilfn() {
	double		tspac;
	double		tsiz = UserStitchLength;
	float		tlen;

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
			tlen = ButtonholeCornerLength;
			ButtonholeCornerLength = getblen();
			satout(20);
			blbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = tlen;
			ritbrd();
			break;

		case EDGEPICOT:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			SequenceIndex = 0;
			tlen = ButtonholeCornerLength;
			ButtonholeCornerLength = getplen();
			clpic(0);
			ButtonholeCornerLength = tlen;
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
			tspac = StitchSpacing;
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
			StitchSpacing = tspac;
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

			tspac = StitchSpacing;
			StitchSpacing = SelectedForm->fillSpacing;
			UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
			satfil();
			StitchSpacing = tspac;
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
		trg = (ClosestFormToCursor << FRMSHFT) | USMSK;
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
	fvars(ClosestFormToCursor);
	l_dpnt = &SelectedForm->vertices[0];
	stchpx.x = Msg.pt.x - StitchWindowOrigin.x;
	stchpx.y = Msg.pt.y - StitchWindowOrigin.y;
	rats();
	px2stchf(stchpx, &l_dpnt[ClosestVertexToCursor]);
	frmout(ClosestFormToCursor);
	refil();
	setMap(WASFPNT);
	rstMap(SELBOX);
	setMap(FRMPSEL);
	ritfcor(&CurrentFormVertices[ClosestVertexToCursor]);
	setMap(RESTCH);
}

unsigned short nxt(unsigned short ind) {
	ind++;
	if (ind > (unsigned)VertexCount - 1)
		ind = 0;
	return ind;
}

unsigned short prv(unsigned ind) {
	if (ind)
		ind--;
	else
		ind = VertexCount - 1;
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

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	for (ind = 0; ind < AngledForm.vertexCount; ind++) {
		AngledForm.vertices[ind].x = SelectedForm->vertices[ind].x;
		AngledForm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&AngledForm.vertices[ind]);
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
	if (chkmax(cnt, SequenceIndex) || cnt + SequenceIndex > MAXSEQ - 3)
		return;
	if (cnt) {
		if (chkMap(FILDIR))
			cnt++;
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		while (cnt > 0) {
			ipnt.x += stp.x;
			ipnt.y += stp.y;
			OSequence[SequenceIndex].x = ipnt.x;
			OSequence[SequenceIndex++].y = ipnt.y;
			cnt--;
		}
	}
	else {
		OSequence[SequenceIndex].x = pntx;
		OSequence[SequenceIndex++].y = pnty;
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
	if (chkmax(cnt, SequenceIndex) || (cnt + SequenceIndex) > MAXSEQ - 3)
		return;
	if (cnt) {
		if (chkMap(FILDIR))
			cnt++;
		stp.x = dif.x / cnt;
		stp.y = dif.y / cnt;
		while (cnt > 0) {
			ipnt.x += stp.x;
			ipnt.y += stp.y;
			OSequence[SequenceIndex].x = ipnt.x;
			OSequence[SequenceIndex++].y = ipnt.y;
			cnt--;
		}
	}
	else {
		OSequence[SequenceIndex].x = pnt.x;
		OSequence[SequenceIndex++].y = pnt.y;
	}
	SelectedPoint.x = pnt.x;
	SelectedPoint.y = pnt.y;
}

unsigned short isclos(SMALPNTL* pnt0, SMALPNTL* pnt1) {
	float		lo0, hi0, lo1, hi1;

	hi0 = pnt0[1].y + GapToClosestRegion;
	lo0 = pnt0[0].y - GapToClosestRegion;
	hi1 = pnt1[1].y + GapToClosestRegion;
	lo1 = pnt1[0].y - GapToClosestRegion;
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

	if (gp1 > GroupIndexCount - 2)
		return 0;
	if (!gp0)
		return 0;
	cnt0 = (GroupIndexSequence[gp0 + 1] - GroupIndexSequence[gp0]) >> 1;
	ind0 = 0;
	pnt0 = &LineEndpoints[GroupIndexSequence[gp0]];
	while (cnt0&&pnt0[ind0].line != ln0) {
		cnt0--;
		ind0 += 2;
	}
	if (cnt0) {
		cnt1 = (GroupIndexSequence[gp1 + 1] - GroupIndexSequence[gp1]) >> 1;
		ind1 = 0;
		pnt1 = &LineEndpoints[GroupIndexSequence[gp1]];
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

	pnt0s = &*SortedLines[RegionsList[rg0].start];
	pnt1s = &*SortedLines[RegionsList[rg1].start];
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
		NextGroup = grps;
		return 1;
	}
	else {
		pnt0e = &*SortedLines[RegionsList[rg0].end];
		pnt1e = &*SortedLines[RegionsList[rg1].end];
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
			NextGroup = grpe;
			return 1;
		}
	}
	if (((grp0s > grp1s) ? (grp0s - grp1s) : (grp1s - grp0s)) < 2) {
		if (isclos(pnt0s, pnt1s)) {
			NextGroup = grp0s;
			return 1;
		}
	}
	if (((grp0s > grp1e) ? (grp0s - grp1e) : (grp1e - grp0s)) < 2) {
		if (isclos(pnt0s, pnt1e)) {
			NextGroup = grp0s;
			return 1;
		}
	}
	if (((grp0e > grp1s) ? (grp0e - grp1s) : (grp1s - grp0e)) < 2) {
		if (isclos(pnt0e, pnt1s)) {
			NextGroup = grp0e;
			return 1;
		}
	}
	if (((grp0e > grp1e) ? (grp0e - grp1e) : (grp1e - grp0e)) < 2) {
		if (isclos(pnt0e, pnt1e)) {
			NextGroup = grp0e;
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

unsigned setseq(unsigned bpnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, SequenceFlagBitmap
		mov		ecx, bpnt
		bts[ebx], ecx
		jnc		short setseqx
		inc		eax
		setseqx :
	}
#else
	return _bittestandset((long *)SequenceFlagBitmap, bpnt);
#endif
}

void rspnt(float fx, float fy) {
	BSequence[OutputIndex].x = fx;
	BSequence[OutputIndex].y = fy;
	BSequence[OutputIndex++].attribute = 0;
}

void dunseq(unsigned strt, unsigned fin) {
	SMALPNTL*	l_lin0;
	SMALPNTL*	l_lin1;
	unsigned	ind;
	double		dy, miny;

	miny = 1e30;
	for (ind = strt; ind <= fin; ind++) {
		l_lin0 = &*SortedLines[ind];
		dy = l_lin0[1].y - l_lin0->y;
		if (dy < miny)
			miny = dy;
	}
	miny /= 2;
	l_lin0 = &*SortedLines[strt];
	l_lin1 = &*SortedLines[fin];
	if (miny == 1e30 / 2)
		miny = 0;
	rspnt(l_lin0->x, l_lin0->y + miny);
	rspnt(l_lin1->x, l_lin1->y + miny);
	LastGroup = l_lin1->group;
}

void movseq(unsigned ind) {
	SMALPNTL* l_lin;

	l_lin = &*SortedLines[ind];
	BSequence[OutputIndex].attribute = SEQBOT;
	BSequence[OutputIndex].x = l_lin->x;
	BSequence[OutputIndex].y = l_lin->y;
	OutputIndex++;
	l_lin++;
	BSequence[OutputIndex].attribute = SEQTOP;
	BSequence[OutputIndex].x = l_lin->x;
	BSequence[OutputIndex].y = l_lin->y;
	OutputIndex++;
}

void duseq2(unsigned ind) {
	SequenceLines = &*SortedLines[ind];
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x, (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq1() {
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x, (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq(unsigned strt, unsigned fin) {
	unsigned	ind, topbak;

	SequenceLines = nullptr;
	rstMap(SEQDUN);
	topbak = SortedLines[strt][1].line;
	if (strt > fin) {
		for (ind = strt; (int)ind >= (int)fin; ind--) {
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq2(ind);
				else {
					if (topbak != SortedLines[ind][1].line) {
						if (ind)
							duseq2(ind + 1);
						duseq2(ind);
						topbak = SequenceLines[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN))
					duseq2(ind + 1);
				SequenceLines = &*SortedLines[ind];
				movseq(ind);
			}
		}
		if (rstMap(SEQDUN))
			duseq2(ind + 1);
		if (SequenceLines != nullptr) { LastGroup = SequenceLines->group; }
	}
	else {
		for (ind = strt; ind <= fin; ind++) {
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq2(ind);
				else {
					if (topbak != SortedLines[ind][1].line) {
						if (ind)
							duseq2(ind - 1);
						duseq2(ind);
						topbak = SequenceLines[1].line;
					}
				}
			}
			else {
				if (rstMap(SEQDUN)) {
					if (ind)
						duseq2(ind - 1);
				}
				SequenceLines = &*SortedLines[ind];
				movseq(ind);
			}
		}
		if (rstMap(SEQDUN)) {
			if (ind)
				duseq2(ind - 1);
		}
		if (SequenceLines != nullptr) { LastGroup = SequenceLines->group; }
	}
}

void brkseq(unsigned strt, unsigned fin) {
	//SMALPNTL* line=0;
	unsigned	ind, bgrp = 0;

	rstMap(SEQDUN);
	if (strt > fin) {
		bgrp = SortedLines[strt]->group + 1;
		for (ind = strt; (int)ind >= (int)fin; ind--) {
			bgrp--;
			if (SortedLines[ind]->group != bgrp) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = &*SortedLines[ind];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				bgrp = SequenceLines[0].group;
			}
			else
				SequenceLines = &*SortedLines[ind];
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(ind);
		}
		LastGroup = SequenceLines->group;
	}
	else {
		bgrp = SortedLines[strt]->group - 1;
		for (ind = strt; ind <= fin; ind++) {
			bgrp++;
			if (SortedLines[ind]->group != bgrp) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = &*SortedLines[ind];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				bgrp = SequenceLines[0].group;
			}
			else
				SequenceLines = &*SortedLines[ind];
			if (setseq(ind)) {
				if (!setMap(SEQDUN))
					duseq1();
			}
			else
				movseq(ind);
		}
		LastGroup = SequenceLines->group;
	}
	if (rstMap(SEQDUN))
		duseq1();
}

void brkdun(unsigned strt, unsigned fin) {
	rspnt(SortedLines[strt]->x, SortedLines[strt]->y);
	rspnt(SortedLines[fin]->x, SortedLines[fin]->y);
	rspnt(WorkingFormVertices[SortedLines[strt]->line].x, WorkingFormVertices[SortedLines[strt]->line].y);
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

	rgind = SequencePath[pthi].vrt;
	CurrentRegion = &RegionsList[rgind];
	grpn = SequencePath[pthi].grpn;
	seqs = CurrentRegion->start;
	seqe = CurrentRegion->end;
	if (SequencePath[pthi].skp || rstMap(BRKFIX)) {
		if (BSequence[OutputIndex - 1].attribute != SEQBOT)
			rspnt(BSequence[OutputIndex - 2].x, BSequence[OutputIndex - 2].y);
		pnts = &*SortedLines[rgind];
		dun = SortedLines[seqs]->line;
		bpnt = &BSequence[OutputIndex - 1];
		minlen = 1e99;
		for (ind = 0; ind < VertexCount; ind++) {
			len = hypot(bpnt->x - WorkingFormVertices[ind].x, bpnt->y - WorkingFormVertices[ind].y);
			if (len < minlen) {
				minlen = len;
				mindif = ind;
			}
		}
		if (minlen)
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
	pnts = &*SortedLines[CurrentRegion->start];
	pnte = &*SortedLines[CurrentRegion->end];
	grps = pnts->group;
	grpe = pnte->group;
	if (grpe != grps)
		seql = (double)(LastGroup - grps) / (grpe - grps)*(seqe - seqs) + seqs;
	else
		seql = 0;
	if (seql > SortedLineIndex)
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
	if (SortedLines[seql]->group != LastGroup) {
		if (seql < seqe&&SortedLines[seql + 1]->group == LastGroup)
			seql++;
		else {
			if (seql > seqs&&SortedLines[seql - 1]->group == LastGroup)
				seql--;
			else {
				mindif = 0xffffffff;
				for (ind = seqs; ind <= seqe; ind++) {
					gdif = ((SortedLines[ind]->group > LastGroup) ? (SortedLines[ind]->group - LastGroup) : (LastGroup - SortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seql = ind;
					}
				}
			}
		}
	}
	if (SortedLines[seqn]->group != grpn) {
		if (seqn < seqe&&SortedLines[seqn + 1]->group == grpn)
			seqn++;
		else {
			if (seqn > seqs&&SortedLines[seqn - 1]->group == grpn)
				seqn--;
			else {
				mindif = 0xffffffff;
				for (ind = seqs; ind <= seqe; ind++) {
					gdif = ((SortedLines[ind]->group > grpn) ? (SortedLines[ind]->group - grpn) : (grpn - SortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seqn = ind;
					}
				}
			}
		}
	}
	if (CurrentRegion->cntbrk) {
		if (dun) {
			brkdun(seql, seqn);
		}
		else {
			if (LastGroup >= grpe) {
				brkseq(seqe, seqs);
				if (pthi < SequencePathIndex - 1 && seqe != seqn)
					brkseq(seqs, seqn);
			}
			else {
				if (grps <= grpn) {
					if (seql != seqs)
						brkseq(seql, seqs);
					brkseq(seqs, seqe);
					if (pthi < SequencePathIndex - 1 && seqe != seqn)
						brkseq(seqe, seqn);
				}
				else {
					if (seql != seqe)
						brkseq(seql, seqe);
					brkseq(seqe, seqs);
					if (pthi < SequencePathIndex - 1 && seqs != seqn)
						brkseq(seqs, seqn);
				}
			}
		}
	}
	else {
		if (dun)
			dunseq(seql, seqn);
		else {
			if (LastGroup >= grpe) {
				duseq(seqe, seqs);
				duseq(seqs, seqn);
			}
			else {
				if (grps <= grpn) {
					if (seql != seqs)
						duseq(seql, seqs);
					duseq(seqs, seqe);
					if (pthi < SequencePathIndex - 1 && seqe != seqn)
						duseq(seqe, seqn);
				}
				else {
					if (seql != seqe)
						duseq(seql, seqe);
					duseq(seqe, seqs);
					if (pthi < SequencePathIndex - 1 && seqs != seqn)
						duseq(seqs, seqn);
				}
			}
		}
	}
}

unsigned notdun(unsigned lvl) {
	unsigned	ind;
	int			tpiv, pivot = lvl - 1;

	RegionPath = &TempPath[SequencePathIndex];
	RegionPath[0].pcon = MapIndexSequence[DoneRegion];
	RegionPath[0].cnt = MapIndexSequence[DoneRegion + 1] - RegionPath[0].pcon;
	for (ind = 1; ind < lvl; ind++) {
		RegionPath[ind].pcon = MapIndexSequence[PathMap[RegionPath[ind - 1].pcon].vrt];
		RegionPath[ind].cnt = MapIndexSequence[PathMap[RegionPath[ind - 1].pcon].vrt + 1] - RegionPath[ind].pcon;
	}
	while (VisitedRegions[PathMap[RegionPath[pivot].pcon].vrt] && pivot >= 0) {
		if (--RegionPath[pivot].cnt > 0)
			RegionPath[pivot].pcon++;
		else {
			tpiv = pivot;
			do {
				tpiv--;
				if (tpiv < 0)
					return 1;
				RegionPath[tpiv].cnt--;
				RegionPath[tpiv].pcon++;
			} while (!RegionPath[tpiv].cnt);
			if (tpiv < 0)
				return 1;
			tpiv++;
			while (tpiv <= pivot) {
				if (tpiv) {
					RegionPath[tpiv].pcon = MapIndexSequence[PathMap[RegionPath[tpiv - 1].pcon].vrt];
					RegionPath[tpiv].cnt = MapIndexSequence[PathMap[RegionPath[tpiv - 1].pcon].vrt + 1] - RegionPath[tpiv].pcon;
				}
				else {
					if (--RegionPath[0].cnt)
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

double reglen(unsigned reg) {
	double		len, minlen = 1e99;
	unsigned	ind, ine;
	SMALPNTL*	pnts[4];

	pnts[0] = SortedLines[RegionsList[reg].start];
	pnts[1] = &SortedLines[RegionsList[reg].start][1];
	pnts[2] = SortedLines[RegionsList[reg].end];
	pnts[3] = &SortedLines[RegionsList[reg].end][1];
	for (ind = 0; ind < 4; ind++) {
		for (ine = 0; ine < 4; ine++) {
			len = hypot(LastRegionCorners[ind].x - pnts[ine]->x, LastRegionCorners[ind].y - pnts[ine]->y);
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
			tpnt = &*SortedLines[RegionsList[DoneRegion].start];
			LastRegionCorners[0].x = tpnt[0].x;
			LastRegionCorners[0].y = tpnt[0].y;
			LastRegionCorners[1].x = tpnt[1].x;
			LastRegionCorners[1].y = tpnt[1].y;
			tpnt = &*SortedLines[RegionsList[DoneRegion].end];
			LastRegionCorners[2].x = tpnt[0].x;
			LastRegionCorners[2].y = tpnt[0].y;
			LastRegionCorners[3].x = tpnt[1].x;
			LastRegionCorners[3].y = tpnt[1].y;
			newRegion = 0;
			for (ind = 0; ind < RegionCount; ind++) {
				if (!VisitedRegions[ind]) {
					len = reglen(ind);
					if (len < minlen) {
						minlen = len;
						newRegion = ind;
					}
				}
			}
			TempPath[SequencePathIndex].skp = 1;
			for (ind = 0; ind < PathMapIndex; ind++) {
				if (PathMap[ind].vrt == newRegion) {
					TempPath[SequencePathIndex++].pcon = ind;
					VisitedRegions[newRegion] = 1;
					DoneRegion = newRegion;
					return;
				}
			}
			TempPath[SequencePathIndex].cnt = VisitedIndex;
			TempPath[SequencePathIndex++].pcon = 0xffffffff;
			VisitedRegions[VisitedIndex] = 1;
			DoneRegion = VisitedIndex;
			return;
		}
	}
	for (ind = 0; ind < pathLength; ind++) {
		TempPath[SequencePathIndex].skp = 0;
		TempPath[SequencePathIndex++].pcon = RegionPath[ind].pcon;
		VisitedRegions[PathMap[RegionPath[ind].pcon].vrt] = 1;
	}
	DoneRegion = PathMap[RegionPath[ind - 1].pcon].vrt;
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

	ind = MapIndexSequence[SequencePath[pthi].vrt];
	nxtvrt = SequencePath[pthi + 1].vrt;
	while (ind < MapIndexSequence[SequencePath[pthi].vrt + 1] && PathMap[ind].vrt != nxtvrt) {
		ind++;
	}
	SequencePath[PathIndex++].grpn = PathMap[ind].grpn;
}

#define BUGSEQ 0

void lcon() {
	unsigned		iPath, ind, ine, l_bLine, cnt, sgrp;
	REGION*			trgns;
	short			tcon;
	RCON*			pcon;
	RCON*			tmap;
	SMALPNTL*		tpnt;
	unsigned*		tsrgns;
	unsigned		sind;

#if BUGSEQ

	unsigned		bugcol, iRegion;
#endif

	if (StitchLineCount) {
		SortedLines = new SMALPNTL*[StitchLineCount >> 1];
		SortedLineIndex = 0;
		for (ind = 0; ind < StitchLineCount; ind += 2)
			SortedLines[SortedLineIndex++] = &LineEndpoints[ind];
		qsort((void*)SortedLines, SortedLineIndex, 4, sqcomp);
		RegionCount = 0;
		trgns = (REGION*)OSequence;
		trgns[0].start = 0;
		l_bLine = SortedLines[0]->line;
		for (ind = 0; ind < SortedLineIndex; ind++) {
			if (l_bLine != SortedLines[ind]->line) {
				trgns[RegionCount++].end = ind - 1;
				trgns[RegionCount].start = ind;
				l_bLine = SortedLines[ind]->line;
			}
		}
		trgns[RegionCount++].end = ind - 1;
		RegionsList = new REGION[RegionCount];
		VisitedRegions = new char[RegionCount];
		for (ind = 0; ind < RegionCount; ind++) {
			RegionsList[ind].start = trgns[ind].start;
			RegionsList[ind].end = trgns[ind].end;
			VisitedRegions[ind] = 0;
			RegionsList[ind].cntbrk = 0;
		}
		tsrgns = (unsigned*)OSequence;
		sind = 0;
		for (ind = 0; ind < RegionCount; ind++) {
			cnt = 0;
			if ((RegionsList[ind].end - RegionsList[ind].start) > 1) {
				sgrp = SortedLines[RegionsList[ind].start]->group;
				for (ine = RegionsList[ind].start + 1; ine <= RegionsList[ind].end; ine++) {
					sgrp++;
					if (SortedLines[ine]->group != sgrp) {
						if (!cnt)
							RegionsList[ind].brk = sind;
						cnt++;
						sgrp = SortedLines[ine]->group;
						tsrgns[sind++] = ine;
					}
				}
			}
			RegionsList[ind].cntbrk = cnt;
		}
		tmap = (RCON*)BSequence;
		MapIndexSequence = new unsigned[RegionCount + 1];

#if BUGSEQ
		bugcol = 0; SequenceIndex = 0;
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			for (ine = RegionsList[iRegion].start; ine <= RegionsList[iRegion].end; ine++) {
				tpnt = &*SortedLines[ine];
				StitchBuffer[SequenceIndex].attribute = bugcol;
				StitchBuffer[SequenceIndex].x = tpnt[0].x;
				StitchBuffer[SequenceIndex++].y = tpnt[0].y;
				StitchBuffer[SequenceIndex].attribute = bugcol;
				StitchBuffer[SequenceIndex].x = tpnt[1].x;
				StitchBuffer[SequenceIndex++].y = tpnt[1].y;
			}
			bugcol++;
			bugcol &= 0xf;
		}
		PCSHeader.stitchCount = SequenceIndex;
		goto seqskip;
#endif
		OutputIndex = 0;
		if (RegionCount > 1) {
			ine = 0; PathMapIndex = 0;
			for (ind = 0; ind < RegionCount; ind++) {
				pcon = &tmap[PathMapIndex];
				MapIndexSequence[ind] = PathMapIndex;
				cnt = 0; GapToClosestRegion = 0;
				for (ine = 0; ine < RegionCount; ine++) {
					if (ind != ine) {
						tcon = regclos(ind, ine);
						if (tcon) {
							tmap[PathMapIndex].con = tcon;
							tmap[PathMapIndex].grpn = NextGroup;
							tmap[PathMapIndex++].vrt = ine;
							cnt++;
						}
					}
				}
				while (!cnt) {
					GapToClosestRegion += StitchSpacing;
					cnt = 0;
					for (ine = 0; ine < RegionCount; ine++) {
						if (ind != ine) {
							tcon = regclos(ind, ine);
							if (tcon) {
								tmap[PathMapIndex].con = tcon;
								tmap[PathMapIndex].grpn = NextGroup;
								tmap[PathMapIndex++].vrt = ine;
								cnt++;
							}
						}
					}
				}
			}
			MapIndexSequence[ind] = PathMapIndex;
			PathMap = new RCON[PathMapIndex + 1];
			for (ind = 0; ind < PathMapIndex; ind++) {
				PathMap[ind].con = tmap[ind].con;
				PathMap[ind].vrt = tmap[ind].vrt;
				PathMap[ind].grpn = tmap[ind].grpn;
			}
			//find the leftmost region
			sgrp = 0xffffffff; ine = 0;
			for (ind = 0; ind < RegionCount; ind++) {
				tpnt = &*SortedLines[RegionsList[ind].start];
				if (tpnt->group < sgrp) {
					sgrp = tpnt->group;
					ine = ind;
				}
			}
			OutputIndex = 0;
			TempPath = (RGSEQ*)OSequence;
			//find the leftmost region in PathMap
			SequencePathIndex = 1;
			for (ind = 0; ind < PathMapIndex; ind++) {
				if (PathMap[ind].vrt == ine)
					goto lconskip;
			}
			PathMap[PathMapIndex].vrt = ine;
			PathMap[PathMapIndex].grpn = 0;
			ind = PathMapIndex;
		lconskip:;
			//set the first entry in the temporary path to the leftmost region
			TempPath[0].pcon = ind;
			TempPath[0].cnt = 1;
			TempPath[0].skp = 0;
			VisitedRegions[ine] = 1;
			DoneRegion = ine;
			while (unvis())
				nxtrgn();
			ine = 0;
			cnt = 0xffffffff;
			SequencePath = (FSEQ*)&OSequence[OSEQLEN >> 1];
			for (ind = 0; ind < SequencePathIndex; ind++) {
				SequencePath[ine].skp = TempPath[ind].skp;
				if (TempPath[ind].pcon == 0xffffffff) {
					SequencePath[ine++].vrt = TempPath[ind].cnt;
					cnt = TempPath[ind].cnt;
				}
				else {
					if (TempPath[ind].pcon != cnt) {
						cnt = TempPath[ind].pcon;
						SequencePath[ine++].vrt = PathMap[TempPath[ind].pcon].vrt;
					}
				}
			}
			SequencePathIndex = ind; PathIndex = 0;
			for (ind = 0; ind < SequencePathIndex; ind++)
				nxtseq(ind);
			ine = (SortedLineIndex >> 5) + 1;
			SequenceFlagBitmap = new unsigned[ine];
			for (ind = 0; ind < ine; ind++)
				SequenceFlagBitmap[ind] = 0;
			for (ind = 0; ind < RegionCount; ind++)
				VisitedRegions[ind] = 0;
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
			ine = (SortedLineIndex >> 5) + 1;
			SequenceFlagBitmap = new unsigned[ine];
			for (ind = 0; ind < ine; ind++)
				SequenceFlagBitmap[ind] = 0;
			LastGroup = 0;
			SequencePath[0].vrt = 0;
			SequencePath[0].grpn = SortedLines[RegionsList[0].end]->group;
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

BOOL notin(unsigned ond) {
	dPOINT		pnt;
	unsigned	ind, cnt;
	dPOINT		ipnt;
	double		len;
	double		minot;		//minimum distance from a line
	unsigned	minotl;		//index to the minimum distance line

	cnt = 0; minot = 1e99;
	pnt.x = (OSequence[ond + 1].x - OSequence[ond].x) / 2 + OSequence[ond].x;
	pnt.y = (OSequence[ond + 1].y - OSequence[ond].y) / 2 + OSequence[ond].y;
	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
		if (projv(pnt.x, CurrentFillVertices[ind], CurrentFillVertices[ind + 1], &ipnt)) {
			if (ipnt.y > pnt.y)
				cnt++;
			len = fabs(ipnt.y - pnt.y);
			if (len < minot) {
				minot = len;
				minotl = ind;
			}
		}
	}
	if (projv(pnt.x, CurrentFillVertices[ind], CurrentFillVertices[0], &ipnt)) {
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

	for (SequenceIndex = 0; SequenceIndex < OutputIndex; SequenceIndex++)
	{
		OSequence[SequenceIndex].x = BSequence[SequenceIndex].x;
		OSequence[SequenceIndex].y = BSequence[SequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	int			cnt, rcnt;
	int			ind, rit;
	dPOINT		dif, pnt, stp;
	double		len, rslop;
	double		usesiz2 = UserStitchLength * 2;
	double		usesiz9 = UserStitchLength / 9;
	double		stspac2 = StitchSpacing * 2;

	SequenceIndex = 0;
	rstMap(FILDIR);
	ind = OutputIndex - 1;
	OSequence[SequenceIndex].x = BSequence[ind].x;
	OSequence[SequenceIndex].y = BSequence[ind].y;
	SequenceIndex++;
	SelectedPoint.x = BSequence[ind].x;
	SelectedPoint.y = BSequence[ind].y;
	ind--;
	while (ind > 0) {
		rcnt = ind%RITSIZ;
		if (SequenceIndex > MAXSEQ) {
			SequenceIndex = MAXSEQ - 1;
			return;
		}
		rit = BSequence[ind].x / stspac2;
		dif.x = BSequence[ind].x - BSequence[ind + 1].x;
		dif.y = BSequence[ind].y - BSequence[ind + 1].y;
		if (dif.y)
			rslop = dif.x / dif.y;
		else
			rslop = 1e99;
		switch (BSequence[ind].attribute) {
		case SEQTOP:

			if (SelectedForm->extendedAttribute&AT_SQR) {
				if (toglMap(FILDIR)) {
					OSequence[SequenceIndex].x = BSequence[ind - 1].x;
					OSequence[SequenceIndex++].y = BSequence[ind - 1].y;
					cnt = ceil(BSequence[ind].y / UserStitchLength);
				blntop:;
					OSequence[SequenceIndex].y = cnt*UserStitchLength + (rit%seqtab[rcnt])*usesiz9;
					if (OSequence[SequenceIndex].y > BSequence[ind].y)
						goto blntopx;
					dif.y = OSequence[SequenceIndex].y - BSequence[ind].y;
					OSequence[SequenceIndex++].x = BSequence[ind].x;
					cnt++;
					goto blntop;
				blntopx:;
					OSequence[SequenceIndex].x = BSequence[ind].x;
					OSequence[SequenceIndex++].y = BSequence[ind].y;
				}
				else {
					OSequence[SequenceIndex].x = BSequence[ind].x;
					OSequence[SequenceIndex++].y = BSequence[ind].y;
					cnt = floor(BSequence[ind].y / UserStitchLength);
				blntbot:;
					OSequence[SequenceIndex].y = cnt*UserStitchLength - ((rit + 2) % seqtab[rcnt])*usesiz9;
					if (OSequence[SequenceIndex].y < BSequence[ind - 1].y)
						goto blntbotx;
					dif.y = OSequence[SequenceIndex].y - BSequence[ind - 1].y;
					OSequence[SequenceIndex++].x = BSequence[ind].x;
					cnt--;
					goto blntbot;
				blntbotx:;
					OSequence[SequenceIndex].x = BSequence[ind - 1].x;
					OSequence[SequenceIndex++].y = BSequence[ind - 1].y;
				}
			}
			else {
				cnt = ceil(BSequence[ind + 1].y / UserStitchLength);
			toplab:;
				OSequence[SequenceIndex].y = cnt*UserStitchLength + (rit%seqtab[rcnt])*usesiz9;
				if (OSequence[SequenceIndex].y > BSequence[ind].y)
					goto toplabx;
				dif.y = OSequence[SequenceIndex].y - BSequence[ind + 1].y;
				dif.x = rslop*dif.y;
				OSequence[SequenceIndex++].x = BSequence[ind + 1].x + dif.x;
				cnt++;
				goto toplab;
			toplabx:;
				OSequence[SequenceIndex].x = BSequence[ind].x;
				OSequence[SequenceIndex++].y = BSequence[ind].y;
			}
			break;

		case SEQBOT:

			if (!(SelectedForm->extendedAttribute&AT_SQR)) {
				cnt = floor(BSequence[ind + 1].y / UserStitchLength);
			botlab:;
				OSequence[SequenceIndex].y = cnt*UserStitchLength - ((rit + 2) % seqtab[rcnt])*usesiz9;
				if (OSequence[SequenceIndex].y < BSequence[ind].y)
					goto botlabx;
				dif.y = OSequence[SequenceIndex].y - BSequence[ind + 1].y;
				dif.x = rslop*dif.y;
				OSequence[SequenceIndex++].x = BSequence[ind + 1].x + dif.x;
				cnt--;
				goto botlab;
			botlabx:;
				OSequence[SequenceIndex].x = BSequence[ind].x;
				OSequence[SequenceIndex++].y = BSequence[ind].y;
			}
			break;

		case 0:

			dif.x = BSequence[ind].x - BSequence[ind + 1].x;
			dif.y = BSequence[ind].y - BSequence[ind + 1].y;
			rstMap(FILDIR);
			len = hypot(dif.x, dif.y);
			if (len) {
				if (len > usesiz2) {
					pnt.x = BSequence[ind + 1].x;
					pnt.y = BSequence[ind + 1].y;
					cnt = len / UserStitchLength - 1;
					if (chkmax(cnt, SequenceIndex) || (cnt + SequenceIndex) > MAXSEQ - 3)
						return;
					stp.x = dif.x / cnt;
					stp.y = dif.y / cnt;
					while (cnt) {
						pnt.x += stp.x;
						pnt.y += stp.y;
						OSequence[SequenceIndex].x = pnt.x;
						OSequence[SequenceIndex++].y = pnt.y;
						cnt--;
					}
				}
			}
			OSequence[SequenceIndex].x = BSequence[ind].x;
			OSequence[SequenceIndex++].y = BSequence[ind].y;
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

	CurrentFillVertices = SelectedForm->vertices;
	VertexCount = SelectedForm->vertexCount;
	lox = hix = CurrentFillVertices[0].x;
	for (ind = 1; ind < VertexCount; ind++) {
		if (CurrentFillVertices[ind].x > hix)
			hix = CurrentFillVertices[ind].x;
		if (CurrentFillVertices[ind].x < lox)
			lox = CurrentFillVertices[ind].x;
	}
	tcnt = lox / StitchSpacing;
	lox = StitchSpacing*tcnt;
	cnt = (hix - lox) / StitchSpacing + 1;
	jpts = new dPOINTLINE[VertexCount + 2];
	pjpts = new dPOINTLINE*[VertexCount + 2];
	mstp = (hix - lox) / cnt;
	mx0 = lox;
	for (ind = 0; ind < cnt; ind++) {
		inf = 0;
		lox += mstp;
		for (ine = 0; ine < (unsigned)VertexCount - 1; ine++) {
			if (projv(lox, CurrentFillVertices[ine], CurrentFillVertices[ine + 1], &tpnt))
				inf++;
		}
		if (projv(lox, CurrentFillVertices[ine], CurrentFillVertices[0], &tpnt))
			inf++;
		lincnt += inf;
		if (inf > maxlins)
			maxlins = inf;
	}
	maxlins = (maxlins >> 1);
	LineEndpoints = new SMALPNTL[lincnt + 1];
	StitchLineCount = 0; LineGroupIndex = 0;
	tgrinds = (unsigned*)BSequence;
	GroupIndexCount = 0;
	for (ind = 0; ind < cnt; ind++) {
		mx0 += mstp;
		inf = 0;
		for (ine = 0; ine < (unsigned)VertexCount - 1; ine++) {
			if (projv(mx0, CurrentFillVertices[ine], CurrentFillVertices[ine + 1], &tpnt)) {
				pjpts[inf] = &jpts[inf];
				jpts[inf].line = ine;
				jpts[inf].x = tpnt.x;
				jpts[inf++].y = tpnt.y;

			}
		}
		if (projv(mx0, CurrentFillVertices[ine], CurrentFillVertices[0], &tpnt)) {
			pjpts[inf] = &jpts[inf];
			jpts[inf].line = ine;
			jpts[inf].x = tpnt.x;
			jpts[inf++].y = tpnt.y;
		}
		if (inf > 1) {
			inf &= 0xfffffffe;
			tgrinds[GroupIndexCount++] = StitchLineCount;
			qsort((void*)pjpts, inf, 4, comp);
			ine = 0;
			tind = StitchLineCount;
			while (ine < inf) {
				if (StitchLineCount < lincnt) {
					LineEndpoints[StitchLineCount].line = pjpts[ine]->line;
					LineEndpoints[StitchLineCount].group = LineGroupIndex;
					LineEndpoints[StitchLineCount].x = pjpts[ine]->x;
					LineEndpoints[StitchLineCount++].y = pjpts[ine++]->y;
					LineEndpoints[StitchLineCount].line = pjpts[ine]->line;
					LineEndpoints[StitchLineCount].group = LineGroupIndex;
					LineEndpoints[StitchLineCount].x = pjpts[ine]->x;
					LineEndpoints[StitchLineCount++].y = pjpts[ine++]->y;
				}
			}
			if (StitchLineCount != tind)
				LineGroupIndex++;
		}
	}
	tgrinds[GroupIndexCount++] = StitchLineCount;
	GroupIndexSequence = new unsigned[GroupIndexCount];
	for (ind = 0; ind < GroupIndexCount; ind++)
		GroupIndexSequence[ind] = tgrinds[ind];
	LineGroupIndex--;
	delete[] jpts;
	delete[] pjpts;
}

void fnhor() {
	unsigned	ind;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	RotationAngle = PI / 2;
	AngledForm.vertices = AngledFormVertices;
	for (ind = 0; ind < AngledForm.vertexCount; ind++) {
		AngledForm.vertices[ind].x = SelectedForm->vertices[ind].x;
		AngledForm.vertices[ind].y = SelectedForm->vertices[ind].y;
		rotflt(&AngledForm.vertices[ind]);
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
	unsigned ind;;

	if (filmsgs(FMM_VRT))
		return;
	if (SelectedFormCount) {
		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
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
	unsigned ind;

	if (filmsgs(FMM_HOR))
		return;
	if (SelectedFormCount) {
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
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
	double bang;

	bang = RotationAngle;
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
	RotationAngle = bang;
}

void filangl() {
	unsigned ind;

	if (filmsgs(FMM_ANG))
		return;
	RotationAngle = IniFile.fillAngle;
	if (SelectedFormCount) {
		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++) {
			ClosestFormToCursor = SelectedFormList[ind];
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

	sfCor2px(FormList[ClosestFormToCursor].vertices[0], &tpnt);
	tof.x = Msg.pt.x - StitchWindowOrigin.x - tpnt.x + FormMoveDelta.x;
	tof.y = Msg.pt.y - StitchWindowOrigin.y - tpnt.y + FormMoveDelta.y;
	for (ind = 0; ind < FormList[ClosestFormToCursor].vertexCount; ind++) {
		sfCor2px(FormList[ClosestFormToCursor].vertices[ind], &tpnt);
		FormLines[ind].x = tpnt.x + tof.x;
		FormLines[ind].y = tpnt.y + tof.y;
	}
	FormLines[ind].x = FormLines[0].x;
	FormLines[ind].y = FormLines[0].y;
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

	fvars(ClosestFormToCursor);
	tpnt0.x = Msg.pt.x - StitchWindowOrigin.x;
	tpnt0.y = Msg.pt.y - StitchWindowOrigin.y;
	NewFormVertexCount = SelectedForm->vertexCount + 1;
	duzrat();
	sRct2px(SelectedForm->rectangle, &trct);
	FormOutlineRect[0].x = FormOutlineRect[6].x = FormOutlineRect[7].x = FormOutlineRect[8].x = trct.left;
	FormOutlineRect[1].x = FormOutlineRect[5].x = midl(trct.right, trct.left);
	FormOutlineRect[0].y = FormOutlineRect[1].y = FormOutlineRect[2].y = FormOutlineRect[8].y = trct.top;
	FormOutlineRect[3].y = FormOutlineRect[7].y = midl(trct.top, trct.bottom);
	FormOutlineRect[4].y = FormOutlineRect[5].y = FormOutlineRect[6].y = trct.bottom;
	FormOutlineRect[2].x = FormOutlineRect[3].x = FormOutlineRect[4].x = trct.right;
	for (ind = 0; ind < 10; ind++) {
		len = hypot(FormOutlineRect[ind].x - tpnt0.x, FormOutlineRect[ind].y - tpnt0.y);
		if (len < mlen) {
			mlen = len;
			SelectedFormControlVertex = ind;
		}
		if (mlen < CLOSENUF) {
			ritfrct(ClosestFormToCursor, StitchWindowDC);
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
				XYratio = (double)(SelectedForm->rectangle.right - SelectedForm->rectangle.left) / (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom);
			}
			SelectedFormControlVertex >>= 1;
			setMap(SHOSTRTCH);
			strtchbox();
			return 1;
		}
	}
	if (tpnt0.x >= trct.left&&tpnt0.x <= trct.right&&
		tpnt0.y >= trct.top&&tpnt0.y <= trct.bottom) {
		sfCor2px(SelectedForm->vertices[0], &tpnt1);
		FormMoveDelta.x = tpnt1.x - tpnt0.x;
		FormMoveDelta.y = tpnt1.y - tpnt0.y;
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
	unsigned	tat = (ClosestFormToCursor << 4);

	setmfrm();
	rstMap(FRMOV);
	tpnt.x = Msg.pt.x + FormMoveDelta.x;
	tpnt.y = Msg.pt.y + FormMoveDelta.y;
	pxCor2stch(tpnt);
	pof.x = SelectedPoint.x - SelectedForm->vertices[0].x;
	pof.y = SelectedPoint.y - SelectedForm->vertices[0].y;
	for (ind = 0; ind < SelectedForm->vertexCount; ind++) {
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

	for (ind = 0; ind < FormIndex; ind++) {
		FormList[ind].clipEntries = 0;
		FormList[ind].lengthOrCount.clipCount = 0;
		FormList[ind].edgeType = 0;
		FormList[ind].fillType = 0;
		FormList[ind].attribute &= NFRECONT;
		FormList[ind].extendedAttribute &= !(AT_UND | AT_WALK);
	}
	ClipPointIndex = 0;
}

void dusat() {
	POINT* l_plin = &FormLines[SatinIndex - 1];

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
	unsigned ind;

	if (SatinIndex > 1) {
		FormList[FormIndex].vertices = adflt(SatinIndex);
		for (ind = 0; ind < SatinIndex; ind++) {
			FormList[FormIndex].vertices[ind].x = TempPolygon[ind].x;
			FormList[FormIndex].vertices[ind].y = TempPolygon[ind].y;
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

void delcon(unsigned p_cpnt)
{
	unsigned ind;
	unsigned loc;
	SATCON* tp;
	FRMHED* fp;

	tp = &SelectedForm->satinOrAngle.sac[p_cpnt];
	loc = &SelectedForm->satinOrAngle.sac[p_cpnt] - SatinConnects;
	if (SatinConnectIndex > loc)
		MoveMemory(tp, &tp[1], (SatinConnectIndex - loc + 1) * sizeof(SATCON));
	for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
	{
		fp = &FormList[ind];
		if (fp->type == SAT&&fp->satinGuideCount)
			fp->satinOrAngle.sac--;
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
	unsigned ind, ine, playcod;
	double		tlen, len = 1e99;
	double		dx, dy;

	px2stch();
	for (ind = 0; ind < FormIndex; ind++) {
		playcod = (FormList[ind].attribute&FRMLMSK) >> 1;
		if (!ActiveLayer || !playcod || playcod == ActiveLayer) {
			for (ine = 0; ine < FormList[ind].vertexCount; ine++) {
				dx = SelectedPoint.x - FormList[ind].vertices[ine].x;
				dy = SelectedPoint.y - FormList[ind].vertices[ine].y;
				tlen = hypot(dx, dy);
				if (tlen < len) {
					len = tlen;
					ClosestFormToCursor = ind;
					ClosestVertexToCursor = ine;
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

unsigned setchk(unsigned bPnt) {
#if	 __UseASM__
	_asm {
		xor		eax, eax
		mov		ebx, CheckMap
		mov		ecx, bPnt
		bts[ebx], ecx
		jnc		short setcx
		dec		eax
		setcx :
	}
#else
	return _bittestandset((long *)CheckMap, bPnt) ? 0xFFFFFFFF : 0;
#endif
}

unsigned chkchk(unsigned bit) {
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
	return _bittest((long *)chkMap, bit) ? 0xFFFFFFFF : 0;
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
	unsigned	ind, ine, inf, psac, mapsiz, prstpt;
	SATCON*		l_spnt;
	SATCON*		l_dpnt;
	unsigned short bstpt;
	FRMHED*		fp;

	fvars(ClosestFormToCursor);
	bstpt = SelectedForm->satinGuideCount;
	for (ind = 0; ind < SelectedForm->satinGuideCount; ind++)
	{
		if (CurrentFormConnections[ind].finish > VertexCount - 1)
			CurrentFormConnections[ind].finish = VertexCount - 1;
		if (CurrentFormConnections[ind].start > VertexCount - 1)
			CurrentFormConnections[ind].start = VertexCount - 1;
	}
	l_spnt = (SATCON*)BSequence;
	mapsiz = (VertexCount >> 5) + 1;
	CheckMap = (unsigned*)OSequence;
	psac = 0;
	prstpt = CurrentFormConnectionsCount;
	for (ind = 0; ind < CurrentFormConnectionsCount; ind++)
	{
		if (CurrentFormConnections[ind].start != CurrentFormConnections[ind].finish)
		{
			CurrentFormConnections[psac].start = CurrentFormConnections[ind].start;
			CurrentFormConnections[psac].finish = CurrentFormConnections[ind].finish;
			psac++;
		}
	}
	CurrentFormConnectionsCount = SelectedForm->satinGuideCount = ind;
	if (WordParam || SelectedForm->attribute&FRMEND)
	{
		for (ind = 0; ind < mapsiz; ind++)
			CheckMap[ind] = 0;
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
		ine = 0;
		for (ind = 0; ind < CurrentFormConnectionsCount; ind++)
		{
			if (chkchk(CurrentFormConnections[ind].start) && chkchk(CurrentFormConnections[ind].finish))
			{
				l_spnt[ine].start = CurrentFormConnections[ind].start;
				l_spnt[ine].finish = CurrentFormConnections[ind].finish;
				ine++;
			}
		}
		CurrentFormConnectionsCount = SelectedForm->satinGuideCount = ine;
		if (WordParam)
		{
			ine = 0;
			for (ind = 0; ind < CurrentFormConnectionsCount; ind++)
			{
				if (CurrentFormConnections[ind].start < WordParam)
				{
					l_spnt[ine].start = CurrentFormConnections[ind].start;
					l_spnt[ine++].finish = CurrentFormConnections[ind].finish;
				}
			}
			CurrentFormConnectionsCount = SelectedForm->satinGuideCount = ine;
		}
	}
	else
	{
		for (ind = 0; ind < CurrentFormConnectionsCount; ind++)
		{
			l_spnt[ind].start = CurrentFormConnections[ind].start;
			l_spnt[ind].finish = CurrentFormConnections[ind].finish;
		}
	}
	if (CurrentFormConnectionsCount)
	{
		for (ind = 0; ind < mapsiz; ind++)
			CheckMap[ind] = 0;
		for (ind = 0; ind < CurrentFormConnectionsCount; ind++)
		{
			ine = CurrentFormConnections[ind].start;
			if (ine > (unsigned)WordParam - 1)
				ine = WordParam - 1;
			if (setchk(ine))
			{
				inf = ine;
				if (inf)
					inf--;
				while (!chkchk(ine) && ine < (unsigned)WordParam - 1)
					ine++;
				while (inf && (!chkchk(inf)))
					inf--;
				if (!chkchk(ine) && !chkchk(inf))
					break;
				if (chkchk(ine) && chkchk(inf))
				{
					if (ine - CurrentFormConnections[ind].start > CurrentFormConnections[ind].start - inf)
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
				if (ine < VertexCount)
					CurrentFormConnections[inf++].start = ine + (ind << 5);
			} while (ine < VertexCount);
		}
		CurrentFormConnectionsCount = SelectedForm->satinGuideCount = inf;
		for (ind = 0; ind < mapsiz; ind++)
			CheckMap[ind] = 0;
		for (ind = 0; ind < CurrentFormConnectionsCount; ind++)
		{
			ine = inf = CurrentFormConnections[ind].finish;
			if (ine > (unsigned)VertexCount - 1)
				ine = VertexCount - 1;
			if (setchk(ine))
			{
				if (ine < (unsigned)VertexCount - 1)
					ine++;
				if (inf > (unsigned)WordParam + 1)
					inf--;
				while (!chkchk(ine) && ine < (unsigned)VertexCount - 1)
					ine++;
				while (inf > (unsigned)WordParam - 1 && (!chkchk(inf)))
					inf--;
				if (!chkchk(ine) && !chkchk(inf))
					break;
				if (chkchk(ine) && chkchk(inf))
				{
					if (ine - CurrentFormConnections[ind].finish > CurrentFormConnections[ind].finish - inf)
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
				if (ine < VertexCount)
					CurrentFormConnections[inf++].finish = ine + ((ind - 1) << 5);
			} while (ine < VertexCount);
		}
		if (inf < CurrentFormConnectionsCount)
			inf = CurrentFormConnectionsCount;
		CurrentFormConnectionsCount = SelectedForm->satinGuideCount = inf;
		if (WordParam)
		{
			if (CurrentFormConnectionsCount > VertexCount - WordParam - 2)
				CurrentFormConnectionsCount = VertexCount - WordParam - 2;
			if (CurrentFormConnectionsCount > WordParam - 2)
				CurrentFormConnectionsCount = WordParam - 2;
			SelectedForm->satinGuideCount = CurrentFormConnectionsCount;
		}
	}
	if (SelectedForm->satinGuideCount < bstpt)
	{
		ine = bstpt - CurrentFormConnectionsCount;
		l_spnt = l_dpnt = SelectedForm->satinOrAngle.sac;
		l_dpnt += SelectedForm->satinGuideCount;
		l_spnt += bstpt;
		MoveMemory(l_dpnt, l_spnt, sizeof(SATCON)*(&SatinConnects[SatinConnectIndex] - l_spnt + 1));
		for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
		{
			fp = &FormList[ind];
			if (fp->type == SAT)
				fp->satinOrAngle.sac -= ine;
		}
		SatinConnectIndex -= ine;
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
		SelectedForm->angleOrClipData.sat.start = StartPoint;
		SelectedForm->angleOrClipData.sat.finish = clos;
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
				sacspac(&SelectedForm->satinOrAngle.sac[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.sac[SelectedForm->satinGuideCount].start = clos;
				SelectedForm->satinOrAngle.sac[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satadj();
			}
			else {
				SelectedForm->satinOrAngle.sac = nusac(ClosestFormToCursor, 1);
				SelectedForm->satinOrAngle.sac[psac].start = clos;
				SelectedForm->satinOrAngle.sac[psac].finish = ClosestVertexToCursor;
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
	satfn(ind, CurrentFormConnections[0].start, VertexCount, CurrentFormConnections[0].finish);
	for (ind = 0; ind < (unsigned)CurrentFormConnectionsCount - 1; ind++)
		satfn(CurrentFormConnections[ind].start, CurrentFormConnections[ind + 1].start, CurrentFormConnections[ind].finish, CurrentFormConnections[ind + 1].finish);
	if (WordParam)
		satfn(CurrentFormConnections[ind].start, WordParam, CurrentFormConnections[ind].finish, WordParam + 1);
	else {
		if (CurrentFormConnections[ind].finish - CurrentFormConnections[ind].start > 2) {
			len = (Lengths[CurrentFormConnections[ind].finish] - Lengths[CurrentFormConnections[ind].start]) / 2 + Lengths[CurrentFormConnections[ind].start];
			ine = CurrentFormConnections[ind].start;
			while (len > Lengths[ine])
				ine++;
			dx = Lengths[ine] - len;
			dy = len - Lengths[ine - 1];
			if (dy > dx)
				ine--;
			satfn(CurrentFormConnections[ind].start, ine, CurrentFormConnections[ind].finish, ine);
		}
		else
			satfn(CurrentFormConnections[ind].start, CurrentFormConnections[ind].start + 1, CurrentFormConnections[ind].finish, CurrentFormConnections[ind].start + 1);
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
		if (FormForInsert->satinOrAngle.sac[ind].start > pnu - 1)
			FormForInsert->satinOrAngle.sac[ind].start++;
		if (FormForInsert->satinOrAngle.sac[ind].finish > pnu - 1)
			FormForInsert->satinOrAngle.sac[ind].finish++;
	}
	if (FormForInsert->wordParam >= pnu) {
		FormForInsert->wordParam++;
		FormForInsert->wordParam %= VertexCount;
	}
	if (FormForInsert->fillType == CONTF) {
		if (FormForInsert->angleOrClipData.sat.start > pnu - 1)
			FormForInsert->angleOrClipData.sat.start++;
		if (FormForInsert->angleOrClipData.sat.finish > pnu - 1)
			FormForInsert->angleOrClipData.sat.finish++;
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
			l_sac = SelectedForm->satinOrAngle.sac;
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
						fp->satinOrAngle.sac++;
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
			if (CurrentFormConnections[ind].start != nu0&&CurrentFormConnections[ind].finish != nu0) {
				CurrentFormConnections[ine].start = (CurrentFormConnections[ind].start + VertexCount - nu0) % VertexCount;
				CurrentFormConnections[ine].finish = (CurrentFormConnections[ind].finish + VertexCount - nu0) % VertexCount;
				if (CurrentFormConnections[ine].start > CurrentFormConnections[ine].finish) {
					tlin = CurrentFormConnections[ine].start;
					CurrentFormConnections[ine].start = CurrentFormConnections[ine].finish;
					CurrentFormConnections[ind].finish = tlin;
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
		nsac[ind].start = CurrentFormConnections[ind].start;
		nsac[ind].finish = CurrentFormConnections[ind].finish;
	}
	qsort((void*)psac, ine, 4, scomp);
	for (ind = 0; ind < ine; ind++) {
		CurrentFormConnections[ind].start = psac[ind]->start;
		CurrentFormConnections[ind].finish = psac[ind]->finish;
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
		nupnt->satinOrAngle.sac = adsatk(nupnt->satinGuideCount);
		mvsatk(nupnt->satinOrAngle.sac, SelectedForm->satinOrAngle.sac, SelectedForm->satinGuideCount);
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
		SelectedForm->satinOrAngle.sac = adsatk(SelectedForm->satinGuideCount);
		MoveMemory(SelectedForm->satinOrAngle.sac, l_spnt->satinOrAngle.sac, SelectedForm->satinGuideCount * sizeof(SATCON));
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
	strt = SelectedForm->angleOrClipData.sat.start;
	fin = SelectedForm->angleOrClipData.sat.finish;
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
				tfrm->satinOrAngle.sac[ind].finish = tfrm->vertexCount - ind - 1;
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
		mvsatk(&TempSatinConnects[SatinConnectIndex], dst->satinOrAngle.sac, dst->satinGuideCount);
		dst->satinOrAngle.sac = &SatinConnects[SatinConnectIndex];
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
		SelectedForm->satinOrAngle.sac[ind].finish += lo;
	if (SelectedForm->wordParam)
		SelectedForm->wordParam = tsac.start;
	lo = ind + 1;
	while (ind < (unsigned)SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.sac[ind].start -= (tsac.start - 1);
		SelectedForm->satinOrAngle.sac[ind].finish -= (tsac.start - 1);
		ind++;
	}
	if (FormList[ClosestFormToCursor + 1].wordParam)
		FormList[ClosestFormToCursor + 1].wordParam -= (tsac.start - 1);
	mvsatk(&SelectedForm->satinOrAngle.sac[lo - 1], &SelectedForm->satinOrAngle.sac[lo], SatinConnectIndex - sacind(&SelectedForm->satinOrAngle.sac[lo]));
	SatinConnectIndex--;
	FormList[ClosestFormToCursor + 1].satinOrAngle.sac = &SelectedForm->satinOrAngle.sac[ActivePointIndex];
	FormList[ClosestFormToCursor + 1].satinGuideCount = SelectedForm->satinGuideCount - ActivePointIndex - 1;
	SelectedForm->satinGuideCount = ActivePointIndex;
	for (ind = ClosestFormToCursor + 2; ind < FormIndex; ind++)
		FormList[ind].satinOrAngle.sac--;
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
					if (SelectedForm->satinOrAngle.sac[ActivePointIndex].start == ClosestVertexToCursor || SelectedForm->satinOrAngle.sac[ActivePointIndex].finish == ClosestVertexToCursor) {
						spltsat(SelectedForm->satinOrAngle.sac[ActivePointIndex]);
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

