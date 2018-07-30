#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// C RunTime Header Files
#include <commdlg.h>
#include <Shlwapi.h>

// Standard Libraries
#include <filesystem>

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <boost/dynamic_bitset.hpp>
#pragma warning(disable : 4127) // supress warning for fmt library header 
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "EnumMap.h"
#include "utf8conv.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "hlp.h"
#include "texture.h"
#include "thred.h"

namespace fs = std::experimental::filesystem;


fPOINT                 DesignSize;                    // design size
unsigned               ColorOrder[16];                // color order adjusted for applique

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

void setfchk() {
	if (IniFile.dataCheck)
		StateMap.set(StateFlag::FCHK);
}

#ifdef _DEBUG

void prbug() noexcept {
	OutputDebugString(MsgBuffer);
}
#endif

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
	wcscpy_s(command, utf::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str());
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
	if (chkp2cnam(utf::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str())) {
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
	auto p2cName = utf::Utf16ToUtf8(std::wstring(programName));
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

