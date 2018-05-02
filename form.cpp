#ifndef NOMINMAX
# define NOMINMAX
#endif

#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <CppCoreCheck\warnings.h>
#pragma warning( push )  
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#include <fmt/format.h>
#pragma warning( pop )  

#include "lang.h"
#include "resource.h"
#include "thred.h"
#include "ExtendedBitSet.h"

extern fPOINT*	adclp (unsigned count);
extern fPOINT*	adflt (unsigned count);
extern SATCON*	adsatk (unsigned count);
extern void		alrotmsg ();
extern void		angrct (fRECTANGLE* rectangle);
extern void		centr ();
extern void		chkcwlk ();
extern void		chkrng (fPOINT* range);
extern void		chkund(std::vector<RNGCNT> &textureSegments);
extern void		chkwlk ();
extern void		coltab ();
extern void		dasyfrm ();
extern void		delfstchs ();
extern void		delinf ();
extern void		delstchm ();
extern void		deltx ();
extern unsigned	duthrsh (double threshold);
extern void		duzrat ();
extern void		fdelstch (FILLSTARTS &FillStartsData);
extern bool		filmsgs (unsigned code);
extern void		fritfil ();
extern void		frm1pnt ();
extern void		frmdel ();
extern void		fthrfn ();
extern void		grpAdj ();
extern void		insadj ();
extern fPOINT*	insid ();
extern void		intlv (const FILLSTARTS &FillStartsData);
extern bool		isclp (unsigned find);
extern bool		isclpx (unsigned find);
extern bool		isfclp ();
extern bool		istx (unsigned find);
extern	inline void		loadString(std::string & sDest, unsigned stringID);
extern void		movStch ();
extern void		mvflpnt (fPOINT* destination, const fPOINT* source, unsigned count);
extern void		mvsatk (SATCON* destination, const SATCON* source, unsigned count);
extern void		mvstch (unsigned destination, unsigned source);
extern void		mvstchs (unsigned destination, unsigned source, unsigned count);
extern void		numWnd ();
extern void		nuRct ();
extern void		pntmsg (unsigned count);
extern unsigned	px2stch ();
extern void		pxCor2stch (POINT point);
extern void		redclp ();
extern void		redraw (HWND window);
extern void		ritfcor (const fPOINT* point);
extern void		ritmov ();
extern void		ritnum (unsigned code, unsigned value);
extern void		ritot (unsigned number);
extern void		rngadj ();
extern void		rotang1 (fPOINTATTR unrotatedPoint, fPOINT* rotatedPoint);
extern void		rotangf (fPOINT unrotatedPoint, fPOINT* rotatedPoint);
extern void		rotflt (fPOINT* point);
extern void		rotfn ();
extern void		rstAll ();
extern void		savdo ();
extern void		sCor2px (dPOINT stitchCoordinate, POINT* pixelCoordinate);
extern void		sdmsg ();
extern void		selRct (fRECTANGLE* sourceRect);
extern void		setangf (double angle);
extern void		setfchk ();
extern void		setpsel ();
extern unsigned	setRmap (fPOINTATTR stitchPoint);
extern void		setxt(std::vector<RNGCNT> &textureSegments);
extern void		shft (fPOINT delta);
extern void		shoMsg (const std::string &message);
extern void		shord ();
extern void		shoseln (unsigned code0, unsigned code1);
extern void		spltmsg ();
extern void		srtcol ();
extern void		stch2pxr (fPOINT stitchCoordinate);
extern void		tabmsg (unsigned code);
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
extern			unsigned		GroupEndStitch;
extern			unsigned		GroupStartStitch;
extern			unsigned		GroupStitchIndex;
extern			INIFILE			IniFile;
extern			POINT			InsertLine[3];
extern			fPOINT			InterleaveSequence[MAXITEMS];
extern			unsigned		InterleaveSequenceIndex;
extern			unsigned		InterleaveSequenceIndex2;
extern			INSREC			InterleaveSequenceIndices[10];
extern			HPEN			LayerPen[5];
extern			HMENU			MainMenu;
extern			HWND			MainStitchWin;
extern			double			MinStitchLength;
extern			MSG				Msg;
extern			char			MsgBuffer[MSGSIZ];
extern			unsigned		MsgIndex;
extern			RECT			MsgRect;
extern			HWND			MsgWindow;
extern			HPEN			MultiFormPen;
extern			unsigned		NearestCount;
extern			HWND			OKButton;
extern			char*			PcdClipFormat;
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
extern			char			SideWindowEntryBuffer[11];
extern			double			SmallStitchLength;
extern			EnumMap<StateFlag>	StateMap;
extern			double			StitchBoxesThreshold;
extern			fPOINTATTR		StitchBuffer[MAXITEMS * 2];
extern			fRECTANGLE		StitchRangeRect;
extern			POINT			StitchCoordinatesPixels;
extern			RECT			StitchWindowClientRect;
extern			HDC				StitchWindowDC;
extern			HDC				StitchWindowMemDC;
extern			POINT			StitchWindowOrigin;
extern			POINT			StitchWindowSize;
extern			POINT			StretchBoxLine[5];
extern			std::vector<std::string> *StringTable;
extern			int				TextureIndex;
extern			TXPNT			TexturePointsBuffer[MAXITEMS];
extern			HINSTANCE		ThrEdInstance;
extern			HWND			ThrEdWindow;
extern			POINT			ThredWindowOrigin;
extern			char			ThrName[_MAX_PATH];
extern			unsigned		AppliqueColor;
extern			POINT			UnzoomedRect;
extern			EnumMap<UserFlag>	UserFlagMap;
extern			HPEN			UserPen[16];
extern			double			UserStitchLength;
extern			double			ZoomFactor;
extern			dPOINT			ZoomMarkPoint;
extern			dPOINT			ZoomRatio;
extern			dRECTANGLE		ZoomRect;

void			angclpfn(std::vector<RNGCNT> &textureSegments);
void			angsclp ();
void			apbrd ();
void			bakseq ();
void			bdrlin(unsigned start, unsigned finish, double stitchSize);
void			bhbrd(double spacing);
void			blbrd (double spacing);
void			bold (double size) noexcept;
void			brdfil (double pd_Size);
void			chan ();
void			chnfn ();
bool			cisin(float xCoordinate, float yCoordinate) noexcept;
void			clpbrd(unsigned short startVertex);
void			clpcon(std::vector<RNGCNT> &textureSegments);
void			clpic();
void			clpout ();
void			clpxadj(std::vector<fPOINT> &tempClipPoints, std::vector<fPOINT> &chainEndPoints) noexcept;
void			contf ();
void			deleclp (unsigned iForm);
void			delmclp (unsigned iForm);
void			duangs () noexcept;
void			dubfn ();
void			dufcntr (dPOINT* center) noexcept;
void			dulast (std::vector<fPOINT> &chainEndPoints) noexcept;
void			dufxlen (std::vector<fPOINT> &chainEndPoints);
void			duhart (unsigned sideCount);
void			durpoli (unsigned vertexCount);
void			duxclp ();
void			filinsb (dPOINT point) noexcept;
void			filvrt ();
void			flipv ();
void			fmclp ();
void			frmpnts (unsigned typ) noexcept;
void			frmsqr (unsigned iVertex);
void			fvars (unsigned iForm) noexcept;
void			horclpfn(std::vector<RNGCNT> &textureSegments);
void			horsclp ();
bool			iseclp (unsigned find);
bool			iseclpx (unsigned find);
void			lapbrd ();
void			lcon(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints);
constexpr unsigned	nxt (unsigned int iVertex);
void			oclp (const fPOINT* clip, unsigned clipEntries);
void			pbrd (double spac);
void			plbrd (double edgeSpacing);
void			prpbrd (double borderStitchSpacing);
constexpr unsigned	prv (unsigned iVertex);
unsigned		psg () noexcept;
void			rotfrm (unsigned newStartVertex);
void			satfil ();
void			satout (double satinWidth);
void			sbrd ();
void			selal ();
void			setr (unsigned pbit) noexcept;
void			slbrd ();
void			sRct2px (fRECTANGLE stitchRect, RECT* screenRect) noexcept;
void			uncon ();
void			uninsf ();
void		wavfrm ();

unsigned		FormMenuEntryCount;		//lines in the form-form
float			MaxStitchLen;			//maximum stitch length
float			UserStitchLen;			//user stitch length
double			AdjustedSpace;			//adjusted space
unsigned		NextStart;				//index of the endpoint of the line segment being processed
unsigned		ChainSequence[] = { 0,1,2,3,0,1,4,3,0,3 };//chain stitch sequence
double			Div4;					//chain space divided by four
unsigned		ClosestFormToCursor;	//closest form to the cursor
unsigned		ClosestVertexToCursor;	//formOrigin closest to the cursor
unsigned		InOutFlag;				//is intersection of line and cursor before, in or after the line
double			VerticalRatio;			//vertical ratio between the zoom window
										// and the entire stitch space
double			HorizontalRatio;		//horizontal ratio between the zoom window
										// and the entire stitch space
POINT			FormLines[MAXFRMLINS];	//used in the form drawing routines
unsigned		NewFormVertexCount;		//points in the new form
FORMINFO		FormInfo;				//form info used in drawing forms
FRMHED			FormList[MAXFORMS];		//a list of form headers
unsigned		FormIndex = 0;			//index into the list of forms
double			LineSpacing = DEFSPACE * PFGRAN;//stitch spacing in stitch units
fPOINT*			CurrentFillVertices;	//pointer to the line of the polygon being filled
unsigned		StitchLineCount;		//count of stitch lines
unsigned		ActivePointIndex;		//pointer to the active form in the sequencing algorithm
unsigned		LineGroupIndex;			//pointer for groups of fill line segments
unsigned		VertexCount;			//sides of the selected form to fill
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
SATCON*			CurrentFormGuides;		//connections in the currently selecteed form
unsigned short	CurrentFormGuidesCount;	//number of connections in the currently selected form
unsigned short	SatinEndGuide;			//satin end guide for the currently selected form
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
POINT			RubberBandLine[3];		//points to form points to be moved
double			SnapLength = SNPLEN * PFGRAN;	//snap together length
double			StarRatio = STARAT;		//star formOrigin to body ratio
double			SpiralWrap = SPIRWRAP;	//number of revolutions in a spiral
fRECTANGLE		BoundingRect;			//isin rectangle
unsigned		PathMapIndex;			//number of entries in the path map
unsigned		VisitedIndex;			//next unvisited region for sequencing
unsigned		RegionCount;			//number of regions to be sequenced
unsigned		DoneRegion;				//last region sequenced
double			GapToClosestRegion;		//region close enough threshold for sequencing
RGSEQ*			RegionPath;				//path to a region
unsigned		GroupIndexCount;		//number of group indices
unsigned		LastGroup;				//group of the last line written in the previous region;
unsigned		SequencePathIndex;		//index to path of sequenced regions
unsigned		NextGroup;				//group that connects to the next region
REGION*			CurrentRegion;			//region currently being sequenced
FRMHED			AngledForm;				//a temporary rotated form for angle fills
unsigned short	SelectedFormList[MAXFORMS];	//a list of selected forms
unsigned		SelectedFormCount = 0;	//number of selected forms
unsigned		PreviousFormIndex;		//previously selected form
RECT			SelectedFormsRect;		//for multiple selections;
POINT			SelectedFormsLine[9];	//line derived from the big rectangle
POINT			SelectedPointsLine[9];	//line derived from the formOrigin select rectangle
fRECTANGLE		AllItemsRect;			//rectangle enclosing all forms and stitches
double			FormAngles[MAXFRMLINS];	//angles of a form for satin border fills
fPOINT			FormVertices[MAXITEMS];	//form points
unsigned		FormVertexIndex;		//next index to append form points
fPOINT			ClipPoints[MAXITEMS];	//main clipboard fill points for forms
unsigned		ClipPointIndex;			//next index to append main clipboard points
SATCON			SatinGuides[MAXSAC];	//satin form connects
unsigned		SatinGuideIndex;		//next index to append satin connect points
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
unsigned		FormRelocationIndex;	//form relocator pointer
unsigned		BeanCount;				//number of stitches added by convert to bean
FRMHED*			FormForInsert;			//insert form vertex in this form
unsigned		FormVertexPrev;			//form vertex storage for form vertex insert
unsigned		FormVertexNext;			//form vertex storage for form vertex insert
SMALPNTL*		SequenceLines;			//line for vertical/horizontal/angle fills
unsigned		PathIndex;				//formOrigin to the next path element for vertical fill sequencing
fPOINT*			WorkingFormVertices;	//form points for angle fills
fPOINT			AngledFormVertices[MAXFRMLINS];	//form formOrigin data for angle fills
unsigned short	ClipIntersectSide;		//clipboard intersect side;
fPOINT			LineSegmentStart;		//vertical clipboard line segment start
fPOINT			LineSegmentEnd;			//vertical clipboard line segment end
float			ClipWidth;				//horizontal spacing for vertical clipboard fill
HWND			TimeWindow;				//progress bar
HDC				TimeDC;					//progress bar device context
double			TimeStep;				//progress bar step
double			TimePosition;			//progress bar postiion
FLOAT			FormOffset;				//form offset for clipboard fills
long			PreferenceWindowWidth;	//width of the preference window
double			EggRatio;				//ratio for shrinking eggs
unsigned		PreferenceWindowTextWidth;	//size of the text part of the preference window
unsigned		MarkedStitchMap[MAXITEMS];	//bitmap to tell when stitches have been marked

char		FormOnOff[16];

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


void frmclr(FRMHED* destination) noexcept {
	memset(destination, 0, sizeof(FRMHED));
}

void duinf(const FRMHED* formHeader) noexcept {
	//Correct
	FormInfo.type = formHeader->attribute & 0xf;
	FormInfo.attribute = (formHeader->attribute >> 4) & 0xf;
	FormInfo.sideCount = formHeader->vertexCount;
}

bool comp(const dPOINTLINE point1, const dPOINTLINE point2) noexcept {
	if (point1.y < point2.y)
		return true;
	if (point1.x < point2.x)
		return true;
	return false;
}

void getfinfo(unsigned iForm) noexcept {
	duinf(&FormList[iForm]);
}

unsigned satind(const SATCON* guide) noexcept {
	return guide - SatinGuides;
}

void dusqr() {
	if (UserFlagMap.test(UserFlag::SQRFIL))
		SelectedForm->extendedAttribute |= AT_SQR;
	else
		SelectedForm->extendedAttribute &= (~AT_SQR);
}

void sacspac(const SATCON* startGuide, unsigned guideCount) noexcept {
	int			iStartGuide = 0;
	int			source = 0, destination = 0;
	unsigned	iForm = 0;

	iStartGuide = satind(startGuide);
	source = SatinGuideIndex - 1;
	destination = SatinGuideIndex + guideCount - 1;
	while (source >= iStartGuide) {
		SatinGuides[destination].start = SatinGuides[source].start;
		SatinGuides[destination--].finish = SatinGuides[source--].finish;
	}
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			FormList[iForm].satinOrAngle.guide += guideCount;
	}
	SatinGuideIndex += guideCount;
}

SATCON* nusac(unsigned formIndex, unsigned guideCount) noexcept {
	unsigned	iForm = 0, guideIndex = 0;

	for (iForm = 0; iForm < formIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			guideIndex += FormList[iForm].satinGuideCount;
	}
	FormList[formIndex].satinOrAngle.guide = &SatinGuides[guideIndex];
	sacspac(FormList[formIndex].satinOrAngle.guide, guideCount);
	return FormList[formIndex].satinOrAngle.guide;
}

void delclps(unsigned iForm) {
	deleclp(iForm);
	delmclp(iForm);
}

unsigned findclp(unsigned formIndex) {
	unsigned	iForm;

	for (iForm = formIndex; iForm != 0; iForm--) {
		if (iseclp(iForm - 1))
			return FormList[iForm - 1].borderClipData - ClipPoints + FormList[iForm - 1].clipEntries;
		if (isclp(iForm - 1))
			return FormList[iForm - 1].angleOrClipData.clip - ClipPoints + FormList[iForm - 1].lengthOrCount.clipCount;
	}
	return 0;
}

bool chkmax(unsigned arg0, unsigned arg1) noexcept {
	if (arg0&MAXMSK)
		return 1;
	if (arg1&MAXMSK)
		return 1;
	if ((arg1 + arg0)&MAXMSK)
		return 1;
	return 0;
}

void clpsub(unsigned fpnt, unsigned cnt) {
	unsigned	iForm;

	for (iForm = fpnt + 1; iForm < FormIndex; iForm++) {
		if (isclpx(iForm))
			FormList[iForm].angleOrClipData.clip -= cnt;
		if (iseclpx(fpnt))
			FormList[iForm].borderClipData -= cnt;
	}
}

void delmclp(unsigned iForm) {
	unsigned	source = 0, destination = 0;

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
	unsigned	source = 0, destination = 0;

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

fPOINT* nueclp(unsigned currentForm, unsigned count) {
	int			find = findclp(ClosestFormToCursor);
	int			source = ClipPointIndex - 1;
	int			destination = ClipPointIndex + count - 1;
	unsigned	iform = currentForm;

	if (isclp(ClosestFormToCursor))
		find += FormList[ClosestFormToCursor].lengthOrCount.clipCount;
	while (source >= find) {
		ClipPoints[destination].x = ClipPoints[source].x;
		ClipPoints[destination--].y = ClipPoints[source--].y;
	}
	for (iform = currentForm; iform < FormIndex; iform++) {
		if (iseclpx(iform))
			FormList[iform].borderClipData += count;
	}
	for (iform = currentForm + 1; iform < FormIndex; iform++) {
		if (isclp(iform))
			FormList[iform].angleOrClipData.clip += count;
	}
	ClipPointIndex += count;
	return &ClipPoints[find];
}

fPOINT* numclp() {
	int			find = findclp(ClosestFormToCursor);
	int			source = ClipPointIndex - 1;
	int			destination = ClipPointIndex + ClipStitchCount - 1;
	unsigned	iForm = ClosestFormToCursor + 1;

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

unsigned fltind(const fPOINT* point) noexcept {
	return point - FormVertices;
}

unsigned clpind(const fPOINT* point) noexcept {
	return point - ClipPoints;
}

void fltspac(const fPOINT* start, unsigned count) noexcept {
	const int	startIndex = fltind(start);
	int			source = FormVertexIndex - 1;
	int			destination = FormVertexIndex + count - 1;
	unsigned	iForm = ClosestFormToCursor + 1;

	while (source >= startIndex) {
		FormVertices[destination].x = FormVertices[source].x;
		FormVertices[destination--].y = FormVertices[source--].y;
	}
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		FormList[iForm].vertices += count;
	FormVertexIndex += count;
}

void delsac(unsigned formIndex) noexcept {
	unsigned	iForm = 0;
	unsigned	source = 0, destination = 0;

	if (SatinGuideIndex) {
		if (FormList[formIndex].type == SAT && FormList[formIndex].satinGuideCount) {
			destination = satind(FormList[formIndex].satinOrAngle.guide);
			source = destination + FormList[formIndex].satinGuideCount;
			while (source < SatinGuideIndex) {
				SatinGuides[destination].start = SatinGuides[source].start;
				SatinGuides[destination++].finish = SatinGuides[source++].finish;
			}
			for (iForm = formIndex + 1; iForm < FormIndex; iForm++) {
				if (FormList[iForm].type == SAT && FormList[iForm].satinGuideCount)
					FormList[iForm].satinOrAngle.guide -= FormList[formIndex].satinGuideCount;
			}
			SatinGuideIndex -= FormList[formIndex].satinGuideCount;
		}
	}
	FormList[formIndex].satinGuideCount = 0;
}

void delflt(unsigned formIndex) noexcept {
	unsigned	iForm = 0;
	unsigned	source = 0, destination = 0;

	if (FormList[formIndex].vertexCount) {
		destination = fltind(FormList[formIndex].vertices);
		source = destination + FormList[formIndex].vertexCount;
		while (source < FormVertexIndex) {
			FormVertices[destination++] = FormVertices[source++];
		}
		for (iForm = formIndex + 1; iForm < FormIndex; iForm++)
			FormList[iForm].vertices -= FormList[formIndex].vertexCount;
		FormVertexIndex -= FormList[formIndex].vertexCount;
		if (FormVertexIndex & 0x8000000)
			FormVertexIndex = 0;
	}
}

bool chk2of() {
	if (!StateMap.test(StateFlag::SELBOX))
		return 0;
	if (UserFlagMap.test(UserFlag::FIL2OF))
		return 0;
	return 1;
}

void rotbak() {
	unsigned	iSequence;

	for (iSequence = 0; iSequence < SequenceIndex; iSequence++)
		rotflt(&OSequence[iSequence]);
}

void delfil(unsigned attribute) {
	unsigned	iSource = 0, iDestination = 0;

	attribute &= TYPMSK | FRMSK;
	StateMap.reset(StateFlag::WASDEL);
	while ((StitchBuffer[iSource].attribute&(TYPMSK | FRMSK)) != attribute && iSource < PCSHeader.stitchCount)
		iSource++;
	iDestination = iSource;
	if (iSource < PCSHeader.stitchCount) {
		StateMap.set(StateFlag::WASDEL);
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

void delmfil() {
	const unsigned	attribute = ClosestFormToCursor << FRMSHFT;
	unsigned	stitchAttribute = 0;
	unsigned	iSource = 0, iDestination = 0;

	if (istx(ClosestFormToCursor))
		deltx();
	delmclp(ClosestFormToCursor);
	for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
		stitchAttribute = StitchBuffer[iSource].attribute;
		if (!(stitchAttribute&NOTFRM)) {
			if (!((stitchAttribute&FRMSK) == attribute && (stitchAttribute&(TYPFRM | FTHMSK))))
				mvstch(iDestination++, iSource);
		}
		else
			mvstch(iDestination++, iSource);
	}
	PCSHeader.stitchCount = iDestination;
}

void fsizpar() noexcept {
	SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
	SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
	SelectedForm->minFillStitchLen = MinStitchLength;
}

void chkcont() {
	unsigned	iGuide = 0, shortestGuideIndex = 0, length = 0, minimumLength = 10000;

	delmclp(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	fsizpar();
	if (SelectedForm->fillType != CONTF) {
		if (CurrentFormGuidesCount) {
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
	unsigned	iStitch = 0, attribute = 0;

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
	StateMap.reset(StateFlag::WASPCDCLP);
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory)
			StateMap.set(StateFlag::WASPCDCLP);
	}
	CloseClipboard();
}

void frmout(unsigned formIndex) noexcept {
	fRECTANGLE*	rectangle = nullptr;
	unsigned	iVertex = 0;
	float		offset = 0.0;

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

void sfCor2px(fPOINT stitchPoint, POINT* screen) noexcept {
	screen->x = (stitchPoint.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	screen->y = StitchWindowClientRect.bottom - (stitchPoint.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void stCor2px(fPOINTATTR stitch, POINT* screen) noexcept {
	screen->x = (stitch.x - ZoomRect.left)*ZoomRatio.x + 0.5;
	screen->y = StitchWindowClientRect.bottom - (stitch.y - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void px2stchf(POINT screen, fPOINT* stitchPoint) noexcept {
	double	factorX = 0.0, factorY = 0.0;

	factorX = static_cast<double>(screen.x) / static_cast<double>(StitchWindowClientRect.right);
	stitchPoint->x = factorX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	factorY = static_cast<double>(StitchWindowClientRect.bottom - screen.y) / StitchWindowClientRect.bottom;
	stitchPoint->y = factorY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

void frmlin(const fPOINT* vertices, unsigned vertexCount) noexcept {
	if (vertices) {
		unsigned	iVertex = 0;

		if (VertexCount) {
			for (iVertex = 0; iVertex < vertexCount; iVertex++) {
				FormLines[iVertex].x = (vertices[iVertex].x - ZoomRect.left)*ZoomRatio.x;
				FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[iVertex].y - ZoomRect.bottom)*ZoomRatio.y;
			}
			FormLines[iVertex].x = (vertices[0].x - ZoomRect.left)*ZoomRatio.x;
			FormLines[iVertex].y = StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom)*ZoomRatio.y;
		}
	}
}

void dufrm() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, FormLines, NewFormVertexCount);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unfrm() {
	if (StateMap.testAndReset(StateFlag::SHOFRM))
		dufrm();
}

void mdufrm() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if (FormList[ClosestFormToCursor].type == FRMLINE)
		Polyline(StitchWindowDC, FormLines, NewFormVertexCount - 1);
	else
		Polyline(StitchWindowDC, FormLines, NewFormVertexCount);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void munfrm() {
	if (StateMap.testAndReset(StateFlag::SHOFRM))
		mdufrm();
}

void rats() {
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
		VerticalRatio = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	}
	else {
		HorizontalRatio = static_cast<double>(UnzoomedRect.x) / StitchWindowClientRect.right;
		VerticalRatio = static_cast<double>(UnzoomedRect.y) / StitchWindowClientRect.bottom;
	}
}

void setfrm() {
	unsigned	iVertex = 0;
	fPOINT		point = {};
	fPOINT		delta = {};

	rats();
	ClosestFormToCursor = FormIndex;
	fvars(ClosestFormToCursor);
	px2stchf(FormLines[0], &point);
	delta.x = point.x - CurrentFormVertices[0].x;
	delta.y = point.y - CurrentFormVertices[0].y;
	SelectedForm->rectangle.left = SelectedForm->rectangle.bottom = static_cast<float>(1e30);
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
	StateMap.reset(StateFlag::FORMIN);
	StateMap.set(StateFlag::INIT);
	StateMap.set(StateFlag::RESTCH);
}

void form() {
	shoMsg(StringTable->at(STR_FMEN));
	StateMap.set(StateFlag::FORMIN);
	StateMap.reset(StateFlag::INSRT);
	duzrat();
}

void frmsqr(unsigned iVertex) {
	double	angle = 0.0;
	double	ratio = 0.0;
	float	length = 0.0;
	fPOINT	offset = {};
	fPOINT	point = {};
	fPOINT	adjustedPoint = {};
	dPOINT	delta = {};
	POINT	line[4] = {};

	stch2pxr(CurrentFormVertices[iVertex]);
	line[1].x = StitchCoordinatesPixels.x;
	line[1].y = StitchCoordinatesPixels.y;
	ratio = static_cast<double>(IniFile.formVertexSizePixels) / StitchWindowClientRect.right;
	length = (ZoomRect.right - ZoomRect.left)*ratio * 2;
	delta.x = CurrentFormVertices[iVertex - 1].x - CurrentFormVertices[iVertex].x;
	delta.y = CurrentFormVertices[iVertex - 1].y - CurrentFormVertices[iVertex].y;
	angle = atan2(delta.y, delta.x);
	offset.x = length * cos(angle);
	offset.y = length * sin(angle);
	point.x = CurrentFormVertices[iVertex].x + offset.x;
	point.y = CurrentFormVertices[iVertex].y + offset.y;
	angle = atan2(-delta.x, delta.y);
	length /= 2;
	offset.x = length * cos(angle);
	offset.y = length * sin(angle);
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

void selsqr(POINT controlPoint, HDC dc) noexcept {
	POINT	line[5];

	line[0].x = line[3].x = line[4].x = controlPoint.x - IniFile.formVertexSizePixels;
	line[0].y = line[1].y = controlPoint.y - IniFile.formVertexSizePixels;
	line[1].x = line[2].x = controlPoint.x + IniFile.formVertexSizePixels;
	line[2].y = line[3].y = controlPoint.y + IniFile.formVertexSizePixels;
	line[4].y = controlPoint.y - IniFile.formVertexSizePixels;
	Polyline(dc, line, 5);
}

void frmsqr0(POINT controlPoint) noexcept {
	POINT		line[5] = {};
	const unsigned	offset = IniFile.formBoxSizePixels;

	if (offset) {
		line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
		line[0].y = line[1].y = controlPoint.y - offset;
		line[1].x = line[2].x = controlPoint.x + offset + 1;
		line[2].y = line[3].y = controlPoint.y + offset + 1;
		line[4].y = controlPoint.y - 1;
		Polyline(StitchWindowMemDC, line, 5);
	}
}

void frmx(POINT controlPoint, HDC dc) noexcept {
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
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
		VerticalRatio = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		HorizontalRatio = static_cast<double>(StitchWindowClientRect.right) / UnzoomedRect.x;
		VerticalRatio = static_cast<double>(StitchWindowClientRect.bottom) / UnzoomedRect.y;
	}
}

constexpr float midl(float high, float low) {
	return (high - low) / 2 + low;
}

void fvars(unsigned iForm) noexcept {
	SelectedForm = &FormList[iForm];
	CurrentFormVertices = FormList[iForm].vertices;
	VertexCount = FormList[iForm].vertexCount;
	CurrentFormGuides = FormList[iForm].satinOrAngle.guide;
	CurrentFormGuidesCount = FormList[iForm].satinGuideCount;
	SatinEndGuide = FormList[iForm].wordParam;
}

void ritfrct(unsigned iForm, HDC dc) {
	unsigned		controlPoint = 0;
	POINT			pixelOutline[10] = {};
	fRECTANGLE*		rectangle = nullptr;
	fPOINT			formOutline[10] = {};

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
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		StateMap.reset(StateFlag::SELSHO);
		SearchLineIndex = 0;
		StateMap.set(StateFlag::RESTCH);
	}
}

void delfrms() {
	unsigned	iStitch = 0;

	savdo();
	FormIndex = FormVertexIndex = SatinGuideIndex = ClipPointIndex = 0;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].attribute &= NFRM_NTYP;
		StitchBuffer[iStitch].attribute |= NOTFRM;
	}
}

void fselrct(unsigned iForm) noexcept {
	const FRMHED*	formHeader = &FormList[iForm];
	fPOINT			formOutline[5] = {};
	POINT			line[6] = {};
	unsigned		iPoint = 0;

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

void rct2sel(RECT rectangle, POINT* line) noexcept {
	if (line) {
		line[0].x = line[6].x = line[7].x = line[8].x = rectangle.left;
		line[1].x = line[5].x = ((rectangle.right - rectangle.left) >> 1) + rectangle.left;
		line[2].x = line[3].x = line[4].x = rectangle.right;
		line[0].y = line[1].y = line[2].y = line[8].y = rectangle.top;
		line[3].y = line[7].y = ((rectangle.bottom - rectangle.top) >> 1) + rectangle.top;
		line[4].y = line[5].y = line[6].y = rectangle.bottom;
	}
}

void dubig() noexcept {
	unsigned	iPoint;

	rct2sel(SelectedFormsRect, SelectedFormsLine);
	SelectObject(StitchWindowMemDC, SelectAllPen);
	Polyline(StitchWindowMemDC, SelectedFormsLine, 9);
	for (iPoint = 0; iPoint < 8; iPoint++)
		selsqr(SelectedFormsLine[iPoint], StitchWindowMemDC);
}

void frmpoly(const POINT* line, unsigned count) noexcept {
	if (line) {
		unsigned	iPoint = 0;

		if (count) {
			for (iPoint = 0; iPoint < count - 1; iPoint++)
				Polyline(StitchWindowMemDC, &line[iPoint], 2);
		}
	}
}

void dupsel(HDC dc) noexcept {
	unsigned	iPoint;

	SelectObject(dc, FormPen);
	SetROP2(dc, R2_XORPEN);
	Polyline(dc, SelectedPointsLine, 9);
	iPoint = SelectedFormVertices.start;
	for (iPoint = 0; iPoint < 8; iPoint++)
		selsqr(SelectedPointsLine[iPoint], dc);
	frmx(EndPointCross, dc);
}

void unpsel() {
	if (StateMap.testAndReset(StateFlag::SHOPSEL))
		dupsel(StitchWindowDC);
}

void drwfrm() {
	unsigned	iForm = 0, iGuide = 0, iVertex = 0, lastPoint = 0, layer = 0;
	POINT		line[2] = {};
	dPOINT		point = {};

	StateMap.reset(StateFlag::SHOMOV);
	StateMap.reset(StateFlag::SHOPSEL);
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
			if (ClosestFormToCursor == iForm && StateMap.test(StateFlag::FRMPSEL)) {
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
			if (StateMap.test(StateFlag::FPSEL) && ClosestFormToCursor == iForm) {
				sRct2px(SelectedVerticesRect, &SelectedPixelsRect);
				rct2sel(SelectedPixelsRect, SelectedPointsLine);
				StateMap.set(StateFlag::SHOPSEL);
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
		if (StateMap.test(StateFlag::FORMSEL))
			ritfrct(ClosestFormToCursor, StitchWindowMemDC);
		if (StateMap.test(StateFlag::FRMPMOV)) {
			ritmov();
			RubberBandLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
			RubberBandLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::SHOMOV);
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
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void setlin() {
	frmclr(&FormList[FormIndex]);
	FormList[FormIndex].type = FRMLINE;
	//frmtyp = FRMLINE;
	FormList[FormIndex].attribute = ActiveLayer << 1;
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void setrpoli() {
	unmsg();
	pntmsg(IDS_REGP);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRPOL);
	numWnd();
}

void setstar() {
	unmsg();
	pntmsg(IDS_STAR);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRSTAR);
	numWnd();
}

void setspir() {
	unmsg();
	pntmsg(IDS_SPIR);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRSPIR);
	numWnd();
}

void sethart() {
	unmsg();
	pntmsg(IDS_HEART);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRHART);
	numWnd();
}

void setlens() {
	unmsg();
	pntmsg(IDS_LENS);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRLENS);
	numWnd();
}

void seteg() {
	unmsg();
	pntmsg(IDS_EGG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTREG);
	numWnd();
}

void setzig() {
	unmsg();
	pntmsg(IDS_ZIG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRZIG);
	numWnd();
}

inline void initTearDlg(HWND hwndlg) noexcept {
	SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), fmt::format("{:d}", IniFile.formSides).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), fmt::format("{:.3f}", IniFile.tearTailLength).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), fmt::format("{:.3f}", IniFile.tearTwistStep / PFGRAN).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), fmt::format("{:.3f}", IniFile.tearTwistRatio).c_str());
}

BOOL CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) noexcept {
	UNREFERENCED_PARAMETER(lparam);

	char	buffer[HBUFSIZ] = { 0 };

	switch (umsg) {
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		initTearDlg(hwndlg);
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
			IniFile.tearTailLength = 1.1f;
			IniFile.tearTwistStep = 0.0f;
			IniFile.tearTwistRatio = 1.6f;
			initTearDlg(hwndlg);
			break;

		case IDC_DEFPAIS:

			IniFile.formSides = 24;
			IniFile.tearTailLength = 1.15f;
			IniFile.tearTwistStep = 0.3f*PFGRAN;
			IniFile.tearTwistRatio = 1.8f;
			initTearDlg(hwndlg);
			break;
		}
	}
	return 0;
}

void setear() {
	unsigned	nResult = 0, iStep = 0, iVertex = 0, count = 0, iRightVertices = 0, iLeftVertices = 0;
	double		middle = 0.0;
	double		verticalPosition = 0.0;
	double		length = 0.0;
	double		step = 0.0;
	double		horizontalRatio = 0.0;
	double		verticalRatio = 0.0;
	float		twistStep = 0.0;
	fPOINT		size = {};

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
		for (iStep = 0; iStep < count; iStep++) {
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
		StateMap.set(StateFlag::FORMSEL);
		fvars(FormIndex);
		frmout(FormIndex);
		flipv();
		StateMap.reset(StateFlag::FORMSEL);
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

float findDistanceToSide(fPOINT lineStart, fPOINT lineEnd, fPOINT point, double &distance) noexcept {
	const double A = point.x - lineStart.x;
	const double B = point.y - lineStart.y;
	const double C = lineEnd.x - lineStart.x;
	const double D = lineEnd.y - lineStart.y;
	double diffX = 0.0;
	double diffY = 0.0;

	if ((C == 0) && (D == 0))
	{
		diffX = A;
		diffY = B;
		distance = sqrt(diffX * diffX + diffY * diffY);
		//Arbitrarily choose the first point since start and end are the same
		return -0.1f;
	}

	const double dot = A * C + B * D;
	const double len_sq = C * C + D * D;
	const float param = dot / len_sq;

	if (param < 0)
	{
		//point is nearest to the first point i.e lineStart.x and lineStart.y
		diffX = point.x - lineStart.x;
		diffY = point.y - lineStart.y;
	}
	else if (param > 1)
	{
		//point is nearest to the end point i.e lineEnd.x and lineEnd.y
		diffX = point.x - lineEnd.x;
		diffY = point.y - lineEnd.y;
	}
	else
	{
		//if perpendicular line intersect the line segment.
		diffX = point.x - (lineStart.x + param * C);
		diffY = point.y - (lineStart.y + param * D);
	}

	//returning shortest distance
	distance = sqrt(diffX * diffX + diffY * diffY);
	return param;
}

unsigned closfrm() {
	unsigned	iForm = 0, iVertex = 0, closestForm = 0, closestVertex = 0, layerCoded = 0, formLayer = 0;
	fPOINT		point = {};
	POINT		screenCoordinate = {};
	double		length = 0, minimumLength = 1e99;
	float		param = 0.0;

	if (FormIndex) {
		screenCoordinate.x = Msg.pt.x - StitchWindowOrigin.x;
		screenCoordinate.y = Msg.pt.y - StitchWindowOrigin.y;
		rats();
		closestForm = closestVertex = 0;
		px2stchf(screenCoordinate, &point);
		layerCoded = ActiveLayer << 1;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			if (StateMap.test(StateFlag::FRMSAM) && iForm == ClosestFormToCursor) {
				continue;
			}
			formLayer = FormList[iForm].attribute&FRMLMSK;
			if (!ActiveLayer || !formLayer || formLayer == layerCoded) {
				getfinfo(iForm);
				const fPOINT* vertices = FormList[iForm].vertices;
				if (vertices) {
					// find the closest line first and then find the closest vertex on that line
					for (iVertex = 0; iVertex < FormInfo.sideCount; iVertex++) {
						param = findDistanceToSide(vertices[iVertex], vertices[nxt(iVertex)], point, length);
						if (length < minimumLength && length >= 0) {
							minimumLength = length;
							closestForm = iForm;
							if (param < 0.5) {
								closestVertex = iVertex;
							}
							else {
								closestVertex = nxt(iVertex);
							}
						}
					}
				}
			}
		}
		if (FormList[closestForm].vertices) {
			stch2pxr(FormList[closestForm].vertices[closestVertex]);
		}
		minimumLength = hypot(StitchCoordinatesPixels.x - screenCoordinate.x, StitchCoordinatesPixels.y - screenCoordinate.y);
		if (minimumLength < CLOSENUF) {
			ClosestFormToCursor = closestForm;
			ClosestVertexToCursor = closestVertex;
			fvars(ClosestFormToCursor);
			StateMap.set(StateFlag::RELAYR);
			return 1;
		}
		else
			return 0;
	}
	else
		return 0;
}

void frmovlin() {
	unsigned	iPoint = 0, previousPoint = 0;

	fvars(ClosestFormToCursor);
	ratsr();
	if (SelectedForm->type == FRMLINE)
		NewFormVertexCount = SelectedForm->vertexCount;
	else
		NewFormVertexCount = SelectedForm->vertexCount + 1;
	frmlin(SelectedForm->vertices, SelectedForm->vertexCount);
	previousPoint = prv(ClosestVertexToCursor);
	for (iPoint = 0; iPoint < 3; iPoint++) {
		RubberBandLine[iPoint].x = FormLines[previousPoint].x;
		RubberBandLine[iPoint].y = FormLines[previousPoint].y;
		previousPoint++;
	}
	ritmov();
}

void makspac(unsigned start, unsigned count) noexcept {
	if (!chkmax(PCSHeader.stitchCount, count)) {
		MoveMemory(&StitchBuffer[start + count], &StitchBuffer[start], sizeof(fPOINTATTR)*(PCSHeader.stitchCount - start));
		PCSHeader.stitchCount += count;
	}
}

bool ritlin(fPOINT start, fPOINT finish) noexcept {
	dPOINT		delta = {}, step = {}, point = {};
	double		length = 0.0;
	unsigned	iStep = 0, count = 0;

	delta.x = finish.x - start.x;
	delta.y = finish.y - start.y;
	length = hypot(delta.x, delta.y);
	InterleaveSequence[InterleaveSequenceIndex].x = start.x;
	InterleaveSequence[InterleaveSequenceIndex++].y = start.y;
	if (length > MaxStitchLen) {
		count = ceil(length / UserStitchLen);
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
					InterleaveSequenceIndex = MAXITEMS - 2;
					return 0;
				}
				InterleaveSequence[InterleaveSequenceIndex].x = point.x;
				InterleaveSequence[InterleaveSequenceIndex++].y = point.y;
				point.x += step.x;
				point.y += step.y;
			}
		}
		else {
			SequenceIndex = MAXITEMS - 2;
			return 0;
		}
	}
	return 1;
}

unsigned closflt(float xCoordinate, float yCoordinate) noexcept {
	double		length = 0.0, minimumLength = 0.0;
	unsigned	iVertex = 0, closestVertex = 0;

	minimumLength = 1e99;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		length = hypot(xCoordinate - CurrentFormVertices[iVertex].x, yCoordinate - CurrentFormVertices[iVertex].y);
		if (length < minimumLength) {
			closestVertex = iVertex;
			minimumLength = length;
		}
	}
	return closestVertex;
}

void chkseq(bool border) {
#if BUGBAK

	unsigned index;

	for (index = 0; index < SequenceIndex; index++) {
		InterleaveSequence[index].x = OSequence[index].x;
		InterleaveSequence[index].y = OSequence[index].y;
	}
	InterleaveSequenceIndex = SequenceIndex;
#else

	double		len = 0.0;
	unsigned	iSequence = 0, destination = 0, savedIndex = 0;
	float		minimumStitchLength = 0.0;

	savedIndex = InterleaveSequenceIndex;
	if (border) {
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EDGELCHAIN || SelectedForm->edgeType == EDGEOCHAIN)
			UserStitchLen = 9 * PFGRAN;
		else
			UserStitchLen = SelectedForm->edgeStitchLen;
		minimumStitchLength = SelectedForm->minBorderStitchLen;
	}
	else {
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		MaxStitchLen = SelectedForm->maxFillStitchLen;
		if (isclp(ClosestFormToCursor))
			UserStitchLen = MaxStitchLen;
		else
			UserStitchLen = SelectedForm->lengthOrCount.stitchLength;
		minimumStitchLength = SelectedForm->minFillStitchLen;
	}
	if (UserStitchLen > MaxStitchLen)
		UserStitchLen = MaxStitchLen;
	bool flag = true;
	for (iSequence = 0; iSequence < SequenceIndex - 1; iSequence++) {
		if (!ritlin(OSequence[iSequence], OSequence[iSequence + 1])) {
			flag = false;
			break;
		}
	}
	if (flag) {
		InterleaveSequence[InterleaveSequenceIndex].x = OSequence[iSequence].x;
		InterleaveSequence[InterleaveSequenceIndex++].y = OSequence[iSequence].y;
	}
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
	if (SequenceIndex) {
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_BRD;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPBRD;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->borderColor&COLMSK;
		chkseq(true);
		InterleaveSequenceIndex2++;
	}
}

void ritapbrd() {
	if (SequenceIndex) {
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_AP;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPMSK;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->borderColor >> 4;
		chkseq(true);
		InterleaveSequenceIndex2++;
	}
}

void ritfil() {
	if (SequenceIndex) {
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPFRM;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->fillColor;
		chkseq(false);
		InterleaveSequenceIndex2++;
	}
}

void okcan() noexcept {
	GetClientRect(MsgWindow, &MsgRect);

	OKButton = CreateWindow(
		"STATIC",
		StringTable->at(STR_OKENT).c_str(),
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
		StringTable->at(STR_CANCEL).c_str(),
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
	char	buffer[HBUFSIZ];

	sdmsg();
	StateMap.reset(StateFlag::BIGBOX);

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
		StringTable->at(STR_CANCEL).c_str(),
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

bool lastch() noexcept {
	if (InterleaveSequenceIndex) {
		LastPoint.x = InterleaveSequence[InterleaveSequenceIndex - 1].x;
		LastPoint.y = InterleaveSequence[InterleaveSequenceIndex - 1].y;
		return 1;
	}
	else
		return 0;
}

unsigned getlast() noexcept {
	unsigned	iVertex = 0;
	unsigned	closestVertex = 0;
	double		length = 0.0, dx = 0.0, dy = 0.0, minimumLength = 1e99;

	if (SelectedForm->fillType) {
		lastch();
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
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

void flt2dub(fPOINT inPoint, dPOINT* outPoint) noexcept {
	outPoint->x = inPoint.x;
	outPoint->y = inPoint.y;
}

void linrutb(unsigned start) noexcept {
	const double	spacing = LineSpacing;
	unsigned		iVertex = start + 1;
	dPOINT			point = { (CurrentFormVertices[start].x),(CurrentFormVertices[start].y) };

	for (iVertex = start + 1; iVertex < VertexCount; iVertex++) {
		flt2dub(CurrentFormVertices[iVertex], &point);
		filinsb(point);
	}
	flt2dub(CurrentFormVertices[0], &point);
	filinsb(point);
	LineSpacing = spacing;
}

void oclp(const fPOINT* clip, unsigned clipEntries) {
	if (clip) {
		unsigned	iClip = 1;

		if (!StateMap.test(StateFlag::NOCLP)) {
			for (iClip = 0; iClip < clipEntries; iClip++) {
				ClipBuffer[iClip].x = clip[iClip].x;
				ClipBuffer[iClip].y = clip[iClip].y;
			}
			ClipRect.left = ClipRect.right = ClipBuffer[0].x;
			ClipRect.bottom = ClipRect.top = ClipBuffer[0].y;
			for (iClip = 1; iClip < clipEntries; iClip++) {
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
}

constexpr float getblen() {
	unsigned	iLength = 0;

	iLength = (FormList[ClosestFormToCursor].clipEntries << 16) | FormList[ClosestFormToCursor].picoLength;
	return static_cast<float>(iLength);
}

void savblen(float fLength) {
	unsigned	iLength;
	iLength = gsl::narrow<int>(round(fLength));

	FormList[ClosestFormToCursor].clipEntries = iLength >> 16;
	FormList[ClosestFormToCursor].picoLength = iLength & 0xFFFF;
}

constexpr float getplen() {
	const unsigned value = SelectedForm->picoLength;

	return(static_cast<float>(value >> 8) + (value & 0xff) / 256);
}

void savplen(float length) {
	unsigned	num = 0, fr = 0;
	double		integerPart = 0.0, fractionalPart = 0.0;

	if (length > 255)
		length = 255;
	fractionalPart = modf(length, &integerPart);
	fr = gsl::narrow<unsigned>(floor(fractionalPart * 256));
	num = integerPart;
	FormList[ClosestFormToCursor].picoLength = gsl::narrow<unsigned short>(num << 8) | fr;
}

void chkbrd() {
	float	length = 0.0;

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
			clpic();
			break;

		case EDGEDOUBLE: // Double

			dubfn();
			break;

		case EDGELCHAIN: // Lin Chain

			StateMap.set(StateFlag::LINCHN);
			chnfn();
			break;

		case EDGEOCHAIN: // Open Chain

			StateMap.reset(StateFlag::LINCHN);
			chnfn();
			break;
		}
		ritbrd();
	}
}

void boldlin(unsigned start, unsigned finish, double size) noexcept {
	dPOINT		delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
						  (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	dPOINT		step = {}, point0 = {}, point1 = {};
	const double	length = hypot(delta.x, delta.y);
	unsigned	count = length / size;

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

void bold(double size) noexcept {
	unsigned	iVertex = 0, iSequence = 0, iOutput = 0;
	unsigned	iNextLine = getlast();
	unsigned	iLine = iNextLine;
	double		length = 0.0;

	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[iLine].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[iLine].y;
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
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

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
bool projv(double xCoordinate, fPOINT lowerPoint, fPOINT upperPoint, dPOINT* intersection) noexcept {
	double	swap = 0.0, slope = 0.0;
	const double	deltaX = upperPoint.x - lowerPoint.x;

	intersection->x = xCoordinate;

	if (deltaX) {
		slope = (upperPoint.y - lowerPoint.y) / deltaX;
		intersection->y = (xCoordinate - lowerPoint.x)*slope + lowerPoint.y;
		if (lowerPoint.x > upperPoint.x) {
			swap = lowerPoint.x;
			lowerPoint.x = upperPoint.x;
			upperPoint.x = swap;
		}
		if (xCoordinate<lowerPoint.x || xCoordinate>upperPoint.x)
			return false;
		else
			return true;
	}
	else
		return false;
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
bool projh(double yCoordinate, fPOINT point0, fPOINT point1, dPOINT* intersection) noexcept {
	double	swap = 0.0, slope = 0.0;
	const double	deltaX = point1.x - point0.x;
	double	deltaY = 0.0;

	intersection->y = yCoordinate;
	if (deltaX) {
		deltaY = point1.y - point0.y;
		if (deltaY) {
			slope = deltaY / deltaX;
			intersection->x = (yCoordinate - point0.y) / slope + point0.x;
		}
		else
			return false;
	}
	else
		intersection->x = point0.x;
	if (point0.y > point1.y) {
		swap = point0.y;
		point0.y = point1.y;
		point1.y = swap;
	}
	if (yCoordinate<point0.y || yCoordinate>point1.y)
		return false;
	else
		return true;
}

void fnvrt(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints) {
	unsigned		iVertex = 0, iNextVertex = 0, iLine = 0, evenPointCount = 0;
	unsigned		iLineCounter = 0, iPoint = 0, fillLineCount = 0, savedLineCount = 0;
	int				lineOffset = 0;
	double			lowX = 0.0, highX = 0.0;
	double			currentX = 0.0, step = 0.0;
	dPOINT			point = {};
	unsigned		maximumLines = 0;	//maximum angle fill lines for any adjusted y cordinate

	CurrentFillVertices = SelectedForm->vertices;
	highX = lowX = CurrentFillVertices[0].x;
	VertexCount = SelectedForm->vertexCount;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		if (CurrentFillVertices[iVertex].x > highX)
			highX = CurrentFillVertices[iVertex].x;
		if (CurrentFillVertices[iVertex].x < lowX)
			lowX = CurrentFillVertices[iVertex].x;
	}

	lineOffset = lowX / LineSpacing;
	lowX = LineSpacing * lineOffset;
	fillLineCount = (highX - lowX) / LineSpacing + 1;
	std::vector<dPOINTLINE> projectedPoints;
	projectedPoints.reserve(VertexCount + 2);
	step = (highX - lowX) / fillLineCount;
	currentX = lowX;
	for (iLine = 0; iLine < fillLineCount; iLine++) {
		iLineCounter = 0;
		currentX += step;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			iNextVertex = (iVertex + 1) % VertexCount;
			if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[iNextVertex], &point))
				iLineCounter++;
		}
		fillLineCount += iLineCounter;
		if (iLineCounter > maximumLines)
			maximumLines = iLineCounter;
	}
	maximumLines = (maximumLines >> 1);
	lineEndpoints.reserve(fillLineCount + 1);
	StitchLineCount = 0; LineGroupIndex = 0;
	// groupIndex cannot be more than fillLineCount so reserve that amount of memory to reduce re-allocations
	groupIndexSequence.reserve(fillLineCount);
	GroupIndexCount = 0;
	currentX = lowX;
	for (iLine = 0; iLine < fillLineCount; iLine++) {
		projectedPoints.clear();
		currentX += step;
		iPoint = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			iNextVertex = (iVertex + 1) % VertexCount;
			if (projv(currentX, CurrentFillVertices[iVertex], CurrentFillVertices[iNextVertex], &point)) {
				dPOINTLINE a = { point.x,point.y,gsl::narrow<unsigned short>(iVertex) };
				projectedPoints.push_back(a);
				iPoint++;
			}
		}
		if (iPoint > 1) {
			evenPointCount = iPoint &= 0xfffffffe;
			groupIndexSequence.push_back(StitchLineCount);
			std::sort(projectedPoints.begin(), projectedPoints.end(), comp);
			iPoint = 0;
			savedLineCount = StitchLineCount;
			while (iPoint < evenPointCount) {
				if (StitchLineCount < fillLineCount) {
					lineEndpoints.push_back({ projectedPoints[iPoint].line, LineGroupIndex, projectedPoints[iPoint].x, projectedPoints[iPoint].y });
					iPoint++;
					lineEndpoints.push_back({ projectedPoints[iPoint].line, LineGroupIndex, projectedPoints[iPoint].x, projectedPoints[iPoint].y });
					iPoint++;
					StitchLineCount += 2;
				}
			}
			if (StitchLineCount != savedLineCount)
				LineGroupIndex++;
		}
	}
	groupIndexSequence.push_back(StitchLineCount);
	GroupIndexCount = groupIndexSequence.size();
	LineGroupIndex--;
}

void fnang(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints) {
	unsigned	iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	RotationCenter.x = static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex].x = SelectedForm->vertices[iVertex].x;
		AngledForm.vertices[iVertex].y = SelectedForm->vertices[iVertex].y;
		rotflt(&AngledForm.vertices[iVertex]);
	}
	SelectedForm = &AngledForm;
	fnvrt(groupIndexSequence, lineEndpoints);
	SelectedForm = &FormList[ClosestFormToCursor];
}

void fnhor(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints) {
	unsigned	iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	RotationCenter.x = static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	RotationAngle = PI / 2;
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex].x = SelectedForm->vertices[iVertex].x;
		AngledForm.vertices[iVertex].y = SelectedForm->vertices[iVertex].y;
		rotflt(&AngledForm.vertices[iVertex]);
	}
	SelectedForm = &AngledForm;
	fnvrt(groupIndexSequence, lineEndpoints);
	SelectedForm = &FormList[ClosestFormToCursor];
}

void refilfn() {
	double			spacing = 0.0;
	const double	stitchLength = UserStitchLength;
	float			length = 0.0;
	FILLSTARTS		FillStartsData = {};				//fill start data for refill

	StateMap.reset(StateFlag::TXFIL);
	fvars(ClosestFormToCursor);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->underlayIndent = 0;
	savdo();
	fdelstch(FillStartsData);
	StateMap.set(StateFlag::WASREFIL);
	if (SelectedForm->fillSpacing < 0.5 && !isclp(ClosestFormToCursor))
		SelectedForm->fillSpacing = 0.5;
	if (SelectedForm->edgeSpacing < 0.5)
		SelectedForm->edgeSpacing = 0.5;
	UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
	if (!StateMap.test(StateFlag::WASDO))
		savdo();
	StateMap.reset(StateFlag::WASDO);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK) && SelectedForm->type == FRMLINE && SelectedForm->fillType != CONTF)
		SelectedForm->type = FRMFPOLY;
	InterleaveSequenceIndex = InterleaveSequenceIndex2 = 0;
	StateMap.reset(StateFlag::ISUND);
	std::vector<RNGCNT> textureSegments(SelectedForm->fillInfo.texture.lines);
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

			StateMap.reset(StateFlag::SAT1);
			slbrd();
			ritbrd();
			break;

		case EDGEPROPSAT:

			if (SelectedForm->vertexCount > 2) {
				StateMap.reset(StateFlag::SAT1);
				plbrd(SelectedForm->edgeSpacing);
				ritbrd();
			}
			break;

		case EDGEAPPL:

			lapbrd();
			ritapbrd();
			StateMap.reset(StateFlag::SAT1);
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
			clpic();
			ButtonholeCornerLength = length;
			ritbrd();
			break;

		case EDGEDOUBLE:

			dubfn();
			ritbrd();
			break;

		case EDGELCHAIN:

			StateMap.set(StateFlag::LINCHN);
			chnfn();
			ritbrd();
			break;

		case EDGEOCHAIN:

			StateMap.reset(StateFlag::LINCHN);
			chnfn();
			ritbrd();
			break;

		case EDGECLIPX:

			oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			duxclp();
			ritbrd();
			break;
		}
		if (SelectedForm->fillType == CONTF && SelectedForm->attribute&FRECONT) {
			contf();
			ritfil();
		}
		break;

	case FRMFPOLY:

		chkcwlk();
		chkwlk();
		chkund(textureSegments);
		StateMap.reset(StateFlag::ISUND);
		if (SelectedForm->fillType) {
			spacing = LineSpacing;
			LineSpacing = SelectedForm->fillSpacing;
			std::vector<SMALPNTL> lineEndpoints;
			std::vector<unsigned> groupIndexSequence;
			bool doFill = true;
			switch (gsl::narrow<unsigned>(SelectedForm->fillType)) {
			case VRTF:

				fnvrt(groupIndexSequence, lineEndpoints);
				WorkingFormVertices = SelectedForm->vertices;
				break;

			case HORF:

				fnhor(groupIndexSequence, lineEndpoints);
				WorkingFormVertices = AngledForm.vertices;
				break;

			case ANGF:

				RotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
				fnang(groupIndexSequence, lineEndpoints);
				WorkingFormVertices = AngledForm.vertices;
				break;

			case VCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				setangf(0);
				fvars(ClosestFormToCursor);
				clpcon(textureSegments);
				doFill = false;
				break;

			case HCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				horclpfn(textureSegments);
				doFill = false;
				break;

			case ANGCLPF:

				oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				StateMap.reset(StateFlag::ISUND);
				angclpfn(textureSegments);
				doFill = false;
				break;

			case TXVRTF:

				setangf(0);
				setxt(textureSegments);
				clpcon(textureSegments);
				doFill = false;
				break;

			case TXHORF:

				setxt(textureSegments);
				horclpfn(textureSegments);
				doFill = false;
				break;

			case TXANGF:

				setxt(textureSegments);
				StateMap.reset(StateFlag::ISUND);
				angclpfn(textureSegments);
				doFill = false;
				break;
			}
			if (doFill) {
				lcon(groupIndexSequence, lineEndpoints);
				bakseq();
				if (SelectedForm->fillType != VRTF && SelectedForm->fillType != TXVRTF) {
					RotationAngle = -RotationAngle;
					rotbak();
				}
			}
			ritfil();
			LineSpacing = spacing;
		}
		chkbrd();
		break;

	case SAT:

		chkcwlk();
		chkwlk();
		chkund(textureSegments);
		StateMap.reset(StateFlag::ISUND);
		switch (SelectedForm->fillType) {
		case SATF:

			spacing = LineSpacing;
			LineSpacing = SelectedForm->fillSpacing;
			UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
			satfil();
			LineSpacing = spacing;
			ritfil();
			break;

		case CLPF:

			oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
			fmclp();
			ritfil();
			break;

		case FTHF:

			if (StateMap.testAndReset(StateFlag::FLPBLND))
				SelectedForm->extendedAttribute ^= AT_FTHBLND;
			StateMap.set(StateFlag::CNV2FTH);
			fthrfn();
			fritfil();
			break;
		}
		chkbrd();
	}
	UserStitchLength = stitchLength;
	intlv(FillStartsData);
	ritot(PCSHeader.stitchCount);
	setfchk();
}

void refil() {
	unsigned	codedForm = 0, iStitch = 0, attribute = 0;

	if (!UserFlagMap.test(UserFlag::WRNOF)) {
		codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			attribute = StitchBuffer[iStitch].attribute;
			if (!(attribute&NOTFRM) && (attribute&(USMSK | FRMSK)) == codedForm) {
				if (FormDataSheet)
					StateMap.set(StateFlag::WASFRMFRM);
				undat();
				tabmsg(IDS_REFIL);
				StateMap.set(StateFlag::MOVMSG);
				okcan();
				return;
			}
		}
	}
	refilfn();
}

void setfpnt() {
	fvars(ClosestFormToCursor);

	POINT	screenCoordinate = { (Msg.pt.x - StitchWindowOrigin.x),
								 (Msg.pt.y - StitchWindowOrigin.y) };
	if (fPOINT*	vertices = &SelectedForm->vertices[0]) {
		unfrm();
		rats();
		px2stchf(screenCoordinate, &vertices[ClosestVertexToCursor]);
		frmout(ClosestFormToCursor);
		refil();
		StateMap.set(StateFlag::WASFPNT);
		StateMap.reset(StateFlag::SELBOX);
		StateMap.set(StateFlag::FRMPSEL);
		ritfcor(&CurrentFormVertices[ClosestVertexToCursor]);
		StateMap.set(StateFlag::RESTCH);
	}
}

constexpr unsigned nxt(unsigned int iVertex) {
	iVertex++;
	if (iVertex > VertexCount - 1)
		iVertex = 0;
	return iVertex;
}

constexpr unsigned prv(unsigned iVertex) {
	if (iVertex)
		iVertex--;
	else
		iVertex = VertexCount - 1;
	return iVertex;
}

/* find the intersection of two lines, one defined by point and slope, the other by the coordinates
   of the endpoints. */
bool proj(dPOINT point, double slope, fPOINT point0, fPOINT point1, dPOINT* intersectionPoint) noexcept {
	dPOINT	delta = {};
	double	sideSlope = 0.0, pointConstant = 0.0, sideConstant = 0.0, xMinimum = 0.0;
	double	xMaximum = 0.0, yMinimum = 0.0, yMaximum = 0.0, swap = 0.0;

	delta.x = point1.x - point0.x;
	delta.y = point1.y - point0.y;
	if (delta.x) {
		sideSlope = delta.y / delta.x;
		sideConstant = point0.y - sideSlope * point0.x;
		pointConstant = point.y - slope * point.x;
		intersectionPoint->x = (sideConstant - pointConstant) / (slope - sideSlope);
		intersectionPoint->y = intersectionPoint->x*slope + pointConstant;
	}
	else {
		intersectionPoint->x = point0.x;
		pointConstant = point.y - slope * point.x;
		intersectionPoint->y = intersectionPoint->x*slope + pointConstant;
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
		if (intersectionPoint->x<xMinimum || intersectionPoint->x>xMaximum || intersectionPoint->y<yMinimum || intersectionPoint->y>yMaximum)
			return false;
		else
			return true;
	}
	else {
		if (intersectionPoint->x<xMinimum || intersectionPoint->x>xMaximum)
			return false;
		else
			return true;
	}
}

void makpoli() noexcept {
	if (SelectedForm->type == SAT) {
		if (SelectedForm->satinGuideCount)
			delsac(ClosestFormToCursor);
	}
	SelectedForm->type = FRMFPOLY;
}

void filinu(float xCoordinate, float yCoordinate) {
	dPOINT		point = { SelectedPoint.x ,SelectedPoint.y };
	dPOINT		delta = { (xCoordinate - SelectedPoint.x),(yCoordinate - SelectedPoint.y) };
	const double		length = hypot(delta.x, delta.y);
	unsigned	count = length / UserStitchLength;
	dPOINT		step = {};

	if (chkmax(count, SequenceIndex) || count + SequenceIndex > MAXITEMS - 3)
		return;
	if (count) {
		if (StateMap.test(StateFlag::FILDIR))
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
	dPOINT	delta = { (currentPoint.x - SelectedPoint.x),(currentPoint.y - SelectedPoint.y) };
	dPOINT	step = {};
	dPOINT	point = { SelectedPoint.x ,SelectedPoint.y };
	const double	length = hypot(delta.x, delta.y);
	int		count = length / UserStitchLength;

	if (chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXITEMS - 3)
		return;
	if (count) {
		if (StateMap.test(StateFlag::FILDIR))
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

unsigned short isclos(const SMALPNTL* lineEndPoint0, const SMALPNTL* lineEndPoint1) noexcept {
	if (lineEndPoint0 && lineEndPoint1) {
		const float	low0 = lineEndPoint0[0].y - GapToClosestRegion;
		const float	high0 = lineEndPoint0[1].y + GapToClosestRegion;
		const float	low1 = lineEndPoint1[0].y - GapToClosestRegion;
		const float	high1 = lineEndPoint1[1].y + GapToClosestRegion;

		if (high0 < low1)
			return 0;
		if (high1 < low0)
			return 0;
	}
	return 1;
}

bool lnclos(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints, unsigned group0, unsigned line0, unsigned group1, unsigned line1) noexcept {
	unsigned		index0 = 0, index1 = 0;
	unsigned		count0 = (groupIndexSequence[group0 + 1] - groupIndexSequence[group0]) >> 1;
	unsigned		count1 = 0;
	const SMALPNTL*	lineEndPoint0 = &lineEndpoints[groupIndexSequence[group0]];

	if (group1 > GroupIndexCount - 2)
		return 0;
	if (group0 == 0)
		return 0;
	if (lineEndPoint0) {
		while (count0 && lineEndPoint0[index0].line != line0) {
			count0--;
			index0 += 2;
		}
		if (count0) {
			count1 = (groupIndexSequence[group1 + 1] - groupIndexSequence[group1]) >> 1;
			index1 = 0;
			if (const SMALPNTL* lineEndPoint1 = &lineEndpoints[groupIndexSequence[group1]]) {
				while (count1 && lineEndPoint1[index1].line != line1) {
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
		}
	}
	return 0;
}

bool regclos(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints, std::vector<SMALPNTL*> &sortedLines, unsigned iRegion0, unsigned iRegion1, std::vector<REGION> &regionsList) noexcept {
	//ToDo - More renaming required

	const SMALPNTL*	lineEndPoint0Start = sortedLines[regionsList[iRegion0].start];
	SMALPNTL*		lineEndPoint0End = nullptr;
	const SMALPNTL*	lineEndPoint1Start = sortedLines[regionsList[iRegion1].start];
	SMALPNTL*		lineEndPoint1End = nullptr;
	const unsigned	group0Start = lineEndPoint0Start->group;
	unsigned		group0End = 0;
	const unsigned	group1Start = lineEndPoint1Start->group;
	unsigned		group1End = 0;
	unsigned		groupStart = 0;
	unsigned		groupEnd = 0;
	unsigned		lineStart = 0, lineEnd = 0;
	unsigned		prevLine = 0, lastLine = 0;

	if (group0Start > group1Start) {
		groupStart = group0Start;
		lineStart = lineEndPoint0Start->line;
		prevLine = lineEndPoint1Start->line;
	}
	else {
		groupStart = group1Start;
		lineStart = lineEndPoint1Start->line;
		prevLine = lineEndPoint0Start->line;
	}
	if (groupStart && lnclos(groupIndexSequence, lineEndpoints, groupStart - 1, prevLine, groupStart, lineStart)) {
		NextGroup = groupStart;
		return 1;
	}
	else {
		lineEndPoint0End = sortedLines[regionsList[iRegion0].end];
		lineEndPoint1End = sortedLines[regionsList[iRegion1].end];
		group1End = lineEndPoint1End->group;
		group0End = lineEndPoint0End->group;
		if (group0End < group1End) {
			groupEnd = group0End;
			lineEnd = lineEndPoint0End->line;
			lastLine = lineEndPoint1End->line;
		}
		else {
			groupEnd = group1End;
			lineEnd = lineEndPoint1End->line;
			lastLine = lineEndPoint0End->line;
		}
		if (lnclos(groupIndexSequence, lineEndpoints, groupEnd, lineEnd, groupEnd + 1, lastLine)) {
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

bool unvis(boost::dynamic_bitset<> &visitedRegions) noexcept {
	for (VisitedIndex = 0; VisitedIndex < RegionCount; VisitedIndex++) {
		if (!visitedRegions[VisitedIndex])
			return true;
	}
	return false;
}

void rspnt(float xCoordinate, float yCoordinate) noexcept {
	BSequence[OutputIndex].x = xCoordinate;
	BSequence[OutputIndex].y = yCoordinate;
	BSequence[OutputIndex++].attribute = 0;
}

void dunseq(std::vector<SMALPNTL*> &sortedLines, unsigned start, unsigned finish) {
	double minimumY = 1e30;

	for (unsigned iLine = start; iLine <= finish; iLine++) {
		const double deltaY = sortedLines[start][1].y - sortedLines[start][0].y;
		if (deltaY < minimumY)
			minimumY = deltaY;
	}
	if (minimumY == 1e30) {
		minimumY = 0;
	}
	else {
		minimumY /= 2;
	}
	rspnt(sortedLines[start][0].x, sortedLines[start][0].y + minimumY);
	rspnt(sortedLines[finish][0].x, sortedLines[finish][0].y + minimumY);
	LastGroup = sortedLines[finish][0].group;
}

void movseq(std::vector<SMALPNTL*> &sortedLines, unsigned ind) noexcept {
	SMALPNTL*	lineEndPoint = sortedLines[ind];

	BSequence[OutputIndex].attribute = SEQBOT;
	BSequence[OutputIndex].x = lineEndPoint->x;
	BSequence[OutputIndex].y = lineEndPoint->y;
	OutputIndex++;
	//Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
	//             and not the next entry in sortedLines
	lineEndPoint++;
	BSequence[OutputIndex].attribute = SEQTOP;
	BSequence[OutputIndex].x = lineEndPoint->x;
	BSequence[OutputIndex].y = lineEndPoint->y;
	OutputIndex++;
}

void duseq2(std::vector<SMALPNTL*> &sortedLines, unsigned iLine) noexcept {
	SequenceLines = sortedLines[iLine];
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x, (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq1() noexcept {
	rspnt((SequenceLines[1].x - SequenceLines[0].x) / 2 + SequenceLines[0].x, (SequenceLines[1].y - SequenceLines[0].y) / 2 + SequenceLines[0].y);
}

void duseq(std::vector<SMALPNTL*> &sortedLines, unsigned start, unsigned finish, boost::dynamic_bitset<> &sequenceMap) {
	unsigned	iLine = 0, iLineDec = 0;
	unsigned	savedTopLine = sortedLines[start][1].line;
	bool		flag = false;

	SequenceLines = sortedLines[start];
	StateMap.reset(StateFlag::SEQDUN);
	if (start > finish) {
		// This odd construction for iLine is used to ensure loop terminates when finish = 0
		for (iLine = start + 1; iLine != finish; iLine--) {
			iLineDec = iLine - 1;
			if (sequenceMap.test_set(iLineDec)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
					flag = true;
					duseq2(sortedLines, iLineDec);
				}
				else {
					if (savedTopLine != sortedLines[iLineDec][1].line) {
						if (iLineDec) {
							duseq2(sortedLines, iLineDec + 1);
						}
						flag = true;
						duseq2(sortedLines, iLineDec);
						savedTopLine = SequenceLines[1].line;
					}
				}
			}
			else {
				if (StateMap.testAndReset(StateFlag::SEQDUN)) {
					duseq2(sortedLines, (iLineDec + 1));
				}
				flag = true;
				SequenceLines = sortedLines[iLineDec];
				movseq(sortedLines, iLineDec);
			}
		}
		if (StateMap.testAndReset(StateFlag::SEQDUN)) {
			flag = true;
			duseq2(sortedLines, iLine);
		}
		if (flag) {
			LastGroup = SequenceLines->group;
		}
	}
	else {
		for (iLine = start; iLine <= finish; iLine++) {
			if (sequenceMap.test_set(iLine)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
					flag = true;
					duseq2(sortedLines, iLine);
				}
				else {
					if (savedTopLine != sortedLines[iLine][1].line) {
						if (iLine) {
							duseq2(sortedLines, (iLine - 1));
						}
						flag = true;
						duseq2(sortedLines, iLine);
						savedTopLine = SequenceLines[1].line;
					}
				}
			}
			else {
				if (StateMap.testAndReset(StateFlag::SEQDUN)) {
					if (iLine) {
						duseq2(sortedLines, (iLine - 1));
					}
				}
				flag = true;
				SequenceLines = sortedLines[iLine];
				movseq(sortedLines, iLine);
			}
		}
		if (StateMap.testAndReset(StateFlag::SEQDUN)) {
			if (iLine) {
				flag = true;
				duseq2(sortedLines, (iLine - 1));
			}
		}
		if (flag) {
			LastGroup = SequenceLines->group;
		}
	}
}

void brkseq(std::vector<SMALPNTL*> &sortedLines, unsigned start, unsigned finish, boost::dynamic_bitset<> &sequenceMap) {
	unsigned	iLine = 0, iLineDec = 0, savedGroup = 0;

	StateMap.reset(StateFlag::SEQDUN);
	if (start > finish) {
		savedGroup = sortedLines[start]->group + 1;
		// This odd construction for iLine is used to ensure
		// loop terminates when finish = 0
		for (iLine = start + 1; iLine != finish; iLine--) {
			iLineDec = iLine - 1;
			savedGroup--;
			if (sortedLines[iLineDec]->group != savedGroup) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = sortedLines[iLineDec];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				savedGroup = SequenceLines[0].group;
			}
			else {
				SequenceLines = sortedLines[iLineDec];
			}
			if (sequenceMap.test_set(iLineDec)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN))
					duseq1();
			}
			else {
				movseq(sortedLines, iLineDec);
			}
		}
		LastGroup = SequenceLines->group;
	}
	else {
		savedGroup = sortedLines[start]->group - 1;
		for (iLine = start; iLine <= finish; iLine++) {
			savedGroup++;
			if (sortedLines[iLine]->group != savedGroup) {
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				SequenceLines = sortedLines[iLine];
				rspnt(SequenceLines[0].x, SequenceLines[0].y);
				savedGroup = SequenceLines[0].group;
			}
			else {
				SequenceLines = sortedLines[iLine];
			}
			if (sequenceMap.test_set(iLine)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN))
					duseq1();
			}
			else {
				movseq(sortedLines, iLine);
			}
		}
		LastGroup = SequenceLines->group;
	}
	if (StateMap.testAndReset(StateFlag::SEQDUN)) {
		duseq1();
	}
}

void brkdun(std::vector<SMALPNTL*> &sortedLines, unsigned start, unsigned finish) {
	rspnt(sortedLines[start]->x, sortedLines[start]->y);
	rspnt(sortedLines[finish]->x, sortedLines[finish]->y);
	rspnt(WorkingFormVertices[sortedLines[start]->line].x, WorkingFormVertices[sortedLines[start]->line].y);
	StateMap.set(StateFlag::BRKFIX);
}

void durgn(std::vector<FSEQ> &sequencePath, boost::dynamic_bitset<> &visitedRegions, std::vector<SMALPNTL*> &sortedLines, unsigned pthi, unsigned lineCount, std::vector<REGION> &regionsList) {
	unsigned	dun = 0, gdif = 0, mindif = 0, iVertex = 0, ind = 0, fdif = 0, bdif = 0;
	unsigned	seql = 0, seqn = 0;
	unsigned	sequenceStart = 0;
	unsigned	sequenceEnd = 0;
	unsigned	groupStart = 0, groupEnd = 0;
	double		length = 0.0, minimumLength = 0.0;
	BSEQPNT*	bpnt = nullptr;
	const unsigned	nextGroup = sequencePath[pthi].nextGroup;
	const unsigned	iRegion = sequencePath[pthi].node;

	boost::dynamic_bitset<> sequenceMap(lineCount);

	//ToDo - More renaming required
	CurrentRegion = &regionsList[iRegion];
	sequenceStart = CurrentRegion->start;
	sequenceEnd = CurrentRegion->end;
	if (sequencePath[pthi].skp || StateMap.testAndReset(StateFlag::BRKFIX)) {
		if (BSequence[OutputIndex - 1].attribute != SEQBOT) {
			rspnt(BSequence[OutputIndex - 2].x, BSequence[OutputIndex - 2].y);
		}
		dun = sortedLines[sequenceStart]->line;
		bpnt = &BSequence[OutputIndex - 1];
		minimumLength = 1e99;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			length = hypot(bpnt->x - WorkingFormVertices[iVertex].x, bpnt->y - WorkingFormVertices[iVertex].y);
			if (length < minimumLength) {
				minimumLength = length;
				mindif = iVertex;
			}
		}
		if (minimumLength) {
			rspnt(WorkingFormVertices[mindif].x, WorkingFormVertices[mindif].y);
		}
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
	if (visitedRegions[iRegion]) {
		dun = 1;
	}
	else {
		dun = 0;
		visitedRegions.set(iRegion);
	}
	groupStart = sortedLines[CurrentRegion->start]->group;
	groupEnd = sortedLines[CurrentRegion->end]->group;
	if (groupEnd != groupStart) {
		seql = static_cast<double>(LastGroup - groupStart) / (groupEnd - groupStart)*(sequenceEnd - sequenceStart) + sequenceStart;
	}
	else {
		seql = 0;
	}
	if (seql > lineCount) {
		seql = 0;
	}
	length = static_cast<double>(groupEnd - groupStart)*(sequenceEnd - sequenceStart);
	if (length) {
		seqn = static_cast<double>(nextGroup - groupStart) / length + sequenceStart;
	}
	else {
		seqn = sequenceEnd;
	}
	if (seql < sequenceStart) {
		seql = sequenceStart;
	}
	if (seql > sequenceEnd) {
		seql = sequenceEnd;
	}
	if (seqn < sequenceStart) {
		seqn = sequenceStart;
	}
	if (seqn > sequenceEnd) {
		seqn = sequenceEnd;
	}
	if (sortedLines[seql]->group != LastGroup) {
		if (seql < sequenceEnd && sortedLines[seql + 1]->group == LastGroup) {
			seql++;
		}
		else {
			if (seql > sequenceStart && sortedLines[seql - 1]->group == LastGroup)
				seql--;
			else {
				mindif = 0xffffffff;
				for (ind = sequenceStart; ind <= sequenceEnd; ind++) {
					gdif = ((sortedLines[ind]->group > LastGroup) ? (sortedLines[ind]->group - LastGroup) : (LastGroup - sortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seql = ind;
					}
				}
			}
		}
	}
	if (sortedLines[seqn]->group != nextGroup) {
		if (seqn < sequenceEnd && sortedLines[seqn + 1]->group == nextGroup) {
			seqn++;
		}
		else {
			if (seqn > sequenceStart && sortedLines[seqn - 1]->group == nextGroup) {
				seqn--;
			}
			else {
				mindif = 0xffffffff;
				for (ind = sequenceStart; ind <= sequenceEnd; ind++) {
					gdif = ((sortedLines[ind]->group > nextGroup) ? (sortedLines[ind]->group - nextGroup) : (nextGroup - sortedLines[ind]->group));
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
			brkdun(sortedLines, seql, seqn);
		}
		else {
			if (LastGroup >= groupEnd) {
				brkseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
				if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn) {
					brkseq(sortedLines, sequenceStart, seqn, sequenceMap);
				}
			}
			else {
				if (groupStart <= nextGroup) {
					if (seql != sequenceStart) {
						brkseq(sortedLines, seql, sequenceStart, sequenceMap);
					}
					brkseq(sortedLines, sequenceStart, sequenceEnd, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn) {
						brkseq(sortedLines, sequenceEnd, seqn, sequenceMap);
					}
				}
				else {
					if (seql != sequenceEnd) {
						brkseq(sortedLines, seql, sequenceEnd, sequenceMap);
					}
					brkseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceStart != seqn) {
						brkseq(sortedLines, sequenceStart, seqn, sequenceMap);
					}
				}
			}
		}
	}
	else {
		if (dun) {
			dunseq(sortedLines, seql, seqn);
		}
		else {
			if (LastGroup >= groupEnd) {
				duseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
				duseq(sortedLines, sequenceStart, seqn, sequenceMap);
			}
			else {
				if (groupStart <= nextGroup) {
					if (seql != sequenceStart) {
						duseq(sortedLines, seql, sequenceStart, sequenceMap);
					}
					duseq(sortedLines, sequenceStart, sequenceEnd, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceEnd != seqn) {
						duseq(sortedLines, sequenceEnd, seqn, sequenceMap);
					}
				}
				else {
					if (seql != sequenceEnd) {
						duseq(sortedLines, seql, sequenceEnd, sequenceMap);
					}
					duseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap);
					if (pthi < SequencePathIndex - 1 && sequenceStart != seqn) {
						duseq(sortedLines, sequenceStart, seqn, sequenceMap);
					}
				}
			}
		}
	}
}

unsigned notdun(std::vector<RGSEQ> &tempPath, std::vector<RCON> &pathMap, std::vector<unsigned> &MapIndexSequence, boost::dynamic_bitset<> &VisitedRegions, unsigned level) noexcept {
	unsigned	iPath = 1;
	int			pivot = 0;
	int			previousLevel = level - 1;

	RegionPath = &tempPath[SequencePathIndex];
	RegionPath[0].pcon = MapIndexSequence[DoneRegion];
	RegionPath[0].count = MapIndexSequence[DoneRegion + 1] - RegionPath[0].pcon;
	for (iPath = 1; iPath < level; iPath++) {
		RegionPath[iPath].pcon = MapIndexSequence[pathMap[RegionPath[iPath - 1].pcon].node];
		RegionPath[iPath].count = MapIndexSequence[pathMap[RegionPath[iPath - 1].pcon].node + 1] - RegionPath[iPath].pcon;
	}
	while (VisitedRegions[pathMap[RegionPath[previousLevel].pcon].node] && previousLevel >= 0) {
		if (--RegionPath[previousLevel].count > 0) {
			RegionPath[previousLevel].pcon++;
		}
		else {
			pivot = previousLevel;
			do {
				pivot--;
				if (pivot < 0) {
					return 1;
				}
				RegionPath[pivot].count--;
				RegionPath[pivot].pcon++;
			} while (!RegionPath[pivot].count);
			if (pivot < 0) {
				return 1;
			}
			pivot++;
			while (pivot <= previousLevel) {
				if (pivot) {
					RegionPath[pivot].pcon = MapIndexSequence[pathMap[RegionPath[pivot - 1].pcon].node];
					RegionPath[pivot].count = MapIndexSequence[pathMap[RegionPath[pivot - 1].pcon].node + 1] - RegionPath[pivot].pcon;
				}
				else {
					if (--RegionPath[0].count) {
						RegionPath[0].pcon++;
					}
					else {
						return 1;
					}
				}
				pivot++;
			}
		}
	}
	return 0;
}

double reglen(std::vector<SMALPNTL*> &sortedLines, unsigned iRegion, std::array<fPOINT, 4> &lastRegionCorners, std::vector<REGION> &regionsList) noexcept {
	double		length = 0.0, minimumLength = 1e99;
	unsigned	iCorner = 0, iPoint = 0;
	SMALPNTL*	lineEndPoints[4] = {};

	lineEndPoints[0] = sortedLines[regionsList[iRegion].start];
	lineEndPoints[1] = &sortedLines[regionsList[iRegion].start][1];
	lineEndPoints[2] = sortedLines[regionsList[iRegion].end];
	lineEndPoints[3] = &sortedLines[regionsList[iRegion].end][1];
	for (iCorner = 0; iCorner < 4; iCorner++) {
		for (iPoint = 0; iPoint < 4; iPoint++) {
			length = hypot(lastRegionCorners[iCorner].x - lineEndPoints[iPoint]->x, lastRegionCorners[iCorner].y - lineEndPoints[iPoint]->y);
			if (length < minimumLength) {
				minimumLength = length;
			}
		}
	}
	return minimumLength;
}

void nxtrgn(std::vector<RGSEQ> &tempPath, std::vector<RCON> &pathMap, std::vector<unsigned> &mapIndexSequence, boost::dynamic_bitset<> &visitedRegions, std::vector<SMALPNTL*> &sortedLines, std::vector<REGION> &regionsList) noexcept {
	unsigned	iRegion = 0, iPath = 0, newRegion = 0;
	double		length = 0, minimumLength = 1e99;
	unsigned	pathLength = 1;				//length of the path to the region

	std::array<fPOINT, 4> lastRegionCorners = {};	//corners of last region sequenced

	while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength)) {
		pathLength++;
		if (pathLength > 8) {
			SMALPNTL* lineEndPoint = sortedLines[regionsList[DoneRegion].start];
			if (lineEndPoint) {
				lastRegionCorners[0].x = lineEndPoint[0].x;
				lastRegionCorners[0].y = lineEndPoint[0].y;
				lastRegionCorners[1].x = lineEndPoint[1].x;
				lastRegionCorners[1].y = lineEndPoint[1].y;
			}
			lineEndPoint = sortedLines[regionsList[DoneRegion].end];
			if (lineEndPoint) {
				lastRegionCorners[2].x = lineEndPoint[0].x;
				lastRegionCorners[2].y = lineEndPoint[0].y;
				lastRegionCorners[3].x = lineEndPoint[1].x;
				lastRegionCorners[3].y = lineEndPoint[1].y;
			}
			newRegion = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++) {
				if (!visitedRegions[iRegion]) {
					length = reglen(sortedLines, iRegion, lastRegionCorners, regionsList);
					if (length < minimumLength) {
						minimumLength = length;
						newRegion = iRegion;
					}
				}
			}
			tempPath[SequencePathIndex].skp = true;
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				if (pathMap[iPath].node == newRegion) {
					tempPath[SequencePathIndex++].pcon = iPath;
					visitedRegions.set(newRegion);
					DoneRegion = newRegion;
					return;
				}
			}
			tempPath[SequencePathIndex].count = VisitedIndex;
			tempPath[SequencePathIndex++].pcon = 0xffffffff;
			visitedRegions.set(VisitedIndex);
			DoneRegion = VisitedIndex;
			return;
		}
	}
	for (iPath = 0; iPath < pathLength; iPath++) {
		tempPath[SequencePathIndex].skp = false;
		tempPath[SequencePathIndex++].pcon = RegionPath[iPath].pcon;
		visitedRegions.set(pathMap[RegionPath[iPath].pcon].node);
	}
	DoneRegion = pathMap[RegionPath[iPath - 1].pcon].node;
}

bool sqcomp(const SMALPNTL *arg1, const SMALPNTL *arg2) noexcept {
	if (arg1 && arg2) {
		const SMALPNTL	lineEnd1 = *arg1;
		const SMALPNTL	lineEnd2 = *arg2;

		if (lineEnd1.line == lineEnd2.line) {
			if (lineEnd1.group == lineEnd2.group) {
				if (lineEnd1.y == lineEnd2.y) {
					return false;
				}
				else {
					return (lineEnd1.y < lineEnd2.y);
				}
			}
			else {
				return (lineEnd1.group < lineEnd2.group);
			}
		}
		else {
			return (lineEnd1.line < lineEnd2.line);
		}
	}
	return false;
}

void nxtseq(std::vector<FSEQ> &sequencePath, std::vector<RCON> &pathMap, std::vector<unsigned> &mapIndexSequence, unsigned pathIndex) noexcept {
	unsigned	iPath = mapIndexSequence[sequencePath[pathIndex].node];
	unsigned	nextNode = 0;
	if ((pathIndex + 1) < sequencePath.size()) {
		nextNode = sequencePath[pathIndex + 1].node;
		while (iPath < mapIndexSequence[sequencePath[pathIndex].node + 1] && pathMap[iPath].node != nextNode) {
			iPath++;
		}
		sequencePath[PathIndex++].nextGroup = pathMap[iPath].nextGroup;
	}
	else {
		sequencePath[PathIndex++].nextGroup = 0;
	}
}

#define BUGSEQ 0

void lcon(std::vector<unsigned> &groupIndexSequence, std::vector<SMALPNTL> &lineEndpoints) {
	unsigned		iPath = 0, iLine = 0, iRegion = 0, iSequence = 0, iNode = 0;
	unsigned		leftRegion = 0, breakLine = 0, count = 0, startGroup = 0;
	bool			isConnected = false;
	SMALPNTL*		lineGroupPoint = nullptr;
	unsigned		iStartLine = 0;
	unsigned		lineCount = 0;

#if BUGSEQ

	unsigned		bugColor;
#endif

	if (StitchLineCount) {
		std::vector<SMALPNTL*> sortedLines;
		sortedLines.reserve(StitchLineCount >> 1);
		for (iLine = 0; iLine < StitchLineCount; iLine += 2) {
			sortedLines.push_back(&lineEndpoints[iLine]);
		}
		std::sort(sortedLines.begin(), sortedLines.end(), sqcomp);
		lineCount = sortedLines.size();
		RegionCount = 0;
		// Count the regions. There cannot be more regions than lines
		std::vector<REGION> regions(lineCount);
		regions[0].start = 0;
		breakLine = sortedLines[0]->line;
		for (iLine = 0; iLine < lineCount; iLine++) {
			if (breakLine != sortedLines[iLine]->line) {
				regions[RegionCount++].end = iLine - 1;
				regions[RegionCount].start = iLine;
				breakLine = sortedLines[iLine]->line;
			}
		}
		regions[RegionCount++].end = iLine - 1;
		std::vector<REGION> RegionsList(RegionCount);
		boost::dynamic_bitset<> visitedRegions(RegionCount);
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			RegionsList[iRegion].start = regions[iRegion].start;
			RegionsList[iRegion].end = regions[iRegion].end;
			RegionsList[iRegion].breakCount = 0;
		}
		iStartLine = 0;
		for (iRegion = 0; iRegion < RegionCount; iRegion++) {
			count = 0;
			if ((RegionsList[iRegion].end - RegionsList[iRegion].start) > 1) {
				startGroup = sortedLines[RegionsList[iRegion].start]->group;
				for (iLine = RegionsList[iRegion].start + 1; iLine <= RegionsList[iRegion].end; iLine++) {
					startGroup++;
					if (sortedLines[iLine]->group != startGroup) {
						if (!count) {
							RegionsList[iRegion].regionBreak = iStartLine;
						}
						count++;
						startGroup = sortedLines[iLine]->group;
					}
				}
			}
			RegionsList[iRegion].breakCount = count;
		}

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

		std::vector<unsigned> mapIndexSequence;
		mapIndexSequence.reserve(RegionCount + 1);
		std::vector<RCON> pathMap;
		std::vector<FSEQ> sequencePath;

		if (RegionCount > 1) {
			PathMapIndex = 0;
			// use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
			pathMap.reserve(((RegionCount * (RegionCount - 1)) / 2) + 2);
			for (iSequence = 0; iSequence < RegionCount; iSequence++) {
				mapIndexSequence.push_back(PathMapIndex);
				count = 0; GapToClosestRegion = 0;
				for (iNode = 0; iNode < RegionCount; iNode++) {
					if (iSequence != iNode) {
						isConnected = regclos(groupIndexSequence, lineEndpoints, sortedLines, iSequence, iNode, RegionsList);
						if (isConnected) {
							pathMap.push_back({ iNode, isConnected, NextGroup });
							PathMapIndex++;
							count++;
						}
					}
				}
				while (!count) {
					GapToClosestRegion += LineSpacing;
					count = 0;
					for (iNode = 0; iNode < RegionCount; iNode++) {
						if (iSequence != iNode) {
							isConnected = regclos(groupIndexSequence, lineEndpoints, sortedLines, iSequence, iNode, RegionsList);
							if (isConnected) {
								pathMap.push_back({ iNode, isConnected, NextGroup });
								PathMapIndex++;
								count++;
							}
						}
					}
				}
			}
			mapIndexSequence.push_back(PathMapIndex);
			//find the leftmost region
			startGroup = 0xffffffff; leftRegion = 0;
			for (iRegion = 0; iRegion < RegionCount; iRegion++) {
				lineGroupPoint = sortedLines[RegionsList[iRegion].start];
				if (lineGroupPoint->group < startGroup) {
					startGroup = lineGroupPoint->group;
					leftRegion = iRegion;
				}
			}
			OutputIndex = 0;
			std::vector<RGSEQ> tempPath(((RegionCount * (RegionCount - 1)) / 2) + 1);

			//find the leftmost region in pathMap
			SequencePathIndex = 1;
			bool dontSkip = true;
			for (iPath = 0; iPath < PathMapIndex; iPath++) {
				if (pathMap[iPath].node == leftRegion) {
					dontSkip = false;
					break;
				}
			}
			if (dontSkip) {
				pathMap[PathMapIndex].node = leftRegion;
				pathMap[PathMapIndex].nextGroup = 0;
				iPath = PathMapIndex;
			}
			//set the first entry in the temporary path to the leftmost region
			tempPath[0].pcon = iPath;
			tempPath[0].count = 1;
			tempPath[0].skp = false;
			visitedRegions.set(leftRegion);
			DoneRegion = leftRegion;
			while (unvis(visitedRegions)) {
				nxtrgn(tempPath, pathMap, mapIndexSequence, visitedRegions, sortedLines, RegionsList);
			}
			count = 0xffffffff;
			sequencePath.reserve(SequencePathIndex);
			for (iPath = 0; iPath < SequencePathIndex; iPath++) {
				const bool tmpSkip = tempPath[iPath].skp;
				unsigned short tmpNode = 0;
				if (tempPath[iPath].pcon == 0xffffffff) {
					tmpNode = tempPath[iPath].count;
					count = tempPath[iPath].count;
				}
				else {
					if (tempPath[iPath].pcon != count) {
						count = tempPath[iPath].pcon;
						tmpNode = pathMap[tempPath[iPath].pcon].node;
					}
				}
				sequencePath.push_back({ tmpNode, 0, tmpSkip });
			}
			PathIndex = 0;
			for (iPath = 0; iPath < SequencePathIndex; iPath++) {
				nxtseq(sequencePath, pathMap, mapIndexSequence, iPath);
			}
			visitedRegions.reset();
			LastGroup = 0;
			for (iPath = 0; iPath < PathIndex; iPath++) {
				OutputDebugString(fmt::format("iterator {},vrt {},grpn {}\n", iPath, pathMap[iPath].node, pathMap[iPath].nextGroup).c_str());
				if (!unvis(visitedRegions))
					break;
				durgn(sequencePath, visitedRegions, sortedLines, iPath, lineCount, RegionsList);
			}
		}
		else {
			sequencePath.resize(1);
			LastGroup = 0;
			sequencePath[0].node = 0;
			sequencePath[0].nextGroup = sortedLines[RegionsList[0].end]->group;
			sequencePath[0].skp = false;
			durgn(sequencePath, visitedRegions, sortedLines, 0, lineCount, RegionsList);
		}

#if BUGSEQ

		seqskip : ;
#endif
	}
}

unsigned seqtab[] =
{
	12,7,15,11,13,17,
};

#define RITSIZ 6

void bakseq() {
#if BUGBAK

	for (SequenceIndex = 0; SequenceIndex < OutputIndex; SequenceIndex++) {
		OSequence[SequenceIndex].x = BSequence[SequenceIndex].x;
		OSequence[SequenceIndex].y = BSequence[SequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	// ToDo - More renaming required
	int		count = 0, rcnt = 0;
	int		iSequence = OutputIndex - 1;
	int		rit = 0;
	dPOINT	delta = {}, point = {}, step = {};
	double	length = 0.0, slope = 0.0;
	const double	UserStitchLength2 = UserStitchLength * 2;
	const double	UserStitchLength9 = UserStitchLength / 9;
	const double	StitchSpacing2 = LineSpacing * 2;

	SequenceIndex = 0;
	StateMap.reset(StateFlag::FILDIR);
	OSequence[SequenceIndex].x = BSequence[iSequence].x;
	OSequence[SequenceIndex].y = BSequence[iSequence].y;
	SequenceIndex++;
	SelectedPoint.x = BSequence[iSequence].x;
	SelectedPoint.y = BSequence[iSequence].y;
	iSequence--;
	while (iSequence > 0) {
		rcnt = iSequence % RITSIZ;
		if (SequenceIndex > MAXITEMS) {
			SequenceIndex = MAXITEMS - 1;
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
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					OSequence[SequenceIndex].x = BSequence[iSequence - 1].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence - 1].y;
					count = ceil(BSequence[iSequence].y / UserStitchLength);
					do {
						OSequence[SequenceIndex].y = count * UserStitchLength + (rit%seqtab[rcnt])*UserStitchLength9;
						if (OSequence[SequenceIndex].y > BSequence[iSequence].y)
							break;
						delta.y = OSequence[SequenceIndex].y - BSequence[iSequence].y;
						OSequence[SequenceIndex++].x = BSequence[iSequence].x;
						count++;
					} while (true);
					OSequence[SequenceIndex].x = BSequence[iSequence].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence].y;
				}
				else {
					OSequence[SequenceIndex].x = BSequence[iSequence].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence].y;
					count = floor(BSequence[iSequence].y / UserStitchLength);
					do {
						OSequence[SequenceIndex].y = count * UserStitchLength - ((rit + 2) % seqtab[rcnt])*UserStitchLength9;
						if (OSequence[SequenceIndex].y < BSequence[iSequence - 1].y)
							break;
						delta.y = OSequence[SequenceIndex].y - BSequence[iSequence - 1].y;
						OSequence[SequenceIndex++].x = BSequence[iSequence].x;
						count--;
					} while (true);
					OSequence[SequenceIndex].x = BSequence[iSequence - 1].x;
					OSequence[SequenceIndex++].y = BSequence[iSequence - 1].y;
				}
			}
			else {
				count = ceil(BSequence[iSequence + 1].y / UserStitchLength);
				do {
					OSequence[SequenceIndex].y = count * UserStitchLength + (rit%seqtab[rcnt])*UserStitchLength9;
					if (OSequence[SequenceIndex].y > BSequence[iSequence].y)
						break;
					delta.y = OSequence[SequenceIndex].y - BSequence[iSequence + 1].y;
					delta.x = slope * delta.y;
					OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
					count++;
				} while (true);
				OSequence[SequenceIndex].x = BSequence[iSequence].x;
				OSequence[SequenceIndex++].y = BSequence[iSequence].y;
			}
			break;

		case SEQBOT:

			if (!(SelectedForm->extendedAttribute&AT_SQR)) {
				count = floor(BSequence[iSequence + 1].y / UserStitchLength);
				do {
					OSequence[SequenceIndex].y = count * UserStitchLength - ((rit + 2) % seqtab[rcnt])*UserStitchLength9;
					if (OSequence[SequenceIndex].y < BSequence[iSequence].y)
						break;
					delta.y = OSequence[SequenceIndex].y - BSequence[iSequence + 1].y;
					delta.x = slope * delta.y;
					OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
					count--;
				} while (true);
				OSequence[SequenceIndex].x = BSequence[iSequence].x;
				OSequence[SequenceIndex++].y = BSequence[iSequence].y;
			}
			break;

		case 0:

			delta.x = BSequence[iSequence].x - BSequence[iSequence + 1].x;
			delta.y = BSequence[iSequence].y - BSequence[iSequence + 1].y;
			StateMap.reset(StateFlag::FILDIR);
			length = hypot(delta.x, delta.y);
			if (length) {
				if (length > UserStitchLength2) {
					point.x = BSequence[iSequence + 1].x;
					point.y = BSequence[iSequence + 1].y;
					count = length / UserStitchLength - 1;
					if (chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXITEMS - 3)
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

void fsvrt() {
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = VRTF;
	SelectedForm->type = FRMFPOLY;
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refilfn();
}

void filvrt() {
	unsigned iForm = 0;

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
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			fsvrt();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
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
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->angleOrClipData.angle = (float)PI / 2;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refil();
}

void filhor() {
	unsigned	iForm = 0;

	if (filmsgs(FMM_HOR))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				fshor();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fshor();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void fsangl() {
	double	savedAngle;

	savedAngle = RotationAngle;
	delmclp(ClosestFormToCursor);
	deltx();
	makpoli();
	SelectedForm->fillType = ANGF;
	RotationAngle = PI / 2 - RotationAngle;
	SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
	SelectedForm->fillColor = ActiveColor;
	fsizpar();
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type = FRMFPOLY;
	dusqr();
	refil();
	RotationAngle = savedAngle;
}

void filangl() {
	unsigned	iForm = 0;

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
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			fsangl();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void sRct2px(fRECTANGLE stitchRect, RECT* screenRect) noexcept {
	screenRect->left = (stitchRect.left - ZoomRect.left)*ZoomRatio.x + 0.5;
	screenRect->right = (stitchRect.right - ZoomRect.left)*ZoomRatio.x + 0.5;
	screenRect->top = (StitchWindowClientRect.bottom) - (stitchRect.top - ZoomRect.bottom)*ZoomRatio.y + 0.5;
	screenRect->bottom = (StitchWindowClientRect.bottom) - (stitchRect.bottom - ZoomRect.bottom)*ZoomRatio.y + 0.5;
}

void setmfrm() noexcept {
	unsigned	iForm = 0;
	POINT		point = {};
	POINT		offset = {};

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

void strtchbox() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, StretchBoxLine, 5);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unstrtch() {
	if (StateMap.testAndReset(StateFlag::SHOSTRTCH))
		strtchbox();
}

unsigned chkfrm() {
	fvars(ClosestFormToCursor);

	POINT		point = { (Msg.pt.x - StitchWindowOrigin.x),
						  (Msg.pt.y - StitchWindowOrigin.y) };
	POINT		formOrigin = {};
	RECT		rectangle = {};
	unsigned	iControl = 0;
	double		length = 0.0, minimumLength = 1e99;

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
				StateMap.set(StateFlag::STRTCH);
			else {
				StateMap.set(StateFlag::EXPAND);
				XYratio = static_cast<double>(SelectedForm->rectangle.right - SelectedForm->rectangle.left) / (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom);
			}
			SelectedFormControlVertex >>= 1;
			StateMap.set(StateFlag::SHOSTRTCH);
			strtchbox();
			return 1;
		}
	}
	if (point.x >= rectangle.left && point.x <= rectangle.right &&
		point.y >= rectangle.top && point.y <= rectangle.bottom) {
		sfCor2px(SelectedForm->vertices[0], &formOrigin);
		FormMoveDelta.x = formOrigin.x - point.x;
		FormMoveDelta.y = formOrigin.y - point.y;
		StateMap.set(StateFlag::FRMOV);
		return 1;
	}
	else
		return 0;
}

void rstfrm() {
	fPOINT		offset = {};
	unsigned	iVertex = 0, iStitch = 0;
	POINT		point = { (Msg.pt.x + gsl::narrow<long>(round(FormMoveDelta.x))),(Msg.pt.y + gsl::narrow<long>(round(FormMoveDelta.y))) };
	const	unsigned	attribute = (ClosestFormToCursor << 4);

	setmfrm();
	StateMap.reset(StateFlag::FRMOV);
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
		if ((StitchBuffer[iStitch].attribute&FRMSK) == attribute && StitchBuffer[iStitch].attribute&ALTYPMSK && !(StitchBuffer[iStitch].attribute&NOTFRM)) {
			StitchBuffer[iStitch].x += offset.x;
			StitchBuffer[iStitch].y += offset.y;
		}
	}
}

void clrfills() noexcept {
	unsigned	iForm;

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

void dusat() noexcept {
	const POINT*	line = &FormLines[SatinIndex - 1];

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, line, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void unsat() {
	if (StateMap.testAndReset(StateFlag::SHOSAT))
		dusat();
}

void drwsat() {
	unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOSAT);
	dusat();
}

void satpnt0() {
	px2stch();
	FormLines[0].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[0].y = Msg.pt.y - StitchWindowOrigin.y;
	TempPolygon[0].x = SelectedPoint.x;
	TempPolygon[0].y = SelectedPoint.y;
	SatinIndex = 1;
	StateMap.set(StateFlag::SATPNT);
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
	StateMap.set(StateFlag::RESTCH);
}

void satfix() {
	unsigned iVertex = 0;

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
		StateMap.set(StateFlag::INIT);
	}
	StateMap.reset(StateFlag::SHOSAT);
	StateMap.set(StateFlag::RESTCH);
}

void delcon(unsigned GuideIndex) {
	unsigned	iForm = ClosestFormToCursor + 1;
	// ToDo - Find a better way to calculate the offset into the SatinGuides array
	const unsigned	iGuide = &SelectedForm->satinOrAngle.guide[GuideIndex] - SatinGuides;
	SATCON*		guide = &SelectedForm->satinOrAngle.guide[GuideIndex];

	if (guide) {
		if (SatinGuideIndex > iGuide)
			MoveMemory(guide, &guide[1], (SatinGuideIndex - iGuide + 1) * sizeof(SATCON));
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			FRMHED* formHeader = &FormList[iForm];
			if (formHeader->type == SAT && formHeader->satinGuideCount)
				formHeader->satinOrAngle.guide--;
		}
		if (ClosestVertexToCursor < SatinEndGuide)
			SatinEndGuide--;
		SelectedForm->satinGuideCount--;
		SatinGuideIndex--;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount;
		if (SelectedForm->fillType == SATF)
			refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

bool satselfn() {
	unsigned	iForm = 0, iVertex = 0, layerCode = 0;
	double		length = 0, minimumLength = 1e99;
	double		deltaX = 0.0, deltaY = 0.0;

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
		StateMap.reset(StateFlag::SHOCON);
		StateMap.set(StateFlag::SATCNKT);
		if (SelectedForm->type == FRMFPOLY)
			SelectedForm->type = SAT;
	}
}

void ducon() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, FormLines, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uncon() {
	if (StateMap.testAndReset(StateFlag::SHOCON))
		ducon();
}

void drwcon() {
	uncon();
	FormLines[1].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[1].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOCON);
	ducon();
}

bool scomp(const SATCON &arg1, const SATCON &arg2) noexcept {
	return (arg1.start < arg2.start);
}

void satcpy(SATCON* destination, std::vector<SATCON> source, unsigned int size) noexcept {
	if (destination) {
		for (unsigned int iSource = 0; iSource < size; iSource++) {
			destination[iSource].start = source[iSource].start;
			destination[iSource].finish = source[iSource].finish;
		}
	}
}

void satadj() {
	fvars(ClosestFormToCursor);

	unsigned		iGuide = 0, iSource = 0, iForm = 0, iForward = 0, iVertex = 0, iReverse = 0, iDestination = 0;
	std::vector<SATCON> interiorGuides(CurrentFormGuidesCount);
	SATCON*			sourceGuide = nullptr;
	SATCON*			destinationGuide = nullptr;
	unsigned short	savedGuideCount = SelectedForm->satinGuideCount;
	FRMHED*			formHeader = nullptr;
	ExtendedBitSet<> satinMap(VertexCount);

	// ensure all guide endpoints are on valid vertices
	for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
		if (CurrentFormGuides[iGuide].finish > VertexCount - 1)
			CurrentFormGuides[iGuide].finish = VertexCount - 1;
		if (CurrentFormGuides[iGuide].start > VertexCount - 1)
			CurrentFormGuides[iGuide].start = VertexCount - 1;
	}

	// remove any guides of 0 length
	iDestination = 0;
	for (iSource = 0; iSource < CurrentFormGuidesCount; iSource++) {
		if (CurrentFormGuides[iSource].start != CurrentFormGuides[iSource].finish) {
			interiorGuides[iDestination].start = CurrentFormGuides[iSource].start;
			interiorGuides[iDestination++].finish = CurrentFormGuides[iSource].finish;
		}
	}
	if (CurrentFormGuidesCount != iDestination) {
		OutputDebugString(fmt::format("Removed {} zero distance guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
		satcpy (CurrentFormGuides, interiorGuides, iDestination);
	}
	if (SatinEndGuide || SelectedForm->attribute&FRMEND) {
		// there are end guides so set the satinMap for the next step
		satinMap.reset();
		if (SelectedForm->attribute&FRMEND) {
			satinMap.set(0);
			satinMap.set(1);
		}
		if (SatinEndGuide) {
			satinMap.set(SatinEndGuide);
			satinMap.set(SatinEndGuide + 1);
		}
		// check to see if any of the current guides are end guides and add to interiorGuides if not
		iDestination = 0;
		for (iSource = 0; iSource < CurrentFormGuidesCount; iSource++) {
			if (!satinMap.test(CurrentFormGuides[iSource].start) && !satinMap.test(CurrentFormGuides[iSource].finish)) {
				interiorGuides[iDestination].start = CurrentFormGuides[iSource].start;
				interiorGuides[iDestination++].finish = CurrentFormGuides[iSource].finish;
			}
		}
		if (CurrentFormGuidesCount != iDestination) {
			OutputDebugString(fmt::format("Removed {} end guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
			CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
			satcpy (CurrentFormGuides, interiorGuides, iDestination);
		}
		// remove any guides that start after the end guide
		if (SatinEndGuide) {
			iDestination = 0;
			for (iSource = 0; iSource < CurrentFormGuidesCount; iSource++) {
				if (CurrentFormGuides[iSource].start < SatinEndGuide) {
					interiorGuides[iDestination].start = CurrentFormGuides[iSource].start;
					interiorGuides[iDestination++].finish = CurrentFormGuides[iSource].finish;
				}
			}
			if (CurrentFormGuidesCount != iDestination) {
				OutputDebugString(fmt::format("Removed {} reversed guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
				CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
				satcpy (CurrentFormGuides, interiorGuides, iDestination);
			}
		}
	}

	if (CurrentFormGuidesCount) {
		satinMap.reset();
		for (iGuide = 0; iGuide < CurrentFormGuidesCount; iGuide++) {
			iForward = CurrentFormGuides[iGuide].start;
			if (iForward > gsl::narrow<unsigned>(SatinEndGuide) - 1)
				iForward = SatinEndGuide - 1;
			if (satinMap.test_set(iForward)) {
				iReverse = iForward;
				if (iReverse)
					iReverse--;
				while (satinMap.test(iForward) && (iForward < (gsl::narrow<unsigned>(SatinEndGuide) - 1)))
					iForward++;
				while (iReverse && (satinMap.test(iReverse)))
					iReverse--;
				if (satinMap.test(iForward) && satinMap.test(iReverse))
					break;
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].start > CurrentFormGuides[iGuide].start - iReverse)
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
				else {
					if (!satinMap.test(iForward))
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}

			}
		}
		iGuide = 0;
		do {
			iVertex = satinMap.getFirst();
			if (iVertex < VertexCount)
				CurrentFormGuides[iGuide++].start = iVertex;
		} while (iVertex < VertexCount);

		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iGuide;
		satinMap.reset();
		// Todo - are iForward and iReverse appropriate variable names below?
		for (iGuide = 0; iGuide < CurrentFormGuidesCount; iGuide++) {
			iForward = iReverse = CurrentFormGuides[iGuide].finish;
			if (iForward > VertexCount - 1)
				iForward = VertexCount - 1;
			if (satinMap.test_set(iForward)) {
				if (iForward < VertexCount - 1)
					iForward++;
				if (iReverse > gsl::narrow<unsigned>(SatinEndGuide) + 1)
					iReverse--;
				while (satinMap.test(iForward) && iForward < VertexCount - 1)
					iForward++;
				while (iReverse > gsl::narrow<unsigned>(SatinEndGuide) - 1 && (satinMap.test(iReverse)))
					iReverse--;
				if (satinMap.test(iForward) && satinMap.test(iReverse))
					break;
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].finish > CurrentFormGuides[iGuide].finish - iReverse)
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
				else {
					if (!satinMap.test(iForward))
						satinMap.set(iForward);
					else
						satinMap.set(iReverse);
				}
			}
		}
		iGuide = 0;
		do {
			iReverse = satinMap.getLast();
			if (iReverse < VertexCount)
				CurrentFormGuides[iGuide++].finish = iReverse;
		} while (iReverse < VertexCount);
		if (iGuide < CurrentFormGuidesCount)
			iGuide = CurrentFormGuidesCount;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iGuide;
		if (SatinEndGuide) {
			if (CurrentFormGuidesCount > VertexCount - SatinEndGuide - 2)
				CurrentFormGuidesCount = VertexCount - SatinEndGuide - 2;
			if (CurrentFormGuidesCount > SatinEndGuide - 2)
				CurrentFormGuidesCount = SatinEndGuide - 2;
			SelectedForm->satinGuideCount = CurrentFormGuidesCount;
		}
	}
	if (SelectedForm->satinGuideCount < savedGuideCount) {
		iGuide = savedGuideCount - CurrentFormGuidesCount;
		sourceGuide = destinationGuide = SelectedForm->satinOrAngle.guide;
		destinationGuide += SelectedForm->satinGuideCount;
		sourceGuide += savedGuideCount;
		MoveMemory(destinationGuide, sourceGuide, sizeof(SATCON)*(&SatinGuides[SatinGuideIndex] - sourceGuide + 1));
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			formHeader = &FormList[iForm];
			if (formHeader->type == SAT)
				formHeader->satinOrAngle.guide -= iGuide;
		}
		SatinGuideIndex -= iGuide;
	}
}

void satclos() {
	unsigned	iVertex = 0, closestVertex = StartPoint, swap = 0;
	unsigned	initialGuideCount = SelectedForm->satinGuideCount;
	double		deltaX = 0.0, deltaY = 0.0, length = 0.0, minimumLength = 1e99;

	uninsf();
	px2stch();
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		deltaX = SelectedPoint.x - SelectedForm->vertices[iVertex].x;
		deltaY = SelectedPoint.y - SelectedForm->vertices[iVertex].y;
		length = hypot(deltaX, deltaY);
		if (length < minimumLength) {
			minimumLength = length;
			ClosestVertexToCursor = iVertex;
		}
	}
	StateMap.reset(StateFlag::SATCNKT);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->fillType = CONTF;
		closestVertex = ClosestVertexToCursor;
		if (StartPoint > closestVertex) {
			swap = closestVertex;
			closestVertex = StartPoint;
			StartPoint = swap;
		}
		if (!StartPoint)
			StartPoint++;
		if (StartPoint == SelectedForm->vertexCount - 2 && closestVertex == gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1) {
			StartPoint = 1;
			closestVertex = SelectedForm->vertexCount - 2;
		}
		if (closestVertex >= gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2) {
			closestVertex = SelectedForm->vertexCount - 2;
			if (StartPoint >= gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2)
				StartPoint = SelectedForm->vertexCount - 2;
		}
		if (closestVertex - StartPoint < 2) {
			closestVertex = StartPoint + 2;
			if (closestVertex > gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2) {
				closestVertex = closestVertex - SelectedForm->vertexCount - 2;
				closestVertex -= closestVertex;
				StartPoint -= closestVertex;
			}
		}
		SelectedForm->angleOrClipData.guide.start = StartPoint;
		SelectedForm->angleOrClipData.guide.finish = closestVertex;
	}
	else {
		if (ClosestVertexToCursor < closestVertex) {
			swap = ClosestVertexToCursor;
			ClosestVertexToCursor = closestVertex;
			closestVertex = swap;
		}
		if (closestVertex == 0 && ClosestVertexToCursor == VertexCount - 1) {
			closestVertex = VertexCount - 1;
			ClosestVertexToCursor = VertexCount;
		}
		if (closestVertex == 1 && ClosestVertexToCursor == VertexCount) {
			closestVertex = 0;
			ClosestVertexToCursor = 1;
		}
		if (ClosestVertexToCursor - closestVertex == 1) {
			if (SelectedForm->attribute&FRMEND)
				SelectedForm->wordParam = closestVertex;
			else {
				if (closestVertex)
					rotfrm(closestVertex);
				SelectedForm->attribute |= FRMEND;
			}
			satadj();
		}
		else {
			if (CurrentFormGuidesCount) {
				sacspac(&SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount].start = closestVertex;
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satadj();
			}
			else {
				SelectedForm->satinOrAngle.guide = nusac(ClosestFormToCursor, 1);
				SelectedForm->satinOrAngle.guide[initialGuideCount].start = closestVertex;
				SelectedForm->satinOrAngle.guide[initialGuideCount].finish = ClosestVertexToCursor;
				SelectedForm->satinGuideCount = 1;
			}
		}
	}
}

void satknkt() {
	satclos();
	fvars(ClosestFormToCursor);
	refil();
	StateMap.set(StateFlag::RESTCH);
}

void ritseq1(unsigned ind) noexcept {
	BSequence[SequenceIndex].x = CurrentFormVertices[ind].x;
	BSequence[SequenceIndex].y = CurrentFormVertices[ind].y;
	SequenceIndex++;
}

void satfn(std::vector<double> &lengths, unsigned line1Start, unsigned line1End, unsigned line2Start, unsigned line2End) {
	unsigned	line1Next = 0, line2Previous = 0, stitchCount = 0;
	unsigned	iSegment = 0, line1Count = 0, line2Count = 0, iVertex = 0;
	unsigned	iNextVertex = 0, segmentStitchCount = 0, iLine1Vertex = 0;
	unsigned	iLine2Vertex = 0, iLine1Count = 0, iLine2Count = 0;
	unsigned	line1Segments = 0, line2Segments = 0;
	double		line1Length = 0.0, line2Length = 0.0;
	dPOINT		line1Point = {}, line2Point = {}, line1Delta = {}, line2Delta = {};
	dPOINT		line1Step = {}, line2Step = {};

	if (line1Start != line1End && line2Start != line2End) {
		if (!StateMap.testAndSet(StateFlag::SAT1)) {
			if (StateMap.test(StateFlag::FTHR)) {
				BSequence[SequenceIndex].attribute = 0;
				ritseq1(line1Start%VertexCount);
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					ritseq1(line1Start%VertexCount);
					ritseq1(line2Start%VertexCount);
				}
				else {
					SelectedPoint.x = CurrentFormVertices[line1Start].x;
					SelectedPoint.y = CurrentFormVertices[line1Start].y;
					OSequence[SequenceIndex].x = SelectedPoint.x;
					OSequence[SequenceIndex++].y = SelectedPoint.y;
				}
			}
		}
		line1Length = lengths[line1End] - lengths[line1Start];
		line2Length = lengths[line2Start] - lengths[line2End];
		if (fabs(line1Length) > fabs(line2Length))
			stitchCount = fabs(line2Length) / LineSpacing;
		else
			stitchCount = fabs(line1Length) / LineSpacing;
		line1Segments = ((line1End > line1Start) ? (line1End - line1Start) : (line1Start - line1End));
		line2Segments = ((line2Start > line2End) ? (line2Start - line2End) : (line2End - line2Start));
		std::vector<unsigned> line1StitchCounts;
		line1StitchCounts.reserve(line1Segments);
		std::vector<unsigned> line2StitchCounts;
		line2StitchCounts.reserve(line2Segments);
		iVertex = line1Start;
		segmentStitchCount = 0;
		for (iSegment = 0; iSegment < line1Segments - 1; iSegment++) {
			iNextVertex = nxt(iVertex);
			const unsigned val = ((lengths[iNextVertex] - lengths[iVertex]) / line1Length)*stitchCount + 0.5;
			line1StitchCounts.push_back(val);
			segmentStitchCount += val;
			iVertex = nxt(iVertex);
		}
		line1StitchCounts.push_back(stitchCount - segmentStitchCount);
		iNextVertex = line2Start;
		iVertex = prv(iNextVertex);
		iSegment = 0;
		segmentStitchCount = 0;
		while (iVertex > line2End) {
			const unsigned val = ((lengths[iNextVertex] - lengths[iVertex]) / line2Length)*stitchCount + 0.5;
			line2StitchCounts.push_back(val);
			segmentStitchCount += val;
			iNextVertex = prv(iNextVertex);
			iVertex = prv(iNextVertex);
		}
		line2StitchCounts.push_back(stitchCount - segmentStitchCount);
		line1Point.x = CurrentFormVertices[line1Start].x;
		line1Point.y = CurrentFormVertices[line1Start].y;
		line1Next = nxt(line1Start);
		line2Previous = prv(line2Start);
		line1Count = line1StitchCounts[iLine1Count++];
		line2Count = line2StitchCounts[iLine2Count++];
		iLine1Vertex = line1Start;
		iLine2Vertex = line2Start;
		line1Delta.x = CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x;
		line1Delta.y = CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y;
		if (iLine2Vertex == VertexCount) {
			line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[0].x;
			line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[0].y;
			line2Point.x = CurrentFormVertices[0].x;
			line2Point.y = CurrentFormVertices[0].y;
		}
		else {
			line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[iLine2Vertex].x;
			line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[iLine2Vertex].y;
			line2Point.x = CurrentFormVertices[iLine2Vertex].x;
			line2Point.y = CurrentFormVertices[iLine2Vertex].y;
		}
		iLine1Vertex = nxt(iLine1Vertex);
		iLine2Vertex = prv(iLine2Vertex);
		line1Step.x = line1Delta.x / line1Count;
		line1Step.y = line1Delta.y / line1Count;
		line2Step.x = line2Delta.x / line2Count;
		line2Step.y = line2Delta.y / line2Count;
		bool flag = false;
		do {
			flag = false;
			if (StateMap.test(StateFlag::FTHR)) {
				while (line1Count && line2Count) {
					line1Point.x += line1Step.x;
					line1Point.y += line1Step.y;
					line2Point.x += line2Step.x;
					line2Point.y += line2Step.y;
					if (StateMap.testAndFlip(StateFlag::FILDIR)) {
						BSequence[SequenceIndex].attribute = 0;
						BSequence[SequenceIndex].x = line1Point.x;
						BSequence[SequenceIndex++].y = line1Point.y;
					}
					else {
						BSequence[SequenceIndex].attribute = 1;
						BSequence[SequenceIndex].x = line2Point.x;
						BSequence[SequenceIndex++].y = line2Point.y;
					}
					if (SequenceIndex > MAXITEMS - 6) {
						SequenceIndex = MAXITEMS - 6;
						return;
					}
					line1Count--;
					line2Count--;
				}
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					while (line1Count && line2Count) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							BSequence[SequenceIndex].attribute = 0;
							BSequence[SequenceIndex].x = line1Point.x;
							BSequence[SequenceIndex++].y = line1Point.y;
							BSequence[SequenceIndex].attribute = 1;
							BSequence[SequenceIndex].x = line2Point.x;
							BSequence[SequenceIndex++].y = line2Point.y;
						}
						else {
							BSequence[SequenceIndex].attribute = 2;
							BSequence[SequenceIndex].x = line2Point.x;
							BSequence[SequenceIndex++].y = line2Point.y;
							BSequence[SequenceIndex].attribute = 3;
							BSequence[SequenceIndex].x = line1Point.x;
							BSequence[SequenceIndex++].y = line1Point.y;
						}
						if (SequenceIndex > MAXITEMS - 6) {
							SequenceIndex = MAXITEMS - 6;
							return;
						}
						line1Count--;
						line2Count--;
					}
				}
				else {
					while (line1Count && line2Count) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							if (UserFlagMap.test(UserFlag::SQRFIL))
								filinu(line2Point.x, line2Point.y);
							filin(line1Point);
						}
						else {
							if (UserFlagMap.test(UserFlag::SQRFIL))
								filinu(line1Point.x, line1Point.y);
							filin(line2Point);
						}
						line1Count--;
						line2Count--;
					}
				}
			}
			if ((iLine1Count < line1Segments || iLine2Count < line2Segments)) {
				if (!line1Count && iLine1Count < line1StitchCounts.size()) {
					line1Count = line1StitchCounts[iLine1Count++];
					line1Next = nxt(iLine1Vertex);
					line1Delta.x = CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x;
					line1Delta.y = CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y;
					iLine1Vertex = nxt(iLine1Vertex);
					line1Step.x = line1Delta.x / line1Count;
					line1Step.y = line1Delta.y / line1Count;
				}

				if (!line2Count && iLine2Count < line2StitchCounts.size()) {
					line2Count = line2StitchCounts[iLine2Count++];
					line2Previous = prv(iLine2Vertex);
					line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[iLine2Vertex].x;
					line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[iLine2Vertex].y;
					iLine2Vertex = prv(iLine2Vertex);
					line2Step.x = line2Delta.x / line2Count;
					line2Step.y = line2Delta.y / line2Count;

				}
				if ((line1Count || line2Count) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
					flag = true;
				}
			}
		} while (flag);
	}
}

void satmf(std::vector<double> &lengths) {
	unsigned	iGuide = 0, iVertex = 0;
	double		length = 0.0, deltaX = 0.0, deltaY = 0.0;

	if (SelectedForm->attribute&FRMEND)
		iGuide = 1;
	satfn(lengths, iGuide, CurrentFormGuides[0].start, VertexCount, CurrentFormGuides[0].finish);
	for (iGuide = 0; iGuide < gsl::narrow<unsigned>(CurrentFormGuidesCount) - 1; iGuide++)
		satfn(lengths, CurrentFormGuides[iGuide].start, CurrentFormGuides[iGuide + 1].start, CurrentFormGuides[iGuide].finish, CurrentFormGuides[iGuide + 1].finish);
	if (SatinEndGuide)
		satfn(lengths, CurrentFormGuides[iGuide].start, SatinEndGuide, CurrentFormGuides[iGuide].finish, SatinEndGuide + 1);
	else {
		if (CurrentFormGuides[iGuide].finish - CurrentFormGuides[iGuide].start > 2) {
			length = (lengths[CurrentFormGuides[iGuide].finish] - lengths[CurrentFormGuides[iGuide].start]) / 2 + lengths[CurrentFormGuides[iGuide].start];
			iVertex = CurrentFormGuides[iGuide].start;
			while (length > lengths[iVertex])
				iVertex++;
			deltaX = lengths[iVertex] - length;
			deltaY = length - lengths[iVertex - 1];
			if (deltaY > deltaX)
				iVertex--;
			satfn(lengths, CurrentFormGuides[iGuide].start, iVertex, CurrentFormGuides[iGuide].finish, iVertex);
		}
		else
			satfn(lengths, CurrentFormGuides[iGuide].start, CurrentFormGuides[iGuide].start + 1, CurrentFormGuides[iGuide].finish, CurrentFormGuides[iGuide].start + 1);
	}
}

extern void prbug();

void satfil() {
	unsigned		iVertex = 0;
	double			length = 0.0, deltaX = 0.0, deltaY = 0.0;
	const double	spacing = LineSpacing;

	fvars(ClosestFormToCursor);
	satadj();
	LineSpacing /= 2;
	SequenceIndex = 0;
	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SelectedForm->fillType = SATF;
	std::vector<double> lengths;
	lengths.reserve(VertexCount + 1);
	length = 0;
	lengths.push_back(length);
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		deltaX = CurrentFormVertices[iVertex].x - CurrentFormVertices[iVertex - 1].x;
		deltaY = CurrentFormVertices[iVertex].y - CurrentFormVertices[iVertex - 1].y;
		length += hypot(deltaX, deltaY);
		lengths.push_back(length);
	}
	deltaX = CurrentFormVertices[0].x - CurrentFormVertices[iVertex - 1].x;
	deltaY = CurrentFormVertices[0].y - CurrentFormVertices[iVertex - 1].y;
	length += hypot(deltaX, deltaY);
	lengths.push_back(length);
	do {
		if (SatinEndGuide) {
			if (CurrentFormGuidesCount) {
				satmf(lengths);
				break;
			}
			else {
				satfn(lengths, 1, SatinEndGuide, VertexCount, SatinEndGuide + 1);
				break;
			}
		}
		if (SelectedForm->attribute&FRMEND) {
			if (CurrentFormGuidesCount) {
				satmf(lengths);
				break;
			}
			else {
				if (VertexCount == 3 && FormList[ClosestFormToCursor].attribute & 1) {
					satfn(lengths, 2, 3, 2, 1);
					break;;
				}
				else {
					length = (length - lengths[1]) / 2;
					iVertex = 1;
					if (!StateMap.test(StateFlag::BARSAT)) {
						OSequence[0].x = SelectedPoint.x = CurrentFormVertices[1].x;
						OSequence[0].y = SelectedPoint.y = CurrentFormVertices[1].y;
						SequenceIndex = 1;
					}
					while ((length > lengths[iVertex]) && (iVertex < (VertexCount + 1))) {
						iVertex++;
					}
					deltaX = lengths[iVertex] - length;
					deltaY = length - lengths[iVertex - 1];
					if (deltaY > deltaX) {
						iVertex--;
					}
					satfn(lengths, 1, iVertex, VertexCount, iVertex);
				}
				break;
			}
		}
		if (CurrentFormGuidesCount) {
			satmf(lengths);
			break;
		}
		length /= 2;
		iVertex = 0;
		if (!StateMap.test(StateFlag::BARSAT) && !StateMap.test(StateFlag::FTHR)) {
			OSequence[0].x = SelectedPoint.x = CurrentFormVertices[0].x;
			OSequence[0].y = SelectedPoint.y = CurrentFormVertices[0].y;
			SequenceIndex = 1;
		}
		while (length > lengths[iVertex]) {
			iVertex++;
		}
		deltaX = lengths[iVertex] - length;
		deltaY = length - lengths[iVertex - 1];
		if (deltaY > deltaX)
			iVertex--;
		satfn(lengths, 0, iVertex, VertexCount, iVertex);
	} while (false);

	LineSpacing = spacing;
}

void filsfn() {
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->type = SAT;
	fsizpar();
	SelectedForm->fillType = SATF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type = SAT;
	refilfn();
}

void filsat() {
	unsigned	iForm = 0;

	if (filmsgs(FMM_FAN))
		return;
	if (SelectedFormCount) {
		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE)
				filsfn();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			filsfn();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

unsigned closat() {
	unsigned		iForm = 0, iVertex = 0, lastVertex = 0;
	double			minimumLength = 1e99, length = 0;
	unsigned int	savedVertex = 0;
	float			param = 0.0;

	px2stch();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (!ActiveLayer || gsl::narrow<unsigned>((FormList[iForm].attribute&FRMLMSK) >> 1) == ActiveLayer || !(FormList[iForm].attribute&FRMLMSK)) {
			CurrentFormVertices = FormList[iForm].vertices;
			savedVertex = VertexCount;
			VertexCount = FormList[iForm].vertexCount;
			if (FormList[iForm].type == FRMLINE) {
				lastVertex = VertexCount - 1;
			}
			else {
				lastVertex = VertexCount;
			}
			// Loop through for all line segments
			for (iVertex = 0; iVertex < lastVertex; iVertex++) {
				param = findDistanceToSide(CurrentFormVertices[iVertex], CurrentFormVertices[nxt(iVertex)], SelectedPoint, length);
				if ((length < minimumLength)) {
					if ((param < 0.0) && (iVertex == 0)) {
						// this should only happen if the Closest vertex is the start of a line (vertex 0)
						minimumLength = length;
						ClosestFormToCursor = iForm;
						ClosestVertexToCursor = iVertex;
						InOutFlag = POINT_BEFORE_LINE;
					}
					else {
						// return the vertex after the intersection
						if ((param > 1.0) && (iVertex == lastVertex - 1)) {
							minimumLength = length;
							ClosestFormToCursor = iForm;
							ClosestVertexToCursor = nxt(iVertex);
							InOutFlag = POINT_AFTER_LINE;
						}
						else {
							minimumLength = length;
							ClosestFormToCursor = iForm;
							ClosestVertexToCursor = nxt(iVertex);
							InOutFlag = POINT_IN_LINE;
						}
					}
				}
			}
			VertexCount = savedVertex;
		}
	}
	if (minimumLength == 1e99)
		return 0;
	else
		return 1;
}

void nufpnt(unsigned vertex) noexcept {
	unsigned	ind = 0;
	unsigned	newVertex = vertex + 1;

	fltspac(&FormForInsert->vertices[newVertex], 1);
	FormForInsert->vertices[newVertex].x = SelectedPoint.x;
	FormForInsert->vertices[newVertex].y = SelectedPoint.y;
	FormForInsert->vertexCount++;
	for (ind = 0; ind < FormForInsert->satinGuideCount; ind++) {
		if (FormForInsert->satinOrAngle.guide[ind].start > newVertex - 1)
			FormForInsert->satinOrAngle.guide[ind].start++;
		if (FormForInsert->satinOrAngle.guide[ind].finish > newVertex - 1)
			FormForInsert->satinOrAngle.guide[ind].finish++;
	}
	if (FormForInsert->wordParam >= newVertex) {
		FormForInsert->wordParam++;
		FormForInsert->wordParam %= VertexCount;
	}
	if (FormForInsert->fillType == CONTF) {
		if (FormForInsert->angleOrClipData.guide.start > newVertex - 1)
			FormForInsert->angleOrClipData.guide.start++;
		if (FormForInsert->angleOrClipData.guide.finish > newVertex - 1)
			FormForInsert->angleOrClipData.guide.finish++;
	}
	frmlin(FormForInsert->vertices, FormForInsert->vertexCount);
}

double p2p(fPOINT point0, fPOINT point1) noexcept {
	return hypot(point0.x - point1.x, point0.y - point1.y);
}

void insat() {
	unsigned int lastVertex = 0;

	if (closat()) {
		savdo();
		SelectedForm = &FormList[ClosestFormToCursor];
		FormForInsert = SelectedForm;
		lastVertex = FormForInsert->vertexCount - 1;
		fvars(ClosestFormToCursor);
		if (InOutFlag) {
			if (ClosestVertexToCursor == 0 && FormForInsert->type == FRMLINE) {
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				if (ClosestVertexToCursor != lastVertex && FormForInsert->type == FRMLINE) {
					ClosestVertexToCursor = prv(ClosestVertexToCursor);
				}
			}
			nufpnt(ClosestVertexToCursor);
			if (StateMap.testAndReset(StateFlag::PRELIN)) {
				SelectedPoint.x = FormForInsert->vertices[0].x;
				SelectedPoint.y = FormForInsert->vertices[0].y;
				FormForInsert->vertices[0].x = FormForInsert->vertices[1].x;
				FormForInsert->vertices[0].y = FormForInsert->vertices[1].y;
				FormForInsert->vertices[1].x = SelectedPoint.x;
				FormForInsert->vertices[1].y = SelectedPoint.y;
			}
		}
		else {
			ClosestVertexToCursor = prv(ClosestVertexToCursor);
			nufpnt(ClosestVertexToCursor);
		}
		refil();
	}
	StateMap.set(StateFlag::RESTCH);
}

bool chkdel() noexcept {
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
	unsigned	iGuide = 0, iForm = 0;
	SATCON*		guide = nullptr;
	fPOINT		vertex = {};
	FRMHED*		formHeader = nullptr;

	fvars(ClosestFormToCursor);
	if (chkdel()) {
		StateMap.set(StateFlag::DELTO);
		frmdel();
		StateMap.reset(StateFlag::FRMPSEL);
		coltab();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedForm->type == SAT) {
		if (ClosestVertexToCursor < SelectedForm->wordParam)
			SelectedForm->wordParam--;
		if (SelectedForm->satinGuideCount) {
			guide = SelectedForm->satinOrAngle.guide;
			if (guide) {
				iGuide = 0;
				while (guide[iGuide].start != ClosestVertexToCursor && guide[iGuide].finish != ClosestVertexToCursor && iGuide < SelectedForm->satinGuideCount)
					iGuide++;
				if (iGuide < SelectedForm->satinGuideCount && (guide[iGuide].start == ClosestVertexToCursor || guide[iGuide].finish == ClosestVertexToCursor)) {
					while (iGuide < SelectedForm->satinGuideCount) {
						guide[iGuide].start = guide[iGuide + 1].start;
						guide[iGuide].finish = guide[iGuide + 1].finish;
						iGuide++;
					}
					SelectedForm->satinGuideCount--;
					SatinGuideIndex--;
					for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
						formHeader = &FormList[iForm];
						if (formHeader->type == SAT && formHeader->satinGuideCount)
							formHeader->satinOrAngle.guide++;
					}
				}
				for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
					if (guide[iGuide].start > ClosestVertexToCursor)
						guide[iGuide].start--;
					if (guide[iGuide].finish > ClosestVertexToCursor)
						guide[iGuide].finish--;
				}
			}
		}
	}
	MoveMemory(&SelectedForm->vertices[ClosestVertexToCursor], &SelectedForm->vertices[ClosestVertexToCursor + 1], (FormVertexIndex - ClosestVertexToCursor) * sizeof(fPOINTATTR));
	SelectedForm->vertexCount--;
	FormVertexIndex--;
	fvars(ClosestFormToCursor);
	if (ClosestVertexToCursor > gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1)
		ClosestVertexToCursor = SelectedForm->vertexCount - 1;
	StateMap.set(StateFlag::FRMPSEL);
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		FormList[iForm].vertices--;
	ritfcor(&CurrentFormVertices[ClosestVertexToCursor]);
	ritnum(STR_NUMPNT, ClosestVertexToCursor);
	frmout(ClosestFormToCursor);
	vertex = SelectedForm->vertices[ClosestVertexToCursor];
	if (vertex.x < ZoomRect.left || vertex.x > ZoomRect.right || vertex.y < ZoomRect.bottom || vertex.y > ZoomRect.top)
		shft(vertex);
	refil();
}

void unfil() {
	unsigned	iForm = 0, iStitch = 0, iSource = 0, iDestination = 0;
	unsigned	codedForm = 0, attribute = 0;
	unsigned	mapLength = 0;

	if (filmsgs(FMX_UNF))
		return;
	if (SelectedFormCount) {
		boost::dynamic_bitset<> formMap(FormIndex);
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			SelectedForm = &FormList[SelectedFormList[iForm]];
			if (SelectedForm->fillType || SelectedForm->edgeType) {
				delclps(SelectedFormList[iForm]);
				deltx();
				formMap.set(SelectedFormList[iForm]);
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				SelectedForm->extendedAttribute &= !(AT_UND | AT_CWLK | AT_WALK);
			}
		}
		iDestination = 0;
		for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
			if (!formMap.test((StitchBuffer[iSource].attribute&FRMSK) >> FRMSHFT)) {
				StitchBuffer[iDestination].attribute = StitchBuffer[iSource].attribute;
				StitchBuffer[iDestination].x = StitchBuffer[iSource].x;
				StitchBuffer[iDestination++].y = StitchBuffer[iSource].y;
			}
		}
		PCSHeader.stitchCount = iDestination;
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (!StateMap.testAndReset(StateFlag::IGNOR) && !UserFlagMap.test(UserFlag::WRNOF)) {
				codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
				mapLength = StitchBuffer[0].attribute&(FRMSK | USMSK);
				for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
					attribute = StitchBuffer[iStitch].attribute;
					if (!(attribute&NOTFRM) && (attribute&(USMSK | FRMSK)) == codedForm) {
						tabmsg(IDS_UNFIL);
						StateMap.set(StateFlag::FILMSG);
						okcan();
						StateMap.set(StateFlag::IGNOR);
						return;
					}
				}
			}
			codedForm = ClosestFormToCursor << FRMSHFT;
			iDestination = 0;
			for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
				if ((StitchBuffer[iSource].attribute&FRMSK) != codedForm || (StitchBuffer[iSource].attribute&NOTFRM)) {
					StitchBuffer[iDestination].attribute = StitchBuffer[iSource].attribute;
					StitchBuffer[iDestination].x = StitchBuffer[iSource].x;
					StitchBuffer[iDestination++].y = StitchBuffer[iSource].y;
				}
			}
			delclps(ClosestFormToCursor);
			deltx();
			SelectedForm->fillType = 0;
			SelectedForm->edgeType = 0;
			SelectedForm->extendedAttribute &= !(AT_UND | AT_CWLK | AT_WALK);
			PCSHeader.stitchCount = iDestination;
			ritot(PCSHeader.stitchCount);
		}
	}
}

void satzum() {
	StateMap.reset(StateFlag::SHOSAT);
	duzrat();
	VertexCount = SatinIndex;
	frmlin(TempPolygon, SatinIndex);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	Polyline(StitchWindowMemDC, FormLines, SatinIndex);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	drwsat();
}

void rotfrm(unsigned newStartVertex) {
	fvars(ClosestFormToCursor);

	fPOINT*			selectedVertices = CurrentFormVertices;
	if (selectedVertices) {
		std::vector<fPOINT>	rotatedVertices(VertexCount);
		unsigned		iVertex = 0, iGuide = 0, iRotatedGuide = 0, iRotated = newStartVertex;
		unsigned short	tlin = 0;

		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			rotatedVertices[iVertex].x = selectedVertices[iVertex].x;
			rotatedVertices[iVertex].y = selectedVertices[iVertex].y;
		}
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			selectedVertices[iVertex].x = rotatedVertices[iRotated].x;
			selectedVertices[iVertex].y = rotatedVertices[iRotated].y;
			iRotated = nxt(iRotated);
		}
		iRotatedGuide = 0;
		if (SelectedForm->type == SAT) {
			if (SelectedForm->wordParam)
				SelectedForm->wordParam = (SelectedForm->wordParam + SelectedForm->vertexCount
					- newStartVertex) % SelectedForm->vertexCount;
			for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				if (CurrentFormGuides[iGuide].start != newStartVertex && CurrentFormGuides[iGuide].finish != newStartVertex) {
					CurrentFormGuides[iRotatedGuide].start = (CurrentFormGuides[iGuide].start + VertexCount - newStartVertex) % VertexCount;
					CurrentFormGuides[iRotatedGuide].finish = (CurrentFormGuides[iGuide].finish + VertexCount - newStartVertex) % VertexCount;
					if (CurrentFormGuides[iRotatedGuide].start > CurrentFormGuides[iRotatedGuide].finish) {
						tlin = CurrentFormGuides[iRotatedGuide].start;
						CurrentFormGuides[iRotatedGuide].start = CurrentFormGuides[iRotatedGuide].finish;
						CurrentFormGuides[iGuide].finish = tlin;
					}
					iRotatedGuide++;
				}
			}
		}
		if (iRotatedGuide) {
			SelectedForm->satinGuideCount = iRotatedGuide;
			// ToDo - Can we do the sort in place?
			std::vector<SATCON> rotatedGuides(iRotatedGuide);
			for (iGuide = 0; iGuide < iRotatedGuide; iGuide++) {
				rotatedGuides[iGuide].start = CurrentFormGuides[iGuide].start;
				rotatedGuides[iGuide].finish = CurrentFormGuides[iGuide].finish;
			}
			std::sort(rotatedGuides.begin(), rotatedGuides.end(), scomp);
			for (iGuide = 0; iGuide < iRotatedGuide; iGuide++) {
				CurrentFormGuides[iGuide].start = rotatedGuides[iGuide].start;
				CurrentFormGuides[iGuide].finish = rotatedGuides[iGuide].finish;
			}
		}
		if (SelectedForm->extendedAttribute&AT_STRT)
			SelectedForm->fillStart = (SelectedForm->fillStart + VertexCount - newStartVertex) % VertexCount;
		if (SelectedForm->extendedAttribute&AT_END)
			SelectedForm->fillEnd = (SelectedForm->fillEnd + VertexCount - newStartVertex) % VertexCount;
	}
}


void frm0() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		savdo();
		rotfrm(ClosestVertexToCursor);
		ClosestVertexToCursor = 0;
		satadj();
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void duinsf() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uninsf() {
	if (StateMap.testAndReset(StateFlag::SHOINSF))
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
	StateMap.set(StateFlag::SHOINSF);
	duinsf();
}

void infrm() {
	if (closat()) {
		FormForInsert = &FormList[ClosestFormToCursor];
		fvars(ClosestFormToCursor);
		if (InOutFlag) {
			if (!ClosestVertexToCursor && FormForInsert->type == FRMLINE) {
				FormVertexPrev = 0;
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				FormVertexPrev = prv(ClosestVertexToCursor);
				FormVertexNext = ClosestVertexToCursor;
			}
		}
		else {
			FormVertexNext = ClosestVertexToCursor;
			FormVertexPrev = prv(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::INSFRM);
		StateMap.set(StateFlag::INIT);
		rinfrm();
	}
}

void setins() {
	px2stch();
	nufpnt(FormVertexPrev);
	if (StateMap.test(StateFlag::PRELIN)) {
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
	StateMap.set(StateFlag::INSFRM);
	duinsf();
	StateMap.set(StateFlag::RESTCH);
}

void bdrlin(unsigned start, unsigned finish, double stitchSize) {
	dPOINT		delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
						  (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	dPOINT		step = {}, point = {};
	const double	length = hypot(delta.x, delta.y);
	double		angle = 0.0;
	unsigned	stitchCount = 0;

	if (UserFlagMap.test(UserFlag::LINSPAC)) {
		stitchCount = length / stitchSize + 0.5;
		if (stitchCount) {
			step.x = delta.x / stitchCount;
			step.y = delta.y / stitchCount;
		}
	}
	else {
		stitchCount = (length - stitchSize / 2) / stitchSize + 1;
		angle = atan2(delta.y, delta.x);
		step.x = cos(angle)*stitchSize;
		step.y = sin(angle)*stitchSize;
	}
	if (stitchCount) {
		point.x = CurrentFormVertices[start].x + step.x;
		point.y = CurrentFormVertices[start].y + step.y;
		stitchCount--;
		while (stitchCount) {
			OSequence[SequenceIndex].x = point.x;
			OSequence[SequenceIndex++].y = point.y;
			point.x += step.x;
			point.y += step.y;
			stitchCount--;
		}
	}
	OSequence[SequenceIndex].x = CurrentFormVertices[finish].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[finish].y;
}

void brdfil(double pd_Size) {
	unsigned	iVertex = 0;
	unsigned	nextVertex = 0, currentVertex = 0;

	if (SelectedForm->extendedAttribute&AT_STRT)
		currentVertex = SelectedForm->fillStart;
	else
		currentVertex = getlast();
	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[currentVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[currentVertex].y;
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		nextVertex = nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, pd_Size);
		currentVertex = nextVertex;
	}
	if (SelectedForm->type != FRMLINE) {
		nextVertex = nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, pd_Size);
	}
}

void bsizpar() noexcept {
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
	unsigned	iForm = 0;

	if (filmsgs(FML_LIN))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			SelectedForm->borderColor = ActiveColor;
			sbord();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->borderColor = ActiveColor;
			sbord();
			coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

bool ritclp(std::vector<fPOINT> clipFillData, fPOINT point) noexcept {
	fPOINT		adjustedPoint = { (point.x - ClipReference.x),
								  (point.y - ClipReference.y) };
	unsigned	iStitch = 0;

	if (chkmax(ClipStitchCount, SequenceIndex))
		return 1;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		OSequence[SequenceIndex].x = clipFillData[iStitch].x + adjustedPoint.x;
		OSequence[SequenceIndex++].y = clipFillData[iStitch].y + adjustedPoint.y;
	}
	return 0;
}

bool clpsid(std::vector<fPOINT> &clipReversedData, std::vector<fPOINT> &clipFillData, unsigned start, unsigned finish) {
	unsigned	ind = 0, clipCount = 0;
	fPOINT		delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
						  (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	fPOINT		step = {}, insertPoint = {};
	const double	length = hypot(delta.x, delta.y);
	double		tdub = 0.0;
	fPOINTATTR	clipReferencePoint = { ClipRect.left ,ClipRect.bottom };

	RotationAngle = atan2(delta.y, delta.x);
	rotang1(clipReferencePoint, &ClipReference);
	clipCount = length / ClipRectSize.cx;
	if (clipCount) {
		if (clipCount > 1)
			tdub = ((length - clipCount * ClipRectSize.cx) / (clipCount - 1) + ClipRectSize.cx) / length;
		else
			tdub = (length - ClipRectSize.cx) / 2;
		step.x = delta.x*tdub;
		step.y = delta.y*tdub;
		insertPoint.x = CurrentFormVertices[start].x;
		insertPoint.y = CurrentFormVertices[start].y;
		RotationAngle = atan2(delta.y, delta.x);
		for (ind = 0; ind < ClipStitchCount; ind++)
			rotangf(clipReversedData[ind], &clipFillData[ind]);
		for (ind = 0; ind < clipCount; ind++) {
			if (ritclp(clipFillData, insertPoint))
				break;
			insertPoint.x += step.x;
			insertPoint.y += step.y;
		}
		return 1;
	}
	return 0;
}

void linsid(std::vector<fPOINT> &clipReversedData, std::vector<fPOINT> &clipFillData) {
	fPOINT		delta = { (CurrentFormVertices[CurrentSide + 1].x - SelectedPoint.x),
						  (CurrentFormVertices[CurrentSide + 1].y - SelectedPoint.y) };
	const double	length = hypot(delta.x, delta.y);
	const unsigned	clipCount = length / ClipRectSize.cx;
	unsigned	iStitch = 0, iClip = 0;

	if (clipCount) {
		RotationAngle = ClipAngle;
		rotangf(BorderClipReference, &ClipReference);
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotangf(clipReversedData[iStitch], &clipFillData[iStitch]);
		for (iClip = 0; iClip < clipCount; iClip++) {
			ritclp(clipFillData, SelectedPoint);
			SelectedPoint.x += Vector0.x;
			SelectedPoint.y += Vector0.y;
		}
	}
}

bool nupnt() noexcept {
	double		length = 0.0, delta = 0.0;
	unsigned	step = 0;

	MoveToCoords.x = CurrentFormVertices[CurrentSide + 2].x;
	MoveToCoords.y = CurrentFormVertices[CurrentSide + 2].y;
	length = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
	if (length > ClipRectSize.cx) {
		for (step = 0; step < 10; step++) {
			length = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
			delta = ClipRectSize.cx - length;
			MoveToCoords.x += delta * CosAngle;
			MoveToCoords.y += delta * SinAngle;
			if (fabs(delta) < 0.01)
				break;
		}
		return 1;
	}
	return 0;
}

void lincrnr(std::vector<fPOINT> &clipReversedData, std::vector<fPOINT> &clipFillData) {
	dPOINT		delta = {};
	unsigned	iStitch = 0;

	SinAngle = sin(ClipAngle);
	CosAngle = cos(ClipAngle);
	if (nupnt()) {
		delta.x = MoveToCoords.x - SelectedPoint.x;
		delta.y = MoveToCoords.y - SelectedPoint.y;
		RotationAngle = atan2(delta.y, delta.x);
		rotangf(BorderClipReference, &ClipReference);
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotangf(clipReversedData[iStitch], &clipFillData[iStitch]);
		ritclp(clipFillData, SelectedPoint);
		SelectedPoint.x = MoveToCoords.x;
		SelectedPoint.y = MoveToCoords.y;
	}
}

void durev(std::vector<fPOINT> &clipReversedData) noexcept {
	unsigned	iStitch = 0;
	const double	midpoint = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;

	if (ClipBuffer[0].x > midpoint) {
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch].x = ClipRect.right - ClipBuffer[iStitch].x;
			clipReversedData[iStitch].y = ClipBuffer[iStitch].y;
		}
	}
	else {
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
			clipReversedData[iStitch].x = ClipBuffer[iStitch].x;
			clipReversedData[iStitch].y = ClipBuffer[iStitch].y;
		}
	}
}

void setvct(unsigned start, unsigned finish) noexcept {
	ClipAngle = atan2(CurrentFormVertices[finish].y - CurrentFormVertices[start].y, CurrentFormVertices[finish].x - CurrentFormVertices[start].x);
	Vector0.x = ClipRectSize.cx*cos(ClipAngle);
	Vector0.y = ClipRectSize.cx*sin(ClipAngle);
}

void clpbrd(unsigned short startVertex) {
	unsigned	iVertex = 0, reference = 0;
	unsigned	currentVertex = 0, nextVertex = 0;

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength = ClipRectSize.cx;
	std::vector<fPOINT> clipFillData(ClipStitchCount);
	std::vector<fPOINT> clipReversedData(ClipStitchCount);
	RotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	ClipReference.y = RotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	ClipReference.x = ClipRect.left;
	durev(clipReversedData);
	if (SelectedForm->type == FRMLINE) {
		SelectedPoint.x = CurrentFormVertices[0].x;
		SelectedPoint.y = CurrentFormVertices[0].y;
		setvct(0, 1);
		// Since ClipRect.bottom is always 0 
		BorderClipReference.y = ClipRect.top / 2;
		// Use 0 to align left edge of clip with beginning of line, ClipRect.right / 2 if you want to align 
		// the center of the clip with the beginning of the line
		BorderClipReference.x = 0;
		// BorderClipReference.x = ClipRect.right / 2;
		for (CurrentSide = 0; CurrentSide < VertexCount - 2; CurrentSide++) {
			linsid(clipReversedData, clipFillData);
			setvct(CurrentSide + 1, CurrentSide + 2);
			lincrnr(clipReversedData, clipFillData);
		}
		linsid(clipReversedData, clipFillData);
	}
	else {
		clpout();
		reference = currentVertex = startVertex;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			nextVertex = prv(currentVertex);
			if (clpsid(clipReversedData, clipFillData, reference, nextVertex))
				reference = nextVertex;
			currentVertex = nextVertex;
		}
	}
}

void outfn(unsigned start, unsigned finish, double satinWidth) noexcept {
	double	angle = 0.0;
	double	length = 0.0;
	double	xOffset = 0.0, yOffset = 0.0;

	if (fabs(FormAngles[start]) < TINY && fabs(FormAngles[finish]) < TINY) {
		xOffset = 0.0;
		yOffset = satinWidth;
	}
	else {
#define SATHRESH 10

		angle = (FormAngles[finish] - FormAngles[start]) / 2;
		length = satinWidth / cos(angle);
		if (length < -satinWidth * SATHRESH)
			length = -satinWidth * SATHRESH;
		if (length > satinWidth*SATHRESH)
			length = satinWidth * SATHRESH;
		angle += FormAngles[start] + PI / 2;
		xOffset = length * cos(angle);
		yOffset = length * sin(angle);
	}
	InsidePoints[finish].x = CurrentFormVertices[finish].x - xOffset;
	InsidePoints[finish].y = CurrentFormVertices[finish].y - yOffset;
	OutsidePoints[finish].x = CurrentFormVertices[finish].x + xOffset;
	OutsidePoints[finish].y = CurrentFormVertices[finish].y + yOffset;
}

void duangs() noexcept {
	unsigned int	iVertex;

	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
		FormAngles[iVertex] = atan2(CurrentFormVertices[iVertex + 1].y - CurrentFormVertices[iVertex].y, CurrentFormVertices[iVertex + 1].x - CurrentFormVertices[iVertex].x);
	FormAngles[iVertex] = atan2(CurrentFormVertices[0].y - CurrentFormVertices[iVertex].y, CurrentFormVertices[0].x - CurrentFormVertices[iVertex].x);
}

void satout(double satinWidth) {
	unsigned	iVertex = 0;
	unsigned	count = 0;

	if (VertexCount) {
		duangs();
		OutsidePoints = OutsidePointList;
		InsidePoints = InsidePointList;
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			outfn(iVertex, iVertex + 1, 0.1);
		count = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (cisin(InsidePoints[iVertex].x, InsidePoints[iVertex].y))
				count++;
		}
		satinWidth /= 2;
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			outfn(iVertex, iVertex + 1, satinWidth);
		outfn(iVertex, 0, satinWidth);
		StateMap.reset(StateFlag::INDIR);
		if (count < VertexCount >> 1) {
			StateMap.set(StateFlag::INDIR);
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
	unsigned	iStitch = 0;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGECLIP;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = ClipRectSize.cx;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->borderClipData[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->borderClipData[iStitch].y = ClipBuffer[iStitch].y;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	clpout();
	refilfn();
}

void fclp() {
	unsigned	iForm = 0;

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
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						fsclp();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
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

void filinsb(dPOINT point) noexcept {
	dPOINT		delta = { (point.x - SelectedPoint.x),
						  (point.y - SelectedPoint.y) };
	const double	length = hypot(delta.x, delta.y);
	unsigned	count = length / MAXSTCH + 1;
	dPOINT		step = { (delta.x / count),(delta.y / count) };

	if (length > MAXSTCH) {
		count--;
		if (chkmax(count, SequenceIndex))
			return;
		while (count) {
			SelectedPoint.x += step.x;
			SelectedPoint.y += step.y;
			OSequence[SequenceIndex].x = SelectedPoint.x;
			OSequence[SequenceIndex++].y = SelectedPoint.y;
			count--;
		}
	}
	if (SequenceIndex & 0xffff0000)
		return;
	OSequence[SequenceIndex].x = point.x;
	OSequence[SequenceIndex++].y = point.y;
	SelectedPoint.x = point.x;
	SelectedPoint.y = point.y;
}

bool chkbak(dPOINT pnt) noexcept {
	unsigned	iBackup = 0;
	double		length = 0.0;

	for (iBackup = 0; iBackup < 8; iBackup++) {
		length = hypot(SatinBackup[iBackup].x - pnt.x, SatinBackup[iBackup].y - pnt.y);
		if (length < LineSpacing)
			return 1;
	}
	return 0;
}

bool linx(const fPOINT* points, unsigned start, unsigned finish, dPOINT* intersection) noexcept {
	if (OutsidePoints && points) {
		dPOINT	delta = { (OutsidePoints[start].x - points[start].x),
						  (OutsidePoints[start].y - points[start].y) };
		dPOINT	point = { (points[start].x),(points[start].y) };

		if (!delta.x && !delta.y)
			return 0;
		if (delta.x) {
			if (proj(point, delta.y / delta.x, OutsidePoints[finish], points[finish], intersection))
				return 1;
			else
				return 0;
		}
		else {
			if (projv(point.x, points[finish], OutsidePoints[finish], intersection))
				return 1;
			else
				return 0;
		}
	}
	else {
		return 0;
	}
}

void filinsbw(dPOINT point) noexcept {
	SatinBackup[SatinBackupIndex].x = point.x;
	SatinBackup[SatinBackupIndex++].y = point.y;
	SatinBackupIndex &= 0x7;
	filinsb(point);
}

void sbfn(const fPOINT* insidePoints, unsigned start, unsigned finish) {
	if (insidePoints && OutsidePoints && SatinBackup) {
		dPOINT		innerDelta = { (insidePoints[finish].x - insidePoints[start].x),
								   (insidePoints[finish].y - insidePoints[start].y) };
		double		innerLength = hypot(innerDelta.x, innerDelta.y);
		dPOINT		outerDelta = { (OutsidePoints[finish].x - OutsidePoints[start].x),
								   (OutsidePoints[finish].y - OutsidePoints[start].y) };
		double		outerLength = hypot(outerDelta.x, outerDelta.y);
		dPOINT		innerPoint = { insidePoints[start].x ,insidePoints[start].y };
		dPOINT		outerPoint = { OutsidePoints[start].x ,OutsidePoints[start].y };
		dPOINT		innerStep = {}, outerStep = {};
		dPOINT		offsetDelta = {}, offsetStep = {}, offset = {};
		dPOINT		intersection = {};
		double		offsetLength = 0.0;
		unsigned	count = 0, innerFlag = 0, outerFlag = 0, offsetCount = 0, iStep = 0;
		unsigned	ind = 0, intersectFlag = 0;

		if (!StateMap.testAndSet(StateFlag::SAT1)) {
			SelectedPoint.x = insidePoints[start].x;
			SelectedPoint.y = insidePoints[start].y;
		}
		SatinBackupIndex = 0;
		for (ind = 0; ind < 8; ind++) {
			SatinBackup[ind].x = 1e12f;
			SatinBackup[ind].y = 1e12f;
		}
		if (outerLength > innerLength) {
			count = outerLength / LineSpacing;
			innerFlag = 1;
			if (linx(insidePoints, start, finish, &intersection)) {
				intersectFlag = 1;
				innerDelta.x = innerDelta.y = innerLength = 0;
				innerPoint.x = intersection.x;
				innerPoint.y = intersection.y;
			}
		}
		else {
			count = innerLength / LineSpacing;
			outerFlag = 1;
			if (linx(insidePoints, start, finish, &intersection)) {
				intersectFlag = 1;
				outerDelta.x = outerDelta.y = outerLength = 0;
				outerPoint.x = intersection.x;
				outerPoint.y = intersection.y;
			}
		}
		if (!count)
			count = 1;
		if (chkmax(count, SequenceIndex))
			return;
		innerStep.x = innerDelta.x / count;
		innerStep.y = innerDelta.y / count;
		outerStep.x = outerDelta.x / count;
		outerStep.y = outerDelta.y / count;
		for (iStep = 0; iStep < count; iStep++) {
			innerPoint.x += innerStep.x;
			innerPoint.y += innerStep.y;
			outerPoint.x += outerStep.x;
			outerPoint.y += outerStep.y;
			if (StateMap.testAndFlip(StateFlag::FILDIR)) {
				if (innerFlag) {
					offsetDelta.x = innerPoint.x - SelectedPoint.x;
					offsetDelta.y = innerPoint.y - SelectedPoint.y;
					offsetLength = hypot(offsetDelta.x, offsetDelta.y);
					offsetCount = offsetLength / LineSpacing;
					offsetStep.x = offsetDelta.x / offsetCount;
					offsetStep.y = offsetDelta.y / offsetCount;
					offset.x = innerPoint.x;
					offset.y = innerPoint.y;
					while (chkbak(offset)) {
						offset.x -= offsetStep.x;
						offset.y -= offsetStep.y;
					}
					filinsbw(offset);
				}
				else
					filinsb(innerPoint);
			}
			else {
				if (outerFlag) {
					offsetDelta.x = outerPoint.x - SelectedPoint.x;
					offsetDelta.y = outerPoint.y - SelectedPoint.y;
					offsetLength = hypot(offsetDelta.x, offsetDelta.y);
					offsetCount = offsetLength / LineSpacing;
					offsetStep.x = offsetDelta.x / offsetCount;
					offsetStep.y = offsetDelta.y / offsetCount;
					offset.x = outerPoint.x;
					offset.y = outerPoint.y;
					while (chkbak(offset)) {
						offset.x -= offsetStep.x;
						offset.y -= offsetStep.y;
					}
					filinsbw(offset);
				}
				else
					filinsb(outerPoint);
			}
		}
	}
}

void sfn(unsigned startVertex) {
	unsigned	iVertex = 0;
	unsigned	nextVertex = 0;

	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		nextVertex = nxt(startVertex);
		sbfn(InsidePoints, startVertex, nextVertex);
		startVertex = nextVertex;
	}
	OSequence[0].x = OSequence[SequenceIndex - 1].x;
	OSequence[0].y = OSequence[SequenceIndex - 1].y;
	if (SequenceIndex > MAXITEMS - 2)
		SequenceIndex = MAXITEMS - 2;
}

void sbrd() {
	const double	spacing = LineSpacing;
	const unsigned	start = getlast();

	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SequenceIndex = 1;
	if (SelectedForm->edgeType&EGUND) {
		LineSpacing = USPAC;
		satout(HorizontalLength2*URAT);
		sfn(start);
		StateMap.set(StateFlag::FILDIR);
		sfn(start);
	}
	fvars(ClosestFormToCursor);
	satout(HorizontalLength2);
	LineSpacing = SelectedForm->edgeSpacing;
	sfn(start);
	LineSpacing = spacing;
}

void satends(unsigned isBlunt) {
	fPOINT		step = {};

	if (isBlunt&SBLNT) {
		step.x = sin(FormAngles[0])*HorizontalLength2 / 2;
		step.y = cos(FormAngles[0])*HorizontalLength2 / 2;
		if (StateMap.test(StateFlag::INDIR)) {
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
	if (isBlunt&FBLNT) {
		step.x = sin(FormAngles[VertexCount - 2])*HorizontalLength2 / 2;
		step.y = cos(FormAngles[VertexCount - 2])*HorizontalLength2 / 2;
		if (StateMap.test(StateFlag::INDIR)) {
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
	unsigned	iVertex = 0;
	const double	spacing = LineSpacing;

	SequenceIndex = 0;
	if (SelectedForm->edgeType&EGUND) {
		HorizontalLength2 = SelectedForm->borderSize*URAT;
		satout(HorizontalLength2);
		satends(SelectedForm->attribute);
		StateMap.reset(StateFlag::SAT1);
		StateMap.reset(StateFlag::FILDIR);
		LineSpacing = USPAC;
		for (iVertex = 0; iVertex < gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1; iVertex++)
			sbfn(InsidePoints, iVertex, iVertex + 1);
		StateMap.flip(StateFlag::FILDIR);
		for (iVertex = SelectedForm->vertexCount - 1; iVertex != 0; iVertex--)
			sbfn(InsidePoints, iVertex, iVertex - 1);
	}
	HorizontalLength2 = SelectedForm->borderSize;
	satout(HorizontalLength2);
	satends(SelectedForm->attribute);
	LineSpacing = SelectedForm->edgeSpacing;
	StateMap.reset(StateFlag::SAT1);
	for (iVertex = 0; iVertex < gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1; iVertex++)
		sbfn(InsidePoints, iVertex, iVertex + 1);
	LineSpacing = spacing;
}

void satsbrd() {
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEANGSAT;
	if (UserFlagMap.test(UserFlag::DUND))
		SelectedForm->edgeType |= EGUND;
	bsizpar();
	SelectedForm->borderSize = BorderWidth;
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void satbrd() {
	unsigned	iForm = 0;

	if (filmsgs(FML_ANGS))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satsbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satsbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void lapbrd() {
	const double	savedStitchLength = UserStitchLength;
	unsigned	iVertex = 0;

	SequenceIndex = 0;
	// ToDo - Should APSPAC be a configurable variable instead?
	UserStitchLength = APSPAC;
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
		bdrlin(iVertex, iVertex + 1, APSPAC);
	for (iVertex = VertexCount - 1; iVertex != 0; iVertex--)
		bdrlin(iVertex, iVertex - 1, APSPAC);
	UserStitchLength = savedStitchLength;
}

void apbrd() {
	unsigned		iVertex = 0;
	unsigned short	nextVertex = 0, currentVertex = 0;

	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[currentVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[currentVertex].y;
	for (iVertex = 0; iVertex < VertexCount << 1; iVertex++) {
		nextVertex = nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, APSPAC);
		currentVertex = nextVertex;
	}
}

void sapliq() {
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEAPPL;
	if (UserFlagMap.test(UserFlag::DUND))
		SelectedForm->edgeType |= EGUND;
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderSize = IniFile.borderWidth;
	bsizpar();
	SelectedForm->borderColor = ActiveColor | (AppliqueColor << 4);
	if (SelectedForm->type != FRMLINE) {
		if (SelectedForm->fillType == SAT && SelectedForm->satinGuideCount)
			delsac(ClosestFormToCursor);
	}
	SelectedForm->fillType = 0;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void apliq() {
	unsigned iForm = 0;

	if (filmsgs(FML_APLQ))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			sapliq();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			sapliq();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void setap() {
	std::string fmtStr;
	loadString(fmtStr, IDS_APCOL);

	AppliqueColor = ActiveColor;
	shoMsg(fmt::format(fmtStr, (AppliqueColor + 1)));
}

void maxtsiz(const std::string &label, POINT &textSize) noexcept {
	SIZE	labelSize;

	GetTextExtentPoint32(StitchWindowMemDC, label.data(), label.size(), &labelSize);
	textSize.y = labelSize.cy;
	if (labelSize.cx > textSize.x)
		textSize.x = labelSize.cx;
}

void maxwid(unsigned start, unsigned finish) noexcept {
	POINT	textSize;

	textSize.x = 0;
	textSize.y = 0;
	while (start <= finish) {
		maxtsiz(StringTable->at(start++), textSize);
	}
	PreferenceWindowTextWidth = textSize.x + 6;
}

HWND txtwin(const std::string &windowName, RECT location) {
	if (StateMap.test(StateFlag::REFCNT)) {
		maxtsiz(windowName, LabelWindowSize);
		return 0;
	}
	return CreateWindow(
		"STATIC",
		windowName.c_str(),
		WS_CHILD | WS_VISIBLE,
		location.left,
		location.top,
		location.right - location.left,
		location.bottom - location.top,
		FormDataSheet,
		NULL,
		ThrEdInstance,
		NULL);
}

HWND txtrwin(std::string winName, RECT location) {
	if (StateMap.test(StateFlag::REFCNT)) {
		maxtsiz(winName, ValueWindowSize);
		return 0;
	}
	return CreateWindow(
		"STATIC",
		winName.c_str(),
		SS_NOTIFY | WS_BORDER | WS_CHILD | WS_VISIBLE,
		location.left,
		location.top,
		location.right - location.left,
		location.bottom - location.top,
		FormDataSheet,
		NULL,
		ThrEdInstance,
		NULL);
}

HWND numwin(std::string winName, RECT location) {
	if (StateMap.test(StateFlag::REFCNT)) {
		maxtsiz(winName, ValueWindowSize);
		return 0;
	}
	return CreateWindow(
		"STATIC",
		winName.c_str(),
		SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
		location.left,
		location.top,
		location.right - location.left,
		location.bottom - location.top,
		FormDataSheet,
		NULL,
		ThrEdInstance,
		NULL);
}

void nxtlin() noexcept {
	FormMenuEntryCount++;
	LabelWindowCoords.top += LabelWindowSize.y;
	LabelWindowCoords.bottom += LabelWindowSize.y;
	ValueWindowCoords.top += ValueWindowSize.y;
	ValueWindowCoords.bottom += ValueWindowSize.y;;
}

BOOL CALLBACK chenum(HWND hwnd, LPARAM lParam) noexcept {
	UNREFERENCED_PARAMETER(lParam);

	return DestroyWindow(hwnd);
}

void refrmfn() {
	std::string choice;
	unsigned	edgeFillType = 0, iEdge = 0;

	edgeFillType = SelectedForm->edgeType&NEGUND;
	if (edgeFillType >= EDGELAST) {
		edgeFillType = EDGELAST - 1;
	}
	iEdge = edgeFillType - 1;
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left = 3;
	LabelWindowCoords.right = 3 + LabelWindowSize.x;
	ValueWindowCoords.left = 6 + LabelWindowSize.x;
	ValueWindowCoords.right = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	LabelWindow[LFRM] = txtwin(StringTable->at(STR_TXT0), LabelWindowCoords);
	if (SelectedForm->type == FRMLINE)
		choice = StringTable->at(STR_EDG1);
	else
		choice = StringTable->at(STR_FREH);
	ValueWindow[LFRM] = txtrwin(choice, ValueWindowCoords);
	nxtlin();
	LabelWindow[LLAYR] = txtwin(StringTable->at(STR_TXT1), LabelWindowCoords);
	ValueWindow[LLAYR] = txtrwin(fmt::format("{}", ((SelectedForm->attribute&FRMLMSK) >> 1)), ValueWindowCoords);
	nxtlin();
	if (SelectedForm->type != FRMLINE) {
		LabelWindow[LCWLK] = txtwin(StringTable->at(STR_CWLK), LabelWindowCoords);
		if (SelectedForm->extendedAttribute&AT_CWLK)
			choice = StringTable->at(STR_ON);
		else
			choice = StringTable->at(STR_OFF);
		ValueWindow[LCWLK] = txtrwin(choice, ValueWindowCoords);
		nxtlin();
		LabelWindow[LWALK] = txtwin(StringTable->at(STR_WALK), LabelWindowCoords);
		if (SelectedForm->extendedAttribute&AT_WALK)
			choice = StringTable->at(STR_ON);
		else
			choice = StringTable->at(STR_OFF);
		ValueWindow[LWALK] = txtrwin(choice, ValueWindowCoords);
		nxtlin();
		LabelWindow[LUND] = txtwin(StringTable->at(STR_UND), LabelWindowCoords);
		if (SelectedForm->extendedAttribute&AT_UND)
			choice = StringTable->at(STR_ON);
		else
			choice = StringTable->at(STR_OFF);
		ValueWindow[LUND] = txtrwin(choice, ValueWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&(AT_WALK | AT_UND | AT_CWLK)) {
			LabelWindow[LUNDCOL] = txtwin(StringTable->at(STR_UNDCOL), LabelWindowCoords);
			ValueWindow[LUNDCOL] = txtrwin(fmt::format("{}", (SelectedForm->underlayColor + 1)), ValueWindowCoords);
			nxtlin();
			LabelWindow[LULEN] = txtwin(StringTable->at(STR_ULEN), LabelWindowCoords);
			ValueWindow[LULEN] = txtrwin(fmt::format("{:.2f}", (SelectedForm->underlayStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		LabelWindow[LWLKIND] = txtwin(StringTable->at(STR_UWLKIND), LabelWindowCoords);
		ValueWindow[LWLKIND] = txtrwin(fmt::format("{:.2f}", (SelectedForm->underlayIndent / PFGRAN)), ValueWindowCoords);
		nxtlin();
		if (SelectedForm->extendedAttribute&AT_UND) {
			LabelWindow[LUSPAC] = txtwin(StringTable->at(STR_FUSPAC), LabelWindowCoords);
			ValueWindow[LUSPAC] = txtrwin(fmt::format("{:.2f}", (SelectedForm->underlaySpacing / PFGRAN)), ValueWindowCoords);
			nxtlin();
			LabelWindow[LUANG] = txtwin(StringTable->at(STR_FUANG), LabelWindowCoords);
			ValueWindow[LUANG] = txtrwin(fmt::format("{:.2f}", (SelectedForm->underlayStitchAngle * 180 / PI)), ValueWindowCoords);
			nxtlin();
		}
	}
	LabelWindow[LFRMFIL] = txtwin(StringTable->at(STR_TXT2), LabelWindowCoords);
	ValueWindow[LFRMFIL] = txtrwin(StringTable->at(STR_FIL0 + SelectedForm->fillType), ValueWindowCoords);
	nxtlin();
	if (SelectedForm->fillType) {
		LabelWindow[LFRMCOL] = txtwin(StringTable->at(STR_TXT3), LabelWindowCoords);
		ValueWindow[LFRMCOL] = numwin(fmt::format("{}", (SelectedForm->fillColor + 1)), ValueWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == FTHF) {
			LabelWindow[LFTHCOL] = txtwin(StringTable->at(STR_FTHCOL), LabelWindowCoords);
			ValueWindow[LFTHCOL] = numwin(fmt::format("{}", (SelectedForm->fillInfo.feather.color + 1)), ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHTYP] = txtwin(StringTable->at(STR_FTHTYP), LabelWindowCoords);
			ValueWindow[LFTHTYP] = numwin(StringTable->at(STR_FTH0 + SelectedForm->fillInfo.feather.fillType - 1), ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHBLND] = txtwin(StringTable->at(STR_FTHBLND), LabelWindowCoords);
			if (SelectedForm->extendedAttribute&AT_FTHBLND)
				choice = StringTable->at(STR_ON);
			else
				choice = StringTable->at(STR_OFF);
			ValueWindow[LFTHBLND] = txtrwin(choice, ValueWindowCoords);
			nxtlin();
			if (!(SelectedForm->extendedAttribute&AT_FTHBLND)) {
				LabelWindow[LFTHBTH] = txtwin(StringTable->at(STR_FTHBOTH), LabelWindowCoords);
				if (SelectedForm->extendedAttribute&(AT_FTHBTH))
					choice = StringTable->at(STR_ON);
				else
					choice = StringTable->at(STR_OFF);
				ValueWindow[LFTHBTH] = txtrwin(choice, ValueWindowCoords);
				nxtlin();
				if (!(SelectedForm->extendedAttribute&AT_FTHBTH)) {
					LabelWindow[LFTHUP] = txtwin(StringTable->at(STR_FTHUP), LabelWindowCoords);
					if (SelectedForm->extendedAttribute&AT_FTHUP)
						choice = StringTable->at(STR_ON);
					else
						choice = StringTable->at(STR_OFF);
					ValueWindow[LFTHUP] = txtrwin(choice, ValueWindowCoords);
					nxtlin();
				}
			}
			LabelWindow[LFTHUPCNT] = txtwin(StringTable->at(STR_FTHUPCNT), LabelWindowCoords);
			ValueWindow[LFTHUPCNT] = numwin(fmt::format("{}", (SelectedForm->fillInfo.feather.upCount)), ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHDWNCNT] = txtwin(StringTable->at(STR_FTHDWNCNT), LabelWindowCoords);
			ValueWindow[LFTHDWNCNT] = numwin(fmt::format("{}", (SelectedForm->fillInfo.feather.downCount)), ValueWindowCoords);
			nxtlin();
			LabelWindow[LFTHSIZ] = txtwin(StringTable->at(STR_FTHSIZ), LabelWindowCoords);
			ValueWindow[LFTHSIZ] = numwin(fmt::format("{:.2f}", (SelectedForm->fillInfo.feather.ratio)), ValueWindowCoords);
			nxtlin();
			if (SelectedForm->fillInfo.feather.fillType == FTHPSG) {
				LabelWindow[LFTHNUM] = txtwin(StringTable->at(STR_FTHNUM), LabelWindowCoords);
				ValueWindow[LFTHNUM] = numwin(fmt::format("{}", (SelectedForm->fillInfo.feather.count)), ValueWindowCoords);
				nxtlin();
			}
			LabelWindow[LFTHFLR] = txtwin(StringTable->at(STR_FTHFLR), LabelWindowCoords);
			ValueWindow[LFTHFLR] = numwin(fmt::format("{:.2f}", (SelectedForm->fillInfo.feather.minStitchSize / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType != CLPF) {
			LabelWindow[LFRMSPAC] = txtwin(StringTable->at(STR_TXT4), LabelWindowCoords);
			ValueWindow[LFRMSPAC] = numwin(fmt::format("{:.2f}", (SelectedForm->fillSpacing / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (istx(ClosestFormToCursor)) {
			LabelWindow[LTXOF] = txtwin(StringTable->at(STR_TXOF), LabelWindowCoords);
			ValueWindow[LTXOF] = numwin(fmt::format("{:.2f}", (SelectedForm->txof / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		LabelWindow[LMAXFIL] = txtwin(StringTable->at(STR_TXT20), LabelWindowCoords);
		ValueWindow[LMAXFIL] = numwin(fmt::format("{:.2f}", (SelectedForm->maxFillStitchLen / PFGRAN)), ValueWindowCoords);
		nxtlin();
		if (!isclp(ClosestFormToCursor) && !istx(ClosestFormToCursor)) {
			LabelWindow[LFRMLEN] = txtwin(StringTable->at(STR_TXT5), LabelWindowCoords);
			ValueWindow[LFRMLEN] = numwin(fmt::format("{:.2f}", (SelectedForm->lengthOrCount.stitchLength / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		LabelWindow[LMINFIL] = txtwin(StringTable->at(STR_TXT21), LabelWindowCoords);
		ValueWindow[LMINFIL] = numwin(fmt::format("{:.2f}", (SelectedForm->minFillStitchLen / PFGRAN)), ValueWindowCoords);
		nxtlin();
		if (SelectedForm->fillType == ANGF || SelectedForm->fillType == TXANGF) {
			LabelWindow[LFRMANG] = txtwin(StringTable->at(STR_TXT6), LabelWindowCoords);
			ValueWindow[LFRMANG] = numwin(fmt::format("{:.2f}", (SelectedForm->angleOrClipData.angle * 180 / PI)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == ANGCLPF) {
			LabelWindow[LSACANG] = txtwin(StringTable->at(STR_TXT6), LabelWindowCoords);
			ValueWindow[LSACANG] = numwin(fmt::format("{:.2f}", (SelectedForm->satinOrAngle.angle * 180 / PI)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VCLPF || SelectedForm->fillType == HCLPF || SelectedForm->fillType == ANGCLPF) {
			LabelWindow[LFRMFAZ] = txtwin(StringTable->at(STR_TXT18), LabelWindowCoords);
			ValueWindow[LFRMFAZ] = numwin(fmt::format("{}", (SelectedForm->wordParam)), ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->fillType == VRTF || SelectedForm->fillType == HORF || SelectedForm->fillType == ANGF || istx(ClosestFormToCursor)) {
			LabelWindow[LBFILSQR] = txtwin(StringTable->at(STR_PRF2), LabelWindowCoords);
			if (SelectedForm->extendedAttribute&AT_SQR)
				choice = StringTable->at(STR_SQR);
			else
				choice = StringTable->at(STR_PNTD);
			ValueWindow[LBFILSQR] = txtrwin(choice, ValueWindowCoords);
			nxtlin();
		}
	}
	LabelWindow[LFSTRT] = txtwin(StringTable->at(STR_FSTRT), LabelWindowCoords);
	if (SelectedForm->extendedAttribute&AT_STRT)
		choice = StringTable->at(STR_ON);
	else
		choice = StringTable->at(STR_OFF);
	ValueWindow[LFSTRT] = txtrwin(choice, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_STRT) {
		LabelWindow[LDSTRT] = txtwin(StringTable->at(STR_FSTRT), LabelWindowCoords);
		ValueWindow[LDSTRT] = numwin(fmt::format("{}", (SelectedForm->fillStart)), ValueWindowCoords);
		nxtlin();
	}
	LabelWindow[LFEND] = txtwin(StringTable->at(STR_FEND), LabelWindowCoords);
	if (SelectedForm->extendedAttribute&AT_END)
		choice = StringTable->at(STR_ON);
	else
		choice = StringTable->at(STR_OFF);
	ValueWindow[LFEND] = txtrwin(choice, ValueWindowCoords);
	nxtlin();
	if (SelectedForm->extendedAttribute&AT_END) {
		LabelWindow[LDEND] = txtwin(StringTable->at(STR_FEND), LabelWindowCoords);
		ValueWindow[LDEND] = numwin(fmt::format("{}", (SelectedForm->fillEnd)), ValueWindowCoords);
		nxtlin();
	}
	LabelWindow[LBRD] = txtwin(StringTable->at(STR_TXT7), LabelWindowCoords);
	ValueWindow[LBRD] = txtrwin(StringTable->at(STR_EDG0 + edgeFillType), ValueWindowCoords);
	nxtlin();
	if (edgeFillType) {
		LabelWindow[LBRDCOL] = txtwin(StringTable->at(STR_TXT8), LabelWindowCoords);
		ValueWindow[LBRDCOL] = numwin(fmt::format("{}", ((SelectedForm->borderColor & COLMSK) + 1)), ValueWindowCoords);
		nxtlin();
		if (EdgeArray[iEdge] & BESPAC) {
			LabelWindow[LBRDSPAC] = txtwin(StringTable->at(STR_TXT9), LabelWindowCoords);
			if (edgeFillType == EDGEPROPSAT || edgeFillType == EDGEOCHAIN || edgeFillType == EDGELCHAIN)
				choice = fmt::format("{:.2f}", (SelectedForm->edgeSpacing / PFGRAN));
			else
				choice = fmt::format("{:.2f}", (SelectedForm->edgeSpacing / PFGRAN * 2));
			ValueWindow[LBRDSPAC] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BPICSPAC) {
			LabelWindow[LBRDPIC] = txtwin(StringTable->at(STR_TXT16), LabelWindowCoords);
			ValueWindow[LBRDPIC] = numwin(fmt::format("{:.2f}", (SelectedForm->edgeSpacing / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BEMAX) {
			LabelWindow[LMAXBRD] = txtwin(StringTable->at(STR_TXT22), LabelWindowCoords);
			ValueWindow[LMAXBRD] = numwin(fmt::format("{:.2f}", (SelectedForm->maxBorderStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BELEN) {
			LabelWindow[LBRDLEN] = txtwin(StringTable->at(STR_TXT10), LabelWindowCoords);
			ValueWindow[LBRDLEN] = numwin(fmt::format("{:.2f}", (SelectedForm->edgeStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BEMIN) {
			LabelWindow[LMINBRD] = txtwin(StringTable->at(STR_TXT23), LabelWindowCoords);
			ValueWindow[LMINBRD] = numwin(fmt::format("{:.2f}", (SelectedForm->minBorderStitchLen / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BESIZ) {
			LabelWindow[LBRDSIZ] = txtwin(StringTable->at(STR_TXT11), LabelWindowCoords);
			ValueWindow[LBRDSIZ] = numwin(fmt::format("{:.2f}", (SelectedForm->borderSize / PFGRAN)), ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BRDPOS) {
			LabelWindow[LBRDPOS] = txtwin(StringTable->at(STR_TXT18), LabelWindowCoords);
			ValueWindow[LBRDPOS] = numwin(fmt::format("{:.2f}", (SelectedForm->edgeStitchLen)), ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & CHNPOS) {
			LabelWindow[LBRDPOS] = txtwin(StringTable->at(STR_TXT19), LabelWindowCoords);
			ValueWindow[LBRDPOS] = numwin(fmt::format("{:.2f}", (SelectedForm->edgeStitchLen)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeFillType == EDGEAPPL) {
			LabelWindow[LAPCOL] = txtwin(StringTable->at(STR_TXT12), LabelWindowCoords);
			ValueWindow[LAPCOL] = numwin(fmt::format("{}", ((SelectedForm->borderColor >> 4) + 1)), ValueWindowCoords);
			nxtlin();
		}
		if (edgeFillType == EDGEANGSAT || edgeFillType == EDGEAPPL || edgeFillType == EDGEPROPSAT) {
			LabelWindow[LBRDUND] = txtwin(StringTable->at(STR_TXT17), LabelWindowCoords);
			if (SelectedForm->edgeType&EGUND)
				choice = StringTable->at(STR_ON);
			else
				choice = StringTable->at(STR_OFF);
			ValueWindow[LBRDUND] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
		if (EdgeArray[iEdge] & BCNRSIZ) {
			if (SelectedForm->edgeType == EDGEBHOL)
				choice = fmt::format("{:.2f}", (getblen() / PFGRAN));
			else
				choice = fmt::format("{:.2f}", (getplen() / PFGRAN));
			LabelWindow[LBCSIZ] = txtwin(StringTable->at(STR_TXT13), LabelWindowCoords);
			ValueWindow[LBCSIZ] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
		if (SelectedForm->type == FRMLINE && EdgeArray[iEdge] & BRDEND) {
			LabelWindow[LBSTRT] = txtwin(StringTable->at(STR_TXT14), LabelWindowCoords);
			if (SelectedForm->attribute&SBLNT)
				choice = StringTable->at(STR_BLUNT);
			else
				choice = StringTable->at(STR_TAPR);
			ValueWindow[LBSTRT] = numwin(choice, ValueWindowCoords);
			nxtlin();
			LabelWindow[LBFIN] = txtwin(StringTable->at(STR_TXT15), LabelWindowCoords);
			if (SelectedForm->attribute&FBLNT)
				choice = StringTable->at(STR_BLUNT);
			else
				choice = StringTable->at(STR_TAPR);
			ValueWindow[LBFIN] = numwin(choice, ValueWindowCoords);
			nxtlin();
		}
	}
}

void refrm() {
	SelectedForm = &FormList[ClosestFormToCursor];
	if (StateMap.testAndReset(StateFlag::PRFACT)) {
		DestroyWindow(PreferencesWindow);
		StateMap.reset(StateFlag::WASRT);
	}
	LabelWindowSize.x = LabelWindowSize.y = ValueWindowSize.x = ValueWindowSize.y = 0;
	StateMap.set(StateFlag::REFCNT);
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
	StateMap.reset(StateFlag::REFCNT);
	refrmfn();
}

void pxrct2stch(RECT screenRect, fRECTANGLE* stitchRect) {
	POINT	corner;

	corner.x = screenRect.left + StitchWindowOrigin.x;
	corner.y = screenRect.top + StitchWindowOrigin.y;
	pxCor2stch(corner);
	stitchRect->left = SelectedPoint.x;
	stitchRect->top = SelectedPoint.y;
	corner.x = screenRect.right + StitchWindowOrigin.x;
	corner.y = screenRect.bottom + StitchWindowOrigin.y;
	pxCor2stch(corner);
	stitchRect->right = SelectedPoint.x;
	stitchRect->bottom = SelectedPoint.y;
}

unsigned pdir(unsigned vertex) {
	if (StateMap.test(StateFlag::PSELDIR))
		return nxt(vertex);
	else
		return prv(vertex);
}

void setstrtch() {
	FLOAT		reference = 0.0;
	double		ratio = 1.0;
	unsigned	iForm = 0, iVertex = 0, iStitch = 0, currentVertex = 0, currentForm = 0;
	fRECTANGLE	stitchRect = {};
	long		offsetY = 0, offsetX = 0;

	savdo();
	if (StateMap.test(StateFlag::FPSEL))
		MoveMemory(&stitchRect, &SelectedPointsLine, sizeof(fRECTANGLE));
	else {
		if (SelectedFormCount || StateMap.test(StateFlag::BIGBOX))
			pxrct2stch(SelectedFormsRect, &stitchRect);
		else {
			fvars(ClosestFormToCursor);
			px2stch();
			stitchRect.bottom = stitchRect.left = stitchRect.right = stitchRect.top = 0;
		}
	}
	switch (SelectedFormControlVertex) {
	case 0:

		if (SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.bottom;
			offsetY = Msg.pt.y - StitchWindowOrigin.y;
			ratio = static_cast<double>(SelectedFormsRect.bottom - offsetY) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.bottom;
				ratio = static_cast<double>(SelectedPoint.y - reference) / (SelectedForm->rectangle.top - reference);
				SelectedForm->rectangle.top = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.bottom;
				ratio = static_cast<double>(SelectedPoint.y - reference) / (StitchRangeRect.top - reference);
			}
		}
		break;

	case 1:

		if (SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.left;
			offsetX = Msg.pt.x - StitchWindowOrigin.x;
			ratio = static_cast<double>(offsetX - SelectedFormsRect.left) / (SelectedFormsRect.right - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.left;
				ratio = static_cast<double>(SelectedPoint.x - reference) / (SelectedForm->rectangle.right - reference);
				SelectedForm->rectangle.right = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.left;
				ratio = static_cast<double>(SelectedPoint.x - reference) / (StitchRangeRect.right - reference);
			}
		}
		break;

	case 2:

		if (SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.top;
			offsetY = Msg.pt.y - StitchWindowOrigin.y;
			ratio = static_cast<double>(offsetY - SelectedFormsRect.top) / (SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.top;
				ratio = static_cast<double>(SelectedPoint.y - reference) / (SelectedForm->rectangle.bottom - reference);
				SelectedForm->rectangle.bottom = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.top;
				ratio = static_cast<double>(SelectedPoint.y - reference) / (StitchRangeRect.bottom - reference);
			}
		}
		break;

	case 3:

		if (SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference = stitchRect.right;
			offsetX = Msg.pt.x - StitchWindowOrigin.x;
			ratio = static_cast<double>(SelectedFormsRect.right - offsetX) / (SelectedFormsRect.right - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.right;
				ratio = static_cast<double>(SelectedPoint.x - reference) / (SelectedForm->rectangle.left - reference);
				SelectedForm->rectangle.left = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.right;
				ratio = static_cast<double>(SelectedPoint.x - reference) / (StitchRangeRect.left - reference);
			}
		}
		break;
	}
	if (SelectedFormControlVertex & 1) {
		if (StateMap.test(StateFlag::FPSEL)) {
			fvars(ClosestFormToCursor);
			currentVertex = SelectedFormVertices.start;
			for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
				CurrentFormVertices[currentVertex].x = (CurrentFormVertices[currentVertex].x - reference)*ratio + reference;
				currentVertex = pdir(currentVertex);
			}
			frmout(ClosestFormToCursor);
			setpsel();
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			for (iForm = 0; iForm < FormIndex; iForm++) {
				CurrentFormVertices = FormList[iForm].vertices;
				for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++)
					CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference)*ratio + reference;
				frmout(iForm);
			}
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
				StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference)*ratio + reference;
			selal();
			return;
		}
		else {
			if (SelectedFormCount) {
				for (iForm = 0; iForm < SelectedFormCount; iForm++) {
					CurrentFormVertices = FormList[SelectedFormList[iForm]].vertices;
					for (iVertex = 0; iVertex < FormList[SelectedFormList[iForm]].vertexCount; iVertex++)
						CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference)*ratio + reference;
				}
			}
			else {
				if (StateMap.test(StateFlag::FORMSEL)) {
					for (iVertex = 0; iVertex < VertexCount; iVertex++)
						CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference)*ratio + reference;
				}
				else {
					for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
						StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference)*ratio + reference;
				}
			}
		}
	}
	else {
		if (StateMap.test(StateFlag::FPSEL)) {
			fvars(ClosestFormToCursor);
			currentVertex = SelectedFormVertices.start;
			for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
				CurrentFormVertices[currentVertex].y = (CurrentFormVertices[currentVertex].y - reference)*ratio + reference;
				currentVertex = pdir(currentVertex);
			}
			frmout(ClosestFormToCursor);
			setpsel();
			refil();
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			for (iForm = 0; iForm < FormIndex; iForm++) {
				CurrentFormVertices = FormList[iForm].vertices;
				for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++)
					CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference)*ratio + reference;
				frmout(iForm);
			}
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
				StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference)*ratio + reference;
			selal();
			return;
		}
		else {
			if (SelectedFormCount) {
				for (iForm = 0; iForm < SelectedFormCount; iForm++) {
					CurrentFormVertices = FormList[SelectedFormList[iForm]].vertices;
					for (iVertex = 0; iVertex < FormList[SelectedFormList[iForm]].vertexCount; iVertex++)
						CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference)*ratio + reference;
				}
			}
			else {
				if (StateMap.test(StateFlag::FORMSEL)) {
					for (iVertex = 0; iVertex < VertexCount; iVertex++)
						CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference)*ratio + reference;
				}
				else {
					for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
						StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference)*ratio + reference;
				}
			}
		}
	}
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			currentForm = SelectedFormList[iForm];
			frmout(currentForm);
			ClosestFormToCursor = currentForm;
			refil();
		}
	}
	else
		if (StateMap.test(StateFlag::FORMSEL))
			refil();
	StateMap.set(StateFlag::RESTCH);
}

void setexpand() {
	dPOINT		reference = {};
	POINT		integerReference = {};
	fPOINT		stitchReference = {};
	dPOINT		size0 = {};
	dPOINT		size1 = {};
	dPOINT		ratio = {};
	double		aspect = 0.0;
	unsigned	iVertex = 0, iForm = 0, iStitch = 0, iCurrent = 0;
	fRECTANGLE	rectangle = {};

	savdo();
	if (SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		rectangle.bottom = SelectedFormsRect.bottom;
		rectangle.left = SelectedFormsRect.left;
		rectangle.right = SelectedFormsRect.right;
		rectangle.top = SelectedFormsRect.top;
		SelectedPoint.x = Msg.pt.x - StitchWindowOrigin.x;
		SelectedPoint.y = Msg.pt.y - StitchWindowOrigin.y;
		size0.y = rectangle.bottom - rectangle.top;
	}
	else {
		px2stch();
		fvars(ClosestFormToCursor);
		if (StateMap.test(StateFlag::FORMSEL))
			rectangle = SelectedForm->rectangle;
		else {
			rectangle.bottom = StitchRangeRect.bottom;
			rectangle.top = StitchRangeRect.top;
			rectangle.right = StitchRangeRect.right;
			rectangle.left = StitchRangeRect.left;
		}
		size0.y = rectangle.top - rectangle.bottom;
	}
	ratio.x = ratio.y = 1;
	reference.x = reference.y = 0;
	size0.x = rectangle.right - rectangle.left;
	switch (SelectedFormControlVertex) {
	case 0:

		reference.x = rectangle.right;
		reference.y = rectangle.bottom;
		size1.x = fabs(SelectedPoint.x - reference.x);
		size1.y = fabs(SelectedPoint.y - reference.y);
		aspect = size1.x / size1.y;
		if (aspect < XYratio)
			size1.x = size1.y*XYratio;
		else
			size1.y = size1.x / XYratio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormCount && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.left = rectangle.right - size1.x;
			SelectedForm->rectangle.top = rectangle.bottom + size1.y;
		}
		break;

	case 1:

		reference.x = rectangle.left;
		reference.y = rectangle.bottom;
		size1.x = fabs(SelectedPoint.x - reference.x);
		size1.y = fabs(SelectedPoint.y - reference.y);
		aspect = size1.x / size1.y;
		if (aspect < XYratio)
			size1.x = size1.y*XYratio;
		else
			size1.y = size1.x / XYratio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormCount && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.right = rectangle.left + size1.x;
			SelectedForm->rectangle.top = rectangle.bottom + size1.y;
		}
		break;

	case 2:

		reference.x = rectangle.left;
		reference.y = rectangle.top;
		size1.x = fabs(SelectedPoint.x - reference.x);
		size1.y = fabs(SelectedPoint.y - reference.y);
		aspect = size1.x / size1.y;
		if (aspect < XYratio)
			size1.x = size1.y*XYratio;
		else
			size1.y = size1.x / XYratio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormCount && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.right = rectangle.left + size1.x;
			SelectedForm->rectangle.bottom = rectangle.top - size1.y;
		}
		break;

	case 3:

		reference.x = rectangle.right;
		reference.y = rectangle.top;
		size1.x = fabs(SelectedPoint.x - reference.x);
		size1.y = fabs(SelectedPoint.y - reference.y);
		aspect = size1.x / size1.y;
		if (aspect < XYratio)
			size1.x = size1.y*XYratio;
		else
			size1.y = size1.x / XYratio;
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (!SelectedFormCount && StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->rectangle.left = rectangle.right - size1.x;
			SelectedForm->rectangle.bottom = rectangle.top - size1.y;
		}
		break;
	}
	integerReference.x = reference.x;
	integerReference.y = reference.y;
	px2stchf(integerReference, &stitchReference);
	if (StateMap.test(StateFlag::FPSEL)) {
		fvars(ClosestFormToCursor);
		iCurrent = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			CurrentFormVertices[iCurrent].x = (CurrentFormVertices[iCurrent].x - stitchReference.x)*ratio.x + stitchReference.x;
			CurrentFormVertices[iCurrent].y = (CurrentFormVertices[iCurrent].y - stitchReference.y)*ratio.y + stitchReference.y;
			iCurrent = pdir(iCurrent);
		}
		setpsel();
		frmout(ClosestFormToCursor);
		refil();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		for (iForm = 0; iForm < FormIndex; iForm++) {
			fvars(iForm);
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
				CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - stitchReference.x)*ratio.x + stitchReference.x;
				CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - stitchReference.y)*ratio.y + stitchReference.y;
			}
			frmout(iForm);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - stitchReference.x)*ratio.x + stitchReference.x;
			StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - stitchReference.y)*ratio.y + stitchReference.y;
		}
		selal();
		return;
	}
	else {
		if (SelectedFormCount) {
			for (iForm = 0; iForm < SelectedFormCount; iForm++) {
				fvars(SelectedFormList[iForm]);
				for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
					CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - stitchReference.x)*ratio.x + stitchReference.x;
					CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - stitchReference.y)*ratio.y + stitchReference.y;
				}
				frmout(SelectedFormList[iForm]);
				ClosestFormToCursor = SelectedFormList[iForm];
				refil();
			}
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				for (iVertex = 0; iVertex < VertexCount; iVertex++) {
					CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference.x)*ratio.x + reference.x;
					CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference.y)*ratio.y + reference.y;
				}
				refil();
			}
			else {
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference.x)*ratio.x + reference.x;
					StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference.y)*ratio.y + reference.y;
				}
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void nufilcol(unsigned color) noexcept {
	unsigned	attribute = 0, iStitch = 0;

	if (SelectedForm->fillColor != color) {
		SelectedForm->fillColor = color;
		attribute = (ClosestFormToCursor << 4) | FRMFIL;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute&(FRMSK | TYPMSK | FTHMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void nufthcol(unsigned color) noexcept {
	unsigned	attribute = 0, iStitch = 0;

	if (SelectedForm->fillInfo.feather.color != color) {
		SelectedForm->fillInfo.feather.color = color;
		attribute = (ClosestFormToCursor << 4) | FTHMSK;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute&(FRMSK | FTHMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void nubrdcol(unsigned color) noexcept {
	unsigned	attribute = 0, iStitch = 0;

	SelectedForm->borderColor = color;
	attribute = (ClosestFormToCursor << 4) | FRMBFIL;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute&(FRMSK | TYPMSK)) == attribute) {
			StitchBuffer[iStitch].attribute &= 0xfffffff0;
			StitchBuffer[iStitch].attribute |= color;
		}
	}
}

void nulapcol(unsigned color) {
	unsigned	attribute = 0, iStitch = 0;

	if (gsl::narrow<unsigned>(SelectedForm->borderColor >> 4) != color) {
		SelectedForm->borderColor &= COLMSK;
		SelectedForm->borderColor |= color << 4;
		attribute = (ClosestFormToCursor << 4) | TYPMSK;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute&(TYPMSK | FRMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void sidwnd(HWND wnd) {
	RECT		windowRect = {};
	unsigned	savedChoice = 0;

	MsgIndex = 0;
	SideWindowEntryBuffer[0] = 0;
	savedChoice = FormMenuChoice;
	unsid();
	FormMenuChoice = savedChoice;
	GetWindowRect(wnd, &windowRect);
	GetWindowRect(FormDataSheet, &MsgRect);
	SideMessageWindow = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		MsgRect.right - ThredWindowOrigin.x + 3,
		windowRect.top - ThredWindowOrigin.y - 3,
		ButtonWidthX3,
		windowRect.bottom - windowRect.top + 3,
		ThrEdWindow,
		NULL,
		ThrEdInstance,
		NULL);
}

void prfsid(HWND wnd) {
	RECT	windowRect = {};

	MsgIndex = 0;
	SideWindowEntryBuffer[0] = 0;
	unsid();
	GetWindowRect(wnd, &windowRect);
	GetClientRect(PreferencesWindow, &MsgRect);
	SideMessageWindow = CreateWindow(
		"STATIC",
		0,
		WS_BORDER | WS_CHILD | WS_VISIBLE,
		windowRect.right - ThredWindowOrigin.x + 6,
		windowRect.top - ThredWindowOrigin.y - 3,
		ValueWindowSize.x,
		windowRect.bottom - windowRect.top + 3,
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
	unsigned	iForm = 0;

	if (filmsgs(FML_BLD))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			sbold();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			sbold();
			coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
			ritot(PCSHeader.stitchCount);
		}
	}
}

void prftwin(const std::string &text) noexcept {
	CreateWindow(
		"STATIC",
		text.c_str(),
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

HWND prfnwin(const std::string &text) noexcept {
	return CreateWindow(
		"STATIC",
		text.c_str(),
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

void prflin(std::string msg, unsigned row) noexcept {
	prftwin(StringTable->at(row));
	ValueWindow[row - STR_PRF0] = prfnwin(msg);
	nxtlin();
}

void sethup() noexcept {
	if (IniFile.hoopSizeX == LHUPX && IniFile.hoopSizeY == LHUPY) {
		IniFile.hoopType = LARGHUP;
		return;
	}
	if (IniFile.hoopSizeX == SHUPX && IniFile.hoopSizeY == SHUPY) {
		IniFile.hoopType = SMALHUP;
		return;
	}
	if (IniFile.hoopSizeX == HUP100XY && IniFile.hoopSizeY == HUP100XY) {
		IniFile.hoopType = HUP100;
		return;
	}
	IniFile.hoopType = CUSTHUP;
}

#if LANG==ENG || LANG==HNG

void prfmsg() {
	std::string choice;
	HDC		preferenceDC = {};
	RECT	preferenceRect = {};

	if (StateMap.testAndReset(StateFlag::INSRT))
		StateMap.set(StateFlag::WASRT);
	StateMap.reset(StateFlag::BIGBOX);
	SelectedFormCount = 0;
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	LabelWindowSize.x = LabelWindowSize.y = 0;
	ValueWindowSize.x = ValueWindowSize.y = 0;
	maxtsiz(StringTable->at(STR_PRF0 + 4), LabelWindowSize);
	maxtsiz(StringTable->at(STR_TAPR), ValueWindowSize);
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
	preferenceDC = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &preferenceRect);
	FillRect(preferenceDC, &preferenceRect, GetSysColorBrush(COLOR_WINDOW));
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left = 3;
	LabelWindowCoords.right = 3 + LabelWindowSize.x;
	ValueWindowCoords.left = 6 + LabelWindowSize.x;
	ValueWindowCoords.right = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	prflin(fmt::format("{}",(AppliqueColor + 1)), STR_PRF10);
	prflin(fmt::format("{:.2f}", (BorderWidth / PFGRAN)), STR_PRF3);
	prflin(fmt::format("{:.2f}", (ButtonholeCornerLength / PFGRAN)), STR_PRF14);
	prflin(fmt::format("{:.2f}", (IniFile.chainSpace / PFGRAN)), STR_PRF23);
	prflin(fmt::format("{:.2f}", (IniFile.chainRatio)), STR_PRF24);
	prflin(fmt::format("{:.2f} mm", (IniFile.clipOffset / PFGRAN)), STR_PRF21);
	prflin(fmt::format("{}", (IniFile.fillPhase)), STR_PRF22);
	prflin(fmt::format("{:.2f}", (IniFile.eggRatio)), STR_PRF26);
	prflin(fmt::format("{:.2f}", (IniFile.fillAngle / PI * 180)), STR_PRF1);
	if (UserFlagMap.test(UserFlag::SQRFIL))
		choice = StringTable->at(STR_SQR);
	else
		choice = StringTable->at(STR_PNTD);
	prflin(choice, STR_PRF2);
	prflin(fmt::format("{:.2f}", (LineSpacing / PFGRAN)), STR_PRF0);
	prflin(fmt::format("{}", (duthrsh(ShowStitchThreshold))), STR_PRF7);
	prflin(fmt::format("{:.2f} mm", (IniFile.gridSize / PFGRAN)), STR_PRF20);
	sethup();
	prflin(fmt::format("{}", StringTable->at(STR_HUP0 + IniFile.hoopType - 1)), STR_PRF17);
	prflin(fmt::format("{:.0f} mm", (IniFile.hoopSizeY / PFGRAN)), STR_PRF27);
	prflin(fmt::format("{:.0f} mm", (IniFile.hoopSizeX / PFGRAN)), STR_PRF18);
	prflin(fmt::format("{:.2f}", (IniFile.cursorNudgeStep)), STR_PRF25);
	prflin(fmt::format("{:.2f}", (PicotSpacing / PFGRAN)), STR_PRF16);
	if (UserFlagMap.test(UserFlag::BLUNT))
		choice = StringTable->at(STR_BLUNT);
	else
		choice = StringTable->at(STR_TAPR);
	prflin(choice, STR_PRF15);
	if (UserFlagMap.test(UserFlag::DUND))
		choice = StringTable->at(STR_ON);
	else
		choice = StringTable->at(STR_OFF);
	prflin(choice, STR_PRF19);
	prflin(fmt::format("{:.2f}", (SmallStitchLength / PFGRAN)), STR_PRF9);
	prflin(fmt::format("{:.2f}", (SnapLength / PFGRAN)), STR_PRF11);
	prflin(fmt::format("{:.2f}", (SpiralWrap)), STR_PRF13);
	prflin(fmt::format("{:.2f}", (StarRatio)), STR_PRF12);
	prflin(fmt::format("{}", (duthrsh(StitchBoxesThreshold))), STR_PRF8);
	prflin(fmt::format("{:.2f}", (IniFile.maxStitchLength / PFGRAN)), STR_PRF4);
	prflin(fmt::format("{:.2f}", (UserStitchLength / PFGRAN)), STR_PRF5);
	prflin(fmt::format("{:.2f}", (MinStitchLength / PFGRAN)), STR_PRF6);
	StateMap.set(StateFlag::PRFACT);
	ReleaseDC(ThrEdWindow, preferenceDC);
}
#endif

#if LANG==GRM

void prfmsg() {
	HDC		preferenceDC;
	RECT	preferenceRect;

	if (StateMap.testAndReset(StateFlag::INSRT))
		StateMap.set(StateFlag::WASRT);
	StateMap.reset(StateFlag::BIGBOX);
	SelectedFormCount = 0;
	if (FormDataSheet) {
		undat();
		unsid();
		FormMenuChoice = 0;
	}
	LabelWindowSize.x = LabelWindowSize.y = 0;
	ValueWindowSize.x = ValueWindowSize.y = 0;
	maxtsiz(StringTable->at(STR_PRF0 + 4), &LabelWindowSize);
	maxtsiz(StringTable->at(STR_BLUNT), &ValueWindowSize);
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
	preferenceDC = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &preferenceRect);
	FillRect(preferenceDC, &preferenceRect, GetSysColorBrush(COLOR_WINDOW));
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
	if (UserFlagMap.test(UserFlag::SQRFIL))
		strcpy_s(MsgBuffer, StringTable->at(STR_SQR]);
	else
		strcpy_s(MsgBuffer, StringTable->at(STR_PNTD]);
	prflin(STR_PRF2);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%.2f", BorderWidth / PFGRAN);
	prflin(STR_PRF3);
	if (UserFlagMap.test(UserFlag::BLUNT))
		strcpy_s(MsgBuffer, StringTable->at(STR_BLUNT]);
	else
		strcpy_s(MsgBuffer, StringTable->at(STR_TAPR]);
	prflin(STR_PRF15);
	if (UserFlagMap.test(UserFlag::DUND))
		strcpy_s(MsgBuffer, StringTable->at(STR_ON]);
	else
		strcpy_s(MsgBuffer, StringTable->at(STR_OFF]);
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
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), StringTable->at(STR_HUP0 + IniFile.hoopType - 1]);
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
	StateMap.set(StateFlag::PRFACT);
	ReleaseDC(ThrEdWindow, preferenceDC);
}
#endif

void durpoli(unsigned vertexCount) {
	double		stepAngle = 0.0;
	double		angle = 0.0;
	double		length = 0.0;
	unsigned	iVertex = 0;
	dPOINT		point = {};

	if (vertexCount < 3)
		vertexCount = 3;
	if (vertexCount > 100)
		vertexCount = 100;
	stepAngle = PI * 2 / vertexCount;
	// ToDo - why 500?
	length = 500 / vertexCount * ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(vertexCount);
	SelectedForm->vertexCount = vertexCount;
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	SelectedForm->type = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void dustar(unsigned starCount, double length) {
	double		stepAngle = 0.0;
	double		angle = 0.0;
	unsigned	iVertex = 0, vertexCount = 0;
	dPOINT		point = {}, center = {};

	if (starCount < 3)
		starCount = 3;
	if (starCount > 100)
		starCount = 100;
	stepAngle = PI / starCount;
	angle = stepAngle / 2 + PI;
	vertexCount = starCount << 1;
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(vertexCount);
	SelectedForm->vertexCount = vertexCount;
	SelectedForm->attribute = (ActiveLayer << 1);
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	StateMap.set(StateFlag::FILDIR);
	for (iVertex = 0; iVertex < vertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	center.x = (CurrentFormVertices[starCount].x - CurrentFormVertices[0].x) / 2 + CurrentFormVertices[0].x;
	center.y = (CurrentFormVertices[starCount].y - CurrentFormVertices[0].y) / 2 + CurrentFormVertices[0].y;
	for (iVertex = 1; iVertex < vertexCount; iVertex += 2) {
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - center.x)*StarRatio + center.x;
		CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - center.y)*StarRatio + center.y;
	}
	SelectedForm->type = FRMFPOLY;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void duspir(unsigned stepCount) {
	double		stepAngle = 0.0;
	double		angle = 0.0;
	double		length = 0.0, stepRatio = 0.0, ratio = 0.0;
	unsigned	iStep = 0, iVertex = 0, vertexCount = 0;
	dPOINT		point = {};
	dPOINT		center = {};

	if (stepCount < 3)
		stepCount = 3;
	if (stepCount > 100)
		stepCount = 100;
	stepAngle = PI * 2 / stepCount;
	// ToDo - Why 800?
	length = 800 / stepCount * ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	vertexCount = stepCount * SpiralWrap;
	SelectedForm->vertices = adflt(vertexCount);
	std::vector<fPOINT> firstSpiral(stepCount);
	std::vector<fPOINT> centeredSpiral(stepCount);
	SelectedForm->vertexCount = vertexCount;
	SelectedForm->attribute = (ActiveLayer << 1);
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	for (iStep = 0; iStep < stepCount; iStep++) {
		firstSpiral[iStep].x = point.x;
		firstSpiral[iStep].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	center.x = (firstSpiral[stepCount >> 1].x - firstSpiral[0].x) / 2 + firstSpiral[0].x;
	center.y = (firstSpiral[stepCount >> 1].y - firstSpiral[0].y) / 2 + firstSpiral[0].y;
	for (iStep = 0; iStep < stepCount; iStep++) {
		centeredSpiral[iStep].x = firstSpiral[iStep].x - center.x;
		centeredSpiral[iStep].y = firstSpiral[iStep].y - center.y;
	}
	stepRatio = 1.0 / vertexCount;
	ratio = stepRatio;
	for (iVertex = 0; iVertex < vertexCount; iVertex++) {
		SelectedForm->vertices[iVertex].x = centeredSpiral[iVertex%stepCount].x*ratio + center.x;
		SelectedForm->vertices[iVertex].y = centeredSpiral[iVertex%stepCount].y*ratio + center.y;
		ratio += stepRatio;
	}
	SelectedForm->type = FRMLINE;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void duhart(unsigned sideCount) {
	double		stepAngle = 0.0;
	double		angle = 0.0;
	double		length = 0.0;
	unsigned	iVertex = 0, lastVertex = 0, firstVertex = 0, iDestination = 0;
	float		maximumX = 0.0;
	dPOINT		point = {};
	double		ratio = 0.0;

	if (sideCount > 100)
		sideCount = 100;
	if (sideCount < 6)
		sideCount = 6;
	SelectedForm = &FormList[FormIndex];
	frmclr(SelectedForm);
	SelectedForm->attribute = ActiveLayer << 1;
	CurrentFormVertices = &FormVertices[FormVertexIndex];
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	stepAngle = PI * 2 / sideCount;
	length = 300 / sideCount * ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	angle = PI * 0.28;
	iVertex = 0;
	maximumX = 0;
	while (angle > -PI * 0.7) {
		if (point.x > maximumX)
			maximumX = point.x;
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex++].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	stepAngle /= 4.5;
	lastVertex = iVertex;
	while (point.x > CurrentFormVertices[0].x && iVertex < 200) {
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex++].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	firstVertex = iVertex;
	ratio = (CurrentFormVertices[lastVertex - 1].x - CurrentFormVertices[0].x) / (CurrentFormVertices[lastVertex - 1].x - CurrentFormVertices[firstVertex - 1].x);
	for (iVertex = lastVertex; iVertex < firstVertex; iVertex++)
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - CurrentFormVertices[lastVertex - 1].x)*ratio + CurrentFormVertices[lastVertex - 1].x;
	lastVertex = iDestination = iVertex;
	for (iVertex = lastVertex - 2; iVertex != 0; iVertex--) {
		CurrentFormVertices[iDestination].y = CurrentFormVertices[iVertex].y;
		CurrentFormVertices[iDestination].x = maximumX + maximumX - CurrentFormVertices[iVertex].x - 2 * (maximumX - CurrentFormVertices[0].x);
		iDestination++;
	}
	NewFormVertexCount = iDestination + 1;
	SelectedForm->vertices = adflt(iDestination);
	SelectedForm->vertexCount = iDestination;
	SelectedForm->type = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void dulens(unsigned sides) {
	double		stepAngle = 0.0;
	double		angle = 0.0;
	double		length = 0.0;
	unsigned	iVertex = 0, iDestination = 0, lastVertex = 0, count = 0;
	float		av = 0.0;
	dPOINT		point = {};

	// ToDo - this does not produce a consistent size of lens 
	//        or the correct number of sides
	if (sides < 6)
		sides = 6;
	if (sides > 48)
		sides = 48;
	sides <<= 1;
	stepAngle = PI * 2 / sides;
	count = sides / 2 * 0.3;
	angle = count * stepAngle;
	length = 500 / sides * ZoomFactor*(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	point.x = SelectedPoint.x;
	point.y = SelectedPoint.y;
	iVertex = 0;
	SelectedPoint.x -= 0.0001f;
	while (point.x >= SelectedPoint.x) {
		CurrentFormVertices[iVertex].x = point.x;
		CurrentFormVertices[iVertex++].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	lastVertex = iVertex - 1;
	iDestination = iVertex;
	av = CurrentFormVertices[0].x;
	for (iVertex = lastVertex; iVertex != 0; iVertex--) {
		CurrentFormVertices[iDestination].y = CurrentFormVertices[iVertex - 1].y;
		CurrentFormVertices[iDestination].x = av + av - CurrentFormVertices[iVertex - 1].x;
		iDestination++;
	}
	NewFormVertexCount = iDestination;
	SelectedForm->vertices = adflt(iDestination - 1);
	SelectedForm->vertexCount = iDestination - 1;
	SelectedForm->type = FRMFPOLY;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

constexpr float shreg(float highValue, float reference) {
	return (highValue - reference)*EggRatio + reference;
}

void dueg(unsigned sides) {
	fvars(FormIndex);

	if (sides < 8)
		sides = 8;
	durpoli(sides);

	const double reference = midl(CurrentFormVertices[sides / 2].y, CurrentFormVertices[0].y);
	const double maximumY = CurrentFormVertices[sides >> 2].y - CurrentFormVertices[0].y;

	for (unsigned iVertex = 0; iVertex < sides; iVertex++) {
		if (CurrentFormVertices[iVertex].y < reference)
			CurrentFormVertices[iVertex].y = reference - (reference - CurrentFormVertices[iVertex].y)*IniFile.eggRatio;
	}
	EggRatio = maximumY / (CurrentFormVertices[sides >> 2].y - CurrentFormVertices[0].y);
	for (unsigned iVertex = 1; iVertex < VertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x = shreg(CurrentFormVertices[iVertex].x, CurrentFormVertices[0].x);
		CurrentFormVertices[iVertex].y = shreg(CurrentFormVertices[iVertex].y, CurrentFormVertices[0].y);
	}
}

void duzig(unsigned vertices) {
	fPOINT		offset = {};
	unsigned	iVertex = 0;

	if (vertices < 3)
		vertices = 3;
	if (vertices > 100)
		vertices = 100;
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = adflt(vertices);
	SelectedForm->vertexCount = vertices;
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	px2stch();
	SelectedPoint.x;
	SelectedPoint.y;
	offset.x = UnzoomedRect.x / 6;
	offset.y = UnzoomedRect.y / (6 * vertices);
	for (iVertex = 0; iVertex < vertices; iVertex++) {
		CurrentFormVertices[iVertex].x = SelectedPoint.x;
		CurrentFormVertices[iVertex].y = SelectedPoint.y;
		SelectedPoint.y -= offset.y;
		if (iVertex & 1)
			SelectedPoint.x += offset.x;
		else
			SelectedPoint.x -= offset.x;
	}
	SelectedForm->type = FRMLINE;
	ClosestFormToCursor = FormIndex;
	frmout(FormIndex);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = vertices + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

void fliph() {
	unsigned	iForm = 0, iVertex = 0, iStitch = 0, currentVertex = 0, decodedForm = 0;
	float		midpoint = 0;
	fRECTANGLE	rectangle = {};

	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		savdo();
		midpoint = (SelectedVerticesRect.right - SelectedVerticesRect.left)*0.5 + SelectedVerticesRect.left;
		currentVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			CurrentFormVertices[currentVertex].x = midpoint + midpoint - CurrentFormVertices[currentVertex].x;
			currentVertex = pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		midpoint = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
			FormVertices[iVertex].x = midpoint + midpoint - FormVertices[iVertex].x;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			FormList[iForm].rectangle.left = midpoint + midpoint - FormList[iForm].rectangle.left;
			FormList[iForm].rectangle.right = midpoint + midpoint - FormList[iForm].rectangle.right;
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedFormCount) {
		savdo();
		boost::dynamic_bitset<> formMap(FormIndex);
		pxrct2stch(SelectedFormsRect, &rectangle);
		midpoint = (rectangle.right - rectangle.left) / 2 + rectangle.left;
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			formMap.set(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++)
				CurrentFormVertices[iVertex].x = midpoint + midpoint - CurrentFormVertices[iVertex].x;
			frmout(ClosestFormToCursor);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			decodedForm = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && !(StitchBuffer[iStitch].attribute&NOTFRM))
				StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			midpoint = (SelectedForm->rectangle.right - SelectedForm->rectangle.left) / 2 + SelectedForm->rectangle.left;
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				CurrentFormVertices[iVertex].x = midpoint + midpoint - CurrentFormVertices[iVertex].x;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor && !(StitchBuffer[iStitch].attribute&NOTFRM))
					StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
			}
			frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				selRct(&rectangle);
				midpoint = (rectangle.right - rectangle.left) / 2 + rectangle.left;
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
					StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void flipv() {
	unsigned	iForm = 0, iStitch = 0, iVertex = 0, currentVertex = 0, decodedForm = 0;
	float		midpoint = 0;
	fRECTANGLE	rectangle = {};

	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		savdo();
		midpoint = (SelectedVerticesRect.top - SelectedVerticesRect.bottom)*0.5 + SelectedVerticesRect.bottom;
		currentVertex = SelectedFormVertices.start;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			CurrentFormVertices[currentVertex].y = midpoint + midpoint - CurrentFormVertices[currentVertex].y;
			currentVertex = pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		savdo();
		midpoint = (AllItemsRect.top - AllItemsRect.bottom) / 2 + AllItemsRect.bottom;
		for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
			FormVertices[iVertex].y = midpoint + midpoint - FormVertices[iVertex].y;
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			FormList[iForm].rectangle.bottom = midpoint + midpoint - FormList[iForm].rectangle.bottom;
			FormList[iForm].rectangle.top = midpoint + midpoint - FormList[iForm].rectangle.top;
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedFormCount) {
		savdo();
		boost::dynamic_bitset<> formMap(FormIndex);
		pxrct2stch(SelectedFormsRect, &rectangle);
		midpoint = (rectangle.top - rectangle.bottom) / 2 + rectangle.bottom;
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			formMap.set(ClosestFormToCursor);
			fvars(ClosestFormToCursor);
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++)
				CurrentFormVertices[iVertex].y = midpoint + midpoint - CurrentFormVertices[iVertex].y;
			frmout(ClosestFormToCursor);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			decodedForm = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && !(StitchBuffer[iStitch].attribute&NOTFRM))
				StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			midpoint = (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom) / 2 + SelectedForm->rectangle.bottom;
			for (iVertex = 0; iVertex < VertexCount; iVertex++)
				CurrentFormVertices[iVertex].y = midpoint + midpoint - CurrentFormVertices[iVertex].y;
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor && !(StitchBuffer[iStitch].attribute&NOTFRM))
					StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
			}
			frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				selRct(&rectangle);
				midpoint = (rectangle.top - rectangle.bottom) / 2 + rectangle.bottom;
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++)
					StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void tomsg() noexcept {
	RECT	OKrect;
	SIZE	textSize;

	GetWindowRect(OKButton, &OKrect);
	GetTextExtentPoint32(StitchWindowMemDC, StringTable->at(STR_DELST2).c_str(), StringTable->at(STR_DELST2).size(), &textSize);
	DeleteStitchesDialog = CreateWindow(
		"STATIC",
		StringTable->at(STR_DELST2).c_str(),
		SS_NOTIFY | WS_CHILD | WS_VISIBLE | WS_BORDER,
		3,
		OKrect.bottom - StitchWindowOrigin.y + 6 + textSize.cy,
		textSize.cx + 6,
		textSize.cy + 6,
		MainStitchWin,
		NULL,
		ThrEdInstance,
		NULL);
}

void sprct(std::vector<VRCT2> &fillVerticalRect, unsigned start, unsigned finish) noexcept {
	dPOINT	delta = { (OutsidePoints[finish].x - OutsidePoints[start].x),
					  (OutsidePoints[finish].y - OutsidePoints[start].y) };
	dPOINT	point = {};
	VRCT2*	verticalRect = &fillVerticalRect[start];

	if (delta.x && delta.y) {
		Slope = -delta.x / delta.y;
		point.x = CurrentFormVertices[finish].x;
		point.y = CurrentFormVertices[finish].y;
		proj(point, Slope, OutsidePoints[start], OutsidePoints[finish], &verticalRect->dopnt);
		proj(point, Slope, InsidePoints[start], InsidePoints[finish], &verticalRect->dipnt);
		point.x = CurrentFormVertices[start].x;
		point.y = CurrentFormVertices[start].y;
		proj(point, Slope, OutsidePoints[start], OutsidePoints[finish], &verticalRect->aopnt);
		proj(point, Slope, InsidePoints[start], InsidePoints[finish], &verticalRect->aipnt);
		point.x = InsidePoints[start].x;
		point.y = InsidePoints[start].y;
		if (proj(point, Slope, OutsidePoints[start], OutsidePoints[finish], &verticalRect->bopnt)) {
			verticalRect->bipnt.x = InsidePoints[start].x;
			verticalRect->bipnt.y = InsidePoints[start].y;
		}
		else {
			verticalRect->bopnt.x = OutsidePoints[start].x;
			verticalRect->bopnt.y = OutsidePoints[start].y;
			point.x = OutsidePoints[start].x;
			point.y = OutsidePoints[start].y;
			proj(point, Slope, InsidePoints[start], InsidePoints[finish], &verticalRect->bipnt);
		}
		point.x = InsidePoints[finish].x;
		point.y = InsidePoints[finish].y;
		if (proj(point, Slope, OutsidePoints[start], OutsidePoints[finish], &verticalRect->copnt)) {
			verticalRect->cipnt.x = InsidePoints[finish].x;
			verticalRect->cipnt.y = InsidePoints[finish].y;
		}
		else {
			verticalRect->copnt.x = OutsidePoints[finish].x;
			verticalRect->copnt.y = OutsidePoints[finish].y;
			point.x = OutsidePoints[finish].x;
			point.y = OutsidePoints[finish].y;
			proj(point, Slope, InsidePoints[start], InsidePoints[finish], &verticalRect->cipnt);
		}
	}
	else {
		if (delta.x) {
			point.x = CurrentFormVertices[finish].x;
			projv(point.x, OutsidePoints[start], OutsidePoints[finish], &verticalRect->dopnt);
			projv(point.x, InsidePoints[start], InsidePoints[finish], &verticalRect->dipnt);
			point.x = CurrentFormVertices[start].x;
			projv(point.x, OutsidePoints[start], OutsidePoints[finish], &verticalRect->aopnt);
			projv(point.x, InsidePoints[start], InsidePoints[finish], &verticalRect->aipnt);
			point.x = InsidePoints[start].x;
			if (projv(point.x, OutsidePoints[start], OutsidePoints[finish], &verticalRect->bopnt)) {
				verticalRect->bipnt.x = InsidePoints[start].x;
				verticalRect->bipnt.y = InsidePoints[start].y;
			}
			else {
				verticalRect->bopnt.x = OutsidePoints[start].x;
				verticalRect->bopnt.y = OutsidePoints[start].y;
				point.x = OutsidePoints[start].x;
				projv(point.x, InsidePoints[start], InsidePoints[finish], &verticalRect->bipnt);
			}
			point.x = InsidePoints[finish].x;
			if (projv(point.x, OutsidePoints[start], OutsidePoints[finish], &verticalRect->copnt)) {
				verticalRect->cipnt.x = InsidePoints[finish].x;
				verticalRect->cipnt.y = InsidePoints[finish].y;
			}
			else {
				verticalRect->copnt.x = OutsidePoints[finish].x;
				verticalRect->copnt.y = OutsidePoints[finish].y;
				point.x = OutsidePoints[finish].x;
				projv(point.x, InsidePoints[start], InsidePoints[finish], &verticalRect->cipnt);
			}
		}
		else {
			point.y = CurrentFormVertices[finish].y;
			projh(point.y, OutsidePoints[start], OutsidePoints[finish], &verticalRect->dopnt);
			projh(point.y, InsidePoints[start], InsidePoints[finish], &verticalRect->dipnt);
			point.y = CurrentFormVertices[start].y;
			projh(point.y, OutsidePoints[start], OutsidePoints[finish], &verticalRect->aopnt);
			projh(point.y, InsidePoints[start], InsidePoints[finish], &verticalRect->aipnt);
			point.y = InsidePoints[start].y;
			if (projh(point.y, OutsidePoints[start], OutsidePoints[finish], &verticalRect->bopnt)) {
				verticalRect->bipnt.x = InsidePoints[start].x;
				verticalRect->bipnt.y = InsidePoints[start].y;
			}
			else {
				verticalRect->bopnt.x = OutsidePoints[start].x;
				verticalRect->bopnt.y = OutsidePoints[start].y;
				point.y = OutsidePoints[start].y;
				projh(point.y, InsidePoints[start], InsidePoints[finish], &verticalRect->bipnt);
			}
			point.y = InsidePoints[finish].y;
			if (projh(point.y, OutsidePoints[start], OutsidePoints[finish], &verticalRect->copnt)) {
				verticalRect->cipnt.x = InsidePoints[finish].x;
				verticalRect->cipnt.y = InsidePoints[finish].y;
			}
			else {
				verticalRect->copnt.x = OutsidePoints[finish].x;
				verticalRect->copnt.y = OutsidePoints[finish].y;
				point.y = OutsidePoints[finish].y;
				projh(OutsidePoints[finish].y, InsidePoints[start], InsidePoints[finish], &verticalRect->cipnt);
			}
		}
	}
}

void spurfn(const dPOINT* innerPoint, const dPOINT* outerPoint, dPOINT* underlayInnerPoint, dPOINT* underlayOuterPoint) noexcept {
	dPOINT	delta;

	delta.x = outerPoint->x - innerPoint->x;
	delta.y = outerPoint->y - innerPoint->y;
	underlayInnerPoint->x = delta.x*DIURAT + innerPoint->x;
	underlayInnerPoint->y = delta.y*DIURAT + innerPoint->y;
	underlayOuterPoint->x = delta.x*DOURAT + innerPoint->x;
	underlayOuterPoint->y = delta.y*DOURAT + innerPoint->y;
}

void spurct(std::vector<VRCT2> &underlayVerticalRect, std::vector<VRCT2> &fillVerticalRect, unsigned iRect) noexcept {
	spurfn(&fillVerticalRect[iRect].aipnt, &fillVerticalRect[iRect].aopnt, &underlayVerticalRect[iRect].aipnt, &underlayVerticalRect[iRect].aopnt);
	spurfn(&fillVerticalRect[iRect].bipnt, &fillVerticalRect[iRect].bopnt, &underlayVerticalRect[iRect].bipnt, &underlayVerticalRect[iRect].bopnt);
	spurfn(&fillVerticalRect[iRect].cipnt, &fillVerticalRect[iRect].copnt, &underlayVerticalRect[iRect].cipnt, &underlayVerticalRect[iRect].copnt);
	spurfn(&fillVerticalRect[iRect].dipnt, &fillVerticalRect[iRect].dopnt, &underlayVerticalRect[iRect].dipnt, &underlayVerticalRect[iRect].dopnt);
}

unsigned psg() noexcept {
	unsigned	temp = 0;

	if (!PseudoRandomValue)
		PseudoRandomValue = SEED;
	temp = PseudoRandomValue & 0x48000000;
	PseudoRandomValue <<= 1;
	if (temp == 0x40000000 || temp == 0x8000000)
		PseudoRandomValue++;
	return PseudoRandomValue;
}

void duromb(dPOINT start0, dPOINT finish0, dPOINT start1, dPOINT finish1) {
	dPOINT		delta0 = {}, delta1 = {}, step0 = {}, step1 = {};
	double		length0 = 0.0, length1 = 0.0;
	unsigned	count = 0, iStep = 0;

	if (!StateMap.test(StateFlag::UND)) {
		delta0.x = SelectedPoint.x - start0.x;
		delta0.y = SelectedPoint.y - start0.y;
		delta1.x = SelectedPoint.x - start1.x;
		delta1.y = SelectedPoint.y - start1.y;
		length0 = hypot(delta0.x, delta0.y);
		length1 = hypot(delta1.x, delta1.y);
		if (length0 > length1)
			StateMap.set(StateFlag::FILDIR);
		else
			StateMap.reset(StateFlag::FILDIR);
	}
	delta0.x = finish0.x - start0.x;
	delta0.y = finish0.y - start0.y;
	delta1.x = finish1.x - start1.x;
	delta1.y = finish1.y - start1.y;
	length0 = hypot(delta0.x, delta0.y);
	count = length0 / (LineSpacing / 2);
	if (!count)
		count++;
	step0.x = delta0.x / count;
	step0.y = delta0.y / count;
	step1.x = delta1.x / count;
	step1.y = delta1.y / count;
	for (iStep = 0; iStep < count; iStep++) {
		if (StateMap.testAndFlip(StateFlag::FILDIR))
			filinsb(start0);
		else
			filinsb(start1);
		start0.x += step0.x;
		start0.y += step0.y;
		start1.x += step1.x;
		start1.y += step1.y;
	}
}

void spend(std::vector<VRCT2> &fillVerticalRect, unsigned start, unsigned finish) noexcept {
	dPOINT		innerDelta = { (fillVerticalRect[finish].cipnt.x - fillVerticalRect[start].bipnt.x),
							   (fillVerticalRect[finish].cipnt.y - fillVerticalRect[start].bipnt.y) };
	const double	innerLength = hypot(innerDelta.x, innerDelta.y);
	dPOINT		outerDelta = { (fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
							   (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y) };
	const double	outerLength = hypot(outerDelta.x, outerDelta.y);
	double		startAngle = 0.0, finishAngle = 0.0, deltaAngle = 0.0, stepAngle = 0.0;
	dPOINT		startDelta = {}, finishDelta = {};
	dPOINT		pivot = {};
	double		radius = 0.0, arc = 0.0, innerRadius = 0.0;
	unsigned	ind = 0, count = 0, level = 0;
	dPOINT		innerPoint = {}, outerPoint = {};

	if (outerLength > innerLength) {
		pivot.x = fillVerticalRect[start].cipnt.x;
		pivot.y = fillVerticalRect[start].cipnt.y;
		startDelta.x = fillVerticalRect[start].copnt.x - pivot.x;
		startDelta.y = fillVerticalRect[start].copnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bopnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bopnt.y - pivot.y;
	}
	else {
		pivot.x = fillVerticalRect[start].copnt.x;
		pivot.y = fillVerticalRect[start].copnt.y;
		startDelta.x = fillVerticalRect[start].cipnt.x - pivot.x;
		startDelta.y = fillVerticalRect[start].cipnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bipnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bipnt.y - pivot.y;
	}
	if (hypot(SelectedPoint.x - pivot.x, SelectedPoint.y - pivot.y) > 2 * PI)
		filinsb(pivot);
	startAngle = atan2(startDelta.y, startDelta.x);
	finishAngle = atan2(finishDelta.y, finishDelta.x);
	deltaAngle = finishAngle - startAngle;
	if (deltaAngle > PI)
		deltaAngle -= 2 * PI;
	if (deltaAngle < -PI)
		deltaAngle += 2 * PI;
	radius = hypot(startDelta.x, startDelta.y);
	arc = fabs(radius*deltaAngle);
	count = arc / LineSpacing;
	stepAngle = deltaAngle / count;
	if (!count)
		count = 1;
	for (ind = 0; ind < count; ind++) {
		outerPoint.x = pivot.x + cos(startAngle)*radius;
		outerPoint.y = pivot.y + sin(startAngle)*radius;
		filinsb(outerPoint);
		if (count & 0xfffffff0)
			level = psg() % count;
		else
			level = Levels[count][ind];
		innerRadius = radius * level / count * 0.4;
		innerPoint.x = pivot.x + cos(startAngle)*innerRadius;
		innerPoint.y = pivot.y + sin(startAngle)*innerRadius;
		filinsb(innerPoint);
		startAngle += stepAngle;
	}
}

void duspnd(std::vector<VRCT2> &underlayVerticalRect, std::vector<VRCT2> &fillVerticalRect, unsigned start, unsigned finish) {
	double	length = 0.0, angle = 0.0;
	dPOINT	point = {}, delta = {};

	if (StateMap.test(StateFlag::UND)) {
		if (StateMap.test(StateFlag::UNDPHAS)) {
			filinsb(underlayVerticalRect[start].copnt);
			filinsb(underlayVerticalRect[start].cipnt);
			delta.x = underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x;
			delta.y = underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y;
			length = hypot(delta.x, delta.y);
			if (length > SelectedForm->edgeStitchLen) {
				angle = atan2(InsidePoints[finish].y - OutsidePoints[finish].y, InsidePoints[finish].x - OutsidePoints[finish].x);
				point.x = underlayVerticalRect[finish].bopnt.x + cos(angle)*HorizontalLength2;
				point.y = underlayVerticalRect[finish].bopnt.y + sin(angle)*HorizontalLength2;
				filinsb(point);
			}
			filinsb(underlayVerticalRect[finish].bipnt);
			filinsb(underlayVerticalRect[finish].bopnt);
		}
		else {
			filinsb(underlayVerticalRect[start].cipnt);
			filinsb(underlayVerticalRect[start].copnt);
			delta.x = underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x;
			delta.y = underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y;
			length = hypot(delta.x, delta.y);
			if (length > SelectedForm->edgeStitchLen) {
				angle = atan2(OutsidePoints[finish].y - InsidePoints[finish].y, OutsidePoints[finish].x - InsidePoints[finish].x);
				point.x = underlayVerticalRect[finish].bipnt.x + cos(angle)*HorizontalLength2;
				point.y = underlayVerticalRect[finish].bipnt.y + sin(angle)*HorizontalLength2;
				filinsb(point);
			}
			filinsb(underlayVerticalRect[finish].bopnt);
			filinsb(underlayVerticalRect[finish].bipnt);
		}
	}
	else
		spend(fillVerticalRect, start, finish);
}

void pfn(std::vector<VRCT2> &underlayVerticalRect, std::vector<VRCT2> &fillVerticalRect, unsigned startVertex, const VRCT2* vrct) {
	if (vrct) {
		unsigned	iVertex = 0;
		unsigned	currentVertex = startVertex;
		unsigned	nextVertex = nxt(currentVertex);

		SelectedPoint.x = CurrentFormVertices[startVertex].x;
		SelectedPoint.y = CurrentFormVertices[startVertex].y;
		for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
			duromb(vrct[currentVertex].bipnt, vrct[currentVertex].cipnt, vrct[currentVertex].bopnt, vrct[currentVertex].copnt);
			duspnd(underlayVerticalRect, fillVerticalRect, currentVertex, nextVertex);
			currentVertex = nextVertex;
			nextVertex = nxt(nextVertex);
		}
	}
}

void plfn(std::vector<VRCT2> &underlayVerticalRect, std::vector<VRCT2> &fillVerticalRect, const VRCT2* prct) {
	if (prct) {
		unsigned	iVertex;

		duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt);
		duspnd(underlayVerticalRect, fillVerticalRect, 1, 2);
		for (iVertex = 2; iVertex < VertexCount - 4; iVertex++) {
			duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt);
			duspnd(underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1);
		}
		duromb(prct[VertexCount - 4].bipnt, prct[VertexCount - 4].dipnt, prct[VertexCount - 4].bopnt, prct[VertexCount - 4].dopnt);
	}
}

void prsmal() noexcept {
	unsigned	iSequence = 0, iOutput = 0, iReference = 0;
	double		minimumLength = USPAC * 0.8;
	dPOINT		delta = {};
	double		length = 0.0;

	if (minimumLength > HorizontalLength2)
		minimumLength = HorizontalLength2 * 0.9;
	for (iSequence = 1; iSequence < SequenceIndex; iSequence++) {
		delta.x = OSequence[iSequence].x - OSequence[iReference].x;
		delta.y = OSequence[iSequence].y - OSequence[iReference].y;
		length = hypot(delta.x, delta.y);
		if (length > minimumLength) {
			OSequence[iOutput].x = OSequence[iSequence].x;
			OSequence[iOutput++].y = OSequence[iSequence].y;
			iReference = iSequence;
		}
	}
	SequenceIndex = iOutput;
}

void plbak(unsigned backPoint) noexcept {
	unsigned	iSequence = SequenceIndex - 1;
	fPOINT		tflt = {};

	while (iSequence > backPoint) {
		tflt.x = OSequence[iSequence].x;
		tflt.y = OSequence[iSequence].y;
		OSequence[iSequence].x = OSequence[backPoint].x;
		OSequence[iSequence].y = OSequence[backPoint].y;
		OSequence[backPoint].x = tflt.x;
		OSequence[backPoint].y = tflt.y;
		iSequence--; backPoint++;
	}
}

void prebrd() noexcept {
	fPOINT	delta = { (CurrentFormVertices[1].x - CurrentFormVertices[0].x),
					  (CurrentFormVertices[1].y - CurrentFormVertices[0].y) };
	double	ratio = 0.0;

	MoveMemory(&AngledFormVertices[1], CurrentFormVertices, sizeof(fPOINT)*VertexCount);
	if (fabs(delta.x) > fabs(delta.y))
		ratio = fabs(0.1 / delta.x);
	else
		ratio = fabs(0.1 / delta.y);
	AngledFormVertices[0].x = CurrentFormVertices[0].x - delta.x*ratio;
	AngledFormVertices[0].y = CurrentFormVertices[0].y - delta.y*ratio;
	MoveMemory(&AngledForm, SelectedForm, sizeof(FRMHED));
	AngledForm.vertices = AngledFormVertices;
	AngledForm.vertexCount += 3;
	delta.x = CurrentFormVertices[VertexCount - 1].x - CurrentFormVertices[VertexCount - 2].x;
	delta.y = CurrentFormVertices[VertexCount - 1].y - CurrentFormVertices[VertexCount - 2].y;
	if (delta.x > delta.y)
		ratio = 0.1 / delta.x;
	else
		ratio = 0.1 / delta.y;
	AngledFormVertices[AngledForm.vertexCount - 1].x = CurrentFormVertices[VertexCount - 1].x + delta.x*ratio;
	AngledFormVertices[AngledForm.vertexCount - 1].y = CurrentFormVertices[VertexCount - 1].y + delta.y*ratio;
	SelectedForm = &AngledForm;
	VertexCount = AngledForm.vertexCount;
	CurrentFormVertices = AngledForm.vertices;
}

void plbrd(double edgeSpacing) {
	unsigned		savedIndex = 0;
	unsigned		iVertex = 0;

	prebrd();
	// Ensure that we have at least 4 array members
	std::vector<VRCT2> fillVerticalRect(VertexCount + 5);
	std::vector<VRCT2> underlayVerticalRect(VertexCount + 5);
	satout(SelectedForm->borderSize);
	InsidePoints[VertexCount].x = InsidePoints[0].x;
	InsidePoints[VertexCount].y = InsidePoints[0].y;
	OutsidePoints[VertexCount].x = OutsidePoints[0].x;
	OutsidePoints[VertexCount].y = OutsidePoints[0].y;
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, iVertex, 0);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	if (!(SelectedForm->attribute&SBLNT)) {
		fillVerticalRect[1].aipnt.x = fillVerticalRect[1].aopnt.x = underlayVerticalRect[1].aipnt.x = underlayVerticalRect[1].aopnt.x = SelectedForm->vertices[1].x;
		fillVerticalRect[1].aipnt.y = fillVerticalRect[1].aopnt.y = underlayVerticalRect[1].aipnt.y = underlayVerticalRect[1].aopnt.y = SelectedForm->vertices[1].y;
	}
	if (!(SelectedForm->attribute&FBLNT)) {
		fillVerticalRect[VertexCount - 4].dipnt.x = fillVerticalRect[VertexCount - 4].dopnt.x = underlayVerticalRect[VertexCount - 4].dipnt.x = underlayVerticalRect[VertexCount - 4].dopnt.x = SelectedForm->vertices[VertexCount - 1].x;
		fillVerticalRect[VertexCount - 4].dipnt.y = fillVerticalRect[VertexCount - 4].dopnt.y = underlayVerticalRect[VertexCount - 4].dipnt.y = underlayVerticalRect[VertexCount - 4].dopnt.y = SelectedForm->vertices[VertexCount - 1].y;
	}
	SequenceIndex = 0;
	SelectedPoint.x = CurrentFormVertices[0].x;
	SelectedPoint.y = CurrentFormVertices[0].y;
	if (SelectedForm->edgeType&EGUND) {
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		HorizontalLength2 = SelectedForm->borderSize*URAT;
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		plfn(underlayVerticalRect, fillVerticalRect, &underlayVerticalRect[0]);
		savedIndex = SequenceIndex;
		StateMap.reset(StateFlag::UNDPHAS);
		SelectedPoint.x = CurrentFormVertices[0].x;
		SelectedPoint.y = CurrentFormVertices[0].y;
		StateMap.set(StateFlag::FILDIR);
		plfn(underlayVerticalRect, fillVerticalRect, &underlayVerticalRect[0]);
		plbak(savedIndex);
		prsmal();
		if (SequenceIndex) { //ensure that we can do a valid read from OSequence
			SelectedPoint.x = OSequence[SequenceIndex - 1].x;
			SelectedPoint.y = OSequence[SequenceIndex - 1].y;
		}
	}
	StateMap.reset(StateFlag::UND);
	LineSpacing = SelectedForm->edgeSpacing;
	plfn(underlayVerticalRect, fillVerticalRect, &fillVerticalRect[0]);
	LineSpacing = edgeSpacing;
	fvars(ClosestFormToCursor);
}

void pbrd(double edgeSpacing) {
	const double	spacing = LineSpacing;
	unsigned	iVertex = 0;
	const unsigned	start = getlast();

	LineSpacing = SelectedForm->edgeSpacing;
	SequenceIndex = 0;
	std::vector<VRCT2> fillVerticalRect(VertexCount);
	std::vector<VRCT2> underlayVerticalRect(VertexCount);
	satout(SelectedForm->borderSize);
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, iVertex, 0);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	if (SelectedForm->edgeType&EGUND) {
		StateMap.reset(StateFlag::SAT1);
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		HorizontalLength2 = SelectedForm->borderSize*URAT;
		satout(HorizontalLength2);
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.set(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, &underlayVerticalRect[0]);
		StateMap.reset(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, &underlayVerticalRect[0]);
		LineSpacing = edgeSpacing;
		prsmal();
		HorizontalLength2 = SelectedForm->borderSize;
		StateMap.reset(StateFlag::UND);
	}
	pfn(underlayVerticalRect, fillVerticalRect, start, &fillVerticalRect[0]);
	LineSpacing = spacing;
}

void prpsbrd() {
	if (SelectedForm->vertexCount > 2) {
		deleclp(ClosestFormToCursor);
		SelectedForm->edgeType = EDGEPROPSAT;
		if (UserFlagMap.test(UserFlag::DUND))
			SelectedForm->edgeType |= EGUND;
		bsizpar();
		SelectedForm->borderSize = BorderWidth;
		SelectedForm->edgeSpacing = LineSpacing;
		SelectedForm->borderColor = ActiveColor;
		refilfn();
	}
}

void prpbrd(double borderStitchSpacing) {
	const double	savedSpacing = LineSpacing;
	unsigned	iForm = 0;

	if (filmsgs(FML_PRPS))
		return;
	LineSpacing = borderStitchSpacing;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			SelectedForm->borderSize = LineSpacing;
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			prpsbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			prpsbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void tglfrm() {
	if (StateMap.testAndReset(StateFlag::SATPNT))
		satfix();
	StateMap.reset(StateFlag::HIDSTCH);
	if (StateMap.testAndFlip(StateFlag::FRMOF)) {
		strcpy_s(FormOnOff, StringTable->at(STR_FRMPLUS).c_str());
	}
	else {
		strcpy_s(FormOnOff, StringTable->at(STR_FRMINUS).c_str());
		StateMap.reset(StateFlag::FORMSEL);
		StateMap.reset(StateFlag::FORMIN);
		StateMap.reset(StateFlag::MOVFRM);
		StateMap.reset(StateFlag::FRMPMOV);
		StateMap.reset(StateFlag::FRMOV);
		StateMap.reset(StateFlag::SATIN);
		StateMap.reset(StateFlag::SATPNT);
		StateMap.reset(StateFlag::SATCNKT);
		StateMap.reset(StateFlag::FENDIN);
		StateMap.reset(StateFlag::DELFRM);
		StateMap.reset(StateFlag::FRMPSEL);
		StateMap.reset(StateFlag::INSFRM);
	}
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, &MenuInfo);
	StateMap.set(StateFlag::DUMEN);
	StateMap.set(StateFlag::RESTCH);
}

void frmon() {
	unbsho();
	StateMap.reset(StateFlag::FRMOF);
	strcpy_s(FormOnOff, StringTable->at(STR_FRMPLUS).c_str());
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, &MenuInfo);
	StateMap.set(StateFlag::DUMEN);
}

void fnord() {
	int			iVertex = 0;
	fPOINTATTR	swapPoint = {};

	fvars(ClosestFormToCursor);
	SelectedForm = &FormList[ClosestFormToCursor];
	for (iVertex = 0; iVertex < SelectedForm->vertexCount >> 1; iVertex++) {
		swapPoint.x = SelectedForm->vertices[iVertex].x;
		swapPoint.y = SelectedForm->vertices[iVertex].y;
		SelectedForm->vertices[iVertex].x = SelectedForm->vertices[SelectedForm->vertexCount - iVertex - 1].x;
		SelectedForm->vertices[iVertex].y = SelectedForm->vertices[SelectedForm->vertexCount - iVertex - 1].y;
		SelectedForm->vertices[SelectedForm->vertexCount - iVertex - 1].x = swapPoint.x;
		SelectedForm->vertices[SelectedForm->vertexCount - iVertex - 1].y = swapPoint.y;
	}
	refil();
}

void flpord() {
	int			iStitch = 0;
	unsigned	iVertex = 0, iForm = 0, iForward = 0, start = 0, finish = 0;
	fPOINTATTR	swapPoint = {};

	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		savdo();
		start = SelectedFormVertices.start;
		finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount >> 1; iVertex++) {
			swapPoint.x = SelectedForm->vertices[start].x;
			swapPoint.y = SelectedForm->vertices[start].y;
			SelectedForm->vertices[start].x = SelectedForm->vertices[finish].x;
			SelectedForm->vertices[start].y = SelectedForm->vertices[finish].y;
			SelectedForm->vertices[finish].x = swapPoint.x;
			SelectedForm->vertices[finish].y = swapPoint.y;
			start = pdir(start);
			StateMap.flip(StateFlag::PSELDIR);
			finish = pdir(finish);
			StateMap.flip(StateFlag::PSELDIR);
		}
		refil();
		return;
	}
	if (SelectedFormCount) {
		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fnord();
		}
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			fnord();
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				iForward = GroupStartStitch;
				for (iStitch = 0; iStitch < (gsl::narrow<signed>(GroupEndStitch - GroupStartStitch) >> 1) + 1; iStitch++) {
					swapPoint.attribute = StitchBuffer[iForward].attribute;
					swapPoint.x = StitchBuffer[iForward].x;
					swapPoint.y = StitchBuffer[iForward].y;
					StitchBuffer[iForward].attribute = StitchBuffer[GroupEndStitch - iStitch].attribute;
					StitchBuffer[iForward].x = StitchBuffer[GroupEndStitch - iStitch].x;
					StitchBuffer[iForward].y = StitchBuffer[GroupEndStitch - iStitch].y;
					StitchBuffer[GroupEndStitch - iStitch].attribute = swapPoint.attribute;
					StitchBuffer[GroupEndStitch - iStitch].x = swapPoint.x;
					StitchBuffer[GroupEndStitch - iStitch].y = swapPoint.y;
					iForward++;
				}
				coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void dudif(dPOINT start, dPOINT finish, dPOINT* delta) noexcept {
	delta->x = finish.x - start.x;
	delta->y = finish.y - start.y;
}

void trfrm(dPOINT bottomLeftPoint, dPOINT topLeftPoint, dPOINT bottomRightPoint, dPOINT topRightPoint) noexcept {
	unsigned	iStitch = 0;
	dPOINT		topDelta = {}, bottomDelta = {}, leftDelta = {}, rightDelta = {};
	dPOINT		clipRatio = {}, topMidpoint = {}, bottomMidpoint = {}, middleDelta = {};

	dudif(topLeftPoint, topRightPoint, &topDelta);
	dudif(bottomLeftPoint, bottomRightPoint, &bottomDelta);
	dudif(bottomLeftPoint, topLeftPoint, &leftDelta);
	dudif(bottomRightPoint, topRightPoint, &rightDelta);
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		clipRatio.x = ClipBuffer[iStitch].x / ClipRectSize.cx;
		clipRatio.y = ClipBuffer[iStitch].y / ClipRectSize.cy;
		topMidpoint.x = clipRatio.x*(topDelta.x) + topLeftPoint.x;
		topMidpoint.y = clipRatio.x*(topDelta.y) + topLeftPoint.y;
		bottomMidpoint.x = clipRatio.x*(bottomDelta.x) + bottomLeftPoint.x;
		bottomMidpoint.y = clipRatio.x*(bottomDelta.y) + bottomLeftPoint.y;
		dudif(bottomMidpoint, topMidpoint, &middleDelta);
		OSequence[ActivePointIndex].x = clipRatio.y*middleDelta.x + bottomMidpoint.x;
		OSequence[ActivePointIndex].y = clipRatio.y*middleDelta.y + bottomMidpoint.y;
		ActivePointIndex++;
	}
}

void clpfm() noexcept {
	unsigned	iSequence = 0, iStep = 0, count = 0;
	double		leftLength = 0.0, rightLength = 0.0;
	dPOINT		leftDelta = {}, rightDelta = {};
	dPOINT		leftStep = {}, rightStep = {};
	dPOINT		bottomLeft = {}, bottomRight = {};
	dPOINT		topLeft = {}, topRight = {};

	ActivePointIndex = 0;
	for (iSequence = 0; iSequence < SequenceIndex - 2; iSequence += 2) {
		leftLength = hypot(BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y);
		rightLength = hypot(BSequence[iSequence + 3].x - BSequence[iSequence + 2].x, BSequence[iSequence + 3].y - BSequence[iSequence + 2].y);
		leftDelta.x = BSequence[iSequence + 1].x - BSequence[iSequence].x;
		leftDelta.y = BSequence[iSequence + 1].y - BSequence[iSequence].y;
		rightDelta.x = BSequence[iSequence + 2].x - BSequence[iSequence + 3].x;
		rightDelta.y = BSequence[iSequence + 2].y - BSequence[iSequence + 3].y;
		if (rightLength > leftLength)
			count = leftLength / ClipRectSize.cy;
		else
			count = rightLength / ClipRectSize.cy;
		if (!count)
			count = 1;
		leftStep.x = leftDelta.x / count;
		leftStep.y = leftDelta.y / count;
		rightStep.x = rightDelta.x / count;
		rightStep.y = rightDelta.y / count;
		topLeft.x = BSequence[iSequence].x;
		topLeft.y = BSequence[iSequence].y;
		topRight.x = BSequence[iSequence + 3].x;
		topRight.y = BSequence[iSequence + 3].y;
		for (iStep = 0; iStep < count; iStep++) {
			bottomLeft.x = topLeft.x;
			bottomLeft.y = topLeft.y;
			bottomRight.x = topRight.x;
			bottomRight.y = topRight.y;
			topLeft.x += leftStep.x;
			topLeft.y += leftStep.y;
			topRight.x += rightStep.x;
			topRight.y += rightStep.y;
			if (ActivePointIndex > MAXITEMS - ClipStitchCount - 1)
				return;
			trfrm(bottomLeft, topLeft, bottomRight, topRight);
		}
	}
}

void fmclp() {
	const double	savedSpacing = LineSpacing;

	LineSpacing = ClipRectSize.cx;
	StateMap.set(StateFlag::BARSAT);
	satfil();
	StateMap.reset(StateFlag::BARSAT);
	clpfm();
	SelectedForm->fillType = CLPF;
	SequenceIndex = ActivePointIndex;
	LineSpacing = savedSpacing;
}

void filsclp() {
	unsigned	iClip = 0;

	deltx();
	fvars(ClosestFormToCursor);
	if (SelectedForm->type != SAT)
		SelectedForm->wordParam = 0;
	SelectedForm->type = SAT;
	SelectedForm->fillType = CLPF;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	for (iClip = 0; iClip < ClipStitchCount; iClip++) {
		SelectedForm->angleOrClipData.clip[iClip].x = ClipBuffer[iClip].x;
		SelectedForm->angleOrClipData.clip[iClip].y = ClipBuffer[iClip].y;
	}
	refilfn();
}

void clpfil() {
	unsigned	iForm = 0;

	if (filmsgs(FMM_CLP))
		return;
	if (OpenClipboard(ThrEdWindow)) {
		Clip = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory) {
			redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN && ClipRectSize.cy > CLPMIN) {
				if (SelectedFormCount) {
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							filsclp();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						filsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
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

void snpfn(std::vector<unsigned> &xPoints, unsigned start, unsigned end, unsigned finish) noexcept {
	unsigned	iPoint = 0, reference = 0, check = 0;
	double		CheckLength = 0.0;

	if (finish - start) {
		for (unsigned current = start; current < end; current++) {
			reference = xPoints[current];
			for (iPoint = current + 1; iPoint < finish; iPoint++) {
				check = xPoints[iPoint];
				CheckLength = hypot(StitchBuffer[check].x - StitchBuffer[reference].x, StitchBuffer[check].y - StitchBuffer[reference].y);
				if (CheckLength < SnapLength) {
					StitchBuffer[check].x = StitchBuffer[reference].x;
					StitchBuffer[check].y = StitchBuffer[reference].y;
				}
			}
		}
	}
}

void nutim(double size) noexcept {
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
	TimeStep = static_cast<double>(StitchWindowSize.x) / size;
	TimePosition = 0;
	FormLines[0].y = 0;
	FormLines[1].y = ButtonHeight;
	FormLines[0].x = FormLines[1].x = 0;
	SelectObject(TimeDC, UserPen[0]);
}

void nxtim() noexcept {
	Polyline(TimeDC, FormLines, 2);
	TimePosition += TimeStep;
	FormLines[0].x = FormLines[1].x = TimePosition;
}

void snp(unsigned start, unsigned finish) {
	unsigned	iColumn = 0, iStitch = 0, swap = 0, accumulator = 0, checkLength = 0, attribute = 0;
	fPOINT		range = {};

	chkrng(&range);
	std::vector<unsigned> xPoints(PCSHeader.stitchCount);
	std::vector<unsigned> xHistogram((round(range.x)) + 1);
	if (StateMap.test(StateFlag::FORMSEL)) {
		attribute = (ClosestFormToCursor << 4)&FRMSK;
		for (iStitch = start; iStitch < finish; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == attribute) {
				iColumn = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
				xHistogram[iColumn]++;
			}
		}
	}
	else {
		for (iStitch = start; iStitch < finish; iStitch++) {
			iColumn = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
			xHistogram[iColumn]++;
		}
	}
	accumulator = 0;
	for (iColumn = 0; iColumn < range.x; iColumn++) {
		swap = xHistogram[iColumn];
		xHistogram[iColumn] = accumulator;
		accumulator += swap;
	}
	xHistogram[iColumn] = accumulator;
	if (StateMap.test(StateFlag::FORMSEL)) {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (!(StitchBuffer[iStitch].attribute&NOTFRM) && (StitchBuffer[iStitch].attribute&FRMSK) == attribute) {
				iColumn = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
				xPoints[xHistogram[iColumn]++] = iStitch;
			}
		}
	}
	else {
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			iColumn = gsl::narrow<unsigned>(floor(StitchBuffer[iStitch].x));
			xPoints[xHistogram[iColumn]++] = iStitch;
		}
	}
	checkLength = SnapLength * 2 + 1;
	nutim(range.x);
	for (iColumn = 1; iColumn < range.x - checkLength - 1; iColumn++) {
		snpfn(xPoints, xHistogram[iColumn], xHistogram[iColumn + 1], xHistogram[(iColumn)+checkLength]);
		nxtim();
	}
	DestroyWindow(TimeWindow);
}

void snap() {
	savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		rngadj();
		snp(GroupStartStitch, GroupEndStitch);
	}
	else
		snp(0, PCSHeader.stitchCount);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void rotpar() {
	if (IniFile.rotationAngle < (PI / 180))
		tabmsg(IDS_ROTIN);
	if (StateMap.test(StateFlag::FPSEL)) {
		RotationCenter.x = midl(SelectedVerticesRect.right, SelectedVerticesRect.left);
		RotationCenter.y = midl(SelectedVerticesRect.top, SelectedVerticesRect.bottom);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		RotationCenter.x = midl(AllItemsRect.right, AllItemsRect.left);
		RotationCenter.y = midl(AllItemsRect.top, AllItemsRect.bottom);
		return;
	}
	if (SelectedFormCount) {
		if (StateMap.test(StateFlag::GMRK)) {
			RotationCenter.x = ZoomMarkPoint.x;
			RotationCenter.y = ZoomMarkPoint.y;
		}
		else
			dufcntr(&RotationCenter);
		StateMap.set(StateFlag::FRMSROT);
		return;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		RotationRect.left = SelectedForm->rectangle.left;
		RotationRect.right = SelectedForm->rectangle.right;
		RotationRect.top = SelectedForm->rectangle.top;
		RotationRect.bottom = SelectedForm->rectangle.bottom;
		if (!StateMap.test(StateFlag::GMRK)) {
			RotationCenter.x = static_cast<double>(RotationRect.right - RotationRect.left) / 2 + RotationRect.left;
			RotationCenter.y = static_cast<double>(RotationRect.top - RotationRect.bottom) / 2 + RotationRect.bottom;
		}
		StateMap.set(StateFlag::FRMROT);
	}
	else {
		if (StateMap.test(StateFlag::GRPSEL)) {
			rngadj();
			selRct(&RotationRect);
		}
	}
	if (StateMap.test(StateFlag::GMRK)) {
		RotationCenter.x = ZoomMarkPoint.x;
		RotationCenter.y = ZoomMarkPoint.y;
	}
	else {
		RotationCenter.x = static_cast<double>(RotationRect.right - RotationRect.left) / 2 + RotationRect.left;
		RotationCenter.y = static_cast<double>(RotationRect.top - RotationRect.bottom) / 2 + RotationRect.bottom;
	}
}

void rotentr() {
	std::string fmtStr;

	loadString(fmtStr, IDS_ROTA);
	shoMsg(fmt::format(fmtStr, (RotationAngle / PI * 180)));
	StateMap.set(StateFlag::NUMIN);
	numWnd();
}

void setrang() {
	RotationAngle = IniFile.rotationAngle;
	rotentr();
	StateMap.set(StateFlag::NUROT);
}

void rotcmd() {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		RotationAngle = IniFile.rotationAngle;
		rotpar();
		rotentr();
		StateMap.set(StateFlag::ENTROT);
	}
	else
		shoseln(IDS_ALLX, IDS_ROTCMD);
}

void fnagain() {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || SelectedFormCount || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
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
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || SelectedFormCount) {
		RotationAngle = IniFile.rotationAngle;
		rotpar();
		rotentr();
		StateMap.set(StateFlag::ENTRDUP);
	}
	else
		shord();
}

void adfrm(unsigned iForm) {
	FRMHED*		formHeader = &FormList[FormIndex];

	SelectedForm = &FormList[iForm];
	*formHeader = *SelectedForm;
	ClosestFormToCursor = FormIndex;
	formHeader->vertices = adflt(SelectedForm->vertexCount);
	mvflpnt(formHeader->vertices, SelectedForm->vertices, SelectedForm->vertexCount);
	if (formHeader->type == SAT && formHeader->satinGuideCount) {
		formHeader->satinOrAngle.guide = adsatk(formHeader->satinGuideCount);
		mvsatk(formHeader->satinOrAngle.guide, SelectedForm->satinOrAngle.guide, SelectedForm->satinGuideCount);
	}
	if (iseclpx(FormIndex)) {
		formHeader->borderClipData = adclp(formHeader->clipEntries);
		mvflpnt(formHeader->borderClipData, SelectedForm->borderClipData, SelectedForm->clipEntries);
	}
	if (isclpx(FormIndex)) {
		formHeader->angleOrClipData.clip = adclp(formHeader->lengthOrCount.clipCount);
		mvflpnt(formHeader->angleOrClipData.clip, SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
	}
	FormIndex++;
}

void duprot() {
	adfrm(ClosestFormToCursor);
	rotfn();
	refil();
	StateMap.set(StateFlag::FORMSEL);
	StateMap.set(StateFlag::RESTCH);
}

void duprotfs() {
	const unsigned	savedFormIndex = FormIndex;
	unsigned	iForm = 0;


	rotpar();
	for (iForm = 0; iForm < SelectedFormCount; iForm++)
		adfrm(SelectedFormList[iForm]);
	FormIndex = savedFormIndex;
	for (iForm = 0; iForm < SelectedFormCount; iForm++)
		SelectedFormList[iForm] = FormIndex++;
	fnagain();
}

void duprots() {
	unsigned	source = 0, destination = PCSHeader.stitchCount;

	rngadj();

	for (source = GroupStartStitch; source <= GroupEndStitch; source++) {
		StitchBuffer[destination].x = StitchBuffer[source].x;
		StitchBuffer[destination].y = StitchBuffer[source].y;
		StitchBuffer[destination++].attribute = StitchBuffer[source].attribute&(~(FRMSK | TYPMSK));
	}
	ClosestPointIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount += (GroupEndStitch - GroupStartStitch);
	GroupStitchIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount++;
	rngadj();
	rotfn();
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void cplayfn(unsigned iForm, unsigned play) {
	const FRMHED*	formHeader = &FormList[iForm];

	fvars(FormIndex);
	*SelectedForm = *formHeader;
	fvars(FormIndex);
	SelectedForm->vertices = adflt(SelectedForm->vertexCount);
	MoveMemory(SelectedForm->vertices, formHeader->vertices, VertexCount * sizeof(fPOINT));
	if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.guide = adsatk(SelectedForm->satinGuideCount);
		MoveMemory(SelectedForm->satinOrAngle.guide, formHeader->satinOrAngle.guide, SelectedForm->satinGuideCount * sizeof(SATCON));
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

void cpylayr(unsigned codedLayer) {
	unsigned	iForm = 0, iStitch = 0, iCurrentStitch = 0;
	const unsigned	codedStitchLayer = codedLayer << (LAYSHFT - 1);

	if (SelectedFormCount) {
		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			cplayfn(SelectedFormList[iForm], codedLayer);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			cplayfn(ClosestFormToCursor, codedLayer);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				iCurrentStitch = PCSHeader.stitchCount;
				for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
					StitchBuffer[iCurrentStitch].attribute = StitchBuffer[iStitch].attribute&NLAYMSK | codedStitchLayer;
					StitchBuffer[iCurrentStitch].x = StitchBuffer[iStitch].x;
					StitchBuffer[iCurrentStitch++].y = StitchBuffer[iStitch].y;
				}
				PCSHeader.stitchCount = iCurrentStitch;
				coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void movlayr(unsigned codedLayer) {
	unsigned	iForm = 0, iStitch = 0, iCurrentForm = 0;
	const unsigned	codedStitchLayer = codedLayer << (LAYSHFT - 1);


	if (SelectedFormCount) {
		savdo();
		//  ToDo - use a form map rather than a marked stitch map 
		boost::dynamic_bitset<> formMap(FormIndex);
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			iCurrentForm = SelectedFormList[iForm];
			FormList[iCurrentForm].attribute = (FormList[iCurrentForm].attribute&NFRMLMSK) | codedLayer;
			formMap.set(iCurrentForm);
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute&ALTYPMSK) {
				iCurrentForm = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
				if (formMap.test(iCurrentForm))
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute&NLAYMSK | codedStitchLayer;
			}
		}
		SelectedFormCount = 0;
		StateMap.reset(StateFlag::FORMSEL);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			savdo();
			FormList[ClosestFormToCursor].attribute = (FormList[ClosestFormToCursor].attribute&NFRMLMSK) | codedLayer;
			StateMap.reset(StateFlag::FORMSEL);
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (StitchBuffer[iStitch].attribute&ALTYPMSK && ((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor)
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute&NLAYMSK | codedStitchLayer;
			}
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				savdo();
				rngadj();
				for (iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++)
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute&NLAYMSK | codedStitchLayer;
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void join() {
	const unsigned	savedFormIndex = ClosestFormToCursor;
	unsigned	iVertex = 0, vertexCount = 0;


	StateMap.set(StateFlag::FRMSAM);
	if (FormIndex > 1 && StateMap.test(StateFlag::FORMSEL) && closfrm()) {
		vertexCount = FormList[ClosestFormToCursor].vertexCount;
		std::vector<fPOINT> vertexList(vertexCount);
		for (iVertex = 0; iVertex < vertexCount; iVertex++) {
			vertexList[iVertex].x = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].x;
			vertexList[iVertex].y = FormList[ClosestFormToCursor].vertices[ClosestVertexToCursor].y;
			ClosestVertexToCursor = nxt(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::DELTO);
		frmdel();
		if (savedFormIndex > ClosestFormToCursor)
			ClosestFormToCursor = savedFormIndex - 1;
		else
			ClosestFormToCursor = savedFormIndex;
		fPOINT* insertedVertex = &FormList[ClosestFormToCursor].vertices[FormList[ClosestFormToCursor].vertexCount];
		if (insertedVertex) {
			fltspac(insertedVertex, vertexCount);
			for (iVertex = 0; iVertex < vertexCount; iVertex++) {
				insertedVertex[iVertex].x = vertexList[iVertex].x;
				insertedVertex[iVertex].y = vertexList[iVertex].y;
			}
		}
		SelectedForm = &FormList[ClosestFormToCursor];
		SelectedForm->vertexCount += vertexCount;
		SelectedForm->rectangle.left = SelectedForm->rectangle.right = SelectedForm->vertices[0].x;
		SelectedForm->rectangle.top = SelectedForm->rectangle.bottom = SelectedForm->vertices[0].y;
		for (iVertex = 1; iVertex < SelectedForm->vertexCount; iVertex++) {
			if (SelectedForm->vertices[iVertex].x < SelectedForm->rectangle.left)
				SelectedForm->rectangle.left = SelectedForm->vertices[iVertex].x;
			if (SelectedForm->vertices[iVertex].x > SelectedForm->rectangle.right)
				SelectedForm->rectangle.right = SelectedForm->vertices[iVertex].x;
			if (SelectedForm->vertices[iVertex].y > SelectedForm->rectangle.top)
				SelectedForm->rectangle.top = SelectedForm->vertices[iVertex].y;
			if (SelectedForm->vertices[iVertex].y < SelectedForm->rectangle.bottom)
				SelectedForm->rectangle.bottom = SelectedForm->vertices[iVertex].y;
		}
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	StateMap.reset(StateFlag::FRMSAM);
}

bool chkbfil() noexcept {
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
	const unsigned	savedFormIndex = ClosestFormToCursor;

	savdo();
	for (ClosestFormToCursor = 0; ClosestFormToCursor < FormIndex; ClosestFormToCursor++)
		refilfn();
	ClosestFormToCursor = savedFormIndex;
	fvars(ClosestFormToCursor);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

bool notsel() {
	unsigned	iForm;

	for (iForm = 0; iForm < SelectedFormCount; iForm++) {
		if (gsl::narrow<unsigned>(SelectedFormList[iForm]) == ClosestFormToCursor)
			return 0;
	}
	return 1;
}

void nufsel() {
	if (FormIndex) {
		StateMap.reset(StateFlag::FORMSEL);
		if (StateMap.testAndReset(StateFlag::WASEL))
			SelectedFormList[SelectedFormCount++] = PreviousFormIndex;
		if (notsel())
			SelectedFormList[SelectedFormCount++] = gsl::narrow<unsigned short>(ClosestFormToCursor);
		StateMap.set(StateFlag::RESTCH);
	}
}

void frmadj(unsigned formIndex) noexcept {
	unsigned	iVertex = 0;

	fvars(formIndex);
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		CurrentFormVertices[iVertex].x += FormMoveDelta.x;
		CurrentFormVertices[iVertex].y -= FormMoveDelta.y;
	}
	frmout(formIndex);
}

unsigned setRmp(unsigned bit) noexcept {
	return _bittestandset((long *)MarkedStitchMap, bit) ? 0 : 0xffffffff;
}

void clRmap(unsigned mapSize) noexcept {
	memset(MarkedStitchMap, 0, mapSize * sizeof(*MarkedStitchMap));
}

#if PESACT

bool setrc(unsigned bit) {
	return _bittestandset((long *)MarkedStitchMap, bit);
}

#endif

void frmsadj() noexcept {
	unsigned	iForm = 0, iStitch = 0;

	boost::dynamic_bitset<> formMap(FormIndex);
	for (iForm = 0; iForm < SelectedFormCount; iForm++) {
		formMap.set(SelectedFormList[iForm]);
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].attribute&ALTYPMSK && formMap.test((StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT)) {
			StitchBuffer[iStitch].x += FormMoveDelta.x;
			StitchBuffer[iStitch].y -= FormMoveDelta.y;
		}
	}
}

void stchrct2px(fRECTANGLE stitchRect, RECT* screenRect) {
	dPOINT	stitchCoord = { stitchRect.left,stitchRect.top };
	POINT	screenCoord = {};

	sCor2px(stitchCoord, &screenCoord);
	screenRect->left = screenCoord.x;
	screenRect->top = screenCoord.y;
	stitchCoord.x = stitchRect.right;
	stitchCoord.y = stitchRect.bottom;
	sCor2px(stitchCoord, &screenCoord);
	screenRect->right = screenCoord.x;
	screenRect->bottom = screenCoord.y;
}

void getbig() noexcept {
	unsigned	iForm = 0, iStitch = 0;
	fRECTANGLE*	trct = nullptr;

	AllItemsRect.bottom = AllItemsRect.left = 1e9;
	AllItemsRect.top = AllItemsRect.right = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		trct = &FormList[iForm].rectangle;
		if (trct->bottom < AllItemsRect.bottom)
			AllItemsRect.bottom = trct->bottom;
		if (trct->left < AllItemsRect.left)
			AllItemsRect.left = trct->left;
		if (trct->right > AllItemsRect.right)
			AllItemsRect.right = trct->right;
		if (trct->top > AllItemsRect.top)
			AllItemsRect.top = trct->top;
	}
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].x < AllItemsRect.left)
			AllItemsRect.left = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].x > AllItemsRect.right)
			AllItemsRect.right = StitchBuffer[iStitch].x;
		if (StitchBuffer[iStitch].y < AllItemsRect.bottom)
			AllItemsRect.bottom = StitchBuffer[iStitch].y;
		if (StitchBuffer[iStitch].y > AllItemsRect.top)
			AllItemsRect.top = StitchBuffer[iStitch].y;
	}
}

void selal() {
	StateMap.reset(StateFlag::FORMSEL);
	SelectedFormCount = 0;
	StateMap.reset(StateFlag::SELBOX);
	StateMap.reset(StateFlag::GRPSEL);
	getbig();
	ZoomRect.bottom = 0;
	ZoomRect.left = 0;
	ZoomRect.right = UnzoomedRect.x;
	ZoomRect.top = UnzoomedRect.y;
	ZoomFactor = 1;
	StateMap.reset(StateFlag::ZUMED);
	movStch();
	NearestCount = 0;
	StateMap.reset(StateFlag::RUNPAT);
	duzrat();
	stchrct2px(AllItemsRect, &SelectedFormsRect);
	StateMap.set(StateFlag::BIGBOX);
	StateMap.set(StateFlag::RESTCH);
}

void frmpnts(unsigned type) noexcept {
	unsigned	iStitch = 0;
	const unsigned	trg = (ClosestFormToCursor << 4) | type;

	while (iStitch < PCSHeader.stitchCount && (StitchBuffer[iStitch].attribute&(ALTYPMSK | FRMSK)) != trg)
		iStitch++;
	ClosestPointIndex = iStitch;
	if (PCSHeader.stitchCount > 0) {
		iStitch = PCSHeader.stitchCount - 1;
	}
	else {
		iStitch = 0;
	}
	while (iStitch > ClosestPointIndex && (StitchBuffer[iStitch].attribute&(ALTYPMSK | FRMSK)) != trg)
		iStitch--;
	GroupStitchIndex = iStitch;
}

void selfil(unsigned type) {
	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		if (type == FRMFIL && !FormList[ClosestFormToCursor].fillType) {
			tabmsg(IDS_FSELF);
			return;
		}
		if (type == FRMBFIL && !FormList[ClosestFormToCursor].edgeType) {
			tabmsg(IDS_FSELB);
			return;
		}
		if (type == FRMAPFIL && (FormList[ClosestFormToCursor].edgeType&NEGUND) != EDGEAPPL) {
			tabmsg(IDS_FSELA);
			return;
		}
		frmpnts(type);
		StateMap.set(StateFlag::GRPSEL);
		StateMap.reset(StateFlag::FORMSEL);
		rngadj();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		tabmsg(IDS_SEL1FRM);
}

bool notfstch(unsigned attribute) noexcept {
	if (attribute&NOTFRM)
		return 1;
	if (((attribute&FRMSK) >> FRMSHFT) == ClosestFormToCursor)
		return 0;
	return 1;
}

void selalfil() {
	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		ClosestPointIndex = 0;
		while (ClosestPointIndex < PCSHeader.stitchCount && notfstch(StitchBuffer[ClosestPointIndex].attribute))
			ClosestPointIndex++;
		if (ClosestPointIndex != PCSHeader.stitchCount) {
			if (ClosestPointIndex)
				ClosestPointIndex--;
			GroupStitchIndex = PCSHeader.stitchCount - 1;
			while (GroupStitchIndex > ClosestPointIndex && notfstch(StitchBuffer[GroupStitchIndex].attribute))
				GroupStitchIndex--;
			StateMap.set(StateFlag::GRPSEL);
			StateMap.reset(StateFlag::FORMSEL);
			rngadj();
			StateMap.set(StateFlag::RESTCH);
		}
		else
			tabmsg(IDS_FSELM);
	}
	else
		tabmsg(IDS_SEL1FRM);
}

bool frmrng(unsigned iForm, RANGE* range) noexcept {
	range->start = 0;
	range->finish = PCSHeader.stitchCount;
	if (FormList[iForm].fillType || FormList[iForm].edgeType) {
		while (range->start < PCSHeader.stitchCount && notfstch(StitchBuffer[range->start].attribute))
			range->start++;
		range->finish = PCSHeader.stitchCount - 1;
		while (range->finish > range->start && notfstch(StitchBuffer[range->finish].attribute))
			range->finish--;
		if (range->finish > range->start)
			return 1;
		else
			return 0;
	}
	else
		return 0;
}

void bhfn(unsigned start, unsigned finish, double spacing) noexcept {
	dPOINT		delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
						  (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	const double	length = hypot(delta.x, delta.y);
	const unsigned	count = length / spacing;
	dPOINT		step = { (delta.x / count),
						 (delta.y / count) };
	dPOINT		innerPoint = { CurrentFormVertices[start].x ,
							   CurrentFormVertices[start].y };
	dPOINT		outerStep = {}, firstPoint = {}, outerPoint = {};
	unsigned	iStep = 0;

	RotationAngle = atan2(-delta.x, delta.y);
	outerStep.x = SelectedForm->borderSize*cos(RotationAngle);
	outerStep.y = SelectedForm->borderSize*sin(RotationAngle);
	if (count) {
		for (iStep = 0; iStep < count - 1; iStep++) {
			firstPoint.x = innerPoint.x + step.x;
			firstPoint.y = innerPoint.y + step.y;
			outerPoint.x = firstPoint.x + outerStep.x;
			outerPoint.y = firstPoint.y + outerStep.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x = innerPoint.x;
			OSequence[SequenceIndex++].y = innerPoint.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		firstPoint.x = innerPoint.x + step.x;
		firstPoint.y = innerPoint.y + step.y;
		OSequence[SequenceIndex].x = firstPoint.x;
		OSequence[SequenceIndex++].y = firstPoint.y;
		OSequence[SequenceIndex].x = innerPoint.x;
		OSequence[SequenceIndex++].y = innerPoint.y;
	}
}

void bhcrnr(unsigned vertex) {
	const unsigned	nextVertex = nxt(vertex);
	dPOINT		delta = {}, point = {};
	double		length = 0.0, ratio = 0.0;

	if (StateMap.test(StateFlag::INDIR)) {
		delta.x = OutsidePoints[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = OutsidePoints[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	else {
		delta.x = InsidePoints[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = InsidePoints[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	length = hypot(delta.x, delta.y);
	ratio = ButtonholeCornerLength / length;
	delta.x *= ratio;
	delta.y *= ratio;
	point.x = CurrentFormVertices[nextVertex].x + delta.x;
	point.y = CurrentFormVertices[nextVertex].y + delta.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[nextVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[nextVertex].y;
	OSequence[SequenceIndex].x = point.x;
	OSequence[SequenceIndex++].y = point.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[nextVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[nextVertex].y;
	OSequence[SequenceIndex].x = point.x;
	OSequence[SequenceIndex++].y = point.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[nextVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[nextVertex].y;
}

void bhbrd(double spacing) {
	unsigned	vertex = getlast();
	unsigned	nextVertex = 0, iVertex = 0;

	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[vertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[vertex].y;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		nextVertex = nxt(vertex);
		bhfn(vertex, nextVertex, spacing);
		bhcrnr(vertex);
		vertex = nextVertex;
	}
}

void blbrd(double spacing) {
	unsigned	iVertex = 0;

	SequenceIndex = 0;
	OSequence[SequenceIndex].x = CurrentFormVertices[0].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[0].y;
	for (iVertex = 0; iVertex < VertexCount - 2; iVertex++) {
		bhfn(iVertex, iVertex + 1, spacing);
		bhcrnr(iVertex);
	}
	bhfn(iVertex, iVertex + 1, spacing);
	OSequence[SequenceIndex].x = CurrentFormVertices[VertexCount - 1].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[VertexCount - 1].y;
}

void bholbrd() {
	deleclp(ClosestFormToCursor);
	SelectedForm->borderSize = BorderWidth;
	bsizpar();
	SelectedForm->edgeType = EDGEBHOL;
	SelectedForm->edgeSpacing = LineSpacing;
	SelectedForm->borderColor = ActiveColor;
	savblen(ButtonholeCornerLength);
	refilfn();
}

void bhol() {
	const double	savedSpacing = LineSpacing;
	unsigned	iForm = 0;

	if (filmsgs(FML_BHOL))
		return;
	LineSpacing = savedSpacing;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			bholbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			bholbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void fcntr() {
	unsigned	iForm = 0, iVertex = 0, iStitch = 0, iCurrentForm = 0, codedForm = 0;
	dPOINT		initialCenter = {};
	dPOINT		currentCenter = {};
	dPOINT		delta = {};

	if (SelectedFormCount) {
		savdo();
		iCurrentForm = SelectedFormList[0];
		initialCenter.x = (FormList[iCurrentForm].rectangle.right - FormList[iCurrentForm].rectangle.left) / 2 + FormList[iCurrentForm].rectangle.left;
		initialCenter.y = (FormList[iCurrentForm].rectangle.top - FormList[iCurrentForm].rectangle.bottom) / 2 + FormList[iCurrentForm].rectangle.bottom;
		for (iForm = 1; iForm < SelectedFormCount; iForm++) {
			iCurrentForm = SelectedFormList[iForm];
			currentCenter.x = (FormList[iCurrentForm].rectangle.right - FormList[iCurrentForm].rectangle.left) / 2 + FormList[iCurrentForm].rectangle.left;
			currentCenter.y = (FormList[iCurrentForm].rectangle.top - FormList[iCurrentForm].rectangle.bottom) / 2 + FormList[iCurrentForm].rectangle.bottom;
			delta.x = initialCenter.x - currentCenter.x;
			delta.y = initialCenter.y - currentCenter.y;
			SelectedForm = &FormList[iCurrentForm];
			for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
				SelectedForm->vertices[iVertex].x += delta.x;
				SelectedForm->vertices[iVertex].y += delta.y;
			}
			frmout(iCurrentForm);
			codedForm = (iCurrentForm << 4);
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute&FRMSK) == codedForm && !(StitchBuffer[iStitch].attribute&NOTFRM)) {
					StitchBuffer[iStitch].x += delta.x;
					StitchBuffer[iStitch].y += delta.y;
				}
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else
		tabmsg(IDS_SELC);
}

void boxsel() {
	StateMap.reset(StateFlag::INSRT);
	if (!StateMap.test(StateFlag::THUMSHO)) {
		StateMap.reset(StateFlag::BZUM);
		StateMap.reset(StateFlag::BZUMIN);
		StateMap.set(StateFlag::VCAPT);
		SetCapture(ThrEdWindow);
		StateMap.set(StateFlag::RESTCH);
	}
}

void clpcrnr(std::vector<fPOINT> clipFillData, unsigned vertex) {
	const unsigned	nextVertex = nxt(vertex);
	unsigned	iStitch = 0;
	dPOINT		delta = {}, point = {};
	fPOINT		tpnt = {};
	double		length = 0.0, ratio = 0.0;
	fPOINTATTR	referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left),ClipRect.top };

	if (StateMap.test(StateFlag::INDIR)) {
		delta.x = OutsidePoints[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = OutsidePoints[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	else {
		delta.x = InsidePoints[nextVertex].x - CurrentFormVertices[nextVertex].x;
		delta.y = InsidePoints[nextVertex].y - CurrentFormVertices[nextVertex].y;
	}
	RotationAngle = atan2(delta.y, delta.x) + PI / 2;
	rotang1(referencePoint, &ClipReference);
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
		rotang1(ClipBuffer[iStitch], &clipFillData[iStitch]);
	length = hypot(delta.x, delta.y);
	ratio = getplen() / length;
	delta.x *= ratio;
	delta.y *= ratio;
	point.x = CurrentFormVertices[nextVertex].x + delta.x;
	point.y = CurrentFormVertices[nextVertex].y + delta.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[nextVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[nextVertex].y;
	OSequence[SequenceIndex].x = point.x;
	OSequence[SequenceIndex++].y = point.y;
	OSequence[SequenceIndex].x = CurrentFormVertices[nextVertex].x;
	OSequence[SequenceIndex++].y = CurrentFormVertices[nextVertex].y;
	OSequence[SequenceIndex].x = point.x;
	OSequence[SequenceIndex++].y = point.y;
	tpnt.x = static_cast<float>(point.x);
	tpnt.y = static_cast<float>(point.y);
	if (!ritclp(clipFillData, tpnt)) {
		OSequence[SequenceIndex].x = point.x;
		OSequence[SequenceIndex++].y = point.y;
		OSequence[SequenceIndex].x = CurrentFormVertices[nextVertex].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[nextVertex].y;
	}
}

void picfn(std::vector<fPOINT> clipFillData, unsigned start, unsigned finish, double spacing) {
	dPOINT		delta = { (CurrentFormVertices[finish].x - CurrentFormVertices[start].x),
						  (CurrentFormVertices[finish].y - CurrentFormVertices[start].y) };
	const double	length = hypot(delta.x, delta.y);
	dPOINT		innerPoint = { CurrentFormVertices[start].x ,CurrentFormVertices[start].y };
	fPOINTATTR	referencePoint = { ((ClipRect.right - ClipRect.left) / 2 + ClipRect.left),ClipRect.top };
	double		tdub = 0.0;
	dPOINT		step = {}, outerStep = {}, firstPoint = {}, outerPoint = {};
	fPOINT		tpnt = {};
	unsigned	count = 0, iStitch = 0, iStep = 0;

	RotationAngle = atan2(-delta.x, delta.y);
	outerStep.x = SelectedForm->borderSize*cos(RotationAngle);
	outerStep.y = SelectedForm->borderSize*sin(RotationAngle);
	spacing += ClipRectSize.cx;
	count = length / spacing;
	RotationAngle = atan2(delta.y, delta.x);
	rotang1(referencePoint, &ClipReference);
	if (count) {
		step.x = 0;
		step.y = 0;
		if (count > 1) {
			tdub = ((length - count * spacing) / (count - 1) + spacing) / length;
			step.x = delta.x*tdub;
			step.y = delta.y*tdub;
		}
		for (iStitch = 0; iStitch < ClipStitchCount; iStitch++)
			rotang1(ClipBuffer[iStitch], &clipFillData[iStitch]);
		bool flag = true;
		for (iStep = 0; iStep < count - 1; iStep++) {
			firstPoint.x = innerPoint.x + step.x;
			firstPoint.y = innerPoint.y + step.y;
			outerPoint.x = firstPoint.x + outerStep.x;
			outerPoint.y = firstPoint.y + outerStep.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x = innerPoint.x;
			OSequence[SequenceIndex++].y = innerPoint.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			OSequence[SequenceIndex].x = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			tpnt.x = static_cast<float>(outerPoint.x);
			tpnt.y = static_cast<float>(outerPoint.y);
			if (ritclp(clipFillData, tpnt)) {
				flag = false;
				break;
			}
			OSequence[SequenceIndex].x = outerPoint.x;
			OSequence[SequenceIndex++].y = outerPoint.y;
			OSequence[SequenceIndex].x = firstPoint.x;
			OSequence[SequenceIndex++].y = firstPoint.y;
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		if (flag) {
			OSequence[SequenceIndex].x = CurrentFormVertices[finish].x;
			OSequence[SequenceIndex++].y = CurrentFormVertices[finish].y;
			OSequence[SequenceIndex].x = innerPoint.x;
			OSequence[SequenceIndex++].y = innerPoint.y;
		}
	}
}

void clpic() {
	const unsigned short start = 0;
	unsigned	iVertex = 0;
	unsigned	currentVertex = start, nextVertex = 0;

	SequenceIndex = 0;
	StateMap.reset(StateFlag::CLPBAK);
	HorizontalLength2 = ClipRectSize.cx / 2;
	HorizontalLength = ClipRectSize.cx;
	std::vector<fPOINT> clipFillData(ClipStitchCount);
	RotationCenter.x = (ClipRect.right - ClipRect.left) / 2 + ClipRect.left;
	ClipReference.y = RotationCenter.y = (ClipRect.top - ClipRect.bottom) / 2 + ClipRect.bottom;
	ClipReference.x = ClipRect.left;
	satout(20);
	if (SelectedForm->type == FRMLINE) {
		for (iVertex = 0; iVertex < VertexCount - 2; iVertex++) {
			picfn(clipFillData, iVertex, iVertex + 1, SelectedForm->edgeSpacing);
			clpcrnr(clipFillData, iVertex);
		}
		picfn(clipFillData, iVertex, iVertex + 1, SelectedForm->edgeSpacing);
	}
	else {
		if (!SelectedForm->fillType) {
			OSequence[SequenceIndex].x = CurrentFormVertices[start].x;
			OSequence[SequenceIndex++].y = CurrentFormVertices[start].y;
		}
		currentVertex = start;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			nextVertex = nxt(currentVertex);
			picfn(clipFillData, currentVertex, nextVertex, SelectedForm->edgeSpacing);
			clpcrnr(clipFillData, currentVertex);
			currentVertex = nextVertex;
		}
		OSequence[SequenceIndex].x = CurrentFormVertices[currentVertex].x;
		OSequence[SequenceIndex++].y = CurrentFormVertices[currentVertex].y;
	}
}

void fspic() {
	unsigned	iStitch = 0;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEPICOT;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = PicotSpacing;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	savplen(ButtonholeCornerLength);
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->borderClipData[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->borderClipData[iStitch].y = ClipBuffer[iStitch].y;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	refilfn();
}

void picot() {
	unsigned	iForm = 0;

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
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						fspic();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fspic();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
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
	const unsigned	start = SelectedForm->angleOrClipData.guide.start;
	const fPOINT*	selectionStart = &CurrentFormVertices[start];
	if (selectionStart) {
		const unsigned	finish = SelectedForm->angleOrClipData.guide.finish;
		const unsigned	lowVertexIndex = start;
		const unsigned	highVertexIndex = VertexCount - start - 1;
		const unsigned	selectedVertexCount = finish - start;
		std::vector<unsigned> lowCounts(lowVertexIndex);
		std::vector<unsigned>	highCounts(highVertexIndex);
		std::vector<double>		lowLengths(lowVertexIndex);
		std::vector<double>		highLengths(highVertexIndex);
		std::vector<dPOINT>		lowDeltas(lowVertexIndex);
		std::vector<dPOINT>		highDeltas(highVertexIndex);
		std::vector<dPOINT>		lowSteps(lowVertexIndex);
		std::vector<dPOINT>		highSteps(highVertexIndex);
		std::vector<fPOINT>		lowVertices(lowVertexIndex);
		std::vector<dPOINT>		highVertices(highVertexIndex);
		unsigned	iVertex = 0, lowIndex = 0, highIndex = 0, selind = 0;
		double		lowLength = 0.0, highLength = 0.0, length = 0.0;
		double		lowSpacing = 0.0, highSpacing = 0.0;
		unsigned	highCount = 0, lowCount = 0, count = 0;
		dPOINT		lowStep = {}, lowPoint = {};
		dPOINT		highStep = {}, highPoint = {};
		dPOINT		delta = {};
		// ToDo - rename pols, polref, polin & poldif
		std::vector<PVEC>		pols(selectedVertexCount);
		PVEC		polref = {}, polin = {}, poldif = {};

		SequenceIndex = 0;
		for (iVertex = lowVertexIndex; iVertex != 0; iVertex--) {
			lowVertices[lowIndex].x = CurrentFormVertices[iVertex].x;
			lowVertices[lowIndex].y = CurrentFormVertices[iVertex].y;
			lowDeltas[lowIndex].x = CurrentFormVertices[iVertex - 1].x - CurrentFormVertices[iVertex].x;
			lowDeltas[lowIndex].y = CurrentFormVertices[iVertex - 1].y - CurrentFormVertices[iVertex].y;
			lowLengths[lowIndex] = hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
			lowLength += lowLengths[lowIndex];
			lowIndex++;
		}
		selind = 0;
		for (iVertex = start + 1; iVertex <= finish; iVertex++) {
			delta.x = CurrentFormVertices[iVertex].x - selectionStart[0].x;
			delta.y = CurrentFormVertices[iVertex].y - selectionStart[0].y;
			pols[selind].length = hypot(delta.x, delta.y);
			pols[selind].angle = atan2(delta.y, delta.x);
			selind++;
		}
		highIndex = 0; highLength = 0;
		for (iVertex = finish; iVertex < VertexCount - 1; iVertex++) {
			highVertices[highIndex].x = CurrentFormVertices[iVertex].x;
			highVertices[highIndex].y = CurrentFormVertices[iVertex].y;
			highDeltas[highIndex].x = CurrentFormVertices[iVertex + 1].x - CurrentFormVertices[iVertex].x;
			highDeltas[highIndex].y = CurrentFormVertices[iVertex + 1].y - CurrentFormVertices[iVertex].y;
			highLengths[highIndex] = hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
			highLength += highLengths[highIndex];
			highIndex++;
		}
		if (highLength > lowLength)
			length = lowLength;
		else
			length = highLength;
		count = length / SelectedForm->fillSpacing;
		if (highLength < lowLength) {
			lowSpacing = SelectedForm->fillSpacing;
			highSpacing = SelectedForm->fillSpacing*highLength / lowLength;
		}
		else {
			highSpacing = SelectedForm->fillSpacing;
			lowSpacing = SelectedForm->fillSpacing*lowLength / highLength;
		}
		for (iVertex = 0; iVertex < lowVertexIndex; iVertex++) {
			lowCounts[iVertex] = lowLengths[iVertex] / lowSpacing;
			lowSteps[iVertex].x = lowDeltas[iVertex].x / lowCounts[iVertex];
			lowSteps[iVertex].y = lowDeltas[iVertex].y / lowCounts[iVertex];
		}
		for (iVertex = 0; iVertex < highVertexIndex; iVertex++) {
			highCounts[iVertex] = highLengths[iVertex] / highSpacing;
			highSteps[iVertex].x = highDeltas[iVertex].x / highCounts[iVertex];
			highSteps[iVertex].y = highDeltas[iVertex].y / highCounts[iVertex];
		}
		lowIndex = highIndex = 0;
		StateMap.reset(StateFlag::FILDIR);
		lowCount = highCount = 0;
		delta.x = CurrentFormVertices[finish].x - CurrentFormVertices[start].x;
		delta.y = CurrentFormVertices[finish].y - CurrentFormVertices[start].y;
		polref.length = hypot(delta.x, delta.y);
		polref.angle = atan2(delta.y, delta.x);
		while (lowCount || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
			if (lowCount)
				lowCount--;
			else {
				if (lowIndex < lowVertexIndex) {
					lowCount = lowCounts[lowIndex];
					lowStep.x = lowSteps[lowIndex].x;
					lowStep.y = lowSteps[lowIndex].y;
					lowPoint.x = lowVertices[lowIndex].x;
					lowPoint.y = lowVertices[lowIndex].y;
					lowIndex++;
				}
			}
			if (highCount)
				highCount--;
			else {
				if (highIndex < highVertexIndex) {
					highCount = highCounts[highIndex];
					highStep.x = highSteps[highIndex].x;
					highStep.y = highSteps[highIndex].y;
					highPoint.x = highVertices[highIndex].x;
					highPoint.y = highVertices[highIndex].y;
					highIndex++;
				}
			}
			delta.x = highPoint.x - lowPoint.x;
			delta.y = highPoint.y - lowPoint.y;
			polin.angle = atan2(delta.y, delta.x);
			polin.length = hypot(delta.x, delta.y);
			poldif.angle = polin.angle - polref.angle;
			if (polref.length > 0.9*LineSpacing) {
				poldif.length = polin.length / polref.length;
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					OSequence[SequenceIndex].x = lowPoint.x;
					OSequence[SequenceIndex].y = lowPoint.y;
					SequenceIndex++;
					for (iVertex = 0; iVertex < (selectedVertexCount - 1); iVertex++) {
						RotationAngle = pols[iVertex].angle + poldif.angle;
						length = pols[iVertex].length*poldif.length;
						OSequence[SequenceIndex].x = lowPoint.x + cos(RotationAngle)*length;
						OSequence[SequenceIndex].y = lowPoint.y + sin(RotationAngle)*length;
						SequenceIndex++;
					}
				}
				else {
					OSequence[SequenceIndex].x = highPoint.x;
					OSequence[SequenceIndex].y = highPoint.y;
					SequenceIndex++;
					for (iVertex = selectedVertexCount - 1; iVertex != 0; iVertex--) {
						RotationAngle = pols[iVertex - 1].angle + poldif.angle;
						length = pols[iVertex - 1].length*poldif.length;
						OSequence[SequenceIndex].x = lowPoint.x + cos(RotationAngle)*length;
						OSequence[SequenceIndex].y = lowPoint.y + sin(RotationAngle)*length;
						SequenceIndex++;
					}
				}
			}
			lowPoint.x += lowStep.x;
			lowPoint.y += lowStep.y;
			highPoint.x += highStep.x;
			highPoint.y += highStep.y;
		}
		if (StateMap.test(StateFlag::FILDIR)) {
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
}

bool contsf(unsigned formIndex) {
	ClosestFormToCursor = formIndex;
	fvars(formIndex);
	if (SelectedForm->vertexCount > 4) {
		delclps(ClosestFormToCursor);
		deltx();
		chkcont();
		SelectedForm->fillSpacing = LineSpacing;
		SelectedForm->fillColor = ActiveColor;
		fsizpar();
		SelectedForm->attribute |= (ActiveLayer << 1);
		refilfn();
		return 1;
	}
	return 0;
}

void contfil() {
	unsigned	iForm = 0;

	if (filmsgs(FML_CONT))
		return;
	if (SelectedFormCount) {
		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			contsf(SelectedFormList[iForm]);
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		fvars(ClosestFormToCursor);
		savdo();
		if (contsf(ClosestFormToCursor)) {
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else
			tabmsg(IDS_CONT);
	}
}

void ribon() {
	unsigned	iVertex = 0, iGuide = 0, isBlunt = 0, iNewVertex = 0, savedFormIndex = 0;

	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (VertexCount > 2) {
			savdo();
			savedFormIndex = ClosestFormToCursor;
			satout(BorderWidth);

			HorizontalLength2 = BorderWidth / 2;
			FRMHED* formHeader = &FormList[FormIndex];
			if (formHeader) {
				frmclr(formHeader);
				iNewVertex = 0;
				formHeader->maxFillStitchLen = 9 * PFGRAN;
				formHeader->minFillStitchLen = MinStitchLength;
				MaxStitchLen = 9 * PFGRAN;
				if (SelectedForm->type == FRMLINE) {
					// Set blunt flags
					if (UserFlagMap.test(UserFlag::BLUNT))
						isBlunt = SBLNT | FBLNT;
					else
						isBlunt = 0;
					satends(isBlunt);
					formHeader->vertices = adflt(VertexCount << 1);
					formHeader->vertices[0].x = OutsidePoints[0].x;
					formHeader->vertices[iNewVertex++].y = OutsidePoints[0].y;
					for (iVertex = 0; iVertex < VertexCount; iVertex++) {
						formHeader->vertices[iNewVertex].x = InsidePoints[iVertex].x;
						formHeader->vertices[iNewVertex++].y = InsidePoints[iVertex].y;
					}
					for (iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						formHeader->vertices[iNewVertex].x = OutsidePoints[iVertex].x;
						formHeader->vertices[iNewVertex++].y = OutsidePoints[iVertex].y;
					}
				}
				else {
					formHeader->vertices = adflt((VertexCount << 1) + 2);
					formHeader->vertices[0].x = OutsidePoints[0].x;
					formHeader->vertices[iNewVertex++].y = OutsidePoints[0].y;
					formHeader->underlayIndent = IniFile.underlayIndent;
					for (iVertex = 0; iVertex < VertexCount; iVertex++) {
						formHeader->vertices[iNewVertex].x = InsidePoints[iVertex].x;
						formHeader->vertices[iNewVertex++].y = InsidePoints[iVertex].y;
					}
					formHeader->vertices[iNewVertex].x = InsidePoints[0].x;
					formHeader->vertices[iNewVertex++].y = InsidePoints[0].y;
					formHeader->vertices[iNewVertex].x = OutsidePoints[0].x;
					formHeader->vertices[iNewVertex++].y = OutsidePoints[0].y;
					for (iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						formHeader->vertices[iNewVertex].x = OutsidePoints[iVertex].x;
						formHeader->vertices[iNewVertex++].y = OutsidePoints[iVertex].y;
					}
				}
				formHeader->type = SAT;
				formHeader->fillColor = ActiveColor;
				formHeader->fillSpacing = LineSpacing;
				formHeader->lengthOrCount.stitchLength = IniFile.maxStitchLength;
				formHeader->vertexCount = iNewVertex;
				formHeader->attribute = 1;
				formHeader->wordParam = iNewVertex >> 1;
				formHeader->satinGuideCount = formHeader->wordParam - 2;
				formHeader->satinOrAngle.guide = adsatk(formHeader->satinGuideCount);
				if (StateMap.test(StateFlag::CNV2FTH)) {
					formHeader->fillType = FTHF;
					formHeader->fillInfo.feather.ratio = IniFile.featherRatio;
					formHeader->fillInfo.feather.upCount = IniFile.featherUpCount;
					formHeader->fillInfo.feather.downCount = IniFile.featherDownCount;
					formHeader->fillInfo.feather.fillType = IniFile.featherFillType;
					formHeader->fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
					formHeader->extendedAttribute = IniFile.featherType;
					formHeader->fillInfo.feather.count = IniFile.featherCount;
					formHeader->fillInfo.feather.color = (ActiveColor + 1)&COLMSK;
				}
				else
					formHeader->fillType = SATF;
				for (iGuide = 0; iGuide < formHeader->satinGuideCount; iGuide++) {
					formHeader->satinOrAngle.guide[iGuide].start = iGuide + 2;
					formHeader->satinOrAngle.guide[iGuide].finish = formHeader->vertexCount - iGuide - 1;
				}
				FormIndex++;
				frmout(FormIndex - 1);
				ClosestFormToCursor = FormIndex - 1;
				refilfn();
				ClosestFormToCursor = savedFormIndex;
				StateMap.set(StateFlag::DELTO);
				frmdel();
				ClosestFormToCursor = FormIndex - 1;
				StateMap.set(StateFlag::FORMSEL);
				StateMap.set(StateFlag::INIT);
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				throw;
			}
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
		if (StateMap.test(StateFlag::FORMSEL))
			duprot();
		else {
			if (StateMap.test(StateFlag::GRPSEL))
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
		StateMap.set(StateFlag::ENTRDUP);
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
	unsigned	iVertex = 0, ine = 0, count = 0;
	dPOINT		delta = {};
	double		length = 0.0, ratio = 0.0, deltaLength = 0.0, truncationDelta = 0.0;

	oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
	for (iVertex = 0; iVertex < VertexCount - 1; iVertex++) {
		delta.x = CurrentFormVertices[iVertex + 1].x - CurrentFormVertices[iVertex].x;
		delta.y = CurrentFormVertices[iVertex + 1].y - CurrentFormVertices[iVertex].y;
		length = hypot(delta.x, delta.y);
		count = length / ClipRectSize.cx + 0.5;
		ratio = (ClipRectSize.cx*count + 0.004) / length;
		CurrentFormVertices[iVertex + 1].x = CurrentFormVertices[iVertex].x + delta.x*ratio;
		CurrentFormVertices[iVertex + 1].y = CurrentFormVertices[iVertex].y + delta.y*ratio;
	}
	delta.x = CurrentFormVertices[0].x - CurrentFormVertices[1].x;
	delta.y = CurrentFormVertices[0].y - CurrentFormVertices[1].y;
	length = hypot(delta.x, delta.y);
	RotationAngle = atan2(delta.y, delta.x);
	// ToDo - what does this loop do?
	for (ine = 0; ine < 5; ine++) {
		delta.x = CurrentFormVertices[0].x - CurrentFormVertices[iVertex].x;
		delta.y = CurrentFormVertices[0].y - CurrentFormVertices[iVertex].y;
		deltaLength = hypot(delta.x, delta.y);
		count = deltaLength / ClipRectSize.cx;
		truncationDelta = deltaLength - count * ClipRectSize.cx;
		RotationAngle -= truncationDelta / length;
		delta.x = cos(RotationAngle)*length;
		delta.y = sin(RotationAngle)*length;
		CurrentFormVertices[0].x = CurrentFormVertices[1].x + delta.x;
		CurrentFormVertices[0].y = CurrentFormVertices[1].y + delta.y;
	}
	refil();
}

void shrnk() {
	fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FORMSEL) && SelectedForm->edgeType == EDGECLIP) {
		shrnks();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_FRMCLP, IDS_SHRNK);
}

void mvfrms(FRMHED* destination, const FRMHED* source, unsigned count) noexcept {
	memcpy(destination, source, count * sizeof (FRMHED));
}

void dufdat(std::vector<fPOINT> &tempClipPoints, std::vector<SATCON> &tempGuides, std::vector<fPOINT> &destinationFormVertices, std::vector<FRMHED> &destinationFormList, unsigned formIndex) {
	FRMHED*			destination = &destinationFormList[FormRelocationIndex++];
	const FRMHED*	source = &FormList[formIndex];

	mvfrms(destination, source, 1);
	mvflpnt(&destinationFormVertices[FormVertexIndex], destination->vertices, destination->vertexCount);
	destination->vertices = &FormVertices[FormVertexIndex];
	FormVertexIndex += destination->vertexCount;
	if (destination->satinGuideCount) {
		mvsatk(&tempGuides[SatinGuideIndex], destination->satinOrAngle.guide, destination->satinGuideCount);
		destination->satinOrAngle.guide = &SatinGuides[SatinGuideIndex];
		SatinGuideIndex += destination->satinGuideCount;
	}
	if (iseclpx(formIndex)) {
		mvflpnt(&tempClipPoints[ClipPointIndex], destination->borderClipData, destination->clipEntries);
		destination->borderClipData = &ClipPoints[ClipPointIndex];
		ClipPointIndex += destination->clipEntries;
	}
	if (isclpx(formIndex)) {
		mvflpnt(&tempClipPoints[ClipPointIndex], destination->angleOrClipData.clip, destination->lengthOrCount.clipCount);
		destination->angleOrClipData.clip = &ClipPoints[ClipPointIndex];
		ClipPointIndex += destination->lengthOrCount.clipCount;
	}
}

void stchfrm(unsigned formIndex, unsigned* attribute) noexcept {
	if (attribute) {
		*attribute &= NFRMSK;
		*attribute |= formIndex << FRMSHFT;
	}
}

void frmnumfn(unsigned newFormIndex) {
	unsigned	iForm = 0, iStitch = 0, sourceForm = 0, start = 0, finish = 0, decodedFormIndex = 0;

	if (newFormIndex != ClosestFormToCursor) {
		if (ClosestFormToCursor > newFormIndex) {
			start = newFormIndex;
			finish = ClosestFormToCursor;
		}
		else {
			start = ClosestFormToCursor;
			finish = newFormIndex;
		}
		sourceForm = FormRelocationIndex = 0;

		std::vector<FRMHED> tempFormList(FormIndex);
		// ToDo - find something better than MAXITEMS
		std::vector<fPOINT> tempFormVertices(MAXITEMS);
		std::vector<SATCON> tempGuides(SatinGuideIndex);
		std::vector<fPOINT> tempClipPoints(MAXITEMS);

		FormVertexIndex = SatinGuideIndex = ClipPointIndex = 0;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			if (iForm == newFormIndex)
				dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, ClosestFormToCursor);
			else {
				if (sourceForm == ClosestFormToCursor) {
					sourceForm++;
				}
				dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, sourceForm++);
			}
		}
		mvfrms(FormList, &tempFormList[0], FormIndex);
		mvflpnt(FormVertices, &tempFormVertices[0], FormVertexIndex);
		// ToDo - 'if' is only required while we are passing memory rather than vector
		if (SatinGuideIndex) {
			mvsatk(SatinGuides, &tempGuides[0], SatinGuideIndex);
		}
		mvflpnt(ClipPoints, &tempClipPoints[0], ClipPointIndex);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (StitchBuffer[iStitch].attribute&SRTYPMSK) {
				decodedFormIndex = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
				if (decodedFormIndex == ClosestFormToCursor)
					stchfrm(newFormIndex, &StitchBuffer[iStitch].attribute);
				else {
					if (decodedFormIndex >= start && decodedFormIndex <= finish) {
						if (newFormIndex < ClosestFormToCursor)
							stchfrm(decodedFormIndex + 1, &StitchBuffer[iStitch].attribute);
						else
							stchfrm(decodedFormIndex - 1, &StitchBuffer[iStitch].attribute);
					}
				}
			}
		}
		ClosestFormToCursor = newFormIndex;
		ritnum(STR_NUMFRM, ClosestFormToCursor);
	}
}

void frmnum() {
	std::string fmtStr;

	loadString(fmtStr, IDS_FRML);
	if (FormIndex && StateMap.test(StateFlag::FORMSEL)) {
		shoMsg(fmt::format(fmtStr, FormIndex));
		StateMap.set(StateFlag::NUMIN);
		StateMap.set(StateFlag::ENTRFNUM);
		numWnd();
	}
	else
		shoseln(IDS_FRM1MSG, IDS_SETFRM);
}

constexpr unsigned duat(unsigned attribute) {
	const unsigned type = ((attribute >> TYPSHFT) + 1) & 3;
	const unsigned frm = (attribute & FRMSK) >> 2;

	return type | frm;
}

void srtf(std::vector<fPOINTATTR> &tempStitchBuffer, unsigned start, unsigned finish) {
	unsigned	iForm = 0, iStitch = 0, stitchAccumulator = 0, swap = 0;

	if (start != finish) {
		std::vector<unsigned> stitchHistogram(FormIndex << 2);
		for (iStitch = start; iStitch < finish; iStitch++)
			stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++;
		stitchAccumulator = start;
		for (iForm = 0; iForm < FormIndex << 2; iForm++) {
			swap = stitchHistogram[iForm];
			stitchHistogram[iForm] = stitchAccumulator;
			stitchAccumulator += swap;
		}
		for (iStitch = start; iStitch < finish; iStitch++) {
			StitchBuffer[stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++] = tempStitchBuffer[iStitch];
		}
	}
}

void srtbyfrm() {
	unsigned	iStitch = 0, iColor = 0, colorAccumulator = 0, swap = 0;
	unsigned	colorHistogram[16] = { 0 };
	unsigned	color[16] = { 0 };

	if (FormIndex) {
		savdo();
		color[AppliqueColor] = 0;
		for (iColor = 0; iColor < 16; iColor++) {
			if (iColor != AppliqueColor)
				color[iColor] = iColor + 1;
		}
		std::vector<fPOINTATTR> tempStitchBuffer(PCSHeader.stitchCount);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			colorHistogram[color[StitchBuffer[iStitch].attribute & 0xf]]++;
		colorAccumulator = 0;
		for (iColor = 0; iColor < 16; iColor++) {
			swap = colorHistogram[iColor];
			colorHistogram[iColor] = colorAccumulator;
			colorAccumulator += swap;
		}
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			tempStitchBuffer[colorHistogram[color[StitchBuffer[iStitch].attribute & 0xf]]++] = StitchBuffer[iStitch];
		}
		srtf(tempStitchBuffer, 0, colorHistogram[0]);
		for (iColor = 0; iColor < 15; iColor++) {
			srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[iColor + 1]);
		}
	}
	else
		srtcol();
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void dufcntr(dPOINT* center) noexcept {
	unsigned	iForm = 0;
	fRECTANGLE*	formRect = nullptr;
	fRECTANGLE	bigRect = {};

	formRect = &FormList[SelectedFormList[0]].rectangle;
	bigRect.left = formRect->left;
	bigRect.right = formRect->right;
	bigRect.top = formRect->top;
	bigRect.bottom = formRect->bottom;
	for (iForm = 1; iForm < SelectedFormCount; iForm++) {
		formRect = &FormList[SelectedFormList[iForm]].rectangle;
		if (formRect->left < bigRect.left)
			bigRect.left = formRect->left;
		if (formRect->right > bigRect.right)
			bigRect.right = formRect->right;
		if (formRect->bottom < bigRect.bottom)
			bigRect.bottom = formRect->bottom;
		if (formRect->top > bigRect.top)
			bigRect.top = formRect->top;
	}
	center->x = (bigRect.right - bigRect.left) / 2 + bigRect.left;
	center->y = (bigRect.top - bigRect.bottom) / 2 + bigRect.bottom;
}

void cntrx() {
	dPOINT		markCenter = {};
	dPOINT		selectedCenter = {};
	unsigned	iForm = 0, iStitch = 0;
	bool		flag = false;
	fRECTANGLE*	formRect = nullptr;
	fRECTANGLE	groupRect = {};

	if (StateMap.test(StateFlag::GMRK)) {
		markCenter.x = ZoomMarkPoint.x;
		markCenter.y = ZoomMarkPoint.y;
	}
	else {
		markCenter.x = UnzoomedRect.x / 2;
		markCenter.y = UnzoomedRect.y / 2;
	}
	if (SelectedFormCount) {
		flag = true;
		savdo();
		dufcntr(&selectedCenter);
		FormMoveDelta.x = markCenter.x - selectedCenter.x;
		FormMoveDelta.y = -markCenter.y + selectedCenter.y;
		if (StateMap.test(StateFlag::CNTRV))
			FormMoveDelta.y = 0;
		if (StateMap.test(StateFlag::CNTRH))
			FormMoveDelta.x = 0;
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			frmadj(SelectedFormList[iForm]);
		frmsadj();
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			flag = true;
			savdo();
			formRect = &FormList[ClosestFormToCursor].rectangle;
			selectedCenter.x = (formRect->right - formRect->left) / 2 + formRect->left;
			selectedCenter.y = (formRect->top - formRect->bottom) / 2 + formRect->bottom;
			FormMoveDelta.x = markCenter.x - selectedCenter.x;
			FormMoveDelta.y = -markCenter.y + selectedCenter.y;
			if (StateMap.test(StateFlag::CNTRV))
				FormMoveDelta.y = 0;
			if (StateMap.test(StateFlag::CNTRH))
				FormMoveDelta.x = 0;
			frmadj(ClosestFormToCursor);
			for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (StitchBuffer[iStitch].attribute&ALTYPMSK && (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT == ClosestFormToCursor) {
					StitchBuffer[iStitch].x += FormMoveDelta.x;
					StitchBuffer[iStitch].y -= FormMoveDelta.y;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				flag = true;
				savdo();
				rngadj();
				groupRect.right = groupRect.left = StitchBuffer[GroupStartStitch].x;
				groupRect.top = groupRect.bottom = StitchBuffer[GroupStartStitch].y;
				for (iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
					if (StitchBuffer[iStitch].x < groupRect.left)
						groupRect.left = StitchBuffer[iStitch].x;
					if (StitchBuffer[iStitch].x > groupRect.right)
						groupRect.right = StitchBuffer[iStitch].x;
					if (StitchBuffer[iStitch].y < groupRect.bottom)
						groupRect.bottom = StitchBuffer[iStitch].y;
					if (StitchBuffer[iStitch].y > groupRect.top)
						groupRect.top = StitchBuffer[iStitch].y;
				}
				selectedCenter.x = (groupRect.right - groupRect.left) / 2 + groupRect.left;
				selectedCenter.y = (groupRect.top - groupRect.bottom) / 2 + groupRect.bottom;
				FormMoveDelta.x = markCenter.x - selectedCenter.x;
				FormMoveDelta.y = -markCenter.y + selectedCenter.y;
				if (StateMap.test(StateFlag::CNTRV))
					FormMoveDelta.y = 0;
				if (StateMap.test(StateFlag::CNTRH))
					FormMoveDelta.x = 0;
				for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x += FormMoveDelta.x;
					StitchBuffer[iStitch].y -= FormMoveDelta.y;
				}
			}
			else
				shoseln(IDS_FGRPF, IDS_CENT);
		}
	}
	if (flag)
		StateMap.set(StateFlag::RESTCH);
}

void centir() {
	dPOINT		hoopCenter = {};
	dPOINT		itemCenter = {};
	dPOINT		delta = {};
	unsigned	iVertex = 0, iForm = 0, iStitch = 0;

	StateMap.reset(StateFlag::BIGBOX);
	getbig();
	itemCenter.x = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
	itemCenter.y = (AllItemsRect.top - AllItemsRect.bottom) / 2 + AllItemsRect.bottom;
	hoopCenter.x = UnzoomedRect.x / 2;
	hoopCenter.y = UnzoomedRect.y / 2;
	delta.x = hoopCenter.x - itemCenter.x;
	delta.y = hoopCenter.y - itemCenter.y;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].x += delta.x;
		StitchBuffer[iStitch].y += delta.y;
	}
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++) {
		FormVertices[iVertex].x += delta.x;
		FormVertices[iVertex].y += delta.y;
	}
	for (iForm = 0; iForm < FormIndex; iForm++)
		frmout(iForm);
	setfchk();
	StateMap.set(StateFlag::RESTCH);
}

void bean(unsigned start, unsigned finish) {
	unsigned	iOutputStitch = start;
	unsigned	iSourceStitch = start;
	unsigned	iCopyStitch = MAXITEMS;

	BeanCount = 0;
	mvstch(iCopyStitch++, iSourceStitch);
	if (StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) {
		mvstch(iCopyStitch++, iSourceStitch + 1);
		mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	mvstch(iCopyStitch++, iSourceStitch);
	if (StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) {
		mvstch(iCopyStitch++, iSourceStitch + 1);
		mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < finish - 2) {
		mvstch(iCopyStitch++, iSourceStitch);
		if ((StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) &&
			(StitchBuffer[iSourceStitch - 2].x != StitchBuffer[iSourceStitch].x || StitchBuffer[iSourceStitch - 2].y != StitchBuffer[iSourceStitch].y)) {
			mvstch(iCopyStitch++, iSourceStitch + 1);
			mvstch(iCopyStitch++, iSourceStitch);
			BeanCount += 2;
		}
		iSourceStitch++;
	}
	mvstch(iCopyStitch++, iSourceStitch);
	if ((StitchBuffer[iSourceStitch - 2].x != StitchBuffer[iSourceStitch].x || StitchBuffer[iSourceStitch - 2].y != StitchBuffer[iSourceStitch].y)) {
		mvstch(iCopyStitch++, iSourceStitch + 1);
		mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < PCSHeader.stitchCount)
		mvstch(iCopyStitch++, iSourceStitch++);
	for (iSourceStitch = MAXITEMS; iSourceStitch < iCopyStitch; iSourceStitch++)
		mvstch(iOutputStitch++, iSourceStitch);
	PCSHeader.stitchCount = iOutputStitch;
}

void dubean() {
	if (PCSHeader.stitchCount) {
		savdo();
		if (StateMap.test(StateFlag::GRPSEL)) {
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
		StateMap.set(StateFlag::RESTCH);
	}
}

void unbean(unsigned start, unsigned finish) {
	unsigned	iSource = start, iCopy = MAXITEMS;

	BeanCount = 0;
	for (iSource = start; iSource <= finish; iSource++) {
		mvstch(iCopy++, iSource);
		if (StitchBuffer[iSource].x == StitchBuffer[iSource + 2].x && StitchBuffer[iSource].y == StitchBuffer[iSource + 2].y) {
			iSource += 2;
			BeanCount += 2;
		}
	}
	if (BeanCount)
		BeanCount -= 2;;
	if (iSource > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
		iSource = PCSHeader.stitchCount - 1;
	while (iSource < PCSHeader.stitchCount)
		mvstch(iCopy++, iSource++);
	mvstchs(start, MAXITEMS, iCopy);
	PCSHeader.stitchCount = start + iCopy;
}

void debean() {
	savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		rngadj();
		unbean(GroupStartStitch, GroupEndStitch);
		if (ClosestPointIndex > GroupStitchIndex)
			ClosestPointIndex -= BeanCount;
		else
			GroupStitchIndex -= BeanCount;
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		if (GroupStitchIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
			GroupStitchIndex = PCSHeader.stitchCount - 1;
		grpAdj();
	}
	else
		unbean(0, PCSHeader.stitchCount - 1);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void mvfrmsb(FRMHED* destination, const FRMHED* source, unsigned count) noexcept {
	memmove(destination, source, count * sizeof(FRMHED));
}

void mvfltsb(fPOINT* destination, const fPOINT* source, unsigned count) noexcept {
	memmove(destination, source, count * sizeof(fPOINT));
}

void clpspac(const fPOINT* insertPoint, unsigned count) noexcept {
	mvfltsb(&ClipPoints[ClipPointIndex + count - 1], &ClipPoints[ClipPointIndex - 1], ClipPointIndex - clpind(insertPoint));
}

void stchadj() {
	unsigned	iStitch = 0, low = 0, high = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		high = StitchBuffer[iStitch].attribute&FRMSK;
		low = high >> FRMSHFT;
		if (low > ClosestFormToCursor) {
			StitchBuffer[iStitch].attribute &= NFRMSK;
			high += 1 << FRMSHFT;
			StitchBuffer[iStitch].attribute |= high;
		}
	}
	refilfn();
	low = ClosestFormToCursor << FRMSHFT;
	for (iStitch = PCSHeader.stitchCount; iStitch != 0; iStitch--) {
		high = StitchBuffer[iStitch - 1].attribute&FRMSK;
		if ((StitchBuffer[iStitch - 1].attribute&FRMSK) == low) {
			break;
		}
	}
	ClosestFormToCursor++;
	refilfn();
	StateMap.reset(StateFlag::FRMPSEL);
}

void spltsat(SATCON currentGuide) {
	// We are adding two additional vertices when splitting the form
	std::vector<fPOINT>	vertexBuffer(VertexCount + 2);
	unsigned	iForm = 0, iGuide = 0, iVertex = 0, iOldVertex = 0, iNewVertex = 0, oldLastVertex = 0;

	mvfrmsb(&FormList[FormIndex], &FormList[FormIndex - 1], FormIndex - ClosestFormToCursor);
	FormIndex++;
	if (ClosestFormToCursor < FormIndex - 2)
		mvfltsb(&FormVertices[FormVertexIndex + 1], &FormVertices[FormVertexIndex - 1], FormVertexIndex - fltind(FormList[ClosestFormToCursor + 2].vertices));
	FormVertexIndex += 2;
	for (iForm = ClosestFormToCursor + 2; iForm < FormIndex; iForm++)
		FormList[iForm].vertices += 2;
	iOldVertex = 0;
	oldLastVertex = currentGuide.start + (VertexCount - currentGuide.finish) + 1;
	iNewVertex = oldLastVertex + 1;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		if (iVertex == currentGuide.start || iVertex == currentGuide.finish) {
			vertexBuffer[iOldVertex].x = CurrentFormVertices[iVertex].x;
			vertexBuffer[iOldVertex++].y = CurrentFormVertices[iVertex].y;
			if (iVertex == currentGuide.start) {
				vertexBuffer[iNewVertex].x = CurrentFormVertices[iVertex].x;
				vertexBuffer[iNewVertex++].y = CurrentFormVertices[iVertex].y;
			}
			else {
				vertexBuffer[oldLastVertex].x = CurrentFormVertices[iVertex].x;
				vertexBuffer[oldLastVertex].y = CurrentFormVertices[iVertex].y;
			}
		}
		else {
			if (iVertex < currentGuide.start) {
				vertexBuffer[iOldVertex].x = CurrentFormVertices[iVertex].x;
				vertexBuffer[iOldVertex++].y = CurrentFormVertices[iVertex].y;
			}
			else {
				if (iVertex < currentGuide.finish) {
					vertexBuffer[iNewVertex].x = CurrentFormVertices[iVertex].x;
					vertexBuffer[iNewVertex++].y = CurrentFormVertices[iVertex].y;
				}
				else {
					vertexBuffer[iOldVertex].x = CurrentFormVertices[iVertex].x;
					vertexBuffer[iOldVertex++].y = CurrentFormVertices[iVertex].y;
				}
			}
		}
	}
	for (iVertex = 0; iVertex < iNewVertex; iVertex++) {
		CurrentFormVertices[iVertex].x = vertexBuffer[iVertex].x;
		CurrentFormVertices[iVertex].y = vertexBuffer[iVertex].y;
	}
	SelectedForm->vertexCount = iOldVertex;
	FormList[ClosestFormToCursor + 1].vertexCount = iNewVertex - iOldVertex;
	FormList[ClosestFormToCursor + 1].vertices = &CurrentFormVertices[iOldVertex];
	frmout(ClosestFormToCursor);
	frmout(ClosestFormToCursor + 1);
	iOldVertex = currentGuide.start + 1 - currentGuide.finish;
	for (iGuide = 0; iGuide < ActivePointIndex; iGuide++)
		SelectedForm->satinOrAngle.guide[iGuide].finish += iOldVertex;
	if (SelectedForm->wordParam)
		SelectedForm->wordParam = currentGuide.start;
	iOldVertex = iGuide + 1;
	while (iGuide < gsl::narrow<unsigned>(SelectedForm->satinGuideCount)) {
		SelectedForm->satinOrAngle.guide[iGuide].start -= (currentGuide.start - 1);
		SelectedForm->satinOrAngle.guide[iGuide].finish -= (currentGuide.start - 1);
		iGuide++;
	}
	if (FormList[ClosestFormToCursor + 1].wordParam)
		FormList[ClosestFormToCursor + 1].wordParam -= (currentGuide.start - 1);
	mvsatk(&SelectedForm->satinOrAngle.guide[iOldVertex - 1], &SelectedForm->satinOrAngle.guide[iOldVertex], SatinGuideIndex - satind(&SelectedForm->satinOrAngle.guide[iOldVertex]));
	SatinGuideIndex--;
	FormList[ClosestFormToCursor + 1].satinOrAngle.guide = &SelectedForm->satinOrAngle.guide[ActivePointIndex];
	FormList[ClosestFormToCursor + 1].satinGuideCount = SelectedForm->satinGuideCount - ActivePointIndex - 1;
	SelectedForm->satinGuideCount = ActivePointIndex;
	for (iForm = ClosestFormToCursor + 2; iForm < FormIndex; iForm++)
		FormList[iForm].satinOrAngle.guide--;
	if (iseclp(ClosestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
}

bool spltlin() {
	unsigned	iForm = 0;

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
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
	return 1;
}

void spltfrm() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
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
					StateMap.set(StateFlag::RESTCH);
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
	unsigned	iStitch = 0, iVertex = 0, vertexCount = 0;

	if (StateMap.test(StateFlag::GRPSEL)) {
		rngadj();
		if ((GroupEndStitch - GroupStartStitch) > 12000) {
			tabmsg(IDS_STMAX);
			return;
		}
		vertexCount = GroupEndStitch - GroupStartStitch + 1;
		SelectedForm = &FormList[FormIndex];
		frmclr(SelectedForm);
		SelectedForm->type = FRMLINE;
		SelectedForm->vertexCount = vertexCount;
		SelectedForm->vertices = adflt(vertexCount);
		iVertex = 0;
		for (iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
			SelectedForm->vertices[iVertex].x = StitchBuffer[iStitch].x;
			SelectedForm->vertices[iVertex++].y = StitchBuffer[iStitch].y;
		}
		frmout(FormIndex);
		FormIndex++;
		if (ClosestPointIndex > GroupStitchIndex) {
			if (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
				ClosestPointIndex++;
		}
		else {
			if (GroupStitchIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1)
				GroupStitchIndex++;
		}
		delstchm();
		StateMap.reset(StateFlag::GRPSEL);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_GRPMSG, IDS_STCH2FRM);
}

bool lencmp(const LENINFO &arg1, const LENINFO &arg2) noexcept {
	return (arg1.length < arg2.length);
}

bool lencmpa(const CLIPSORT *arg1, const CLIPSORT *arg2) noexcept {
	const float local1 = arg1->segmentLength, local2 = arg2->segmentLength;

	return (local1 < local2);
}


void chksid(unsigned vertexIndex) noexcept {
	unsigned	iVertex = 0, limit = 0;

	if (ClipIntersectSide != vertexIndex) {
		if ((vertexIndex - ClipIntersectSide + VertexCount) % VertexCount < (VertexCount >> 1)) {
			iVertex = nxt(ClipIntersectSide);
			limit = nxt(vertexIndex);
			while (iVertex != limit) {
				OSequence[SequenceIndex].x = CurrentFormVertices[iVertex].x;
				OSequence[SequenceIndex++].y = CurrentFormVertices[iVertex].y;
				iVertex = nxt(iVertex);
			}
		}
		else {
			iVertex = ClipIntersectSide;
			while (iVertex != vertexIndex) {
				OSequence[SequenceIndex].x = CurrentFormVertices[iVertex].x;
				OSequence[SequenceIndex++].y = CurrentFormVertices[iVertex].y;
				iVertex = prv(iVertex);
			}
		}
	}
}

void ritseg(std::vector<CLIPNT> &clipStitchPoints, std::vector<CLPSEG> &clipSegments, const unsigned currentSegmentIndex) {
	unsigned	iPoint = 0;
	bool		isPointedEnd = true;

	if (SelectedForm->extendedAttribute&AT_SQR)
		isPointedEnd = false;
	if (StateMap.test(StateFlag::FILDIR)) {
		iPoint = clipSegments[currentSegmentIndex].start;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd)
			iPoint++;
		chksid(clipSegments[currentSegmentIndex].asid);
		while (iPoint <= clipSegments[currentSegmentIndex].finish) {
			OSequence[SequenceIndex].x = clipStitchPoints[iPoint].x;
			OSequence[SequenceIndex++].y = clipStitchPoints[iPoint++].y;
		}
		ClipIntersectSide = clipSegments[currentSegmentIndex].zsid;
	}
	else {
		iPoint = clipSegments[currentSegmentIndex].finish;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd)
			iPoint--;
		chksid(clipSegments[currentSegmentIndex].zsid);
		if (clipSegments[currentSegmentIndex].start) {
			while (iPoint >= clipSegments[currentSegmentIndex].start) {
				OSequence[SequenceIndex].x = clipStitchPoints[iPoint].x;
				OSequence[SequenceIndex++].y = clipStitchPoints[iPoint--].y;
			}
		}
		else {
			while (iPoint < clipSegments[currentSegmentIndex].start) {
				OSequence[SequenceIndex].x = clipStitchPoints[iPoint].x;
				OSequence[SequenceIndex++].y = clipStitchPoints[iPoint--].y;
			}
		}
		ClipIntersectSide = clipSegments[currentSegmentIndex].asid;
	}
	clipSegments[currentSegmentIndex].dun = 1;
}

bool clpnxt(std::vector<CLPSEG> &clipSegments, std::vector<LENINFO> &sortedLengths, unsigned sind) {
	// ToDo - rename local variables

	unsigned	ind = 1;
	const unsigned	indexDoubled = clipSegments.size() << 1;

	StateMap.reset(StateFlag::FILDIR);
	while (ind < clipSegments.size()) {
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			OutputIndex = (sind + ind) % indexDoubled;
			if (!clipSegments[sortedLengths[OutputIndex].index].dun)
				return 0;
			ind++;
		}
		else {
			OutputIndex = (sind + indexDoubled - ind) % indexDoubled;
			if (!clipSegments[sortedLengths[OutputIndex].index].dun)
				return 0;
		}
	}
	return 1;
}

bool nucseg(std::vector<CLPSEG> &clipSegments, std::vector<LENINFO> &sortedLengths, unsigned &currentSegmentIndex) {
	unsigned	ind = 0;

	if (StateMap.test(StateFlag::FILDIR))
		ind = clipSegments[currentSegmentIndex].endIndex;
	else
		ind = clipSegments[currentSegmentIndex].beginIndex;
	if (clpnxt(clipSegments, sortedLengths, ind))
		return false;
	if (sortedLengths[OutputIndex].isEnd)
		StateMap.reset(StateFlag::FILDIR);
	else
		StateMap.set(StateFlag::FILDIR);
	currentSegmentIndex = sortedLengths[OutputIndex].index;
	return true;
}

void mvpclp(std::vector<CLIPSORT *> &arrayOfClipIntersectData, unsigned destination, unsigned source) noexcept {
	if (destination != source) {
		arrayOfClipIntersectData[destination] = arrayOfClipIntersectData[source];
	}
}

float getlen(std::vector<CLIPNT> &clipStitchPoints, std::vector<double> &lengths, unsigned iPoint) noexcept {
	clipStitchPoints[iPoint].vertexIndex %= VertexCount;
	return	lengths[clipStitchPoints[iPoint].vertexIndex] +
		hypot(CurrentFormVertices[clipStitchPoints[iPoint].vertexIndex].x - clipStitchPoints[iPoint].x,
			CurrentFormVertices[clipStitchPoints[iPoint].vertexIndex].y - clipStitchPoints[iPoint].y);
}

constexpr unsigned leftsid() {
	unsigned	iVertex = 0, leftVertex = 0;
	float		minimumX = 1e9;

	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		if (CurrentFormVertices[iVertex].x < minimumX) {
			minimumX = CurrentFormVertices[iVertex].x;
			leftVertex = iVertex;
		}
	}
	return leftVertex;
}

bool clpcmp(const VCLPX &vclpx1, const VCLPX &vclpx2) noexcept {

	return(vclpx1.segment < vclpx2.segment);
}

bool isect(unsigned vertex0, unsigned vertex1, fPOINT* intersection, float* length) noexcept {
	dPOINT	delta = { (LineSegmentEnd.x - LineSegmentStart.x),(LineSegmentEnd.y - LineSegmentStart.y) };
	dPOINT	point = { (LineSegmentStart.x),(LineSegmentStart.y) };
	dPOINT	tempIntersection = {};
	bool	flag = false;
	float	left = 0.0;
	float	right = 0.0;

	if (delta.x && delta.y)
		flag = proj(point, delta.y / delta.x, CurrentFormVertices[vertex0], CurrentFormVertices[vertex1], &tempIntersection);
	else {
		if (delta.y)
			flag = projv(point.x, CurrentFormVertices[vertex0], CurrentFormVertices[vertex1], &tempIntersection);
		else {
			if (delta.x)
				flag = projh(point.y, CurrentFormVertices[vertex0], CurrentFormVertices[vertex1], &tempIntersection);
			else
				if (CurrentFormVertices[vertex0].y == LineSegmentStart.y && CurrentFormVertices[vertex1].y == LineSegmentStart.y) {
					if (CurrentFormVertices[vertex0].x < CurrentFormVertices[vertex1].x) {
						left = CurrentFormVertices[vertex0].x;
						right = CurrentFormVertices[vertex1].x;
					}
					else {
						left = CurrentFormVertices[vertex1].x;
						right = CurrentFormVertices[vertex0].x;
					}
					if (LineSegmentStart.x > left && LineSegmentStart.x < right) {
						intersection->x = LineSegmentStart.x;
						intersection->y = LineSegmentStart.y;
						if (length) {
							*length = 0;
						}
						return true;
					}
					return false;
				}
				else
					return false;
		}
	}
	if (tempIntersection.x < TINY)
		tempIntersection.x = 0;
	if (tempIntersection.y < TINY)
		tempIntersection.y = 0;
	intersection->x = static_cast<float>(tempIntersection.x);
	intersection->y = static_cast<float>(tempIntersection.y);
	if (length) {
		*length = hypot(tempIntersection.x - LineSegmentStart.x, tempIntersection.y - LineSegmentStart.y);
	}
	// ToDo - should length be determined from start or end?
	//	 hypot(tipnt.x-LineSegmentEnd.x,tipnt.y-LineSegmentEnd.y);
	return flag;
}

unsigned insect(std::vector<CLIPSORT> &clipIntersectData, std::vector<VCLPX> &regionCrossingData, std::vector<CLIPSORT *> &arrayOfClipIntersectData, unsigned regionCrossingStart, unsigned regionCrossingEnd) noexcept {
	unsigned	iRegions = 0, iDestination = 0, iIntersection = 0, count = 0;
	unsigned	currentVertex = 0, nextVertex = 0;
	fRECTANGLE	lineSegmentRect = {};
	fPOINT*		intersection = nullptr;

	if (LineSegmentEnd.x > LineSegmentStart.x) {
		lineSegmentRect.left = LineSegmentStart.x;
		lineSegmentRect.right = LineSegmentEnd.x;
	}
	else {
		lineSegmentRect.left = LineSegmentEnd.x;
		lineSegmentRect.right = LineSegmentStart.x;
	}
	if (LineSegmentEnd.y > LineSegmentStart.y) {
		lineSegmentRect.top = LineSegmentEnd.y;
		lineSegmentRect.bottom = LineSegmentStart.y;
	}
	else {
		lineSegmentRect.top = LineSegmentStart.y;
		lineSegmentRect.bottom = LineSegmentEnd.y;
	}
	iIntersection = count = 0;
	arrayOfClipIntersectData.clear();
	for (iRegions = regionCrossingStart; iRegions < regionCrossingEnd; iRegions++) {
		currentVertex = regionCrossingData[iRegions].vertex;
		nextVertex = nxt(currentVertex);
		if (isect(currentVertex, nextVertex, &clipIntersectData[iIntersection].point, &clipIntersectData[iIntersection].sideLength)) {
			intersection = &clipIntersectData[iIntersection].point;
			if (intersection->x >= lineSegmentRect.left &&
				intersection->x <= lineSegmentRect.right &&
				intersection->y >= lineSegmentRect.bottom &&
				intersection->y <= lineSegmentRect.top) {
				clipIntersectData[iIntersection].segmentLength = hypot(clipIntersectData[iIntersection].point.x - LineSegmentStart.x, clipIntersectData[iIntersection].point.y - LineSegmentStart.y);
				clipIntersectData[iIntersection].vertexIndex = currentVertex;
				arrayOfClipIntersectData.push_back(&clipIntersectData[iIntersection]);
				iIntersection++;
				count++;
			}
		}
	}
	if (count > 1) {
		std::sort(arrayOfClipIntersectData.begin(), arrayOfClipIntersectData.end(), lencmpa);
		iDestination = 1;
		for (iIntersection = 0; iIntersection < count - 1; iIntersection++) {
			if (fabs(arrayOfClipIntersectData[iIntersection]->segmentLength - arrayOfClipIntersectData[iIntersection + 1]->segmentLength) > TINY)
				mvpclp(arrayOfClipIntersectData, iDestination++, iIntersection + 1);
		}
		count = iDestination;
	}
	return count;
}

bool isin(std::vector<VCLPX> regionCrossingData, float xCoordinate, float yCoordinate, unsigned regionCrossingStart, unsigned regionCrossingEnd) noexcept {
	// ToDo - rename local variables

	unsigned	iRegion = 0, acnt = 0;
	unsigned	svrt = 0, nvrt = 0;
	dPOINT		ipnt = {};

	if (xCoordinate < BoundingRect.left)
		return 0;
	if (xCoordinate > BoundingRect.right)
		return 0;
	if (yCoordinate < BoundingRect.bottom)
		return 0;
	if (yCoordinate > BoundingRect.top)
		return 0;
	for (iRegion = regionCrossingStart; iRegion < regionCrossingEnd; iRegion++) {
		svrt = regionCrossingData[iRegion].vertex;
		nvrt = nxt(svrt);
		if (projv(xCoordinate, CurrentFormVertices[svrt], CurrentFormVertices[nvrt], &ipnt)) {
			if (ipnt.y > yCoordinate) {
				if (CurrentFormVertices[svrt].x != xCoordinate && CurrentFormVertices[nvrt].x != xCoordinate)
					acnt++;
				else {
					if (CurrentFormVertices[svrt].x < CurrentFormVertices[nvrt].x) {
						if (CurrentFormVertices[nvrt].x != xCoordinate)
							acnt++;
					}
					else {
						if (CurrentFormVertices[svrt].x != xCoordinate)
							acnt++;
					}
				}
			}
		}
	}
	return acnt & 1;
}

unsigned clpnseg(std::vector<CLIPNT> &clipStitchPoints, std::vector<CLPSEG> &clipSegments, std::vector<double> &lengths, unsigned start, unsigned finish) noexcept {
	CLPSEG clipSegment;

	clipSegment.start = start;
	clipSegment.beginLength = getlen(clipStitchPoints, lengths, start);
	clipSegment.asid = clipStitchPoints[start].vertexIndex;
	clipSegment.endLength = getlen(clipStitchPoints, lengths, finish);
	clipSegment.zsid = clipStitchPoints[finish].vertexIndex;
	clipSegment.finish = finish;
	clipSegment.dun = 0;
	clipSegments.push_back(clipSegment);
	return finish + 1;
}

bool vscmp(unsigned index1, unsigned index2) noexcept {
	if (OSequence[index1].x != OSequence[index2].x)
		return 1;

	return OSequence[index1].y != OSequence[index2].y ? 1 : 0;
}

void duflt() {
	unsigned	iVertex = 0;
	float		leftEdge = 1e9;

	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		if (CurrentFormVertices[iVertex].x < leftEdge)
			leftEdge = CurrentFormVertices[iVertex].x;
	}
	if (leftEdge < ClipRectSize.cx) {
		StateMap.set(StateFlag::WASNEG);
		FormOffset = ClipRectSize.cx + fabs(leftEdge) + .05;
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			CurrentFormVertices[iVertex].x += FormOffset;
		SelectedForm->rectangle.left += FormOffset;
		SelectedForm->rectangle.right += FormOffset;
	}
	else
		StateMap.reset(StateFlag::WASNEG);
}

void inspnt(std::vector<CLIPNT> &clipStitchPoints) noexcept {
	CLIPNT	clipStitchPoint{ clipStitchPoints.back().x,
								clipStitchPoints.back().y,
								0,
								0 };
	clipStitchPoints.back().x = midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2].x);
	clipStitchPoints.back().y = midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2].y);
	clipStitchPoints.back().flag = 1;
	clipStitchPoints.push_back(clipStitchPoint);
}

void clpcon(std::vector<RNGCNT> &textureSegments) {
	RECT		clipGrid = {};
	unsigned	iSegment = 0, iStitchPoint = 0, iVertex = 0, iPoint = 0, iSorted = 0, iSequence = 0, vertex = 0;
	unsigned	iRegion = 0, ine = 0, nextVertex = 0, regionSegment = 0, iStitch = 0;
	unsigned	lineOffset = 0, negativeOffset = 0, clipNegative = 0;
	unsigned	start = 0, finish = 0, segmentCount = 0, previousPoint = 0;
	// ToDo - rename variables
	unsigned	inf = 0, ing = 0, cnt = 0;
	int			iVerticalGrid = 0, textureLine = 0;
	fPOINT		pasteLocation = {};
	double		totalLength = 0.0, minx = 0.0;
	float		formNegativeOffset = 0.0;
	unsigned	clipGridOffset = 0;
	double		clipVerticalOffset = 0.0;
	TXPNT*		texture = nullptr;
	unsigned	clplim = 0;					//vertical clipboard search limit
	unsigned	regionCrossingStart = 0;	//start of region crossing data for a particular region
	unsigned	regionCrossingEnd = 0;		//end of region crossing data for a particular region


	duflt();
	ClipWidth = ClipRectSize.cx + SelectedForm->fillSpacing;
	if (StateMap.test(StateFlag::ISUND))
		ClipWidth = SelectedForm->underlaySpacing;
	if (SelectedForm->fillSpacing < 0)
		clipNegative = 1;
	else
		clipNegative = 0;
	if (ClipWidth < CLPMINAUT)
		ClipWidth = CLPMINAUT;
	if (StateMap.test(StateFlag::TXFIL)) {
		if (TextureIndex && SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count <= TextureIndex)
			ClipWidth = SelectedForm->fillSpacing;
		else {
			return;
		}
	}
	std::vector<double> lengths(VertexCount);
	std::vector<double> clipSideLengths(VertexCount); //lengths of form sides for clipboard fill
	std::vector<CLIPSORT> clipIntersectData(VertexCount); //intersect points for clipboard fill
	std::vector<CLIPSORT *> arrayOfClipIntersectData;
	arrayOfClipIntersectData.reserve(VertexCount);
	vertex = leftsid();
	totalLength = 0;
	vertex = nxt(vertex);
	for (iVertex = 0; iVertex <= VertexCount; iVertex++) {
		nextVertex = nxt(vertex);
		lengths[vertex] = totalLength;
		clipSideLengths[vertex] = hypot(CurrentFormVertices[nextVertex].x - CurrentFormVertices[vertex].x, CurrentFormVertices[nextVertex].y - CurrentFormVertices[vertex].y);
		totalLength += clipSideLengths[vertex];
		vertex = nextVertex;
	}
	clipGrid.left = floor(SelectedForm->rectangle.left / ClipWidth);
	clipGrid.right = ceil(SelectedForm->rectangle.right / ClipWidth);
	clipGrid.bottom = floor(SelectedForm->rectangle.bottom / ClipRectSize.cy - 1);
	clipGrid.top = ceil(SelectedForm->rectangle.top / ClipRectSize.cy + 1) + 2;
	negativeOffset = 0;
	if (SelectedForm->wordParam > 1)
		clipGridOffset = SelectedForm->wordParam;
	else
		clipGridOffset = 0;
	if (clipGridOffset) {
		clipGrid.top++;
		if (SelectedForm->fillSpacing < 0) {
			clipGrid.bottom--;
			clipGrid.left -= gsl::narrow<long>(round(ClipRectSize.cx / ClipWidth));
			clipGrid.right += gsl::narrow<long>(round(ClipRectSize.cx / ClipWidth));
		}
	}
	if (clipNegative && !clipGridOffset)
		clipGrid.left -= gsl::narrow<long>(round(ClipRectSize.cx / ClipWidth));
	if (clipGrid.bottom < 0) {
		negativeOffset = 1 - clipGrid.bottom;
		clipGrid.bottom += negativeOffset;
		clipGrid.top += negativeOffset;
		formNegativeOffset = ClipRectSize.cy*negativeOffset;
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			CurrentFormVertices[iVertex].y += formNegativeOffset;
	}
	segmentCount = 0;
	std::vector<VCLPX>	regionCrossingData; //region crossing data for vertical clipboard fills
	regionCrossingData.reserve(MAXFRMLINS);
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		start = floor(CurrentFormVertices[iVertex].x / ClipWidth);
		finish = floor((CurrentFormVertices[nxt(iVertex)].x) / ClipWidth);
		if (start > finish) {
			const unsigned int swap = start;
			start = finish;
			finish = swap;
		}
		if (SelectedForm->fillSpacing < 0)
			finish += ClipRectSize.cx / ClipWidth;
		if (finish > gsl::narrow<unsigned>(clipGrid.right))
			finish = gsl::narrow<unsigned>(clipGrid.right);
		if (clipNegative)
			start -= static_cast<unsigned int>(ClipRectSize.cx / ClipWidth);
		for (iSegment = start; iSegment <= finish; iSegment++) {
			regionCrossingData.push_back({ iSegment, iVertex });
		}
	}
	std::sort(regionCrossingData.begin(), regionCrossingData.end(), clpcmp);
	std::vector<unsigned> iclpx;
	iclpx.reserve(regionCrossingData.size());
	regionSegment = regionCrossingData[0].segment;
	iclpx.push_back(0);
	for (iSegment = 1; iSegment < regionCrossingData.size(); iSegment++) {
		if (regionCrossingData[iSegment].segment != regionSegment) {
			iclpx.push_back(iSegment);
			regionSegment = regionCrossingData[iSegment].segment;
		}
	}
	iclpx.push_back(iSegment);
	BoundingRect.left = BoundingRect.right = CurrentFormVertices[0].x;
	BoundingRect.top = BoundingRect.bottom = CurrentFormVertices[0].y;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		if (CurrentFormVertices[iVertex].x > BoundingRect.right)
			BoundingRect.right = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].x < BoundingRect.left)
			BoundingRect.left = CurrentFormVertices[iVertex].x;
		if (CurrentFormVertices[iVertex].y > BoundingRect.top)
			BoundingRect.top = CurrentFormVertices[iVertex].y;
		if (CurrentFormVertices[iVertex].y < BoundingRect.bottom)
			BoundingRect.bottom = CurrentFormVertices[iVertex].y;
	}
	ActivePointIndex = 0;
	std::vector<CLIPNT> clipStitchPoints;
	// ToDo - find a better number than MAXITEMS
	clipStitchPoints.reserve(MAXITEMS);
	bool breakFlag = false;
	for (iRegion = 0; iRegion < (iclpx.size() - 1); iRegion++) {
		regionCrossingStart = iclpx[iRegion];
		regionCrossingEnd = iclpx[iRegion + 1];
		pasteLocation.x = ClipWidth * (iRegion + clipGrid.left);
		clipVerticalOffset = 0;
		if (StateMap.test(StateFlag::TXFIL)) {
			textureLine = (iRegion + clipGrid.left) % SelectedForm->fillInfo.texture.lines;
			ClipStitchCount = textureSegments[textureLine].stitchCount;
			texture = &TexturePointsBuffer[SelectedForm->fillInfo.texture.index + textureSegments[textureLine].line];
			LineSegmentStart.x = pasteLocation.x;
			if (SelectedForm->txof) {
				lineOffset = (iRegion + clipGrid.left) / SelectedForm->fillInfo.texture.lines;
				clipVerticalOffset = fmod(SelectedForm->txof*lineOffset, SelectedForm->fillInfo.texture.height);
			}
		}
		else {
			if (clipGridOffset) {
				clipVerticalOffset = static_cast<float>(iRegion%clipGridOffset) / (clipGridOffset*ClipRectSize.cy);
			}
			LineSegmentStart.x = pasteLocation.x + ClipBuffer[0].x;
		}
		LineSegmentStart.y = clipGrid.bottom*ClipRectSize.cy;
		if (clipGridOffset) {
			clipVerticalOffset = static_cast<float>(iRegion%clipGridOffset) / (clipGridOffset*ClipRectSize.cy);
		}
		for (iVerticalGrid = clipGrid.bottom; iVerticalGrid < clipGrid.top; iVerticalGrid++) {
			pasteLocation.y = iVerticalGrid * ClipRectSize.cy - clipVerticalOffset;
			LineSegmentEnd.x = pasteLocation.x + ClipBuffer[0].x;
			LineSegmentEnd.y = pasteLocation.y + ClipBuffer[0].y;
			if (!clipStitchPoints.size()) {
				LineSegmentStart.x = LineSegmentEnd.x;
				LineSegmentStart.y = LineSegmentEnd.y;
			}
			for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
				if (StateMap.test(StateFlag::TXFIL)) {
					if (texture != nullptr) {
						LineSegmentEnd.x = pasteLocation.x;
						[[gsl::suppress(26413)]]LineSegmentEnd.y = pasteLocation.y + texture[iStitch].y;
					}
				}
				else {
					LineSegmentEnd.x = pasteLocation.x + ClipBuffer[iStitch].x;
					LineSegmentEnd.y = pasteLocation.y + ClipBuffer[iStitch].y;
				}

				clipStitchPoints.push_back({ LineSegmentStart.x, LineSegmentStart.y, 0, 0 });
				if (isin(regionCrossingData, LineSegmentStart.x, LineSegmentStart.y, regionCrossingStart, regionCrossingEnd)) {
					if ((clipStitchPoints.size() > 1) && clipStitchPoints[clipStitchPoints.size() - 1].flag == 2) {
						inspnt(clipStitchPoints);
					}
					clipStitchPoints.back().flag = 0;
				}
				else {
					if ((clipStitchPoints.size() > 1) && !clipStitchPoints[clipStitchPoints.size() - 1].flag) {
						inspnt(clipStitchPoints);
					}
					clipStitchPoints.back().flag = 2;
				}
				cnt = insect(clipIntersectData, regionCrossingData, arrayOfClipIntersectData, regionCrossingStart, regionCrossingEnd);
				if (cnt) {
					for (ing = 0; ing < cnt; ing++) {
						clipStitchPoints.push_back({ arrayOfClipIntersectData[ing]->point.x,
													arrayOfClipIntersectData[ing]->point.y,
													arrayOfClipIntersectData[ing]->vertexIndex,
													1 });
						if (clipStitchPoints.size() > MAXITEMS << 2) {
							breakFlag = true;
							break;
						}
					}
					if (breakFlag) {
						break;
					}
				}
				LineSegmentStart.x = LineSegmentEnd.x;
				LineSegmentStart.y = LineSegmentEnd.y;
			}
			if (breakFlag) {
				break;
			}
		}
		if (!breakFlag) {
			if (clipStitchPoints.size()) {
				clipStitchPoints[clipStitchPoints.size() - 1].flag = 2;
			};
		}
		else {
			break;
		}
	}
	if (clipStitchPoints.size()) {
		clipStitchPoints[clipStitchPoints.size() - 1].flag = 2;
	};
	if (negativeOffset) {
		formNegativeOffset = negativeOffset * ClipRectSize.cy;
		for (iStitchPoint = 0; iStitchPoint < clipStitchPoints.size(); iStitchPoint++)
			clipStitchPoints[iStitchPoint].y -= formNegativeOffset;
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			CurrentFormVertices[iVertex].y -= formNegativeOffset;
	}
#define CLPVU 0


#if CLPVU==1

	goto clp1skp;

#endif

	StateMap.reset(StateFlag::FILDIR);
	previousPoint = 0;
	std::vector<CLPSEG> clipSegments;
	if (clipStitchPoints.size()) {
		// reserve a reasonable amount but not the full amount potentially required
		clipSegments.reserve(clipStitchPoints.size() / 10);

		for (iPoint = 0; iPoint < clipStitchPoints.size() - 1; iPoint++) {
			switch (clipStitchPoints[iPoint].flag) {
			case 0:		//inside

				StateMap.set(StateFlag::FILDIR);
				break;

			case 1:		//line

				if (StateMap.testAndFlip(StateFlag::FILDIR))
					clpnseg(clipStitchPoints, clipSegments, lengths, previousPoint, iPoint);
				else
					previousPoint = iPoint;
				break;

			case 2:		//outside

				StateMap.reset(StateFlag::FILDIR);
				break;
			}
		}
	}

#if CLPVU==1

	clp1skp:;

#endif

			if (clipSegments.size()) {
				clplim = clipSegments.size() >> 3;
				clplim = clipSegments.size() >> 1;
				if (!clplim)
					clplim = 1;
				if (clplim > 12)
					clplim = 12;
				std::vector<LENINFO> sortedLengths;
				sortedLengths.reserve(clipSegments.size() * 2);
				for (iSegment = 0; iSegment < clipSegments.size(); iSegment++) {
					sortedLengths.push_back({ iSegment, false, clipSegments[iSegment].beginLength });
					sortedLengths.push_back({ iSegment, true, clipSegments[iSegment].endLength });
				}
				std::sort(sortedLengths.begin(), sortedLengths.end(), lencmp);
				for (iSorted = 0; iSorted < sortedLengths.size(); iSorted++) {
					if (sortedLengths[iSorted].isEnd)
						clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
					else
						clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
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
					for (iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish; iStitchPoint++) {
						StitchBuffer[inf].x = ClipStitchPoints[iStitchPoint].x;
						StitchBuffer[inf].y = ClipStitchPoints[iStitchPoint].y;
						StitchBuffer[inf++].attribute = iSegment & 0xf;
					}
				}
				PCSHeader.stitchCount = inf;

#endif

				minx = 1e99;

#if CLPVU==0

				unsigned currentSegmentIndex = 0;
				StateMap.set(StateFlag::FILDIR);
				SequenceIndex = 0;
				ClipIntersectSide = clipSegments[0].asid;
				ritseg(clipStitchPoints, clipSegments, currentSegmentIndex);
				while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
					if (SequenceIndex > MAXITEMS - 3)
						break;
					ritseg(clipStitchPoints, clipSegments, currentSegmentIndex);
				}
				chksid(0);
				if (SequenceIndex > MAXITEMS - 100)
					SequenceIndex = MAXITEMS - 100;
				ine = 0; inf = 0;
				for (iSequence = 0; iSequence < SequenceIndex; iSequence++) {
					if (vscmp(iSequence, ine)) {
						ine++;
						OSequence[ine].x = OSequence[iSequence].x;
						OSequence[ine].y = OSequence[iSequence].y;
					}
					else
						inf++;
				}
				SequenceIndex = ine;
				if (StateMap.test(StateFlag::WASNEG)) {
					for (iSequence = 0; iSequence < SequenceIndex; iSequence++)
						OSequence[iSequence].x -= FormOffset;
					for (iVertex = 0; iVertex < VertexCount; iVertex++)
						CurrentFormVertices[iVertex].x -= FormOffset;
					SelectedForm->rectangle.left -= FormOffset;
					SelectedForm->rectangle.right -= FormOffset;
				}
#endif
			}
}

void vrtsclp() {
	unsigned	iStitch = 0;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->wordParam = IniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->angleOrClipData.clip[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->angleOrClipData.clip[iStitch].y = ClipBuffer[iStitch].y;
	}
	SelectedForm->fillType = VCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void vrtclp() {
	unsigned	iForm = 0;

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
					StateMap.set(StateFlag::NOCLP);
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							vrtsclp();
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fvars(ClosestFormToCursor);
						vrtsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
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

void angout() noexcept {
	fRECTANGLE*	rectangle = nullptr;
	unsigned	iVertex = 1;

	if (AngledForm.vertexCount) {
		rectangle = &AngledForm.rectangle;
		CurrentFormVertices = AngledForm.vertices;
		rectangle->left = rectangle->right = CurrentFormVertices[0].x;
		rectangle->bottom = rectangle->top = CurrentFormVertices[0].y;
		for (iVertex = 1; iVertex < AngledForm.vertexCount; iVertex++) {
			if (CurrentFormVertices[iVertex].x > rectangle->right)
				rectangle->right = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].x < rectangle->left)
				rectangle->left = CurrentFormVertices[iVertex].x;
			if (CurrentFormVertices[iVertex].y < rectangle->bottom)
				rectangle->bottom = CurrentFormVertices[iVertex].y;
			if (CurrentFormVertices[iVertex].y > rectangle->top)
				rectangle->top = CurrentFormVertices[iVertex].y;
		}
	}
}

void horclpfn(std::vector<RNGCNT> &textureSegments) {
	unsigned	iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	RotationCenter.x = static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	RotationAngle = PI / 2;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledForm.vertices[iVertex].x = SelectedForm->vertices[iVertex].x;
		AngledForm.vertices[iVertex].y = SelectedForm->vertices[iVertex].y;
		rotflt(&AngledForm.vertices[iVertex]);
	}
	angout();
	SelectedForm = &AngledForm;
	CurrentFormVertices = AngledForm.vertices;
	clpcon(textureSegments);
	RotationAngle = -PI / 2;
	rotbak();
	fvars(ClosestFormToCursor);
}

void horsclp() {
	unsigned	iStitch = 0;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->wordParam = IniFile.fillPhase;
	makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->angleOrClipData.clip[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->angleOrClipData.clip[iStitch].y = ClipBuffer[iStitch].y;
	}
	SelectedForm->fillType = HCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type = FRMFPOLY;
	CurrentFormVertices = SelectedForm->vertices;
	refilfn();
}

void horclp() {
	unsigned	iForm = 0;

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
					StateMap.set(StateFlag::NOCLP);
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							horsclp();
					}
					StateMap.set(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fvars(ClosestFormToCursor);
						horsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
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

void angclpfn(std::vector<RNGCNT> &textureSegments) {
	unsigned	iVertex = 0;

	AngledForm = FormList[ClosestFormToCursor];
	RotationCenter.x = static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	if (StateMap.test(StateFlag::ISUND)) {
		RotationAngle = PI / 2 - SelectedForm->underlayStitchAngle;
		const fPOINT* vertexList = insid();
		for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex].x = vertexList[iVertex].x;
			AngledFormVertices[iVertex].y = vertexList[iVertex].y;
			rotflt(&AngledFormVertices[iVertex]);
		}
	}
	else {
		if (StateMap.test(StateFlag::TXFIL))
			RotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
		else
			RotationAngle = PI / 2 - SelectedForm->satinOrAngle.angle;
		for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex].x = SelectedForm->vertices[iVertex].x;
			AngledFormVertices[iVertex].y = SelectedForm->vertices[iVertex].y;
			rotflt(&AngledFormVertices[iVertex]);
		}
	}
	angout();
	SelectedForm = &AngledForm;
	CurrentFormVertices = AngledFormVertices;
	clpcon(textureSegments);
	RotationAngle = -RotationAngle;
	rotbak();
	fvars(ClosestFormToCursor);
}

void angsclp() {
	unsigned	iStitch = 0;

	fvars(ClosestFormToCursor);
	delmclp(ClosestFormToCursor);
	deltx();
	SelectedForm->angleOrClipData.clip = numclp();
	SelectedForm->lengthOrCount.clipCount = ClipStitchCount;
	SelectedForm->wordParam = IniFile.fillPhase;
	makpoli();
	SelectedForm->satinOrAngle.angle = IniFile.fillAngle;
	SelectedForm->fillSpacing = IniFile.clipOffset;
	for (iStitch = 0; iStitch < ClipStitchCount; iStitch++) {
		SelectedForm->angleOrClipData.clip[iStitch].x = ClipBuffer[iStitch].x;
		SelectedForm->angleOrClipData.clip[iStitch].y = ClipBuffer[iStitch].y;
	}
	SelectedForm->fillType = ANGCLPF;
	SelectedForm->fillColor = ActiveColor;
	SelectedForm->type = FRMFPOLY;
	refilfn();
}

void angclp() {
	unsigned	iForm = 0;

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
					StateMap.set(StateFlag::NOCLP);
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE)
							angsclp();
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						SelectedForm = &FormList[ClosestFormToCursor];
						angsclp();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
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
	unsigned	iBackward = 0, iForward = 0;

	brdfil(SelectedForm->edgeStitchLen);
	iForward = SequenceIndex;
	for (iBackward = SequenceIndex; iBackward != 0; iBackward--) {
		OSequence[iForward].x = OSequence[iBackward - 1].x;
		OSequence[iForward++].y = OSequence[iBackward - 1].y;
	}
	SequenceIndex = iForward;
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
	unsigned	iForm = 0;

	if (filmsgs(FML_LIN))
		return;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			dubsfil();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			dubsfil();
			coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void col2frm() {
	unsigned	iStitch = 0, iForm = 0, iColor = 0, formColorCode = 0, count = 0, majorityColor = 0;
	unsigned	startColorOffset = 0, endColorOffset = 0, colorChangedCount = 0;
	const unsigned	formColorPermutations = FormIndex << 4; // total number of form and color combinations



	if (FormIndex) {
		std::vector<unsigned> fillColorHistogram(formColorPermutations);
		std::vector<unsigned> borderColorHistogram(formColorPermutations);
		std::vector<unsigned> featherColorHistogram(formColorPermutations);
		std::vector<unsigned> underlayColorHistogram(formColorPermutations);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			formColorCode = StitchBuffer[iStitch].attribute & 0x3fff;
			if (StitchBuffer[iStitch].attribute&(WLKMSK | CWLKMSK | UNDMSK))
				underlayColorHistogram[formColorCode]++;
			else {
				if (StitchBuffer[iStitch].attribute&FTHMSK)
					featherColorHistogram[formColorCode]++;
				else {
					switch (StitchBuffer[iStitch].attribute&TYPMSK) {
					case FRMFIL:

						fillColorHistogram[formColorCode]++;
						break;

					case FRMBFIL:

						borderColorHistogram[formColorCode]++;
						break;
					}
				}
			}
		}
		startColorOffset = 0; endColorOffset = 16;
		for (iForm = 0; iForm < FormIndex; iForm++) {
			if (FormList[iForm].fillType) {
				count = majorityColor = 0;
				for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (fillColorHistogram[iColor] > count) {
						count = fillColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (FormList[iForm].fillColor != majorityColor) {
					colorChangedCount++;
					FormList[iForm].fillColor = majorityColor;
				}
				if (FormList[iForm].fillType == FTHF && FormList[iForm].extendedAttribute&AT_FTHBLND) {
					count = majorityColor = 0;
					for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
						if (featherColorHistogram[iColor] > count) {
							count = fillColorHistogram[iColor];
							majorityColor = iColor;
						}
					}
					majorityColor &= 0xf;
					if (FormList[iForm].fillInfo.feather.color != majorityColor) {
						colorChangedCount++;
						FormList[iForm].fillInfo.feather.color = majorityColor;
					}
				}
			}
			if (FormList[iForm].edgeType) {
				count = majorityColor = 0;
				for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (borderColorHistogram[iColor] > count) {
						count = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (FormList[iForm].borderColor != majorityColor) {
					colorChangedCount++;
					FormList[iForm].borderColor = majorityColor;
				}
			}
			if (FormList[iForm].extendedAttribute&(AT_WALK | AT_CWLK | AT_UND)) {
				count = majorityColor = 0;
				for (iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (underlayColorHistogram[iColor] > count) {
						count = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (FormList[iForm].underlayColor != majorityColor) {
					colorChangedCount++;
					FormList[iForm].underlayColor = majorityColor;
				}
			}
			startColorOffset += 16;
			endColorOffset += 16;
		}
	}
	std::string fmtStr;
	loadString(fmtStr, IDS_NCOLCHG);
	shoMsg(fmt::format(fmtStr, colorChangedCount));
}

bool fxpnt(std::vector<double> &listSINEs, std::vector<double> &listCOSINEs) noexcept {
	double		length = 0.0, delta = 0.0;
	unsigned	iGuess = 0;

	MoveToCoords.x = CurrentFormVertices[NextStart].x;
	MoveToCoords.y = CurrentFormVertices[NextStart].y;
	length = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
	if (length > AdjustedSpace) {
		for (iGuess = 0; iGuess < 10; iGuess++) {
			length = hypot(MoveToCoords.x - SelectedPoint.x, MoveToCoords.y - SelectedPoint.y);
			delta = AdjustedSpace - length;
			MoveToCoords.x += delta * listCOSINEs[CurrentSide];
			MoveToCoords.y += delta * listSINEs[CurrentSide];
			if (fabs(delta) < 0.2)
				break;
		}
		return 1;
	}
	return 0;
}

void fxlit(std::vector<double> &listSINEs, std::vector<double> &listCOSINEs) noexcept {
	double		length = 0.0;
	unsigned	count = 0;
	dPOINT		delta = {};

	if (fxpnt(listSINEs, listCOSINEs)) {
		SelectedPoint.x = MoveToCoords.x;
		SelectedPoint.y = MoveToCoords.y;
		BeanCount++;
		length = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		count = floor(length / AdjustedSpace);
		delta.x = AdjustedSpace * listCOSINEs[CurrentSide];
		delta.y = AdjustedSpace * listSINEs[CurrentSide];
		SelectedPoint.x += delta.x*count;
		SelectedPoint.y += delta.y*count;
		BeanCount += count;
	}
}

void fxlin(std::vector<fPOINT> &chainEndPoints, std::vector<double> &ListSINEs, std::vector<double> &ListCOSINEs) noexcept {
	double		length = 0.0;
	unsigned	count = 0;
	unsigned	iChain = 0;
	dPOINT		delta = {};

	if (fxpnt(ListSINEs, ListCOSINEs)) {
		SelectedPoint.x = MoveToCoords.x;
		SelectedPoint.y = MoveToCoords.y;
		chainEndPoints.push_back(SelectedPoint);
		length = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
		count = floor(length / AdjustedSpace);
		delta.x = AdjustedSpace * ListCOSINEs[CurrentSide];
		delta.y = AdjustedSpace * ListSINEs[CurrentSide];
		for (iChain = 0; iChain < count; iChain++) {
			SelectedPoint.x += delta.x;
			SelectedPoint.y += delta.y;
			chainEndPoints.push_back(SelectedPoint);
		}
	}
}

void fxlen(std::vector<fPOINT> &chainEndPoints, std::vector<double> &listSINEs, std::vector<double> &listCOSINEs) noexcept {
	double		interval = 0.0;
	double		minimumInterval = 0.0;
	double		minimumSpacing = 0.0;
	double		largestSpacing = 0.0;
	double		smallestSpacing = 0.0;
	double		halfSpacing = 0.0;
	unsigned	initialCount = 0;
	unsigned	loopCount = 0;
	unsigned	iVertex = 0;
	double		length = 0.0;

	AdjustedSpace = 0;
	bool flag = true;
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		length = hypot(CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x, CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y);
		if (length > SelectedForm->edgeSpacing) {
			flag = false;
			break;
		}
		else {
			if (length > AdjustedSpace) {
				AdjustedSpace = length;
			}
		}
	}
	if (flag) {
		chainEndPoints.push_back(CurrentFormVertices[0]);
		chainEndPoints.push_back(CurrentFormVertices[1]);
		return;
	}
	AdjustedSpace = minimumSpacing = SelectedForm->edgeSpacing;
	halfSpacing = AdjustedSpace / 2;
	interval = minimumInterval = 1e9;
	loopCount = initialCount = 0;
	smallestSpacing = 0;
	largestSpacing = 1;
	// loop at least 50 times to guarantee convergence
	while (loopCount<50 && (largestSpacing - smallestSpacing)>TINY) {
		BeanCount = 0;
		SelectedPoint = CurrentFormVertices[0];
		for (CurrentSide = 0; CurrentSide < VertexCount - 1; CurrentSide++) {
			NextStart = CurrentSide + 1;
			fxlit(listSINEs, listCOSINEs);
		}
		if (SelectedForm->type != FRMLINE) {
			NextStart = 0;
			fxlit(listSINEs, listCOSINEs);
		}
		else
			NextStart = VertexCount - 1;
		if (!initialCount) {
			initialCount = BeanCount;
			smallestSpacing = AdjustedSpace;
			minimumInterval = interval = static_cast<double>(hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y));
			minimumSpacing = AdjustedSpace;
			interval /= initialCount;
			AdjustedSpace += interval / 2;
			largestSpacing = smallestSpacing + interval;
		}
		else {
			interval = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
			if (interval > halfSpacing)
				interval = SelectedForm->edgeSpacing - interval;
			if (interval < minimumInterval) {
				minimumInterval = interval;
				minimumSpacing = AdjustedSpace;
			}
			if (initialCount == BeanCount)
				smallestSpacing = AdjustedSpace;
			else {
				if (BeanCount > initialCount)
					largestSpacing = AdjustedSpace;
				else
					smallestSpacing = AdjustedSpace;
			}
			AdjustedSpace = smallestSpacing + (largestSpacing - smallestSpacing) / 2;
		}
		loopCount++;
	}
	SelectedPoint = CurrentFormVertices[0];
	OutputIndex = 1;
	AdjustedSpace = minimumSpacing;
	chainEndPoints.push_back(SelectedPoint);
	for (CurrentSide = 0; CurrentSide < VertexCount - 1; CurrentSide++) {
		NextStart = CurrentSide + 1;
		fxlin(chainEndPoints, listSINEs, listCOSINEs);
	}
	if (SelectedForm->type != FRMLINE) {
		NextStart = 0;
		fxlin(chainEndPoints, listSINEs, listCOSINEs);
	}
	interval = hypot(CurrentFormVertices[NextStart].x - SelectedPoint.x, CurrentFormVertices[NextStart].y - SelectedPoint.y);
	if (interval > halfSpacing) {
		chainEndPoints.push_back(CurrentFormVertices[NextStart]);
	}
	else {
		chainEndPoints.back() = CurrentFormVertices[NextStart];
	}
}

void duchfn(std::vector<fPOINT> &chainEndPoints, unsigned start, unsigned finish) noexcept {
	unsigned		iChain = 0;
	fPOINT			chainPoint[5] = {};
	dPOINT			delta = { (chainEndPoints[finish].x - chainEndPoints[start].x),
							  (chainEndPoints[finish].y - chainEndPoints[start].y) };
	dPOINT			lengthDelta = { (delta.x*SelectedForm->edgeStitchLen),
									(delta.y*SelectedForm->edgeStitchLen) };
	const double	angle = atan2(delta.y, delta.x) + PI / 2;
	dPOINT			offset = { (cos(angle)*SelectedForm->borderSize),
								(sin(angle)*SelectedForm->borderSize) };
	const float		middleXcoord = chainEndPoints[start].x + lengthDelta.x;
	const float		middleYcoord = chainEndPoints[start].y + lengthDelta.y;

	chainPoint[0] = chainEndPoints[start];
	chainPoint[4] = chainEndPoints[finish];
	chainPoint[1].x = middleXcoord + offset.x;
	chainPoint[1].y = middleYcoord + offset.y;
	chainPoint[3].x = middleXcoord - offset.x;
	chainPoint[3].y = middleYcoord - offset.y;
	//ToDo - This may not be correct
	if (finish < chainEndPoints.size() - 1) {
		delta.x = chainEndPoints[finish + 1].x - chainEndPoints[finish].x;
		delta.y = chainEndPoints[finish + 1].y - chainEndPoints[finish].y;
	}
	else {
		delta.x = chainEndPoints[finish].x - chainEndPoints[finish - 1].x;
		delta.y = chainEndPoints[finish].y - chainEndPoints[finish - 1].y;
	}
	chainPoint[2].x = chainEndPoints[finish].x + delta.x / 4;
	chainPoint[2].y = chainEndPoints[finish].y + delta.y / 4;
	unsigned chainCount = 10;
	if (StateMap.test(StateFlag::LINCHN))
		chainCount--;

	for (iChain = 0; iChain < chainCount; iChain++) {
		OSequence[SequenceIndex] = chainPoint[ChainSequence[iChain]];
		SequenceIndex++;
	}
}

void duch(std::vector<fPOINT> &chainEndPoints) {
	unsigned	iPoint = 0, backupAt = 0;

	Div4 = AdjustedSpace / 4;
	SequenceIndex = 0;
	if (chainEndPoints.size()) {
		for (iPoint = 0; iPoint < chainEndPoints.size() - 2; iPoint++) {
			duchfn(chainEndPoints, iPoint, iPoint + 1);
		}
		if (SelectedForm->type == FRMLINE) {
			duchfn(chainEndPoints, iPoint, iPoint + 1);
			backupAt = 8;
			if (StateMap.test(StateFlag::LINCHN))
				backupAt--;
			if ((SequenceIndex >= backupAt)) {
				OSequence[SequenceIndex - backupAt] = chainEndPoints[iPoint + 1];
			}
			OSequence[SequenceIndex++] = chainEndPoints[iPoint + 1];
		}
		else {
			duchfn(chainEndPoints, iPoint, 0);
			OSequence[SequenceIndex++] = chainEndPoints[chainEndPoints.size() - 1];
		}
	}
	else
		tabmsg(IDS_CHANSMAL);
}

void dufxlen(std::vector<fPOINT> &chainEndPoints) {
	unsigned	iVertex = 0;

	duangs();
	std::vector<double> listSINEs;
	listSINEs.reserve(VertexCount + 1);
	std::vector<double> listCOSINEs;
	listCOSINEs.reserve(VertexCount);
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		listSINEs.push_back(sin(FormAngles[iVertex]));
		listCOSINEs.push_back(cos(FormAngles[iVertex]));
	}
	listSINEs.push_back(sin((FormAngles[0] > FormAngles[iVertex]) ? (FormAngles[0] - FormAngles[iVertex]) : (FormAngles[iVertex] - FormAngles[0])));
	fxlen(chainEndPoints, listSINEs, listCOSINEs);
}

void chnfn() {
	std::vector<fPOINT> chainEndPoints;
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	fvars(ClosestFormToCursor);
	deleclp(ClosestFormToCursor);
	dufxlen(chainEndPoints);
	dulast(chainEndPoints);
	SequenceIndex = 0;
	duch(chainEndPoints);
}

void chan() {
	SelectedForm->borderColor = ActiveColor;
	SelectedForm->edgeSpacing = IniFile.chainSpace;
	SelectedForm->borderSize = BorderWidth;
	SelectedForm->edgeStitchLen = IniFile.chainRatio;
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
	if (StateMap.test(StateFlag::LINCHN))
		SelectedForm->edgeType = EDGELCHAIN;
	else
		SelectedForm->edgeType = EDGEOCHAIN;
	refilfn();
}

void chain() {
	unsigned	iForm = 0;

	if (filmsgs(FML_CHAIN))
		return;
	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			chan();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			chan();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

bool cisin(float xCoordinate, float yCoordinate) noexcept {
	unsigned	iVertex = 0, count = 0;
	unsigned	nextVertex = 0;
	dPOINT		intersection = {};
	const fRECTANGLE*	rectangle = &SelectedForm->rectangle;

	if (xCoordinate >= rectangle->right)
		return 0;
	if (xCoordinate <= rectangle->left)
		return 0;
	if (yCoordinate >= rectangle->top)
		return 0;
	if (yCoordinate <= rectangle->bottom)
		return 0;
	count = 0;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		nextVertex = nxt(iVertex);
		if (projv(xCoordinate, CurrentFormVertices[iVertex], CurrentFormVertices[nextVertex], &intersection)) {
			if (intersection.y >= yCoordinate) {
				if (CurrentFormVertices[iVertex].x != xCoordinate && CurrentFormVertices[nextVertex].x != xCoordinate)
					count++;
				else {
					if (CurrentFormVertices[iVertex].x < CurrentFormVertices[nextVertex].x) {
						if (CurrentFormVertices[nextVertex].x != xCoordinate)
							count++;
					}
					else {
						if (CurrentFormVertices[iVertex].x != xCoordinate)
							count++;
					}
				}
			}
		}
	}
	return count & 1;
}

void crop() {
	unsigned	iSource = 0, iDestination = 0;

	frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		savdo();
		fvars(ClosestFormToCursor);
		iDestination = 0;
		for (iSource = 0; iSource < PCSHeader.stitchCount; iSource++) {
			if (cisin(StitchBuffer[iSource].x, StitchBuffer[iSource].y)) {
				StitchBuffer[iDestination].x = StitchBuffer[iSource].x;
				StitchBuffer[iDestination].y = StitchBuffer[iSource].y;
				StitchBuffer[iDestination++].attribute = StitchBuffer[iSource].attribute;
			}
		}
		PCSHeader.stitchCount = iDestination;
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_FRM1MSG, IDS_CROP);
}

void xclpfn(std::vector<fPOINT> &tempClipPoints, std::vector<fPOINT> &chainEndPoints, unsigned start, unsigned finish) {
	dPOINT			delta = { (chainEndPoints[finish].x - chainEndPoints[start].x),
								(chainEndPoints[finish].y - chainEndPoints[start].y) };
	unsigned		iPoint = 0;
	const double	length = hypot(delta.x, delta.y);
	const double	ratio = length / ClipRectSize.cx;
	std::vector<fPOINT> points(ClipStitchCount);

	RotationAngle = atan2(delta.y, delta.x);
	for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
		points[iPoint].x = tempClipPoints[iPoint].x*ratio;
		points[iPoint].y = tempClipPoints[iPoint].y;
		rotflt(&points[iPoint]);
		OSequence[SequenceIndex].x = chainEndPoints[start].x + points[iPoint].x;
		OSequence[SequenceIndex++].y = chainEndPoints[start].y + points[iPoint].y;
	}
}

void duxclp() {
	std::vector<fPOINT> chainEndPoints;
	// reserve some memory and rely on push_back behaviour and geometric memory re-allocation for efficiency
	chainEndPoints.reserve(50);
	duangs();
	dufxlen(chainEndPoints);
	std::vector<fPOINT> tempClipPoints;
	tempClipPoints.reserve(ClipStitchCount);
	clpxadj(tempClipPoints, chainEndPoints);
	SequenceIndex = 0;
	RotationCenter.x = RotationCenter.y = 0;
	for (unsigned iPoint = 1; iPoint < chainEndPoints.size(); iPoint++) {
		xclpfn(tempClipPoints, chainEndPoints, iPoint - 1, iPoint);
	}
	if (SelectedForm->type != FRMLINE) {
		OSequence[SequenceIndex++] = chainEndPoints[0];
	}
}

void dulast(std::vector<fPOINT> &chainEndPoints) noexcept {
	unsigned	iPoint = 0, iDestination = 0, minimumIndex = 0;
	double		minimumLength = 0.0;
	double		length = 0.0;

	std::vector<fPOINT> tempClipPoints;
	tempClipPoints.reserve(chainEndPoints.size());
	if (lastch()) {
		minimumLength = 1e99;
		minimumIndex = 0;
		for (iPoint = 0; iPoint < chainEndPoints.size() - 1; iPoint++) {
			length = hypot(LastPoint.x - chainEndPoints[iPoint].x, LastPoint.y - chainEndPoints[iPoint].y);
			if (length < minimumLength) {
				minimumLength = length;
				minimumIndex = iPoint;
			}
		}
		if (minimumIndex) {
			iDestination = 0;
			for (iPoint = minimumIndex; iPoint < chainEndPoints.size() - 2; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			for (iPoint = 0; iPoint <= minimumIndex; iPoint++) {
				tempClipPoints.push_back(chainEndPoints[iPoint]);
			}
			chainEndPoints = tempClipPoints;
		}
	}
}

void clpxadj(std::vector<fPOINT> &tempClipPoints, std::vector<fPOINT> &chainEndPoints) noexcept {
	unsigned	iPoint = 0;
	float		pivot = 0.0;

	dulast(chainEndPoints);
	if (SelectedForm->type == FRMLINE) {
		pivot = ClipRectSize.cy / 2;
		for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.push_back({ ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y + pivot) });
		}
	}
	else {
		for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
			tempClipPoints.push_back({ ClipBuffer[iPoint].x, (-ClipBuffer[iPoint].y) });
		}
	}
}

void fsclpx() {
	unsigned	iPoint = 0;

	deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGECLIPX;
	SelectedForm->clipEntries = ClipStitchCount;
	SelectedForm->borderClipData = nueclp(ClosestFormToCursor, ClipStitchCount);
	SelectedForm->borderSize = ClipRectSize.cy;
	SelectedForm->edgeSpacing = ClipRectSize.cx;
	SelectedForm->borderColor = ActiveColor;
	bsizpar();
	for (iPoint = 0; iPoint < ClipStitchCount; iPoint++) {
		SelectedForm->borderClipData[iPoint].x = ClipBuffer[iPoint].x;
		SelectedForm->borderClipData[iPoint].y = ClipBuffer[iPoint].y;
	}
	RotationCenter.x = RotationCenter.y = 0;
	duxclp();
	refilfn();
}

void filclpx() {
	unsigned	iForm = 0;

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
					for (iForm = 0; iForm < SelectedFormCount; iForm++) {
						ClosestFormToCursor = SelectedFormList[iForm];
						fvars(ClosestFormToCursor);
						fsclpx();
					}
					StateMap.set(StateFlag::INIT);
					coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fsclpx();
						StateMap.set(StateFlag::INIT);
						coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else
				tabmsg(IDS_CLP);
		}
		else {
			std::string str("no clipboard data");
			shoMsg(str);
			CloseClipboard();
		}
	}
}

inline void wavinit (HWND hwndlg) noexcept {
	char	buffer[HBUFSIZ] = { 0 };

	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.wavePoints);
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buffer);
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.waveStart);
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buffer);
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.waveEnd);
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buffer);
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.waveLobes);
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buffer);
}

bool CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) noexcept {
	UNREFERENCED_PARAMETER(lparam);

	char	buffer[HBUFSIZ] = { 0 };

	switch (umsg) {
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		wavinit (hwndlg);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buffer, HBUFSIZ);
			IniFile.wavePoints = atoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buffer, HBUFSIZ);
			IniFile.waveStart = atoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buffer, HBUFSIZ);
			IniFile.waveEnd = atoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buffer, HBUFSIZ);
			IniFile.waveLobes = atoi(buffer);
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
			wavinit (hwndlg);
			break;
		}
	}
	return false;
}

void wavfrm() {
	unsigned	iVertex = 0, iPoint = 0, vertexCount = 0, iNextVertex = 0, iLobe = 0;
	unsigned	end = 0;
	unsigned	count = 0;
	fPOINT		currentPosition = {};
	double		horizontalRatio = 0.0;
	double		verticalRatio = 0.0;
	fPOINT		selectedSize = {};

	unmsg();
	if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, (DLGPROC)wavprc)) {
		iVertex = FormVertexIndex;
		end = IniFile.waveEnd + 1;
		durpoli(IniFile.wavePoints);
		mdufrm();
		FormVertexIndex = iVertex;
		std::vector<fPOINT> points(IniFile.wavePoints);
		iPoint = 0;
		iVertex = IniFile.waveStart;
		while (iVertex != IniFile.waveEnd && iPoint < IniFile.wavePoints) {
			iNextVertex = (iVertex + 1) % IniFile.wavePoints;
			points[iPoint].x = -CurrentFormVertices[iNextVertex].x + CurrentFormVertices[iVertex].x;
			points[iPoint].y = -CurrentFormVertices[iNextVertex].y + CurrentFormVertices[iVertex].y;
			iPoint++;
			iVertex = iNextVertex;
		}
		count = iPoint;
		iVertex = 0;
		currentPosition.x = currentPosition.y = 0;
		for (iLobe = 0; iLobe < IniFile.waveLobes; iLobe++) {
			if (iLobe & 1) {
				for (iPoint = 0; iPoint < count; iPoint++) {
					CurrentFormVertices[iVertex].x = currentPosition.x;
					CurrentFormVertices[iVertex].y = currentPosition.y;
					iVertex++;
					currentPosition.x += points[iPoint].x;
					currentPosition.y += points[iPoint].y;
				}
			}
			else {
				for (iPoint = count; iPoint != 0; iPoint--) {
					CurrentFormVertices[iVertex].x = currentPosition.x;
					CurrentFormVertices[iVertex].y = currentPosition.y;
					iVertex++;
					currentPosition.x += points[iPoint - 1].x;
					currentPosition.y += points[iPoint - 1].y;
				}
			}
		}
		CurrentFormVertices[iVertex].x = currentPosition.x;
		CurrentFormVertices[iVertex].y = currentPosition.y;
		vertexCount = iVertex + 1;
		RotationAngle = -atan2(CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y, CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x);
		for (iVertex = 0; iVertex < vertexCount; iVertex++)
			rotflt(&CurrentFormVertices[iVertex]);
		SelectedForm->type = FRMLINE;
		SelectedForm->vertexCount = vertexCount;
		FormVertexIndex += vertexCount;
		frmout(FormIndex);
		StateMap.reset(StateFlag::FORMSEL);
		selectedSize.x = SelectedForm->rectangle.right - SelectedForm->rectangle.left;
		selectedSize.y = SelectedForm->rectangle.top - SelectedForm->rectangle.bottom;
		horizontalRatio = UnzoomedRect.x / 4 / selectedSize.x;
		if (horizontalRatio > 1)
			horizontalRatio = 1;
		verticalRatio = UnzoomedRect.y / 4 / selectedSize.y;
		if (verticalRatio < horizontalRatio)
			horizontalRatio = verticalRatio;
		if (horizontalRatio < 1) {
			for (iVertex = 0; iVertex < vertexCount; iVertex++) {
				CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - CurrentFormVertices[0].x)*horizontalRatio + CurrentFormVertices[0].x;
				CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - CurrentFormVertices[0].y)*horizontalRatio + CurrentFormVertices[0].y;
			}
		}
		frmout(FormIndex);
		for (iVertex = 0; iVertex < vertexCount; iVertex++) {
			CurrentFormVertices[iVertex].x -= SelectedForm->rectangle.left;
			CurrentFormVertices[iVertex].y -= SelectedForm->rectangle.bottom;
		}
		FormMoveDelta.x = FormMoveDelta.y = 0;
		NewFormVertexCount = vertexCount + 1;
		setmfrm();
		mdufrm();
	}
}

void srtfrm() {
	unsigned	iStitch = 0, iForm = 0, iHighStitch = 0, totalStitches = 0, formStitchCount = 0;
	unsigned	histogram[MAXFORMS] = { 0 };

	if (PCSHeader.stitchCount) {
		savdo();
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
			histogram[(StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT]++;
		totalStitches = 0;
		for (iForm = 0; iForm < MAXFORMS; iForm++) {
			formStitchCount = histogram[iForm];
			histogram[iForm] = totalStitches;
			totalStitches += formStitchCount;
		}
		std::vector<fPOINTATTR> highStitchBuffer(PCSHeader.stitchCount);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			iForm = (StitchBuffer[iStitch].attribute&FRMSK) >> FRMSHFT;
			iHighStitch = histogram[iForm]++;
			highStitchBuffer[iHighStitch].x = StitchBuffer[iStitch].x;
			highStitchBuffer[iHighStitch].y = StitchBuffer[iStitch].y;
			highStitchBuffer[iHighStitch].attribute = StitchBuffer[iStitch].attribute;
		}
		MoveMemory(StitchBuffer, &highStitchBuffer[0], sizeof(fPOINTATTR)*PCSHeader.stitchCount);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

