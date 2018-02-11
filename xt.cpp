#ifndef NOMINMAX
# define NOMINMAX
#endif
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include <CppCoreCheck\warnings.h>
#pragma warning( push )  
#pragma warning(disable: ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning( pop )  

#include "lang.h"
#include "resource.h"
#include "thred.h"

void		chktxnum();
void		deltx();
unsigned	dutyp(unsigned attribute) noexcept;
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
extern	TCHAR*			RepairString;
extern	double			RotationAngle;
extern	dPOINT			RotationCenter;
extern	FRMHED*			SelectedForm;
extern	unsigned		SatinGuideIndex;
extern	SATCON			SatinGuides[MAXSAC];
extern	EnumMap<StateFlag>	StateMap;
extern	TCHAR*			StringData;
extern	unsigned		SelectedFormCount;
extern	unsigned		SelectedFormCount;
extern	unsigned short	SelectedFormList[MAXFORMS];
extern	fPOINT			SelectedPoint;
extern	unsigned		SequenceIndex;
extern	TCHAR*			StringTable[STR_LEN];
extern	fPOINTATTR		StitchBuffer[MAXITEMS * 2];
extern	double			LineSpacing;
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
extern	EnumMap<UserFlag>	UserFlagMap;
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
extern	void		butxt(unsigned iButton, const TCHAR* buttonText);
extern	void		centir();
extern	void		chkhup();
extern	bool		chkmax(unsigned arg0, unsigned arg1);
extern	void		chkmen();
extern	bool		chkr(unsigned bit);
extern	void		chkseq(bool border);
extern	bool		cisin(float xCoordinate, float yCoordinate);
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
extern	bool		filmsgs(unsigned code);
extern	void		filsat();
extern	void		filsfn();
extern	void		filvrt();
extern	unsigned	find1st();
extern	void		fnhor();
extern	void		fnvrt();
extern	void		frmclr(FRMHED* destination);
extern	void		frmcpy(FRMHED* destination, const FRMHED* source);
extern	void		frmout(unsigned formIndex);
extern	void		frmrct(fRECTANGLE* rectangle);
extern	void		fshor();
extern	void		fvars(unsigned iForm);
extern	unsigned	getlast();
extern	void		hsizmsg();
extern	bool		isclp(unsigned iForm);
extern	bool		iseclp(unsigned iForm);
extern	bool		isin(float xCoordinate, float yCoordinate);
extern	void		lcon();
extern	void		makspac(unsigned start, unsigned count);
extern	void		mdufrm();
extern	float		midl(float high, float low);
extern	void		moveStitch(fPOINTATTR* destination, const fPOINTATTR* source);
extern	void		movStch();
extern	void		movStch();
extern	void		msgflt(unsigned messageId, float value);
extern	void		mvstchs(unsigned destination, unsigned source, unsigned count);
extern	void		numWnd();
extern	constexpr unsigned	nxt(unsigned int iVertex);
extern	constexpr unsigned	prv(unsigned iVertex);
extern	unsigned	psg();
extern	unsigned	px2stch();
extern	void		redraw(HWND window);
extern	void		refil();
extern	void		refilal();
extern	void		refilfn();
extern	void		ritfil();
extern	void		rotflt(fPOINT* point);
extern	void		rtrclpfn();
extern	void		satfil();
extern	void		satout(double satinWidth);
extern	void		savdo();
extern	void		save();
extern	void		setknots();
extern	void		setmfrm();
extern	void		setr(unsigned bit);
extern	void		shoMsg(TCHAR* string);
extern	void		shoseln(unsigned code0, unsigned code1);
extern	void		stchrct(fRECTANGLE* rectangle);
extern	void		tabmsg(unsigned code);
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
fPOINT		InterleaveSequence[MAXITEMS]; //storage for interleave points
unsigned	InterleaveSequenceIndex;	//index into the interleave sequence
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
		StateMap.set(StateFlag::FCHK);
}

#ifdef _DEBUG

void prbug() noexcept {
	OutputDebugString(MsgBuffer);
}
#endif

bool istx(unsigned iForm) noexcept {
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

TXPNT* adtx(int count) noexcept {

	unsigned iPoint = TextureIndex;

	TextureIndex += count;
	return &TexturePointsBuffer[iPoint];
}

void txspac(int start, unsigned count) noexcept {
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
	StateMap.reset(StateFlag::TXTMOV);
	StateMap.reset(StateFlag::BZUM);
	StateMap.reset(StateFlag::BZUMIN);
	StateMap.set(StateFlag::RESTCH);
	StateMap.reset(StateFlag::POLIMOV);
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	StateMap.set(StateFlag::RESTCH);
}

void txrbak() noexcept {
	TextureHistoryIndex--;
	TextureHistoryIndex &= 0xf;
}

void txrfor() noexcept {
	TextureHistoryIndex++;
	TextureHistoryIndex &= 0xf;
}

bool chktxh(_In_ const TXHST* historyItem) noexcept {
	if (historyItem) {
		unsigned iPoint = 0;

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
	}
	return 0;
}

void savtxt() {
	TXHST*	 currentHistoryItem = nullptr;

	if (TextureScreen.index) {
		currentHistoryItem = &TextureHistory[TextureHistoryIndex];
		if (chktxh(currentHistoryItem)) {
			StateMap.set(StateFlag::WASTXBAK);
			StateMap.reset(StateFlag::TXBDIR);
			StateMap.reset(StateFlag::LASTXBAK);
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

void deorg(POINT* point) noexcept {
	point->x = Msg.pt.x - StitchWindowOrigin.x;
	point->y = Msg.pt.y - StitchWindowOrigin.y;
}

void fthvars() {

	StateMap.reset(StateFlag::BARSAT);
	StateMap.reset(StateFlag::FTHR);
	FeatherFillType = SelectedForm->fillInfo.feather.fillType;
	FormFeatherRatio = SelectedForm->fillInfo.feather.ratio;
	FeatherMinStitch = SelectedForm->fillInfo.feather.minStitchSize;
	FeatherTotalCount = SelectedForm->fillInfo.feather.count;
	ExtendedAttribute = SelectedForm->extendedAttribute;
	FeatherCountUp = FeatherUpCount = SelectedForm->fillInfo.feather.upCount;
	FeatherCountDown = FeatherDownCount = SelectedForm->fillInfo.feather.downCount;
	FeatherPhaseIndex = FeatherUpCount + FeatherDownCount;
	if (ExtendedAttribute&AT_FTHBLND)
		StateMap.set(StateFlag::BARSAT);
	else
		StateMap.set(StateFlag::FTHR);
}

constexpr float durat(float start, float finish) {

	return (finish - start)*FeatherRatio + start;
}

constexpr float duxrat(float strt, float fin) {

	return (fin - strt)*FeatherRatioLocal + strt;
}

void duxrats(unsigned start, unsigned finish, fPOINT* point) noexcept {

	point->x = duxrat(BSequence[finish].x, BSequence[start].x);
	point->y = duxrat(BSequence[finish].y, BSequence[start].y);
}

void durats(unsigned iSequence, fPOINT* point) noexcept {

	const double	stitchLength = hypot(BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y);
	fPOINT	adjustedPoint = {};

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

void xoseq(unsigned iSequence) noexcept {

	OSequence[OutputIndex].x = BSequence[iSequence].x;
	OSequence[OutputIndex].y = BSequence[iSequence].y;
	OutputIndex++;
}

void xpfth(unsigned iSequence) noexcept {

	FeatherSequence[ActivePointIndex].x = BSequence[iSequence].x;
	FeatherSequence[ActivePointIndex].y = BSequence[iSequence].y;
	ActivePointIndex++;
}

unsigned bpsg() noexcept {

	unsigned testValue = 0;

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

					FeatherRatio = static_cast<float>(FeatherTotalCount - (psg() % FeatherTotalCount)) / FeatherTotalCount;
					FeatherCountUp--;
				}
				else {

					FeatherRatio = static_cast<float>(FeatherTotalCount - (bpsg() % FeatherTotalCount)) / FeatherTotalCount;
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
				FeatherRatio = static_cast<float>(FeatherTotalCount - (psg() % FeatherTotalCount)) / FeatherTotalCount;
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

void ratpnt(unsigned iPoint, unsigned iNextPoint, fPOINT* point) noexcept {

	point->x = (BSequence[iNextPoint].x - BSequence[iPoint].x)*FeatherRatio + BSequence[iPoint].x;
	point->y = (BSequence[iNextPoint].y - BSequence[iPoint].y)*FeatherRatio + BSequence[iPoint].y;
}

void midpnt(fPOINT startPoint, fPOINT endPoint, fPOINT* midPoint) noexcept {

	midPoint->x = (endPoint.x - startPoint.x) * 0.5f + startPoint.x;
	midPoint->y = (endPoint.y - startPoint.y) * 0.5f + startPoint.y;
}

void xratf(fPOINT startPoint, fPOINT endPoint, fPOINT* point) noexcept {

	point->x = (endPoint.x - startPoint.x)*FeatherRatioLocal + startPoint.x;
	point->y = (endPoint.y - startPoint.y)*FeatherRatioLocal + startPoint.y;
}

void fthrbfn(unsigned iSequence) {

	fPOINT	currentPoint = {};
	fPOINT	nextPoint = {};
	fPOINT	currentHighPoint = {};
	fPOINT	currentLowPoint = {};
	fPOINT	nextHighPoint = {};
	fPOINT	nextLowPoint = {};
	fPOINT	midPoint = {};

	const double	length = hypot(BSequence[iSequence + 1].y - BSequence[iSequence].y, BSequence[iSequence + 1].x - BSequence[iSequence].x);

	nurat();
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

void duoseq(unsigned iSequence) noexcept {

	OSequence[iSequence].x = BSequence[iSequence].x;
	OSequence[iSequence].y = BSequence[iSequence].y;
}

void fthdfn(unsigned iSequence) {

	const double	length = hypot(BSequence[iSequence + 1].y - BSequence[iSequence].y, BSequence[iSequence + 1].x - BSequence[iSequence].x);
	fPOINT	adjustedPoint = {};
	fPOINT	currentPoint = {};
	fPOINT	nextPoint = {};

	nurat();
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

	unsigned		ind = 0, res = 0;
	const double	savedSpacing = LineSpacing;

	// ToDo - what does this function do
	PseudoRandomValue = FSED;
	fthvars();
	LineSpacing = SelectedForm->fillSpacing;
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
	FeatherGlobalStep = 4.0 / SequenceIndex*ind;
	FeatherGlobalPhase = static_cast<float>(SequenceIndex) / ind;
	FeatherGlobalRatio = static_cast<float>(FeatherCountUp) / FeatherPhaseIndex;
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
	StateMap.reset(StateFlag::FTHR);
	StateMap.reset(StateFlag::BARSAT);
	LineSpacing = savedSpacing;
	SequenceIndex = OutputIndex;
}

void fritfil() {

	unsigned iSequence = 0, iReverseSequence = 0;

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
		SelectedForm->fillSpacing = LineSpacing;
		SelectedForm->fillColor = ActiveColor;
		SelectedForm->fillInfo.feather.color = (ActiveColor + 1)&COLMSK;
		SelectedForm->fillType = FTHF;
		refilfn();
	}
}

void fethr() {

	unsigned iForm = 0;

	if (filmsgs(FMM_FTH))
		return;
	if (SelectedFormCount) {

		savdo();
		for (iForm = 0; iForm < SelectedFormCount; iForm++) {

			ClosestFormToCursor = SelectedFormList[iForm];
			fethrf();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {

		if (StateMap.test(StateFlag::FORMSEL)) {

			savdo();
			fethrf();
			StateMap.set(StateFlag::INIT);
			coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

constexpr ULARGE_INTEGER  tim2int(FILETIME time) noexcept {

	ULARGE_INTEGER  op = { {0,0} };

	op.LowPart = time.dwLowDateTime;
	op.HighPart = time.dwHighDateTime;
	return op;
}

int fil2crd(const char* fileName) noexcept {
	STARTUPINFO			startupInfo = {};
	PROCESS_INFORMATION	processInfo = {};
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

bool chkp2cnam(const char* fileName) noexcept {

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

	HKEY			registryKey = {};
	unsigned long	keyType = 0, size = 0;
	TCHAR			programName[_MAX_PATH] = { 0 };
	const char		filter[] = "ComputerService (Lind2PC.exe)\0LinkP2C.exe\0\0";
	TCHAR			message[P2CBUFSIZ] = { 0 };
	TCHAR			caption[P2CBUFSIZ] = { 0 };
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
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 0, KEY_READ, &registryKey) == ERROR_SUCCESS) {

		size = _MAX_PATH;
		keyType = REG_SZ;
		if (RegQueryValueEx(registryKey, "ProgramFilesDir", 0, &keyType, (unsigned char*)programName, &size) == ERROR_SUCCESS) {

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

void sidlen(unsigned start, unsigned finish, double* insideLength, double* outsideLength) noexcept {
	if (insideLength && outsideLength) {
		*insideLength += hypot(InsidePoints[finish].x - InsidePoints[start].x, InsidePoints[finish].x - InsidePoints[start].x);
		*outsideLength += hypot(OutsidePoints[finish].x - OutsidePoints[start].x, OutsidePoints[finish].x - OutsidePoints[start].x);
	}
}

fPOINT* insid() {
	unsigned iVertex = 0;

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
	unsigned	iStitch = 0, stitchCount = 0;

	if (PCSHeader.stitchCount) {
		stitchCount = 0;
		fPOINTATTR* highStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
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

void chkuseq() noexcept {
#if BUGBAK
	unsigned index;

	for (index = 0; index < OutputIndex; index++) {
		InterleaveSequence[index].x = OSequence[index].x;
		InterleaveSequence[index].y = OSequence[index].y;
	}
	InterleaveSequenceIndex = index;
	InterleaveSequenceIndices[InterleaveSequenceIndex2].color = SelectedForm->UnderlayColor;
#else

	unsigned	iSequence = 0, index = 0, stitchCount = 0;
	double		length = 0.0;
	fPOINT		delta = {};
	fPOINT		point = {};
	fPOINT		step = {};
	double		underlayStitchLength = 0.0;

	if (OutputIndex) {
		if (SelectedForm->underlayStitchLen < MINWLK)
			SelectedForm->underlayStitchLen = MINWLK;
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

void ritwlk() noexcept {
	if (OutputIndex) {
		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = WLKMSK;
		chkuseq();
		InterleaveSequenceIndex2++;
	}
}

void ritcwlk() noexcept {
	if (OutputIndex) {

		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = CWLKMSK;
		chkuseq();
		InterleaveSequenceIndex2++;
	}
}

unsigned gucon(fPOINT start, fPOINT finish, unsigned destination, unsigned code) {
	double			length = hypot(finish.x - start.x, finish.y - start.y);
	unsigned		startVertex = closflt(start.x, start.y); 
	const unsigned	endVertex = closflt(finish.x, finish.y); 
	unsigned		stitchCount = 0, intermediateVertex = 0;
	unsigned		up = 0, down = 0, iStitch = 0, iStep = 0;
	fPOINT			localPoint = {}, step = {}, delta = {};

	if (length < 5)
		return 0;
	if (startVertex == endVertex)
		return 0;
	const fPOINT* indentedPoint = insid();
	up = down = startVertex;
	do {
		if (up == endVertex) {
			StateMap.reset(StateFlag::WLKDIR);
			break;
		}
		if (down == endVertex) {
			StateMap.set(StateFlag::WLKDIR);
			break;
		}
		up = nxt(up);
		down = prv(down);
	} while (true);
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
		if (StateMap.test(StateFlag::WLKDIR))
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
		if (StateMap.test(StateFlag::WLKDIR))
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
	unsigned	start = 0, count = 0;

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
	const fPOINT* walkPoints = insid();
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

	unsigned	iVertex = 0;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
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

void ritund() noexcept {
	if (SequenceIndex) {

		InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].seq = I_FIL;
		InterleaveSequenceIndices[InterleaveSequenceIndex2].code = UNDMSK;
		chkuseq();
		InterleaveSequenceIndex2++;
	}
}

void undclp() noexcept {
	ClipRectSize.cx = ClipRect.bottom = ClipRect.left = ClipRect.right = ClipBuffer[0].x = ClipBuffer[1].x = ClipBuffer[0].y = 0;
	ClipRectSize.cy = ClipRect.top = ClipBuffer[1].y = SelectedForm->underlayStitchLen;
	ClipStitchCount = 2;
}

void fnund(unsigned find) {
	const float	savedStitchSize = UserStitchLength;

	UnderlayVertices = insid();
	UserStitchLength = 1e99;
	if (!SelectedForm->underlaySpacing)
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	if (!SelectedForm->underlayStitchLen)
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	undclp();
	StateMap.set(StateFlag::ISUND);
	angclpfn();
	OutputIndex = SequenceIndex;
	ritund();
	fvars(find);
	UserStitchLength = savedStitchSize;
}

void fncwlk() {
	unsigned	iGuide = 0, iVertex = 0, start = 0, finish = 0;

	OutputIndex = 0;
	SelectedForm->extendedAttribute |= AT_CWLK;
	if (SelectedForm->satinGuideCount) {
		if (SelectedForm->wordParam) {
			iVertex = SelectedForm->wordParam;
			OSequence[OutputIndex].x = midl(CurrentFormVertices[iVertex].x, CurrentFormVertices[iVertex + 1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[iVertex].y, CurrentFormVertices[iVertex + 1].y);
			OutputIndex++;
		}
		const SATCON*	guide = SelectedForm->satinOrAngle.guide;
		if (guide) {
			for (iGuide = SelectedForm->satinGuideCount; iGuide != 0; iGuide--) {
				OSequence[OutputIndex].x = midl(CurrentFormVertices[guide[iGuide - 1].finish].x, CurrentFormVertices[guide[iGuide - 1].start].x);
				OSequence[OutputIndex].y = midl(CurrentFormVertices[guide[iGuide - 1].finish].y, CurrentFormVertices[guide[iGuide - 1].start].y);
				OutputIndex++;
			}
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

BOOL CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) noexcept {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buf[HBUFSIZ] = { 0 };
	TCHAR		buf1[HBUFSIZ] = { 0 };
	unsigned	iFeatherStyle = 0, state = 0, featherType = 0;

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
				[[gsl::suppress(type.1)]]
				SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(buf));
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

void dufdef() noexcept {

	DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThrEdWindow, (DLGPROC)fthdefprc);
}

void srtcol() {

	unsigned		histogram[16] = { 0 }, colorStartStitch[16] = { 0 };
	unsigned		iStitch = 0, iColor = 0, startStitch = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		histogram[StitchBuffer[iStitch].attribute&COLMSK]++;
	startStitch = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		colorStartStitch[iColor] = startStitch;
		startStitch += histogram[iColor];
	}
	fPOINTATTR* highStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		moveStitch(&highStitchBuffer[colorStartStitch[StitchBuffer[iStitch].attribute&COLMSK]++], &StitchBuffer[iStitch]);
	MoveMemory(&StitchBuffer, highStitchBuffer, PCSHeader.stitchCount * sizeof(fPOINTATTR));
	delete[] highStitchBuffer;
}

void dubit(unsigned bit) {
	unsigned code = 0;

	savdo();
	StateMap.set(StateFlag::WASDO);
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
	StateMap.set(StateFlag::RESTCH);
}

void setuang() {
	StateMap.set(StateFlag::GTUANG);
	msgflt(IDS_UANG, IniFile.underlayAngle / PI * 180);
}

void setuspac() {
	StateMap.set(StateFlag::GTUSPAC);
	msgflt(IDS_USPAC, IniFile.underlaySpacing / PFGRAN);
}

void setwlkind() {
	StateMap.set(StateFlag::GTWLKIND);
	msgflt(IDS_WLKIND, IniFile.underlayIndent / PFGRAN);
}

void setulen() {
	StateMap.set(StateFlag::GTWLKLEN);
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
	StateMap.set(StateFlag::RESTCH);
}

void chkdaz() noexcept {

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
	UserFlagMap.set(UserFlag::DAZHOL);
	UserFlagMap.set(UserFlag::DAZD);
	IniFile.daisyBorderType = DAZTYP;
}

void initdaz(HWND hWinDialog) {
	TCHAR		buffer[HBUFSIZ] = { 0 };
	unsigned	iType = 0;

	chkdaz();
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyPetalPoints);
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS), buffer);
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyHeartCount);
	SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT), buffer);
	sprintf_s(buffer, sizeof(buffer), "%.2f", IniFile.daisyDiameter);
	SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN), buffer);
	sprintf_s(buffer, sizeof(buffer), "%.2f", IniFile.daisyHoleDiameter);
	SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ), buffer);
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyInnerCount);
	SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS), buffer);
	sprintf_s(buffer, sizeof(buffer), "%d", IniFile.daisyPetalCount);
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS), buffer);
	sprintf_s(buffer, sizeof(buffer), "%.2f", IniFile.daisyPetalLen);
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN), buffer);
	CheckDlgButton(hWinDialog, IDC_HOLE, UserFlagMap.test(UserFlag::DAZHOL));
	CheckDlgButton(hWinDialog, IDC_DLIN, UserFlagMap.test(UserFlag::DAZD));
	for (iType = 0; iType < 6; iType++) {
		LoadString(ThrEdInstance, DaisyTypeStrings[iType], buffer, HBUFSIZ);
		[[gsl::suppress(type.1)]]
		SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(buffer));
	}
	SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
}

BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buffer[HBUFSIZ] = { 0 };
	TCHAR		compareBuffer[HBUFSIZ] = { 0 };
	unsigned	iType = 0;

	switch (umsg) {

		case WM_INITDIALOG:

			SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
			initdaz(hwndlg);
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
						UserFlagMap.set(UserFlag::DAZHOL);
					else
						UserFlagMap.reset(UserFlag::DAZHOL);
					if (IsDlgButtonChecked(hwndlg, IDC_DLIN))
						UserFlagMap.set(UserFlag::DAZD);
					else
						UserFlagMap.reset(UserFlag::DAZD);
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
					initdaz(hwndlg);
					break;

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

	double		holeSegmentAngle = 0.0, petalSegmentAngle = 0.0;
	double		angle = 0.0;
	double		deltaPetalAngle = 0.0;
	double		petalPointAngle = 0.0;
	unsigned	holeVertexCount = 0, petalVertexCount = 0, iSegment = 0, iMacroPetal = 0, iPoint = 0;
	unsigned	iVertex = 0, borderType = 0, sawPointCount = 0, halfPetalPointCount = 0, fref = 0, petalPointCount = 0;
	double		distanceFromDaisyCenter = 0.0;
	double		diameter = 0.0;
	double		petalLength = 0.0;
	double		holeDiameter = 0.0;
	double		ratio = 0.0;
	float		maximumXsize = 0.0, maximumYsize = 0.0;
	fPOINT		referencePoint = {};

	unmsg();
	if (!DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, (DLGPROC)dasyproc)) {
		StateMap.reset(StateFlag::FORMIN);
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
	ratio = maximumXsize / (diameter + petalLength);
	diameter *= ratio;
	petalLength *= ratio;
	holeDiameter *= ratio;
	SelectedForm->type = FRMFPOLY;
	iVertex = fref = 0;
	if (UserFlagMap.test(UserFlag::DAZHOL)) {
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
	if (UserFlagMap.test(UserFlag::DAZD)) {
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

					distanceFromDaisyCenter = diameter + (static_cast<double>(iPoint) / IniFile.daisyPetalPoints*petalLength);
					break;

				case DSAW:

					if (iPoint > halfPetalPointCount)
						sawPointCount = IniFile.daisyPetalPoints - iPoint;
					else
						sawPointCount = iPoint;
					distanceFromDaisyCenter = diameter + (static_cast<double>(sawPointCount) / IniFile.daisyPetalPoints*petalLength);
					break;

				case DRAG:

					distanceFromDaisyCenter = diameter + (static_cast<double>(psg() % IniFile.daisyPetalPoints) / IniFile.daisyPetalPoints*petalLength);
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
			if (UserFlagMap.test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
				SelectedForm->satinOrAngle.guide[iMacroPetal].start = (IniFile.daisyPetalCount - iMacroPetal - 1)*IniFile.daisyInnerCount + 1;
				SelectedForm->satinOrAngle.guide[iMacroPetal].finish = iVertex;
			}
		}
	}
	if (UserFlagMap.test(UserFlag::DAZHOL)) {
		CurrentFormVertices[fref - 1].y += 0.01f;
		CurrentFormVertices[fref].y += 0.01f;
	}
	SelectedForm->vertexCount = iVertex;
	if (UserFlagMap.test(UserFlag::DAZD)) {
		SelectedForm->type = SAT;
		SelectedForm->attribute = 1;
	}
	FormVertexIndex += iVertex;
	StateMap.set(StateFlag::INIT);
	frmout(FormIndex);
	for (iMacroPetal = 0; iMacroPetal < iVertex; iMacroPetal++) {

		CurrentFormVertices[iMacroPetal].x -= SelectedForm->rectangle.left;
		CurrentFormVertices[iMacroPetal].y -= SelectedForm->rectangle.bottom;
	}
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = iVertex + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	mdufrm();
}

void durec(OREC* record) noexcept {

	unsigned			attribute = 0;
	const fPOINTATTR*	stitch = &StitchBuffer[record->start];
	
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
	if (arg1 && arg2) {
		const OREC record1 = **static_cast<OREC * const *>(arg1);
		const OREC record2 = **static_cast<OREC * const *>(arg2);

		if (ColorOrder[record1.color] == ColorOrder[record2.color]) {
			if (record1.form == record2.form) {
				if (record1.type == record2.type)
					return gsl::narrow<int>(record1.start) - gsl::narrow<int>(record2.start);
				else
					return gsl::narrow<int>(record1.type) - gsl::narrow<int>(record2.type);
			}
			else
				return gsl::narrow<int>(record1.form) - gsl::narrow<int>(record2.form);
		}
		return gsl::narrow<int>(ColorOrder[record1.color]) - gsl::narrow<int>(ColorOrder[record2.color]);
	}
	return 0;
}

int refcmp(const void *arg1, const void *arg2) {
	if (arg1 && arg2) {
		const OREC record1 = **static_cast<OREC * const *>(arg1);
		const OREC record2 = **static_cast<OREC * const *>(arg2);

		if (record1.form == record2.form)
			return gsl::narrow<int>(record1.type) - gsl::narrow<int>(record2.type);
		return gsl::narrow<int>(record1.form) - gsl::narrow<int>(record2.form);
	}
	return 0;
}
#endif

bool chkrdun(const SRTREC* stitchRecord) noexcept {
	unsigned iStitch;

	for (iStitch = stitchRecord->start; iStitch < stitchRecord->finish; iStitch++) {
		if (PRecs[iStitch]->otyp == FormFillCounter[PRecs[iStitch]->form])
			return 1;
	}
	return 0;
}

double precjmps(const SRTREC* sortRecord) {
	unsigned		totalJumps;
	double			length;
	double			minimumLength;
	unsigned		iRegion, currentRegion;
	fPOINTATTR*		currentStitch;
	bool			direction;

	FormFillCounter = new unsigned[(FormIndex + 2) << 2]();
	currentRegion = sortRecord->currentRegion;
	direction = sortRecord->direction;
	totalJumps = 0;
	while (chkrdun(sortRecord)) {
		minimumLength = 1e9;
		if (direction)
			currentStitch = &StitchBuffer[PRecs[currentRegion]->finish];
		else
			currentStitch = &StitchBuffer[PRecs[currentRegion]->start];
		for (iRegion = sortRecord->start; iRegion < sortRecord->finish; iRegion++) {
			if (PRecs[iRegion]->otyp == FormFillCounter[PRecs[iRegion]->form]) {
				length = hypot(PRecs[iRegion]->startStitch->x - currentStitch->x, PRecs[iRegion]->startStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction = 0;
					currentRegion = iRegion;
				}
				length = hypot(PRecs[iRegion]->endStitch->x - currentStitch->x, PRecs[iRegion]->endStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction = 1;
					currentRegion = iRegion;
				}
			}
		}
		if (minimumLength > 9 * PFGRAN)
			totalJumps++;
		FormFillCounter[PRecs[currentRegion]->form]++;
		if (StateMap.test(StateFlag::DUSRT)) {
			if (direction) {
				if (PRecs[currentRegion]->start) {
					for (iRegion = PRecs[currentRegion]->finish - 1; iRegion >= PRecs[currentRegion]->start; iRegion--)
						moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[iRegion]);
				}
				else {
					iRegion = PRecs[currentRegion]->finish;
					while (iRegion)
						moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[--iRegion]);
				}
			}
			else {
				for (iRegion = PRecs[currentRegion]->start; iRegion < PRecs[currentRegion]->finish; iRegion++)
					moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[iRegion]);
			}
		}
	}
	delete[] FormFillCounter;
	return totalJumps;
}

unsigned duprecs(SRTREC* sortRecord) {

	sortRecord->direction = 0;
	const unsigned	jumps0 = precjmps(sortRecord);

	sortRecord->direction = 1;
	const unsigned	jumps1 = precjmps(sortRecord);

	if (jumps0 < jumps1) {
		sortRecord->direction = 0;
		return jumps0;
	}
	sortRecord->direction = 1;
	return jumps1;
}

#ifdef _DEBUG

void dmprec(const OREC* stitchRegion, unsigned count) noexcept {
	if (stitchRegion) {
		unsigned iRegion;

		for (iRegion = 0; iRegion < count; iRegion++) {
			sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%4d off: %4d at: %08x frm: %4d typ: %d col: %2d st: %5d fin: %5d\n",
				iRegion,
				&stitchRegion[iRegion] - &stitchRegion[0],
				StitchBuffer[stitchRegion[iRegion].start].attribute,
				stitchRegion[iRegion].form,
				stitchRegion[iRegion].type,
				stitchRegion[iRegion].color,
				stitchRegion[iRegion].start,
				stitchRegion[iRegion].finish);
			OutputDebugString(MsgBuffer);
		}
	}
}
#endif

bool srtchk(const OREC* stitchRegion, unsigned count, unsigned* badForm) noexcept {
	if (stitchRegion) {
		unsigned	iRegion = 1;
		unsigned	form = stitchRegion[0].form;
		unsigned	color = stitchRegion[0].color;
		FRMHED*		formHeader = nullptr;

		for (iRegion = 1; iRegion < count; iRegion++) {
			if (stitchRegion[iRegion].form == form) {
				if (ColorOrder[stitchRegion[iRegion].color] < ColorOrder[color]) {
					formHeader = &FormList[form];
					if (formHeader->fillType == FTHF && formHeader->extendedAttribute&AT_FTHBLND && stitchRegion[iRegion].color == formHeader->fillColor)
						continue;
					if (badForm) {
						*badForm = iRegion;
					}
					return 0;
				}
				else
					color = stitchRegion[iRegion].color;
			}
			else {
				color = stitchRegion[iRegion].color;
				form = stitchRegion[iRegion].form;
			}
		}
		return 1;
	}
	return 0;
}

void fsort() {
	unsigned		attribute = StitchBuffer->attribute&SRTMSK;
	unsigned		iColor = 0, iStitch = 0, badForm = 0, iRange = 0, lastRange = 0;
	unsigned		currentForm = 0, iRegion = 0, lastRegion = 0;
	unsigned		minimumIndex = 0, minimumDirection = 0;
	unsigned		typeCount = 0, jumps = 0, minimumJumps = 0;
	// There cannot be more records than stitches
	OREC*			stitchRegion = new OREC[PCSHeader.stitchCount];
	SRTREC			sortRecord = {};
	FILETIME		fileTime = {};
	ULARGE_INTEGER	startTime = {};
	ULARGE_INTEGER	nextTime = {};

	// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors 
	//        in a single form are not in ascending order already. 
	savdo();
	stitchRegion[0].start = 0;
	stitchRegion[0].startStitch = StitchBuffer;
	ColorOrder[AppliqueColor] = 0;
	for (iColor = 0; iColor < 16; iColor++) {

		if (iColor != AppliqueColor)
			ColorOrder[iColor] = iColor + 1;
	}
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute&SRTMSK) != attribute) {
			stitchRegion[iRegion].finish = iStitch;
			stitchRegion[iRegion].endStitch = &StitchBuffer[iStitch - 1];
			iRegion++;
			stitchRegion[iRegion].start = iStitch;
			stitchRegion[iRegion].startStitch = &StitchBuffer[iStitch];
			attribute = StitchBuffer[iStitch].attribute&SRTMSK;
		}
	}
	stitchRegion[iRegion].endStitch = &StitchBuffer[PCSHeader.stitchCount - 1];
	stitchRegion[iRegion].finish = PCSHeader.stitchCount;
	iRegion++;
	lastRegion = iRegion;
	PRecs = new OREC*[lastRegion];
	PFRecs = new OREC*[lastRegion];
	for (iRegion = 0; iRegion < lastRegion; iRegion++) {
		durec(&stitchRegion[iRegion]);
		PRecs[iRegion] = &stitchRegion[iRegion];
		PFRecs[iRegion] = &stitchRegion[iRegion];
	}
	qsort(PRecs, lastRegion, sizeof(OREC *), recmp);
	qsort(PFRecs, lastRegion, sizeof(OREC *), refcmp);
#ifdef _DEBUG
	dmprec(*PRecs, lastRegion);
#endif
	if (srtchk(*PFRecs, lastRegion, &badForm)) {
		RANGE* stitchRange = new RANGE[lastRegion];
		stitchRange[0].start = 0;
		attribute = PRecs[0]->color;
		currentForm = 0xffffffff;
		typeCount = 0;
		iRange = 0;
		for (iRegion = 0; iRegion < lastRegion; iRegion++) {
			if (attribute != PRecs[iRegion]->color) {
				stitchRange[iRange].finish = iRegion;
				iRange++;
				stitchRange[iRange].start = iRegion;
				attribute = PRecs[iRegion]->color;
				currentForm = PRecs[iRegion]->form;
				typeCount = 0;
				goto srtskp;
			}
			if (PRecs[iRegion]->form == currentForm)
				typeCount++;
			else {
				typeCount = 0;
				currentForm = PRecs[iRegion]->form;
			}
srtskp:;
			PRecs[iRegion]->otyp = typeCount;
		}
		stitchRange[iRange].finish = lastRegion;
		lastRange = ++iRange;
		TempStitchBuffer = new fPOINTATTR[PCSHeader.stitchCount];
		OutputIndex = 0;
		for (iRange = 0; iRange < lastRange; iRange++) {
			StateMap.reset(StateFlag::DUSRT);
			sortRecord.start = stitchRange[iRange].start;
			sortRecord.finish = stitchRange[iRange].finish;
			sortRecord.count = sortRecord.finish - sortRecord.start;
			minimumJumps = 0xffffffff;
			// timeout used to put an upper bound on the number of sorting permutations checked
			GetSystemTimeAsFileTime(&fileTime);
			startTime = tim2int(fileTime);
			for (iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
				sortRecord.currentRegion = iRegion;
				if (!PRecs[iRegion]->otyp) {
					jumps = duprecs(&sortRecord);
					if (jumps < minimumJumps) {
						minimumJumps = jumps;
						minimumIndex = iRegion;
						minimumDirection = sortRecord.direction;
					}
				}
				GetSystemTimeAsFileTime(&fileTime);
				nextTime = tim2int(fileTime);
				if (nextTime.QuadPart - startTime.QuadPart > SRTIM)
					break;
			}
			StateMap.set(StateFlag::DUSRT);
			sortRecord.currentRegion = minimumIndex;
			sortRecord.direction = minimumDirection;
			precjmps(&sortRecord);
		}
		MoveMemory(StitchBuffer, TempStitchBuffer, OutputIndex * sizeof(fPOINTATTR));
		PCSHeader.stitchCount = OutputIndex;
		delete[] TempStitchBuffer;
		coltab();
		StateMap.set(StateFlag::RESTCH);
		delete[] stitchRange;
	}
	else {
		LoadString(ThrEdInstance, IDS_SRTER, HelpBuffer, HBUFSIZ);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), HelpBuffer, PFRecs[badForm]->form);
		shoMsg(MsgBuffer);
	}
	delete[] PFRecs;
	delete[] PRecs;
	delete[] stitchRegion;
}

unsigned dutyp(unsigned attribute) noexcept {
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
	char			result = 0;
	DWORD			bit = 0;
	const unsigned	maskedAttribute = attribute & SRTYPMSK;

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
	char			attributeBuffer[256] = { 0 };
	const unsigned	attribute = StitchBuffer[ind].attribute;
	ATFLD			attributeFields = { (attribute&COLMSK),
										((attribute >> FRMSHFT)&FRMSK),
										gsl::narrow<unsigned>(StitchTypes[dutyp(attribute)]),
										((attribute >> LAYSHFT) & 7),
										0};

	if (attribute & 0x80000000)
		attributeFields.user = 1;
	else
		attributeFields.user = 0;
	sprintf_s(attributeBuffer, sizeof(attributeBuffer), "%5d: col: %2d frm: %5d typ: %2d, lay: %1d: usr: %1d at: %08x\n",
		ind,
		attributeFields.color,
		attributeFields.form,
		attributeFields.type,
		attributeFields.layer,
		attributeFields.user,
		attribute);
	OutputDebugString(attributeBuffer);
}

void dmpat() noexcept {
	unsigned iStitch = 0, attribute = StitchBuffer[0].attribute;

	duatf(0);
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (attribute != StitchBuffer[iStitch].attribute) {
			duatf(iStitch);
			attribute = StitchBuffer[iStitch].attribute;
		}
	}
}
#endif

void fdelstch(FILLSTARTS &FillStartsData) {
	unsigned	iSourceStitch = 0, ind = 0, iDestinationStitch = 0, codedFormIndex = 0, attribute = 0, type = 0, tmap = 0, color = 0, bordercolor = 0, tapcol = 0;
	// ToDo - Still not sure what this function does?
	//        I suspect the FillStartsData members are not correctly named
	codedFormIndex = (ClosestFormToCursor << FRMSHFT);
	bordercolor = SelectedForm->borderColor&COLMSK;
	tapcol = SelectedForm->borderColor >> 4;
	for (iSourceStitch = 0; iSourceStitch < PCSHeader.stitchCount; iSourceStitch++) {
		if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX) && iSourceStitch == ClosestPointIndex)
			ClosestPointIndex = iDestinationStitch;
		attribute = StitchBuffer[iSourceStitch].attribute;
 		if (codedFormIndex == (attribute&(FRMSK | NOTFRM))) {
			type = StitchTypes[dutyp(attribute)];
			switch (type) {
				case TYPE_APPLIQUE:

					if (!(tmap&M_AP)) {
						tmap |= M_AP;
						FillStartsData.fillNamed.applique = iDestinationStitch;
					}
					break;

				case TYPE_FTHR:

					if (!(tmap&M_FTH)) {
						tmap |= M_FTH;
						FillStartsData.fillNamed.feather = iDestinationStitch;
					}
					break;

				case TYPE_FILL:

					if (!(tmap&M_FIL)) {
						tmap |= M_FIL;
						FillStartsData.fillNamed.fill = iDestinationStitch;
					}
					break;

				case TYPE_BORDER:

					if (!(tmap&M_BRD)) {
						tmap |= M_BRD;
						FillStartsData.fillNamed.border = iDestinationStitch;
					}
					break;

				default:

					if (SelectedForm->fillType && !(tmap&M_FIL)) {
						tmap |= M_FIL;
						FillStartsData.fillNamed.fill = iDestinationStitch;
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
				FillStartsData.fillNamed.fillColor = iDestinationStitch;
			}
			if (color == SelectedForm->fillInfo.feather.color) {
				tmap |= M_FTHCOL;
				FillStartsData.fillNamed.featherColor = iDestinationStitch;
			}
			if (color == bordercolor) {
				tmap |= M_ECOL;
				FillStartsData.fillNamed.borderColor = iDestinationStitch;
			}
			if (color == tapcol) {
				tmap |= M_APCOL;
				FillStartsData.fillNamed.appliqueColor = iDestinationStitch;
			}
			StitchBuffer[iDestinationStitch].x = StitchBuffer[iSourceStitch].x;
			StitchBuffer[iDestinationStitch].y = StitchBuffer[iSourceStitch].y;
			StitchBuffer[iDestinationStitch].attribute = StitchBuffer[iSourceStitch].attribute;
			iDestinationStitch++;
		}
	}
	FillStartsData.fillNamed.fillColor++;
	FillStartsData.fillNamed.featherColor++;
	FillStartsData.fillNamed.borderColor++;
	FillStartsData.fillNamed.appliqueColor++;
	FillStartsMap = tmap;
	PCSHeader.stitchCount = iDestinationStitch;
	iDestinationStitch = 0;
	if (!(tmap&M_ECOL))
		FillStartsData.fillNamed.borderColor = PCSHeader.stitchCount;
	if (!(tmap&M_FTHCOL))
		FillStartsData.fillNamed.featherColor = PCSHeader.stitchCount;
	if (!(tmap&M_FCOL))
		FillStartsData.fillNamed.fillColor = PCSHeader.stitchCount;
	if (SelectedForm->edgeType) {
		if (SelectedForm->edgeType == EDGEAPPL) {
			if (!(tmap&M_AP)) {
				if (tmap&M_APCOL)
					FillStartsData.fillNamed.applique = FillStartsData.fillNamed.appliqueColor + 1;
				else
					FillStartsData.fillNamed.applique = PCSHeader.stitchCount;
			}
		}
		if (!(tmap&M_BRD)) {
			if (tmap&M_ECOL)
				FillStartsData.fillNamed.border = FillStartsData.fillNamed.borderColor + 1;
			else
				FillStartsData.fillNamed.border = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType || (tmap&(M_WALK | M_UND | M_CWLK))) {
		if (!(tmap&M_FIL)) {
			if (tmap&M_FCOL)
				FillStartsData.fillNamed.fill = FillStartsData.fillNamed.fillColor + 1;
			else
				FillStartsData.fillNamed.fill = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType == FTHF) {
		if (!(tmap&M_FTH)) {
			if (tmap&M_FTHCOL)
				FillStartsData.fillNamed.feather = FillStartsData.fillNamed.featherColor + 1;
			else
				FillStartsData.fillNamed.feather = PCSHeader.stitchCount;
		}
	}
	for (ind = 3; ind; ind--) {
		iDestinationStitch = ind - 1;
		while (iDestinationStitch < ind) {
			if (FillStartsData.fillArray[iDestinationStitch] > FillStartsData.fillArray[ind])
				FillStartsData.fillArray[ind] = FillStartsData.fillArray[iDestinationStitch];
			iDestinationStitch--;
		}
	}
	if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX)) {
		for (ind = 0; ind < sizeof(FillStartsData) >> 2; ind++)
			FillStartsData.fillArray[ind] = ClosestPointIndex;
	}
}

bool lastcol(unsigned index, fPOINT* point) noexcept {
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

void duint(unsigned offset, unsigned code, INTINF *ilData) {
	if (ilData) {
		unsigned	count = 0, iSequence = 0;
		fPOINT		point = {};

		if (ilData->coloc > ilData->start) {
			count = ilData->coloc - ilData->start;
			MoveMemory(&ilData->highStitchBuffer[ilData->output], &StitchBuffer[ilData->start], sizeof(fPOINTATTR)*count);
			ilData->start += count;
			ilData->output += count;
		}
		if (SelectedForm->extendedAttribute&AT_STRT) {
			if (!StateMap.testAndSet(StateFlag::DIDSTRT))
				ilData->output += gucon(CurrentFormVertices[SelectedForm->fillStart], InterleaveSequence[InterleaveSequenceIndices[ilData->pins].index], ilData->output + offset, code);
		}
		if (lastcol(ilData->pins, &point))
			ilData->output += gucon(point, InterleaveSequence[InterleaveSequenceIndices[ilData->pins].index], ilData->output + MAXITEMS, code);
		for (iSequence = InterleaveSequenceIndices[ilData->pins].index; iSequence < InterleaveSequenceIndices[ilData->pins + 1].index; iSequence++) {
			ilData->highStitchBuffer[ilData->output].x = InterleaveSequence[iSequence].x;
			ilData->highStitchBuffer[ilData->output].y = InterleaveSequence[iSequence].y;
			ilData->highStitchBuffer[ilData->output].attribute = code;
			if (ilData->highStitchBuffer[ilData->output].x != ilData->highStitchBuffer[ilData->output - 1].x ||
				ilData->highStitchBuffer[ilData->output].y != ilData->highStitchBuffer[ilData->output - 1].y)
				ilData->output++;
		}
	}
}

bool isfil() noexcept {
	if (SelectedForm->fillType)
		return 1;
	if (SelectedForm->edgeType)
		return 1;
	if (SelectedForm->extendedAttribute&(AT_CWLK | AT_WALK | AT_UND))
		return 1;
	return 0;
}

void chkend(unsigned offset, unsigned code, INTINF *ilData) {
	if (isfil()) {
		StateMap.set(StateFlag::ISEND);
		if (SelectedForm->extendedAttribute&AT_END)
			ilData->output += gucon(InterleaveSequence[InterleaveSequenceIndex - 1], CurrentFormVertices[SelectedForm->fillEnd], ilData->output + offset, code);
	}
}

void intlv(const FILLSTARTS &FillStartsData) {
	unsigned	iSequence = 0, ine = 0, code = 0, offset = 0;
	fPOINT		colpnt = {};
	INTINF		ilData = {};

	StateMap.reset(StateFlag::ISEND);
	fvars(ClosestFormToCursor);
	InterleaveSequenceIndices[InterleaveSequenceIndex2].index = InterleaveSequenceIndex;
	ilData.layerIndex = ((SelectedForm->attribute&FRMLMSK) << (LAYSHFT - 1)) | (ClosestFormToCursor << FRMSHFT);
	StateMap.reset(StateFlag::DIDSTRT);
	if (PCSHeader.stitchCount) {
		offset = MAXITEMS;
		// Todo - Allocate memory locally for ilData.highStitchBuffer
		ilData.highStitchBuffer = &StitchBuffer[MAXITEMS];
		for (iSequence = 0; iSequence < InterleaveSequenceIndex2; iSequence++) {
			ilData.pins = iSequence;
			switch (InterleaveSequenceIndices[iSequence].seq) {
				case I_AP:

					if (FillStartsMap&M_FIL && FillStartsData.fillNamed.applique >= ilData.coloc)
						ilData.coloc = FillStartsData.fillNamed.applique;
					else {
						ilData.coloc = FillStartsData.fillNamed.appliqueColor;
						if (ilData.coloc == 1)
							ilData.coloc = 0;
					}
					break;

				case I_FIL:

					if (FillStartsMap&M_FIL && FillStartsData.fillNamed.fill >= ilData.coloc)
						ilData.coloc = FillStartsData.fillNamed.fill;
					else
						ilData.coloc = FillStartsData.fillNamed.fillColor;
					break;

				case I_FTH:

					if (FillStartsMap&M_FIL && FillStartsData.fillNamed.feather >= ilData.coloc)
						ilData.coloc = FillStartsData.fillNamed.feather;
					else
						ilData.coloc = FillStartsData.fillNamed.featherColor;
					break;

				case I_BRD:

					if (FillStartsMap&M_BRD && FillStartsData.fillNamed.border >= ilData.coloc)
						ilData.coloc = FillStartsData.fillNamed.border;
					else
						ilData.coloc = FillStartsData.fillNamed.borderColor;
					break;
			}
			code = ilData.layerIndex | InterleaveSequenceIndices[ilData.pins].code | InterleaveSequenceIndices[ilData.pins].color;
			duint(offset, code, &ilData);
		}
		chkend(MAXITEMS, code, &ilData);
		if (PCSHeader.stitchCount && ilData.start < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			ine = PCSHeader.stitchCount - ilData.start;
			MoveMemory(&StitchBuffer[ilData.output + MAXITEMS], &StitchBuffer[ilData.start], sizeof(fPOINTATTR)*ine);
			ilData.output += ine;
		}
		MoveMemory(StitchBuffer, ilData.highStitchBuffer, sizeof(fPOINTATTR)*ilData.output);
	}
	else {
		offset = 0;
		for (iSequence = 0; iSequence < InterleaveSequenceIndex2; iSequence++) {
			code = ilData.layerIndex | InterleaveSequenceIndices[iSequence].code | InterleaveSequenceIndices[iSequence].color;
			if (SelectedForm->extendedAttribute&AT_STRT) {
				if (!StateMap.testAndSet(StateFlag::DIDSTRT))
					ilData.output += gucon(CurrentFormVertices[SelectedForm->fillStart], InterleaveSequence[InterleaveSequenceIndices[ilData.pins].index], ilData.output + offset, code);
			}
			if (lastcol(iSequence, &colpnt))
				ilData.output += gucon(colpnt, InterleaveSequence[InterleaveSequenceIndices[iSequence].index], ilData.output, code);
			for (ine = InterleaveSequenceIndices[iSequence].index; ine < InterleaveSequenceIndices[iSequence + 1].index; ine++) {
				StitchBuffer[ilData.output].x = InterleaveSequence[ine].x;
				StitchBuffer[ilData.output].y = InterleaveSequence[ine].y;
				StitchBuffer[ilData.output].attribute = code;
				if (ilData.output > 0) {
					if (StitchBuffer[ilData.output].x != StitchBuffer[ilData.output - 1].x ||
						StitchBuffer[ilData.output].y != StitchBuffer[ilData.output - 1].y)
						ilData.output++;
				}
				else {
					ilData.output++;
				}
			}
		}
		chkend(0, code, &ilData);
	}
	PCSHeader.stitchCount = ilData.output;
	coltab();
}

void setundfn(unsigned code) {
	unsigned iForm = 0;

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
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		SelectedForm->extendedAttribute |= code;
		refilfn();
		coltab();
		StateMap.set(StateFlag::RESTCH);
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
	unsigned iForm = 0;

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
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (SelectedForm->type == FRMLINE)
			return;
		SelectedForm->extendedAttribute &= code;
		refilfn();
		coltab();
		StateMap.set(StateFlag::RESTCH);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			ulenfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		ulenfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void undlen() {
	tabmsg(IDS_SETULEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETULEN);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			uspacfn(SelectedFormList[iForm], spacing);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		uspacfn(ClosestFormToCursor, spacing);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void uspac() {
	tabmsg(IDS_SETUSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUSPAC);
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
	unsigned iForm = 0;

	savdo();
	angle *= (float)PI / 180;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			uangfn(SelectedFormList[iForm], angle);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		uangfn(ClosestFormToCursor, angle);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void sfuang() {
	tabmsg(IDS_SETUANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUANG);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			flenfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		flenfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setflen() {
	tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFLEN);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fspacfn(SelectedFormList[iForm], spacing);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fspacfn(ClosestFormToCursor, spacing);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfspac() {
	tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFSPAC);
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
	unsigned iForm = 0;

	savdo();
	indent *= PFGRAN;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			findfn(SelectedFormList[iForm], indent);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		findfn(ClosestFormToCursor, indent);
	coltab();
	StateMap.set(StateFlag::RESTCH);
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
	unsigned iForm = 0;

	savdo();
	angle *= (float)PI / 180;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fangfn(SelectedFormList[iForm], angle);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fangfn(ClosestFormToCursor, angle);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfang() {
	tabmsg(IDS_SETFANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFANG);
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
	unsigned iForm = 0;

	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			ucolfn(SelectedFormList[iForm], color);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		ucolfn(ClosestFormToCursor, color);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setucol() {
	tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUCOL);
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
	unsigned iForm = 0;

	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fcolfn(SelectedFormList[iForm], color);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fcolfn(ClosestFormToCursor, color);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfcol() {
	tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFCOL);
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
	unsigned iForm = 0;

	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bcolfn(SelectedFormList[iForm], color);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		bcolfn(ClosestFormToCursor, color);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbcol() {
	tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBCOL);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			blenfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		blenfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setblen() {
	tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBLEN);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bspacfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		bspacfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbspac() {
	tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBSPAC);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bminfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		bminfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbmin() {
	tabmsg(IDS_TXT23);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMIN);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			bmaxfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		bmaxfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbmax() {
	tabmsg(IDS_TXT22);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMAX);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fminfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fminfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfmin() {
	tabmsg(IDS_TXT21);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMIN);
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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fmaxfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fmaxfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfmax() {
	tabmsg(IDS_TXT20);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMAX);
	numWnd();
}

void fwidfn(unsigned find, float length) {
	unsigned	iVertex = 0;
	double		ratio = 0.0;
	float		reference = 0.0;

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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fwidfn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fwidfn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfwid() {
	tabmsg(IDS_WID);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFWID);
	numWnd();
}

void setfind() {
	tabmsg(IDS_UWLKIND);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFIND);
	numWnd();
}

void fhifn(unsigned find, float length
) {
	unsigned	iVertex = 0;
	double		ratio = 0.0;
	float		reference = 0.0;

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
	unsigned iForm = 0;

	savdo();
	if (SelectedFormCount) {
		for (iForm = 0; iForm < SelectedFormCount; iForm++)
			fhifn(SelectedFormList[iForm], length);
	}
	if (StateMap.test(StateFlag::FORMSEL))
		fhifn(ClosestFormToCursor, length);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfhi() {
	tabmsg(IDS_HI);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFHI);
	numWnd();
}

void setfilstrt() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		FormList[ClosestFormToCursor].fillStart = ClosestVertexToCursor;
		FormList[ClosestFormToCursor].extendedAttribute |= AT_STRT;
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_FORMP, IDS_FSTRT);
}

void setfilend() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		FormList[ClosestFormToCursor].fillEnd = ClosestVertexToCursor;
		FormList[ClosestFormToCursor].extendedAttribute |= AT_END;
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
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
	StateMap.set(StateFlag::TXTRED);
	StateMap.set(StateFlag::ZUMED);
	StateMap.reset(StateFlag::WASPAT);
	StateMap.reset(StateFlag::RUNPAT);
	movStch();
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
	SelectedTexturePointsList = new unsigned[MAXITEMS];
	SelectedTexturePointsCount = 0;
	StateMap.set(StateFlag::INIT);
	SideWindowButton = 0;
	if (StateMap.test(StateFlag::WASTXBAK)) {
		redtbak();
		if (!TextureScreen.areaHeight)
			TextureScreen.areaHeight = IniFile.textureHeight;
		if (!TextureScreen.spacing)
			TextureScreen.spacing = IniFile.underlaySpacing;
		if (!TextureScreen.width)
			TextureScreen.width = IniFile.textureWidth;
		StateMap.set(StateFlag::LASTXBAK);
		StateMap.reset(StateFlag::TXBDIR);
	}
	else {
		TextureScreen.index = 0;
		TextureScreen.areaHeight = IniFile.textureHeight;
		TextureScreen.width = IniFile.textureWidth;
		TextureScreen.spacing = IniFile.textureSpacing;
	}
	StateMap.set(StateFlag::RESTCH);
}

void txt2pix(TXPNT texturePoint, POINT* screenPoint) noexcept {
	screenPoint->y = TextureScreen.height - texturePoint.y / TextureScreen.areaHeight*TextureScreen.height + TextureScreen.top;
	screenPoint->x = (texturePoint.line*TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
}

void txtxfn(POINT reference, int offsetPixels) noexcept {
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

void dutxtx(int index, int offsetPixels) noexcept {
	POINT	ref;

	txt2pix(TempTexturePoints[index], &ref);
	txtxfn(ref, offsetPixels);
	if (ref.y > TextureScreen.halfHeight)
		ref.y -= TextureScreen.height;
	else
		ref.y += TextureScreen.height;
	txtxfn(ref, offsetPixels);
}

void txrct2rct(TXTRCT textureRect, RECT* rectangle) noexcept {
	TXPNT	texturePoint = { textureRect.top ,textureRect.left };
	POINT	point = {};

	txt2pix(texturePoint, &point);
	rectangle->left = point.x - IniFile.textureEditorSize;
	rectangle->top = point.y - IniFile.textureEditorSize;
	texturePoint.y = textureRect.bottom;
	texturePoint.line = textureRect.right;
	txt2pix(texturePoint, &point);
	rectangle->right = point.x + IniFile.textureEditorSize;
	rectangle->bottom = point.y + IniFile.textureEditorSize;
}

void ed2px(fPOINT editPoint, POINT* point) noexcept {
	point->x = editPoint.x / TextureScreen.editToPixelRatio;
	point->y = StitchWindowClientRect.bottom - editPoint.y / TextureScreen.editToPixelRatio;
}

void px2ed(POINT point, fPOINT* editPoint) noexcept {
	editPoint->x = point.x*TextureScreen.editToPixelRatio;
	editPoint->y = TextureScreen.screenHeight - point.y*TextureScreen.editToPixelRatio;
}

void bxtxt(unsigned iButton, const TCHAR* string) noexcept {
	SetWindowText(ButtonWin[iButton], string);
}

void lodhbuf(unsigned iString) noexcept {
	LoadString(ThrEdInstance, iString, HelpBuffer, HBUFSIZ);
}

void hlpflt(unsigned iMessage, unsigned iButton, float data) noexcept {
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

void chktx() noexcept {
	unsigned iPoint = 0, iNextPoint = 0;

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
	POINT		line[2] = {};
	int			iGrid = 0, iVertical = 0;
	unsigned	iPoint = 0;
	unsigned	index = 0;
	int			yOffset = 0;
	TXPNT		textureRecord = {};
	POINT		point = {};
	int			gridLineCount = 0;
	double		pixelSpace = 0.0;
	float		extraWidth = 0.0;

	const double editSpace = TextureScreen.areaHeight * 2 / (TextureScreen.spacing*(TextureScreen.lines + 2));

	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	pixelSpace = static_cast<double>(StitchWindowClientRect.bottom) / StitchWindowClientRect.right;
	TextureScreen.lines = floor(TextureScreen.width / TextureScreen.spacing);
	extraWidth = TextureScreen.spacing*(TextureScreen.lines + 2);
	if (StateMap.testAndReset(StateFlag::CHKTX))
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

bool px2txt(POINT offset, TXPNT* textureRecord) noexcept {
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
			textureRecord->y = TextureScreen.areaHeight - (static_cast<float>(offset.y - TextureScreen.top) / TextureScreen.height*TextureScreen.areaHeight);
	}
	else
		return 0;
	return 1;
}

bool txbutfn(TXPNT* textureRecord) noexcept {
	POINT	offset;

	deorg(&offset);
	return px2txt(offset, textureRecord);
}

void txtrbut() {
	if (txbutfn(&TempTexturePoints[TextureScreen.index])) {
		savtxt();
		TextureScreen.index++;
		StateMap.set(StateFlag::RESTCH);
	}
	else
		rstxt();

}

bool txtclos(unsigned* closestTexturePoint) {
	if (closestTexturePoint) {
		double		length = 0.0;
		double		minimumLength = 1e99;
		unsigned	iPoint = 0;
		POINT		reference = {}, point = {};

		deorg(&reference);
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
	}
	return 0;
}

void tritx() noexcept {
	POINT		line[2] = {};
	const int	size = IniFile.textureEditorSize << 2;

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
	StateMap.set(StateFlag::TXTMOV);
	TextureCursorLocation.x = SelectTexturePointsOrigin.x = Msg.pt.x - StitchWindowOrigin.x;
	TextureCursorLocation.y = SelectTexturePointsOrigin.y = Msg.pt.y - StitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void ritxrct() noexcept {
	POINT	offset = { (TextureCursorLocation.x - SelectTexturePointsOrigin.x),
					   (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };
	RECT	rectangle = { (TexturePixelRect.left + offset.x),(TexturePixelRect.top + offset.y),
						  (TexturePixelRect.right + offset.x),(TexturePixelRect.bottom + offset.y) };
	POINT	line[5] = {};

	line[0].x = line[1].x = line[4].x = rectangle.left;
	line[2].x = line[3].x = rectangle.right;
	line[0].y = line[3].y = line[4].y = rectangle.top;
	line[1].y = line[2].y = rectangle.bottom;
	Polyline(StitchWindowDC, line, 5);
}

void dutxrct(TXTRCT* textureRect) noexcept {
	unsigned	iPoint = 0;
	TXPNT*		texturePoint = nullptr;

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
	else 
		textureRect->left = textureRect->right = textureRect->top = textureRect->bottom = 0;
}

void ed2stch(fPOINT* pt) noexcept {
	pt->x -= TextureScreen.xOffset;
	pt->y -= TextureScreen.yOffset;
}

void dutxlin(fPOINT point0, fPOINT point1) noexcept {
	double	slope = 0.0;
	float	deltaX = 0.0;
	float	start = 0.0, finish = 0.0, swap = 0.0, yOffset = 0.0;
	int		integerStart = 0, integerFinish = 0;

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
	POINT		screenOffset = {};
	fPOINT		editorOffset = {};
	unsigned	iVertex = 0, iLine = 0, iNextVertex = 0, lineCount = 0;

	deorg(&screenOffset);
	px2ed(screenOffset, &editorOffset);
	if (StateMap.testAndReset(StateFlag::TXHCNTR))
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

	StateMap.reset(StateFlag::TXTMOV);
	deorg(&offset);
	px2ed(offset, &point1);
	px2ed(FormLines[0], &point0);
	dutxlin(point0, point1);
	StateMap.set(StateFlag::RESTCH);
}

void ed2txp(POINT offset, TXPNT* textureRecord) noexcept {
	fPOINT point;

	px2ed(offset, &point);
	textureRecord->line = (point.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5;
	textureRecord->y = TextureScreen.areaHeight - (static_cast<float>(offset.y - TextureScreen.top) / TextureScreen.height*TextureScreen.areaHeight);
}

constexpr int	hitxlin() {
	unsigned	iPoint = 0;
	short		highestLine = 0;

	for (iPoint = 0; iPoint < SelectedTexturePointsCount; iPoint++) {
		if (TempTexturePoints[SelectedTexturePointsList[iPoint]].line > highestLine)
			highestLine = TempTexturePoints[SelectedTexturePointsList[iPoint]].line;
	}
	return highestLine;
}

void txtrup() {
	TXPNT		highestTexturePoint = {};
	TXPNT		lowestTexturePoint = {};
	TXPNT		textureOffset = {};
	float		yOffset = 0.0;
	short		swap = 0;
	unsigned	iPoint = 0;
	int			xCoord = 0, Xmagnitude = 0;
	POINT		offset = {};
	TXPNT*		texturePoint = nullptr;

	if (StateMap.testAndReset(StateFlag::TXTMOV)) {
		savtxt();
		deorg(&offset);
		offset.x -= SelectTexturePointsOrigin.x;
		offset.y -= SelectTexturePointsOrigin.y;
		Xmagnitude = abs(offset.x);
		textureOffset.line = Xmagnitude*TextureScreen.editToPixelRatio / TextureScreen.spacing + 0.5;
		if (offset.x < 0)
			textureOffset.line = -textureOffset.line;
		textureOffset.y = static_cast<float>(-offset.y) / TextureScreen.height*TextureScreen.areaHeight;
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
		if (StateMap.testAndReset(StateFlag::BZUMIN)) {
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
	StateMap.set(StateFlag::RESTCH);
}

void angrct(fRECTANGLE* rectangle) noexcept {
	unsigned	iVertex = 0;

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

void ritxfrm() noexcept {
	unsigned	iVertex = 0, vertexCount = 0;
	POINT		offset = { (TextureCursorLocation.x - SelectTexturePointsOrigin.x),(TextureCursorLocation.y - SelectTexturePointsOrigin.y) };

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
	unsigned	iVertex = 0;
	fRECTANGLE	angleRect = {};
	float		height = 0.0;
	double		ratio = 0.0;

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
		ClipFormHeader = static_cast<FORMCLIP *>(GlobalLock(ClipMemory));
		if (ClipFormHeader) {
			if (ClipFormHeader->clipType == CLP_FRM) {
				SelectedForm = &ClipFormHeader->form;
				frmcpy(&AngledForm, SelectedForm);
				MoveMemory(&AngledFormVertices, &SelectedForm[1], sizeof(fPOINT)*SelectedForm->vertexCount);
				AngledForm.vertices = AngledFormVertices;
				StateMap.reset(StateFlag::TXTLIN);
				StateMap.set(StateFlag::TXTCLP);
				StateMap.set(StateFlag::TXTMOV);
				setxfrm();
				TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
				TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			}
			GlobalUnlock(ClipMemory);
		}
	}
	StateMap.set(StateFlag::RESTCH);
	StateMap.reset(StateFlag::WASWROT);
}

void dutxtlin() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, 2);
}

void txtrmov() {
	if (StateMap.test(StateFlag::TXTLIN)) {
		dutxtlin();
		deorg(&FormLines[1]);
		dutxtlin();
		return;
	}
	if (StateMap.test(StateFlag::TXTCLP)) {
		if (StateMap.testAndSet(StateFlag::WASWROT))
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
	StateMap.reset(StateFlag::TXTCLP);
	StateMap.set(StateFlag::TXTLIN);
	StateMap.set(StateFlag::TXTMOV);
}

void butsid(unsigned windowId) {
	RECT buttonRect = {};

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

int txcmp(const void *arg1, const void *arg2) noexcept {
	const TXPNT	texturePoint0 = *static_cast<const TXPNT *>(arg1);
	const TXPNT	texturePoint1 = *static_cast<const TXPNT *>(arg2);

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
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXVRTF;
			txpar();
		}
	}
}

void txhor() {
	if (TextureScreen.index) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXHORF;
			txpar();
		}
	}
}

void txang() {
	if (TextureScreen.index) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXANGF;
			SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
			txpar();
		}
	}
}

void deltx() {
	unsigned		iBuffer = 0;
	unsigned		iForm = 0;
	bool			flag = false;
	
	const unsigned short currentIndex = FormList[ClosestFormToCursor].fillInfo.texture.index;

	if (TextureIndex && istx(ClosestFormToCursor) && SelectedForm->fillInfo.texture.count) {
		// First check to see if the texture is shared between forms
		for (iForm = 0; iForm < ClosestFormToCursor; iForm++) {
			if (istx(iForm)) {
				if (FormList[iForm].fillInfo.texture.index == currentIndex) {
					flag = true;
				}
			}
		}
		for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			if (istx(iForm)) {
				if (FormList[iForm].fillInfo.texture.index == currentIndex) {
					flag = true;
				}
			}
		}
		// Only if it is not shared, should the texture be deleted
		if (!flag) {
			TXPNT* textureBuffer = new TXPNT[TextureIndex]();
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
		FormList[ClosestFormToCursor].fillType = 0;
	}
}

void nutx() {
	int			iForm = 0, iPoint = 0;
	FRMHED*		formHeader = nullptr;

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
	unsigned	iLine = 0, iPoint = 0;
	float		halfHeight = 0.0;

	if (StateMap.test(StateFlag::FORMSEL)) {
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
	if (StateMap.test(StateFlag::FORMSEL)) {
		altx();
		delmclp(ClosestFormToCursor);
		delsac(ClosestFormToCursor);
		deltx();
		savtxt();
		nutx();
		if (UserFlagMap.test(UserFlag::SQRFIL))
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

void txsrt() noexcept {
	qsort(TempTexturePoints, TextureScreen.index, sizeof(TXPNT), txcmp);
}

void dutxmir() {
	const int	centerLine = (TextureScreen.lines + 1) >> 1; 
	int			iPoint = TextureScreen.index - 1;
	int			iMirrorPoint = 0;

	savtxt();
	txsrt();
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
	StateMap.set(StateFlag::RESTCH);
}

bool chkbut() {
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
	POINT	point = {};

	fvars(ClosestFormToCursor);
	if (chkbut())
		return;
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		savtxt();
		setxclp();
		return;
	}
	if (StateMap.testAndReset(StateFlag::TXTLIN)) {
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
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	SelectedTexturePointsCount = 0;
	StateMap.set(StateFlag::BZUMIN);
	StateMap.reset(StateFlag::BZUM);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y = ZoomBoxLine[0].y - 1;
}

void redtbak() {
	TXHST*		textureHistoryItem;

	sprintf_s(MsgBuffer, sizeof(MsgBuffer),"%d\n",TextureHistoryIndex);
	OutputDebugString(MsgBuffer);
	textureHistoryItem = &TextureHistory[TextureHistoryIndex];
	TextureScreen.areaHeight = textureHistoryItem->height;
	TextureScreen.width = textureHistoryItem->width;
	TextureScreen.spacing = textureHistoryItem->spacing;
	TextureScreen.index = textureHistoryItem->count;
	if (textureHistoryItem->texturePoint) {
		MoveMemory (TempTexturePoints, textureHistoryItem->texturePoint, textureHistoryItem->count * sizeof (TXPNT));
	}
	StateMap.set(StateFlag::RESTCH);
}

void txbak() {
	unsigned	iHistory = 0;

	if (StateMap.test(StateFlag::WASTXBAK)) {
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
	unsigned	iHistory = 0;

	if (StateMap.test(StateFlag::WASTXBAK)) {
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
	unsigned	iPoint = 0, iSourcePoint = 0, iOutputPoint = 0;
	unsigned	iClosestPoint = 0;

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
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (TextureScreen.index && txtclos(&iClosestPoint)) {
		MoveMemory(&TempTexturePoints[iClosestPoint], &TempTexturePoints[iClosestPoint + 1], (TextureScreen.index - iClosestPoint) * sizeof(TXPNT));
		TextureScreen.index--;
		StateMap.set(StateFlag::RESTCH);
	}
}

void txdelal() {
	chktxnum();
	savtxt();
	TextureScreen.index = 0;
	rstxt();
	StateMap.set(StateFlag::RESTCH);
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
				StateMap.set(StateFlag::CHKTX);
				break;

			case HTXWID:

				savtxt();
				TextureScreen.width = value;
				IniFile.textureWidth = value;
				StateMap.set(StateFlag::CHKTX);
				break;

			case HTXSPAC:

				savtxt();
				TextureScreen.spacing = value;
				IniFile.textureSpacing = value;
				TextureScreen.width = value*TextureScreen.lines + value / 2;
				StateMap.set(StateFlag::CHKTX);
				break;
		}
	}
	TextureInputIndex = 0;
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	StateMap.set(StateFlag::RESTCH);
}

void txcntrv() {
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		StateMap.set(StateFlag::TXHCNTR);
		savtxt();
		setxclp();
		StateMap.set(StateFlag::RESTCH);
	}
}

void txof() {
	butxt(HBOXSEL, StringTable[STR_BOXSEL]);
	redraw(ButtonWin[HHID]);
	if (StateMap.test(StateFlag::UPTO))
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
	StateMap.reset(StateFlag::TXTRED);
}

bool istxclp() {
	if (StateMap.test(StateFlag::TXTMOV) && StateMap.test(StateFlag::TXTCLP))
		return 1;
	else
		return 0;

}

void txsiz(double ratio) {
	unsigned	iVertex = 0;
	fRECTANGLE	angleRect = {};

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

bool txdig(unsigned keyCode, TCHAR* character) {
	if (character) {
		if (isdigit(keyCode)) {
			*character = gsl::narrow<TCHAR>(keyCode);
			return 1;
		}
		if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
			*character = gsl::narrow<TCHAR>(keyCode) - VK_NUMPAD0 + 0x30;
			return 1;
		}
		if (keyCode == 0xbe || keyCode == 0x6e) {
			*character = '.';
			return 1;
		}
	}
	return 0;
}

void txnudg(int deltaX, float deltaY) {
	float		screenDeltaY = 0.0;
	float		yCoord = 0.0;
	unsigned	iPoint = 0;
	int			textureLine = 0;

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
	StateMap.set(StateFlag::RESTCH);
}

void txsnap() {
	unsigned	iPoint = 0;
	int			yStep = 0;
	float		halfGrid = 0.0;
	TXPNT*		texturePoint = nullptr;

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
		StateMap.set(StateFlag::RESTCH);
	}
}

void txtkey(unsigned keyCode) {
	TCHAR	character = {};

	if (SideWindowButton) {
		switch (keyCode) {
			case VK_RETURN:

				chktxnum();
				return;

			case VK_ESCAPE:

				txof();

			case 'Q':

				rstxt();
				StateMap.set(StateFlag::RESTCH);
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
			StateMap.set(StateFlag::RESTCH);
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

			if (!StateMap.testAndSet(StateFlag::LASTXBAK)) {
				savtxt();
				txrbak();
			}
			else {
				if (StateMap.testAndReset(StateFlag::TXBDIR))
					txrbak();
			}
			txbak();
			return;

		case 'V':

			if (OpenClipboard(ThrEdWindow))
				txtclp();
			break;

		case 'N':

			StateMap.set(StateFlag::LASTXBAK);
			if (!StateMap.testAndSet(StateFlag::TXBDIR))
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
	StateMap.reset(StateFlag::LASTXBAK);
}

void setxt() {
	const TXPNT *	currentFormTexture = &TexturePointsBuffer[SelectedForm->fillInfo.texture.index];
	if (currentFormTexture) {
		int				iTexturePoint = 0, iSegment = 0;
		const int		count = SelectedForm->fillInfo.texture.count;

		savtxt();
		SelectedForm->wordParam = 0;
		StateMap.set(StateFlag::TXFIL);
		ClipRectSize.cx = SelectedForm->fillSpacing;
		ClipRectSize.cy = SelectedForm->fillInfo.texture.height;
		TextureSegments = new RNGCNT[SelectedForm->fillInfo.texture.lines](); //this is deleted in clpcon
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
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (istx(ClosestFormToCursor))
			rtrtx();
		else
			rtrclpfn();
	}
}

void setstxt(unsigned stringIndex, float value) noexcept {
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), "%.2f", (value / PFGRAN));
	SetWindowText(GetDlgItem(DesignSizeDialog, stringIndex), HelpBuffer);
}

float getstxt(unsigned stringIndex) noexcept {
	GetWindowText(GetDlgItem(DesignSizeDialog, stringIndex), HelpBuffer, HBUFSIZ);
	return atof(HelpBuffer)*PFGRAN;
}

bool chkasp(fPOINT* point) noexcept {
	point->x = getstxt(IDC_DESWID);
	point->y = getstxt(IDC_DESHI);
	if (point->y / point->x == DesignAspectRatio)
		return 1;
	else
		return 0;
}

BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	fPOINT	designSize = {};

	DesignSizeDialog = hwndlg;
	switch (umsg) {
		case WM_INITDIALOG:

			SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
			setstxt(IDC_DESWID, DesignSize.x);
			setstxt(IDC_DESHI, DesignSize.y);
			DesignAspectRatio = DesignSize.y / DesignSize.x;
			CheckDlgButton(hwndlg, IDC_REFILF, UserFlagMap.test(UserFlag::CHREF));
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
						UserFlagMap.set(UserFlag::CHREF);
					else
						UserFlagMap.reset(UserFlag::CHREF);
					EndDialog(hwndlg, 1);
					return TRUE;

				case IDC_DESWID:

					if ((wparam >> 16) == EN_CHANGE)
						StateMap.reset(StateFlag::DESCHG);
					break;

				case IDC_DESHI:

					if ((wparam >> 16) == EN_CHANGE)
						StateMap.set(StateFlag::DESCHG);
					break;

				case IDC_DUASP:

					if (!chkasp(&designSize)) {
						if (StateMap.test(StateFlag::DESCHG))
							setstxt(IDC_DESWID, designSize.y / DesignAspectRatio);
						else
							setstxt(IDC_DESHI, designSize.x*DesignAspectRatio);
					}
					break;
			}
	}
	return 0;
}

// ToDo - Find a better name
void sadj(fPOINTATTR* stitch) noexcept {
	stitch->x = (stitch->x - DesignSizeRect.left)*DesignSizeRatio.x + DesignSizeRect.left;
	stitch->y = (stitch->y - DesignSizeRect.bottom)*DesignSizeRatio.y + DesignSizeRect.bottom;
}

void sadj(fPOINT* point) noexcept {
	point->x = (point->x - DesignSizeRect.left)*DesignSizeRatio.x + DesignSizeRect.left;
	point->y = (point->y - DesignSizeRect.bottom)*DesignSizeRatio.y + DesignSizeRect.bottom;
}

void nudfn() {
	unsigned	iStitch = 0, iVertex = 0;
	fPOINT		newSize = { (DesignSizeRect.right - DesignSizeRect.left),(DesignSizeRect.top - DesignSizeRect.bottom) };

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
	int			flag = 0;
	unsigned	iForm = 0;

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
			if (UserFlagMap.test(UserFlag::CHREF))
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
	//	StateMap.set(StateFlag::BOXSLCT);
	StateMap.set(StateFlag::BZUMIN);
	//	StateMap.set(StateFlag::NOSEL);
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
	if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT))
		SetCapture(ThrEdWindow);
	if (StateMap.test(StateFlag::BZUMIN)) {
		if (Msg.wParam&MK_LBUTTON) {
			if (StateMap.testAndSet(StateFlag::VCAPT))
				SetCapture(ThrEdWindow);
			unbBox();
			ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::BZUM);
			bBox();
		}
	}
}

bool inrct(fRECTANGLE rectangle, fPOINTATTR stitch) noexcept {
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
	fRECTANGLE	selectionRect = {};
	unsigned	iStitch = 0, line = 0;

	savtxt();
	SelectedTexturePointsList = new unsigned[MAXITEMS];
	unbBox();
	StateMap.reset(StateFlag::BZUMIN);
	pxCor2stch(ZoomBoxLine[0]);
	selectionRect.top = SelectedPoint.y;
	selectionRect.left = SelectedPoint.x;
	pxCor2stch(ZoomBoxLine[2]);
	selectionRect.bottom = SelectedPoint.y;
	selectionRect.right = SelectedPoint.x;
	StateMap.reset(StateFlag::TXIN);
	TextureScreen.index = 0;
	line = 1;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (inrct(selectionRect, StitchBuffer[iStitch])) {
			StateMap.set(StateFlag::TXIN);
			TempTexturePoints[TextureScreen.index].line = line;
			TempTexturePoints[TextureScreen.index].y = StitchBuffer[iStitch].y - selectionRect.bottom;
			TextureScreen.index++;
		}
		else {
			if (StateMap.testAndReset(StateFlag::TXIN))
				line++;
		}
	}
	if (TextureScreen.index) { line = TempTexturePoints[TextureScreen.index - 1].line; }
	TextureScreen.spacing = (selectionRect.right - selectionRect.left) / line;
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width = TextureScreen.spacing*line + TextureScreen.spacing / 2;
	StateMap.set(StateFlag::TXTRED);
	StateMap.set(StateFlag::RESTCH);
}

void setclpspac() {
	msgflt(IDS_CLPSPAC, IniFile.clipOffset / PFGRAN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::SCLPSPAC);
	numWnd();
}

BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam) noexcept {
	UNREFERENCED_PARAMETER(lParam);

	DestroyWindow(hwnd);
	return 1;
}

void clrstch() noexcept {
	while (EnumChildWindows(MainStitchWin, enumch, 0));
}

bool txnam(char *name, int sizeName) {
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
	char			name[_MAX_PATH] = { 0 };
	HANDLE			handle = {};
	unsigned long	bytesWritten = 0;
	int				iHistory = 0;
	const char		signature[4] = "txh";

	if (TextureHistory[0].count) {
		if (txnam(name, sizeof(name))) {
			handle = CreateFile(name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (handle != INVALID_HANDLE_VALUE) {
				WriteFile(handle, &signature, sizeof(signature), &bytesWritten, 0);
				WriteFile(handle, &TextureHistoryIndex, sizeof(int), &bytesWritten, 0);
				WriteFile(handle, &TextureHistory, sizeof(TXHST) * 16, &bytesWritten, 0);
				for (iHistory = 0; iHistory < 16; iHistory++) {
					if (TextureHistory[iHistory].count)
						WriteFile(handle, TextureHistory[iHistory].texturePoint, TextureHistory[iHistory].count * sizeof(TXPNT), &bytesWritten, 0);
				}
			}
			CloseHandle(handle);
		}
	}
}

void redtx() {
	char			name[_MAX_PATH] = { 0 };
	HANDLE			handle = {};
	DWORD			bytesRead = 0, historyBytesRead = 0;
	unsigned int	ind = 0,ine = 0;
	char			sig[4] = { 0 };

	TextureHistoryIndex = 15;
	ZeroMemory(&TextureHistory, sizeof(TXHST) * 16);
	if (txnam(name, sizeof(name))) {
		handle = CreateFile(name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (handle != INVALID_HANDLE_VALUE) {
			if (ReadFile(handle, &sig, sizeof(sig), &bytesRead, 0)) {
				if (!strcmp(sig, "txh")) {
					if (ReadFile(handle, &TextureHistoryIndex, sizeof(int), &bytesRead, 0)) {
						if (ReadFile(handle, &TextureHistory, sizeof(TXHST) * 16, &historyBytesRead, 0)) {
							// texturePoint should be a null pointer at this point as no memory has been allocated, but it is not
							// because the old pointer value is read in from the file, so zero it out here as it is easier than 
							// writing a zero to the file
							for (ind = 0; ind < 16; ind++) {
								TextureHistory[ind].texturePoint = 0;
							}

							for (ind = 0; ind < (historyBytesRead / sizeof(TXHST)); ind++) {
								if (TextureHistory[ind].count) {
									TextureHistory[ind].texturePoint = new TXPNT[TextureHistory[ind].count];
									if (!ReadFile(handle, TextureHistory[ind].texturePoint, sizeof(TXPNT)*TextureHistory[ind].count, &bytesRead, 0)) {
										for (ine = 0; ine < TextureHistory[ind].count; ine++) {
											TextureHistory[ine].texturePoint->line = 0;
											TextureHistory[ine].texturePoint->y = 0;
										}
									}
								}
							}
						}
					}
					StateMap.set(StateFlag::WASTXBAK);
				}
			}
		}
		CloseHandle(handle);
	}
	redtbak();
}

void setangf(double angle) {
	unsigned		iVertex = 0;
	const double	savedAngle = RotationAngle;

	RotationAngle = angle;
	MoveMemory(&AngledForm, SelectedForm, sizeof(FRMHED));
	MoveMemory(&AngledFormVertices, CurrentFormVertices, sizeof(fPOINT)*VertexCount);
	RotationCenter.x = static_cast<double>(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = static_cast<double>(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
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
	UserFlagMap.flip(UserFlag::WRNOF);
	wrnmen();
	StateMap.set(StateFlag::DUMEN);
}

void chgchk(int code) {
	IniFile.dataCheck = code;
	chkmen();
	StateMap.set(StateFlag::DUMEN);
}

void lodchk() {
	unsigned	iForm = 0, iStitch = 0, attribute = 0;

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

void chkclp(const FRMHED* formHeader, BADCNTS* badData) noexcept {
	if (badData->clip == formHeader->angleOrClipData.clip - ClipPoints)
		badData->clip += formHeader->lengthOrCount.clipCount;
	else
		badData->attribute |= BADCLP;
}

void chkeclp(const FRMHED* formHeader, BADCNTS* badData) noexcept {
	if (badData->clip == formHeader->borderClipData - ClipPoints)
		badData->clip += formHeader->clipEntries;
	else
		badData->attribute |= BADCLP;
}

unsigned frmchkfn() {
	unsigned	iForm = 0;
	FRMHED*		formHeader = nullptr;
	BADCNTS		badData = {};

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
					if (badData.guideCount == formHeader->satinOrAngle.guide - SatinGuides)
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
		if (badData.flt != gsl::narrow<int>(FormVertexIndex))
			badData.attribute |= BADFLT;
		if (badData.clip != gsl::narrow<int>(ClipPointIndex))
			badData.attribute |= BADCLP;
		if (badData.guideCount != gsl::narrow<int>(SatinGuideIndex))
			badData.attribute |= BADSAT;
		if (badData.tx != TextureIndex)
			badData.attribute |= BADTX;
	}
	return badData.attribute;
}

void frmchkx() {
	unsigned	code = 0;

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

void chkfstch() noexcept {
	const unsigned	codedFormIndex = FormIndex << FRMSHFT; 
	unsigned		iStitch = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute&FRMSK) >= codedFormIndex)
			StitchBuffer[iStitch].attribute = NOTFRM;
	}
}

void repflt() {
	unsigned	iForm = 0, iDestination = 0, iVertex = 0, vertexDifference = 0, vertexCount = 0;
	BADCNTS		badData = {};

	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (FormList[iForm].vertexCount) {
			MoveMemory(&FormList[iDestination++], &FormList[iForm], sizeof(FRMHED));
			vertexCount += FormList[iForm].vertexCount;
		}
	}
	FormIndex = iDestination;
	ZeroMemory(&badData, sizeof(BADCNTS));
	fPOINT* vertexPoint = new fPOINT[vertexCount];
	iVertex = 0;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		FRMHED* formHeader = &FormList[iForm];
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
				SatinGuideIndex = badData.guideCount;
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
	FRMHED*		formHeader = nullptr;
	unsigned	iForm = 0, clipCount = 0, clipDifference = 0, badClipCount = 0;
	fPOINT*		clipPoint = new fPOINT[MAXITEMS];
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
	unsigned	iForm = 0, guideCount = 0, guideDifference = 0;
	FRMHED*		formHeader = nullptr;
	BADCNTS		badData = {};

	for (iForm = 0; iForm < FormIndex; iForm++) {
		formHeader = &FormList[iForm];
		if (formHeader->type == SAT) {
			guideDifference = formHeader->satinOrAngle.guide - SatinGuides;
			if (FormVertexIndex > guideDifference + formHeader->vertexCount) {
				MoveMemory(&SatinGuides[guideCount], formHeader->satinOrAngle.guide, formHeader->satinGuideCount * sizeof(SATCON));
				formHeader->satinOrAngle.guide = &SatinGuides[guideCount];
				guideCount += formHeader->satinGuideCount;
				bcup(iForm, &badData);
			}
			else {
				if (guideDifference < SatinGuideIndex) {
					formHeader->satinGuideCount = SatinGuideIndex - guideDifference;
					MoveMemory(&SatinGuides[guideCount], formHeader->satinOrAngle.guide, formHeader->satinGuideCount * sizeof(SATCON));
					bcup(iForm, &badData);
				}
				else {
					guideCount = badData.guideCount;
					formHeader->satinGuideCount = 0;
				}
			}
		}
	}
	SatinGuideIndex = guideCount;
}

void reptx() {
	unsigned	iForm = 0, textureCount = 0;
	FRMHED*		formHeader = nullptr;
	BADCNTS		badData = {};
		
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (istx(iForm)) {
			formHeader = &FormList[iForm];
			if (TextureIndex > formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count) {
				MoveMemory(&SatinGuides[textureCount], &SatinGuides[formHeader->fillInfo.texture.index], formHeader->fillInfo.texture.count * sizeof(SATCON));
				formHeader->fillInfo.texture.index = textureCount;
				textureCount += formHeader->fillInfo.texture.count;
				bcup(iForm, &badData);
			}
			else {
				if (TextureIndex > formHeader->fillInfo.texture.index) {
					formHeader->fillInfo.texture.count = TextureIndex - formHeader->fillInfo.texture.index;
					MoveMemory(&SatinGuides[textureCount], &SatinGuides[formHeader->fillInfo.texture.index], formHeader->fillInfo.texture.count * sizeof(SATCON));
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
	const unsigned	repairType = frmchkfn();

	savdo();
	RepairString = MsgBuffer;
	if (repairType&BADFLT)
		repflt();
	if (repairType&BADCLP)
		repclp();
	if (repairType&BADSAT)
		repsat();
	if (repairType&BADTX)
		reptx();
	lodchk();
	StateMap.set(StateFlag::RESTCH);
	if (RepairString != MsgBuffer) {
		RepairString--;
		*RepairString = 0;
		shoMsg(MsgBuffer);
	}
}

void tst() {
	strcpy_s(IniFile.designerName, "Mr");
	strcpy_s(ThrName, IniFile.designerName);
	StateMap.set(StateFlag::RESTCH);
}
