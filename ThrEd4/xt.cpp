#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

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
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

namespace fs = std::experimental::filesystem;
namespace xi = xt::internal;

fPOINT   DesignSize;     // design size
unsigned ColorOrder[16]; // color order adjusted for applique

enum stitchStyles
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

void xt::setfchk() {
	if (IniFile.dataCheck)
		StateMap.set(StateFlag::FCHK);
}

#ifdef _DEBUG

void xt::internal::prbug() noexcept {
	OutputDebugString(MsgBuffer);
}
#endif

void xt::internal::fthvars(FEATHER& feather) {
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

constexpr float xt::internal::durat(float start, float finish, float featherRatio) {
	return (finish - start) * featherRatio + start;
}

constexpr float xt::internal::duxrat(float strt, float fin, float featherRatioLocal) {
	return (fin - strt) * featherRatioLocal + strt;
}

void xt::internal::duxrats(size_t start, size_t finish, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = duxrat(BSequence[finish].x, BSequence[start].x, featherRatioLocal);
	point.y = duxrat(BSequence[finish].y, BSequence[start].y, featherRatioLocal);
}

void xt::internal::durats(size_t iSequence, fPOINT& point, FEATHER& feather) noexcept {
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

unsigned xt::internal::bpsg() noexcept {
	unsigned testValue = 0;

	if (!PseudoRandomValue)
		PseudoRandomValue = FSED;
	testValue = PseudoRandomValue & 0x40000008;
	PseudoRandomValue >>= 1;
	if (testValue == 0x8 || testValue == 0x40000000)
		PseudoRandomValue |= 0x40000000;
	return PseudoRandomValue;
}

void xt::internal::nurat(FEATHER& feather) {
	float remainder;

	remainder = fmod(feather.globalPosition, 1);
	switch (feather.fillType) {
	case FTHPSG:
		if (feather.upCount) {
			if (feather.countUp) {
				feather.ratio = static_cast<float>(feather.totalCount - (form::psg() % feather.totalCount)) / feather.totalCount;
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
			feather.ratio = static_cast<float>(feather.totalCount - (form::psg() % feather.totalCount)) / feather.totalCount;
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

void xt::internal::fthfn(size_t iSequence, FEATHER& feather) {
	nurat(feather);
	durats(iSequence, OSequence[iSequence], feather);
}

void xt::internal::ratpnt(size_t iPoint, size_t iNextPoint, fPOINT& point, float featherRatio) noexcept {
	point.x = (BSequence[iNextPoint].x - BSequence[iPoint].x) * featherRatio + BSequence[iPoint].x;
	point.y = (BSequence[iNextPoint].y - BSequence[iPoint].y) * featherRatio + BSequence[iPoint].y;
}

fPOINT xt::internal::midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept {
	return { (endPoint.x - startPoint.x) * 0.5f + startPoint.x, (endPoint.y - startPoint.y) * 0.5f + startPoint.y };
}

void xt::internal::xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = (endPoint.x - startPoint.x) * featherRatioLocal + startPoint.x;
	point.y = (endPoint.y - startPoint.y) * featherRatioLocal + startPoint.y;
}

void xt::internal::fthrbfn(size_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence) {
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

void xt::internal::fthdfn(size_t iSequence, FEATHER& feather) {
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

void xt::internal::fritfil(std::vector<fPOINT>& featherSequence, unsigned& interleaveSequenceIndex2) {
	unsigned iSequence        = 0;
	size_t   iReverseSequence = 0;

	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPFRM;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillColor;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		form::chkseq(false);
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
			form::chkseq(false);
			interleaveSequenceIndex2++;
		}
	}
}

void xt::fthrfn(unsigned& interleaveSequenceIndex2) {
	size_t              ind = 0, res = 0;
	const double        savedSpacing = LineSpacing;
	FEATHER             feather      = {};
	std::vector<fPOINT> featherSequence;

	// ToDo - what does this function do
	PseudoRandomValue = FSED;
	xi::fthvars(feather);
	LineSpacing = SelectedForm->fillSpacing;
	satin::satfil();
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
				xi::fthrbfn(ind, feather, featherSequence);
		}
	}
	else {
		if (SelectedForm->extendedAttribute & AT_FTHBTH) {
			for (ind = 0; ind <= SequenceIndex; ind++) {
				if (!BSequence[ind].attribute)
					xi::fthdfn(ind, feather);
			}
			ind--;
		}
		else {
			for (ind = 0; ind <= SequenceIndex; ind++) {
				if (BSequence[ind].attribute) {
					if (feather.extendedAttribute & AT_FTHUP)
						xi::fthfn(ind, feather);
					else
						OSequence[ind] = BSequence[ind];
					;
				}
				else {
					if (feather.extendedAttribute & AT_FTHUP)
						OSequence[ind] = BSequence[ind];
					else
						xi::fthfn(ind, feather);
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
	xi::fritfil(featherSequence, interleaveSequenceIndex2);
}

void xt::fethrf() {
	if (FormIndex) {
		form::fvars(ClosestFormToCursor);
		clip::delclps(ClosestFormToCursor);
		texture::deltx();
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
		form::refilfn();
	}
}

void xt::fethr() {
	if (displayText::filmsgs(FMM_FTH))
		return;
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xt::fethrf();
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			xt::fethrf();
		}
	}
	StateMap.set(StateFlag::INIT);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

constexpr ULARGE_INTEGER xt::internal::tim2int(FILETIME time) noexcept {
	ULARGE_INTEGER op = { { 0, 0 } };

	op.LowPart  = time.dwLowDateTime;
	op.HighPart = time.dwHighDateTime;
	return op;
}

int xt::internal::fil2crd(const fs::path& fileName) {
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

bool xt::internal::chkp2cnam(const wchar_t* fileName) noexcept {
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

void xt::pes2crd() {
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
		thred::save();
	else {
		displayText::tabmsg(IDS_P2CNODAT);
		return;
	}
	if (xi::chkp2cnam(utf::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str())) {
		xi::fil2crd(*ThrName);
		return;
	}
	*IniFile.p2cName = 0;
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 0, KEY_READ, &registryKey)
	    == ERROR_SUCCESS) {
		size    = _MAX_PATH;
		keyType = REG_SZ;
		if (RegQueryValueEx(registryKey, L"ProgramFilesDir", 0, &keyType, (unsigned char*)programName, &size) == ERROR_SUCCESS) {
			wcscat_s(programName, L"\\Computerservice SSHSBV\\PES2Card\\LinkP2C.exe");
			if (!xi::chkp2cnam(programName))
				*programName = 0;
		}
	}
	if (!*programName) {
		LoadString(ThrEdInstance, IDS_P2CMSG, message, P2CBUFSIZ);
		LoadString(ThrEdInstance, IDS_P2CTITL, caption, P2CBUFSIZ);
		if (IDOK == MessageBox(ThrEdWindow, message, caption, MB_OKCANCEL)) {
			if (GetOpenFileName(&openFileName)) {
				if (!xi::chkp2cnam(programName))
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
	xi::fil2crd(*AuxName);
}

std::vector<fPOINT>& xt::insid() {
	unsigned iVertex = 0;

	satin::satout(fabs(SelectedForm->underlayIndent));
	if (SelectedForm->underlayIndent > 0) {
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (!form::cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y)) {
				(*InsidePoints)[iVertex] = CurrentFormVertices[iVertex];
			}
		}
		return *InsidePoints;
	}
	else
		return *OutsidePoints;
}

void xt::internal::delwlk(size_t code) {
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

void xt::internal::chkuseq(const unsigned interleaveSequenceIndex2) noexcept {
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

void xt::internal::ritwlk(unsigned& interleaveSequenceIndex2) noexcept {
	if (OutputIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = WLKMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkuseq(interleaveSequenceIndex2);
		interleaveSequenceIndex2++;
	}
}

void xt::internal::ritcwlk(unsigned& interleaveSequenceIndex2) noexcept {
	if (OutputIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = CWLKMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkuseq(interleaveSequenceIndex2);
		interleaveSequenceIndex2++;
	}
}

unsigned xt::internal::gucon(const fPOINT& start, const fPOINT& finish, unsigned destination, unsigned code) {
	double       length             = hypot(finish.x - start.x, finish.y - start.y);
	size_t       startVertex        = form::closflt(start.x, start.y);
	const size_t endVertex          = form::closflt(finish.x, finish.y);
	unsigned     stitchCount        = 0;
	size_t       intermediateVertex = 0;
	unsigned     iStitch = 0, iStep = 0;
	size_t       up = 0, down = 0;
	fPOINT       localPoint = {}, step = {}, delta = {};

	if (length < 5)
		return 0;
	if (startVertex == endVertex)
		return 0;
	const std::vector<fPOINT>& indentedPoint = xt::insid();
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
		up   = form::nxt(up);
		down = form::prv(down);
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
			intermediateVertex = form::prv(startVertex);
		else
			intermediateVertex = form::nxt(startVertex);
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
			startVertex = form::prv(startVertex);
		else
			startVertex = form::nxt(startVertex);
	}
	StitchBuffer[iStitch].x         = indentedPoint[startVertex].x;
	StitchBuffer[iStitch].y         = indentedPoint[startVertex].y;
	StitchBuffer[iStitch].attribute = code;
	iStitch++;
	return iStitch - destination;
}

void xt::internal::fnwlk(size_t find, unsigned& interleaveSequenceIndex2) {
	size_t start = 0;
	size_t count = 0;

	form::fvars(find);
	if (SelectedForm->type == FRMLINE)
		SelectedForm->type = FRMFPOLY;
	if (SelectedForm->extendedAttribute & AT_STRT && SelectedForm->type != FRMLINE)
		start = SelectedForm->fillStart;
	else
		start = 0;
	count = VertexCount;
	if (SelectedForm->type != FRMLINE)
		count++;
	const std::vector<fPOINT>& walkPoints = xt::insid();
	OutputIndex                           = 0;
	while (count) {
		OSequence[OutputIndex] = walkPoints[start];
		start                  = form::nxt(start);
		OutputIndex++;
		count--;
	}
	ritwlk(interleaveSequenceIndex2);
}

void xt::internal::ritund(unsigned& interleaveSequenceIndex2) noexcept {
	if (SequenceIndex) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = UNDMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		chkuseq(interleaveSequenceIndex2);
		interleaveSequenceIndex2++;
	}
}

void xt::internal::undclp() noexcept {
	ClipRectSize.cx = ClipRect.bottom = ClipRect.left = ClipRect.right = ClipBuffer[0].x = ClipBuffer[1].x = ClipBuffer[0].y = 0;
	ClipRectSize.cy = ClipRect.top = ClipBuffer[1].y = SelectedForm->underlayStitchLen;
	ClipStitchCount                                  = 2;
}

void xt::internal::fncwlk(unsigned& interleaveSequenceIndex2) noexcept {
	size_t iVertex = 0;
	size_t start = 0, finish = 0;

	OutputIndex = 0;
	SelectedForm->extendedAttribute |= AT_CWLK;
	if (SelectedForm->satinGuideCount) {
		if (SelectedForm->wordParam) {
			iVertex                  = SelectedForm->wordParam;
			OSequence[OutputIndex].x = form::midl(CurrentFormVertices[iVertex].x, CurrentFormVertices[iVertex + 1].x);
			OSequence[OutputIndex].y = form::midl(CurrentFormVertices[iVertex].y, CurrentFormVertices[iVertex + 1].y);
			OutputIndex++;
		}
		const SATCON* guide = SelectedForm->satinOrAngle.guide;
		if (guide) {
			for (auto iGuide = SelectedForm->satinGuideCount; iGuide != 0; iGuide--) {
				OSequence[OutputIndex].x
				    = form::midl(CurrentFormVertices[guide[iGuide - 1].finish].x, CurrentFormVertices[guide[iGuide - 1].start].x);
				OSequence[OutputIndex].y
				    = form::midl(CurrentFormVertices[guide[iGuide - 1].finish].y, CurrentFormVertices[guide[iGuide - 1].start].y);
				OutputIndex++;
			}
		}
		if (SelectedForm->attribute & FRMEND) {
			OSequence[OutputIndex].x = form::midl(CurrentFormVertices[0].x, CurrentFormVertices[1].x);
			OSequence[OutputIndex].y = form::midl(CurrentFormVertices[0].y, CurrentFormVertices[1].y);
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
		finish = form::prv(start);
		start  = form::nxt(start);
		for (size_t iGuide = 1; iGuide<VertexCount>> 1; iGuide++) {
			OSequence[OutputIndex].x = form::midl(CurrentFormVertices[finish].x, CurrentFormVertices[start].x);
			OSequence[OutputIndex].y = form::midl(CurrentFormVertices[finish].y, CurrentFormVertices[start].y);
			if (form::cisin(OSequence[OutputIndex].x, OSequence[OutputIndex].y))
				OutputIndex++;
			start  = form::nxt(start);
			finish = form::prv(finish);
		}
		OSequence[OutputIndex] = CurrentFormVertices[start];
		OutputIndex++;
	}
	ritcwlk(interleaveSequenceIndex2);
}

void xt::srtcol() {
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

void xt::dubit(unsigned bit) {
	unsigned code = 0;

	thred::savdo();
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
	form::refil();
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setuang() {
	StateMap.set(StateFlag::GTUANG);
	displayText::msgflt(IDS_UANG, IniFile.underlayAngle / PI * 180);
}

void xt::setuspac() {
	StateMap.set(StateFlag::GTUSPAC);
	displayText::msgflt(IDS_USPAC, IniFile.underlaySpacing / PFGRAN);
}

void xt::setwlkind() {
	StateMap.set(StateFlag::GTWLKIND);
	displayText::msgflt(IDS_WLKIND, IniFile.underlayIndent / PFGRAN);
}

void xt::setulen() {
	StateMap.set(StateFlag::GTWLKLEN);
	displayText::msgflt(IDS_WLKLEN, IniFile.underlayStitchLen / PFGRAN);
}

void xt::chkcwlk(unsigned& interleaveSequenceIndex2) {
	if (SelectedForm->extendedAttribute & AT_CWLK)
		xi::fncwlk(interleaveSequenceIndex2);
	else
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
}

void xt::chkwlk(unsigned& interleaveSequenceIndex2) {
	if (SelectedForm->extendedAttribute & AT_WALK)
		xi::fnwlk(ClosestFormToCursor, interleaveSequenceIndex2);
	else
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
}

void xt::internal::fnund(const std::vector<RNGCNT>& textureSegments, size_t find, unsigned& interleaveSequenceIndex2) {
	const float savedStitchSize = UserStitchLength;

	UserStitchLength = 1e99;
	if (!SelectedForm->underlaySpacing)
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	if (!SelectedForm->underlayStitchLen)
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	undclp();
	StateMap.set(StateFlag::ISUND);
	form::angclpfn(textureSegments);
	OutputIndex = SequenceIndex;
	ritund(interleaveSequenceIndex2);
	form::fvars(find);
	UserStitchLength = savedStitchSize;
}

void xt::chkund(const std::vector<RNGCNT>& textureSegments, unsigned& interleaveSequenceIndex2) {
	if (SelectedForm->extendedAttribute & AT_UND)
		xi::fnund(textureSegments, ClosestFormToCursor, interleaveSequenceIndex2);
	else
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
}

void xt::selalfrm() {
	SelectedFormList->reserve(FormIndex);
	for (auto formIndex = 0u; formIndex < FormIndex; formIndex++)
		SelectedFormList->push_back(formIndex);
	StateMap.set(StateFlag::RESTCH);
}

unsigned xt::internal::dutyp(unsigned attribute) noexcept {
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

void xt::internal::durec(OREC& record) noexcept {
	unsigned          attribute = 0;
	const fPOINTATTR* stitch    = &StitchBuffer[record.start];

	record.type  = StitchTypes[dutyp(stitch->attribute)];
	attribute    = stitch->attribute & SRTMSK;
	record.color = attribute & 0xf;
	record.form  = (attribute & FRMSK) >> FRMSHFT;
}

bool xt::internal::recmp(const OREC* record1, const OREC* record2) noexcept {
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

bool xt::internal::refcmp(const OREC* record1, const OREC* record2) noexcept {
	if (record1->form == record2->form) {
		return (record1->type < record2->type);
	}
	else {
		return (record1->form < record2->form);
	}
}

bool xt::internal::chkrdun(const std::vector<unsigned>& formFillCounter,
                           const std::vector<OREC*>&    pRecs,
                           const SRTREC&                stitchRecord) {
	unsigned iStitch;

	for (iStitch = stitchRecord.start; iStitch < stitchRecord.finish; iStitch++) {
		if (pRecs[iStitch]->otyp == formFillCounter[pRecs[iStitch]->form])
			return 1;
	}
	return 0;
}

double
xt::internal::precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord) {
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

unsigned xt::internal::duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord) {
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

void xt::internal::dmprec(const std::vector<OREC*>& stitchRegion, unsigned count) {
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

bool xt::internal::srtchk(const std::vector<OREC*>& stitchRegion, unsigned count, unsigned& badForm) {
	unsigned iRegion = 1;
	unsigned form    = stitchRegion[0]->form;
	unsigned color   = stitchRegion[0]->color;

	for (iRegion = 1; iRegion < count; iRegion++) {
		if (stitchRegion[iRegion]->form == form) {
			if (ColorOrder[stitchRegion[iRegion]->color] < ColorOrder[color]) {
				const auto& formHeader = FormList[form];
				if (formHeader.fillType == FTHF && formHeader.extendedAttribute & AT_FTHBLND
				    && stitchRegion[iRegion]->color == formHeader.fillColor)
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

void xt::fsort() {
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
	thred::savdo();
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
		xi::durec(stitchRegion[iRegion]);
		pRecs[iRegion]  = &stitchRegion[iRegion];
		pFRecs[iRegion] = &stitchRegion[iRegion];
	}
	std::sort(pRecs.begin(), pRecs.end(), xi::recmp);
	std::sort(pFRecs.begin(), pFRecs.end(), xi::refcmp);
#ifdef _DEBUG
	xi::dmprec(pRecs, lastRegion);
#endif
	if (xi::srtchk(pFRecs, lastRegion, badForm)) {
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
			startTime = xi::tim2int(fileTime);
			for (iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
				sortRecord.currentRegion = iRegion;
				if (!pRecs[iRegion]->otyp) {
					jumps = xi::duprecs(tempStitchBuffer, pRecs, sortRecord);
					if (jumps < minimumJumps) {
						minimumJumps     = jumps;
						minimumIndex     = iRegion;
						minimumDirection = sortRecord.direction;
					}
				}
				GetSystemTimeAsFileTime(&fileTime);
				nextTime = xi::tim2int(fileTime);
				if (nextTime.QuadPart - startTime.QuadPart > SRTIM)
					break;
			}
			StateMap.set(StateFlag::DUSRT);
			sortRecord.currentRegion = minimumIndex;
			sortRecord.direction     = minimumDirection;
			xi::precjmps(tempStitchBuffer, pRecs, sortRecord);
		}
		std::copy(tempStitchBuffer.cbegin(), tempStitchBuffer.cbegin() + OutputIndex, StitchBuffer);
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(OutputIndex);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		std::wstring str;
		displayText::loadString(str, IDS_SRTER);
		displayText::shoMsg(fmt::format(str, pFRecs[badForm]->form));
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

void xt::internal::duatf(unsigned ind) {
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

void xt::dmpat() {
	unsigned iStitch = 0, attribute = StitchBuffer[0].attribute;

	xi::duatf(0);
	for (iStitch = 1; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (attribute != StitchBuffer[iStitch].attribute) {
			xi::duatf(iStitch);
			attribute = StitchBuffer[iStitch].attribute;
		}
	}
}
#endif

void xt::fdelstch(FILLSTARTS& fillStartsData, unsigned& fillStartsMap) {
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
			type = StitchTypes[xi::dutyp(attribute)];
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

bool xt::internal::lastcol(unsigned index, fPOINT& point) noexcept {
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

void xt::internal::duint(unsigned offset, unsigned code, INTINF& ilData) {
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

bool xt::internal::isfil() noexcept {
	if (SelectedForm->fillType)
		return 1;
	if (SelectedForm->edgeType)
		return 1;
	if (SelectedForm->extendedAttribute & (AT_CWLK | AT_WALK | AT_UND))
		return 1;
	return 0;
}

void xt::internal::chkend(unsigned offset, unsigned code, INTINF& ilData) {
	if (isfil()) {
		StateMap.set(StateFlag::ISEND);
		if (SelectedForm->extendedAttribute & AT_END)
			ilData.output += gucon(InterleaveSequence[InterleaveSequenceIndex - 1],
			                       CurrentFormVertices[SelectedForm->fillEnd],
			                       ilData.output + offset,
			                       code);
	}
}

void xt::intlv(const FILLSTARTS& fillStartsData, unsigned fillStartsMap, const unsigned interleaveSequenceIndex2) {
	unsigned ine = 0, code = 0, offset = 0;
	fPOINT   colpnt = {};
	INTINF   ilData = {};

	StateMap.reset(StateFlag::ISEND);
	form::fvars(ClosestFormToCursor);
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
			xi::duint(offset, code, ilData);
		}
		xi::chkend(MAXITEMS, code, ilData);
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
					ilData.output += xi::gucon(CurrentFormVertices[SelectedForm->fillStart],
					                           InterleaveSequence[InterleaveSequenceIndices[ilData.pins].index],
					                           ilData.output + offset,
					                           code);
			}
			if (xi::lastcol(iSequence, colpnt))
				ilData.output
				    += xi::gucon(colpnt, InterleaveSequence[InterleaveSequenceIndices[iSequence].index], ilData.output, code);
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
		xi::chkend(0, code, ilData);
	}
	PCSHeader.stitchCount = ilData.output;
	thred::coltab();
}

void xt::internal::setundfn(unsigned code) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		form::fvars(ClosestFormToCursor);
		if (SelectedForm->type != FRMLINE) {
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute |= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				form::refilfn();
			}
		}
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE) {
				continue;
			}
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute |= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				form::refilfn();
			}
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setund() {
	xi::setundfn(AT_UND);
}

void xt::setwlk() {
	xi::setundfn(AT_WALK);
}

void xt::setcwlk() {
	xi::setundfn(AT_CWLK);
}

void xt::internal::notundfn(unsigned code) {
	thred::savdo();
	code = ~code;
	if (StateMap.test(StateFlag::FORMSEL)) {
		form::fvars(ClosestFormToCursor);
		if (SelectedForm->type != FRMLINE) {
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute &= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				form::refilfn();
			}
		}
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			if (SelectedForm->type == FRMLINE) {
				continue;
			}
			const auto savedAttribute = SelectedForm->extendedAttribute;
			SelectedForm->extendedAttribute &= code;
			if (savedAttribute != SelectedForm->extendedAttribute) {
				form::refilfn();
			}
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::notund() {
	xi::notundfn(AT_UND);
}

void xt::notwlk() {
	xi::notundfn(AT_WALK);
}

void xt::notcwlk() {
	xi::notundfn(AT_CWLK);
}

void xt::internal::ulenfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
		SelectedForm->underlayStitchLen = length;
		form::refilfn();
	}
}

void xt::dusulen(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::ulenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::ulenfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::undlen() {
	displayText::tabmsg(IDS_SETULEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETULEN);
	displayText::numWnd();
}

void xt::internal::uspacfn(size_t find, float spacing) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & AT_UND) {
		SelectedForm->underlaySpacing = spacing;
		form::refilfn();
	}
}

void xt::duspac(float spacing) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::uspacfn(ClosestFormToCursor, spacing);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::uspacfn(selectedForm, spacing);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::uspac() {
	displayText::tabmsg(IDS_SETUSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUSPAC);
	displayText::numWnd();
}

void xt::internal::uangfn(size_t find, float angle) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & AT_UND) {
		SelectedForm->underlayStitchAngle = angle;
		form::refilfn();
	}
}

void xt::dufang(float angle) {
	thred::savdo();
	angle *= (float)PI / 180;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::uangfn(ClosestFormToCursor, angle);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::uangfn(selectedForm, angle);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::sfuang() {
	displayText::tabmsg(IDS_SETUANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUANG);
	displayText::numWnd();
}

void xt::internal::flenfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType && !clip::isclp(find)) {
		SelectedForm->lengthOrCount.stitchLength = length;
		form::refilfn();
	}
}

void xt::duflen(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::flenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::flenfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setflen() {
	displayText::tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFLEN);
	displayText::numWnd();
}

void xt::internal::fspacfn(size_t find, float spacing) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		if (spacing < 0) {
			if (!clip::isclp(find))
				return;
		}
		SelectedForm->fillSpacing = spacing;
		form::refilfn();
	}
}

void xt::dufspac(float spacing) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fspacfn(ClosestFormToCursor, spacing);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fspacfn(selectedForm, spacing);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfspac() {
	displayText::tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFSPAC);
	displayText::numWnd();
}

void xt::internal::findfn(size_t find, float indent) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	SelectedForm->underlayIndent = indent;
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK))
		form::refilfn();
}

void xt::dufind(float indent) {
	thred::savdo();
	indent *= PFGRAN;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::findfn(ClosestFormToCursor, indent);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::findfn(selectedForm, indent);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::internal::fangfn(size_t find, float angle) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
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
		form::refilfn();
	}
}

void xt::dufxang(float angle) {
	thred::savdo();
	angle *= (float)PI / 180;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fangfn(ClosestFormToCursor, angle);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fangfn(selectedForm, angle);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfang() {
	displayText::tabmsg(IDS_SETFANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFANG);
	displayText::numWnd();
}

void xt::internal::ucolfn(size_t find, unsigned color) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) {
		SelectedForm->underlayColor = color;
		form::refilfn();
	}
}

void xt::dundcol(unsigned color) {
	thred::savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::ucolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::ucolfn(selectedForm, color);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setucol() {
	displayText::tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUCOL);
	displayText::numWnd();
}

void xt::internal::fcolfn(size_t find, unsigned color) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->fillColor = color;
		form::refilfn();
	}
}

void xt::dufcol(unsigned color) {
	thred::savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fcolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fcolfn(selectedForm, color);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfcol() {
	displayText::tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFCOL);
	displayText::numWnd();
}

void xt::internal::bcolfn(size_t find, unsigned color) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->borderColor = color;
		form::refilfn();
	}
}

void xt::dubcol(unsigned color) {
	thred::savdo();
	if (color)
		color--;
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bcolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bcolfn(selectedForm, color);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbcol() {
	displayText::tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBCOL);
	displayText::numWnd();
}

void xt::internal::blenfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType && !clip::iseclp(find)) {
		SelectedForm->lengthOrCount.stitchLength = length;
		form::refilfn();
	}
}

void xt::dublen(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::blenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::blenfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setblen() {
	displayText::tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBLEN);
	displayText::numWnd();
}

void xt::internal::bspacfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->edgeSpacing = length;
		form::refilfn();
	}
}

void xt::dubspac(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bspacfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bspacfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbspac() {
	displayText::tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBSPAC);
	displayText::numWnd();
}

void xt::internal::bminfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->minBorderStitchLen = length;
		form::refilfn();
	}
}

void xt::dubmin(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bminfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bminfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbmin() {
	displayText::tabmsg(IDS_TXT23);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMIN);
	displayText::numWnd();
}

void xt::internal::bmaxfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType) {
		SelectedForm->maxBorderStitchLen = length;
		form::refilfn();
	}
}

void xt::dubmax(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bmaxfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bmaxfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbmax() {
	displayText::tabmsg(IDS_TXT22);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMAX);
	displayText::numWnd();
}

void xt::internal::fminfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->minFillStitchLen = length;
		form::refilfn();
	}
}

void xt::dufmin(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fminfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fminfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfmin() {
	displayText::tabmsg(IDS_TXT21);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMIN);
	displayText::numWnd();
}

void xt::internal::fmaxfn(size_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType) {
		SelectedForm->maxFillStitchLen = length;
		form::refilfn();
	}
}

void xt::dufmax(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fmaxfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fmaxfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfmax() {
	displayText::tabmsg(IDS_TXT20);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMAX);
	displayText::numWnd();
}

void xt::internal::fwidfn(size_t find, float length) {
	unsigned iVertex   = 0;
	double   ratio     = 0.0;
	float    reference = 0.0;

	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	reference = SelectedForm->rectangle.left;
	ratio     = length / (SelectedForm->rectangle.right - reference);
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
		CurrentFormVertices[iVertex].x = (CurrentFormVertices[iVertex].x - reference) * ratio + reference;
	form::frmout(ClosestFormToCursor);
	form::refilfn();
}

void xt::dufwid(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fwidfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fwidfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfwid() {
	displayText::tabmsg(IDS_WID);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFWID);
	displayText::numWnd();
}

void xt::setfind() {
	displayText::tabmsg(IDS_UWLKIND);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFIND);
	displayText::numWnd();
}

void xt::internal::fhifn(size_t find, float length) {
	unsigned iVertex   = 0;
	double   ratio     = 0.0;
	float    reference = 0.0;

	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	reference = SelectedForm->rectangle.bottom;
	ratio     = length / (SelectedForm->rectangle.top - reference);
	for (iVertex = 0; iVertex < VertexCount; iVertex++)
		CurrentFormVertices[iVertex].y = (CurrentFormVertices[iVertex].y - reference) * ratio + reference;
	form::frmout(ClosestFormToCursor);
	form::refilfn();
}

void xt::dufhi(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fhifn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fhifn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfhi() {
	displayText::tabmsg(IDS_HI);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFHI);
	displayText::numWnd();
}

void xt::setfilstrt() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		FormList[ClosestFormToCursor].fillStart = gsl::narrow<unsigned short>(ClosestVertexToCursor);
		FormList[ClosestFormToCursor].extendedAttribute |= AT_STRT;
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		displayText::shoseln(IDS_FORMP, IDS_FSTRT);
}

void xt::setfilend() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		FormList[ClosestFormToCursor].fillEnd = gsl::narrow<unsigned short>(ClosestVertexToCursor);
		FormList[ClosestFormToCursor].extendedAttribute |= AT_END;
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		displayText::shoseln(IDS_FORMP, IDS_FEND);
}

void xt::duauxnam() {
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

void xt::internal::rtrclpfn() {
	size_t count = 0;

	if (OpenClipboard(ThrEdWindow)) {
		form::fvars(ClosestFormToCursor);
		if (clip::iseclp(ClosestFormToCursor)) {
			count = SelectedForm->clipEntries;
			clip::oclp(SelectedForm->borderClipData, count);
		}
		else {
			if (clip::isclp(ClosestFormToCursor)) {
				count = SelectedForm->lengthOrCount.clipCount;
				clip::oclp(SelectedForm->angleOrClipData.clip, count);
			}
		}
		if (count) {
			LowerLeftStitch = {};
			EmptyClipboard();
			Clip        = RegisterClipboardFormat(PcdClipFormat);
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, count * sizeof(CLPSTCH) + 2);
			if (ClipPointer) {
				ClipStitchData = *(static_cast<CLPSTCH**>(ClipPointer));
				thred::rtclpfn(0, 0);
				ClipStitchData[0].led = count;
				for (size_t iStitch = 1; iStitch < count; iStitch++)
					thred::rtclpfn(iStitch, iStitch);
				SetClipboardData(Clip, ClipPointer);
			}
			CloseClipboard();
		}
	}
}

void xt::rtrclp() {
	if (StateMap.test(StateFlag::FORMSEL)) {
		form::fvars(ClosestFormToCursor);
		if (texture::istx(ClosestFormToCursor))
			texture::rtrtx();
		else
			xi::rtrclpfn();
	}
}

void xt::internal::setstxt(unsigned stringIndex, float value, HWND dialog) {
	SetWindowText(GetDlgItem(dialog, stringIndex), fmt::format(L"{:.2f}", (value / PFGRAN)).c_str());
}

float xt::internal::getstxt(unsigned stringIndex, HWND dialog) {
	// ToDo - This is not great code.
	wchar_t buffer[16] = {};
	GetWindowText(GetDlgItem(dialog, stringIndex), buffer, sizeof(buffer) / sizeof(buffer[0]));
	return std::stof(buffer) * PFGRAN;
}

bool xt::internal::chkasp(fPOINT& point, float aspectRatio, HWND dialog) {
	point.x = getstxt(IDC_DESWID, dialog);
	point.y = getstxt(IDC_DESHI, dialog);
	if ((point.y / point.x) == aspectRatio)
		return 1;
	else
		return 0;
}

BOOL CALLBACK xt::internal::setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
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
void xt::internal::sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	stitch.x = (stitch.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	stitch.y = (stitch.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xt::internal::sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	point.x = (point.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	point.y = (point.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xt::internal::nudfn(const fRECTANGLE& designSizeRect) noexcept {
	unsigned iStitch = 0, iVertex = 0;
	fPOINT   newSize = { (designSizeRect.right - designSizeRect.left), (designSizeRect.top - designSizeRect.bottom) };

	newSize.x              = designSizeRect.right - designSizeRect.left;
	newSize.y              = designSizeRect.top - designSizeRect.bottom;
	dPOINT designSizeRatio = { (DesignSize.x / newSize.x), (DesignSize.y / newSize.y) };
	for (iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++)
		sadj(StitchBuffer[iStitch], designSizeRatio, designSizeRect);
	for (iVertex = 0; iVertex < FormVertexIndex; iVertex++)
		sadj(FormVertices[iVertex], designSizeRatio, designSizeRect);
	form::frmout(ClosestFormToCursor);
}

void xt::nudsiz() {
	int        flag           = 0;
	unsigned   iForm          = 0;
	fRECTANGLE designSizeRect = {}; // design size rectangle

	thred::savdo();
	flag = 0;
	if (PCSHeader.stitchCount) {
		thred::stchrct(designSizeRect);
		flag = 1;
	}
	else {
		if (FormIndex) {
			thred::frmrct(designSizeRect);
			flag = 2;
		}
	}
	if (flag) {
		DesignSize.x = designSizeRect.right - designSizeRect.left;
		DesignSize.y = designSizeRect.top - designSizeRect.bottom;
		if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, (DLGPROC)xi::setsprc)) {
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
			xi::nudfn(designSizeRect);
			if (UserFlagMap.test(UserFlag::CHREF))
				form::refilal();
			if (flag) {
				thred::movStch();
				thred::zumhom();
				displayText::hsizmsg();
			}
			form::centir();
			for (iForm = 0; iForm < FormIndex; iForm++)
				form::frmout(iForm);
		}
	}
}

void xt::dushft() {
	//	StateMap.set(StateFlag::BOXSLCT);
	StateMap.set(StateFlag::BZUMIN);
	//	StateMap.set(StateFlag::NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	thred::px2stch();
	ZoomBoxOrigin = SelectedPoint;
}

void xt::mvshft() {
	if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT))
		SetCapture(ThrEdWindow);
	if (StateMap.test(StateFlag::BZUMIN)) {
		if (Msg.wParam & MK_LBUTTON) {
			if (StateMap.testAndSet(StateFlag::VCAPT))
				SetCapture(ThrEdWindow);
			thred::unbBox();
			ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::BZUM);
			thred::bBox();
		}
	}
}

void xt::setclpspac() {
	displayText::msgflt(IDS_CLPSPAC, IniFile.clipOffset / PFGRAN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::SCLPSPAC);
	displayText::numWnd();
}

BOOL CALLBACK xt::internal::enumch(HWND hwnd, LPARAM lParam) noexcept {
	UNREFERENCED_PARAMETER(lParam);

	DestroyWindow(hwnd);
	return 1;
}

void xt::clrstch() noexcept {
	while (EnumChildWindows(MainStitchWin, xi::enumch, 0))
		;
}

void xt::chgwrn() {
	UserFlagMap.flip(UserFlag::WRNOF);
	thred::wrnmen();
	StateMap.set(StateFlag::DUMEN);
}

void xt::chgchk(int code) {
	IniFile.dataCheck = code;
	thred::chkmen();
	StateMap.set(StateFlag::DUMEN);
}

void xt::tst() {
	DesignerName->assign(L"Coder");
	ThrName->assign(*DesignerName);
	StateMap.set(StateFlag::RESTCH);
}
