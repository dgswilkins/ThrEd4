// Local Headers
#include "stdafx.h"
#include "displayText.h"
#include "form.h"
#include "formForms.h"
#include "formHeader.h"
#include "globals.h"
#include "satin.h"
#include "thred.h"

// Open Source headers

// Standard Libraries
#include <cstdint>

constexpr auto TXTMARG  = 3L; // text margin in pixels
constexpr auto TXTMARG2 = 6L; // wide text margin in pixels

/*
fill        edgeStitchLen   edgeSpacing borderSize  clipCount   picspac		crnrsiz		brdend

EDGELINE	edgeStitchLen
EDGEBEAN	edgeStitchLen
EDGECLIP											clipCount
EDGEANGSAT	edgeStitchLen	edgeSpacing	borderSize									attribute
EDGEAPPL	edgeStitchLen	edgeSpacing	borderSize									attribute
EDGEPROPSAT	edgeStitchLen	edgeSpacing	borderSize									attribute
EDGEBHOL	edgeStitchLen	edgeSpacing	borderSize	clipCount,res
EDGEPICOT	edgeStitchLen				borderSize	clipCount	edgeSpacing		res
*/

constexpr auto BELEN    = 1U;
constexpr auto BESPAC   = 1U << 1U;
constexpr auto BESIZ    = 1U << 2U;
constexpr auto BNCLP    = 1U << 3U;
constexpr auto BPICSPAC = 1U << 4U;
constexpr auto BCNRSIZ  = 1U << 5U;
constexpr auto BRDEND   = 1U << 6U;
constexpr auto BRDPOS   = 1U << 7U;
constexpr auto BEMAX    = 1U << 8U;
constexpr auto BEMIN    = 1U << 9U;
constexpr auto CHNPOS   = 1U << 10U;

constexpr auto MEGLIN  = (BELEN | BEMAX | BEMIN);
constexpr auto MEGBLD  = (BELEN | BEMAX | BEMIN);
constexpr auto MEGCLP  = (BNCLP | BEMAX | BEMIN);
constexpr auto MEGSAT  = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr auto MEGAP   = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr auto MEGPRP  = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr auto MEGHOL  = (BELEN | BESPAC | BESIZ | BCNRSIZ | BEMAX | BEMIN);
constexpr auto MEGPIC  = (BELEN | BESIZ | BNCLP | BPICSPAC | BCNRSIZ | BEMAX | BEMIN);
constexpr auto MEGDUB  = (BELEN | BEMAX | BEMIN);
constexpr auto MEGCHNL = (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS);
constexpr auto MEGCHNH = (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS);
constexpr auto MEGCLPX = (BNCLP | BEMAX | BEMIN);

enum DaisyStyles { // daisy form types
  DSIN,            // Sine shape
  DRAMP,           // Ramp shape
  DSAW,            // Sawtooth shape
  DRAG,            // Ragged shape
  DCOG,            // Cog shape
  DHART            // Heart shape
};

// formForms Internal namespace
namespace ffi {
void chkdaz();

auto CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

void initdaz(HWND hWinDialog);
void initTearDlg(HWND hwndlg);
auto maxwid() -> SIZE;
auto numwin(std::wstring const& winName, RECT const& location) -> HWND;
void nxtlin(uint32_t& formMenuEntryCount) noexcept;
void nxtlinprf() noexcept;
void prflin(std::wstring const& msg, LIST_TYPE const& row);
auto prfnwin(std::wstring const& text) noexcept -> HWND;
void prftwin(std::wstring const& text) noexcept;
void refrmfn(FRM_HEAD& form, uint32_t& formMenuEntryCount);

auto CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;

auto txtrwin(std::wstring const& winName, RECT const& location) -> HWND;
auto txtwin(std::wstring const& windowName, RECT const& location) -> HWND;
void wavinit(HWND hwndlg);

auto CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL;
} // namespace ffi

namespace {
auto LabelWindowCoords = RECT {}; // location of left windows in the form data sheet
auto LabelWindowSize   = SIZE {}; // size of the left windows in the form data sheet
auto ValueWindowCoords = RECT {}; // location of right windows in the form data sheet
auto ValueWindowSize   = SIZE {}; // size of the right windows in the form data sheet
} // namespace

auto constexpr DAISY_TYPE_STRINGS = std::array<uint16_t, 6> {
    IDS_DAZCRV,
    IDS_DAZSAW,
    IDS_DAZRMP,
    IDS_DAZRAG,
    IDS_DAZCOG,
    IDS_DAZHART,
};

void formForms::maxtsiz(std::wstring const& label, SIZE& textSize) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto labelSize = SIZE {};
  wrap::getTextExtentPoint32(GetDC(ThrEdWindow), label.data(), wrap::toUnsigned(label.size()), &labelSize);
  textSize.cy = labelSize.cy;
  if (labelSize.cx > textSize.cx) {
	textSize.cx = labelSize.cx;
  }
}

auto ffi::maxwid() -> SIZE {
  auto textSize = SIZE {};
  for (auto const& item : PREFLIST) {
	formForms::maxtsiz(displayText::loadStr(item.stringID), textSize);
  }
  return textSize;
}

auto ffi::txtwin(std::wstring const& windowName, RECT const& location) -> HWND {
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

auto ffi::txtrwin(std::wstring const& winName, RECT const& location) -> HWND {
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

auto ffi::numwin(std::wstring const& winName, RECT const& location) -> HWND {
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

void ffi::nxtlin(uint32_t& formMenuEntryCount) noexcept {
  ++formMenuEntryCount;
  ffi::nxtlinprf();
}

void ffi::nxtlinprf() noexcept {
  LabelWindowCoords.top += LabelWindowSize.cy;
  LabelWindowCoords.bottom += LabelWindowSize.cy;
  ValueWindowCoords.top += ValueWindowSize.cy;
  ValueWindowCoords.bottom += ValueWindowSize.cy;
}

void ffi::refrmfn(FRM_HEAD& form, uint32_t& formMenuEntryCount) {
  static constexpr auto EDGE_ARRAY = std::array<uint16_t, 13> {
      MEGLIN, MEGBLD, MEGCLP, MEGSAT, MEGAP, MEGPRP, MEGHOL, MEGPIC, MEGDUB, MEGCHNH, MEGCHNL, MEGCLPX, 0};
  auto const strOn         = displayText::loadStr(IDS_ON);
  auto const strOff        = displayText::loadStr(IDS_OFF);
  auto&      labelWindow   = *LabelWindow;
  auto&      valueWindow   = *ValueWindow;
  ValueWindowCoords.top    = TXTMARG;
  LabelWindowCoords.top    = ValueWindowCoords.top;
  ValueWindowCoords.bottom = TXTMARG + LabelWindowSize.cy;
  LabelWindowCoords.bottom = ValueWindowCoords.bottom;
  LabelWindowCoords.left   = TXTMARG;
  LabelWindowCoords.right  = TXTMARG + LabelWindowSize.cx;
  ValueWindowCoords.left   = TXTMARG2 + LabelWindowSize.cx;
  ValueWindowCoords.right  = TXTMARG2 + LabelWindowSize.cx + ValueWindowSize.cx + TXTMARG2;
  labelWindow[LFRM]        = ffi::txtwin(displayText::loadStr(IDS_TXT0), LabelWindowCoords);
  auto choice = (form.type == FRMLINE) ? displayText::loadStr(IDS_EDG1) : displayText::loadStr(IDS_FREH);
  valueWindow[LFRM] = ffi::txtrwin(choice, ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  labelWindow[LLAYR] = ffi::txtwin(displayText::loadStr(IDS_TXT1), LabelWindowCoords);
  // NOLINTNEXTLINE hicpp-signed-bitwise
  auto itLayer       = wrap::next(LAYRLIST.begin(), (form.attribute & FRMLMSK) >> 1U);
  valueWindow[LLAYR] = ffi::txtrwin(displayText::loadStr(itLayer->stringID), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if (form.type != FRMLINE) {
	labelWindow[LCWLK] = ffi::txtwin(displayText::loadStr(IDS_CWLK), LabelWindowCoords);
	choice             = ((form.extendedAttribute & AT_CWLK) != 0U) ? strOn : strOff;
	valueWindow[LCWLK] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	labelWindow[LWALK] = ffi::txtwin(displayText::loadStr(IDS_WALK), LabelWindowCoords);
	choice             = ((form.extendedAttribute & AT_WALK) != 0U) ? strOn : strOff;
	valueWindow[LWALK] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	labelWindow[LUND] = ffi::txtwin(displayText::loadStr(IDS_UND), LabelWindowCoords);
	choice            = ((form.extendedAttribute & AT_UND) != 0U) ? strOn : strOff;
	valueWindow[LUND] = ffi::txtrwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((form.extendedAttribute & (AT_WALK | AT_UND | AT_CWLK)) != 0U) {
	  labelWindow[LUNDCOL] = ffi::txtwin(displayText::loadStr(IDS_UNDCOL), LabelWindowCoords);
	  valueWindow[LUNDCOL] =
	      ffi::txtrwin(fmt::format(FMT_COMPILE(L"{}"), (form.underlayColor + 1U)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LULEN] = ffi::txtwin(displayText::loadStr(IDS_ULEN), LabelWindowCoords);
	  valueWindow[LULEN] = ffi::txtrwin(
	      fmt::format(FMT_COMPILE(L"{:.2f}"), (form.underlayStitchLen * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LWLKIND] = ffi::txtwin(displayText::loadStr(IDS_UWLKIND), LabelWindowCoords);
	valueWindow[LWLKIND] =
	    ffi::txtrwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.underlayIndent * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if ((form.extendedAttribute & AT_UND) != 0U) {
	  labelWindow[LUSPAC] = ffi::txtwin(displayText::loadStr(IDS_FUSPAC), LabelWindowCoords);
	  valueWindow[LUSPAC] =
	      ffi::txtrwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.underlaySpacing * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LUANG] = ffi::txtwin(displayText::loadStr(IDS_FUANG), LabelWindowCoords);
	  valueWindow[LUANG] = ffi::txtrwin(
	      fmt::format(FMT_COMPILE(L"{:.2f}"), (form.underlayStitchAngle * RADDEGF)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
  }
  labelWindow[LFRMFIL] = ffi::txtwin(displayText::loadStr(IDS_TXT2), LabelWindowCoords);
  auto itFill          = wrap::next(FILLLIST.begin(), form.fillType);
  valueWindow[LFRMFIL] = ffi::txtrwin(displayText::loadStr(itFill->stringID), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if (form.fillType != 0U) {
	labelWindow[LFRMCOL] = ffi::txtwin(displayText::loadStr(IDS_TXT3), LabelWindowCoords);
	valueWindow[LFRMCOL] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillColor + 1U)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (form.fillType == FTHF) {
	  labelWindow[LFTHCOL] = ffi::txtwin(displayText::loadStr(IDS_FTHCOL), LabelWindowCoords);
	  valueWindow[LFTHCOL] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillInfo.feather.color + 1U)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHTYP] = ffi::txtwin(displayText::loadStr(IDS_FTHTYP), LabelWindowCoords);
	  auto itFeather       = wrap::next(FTHRLIST.begin(), form.fillInfo.feather.fillType - 1U);
	  valueWindow[LFTHTYP] = ffi::numwin(displayText::loadStr(itFeather->stringID), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHBLND] = ffi::txtwin(displayText::loadStr(IDS_FTHBLND), LabelWindowCoords);
	  choice                = ((form.extendedAttribute & AT_FTHBLND) != 0U) ? strOn : strOff;
	  valueWindow[LFTHBLND] = ffi::txtrwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  if ((form.extendedAttribute & AT_FTHBLND) == 0U) {
		labelWindow[LFTHBTH] = ffi::txtwin(displayText::loadStr(IDS_FTHBOTH), LabelWindowCoords);
		choice               = ((form.extendedAttribute & (AT_FTHBTH)) != 0U) ? strOn : strOff;
		valueWindow[LFTHBTH] = ffi::txtrwin(choice, ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
		if ((form.extendedAttribute & AT_FTHBTH) == 0U) {
		  labelWindow[LFTHUP] = ffi::txtwin(displayText::loadStr(IDS_FTHUP), LabelWindowCoords);
		  choice              = ((form.extendedAttribute & AT_FTHUP) != 0U) ? strOn : strOff;
		  valueWindow[LFTHUP] = ffi::txtrwin(choice, ValueWindowCoords);
		  ffi::nxtlin(formMenuEntryCount);
		}
	  }
	  labelWindow[LFTHUPCNT] = ffi::txtwin(displayText::loadStr(IDS_FTHUPCNT), LabelWindowCoords);
	  valueWindow[LFTHUPCNT] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillInfo.feather.upCount)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHDWNCNT] = ffi::txtwin(displayText::loadStr(IDS_FTHDWNCNT), LabelWindowCoords);
	  valueWindow[LFTHDWNCNT] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillInfo.feather.downCount)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  labelWindow[LFTHSIZ] = ffi::txtwin(displayText::loadStr(IDS_FTHSIZ), LabelWindowCoords);
	  valueWindow[LFTHSIZ] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.fillInfo.feather.ratio)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	  if (form.fillInfo.feather.fillType == FTHPSG) {
		labelWindow[LFTHNUM] = ffi::txtwin(displayText::loadStr(IDS_FTHNUM), LabelWindowCoords);
		valueWindow[LFTHNUM] =
		    ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillInfo.feather.count)), ValueWindowCoords);
		ffi::nxtlin(formMenuEntryCount);
	  }
	  labelWindow[LFTHFLR] = ffi::txtwin(displayText::loadStr(IDS_FTHFLR), LabelWindowCoords);
	  valueWindow[LFTHFLR] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.fillInfo.feather.minStitchSize * IPFGRAN)),
	                  ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType != CLPF) {
	  labelWindow[LFRMSPAC] = ffi::txtwin(displayText::loadStr(IDS_TXT4), LabelWindowCoords);
	  valueWindow[LFRMSPAC] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.fillSpacing * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.isTexture()) {
	  labelWindow[LTXOF] = ffi::txtwin(displayText::loadStr(IDS_TXOF), LabelWindowCoords);
	  valueWindow[LTXOF] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.txof * IPFGRAN)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LMAXFIL] = ffi::txtwin(displayText::loadStr(IDS_TXT20), LabelWindowCoords);
	valueWindow[LMAXFIL] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.maxFillStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (!form.isClip() && !form.isTexture()) {
	  labelWindow[LFRMLEN] = ffi::txtwin(displayText::loadStr(IDS_TXT5), LabelWindowCoords);
	  valueWindow[LFRMLEN] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.lengthOrCount.stitchLength * IPFGRAN)),
	                  ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	labelWindow[LMINFIL] = ffi::txtwin(displayText::loadStr(IDS_TXT21), LabelWindowCoords);
	valueWindow[LMINFIL] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.minFillStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	if (form.fillType == ANGF || form.fillType == TXANGF) {
	  labelWindow[LFRMANG] = ffi::txtwin(displayText::loadStr(IDS_TXT6), LabelWindowCoords);
	  valueWindow[LFRMANG] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.angleOrClipData.angle * RADDEGF)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType == ANGCLPF) {
	  labelWindow[LSACANG] = ffi::txtwin(displayText::loadStr(IDS_TXT6), LabelWindowCoords);
	  valueWindow[LSACANG] =
	      ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.satinOrAngle.angle * RADDEGF)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType == VCLPF || form.fillType == HCLPF || form.fillType == ANGCLPF) {
	  labelWindow[LFRMFAZ] = ffi::txtwin(displayText::loadStr(IDS_TXT18), LabelWindowCoords);
	  valueWindow[LFRMFAZ] = ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.wordParam)), ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
	if (form.fillType == VRTF || form.fillType == HORF || form.fillType == ANGF || form.isTexture()) {
	  labelWindow[LBFILSQR] = ffi::txtwin(displayText::loadStr(PREFLIST[PRFFILEND].stringID), LabelWindowCoords);
	  choice = ((form.extendedAttribute & AT_SQR) != 0U) ? displayText::loadStr(IDS_SQR)
	                                                     : displayText::loadStr(IDS_PNTD);
	  valueWindow[LBFILSQR] = ffi::txtrwin(choice, ValueWindowCoords);
	  ffi::nxtlin(formMenuEntryCount);
	}
  }
  labelWindow[LFSTRT] = ffi::txtwin(displayText::loadStr(IDS_FSTRT), LabelWindowCoords);
  choice              = ((form.extendedAttribute & AT_STRT) != 0U) ? strOn : strOff;
  valueWindow[LFSTRT] = ffi::txtrwin(choice, ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if ((form.extendedAttribute & AT_STRT) != 0U) {
	labelWindow[LDSTRT] = ffi::txtwin(displayText::loadStr(IDS_FSTRT), LabelWindowCoords);
	valueWindow[LDSTRT] = ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillStart)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  labelWindow[LFEND] = ffi::txtwin(displayText::loadStr(IDS_FEND), LabelWindowCoords);
  choice             = ((form.extendedAttribute & AT_END) != 0U) ? strOn : strOff;
  valueWindow[LFEND] = ffi::txtrwin(choice, ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if ((form.extendedAttribute & AT_END) != 0U) {
	labelWindow[LDEND] = ffi::txtwin(displayText::loadStr(IDS_FEND), LabelWindowCoords);
	valueWindow[LDEND] = ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), (form.fillEnd)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  labelWindow[LBRD] = ffi::txtwin(displayText::loadStr(IDS_TXT7), LabelWindowCoords);
  auto edgeFillType = gsl::narrow_cast<uint8_t>(form.edgeType & NEGUND);
  if (edgeFillType >= EDGETMAX) {
	edgeFillType = EDGETMAX - 1U;
  }
  auto const edgeIdx = edgeFillType - 1U;
  auto       itEdge  = wrap::next(EDGELIST.begin(), edgeFillType);
  valueWindow[LBRD]  = ffi::txtrwin(displayText::loadStr(itEdge->stringID), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if (edgeFillType == 0U) {
	return;
  }
  labelWindow[LBRDCOL] = ffi::txtwin(displayText::loadStr(IDS_TXT8), LabelWindowCoords);
  valueWindow[LBRDCOL] =
      ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), ((form.borderColor & COLMSK) + 1U)), ValueWindowCoords);
  ffi::nxtlin(formMenuEntryCount);
  if ((EDGE_ARRAY[edgeIdx] & BESPAC) != 0) {
	labelWindow[LBRDSPAC] = ffi::txtwin(displayText::loadStr(IDS_TXT9), LabelWindowCoords);
	choice = (edgeFillType == EDGEPROPSAT || edgeFillType == EDGEOCHAIN || edgeFillType == EDGELCHAIN)
	             ? fmt::format(FMT_COMPILE(L"{:.2f}"), (form.edgeSpacing * IPFGRAN))
	             : fmt::format(FMT_COMPILE(L"{:.2f}"), (form.edgeSpacing * IPFGRAN * 2));
	valueWindow[LBRDSPAC] = ffi::numwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BPICSPAC) != 0) {
	labelWindow[LBRDPIC] = ffi::txtwin(displayText::loadStr(IDS_TXT16), LabelWindowCoords);
	valueWindow[LBRDPIC] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.edgeSpacing * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BEMAX) != 0) {
	labelWindow[LMAXBRD] = ffi::txtwin(displayText::loadStr(IDS_TXT22), LabelWindowCoords);
	valueWindow[LMAXBRD] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.maxBorderStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BELEN) != 0) {
	labelWindow[LBRDLEN] = ffi::txtwin(displayText::loadStr(IDS_TXT10), LabelWindowCoords);
	valueWindow[LBRDLEN] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.edgeStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BEMIN) != 0) {
	labelWindow[LMINBRD] = ffi::txtwin(displayText::loadStr(IDS_TXT23), LabelWindowCoords);
	valueWindow[LMINBRD] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.minBorderStitchLen * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BESIZ) != 0) {
	labelWindow[LBRDSIZ] = ffi::txtwin(displayText::loadStr(IDS_TXT11), LabelWindowCoords);
	valueWindow[LBRDSIZ] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.borderSize * IPFGRAN)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BRDPOS) != 0) {
	labelWindow[LBRDPOS] = ffi::txtwin(displayText::loadStr(IDS_TXT18), LabelWindowCoords);
	valueWindow[LBRDPOS] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.edgeStitchLen)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & CHNPOS) != 0) {
	labelWindow[LBRDPOS] = ffi::txtwin(displayText::loadStr(IDS_TXT19), LabelWindowCoords);
	valueWindow[LBRDPOS] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{:.2f}"), (form.edgeStitchLen)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if (edgeFillType == EDGEAPPL) {
	labelWindow[LAPCOL] = ffi::txtwin(displayText::loadStr(IDS_TXT12), LabelWindowCoords);
	valueWindow[LAPCOL] =
	    ffi::numwin(fmt::format(FMT_COMPILE(L"{}"), ((form.borderColor >> 4U) + 1U)), ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if (edgeFillType == EDGEANGSAT || edgeFillType == EDGEAPPL || edgeFillType == EDGEPROPSAT) {
	labelWindow[LBRDUND] = ffi::txtwin(displayText::loadStr(IDS_TXT17), LabelWindowCoords);
	choice               = ((form.edgeType & EGUND) != 0U) ? strOn : strOff;
	valueWindow[LBRDUND] = ffi::numwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY[edgeIdx] & BCNRSIZ) != 0) {
	choice              = (form.edgeType == EDGEBHOL)
	                          ? fmt::format(FMT_COMPILE(L"{:.2f}"), (form::getblen() * IPFGRAN))
	                          : fmt::format(FMT_COMPILE(L"{:.2f}"), (form::getplen() * IPFGRAN));
	labelWindow[LBCSIZ] = ffi::txtwin(displayText::loadStr(IDS_TXT13), LabelWindowCoords);
	valueWindow[LBCSIZ] = ffi::numwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
  if (form.type == FRMLINE && ((EDGE_ARRAY[edgeIdx] & BRDEND) != 0)) {
	labelWindow[LBSTRT] = ffi::txtwin(displayText::loadStr(IDS_TXT14), LabelWindowCoords);
	choice              = ((form.attribute & SBLNT) != 0U) ? displayText::loadStr(IDS_BLUNT)
	                                                       : displayText::loadStr(IDS_TAPR);
	valueWindow[LBSTRT] = ffi::numwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
	labelWindow[LBFIN] = ffi::txtwin(displayText::loadStr(IDS_TXT15), LabelWindowCoords);
	choice             = ((form.attribute & FBLNT) != 0U) ? displayText::loadStr(IDS_BLUNT)
	                                                      : displayText::loadStr(IDS_TAPR);
	valueWindow[LBFIN] = ffi::numwin(choice, ValueWindowCoords);
	ffi::nxtlin(formMenuEntryCount);
  }
}

void formForms::refrm() {
  auto& form = FormList->operator[](ClosestFormToCursor);
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
                               LabelWindowSize.cx + ValueWindowSize.cx + 18,
                               LabelWindowSize.cy * gsl::narrow<LONG>(formMenuEntryCount) + 12,
                               ThrEdWindow,
                               nullptr,
                               ThrEdInstance,
                               nullptr);
  StateMap->reset(StateFlag::REFCNT); // this time create the windows
  ffi::refrmfn(form, formMenuEntryCount);
}

void formForms::sidwnd(HWND wnd) {
  auto windowRect = RECT {};
  thred::resetSideBuffer();
  auto const savedChoice = FormMenuChoice;
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

void formForms::prfsid(HWND wnd) {
  thred::resetSideBuffer();
  thred::unsid();
  auto windowRect = RECT {};
  GetWindowRect(wnd, &windowRect);
  GetClientRect(PreferencesWindow, &MsgRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  SideMessageWindow = CreateWindow(L"STATIC",
                                   nullptr,
                                   WS_BORDER | WS_CHILD | WS_VISIBLE,
                                   windowRect.right - ThredWindowOrigin.x + 6,
                                   windowRect.top - ThredWindowOrigin.y - 3,
                                   ValueWindowSize.cx,
                                   windowRect.bottom - windowRect.top + 3,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
}

void ffi::prftwin(std::wstring const& text) noexcept {
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

auto ffi::prfnwin(std::wstring const& text) noexcept -> HWND {
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

void ffi::prflin(std::wstring const& msg, LIST_TYPE const& row) {
  ffi::prftwin(displayText::loadStr(row.stringID));
  ValueWindow->operator[](row.value) = ffi::prfnwin(msg);
  ffi::nxtlinprf();
}

void formForms::prfmsg() {
  auto preferenceRect = RECT {};
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
  LabelWindowSize.cx = 0;
  LabelWindowSize.cy = 0;
  ValueWindowSize.cx = 0;
  ValueWindowSize.cy = 0;
  LabelWindowSize    = ffi::maxwid();
  LabelWindowSize.cx += TXTMARG2;
  formForms::maxtsiz(displayText::loadStr(IDS_TAPR), ValueWindowSize);
  DestroyWindow(PreferencesWindow);
  auto const windowWidth = LabelWindowSize.cx + ValueWindowSize.cx + 18;
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  PreferencesWindow = CreateWindow(L"STATIC",
                                   nullptr,
                                   WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   ButtonWidthX3 + 3,
                                   3,
                                   windowWidth,
                                   LabelWindowSize.cy * PRFLINS + 12,
                                   ThrEdWindow,
                                   nullptr,
                                   ThrEdInstance,
                                   nullptr);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const preferenceDC = GetDC(PreferencesWindow);
  GetClientRect(PreferencesWindow, &preferenceRect);
  FillRect(preferenceDC, &preferenceRect, GetSysColorBrush(COLOR_WINDOW));
  LabelWindowCoords.top = ValueWindowCoords.top = TXTMARG;
  LabelWindowCoords.bottom = ValueWindowCoords.bottom = TXTMARG + LabelWindowSize.cy;
  LabelWindowCoords.left                              = TXTMARG;
  LabelWindowCoords.right                             = TXTMARG + LabelWindowSize.cx;
  ValueWindowCoords.left                              = TXTMARG2 + LabelWindowSize.cx;
  ValueWindowCoords.right = TXTMARG2 + LabelWindowSize.cx + ValueWindowSize.cx + TXTMARG2;
  auto row                = PREFLIST.begin();
  ffi::prflin(fmt::format(FMT_COMPILE(L"{}"), (AppliqueColor + 1U)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.AppStitchLen * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (BorderWidth * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (ButtonholeCornerLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.chainSpace * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.chainRatio)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f} mm"), (IniFile.clipOffset * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{}"), (IniFile.fillPhase)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.eggRatio)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.fillAngle * RADDEGF)), *(row++));
  auto choice = (UserFlagMap->test(UserFlag::SQRFIL)) ? displayText::loadStr(IDS_SQR)
                                                      : displayText::loadStr(IDS_PNTD);
  ffi::prflin(choice, *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (LineSpacing * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{}"), (thred::duthrsh(ShowStitchThreshold))), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f} mm"), (IniFile.gridSize * IPFGRAN)), *(row++));
  form::sethup();
  ffi::prflin(fmt::format(FMT_COMPILE(L"{}"),
                          displayText::loadStr(wrap::toUnsigned(IniFile.hoopType) - 1U + IDS_HUP0)),
              *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.0f} mm"), (IniFile.hoopSizeY * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.0f} mm"), (IniFile.hoopSizeX * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.lensRatio)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.cursorNudgeStep)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (PicotSpacing * IPFGRAN)), *(row++));
  choice = (UserFlagMap->test(UserFlag::BLUNT)) ? displayText::loadStr(IDS_BLUNT)
                                                : displayText::loadStr(IDS_TAPR);
  ffi::prflin(choice, *(row++));
  choice = (UserFlagMap->test(UserFlag::DUND)) ? displayText::loadStr(IDS_ON) : displayText::loadStr(IDS_OFF);
  ffi::prflin(choice, *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (SmallStitchLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (SnapLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (SpiralWrap)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (StarRatio)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{}"), (thred::duthrsh(StitchBoxesThreshold))), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (IniFile.maxStitchLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (UserStitchLength * IPFGRAN)), *(row++));
  ffi::prflin(fmt::format(FMT_COMPILE(L"{:.2f}"), (MinStitchLength * IPFGRAN)), *row);
  StateMap->set(StateFlag::PRFACT);
  ReleaseDC(ThrEdWindow, preferenceDC);
}

void formForms::frmnum() {
  if (FormList->empty() || !StateMap->test(StateFlag::FORMSEL)) {
	displayText::shoseln(IDS_FRM1MSG, IDS_SETFRM);
	return;
  }
  displayText::showMessage(IDS_FRML, FormList->size());
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRFNUM);
  displayText::numWnd();
}

void ffi::chkdaz() {
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

void ffi::initdaz(HWND hWinDialog) {
  ffi::chkdaz();
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS),
                fmt::format(FMT_COMPILE(L"{}"), IniFile.daisyPetalPoints).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT),
                fmt::format(FMT_COMPILE(L"{}"), IniFile.daisyHeartCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN),
                fmt::format(FMT_COMPILE(L"{:.2f}"), IniFile.daisyDiameter).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ),
                fmt::format(FMT_COMPILE(L"{:.2f}"), IniFile.daisyHoleDiameter).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS),
                fmt::format(FMT_COMPILE(L"{}"), IniFile.daisyInnerCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS),
                fmt::format(FMT_COMPILE(L"{}"), IniFile.daisyPetalCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN),
                fmt::format(FMT_COMPILE(L"{:.2f}"), IniFile.daisyPetalLen).c_str());
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
  for (auto const& daisyTypeString : DAISY_TYPE_STRINGS) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP),
	            CB_ADDSTRING,
	            0,
	            reinterpret_cast<LPARAM>(displayText::loadStr(daisyTypeString).c_str())); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  }
  SendMessage(GetDlgItem(hWinDialog, IDC_DAZTYP), CB_SETCURSEL, IniFile.daisyBorderType, 0);
}

auto CALLBACK ffi::dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
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
		  wrap::wcsToULong(IniFile.daisyPetalPoints, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_DAZPCNT), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.daisyHeartCount, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_CNTLEN), buffer.data(), HBUFSIZ);
		  IniFile.daisyDiameter = wrap::wcsToFloat(buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_HOLSIZ), buffer.data(), HBUFSIZ);
		  IniFile.daisyHoleDiameter = wrap::wcsToFloat(buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_INPNTS), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.daisyInnerCount, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_PETALS), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.daisyPetalCount, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_PETLEN), buffer.data(), HBUFSIZ);
		  IniFile.daisyPetalLen = wrap::wcsToFloat(buffer.data());
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
		  for (auto iType = uint8_t {}; iType < gsl::narrow_cast<uint8_t>(DAISY_TYPE_STRINGS.size()); ++iType) {
			auto compareBuffer = displayText::loadStr(DAISY_TYPE_STRINGS[iType]);
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
		  IniFile.dazdef();
		  UserFlagMap->set(UserFlag::DAZHOL);
		  UserFlagMap->set(UserFlag::DAZD);
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
  // clang-format off
  // ReSharper disable CppClangTidyClangDiagnosticCastFunctionTypeStrict
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts
  auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::dasyproc)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  // ReSharper restore CppClangTidyClangDiagnosticCastFunctionTypeStrict
  // clang-format on
  if (nResult < 1) {
	StateMap->reset(StateFlag::FORMIN);
	return;
  }
  auto const referencePoint =
      F_POINT {wrap::midl(ZoomRect.right, ZoomRect.left), wrap::midl(ZoomRect.top, ZoomRect.bottom)};
  auto form               = FRM_HEAD {};
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
	FormVertices->emplace_back(referencePoint.x + diameter * cos(angle),
	                           referencePoint.y + diameter * sin(angle));
	++iVertex;
	for (auto iSegment = 0U; iSegment < holeVertexCount + 1U; ++iSegment) {
	  FormVertices->emplace_back(referencePoint.x + holeDiameter * cos(angle),
	                             referencePoint.y + holeDiameter * sin(angle));
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
	  FormVertices->emplace_back(referencePoint.x + cos(angle) * distanceFromDaisyCenter,
	                             referencePoint.y + sin(angle) * distanceFromDaisyCenter);
	  ++iVertex;
	  angle += petalSegmentAngle;
	  if (UserFlagMap->test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
		auto const itGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide + iMacroPetal);
		itGuide->start = (IniFile.daisyPetalCount - iMacroPetal - 1) * IniFile.daisyInnerCount + 1U;
		itGuide->finish = iVertex;
	  }
	}
  }
  auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);

  if (UserFlagMap->test(UserFlag::DAZHOL)) {
	auto           referenceVertex = wrap::next(itVertex, fref - 1U);
	constexpr auto HOLEMARG        = 0.01F; // hole margin offset
	referenceVertex->y += HOLEMARG;
	++referenceVertex;
	referenceVertex->y += HOLEMARG;
  }
  form.vertexCount = iVertex;
  if (UserFlagMap->test(UserFlag::DAZD)) {
	form.type      = SAT;
	form.attribute = 1;
  }
  StateMap->set(StateFlag::INIT);
  form.outline();
  FormList->push_back(form);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  for (auto iMacroPetal = 0U; iMacroPetal < iVertex; ++iMacroPetal) {
	itVertex->x -= form.rectangle.left;
	itVertex->y -= form.rectangle.bottom;
	++itVertex;
  }
  FormMoveDelta      = F_POINT {};
  NewFormVertexCount = iVertex + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm(ClosestFormToCursor);
  form::mdufrm();
}

void ffi::initTearDlg(HWND hwndlg) {
  SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS),
                fmt::format(FMT_COMPILE(L"{:d}"), IniFile.formSides).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT),
                fmt::format(FMT_COMPILE(L"{:.3f}"), IniFile.tearTailLength).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP),
                fmt::format(FMT_COMPILE(L"{:.3f}"), IniFile.tearTwistStep * IPFGRAN).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT),
                fmt::format(FMT_COMPILE(L"{:.3f}"), IniFile.tearTwistRatio).c_str());
}

auto CALLBACK ffi::tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast,hicpp-signed-bitwise)
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, 0);
		  return TRUE;
		}
		case IDOK: {
		  auto buffer = std::array<wchar_t, HBUFSIZ> {};
		  GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.formSides, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buffer.data(), HBUFSIZ);
		  IniFile.tearTailLength = wrap::wcsToFloat(buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buffer.data(), HBUFSIZ);
		  IniFile.tearTwistStep = wrap::wcsToFloat(buffer.data()) * PFGRAN;
		  GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buffer.data(), HBUFSIZ);
		  IniFile.tearTwistRatio = wrap::wcsToFloat(buffer.data());
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
	  outDebugString(L"umsg [{}] not handled in tearprc\n", umsg);
	  break;
	}
  }
  return FALSE;
}

void formForms::setear() {
  thred::unmsg();
  // clang-format off
  // resharper disable CppClangTidyClangDiagnosticCastFunctionTypeStrict
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts
  auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::tearprc)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  // resharper restore CppClangTidyClangDiagnosticCastFunctionTypeStrict
  // clang-format on
  if (nResult <= 0) {
	return;
  }
  thred::savdo();
  constexpr auto TWSTFACT = 4.0F; // teardrop twist factor

  auto twistStep = IniFile.tearTwistStep;
  form::durpoli(IniFile.formSides);
  auto&      form             = FormList->back();
  auto const formVertexIndex  = form.vertexIndex;
  auto const formVertexCount  = form.vertexCount;
  auto       firstVertex      = wrap::next(FormVertices->begin(), formVertexIndex);
  auto       nextVertex       = std::next(firstVertex);
  auto const count            = wrap::toPtrdiff(formVertexCount) / 4;
  auto const middle           = wrap::midl(nextVertex->x, firstVertex->x);
  auto       lastVertex       = std::next(firstVertex, count + 1);
  auto       verticalPosition = lastVertex->y;
  --lastVertex;
  auto step        = verticalPosition - lastVertex->y;
  auto leftVertex  = std::next(firstVertex, wrap::toPtrdiff(formVertexCount) - count);
  auto rightVertex = std::next(firstVertex, count + 1);
  for (auto iStep = 0; iStep < count; ++iStep) {
	leftVertex->y  = verticalPosition;
	rightVertex->y = leftVertex->y;
	rightVertex->x += twistStep;
	leftVertex->x += twistStep;
	twistStep *= IniFile.tearTwistRatio;
	verticalPosition -= step;
	step *= IniFile.tearTailLength;
	--rightVertex;
	++leftVertex;
  }
  firstVertex->y = nextVertex->y = verticalPosition;
  firstVertex->x += twistStep;
  nextVertex->x += twistStep;
  verticalPosition -= step / 2.0F;
  FormVertices->push_back(*firstVertex);
  firstVertex = wrap::next(FormVertices->begin(), formVertexIndex); // iterator invalidated by push_back
  nextVertex = std::next(firstVertex);
  if (twistStep != 0.0F) {
	firstVertex->x = nextVertex->x + twistStep / TWSTFACT;
  }
  else {
	firstVertex->x = middle;
  }
  firstVertex->y = verticalPosition;
  ++(form.vertexCount);
  ++NewFormVertexCount;
  StateMap->set(StateFlag::FORMSEL);
  form::flipv();
  StateMap->reset(StateFlag::FORMSEL);
  auto const size =
      F_POINT {form.rectangle.right - form.rectangle.left, form.rectangle.top - form.rectangle.bottom};
  auto horizontalRatio = wrap::toFloat(UnzoomedRect.cx) / TWSTFACT / size.x;
  if (horizontalRatio > 1.0F) {
	horizontalRatio = 1.0F;
  }
  auto const verticalRatio = wrap::toFloat(UnzoomedRect.cy) / TWSTFACT / size.y;
  if (verticalRatio < horizontalRatio) {
	horizontalRatio = verticalRatio;
  }
  if (horizontalRatio < 1.0F) {
	auto       scaledVertex = firstVertex;
	auto const vertexMax    = form.vertexCount;
	for (auto iVertex = 0U; iVertex < vertexMax; ++iVertex) {
	  scaledVertex->x = (scaledVertex->x - firstVertex->x) * horizontalRatio + firstVertex->x;
	  scaledVertex->y = (scaledVertex->y - firstVertex->y) * horizontalRatio + firstVertex->y;
	  ++scaledVertex;
	}
  }
  form.outline();
  // ToDo - is this copy of an iterator required?
  auto       shiftedVertex = firstVertex;
  auto const left          = form.rectangle.left;
  auto const bottom        = form.rectangle.bottom;
  auto const vertexMax     = form.vertexCount;
  for (auto iVertex = 0U; iVertex < vertexMax; ++iVertex) {
	shiftedVertex->x -= left;
	shiftedVertex->y -= bottom;
	++shiftedVertex;
  }
}

void ffi::wavinit(HWND hwndlg) {
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS),
                fmt::format(FMT_COMPILE(L"{:d}"), IniFile.wavePoints).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT),
                fmt::format(FMT_COMPILE(L"{:d}"), IniFile.waveStart).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), fmt::format(FMT_COMPILE(L"{:d}"), IniFile.waveEnd).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), fmt::format(FMT_COMPILE(L"{:d}"), IniFile.waveLobes).c_str());
}

auto CALLBACK ffi::wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
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
		  wrap::wcsToULong(IniFile.wavePoints, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.waveStart, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.waveEnd, buffer.data());
		  GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buffer.data(), HBUFSIZ);
		  wrap::wcsToULong(IniFile.waveLobes, buffer.data());

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
  // clang-format off
  // resharper disable CppClangTidyClangDiagnosticCastFunctionTypeStrict
#pragma warning(suppress : 26490 26493) // type.1 Don't use reinterpret_cast type.4 Don't use C-style casts
  auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, reinterpret_cast<DLGPROC>(ffi::wavprc)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  // resharper restore CppClangTidyClangDiagnosticCastFunctionTypeStrict
  // clang-format on
  if (nResult <= 0) {
	return;
  }
  thred::savdo();
  auto points = std::vector<F_POINT> {};
  points.reserve(IniFile.wavePoints);
  // reuse regular polygon code to build the template for points
  form::durpoli(IniFile.wavePoints);
  auto&      form            = FormList->back();
  auto const formVertexIndex = form.vertexIndex;
  form::mdufrm();
  auto iPoint      = 0U;
  auto waveIndex   = IniFile.waveStart;
  auto firstVertex = wrap::next(FormVertices->begin(), formVertexIndex);
  while (waveIndex != IniFile.waveEnd && iPoint < IniFile.wavePoints) {
	uint16_t const iNextVertex = (waveIndex + 1U) % IniFile.wavePoints;
	auto const     nextVertex  = wrap::next(firstVertex, iNextVertex);
	auto const     waveVertex  = wrap::next(firstVertex, waveIndex);
	points.emplace_back(-nextVertex->x + waveVertex->x, -nextVertex->y + waveVertex->y);
	++iPoint;
	waveIndex = iNextVertex;
  }
  auto const count            = iPoint;
  auto       currentPosition  = F_POINT {};
  auto const formVerticesSize = (IniFile.waveLobes * count) + 1 -
                                IniFile.wavePoints; // account for vertices already allocated by durpoli above
  FormVertices->resize(FormVertices->size() + formVerticesSize);
  firstVertex = wrap::next(FormVertices->begin(), formVertexIndex); // resize may invalidate iterator

  auto itVertex = firstVertex; // copy intended
  for (auto iLobe = 0U; iLobe < IniFile.waveLobes; ++iLobe) {
	if ((iLobe & 1U) != 0U) {
	  for (auto index = 0U; index < count; ++index) {
		*itVertex = currentPosition;
		++itVertex;
		currentPosition.x += points[index].x;
		currentPosition.y += points[index].y;
	  }
	}
	else {
	  for (auto index = count; index != 0; --index) {
		*itVertex = currentPosition;
		++itVertex;
		currentPosition.x += points[index - 1U].x;
		currentPosition.y += points[index - 1U].y;
	  }
	}
  }
  *itVertex = currentPosition;

  auto const vertexCount   = wrap::distance<uint32_t>(firstVertex, itVertex) + 1U;
  auto const rotationAngle = -atan2(itVertex->y - firstVertex->y, itVertex->x - firstVertex->x);

  auto rotatedVertex = firstVertex;
  for (auto index = 0U; index < vertexCount; ++index) {
	thred::rotflt(*rotatedVertex, rotationAngle, F_POINT {});
	++rotatedVertex;
  }
  form.type        = FRMLINE;
  form.vertexCount = vertexCount;
  form.outline();
  StateMap->reset(StateFlag::FORMSEL);
  auto const selectedSize =
      F_POINT {form.rectangle.right - form.rectangle.left, form.rectangle.top - form.rectangle.bottom};
  constexpr auto WAVSIZE         = 4.0F; // wave size factor
  auto           horizontalRatio = wrap::toFloat(UnzoomedRect.cx) / WAVSIZE / selectedSize.x;
  if (horizontalRatio > 1) {
	horizontalRatio = 1.0F;
  }
  auto const verticalRatio = wrap::toFloat(UnzoomedRect.cy) / WAVSIZE / selectedSize.y;
  if (verticalRatio < horizontalRatio) {
	horizontalRatio = verticalRatio;
  }
  if (horizontalRatio < 1.0F) {
	auto vScaled = firstVertex; // copy intended
	for (auto index = 0U; index < vertexCount; ++index) {
	  vScaled->x = (vScaled->x - firstVertex->x) * horizontalRatio + firstVertex->x;
	  vScaled->y = (vScaled->y - firstVertex->y) * horizontalRatio + firstVertex->y;
	  ++vScaled;
	}
  }
  form.outline();
  auto       vShifted = firstVertex; // copy intended
  auto const left     = FormList->back().rectangle.left;
  auto const bottom   = FormList->back().rectangle.bottom;
  for (auto index = 0U; index < vertexCount; ++index) {
	vShifted->x -= left;
	vShifted->y -= bottom;
	++vShifted;
  }
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  form::setmfrm(ClosestFormToCursor);
  form::mdufrm();
}
