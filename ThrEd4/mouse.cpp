// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "backup.h"
#include "displayText.h"
#include "EnumMap.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "mouse.h"
#include "point.h"
#include "Resources/resource.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "trace.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

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
#include <windef.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <filesystem>
// ReSharper disable CppUnusedIncludeDirective
#include <ranges>
// ReSharper restore CppUnusedIncludeDirective
#include <string>
#include <vector>

// mouse internal namespace
namespace {
// cursors
auto FormCursor            = gsl::narrow_cast<HCURSOR>(nullptr); // form
auto DLineCursor           = gsl::narrow_cast<HCURSOR>(nullptr); // dline
auto NeedleUpCursor        = gsl::narrow_cast<HCURSOR>(nullptr); // upright needle
auto NeedleLeftUpCursor    = gsl::narrow_cast<HCURSOR>(nullptr); // left up needle
auto NeedleLeftDownCursor  = gsl::narrow_cast<HCURSOR>(nullptr); // left down needle
auto NeedleRightUpCursor   = gsl::narrow_cast<HCURSOR>(nullptr); // right up needle
auto NeedleRightDownCursor = gsl::narrow_cast<HCURSOR>(nullptr); // right down needle
auto ArrowCursor           = gsl::narrow_cast<HCURSOR>(nullptr); // arrow
auto CrossCursor           = gsl::narrow_cast<HCURSOR>(nullptr); // cross

// Definitions
auto chkok() noexcept -> bool;
auto finrng(uint32_t find) noexcept -> bool;
void moveForms();

constexpr auto nxtcrnr(uint32_t corner) -> uint32_t;

void rngal();
void unmov();
void updateCursor();

// Functions
auto chkok() noexcept -> bool {
  return thred::chkwnd(OKButton);
}

auto finrng(uint32_t const find) noexcept -> bool {
  auto const& formList = Instance->FormList;

  if (auto const& rectFind = formList.operator[](find).rectangle;
      rectFind.left < StitchRangeRect.left || rectFind.right > StitchRangeRect.right ||
      rectFind.bottom < StitchRangeRect.bottom || rectFind.top > StitchRangeRect.top) {
	return false;
  }
  if (ActiveLayer == 0U) {
	return true;
  }
  auto const cod = gsl::narrow_cast<uint8_t>(
      gsl::narrow_cast<uint8_t>(formList.operator[](find).attribute & FRMLMSK) >> 1U);
  return cod == 0U || ActiveLayer == cod;
}

void moveForms() {
  auto const point =
      POINT {WinMsg.pt.x - std::lround(FormMoveDelta.x) - StitchWindowOrigin.x - SelectedFormsRect.left,
             WinMsg.pt.y - std::lround(FormMoveDelta.y) - StitchWindowOrigin.y - SelectedFormsRect.top};
  form::ratsr();
  FormMoveDelta = F_POINT {wrap::toFloat(point.x) / HorizontalRatio, -wrap::toFloat(point.y) / VerticalRatio};

  auto& formList = Instance->FormList;
  if (Instance->StateMap.test(StateFlag::FPSEL)) { // moving a group of form points
	// clang-format off
	  auto&      form            = formList.operator[](ClosestFormToCursor);
	  auto       iSelectedVertex = SelectedFormVertices.start;
	  auto const itVertex        = wrap::next(Instance->FormVertices.begin(), form.vertexIndex);
	// clang-format on
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const thisIt = wrap::next(itVertex, iSelectedVertex);
	  *thisIt += FormMoveDelta;
	  iSelectedVertex = form::pdir(form, iSelectedVertex);
	}
	thred::setpsel();
	form.outline();
	form::refil(ClosestFormToCursor);
	Instance->StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (Instance->StateMap.test(StateFlag::BIGBOX)) {
	  thred::savdo();
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
		form::frmadj(iForm);
	  }
	  for (auto& stitch : Instance->StitchBuffer) {
		stitch += FormMoveDelta;
	  }
	  form::selal();
	}
	else {
	  thred::savdo();
	  for (auto const selectedForm : Instance->SelectedFormList) {
		form::frmadj(selectedForm);
	  }
	  form::frmsadj();
	  Instance->StateMap.set(StateFlag::RESTCH);
	}
  }
}

constexpr auto nxtcrnr(uint32_t const corner) -> uint32_t {
  return (corner + 1U) & 3U;
}

void rngal() {
  if (Instance->StateMap.testAndReset(StateFlag::WASFPNT)) {
	return;
  }
  Instance->StateMap.reset(StateFlag::GRPSEL);
  auto prng = std::vector<RANGE> {};
  Instance->StateMap.reset(StateFlag::GRPSEL);
  auto iStitch     = 0U;
  auto flagInRange = false;
  for (; iStitch < wrap::toUnsigned(Instance->StitchBuffer.size()); ++iStitch) {
	if (thred::inrng(iStitch)) {
	  if (!flagInRange) {
		prng.push_back(RANGE {.start = iStitch, .finish = 0U});
		flagInRange = true;
	  }
	}
	else {
	  if (flagInRange) {
		prng.back().finish = iStitch - 1U;
		flagInRange        = false;
	  }
	}
  }
  if (flagInRange) {
	prng.back().finish = iStitch - 1U;
  }
  if (prng.empty()) {
	return;
  }
  auto maximumLength = 0U;
  auto largestRange  = 0U;
  for (auto index = 0U; index < prng.size(); ++index) {
	if (auto const length = prng[index].finish - prng[index].start; length > maximumLength) {
	  maximumLength = length;
	  largestRange  = index;
	}
  }
  if (maximumLength != 0U) {
	ClosestPointIndex = prng[largestRange].start;
	GroupStitchIndex  = prng[largestRange].finish;
	Instance->StateMap.set(StateFlag::GRPSEL);
  }
  thred::gotbox();
}

void unmov() {
  if (Instance->StateMap.testAndReset(StateFlag::SHOMOV)) {
	thred::ritmov(ClosestFormToCursor);
  }
}

void updateCursor() {
  if (UserFlagMap->test(UserFlag::NEDOF)) {
	wrap::setCursor(CrossCursor);
  }
  else {
	if (Instance->StateMap.test(StateFlag::LIN1)) {
	  wrap::setCursor(NeedleUpCursor);
	}
	else {
	  if (Instance->StitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).x >
	      Instance->StitchBuffer.operator[](ClosestPointIndex).x) {
		if (Instance->StitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).y >
		    Instance->StitchBuffer.operator[](ClosestPointIndex).y) {
		  wrap::setCursor(NeedleLeftUpCursor);
		}
		else {
		  wrap::setCursor(NeedleLeftDownCursor);
		}
	  }
	  else {
		if (Instance->StitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).y >
		    Instance->StitchBuffer.operator[](ClosestPointIndex).y) {
		  wrap::setCursor(NeedleRightUpCursor);
		}
		else {
		  wrap::setCursor(NeedleRightDownCursor);
		}
	  }
	}
  }
}

} // namespace

auto mouse::getArrowCursor() noexcept -> HCURSOR {
  return ArrowCursor;
}

void mouse::crtcurs() noexcept {
  ArrowCursor           = LoadCursor(nullptr, IDC_ARROW);
  CrossCursor           = LoadCursor(nullptr, IDC_CROSS);
  FormCursor            = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Form));
  DLineCursor           = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_DLIN));
  NeedleUpCursor        = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Upright));
  NeedleRightDownCursor = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightDown));
  NeedleRightUpCursor   = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightUp));
  NeedleLeftDownCursor  = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftDown));
  NeedleLeftUpCursor    = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftUp));
}

void mouse::dstcurs() noexcept {
  DestroyCursor(FormCursor);
  DestroyCursor(DLineCursor);
  DestroyCursor(NeedleUpCursor);
  DestroyCursor(NeedleRightDownCursor);
  DestroyCursor(NeedleRightUpCursor);
  DestroyCursor(NeedleLeftDownCursor);
  DestroyCursor(NeedleLeftUpCursor);
}

auto mouse::handleEitherButtonDown() -> bool {
  if (FormDataSheet != nullptr) {
	auto formDataRect = RECT {};
	GetWindowRect(FormDataSheet, &formDataRect);
  }
  if (Instance->StateMap.testAndReset(StateFlag::THUMON)) {
	if (chkok()) {
	  thred::save();
	  thred::thumbak();
	  thred::unmsg();
	  return true;
	}
	if (thred::chkwnd(DiscardButton)) {
	  thred::thumbak();
	  thred::unmsg();
	  return true;
	}
	Instance->StateMap.set(StateFlag::BAKSHO);
	thred::unbsho();
	Instance->StateMap.reset(StateFlag::THUMSHO);
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::BAKSHO)) {
	if (WinMsg.message == WM_RBUTTONDOWN) {
	  Instance->StateMap.set(StateFlag::RBUT);
	}
	else {
	  Instance->StateMap.reset(StateFlag::RBUT);
	}
	if (thred::displayBackups()) {
	  return true;
	}
  }
  if (thred::inColorbar()) {
	thred::unpat();
	auto const colorBarPosition = thred::getColorbarVertPosition();
	if (WinMsg.message == WM_RBUTTONDOWN) {
	  if ((WinMsg.wParam & MK_SHIFT) != 0U &&
	      (Instance->StateMap.test(StateFlag::SELBOX) || Instance->StateMap.test(StateFlag::GRPSEL))) {
		thred::unbox();
		GroupStitchIndex =
		    wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(Instance->StitchBuffer.size()));
		Instance->StateMap.set(StateFlag::GRPSEL);
		thred::grpAdj();
		thred::nuAct(GroupStitchIndex);
		Instance->StateMap.set(StateFlag::RESTCH);
	  }
	  else {
		ClosestPointIndex =
		    wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(Instance->StitchBuffer.size()));
		thred::nuAct(ClosestPointIndex);
		thred::movbox();
		if (Instance->StateMap.testAndReset(StateFlag::GRPSEL)) {
		  Instance->StateMap.reset(StateFlag::SCROS);
		  Instance->StateMap.reset(StateFlag::ECROS);
		  Instance->StateMap.set(StateFlag::RESTCH);
		}
	  }
	}
	else {
	  ClosestPointIndex =
	      wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(Instance->StitchBuffer.size()));
	  thred::nuAct(ClosestPointIndex);
	  thred::rstAll();
	  Instance->StateMap.set(StateFlag::SELBOX);
	  thred::selCol();
	}
	thred::redrawColorBar();
	return true;
  }
  return false;
}

auto mouse::handleLeftButtonDown(std::vector<POINT>& stretchBoxLine,
                                 float&              xyRatio,
                                 FRM_HEAD const&     textureForm,
                                 bool&               retflag) -> bool {
  static auto threadSizeSelected = uint32_t {}; // thread selected for size change

  retflag = true;
  if (wrap::pressed(VK_SHIFT) && thred::inStitchWin()) { // shift key pressed
	xt::dushft();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::TXTRED) && !Instance->StateMap.test(StateFlag::FORMIN)) { // texture entry
	texture::txtlbut(textureForm);
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::FSETFCOL)) { // setting fill color for forms
	thred::unsid();
	thred::unmsg();
	if (thred::inDefaultColorWindows()) {
	  xt::dufcol(thred::getVerticalIndex() + 1U);
	  return true;
	}
  }
  if (Instance->StateMap.testAndReset(StateFlag::FSETBCOL)) { // setting border color for forms
	thred::unsid();
	thred::unmsg();
	if (thred::inDefaultColorWindows()) {
	  xt::dubcol(thred::getVerticalIndex() + 1U);
	  return true;
	}
  }
  if (Instance->StateMap.testAndReset(StateFlag::FPUNCLP)) { // pasting points into a form
	thred::savdo();
	thred::fixpclp(ClosestFormToCursor);
	Instance->FormList.operator[](ClosestFormToCursor).outline();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::FPSEL) && !Instance->StateMap.test(StateFlag::FUNCLP) &&
      !Instance->StateMap.test(StateFlag::ROTAT)) {
	Instance->SelectedFormsLine = Instance->SelectedPointsLine;
	SelectedFormsRect           = SelectedPixelsRect;
	if (thred::chkbig(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (Instance->StateMap.test(StateFlag::WASTRAC)) { // tracing
	trace::tracpar();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::HUPMSG)) { // hoop size update
	thred::updateHoopSize();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::INSFIL)) { // inserting a file
	thred::duinsfil();
	return true;
  }
  if (!Instance->StateMap.test(StateFlag::ROTAT) && Instance->StateMap.test(StateFlag::BIGBOX) &&
      thred::chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::DELSFRMS)) { // deleting a group of forms
	if (chkok()) {
	  thred::savdo();
	  Instance->StateMap.reset(StateFlag::DELTO);
	}
	else {
	  auto windowRect = RECT {};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (WinMsg.pt.x >= windowRect.left && WinMsg.pt.x <= windowRect.right &&
	      WinMsg.pt.y >= windowRect.top && WinMsg.pt.y <= windowRect.bottom) {
		thred::savdo();
		Instance->StateMap.set(StateFlag::DELTO);
	  }
	}
	thred::delsfrms();
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::MOVMSG)) { // tried to move an edited form
	if (chkok()) {
	  thred::savdo();
	  if (!Instance->SelectedFormList.empty()) {
		for (auto const selectedForm : Instance->SelectedFormList) {
		  form::refilfn(selectedForm);
		}
	  }
	  else {
		form::refilfn(ClosestFormToCursor);
	  }
	  thred::unmsg();
	}
	else {
	  backup::bak();
	}
	if (Instance->StateMap.testAndReset(StateFlag::WASFRMFRM)) {
	  formForms::refrm();
	}
	return true;
  }
  if (Instance->StateMap.test(StateFlag::FUNSCLP)) { // pasting a group of forms
	Instance->StateMap.reset(StateFlag::MOVFRMS);
	auto formsRect = F_RECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, formsRect);
	auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);

	FormMoveDelta = F_POINT {stitchPoint.x - wrap::midl(formsRect.right, formsRect.left),
	                         stitchPoint.y - wrap::midl(formsRect.top, formsRect.bottom)};
	auto const clipFormsCount = tfc::getClipFormCount();
	for (auto iForm = 0U; iForm < clipFormsCount; ++iForm) {
	  auto& formList = Instance->FormList;

	  ClosestFormToCursor = wrap::toUnsigned(formList.size() - iForm - 1U);
	  // clang-format off
	  auto& form     = formList.operator[](ClosestFormToCursor);
	  auto  itVertex = wrap::next(Instance->FormVertices.begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		*itVertex += FormMoveDelta;
		++itVertex;
	  }
	  form.outline();
	  form::refil(ClosestFormToCursor);
	}
	Instance->StateMap.reset(StateFlag::FUNSCLP);
	Instance->StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (!Instance->SelectedFormList.empty() && !Instance->StateMap.test(StateFlag::ROTAT) &&
      thred::chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (Instance->StateMap.test(StateFlag::SIDCOL) && thred::inDefaultColorWindows()) {
	return thred::updateFillColor();
  }
  if (Instance->StateMap.testAndReset(StateFlag::OSAV)) { // trying to load a new form
	if (chkok()) {
	  thred::save();
	  thred::nuTHRfile();
	  thred::unmsg();
	  return true;
	}
	if (thred::chkwnd(DiscardButton)) {
	  thred::nuTHRfile();
	  thred::unmsg();
	  return true;
	}
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::FCLOS)) {
	if (chkok()) {
	  thred::save();
	}
	else {
	  if (!thred::chkwnd(CancelButton)) {
		thred::closfn();
	  }
	}
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::SAVEX)) {
	if (chkok()) {
	  thred::save();
	  thred::reldun();
	}
	if (thred::chkwnd(DiscardButton)) {
	  thred::reldun();
	}
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::PRFACT)) { // update preferences
	return thred::updatePreferences();
  }
  if (!Instance->StateMap.test(StateFlag::ROTAT) && Instance->StateMap.test(StateFlag::GRPSEL)) { // if we have a group of stitches selected
	auto const* formControlPoints = thred::getFormControlPoints();
	auto const& controlPoint      = *formControlPoints;
	if (thred::iselpnt()) {
	  for (auto iSide = 0U; iSide < wrap::toUnsigned(stretchBoxLine.size()); ++iSide) {
		stretchBoxLine[iSide] = controlPoint[wrap::toSize(iSide) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		Instance->StateMap.set(StateFlag::STRTCH);
	  }
	  else {
		Instance->StateMap.set(StateFlag::EXPAND);
		xyRatio = (StitchRangeRect.right - StitchRangeRect.left) /
		          (StitchRangeRect.top - StitchRangeRect.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  Instance->StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (auto const relativePoint =
	        POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
	    relativePoint.x >= controlPoint[0].x && relativePoint.x <= controlPoint[2].x &&
	    relativePoint.y >= controlPoint[0].y && relativePoint.y <= controlPoint[4].y) {
	  thred::duSelbox();
	  Instance->StateMap.set(StateFlag::SELPNT);
	  SetCapture(ThrEdWindow);
	  Instance->StateMap.set(StateFlag::VCAPT);
	  thred::rSelbox();
	  return true;
	}
  }
  if (Instance->StateMap.testAndReset(StateFlag::NEWBAK)) { // starting a new design
	if (chkok()) {
	  thred::unmsg();
	  thred::save();
	  thred::newFil();
	  return true;
	}
	if (thred::chkwnd(DiscardButton)) {
	  thred::unmsg();
	  thred::newFil();
	  return true;
	}
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::PRGMSG)) { // deleting backups
	if (chkok()) {
	  thred::deldir();
	  return true;
	}
  }
  if (Instance->StateMap.test(StateFlag::DELFRM)) { // deleting a form
	auto code = 0;
	if (chkok()) {
	  thred::savdo();
	  Instance->StateMap.reset(StateFlag::DELTO);
	  code = 1;
	}
	else {
	  auto windowRect = RECT {};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (WinMsg.pt.x >= windowRect.left && WinMsg.pt.x <= windowRect.right &&
	      WinMsg.pt.y >= windowRect.top && WinMsg.pt.y <= windowRect.bottom) {
		thred::savdo();
		Instance->StateMap.set(StateFlag::DELTO);
		code = 1;
	  }
	}
	if (code != 0) {
	  thred::frmdel();
	  thred::coltab();
	  Instance->StateMap.set(StateFlag::RESTCH);
	}
	thred::unmsg();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::FILMSG)) { // user wants to remove an edited fill
	if (chkok()) {
	  thred::savdo();
	  form::unfil();
	  thred::coltab();
	  thred::unmsg();
	  return true;
	}
  }
  if (Instance->StateMap.testAndReset(StateFlag::SIZSEL)) { // updating thread sizes
	if (thred::inChangeThreadWindows()) {
	  thred::updateThreadSize(threadSizeSelected);
	  return true;
	}
	thred::destroyChangeThreadSizeWindows();
  }
  if (Instance->StateMap.testAndReset(StateFlag::POLIMOV)) { // placing a new form
	thred::savdo();
	form::setfrm();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::FORMIN)) {
	auto windowRect = RECT {};
	GetWindowRect(MsgWindow, &windowRect);
	if (WinMsg.pt.x >= windowRect.left && WinMsg.pt.x <= windowRect.right &&
	    WinMsg.pt.y >= windowRect.top && WinMsg.pt.y <= windowRect.bottom) {
	  auto const iFillType = (WinMsg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
	  if (Instance->StateMap.testAndReset(StateFlag::FENDIN)) {
		if (iFillType == 3) {
		  UserFlagMap->reset(UserFlag::SQRFIL);
		}
		if (iFillType == 4) {
		  UserFlagMap->set(UserFlag::SQRFIL);
		}
	  }
	  else {
		form::duform(iFillType);
	  }
	  switch (iFillType) {
		case FRMRPOLY - 1:
		case FRMSTAR - 1:
		case FRMSPIRAL - 1:
		case FRMHEART - 2:
		case FRMLENS - 2:
		case FRMEGG - 2:
		case FRMZIGZAG - 2:
		  return true;
		default: {
		}
	  }
	}
	thred::unmsg();
	return true;
  }
  if (PreferenceIndex == PRFAPPCOL + 1 && thred::inDefaultColorWindows()) {
	AppliqueColor = thred::getVerticalIndex();
	SetWindowText(Instance->ValueWindow.operator[](PRFAPPCOL),
	              format(FMT_COMPILE(L"{}"), AppliqueColor).c_str());
	thred::unsid();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::SIDACT)) {
	return thred::handleSideWindowActive();
  }
  if (FormDataSheet != nullptr) {
	return thred::handleFormDataSheet();
  }
  if (Instance->StateMap.test(StateFlag::INSFRM)) {
	thred::savdo();
	form::setins();
	return true;
  }
  auto& formList = Instance->FormList;

  if (Instance->StateMap.test(StateFlag::FUNCLP)) { // pasting a form
	Instance->StateMap.set(StateFlag::INIT);
	form::rstfrm();
	form::refil(ClosestFormToCursor);
	Instance->StateMap.reset(StateFlag::FUNCLP);
	if (Instance->StateMap.testAndReset(StateFlag::FPSEL)) {
	  formList.back().outline();
	}
	Instance->StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Instance->StateMap.test(StateFlag::SATCNKT)) { // connecting a satin form guide
	thred::savdo();
	satin::satknkt();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::SATPNT)) { // adding a satin point
	satin::satpnt1();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::SATIN)) {
	satin::satpnt0();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	if (!Instance->StateMap.test(StateFlag::FRMROT) && thred::chkForm(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (Instance->StateMap.testAndReset(StateFlag::MOVFRM)) { // moving a form vertex
	thred::savdo();
	form::setfrm();
	return true;
  }
  thred::unmsg();
  if (thred::inStitchWin() && !(Instance->StateMap.test(StateFlag::SIZSEL) && thred::inChangeThreadWindows())) { // we are in the stitch window, so choose an action
	thred::unpat();
	if (Instance->StateMap.testAndReset(StateFlag::ROTAT)) { // start rotating a form
	  thred::setRotateCapture();
	  return true;
	}
	if (Instance->StateMap.testAndReset(StateFlag::CLPSHO)) { // pasting
	  thred::savdo();
	  if (!Instance->StitchBuffer.empty() &&
	      (Instance->StateMap.testAndReset(StateFlag::SELBOX) || Instance->StateMap.testAndReset(StateFlag::INSRT)) &&
	      ClosestPointIndex != wrap::toUnsigned(Instance->StitchBuffer.size() - 1U)) {
		tfc::lodclp(ClosestPointIndex);
	  }
	  else {
		tfc::lodclp(wrap::toUnsigned(Instance->StitchBuffer.size()));
	  }
	  thred::rngadj();
	  thred::setFormControls();
	  thred::coltab();
	  Instance->StateMap.set(StateFlag::RESTCH);
	  return true;
	}
	if (Instance->StateMap.testAndReset(StateFlag::BOXZUM)) { // zooming in
	  Instance->StateMap.set(StateFlag::BZUMIN);
	  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = WinMsg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[0].y = ZoomBoxLine[1].y = WinMsg.pt.y - StitchWindowOrigin.y;
	  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	  ZoomBoxOrigin                       = thred::pxCor2stch(WinMsg.pt);
	  Instance->StateMap.set(StateFlag::VCAPT);
	  return true;
	}
	if (!formList.empty() && !Instance->StateMap.test(StateFlag::FRMOF)) {
	  if (!Instance->StateMap.test(StateFlag::INSRT)) {
		if (Instance->StateMap.testAndReset(StateFlag::FORMSEL)) {
		  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		}
		if (form::closfrm(ClosestFormToCursor)) {
		  Instance->StateMap.set(StateFlag::FRMPMOV);
		  form::frmovlin();
		  return true;
		}
	  }
	}
	if (Instance->StateMap.test(StateFlag::INIT)) {
	  thred::unlin();
	  if (Instance->StateMap.test(StateFlag::INSRT)) { // insert mode active
		auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
		auto       code =
		    (ActiveColor | USMSK | gsl::narrow_cast<decltype(ActiveColor)>(ActiveLayer << LAYSHFT) | NOTFRM) & NKNOTMSK;
		if (Instance->StateMap.test(StateFlag::LIN1)) { // inserting a stitch
		  if (Instance->StateMap.test(StateFlag::BAKEND)) {
			thred::xlin1();
			auto const iStitch = wrap::toUnsigned(Instance->StitchBuffer.size());
			Instance->StitchBuffer.emplace_back(stitchPoint.x, stitchPoint.y, code);
			thred::duzrat();
			InsertLine[0] = thred::stch2px1(iStitch);
			InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
			                 .y = WinMsg.pt.y - StitchWindowOrigin.y};
			thred::coltab();
			Instance->StateMap.set(StateFlag::RESTCH);
			return true;
		  }

		  thred::xlin1();
		  Instance->StitchBuffer.insert(Instance->StitchBuffer.begin(),
		                                F_POINT_ATTR {stitchPoint.x, stitchPoint.y, code});
		  Instance->StitchBuffer.front().attribute &= ~KNOTMSK;
		  InsertLine[0] = thred::stch2px1(0);
		  InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
		                   .y = WinMsg.pt.y - StitchWindowOrigin.y};
		  thred::coltab();
		  Instance->StateMap.set(StateFlag::RESTCH);
		  return true;
		}
		thred::xlin();
		if ((Instance->StitchBuffer.operator[](ClosestPointIndex).attribute & ALTYPMSK) != 0U &&
		    (Instance->StitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).attribute & ALTYPMSK) != 0U) {
		  if ((Instance->StitchBuffer.operator[](ClosestPointIndex).attribute & FRMSK) ==
		      (Instance->StitchBuffer.operator[](wrap::toSize(ClosestPointIndex) + 1U).attribute & FRMSK)) {
			code = Instance->StitchBuffer.operator[](ClosestPointIndex).attribute | USMSK;
		  }
		}
		++ClosestPointIndex;
		thred::savdo();
		Instance->StitchBuffer.insert(wrap::next(Instance->StitchBuffer.begin(), ClosestPointIndex),
		                              F_POINT_ATTR {stitchPoint.x, stitchPoint.y, code});
		thred::xlin();
		InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
		                 .y = WinMsg.pt.y - StitchWindowOrigin.y};
		InsertLine[0] = thred::stch2px1(ClosestPointIndex);
		InsertLine[2] = thred::stch2px1(ClosestPointIndex + 1U);
		thred::coltab();
		Instance->StateMap.set(StateFlag::RESTCH);
		displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
		return true;
	  }
	  if (auto closestPointIndexClone = uint32_t {};
	      !Instance->StateMap.test(StateFlag::HIDSTCH) && thred::closPnt1(closestPointIndexClone)) {
		thred::redrawCapturedStitch(closestPointIndexClone);
	  }
	}
	else {
	  if (thred::inStitchWin()) {
		thred::savdo();
		InsertLine[0]          = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
		                          .y = WinMsg.pt.y - StitchWindowOrigin.y};
		InsertLine[1]          = InsertLine[0];
		auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
		Instance->StitchBuffer.emplace_back(
		    stitchPoint.x,
		    stitchPoint.y,
		    USMSK | ActiveColor | gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT) | NOTFRM);
		thred::addColor(0, ActiveColor);
		Instance->StateMap.set(StateFlag::LIN1);
		Instance->StateMap.set(StateFlag::INSRT);
		SetCapture(ThrEdWindow);
		ClosestPointIndex = 1;
		Instance->StateMap.set(StateFlag::INIT);
		Instance->StateMap.set(StateFlag::BAKEND);
	  }
	}
	thred::ritot(wrap::toUnsigned(Instance->StitchBuffer.size()));
	Instance->StateMap.set(StateFlag::BOXSLCT);
	Instance->StateMap.set(StateFlag::BZUMIN);
	Instance->StateMap.set(StateFlag::NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = WinMsg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = WinMsg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	ZoomBoxOrigin                       = thred::pxCor2stch(WinMsg.pt);
	Instance->StateMap.set(StateFlag::VCAPT);
	return true;
  }
  if (WinMsg.hwnd == Instance->ButtonWin.operator[](HBOXSEL)) {
	form::boxsel();
	return true;
  }
  if (WinMsg.hwnd == Instance->ButtonWin.operator[](HUPTO)) {
	thred::toglup();
	return true;
  }
  if (WinMsg.hwnd == Instance->ButtonWin.operator[](HHID)) { // only show stitches of the selected color
	thred::toglHid();
	return true;
  }
  if (thred::inDefaultColorWindows()) { // user has clicked in the color windows
	if (WinMsg.message == WM_LBUTTONDOWN) {
	  thred::switchUserColors();
	  if (Instance->StateMap.test(StateFlag::HID)) {
		Instance->StateMap.reset(StateFlag::SELBOX);
		Instance->StateMap.reset(StateFlag::GRPSEL);
		Instance->StateMap.reset(StateFlag::SCROS);
		Instance->StateMap.reset(StateFlag::ECROS);
		Instance->StateMap.set(StateFlag::RESTCH);
		thred::redraw(Instance->ButtonWin.operator[](HHID));
	  }
	  else {
		if (!Instance->SelectedFormList.empty()) {
		  thred::savdo();
		  thred::nucols();
		  thred::coltab();
		  Instance->StateMap.set(StateFlag::RESTCH);
		  return true;
		}
		if (Instance->StateMap.test(StateFlag::FORMSEL)) { // user has selected a form so only update the color for that form
		  if (auto& form = formList.operator[](ClosestFormToCursor);
		      form.fillType != 0U || form.edgeType != 0U ||
		      (form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
			thred::savdo();
			if (form.fillType != 0U) {
			  form.fillColor = ActiveColor;
			  if (form.fillType == FTHF) {
				form.feather.color = ActiveColor;
			  }
			}
			if (form.edgeType != 0U) {
			  if (form.edgeType == EDGEAPPL) {
				form.borderColor &= APCOLMSK;
				form.borderColor |= ActiveColor;
			  }
			  else {
				form.borderColor = ActiveColor;
			  }
			}
			if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
			  form.underlayColor = ActiveColor;
			}
			auto const formCode = ClosestFormToCursor << FRMSHFT;
			for (auto& stitch : Instance->StitchBuffer) {
			  if ((stitch.attribute & ALTYPMSK) != 0U && (stitch.attribute & FRMSK) == formCode &&
			      (stitch.attribute & TYPMSK) != TYPMSK) {
				stitch.attribute &= NCOLMSK;
				stitch.attribute |= ActiveColor;
			  }
			}
			thred::coltab();
			Instance->StateMap.set(StateFlag::RESTCH);
		  }
		}
		else {
		  if (Instance->StateMap.test(StateFlag::GRPSEL)) { // user has selected a group of stitches so update stitch colors
			thred::savdo();
			if (GroupStartStitch != GroupEndStitch) {
			  for (auto const groupStitchRange =
			           std::ranges::subrange(wrap::next(Instance->StitchBuffer.begin(), GroupStartStitch),
			                                 wrap::next(Instance->StitchBuffer.begin(), GroupEndStitch));
			       auto& stitch : groupStitchRange) {
				stitch.attribute &= NCOLMSK;
				stitch.attribute |= ActiveColor;
			  }
			  thred::coltab();
			  Instance->StateMap.set(StateFlag::RESTCH);
			}
			else {
			  auto& stitch = Instance->StitchBuffer.operator[](GroupStartStitch);
			  stitch.attribute &= NCOLMSK;
			  stitch.attribute |= ActiveColor;
			}
		  }
		  else {
			if (Instance->StateMap.test(StateFlag::COL)) {
			  Instance->StateMap.set(StateFlag::RESTCH);
			}
		  }
		}
	  }
	}
	return true;
  }
  if (thred::inUserColorWindows()) { // user has clicked in the user color windows
	thred::updateUserColor();
	return true;
  }
  if (thred::inThreadSizeWindows()) { // user has clicked in the thread size windows
	if (WinMsg.message == WM_LBUTTONDOWN) {
	  threadSizeSelected = thred::createChangeThreadSizeWindows();
	}
	return true;
  }
  retflag = false;
  return false;
}

auto mouse::handleLeftButtonUp(float const xyRatio, float const rotationAngle, F_POINT& rotationCenter, bool& retflag)
    -> bool {
  retflag = true;
  if (wrap::pressed(VK_SHIFT) && thred::inStitchWin() && !Instance->StateMap.test(StateFlag::TXTRED)) { // shift key pressed as well
	texture::setshft();
	return true;
  }
  if (Instance->StateMap.test(StateFlag::TXTRED)) { // in texture fill editor
	texture::txtrup();
	return true;
  }
  ReleaseCapture();
  thred::movchk();
  if (Instance->StateMap.testAndReset(StateFlag::MOVFRMS)) { // moving forms or form points
	thred::savdo();
	moveForms();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::EXPAND)) { // expanding a form
	form::setexpand(xyRatio);
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::STRTCH)) { // stretching a form
	form::setstrtch();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::FRMOV)) { // moving a single form
	thred::savdo();
	form::rstfrm();
	Instance->StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::FRMPMOV)) { // moving a form point
	thred::savdo();
	form::setfpnt();
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::MOVCNTR)) { // moving the rotate center of a form
	rotationCenter = thred::pxCor2stch(WinMsg.pt);
	Instance->StateMap.set(StateFlag::ROTAT);
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::ROTCAPT)) { // rotating a form
	thred::rotfn(rotationAngle, rotationCenter);
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::SELPNT)) { // moving a group of stitches
	thred::savdo();
	ReleaseCapture();
	thred::unsel();
	auto const stitchPoint   = thred::pxCor2stch(WinMsg.pt);
	auto const adjustedPoint = thred::getAdjustedPoint(stitchPoint);

	for (auto const groupStitchRange =
	         std::ranges::subrange(wrap::next(Instance->StitchBuffer.begin(), GroupStartStitch),
	                               wrap::next(Instance->StitchBuffer.begin(), GroupEndStitch));
	     auto& stitch : groupStitchRange) {
	  stitch -= adjustedPoint;
	}
	Instance->StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Instance->StateMap.testAndReset(StateFlag::CAPT)) {
	thred::unlin();
	ReleaseCapture();
	Instance->StateMap.reset(StateFlag::CAPT);
	thred::savdo();
	auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);

	Instance->StitchBuffer.operator[](ClosestPointIndex) = stitchPoint;
	Instance->StitchBuffer.operator[](ClosestPointIndex).attribute |= USMSK;
	if (ZoomFactor < STCHBOX) {
	  thred::drawCapturedStitchBox();
	}
	Instance->StateMap.set(StateFlag::SELBOX);
	Instance->StateMap.reset(StateFlag::FRMPSEL);
	Instance->StateMap.set(StateFlag::RESTCH);
	return true;
  }
  if (Instance->StateMap.test(StateFlag::BZUMIN)) { // zooming in
	auto stitchPoint = thred::pxCor2stch(WinMsg.pt);
	if (Instance->StateMap.testAndReset(StateFlag::BOXSLCT)) {
	  if (ZoomBoxOrigin.x > stitchPoint.x) {
		StitchRangeRect.right = ZoomBoxOrigin.x;
		StitchRangeRect.left  = stitchPoint.x;
	  }
	  else {
		StitchRangeRect.right = stitchPoint.x;
		StitchRangeRect.left  = ZoomBoxOrigin.x;
	  }
	  if (ZoomBoxOrigin.y > stitchPoint.y) {
		StitchRangeRect.top    = ZoomBoxOrigin.y;
		StitchRangeRect.bottom = stitchPoint.y;
	  }
	  else {
		StitchRangeRect.top    = stitchPoint.y;
		StitchRangeRect.bottom = ZoomBoxOrigin.y;
	  }
	  if (Instance->StateMap.testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; ++iStitch) {
		  if (thred::inrng(iStitch)) {
			ClosestPointIndex = iStitch;
			Instance->StateMap.set(StateFlag::SELBOX);
			break;
		  }
		}
	  }
	  if (Instance->StateMap.testAndReset(StateFlag::NOSEL)) {
		Instance->SelectedFormList.clear();
		auto const& formList = Instance->FormList;
		// We potentially reserve too much memory, but the cost of reallocation is higher than the
		// small amount overallocated
		Instance->SelectedFormList.reserve(formList.size());
		Instance->StateMap.reset(StateFlag::FORMSEL);
		auto const maxForm = wrap::toUnsigned(formList.size());
		for (auto iForm = 0U; iForm < maxForm; ++iForm) {
		  if (finrng(iForm)) {
			Instance->SelectedFormList.push_back(iForm);
		  }
		}
		if (Instance->SelectedFormList.size() == 1) {
		  ReleaseCapture();
		  thred::gotbox();
		  ClosestFormToCursor   = Instance->SelectedFormList.front();
		  ClosestVertexToCursor = 0;
		  Instance->SelectedFormList.clear();
		  displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
		  Instance->StateMap.set(StateFlag::RESTCH);
		  Instance->StateMap.set(StateFlag::FORMSEL);
		  return true;
		}
		if (!Instance->SelectedFormList.empty()) {
		  thred::gotbox();
		  return true;
		}
		Instance->StateMap.reset(StateFlag::BZUMIN);
		Instance->StateMap.reset(StateFlag::BOXSLCT);
		Instance->StateMap.reset(StateFlag::FRMPSEL);
		Instance->StateMap.set(StateFlag::RESTCH);
	  }
	  if (Instance->StateMap.testAndReset(StateFlag::SELBOX)) {
		if (thred::inrng(ClosestPointIndex)) {
		  GroupStitchIndex = ClosestPointIndex + 1U;
		  --ClosestPointIndex;
		  while (thred::inrng(ClosestPointIndex)) {
			--ClosestPointIndex;
		  }
		  ++ClosestPointIndex;
		  while (thred::inrng(GroupStitchIndex)) {
			++GroupStitchIndex;
		  }
		  --GroupStitchIndex;
		  Instance->StateMap.set(StateFlag::GRPSEL);
		  thred::gotbox();
		  return true;
		}
	  }
	  if (!Instance->StateMap.test(StateFlag::INSRT)) {
		rngal();
	  }
	  return true;
	}

	if (stitchPoint.x < ZoomBoxOrigin.x) {
	  std::swap(ZoomBoxOrigin.x, stitchPoint.x);
	}
	if (stitchPoint.y < ZoomBoxOrigin.y) {
	  std::swap(ZoomBoxOrigin.y, stitchPoint.y);
	}
	auto newSize = F_POINT {stitchPoint.x - ZoomBoxOrigin.x, stitchPoint.y - ZoomBoxOrigin.y};
	stitchPoint = F_POINT {ZoomBoxOrigin.x + (newSize.x * HALF), ZoomBoxOrigin.y + (newSize.y * HALF)};
	auto const saveFactor = ZoomFactor;
	ZoomFactor            = thred::updateZoomFactor(newSize);
	if (ZoomFactor < thred::getZoomMin()) {
	  ZoomFactor = saveFactor;
	  thred::zumin();
	  return true;
	}
	ZoomRect = F_RECTANGLE {0.0F, newSize.y, newSize.x, 0.0F};
	thred::shft(stitchPoint);
	Instance->StateMap.reset(StateFlag::BZUMIN);
	Instance->StateMap.set(StateFlag::RESTCH);
	if (!Instance->StateMap.testAndSet(StateFlag::ZUMED)) {
	  thred::movStch();
	}
	return true;
  }
  retflag = false;
  return false;
}

auto mouse::handleMouseMove(std::vector<POINT>& stretchBoxLine,
                            float const         xyRatio,
                            float&              rotationAngle,
                            F_POINT const&      rotationCenter,
                            FRM_HEAD const&     textureForm) -> bool {
  if (Instance->StateMap.test(StateFlag::TXTMOV)) {
	texture::txtrmov(textureForm);
	return true;
  }
  thred::movchk();
  if (thred::inStitchWin()) {
	if (wrap::pressed(VK_SHIFT)) {
	  xt::mvshft();
	  return true;
	}
	while (true) {
	  if (wrap::pressed(VK_SHIFT) && thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(WinMsg.pt));
	  }
	  if (Instance->StateMap.test(StateFlag::PRFACT) || Instance->StateMap.test(StateFlag::FORMIN) ||
	      Instance->StateMap.test(StateFlag::POLIMOV) || FormDataSheet != nullptr) {
		wrap::setCursor(ArrowCursor);
		break;
	  }
	  if (!Instance->StateMap.test(StateFlag::INIT)) { // If we are not inserting a stitch
		if (UserFlagMap->test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(NeedleUpCursor);
		}
		break;
	  }
	  if (Instance->StateMap.test(StateFlag::INSRT)) { // If we are inserting a stitch
		updateCursor();
		break;
	  }
	  if (Instance->StateMap.test(StateFlag::BZUMIN) || Instance->StateMap.test(StateFlag::BOXZUM) ||
	      Instance->StateMap.test(StateFlag::SELPNT)) { // If we are zooming or selecting a point
		wrap::setCursor(CrossCursor);
		break;
	  }
	  if (Instance->StateMap.test(StateFlag::SATIN) || Instance->StateMap.test(StateFlag::SATPNT) ||
	      Instance->StateMap.test(StateFlag::INSFRM)) { // If we are inserting a satin form or a satin point
		if (UserFlagMap->test(UserFlag::FRMX)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(FormCursor);
		}
		break;
	  }
	  if (Instance->StateMap.test(StateFlag::SATCNKT)) { // If we are connecting a satin form guide
		wrap::setCursor(DLineCursor);
	  }
	  else {
		wrap::setCursor(ArrowCursor);
	  }
	  break;
	}
	if (Instance->StateMap.test(StateFlag::FPUNCLP)) { // If we are retrieve a clip into a form
	  tfc::fpUnClip();
	}
	if (Instance->StateMap.test(StateFlag::INSFIL)) { // If we are inserting a fill form
	  form::unfrm();
	  auto const point = POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
	  thred::insflin(point);
	  Instance->StateMap.set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::MOVFRMS)) { // If we are moving forms
	  thred::unstrtch(stretchBoxLine);
	  stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x =
	      WinMsg.pt.x - std::lround(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[1].x = stretchBoxLine[2].x = WinMsg.pt.x + std::lround(SelectedFormsSize.x) -
	                                              std::lround(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y =
	      WinMsg.pt.y - std::lround(FormMoveDelta.y) - StitchWindowOrigin.y;
	  stretchBoxLine[2].y = stretchBoxLine[3].y = WinMsg.pt.y + std::lround(SelectedFormsSize.y) -
	                                              std::lround(FormMoveDelta.y) - StitchWindowOrigin.y;
	  if (thred::isLine(stretchBoxLine)) {
		stretchBoxLine[0].x -= 1;
		stretchBoxLine[0].y -= 1;
		stretchBoxLine[1].x += 1;
		stretchBoxLine[1].y -= 1;
		stretchBoxLine[2].x += 1;
		stretchBoxLine[2].y += 1;
		stretchBoxLine[3].x -= 1;
		stretchBoxLine[3].y += 1;
		stretchBoxLine[4].x -= 1;
		stretchBoxLine[4].y -= 1;
	  }
	  Instance->StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::POLIMOV)) { // If we are placing a form
	  form::munfrm();
	  form::setmfrm(ClosestFormToCursor);
	  Instance->StateMap.set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::EXPAND)) { // If we are expanding a form
	  thred::unstrtch(stretchBoxLine);
	  auto newSize = F_POINT {gsl::narrow<float>(WinMsg.pt.x - StitchWindowOrigin.x),
	                          gsl::narrow<float>(WinMsg.pt.y - StitchWindowOrigin.y)};

	  auto       iSide = (SelectedFormControlVertex + 2U) % 4U;
	  auto const ratio = std::fabs((newSize.x - wrap::toFloat(stretchBoxLine[iSide].x)) /
	                               (newSize.y - wrap::toFloat(stretchBoxLine[iSide].y)));
	  if ((iSide & 1U) != 0U) { // If the side is horizontal
		if (ratio < xyRatio) {
		  newSize.x = (wrap::toFloat(stretchBoxLine[iSide].y) - newSize.y) * xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].x);
		}
		else {
		  newSize.y = (wrap::toFloat(stretchBoxLine[iSide].x) - newSize.x) / xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].y);
		}
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
	  }
	  else {
		if (ratio < xyRatio) {
		  newSize.x = (newSize.y - wrap::toFloat(stretchBoxLine[iSide].y)) * xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].x);
		}
		else {
		  newSize.y = (newSize.x - wrap::toFloat(stretchBoxLine[iSide].x)) / xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].y);
		}
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = nxtcrnr(iSide);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
	  }
	  stretchBoxLine[4] = stretchBoxLine[0];
	  Instance->StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::STRTCH)) { // If we are stretching a form horizontally or vertically
	  thred::unstrtch(stretchBoxLine);
	  auto const lineLength = (SelectedFormControlVertex & 1U) != 0U
	                              ? WinMsg.pt.x - StitchWindowOrigin.x
	                              : WinMsg.pt.y - StitchWindowOrigin.y;
	  auto const dst        = (SelectedFormControlVertex + 2U) % 4U;
	  auto const code       = nxtcrnr(dst);
	  for (auto iSide = 0U; iSide < 4; ++iSide) {
		if (iSide != dst && iSide != code) {
		  if ((SelectedFormControlVertex & 1U) != 0U) {
			stretchBoxLine[iSide].x = lineLength;
		  }
		  else {
			stretchBoxLine[iSide].y = lineLength;
		  }
		}
	  }
	  stretchBoxLine[4] = stretchBoxLine[0];
	  Instance->StateMap.set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::INSFRM)) { // If we are inserting points into a form
	  form::uninsf();
	  InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
	                   .y = WinMsg.pt.y - StitchWindowOrigin.y};
	  Instance->StateMap.set(StateFlag::SHOINSF);
	  form::duinsf();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::FUNCLP)) { // If we are pasting a form
	  form::unfrm();
	  form::setmfrm(ClosestFormToCursor);
	  Instance->StateMap.set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::SATCNKT)) { // If we are connecting a satin form guide
	  form::drwcon();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::SATPNT)) { // If we are placing a satin point
	  satin::drwsat();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::FRMOV)) { // If we are moving a form
	  form::munfrm();
	  form::setmfrm(ClosestFormToCursor);
	  Instance->StateMap.set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::FRMPMOV)) { // If we are moving a form point
	  unmov();
	  Instance->RubberBandLine.operator[](1) = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
	                                            .y = WinMsg.pt.y - StitchWindowOrigin.y};
	  Instance->StateMap.set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(WinMsg.pt));
	  }
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::MOVCNTR)) { // If we are moving the center of rotation
	  thred::unrot();
	  thred::ritrot(rotationAngle, thred::pxCor2stch(WinMsg.pt));
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::ROTCAPT)) { // If we are rotating
	  thred::unrotu();
	  thred::unrot();
	  if (auto const adjustedDelta = thred::getAdjustedDelta(); adjustedDelta.x != 0.0F) {
		rotationAngle = -atan2(adjustedDelta.y, adjustedDelta.x);
	  }
	  else {
		if (adjustedDelta.y > 0) {
		  rotationAngle = PI_FHALF;
		}
		else {
		  rotationAngle = -PI_FHALF;
		}
	  }
	  rotationAngle -= thred::getRotationHandleAngle();
	  thred::ritrot(rotationAngle, rotationCenter);
	  Instance->StateMap.set(StateFlag::ROTUSHO);
	  thred::durotu();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::SELPNT)) { // If we are selecting a point
	  if (Instance->StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  thred::unsel();
	  thred::rSelbox();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::CLPSHO)) { // If we are pasting a clip
	  thred::unclp();
	  thred::clpbox();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::CAPT)) { // If we are doing a screen capture
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(WinMsg.pt));
	  }
	  thred::unlin();
	  thred::updateMoveLine();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::INSRT)) { // If we are inserting a stitch
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(WinMsg.pt));
	  }
	  if (Instance->StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  if (Instance->StateMap.test(StateFlag::LIN1)) {
		if (!Instance->StitchBuffer.empty()) {
		  thred::xlin1();
		  InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
		                   .y = WinMsg.pt.y - StitchWindowOrigin.y};
		  Instance->StateMap.set(StateFlag::ILIN1);
		  thred::ilin1();
		}
		return true;
	  }
	  thred::xlin();
	  InsertLine[1] = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
	                   .y = WinMsg.pt.y - StitchWindowOrigin.y};
	  Instance->StateMap.set(StateFlag::ILIN);
	  thred::ilin();
	  return true;
	}
	if (Instance->StateMap.test(StateFlag::BOXZUM) && Instance->StateMap.testAndSet(StateFlag::VCAPT)) { // If we are zooming
	  SetCapture(ThrEdWindow);
	}
	if (Instance->StateMap.test(StateFlag::BZUMIN) && (WinMsg.wParam & MK_LBUTTON) != 0U) { // If we are zooming in
	  if (Instance->StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  thred::unbBox();
	  ZoomBoxLine[1].x = ZoomBoxLine[2].x = WinMsg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[2].y = ZoomBoxLine[3].y = WinMsg.pt.y - StitchWindowOrigin.y;
	  Instance->StateMap.set(StateFlag::BZUM);
	  thred::bBox();
	  return true;
	}
  }
  else {
	SetCursor(ArrowCursor);
	wrap::setCursor(ArrowCursor);
	if (Instance->StateMap.testAndReset(StateFlag::VCAPT)) {
	  ReleaseCapture();
	}
  }
  return true;
}

auto mouse::handleRightButtonDown() -> bool {
  if (Instance->StateMap.test(StateFlag::TXTRED) && MsgWindow == nullptr) {
	texture::txtrbut();
	return true;
  }
  auto const& formList = Instance->FormList;

  if (wrap::pressed(VK_SHIFT)) { // If shift is pressed as well
	if (form::closfrm(ClosestFormToCursor)) {
	  if ((Instance->StateMap.test(StateFlag::FRMPSEL) || Instance->StateMap.test(StateFlag::FPSEL)) &&
	      SelectedFormVertices.form == ClosestFormToCursor) { // and if we are selecting one of more form points
		// then we are expanding the selection
		auto const& vertexCount = formList.operator[](ClosestFormToCursor).vertexCount;
		Instance->StateMap.reset(StateFlag::FRMPSEL);
		Instance->StateMap.set(StateFlag::FPSEL);
		SelectedFormVertices.finish = ClosestVertexToCursor;
		if (auto const selectedVertexCount =
		        (SelectedFormVertices.finish - SelectedFormVertices.start + vertexCount) % vertexCount;
		    selectedVertexCount < vertexCount / 2U) {
		  SelectedFormVertices.vertexCount = selectedVertexCount;
		  Instance->StateMap.set(StateFlag::PSELDIR);
		}
		else {
		  SelectedFormVertices.vertexCount = vertexCount - selectedVertexCount;
		  Instance->StateMap.reset(StateFlag::PSELDIR);
		}
		thred::setpsel();
		return true;
	  }
	  if (!thred::unselectAll()) { // then we are selecting a form point
		thred::selfpnt();
	  }
	  return true;
	}
  }
  if (Instance->StateMap.test(StateFlag::WASTRAC)) {
	trace::wasTrace1();
	return true;
  }
  if (thred::unselectAll()) {
	return true;
  }
  if (thred::inStitchWin() && !(Instance->StateMap.test(StateFlag::SIZSEL) &&
                                thred::inChangeThreadWindows())) { // If we are in the thread windows
	if (!formList.empty() && !Instance->StateMap.test(StateFlag::FRMOF)) { // and we are not in a form
	  if ((WinMsg.wParam & MK_SHIFT) != 0U) {
		auto tempIndex = ClosestFormToCursor;
		if (form::closfrm(ClosestFormToCursor)) {
		  // ToDo - I don't think this can ever be hit with closfrm
		  if (!Instance->SelectedFormList.empty()) {
			thred::nuslst(ClosestFormToCursor);
			Instance->StateMap.set(StateFlag::RESTCH);
			return true;
		  }
		  if (Instance->StateMap.testAndReset(StateFlag::FORMSEL) && tempIndex != ClosestFormToCursor) {
			if (tempIndex > ClosestFormToCursor) {
			  std::swap(ClosestFormToCursor, tempIndex);
			}
			for (auto iForm = tempIndex; iForm <= ClosestFormToCursor; ++iForm) {
			  Instance->SelectedFormList.push_back(iForm);
			}
			Instance->StateMap.set(StateFlag::RESTCH);
			return true;
		  }

		  form::nufsel();
		}
	  }
	  if ((WinMsg.wParam & MK_CONTROL) != 0U) { // If control is pressed
		if (Instance->SelectedFormList.empty() && Instance->StateMap.test(StateFlag::FORMSEL)) {
		  Instance->StateMap.set(StateFlag::WASEL);
		  PreviousFormIndex = ClosestFormToCursor;
		}
		if (form::closfrm(ClosestFormToCursor)) {
		  form::nufsel();
		}
		if (Instance->SelectedFormList.size() > 1) {
		  std::wstring const blank;
		  displayText::butxt(HNUM, blank);
		}
		return true;
	  }
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) { // If we are already selecting a form
		thred::handleFormSelected();
	  }
	  if (form::closfrm(ClosestFormToCursor)) {
		Instance->StateMap.set(StateFlag::FORMSEL);
		Instance->StateMap.reset(StateFlag::FPSEL);
		form::unpsel();
		form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		if (Instance->StateMap.testAndReset(StateFlag::FRMPSEL) || !Instance->SelectedFormList.empty()) {
		  Instance->StateMap.set(StateFlag::RESTCH);
		  Instance->SelectedFormList.clear();
		}
		displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
		thred::lenCalc();
		thred::unrot();
		return true;
	  }
	  // unselect form, forms or form points
	  if (!Instance->SelectedFormList.empty()) {
		Instance->SelectedFormList.clear();
	  }
	  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
		Instance->StateMap.set(StateFlag::RESTCH);
	  }
	  if (Instance->StateMap.testAndReset(StateFlag::FRMPSEL)) {
		Instance->StateMap.set(StateFlag::RESTCH);
	  }
	}
	if (Instance->StateMap.test(StateFlag::INIT) || !Instance->ThrName.empty()) { // If we are inserting a stitch
	  if ((WinMsg.wParam & MK_SHIFT) != 0U) {
		if (Instance->StateMap.test(StateFlag::SELBOX)) { // if we are holding shift and selecting a box
		  // extend the thread selection
		  auto const code = ClosestPointIndex;
		  thred::closPnt1(ClosestPointIndex);
		  if (ClosestPointIndex != code) {
			thred::unbox();
			GroupStitchIndex  = ClosestPointIndex;
			ClosestPointIndex = code;
			Instance->StateMap.set(StateFlag::GRPSEL);
			thred::grpAdj();
		  }
		  thred::nuAct(GroupStitchIndex);
		  thred::redrawColorBar();
		  return true;
		}
		if (Instance->StateMap.test(StateFlag::GRPSEL)) {
		  auto const code = ClosestPointIndex;
		  thred::closPnt1(ClosestPointIndex);
		  GroupStitchIndex  = ClosestPointIndex;
		  ClosestPointIndex = code;
		  thred::grpAdj();
		  thred::nuAct(GroupStitchIndex);
		  thred::redrawColorBar();
		  return true;
		}
		thred::rebox();
	  }
	  else {
		Instance->StateMap.reset(StateFlag::LENSRCH);
		Instance->StateMap.reset(StateFlag::WASGRP);
		Instance->StateMap.reset(StateFlag::FPSEL);
		Instance->StateMap.reset(StateFlag::WASFRMFRM);
		Instance->StateMap.reset(StateFlag::SIDACT);
		if (Instance->StateMap.testAndReset(StateFlag::INSRT)) {
		  ReleaseCapture();
		  thred::rstAll();
		  Instance->StateMap.set(StateFlag::RESTCH);
		}
		else {
		  if (Instance->StateMap.testAndReset(StateFlag::GRPSEL)) {
			Instance->StateMap.set(StateFlag::RESTCH);
		  }
		  if (!Instance->StitchBuffer.empty()) {
			thred::rebox();
		  }
		}
	  }
	  xt::clrstch();
	  Instance->StateMap.set(StateFlag::RESTCH);
	}
	return true;
  }
  if (!Instance->ButtonWin.empty()) {
	auto minLenRect = RECT {};
	if (GetWindowRect(Instance->ButtonWin.operator[](HMINLEN), &minLenRect) != 0) {
	  if (WinMsg.pt.x >= minLenRect.left && WinMsg.pt.x <= minLenRect.right &&
	      WinMsg.pt.y > minLenRect.top && WinMsg.pt.y <= minLenRect.bottom) {
		thred::setSmallestStitchVal();
		Instance->StateMap.set(StateFlag::GRPSEL);
		Instance->StateMap.set(StateFlag::RESTCH);
		return true;
	  }
	}
	auto maxLenRect = RECT {};
	if (GetWindowRect(Instance->ButtonWin.operator[](HMAXLEN), &maxLenRect) != 0) {
	  if (WinMsg.pt.x >= maxLenRect.left && WinMsg.pt.x <= maxLenRect.right &&
	      WinMsg.pt.y > maxLenRect.top && WinMsg.pt.y <= maxLenRect.bottom) {
		thred::setLargestStitchVal();
		Instance->StateMap.set(StateFlag::GRPSEL);
		Instance->StateMap.set(StateFlag::RESTCH);
	  }
	}
  }
  return true;
}
