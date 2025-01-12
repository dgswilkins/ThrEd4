// Local Headers
#include "stdafx.h"
#include "displayText.h"
#include "form.h"
#include "formForms.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "Resources/resource.h"
#include "satin.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "fmt/compile.h"
#include "gsl/narrow"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <basetsd.h>
#include <minwindef.h>
#include <windef.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <iterator>
#include <string>
// ReSharper disable CppUnusedIncludeDirective
#include <type_traits>
// ReSharper restore CppUnusedIncludeDirective>
#include <vector>

namespace {
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

constexpr auto BELEN    = 1U;        // border edge stitch length
constexpr auto BESPAC   = 1U << 1U;  // border edge spacing
constexpr auto BESIZ    = 1U << 2U;  // border edge width
constexpr auto BNCLP    = 1U << 3U;  // border clip count
constexpr auto BPICSPAC = 1U << 4U;  // border picot spacing
constexpr auto BCNRSIZ  = 1U << 5U;  // buttonhole corner size
constexpr auto BRDEND   = 1U << 6U;  // border end
constexpr auto BRDPOS   = 1U << 7U;  // fill phase
constexpr auto BEMAX    = 1U << 8U;  // maximum border stitch
constexpr auto BEMIN    = 1U << 9U;  // minimum border stitch
constexpr auto CHNPOS   = 1U << 10U; // chain position

constexpr auto MEGLIN  = BELEN | BEMAX | BEMIN;
constexpr auto MEGBLD  = BELEN | BEMAX | BEMIN;
constexpr auto MEGCLP  = BNCLP | BEMAX | BEMIN;
constexpr auto MEGSAT  = BESPAC | BESIZ | BRDEND | BEMAX | BEMIN;
constexpr auto MEGAP   = BESPAC | BESIZ | BRDEND | BEMAX | BEMIN;
constexpr auto MEGPRP  = BESPAC | BESIZ | BRDEND | BEMAX | BEMIN;
constexpr auto MEGHOL  = BELEN | BESPAC | BESIZ | BCNRSIZ | BEMAX | BEMIN;
constexpr auto MEGPIC  = BELEN | BESIZ | BNCLP | BPICSPAC | BCNRSIZ | BEMAX | BEMIN;
constexpr auto MEGDUB  = BELEN | BEMAX | BEMIN;
constexpr auto MEGCHNL = BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS;
constexpr auto MEGCHNH = BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS;
constexpr auto MEGCLPX = BNCLP | BEMAX | BEMIN;

auto constexpr DAISY_TYPE_STRINGS = std::array<uint16_t, 6> {
    IDS_DAZCRV,
    IDS_DAZSAW,
    IDS_DAZRMP,
    IDS_DAZRAG,
    IDS_DAZCOG,
    IDS_DAZHART,
};

enum DaisyStyles : uint8_t { // daisy form types
  DSIN,                      // Sine shape
  DRAMP,                     // Ramp shape
  DSAW,                      // Sawtooth shape
  DRAG,                      // Ragged shape
  DCOG,                      // Cog shape
  DHART                      // Heart shape
};

auto FormDataSheet     = HWND {}; // form data sheet
auto LabelWindowCoords = RECT {}; // location of left windows in the form data sheet
auto LabelWindowSize   = SIZE {}; // size of the left windows in the form data sheet
auto ValueWindowCoords = RECT {}; // location of right windows in the form data sheet
auto ValueWindowSize   = SIZE {}; // size of the right windows in the form data sheet
auto PreferencesWindow = HWND {}; // preferences window

// Definitions
void chkdaz();

auto CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

auto handleDaisyWMCOMMAND(WPARAM const& wparam, HWND hwndlg) -> bool;
void handleDaisyIDOK(HWND hwndlg);
void handlePaisleyDefault(HWND hwndlg);
void handleTearDefault(HWND hwndlg);
void handleTearIDOK(HWND hwndlg);
void handleTearInit(HWND hwndlg);
void handleWaveIDOK(HWND hwndlg);
void initTearDlg(HWND hwndlg);
void initdaz(HWND hWinDialog);
auto maxwid() -> SIZE;
auto numwin(std::wstring const& winName, RECT const& location) -> HWND;
void nxtlin(uint32_t& formMenuEntryCount) noexcept;
void nxtlinprf() noexcept;
void prflin(std::wstring const& msg, LIST_TYPE const& row);
auto prfnwin(std::wstring const& text) noexcept -> HWND;
void prftwin(std::wstring const& text) noexcept;
void refrmfn(FRM_HEAD& form, uint32_t& formMenuEntryCount);

auto CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

auto txtrwin(std::wstring const& winName, RECT const& location) -> HWND;
auto txtwin(std::wstring const& windowName, RECT const& location) -> HWND;
void wavinit(HWND hwndlg);

auto CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR;

// Functions
auto maxwid() -> SIZE {
  auto textSize = SIZE {};
  for (auto const& item : PREFLIST) {
	formForms::maxtsiz(displayText::loadStr(item.stringID), textSize);
  }
  return textSize;
}

auto txtwin(std::wstring const& windowName, RECT const& location) -> HWND {
  if (Instance->stateMap.test(StateFlag::REFCNT)) {
	formForms::maxtsiz(windowName, LabelWindowSize);
	return nullptr;
  }
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

auto txtrwin(std::wstring const& winName, RECT const& location) -> HWND {
  if (Instance->stateMap.test(StateFlag::REFCNT)) {
	formForms::maxtsiz(winName, ValueWindowSize);
	return nullptr;
  }
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

auto numwin(std::wstring const& winName, RECT const& location) -> HWND {
  if (Instance->stateMap.test(StateFlag::REFCNT)) {
	formForms::maxtsiz(winName, ValueWindowSize);
	return nullptr;
  }
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

void nxtlin(uint32_t& formMenuEntryCount) noexcept {
  ++formMenuEntryCount;
  nxtlinprf();
}

void nxtlinprf() noexcept {
  LabelWindowCoords.top += LabelWindowSize.cy;
  LabelWindowCoords.bottom += LabelWindowSize.cy;
  ValueWindowCoords.top += ValueWindowSize.cy;
  ValueWindowCoords.bottom += ValueWindowSize.cy;
}

void refrmfn(FRM_HEAD& form, uint32_t& formMenuEntryCount) {
  static constexpr auto EDGE_ARRAY = std::array<uint16_t, 13> {
      MEGLIN, MEGBLD, MEGCLP, MEGSAT, MEGAP, MEGPRP, MEGHOL, MEGPIC, MEGDUB, MEGCHNH, MEGCHNL, MEGCLPX, 0};
  auto const strOn       = displayText::loadStr(IDS_ON);
  auto const strOff      = displayText::loadStr(IDS_OFF);
  auto&      labelWindow = thred::getLabelWindow();
  auto&      valueWindow = Instance->valueWindow;
  // set the window coordinates
  ValueWindowCoords.top    = TXTMARG;
  LabelWindowCoords.top    = ValueWindowCoords.top;
  ValueWindowCoords.bottom = TXTMARG + LabelWindowSize.cy;
  LabelWindowCoords.bottom = ValueWindowCoords.bottom;
  LabelWindowCoords.left   = TXTMARG;
  LabelWindowCoords.right  = TXTMARG + LabelWindowSize.cx;
  ValueWindowCoords.left   = TXTMARG2 + LabelWindowSize.cx;
  ValueWindowCoords.right  = TXTMARG2 + LabelWindowSize.cx + ValueWindowSize.cx + TXTMARG2;
  // label and fill the form field
  labelWindow[LFRM] = txtwin(displayText::loadStr(IDS_TXT0), LabelWindowCoords);
  auto choice = form.type == FRMLINE ? displayText::loadStr(IDS_EDG1) : displayText::loadStr(IDS_FREH);
  valueWindow[LFRM] = txtrwin(choice, ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  // label and fill the layer field
  labelWindow[LLAYR] = txtwin(displayText::loadStr(IDS_TXT1), LabelWindowCoords);
  // NOLINTNEXTLINE hicpp-signed-bitwise
  auto itLayer       = wrap::next(LAYRLIST.begin(), (form.attribute & FRMLMSK) >> 1U);
  valueWindow[LLAYR] = txtrwin(displayText::loadStr(itLayer->stringID), ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  if (form.type != FRMLINE) { // not a line form
	// label and fill the center walk, walk, and underlay fields with 'on' or 'off'
	labelWindow[LCWLK] = txtwin(displayText::loadStr(IDS_CWLK), LabelWindowCoords);
	choice             = (form.extendedAttribute & AT_CWLK) != 0U ? strOn : strOff;
	valueWindow[LCWLK] = txtrwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	labelWindow[LWALK] = txtwin(displayText::loadStr(IDS_WALK), LabelWindowCoords);
	choice             = (form.extendedAttribute & AT_WALK) != 0U ? strOn : strOff;
	valueWindow[LWALK] = txtrwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	labelWindow[LUND] = txtwin(displayText::loadStr(IDS_UND), LabelWindowCoords);
	choice            = (form.extendedAttribute & AT_UND) != 0U ? strOn : strOff;
	valueWindow[LUND] = txtrwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	if ((form.extendedAttribute & (AT_WALK | AT_UND | AT_CWLK)) != 0U) {
	  // label and fill the underlay color and stitch length fields
	  labelWindow[LUNDCOL] = txtwin(displayText::loadStr(IDS_UNDCOL), LabelWindowCoords);
	  valueWindow[LUNDCOL] = txtrwin(format(FMT_COMPILE(L"{}"), form.underlayColor + 1U), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  labelWindow[LULEN] = txtwin(displayText::loadStr(IDS_ULEN), LabelWindowCoords);
	  valueWindow[LULEN] =
	      txtrwin(format(FMT_COMPILE(L"{:.2f}"), form.underlayStitchLen * IPFGRAN), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	// label and fill the underlay indent field
	labelWindow[LWLKIND] = txtwin(displayText::loadStr(IDS_UWLKIND), LabelWindowCoords);
	valueWindow[LWLKIND] =
	    txtrwin(format(FMT_COMPILE(L"{:.2f}"), form.underlayIndent * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	if ((form.extendedAttribute & AT_UND) != 0U) {
	  // label and fill the underlay spacing and angle fields
	  labelWindow[LUSPAC] = txtwin(displayText::loadStr(IDS_FUSPAC), LabelWindowCoords);
	  valueWindow[LUSPAC] =
	      txtrwin(format(FMT_COMPILE(L"{:.2f}"), form.underlaySpacing * IPFGRAN), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  labelWindow[LUANG] = txtwin(displayText::loadStr(IDS_FUANG), LabelWindowCoords);
	  valueWindow[LUANG] =
	      txtrwin(format(FMT_COMPILE(L"{:.2f}"), form.underlayStitchAngle * RADDEGF), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
  }
  // label and fill the fill type field
  labelWindow[LFRMFIL] = txtwin(displayText::loadStr(IDS_TXT2), LabelWindowCoords);
  auto itFill          = wrap::next(FILLLIST.begin(), form.fillType);
  valueWindow[LFRMFIL] = txtrwin(displayText::loadStr(itFill->stringID), ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  if (form.fillType != 0U) { // if the form has a fill
	// label and fill the fill color field
	labelWindow[LFRMCOL] = txtwin(displayText::loadStr(IDS_TXT3), LabelWindowCoords);
	valueWindow[LFRMCOL] = numwin(format(FMT_COMPILE(L"{}"), form.fillColor + 1U), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	if (form.fillType == FTHF) {
	  // label and fill the feather color, feather type & blend fields
	  labelWindow[LFTHCOL] = txtwin(displayText::loadStr(IDS_FTHCOL), LabelWindowCoords);
	  valueWindow[LFTHCOL] = numwin(format(FMT_COMPILE(L"{}"), form.feather.color + 1U), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  labelWindow[LFTHTYP] = txtwin(displayText::loadStr(IDS_FTHTYP), LabelWindowCoords);
	  auto itFeather       = wrap::next(FTHRLIST.begin(), form.feather.fillType - 1U);
	  valueWindow[LFTHTYP] = numwin(displayText::loadStr(itFeather->stringID), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  labelWindow[LFTHBLND] = txtwin(displayText::loadStr(IDS_FTHBLND), LabelWindowCoords);
	  choice                = (form.extendedAttribute & AT_FTHBLND) != 0U ? strOn : strOff;
	  valueWindow[LFTHBLND] = txtrwin(choice, ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  if ((form.extendedAttribute & AT_FTHBLND) == 0U) {
		// label and fill the feather both field
		labelWindow[LFTHBTH] = txtwin(displayText::loadStr(IDS_FTHBOTH), LabelWindowCoords);
		choice               = (form.extendedAttribute & AT_FTHBTH) != 0U ? strOn : strOff;
		valueWindow[LFTHBTH] = txtrwin(choice, ValueWindowCoords);
		nxtlin(formMenuEntryCount);
		if ((form.extendedAttribute & AT_FTHBTH) == 0U) {
		  // label and fill the feather up field
		  labelWindow[LFTHUP] = txtwin(displayText::loadStr(IDS_FTHUP), LabelWindowCoords);
		  choice              = (form.extendedAttribute & AT_FTHUP) != 0U ? strOn : strOff;
		  valueWindow[LFTHUP] = txtrwin(choice, ValueWindowCoords);
		  nxtlin(formMenuEntryCount);
		}
	  }
	  // label and fill the feather up and down count fields
	  labelWindow[LFTHUPCNT] = txtwin(displayText::loadStr(IDS_FTHUPCNT), LabelWindowCoords);
	  valueWindow[LFTHUPCNT] = numwin(format(FMT_COMPILE(L"{}"), form.feather.upCount), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  labelWindow[LFTHDWNCNT] = txtwin(displayText::loadStr(IDS_FTHDWNCNT), LabelWindowCoords);
	  valueWindow[LFTHDWNCNT] = numwin(format(FMT_COMPILE(L"{}"), form.feather.downCount), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  // label and fill the feather ratio, number and floor fields
	  labelWindow[LFTHSIZ] = txtwin(displayText::loadStr(IDS_FTHSIZ), LabelWindowCoords);
	  valueWindow[LFTHSIZ] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.feather.ratio), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	  if (form.feather.fillType == FTHPSG) { // if the feather is ragged
		// label and fill the feather steps field
		labelWindow[LFTHNUM] = txtwin(displayText::loadStr(IDS_FTHNUM), LabelWindowCoords);
		valueWindow[LFTHNUM] = numwin(format(FMT_COMPILE(L"{}"), form.feather.count), ValueWindowCoords);
		nxtlin(formMenuEntryCount);
	  }
	  // label and fill the feather floor field
	  labelWindow[LFTHFLR] = txtwin(displayText::loadStr(IDS_FTHFLR), LabelWindowCoords);
	  valueWindow[LFTHFLR] =
	      numwin(format(FMT_COMPILE(L"{:.2f}"), form.feather.minStitchSize * IPFGRAN), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	if (form.fillType != CLPF) {
	  // label and fill the fill spacing field
	  labelWindow[LFRMSPAC] = txtwin(displayText::loadStr(IDS_TXT4), LabelWindowCoords);
	  valueWindow[LFRMSPAC] =
	      numwin(format(FMT_COMPILE(L"{:.2f}"), form.fillSpacing * IPFGRAN), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	if (form.isTexture()) {
	  // label and fill the texture offset field
	  labelWindow[LTXOF] = txtwin(displayText::loadStr(IDS_TXOF), LabelWindowCoords);
	  valueWindow[LTXOF] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.txof * IPFGRAN), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	// label and fill the fill stitch maximum length field
	labelWindow[LMAXFIL] = txtwin(displayText::loadStr(IDS_TXT20), LabelWindowCoords);
	valueWindow[LMAXFIL] =
	    numwin(format(FMT_COMPILE(L"{:.2f}"), form.maxFillStitchLen * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	if (!form.isClip() && !form.isTexture()) { // if the form is not a clip or texture
	  // label and fill the fill stitch length field
	  labelWindow[LFRMLEN] = txtwin(displayText::loadStr(IDS_TXT5), LabelWindowCoords);
	  valueWindow[LFRMLEN] =
	      numwin(format(FMT_COMPILE(L"{:.2f}"), form.stitchLength * IPFGRAN), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	// label and fill the fill stitch minimum length field
	labelWindow[LMINFIL] = txtwin(displayText::loadStr(IDS_TXT21), LabelWindowCoords);
	valueWindow[LMINFIL] =
	    numwin(format(FMT_COMPILE(L"{:.2f}"), form.minFillStitchLen * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	if (form.fillType == ANGF || form.fillType == TXANGF) {
	  // label and fill the fill angle field
	  labelWindow[LFRMANG] = txtwin(displayText::loadStr(IDS_TXT6), LabelWindowCoords);
	  valueWindow[LFRMANG] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.fillAngle * RADDEGF), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	if (form.fillType == ANGCLPF) {
	  // label and fill the clip fill angle field
	  labelWindow[LSACANG] = txtwin(displayText::loadStr(IDS_TXT6), LabelWindowCoords);
	  valueWindow[LSACANG] =
	      numwin(format(FMT_COMPILE(L"{:.2f}"), form.clipFillAngle * RADDEGF), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	if (form.fillType == VCLPF || form.fillType == HCLPF || form.fillType == ANGCLPF) {
	  // label and fill the clip fill spacing field
	  labelWindow[LFRMFAZ] = txtwin(displayText::loadStr(IDS_TXT18), LabelWindowCoords);
	  valueWindow[LFRMFAZ] = numwin(format(FMT_COMPILE(L"{}"), form.wordParam), ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
	if (form.fillType == VRTF || form.fillType == HORF || form.fillType == ANGF || form.isTexture()) {
	  // label and fill the fill ends field
	  labelWindow[LBFILSQR] = txtwin(displayText::loadStr(PREFLIST[PRFFILEND].stringID), LabelWindowCoords);
	  choice = (form.extendedAttribute & AT_SQR) != 0U ? displayText::loadStr(IDS_SQR)
	                                                   : displayText::loadStr(IDS_PNTD);
	  valueWindow[LBFILSQR] = txtrwin(choice, ValueWindowCoords);
	  nxtlin(formMenuEntryCount);
	}
  }
  // label and fill the fill start and end fields
  labelWindow[LFSTRT] = txtwin(displayText::loadStr(IDS_FSTRT), LabelWindowCoords);
  choice              = (form.extendedAttribute & AT_STRT) != 0U ? strOn : strOff;
  valueWindow[LFSTRT] = txtrwin(choice, ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  if ((form.extendedAttribute & AT_STRT) != 0U) {
	labelWindow[LDSTRT] = txtwin(displayText::loadStr(IDS_FSTRT), LabelWindowCoords);
	valueWindow[LDSTRT] = numwin(format(FMT_COMPILE(L"{}"), form.fillStart), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  labelWindow[LFEND] = txtwin(displayText::loadStr(IDS_FEND), LabelWindowCoords);
  choice             = (form.extendedAttribute & AT_END) != 0U ? strOn : strOff;
  valueWindow[LFEND] = txtrwin(choice, ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  if ((form.extendedAttribute & AT_END) != 0U) {
	labelWindow[LDEND] = txtwin(displayText::loadStr(IDS_FEND), LabelWindowCoords);
	valueWindow[LDEND] = numwin(format(FMT_COMPILE(L"{}"), form.fillEnd), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  // label and fill border type field
  labelWindow[LBRD] = txtwin(displayText::loadStr(IDS_TXT7), LabelWindowCoords);
  auto edgeFillType = gsl::narrow_cast<uint8_t>(form.edgeType & NEGUND);
  if (edgeFillType >= EDGETMAX) {
	edgeFillType = EDGETMAX - 1U;
  }
  auto const edgeIdx = edgeFillType - 1U;
  auto       itEdge  = wrap::next(EDGELIST.begin(), edgeFillType);
  valueWindow[LBRD]  = txtrwin(displayText::loadStr(itEdge->stringID), ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  if (edgeFillType == 0U) {
	return;
  }
  // label and fill the border color field
  labelWindow[LBRDCOL] = txtwin(displayText::loadStr(IDS_TXT8), LabelWindowCoords);
  valueWindow[LBRDCOL] = numwin(format(FMT_COMPILE(L"{}"), form.borderColor & (COLMSK + 1U)), ValueWindowCoords);
  nxtlin(formMenuEntryCount);
  if ((EDGE_ARRAY.at(edgeIdx) & BESPAC) != 0) {
	// label and fill the border spacing field
	labelWindow[LBRDSPAC] = txtwin(displayText::loadStr(IDS_TXT9), LabelWindowCoords);
	choice = edgeFillType == EDGEPROPSAT || edgeFillType == EDGEOCHAIN || edgeFillType == EDGELCHAIN
	             ? format(FMT_COMPILE(L"{:.2f}"), form.edgeSpacing * IPFGRAN)
	             : format(FMT_COMPILE(L"{:.2f}"), form.edgeSpacing * IPFGRAN * 2);
	valueWindow[LBRDSPAC] = numwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BPICSPAC) != 0) {
	// label and fill the border picot spacing field
	labelWindow[LBRDPIC] = txtwin(displayText::loadStr(IDS_TXT16), LabelWindowCoords);
	valueWindow[LBRDPIC] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.edgeSpacing * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BEMAX) != 0) {
	// label and fill the maximum border stitch field
	labelWindow[LMAXBRD] = txtwin(displayText::loadStr(IDS_TXT22), LabelWindowCoords);
	valueWindow[LMAXBRD] =
	    numwin(format(FMT_COMPILE(L"{:.2f}"), form.maxBorderStitchLen * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BELEN) != 0) {
	// label and fill the border stitch length field
	labelWindow[LBRDLEN] = txtwin(displayText::loadStr(IDS_TXT10), LabelWindowCoords);
	valueWindow[LBRDLEN] =
	    numwin(format(FMT_COMPILE(L"{:.2f}"), form.edgeStitchLen * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BEMIN) != 0) {
	// label and fill the minimum border stitch length field
	labelWindow[LMINBRD] = txtwin(displayText::loadStr(IDS_TXT23), LabelWindowCoords);
	valueWindow[LMINBRD] =
	    numwin(format(FMT_COMPILE(L"{:.2f}"), form.minBorderStitchLen * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BESIZ) != 0) {
	// label and fill the border width field
	labelWindow[LBRDSIZ] = txtwin(displayText::loadStr(IDS_TXT11), LabelWindowCoords);
	valueWindow[LBRDSIZ] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.borderSize * IPFGRAN), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BRDPOS) != 0) {
	// label and fill the fill phase field
	labelWindow[LBRDPOS] = txtwin(displayText::loadStr(IDS_TXT18), LabelWindowCoords);
	valueWindow[LBRDPOS] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.edgeStitchLen), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & CHNPOS) != 0) {
	// label and fill the chain position field
	labelWindow[LBRDPOS] = txtwin(displayText::loadStr(IDS_TXT19), LabelWindowCoords);
	valueWindow[LBRDPOS] = numwin(format(FMT_COMPILE(L"{:.2f}"), form.edgeStitchLen), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if (edgeFillType == EDGEAPPL) {
	// label and fill the applique color field
	labelWindow[LAPCOL] = txtwin(displayText::loadStr(IDS_TXT12), LabelWindowCoords);
	valueWindow[LAPCOL] = numwin(format(FMT_COMPILE(L"{}"), (form.borderColor >> 4U) + 1U), ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if (edgeFillType == EDGEANGSAT || edgeFillType == EDGEAPPL || edgeFillType == EDGEPROPSAT) {
	// label and fill the border underlay field
	labelWindow[LBRDUND] = txtwin(displayText::loadStr(IDS_TXT17), LabelWindowCoords);
	choice               = (form.edgeType & EGUND) != 0U ? strOn : strOff;
	valueWindow[LBRDUND] = numwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if ((EDGE_ARRAY.at(edgeIdx) & BCNRSIZ) != 0) {
	// label and fill the buttonhole corner size field
	choice = form.edgeType == EDGEBHOL ? format(FMT_COMPILE(L"{:.2f}"), form::getblen() * IPFGRAN)
	                                   : format(FMT_COMPILE(L"{:.2f}"), form::getplen() * IPFGRAN);
	labelWindow[LBCSIZ] = txtwin(displayText::loadStr(IDS_TXT13), LabelWindowCoords);
	valueWindow[LBCSIZ] = numwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
  if (form.type == FRMLINE && (EDGE_ARRAY.at(edgeIdx) & BRDEND) != 0) {
	// label and fill the border start & end fields
	labelWindow[LBSTRT] = txtwin(displayText::loadStr(IDS_TXT14), LabelWindowCoords);
	choice = (form.attribute & SBLNT) != 0U ? displayText::loadStr(IDS_BLUNT) : displayText::loadStr(IDS_TAPR);
	valueWindow[LBSTRT] = numwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
	labelWindow[LBFIN] = txtwin(displayText::loadStr(IDS_TXT15), LabelWindowCoords);
	choice = (form.attribute & FBLNT) != 0U ? displayText::loadStr(IDS_BLUNT) : displayText::loadStr(IDS_TAPR);
	valueWindow[LBFIN] = numwin(choice, ValueWindowCoords);
	nxtlin(formMenuEntryCount);
  }
}

void prftwin(std::wstring const& text) noexcept {
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

auto prfnwin(std::wstring const& text) noexcept -> HWND {
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

void prflin(std::wstring const& msg, LIST_TYPE const& row) {
  prftwin(displayText::loadStr(row.stringID));
  Instance->valueWindow.operator[](row.value) = prfnwin(msg);
  nxtlinprf();
}

void chkdaz() {
  IniFile.daisyPetalPoints = std::max(IniFile.daisyPetalPoints, 1U);
  IniFile.daisyInnerCount  = std::max(IniFile.daisyInnerCount, 1U);
  IniFile.daisyPetalCount  = std::max(IniFile.daisyPetalCount, 1U);
  IniFile.daisyHeartCount =
      std::min(IniFile.daisyHeartCount, gsl::narrow<decltype(IniFile.daisyHeartCount)>(IniFile.daisyPetalPoints));
}

// ReSharper disable CppParameterMayBeConst
void initdaz(HWND hWinDialog) {
  chkdaz();
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETLPNTS),
                format(FMT_COMPILE(L"{}"), IniFile.daisyPetalPoints).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_DAZPCNT),
                format(FMT_COMPILE(L"{}"), IniFile.daisyHeartCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_CNTLEN),
                format(FMT_COMPILE(L"{:.2f}"), IniFile.daisyDiameter).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_HOLSIZ),
                format(FMT_COMPILE(L"{:.2f}"), IniFile.daisyHoleDiameter).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_INPNTS),
                format(FMT_COMPILE(L"{}"), IniFile.daisyInnerCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETALS),
                format(FMT_COMPILE(L"{}"), IniFile.daisyPetalCount).c_str());
  SetWindowText(GetDlgItem(hWinDialog, IDC_PETLEN),
                format(FMT_COMPILE(L"{:.2f}"), IniFile.daisyPetalLen).c_str());
  auto flag = 1U;
  if (!Instance->userFlagMap.test(UserFlag::DAZHOL)) {
	flag = 0U;
  }
  CheckDlgButton(hWinDialog, IDC_HOLE, flag);
  flag = 1U;
  if (!Instance->userFlagMap.test(UserFlag::DAZD)) {
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
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleDaisyIDOK(HWND hwndlg) {
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
	Instance->userFlagMap.set(UserFlag::DAZHOL);
  }
  else {
	Instance->userFlagMap.reset(UserFlag::DAZHOL);
  }
  if (IsDlgButtonChecked(hwndlg, IDC_DLIN) != 0U) {
	Instance->userFlagMap.set(UserFlag::DAZD);
  }
  else {
	Instance->userFlagMap.reset(UserFlag::DAZD);
  }
  GetWindowText(GetDlgItem(hwndlg, IDC_DAZTYP), buffer.data(), HBUFSIZ);
  for (auto iType = uint8_t {}; iType < gsl::narrow_cast<uint8_t>(DAISY_TYPE_STRINGS.size()); ++iType) {
	auto compareBuffer = displayText::loadStr(DAISY_TYPE_STRINGS.at(iType));
	if (wcscmp(buffer.data(), compareBuffer.c_str()) == 0) {
	  IniFile.daisyBorderType = iType;
	  break;
	}
  }
  chkdaz();
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto handleDaisyWMCOMMAND(WPARAM const& wparam, HWND hwndlg) -> bool {
  switch (LOWORD(wparam)) {
	case IDCANCEL: {
	  EndDialog(hwndlg, FALSE);
	  return true;
	}
	case IDOK:
	  handleDaisyIDOK(hwndlg);
	  EndDialog(hwndlg, TRUE);
	  break;
	case IDC_DAZRST: {
	  IniFile.dazdef();
	  Instance->userFlagMap.set(UserFlag::DAZHOL);
	  Instance->userFlagMap.set(UserFlag::DAZD);
	  initdaz(hwndlg);
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
	  outDebugString(L"wparam [{}] not handled in dasyproc\n", LOWORD(wparam));
	  break;
	}
  }
  return false;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto CALLBACK dasyproc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  initdaz(hwndlg);
	  break;
	}
	case WM_COMMAND: {
	  if (handleDaisyWMCOMMAND(wparam, hwndlg)) {
		return TRUE;
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
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void initTearDlg(HWND hwndlg) {
  SetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), format(FMT_COMPILE(L"{:d}"), IniFile.formSides).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TEARAT),
                format(FMT_COMPILE(L"{:.3f}"), IniFile.tearTailLength).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP),
                format(FMT_COMPILE(L"{:.3f}"), IniFile.tearTwistStep * IPFGRAN).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT),
                format(FMT_COMPILE(L"{:.3f}"), IniFile.tearTwistRatio).c_str());
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleTearIDOK(HWND hwndlg) {
  auto buffer = std::array<wchar_t, HBUFSIZ> {};
  GetWindowText(GetDlgItem(hwndlg, IDC_TEARSIDS), buffer.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.formSides, buffer.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_TEARAT), buffer.data(), HBUFSIZ);
  IniFile.tearTailLength = wrap::wcsToFloat(buffer.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_TWSTSTP), buffer.data(), HBUFSIZ);
  IniFile.tearTwistStep = wrap::wcsToFloat(buffer.data()) * PFGRAN;
  GetWindowText(GetDlgItem(hwndlg, IDC_TWSTRAT), buffer.data(), HBUFSIZ);
  IniFile.tearTwistRatio = wrap::wcsToFloat(buffer.data());
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleTearDefault(HWND hwndlg) {
  constexpr auto TEARSIDES    = uint16_t {20U};
  constexpr auto TEARTAILFACT = 1.1F;
  constexpr auto TEARTWIST    = 1.6F;
  IniFile.formSides           = TEARSIDES;
  IniFile.tearTailLength      = TEARTAILFACT;
  IniFile.tearTwistStep       = 0.0F;
  IniFile.tearTwistRatio      = TEARTWIST;
  initTearDlg(hwndlg);
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handlePaisleyDefault(HWND hwndlg) {
  constexpr auto PAISSIDES    = uint16_t {24U};
  constexpr auto PAISTAILFACT = 1.15F;
  constexpr auto PAISSTEP     = 0.3F;
  constexpr auto PAISTWIST    = 1.8F;
  IniFile.formSides           = PAISSIDES;
  IniFile.tearTailLength      = PAISTAILFACT;
  IniFile.tearTwistStep       = PAISSTEP * PFGRAN;
  IniFile.tearTwistRatio      = PAISTWIST;
  initTearDlg(hwndlg);
}
// ReSharper restore CppParameterMayBeConst

void handleTearInit(HWND hwndlg) {
  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
#ifdef TESTCODE
  auto const uDpi     = GetDpiForWindow(hwndlg);
  RECT       rcClient = {};
  GetWindowRect(hwndlg, &rcClient);
  auto uWidth  = MulDiv((rcClient.right - rcClient.left), uDpi, 96);
  auto uHeight = MulDiv((rcClient.bottom - rcClient.top), uDpi, 96);
  SetWindowPos(hwndlg, nullptr, rcClient.left, rcClient.top, uWidth, uHeight, SWP_NOZORDER | SWP_NOACTIVATE);

  auto hFont = displayText::getThrEdFont(400);
  EnumChildWindows(
      hwndlg,
      [](HWND p_hWnd, LPARAM lParam) -> BOOL {
	    SendMessage(p_hWnd, WM_SETFONT, (WPARAM)lParam, MAKELPARAM(TRUE, 0));
	    return TRUE;
      },
      (LPARAM)hFont);
#endif // !TESTCODE
  initTearDlg(hwndlg);
}

// ReSharper disable CppParameterMayBeConst
auto CALLBACK tearprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG:
	  handleTearInit(hwndlg);
	  break;
	case WM_COMMAND: {
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, FALSE);
		  return TRUE;
		}
		case IDOK:
		  handleTearIDOK(hwndlg);
		  EndDialog(hwndlg, TRUE);
		  break;
		case IDC_DEFTEAR:
		  handleTearDefault(hwndlg);
		  break;
		case IDC_DEFPAIS:
		  handlePaisleyDefault(hwndlg);
		  break;
		default: {
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
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void wavinit(HWND hwndlg) {
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), format(FMT_COMPILE(L"{:d}"), IniFile.wavePoints).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), format(FMT_COMPILE(L"{:d}"), IniFile.waveStart).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), format(FMT_COMPILE(L"{:d}"), IniFile.waveEnd).c_str());
  SetWindowText(GetDlgItem(hwndlg, IDC_WAVS), format(FMT_COMPILE(L"{:d}"), IniFile.waveLobes).c_str());
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void handleWaveIDOK(HWND hwndlg) {
  auto buffer = std::array<wchar_t, HBUFSIZ> {};
  GetWindowText(GetDlgItem(hwndlg, IDC_WAVPNTS), buffer.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.wavePoints, buffer.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_WAVSTRT), buffer.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.waveStart, buffer.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_WAVEND), buffer.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.waveEnd, buffer.data());
  GetWindowText(GetDlgItem(hwndlg, IDC_WAVS), buffer.data(), HBUFSIZ);
  wrap::wcsToULong(IniFile.waveLobes, buffer.data());

  constexpr auto WPCLAMP = uint16_t {100U}; // max number of points in a wave form
  IniFile.wavePoints     = std::clamp(IniFile.wavePoints, uint16_t {3U}, WPCLAMP);
  if (IniFile.waveStart == IniFile.waveEnd) {
	IniFile.waveEnd += IniFile.wavePoints >> 2U;
  }
  IniFile.waveStart %= IniFile.wavePoints;
  IniFile.waveEnd %= IniFile.wavePoints;
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
auto CALLBACK wavprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> INT_PTR {
  UNREFERENCED_PARAMETER(lparam);
  switch (umsg) {
	case WM_INITDIALOG: {
	  SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
	  wavinit(hwndlg);
	  break;
	}
	case WM_COMMAND: {
	  switch (LOWORD(wparam)) {
		case IDCANCEL: {
		  EndDialog(hwndlg, FALSE);
		  return TRUE;
		}
		case IDOK:
		  handleWaveIDOK(hwndlg);
		  EndDialog(hwndlg, TRUE);
		  break;
		case IDC_DEFWAV: {
		  IniFile.wavePoints = IWAVPNTS;
		  IniFile.waveStart  = IWAVSTRT;
		  IniFile.waveEnd    = IWAVEND;
		  IniFile.waveLobes  = IWAVS;
		  wavinit(hwndlg);
		  break;
		}
		default: {
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
// ReSharper restore CppParameterMayBeConst

} // namespace

void formForms::maxtsiz(std::wstring const& label, SIZE& textSize) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto labelSize = SIZE {};
  wrap::getTextExtentPoint32(GetDC(ThrEdWindow), label.data(), wrap::toUnsigned(label.size()), &labelSize);
  textSize.cx = std::max(textSize.cx, labelSize.cx);
  textSize.cy = std::max(textSize.cy, labelSize.cy);
}

void formForms::refrm() {
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  if (Instance->stateMap.testAndReset(StateFlag::PRFACT)) {
	destroyPreferencesWindow();
	Instance->stateMap.reset(StateFlag::WASRT);
  }
  LabelWindowSize = ValueWindowSize = {};
  Instance->stateMap.set(StateFlag::REFCNT); // don't create windows - just size them
  auto formMenuEntryCount = 0U;
  refrmfn(form, formMenuEntryCount);
  destroyFormDataSheet();
  FormDataSheet = CreateWindow(L"STATIC",
                               nullptr,
                               WS_CHILD | WS_VISIBLE | WS_BORDER,
                               ButtonWidthX3 + 3,
                               3,
                               LabelWindowSize.cx + ValueWindowSize.cx + 18,
                               (LabelWindowSize.cy * gsl::narrow<LONG>(formMenuEntryCount)) + 12,
                               ThrEdWindow,
                               nullptr,
                               ThrEdInstance,
                               nullptr);
  Instance->stateMap.reset(StateFlag::REFCNT); // this time create the windows
  refrmfn(form, formMenuEntryCount);
}

// ReSharper disable CppParameterMayBeConst
void formForms::sidwnd(HWND wnd) {
  auto windowRect = RECT {};
  thred::resetSideBuffer();
  thred::unsid(false);
  GetWindowRect(wnd, &windowRect);
  GetWindowRect(FormDataSheet, &MsgRect);
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
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void formForms::prfsid(HWND wnd) {
  thred::resetSideBuffer();
  thred::unsid(true);
  auto windowRect = RECT {};
  GetWindowRect(wnd, &windowRect);
  GetClientRect(PreferencesWindow, &MsgRect);
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
// ReSharper restore CppParameterMayBeConst

void formForms::prfmsg() {
  auto preferenceRect = RECT {};
  if (Instance->stateMap.testAndReset(StateFlag::INSRT)) {
	Instance->stateMap.set(StateFlag::WASRT);
  }
  Instance->stateMap.reset(StateFlag::BIGBOX);
  Instance->selectedFormList.clear();
  if (FormDataSheet != nullptr) {
	destroyFormDataSheet();
	thred::unsid(true);
  }
  LabelWindowSize.cx = 0;
  LabelWindowSize.cy = 0;
  ValueWindowSize.cx = 0;
  ValueWindowSize.cy = 0;
  LabelWindowSize    = maxwid();
  LabelWindowSize.cx += TXTMARG2;
  maxtsiz(displayText::loadStr(IDS_TAPR), ValueWindowSize);
  destroyPreferencesWindow();
  auto const windowWidth = LabelWindowSize.cx + ValueWindowSize.cx + 18;
  PreferencesWindow      = CreateWindow(L"STATIC",
                                   nullptr,
                                   WS_CHILD | WS_VISIBLE | WS_BORDER,
                                   ButtonWidthX3 + 3,
                                   3,
                                   windowWidth,
                                   (LabelWindowSize.cy * PRFLINS) + 12,
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
  prflin(format(FMT_COMPILE(L"{}"), AppliqueColor + 1U), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.AppStitchLen * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), Instance->borderWidth * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), ButtonholeCornerLength * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.chainSpace * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.chainRatio), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f} mm"), IniFile.clipOffset * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{}"), IniFile.fillPhase), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.eggRatio), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.fillAngle * RADDEGF), *row++);
  auto choice = Instance->userFlagMap.test(UserFlag::SQRFIL) ? displayText::loadStr(IDS_SQR)
                                                             : displayText::loadStr(IDS_PNTD);
  prflin(choice, *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), LineSpacing * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{}"), thred::duthrsh(thred::getShowStitchThreshold())), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f} mm"), IniFile.gridSize * IPFGRAN), *row++);
  form::sethup();
  prflin(format(FMT_COMPILE(L"{}"), displayText::loadStr(wrap::toUnsigned(IniFile.hoopType) - 1U + IDS_HUP0)),
         *row++);
  prflin(format(FMT_COMPILE(L"{:.0f} mm"), IniFile.hoopSizeY * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.0f} mm"), IniFile.hoopSizeX * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.lensRatio), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.cursorNudgeStep), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), PicotSpacing * IPFGRAN), *row++);
  choice = Instance->userFlagMap.test(UserFlag::BLUNT) ? displayText::loadStr(IDS_BLUNT)
                                                       : displayText::loadStr(IDS_TAPR);
  prflin(choice, *row++);
  choice = Instance->userFlagMap.test(UserFlag::DUND) ? displayText::loadStr(IDS_ON)
                                                      : displayText::loadStr(IDS_OFF);
  prflin(choice, *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), thred::getSmallStitchLength() * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), SnapLength * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), SpiralWrap), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), thred::getStarRatio()), *row++);
  prflin(format(FMT_COMPILE(L"{}"), thred::duthrsh(thred::getStitchBoxesThreshold())), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), IniFile.maxStitchLength * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), UserStitchLength * IPFGRAN), *row++);
  prflin(format(FMT_COMPILE(L"{:.2f}"), MinStitchLength * IPFGRAN), *row);
  Instance->stateMap.set(StateFlag::PRFACT);
  ReleaseDC(ThrEdWindow, preferenceDC);
}

void formForms::frmnum() {
  auto const& formList = Instance->formList;

  if (formList.empty() || !Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::shoseln(IDS_FRM1MSG, IDS_SETFRM);
	return;
  }
  displayText::showMessage(IDS_FRML, formList.size());
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRFNUM);
  thred::numWnd();
}

void formForms::dasyfrm() {
  constexpr auto DASYSIZE = 6.0F; // ratio of default daisy form to the screen size
  thred::unmsg();
  // set the daisy form parameters
  if (auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_DASY), ThrEdWindow, &dasyproc);
      nResult < 1) {
	Instance->stateMap.reset(StateFlag::FORMIN);
	return;
  }
  auto const referencePoint =
      F_POINT {wrap::midl(ZoomRect.right, ZoomRect.left), wrap::midl(ZoomRect.top, ZoomRect.bottom)};
  auto form        = FRM_HEAD {};
  form.vertexIndex = wrap::toUnsigned(Instance->formVertices.size());
  form.attribute   = gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
  // Determine the width of a daisy based on the Zoom rectangle and the default width percent.
  auto       maximumXsize = ZoomRect.right - ZoomRect.left;
  auto const maximumYsize = ZoomRect.top - ZoomRect.bottom;
  maximumXsize            = std::max(maximumXsize, maximumYsize);
  maximumXsize /= DASYSIZE;
  auto       diameter     = IniFile.daisyDiameter;
  auto       petalLength  = IniFile.daisyPetalLen;
  auto       holeDiameter = IniFile.daisyHoleDiameter;
  auto const ratio        = maximumXsize / (diameter + petalLength);
  // now adjust on the ratio
  diameter *= ratio;
  petalLength *= ratio;
  holeDiameter *= ratio;
  form.type    = FRMFPOLY;
  auto iVertex = 0U;
  auto fref    = 0U;
  if (Instance->userFlagMap.test(UserFlag::DAZHOL)) { // add the hole at the center of the daisy
	auto       angle            = PI_F2;
	auto const holeVertexCount  = IniFile.daisyPetalCount * IniFile.daisyInnerCount;
	auto const holeSegmentAngle = PI_F2 / wrap::toFloat(holeVertexCount);
	Instance->formVertices.emplace_back(referencePoint.x + (diameter * cos(angle)),
	                                    referencePoint.y + (diameter * sin(angle)));
	++iVertex;
	for (auto iSegment = 0U; iSegment < holeVertexCount + 1U; ++iSegment) {
	  Instance->formVertices.emplace_back(referencePoint.x + (holeDiameter * cos(angle)),
	                                      referencePoint.y + (holeDiameter * sin(angle)));
	  ++iVertex;
	  angle -= holeSegmentAngle;
	}
	fref = iVertex;
  }
  auto       petalVertexCount = IniFile.daisyPetalCount * IniFile.daisyPetalPoints;
  auto       petalPointCount  = IniFile.daisyPetalPoints;
  auto const borderType       = IniFile.daisyBorderType;
  if (borderType == DHART) { // update the petal count for heart shaped daisies
	petalPointCount  = (IniFile.daisyHeartCount + 1U) * 2U;
	petalVertexCount = IniFile.daisyPetalCount * petalPointCount;
  }
  auto const petalSegmentAngle = PI_F2 / wrap::toFloat(petalVertexCount);
  auto const deltaPetalAngle   = PI_F / wrap::toFloat(IniFile.daisyPetalPoints);
  if (Instance->userFlagMap.test(UserFlag::DAZD)) { // add the satin guides
	form.satinGuideCount = IniFile.daisyPetalCount - 1U;
	form.wordParam       = IniFile.daisyPetalCount * IniFile.daisyInnerCount + 1U;
	form.satinGuideIndex = satin::adsatk(IniFile.daisyPetalCount - 1);
  }
  auto const halfPetalPointCount = IniFile.daisyPetalPoints / 2;
  auto       angle               = 0.0F;
  for (auto iMacroPetal = 0U; iMacroPetal < IniFile.daisyPetalCount; ++iMacroPetal) { // add the petals
	auto petalPointAngle = 0.0F;
	PseudoRandomValue    = SEED;
	for (auto iPoint = 0U; iPoint < petalPointCount; ++iPoint) {
	  auto distanceFromDaisyCenter = 0.0F;
	  switch (borderType) {
		case DSIN: { // sin wave
		  distanceFromDaisyCenter = diameter + sin(petalPointAngle) * petalLength;
		  petalPointAngle += deltaPetalAngle;
		  break;
		}
		case DRAMP: { // ramp
		  distanceFromDaisyCenter =
		      diameter + (wrap::toFloat(iPoint) / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
		  break;
		}
		case DSAW: { // sawtooth
		  auto const sawPointCount =
		      wrap::toFloat(iPoint > halfPetalPointCount ? IniFile.daisyPetalPoints - iPoint : iPoint);
		  auto const offset = (sawPointCount / wrap::toFloat(IniFile.daisyPetalPoints) * petalLength);
		  distanceFromDaisyCenter = diameter + offset;
		  break;
		}
		case DRAG: { // ragged
		  distanceFromDaisyCenter = diameter + wrap::toFloat(form::psg() % IniFile.daisyPetalPoints) /
		                                           wrap::toFloat(IniFile.daisyPetalPoints) * petalLength;
		  break;
		}
		case DCOG: { // cog wheel
		  distanceFromDaisyCenter = diameter;
		  if (iPoint > halfPetalPointCount) {
			distanceFromDaisyCenter += petalLength;
		  }
		  break;
		}
		case DHART: { // heart shaped
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
	  Instance->formVertices.emplace_back(referencePoint.x + (cos(angle) * distanceFromDaisyCenter),
	                                      referencePoint.y + (sin(angle) * distanceFromDaisyCenter));
	  ++iVertex;
	  angle += petalSegmentAngle;
	  if (Instance->userFlagMap.test(UserFlag::DAZD) && iMacroPetal != IniFile.daisyPetalCount - 1) {
		auto const itGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex + iMacroPetal);
		itGuide->start = (IniFile.daisyPetalCount - iMacroPetal - 1) * IniFile.daisyInnerCount + 1U;
		itGuide->finish = iVertex;
	  }
	}
  }
  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);

  if (Instance->userFlagMap.test(UserFlag::DAZHOL)) {
	auto           referenceVertex = wrap::next(itVertex, fref - 1U);
	constexpr auto HOLEMARG        = 0.01F; // hole margin offset
	referenceVertex->y += HOLEMARG;
	++referenceVertex;
	referenceVertex->y += HOLEMARG;
  }
  form.vertexCount = iVertex;
  if (Instance->userFlagMap.test(UserFlag::DAZD)) {
	form.type      = SAT;
	form.attribute = 1;
  }
  Instance->stateMap.set(StateFlag::INIT);
  form.outline();
  auto& formList = Instance->formList;

  formList.push_back(form);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  for (auto iMacroPetal = 0U; iMacroPetal < iVertex; ++iMacroPetal) {
	itVertex->x -= form.rectangle.left;
	itVertex->y -= form.rectangle.bottom;
	++itVertex;
  }
  FormMoveDelta      = F_POINT {};
  NewFormVertexCount = iVertex + 1U;
  Instance->stateMap.set(StateFlag::POLIMOV);
  form::setmfrm(ClosestFormToCursor);
  form::mdufrm();
}

void formForms::setear() {
  thred::unmsg();
  if (auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_TEAR), ThrEdWindow, &tearprc);
      nResult <= 0) {
	return;
  }
  thred::savdo();
  constexpr auto TWSTFACT = 4.0F; // teardrop twist factor

  auto twistStep = IniFile.tearTwistStep;
  form::durpoli(IniFile.formSides);
  auto&      form             = Instance->formList.back();
  auto const formVertexIndex  = form.vertexIndex;
  auto const formVertexCount  = form.vertexCount;
  auto       firstVertex      = wrap::next(Instance->formVertices.begin(), formVertexIndex);
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
  verticalPosition -= step * HALF;
  Instance->formVertices.push_back(*firstVertex);
  firstVertex = wrap::next(Instance->formVertices.begin(), formVertexIndex); // iterator invalidated by push_back
  nextVertex = std::next(firstVertex);
  if (twistStep != 0.0F) {
	firstVertex->x = nextVertex->x + twistStep / TWSTFACT;
  }
  else {
	firstVertex->x = middle;
  }
  firstVertex->y = verticalPosition;
  ++form.vertexCount;
  ++NewFormVertexCount;
  Instance->stateMap.set(StateFlag::FORMSEL);
  form::flipv();
  Instance->stateMap.reset(StateFlag::FORMSEL);
  auto const size =
      F_POINT {form.rectangle.right - form.rectangle.left, form.rectangle.top - form.rectangle.bottom};
  auto horizontalRatio     = wrap::toFloat(UnzoomedRect.cx) / TWSTFACT / size.x;
  horizontalRatio          = std::min(horizontalRatio, 1.0F);
  auto const verticalRatio = wrap::toFloat(UnzoomedRect.cy) / TWSTFACT / size.y;
  horizontalRatio          = std::min(horizontalRatio, verticalRatio);
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
  auto       shiftedVertex = firstVertex; // assignment is optimized out, but is clearer
  auto const left          = form.rectangle.left;
  auto const bottom        = form.rectangle.bottom;
  auto const vertexMax     = form.vertexCount;
  for (auto iVertex = 0U; iVertex < vertexMax; ++iVertex) {
	shiftedVertex->x -= left;
	shiftedVertex->y -= bottom;
	++shiftedVertex;
  }
}

void formForms::wavfrm() {
  thred::unmsg();
  if (auto const nResult = DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_WAV), ThrEdWindow, &wavprc);
      nResult <= 0) {
	return;
  }
  thred::savdo();
  auto points = std::vector<F_POINT> {};
  points.reserve(IniFile.wavePoints);
  // reuse regular polygon code to build the template for points
  form::durpoli(IniFile.wavePoints);
  auto& formList = Instance->formList;

  auto&      form            = formList.back();
  auto const formVertexIndex = form.vertexIndex;
  form::mdufrm();
  auto iPoint      = 0U;
  auto waveIndex   = IniFile.waveStart;
  auto firstVertex = wrap::next(Instance->formVertices.begin(), formVertexIndex);
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
  Instance->formVertices.resize(Instance->formVertices.size() + formVerticesSize);
  firstVertex = wrap::next(Instance->formVertices.begin(), formVertexIndex); // resize may invalidate iterator

  auto itVertex = firstVertex; // copy intended
  for (auto iLobe = 0U; iLobe < IniFile.waveLobes; ++iLobe) {
	if ((iLobe & 1U) != 0U) {
	  for (auto index = 0U; index < count; ++index) {
		*itVertex = currentPosition;
		++itVertex;
		currentPosition += points[index];
	  }
	}
	else {
	  for (auto index = count; index != 0; --index) {
		*itVertex = currentPosition;
		++itVertex;
		auto const prevCount = index - 1U;
		currentPosition += points[prevCount];
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
  Instance->stateMap.reset(StateFlag::FORMSEL);
  auto const selectedSize =
      F_POINT {form.rectangle.right - form.rectangle.left, form.rectangle.top - form.rectangle.bottom};
  constexpr auto WAVSIZE         = 4.0F; // wave size factor
  auto           horizontalRatio = wrap::toFloat(UnzoomedRect.cx) / WAVSIZE / selectedSize.x;
  horizontalRatio                = std::min(horizontalRatio, 1.0F);
  auto const verticalRatio       = wrap::toFloat(UnzoomedRect.cy) / WAVSIZE / selectedSize.y;
  horizontalRatio                = std::min(horizontalRatio, verticalRatio);
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
  auto const left     = formList.back().rectangle.left;
  auto const bottom   = formList.back().rectangle.bottom;
  for (auto index = 0U; index < vertexCount; ++index) {
	vShifted->x -= left;
	vShifted->y -= bottom;
	++vShifted;
  }
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  form::setmfrm(ClosestFormToCursor);
  form::mdufrm();
}

void formForms::destroyPreferencesWindow() noexcept {
  if (nullptr == PreferencesWindow) {
	return;
  }
  thred::clearLabelWindow();
  std::ranges::fill(Instance->valueWindow, nullptr);
  DestroyWindow(PreferencesWindow);
  PreferencesWindow = nullptr;
}

void formForms::redrawPreferencesWindow() noexcept {
  if (nullptr != PreferencesWindow) {
	RedrawWindow(PreferencesWindow, nullptr, nullptr, RDW_INVALIDATE);
  }
}

void formForms::getPreferencesRect(RECT& prefRect) noexcept {
  GetWindowRect(PreferencesWindow, &prefRect);
}

void formForms::destroyFormDataSheet() noexcept {
  if (nullptr == FormDataSheet) {
	return;
  }
  thred::clearLabelWindow();
  std::ranges::fill(Instance->valueWindow, nullptr);
  DestroyWindow(FormDataSheet);
  FormDataSheet = nullptr;
}

auto formForms::inFormDataSheet() noexcept -> bool {
  return nullptr != FormDataSheet;
}

void formForms::redrawFormDataSheet() noexcept {
  if (nullptr != FormDataSheet) {
	RedrawWindow(FormDataSheet, nullptr, nullptr, RDW_INVALIDATE);
  }
}

void formForms::getFormDataSheetRect(RECT& dataRect) noexcept {
  GetWindowRect(FormDataSheet, &dataRect);
}
