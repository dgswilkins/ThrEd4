// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "backup.h"
#include "balarad.h"
#include "bitmap.h"
#include "displayText.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "globals.h"
#include "mouse.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "trace.h"
#include "xt.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <commdlg.h>
#include <shellapi.h>

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

} // namespace

namespace mi {
auto chkok() noexcept -> bool;
auto finrng(uint32_t find) noexcept -> bool;

constexpr auto nxtcrnr(uint32_t corner) -> uint32_t;

void rngal();
void unmov();
} // namespace mi

auto mi::chkok() noexcept -> bool {
  return thred::chkwnd(OKButton);
}

auto mi::finrng(uint32_t find) noexcept -> bool {
  auto const& rectFind = FormList->operator[](find).rectangle;
  if (rectFind.left < StitchRangeRect.left || rectFind.right > StitchRangeRect.right ||
      rectFind.bottom < StitchRangeRect.bottom || rectFind.top > StitchRangeRect.top) {
	return false;
  }
  if (ActiveLayer == 0U) {
	return true;
  }
  auto const cod = gsl::narrow_cast<uint8_t>(
      gsl::narrow_cast<uint8_t>(FormList->operator[](find).attribute & FRMLMSK) >> 1U);
  return (cod == 0U) || (ActiveLayer == cod);
}

constexpr auto mi::nxtcrnr(uint32_t corner) -> uint32_t {
  return (corner + 1U) & 3U;
}

void mi::rngal() {
  if (StateMap->testAndReset(StateFlag::WASFPNT)) {
	return;
  }
  StateMap->reset(StateFlag::GRPSEL);
  auto prng = std::vector<RANGE> {};
  StateMap->reset(StateFlag::GRPSEL);
  auto iStitch     = 0U;
  auto flagInRange = false;
  for (; iStitch < wrap::toUnsigned(StitchBuffer->size()); ++iStitch) {
	if (thred::inrng(iStitch)) {
	  if (!flagInRange) {
		prng.push_back(RANGE {iStitch, 0U});
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
	auto const length = prng[index].finish - prng[index].start;
	if (length > maximumLength) {
	  maximumLength = length;
	  largestRange  = index;
	}
  }
  if (maximumLength != 0U) {
	ClosestPointIndex = prng[largestRange].start;
	GroupStitchIndex  = prng[largestRange].finish;
	StateMap->set(StateFlag::GRPSEL);
  }
  thred::gotbox();
}

void mi::unmov() {
  if (StateMap->testAndReset(StateFlag::SHOMOV)) {
	thred::ritmov(ClosestFormToCursor);
  }
}

auto mouse::getArrowCursor() noexcept -> HCURSOR {
  return ArrowCursor;
}

#pragma warning(push)
#pragma warning(disable : 26493) // Don't use C-style casts (type.4)
void mouse::crtcurs() noexcept {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  ArrowCursor = LoadCursor(nullptr, IDC_ARROW);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  CrossCursor = LoadCursor(nullptr, IDC_CROSS);
  // clang-format off
  FormCursor            = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Form));      // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  DLineCursor           = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_DLIN));      // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  NeedleUpCursor        = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_Upright));   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  NeedleRightDownCursor = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightDown)); // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  NeedleRightUpCursor   = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_RightUp));   // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  NeedleLeftDownCursor  = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftDown));  // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  NeedleLeftUpCursor    = LoadCursor(ThrEdInstance, MAKEINTRESOURCE(IDC_LeftUp));    // NOLINT(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  // clang-format on
}
#pragma warning(pop)

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
  if (StateMap->testAndReset(StateFlag::THUMON)) {
	if (mi::chkok()) {
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
	StateMap->set(StateFlag::BAKSHO);
	thred::unbsho();
	StateMap->reset(StateFlag::THUMSHO);
	thred::unmsg();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::BAKSHO)) {
	if (Msg.message == WM_RBUTTONDOWN) {
	  StateMap->set(StateFlag::RBUT);
	}
	else {
	  StateMap->reset(StateFlag::RBUT);
	}
	if (thred::displayBackups()) {
	  return true;
	}
  }
  if (thred::inColorbar()) {
	thred::unpat();
	auto const colorBarPosition = thred::getColorbarVertPosition();
	if (Msg.message == WM_RBUTTONDOWN) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if (((Msg.wParam & MK_SHIFT) != 0U) &&
	      (StateMap->test(StateFlag::SELBOX) || StateMap->test(StateFlag::GRPSEL))) {
		thred::unbox();
		GroupStitchIndex = wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(StitchBuffer->size()));
		StateMap->set(StateFlag::GRPSEL);
		thred::grpAdj();
		thred::nuAct(GroupStitchIndex);
		StateMap->set(StateFlag::RESTCH);
	  }
	  else {
		ClosestPointIndex = wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(StitchBuffer->size()));
		thred::nuAct(ClosestPointIndex);
		thred::movbox();
		if (StateMap->testAndReset(StateFlag::GRPSEL)) {
		  StateMap->reset(StateFlag::SCROS);
		  StateMap->reset(StateFlag::ECROS);
		  StateMap->set(StateFlag::RESTCH);
		}
	  }
	}
	else {
	  ClosestPointIndex = wrap::round<uint32_t>(colorBarPosition * wrap::toFloat(StitchBuffer->size()));
	  thred::nuAct(ClosestPointIndex);
	  thred::rstAll();
	  StateMap->set(StateFlag::SELBOX);
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
  if ((wrap::pressed(VK_SHIFT)) && thred::inStitchWin()) {
	xt::dushft();
	return true;
  }
  if (StateMap->test(StateFlag::TXTRED) && !StateMap->test(StateFlag::FORMIN)) {
	texture::txtlbut(textureForm);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FSETFCOL)) {
	thred::unsid();
	thred::unmsg();
	if (thred::inDefaultColorWindows()) {
	  xt::dufcol(thred::getVerticalIndex() + 1U);
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::FSETBCOL)) {
	thred::unsid();
	thred::unmsg();
	if (thred::inDefaultColorWindows()) {
	  xt::dubcol(thred::getVerticalIndex() + 1U);
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::FPUNCLP)) {
	thred::savdo();
	thred::fixpclp(ClosestFormToCursor);
	FormList->operator[](ClosestFormToCursor).outline();
	return true;
  }
  if (StateMap->test(StateFlag::FPSEL) && !StateMap->test(StateFlag::FUNCLP) &&
      !StateMap->test(StateFlag::ROTAT)) {
	*SelectedFormsLine = *SelectedPointsLine;
	SelectedFormsRect  = SelectedPixelsRect;
	if (thred::chkbig(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (StateMap->test(StateFlag::WASTRAC)) {
	trace::tracpar();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::HUPMSG)) {
	thred::updateHoopSize();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::INSFIL)) {
	thred::duinsfil();
	return true;
  }
  if (StateMap->test(StateFlag::BIGBOX) && thred::chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (StateMap->testAndReset(StateFlag::DELSFRMS)) {
	if (mi::chkok()) {
	  thred::savdo();
	  StateMap->reset(StateFlag::DELTO);
	}
	else {
	  auto windowRect = RECT {};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
	      Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom) {
		thred::savdo();
		StateMap->set(StateFlag::DELTO);
	  }
	}
	thred::delsfrms();
	thred::unmsg();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::MOVMSG)) {
	if (mi::chkok()) {
	  thred::savdo();
	  if (!SelectedFormList->empty()) {
		for (auto const selectedForm : (*SelectedFormList)) {
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
	if (StateMap->testAndReset(StateFlag::WASFRMFRM)) {
	  formForms::refrm();
	}
	return true;
  }
  if (StateMap->test(StateFlag::FUNSCLP)) {
	StateMap->reset(StateFlag::MOVFRMS);
	auto formsRect = F_RECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, formsRect);
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);

	FormMoveDelta = F_POINT {stitchPoint.x - wrap::midl(formsRect.right, formsRect.left),
	                         stitchPoint.y - wrap::midl(formsRect.top, formsRect.bottom)};
	auto const clipFormsCount = tfc::getClipFormCount();
	for (auto iForm = 0U; iForm < clipFormsCount; ++iForm) {
	  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - iForm - 1U);
	  // clang-format off
	  auto& form     = FormList->operator[](ClosestFormToCursor);
	  auto  itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		itVertex->x += FormMoveDelta.x;
		itVertex->y += FormMoveDelta.y;
		++itVertex;
	  }
	  form.outline();
	  form::refil(ClosestFormToCursor);
	}
	StateMap->reset(StateFlag::FUNSCLP);
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (!SelectedFormList->empty() && !StateMap->test(StateFlag::ROTAT) && thred::chkbig(stretchBoxLine, xyRatio)) {
	return true;
  }
  if (StateMap->test(StateFlag::SIDCOL) && thred::inDefaultColorWindows()) {
	return thred::updateFillColor();
  }
  if (StateMap->testAndReset(StateFlag::OSAV)) {
	if (mi::chkok()) {
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
  if (StateMap->testAndReset(StateFlag::FCLOS)) {
	if (mi::chkok()) {
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
  if (StateMap->testAndReset(StateFlag::SAVEX)) {
	if (mi::chkok()) {
	  thred::save();
	  thred::reldun();
	}
	if (thred::chkwnd(DiscardButton)) {
	  thred::reldun();
	}
	thred::unmsg();
	return true;
  }
  if (StateMap->test(StateFlag::PRFACT)) {
	return thred::updatePreferences();
  }
  if (!StateMap->test(StateFlag::ROTAT) && StateMap->test(StateFlag::GRPSEL)) {
	auto const* formControlPoints = thred::getFormControlPoints();
	auto const& controlPoint = *formControlPoints;
	if (thred::iselpnt()) {
	  for (auto iSide = 0U; iSide < wrap::toUnsigned(stretchBoxLine.size()); ++iSide) {
		stretchBoxLine[iSide] = controlPoint[wrap::toSize(iSide) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		StateMap->set(StateFlag::STRTCH);
	  }
	  else {
		StateMap->set(StateFlag::EXPAND);
		xyRatio = (StitchRangeRect.right - StitchRangeRect.left) /
		          (StitchRangeRect.top - StitchRangeRect.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	auto const relativePoint = POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	if (relativePoint.x >= controlPoint[0].x && relativePoint.x <= controlPoint[2].x &&
	    relativePoint.y >= controlPoint[0].y && relativePoint.y <= controlPoint[4].y) {
	  thred::duSelbox();
	  StateMap->set(StateFlag::SELPNT);
	  SetCapture(ThrEdWindow);
	  StateMap->set(StateFlag::VCAPT);
	  thred::rSelbox();
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::NEWBAK)) {
	if (mi::chkok()) {
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
  if (StateMap->testAndReset(StateFlag::PRGMSG)) {
	if (mi::chkok()) {
	  thred::deldir();
	  return true;
	}
  }
  if (StateMap->test(StateFlag::DELFRM)) {
	auto code = 0;
	if (mi::chkok()) {
	  thred::savdo();
	  StateMap->reset(StateFlag::DELTO);
	  code = 1;
	}
	else {
	  auto windowRect = RECT {};
	  GetWindowRect(DeleteStitchesDialog, &windowRect);
	  if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right &&
	      Msg.pt.y >= windowRect.top && Msg.pt.y <= windowRect.bottom) {
		thred::savdo();
		StateMap->set(StateFlag::DELTO);
		code = 1;
	  }
	}
	if (code != 0) {
	  thred::frmdel();
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	thred::unmsg();
	return true;
  }
  if (StateMap->test(StateFlag::FILMSG)) {
	if (mi::chkok()) {
	  thred::savdo();
	  form::unfil();
	  thred::coltab();
	  thred::unmsg();
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::SIZSEL)) {
	if (thred::inChangeThreadWindows()) {
	  thred::updateThreadSize(threadSizeSelected);
	  return true;
	}
	thred::destroyChangeThreadSizeWindows();
  }
  if (StateMap->testAndReset(StateFlag::POLIMOV)) {
	thred::savdo();
	form::setfrm();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FORMIN)) {
	auto windowRect = RECT {};
	GetWindowRect(MsgWindow, &windowRect);
	if (Msg.pt.x >= windowRect.left && Msg.pt.x <= windowRect.right && Msg.pt.y >= windowRect.top &&
	    Msg.pt.y <= windowRect.bottom) {
	  auto const iFillType = (Msg.pt.y - windowRect.top - 1) / (ButtonHeight - 4);
	  if (StateMap->testAndReset(StateFlag::FENDIN)) {
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
	SetWindowText(ValueWindow->operator[](PRFAPPCOL), fmt::format(FMT_COMPILE(L"{}"), AppliqueColor).c_str());
	thred::unsid();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::SIDACT)) {
	return thred::handleSideWindowActive();
  }
  if (FormDataSheet != nullptr) {
	return thred::handleFormDataSheet();
  }
  if (StateMap->test(StateFlag::INSFRM)) {
	thred::savdo();
	form::setins();
	return true;
  }
  if (StateMap->test(StateFlag::FUNCLP)) {
	StateMap->set(StateFlag::INIT);
	form::rstfrm();
	form::refil(ClosestFormToCursor);
	StateMap->reset(StateFlag::FUNCLP);
	if (StateMap->testAndReset(StateFlag::FPSEL)) {
	  FormList->back().outline();
	}
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->test(StateFlag::SATCNKT)) {
	thred::savdo();
	satin::satknkt();
	return true;
  }
  if (StateMap->test(StateFlag::SATPNT)) {
	satin::satpnt1();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::SATIN)) {
	satin::satpnt0();
	return true;
  }
  if (StateMap->test(StateFlag::FORMSEL)) {
	if (!StateMap->test(StateFlag::FRMROT) && thred::chkForm(stretchBoxLine, xyRatio)) {
	  return true;
	}
  }
  if (StateMap->testAndReset(StateFlag::MOVFRM)) {
	thred::savdo();
	form::setfrm();
	return true;
  }
  thred::unmsg();
  if (thred::inStitchWin() &&
      !(StateMap->test(StateFlag::SIZSEL) && thred::inChangeThreadWindows())) {
	thred::unpat();
	if (StateMap->testAndReset(StateFlag::ROTAT)) {
	  thred::setRotateCapture();
	  return true;
	}
	if (StateMap->testAndReset(StateFlag::CLPSHO)) {
	  thred::savdo();
	  if ((!StitchBuffer->empty()) &&
	      (StateMap->testAndReset(StateFlag::SELBOX) || StateMap->testAndReset(StateFlag::INSRT)) &&
	      ClosestPointIndex != wrap::toUnsigned(StitchBuffer->size() - 1U)) {
		tfc::lodclp(ClosestPointIndex);
	  }
	  else {
		tfc::lodclp(wrap::toUnsigned(StitchBuffer->size()));
	  }
	  thred::rngadj();
	  thred::setFormControls();
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	  return true;
	}
	if (StateMap->testAndReset(StateFlag::BOXZUM)) {
	  StateMap->set(StateFlag::BZUMIN);
	  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	  ZoomBoxOrigin                       = thred::pxCor2stch(Msg.pt);
	  StateMap->set(StateFlag::VCAPT);
	  return true;
	}
	if (!FormList->empty() && !StateMap->test(StateFlag::FRMOF)) {
	  if (!StateMap->test(StateFlag::INSRT)) {
		if (StateMap->testAndReset(StateFlag::FORMSEL)) {
		  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		}
		if (form::closfrm()) {
		  StateMap->set(StateFlag::FRMPMOV);
		  form::frmovlin();
		  return true;
		}
	  }
	}
	if (StateMap->test(StateFlag::INIT)) {
	  thred::unlin();
	  if (StateMap->test(StateFlag::INSRT)) {
		auto const stitchPoint = thred::pxCor2stch(Msg.pt);
		auto       code =
		    (ActiveColor | USMSK | gsl::narrow_cast<decltype(ActiveColor)>(ActiveLayer << LAYSHFT) | NOTFRM) & NKNOTMSK;
		if (StateMap->test(StateFlag::LIN1)) {
		  if (StateMap->test(StateFlag::BAKEND)) {
			thred::xlin1();
			auto const iStitch = wrap::toUnsigned(StitchBuffer->size());
			StitchBuffer->emplace_back(stitchPoint.x, stitchPoint.y, code);
			thred::duzrat();
			InsertLine[0] = thred::stch2px1(iStitch);
			InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
			return true;
		  }

		  thred::xlin1();
		  StitchBuffer->insert(StitchBuffer->begin(), F_POINT_ATTR {stitchPoint.x, stitchPoint.y, code});
		  StitchBuffer->front().attribute &= (~KNOTMSK);
		  InsertLine[0] = thred::stch2px1(0);
		  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		  return true;
		}
		thred::xlin();
		if (((StitchBuffer->operator[](ClosestPointIndex).attribute & ALTYPMSK) != 0U) &&
		    ((StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).attribute & ALTYPMSK) != 0U)) {
		  if ((StitchBuffer->operator[](ClosestPointIndex).attribute & FRMSK) ==
		      (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).attribute & FRMSK)) {
			code = StitchBuffer->operator[](ClosestPointIndex).attribute | USMSK;
		  }
		}
		++ClosestPointIndex;
		thred::savdo();
		StitchBuffer->insert(wrap::next(StitchBuffer->begin(), ClosestPointIndex),
		                     F_POINT_ATTR {stitchPoint.x, stitchPoint.y, code});
		thred::xlin();
		InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		InsertLine[0] = thred::stch2px1(ClosestPointIndex);
		InsertLine[2] = thred::stch2px1(ClosestPointIndex + 1U);
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
		displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
		return true;
	  }
	  auto closestPointIndexClone = uint32_t {};
	  if ((!StateMap->test(StateFlag::HIDSTCH)) && thred::closPnt1(closestPointIndexClone)) {
		thred::redrawCapturedStitch(closestPointIndexClone);
	  }
	}
	else {
	  if (thred::inStitchWin()) {
		thred::savdo();
		InsertLine[0]          = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		InsertLine[1]          = InsertLine[0];
		auto const stitchPoint = thred::pxCor2stch(Msg.pt);
		StitchBuffer->emplace_back(stitchPoint.x,
		                           stitchPoint.y,
		                           USMSK | ActiveColor |
		                               gsl::narrow_cast<uint32_t>(ActiveLayer << LAYSHFT) | NOTFRM);
		thred::addColor(0, ActiveColor);
		StateMap->set(StateFlag::LIN1);
		StateMap->set(StateFlag::INSRT);
		SetCapture(ThrEdWindow);
		ClosestPointIndex = 1;
		StateMap->set(StateFlag::INIT);
		StateMap->set(StateFlag::BAKEND);
	  }
	}
	thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	StateMap->set(StateFlag::BOXSLCT);
	StateMap->set(StateFlag::BZUMIN);
	StateMap->set(StateFlag::NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	ZoomBoxOrigin                       = thred::pxCor2stch(Msg.pt);
	StateMap->set(StateFlag::VCAPT);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HBOXSEL)) {
	form::boxsel();
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HUPTO)) {
	thred::toglup();
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HHID)) {
	thred::toglHid();
	return true;
  }
  if (thred::inDefaultColorWindows()) {
	if (Msg.message == WM_LBUTTONDOWN) {
	  thred::switchUserColors();
	  if (StateMap->test(StateFlag::HID)) {
		StateMap->reset(StateFlag::SELBOX);
		StateMap->reset(StateFlag::GRPSEL);
		StateMap->reset(StateFlag::SCROS);
		StateMap->reset(StateFlag::ECROS);
		StateMap->set(StateFlag::RESTCH);
		thred::redraw(ButtonWin->operator[](HHID));
	  }
	  else {
		if (!SelectedFormList->empty()) {
		  thred::savdo();
		  thred::nucols();
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		  return true;
		}
		if (StateMap->test(StateFlag::FORMSEL)) {
		  auto& form = FormList->operator[](ClosestFormToCursor);
		  if ((form.fillType != 0U) || (form.edgeType != 0U) ||
		      ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U)) {
			thred::savdo();
			if (form.fillType != 0U) {
			  form.fillColor = ActiveColor;
			  if (form.fillType == FTHF) {
				form.fillInfo.feather.color = ActiveColor;
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
			for (auto& stitch : *StitchBuffer) {
			  if (((stitch.attribute & ALTYPMSK) != 0U) && (stitch.attribute & FRMSK) == formCode &&
			      (stitch.attribute & TYPMSK) != TYPMSK) {
				stitch.attribute &= NCOLMSK;
				stitch.attribute |= ActiveColor;
			  }
			}
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
		else {
		  if (StateMap->test(StateFlag::GRPSEL)) {
			thred::savdo();
			for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
			  StitchBuffer->operator[](iStitch).attribute &= NCOLMSK;
			  StitchBuffer->operator[](iStitch).attribute |= ActiveColor;
			}
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		  else {
			if (StateMap->test(StateFlag::COL)) {
			  StateMap->set(StateFlag::RESTCH);
			}
		  }
		}
	  }
	}
	return true;
  }
  if (thred::inUserColorWindows()) {
	thred::updateUserColor();
	return true;
  }
  if (thred::inThreadSizeWindows()) {
	if (Msg.message == WM_LBUTTONDOWN) {
	  threadSizeSelected = thred::createChangeThreadSizeWindows();
	}
	return true;
  }
  retflag = false;
  return false;
}

auto mouse::handleLeftButtonUp(float xyRatio, float rotationAngle, F_POINT& rotationCenter, bool& retflag)
    -> bool {
  retflag = true;
  if ((wrap::pressed(VK_SHIFT)) && thred::inStitchWin()) {
	texture::setshft();
	return true;
  }
  if (StateMap->test(StateFlag::TXTRED)) {
	texture::txtrup();
	return true;
  }
  ReleaseCapture();
  thred::movchk();
  if (StateMap->testAndReset(StateFlag::MOVFRMS)) {
	thred::savdo();
	auto const point =
	    POINT {(Msg.pt.x - std::lround(FormMoveDelta.x) - StitchWindowOrigin.x) - SelectedFormsRect.left,
	           (Msg.pt.y - std::lround(FormMoveDelta.y) - StitchWindowOrigin.y) - SelectedFormsRect.top};
	form::ratsr();
	FormMoveDelta = F_POINT {wrap::toFloat(point.x) / HorizontalRatio, wrap::toFloat(point.y) / VerticalRatio};
	if (StateMap->test(StateFlag::FPSEL)) {
	  // clang-format off
	  auto&      form            = FormList->operator[](ClosestFormToCursor);
	  auto       iSelectedVertex = SelectedFormVertices.start;
	  auto const itVertex        = wrap::next(FormVertices->begin(), form.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const thisIt = wrap::next(itVertex, iSelectedVertex);
		thisIt->x += FormMoveDelta.x;
		thisIt->y -= FormMoveDelta.y;
		iSelectedVertex = form::pdir(form, iSelectedVertex);
	  }
	  thred::setpsel();
	  form.outline();
	  form::refil(ClosestFormToCursor);
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::BIGBOX)) {
		thred::savdo();
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		  form::frmadj(iForm);
		}
		for (auto& stitch : *StitchBuffer) {
		  stitch.x += FormMoveDelta.x;
		  stitch.y -= FormMoveDelta.y;
		}
		form::selal();
	  }
	  else {
		thred::savdo();
		for (auto const selectedForm : (*SelectedFormList)) {
		  form::frmadj(selectedForm);
		}
		form::frmsadj();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	return true;
  }
  if (StateMap->testAndReset(StateFlag::EXPAND)) {
	form::setexpand(xyRatio);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::STRTCH)) {
	form::setstrtch();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FRMOV)) {
	thred::savdo();
	form::rstfrm();
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::FRMPMOV)) {
	thred::savdo();
	form::setfpnt();
	return true;
  }
  if (StateMap->testAndReset(StateFlag::MOVCNTR)) {
	rotationCenter = thred::pxCor2stch(Msg.pt);
	StateMap->set(StateFlag::ROTAT);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::ROTCAPT)) {
	thred::rotfn(rotationAngle, rotationCenter);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::SELPNT)) {
	thred::savdo();
	ReleaseCapture();
	thred::unsel();
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);
	auto const adjustedPoint = thred::getAdjustedPoint(stitchPoint);
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  StitchBuffer->operator[](iStitch).x -= adjustedPoint.x;
	  StitchBuffer->operator[](iStitch).y -= adjustedPoint.y;
	}
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->testAndReset(StateFlag::CAPT)) {
	thred::unlin();
	ReleaseCapture();
	StateMap->reset(StateFlag::CAPT);
	thred::savdo();
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);

	StitchBuffer->operator[](ClosestPointIndex).x = stitchPoint.x;
	StitchBuffer->operator[](ClosestPointIndex).y = stitchPoint.y;
	StitchBuffer->operator[](ClosestPointIndex).attribute |= USMSK;
	if (ZoomFactor < STCHBOX) {
	  thred::drawCapturedStitchBox();
	}
	StateMap->set(StateFlag::SELBOX);
	StateMap->reset(StateFlag::FRMPSEL);
	StateMap->set(StateFlag::RESTCH);
	return true;
  }
  if (StateMap->test(StateFlag::BZUMIN)) {
	auto stitchPoint = thred::pxCor2stch(Msg.pt);
	if (StateMap->testAndReset(StateFlag::BOXSLCT)) {
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
	  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
		thred::rngadj();
		for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; ++iStitch) {
		  if (thred::inrng(iStitch)) {
			ClosestPointIndex = iStitch;
			StateMap->set(StateFlag::SELBOX);
			break;
		  }
		}
	  }
	  if (StateMap->testAndReset(StateFlag::NOSEL)) {
		SelectedFormList->clear();
		// We potentially reserve too much memory, but the cost of reallocatioon is higher than the
		// small amount overallocated
		SelectedFormList->reserve(FormList->size());
		StateMap->reset(StateFlag::FORMSEL);
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		  if (mi::finrng(iForm)) {
			SelectedFormList->push_back(iForm);
		  }
		}
		if (SelectedFormList->size() == 1) {
		  ReleaseCapture();
		  thred::gotbox();
		  ClosestFormToCursor   = SelectedFormList->front();
		  ClosestVertexToCursor = 0;
		  SelectedFormList->clear();
		  displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
		  StateMap->set(StateFlag::RESTCH);
		  StateMap->set(StateFlag::FORMSEL);
		  return true;
		}
		if (!SelectedFormList->empty()) {
		  thred::gotbox();
		  return true;
		}
		StateMap->reset(StateFlag::BZUMIN);
		StateMap->reset(StateFlag::BOXSLCT);
		StateMap->reset(StateFlag::FRMPSEL);
		StateMap->set(StateFlag::RESTCH);
	  }
	  if (StateMap->testAndReset(StateFlag::SELBOX)) {
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
		  StateMap->set(StateFlag::GRPSEL);
		  thred::gotbox();
		  return true;
		}
	  }
	  if (!StateMap->test(StateFlag::INSRT)) {
		mi::rngal();
	  }
	  //				gotbox();
	  return true;
	}

	if (stitchPoint.x < ZoomBoxOrigin.x) {
	  std::swap(ZoomBoxOrigin.x, stitchPoint.x);
	}
	if (stitchPoint.y < ZoomBoxOrigin.y) {
	  std::swap(ZoomBoxOrigin.y, stitchPoint.y);
	}
	auto newSize = F_POINT {stitchPoint.x - ZoomBoxOrigin.x, stitchPoint.y - ZoomBoxOrigin.y};
	stitchPoint  = F_POINT {ZoomBoxOrigin.x + newSize.x * 0.5F, ZoomBoxOrigin.y + newSize.y * 0.5F};
	auto const saveFactor = ZoomFactor;
	ZoomFactor = thred::updateZoomFactor(newSize);
	if (ZoomFactor < thred::getZoomMin()) {
	  ZoomFactor = saveFactor;
	  thred::zumin();
	  return true;
	}
	ZoomRect = F_RECTANGLE {0.0F, newSize.y, newSize.x, 0.0F};
	thred::shft(stitchPoint);
	StateMap->reset(StateFlag::BZUMIN);
	StateMap->set(StateFlag::RESTCH);
	if (!StateMap->testAndSet(StateFlag::ZUMED)) {
	  thred::movStch();
	}
	return true;
	//			StateMap->reset(StateFlag::BZUMIN);
  }
  retflag = false;
  return false;
}

auto mouse::handleMouseMove(std::vector<POINT>& stretchBoxLine,
                          float               xyRatio,
                          float&              rotationAngle,
                          F_POINT const&      rotationCenter,
                          FRM_HEAD const&     textureForm) -> bool {
  if (StateMap->test(StateFlag::TXTMOV)) {
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
	  if ((wrap::pressed(VK_SHIFT)) && thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  if ((StateMap->test(StateFlag::PRFACT) || StateMap->test(StateFlag::FORMIN) ||
	       StateMap->test(StateFlag::POLIMOV)) ||
	      (FormDataSheet != nullptr)) {
		wrap::setCursor(ArrowCursor);
		break;
	  }
	  if (!StateMap->test(StateFlag::INIT)) {
		if (UserFlagMap->test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(NeedleUpCursor);
		}
		break;
	  }
	  if (StateMap->test(StateFlag::INSRT)) {
		if (UserFlagMap->test(UserFlag::NEDOF)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  if (StateMap->test(StateFlag::LIN1)) {
			wrap::setCursor(NeedleUpCursor);
		  }
		  else {
			if (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).x >
			    StitchBuffer->operator[](ClosestPointIndex).x) {
			  if (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).y >
			      StitchBuffer->operator[](ClosestPointIndex).y) {
				wrap::setCursor(NeedleLeftUpCursor);
			  }
			  else {
				wrap::setCursor(NeedleLeftDownCursor);
			  }
			}
			else {
			  if (StitchBuffer->operator[](wrap::toSize(ClosestPointIndex) + 1U).y >
			      StitchBuffer->operator[](ClosestPointIndex).y) {
				wrap::setCursor(NeedleRightUpCursor);
			  }
			  else {
				wrap::setCursor(NeedleRightDownCursor);
			  }
			}
		  }
		}
		break;
	  }
	  if (StateMap->test(StateFlag::BZUMIN) || StateMap->test(StateFlag::BOXZUM) ||
	      StateMap->test(StateFlag::SELPNT)) {
		wrap::setCursor(CrossCursor);
		break;
	  }
	  if (StateMap->test(StateFlag::SATIN) || StateMap->test(StateFlag::SATPNT) ||
	      StateMap->test(StateFlag::INSFRM)) {
		if (UserFlagMap->test(UserFlag::FRMX)) {
		  wrap::setCursor(CrossCursor);
		}
		else {
		  wrap::setCursor(FormCursor);
		}
		break;
	  }
	  if (StateMap->test(StateFlag::SATCNKT)) {
		wrap::setCursor(DLineCursor);
	  }
	  else {
		wrap::setCursor(ArrowCursor);
	  }
	  break;
	}
	if (StateMap->test(StateFlag::FPUNCLP)) {
	  tfc::fpUnClip();
	}
	if (StateMap->test(StateFlag::INSFIL)) {
	  form::unfrm();
	  auto const point = POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  thred::insflin(point);
	  StateMap->set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::MOVFRMS)) {
	  thred::unstrtch(stretchBoxLine);
	  stretchBoxLine[0].x = stretchBoxLine[3].x = stretchBoxLine[4].x =
	      Msg.pt.x - std::lround(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[1].x = stretchBoxLine[2].x = Msg.pt.x + std::lround(SelectedFormsSize.x) -
	                                              std::lround(FormMoveDelta.x) - StitchWindowOrigin.x;
	  stretchBoxLine[0].y = stretchBoxLine[1].y = stretchBoxLine[4].y =
	      Msg.pt.y - std::lround(FormMoveDelta.y) - StitchWindowOrigin.y;
	  stretchBoxLine[2].y = stretchBoxLine[3].y = Msg.pt.y + std::lround(SelectedFormsSize.y) -
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
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap->test(StateFlag::POLIMOV)) {
	  form::munfrm();
	  form::setmfrm(ClosestFormToCursor);
	  StateMap->set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::EXPAND)) {
	  thred::unstrtch(stretchBoxLine);
	  auto newSize = F_POINT {gsl::narrow<float>(Msg.pt.x - StitchWindowOrigin.x),
	                          gsl::narrow<float>(Msg.pt.y - StitchWindowOrigin.y)};

	  auto       iSide = (SelectedFormControlVertex + 2U) % 4U;
	  auto const ratio = std::fabs((newSize.x - wrap::toFloat(stretchBoxLine[iSide].x)) /
	                               (newSize.y - wrap::toFloat(stretchBoxLine[iSide].y)));
	  if ((iSide & 1U) != 0U) {
		if (ratio < xyRatio) {
		  newSize.x = (wrap::toFloat(stretchBoxLine[iSide].y) - newSize.y) * xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].x);
		}
		else {
		  newSize.y = (wrap::toFloat(stretchBoxLine[iSide].x) - newSize.x) / xyRatio +
		              wrap::toFloat(stretchBoxLine[iSide].y);
		}
		iSide                   = mi::nxtcrnr(iSide);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = mi::nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = mi::nxtcrnr(iSide);
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
		iSide                   = mi::nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		iSide                   = mi::nxtcrnr(iSide);
		stretchBoxLine[iSide].x = std::lround(newSize.x);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
		iSide                   = mi::nxtcrnr(iSide);
		stretchBoxLine[iSide].y = std::lround(newSize.y);
	  }
	  stretchBoxLine[4] = stretchBoxLine[0];
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap->test(StateFlag::STRTCH)) {
	  thred::unstrtch(stretchBoxLine);
	  auto const lineLength = ((SelectedFormControlVertex & 1U) != 0U)
	                              ? Msg.pt.x - StitchWindowOrigin.x
	                              : Msg.pt.y - StitchWindowOrigin.y;
	  auto const dst        = (SelectedFormControlVertex + 2U) % 4U;
	  auto const code       = mi::nxtcrnr(dst);
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
	  StateMap->set(StateFlag::SHOSTRTCH);
	  thred::strtchbox(stretchBoxLine);
	  return true;
	}
	if (StateMap->test(StateFlag::INSFRM)) {
	  form::uninsf();
	  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  StateMap->set(StateFlag::SHOINSF);
	  form::duinsf();
	  return true;
	}
	if (StateMap->test(StateFlag::FUNCLP)) {
	  form::unfrm();
	  form::setmfrm(ClosestFormToCursor);
	  StateMap->set(StateFlag::SHOFRM);
	  form::dufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::SATCNKT)) {
	  form::drwcon();
	  return true;
	}
	if (StateMap->test(StateFlag::SATPNT)) {
	  satin::drwsat();
	  return true;
	}
	if (StateMap->test(StateFlag::FRMOV)) {
	  form::munfrm();
	  form::setmfrm(ClosestFormToCursor);
	  StateMap->set(StateFlag::SHOFRM);
	  form::mdufrm();
	  return true;
	}
	if (StateMap->test(StateFlag::FRMPMOV)) {
	  mi::unmov();
	  RubberBandLine->operator[](1) = {Msg.pt.x - StitchWindowOrigin.x,
	                                   Msg.pt.y - StitchWindowOrigin.y};
	  StateMap->set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  return true;
	}
	if (StateMap->test(StateFlag::MOVCNTR)) {
	  thred::unrot();
	  thred::ritrot(rotationAngle, thred::pxCor2stch(Msg.pt));
	  return true;
	}
	if (StateMap->test(StateFlag::ROTCAPT)) {
	  thred::unrotu();
	  thred::unrot();
	  auto const adjustedDelta = thred::getAdjustedDelta();
	  if (adjustedDelta.x != 0.0F) {
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
	  StateMap->set(StateFlag::ROTUSHO);
	  thred::durotu();
	  return true;
	}
	if (StateMap->test(StateFlag::SELPNT)) {
	  if (StateMap->testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  thred::unsel();
	  thred::rSelbox();
	  return true;
	}
	if (StateMap->test(StateFlag::CLPSHO)) {
	  thred::unclp();
	  thred::clpbox();
	  return true;
	}
	if (StateMap->test(StateFlag::CAPT)) {
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  thred::unlin();
	  thred::updateMoveLine();
	  return true;
	}
	if (StateMap->test(StateFlag::INSRT)) {
	  if (thred::inStitchWin()) {
		thred::ritfcor(thred::pxCor2stch(Msg.pt));
	  }
	  if (StateMap->testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  if (StateMap->test(StateFlag::LIN1)) {
		if (!StitchBuffer->empty()) {
		  thred::xlin1();
		  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		  StateMap->set(StateFlag::ILIN1);
		  thred::ilin1();
		}
		return true;
	  }
	  thred::xlin();
	  InsertLine[1] = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  StateMap->set(StateFlag::ILIN);
	  thred::ilin();
	  return true;
	}
	if (StateMap->test(StateFlag::BOXZUM) && StateMap->testAndSet(StateFlag::VCAPT)) {
	  SetCapture(ThrEdWindow);
	}
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	if (StateMap->test(StateFlag::BZUMIN) && ((Msg.wParam & MK_LBUTTON) != 0U)) {
	  if (StateMap->testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	  }
	  thred::unbBox();
	  ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
	  ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
	  StateMap->set(StateFlag::BZUM);
	  thred::bBox();
	  return true;
	}
  }
  else {
	SetCursor(ArrowCursor);
	wrap::setCursor(ArrowCursor);
	if (StateMap->testAndReset(StateFlag::VCAPT)) {
	  ReleaseCapture();
	}
  }
  return true;
}

auto mouse::handleRightButtonDown() -> bool {
  if (StateMap->test(StateFlag::TXTRED) && (MsgWindow == nullptr)) {
	texture::txtrbut();
	return true;
  }
  if (wrap::pressed(VK_SHIFT)) {
	if (form::closfrm()) {
	  if ((StateMap->test(StateFlag::FRMPSEL) || StateMap->test(StateFlag::FPSEL)) &&
	      SelectedFormVertices.form == ClosestFormToCursor) {
		auto const& vertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
		StateMap->reset(StateFlag::FRMPSEL);
		StateMap->set(StateFlag::FPSEL);
		SelectedFormVertices.finish = ClosestVertexToCursor;
		auto const selectedVertexCount =
		    (SelectedFormVertices.finish - SelectedFormVertices.start + vertexCount) % vertexCount;
		if (selectedVertexCount < vertexCount / 2U) {
		  SelectedFormVertices.vertexCount = selectedVertexCount;
		  StateMap->set(StateFlag::PSELDIR);
		}
		else {
		  SelectedFormVertices.vertexCount = vertexCount - selectedVertexCount;
		  StateMap->reset(StateFlag::PSELDIR);
		}
		thred::setpsel();
		return true;
	  }
	  if (!thred::unselectAll()) {
		thred::selfpnt();
	  }
	  return true;
	}
  }
  if (StateMap->test(StateFlag::WASTRAC)) {
	trace::wasTrace1();
	return true;
  }
  if (thred::unselectAll()) {
	return true;
  }
  if (thred::inStitchWin() &&
      !(StateMap->test(StateFlag::SIZSEL) && thred::inChangeThreadWindows())) {
	if (!FormList->empty() && !StateMap->test(StateFlag::FRMOF)) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((Msg.wParam & MK_SHIFT) != 0U) {
		auto tempIndex = ClosestFormToCursor;
		if (form::closfrm()) {
		  // ToDo - I don't think this can ever be hit with closfrm
		  if (!SelectedFormList->empty()) {
			thred::nuslst(ClosestFormToCursor);
			StateMap->set(StateFlag::RESTCH);
			return true;
		  }
		  if (StateMap->testAndReset(StateFlag::FORMSEL) && tempIndex != ClosestFormToCursor) {
			if (tempIndex > ClosestFormToCursor) {
			  std::swap(ClosestFormToCursor, tempIndex);
			}
			for (auto iForm = tempIndex; iForm <= ClosestFormToCursor; ++iForm) {
			  SelectedFormList->push_back(iForm);
			}
			StateMap->set(StateFlag::RESTCH);
			return true;
		  }

		  form::nufsel();
		}
	  }
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((Msg.wParam & MK_CONTROL) != 0U) {
		if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		  StateMap->set(StateFlag::WASEL);
		  PreviousFormIndex = ClosestFormToCursor;
		}
		if (form::closfrm()) {
		  form::nufsel();
		}
		if (SelectedFormList->size() > 1) {
		  std::wstring const blank;
		  displayText::butxt(HNUM, blank);
		}
		return true;
	  }
	  if (StateMap->test(StateFlag::FORMSEL)) {
		thred::handleFormSelected();
	  }
	  if (form::closfrm()) {
		StateMap->set(StateFlag::FORMSEL);
		StateMap->reset(StateFlag::FPSEL);
		form::unpsel();
		form::ritfrct(ClosestFormToCursor, StitchWindowDC);
		if (StateMap->testAndReset(StateFlag::FRMPSEL) || !SelectedFormList->empty()) {
		  StateMap->set(StateFlag::RESTCH);
		  SelectedFormList->clear();
		}
		displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
		thred::lenCalc();
		thred::unrot();
		return true;
	  }
	  if (!SelectedFormList->empty()) {
		SelectedFormList->clear();
		StateMap->set(StateFlag::RESTCH);
	  }
	  if (StateMap->testAndReset(StateFlag::FRMPSEL)) {
		StateMap->set(StateFlag::RESTCH);
	  }
	}
	if (StateMap->test(StateFlag::INIT) || (!ThrName->empty())) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if ((Msg.wParam & MK_SHIFT) != 0U) {
		if (StateMap->test(StateFlag::SELBOX)) {
		  auto const code = ClosestPointIndex;
		  thred::closPnt1(ClosestPointIndex);
		  if (ClosestPointIndex != code) {
			thred::unbox();
			GroupStitchIndex  = ClosestPointIndex;
			ClosestPointIndex = code;
			StateMap->set(StateFlag::GRPSEL);
			thred::grpAdj();
		  }
		  thred::nuAct(GroupStitchIndex);
		  thred::redrawColorBar();
		  return true;
		}
		if (StateMap->test(StateFlag::GRPSEL)) {
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
		StateMap->reset(StateFlag::LENSRCH);
		StateMap->reset(StateFlag::WASGRP);
		StateMap->reset(StateFlag::FPSEL);
		StateMap->reset(StateFlag::WASFRMFRM);
		StateMap->reset(StateFlag::SIDACT);
		if (StateMap->testAndReset(StateFlag::INSRT)) {
		  ReleaseCapture();
		  thred::rstAll();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
			StateMap->set(StateFlag::RESTCH);
		  }
		  if (!StitchBuffer->empty()) {
			thred::rebox();
		  }
		}
	  }
	  xt::clrstch();
	  StateMap->set(StateFlag::RESTCH);
	}
	return true;
  }
  if (!ButtonWin->empty()) {
	auto minLenRect = RECT {};
	if (GetWindowRect(ButtonWin->operator[](HMINLEN), &minLenRect) != 0) {
	  if (Msg.pt.x >= minLenRect.left && Msg.pt.x <= minLenRect.right &&
	      Msg.pt.y > minLenRect.top && Msg.pt.y <= minLenRect.bottom) {
		thred::setSmallestStitchVal();
		StateMap->set(StateFlag::GRPSEL);
		StateMap->set(StateFlag::RESTCH);
		return true;
	  }
	}
	auto maxLenRect = RECT {};
	if (GetWindowRect(ButtonWin->operator[](HMAXLEN), &maxLenRect) != 0) {
	  if (Msg.pt.x >= maxLenRect.left && Msg.pt.x <= maxLenRect.right &&
	      Msg.pt.y > maxLenRect.top && Msg.pt.y <= maxLenRect.bottom) {
		thred::setLargestStitchVal();
		StateMap->set(StateFlag::GRPSEL);
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
  return true;
}
