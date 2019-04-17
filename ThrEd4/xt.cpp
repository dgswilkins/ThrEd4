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
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
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
uint32_t ColorOrder[16]; // color order adjusted for applique

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
	if (IniFile.dataCheck != 0u) {
		StateMap.set(StateFlag::FCHK);
	}
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
	if ((feather.extendedAttribute & AT_FTHBLND) != 0u) {
		StateMap.set(StateFlag::BARSAT);
	}
	else {
		StateMap.set(StateFlag::FTHR);
	}
}

constexpr float xt::internal::durat(float start, float finish, float featherRatio) {
	return (finish - start) * featherRatio + start;
}

void xt::internal::duxrats(uint32_t start, uint32_t finish, fPOINT& point, float featherRatioLocal) {
	point.x = durat((*BSequence)[finish].x, (*BSequence)[start].x, featherRatioLocal);
	point.y = durat((*BSequence)[finish].y, (*BSequence)[start].y, featherRatioLocal);
}

void xt::internal::durats(uint32_t iSequence, std::vector<fPOINT>* sequence, FEATHER& feather) {
	if (sequence != nullptr) {
		auto&      bCurrent     = (*BSequence)[iSequence];
		auto&      bNext        = (*BSequence)[wrap::toSize(iSequence) + 1u];
		const auto stitchLength = hypot(bNext.x - bCurrent.x, bNext.y - bCurrent.y);

		if (stitchLength < feather.minStitch) {
			sequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
		}
		else {
			feather.ratioLocal = feather.minStitch / stitchLength;
			const auto adjustedPoint
			    = fPOINT { durat(bNext.x, bCurrent.x, feather.ratioLocal), durat(bNext.y, bCurrent.y, feather.ratioLocal) };

			sequence->push_back(
			    fPOINT { durat(adjustedPoint.x, bCurrent.x, feather.ratio), durat(adjustedPoint.y, bCurrent.y, feather.ratio) });
		}
	}
}

uint32_t xt::internal::bpsg() noexcept {
	auto testValue = 0u;

	if (PseudoRandomValue == 0u) {
		PseudoRandomValue = FSED;
	}
	testValue = PseudoRandomValue & 0x40000008;
	PseudoRandomValue >>= 1u;
	if (testValue == 0x8 || testValue == 0x40000000) {
		PseudoRandomValue |= 0x40000000;
	}
	return PseudoRandomValue;
}

void xt::internal::nurat(FEATHER& feather) {
	const auto remainder = fmod(feather.globalPosition, 1);
	switch (feather.fillType) {
	case FTHPSG: {
		if (feather.upCount != 0u) {
			if (feather.countUp != 0u) {
				feather.ratio
				    = (gsl::narrow_cast<float>(feather.totalCount) - (form::psg() % feather.totalCount)) / feather.totalCount;
				feather.countUp--;
			}
			else {
				feather.ratio
				    = (gsl::narrow_cast<float>(feather.totalCount) - (bpsg() % feather.totalCount)) / feather.totalCount;
				if (feather.countDown != 0u) {
					feather.countDown--;
				}
				else {
					PseudoRandomValue = FSED;
					feather.countUp   = feather.upCount;
					feather.countDown = feather.downCount;
				}
			}
		}
		else {
			feather.ratio
			    = (gsl::narrow_cast<float>(feather.totalCount) - (form::psg() % feather.totalCount)) / feather.totalCount;
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHFAZ: {
		if (feather.phase >= feather.upCount) {
			feather.ratio = 1;
		}
		else {
			feather.ratio = feather.formRatio;
		}
		break;
	}
	case FTHSIN: {
		if (remainder > feather.globalRatio) {
			feather.ratio = sin((1.0 - remainder) / (1.0 - feather.globalRatio) * PI + PI) * 0.5 + 0.5;
		}
		else {
			feather.ratio = sin(remainder / feather.globalRatio * PI) * 0.5 + 0.5;
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHSIN2: {
		if (remainder > feather.globalRatio) {
			feather.ratio = sin((1.0 - remainder) / (1.0 - feather.globalRatio) * PI);
		}
		else {
			feather.ratio = sin(remainder / feather.globalRatio * PI);
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHRMP: {
		if (remainder > feather.globalRatio) {
			feather.ratio = (1.0 - remainder) / (1.0 - feather.globalRatio);
		}
		else {
			feather.ratio = remainder / feather.globalRatio;
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHLIN:
	default: {
		feather.ratio = feather.formRatio;
		break;
	}
	}
	++feather.phase %= feather.phaseIndex;
	feather.globalPosition += feather.globalStep;
}

void xt::internal::fthfn(uint32_t iSequence, FEATHER& feather) {
	nurat(feather);
	durats(iSequence, OSequence, feather);
}

void xt::internal::ratpnt(uint32_t iPoint, uint32_t iNextPoint, fPOINT& point, float featherRatio) {
	point.x = ((*BSequence)[iNextPoint].x - (*BSequence)[iPoint].x) * featherRatio + (*BSequence)[iPoint].x;
	point.y = ((*BSequence)[iNextPoint].y - (*BSequence)[iPoint].y) * featherRatio + (*BSequence)[iPoint].y;
}

fPOINT xt::internal::midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept {
	return fPOINT { form::midl(endPoint.x, startPoint.x), form::midl(endPoint.y, startPoint.y) };
}

void xt::internal::xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = (endPoint.x - startPoint.x) * featherRatioLocal + startPoint.x;
	point.y = (endPoint.y - startPoint.y) * featherRatioLocal + startPoint.y;
}

void xt::internal::fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence) {
	auto       currentPoint = fPOINT {};
	auto       nextPoint    = fPOINT {};
	auto       midPoint     = fPOINT {};
	auto&      bCurrent     = (*BSequence)[iSequence];
	auto&      bNext        = (*BSequence)[wrap::toSize(iSequence) + 1u];
	const auto length       = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);

	nurat(feather);
	if (length < (2.0 * feather.minStitch)) {
		feather.ratio = 0.5;
		ratpnt(iSequence, iSequence + 1, currentPoint, feather.ratio);
		ratpnt(iSequence + 3, iSequence + 2, nextPoint, feather.ratio);
	}
	else {
		auto currentHighPoint = fPOINT {};
		auto currentLowPoint  = fPOINT {};
		auto nextHighPoint    = fPOINT {};
		auto nextLowPoint     = fPOINT {};
		feather.ratioLocal    = feather.minStitch / length;
		duxrats(iSequence, iSequence + 1, currentLowPoint, feather.ratioLocal);
		duxrats(iSequence + 3, iSequence + 2, nextLowPoint, feather.ratioLocal);
		feather.ratioLocal = 1 - feather.ratioLocal;
		duxrats(iSequence, iSequence + 1, currentHighPoint, feather.ratioLocal);
		duxrats(iSequence + 3, iSequence + 2, nextHighPoint, feather.ratioLocal);
		feather.ratioLocal = feather.ratio;
		xratf(currentLowPoint, currentHighPoint, currentPoint, feather.ratioLocal);
		xratf(nextLowPoint, nextHighPoint, nextPoint, feather.ratioLocal);
	}
	midPoint = midpnt(currentPoint, nextPoint);
	OSequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
	OutputIndex++;
	OSequence->push_back(midPoint);
	OutputIndex++;
	featherSequence.emplace_back(bNext.x, bNext.y);
	featherSequence.push_back(midPoint);
}

void xt::internal::fthdfn(uint32_t iSequence, FEATHER& feather) {
	auto&      bCurrent = (*BSequence)[iSequence];
	auto&      bNext    = (*BSequence)[wrap::toSize(iSequence) + 1u];
	const auto length   = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);

	nurat(feather);
	OSequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
	OSequence->push_back(fPOINT { bNext.x, bNext.y });
	if (length > feather.minStitch) {
		auto adjustedPoint = fPOINT {};
		auto currentPoint  = fPOINT {};
		auto nextPoint     = fPOINT {};
		feather.ratioLocal = 0.5;
		duxrats(iSequence + 1, iSequence, adjustedPoint, feather.ratioLocal);
		feather.ratioLocal = feather.minStitch / length / 2;
		xratf(adjustedPoint, (*OSequence)[iSequence], currentPoint, feather.ratioLocal);
		xratf(adjustedPoint, (*OSequence)[wrap::toSize(iSequence) + 1], nextPoint, feather.ratioLocal);
		feather.ratioLocal = feather.ratio;
		xratf(currentPoint, (*OSequence)[iSequence], (*OSequence)[iSequence], feather.ratioLocal);
		xratf(
		    nextPoint, (*OSequence)[wrap::toSize(iSequence) + 1], (*OSequence)[wrap::toSize(iSequence) + 1], feather.ratioLocal);
	}
}

void xt::internal::fritfil(std::vector<fPOINT>& featherSequence) {
	if (!OSequence->empty()) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { TYPFRM, SelectedForm->fillColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		form::chkseq(false);
		if (((SelectedForm->extendedAttribute & AT_FTHBLND) != 0u)
		    && ~(SelectedForm->extendedAttribute & (AT_FTHUP | AT_FTHBTH)) != (AT_FTHUP | AT_FTHBTH)) {
			InterleaveSequenceIndices->emplace_back(
			    INSREC { FTHMSK, SelectedForm->fillInfo.feather.color, wrap::toUnsigned(InterleaveSequence->size()), I_FTH });

			const auto sequenceMax      = wrap::toUnsigned(featherSequence.size());
			auto       iReverseSequence = sequenceMax - 1;
			for (auto iSequence = 0u; iSequence < sequenceMax; iSequence++) {
				(*OSequence)[iSequence] = featherSequence[iReverseSequence];
				iReverseSequence--;
			}
			OSequence->resize(sequenceMax);
			form::chkseq(false);
		}
	}
}

void xt::fthrfn() {
	const auto savedSpacing = LineSpacing;
	auto       feather      = FEATHER {};

	auto featherSequence = std::vector<fPOINT> {};

	// ToDo - what does this function do
	PseudoRandomValue = FSED;
	xi::fthvars(feather);
	LineSpacing = SelectedForm->fillSpacing;
	satin::satfil();
	(*BSequence)[0].attribute = 0;
	(*BSequence)[1].attribute = 1;
	if (feather.phaseIndex == 0u) {
		feather.phaseIndex = 1;
	}
	auto       ind = gsl::narrow_cast<uint32_t>(BSequence->size()) / (feather.phaseIndex << 2u);
	const auto res = gsl::narrow_cast<uint32_t>(BSequence->size()) % (feather.phaseIndex << 2u);
	if (res > (feather.phaseIndex << 1u)) {
		ind++;
	}
	feather.globalPosition = 0;
	feather.globalStep     = 4.0 / BSequence->size() * ind;
	feather.globalPhase    = gsl::narrow_cast<float>(BSequence->size()) / ind;
	feather.globalRatio    = gsl::narrow_cast<float>(feather.countUp) / feather.phaseIndex;
	feather.globalUp       = feather.globalPhase * feather.globalRatio;
	feather.globalDown     = feather.globalPhase - feather.globalUp;
	SelectedForm->fillType = FTHF;
	feather.phase          = 1;
	BSequence->push_back((*BSequence)[BSequence->size() - 2]);
	BSequence->push_back((*BSequence)[BSequence->size() - 1]);
	if ((feather.extendedAttribute & AT_FTHBLND) != 0u) {
		OutputIndex = 0;
		for (ind = 0; ind < wrap::toUnsigned(BSequence->size()) - 2; ind++) {
			if ((*BSequence)[ind].attribute == 0) {
				xi::fthrbfn(ind, feather, featherSequence);
			}
		}
	}
	else {
		if ((SelectedForm->extendedAttribute & AT_FTHBTH) != 0u) {
			for (ind = 0; ind <= BSequence->size() - 2; ind++) {
				if ((*BSequence)[ind].attribute == 0) {
					xi::fthdfn(ind, feather);
				}
			}
		}
		else {
			for (ind = 0; ind <= BSequence->size() - 2; ind++) {
				if ((*BSequence)[ind].attribute != 0) {
					if ((feather.extendedAttribute & AT_FTHUP) != 0u) {
						xi::fthfn(ind, feather);
					}
					else {
						OSequence->push_back(fPOINT { (*BSequence)[ind].x, (*BSequence)[ind].y });
					}
				}
				else {
					if ((feather.extendedAttribute & AT_FTHUP) != 0u) {
						OSequence->push_back(fPOINT { (*BSequence)[ind].x, (*BSequence)[ind].y });
					}
					else {
						xi::fthfn(ind, feather);
					}
				}
			}
		}
		OutputIndex = ind;
	}
	StateMap.reset(StateFlag::FTHR);
	StateMap.reset(StateFlag::BARSAT);
	LineSpacing = savedSpacing;
	xi::fritfil(featherSequence);
}

void xt::fethrf() {
	if (!FormList->empty()) {
		form::fvars(ClosestFormToCursor);
		clip::delmclp(ClosestFormToCursor);
		texture::deltx(ClosestFormToCursor);
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
		SelectedForm->fillColor                  = gsl::narrow<uint8_t>(ActiveColor);
		SelectedForm->fillInfo.feather.color     = (ActiveColor + 1) & COLMSK;
		SelectedForm->fillType                   = FTHF;
		form::refilfn();
	}
}

void xt::fethr() {
	if (displayText::filmsgs(FMM_FTH)) {
		return;
	}
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
	auto op = ULARGE_INTEGER { { 0, 0 } };

	op.LowPart  = time.dwLowDateTime;
	op.HighPart = time.dwHighDateTime;
	return op;
}

int32_t xt::internal::fil2crd(const fs::path& fileName) {
	auto    startupInfo = STARTUPINFO {};
	auto    processInfo = PROCESS_INFORMATION {};
	auto    errorCode   = 0;
	wchar_t command[_MAX_PATH * 2 + 1 + 4];

	wcscpy_s(command, L"\"");
	wcscpy_s(command, utf::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str());
	wcscpy_s(command, L"\" \"");
	wcscpy_s(command, fileName.wstring().c_str());
	wcscpy_s(command, L"\"");
	startupInfo    = {};
	startupInfo.cb = sizeof(startupInfo);
	if (!CreateProcess(
	        nullptr, command, nullptr, nullptr, 0, NORMAL_PRIORITY_CLASS, nullptr, nullptr, &startupInfo, &processInfo)) {
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
	auto handleP2C = CreateFile(fileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (handleP2C == INVALID_HANDLE_VALUE) {
		return false;
	}

	CloseHandle(handleP2C);
	return true;
}

void xt::pes2crd() {
	constexpr auto P2CBUFSIZ = 256;

	auto          registryKey            = HKEY {};
	wchar_t       programName[_MAX_PATH] = { 0 };
	const wchar_t filter[]               = L"ComputerService (Lind2PC.exe)\0LinkP2C.exe\0\0";
	wchar_t       message[P2CBUFSIZ]     = { 0 };
	wchar_t       caption[P2CBUFSIZ]     = { 0 };
	auto          openFileName           = OPENFILENAME {
        sizeof(OPENFILENAME), // lStructsize
        ThrEdWindow,          // hwndOwner
        ThrEdInstance,        // hInstance
        filter,               // lpstrFilter
        nullptr,              // lpstrCustomFilter
        0,                    // nMaxCustFilter
        0,                    // nFilterIndex
        programName,          // lpstrFile
        _MAX_PATH,            // nMaxFile
        nullptr,              // lpstrFileTitle
        0,                    // nMaxFileTitle
        L"C:\\",              // lpstr	ialDir
        nullptr,              // lpstrTitle
        0,                    // Flags
        0,                    // nFileOffset
        0,                    // nFileExtension
        L"exe",               // lpstrDefExt
        0,                    // lCustData
        nullptr,              // lpfnHook
        nullptr,              // lpTemplateName
	};

	if (PCSHeader.stitchCount != 0u) {
		thred::save();
	}
	else {
		displayText::tabmsg(IDS_P2CNODAT);
		return;
	}
	if (xi::chkp2cnam(utf::Utf8ToUtf16(std::string(IniFile.p2cName)).c_str())) {
		xi::fil2crd(*ThrName);
		return;
	}
	*IniFile.p2cName = 0;
	// ToDo - fix registry handling
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion", 0, KEY_READ, &registryKey)
	    == ERROR_SUCCESS) {
		auto size    = DWORD { _MAX_PATH };
		auto keyType = DWORD { REG_SZ };
		GSL_SUPPRESS(26490) {
			if (RegQueryValueEx(registryKey, L"ProgramFilesDir", nullptr, &keyType, reinterpret_cast<LPBYTE>(programName), &size)
			    == ERROR_SUCCESS) {
				wcscat_s(programName, L"\\Computerservice SSHSBV\\PES2Card\\LinkP2C.exe");
				if (!xi::chkp2cnam(programName)) {
					*programName = 0;
				}
			}
		}
	}
	if (*programName == 0u) {
		LoadString(ThrEdInstance, IDS_P2CMSG, message, P2CBUFSIZ);
		LoadString(ThrEdInstance, IDS_P2CTITL, caption, P2CBUFSIZ);
		if (IDOK == MessageBox(ThrEdWindow, message, caption, MB_OKCANCEL)) {
			if (GetOpenFileName(&openFileName)) {
				if (!xi::chkp2cnam(programName)) {
					return;
				}
			}
			else {
				return;
			}
		}
		else {
			return;
		}
	}
	auto p2cName = utf::Utf16ToUtf8(std::wstring(programName));
	std::copy(p2cName.cbegin(), p2cName.cend(), IniFile.p2cName);
	xi::fil2crd(*AuxName);
}

#pragma warning(push)
#pragma warning(disable : 26487)
std::vector<fPOINT>& xt::insid() {
	satin::satout(fabs(SelectedForm->underlayIndent));
	if (SelectedForm->underlayIndent > 0) {
		auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			if (!form::cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y)) {
				(*InsidePoints)[iVertex] = vertexIt[iVertex];
			}
		}
		return *InsidePoints;
	}

	return *OutsidePoints;
}
#pragma warning(pop)

void xt::internal::delwlk(uint32_t code) {
	if (PCSHeader.stitchCount != 0u) {
		auto highStitchBuffer = std::vector<fPOINTATTR> {};
		highStitchBuffer.reserve(PCSHeader.stitchCount);
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & WLKFMSK) != code) {
				highStitchBuffer.push_back(StitchBuffer[iStitch]);
			}
		}
		if (highStitchBuffer.size() != PCSHeader.stitchCount) {
			std::copy(highStitchBuffer.cbegin(),
			          highStitchBuffer.cend(),
			          stdext::make_checked_array_iterator(StitchBuffer, PCSHeader.stitchCount));
			PCSHeader.stitchCount = gsl::narrow<uint16_t>(highStitchBuffer.size());
		}
	}
}

void xt::internal::chkuseq() {
#if BUGBAK
	uint32_t index;

	for (index = 0; index < OutputIndex; index++) {
		InterleaveSequence->push_back((*OSequence)[index]);
	}
	InterleaveSequenceIndices->back().color = SelectedForm->underlayColor;
#else

	if (OutputIndex != 0u) {
		if (SelectedForm->underlayStitchLen < MINWLK) {
			SelectedForm->underlayStitchLen = MINWLK;
		}
		if (SelectedForm->underlayStitchLen > MAXWLK) {
			SelectedForm->underlayStitchLen = MAXWLK;
		}
		const auto underlayStitchLength = SelectedForm->underlayStitchLen;
		for (auto iSequence = 0u; iSequence < OutputIndex - 1; iSequence++) {
			const auto delta       = fPOINT { (*OSequence)[wrap::toSize(iSequence) + 1].x - (*OSequence)[iSequence].x,
                                        (*OSequence)[wrap::toSize(iSequence) + 1].y - (*OSequence)[iSequence].y };
			const auto length      = hypot(delta.x, delta.y);
			const auto stitchCount = wrap::round<uint32_t>(length / underlayStitchLength);
			if (stitchCount != 0u) {
				const auto step  = fPOINT { delta.x / stitchCount, delta.y / stitchCount };
				auto       point = (*OSequence)[iSequence];
				for (auto index = 0u; index < stitchCount; index++) {
					InterleaveSequence->push_back(point);
					point.x += step.x;
					point.y += step.y;
				}
			}
			else {
				InterleaveSequence->push_back((*OSequence)[iSequence]);
			}
		}
		InterleaveSequence->push_back((*OSequence)[OutputIndex - 1]);
		// ToDo - should this be front or (back - 1) ?
		InterleaveSequenceIndices->front().color = SelectedForm->underlayColor;
	}
#endif
}

void xt::internal::ritwlk() {
	if (OutputIndex != 0u) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { WLKMSK, SelectedForm->underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		chkuseq();
	}
}

void xt::internal::ritcwlk() {
	if (OutputIndex != 0u) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { CWLKMSK, SelectedForm->underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		chkuseq();
	}
}

uint32_t xt::internal::gucon(const fPOINT& start, const fPOINT& finish, uint32_t destination, uint32_t code) {
	auto       length      = hypot(finish.x - start.x, finish.y - start.y);
	auto       startVertex = form::closflt(start.x, start.y);
	const auto endVertex   = form::closflt(finish.x, finish.y);
	auto       up = 0u, down = 0u;

	if (length < 5) {
		return 0;
	}
	if (startVertex == endVertex) {
		return 0;
	}
	const auto& indentedPoint = xt::insid();
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
	auto iStitch = destination;
	while (startVertex != endVertex) {
		StitchBuffer[iStitch].x         = indentedPoint[startVertex].x;
		StitchBuffer[iStitch].y         = indentedPoint[startVertex].y;
		StitchBuffer[iStitch].attribute = code;
		if (iStitch != 0u) {
			if (StitchBuffer[iStitch - 1].x != StitchBuffer[iStitch].x
			    || StitchBuffer[iStitch - 1].y != StitchBuffer[iStitch].y) {
				iStitch++;
			}
		}
		else {
			iStitch++;
		}
		auto intermediateVertex = 0u;
		if (StateMap.test(StateFlag::WLKDIR)) {
			intermediateVertex = form::prv(startVertex);
		}
		else {
			intermediateVertex = form::nxt(startVertex);
		}
		const auto delta = fPOINT { indentedPoint[intermediateVertex].x - indentedPoint[startVertex].x,
			                        indentedPoint[intermediateVertex].y - indentedPoint[startVertex].y };
		length           = hypot(delta.x, delta.y);

		const auto stitchCount = wrap::round<uint32_t>(length / UserStitchLength);
		if (stitchCount > 1) {
			const auto step       = fPOINT { delta.x / stitchCount, delta.y / stitchCount };
			auto       localPoint = fPOINT { indentedPoint[startVertex].x + step.x, indentedPoint[startVertex].y + step.y };
			for (auto iStep = 0u; iStep < stitchCount - 1; iStep++) {
				StitchBuffer[iStitch] = { localPoint.x, localPoint.y, code };
				iStitch++;
				localPoint.x += step.x;
				localPoint.y += step.y;
			}
		}
		if (StateMap.test(StateFlag::WLKDIR)) {
			startVertex = form::prv(startVertex);
		}
		else {
			startVertex = form::nxt(startVertex);
		}
	}
	StitchBuffer[iStitch] = { indentedPoint[startVertex].x, indentedPoint[startVertex].y, code };
	iStitch++;
	return iStitch - destination;
}

void xt::internal::fnwlk(uint32_t find) {
	form::fvars(find);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->type = FRMFPOLY;
	}
	auto start = 0u;
	if (((SelectedForm->extendedAttribute & AT_STRT) != 0u) && SelectedForm->type != FRMLINE) {
		start = SelectedForm->fillStart;
	}
	auto count = VertexCount;
	if (SelectedForm->type != FRMLINE) {
		count++;
	}
	const auto& walkPoints = xt::insid();
	OutputIndex            = 0;
	while (count != 0u) {
		OSequence->push_back(walkPoints[start]);
		OutputIndex++;
		start = form::nxt(start);
		count--;
	}
	ritwlk();
}

void xt::internal::ritund() {
	if (!OSequence->empty()) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { UNDMSK, SelectedForm->underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		chkuseq();
	}
}

void xt::internal::undclp() {
	// ToDo - Is it better to initialize individually?
	auto& clipBuffer = *ClipBuffer;
	clipBuffer.clear();
	clipBuffer.reserve(2);
	ClipRectSize = FLSIZ { 0, SelectedForm->underlayStitchLen };
	ClipRect     = fRECTANGLE { 0, SelectedForm->underlayStitchLen, 0, 0 };
	clipBuffer.emplace_back(0.0f, 00.0f, 0u);
	clipBuffer.emplace_back(0.0f, SelectedForm->underlayStitchLen, 0u);
}

void xt::internal::fncwlk() {
	OutputIndex = 0;
	OSequence->clear();
	SelectedForm->extendedAttribute |= AT_CWLK;
	if (SelectedForm->satinGuideCount != 0u) {
		auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		if (SelectedForm->wordParam != 0u) {
			const auto iVertex    = SelectedForm->wordParam;
			auto&      thisVertex = vertexIt[iVertex];
			auto&      nextVertex = vertexIt[wrap::toSize(iVertex) + 1u];
			OSequence->push_back(fPOINT { form::midl(thisVertex.x, nextVertex.x), form::midl(thisVertex.y, nextVertex.y) });
			OutputIndex++;
		}
		auto guideIt = std::next(SatinGuides->cbegin(), SelectedForm->satinOrAngle.guide);
		for (auto iGuide = SelectedForm->satinGuideCount; iGuide != 0; iGuide--) {
			OSequence->push_back(
			    fPOINT { form::midl(vertexIt[guideIt[iGuide - 1].finish].x, vertexIt[guideIt[iGuide - 1].start].x),
			             form::midl(vertexIt[guideIt[iGuide - 1].finish].y, vertexIt[guideIt[iGuide - 1].start].y) });
			OutputIndex++;
		}
		if ((SelectedForm->attribute & FRMEND) != 0u) {
			OSequence->push_back(fPOINT { form::midl(vertexIt[0].x, vertexIt[1].x), form::midl(vertexIt[0].y, vertexIt[1].y) });
			OutputIndex++;
		}
	}
	else {
		auto start = 0u;
		if ((SelectedForm->extendedAttribute & AT_STRT) != 0u) {
			start = SelectedForm->fillStart;
		}
		auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		OSequence->push_back(vertexIt[start]);
		OutputIndex++;
		auto finish = form::prv(start);
		start       = form::nxt(start);
		for (auto iGuide = 1u; iGuide < (VertexCount / 2); iGuide++) {
			OSequence->push_back(
			    fPOINT { form::midl(vertexIt[finish].x, vertexIt[start].x), form::midl(vertexIt[finish].y, vertexIt[start].y) });
			if (form::cisin(OSequence->back().x, OSequence->back().y)) {
				OutputIndex++;
			}
			start  = form::nxt(start);
			finish = form::prv(finish);
		}
		OSequence->push_back(vertexIt[start]);
		OutputIndex++;
	}
	ritcwlk();
}

void xt::srtcol() {
	const auto colorSize = 16;
	auto       histogram = std::vector<uint32_t> {};
	histogram.resize(colorSize);
	auto colorStartStitch = std::vector<uint32_t> {};
	colorStartStitch.resize(colorSize);

	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		histogram[StitchBuffer[iStitch].attribute & COLMSK]++;
	}
	auto startStitch = 0u;
	auto it          = histogram.cbegin();
	for (auto& stitchColor : colorStartStitch) {
		stitchColor = startStitch;
		startStitch += *it;
		const auto _ = it++;
	}
	auto highStitchBuffer = std::vector<fPOINTATTR> {};
	highStitchBuffer.resize(PCSHeader.stitchCount);
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		highStitchBuffer[colorStartStitch[StitchBuffer[iStitch].attribute & COLMSK]++] = StitchBuffer[iStitch];
	}
	std::copy(highStitchBuffer.cbegin(), highStitchBuffer.cend(), StitchBuffer);
}

void xt::dubit(uint32_t bit) {
	thred::savdo();
	StateMap.set(StateFlag::WASDO);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->type = FRMFPOLY;
	}
	if (((SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) == 0u)
	    && ((bit & (AT_UND | AT_WALK | AT_CWLK)) != 0u)) {
		if (SelectedForm->fillType != 0u) {
			SelectedForm->underlayColor = SelectedForm->fillColor;
		}
		else {
			SelectedForm->underlayColor = gsl::narrow<uint8_t>(ActiveColor);
		}
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	}
	if (((SelectedForm->extendedAttribute & AT_UND) == 0u) && ((bit & AT_UND) != 0u)) {
		SelectedForm->underlayStitchAngle = IniFile.underlayAngle;
		SelectedForm->underlaySpacing     = IniFile.underlaySpacing;
	}
	const auto code = SelectedForm->extendedAttribute & bit;
	if (code != 0u) {
		SelectedForm->extendedAttribute &= ~(bit);
	}
	else {
		SelectedForm->extendedAttribute |= bit;
	}
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

void xt::chkcwlk() {
	if ((SelectedForm->extendedAttribute & AT_CWLK) != 0u) {
		xi::fncwlk();
	}
	else {
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
	}
}

void xt::chkwlk() {
	if ((SelectedForm->extendedAttribute & AT_WALK) != 0u) {
		xi::fnwlk(ClosestFormToCursor);
	}
	else {
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
	}
}

void xt::internal::fnund(const std::vector<RNGCNT>& textureSegments, uint32_t find, std::vector<fPOINT>& angledFormVertices) {
	const auto savedStitchSize = UserStitchLength;

	UserStitchLength = 1e38f;
	if (SelectedForm->underlaySpacing == 0.0f) {
		SelectedForm->underlaySpacing = IniFile.underlaySpacing;
	}
	if (SelectedForm->underlayStitchLen == 0.0f) {
		SelectedForm->underlayStitchLen = IniFile.underlayStitchLen;
	}
	undclp();
	StateMap.set(StateFlag::ISUND);
	form::angclpfn(textureSegments, angledFormVertices);
	OutputIndex = wrap::toUnsigned(OSequence->size());
	ritund();
	form::fvars(find);
	UserStitchLength = savedStitchSize;
}

void xt::chkund(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices) {
	if ((SelectedForm->extendedAttribute & AT_UND) != 0u) {
		xi::fnund(textureSegments, ClosestFormToCursor, angledFormVertices);
	}
	else {
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
	}
}

void xt::selalfrm() {
	SelectedFormList->reserve(FormList->size());
	for (auto formIndex = 0u; formIndex < FormList->size(); formIndex++) {
		SelectedFormList->push_back(formIndex);
	}
	StateMap.set(StateFlag::RESTCH);
}

uint32_t xt::internal::dutyp(uint32_t attribute) noexcept {
	auto       result          = char { 0 };
	auto       bit             = DWORD { 0 };
	const auto maskedAttribute = attribute & SRTYPMSK;

	// ToDo - replace
	_BitScanReverse(&bit, maskedAttribute);

	if (bit == 0) {
		return 0;
	}

	result = ((bit & 0xff) - 18);

	if ((result != 12) || ((maskedAttribute & TYPATMSK) == 0)) {
		return result & 0xf;
	}

	return 1;
}

void xt::internal::durec(OREC& record) noexcept {
	const auto* stitch = &StitchBuffer[record.start];

	record.type          = StitchTypes[dutyp(stitch->attribute)];
	const auto attribute = stitch->attribute & SRTMSK;
	record.color         = attribute & 0xf;
	record.form          = (attribute & FRMSK) >> FRMSHFT;
}

bool xt::internal::recmp(const OREC* record1, const OREC* record2) noexcept {
	if (ColorOrder[record1->color] == ColorOrder[record2->color]) {
		if (record1->form == record2->form) {
			if (record1->type == record2->type) {
				return (record1->start < record2->start);
			}
			return (record1->type < record2->type);
		}

		return (record1->form < record2->form);
	}
	return (ColorOrder[record1->color] < ColorOrder[record2->color]);
}

bool xt::internal::refcmp(const OREC* record1, const OREC* record2) noexcept {
	if (record1->form == record2->form) {
		return (record1->type < record2->type);
	}

	return (record1->form < record2->form);
}

bool xt::internal::chkrdun(const std::vector<uint32_t>& formFillCounter,
                           const std::vector<OREC*>&    pRecs,
                           const SRTREC&                stitchRecord) {
	for (auto iStitch = stitchRecord.start; iStitch < stitchRecord.finish; iStitch++) {
		if (pRecs[iStitch]->otyp == formFillCounter[pRecs[iStitch]->form]) {
			return true;
		}
	}
	return false;
}

double
xt::internal::precjmps(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord) {
	auto currentRegion = sortRecord.currentRegion;
	auto currentStitch = gsl::narrow_cast<fPOINTATTR*>(nullptr);
	auto direction     = sortRecord.direction;

	auto formFillCounter = std::vector<uint32_t> {};
	formFillCounter.resize((FormList->size() + 2u) << 2u);
	auto totalJumps = 0u;
	while (chkrdun(formFillCounter, pRecs, sortRecord)) {
		double minimumLength = 1e9;
		if (direction) {
			currentStitch = &StitchBuffer[pRecs[currentRegion]->finish];
		}
		else {
			currentStitch = &StitchBuffer[pRecs[currentRegion]->start];
		}
		for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
			if (pRecs[iRegion]->otyp == formFillCounter[pRecs[iRegion]->form]) {
				auto length
				    = hypot(pRecs[iRegion]->startStitch->x - currentStitch->x, pRecs[iRegion]->startStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction     = false;
					currentRegion = iRegion;
				}
				length = hypot(pRecs[iRegion]->endStitch->x - currentStitch->x, pRecs[iRegion]->endStitch->y - currentStitch->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction     = true;
					currentRegion = iRegion;
				}
			}
		}
		if (minimumLength > 9.0 * PFGRAN) {
			totalJumps++;
		}
		formFillCounter[pRecs[currentRegion]->form]++;
		if (StateMap.test(StateFlag::DUSRT)) {
			if (direction) {
				if (pRecs[currentRegion]->start != 0u) {
					for (auto iRegion = pRecs[currentRegion]->finish - 1; iRegion >= pRecs[currentRegion]->start; iRegion--) {
						tempStitchBuffer[OutputIndex++] = StitchBuffer[iRegion];
					}
				}
				else {
					auto iRegion = pRecs[currentRegion]->finish;
					while (iRegion != 0u) {
						tempStitchBuffer[OutputIndex++] = StitchBuffer[--iRegion];
					}
				}
			}
			else {
				for (auto iRegion = pRecs[currentRegion]->start; iRegion < pRecs[currentRegion]->finish; iRegion++) {
					tempStitchBuffer[OutputIndex++] = StitchBuffer[iRegion];
				}
			}
		}
	}
	return totalJumps;
}

uint32_t xt::internal::duprecs(std::vector<fPOINTATTR>& tempStitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord) {
	sortRecord.direction = false;
	const auto jumps0    = wrap::round<uint32_t>(precjmps(tempStitchBuffer, pRecs, sortRecord));

	sortRecord.direction = true;
	const auto jumps1    = wrap::round<uint32_t>(precjmps(tempStitchBuffer, pRecs, sortRecord));

	if (jumps0 < jumps1) {
		sortRecord.direction = false;
		return jumps0;
	}
	sortRecord.direction = true;
	return jumps1;
}

#ifdef _DEBUG

void xt::internal::dmprec(const std::vector<OREC*>& stitchRegion, uint32_t count) {
	for (auto iRegion = 0u; iRegion < count; iRegion++) {
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

bool xt::internal::srtchk(const std::vector<OREC*>& stitchRegion, uint32_t count, uint32_t& badForm) {
	auto formIndex = stitchRegion[0]->form;
	auto color     = stitchRegion[0]->color;

	for (auto iRegion = 1u; iRegion < count; iRegion++) {
		if (stitchRegion[iRegion]->form == formIndex) {
			if (ColorOrder[stitchRegion[iRegion]->color] < ColorOrder[color]) {
				auto& form = (*FormList)[formIndex];
				if (form.fillType == FTHF && ((form.extendedAttribute & AT_FTHBLND) != 0u)
				    && stitchRegion[iRegion]->color == form.fillColor) {
					continue;
				}
				badForm = iRegion;
				return false;
			}

			color = stitchRegion[iRegion]->color;
		}
		else {
			color     = stitchRegion[iRegion]->color;
			formIndex = stitchRegion[iRegion]->form;
		}
	}
	return true;
}

void xt::fsort() {
	auto attribute = StitchBuffer->attribute & SRTMSK;

	// There cannot be more records than stitches
	// ToDo - convert this to a reserve/pushback
	auto stitchRegion = std::vector<OREC> {};
	stitchRegion.resize(PCSHeader.stitchCount);

	// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors
	//        in a single form are not in ascending order already.
	thred::savdo();
	stitchRegion[0].start       = 0;
	stitchRegion[0].startStitch = StitchBuffer;
	ColorOrder[AppliqueColor]   = 0;
	for (auto iColor = 0u; iColor < 16; iColor++) {
		if (iColor != AppliqueColor) {
			ColorOrder[iColor] = iColor + 1;
		}
	}
	auto index = 0u;
	for (auto iStitch = 1u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & SRTMSK) != attribute) {
			stitchRegion[index].finish    = iStitch;
			stitchRegion[index].endStitch = &StitchBuffer[iStitch - 1];
			index++;
			stitchRegion[index].start       = iStitch;
			stitchRegion[index].startStitch = &StitchBuffer[iStitch];
			attribute                       = StitchBuffer[iStitch].attribute & SRTMSK;
		}
	}
	stitchRegion[index].endStitch = &StitchBuffer[PCSHeader.stitchCount - 1];
	stitchRegion[index].finish    = PCSHeader.stitchCount;
	index++;
	const auto lastRegion = index;
	auto       pRecs      = std::vector<OREC*> {};
	pRecs.resize(lastRegion);
	auto pFRecs = std::vector<OREC*> {};
	pFRecs.resize(lastRegion);
	for (auto iRegion = 0u; iRegion < lastRegion; iRegion++) {
		xi::durec(stitchRegion[iRegion]);
		pRecs[iRegion]  = &stitchRegion[iRegion];
		pFRecs[iRegion] = &stitchRegion[iRegion];
	}
	std::sort(pRecs.begin(), pRecs.end(), xi::recmp);
	std::sort(pFRecs.begin(), pFRecs.end(), xi::refcmp);
#ifdef _DEBUG
	xi::dmprec(pRecs, lastRegion);
#endif
	auto badForm = 0u;
	if (xi::srtchk(pFRecs, lastRegion, badForm)) {
		auto stitchRange = std::vector<RANGE> {};
		stitchRange.resize(lastRegion);
		stitchRange[0].start = 0;
		attribute            = pRecs[0]->color;
		auto currentForm     = 0xffffffffu;
		auto typeCount       = 0u;
		auto iRange          = 0u;
		for (auto iRegion = 0u; iRegion < lastRegion; iRegion++) {
			auto srtskp = true;
			if (attribute != pRecs[iRegion]->color) {
				stitchRange[iRange++].finish = iRegion;
				stitchRange[iRange].start    = iRegion;
				attribute                    = pRecs[iRegion]->color;
				currentForm                  = pRecs[iRegion]->form;
				typeCount                    = 0;
				srtskp                       = false;
			}
			if (srtskp) {
				if (pRecs[iRegion]->form == currentForm) {
					typeCount++;
				}
				else {
					typeCount   = 0;
					currentForm = pRecs[iRegion]->form;
				}
			}
			pRecs[iRegion]->otyp = typeCount;
		}
		stitchRange[iRange].finish  = lastRegion;
		const auto lastRange        = ++iRange;
		auto       tempStitchBuffer = std::vector<fPOINTATTR> {};
		tempStitchBuffer.resize(PCSHeader.stitchCount);
		OutputIndex = 0;
		for (iRange = 0; iRange < lastRange; iRange++) {
			StateMap.reset(StateFlag::DUSRT);
			auto sortRecord   = SRTREC {};
			sortRecord.start  = stitchRange[iRange].start;
			sortRecord.finish = stitchRange[iRange].finish;
			sortRecord.count  = sortRecord.finish - sortRecord.start;
			auto minimumJumps = 0xffffffffu;
			// timeout used to put an upper bound on the number of sorting permutations checked
			auto fileTime = FILETIME {};
			GetSystemTimeAsFileTime(&fileTime);
			const auto startTime        = xi::tim2int(fileTime);
			auto       minimumIndex     = 0u;
			auto       minimumDirection = 0u;
			for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
				sortRecord.currentRegion = iRegion;
				if (pRecs[iRegion]->otyp == 0u) {
					const auto jumps = xi::duprecs(tempStitchBuffer, pRecs, sortRecord);
					if (jumps < minimumJumps) {
						minimumJumps     = jumps;
						minimumIndex     = iRegion;
						minimumDirection = gsl::narrow_cast<uint32_t>(sortRecord.direction);
					}
				}
				GetSystemTimeAsFileTime(&fileTime);
				const auto nextTime = xi::tim2int(fileTime);
				if (nextTime.QuadPart - startTime.QuadPart > SRTIM) {
					break;
				}
			}
			StateMap.set(StateFlag::DUSRT);
			sortRecord.currentRegion = minimumIndex;
			sortRecord.direction     = (minimumDirection != 0u);
			xi::precjmps(tempStitchBuffer, pRecs, sortRecord);
		}
		std::copy(tempStitchBuffer.cbegin(), tempStitchBuffer.cbegin() + OutputIndex, StitchBuffer);
		PCSHeader.stitchCount = gsl::narrow<uint16_t>(OutputIndex);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		auto str = std::wstring {};
		displayText::loadString(str, IDS_SRTER);
		displayText::shoMsg(fmt::format(str, pFRecs[badForm]->form));
	}
}

#ifdef _DEBUG

struct _atfld {
	uint32_t color;
	uint32_t form;
	uint32_t type;
	uint32_t layer;
	uint32_t user;
};

using ATFLD = struct _atfld;

void xt::internal::duatf(uint32_t ind) {
	const auto attribute = StitchBuffer[ind].attribute;

	auto attributeFields = ATFLD { (attribute & COLMSK),
		                           ((attribute & FRMSK) >> FRMSHFT),
		                           gsl::narrow<uint32_t>(StitchTypes[dutyp(attribute)]),
		                           ((attribute >> LAYSHFT) & 7),
		                           0 };

	if ((attribute & 0x80000000) != 0u) {
		attributeFields.user = 1;
	}
	else {
		attributeFields.user = 0;
	}
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
	auto attribute = StitchBuffer[0].attribute;

	xi::duatf(0);
	for (auto iStitch = 1u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (attribute != StitchBuffer[iStitch].attribute) {
			xi::duatf(iStitch);
			attribute = StitchBuffer[iStitch].attribute;
		}
	}
}
#endif

void xt::fdelstch(FILLSTARTS& fillStartsData, uint32_t& fillStartsMap) {
	auto iDestinationStitch = 0u;
	auto tmap               = 0u;
	// ToDo - Still not sure what this function does?
	//        I suspect the fillStartsData members are not correctly named
	const auto codedFormIndex = (ClosestFormToCursor << FRMSHFT);
	auto       bordercolor    = gsl::narrow<uint32_t>(SelectedForm->borderColor & COLMSK);

	auto tapcol = gsl::narrow<uint32_t>(SelectedForm->borderColor >> 4u);
	for (auto iSourceStitch = 0u; iSourceStitch < PCSHeader.stitchCount; iSourceStitch++) {
		if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX) && iSourceStitch == ClosestPointIndex) {
			ClosestPointIndex = iDestinationStitch;
		}
		const auto attribute = StitchBuffer[iSourceStitch].attribute;
		if (codedFormIndex == (attribute & (FRMSK | NOTFRM))) {
			const auto type = StitchTypes[xi::dutyp(attribute)];
			switch (type) {
			case TYPE_APPLIQUE: {
				if ((tmap & M_AP) == 0u) {
					tmap |= M_AP;
					fillStartsData.fillNamed.applique = iDestinationStitch;
				}
				break;
			}
			case TYPE_FTHR: {
				if ((tmap & M_FTH) == 0u) {
					tmap |= M_FTH;
					fillStartsData.fillNamed.feather = iDestinationStitch;
				}
				break;
			}
			case TYPE_FILL: {
				if ((tmap & M_FIL) == 0u) {
					tmap |= M_FIL;
					fillStartsData.fillNamed.fill = iDestinationStitch;
				}
				break;
			}
			case TYPE_BORDER: {
				if ((tmap & M_BRD) == 0u) {
					tmap |= M_BRD;
					fillStartsData.fillNamed.border = iDestinationStitch;
				}
				break;
			}
			default: {
				if ((SelectedForm->fillType != 0u) && ((tmap & M_FIL) == 0u)) {
					tmap |= M_FIL;
					fillStartsData.fillNamed.fill = iDestinationStitch;
				}
				break;
			}
			}
		}
		else {
			// ToDo - there is a potential problem here when the active color is 0
			//        because the default color is 0 as well. Should the default color
			//        be e.g. -1 ?
			const auto color = attribute & COLMSK;
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
	PCSHeader.stitchCount = gsl::narrow<uint16_t>(iDestinationStitch);
	if ((tmap & M_ECOL) == 0u) {
		fillStartsData.fillNamed.borderColor = PCSHeader.stitchCount;
	}
	if ((tmap & M_FTHCOL) == 0u) {
		fillStartsData.fillNamed.featherColor = PCSHeader.stitchCount;
	}
	if ((tmap & M_FCOL) == 0u) {
		fillStartsData.fillNamed.fillColor = PCSHeader.stitchCount;
	}
	if (SelectedForm->edgeType != 0u) {
		if (SelectedForm->edgeType == EDGEAPPL) {
			if ((tmap & M_AP) == 0u) {
				if ((tmap & M_APCOL) != 0u) {
					fillStartsData.fillNamed.applique = fillStartsData.fillNamed.appliqueColor + 1;
				}
				else {
					fillStartsData.fillNamed.applique = PCSHeader.stitchCount;
				}
			}
		}
		if ((tmap & M_BRD) == 0u) {
			if ((tmap & M_ECOL) != 0u) {
				fillStartsData.fillNamed.border = fillStartsData.fillNamed.borderColor + 1;
			}
			else {
				fillStartsData.fillNamed.border = PCSHeader.stitchCount;
			}
		}
	}
	if ((SelectedForm->fillType != 0u) || ((tmap & (M_WALK | M_UND | M_CWLK)) != 0u)) {
		if ((tmap & M_FIL) == 0u) {
			if ((tmap & M_FCOL) != 0u) {
				fillStartsData.fillNamed.fill = fillStartsData.fillNamed.fillColor + 1;
			}
			else {
				fillStartsData.fillNamed.fill = PCSHeader.stitchCount;
			}
		}
	}
	if (SelectedForm->fillType == FTHF) {
		if ((tmap & M_FTH) == 0u) {
			if ((tmap & M_FTHCOL) != 0u) {
				fillStartsData.fillNamed.feather = fillStartsData.fillNamed.featherColor + 1;
			}
			else {
				fillStartsData.fillNamed.feather = PCSHeader.stitchCount;
			}
		}
	}
	for (auto ind = 3u; ind != 0u; ind--) {
		iDestinationStitch = ind - 1;
		while (iDestinationStitch < ind) {
			if (fillStartsData.fillArray[iDestinationStitch] > fillStartsData.fillArray[ind]) {
				fillStartsData.fillArray[ind] = fillStartsData.fillArray[iDestinationStitch];
			}
			iDestinationStitch--;
		}
	}
	if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX)) {
		for (auto& startPoint : fillStartsData.fillArray) {
			startPoint = ClosestPointIndex;
		}
	}
}

bool xt::internal::lastcol(uint32_t index, fPOINT& point) {
	const auto color = (*InterleaveSequenceIndices)[index].color;
	while (index != 0u) {
		index--;
		if ((*InterleaveSequenceIndices)[index].color == color) {
			point = (*InterleaveSequence)[(*InterleaveSequenceIndices)[wrap::toSize(index) + 1].index - 1];
			return true;
		}
	}
	return false;
}

void xt::internal::duint(uint32_t offset, uint32_t code, INTINF& ilData) {
	if (ilData.coloc > ilData.start) {
		auto count       = ilData.coloc - ilData.start;
		auto sourceStart = &StitchBuffer[ilData.start];
		auto sourceEnd   = sourceStart + count;

		const auto destination
		    = stdext::make_checked_array_iterator(&ilData.highStitchBuffer[ilData.output], MAXITEMS - ilData.output);
		std::copy(sourceStart, sourceEnd, destination);
		ilData.start += count;
		ilData.output += count;
	}
	if ((SelectedForm->extendedAttribute & AT_STRT) != 0u) {
		if (!StateMap.testAndSet(StateFlag::DIDSTRT)) {
			auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
			ilData.output += gucon(vertexIt[SelectedForm->fillStart],
			                       (*InterleaveSequence)[(*InterleaveSequenceIndices)[ilData.pins].index],
			                       ilData.output + offset,
			                       code);
		}
	}
	auto point = fPOINT {};
	if (lastcol(ilData.pins, point)) {
		ilData.output += gucon(
		    point, (*InterleaveSequence)[(*InterleaveSequenceIndices)[ilData.pins].index], ilData.output + MAXITEMS, code);
	}
	for (auto iSequence = (*InterleaveSequenceIndices)[ilData.pins].index;
	     iSequence < (*InterleaveSequenceIndices)[wrap::toSize(ilData.pins) + 1].index;
	     iSequence++) {
		ilData.highStitchBuffer[ilData.output].x         = (*InterleaveSequence)[iSequence].x;
		ilData.highStitchBuffer[ilData.output].y         = (*InterleaveSequence)[iSequence].y;
		ilData.highStitchBuffer[ilData.output].attribute = code;
		if (ilData.highStitchBuffer[ilData.output].x != ilData.highStitchBuffer[ilData.output - 1].x
		    || ilData.highStitchBuffer[ilData.output].y != ilData.highStitchBuffer[ilData.output - 1].y) {
			ilData.output++;
		}
	}
}

bool xt::internal::isfil() noexcept {
	if (SelectedForm->fillType != 0u) {
		return true;
	}
	if (SelectedForm->edgeType != 0u) {
		return true;
	}
	if ((SelectedForm->extendedAttribute & (AT_CWLK | AT_WALK | AT_UND)) != 0u) {
		return true;
	}
	return false;
}

void xt::internal::chkend(uint32_t offset, uint32_t code, INTINF& ilData) {
	if (isfil()) {
		StateMap.set(StateFlag::ISEND);
		if ((SelectedForm->extendedAttribute & AT_END) != 0u) {
			auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
			ilData.output += gucon(InterleaveSequence->back(), vertexIt[SelectedForm->fillEnd], ilData.output + offset, code);
		}
	}
}

void xt::intlv(const FILLSTARTS& fillStartsData, uint32_t fillStartsMap) {
	auto ilData = INTINF {};

	StateMap.reset(StateFlag::ISEND);
	form::fvars(ClosestFormToCursor);
	InterleaveSequenceIndices->emplace_back(INSREC { 0, 0, wrap::toUnsigned(InterleaveSequence->size()), 0 });
	ilData.layerIndex
	    = (gsl::narrow<uint32_t>(SelectedForm->attribute & FRMLMSK) << (LAYSHFT - 1)) | (ClosestFormToCursor << FRMSHFT);
	StateMap.reset(StateFlag::DIDSTRT);
	if (PCSHeader.stitchCount != 0u) {
		const auto offset = MAXITEMS;
		// Todo - Allocate memory locally for ilData.highStitchBuffer
		ilData.highStitchBuffer = &StitchBuffer[MAXITEMS];
		auto code               = 0u;
		for (auto iSequence = 0u; iSequence < (InterleaveSequenceIndices->size() - 1u); iSequence++) {
			ilData.pins = iSequence;
			switch ((*InterleaveSequenceIndices)[iSequence].seq) {
			case I_AP: {
				if (((fillStartsMap & M_FIL) != 0u) && fillStartsData.fillNamed.applique >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.applique;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.appliqueColor;
					if (ilData.coloc == 1) {
						ilData.coloc = 0;
					}
				}
				break;
			}
			case I_FIL: {
				if (((fillStartsMap & M_FIL) != 0u) && fillStartsData.fillNamed.fill >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.fill;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.fillColor;
				}
				break;
			}
			case I_FTH: {
				if (((fillStartsMap & M_FIL) != 0u) && fillStartsData.fillNamed.feather >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.feather;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.featherColor;
				}
				break;
			}
			case I_BRD: {
				if (((fillStartsMap & M_BRD) != 0u) && fillStartsData.fillNamed.border >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.border;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.borderColor;
				}
				break;
			}
			}
			code = gsl::narrow<uint32_t>(ilData.layerIndex | (*InterleaveSequenceIndices)[ilData.pins].code
			                             | (*InterleaveSequenceIndices)[ilData.pins].color);
			xi::duint(offset, code, ilData);
		}
		xi::chkend(MAXITEMS, code, ilData);
		if ((PCSHeader.stitchCount != 0u) && ilData.start < gsl::narrow<uint32_t>(PCSHeader.stitchCount) - 1) {
			auto ine         = PCSHeader.stitchCount - ilData.start;
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
		const auto offset   = 0;
		auto       code     = 0u;
		auto       vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		for (auto iSequence = 0u; iSequence < (InterleaveSequenceIndices->size() - 1u); iSequence++) {
			code = gsl::narrow<uint32_t>(ilData.layerIndex | (*InterleaveSequenceIndices)[iSequence].code
			                             | (*InterleaveSequenceIndices)[iSequence].color);
			if ((SelectedForm->extendedAttribute & AT_STRT) != 0u) {
				if (!StateMap.testAndSet(StateFlag::DIDSTRT)) {
					ilData.output += xi::gucon(vertexIt[SelectedForm->fillStart],
					                           (*InterleaveSequence)[(*InterleaveSequenceIndices)[ilData.pins].index],
					                           ilData.output + offset,
					                           code);
				}
			}
			auto colpnt = fPOINT {};
			if (xi::lastcol(iSequence, colpnt)) {
				ilData.output += xi::gucon(
				    colpnt, (*InterleaveSequence)[(*InterleaveSequenceIndices)[iSequence].index], ilData.output, code);
			}
			for (auto ine = (*InterleaveSequenceIndices)[iSequence].index;
			     ine < (*InterleaveSequenceIndices)[wrap::toSize(iSequence) + 1].index;
			     ine++) {
				StitchBuffer[ilData.output] = { (*InterleaveSequence)[ine].x, (*InterleaveSequence)[ine].y, code };
				if (ilData.output > 0) {
					if (StitchBuffer[ilData.output].x != StitchBuffer[ilData.output - 1].x
					    || StitchBuffer[ilData.output].y != StitchBuffer[ilData.output - 1].y) {
						ilData.output++;
					}
				}
				else {
					ilData.output++;
				}
			}
		}
		xi::chkend(0, code, ilData);
	}
	PCSHeader.stitchCount = gsl::narrow<uint16_t>(ilData.output);
	thred::coltab();
}

void xt::internal::setundfn(uint32_t code) {
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

void xt::internal::notundfn(uint32_t code) {
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

void xt::internal::ulenfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if ((SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0u) {
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

void xt::internal::uspacfn(uint32_t find, float spacing) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if ((SelectedForm->extendedAttribute & AT_UND) != 0u) {
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

void xt::internal::uangfn(uint32_t find, float angle) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if ((SelectedForm->extendedAttribute & AT_UND) != 0u) {
		SelectedForm->underlayStitchAngle = angle;
		form::refilfn();
	}
}

void xt::dufang(float angle) {
	thred::savdo();
	angle *= gsl::narrow_cast<float>(PI) / 180;
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

void xt::internal::flenfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if ((SelectedForm->fillType != 0u) && !clip::isclp(find)) {
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

void xt::internal::fspacfn(uint32_t find, float spacing) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType != 0u) {
		if (spacing < 0) {
			if (!clip::isclp(find)) {
				return;
			}
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

void xt::internal::findfn(uint32_t find, float indent) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	SelectedForm->underlayIndent = indent;
	if ((SelectedForm->extendedAttribute & (AT_UND | AT_WALK)) != 0u) {
		form::refilfn();
	}
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

void xt::internal::fangfn(uint32_t find, float angle) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	// ToDo - also do angle updates for texture filled forms
	if (SelectedForm->type == FRMFPOLY && (SelectedForm->fillType != 0u)) {
		switch (SelectedForm->fillType) {
		case VRTF:
		case HORF:
		case ANGF: {
			SelectedForm->fillType              = ANGF;
			SelectedForm->angleOrClipData.angle = angle;
			break;
		}
		case VCLPF:
		case HCLPF:
		case ANGCLPF: {
			SelectedForm->fillType           = ANGCLPF;
			SelectedForm->satinOrAngle.angle = angle;
			break;
		}
		}
		form::refilfn();
	}
}

void xt::dufxang(float angle) {
	thred::savdo();
	angle *= gsl::narrow_cast<float>(PI) / 180;
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

void xt::internal::ucolfn(uint32_t find, uint32_t color) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if ((SelectedForm->extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0u) {
		SelectedForm->underlayColor = gsl::narrow<uint8_t>(color);
		form::refilfn();
	}
}

void xt::dundcol(uint32_t color) {
	thred::savdo();
	if (color != 0u) {
		color--;
	}
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

void xt::internal::fcolfn(uint32_t find, uint32_t color) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType != 0u) {
		SelectedForm->fillColor = gsl::narrow<uint8_t>(color);
		form::refilfn();
	}
}

void xt::dufcol(uint32_t color) {
	thred::savdo();
	if (color != 0u) {
		color--;
	}
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

void xt::internal::bcolfn(uint32_t find, uint32_t color) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType != 0u) {
		SelectedForm->borderColor = gsl::narrow<uint8_t>(color);
		form::refilfn();
	}
}

void xt::dubcol(uint32_t color) {
	thred::savdo();
	if (color != 0u) {
		color--;
	}
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

void xt::internal::blenfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if ((SelectedForm->edgeType != 0u) && !clip::iseclp(find)) {
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

void xt::internal::bspacfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType != 0u) {
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

void xt::internal::bminfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType != 0u) {
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

void xt::internal::bmaxfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType != 0u) {
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

void xt::internal::fminfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType != 0u) {
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

void xt::internal::fmaxfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->fillType != 0u) {
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

void xt::internal::fwidfn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	const auto reference = SelectedForm->rectangle.left;
	const auto ratio     = length / (SelectedForm->rectangle.right - reference);
	auto       vertexIt  = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
	}
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

void xt::internal::fhifn(uint32_t find, float length) {
	ClosestFormToCursor = find;
	form::fvars(ClosestFormToCursor);
	const auto reference = SelectedForm->rectangle.bottom;
	const auto ratio     = length / (SelectedForm->rectangle.top - reference);
	auto       vertexIt  = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
	}
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
		auto& form     = (*FormList)[ClosestFormToCursor];
		form.fillStart = gsl::narrow<uint16_t>(ClosestVertexToCursor);
		form.extendedAttribute |= AT_STRT;
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_FORMP, IDS_FSTRT);
	}
}

void xt::setfilend() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		auto& form   = (*FormList)[ClosestFormToCursor];
		form.fillEnd = gsl::narrow<uint16_t>(ClosestVertexToCursor);
		form.extendedAttribute |= AT_END;
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_FORMP, IDS_FEND);
	}
}

void xt::duauxnam() {
	*AuxName = *WorkingFileName;
	switch (IniFile.auxFileType) {
	case AUXDST: {
		AuxName->replace_extension(".dst");
		break;
	}
#if PESACT
	case AUXPES: {
		AuxName->replace_extension(".pes");
		break;
	}
#endif
	default: {
		AuxName->replace_extension("pcs");
		break;
	}
	}
}

void xt::internal::rtrclpfn() {
	auto count = 0u;

	if (OpenClipboard(ThrEdWindow) != 0) {
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
		if (count != 0u) {
			LowerLeftStitch.x = 0.0f;
			LowerLeftStitch.y = 0.0f;
			EmptyClipboard();
			Clip        = RegisterClipboardFormat(PcdClipFormat);
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, count * sizeof(CLPSTCH) + 2);
			if (ClipPointer != nullptr) {
				ClipStitchData = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
				thred::rtclpfn(0, 0);
				ClipStitchData[0].led = count;
				for (auto iStitch = 1u; iStitch < count; iStitch++) {
					thred::rtclpfn(iStitch, iStitch);
				}
				SetClipboardData(Clip, ClipPointer);
			}
			CloseClipboard();
		}
	}
}

void xt::rtrclp() {
	if (StateMap.test(StateFlag::FORMSEL)) {
		form::fvars(ClosestFormToCursor);
		if (texture::istx(ClosestFormToCursor)) {
			texture::rtrtx();
		}
		else {
			xi::rtrclpfn();
		}
	}
}

void xt::internal::setstxt(uint32_t stringIndex, float value, HWND dialog) {
	SetWindowText(GetDlgItem(dialog, stringIndex), fmt::format(L"{:.2f}", (value / PFGRAN)).c_str());
}

float xt::internal::getstxt(uint32_t stringIndex, HWND dialog) {
	// ToDo - This is not great code.
	wchar_t buffer[16] = {};
	GetWindowText(GetDlgItem(dialog, stringIndex), buffer, sizeof(buffer) / sizeof(buffer[0]));
	return wrap::bufToFloat(buffer) * PFGRAN;
}

bool xt::internal::chkasp(fPOINT& point, float aspectRatio, HWND dialog) {
	point.x = getstxt(IDC_DESWID, dialog);
	point.y = getstxt(IDC_DESHI, dialog);
	// ToDo - should this have a range? aspectRatio +/- %
	return (point.y / point.x) == aspectRatio;
}

BOOL CALLBACK xt::internal::setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) {
	UNREFERENCED_PARAMETER(lparam);

	auto designSizeDialog = hwndlg; // change design size dialog window

	switch (umsg) {
	case WM_INITDIALOG: {
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		setstxt(IDC_DESWID, DesignSize.x, designSizeDialog);
		setstxt(IDC_DESHI, DesignSize.y, designSizeDialog);
		CheckDlgButton(hwndlg, IDC_REFILF, gsl::narrow_cast<UINT>(UserFlagMap.test(UserFlag::CHREF)));
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) {
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			DesignSize.x = getstxt(IDC_DESWID, designSizeDialog);
			DesignSize.y = getstxt(IDC_DESHI, designSizeDialog);
			if (IsDlgButtonChecked(hwndlg, IDC_REFILF) != 0u) {
				UserFlagMap.set(UserFlag::CHREF);
			}
			else {
				UserFlagMap.reset(UserFlag::CHREF);
			}
			EndDialog(hwndlg, 1);
			return TRUE;
		}
		case IDC_DESWID: {
			if ((wparam >> 16u) == EN_CHANGE) {
				StateMap.reset(StateFlag::DESCHG);
			}
			break;
		}
		case IDC_DESHI: {
			if ((wparam >> 16u) == EN_CHANGE) {
				StateMap.set(StateFlag::DESCHG);
			}
			break;
		}
		case IDC_DUASP: {
			const auto designAspectRatio = DesignSize.y / DesignSize.x;
			auto       designSize        = fPOINT {};
			if (!chkasp(designSize, designAspectRatio, designSizeDialog)) {
				if (StateMap.test(StateFlag::DESCHG)) {
					setstxt(IDC_DESWID, designSize.y / designAspectRatio, designSizeDialog);
				}
				else {
					setstxt(IDC_DESHI, designSize.x * designAspectRatio, designSizeDialog);
				}
			}
			break;
		}
		}
	}
	}
	return 0;
}

void xt::internal::sadj(fPOINTATTR& stitch, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	stitch.x = (stitch.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	stitch.y = (stitch.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xt::internal::sadj(fPOINT& point, const dPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	point.x = (point.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	point.y = (point.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xt::internal::nudfn(const fRECTANGLE& designSizeRect) {
	const auto newSize = fPOINT { (designSizeRect.right - designSizeRect.left), (designSizeRect.top - designSizeRect.bottom) };
	const auto designSizeRatio = dPOINT { (DesignSize.x / newSize.x), (DesignSize.y / newSize.y) };
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		sadj(StitchBuffer[iStitch], designSizeRatio, designSizeRect);
	}
	for (auto& FormVertice : *FormVertices) {
		sadj(FormVertice, designSizeRatio, designSizeRect);
	}
	form::frmout(ClosestFormToCursor);
}

void xt::nudsiz() {
	auto flag           = 0;
	auto designSizeRect = fRECTANGLE {}; // design size rectangle

	thred::savdo();
	flag = 0;
	if (PCSHeader.stitchCount != 0u) {
		thred::stchrct(designSizeRect);
		flag = 1;
	}
	else {
		if (!FormList->empty()) {
			thred::frmrct(designSizeRect);
			flag = 2;
		}
	}
	if (flag != 0) {
		DesignSize.x = designSizeRect.right - designSizeRect.left;
		DesignSize.y = designSizeRect.top - designSizeRect.bottom;
		if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, (DLGPROC)xi::setsprc)) {
			flag = 0;
			if (DesignSize.x > IniFile.hoopSizeX) {
				IniFile.hoopSizeX = DesignSize.x * 1.05;
				UnzoomedRect.x    = wrap::round<int32_t>(IniFile.hoopSizeX);
				flag              = 1;
			}
			if (DesignSize.y > IniFile.hoopSizeY) {
				IniFile.hoopSizeY = DesignSize.y * 1.05;
				UnzoomedRect.y    = wrap::round<int32_t>(IniFile.hoopSizeY);
				flag              = 1;
			}
			xi::nudfn(designSizeRect);
			if (UserFlagMap.test(UserFlag::CHREF)) {
				form::refilal();
			}
			if (flag != 0) {
				thred::movStch();
				thred::zumhom();
				displayText::hsizmsg();
			}
			form::centir();
			for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
				form::frmout(iForm);
			}
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
	if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	}
	if (StateMap.test(StateFlag::BZUMIN)) {
		if ((Msg.wParam & MK_LBUTTON) != 0u) {
			if (StateMap.testAndSet(StateFlag::VCAPT)) {
				SetCapture(ThrEdWindow);
			}
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
	return TRUE;
}

void xt::clrstch() noexcept {
	while (EnumChildWindows(MainStitchWin, xi::enumch, 0) != 0) {
		;
	}
}

void xt::chgwrn() {
	UserFlagMap.flip(UserFlag::WRNOF);
	thred::wrnmen();
	StateMap.set(StateFlag::DUMEN);
}

void xt::chgchk(uint8_t code) {
	IniFile.dataCheck = code;
	thred::chkmen();
	StateMap.set(StateFlag::DUMEN);
}

void xt::tst() {
	DesignerName->assign(L"Coder");
	ThrName->assign(*DesignerName);
	StateMap.set(StateFlag::RESTCH);
}
