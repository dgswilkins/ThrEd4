#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <tchar.h>
#include "lang.h"
#include "resource.h"
#include "thred.h"

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable:4244)

void repar();
void tst();
void chktxnum();
void txof();
void rotbak();
void txbak();
void redtbak();
void txdelal();
unsigned dutyp(unsigned tat);
void deltx();

extern void delmclp(unsigned fpnt);
extern void adbad(unsigned cod, unsigned cnt);
extern TCHAR* sdat;
extern void delclps(unsigned ind);
extern void delsac(unsigned fpnt);
extern void chkmen();
extern void datmsg(unsigned cod);
extern void delinf();
extern unsigned clpad;
extern SATCON satks[MAXSAC];
extern fPOINT clipboardPoints[MAXCLPNTS];
extern unsigned toglu(unsigned bPnt);
extern void wrnmen();
extern unsigned markedStitchMap[RMAPSIZ];
extern fPOINT zoomBoxOrigin;
extern void ritfil();
extern void angclpfn();
extern unsigned clipboardStitchCount;
extern fPOINTATTRIBUTE clipBuffer[MAXFRMLINS];
extern fRECTANGLE clipboardRect;
extern FLSIZ clipboardRectSize;
extern void centir();
extern void movStch();
extern void hsizmsg();
extern void chkhup();
extern void refilal();
extern void stchrct(fRECTANGLE* rct);
extern void frmrct(fRECTANGLE* rct);
extern void rtrclpfn();
extern fPOINT selectedPoint;
extern void butxt(unsigned ind, TCHAR* str);
extern void unthum();
extern HWND hMainStitchWin;
extern unsigned buttonWidthX3;
extern void redraw(HWND dWnd);
extern HWND hButtonWin[9];
extern BOOL chkr(unsigned pbit);
extern void setr(unsigned pbit);
extern void clRmap(unsigned len);
extern void zumhom();
extern unsigned buttonHeight;
extern POINT flin[MAXFRMLINS];
extern unsigned hThrEdClip;
extern TCHAR* thrEdClipFormat;
extern HGLOBAL hClipMem;
extern FRMHED angfrm;
extern FORMCLIP* clipboardFormData;
extern HPEN gridPen;
extern unsigned satkad;
extern POINT zoomBoxLine[5];
extern MSG msg;
extern POINT stitchWindowOrigin;
extern unsigned px2stch();
extern HWND hVerticalScrollBar;
extern HWND hHorizontalScrollBar;
extern HDC StitchWindowMemDC;
extern HDC StitchWindowDC;
extern RECT stitchWindowClientRect;
extern HBRUSH hBackgroundBrush;
extern TCHAR fileName[_MAX_PATH];
extern void movStch();
extern void shoseln(unsigned cod0, unsigned cod1);
extern unsigned closestVertexToCursor;
extern BOOL isclp(unsigned find);
extern BOOL iseclp(unsigned find);
extern TCHAR hlpbuf[HBUFSIZ];
extern unsigned selectedFormCount;
extern BOOL isin(float pntx, float pnty);
extern void clpfil();
extern unsigned	underlayColor;
extern fPOINT lastpnt;
extern void refilfn();
extern void filvrt();
extern unsigned closflt(float px, float py);
extern unsigned short prv(unsigned ind);
extern unsigned getlast();
extern BOOL cisin(float pntx, float pnty);
extern fPOINTATTRIBUTE* hifstch;
extern SATCON* adsatk(unsigned cnt);
extern unsigned rstu(unsigned bPnt);
extern unsigned chku(unsigned bPnt);
extern unsigned setu(unsigned bPnt);
extern unsigned activeLayer;
extern fPOINT* adflt(unsigned cnt);
extern void frmclr(FRMHED* dst);
extern void setmfrm();
extern void mdufrm();
extern unsigned newFormVertexCount;
extern fPOINT fmovdif;
extern void dumrk(double pntx, double pnty);
extern dRECTANGLE zoomRect;
extern POINT unzoomedRect;
extern void unmsg();
extern fPOINT formPoints[MAXFRMPNTS];
extern fPOINT* currentFormVertices;
extern unsigned fltad;
extern unsigned char cryptkey[4096];
extern fPOINT angflt[MAXFRMLINS];
extern FRMHED angfrm;
extern void frmcpy(FRMHED* dst, FRMHED* src);
extern float midl(float hi, float lo);
extern dPOINT rotationCenter;
extern void rotflt(fPOINT* pnt);
extern double rotationAngle;
extern void filang();
extern void fnhor();
extern fPOINT* lconflt;
extern void lcon();
extern void bakseq();
extern BOOL chkmax(unsigned arg0, unsigned arg1);
extern unsigned isind;
extern void msgflt(unsigned msgid, float par);
extern unsigned short nxt(unsigned short ind);
extern void mvstchs(unsigned dst, unsigned src, unsigned cnt);
extern void numWnd();
extern void satout(double satwid);
extern void tabmsg(unsigned cod);
extern unsigned auth;
extern void frmout(unsigned ind);
extern TCHAR thrName[_MAX_PATH];
extern TCHAR auxName[_MAX_PATH];
extern void save();
extern COLORREF userColor[16];
extern unsigned psg();
extern unsigned psgacc;
extern void setknots();
extern void refil();
extern void moveStitchPoints(fPOINTATTRIBUTE* dst, fPOINTATTRIBUTE* src);
extern BOOL filmsgs(unsigned cod);
extern unsigned find1st();
extern double StitchSpace;
extern void savdo();
extern void delmfil(unsigned col);
extern double userStitchLength;
extern unsigned activeColor;
extern void rseq(unsigned strt, unsigned fin, unsigned ostrt, unsigned at);
extern unsigned delpnt;
extern void makspac(unsigned strt, unsigned cnt);
extern void chkseq(BOOL brd);
extern HWND hWnd;
extern HINSTANCE hInst;
extern INIFILE iniFile;
extern unsigned xpnt;
extern fPOINT oseq[OSEQLEN];
extern unsigned opnt;
extern unsigned toglMap(unsigned bPnt);
extern unsigned seqpnt;
extern void satfil();
extern unsigned rstMap(unsigned bPnt);
extern void filsat();
extern FRMHED* ptrSelectedForm;
extern unsigned closestFormToCursor;
extern FRMHED formList[MAXFORMS];
extern void fvars(unsigned ind);
extern void satout(double satwid);
extern fPOINT* opnts;
extern fPOINT* ipnts;
extern void fnvrt();
extern void fshor();
extern void coltab();
extern unsigned setMap(unsigned bPnt);
extern unsigned chkMap(unsigned bPnt);
extern unsigned short sides;
extern PCSHEADER header;
extern fPOINTATTRIBUTE stitchBuffer[MAXPCS];
extern void mvstch(unsigned dst, unsigned src);
extern unsigned	formIndex;
extern BSEQPNT bseq[BSEQLEN];
extern unsigned selectedFormCount;
extern unsigned short selectedFormList[MAXFORMS];
extern unsigned closestPointIndex;
extern TCHAR msgbuf[MSGSIZ];
extern TCHAR* stab[STR_LEN];
extern void shoMsg(TCHAR* str);
extern void filsfn();

unsigned short daztab[] =
{
	IDS_DAZCRV,
	IDS_DAZSAW,
	IDS_DAZRMP,
	IDS_DAZRAG,
	IDS_DAZCOG,
	IDS_DAZHART,
};

RNGCNT*		txsegs;	//txture fill groups of points
fRECTANGLE		isrct;	//isin rectangle
dPOINT		sizrat;	//design size ratio
fRECTANGLE		sizrct;	//design size rectangle
float		daspct;	//design aspect ratio
HWND		sizdlg;	//change design size dialog window
fPOINT		dsgnsiz;	//design size
TXPNT		txpnts[MAXSEQ];//buffer for textured fill points
int			txad;	//next textured fill point index
unsigned	txsidtyp; //id of the window being updated
TCHAR		txbuf[16];//texture fill number buffer
int			txnind;	//text number pointer
HWND		hbsid;	//button side window
unsigned	clofbak;//backup for the close form pointer
RECT		txprct; //screen selected textreu points rectangle
TXTRCT		txrct;	//selected texture points rectangle
POINT		cloxref; //original location of selected texture points
int			cloxcnt; //number of selected texture points
unsigned*	cloxlst; //list of selected points
POINT		txtloc;	//texture editor move cursor location
HPEN		xpen; //texture editor cross pen
TXTSCR		tscr; //texture editor layout parameters
TXPNT		txs[MAXSEQ];//textured fill points
TXPNT		txtmp[16384];//temporary storage for textured fill data
unsigned	txis[MAXFORMS];//testured fill indices
unsigned	colord[16];	//color order adjusted for aplique
unsigned	lastcols[16]; //last stitches in a color in a form fill
INTINF		itf;	//interleave data
fPOINT		iseq[MAXSEQ]; //storage for interleave points
unsigned	isind; //index into the interleave points
FSTRTS		fstrts;//fill start data for refill
INSREC		isinds[10];//indices into interleave points
unsigned	isind2;	//index into interleave indices
unsigned	smap;	//fill starts bitmap
unsigned	lastfltx;//last form point used in a fill
unsigned*	frmcnts;//form fill type counters for sort
fPOINT*		uflt;	//underlay offset points
unsigned	fthtyp;	//type of feather fill
float		fthrat;	//feather ratio
float		fthflr;	//smallest stitch allowed
float		flr2;	//twice smallest stitch
float		xrat;	//local feather ratio
float		brat;	//feather ratio from form
unsigned	xat;	//exterded form attribute
unsigned	fthup;	//feather up count
unsigned	fthdwn;	//feather down count
unsigned	fthnum;	//up count plus down count
unsigned	faznum;
unsigned	faz;
fPOINT		fthseq[MAXSEQ];
unsigned	upfth;
unsigned	dwnfth;
//char*		dgbuf;

char*		knam = "BhbcT\\BTRdaXch3TbRaX_c^a"; //SystemSecurityDescriptor
char*		ksig = "DbTaEP[XSPcX^]2^ST";		  //UserValidationCode
char*		ktim = ";XUTBT`dT]RT";			  //LifeSequence

float		fltstp;
float		fltpos;
float		fltfaz;
float		fltup;
float		fltdwn;
float		fltrat;

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

TXHST	thsts[16];	//txt editor history headers
int		ptxhst;		//pointer to the next texture history buffer

void setfchk()
{
	if (iniFile.dataCheck)
		setMap(FCHK);
}

//#ifdef _DEBUG

void prbug()
{
	OutputDebugString(msgbuf);
}
//#endif

BOOL istx(unsigned find)
{
	FRMHED* pfrm;

	pfrm = &formList[find];
	if (pfrm->fillType == TXVRTF)
		return 1;
	if (pfrm->fillType == TXHORF)
		return 1;
	if (pfrm->fillType == TXANGF)
		return 1;
	return 0;
}

TXPNT* adtx(int cnt) {

	unsigned ind = txad;

	txad += cnt;
	return &txpnts[ind];
}

void txspac(int strt, unsigned cnt)
{
	unsigned	ind;

	MoveMemory(&txpnts[strt + cnt], &txpnts[strt], (txad - strt) & sizeof(TXPNT));
	for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
	{
		if (istx(ind))
			formList[ind].fillInfo.texture.index += cnt;
	}
	txad += cnt;
}

void rstxt()
{
	cloxcnt = 0;
	rstMap(TXTMOV);
	rstMap(BZUM);
	rstMap(BZUMIN);
	setMap(RESTCH);
	rstMap(POLIMOV);
	DestroyWindow(hbsid);
	hbsid = 0;
	setMap(RESTCH);
}

void txrbak()
{
	ptxhst--;
	ptxhst &= 0xf;
}

void txrfor()
{
	ptxhst++;
	ptxhst &= 0xf;
}

BOOL chktxh(TXHST* phst)
{
	int ind;

	if (phst->count != tscr.index)
		return 1;
	if (phst->height != tscr.areaHeight)
		return 1;
	if (phst->spacing != tscr.spacing)
		return 1;
	if (phst->width != tscr.width)
		return 1;
	for (ind = 0; ind < tscr.index; ind++)
	{
		if (txtmp[ind].line != phst->texturePoint[ind].line)
			return 1;
		if (txtmp[ind].y != phst->texturePoint[ind].y)
			return 1;
	}
	return 0;
}

void savtxt()
{
	TXHST*	 phst;

	if (tscr.index)
	{
		phst = &thsts[ptxhst];
		if (chktxh(phst))
		{
			setMap(WASTXBAK);
			rstMap(TXBDIR);
			rstMap(LASTXBAK);
			txrfor();
			phst = &thsts[ptxhst];
			phst->count = tscr.index;
			phst->height = tscr.areaHeight;
			phst->width = tscr.width;
			phst->spacing = tscr.spacing;
			if (phst->texturePoint)
			{
				delete[](phst->texturePoint);
				phst->texturePoint = 0;
			}
			phst->texturePoint = new TXPNT[phst->count];
			MoveMemory(phst->texturePoint, txtmp, phst->count * sizeof(TXPNT));
		}
	}
}

void deorg(POINT* pt)
{
	pt->x = msg.pt.x - stitchWindowOrigin.x;
	pt->y = msg.pt.y - stitchWindowOrigin.y;
}

BOOL chkrypt(unsigned arg0, unsigned arg1, unsigned arg2, unsigned arg3) {

	if (cryptkey[100] == arg0&&
		cryptkey[25] == arg1&&
		cryptkey[104] == arg2&&
		cryptkey[2074] == arg3) {
		return 1;
	} else {
		return 0;
	}
}

void fthvars() {

	rstMap(BARSAT);
	rstMap(FTHR);
	fthtyp = ptrSelectedForm->fillInfo.feather.fillType;
	brat = ptrSelectedForm->fillInfo.feather.ratio;
	fthflr = ptrSelectedForm->fillInfo.feather.minStitchSize;
	flr2 = fthflr * 2;
	fthnum = ptrSelectedForm->fillInfo.feather.count;
	xat = ptrSelectedForm->extendedAttribute;
	upfth = fthup = ptrSelectedForm->fillInfo.feather.upCount;
	dwnfth = fthdwn = ptrSelectedForm->fillInfo.feather.downCount;
	faznum = fthup + fthdwn;
	if (xat&AT_FTHBLND)
		setMap(BARSAT);
	else
		setMap(FTHR);
}

float durat(float strt, float fin) {

	return (fin - strt)*fthrat + strt;
}

float duxrat(float strt, float fin) {

	return (fin - strt)*xrat + strt;
}

void duxrats(unsigned strt, unsigned fin, fPOINT* opt) {

	opt->x = duxrat(bseq[fin].x, bseq[strt].x);
	opt->y = duxrat(bseq[fin].y, bseq[strt].y);
}

void durats(unsigned ind, fPOINT* pflt) {

	double	olen;
	fPOINT	ipnt;

	olen = hypot(bseq[ind + 1].x - bseq[ind].x, bseq[ind + 1].y - bseq[ind].y);
	if (olen < fthflr) {

		pflt->x = bseq[ind].x;
		pflt->y = bseq[ind].y;
	}
	else {

		xrat = fthflr / olen;
		ipnt.x = duxrat(bseq[ind + 1].x, bseq[ind].x);
		ipnt.y = duxrat(bseq[ind + 1].y, bseq[ind].y);
		pflt->x = durat(ipnt.x, bseq[ind].x);
		pflt->y = durat(ipnt.y, bseq[ind].y);
	}
}

void xoseq(unsigned ind) {

	oseq[opnt].x = bseq[ind].x;
	oseq[opnt].y = bseq[ind].y;
	opnt++;
}

void xpfth(unsigned ind) {

	fthseq[xpnt].x = bseq[ind].x;
	fthseq[xpnt].y = bseq[ind].y;
	xpnt++;
}

unsigned bpsg() {

	unsigned tmp;

	if (!psgacc)
		psgacc = FSED;
	tmp = psgacc & 0x40000008;
	psgacc >>= 1;
	if (tmp == 0x8 || tmp == 0x40000000)
		psgacc |= 0x40000000;
	return psgacc;
}

void nurat() {

	float	rem;

	rem = fmod(fltpos, 1);
	switch (fthtyp) {

	case FTHPSG:

		if (fthup) {

			if (upfth) {

				fthrat = (float)(fthnum - (psg() % fthnum)) / fthnum;
				upfth--;
			}
			else {

				fthrat = (float)(fthnum - (bpsg() % fthnum)) / fthnum;
				if (dwnfth)
					dwnfth--;
				else {

					psgacc = FSED;
					upfth = fthup;
					dwnfth = fthdwn;
				}
			}
		}
		else
			fthrat = (float)(fthnum - (psg() % fthnum)) / fthnum;
		fthrat *= brat;
		break;

	case FTHFAZ:

		if (faz >= fthup)
			fthrat = 1;
		else
			fthrat = brat;
		break;

	case FTHSIN:

		if (rem > fltrat)
			fthrat = sin((1 - rem) / (1 - fltrat)*PI + PI)*0.5 + 0.5;
		else
			fthrat = sin(rem / fltrat*PI)*0.5 + 0.5;
		fthrat *= brat;
		break;

	case FTHSIN2:

		if (rem > fltrat)
			fthrat = sin((1 - rem) / (1 - fltrat)*PI);
		else
			fthrat = sin(rem / fltrat*PI);
		fthrat *= brat;
		break;

	case FTHRMP:

		if (rem > fltrat)
			fthrat = (1 - rem) / (1 - fltrat);
		else
			fthrat = rem / fltrat;
		fthrat *= brat;
		break;

	case FTHLIN:
	default:

		fthrat = brat;
	}
	++faz %= faznum;
	fltpos += fltstp;
}

void fthfn(unsigned ind) {

	nurat();
	durats(ind, &oseq[ind]);
}

void ratpnt(unsigned pt0, unsigned pt1, fPOINT* opt) {

	opt->x = (bseq[pt1].x - bseq[pt0].x)*fthrat + bseq[pt0].x;
	opt->y = (bseq[pt1].y - bseq[pt0].y)*fthrat + bseq[pt0].y;
}

void midpnt(fPOINT pt0, fPOINT pt1, fPOINT* opt) {

	opt->x = (pt1.x - pt0.x)*(float)0.5 + pt0.x;
	opt->y = (pt1.y - pt0.y)*(float)0.5 + pt0.y;
}

void xratf(fPOINT pt0, fPOINT pt1, fPOINT* opt) {

	opt->x = (pt1.x - pt0.x)*xrat + pt0.x;
	opt->y = (pt1.y - pt0.y)*xrat + pt0.y;
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
	len = hypot(bseq[ind + 1].y - bseq[ind].y, bseq[ind + 1].x - bseq[ind].x);
	if (len < flr2)
	{
		fthrat = 0.5;
		ratpnt(ind, ind + 1, &pnt0);
		ratpnt(ind + 3, ind + 2, &pnt1);
	}
	else
	{
		xrat = fthflr / len;
		duxrats(ind, ind + 1, &pnt0l);
		duxrats(ind + 3, ind + 2, &pnt1l);
		xrat = 1 - xrat;
		duxrats(ind, ind + 1, &pnt0h);
		duxrats(ind + 3, ind + 2, &pnt1h);
		xrat = fthrat;
		xratf(pnt0l, pnt0h, &pnt0);
		xratf(pnt1l, pnt1h, &pnt1);
	}
	midpnt(pnt0, pnt1, &pntm);
	xoseq(ind);
	oseq[opnt].x = pntm.x;
	oseq[opnt].y = pntm.y;
	opnt++;
	xpfth(ind + 1);
	fthseq[xpnt].x = pntm.x;
	fthseq[xpnt].y = pntm.y;
	xpnt++;
}

void duoseq(unsigned ind) {

	oseq[ind].x = bseq[ind].x;
	oseq[ind].y = bseq[ind].y;
}

void fthdfn(unsigned ind) {

	double	len;
	fPOINT	mpnt;
	fPOINT	pt0;
	fPOINT	pt1;

	nurat();
	len = hypot(bseq[ind + 1].y - bseq[ind].y, bseq[ind + 1].x - bseq[ind].x);
	duoseq(ind);
	duoseq(ind + 1);
	if (len > fthflr) {

		xrat = 0.5;
		duxrats(ind + 1, ind, &mpnt);
		xrat = fthflr / len / 2;
		xratf(mpnt, oseq[ind], &pt0);
		xratf(mpnt, oseq[ind + 1], &pt1);
		xrat = fthrat;
		xratf(pt0, oseq[ind], &oseq[ind]);
		xratf(pt1, oseq[ind + 1], &oseq[ind + 1]);
	}
}

void fthrfn() {

	unsigned	ind, res;
	double		bakspac;

	psgacc = FSED;
	fthvars();
	bakspac = StitchSpace;
	StitchSpace = ptrSelectedForm->fillSpacing;
	satfil();
	bseq[0].attribute = 0;
	bseq[1].attribute = 1;
	if (!faznum)
		faznum = 1;
	ind = seqpnt / (faznum << 2);
	res = seqpnt % (faznum << 2);
	if (res > (faznum << 1))
		ind++;
	fltpos = 0;
	fltstp = (float)4 / seqpnt*ind;
	fltfaz = (float)seqpnt / ind;
	fltrat = (float)upfth / faznum;
	fltup = fltfaz*fltrat;
	fltdwn = fltfaz - fltup;
	ptrSelectedForm->fillType = FTHF;
	faz = 1;
	bseq[seqpnt].x = bseq[seqpnt - 2].x;
	bseq[seqpnt].y = bseq[seqpnt - 2].y;
	bseq[seqpnt].attribute = bseq[seqpnt - 2].attribute;
	bseq[seqpnt + 1].x = bseq[seqpnt - 1].x;
	bseq[seqpnt + 1].y = bseq[seqpnt - 1].y;
	bseq[seqpnt + 1].attribute = bseq[seqpnt - 1].attribute;
	if (xat&AT_FTHBLND) {

		opnt = xpnt = 0;
		for (ind = 0; ind < seqpnt; ind++)
		{
			if (!bseq[ind].attribute)
				fthrbfn(ind);
		}
	}
	else {

		if (ptrSelectedForm->extendedAttribute&AT_FTHDWN) {

			for (ind = 0; ind <= seqpnt; ind++) {

				if (!bseq[ind].attribute)
					fthdfn(ind);
			}
			ind--;
		}
		else {

			for (ind = 0; ind <= seqpnt; ind++) {

				if (bseq[ind].attribute) {

					if (xat&AT_FTHUP)
						fthfn(ind);
					else
						duoseq(ind);
				}
				else {

					if (xat&AT_FTHUP)
						duoseq(ind);
					else
						fthfn(ind);
				}
			}
			ind--;
		}
		opnt = ind;
	}
	rstMap(FTHR);
	rstMap(BARSAT);
	StitchSpace = bakspac;
	seqpnt = opnt;
}

void fritfil() {

	unsigned ind, ine;

	if (seqpnt) {

		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_FIL;
		isinds[isind2].cod = TYPFRM;
		isinds[isind2].col = ptrSelectedForm->fillColor;
		chkseq(0);
		isind2++;
		if (ptrSelectedForm->extendedAttribute&AT_FTHBLND&&~(ptrSelectedForm->extendedAttribute&(AT_FTHUP | AT_FTHDWN)) != (AT_FTHUP | AT_FTHDWN)) {

			isinds[isind2].ind = isind;
			isinds[isind2].seq = I_FTH;
			isinds[isind2].cod = FTHMSK;
			isinds[isind2].col = ptrSelectedForm->fillInfo.feather.color;
			ine = xpnt - 1;
			for (ind = 0; ind < xpnt; ind++) {

				oseq[ind].x = fthseq[ine].x;
				oseq[ind].y = fthseq[ine].y;
				ine--;
			}
			seqpnt = xpnt;
			chkseq(0);
			isind2++;
		}
	}
}

void fethrf() {

	if (formIndex) {

		fvars(closestFormToCursor);
		delclps(closestFormToCursor);
		deltx();
		ptrSelectedForm->type = SAT;
		ptrSelectedForm->fillInfo.feather.ratio = iniFile.featherRatio;
		ptrSelectedForm->fillInfo.feather.upCount = iniFile.featherUpCount;
		ptrSelectedForm->fillInfo.feather.downCount = iniFile.featherDownCount;
		ptrSelectedForm->fillInfo.feather.fillType = iniFile.featherFillType;
		ptrSelectedForm->fillInfo.feather.minStitchSize = iniFile.featherMinStitchSize;
		ptrSelectedForm->extendedAttribute &= !(AT_FTHUP | AT_FTHDWN | AT_FTHBLND);
		ptrSelectedForm->extendedAttribute |= iniFile.featherType;
		ptrSelectedForm->fillInfo.feather.count = iniFile.featherCount;
		ptrSelectedForm->lengthOrCount.stitchLength = userStitchLength;
		ptrSelectedForm->fillSpacing = StitchSpace;
		ptrSelectedForm->fillColor = activeColor;
		ptrSelectedForm->fillInfo.feather.color = (activeColor + 1)&COLMSK;
		ptrSelectedForm->fillType = FTHF;
		refilfn();
	}
}

void fethr() {

	unsigned ind;

	if (filmsgs(FMM_FTH))
		return;
	if (selectedFormCount) {

		savdo();
		for (ind = 0; ind < selectedFormCount; ind++) {

			closestFormToCursor = selectedFormList[ind];
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
	psgacc = sed;
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

void rstnam(char* dst, char* src) {

	unsigned ind;

	ind = 0;
	while (src[ind]) {

		dst[ind] = src[ind] + 17;
		ind++;
	}
	dst[ind] = 0;
}

void keynams(char* nam, char* sig, char*tim) {

	rstnam(nam, knam);
	rstnam(sig, ksig);
	rstnam(tim, ktim);
}

int fil2crd(const char* p_filnam)
{
	STARTUPINFO			sinfo;
	PROCESS_INFORMATION	pinfo;
	int					erc = 0;
	char				cmd[_MAX_PATH * 2 + 1 + 4];

	strcpy_s(cmd, "\"");
	strcat_s(cmd, iniFile.p2cName);
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
		hWnd,				//hwndOwner 
		hInst,				//hInstance 
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

	if (header.stitchCount)
		save();
	else {

		tabmsg(IDS_P2CNODAT);
		return;
	}
	if (chkp2cnam(iniFile.p2cName)) {

		fil2crd(thrName);
		return;
	}
	*iniFile.p2cName = 0;
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

		LoadString(hInst, IDS_P2CMSG, mbuf, P2CBUFSIZ);
		LoadString(hInst, IDS_P2CTITL, tbuf, P2CBUFSIZ);
		if (IDOK == MessageBox(hWnd, mbuf, tbuf, MB_OKCANCEL)) {

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
	strcpy_s(iniFile.p2cName, prgnam);
	fil2crd(auxName);
}

void sidlen(unsigned strt, unsigned fin, double* ilen, double* olen)
{
	*ilen += hypot(ipnts[fin].x - ipnts[strt].x, ipnts[fin].x - ipnts[strt].x);
	*olen += hypot(opnts[fin].x - opnts[strt].x, opnts[fin].x - opnts[strt].x);
}

fPOINT* insid()
{
	unsigned ind;

	satout(fabs(ptrSelectedForm->underlayIndent));
	if (ptrSelectedForm->underlayIndent > 0)
	{
		for (ind = 0; ind < sides; ind++)
		{
			if (!cisin(ipnts[ind].x, ipnts[ind].y))
			{
				ipnts[ind].x = currentFormVertices[ind].x;
				ipnts[ind].y = currentFormVertices[ind].y;
			}
		}
		return ipnts;
	}
	else
		return opnts;
}

void delwlk(unsigned cod)
{
	unsigned	ind, ine;
	fPOINTATTRIBUTE*	histch;
	BOOL		flg;

	ine = 0;
	flg = 1;
	histch = &stitchBuffer[MAXSEQ];
	delpnt = header.stitchCount;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		if ((stitchBuffer[ind].attribute&WLKFMSK) != cod)
		{
			moveStitchPoints(&histch[ine], &stitchBuffer[ind]);
			ine++;
		}
		else
		{
			if (flg)
			{
				flg = 0;
				delpnt = ind;
			}
		}
	}
	mvstchs(0, MAXSEQ, ine);
	header.stitchCount = ine;
	if (ine)
	{
		if (delpnt > (unsigned)header.stitchCount)
			delpnt = header.stitchCount;
	}
}

void chkuseq()
{
#if BUGBAK
	unsigned index;

	for (index = 0; index < opnt; index++)
	{
		iseq[index].x = oseq[index].x;
		iseq[index].y = oseq[index].y;
	}
	isind = index;
	isinds[isind2].col = ptrSelectedForm->underlayColor;
#else

	unsigned	ind, ine, cnt;
	double		len;
	fPOINT		dif;
	fPOINT		loc;
	fPOINT		stp;
	double		ulen;

	if (opnt)
	{
		if (ptrSelectedForm->underlayStitchLen < MINWLK)
			ptrSelectedForm->underlayStitchLen = (float)MINWLK;
		if (ptrSelectedForm->underlayStitchLen > MAXWLK)
			ptrSelectedForm->underlayStitchLen = MAXWLK;
		ulen = ptrSelectedForm->underlayStitchLen;
		for (ind = 0; ind < opnt - 1; ind++)
		{
			dif.x = oseq[ind + 1].x - oseq[ind].x;
			dif.y = oseq[ind + 1].y - oseq[ind].y;
			len = hypot(dif.x, dif.y);
			cnt = floor(len / ulen);
			if (cnt)
			{
				stp.x = dif.x / cnt;
				stp.y = dif.y / cnt;
				loc.x = oseq[ind].x;
				loc.y = oseq[ind].y;
				for (ine = 0; ine < cnt; ine++)
				{
					iseq[isind].x = loc.x;
					iseq[isind].y = loc.y;
					isind++;
					loc.x += stp.x;
					loc.y += stp.y;
				}
			}
			else
			{
				iseq[isind].x = oseq[ind].x;
				iseq[isind].y = oseq[ind].y;
				isind++;
			}
		}
		iseq[isind].x = oseq[ind].x;
		iseq[isind].y = oseq[ind].y;
		isind++;
		isinds[isind2].col = ptrSelectedForm->underlayColor;
	}
#endif
}

void ritwlk()
{
	if (opnt)
	{
		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_FIL;
		isinds[isind2].cod = WLKMSK;
		chkuseq();
		isind2++;
	}
}

void ritcwlk()
{
	if (opnt) {

		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_FIL;
		isinds[isind2].cod = CWLKMSK;
		chkuseq();
		isind2++;
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
		stitchBuffer[ind].x = ipnt[strt].x;
		stitchBuffer[ind].y = ipnt[strt].y;
		stitchBuffer[ind].attribute = cod;
		if (ind)
		{
			if (stitchBuffer[ind - 1].x != stitchBuffer[ind].x || stitchBuffer[ind - 1].y != stitchBuffer[ind].y)
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
		cnt = len / ptrSelectedForm->lengthOrCount.stitchLength;
		if (cnt > 1)
		{
			stp.x = dif.x / cnt;
			stp.y = dif.y / cnt;
			loc.x = ipnt[strt].x + stp.x;
			loc.y = ipnt[strt].y + stp.y;
			for (ine = 0; ine < cnt - 1; ine++)
			{
				stitchBuffer[ind].x = loc.x;
				stitchBuffer[ind].y = loc.y;
				stitchBuffer[ind].attribute = cod;
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
	stitchBuffer[ind].x = ipnt[strt].x;
	stitchBuffer[ind].y = ipnt[strt].y;
	stitchBuffer[ind].attribute = cod;
	ind++;
	return ind - dst;
}

void fnwlk(unsigned find)
{
	unsigned	strt, cnt;
	fPOINT*		wpnt;

	fvars(find);
	if (ptrSelectedForm->type == LIN)
		ptrSelectedForm->type = POLI;
	if (ptrSelectedForm->extendedAttribute&AT_STRT&&ptrSelectedForm->type != LIN)
		strt = ptrSelectedForm->fillStart;
	else
		strt = 0;
	cnt = sides;
	if (ptrSelectedForm->type != LIN)
		cnt++;
	wpnt = insid();
	opnt = 0;
	while (cnt)
	{
		oseq[opnt].x = wpnt[strt].x;
		oseq[opnt].y = wpnt[strt].y;
		strt = nxt(strt);
		opnt++;
		cnt--;
	}
	lastfltx = prv(strt);
	ritwlk();
}

void fnuang() {

	unsigned	ind;

	frmcpy(&angfrm, &formList[closestFormToCursor]);
	rotationCenter.x = (double)(angfrm.rectangle.right - angfrm.rectangle.left) / 2 + angfrm.rectangle.left;
	rotationCenter.y = (double)(angfrm.rectangle.top - angfrm.rectangle.bottom) / 2 + angfrm.rectangle.bottom;
	angfrm.vertices = angflt;
	for (ind = 0; ind < angfrm.sides; ind++) {

		angfrm.vertices[ind].x = uflt[ind].x;
		angfrm.vertices[ind].y = uflt[ind].y;
		rotflt(&angfrm.vertices[ind]);
	}
	ptrSelectedForm = &angfrm;
	fnvrt();
	fvars(closestFormToCursor);
}

void ritund()
{
	if (seqpnt) {

		isinds[isind2].ind = isind;
		isinds[isind2].seq = I_FIL;
		isinds[isind2].cod = UNDMSK;
		chkuseq();
		isind2++;
	}
}

void undclp()
{
	clipboardRectSize.cx = clipboardRect.bottom = clipboardRect.left = clipboardRect.right = clipBuffer[0].x = clipBuffer[1].x = clipBuffer[0].y = 0;
	clipboardRectSize.cy = clipboardRect.top = clipBuffer[1].y = ptrSelectedForm->underlayStitchLen;
	clipboardStitchCount = 2;
}

void fnund(unsigned find)
{
	float baksiz;
	uflt = insid();
	baksiz = userStitchLength;
	userStitchLength = 1e99;
	if (!ptrSelectedForm->underlaySpacing)
		ptrSelectedForm->underlaySpacing = iniFile.underlaySpacing;
	if (!ptrSelectedForm->underlayStitchLen)
		ptrSelectedForm->underlayStitchLen = iniFile.underlayStitchLen;
	undclp();
	setMap(ISUND);
	angclpfn();
	opnt = seqpnt;
	ritund();
	fvars(find);
	userStitchLength = baksiz;
}

void fncwlk()
{
	unsigned	ind, ine, strt, fin;
	SATCON*		sac;

	opnt = 0;
	ptrSelectedForm->extendedAttribute |= AT_CWLK;
	if (ptrSelectedForm->satinGuideCount)
	{
		if (ptrSelectedForm->wpar)
		{
			ine = ptrSelectedForm->wpar;
			oseq[opnt].x = midl(currentFormVertices[ine].x, currentFormVertices[ine + 1].x);
			oseq[opnt].y = midl(currentFormVertices[ine].y, currentFormVertices[ine + 1].y);
			opnt++;
		}
		sac = ptrSelectedForm->satinOrAngle.sac;
		for (ind = ptrSelectedForm->satinGuideCount; ind != 0; ind--)
		{
			oseq[opnt].x = midl(currentFormVertices[sac[ind - 1].finish].x, currentFormVertices[sac[ind - 1].start].x);
			oseq[opnt].y = midl(currentFormVertices[sac[ind - 1].finish].y, currentFormVertices[sac[ind - 1].start].y);
			opnt++;
		}
		if (ptrSelectedForm->attribute&FRMEND)
		{
			oseq[opnt].x = midl(currentFormVertices[0].x, currentFormVertices[1].x);
			oseq[opnt].y = midl(currentFormVertices[0].y, currentFormVertices[1].y);
			opnt++;
		}
	}
	else
	{
		if (ptrSelectedForm->extendedAttribute&AT_STRT)
			strt = ptrSelectedForm->fillStart;
		else
			strt = 0;
		oseq[opnt].x = currentFormVertices[strt].x;
		oseq[opnt].y = currentFormVertices[strt].y;
		opnt++;
		fin = prv(strt);
		strt = nxt(strt);
		for (ind = 1; ind < (unsigned)sides >> 1; ind++)
		{
			oseq[opnt].x = midl(currentFormVertices[fin].x, currentFormVertices[strt].x);
			oseq[opnt].y = midl(currentFormVertices[fin].y, currentFormVertices[strt].y);
			if (cisin(oseq[opnt].x, oseq[opnt].y))
				opnt++;
			strt = nxt(strt);
			fin = prv(fin);
		}
		oseq[opnt].x = currentFormVertices[strt].x;
		oseq[opnt].y = currentFormVertices[strt].y;
		opnt++;
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

		l_xat = iniFile.featherType;
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		sprintf_s(buf, sizeof(buf), "%.2f", iniFile.featherRatio);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.featherUpCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.featherDownCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", iniFile.featherMinStitchSize / PFGRAN);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.featherCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf);
		for (ind = 0; ind < 6; ind++) {

			LoadString(hInst, IDS_FTH0 + ind, buf, HBUFSIZ);
			SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_ADDSTRING, 0, (long)buf);
		}
		SendMessage(GetDlgItem(hwndlg, IDC_FDTYP), CB_SETCURSEL, iniFile.featherFillType - 1, 0);
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

			iniFile.featherType = 0;
			if (IsDlgButtonChecked(hwndlg, IDC_FDBLND))
				iniFile.featherType = AT_FTHBLND;
			if (IsDlgButtonChecked(hwndlg, IDC_FDUP))
				iniFile.featherType |= AT_FTHUP;
			if (IsDlgButtonChecked(hwndlg, IDC_FDWN))
				iniFile.featherType |= AT_FTHDWN;
			GetWindowText(GetDlgItem(hwndlg, IDC_FDTYP), buf, HBUFSIZ);
			iniFile.featherFillType = FDEFTYP;
			for (ind = 0; ind < 6; ind++) {

				LoadString(hInst, IDS_FTH0 + ind, buf1, HBUFSIZ);
				if (!strcmp(buf, buf1)) {

					iniFile.featherFillType = ind + 1;
					break;
				}
			}
			GetWindowText(GetDlgItem(hwndlg, IDC_DFRAT), buf, HBUFSIZ);
			iniFile.featherRatio = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFUPCNT), buf, HBUFSIZ);
			iniFile.featherUpCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFDWNCNT), buf, HBUFSIZ);
			iniFile.featherDownCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DFLR), buf, HBUFSIZ);
			iniFile.featherMinStitchSize = atof(buf)*PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_DFNUM), buf, HBUFSIZ);
			iniFile.featherCount = atoi(buf);
			if (iniFile.featherCount < 1)
				iniFile.featherCount = 1;
			EndDialog(hwndlg, 1);
			break;
		}
	}
	return 0;
}

void dufdef() {

	DialogBox(hInst, MAKEINTRESOURCE(IDD_FETHDEF), hWnd, (DLGPROC)fthdefprc);
}

void srtcol() {

	unsigned		hst[16];
	unsigned		ind, ine, tmp;
	fPOINTATTRIBUTE*		phi;

	FillMemory(&hst, 64, 0);
	for (ind = 0; ind < header.stitchCount; ind++)
		hst[stitchBuffer[ind].attribute&COLMSK]++;
	ine = 0;
	for (ind = 0; ind < 16; ind++) {

		tmp = hst[ind];
		hst[ind] = ine;
		ine += tmp;
	}
	phi = &stitchBuffer[MAXSEQ];
	for (ind = 0; ind < header.stitchCount; ind++)
		moveStitchPoints(&phi[hst[stitchBuffer[ind].attribute&COLMSK]++], &stitchBuffer[ind]);
	MoveMemory(&stitchBuffer, phi, sizeof(fPOINTATTRIBUTE)*header.stitchCount);
}

void dubit(unsigned bit)
{
	unsigned cod;

	savdo();
	setMap(WASDO);
	if (ptrSelectedForm->type == LIN)
		ptrSelectedForm->type = POLI;
	if (!(ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK)) && bit&(AT_UND | AT_WALK | AT_CWLK))
	{
		if (ptrSelectedForm->fillType)
			ptrSelectedForm->underlayColor = ptrSelectedForm->fillColor;
		else
			ptrSelectedForm->underlayColor = activeColor;
		ptrSelectedForm->underlayStitchLen = iniFile.underlayStitchLen;
	}
	if (!(ptrSelectedForm->extendedAttribute&AT_UND) && bit&AT_UND)
	{
		ptrSelectedForm->underlayStitchAngle = iniFile.underlayAngle;
		ptrSelectedForm->underlaySpacing = iniFile.underlaySpacing;
	}
	cod = ptrSelectedForm->extendedAttribute&bit;
	if (cod)
		ptrSelectedForm->extendedAttribute &= (~bit);
	else
		ptrSelectedForm->extendedAttribute |= bit;
	refil();
	coltab();
	setMap(RESTCH);
}

void setuang()
{
	setMap(GTUANG);
	msgflt(IDS_UANG, iniFile.underlayAngle / PI * 180);
}

void setuspac()
{
	setMap(GTUSPAC);
	msgflt(IDS_USPAC, iniFile.underlaySpacing / PFGRAN);
}

void setwlkind()
{
	setMap(GTWLKIND);
	msgflt(IDS_WLKIND, iniFile.underlayIndent / PFGRAN);
}

void setulen()
{
	setMap(GTWLKLEN);
	msgflt(IDS_WLKLEN, iniFile.underlayStitchLen / PFGRAN);
}

void chkcwlk()
{
	if (ptrSelectedForm->extendedAttribute&AT_CWLK)
		fncwlk();
	else
		delwlk((closestFormToCursor << FRMSHFT) | CWLKMSK);
}

void chkwlk()
{
	if (ptrSelectedForm->extendedAttribute&AT_WALK)
		fnwlk(closestFormToCursor);
	else
		delwlk((closestFormToCursor << FRMSHFT) | WLKMSK);
}

void chkund()
{
	if (ptrSelectedForm->extendedAttribute&AT_UND)
		fnund(closestFormToCursor);
	else
		delwlk((closestFormToCursor << FRMSHFT) | UNDMSK);
}

void selalfrm()
{
	for (selectedFormCount = 0; selectedFormCount < formIndex; selectedFormCount++)
		selectedFormList[selectedFormCount] = selectedFormCount;
	setMap(RESTCH);
}

void chkdaz()
{
	float	len;

	if (!iniFile.daisyPetalPoints)
		iniFile.daisyPetalPoints = 1;
	if (!iniFile.daisyInnerCount)
		iniFile.daisyInnerCount = 1;
	if (!iniFile.daisyPetalCount)
		iniFile.daisyPetalCount = 1;
	len = iniFile.daisyDiameter*0.999;
	if (iniFile.daisyHeartCount > iniFile.daisyPetalPoints)
		iniFile.daisyHeartCount = iniFile.daisyPetalPoints;
}

void dazdef()
{
	iniFile.daisyPetalPoints = DAZCNT;
	iniFile.daisyHoleDiameter = DAZHLEN;
	iniFile.daisyInnerCount = DAZICNT;
	iniFile.daisyDiameter = DAZLEN;
	iniFile.daisyPetalCount = DAZPETS;
	iniFile.daisyPetalLen = DAZPLEN;
	iniFile.daisyHeartCount = DAZMCNT;
	setu(DAZHOL);
	setu(DAZD);
	iniFile.daisyBorderType = DAZTYP;
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
		sprintf_s(buf, sizeof(buf), "%d", iniFile.daisyPetalPoints);
		SetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.daisyHeartCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", iniFile.daisyDiameter);
		SetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", iniFile.daisyHoleDiameter);
		SetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.daisyInnerCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buf);
		sprintf_s(buf, sizeof(buf), "%d", iniFile.daisyPetalCount);
		SetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buf);
		sprintf_s(buf, sizeof(buf), "%.2f", iniFile.daisyPetalLen);
		SetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buf);
		CheckDlgButton(hwndlg, IDC_HOLE, chku(DAZHOL));
		CheckDlgButton(hwndlg, IDC_DLIN, chku(DAZD));
		for (ind = 0; ind < 6; ind++)
		{
			LoadString(hInst, daztab[ind], buf, HBUFSIZ);
			SendMessage(GetDlgItem(hwndlg, IDC_DAZTYP), CB_ADDSTRING, 0, (long)buf);
		}
		SendMessage(GetDlgItem(hwndlg, IDC_DAZTYP), CB_SETCURSEL, iniFile.daisyBorderType, 0);
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam)) {

		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			GetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buf, HBUFSIZ);
			iniFile.daisyPetalPoints = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buf, HBUFSIZ);
			iniFile.daisyHeartCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buf, HBUFSIZ);
			iniFile.daisyDiameter = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buf, HBUFSIZ);
			iniFile.daisyHoleDiameter = atof(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buf, HBUFSIZ);
			iniFile.daisyInnerCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buf, HBUFSIZ);
			iniFile.daisyPetalCount = atoi(buf);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buf, HBUFSIZ);
			iniFile.daisyPetalLen = atof(buf);
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
				LoadString(hInst, daztab[ind], buf1, HBUFSIZ);
				if (!strcmp(buf, buf1))
				{
					iniFile.daisyBorderType = ind;
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
	if (!DialogBox(hInst, MAKEINTRESOURCE(IDD_DASY), hWnd, (DLGPROC)dasyproc))
	{
		rstMap(FORMIN);
		return;
	}
	iniFile.daisyPetalCount = iniFile.daisyPetalCount;
	len = iniFile.daisyDiameter;
	elen = iniFile.daisyPetalLen;
	ilen = iniFile.daisyHoleDiameter;
	dtyp = iniFile.daisyBorderType;
	ref.x = midl(zoomRect.right, zoomRect.left);
	ref.y = midl(zoomRect.top, zoomRect.bottom);
	ptrSelectedForm = &formList[formIndex];
	closestFormToCursor = formIndex;
	frmclr(ptrSelectedForm);
	ptrSelectedForm->vertices = &formPoints[fltad];
	ptrSelectedForm->attribute = activeLayer << 1;
	fvars(formIndex);
	cnt2 = iniFile.daisyPetalPoints >> 1;
	maxcor = zoomRect.right - zoomRect.left;
	drat = zoomRect.top - zoomRect.bottom;
	if (drat > maxcor)
		maxcor = drat;
	maxcor /= 6;
	drat = (double)maxcor / (len + elen);
	len *= drat;
	elen *= drat;
	ilen *= drat;
	ptrSelectedForm->type = POLI;
	inf = fref = 0;
	if (chku(DAZHOL))
	{
		pang = PI2;
		bcnt = iniFile.daisyPetalCount*iniFile.daisyInnerCount;
		dang = PI2 / bcnt;
		currentFormVertices[inf].x = ref.x + len*cos(pang);
		currentFormVertices[inf].y = ref.y + len*sin(pang);
		inf++;
		for (ind = 0; ind < bcnt + 1; ind++)
		{
			currentFormVertices[inf].x = ref.x + ilen*cos(pang);
			currentFormVertices[inf].y = ref.y + ilen*sin(pang);
			inf++;
			pang -= dang;
		}
		fref = inf;
	}
	pang = 0;
	bcnt = iniFile.daisyPetalCount*iniFile.daisyPetalPoints;
	pcnt = iniFile.daisyPetalPoints;
	if (dtyp == DHART)
	{
		pcnt = (iniFile.daisyHeartCount + 1) << 1;
		bcnt = iniFile.daisyPetalCount*pcnt;
	}
	dang = PI2 / bcnt;
	dangp = PI / iniFile.daisyPetalPoints;
	if (chku(DAZD))
	{
		ptrSelectedForm->satinGuideCount = iniFile.daisyPetalCount - 1;
		ptrSelectedForm->wpar = iniFile.daisyPetalCount*iniFile.daisyInnerCount + 1;
		ptrSelectedForm->satinOrAngle.sac = adsatk(iniFile.daisyPetalCount - 1);
	}
	for (ind = 0; ind < iniFile.daisyPetalCount; ind++)
	{
		pangp = 0;
		psgacc = SEED;
		for (ine = 0; ine < pcnt; ine++)
		{
			switch (dtyp)
			{
			case DSIN:

				dlen = len + sin(pangp)*elen;
				pangp += dangp;
				break;

			case DRAMP:

				dlen = len + (double)ine / iniFile.daisyPetalPoints*elen;
				break;

			case DSAW:

				if (ine > cnt2)
					dcnt = iniFile.daisyPetalPoints - ine;
				else
					dcnt = ine;
				dlen = len + (double)dcnt / iniFile.daisyPetalPoints*elen;
				break;

			case DRAG:

				dlen = len + (double)(psg() % iniFile.daisyPetalPoints) / iniFile.daisyPetalPoints*elen;
				break;

			case DCOG:

				dlen = len;
				if (ine > cnt2)
					dlen += elen;
				break;

			case DHART:

				dlen = len + sin(pangp)*elen;
				if (ine > iniFile.daisyHeartCount)
					pangp -= dangp;
				else
					pangp += dangp;
				break;
			}
			currentFormVertices[inf].x = ref.x + cos(pang)*dlen;
			currentFormVertices[inf].y = ref.y + sin(pang)*dlen;
			inf++;
			pang += dang;
			if (chku(DAZD) && ind != iniFile.daisyPetalCount - 1)
			{
				ptrSelectedForm->satinOrAngle.sac[ind].start = (iniFile.daisyPetalCount - ind - 1)*iniFile.daisyInnerCount + 1;
				ptrSelectedForm->satinOrAngle.sac[ind].finish = inf;
			}
		}
	}
	if (chku(DAZHOL))
	{
		currentFormVertices[fref - 1].y += (float)0.01;
		currentFormVertices[fref].y += (float)0.01;
	}
	ptrSelectedForm->sides = inf;
	if (chku(DAZD))
	{
		ptrSelectedForm->type = SAT;
		ptrSelectedForm->attribute = 1;
	}
	fltad += inf;
	setMap(INIT);
	frmout(formIndex);
	for (ind = 0; ind < inf; ind++) {

		currentFormVertices[ind].x -= ptrSelectedForm->rectangle.left;
		currentFormVertices[ind].y -= ptrSelectedForm->rectangle.bottom;
	}
	fmovdif.x = fmovdif.y = 0;
	newFormVertexCount = inf + 1;
	setMap(POLIMOV);
	setmfrm();
	mdufrm();
}

void durec(OREC* prec)
{
	unsigned	at;
	fPOINTATTRIBUTE*	pstch;

	pstch = &stitchBuffer[prec->start];
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
	if (colord[pa1->col] == colord[pa2->col])
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
	return (int)colord[pa1->col] - colord[pa2->col];
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

double srtlen(fPOINTATTRIBUTE* pnt, OREC* prec, unsigned swtch)
{
	fPOINTATTRIBUTE* tpnt;

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
		if (precs[ind]->otyp == frmcnts[precs[ind]->frm])
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
	fPOINTATTRIBUTE*		ploc;
	BOOL			locdir;

	frmcnts = (unsigned*)&oseq;
	FillMemory(&oseq, (xpnt + 2) << 2, 0);
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
		ploc = &stitchBuffer[loci];
		for (ind = psrec->start; ind < psrec->finish; ind++)
		{
			if (precs[ind]->otyp == frmcnts[precs[ind]->frm])
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
		frmcnts[precs[loc]->frm]++;
		if (chkMap(DUSRT))
		{
			if (locdir)
			{
				if (precs[loc]->start)
				{
					for (ind = precs[loc]->finish - 1; ind >= precs[loc]->start; ind--)
						moveStitchPoints(&hifstch[opnt++], &stitchBuffer[ind]);
				}
				else
				{
					ind = precs[loc]->finish;
					while (ind)
						moveStitchPoints(&hifstch[opnt++], &stitchBuffer[--ind]);
				}
			}
			else
			{
				for (ind = precs[loc]->start; ind < precs[loc]->finish; ind++)
					moveStitchPoints(&hifstch[opnt++], &stitchBuffer[ind]);
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
		sprintf_s(msgbuf, sizeof(msgbuf), "%4d off: %4d at: %08x frm: %4d typ: %d col: %2d st: %5d fin: %5d\n",
			ind,
			recs[ind] - (OREC*)bseq,
			stitchBuffer[recs[ind]->start].attribute,
			recs[ind]->frm,
			recs[ind]->typ,
			recs[ind]->col,
			recs[ind]->start,
			recs[ind]->finish);
		OutputDebugString(msgbuf);
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
			if (colord[pfrec[ind]->col] < colord[hcol])
			{
				fp = &formList[frm];
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
	recs = (OREC*)&bseq;
	recs->start = 0;
	recs->spnt = stitchBuffer;
	at = stitchBuffer->attribute&SRTMSK;
	rind = 0;
	xpnt = formIndex;
	colord[underlayColor] = 0;
	for (ind = 0; ind < 16; ind++) {

		if (ind != underlayColor)
			colord[ind] = ind + 1;
	}
	for (ind = 1; ind < header.stitchCount; ind++)
	{
		if ((stitchBuffer[ind].attribute&SRTMSK) != at)
		{
			recs[rind].finish = ind;
			recs[rind].epnt = &stitchBuffer[ind - 1];
			rind++;
			recs[rind].start = ind;
			recs[rind].spnt = &stitchBuffer[ind];
			at = stitchBuffer[ind].attribute&SRTMSK;
		}
	}
	recs[rind].epnt = &stitchBuffer[header.stitchCount - 1];
	recs[rind].finish = header.stitchCount;
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
		l_frmcnts = (unsigned*)&oseq;
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
		hifstch = &stitchBuffer[MAXSEQ];
		opnt = 0;
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
		mvstchs(0, MAXSEQ, opnt);
		header.stitchCount = opnt;
		coltab();
		setMap(RESTCH);
	}
	else
	{
		LoadString(hInst, IDS_SRTER, hlpbuf, HBUFSIZ);
		sprintf_s(msgbuf, sizeof(msgbuf), hlpbuf, pfrecs[ine]->frm);
		shoMsg(msgbuf);
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

	fcod = closestFormToCursor << FRMSHFT;
	*lmap = 0;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		at = stitchBuffer[ind].attribute;
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

				if (ptrSelectedForm->fillColor == ptrSelectedForm->borderColor)
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

	at = stitchBuffer[ind].attribute;
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
	at = stitchBuffer[0].attribute;
	for (ind = 1; ind < header.stitchCount; ind++)
	{
		if (at != stitchBuffer[ind].attribute)
		{
			duatf(ind);
			at = stitchBuffer[ind].attribute;
		}
	}
}
#endif

void fdelstch()
{
	unsigned	ind, ine, fcod, at, typ, tmap, col, bcol, tapcol;
	unsigned*	stpnt;

	fvars(closestFormToCursor);
	tmap = 0;
	FillMemory(&fstrts, sizeof(FSTRTS), 0);
	fcod = (closestFormToCursor << FRMSHFT);
	ine = 0;
	bcol = ptrSelectedForm->borderColor&COLMSK;
	tapcol = ptrSelectedForm->borderColor >> 4;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		if (!chku(FIL2OF) && chkMap(SELBOX) && ind == closestPointIndex)
			closestPointIndex = ine;
		at = stitchBuffer[ind].attribute;
		if (fcod == (at&(FRMSK | NOTFRM)))
		{
			typ = ftyps[dutyp(at)];
			switch (typ)
			{
			case 1:

				if (!(tmap&M_AP))
				{
					tmap |= M_AP;
					fstrts.apl = ine;
				}
				break;

			case 5:

				if (!(tmap&M_FTH))
				{
					tmap |= M_FTH;
					fstrts.fth = ine;
				}
				break;

			case 6:

				if (!(tmap&M_FIL))
				{
					tmap |= M_FIL;
					fstrts.fil = ine;
				}
				break;

			case 7:

				if (!(tmap&M_BRD))
				{
					tmap |= M_BRD;
					fstrts.brd = ine;
				}
				break;

			default:

				if (ptrSelectedForm->fillType && !(tmap&M_FIL))
				{
					tmap |= M_FIL;
					fstrts.fil = ine;
				}
				break;
			}
		}
		else
		{
			col = at&COLMSK;
			if (col == ptrSelectedForm->fillColor)
			{
				tmap |= M_FCOL;
				fstrts.fcol = ine;
			}
			if (col == ptrSelectedForm->fillInfo.feather.color)
			{
				tmap |= M_FTHCOL;
				fstrts.fthcol = ine;
			}
			if (col == bcol)
			{
				tmap |= M_ECOL;
				fstrts.ecol = ine;
			}
			if (col == tapcol)
			{
				tmap |= M_APCOL;
				fstrts.apcol = ine;
			}
			stitchBuffer[ine].x = stitchBuffer[ind].x;
			stitchBuffer[ine].y = stitchBuffer[ind].y;
			stitchBuffer[ine].attribute = stitchBuffer[ind].attribute;
			ine++;
		}
	}
	fstrts.fcol++;
	fstrts.fthcol++;
	fstrts.ecol++;
	fstrts.apcol++;
	smap = tmap;
	header.stitchCount = ine;
	ine = 0;
	if (!(tmap&M_ECOL))
		fstrts.ecol = header.stitchCount;
	if (!(tmap&M_FTHCOL))
		fstrts.fthcol = header.stitchCount;
	if (!(tmap&M_FCOL))
		fstrts.fcol = header.stitchCount;
	if (ptrSelectedForm->edgeType)
	{
		if (ptrSelectedForm->edgeType == EGAP)
		{
			if (!(tmap&M_AP))
			{
				if (tmap&M_APCOL)
					fstrts.apl = fstrts.apcol + 1;
				else
					fstrts.apl = header.stitchCount;
			}
		}
		if (!(tmap&M_BRD))
		{
			if (tmap&M_ECOL)
				fstrts.brd = fstrts.ecol + 1;
			else
				fstrts.brd = header.stitchCount;
		}
	}
	if (ptrSelectedForm->fillType || (tmap&(M_WALK | M_UND | M_CWLK)))
	{
		if (!(tmap&M_FIL))
		{
			if (tmap&M_FCOL)
				fstrts.fil = fstrts.fcol + 1;
			else
				fstrts.fil = header.stitchCount;
		}
	}
	if (ptrSelectedForm->fillType == FTHF)
	{
		if (!(tmap&M_FTH))
		{
			if (tmap&M_FTHCOL)
				fstrts.fth = fstrts.fthcol + 1;
			else
				fstrts.fth = header.stitchCount;
		}
	}
	stpnt = (unsigned*)&fstrts;
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
		for (ind = 0; ind < sizeof(fstrts) >> 2; ind++)
			stpnt[ind] = closestPointIndex;
	}
}

BOOL lastcol(unsigned ind, fPOINT* pnt)
{
	unsigned col;

	col = isinds[ind].col;
	while (ind)
	{
		ind--;
		if (isinds[ind].col == col)
		{
			pnt->x = iseq[isinds[ind + 1].ind - 1].x;
			pnt->y = iseq[isinds[ind + 1].ind - 1].y;
			return 1;
		}
	}
	return 0;
}

void duint(unsigned off, unsigned cod)
{
	unsigned	cnt, ind;
	fPOINT		cpnt;

	if (itf.coloc > itf.sloc)
	{
		cnt = itf.coloc - itf.sloc;
		MoveMemory(&itf.histch[itf.oloc], &stitchBuffer[itf.sloc], sizeof(fPOINTATTRIBUTE)*cnt);
		itf.sloc += cnt;
		itf.oloc += cnt;
	}
	if (ptrSelectedForm->extendedAttribute&AT_STRT)
	{
		if (!setMap(DIDSTRT))
			itf.oloc += gucon(currentFormVertices[ptrSelectedForm->fillStart], iseq[isinds[itf.pins].ind], itf.oloc + off, cod);
	}
	if (lastcol(itf.pins, &cpnt))
		itf.oloc += gucon(cpnt, iseq[isinds[itf.pins].ind], itf.oloc + MAXSEQ, cod);
	for (ind = isinds[itf.pins].ind; ind < isinds[itf.pins + 1].ind; ind++)
	{
		itf.histch[itf.oloc].x = iseq[ind].x;
		itf.histch[itf.oloc].y = iseq[ind].y;
		itf.histch[itf.oloc].attribute = cod;
		if (itf.histch[itf.oloc].x != itf.histch[itf.oloc - 1].x ||
			itf.histch[itf.oloc].y != itf.histch[itf.oloc - 1].y)
			itf.oloc++;
	}
}

BOOL isfil()
{
	if (ptrSelectedForm->fillType)
		return 1;
	if (ptrSelectedForm->edgeType)
		return 1;
	if (ptrSelectedForm->extendedAttribute&(AT_CWLK | AT_WALK | AT_UND))
		return 1;
	return 0;
}

void chkend(unsigned off, unsigned cod)
{
	if (isfil())
	{
		setMap(ISEND);
		if (ptrSelectedForm->extendedAttribute&AT_END)
			itf.oloc += gucon(iseq[isind - 1], currentFormVertices[ptrSelectedForm->fillEnd], itf.oloc + off, cod);
	}
}

void intlv()
{
	unsigned	ind, ine, cod = 0, off;
	fPOINT		cpnt;

	rstMap(ISEND);
	fvars(closestFormToCursor);
	isinds[isind2].ind = isind;
	FillMemory(&itf, sizeof(INTINF), 0);
	itf.layerIndex = ((ptrSelectedForm->attribute&FRMLMSK) << (LAYSHFT - 1)) | (closestFormToCursor << FRMSHFT);
	rstMap(DIDSTRT);
	if (header.stitchCount)
	{
		off = MAXSEQ;
		itf.histch = &stitchBuffer[MAXSEQ];
		for (ind = 0; ind < isind2; ind++)
		{
			itf.pins = ind;
			switch (isinds[ind].seq)
			{
			case I_AP:

				if (smap&M_FIL&&fstrts.apl >= itf.coloc)
					itf.coloc = fstrts.apl;
				else
				{
					itf.coloc = fstrts.apcol;
					if (itf.coloc == 1)
						itf.coloc = 0;
				}
				break;

			case I_FIL:

				if (smap&M_FIL&&fstrts.fil >= itf.coloc)
					itf.coloc = fstrts.fil;
				else
					itf.coloc = fstrts.fcol;
				break;

			case I_FTH:

				if (smap&M_FIL&&fstrts.fth >= itf.coloc)
					itf.coloc = fstrts.fth;
				else
					itf.coloc = fstrts.fthcol;
				break;

			case I_BRD:

				if (smap&M_BRD&&fstrts.brd >= itf.coloc)
					itf.coloc = fstrts.brd;
				else
					itf.coloc = fstrts.ecol;
				break;
			}
			cod = itf.layerIndex | isinds[itf.pins].cod | isinds[itf.pins].col;
			duint(off, cod);
		}
		chkend(MAXSEQ, cod);
		if (header.stitchCount&&itf.sloc < (unsigned)header.stitchCount - 1)
		{
			ine = header.stitchCount - itf.sloc;
			MoveMemory(&stitchBuffer[itf.oloc + MAXSEQ], &stitchBuffer[itf.sloc], sizeof(fPOINTATTRIBUTE)*ine);
			itf.oloc += ine;
		}
		MoveMemory(stitchBuffer, itf.histch, sizeof(fPOINTATTRIBUTE)*itf.oloc);
	}
	else
	{
		off = 0;
		for (ind = 0; ind < isind2; ind++)
		{
			cod = itf.layerIndex | isinds[ind].cod | isinds[ind].col;
			if (ptrSelectedForm->extendedAttribute&AT_STRT)
			{
				if (!setMap(DIDSTRT))
					itf.oloc += gucon(currentFormVertices[ptrSelectedForm->fillStart], iseq[isinds[itf.pins].ind], itf.oloc + off, cod);
			}
			if (lastcol(ind, &cpnt))
				itf.oloc += gucon(cpnt, iseq[isinds[ind].ind], itf.oloc, cod);
			for (ine = isinds[ind].ind; ine < isinds[ind + 1].ind; ine++)
			{
				stitchBuffer[itf.oloc].x = iseq[ine].x;
				stitchBuffer[itf.oloc].y = iseq[ine].y;
				stitchBuffer[itf.oloc].attribute = cod;
				if (stitchBuffer[itf.oloc].x != stitchBuffer[itf.oloc - 1].x ||
					stitchBuffer[itf.oloc].y != stitchBuffer[itf.oloc - 1].y)
					itf.oloc++;
			}
		}
		chkend(0, cod);
	}
	header.stitchCount = itf.oloc;
	coltab();
}

void setundfn(unsigned cod)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
		{
			closestFormToCursor = selectedFormList[ind];
			fvars(closestFormToCursor);
			if (ptrSelectedForm->type == LIN)
				continue;
			ptrSelectedForm->extendedAttribute |= cod;
			refilfn();
		}
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL))
	{
		fvars(closestFormToCursor);
		ptrSelectedForm->extendedAttribute |= cod;
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
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
		{
			closestFormToCursor = ind;
			fvars(closestFormToCursor);
			if (ptrSelectedForm->type == LIN)
				continue;
			ptrSelectedForm->extendedAttribute &= cod;
			refilfn();
		}
		coltab();
		setMap(RESTCH);
		return;
	}
	if (chkMap(FORMSEL))
	{
		fvars(closestFormToCursor);
		if (ptrSelectedForm->type == LIN)
			return;
		ptrSelectedForm->extendedAttribute &= cod;
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
	{
		ptrSelectedForm->underlayStitchLen = len;
		refilfn();
	}
}

void dusulen(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			ulenfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		ulenfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->extendedAttribute&AT_UND)
	{
		ptrSelectedForm->underlaySpacing = spac;
		refilfn();
	}
}

void duspac(float spac)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			uspacfn(selectedFormList[ind], spac);
	}
	if (chkMap(FORMSEL))
		uspacfn(closestFormToCursor, spac);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->extendedAttribute&AT_UND)
	{
		ptrSelectedForm->underlayStitchAngle = p_ang;
		refilfn();
	}
}

void dufang(float p_ang)
{
	unsigned ind;

	savdo();
	p_ang *= (float)PI / 180;
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			uangfn(selectedFormList[ind], p_ang);
	}
	if (chkMap(FORMSEL))
		uangfn(closestFormToCursor, p_ang);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->fillType && !isclp(find))
	{
		ptrSelectedForm->lengthOrCount.stitchLength = len;
		refilfn();
	}
}

void duflen(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			flenfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		flenfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->fillType)
	{
		if (spac < 0)
		{
			if (!isclp(find))
				return;
		}
		ptrSelectedForm->fillSpacing = spac;
		refilfn();
	}
}

void dufspac(float spac)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fspacfn(selectedFormList[ind], spac);
	}
	if (chkMap(FORMSEL))
		fspacfn(closestFormToCursor, spac);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	ptrSelectedForm->underlayIndent = idnt;
	if (ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK))
		refilfn();
}

void dufind(float idnt)
{
	unsigned ind;

	savdo();
	idnt *= PFGRAN;
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			findfn(selectedFormList[ind], idnt);
	}
	if (chkMap(FORMSEL))
		findfn(closestFormToCursor, idnt);
	coltab();
	setMap(RESTCH);
}

void fangfn(unsigned find, float p_ang)
{
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->type == POLI&&ptrSelectedForm->fillType)
	{
		switch (ptrSelectedForm->fillType)
		{
		case VRTF:
		case HORF:
		case ANGF:

			ptrSelectedForm->fillType = ANGF;
			ptrSelectedForm->angleOrClipData.angle = p_ang;
			break;

		case	VCLPF:
		case	HCLPF:
		case	ANGCLPF:

			ptrSelectedForm->fillType = ANGCLPF;
			ptrSelectedForm->satinOrAngle.angle = p_ang;
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
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fangfn(selectedFormList[ind], p_ang);
	}
	if (chkMap(FORMSEL))
		fangfn(closestFormToCursor, p_ang);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->extendedAttribute&(AT_UND | AT_WALK | AT_CWLK))
	{
		ptrSelectedForm->underlayColor = col;
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
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			ucolfn(selectedFormList[ind], col);
	}
	if (chkMap(FORMSEL))
		ucolfn(closestFormToCursor, col);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->fillType)
	{
		ptrSelectedForm->fillColor = col;
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
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fcolfn(selectedFormList[ind], col);
	}
	if (chkMap(FORMSEL))
		fcolfn(closestFormToCursor, col);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->edgeType)
	{
		ptrSelectedForm->borderColor = col;
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
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			bcolfn(selectedFormList[ind], col);
	}
	if (chkMap(FORMSEL))
		bcolfn(closestFormToCursor, col);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->edgeType && !iseclp(find))
	{
		ptrSelectedForm->lengthOrCount.stitchLength = len;
		refilfn();
	}
}

void dublen(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			blenfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		blenfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->edgeType)
	{
		ptrSelectedForm->edgeSpacing = len;
		refilfn();
	}
}

void dubspac(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			bspacfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		bspacfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->edgeType)
	{
		ptrSelectedForm->minBorderStitchLen = len;
		refilfn();
	}
}

void dubmin(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			bminfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		bminfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->edgeType)
	{
		ptrSelectedForm->maxBorderStitchLen = len;
		refilfn();
	}
}

void dubmax(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			bmaxfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		bmaxfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->fillType)
	{
		ptrSelectedForm->minFillStitchLen = len;
		refilfn();
	}
}

void dufmin(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fminfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fminfn(closestFormToCursor, len);
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
	closestFormToCursor = find;
	fvars(closestFormToCursor);
	if (ptrSelectedForm->fillType)
	{
		ptrSelectedForm->maxFillStitchLen = len;
		refilfn();
	}
}

void dufmax(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fmaxfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fmaxfn(closestFormToCursor, len);
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

	closestFormToCursor = find;
	fvars(closestFormToCursor);
	ref = ptrSelectedForm->rectangle.left;
	rat = len / (ptrSelectedForm->rectangle.right - ref);
	for (ind = 0; ind < sides; ind++)
		currentFormVertices[ind].x = (currentFormVertices[ind].x - ref)*rat + ref;
	frmout(closestFormToCursor);
	refilfn();
}

void dufwid(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fwidfn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fwidfn(closestFormToCursor, len);
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

	closestFormToCursor = find;
	fvars(closestFormToCursor);
	ref = ptrSelectedForm->rectangle.bottom;
	rat = len / (ptrSelectedForm->rectangle.top - ref);
	for (ind = 0; ind < sides; ind++)
		currentFormVertices[ind].y = (currentFormVertices[ind].y - ref)*rat + ref;
	frmout(closestFormToCursor);
	refilfn();
}

void dufhi(float len)
{
	unsigned ind;

	savdo();
	if (selectedFormCount)
	{
		for (ind = 0; ind < selectedFormCount; ind++)
			fhifn(selectedFormList[ind], len);
	}
	if (chkMap(FORMSEL))
		fhifn(closestFormToCursor, len);
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
		formList[closestFormToCursor].fillStart = closestVertexToCursor;
		formList[closestFormToCursor].extendedAttribute |= AT_STRT;
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
		formList[closestFormToCursor].fillEnd = closestVertexToCursor;
		formList[closestFormToCursor].extendedAttribute |= AT_END;
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

	_strlwr_s(fileName);
	strcpy_s(auxName, fileName);
	pext = strrchr(auxName, '.');
	if (pext)
		pext++;
	else
		pext = &fileName[strlen(fileName)];
	*pext = 0;
	switch (iniFile.auxFileType) {

	case AUXDST:

		strcat_s(auxName, "dst");
		break;

#if PESACT

	case AUXPES:

		strcat_s(auxName, "pes");
		break;

#endif

	default:

		strcat_s(auxName, "pcs");
	}
}

void dutxtfil()
{
	if (!iniFile.textureHeight)
		iniFile.textureHeight = ITXHI;
	if (!iniFile.textureWidth)
		iniFile.textureWidth = ITXWID;
	if (!iniFile.textureSpacing)
		iniFile.textureSpacing = (float)ITXSPAC;
	if (!iniFile.textureEditorSizePixels)
		iniFile.textureEditorSizePixels = ITXPIX;
	angfrm.sides = 0;
	setMap(TXTRED);
	setMap(ZUMED);
	rstMap(WASPAT);
	rstMap(RUNPAT);
	movStch();
	ShowWindow(hVerticalScrollBar, FALSE);
	ShowWindow(hHorizontalScrollBar, FALSE);
	cloxlst = (unsigned*)&oseq;
	cloxcnt = 0;
	setMap(INIT);
	hbsid = 0;
	if (chkMap(WASTXBAK))
	{
		redtbak();
		if (!tscr.areaHeight)
			tscr.areaHeight = iniFile.textureHeight;
		if (!tscr.spacing)
			tscr.spacing = iniFile.underlaySpacing;
		if (!tscr.width)
			tscr.width = iniFile.textureWidth;
		setMap(LASTXBAK);
		rstMap(TXBDIR);
	}
	else
	{
		tscr.index = 0;
		tscr.areaHeight = iniFile.textureHeight;
		tscr.width = iniFile.textureWidth;
		tscr.spacing = iniFile.textureSpacing;
	}
	setMap(RESTCH);
}

void txt2pix(TXPNT txp, POINT* pixp)
{
	pixp->y = tscr.height - txp.y / tscr.areaHeight*tscr.height + tscr.top;
	pixp->x = (txp.line*tscr.spacing + tscr.xOffset) / tscr.editToPixelRatio;
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

	txt2pix(txtmp[ind], &ref);
	txtxfn(ref, pix);
	if (ref.y > tscr.halfHeight)
		ref.y -= tscr.height;
	else
		ref.y += tscr.height;
	txtxfn(ref, pix);
}

void txrct2rct(TXTRCT txr, RECT* rct)
{
	TXPNT	txp;
	POINT	tpnt;
	int		bh2;

	bh2 = buttonHeight >> 1;
	txp.line = txr.left;
	txp.y = txr.top;
	txt2pix(txp, &tpnt);
	rct->left = tpnt.x - iniFile.textureEditorSizePixels;
	rct->top = tpnt.y - iniFile.textureEditorSizePixels;
	txp.line = txr.right;
	txp.y = txr.bottom;
	txt2pix(txp, &tpnt);
	rct->right = tpnt.x + iniFile.textureEditorSizePixels;
	rct->bottom = tpnt.y + iniFile.textureEditorSizePixels;
}

void ed2px(fPOINT ped, POINT* px)
{
	px->x = ped.x / tscr.editToPixelRatio;
	px->y = stitchWindowClientRect.bottom - ped.y / tscr.editToPixelRatio;
}

void px2ed(POINT px, fPOINT* ped)
{
	ped->x = px.x*tscr.editToPixelRatio;
	ped->y = tscr.screenHeight - px.y*tscr.editToPixelRatio;
}

void bxtxt(unsigned cod, TCHAR* str)
{
	SetWindowText(hButtonWin[cod], str);
}

void lodhbuf(unsigned cod)
{
	LoadString(hInst, cod, hlpbuf, HBUFSIZ);
}

void hlpflt(unsigned mcod, unsigned bcod, float dat)
{
	lodhbuf(mcod);
	sprintf_s(msgbuf, sizeof(msgbuf), hlpbuf, dat);
	bxtxt(bcod, msgbuf);
}

void drwtxbut()
{
	lodhbuf(IDS_CLEAR);
	bxtxt(HTXCLR, hlpbuf);
	hlpflt(IDS_TXHI, HTXHI, tscr.areaHeight / PFGRAN);
	redraw(hButtonWin[HTXWID]);
	hlpflt(IDS_TXSPAC, HTXSPAC, tscr.spacing / PFGRAN);
	lodhbuf(IDS_TXVRT);
	bxtxt(HTXVRT, hlpbuf);
	lodhbuf(IDS_TXHOR);
	bxtxt(HTXHOR, hlpbuf);
	lodhbuf(IDS_TXANG);
	bxtxt(HTXANG, hlpbuf);
	lodhbuf(IDS_TXMIR);
	bxtxt(HTXMIR, hlpbuf);
	SetWindowText(hButtonWin[HTXMIR + 1], "");
}

void chktx()
{
	int ind, ine;

	ine = 0;
	for (ind = 0; ind < tscr.index; ind++)
	{
		if (txtmp[ind].line <= tscr.lines&&txtmp[ind].y < tscr.areaHeight)
		{
			txtmp[ine].line = txtmp[ind].line;
			txtmp[ine].y = txtmp[ind].y;
			ine++;
		}
	}
	tscr.index = ine;
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

	FillRect(StitchWindowMemDC, &stitchWindowClientRect, hBackgroundBrush);
	edsp = tscr.areaHeight * 2 / (tscr.spacing*(tscr.lines + 2));
	pxsp = (double)stitchWindowClientRect.bottom / stitchWindowClientRect.right;
	tscr.lines = floor(tscr.width / tscr.spacing);
	wid2 = tscr.spacing*(tscr.lines + 2);
	if (rstMap(CHKTX))
		chktx();
	if (pxsp > edsp)
	{
		tscr.xOffset = 0;
		tscr.editToPixelRatio = wid2 / stitchWindowClientRect.bottom;
		ofy = (stitchWindowClientRect.bottom - tscr.areaHeight / tscr.editToPixelRatio) / 2;
	}
	else
	{
		tscr.editToPixelRatio = tscr.areaHeight * 2 / stitchWindowClientRect.bottom;
		ofy = stitchWindowClientRect.bottom >> 2;
		tscr.xOffset = (stitchWindowClientRect.right*tscr.editToPixelRatio - (tscr.lines + 2)*tscr.spacing) / 2;
	}
	tscr.top = ofy;
	tscr.bottom = stitchWindowClientRect.bottom - ofy;
	tscr.height = tscr.bottom - tscr.top;
	tscr.halfHeight = stitchWindowClientRect.bottom >> 1;
	tscr.screenHeight = stitchWindowClientRect.bottom*tscr.editToPixelRatio;
	tscr.yOffset = (tscr.screenHeight - tscr.areaHeight) / 2;
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, gridPen);
	cnt = tscr.areaHeight / iniFile.gridSize + 1;
	txp.line = 0;
	xlin[0].x = 0;
	xlin[1].x = stitchWindowClientRect.right;
	txp.y = 0;
	for (ind = 0; ind < cnt; ind++)
	{
		txt2pix(txp, &tpnt);
		xlin[0].y = xlin[1].y = tpnt.y;
		Polyline(StitchWindowMemDC, xlin, 2);
		txp.y += iniFile.gridSize;
	}
	DeleteObject(xpen);
	xpen = CreatePen(PS_SOLID, 1, userColor[activeColor]);
	SelectObject(StitchWindowMemDC, xpen);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	xlin[0].y = 0;
	xlin[1].y = stitchWindowClientRect.bottom;
	for (ind = 1; ind < tscr.lines + 1; ind++)
	{
		xlin[0].x = xlin[1].x = (ind*tscr.spacing + tscr.xOffset) / tscr.editToPixelRatio;
		Polyline(StitchWindowMemDC, xlin, 2);
	}
	xlin[0].x = 0;
	xlin[1].x = stitchWindowClientRect.right;
	xlin[0].y = xlin[1].y = tscr.top;
	Polyline(StitchWindowMemDC, xlin, 2);
	xlin[0].y = xlin[1].y = tscr.bottom;
	Polyline(StitchWindowMemDC, xlin, 2);
	DeleteObject(xpen);
	xpen = CreatePen(PS_SOLID, 1, 0xffffff);
	SelectObject(StitchWindowMemDC, xpen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	col = userColor[activeColor];
	for (ind = 0; ind < tscr.index; ind++)
	{
		dutxtx(ind, iniFile.textureEditorSizePixels);
	}
	if (cloxcnt)
	{
		txrct2rct(txrct, &txprct);
		xlin[0].y = xlin[1].y = txprct.top;
		xlin[0].x = txprct.left;
		xlin[1].x = txprct.right;
		Polyline(StitchWindowMemDC, xlin, 2);
		xlin[1].y = txprct.bottom;
		xlin[1].x = txprct.left;
		Polyline(StitchWindowMemDC, xlin, 2);
		xlin[0].x = txprct.right;
		xlin[0].y = txprct.bottom;
		Polyline(StitchWindowMemDC, xlin, 2);
		xlin[1].x = txprct.right;
		xlin[1].y = txprct.top;
		Polyline(StitchWindowMemDC, xlin, 2);
	}
	for (ind = 0; ind < cloxcnt; ind++)
	{
		dutxtx(cloxlst[ind], iniFile.textureEditorSizePixels);
		dutxtx(cloxlst[ind], iniFile.textureEditorSizePixels << 1);
	}
	BitBlt(StitchWindowDC, 0, 0, stitchWindowClientRect.right, stitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
	drwtxbut();
}

BOOL px2txt(POINT pof, TXPNT* txrec)
{
	fPOINT epnt;

	px2ed(pof, &epnt);
	txrec->line = (epnt.x - tscr.xOffset) / tscr.spacing + 0.5;
	if (txrec->line > tscr.lines)
		return 0;
	if (txrec->line < 1)
		return 0;
	if (pof.y > tscr.top)
	{
		if (pof.y > tscr.bottom)
			return 0;
		else
			txrec->y = (float)tscr.areaHeight - (float)(pof.y - tscr.top) / tscr.height*tscr.areaHeight;
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
	if (txbutfn(&txtmp[tscr.index]))
	{
		savtxt();
		tscr.index++;
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

	ref.x = msg.pt.x - stitchWindowOrigin.x;
	ref.y = msg.pt.y - stitchWindowOrigin.y;
	minlen = 1e99;
	*pclo = 0;
	for (ind = 0; ind < tscr.index; ind++)
	{
		txt2pix(txtmp[ind], &tpnt);
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

	li_Size = iniFile.textureEditorSizePixels << 2;
	xlin[0].x = xlin[1].x = txtloc.x;
	xlin[0].y = txtloc.y - li_Size;
	xlin[1].y = txtloc.y + li_Size;
	Polyline(StitchWindowDC, xlin, 2);
	xlin[0].y = xlin[1].y = txtloc.y;
	xlin[0].x = txtloc.x - li_Size;
	xlin[1].x = txtloc.x + li_Size;
	Polyline(StitchWindowDC, xlin, 2);
}

void setxmov()
{
	setMap(TXTMOV);
	txtloc.x = cloxref.x = msg.pt.x - stitchWindowOrigin.x;
	txtloc.y = cloxref.y = msg.pt.y - stitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void ritxrct()
{
	POINT	of;
	RECT	trct;
	POINT	xlin[5];

	of.x = txtloc.x - cloxref.x;
	of.y = txtloc.y - cloxref.y;
	trct.bottom = txprct.bottom + of.y;
	trct.left = txprct.left + of.x;
	trct.right = txprct.right + of.x;
	trct.top = txprct.top + of.y;
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
		txp = &txtmp[cloxlst[0]];
		rct->left = rct->right = txp->line;
		rct->top = rct->bottom = txp->y;
		for (ind = 1; ind < cloxcnt; ind++)
		{
			txp = &txtmp[cloxlst[ind]];
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
	pt->x -= tscr.xOffset;
	pt->y -= tscr.yOffset;
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
	istrt = ceil(strt / tscr.spacing);
	ifin = floor(fin / tscr.spacing);
	if (istrt < 1)
		istrt = 1;
	if (ifin > tscr.lines)
		ifin = tscr.lines;
	while (istrt <= ifin)
	{
		tflt = slop*(-pt0.x + istrt*tscr.spacing) + pt0.y;
		if (tflt > 0 && tflt < tscr.areaHeight)
		{
			txtmp[tscr.index].line = istrt;
			txtmp[tscr.index].y = tflt;
			tscr.index++;
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
		fof.x = (tscr.lines*tscr.spacing) / 2 + tscr.xOffset - tscr.formCenter.x + tscr.spacing / 2;
	else
		fof.x -= tscr.formCenter.x;
	fof.y -= tscr.formCenter.y;
	for (ind = 0; ind < angfrm.sides; ind++)
	{
		angflt[ind].x += fof.x;
		angflt[ind].y += fof.y;
	}
	cnt = angfrm.sides - 1;
	if (angfrm.type != LIN)
		cnt++;
	sides = angfrm.sides;
	for (ind = 0; ind < cnt; ind++)
	{
		inx = nxt(ind);
		dutxlin(angflt[ind], angflt[inx]);
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
	px2ed(flin[0], &pt0);
	dutxlin(pt0, pt1);
	setMap(RESTCH);
}

void ed2txp(POINT pof, TXPNT* txrec)
{
	fPOINT epnt;

	px2ed(pof, &epnt);
	txrec->line = (epnt.x - tscr.xOffset) / tscr.spacing + 0.5;
	txrec->y = (float)tscr.areaHeight - (float)(pof.y - tscr.top) / tscr.height*tscr.areaHeight;
}

int	hitxlin()
{
	int ind, hilin;

	hilin = 0;
	for (ind = 0; ind < cloxcnt; ind++)
	{
		if (txtmp[cloxlst[ind]].line > hilin)
			hilin = txtmp[cloxlst[ind]].line;
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
		tof.line = bakp*tscr.editToPixelRatio / tscr.spacing + 0.5;
		if (of.x < 0)
			tof.line = -tof.line;
		tof.y = (float)-of.y / tscr.height*tscr.areaHeight;
		tflt = txrct.top + tof.y - tscr.areaHeight;
		if (tflt > 0)
			tof.y -= tflt;
		tflt = txrct.bottom + tof.y;
		if (tflt < 0)
			tof.y -= tflt;
		ind = txrct.left + tof.line - 1;
		if (ind < 0)
			tof.line -= ind;
		ind = txrct.right + tof.line - tscr.lines;
		if (ind > 0)
			tof.line -= ind;
		for (ind = 0; ind < cloxcnt; ind++)
		{
			ptxt = &txtmp[cloxlst[ind]];
			ptxt->line += tof.line;
			ptxt->y += tof.y;
		}
		dutxrct(&txrct);
	}
	else
	{
		if (rstMap(BZUMIN))
		{
			deorg(&of);
			ed2txp(zoomBoxLine[0], &hi);
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
			for (ind = 0; ind < tscr.index; ind++)
			{
				if (txtmp[ind].y<hi.y&&
					txtmp[ind].y>lo.y&&
					txtmp[ind].line <= hi.line&&
					txtmp[ind].line >= lo.line)
				{
					cloxlst[cloxcnt] = ind;
					cloxcnt++;
				}
			}
			dutxrct(&txrct);
		}
	}
	setMap(RESTCH);
}

void angrct(fRECTANGLE* rct)
{
	unsigned	ind;

	rct->left = rct->right = angflt[0].x;
	rct->bottom = rct->top = angflt[0].y;
	for (ind = 1; ind < angfrm.sides; ind++)
	{
		if (angflt[ind].x < rct->left)
			rct->left = angflt[ind].x;
		if (angflt[ind].x > rct->right)
			rct->right = angflt[ind].x;
		if (angflt[ind].y > rct->top)
			rct->top = angflt[ind].y;
		if (angflt[ind].y < rct->bottom)
			rct->bottom = angflt[ind].y;
	}
}

void ritxfrm()
{
	unsigned ind, cnt;
	POINT		of;

	of.x = txtloc.x - cloxref.x;
	of.y = txtloc.y - cloxref.y;
	for (ind = 0; ind < angfrm.sides; ind++)
	{
		ed2px(angflt[ind], &flin[ind]);
		flin[ind].x += of.x;
		flin[ind].y += of.y;
	}
	flin[ind].x = flin[0].x;
	flin[ind].y = flin[0].y;
	cnt = angfrm.sides;
	if (angfrm.type != LIN)
		cnt++;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, flin, cnt);
}

void setxfrm()
{
	unsigned	ind;
	fRECTANGLE		arct;
	float		hi;
	double		rat;

	angrct(&arct);
	for (ind = 0; ind < angfrm.sides; ind++)
	{
		angflt[ind].x -= arct.left;
		angflt[ind].y -= arct.bottom;
	}
	angrct(&arct);
	hi = arct.top - arct.bottom;
	if (hi > tscr.areaHeight)
	{
		rat = tscr.areaHeight / hi*0.95;
		for (ind = 0; ind < angfrm.sides; ind++)
		{
			angflt[ind].x *= rat;
			angflt[ind].y *= rat;
		}
		angrct(&arct);
	}
	tscr.formCenter.x = midl(arct.right, arct.left);
	tscr.formCenter.y = midl(arct.top, arct.bottom);
	ed2px(tscr.formCenter, &cloxref);
}

void txtclp()
{
	hThrEdClip = RegisterClipboardFormat(thrEdClipFormat);
	hClipMem = GetClipboardData(hThrEdClip);
	if (hClipMem)
	{
		clipboardFormData = (FORMCLIP*)GlobalLock(hClipMem);
		if (clipboardFormData) {
			if (clipboardFormData->clipType == CLP_FRM) {
				ptrSelectedForm = &clipboardFormData->form;
				frmcpy(&angfrm, ptrSelectedForm);
				MoveMemory(&angflt, &ptrSelectedForm[1], sizeof(fPOINT)*ptrSelectedForm->sides);
				angfrm.vertices = angflt;
				rstMap(TXTLIN);
				setMap(TXTCLP);
				setMap(TXTMOV);
				setxfrm();
				txtloc.x = msg.pt.x - stitchWindowOrigin.x;
				txtloc.y = msg.pt.y - stitchWindowOrigin.y;
			}
			GlobalUnlock(hClipMem);
		}
	}
	setMap(RESTCH);
	rstMap(WASWROT);
}

void dutxtlin()
{
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, flin, 2);
}

void txtrmov()
{
	if (chkMap(TXTLIN))
	{
		dutxtlin();
		deorg(&flin[1]);
		dutxtlin();
		return;
	}
	if (chkMap(TXTCLP))
	{
		if (setMap(WASWROT))
			ritxfrm();
		txtloc.x = msg.pt.x - stitchWindowOrigin.x;
		txtloc.y = msg.pt.y - stitchWindowOrigin.y;
		ritxfrm();
	}
	else
	{
		if (cloxcnt)
		{
			ritxrct();
			txtloc.x = msg.pt.x - stitchWindowOrigin.x;
			txtloc.y = msg.pt.y - stitchWindowOrigin.y;
			ritxrct();
		}
	}
}

void txtlin()
{
	deorg(flin);
	deorg(&flin[1]);
	rstMap(TXTCLP);
	setMap(TXTLIN);
	setMap(TXTMOV);
}

void butsid(unsigned cod)
{
	RECT brct;

	chktxnum();
	txsidtyp = cod;
	GetWindowRect(hButtonWin[cod], &brct);
	hbsid = CreateWindow(
		"STATIC",
		0,
		SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
		brct.left + buttonWidthX3 - stitchWindowOrigin.x,
		brct.top - stitchWindowOrigin.y,
		buttonWidthX3,
		buttonHeight,
		hMainStitchWin,
		NULL,
		hInst,
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
	ptrSelectedForm->type = POLI;
	ptrSelectedForm->fillInfo.texture.lines = tscr.lines;
	ptrSelectedForm->fillInfo.texture.height = tscr.areaHeight;
	ptrSelectedForm->fillSpacing = tscr.spacing;
	ptrSelectedForm->lengthOrCount.stitchLength = iniFile.userStitchLength;
	ptrSelectedForm->maxFillStitchLen = iniFile.maxStitchLength;
	ptrSelectedForm->minFillStitchLen = iniFile.minStitchLength;
	ptrSelectedForm->fillColor = activeColor;
	refilfn();
}

void txvrt()
{
	if (tscr.index)
	{
		if (chkMap(FORMSEL))
		{
			fvars(closestFormToCursor);
			ptrSelectedForm->fillType = TXVRTF;
			txpar();
		}
	}
}

void txhor()
{
	if (tscr.index)
	{
		if (chkMap(FORMSEL))
		{
			fvars(closestFormToCursor);
			ptrSelectedForm->fillType = TXHORF;
			txpar();
		}
	}
}

void txang()
{
	if (tscr.index)
	{
		if (chkMap(FORMSEL))
		{
			fvars(closestFormToCursor);
			ptrSelectedForm->fillType = TXANGF;
			ptrSelectedForm->angleOrClipData.angle = (float)iniFile.fillAngle;
			txpar();
		}
	}
}

void deltx()
{
	unsigned ind, cnt;

	cnt = ptrSelectedForm->fillInfo.texture.count;
	if (txad&&istx(closestFormToCursor) && cnt)
	{
		ind = ptrSelectedForm->fillInfo.texture.index;
		MoveMemory(&txpnts[ind], &txpnts[ind + cnt], txad - (ind + cnt));
		for (ind = closestFormToCursor + 1; ind < formIndex; ind++)
		{
			if (istx(ind))
				formList[ind].fillInfo.texture.index -= cnt;
		}
		txad -= ptrSelectedForm->fillInfo.texture.count;
		ptrSelectedForm->fillInfo.texture.count = 0;
	}
}

void nutx()
{
	int			ind, ins;
	FRMHED*		pf;

	qsort((void*)&txtmp, tscr.index, 6, txcmp);
	ins = 0;
	if (formIndex)
	{
		if (istx(closestFormToCursor))
		{
			ins = formList[closestFormToCursor].fillInfo.texture.index;
			deltx();
		}
		else
		{
			for (ind = closestFormToCursor - 1; ind >= 0; ind--)
			{
				if (istx(ind))
				{
					pf = &formList[ind];
					ins = pf->fillInfo.texture.index + pf->fillInfo.texture.count;
					goto nutskp;
				}
			}
		nutskp:;
		}
	}
	txspac(ins, tscr.index);
	MoveMemory(&txpnts[ins], &txtmp, tscr.index * sizeof(TXPNT));
	ptrSelectedForm->fillInfo.texture.index = ins;
	ptrSelectedForm->fillInfo.texture.count = tscr.index;
}

void altx()
{
	int ind;
	float hi2;

	if (chkMap(FORMSEL))
	{
		hi2 = tscr.areaHeight / 2;
		clRmap((tscr.lines >> 5) + 1);
		for (ind = 0; ind < tscr.index; ind++)
			setr(txtmp[ind].line);
		for (ind = 1; ind <= tscr.lines; ind++)
		{
			if (!chkr(ind))
			{
				txtmp[tscr.index].line = ind;
				txtmp[tscr.index].y = hi2;
				tscr.index++;
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
		delmclp(closestFormToCursor);
		delsac(closestFormToCursor);
		deltx();
		savtxt();
		nutx();
		if (chku(SQRFIL))
			ptrSelectedForm->extendedAttribute |= AT_SQR;
		else
			ptrSelectedForm->extendedAttribute &= (~AT_SQR);
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
	qsort((void*)txtmp, tscr.index, 6, txcmp);
}

void dutxmir()
{
	int lin, ind, ine;

	savtxt();
	txsrt();
	lin = (tscr.lines + 1) >> 1;
	ind = tscr.index - 1;
	while (txtmp[ind].line > lin&&ind >= 0)
		ind--;
	ine = ind + 1;
	if (tscr.lines & 1)
	{
		while (ind >= 0) {
			if (txtmp[ind].line == lin) {
				ind--;
			}
			else { break; }
		}
	}
	while (ind >= 0)
	{
		txtmp[ine].line = tscr.lines - txtmp[ind].line + 1;
		txtmp[ine].y = txtmp[ind].y;
		ine++;
		ind--;
	}
	tscr.index = ine;
	setMap(RESTCH);
}

BOOL chkbut()
{
	if (msg.hwnd == hButtonWin[HTXCLR])
	{
		txdelal();
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXHI])
	{
		butsid(HTXHI);
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXWID])
	{
		butsid(HTXWID);
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXSPAC])
	{
		butsid(HTXSPAC);
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXVRT])
	{
		dutxfn(VRTYP);
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXHOR])
	{
		dutxfn(HORTYP);
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXANG])
	{
		dutxfn(ANGTYP);
		return 1;
	}
	if (msg.hwnd == hButtonWin[HTXMIR])
	{
		dutxmir();
		return 1;
	}
	return 0;
}

void txtlbut()
{
	POINT	tpnt;

	fvars(closestFormToCursor);
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
		if (tpnt.x > txprct.left&&
			tpnt.x<txprct.right&&
			tpnt.y>txprct.top&&
			tpnt.y < txprct.bottom)
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
		dutxrct(&txrct);
		setMap(RESTCH);
		return;
	}
	cloxcnt = 0;
	setMap(BZUMIN);
	rstMap(BZUM);
	zoomBoxLine[0].x = zoomBoxLine[3].x = zoomBoxLine[4].x = msg.pt.x - stitchWindowOrigin.x;
	zoomBoxLine[0].y = zoomBoxLine[1].y = msg.pt.y - stitchWindowOrigin.y;
	zoomBoxLine[4].y = zoomBoxLine[0].y - 1;
}

void redtbak()
{
	TXHST*		phst;

	//	sprintf_s(msgbuf, sizeof(msgbuf),"%d\n",ptxhst);
	//	OutputDebugString(msgbuf);
	phst = &thsts[ptxhst];
	tscr.areaHeight = phst->height;
	tscr.width = phst->width;
	tscr.spacing = phst->spacing;
	tscr.index = phst->count;
	MoveMemory(txtmp, phst->texturePoint, phst->count * sizeof(TXPNT));
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
			if (thsts[ptxhst].width)
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
			if (thsts[ptxhst].width)
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
		for (ind = 0; ind < tscr.index; ind++)
		{
			if (!chkr(ind))
			{
				txtmp[ine].line = txtmp[ind].line;
				txtmp[ine].y = txtmp[ind].y;
				ine++;
			}
		}
		cloxcnt = 0;
		tscr.index = ine;
		setMap(RESTCH);
		return;
	}
	if (tscr.index&&txtclos(&clo))
	{
		MoveMemory(&txtmp[clo], &txtmp[clo + 1], (tscr.index - clo) * sizeof(TXPNT));
		tscr.index--;
		setMap(RESTCH);
	}
}

void txdelal()
{
	chktxnum();
	savtxt();
	tscr.index = 0;
	rstxt();
	setMap(RESTCH);
}

void chktxnum()
{
	float tflt;

	tflt = atof(txbuf);
	if (tflt)
	{
		tflt *= PFGRAN;
		switch (txsidtyp)
		{
		case HTXHI:

			savtxt();
			tscr.areaHeight = tflt;
			iniFile.textureHeight = tflt;
			setMap(CHKTX);
			break;

		case HTXWID:

			savtxt();
			tscr.width = tflt;
			iniFile.textureWidth = tflt;
			setMap(CHKTX);
			break;

		case HTXSPAC:

			savtxt();
			tscr.spacing = tflt;
			iniFile.textureSpacing = tflt;
			tscr.width = tflt*tscr.lines + tflt / 2;
			setMap(CHKTX);
			break;
		}
	}
	txnind = 0;
	DestroyWindow(hbsid);
	hbsid = 0;
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
	butxt(HBOXSEL, stab[STR_BOXSEL]);
	redraw(hButtonWin[HHID]);
	if (chkMap(UPTO))
		butxt(HUPTO, stab[STR_UPON]);
	else
		butxt(HUPTO, stab[STR_UPOF]);
	SetWindowText(hButtonWin[HTXSPAC], "");
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
	for (ind = 0; ind < angfrm.sides; ind++)
	{
		angflt[ind].x *= rat;
		angflt[ind].y *= rat;
	}
	angrct(&arct);
	tscr.formCenter.x = midl(arct.right, arct.left);
	tscr.formCenter.y = midl(arct.top, arct.bottom);
	ed2px(tscr.formCenter, &cloxref);
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
			fdy = dy*tscr.editToPixelRatio;
			for (ind = 0; ind < cloxcnt; ind++)
			{
				tflt = txtmp[cloxlst[ind]].y + fdy;
				if (tflt < 0)
					return;
				if (tflt > tscr.areaHeight)
					return;
			}
			for (ind = 0; ind < cloxcnt; ind++)
				txtmp[cloxlst[ind]].y += fdy;
		}
		else
		{
			for (ind = 0; ind < cloxcnt; ind++)
			{
				tlin = txtmp[cloxlst[ind]].line + dx;
				if (tlin < 1)
					return;
				if (tlin > tscr.lines)
					return;
			}
			for (ind = 0; ind < cloxcnt; ind++)
				txtmp[cloxlst[ind]].line += dx;
		}
	}
	dutxrct(&txrct);
	setMap(RESTCH);
}

void txsnap()
{
	int ind, cnt;
	float siz2;
	TXPNT*	tp;

	if (tscr.index)
	{
		savtxt();
		siz2 = iniFile.gridSize / 2;
		if (cloxcnt)
		{
			for (ind = 0; ind < cloxcnt; ind++)
			{
				tp = &txtmp[cloxlst[ind]];
				cnt = (tp->y + siz2) / iniFile.gridSize;
				tp->y = cnt*iniFile.gridSize;
			}
		}
		else
		{
			for (ind = 0; ind < tscr.index; ind++)
			{
				tp = &txtmp[ind];
				cnt = (tp->y + siz2) / iniFile.gridSize;
				tp->y = cnt*iniFile.gridSize;
			}
		}
		setMap(RESTCH);
	}
}

void txtkey(unsigned cod)
{
	TCHAR chr;

	if (hbsid)
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

			if (txnind)
				txnind--;
			goto txskp;
		}
		if (txdig(cod, &chr))
		{
			txbuf[txnind] = chr;
			txnind++;
		}
	txskp:;
		txbuf[txnind] = 0;
		SetWindowText(hbsid, txbuf);
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

		if (OpenClipboard(hWnd))
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

		txnudg(0, iniFile.cursorNudgeStep);
		break;

	case VK_DOWN:

		txnudg(0, -iniFile.cursorNudgeStep);
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
	ptrSelectedForm->wpar = 0;
	setMap(TXFIL);
	clipboardRectSize.cx = ptrSelectedForm->fillSpacing;
	clipboardRectSize.cy = ptrSelectedForm->fillInfo.texture.height;
	txsegs = (RNGCNT*)&markedStitchMap;
	pbak = &txpnts[ptrSelectedForm->fillInfo.texture.index];
	FillMemory(txsegs, ptrSelectedForm->fillInfo.texture.lines * sizeof(RNGCNT), 0);
	pbak = &txpnts[ptrSelectedForm->fillInfo.texture.index];
	cnt = ptrSelectedForm->fillInfo.texture.count;
	if (cnt)
	{
		for (ind = cnt - 1; ind >= 0; ind--)
		{
			txsegs[pbak[ind].line - 1].line = ind;
			txsegs[pbak[ind].line - 1].stitchCount++;
		}
	}
}

void rtrtx()
{
	TXPNT*	tps;

	fvars(closestFormToCursor);
	tps = &txpnts[ptrSelectedForm->fillInfo.texture.index];
	tscr.index = ptrSelectedForm->fillInfo.texture.count;
	MoveMemory(txtmp, tps, sizeof(TXPNT)*tscr.index);
	tscr.areaHeight = ptrSelectedForm->fillInfo.texture.height;
	tscr.spacing = ptrSelectedForm->fillSpacing;
	tscr.lines = ptrSelectedForm->fillInfo.texture.lines;
	tscr.width = tscr.lines*tscr.spacing + tscr.spacing / 2;
	savtxt();
}

void rtrclp()
{
	if (chkMap(FORMSEL))
	{
		fvars(closestFormToCursor);
		if (istx(closestFormToCursor))
			rtrtx();
		else
			rtrclpfn();
	}
}

void setstxt(unsigned cod, float num)
{
	sprintf_s(hlpbuf, sizeof(hlpbuf), "%.2f", (float)num / PFGRAN);
	SetWindowText(GetDlgItem(sizdlg, cod), hlpbuf);
}

float getstxt(unsigned cod)
{
	GetWindowText(GetDlgItem(sizdlg, cod), hlpbuf, HBUFSIZ);
	return atof(hlpbuf)*PFGRAN;
}

BOOL chkasp(fPOINT* p_flt)
{
	p_flt->x = getstxt(IDC_DESWID);
	p_flt->y = getstxt(IDC_DESHI);
	if (p_flt->y / p_flt->x == daspct)
		return 1;
	else
		return 0;
}

BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	UNREFERENCED_PARAMETER(lparam);

	fPOINT	tflt;

	sizdlg = hwndlg;
	switch (umsg)
	{
	case WM_INITDIALOG:

		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		setstxt(IDC_DESWID, dsgnsiz.x);
		setstxt(IDC_DESHI, dsgnsiz.y);
		daspct = dsgnsiz.y / dsgnsiz.x;
		CheckDlgButton(hwndlg, IDC_REFILF, chku(CHREF));
		break;

	case WM_COMMAND:

		switch (LOWORD(wparam))
		{
		case IDCANCEL:

			EndDialog(hwndlg, 0);
			return TRUE;

		case IDOK:

			dsgnsiz.x = getstxt(IDC_DESWID);
			dsgnsiz.y = getstxt(IDC_DESHI);
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
					setstxt(IDC_DESWID, (float)tflt.y / daspct);
				else
					setstxt(IDC_DESHI, (float)tflt.x*daspct);
			}
			break;
		}
	}
	return 0;
}

void sadj(fPOINTATTRIBUTE* pt)
{
	pt->x = (pt->x - sizrct.left)*sizrat.x + sizrct.left;
	pt->y = (pt->y - sizrct.bottom)*sizrat.y + sizrct.bottom;
}

void sadj(fPOINT* pt)
{
	pt->x = (pt->x - sizrct.left)*sizrat.x + sizrct.left;
	pt->y = (pt->y - sizrct.bottom)*sizrat.y + sizrct.bottom;
}

void nudfn()
{
	unsigned	ind;
	fPOINT		osiz;

	osiz.x = sizrct.right - sizrct.left;
	osiz.y = sizrct.top - sizrct.bottom;
	sizrat.x = dsgnsiz.x / osiz.x;
	sizrat.y = dsgnsiz.y / osiz.y;
	for (ind = 0; ind < header.stitchCount; ind++)
		sadj(&stitchBuffer[ind]);
	for (ind = 0; ind < fltad; ind++)
		sadj(&formPoints[ind]);
	frmout(closestFormToCursor);
}

void nudsiz()
{
	fPOINT		osiz;
	int			flg;
	unsigned	ind;

	savdo();
	flg = 0;
	if (header.stitchCount)
	{
		stchrct(&sizrct);
		flg = 1;
	}
	else
	{
		if (formIndex)
		{
			frmrct(&sizrct);
			flg = 2;
		}
	}
	if (flg)
	{
		osiz.x = dsgnsiz.x = sizrct.right - sizrct.left;
		osiz.y = dsgnsiz.y = sizrct.top - sizrct.bottom;
		if (DialogBox(hInst, MAKEINTRESOURCE(IDD_SIZ), hWnd, (DLGPROC)setsprc))
		{
			flg = 0;
			if (dsgnsiz.x > iniFile.hoopSizeX)
			{
				iniFile.hoopSizeX = dsgnsiz.x*1.05;
				unzoomedRect.x = iniFile.hoopSizeX;
				flg = 1;
			}
			if (dsgnsiz.y > iniFile.hoopSizeY)
			{
				iniFile.hoopSizeY = dsgnsiz.y*1.05;
				unzoomedRect.y = iniFile.hoopSizeY;
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
			for (ind = 0; ind < formIndex; ind++)
				frmout(ind);
		}
	}
}

void dushft()
{
	//	setMap(BOXSLCT);
	setMap(BZUMIN);
	//	setMap(NOSEL);
	zoomBoxLine[0].x = zoomBoxLine[3].x = zoomBoxLine[4].x = msg.pt.x - stitchWindowOrigin.x;
	zoomBoxLine[0].y = zoomBoxLine[1].y = msg.pt.y - stitchWindowOrigin.y;
	zoomBoxLine[4].y = zoomBoxLine[0].y - 1;
	px2stch();
	zoomBoxOrigin.x = selectedPoint.x;
	zoomBoxOrigin.y = selectedPoint.y;
}

extern void unbBox();
extern void bBox();
extern void pxCor2stch(POINT pnt);

void mvshft()
{
	if (chkMap(BOXZUM) && setMap(VCAPT))
		SetCapture(hWnd);
	if (chkMap(BZUMIN))
	{
		if (msg.wParam&MK_LBUTTON)
		{
			if (setMap(VCAPT))
				SetCapture(hWnd);
			unbBox();
			zoomBoxLine[1].x = zoomBoxLine[2].x = msg.pt.x - stitchWindowOrigin.x;
			zoomBoxLine[2].y = zoomBoxLine[3].y = msg.pt.y - stitchWindowOrigin.y;
			setMap(BZUM);
			bBox();
		}
	}
}

BOOL inrct(fRECTANGLE rct, fPOINTATTRIBUTE pt)
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
	cloxlst = (unsigned*)&oseq;
	unbBox();
	rstMap(BZUMIN);
	pxCor2stch(zoomBoxLine[0]);
	srct.top = selectedPoint.y;
	srct.left = selectedPoint.x;
	pxCor2stch(zoomBoxLine[2]);
	srct.bottom = selectedPoint.y;
	srct.right = selectedPoint.x;
	rstMap(TXIN);
	tscr.index = 0;
	lin = 1;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		if (inrct(srct, stitchBuffer[ind]))
		{
			setMap(TXIN);
			txtmp[tscr.index].line = lin;
			txtmp[tscr.index].y = stitchBuffer[ind].y - srct.bottom;
			tscr.index++;
		}
		else
		{
			if (rstMap(TXIN))
				lin++;
		}
	}
	if (tscr.index) { lin = txtmp[tscr.index - 1].line; }
	tscr.spacing = (srct.right - srct.left) / lin;
	tscr.areaHeight = srct.top - srct.bottom;
	tscr.width = tscr.spacing*lin + tscr.spacing / 2;
	setMap(TXTRED);
	setMap(RESTCH);
}

void setclpspac()
{
	msgflt(IDS_CLPSPAC, iniFile.clipboardOffset / PFGRAN);
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
	while (EnumChildWindows(hMainStitchWin, enumch, 0));
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

	if (thsts[0].count)
	{
		if (txnam(nam, sizeof(nam)))
		{
			hnam = CreateFile(nam, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (hnam != INVALID_HANDLE_VALUE)
			{
				WriteFile(hnam, (char*)"txh", 4, &rot, 0);
				WriteFile(hnam, (int*)&ptxhst, 4, &rot, 0);
				WriteFile(hnam, (TXHST*)&thsts, sizeof(TXHST) * 16, &rot, 0);
				for (ind = 0; ind < 16; ind++)
				{
					if (thsts[ind].count)
						WriteFile(hnam, (TXPNT*)thsts[ind].texturePoint, thsts[ind].count * sizeof(TXPNT), &rot, 0);
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

	ptxhst = 15;
	ZeroMemory(&thsts, sizeof(TXHST) * 16);
	if (txnam(nam, sizeof(nam)))
	{
		hnam = CreateFile(nam, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (hnam != INVALID_HANDLE_VALUE)
		{
			ReadFile(hnam, (char*)&sig, 4, &l_BytesRead, 0);
			if (!strcmp(sig, "txh"))
			{
				ReadFile(hnam, (int*)&ptxhst, 4, &l_BytesRead, 0);
				ReadFile(hnam, (TXHST*)&thsts, sizeof(TXHST) * 16, &l_BytesRead, 0);
				for (ind = 0; ind < (l_BytesRead / sizeof(TXHST)); ind++)
				{
					if (thsts[ind].count)
					{
						thsts[ind].texturePoint = new TXPNT[thsts[ind].count];
						ReadFile((TXPNT*)hnam, thsts[ind].texturePoint, sizeof(TXPNT)*thsts[ind].count, &l_BytesRead, 0);
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

	angbak = rotationAngle;
	rotationAngle = tang;
	MoveMemory(&angfrm, ptrSelectedForm, sizeof(FRMHED));
	MoveMemory(&angflt, currentFormVertices, sizeof(fPOINT)*sides);
	rotationCenter.x = (double)(angfrm.rectangle.right - angfrm.rectangle.left) / 2 + angfrm.rectangle.left;
	rotationCenter.y = (double)(angfrm.rectangle.top - angfrm.rectangle.bottom) / 2 + angfrm.rectangle.bottom;
	angfrm.vertices = angflt;
	if (rotationAngle)
	{
		for (ind = 0; ind < sides; ind++)
			rotflt(&angflt[ind]);
	}
	ptrSelectedForm = &angfrm;
	currentFormVertices = angflt;
	rotationAngle = angbak;
}

void chgwrn()
{
	toglu(WRNOF);
	wrnmen();
	setMap(DUMEN);
}

void chgchk(int cod)
{
	iniFile.dataCheck = cod;
	chkmen();
	setMap(DUMEN);
}

void lodchk()
{
	unsigned	ind, at;

	delinf();
	for (ind = 0; ind < formIndex; ind++) {

		ptrSelectedForm = &formList[ind];
		if (!ptrSelectedForm->type)
			ptrSelectedForm->type = POLI;
		else {

			if (ptrSelectedForm->type == LIN) {

				if (ptrSelectedForm->fillType != CONTF) {

					ptrSelectedForm->fillType = 0;
					ptrSelectedForm->lengthOrCount.clipCount = 0;
				}
			}
		}
		frmout(ind);
		if (!ptrSelectedForm->maxFillStitchLen)
			ptrSelectedForm->maxFillStitchLen = iniFile.maxStitchLength;
		if (!ptrSelectedForm->maxBorderStitchLen)
			ptrSelectedForm->maxBorderStitchLen = iniFile.maxStitchLength;
	}
	clRmap((MAXFORMS >> 5) + 1);
	for (ind = 0; ind < header.stitchCount; ind++) {

		at = stitchBuffer[ind].attribute;
		if ((at&TYPMSK) == TYPFRM)
			setr((at&FRMSK) >> FRMSHFT);

	}
	for (ind = 0; ind < formIndex; ind++) {

		if (!chkr(ind))
			formList[ind].fillType = 0;
	}
	clRmap((MAXFORMS >> 5) + 1);
	for (ind = 0; ind < header.stitchCount; ind++) {

		at = stitchBuffer[ind].attribute;
		if (at&TYPBRD)
			setr((at&FRMSK) >> FRMSHFT);
	}
	for (ind = 0; ind < formIndex; ind++) {

		if (!chkr(ind))
			formList[ind].edgeType = 0;
	}
}

void chkclp(FRMHED* fp, BADCNTS* bc)
{
	if (bc->clp == fp->angleOrClipData.clip - clipboardPoints)
		bc->clp += fp->lengthOrCount.clipCount;
	else
		bc->attribute |= BADCLP;
}

void chkeclp(FRMHED* fp, BADCNTS* bc)
{
	if (bc->clp == fp->borderClipData - clipboardPoints)
		bc->clp += fp->clipEntries;
	else
		bc->attribute |= BADCLP;
}

unsigned frmchkfn()
{
	unsigned	ind;
	FRMHED*		fp;
	BADCNTS		bc;

	FillMemory(&bc, sizeof(BADCNTS), 0);
	if (formIndex)
	{
		for (ind = 0; ind < formIndex; ind++)
		{
			fp = &formList[ind];
			if (!(bc.attribute&BADFLT))
			{
				if (!fp->sides)
					bc.attribute |= BADFLT;
				if (bc.flt == fp->vertices - formPoints)
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
					if (bc.sat == fp->satinOrAngle.sac - satks)
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
		if (bc.flt != (int)fltad)
			bc.attribute |= BADFLT;
		if (bc.clp != (int)clpad)
			bc.attribute |= BADCLP;
		if (bc.sat != (int)satkad)
			bc.attribute |= BADSAT;
		if (bc.tx != txad)
			bc.attribute |= BADTX;
	}
	return bc.attribute;
}

void frmchkx()
{
	unsigned cod;

	if (iniFile.dataCheck)
	{
		cod = frmchkfn();
		switch (iniFile.dataCheck)
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

	fp = &formList[find];
	if (isclp(find))
		bc->clp += fp->lengthOrCount.clipCount;
	if (iseclp(find))
		bc->clp += fp->clipEntries;
	if (fp->type == SAT)
		bc->sat += fp->satinGuideCount;
	if (istx(find))
		bc->tx += fp->fillInfo.texture.count;
}

void chkfstch()
{
	unsigned cod, ind;

	cod = formIndex << FRMSHFT;
	for (ind = 0; ind < header.stitchCount; ind++)
	{
		if ((stitchBuffer[ind].attribute&FRMSK) >= cod)
			stitchBuffer[ind].attribute = NOTFRM;
	}
}

void repflt()
{
	fPOINT* tflt;
	unsigned ind, loc, cnt, dif;
	FRMHED* fp;
	BADCNTS bc;

	loc = 0;
	for (ind = 0; ind < formIndex; ind++)
	{
		if (formList[ind].sides)
			MoveMemory(&formList[loc++], &formList[ind], sizeof(FRMHED));
	}
	formIndex = loc;
	ZeroMemory(&bc, sizeof(BADCNTS));
	tflt = (fPOINT*)bseq;
	loc = cnt = 0;
	for (ind = 0; ind < formIndex; ind++)
	{
		fp = &formList[ind];
		dif = fp->vertices - formPoints;
		if (fltad >= dif + fp->sides)
		{
			MoveMemory(&tflt[loc], fp->vertices, fp->sides * sizeof(fPOINT));
			fp->vertices = &formPoints[loc];
			loc += fp->sides;
			bcup(ind, &bc);
		}
		else
		{
			if (dif < fltad)
			{
				fp->sides = fltad - dif;
				delsac(ind);
				MoveMemory(&tflt[loc], fp->vertices, fp->sides * sizeof(fPOINT));
				bcup(ind, &bc);
			}
			else
			{
				formIndex = ind;
				clpad = bc.clp;
				satkad = bc.sat;
				txad = bc.tx;
				chkfstch();
				adbad(IDS_FRMDAT, formIndex - ind + 1);
				goto rfltskp;
			}
		}
	}
	fltad = loc;
rfltskp:;
	MoveMemory(formPoints, tflt, sizeof(fPOINT)*fltad);
}

void repclp()
{
	FRMHED*		fp;
	unsigned	ind, cnt, loc, bcnt;
	fPOINT*		tclps;

	bcnt = cnt = 0;
	tclps = (fPOINT*)&bseq;
	for (ind = 0; ind < formIndex; ind++)
	{
		fp = &formList[ind];
		if (isclp(ind))
		{
			loc = fp->angleOrClipData.clip - clipboardPoints;
			if (loc + fp->lengthOrCount.clipCount < clpad)
			{
				MoveMemory(&tclps[cnt], fp->angleOrClipData.clip, sizeof(fPOINT)*fp->lengthOrCount.clipCount);
				fp->angleOrClipData.clip = &clipboardPoints[cnt];
				cnt += fp->lengthOrCount.clipCount;
			}
			else
			{
				if (loc < clpad)
				{
					fp->lengthOrCount.clipCount = fltad - loc;
					MoveMemory(&tclps[cnt], fp->angleOrClipData.clip, sizeof(fPOINT)*fp->lengthOrCount.clipCount);
					fp->angleOrClipData.clip = &clipboardPoints[cnt];
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
			loc = fp->borderClipData - clipboardPoints;
			if (loc + fp->clipEntries < clpad)
			{
				MoveMemory(&tclps[cnt], fp->borderClipData, sizeof(fPOINT)*fp->clipEntries);
				fp->borderClipData = &clipboardPoints[cnt];
				cnt += fp->clipEntries;
			}
			else
			{
				if (loc < clpad)
				{
					fp->clipEntries = fltad - loc;
					MoveMemory(&tclps[cnt], fp->borderClipData, sizeof(fPOINT)*fp->clipEntries);
					fp->borderClipData = &clipboardPoints[cnt];
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
	MoveMemory(&clipboardPoints, tclps, cnt * sizeof(fPOINT));
	clpad = cnt;
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
	for (ind = 0; ind < formIndex; ind++)
	{
		fp = &formList[ind];
		if (fp->type == SAT)
		{
			dif = fp->satinOrAngle.sac - satks;
			if (fltad > dif + fp->sides)
			{
				MoveMemory(&satks[loc], fp->satinOrAngle.sac, fp->satinGuideCount * sizeof(SATCON));
				fp->satinOrAngle.sac = &satks[loc];
				loc += fp->satinGuideCount;
				bcup(ind, &bc);
			}
			else
			{
				if (dif < satkad)
				{
					fp->satinGuideCount = satkad - dif;
					MoveMemory(&satks[loc], fp->satinOrAngle.sac, fp->satinGuideCount * sizeof(SATCON));
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
	satkad = loc;
}

void reptx()
{
	unsigned ind, loc, cnt;
	FRMHED* fp;
	BADCNTS bc;

	ZeroMemory(&bc, sizeof(BADCNTS));
	loc = cnt = 0;
	for (ind = 0; ind < formIndex; ind++)
	{
		if (istx(ind))
		{
			fp = &formList[ind];
			if (txad > fp->fillInfo.texture.index + fp->fillInfo.texture.count)
			{
				MoveMemory(&satks[loc], &satks[fp->fillInfo.texture.index], fp->fillInfo.texture.count * sizeof(SATCON));
				fp->fillInfo.texture.index = loc;
				loc += fp->fillInfo.texture.count;
				bcup(ind, &bc);
			}
			else
			{
				if (txad > fp->fillInfo.texture.index)
				{
					fp->fillInfo.texture.count = txad - fp->fillInfo.texture.index;
					MoveMemory(&satks[loc], &satks[fp->fillInfo.texture.index], fp->fillInfo.texture.count * sizeof(SATCON));
					fp->fillInfo.texture.index = loc;
					bcup(ind, &bc);
					loc = bc.tx;
				}
				else
					fp->fillType = 0;
			}
		}
	}
	txad = loc;
}

void repar()
{
	unsigned cod;

	savdo();
	sdat = msgbuf;
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
	if (sdat != msgbuf)
	{
		sdat--;
		*sdat = 0;
		shoMsg(msgbuf);
	}
}

void tst()
{
	strcpy_s(iniFile.designerName, "Mr");
	strcpy_s(thrName, iniFile.designerName);
	setMap(RESTCH);
}
