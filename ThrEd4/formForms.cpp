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
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "formForms.h"

namespace ffi = formForms::internal;
namespace dT  = displayText;

constexpr auto TXTMARG  = 3L; // text margin in pixels
constexpr auto TXTMARG2 = 6L; // wide text margin in pixels

static auto LabelWindowCoords = RECT {};  // location of left windows in the form data sheet
static auto LabelWindowSize   = POINT {}; // size of the left windows in the form data sheet
static auto ValueWindowCoords = RECT {};  // location of right windows in the form data sheet
static auto ValueWindowSize   = POINT {}; // size of the right windows in the form data sheet

auto const DaisyTypeStrings = std::array<uint16_t, 6> {
    IDS_DAZCRV,
    IDS_DAZSAW,
    IDS_DAZRMP,
    IDS_DAZRAG,
    IDS_DAZCOG,
    IDS_DAZHART,
};

void formForms::maxtsiz(std::wstring const& label, POINT& textSize) {
  auto labelSize = SIZE {0L, 0L};
  wrap::getTextExtentPoint32(GetDC(ThrEdWindow), label.data(), wrap::toUnsigned(label.size()), &labelSize);
  textSize.y = labelSize.cy;
  if (labelSize.cx > textSize.x) {
	textSize.x = labelSize.cx;
  }
}

auto formForms::internal::maxwid() {
  auto prfList = std::array<uint32_t, 28> {
	IDS_PRF0,  IDS_PRF1,  IDS_PRF2,  IDS_PRF3,  IDS_PRF4,  IDS_PRF5,  IDS_PRF6,
	  IDS_PRF7,  IDS_PRF8,  IDS_PRF9,  IDS_PRF10, IDS_PRF11, IDS_PRF12, IDS_PRF13,
	  IDS_PRF14, IDS_PRF15, IDS_PRF16, IDS_PRF17, IDS_PRF18, IDS_PRF19, IDS_PRF20,
	  IDS_PRF21, IDS_PRF22, IDS_PRF23, IDS_PRF24, IDS_PRF25, IDS_PRF26, IDS_PRF27};
  auto textSize = POINT {0L, 0L};
  for (auto item : prfList) {
	formForms::maxtsiz(dT::loadStr(item), textSize);
  }
  return textSize.x + TXTMARG2;
}

auto formForms::internal::txtwin(std::wstring const& windowName, RECT const& location) -> HWND {
  if (StateMap->test(StateFlag::REFCNT)) {
	formForms::maxtsiz(windowName, LabelWindowSize);
	return nullptr;
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  return CreateWindow(L"STATIC",
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

auto formForms::internal::txtrwin(std::wstring const& winName, RECT const& location) -> HWND {
  if (StateMap->test(StateFlag::REFCNT)) {
	formForms::maxtsiz(winName, ValueWindowSize);
	return nullptr;
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  return CreateWindow(L"STATIC",
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

auto formForms::internal::numwin(std::wstring const& winName, RECT const& location) -> HWND {
  if (StateMap->test(StateFlag::REFCNT)) {
	formForms::maxtsiz(winName, ValueWindowSize);
	return nullptr;
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  return CreateWindow(L"STATIC",
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
  ++formMenuEntryCount;
  ffi::nxtlinprf();
}

void formForms::internal::nxtlinprf() noexcept {
  LabelWindowCoords.top += LabelWindowSize.y;
  LabelWindowCoords.bottom += LabelWindowSize.y;
  ValueWindowCoords.top += ValueWindowSize.y;
  ValueWindowCoords.bottom += ValueWindowSize.y;
}

void formForms::internal::refrmfn(FRMHED const& form, uint32_t& formMenuEntryCount) {
  static constexpr auto edgeArray = std::array<uint16_t, 13> {
      MEGLIN, MEGBLD, MEGCLP, MEGSAT, MEGAP, MEGPRP, MEGHOL, MEGPIC, MEGDUB, MEGCHNH, MEGCHNL, MEGCLPX, 0};
  auto const strOn         = dT::loadStr(IDS_ON);
  auto const strOff        = dT::loadStr(IDS_OFF);
  auto&      labelWindow   = *LabelWindow;
  auto&      valueWindow   = *ValueWindow;
  ValueWindowCoords.top    = TXTMARG;
  LabelWindowCoords.top    = ValueWindowCoords.top;
  ValueWindowCoords.bottom = TXTMARG + LabelWindowSize.y;
  LabelWindowCoords.bottom = ValueWindowCoords.bottom;
  LabelWindowCoords.left   = TXTMARG;
  LabelWindowCoords.right  = TXTMARG + LabelWindowSize.x;
  ValueWindowCoords.left   = TXTMARG2 + LabelWindowSize.x;
  ValueWindowCoords.right  = TXTMARG2 + LabelWindowSize.x + ValueWindowSize.x + TXTMARG2;
  labelWindow[LFRM]        = ffi::txtwin(dT::loadStr(IDS_TXT0), LabelWindowCoords);
  auto choice              = (form.type == FRMLINE) ? dT::loadStr(IDS_EDG1) : dT::loadStr(IDS_FREH);
  valueWindow[LFRM]        = ffi::txtrwin(choice, ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  labelWindow[LLAYR] = ffi::txtwin(dT::loadStr(IDS_TXT1), LabelWindowCoords);
  // NOLINTNEXTLINE hicpp-signed-bitwise
  auto       pLayr   = wrap::next(LAYRLIST.begin(), (form.attribute & FRMLMSK) >> 1U);
  valueWindow[LLAYR] = ffi::txtrwin(dT::loadStr(pLayr->stringID), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if (form.type != FRMLINE) {
	labelWindow[LCWLK] = ffi::txtwin(dT::loadStr(IDS_CWLK), LabelWindowCoords);
	choice             = ((form.extendedAttribute & AT_CWLK) != 0U) ? strOn : strOff;
	valueWindow[LCWLK] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	labelWindow[LWALK] = ffi::txtwin(dT::loadStr(IDS_WALK), LabelWindowCoords);
	choice             = ((form.extendedAttribute & AT_WALK) != 0U) ? strOn : strOff;
	valueWindow[LWALK] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	labelWindow[LUND] = ffi::txtwin(dT::loadStr(IDS_UND), LabelWindowCoords);
	choice            = ((form.extendedAttribute & AT_UND) != 0U) ? strOn : strOff;
	valueWindow[LUND] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((form.extendedAttribute & (AT_WALK | AT_UND | AT_CWLK)) != 0U) {
	  labelWindow[LUNDCOL] = ffi::txtwin(dT::loadStr(IDS_UNDCOL), LabelWindowCoords);
	  // NOLINTNEXTLINE (clang-diagnostic-sign-conversion)
	  valueWindow[LUNDCOL] = ffi::txtrwin(fmt::format(L"{}", (form.underlayColor + 1U)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LULEN] = ffi::txtwin(dT::loadStr(IDS_ULEN), LabelWindowCoords);
	  valueWindow[LULEN] =
	      ffi::txtrwin(fmt::format(L"{:.2f}", (form.underlayStitchLen * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LWLKIND] = ffi::txtwin(dT::loadStr(IDS_UWLKIND), LabelWindowCoords);
	valueWindow[LWLKIND] =
	    ffi::txtrwin(fmt::format(L"{:.2f}", (form.underlayIndent * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((form.extendedAttribute & AT_UND) != 0U) {
	  labelWindow[LUSPAC] = ffi::txtwin(dT::loadStr(IDS_FUSPAC), LabelWindowCoords);
	  valueWindow[LUSPAC] =
	      ffi::txtrwin(fmt::format(L"{:.2f}", (form.underlaySpacing * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LUANG] = ffi::txtwin(dT::loadStr(IDS_FUANG), LabelWindowCoords);
	  valueWindow[LUANG] =
	      ffi::txtrwin(fmt::format(L"{:.2f}", (form.underlayStitchAngle * RADDEGF)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
  }
  labelWindow[LFRMFIL] = ffi::txtwin(dT::loadStr(IDS_TXT2), LabelWindowCoords);
  auto pFill           = wrap::next(FILLLIST.begin(), form.fillType);
  valueWindow[LFRMFIL] = ffi::txtrwin(dT::loadStr(pFill->stringID), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if (form.fillType != 0U) {
	labelWindow[LFRMCOL] = ffi::txtwin(dT::loadStr(IDS_TXT3), LabelWindowCoords);
	valueWindow[LFRMCOL] = ffi::numwin(fmt::format(L"{}", (form.fillColor + 1U)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (form.fillType == FTHF) {
	  labelWindow[LFTHCOL] = ffi::txtwin(dT::loadStr(IDS_FTHCOL), LabelWindowCoords);
	  valueWindow[LFTHCOL] =
	      ffi::numwin(fmt::format(L"{}", (form.fillInfo.feather.color + 1U)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHTYP] = ffi::txtwin(dT::loadStr(IDS_FTHTYP), LabelWindowCoords);
	  auto pFthr           = wrap::next(FTHRLIST.begin(), form.fillInfo.feather.fillType - 1U);
	  valueWindow[LFTHTYP] = ffi::numwin(dT::loadStr(pFthr->stringID), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHBLND] = ffi::txtwin(dT::loadStr(IDS_FTHBLND), LabelWindowCoords);
	  choice                = ((form.extendedAttribute & AT_FTHBLND) != 0U) ? strOn : strOff;
	  valueWindow[LFTHBLND] = ffi::txtrwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  if ((form.extendedAttribute & AT_FTHBLND) == 0U) {
		labelWindow[LFTHBTH] = ffi::txtwin(dT::loadStr(IDS_FTHBOTH), LabelWindowCoords);
		choice               = ((form.extendedAttribute & (AT_FTHBTH)) != 0U) ? strOn : strOff;
		valueWindow[LFTHBTH] = ffi::txtrwin(choice, ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if ((form.extendedAttribute & AT_FTHBTH) == 0U) {
		  labelWindow[LFTHUP] = ffi::txtwin(dT::loadStr(IDS_FTHUP), LabelWindowCoords);
		  choice              = ((form.extendedAttribute & AT_FTHUP) != 0U) ? strOn : strOff;
		  valueWindow[LFTHUP] = ffi::txtrwin(choice, ValueWindowCoords);
		  ffi::nxtlin(formMenuEntryCount);
		}
	  }
	  labelWindow[LFTHUPCNT] = ffi::txtwin(dT::loadStr(IDS_FTHUPCNT), LabelWindowCoords);
	  valueWindow[LFTHUPCNT] =
	      ffi::numwin(fmt::format(L"{}", (form.fillInfo.feather.upCount)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHDWNCNT] = ffi::txtwin(dT::loadStr(IDS_FTHDWNCNT), LabelWindowCoords);
	  valueWindow[LFTHDWNCNT] =
	      ffi::numwin(fmt::format(L"{}", (form.fillInfo.feather.downCount)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHSIZ] = ffi::txtwin(dT::loadStr(IDS_FTHSIZ), LabelWindowCoords);
	  valueWindow[LFTHSIZ] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.fillInfo.feather.ratio)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  if (form.fillInfo.feather.fillType == FTHPSG) {
		labelWindow[LFTHNUM] = ffi::txtwin(dT::loadStr(IDS_FTHNUM), LabelWindowCoords);
		valueWindow[LFTHNUM] = ffi::numwin(fmt::format(L"{}", (form.fillInfo.feather.count)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
	  }
	  labelWindow[LFTHFLR] = ffi::txtwin(dT::loadStr(IDS_FTHFLR), LabelWindowCoords);
	  valueWindow[LFTHFLR] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.fillInfo.feather.minStitchSize * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType != CLPF) {
	  labelWindow[LFRMSPAC] = ffi::txtwin(dT::loadStr(IDS_TXT4), LabelWindowCoords);
	  valueWindow[LFRMSPAC] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.fillSpacing * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (texture::istx(ClosestFormToCursor)) {
	  labelWindow[LTXOF] = ffi::txtwin(dT::loadStr(IDS_TXOF), LabelWindowCoords);
	  valueWindow[LTXOF] = ffi::numwin(fmt::format(L"{:.2f}", (form.txof * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LMAXFIL] = ffi::txtwin(dT::loadStr(IDS_TXT20), LabelWindowCoords);
	valueWindow[LMAXFIL] =
	    ffi::numwin(fmt::format(L"{:.2f}", (form.maxFillStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (!clip::isclp(form) && !texture::istx(ClosestFormToCursor)) {
	  labelWindow[LFRMLEN] = ffi::txtwin(dT::loadStr(IDS_TXT5), LabelWindowCoords);
	  valueWindow[LFRMLEN] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.lengthOrCount.stitchLength * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LMINFIL] = ffi::txtwin(dT::loadStr(IDS_TXT21), LabelWindowCoords);
	valueWindow[LMINFIL] =
	    ffi::numwin(fmt::format(L"{:.2f}", (form.minFillStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (form.fillType == ANGF || form.fillType == TXANGF) {
	  labelWindow[LFRMANG] = ffi::txtwin(dT::loadStr(IDS_TXT6), LabelWindowCoords);
	  valueWindow[LFRMANG] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.angleOrClipData.angle * RADDEGF)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType == ANGCLPF) {
	  labelWindow[LSACANG] = ffi::txtwin(dT::loadStr(IDS_TXT6), LabelWindowCoords);
	  valueWindow[LSACANG] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.satinOrAngle.angle * RADDEGF)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType == VCLPF || form.fillType == HCLPF || form.fillType == ANGCLPF) {
	  labelWindow[LFRMFAZ] = ffi::txtwin(dT::loadStr(IDS_TXT18), LabelWindowCoords);
	  valueWindow[LFRMFAZ] = ffi::numwin(fmt::format(L"{}", (form.wordParam)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType == VRTF || form.fillType == HORF || form.fillType == ANGF ||
	    texture::istx(ClosestFormToCursor)) {
	  labelWindow[LBFILSQR] = ffi::txtwin(dT::loadStr(PREFLIST[PRFFILEND].stringID), LabelWindowCoords);
	  choice = ((form.extendedAttribute & AT_SQR) != 0U) ? dT::loadStr(IDS_SQR) : dT::loadStr(IDS_PNTD);
	  valueWindow[LBFILSQR] = ffi::txtrwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
  }
  labelWindow[LFSTRT] = ffi::txtwin(dT::loadStr(IDS_FSTRT), LabelWindowCoords);
  choice              = ((form.extendedAttribute & AT_STRT) != 0U) ? strOn : strOff;
  valueWindow[LFSTRT] = ffi::txtrwin(choice, ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if ((form.extendedAttribute & AT_STRT) != 0U) {
	labelWindow[LDSTRT] = ffi::txtwin(dT::loadStr(IDS_FSTRT), LabelWindowCoords);
	valueWindow[LDSTRT] = ffi::numwin(fmt::format(L"{}", (form.fillStart)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  labelWindow[LFEND] = ffi::txtwin(dT::loadStr(IDS_FEND), LabelWindowCoords);
  choice             = ((form.extendedAttribute & AT_END) != 0U) ? strOn : strOff;
  valueWindow[LFEND] = ffi::txtrwin(choice, ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if ((form.extendedAttribute & AT_END) != 0U) {
	labelWindow[LDEND] = ffi::txtwin(dT::loadStr(IDS_FEND), LabelWindowCoords);
	valueWindow[LDEND] = ffi::numwin(fmt::format(L"{}", (form.fillEnd)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  labelWindow[LBRD] = ffi::txtwin(dT::loadStr(IDS_TXT7), LabelWindowCoords);
  auto edgeFillType = gsl::narrow_cast<uint8_t>(form.edgeType & NEGUND);
  if (edgeFillType >= EDGETMAX) {
	edgeFillType = EDGETMAX - 1U;
  }
  auto const edgeIdx  = edgeFillType - 1U;
  auto       pEdge  = wrap::next(EDGELIST.begin(), edgeFillType);
  valueWindow[LBRD] = ffi::txtrwin(dT::loadStr(pEdge->stringID), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if (edgeFillType != 0U) {
	labelWindow[LBRDCOL] = ffi::txtwin(dT::loadStr(IDS_TXT8), LabelWindowCoords);
	valueWindow[LBRDCOL] =
	    ffi::numwin(fmt::format(L"{}", ((form.borderColor & COLMSK) + 1U)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((edgeArray[edgeIdx] & BESPAC) != 0) {
	  labelWindow[LBRDSPAC] = ffi::txtwin(dT::loadStr(IDS_TXT9), LabelWindowCoords);
	  choice = (edgeFillType == EDGEPROPSAT || edgeFillType == EDGEOCHAIN || edgeFillType == EDGELCHAIN)
	               ? fmt::format(L"{:.2f}", (form.edgeSpacing * IPFGRAN))
	               : fmt::format(L"{:.2f}", (form.edgeSpacing * IPFGRAN * 2));
	  valueWindow[LBRDSPAC] = ffi::numwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BPICSPAC) != 0) {
	  labelWindow[LBRDPIC] = ffi::txtwin(dT::loadStr(IDS_TXT16), LabelWindowCoords);
	  valueWindow[LBRDPIC] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.edgeSpacing * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BEMAX) != 0) {
	  labelWindow[LMAXBRD] = ffi::txtwin(dT::loadStr(IDS_TXT22), LabelWindowCoords);
	  valueWindow[LMAXBRD] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.maxBorderStitchLen * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BELEN) != 0) {
	  labelWindow[LBRDLEN] = ffi::txtwin(dT::loadStr(IDS_TXT10), LabelWindowCoords);
	  valueWindow[LBRDLEN] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.edgeStitchLen * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BEMIN) != 0) {
	  labelWindow[LMINBRD] = ffi::txtwin(dT::loadStr(IDS_TXT23), LabelWindowCoords);
	  valueWindow[LMINBRD] =
	      ffi::numwin(fmt::format(L"{:.2f}", (form.minBorderStitchLen * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BESIZ) != 0) {
	  labelWindow[LBRDSIZ] = ffi::txtwin(dT::loadStr(IDS_TXT11), LabelWindowCoords);
	  valueWindow[LBRDSIZ] = ffi::numwin(fmt::format(L"{:.2f}", (form.borderSize * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BRDPOS) != 0) {
	  labelWindow[LBRDPOS] = ffi::txtwin(dT::loadStr(IDS_TXT18), LabelWindowCoords);
	  valueWindow[LBRDPOS] = ffi::numwin(fmt::format(L"{:.2f}", (form.edgeStitchLen)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & CHNPOS) != 0) {
	  labelWindow[LBRDPOS] = ffi::txtwin(dT::loadStr(IDS_TXT19), LabelWindowCoords);
	  valueWindow[LBRDPOS] = ffi::numwin(fmt::format(L"{:.2f}", (form.edgeStitchLen)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (edgeFillType == EDGEAPPL) {
	  labelWindow[LAPCOL] = ffi::txtwin(dT::loadStr(IDS_TXT12), LabelWindowCoords);
	  valueWindow[LAPCOL] = ffi::numwin(fmt::format(L"{}", ((form.borderColor >> 4U) + 1U)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (edgeFillType == EDGEANGSAT || edgeFillType == EDGEAPPL || edgeFillType == EDGEPROPSAT) {
	  labelWindow[LBRDUND] = ffi::txtwin(dT::loadStr(IDS_TXT17), LabelWindowCoords);
	  choice               = ((form.edgeType & EGUND) != 0U) ? strOn : strOff;
	  valueWindow[LBRDUND] = ffi::numwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if ((edgeArray[edgeIdx] & BCNRSIZ) != 0) {
	  choice = (form.edgeType == EDGEBHOL) ? fmt::format(L"{:.2f}", (form::getblen() * IPFGRAN))
	                                       : fmt::format(L"{:.2f}", (form::getplen() * IPFGRAN));
	  labelWindow[LBCSIZ] = ffi::txtwin(dT::loadStr(IDS_TXT13), LabelWindowCoords);
	  valueWindow[LBCSIZ] = ffi::numwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.type == FRMLINE && ((edgeArray[edgeIdx] & BRDEND) != 0)) {
	  labelWindow[LBSTRT] = ffi::txtwin(dT::loadStr(IDS_TXT14), LabelWindowCoords);
	  choice = ((form.attribute & SBLNT) != 0U) ? dT::loadStr(IDS_BLUNT) : dT::loadStr(IDS_TAPR);
	  valueWindow[LBSTRT] = ffi::numwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LBFIN] = ffi::txtwin(dT::loadStr(IDS_TXT15), LabelWindowCoords);
	  choice = ((form.attribute & FBLNT) != 0U) ? dT::loadStr(IDS_BLUNT) : dT::loadStr(IDS_TAPR);
	  valueWindow[LBFIN] = ffi::numwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
  }
}

void formForms::refrm() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->testAndReset(StateFlag::PRFACT)) {
	DestroyWindow(PreferencesWindow);
	StateMap->reset(StateFlag::WASRT);
  }
  LabelWindowSize = ValueWindowSize = {};
  StateMap->set(StateFlag::REFCNT); // don't create windows - just size them
  auto formMenuEntryCount = 0U;
  ffi::refrmfn(form, formMenuEntryCount);
  if (FormDataSheet != nullptr) {
	DestroyWindow(FormDataSheet);
  }
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  FormDataSheet = CreateWindow(L"STATIC",
                               nullptr,
                               WS_CHILD | WS_VISIBLE | WS_BORDER,
                               ButtonWidthX3 + 3,
                               3,
                               LabelWindowSize.x + ValueWindowSize.x + 18,
                               LabelWindowSize.y * gsl::narrow<LONG>(formMenuEntryCount) + 12,
                               ThrEdWindow,
                               nullptr,
                               ThrEdInstance,
                               nullptr);
  StateMap->reset(StateFlag::REFCNT); // this time create the windows
  ffi::refrmfn(form, formMenuEntryCount);
}

void formForms::sidwnd(HWND wnd) noexcept {
  auto windowRect          = RECT {0L, 0L, 0L, 0L};
  MsgIndex                 = 0;
  SideWindowEntryBuffer[0] = 0;
  auto const savedChoice   = FormMenuChoice;
  thred::unsid();
  FormMenuChoice = savedChoice;
  GetWindowRect(wnd, &windowRect);
  GetWindowRect(FormDataSheet, &MsgRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  SideMessageWindow = CreateWindow(L"STATIC",
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
  auto windowRect          = RECT {0L, 0L, 0L, 0L};
  MsgIndex                 = 0;
  SideWindowEntryBuffer[0] = 0;
  thred::unsid();
  GetWindowRect(wnd, &windowRect);
  GetClientRect(PreferencesWindow, &MsgRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  SideMessageWindow = CreateWindow(L"STATIC",
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

void formForms::internal::prftwin(std::wstring const& text) noexcept {
  // cppcheck-suppress ignoredReturnValue
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  CreateWindow(L"STATIC",
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

auto formForms::internal::prfnwin(std::wstring const& text) noexcept -> HWND {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  return CreateWindow(L"STATIC",
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

void formForms::internal::prflin(std::wstring const& msg, LSTTYPE const& row) {
  ffi::prftwin(dT::loadStr(row.stringID));
  ValueWindow->operator[](row.value) = ffi::prfnwin(msg);
  ffi::nxtlinprf();
}

void formForms::prfmsg() {
  auto preferenceRect = RECT {0L, 0L, 0L, 0L};
  if (StateMap->testAndReset(StateFlag::INSRT)) {
	StateMap->set(StateFlag::WASRT);
  }
  StateMap->reset(StateFlag::BIGBOX);
  SelectedFormList->clear();
  if (FormDataSheet != nullptr) {
	thred::undat();
	thred::unsid();
	FormMenuChoice = 0U;
  }
  LabelWindowSize.x = LabelWindowSize.y = 0;
  ValueWindowSize.x = ValueWindowSize.y = 0;
  formForms::maxtsiz(dT::loadStr(IDS_PRF4), LabelWindowSize); // PRF4 has the most characters
  formForms::maxtsiz(dT::loadStr(IDS_TAPR), ValueWindowSize);
  LabelWindowSize.x = formForms::internal::maxwid();
  LabelWindowSize.x += 4;
  DestroyWindow(PreferencesWindow);
  auto const windowWidth = LabelWindowSize.x + ValueWindowSize.x + 18;
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  PreferencesWindow = CreateWindow(L"STATIC",
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
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const preferenceDC = GetDC(PreferencesWindow);
  GetClientRect(PreferencesWindow, &preferenceRect);
  FillRect(preferenceDC, &preferenceRect, GetSysColorBrush(COLOR_WINDOW));
  LabelWindowCoords.top = ValueWindowCoords.top = TXTMARG;
  LabelWindowCoords.bottom = ValueWindowCoords.bottom = TXTMARG + LabelWindowSize.y;
  LabelWindowCoords.left                              = TXTMARG;
  LabelWindowCoords.right                             = TXTMARG + LabelWindowSize.x;
  ValueWindowCoords.left                              = TXTMARG2 + LabelWindowSize.x;
  ValueWindowCoords.right = TXTMARG2 + LabelWindowSize.x + ValueWindowSize.x + TXTMARG2;
  auto row                = PREFLIST.begin();
  ffi::prflin(fmt::format(L"{}", (AppliqueColor + 1U)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.AppStitchLen * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (BorderWidth * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (ButtonholeCornerLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.chainSpace * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.chainRatio)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f} mm", (IniFile.clipOffset * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{}", (IniFile.fillPhase)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.eggRatio)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.fillAngle * RADDEGF)), *(row++));
  auto choice = (UserFlagMap->test(UserFlag::SQRFIL)) ? dT::loadStr(IDS_SQR) : dT::loadStr(IDS_PNTD);
  ffi::prflin(choice, *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (LineSpacing * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{}", (thred::duthrsh(ShowStitchThreshold))), *(row++));
  ffi::prflin(fmt::format(L"{:.2f} mm", (IniFile.gridSize * IPFGRAN)), *(row++));
  form::sethup();
  ffi::prflin(fmt::format(L"{}", dT::loadStr(wrap::toUnsigned(IniFile.hoopType) - 1U + IDS_HUP0)), *(row++));
  ffi::prflin(fmt::format(L"{:.0f} mm", (IniFile.hoopSizeY * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.0f} mm", (IniFile.hoopSizeX * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.lensRatio)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.cursorNudgeStep)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (PicotSpacing * IPFGRAN)), *(row++));
  choice = (UserFlagMap->test(UserFlag::BLUNT)) ? dT::loadStr(IDS_BLUNT) : dT::loadStr(IDS_TAPR);
  ffi::prflin(choice, *(row++));
  choice = (UserFlagMap->test(UserFlag::DUND)) ? dT::loadStr(IDS_ON) : dT::loadStr(IDS_OFF);
  ffi::prflin(choice, *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (SmallStitchLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (SnapLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (SpiralWrap)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (StarRatio)), *(row++));
  ffi::prflin(fmt::format(L"{}", (thred::duthrsh(StitchBoxesThreshold))), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (IniFile.maxStitchLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (UserStitchLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(L"{:.2f}", (MinStitchLength * IPFGRAN)), *row);
  StateMap->set(StateFlag::PRFACT);
  ReleaseDC(ThrEdWindow, preferenceDC);
}

void formForms::frmnum() {
  if (!FormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
	dT::showMessage(IDS_FRML, FormList->size());
	StateMap->set(StateFlag::NUMIN);
	StateMap->set(StateFlag::ENTRFNUM);
	dT::numWnd();
  }
  else {
	dT::shoseln(IDS_FRM1MSG, IDS_SETFRM);
  }
}

void formForms::internal::chkdaz() {
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
  UserFlagMap->set(UserFlag::DAZHOL);
  UserFlagMap->set(UserFlag::DAZD);
  IniFile.daisyBorderType = DAZTYP;
}

void formForms::internal::initdaz(HWND hWinDialog) {
  ffi::chkdaz();
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS), fmt::format(L"{}", IniFile.daisyPetalPoints).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT), fmt::format(L"{}", IniFile.daisyHeartCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN), fmt::format(L"{:.2f}", IniFile.daisyDiameter).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ),
                fmt::format(L"{:.2f}", IniFile.daisyHoleDiameter).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS), fmt::format(L"{}", IniFile.daisyInnerCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS), fmt::format(L"{}", IniFile.daisyPetalCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN), fmt::format(L"{:.2f}", IniFile.daisyPetalLen).c_str());
  auto flag = 1U;
  if (!UserFlagMap->test(UserFlag::DAZHOL)) {
	flag = 0U;
  }
  CheckDlgButton(hWinDialog, IDC_HOLE, flag);
  flag = 1U;
  if (!UserFlagMap->test(UserFlag::DAZD)) {
	flag = 0U;
  }
  CheckDlgButton(hWinDialog, IDC_DLIN, flag);
  for (auto const& DaisyTypeString : DaisyTypeStrings) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP),
	            CB_ADDSTRING,
	            0,
	            reinterpret_cast<LPARAM>(dT::loadStr(DaisyTypeString).c_str())); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  }
  SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
}

auto CALLBACK formForms::internal::dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  ffi::initdaz(hwndlg);
	  break;
	}
	case WM_COMMAND: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  auto buffer = std::array<wchar_t, HBUFSIZ> {};
		  GetWindowText(GetDlgItem(hwndlg, IDC_PETLPNTS), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.daisyPetalPoints, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.daisyHeartCount, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buffer.data(), HBUFSIZ);
		  IniFile.daisyDiameter = wrap::wcstof(buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buffer.data(), HBUFSIZ);
		  IniFile.daisyHoleDiameter = wrap::wcstof(buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.daisyInnerCount, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.daisyPetalCount, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buffer.data(), HBUFSIZ);
		  IniFile.daisyPetalLen = wrap::wcstof(buffer.data());
		  if (IsDlgButtonChecked(hwndlg, IDC_HOLE) != 0U) {
			UserFlagMap->set(UserFlag::DAZHOL);
		  }
		  else {
			UserFlagMap->reset(UserFlag::DAZHOL);
		  }
		  if (IsDlgButtonChecked(hwndlg, IDC_DLIN) != 0U) {
			UserFlagMap->set(UserFlag::DAZD);
		  }
		  else {
			UserFlagMap->reset(UserFlag::DAZD);
		  }
		  GetWindowText(GetDlgItem(hwndlg, IDC_DAZTYP), buffer.data(), HBUFSIZ);
		  for (auto iType = uint8_t {}; iType < gsl::narrow_cast<uint8_t>(DaisyTypeStrings.size()); ++iType) {
			auto compareBuffer = dT::loadStr(DaisyTypeStrings[iType]);
			if (wcscmp(buffer.data(), compareBuffer.c_str()) == 0) {
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
		default: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		  outDebugString(L"wparam [{}] not handled in dasyproc\n", LOWORD(wparam));
		  break;
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"umsg [{}] not handled in dasyproc\n", umsg);
	  break;
	}
  }
  return FALSE;
}

void formForms::dasyfrm() {
  constexpr auto DASYSIZE = 6.0F; // ratio of default daisy form to the screen size
  thred::unmsg();
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast)
  if (!DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::dasyproc))) {
	StateMap->reset(StateFlag::FORMIN);
	return;
  }
  auto const referencePoint =
      fPOINT {wrap::midl(ZoomRect.right, ZoomRect.left), wrap::midl(ZoomRect.top, ZoomRect.bottom)};
  FormList->push_back(FRMHED {});
  auto& form              = FormList->back();
  ClosestFormToCursor     = wrap::toUnsigned(FormList->size() - 1U);
  form.vertexIndex        = wrap::toUnsigned(FormVertices->size());
  form.attribute          = gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
  auto       maximumXsize = ZoomRect.right - ZoomRect.left;
  auto const maximumYsize = ZoomRect.top - ZoomRect.bottom;
  if (maximumYsize > maximumXsize) {
	maximumXsize = maximumYsize;
  }
  maximumXsize /= DASYSIZE;
  auto       diameter     = IniFile.daisyDiameter;
  auto       petalLength  = IniFile.daisyPetalLen;
  auto       holeDiameter = IniFile.daisyHoleDiameter;
  auto const ratio        = maximumXsize / (diameter + petalLength);
  diameter *= ratio;
  petalLength *= ratio;
  holeDiameter *= ratio;
  form.type    = FRMFPOLY;
  auto iVertex = 0U;
  auto fref    = 0U;
  if (UserFlagMap->test(UserFlag::DAZHOL)) {
	auto       angle            = PI_F2;
	auto const holeVertexCount  = IniFile.daisyPetalCount * IniFile.daisyInnerCount;
	auto const holeSegmentAngle = PI_F2 / wrap::toFloat(holeVertexCount);
	FormVertices->push_back(
	    fPOINT {referencePoint.x + diameter * cos(angle), referencePoint.y + diameter * sin(angle)});
	++iVertex;
	for (auto iSegment = 0U; iSegment < holeVertexCount + 1U; ++iSegment) {
	  FormVertices->push_back(fPOINT {referencePoint.x + holeDiameter * cos(angle),
	                                  referencePoint.y + holeDiameter * sin(angle)});
	  ++iVertex;
	  angle -= holeSegmentAngle;
	}
	fref = iVertex;
  }
  auto       petalVertexCount = IniFile.daisyPetalCount * IniFile.daisyPetalPoints;
  auto       petalPointCount  = IniFile.daisyPetalPoints;
  auto const borderType       = IniFile.daisyBorderType;
  if (borderType == DHART) {
	petalPointCount  = (IniFile.daisyHeartCount + 1U) * 2U;
	petalVertexCount = IniFile.daisyPetalCount * petalPointCount;
  }
  auto const petalSegmentAngle = PI_F2 / wrap::toFloat(petalVertexCount);
  auto const deltaPetalAngle   = PI_F / wrap::toFloat(IniFile.daisyPetalPoints);
  if (UserFlagMap->test(UserFlag::DAZD)) {
	form.satinGuideCount    = IniFile.daisyPetalCount - 1U;
	form.wordParam          = IniFile.daisyPetalCount * IniFile.daisyInnerCount + 1U;
	form.satinOrAngle.guide = satin::adsatk(IniFile.daisyPetalCount - 1);
  }
  auto const halfPetalPointCount = IniFile.daisyPetalPoints / 2;
  auto       angle               = 0.0F;
  for (auto iMacroPetal = 0U; iMacroPetal < IniFile.daisyPetalCount; ++iMacroPetal) {
	auto petalPointAngle = 0.0F;
	PseudoRandomValue    = SEED;
	for (auto iPoint = 0U; iPoint < petalPointCount; ++iPoint) {
	  auto distanceFromDaisyCenter = 0.0F;
	  switch (borderType) {
		case DSIN: {
		  distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
		  petalPointAngle += deltaPetalAngle;
		  break;
		}
		case DRAMP: {
		  distanceFromDaisyCenter =
		      diameter + (wrap::toFloat(iPoint) / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
		  break;
		}
		case DSAW: {
		  auto const sawPointCount =
		      wrap::toFloat((iPoint > halfPetalPointCount) ? IniFile.daisyPetalPoints - iPoint : iPoint);
		  auto const offset = (sawPointCount / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
		  distanceFromDaisyCenter = diameter + offset;
		  break;
		}
		case DRAG: {
		  distanceFromDaisyCenter = diameter + (wrap::toFloat(form::psg() % IniFile.daisyPetalPoints) /
		                                        wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
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
		  outDebugString(L"Unknown borderType [{}]", borderType);
		  break;
		}
	  }
	  FormVertices->push_back(fPOINT {referencePoint.x + cos(angle) * distanceFromDaisyCenter,
	                                  referencePoint.y + sin(angle) * distanceFromDaisyCenter});
	  ++iVertex;
	  angle += petalSegmentAngle;
	  if (UserFlagMap->test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
		auto const guideIt = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide + iMacroPetal);
		guideIt->start = (IniFile.daisyPetalCount - iMacroPetal - 1) * IniFile.daisyInnerCount + 1U;
		guideIt->finish = iVertex;
	  }
	}
  }
  auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);

  constexpr auto HOLEMARG = 0.01F; // hole margin offset
  if (UserFlagMap->test(UserFlag::DAZHOL)) {
	auto vRef = wrap::next(vertexIt, fref - 1U);
	vRef->y += HOLEMARG;
	++vRef;
	vRef->y += HOLEMARG;
  }
  form.vertexCount = iVertex;
  if (UserFlagMap->test(UserFlag::DAZD)) {
	form.type      = SAT;
	form.attribute = 1;
  }
  StateMap->set(StateFlag::INIT);
  form::frmout(wrap::toUnsigned(FormList->size() - 1U));
  for (auto iMacroPetal = 0U; iMacroPetal < iVertex; ++iMacroPetal) {
	vertexIt->x -= form.rectangle.left;
	vertexIt->y -= form.rectangle.bottom;
	++vertexIt;
  }
  FormMoveDelta      = fPOINT {};
  NewFormVertexCount = iVertex + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  form::mdufrm();
}

void formForms::internal::initTearDlg(HWND hwndlg) {
  SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), fmt::format(L"{:d}", IniFile.formSides).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), fmt::format(L"{:.3f}", IniFile.tearTailLength).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP),
                fmt::format(L"{:.3f}", IniFile.tearTwistStep * IPFGRAN).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), fmt::format(L"{:.3f}", IniFile.tearTwistRatio).c_str());
}

auto CALLBACK formForms::internal::tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
#ifdef TESTCODE
	  auto const uDpi     = GetDpiForWindow(hwndlg);
	  RECT       rcClient = {};
	  GetWindowRect(hwndlg, &rcClient);
	  auto uWidth  = MulDiv((rcClient.right - rcClient.left), uDpi, 96);
	  auto uHeight = MulDiv((rcClient.bottom - rcClient.top), uDpi, 96);
	  SetWindowPos(hwndlg, nullptr, rcClient.left, rcClient.top, uWidth, uHeight, SWP_NOZORDER | SWP_NOACTIVATE);

	  auto hFont = dT::getThrEdFont(400);
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  auto buffer = std::array<wchar_t, HBUFSIZ> {};
		  GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.formSides, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buffer.data(), HBUFSIZ);
		  IniFile.tearTailLength = wrap::wcstof(buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buffer.data(), HBUFSIZ);
		  IniFile.tearTwistStep = wrap::wcstof(buffer.data()) * PFGRAN;
		  GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buffer.data(), HBUFSIZ);
		  IniFile.tearTwistRatio = wrap::wcstof(buffer.data());
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
		default: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		  outDebugString(L"wparam [{}] not handled in tearprc\n", LOWORD(wparam));
		  break;
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"umsg [{}] not handled in dasyproc\n", umsg);
	  break;
	}
  }
  return FALSE;
}

void formForms::setear() {
  thred::unmsg();
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast)
  auto const nResult = DialogBox(
      ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::tearprc));
  if (nResult > 0) {
	thred::savdo();
	constexpr auto TWSTFACT = 4.0F; // teardrop twist factor

	auto twistStep = IniFile.tearTwistStep;
	form::durpoli(IniFile.formSides);
	auto&      form             = FormList->back();
	auto       vBegin           = wrap::next(FormVertices->begin(), form.vertexIndex);
	auto       vNext            = std::next(vBegin);
	auto const count            = wrap::toSize(form.vertexCount) / 4U;
	auto const middle           = wrap::midl(vNext->x, vBegin->x);
	auto       vLast            = wrap::next(vBegin, count + 1U);
	auto       verticalPosition = vLast->y;
	--vLast;
	auto step   = verticalPosition - vLast->y;
	auto vLeft  = wrap::next(vBegin, wrap::toSize(form.vertexCount) - count);
	auto vRight = wrap::next(vBegin, count + 1U);
	for (auto iStep = 0U; iStep < count; ++iStep) {
	  vLeft->y  = verticalPosition;
	  vRight->y = vLeft->y;
	  vRight->x += twistStep;
	  vLeft->x += twistStep;
	  twistStep *= IniFile.tearTwistRatio;
	  verticalPosition -= step;
	  step *= IniFile.tearTailLength;
	  --vRight;
	  ++vLeft;
	}
	vBegin->y = vNext->y = verticalPosition;
	vBegin->x += twistStep;
	vNext->x += twistStep;
	verticalPosition -= step / 2.0F;
	FormVertices->push_back(*vBegin);
	vBegin = wrap::next(FormVertices->begin(), form.vertexIndex); // iterator invalidated by push_back
	vNext  = std::next(vBegin);
	if (twistStep != 0.0F) {
	  vBegin->x = vNext->x + twistStep / TWSTFACT;
	}
	else {
	  vBegin->x = middle;
	}
	vBegin->y = verticalPosition;
	++(form.vertexCount);
	++NewFormVertexCount;
	StateMap->set(StateFlag::FORMSEL);
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	form::flipv();
	StateMap->reset(StateFlag::FORMSEL);
	auto const size =
	    fPOINT {form.rectangle.right - form.rectangle.left, form.rectangle.top - form.rectangle.bottom};
	auto horizontalRatio = wrap::toFloat(UnzoomedRect.x) / TWSTFACT / size.x;
	if (horizontalRatio > 1.0F) {
	  horizontalRatio = 1.0F;
	}
	auto const verticalRatio = wrap::toFloat(UnzoomedRect.y) / TWSTFACT / size.y;
	if (verticalRatio < horizontalRatio) {
	  horizontalRatio = verticalRatio;
	}
	if (horizontalRatio < 1.0F) {
	  auto vScaled = vBegin;
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		vScaled->x = (vScaled->x - vBegin->x) * horizontalRatio + vBegin->x;
		vScaled->y = (vScaled->y - vBegin->y) * horizontalRatio + vBegin->y;
		++vScaled;
	  }
	}
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	auto vShifted = vBegin;
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  vShifted->x -= form.rectangle.left;
	  vShifted->y -= form.rectangle.bottom;
	  ++vShifted;
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
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  ffi::wavinit(hwndlg);
	  break;
	}
	case WM_COMMAND: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  auto buffer = std::array<wchar_t, HBUFSIZ> {};
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.wavePoints, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.waveStart, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.waveEnd, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buffer.data(), HBUFSIZ);
		  wrap::wcstoul(IniFile.waveLobes, buffer.data());

		  constexpr auto WPCLAMP = 100U; // max number of points in a wave form
		  if (IniFile.wavePoints > WPCLAMP) {
			IniFile.wavePoints = WPCLAMP;
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
		default: {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
		  outDebugString(L"wparam [{}] not handled in wavprc\n", LOWORD(wparam));
		  break;
		}
	  }
	  break;
	}
	default: {
	  outDebugString(L"umsg [{}] not handled in wavprc\n", umsg);
	  break;
	}
  }
  return FALSE;
}

void formForms::wavfrm() {
  thred::unmsg();
  constexpr auto WAVSIZE = 4.0F;        // wave size factor
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,cppcoreguidelines-pro-type-cstyle-cast)
  if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::wavprc))) {
	thred::savdo();
	auto points = std::vector<fPOINT> {};
	points.reserve(IniFile.wavePoints);
	// reuse regular polygon code to build the template for points
	form::durpoli(IniFile.wavePoints);
	auto& form = FormList->back();
	form::mdufrm();
	auto iPoint    = 0U;
	auto waveIndex = IniFile.waveStart;
	auto vBegin    = wrap::next(FormVertices->begin(), form.vertexIndex);
	while (waveIndex != IniFile.waveEnd && iPoint < IniFile.wavePoints) {
	  uint16_t const iNextVertex = (waveIndex + 1U) % IniFile.wavePoints;
	  auto const     vNext       = wrap::next(vBegin, iNextVertex);
	  auto const     vWave       = wrap::next(vBegin, waveIndex);
	  points.emplace_back(-vNext->x + vWave->x, -vNext->y + vWave->y);
	  ++iPoint;
	  waveIndex = iNextVertex;
	}
	auto const count            = iPoint;
	auto       currentPosition  = fPOINT {};
	auto const formVerticesSize = (IniFile.waveLobes * count) + 1 -
	                              IniFile.wavePoints; // account for vertices already allocated by durpoli above
	FormVertices->resize(FormVertices->size() + formVerticesSize);
	vBegin = wrap::next(FormVertices->begin(), form.vertexIndex); // resize may invalidate iterator
	auto vertexIt = vBegin;
	for (auto iLobe = 0U; iLobe < IniFile.waveLobes; ++iLobe) {
	  if ((iLobe & 1U) != 0U) {
		for (auto index = 0U; index < count; ++index) {
		  *vertexIt = currentPosition;
		  ++vertexIt;
		  currentPosition.x += points[index].x;
		  currentPosition.y += points[index].y;
		}
	  }
	  else {
		for (auto index = count; index != 0; --index) {
		  *vertexIt = currentPosition;
		  ++vertexIt;
		  currentPosition.x += points[index - 1U].x;
		  currentPosition.y += points[index - 1U].y;
		}
	  }
	}
	*vertexIt = currentPosition;

	auto const vertexCount   = wrap::distance<uint32_t>(vBegin, vertexIt) + 1U;
	auto const rotationAngle = -atan2(vertexIt->y - vBegin->y, vertexIt->x - vBegin->x);

	auto vRotate = vBegin;
	for (auto index = 0U; index < vertexCount; ++index) {
	  thred::rotflt(*vRotate, rotationAngle, {0.0, 0.0});
	  ++vRotate;
	}
	form.type        = FRMLINE;
	form.vertexCount = vertexCount;
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	StateMap->reset(StateFlag::FORMSEL);
	auto const selectedSize =
	    fPOINT {form.rectangle.right - form.rectangle.left, form.rectangle.top - form.rectangle.bottom};
	auto horizontalRatio = wrap::toFloat(UnzoomedRect.x) / WAVSIZE / selectedSize.x;
	if (horizontalRatio > 1) {
	  horizontalRatio = 1.0F;
	}
	auto const verticalRatio = wrap::toFloat(UnzoomedRect.y) / WAVSIZE / selectedSize.y;
	if (verticalRatio < horizontalRatio) {
	  horizontalRatio = verticalRatio;
	}
	if (horizontalRatio < 1.0F) {
	  auto vScaled = vBegin;
	  for (auto index = 0U; index < vertexCount; ++index) {
		vScaled->x = (vScaled->x - vBegin->x) * horizontalRatio + vBegin->x;
		vScaled->y = (vScaled->y - vBegin->y) * horizontalRatio + vBegin->y;
		++vScaled;
	  }
	}
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	auto vShifted = vBegin;
	for (auto index = 0U; index < vertexCount; ++index) {
	  vShifted->x -= form.rectangle.left;
	  vShifted->y -= form.rectangle.bottom;
	  ++vShifted;
	}
	FormMoveDelta      = fPOINT {};
	NewFormVertexCount = vertexCount + 1U;
	form::setmfrm();
	form::mdufrm();
  }
}
