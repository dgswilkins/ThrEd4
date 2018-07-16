#include "stdafx.h"

#include "wrappers.h"
#include "Resources/resource.h"
#include "thred.h"

namespace fs = std::experimental::filesystem;

extern unsigned                   ActiveColor;
extern unsigned                   ActiveLayer;
extern fPOINT                     AngledFormVertices[MAXFRMLINS];
extern FRMHED                     AngledForm;
extern LPWSTR*                    ArgList; // command line argument array
extern fs::path*                  AuxName;
extern HBRUSH                     BackgroundBrush;
extern BSEQPNT                    BSequence[BSEQLEN];
extern unsigned                   ButtonHeight;
extern unsigned                   ButtonWidthX3;
extern std::vector<HWND>*         ButtonWin;
extern FORMCLIP*                  ClipFormHeader;
extern HGLOBAL                    ClipMemory;
extern fPOINT                     ClipPoints[MAXITEMS];
extern fRECTANGLE                 ClipRect;
extern FLSIZ                      ClipRectSize;
extern size_t                     ClipStitchCount;
extern fPOINTATTR                 ClipBuffer[MAXFRMLINS];
extern size_t                     ClosestFormToCursor;
extern unsigned                   ClosestPointIndex;
extern size_t                     ClosestVertexToCursor;
extern size_t                     ClipPointIndex;
extern fPOINT*                    CurrentFormVertices;
extern std::wstring*              DesignerName;
extern size_t                     FormVertexIndex;
extern fPOINT                     FormMoveDelta;
extern size_t                     FormIndex;
extern POINT                      FormLines[MAXFRMLINS];
extern FRMHED                     FormList[MAXFORMS];
extern fPOINT                     FormVertices[MAXITEMS];
extern HPEN                       GridPen;
extern HWND                       HorizontalScrollBar;
extern INIFILE                    IniFile;
extern std::vector<fPOINT>*       InsidePoints;
extern double                     LineSpacing;
extern HWND                       MainStitchWin;
extern MSG                        Msg;
extern wchar_t                    MsgBuffer[MSGSIZ];
extern size_t                     NewFormVertexCount;
extern size_t                     OutputIndex;
extern std::vector<fPOINT>*       OutsidePoints;
extern fPOINT                     OSequence[OSEQLEN];
extern PCSHEADER                  PCSHeader;
extern unsigned                   PseudoRandomValue;
extern FRMHED*                    SelectedForm;
extern size_t                     SatinGuideIndex;
extern SATCON                     SatinGuides[MAXSAC];
extern EnumMap<StateFlag>         StateMap;
extern std::vector<size_t>*       SelectedFormList;
extern fPOINT                     SelectedPoint;
extern size_t                     SequenceIndex;
extern std::vector<std::wstring>* StringTable;
extern fPOINTATTR                 StitchBuffer[MAXITEMS * 2];
extern RECT                       StitchWindowClientRect;
extern HDC                        StitchWindowDC;
extern HDC                        StitchWindowMemDC;
extern POINT                      StitchWindowOrigin;
extern unsigned                   ThrEdClip;
extern const wchar_t*             ThrEdClipFormat;
extern HINSTANCE                  ThrEdInstance;
extern HWND                       ThrEdWindow;
extern fs::path*                  ThrName;
extern unsigned                   AppliqueColor;
extern POINT                      UnzoomedRect;
extern COLORREF                   UserColor[16];
extern EnumMap<UserFlag>          UserFlagMap;
extern double                     UserStitchLength;
extern size_t                     VertexCount;
extern HWND                       VerticalScrollBar;
extern fs::path*                  WorkingFileName;
extern POINT                      ZoomBoxLine[5];
extern fPOINT                     ZoomBoxOrigin;
extern dRECTANGLE                 ZoomRect;

extern void             adbad(std::wstring& repairMessage, unsigned code, size_t count);
extern SATCON*          adsatk(size_t count);
extern void             angclpfn(const std::vector<RNGCNT>& textureSegments);
extern void             bBox();
extern void             butxt(unsigned iButton, const std::wstring& buttonText);
extern void             centir();
extern void             chkmen();
extern void             chkseq(bool border);
extern bool             cisin(float xCoordinate, float yCoordinate);
extern size_t           closflt(float xCoordinate, float yCoordinate);
extern void             coltab();
extern void             datmsg(unsigned code);
extern void             delclps(size_t iForm);
extern void             delinf();
extern void             delmclp(size_t iForm);
extern void             delsac(size_t formIndex);
extern void             dusqr();
extern bool             filmsgs(unsigned code);
extern void             frmclr(FRMHED* const destination);
extern void             frmout(size_t formIndex);
extern void             frmrct(fRECTANGLE& rectangle);
extern void             fvars(size_t iForm);
extern void             hsizmsg();
extern bool             isclp(size_t iForm);
extern bool             iseclp(size_t iForm);
extern inline void      loadString(std::wstring& sDest, unsigned stringID);
extern void             mdufrm();
extern float            midl(float high, float low);
extern void             movStch();
extern void             msgflt(unsigned messageId, float value);
extern void             numWnd();
extern constexpr size_t nxt(size_t iVertex);
extern constexpr size_t prv(size_t iVertex);
extern unsigned         psg();
extern unsigned         px2stch();
extern void             pxCor2stch(const POINT& pnt);
extern void             redraw(HWND window);
extern void             refil();
extern void             refilal();
extern void             refilfn();
extern void             rtrclpfn();
extern void             satfil();
extern void             satout(double satinWidth);
extern void             savdo();
extern void             save();
extern void             setmfrm();
extern void             shoMsg(const std::wstring& message);
extern void             shoseln(unsigned code0, unsigned code1);
extern void             stchrct(fRECTANGLE& rectangle);
extern void             tabmsg(unsigned code);
extern void             unbBox();
extern void             unmsg();
extern void             wrnmen();
extern void             zumhom();

unsigned short DaisyTypeStrings[] = {
	IDS_DAZCRV, IDS_DAZSAW, IDS_DAZRMP, IDS_DAZRAG, IDS_DAZCOG, IDS_DAZHART,
};

fPOINT                 DesignSize;                    // design size
std::vector<TXPNT>*    TexturePointsBuffer;           // buffer for textured fill points
size_t                 TextureIndex;                  // next textured fill point index
unsigned               TextureWindowId;               // id of the window being updated
std::wstring*          TextureInputBuffer;            // texture fill number buffer
HWND                   SideWindowButton;              // button side window
RECT                   TexturePixelRect;              // screen selected texture points rectangle
TXTRCT                 TextureRect;                   // selected texture points rectangle
POINT                  SelectTexturePointsOrigin;     // original location of selected texture points
std::vector<unsigned>* SelectedTexturePointsList;     // list of selected points
POINT                  TextureCursorLocation;         // texture editor move cursor location
HPEN                   TextureCrossPen;               // texture editor cross pen
TXTSCR                 TextureScreen;                 // texture editor layout parameters
std::vector<TXPNT>*    TempTexturePoints;             // temporary storage for textured fill data
unsigned               ColorOrder[16];                // color order adjusted for applique
fPOINT                 InterleaveSequence[MAXITEMS];  // storage for interleave points
unsigned               InterleaveSequenceIndex;       // index into the interleave sequence
INSREC                 InterleaveSequenceIndices[10]; // indices into interleave points

enum
{
	TYPE_APPLIQUE = 1, // applique
	TYPE_CWALK,        // center walk
	TYPE_EWALK,        // edge walk
	TYPE_UNDERLAY,     // underlay
	TYPE_FTHR,         // feather
	TYPE_FILL,         // fill
	TYPE_BORDER        // border
};

char StitchTypes[] = {
	0,             // 0 no type
	TYPE_APPLIQUE, // 1 applique
	TYPE_CWALK,    // 2 center walk
	TYPE_EWALK,    // 3 edge walk
	TYPE_UNDERLAY, // 4 underlay
	0,             // 5 knot
	TYPE_FTHR,     // 6 feather
	0,             // 7 layer
	0,             // 8 layer
	0,             // 9 layer
	0,             // 10 reserved
	TYPE_FILL,     // 11 fill
	TYPE_BORDER,   // 12 border
};

TXHST TextureHistory[ITXBUFLEN]; // texture editor history headers
int   TextureHistoryIndex;       // pointer to the next texture history buffer

void setfchk() {
	if (IniFile.dataCheck)
		StateMap.set(StateFlag::FCHK);
}

#ifdef _DEBUG

void prbug() noexcept {
	OutputDebugString(MsgBuffer);
}
#endif

bool istx(size_t iForm) noexcept {
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

void rstxt() {
	SelectedTexturePointsList->clear();
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
	if (TextureHistoryIndex > 0) {
		TextureHistoryIndex--;
	}
	else {
		TextureHistoryIndex = ITXBUFLEN - 1;
	}
}

void txrfor() noexcept {
	if (TextureHistoryIndex < (ITXBUFLEN - 1)) {
		TextureHistoryIndex++;
	}
	else {
		TextureHistoryIndex = 0;
	}
}

bool chktxh(_In_ const TXHST* historyItem) {
	if (historyItem) {
		unsigned iPoint = 0;

		if (historyItem->texturePoint.size() != TempTexturePoints->size())
			return true;
		if (historyItem->height != TextureScreen.areaHeight)
			return true;
		if (historyItem->spacing != TextureScreen.spacing)
			return true;
		if (historyItem->width != TextureScreen.width)
			return true;
		for (iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
			if ((*TempTexturePoints)[iPoint].line != historyItem->texturePoint[iPoint].line)
				return true;
			if ((*TempTexturePoints)[iPoint].y != historyItem->texturePoint[iPoint].y)
				return true;
		}
	}
	return false;
}

void savtxt() {
	TXHST* currentHistoryItem = nullptr;

	if (TempTexturePoints->size()) {
		currentHistoryItem = &TextureHistory[TextureHistoryIndex];
		if (chktxh(currentHistoryItem)) {
			StateMap.set(StateFlag::WASTXBAK);
			StateMap.reset(StateFlag::TXBDIR);
			StateMap.reset(StateFlag::LASTXBAK);
			txrfor();
			currentHistoryItem          = &TextureHistory[TextureHistoryIndex];
			currentHistoryItem->height  = TextureScreen.areaHeight;
			currentHistoryItem->width   = TextureScreen.width;
			currentHistoryItem->spacing = TextureScreen.spacing;
			currentHistoryItem->texturePoint.clear();
			currentHistoryItem->texturePoint.reserve(TempTexturePoints->size());
			for (auto i = 0u; i < TempTexturePoints->size(); i++) {
				currentHistoryItem->texturePoint.push_back((*TempTexturePoints)[i]);
			}
		}
	}
}

void deorg(POINT& point) noexcept {
	point.x = Msg.pt.x - StitchWindowOrigin.x;
	point.y = Msg.pt.y - StitchWindowOrigin.y;
}

void fthvars(FEATHER& feather) {
	StateMap.reset(StateFlag::BARSAT);
	StateMap.reset(StateFlag::FTHR);

	feather.fillType          = SelectedForm->fillInfo.feather.fillType;
	feather.formRatio         = SelectedForm->fillInfo.feather.ratio;
	feather.minStitch         = SelectedForm->fillInfo.feather.minStitchSize;
	feather.totalCount        = SelectedForm->fillInfo.feather.count;
	feather.extendedAttribute = SelectedForm->extendedAttribute;
	feather.countUp           = SelectedForm->fillInfo.feather.upCount;
	feather.upCount           = feather.countUp;
	feather.countDown         = SelectedForm->fillInfo.feather.downCount;
	feather.downCount         = feather.countDown;
	feather.phaseIndex        = feather.upCount + feather.downCount;
	if (feather.extendedAttribute & AT_FTHBLND)
		StateMap.set(StateFlag::BARSAT);
	else
		StateMap.set(StateFlag::FTHR);
}

constexpr float durat(float start, float finish, float featherRatio) {
	return (finish - start) * featherRatio + start;
}

constexpr float duxrat(float strt, float fin, float featherRatioLocal) {
	return (fin - strt) * featherRatioLocal + strt;
}

void duxrats(size_t start, size_t finish, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = duxrat(BSequence[finish].x, BSequence[start].x, featherRatioLocal);
	point.y = duxrat(BSequence[finish].y, BSequence[start].y, featherRatioLocal);
}

void durats(size_t iSequence, fPOINT& point, FEATHER& feather) noexcept {
	const double stitchLength
	    = hypot(BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y);
	fPOINT adjustedPoint = {};

	if (stitchLength < feather.minStitch) {
		point.x = BSequence[iSequence].x;
		point.y = BSequence[iSequence].y;
	}
	else {
		feather.ratioLocal = feather.minStitch / stitchLength;
		adjustedPoint.x    = duxrat(BSequence[iSequence + 1].x, BSequence[iSequence].x, feather.ratioLocal);
		adjustedPoint.y    = duxrat(BSequence[iSequence + 1].y, BSequence[iSequence].y, feather.ratioLocal);
		point.x            = durat(adjustedPoint.x, BSequence[iSequence].x, feather.ratio);
		point.y            = durat(adjustedPoint.y, BSequence[iSequence].y, feather.ratio);
	}
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

void nurat(FEATHER& feather) {
	float remainder;

	remainder = fmod(feather.globalPosition, 1);
	switch (feather.fillType) {
	case FTHPSG:
		if (feather.upCount) {
			if (feather.countUp) {
				feather.ratio = static_cast<float>(feather.totalCount - (psg() % feather.totalCount)) / feather.totalCount;
				feather.countUp--;
			}
			else {
				feather.ratio = static_cast<float>(feather.totalCount - (bpsg() % feather.totalCount)) / feather.totalCount;
				if (feather.countDown)
					feather.countDown--;
				else {
					PseudoRandomValue = FSED;
					feather.countUp   = feather.upCount;
					feather.countDown = feather.downCount;
				}
			}
		}
		else
			feather.ratio = static_cast<float>(feather.totalCount - (psg() % feather.totalCount)) / feather.totalCount;
		feather.ratio *= feather.formRatio;
		break;
	case FTHFAZ:
		if (feather.phase >= feather.upCount)
			feather.ratio = 1;
		else
			feather.ratio = feather.formRatio;
		break;
	case FTHSIN:
		if (remainder > feather.globalRatio)
			feather.ratio = sin((1 - remainder) / (1 - feather.globalRatio) * PI + PI) * 0.5 + 0.5;
		else
			feather.ratio = sin(remainder / feather.globalRatio * PI) * 0.5 + 0.5;
		feather.ratio *= feather.formRatio;
		break;
	case FTHSIN2:
		if (remainder > feather.globalRatio)
			feather.ratio = sin((1 - remainder) / (1 - feather.globalRatio) * PI);
		else
			feather.ratio = sin(remainder / feather.globalRatio * PI);
		feather.ratio *= feather.formRatio;
		break;
	case FTHRMP:
		if (remainder > feather.globalRatio)
			feather.ratio = (1 - remainder) / (1 - feather.globalRatio);
		else
			feather.ratio = remainder / feather.globalRatio;
		feather.ratio *= feather.formRatio;
		break;
	case FTHLIN:
	default:
		feather.ratio = feather.formRatio;
	}
	++feather.phase %= feather.phaseIndex;
	feather.globalPosition += feather.globalStep;
}

void fthfn(size_t iSequence, FEATHER& feather) {
	nurat(feather);
	durats(iSequence, OSequence[iSequence], feather);
}

void ratpnt(size_t iPoint, size_t iNextPoint, fPOINT& point, float featherRatio) noexcept {
	point.x = (BSequence[iNextPoint].x - BSequence[iPoint].x) * featherRatio + BSequence[iPoint].x;
	point.y = (BSequence[iNextPoint].y - BSequence[iPoint].y) * featherRatio + BSequence[iPoint].y;
}

fPOINT midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept {
	return { (endPoint.x - startPoint.x) * 0.5f + startPoint.x, (endPoint.y - startPoint.y) * 0.5f + startPoint.y };
}

void xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = (endPoint.x - startPoint.x) * featherRatioLocal + startPoint.x;
	point.y = (endPoint.y - startPoint.y) * featherRatioLocal + startPoint.y;
}

void fthrbfn(size_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence) {
	fPOINT currentPoint     = {};
	fPOINT nextPoint        = {};
	fPOINT currentHighPoint = {};
	fPOINT currentLowPoint  = {};
	fPOINT nextHighPoint    = {};
	fPOINT nextLowPoint     = {};
	fPOINT midPoint         = {};

	const double length
	    = hypot(BSequence[iSequence + 1].y - BSequence[iSequence].y, BSequence[iSequence + 1].x - BSequence[iSequence].x);

	nurat(feather);
	if (length < (feather.minStitch * 2)) {
		feather.ratio = 0.5;
		ratpnt(iSequence, iSequence + 1, currentPoint, feather.ratio);
		ratpnt(iSequence + 3, iSequence + 2, nextPoint, feather.ratio);
	}
	else {
		feather.ratioLocal = feather.minStitch / length;
		duxrats(iSequence, iSequence + 1, currentLowPoint, feather.ratioLocal);
		duxrats(iSequence + 3, iSequence + 2, nextLowPoint, feather.ratioLocal);
		feather.ratioLocal = 1 - feather.ratioLocal;
		duxrats(iSequence, iSequence + 1, currentHighPoint, feather.ratioLocal);
		duxrats(iSequence + 3, iSequence + 2, nextHighPoint, feather.ratioLocal);
		feather.ratioLocal = feather.ratio;
		xratf(currentLowPoint, currentHighPoint, currentPoint, feather.ratioLocal);
		xratf(nextLowPoint, nextHighPoint, nextPoint, feather.ratioLocal);
	}
	midPoint                 = midpnt(currentPoint, nextPoint);
	OSequence[OutputIndex++] = BSequence[iSequence];
	OSequence[OutputIndex++] = midPoint;
	featherSequence.push_back({ BSequence[iSequence + 1].x, BSequence[iSequence + 1].y });
	featherSequence.push_back(midPoint);
}

void fthdfn(size_t iSequence, FEATHER& feather) {
	const double length
	    = hypot(BSequence[iSequence + 1].y - BSequence[iSequence].y, BSequence[iSequence + 1].x - BSequence[iSequence].x);
	fPOINT adjustedPoint = {};
	fPOINT currentPoint  = {};
	fPOINT nextPoint     = {};

	nurat(feather);
	OSequence[iSequence]     = BSequence[iSequence];
	OSequence[iSequence + 1] = BSequence[iSequence + 1];
	if (length > feather.minStitch) {
		feather.ratioLocal = 0.5;
		duxrats(iSequence + 1, iSequence, adjustedPoint, feather.ratioLocal);
		feather.ratioLocal = feather.minStitch / length / 2;
		xratf(adjustedPoint, OSequence[iSequence], currentPoint, feather.ratioLocal);
		xratf(adjustedPoint, OSequence[iSequence + 1], nextPoint, feather.ratioLocal);
		feather.ratioLocal = feather.ratio;
		xratf(currentPoint, OSequence[iSequence], OSequence[iSequence], feather.ratioLocal);
		xratf(nextPoint, OSequence[iSequence + 1], OSequence[iSequence + 1], feather.ratioLocal);
	}
}

void fritfil(std::vector<fPOINT>& featherSequence, unsigned& interleaveSequenceIndex2) {
	unsigned iSequence        = 0;
	size_t   iReverseSequence = 0;

	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPFRM;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillColor;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkseq(false);
		interleaveSequenceIndex2++;
		if (SelectedForm->extendedAttribute & AT_FTHBLND
		    && ~(SelectedForm->extendedAttribute & (AT_FTHUP | AT_FTHBTH)) != (AT_FTHUP | AT_FTHBTH)) {
			InterleaveSequenceIndices[interleaveSequenceIndex2].code  = FTHMSK;
			InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillInfo.feather.color;
			InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
			InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FTH;

			const auto sequenceMax = featherSequence.size();
			iReverseSequence       = sequenceMax - 1;
			for (iSequence = 0; iSequence < sequenceMax; iSequence++) {
				OSequence[iSequence] = featherSequence[iReverseSequence];
				iReverseSequence--;
			}
			SequenceIndex = sequenceMax;
			chkseq(false);
			interleaveSequenceIndex2++;
		}
	}
}

void fthrfn(unsigned& interleaveSequenceIndex2) {
	size_t              ind = 0, res = 0;
	const double        savedSpacing = LineSpacing;
	FEATHER             feather      = {};
	std::vector<fPOINT> featherSequence;

	// ToDo - what does this function do
	PseudoRandomValue = FSED;
	fthvars(feather);
	LineSpacing = SelectedForm->fillSpacing;
	satfil();
	BSequence[0].attribute = 0;
	BSequence[1].attribute = 1;
	if (!feather.phaseIndex)
		feather.phaseIndex = 1;
	ind = SequenceIndex / (feather.phaseIndex << 2);
	res = SequenceIndex % (feather.phaseIndex << 2);
	if (res > (feather.phaseIndex << 1))
		ind++;
	feather.globalPosition       = 0;
	feather.globalStep           = 4.0 / SequenceIndex * ind;
	feather.globalPhase          = static_cast<float>(SequenceIndex) / ind;
	feather.globalRatio          = static_cast<float>(feather.countUp) / feather.phaseIndex;
	feather.globalUp             = feather.globalPhase * feather.globalRatio;
	feather.globalDown           = feather.globalPhase - feather.globalUp;
	SelectedForm->fillType       = FTHF;
	feather.phase                = 1;
	BSequence[SequenceIndex]     = BSequence[SequenceIndex - 2];
	BSequence[SequenceIndex + 1] = BSequence[SequenceIndex - 1];
	if (feather.extendedAttribute & AT_FTHBLND) {
		OutputIndex = 0;
		for (ind = 0; ind < SequenceIndex; ind++) {
			if (!BSequence[ind].attribute)
				fthrbfn(ind, feather, featherSequence);
		}
	}
	else {
		if (SelectedForm->extendedAttribute & AT_FTHBTH) {
			for (ind = 0; ind <= SequenceIndex; ind++) {
				if (!BSequence[ind].attribute)
					fthdfn(ind, feather);
			}
			ind--;
		}
		else {
			for (ind = 0; ind <= SequenceIndex; ind++) {
				if (BSequence[ind].attribute) {
					if (feather.extendedAttribute & AT_FTHUP)
						fthfn(ind, feather);
					else
						OSequence[ind] = BSequence[ind];
					;
				}
				else {
					if (feather.extendedAttribute & AT_FTHUP)
						OSequence[ind] = BSequence[ind];
					else
						fthfn(ind, feather);
				}
			}
			ind--;
		}
		OutputIndex = ind;
	}
	StateMap.reset(StateFlag::FTHR);
	StateMap.reset(StateFlag::BARSAT);
	LineSpacing   = savedSpacing;
	SequenceIndex = OutputIndex;
	fritfil(featherSequence, interleaveSequenceIndex2);
}

void deltx() {
	unsigned iBuffer = 0;
	size_t   iForm   = 0;
	bool     flag    = false;

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
			std::vector<TXPNT> textureBuffer;
			textureBuffer.reserve(TextureIndex);
			iBuffer = 0;
			for (iForm = 0; iForm < ClosestFormToCursor; iForm++) {
				if (istx(iForm)) {
					auto startSource = TexturePointsBuffer->cbegin() + FormList[iForm].fillInfo.texture.index;
					auto endSource   = startSource + FormList[iForm].fillInfo.texture.count;
					textureBuffer.resize(textureBuffer.size() + FormList[iForm].fillInfo.texture.count);
					auto destination                       = textureBuffer.begin() + iBuffer;
					auto _                                 = std::copy(startSource, endSource, destination);
					FormList[iForm].fillInfo.texture.index = iBuffer;
					iBuffer += FormList[iForm].fillInfo.texture.count;
				}
			}
			for (iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
				if (istx(iForm)) {
					auto startSource = TexturePointsBuffer->cbegin() + FormList[iForm].fillInfo.texture.index;
					auto endSource   = startSource + FormList[iForm].fillInfo.texture.count;
					textureBuffer.resize(textureBuffer.size() + FormList[iForm].fillInfo.texture.count);
					auto destination                       = textureBuffer.begin() + iBuffer;
					auto _                                 = std::copy(startSource, endSource, destination);
					FormList[iForm].fillInfo.texture.index = iBuffer;
					iBuffer += FormList[iForm].fillInfo.texture.count;
				}
			}
			TextureIndex         = iBuffer;
			*TexturePointsBuffer = textureBuffer;
		}
		FormList[ClosestFormToCursor].fillType = 0;
	}
}

void fethrf() {
	if (FormIndex) {
		fvars(ClosestFormToCursor);
		delclps(ClosestFormToCursor);
		deltx();
		SelectedForm->type                           = SAT;
		SelectedForm->fillInfo.feather.ratio         = IniFile.featherRatio;
		SelectedForm->fillInfo.feather.upCount       = IniFile.featherUpCount;
		SelectedForm->fillInfo.feather.downCount     = IniFile.featherDownCount;
		SelectedForm->fillInfo.feather.fillType      = IniFile.featherFillType;
		SelectedForm->fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
		SelectedForm->extendedAttribute &= ~(AT_FTHUP | AT_FTHBTH | AT_FTHBLND);
		SelectedForm->extendedAttribute |= IniFile.featherType;
		SelectedForm->fillInfo.feather.count     = IniFile.featherCount;
		SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
		SelectedForm->fillSpacing                = LineSpacing;
		SelectedForm->fillColor                  = ActiveColor;
		SelectedForm->fillInfo.feather.color     = (ActiveColor + 1) & COLMSK;
		SelectedForm->fillType                   = FTHF;
		refilfn();
	}
}

void fethr() {
	if (filmsgs(FMM_FTH))
		return;
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fethrf();
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fethrf();
		}
	}
	StateMap.set(StateFlag::INIT);
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

constexpr ULARGE_INTEGER tim2int(FILETIME time) noexcept {
	ULARGE_INTEGER op = { { 0, 0 } };

	op.LowPart  = time.dwLowDateTime;
	op.HighPart = time.dwHighDateTime;
	return op;
}

int fil2crd(const fs::path& fileName) {
	STARTUPINFO         startupInfo = {};
	PROCESS_INFORMATION processInfo = {};
	int                 errorCode   = 0;
	wchar_t             command[_MAX_PATH * 2 + 1 + 4];

	wcscpy_s(command, L"\"");
	wcscpy_s(command, win32::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str());
	wcscpy_s(command, L"\" \"");
	wcscpy_s(command, fileName.wstring().c_str());
	wcscpy_s(command, L"\"");
	startupInfo    = {};
	startupInfo.cb = sizeof(startupInfo);
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

bool chkp2cnam(const wchar_t* fileName) noexcept {
	HANDLE handleP2C;

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

	HKEY          registryKey = {};
	unsigned long keyType = 0, size = 0;
	wchar_t       programName[_MAX_PATH] = { 0 };
	const wchar_t filter[]               = L"ComputerService (Lind2PC.exe)\0LinkP2C.exe\0\0";
	wchar_t       message[P2CBUFSIZ]     = { 0 };
	wchar_t       caption[P2CBUFSIZ]     = { 0 };
	OPENFILENAME  openFileName           = {
        sizeof(openFileName), // lStructsize
        ThrEdWindow,          // hwndOwner
        ThrEdInstance,        // hInstance
        filter,               // lpstrFilter
        0,                    // lpstrCustomFilter
        0,                    // nMaxCustFilter
        0,                    // nFilterIndex
        programName,          // lpstrFile
        _MAX_PATH,            // nMaxFile
        0,                    // lpstrFileTitle
        0,                    // nMaxFileTitle
        L"C:\\",              // lpstr	ialDir
        0,                    // lpstrTitle
        0,                    // Flags
        0,                    // nFileOffset
        0,                    // nFileExtension
        L"exe",               // lpstrDefExt
        0,                    // lCustData
        0,                    // lpfnHook
        0,                    // lpTemplateName
	};

	if (PCSHeader.stitchCount)
		save();
	else {
		tabmsg(IDS_P2CNODAT);
		return;
	}
	if (chkp2cnam(win32::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str())) {
		fil2crd(*ThrName);
		return;
	}
	*IniFile.p2cName = 0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 0, KEY_READ, &registryKey)
	    == ERROR_SUCCESS) {
		size    = _MAX_PATH;
		keyType = REG_SZ;
		if (RegQueryValueEx(registryKey, L"ProgramFilesDir", 0, &keyType, (unsigned char*)programName, &size) == ERROR_SUCCESS) {
			wcscat_s(programName, L"\\Computerservice SSHSBV\\PES2Card\\LinkP2C.exe");
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
	auto p2cName = win32::Utf16ToUtf8(std::wstring(programName));
	std::copy(p2cName.begin(), p2cName.end(), IniFile.p2cName);
	fil2crd(*AuxName);
}

std::vector<fPOINT>& insid() {
	unsigned iVertex = 0;

	satout(fabs(SelectedForm->underlayIndent));
	if (SelectedForm->underlayIndent > 0) {
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (!cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y)) {
				(*InsidePoints)[iVertex] = CurrentFormVertices[iVertex];
			}
		}
		return *InsidePoints;
	}
	else
		return *OutsidePoints;
}

void delwlk(size_t code) {
	unsigned iStitch = 0, stitchCount = 0;

	if (PCSHeader.stitchCount) {
		stitchCount = 0;
		std::vector<fPOINTATTR> highStitchBuffer;
		highStitchBuffer.reserve(PCSHeader.stitchCount);
		for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & WLKFMSK) != code) {
				highStitchBuffer.push_back(StitchBuffer[iStitch]);
			}
		}
		if (highStitchBuffer.size() != PCSHeader.stitchCount) {
			std::copy(highStitchBuffer.cbegin(),
			          highStitchBuffer.cend(),
			          stdext::make_checked_array_iterator(StitchBuffer, PCSHeader.stitchCount));
			PCSHeader.stitchCount = gsl::narrow<unsigned short>(highStitchBuffer.size());
		}
	}
}

void chkuseq(const unsigned interleaveSequenceIndex2) noexcept {
#if BUGBAK
	unsigned index;

	for (index = 0; index < OutputIndex; index++) {
		InterleaveSequence[index] = OSequence[index];
	}
	InterleaveSequenceIndex                                   = index;
	InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->UnderlayColor;
#else

	unsigned iSequence = 0, index = 0, stitchCount = 0;
	double   length               = 0.0;
	fPOINT   delta                = {};
	fPOINT   point                = {};
	fPOINT   step                 = {};
	double   underlayStitchLength = 0.0;

	if (OutputIndex) {
		if (SelectedForm->underlayStitchLen < MINWLK)
			SelectedForm->underlayStitchLen = MINWLK;
		if (SelectedForm->underlayStitchLen > MAXWLK)
			SelectedForm->underlayStitchLen = MAXWLK;
		underlayStitchLength = SelectedForm->underlayStitchLen;
		for (iSequence = 0; iSequence < OutputIndex - 1; iSequence++) {
			delta.x     = OSequence[iSequence + 1].x - OSequence[iSequence].x;
			delta.y     = OSequence[iSequence + 1].y - OSequence[iSequence].y;
			length      = hypot(delta.x, delta.y);
			stitchCount = floor(length / underlayStitchLength);
			if (stitchCount) {
				step.x = delta.x / stitchCount;
				step.y = delta.y / stitchCount;
				point  = OSequence[iSequence];
				for (index = 0; index < stitchCount; index++) {
					InterleaveSequence[InterleaveSequenceIndex] = point;
					InterleaveSequenceIndex++;
					point.x += step.x;
					point.y += step.y;
				}
			}
			else {
				InterleaveSequence[InterleaveSequenceIndex] = OSequence[iSequence];
				InterleaveSequenceIndex++;
			}
		}
		InterleaveSequence[InterleaveSequenceIndex] = OSequence[iSequence];
		InterleaveSequenceIndex++;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->underlayColor;
	}
#endif
}

void ritwlk(unsigned& interleaveSequenceIndex2) noexcept {
	if (OutputIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = WLKMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkuseq(interleaveSequenceIndex2);
		interleaveSequenceIndex2++;
	}
}

void ritcwlk(unsigned& interleaveSequenceIndex2) noexcept {
	if (OutputIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = CWLKMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkuseq(interleaveSequenceIndex2);
		interleaveSequenceIndex2++;
	}
}

unsigned gucon(const fPOINT& start, const fPOINT& finish, unsigned destination, unsigned code) {
	double       length             = hypot(finish.x - start.x, finish.y - start.y);
	size_t       startVertex        = closflt(start.x, start.y);
	const size_t endVertex          = closflt(finish.x, finish.y);
	unsigned     stitchCount        = 0;
	size_t       intermediateVertex = 0;
	unsigned     iStitch = 0, iStep = 0;
	size_t       up = 0, down = 0;
	fPOINT       localPoint = {}, step = {}, delta = {};

	if (length < 5)
		return 0;
	if (startVertex == endVertex)
		return 0;
	const std::vector<fPOINT>& indentedPoint = insid();
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
		up   = nxt(up);
		down = prv(down);
	} while (true);
	iStitch = destination;
	while (startVertex != endVertex) {
		StitchBuffer[iStitch].x         = indentedPoint[startVertex].x;
		StitchBuffer[iStitch].y         = indentedPoint[startVertex].y;
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
		delta.x     = indentedPoint[intermediateVertex].x - indentedPoint[startVertex].x;
		delta.y     = indentedPoint[intermediateVertex].y - indentedPoint[startVertex].y;
		length      = hypot(delta.x, delta.y);
		stitchCount = length / SelectedForm->lengthOrCount.stitchLength;
		if (stitchCount > 1) {
			step.x       = delta.x / stitchCount;
			step.y       = delta.y / stitchCount;
			localPoint.x = indentedPoint[startVertex].x + step.x;
			localPoint.y = indentedPoint[startVertex].y + step.y;
			for (iStep = 0; iStep < stitchCount - 1; iStep++) {
				StitchBuffer[iStitch].x         = localPoint.x;
				StitchBuffer[iStitch].y         = localPoint.y;
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
	StitchBuffer[iStitch].x         = indentedPoint[startVertex].x;
	StitchBuffer[iStitch].y         = indentedPoint[startVertex].y;
	StitchBuffer[iStitch].attribute = code;
	iStitch++;
	return iStitch - destination;
}

void fnwlk(size_t find, unsigned& interleaveSequenceIndex2) {
	size_t start = 0;
	size_t count = 0;

	fvars(find);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (SelectedForm->extendedAttribute & AT_STRT && SelectedForm->type != FRMLINE)
		start = SelectedForm->fillStart;
	else
		start = 0;
	count = VertexCount;
	if (SelectedForm->type != FRMLINE)
		count++;
	const std::vector<fPOINT>& walkPoints = insid();
	OutputIndex                           = 0;
	while (count) {
		OSequence[OutputIndex] = walkPoints[start];
		start                  = nxt(start);
		OutputIndex++;
		count--;
	}
	ritwlk(interleaveSequenceIndex2);
}

void ritund(unsigned& interleaveSequenceIndex2) noexcept {
	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = UNDMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkuseq(interleaveSequenceIndex2);
		interleaveSequenceIndex2++;
	}
}

void undclp() noexcept {
	ClipRectSize.cx = ClipRect.bottom = ClipRect.left = ClipRect.right = ClipBuffer[0].x = ClipBuffer[1].x = ClipBuffer[0].y = 0;
	ClipRectSize.cy = ClipRect.top = ClipBuffer[1].y = SelectedForm->underlayStitchLen;
	ClipStitchCount                                  = 2;
}

void fncwlk(unsigned& interleaveSequenceIndex2) {
	size_t iVertex = 0;
	size_t start = 0, finish = 0;

	OutputIndex = 0;
	SelectedForm->extendedAttribute |= AT_CWLK;
	if (SelectedForm->satinGuideCount) {
		if (SelectedForm->wordParam) {
			iVertex                  = SelectedForm->wordParam;
			OSequence[OutputIndex].x = midl(CurrentFormVertices[iVertex].x, CurrentFormVertices[iVertex + 1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[iVertex].y, CurrentFormVertices[iVertex + 1].y);
			OutputIndex++;
		}
		const SATCON* guide = SelectedForm->satinOrAngle.guide;
		if (guide) {
			for (auto iGuide = SelectedForm->satinGuideCount; iGuide != 0; iGuide--) {
				OSequence[OutputIndex].x
				    = midl(CurrentFormVertices[guide[iGuide - 1].finish].x, CurrentFormVertices[guide[iGuide - 1].start].x);
				OSequence[OutputIndex].y
				    = midl(CurrentFormVertices[guide[iGuide - 1].finish].y, CurrentFormVertices[guide[iGuide - 1].start].y);
				OutputIndex++;
			}
		}
		if (SelectedForm->attribute & FRMEND) {
			OSequence[OutputIndex].x = midl(CurrentFormVertices[0].x, CurrentFormVertices[1].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[0].y, CurrentFormVertices[1].y);
			OutputIndex++;
		}
	}
	else {
		if (SelectedForm->extendedAttribute & AT_STRT)
			start = SelectedForm->fillStart;
		else
			start = 0;
		OSequence[OutputIndex] = CurrentFormVertices[start];
		OutputIndex++;
		finish = prv(start);
		start  = nxt(start);
		for (size_t iGuide = 1; iGuide<VertexCount>> 1; iGuide++) {
			OSequence[OutputIndex].x = midl(CurrentFormVertices[finish].x, CurrentFormVertices[start].x);
			OSequence[OutputIndex].y = midl(CurrentFormVertices[finish].y, CurrentFormVertices[start].y);
			if (cisin(OSequence[OutputIndex].x, OSequence[OutputIndex].y))
				OutputIndex++;
			start  = nxt(start);
			finish = prv(finish);
		}
		OSequence[OutputIndex] = CurrentFormVertices[start];
		OutputIndex++;
	}
	ritcwlk(interleaveSequenceIndex2);
}

void srtcol() {
	const int             colorSize = 16;
	std::vector<unsigned> histogram(colorSize);
	std::vector<unsigned> colorStartStitch(colorSize);
	unsigned              iStitch = 0, startStitch = 0;

	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		histogram[StitchBuffer[iStitch].attribute & COLMSK]++;
	startStitch = 0;
	auto it     = histogram.begin();
	for (auto& stitchColor : colorStartStitch) {
		stitchColor = startStitch;
		startStitch += *it;
		const auto _ = it++;
	}
	std::vector<fPOINTATTR> highStitchBuffer(PCSHeader.stitchCount);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		highStitchBuffer[colorStartStitch[StitchBuffer[iStitch].attribute & COLMSK]++] = StitchBuffer[iStitch];
	}
	std::copy(highStitchBuffer.cbegin(), highStitchBuffer.cend(), StitchBuffer);
}

void dubit(unsigned bit) {
	unsigned code = 0;

	savdo();
	StateMap.set(StateFlag::WASDO);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (!(SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) && bit & (AT_UND | AT_WALK | AT_CWLK)) {
		if (SelectedForm->fillType)
			SelectedForm->underlayColor = SelectedForm->fillColor;
		else
			SelectedForm->underlayColor = ActiveColor;
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	}
	if (!(SelectedForm->extendedAttribute & AT_UND) && bit & AT_UND) {
		SelectedForm->underlayStitchAngle = IniFile.underlayAngle;
		SelectedForm->underlaySpacing     = IniFile.underlaySpacing;
	}
	code = SelectedForm->extendedAttribute & bit;
	if (code)
		SelectedForm->extendedAttribute &= ~(bit);
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

void chkcwlk(unsigned& interleaveSequenceIndex2) {
	if (SelectedForm->extendedAttribute & AT_CWLK)
		fncwlk(interleaveSequenceIndex2);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
}

void chkwlk(unsigned& interleaveSequenceIndex2) {
	if (SelectedForm->extendedAttribute & AT_WALK)
		fnwlk(ClosestFormToCursor, interleaveSequenceIndex2);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
}

void fnund(const std::vector<RNGCNT>& textureSegments, size_t find, unsigned& interleaveSequenceIndex2) {
	const float savedStitchSize = UserStitchLength;

	UserStitchLength = 1e99;
	if (!SelectedForm->underlaySpacing)
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	if (!SelectedForm->underlayStitchLen)
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	undclp();
	StateMap.set(StateFlag::ISUND);
	angclpfn(textureSegments);
	OutputIndex = SequenceIndex;
	ritund(interleaveSequenceIndex2);
	fvars(find);
	UserStitchLength = savedStitchSize;
}

void chkund(const std::vector<RNGCNT>& textureSegments, unsigned& interleaveSequenceIndex2) {
	if (SelectedForm->extendedAttribute & AT_UND)
		fnund(textureSegments, ClosestFormToCursor, interleaveSequenceIndex2);
	else
		delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
}

void selalfrm() {
	SelectedFormList->reserve(FormIndex);
	for (auto formIndex = 0u; formIndex < FormIndex; formIndex++)
		SelectedFormList->push_back(formIndex);
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
	IniFile.daisyPetalPoints  = DAZCNT;
	IniFile.daisyHoleDiameter = DAZHLEN;
	IniFile.daisyInnerCount   = DAZICNT;
	IniFile.daisyDiameter     = DAZLEN;
	IniFile.daisyPetalCount   = DAZPETS;
	IniFile.daisyPetalLen     = DAZPLEN;
	IniFile.daisyHeartCount   = DAZMCNT;
	UserFlagMap.set(UserFlag::DAZHOL);
	UserFlagMap.set(UserFlag::DAZD);
	IniFile.daisyBorderType = DAZTYP;
}

void initdaz(HWND hWinDialog) {
	unsigned iType = 0;

	chkdaz();
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS), fmt::format(L"{}", IniFile.daisyPetalPoints).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT), fmt::format(L"{}", IniFile.daisyHeartCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN), fmt::format(L"{:.2f}", IniFile.daisyDiameter).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ), fmt::format(L"{:.2f}", IniFile.daisyHoleDiameter).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS), fmt::format(L"{}", IniFile.daisyInnerCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS), fmt::format(L"{}", IniFile.daisyPetalCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN), fmt::format(L"{:.2f}", IniFile.daisyPetalLen).c_str());
	CheckDlgButton(hWinDialog, IDC_HOLE, UserFlagMap.test(UserFlag::DAZHOL));
	CheckDlgButton(hWinDialog, IDC_DLIN, UserFlagMap.test(UserFlag::DAZD));
	std::wstring daisyType;
	for (iType = 0; iType < 6; iType++) {
		loadString(daisyType, DaisyTypeStrings[iType]);
		[[gsl::suppress(type .1)]] SendMessage(
		    GetDlgItem(hWinDialog, IDC_DAZTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(daisyType.c_str()));
	}
	SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
}

BOOL CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	wchar_t  buffer[HBUFSIZ]        = { 0 };
	wchar_t  compareBuffer[HBUFSIZ] = { 0 };
	unsigned iType                  = 0;

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
			IniFile.daisyPetalPoints = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buffer, HBUFSIZ);
			IniFile.daisyHeartCount = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buffer, HBUFSIZ);
			IniFile.daisyDiameter = std::stof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buffer, HBUFSIZ);
			IniFile.daisyHoleDiameter = std::stof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buffer, HBUFSIZ);
			IniFile.daisyInnerCount = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buffer, HBUFSIZ);
			IniFile.daisyPetalCount = std::stoi(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buffer, HBUFSIZ);
			IniFile.daisyPetalLen = std::stof(buffer);
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
				if (!StrCmpW(buffer, compareBuffer)) {
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
	double   holeSegmentAngle = 0.0, petalSegmentAngle = 0.0;
	double   angle           = 0.0;
	double   deltaPetalAngle = 0.0;
	double   petalPointAngle = 0.0;
	unsigned holeVertexCount = 0, petalVertexCount = 0, iSegment = 0, iMacroPetal = 0, iPoint = 0;
	unsigned iVertex = 0, borderType = 0, sawPointCount = 0, halfPetalPointCount = 0, fref = 0, petalPointCount = 0;
	double   distanceFromDaisyCenter = 0.0;
	double   diameter                = 0.0;
	double   petalLength             = 0.0;
	double   holeDiameter            = 0.0;
	double   ratio                   = 0.0;
	float    maximumXsize = 0.0, maximumYsize = 0.0;
	fPOINT   referencePoint = {};

	unmsg();
	if (!DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, (DLGPROC)dasyproc)) {
		StateMap.reset(StateFlag::FORMIN);
		return;
	}
	IniFile.daisyPetalCount = IniFile.daisyPetalCount;
	diameter                = IniFile.daisyDiameter;
	petalLength             = IniFile.daisyPetalLen;
	holeDiameter            = IniFile.daisyHoleDiameter;
	borderType              = IniFile.daisyBorderType;
	referencePoint.x        = midl(ZoomRect.right, ZoomRect.left);
	referencePoint.y        = midl(ZoomRect.top, ZoomRect.bottom);
	SelectedForm            = &FormList[FormIndex];
	ClosestFormToCursor     = FormIndex;
	frmclr(SelectedForm);
	SelectedForm->vertices  = &FormVertices[FormVertexIndex];
	SelectedForm->attribute = ActiveLayer << 1;
	fvars(FormIndex);
	halfPetalPointCount = IniFile.daisyPetalPoints >> 1;
	maximumXsize        = ZoomRect.right - ZoomRect.left;
	maximumYsize        = ZoomRect.top - ZoomRect.bottom;
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
		angle                          = PI2;
		holeVertexCount                = IniFile.daisyPetalCount * IniFile.daisyInnerCount;
		holeSegmentAngle               = PI2 / holeVertexCount;
		CurrentFormVertices[iVertex].x = referencePoint.x + diameter * cos(angle);
		CurrentFormVertices[iVertex].y = referencePoint.y + diameter * sin(angle);
		iVertex++;
		for (iSegment = 0; iSegment < holeVertexCount + 1; iSegment++) {
			CurrentFormVertices[iVertex].x = referencePoint.x + holeDiameter * cos(angle);
			CurrentFormVertices[iVertex].y = referencePoint.y + holeDiameter * sin(angle);
			iVertex++;
			angle -= holeSegmentAngle;
		}
		fref = iVertex;
	}
	angle            = 0;
	petalVertexCount = IniFile.daisyPetalCount * IniFile.daisyPetalPoints;
	petalPointCount  = IniFile.daisyPetalPoints;
	if (borderType == DHART) {
		petalPointCount  = (IniFile.daisyHeartCount + 1) << 1;
		petalVertexCount = IniFile.daisyPetalCount * petalPointCount;
	}
	petalSegmentAngle = PI2 / petalVertexCount;
	deltaPetalAngle   = PI / IniFile.daisyPetalPoints;
	if (UserFlagMap.test(UserFlag::DAZD)) {
		SelectedForm->satinGuideCount    = IniFile.daisyPetalCount - 1;
		SelectedForm->wordParam          = IniFile.daisyPetalCount * IniFile.daisyInnerCount + 1;
		SelectedForm->satinOrAngle.guide = adsatk(IniFile.daisyPetalCount - 1);
	}
	for (iMacroPetal = 0; iMacroPetal < IniFile.daisyPetalCount; iMacroPetal++) {
		petalPointAngle   = 0;
		PseudoRandomValue = SEED;
		for (iPoint = 0; iPoint < petalPointCount; iPoint++) {
			switch (borderType) {
			case DSIN:
				distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
				petalPointAngle += deltaPetalAngle;
				break;
			case DRAMP:
				distanceFromDaisyCenter = diameter + (static_cast<double>(iPoint) / IniFile.daisyPetalPoints * petalLength);
				break;
			case DSAW:
				if (iPoint > halfPetalPointCount)
					sawPointCount = IniFile.daisyPetalPoints - iPoint;
				else
					sawPointCount = iPoint;
				distanceFromDaisyCenter
				    = diameter + (static_cast<double>(sawPointCount) / IniFile.daisyPetalPoints * petalLength);
				break;
			case DRAG:
				distanceFromDaisyCenter
				    = diameter + (static_cast<double>(psg() % IniFile.daisyPetalPoints) / IniFile.daisyPetalPoints * petalLength);
				break;
			case DCOG:
				distanceFromDaisyCenter = diameter;
				if (iPoint > halfPetalPointCount)
					distanceFromDaisyCenter += petalLength;
				break;
			case DHART:
				distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
				if (iPoint > IniFile.daisyHeartCount)
					petalPointAngle -= deltaPetalAngle;
				else
					petalPointAngle += deltaPetalAngle;
				break;
			}
			CurrentFormVertices[iVertex].x = referencePoint.x + cos(angle) * distanceFromDaisyCenter;
			CurrentFormVertices[iVertex].y = referencePoint.y + sin(angle) * distanceFromDaisyCenter;
			iVertex++;
			angle += petalSegmentAngle;
			if (UserFlagMap.test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
				SelectedForm->satinOrAngle.guide[iMacroPetal].start
				    = (IniFile.daisyPetalCount - iMacroPetal - 1) * IniFile.daisyInnerCount + 1;
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
		SelectedForm->type      = SAT;
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
	NewFormVertexCount                = iVertex + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	mdufrm();
}

unsigned dutyp(unsigned attribute) noexcept {
	char           result          = 0;
	DWORD          bit             = 0;
	const unsigned maskedAttribute = attribute & SRTYPMSK;

	_BitScanReverse(&bit, maskedAttribute);

	if (bit == 0)
		return 0;

	result = ((bit & 0xff) - 18);

	if ((result != 12) || ((maskedAttribute & TYPATMSK) == 0))
		return result & 0xf;

	return 1;
}

void durec(OREC& record) noexcept {
	unsigned          attribute = 0;
	const fPOINTATTR* stitch    = &StitchBuffer[record.start];

	record.type  = StitchTypes[dutyp(stitch->attribute)];
	attribute    = stitch->attribute & SRTMSK;
	record.color = attribute & 0xf;
	record.form  = (attribute & FRMSK) >> FRMSHFT;
}

bool recmp(const OREC* record1, const OREC* record2) noexcept {
	if (ColorOrder[record1->color] == ColorOrder[record2->color]) {
		if (record1->form == record2->form) {
			if (record1->type == record2->type)
				return (record1->start < record2->start);
			else
				return (record1->type < record2->type);
		}
		else {
			return (record1->form < record2->form);
		}
	}
	else {
		return (ColorOrder[record1->color] < ColorOrder[record2->color]);
	}
}

bool refcmp(const OREC* record1, const OREC* record2) noexcept {
	if (record1->form == record2->form) {
		return (record1->type < record2->type);
	}
	else {
		return (record1->form < record2->form);
	}
}

bool chkrdun(const std::vector<unsigned>& formFillCounter, const std::vector<OREC*>& pRecs, const SRTREC& stitchRecord) {
	unsigned iStitch;

	for (iStitch = stitchRecord.start; iStitch < stitchRecord.finish; iStitch++) {
		if (pRecs[iStitch]->otyp == formFillCounter[pRecs[iStitch]->form])
			return 1;
	}
	return 0;
}

double precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord) {
	unsigned    iRegion       = 0;
	unsigned    currentRegion = sortRecord.currentRegion;
	fPOINTATTR* currentStitch = nullptr;
	bool        direction     = sortRecord.direction;

	std::vector<unsigned> formFillCounter((FormIndex + 2) << 2);
	unsigned              totalJumps = 0;
	while (chkrdun(formFillCounter, pRecs, sortRecord)) {
		double minimumLength = 1e9;
		if (direction)
			currentStitch = &StitchBuffer[pRecs[currentRegion]->finish];
		else
			currentStitch = &StitchBuffer[pRecs[currentRegion]->start];
		for (iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
			if (pRecs[iRegion]->otyp == formFillCounter[pRecs[iRegion]->form]) {
				double length
				    = hypot(pRecs[iRegion]->startStitch->x - currentStitch->x, pRecs[iRegion]->startStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction     = 0;
					currentRegion = iRegion;
				}
				length = hypot(pRecs[iRegion]->endStitch->x - currentStitch->x, pRecs[iRegion]->endStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction     = 1;
					currentRegion = iRegion;
				}
			}
		}
		if (minimumLength > 9 * PFGRAN)
			totalJumps++;
		formFillCounter[pRecs[currentRegion]->form]++;
		if (StateMap.test(StateFlag::DUSRT)) {
			if (direction) {
				if (pRecs[currentRegion]->start) {
					for (iRegion = pRecs[currentRegion]->finish - 1; iRegion >= pRecs[currentRegion]->start; iRegion--) {
						tempStitchBuffer[OutputIndex++] = StitchBuffer[iRegion];
					}
				}
				else {
					iRegion = pRecs[currentRegion]->finish;
					while (iRegion) {
						tempStitchBuffer[OutputIndex++] = StitchBuffer[--iRegion];
					}
				}
			}
			else {
				for (iRegion = pRecs[currentRegion]->start; iRegion < pRecs[currentRegion]->finish; iRegion++) {
					tempStitchBuffer[OutputIndex++] = StitchBuffer[iRegion];
				}
			}
		}
	}
	return totalJumps;
}

unsigned duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord) {
	sortRecord.direction  = 0;
	const unsigned jumps0 = precjmps(tempStitchBuffer, pRecs, sortRecord);

	sortRecord.direction  = 1;
	const unsigned jumps1 = precjmps(tempStitchBuffer, pRecs, sortRecord);

	if (jumps0 < jumps1) {
		sortRecord.direction = 0;
		return jumps0;
	}
	sortRecord.direction = 1;
	return jumps1;
}

#ifdef _DEBUG

void dmprec(const std::vector<OREC*>& stitchRegion, unsigned count) {
	unsigned iRegion;

	for (iRegion = 0; iRegion < count; iRegion++) {
		OutputDebugString(fmt::format(L"{:4d} attrb: 0x{:08x} form: {:4d} type: {} color: {:2d} start: {:5d} finish: {:5d}\n",
		                              iRegion,
		                              StitchBuffer[stitchRegion[iRegion]->start].attribute,
		                              stitchRegion[iRegion]->form,
		                              stitchRegion[iRegion]->type,
		                              stitchRegion[iRegion]->color,
		                              stitchRegion[iRegion]->start,
		                              stitchRegion[iRegion]->finish)
		                      .c_str());
	}
}
#endif

bool srtchk(const std::vector<OREC*>& stitchRegion, unsigned count, unsigned& badForm) {
	unsigned iRegion    = 1;
	unsigned form       = stitchRegion[0]->form;
	unsigned color      = stitchRegion[0]->color;
	FRMHED*  formHeader = nullptr;

	for (iRegion = 1; iRegion < count; iRegion++) {
		if (stitchRegion[iRegion]->form == form) {
			if (ColorOrder[stitchRegion[iRegion]->color] < ColorOrder[color]) {
				formHeader = &FormList[form];
				if (formHeader->fillType == FTHF && formHeader->extendedAttribute & AT_FTHBLND
				    && stitchRegion[iRegion]->color == formHeader->fillColor)
					continue;
				badForm = iRegion;
				return false;
			}
			else
				color = stitchRegion[iRegion]->color;
		}
		else {
			color = stitchRegion[iRegion]->color;
			form  = stitchRegion[iRegion]->form;
		}
	}
	return true;
}

void fsort() {
	unsigned attribute = StitchBuffer->attribute & SRTMSK;
	unsigned iColor = 0, iStitch = 0, badForm = 0, iRange = 0, lastRange = 0;
	unsigned currentForm = 0, iRegion = 0, lastRegion = 0;
	unsigned minimumIndex = 0, minimumDirection = 0;
	unsigned typeCount = 0, jumps = 0, minimumJumps = 0;
	// There cannot be more records than stitches
	std::vector<OREC> stitchRegion(PCSHeader.stitchCount);
	SRTREC            sortRecord = {};
	FILETIME          fileTime   = {};
	ULARGE_INTEGER    startTime  = {};
	ULARGE_INTEGER    nextTime   = {};

	// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors
	//        in a single form are not in ascending order already.
	savdo();
	stitchRegion[0].start       = 0;
	stitchRegion[0].startStitch = StitchBuffer;
	ColorOrder[AppliqueColor]   = 0;
	for (iColor = 0; iColor < 16; iColor++) {
		if (iColor != AppliqueColor)
			ColorOrder[iColor] = iColor + 1;
	}
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & SRTMSK) != attribute) {
			stitchRegion[iRegion].finish    = iStitch;
			stitchRegion[iRegion].endStitch = &StitchBuffer[iStitch - 1];
			iRegion++;
			stitchRegion[iRegion].start       = iStitch;
			stitchRegion[iRegion].startStitch = &StitchBuffer[iStitch];
			attribute                         = StitchBuffer[iStitch].attribute & SRTMSK;
		}
	}
	stitchRegion[iRegion].endStitch = &StitchBuffer[PCSHeader.stitchCount - 1];
	stitchRegion[iRegion].finish    = PCSHeader.stitchCount;
	iRegion++;
	lastRegion = iRegion;
	std::vector<OREC*> pRecs(lastRegion);
	std::vector<OREC*> pFRecs(lastRegion);
	for (iRegion = 0; iRegion < lastRegion; iRegion++) {
		durec(stitchRegion[iRegion]);
		pRecs[iRegion]  = &stitchRegion[iRegion];
		pFRecs[iRegion] = &stitchRegion[iRegion];
	}
	std::sort(pRecs.begin(), pRecs.end(), recmp);
	std::sort(pFRecs.begin(), pFRecs.end(), refcmp);
#ifdef _DEBUG
	dmprec(pRecs, lastRegion);
#endif
	if (srtchk(pFRecs, lastRegion, badForm)) {
		std::vector<RANGE> stitchRange(lastRegion);
		stitchRange[0].start = 0;
		attribute            = pRecs[0]->color;
		currentForm          = 0xffffffff;
		typeCount            = 0;
		iRange               = 0;
		for (iRegion = 0; iRegion < lastRegion; iRegion++) {
			bool srtskp = true;
			if (attribute != pRecs[iRegion]->color) {
				stitchRange[iRange++].finish = iRegion;
				stitchRange[iRange].start    = iRegion;
				attribute                    = pRecs[iRegion]->color;
				currentForm                  = pRecs[iRegion]->form;
				typeCount                    = 0;
				srtskp                       = false;
			}
			if (srtskp) {
				if (pRecs[iRegion]->form == currentForm)
					typeCount++;
				else {
					typeCount   = 0;
					currentForm = pRecs[iRegion]->form;
				}
			}
			pRecs[iRegion]->otyp = typeCount;
		}
		stitchRange[iRange].finish = lastRegion;
		lastRange                  = ++iRange;
		std::vector<fPOINTATTR> tempStitchBuffer(PCSHeader.stitchCount);
		OutputIndex = 0;
		for (iRange = 0; iRange < lastRange; iRange++) {
			StateMap.reset(StateFlag::DUSRT);
			sortRecord.start  = stitchRange[iRange].start;
			sortRecord.finish = stitchRange[iRange].finish;
			sortRecord.count  = sortRecord.finish - sortRecord.start;
			minimumJumps      = 0xffffffff;
			// timeout used to put an upper bound on the number of sorting permutations checked
			GetSystemTimeAsFileTime(&fileTime);
			startTime = tim2int(fileTime);
			for (iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
				sortRecord.currentRegion = iRegion;
				if (!pRecs[iRegion]->otyp) {
					jumps = duprecs(tempStitchBuffer, pRecs, sortRecord);
					if (jumps < minimumJumps) {
						minimumJumps     = jumps;
						minimumIndex     = iRegion;
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
			sortRecord.direction     = minimumDirection;
			precjmps(tempStitchBuffer, pRecs, sortRecord);
		}
		std::copy(tempStitchBuffer.cbegin(), tempStitchBuffer.cbegin() + OutputIndex, StitchBuffer);
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(OutputIndex);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		std::wstring str;
		loadString(str, IDS_SRTER);
		shoMsg(fmt::format(str, pFRecs[badForm]->form));
	}
}

#ifdef _DEBUG

typedef struct _atfld {
	unsigned color;
	unsigned form;
	unsigned type;
	unsigned layer;
	unsigned user;
} ATFLD;

void duatf(unsigned ind) {
	const unsigned attribute       = StitchBuffer[ind].attribute;
	ATFLD          attributeFields = { (attribute & COLMSK),
                              ((attribute & FRMSK) >> FRMSHFT),
                              gsl::narrow<unsigned>(StitchTypes[dutyp(attribute)]),
                              ((attribute >> LAYSHFT) & 7),
                              0 };

	if (attribute & 0x80000000)
		attributeFields.user = 1;
	else
		attributeFields.user = 0;
	OutputDebugString(fmt::format(L"{:5d}: color: {:2d} form: {:5d} type: {:2d}, layer: {:1d}: user: {:1d} attrib: 0x{:08x}\n",
	                              ind,
	                              attributeFields.color,
	                              attributeFields.form,
	                              attributeFields.type,
	                              attributeFields.layer,
	                              attributeFields.user,
	                              attribute)
	                      .c_str());
}

void dmpat() {
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

void fdelstch(FILLSTARTS& fillStartsData, unsigned& fillStartsMap) {
	unsigned iSourceStitch = 0, ind = 0, iDestinationStitch = 0, attribute = 0, type = 0, tmap = 0, color = 0, bordercolor = 0,
	         tapcol       = 0;
	size_t codedFormIndex = 0;
	// ToDo - Still not sure what this function does?
	//        I suspect the fillStartsData members are not correctly named
	codedFormIndex = (ClosestFormToCursor << FRMSHFT);
	bordercolor    = SelectedForm->borderColor & COLMSK;
	tapcol         = SelectedForm->borderColor >> 4;
	for (iSourceStitch = 0; iSourceStitch < PCSHeader.stitchCount; iSourceStitch++) {
		if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX) && iSourceStitch == ClosestPointIndex)
			ClosestPointIndex = iDestinationStitch;
		attribute = StitchBuffer[iSourceStitch].attribute;
		if (codedFormIndex == (attribute & (FRMSK | NOTFRM))) {
			type = StitchTypes[dutyp(attribute)];
			switch (type) {
			case TYPE_APPLIQUE:
				if (!(tmap & M_AP)) {
					tmap |= M_AP;
					fillStartsData.fillNamed.applique = iDestinationStitch;
				}
				break;
			case TYPE_FTHR:
				if (!(tmap & M_FTH)) {
					tmap |= M_FTH;
					fillStartsData.fillNamed.feather = iDestinationStitch;
				}
				break;
			case TYPE_FILL:
				if (!(tmap & M_FIL)) {
					tmap |= M_FIL;
					fillStartsData.fillNamed.fill = iDestinationStitch;
				}
				break;
			case TYPE_BORDER:
				if (!(tmap & M_BRD)) {
					tmap |= M_BRD;
					fillStartsData.fillNamed.border = iDestinationStitch;
				}
				break;
			default:
				if (SelectedForm->fillType && !(tmap & M_FIL)) {
					tmap |= M_FIL;
					fillStartsData.fillNamed.fill = iDestinationStitch;
				}
				break;
			}
		}
		else {
			// ToDo - there is a potential problem here when the active color is 0
			//        because the default color is 0 as well. Should the default color
			//        be e.g. -1 ?
			color = attribute & COLMSK;
			if (color == SelectedForm->fillColor) {
				tmap |= M_FCOL;
				fillStartsData.fillNamed.fillColor = iDestinationStitch;
			}
			if (color == SelectedForm->fillInfo.feather.color) {
				tmap |= M_FTHCOL;
				fillStartsData.fillNamed.featherColor = iDestinationStitch;
			}
			if (color == bordercolor) {
				tmap |= M_ECOL;
				fillStartsData.fillNamed.borderColor = iDestinationStitch;
			}
			if (color == tapcol) {
				tmap |= M_APCOL;
				fillStartsData.fillNamed.appliqueColor = iDestinationStitch;
			}
			StitchBuffer[iDestinationStitch] = StitchBuffer[iSourceStitch];
			iDestinationStitch++;
		}
	}
	fillStartsData.fillNamed.fillColor++;
	fillStartsData.fillNamed.featherColor++;
	fillStartsData.fillNamed.borderColor++;
	fillStartsData.fillNamed.appliqueColor++;
	fillStartsMap         = tmap;
	PCSHeader.stitchCount = iDestinationStitch;
	iDestinationStitch    = 0;
	if (!(tmap & M_ECOL))
		fillStartsData.fillNamed.borderColor = PCSHeader.stitchCount;
	if (!(tmap & M_FTHCOL))
		fillStartsData.fillNamed.featherColor = PCSHeader.stitchCount;
	if (!(tmap & M_FCOL))
		fillStartsData.fillNamed.fillColor = PCSHeader.stitchCount;
	if (SelectedForm->edgeType) {
		if (SelectedForm->edgeType == EDGEAPPL) {
			if (!(tmap & M_AP)) {
				if (tmap & M_APCOL)
					fillStartsData.fillNamed.applique = fillStartsData.fillNamed.appliqueColor + 1;
				else
					fillStartsData.fillNamed.applique = PCSHeader.stitchCount;
			}
		}
		if (!(tmap & M_BRD)) {
			if (tmap & M_ECOL)
				fillStartsData.fillNamed.border = fillStartsData.fillNamed.borderColor + 1;
			else
				fillStartsData.fillNamed.border = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType || (tmap & (M_WALK | M_UND | M_CWLK))) {
		if (!(tmap & M_FIL)) {
			if (tmap & M_FCOL)
				fillStartsData.fillNamed.fill = fillStartsData.fillNamed.fillColor + 1;
			else
				fillStartsData.fillNamed.fill = PCSHeader.stitchCount;
		}
	}
	if (SelectedForm->fillType == FTHF) {
		if (!(tmap & M_FTH)) {
			if (tmap & M_FTHCOL)
				fillStartsData.fillNamed.feather = fillStartsData.fillNamed.featherColor + 1;
			else
				fillStartsData.fillNamed.feather = PCSHeader.stitchCount;
		}
	}
	for (ind = 3; ind; ind--) {
		iDestinationStitch = ind - 1;
		while (iDestinationStitch < ind) {
			if (fillStartsData.fillArray[iDestinationStitch] > fillStartsData.fillArray[ind])
				fillStartsData.fillArray[ind] = fillStartsData.fillArray[iDestinationStitch];
			iDestinationStitch--;
		}
	}
	if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX)) {
		for (ind = 0; ind < (sizeof(fillStartsData) >> 2); ind++)
			fillStartsData.fillArray[ind] = ClosestPointIndex;
	}
}

bool lastcol(unsigned index, fPOINT& point) noexcept {
	unsigned color;

	color = InterleaveSequenceIndices[index].color;
	while (index) {
		index--;
		if (InterleaveSequenceIndices[index].color == color) {
			point = InterleaveSequence[InterleaveSequenceIndices[index + 1].index - 1];
			return 1;
		}
	}
	return 0;
}

void duint(unsigned offset, unsigned code, INTINF& ilData) {
	unsigned count = 0, iSequence = 0;
	fPOINT   point = {};

	if (ilData.coloc > ilData.start) {
		count            = ilData.coloc - ilData.start;
		auto sourceStart = &StitchBuffer[ilData.start];
		auto sourceEnd   = sourceStart + count;

		const auto destination
		    = stdext::make_checked_array_iterator(&ilData.highStitchBuffer[ilData.output], MAXITEMS - ilData.output);
		std::copy(sourceStart, sourceEnd, destination);
		ilData.start += count;
		ilData.output += count;
	}
	if (SelectedForm->extendedAttribute & AT_STRT) {
		if (!StateMap.testAndSet(StateFlag::DIDSTRT))
			ilData.output += gucon(CurrentFormVertices[SelectedForm->fillStart],
			                       InterleaveSequence[InterleaveSequenceIndices[ilData.pins].index],
			                       ilData.output + offset,
			                       code);
	}
	if (lastcol(ilData.pins, point))
		ilData.output
		    += gucon(point, InterleaveSequence[InterleaveSequenceIndices[ilData.pins].index], ilData.output + MAXITEMS, code);
	for (iSequence = InterleaveSequenceIndices[ilData.pins].index; iSequence < InterleaveSequenceIndices[ilData.pins + 1].index;
	     iSequence++) {
		ilData.highStitchBuffer[ilData.output].x         = InterleaveSequence[iSequence].x;
		ilData.highStitchBuffer[ilData.output].y         = InterleaveSequence[iSequence].y;
		ilData.highStitchBuffer[ilData.output].attribute = code;
		if (ilData.highStitchBuffer[ilData.output].x != ilData.highStitchBuffer[ilData.output - 1].x
		    || ilData.highStitchBuffer[ilData.output].y != ilData.highStitchBuffer[ilData.output - 1].y)
			ilData.output++;
	}
}

bool isfil() noexcept {
	if (SelectedForm->fillType)
		return 1;
	if (SelectedForm->edgeType)
		return 1;
	if (SelectedForm->extendedAttribute & (AT_CWLK | AT_WALK | AT_UND))
		return 1;
	return 0;
}

void chkend(unsigned offset, unsigned code, INTINF& ilData) {
	if (isfil()) {
		StateMap.set(StateFlag::ISEND);
		if (SelectedForm->extendedAttribute & AT_END)
			ilData.output += gucon(InterleaveSequence[InterleaveSequenceIndex - 1],
			                       CurrentFormVertices[SelectedForm->fillEnd],
			                       ilData.output + offset,
			                       code);
	}
}

void intlv(const FILLSTARTS& fillStartsData, unsigned fillStartsMap, const unsigned interleaveSequenceIndex2) {
	unsigned ine = 0, code = 0, offset = 0;
	fPOINT   colpnt = {};
	INTINF   ilData = {};

	StateMap.reset(StateFlag::ISEND);
	fvars(ClosestFormToCursor);
	InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
	ilData.layerIndex
	    = (gsl::narrow<size_t>(SelectedForm->attribute & FRMLMSK) << (LAYSHFT - 1)) | (ClosestFormToCursor << FRMSHFT);
	StateMap.reset(StateFlag::DIDSTRT);
	if (PCSHeader.stitchCount) {
		offset = MAXITEMS;
		// Todo - Allocate memory locally for ilData.highStitchBuffer
		ilData.highStitchBuffer = &StitchBuffer[MAXITEMS];
		for (auto iSequence = 0u; iSequence < interleaveSequenceIndex2; iSequence++) {
			ilData.pins = iSequence;
			switch (InterleaveSequenceIndices[iSequence].seq) {
			case I_AP:
				if (fillStartsMap & M_FIL && fillStartsData.fillNamed.applique >= ilData.coloc)
					ilData.coloc = fillStartsData.fillNamed.applique;
				else {
					ilData.coloc = fillStartsData.fillNamed.appliqueColor;
					if (ilData.coloc == 1)
						ilData.coloc = 0;
				}
				break;
			case I_FIL:
				if (fillStartsMap & M_FIL && fillStartsData.fillNamed.fill >= ilData.coloc)
					ilData.coloc = fillStartsData.fillNamed.fill;
				else
					ilData.coloc = fillStartsData.fillNamed.fillColor;
				break;
			case I_FTH:
				if (fillStartsMap & M_FIL && fillStartsData.fillNamed.feather >= ilData.coloc)
					ilData.coloc = fillStartsData.fillNamed.feather;
				else
					ilData.coloc = fillStartsData.fillNamed.featherColor;
				break;
			case I_BRD:
				if (fillStartsMap & M_BRD && fillStartsData.fillNamed.border >= ilData.coloc)
					ilData.coloc = fillStartsData.fillNamed.border;
				else
					ilData.coloc = fillStartsData.fillNamed.borderColor;
				break;
			}
			code = gsl::narrow<unsigned int>(ilData.layerIndex | InterleaveSequenceIndices[ilData.pins].code
			                                 | InterleaveSequenceIndices[ilData.pins].color);
			duint(offset, code, ilData);
		}
		chkend(MAXITEMS, code, ilData);
		if (PCSHeader.stitchCount && ilData.start < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			ine              = PCSHeader.stitchCount - ilData.start;
			auto sourceStart = &StitchBuffer[ilData.start];
			auto sourceEnd   = sourceStart + ine;

			const auto destination
			    = stdext::make_checked_array_iterator(&StitchBuffer[ilData.output + MAXITEMS], MAXITEMS - ilData.output);
			std::copy(sourceStart, sourceEnd, destination);
			ilData.output += ine;
		}
		auto sourceStart = ilData.highStitchBuffer;
		auto sourceEnd   = sourceStart + ilData.output;
		std::copy(sourceStart, sourceEnd, StitchBuffer);
	}
	else {
		offset = 0;
		for (auto iSequence = 0u; iSequence < interleaveSequenceIndex2; iSequence++) {
			code = gsl::narrow<unsigned int>(ilData.layerIndex | InterleaveSequenceIndices[iSequence].code
			                                 | InterleaveSequenceIndices[iSequence].color);
			if (SelectedForm->extendedAttribute & AT_STRT) {
				if (!StateMap.testAndSet(StateFlag::DIDSTRT))
					ilData.output += gucon(CurrentFormVertices[SelectedForm->fillStart],
					                       InterleaveSequence[InterleaveSequenceIndices[ilData.pins].index],
					                       ilData.output + offset,
					                       code);
			}
			if (lastcol(iSequence, colpnt))
				ilData.output
				    += gucon(colpnt, InterleaveSequence[InterleaveSequenceIndices[iSequence].index], ilData.output, code);
			for (ine = InterleaveSequenceIndices[iSequence].index; ine < InterleaveSequenceIndices[iSequence + 1].index; ine++) {
				StitchBuffer[ilData.output].x         = InterleaveSequence[ine].x;
				StitchBuffer[ilData.output].y         = InterleaveSequence[ine].y;
				StitchBuffer[ilData.output].attribute = code;
				if (ilData.output > 0) {
					if (StitchBuffer[ilData.output].x != StitchBuffer[ilData.output - 1].x
					    || StitchBuffer[ilData.output].y != StitchBuffer[ilData.output - 1].y)
						ilData.output++;
				}
				else {
					ilData.output++;
				}
			}
		}
		chkend(0, code, ilData);
	}
	PCSHeader.stitchCount = ilData.output;
	coltab();
}

void setundfn(unsigned code) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (SelectedForm->type != FRMLINE) {
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute |= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				refilfn();
			}
		}
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE) {
				continue;
			}
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute |= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				refilfn();
			}
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
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
	savdo();
	code = ~code;
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (SelectedForm->type != FRMLINE) {
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute &= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				refilfn();
			}
		}
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE) {
				continue;
			}
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute &= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				refilfn();
			}
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
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

void ulenfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
		SelectedForm->underlayStitchLen = length;
		refilfn();
	}
}

void dusulen(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		ulenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ulenfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void undlen() {
	tabmsg(IDS_SETULEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETULEN);
	numWnd();
}

void uspacfn(size_t find, float spacing) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & AT_UND) {
		SelectedForm->underlaySpacing = spacing;
		refilfn();
	}
}

void duspac(float spacing) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		uspacfn(ClosestFormToCursor, spacing);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			uspacfn(selectedForm, spacing);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void uspac() {
	tabmsg(IDS_SETUSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUSPAC);
	numWnd();
}

void uangfn(size_t find, float angle) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & AT_UND) {
		SelectedForm->underlayStitchAngle = angle;
		refilfn();
	}
}

void dufang(float angle) {
	savdo();
	angle *= (float)PI / 180;
	if (StateMap.test(StateFlag::FORMSEL)) {
		uangfn(ClosestFormToCursor, angle);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			uangfn(selectedForm, angle);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void sfuang() {
	tabmsg(IDS_SETUANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUANG);
	numWnd();
}

void flenfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType && !isclp(find)) {
		SelectedForm->lengthOrCount.stitchLength = length;
		refilfn();
	}
}

void duflen(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		flenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			flenfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setflen() {
	tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFLEN);
	numWnd();
}

void fspacfn(size_t find, float spacing) {
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
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fspacfn(ClosestFormToCursor, spacing);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fspacfn(selectedForm, spacing);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfspac() {
	tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFSPAC);
	numWnd();
}

void findfn(size_t find, float indent) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	SelectedForm->underlayIndent = indent;
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK))
		refilfn();
}

void dufind(float indent) {
	savdo();
	indent *= PFGRAN;
	if (StateMap.test(StateFlag::FORMSEL)) {
		findfn(ClosestFormToCursor, indent);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			findfn(selectedForm, indent);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void fangfn(size_t find, float angle) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	// ToDo - also do angle updates for texture filled forms
	if (SelectedForm->type == FRMFPOLY && SelectedForm->fillType) {
		switch (SelectedForm->fillType) {
		case VRTF:
		case HORF:
		case ANGF:
			SelectedForm->fillType              = ANGF;
			SelectedForm->angleOrClipData.angle = angle;
			break;
		case VCLPF:
		case HCLPF:
		case ANGCLPF:
			SelectedForm->fillType           = ANGCLPF;
			SelectedForm->satinOrAngle.angle = angle;
			break;
		}
		refilfn();
	}
}

void dufxang(float angle) {
	savdo();
	angle *= (float)PI / 180;
	if (StateMap.test(StateFlag::FORMSEL)) {
		fangfn(ClosestFormToCursor, angle);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fangfn(selectedForm, angle);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfang() {
	tabmsg(IDS_SETFANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFANG);
	numWnd();
}

void ucolfn(size_t find, unsigned color) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
		SelectedForm->underlayColor = color;
		refilfn();
	}
}

void dundcol(unsigned color) {
	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		ucolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ucolfn(selectedForm, color);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setucol() {
	tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUCOL);
	numWnd();
}

void fcolfn(size_t find, unsigned color) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->fillColor = color;
		refilfn();
	}
}

void dufcol(unsigned color) {
	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		fcolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fcolfn(selectedForm, color);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfcol() {
	tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFCOL);
	numWnd();
}

void bcolfn(size_t find, unsigned color) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->borderColor = color;
		refilfn();
	}
}

void dubcol(unsigned color) {
	savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		bcolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			bcolfn(selectedForm, color);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbcol() {
	tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBCOL);
	numWnd();
}

void blenfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType && !iseclp(find)) {
		SelectedForm->lengthOrCount.stitchLength = length;
		refilfn();
	}
}

void dublen(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		blenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			blenfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setblen() {
	tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBLEN);
	numWnd();
}

void bspacfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->edgeSpacing = length;
		refilfn();
	}
}

void dubspac(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		bspacfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			bspacfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbspac() {
	tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBSPAC);
	numWnd();
}

void bminfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->minBorderStitchLen = length;
		refilfn();
	}
}

void dubmin(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		bminfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			bminfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbmin() {
	tabmsg(IDS_TXT23);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMIN);
	numWnd();
}

void bmaxfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->maxBorderStitchLen = length;
		refilfn();
	}
}

void dubmax(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		bmaxfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			bmaxfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setbmax() {
	tabmsg(IDS_TXT22);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMAX);
	numWnd();
}

void fminfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->minFillStitchLen = length;
		refilfn();
	}
}

void dufmin(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fminfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fminfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfmin() {
	tabmsg(IDS_TXT21);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMIN);
	numWnd();
}

void fmaxfn(size_t find, float length) {
	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->maxFillStitchLen = length;
		refilfn();
	}
}

void dufmax(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fmaxfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fmaxfn(selectedForm, length);
		}
	}
	coltab();
	StateMap.set(StateFlag::RESTCH);
}

void setfmax() {
	tabmsg(IDS_TXT20);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMAX);
	numWnd();
}

void fwidfn(size_t find, float length) {
	unsigned iVertex   = 0;
	double   ratio     = 0.0;
	float    reference = 0.0;

	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	reference = SelectedForm->rectangle.left;
	ratio     = length / (SelectedForm->rectangle.right - reference);
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference) * ratio + reference;
	frmout(ClosestFormToCursor);
	refilfn();
}

void dufwid(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fwidfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fwidfn(selectedForm, length);
		}
	}
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

void fhifn(size_t find, float length) {
	unsigned iVertex   = 0;
	double   ratio     = 0.0;
	float    reference = 0.0;

	ClosestFormToCursor = find;
	fvars(ClosestFormToCursor);
	reference = SelectedForm->rectangle.bottom;
	ratio     = length / (SelectedForm->rectangle.top - reference);
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
		CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference) * ratio + reference;
	frmout(ClosestFormToCursor);
	refilfn();
}

void dufhi(float length) {
	savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fhifn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			fhifn(selectedForm, length);
		}
	}
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
		FormList[ClosestFormToCursor].fillStart = gsl::narrow<unsigned short>(ClosestVertexToCursor);
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
		FormList[ClosestFormToCursor].fillEnd = gsl::narrow<unsigned short>(ClosestVertexToCursor);
		FormList[ClosestFormToCursor].extendedAttribute |= AT_END;
		refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		shoseln(IDS_FORMP, IDS_FEND);
}

void duauxnam() {
	*AuxName = *WorkingFileName;
	switch (IniFile.auxFileType) {
	case AUXDST:
		AuxName->replace_extension(".dst");
		break;
#if PESACT
	case AUXPES:
		AuxName->replace_extension(".pes");
		break;
#endif
	default:
		AuxName->replace_extension("pcs");
	}
}

void redtbak() {
	OutputDebugString(fmt::format(L"retrieving texture history {}\n", TextureHistoryIndex).c_str());
	const TXHST* textureHistoryItem = &TextureHistory[TextureHistoryIndex];
	if (textureHistoryItem) {
		TextureScreen.areaHeight = textureHistoryItem->height;
		TextureScreen.width      = textureHistoryItem->width;
		TextureScreen.spacing    = textureHistoryItem->spacing;
		if (textureHistoryItem->texturePoint.size()) {
			TempTexturePoints->clear();
			TempTexturePoints->reserve(textureHistoryItem->texturePoint.size());
			for (auto i = 0u; i < textureHistoryItem->texturePoint.size(); i++) {
				TempTexturePoints->push_back(textureHistoryItem->texturePoint[i]);
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void dutxtfil() {
	if (!IniFile.textureHeight)
		IniFile.textureHeight = ITXHI;
	if (!IniFile.textureWidth)
		IniFile.textureWidth = ITXWID;
	if (!IniFile.textureSpacing)
		IniFile.textureSpacing = static_cast<float>(ITXSPAC);
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
	SelectedTexturePointsList->clear();
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
		TempTexturePoints->clear();
		TextureScreen.areaHeight = IniFile.textureHeight;
		TextureScreen.width      = IniFile.textureWidth;
		TextureScreen.spacing    = IniFile.textureSpacing;
	}
	StateMap.set(StateFlag::RESTCH);
}

void txt2pix(const TXPNT& texturePoint, POINT& screenPoint) noexcept {
	screenPoint.y = TextureScreen.height - texturePoint.y / TextureScreen.areaHeight * TextureScreen.height + TextureScreen.top;
	screenPoint.x = (texturePoint.line * TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
}

void txtxfn(const POINT& reference, int offsetPixels) noexcept {
	POINT line[2];

	line[0].x = line[1].x = reference.x;
	line[0].y             = reference.y - offsetPixels;
	line[1].y             = reference.y + offsetPixels;
	Polyline(StitchWindowMemDC, line, 2);
	line[0].y = line[1].y = reference.y;
	line[0].x             = reference.x - offsetPixels;
	line[1].x             = reference.x + offsetPixels;
	Polyline(StitchWindowMemDC, line, 2);
}

void dutxtx(int index, int offsetPixels) {
	POINT ref;

	txt2pix((*TempTexturePoints)[index], ref);
	txtxfn(ref, offsetPixels);
	if (ref.y > TextureScreen.halfHeight)
		ref.y -= TextureScreen.height;
	else
		ref.y += TextureScreen.height;
	txtxfn(ref, offsetPixels);
}

void txrct2rct(const TXTRCT& textureRect, RECT& rectangle) noexcept {
	TXPNT texturePoint = { textureRect.top, textureRect.left };
	POINT point        = {};

	txt2pix(texturePoint, point);
	rectangle.left    = point.x - IniFile.textureEditorSize;
	rectangle.top     = point.y - IniFile.textureEditorSize;
	texturePoint.y    = textureRect.bottom;
	texturePoint.line = textureRect.right;
	txt2pix(texturePoint, point);
	rectangle.right  = point.x + IniFile.textureEditorSize;
	rectangle.bottom = point.y + IniFile.textureEditorSize;
}

void ed2px(const fPOINT& editPoint, POINT& point) noexcept {
	point.x = editPoint.x / TextureScreen.editToPixelRatio;
	point.y = StitchWindowClientRect.bottom - editPoint.y / TextureScreen.editToPixelRatio;
}

void px2ed(const POINT& point, fPOINT& editPoint) noexcept {
	editPoint.x = point.x * TextureScreen.editToPixelRatio;
	editPoint.y = TextureScreen.screenHeight - point.y * TextureScreen.editToPixelRatio;
}

void bxtxt(unsigned iButton, unsigned iMessage) {
	std::wstring message;
	loadString(message, iMessage);
	SetWindowText((*ButtonWin)[iButton], message.c_str());
}

void hlpflt(unsigned iButton, unsigned iMessage, float data) {
	std::wstring fmtStr;
	loadString(fmtStr, iMessage);
	SetWindowText((*ButtonWin)[iButton], fmt::format(fmtStr, data).c_str());
}

void drwtxbut() {
	bxtxt(HTXCLR, IDS_CLEAR);
	hlpflt(HTXHI, IDS_TXHI, TextureScreen.areaHeight / PFGRAN);
	redraw((*ButtonWin)[HTXWID]);
	hlpflt(HTXSPAC, IDS_TXSPAC, TextureScreen.spacing / PFGRAN);
	bxtxt(HTXVRT, IDS_TXVRT);
	bxtxt(HTXHOR, IDS_TXHOR);
	bxtxt(HTXANG, IDS_TXANG);
	bxtxt(HTXMIR, IDS_TXMIR);
	SetWindowText((*ButtonWin)[HTXMIR + 1], L"");
}

void chktx() {
	std::vector<TXPNT> tmpTexture;
	for (auto& p : *TempTexturePoints) {
		if (p.line <= TextureScreen.lines && p.y <= TextureScreen.areaHeight) {
			tmpTexture.push_back(p);
		}
	}
	*TempTexturePoints = tmpTexture;
}

void drwtxtr() {
	POINT    line[2] = {};
	int      iGrid = 0, iVertical = 0;
	unsigned iPoint        = 0;
	unsigned index         = 0;
	int      yOffset       = 0;
	TXPNT    textureRecord = {};
	POINT    point         = {};
	int      gridLineCount = 0;
	double   pixelSpace    = 0.0;
	float    extraWidth    = 0.0;

	const double editSpace = TextureScreen.areaHeight * 2 / (TextureScreen.spacing * (TextureScreen.lines + 2));

	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	pixelSpace          = static_cast<double>(StitchWindowClientRect.bottom) / StitchWindowClientRect.right;
	TextureScreen.lines = floor(TextureScreen.width / TextureScreen.spacing);
	extraWidth          = TextureScreen.spacing * (TextureScreen.lines + 2);
	if (StateMap.testAndReset(StateFlag::CHKTX))
		chktx();
	if (pixelSpace > editSpace) {
		TextureScreen.xOffset          = 0;
		TextureScreen.editToPixelRatio = extraWidth / StitchWindowClientRect.bottom;
		yOffset = (StitchWindowClientRect.bottom - TextureScreen.areaHeight / TextureScreen.editToPixelRatio) / 2;
	}
	else {
		TextureScreen.editToPixelRatio = TextureScreen.areaHeight * 2 / StitchWindowClientRect.bottom;
		yOffset                        = StitchWindowClientRect.bottom >> 2;
		TextureScreen.xOffset
		    = (StitchWindowClientRect.right * TextureScreen.editToPixelRatio - (TextureScreen.lines + 2) * TextureScreen.spacing)
		      / 2;
	}
	TextureScreen.top          = yOffset;
	TextureScreen.bottom       = StitchWindowClientRect.bottom - yOffset;
	TextureScreen.height       = TextureScreen.bottom - TextureScreen.top;
	TextureScreen.halfHeight   = StitchWindowClientRect.bottom >> 1;
	TextureScreen.screenHeight = StitchWindowClientRect.bottom * TextureScreen.editToPixelRatio;
	TextureScreen.yOffset      = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, GridPen);
	gridLineCount      = TextureScreen.areaHeight / IniFile.gridSize + 1;
	textureRecord.line = 0;
	line[0].x          = 0;
	line[1].x          = StitchWindowClientRect.right;
	textureRecord.y    = 0;
	for (iGrid = 0; iGrid < gridLineCount; iGrid++) {
		txt2pix(textureRecord, point);
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
		line[0].x = line[1].x = (iVertical * TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
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
	for (index = 0; index < TempTexturePoints->size(); index++) {
		dutxtx(index, IniFile.textureEditorSize);
	}
	if (SelectedTexturePointsList->size()) {
		txrct2rct(TextureRect, TexturePixelRect);
		line[0].y = line[1].y = TexturePixelRect.top;
		line[0].x             = TexturePixelRect.left;
		line[1].x             = TexturePixelRect.right;
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
	for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
		dutxtx((*SelectedTexturePointsList)[iPoint], IniFile.textureEditorSize);
		dutxtx((*SelectedTexturePointsList)[iPoint], IniFile.textureEditorSize << 1);
	}
	BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
	drwtxbut();
}

bool px2txt(const POINT& offset) {
	fPOINT editPoint;

	px2ed(offset, editPoint);
	TXPNT tmp = { 0, gsl::narrow<unsigned short>(floor((editPoint.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5)) };
	if (tmp.line > TextureScreen.lines)
		return false;
	if (tmp.line < 1)
		return false;
	if (offset.y > TextureScreen.top) {
		if (offset.y > TextureScreen.bottom)
			return false;
		else {
			tmp.y = TextureScreen.areaHeight
			        - (static_cast<float>(offset.y - TextureScreen.top) / TextureScreen.height * TextureScreen.areaHeight);
			TempTexturePoints->push_back(tmp);
			return true;
		}
	}
	else {
		return false;
	}
}

bool txbutfn() {
	POINT offset;

	deorg(offset);
	return px2txt(offset);
}

void txtrbut() {
	if (txbutfn()) {
		savtxt();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		rstxt();
}

bool txtclos(unsigned& closestTexturePoint) {
	if (closestTexturePoint) {
		double   length        = 0.0;
		double   minimumLength = 1e99;
		unsigned iPoint        = 0;
		POINT    reference = {}, point = {};

		deorg(reference);
		closestTexturePoint = 0;
		for (iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
			txt2pix((*TempTexturePoints)[iPoint], point);
			length = hypot(point.x - reference.x, point.y - reference.y);
			if (length < minimumLength) {
				minimumLength       = length;
				closestTexturePoint = iPoint;
			}
		}
		if (minimumLength < CLOSENUF)
			return true;
	}
	return false;
}

void tritx() noexcept {
	POINT     line[2] = {};
	const int size    = IniFile.textureEditorSize << 2;

	line[0].x = line[1].x = TextureCursorLocation.x;
	line[0].y             = TextureCursorLocation.y - size;
	line[1].y             = TextureCursorLocation.y + size;
	Polyline(StitchWindowDC, line, 2);
	line[0].y = line[1].y = TextureCursorLocation.y;
	line[0].x             = TextureCursorLocation.x - size;
	line[1].x             = TextureCursorLocation.x + size;
	Polyline(StitchWindowDC, line, 2);
}

void setxmov() {
	StateMap.set(StateFlag::TXTMOV);
	TextureCursorLocation.x = SelectTexturePointsOrigin.x = Msg.pt.x - StitchWindowOrigin.x;
	TextureCursorLocation.y = SelectTexturePointsOrigin.y = Msg.pt.y - StitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void ritxrct() noexcept {
	POINT offset
	    = { (TextureCursorLocation.x - SelectTexturePointsOrigin.x), (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };
	RECT  rectangle = { (TexturePixelRect.left + offset.x),
                       (TexturePixelRect.top + offset.y),
                       (TexturePixelRect.right + offset.x),
                       (TexturePixelRect.bottom + offset.y) };
	POINT line[5]   = {};

	line[0].x = line[1].x = line[4].x = rectangle.left;
	line[2].x = line[3].x = rectangle.right;
	line[0].y = line[3].y = line[4].y = rectangle.top;
	line[1].y = line[2].y = rectangle.bottom;
	Polyline(StitchWindowDC, line, 5);
}

void dutxrct(TXTRCT& textureRect) {
	unsigned iPoint       = 0;
	TXPNT*   texturePoint = nullptr;

	if (SelectedTexturePointsList->size()) {
		texturePoint     = &(*TempTexturePoints)[(*SelectedTexturePointsList)[0]];
		textureRect.left = textureRect.right = texturePoint->line;
		textureRect.top = textureRect.bottom = texturePoint->y;
		for (iPoint = 1; iPoint < SelectedTexturePointsList->size(); iPoint++) {
			texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
			if (texturePoint->y > textureRect.top)
				textureRect.top = texturePoint->y;
			if (texturePoint->y < textureRect.bottom)
				textureRect.bottom = texturePoint->y;
			if (texturePoint->line < textureRect.left)
				textureRect.left = texturePoint->line;
			if (texturePoint->line > textureRect.right)
				textureRect.right = texturePoint->line;
		}
	}
	else
		textureRect.left = textureRect.right = textureRect.top = textureRect.bottom = 0;
}

fPOINT ed2stch(const fPOINT& point) noexcept {
	return { (point.x - TextureScreen.xOffset), (point.y - TextureScreen.yOffset) };
}

void dutxlin(const fPOINT& point0in, const fPOINT& point1in) {
	double       slope  = 0.0;
	float        deltaX = 0.0;
	float        start = 0.0, finish = 0.0, yOffset = 0.0;
	int          integerStart = 0, integerFinish = 0;
	const fPOINT point0 = ed2stch(point0in);
	const fPOINT point1 = ed2stch(point1in);

	deltaX = point1.x - point0.x;
	if (fabs(deltaX) < TINY)
		return;
	slope  = (point1.y - point0.y) / deltaX;
	start  = point0.x;
	finish = point1.x;
	if (start > finish) {
		std::swap(start, finish);
	}
	integerStart  = ceil(start / TextureScreen.spacing);
	integerFinish = floor(finish / TextureScreen.spacing);
	if (integerStart < 1) {
		integerStart = 1;
	}
	if (integerFinish > TextureScreen.lines) {
		integerFinish = TextureScreen.lines;
	}
	const auto lineRange = integerFinish - integerStart;
	if (lineRange >= 0) {
		TempTexturePoints->reserve(TempTexturePoints->size() + lineRange);
		while (integerStart <= integerFinish) {
			yOffset = slope * (-point0.x + integerStart * TextureScreen.spacing) + point0.y;
			if (yOffset > 0 && yOffset < TextureScreen.areaHeight) {
				TempTexturePoints->push_back({ yOffset, gsl::narrow<unsigned short>(integerStart) });
			}
			integerStart++;
		}
	}
}

void setxclp() {
	POINT  screenOffset = {};
	fPOINT editorOffset = {};
	size_t lineCount    = 0;
	size_t iNextVertex  = 0;

	deorg(screenOffset);
	px2ed(screenOffset, editorOffset);
	if (StateMap.testAndReset(StateFlag::TXHCNTR))
		editorOffset.x = (TextureScreen.lines * TextureScreen.spacing) / 2 + TextureScreen.xOffset - TextureScreen.formCenter.x
		                 + TextureScreen.spacing / 2;
	else
		editorOffset.x -= TextureScreen.formCenter.x;
	editorOffset.y -= TextureScreen.formCenter.y;
	for (size_t iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x += editorOffset.x;
		AngledFormVertices[iVertex].y += editorOffset.y;
	}
	lineCount = AngledForm.vertexCount - 1;
	if (AngledForm.type != FRMLINE)
		lineCount++;
	VertexCount = AngledForm.vertexCount;
	for (size_t iLine = 0u; iLine < lineCount; iLine++) {
		iNextVertex = nxt(iLine);
		dutxlin(AngledFormVertices[iLine], AngledFormVertices[iNextVertex]);
	}
}

void stxlin() {
	POINT  offset;
	fPOINT point0;
	fPOINT point1;

	StateMap.reset(StateFlag::TXTMOV);
	deorg(offset);
	px2ed(offset, point1);
	px2ed(FormLines[0], point0);
	dutxlin(point0, point1);
	StateMap.set(StateFlag::RESTCH);
}

void ed2txp(const POINT& offset, TXPNT& textureRecord) noexcept {
	fPOINT point;

	px2ed(offset, point);
	textureRecord.line = (point.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5;
	textureRecord.y    = TextureScreen.areaHeight
	                  - (static_cast<float>(offset.y - TextureScreen.top) / TextureScreen.height * TextureScreen.areaHeight);
}

void txtrup() {
	TXPNT highestTexturePoint = {};
	TXPNT lowestTexturePoint  = {};
	TXPNT textureOffset       = {};
	float yOffset             = 0.0;
	// short    swap                = 0;
	unsigned iPoint = 0;
	int      xCoord = 0, Xmagnitude = 0;
	POINT    offset       = {};
	TXPNT*   texturePoint = nullptr;

	if (StateMap.testAndReset(StateFlag::TXTMOV)) {
		savtxt();
		deorg(offset);
		offset.x -= SelectTexturePointsOrigin.x;
		offset.y -= SelectTexturePointsOrigin.y;
		Xmagnitude         = abs(offset.x);
		textureOffset.line = Xmagnitude * TextureScreen.editToPixelRatio / TextureScreen.spacing + 0.5;
		if (offset.x < 0)
			textureOffset.line = -textureOffset.line;
		textureOffset.y = static_cast<float>(-offset.y) / TextureScreen.height * TextureScreen.areaHeight;
		yOffset         = TextureRect.top + textureOffset.y - TextureScreen.areaHeight;
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
		for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
			texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
			texturePoint->line += textureOffset.line;
			texturePoint->y += textureOffset.y;
		}
		dutxrct(TextureRect);
	}
	else {
		if (StateMap.testAndReset(StateFlag::BZUMIN)) {
			deorg(offset);
			ed2txp(ZoomBoxLine[0], highestTexturePoint);
			ed2txp(offset, lowestTexturePoint);
			if (highestTexturePoint.line < lowestTexturePoint.line) {
				std::swap(highestTexturePoint.line, lowestTexturePoint.line);
			}
			if (highestTexturePoint.y < lowestTexturePoint.y) {
				std::swap(highestTexturePoint.y, lowestTexturePoint.y);
			}
			SelectedTexturePointsList->clear();
			for (iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
				if ((*TempTexturePoints)[iPoint].y < highestTexturePoint.y
				    && (*TempTexturePoints)[iPoint].y > lowestTexturePoint.y
				    && (*TempTexturePoints)[iPoint].line <= highestTexturePoint.line
				    && (*TempTexturePoints)[iPoint].line >= lowestTexturePoint.line) {
					SelectedTexturePointsList->push_back(iPoint);
				}
			}
			dutxrct(TextureRect);
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

void angrct(fRECTANGLE& rectangle) noexcept {
	unsigned iVertex = 0;

	rectangle.left = rectangle.right = AngledFormVertices[0].x;
	rectangle.bottom = rectangle.top = AngledFormVertices[0].y;
	for (iVertex = 1; iVertex < AngledForm.vertexCount; iVertex++) {
		if (AngledFormVertices[iVertex].x < rectangle.left)
			rectangle.left = AngledFormVertices[iVertex].x;
		if (AngledFormVertices[iVertex].x > rectangle.right)
			rectangle.right = AngledFormVertices[iVertex].x;
		if (AngledFormVertices[iVertex].y > rectangle.top)
			rectangle.top = AngledFormVertices[iVertex].y;
		if (AngledFormVertices[iVertex].y < rectangle.bottom)
			rectangle.bottom = AngledFormVertices[iVertex].y;
	}
}

void ritxfrm() {
	size_t iVertex     = 0u;
	size_t vertexCount = 0;
	POINT  offset
	    = { (TextureCursorLocation.x - SelectTexturePointsOrigin.x), (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };

	offset.x = TextureCursorLocation.x - SelectTexturePointsOrigin.x;
	offset.y = TextureCursorLocation.y - SelectTexturePointsOrigin.y;
	for (iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		ed2px(AngledFormVertices[iVertex], FormLines[iVertex]);
		FormLines[iVertex].x += offset.x;
		FormLines[iVertex].y += offset.y;
	}
	FormLines[iVertex] = FormLines[0];
	vertexCount        = AngledForm.vertexCount;
	if (AngledForm.type != FRMLINE)
		vertexCount++;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	PolylineInt(StitchWindowDC, FormLines, vertexCount);
}

void setxfrm() {
	unsigned   iVertex   = 0;
	fRECTANGLE angleRect = {};
	float      height    = 0.0;
	double     ratio     = 0.0;

	angrct(angleRect);
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x -= angleRect.left;
		AngledFormVertices[iVertex].y -= angleRect.bottom;
	}
	angrct(angleRect);
	height = angleRect.top - angleRect.bottom;
	if (height > TextureScreen.areaHeight) {
		ratio = TextureScreen.areaHeight / height * 0.95;
		for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex].x *= ratio;
			AngledFormVertices[iVertex].y *= ratio;
		}
		angrct(angleRect);
	}
	TextureScreen.formCenter.x = midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = midl(angleRect.top, angleRect.bottom);
	ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
}

void txtclp() {
	ThrEdClip  = RegisterClipboardFormat(ThrEdClipFormat);
	ClipMemory = GetClipboardData(ThrEdClip);
	if (ClipMemory) {
		ClipFormHeader = static_cast<FORMCLIP*>(GlobalLock(ClipMemory));
		if (ClipFormHeader) {
			if (ClipFormHeader->clipType == CLP_FRM) {
				SelectedForm     = &ClipFormHeader->form;
				auto vertices    = convert_ptr<fPOINT*>(&SelectedForm[1]);
				AngledForm       = *SelectedForm;
				auto sourceStart = vertices;
				auto sourceEnd   = sourceStart + SelectedForm->vertexCount;
				std::copy(sourceStart, sourceEnd, AngledFormVertices);
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
		deorg(FormLines[1]);
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
		if (SelectedTexturePointsList->size()) {
			ritxrct();
			TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
			TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			ritxrct();
		}
	}
}

void txtlin() {
	deorg(FormLines[0]);
	deorg(FormLines[1]);
	StateMap.reset(StateFlag::TXTCLP);
	StateMap.set(StateFlag::TXTLIN);
	StateMap.set(StateFlag::TXTMOV);
}

void chktxnum() {
	float  value    = 0;
	size_t lastChar = 0;

	if (TextureInputBuffer->size()) {
		value = stof(*TextureInputBuffer, &lastChar);
	}
	if (value) {
		TextureInputBuffer->clear();
		value *= PFGRAN;
		switch (TextureWindowId) {
		case HTXHI:
			savtxt();
			TextureScreen.areaHeight = value;
			IniFile.textureHeight    = value;
			StateMap.set(StateFlag::CHKTX);
			break;
		case HTXWID:
			savtxt();
			TextureScreen.width  = value;
			IniFile.textureWidth = value;
			StateMap.set(StateFlag::CHKTX);
			break;
		case HTXSPAC:
			savtxt();
			TextureScreen.spacing  = value;
			IniFile.textureSpacing = value;
			TextureScreen.width    = value * TextureScreen.lines + value / 2;
			StateMap.set(StateFlag::CHKTX);
			break;
		}
	}
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	StateMap.set(StateFlag::RESTCH);
}

void butsid(unsigned windowId) {
	RECT buttonRect = {};

	chktxnum();
	TextureWindowId = windowId;
	GetWindowRect((*ButtonWin)[windowId], &buttonRect);
	SideWindowButton = CreateWindow(L"STATIC",
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

bool txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1) noexcept {
	if (texturePoint0.line == texturePoint1.line) {
		return (texturePoint0.y < texturePoint1.y);
	}
	else {
		return (texturePoint0.line < texturePoint1.line);
	}
}

void txpar() {
	SelectedForm->type                       = FRMFPOLY;
	SelectedForm->fillInfo.texture.lines     = TextureScreen.lines;
	SelectedForm->fillInfo.texture.height    = TextureScreen.areaHeight;
	SelectedForm->fillSpacing                = TextureScreen.spacing;
	SelectedForm->lengthOrCount.stitchLength = IniFile.userStitchLength;
	SelectedForm->maxFillStitchLen           = IniFile.maxStitchLength;
	SelectedForm->minFillStitchLen           = IniFile.minStitchLength;
	SelectedForm->fillColor                  = ActiveColor;
	refilfn();
}

void txvrt() {
	if (TempTexturePoints->size()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXVRTF;
			txpar();
		}
	}
}

void txhor() {
	if (TempTexturePoints->size()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXHORF;
			txpar();
		}
	}
}

void txang() {
	if (TempTexturePoints->size()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType              = TXANGF;
			SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
			txpar();
		}
	}
}

void nutx() {
	unsigned iPoint     = 0;
	FRMHED*  formHeader = nullptr;

	std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), txcmp);
	if (FormIndex) {
		if (istx(ClosestFormToCursor)) {
			iPoint = FormList[ClosestFormToCursor].fillInfo.texture.index;
			deltx();
		}
		else {
			for (auto iForm = ClosestFormToCursor; iForm-- > 0;) {
				if (istx(iForm)) {
					formHeader = &FormList[iForm];
					iPoint     = formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count;
					break;
				}
			}
		}
	}
	if (TempTexturePoints->size()) {
		const auto tempPointCount = TempTexturePoints->size();
		const auto pointCount     = TextureIndex - iPoint;
		TexturePointsBuffer->resize(TexturePointsBuffer->size() + tempPointCount);
		TextureIndex += tempPointCount;
		auto startSource = TexturePointsBuffer->begin() + iPoint;
		if (pointCount) {
			auto endSource   = startSource + pointCount;
			auto destination = startSource + tempPointCount;
			auto _           = std::copy(startSource, endSource, destination);
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			if (istx(iForm))
				FormList[iForm].fillInfo.texture.index += gsl::narrow<unsigned short>(tempPointCount);
		}
		auto _                               = std::copy(TempTexturePoints->cbegin(), TempTexturePoints->cend(), startSource);
		SelectedForm->fillInfo.texture.index = iPoint;
		SelectedForm->fillInfo.texture.count = gsl::narrow<unsigned short>(tempPointCount);
	}
}

// Ensure all lines in the texture have at least 1 point
void altx() {
	unsigned short iLine      = 0;
	unsigned       iPoint     = 0;
	float          halfHeight = 0.0;

	boost::dynamic_bitset<> txtLines(TextureScreen.lines + 1);
	if (StateMap.test(StateFlag::FORMSEL)) {
		halfHeight = TextureScreen.areaHeight / 2;
		for (iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
			txtLines.set((*TempTexturePoints)[iPoint].line);
		}
		for (iLine = 1; iLine <= TextureScreen.lines; iLine++) {
			if (!txtLines.test(iLine)) {
				TempTexturePoints->push_back({ halfHeight, iLine });
			}
		}
	}
}

void txof() {
	butxt(HBOXSEL, (*StringTable)[STR_BOXSEL]);
	redraw((*ButtonWin)[HHID]);
	if (StateMap.test(StateFlag::UPTO))
		butxt(HUPTO, (*StringTable)[STR_UPON]);
	else
		butxt(HUPTO, (*StringTable)[STR_UPOF]);
	SetWindowText((*ButtonWin)[HTXSPAC], L"");
	savtxt();
	zumhom();
	SelectedTexturePointsList->clear();
	SelectedTexturePointsList->shrink_to_fit();
	StateMap.reset(StateFlag::TXTRED);
}

enum
{
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
		dusqr();
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

void dutxmir() {
	const int centerLine   = (TextureScreen.lines + 1) >> 1;
	size_t    iPoint       = TempTexturePoints->size() - 1;
	size_t    iMirrorPoint = 0;

	savtxt();
	std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), txcmp);
	while ((*TempTexturePoints)[iPoint].line > centerLine && iPoint >= 0)
		iPoint--;
	iMirrorPoint = iPoint + 1;
	if (TextureScreen.lines & 1) {
		while (iPoint >= 0) {
			if ((*TempTexturePoints)[iPoint].line == centerLine) {
				iPoint--;
			}
			else {
				break;
			}
		}
	}
	TempTexturePoints->resize(iMirrorPoint);
	for (iPoint = 0; iPoint < iMirrorPoint; iPoint++) {
		TempTexturePoints->push_back(
		    { (*TempTexturePoints)[iPoint].y,
		      gsl::narrow<unsigned short>(TextureScreen.lines - (*TempTexturePoints)[iPoint].line + 1) });
	}
	StateMap.set(StateFlag::RESTCH);
}

void txdelal() {
	chktxnum();
	savtxt();
	TempTexturePoints->clear();
	rstxt();
	StateMap.set(StateFlag::RESTCH);
}

bool chkbut() {
	if (Msg.hwnd == (*ButtonWin)[HTXCLR]) {
		txdelal();
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXHI]) {
		butsid(HTXHI);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXWID]) {
		butsid(HTXWID);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXSPAC]) {
		butsid(HTXSPAC);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXVRT]) {
		dutxfn(VRTYP);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXHOR]) {
		dutxfn(HORTYP);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXANG]) {
		dutxfn(ANGTYP);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXMIR]) {
		dutxmir();
		return 1;
	}
	return 0;
}

void txtlbut() {
	POINT point = {};

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
	if (SelectedTexturePointsList->size()) {
		deorg(point);
		if (point.x > TexturePixelRect.left && point.x < TexturePixelRect.right && point.y > TexturePixelRect.top
		    && point.y < TexturePixelRect.bottom) {
			setxmov();
			ritxrct();
			return;
		}
	}
	if (SelectedTexturePointsList->size()) {
		if (txtclos((*SelectedTexturePointsList)[0])) {
			SelectedTexturePointsList->resize(1);
			setxmov();
			dutxrct(TextureRect);
			StateMap.set(StateFlag::RESTCH);
			return;
		}
	}
	SelectedTexturePointsList->clear();
	StateMap.set(StateFlag::BZUMIN);
	StateMap.reset(StateFlag::BZUM);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
}

void txbak() {
	unsigned iHistory = 0;
	bool     flag     = false;

	if (StateMap.test(StateFlag::WASTXBAK)) {
		SelectedTexturePointsList->clear();
		for (iHistory = 0; iHistory < ITXBUFLEN; iHistory++) {
			if (TextureHistory[TextureHistoryIndex].width) {
				flag = true;
				break;
			}
			txrbak();
		}
		if (flag) {
			redtbak();
			txrbak();
		}
	}
}

void nxbak() {
	unsigned iHistory = 0;
	bool     flag     = false;

	if (StateMap.test(StateFlag::WASTXBAK)) {
		for (iHistory = 0; iHistory < ITXBUFLEN; iHistory++) {
			txrfor();
			if (TextureHistory[TextureHistoryIndex].width) {
				flag = true;
				break;
			}
		}
		if (flag) {
			redtbak();
		}
	}
}

void txtdel() {
	unsigned iSourcePoint  = 0;
	unsigned iClosestPoint = 0;

	if (SelectedTexturePointsList->size()) {
		savtxt();
		boost::dynamic_bitset<> texturePointsMap(TempTexturePoints->size());
		for (auto& p : *SelectedTexturePointsList) {
			texturePointsMap.set(p);
		}
		// Another potential pattern for this:
		// myVector.erase(remove_if(myVector.begin(), myVector.end(), testFunction), myVector.end());
		std::vector<TXPNT> tmpTexture;
		tmpTexture.reserve(TempTexturePoints->size() - SelectedTexturePointsList->size());
		for (iSourcePoint = 0; iSourcePoint < TempTexturePoints->size(); iSourcePoint++) {
			if (!texturePointsMap.test(iSourcePoint)) {
				tmpTexture.push_back((*TempTexturePoints)[iSourcePoint]);
			}
		}
		*TempTexturePoints = tmpTexture;
		SelectedTexturePointsList->clear();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (TempTexturePoints->size() && txtclos(iClosestPoint)) {
		auto it = TempTexturePoints->begin();
		std::advance(it, iClosestPoint);
		it = TempTexturePoints->erase(it);
		StateMap.set(StateFlag::RESTCH);
	}
}

void txcntrv() {
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		StateMap.set(StateFlag::TXHCNTR);
		savtxt();
		setxclp();
		StateMap.set(StateFlag::RESTCH);
	}
}

bool istxclp() {
	if (StateMap.test(StateFlag::TXTMOV) && StateMap.test(StateFlag::TXTCLP))
		return 1;
	else
		return 0;
}

void txsiz(double ratio) {
	unsigned   iVertex   = 0;
	fRECTANGLE angleRect = {};

	ritxfrm();
	for (iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x *= ratio;
		AngledFormVertices[iVertex].y *= ratio;
	}
	angrct(angleRect);
	TextureScreen.formCenter.x = midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = midl(angleRect.top, angleRect.bottom);
	ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
	ritxfrm();
}

void txshrnk() {
	txsiz(TXTRAT);
}

void txgro() {
	txsiz(1 / TXTRAT);
}

bool txdig(unsigned keyCode, char& character) {
	if (isdigit(keyCode)) {
		character = gsl::narrow<char>(keyCode);
		return 1;
	}
	if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
		character = gsl::narrow<char>(keyCode) - VK_NUMPAD0 + 0x30;
		return 1;
	}
	if (keyCode == 0xbe || keyCode == 0x6e) {
		character = '.';
		return 1;
	}
	return 0;
}

void txnudg(int deltaX, float deltaY) {
	float    screenDeltaY = 0.0;
	float    yCoord       = 0.0;
	unsigned iPoint       = 0;
	int      textureLine  = 0;

	if (SelectedTexturePointsList->size()) {
		if (deltaY) {
			screenDeltaY = deltaY * TextureScreen.editToPixelRatio;
			for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
				yCoord = (*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].y + screenDeltaY;
				if (yCoord < 0)
					return;
				if (yCoord > TextureScreen.areaHeight)
					return;
			}
			for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++)
				(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].y += screenDeltaY;
		}
		else {
			for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
				textureLine = (*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].line + deltaX;
				if (textureLine < 1)
					return;
				if (textureLine > TextureScreen.lines)
					return;
			}
			for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++)
				(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].line += deltaX;
		}
	}
	dutxrct(TextureRect);
	StateMap.set(StateFlag::RESTCH);
}

void txsnap() {
	unsigned iPoint       = 0;
	int      yStep        = 0;
	float    halfGrid     = 0.0;
	TXPNT*   texturePoint = nullptr;

	if (TempTexturePoints->size()) {
		savtxt();
		halfGrid = IniFile.gridSize / 2;
		if (SelectedTexturePointsList->size()) {
			for (iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
				texturePoint    = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
				yStep           = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y = yStep * IniFile.gridSize;
			}
		}
		else {
			for (iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
				texturePoint    = &(*TempTexturePoints)[iPoint];
				yStep           = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y = yStep * IniFile.gridSize;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void tst() {
	DesignerName->assign(L"Coder");
	ThrName->assign(*DesignerName);
	StateMap.set(StateFlag::RESTCH);
}

void txtkey(unsigned keyCode) {
	char character = {};
	bool flag      = true;

	if (SideWindowButton) {
		switch (keyCode) {
		case VK_RETURN:
			chktxnum();
			return;
		case VK_ESCAPE:
			rstxt();
			StateMap.set(StateFlag::RESTCH);
			break;
		case 'Q':
			rstxt();
			StateMap.set(StateFlag::RESTCH);
			break;
		case 8: // backspace
			if (TextureInputBuffer->size())
				TextureInputBuffer->pop_back();
			flag = false;
			break;
		}
		if (flag) {
			if (TextureInputBuffer->size() < 8) { // floating point 7 digits of precision + '.'
				if (txdig(keyCode, character)) {
					TextureInputBuffer->push_back(character);
				}
			}
		}
		SetWindowText(SideWindowButton, TextureInputBuffer->c_str());
		return;
	}
	switch (keyCode) {
	case VK_ESCAPE:
		txof();
	case 'Q':
		rstxt();
		StateMap.set(StateFlag::RESTCH);
		break;
	case 0xdb: //[
		txshrnk();
		break;
	case 0xdd: //]
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
		if (GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & 0X8000)
			txdelal();
		else
			txtdel();
		break;
	case 0xbd: //-
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

void setxt(std::vector<RNGCNT>& textureSegments) {
	const auto currentIndex = SelectedForm->fillInfo.texture.index;
	const auto currentCount = SelectedForm->fillInfo.texture.count;

	savtxt();
	SelectedForm->wordParam = 0;
	StateMap.set(StateFlag::TXFIL);
	ClipRectSize.cx = SelectedForm->fillSpacing;
	ClipRectSize.cy = SelectedForm->fillInfo.texture.height;
	if (currentCount) {
		int iSegment = 0;
		for (auto iTexturePoint = currentCount - 1; iTexturePoint >= 0; iTexturePoint--) {
			const auto currentPoint = TexturePointsBuffer->at(currentIndex + iTexturePoint);
			if (currentPoint.line) {
				iSegment                       = currentPoint.line - 1;
				textureSegments[iSegment].line = iTexturePoint;
				textureSegments[iSegment].stitchCount++;
			}
		}
	}
}

void rtrtx() {
	fvars(ClosestFormToCursor);
	TempTexturePoints->clear();

	const auto currentIndex = SelectedForm->fillInfo.texture.index;
	if (TexturePointsBuffer->size() > gsl::narrow<size_t>(currentIndex)) {
		auto currentCount = SelectedForm->fillInfo.texture.count;
		if (TexturePointsBuffer->size() < gsl::narrow<size_t>(currentIndex + currentCount)) {
			currentCount = gsl::narrow<unsigned short>(TexturePointsBuffer->size()) - currentIndex;
		}
		TempTexturePoints->resize(currentCount);
		auto startSource         = TexturePointsBuffer->cbegin() + currentIndex;
		auto endSource           = startSource + currentCount;
		auto _                   = std::copy(startSource, endSource, TempTexturePoints->begin());
		TextureScreen.areaHeight = SelectedForm->fillInfo.texture.height;
		TextureScreen.spacing    = SelectedForm->fillSpacing;
		TextureScreen.lines      = SelectedForm->fillInfo.texture.lines;
		TextureScreen.width      = TextureScreen.lines * TextureScreen.spacing + TextureScreen.spacing / 2;
		savtxt();
	}
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

void setstxt(unsigned stringIndex, float value, HWND dialog) {
	SetWindowText(GetDlgItem(dialog, stringIndex), fmt::format(L"{:.2f}", (value / PFGRAN)).c_str());
}

float getstxt(unsigned stringIndex, HWND dialog) {
	// ToDo - This is not great code.
	wchar_t buffer[16] = {};
	GetWindowText(GetDlgItem(dialog, stringIndex), buffer, sizeof(buffer) / sizeof(buffer[0]));
	return std::stof(buffer) * PFGRAN;
}

bool chkasp(fPOINT& point, float aspectRatio, HWND dialog) {
	point.x = getstxt(IDC_DESWID, dialog);
	point.y = getstxt(IDC_DESHI, dialog);
	if ((point.y / point.x) == aspectRatio)
		return 1;
	else
		return 0;
}

BOOL CALLBACK setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	fPOINT designSize        = {};
	float  designAspectRatio = 0.0;    // design aspect ratio
	HWND   designSizeDialog  = hwndlg; // change design size dialog window

	switch (umsg) {
	case WM_INITDIALOG:
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		setstxt(IDC_DESWID, DesignSize.x, designSizeDialog);
		setstxt(IDC_DESHI, DesignSize.y, designSizeDialog);
		CheckDlgButton(hwndlg, IDC_REFILF, UserFlagMap.test(UserFlag::CHREF));
		break;
	case WM_COMMAND:
		switch (LOWORD(wparam)) {
		case IDCANCEL:
			EndDialog(hwndlg, 0);
			return TRUE;
		case IDOK:
			DesignSize.x = getstxt(IDC_DESWID, designSizeDialog);
			DesignSize.y = getstxt(IDC_DESHI, designSizeDialog);
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
			designAspectRatio = DesignSize.y / DesignSize.x;
			if (!chkasp(designSize, designAspectRatio, designSizeDialog)) {
				if (StateMap.test(StateFlag::DESCHG))
					setstxt(IDC_DESWID, designSize.y / designAspectRatio, designSizeDialog);
				else
					setstxt(IDC_DESHI, designSize.x * designAspectRatio, designSizeDialog);
			}
			break;
		}
	}
	return 0;
}

// ToDo - Find a better name
void sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	stitch.x = (stitch.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	stitch.y = (stitch.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	point.x = (point.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	point.y = (point.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void nudfn(const fRECTANGLE& designSizeRect) {
	unsigned iStitch = 0, iVertex = 0;
	fPOINT   newSize = { (designSizeRect.right - designSizeRect.left), (designSizeRect.top - designSizeRect.bottom) };

	newSize.x              = designSizeRect.right - designSizeRect.left;
	newSize.y              = designSizeRect.top - designSizeRect.bottom;
	dPOINT designSizeRatio = { (DesignSize.x / newSize.x), (DesignSize.y / newSize.y) };
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		sadj(StitchBuffer[iStitch], designSizeRatio, designSizeRect);
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
		sadj(FormVertices[iVertex], designSizeRatio, designSizeRect);
	frmout(ClosestFormToCursor);
}

void nudsiz() {
	int        flag           = 0;
	unsigned   iForm          = 0;
	fRECTANGLE designSizeRect = {}; // design size rectangle

	savdo();
	flag = 0;
	if (PCSHeader.stitchCount) {
		stchrct(designSizeRect);
		flag = 1;
	}
	else {
		if (FormIndex) {
			frmrct(designSizeRect);
			flag = 2;
		}
	}
	if (flag) {
		DesignSize.x = designSizeRect.right - designSizeRect.left;
		DesignSize.y = designSizeRect.top - designSizeRect.bottom;
		if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, (DLGPROC)setsprc)) {
			flag = 0;
			if (DesignSize.x > IniFile.hoopSizeX) {
				IniFile.hoopSizeX = DesignSize.x * 1.05;
				UnzoomedRect.x    = IniFile.hoopSizeX;
				flag              = 1;
			}
			if (DesignSize.y > IniFile.hoopSizeY) {
				IniFile.hoopSizeY = DesignSize.y * 1.05;
				UnzoomedRect.y    = IniFile.hoopSizeY;
				flag              = 1;
			}
			nudfn(designSizeRect);
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
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	px2stch();
	ZoomBoxOrigin = SelectedPoint;
}

void mvshft() {
	if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT))
		SetCapture(ThrEdWindow);
	if (StateMap.test(StateFlag::BZUMIN)) {
		if (Msg.wParam & MK_LBUTTON) {
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

bool inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch) noexcept {
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
	fRECTANGLE selectionRect = {};
	unsigned   iStitch = 0, line = 0;

	savtxt();
	unbBox();
	StateMap.reset(StateFlag::BZUMIN);
	pxCor2stch(ZoomBoxLine[0]);
	selectionRect.top  = SelectedPoint.y;
	selectionRect.left = SelectedPoint.x;
	pxCor2stch(ZoomBoxLine[2]);
	selectionRect.bottom = SelectedPoint.y;
	selectionRect.right  = SelectedPoint.x;
	StateMap.reset(StateFlag::TXIN);
	line = 1;
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (inrct(selectionRect, StitchBuffer[iStitch])) {
			StateMap.set(StateFlag::TXIN);
			TempTexturePoints->push_back({ (StitchBuffer[iStitch].y - selectionRect.bottom), gsl::narrow<unsigned short>(line) });
		}
		else {
			if (StateMap.testAndReset(StateFlag::TXIN))
				line++;
		}
	}
	if (TempTexturePoints->size()) {
		line = TempTexturePoints->back().line;
	}
	TextureScreen.spacing    = (selectionRect.right - selectionRect.left) / line;
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width      = TextureScreen.spacing * line + TextureScreen.spacing / 2;
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
	while (EnumChildWindows(MainStitchWin, enumch, 0))
		;
}

bool txnam(wchar_t* name, int sizeName) noexcept {
	wchar_t* lastCharacter;

	wcscpy_s(name, sizeName, ArgList[0]);
	lastCharacter = StrRChrW(name, 0, L'\\');
	if (lastCharacter) {
		lastCharacter++;
		wcscpy_s(lastCharacter, sizeName - (lastCharacter - name), L"thred.txr");
		return 1;
	}
	else
		return 0;
}

void txdun() {
	wchar_t               name[_MAX_PATH] = { 0 };
	HANDLE                handle          = {};
	unsigned long         bytesWritten    = 0;
	int                   iHistory        = 0;
	const char            signature[4]    = "txh";
	std::vector<TXHSTBUF> textureHistoryBuffer(ITXBUFLEN);

	if (TextureHistory[0].texturePoint.size()) {
		if (txnam(name, sizeof(name) / sizeof(name[0]))) {
			handle = CreateFile(name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (handle != INVALID_HANDLE_VALUE) {
				WriteFile(handle, &signature, sizeof(signature), &bytesWritten, 0);
				WriteFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesWritten, 0);
				for (auto i = 0; i < ITXBUFLEN; i++) {
					auto&       bufferEntry  = textureHistoryBuffer[i];
					const auto& historyEntry = TextureHistory[i];
					bufferEntry.placeholder  = nullptr;
					bufferEntry.count        = historyEntry.texturePoint.size();
					bufferEntry.height       = historyEntry.height;
					bufferEntry.width        = historyEntry.width;
					bufferEntry.spacing      = historyEntry.spacing;
				}
				WriteFileInt(handle, textureHistoryBuffer.data(), textureHistoryBuffer.size() * ITXBUFLEN, &bytesWritten, 0);
				for (iHistory = 0; iHistory < ITXBUFLEN; iHistory++) {
					if (TextureHistory[iHistory].texturePoint.size())
						WriteFileInt(handle,
						             TextureHistory[iHistory].texturePoint.data(),
						             TextureHistory[iHistory].texturePoint.size() * sizeof(TextureHistory[0].texturePoint[0]),
						             &bytesWritten,
						             0);
				}
			}
			CloseHandle(handle);
		}
	}
}

void redtx() {
	wchar_t               name[_MAX_PATH] = { 0 };
	HANDLE                handle          = {};
	DWORD                 bytesRead = 0, historyBytesRead = 0;
	unsigned int          ind    = 0;
	char                  sig[4] = { 0 };
	std::vector<TXHSTBUF> textureHistoryBuffer(ITXBUFLEN);

	TextureHistoryIndex = ITXBUFLEN - 1;
	if (txnam(name, sizeof(name) / sizeof(name[0]))) {
		handle = CreateFile(name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (handle != INVALID_HANDLE_VALUE) {
			if (ReadFile(handle, &sig, sizeof(sig), &bytesRead, 0)) {
				if (!strcmp(sig, "txh")) {
					if (ReadFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesRead, 0)) {
						if (ReadFileInt(handle,
						                textureHistoryBuffer.data(),
						                textureHistoryBuffer.size() * ITXBUFLEN,
						                &historyBytesRead,
						                0)) {
							for (ind = 0; ind < (historyBytesRead / sizeof(textureHistoryBuffer[0])); ind++) {
								TextureHistory[ind].height  = textureHistoryBuffer[ind].height;
								TextureHistory[ind].width   = textureHistoryBuffer[ind].width;
								TextureHistory[ind].spacing = textureHistoryBuffer[ind].spacing;
								if (textureHistoryBuffer[ind].count) {
									TextureHistory[ind].texturePoint.resize(textureHistoryBuffer[ind].count);
									if (!ReadFileInt(handle,
									                 TextureHistory[ind].texturePoint.data(),
									                 sizeof(TextureHistory[0].texturePoint[0])
									                     * textureHistoryBuffer[ind].count,
									                 &bytesRead,
									                 0)) {
										TextureHistory[ind].texturePoint.clear();
										TextureHistory[ind].texturePoint.shrink_to_fit();
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
	unsigned iForm = 0, iStitch = 0, attribute = 0;

	delinf();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		SelectedForm = &FormList[iForm];
		if (!SelectedForm->type)
			SelectedForm->type = FRMFPOLY;
		else {
			if (SelectedForm->type == FRMLINE) {
				if (SelectedForm->fillType != CONTF) {
					SelectedForm->fillType                = 0;
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
	boost::dynamic_bitset<> formMap(FormIndex);
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		attribute = StitchBuffer[iStitch].attribute;
		if ((attribute & TYPMSK) == TYPFRM) {
			const unsigned tform = (attribute & FRMSK) >> FRMSHFT;
			if (tform < formMap.size()) {
				formMap.set(tform);
			}
			else {
				// ToDo - unassign the stitch from any form
				StitchBuffer[iStitch].attribute &= (NFRMSK & NTYPMSK);
			}
		}
	}
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (!formMap.test(iForm))
			FormList[iForm].fillType = 0;
	}
	formMap.reset();
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		attribute = StitchBuffer[iStitch].attribute;
		if (attribute & TYPBRD)
			formMap.set((attribute & FRMSK) >> FRMSHFT);
	}
	for (iForm = 0; iForm < FormIndex; iForm++) {
		if (!formMap.test(iForm))
			FormList[iForm].edgeType = 0;
	}
}

void chkclp(const FRMHED* const formHeader, BADCNTS& badData) {
	if (badData.clip == gsl::narrow<size_t>(formHeader->angleOrClipData.clip - ClipPoints))
		badData.clip += formHeader->lengthOrCount.clipCount;
	else
		badData.attribute |= BADCLP;
}

void chkeclp(const FRMHED* const formHeader, BADCNTS& badData) {
	if (badData.clip == gsl::narrow<size_t>(formHeader->borderClipData - ClipPoints))
		badData.clip += formHeader->clipEntries;
	else
		badData.attribute |= BADCLP;
}

unsigned frmchkfn() {
	unsigned iForm      = 0;
	FRMHED*  formHeader = nullptr;
	BADCNTS  badData    = {};

	if (FormIndex) {
		for (iForm = 0; iForm < FormIndex; iForm++) {
			formHeader = &FormList[iForm];
			if (!(badData.attribute & BADFLT)) {
				if (!formHeader->vertexCount)
					badData.attribute |= BADFLT;
				if (badData.flt == gsl::narrow<size_t>(formHeader->vertices - FormVertices))
					badData.flt += formHeader->vertexCount;
				else
					badData.attribute |= BADFLT;
			}
			if (!(badData.attribute & BADCLP)) {
				if (isclp(iForm))
					chkclp(formHeader, badData);
				if (iseclp(iForm))
					chkeclp(formHeader, badData);
			}
			if (formHeader->type == SAT && formHeader->satinGuideCount) {
				if (!(badData.attribute & BADSAT)) {
					if (badData.guideCount == gsl::narrow<size_t>(formHeader->satinOrAngle.guide - SatinGuides))
						badData.guideCount += formHeader->satinGuideCount;
					else
						badData.attribute |= BADSAT;
				}
			}
			if (istx(iForm)) {
				if (!(badData.attribute & BADTX)) {
					if (badData.tx == formHeader->fillInfo.texture.index)
						badData.tx += formHeader->fillInfo.texture.count;
					else
						badData.attribute |= BADTX;
				}
			}
			if (badData.attribute == (BADFLT | BADCLP | BADSAT | BADTX))
				break;
		}
		if (badData.flt != FormVertexIndex)
			badData.attribute |= BADFLT;
		if (badData.clip != ClipPointIndex)
			badData.attribute |= BADCLP;
		if (badData.guideCount != SatinGuideIndex)
			badData.attribute |= BADSAT;
		if (badData.tx != TextureIndex)
			badData.attribute |= BADTX;
	}
	return badData.attribute;
}

void bcup(size_t find, BADCNTS& badData) {
	FRMHED* formHeader;

	formHeader = &FormList[find];
	if (isclp(find))
		badData.clip += formHeader->lengthOrCount.clipCount;
	if (iseclp(find))
		badData.clip += formHeader->clipEntries;
	if (formHeader->type == SAT)
		badData.guideCount += formHeader->satinGuideCount;
	if (istx(find))
		badData.tx += formHeader->fillInfo.texture.count;
}

void chkfstch() noexcept {
	const size_t codedFormIndex = FormIndex << FRMSHFT;

	for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) >= codedFormIndex)
			StitchBuffer[iStitch].attribute = NOTFRM;
	}
}

void repflt(std::wstring& repairMessage) {
	unsigned iDestination     = 0;
	size_t   vertexDifference = 0, vertexCount = 0;
	BADCNTS  badData = {};

	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		if (FormList[iForm].vertexCount) {
			FormList[iDestination++] = FormList[iForm];
			vertexCount += FormList[iForm].vertexCount;
		}
	}
	FormIndex = iDestination;
	std::vector<fPOINT> vertexPoint;
	size_t              iVertex = 0;
	bool                flag    = true;
	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		FRMHED* formHeader = &FormList[iForm];
		vertexDifference   = formHeader->vertices - FormVertices;
		if (FormVertexIndex >= vertexDifference + formHeader->vertexCount) {
			vertexPoint.resize(vertexPoint.size() + formHeader->vertexCount);
			auto       sourceStart = formHeader->vertices;
			auto       sourceEnd   = sourceStart + formHeader->vertexCount;
			auto       destination = vertexPoint.begin() + iVertex;
			const auto _           = std::copy(sourceStart, sourceEnd, destination);
			formHeader->vertices   = &FormVertices[iVertex];
			iVertex += formHeader->vertexCount;
			bcup(iForm, badData);
		}
		else {
			if (vertexDifference < FormVertexIndex) {
				formHeader->vertexCount = gsl::narrow<unsigned short>(FormVertexIndex - vertexDifference);
				delsac(iForm);
				// ToDo - do we need to increase the size of vertexPoint?
				// vertexPoint.resize(vertexPoint.size + formHeader->vertexCount);
				auto sourceStart = formHeader->vertices;
				auto sourceEnd   = sourceStart + formHeader->vertexCount;
				auto destination = vertexPoint.begin() + iVertex;
				auto _           = std::copy(sourceStart, sourceEnd, destination);
				bcup(iForm, badData);
			}
			else {
				FormIndex       = iForm;
				ClipPointIndex  = badData.clip;
				SatinGuideIndex = badData.guideCount;
				TextureIndex    = badData.tx;
				chkfstch();
				adbad(repairMessage, IDS_FRMDAT, FormIndex - iForm + 1);
				flag = false;
				break;
			}
		}
	}
	if (flag) {
		FormVertexIndex = iVertex;
	}
	std::copy(vertexPoint.cbegin(), vertexPoint.cend(), FormVertices);
}

void repclp(std::wstring& repairMessage) {
	FRMHED*             formHeader = nullptr;
	unsigned            iForm = 0, badClipCount = 0;
	size_t              clipCount = 0, clipDifference = 0;
	std::vector<fPOINT> clipPoint;
	for (iForm = 0; iForm < FormIndex; iForm++) {
		formHeader = &FormList[iForm];
		if (isclp(iForm)) {
			// ToDo - pointer arithmetic to be fixed
			clipDifference = formHeader->angleOrClipData.clip - ClipPoints;
			if (clipDifference + formHeader->lengthOrCount.clipCount < ClipPointIndex) {
				clipPoint.resize(clipPoint.size() + formHeader->lengthOrCount.clipCount);
				auto sourceStart = formHeader->angleOrClipData.clip;
				auto sourceEnd   = sourceStart + formHeader->lengthOrCount.clipCount;
				auto destination = clipPoint.begin() + clipCount;
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				formHeader->angleOrClipData.clip = &ClipPoints[clipCount];
				clipCount += formHeader->lengthOrCount.clipCount;
			}
			else {
				if (clipDifference < ClipPointIndex) {
					formHeader->lengthOrCount.clipCount = gsl::narrow<unsigned int>(FormVertexIndex - clipDifference);
					clipPoint.resize(clipPoint.size() + formHeader->lengthOrCount.clipCount);
					auto sourceStart = formHeader->angleOrClipData.clip;
					auto sourceEnd   = sourceStart + formHeader->lengthOrCount.clipCount;
					auto destination = clipPoint.begin() + clipCount;
					auto _           = std::copy(sourceStart, sourceEnd, destination);

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
			// ToDo - pointer arithmetic to be fixed
			clipDifference = formHeader->borderClipData - ClipPoints;
			if (clipDifference + formHeader->clipEntries < ClipPointIndex) {
				clipPoint.resize(clipPoint.size() + formHeader->clipEntries);
				auto sourceStart = formHeader->borderClipData;
				auto sourceEnd   = sourceStart + formHeader->clipEntries;
				auto destination = clipPoint.begin() + clipCount;
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				formHeader->borderClipData = &ClipPoints[clipCount];
				clipCount += formHeader->clipEntries;
			}
			else {
				if (clipDifference < ClipPointIndex) {
					formHeader->clipEntries = gsl::narrow<unsigned short>(FormVertexIndex - clipDifference);
					clipPoint.resize(clipPoint.size() + formHeader->clipEntries);
					auto sourceStart = formHeader->borderClipData;
					auto sourceEnd   = sourceStart + formHeader->clipEntries;
					auto destination = clipPoint.begin() + clipCount;
					auto _           = std::copy(sourceStart, sourceEnd, destination);

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
	std::copy(clipPoint.cbegin(), clipPoint.cend(), ClipPoints);
	ClipPointIndex = clipCount;
	if (badClipCount)
		adbad(repairMessage, IDS_CLPDAT, badClipCount);
}

void repsat() {
	unsigned iForm = 0, guideDifference = 0;
	size_t   guideCount = 0;
	FRMHED*  formHeader = nullptr;
	BADCNTS  badData    = {};

	for (iForm = 0; iForm < FormIndex; iForm++) {
		formHeader = &FormList[iForm];
		if (formHeader->type == SAT) {
			// ToDo - pointer arithmetic to be fixed
			guideDifference = formHeader->satinOrAngle.guide - SatinGuides;
			if (FormVertexIndex > guideDifference + formHeader->vertexCount) {
				auto       sourceStart = formHeader->satinOrAngle.guide;
				auto       sourceEnd   = sourceStart + formHeader->satinGuideCount;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[guideCount], 10000 - guideCount);
				std::copy(sourceStart, sourceEnd, destination);
				formHeader->satinOrAngle.guide = &SatinGuides[guideCount];
				guideCount += formHeader->satinGuideCount;
				bcup(iForm, badData);
			}
			else {
				if (guideDifference < SatinGuideIndex) {
					formHeader->satinGuideCount = gsl::narrow<unsigned short>(SatinGuideIndex - guideDifference);
					auto       sourceStart      = formHeader->satinOrAngle.guide;
					auto       sourceEnd        = sourceStart + formHeader->satinGuideCount;
					const auto destination = stdext::make_checked_array_iterator(&SatinGuides[guideCount], 10000 - guideCount);
					std::copy(sourceStart, sourceEnd, destination);
					bcup(iForm, badData);
				}
				else {
					guideCount                  = badData.guideCount;
					formHeader->satinGuideCount = 0;
				}
			}
		}
	}
	SatinGuideIndex = guideCount;
}

void reptx() {
	size_t  textureCount = 0;
	FRMHED* formHeader   = nullptr;
	BADCNTS badData      = {};

	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		if (istx(iForm)) {
			formHeader = &FormList[iForm];
			if (gsl::narrow<unsigned short>(TextureIndex)
			    > formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count) {
				auto       sourceStart = &SatinGuides[formHeader->fillInfo.texture.index];
				auto       sourceEnd   = sourceStart + formHeader->fillInfo.texture.count;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
				std::copy(sourceStart, sourceEnd, destination);
				formHeader->fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
				textureCount += formHeader->fillInfo.texture.count;
				bcup(iForm, badData);
			}
			else {
				if (TextureIndex > formHeader->fillInfo.texture.index) {
					formHeader->fillInfo.texture.count
					    = gsl::narrow<unsigned short>(TextureIndex) - formHeader->fillInfo.texture.index;
					auto       sourceStart = &SatinGuides[formHeader->fillInfo.texture.index];
					auto       sourceEnd   = sourceStart + formHeader->fillInfo.texture.count;
					const auto destination
					    = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
					std::copy(sourceStart, sourceEnd, destination);
					formHeader->fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
					bcup(iForm, badData);
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
	std::wstring   repairMessage;
	const unsigned repairType = frmchkfn();

	savdo();
	// RepairString = MsgBuffer;
	if (repairType & BADFLT)
		repflt(repairMessage);
	if (repairType & BADCLP)
		repclp(repairMessage);
	if (repairType & BADSAT)
		repsat();
	if (repairType & BADTX)
		reptx();
	lodchk();
	StateMap.set(StateFlag::RESTCH);
	shoMsg(repairMessage);
}

void frmchkx() {
	unsigned code = 0;

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
