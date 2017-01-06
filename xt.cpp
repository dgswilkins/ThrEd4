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
unsigned	dutyp(unsigned tat);
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
extern	unsigned		auth;
extern	TCHAR			AuxName[_MAX_PATH];
extern	BSEQPNT			BSequence[BSEQLEN];
extern	unsigned		ButtonHeight;
extern	unsigned		ButtonWidthX3;
extern	FORMCLIP*		ClipFormData;
extern	fPOINT			ClipPoints[MAXCLPNTS];
extern	fRECTANGLE		ClipRect;
extern	FLSIZ			ClipRectSize;
extern	unsigned		ClipStitchCount;
extern	fPOINTATTR		ClipBuffer[MAXFRMLINS];
extern	unsigned		ClosestFormToCursor;
extern	unsigned		ClosestPointIndex;
extern	unsigned		ClosestVertexToCursor;
extern	unsigned		ClipPointIndex;
extern	fPOINT*			CurrentFormVertices;
extern	TCHAR			WorkingFileName[_MAX_PATH];
extern	unsigned		FormVertexIndex;
extern	fPOINT			FormMoveDelta;
extern	unsigned		FormIndex;
extern	POINT			FormLines[MAXFRMLINS];
extern	FRMHED			FormList[MAXFORMS];
extern	fPOINT			FormVertices[MAXFRMPNTS];
extern	HPEN			GridPen;
extern	HBRUSH			BackgroundBrush;
extern	HWND			ButtonWin[9];
extern	HGLOBAL			ClipMemory;
extern	PCSHEADER		PCSHeader;
extern	HWND			HorizontalScrollBar;
extern	fPOINTATTR*		TempStitchBuffer;
extern	HINSTANCE		ThredInstance;
extern	TCHAR			HelpBuffer[HBUFSIZ];
extern	HWND			MainStitchWin;
extern	unsigned		ThrEdClip;
extern	HWND			VerticalScrollBar;
extern	HWND			ThredWindow;
extern	INIFILE			IniFile;
extern	fPOINT*			InsidePoints;
extern	unsigned		MarkedStitchMap[RMAPSIZ];
extern	MSG				Msg;
extern	TCHAR			MsgBuffer[MSGSIZ];
extern	unsigned		NewFormVertexCount;
extern	unsigned		OutputIndex;
extern	fPOINT*			OutsidePoints;
extern	fPOINT			OSequence[OSEQLEN];
extern	unsigned		PseudoRandomValue;
extern	FRMHED*			SelectedForm;
extern	double			RotationAngle;
extern	dPOINT			RotationCenter;
extern	unsigned		SatinConnectIndex;
extern	SATCON			SatinConnects[MAXSAC];
extern	TCHAR*			StringData;
extern	unsigned		SelectedFormCount;
extern	unsigned		SelectedFormCount;
extern	unsigned short	SelectedFormList[MAXFORMS];
extern	fPOINT			SelectedPoint;
extern	unsigned		SequenceIndex;
extern	unsigned short	SideCount;
extern	TCHAR*			StringTable[STR_LEN];
extern	fPOINTATTR		StitchBuffer[MAXPCS];
extern	double			StitchSpacing;
extern	RECT			StitchWindowClientRect;
extern	HDC				StitchWindowDC;
extern	HDC				StitchWindowMemDC;
extern	POINT			StitchWindowOrigin;
extern	TCHAR*			ThrEdClipFormat;
extern	TCHAR			ThrName[_MAX_PATH];
extern	unsigned		UnderlayColor;
extern	POINT			UnzoomedRect;
extern	COLORREF		UserColor[16];
extern	double			UserStitchLength;
extern	POINT			ZoomBoxLine[5];
extern	fPOINT			ZoomBoxOrigin;
extern	dRECTANGLE		ZoomRect;

extern	void		adbad(unsigned cod, unsigned cnt);
extern	fPOINT*		adflt(unsigned cnt);
extern	SATCON*		adsatk(unsigned	cnt);
extern	void		angclpfn();
extern	void		bakseq();
extern	void		butxt(unsigned ind, TCHAR* str);
extern	void		centir();
extern	void		chkhup();
extern	unsigned	chkMap(unsigned bPnt);
extern	BOOL		chkmax(unsigned arg0, unsigned arg1);
extern	void		chkmen();
extern	BOOL		chkr(unsigned pbit);
extern	void		chkseq(BOOL brd);
extern	unsigned	chku(unsigned bPnt);
extern	BOOL		cisin(float pntx, float pnty);
extern	unsigned	closflt(float px, float py);
extern	void		clpfil();
extern	void		clRmap(unsigned len);
extern	void		coltab();
extern	void		datmsg(unsigned cod);
extern	void		delclps(unsigned ind);
extern	void		delinf();
extern	void		delmclp(unsigned fpnt);
extern	void		delmfil(unsigned col);
extern	void		delsac(unsigned fpnt);
extern	void		dumrk(double pntx, double pnty);
extern	void		filang();
extern	BOOL		filmsgs(unsigned cod);
extern	void		filsat();
extern	void		filsfn();
extern	void		filvrt();
extern	unsigned	find1st();
extern	void		fnhor();
extern	void		fnvrt();
extern	void		frmclr(FRMHED* dst);
extern	void		frmcpy(FRMHED* dst, FRMHED* src);
extern	void		frmout(unsigned ind);
extern	void		frmrct(fRECTANGLE* rct);
extern	void		fshor();
extern	void		fvars(unsigned ind);
extern	unsigned	getlast();
extern	void		hsizmsg();
extern	BOOL		isclp(unsigned find);
extern	BOOL		iseclp(unsigned find);
extern	BOOL		isin(float pntx, float pnty);
extern	void		lcon();
extern	void		makspac(unsigned strt, unsigned cnt);
extern	void		mdufrm();
extern	float		midl(float hi, float lo);
extern	void		moveStitch(fPOINTATTR* dst, fPOINTATTR* src);
extern	void		movStch();
extern	void		movStch();
extern	void		msgflt(unsigned msgid, float par);
extern	void		mvstch(unsigned dst, unsigned src);
extern	void		mvstchs(unsigned dst, unsigned src, unsigned cnt);
extern	void		numWnd();
extern	unsigned short	nxt(unsigned short ind);
extern	unsigned short	prv(unsigned ind);
extern	unsigned	psg();
extern	unsigned	px2stch();
extern	void		redraw(HWND dWnd);
extern	void		refil();
extern	void		refilal();
extern	void		refilfn();
extern	void		ritfil();
extern	void		rotflt(fPOINT* pnt);
extern	void		rseq(unsigned strt, unsigned fin, unsigned ostrt, unsigned at);
extern	unsigned	rstMap(unsigned bPnt);
extern	unsigned	rstu(unsigned bPnt);
extern	void		rtrclpfn();
extern	void		satfil();
extern	void		satout(double satwid);
extern	void		savdo();
extern	void		save();
extern	void		setknots();
extern	unsigned	setMap(unsigned bPnt);
extern	void		setmfrm();
extern	void		setr(unsigned pbit);
extern	unsigned	setu(unsigned bPnt);
extern	void		shoMsg(TCHAR* str);
extern	void		shoseln(unsigned cod0, unsigned cod1);
extern	void		stchrct(fRECTANGLE* rct);
extern	void		tabmsg(unsigned cod);
extern	unsigned	toglMap(unsigned bPnt);
extern	unsigned	toglu(unsigned bPnt);
extern	void		unmsg();
extern	void		unthum();
extern	void		wrnmen();
extern	void		zumhom();

unsigned short daztab[] =
{
	IDS_DAZCRV,
	IDS_DAZSAW,
	IDS_DAZRMP,
	IDS_DAZRAG,
	IDS_DAZCOG,
	IDS_DAZHART,
};

RNGCNT*		textureSegments;			//texture fill groups of points
dPOINT		designSizeRatio;			//design size ratio
fRECTANGLE	designSizeRect;				//design size rectangle
float		designAspectRatio;			//design aspect ratio
HWND		hDlgDesignSize;				//change design size dialog window
fPOINT		designSize;					//design size
TXPNT		texturePointsBuffer[MAXSEQ];	//buffer for textured fill points
int			textureIndex;				//next textured fill point index
unsigned	textureWindowId;			//id of the window being updated
TCHAR		textureInputBuffer[16];		//texture fill number buffer
int			textureInputIndex;			//text number pointer
HWND		hBtnSideWindow;				//button side window
RECT		texturePixelRect;			//screen selected texture points rectangle
TXTRCT		textureRect;				//selected texture points rectangle
POINT		cloxref;	//original location of selected texture points
int			cloxcnt;	//number of selected texture points
unsigned*	cloxlst;	//list of selected points
POINT		textureCursorLocation;		//texture editor move cursor location
HPEN		textureCrossPen;			//texture editor cross pen
TXTSCR		textureScreen;				//texture editor layout parameters
TXPNT		tmpTexturePoints[16384];	//temporary storage for textured fill data
unsigned	colorOrder[16];				//color order adjusted for applique
INTINF		interleaveData;				//interleave data
fPOINT		interleaveSequence[MAXSEQ]; //storage for interleave points
unsigned	interleaveSequenceIndex;	//index into the interleave sequence
FSTRTS		fillStartsData;				//fill start data for refill
INSREC		interleaveSequenceIndices[10];	//indices into interleave points
unsigned	interleaveSequenceIndex2;	//index into interleave indices
unsigned	fillStartsMap;				//fill starts bitmap
unsigned*	formFillCounter;			//form fill type counters for sort
fPOINT*		underlayVertices;			//underlay offset points
unsigned	featherFillType;			//type of feather fill
float		featherRatio;				//feather ratio
float		featherMinStitch;			//smallest stitch allowed
float		featherRatioLocal;			//local feather ratio
float		formFeatherRatio;			//feather ratio from form
unsigned	extendedAttribute;			//extended form attribute
unsigned	featherUpCount;				//feather up count
unsigned	featherDownCount;			//feather down count
unsigned	featherTotalCount;			//up count plus down count
unsigned	featherPhaseIndex;
unsigned	featherPhase;
fPOINT		featherSequence[MAXSEQ];
unsigned	featherCountUp;
unsigned	featherCountDown;

// ToDo - Find better names for these than using 'xt' prefix
float		xtStep;
float		xtPosition;
float		xtPhase;
float		xtUp;
float		xtDown;
float		xtRatio;

OREC**		precs;
OREC**		pfrecs;


char ftyps[] =
{
	0,	// 0 no type
	1,	// 1 applique
	2,	// 2 center walk
	3,	// 3 edge walk	
	4,	// 4 underlay
	0,	// 5 knot
	5,	// 6 feather
	0,	// 7 layer
	0,	// 8 layer
	0,	// 9 layer
	0,	//10 reserved
	6,	//11 fill
	7,	//12 border
};

TXHST	textureHistory[16];		//text editor history headers
int		textureHistoryIndex;	//pointer to the next texture history buffer

void setfchk()
{
	if (IniFile.dataCheck)
		setMap(FCHK);
}

#ifdef _DEBUG

void prbug()
{
	OutputDebugString(MsgBuffer);
}
#endif

BOOL istx(unsigned find)
{
	FRMHED* pfrm;

	pfrm = &FormList[find];
	if (pfrm->fillType == TXVRTF)
		return 1;
	if (pfrm->fillType == TXHORF)
		return 1;
	if (pfrm->fillType == TXANGF)
		return 1;
	return 0;
}

TXPNT* adtx(int cnt) {

	unsigned ind = textureIndex;

	textureIndex += cnt;
	return &texturePointsBuffer[ind];
}

void txspac(int strt, unsigned cnt)
{
	unsigned	ind;

	MoveMemory(&texturePointsBuffer[strt + cnt], &texturePointsBuffer[strt], (textureIndex - strt) & sizeof(TXPNT));
	for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
	{
		if (istx(ind))
			FormList[ind].fillInfo.texture.index += cnt;
	}
	textureIndex += cnt;
}

void rstxt()
{
	cloxcnt = 0;
	rstMap(TXTMOV);
	rstMap(BZUM);
	rstMap(BZUMIN);
	setMap(RESTCH);
	rstMap(POLIMOV);
	DestroyWindow(hBtnSideWindow);
	hBtnSideWindow = 0;
	setMap(RESTCH);
}

void txrbak()
{
	textureHistoryIndex--;
	textureHistoryIndex &= 0xf;
}

void txrfor()
{
	textureHistoryIndex++;
	textureHistoryIndex &= 0xf;
}

BOOL chktxh(TXHST* phst)
{
	int ind;

	if (phst->count != textureScreen.index)
		return 1;
	if (phst->height != textureScreen.areaHeight)
		return 1;
	if (phst->spacing != textureScreen.spacing)
		return 1;
	if (phst->width != textureScreen.width)
		return 1;
	for (ind = 0; ind < textureScreen.index; ind++)
	{
		if (tmpTexturePoints[ind].line != phst->texturePoint[ind].line)
			return 1;
		if (tmpTexturePoints[ind].y != phst->texturePoint[ind].y)
			return 1;
	}
	return 0;
}

void savtxt()
{
	TXHST*	 currentTextureHistory;

	if (textureScreen.index)
	{
		currentTextureHistory = &textureHistory[textureHistoryIndex];
		if (chktxh(currentTextureHistory))
		{
			setMap(WASTXBAK);
			rstMap(TXBDIR);
			rstMap(LASTXBAK);
			txrfor();
			currentTextureHistory = &textureHistory[textureHistoryIndex];
			currentTextureHistory->count = textureScreen.index;
			currentTextureHistory->height = textureScreen.areaHeight;
			currentTextureHistory->width = textureScreen.width;
			currentTextureHistory->spacing = textureScreen.spacing;
			if (currentTextureHistory->texturePoint)
			{
				delete[](currentTextureHistory->texturePoint);
				currentTextureHistory->texturePoint = 0;
			}
			currentTextureHistory->texturePoint = new TXPNT[currentTextureHistory->count];
			MoveMemory(currentTextureHistory->texturePoint, tmpTexturePoints, currentTextureHistory->count * sizeof(TXPNT));
		}
	}
}

void deorg(POINT* pt)
{
	pt->x = Msg.pt.x - StitchWindowOrigin.x;
	pt->y = Msg.pt.y - StitchWindowOrigin.y;
}

void fthvars() {

	rstMap(BARSAT);
	rstMap(FTHR);
	featherFillType = SelectedForm->fillInfo.feather.fillType;
	formFeatherRatio = SelectedForm->fillInfo.feather.ratio;
	featherMinStitch = SelectedForm->fillInfo.feather.minStitchSize;
	featherTotalCount = SelectedForm->fillInfo.feather.count;
	extendedAttribute = SelectedForm->extendedAttribute;
	featherCountUp = featherUpCount = SelectedForm->fillInfo.feather.upCount;
	featherCountDown = featherDownCount = SelectedForm->fillInfo.feather.downCount;
	featherPhaseIndex = featherUpCount + featherDownCount;
	if (extendedAttribute&AT_FTHBLND)
		setMap(BARSAT);
	else
		setMap(FTHR);
}

float durat(float strt, float fin) {

	return (fin - strt)*featherRatio + strt;
}

float duxrat(float strt, float fin) {

	return (fin - strt)*featherRatioLocal + strt;
}

void duxrats(unsigned strt, unsigned fin, fPOINT* opt) {

	opt->x = duxrat(BSequence[fin].x, BSequence[strt].x);
	opt->y = duxrat(BSequence[fin].y, BSequence[strt].y);
}

void durats(unsigned ind, fPOINT* pflt) {

	double	olen;
	fPOINT	ipnt;

	olen = hypot(BSequence[ind + 1].x - BSequence[ind].x, BSequence[ind + 1].y - BSequence[ind].y);
	if (olen < featherMinStitch) {

		pflt->x = BSequence[ind].x;
		pflt->y = BSequence[ind].y;
	}
	else {

		featherRatioLocal = featherMinStitch / olen;
		ipnt.x = duxrat(BSequence[ind + 1].x, BSequence[ind].x);
		ipnt.y = duxrat(BSequence[ind + 1].y, BSequence[ind].y);
		pflt->x = durat(ipnt.x, BSequence[ind].x);
		pflt->y = durat(ipnt.y, BSequence[ind].y);
	}
}

void xoseq(unsigned ind) {

	OSequence[OutputIndex].x = BSequence[ind].x;
	OSequence[OutputIndex].y = BSequence[ind].y;
	OutputIndex++;
}

void xpfth(unsigned ind) {

	featherSequence[ActivePointIndex].x = BSequence[ind].x;
	featherSequence[ActivePointIndex].y = BSequence[ind].y;
	ActivePointIndex++;
}

unsigned bpsg() {

	unsigned tmp;

	if (!PseudoRandomValue)
		PseudoRandomValue = FSED;
	tmp = PseudoRandomValue & 0x40000008;
	PseudoRandomValue >>= 1;
	if (tmp == 0x8 || tmp == 0x40000000)
		PseudoRandomValue |= 0x40000000;
	return PseudoRandomValue;
}

void nurat() {

	float	rem;

	rem = fmod(xtPosition, 1);
	switch (featherFillType) {

	case FTHPSG:

		if (featherUpCount) {

			if (featherCountUp) {

				featherRatio = (float)(featherTotalCount - (psg() % featherTotalCount)) / featherTotalCount;
				featherCountUp--;
			}
			else {

				featherRatio = (float)(featherTotalCount - (bpsg() % featherTotalCount)) / featherTotalCount;
				if (featherCountDown)
					featherCountDown--;
				else {

					PseudoRandomValue = FSED;
					featherCountUp = featherUpCount;
					featherCountDown = featherDownCount;
				}
			}
		}
		else
			featherRatio = (float)(featherTotalCount - (psg() % featherTotalCount)) / featherTotalCount;
		featherRatio *= formFeatherRatio;
		break;

	case FTHFAZ:

		if (featherPhase >= featherUpCount)
			featherRatio = 1;
		else
			featherRatio = formFeatherRatio;
		break;

	case FTHSIN:

		if (rem > xtRatio)
			featherRatio = sin((1 - rem) / (1 - xtRatio)*PI + PI)*0.5 + 0.5;
		else
			featherRatio = sin(rem / xtRatio*PI)*0.5 + 0.5;
		featherRatio *= formFeatherRatio;
		break;

	case FTHSIN2:

		if (rem > xtRatio)
			featherRatio = sin((1 - rem) / (1 - xtRatio)*PI);
		else
			featherRatio = sin(rem / xtRatio*PI);
		featherRatio *= formFeatherRatio;
		break;

	case FTHRMP:

		if (rem > xtRatio)
			featherRatio = (1 - rem) / (1 - xtRatio);
		else
			featherRatio = rem / xtRatio;
		featherRatio *= formFeatherRatio;
		break;

	case FTHLIN:
	default:

		featherRatio = formFeatherRatio;
	}
	++featherPhase %= featherPhaseIndex;
	xtPosition += xtStep;
}

void fthfn(unsigned ind) {

	nurat();
	durats(ind, &OSequence[ind]);
}

void ratpnt(unsigned pt0, unsigned pt1, fPOINT* opt) {

	opt->x = (BSequence[pt1].x - BSequence[pt0].x)*featherRatio + BSequence[pt0].x;
	opt->y = (BSequence[pt1].y - BSequence[pt0].y)*featherRatio + BSequence[pt0].y;
}

void midpnt(fPOINT pt0, fPOINT pt1, fPOINT* opt) {

	opt->x = (pt1.x - pt0.x)*(float)0.5 + pt0.x;
	opt->y = (pt1.y - pt0.y)*(float)0.5 + pt0.y;
}

void xratf(fPOINT pt0, fPOINT pt1, fPOINT* opt) {

	opt->x = (pt1.x - pt0.x)*featherRatioLocal + pt0.x;
	opt->y = (pt1.y - pt0.y)*featherRatioLocal + pt0.y;
}

void fthrbfn(unsigned ind) {

	fPOINT	pnt0;
	fPOINT	pnt1;
	fPOINT	pnt0h;
	fPOINT	pnt0l;
	fPOINT	pnt1h;
	fPOINT	pnt1l;
	fPOINT	pntm;
	double	len;

	nurat();
	len = hypot(BSequence[ind + 1].y - BSequence[ind].y, BSequence[ind + 1].x - BSequence[ind].x);
	if (len < (featherMinStitch * 2))
	{
		featherRatio = 0.5;
		ratpnt(ind, ind + 1, &pnt0);
		ratpnt(ind + 3, ind + 2, &pnt1);
	}
	else
	{
		featherRatioLocal = featherMinStitch / len;
		duxrats(ind, ind + 1, &pnt0l);
		duxrats(ind + 3, ind + 2, &pnt1l);
		featherRatioLocal = 1 - featherRatioLocal;
		duxrats(ind, ind + 1, &pnt0h);
		duxrats(ind + 3, ind + 2, &pnt1h);
		featherRatioLocal = featherRatio;
		xratf(pnt0l, pnt0h, &pnt0);
		xratf(pnt1l, pnt1h, &pnt1);
	}
	midpnt(pnt0, pnt1, &pntm);
	xoseq(ind);
	OSequence[OutputIndex].x = pntm.x;
	OSequence[OutputIndex].y = pntm.y;
	OutputIndex++;
	xpfth(ind + 1);
	featherSequence[ActivePointIndex].x = pntm.x;
	featherSequence[ActivePointIndex].y = pntm.y;
	ActivePointIndex++;
}

void duoseq(unsigned ind) {

	OSequence[ind].x = BSequence[ind].x;
	OSequence[ind].y = BSequence[ind].y;
}

void fthdfn(unsigned ind) {

	double	len;
	fPOINT	mpnt;
	fPOINT	pt0;
	fPOINT	pt1;

	nurat();
	len = hypot(BSequence[ind + 1].y - BSequence[ind].y, BSequence[ind + 1].x - BSequence[ind].x);
	duoseq(ind);
	duoseq(ind + 1);
	if (len > featherMinStitch) {

		featherRatioLocal = 0.5;
		duxrats(ind + 1, ind, &mpnt);
		featherRatioLocal = featherMinStitch / len / 2;
		xratf(mpnt, OSequence[ind], &pt0);
		xratf(mpnt, OSequence[ind + 1], &pt1);
		featherRatioLocal = featherRatio;
		xratf(pt0, OSequence[ind], &OSequence[ind]);
		xratf(pt1, OSequence[ind + 1], &OSequence[ind + 1]);
	}
}

void fthrfn() {

	unsigned	ind, res;
	double		bakspac;

	PseudoRandomValue = FSED;
	fthvars();
	bakspac = StitchSpacing;
	StitchSpacing = SelectedForm->fillSpacing;
	satfil();
	BSequence[0].attribute = 0;
	BSequence[1].attribute = 1;
	if (!featherPhaseIndex)
		featherPhaseIndex = 1;
	ind = SequenceIndex / (featherPhaseIndex << 2);
	res = SequenceIndex % (featherPhaseIndex << 2);
	if (res > (featherPhaseIndex << 1))
		ind++;
	xtPosition = 0;
	xtStep = (float)4 / SequenceIndex*ind;
	xtPhase = (float)SequenceIndex / ind;
	xtRatio = (float)featherCountUp / featherPhaseIndex;
	xtUp = xtPhase*xtRatio;
	xtDown = xtPhase - xtUp;
	SelectedForm->fillType = FTHF;
	featherPhase = 1;
	BSequence[SequenceIndex].x = BSequence[SequenceIndex - 2].x;
	BSequence[SequenceIndex].y = BSequence[SequenceIndex - 2].y;
	BSequence[SequenceIndex].attribute = BSequence[SequenceIndex - 2].attribute;
	BSequence[SequenceIndex + 1].x = BSequence[SequenceIndex - 1].x;
	BSequence[SequenceIndex + 1].y = BSequence[SequenceIndex - 1].y;
	BSequence[SequenceIndex + 1].attribute = BSequence[SequenceIndex - 1].attribute;
	if (extendedAttribute&AT_FTHBLND) {

		OutputIndex = ActivePointIndex = 0;
		for (ind = 0; ind < SequenceIndex; ind++)
		{
			if (!BSequence[ind].attribute)
				fthrbfn(ind);
		}
	}
	else {

		if (SelectedForm->extendedAttribute&AT_FTHDWN) {

			for (ind = 0; ind <= SequenceIndex; ind++) {

				if (!BSequence[ind].attribute)
					fthdfn(ind);
			}
			ind--;
		}
		else {

			for (ind = 0; ind <= SequenceIndex; ind++) {

				if (BSequence[ind].attribute) {

					if (extendedAttribute&AT_FTHUP)
						fthfn(ind);
					else
						duoseq(ind);
				}
				else {

					if (extendedAttribute&AT_FTHUP)
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
	StitchSpacing = bakspac;
	SequenceIndex = OutputIndex;
}

void fritfil() {

	unsigned ind, ine;

	if (SequenceIndex) {

		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_FIL;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = TYPFRM;
		interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillColor;
		chkseq(0);
		interleaveSequenceIndex2++;
		if (SelectedForm->extendedAttribute&AT_FTHBLND&&~(SelectedForm->extendedAttribute&(AT_FTHUP | AT_FTHDWN)) != (AT_FTHUP | AT_FTHDWN)) {

			interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
			interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_FTH;
			interleaveSequenceIndices[interleaveSequenceIndex2].cod = FTHMSK;
			interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillInfo.feather.color;
			ine = ActivePointIndex - 1;
			for (ind = 0; ind < ActivePointIndex; ind++) {

				OSequence[ind].x = featherSequence[ine].x;
				OSequence[ind].y = featherSequence[ine].y;
				ine--;
			}
			SequenceIndex = ActivePointIndex;
			chkseq(0);
			interleaveSequenceIndex2++;
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
		SelectedForm->extendedAttribute &= !(AT_FTHUP | AT_FTHDWN | AT_FTHBLND);
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

	unsigned ind;

	if (filmsgs(FMM_FTH))
		return;
	if (SelectedFormCount) {

		savdo();
		for (ind = 0; ind < SelectedFormCount; ind++) {

			ClosestFormToCursor = SelectedFormList[ind];
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

void keynam(unsigned sed, TCHAR* nam) {

	unsigned ind;

	ind = 1;
	nam[0] = '\\';
	nam[1] = 's';
	PseudoRandomValue = sed;
	for (ind = 2; ind < 20; ind++)
		nam[ind] = psg() % 26 + 0x61;
	nam[ind] = 0;
}

ULARGE_INTEGER  tim2int(FILETIME tim) {

	ULARGE_INTEGER  op;

	op.LowPart = tim.dwLowDateTime;
	op.HighPart = tim.dwHighDateTime;
	return op;
}

int fil2crd(const char* p_filnam)
{
	STARTUPINFO			sinfo;
	PROCESS_INFORMATION	pinfo;
	int					erc = 0;
	char				cmd[_MAX_PATH * 2 + 1 + 4];

	strcpy_s(cmd, "\"");
	strcat_s(cmd, IniFile.p2cName);
	strcat_s(cmd, "\" \"");
	strcat_s(cmd, p_filnam);
	strcat_s(cmd, "\"");
	memset(&sinfo, 0, sizeof(STARTUPINFO));
	sinfo.cb = sizeof(STARTUPINFO);
	if (!CreateProcess(0, cmd, 0, 0, 0, NORMAL_PRIORITY_CLASS, 0, 0, &sinfo, &pinfo))
	{
		erc = GetLastError();
	}
	else {
		WaitForSingleObject(pinfo.hProcess, INFINITE);
	}
	CloseHandle(pinfo.hProcess);
	CloseHandle(pinfo.hThread);

	return erc;
}

BOOL chkp2cnam(char* nam) {

	HANDLE	hp2c;

	hp2c = CreateFile(nam, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
	CloseHandle(hp2c);
	if (hp2c == INVALID_HANDLE_VALUE)
		return 0;
	return 1;
}

void pes2crd() {

#define P2CBUFSIZ 256

	HKEY			hkey;
	unsigned long	typ, lul_Size;
	TCHAR			prgnam[_MAX_PATH];
	char			tfltr[] = "ComputerService (Lind2PC.exe)\0LinkP2C.exe\0\0";
	TCHAR			mbuf[P2CBUFSIZ];
	TCHAR			tbuf[P2CBUFSIZ];
	OPENFILENAME	tofn = {

	sizeof(OPENFILENAME),	//lStructsize
		ThredWindow,				//hwndOwner 
		ThredInstance,				//hInstance 
		tfltr,				//lpstrFilter 
		0,					//lpstrCustomFilter 
		0,					//nMaxCustFilter 
		0,					//nFilterIndex 
		prgnam,				//lpstrFile 
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
		0, KEY_READ, &hkey)) {

		lul_Size = _MAX_PATH;
		typ = REG_SZ;
		if (!RegQueryValueEx(hkey, "ProgramFilesDir", 0, &typ, (unsigned char*)prgnam, &lul_Size)) {

			strcat_s(prgnam, "\\Computerservice SSHSBV\\PES2Card\\LinkP2C.exe");
			if (!chkp2cnam(prgnam))
				*prgnam = 0;
		}
	}
	if (!*prgnam) {

		LoadString(ThredInstance, IDS_P2CMSG, mbuf, P2CBUFSIZ);
		LoadString(ThredInstance, IDS_P2CTITL, tbuf, P2CBUFSIZ);
		if (IDOK == MessageBox(ThredWindow, mbuf, tbuf, MB_OKCANCEL)) {

			if (GetOpenFileName(&tofn)) {

				if (!chkp2cnam(prgnam))
					return;
			}
			else
				return;
		}
		else
			return;
	}
	strcpy_s(IniFile.p2cName, prgnam);
	fil2crd(AuxName);
}

void sidlen(unsigned strt, unsigned fin, double* ilen, double* olen)
{
	*ilen += hypot(InsidePoints[fin].x - InsidePoints[strt].x, InsidePoints[fin].x - InsidePoints[strt].x);
	*olen += hypot(OutsidePoints[fin].x - OutsidePoints[strt].x, OutsidePoints[fin].x - OutsidePoints[strt].x);
}

fPOINT* insid()
{
	unsigned ind;

	satout(fabs(SelectedForm->underlayIndent));
	if (SelectedForm->underlayIndent > 0)
	{
		for (ind = 0; ind < SideCount; ind++)
		{
			if (!cisin(InsidePoints[ind].x, InsidePoints[ind].y))
			{
				InsidePoints[ind].x = CurrentFormVertices[ind].x;
				InsidePoints[ind].y = CurrentFormVertices[ind].y;
			}
		}
		return InsidePoints;
	}
	else
		return OutsidePoints;
}

void delwlk(unsigned cod)
{
	unsigned	ind, ine;
	fPOINTATTR*	histch;
	BOOL		flg;

	ine = 0;
	flg = 1;
	histch = &StitchBuffer[MAXSEQ];
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		if ((StitchBuffer[ind].attribute&WLKFMSK) != cod)
		{
			moveStitch(&histch[ine], &StitchBuffer[ind]);
			ine++;
		}
		else
		{
			if (flg)
			{
				flg = 0;
			}
		}
	}
	mvstchs(0, MAXSEQ, ine);
	PCSHeader.stitchCount = ine;
}

void chkuseq()
{
#if BUGBAK
	unsigned index;

	for (index = 0; index < OutputIndex; index++)
	{
		interleaveSequence[index].x = OSequence[index].x;
		interleaveSequence[index].y = OSequence[index].y;
	}
	interleaveSequenceIndex = index;
	interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->UnderlayColor;
#else

	unsigned	ind, ine, cnt;
	double		len;
	fPOINT		dif;
	fPOINT		loc;
	fPOINT		stp;
	double		ulen;

	if (OutputIndex)
	{
		if (SelectedForm->underlayStitchLen < MINWLK)
			SelectedForm->underlayStitchLen = (float)MINWLK;
		if (SelectedForm->underlayStitchLen > MAXWLK)
			SelectedForm->underlayStitchLen = MAXWLK;
		ulen = SelectedForm->underlayStitchLen;
		for (ind = 0; ind < OutputIndex - 1; ind++)
		{
			dif.x = OSequence[ind + 1].x - OSequence[ind].x;
			dif.y = OSequence[ind + 1].y - OSequence[ind].y;
			len = hypot(dif.x, dif.y);
			cnt = floor(len / ulen);
			if (cnt)
			{
				stp.x = dif.x / cnt;
				stp.y = dif.y / cnt;
				loc.x = OSequence[ind].x;
				loc.y = OSequence[ind].y;
				for (ine = 0; ine < cnt; ine++)
				{
					interleaveSequence[interleaveSequenceIndex].x = loc.x;
					interleaveSequence[interleaveSequenceIndex].y = loc.y;
					interleaveSequenceIndex++;
					loc.x += stp.x;
					loc.y += stp.y;
				}
			}
			else
			{
				interleaveSequence[interleaveSequenceIndex].x = OSequence[ind].x;
				interleaveSequence[interleaveSequenceIndex].y = OSequence[ind].y;
				interleaveSequenceIndex++;
			}
		}
		interleaveSequence[interleaveSequenceIndex].x = OSequence[ind].x;
		interleaveSequence[interleaveSequenceIndex].y = OSequence[ind].y;
		interleaveSequenceIndex++;
		interleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->underlayColor;
	}
#endif
}

void ritwlk()
{
	if (OutputIndex)
	{
		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_FIL;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = WLKMSK;
		chkuseq();
		interleaveSequenceIndex2++;
	}
}

void ritcwlk()
{
	if (OutputIndex) {

		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_FIL;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = CWLKMSK;
		chkuseq();
		interleaveSequenceIndex2++;
	}
}

unsigned gucon(fPOINT pstrt, fPOINT pfin, unsigned dst, unsigned cod)
{
	double		len;
	unsigned	strt, fin, cnt, ifin;
	fPOINT*		ipnt;
	unsigned	up, dwn, ind, ine;
	fPOINT		loc, stp, dif;

	len = hypot(pfin.x - pstrt.x, pfin.y - pstrt.y);
	if (len < 5)
		return 0;
	strt = closflt(pstrt.x, pstrt.y);
	fin = closflt(pfin.x, pfin.y);
	if (strt == fin)
		return 0;
	//	if(chkMap(ISEND))
	//		ipnt=vertices;
	//	else
	ipnt = insid();
	up = dwn = strt;
gulab:
	if (up == fin)
	{
		rstMap(WLKDIR);
		goto gulabx;
	}
	if (dwn == fin)
	{
		setMap(WLKDIR);
		goto gulabx;
	}
	up = nxt(up);
	dwn = prv(dwn);
	goto gulab;
gulabx:;
	ind = dst;
	while (strt != fin)
	{
		StitchBuffer[ind].x = ipnt[strt].x;
		StitchBuffer[ind].y = ipnt[strt].y;
		StitchBuffer[ind].attribute = cod;
		if (ind)
		{
			if (StitchBuffer[ind - 1].x != StitchBuffer[ind].x || StitchBuffer[ind - 1].y != StitchBuffer[ind].y)
				ind++;
		}
		else
			ind++;
		if (chkMap(WLKDIR))
			ifin = prv(strt);
		else
			ifin = nxt(strt);
		dif.x = ipnt[ifin].x - ipnt[strt].x;
		dif.y = ipnt[ifin].y - ipnt[strt].y;
		len = hypot(dif.x, dif.y);
		cnt = len / SelectedForm->lengthOrCount.stitchLength;
		if (cnt > 1)
		{
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			loc.x = ipnt[strt].x + stp.x;
			loc.y = ipnt[strt].y + stp.y;
			for (ine = 0; ine < cnt - 1; ine++)
			{
				StitchBuffer[ind].x = loc.x;
				StitchBuffer[ind].y = loc.y;
				StitchBuffer[ind].attribute = cod;
				ind++;
				loc.x += stp.x;
				loc.y += stp.y;
			}
		}
		if (chkMap(WLKDIR))
			strt = prv(strt);
		else
			strt = nxt(strt);
	}
	StitchBuffer[ind].x = ipnt[strt].x;
	StitchBuffer[ind].y = ipnt[strt].y;
	StitchBuffer[ind].attribute = cod;
	ind++;
	return ind - dst;
}

void fnwlk(unsigned find)
{
	unsigned	strt, cnt;
	fPOINT*		wpnt;

	fvars(find);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (SelectedForm->extendedAttribute&AT_STRT&&SelectedForm->type != FRMLINE)
		strt = SelectedForm->fillStart;
	else
		strt = 0;
	cnt = SideCount;
	if (SelectedForm->type != FRMLINE)
		cnt++;
	wpnt = insid();
	OutputIndex = 0;
	while (cnt)
	{
		OSequence[OutputIndex].x = wpnt[strt].x;
		OSequence[OutputIndex].y = wpnt[strt].y;
		strt = nxt(strt);
		OutputIndex++;
		cnt--;
	}
	ritwlk();
}

void fnuang() {

	unsigned	ind;

	frmcpy(&AngledForm, &FormList[ClosestFormToCursor]);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	for (ind = 0; ind < AngledForm.sides; ind++) {

		AngledForm.vertices[ind].x = underlayVertices[ind].x;
		AngledForm.vertices[ind].y = underlayVertices[ind].y;
		rotflt(&AngledForm.vertices[ind]);
	}
	SelectedForm = &AngledForm;
	fnvrt();
	fvars(ClosestFormToCursor);
}

void ritund()
{
	if (SequenceIndex) {

		interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
		interleaveSequenceIndices[interleaveSequenceIndex2].seq = I_FIL;
		interleaveSequenceIndices[interleaveSequenceIndex2].cod = UNDMSK;
		chkuseq();
		interleaveSequenceIndex2++;
	}
}

void undclp()
{
	ClipRectSize.cx = ClipRect.bottom = ClipRect.left = ClipRect.right = ClipBuffer[0].x = ClipBuffer[1].x = ClipBuffer[0].y = 0;
	ClipRectSize.cy = ClipRect.top = ClipBuffer[1].y = SelectedForm->underlayStitchLen;
	ClipStitchCount = 2;
}

void fnund(unsigned find)
{
	float baksiz;
	underlayVertices = insid();
	baksiz = UserStitchLength;
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
	UserStitchLength = baksiz;
}

void fncwlk()
{
	unsigned	ind, ine, strt, fin;
	SATCON*		sac;

	OutputIndex = 0;
	SelectedForm->extendedAttribute |= AT_CWLK;
	if (SelectedForm->satinGuideCount)
	{
		if (SelectedForm->wordParam)
		{
			ine = SelectedForm->wordParam;
			OSequence[OutputIndex].x = midl(CurrentFormVertices[ine].x, CurrentFormVertices[ine + 1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[ine].y, CurrentFormVertices[ine + 1].y);
			OutputIndex++;
		}
		sac = SelectedForm->satinOrAngle.sac;
		for (ind = SelectedForm->satinGuideCount; ind != 0; ind--)
		{
			OSequence[OutputIndex].x = midl(CurrentFormVertices[sac[ind - 1].finish].x, CurrentFormVertices[sac[ind - 1].start].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[sac[ind - 1].finish].y, CurrentFormVertices[sac[ind - 1].start].y);
			OutputIndex++;
		}
		if (SelectedForm->attribute&FRMEND)
		{
			OSequence[OutputIndex].x = midl(CurrentFormVertices[0].x, CurrentFormVertices[1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[0].y, CurrentFormVertices[1].y);
			OutputIndex++;
		}
	}
	else
	{
		if (SelectedForm->extendedAttribute&AT_STRT)
			strt = SelectedForm->fillStart;
		else
			strt = 0;
		OSequence[OutputIndex].x = CurrentFormVertices[strt].x;
		OSequence[OutputIndex].y = CurrentFormVertices[strt].y;
		OutputIndex++;
		fin = prv(strt);
		strt = nxt(strt);
		for (ind = 1; ind < (unsigned)SideCount >> 1; ind++)
		{
			OSequence[OutputIndex].x = midl(CurrentFormVertices[fin].x, CurrentFormVertices[strt].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[fin].y, CurrentFormVertices[strt].y);
			if (cisin(OSequence[OutputIndex].x, OSequence[OutputIndex].y))
				OutputIndex++;
			strt = nxt(strt);
			fin = prv(fin);
		}
		OSequence[OutputIndex].x = CurrentFormVertices[strt].x;
		OSequence[OutputIndex].y = CurrentFormVertices[strt].y;
		OutputIndex++;
	}
	ritcwlk();
}

BOOL CALLBACK fthdefprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buf[HBUFSIZ];
	TCHAR		buf1[HBUFSIZ];
	unsigned	ind, stat, l_xat;

	switch (umsg) {

	case WM_INITDIALOG:

		l_xat = IniFile.featherType;
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
		for (ind = 0; ind < 6; ind++) {

			LoadString(ThredInstance, IDS_FTH0 + ind, buf, HBUFSIZ);
			SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, (long)buf);
		}
		SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, IniFile.featherFillType - 1, 0);
		if (l_xat&AT_FTHBLND)
			stat = BST_CHECKED;
		else
			stat = BST_UNCHECKED;
		CheckDlgButton(hwndlg, IDC_FDBLND, stat);
		if (l_xat&AT_FTHUP)
			stat = BST_CHECKED;
		else
			stat = BST_UNCHECKED;
		CheckDlgButton(hwndlg, IDC_FDUP, stat);
		if (l_xat&AT_FTHDWN)
			stat = BST_CHECKED;
		else
			stat = BST_UNCHECKED;
		CheckDlgButton(hwndlg, IDC_FDWN, stat);
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
			if (IsDlgButtonChecked(hwndlg, IDC_FDWN))
				IniFile.featherType |= AT_FTHDWN;
			GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf, HBUFSIZ);
			IniFile.featherFillType = FDEFTYP;
			for (ind = 0; ind < 6; ind++) {

				LoadString(ThredInstance, IDS_FTH0 + ind, buf1, HBUFSIZ);
				if (!strcmp(buf, buf1)) {

					IniFile.featherFillType = ind + 1;
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

	DialogBox(ThredInstance, MAKEINTRESOURCE(IDD_FETHDEF), ThredWindow, (DLGPROC)fthdefprc);
}

void srtcol() {

	unsigned		hst[16];
	unsigned		ind, ine, tmp;
	fPOINTATTR*		phi;

	FillMemory(&hst, 64, 0);
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
		hst[StitchBuffer[ind].attribute&COLMSK]++;
	ine = 0;
	for (ind = 0; ind < 16; ind++) {

		tmp = hst[ind];
		hst[ind] = ine;
		ine += tmp;
	}
	phi = &StitchBuffer[MAXSEQ];
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
		moveStitch(&phi[hst[StitchBuffer[ind].attribute&COLMSK]++], &StitchBuffer[ind]);
	MoveMemory(&StitchBuffer, phi, sizeof(fPOINTATTR)*PCSHeader.stitchCount);
}

void dubit(unsigned bit)
{
	unsigned cod;

	savdo();
	setMap(WASDO);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (!(SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) && bit&(AT_UND | AT_WALK | AT_CWLK))
	{
		if (SelectedForm->fillType)
			SelectedForm->underlayColor = SelectedForm->fillColor;
		else
			SelectedForm->underlayColor = ActiveColor;
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	}
	if (!(SelectedForm->extendedAttribute&AT_UND) && bit&AT_UND)
	{
		SelectedForm->underlayStitchAngle = IniFile.underlayAngle;
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	}
	cod = SelectedForm->extendedAttribute&bit;
	if (cod)
		SelectedForm->extendedAttribute &= (~bit);
	else
		SelectedForm->extendedAttribute |= bit;
	refil();
	coltab();
	setMap(RESTCH);
}

void setuang()
{
	setMap(GTUANG);
	msgflt(IDS_UANG, IniFile.underlayAngle / PI * 180);
}

void setuspac()
{
	setMap(GTUSPAC);
	msgflt(IDS_USPAC, IniFile.underlaySpacing / PFGRAN);
}

void setwlkind()
{
	setMap(GTWLKIND);
	msgflt(IDS_WLKIND, IniFile.underlayIndent / PFGRAN);
}

void setulen()
{
	setMap(GTWLKLEN);
	msgflt(IDS_WLKLEN, IniFile.underlayStitchLen / PFGRAN);
}

void chkcwlk()
{
	if (SelectedForm->extendedAttribute&AT_CWLK)
		fncwlk();
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
}

void chkwlk()
{
	if (SelectedForm->extendedAttribute&AT_WALK)
		fnwlk(ClosestFormToCursor);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
}

void chkund()
{
	if (SelectedForm->extendedAttribute&AT_UND)
		fnund(ClosestFormToCursor);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
}

void selalfrm()
{
	for (SelectedFormCount = 0; SelectedFormCount < FormIndex; SelectedFormCount++)
		SelectedFormList[SelectedFormCount] = SelectedFormCount;
	setMap(RESTCH);
}

void chkdaz()
{
	float	len;

	if (!IniFile.daisyPetalPoints)
		IniFile.daisyPetalPoints = 1;
	if (!IniFile.daisyInnerCount)
		IniFile.daisyInnerCount = 1;
	if (!IniFile.daisyPetalCount)
		IniFile.daisyPetalCount = 1;
	len = IniFile.daisyDiameter*0.999;
	if (IniFile.daisyHeartCount > IniFile.daisyPetalPoints)
		IniFile.daisyHeartCount = IniFile.daisyPetalPoints;
}

void dazdef()
{
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

BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);

	TCHAR		buf[HBUFSIZ];
	TCHAR		buf1[HBUFSIZ];
	unsigned	ind;

	switch (umsg) {

	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	reinit:;
		chkdaz();
		sprintf_s(buf, sizeof(buf), "%d", IniFile.daisyPetalPoints);
		SetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buf);
		sprintf_s(buf, sizeof(buf), "%d", IniFile.daisyHeartCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", IniFile.daisyDiameter);
		SetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", IniFile.daisyHoleDiameter);
		SetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buf);
		sprintf_s(buf, sizeof(buf), "%d", IniFile.daisyInnerCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buf);
		sprintf_s(buf, sizeof(buf), "%d", IniFile.daisyPetalCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", IniFile.daisyPetalLen);
		SetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buf);
		CheckDlgButton(hwndlg, IDC_HOLE, chku(DAZHOL));
		CheckDlgButton(hwndlg, IDC_DLIN, chku(DAZD));
		for (ind = 0; ind < 6; ind++)
		{
			LoadString(ThredInstance, daztab[ind], buf, HBUFSIZ);
			SendMessage(GetDlgItem(hwndlg, IDC_DAZTYP), CB_ADDSTRING, 0, (long)buf);
		}
		SendMessage(GetDlgItem(hwndlg, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {

		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buf, HBUFSIZ);
			IniFile.daisyPetalPoints = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buf, HBUFSIZ);
			IniFile.daisyHeartCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buf, HBUFSIZ);
			IniFile.daisyDiameter = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buf, HBUFSIZ);
			IniFile.daisyHoleDiameter = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buf, HBUFSIZ);
			IniFile.daisyInnerCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buf, HBUFSIZ);
			IniFile.daisyPetalCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buf, HBUFSIZ);
			IniFile.daisyPetalLen = atof(buf);
			if (IsDlgButtonChecked(hwndlg, IDC_HOLE))
				setu(DAZHOL);
			else
				rstu(DAZHOL);
			if (IsDlgButtonChecked(hwndlg, IDC_DLIN))
				setu(DAZD);
			else
				rstu(DAZD);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZTYP), buf, HBUFSIZ);
			for (ind = 0; ind < 6; ind++)
			{
				LoadString(ThredInstance, daztab[ind], buf1, HBUFSIZ);
				if (!strcmp(buf, buf1))
				{
					IniFile.daisyBorderType = ind;
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

	double		dang;
	double		pang;
	double		dangp;
	double		pangp;
	unsigned	bcnt, ind, ine, inf, dtyp, dcnt, cnt2, fref, pcnt;
	double		dlen = 0;
	double		len;
	double		elen;
	double		ilen;
	double		drat;
	float		maxcor;
	fPOINT		ref;

	unmsg();
	if (!DialogBox(ThredInstance, MAKEINTRESOURCE(IDD_DASY), ThredWindow, (DLGPROC)dasyproc))
	{
		rstMap(FORMIN);
		return;
	}
	IniFile.daisyPetalCount = IniFile.daisyPetalCount;
	len = IniFile.daisyDiameter;
	elen = IniFile.daisyPetalLen;
	ilen = IniFile.daisyHoleDiameter;
	dtyp = IniFile.daisyBorderType;
	ref.x = midl(ZoomRect.right, ZoomRect.left);
	ref.y = midl(ZoomRect.top, ZoomRect.bottom);
	SelectedForm = &FormList[FormIndex];
	ClosestFormToCursor = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	cnt2 = IniFile.daisyPetalPoints >> 1;
	maxcor = ZoomRect.right - ZoomRect.left;
	drat = ZoomRect.top - ZoomRect.bottom;
	if (drat > maxcor)
		maxcor = drat;
	maxcor /= 6;
	drat = (double)maxcor / (len + elen);
	len *= drat;
	elen *= drat;
	ilen *= drat;
	SelectedForm->type = FRMFPOLY;
	inf = fref = 0;
	if (chku(DAZHOL))
	{
		pang = PI2;
		bcnt = IniFile.daisyPetalCount*IniFile.daisyInnerCount;
		dang = PI2 / bcnt;
		CurrentFormVertices[inf].x = ref.x + len*cos(pang);
		CurrentFormVertices[inf].y = ref.y + len*sin(pang);
		inf++;
		for (ind = 0; ind < bcnt + 1; ind++)
		{
			CurrentFormVertices[inf].x = ref.x + ilen*cos(pang);
			CurrentFormVertices[inf].y = ref.y + ilen*sin(pang);
			inf++;
			pang -= dang;
		}
		fref = inf;
	}
	pang = 0;
	bcnt = IniFile.daisyPetalCount*IniFile.daisyPetalPoints;
	pcnt = IniFile.daisyPetalPoints;
	if (dtyp == DHART)
	{
		pcnt = (IniFile.daisyHeartCount + 1) << 1;
		bcnt = IniFile.daisyPetalCount*pcnt;
	}
	dang = PI2 / bcnt;
	dangp = PI / IniFile.daisyPetalPoints;
	if (chku(DAZD))
	{
		SelectedForm->satinGuideCount = IniFile.daisyPetalCount - 1;
		SelectedForm->wordParam = IniFile.daisyPetalCount*IniFile.daisyInnerCount + 1;
		SelectedForm->satinOrAngle.sac = adsatk(IniFile.daisyPetalCount - 1);
	}
	for (ind = 0; ind < IniFile.daisyPetalCount; ind++)
	{
		pangp = 0;
		PseudoRandomValue = SEED;
		for (ine = 0; ine < pcnt; ine++)
		{
			switch (dtyp)
			{
			case DSIN:

				dlen = len + sin(pangp)*elen;
				pangp += dangp;
				break;

			case DRAMP:

				dlen = len + (double)ine / IniFile.daisyPetalPoints*elen;
				break;

			case DSAW:

				if (ine > cnt2)
					dcnt = IniFile.daisyPetalPoints - ine;
				else
					dcnt = ine;
				dlen = len + (double)dcnt / IniFile.daisyPetalPoints*elen;
				break;

			case DRAG:

				dlen = len + (double)(psg() % IniFile.daisyPetalPoints) / IniFile.daisyPetalPoints*elen;
				break;

			case DCOG:

				dlen = len;
				if (ine > cnt2)
					dlen += elen;
				break;

			case DHART:

				dlen = len + sin(pangp)*elen;
				if (ine > IniFile.daisyHeartCount)
					pangp -= dangp;
				else
					pangp += dangp;
				break;
			}
			CurrentFormVertices[inf].x = ref.x + cos(pang)*dlen;
			CurrentFormVertices[inf].y = ref.y + sin(pang)*dlen;
			inf++;
			pang += dang;
			if (chku(DAZD) && ind != IniFile.daisyPetalCount - 1)
			{
				SelectedForm->satinOrAngle.sac[ind].start = (IniFile.daisyPetalCount - ind - 1)*IniFile.daisyInnerCount + 1;
				SelectedForm->satinOrAngle.sac[ind].finish = inf;
			}
		}
	}
	if (chku(DAZHOL))
	{
		CurrentFormVertices[fref - 1].y += (float)0.01;
		CurrentFormVertices[fref].y += (float)0.01;
	}
	SelectedForm->sides = inf;
	if (chku(DAZD))
	{
		SelectedForm->type = SAT;
		SelectedForm->attribute = 1;
	}
	FormVertexIndex += inf;
	setMap(INIT);
	frmout(FormIndex);
	for (ind = 0; ind < inf; ind++) {

		CurrentFormVertices[ind].x -= SelectedForm->rectangle.left;
		CurrentFormVertices[ind].y -= SelectedForm->rectangle.bottom;
	}
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount = inf + 1;
	setMap(POLIMOV);
	setmfrm();
	mdufrm();
}

void durec(OREC* prec)
{
	unsigned	at;
	fPOINTATTR*	pstch;

	pstch = &StitchBuffer[prec->start];
	prec->typ = ftyps[dutyp(pstch->attribute)];
	at = pstch->attribute&SRTMSK;
	prec->col = at & 0xf;
	prec->frm = (at&FRMSK) >> FRMSHFT;
}


OREC*	recref(const void* arg)
{
#if  __UseASM__
	_asm
	{
		mov		eax, arg
		mov		eax, [eax]
	}
#else
	// Check translation
	return *(OREC **)arg;
#endif
}

int recmp(const void *arg1, const void *arg2)
{
	OREC* pa1;
	OREC* pa2;

	pa1 = recref((const void*)arg1);
	pa2 = recref((const void*)arg2);
	if (colorOrder[pa1->col] == colorOrder[pa2->col])
	{
		if (pa1->frm == pa2->frm)
		{
			if (pa1->typ == pa2->typ)
				return (int)pa1->start - pa2->start;
			else
				return (int)pa1->typ - pa2->typ;
		}
		else
			return (int)pa1->frm - pa2->frm;
	}
	return (int)colorOrder[pa1->col] - colorOrder[pa2->col];
}

int refcmp(const void *arg1, const void *arg2)
{
	OREC* pa1;
	OREC* pa2;

	pa1 = recref((const void*)arg1);
	pa2 = recref((const void*)arg2);
	if (pa1->frm == pa2->frm)
		return (int)pa1->typ - pa2->typ;
	return (int)pa1->frm - pa2->frm;
}

double srtlen(fPOINTATTR* pnt, OREC* prec, unsigned swtch)
{
	fPOINTATTR* tpnt;

	if (swtch)
		tpnt = prec->epnt;
	else
		tpnt = prec->spnt;
	return hypot(pnt->x - tpnt->x, pnt->y - tpnt->y);
}

BOOL chkrdun(SRTREC* psrec)
{
	unsigned ind;

	for (ind = psrec->start; ind < psrec->finish; ind++)
	{
		if (precs[ind]->otyp == formFillCounter[precs[ind]->frm])
			return 1;
	}
	return 0;
}

double precjmps(SRTREC* psrec)
{
	unsigned		totjmps;
	double			len;
	double			minlen;
	unsigned		ind, loc, loci;
	fPOINTATTR*		ploc;
	BOOL			locdir;

	formFillCounter = (unsigned*)&OSequence;
	FillMemory(&OSequence, (ActivePointIndex + 2) << 2, 0);
	loc = psrec->loc;
	locdir = psrec->dir;
	totjmps = 0;
	while (chkrdun(psrec))
	{
		minlen = 1e9;
		if (locdir)
			loci = precs[loc]->finish;
		else
			loci = precs[loc]->start;
		ploc = &StitchBuffer[loci];
		for (ind = psrec->start; ind < psrec->finish; ind++)
		{
			if (precs[ind]->otyp == formFillCounter[precs[ind]->frm])
			{
				len = hypot(precs[ind]->spnt->x - ploc->x, precs[ind]->spnt->y - ploc->y);
				if (len < minlen)
				{
					minlen = len;
					locdir = 0;
					loc = ind;
				}
				len = hypot(precs[ind]->epnt->x - ploc->x, precs[ind]->epnt->y - ploc->y);
				if (len < minlen)
				{
					minlen = len;
					locdir = 1;
					loc = ind;
				}
			}
		}
		if (minlen > 9 * PFGRAN)
			totjmps++;
		formFillCounter[precs[loc]->frm]++;
		if (chkMap(DUSRT))
		{
			if (locdir)
			{
				if (precs[loc]->start)
				{
					for (ind = precs[loc]->finish - 1; ind >= precs[loc]->start; ind--)
						moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[ind]);
				}
				else
				{
					ind = precs[loc]->finish;
					while (ind)
						moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[--ind]);
				}
			}
			else
			{
				for (ind = precs[loc]->start; ind < precs[loc]->finish; ind++)
					moveStitch(&TempStitchBuffer[OutputIndex++], &StitchBuffer[ind]);
			}
		}
	}
	return totjmps;
}

unsigned duprecs(SRTREC* prec)
{
	unsigned jmps0;
	unsigned jmps1;

	prec->dir = 0;
	jmps0 = precjmps(prec);
	prec->dir = 1;
	jmps1 = precjmps(prec);
	if (jmps0 < jmps1)
	{
		prec->dir = 0;
		return jmps0;
	}
	prec->dir = 1;
	return jmps1;
}

#ifdef _DEBUG

void dmprec(OREC** recs, unsigned cnt)
{
	unsigned ind;

	for (ind = 0; ind < cnt; ind++)
	{
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), "%4d off: %4d at: %08x frm: %4d typ: %d col: %2d st: %5d fin: %5d\n",
			ind,
			recs[ind] - (OREC*)BSequence,
			StitchBuffer[recs[ind]->start].attribute,
			recs[ind]->frm,
			recs[ind]->typ,
			recs[ind]->col,
			recs[ind]->start,
			recs[ind]->finish);
		OutputDebugString(MsgBuffer);
	}
}
#endif

BOOL srtchk(OREC** pfrec, unsigned cnt, unsigned* badfrm)
{
	unsigned ind, frm, hcol;
	FRMHED* fp;

	frm = pfrec[0]->frm;
	hcol = pfrec[0]->col;
	for (ind = 1; ind < cnt; ind++)
	{
		if (pfrec[ind]->frm == frm)
		{
			if (colorOrder[pfrec[ind]->col] < colorOrder[hcol])
			{
				fp = &FormList[frm];
				if (fp->fillType == FTHF&&fp->extendedAttribute&AT_FTHBLND&&pfrec[ind]->col == fp->fillColor)
					continue;
				*badfrm = ind;
				return 0;
			}
			else
				hcol = pfrec[ind]->col;
		}
		else
		{
			hcol = pfrec[ind]->col;
			frm = pfrec[ind]->frm;
		}
	}
	return 1;
}

void fsort()
{
	unsigned		ind, ine, pind, at, cfrm, rind, mind = 0, mindir = 0;
	unsigned		typcnt, jmps, minjmps;
	OREC*			recs;
	RANGE*			prngs;
	unsigned*		l_frmcnts;
	SRTREC			srec;
	FILETIME		ftim;
	ULARGE_INTEGER	stim;
	ULARGE_INTEGER	ntim;

	savdo();
	recs = (OREC*)&BSequence;
	recs->start = 0;
	recs->spnt = StitchBuffer;
	at = StitchBuffer->attribute&SRTMSK;
	rind = 0;
	ActivePointIndex = FormIndex;
	colorOrder[UnderlayColor] = 0;
	for (ind = 0; ind < 16; ind++) {

		if (ind != UnderlayColor)
			colorOrder[ind] = ind + 1;
	}
	for (ind = 1; ind < PCSHeader.stitchCount; ind++)
	{
		if ((StitchBuffer[ind].attribute&SRTMSK) != at)
		{
			recs[rind].finish = ind;
			recs[rind].epnt = &StitchBuffer[ind - 1];
			rind++;
			recs[rind].start = ind;
			recs[rind].spnt = &StitchBuffer[ind];
			at = StitchBuffer[ind].attribute&SRTMSK;
		}
	}
	recs[rind].epnt = &StitchBuffer[PCSHeader.stitchCount - 1];
	recs[rind].finish = PCSHeader.stitchCount;
	rind++;
	precs = (OREC**)&recs[rind];
	pfrecs = (OREC**)&recs[rind << 1];
	for (ind = 0; ind < rind; ind++)
	{
		durec(&recs[ind]);
		precs[ind] = &recs[ind];
		pfrecs[ind] = &recs[ind];
	}
	qsort((void*)precs, rind, 4, recmp);
	qsort((void*)pfrecs, rind, 4, refcmp);
#ifdef _DEBUG
	//	dmprec(precs,rind);
#endif
	if (srtchk(pfrecs, rind, &ine))
	{
		prngs = (RANGE*)&precs[rind];
		prngs[0].start = 0;
		at = precs[0]->col;
		cfrm = 0xffffffff;
		typcnt = 0;
		pind = 0;
		l_frmcnts = (unsigned*)&OSequence;
		for (ind = 0; ind < rind; ind++)
		{
			if (at != precs[ind]->col)
			{
				prngs[pind].finish = ind;
				pind++;
				prngs[pind].start = ind;
				at = precs[ind]->col;
				cfrm = precs[ind]->frm;
				typcnt = 0;
				goto srtskp;
			}
			if (precs[ind]->frm == cfrm)
				typcnt++;
			else
			{
				typcnt = 0;
				cfrm = precs[ind]->frm;
			}
		srtskp:;
			precs[ind]->otyp = typcnt;
		}
		prngs[pind].finish = rind;
		pind++;
		TempStitchBuffer = &StitchBuffer[MAXSEQ];
		OutputIndex = 0;
		for (ind = 0; ind < pind; ind++)
		{
			rstMap(DUSRT);
			srec.start = prngs[ind].start;
			srec.finish = prngs[ind].finish;
			srec.cnt = srec.finish - srec.start;
			minjmps = 0xffffffff;
			GetSystemTimeAsFileTime(&ftim);
			stim = tim2int(ftim);
			for (ine = srec.start; ine < srec.finish; ine++)
			{
				srec.loc = ine;
				if (!precs[ine]->otyp)
				{
					jmps = duprecs(&srec);
					if (jmps < minjmps)
					{
						minjmps = jmps;
						mind = ine;
						mindir = srec.dir;
					}
				}
				GetSystemTimeAsFileTime(&ftim);
				ntim = tim2int(ftim);
				if (ntim.QuadPart - stim.QuadPart > SRTIM)
					break;
			}
			setMap(DUSRT);
			srec.loc = mind;
			srec.dir = mindir;
			precjmps(&srec);
		}
		mvstchs(0, MAXSEQ, OutputIndex);
		PCSHeader.stitchCount = OutputIndex;
		coltab();
		setMap(RESTCH);
	}
	else
	{
		LoadString(ThredInstance, IDS_SRTER, HelpBuffer, HBUFSIZ);
		sprintf_s(MsgBuffer, sizeof(MsgBuffer), HelpBuffer, pfrecs[ine]->frm);
		shoMsg(MsgBuffer);
	}
}

unsigned dutyp(unsigned tat)
{
#if  __UseASM__
	_asm
	{
		xor		eax, eax
		mov		ebx, tat
		and		ebx, SRTYPMSK
		bsr		eax, ebx
		je		short dutypx
		sub		al, 18
		cmp		al, 12
		jne		short dutypx
		test	ebx, 0x20000000
		je		short dutypx
		mov		al, 1
		dutypx:		and		eax, 0xf
	}
#else
	//correct
	char result;
	DWORD bit;
	unsigned lat = tat & SRTYPMSK;

	_BitScanReverse(&bit, lat);

	if (bit == 0)
		return 0;

	result = ((bit & 0xff) - 18);

	if ((result != 12) || ((lat & TYPATMSK) == 0))
		return result & 0xf;

	return 1;
#endif
}

void filim(FRMLIM* flim, unsigned * lmap)
{
	unsigned	ind, fcod, at, typ;

	fcod = ClosestFormToCursor << FRMSHFT;
	*lmap = 0;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		at = StitchBuffer[ind].attribute;
		if (fcod == (at&FRMSK))
		{
			typ = ftyps[dutyp(at)];
			switch (typ)
			{
			case 1:	//applique

				if (!(*lmap&M_CWLK))
					flim->apstrt = ind;
				*lmap |= M_AP;
				flim->apend = ind;
				break;

			case 2:	//center walk

				if (!(*lmap&M_CWLK))
					flim->cstrt = ind;
				*lmap |= M_CWLK;
				flim->cend = ind;
				break;

			case 3:	//edge walk

				if (!(*lmap&M_WALK))
					flim->wstrt = ind;
				*lmap |= M_WALK;
				flim->wend = ind;
				break;

			case 4:	//underlay

				if (!(*lmap&M_UND))
					flim->ustrt = ind;
				*lmap |= M_UND;
				flim->uend = ind;
				break;

			case 5:	//fill

				if (!(*lmap&M_FIL))
					flim->fstrt = ind;
				*lmap |= M_FIL;
				flim->fend = ind;
				break;

			case 6:	//border

				if (SelectedForm->fillColor == SelectedForm->borderColor)
				{
					if (!(*lmap&M_BRD))
						flim->bstrt = ind;
					*lmap |= M_BRD;
					flim->bend = ind;
				}
				break;
			}
		}
	}
}

unsigned frstmap(unsigned* map)
{
#if  __UseASM__
	_asm
	{
		xor		eax, eax
		mov		ecx, map
		mov		ebx, [ecx]
		bsf		eax, ebx
		je		short fmapx
		btr		ebx, eax
		mov[ecx], ebx
		fmapx :
	}
#else
	//check translation
	DWORD bit;

	_BitScanForward(&bit, *map);

	if (bit == 0)
		return 0;
	else {
		_bittestandreset((long *)map, bit);
		return bit;
	}
#endif
}

#ifdef _DEBUG

typedef struct _atfld {

	unsigned	col;
	unsigned	frm;
	unsigned	typ;
	unsigned	lay;
	unsigned	usr;
}ATFLD;

void duatf(unsigned ind)
{
	char		abuf[256];
	unsigned	at;
	ATFLD		atf;

	at = StitchBuffer[ind].attribute;
	atf.col = at&COLMSK;
	atf.frm = (at >> FRMSHFT)&FRMSK;
	atf.typ = ftyps[dutyp(at)];
	atf.lay = (at >> LAYSHFT) & 7;
	if (at & 0x80000000)
		atf.usr = 1;
	else
		atf.usr = 0;
	sprintf_s(abuf, sizeof(abuf), "%5d: col: %2d frm: %5d typ: %2d, lay: %1d: usr: %1d at: %08x\n",
		ind,
		atf.col,
		atf.frm,
		atf.typ,
		atf.lay,
		atf.usr,
		at);
	OutputDebugString(abuf);
}

void dmpat()
{
	unsigned ind, at;

	duatf(0);
	at = StitchBuffer[0].attribute;
	for (ind = 1; ind < PCSHeader.stitchCount; ind++)
	{
		if (at != StitchBuffer[ind].attribute)
		{
			duatf(ind);
			at = StitchBuffer[ind].attribute;
		}
	}
}
#endif

void fdelstch()
{
	unsigned	ind, ine, fcod, at, typ, tmap, col, bcol, tapcol;
	unsigned*	stpnt;

	fvars(ClosestFormToCursor);
	tmap = 0;
	FillMemory(&fillStartsData, sizeof(FSTRTS), 0);
	fcod = (ClosestFormToCursor << FRMSHFT);
	ine = 0;
	bcol = SelectedForm->borderColor&COLMSK;
	tapcol = SelectedForm->borderColor >> 4;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		if (!chku(FIL2OF) && chkMap(SELBOX) && ind == ClosestPointIndex)
			ClosestPointIndex = ine;
		at = StitchBuffer[ind].attribute;
		if (fcod == (at&(FRMSK | NOTFRM)))
		{
			typ = ftyps[dutyp(at)];
			switch (typ)
			{
			case 1:

				if (!(tmap&M_AP))
				{
					tmap |= M_AP;
					fillStartsData.apl = ine;
				}
				break;

			case 5:

				if (!(tmap&M_FTH))
				{
					tmap |= M_FTH;
					fillStartsData.fth = ine;
				}
				break;

			case 6:

				if (!(tmap&M_FIL))
				{
					tmap |= M_FIL;
					fillStartsData.fil = ine;
				}
				break;

			case 7:

				if (!(tmap&M_BRD))
				{
					tmap |= M_BRD;
					fillStartsData.brd = ine;
				}
				break;

			default:

				if (SelectedForm->fillType && !(tmap&M_FIL))
				{
					tmap |= M_FIL;
					fillStartsData.fil = ine;
				}
				break;
			}
		}
		else
		{
			col = at&COLMSK;
			if (col == SelectedForm->fillColor)
			{
				tmap |= M_FCOL;
				fillStartsData.fcol = ine;
			}
			if (col == SelectedForm->fillInfo.feather.color)
			{
				tmap |= M_FTHCOL;
				fillStartsData.fthcol = ine;
			}
			if (col == bcol)
			{
				tmap |= M_ECOL;
				fillStartsData.ecol = ine;
			}
			if (col == tapcol)
			{
				tmap |= M_APCOL;
				fillStartsData.apcol = ine;
			}
			StitchBuffer[ine].x = StitchBuffer[ind].x;
			StitchBuffer[ine].y = StitchBuffer[ind].y;
			StitchBuffer[ine].attribute = StitchBuffer[ind].attribute;
			ine++;
		}
	}
	fillStartsData.fcol++;
	fillStartsData.fthcol++;
	fillStartsData.ecol++;
	fillStartsData.apcol++;
	fillStartsMap = tmap;
	PCSHeader.stitchCount = ine;
	ine = 0;
	if (!(tmap&M_ECOL))
		fillStartsData.ecol = PCSHeader.stitchCount;
	if (!(tmap&M_FTHCOL))
		fillStartsData.fthcol = PCSHeader.stitchCount;
	if (!(tmap&M_FCOL))
		fillStartsData.fcol = PCSHeader.stitchCount;
	if (SelectedForm->edgeType)
	{
		if (SelectedForm->edgeType == EDGEAPPL)
		{
			if (!(tmap&M_AP))
			{
				if (tmap&M_APCOL)
					fillStartsData.apl = fillStartsData.apcol + 1;
				else
					fillStartsData.apl = PCSHeader.stitchCount;
			}
		}
		if (!(tmap&M_BRD))
		{
			if (tmap&M_ECOL)
				fillStartsData.brd = fillStartsData.ecol + 1;
			else
				fillStartsData.brd = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType || (tmap&(M_WALK | M_UND | M_CWLK)))
	{
		if (!(tmap&M_FIL))
		{
			if (tmap&M_FCOL)
				fillStartsData.fil = fillStartsData.fcol + 1;
			else
				fillStartsData.fil = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType == FTHF)
	{
		if (!(tmap&M_FTH))
		{
			if (tmap&M_FTHCOL)
				fillStartsData.fth = fillStartsData.fthcol + 1;
			else
				fillStartsData.fth = PCSHeader.stitchCount;
		}
	}
	stpnt = (unsigned*)&fillStartsData;
	for (ind = 3; ind; ind--)
	{
		ine = ind - 1;
		while (ine < ind)
		{
			if (stpnt[ine] > stpnt[ind])
				stpnt[ind] = stpnt[ine];
			ine--;
		}
	}
	if (!chku(FIL2OF) && chkMap(SELBOX))
	{
		for (ind = 0; ind < sizeof(fillStartsData) >> 2; ind++)
			stpnt[ind] = ClosestPointIndex;
	}
}

BOOL lastcol(unsigned ind, fPOINT* pnt)
{
	unsigned col;

	col = interleaveSequenceIndices[ind].color;
	while (ind)
	{
		ind--;
		if (interleaveSequenceIndices[ind].color == col)
		{
			pnt->x = interleaveSequence[interleaveSequenceIndices[ind + 1].ind - 1].x;
			pnt->y = interleaveSequence[interleaveSequenceIndices[ind + 1].ind - 1].y;
			return 1;
		}
	}
	return 0;
}

void duint(unsigned off, unsigned cod)
{
	unsigned	cnt, ind;
	fPOINT		colpnt;

	if (interleaveData.coloc > interleaveData.sloc)
	{
		cnt = interleaveData.coloc - interleaveData.sloc;
		MoveMemory(&interleaveData.histch[interleaveData.oloc], &StitchBuffer[interleaveData.sloc], sizeof(fPOINTATTR)*cnt);
		interleaveData.sloc += cnt;
		interleaveData.oloc += cnt;
	}
	if (SelectedForm->extendedAttribute&AT_STRT)
	{
		if (!setMap(DIDSTRT))
			interleaveData.oloc += gucon(CurrentFormVertices[SelectedForm->fillStart], interleaveSequence[interleaveSequenceIndices[interleaveData.pins].ind], interleaveData.oloc + off, cod);
	}
	if (lastcol(interleaveData.pins, &colpnt))
		interleaveData.oloc += gucon(colpnt, interleaveSequence[interleaveSequenceIndices[interleaveData.pins].ind], interleaveData.oloc + MAXSEQ, cod);
	for (ind = interleaveSequenceIndices[interleaveData.pins].ind; ind < interleaveSequenceIndices[interleaveData.pins + 1].ind; ind++)
	{
		interleaveData.histch[interleaveData.oloc].x = interleaveSequence[ind].x;
		interleaveData.histch[interleaveData.oloc].y = interleaveSequence[ind].y;
		interleaveData.histch[interleaveData.oloc].attribute = cod;
		if (interleaveData.histch[interleaveData.oloc].x != interleaveData.histch[interleaveData.oloc - 1].x ||
			interleaveData.histch[interleaveData.oloc].y != interleaveData.histch[interleaveData.oloc - 1].y)
			interleaveData.oloc++;
	}
}

BOOL isfil()
{
	if (SelectedForm->fillType)
		return 1;
	if (SelectedForm->edgeType)
		return 1;
	if (SelectedForm->extendedAttribute&(AT_CWLK | AT_WALK | AT_UND))
		return 1;
	return 0;
}

void chkend(unsigned off, unsigned cod)
{
	if (isfil())
	{
		setMap(ISEND);
		if (SelectedForm->extendedAttribute&AT_END)
			interleaveData.oloc += gucon(interleaveSequence[interleaveSequenceIndex - 1], CurrentFormVertices[SelectedForm->fillEnd], interleaveData.oloc + off, cod);
	}
}

void intlv()
{
	unsigned	ind, ine, cod = 0, off;
	fPOINT		colpnt;

	rstMap(ISEND);
	fvars(ClosestFormToCursor);
	interleaveSequenceIndices[interleaveSequenceIndex2].ind = interleaveSequenceIndex;
	FillMemory(&interleaveData, sizeof(INTINF), 0);
	interleaveData.layerIndex = ((SelectedForm->attribute&FRMLMSK) << (LAYSHFT - 1)) | (ClosestFormToCursor << FRMSHFT);
	rstMap(DIDSTRT);
	if (PCSHeader.stitchCount)
	{
		off = MAXSEQ;
		interleaveData.histch = &StitchBuffer[MAXSEQ];
		for (ind = 0; ind < interleaveSequenceIndex2; ind++)
		{
			interleaveData.pins = ind;
			switch (interleaveSequenceIndices[ind].seq)
			{
			case I_AP:

				if (fillStartsMap&M_FIL&&fillStartsData.apl >= interleaveData.coloc)
					interleaveData.coloc = fillStartsData.apl;
				else
				{
					interleaveData.coloc = fillStartsData.apcol;
					if (interleaveData.coloc == 1)
						interleaveData.coloc = 0;
				}
				break;

			case I_FIL:

				if (fillStartsMap&M_FIL&&fillStartsData.fil >= interleaveData.coloc)
					interleaveData.coloc = fillStartsData.fil;
				else
					interleaveData.coloc = fillStartsData.fcol;
				break;

			case I_FTH:

				if (fillStartsMap&M_FIL&&fillStartsData.fth >= interleaveData.coloc)
					interleaveData.coloc = fillStartsData.fth;
				else
					interleaveData.coloc = fillStartsData.fthcol;
				break;

			case I_BRD:

				if (fillStartsMap&M_BRD&&fillStartsData.brd >= interleaveData.coloc)
					interleaveData.coloc = fillStartsData.brd;
				else
					interleaveData.coloc = fillStartsData.ecol;
				break;
			}
			cod = interleaveData.layerIndex | interleaveSequenceIndices[interleaveData.pins].cod | interleaveSequenceIndices[interleaveData.pins].color;
			duint(off, cod);
		}
		chkend(MAXSEQ, cod);
		if (PCSHeader.stitchCount&&interleaveData.sloc < (unsigned)PCSHeader.stitchCount - 1)
		{
			ine = PCSHeader.stitchCount - interleaveData.sloc;
			MoveMemory(&StitchBuffer[interleaveData.oloc + MAXSEQ], &StitchBuffer[interleaveData.sloc], sizeof(fPOINTATTR)*ine);
			interleaveData.oloc += ine;
		}
		MoveMemory(StitchBuffer, interleaveData.histch, sizeof(fPOINTATTR)*interleaveData.oloc);
	}
	else
	{
		off = 0;
		for (ind = 0; ind < interleaveSequenceIndex2; ind++)
		{
			cod = interleaveData.layerIndex | interleaveSequenceIndices[ind].cod | interleaveSequenceIndices[ind].color;
			if (SelectedForm->extendedAttribute&AT_STRT)
			{
				if (!setMap(DIDSTRT))
					interleaveData.oloc += gucon(CurrentFormVertices[SelectedForm->fillStart], interleaveSequence[interleaveSequenceIndices[interleaveData.pins].ind], interleaveData.oloc + off, cod);
			}
			if (lastcol(ind, &colpnt))
				interleaveData.oloc += gucon(colpnt, interleaveSequence[interleaveSequenceIndices[ind].ind], interleaveData.oloc, cod);
			for (ine = interleaveSequenceIndices[ind].ind; ine < interleaveSequenceIndices[ind + 1].ind; ine++)
			{
				StitchBuffer[interleaveData.oloc].x = interleaveSequence[ine].x;
				StitchBuffer[interleaveData.oloc].y = interleaveSequence[ine].y;
				StitchBuffer[interleaveData.oloc].attribute = cod;
				if (StitchBuffer[interleaveData.oloc].x != StitchBuffer[interleaveData.oloc - 1].x ||
					StitchBuffer[interleaveData.oloc].y != StitchBuffer[interleaveData.oloc - 1].y)
					interleaveData.oloc++;
			}
		}
		chkend(0, cod);
	}
	PCSHeader.stitchCount = interleaveData.oloc;
	coltab();
}

void setundfn(unsigned cod)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
		{
			ClosestFormToCursor = SelectedFormList[ind];
			fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE)
				continue;
			SelectedForm->extendedAttribute |= cod;
			refilfn();
		}
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL))
	{
		fvars(ClosestFormToCursor);
		SelectedForm->extendedAttribute |= cod;
		refilfn();
		coltab();
		setMap(RESTCH);
	}
}

void setund()
{
	setundfn(AT_UND);
}

void setwlk()
{
	setundfn(AT_WALK);
}

void setcwlk()
{
	setundfn(AT_CWLK);
}

void notundfn(unsigned cod)
{
	unsigned ind;

	savdo();
	cod = !cod;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
		{
			ClosestFormToCursor = ind;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE)
				continue;
			SelectedForm->extendedAttribute &= cod;
			refilfn();
		}
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL))
	{
		fvars(ClosestFormToCursor);
		if (SelectedForm->type == FRMLINE)
			return;
		SelectedForm->extendedAttribute &= cod;
		refilfn();
		coltab();
		setMap(RESTCH);
	}
}

void notund()
{
	notundfn(AT_UND);
}

void notwlk()
{
	notundfn(AT_WALK);
}

void notcwlk()
{
	notundfn(AT_CWLK);
}

void ulenfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
	{
		SelectedForm->underlayStitchLen = len;
		refilfn();
	}
}

void dusulen(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			ulenfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		ulenfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void undlen()
{
	tabmsg(IDS_SETULEN);
	setMap(NUMIN);
	setMap(FSETULEN);
	numWnd();
}

void uspacfn(unsigned find, float spac)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&AT_UND)
	{
		SelectedForm->underlaySpacing = spac;
		refilfn();
	}
}

void duspac(float spac)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			uspacfn(SelectedFormList[ind], spac);
	}
	if (chkMap(FORMSEL))
		uspacfn(ClosestFormToCursor, spac);
	coltab();
	setMap(RESTCH);
}

void uspac()
{
	tabmsg(IDS_SETUSPAC);
	setMap(NUMIN);
	setMap(FSETUSPAC);
	numWnd();
}

void uangfn(unsigned find, float p_ang)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&AT_UND)
	{
		SelectedForm->underlayStitchAngle = p_ang;
		refilfn();
	}
}

void dufang(float p_ang)
{
	unsigned ind;

	savdo();
	p_ang *= (float)PI / 180;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			uangfn(SelectedFormList[ind], p_ang);
	}
	if (chkMap(FORMSEL))
		uangfn(ClosestFormToCursor, p_ang);
	coltab();
	setMap(RESTCH);
}

void sfuang()
{
	tabmsg(IDS_SETUANG);
	setMap(NUMIN);
	setMap(FSETUANG);
	numWnd();
}

void flenfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType && !isclp(find))
	{
		SelectedForm->lengthOrCount.stitchLength = len;
		refilfn();
	}
}

void duflen(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			flenfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		flenfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setflen()
{
	tabmsg(IDS_SETFLEN);
	setMap(NUMIN);
	setMap(FSETFLEN);
	numWnd();
}

void fspacfn(unsigned find, float spac)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType)
	{
		if (spac < 0)
		{
			if (!isclp(find))
				return;
		}
		SelectedForm->fillSpacing = spac;
		refilfn();
	}
}

void dufspac(float spac)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fspacfn(SelectedFormList[ind], spac);
	}
	if (chkMap(FORMSEL))
		fspacfn(ClosestFormToCursor, spac);
	coltab();
	setMap(RESTCH);
}

void setfspac()
{
	tabmsg(IDS_SETFSPAC);
	setMap(NUMIN);
	setMap(FSETFSPAC);
	numWnd();
}

void findfn(unsigned find, float idnt)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	SelectedForm->underlayIndent = idnt;
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK))
		refilfn();
}

void dufind(float idnt)
{
	unsigned ind;

	savdo();
	idnt *= PFGRAN;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			findfn(SelectedFormList[ind], idnt);
	}
	if (chkMap(FORMSEL))
		findfn(ClosestFormToCursor, idnt);
	coltab();
	setMap(RESTCH);
}

void fangfn(unsigned find, float p_ang)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->type == FRMFPOLY&&SelectedForm->fillType)
	{
		switch (SelectedForm->fillType)
		{
		case VRTF:
		case HORF:
		case ANGF:

			SelectedForm->fillType = ANGF;
			SelectedForm->angleOrClipData.angle = p_ang;
			break;

		case	VCLPF:
		case	HCLPF:
		case	ANGCLPF:

			SelectedForm->fillType = ANGCLPF;
			SelectedForm->satinOrAngle.angle = p_ang;
			break;
		}
		refilfn();
	}
}

void dufxang(float p_ang)
{
	unsigned ind;

	savdo();
	p_ang *= (float)PI / 180;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fangfn(SelectedFormList[ind], p_ang);
	}
	if (chkMap(FORMSEL))
		fangfn(ClosestFormToCursor, p_ang);
	coltab();
	setMap(RESTCH);
}

void setfang()
{
	tabmsg(IDS_SETFANG);
	setMap(NUMIN);
	setMap(FSETFANG);
	numWnd();
}

void ucolfn(unsigned find, unsigned col)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
	{
		SelectedForm->underlayColor = col;
		refilfn();
	}
}

void dundcol(unsigned col)
{
	unsigned ind;

	savdo();
	if (col)
		col--;
	col &= COLMSK;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			ucolfn(SelectedFormList[ind], col);
	}
	if (chkMap(FORMSEL))
		ucolfn(ClosestFormToCursor, col);
	coltab();
	setMap(RESTCH);
}

void setucol()
{
	tabmsg(IDS_COL);
	setMap(NUMIN);
	setMap(FSETUCOL);
	numWnd();
}

void fcolfn(unsigned find, unsigned col)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType)
	{
		SelectedForm->fillColor = col;
		refilfn();
	}
}

void dufcol(unsigned col)
{
	unsigned ind;

	savdo();
	if (col)
		col--;
	col &= COLMSK;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fcolfn(SelectedFormList[ind], col);
	}
	if (chkMap(FORMSEL))
		fcolfn(ClosestFormToCursor, col);
	coltab();
	setMap(RESTCH);
}

void setfcol()
{
	tabmsg(IDS_COL);
	setMap(NUMIN);
	setMap(FSETFCOL);
	numWnd();
}

void bcolfn(unsigned find, unsigned col)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType)
	{
		SelectedForm->borderColor = col;
		refilfn();
	}
}

void dubcol(unsigned col)
{
	unsigned ind;

	savdo();
	if (col)
		col--;
	col &= COLMSK;
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			bcolfn(SelectedFormList[ind], col);
	}
	if (chkMap(FORMSEL))
		bcolfn(ClosestFormToCursor, col);
	coltab();
	setMap(RESTCH);
}

void setbcol()
{
	tabmsg(IDS_COL);
	setMap(NUMIN);
	setMap(FSETBCOL);
	numWnd();
}

void blenfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType && !iseclp(find))
	{
		SelectedForm->lengthOrCount.stitchLength = len;
		refilfn();
	}
}

void dublen(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			blenfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		blenfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setblen()
{
	tabmsg(IDS_SETFLEN);
	setMap(NUMIN);
	setMap(FSETBLEN);
	numWnd();
}

void bspacfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType)
	{
		SelectedForm->edgeSpacing = len;
		refilfn();
	}
}

void dubspac(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			bspacfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		bspacfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setbspac()
{
	tabmsg(IDS_SETFSPAC);
	setMap(NUMIN);
	setMap(FSETBSPAC);
	numWnd();
}

void bminfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType)
	{
		SelectedForm->minBorderStitchLen = len;
		refilfn();
	}
}

void dubmin(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			bminfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		bminfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setbmin()
{
	tabmsg(IDS_TXT23);
	setMap(NUMIN);
	setMap(FSETBMIN);
	numWnd();
}

void bmaxfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType)
	{
		SelectedForm->maxBorderStitchLen = len;
		refilfn();
	}
}

void dubmax(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			bmaxfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		bmaxfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setbmax()
{
	tabmsg(IDS_TXT22);
	setMap(NUMIN);
	setMap(FSETBMAX);
	numWnd();
}

void fminfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType)
	{
		SelectedForm->minFillStitchLen = len;
		refilfn();
	}
}

void dufmin(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fminfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fminfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setfmin()
{
	tabmsg(IDS_TXT21);
	setMap(NUMIN);
	setMap(FSETFMIN);
	numWnd();
}

void fmaxfn(unsigned find, float len)
{
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType)
	{
		SelectedForm->maxFillStitchLen = len;
		refilfn();
	}
}

void dufmax(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fmaxfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fmaxfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setfmax()
{
	tabmsg(IDS_TXT20);
	setMap(NUMIN);
	setMap(FSETFMAX);
	numWnd();
}

void fwidfn(unsigned find, float len)
{
	unsigned	ind;
	double		rat;
	float		ref;

	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	ref = SelectedForm->rectangle.left;
	rat = len / (SelectedForm->rectangle.right - ref);
	for (ind = 0; ind < SideCount; ind++)
		CurrentFormVertices[ind].x = (CurrentFormVertices[ind].x - ref)*rat + ref;
	frmout(ClosestFormToCursor);
	refilfn();
}

void dufwid(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fwidfn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fwidfn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setfwid()
{
	tabmsg(IDS_WID);
	setMap(NUMIN);
	setMap(FSETFWID);
	numWnd();
}

void setfind()
{
	tabmsg(IDS_UWLKIND);
	setMap(NUMIN);
	setMap(FSETFIND);
	numWnd();
}

void fhifn(unsigned find, float len)
{
	unsigned	ind;
	double		rat;
	float		ref;

	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	ref = SelectedForm->rectangle.bottom;
	rat = len / (SelectedForm->rectangle.top - ref);
	for (ind = 0; ind < SideCount; ind++)
		CurrentFormVertices[ind].y = (CurrentFormVertices[ind].y - ref)*rat + ref;
	frmout(ClosestFormToCursor);
	refilfn();
}

void dufhi(float len)
{
	unsigned ind;

	savdo();
	if (SelectedFormCount)
	{
		for (ind = 0; ind < SelectedFormCount; ind++)
			fhifn(SelectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fhifn(ClosestFormToCursor, len);
	coltab();
	setMap(RESTCH);
}

void setfhi()
{
	tabmsg(IDS_HI);
	setMap(NUMIN);
	setMap(FSETFHI);
	numWnd();
}

void setfilstrt()
{
	if (chkMap(FRMPSEL))
	{
		FormList[ClosestFormToCursor].fillStart = ClosestVertexToCursor;
		FormList[ClosestFormToCursor].extendedAttribute |= AT_STRT;
		refil();
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FORMP, IDS_FSTRT);
}

void setfilend()
{
	if (chkMap(FRMPSEL))
	{
		FormList[ClosestFormToCursor].fillEnd = ClosestVertexToCursor;
		FormList[ClosestFormToCursor].extendedAttribute |= AT_END;
		refil();
		coltab();
		setMap(RESTCH);
	}
	else
		shoseln(IDS_FORMP, IDS_FEND);
}

void duauxnam()
{
	TCHAR* pext;

	_strlwr_s(WorkingFileName);
	strcpy_s(AuxName, WorkingFileName);
	pext = strrchr(AuxName, '.');
	if (pext)
		pext++;
	else
		pext = &WorkingFileName[strlen(WorkingFileName)];
	*pext = 0;
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

void dutxtfil()
{
	if (!IniFile.textureHeight)
		IniFile.textureHeight = ITXHI;
	if (!IniFile.textureWidth)
		IniFile.textureWidth = ITXWID;
	if (!IniFile.textureSpacing)
		IniFile.textureSpacing = (float)ITXSPAC;
	if (!IniFile.textureEditorSize)
		IniFile.textureEditorSize = ITXPIX;
	AngledForm.sides = 0;
	setMap(TXTRED);
	setMap(ZUMED);
	rstMap(WASPAT);
	rstMap(RUNPAT);
	movStch();
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
	cloxlst = (unsigned*)&OSequence;
	cloxcnt = 0;
	setMap(INIT);
	hBtnSideWindow = 0;
	if (chkMap(WASTXBAK))
	{
		redtbak();
		if (!textureScreen.areaHeight)
			textureScreen.areaHeight = IniFile.textureHeight;
		if (!textureScreen.spacing)
			textureScreen.spacing = IniFile.underlaySpacing;
		if (!textureScreen.width)
			textureScreen.width = IniFile.textureWidth;
		setMap(LASTXBAK);
		rstMap(TXBDIR);
	}
	else
	{
		textureScreen.index = 0;
		textureScreen.areaHeight = IniFile.textureHeight;
		textureScreen.width = IniFile.textureWidth;
		textureScreen.spacing = IniFile.textureSpacing;
	}
	setMap(RESTCH);
}

void txt2pix(TXPNT txp, POINT* pixp)
{
	pixp->y = textureScreen.height - txp.y / textureScreen.areaHeight*textureScreen.height + textureScreen.top;
	pixp->x = (txp.line*textureScreen.spacing + textureScreen.xOffset) / textureScreen.editToPixelRatio;
}

void txtxfn(POINT ref, int pix)
{
	POINT xlin[2];

	xlin[0].x = xlin[1].x = ref.x;
	xlin[0].y = ref.y - pix;
	xlin[1].y = ref.y + pix;
	Polyline(StitchWindowMemDC, xlin, 2);
	xlin[0].y = xlin[1].y = ref.y;
	xlin[0].x = ref.x - pix;
	xlin[1].x = ref.x + pix;
	Polyline(StitchWindowMemDC, xlin, 2);
}

void dutxtx(int ind, int pix)
{
	POINT	ref;

	txt2pix(tmpTexturePoints[ind], &ref);
	txtxfn(ref, pix);
	if (ref.y > textureScreen.halfHeight)
		ref.y -= textureScreen.height;
	else
		ref.y += textureScreen.height;
	txtxfn(ref, pix);
}

void txrct2rct(TXTRCT txr, RECT* rct)
{
	TXPNT	txp;
	POINT	tpnt;
	int		bh2;

	bh2 = ButtonHeight >> 1;
	txp.line = txr.left;
	txp.y = txr.top;
	txt2pix(txp, &tpnt);
	rct->left = tpnt.x - IniFile.textureEditorSize;
	rct->top = tpnt.y - IniFile.textureEditorSize;
	txp.line = txr.right;
	txp.y = txr.bottom;
	txt2pix(txp, &tpnt);
	rct->right = tpnt.x + IniFile.textureEditorSize;
	rct->bottom = tpnt.y + IniFile.textureEditorSize;
}

void ed2px(fPOINT ped, POINT* px)
{
	px->x = ped.x / textureScreen.editToPixelRatio;
	px->y = StitchWindowClientRect.bottom - ped.y / textureScreen.editToPixelRatio;
}

void px2ed(POINT px, fPOINT* ped)
{
	ped->x = px.x*textureScreen.editToPixelRatio;
	ped->y = textureScreen.screenHeight - px.y*textureScreen.editToPixelRatio;
}

void bxtxt(unsigned cod, TCHAR* str)
{
	SetWindowText(ButtonWin[cod], str);
}

void lodhbuf(unsigned cod)
{
	LoadString(ThredInstance, cod, HelpBuffer, HBUFSIZ);
}

void hlpflt(unsigned mcod, unsigned bcod, float dat)
{
	lodhbuf(mcod);
	sprintf_s(MsgBuffer, sizeof(MsgBuffer), HelpBuffer, dat);
	bxtxt(bcod, MsgBuffer);
}

void drwtxbut()
{
	lodhbuf(IDS_CLEAR);
	bxtxt(HTXCLR, HelpBuffer);
	hlpflt(IDS_TXHI, HTXHI, textureScreen.areaHeight / PFGRAN);
	redraw(ButtonWin[HTXWID]);
	hlpflt(IDS_TXSPAC, HTXSPAC, textureScreen.spacing / PFGRAN);
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

void chktx()
{
	int ind, ine;

	ine = 0;
	for (ind = 0; ind < textureScreen.index; ind++)
	{
		if (tmpTexturePoints[ind].line <= textureScreen.lines&&tmpTexturePoints[ind].y < textureScreen.areaHeight)
		{
			tmpTexturePoints[ine].line = tmpTexturePoints[ind].line;
			tmpTexturePoints[ine].y = tmpTexturePoints[ind].y;
			ine++;
		}
	}
	textureScreen.index = ine;
}

void drwtxtr()
{
	POINT		xlin[2];
	int			ind;
	int			ofy;
	COLORREF	col;
	TXPNT		txp;
	POINT		tpnt;
	int			cnt;
	double		edsp;
	double		pxsp;
	float		wid2;

	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	edsp = textureScreen.areaHeight * 2 / (textureScreen.spacing*(textureScreen.lines + 2));
	pxsp = (double)StitchWindowClientRect.bottom / StitchWindowClientRect.right;
	textureScreen.lines = floor(textureScreen.width / textureScreen.spacing);
	wid2 = textureScreen.spacing*(textureScreen.lines + 2);
	if (rstMap(CHKTX))
		chktx();
	if (pxsp > edsp)
	{
		textureScreen.xOffset = 0;
		textureScreen.editToPixelRatio = wid2 / StitchWindowClientRect.bottom;
		ofy = (StitchWindowClientRect.bottom - textureScreen.areaHeight / textureScreen.editToPixelRatio) / 2;
	}
	else
	{
		textureScreen.editToPixelRatio = textureScreen.areaHeight * 2 / StitchWindowClientRect.bottom;
		ofy = StitchWindowClientRect.bottom >> 2;
		textureScreen.xOffset = (StitchWindowClientRect.right*textureScreen.editToPixelRatio - (textureScreen.lines + 2)*textureScreen.spacing) / 2;
	}
	textureScreen.top = ofy;
	textureScreen.bottom = StitchWindowClientRect.bottom - ofy;
	textureScreen.height = textureScreen.bottom - textureScreen.top;
	textureScreen.halfHeight = StitchWindowClientRect.bottom >> 1;
	textureScreen.screenHeight = StitchWindowClientRect.bottom*textureScreen.editToPixelRatio;
	textureScreen.yOffset = (textureScreen.screenHeight - textureScreen.areaHeight) / 2;
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, GridPen);
	cnt = textureScreen.areaHeight / IniFile.gridSize + 1;
	txp.line = 0;
	xlin[0].x = 0;
	xlin[1].x = StitchWindowClientRect.right;
	txp.y = 0;
	for (ind = 0; ind < cnt; ind++)
	{
		txt2pix(txp, &tpnt);
		xlin[0].y = xlin[1].y = tpnt.y;
		Polyline(StitchWindowMemDC, xlin, 2);
		txp.y += IniFile.gridSize;
	}
	DeleteObject(textureCrossPen);
	textureCrossPen = CreatePen(PS_SOLID, 1, UserColor[ActiveColor]);
	SelectObject(StitchWindowMemDC, textureCrossPen);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	xlin[0].y = 0;
	xlin[1].y = StitchWindowClientRect.bottom;
	for (ind = 1; ind < textureScreen.lines + 1; ind++)
	{
		xlin[0].x = xlin[1].x = (ind*textureScreen.spacing + textureScreen.xOffset) / textureScreen.editToPixelRatio;
		Polyline(StitchWindowMemDC, xlin, 2);
	}
	xlin[0].x = 0;
	xlin[1].x = StitchWindowClientRect.right;
	xlin[0].y = xlin[1].y = textureScreen.top;
	Polyline(StitchWindowMemDC, xlin, 2);
	xlin[0].y = xlin[1].y = textureScreen.bottom;
	Polyline(StitchWindowMemDC, xlin, 2);
	DeleteObject(textureCrossPen);
	textureCrossPen = CreatePen(PS_SOLID, 1, 0xffffff);
	SelectObject(StitchWindowMemDC, textureCrossPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	col = UserColor[ActiveColor];
	for (ind = 0; ind < textureScreen.index; ind++)
	{
		dutxtx(ind, IniFile.textureEditorSize);
	}
	if (cloxcnt)
	{
		txrct2rct(textureRect, &texturePixelRect);
		xlin[0].y = xlin[1].y = texturePixelRect.top;
		xlin[0].x = texturePixelRect.left;
		xlin[1].x = texturePixelRect.right;
		Polyline(StitchWindowMemDC, xlin, 2);
		xlin[1].y = texturePixelRect.bottom;
		xlin[1].x = texturePixelRect.left;
		Polyline(StitchWindowMemDC, xlin, 2);
		xlin[0].x = texturePixelRect.right;
		xlin[0].y = texturePixelRect.bottom;
		Polyline(StitchWindowMemDC, xlin, 2);
		xlin[1].x = texturePixelRect.right;
		xlin[1].y = texturePixelRect.top;
		Polyline(StitchWindowMemDC, xlin, 2);
	}
	for (ind = 0; ind < cloxcnt; ind++)
	{
		dutxtx(cloxlst[ind], IniFile.textureEditorSize);
		dutxtx(cloxlst[ind], IniFile.textureEditorSize << 1);
	}
	BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
	drwtxbut();
}

BOOL px2txt(POINT pof, TXPNT* txrec)
{
	fPOINT epnt;

	px2ed(pof, &epnt);
	txrec->line = (epnt.x - textureScreen.xOffset) / textureScreen.spacing + 0.5;
	if (txrec->line > textureScreen.lines)
		return 0;
	if (txrec->line < 1)
		return 0;
	if (pof.y > textureScreen.top)
	{
		if (pof.y > textureScreen.bottom)
			return 0;
		else
			txrec->y = (float)textureScreen.areaHeight - (float)(pof.y - textureScreen.top) / textureScreen.height*textureScreen.areaHeight;
	}
	else
		return 0;
	return 1;
}

BOOL txbutfn(TXPNT* txrec)
{
	POINT	pof;

	deorg(&pof);
	return px2txt(pof, txrec);
}

void txtrbut()
{
	if (txbutfn(&tmpTexturePoints[textureScreen.index]))
	{
		savtxt();
		textureScreen.index++;
		setMap(RESTCH);
	}
	else
		rstxt();

}

BOOL txtclos(unsigned* pclo)
{
	double	len;
	double	minlen;
	int		ind;
	POINT	ref, tpnt;

	ref.x = Msg.pt.x - StitchWindowOrigin.x;
	ref.y = Msg.pt.y - StitchWindowOrigin.y;
	minlen = 1e99;
	*pclo = 0;
	for (ind = 0; ind < textureScreen.index; ind++)
	{
		txt2pix(tmpTexturePoints[ind], &tpnt);
		len = hypot(tpnt.x - ref.x, tpnt.y - ref.y);
		if (len < minlen)
		{
			minlen = len;
			*pclo = ind;
		}
	}
	if (minlen < CLOSENUF)
		return 1;
	return 0;
}

void tritx()
{
	POINT	xlin[2];
	int		li_Size;

	li_Size = IniFile.textureEditorSize << 2;
	xlin[0].x = xlin[1].x = textureCursorLocation.x;
	xlin[0].y = textureCursorLocation.y - li_Size;
	xlin[1].y = textureCursorLocation.y + li_Size;
	Polyline(StitchWindowDC, xlin, 2);
	xlin[0].y = xlin[1].y = textureCursorLocation.y;
	xlin[0].x = textureCursorLocation.x - li_Size;
	xlin[1].x = textureCursorLocation.x + li_Size;
	Polyline(StitchWindowDC, xlin, 2);
}

void setxmov()
{
	setMap(TXTMOV);
	textureCursorLocation.x = cloxref.x = Msg.pt.x - StitchWindowOrigin.x;
	textureCursorLocation.y = cloxref.y = Msg.pt.y - StitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void ritxrct()
{
	POINT	of;
	RECT	trct;
	POINT	xlin[5];

	of.x = textureCursorLocation.x - cloxref.x;
	of.y = textureCursorLocation.y - cloxref.y;
	trct.bottom = texturePixelRect.bottom + of.y;
	trct.left = texturePixelRect.left + of.x;
	trct.right = texturePixelRect.right + of.x;
	trct.top = texturePixelRect.top + of.y;
	xlin[0].x = xlin[1].x = xlin[4].x = trct.left;
	xlin[2].x = xlin[3].x = trct.right;
	xlin[0].y = xlin[3].y = xlin[4].y = trct.top;
	xlin[1].y = xlin[2].y = trct.bottom;
	Polyline(StitchWindowDC, xlin, 5);
}

void dutxrct(TXTRCT* rct)
{
	int		ind;
	TXPNT*	txp;

	if (cloxcnt)
	{
		txp = &tmpTexturePoints[cloxlst[0]];
		rct->left = rct->right = txp->line;
		rct->top = rct->bottom = txp->y;
		for (ind = 1; ind < cloxcnt; ind++)
		{
			txp = &tmpTexturePoints[cloxlst[ind]];
			if (txp->y > rct->top)
				rct->top = txp->y;
			if (txp->y < rct->bottom)
				rct->bottom = txp->y;
			if (txp->line < rct->left)
				rct->left = txp->line;
			if (txp->line > rct->right)
				rct->right = txp->line;
		}
	}
	else
		rct->left = rct->right = rct->top = rct->bottom;
}

void ed2stch(fPOINT* pt)
{
	pt->x -= textureScreen.xOffset;
	pt->y -= textureScreen.yOffset;
}

void dutxlin(fPOINT pt0, fPOINT pt1)
{
	double	slop;
	float	num;
	float	strt, fin, tflt;
	int		istrt, ifin;

	ed2stch(&pt0);
	ed2stch(&pt1);
	num = pt1.x - pt0.x;
	if (fabs(num) < TINY)
		return;
	slop = (pt1.y - pt0.y) / num;
	strt = pt0.x;
	fin = pt1.x;
	if (strt > fin)
	{
		tflt = strt;
		strt = fin;
		fin = tflt;
	}
	istrt = ceil(strt / textureScreen.spacing);
	ifin = floor(fin / textureScreen.spacing);
	if (istrt < 1)
		istrt = 1;
	if (ifin > textureScreen.lines)
		ifin = textureScreen.lines;
	while (istrt <= ifin)
	{
		tflt = slop*(-pt0.x + istrt*textureScreen.spacing) + pt0.y;
		if (tflt > 0 && tflt < textureScreen.areaHeight)
		{
			tmpTexturePoints[textureScreen.index].line = istrt;
			tmpTexturePoints[textureScreen.index].y = tflt;
			textureScreen.index++;
		}
		istrt++;
	}
}

void setxclp()
{
	POINT		of;
	fPOINT		fof;
	unsigned	ind, inx, cnt;

	deorg(&of);
	px2ed(of, &fof);
	if (rstMap(TXHCNTR))
		fof.x = (textureScreen.lines*textureScreen.spacing) / 2 + textureScreen.xOffset - textureScreen.formCenter.x + textureScreen.spacing / 2;
	else
		fof.x -= textureScreen.formCenter.x;
	fof.y -= textureScreen.formCenter.y;
	for (ind = 0; ind < AngledForm.sides; ind++)
	{
		AngledFormVertices[ind].x += fof.x;
		AngledFormVertices[ind].y += fof.y;
	}
	cnt = AngledForm.sides - 1;
	if (AngledForm.type != FRMLINE)
		cnt++;
	SideCount = AngledForm.sides;
	for (ind = 0; ind < cnt; ind++)
	{
		inx = nxt(ind);
		dutxlin(AngledFormVertices[ind], AngledFormVertices[inx]);
	}
}

void stxlin()
{
	POINT	of;
	fPOINT	pt0;
	fPOINT	pt1;

	rstMap(TXTMOV);
	deorg(&of);
	px2ed(of, &pt1);
	px2ed(FormLines[0], &pt0);
	dutxlin(pt0, pt1);
	setMap(RESTCH);
}

void ed2txp(POINT pof, TXPNT* txrec)
{
	fPOINT epnt;

	px2ed(pof, &epnt);
	txrec->line = (epnt.x - textureScreen.xOffset) / textureScreen.spacing + 0.5;
	txrec->y = (float)textureScreen.areaHeight - (float)(pof.y - textureScreen.top) / textureScreen.height*textureScreen.areaHeight;
}

int	hitxlin()
{
	int ind, hilin;

	hilin = 0;
	for (ind = 0; ind < cloxcnt; ind++)
	{
		if (tmpTexturePoints[cloxlst[ind]].line > hilin)
			hilin = tmpTexturePoints[cloxlst[ind]].line;
	}
	return hilin;
}

void txtrup()
{
	TXPNT	hi;
	TXPNT	lo;
	TXPNT	tof;
	float	tflt;
	short	tsht;
	int		ind, bakp;
	POINT	of;
	TXPNT*	ptxt;

	if (rstMap(TXTMOV))
	{
		savtxt();
		deorg(&of);
		of.x -= cloxref.x;
		of.y -= cloxref.y;
		bakp = abs(of.x);
		tof.line = bakp*textureScreen.editToPixelRatio / textureScreen.spacing + 0.5;
		if (of.x < 0)
			tof.line = -tof.line;
		tof.y = (float)-of.y / textureScreen.height*textureScreen.areaHeight;
		tflt = textureRect.top + tof.y - textureScreen.areaHeight;
		if (tflt > 0)
			tof.y -= tflt;
		tflt = textureRect.bottom + tof.y;
		if (tflt < 0)
			tof.y -= tflt;
		ind = textureRect.left + tof.line - 1;
		if (ind < 0)
			tof.line -= ind;
		ind = textureRect.right + tof.line - textureScreen.lines;
		if (ind > 0)
			tof.line -= ind;
		for (ind = 0; ind < cloxcnt; ind++)
		{
			ptxt = &tmpTexturePoints[cloxlst[ind]];
			ptxt->line += tof.line;
			ptxt->y += tof.y;
		}
		dutxrct(&textureRect);
	}
	else
	{
		if (rstMap(BZUMIN))
		{
			deorg(&of);
			ed2txp(ZoomBoxLine[0], &hi);
			ed2txp(of, &lo);
			if (hi.line < lo.line)
			{
				tsht = hi.line;
				hi.line = lo.line;
				lo.line = tsht;
			}
			if (hi.y < lo.y)
			{
				tflt = hi.y;
				hi.y = lo.y;
				lo.y = tflt;
			}
			cloxcnt = 0;
			for (ind = 0; ind < textureScreen.index; ind++)
			{
				if (tmpTexturePoints[ind].y<hi.y&&
					tmpTexturePoints[ind].y>lo.y&&
					tmpTexturePoints[ind].line <= hi.line&&
					tmpTexturePoints[ind].line >= lo.line)
				{
					cloxlst[cloxcnt] = ind;
					cloxcnt++;
				}
			}
			dutxrct(&textureRect);
		}
	}
	setMap(RESTCH);
}

void angrct(fRECTANGLE* rct)
{
	unsigned	ind;

	rct->left = rct->right = AngledFormVertices[0].x;
	rct->bottom = rct->top = AngledFormVertices[0].y;
	for (ind = 1; ind < AngledForm.sides; ind++)
	{
		if (AngledFormVertices[ind].x < rct->left)
			rct->left = AngledFormVertices[ind].x;
		if (AngledFormVertices[ind].x > rct->right)
			rct->right = AngledFormVertices[ind].x;
		if (AngledFormVertices[ind].y > rct->top)
			rct->top = AngledFormVertices[ind].y;
		if (AngledFormVertices[ind].y < rct->bottom)
			rct->bottom = AngledFormVertices[ind].y;
	}
}

void ritxfrm()
{
	unsigned ind, cnt;
	POINT		of;

	of.x = textureCursorLocation.x - cloxref.x;
	of.y = textureCursorLocation.y - cloxref.y;
	for (ind = 0; ind < AngledForm.sides; ind++)
	{
		ed2px(AngledFormVertices[ind], &FormLines[ind]);
		FormLines[ind].x += of.x;
		FormLines[ind].y += of.y;
	}
	FormLines[ind].x = FormLines[0].x;
	FormLines[ind].y = FormLines[0].y;
	cnt = AngledForm.sides;
	if (AngledForm.type != FRMLINE)
		cnt++;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, cnt);
}

void setxfrm()
{
	unsigned	ind;
	fRECTANGLE		arct;
	float		hi;
	double		rat;

	angrct(&arct);
	for (ind = 0; ind < AngledForm.sides; ind++)
	{
		AngledFormVertices[ind].x -= arct.left;
		AngledFormVertices[ind].y -= arct.bottom;
	}
	angrct(&arct);
	hi = arct.top - arct.bottom;
	if (hi > textureScreen.areaHeight)
	{
		rat = textureScreen.areaHeight / hi*0.95;
		for (ind = 0; ind < AngledForm.sides; ind++)
		{
			AngledFormVertices[ind].x *= rat;
			AngledFormVertices[ind].y *= rat;
		}
		angrct(&arct);
	}
	textureScreen.formCenter.x = midl(arct.right, arct.left);
	textureScreen.formCenter.y = midl(arct.top, arct.bottom);
	ed2px(textureScreen.formCenter, &cloxref);
}

void txtclp()
{
	ThrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
	ClipMemory = GetClipboardData(ThrEdClip);
	if (ClipMemory)
	{
		ClipFormData = (FORMCLIP*)GlobalLock(ClipMemory);
		if (ClipFormData) {
			if (ClipFormData->clipType == CLP_FRM) {
				SelectedForm = &ClipFormData->form;
				frmcpy(&AngledForm, SelectedForm);
				MoveMemory(&AngledFormVertices, &SelectedForm[1], sizeof(fPOINT)*SelectedForm->sides);
				AngledForm.vertices = AngledFormVertices;
				rstMap(TXTLIN);
				setMap(TXTCLP);
				setMap(TXTMOV);
				setxfrm();
				textureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
				textureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			}
			GlobalUnlock(ClipMemory);
		}
	}
	setMap(RESTCH);
	rstMap(WASWROT);
}

void dutxtlin()
{
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, 2);
}

void txtrmov()
{
	if (chkMap(TXTLIN))
	{
		dutxtlin();
		deorg(&FormLines[1]);
		dutxtlin();
		return;
	}
	if (chkMap(TXTCLP))
	{
		if (setMap(WASWROT))
			ritxfrm();
		textureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
		textureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
		ritxfrm();
	}
	else
	{
		if (cloxcnt)
		{
			ritxrct();
			textureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
			textureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			ritxrct();
		}
	}
}

void txtlin()
{
	deorg(FormLines);
	deorg(&FormLines[1]);
	rstMap(TXTCLP);
	setMap(TXTLIN);
	setMap(TXTMOV);
}

void butsid(unsigned cod)
{
	RECT brct;

	chktxnum();
	textureWindowId = cod;
	GetWindowRect(ButtonWin[cod], &brct);
	hBtnSideWindow = CreateWindow(
		"STATIC",
		0,
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		brct.left + ButtonWidthX3 - StitchWindowOrigin.x,
		brct.top - StitchWindowOrigin.y,
		ButtonWidthX3,
		ButtonHeight,
		MainStitchWin,
		NULL,
		ThredInstance,
		NULL);
}

int txcmp(const void *arg1, const void *arg2)
{
	TXPNT* p0;
	TXPNT* p1;

	p0 = (TXPNT*)arg1;
	p1 = (TXPNT*)arg2;
	if (p0->line == p1->line)
	{
		if (p0->y == p1->y)
			return 0;
		else
		{
			if (p0->y > p1->y)
				return 1;
			else
				return -1;
		}
	}
	else
		return p0->line - p1->line;
	//	return 0;
}

void txpar()
{
	SelectedForm->type = FRMFPOLY;
	SelectedForm->fillInfo.texture.lines = textureScreen.lines;
	SelectedForm->fillInfo.texture.height = textureScreen.areaHeight;
	SelectedForm->fillSpacing = textureScreen.spacing;
	SelectedForm->lengthOrCount.stitchLength = IniFile.userStitchLength;
	SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
	SelectedForm->minFillStitchLen = IniFile.minStitchLength;
	SelectedForm->fillColor = ActiveColor;
	refilfn();
}

void txvrt()
{
	if (textureScreen.index)
	{
		if (chkMap(FORMSEL))
		{
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXVRTF;
			txpar();
		}
	}
}

void txhor()
{
	if (textureScreen.index)
	{
		if (chkMap(FORMSEL))
		{
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXHORF;
			txpar();
		}
	}
}

void txang()
{
	if (textureScreen.index)
	{
		if (chkMap(FORMSEL))
		{
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXANGF;
			SelectedForm->angleOrClipData.angle = (float)IniFile.fillAngle;
			txpar();
		}
	}
}

void deltx()
{
	unsigned ind, cnt;

	cnt = SelectedForm->fillInfo.texture.count;
	if (textureIndex&&istx(ClosestFormToCursor) && cnt)
	{
		ind = SelectedForm->fillInfo.texture.index;
		MoveMemory(&texturePointsBuffer[ind], &texturePointsBuffer[ind + cnt], textureIndex - (ind + cnt));
		for (ind = ClosestFormToCursor + 1; ind < FormIndex; ind++)
		{
			if (istx(ind))
				FormList[ind].fillInfo.texture.index -= cnt;
		}
		textureIndex -= SelectedForm->fillInfo.texture.count;
		SelectedForm->fillInfo.texture.count = 0;
	}
}

void nutx()
{
	int			ind, ins;
	FRMHED*		pf;

	qsort((void*)&tmpTexturePoints, textureScreen.index, 6, txcmp);
	ins = 0;
	if (FormIndex)
	{
		if (istx(ClosestFormToCursor))
		{
			ins = FormList[ClosestFormToCursor].fillInfo.texture.index;
			deltx();
		}
		else
		{
			for (ind = ClosestFormToCursor - 1; ind >= 0; ind--)
			{
				if (istx(ind))
				{
					pf = &FormList[ind];
					ins = pf->fillInfo.texture.index + pf->fillInfo.texture.count;
					goto nutskp;
				}
			}
		nutskp:;
		}
	}
	txspac(ins, textureScreen.index);
	MoveMemory(&texturePointsBuffer[ins], &tmpTexturePoints, textureScreen.index * sizeof(TXPNT));
	SelectedForm->fillInfo.texture.index = ins;
	SelectedForm->fillInfo.texture.count = textureScreen.index;
}

void altx()
{
	int ind;
	float hi2;

	if (chkMap(FORMSEL))
	{
		hi2 = textureScreen.areaHeight / 2;
		clRmap((textureScreen.lines >> 5) + 1);
		for (ind = 0; ind < textureScreen.index; ind++)
			setr(tmpTexturePoints[ind].line);
		for (ind = 1; ind <= textureScreen.lines; ind++)
		{
			if (!chkr(ind))
			{
				tmpTexturePoints[textureScreen.index].line = ind;
				tmpTexturePoints[textureScreen.index].y = hi2;
				textureScreen.index++;
			}
		}
	}
	txof();
}

enum
{
	VRTYP,
	HORTYP,
	ANGTYP,
};

void dutxfn(unsigned typ)
{
	if (chkMap(FORMSEL))
	{
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
		switch (typ)
		{
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

void txsrt()
{
	qsort((void*)tmpTexturePoints, textureScreen.index, 6, txcmp);
}

void dutxmir()
{
	int lin, ind, ine;

	savtxt();
	txsrt();
	lin = (textureScreen.lines + 1) >> 1;
	ind = textureScreen.index - 1;
	while (tmpTexturePoints[ind].line > lin&&ind >= 0)
		ind--;
	ine = ind + 1;
	if (textureScreen.lines & 1)
	{
		while (ind >= 0) {
			if (tmpTexturePoints[ind].line == lin) {
				ind--;
			}
			else { break; }
		}
	}
	while (ind >= 0)
	{
		tmpTexturePoints[ine].line = textureScreen.lines - tmpTexturePoints[ind].line + 1;
		tmpTexturePoints[ine].y = tmpTexturePoints[ind].y;
		ine++;
		ind--;
	}
	textureScreen.index = ine;
	setMap(RESTCH);
}

BOOL chkbut()
{
	if (Msg.hwnd == ButtonWin[HTXCLR])
	{
		txdelal();
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXHI])
	{
		butsid(HTXHI);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXWID])
	{
		butsid(HTXWID);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXSPAC])
	{
		butsid(HTXSPAC);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXVRT])
	{
		dutxfn(VRTYP);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXHOR])
	{
		dutxfn(HORTYP);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXANG])
	{
		dutxfn(ANGTYP);
		return 1;
	}
	if (Msg.hwnd == ButtonWin[HTXMIR])
	{
		dutxmir();
		return 1;
	}
	return 0;
}

void txtlbut()
{
	POINT	tpnt;

	fvars(ClosestFormToCursor);
	if (chkbut())
		return;
	if (rstMap(TXTCLP))
	{
		savtxt();
		setxclp();
		return;
	}
	if (rstMap(TXTLIN))
	{
		savtxt();
		stxlin();
		return;
	}
	if (cloxcnt)
	{
		deorg(&tpnt);
		if (tpnt.x > texturePixelRect.left&&
			tpnt.x<texturePixelRect.right&&
			tpnt.y>texturePixelRect.top&&
			tpnt.y < texturePixelRect.bottom)
		{
			setxmov();
			ritxrct();
			return;
		}
	}
	if (txtclos(cloxlst))
	{
		cloxcnt = 1;
		setxmov();
		dutxrct(&textureRect);
		setMap(RESTCH);
		return;
	}
	cloxcnt = 0;
	setMap(BZUMIN);
	rstMap(BZUM);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y = ZoomBoxLine[0].y - 1;
}

void redtbak()
{
	TXHST*		phst;

	//	sprintf_s(MsgBuffer, sizeof(MsgBuffer),"%d\n",textureHistoryIndex);
	//	OutputDebugString(MsgBuffer);
	phst = &textureHistory[textureHistoryIndex];
	textureScreen.areaHeight = phst->height;
	textureScreen.width = phst->width;
	textureScreen.spacing = phst->spacing;
	textureScreen.index = phst->count;
	// ToDo - check texturePoint is valid first
	MoveMemory (tmpTexturePoints, phst->texturePoint, phst->count * sizeof (TXPNT));
	setMap(RESTCH);
}

void txbak()
{
	unsigned	ind;

	if (chkMap(WASTXBAK))
	{
		cloxcnt = 0;
		for (ind = 0; ind < 16; ind++)
		{
			if (textureHistory[textureHistoryIndex].width)
				goto txbak1;
			txrbak();
		}
		return;
	txbak1:;
		redtbak();
		txrbak();
	}
}

void nxbak()
{
	unsigned ind;

	if (chkMap(WASTXBAK))
	{
		for (ind = 0; ind < 16; ind++)
		{
			txrfor();
			if (textureHistory[textureHistoryIndex].width)
				goto nxbak1;
		}
		return;
	nxbak1:;
		redtbak();
	}
}

void txtdel()
{
	int			ind, ine;
	unsigned	clo;

	if (cloxcnt)
	{
		savtxt();
		clRmap(RMAPSIZ);
		for (ind = 0; ind < cloxcnt; ind++)
			setr(cloxlst[ind]);
		ine = 0;
		for (ind = 0; ind < textureScreen.index; ind++)
		{
			if (!chkr(ind))
			{
				tmpTexturePoints[ine].line = tmpTexturePoints[ind].line;
				tmpTexturePoints[ine].y = tmpTexturePoints[ind].y;
				ine++;
			}
		}
		cloxcnt = 0;
		textureScreen.index = ine;
		setMap(RESTCH);
		return;
	}
	if (textureScreen.index&&txtclos(&clo))
	{
		MoveMemory(&tmpTexturePoints[clo], &tmpTexturePoints[clo + 1], (textureScreen.index - clo) * sizeof(TXPNT));
		textureScreen.index--;
		setMap(RESTCH);
	}
}

void txdelal()
{
	chktxnum();
	savtxt();
	textureScreen.index = 0;
	rstxt();
	setMap(RESTCH);
}

void chktxnum()
{
	float tflt;

	tflt = atof(textureInputBuffer);
	if (tflt)
	{
		tflt *= PFGRAN;
		switch (textureWindowId)
		{
		case HTXHI:

			savtxt();
			textureScreen.areaHeight = tflt;
			IniFile.textureHeight = tflt;
			setMap(CHKTX);
			break;

		case HTXWID:

			savtxt();
			textureScreen.width = tflt;
			IniFile.textureWidth = tflt;
			setMap(CHKTX);
			break;

		case HTXSPAC:

			savtxt();
			textureScreen.spacing = tflt;
			IniFile.textureSpacing = tflt;
			textureScreen.width = tflt*textureScreen.lines + tflt / 2;
			setMap(CHKTX);
			break;
		}
	}
	textureInputIndex = 0;
	DestroyWindow(hBtnSideWindow);
	hBtnSideWindow = 0;
	setMap(RESTCH);
}

void txcntrv()
{
	if (rstMap(TXTCLP))
	{
		setMap(TXHCNTR);
		savtxt();
		setxclp();
		setMap(RESTCH);
	}
}

void txof()
{
	butxt(HBOXSEL, StringTable[STR_BOXSEL]);
	redraw(ButtonWin[HHID]);
	if (chkMap(UPTO))
		butxt(HUPTO, StringTable[STR_UPON]);
	else
		butxt(HUPTO, StringTable[STR_UPOF]);
	SetWindowText(ButtonWin[HTXSPAC], "");
	savtxt();
	zumhom();
	rstMap(TXTRED);
}

BOOL istxclp()
{
	if (chkMap(TXTMOV) && chkMap(TXTCLP))
		return 1;
	else
		return 0;

}

void txsiz(double rat)
{
	unsigned	ind;
	fRECTANGLE		arct;

	ritxfrm();
	for (ind = 0; ind < AngledForm.sides; ind++)
	{
		AngledFormVertices[ind].x *= rat;
		AngledFormVertices[ind].y *= rat;
	}
	angrct(&arct);
	textureScreen.formCenter.x = midl(arct.right, arct.left);
	textureScreen.formCenter.y = midl(arct.top, arct.bottom);
	ed2px(textureScreen.formCenter, &cloxref);
	ritxfrm();
}

void txshrnk()
{
	txsiz(TXTRAT);
}

void txgro()
{
	txsiz(1 / TXTRAT);
}

BOOL txdig(unsigned cod, TCHAR* chr)
{
	if (isdigit(cod))
	{
		*chr = (TCHAR)cod;
		return 1;
	}
	if (cod >= VK_NUMPAD0&&cod <= VK_NUMPAD9)
	{
		*chr = (TCHAR)cod - VK_NUMPAD0 + 0x30;
		return 1;
	}
	if (cod == 0xbe || cod == 0x6e)
	{
		*chr = '.';
		return 1;
	}
	return 0;
}

void txnudg(int dx, float dy)
{
	float fdy;
	float tflt;
	int ind, tlin;

	if (cloxcnt)
	{
		if (dy)
		{
			fdy = dy*textureScreen.editToPixelRatio;
			for (ind = 0; ind < cloxcnt; ind++)
			{
				tflt = tmpTexturePoints[cloxlst[ind]].y + fdy;
				if (tflt < 0)
					return;
				if (tflt > textureScreen.areaHeight)
					return;
			}
			for (ind = 0; ind < cloxcnt; ind++)
				tmpTexturePoints[cloxlst[ind]].y += fdy;
		}
		else
		{
			for (ind = 0; ind < cloxcnt; ind++)
			{
				tlin = tmpTexturePoints[cloxlst[ind]].line + dx;
				if (tlin < 1)
					return;
				if (tlin > textureScreen.lines)
					return;
			}
			for (ind = 0; ind < cloxcnt; ind++)
				tmpTexturePoints[cloxlst[ind]].line += dx;
		}
	}
	dutxrct(&textureRect);
	setMap(RESTCH);
}

void txsnap()
{
	int ind, cnt;
	float siz2;
	TXPNT*	tp;

	if (textureScreen.index)
	{
		savtxt();
		siz2 = IniFile.gridSize / 2;
		if (cloxcnt)
		{
			for (ind = 0; ind < cloxcnt; ind++)
			{
				tp = &tmpTexturePoints[cloxlst[ind]];
				cnt = (tp->y + siz2) / IniFile.gridSize;
				tp->y = cnt*IniFile.gridSize;
			}
		}
		else
		{
			for (ind = 0; ind < textureScreen.index; ind++)
			{
				tp = &tmpTexturePoints[ind];
				cnt = (tp->y + siz2) / IniFile.gridSize;
				tp->y = cnt*IniFile.gridSize;
			}
		}
		setMap(RESTCH);
	}
}

void txtkey(unsigned cod)
{
	TCHAR chr;

	if (hBtnSideWindow)
	{
		switch (cod)
		{
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

			if (textureInputIndex)
				textureInputIndex--;
			goto txskp;
		}
		if (txdig(cod, &chr))
		{
			textureInputBuffer[textureInputIndex] = chr;
			textureInputIndex++;
		}
	txskp:;
		textureInputBuffer[textureInputIndex] = 0;
		SetWindowText(hBtnSideWindow, textureInputBuffer);
		return;
	}
	switch (cod)
	{
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

	case 192:

		tst();
		break;

	case 'R':

		dutxfn(VRTYP);
		break;

	case 'A':

		dutxfn(ANGTYP);
		break;

	case 'H':

		// ToDo - shouldn't this be HORTYP?
		dutxfn(ANGTYP);
		break;

	case 'E':

		txtlin();
		break;

	case 'Z':
	case 'B':

		if (!setMap(LASTXBAK))
		{
			savtxt();
			txrbak();
		}
		else
		{
			if (rstMap(TXBDIR))
				txrbak();
		}
		txbak();
		return;

	case 'V':

		if (OpenClipboard(ThredWindow))
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

void setxt()
{
	TXPNT*		pbak;
	int			ind, cnt;

	savtxt();
	SelectedForm->wordParam = 0;
	setMap(TXFIL);
	ClipRectSize.cx = SelectedForm->fillSpacing;
	ClipRectSize.cy = SelectedForm->fillInfo.texture.height;
	textureSegments = (RNGCNT*)&MarkedStitchMap;
	pbak = &texturePointsBuffer[SelectedForm->fillInfo.texture.index];
	FillMemory(textureSegments, SelectedForm->fillInfo.texture.lines * sizeof(RNGCNT), 0);
	pbak = &texturePointsBuffer[SelectedForm->fillInfo.texture.index];
	cnt = SelectedForm->fillInfo.texture.count;
	if (cnt)
	{
		for (ind = cnt - 1; ind >= 0; ind--)
		{
			textureSegments[pbak[ind].line - 1].line = ind;
			textureSegments[pbak[ind].line - 1].stitchCount++;
		}
	}
}

void rtrtx()
{
	TXPNT*	tps;

	fvars(ClosestFormToCursor);
	tps = &texturePointsBuffer[SelectedForm->fillInfo.texture.index];
	textureScreen.index = SelectedForm->fillInfo.texture.count;
	MoveMemory(tmpTexturePoints, tps, sizeof(TXPNT)*textureScreen.index);
	textureScreen.areaHeight = SelectedForm->fillInfo.texture.height;
	textureScreen.spacing = SelectedForm->fillSpacing;
	textureScreen.lines = SelectedForm->fillInfo.texture.lines;
	textureScreen.width = textureScreen.lines*textureScreen.spacing + textureScreen.spacing / 2;
	savtxt();
}

void rtrclp()
{
	if (chkMap(FORMSEL))
	{
		fvars(ClosestFormToCursor);
		if (istx(ClosestFormToCursor))
			rtrtx();
		else
			rtrclpfn();
	}
}

void setstxt(unsigned cod, float num)
{
	sprintf_s(HelpBuffer, sizeof(HelpBuffer), "%.2f", (float)num / PFGRAN);
	SetWindowText(GetDlgItem(hDlgDesignSize, cod), HelpBuffer);
}

float getstxt(unsigned cod)
{
	GetWindowText(GetDlgItem(hDlgDesignSize, cod), HelpBuffer, HBUFSIZ);
	return atof(HelpBuffer)*PFGRAN;
}

BOOL chkasp(fPOINT* p_flt)
{
	p_flt->x = getstxt(IDC_DESWID);
	p_flt->y = getstxt(IDC_DESHI);
	if (p_flt->y / p_flt->x == designAspectRatio)
		return 1;
	else
		return 0;
}

BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);

	fPOINT	tflt;

	hDlgDesignSize = hwndlg;
	switch (umsg)
	{
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		setstxt(IDC_DESWID, designSize.x);
		setstxt(IDC_DESHI, designSize.y);
		designAspectRatio = designSize.y / designSize.x;
		CheckDlgButton(hwndlg, IDC_REFILF, chku(CHREF));
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			designSize.x = getstxt(IDC_DESWID);
			designSize.y = getstxt(IDC_DESHI);
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

			if (!chkasp(&tflt))
			{
				if (chkMap(DESCHG))
					setstxt(IDC_DESWID, (float)tflt.y / designAspectRatio);
				else
					setstxt(IDC_DESHI, (float)tflt.x*designAspectRatio);
			}
			break;
		}
	}
	return 0;
}

void sadj(fPOINTATTR* pt)
{
	pt->x = (pt->x - designSizeRect.left)*designSizeRatio.x + designSizeRect.left;
	pt->y = (pt->y - designSizeRect.bottom)*designSizeRatio.y + designSizeRect.bottom;
}

void sadj(fPOINT* pt)
{
	pt->x = (pt->x - designSizeRect.left)*designSizeRatio.x + designSizeRect.left;
	pt->y = (pt->y - designSizeRect.bottom)*designSizeRatio.y + designSizeRect.bottom;
}

void nudfn()
{
	unsigned	ind;
	fPOINT		osiz;

	osiz.x = designSizeRect.right - designSizeRect.left;
	osiz.y = designSizeRect.top - designSizeRect.bottom;
	designSizeRatio.x = designSize.x / osiz.x;
	designSizeRatio.y = designSize.y / osiz.y;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
		sadj(&StitchBuffer[ind]);
	for (ind = 0; ind < FormVertexIndex; ind++)
		sadj(&FormVertices[ind]);
	frmout(ClosestFormToCursor);
}

void nudsiz()
{
	fPOINT		osiz;
	int			flg;
	unsigned	ind;

	savdo();
	flg = 0;
	if (PCSHeader.stitchCount)
	{
		stchrct(&designSizeRect);
		flg = 1;
	}
	else
	{
		if (FormIndex)
		{
			frmrct(&designSizeRect);
			flg = 2;
		}
	}
	if (flg)
	{
		osiz.x = designSize.x = designSizeRect.right - designSizeRect.left;
		osiz.y = designSize.y = designSizeRect.top - designSizeRect.bottom;
		if (DialogBox(ThredInstance, MAKEINTRESOURCE(IDD_SIZ), ThredWindow, (DLGPROC)setsprc))
		{
			flg = 0;
			if (designSize.x > IniFile.hoopSizeX)
			{
				IniFile.hoopSizeX = designSize.x*1.05;
				UnzoomedRect.x = IniFile.hoopSizeX;
				flg = 1;
			}
			if (designSize.y > IniFile.hoopSizeY)
			{
				IniFile.hoopSizeY = designSize.y*1.05;
				UnzoomedRect.y = IniFile.hoopSizeY;
				flg = 1;
			}
			nudfn();
			if (chku(CHREF))
				refilal();
			if (flg)
			{
				movStch();
				zumhom();
				hsizmsg();
			}
			centir();
			for (ind = 0; ind < FormIndex; ind++)
				frmout(ind);
		}
	}
}

void dushft()
{
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

void mvshft()
{
	if (chkMap(BOXZUM) && setMap(VCAPT))
		SetCapture(ThredWindow);
	if (chkMap(BZUMIN))
	{
		if (Msg.wParam&MK_LBUTTON)
		{
			if (setMap(VCAPT))
				SetCapture(ThredWindow);
			unbBox();
			ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
			setMap(BZUM);
			bBox();
		}
	}
}

BOOL inrct(fRECTANGLE rct, fPOINTATTR pt)
{
	if (pt.x < rct.left)
		return 0;
	if (pt.x > rct.right)
		return 0;
	if (pt.y < rct.bottom)
		return 0;
	if (pt.y > rct.top)
		return 0;
	return 1;
}

void setshft()
{
	fRECTANGLE	srct;
	unsigned ind, lin;

	savtxt();
	cloxlst = (unsigned*)&OSequence;
	unbBox();
	rstMap(BZUMIN);
	pxCor2stch(ZoomBoxLine[0]);
	srct.top = SelectedPoint.y;
	srct.left = SelectedPoint.x;
	pxCor2stch(ZoomBoxLine[2]);
	srct.bottom = SelectedPoint.y;
	srct.right = SelectedPoint.x;
	rstMap(TXIN);
	textureScreen.index = 0;
	lin = 1;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		if (inrct(srct, StitchBuffer[ind]))
		{
			setMap(TXIN);
			tmpTexturePoints[textureScreen.index].line = lin;
			tmpTexturePoints[textureScreen.index].y = StitchBuffer[ind].y - srct.bottom;
			textureScreen.index++;
		}
		else
		{
			if (rstMap(TXIN))
				lin++;
		}
	}
	if (textureScreen.index) { lin = tmpTexturePoints[textureScreen.index - 1].line; }
	textureScreen.spacing = (srct.right - srct.left) / lin;
	textureScreen.areaHeight = srct.top - srct.bottom;
	textureScreen.width = textureScreen.spacing*lin + textureScreen.spacing / 2;
	setMap(TXTRED);
	setMap(RESTCH);
}

void setclpspac()
{
	msgflt(IDS_CLPSPAC, IniFile.clipOffset / PFGRAN);
	setMap(NUMIN);
	setMap(SCLPSPAC);
	numWnd();
}

BOOL CALLBACK enumch(HWND hwnd, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	DestroyWindow(hwnd);
	return 1;
}

void clrstch()
{
	while (EnumChildWindows(MainStitchWin, enumch, 0));
}

BOOL txnam(char *nam, int sizeNam)
{
	char* pchr;

	strcpy_s(nam, sizeNam, __argv[0]);
	pchr = strrchr(nam, '\\');
	if (pchr)
	{
		pchr++;
		strcpy_s(pchr, sizeNam - (pchr - nam), "thred.txr");
		return 1;
	}
	else
		return 0;
}

void txdun()
{
	char nam[_MAX_PATH];
	HANDLE hnam;
	unsigned long rot;
	int ind;
	//char* signature="txh";

	if (textureHistory[0].count)
	{
		if (txnam(nam, sizeof(nam)))
		{
			hnam = CreateFile(nam, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (hnam != INVALID_HANDLE_VALUE)
			{
				WriteFile(hnam, (char*)"txh", 4, &rot, 0);
				WriteFile(hnam, (int*)&textureHistoryIndex, 4, &rot, 0);
				WriteFile(hnam, (TXHST*)&textureHistory, sizeof(TXHST) * 16, &rot, 0);
				for (ind = 0; ind < 16; ind++)
				{
					if (textureHistory[ind].count)
						WriteFile(hnam, (TXPNT*)textureHistory[ind].texturePoint, textureHistory[ind].count * sizeof(TXPNT), &rot, 0);
				}
			}
			CloseHandle(hnam);
		}
	}
}

// Suppress C6031: return value ignored
#pragma warning(push)
#pragma warning(disable : 6031)
void redtx()
{
	char nam[_MAX_PATH];
	HANDLE hnam;
	DWORD l_BytesRead;
	unsigned int ind;
	char sig[4] = { 0 };

	textureHistoryIndex = 15;
	ZeroMemory(&textureHistory, sizeof(TXHST) * 16);
	if (txnam(nam, sizeof(nam)))
	{
		hnam = CreateFile(nam, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (hnam != INVALID_HANDLE_VALUE)
		{
			ReadFile(hnam, (char*)&sig, 4, &l_BytesRead, 0);
			if (!strcmp(sig, "txh"))
			{
				ReadFile(hnam, (int*)&textureHistoryIndex, 4, &l_BytesRead, 0);
				ReadFile(hnam, (TXHST*)&textureHistory, sizeof(TXHST) * 16, &l_BytesRead, 0);
				//ToDo - texturePoint should be a null pointer at this point as no memory has been allocated, but it is not
				//       because the old pointer value is read in from the file, so zero it out
				for (ind = 0; ind < (l_BytesRead / sizeof(TXHST)); ind++)
				{
					if (textureHistory[ind].count)
					{
						textureHistory[ind].texturePoint = new TXPNT[textureHistory[ind].count];
						ReadFile((TXPNT*)hnam, textureHistory[ind].texturePoint, sizeof(TXPNT)*textureHistory[ind].count, &l_BytesRead, 0);
					}
				}
				setMap(WASTXBAK);
			}
		}
		CloseHandle(hnam);
	}
	redtbak();
}
#pragma warning(pop)

void setangf(double tang)
{
	unsigned ind;
	double angbak;

	angbak = RotationAngle;
	RotationAngle = tang;
	MoveMemory(&AngledForm, SelectedForm, sizeof(FRMHED));
	MoveMemory(&AngledFormVertices, CurrentFormVertices, sizeof(fPOINT)*SideCount);
	RotationCenter.x = (double)(AngledForm.rectangle.right - AngledForm.rectangle.left) / 2 + AngledForm.rectangle.left;
	RotationCenter.y = (double)(AngledForm.rectangle.top - AngledForm.rectangle.bottom) / 2 + AngledForm.rectangle.bottom;
	AngledForm.vertices = AngledFormVertices;
	if (RotationAngle)
	{
		for (ind = 0; ind < SideCount; ind++)
			rotflt(&AngledFormVertices[ind]);
	}
	SelectedForm = &AngledForm;
	CurrentFormVertices = AngledFormVertices;
	RotationAngle = angbak;
}

void chgwrn()
{
	toglu(WRNOF);
	wrnmen();
	setMap(DUMEN);
}

void chgchk(int cod)
{
	IniFile.dataCheck = cod;
	chkmen();
	setMap(DUMEN);
}

void lodchk()
{
	unsigned	ind, at;

	delinf();
	for (ind = 0; ind < FormIndex; ind++) {

		SelectedForm = &FormList[ind];
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
		frmout(ind);
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	}
	clRmap((MAXFORMS >> 5) + 1);
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

		at = StitchBuffer[ind].attribute;
		if ((at&TYPMSK) == TYPFRM)
			setr((at&FRMSK) >> FRMSHFT);

	}
	for (ind = 0; ind < FormIndex; ind++) {

		if (!chkr(ind))
			FormList[ind].fillType = 0;
	}
	clRmap((MAXFORMS >> 5) + 1);
	for (ind = 0; ind < PCSHeader.stitchCount; ind++) {

		at = StitchBuffer[ind].attribute;
		if (at&TYPBRD)
			setr((at&FRMSK) >> FRMSHFT);
	}
	for (ind = 0; ind < FormIndex; ind++) {

		if (!chkr(ind))
			FormList[ind].edgeType = 0;
	}
}

void chkclp(FRMHED* fp, BADCNTS* bc)
{
	if (bc->clip == fp->angleOrClipData.clip - ClipPoints)
		bc->clip += fp->lengthOrCount.clipCount;
	else
		bc->attribute |= BADCLP;
}

void chkeclp(FRMHED* fp, BADCNTS* bc)
{
	if (bc->clip == fp->borderClipData - ClipPoints)
		bc->clip += fp->clipEntries;
	else
		bc->attribute |= BADCLP;
}

unsigned frmchkfn()
{
	unsigned	ind;
	FRMHED*		fp;
	BADCNTS		bc;

	FillMemory(&bc, sizeof(BADCNTS), 0);
	if (FormIndex)
	{
		for (ind = 0; ind < FormIndex; ind++)
		{
			fp = &FormList[ind];
			if (!(bc.attribute&BADFLT))
			{
				if (!fp->sides)
					bc.attribute |= BADFLT;
				if (bc.flt == fp->vertices - FormVertices)
					bc.flt += fp->sides;
				else
					bc.attribute |= BADFLT;
			}
			if (!(bc.attribute&BADCLP))
			{
				if (isclp(ind))
					chkclp(fp, &bc);
				if (iseclp(ind))
					chkeclp(fp, &bc);
			}
			if (fp->type == SAT&&fp->satinGuideCount)
			{
				if (!(bc.attribute&BADSAT))
				{
					if (bc.sat == fp->satinOrAngle.sac - SatinConnects)
						bc.sat += fp->satinGuideCount;
					else
						bc.attribute |= BADSAT;
				}
			}
			if (istx(ind))
			{
				if (!(bc.attribute&BADTX))
				{
					if (bc.tx == fp->fillInfo.texture.index)
						bc.tx += fp->fillInfo.texture.count;
					else
						bc.attribute |= BADTX;
				}
			}
			if (bc.attribute == (BADFLT | BADCLP | BADSAT | BADTX))
				break;
		}
		if (bc.flt != (int)FormVertexIndex)
			bc.attribute |= BADFLT;
		if (bc.clip != (int)ClipPointIndex)
			bc.attribute |= BADCLP;
		if (bc.sat != (int)SatinConnectIndex)
			bc.attribute |= BADSAT;
		if (bc.tx != textureIndex)
			bc.attribute |= BADTX;
	}
	return bc.attribute;
}

void frmchkx()
{
	unsigned cod;

	if (IniFile.dataCheck)
	{
		cod = frmchkfn();
		switch (IniFile.dataCheck)
		{
		case 1:

			if (cod)
				datmsg(cod);
			break;

		case 2:

			if (cod)
				repar();
			break;

		case 3:

			if (cod)
			{
				repar();
				tabmsg(IDS_DATREP);
			}
		}
	}
}

void bcup(unsigned find, BADCNTS* bc)
{
	FRMHED* fp;

	fp = &FormList[find];
	if (isclp(find))
		bc->clip += fp->lengthOrCount.clipCount;
	if (iseclp(find))
		bc->clip += fp->clipEntries;
	if (fp->type == SAT)
		bc->sat += fp->satinGuideCount;
	if (istx(find))
		bc->tx += fp->fillInfo.texture.count;
}

void chkfstch()
{
	unsigned cod, ind;

	cod = FormIndex << FRMSHFT;
	for (ind = 0; ind < PCSHeader.stitchCount; ind++)
	{
		if ((StitchBuffer[ind].attribute&FRMSK) >= cod)
			StitchBuffer[ind].attribute = NOTFRM;
	}
}

void repflt()
{
	fPOINT* tflt;
	unsigned ind, loc, cnt, dif;
	FRMHED* fp;
	BADCNTS bc;

	loc = 0;
	for (ind = 0; ind < FormIndex; ind++)
	{
		if (FormList[ind].sides)
			MoveMemory(&FormList[loc++], &FormList[ind], sizeof(FRMHED));
	}
	FormIndex = loc;
	ZeroMemory(&bc, sizeof(BADCNTS));
	tflt = (fPOINT*)BSequence;
	loc = cnt = 0;
	for (ind = 0; ind < FormIndex; ind++)
	{
		fp = &FormList[ind];
		dif = fp->vertices - FormVertices;
		if (FormVertexIndex >= dif + fp->sides)
		{
			MoveMemory(&tflt[loc], fp->vertices, fp->sides * sizeof(fPOINT));
			fp->vertices = &FormVertices[loc];
			loc += fp->sides;
			bcup(ind, &bc);
		}
		else
		{
			if (dif < FormVertexIndex)
			{
				fp->sides = FormVertexIndex - dif;
				delsac(ind);
				MoveMemory(&tflt[loc], fp->vertices, fp->sides * sizeof(fPOINT));
				bcup(ind, &bc);
			}
			else
			{
				FormIndex = ind;
				ClipPointIndex = bc.clip;
				SatinConnectIndex = bc.sat;
				textureIndex = bc.tx;
				chkfstch();
				adbad(IDS_FRMDAT, FormIndex - ind + 1);
				goto rfltskp;
			}
		}
	}
	FormVertexIndex = loc;
rfltskp:;
	MoveMemory(FormVertices, tflt, sizeof(fPOINT)*FormVertexIndex);
}

void repclp()
{
	FRMHED*		fp;
	unsigned	ind, cnt, loc, bcnt;
	fPOINT*		tclps;

	bcnt = cnt = 0;
	tclps = (fPOINT*)&BSequence;
	for (ind = 0; ind < FormIndex; ind++)
	{
		fp = &FormList[ind];
		if (isclp(ind))
		{
			loc = fp->angleOrClipData.clip - ClipPoints;
			if (loc + fp->lengthOrCount.clipCount < ClipPointIndex)
			{
				MoveMemory(&tclps[cnt], fp->angleOrClipData.clip, sizeof(fPOINT)*fp->lengthOrCount.clipCount);
				fp->angleOrClipData.clip = &ClipPoints[cnt];
				cnt += fp->lengthOrCount.clipCount;
			}
			else
			{
				if (loc < ClipPointIndex)
				{
					fp->lengthOrCount.clipCount = FormVertexIndex - loc;
					MoveMemory(&tclps[cnt], fp->angleOrClipData.clip, sizeof(fPOINT)*fp->lengthOrCount.clipCount);
					fp->angleOrClipData.clip = &ClipPoints[cnt];
					cnt += fp->lengthOrCount.clipCount;
				}
				else
				{
					bcnt++;
					fp->fillType = 0;
				}
			}
		}
		if (iseclp(ind))
		{
			loc = fp->borderClipData - ClipPoints;
			if (loc + fp->clipEntries < ClipPointIndex)
			{
				MoveMemory(&tclps[cnt], fp->borderClipData, sizeof(fPOINT)*fp->clipEntries);
				fp->borderClipData = &ClipPoints[cnt];
				cnt += fp->clipEntries;
			}
			else
			{
				if (loc < ClipPointIndex)
				{
					fp->clipEntries = FormVertexIndex - loc;
					MoveMemory(&tclps[cnt], fp->borderClipData, sizeof(fPOINT)*fp->clipEntries);
					fp->borderClipData = &ClipPoints[cnt];
					cnt += fp->clipEntries;
				}
				else
				{
					bcnt++;
					fp->fillType = 0;
				}
			}
		}
	}
	MoveMemory(&ClipPoints, tclps, cnt * sizeof(fPOINT));
	ClipPointIndex = cnt;
	if (bcnt)
		adbad(IDS_CLPDAT, bcnt);
}

void repsat()
{
	unsigned ind, loc, cnt, dif;
	FRMHED* fp;
	BADCNTS bc;

	ZeroMemory(&bc, sizeof(BADCNTS));
	loc = cnt = 0;
	for (ind = 0; ind < FormIndex; ind++)
	{
		fp = &FormList[ind];
		if (fp->type == SAT)
		{
			dif = fp->satinOrAngle.sac - SatinConnects;
			if (FormVertexIndex > dif + fp->sides)
			{
				MoveMemory(&SatinConnects[loc], fp->satinOrAngle.sac, fp->satinGuideCount * sizeof(SATCON));
				fp->satinOrAngle.sac = &SatinConnects[loc];
				loc += fp->satinGuideCount;
				bcup(ind, &bc);
			}
			else
			{
				if (dif < SatinConnectIndex)
				{
					fp->satinGuideCount = SatinConnectIndex - dif;
					MoveMemory(&SatinConnects[loc], fp->satinOrAngle.sac, fp->satinGuideCount * sizeof(SATCON));
					bcup(ind, &bc);
				}
				else
				{
					loc = bc.sat;
					fp->satinGuideCount = 0;
				}
			}
		}
	}
	SatinConnectIndex = loc;
}

void reptx()
{
	unsigned ind, loc, cnt;
	FRMHED* fp;
	BADCNTS bc;

	ZeroMemory(&bc, sizeof(BADCNTS));
	loc = cnt = 0;
	for (ind = 0; ind < FormIndex; ind++)
	{
		if (istx(ind))
		{
			fp = &FormList[ind];
			if (textureIndex > fp->fillInfo.texture.index + fp->fillInfo.texture.count)
			{
				MoveMemory(&SatinConnects[loc], &SatinConnects[fp->fillInfo.texture.index], fp->fillInfo.texture.count * sizeof(SATCON));
				fp->fillInfo.texture.index = loc;
				loc += fp->fillInfo.texture.count;
				bcup(ind, &bc);
			}
			else
			{
				if (textureIndex > fp->fillInfo.texture.index)
				{
					fp->fillInfo.texture.count = textureIndex - fp->fillInfo.texture.index;
					MoveMemory(&SatinConnects[loc], &SatinConnects[fp->fillInfo.texture.index], fp->fillInfo.texture.count * sizeof(SATCON));
					fp->fillInfo.texture.index = loc;
					bcup(ind, &bc);
					loc = bc.tx;
				}
				else
					fp->fillType = 0;
			}
		}
	}
	textureIndex = loc;
}

void repar()
{
	unsigned cod;

	savdo();
	StringData = MsgBuffer;
	cod = frmchkfn();
	if (cod&BADFLT)
		repflt();
	if (cod&BADCLP)
		repclp();
	if (cod&BADSAT)
		repsat();
	if (cod&BADTX)
		reptx();
	lodchk();
	setMap(RESTCH);
	if (StringData != MsgBuffer)
	{
		StringData--;
		*StringData = 0;
		shoMsg(MsgBuffer);
	}
}

void tst()
{
	strcpy_s(IniFile.designerName, "Mr");
	strcpy_s(ThrName, IniFile.designerName);
	setMap(RESTCH);
}
