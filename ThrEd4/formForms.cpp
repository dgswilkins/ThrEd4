#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "ExtendedBitSet.h"
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
#include "formForms.h"

namespace ffi = formForms::internal;

RECT  LabelWindowCoords; // location of left windows in the form data sheet
POINT LabelWindowSize;   // size of the left windows in the form data sheet
RECT  ValueWindowCoords; // location of right windows in the form data sheet
POINT ValueWindowSize;   // size of the right windows in the form data sheet

uint16_t DaisyTypeStrings[] = {
	IDS_DAZCRV, IDS_DAZSAW, IDS_DAZRMP, IDS_DAZRAG, IDS_DAZCOG, IDS_DAZHART,
};

void formForms::maxtsiz(const std::wstring& label, POINT& textSize) {
	auto labelSize = SIZE { 0L, 0L };

	wrap::GetTextExtentPoint32(GetDC(ThrEdWindow), label.data(), wrap::toUnsigned(label.size()), &labelSize);
	textSize.y = labelSize.cy;
	if (labelSize.cx > textSize.x) {
		textSize.x = labelSize.cx;
	}
}

auto formForms::maxwid(uint32_t start, uint32_t finish) {
	auto textSize = POINT { 0L, 0L };

	while (start <= finish) {
		formForms::maxtsiz((*StringTable)[start++], textSize);
	}
	return textSize.x + 6;
}

auto formForms::internal::txtwin(const std::wstring& windowName, const RECT& location) -> HWND {
	if (StateMap.test(StateFlag::REFCNT)) {
		formForms::maxtsiz(windowName, LabelWindowSize);
		return nullptr;
	}
	return CreateWindow(L"STATIC", // NOLINT
	                    windowName.c_str(),
	                    SS_NOTIFY | WS_CHILD | WS_VISIBLE,
	                    location.left,
	                    location.top,
	                    location.right - location.left,
	                    location.bottom - location.top,
	                    FormDataSheet,
	                    nullptr,
	                    ThrEdInstance,
	                    nullptr);
}

auto formForms::internal::txtrwin(const std::wstring& winName, const RECT& location) -> HWND {
	if (StateMap.test(StateFlag::REFCNT)) {
		formForms::maxtsiz(winName, ValueWindowSize);
		return nullptr;
	}
	return CreateWindow(L"STATIC", // NOLINT
	                    winName.c_str(),
	                    SS_NOTIFY | WS_BORDER | WS_CHILD | WS_VISIBLE,
	                    location.left,
	                    location.top,
	                    location.right - location.left,
	                    location.bottom - location.top,
	                    FormDataSheet,
	                    nullptr,
	                    ThrEdInstance,
	                    nullptr);
}

auto formForms::internal::numwin(const std::wstring& winName, const RECT& location) -> HWND {
	if (StateMap.test(StateFlag::REFCNT)) {
		formForms::maxtsiz(winName, ValueWindowSize);
		return nullptr;
	}
	return CreateWindow(L"STATIC", // NOLINT
	                    winName.c_str(),
	                    SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
	                    location.left,
	                    location.top,
	                    location.right - location.left,
	                    location.bottom - location.top,
	                    FormDataSheet,
	                    nullptr,
	                    ThrEdInstance,
	                    nullptr);
}

void formForms::internal::nxtlin(uint32_t& formMenuEntryCount) noexcept {
	formMenuEntryCount++;
	ffi::nxtlinprf();
}

void formForms::internal::nxtlinprf() noexcept {
	LabelWindowCoords.top += LabelWindowSize.y;
	LabelWindowCoords.bottom += LabelWindowSize.y;
	ValueWindowCoords.top += ValueWindowSize.y;
	ValueWindowCoords.bottom += ValueWindowSize.y;
}

void formForms::internal::refrmfn(uint32_t& formMenuEntryCount) {
	const uint16_t edgeArray[]
	    = { MEGLIN, MEGBLD, MEGCLP, MEGSAT, MEGAP, MEGPRP, MEGHOL, MEGPIC, MEGDUB, MEGCHNH, MEGCHNL, MEGCLPX, 0 };

	auto  choice      = std::wstring {};
	auto& stringTable = *StringTable;
	auto& labelWindow = *LabelWindow;
	auto& valueWindow = *ValueWindow;

	ValueWindowCoords.top    = 3;
	LabelWindowCoords.top    = ValueWindowCoords.top;
	ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom;
	LabelWindowCoords.left   = 3;
	LabelWindowCoords.right  = 3 + LabelWindowSize.x;
	ValueWindowCoords.left   = 6 + LabelWindowSize.x;
	ValueWindowCoords.right  = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	labelWindow[LFRM]        = ffi::txtwin(stringTable[STR_TXT0], LabelWindowCoords);
	if (SelectedForm->type == FRMLINE) {
		choice = stringTable[STR_EDG1];
	}
	else {
		choice = stringTable[STR_FREH];
	}
	valueWindow[LFRM] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	labelWindow[LLAYR] = ffi::txtwin(stringTable[STR_TXT1], LabelWindowCoords);
	valueWindow[LLAYR] = ffi::txtrwin(fmt::format(L"{}", (gsl::narrow_cast<uint8_t>(SelectedForm->attribute & FRMLMSK) >> 1U)),
	                                  ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (SelectedForm->type != FRMLINE) {
		labelWindow[LCWLK] = ffi::txtwin(stringTable[STR_CWLK], LabelWindowCoords);
		if ((SelectedForm->extendedAttribute & AT_CWLK) != 0U) {
			choice = stringTable[STR_ON];
		}
		else {
			choice = stringTable[STR_OFF];
		}
		valueWindow[LCWLK] = ffi::txtrwin(choice, ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		labelWindow[LWALK] = ffi::txtwin(stringTable[STR_WALK], LabelWindowCoords);
		if ((SelectedForm->extendedAttribute & AT_WALK) != 0U) {
			choice = stringTable[STR_ON];
		}
		else {
			choice = stringTable[STR_OFF];
		}
		valueWindow[LWALK] = ffi::txtrwin(choice, ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		labelWindow[LUND] = ffi::txtwin(stringTable[STR_UND], LabelWindowCoords);
		if ((SelectedForm->extendedAttribute & AT_UND) != 0U) {
			choice = stringTable[STR_ON];
		}
		else {
			choice = stringTable[STR_OFF];
		}
		valueWindow[LUND] = ffi::txtrwin(choice, ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if ((SelectedForm->extendedAttribute & (AT_WALK | AT_UND | AT_CWLK)) != 0U) {
			labelWindow[LUNDCOL] = ffi::txtwin(stringTable[STR_UNDCOL], LabelWindowCoords);
			valueWindow[LUNDCOL] = ffi::txtrwin(fmt::format(L"{}", (SelectedForm->underlayColor + 1U)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LULEN] = ffi::txtwin(stringTable[STR_ULEN], LabelWindowCoords);
			valueWindow[LULEN]
			    = ffi::txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlayStitchLen / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		labelWindow[LWLKIND] = ffi::txtwin(stringTable[STR_UWLKIND], LabelWindowCoords);
		valueWindow[LWLKIND] = ffi::txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlayIndent / PFGRAN)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if ((SelectedForm->extendedAttribute & AT_UND) != 0U) {
			labelWindow[LUSPAC] = ffi::txtwin(stringTable[STR_FUSPAC], LabelWindowCoords);
			valueWindow[LUSPAC]
			    = ffi::txtrwin(fmt::format(L"{:.2f}", (SelectedForm->underlaySpacing / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LUANG] = ffi::txtwin(stringTable[STR_FUANG], LabelWindowCoords);
			valueWindow[LUANG]
			    = ffi::txtrwin(fmt::format(L"{:.2f}", (gsl::narrow_cast<double>(SelectedForm->underlayStitchAngle) * 180.0 / PI)),
			                   ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
	}
	labelWindow[LFRMFIL] = ffi::txtwin(stringTable[STR_TXT2], LabelWindowCoords);
	valueWindow[LFRMFIL] = ffi::txtrwin(stringTable[wrap::toSize(SelectedForm->fillType) + STR_FIL0], ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (SelectedForm->fillType != 0U) {
		labelWindow[LFRMCOL] = ffi::txtwin(stringTable[STR_TXT3], LabelWindowCoords);
		valueWindow[LFRMCOL] = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillColor + 1U)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if (SelectedForm->fillType == FTHF) {
			labelWindow[LFTHCOL] = ffi::txtwin(stringTable[STR_FTHCOL], LabelWindowCoords);
			valueWindow[LFTHCOL]
			    = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.color + 1U)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LFTHTYP] = ffi::txtwin(stringTable[STR_FTHTYP], LabelWindowCoords);
			valueWindow[LFTHTYP] = ffi::numwin(stringTable[wrap::toSize(SelectedForm->fillInfo.feather.fillType) - 1U + STR_FTH0],
			                                   ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LFTHBLND] = ffi::txtwin(stringTable[STR_FTHBLND], LabelWindowCoords);
			if ((SelectedForm->extendedAttribute & AT_FTHBLND) != 0U) {
				choice = stringTable[STR_ON];
			}
			else {
				choice = stringTable[STR_OFF];
			}
			valueWindow[LFTHBLND] = ffi::txtrwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			if ((SelectedForm->extendedAttribute & AT_FTHBLND) == 0U) {
				labelWindow[LFTHBTH] = ffi::txtwin(stringTable[STR_FTHBOTH], LabelWindowCoords);
				if ((SelectedForm->extendedAttribute & (AT_FTHBTH)) != 0U) {
					choice = stringTable[STR_ON];
				}
				else {
					choice = stringTable[STR_OFF];
				}
				valueWindow[LFTHBTH] = ffi::txtrwin(choice, ValueWindowCoords);
				ffi::nxtlin(formMenuEntryCount);
				if ((SelectedForm->extendedAttribute & AT_FTHBTH) == 0U) {
					labelWindow[LFTHUP] = ffi::txtwin(stringTable[STR_FTHUP], LabelWindowCoords);
					if ((SelectedForm->extendedAttribute & AT_FTHUP) != 0U) {
						choice = stringTable[STR_ON];
					}
					else {
						choice = stringTable[STR_OFF];
					}
					valueWindow[LFTHUP] = ffi::txtrwin(choice, ValueWindowCoords);
					ffi::nxtlin(formMenuEntryCount);
				}
			}
			labelWindow[LFTHUPCNT] = ffi::txtwin(stringTable[STR_FTHUPCNT], LabelWindowCoords);
			valueWindow[LFTHUPCNT] = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.upCount)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LFTHDWNCNT] = ffi::txtwin(stringTable[STR_FTHDWNCNT], LabelWindowCoords);
			valueWindow[LFTHDWNCNT]
			    = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.downCount)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LFTHSIZ] = ffi::txtwin(stringTable[STR_FTHSIZ], LabelWindowCoords);
			valueWindow[LFTHSIZ] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->fillInfo.feather.ratio)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			if (SelectedForm->fillInfo.feather.fillType == FTHPSG) {
				labelWindow[LFTHNUM] = ffi::txtwin(stringTable[STR_FTHNUM], LabelWindowCoords);
				valueWindow[LFTHNUM] = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillInfo.feather.count)), ValueWindowCoords);
				ffi::nxtlin(formMenuEntryCount);
			}
			labelWindow[LFTHFLR] = ffi::txtwin(stringTable[STR_FTHFLR], LabelWindowCoords);
			valueWindow[LFTHFLR]
			    = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->fillInfo.feather.minStitchSize / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (SelectedForm->fillType != CLPF) {
			labelWindow[LFRMSPAC] = ffi::txtwin(stringTable[STR_TXT4], LabelWindowCoords);
			valueWindow[LFRMSPAC] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->fillSpacing / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (texture::istx(ClosestFormToCursor)) {
			labelWindow[LTXOF] = ffi::txtwin(stringTable[STR_TXOF], LabelWindowCoords);
			valueWindow[LTXOF] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->txof / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		labelWindow[LMAXFIL] = ffi::txtwin(stringTable[STR_TXT20], LabelWindowCoords);
		valueWindow[LMAXFIL] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->maxFillStitchLen / PFGRAN)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if (!clip::isclp(ClosestFormToCursor) && !texture::istx(ClosestFormToCursor)) {
			labelWindow[LFRMLEN] = ffi::txtwin(stringTable[STR_TXT5], LabelWindowCoords);
			valueWindow[LFRMLEN]
			    = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->lengthOrCount.stitchLength / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		labelWindow[LMINFIL] = ffi::txtwin(stringTable[STR_TXT21], LabelWindowCoords);
		valueWindow[LMINFIL] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->minFillStitchLen / PFGRAN)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if (SelectedForm->fillType == ANGF || SelectedForm->fillType == TXANGF) {
			labelWindow[LFRMANG] = ffi::txtwin(stringTable[STR_TXT6], LabelWindowCoords);
			valueWindow[LFRMANG] = ffi::numwin(
			    fmt::format(L"{:.2f}", (gsl::narrow_cast<double>(SelectedForm->angleOrClipData.angle) * 180.0 / PI)),
			    ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (SelectedForm->fillType == ANGCLPF) {
			labelWindow[LSACANG] = ffi::txtwin(stringTable[STR_TXT6], LabelWindowCoords);
			valueWindow[LSACANG]
			    = ffi::numwin(fmt::format(L"{:.2f}", (gsl::narrow_cast<double>(SelectedForm->satinOrAngle.angle) * 180.0 / PI)),
			                  ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (SelectedForm->fillType == VCLPF || SelectedForm->fillType == HCLPF || SelectedForm->fillType == ANGCLPF) {
			labelWindow[LFRMFAZ] = ffi::txtwin(stringTable[STR_TXT18], LabelWindowCoords);
			valueWindow[LFRMFAZ] = ffi::numwin(fmt::format(L"{}", (SelectedForm->wordParam)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (SelectedForm->fillType == VRTF || SelectedForm->fillType == HORF || SelectedForm->fillType == ANGF
		    || texture::istx(ClosestFormToCursor)) {
			labelWindow[LBFILSQR] = ffi::txtwin(stringTable[STR_PRF2], LabelWindowCoords);
			if ((SelectedForm->extendedAttribute & AT_SQR) != 0U) {
				choice = stringTable[STR_SQR];
			}
			else {
				choice = stringTable[STR_PNTD];
			}
			valueWindow[LBFILSQR] = ffi::txtrwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
	}
	labelWindow[LFSTRT] = ffi::txtwin(stringTable[STR_FSTRT], LabelWindowCoords);
	if ((SelectedForm->extendedAttribute & AT_STRT) != 0U) {
		choice = stringTable[STR_ON];
	}
	else {
		choice = stringTable[STR_OFF];
	}
	valueWindow[LFSTRT] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((SelectedForm->extendedAttribute & AT_STRT) != 0U) {
		labelWindow[LDSTRT] = ffi::txtwin(stringTable[STR_FSTRT], LabelWindowCoords);
		valueWindow[LDSTRT] = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillStart)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LFEND] = ffi::txtwin(stringTable[STR_FEND], LabelWindowCoords);
	if ((SelectedForm->extendedAttribute & AT_END) != 0U) {
		choice = stringTable[STR_ON];
	}
	else {
		choice = stringTable[STR_OFF];
	}
	valueWindow[LFEND] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((SelectedForm->extendedAttribute & AT_END) != 0U) {
		labelWindow[LDEND] = ffi::txtwin(stringTable[STR_FEND], LabelWindowCoords);
		valueWindow[LDEND] = ffi::numwin(fmt::format(L"{}", (SelectedForm->fillEnd)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LBRD] = ffi::txtwin(stringTable[STR_TXT7], LabelWindowCoords);

	auto edgeFillType = SelectedForm->edgeType & NEGUND;
	if (edgeFillType >= EDGELAST) {
		edgeFillType = EDGELAST - 1U;
	}
	const auto iEdge = edgeFillType - 1U;

	valueWindow[LBRD] = ffi::txtrwin(stringTable[wrap::toSize(edgeFillType) + STR_EDG0], ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (edgeFillType != 0U) {
		labelWindow[LBRDCOL] = ffi::txtwin(stringTable[STR_TXT8], LabelWindowCoords);
		valueWindow[LBRDCOL] = ffi::numwin(fmt::format(L"{}", ((SelectedForm->borderColor & COLMSK) + 1U)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if ((edgeArray[iEdge] & BESPAC) != 0) {
			labelWindow[LBRDSPAC] = ffi::txtwin(stringTable[STR_TXT9], LabelWindowCoords);
			if (edgeFillType == EDGEPROPSAT || edgeFillType == EDGEOCHAIN || edgeFillType == EDGELCHAIN) {
				choice = fmt::format(L"{:.2f}", (SelectedForm->edgeSpacing / PFGRAN));
			}
			else {
				choice = fmt::format(L"{:.2f}", (SelectedForm->edgeSpacing / PFGRAN * 2));
			}
			valueWindow[LBRDSPAC] = ffi::numwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BPICSPAC) != 0) {
			labelWindow[LBRDPIC] = ffi::txtwin(stringTable[STR_TXT16], LabelWindowCoords);
			valueWindow[LBRDPIC] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeSpacing / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BEMAX) != 0) {
			labelWindow[LMAXBRD] = ffi::txtwin(stringTable[STR_TXT22], LabelWindowCoords);
			valueWindow[LMAXBRD]
			    = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->maxBorderStitchLen / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BELEN) != 0) {
			labelWindow[LBRDLEN] = ffi::txtwin(stringTable[STR_TXT10], LabelWindowCoords);
			valueWindow[LBRDLEN] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeStitchLen / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BEMIN) != 0) {
			labelWindow[LMINBRD] = ffi::txtwin(stringTable[STR_TXT23], LabelWindowCoords);
			valueWindow[LMINBRD]
			    = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->minBorderStitchLen / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BESIZ) != 0) {
			labelWindow[LBRDSIZ] = ffi::txtwin(stringTable[STR_TXT11], LabelWindowCoords);
			valueWindow[LBRDSIZ] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->borderSize / PFGRAN)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BRDPOS) != 0) {
			labelWindow[LBRDPOS] = ffi::txtwin(stringTable[STR_TXT18], LabelWindowCoords);
			valueWindow[LBRDPOS] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeStitchLen)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & CHNPOS) != 0) {
			labelWindow[LBRDPOS] = ffi::txtwin(stringTable[STR_TXT19], LabelWindowCoords);
			valueWindow[LBRDPOS] = ffi::numwin(fmt::format(L"{:.2f}", (SelectedForm->edgeStitchLen)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (edgeFillType == EDGEAPPL) {
			labelWindow[LAPCOL] = ffi::txtwin(stringTable[STR_TXT12], LabelWindowCoords);
			valueWindow[LAPCOL] = ffi::numwin(fmt::format(L"{}", ((SelectedForm->borderColor >> 4U) + 1U)), ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (edgeFillType == EDGEANGSAT || edgeFillType == EDGEAPPL || edgeFillType == EDGEPROPSAT) {
			labelWindow[LBRDUND] = ffi::txtwin(stringTable[STR_TXT17], LabelWindowCoords);
			if ((SelectedForm->edgeType & EGUND) != 0U) {
				choice = stringTable[STR_ON];
			}
			else {
				choice = stringTable[STR_OFF];
			}
			valueWindow[LBRDUND] = ffi::numwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if ((edgeArray[iEdge] & BCNRSIZ) != 0) {
			if (SelectedForm->edgeType == EDGEBHOL) {
				choice = fmt::format(L"{:.2f}", (form::getblen() / PFGRAN));
			}
			else {
				choice = fmt::format(L"{:.2f}", (form::getplen() / PFGRAN));
			}
			labelWindow[LBCSIZ] = ffi::txtwin(stringTable[STR_TXT13], LabelWindowCoords);
			valueWindow[LBCSIZ] = ffi::numwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
		if (SelectedForm->type == FRMLINE && ((edgeArray[iEdge] & BRDEND) != 0)) {
			labelWindow[LBSTRT] = ffi::txtwin(stringTable[STR_TXT14], LabelWindowCoords);
			if ((SelectedForm->attribute & SBLNT) != 0U) {
				choice = stringTable[STR_BLUNT];
			}
			else {
				choice = stringTable[STR_TAPR];
			}
			valueWindow[LBSTRT] = ffi::numwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
			labelWindow[LBFIN] = ffi::txtwin(stringTable[STR_TXT15], LabelWindowCoords);
			if ((SelectedForm->attribute & FBLNT) != 0U) {
				choice = stringTable[STR_BLUNT];
			}
			else {
				choice = stringTable[STR_TAPR];
			}
			valueWindow[LBFIN] = ffi::numwin(choice, ValueWindowCoords);
			ffi::nxtlin(formMenuEntryCount);
		}
	}
}

void formForms::refrm() {
	SelectedForm = &(FormList->operator[](ClosestFormToCursor));
	if (StateMap.testAndReset(StateFlag::PRFACT)) {
		DestroyWindow(PreferencesWindow);
		StateMap.reset(StateFlag::WASRT);
	}
	LabelWindowSize = ValueWindowSize = {};
	StateMap.set(StateFlag::REFCNT); // don't create windows - just size them
	auto formMenuEntryCount = 0U;
	ffi::refrmfn(formMenuEntryCount);
	if (FormDataSheet != nullptr) {
		DestroyWindow(FormDataSheet);
	}
	FormDataSheet = CreateWindow(L"STATIC", // NOLINT
	                             nullptr,
	                             WS_CHILD | WS_VISIBLE | WS_BORDER,
	                             ButtonWidthX3 + 3,
	                             3,
	                             LabelWindowSize.x + ValueWindowSize.x + 18,
	                             LabelWindowSize.y * formMenuEntryCount + 12,
	                             ThrEdWindow,
	                             nullptr,
	                             ThrEdInstance,
	                             nullptr);
	StateMap.reset(StateFlag::REFCNT); // this time create the windows
	ffi::refrmfn(formMenuEntryCount);
}

void formForms::sidwnd(HWND wnd) noexcept {
	auto windowRect = RECT { 0L, 0L, 0L, 0L };

	MsgIndex                 = 0;
	SideWindowEntryBuffer[0] = 0;
	const auto savedChoice   = FormMenuChoice;
	thred::unsid();
	FormMenuChoice = savedChoice;
	GetWindowRect(wnd, &windowRect);
	GetWindowRect(FormDataSheet, &MsgRect);
	SideMessageWindow = CreateWindow(L"STATIC", // NOLINT
	                                 nullptr,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 MsgRect.right - ThredWindowOrigin.x + 3,
	                                 windowRect.top - ThredWindowOrigin.y - 3,
	                                 ButtonWidthX3,
	                                 windowRect.bottom - windowRect.top + 3,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
}

void formForms::prfsid(HWND wnd) noexcept {
	auto windowRect = RECT { 0L, 0L, 0L, 0L };

	MsgIndex                 = 0;
	SideWindowEntryBuffer[0] = 0;
	thred::unsid();
	GetWindowRect(wnd, &windowRect);
	GetClientRect(PreferencesWindow, &MsgRect);
	SideMessageWindow = CreateWindow(L"STATIC", // NOLINT
	                                 nullptr,
	                                 WS_BORDER | WS_CHILD | WS_VISIBLE,
	                                 windowRect.right - ThredWindowOrigin.x + 6,
	                                 windowRect.top - ThredWindowOrigin.y - 3,
	                                 ValueWindowSize.x,
	                                 windowRect.bottom - windowRect.top + 3,
	                                 ThrEdWindow,
	                                 nullptr,
	                                 ThrEdInstance,
	                                 nullptr);
}

void formForms::internal::prftwin(const std::wstring& text) noexcept {
	CreateWindow(L"STATIC", // NOLINT
	             text.c_str(),
	             WS_CHILD | WS_VISIBLE,
	             LabelWindowCoords.left,
	             LabelWindowCoords.top,
	             LabelWindowCoords.right - LabelWindowCoords.left,
	             LabelWindowCoords.bottom - LabelWindowCoords.top,
	             PreferencesWindow,
	             nullptr,
	             ThrEdInstance,
	             nullptr);
}

auto formForms::internal::prfnwin(const std::wstring& text) noexcept -> HWND {
	return CreateWindow(L"STATIC", // NOLINT
	                    text.c_str(),
	                    SS_NOTIFY | SS_RIGHT | WS_BORDER | WS_CHILD | WS_VISIBLE,
	                    ValueWindowCoords.left,
	                    ValueWindowCoords.top,
	                    ValueWindowCoords.right - ValueWindowCoords.left,
	                    ValueWindowCoords.bottom - ValueWindowCoords.top,
	                    PreferencesWindow,
	                    nullptr,
	                    ThrEdInstance,
	                    nullptr);
}

void formForms::internal::prflin(const std::wstring& msg, uint32_t row) noexcept {
	ffi::prftwin((*StringTable)[row]);
	(*ValueWindow)[row - STR_PRF0] = ffi::prfnwin(msg);
	ffi::nxtlinprf();
}

void formForms::prfmsg() {
	auto choice         = std::wstring {};
	auto preferenceRect = RECT { 0L, 0L, 0L, 0L };

	if (StateMap.testAndReset(StateFlag::INSRT)) {
		StateMap.set(StateFlag::WASRT);
	}
	StateMap.reset(StateFlag::BIGBOX);
	SelectedFormList->clear();
	if (FormDataSheet != nullptr) {
		thred::undat();
		thred::unsid();
		FormMenuChoice = 0U;
	}
	LabelWindowSize.x = LabelWindowSize.y = 0;
	ValueWindowSize.x = ValueWindowSize.y = 0;
	formForms::maxtsiz((*StringTable)[STR_PRF0 + 4U], LabelWindowSize);
	formForms::maxtsiz((*StringTable)[STR_TAPR], ValueWindowSize);
	LabelWindowSize.x = formForms::maxwid(STR_PRF0, STR_PRF27);
	LabelWindowSize.x += 4;
	DestroyWindow(PreferencesWindow);
	const auto windowWidth = LabelWindowSize.x + ValueWindowSize.x + 18;
	PreferencesWindow      = CreateWindow(L"STATIC", // NOLINT
                                     nullptr,
                                     WS_CHILD | WS_VISIBLE | WS_BORDER,
                                     ButtonWidthX3 + 3,
                                     3,
                                     windowWidth,
                                     LabelWindowSize.y * PRFLINS + 12,
                                     ThrEdWindow,
                                     nullptr,
                                     ThrEdInstance,
                                     nullptr);
	auto preferenceDC      = GetDC(PreferencesWindow);
	GetClientRect(PreferencesWindow, &preferenceRect);
	FillRect(preferenceDC, &preferenceRect, GetSysColorBrush(COLOR_WINDOW));
	LabelWindowCoords.top = ValueWindowCoords.top = 3;
	LabelWindowCoords.bottom = ValueWindowCoords.bottom = 3 + LabelWindowSize.y;
	LabelWindowCoords.left                              = 3;
	LabelWindowCoords.right                             = 3 + LabelWindowSize.x;
	ValueWindowCoords.left                              = 6 + LabelWindowSize.x;
	ValueWindowCoords.right                             = 6 + LabelWindowSize.x + ValueWindowSize.x + 6;
	ffi::prflin(fmt::format(L"{}", (AppliqueColor + 1U)), STR_PRF10);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.AppStitchLen / PFGRAN)), STR_PRF29);
	ffi::prflin(fmt::format(L"{:.2f}", (BorderWidth / PFGRAN)), STR_PRF3);
	ffi::prflin(fmt::format(L"{:.2f}", (ButtonholeCornerLength / PFGRAN)), STR_PRF14);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.chainSpace / PFGRAN)), STR_PRF23);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.chainRatio)), STR_PRF24);
	ffi::prflin(fmt::format(L"{:.2f} mm", (IniFile.clipOffset / PFGRAN)), STR_PRF21);
	ffi::prflin(fmt::format(L"{}", (IniFile.fillPhase)), STR_PRF22);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.eggRatio)), STR_PRF26);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.fillAngle / PI_F * 180.0F)), STR_PRF1);
	if (UserFlagMap.test(UserFlag::SQRFIL)) {
		choice = (*StringTable)[STR_SQR];
	}
	else {
		choice = (*StringTable)[STR_PNTD];
	}
	ffi::prflin(choice, STR_PRF2);
	ffi::prflin(fmt::format(L"{:.2f}", (LineSpacing / PFGRAN)), STR_PRF0);
	ffi::prflin(fmt::format(L"{}", (thred::duthrsh(ShowStitchThreshold))), STR_PRF7);
	ffi::prflin(fmt::format(L"{:.2f} mm", (IniFile.gridSize / PFGRAN)), STR_PRF20);
	form::sethup();
	ffi::prflin(fmt::format(L"{}", (*StringTable)[wrap::toSize(IniFile.hoopType) - 1U + STR_HUP0]), STR_PRF17);
	ffi::prflin(fmt::format(L"{:.0f} mm", (IniFile.hoopSizeY / PFGRAN)), STR_PRF27);
	ffi::prflin(fmt::format(L"{:.0f} mm", (IniFile.hoopSizeX / PFGRAN)), STR_PRF18);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.lensRatio)), STR_PRF28);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.cursorNudgeStep)), STR_PRF25);
	ffi::prflin(fmt::format(L"{:.2f}", (PicotSpacing / PFGRAN)), STR_PRF16);
	if (UserFlagMap.test(UserFlag::BLUNT)) {
		choice = (*StringTable)[STR_BLUNT];
	}
	else {
		choice = (*StringTable)[STR_TAPR];
	}
	ffi::prflin(choice, STR_PRF15);
	if (UserFlagMap.test(UserFlag::DUND)) {
		choice = (*StringTable)[STR_ON];
	}
	else {
		choice = (*StringTable)[STR_OFF];
	}
	ffi::prflin(choice, STR_PRF19);
	ffi::prflin(fmt::format(L"{:.2f}", (SmallStitchLength / PFGRAN)), STR_PRF9);
	ffi::prflin(fmt::format(L"{:.2f}", (SnapLength / PFGRAN)), STR_PRF11);
	ffi::prflin(fmt::format(L"{:.2f}", (SpiralWrap)), STR_PRF13);
	ffi::prflin(fmt::format(L"{:.2f}", (StarRatio)), STR_PRF12);
	ffi::prflin(fmt::format(L"{}", (thred::duthrsh(StitchBoxesThreshold))), STR_PRF8);
	ffi::prflin(fmt::format(L"{:.2f}", (IniFile.maxStitchLength / PFGRAN)), STR_PRF4);
	ffi::prflin(fmt::format(L"{:.2f}", (UserStitchLength / PFGRAN)), STR_PRF5);
	ffi::prflin(fmt::format(L"{:.2f}", (MinStitchLength / PFGRAN)), STR_PRF6);
	StateMap.set(StateFlag::PRFACT);
	ReleaseDC(ThrEdWindow, preferenceDC);
}

void formForms::frmnum() {
	auto fmtStr = std::wstring {};

	displayText::loadString(fmtStr, IDS_FRML);
	if (!FormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
		displayText::shoMsg(fmt::format(fmtStr, FormList->size()));
		StateMap.set(StateFlag::NUMIN);
		StateMap.set(StateFlag::ENTRFNUM);
		displayText::numWnd();
	}
	else {
		displayText::shoseln(IDS_FRM1MSG, IDS_SETFRM);
	}
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void formForms::internal::chkdaz() {
	if (IniFile.daisyPetalPoints == 0U) {
		IniFile.daisyPetalPoints = 1U;
	}
	if (IniFile.daisyInnerCount == 0U) {
		IniFile.daisyInnerCount = 1U;
	}
	if (IniFile.daisyPetalCount == 0U) {
		IniFile.daisyPetalCount = 1U;
	}
	if (IniFile.daisyHeartCount > gsl::narrow<decltype(IniFile.daisyHeartCount)>(IniFile.daisyPetalPoints)) {
		IniFile.daisyHeartCount = gsl::narrow<decltype(IniFile.daisyHeartCount)>(IniFile.daisyPetalPoints);
	}
}

void formForms::dazdef() {
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

void formForms::internal::initdaz(HWND hWinDialog) {
	ffi::chkdaz();
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS), fmt::format(L"{}", IniFile.daisyPetalPoints).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT), fmt::format(L"{}", IniFile.daisyHeartCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN), fmt::format(L"{:.2f}", IniFile.daisyDiameter).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ), fmt::format(L"{:.2f}", IniFile.daisyHoleDiameter).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS), fmt::format(L"{}", IniFile.daisyInnerCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS), fmt::format(L"{}", IniFile.daisyPetalCount).c_str());
	SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN), fmt::format(L"{:.2f}", IniFile.daisyPetalLen).c_str());
	auto flag = 1U;
	if (!UserFlagMap.test(UserFlag::DAZHOL)) {
		flag = 0U;
	}
	CheckDlgButton(hWinDialog, IDC_HOLE, flag);
	flag = 1U;
	if (!UserFlagMap.test(UserFlag::DAZD)) {
		flag = 0U;
	}
	CheckDlgButton(hWinDialog, IDC_DLIN, flag);
	auto daisyType = std::wstring {};
	for (const auto DaisyTypeString : DaisyTypeStrings) {
		displayText::loadString(daisyType, DaisyTypeString);
		GSL_SUPPRESS(26490)
		SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(daisyType.c_str())); // NOLINT
	}
	SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
}

auto CALLBACK formForms::internal::dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
	UNREFERENCED_PARAMETER(lparam);

	switch (umsg) { // NOLINT
	case WM_INITDIALOG: {
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		ffi::initdaz(hwndlg);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) { // NOLINT
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			wchar_t buffer[HBUFSIZ] = { 0 };
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyPetalPoints = wrap::wcstoi<uint32_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyHeartCount = wrap::wcstoi<uint16_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyDiameter = wrap::wcstof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyHoleDiameter = wrap::wcstof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyInnerCount = wrap::wcstoi<uint32_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyPetalCount = wrap::wcstoi<uint32_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.daisyPetalLen = wrap::wcstof(buffer);
			if (IsDlgButtonChecked(hwndlg, IDC_HOLE) != 0U) {
				UserFlagMap.set(UserFlag::DAZHOL);
			}
			else {
				UserFlagMap.reset(UserFlag::DAZHOL);
			}
			if (IsDlgButtonChecked(hwndlg, IDC_DLIN) != 0U) {
				UserFlagMap.set(UserFlag::DAZD);
			}
			else {
				UserFlagMap.reset(UserFlag::DAZD);
			}
			GetWindowText(GetDlgItem(hwndlg, IDC_DAZTYP), static_cast<LPTSTR>(buffer), HBUFSIZ);
			wchar_t compareBuffer[HBUFSIZ] = { 0 };
			for (uint8_t iType = 0U; iType < 6U; iType++) {
				LoadString(ThrEdInstance, DaisyTypeStrings[iType], static_cast<LPTSTR>(compareBuffer), HBUFSIZ);
				if (wcscmp(static_cast<wchar_t*>(buffer), static_cast<wchar_t*>(compareBuffer)) == 0) {
					IniFile.daisyBorderType = iType;
					break;
				}
			}
			ffi::chkdaz();
			EndDialog(hwndlg, 1);
			break;
		}
		case IDC_DAZRST: {
			formForms::dazdef();
			ffi::initdaz(hwndlg);
			break;
		}
		case IDC_DLIN: {
			if (IsDlgButtonChecked(hwndlg, IDC_DLIN) != 0U) {
				CheckDlgButton(hwndlg, IDC_HOLE, BST_CHECKED);
			}
			break;
		}
		case IDC_HOLE: {
			if (IsDlgButtonChecked(hwndlg, IDC_HOLE) == 0U) {
				CheckDlgButton(hwndlg, IDC_DLIN, BST_UNCHECKED);
			}
			break;
		}
		}
	}
	}
	return FALSE;
}

void formForms::dasyfrm() {
	thred::unmsg();
	if (!DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::dasyproc))) { // NOLINT
		StateMap.reset(StateFlag::FORMIN);
		return;
	}
	const auto referencePoint = fPOINT { form::midl(ZoomRect.right, ZoomRect.left), form::midl(ZoomRect.top, ZoomRect.bottom) };
	FormList->push_back(FRMHED {});
	SelectedForm              = &FormList->back();
	ClosestFormToCursor       = wrap::toUnsigned(FormList->size() - 1U);
	SelectedForm->vertexIndex = wrap::toUnsigned(FormVertices->size());
	SelectedForm->attribute   = gsl::narrow<decltype(SelectedForm->attribute)>(ActiveLayer << 1U);
	form::fvars(ClosestFormToCursor);
	auto       maximumXsize = ZoomRect.right - ZoomRect.left;
	const auto maximumYsize = ZoomRect.top - ZoomRect.bottom;
	if (maximumYsize > maximumXsize) {
		maximumXsize = maximumYsize;
	}
	maximumXsize /= 6.0F;
	auto       diameter     = IniFile.daisyDiameter;
	auto       petalLength  = IniFile.daisyPetalLen;
	auto       holeDiameter = IniFile.daisyHoleDiameter;
	const auto ratio        = maximumXsize / (diameter + petalLength);
	diameter *= ratio;
	petalLength *= ratio;
	holeDiameter *= ratio;
	SelectedForm->type = FRMFPOLY;
	auto iVertex       = 0U;
	auto fref          = 0U;
	if (UserFlagMap.test(UserFlag::DAZHOL)) {
		auto       angle            = PI_F2;
		const auto holeVertexCount  = IniFile.daisyPetalCount * IniFile.daisyInnerCount;
		const auto holeSegmentAngle = PI_F2 / holeVertexCount;
		FormVertices->push_back(fPOINT { referencePoint.x + diameter * cos(angle), referencePoint.y + diameter * sin(angle) });
		iVertex++;
		for (auto iSegment = 0U; iSegment < holeVertexCount + 1U; iSegment++) {
			FormVertices->push_back(
			    fPOINT { referencePoint.x + holeDiameter * cos(angle), referencePoint.y + holeDiameter * sin(angle) });
			iVertex++;
			angle -= holeSegmentAngle;
		}
		fref = iVertex;
	}
	auto       petalVertexCount = IniFile.daisyPetalCount * IniFile.daisyPetalPoints;
	auto       petalPointCount  = IniFile.daisyPetalPoints;
	const auto borderType       = IniFile.daisyBorderType;
	if (borderType == DHART) {
		petalPointCount  = (IniFile.daisyHeartCount + 1U) * 2U;
		petalVertexCount = IniFile.daisyPetalCount * petalPointCount;
	}
	const auto petalSegmentAngle = PI_F2 / petalVertexCount;
	const auto deltaPetalAngle   = PI_F / IniFile.daisyPetalPoints;
	if (UserFlagMap.test(UserFlag::DAZD)) {
		SelectedForm->satinGuideCount    = IniFile.daisyPetalCount - 1U;
		SelectedForm->wordParam          = IniFile.daisyPetalCount * IniFile.daisyInnerCount + 1U;
		SelectedForm->satinOrAngle.guide = satin::adsatk(IniFile.daisyPetalCount - 1);
	}
	const auto halfPetalPointCount = IniFile.daisyPetalPoints / 2;
	auto       angle               = 0.0F;
	for (auto iMacroPetal = 0U; iMacroPetal < IniFile.daisyPetalCount; iMacroPetal++) {
		auto petalPointAngle         = 0.0F;
		PseudoRandomValue            = SEED;
		auto distanceFromDaisyCenter = 0.0F;
		for (auto iPoint = 0U; iPoint < petalPointCount; iPoint++) {
			switch (borderType) {
			case DSIN: {
				distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
				petalPointAngle += deltaPetalAngle;
				break;
			}
			case DRAMP: {
				distanceFromDaisyCenter
				    = diameter + (wrap::toFloat(iPoint) / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
				break;
			}
			case DSAW: {
				auto sawPointCount = 0U;
				if (iPoint > halfPetalPointCount) {
					sawPointCount = IniFile.daisyPetalPoints - iPoint;
				}
				else {
					sawPointCount = iPoint;
				}
				distanceFromDaisyCenter
				    = diameter + (wrap::toFloat(sawPointCount) / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
				break;
			}
			case DRAG: {
				distanceFromDaisyCenter = diameter
				                          + (wrap::toFloat(form::psg() % IniFile.daisyPetalPoints)
				                             / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
				break;
			}
			case DCOG: {
				distanceFromDaisyCenter = diameter;
				if (iPoint > halfPetalPointCount) {
					distanceFromDaisyCenter += petalLength;
				}
				break;
			}
			case DHART: {
				distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
				if (iPoint > IniFile.daisyHeartCount) {
					petalPointAngle -= deltaPetalAngle;
				}
				else {
					petalPointAngle += deltaPetalAngle;
				}
				break;
			}
			default: {
				throw;
			}
			}
			FormVertices->push_back(fPOINT { referencePoint.x + cos(angle) * distanceFromDaisyCenter,
			                                 referencePoint.y + sin(angle) * distanceFromDaisyCenter });
			iVertex++;
			angle += petalSegmentAngle;
			auto guideIt = std::next(SatinGuides->begin(), SelectedForm->satinOrAngle.guide);
			if (UserFlagMap.test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
				guideIt[iMacroPetal].start  = (IniFile.daisyPetalCount - iMacroPetal - 1) * IniFile.daisyInnerCount + 1U;
				guideIt[iMacroPetal].finish = iVertex;
			}
		}
	}
	auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
	if (UserFlagMap.test(UserFlag::DAZHOL)) {
		vertexIt[fref - 1U].y += 0.01F;
		vertexIt[fref].y += 0.01F;
	}
	SelectedForm->vertexCount = iVertex;
	if (UserFlagMap.test(UserFlag::DAZD)) {
		SelectedForm->type      = SAT;
		SelectedForm->attribute = 1;
	}
	StateMap.set(StateFlag::INIT);
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	for (auto iMacroPetal = 0U; iMacroPetal < iVertex; iMacroPetal++) {
		vertexIt[iMacroPetal].x -= SelectedForm->rectangle.left;
		vertexIt[iMacroPetal].y -= SelectedForm->rectangle.bottom;
	}
	FormMoveDelta      = fPOINT {};
	NewFormVertexCount = iVertex + 1U;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	form::mdufrm();
}

void formForms::internal::initTearDlg(HWND hwndlg) {
	SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), fmt::format(L"{:d}", IniFile.formSides).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), fmt::format(L"{:.3f}", IniFile.tearTailLength).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), fmt::format(L"{:.3f}", IniFile.tearTwistStep / PFGRAN).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), fmt::format(L"{:.3f}", IniFile.tearTwistRatio).c_str());
}

auto CALLBACK formForms::internal::tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
	UNREFERENCED_PARAMETER(lparam);

	switch (umsg) { // NOLINT
	case WM_INITDIALOG: {
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
#ifdef TESTCODE
		const auto uDpi     = GetDpiForWindow(hwndlg);
		RECT       rcClient = {};
		GetWindowRect(hwndlg, &rcClient);
		auto uWidth  = MulDiv((rcClient.right - rcClient.left), uDpi, 96);
		auto uHeight = MulDiv((rcClient.bottom - rcClient.top), uDpi, 96);
		SetWindowPos(hwndlg, nullptr, rcClient.left, rcClient.top, uWidth, uHeight, SWP_NOZORDER | SWP_NOACTIVATE);

		auto hFont = displayText::getThrEdFont(400);
		// SendMessage(hwndlg, WM_SETFONT, (WPARAM)hFont, MAKELPARAM(TRUE, 0));
		EnumChildWindows(
		    hwndlg,
		    [](HWND p_hWnd, LPARAM lParam) -> BOOL {
			    SendMessage(p_hWnd, WM_SETFONT, (WPARAM)lParam, MAKELPARAM(TRUE, 0));
			    return TRUE;
		    },
		    (LPARAM)hFont);
#endif // !TESTCODE
		ffi::initTearDlg(hwndlg);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) { // NOLINT
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			wchar_t buffer[HBUFSIZ] = { 0 };
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.formSides = wrap::wcstoi<uint16_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.tearTailLength = wrap::wcstof(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.tearTwistStep = wrap::wcstof(buffer) * PFGRAN;
			GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.tearTwistRatio = wrap::wcstof(buffer);
			EndDialog(hwndlg, 1);
			break;
		}
		case IDC_DEFTEAR: {
			IniFile.formSides      = 20;
			IniFile.tearTailLength = 1.1F;
			IniFile.tearTwistStep  = 0.0F;
			IniFile.tearTwistRatio = 1.6F;
			ffi::initTearDlg(hwndlg);
			break;
		}
		case IDC_DEFPAIS: {
			IniFile.formSides      = 24U;
			IniFile.tearTailLength = 1.15F;
			IniFile.tearTwistStep  = 0.3F * PFGRAN;
			IniFile.tearTwistRatio = 1.8F;
			ffi::initTearDlg(hwndlg);
			break;
		}
		}
	}
	}
	return FALSE;
}

void formForms::setear() {
	thred::unmsg();
	const auto nResult
	    = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::tearprc)); // NOLINT
	if (nResult > 0) {
		thred::savdo();
		auto twistStep = IniFile.tearTwistStep;
		form::durpoli(IniFile.formSides);
		form::fvars(wrap::toUnsigned(FormList->size() - 1U));
		auto       vertexIt         = std::next(FormVertices->begin(), CurrentVertexIndex);
		const auto count            = wrap::toSize(VertexCount) / 4U;
		const auto middle           = form::midl(vertexIt[1].x, vertexIt[0].x);
		auto       step             = vertexIt[count + 1U].y - vertexIt[count].y;
		auto       verticalPosition = vertexIt[count + 1U].y;
		auto       iLeftVertices    = wrap::toSize(VertexCount) - count;
		auto       iRightVertices   = count + 1U;
		for (auto iStep = 0U; iStep < count; iStep++) {
			vertexIt[iLeftVertices].y  = verticalPosition;
			vertexIt[iRightVertices].y = vertexIt[iLeftVertices].y;
			vertexIt[iRightVertices].x += twistStep;
			vertexIt[iLeftVertices].x += twistStep;
			twistStep *= IniFile.tearTwistRatio;
			verticalPosition -= step;
			step *= IniFile.tearTailLength;
			iRightVertices--;
			iLeftVertices++;
		}
		vertexIt[0].y = vertexIt[1].y = verticalPosition;
		vertexIt[0].x += twistStep;
		vertexIt[1].x += twistStep;
		verticalPosition -= step / 2.0F;
		FormVertices->push_back(vertexIt[0]);
		vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex); // iterator invalidated by push_back
		if (twistStep != 0.0F) {
			vertexIt[0].x = vertexIt[1].x + twistStep / 4.0F;
		}
		else {
			vertexIt[0].x = middle;
		}
		vertexIt[0].y = verticalPosition;
		SelectedForm->vertexCount++;
		NewFormVertexCount++;
		StateMap.set(StateFlag::FORMSEL);
		form::fvars(wrap::toUnsigned(FormList->size() - 1U));
		form::frmout(wrap::toUnsigned(FormList->size() - 1U));
		form::flipv();
		StateMap.reset(StateFlag::FORMSEL);
		const auto size = fPOINT { SelectedForm->rectangle.right - SelectedForm->rectangle.left,
			                       SelectedForm->rectangle.top - SelectedForm->rectangle.bottom };

		auto horizontalRatio = UnzoomedRect.x / 4.0F / size.x;
		if (horizontalRatio > 1.0F) {
			horizontalRatio = 1.0F;
		}
		const auto verticalRatio = UnzoomedRect.y / 4.0F / size.y;
		if (verticalRatio < horizontalRatio) {
			horizontalRatio = verticalRatio;
		}
		if (horizontalRatio < 1.0F) {
			for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].x = (vertexIt[iVertex].x - vertexIt[0].x) * horizontalRatio + vertexIt[0].x;
				vertexIt[iVertex].y = (vertexIt[iVertex].y - vertexIt[0].y) * horizontalRatio + vertexIt[0].y;
			}
		}
		form::frmout(wrap::toUnsigned(FormList->size() - 1U));
		for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
			vertexIt[iVertex].x -= SelectedForm->rectangle.left;
			vertexIt[iVertex].y -= SelectedForm->rectangle.bottom;
		}
	}
}

void formForms::internal::wavinit(HWND hwndlg) {
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), fmt::format(L"{}", IniFile.wavePoints).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), fmt::format(L"{}", IniFile.waveStart).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), fmt::format(L"{}", IniFile.waveEnd).c_str());
	SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), fmt::format(L"{}", IniFile.waveLobes).c_str());
}

auto CALLBACK formForms::internal::wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
	UNREFERENCED_PARAMETER(lparam);

	switch (umsg) { // NOLINT
	case WM_INITDIALOG: {
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		ffi::wavinit(hwndlg);
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) { // NOLINT
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			wchar_t buffer[HBUFSIZ] = { 0 };
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.wavePoints = wrap::wcstoi<uint16_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.waveStart = wrap::wcstoi<uint16_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.waveEnd = wrap::wcstoi<uint8_t>(buffer);
			GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), static_cast<LPTSTR>(buffer), HBUFSIZ);
			IniFile.waveLobes = wrap::wcstoi<uint8_t>(buffer);
			if (IniFile.wavePoints > 100U) {
				IniFile.wavePoints = 100U;
			}
			if (IniFile.wavePoints < 3) {
				IniFile.wavePoints = 3;
			}
			if (IniFile.waveStart == IniFile.waveEnd) {
				IniFile.waveEnd += (IniFile.wavePoints >> 2U);
			}
			IniFile.waveStart %= IniFile.wavePoints;
			IniFile.waveEnd %= IniFile.wavePoints;
			EndDialog(hwndlg, 1);
			break;
		}
		case IDC_DEFWAV: {
			IniFile.wavePoints = IWAVPNTS;
			IniFile.waveStart  = IWAVSTRT;
			IniFile.waveEnd    = IWAVEND;
			IniFile.waveLobes  = IWAVS;
			ffi::wavinit(hwndlg);
			break;
		}
		}
	}
	}
	return FALSE;
}

void formForms::wavfrm() {
	thred::unmsg();
	if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::wavprc))) { // NOLINT
		thred::savdo();
		auto points = std::vector<fPOINT> {};
		points.reserve(IniFile.wavePoints);
		// reuse regular polygon code to build the template for points
		form::durpoli(IniFile.wavePoints);
		form::mdufrm();
		auto iPoint    = 0U;
		auto waveIndex = IniFile.waveStart;
		auto vertexIt  = std::next(FormVertices->begin(), CurrentVertexIndex);
		while (waveIndex != IniFile.waveEnd && iPoint < IniFile.wavePoints) {
			const uint16_t iNextVertex = (waveIndex + 1U) % IniFile.wavePoints;

			points.emplace_back(-vertexIt[iNextVertex].x + vertexIt[waveIndex].x,
			                    -vertexIt[iNextVertex].y + vertexIt[waveIndex].y);
			iPoint++;
			waveIndex = iNextVertex;
		}
		const auto count           = iPoint;
		auto       iVertex         = 0U;
		auto       currentPosition = fPOINT {};
		const auto formVerticesSize
		    = (IniFile.waveLobes * count) + 1 - IniFile.wavePoints; // account for vertices already allocated by durpoli above
		FormVertices->resize(FormVertices->size() + formVerticesSize);
		vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex); // resize may invalidate iterator
		for (auto iLobe = 0U; iLobe < IniFile.waveLobes; iLobe++) {
			if ((iLobe & 1U) != 0U) {
				for (auto index = 0U; index < count; index++) {
					vertexIt[iVertex] = currentPosition;
					iVertex++;
					currentPosition.x += points[index].x;
					currentPosition.y += points[index].y;
				}
			}
			else {
				for (auto index = count; index != 0; index--) {
					vertexIt[iVertex] = currentPosition;
					iVertex++;
					currentPosition.x += points[index - 1U].x;
					currentPosition.y += points[index - 1U].y;
				}
			}
		}
		vertexIt[iVertex]        = currentPosition;
		const auto vertexCount   = iVertex + 1U;
		const auto rotationAngle = -atan2(vertexIt[iVertex].y - vertexIt[0].y, vertexIt[iVertex].x - vertexIt[0].x);
		for (auto index = 0U; index < vertexCount; index++) {
			thred::rotflt(vertexIt[index], rotationAngle, { 0.0, 0.0 });
		}
		SelectedForm->type        = FRMLINE;
		SelectedForm->vertexCount = vertexCount;
		form::frmout(wrap::toUnsigned(FormList->size() - 1U));
		StateMap.reset(StateFlag::FORMSEL);
		const auto selectedSize    = fPOINT { SelectedForm->rectangle.right - SelectedForm->rectangle.left,
                                           SelectedForm->rectangle.top - SelectedForm->rectangle.bottom };
		auto       horizontalRatio = UnzoomedRect.x / 4.0F / selectedSize.x;
		if (horizontalRatio > 1) {
			horizontalRatio = 1.0F;
		}
		const auto verticalRatio = UnzoomedRect.y / 4.0F / selectedSize.y;
		if (verticalRatio < horizontalRatio) {
			horizontalRatio = verticalRatio;
		}
		if (horizontalRatio < 1.0F) {
			for (auto index = 0U; index < vertexCount; index++) {
				vertexIt[index].x = (vertexIt[index].x - vertexIt[0].x) * horizontalRatio + vertexIt[0].x;
				vertexIt[index].y = (vertexIt[index].y - vertexIt[0].y) * horizontalRatio + vertexIt[0].y;
			}
		}
		form::frmout(wrap::toUnsigned(FormList->size() - 1U));
		for (auto index = 0U; index < vertexCount; index++) {
			vertexIt[index].x -= SelectedForm->rectangle.left;
			vertexIt[index].y -= SelectedForm->rectangle.bottom;
		}
		FormMoveDelta      = fPOINT {};
		NewFormVertexCount = vertexCount + 1U;
		form::setmfrm();
		form::mdufrm();
	}
}
