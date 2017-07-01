#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)

void		chktxnum();
void		deltx();
unsigned	dutyp(unsigned attribute);
void		redtbak();
void		repar();
void		rotbak();
void		tst();
void		txbak();
void		txdelal();
void		txof();

extern	unsigned		ActiveColor;
extern	unsigned		ActiveLayer;
extern	unsigned		ActivePointIndex;
extern	fPOINT			AngledFormVertices[MAXFRMLINS];
extern	FRMHED			AngledForm;
extern	TCHAR			AuxName[_MAX_PATH];
extern	HBRUSH			BackgroundBrush;
extern	BSEQPNT			BSequence[BSEQLEN];
extern	unsigned		ButtonHeight;
extern	unsigned		ButtonWidthX3;
extern	HWND			ButtonWin[9];
extern	FORMCLIP*		ClipFormHeader;
extern	HGLOBAL			ClipMemory;
extern	fPOINT			ClipPoints[MAXITEMS];
extern	fRECTANGLE		ClipRect;
extern	FLSIZ			ClipRectSize;
extern	unsigned		ClipStitchCount;
extern	fPOINTATTR		ClipBuffer[MAXFRMLINS];
extern	unsigned		ClosestFormToCursor;
extern	unsigned		ClosestPointIndex;
extern	unsigned		ClosestVertexToCursor;
extern	unsigned		ClipPointIndex;
extern	fPOINT*			CurrentFormVertices;
extern	unsigned		FormVertexIndex;
extern	fPOINT			FormMoveDelta;
extern	unsigned		FormIndex;
extern	POINT			FormLines[MAXFRMLINS];
extern	FRMHED			FormList[MAXFORMS];
extern	fPOINT			FormVertices[MAXITEMS];
extern	HPEN			GridPen;
extern	HWND			HorizontalScrollBar;
extern	TCHAR			HelpBuffer[HBUFSIZ];
extern	INIFILE			IniFile;
extern	fPOINT*			InsidePoints;
extern	HWND			MainStitchWin;
extern	unsigned		MarkedStitchMap[MAXITEMS];
extern	MSG				Msg;
extern	TCHAR			MsgBuffer[MSGSIZ];
extern	unsigned		NewFormVertexCount;
extern	unsigned		OutputIndex;
extern	fPOINT*			OutsidePoints;
extern	fPOINT			OSequence[OSEQLEN];
extern	PCSHEADER		PCSHeader;
extern	unsigned		PseudoRandomValue;
extern	double			RotationAngle;
extern	dPOINT			RotationCenter;
extern	FRMHED*			SelectedForm;
extern	unsigned		SatinConnectIndex;
extern	SATCON			SatinConnects[MAXSAC];
extern	TCHAR*			StringData;
extern	unsigned		SelectedFormCount;
extern	unsigned		SelectedFormCount;
extern	unsigned short	SelectedFormList[MAXFORMS];
extern	fPOINT			SelectedPoint;
extern	unsigned		SequenceIndex;
extern	TCHAR*			StringTable[STR_LEN];
extern	fPOINTATTR		StitchBuffer[MAXITEMS * 2];
extern	double			StitchSpacing;
extern	RECT			StitchWindowClientRect;
extern	HDC				StitchWindowDC;
extern	HDC				StitchWindowMemDC;
extern	POINT			StitchWindowOrigin;
extern	fPOINTATTR*		TempStitchBuffer;
extern	unsigned		ThrEdClip;
extern	TCHAR*			ThrEdClipFormat;
extern	HINSTANCE		ThrEdInstance;
extern	HWND			ThrEdWindow;
extern	TCHAR			ThrName[_MAX_PATH];
extern	unsigned		AppliqueColor;
extern	POINT			UnzoomedRect;
extern	COLORREF		UserColor[16];
extern	double			UserStitchLength;
extern	unsigned		VertexCount;
extern	HWND			VerticalScrollBar;
extern	TCHAR			WorkingFileName[_MAX_PATH];
extern	POINT			ZoomBoxLine[5];
extern	fPOINT			ZoomBoxOrigin;
extern	dRECTANGLE		ZoomRect;

extern	void		adbad(unsigned code, unsigned count);
extern	fPOINT*		adflt(unsigned count);
extern	SATCON*		adsatk(unsigned	count);
extern	void		angclpfn();
extern	void		bakseq();
extern	void		butxt(unsigned iButton, TCHAR* buttonText);
extern	void		centir();
extern	void		chkhup();
extern	unsigned	chkMap(unsigned bit);
extern	BOOL		chkmax(unsigned arg0, unsigned arg1);
extern	void		chkmen();
extern	BOOL		chkr(unsigned bit);
extern	void		chkseq(BOOL border);
extern	unsigned	chku(unsigned bit);
extern	BOOL		cisin(float xCoordinate, float yCoordinate);
extern	unsigned	closflt(float xCoordinate, float yCoordinate);
extern	void		clpfil();
extern	void		clRmap(unsigned mapSize);
extern	void		coltab();
extern	void		datmsg(unsigned code);
extern	void		delclps(unsigned iForm);
extern	void		delinf();
extern	void		delmclp(unsigned iForm);
extern	void		delsac(unsigned formIndex);
extern	void		dumrk(double xCoord, double yCoord);
extern	void		filang();
extern	BOOL		filmsgs(unsigned code);
extern	void		filsat();
extern	void		filsfn();
extern	void		filvrt();
extern	unsigned	find1st();
extern	void		fnhor();
extern	void		fnvrt();
extern	void		frmclr(FRMHED* destination);
extern	void		frmcpy(FRMHED* destination, FRMHED* source);
extern	void		frmout(unsigned formIndex);
extern	void		frmrct(fRECTANGLE* rectangle);
extern	void		fshor();
extern	void		fvars(unsigned iForm);
extern	unsigned	getlast();
extern	void		hsizmsg();
extern	BOOL		isclp(unsigned iForm);
extern	BOOL		iseclp(unsigned iForm);
extern	BOOL		isin(float xCoordinate, float yCoordinate);
extern	void		lcon();
extern	void		makspac(unsigned start, unsigned count);
extern	void		mdufrm();
extern	float		midl(float high, float low);
extern	void		moveStitch(fPOINTATTR* destination, fPOINTATTR* source);
extern	void		movStch();
extern	void		movStch();
extern	void		msgflt(unsigned messageId, float value);
extern	void		mvstchs(unsigned destination, unsigned source, unsigned count);
extern	void		numWnd();
extern	unsigned	nxt(unsigned iVertex);
extern	unsigned	prv(unsigned iVertex);
extern	unsigned	psg();
extern	unsigned	px2stch();
extern	void		redraw(HWND window);
extern	void		refil();
extern	void		refilal();
extern	void		refilfn();
extern	void		ritfil();
extern	void		rotflt(fPOINT* point);
extern	unsigned	rstMap(unsigned bit);
extern	unsigned	rstu(unsigned bit);
extern	void		rtrclpfn();
extern	void		satfil();
extern	void		satout(double satinWidth);
extern	void		savdo();
extern	void		save();
extern	void		setknots();
extern	unsigned	setMap(unsigned bit);
extern	void		setmfrm();
extern	void		setr(unsigned bit);
extern	unsigned	setu(unsigned bit);
extern	void		shoMsg(TCHAR* string);
extern	void		shoseln(unsigned code0, unsigned code1);
extern	void		stchrct(fRECTANGLE* rectangle);
extern	void		tabmsg(unsigned code);
extern	unsigned	toglMap(unsigned bit);
extern	unsigned	toglu(unsigned bit);
extern	void		unmsg();
extern	void		unthum();
extern	void		wrnmen();
extern	void		zumhom();

unsigned short DaisyTypeStrings[] =
{
	IDS_DAZCRV,
	IDS_DAZSAW,
	IDS_DAZRMP,
	IDS_DAZRAG,
	IDS_DAZCOG,
	IDS_DAZHART,
};

RNGCNT*		TextureSegments;			//texture fill groups of points
dPOINT		DesignSizeRatio;			//design size ratio
fRECTANGLE	DesignSizeRect;				//design size rectangle
float		DesignAspectRatio;			//design aspect ratio
HWND		DesignSizeDialog;			//change design size dialog window
fPOINT		DesignSize;					//design size
TXPNT		TexturePointsBuffer[MAXITEMS];	//buffer for textured fill points
int			TextureIndex;				//next textured fill point index
unsigned	TextureWindowId;			//id of the window being updated
TCHAR		TextureInputBuffer[16];		//texture fill number buffer
int			TextureInputIndex;			//text number pointer
HWND		SideWindowButton;			//button side window
RECT		TexturePixelRect;			//screen selected texture points rectangle
TXTRCT		TextureRect;				//selected texture points rectangle
POINT		SelectTexturePointsOrigin;	//original location of selected texture points
unsigned	SelectedTexturePointsCount;	//number of selected texture points
unsigned*	SelectedTexturePointsList;	//list of selected points
POINT		TextureCursorLocation;		//texture editor move cursor location
HPEN		TextureCrossPen;			//texture editor cross pen
TXTSCR		TextureScreen;				//texture editor layout parameters
TXPNT		TempTexturePoints[16384];	//temporary storage for textured fill data
unsigned	ColorOrder[16];				//color order adjusted for applique
INTINF		InterleaveData;				//interleave data
fPOINT		InterleaveSequence[MAXITEMS]; //storage for interleave points
unsigned	InterleaveSequenceIndex;	//index into the interleave sequence
FSTRTS		FillStartsData;				//fill start data for refill
INSREC		InterleaveSequenceIndices[10];	//indices into interleave points
unsigned	InterleaveSequenceIndex2;	//index into interleave indices
unsigned	FillStartsMap;				//fill starts bitmap
unsigned*	FormFillCounter;			//form fill type counters for sort
fPOINT*		UnderlayVertices;			//underlay offset points
unsigned	FeatherFillType;			//type of feather fill
float		FeatherRatio;				//feather ratio
float		FeatherMinStitch;			//smallest stitch allowed
float		FeatherRatioLocal;			//local feather ratio
float		FormFeatherRatio;			//feather ratio from form
unsigned	ExtendedAttribute;			//extended form attribute
unsigned	FeatherUpCount;				//feather up count
unsigned	FeatherDownCount;			//feather down count
unsigned	FeatherTotalCount;			//up count plus down count
unsigned	FeatherPhaseIndex;
unsigned	FeatherPhase;
fPOINT		FeatherSequence[MAXITEMS];
unsigned	FeatherCountUp;
unsigned	FeatherCountDown;

float		FeatherGlobalStep;
float		FeatherGlobalPosition;
float		FeatherGlobalPhase;
float		FeatherGlobalUp;
float		FeatherGlobalDown;
float		FeatherGlobalRatio;

OREC**		PRecs;
OREC**		PFRecs;

enum {
	TYPE_APPLIQUE = 1,	// applique
	TYPE_CWALK,			// center walk
	TYPE_EWALK,			// edge walk
	TYPE_UNDERLAY,		// underlay
	TYPE_FTHR,			// feather
	TYPE_FILL,			// fill
	TYPE_BORDER			// border
};

char StitchTypes[] =
{
	0,				// 0 no type
	TYPE_APPLIQUE,	// 1 applique
	TYPE_CWALK,		// 2 center walk
	TYPE_EWALK,		// 3 edge walk	
	TYPE_UNDERLAY,	// 4 underlay
	0,				// 5 knot
	TYPE_FTHR,		// 6 feather
	0,				// 7 layer
	0,				// 8 layer
	0,				// 9 layer
	0,				//10 reserved
	TYPE_FILL,		//11 fill
	TYPE_BORDER,	//12 border
};

TXHST	TextureHistory[16];		//texture editor history headers
int		TextureHistoryIndex;	//pointer to the next texture history buffer

void setfchk() {
	if (IniFile.dataCheck)
		setMap(FCHK);
}

#ifdef _DEBUG

void prbug() {
	OutputDebugString(MsgBuffer);
}
#endif

BOOL istx(unsigned iForm) {
	FRMHED* pfrm;

	pfrm = &FormList[iForm];
	if (pfrm->fillType == TXVRTF)
		return 1;
	if (pfrm->fillType == TXHORF)
		return 1;
	if (pfrm->fillType == TXANGF)
		return 1;
	return 0;
}

TXPNT* adtx(int count) {

	unsigned iPoint = TextureIndex;

	TextureIndex += count;
	return &TexturePointsBuffer[iPoint];
}

void txspac(int start, unsigned count) {
	unsigned	iForm;

	MoveMemory(&TexturePointsBuffer[start + count], &TexturePointsBuffer[start], (TextureIndex - start) * sizeof(TXPNT));
	for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (istx(iForm))
			FormList[iForm].fillInfo.texture.index += count;
	}
	TextureIndex += count;
}

void rstxt() {
	SelectedTexturePointsCount = 0;
	rstMap(TXTMOV);
	rstMap(BZUM);
	rstMap(BZUMIN);
	setMap(RESTCH);
	rstMap(POLIMOV);
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	setMap(RESTCH);
}

void txrbak() {
	TextureHistoryIndex--;
	TextureHistoryIndex &= 0xf;
}

void txrfor() {
	TextureHistoryIndex++;
	TextureHistoryIndex &= 0xf;
}

BOOL chktxh(TXHST* historyItem) {
	unsigned iPoint;

	if (historyItem->count != TextureScreen.index)
		return 1;
	if (historyItem->height != TextureScreen.areaHeight)
		return 1;
	if (historyItem->spacing != TextureScreen.spacing)
		return 1;
	if (historyItem->width != TextureScreen.width)
		return 1;
	for (iPoint = 0; iPoint < TextureScreen.index; iPoint++) {
		if (TempTexturePoints[iPoint].line != historyItem->texturePoint[iPoint].line)
			return 1;
		if (TempTexturePoints[iPoint].y != historyItem->texturePoint[iPoint].y)
			return 1;
	}
	return 0;
}

void savtxt() {
	TXHST*	 currentHistoryItem;

	if (TextureScreen.index) {
		currentHistoryItem = &TextureHistory[TextureHistoryIndex];
		if (chktxh(currentHistoryItem)) {
			setMap(WASTXBAK);
			rstMap(TXBDIR);
			rstMap(LASTXBAK);
			txrfor();
			currentHistoryItem = &TextureHistory[TextureHistoryIndex];
			currentHistoryItem->count = TextureScreen.index;
			currentHistoryItem->height = TextureScreen.areaHeight;
			currentHistoryItem->width = TextureScreen.width;
			currentHistoryItem->spacing = TextureScreen.spacing;
			if (currentHistoryItem->texturePoint) {
				delete[](currentHistoryItem->texturePoint);
				currentHistoryItem->texturePoint = nullptr;
			}
			currentHistoryItem->texturePoint = new TXPNT[currentHistoryItem->count];
			MoveMemory(currentHistoryItem->texturePoint, TempTexturePoints, currentHistoryItem->count * sizeof(TXPNT));
		}
	}
}

void deorg(POINT* point) {
	point->x = Msg.pt.x - StitchWindowOrigin.x;
	point->y = Msg.pt.y - StitchWindowOrigin.y;
}

void fthvars() {

	rstMap(BARSAT);
	rstMap(FTHR);
	FeatherFillType = SelectedForm->fillInfo.feather.fillType;
	FormFeatherRatio = SelectedForm->fillInfo.feather.ratio;
	FeatherMinStitch = SelectedForm->fillInfo.feather.minStitchSize;
	FeatherTotalCount = SelectedForm->fillInfo.feather.count;
	ExtendedAttribute = SelectedForm->extendedAttribute;
	FeatherCountUp = FeatherUpCount = SelectedForm->fillInfo.feather.upCount;
	FeatherCountDown = FeatherDownCount = SelectedForm->fillInfo.feather.downCount;
	FeatherPhaseIndex = FeatherUpCount + FeatherDownCount;
	if (ExtendedAttribute&AT_FTHBLND)
		setMap(BARSAT);
	else
		setMap(FTHR);
}

float durat(float start, float finish) {

	return (finish - start)*FeatherRatio + start;
}

float duxrat(float strt, float fin) {

	return (fin - strt)*FeatherRatioLocal + strt;
}

void duxrats(unsigned start, unsigned finish, fPOINT* point) {

	point->x = duxrat(BSequence[finish].x, BSequence[start].x);
	point->y = duxrat(BSequence[finish].y, BSequence[start].y);
}

void durats(unsigned iSequence, fPOINT* point) {

	double	stitchLength;
	fPOINT	adjustedPoint;

	stitchLength = hypot(BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y);
	if (stitchLength < FeatherMinStitch) {

		point->x = BSequence[iSequence].x;
		point->y = BSequence[iSequence].y;
	}
	else {

		FeatherRatioLocal = FeatherMinStitch / stitchLength;
		adjustedPoint.x = duxrat(BSequence[iSequence + 1].x, BSequence[iSequence].x);
		adjustedPoint.y = duxrat(BSequence[iSequence + 1].y, BSequence[iSequence].y);
		point->x = durat(adjustedPoint.x, BSequence[iSequence].x);
		point->y = durat(adjustedPoint.y, BSequence[iSequence].y);
	}
}

void xoseq(unsigned iSequence) {

	OSequence[OutputIndex].x = BSequence[iSequence].x;
	OSequence[OutputIndex].y = BSequence[iSequence].y;
	OutputIndex++;
}

void xpfth(unsigned iSequence) {

	FeatherSequence[ActivePointIndex].x = BSequence[iSequence].x;
	FeatherSequence[ActivePointIndex].y = BSequence[iSequence].y;
	ActivePointIndex++;
}

unsigned bpsg() {

	unsigned testValue;

	if (!PseudoRandomValue)
		PseudoRandomValue = FSED;
	testValue = PseudoRandomValue & 0x40000008;
	PseudoRandomValue >>= 1;
	if (testValue == 0x8 || testValue == 0x40000000)
		PseudoRandomValue |= 0x40000000;
	return PseudoRandomValue;
}

void nurat() {

	float	remainder;

	remainder = fmod(FeatherGlobalPosition, 1);
	switch (FeatherFillType) {

		case FTHPSG:

			if (FeatherUpCount) {

				if (FeatherCountUp) {

					FeatherRatio = (float)(FeatherTotalCount - (psg() % FeatherTotalCount)) / FeatherTotalCount;
					FeatherCountUp--;
				}
				else {

					FeatherRatio = (float)(FeatherTotalCount - (bpsg() % FeatherTotalCount)) / FeatherTotalCount;
					if (FeatherCountDown)
						FeatherCountDown--;
					else {

						PseudoRandomValue = FSED;
						FeatherCountUp = FeatherUpCount;
						FeatherCountDown = FeatherDownCount;
					}
				}
			}
			else
				FeatherRatio = (float)(FeatherTotalCount - (psg() % FeatherTotalCount)) / FeatherTotalCount;
			FeatherRatio *= FormFeatherRatio;
			break;

		case FTHFAZ:

			if (FeatherPhase >= FeatherUpCount)
				FeatherRatio = 1;
			else
				FeatherRatio = FormFeatherRatio;
			break;

		case FTHSIN:

			if (remainder > FeatherGlobalRatio)
				FeatherRatio = sin((1 - remainder) / (1 - FeatherGlobalRatio)*PI + PI)*0.5 + 0.5;
			else
				FeatherRatio = sin(remainder / FeatherGlobalRatio*PI)*0.5 + 0.5;
			FeatherRatio *= FormFeatherRatio;
			break;

		case FTHSIN2:

			if (remainder > FeatherGlobalRatio)
				FeatherRatio = sin((1 - remainder) / (1 - FeatherGlobalRatio)*PI);
			else
				FeatherRatio = sin(remainder / FeatherGlobalRatio*PI);
			FeatherRatio *= FormFeatherRatio;
			break;

		case FTHRMP:

			if (remainder > FeatherGlobalRatio)
				FeatherRatio = (1 - remainder) / (1 - FeatherGlobalRatio);
			else
				FeatherRatio = remainder / FeatherGlobalRatio;
			FeatherRatio *= FormFeatherRatio;
			break;

		case FTHLIN:
		default:

			FeatherRatio = FormFeatherRatio;
	}
	++FeatherPhase %= FeatherPhaseIndex;
	FeatherGlobalPosition += FeatherGlobalStep;
}

void fthfn(unsigned iSequence) {

	nurat();
	durats(iSequence, &OSequence[iSequence]);
}

void ratpnt(unsigned iPoint, unsigned iNextPoint, fPOINT* point) {

	point->x = (BSequence[iNextPoint].x - BSequence[iPoint].x)*FeatherRatio + BSequence[iPoint].x;
	point->y = (BSequence[iNextPoint].y - BSequence[iPoint].y)*FeatherRatio + BSequence[iPoint].y;
}

void midpnt(fPOINT startPoint, fPOINT endPoint, fPOINT* midPoint) {

	midPoint->x = (endPoint.x - startPoint.x)*(float)0.5 + startPoint.x;
	midPoint->y = (endPoint.y - startPoint.y)*(float)0.5 + startPoint.y;
}

void xratf(fPOINT startPoint, fPOINT endPoint, fPOINT* point) {

	point->x = (endPoint.x - startPoint.x)*FeatherRatioLocal + startPoint.x;
	point->y = (endPoint.y - startPoint.y)*FeatherRatioLocal + startPoint.y;
}

void fthrbfn(unsigned iSequence) {

	fPOINT	currentPoint;
	fPOINT	nextPoint;
	fPOINT	currentHighPoint;
	fPOINT	currentLowPoint;
	fPOINT	nextHighPoint;
	fPOINT	nextLowPoint;
	fPOINT	midPoint;
	double	length;

	nurat();
	length = hypot(BSequence[iSequence + 1].y - BSequence[iSequence].y, BSequence[iSequence + 1].x - BSequence[iSequence].x);
	if (length < (FeatherMinStitch * 2)) {
		FeatherRatio = 0.5;
		ratpnt(iSequence, iSequence + 1, &currentPoint);
		ratpnt(iSequence + 3, iSequence + 2, &nextPoint);
	}
	else {
		FeatherRatioLocal = FeatherMinStitch / length;
		duxrats(iSequence, iSequence + 1, &currentLowPoint);
		duxrats(iSequence + 3, iSequence + 2, &nextLowPoint);
		FeatherRatioLocal = 1 - FeatherRatioLocal;
		duxrats(iSequence, iSequence + 1, &currentHighPoint);
		duxrats(iSequence + 3, iSequence + 2, &nextHighPoint);
		FeatherRatioLocal = FeatherRatio;
		xratf(currentLowPoint, currentHighPoint, &currentPoint);
		xratf(nextLowPoint, nextHighPoint, &nextPoint);
	}
	midpnt(currentPoint, nextPoint, &midPoint);
	xoseq(iSequence);
	OSequence[OutputIndex].x = midPoint.x;
	OSequence[OutputIndex].y = midPoint.y;
	OutputIndex++;
	xpfth(iSequence + 1);
	FeatherSequence[ActivePointIndex].x = midPoint.x;
	FeatherSequence[ActivePointIndex].y = midPoint.y;
	ActivePointIndex++;
}

void duoseq(unsigned iSequence) {

	OSequence[iSequence].x = BSequence[iSequence].x;
	OSequence[iSequence].y = BSequence[iSequence].y;
}

void fthdfn(unsigned iSequence) {

	double	length;
	fPOINT	adjustedPoint;
	fPOINT	currentPoint;
	fPOINT	nextPoint;

	nurat();
	length = hypot(BSequence[iSequence + 1].y - BSequence[iSequence].y, BSequence[iSequence + 1].x - BSequence[iSequence].x);
	duoseq(iSequence);
	duoseq(iSequence + 1);
	if (length > FeatherMinStitch) {

		FeatherRatioLocal = 0.5;
		duxrats(iSequence + 1, iSequence, &adjustedPoint);
		FeatherRatioLocal = FeatherMinStitch / length / 2;
		xratf(adjustedPoint, OSequence[iSequence], &currentPoint);
		xratf(adjustedPoint, OSequence[iSequence + 1], &nextPoint);
		FeatherRatioLocal = FeatherRatio;
		xratf(currentPoint, OSequence[iSequence], &OSequence[iSequence]);
		xratf(nextPoint, OSequence[iSequence + 1], &OSequence[iSequence + 1]);
	}
}

void fthrfn() {

	unsigned	ind, res;
	double		savedSpacing;

	// ToDo - what does this function do
	PseudoRandomValue = FSED;
	fthvars();
	savedSpacing = StitchSpacing;
	StitchSpacing = SelectedForm->fillSpacing;
	satfil();
	BSequence[0].attribute = 0;
	BSequence[1].attribute = 1;
	if (!FeatherPhaseIndex)
		FeatherPhaseIndex = 1;
	ind = SequenceIndex / (FeatherPhaseIndex << 2);
	res = SequenceIndex % (FeatherPhaseIndex << 2);
	if (res > (FeatherPhaseIndex << 1))
		ind++;
	FeatherGlobalPosition = 0;
	FeatherGlobalStep = (float)4 / SequenceIndex*ind;
	FeatherGlobalPhase = (float)SequenceIndex / ind;
	FeatherGlobalRatio = (float)FeatherCountUp / FeatherPhaseIndex;
	FeatherGlobalUp = FeatherGlobalPhase*FeatherGlobalRatio;
	FeatherGlobalDown = FeatherGlobalPhase - FeatherGlobalUp;
	SelectedForm->fillType = FTHF;
	FeatherPhase = 1;
	BSequence[SequenceIndex].x = BSequence[SequenceIndex - 2].x;
	BSequence[SequenceIndex].y = BSequence[SequenceIndex - 2].y;
	BSequence[SequenceIndex].attribute = BSequence[SequenceIndex - 2].attribute;
	BSequence[SequenceIndex + 1].x = BSequence[SequenceIndex - 1].x;
	BSequence[SequenceIndex + 1].y = BSequence[SequenceIndex - 1].y;
	BSequence[SequenceIndex + 1].attribute = BSequence[SequenceIndex - 1].attribute;
	if (ExtendedAttribute&AT_FTHBLND) {

		OutputIndex = ActivePointIndex = 0;
		for (ind = 0; ind < SequenceIndex; ind++) {
			if (!BSequence[ind].attribute)
				fthrbfn(ind);
		}
	}
	else {

		if (SelectedForm->extendedAttribute&AT_FTHBTH) {

			for (ind = 0; ind <= SequenceIndex; ind++) {

				if (!BSequence[ind].attribute)
					fthdfn(ind);
			}
			ind--;
		}
		else {

			for (ind = 0; ind <= SequenceIndex; ind++) {

				if (BSequence[ind].attribute) {

					if (ExtendedAttribute&AT_FTHUP)
						fthfn(ind);
					else
						duoseq(ind);
				}
				else {

					if (ExtendedAttribute&AT_FTHUP)
						duoseq(ind);
					else
						fthfn(ind);
				}
			}
			ind--;
		}
		OutputIndex = ind;
	}
	rstMap(FTHR);
	rstMap(BARSAT);
	StitchSpacing = savedSpacing;
	SequenceIndex = OutputIndex;
}

void fritfil() {

	unsigned iSequence, iReverseSequence;

	if (SequenceIndex) {

		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = TYPFRM;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->fillColor;
		chkseq(false);
		InterleaveSequenceIndex2++;
		if (SelectedForm->extendedAttribute&AT_FTHBLND && ~(SelectedForm->extendedAttribute&(AT_FTHUP | AT_FTHBTH)) != (AT_FTHUP | AT_FTHBTH)) {

			InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
			InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FTH;
			InterleaveSequenceIndices[InterleaveSequenceIndex2].code = FTHMSK;
			InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->fillInfo.feather.color;
			iReverseSequence = ActivePointIndex - 1;
			for (iSequence = 0; iSequence < ActivePointIndex; iSequence++) {

				OSequence[iSequence].x = FeatherSequence[iReverseSequence].x;
				OSequence[iSequence].y = FeatherSequence[iReverseSequence].y;
				iReverseSequence--;
			}
			SequenceIndex = ActivePointIndex;
			chkseq(false);
			InterleaveSequenceIndex2++;
		}
	}
}

void fethrf() {

	if (FormIndex) {

		fvars(ClosestFormToCursor);
		delclps(ClosestFormToCursor);
		deltx();
		SelectedForm->type = SAT;
		SelectedForm->fillInfo.feather.ratio = IniFile.featherRatio;
		SelectedForm->fillInfo.feather.upCount = IniFile.featherUpCount;
		SelectedForm->fillInfo.feather.downCount = IniFile.featherDownCount;
		SelectedForm->fillInfo.feather.fillType = IniFile.featherFillType;
		SelectedForm->fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
		SelectedForm->extendedAttribute &= !(AT_FTHUP | AT_FTHBTH | AT_FTHBLND);
		SelectedForm->extendedAttribute |= IniFile.featherType;
		SelectedForm->fillInfo.feather.count = IniFile.featherCount;
		SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
		SelectedForm->fillSpacing = StitchSpacing;
		SelectedForm->fillColor = ActiveColor;
		SelectedForm->fillInfo.feather.color = (ActiveColor + 1)&COLMSK;
		SelectedForm->fillType = FTHF;
		refilfn();
	}
}

void fethr() {

	unsigned iForm;

	if (filmsgs(FMM_FTH))
		return;
	if (SelectedFormCount) {

		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {

			ClosestFormToCursor = SelectedFormList[iForm];
			fethrf();
		}
		setMap(INIT);
		coltab();
		setMap(RESTCH);
	}
	else {

		if (chkMap(FORMSEL)) {

			savdo();
			fethrf();
			setMap(INIT);
			coltab();
			setMap(RESTCH);
		}
	}
}

ULARGE_INTEGER  tim2int(FILETIME time) {

	ULARGE_INTEGER  op;

	op.LowPart = time.dwLowDateTime;
	op.HighPart = time.dwHighDateTime;
	return op;
}

int fil2crd(const char* fileName) {
	STARTUPINFO			startupInfo;
	PROCESS_INFORMATION	processInfo;
	int					errorCode = 0;
	char				command[_MAX_PATH * 2 + 1 + 4];

	strcpy_s(command, "\"");
	strcat_s(command, IniFile.p2cName);
	strcat_s(command, "\" \"");
	strcat_s(command, fileName);
	strcat_s(command, "\"");
	memset(&startupInfo, 0, sizeof(STARTUPINFO));
	startupInfo.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(0, command, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, &startupInfo, &processInfo)) {
		errorCode = GetLastError();
	}
	else {
		WaitForSingleObject(processInfo.hProcess, INFINITE);
	}
	CloseHandle(processInfo.hProcess);
	CloseHandle(processInfo.hThread);

	return errorCode;
}

BOOL chkp2cnam(char* fileName) {

	HANDLE	handleP2C;

	handleP2C = CreateFile(fileName, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	if (handleP2C == INVALID_HANDLE_VALUE) {
		return 0;
	} 
	else {
		CloseHandle(handleP2C);
		return 1;
	}
}

void pes2crd() {

#define P2CBUFSIZ 256

	HKEY			registryKey;
	unsigned long	keyType, size;
	TCHAR			programName[_MAX_PATH];
	char			filter[] = "ComputerService (Lind2PC.exe)\0LinkP2C.exe\0\0";
	TCHAR			message[P2CBUFSIZ];
	TCHAR			caption[P2CBUFSIZ];
	OPENFILENAME	openFileName = {

	sizeof(OPENFILENAME),	//lStructsize
		ThrEdWindow,		//hwndOwner 
		ThrEdInstance,		//hInstance 
		filter,				//lpstrFilter 
		0,					//lpstrCustomFilter 
		0,					//nMaxCustFilter 
		0,					//nFilterIndex 
		programName,		//lpstrFile 
		_MAX_PATH,			//nMaxFile 
		0,					//lpstrFileTitle 
		0,					//nMaxFileTitle 
		"C:\\",				//lpstr	ialDir 
		0,					//lpstrTitle
		0,					//Flags
		0,					//nFileOffset
		0,					//nFileExtension 
		"exe",				//lpstrDefExt 
		0,					//lCustData 
		0,					//lpfnHook
		0,					//lpTemplateName
	};

	if (PCSHeader.stitchCount)
		save();
	else {

		tabmsg(IDS_P2CNODAT);
		return;
	}
	if (chkp2cnam(IniFile.p2cName)) {

		fil2crd(ThrName);
		return;
	}
	*IniFile.p2cName = 0;
	if (!RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion",
		0, KEY_READ, &registryKey)) {

		size = _MAX_PATH;
		keyType = REG_SZ;
		if (!RegQueryValueEx(registryKey, "ProgramFilesDir", 0, &keyType, (unsigned char*)programName, &size)) {

			strcat_s(programName, "\\Computerservice SSHSBV\\PES2Card\\LinkP2C.exe");
			if (!chkp2cnam(programName))
				*programName = 0;
		}
	}
	if (!*programName) {

		LoadString(ThrEdInstance, IDS_P2CMSG, message, P2CBUFSIZ);
		LoadString(ThrEdInstance, IDS_P2CTITL, caption, P2CBUFSIZ);
		if (IDOK == MessageBox(ThrEdWindow, message, caption, MB_OKCANCEL)) {

			if (GetOpenFileName(&openFileName)) {

				if (!chkp2cnam(programName))
					return;
			}
			else
				return;
		}
		else
			return;
	}
	strcpy_s(IniFile.p2cName, programName);
	fil2crd(AuxName);
}

void sidlen(unsigned start, unsigned finish, double* insideLength, double* outsideLength) {
	*insideLength += hypot(InsidePoints[finish].x - InsidePoints[start].x, InsidePoints[finish].x - InsidePoints[start].x);
	*outsideLength += hypot(OutsidePoints[finish].x - OutsidePoints[start].x, OutsidePoints[finish].x - OutsidePoints[start].x);
}

fPOINT* insid() {
	unsigned iVertex;

	satout(fabs(SelectedForm->underlayIndent));
	if (SelectedForm->underlayIndent > 0) {
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (!cisin(InsidePoints[iVertex].x, InsidePoints[iVertex].y)) {
				InsidePoints[iVertex].x = CurrentFormVertices[iVertex].x;
				InsidePoints[iVertex].y = CurrentFormVertices[iVertex].y;
			}
		}
		return InsidePoints;
	}
	else
		return OutsidePoints;
}

void delwlk(unsigned code) {
	unsigned	iStitch, stitchCount;
	fPOINTATTR*	highStitchBuffer;

	if (PCSHeader.stitchCount) {
		stitchCount = 0;
		highStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute&WLKFMSK) != code) {
				MoveMemory(&highStitchBuffer[stitchCount++], &StitchBuffer[iStitch], sizeof(fPOINTATTR));
			}
		}
		MoveMemory(StitchBuffer, highStitchBuffer, stitchCount * sizeof(fPOINTATTR));
		PCSHeader.stitchCount = stitchCount;
		delete[] highStitchBuffer;
	}
}

void chkuseq() {
#if BUGBAK
	unsigned index;

	for (index = 0; index < OutputIndex; index++) {
		InterleaveSequence[index].x = OSequence[index].x;
		InterleaveSequence[index].y = OSequence[index].y;
	}
	InterleaveSequenceIndex = index;
	InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->UnderlayColor;
#else

	unsigned	iSequence, index, stitchCount;
	double		length;
	fPOINT		delta;
	fPOINT		point;
	fPOINT		step;
	double		underlayStitchLength;

	if (OutputIndex) {
		if (SelectedForm->underlayStitchLen < MINWLK)
			SelectedForm->underlayStitchLen = (float)MINWLK;
		if (SelectedForm->underlayStitchLen > MAXWLK)
			SelectedForm->underlayStitchLen = MAXWLK;
		underlayStitchLength = SelectedForm->underlayStitchLen;
		for (iSequence = 0; iSequence < OutputIndex - 1; iSequence++) {
			delta.x = OSequence[iSequence + 1].x - OSequence[iSequence].x;
			delta.y = OSequence[iSequence + 1].y - OSequence[iSequence].y;
			length = hypot(delta.x, delta.y);
			stitchCount = floor(length / underlayStitchLength);
			if (stitchCount) {
				step.x = delta.x / stitchCount;
				step.y = delta.y / stitchCount;
				point.x = OSequence[iSequence].x;
				point.y = OSequence[iSequence].y;
				for (index = 0; index < stitchCount; index++) {
					InterleaveSequence[InterleaveSequenceIndex].x = point.x;
					InterleaveSequence[InterleaveSequenceIndex].y = point.y;
					InterleaveSequenceIndex++;
					point.x += step.x;
					point.y += step.y;
				}
			}
			else {
				InterleaveSequence[InterleaveSequenceIndex].x = OSequence[iSequence].x;
				InterleaveSequence[InterleaveSequenceIndex].y = OSequence[iSequence].y;
				InterleaveSequenceIndex++;
			}
		}
		InterleaveSequence[InterleaveSequenceIndex].x = OSequence[iSequence].x;
		InterleaveSequence[InterleaveSequenceIndex].y = OSequence[iSequence].y;
		InterleaveSequenceIndex++;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->underlayColor;
	}
#endif
}

void ritwlk() {
	if (OutputIndex) {
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = WLKMSK;
		chkuseq();
		InterleaveSequenceIndex2++;
	}
}

void ritcwlk() {
	if (OutputIndex) {

		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = CWLKMSK;
		chkuseq();
		InterleaveSequenceIndex2++;
	}
}

unsigned gucon(fPOINT start, fPOINT finish, unsigned destination, unsigned code) {
	double		length;
	unsigned	startVertex, endVertex, stitchCount, intermediateVertex;
	fPOINT*		indentedPoint;
	unsigned	up, down, iStitch, iStep;
	fPOINT		localPoint, step, delta;

	length = hypot(finish.x - start.x, finish.y - start.y);
	if (length < 5)
		return 0;
	startVertex = closflt(start.x, start.y);
	endVertex = closflt(finish.x, finish.y);
	if (startVertex == endVertex)
		return 0;
	//	if(chkMap(ISEND))
	//		ipnt=vertices;
	//	else
	indentedPoint = insid();
	up = down = startVertex;
gulab:
	if (up == endVertex) {
		rstMap(WLKDIR);
		goto gulabx;
	}
	if (down == endVertex) {
		setMap(WLKDIR);
		goto gulabx;
	}
	up = nxt(up);
	down = prv(down);
	goto gulab;
gulabx:;
	iStitch = destination;
	while (startVertex != endVertex) {
		StitchBuffer[iStitch].x = indentedPoint[startVertex].x;
		StitchBuffer[iStitch].y = indentedPoint[startVertex].y;
		StitchBuffer[iStitch].attribute = code;
		if (iStitch) {
			if (StitchBuffer[iStitch - 1].x != StitchBuffer[iStitch].x || StitchBuffer[iStitch - 1].y != StitchBuffer[iStitch].y)
				iStitch++;
		}
		else
			iStitch++;
		if (chkMap(WLKDIR))
			intermediateVertex = prv(startVertex);
		else
			intermediateVertex = nxt(startVertex);
		delta.x = indentedPoint[intermediateVertex].x - indentedPoint[startVertex].x;
		delta.y = indentedPoint[intermediateVertex].y - indentedPoint[startVertex].y;
		length = hypot(delta.x, delta.y);
		stitchCount = length / SelectedForm->lengthOrCount.stitchLength;
		if (stitchCount > 1) {
			step.x = delta.x / stitchCount;
			step.y = delta.y / stitchCount;
			localPoint.x = indentedPoint[startVertex].x + step.x;
			localPoint.y = indentedPoint[startVertex].y + step.y;
			for (iStep = 0; iStep < stitchCount - 1; iStep++) {
				StitchBuffer[iStitch].x = localPoint.x;
				StitchBuffer[iStitch].y = localPoint.y;
				StitchBuffer[iStitch].attribute = code;
				iStitch++;
				localPoint.x += step.x;
				localPoint.y += step.y;
			}
		}
		if (chkMap(WLKDIR))
			startVertex = prv(startVertex);
		else
			startVertex = nxt(startVertex);
	}
	StitchBuffer[iStitch].x = indentedPoint[startVertex].x;
	StitchBuffer[iStitch].y = indentedPoint[startVertex].y;
	StitchBuffer[iStitch].attribute = code;
	iStitch++;
	return iStitch - destination;
}

void fnwlk(unsigned find) {
	unsigned	start, count;
	fPOINT*		walkPoints;

	fvars(find);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (SelectedForm->extendedAttribute&AT_STRT && SelectedForm->type != FRMLINE)
		start = SelectedForm->fillStart;
	else
		start = 0;
	count = VertexCount;
	if (SelectedForm->type != FRMLINE)
		count++;
	walkPoints = insid();
	OutputIndex = 0;
	while (count) {
		OSequence[OutputIndex].x = walkPoints[start].x;
		OSequence[OutputIndex].y = walkPoints[start].y;
		start = nxt(start);
		OutputIndex++;
		count--;
	}
	ritwlk();
}

void fnuang() {

	unsigned	iVertex;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {

		AngledForm.vertices[iVertex].x = UnderlayVertices[iVertex].x;
		AngledForm.vertices[iVertex].y = UnderlayVertices[iVertex].y;
		rotflt(&AngledForm.vertices[iVertex]);
	}
	SelectedForm = &AngledForm;
	fnvrt();
	fvars(ClosestFormToCursor);
}

void ritund() {
	if (SequenceIndex) {

		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = UNDMSK;
		chkuseq();
		InterleaveSequenceIndex2++;
	}
}

void undclp() {
	ClipRectSize.cx = ClipRect.bottom = ClipRect.left = ClipRect.right = ClipBuffer[0].x = ClipBuffer[1].x = ClipBuffer[0].y = 0;
	ClipRectSize.cy = ClipRect.top = ClipBuffer[1].y = SelectedForm->underlayStitchLen;
	ClipStitchCount = 2;
}

void fnund(unsigned find) {
	float	savedStitchSize;

	UnderlayVertices = insid();
	savedStitchSize = UserStitchLength;
	UserStitchLength = 1e99;
	if (!SelectedForm->underlaySpacing)
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	if (!SelectedForm->underlayStitchLen)
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	undclp();
	setMap(ISUND);
	angclpfn();
	OutputIndex = SequenceIndex;
	ritund();
	fvars(find);
	UserStitchLength = savedStitchSize;
}

void fncwlk() {
	unsigned	iGuide, iVertex, start, finish;
	SATCON*		guide;

	OutputIndex = 0;
	SelectedForm->extendedAttribute |= AT_CWLK;
	if (SelectedForm->satinGuideCount) {
		if (SelectedForm->wordParam) {
			iVertex = SelectedForm->wordParam;
			OSequence[OutputIndex].x = midl(CurrentFormVertices[iVertex].x, CurrentFormVertices[iVertex + 1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[iVertex].y, CurrentFormVertices[iVertex + 1].y);
			OutputIndex++;
		}
		guide = SelectedForm->satinOrAngle.guide;
		for (iGuide = SelectedForm->satinGuideCount; iGuide != 0; iGuide--) {
			OSequence[OutputIndex].x = midl(CurrentFormVertices[guide[iGuide - 1].finish].x, CurrentFormVertices[guide[iGuide - 1].start].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[guide[iGuide - 1].finish].y, CurrentFormVertices[guide[iGuide - 1].start].y);
			OutputIndex++;
		}
		if (SelectedForm->attribute&FRMEND) {
			OSequence[OutputIndex].x = midl(CurrentFormVertices[0].x, CurrentFormVertices[1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[0].y, CurrentFormVertices[1].y);
			OutputIndex++;
		}
	}
	else {
		if (SelectedForm->extendedAttribute&AT_STRT)
			start = SelectedForm->fillStart;
		else
			start = 0;
		OSequence[OutputIndex].x = CurrentFormVertices[start].x;
		OSequence[OutputIndex].y = CurrentFormVertices[start].y;
		OutputIndex++;
		finish = prv(start);
		start = nxt(start);
		for (iGuide = 1; iGuide < VertexCount >> 1; iGuide++) {
			OSequence[OutputIndex].x = midl(CurrentFormVertices[finish].x, CurrentFormVertices[start].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[finish].y, CurrentFormVertices[start].y);
			if (cisin(OSequence[OutputIndex].x, OSequence[OutputIndex].y))
				OutputIndex++;
			start = nxt(start);
			finish = prv(finish);
		}
		OSequence[OutputIndex].x = CurrentFormVertices[start].x;
		OSequence[OutputIndex].y = CurrentFormVertices[start].y;
		OutputIndex++;
	}
	ritcwlk();
}

BOOL CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buf[HBUFSIZ];
	TCHAR		buf1[HBUFSIZ];
	unsigned	iFeatherStyle, state, featherType;

	switch (umsg) {

		case WM_INITDIALOG:

			featherType = IniFile.featherType;
			SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
			sprintf_s(buf, sizeof(buf), "%.2f", IniFile.featherRatio);
			SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf);
			sprintf_s(buf, sizeof(buf), "%d", IniFile.featherUpCount);
			SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf);
			sprintf_s(buf, sizeof(buf), "%d", IniFile.featherDownCount);
			SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf);
			sprintf_s(buf, sizeof(buf), "%.2f", IniFile.featherMinStitchSize / PFGRAN);
			SetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf);
			sprintf_s(buf, sizeof(buf), "%d", IniFile.featherCount);
			SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf);
			for (iFeatherStyle = 0; iFeatherStyle < 6; iFeatherStyle++) {

				LoadString(ThrEdInstance, IDS_FTH0 + iFeatherStyle, buf, HBUFSIZ);
				SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, (long)buf);
			}
			SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, IniFile.featherFillType - 1, 0);
			if (featherType&AT_FTHBLND)
				state = BST_CHECKED;
			else
				state = BST_UNCHECKED;
			CheckDlgButton(hwndlg, IDC_FDBLND, state);
			if (featherType&AT_FTHUP)
				state = BST_CHECKED;
			else
				state = BST_UNCHECKED;
			CheckDlgButton(hwndlg, IDC_FDUP, state);
			if (featherType&AT_FTHBTH)
				state = BST_CHECKED;
			else
				state = BST_UNCHECKED;
			CheckDlgButton(hwndlg, IDC_FBTH, state);
			break;

		case WM_COMMAND:

			switch (LOWORD(wparam)) {

				case IDCANCEL:

					EndDialog(hwndlg, 0);
					return TRUE;

				case IDOK:

					IniFile.featherType = 0;
					if (IsDlgButtonChecked(hwndlg, IDC_FDBLND))
						IniFile.featherType = AT_FTHBLND;
					if (IsDlgButtonChecked(hwndlg, IDC_FDUP))
						IniFile.featherType |= AT_FTHUP;
					if (IsDlgButtonChecked(hwndlg, IDC_FBTH))
						IniFile.featherType |= AT_FTHBTH;
					GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf, HBUFSIZ);
					IniFile.featherFillType = FDEFTYP;
					for (iFeatherStyle = 0; iFeatherStyle < 6; iFeatherStyle++) {

						LoadString(ThrEdInstance, IDS_FTH0 + iFeatherStyle, buf1, HBUFSIZ);
						if (!strcmp(buf, buf1)) {

							IniFile.featherFillType = iFeatherStyle + 1;
							break;
						}
					}
					GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf, HBUFSIZ);
					IniFile.featherRatio = atof(buf);
					GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf, HBUFSIZ);
					IniFile.featherUpCount = atoi(buf);
					GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf, HBUFSIZ);
					IniFile.featherDownCount = atoi(buf);
					GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf, HBUFSIZ);
					IniFile.featherMinStitchSize = atof(buf)*PFGRAN;
					GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf, HBUFSIZ);
					IniFile.featherCount = atoi(buf);
					if (IniFile.featherCount < 1)
						IniFile.featherCount = 1;
					EndDialog(hwndlg, 1);
					break;
			}
	}
	return 0;
}

void dufdef() {

	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, (DLGPROC)fthdefprc);
}

void srtcol() {

	unsigned		histogram[16] = { 0 }, colorStartStitch[16] = { 0 };
	unsigned		iStitch, iColor, startStitch;
	fPOINTATTR*		highStitchBuffer;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		histogram[StitchBuffer[iStitch].attribute&COLMSK]++;
	startStitch = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		colorStartStitch[iColor] = startStitch;
		startStitch += histogram[iColor];
	}
	highStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		moveStitch(&highStitchBuffer[colorStartStitch[StitchBuffer[iStitch].attribute&COLMSK]++], &StitchBuffer[iStitch]);
	MoveMemory(&StitchBuffer, highStitchBuffer, PCSHeader.stitchCount * sizeof(fPOINTATTR));
	delete[] highStitchBuffer;
}

void dubit(unsigned bit) {
	unsigned code;

	savdo();
	setMap(WASDO);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (!(SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) && bit&(AT_UND | AT_WALK | AT_CWLK)) {
		if (SelectedForm->fillType)
			SelectedForm->underlayColor = SelectedForm->fillColor;
		else
			SelectedForm->underlayColor = ActiveColor;
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	}
	if (!(SelectedForm->extendedAttribute&AT_UND) && bit&AT_UND) {
		SelectedForm->underlayStitchAngle = IniFile.underlayAngle;
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	}
	code = SelectedForm->extendedAttribute&bit;
	if (code)
		SelectedForm->extendedAttribute &= (~bit);
	else
		SelectedForm->extendedAttribute |= bit;
	refil();
	coltab();
	setMap(RESTCH);
}

void setuang() {
	setMap(GTUANG);
	msgflt(IDS_UANG, IniFile.underlayAngle / PI * 180);
}

void setuspac() {
	setMap(GTUSPAC);
	msgflt(IDS_USPAC, IniFile.underlaySpacing / PFGRAN);
}

void setwlkind() {
	setMap(GTWLKIND);
	msgflt(IDS_WLKIND, IniFile.underlayIndent / PFGRAN);
}

void setulen() {
	setMap(GTWLKLEN);
	msgflt(IDS_WLKLEN, IniFile.underlayStitchLen / PFGRAN);
}

void chkcwlk() {
	if (SelectedForm->extendedAttribute&AT_CWLK)
		fncwlk();
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
}

void chkwlk() {
	if (SelectedForm->extendedAttribute&AT_WALK)
		fnwlk(ClosestFormToCursor);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
}

void chkund() {
	if (SelectedForm->extendedAttribute&AT_UND)
		fnund(ClosestFormToCursor);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
}

void selalfrm() {
	for (SelectedFormCount = 0; SelectedFormCount < FormIndex; SelectedFormCount++)
		SelectedFormList[SelectedFormCount] = SelectedFormCount;
	setMap(RESTCH);
}

void chkdaz() {

	if (!IniFile.daisyPetalPoints)
		IniFile.daisyPetalPoints = 1;
	if (!IniFile.daisyInnerCount)
		IniFile.daisyInnerCount = 1;
	if (!IniFile.daisyPetalCount)
		IniFile.daisyPetalCount = 1;
	if (IniFile.daisyHeartCount > IniFile.daisyPetalPoints)
		IniFile.daisyHeartCount = IniFile.daisyPetalPoints;
}

void dazdef() {
	IniFile.daisyPetalPoints = DAZCNT;
	IniFile.daisyHoleDiameter = DAZHLEN;
	IniFile.daisyInnerCount = DAZICNT;
	IniFile.daisyDiameter = DAZLEN;
	IniFile.daisyPetalCount = DAZPETS;
	IniFile.daisyPetalLen = DAZPLEN;
	IniFile.daisyHeartCount = DAZMCNT;
	setu(DAZHOL);
	setu(DAZD);
	IniFile.daisyBorderType = DAZTYP;
}

BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buffer[HBUFSIZ];
	TCHAR		compareBuffer[HBUFSIZ];
	unsigned	iType;

	switch (umsg) {

		case WM_INITDIALOG:

			SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
			// ToDo - remove label reinit 
reinit:;
			chkdaz();
			sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyPetalPoints);
			SetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buffer);
			sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyHeartCount);
			SetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buffer);
			sprintf_s(buffer, sizeof(buffer), "%.2f", IniFile.daisyDiameter);
			SetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buffer);
			sprintf_s(buffer, sizeof(buffer), "%.2f", IniFile.daisyHoleDiameter);
			SetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buffer);
			sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyInnerCount);
			SetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buffer);
			sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyPetalCount);
			SetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buffer);
			sprintf_s(buffer, sizeof(buffer), "%.2f", IniFile.daisyPetalLen);
			SetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buffer);
			CheckDlgButton(hwndlg, IDC_HOLE, chku(DAZHOL));
			CheckDlgButton(hwndlg, IDC_DLIN, chku(DAZD));
			for (iType = 0; iType < 6; iType++) {
				LoadString(ThrEdInstance, DaisyTypeStrings[iType], buffer, HBUFSIZ);
				SendMessage(GetDlgItem(hwndlg, IDC_DAZTYP), CB_ADDSTRING, 0, (long)buffer);
			}
			SendMessage(GetDlgItem(hwndlg, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
			break;

		case WM_COMMAND:

			switch (LOWORD(wparam)) {

				case IDCANCEL:

					EndDialog(hwndlg, 0);
					return TRUE;

				case IDOK:

					GetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buffer, HBUFSIZ);
					IniFile.daisyPetalPoints = atoi(buffer);
					GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buffer, HBUFSIZ);
					IniFile.daisyHeartCount = atoi(buffer);
					GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buffer, HBUFSIZ);
					IniFile.daisyDiameter = atof(buffer);
					GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buffer, HBUFSIZ);
					IniFile.daisyHoleDiameter = atof(buffer);
					GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buffer, HBUFSIZ);
					IniFile.daisyInnerCount = atoi(buffer);
					GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buffer, HBUFSIZ);
					IniFile.daisyPetalCount = atoi(buffer);
					GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buffer, HBUFSIZ);
					IniFile.daisyPetalLen = atof(buffer);
					if (IsDlgButtonChecked(hwndlg, IDC_HOLE))
						setu(DAZHOL);
					else
						rstu(DAZHOL);
					if (IsDlgButtonChecked(hwndlg, IDC_DLIN))
						setu(DAZD);
					else
						rstu(DAZD);
					GetWindowText(GetDlgItem(hwndlg, IDC_DAZTYP), buffer, HBUFSIZ);
					for (iType = 0; iType < 6; iType++) {
						LoadString(ThrEdInstance, DaisyTypeStrings[iType], compareBuffer, HBUFSIZ);
						if (!strcmp(buffer, compareBuffer)) {
							IniFile.daisyBorderType = iType;
							break;
						}
					}
					chkdaz();
					EndDialog(hwndlg, 1);
					break;

				case IDC_DAZRST:

					dazdef();
					goto reinit;

				case IDC_DLIN:

					if (IsDlgButtonChecked(hwndlg, IDC_DLIN))
						CheckDlgButton(hwndlg, IDC_HOLE, BST_CHECKED);
					break;

				case IDC_HOLE:

					if (!IsDlgButtonChecked(hwndlg, IDC_HOLE))
						CheckDlgButton(hwndlg, IDC_DLIN, BST_UNCHECKED);
					break;
			}
	}
	return 0;
}

void dasyfrm() {

	double		holeSegmentAngle, petalSegmentAngle;
	double		angle;
	double		deltaPetalAngle;
	double		petalPointAngle;
	unsigned	holeVertexCount, petalVertexCount, iSegment, iMacroPetal, iPoint;
	unsigned	iVertex, borderType, sawPointCount, halfPetalPointCount, fref, petalPointCount;
	double		distanceFromDaisyCenter = 0;
	double		diameter;
	double		petalLength;
	double		holeDiameter;
	double		ratio;
	float		maximumXsize, maximumYsize;
	fPOINT		referencePoint;

	unmsg();
	if (!DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, (DLGPROC)dasyproc)) {
		rstMap(FORMIN);
		return;
	}
	IniFile.daisyPetalCount = IniFile.daisyPetalCount;
	diameter = IniFile.daisyDiameter;
	petalLength = IniFile.daisyPetalLen;
	holeDiameter = IniFile.daisyHoleDiameter;
	borderType = IniFile.daisyBorderType;
	referencePoint.x = midl(ZoomRect.right, ZoomRect.left);
	referencePoint.y = midl(ZoomRect.top, ZoomRect.bottom);
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	halfPetalPointCount = IniFile.daisyPetalPoints >> 1;
	maximumXsize = ZoomRect.right - ZoomRect.left;
	maximumYsize = ZoomRect.top - ZoomRect.bottom;
	if (maximumYsize > maximumXsize)
		maximumXsize = maximumYsize;
	maximumXsize /= 6;
	ratio = (double)maximumXsize / (diameter + petalLength);
	diameter *= ratio;
	petalLength *= ratio;
	holeDiameter *= ratio;
	SelectedForm->type = FRMFPOLY;
	iVertex = fref = 0;
	if (chku(DAZHOL)) {
		angle = PI2;
		holeVertexCount = IniFile.daisyPetalCount*IniFile.daisyInnerCount;
		holeSegmentAngle = PI2 / holeVertexCount;
		CurrentFormVertices[iVertex].x = referencePoint.x + diameter*cos(angle);
		CurrentFormVertices[iVertex].y = referencePoint.y + diameter*sin(angle);
		iVertex++;
		for (iSegment = 0; iSegment < holeVertexCount + 1; iSegment++) {
			CurrentFormVertices[iVertex].x = referencePoint.x + holeDiameter*cos(angle);
			CurrentFormVertices[iVertex].y = referencePoint.y + holeDiameter*sin(angle);
			iVertex++;
			angle -= holeSegmentAngle;
		}
		fref = iVertex;
	}
	angle = 0;
	petalVertexCount = IniFile.daisyPetalCount*IniFile.daisyPetalPoints;
	petalPointCount = IniFile.daisyPetalPoints;
	if (borderType == DHART) {
		petalPointCount = (IniFile.daisyHeartCount + 1) << 1;
		petalVertexCount = IniFile.daisyPetalCount*petalPointCount;
	}
	petalSegmentAngle = PI2 / petalVertexCount;
	deltaPetalAngle = PI / IniFile.daisyPetalPoints;
	if (chku(DAZD)) {
		SelectedForm->satinGuideCount = IniFile.daisyPetalCount - 1;
		SelectedForm->wordParam = IniFile.daisyPetalCount*IniFile.daisyInnerCount + 1;
		SelectedForm->satinOrAngle.guide = adsatk(IniFile.daisyPetalCount - 1);
	}
	for (iMacroPetal = 0; iMacroPetal < IniFile.daisyPetalCount; iMacroPetal++) {
		petalPointAngle = 0;
		PseudoRandomValue = SEED;
		for (iPoint = 0; iPoint < petalPointCount; iPoint++) {
			switch (borderType) {
				case DSIN:

					distanceFromDaisyCenter = diameter + sin(petalPointAngle)*petalLength;
					petalPointAngle += deltaPetalAngle;
					break;

				case DRAMP:

					distanceFromDaisyCenter = diameter + (double)iPoint / IniFile.daisyPetalPoints*petalLength;
					break;

				case DSAW:

					if (iPoint > halfPetalPointCount)
						sawPointCount = IniFile.daisyPetalPoints - iPoint;
					else
						sawPointCount = iPoint;
					distanceFromDaisyCenter = diameter + (double)sawPointCount / IniFile.daisyPetalPoints*petalLength;
					break;

				case DRAG:

					distanceFromDaisyCenter = diameter + (double)(psg() % IniFile.daisyPetalPoints) / IniFile.daisyPetalPoints*petalLength;
					break;

				case DCOG:

					distanceFromDaisyCenter = diameter;
					if (iPoint > halfPetalPointCount)
						distanceFromDaisyCenter += petalLength;
					break;

				case DHART:

					distanceFromDaisyCenter = diameter + sin(petalPointAngle)*petalLength;
					if (iPoint > IniFile.daisyHeartCount)
						petalPointAngle -= deltaPetalAngle;
					else
						petalPointAngle += deltaPetalAngle;
					break;
			}
			CurrentFormVertices[iVertex].x = referencePoint.x + cos(angle)*distanceFromDaisyCenter;
			CurrentFormVertices[iVertex].y = referencePoint.y + sin(angle)*distanceFromDaisyCenter;
			iVertex++;
			angle += petalSegmentAngle;
			if (chku(DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
				SelectedForm->satinOrAngle.guide[iMacroPetal].start = (IniFile.daisyPetalCount - iMacroPetal - 1)*IniFile.daisyInnerCount + 1;
				SelectedForm->satinOrAngle.guide[iMacroPetal].finish = iVertex;
			}
		}
	}
	if (chku(DAZHOL)) {
		CurrentFormVertices[fref - 1].y += (float)0.01;
		CurrentFormVertices[fref].y += (float)0.01;
	}
	SelectedForm->vertexCount = iVertex;
	if (chku(DAZD)) {
		SelectedForm->type = SAT;
		SelectedForm->attribute = 1;
	}
	FormVertexIndex += iVertex;
	setMap(INIT);
	frmout(FormIndex);
	for (iMacroPetal = 0; iMacroPetal < iVertex; iMacroPetal++) {

		CurrentFormVertices[iMacroPetal].x -= SelectedForm->rectangle.left;
		CurrentFormVertices[iMacroPetal].y -= SelectedForm->rectangle.bottom;
	}
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = iVertex + 1;
	setMap(POLIMOV);
	setmfrm();
	mdufrm();
}

void durec(OREC* record) {
	unsigned	attribute;
	fPOINTATTR*	stitch;

	stitch = &StitchBuffer[record->start];
	record->type = StitchTypes[dutyp(stitch->attribute)];
	attribute = stitch->attribute&SRTMSK;
	record->color = attribute & 0xf;
	record->form = (attribute&FRMSK) >> FRMSHFT;
}


#if  __UseASM__
OREC*	recref(const void* arg) {
	_asm
	{
		mov		eax, arg
		mov		eax, [eax]
	}
}

int recmp(const void *arg1, const void *arg2) {
	OREC* record1;
	OREC* record2;

	record1 = recref((const void*)arg1);
	record2 = recref((const void*)arg2);
	if (ColorOrder[record1->color] == ColorOrder[record2->color]) {
		if (record1->form == record2->form) {
			if (record1->type == record2->type)
				return (int)record1->start - record2->start;
			else
				return (int)record1->type - record2->type;
		}
		else
			return (int)record1->form - record2->form;
	}
	return (int)ColorOrder[record1->color] - ColorOrder[record2->color];
}

int refcmp(const void *arg1, const void *arg2) {
	OREC* record1;
	OREC* record2;

	record1 = recref((const void*)arg1);
	record2 = recref((const void*)arg2);
	if (record1->form == record2->form)
		return (int)record1->type - record2->type;
	return (int)record1->form - record2->form;
}
#else
int recmp(const void *arg1, const void *arg2) {
	OREC record1 = **(OREC **)arg1;
	OREC record2 = **(OREC **)arg2;

	if (ColorOrder[record1.color] == ColorOrder[record2.color]) {
		if (record1.form == record2.form) {
			if (record1.type == record2.type)
				return (int)record1.start - record2.start;
			else
				return (int)record1.type - record2.type;
		}
		else
			return (int)record1.form - record2.form;
	}
	return (int)ColorOrder[record1.color] - ColorOrder[record2.color];
}

int refcmp(const void *arg1, const void *arg2) {
	OREC record1 = **(OREC **)arg1;
	OREC record2 = **(OREC **)arg2;

	if (record1.form == record2.form)
		return (int)record1.type - record2.type;
	return (int)record1.form - record2.form;
}
#endif

BOOL chkrdun(SRTREC* stitchRecord) {
	unsigned iStitch;

	for (iStitch = stitchRecord->start; iStitch < stitchRecord->finish; iStitch++) {
		if (PRecs[iStitch]->otyp == FormFillCounter[PRecs[iStitch]->form])
			return 1;
	}
	return 0;
}

double precjmps(SRTREC* stitchRecord) {
	unsigned		totalJumps;
	double			length;
	double			minimumLength;
	unsigned		iStitch, loc, loci;
	fPOINTATTR*		currentStitch;
	BOOL			direction;

	FormFillCounter = new unsigned[(FormIndex + 2) << 2]();
	// ToDo - rename loc (and loci)
	loc = stitchRecord->loc;
	direction = stitchRecord->direction;
	totalJumps = 0;
	while (chkrdun(stitchRecord)) {
		minimumLength = 1e9;
		if (direction)
			loci = PRecs[loc]->finish;
		else
			loci = PRecs[loc]->start;
		currentStitch = &StitchBuffer[loci];
		for (iStitch = stitchRecord->start; iStitch < stitchRecord->finish; iStitch++) {
			if (PRecs[iStitch]->otyp == FormFillCounter[PRecs[iStitch]->form]) {
				length = hypot(PRecs[iStitch]->startStitch->x - currentStitch->x, PRecs[iStitch]->startStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction = 0;
					loc = iStitch;
				}
				length = hypot(PRecs[iStitch]->endStitch->x - currentStitch->x, PRecs[iStitch]->endStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction = 1;
					loc = iStitch;
				}
			}
		}
		if (minimumLength > 9 * PFGRAN)
			totalJumps++;
		FormFillCounter[PRecs[loc]->form]++;
		if (chkMap(DUSRT)) {
			if (direction) {
				if (PRecs[loc]->start) {
					for (iStitch = PRecs[loc]->finish - 1; iStitch >= PRecs[loc]->start; iStitch--)
						moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[iStitch]);
				}
				else {
					iStitch = PRecs[loc]->finish;
					while (iStitch)
						moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[--iStitch]);
				}
			}
			else {
				for (iStitch = PRecs[loc]->start; iStitch < PRecs[loc]->finish; iStitch++)
					moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[iStitch]);
			}
		}
	}
	delete[] FormFillCounter;
	return totalJumps;
}

unsigned duprecs(SRTREC* record) {
	unsigned jumps0;
	unsigned jumps1;

	record->direction = 0;
	jumps0 = precjmps(record);
	record->direction = 1;
	jumps1 = precjmps(record);
	if (jumps0 < jumps1) {
		record->direction = 0;
		return jumps0;
	}
	record->direction = 1;
	return jumps1;
}

#ifdef _DEBUG

void dmprec(OREC** records, unsigned count) {
	unsigned iRecord;

	for (iRecord = 0; iRecord < count; iRecord++) {
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%4d off: %4d at: %08x frm: %4d typ: %d col: %2d st: %5d fin: %5d\n",
			iRecord,
			records[iRecord] - records[0],
			StitchBuffer[records[iRecord]->start].attribute,
			records[iRecord]->form,
			records[iRecord]->type,
			records[iRecord]->color,
			records[iRecord]->start,
			records[iRecord]->finish);
		OutputDebugString(MsgBuffer);
	}
}
#endif

BOOL srtchk(OREC** record, unsigned count, unsigned* badForm) {
	unsigned iRecord, form, color;
	FRMHED* formHeader;

	form = record[0]->form;
	color = record[0]->color;
	for (iRecord = 1; iRecord < count; iRecord++) {
		if (record[iRecord]->form == form) {
			if (ColorOrder[record[iRecord]->color] < ColorOrder[color]) {
				formHeader = &FormList[form];
				if (formHeader->fillType == FTHF && formHeader->extendedAttribute&AT_FTHBLND && record[iRecord]->color == formHeader->fillColor)
					continue;
				*badForm = iRecord;
				return 0;
			}
			else
				color = record[iRecord]->color;
		}
		else {
			color = record[iRecord]->color;
			form = record[iRecord]->form;
		}
	}
	return 1;
}

void fsort() {
	unsigned		iColor, iStitch, badForm, iRange, lastRange;
	unsigned		currentForm, attribute, iRecord, lastRecord;
	unsigned		minimumIndex = 0, minimumDirection = 0;
	unsigned		typeCount, jumps, minimumJumps;
	OREC*			records;
	RANGE*			stitchRange;
	SRTREC			sortRecord;
	FILETIME		fileTime;
	ULARGE_INTEGER	startTime;
	ULARGE_INTEGER	nextTime;

	// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors 
	//        in a single form are not in ascending order already. 
	savdo();
	// There cannot be more records than stitches
	records = new OREC[PCSHeader.stitchCount];
	records[0].start = 0;
	records[0].startStitch = StitchBuffer;
	attribute = StitchBuffer->attribute&SRTMSK;
	iRecord = 0;
	ColorOrder[AppliqueColor] = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		if (iColor != AppliqueColor)
			ColorOrder[iColor] = iColor + 1;
	}
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute&SRTMSK) != attribute) {
			records[iRecord].finish = iStitch;
			records[iRecord].endStitch = &StitchBuffer[iStitch - 1];
			iRecord++;
			records[iRecord].start = iStitch;
			records[iRecord].startStitch = &StitchBuffer[iStitch];
			attribute = StitchBuffer[iStitch].attribute&SRTMSK;
		}
	}
	records[iRecord].endStitch = &StitchBuffer[PCSHeader.stitchCount - 1];
	records[iRecord].finish = PCSHeader.stitchCount;
	iRecord++;
	lastRecord = iRecord;
	PRecs = new OREC*[lastRecord];
	PFRecs = new OREC*[lastRecord];
	for (iRecord = 0; iRecord < lastRecord; iRecord++) {
		durec(&records[iRecord]);
		PRecs[iRecord] = &records[iRecord];
		PFRecs[iRecord] = &records[iRecord];
	}
	qsort(PRecs, lastRecord, sizeof(OREC *), recmp);
	qsort(PFRecs, lastRecord, sizeof(OREC *), refcmp);
#ifdef _DEBUG
	dmprec(PRecs, lastRecord);
#endif
	if (srtchk(PFRecs, lastRecord, &badForm)) {
		stitchRange = new RANGE[lastRecord];
		stitchRange[0].start = 0;
		attribute = PRecs[0]->color;
		currentForm = 0xffffffff;
		typeCount = 0;
		iRange = 0;
		for (iRecord = 0; iRecord < lastRecord; iRecord++) {
			if (attribute != PRecs[iRecord]->color) {
				stitchRange[iRange].finish = iRecord;
				iRange++;
				stitchRange[iRange].start = iRecord;
				attribute = PRecs[iRecord]->color;
				currentForm = PRecs[iRecord]->form;
				typeCount = 0;
				goto srtskp;
			}
			if (PRecs[iRecord]->form == currentForm)
				typeCount++;
			else {
				typeCount = 0;
				currentForm = PRecs[iRecord]->form;
			}
srtskp:;
			PRecs[iRecord]->otyp = typeCount;
		}
		stitchRange[iRange].finish = lastRecord;
		lastRange = ++iRange;
		TempStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
		OutputIndex = 0;
		for (iRange = 0; iRange < lastRange; iRange++) {
			rstMap(DUSRT);
			sortRecord.start = stitchRange[iRange].start;
			sortRecord.finish = stitchRange[iRange].finish;
			sortRecord.count = sortRecord.finish - sortRecord.start;
			minimumJumps = 0xffffffff;
			// ToDo - why is there a timeout implemented here?
			GetSystemTimeAsFileTime(&fileTime);
			startTime = tim2int(fileTime);
			for (iRecord = sortRecord.start; iRecord < sortRecord.finish; iRecord++) {
				sortRecord.loc = iRecord;
				if (!PRecs[iRecord]->otyp) {
					jumps = duprecs(&sortRecord);
					if (jumps < minimumJumps) {
						minimumJumps = jumps;
						minimumIndex = iRecord;
						minimumDirection = sortRecord.direction;
					}
				}
				GetSystemTimeAsFileTime(&fileTime);
				nextTime = tim2int(fileTime);
				if (nextTime.QuadPart - startTime.QuadPart > SRTIM)
					break;
			}
			setMap(DUSRT);
			sortRecord.loc = minimumIndex;
			sortRecord.direction = minimumDirection;
			precjmps(&sortRecord);
		}
		MoveMemory(StitchBuffer, TempStitchBuffer, OutputIndex * sizeof(fPOINTATTR));
		PCSHeader.stitchCount = OutputIndex;
		delete[] TempStitchBuffer;
		coltab();
		setMap(RESTCH);
		delete[] stitchRange;
	}
	else {
		LoadString(ThrEdInstance, IDS_SRTER, HelpBuffer, HBUFSIZ);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), HelpBuffer, PFRecs[badForm]->form);
		shoMsg(MsgBuffer);
	}
	delete[] PFRecs;
	delete[] PRecs;
	delete[] records;
}

unsigned dutyp(unsigned attribute) {
#if  __UseASM__
	_asm
	{
		xor		eax, eax
		mov		ebx, attribute
		and		ebx, SRTYPMSK
		bsr		eax, ebx
		je		short dutypx
		sub		al, 18
		cmp		al, 12
		jne		short dutypx
		test	ebx, 0x20000000
		je		short dutypx
		mov		al, 1
dutypx :
		and		eax, 0xf
	}
#else
	//correct
	char result;
	DWORD bit;
	unsigned maskedAttribute;

	maskedAttribute = attribute & SRTYPMSK;

	_BitScanReverse(&bit, maskedAttribute);

	if (bit == 0)
		return 0;

	result = ((bit & 0xff) - 18);

	if ((result != 12) || ((maskedAttribute & TYPATMSK) == 0))
		return result & 0xf;

	return 1;
#endif
}

#ifdef _DEBUG

typedef struct _atfld {

	unsigned	color;
	unsigned	form;
	unsigned	type;
	unsigned	layer;
	unsigned	user;
}ATFLD;

void duatf(unsigned ind) {
	char		attributeBuffer[256];
	unsigned	attribute;
	ATFLD		AttributeFields;

	attribute = StitchBuffer[ind].attribute;
	AttributeFields.color = attribute&COLMSK;
	AttributeFields.form = (attribute >> FRMSHFT)&FRMSK;
	AttributeFields.type = StitchTypes[dutyp(attribute)];
	AttributeFields.layer = (attribute >> LAYSHFT) & 7;
	if (attribute & 0x80000000)
		AttributeFields.user = 1;
	else
		AttributeFields.user = 0;
	sprintf_s(attributeBuffer, sizeof(attributeBuffer), "%5d: col: %2d frm: %5d typ: %2d, lay: %1d: usr: %1d at: %08x\n",
		ind,
		AttributeFields.color,
		AttributeFields.form,
		AttributeFields.type,
		AttributeFields.layer,
		AttributeFields.user,
		attribute);
	OutputDebugString(attributeBuffer);
}

void dmpat() {
	unsigned iStitch, attribute;

	duatf(0);
	attribute = StitchBuffer[0].attribute;
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (attribute != StitchBuffer[iStitch].attribute) {
			duatf(iStitch);
			attribute = StitchBuffer[iStitch].attribute;
		}
	}
}
#endif

void fdelstch() {
	unsigned	iStitch, ind, ine, codedFormIndex, attribute, type, tmap, color, bordercolor, tapcol;
	unsigned*	stpnt;
	// ToDo - Still not sure what this function does?
	//        I suspect the FillStartsData members are not correctly named
	fvars(ClosestFormToCursor);
	tmap = 0;
	FillMemory(&FillStartsData, sizeof(FSTRTS), 0);
	codedFormIndex = (ClosestFormToCursor << FRMSHFT);
	ine = 0;
	bordercolor = SelectedForm->borderColor&COLMSK;
	tapcol = SelectedForm->borderColor >> 4;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (!chku(FIL2OF) && chkMap(SELBOX) && iStitch == ClosestPointIndex)
			ClosestPointIndex = ine;
		attribute = StitchBuffer[iStitch].attribute;
		if (codedFormIndex == (attribute&(FRMSK | NOTFRM))) {
			type = StitchTypes[dutyp(attribute)];
			switch (type) {
				case TYPE_APPLIQUE:

					if (!(tmap&M_AP)) {
						tmap |= M_AP;
						FillStartsData.applique = ine;
					}
					break;

				case TYPE_FTHR:

					if (!(tmap&M_FTH)) {
						tmap |= M_FTH;
						FillStartsData.feather = ine;
					}
					break;

				case TYPE_FILL:

					if (!(tmap&M_FIL)) {
						tmap |= M_FIL;
						FillStartsData.fill = ine;
					}
					break;

				case TYPE_BORDER:

					if (!(tmap&M_BRD)) {
						tmap |= M_BRD;
						FillStartsData.border = ine;
					}
					break;

				default:

					if (SelectedForm->fillType && !(tmap&M_FIL)) {
						tmap |= M_FIL;
						FillStartsData.fill = ine;
					}
					break;
			}
		}
		else {
			// ToDo - there is a potential problem here when the active color is 0
			//        because the default color is 0 as well. Should the default color 
			//        be e.g. -1 ?
			color = attribute&COLMSK;
			if (color == SelectedForm->fillColor) {
				tmap |= M_FCOL;
				FillStartsData.fillColor = ine;
			}
			if (color == SelectedForm->fillInfo.feather.color) {
				tmap |= M_FTHCOL;
				FillStartsData.featherColor = ine;
			}
			if (color == bordercolor) {
				tmap |= M_ECOL;
				FillStartsData.borderColor = ine;
			}
			if (color == tapcol) {
				tmap |= M_APCOL;
				FillStartsData.appliqueColor = ine;
			}
			StitchBuffer[ine].x = StitchBuffer[iStitch].x;
			StitchBuffer[ine].y = StitchBuffer[iStitch].y;
			StitchBuffer[ine].attribute = StitchBuffer[iStitch].attribute;
			ine++;
		}
	}
	FillStartsData.fillColor++;
	FillStartsData.featherColor++;
	FillStartsData.borderColor++;
	FillStartsData.appliqueColor++;
	FillStartsMap = tmap;
	PCSHeader.stitchCount = ine;
	ine = 0;
	if (!(tmap&M_ECOL))
		FillStartsData.borderColor = PCSHeader.stitchCount;
	if (!(tmap&M_FTHCOL))
		FillStartsData.featherColor = PCSHeader.stitchCount;
	if (!(tmap&M_FCOL))
		FillStartsData.fillColor = PCSHeader.stitchCount;
	if (SelectedForm->edgeType) {
		if (SelectedForm->edgeType == EDGEAPPL) {
			if (!(tmap&M_AP)) {
				if (tmap&M_APCOL)
					FillStartsData.applique = FillStartsData.appliqueColor + 1;
				else
					FillStartsData.applique = PCSHeader.stitchCount;
			}
		}
		if (!(tmap&M_BRD)) {
			if (tmap&M_ECOL)
				FillStartsData.border = FillStartsData.borderColor + 1;
			else
				FillStartsData.border = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType || (tmap&(M_WALK | M_UND | M_CWLK))) {
		if (!(tmap&M_FIL)) {
			if (tmap&M_FCOL)
				FillStartsData.fill = FillStartsData.fillColor + 1;
			else
				FillStartsData.fill = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType == FTHF) {
		if (!(tmap&M_FTH)) {
			if (tmap&M_FTHCOL)
				FillStartsData.feather = FillStartsData.featherColor + 1;
			else
				FillStartsData.feather = PCSHeader.stitchCount;
		}
	}
	stpnt = (unsigned*)&FillStartsData;
	for (ind = 3; ind; ind--) {
		ine = ind - 1;
		while (ine < ind) {
			if (stpnt[ine] > stpnt[ind])
				stpnt[ind] = stpnt[ine];
			ine--;
		}
	}
	if (!chku(FIL2OF) && chkMap(SELBOX)) {
		for (ind = 0; ind < sizeof(FillStartsData) >> 2; ind++)
			stpnt[ind] = ClosestPointIndex;
	}
}

BOOL lastcol(unsigned index, fPOINT* point) {
	unsigned color;

	color = InterleaveSequenceIndices[index].color;
	while (index) {
		index--;
		if (InterleaveSequenceIndices[index].color == color) {
			point->x = InterleaveSequence[InterleaveSequenceIndices[index + 1].index - 1].x;
			point->y = InterleaveSequence[InterleaveSequenceIndices[index + 1].index - 1].y;
			return 1;
		}
	}
	return 0;
}

void duint(unsigned offset, unsigned code) {
	unsigned	count, iSequence;
	fPOINT		point;

	if (InterleaveData.coloc > InterleaveData.start) {
		count = InterleaveData.coloc - InterleaveData.start;
		MoveMemory(&InterleaveData.highStitchBuffer[InterleaveData.output], &StitchBuffer[InterleaveData.start], sizeof(fPOINTATTR)*count);
		InterleaveData.start += count;
		InterleaveData.output += count;
	}
	if (SelectedForm->extendedAttribute&AT_STRT) {
		if (!setMap(DIDSTRT))
			InterleaveData.output += gucon(CurrentFormVertices[SelectedForm->fillStart], InterleaveSequence[InterleaveSequenceIndices[InterleaveData.pins].index], InterleaveData.output + offset, code);
	}
	if (lastcol(InterleaveData.pins, &point))
		InterleaveData.output += gucon(point, InterleaveSequence[InterleaveSequenceIndices[InterleaveData.pins].index], InterleaveData.output + MAXITEMS, code);
	for (iSequence = InterleaveSequenceIndices[InterleaveData.pins].index; iSequence < InterleaveSequenceIndices[InterleaveData.pins + 1].index; iSequence++) {
		InterleaveData.highStitchBuffer[InterleaveData.output].x = InterleaveSequence[iSequence].x;
		InterleaveData.highStitchBuffer[InterleaveData.output].y = InterleaveSequence[iSequence].y;
		InterleaveData.highStitchBuffer[InterleaveData.output].attribute = code;
		if (InterleaveData.highStitchBuffer[InterleaveData.output].x != InterleaveData.highStitchBuffer[InterleaveData.output - 1].x ||
			InterleaveData.highStitchBuffer[InterleaveData.output].y != InterleaveData.highStitchBuffer[InterleaveData.output - 1].y)
			InterleaveData.output++;
	}
}

BOOL isfil() {
	if (SelectedForm->fillType)
		return 1;
	if (SelectedForm->edgeType)
		return 1;
	if (SelectedForm->extendedAttribute&(AT_CWLK | AT_WALK | AT_UND))
		return 1;
	return 0;
}

void chkend(unsigned offset, unsigned code) {
	if (isfil()) {
		setMap(ISEND);
		if (SelectedForm->extendedAttribute&AT_END)
			InterleaveData.output += gucon(InterleaveSequence[InterleaveSequenceIndex - 1], CurrentFormVertices[SelectedForm->fillEnd], InterleaveData.output + offset, code);
	}
}

void intlv() {
	unsigned	iSequence, ine, code = 0, offset;
	fPOINT		colpnt;

	rstMap(ISEND);
	fvars(ClosestFormToCursor);
	InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
	FillMemory(&InterleaveData, sizeof(INTINF), 0);
	InterleaveData.layerIndex = ((SelectedForm->attribute&FRMLMSK) << (LAYSHFT - 1)) | (ClosestFormToCursor << FRMSHFT);
	rstMap(DIDSTRT);
	if (PCSHeader.stitchCount) {
		offset = MAXITEMS;
		// Todo - Allocate memory locally for InterleaveData.highStitchBuffer
		InterleaveData.highStitchBuffer = &StitchBuffer[MAXITEMS];
		for (iSequence = 0; iSequence < InterleaveSequenceIndex2; iSequence++) {
			InterleaveData.pins = iSequence;
			switch (InterleaveSequenceIndices[iSequence].seq) {
				case I_AP:

					if (FillStartsMap&M_FIL && FillStartsData.applique >= InterleaveData.coloc)
						InterleaveData.coloc = FillStartsData.applique;
					else {
						InterleaveData.coloc = FillStartsData.appliqueColor;
						if (InterleaveData.coloc == 1)
							InterleaveData.coloc = 0;
					}
					break;

				case I_FIL:

					if (FillStartsMap&M_FIL && FillStartsData.fill >= InterleaveData.coloc)
						InterleaveData.coloc = FillStartsData.fill;
					else
						InterleaveData.coloc = FillStartsData.fillColor;
					break;

				case I_FTH:

					if (FillStartsMap&M_FIL && FillStartsData.feather >= InterleaveData.coloc)
						InterleaveData.coloc = FillStartsData.feather;
					else
						InterleaveData.coloc = FillStartsData.featherColor;
					break;

				case I_BRD:

					if (FillStartsMap&M_BRD && FillStartsData.border >= InterleaveData.coloc)
						InterleaveData.coloc = FillStartsData.border;
					else
						InterleaveData.coloc = FillStartsData.borderColor;
					break;
			}
			code = InterleaveData.layerIndex | InterleaveSequenceIndices[InterleaveData.pins].code | InterleaveSequenceIndices[InterleaveData.pins].color;
			duint(offset, code);
		}
		chkend(MAXITEMS, code);
		if (PCSHeader.stitchCount && InterleaveData.start < (unsigned)PCSHeader.stitchCount - 1) {
			ine = PCSHeader.stitchCount - InterleaveData.start;
			MoveMemory(&StitchBuffer[InterleaveData.output + MAXITEMS], &StitchBuffer[InterleaveData.start], sizeof(fPOINTATTR)*ine);
			InterleaveData.output += ine;
		}
		MoveMemory(StitchBuffer, InterleaveData.highStitchBuffer, sizeof(fPOINTATTR)*InterleaveData.output);
	}
	else {
		offset = 0;
		for (iSequence = 0; iSequence < InterleaveSequenceIndex2; iSequence++) {
			code = InterleaveData.layerIndex | InterleaveSequenceIndices[iSequence].code | InterleaveSequenceIndices[iSequence].color;
			if (SelectedForm->extendedAttribute&AT_STRT) {
				if (!setMap(DIDSTRT))
					InterleaveData.output += gucon(CurrentFormVertices[SelectedForm->fillStart], InterleaveSequence[InterleaveSequenceIndices[InterleaveData.pins].index], InterleaveData.output + offset, code);
			}
			if (lastcol(iSequence, &colpnt))
				InterleaveData.output += gucon(colpnt, InterleaveSequence[InterleaveSequenceIndices[iSequence].index], InterleaveData.output, code);
			for (ine = InterleaveSequenceIndices[iSequence].index; ine < InterleaveSequenceIndices[iSequence + 1].index; ine++) {
				StitchBuffer[InterleaveData.output].x = InterleaveSequence[ine].x;
				StitchBuffer[InterleaveData.output].y = InterleaveSequence[ine].y;
				StitchBuffer[InterleaveData.output].attribute = code;
				if (StitchBuffer[InterleaveData.output].x != StitchBuffer[InterleaveData.output - 1].x ||
					StitchBuffer[InterleaveData.output].y != StitchBuffer[InterleaveData.output - 1].y)
					InterleaveData.output++;
			}
		}
		chkend(0, code);
	}
	PCSHeader.stitchCount = InterleaveData.output;
	coltab();
}

void setundfn(unsigned code) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = SelectedFormList[iForm];
			fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE)
				continue;
			SelectedForm->extendedAttribute |= code;
			refilfn();
		}
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL)) {
		fvars(ClosestFormToCursor);
		SelectedForm->extendedAttribute |= code;
		refilfn();
		coltab();
		setMap(RESTCH);
	}
}

void setund() {
	setundfn(AT_UND);
}

void setwlk() {
	setundfn(AT_WALK);
}

void setcwlk() {
	setundfn(AT_CWLK);
}

void notundfn(unsigned code) {
	unsigned iForm;

	savdo();
	code = !code;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {
			ClosestFormToCursor = iForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE)
				continue;
			SelectedForm->extendedAttribute &= code;
			refilfn();
		}
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (SelectedForm->type == FRMLINE)
			return;
		SelectedForm->extendedAttribute &= code;
		refilfn();
		coltab();
		setMap(RESTCH);
	}
}

void notund() {
	notundfn(AT_UND);
}

void notwlk() {
	notundfn(AT_WALK);
}

void notcwlk() {
	notundfn(AT_CWLK);
}

void ulenfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) {
		SelectedForm->underlayStitchLen = length;
		refilfn();
	}
}

void dusulen(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			ulenfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		ulenfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void undlen() {
	tabmsg(IDS_SETULEN);
	setMap(NUMIN);
	setMap(FSETULEN);
	numWnd();
}

void uspacfn(unsigned find, float spacing) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&AT_UND) {
		SelectedForm->underlaySpacing = spacing;
		refilfn();
	}
}

void duspac(float spacing) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			uspacfn(SelectedFormList[iForm], spacing);
	}
	if (chkMap(FORMSEL))
		uspacfn(ClosestFormToCursor, spacing);
	coltab();
	setMap(RESTCH);
}

void uspac() {
	tabmsg(IDS_SETUSPAC);
	setMap(NUMIN);
	setMap(FSETUSPAC);
	numWnd();
}

void uangfn(unsigned find, float angle) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&AT_UND) {
		SelectedForm->underlayStitchAngle = angle;
		refilfn();
	}
}

void dufang(float angle) {
	unsigned iForm;

	savdo();
	angle *= (float)PI / 180;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			uangfn(SelectedFormList[iForm], angle);
	}
	if (chkMap(FORMSEL))
		uangfn(ClosestFormToCursor, angle);
	coltab();
	setMap(RESTCH);
}

void sfuang() {
	tabmsg(IDS_SETUANG);
	setMap(NUMIN);
	setMap(FSETUANG);
	numWnd();
}

void flenfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType && !isclp(find)) {
		SelectedForm->lengthOrCount.stitchLength = length;
		refilfn();
	}
}

void duflen(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			flenfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		flenfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setflen() {
	tabmsg(IDS_SETFLEN);
	setMap(NUMIN);
	setMap(FSETFLEN);
	numWnd();
}

void fspacfn(unsigned find, float spacing) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		if (spacing < 0) {
			if (!isclp(find))
				return;
		}
		SelectedForm->fillSpacing = spacing;
		refilfn();
	}
}

void dufspac(float spacing) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fspacfn(SelectedFormList[iForm], spacing);
	}
	if (chkMap(FORMSEL))
		fspacfn(ClosestFormToCursor, spacing);
	coltab();
	setMap(RESTCH);
}

void setfspac() {
	tabmsg(IDS_SETFSPAC);
	setMap(NUMIN);
	setMap(FSETFSPAC);
	numWnd();
}

void findfn(unsigned find, float indent) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	SelectedForm->underlayIndent = indent;
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK))
		refilfn();
}

void dufind(float indent) {
	unsigned iForm;

	savdo();
	indent *= PFGRAN;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			findfn(SelectedFormList[iForm], indent);
	}
	if (chkMap(FORMSEL))
		findfn(ClosestFormToCursor, indent);
	coltab();
	setMap(RESTCH);
}

void fangfn(unsigned find, float angle) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->type == FRMFPOLY && SelectedForm->fillType) {
		switch (SelectedForm->fillType) {
			case VRTF:
			case HORF:
			case ANGF:

				SelectedForm->fillType = ANGF;
				SelectedForm->angleOrClipData.angle = angle;
				break;

			case VCLPF:
			case HCLPF:
			case ANGCLPF:

				SelectedForm->fillType = ANGCLPF;
				SelectedForm->satinOrAngle.angle = angle;
				break;
		}
		refilfn();
	}
}

void dufxang(float angle) {
	unsigned iForm;

	savdo();
	angle *= (float)PI / 180;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fangfn(SelectedFormList[iForm], angle);
	}
	if (chkMap(FORMSEL))
		fangfn(ClosestFormToCursor, angle);
	coltab();
	setMap(RESTCH);
}

void setfang() {
	tabmsg(IDS_SETFANG);
	setMap(NUMIN);
	setMap(FSETFANG);
	numWnd();
}

void ucolfn(unsigned find, unsigned color) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) {
		SelectedForm->underlayColor = color;
		refilfn();
	}
}

void dundcol(unsigned color) {
	unsigned iForm;

	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			ucolfn(SelectedFormList[iForm], color);
	}
	if (chkMap(FORMSEL))
		ucolfn(ClosestFormToCursor, color);
	coltab();
	setMap(RESTCH);
}

void setucol() {
	tabmsg(IDS_COL);
	setMap(NUMIN);
	setMap(FSETUCOL);
	numWnd();
}

void fcolfn(unsigned find, unsigned color) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->fillColor = color;
		refilfn();
	}
}

void dufcol(unsigned color) {
	unsigned iForm;

	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fcolfn(SelectedFormList[iForm], color);
	}
	if (chkMap(FORMSEL))
		fcolfn(ClosestFormToCursor, color);
	coltab();
	setMap(RESTCH);
}

void setfcol() {
	tabmsg(IDS_COL);
	setMap(NUMIN);
	setMap(FSETFCOL);
	numWnd();
}

void bcolfn(unsigned find, unsigned color) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->borderColor = color;
		refilfn();
	}
}

void dubcol(unsigned color) {
	unsigned iForm;

	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bcolfn(SelectedFormList[iForm], color);
	}
	if (chkMap(FORMSEL))
		bcolfn(ClosestFormToCursor, color);
	coltab();
	setMap(RESTCH);
}

void setbcol() {
	tabmsg(IDS_COL);
	setMap(NUMIN);
	setMap(FSETBCOL);
	numWnd();
}

void blenfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType && !iseclp(find)) {
		SelectedForm->lengthOrCount.stitchLength = length;
		refilfn();
	}
}

void dublen(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			blenfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		blenfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setblen() {
	tabmsg(IDS_SETFLEN);
	setMap(NUMIN);
	setMap(FSETBLEN);
	numWnd();
}

void bspacfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->edgeSpacing = length;
		refilfn();
	}
}

void dubspac(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bspacfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		bspacfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setbspac() {
	tabmsg(IDS_SETFSPAC);
	setMap(NUMIN);
	setMap(FSETBSPAC);
	numWnd();
}

void bminfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->minBorderStitchLen = length;
		refilfn();
	}
}

void dubmin(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bminfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		bminfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setbmin() {
	tabmsg(IDS_TXT23);
	setMap(NUMIN);
	setMap(FSETBMIN);
	numWnd();
}

void bmaxfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->maxBorderStitchLen = length;
		refilfn();
	}
}

void dubmax(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bmaxfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		bmaxfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setbmax() {
	tabmsg(IDS_TXT22);
	setMap(NUMIN);
	setMap(FSETBMAX);
	numWnd();
}

void fminfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->minFillStitchLen = length;
		refilfn();
	}
}

void dufmin(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fminfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		fminfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setfmin() {
	tabmsg(IDS_TXT21);
	setMap(NUMIN);
	setMap(FSETFMIN);
	numWnd();
}

void fmaxfn(unsigned find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->maxFillStitchLen = length;
		refilfn();
	}
}

void dufmax(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fmaxfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		fmaxfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setfmax() {
	tabmsg(IDS_TXT20);
	setMap(NUMIN);
	setMap(FSETFMAX);
	numWnd();
}

void fwidfn(unsigned find, float length) {
	unsigned	iVertex;
	double		ratio;
	float		reference;

	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	reference = SelectedForm->rectangle.left;
	ratio = length / (SelectedForm->rectangle.right - reference);
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference)*ratio + reference;
	frmout(ClosestFormToCursor);
	refilfn();
}

void dufwid(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fwidfn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		fwidfn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setfwid() {
	tabmsg(IDS_WID);
	setMap(NUMIN);
	setMap(FSETFWID);
	numWnd();
}

void setfind() {
	tabmsg(IDS_UWLKIND);
	setMap(NUMIN);
	setMap(FSETFIND);
	numWnd();
}

void fhifn(unsigned find, float length
) {
	unsigned	iVertex;
	double		ratio;
	float		reference;

	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	reference = SelectedForm->rectangle.bottom;
	ratio = length / (SelectedForm->rectangle.top - reference);
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
		CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference)*ratio + reference;
	frmout(ClosestFormToCursor);
	refilfn();
}

void dufhi(float length) {
	unsigned iForm;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fhifn(SelectedFormList[iForm], length);
	}
	if (chkMap(FORMSEL))
		fhifn(ClosestFormToCursor, length);
	coltab();
	setMap(RESTCH);
}

void setfhi() {
	tabmsg(IDS_HI);
	setMap(NUMIN);
	setMap(FSETFHI);
	numWnd();
}

void setfilstrt() {
	if (chkMap(FRMPSEL)) {
		FormList[ClosestFormToCursor].fillStart = ClosestVertexToCursor;
		FormList[ClosestFormToCursor].extendedAttribute |= AT_STRT;
		refil();
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FORMP, IDS_FSTRT);
}

void setfilend() {
	if (chkMap(FRMPSEL)) {
		FormList[ClosestFormToCursor].fillEnd = ClosestVertexToCursor;
		FormList[ClosestFormToCursor].extendedAttribute |= AT_END;
		refil();
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FORMP, IDS_FEND);
}

void duauxnam() {
	TCHAR* fileExtention;

	_strlwr_s(WorkingFileName);
	strcpy_s(AuxName, WorkingFileName);
	fileExtention = strrchr(AuxName, '.');
	if (fileExtention)
		fileExtention++;
	else
		fileExtention = &WorkingFileName[strlen(WorkingFileName)];
	*fileExtention = 0;
	switch (IniFile.auxFileType) {

		case AUXDST:

			strcat_s(AuxName, "dst");
			break;

#if PESACT

		case AUXPES:

			strcat_s(AuxName, "pes");
			break;

#endif

		default:

			strcat_s(AuxName, "pcs");
	}
}

void dutxtfil() {
	if (!IniFile.textureHeight)
		IniFile.textureHeight = ITXHI;
	if (!IniFile.textureWidth)
		IniFile.textureWidth = ITXWID;
	if (!IniFile.textureSpacing)
		IniFile.textureSpacing = (float)ITXSPAC;
	if (!IniFile.textureEditorSize)
		IniFile.textureEditorSize = ITXPIX;
	AngledForm.vertexCount = 0;
	setMap(TXTRED);
	setMap(ZUMED);
	rstMap(WASPAT);
	rstMap(RUNPAT);
	movStch();
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
	SelectedTexturePointsList = new unsigned[MAXITEMS];
	SelectedTexturePointsCount = 0;
	setMap(INIT);
	SideWindowButton = 0;
	if (chkMap(WASTXBAK)) {
		redtbak();
		if (!TextureScreen.areaHeight)
			TextureScreen.areaHeight = IniFile.textureHeight;
		if (!TextureScreen.spacing)
			TextureScreen.spacing = IniFile.underlaySpacing;
		if (!TextureScreen.width)
			TextureScreen.width = IniFile.textureWidth;
		setMap(LASTXBAK);
		rstMap(TXBDIR);
	}
	else {
		TextureScreen.index = 0;
		TextureScreen.areaHeight = IniFile.textureHeight;
		TextureScreen.width = IniFile.textureWidth;
		TextureScreen.spacing = IniFile.textureSpacing;
	}
	setMap(RESTCH);
}

void txt2pix(TXPNT texturePoint, POINT* screenPoint) {
	screenPoint->y = TextureScreen.height - texturePoint.y / TextureScreen.areaHeight*TextureScreen.height + TextureScreen.top;
	screenPoint->x = (texturePoint.line*TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
}

void txtxfn(POINT reference, int offsetPixels) {
	POINT line[2];

	line[0].x = line[1].x = reference.x;
	line[0].y = reference.y - offsetPixels;
	line[1].y = reference.y + offsetPixels;
	Polyline(StitchWindowMemDC, line, 2);
	line[0].y = line[1].y = reference.y;
	line[0].x = reference.x - offsetPixels;
	line[1].x = reference.x + offsetPixels;
	Polyline(StitchWindowMemDC, line, 2);
}

void dutxtx(int index, int offsetPixels) {
	POINT	ref;

	txt2pix(TempTexturePoints[index], &ref);
	txtxfn(ref, offsetPixels);
	if (ref.y > TextureScreen.halfHeight)
		ref.y -= TextureScreen.height;
	else
		ref.y += TextureScreen.height;
	txtxfn(ref, offsetPixels);
}

void txrct2rct(TXTRCT textureRect, RECT* rectangle) {
	TXPNT	texturePoint;
	POINT	point;

	texturePoint.line = textureRect.left;
	texturePoint.y = textureRect.top;
	txt2pix(texturePoint, &point);
	rectangle->left = point.x - IniFile.textureEditorSize;
	rectangle->top = point.y - IniFile.textureEditorSize;
	texturePoint.line = textureRect.right;
	texturePoint.y = textureRect.bottom;
	txt2pix(texturePoint, &point);
	rectangle->right = point.x + IniFile.textureEditorSize;
	rectangle->bottom = point.y + IniFile.textureEditorSize;
}

void ed2px(fPOINT editPoint, POINT* point) {
	point->x = editPoint.x / TextureScreen.editToPixelRatio;
	point->y = StitchWindowClientRect.bottom - editPoint.y / TextureScreen.editToPixelRatio;
}

void px2ed(POINT point, fPOINT* editPoint) {
	editPoint->x = point.x*TextureScreen.editToPixelRatio;
	editPoint->y = TextureScreen.screenHeight - point.y*TextureScreen.editToPixelRatio;
}

void bxtxt(unsigned iButton, TCHAR* string) {
	SetWindowText(ButtonWin[iButton], string);
}

void lodhbuf(unsigned iString) {
	LoadString(ThrEdInstance, iString, HelpBuffer, HBUFSIZ);
}

void hlpflt(unsigned iMessage, unsigned iButton, float data) {
	lodhbuf(iMessage);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), HelpBuffer, data);
	bxtxt(iButton, MsgBuffer);
}

void drwtxbut() {
	lodhbuf(IDS_CLEAR);
	bxtxt(HTXCLR, HelpBuffer);
	hlpflt(IDS_TXHI, HTXHI, TextureScreen.areaHeight / PFGRAN);
	redraw(ButtonWin[HTXWID]);
	hlpflt(IDS_TXSPAC, HTXSPAC, TextureScreen.spacing / PFGRAN);
	lodhbuf(IDS_TXVRT);
	bxtxt(HTXVRT, HelpBuffer);
	lodhbuf(IDS_TXHOR);
	bxtxt(HTXHOR, HelpBuffer);
	lodhbuf(IDS_TXANG);
	bxtxt(HTXANG, HelpBuffer);
	lodhbuf(IDS_TXMIR);
	bxtxt(HTXMIR, HelpBuffer);
	SetWindowText(ButtonWin[HTXMIR + 1], "");
}

void chktx() {
	unsigned iPoint, iNextPoint;

	iNextPoint = 0;
	for (iPoint = 0; iPoint < TextureScreen.index; iPoint++) {
		if (TempTexturePoints[iPoint].line <= TextureScreen.lines && TempTexturePoints[iPoint].y < TextureScreen.areaHeight) {
			TempTexturePoints[iNextPoint].line = TempTexturePoints[iPoint].line;
			TempTexturePoints[iNextPoint].y = TempTexturePoints[iPoint].y;
			iNextPoint++;
		}
	}
	TextureScreen.index = iNextPoint;
}

void drwtxtr() {
	POINT		line[2];
	int			iGrid, iVertical;
	unsigned	iPoint;
	unsigned	index;
	int			yOffset;
	TXPNT		textureRecord;
	POINT		point;
	int			gridLineCount;
	double		editSpace;
	double		pixelSpace;
	float		extraWidth;

	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	editSpace = TextureScreen.areaHeight * 2 / (TextureScreen.spacing*(TextureScreen.lines + 2));
	pixelSpace = (double)StitchWindowClientRect.bottom / StitchWindowClientRect.right;
	TextureScreen.lines = floor(TextureScreen.width / TextureScreen.spacing);
	extraWidth = TextureScreen.spacing*(TextureScreen.lines + 2);
	if (rstMap(CHKTX))
		chktx();
	if (pixelSpace > editSpace) {
		TextureScreen.xOffset = 0;
		TextureScreen.editToPixelRatio = extraWidth / StitchWindowClientRect.bottom;
		yOffset = (StitchWindowClientRect.bottom - TextureScreen.areaHeight / TextureScreen.editToPixelRatio) / 2;
	}
	else {
		TextureScreen.editToPixelRatio = TextureScreen.areaHeight * 2 / StitchWindowClientRect.bottom;
		yOffset = StitchWindowClientRect.bottom >> 2;
		TextureScreen.xOffset = (StitchWindowClientRect.right*TextureScreen.editToPixelRatio - (TextureScreen.lines + 2)*TextureScreen.spacing) / 2;
	}
	TextureScreen.top = yOffset;
	TextureScreen.bottom = StitchWindowClientRect.bottom - yOffset;
	TextureScreen.height = TextureScreen.bottom - TextureScreen.top;
	TextureScreen.halfHeight = StitchWindowClientRect.bottom >> 1;
	TextureScreen.screenHeight = StitchWindowClientRect.bottom*TextureScreen.editToPixelRatio;
	TextureScreen.yOffset = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, GridPen);
	gridLineCount = TextureScreen.areaHeight / IniFile.gridSize + 1;
	textureRecord.line = 0;
	line[0].x = 0;
	line[1].x = StitchWindowClientRect.right;
	textureRecord.y = 0;
	for (iGrid = 0; iGrid < gridLineCount; iGrid++) {
		txt2pix(textureRecord, &point);
		line[0].y = line[1].y = point.y;
		Polyline(StitchWindowMemDC, line, 2);
		textureRecord.y += IniFile.gridSize;
	}
	DeleteObject(TextureCrossPen);
	TextureCrossPen = CreatePen(PS_SOLID, 1, UserColor[ActiveColor]);
	SelectObject(StitchWindowMemDC, TextureCrossPen);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	line[0].y = 0;
	line[1].y = StitchWindowClientRect.bottom;
	for (iVertical = 1; iVertical < TextureScreen.lines + 1; iVertical++) {
		line[0].x = line[1].x = (iVertical*TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
		Polyline(StitchWindowMemDC, line, 2);
	}
	line[0].x = 0;
	line[1].x = StitchWindowClientRect.right;
	line[0].y = line[1].y = TextureScreen.top;
	Polyline(StitchWindowMemDC, line, 2);
	line[0].y = line[1].y = TextureScreen.bottom;
	Polyline(StitchWindowMemDC, line, 2);
	DeleteObject(TextureCrossPen);
	TextureCrossPen = CreatePen(PS_SOLID, 1, 0xffffff);
	SelectObject(StitchWindowMemDC, TextureCrossPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	for (index = 0; index < TextureScreen.index; index++) {
		dutxtx(index, IniFile.textureEditorSize);
	}
	if (SelectedTexturePointsCount) {
		txrct2rct(TextureRect, &TexturePixelRect);
		line[0].y = line[1].y = TexturePixelRect.top;
		line[0].x = TexturePixelRect.left;
		line[1].x = TexturePixelRect.right;
		Polyline(StitchWindowMemDC, line, 2);
		line[1].y = TexturePixelRect.bottom;
		line[1].x = TexturePixelRect.left;
		Polyline(StitchWindowMemDC, line, 2);
		line[0].x = TexturePixelRect.right;
		line[0].y = TexturePixelRect.bottom;
		Polyline(StitchWindowMemDC, line, 2);
		line[1].x = TexturePixelRect.right;
		line[1].y = TexturePixelRect.top;
		Polyline(StitchWindowMemDC, line, 2);
	}
	for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
		dutxtx(SelectedTexturePointsList[iPoint], IniFile.textureEditorSize);
		dutxtx(SelectedTexturePointsList[iPoint], IniFile.textureEditorSize << 1);
	}
	BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
	drwtxbut();
}

BOOL px2txt(POINT offset, TXPNT* textureRecord) {
	fPOINT editPoint;

	px2ed(offset, &editPoint);
	textureRecord->line = (editPoint.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5;
	if (textureRecord->line > TextureScreen.lines)
		return 0;
	if (textureRecord->line < 1)
		return 0;
	if (offset.y > TextureScreen.top) {
		if (offset.y > TextureScreen.bottom)
			return 0;
		else
			textureRecord->y = (float)TextureScreen.areaHeight - (float)(offset.y - TextureScreen.top) / TextureScreen.height*TextureScreen.areaHeight;
	}
	else
		return 0;
	return 1;
}

BOOL txbutfn(TXPNT* textureRecord) {
	POINT	offset;

	deorg(&offset);
	return px2txt(offset, textureRecord);
}

void txtrbut() {
	if (txbutfn(&TempTexturePoints[TextureScreen.index])) {
		savtxt();
		TextureScreen.index++;
		setMap(RESTCH);
	}
	else
		rstxt();

}

BOOL txtclos(unsigned* closestTexturePoint) {
	double		length;
	double		minimumLength;
	unsigned	iPoint;
	POINT		reference, point;

	deorg(&reference);
	minimumLength = 1e99;
	*closestTexturePoint = 0;
	for (iPoint = 0; iPoint < TextureScreen.index; iPoint++) {
		txt2pix(TempTexturePoints[iPoint], &point);
		length = hypot(point.x - reference.x, point.y - reference.y);
		if (length < minimumLength) {
			minimumLength = length;
			*closestTexturePoint = iPoint;
		}
	}
	if (minimumLength < CLOSENUF)
		return 1;
	return 0;
}

void tritx() {
	POINT	line[2];
	int		size;

	size = IniFile.textureEditorSize << 2;
	line[0].x = line[1].x = TextureCursorLocation.x;
	line[0].y = TextureCursorLocation.y - size;
	line[1].y = TextureCursorLocation.y + size;
	Polyline(StitchWindowDC, line, 2);
	line[0].y = line[1].y = TextureCursorLocation.y;
	line[0].x = TextureCursorLocation.x - size;
	line[1].x = TextureCursorLocation.x + size;
	Polyline(StitchWindowDC, line, 2);
}

void setxmov() {
	setMap(TXTMOV);
	TextureCursorLocation.x = SelectTexturePointsOrigin.x = Msg.pt.x - StitchWindowOrigin.x;
	TextureCursorLocation.y = SelectTexturePointsOrigin.y = Msg.pt.y - StitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void ritxrct() {
	POINT	offset;
	RECT	rectangle;
	POINT	line[5];

	offset.x = TextureCursorLocation.x - SelectTexturePointsOrigin.x;
	offset.y = TextureCursorLocation.y - SelectTexturePointsOrigin.y;
	rectangle.bottom = TexturePixelRect.bottom + offset.y;
	rectangle.left = TexturePixelRect.left + offset.x;
	rectangle.right = TexturePixelRect.right + offset.x;
	rectangle.top = TexturePixelRect.top + offset.y;
	line[0].x = line[1].x = line[4].x = rectangle.left;
	line[2].x = line[3].x = rectangle.right;
	line[0].y = line[3].y = line[4].y = rectangle.top;
	line[1].y = line[2].y = rectangle.bottom;
	Polyline(StitchWindowDC, line, 5);
}

void dutxrct(TXTRCT* textureRect) {
	unsigned	iPoint;
	TXPNT*		texturePoint;

	if (SelectedTexturePointsCount) {
		texturePoint = &TempTexturePoints[SelectedTexturePointsList[0]];
		textureRect->left = textureRect->right = texturePoint->line;
		textureRect->top = textureRect->bottom = texturePoint->y;
		for (iPoint = 1; iPoint < SelectedTexturePointsCount; iPoint++) {
			texturePoint = &TempTexturePoints[SelectedTexturePointsList[iPoint]];
			if (texturePoint->y > textureRect->top)
				textureRect->top = texturePoint->y;
			if (texturePoint->y < textureRect->bottom)
				textureRect->bottom = texturePoint->y;
			if (texturePoint->line < textureRect->left)
				textureRect->left = texturePoint->line;
			if (texturePoint->line > textureRect->right)
				textureRect->right = texturePoint->line;
		}
	}
	else // should all coordinate values be set to 0 (or to center of screen)?
		textureRect->left = textureRect->right = textureRect->top = textureRect->bottom;
}

void ed2stch(fPOINT* pt) {
	pt->x -= TextureScreen.xOffset;
	pt->y -= TextureScreen.yOffset;
}

void dutxlin(fPOINT point0, fPOINT point1) {
	double	slope;
	float	deltaX;
	float	start, finish, swap, yOffset;
	int		integerStart, integerFinish;

	ed2stch(&point0);
	ed2stch(&point1);
	deltaX = point1.x - point0.x;
	if (fabs(deltaX) < TINY)
		return;
	slope = (point1.y - point0.y) / deltaX;
	start = point0.x;
	finish = point1.x;
	if (start > finish) {
		swap = start;
		start = finish;
		finish = swap;
	}
	integerStart = ceil(start / TextureScreen.spacing);
	integerFinish = floor(finish / TextureScreen.spacing);
	if (integerStart < 1)
		integerStart = 1;
	if (integerFinish > TextureScreen.lines)
		integerFinish = TextureScreen.lines;
	while (integerStart <= integerFinish) {
		yOffset = slope*(-point0.x + integerStart*TextureScreen.spacing) + point0.y;
		if (yOffset > 0 && yOffset < TextureScreen.areaHeight) {
			TempTexturePoints[TextureScreen.index].line = integerStart;
			TempTexturePoints[TextureScreen.index].y = yOffset;
			TextureScreen.index++;
		}
		integerStart++;
	}
}

void setxclp() {
	POINT		screenOffset;
	fPOINT		editorOffset;
	unsigned	iVertex, iLine, iNextVertex, lineCount;

	deorg(&screenOffset);
	px2ed(screenOffset, &editorOffset);
	if (rstMap(TXHCNTR))
		editorOffset.x = (TextureScreen.lines*TextureScreen.spacing) / 2 + TextureScreen.xOffset - TextureScreen.formCenter.x + TextureScreen.spacing / 2;
	else
		editorOffset.x -= TextureScreen.formCenter.x;
	editorOffset.y -= TextureScreen.formCenter.y;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x += editorOffset.x;
		AngledFormVertices[iVertex].y += editorOffset.y;
	}
	lineCount = AngledForm.vertexCount - 1;
	if (AngledForm.type != FRMLINE)
		lineCount++;
	VertexCount = AngledForm.vertexCount;
	for (iLine = 0; iLine < lineCount; iLine++) {
		iNextVertex = nxt(iLine);
		dutxlin(AngledFormVertices[iLine], AngledFormVertices[iNextVertex]);
	}
}

void stxlin() {
	POINT	offset;
	fPOINT	point0;
	fPOINT	point1;

	rstMap(TXTMOV);
	deorg(&offset);
	px2ed(offset, &point1);
	px2ed(FormLines[0], &point0);
	dutxlin(point0, point1);
	setMap(RESTCH);
}

void ed2txp(POINT offset, TXPNT* textureRecord) {
	fPOINT point;

	px2ed(offset, &point);
	textureRecord->line = (point.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5;
	textureRecord->y = (float)TextureScreen.areaHeight - (float)(offset.y - TextureScreen.top) / TextureScreen.height*TextureScreen.areaHeight;
}

int	hitxlin() {
	unsigned	iPoint;
	short		highestLine;

	highestLine = 0;
	for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
		if (TempTexturePoints[SelectedTexturePointsList[iPoint]].line > highestLine)
			highestLine = TempTexturePoints[SelectedTexturePointsList[iPoint]].line;
	}
	return highestLine;
}

void txtrup() {
	TXPNT		highestTexturePoint;
	TXPNT		lowestTexturePoint;
	TXPNT		textureOffset;
	float		yOffset;
	short		swap;
	unsigned	iPoint;
	int			xCoord, Xmagnitude;
	POINT		offset;
	TXPNT*		texturePoint;

	if (rstMap(TXTMOV)) {
		savtxt();
		deorg(&offset);
		offset.x -= SelectTexturePointsOrigin.x;
		offset.y -= SelectTexturePointsOrigin.y;
		Xmagnitude = abs(offset.x);
		textureOffset.line = Xmagnitude*TextureScreen.editToPixelRatio / TextureScreen.spacing + 0.5;
		if (offset.x < 0)
			textureOffset.line = -textureOffset.line;
		textureOffset.y = (float)-offset.y / TextureScreen.height*TextureScreen.areaHeight;
		yOffset = TextureRect.top + textureOffset.y - TextureScreen.areaHeight;
		if (yOffset > 0)
			textureOffset.y -= yOffset;
		yOffset = TextureRect.bottom + textureOffset.y;
		if (yOffset < 0)
			textureOffset.y -= yOffset;
		xCoord = TextureRect.left + textureOffset.line - 1;
		if (xCoord < 0)
			textureOffset.line -= xCoord;
		xCoord = TextureRect.right + textureOffset.line - TextureScreen.lines;
		if (xCoord > 0)
			textureOffset.line -= xCoord;
		for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
			texturePoint = &TempTexturePoints[SelectedTexturePointsList[iPoint]];
			texturePoint->line += textureOffset.line;
			texturePoint->y += textureOffset.y;
		}
		dutxrct(&TextureRect);
	}
	else {
		if (rstMap(BZUMIN)) {
			deorg(&offset);
			ed2txp(ZoomBoxLine[0], &highestTexturePoint);
			ed2txp(offset, &lowestTexturePoint);
			if (highestTexturePoint.line < lowestTexturePoint.line) {
				swap = highestTexturePoint.line;
				highestTexturePoint.line = lowestTexturePoint.line;
				lowestTexturePoint.line = swap;
			}
			if (highestTexturePoint.y < lowestTexturePoint.y) {
				swap = highestTexturePoint.y;
				highestTexturePoint.y = lowestTexturePoint.y;
				lowestTexturePoint.y = swap;
			}
			SelectedTexturePointsCount = 0;
			for (iPoint = 0; iPoint < TextureScreen.index; iPoint++) {
				if (TempTexturePoints[iPoint].y<highestTexturePoint.y &&
					TempTexturePoints[iPoint].y>lowestTexturePoint.y &&
					TempTexturePoints[iPoint].line <= highestTexturePoint.line &&
					TempTexturePoints[iPoint].line >= lowestTexturePoint.line) {
					SelectedTexturePointsList[SelectedTexturePointsCount] = iPoint;
					SelectedTexturePointsCount++;
				}
			}
			dutxrct(&TextureRect);
		}
	}
	setMap(RESTCH);
}

void angrct(fRECTANGLE* rectangle) {
	unsigned	iVertex;

	rectangle->left = rectangle->right = AngledFormVertices[0].x;
	rectangle->bottom = rectangle->top = AngledFormVertices[0].y;
	for (iVertex = 1; iVertex < AngledForm.vertexCount; iVertex++) {
		if (AngledFormVertices[iVertex].x < rectangle->left)
			rectangle->left = AngledFormVertices[iVertex].x;
		if (AngledFormVertices[iVertex].x > rectangle->right)
			rectangle->right = AngledFormVertices[iVertex].x;
		if (AngledFormVertices[iVertex].y > rectangle->top)
			rectangle->top = AngledFormVertices[iVertex].y;
		if (AngledFormVertices[iVertex].y < rectangle->bottom)
			rectangle->bottom = AngledFormVertices[iVertex].y;
	}
}

void ritxfrm() {
	unsigned	iVertex, vertexCount;
	POINT		offset;

	offset.x = TextureCursorLocation.x - SelectTexturePointsOrigin.x;
	offset.y = TextureCursorLocation.y - SelectTexturePointsOrigin.y;
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		ed2px(AngledFormVertices[iVertex], &FormLines[iVertex]);
		FormLines[iVertex].x += offset.x;
		FormLines[iVertex].y += offset.y;
	}
	FormLines[iVertex].x = FormLines[0].x;
	FormLines[iVertex].y = FormLines[0].y;
	vertexCount = AngledForm.vertexCount;
	if (AngledForm.type != FRMLINE)
		vertexCount++;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, vertexCount);
}

void setxfrm() {
	unsigned	iVertex;
	fRECTANGLE	angleRect;
	float		height;
	double		ratio;

	angrct(&angleRect);
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x -= angleRect.left;
		AngledFormVertices[iVertex].y -= angleRect.bottom;
	}
	angrct(&angleRect);
	height = angleRect.top - angleRect.bottom;
	if (height > TextureScreen.areaHeight) {
		ratio = TextureScreen.areaHeight / height*0.95;
		for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex].x *= ratio;
			AngledFormVertices[iVertex].y *= ratio;
		}
		angrct(&angleRect);
	}
	TextureScreen.formCenter.x = midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = midl(angleRect.top, angleRect.bottom);
	ed2px(TextureScreen.formCenter, &SelectTexturePointsOrigin);
}

void txtclp() {
	ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
	ClipMemory = GetClipboardData(ThrEdClip);
	if (ClipMemory) {
		ClipFormHeader = (FORMCLIP*)GlobalLock(ClipMemory);
		if (ClipFormHeader) {
			if (ClipFormHeader->clipType == CLP_FRM) {
				SelectedForm = &ClipFormHeader->form;
				frmcpy(&AngledForm, SelectedForm);
				MoveMemory(&AngledFormVertices, &SelectedForm[1], sizeof(fPOINT)*SelectedForm->vertexCount);
				AngledForm.vertices = AngledFormVertices;
				rstMap(TXTLIN);
				setMap(TXTCLP);
				setMap(TXTMOV);
				setxfrm();
				TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
				TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			}
			GlobalUnlock(ClipMemory);
		}
	}
	setMap(RESTCH);
	rstMap(WASWROT);
}

void dutxtlin() {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, 2);
}

void txtrmov() {
	if (chkMap(TXTLIN)) {
		dutxtlin();
		deorg(&FormLines[1]);
		dutxtlin();
		return;
	}
	if (chkMap(TXTCLP)) {
		if (setMap(WASWROT))
			ritxfrm();
		TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
		TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
		ritxfrm();
	}
	else {
		if (SelectedTexturePointsCount) {
			ritxrct();
			TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
			TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			ritxrct();
		}
	}
}

void txtlin() {
	deorg(FormLines);
	deorg(&FormLines[1]);
	rstMap(TXTCLP);
	setMap(TXTLIN);
	setMap(TXTMOV);
}

void butsid(unsigned windowId) {
	RECT buttonRect;

	chktxnum();
	TextureWindowId = windowId;
	GetWindowRect(ButtonWin[windowId], &buttonRect);
	SideWindowButton = CreateWindow(
		"STATIC",
		0,
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		buttonRect.left + ButtonWidthX3 - StitchWindowOrigin.x,
		buttonRect.top - StitchWindowOrigin.y,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThrEdInstance,
		NULL);
}

int txcmp(const void *arg1, const void *arg2) {
	TXPNT	texturePoint0 = *(TXPNT*)arg1;
	TXPNT	texturePoint1 = *(TXPNT*)arg2;

	if (texturePoint0.line == texturePoint1.line) {
		if (texturePoint0.y == texturePoint1.y)
			return 0;
		else {
			if (texturePoint0.y > texturePoint1.y)
				return 1;
			else
				return -1;
		}
	}
	else
		return texturePoint0.line - texturePoint1.line;
	//	return 0;
}

void txpar() {
	SelectedForm->type = FRMFPOLY;
	SelectedForm->fillInfo.texture.lines = TextureScreen.lines;
	SelectedForm->fillInfo.texture.height = TextureScreen.areaHeight;
	SelectedForm->fillSpacing = TextureScreen.spacing;
	SelectedForm->lengthOrCount.stitchLength = IniFile.userStitchLength;
	SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
	SelectedForm->minFillStitchLen = IniFile.minStitchLength;
	SelectedForm->fillColor = ActiveColor;
	refilfn();
}

void txvrt() {
	if (TextureScreen.index) {
		if (chkMap(FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXVRTF;
			txpar();
		}
	}
}

void txhor() {
	if (TextureScreen.index) {
		if (chkMap(FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXHORF;
			txpar();
		}
	}
}

void txang() {
	if (TextureScreen.index) {
		if (chkMap(FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXANGF;
			SelectedForm->angleOrClipData.angle = (float)IniFile.fillAngle;
			txpar();
		}
	}
}

void deltx() {
	TXPNT		*textureBuffer;
	unsigned	iBuffer;
	unsigned	iForm;

	if (TextureIndex && istx(ClosestFormToCursor) && SelectedForm->fillInfo.texture.count) {
		textureBuffer = new TXPNT[TextureIndex]();
		iBuffer = 0;
		for (iForm = 0; iForm < ClosestFormToCursor; iForm++) {
			if (istx(iForm)) {
				MoveMemory(&textureBuffer[iBuffer], &TexturePointsBuffer[FormList[iForm].fillInfo.texture.index], FormList[iForm].fillInfo.texture.count * sizeof(TXPNT));
				FormList[iForm].fillInfo.texture.index = iBuffer;
				iBuffer += FormList[iForm].fillInfo.texture.count;
			}
		}
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			if (istx(iForm)) {
				MoveMemory(&textureBuffer[iBuffer], &TexturePointsBuffer[FormList[iForm].fillInfo.texture.index], FormList[iForm].fillInfo.texture.count * sizeof(TXPNT));
				FormList[iForm].fillInfo.texture.index = iBuffer;
				iBuffer += FormList[iForm].fillInfo.texture.count;
			}
		}
		TextureIndex = iBuffer;
		MoveMemory(&TexturePointsBuffer[0], &textureBuffer[0], iBuffer * sizeof(TXPNT));
		delete[] textureBuffer;
	}
}

void nutx() {
	int			iForm, iPoint;
	FRMHED*		formHeader;

	qsort(TempTexturePoints, TextureScreen.index, sizeof(TXPNT), txcmp);
	iPoint = 0;
	if (FormIndex) {
		if (istx(ClosestFormToCursor)) {
			iPoint = FormList[ClosestFormToCursor].fillInfo.texture.index;
			deltx();
		}
		else {
			for (iForm = ClosestFormToCursor - 1; iForm >= 0; iForm--) {
				if (istx(iForm)) {
					formHeader = &FormList[iForm];
					iPoint = formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count;
					goto nutskp;
				}
			}
nutskp:;
		}
	}
	txspac(iPoint, TextureScreen.index);
	MoveMemory(&TexturePointsBuffer[iPoint], &TempTexturePoints, TextureScreen.index * sizeof(TXPNT));
	SelectedForm->fillInfo.texture.index = iPoint;
	SelectedForm->fillInfo.texture.count = TextureScreen.index;
}

//Ensure all lines in the texture have at least 1 point
void altx() {
	unsigned	iLine, iPoint;
	float		halfHeight;

	if (chkMap(FORMSEL)) {
		halfHeight = TextureScreen.areaHeight / 2;
		clRmap((TextureScreen.lines >> 5) + 1);
		for (iPoint = 0; iPoint < TextureScreen.index; iPoint++)
			setr(TempTexturePoints[iPoint].line);
		for (iLine = 1; iLine <= TextureScreen.lines; iLine++) {
			if (!chkr(iLine)) {
				TempTexturePoints[TextureScreen.index].line = iLine;
				TempTexturePoints[TextureScreen.index].y = halfHeight;
				TextureScreen.index++;
			}
		}
	}
}

enum {
	VRTYP,
	HORTYP,
	ANGTYP,
};

void dutxfn(unsigned textureType) {
	if (chkMap(FORMSEL)) {
		altx();
		delmclp(ClosestFormToCursor);
		delsac(ClosestFormToCursor);
		deltx();
		savtxt();
		nutx();
		if (chku(SQRFIL))
			SelectedForm->extendedAttribute |= AT_SQR;
		else
			SelectedForm->extendedAttribute &= (~AT_SQR);
		switch (textureType) {
			case VRTYP:

				txvrt();
				break;

			case HORTYP:

				txhor();
				break;

			case ANGTYP:

				txang();
				break;
		}
	}
	txof();
	rstxt();
}

void txsrt() {
	qsort(TempTexturePoints, TextureScreen.index, sizeof(TXPNT), txcmp);
}

void dutxmir() {
	int centerLine, iPoint, iMirrorPoint;

	savtxt();
	txsrt();
	centerLine = (TextureScreen.lines + 1) >> 1;
	iPoint = TextureScreen.index - 1;
	while (TempTexturePoints[iPoint].line > centerLine && iPoint >= 0)
		iPoint--;
	iMirrorPoint = iPoint + 1;
	if (TextureScreen.lines & 1) {
		while (iPoint >= 0) {
			if (TempTexturePoints[iPoint].line == centerLine) {
				iPoint--;
			}
			else { break; }
		}
	}
	while (iPoint >= 0) {
		TempTexturePoints[iMirrorPoint].line = TextureScreen.lines - TempTexturePoints[iPoint].line + 1;
		TempTexturePoints[iMirrorPoint].y = TempTexturePoints[iPoint].y;
		iMirrorPoint++;
		iPoint--;
	}
	TextureScreen.index = iMirrorPoint;
	setMap(RESTCH);
}

BOOL chkbut() {
	if (Msg.hwnd == ButtonWin[HTXCLR]) {
		txdelal();
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXHI]) {
		butsid(HTXHI);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXWID]) {
		butsid(HTXWID);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXSPAC]) {
		butsid(HTXSPAC);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXVRT]) {
		dutxfn(VRTYP);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXHOR]) {
		dutxfn(HORTYP);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXANG]) {
		dutxfn(ANGTYP);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXMIR]) {
		dutxmir();
		return 1;
	}
	return 0;
}

void txtlbut() {
	POINT	point;

	fvars(ClosestFormToCursor);
	if (chkbut())
		return;
	if (rstMap(TXTCLP)) {
		savtxt();
		setxclp();
		return;
	}
	if (rstMap(TXTLIN)) {
		savtxt();
		stxlin();
		return;
	}
	if (SelectedTexturePointsCount) {
		deorg(&point);
		if (point.x > TexturePixelRect.left &&
			point.x<TexturePixelRect.right &&
			point.y>TexturePixelRect.top &&
			point.y < TexturePixelRect.bottom) {
			setxmov();
			ritxrct();
			return;
		}
	}
	if (txtclos(&SelectedTexturePointsList[0])) {
		SelectedTexturePointsCount = 1;
		setxmov();
		dutxrct(&TextureRect);
		setMap(RESTCH);
		return;
	}
	SelectedTexturePointsCount = 0;
	setMap(BZUMIN);
	rstMap(BZUM);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y = ZoomBoxLine[0].y - 1;
}

void redtbak() {
	TXHST*		textureHistoryItem;

	//	sprintf_s(MsgBuffer, sizeof(MsgBuffer),"%d\n",TextureHistoryIndex);
	//	OutputDebugString(MsgBuffer);
	textureHistoryItem = &TextureHistory[TextureHistoryIndex];
	TextureScreen.areaHeight = textureHistoryItem->height;
	TextureScreen.width = textureHistoryItem->width;
	TextureScreen.spacing = textureHistoryItem->spacing;
	TextureScreen.index = textureHistoryItem->count;
	if (textureHistoryItem->texturePoint) {
		MoveMemory (TempTexturePoints, textureHistoryItem->texturePoint, textureHistoryItem->count * sizeof (TXPNT));
	}
	setMap(RESTCH);
}

void txbak() {
	unsigned	iHistory;

	if (chkMap(WASTXBAK)) {
		SelectedTexturePointsCount = 0;
		for (iHistory = 0; iHistory < 16; iHistory++) {
			if (TextureHistory[TextureHistoryIndex].width)
				goto txbak1;
			txrbak();
		}
		return;
txbak1:;
		redtbak();
		txrbak();
	}
}

void nxbak() {
	unsigned	iHistory;

	if (chkMap(WASTXBAK)) {
		for (iHistory = 0; iHistory < 16; iHistory++) {
			txrfor();
			if (TextureHistory[TextureHistoryIndex].width)
				goto nxbak1;
		}
		return;
nxbak1:;
		redtbak();
	}
}

void txtdel() {
	unsigned	iPoint, iSourcePoint, iOutputPoint;
	unsigned	iClosestPoint;

	if (SelectedTexturePointsCount) {
		savtxt();
		clRmap(MAXITEMS);
		for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++)
			setr(SelectedTexturePointsList[iPoint]);
		iOutputPoint = 0;
		for (iSourcePoint = 0; iSourcePoint < TextureScreen.index; iSourcePoint++) {
			if (!chkr(iSourcePoint)) {
				TempTexturePoints[iOutputPoint].line = TempTexturePoints[iSourcePoint].line;
				TempTexturePoints[iOutputPoint].y = TempTexturePoints[iSourcePoint].y;
				iOutputPoint++;
			}
		}
		SelectedTexturePointsCount = 0;
		TextureScreen.index = iOutputPoint;
		setMap(RESTCH);
		return;
	}
	if (TextureScreen.index && txtclos(&iClosestPoint)) {
		MoveMemory(&TempTexturePoints[iClosestPoint], &TempTexturePoints[iClosestPoint + 1], (TextureScreen.index - iClosestPoint) * sizeof(TXPNT));
		TextureScreen.index--;
		setMap(RESTCH);
	}
}

void txdelal() {
	chktxnum();
	savtxt();
	TextureScreen.index = 0;
	rstxt();
	setMap(RESTCH);
}

void chktxnum() {
	float	value;

	value = atof(TextureInputBuffer);
	if (value) {
		value *= PFGRAN;
		switch (TextureWindowId) {
			case HTXHI:

				savtxt();
				TextureScreen.areaHeight = value;
				IniFile.textureHeight = value;
				setMap(CHKTX);
				break;

			case HTXWID:

				savtxt();
				TextureScreen.width = value;
				IniFile.textureWidth = value;
				setMap(CHKTX);
				break;

			case HTXSPAC:

				savtxt();
				TextureScreen.spacing = value;
				IniFile.textureSpacing = value;
				TextureScreen.width = value*TextureScreen.lines + value / 2;
				setMap(CHKTX);
				break;
		}
	}
	TextureInputIndex = 0;
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	setMap(RESTCH);
}

void txcntrv() {
	if (rstMap(TXTCLP)) {
		setMap(TXHCNTR);
		savtxt();
		setxclp();
		setMap(RESTCH);
	}
}

void txof() {
	butxt(HBOXSEL, StringTable[STR_BOXSEL]);
	redraw(ButtonWin[HHID]);
	if (chkMap(UPTO))
		butxt(HUPTO, StringTable[STR_UPON]);
	else
		butxt(HUPTO, StringTable[STR_UPOF]);
	SetWindowText(ButtonWin[HTXSPAC], "");
	savtxt();
	zumhom();
	if (SelectedTexturePointsList) {
		delete[] SelectedTexturePointsList; // Allocated in setshft or dutxtfil
		SelectedTexturePointsList = nullptr;
	}
	rstMap(TXTRED);
}

BOOL istxclp() {
	if (chkMap(TXTMOV) && chkMap(TXTCLP))
		return 1;
	else
		return 0;

}

void txsiz(double ratio) {
	unsigned	iVertex;
	fRECTANGLE	angleRect;

	ritxfrm();
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x *= ratio;
		AngledFormVertices[iVertex].y *= ratio;
	}
	angrct(&angleRect);
	TextureScreen.formCenter.x = midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = midl(angleRect.top, angleRect.bottom);
	ed2px(TextureScreen.formCenter, &SelectTexturePointsOrigin);
	ritxfrm();
}

void txshrnk() {
	txsiz(TXTRAT);
}

void txgro() {
	txsiz(1 / TXTRAT);
}

BOOL txdig(unsigned keyCode, TCHAR* character) {
	if (isdigit(keyCode)) {
		*character = (TCHAR)keyCode;
		return 1;
	}
	if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
		*character = (TCHAR)keyCode - VK_NUMPAD0 + 0x30;
		return 1;
	}
	if (keyCode == 0xbe || keyCode == 0x6e) {
		*character = '.';
		return 1;
	}
	return 0;
}

void txnudg(int deltaX, float deltaY) {
	float		screenDeltaY;
	float		yCoord;
	unsigned	iPoint;
	int			textureLine;

	if (SelectedTexturePointsCount) {
		if (deltaY) {
			screenDeltaY = deltaY*TextureScreen.editToPixelRatio;
			for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
				yCoord = TempTexturePoints[SelectedTexturePointsList[iPoint]].y + screenDeltaY;
				if (yCoord < 0)
					return;
				if (yCoord > TextureScreen.areaHeight)
					return;
			}
			for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++)
				TempTexturePoints[SelectedTexturePointsList[iPoint]].y += screenDeltaY;
		}
		else {
			for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
				textureLine = TempTexturePoints[SelectedTexturePointsList[iPoint]].line + deltaX;
				if (textureLine < 1)
					return;
				if (textureLine > TextureScreen.lines)
					return;
			}
			for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++)
				TempTexturePoints[SelectedTexturePointsList[iPoint]].line += deltaX;
		}
	}
	dutxrct(&TextureRect);
	setMap(RESTCH);
}

void txsnap() {
	unsigned	iPoint;
	int			yStep;
	float		halfGrid;
	TXPNT*		texturePoint;

	if (TextureScreen.index) {
		savtxt();
		halfGrid = IniFile.gridSize / 2;
		if (SelectedTexturePointsCount) {
			for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
				texturePoint = &TempTexturePoints[SelectedTexturePointsList[iPoint]];
				yStep = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y = yStep*IniFile.gridSize;
			}
		}
		else {
			for (iPoint = 0; iPoint < TextureScreen.index; iPoint++) {
				texturePoint = &TempTexturePoints[iPoint];
				yStep = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y = yStep*IniFile.gridSize;
			}
		}
		setMap(RESTCH);
	}
}

void txtkey(unsigned keyCode) {
	TCHAR	character;

	if (SideWindowButton) {
		switch (keyCode) {
			case VK_RETURN:

				chktxnum();
				return;

			case VK_ESCAPE:

				txof();

			case 'Q':

				rstxt();
				setMap(RESTCH);
				break;

			case 8:	//backspace

				if (TextureInputIndex)
					TextureInputIndex--;
				goto txskp;
		}
		if (txdig(keyCode, &character)) {
			TextureInputBuffer[TextureInputIndex] = character;
			TextureInputIndex++;
		}
txskp:;
		TextureInputBuffer[TextureInputIndex] = 0;
		SetWindowText(SideWindowButton, TextureInputBuffer);
		return;
	}
	switch (keyCode) {
		case VK_ESCAPE:

			txof();

		case 'Q':

			rstxt();
			setMap(RESTCH);
			break;

		case 0xdb:	//[

			txshrnk();
			break;

		case 0xdd:	//]

			txgro();
			break;

		case 192: //`

			tst();
			break;

		case 'R':

			dutxfn(VRTYP);
			break;

		case 'A':

			dutxfn(ANGTYP);
			break;

		case 'H':

			dutxfn(HORTYP);
			break;

		case 'E':

			txtlin();
			break;

		case 'Z':
		case 'B':

			if (!setMap(LASTXBAK)) {
				savtxt();
				txrbak();
			}
			else {
				if (rstMap(TXBDIR))
					txrbak();
			}
			txbak();
			return;

		case 'V':

			if (OpenClipboard(ThrEdWindow))
				txtclp();
			break;

		case 'N':

			setMap(LASTXBAK);
			if (!setMap(TXBDIR))
				txrfor();
			nxbak();
			return;

		case 'D':
		case VK_DELETE:

			if (GetKeyState(VK_SHIFT)&GetKeyState(VK_CONTROL) & 0X8000)
				txdelal();
			else
				txtdel();
			break;

		case 0xbd:		//-

			txcntrv();
			break;

		case VK_LEFT:

			txnudg(-1, 0);
			break;

		case VK_RIGHT:

			txnudg(1, 0);
			break;

		case VK_UP:

			txnudg(0, IniFile.cursorNudgeStep);
			break;

		case VK_DOWN:

			txnudg(0, -IniFile.cursorNudgeStep);
			break;

		case 'S':

			txsnap();
			break;
	}
	rstMap(LASTXBAK);
}

void setxt() {
	TXPNT*	currentFormTexture;
	int		iTexturePoint, count, iSegment;

	savtxt();
	SelectedForm->wordParam = 0;
	setMap(TXFIL);
	ClipRectSize.cx = SelectedForm->fillSpacing;
	ClipRectSize.cy = SelectedForm->fillInfo.texture.height;
	TextureSegments = new RNGCNT[SelectedForm->fillInfo.texture.lines]();
	currentFormTexture = &TexturePointsBuffer[SelectedForm->fillInfo.texture.index];
	count = SelectedForm->fillInfo.texture.count;
	if (count) {
		for (iTexturePoint = count - 1; iTexturePoint >= 0; iTexturePoint--) {
			if (currentFormTexture[iTexturePoint].line) {
				iSegment = currentFormTexture[iTexturePoint].line - 1;
				TextureSegments[iSegment].line = iTexturePoint;
				TextureSegments[iSegment].stitchCount++;
			}
		}
	}
}

void rtrtx() {
	TXPNT*	currentFormTexture;

	fvars(ClosestFormToCursor);
	currentFormTexture = &TexturePointsBuffer[SelectedForm->fillInfo.texture.index];
	TextureScreen.index = SelectedForm->fillInfo.texture.count;
	MoveMemory(TempTexturePoints, currentFormTexture, sizeof(TXPNT)*TextureScreen.index);
	TextureScreen.areaHeight = SelectedForm->fillInfo.texture.height;
	TextureScreen.spacing = SelectedForm->fillSpacing;
	TextureScreen.lines = SelectedForm->fillInfo.texture.lines;
	TextureScreen.width = TextureScreen.lines*TextureScreen.spacing + TextureScreen.spacing / 2;
	savtxt();
}

void rtrclp() {
	if (chkMap(FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (istx(ClosestFormToCursor))
			rtrtx();
		else
			rtrclpfn();
	}
}

void setstxt(unsigned stringIndex, float value) {
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), "%.2f", (float)value / PFGRAN);
	SetWindowText(GetDlgItem(DesignSizeDialog, stringIndex), HelpBuffer);
}

float getstxt(unsigned stringIndex) {
	GetWindowText(GetDlgItem(DesignSizeDialog, stringIndex), HelpBuffer, HBUFSIZ);
	return atof(HelpBuffer)*PFGRAN;
}

BOOL chkasp(fPOINT* point) {
	point->x = getstxt(IDC_DESWID);
	point->y = getstxt(IDC_DESHI);
	if (point->y / point->x == DesignAspectRatio)
		return 1;
	else
		return 0;
}

BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	fPOINT	designSize;

	DesignSizeDialog = hwndlg;
	switch (umsg) {
		case WM_INITDIALOG:

			SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
			setstxt(IDC_DESWID, DesignSize.x);
			setstxt(IDC_DESHI, DesignSize.y);
			DesignAspectRatio = DesignSize.y / DesignSize.x;
			CheckDlgButton(hwndlg, IDC_REFILF, chku(CHREF));
			break;

		case WM_COMMAND:

			switch (LOWORD(wparam)) {
				case IDCANCEL:

					EndDialog(hwndlg, 0);
					return TRUE;

				case IDOK:

					DesignSize.x = getstxt(IDC_DESWID);
					DesignSize.y = getstxt(IDC_DESHI);
					if (IsDlgButtonChecked(hwndlg, IDC_REFILF))
						setu(CHREF);
					else
						rstu(CHREF);
					EndDialog(hwndlg, 1);
					return TRUE;

				case IDC_DESWID:

					if ((wparam >> 16) == EN_CHANGE)
						rstMap(DESCHG);
					break;

				case IDC_DESHI:

					if ((wparam >> 16) == EN_CHANGE)
						setMap(DESCHG);
					break;

				case IDC_DUASP:

					if (!chkasp(&designSize)) {
						if (chkMap(DESCHG))
							setstxt(IDC_DESWID, (float)designSize.y / DesignAspectRatio);
						else
							setstxt(IDC_DESHI, (float)designSize.x*DesignAspectRatio);
					}
					break;
			}
	}
	return 0;
}

void sadj(fPOINTATTR* stitch) {
	stitch->x = (stitch->x - DesignSizeRect.left)*DesignSizeRatio.x + DesignSizeRect.left;
	stitch->y = (stitch->y - DesignSizeRect.bottom)*DesignSizeRatio.y + DesignSizeRect.bottom;
}

void sadj(fPOINT* point) {
	point->x = (point->x - DesignSizeRect.left)*DesignSizeRatio.x + DesignSizeRect.left;
	point->y = (point->y - DesignSizeRect.bottom)*DesignSizeRatio.y + DesignSizeRect.bottom;
}

void nudfn() {
	unsigned	iStitch, iVertex;
	fPOINT		newSize;

	newSize.x = DesignSizeRect.right - DesignSizeRect.left;
	newSize.y = DesignSizeRect.top - DesignSizeRect.bottom;
	DesignSizeRatio.x = DesignSize.x / newSize.x;
	DesignSizeRatio.y = DesignSize.y / newSize.y;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		sadj(&StitchBuffer[iStitch]);
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
		sadj(&FormVertices[iVertex]);
	frmout(ClosestFormToCursor);
}

void nudsiz() {
	int			flag;
	unsigned	iForm;

	savdo();
	flag = 0;
	if (PCSHeader.stitchCount) {
		stchrct(&DesignSizeRect);
		flag = 1;
	}
	else {
		if (FormIndex) {
			frmrct(&DesignSizeRect);
			flag = 2;
		}
	}
	if (flag) {
		DesignSize.x = DesignSizeRect.right - DesignSizeRect.left;
		DesignSize.y = DesignSizeRect.top - DesignSizeRect.bottom;
		if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, (DLGPROC)setsprc)) {
			flag = 0;
			if (DesignSize.x > IniFile.hoopSizeX) {
				IniFile.hoopSizeX = DesignSize.x*1.05;
				UnzoomedRect.x = IniFile.hoopSizeX;
				flag = 1;
			}
			if (DesignSize.y > IniFile.hoopSizeY) {
				IniFile.hoopSizeY = DesignSize.y*1.05;
				UnzoomedRect.y = IniFile.hoopSizeY;
				flag = 1;
			}
			nudfn();
			if (chku(CHREF))
				refilal();
			if (flag) {
				movStch();
				zumhom();
				hsizmsg();
			}
			centir();
			for (iForm = 0; iForm < FormIndex; iForm++)
				frmout(iForm);
		}
	}
}

void dushft() {
	//	setMap(BOXSLCT);
	setMap(BZUMIN);
	//	setMap(NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y = ZoomBoxLine[0].y - 1;
	px2stch();
	ZoomBoxOrigin.x = SelectedPoint.x;
	ZoomBoxOrigin.y = SelectedPoint.y;
}

extern void unbBox();
extern void bBox();
extern void pxCor2stch(POINT pnt);

void mvshft() {
	if (chkMap(BOXZUM) && setMap(VCAPT))
		SetCapture(ThrEdWindow);
	if (chkMap(BZUMIN)) {
		if (Msg.wParam&MK_LBUTTON) {
			if (setMap(VCAPT))
				SetCapture(ThrEdWindow);
			unbBox();
			ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
			setMap(BZUM);
			bBox();
		}
	}
}

BOOL inrct(fRECTANGLE rectangle, fPOINTATTR stitch) {
	if (stitch.x < rectangle.left)
		return 0;
	if (stitch.x > rectangle.right)
		return 0;
	if (stitch.y < rectangle.bottom)
		return 0;
	if (stitch.y > rectangle.top)
		return 0;
	return 1;
}

void setshft() {
	fRECTANGLE	selectionRect;
	unsigned	iStitch, line;

	savtxt();
	SelectedTexturePointsList = new unsigned[MAXITEMS];
	unbBox();
	rstMap(BZUMIN);
	pxCor2stch(ZoomBoxLine[0]);
	selectionRect.top = SelectedPoint.y;
	selectionRect.left = SelectedPoint.x;
	pxCor2stch(ZoomBoxLine[2]);
	selectionRect.bottom = SelectedPoint.y;
	selectionRect.right = SelectedPoint.x;
	rstMap(TXIN);
	TextureScreen.index = 0;
	line = 1;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (inrct(selectionRect, StitchBuffer[iStitch])) {
			setMap(TXIN);
			TempTexturePoints[TextureScreen.index].line = line;
			TempTexturePoints[TextureScreen.index].y = StitchBuffer[iStitch].y - selectionRect.bottom;
			TextureScreen.index++;
		}
		else {
			if (rstMap(TXIN))
				line++;
		}
	}
	if (TextureScreen.index) { line = TempTexturePoints[TextureScreen.index - 1].line; }
	TextureScreen.spacing = (selectionRect.right - selectionRect.left) / line;
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width = TextureScreen.spacing*line + TextureScreen.spacing / 2;
	setMap(TXTRED);
	setMap(RESTCH);
}

void setclpspac() {
	msgflt(IDS_CLPSPAC, IniFile.clipOffset / PFGRAN);
	setMap(NUMIN);
	setMap(SCLPSPAC);
	numWnd();
}

BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam) {
	UNREFERENCED_PARAMETER(lParam);

	DestroyWindow(hwnd);
	return 1;
}

void clrstch() {
	while (EnumChildWindows(MainStitchWin, enumch, 0));
}

BOOL txnam(char *name, int sizeName) {
	char*	lastCharacter;

	strcpy_s(name, sizeName, __argv[0]);
	lastCharacter = strrchr(name, '\\');
	if (lastCharacter) {
		lastCharacter++;
		strcpy_s(lastCharacter, sizeName - (lastCharacter - name), "thred.txr");
		return 1;
	}
	else
		return 0;
}

void txdun() {
	char			name[_MAX_PATH];
	HANDLE			handle;
	unsigned long	bytesWritten;
	int				iHistory;
	//char* signature="txh";

	if (TextureHistory[0].count) {
		if (txnam(name, sizeof(name))) {
			handle = CreateFile(name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (handle != INVALID_HANDLE_VALUE) {
				WriteFile(handle, (char*)"txh", 4, &bytesWritten, 0);
				WriteFile(handle, (int*)&TextureHistoryIndex, 4, &bytesWritten, 0);
				WriteFile(handle, (TXHST*)&TextureHistory, sizeof(TXHST) * 16, &bytesWritten, 0);
				for (iHistory = 0; iHistory < 16; iHistory++) {
					if (TextureHistory[iHistory].count)
						WriteFile(handle, (TXPNT*)TextureHistory[iHistory].texturePoint, TextureHistory[iHistory].count * sizeof(TXPNT), &bytesWritten, 0);
				}
			}
			CloseHandle(handle);
		}
	}
}

void redtx() {
	char			name[_MAX_PATH];
	HANDLE			handle;
	DWORD			bytesRead, historyBytesRead;
	unsigned int	ind,ine;
	char			sig[4] = { 0 };

	TextureHistoryIndex = 15;
	ZeroMemory(&TextureHistory, sizeof(TXHST) * 16);
	if (txnam(name, sizeof(name))) {
		handle = CreateFile(name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (handle != INVALID_HANDLE_VALUE) {
			if (ReadFile(handle, (char*)&sig, 4, &bytesRead, 0)) {
				if (!strcmp(sig, "txh")) {
					if (ReadFile(handle, (int*)&TextureHistoryIndex, 4, &bytesRead, 0)) {
						if (ReadFile(handle, (TXHST*)&TextureHistory, sizeof(TXHST) * 16, &historyBytesRead, 0)) {
							// texturePoint should be a null pointer at this point as no memory has been allocated, but it is not
							// because the old pointer value is read in from the file, so zero it out here as it is easier than 
							// writing a zero to the file
							for (ind = 0; ind < 16; ind++) {
								TextureHistory[ind].texturePoint = 0;
							}

							for (ind = 0; ind < (historyBytesRead / sizeof(TXHST)); ind++) {
								if (TextureHistory[ind].count) {
									TextureHistory[ind].texturePoint = new TXPNT[TextureHistory[ind].count];
									if (!ReadFile((TXPNT*)handle, TextureHistory[ind].texturePoint, sizeof(TXPNT)*TextureHistory[ind].count, &bytesRead, 0)) {
										for (ine = 0; ine < TextureHistory[ind].count; ine++) {
											TextureHistory[ine].texturePoint->line = 0;
											TextureHistory[ine].texturePoint->y = 0;
										}
									}
								}
							}
						}
					}
					setMap(WASTXBAK);
				}
			}
		}
		CloseHandle(handle);
	}
	redtbak();
}

void setangf(double angle) {
	unsigned	iVertex;
	double		savedAngle;

	savedAngle = RotationAngle;
	RotationAngle = angle;
	MoveMemory(&AngledForm, SelectedForm, sizeof(FRMHED));
	MoveMemory(&AngledFormVertices, CurrentFormVertices, sizeof(fPOINT)*VertexCount);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	if (RotationAngle) {
		for (iVertex = 0; iVertex < VertexCount; iVertex++)
			rotflt(&AngledFormVertices[iVertex]);
	}
	SelectedForm = &AngledForm;
	CurrentFormVertices = AngledFormVertices;
	RotationAngle = savedAngle;
}

void chgwrn() {
	toglu(WRNOF);
	wrnmen();
	setMap(DUMEN);
}

void chgchk(int code) {
	IniFile.dataCheck = code;
	chkmen();
	setMap(DUMEN);
}

void lodchk() {
	unsigned	iForm, iStitch, attribute;

	delinf();
	for (iForm = 0; iForm < FormIndex; iForm++) {

		SelectedForm = &FormList[iForm];
		if (!SelectedForm->type)
			SelectedForm->type = FRMFPOLY;
		else {

			if (SelectedForm->type == FRMLINE) {

				if (SelectedForm->fillType != CONTF) {

					SelectedForm->fillType = 0;
					SelectedForm->lengthOrCount.clipCount = 0;
				}
			}
		}
		frmout(iForm);
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	}
	clRmap((MAXFORMS >> 5) + 1);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		attribute = StitchBuffer[iStitch].attribute;
		if ((attribute&TYPMSK) == TYPFRM)
			setr((attribute&FRMSK) >> FRMSHFT);

	}
	for (iForm = 0; iForm < FormIndex; iForm++) {

		if (!chkr(iForm))
			FormList[iForm].fillType = 0;
	}
	clRmap((MAXFORMS >> 5) + 1);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {

		attribute = StitchBuffer[iStitch].attribute;
		if (attribute&TYPBRD)
			setr((attribute&FRMSK) >> FRMSHFT);
	}
	for (iForm = 0; iForm < FormIndex; iForm++) {

		if (!chkr(iForm))
			FormList[iForm].edgeType = 0;
	}
}

void chkclp(FRMHED* formHeader, BADCNTS* badData) {
	if (badData->clip == formHeader->angleOrClipData.clip - ClipPoints)
		badData->clip += formHeader->lengthOrCount.clipCount;
	else
		badData->attribute |= BADCLP;
}

void chkeclp(FRMHED* formHeader, BADCNTS* badData) {
	if (badData->clip == formHeader->borderClipData - ClipPoints)
		badData->clip += formHeader->clipEntries;
	else
		badData->attribute |= BADCLP;
}

unsigned frmchkfn() {
	unsigned	iForm;
	FRMHED*		formHeader;
	BADCNTS		badData;

	FillMemory(&badData, sizeof(BADCNTS), 0);
	if (FormIndex) {
		for (iForm = 0; iForm < FormIndex; iForm++) {
			formHeader = &FormList[iForm];
			if (!(badData.attribute&BADFLT)) {
				if (!formHeader->vertexCount)
					badData.attribute |= BADFLT;
				if (badData.flt == formHeader->vertices - FormVertices)
					badData.flt += formHeader->vertexCount;
				else
					badData.attribute |= BADFLT;
			}
			if (!(badData.attribute&BADCLP)) {
				if (isclp(iForm))
					chkclp(formHeader, &badData);
				if (iseclp(iForm))
					chkeclp(formHeader, &badData);
			}
			if (formHeader->type == SAT && formHeader->satinGuideCount) {
				if (!(badData.attribute&BADSAT)) {
					if (badData.guideCount == formHeader->satinOrAngle.guide - SatinConnects)
						badData.guideCount += formHeader->satinGuideCount;
					else
						badData.attribute |= BADSAT;
				}
			}
			if (istx(iForm)) {
				if (!(badData.attribute&BADTX)) {
					if (badData.tx == formHeader->fillInfo.texture.index)
						badData.tx += formHeader->fillInfo.texture.count;
					else
						badData.attribute |= BADTX;
				}
			}
			if (badData.attribute == (BADFLT | BADCLP | BADSAT | BADTX))
				break;
		}
		if (badData.flt != (int)FormVertexIndex)
			badData.attribute |= BADFLT;
		if (badData.clip != (int)ClipPointIndex)
			badData.attribute |= BADCLP;
		if (badData.guideCount != (int)SatinConnectIndex)
			badData.attribute |= BADSAT;
		if (badData.tx != TextureIndex)
			badData.attribute |= BADTX;
	}
	return badData.attribute;
}

void frmchkx() {
	unsigned	code;

	if (IniFile.dataCheck) {
		code = frmchkfn();
		switch (IniFile.dataCheck) {
			case 1:

				if (code)
					datmsg(code);
				break;

			case 2:

				if (code)
					repar();
				break;

			case 3:

				if (code) {
					repar();
					tabmsg(IDS_DATREP);
				}
		}
	}
}

void bcup(unsigned find, BADCNTS* badData) {
	FRMHED*	formHeader;

	formHeader = &FormList[find];
	if (isclp(find))
		badData->clip += formHeader->lengthOrCount.clipCount;
	if (iseclp(find))
		badData->clip += formHeader->clipEntries;
	if (formHeader->type == SAT)
		badData->guideCount += formHeader->satinGuideCount;
	if (istx(find))
		badData->tx += formHeader->fillInfo.texture.count;
}

void chkfstch() {
	unsigned	codedFormIndex, iStitch;

	codedFormIndex = FormIndex << FRMSHFT;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute&FRMSK) >= codedFormIndex)
			StitchBuffer[iStitch].attribute = NOTFRM;
	}
}

void repflt() {
	fPOINT*		vertexPoint;
	unsigned	iForm, iDestination, iVertex, vertexDifference, vertexCount = 0;
	FRMHED*		formHeader;
	BADCNTS		badData;

	iDestination = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (FormList[iForm].vertexCount) {
			MoveMemory(&FormList[iDestination++], &FormList[iForm], sizeof(FRMHED));
			vertexCount += FormList[iForm].vertexCount;
		}
	}
	FormIndex = iDestination;
	ZeroMemory(&badData, sizeof(BADCNTS));
	vertexPoint = new fPOINT[vertexCount];
	iVertex = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		formHeader = &FormList[iForm];
		vertexDifference = formHeader->vertices - FormVertices;
		if (FormVertexIndex >= vertexDifference + formHeader->vertexCount) {
			MoveMemory(&vertexPoint[iVertex], formHeader->vertices, formHeader->vertexCount * sizeof(fPOINT));
			formHeader->vertices = &FormVertices[iVertex];
			iVertex += formHeader->vertexCount;
			bcup(iForm, &badData);
		}
		else {
			if (vertexDifference < FormVertexIndex) {
				formHeader->vertexCount = FormVertexIndex - vertexDifference;
				delsac(iForm);
				MoveMemory(&vertexPoint[iVertex], formHeader->vertices, formHeader->vertexCount * sizeof(fPOINT));
				bcup(iForm, &badData);
			}
			else {
				FormIndex = iForm;
				ClipPointIndex = badData.clip;
				SatinConnectIndex = badData.guideCount;
				TextureIndex = badData.tx;
				chkfstch();
				adbad(IDS_FRMDAT, FormIndex - iForm + 1);
				goto rfltskp;
			}
		}
	}
	FormVertexIndex = iVertex;
rfltskp:;
	MoveMemory(FormVertices, vertexPoint, sizeof(fPOINT)*FormVertexIndex);
	delete[] vertexPoint;
}

void repclp() {
	FRMHED*		formHeader;
	unsigned	iForm, clipCount, clipDifference, badClipCount;
	fPOINT*		clipPoint;

	badClipCount = clipCount = 0;
	clipPoint = new fPOINT[MAXITEMS];
	for (iForm = 0; iForm < FormIndex; iForm++) {
		formHeader = &FormList[iForm];
		if (isclp(iForm)) {
			clipDifference = formHeader->angleOrClipData.clip - ClipPoints;
			if (clipDifference + formHeader->lengthOrCount.clipCount < ClipPointIndex) {
				MoveMemory(&clipPoint[clipCount], formHeader->angleOrClipData.clip, sizeof(fPOINT)*formHeader->lengthOrCount.clipCount);
				formHeader->angleOrClipData.clip = &ClipPoints[clipCount];
				clipCount += formHeader->lengthOrCount.clipCount;
			}
			else {
				if (clipDifference < ClipPointIndex) {
					formHeader->lengthOrCount.clipCount = FormVertexIndex - clipDifference;
					MoveMemory(&clipPoint[clipCount], formHeader->angleOrClipData.clip, sizeof(fPOINT)*formHeader->lengthOrCount.clipCount);
					formHeader->angleOrClipData.clip = &ClipPoints[clipCount];
					clipCount += formHeader->lengthOrCount.clipCount;
				}
				else {
					badClipCount++;
					formHeader->fillType = 0;
				}
			}
		}
		if (iseclp(iForm)) {
			clipDifference = formHeader->borderClipData - ClipPoints;
			if (clipDifference + formHeader->clipEntries < ClipPointIndex) {
				MoveMemory(&clipPoint[clipCount], formHeader->borderClipData, sizeof(fPOINT)*formHeader->clipEntries);
				formHeader->borderClipData = &ClipPoints[clipCount];
				clipCount += formHeader->clipEntries;
			}
			else {
				if (clipDifference < ClipPointIndex) {
					formHeader->clipEntries = FormVertexIndex - clipDifference;
					MoveMemory(&clipPoint[clipCount], formHeader->borderClipData, sizeof(fPOINT)*formHeader->clipEntries);
					formHeader->borderClipData = &ClipPoints[clipCount];
					clipCount += formHeader->clipEntries;
				}
				else {
					badClipCount++;
					formHeader->fillType = 0;
				}
			}
		}
	}
	MoveMemory(&ClipPoints, clipPoint, clipCount * sizeof(fPOINT));
	ClipPointIndex = clipCount;
	if (badClipCount)
		adbad(IDS_CLPDAT, badClipCount);
	delete[] clipPoint;
}

void repsat() {
	unsigned	iForm, guideCount, guideDifference;
	FRMHED*		formHeader;
	BADCNTS		badData;

	ZeroMemory(&badData, sizeof(BADCNTS));
	guideCount = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		formHeader = &FormList[iForm];
		if (formHeader->type == SAT) {
			guideDifference = formHeader->satinOrAngle.guide - SatinConnects;
			if (FormVertexIndex > guideDifference + formHeader->vertexCount) {
				MoveMemory(&SatinConnects[guideCount], formHeader->satinOrAngle.guide, formHeader->satinGuideCount * sizeof(SATCON));
				formHeader->satinOrAngle.guide = &SatinConnects[guideCount];
				guideCount += formHeader->satinGuideCount;
				bcup(iForm, &badData);
			}
			else {
				if (guideDifference < SatinConnectIndex) {
					formHeader->satinGuideCount = SatinConnectIndex - guideDifference;
					MoveMemory(&SatinConnects[guideCount], formHeader->satinOrAngle.guide, formHeader->satinGuideCount * sizeof(SATCON));
					bcup(iForm, &badData);
				}
				else {
					guideCount = badData.guideCount;
					formHeader->satinGuideCount = 0;
				}
			}
		}
	}
	SatinConnectIndex = guideCount;
}

void reptx() {
	unsigned	iForm, textureCount;
	FRMHED*		formHeader;
	BADCNTS		badData;

	ZeroMemory(&badData, sizeof(BADCNTS));
	textureCount = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (istx(iForm)) {
			formHeader = &FormList[iForm];
			if (TextureIndex > formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count) {
				MoveMemory(&SatinConnects[textureCount], &SatinConnects[formHeader->fillInfo.texture.index], formHeader->fillInfo.texture.count * sizeof(SATCON));
				formHeader->fillInfo.texture.index = textureCount;
				textureCount += formHeader->fillInfo.texture.count;
				bcup(iForm, &badData);
			}
			else {
				if (TextureIndex > formHeader->fillInfo.texture.index) {
					formHeader->fillInfo.texture.count = TextureIndex - formHeader->fillInfo.texture.index;
					MoveMemory(&SatinConnects[textureCount], &SatinConnects[formHeader->fillInfo.texture.index], formHeader->fillInfo.texture.count * sizeof(SATCON));
					formHeader->fillInfo.texture.index = textureCount;
					bcup(iForm, &badData);
					textureCount = badData.tx;
				}
				else
					formHeader->fillType = 0;
			}
		}
	}
	TextureIndex = textureCount;
}

void repar() {
	unsigned	repairType;
	TCHAR*		savedStringData;

	savedStringData = StringData;
	savdo();
	StringData = MsgBuffer;
	repairType = frmchkfn();
	if (repairType&BADFLT)
		repflt();
	if (repairType&BADCLP)
		repclp();
	if (repairType&BADSAT)
		repsat();
	if (repairType&BADTX)
		reptx();
	lodchk();
	setMap(RESTCH);
	if (StringData != MsgBuffer) {
		StringData--;
		*StringData = 0;
		shoMsg(MsgBuffer);
	}
	StringData = savedStringData;
}

void tst() {
	strcpy_s(IniFile.designerName, "Mr");
	strcpy_s(ThrName, IniFile.designerName);
	setMap(RESTCH);
}
