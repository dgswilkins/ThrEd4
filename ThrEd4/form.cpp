// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
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
#include <limits>

// Open Source headers
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
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "formForms.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"
#include "form.h"

namespace fi = form::internal;

constexpr auto BHWIDTH = 20.0F;

static auto FormForInsert  = static_cast<FRMHED*>(nullptr); // insert form vertex in this form
static auto FormVertexNext = uint32_t {0U}; // form vertex storage for form vertex insert
static auto FormVertexPrev = uint32_t {0U}; // form vertex storage for form vertex insert

void form::frmclr(FRMHED& destination) noexcept {
  auto const head = FRMHED {};
  destination     = head;
}

auto form::internal::fplComp(fPOINTLINE const& point1, fPOINTLINE const& point2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (point1.y < point2.y) {
	return true;
  }
  if (point2.y < point1.y) {
	return false;
  }
  if (point1.x < point2.x) {
	return true;
  }
  if (point2.x < point1.x) {
	return false;
  }
  return false;
}

void form::dusqr(FRMHED& form) {
  if (UserFlagMap->test(UserFlag::SQRFIL)) {
	form.extendedAttribute |= AT_SQR;
  }
  else {
	form.extendedAttribute &= ~(AT_SQR);
  }
}

auto form::chkmax(uint32_t arg0, uint32_t arg1) noexcept -> bool {
  if ((arg0 & MAXMSK) != 0U) {
	return true;
  }
  if ((arg1 & MAXMSK) != 0U) {
	return true;
  }
  if (((arg1 + arg0) & MAXMSK) != 0U) {
	return true;
  }
  return false;
}

void form::fltspac(uint32_t vertexOffset, uint32_t count) {
  auto const currentVertex = FormList->operator[](ClosestFormToCursor).vertexIndex + vertexOffset;
  auto const sourceStart   = wrap::next(FormVertices->cbegin(), currentVertex);
  auto const val           = fPOINT {};
  FormVertices->insert(sourceStart, count, val);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	form.vertexIndex += count;
  }
}

void form::delflt(uint32_t formIndex) {
  auto form = wrap::next(FormList->begin(), formIndex);
  if (form->vertexCount != 0U) {
	auto const vertexCount = form->vertexCount;
	auto const eraseStart  = wrap::next(FormVertices->cbegin(), form->vertexIndex);
	auto const eraseEnd    = wrap::next(eraseStart, form->vertexCount);
	FormVertices->erase(eraseStart, eraseEnd);
	for (++form; form < FormList->end(); ++form) {
	  form->vertexIndex -= vertexCount;
	}
  }
}

auto form::internal::chk2of() -> bool {
  if (!StateMap->test(StateFlag::SELBOX)) {
	return false;
  }
  if (UserFlagMap->test(UserFlag::FIL2OF)) {
	return false;
  }
  return true;
}

void form::internal::rotbak(float rotationAngle, fPOINT const& rotationCenter) noexcept {
  for (auto& iSequence : *OSequence) {
	thred::rotflt(iSequence, rotationAngle, rotationCenter);
  }
}

void form::delmfil(uint32_t formIndex) {
  if (texture::istx(formIndex)) {
	texture::deltx(formIndex);
  }
  clip::delmclp(ClosestFormToCursor);
  // find the first stitch to delete
  auto const codedForm = ClosestFormToCursor << FRMSHFT;
  auto const firstStitch =
      std::find_if(StitchBuffer->begin(), StitchBuffer->end(), [codedForm](fPOINTATTR const& m) -> bool {
	    return ((m.attribute & (FRMSK | NOTFRM)) == codedForm) && ((m.attribute & (TYPFRM | FTHMSK)) != 0U);
      });
  if (firstStitch != StitchBuffer->end()) {
	// we found the first stitch, so now delete the stitches in the form
	StitchBuffer->erase(std::remove_if(firstStitch,
	                                   StitchBuffer->end(),
	                                   [codedForm](fPOINTATTR const& m) -> bool {
	                                     return (m.attribute & FRMSK) == codedForm;
	                                   }),
	                    StitchBuffer->end());
  }
}

void form::fsizpar(FRMHED& form) noexcept {
  form.lengthOrCount.stitchLength = UserStitchLength;
  form.maxFillStitchLen           = IniFile.maxStitchLength;
  form.minFillStitchLen           = MinStitchLength;
}

void form::chkcont() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  fsizpar(form);
  if (form.fillType != CONTF) {
	if (form.satinGuideCount != 0U) {
	  auto shortestGuideIndex = 0U;
	  auto minimumLength      = MAXDWORD;
	  auto guideIt            = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		auto const length = guideIt->finish - guideIt->start;
		if (length < minimumLength) {
		  minimumLength      = length;
		  shortestGuideIndex = iGuide;
		}
		++guideIt;
	  }
	  auto const vShortest = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide + shortestGuideIndex);

	  form.angleOrClipData.guide = *vShortest;
	  satin::delsac(ClosestFormToCursor);
	}
	else {
	  form.angleOrClipData.guide.start  = 1;
	  form.angleOrClipData.guide.finish = form.vertexCount - 2U;
	}
  }
  form.attribute |= FRECONT;
  form.type     = FRMLINE;
  form.fillType = CONTF;
}

auto form::find1st() -> uint32_t {
  if (fi::chk2of()) {
	return ClosestPointIndex;
  }
  auto const attribute = ClosestFormToCursor << 4U;
  auto       iStitch   = 0U;
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & FRMSK) == attribute) {
	  if ((stitch.attribute & NOTFRM) == 0U) {
		break;
	  }
	}
	++iStitch;
  }
  return iStitch;
}

void form::ispcdclp() {
  StateMap->reset(StateFlag::WASPCDCLP);
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  StateMap->set(StateFlag::WASPCDCLP);
	}
  }
  CloseClipboard();
}

void form::frmout(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  if (form.vertexCount != 0U) {
	auto& rectangle = form.rectangle;
	auto  vertexIt  = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	rectangle       = fRECTANGLE {vertexIt->x, vertexIt->y, vertexIt->x, vertexIt->y};
	for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	  ++vertexIt;
	  if (vertexIt->x < rectangle.left) {
		rectangle.left = vertexIt->x;
	  }
	  if (vertexIt->y > rectangle.top) {
		rectangle.top = vertexIt->y;
	  }
	  if (vertexIt->x > rectangle.right) {
		rectangle.right = vertexIt->x;
	  }
	  if (vertexIt->y < rectangle.bottom) {
		rectangle.bottom = vertexIt->y;
	  }
	}
	if (rectangle.top - rectangle.bottom < MINRCT) {
	  auto const offset = (MINRCT - (rectangle.top - rectangle.bottom)) / 2;
	  rectangle.top += offset;
	  rectangle.bottom -= offset;
	}
	if (rectangle.right - rectangle.left < MINRCT) {
	  auto const offset = (MINRCT - (rectangle.right - rectangle.left)) / 2;
	  rectangle.left -= offset;
	  rectangle.right += offset;
	}
  }
}

auto form::sfCor2px(fPOINT const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(StitchWindowClientRect.bottom -
                                    (stitchPoint.y - ZoomRect.bottom) * ZoomRatio.y)};
}

auto form::internal::px2stchf(POINT const& screen) noexcept -> fPOINT {
  auto const factorX = wrap::toFloat(screen.x) / wrap::toFloat(StitchWindowClientRect.right);
  auto const factorY = wrap::toFloat(StitchWindowClientRect.bottom - screen.y) /
                       wrap::toFloat(StitchWindowClientRect.bottom);
  return fPOINT {factorX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left,
                 factorY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom};
}

void form::frmlin(FRMHED const& form) {
  if (form.vertexCount != 0U) {
	auto& formLines = *FormLines;
	formLines.clear();
	formLines.reserve(form.vertexCount);
	auto const vBegin = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	auto       vCurr  = vBegin;
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  formLines.push_back(POINT {
	      std::lround((vCurr->x - ZoomRect.left) * ZoomRatio.x),
	      std::lround(StitchWindowClientRect.bottom - (vCurr->y - ZoomRect.bottom) * ZoomRatio.y)});
	  ++vCurr;
	}
	formLines.push_back(POINT {
	    std::lround((vBegin->x - ZoomRect.left) * ZoomRatio.x),
	    std::lround(StitchWindowClientRect.bottom - (vBegin->y - ZoomRect.bottom) * ZoomRatio.y)});
  }
}

void form::frmlin(std::vector<fPOINT> const& vertices) {
  auto const vertexMax = vertices.size();

  if (vertexMax != 0U) {
	auto& formLines = *FormLines;
	formLines.clear();
	formLines.reserve(vertexMax);
	for (auto iVertex = 0U; iVertex < vertexMax; ++iVertex) {
	  formLines.push_back(POINT {std::lround((vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x),
	                             std::lround(StitchWindowClientRect.bottom -
	                                         (vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y)});
	}
	formLines.push_back(POINT {
	    std::lround((vertices[0].x - ZoomRect.left) * ZoomRatio.x),
	    std::lround(StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom) * ZoomRatio.y)});
  }
}

void form::dufrm() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::unfrm() {
  if (StateMap->testAndReset(StateFlag::SHOFRM)) {
	dufrm();
  }
}

void form::mdufrm() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  if (FormList->operator[](ClosestFormToCursor).type == FRMLINE) {
	wrap::Polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount - 1);
  }
  else {
	wrap::Polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount);
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void form::munfrm() {
  if (StateMap->testAndReset(StateFlag::SHOFRM)) {
	form::mdufrm();
  }
}

void form::internal::rats() {
  if (StateMap->test(StateFlag::ZUMED)) {
	HorizontalRatio = (ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right);
	VerticalRatio = (ZoomRect.top - ZoomRect.bottom) / wrap::toFloat(StitchWindowClientRect.bottom);
  }
  else {
	HorizontalRatio = wrap::toFloat(UnzoomedRect.x) / wrap::toFloat(StitchWindowClientRect.right);
	VerticalRatio   = wrap::toFloat(UnzoomedRect.y) / wrap::toFloat(StitchWindowClientRect.bottom);
  }
}

void form::setfrm() {
  if (!FormList->empty()) {
	fi::rats();
	ClosestFormToCursor  = wrap::toUnsigned(FormList->size() - 1U);
	auto&      form      = FormList->back();
	auto const point     = fi::px2stchf(FormLines->front());
	auto       vertexIt  = wrap::next(FormVertices->begin(), form.vertexIndex);
	auto const delta     = fPOINT {point.x - vertexIt->x, point.y - vertexIt->y};
	auto&      rectangle = form.rectangle;
	rectangle            = fRECTANGLE {BIGFLOAT, 0.0F, 0.0F, BIGFLOAT};
	for (auto iVertex = 0U; iVertex < NewFormVertexCount - 1U; ++iVertex) {
	  vertexIt->x += delta.x;
	  vertexIt->y += delta.y;
	  if (vertexIt->x < rectangle.left) {
		rectangle.left = vertexIt->x;
	  }
	  if (vertexIt->x > rectangle.right) {
		rectangle.right = vertexIt->x;
	  }
	  if (vertexIt->y > rectangle.top) {
		rectangle.top = vertexIt->y;
	  }
	  if (vertexIt->y < rectangle.bottom) {
		rectangle.bottom = vertexIt->y;
	  }
	  ++vertexIt;
	}
	StateMap->reset(StateFlag::FORMIN);
	StateMap->set(StateFlag::INIT);
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::form() {
  displayText::shoMsg(displayText::loadStr(IDS_FMEN));
  StateMap->set(StateFlag::FORMIN);
  StateMap->reset(StateFlag::INSRT);
  thred::duzrat();
}

void form::internal::frmsqr(uint32_t vertexIndex, uint32_t iVertex) {
  auto       line  = std::array<POINT, TRIPNTS> {};
  auto const vCurr = wrap::next(FormVertices->cbegin(), vertexIndex + iVertex);
  auto const vPrev = std::next(vCurr, -1);
  line[1]          = thred::stch2pxr(*vCurr);
  auto const ratio = wrap::toFloat(MulDiv(IniFile.formVertexSizePixels, *screenDPI, STDDPI)) /
                     wrap::toFloat(StitchWindowClientRect.right);
  auto       length = (ZoomRect.right - ZoomRect.left) * ratio * 2.0F;
  auto const delta  = fPOINT {vPrev->x - vCurr->x, vPrev->y - vCurr->y};
  auto       angle  = atan2(delta.y, delta.x);
  auto       offset = fPOINT {length * cos(angle), length * sin(angle)};
  auto const point  = fPOINT {vCurr->x + offset.x, vCurr->y + offset.y};
  angle             = atan2(-delta.x, delta.y);
  length /= 2.0F;
  offset             = fPOINT {length * cos(angle), length * sin(angle)};
  auto adjustedPoint = fPOINT {point.x + offset.x, point.y + offset.y};
  line[0] = line[3] = thred::stch2pxr(adjustedPoint);
  adjustedPoint     = fPOINT {point.x - offset.x, point.y - offset.y};
  line[2]           = thred::stch2pxr(adjustedPoint);
  wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
}

void form::selsqr(POINT const& controlPoint, HDC dc) {
  auto       line   = std::array<POINT, SQPNTS> {};
  auto const offset = MulDiv(gsl::narrow<int32_t>(IniFile.formVertexSizePixels), *screenDPI, STDDPI);
  line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
  line[0].y = line[1].y = controlPoint.y - offset;
  line[1].x = line[2].x = controlPoint.x + offset;
  line[2].y = line[3].y = controlPoint.y + offset;
  line[4].y             = controlPoint.y - offset;
  wrap::Polyline(dc, line.data(), wrap::toUnsigned(line.size()));
}

void form::internal::frmsqr0(POINT const& controlPoint) {
  auto       line   = std::array<POINT, SQPNTS> {};
  auto const offset = MulDiv(gsl::narrow<int32_t>(IniFile.formBoxSizePixels), *screenDPI, STDDPI);
  if (offset != 0) {
	line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
	line[0].y = line[1].y = controlPoint.y - offset;
	line[1].x = line[2].x = controlPoint.x + offset + 1;
	line[2].y = line[3].y = controlPoint.y + offset + 1;
	line[4].y             = controlPoint.y - 1;
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
}

void form::internal::frmx(POINT const& controlPoint, HDC dc) {
  auto       line   = std::array<POINT, LNPNTS> {};
  auto const offset = MulDiv(8, *screenDPI, STDDPI);
  SelectObject(dc, FormSelectedPen);
  line[0].x = line[1].x = controlPoint.x;
  line[0].y             = controlPoint.y + offset;
  line[1].y             = controlPoint.y - offset;
  wrap::Polyline(dc, line.data(), wrap::toUnsigned(line.size()));
  line[0].y = line[1].y = controlPoint.y;
  line[0].x             = controlPoint.x - offset;
  line[1].x             = controlPoint.x + offset;
  wrap::Polyline(dc, line.data(), wrap::toUnsigned(line.size()));
  SelectObject(dc, FormPen);
}

void form::ratsr() {
  if (StateMap->test(StateFlag::ZUMED)) {
	HorizontalRatio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	VerticalRatio = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
  }
  else {
	HorizontalRatio = wrap::toFloat(StitchWindowClientRect.right) / wrap::toFloat(UnzoomedRect.x);
	VerticalRatio   = wrap::toFloat(StitchWindowClientRect.bottom) / wrap::toFloat(UnzoomedRect.y);
  }
}

void form::ritfrct(uint32_t iForm, HDC dc) {
  auto pixelOutline = std::array<POINT, OUTPNTS> {};
  auto formOutline  = std::array<fPOINT, OUTPNTS> {};
  ratsr();
  SelectObject(StitchWindowDC, FormPen);
  SetROP2(StitchWindowDC, R2_XORPEN);
  auto const& rectangle = FormList->operator[](iForm).rectangle;
  SelectObject(dc, FormSelectedPen);
  formOutline[0].x = formOutline[6].x = formOutline[7].x = formOutline[8].x = rectangle.left;
  formOutline[0].y = formOutline[1].y = formOutline[2].y = formOutline[8].y = rectangle.top;
  formOutline[2].x = formOutline[3].x = formOutline[4].x = rectangle.right;
  formOutline[4].y = formOutline[5].y = formOutline[6].y = rectangle.bottom;
  formOutline[1].x = formOutline[5].x = wrap::midl(rectangle.right, rectangle.left);
  formOutline[3].y = formOutline[7].y = wrap::midl(rectangle.top, rectangle.bottom);

  auto ipixelOutline = pixelOutline.begin();
  for (auto controlPoint : formOutline) {
	*ipixelOutline = form::sfCor2px(controlPoint);
	++ipixelOutline;
  }
  wrap::Polyline(dc, pixelOutline.data(), wrap::toUnsigned(pixelOutline.size()));
  auto const sp = gsl::make_span(pixelOutline);
  // drawing the boxes on corners and sides, so don't overwrite the first box which will "erase" it
  auto subsp = sp.subspan(0, sp.size() - 1);
  for (auto const& controlPoint : subsp) {
	form::selsqr(controlPoint, dc);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	StateMap->reset(StateFlag::SELSHO);
	SearchLine->clear();
	SearchLine->shrink_to_fit();
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::delfrms() {
  thred::savdo();
  FormVertices->clear();
  ClipPoints->clear();
  FormList->clear();
  SatinGuides->clear();
  for (auto& stitch : *StitchBuffer) {
	stitch.attribute &= NFRM_NTYP;
	stitch.attribute |= NOTFRM;
  }
}

void form::fselrct(uint32_t iForm) {
  // clang-format off
  auto const& form = FormList->operator[](iForm);
  auto formOutline = std::array<fPOINT, SQPNTS> {};
  auto line        = std::array<POINT, SQPNTS> {};
  // clang-format on
  formOutline[0].x = formOutline[3].x = formOutline[4].x = form.rectangle.left;
  formOutline[0].y = formOutline[1].y = formOutline[4].y = form.rectangle.top;
  formOutline[1].x = formOutline[2].x = form.rectangle.right;
  formOutline[2].y = formOutline[3].y = form.rectangle.bottom;
  auto iFormOutline                   = formOutline.begin();
  for (auto& point : line) {
	point.x = std::lround((iFormOutline->x - ZoomRect.left) * HorizontalRatio);
	point.y = std::lround((ZoomRect.top - iFormOutline->y) * VerticalRatio);
	if (point.x < SelectedFormsRect.left) {
	  SelectedFormsRect.left = point.x;
	}
	if (point.y < SelectedFormsRect.top) {
	  SelectedFormsRect.top = point.y;
	}
	if (point.x > SelectedFormsRect.right) {
	  SelectedFormsRect.right = point.x;
	}
	if (point.y > SelectedFormsRect.bottom) {
	  SelectedFormsRect.bottom = point.y;
	}
	++iFormOutline;
  }
  auto const last = POINT {std::lround((formOutline[0].x - ZoomRect.left) * HorizontalRatio),
                           std::lround((ZoomRect.top - formOutline[0].y) * VerticalRatio)};
  if (last.x < SelectedFormsRect.left) {
	SelectedFormsRect.left = last.x;
  }
  if (last.y < SelectedFormsRect.top) {
	SelectedFormsRect.top = last.y;
  }
  if (last.x > SelectedFormsRect.right) {
	SelectedFormsRect.right = last.x;
  }
  if (last.y > SelectedFormsRect.bottom) {
	SelectedFormsRect.bottom = last.y;
  }
  if (OutLineEverySelectedForm) {
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
}

void form::rct2sel(RECT const& rectangle, std::vector<POINT>& line) noexcept {
  line[0].x = line[6].x = line[7].x = line[8].x = rectangle.left;
  line[0].y = line[1].y = line[2].y = line[8].y = rectangle.top;
  line[2].x = line[3].x = line[4].x = rectangle.right;
  line[4].y = line[5].y = line[6].y = rectangle.bottom;
  line[1].x = line[5].x = ((rectangle.right - rectangle.left) / 2) + rectangle.left;
  line[3].y = line[7].y = ((rectangle.bottom - rectangle.top) / 2) + rectangle.top;
}

void form::dubig() {
  form::rct2sel(SelectedFormsRect, *SelectedFormsLine);
  SelectObject(StitchWindowMemDC, SelectAllPen);
  wrap::Polyline(StitchWindowMemDC, SelectedFormsLine->data(), wrap::toUnsigned(SelectedFormsLine->size()));
  for (auto iPoint = 0U; iPoint < (SelectedFormsLine->size() - 1U); ++iPoint) {
	form::selsqr(SelectedFormsLine->operator[](iPoint), StitchWindowMemDC);
  }
}

void form::internal::frmpoly(POINT const* const line, uint32_t count) noexcept {
  if (line != nullptr) {
	if (count != 0U) {
	  for (auto iPoint = 0U; iPoint < count - 1U; ++iPoint) {
		wrap::Polyline(StitchWindowMemDC, &line[iPoint], LNPNTS);
	  }
	}
  }
}

void form::dupsel(HDC dc) {
  SelectObject(dc, FormPen);
  SetROP2(dc, R2_XORPEN);
  wrap::Polyline(dc, SelectedPointsLine->data(), wrap::toUnsigned(SelectedPointsLine->size()));
  // iPoint = SelectedFormVertices.start;
  for (auto iPoint = 0U; iPoint < (SelectedPointsLine->size() - 1U); ++iPoint) {
	form::selsqr(SelectedPointsLine->operator[](iPoint), dc);
  }
  fi::frmx(EndPointCross, dc);
}

void form::unpsel() {
  if (StateMap->testAndReset(StateFlag::SHOPSEL)) {
	form::dupsel(StitchWindowDC);
  }
}

auto form::sRct2px(fRECTANGLE const& stitchRect) -> RECT {
  return RECT {wrap::ceil<int32_t>((stitchRect.left - ZoomRect.left) * ZoomRatio.x),
               wrap::ceil<int32_t>((StitchWindowClientRect.bottom) -
                                   (stitchRect.top - ZoomRect.bottom) * ZoomRatio.y),
               wrap::ceil<int32_t>((stitchRect.right - ZoomRect.left) * ZoomRatio.x),
               wrap::ceil<int32_t>((StitchWindowClientRect.bottom) -
                                   (stitchRect.bottom - ZoomRect.bottom) * ZoomRatio.y)};
}

void form::drwfrm() {
  StateMap->reset(StateFlag::SHOMOV);
  StateMap->reset(StateFlag::SHOPSEL);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  ratsr();
  thred::duzrat();
  auto const maxForm = FormList->size();
  for (auto iForm = 0U; iForm < maxForm; ++iForm) {
	auto const& form = FormList->operator[](iForm);
	form::frmlin(form);
	if (!FormLines->empty()) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  auto const layer = gsl::narrow_cast<uint8_t>((form.attribute & FRMLMSK) >> 1U);
	  if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
		auto line      = std::array<POINT, 2> {};
		auto lastPoint = 0U;
		if (form.type == SAT) {
		  if ((form.attribute & FRMEND) != 0U) {
			SelectObject(StitchWindowMemDC, FormPen3px);
			wrap::Polyline(StitchWindowMemDC, FormLines->data(), LNPNTS);
			lastPoint = 1;
		  }
		  if (form.wordParam != 0U) {
			SelectObject(StitchWindowMemDC, FormPen);
			fi::frmpoly(&(FormLines->operator[](1)), form.wordParam);
			SelectObject(StitchWindowMemDC, FormPen3px);
			wrap::Polyline(StitchWindowMemDC, &(FormLines->operator[](form.wordParam)), LNPNTS);
			SelectObject(StitchWindowMemDC, thred::getLayerPen(layer));
			lastPoint = form.wordParam + 1U;
		  }
		  auto const maxGuide = FormList->operator[](iForm).satinGuideCount;
		  auto const vBegin   = wrap::next(FormVertices->cbegin(), form.vertexIndex);
		  auto       guideIt  = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
		  for (auto iGuide = 0U; iGuide < maxGuide; ++iGuide) {
			auto const vStart  = wrap::next(vBegin, guideIt->start);
			auto const vFinish = wrap::next(vBegin, guideIt->finish);
			line[0]            = form::sfCor2px(*vStart);
			line[1]            = form::sfCor2px(*vFinish);
			SelectObject(StitchWindowMemDC, FormPen);
			wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
			++guideIt;
		  }
		}
		SelectObject(StitchWindowMemDC, thred::getLayerPen(layer));
		if (form.type == FRMLINE) {
		  fi::frmpoly(FormLines->data(), form.vertexCount);
		  if (form.fillType == CONTF) {
			auto const vBegin  = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto const vStart  = wrap::next(vBegin, form.angleOrClipData.guide.start);
			auto const vFinish = wrap::next(vBegin, form.angleOrClipData.guide.finish);
			thred::sCor2px(*vStart, line[0]);
			thred::sCor2px(*vFinish, line[1]);
			wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
		  }
		}
		else {
		  fi::frmpoly(&(FormLines->operator[](lastPoint)), form.vertexCount + 1 - lastPoint);
		}
		if (ClosestFormToCursor == iForm && StateMap->test(StateFlag::FRMPSEL)) {
		  auto& formLines = *FormLines;
		  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
			if (iVertex == ClosestVertexToCursor) {
			  fi::frmx(formLines[iVertex], StitchWindowMemDC);
			}
			else {
			  fi::frmsqr(form.vertexIndex, iVertex);
			}
		  }
		  if (ClosestVertexToCursor != 0U) {
			fi::frmsqr0(formLines[0]);
		  }
		  else {
			fi::frmx(formLines[0], StitchWindowMemDC);
		  }
		  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
		}
		else {
		  auto& formLines = *FormLines;
		  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
			fi::frmsqr(form.vertexIndex, iVertex);
		  }
		  SelectObject(StitchWindowMemDC, FormSelectedPen);
		  fi::frmsqr0(formLines[0]);
		}
		if (StateMap->test(StateFlag::FPSEL) && ClosestFormToCursor == iForm) {
		  SelectedPixelsRect = form::sRct2px(SelectedVerticesRect);
		  form::rct2sel(SelectedPixelsRect, *SelectedPointsLine);
		  StateMap->set(StateFlag::SHOPSEL);
		  form::dupsel(StitchWindowMemDC);
		}
	  }
	}
  }
  if (!SelectedFormList->empty()) {
	SelectObject(StitchWindowMemDC, MultiFormPen);
	ratsr();
	SelectedFormsRect.top = SelectedFormsRect.left =
	    std::numeric_limits<decltype(SelectedFormsRect.top)>::max();
	SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
	for (auto selectedForm : (*SelectedFormList)) {
	  fselrct(selectedForm);
	}
	wrap::narrow_cast(SelectedFormsSize.x, SelectedFormsRect.right - SelectedFormsRect.left);
	wrap::narrow_cast(SelectedFormsSize.y, SelectedFormsRect.bottom - SelectedFormsRect.top);
	dubig();
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form::ritfrct(ClosestFormToCursor, StitchWindowMemDC);
	}
	if (StateMap->test(StateFlag::FRMPMOV)) {
	  thred::ritmov(ClosestFormToCursor);
	  RubberBandLine->operator[](1).x = Msg.pt.x - StitchWindowOrigin.x;
	  RubberBandLine->operator[](1).y = Msg.pt.y - StitchWindowOrigin.y;
	  StateMap->set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	}
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void form::internal::setpoli() {
  auto form           = FRMHED {};
  form.type           = FRMFPOLY;
  form.attribute      = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  form.underlayIndent = IniFile.underlayIndent;
  FormList->push_back(form);
  StateMap->set(StateFlag::SATIN);
  StateMap->set(StateFlag::INIT);
}

void form::internal::setlin() {
  auto form      = FRMHED {};
  form.type      = FRMLINE;
  form.attribute = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  FormList->push_back(form);
  StateMap->set(StateFlag::SATIN);
  StateMap->set(StateFlag::INIT);
}

void form::internal::setrpoli() {
  thred::unmsg();
  displayText::pntmsg(IDS_REGP);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRPOL);
  displayText::numWnd();
}

void form::internal::setstar() {
  thred::unmsg();
  displayText::pntmsg(IDS_STAR);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRSTAR);
  displayText::numWnd();
}

void form::internal::setspir() {
  thred::unmsg();
  displayText::pntmsg(IDS_SPIR);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRSPIR);
  displayText::numWnd();
}

void form::internal::sethart() {
  thred::unmsg();
  displayText::pntmsg(IDS_HEART);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRHART);
  displayText::numWnd();
}

void form::internal::setlens() {
  thred::unmsg();
  displayText::pntmsg(IDS_LENS);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRLENS);
  displayText::numWnd();
}

void form::internal::seteg() {
  thred::unmsg();
  displayText::pntmsg(IDS_EGG);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTREG);
  displayText::numWnd();
}

void form::internal::setzig() {
  thred::unmsg();
  displayText::pntmsg(IDS_ZIG);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRZIG);
  displayText::numWnd();
}

void form::setmfrm() {
  // clang-format off
  auto const& closeForm = FormList->operator[](ClosestFormToCursor);
  auto        vertexIt  = wrap::next(FormVertices->cbegin(), closeForm.vertexIndex);
  auto        point     = form::sfCor2px(vertexIt[0]);
  auto const  offset    =
      POINT {Msg.pt.x - StitchWindowOrigin.x - point.x + std::lround(FormMoveDelta.x),
             Msg.pt.y - StitchWindowOrigin.y - point.y + std::lround(FormMoveDelta.y)};
  auto&       formLines = *FormLines;
  // clang-format on
  formLines.resize(wrap::toSize(closeForm.vertexCount) + 1U);
  for (auto iForm = 0U; iForm < closeForm.vertexCount; ++iForm) {
	point              = form::sfCor2px(*vertexIt);
	formLines[iForm].x = point.x + offset.x;
	formLines[iForm].y = point.y + offset.y;
	++vertexIt;
  }
  formLines[closeForm.vertexCount] = formLines[0];
}

void form::durpoli(uint32_t vertexCount) {
  if (vertexCount < 3) {
	vertexCount = 3;
  }
  auto const stepAngle = PI_F2 / wrap::toFloat(vertexCount);
  // 500 gives us a reasonably sized default
  auto const length = 500.0F / wrap::toFloat(vertexCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
  auto newForm        = FRMHED {};
  newForm.vertexIndex = thred::adflt(vertexCount);
  newForm.vertexCount = vertexCount;
  newForm.attribute   = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  newForm.type        = FRMFPOLY;
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  auto point          = thred::pxCor2stch(Msg.pt);
  auto angle          = 0.0F;
  auto vertexIt       = wrap::next(FormVertices->begin(), newForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	*vertexIt = point;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle += stepAngle;
	++vertexIt;
  }
  form::frmout(ClosestFormToCursor);
  FormMoveDelta      = fPOINT {};
  NewFormVertexCount = vertexCount + 1U;
  StateMap->set(StateFlag::POLIMOV);
  setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  mdufrm();
}

auto form::nxt(FRMHED const& form, uint32_t iVertex) noexcept -> uint32_t {
  ++iVertex;
  if (iVertex > form.vertexCount - 1U) {
	iVertex = 0;
  }
  return iVertex;
}

auto form::prv(FRMHED const& form, uint32_t iVertex) noexcept -> uint32_t {
  if (iVertex != 0U) {
	--iVertex;
  }
  else {
	iVertex = form.vertexCount - 1U;
  }
  return iVertex;
}

auto form::pdir(FRMHED const& form, uint32_t vertex) -> uint32_t {
  if (StateMap->test(StateFlag::PSELDIR)) {
	return form::nxt(form, vertex);
  }

  return form::prv(form, vertex);
}

void form::pxrct2stch(RECT const& screenRect, fRECTANGLE& stitchRect) noexcept {
  auto corner = POINT {screenRect.left + StitchWindowOrigin.x, screenRect.top + StitchWindowOrigin.y};
  auto stitchPoint  = thred::pxCor2stch(corner);
  stitchRect.left   = stitchPoint.x;
  stitchRect.top    = stitchPoint.y;
  corner.x          = screenRect.right + StitchWindowOrigin.x;
  corner.y          = screenRect.bottom + StitchWindowOrigin.y;
  stitchPoint       = thred::pxCor2stch(corner);
  stitchRect.right  = stitchPoint.x;
  stitchRect.bottom = stitchPoint.y;
}

void form::flipv() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	auto const offset        = SelectedVerticesRect.top + SelectedVerticesRect.bottom;
	auto       currentVertex = SelectedFormVertices.start;
	auto const vBegin        = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const vertexIt = wrap::next(vBegin, currentVertex);
	  vertexIt->y         = offset - vertexIt->y;
	  currentVertex       = pdir(form, currentVertex);
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	auto const offset = AllItemsRect->top + AllItemsRect->bottom;
	for (auto& formVertice : *FormVertices) {
	  formVertice.y = offset - formVertice.y;
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.y = offset - stitch.y;
	}
	for (auto& iForm : *FormList) {
	  iForm.rectangle.bottom = offset - iForm.rectangle.bottom;
	  iForm.rectangle.top    = offset - iForm.rectangle.top;
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (!SelectedFormList->empty()) {
	auto formMap   = boost::dynamic_bitset<> {FormList->size()};
	auto rectangle = fRECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, rectangle);
	auto const offset = rectangle.top + rectangle.bottom;
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto const& iForm   = FormList->operator[](ClosestFormToCursor);
	  formMap.set(ClosestFormToCursor);
	  auto vertexIt = wrap::next(FormVertices->begin(), iForm.vertexIndex);
	  for (auto iVertex = 0U; iVertex < iForm.vertexCount; ++iVertex) {
		vertexIt->y = offset - vertexIt->y;
		++vertexIt;
	  }
	  form::frmout(ClosestFormToCursor);
	}
	for (auto& stitch : *StitchBuffer) {
	  auto const decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	  if (formMap.test(decodedForm) && ((stitch.attribute & NOTFRM) == 0U)) {
		stitch.y = offset - stitch.y;
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto const offset   = form.rectangle.top + form.rectangle.bottom;
	  auto       vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		vertexIt->y = offset - vertexIt->y;
		++vertexIt;
	  }
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor &&
		    ((stitch.attribute & NOTFRM) == 0U)) {
		  stitch.y = offset - stitch.y;
		}
	  }
	  form::frmout(ClosestFormToCursor);
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto rectangle = fRECTANGLE {};
		thred::selRct(rectangle);
		auto const offset = rectangle.top + rectangle.bottom;
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).y = offset - StitchBuffer->operator[](iStitch).y;
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::duform(int32_t formType) {
  switch (formType + 1) {
	case FRMLINE:
	  thred::savdo();
	  fi::setlin();
	  break;
	case FRMFPOLY:
	  thred::savdo();
	  fi::setpoli();
	  break;
	case FRMRPOLY:
	  fi::setrpoli();
	  break;
	case FRMSTAR:
	  fi::setstar();
	  break;
	case FRMSPIRAL:
	  fi::setspir();
	  break;
	case FRMHEART - 1:
	  fi::sethart();
	  break;
	case FRMLENS - 1:
	  fi::setlens();
	  break;
	case FRMEGG - 1:
	  fi::seteg();
	  break;
	case FRMTEAR - 1:
	  formForms::setear();
	  break;
	case FRMZIGZAG - 1:
	  fi::setzig();
	  break;
	case FRMWAVE - 1:
	  formForms::wavfrm();
	  break;
	case FRMDAISY - 1:
	  formForms::dasyfrm();
	  break;
	default:
	  // NOLINTNEXTLINE (clang-diagnostic-sign-conversion) ToDo - why is this neccessary?
	  outDebugString(L"default hit in duform: formType [{}]\n", formType);
	  break;
  }
}

auto form::internal::findDistanceToSide(fPOINT const& lineStart,
                                        fPOINT const& lineEnd,
                                        fPOINT const& point,
                                        float&        distance) noexcept -> float {
  auto const A = point.x - lineStart.x;
  auto const B = point.y - lineStart.y;
  auto const C = lineEnd.x - lineStart.x;
  auto const D = lineEnd.y - lineStart.y;
  if ((C == 0.0F) && (D == 0.0F)) {
	distance = sqrt(A * B);
	// Arbitrarily choose the first point since start and end are the same
	return -0.1F;
  }
  auto const dot    = A * C + B * D;
  auto const len_sq = C * C + D * D;
  auto const param  = dot / len_sq;
  // param < 0 = before the first point
  // param > 1 = after last point
  // 0 < param < 1 = between endpoints
  auto const diff = (param < 0) ? fPOINT {point.x - lineStart.x, point.y - lineStart.y}
                                : (param > 1) ? fPOINT {point.x - lineEnd.x, point.y - lineEnd.y}
                                              : fPOINT {point.x - (lineStart.x + param * C),
                                                        point.y - (lineStart.y + param * D)};
  // returning shortest distance
  distance = sqrt(diff.x * diff.x + diff.y * diff.y);
  return param;
}

auto form::closfrm() -> bool {
  if (!FormList->empty()) {
	auto const screenCoordinate =
	    POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	fi::rats();
	auto       closestForm   = 0U;
	auto       closestVertex = 0U;
	auto       minimumLength = BIGFLOAT;
	auto const point         = fi::px2stchf(screenCoordinate);
	auto const layerCoded    = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
	auto const maxForm       = wrap::toUnsigned(FormList->size());
	for (auto iForm = 0U; iForm < maxForm; ++iForm) {
	  if (StateMap->test(StateFlag::FRMSAM) && iForm == ClosestFormToCursor) {
		continue;
	  }
	  auto& currentForm    = FormList->operator[](iForm);
	  auto const formLayer = gsl::narrow_cast<uint8_t>(currentForm.attribute & FRMLMSK);
	  if ((ActiveLayer == 0U) || (formLayer == 0U) || formLayer == layerCoded) {
		auto const vBegin   = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex);
		auto       vertexIt = vBegin;
		// find the closest line first and then find the closest vertex on that line
		auto       length    = 0.0F;
		auto const sideCount = currentForm.vertexCount;
		for (auto iVertex = 0U; iVertex < sideCount; ++iVertex) {
		  auto const vNext = wrap::next(vBegin, form::nxt(currentForm, iVertex));
		  auto const param = fi::findDistanceToSide(*vertexIt, *vNext, point, length);
		  if ((length < minimumLength) && (length >= 0.0F)) {
			minimumLength = length;
			closestForm   = iForm;
			if (param < 0.5F) {
			  closestVertex = iVertex;
			}
			else {
			  closestVertex = form::nxt(currentForm, iVertex);
			}
		  }
		  ++vertexIt;
		}
	  }
	}

	auto&             vertex =
	    FormVertices->operator[](wrap::toSize(FormList->operator[](closestForm).vertexIndex) + closestVertex);
	auto const stitchCoordsInPixels = thred::stch2pxr(vertex);
	minimumLength = wrap::toFloat(hypot(stitchCoordsInPixels.x - screenCoordinate.x,
	                                    stitchCoordsInPixels.y - screenCoordinate.y));
	if (minimumLength < FCLOSNUF) {
	  ClosestFormToCursor   = closestForm;
	  ClosestVertexToCursor = closestVertex;
	  StateMap->set(StateFlag::RELAYR);
	  return true;
	}
	return false;
  }
  return false;
}

void form::frmovlin() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  ratsr();
  if (form.type == FRMLINE) {
	NewFormVertexCount = form.vertexCount;
  }
  else {
	NewFormVertexCount = form.vertexCount + 1U;
  }
  form::frmlin(form);
  auto        previousPoint = form::prv(form, ClosestVertexToCursor);
  auto const& formLines     = *FormLines;
  for (auto iPoint = 0U; iPoint < 3U; ++iPoint) {
	RubberBandLine->operator[](iPoint) = formLines[previousPoint];
	previousPoint                      = form::nxt(form, previousPoint);
  }
  thred::ritmov(ClosestFormToCursor);
}

auto form::internal::ritlin(fPOINT const& start, fPOINT const& finish, float userStitchLen) -> bool {
  auto const delta  = fPOINT {finish.x - start.x, finish.y - start.y};
  auto       length = hypot(delta.x, delta.y);
  // This clamp is temporary to avoid overflow when BH corner value is too large. Find a better fix
  constexpr auto CLAMP = 200.0F;
  if (length > CLAMP) {
	length = CLAMP;
  }

  InterleaveSequence->push_back(start);
  if (length > MaxStitchLen) {
	constexpr auto MINSTLEN = 1e-1F; // clamp minimum stitch length
	if (userStitchLen < MINSTLEN) {
	  userStitchLen = MINSTLEN;
	}
	auto count = wrap::ceil<uint32_t>(length / userStitchLen);
	if (count == 0U) {
	  count = 1;
	}
	auto newLength = length / wrap::toFloat(count);
	while ((newLength > MaxStitchLen) && (newLength > MINSTLEN)) {
	  ++count;
	  newLength = length / wrap::toFloat(count);
	}
	auto const step  = fPOINT {delta.x / wrap::toFloat(count), delta.y / wrap::toFloat(count)};
	auto       point = fPOINT {start.x + step.x, start.y + step.y};
	for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	  InterleaveSequence->push_back(point);
	  point.x += step.x;
	  point.y += step.y;
	}
  }
  return true;
}

auto form::closflt(FRMHED const& form, float xCoordinate, float yCoordinate) -> uint32_t {
  auto closestVertex = 0U;
  auto minimumLength = BIGFLOAT;
  auto vertexIt      = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const length = hypot(xCoordinate - vertexIt->x, yCoordinate - vertexIt->y);
	if (length < minimumLength) {
	  closestVertex = iVertex;
	  minimumLength = length;
	}
	++vertexIt;
  }
  return closestVertex;
}

void form::chkseq(bool border) {
#if BUGBAK
  UNREFERENCED_PARAMETER(border);

  for (auto val : *OSequence) {
	InterleaveSequence->push_back(val);
  }
#else
  auto const savedIndex = InterleaveSequence->size();
  auto& form            = FormList->operator[](ClosestFormToCursor);

  auto userStitchLen =
      border ? (form.edgeType == EDGELCHAIN || form.edgeType == EDGEOCHAIN) ? MAXSIZ * PFGRAN : form.edgeStitchLen
             : (clip::isclp(form)) ? MaxStitchLen : form.lengthOrCount.stitchLength;
  auto const minimumStitchLength = border ? form.minBorderStitchLen : form.minFillStitchLen;
  if (border) {
	if (form.maxBorderStitchLen == 0.0F) {
	  form.maxBorderStitchLen = IniFile.maxStitchLength;
	}
  }
  else {
	if (form.maxFillStitchLen == 0.0F) {
	  form.maxFillStitchLen = IniFile.maxStitchLength;
	}
  }
  MaxStitchLen = border ? form.maxBorderStitchLen : form.maxFillStitchLen;
  if (userStitchLen > MaxStitchLen) {
	userStitchLen = MaxStitchLen;
  }
  if (!OSequence->empty()) {
	bool flag = true;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size()) - 1U; ++iSequence) {
	  if (!fi::ritlin(OSequence->operator[](iSequence), OSequence->operator[](wrap::toSize(iSequence) + 1U), userStitchLen)) {
		flag = false;
		break;
	  }
	}
	if (flag) {
	  InterleaveSequence->push_back(OSequence->back());
	}
  }
  else {
	outDebugString(L"chkseq: OSequence empty! border [{}]\n", border);
  }
  if (minimumStitchLength == 0.0F) {
	return;
  }
  auto destination = wrap::toUnsigned(savedIndex + 1U);
  for (auto iSequence = savedIndex + 1U; iSequence < InterleaveSequence->size(); ++iSequence) {
	// clang-format off
	auto const seq      = InterleaveSequence->operator[](iSequence);
	auto const seqBack1 = InterleaveSequence->operator[](iSequence - 1U);
	auto const len      = hypot(seq.x - seqBack1.x, seq.y - seqBack1.y);
	// clang-format on
	if (len > minimumStitchLength) {
	  InterleaveSequence->operator[](destination) = seq;
	  ++destination;
	}
  }
  InterleaveSequence->resize(destination);
#endif
}

void form::internal::ritbrd(FRMHED const& form) {
  if (!OSequence->empty()) {
	InterleaveSequenceIndices->emplace_back(INSREC {TYPBRD,
	                                                gsl::narrow_cast<uint32_t>(form.borderColor) & COLMSK,
	                                                wrap::toUnsigned(InterleaveSequence->size()),
	                                                I_BRD});
	form::chkseq(true);
  }
}

void form::internal::ritapbrd() {
  if (!OSequence->empty()) {
	auto const& form = FormList->operator[](ClosestFormToCursor);
	InterleaveSequenceIndices->emplace_back(INSREC {TYPMSK,
	                                                gsl::narrow_cast<uint32_t>(form.borderColor) >> 4U,
	                                                wrap::toUnsigned(InterleaveSequence->size()),
	                                                I_AP});
	form::chkseq(true);
  }
}

void form::internal::ritfil() {
  if (!OSequence->empty()) {
	auto const& form = FormList->operator[](ClosestFormToCursor);
	InterleaveSequenceIndices->emplace_back(INSREC {
	    TYPFRM, gsl::narrow_cast<uint32_t>(form.fillColor), wrap::toUnsigned(InterleaveSequence->size()), I_FIL});
	form::chkseq(false);
  }
}

auto form::lastch() noexcept -> bool {
  if (!InterleaveSequence->empty()) {
	LastPoint = InterleaveSequence->back();
	return true;
  }
  { return false; }
}

auto form::getlast(FRMHED const& form) -> uint32_t {
  if (form.fillType != 0U) {
	form::lastch();
	auto minimumLength = BIGFLOAT;
	auto closestVertex = 0U;
	auto vertexIt      = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  auto const dx     = LastPoint.x - vertexIt->x;
	  auto const dy     = LastPoint.y - vertexIt->y;
	  auto const length = hypot(dx, dy);
	  if (length < minimumLength) {
		minimumLength = length;
		closestVertex = iVertex;
	  }
	  ++vertexIt;
	}
	return closestVertex;
  }
  { return 0; }
}

void form::filinsb(fPOINT const& point, fPOINT& stitchPoint) {
  constexpr auto MAXSTCH = 54.0F; // maximum permitted stitch length for pfaf in pfaf "stitch pixels"
  auto const delta  = fPOINT {(point.x - stitchPoint.x), (point.y - stitchPoint.y)};
  auto const length = hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>(length / MAXSTCH + 1.0F);
  auto const step   = fPOINT {(delta.x / wrap::toFloat(count)), (delta.y / wrap::toFloat(count))};
  if (length > MAXSTCH) {
	--count;
	if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
	  return;
	}
	while (count != 0U) {
	  stitchPoint.x += step.x;
	  stitchPoint.y += step.y;
	  OSequence->push_back(stitchPoint);
	  --count;
	}
  }
  OSequence->push_back(point);
  stitchPoint.x = point.x;
  stitchPoint.y = point.y;
}

auto form::getblen() noexcept -> float {
  // clang-format off
  auto&      form    = FormList->operator[](ClosestFormToCursor);
  auto const iLength = (form.clipEntries << WRDSHFT) | form.picoLength;
  // clang-format on
  return wrap::toFloat(iLength);
}

void form::savblen(float fLength) {
  auto const iLength = wrap::round<uint32_t>(fLength);
  auto& form         = FormList->operator[](ClosestFormToCursor);
  form.clipEntries   = iLength >> WRDSHFT;
  form.picoLength    = iLength & WRDMASK;
}

auto form::getplen() noexcept -> float {
  // clang-format off
  auto const& form  = FormList->operator[](ClosestFormToCursor);
  auto const  value = form.picoLength;
  // clang-format on
  return (wrap::toFloat(value >> BYTSHFT) + wrap::toFloat(value & BYTMASK) / FRACFACT);
}

void form::savplen(float length) {
  auto integerPart = 0.0F;

  constexpr auto PCLAMP = 255.0F;
  if (length > PCLAMP) {
	length = PCLAMP;
  }
  auto const fractionalPart = std::modf(length, &integerPart);
  auto const fr             = wrap::floor<uint16_t>(fractionalPart * FRACFACT);
  auto const num            = gsl::narrow<uint32_t>(integerPart);
  FormList-> operator[](ClosestFormToCursor).picoLength = ((num << BYTSHFT) & B2MASK) | fr;
}

void form::internal::bdrlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float stitchSize) {
  auto const vStart      = wrap::next(FormVertices->cbegin(), vertexIndex + start);
  auto const vFinish     = wrap::next(FormVertices->cbegin(), vertexIndex + finish);
  auto const delta       = fPOINT {(vFinish->x - vStart->x), (vFinish->y - vStart->y)};
  auto const length      = hypot(delta.x, delta.y);
  auto       stitchCount = (UserFlagMap->test(UserFlag::LINSPAC))
                         ? wrap::ceil<uint32_t>(length / stitchSize)
                         : wrap::round<uint32_t>((length - stitchSize / 2.0F) / stitchSize + 1.0F);
  auto step = fPOINT {};
  if (UserFlagMap->test(UserFlag::LINSPAC)) {
	if (stitchCount != 0U) {
	  step.x = delta.x / wrap::toFloat(stitchCount);
	  step.y = delta.y / wrap::toFloat(stitchCount);
	}
  }
  else {
	// ToDo - Is this calculation correct?
	auto const angle = atan2(delta.y, delta.x);
	step.x           = cos(angle) * stitchSize;
	step.y           = sin(angle) * stitchSize;
  }
  if (stitchCount != 0U) {
	auto point = fPOINT {vStart->x + step.x, vStart->y + step.y};
	--stitchCount;
	while (stitchCount != 0U) {
	  OSequence->push_back(point);
	  point.x += step.x;
	  point.y += step.y;
	  --stitchCount;
	}
  }
  OSequence->push_back(*vFinish);
}

void form::internal::brdfil(FRMHED const& form) {
  auto currentVertex = ((form.extendedAttribute & AT_STRT) != 0U) ? form.fillStart : form::getlast(form);
  OSequence->clear();
  auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex + currentVertex);
  OSequence->push_back(*vertexIt);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, form.edgeStitchLen);
	currentVertex = nextVertex;
  }
  if (form.type != FRMLINE) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, form.edgeStitchLen);
  }
}

void form::internal::boldlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float size) {
  auto const vStart = wrap::next(FormVertices->cbegin(), vertexIndex + start);
  auto const vEnd   = wrap::next(FormVertices->cbegin(), vertexIndex + finish);
  auto const delta  = fPOINT {(vEnd->x - vStart->x), (vEnd->y - vStart->y)};
  auto const length = hypot(delta.x, delta.y);

  constexpr auto ESCLAMP = 1e-1F; // edge stitch minimum length clamp
  if (size < ESCLAMP) {
	size = ESCLAMP;
  }
  auto count = wrap::round<uint32_t>(length / size);
  if (count != 0U) {
	auto const step   = fPOINT {delta.x / wrap::toFloat(count), delta.y / wrap::toFloat(count)};
	auto       point0 = *vStart;
	auto       point1 = fPOINT {point0.x + step.x, point0.y + step.y};
	while (count != 0U) {
	  OSequence->push_back(point1);
	  OSequence->push_back(point0);
	  OSequence->push_back(point1);
	  point0.x += step.x;
	  point0.y += step.y;
	  point1.x += step.x;
	  point1.y += step.y;
	  --count;
	}
	OSequence->push_back(*vEnd);
  }
  else {
	OSequence->push_back(*vEnd);
	OSequence->push_back(*vStart);
	OSequence->push_back(*vEnd);
  }
}

void form::internal::bold(FRMHED const& form) {
  auto iOutput   = 0U;
  auto iNextLine = form::getlast(form);
  auto iLine     = iNextLine;
  OSequence->clear();
  auto const vLine = wrap::next(FormVertices->cbegin(), form.vertexIndex + iLine);
  OSequence->push_back(*vLine);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	iNextLine = form::nxt(form, iLine);
	boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
	iLine = iNextLine;
  }
  if (form.type != FRMLINE) {
	iNextLine = form::nxt(form, iLine);
	boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
  }
  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size() - 1U); ++iSequence) {
	auto const sequence     = OSequence->operator[](iSequence);
	auto const sequenceFwd1 = OSequence->operator[](wrap::toSize(iSequence) + 1U);
	auto const length       = hypot(sequenceFwd1.x - sequence.x, sequenceFwd1.y - sequence.y);
	if (length > TNYFLOAT) {
	  OSequence->operator[](iOutput++) = sequence;
	}
  }
  auto const vNext = wrap::next(FormVertices->cbegin(), form.vertexIndex + iNextLine);
  OSequence->operator[](iOutput++) = *vNext;
  OSequence->resize(iOutput);
}

void form::duangs(FRMHED const& form) {
  FormAngles->clear();
  auto vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
	vertexIt = wrap::next(AngledFormVertices->cbegin(), form.vertexIndex);
  }
  auto const vMax = gsl::narrow<ptrdiff_t>(form.vertexCount - 1U);
  for (auto iVertex = ptrdiff_t {0}; iVertex < vMax; ++iVertex) {
	auto const& thisVertex = vertexIt[iVertex];
	auto const& nextVertex = vertexIt[iVertex + 1];
	FormAngles->push_back(atan2(nextVertex.y - thisVertex.y, nextVertex.x - thisVertex.x));
  }
  FormAngles->push_back(atan2(vertexIt[0].y - vertexIt[vMax].y, vertexIt[0].x - vertexIt[vMax].x));
}

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
auto form::internal::projv(float xCoordinate, fPOINT const& lowerPoint, fPOINT const& upperPoint, fPOINT& intersection) noexcept
    -> bool {
  auto const deltaX = upperPoint.x - lowerPoint.x;
  intersection.x    = xCoordinate;
  if (deltaX != 0.0F) {
	auto const slope = (upperPoint.y - lowerPoint.y) / deltaX;
	intersection.y   = (xCoordinate - lowerPoint.x) * slope + lowerPoint.y;
	auto lower       = lowerPoint.x;
	auto upper       = upperPoint.x;
	if (lower > upper) {
	  std::swap(lower, upper);
	}
	return !(xCoordinate < lower || xCoordinate > upper);
  }
  return false;
}

auto form::cisin(FRMHED const& form, float xCoordinate, float yCoordinate) -> bool {
  auto const& rectangle = form.rectangle;
  if (xCoordinate >= rectangle.right) {
	return false;
  }
  if (xCoordinate <= rectangle.left) {
	return false;
  }
  if (yCoordinate >= rectangle.top) {
	return false;
  }
  if (yCoordinate <= rectangle.bottom) {
	return false;
  }
  auto count        = 0U;
  auto intersection = fPOINT {};
  auto vertexIt     = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
	vertexIt = wrap::next(AngledFormVertices->cbegin(), form.vertexIndex);
  }
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const vThis = wrap::next(vertexIt, iVertex);
	auto const vNext = wrap::next(vertexIt, nxt(form, iVertex));
	if (fi::projv(xCoordinate, *vThis, *vNext, intersection)) {
	  if (intersection.y >= yCoordinate) {
		if (vThis->x != xCoordinate && vNext->x != xCoordinate) {
		  ++count;
		}
		else {
		  if (vThis->x < vNext->x) {
			if (vNext->x != xCoordinate) {
			  ++count;
			}
		  }
		  else {
			if (vThis->x != xCoordinate) {
			  ++count;
			}
		  }
		}
	  }
	}
  }
  return (count & 1U) != 0U;
}

/* find the intersection of two lines, one defined by point and slope, the other by the coordinates
   of the endpoints. */
auto form::internal::proj(fPOINT const& point, float slope, fPOINT const& point0, fPOINT const& point1, fPOINT& intersectionPoint) noexcept
    -> bool {
  auto const delta = fPOINT {point1.x - point0.x, point1.y - point0.y};
  if (delta.x != 0.0F) {
	auto const sideSlope     = delta.y / delta.x;
	auto const sideConstant  = point0.y - sideSlope * point0.x;
	auto const pointConstant = point.y - slope * point.x;
	intersectionPoint.x      = (sideConstant - pointConstant) / (slope - sideSlope);
	intersectionPoint.y      = intersectionPoint.x * slope + pointConstant;
  }
  else {
	intersectionPoint.x      = point0.x;
	auto const pointConstant = point.y - slope * point.x;
	intersectionPoint.y      = intersectionPoint.x * slope + pointConstant;
  }
  auto xMinimum = point0.x;
  auto xMaximum = point1.x;
  if (xMinimum > xMaximum) {
	std::swap(xMinimum, xMaximum);
  }
  if (delta.y != 0.0F) {
	auto yMinimum = point0.y;
	auto yMaximum = point1.y;
	if (yMinimum > yMaximum) {
	  std::swap(yMinimum, yMaximum);
	}
	return !(intersectionPoint.x <= xMinimum || intersectionPoint.x > xMaximum ||
	         intersectionPoint.y < yMinimum || intersectionPoint.y > yMaximum);
  }

  return !(intersectionPoint.x <= xMinimum || intersectionPoint.x > xMaximum);
}

auto form::linx(std::vector<fPOINT> const& points, uint32_t start, uint32_t finish, fPOINT& intersection) noexcept
    -> bool {
  if (OutsidePoints != nullptr) {
	auto const delta = fPOINT {(OutsidePoints->operator[](start).x - points[start].x),
	                           (OutsidePoints->operator[](start).y - points[start].y)};
	auto const point = points[start];

	if ((delta.x == 0.0F) && (delta.y == 0.0F)) {
	  return false;
	}
	if (delta.x != 0.0F) {
	  return fi::proj(point, delta.y / delta.x, OutsidePoints->operator[](finish), points[finish], intersection);
	}

	return fi::projv(point.x, points[finish], OutsidePoints->operator[](finish), intersection);
  }

  return false;
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
auto form::internal::projh(float yCoordinate, fPOINT const& point0, fPOINT const& point1, fPOINT& intersection) noexcept
    -> bool {
  auto const deltaX = point1.x - point0.x;
  intersection.y    = yCoordinate;
  if (deltaX != 0.0F) {
	auto const deltaY = point1.y - point0.y;
	if (deltaY != 0.0F) {
	  auto const slope = deltaY / deltaX;
	  intersection.x   = (yCoordinate - point0.y) / slope + point0.x;
	}
	else {
	  return false;
	}
  }
  else {
	intersection.x = point0.x;
  }
  auto leftY  = point0.y;
  auto rightY = point1.y;
  if (leftY > rightY) {
	std::swap(leftY, rightY);
  }
  return !(yCoordinate < leftY || yCoordinate > rightY);
}

void form::internal::sprct(std::vector<fPOINT> const* vertices,
                           uint32_t                   vertexIndex,
                           std::vector<VRCT2>&        fillVerticalRect,
                           uint32_t                   start,
                           uint32_t                   finish) {
  auto const& opStart     = OutsidePoints->operator[](start);
  auto const& opFinish    = OutsidePoints->operator[](finish);
  auto const& ipStart     = InsidePoints->operator[](start);
  auto const& ipFinish    = InsidePoints->operator[](finish);
  auto const delta        = fPOINT {(opFinish.x - opStart.x), (opFinish.y - opStart.y)};
  auto&      verticalRect = fillVerticalRect[start];
  auto const vStart       = wrap::next(vertices->cbegin(), vertexIndex + start);
  auto const vEnd         = wrap::next(vertices->cbegin(), vertexIndex + finish);
  if ((delta.x != 0.0F) && (delta.y != 0.0F)) {
	auto const slope = -delta.x / delta.y;
	auto       point = *vEnd;
	proj(point, slope, opStart, opFinish, verticalRect.dopnt);
	proj(point, slope, ipStart, ipFinish, verticalRect.dipnt);
	point = *vStart;
	proj(point, slope, opStart, opFinish, verticalRect.aopnt);
	proj(point, slope, ipStart, ipFinish, verticalRect.aipnt);
	point = ipStart;
	if (proj(point, slope, opStart, opFinish, verticalRect.bopnt)) {
	  verticalRect.bipnt = ipStart;
	}
	else {
	  verticalRect.bopnt = opStart;
	  point              = opStart;
	  proj(point, slope, ipStart, ipFinish, verticalRect.bipnt);
	}
	point = ipFinish;
	if (proj(point, slope, opStart, opFinish, verticalRect.copnt)) {
	  verticalRect.cipnt = ipFinish;
	}
	else {
	  verticalRect.copnt = opFinish;
	  point              = opFinish;
	  proj(point, slope, ipStart, ipFinish, verticalRect.cipnt);
	}
  }
  else {
	if (delta.x != 0.0F) {
	  auto pointX = vEnd->x;
	  projv(pointX, opStart, opFinish, verticalRect.dopnt);
	  projv(pointX, ipStart, ipFinish, verticalRect.dipnt);
	  pointX = vStart->x;
	  projv(pointX, opStart, opFinish, verticalRect.aopnt);
	  projv(pointX, ipStart, ipFinish, verticalRect.aipnt);
	  if (projv(ipStart.x, opStart, opFinish, verticalRect.bopnt)) {
		verticalRect.bipnt = ipStart;
	  }
	  else {
		verticalRect.bopnt = opStart;
		projv(opStart.x, ipStart, ipFinish, verticalRect.bipnt);
	  }
	  if (projv(ipFinish.x, opStart, opFinish, verticalRect.copnt)) {
		verticalRect.cipnt = ipFinish;
	  }
	  else {
		verticalRect.copnt = opFinish;
		projv(opFinish.x, ipStart, ipFinish, verticalRect.cipnt);
	  }
	}
	else {
	  auto pointY = vEnd->y;
	  projh(pointY, opStart, opFinish, verticalRect.dopnt);
	  projh(pointY, ipStart, ipFinish, verticalRect.dipnt);
	  pointY = vStart->y;
	  projh(pointY, opStart, opFinish, verticalRect.aopnt);
	  projh(pointY, ipStart, ipFinish, verticalRect.aipnt);
	  if (projh(ipStart.y, opStart, opFinish, verticalRect.bopnt)) {
		verticalRect.bipnt = ipStart;
	  }
	  else {
		verticalRect.bopnt = opStart;
		projh(opStart.y, ipStart, ipFinish, verticalRect.bipnt);
	  }
	  if (projh(ipFinish.y, opStart, opFinish, verticalRect.copnt)) {
		verticalRect.cipnt = ipFinish;
	  }
	  else {
		verticalRect.copnt = opFinish;
		projh(opFinish.y, ipStart, ipFinish, verticalRect.cipnt);
	  }
	}
  }
}

void form::internal::spurfn(fPOINT const& innerPoint,
                            fPOINT const& outerPoint,
                            fPOINT&       underlayInnerPoint,
                            fPOINT&       underlayOuterPoint) noexcept {
  auto const delta     = fPOINT {outerPoint.x - innerPoint.x, outerPoint.y - innerPoint.y};
  underlayInnerPoint.x = delta.x * DIURAT + innerPoint.x;
  underlayInnerPoint.y = delta.y * DIURAT + innerPoint.y;
  underlayOuterPoint.x = delta.x * DOURAT + innerPoint.x;
  underlayOuterPoint.y = delta.y * DOURAT + innerPoint.y;
}

void form::internal::spurct(std::vector<VRCT2>&       underlayVerticalRect,
                            std::vector<VRCT2> const& fillVerticalRect,
                            uint32_t                  iRect) noexcept {
  auto const& fRect  = fillVerticalRect[iRect];
  auto&       ulRect = underlayVerticalRect[iRect];
  spurfn(fRect.aipnt, fRect.aopnt, ulRect.aipnt, ulRect.aopnt);
  spurfn(fRect.bipnt, fRect.bopnt, ulRect.bipnt, ulRect.bopnt);
  spurfn(fRect.cipnt, fRect.copnt, ulRect.cipnt, ulRect.copnt);
  spurfn(fRect.dipnt, fRect.dopnt, ulRect.dipnt, ulRect.dopnt);
}

void form::internal::duromb(fPOINT const& start0,
                            fPOINT const& finish0,
                            fPOINT const& start1,
                            fPOINT const& finish1,
                            fPOINT&       stitchPoint) {
  if (!StateMap->test(StateFlag::UND)) {
	auto const delta0  = fPOINT {stitchPoint.x - start0.x, stitchPoint.y - start0.y};
	auto const delta1  = fPOINT {stitchPoint.x - start1.x, stitchPoint.y - start1.y};
	auto const length0 = hypot(delta0.x, delta0.y);
	auto const length1 = hypot(delta1.x, delta1.y);
	if (length0 > length1) {
	  StateMap->set(StateFlag::FILDIR);
	}
	else {
	  StateMap->reset(StateFlag::FILDIR);
	}
  }
  auto const delta0  = fPOINT {finish0.x - start0.x, finish0.y - start0.y};
  auto const delta1  = fPOINT {finish1.x - start1.x, finish1.y - start1.y};
  auto const length0 = hypot(delta0.x, delta0.y);
  auto       count   = wrap::round<uint32_t>(length0 / (LineSpacing / 2.0F));
  if (count == 0U) {
	++count;
  }
  auto const factor  = wrap::toFloat(count);
  auto const step0   = fPOINT {delta0.x / factor, delta0.y / factor};
  auto const step1   = fPOINT {delta1.x / factor, delta1.y / factor};
  auto       start00 = start0;
  auto       start11 = start1;
  for (auto iStep = 0U; iStep < count; ++iStep) {
	if (StateMap->testAndFlip(StateFlag::FILDIR)) {
	  form::filinsb(start00, stitchPoint);
	}
	else {
	  form::filinsb(start11, stitchPoint);
	}
	start00.x += step0.x;
	start00.y += step0.y;
	start11.x += step1.x;
	start11.y += step1.y;
  }
}

auto form::psg() noexcept -> uint32_t {
  if (PseudoRandomValue == 0U) {
	PseudoRandomValue = SEED;
  }
  auto const temp = PseudoRandomValue & 0x48000000U;
  PseudoRandomValue <<= 1U;
  if (temp == 0x40000000U || temp == 0x8000000U) {
	++PseudoRandomValue;
  }
  return PseudoRandomValue;
}

void form::internal::fillSB(const fPOINT& pivot, float angle, float const& radius, fPOINT& stitchPoint, float const& level) {
  auto const outerPoint = fPOINT {pivot.x + cos(angle) * radius, pivot.y + sin(angle) * radius};
  form::filinsb(outerPoint, stitchPoint);
  auto const innerRadius = radius * level * 0.4F;
  auto const innerPoint = fPOINT {pivot.x + cos(angle) * innerRadius, pivot.y + sin(angle) * innerRadius};
  form::filinsb(innerPoint, stitchPoint);
}

void form::internal::spend(std::vector<VRCT2> const& fillVerticalRect, uint32_t start, uint32_t finish, fPOINT& stitchPoint) {
  // clang-format off
  constexpr auto level00 = std::array<float,  1U>{ 0.0F };
  constexpr auto level01 = std::array<float,  1U>{ 1.0F       };
  constexpr auto level02 = std::array<float,  2U>{ 0.0F/2.0F , 1.0F/2.0F  };
  constexpr auto level03 = std::array<float,  3U>{ 1.0F/3.0F , 0.0F/3.0F , 2.0F/3.0F  };
  constexpr auto level04 = std::array<float,  4U>{ 1.0F/4.0F , 3.0F/4.0F , 0.0F/4.0F , 2.0F/4.0F  };
  constexpr auto level05 = std::array<float,  5U>{ 2.0F/5.0F , 0.0F/5.0F , 3.0F/5.0F , 1.0F/5.0F , 4.0F/5.0F  };
  constexpr auto level06 = std::array<float,  6U>{ 3.0F/6.0F , 0.0F/6.0F , 2.0F/6.0F , 4.0F/6.0F , 1.0F/6.0F , 5.0F/6.0F  };
  constexpr auto level07 = std::array<float,  7U>{ 3.0F/7.0F , 0.0F/7.0F , 4.0F/7.0F , 1.0F/7.0F , 6.0F/7.0F , 2.0F/7.0F ,  5.0F/7.0F  };
  constexpr auto level08 = std::array<float,  8U>{ 4.0F/8.0F , 0.0F/8.0F , 5.0F/8.0F , 1.0F/8.0F , 3.0F/8.0F , 6.0F/8.0F ,  2.0F/8.0F , 7.0F/8.0F  };
  constexpr auto level09 = std::array<float,  9U>{ 4.0F/9.0F , 0.0F/9.0F , 5.0F/9.0F , 1.0F/9.0F , 6.0F/9.0F , 2.0F/9.0F ,  7.0F/9.0F , 3.0F/9.0F ,  8.0F/9.0F  };
  constexpr auto level10 = std::array<float, 10U>{ 5.0F/10.0F, 0.0F/10.0F, 6.0F/10.0F, 1.0F/10.0F, 7.0F/10.0F, 2.0F/10.0F,  8.0F/10.0F, 3.0F/10.0F,  9.0F/10.0F,  4.0F/10.0F };
  constexpr auto level11 = std::array<float, 11U>{ 5.0F/11.0F, 0.0F/11.0F, 6.0F/11.0F, 1.0F/11.0F, 7.0F/11.0F, 2.0F/11.0F,  8.0F/11.0F, 3.0F/11.0F,  9.0F/11.0F,  4.0F/11.0F,  2.0F/11.0F };
  constexpr auto level12 = std::array<float, 12U>{ 6.0F/12.0F, 0.0F/12.0F, 7.0F/12.0F, 1.0F/12.0F, 8.0F/12.0F, 2.0F/12.0F,  9.0F/12.0F, 3.0F/12.0F, 10.0F/12.0F,  4.0F/12.0F,  9.0F/12.0F,  2.0F/12.0F };
  constexpr auto level13 = std::array<float, 13U>{ 6.0F/13.0F, 0.0F/13.0F, 1.0F/13.0F, 7.0F/13.0F, 2.0F/13.0F, 8.0F/13.0F,  3.0F/13.0F, 9.0F/13.0F,  4.0F/13.0F, 10.0F/13.0F,  5.0F/13.0F, 10.0F/13.0F,  2.0F/13.0F };
  constexpr auto level14 = std::array<float, 14U>{ 7.0F/14.0F, 0.0F/14.0F, 8.0F/14.0F, 1.0F/14.0F, 9.0F/14.0F, 2.0F/14.0F, 10.0F/14.0F, 3.0F/14.0F, 11.0F/14.0F,  4.0F/14.0F, 12.0F/14.0F,  5.0F/14.0F, 11.0F/14.0F,  2.0F/14.0F };
  constexpr auto level15 = std::array<float, 15U>{ 7.0F/15.0F, 0.0F/15.0F, 8.0F/15.0F, 1.0F/15.0F, 9.0F/15.0F, 2.0F/15.0F, 10.0F/15.0F, 3.0F/15.0F, 11.0F/15.0F,  4.0F/15.0F, 12.0F/15.0F,  4.0F/15.0F, 13.0F/15.0F,  6.0F/15.0F,  2.0F/15.0F };
  // clang-format on

  static auto levels = std::array<float const*, 16> {level00.data(),
                                                     level01.data(),
                                                     level02.data(),
                                                     level03.data(),
                                                     level04.data(),
                                                     level05.data(),
                                                     level06.data(),
                                                     level07.data(),
                                                     level08.data(),
                                                     level09.data(),
                                                     level10.data(),
                                                     level11.data(),
                                                     level12.data(),
                                                     level13.data(),
                                                     level14.data(),
                                                     level15.data()};

  auto const innerDelta = fPOINT {(fillVerticalRect[finish].cipnt.x - fillVerticalRect[start].bipnt.x),
                                  (fillVerticalRect[finish].cipnt.y - fillVerticalRect[start].bipnt.y)};
  auto const innerLength = hypot(innerDelta.x, innerDelta.y);
  auto const outerDelta = fPOINT {(fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
                                  (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y)};
  auto const outerLength = hypot(outerDelta.x, outerDelta.y);
  auto const flag        = (outerLength > innerLength);
  auto const pivot       = flag ? fillVerticalRect[start].cipnt : fillVerticalRect[start].copnt;
  auto const startDelta  = flag ? fPOINT {fillVerticalRect[start].copnt.x - pivot.x,
                                         fillVerticalRect[start].copnt.y - pivot.y}
                               : fPOINT {fillVerticalRect[start].cipnt.x - pivot.x,
                                         fillVerticalRect[start].cipnt.y - pivot.y};
  auto const finishDelta = flag ? fPOINT {fillVerticalRect[finish].bopnt.x - pivot.x,
                                          fillVerticalRect[finish].bopnt.y - pivot.y}
                                : fPOINT {fillVerticalRect[finish].bipnt.x - pivot.x,
                                          fillVerticalRect[finish].bipnt.y - pivot.y};
  if (hypot(stitchPoint.x - pivot.x, stitchPoint.y - pivot.y) > PI_F2) {
	form::filinsb(pivot, stitchPoint);
  }
  auto       startAngle  = atan2(startDelta.y, startDelta.x);
  auto const finishAngle = atan2(finishDelta.y, finishDelta.x);
  auto       deltaAngle  = finishAngle - startAngle;
  if (deltaAngle > PI_F) {
	deltaAngle -= PI_F2;
  }
  if (deltaAngle < -PI_F) {
	deltaAngle += PI_F2;
  }
  auto const radius    = hypot(startDelta.x, startDelta.y);
  auto const arc       = fabs(radius * deltaAngle);
  auto       count     = wrap::round<uint32_t>(arc / LineSpacing);
  auto const stepAngle = deltaAngle / wrap::toFloat(count);
  if (count == 0U) {
	count = 1;
  }
  constexpr auto MASKLSN = 0xfffffff0; // mask out the least significant nibble to check if count > 15
  if ((count & MASKLSN) != 0U) {
	for (auto iCount = 0U; iCount < count; ++iCount) {
	  auto const level = wrap::toFloat(form::psg() % count) / wrap::toFloat(count);
	  fillSB(pivot, startAngle, radius, stitchPoint, level);
	  startAngle += stepAngle;
	}
  }
  else {
	auto const* levelData = levels.operator[](count);
	if (nullptr != levelData) {
	  for (auto iCount = 0U; iCount < count; ++iCount) {
		fillSB(pivot, startAngle, radius, stitchPoint, *levelData);
		++levelData;
		startAngle += stepAngle;
	  }
	}
  }
}

void form::internal::duspnd(float                     stitchLen,
                            std::vector<VRCT2> const& underlayVerticalRect,
                            std::vector<VRCT2> const& fillVerticalRect,
                            uint32_t                  start,
                            uint32_t                  finish,
                            float                     width,
                            fPOINT&                   stitchPoint) {
  if (StateMap->test(StateFlag::UND)) {
	if (StateMap->test(StateFlag::UNDPHAS)) {
	  form::filinsb(underlayVerticalRect[start].copnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[start].cipnt, stitchPoint);
	  auto const delta =
	      fPOINT {underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x,
	              underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y};
	  auto const length = hypot(delta.x, delta.y);
	  if (length > stitchLen) {
		auto const angle =
		    atan2(InsidePoints->operator[](finish).y - OutsidePoints->operator[](finish).y,
		          InsidePoints->operator[](finish).x - OutsidePoints->operator[](finish).x);
		auto const point = fPOINT {underlayVerticalRect[finish].bopnt.x + cos(angle) * width,
		                           underlayVerticalRect[finish].bopnt.y + sin(angle) * width};
		form::filinsb(point, stitchPoint);
	  }
	  form::filinsb(underlayVerticalRect[finish].bipnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[finish].bopnt, stitchPoint);
	}
	else {
	  form::filinsb(underlayVerticalRect[start].cipnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[start].copnt, stitchPoint);
	  auto const delta =
	      fPOINT {underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x,
	              underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y};
	  auto const length = hypot(delta.x, delta.y);
	  if (length > stitchLen) {
		auto const angle =
		    atan2(OutsidePoints->operator[](finish).y - InsidePoints->operator[](finish).y,
		          OutsidePoints->operator[](finish).x - InsidePoints->operator[](finish).x);
		auto const point = fPOINT {underlayVerticalRect[finish].bipnt.x + cos(angle) * width,
		                           underlayVerticalRect[finish].bipnt.y + sin(angle) * width};
		form::filinsb(point, stitchPoint);
	  }
	  form::filinsb(underlayVerticalRect[finish].bopnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[finish].bipnt, stitchPoint);
	}
  }
  else {
	spend(fillVerticalRect, start, finish, stitchPoint);
  }
}

void form::internal::pfn(std::vector<VRCT2> const& underlayVerticalRect,
                         std::vector<VRCT2> const& fillVerticalRect,
                         uint32_t                  startVertex,
                         std::vector<VRCT2> const& vrct,
                         float                     width) {
  // clang-format off
  auto const& form          = FormList->operator[](ClosestFormToCursor);
  auto        currentVertex = startVertex;
  auto        nextVertex    = form::nxt(form, currentVertex);
  auto const vertexIt      = wrap::next(FormVertices->cbegin(), form.vertexIndex + startVertex);
  auto        stitchPoint   = *vertexIt;
  // clang-format on
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	duromb(vrct[currentVertex].bipnt,
	       vrct[currentVertex].cipnt,
	       vrct[currentVertex].bopnt,
	       vrct[currentVertex].copnt,
	       stitchPoint);
	duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, currentVertex, nextVertex, width, stitchPoint);
	currentVertex = nextVertex;
	nextVertex    = form::nxt(form, nextVertex);
  }
}

void form::internal::prsmal(float width) {
  constexpr auto UFSFACT = 0.8F; // set the minimum to 80% of the underlay fill spacing
  constexpr auto WIDFACT = 0.9F; // or 90% of the actual width

  auto iOutput       = 0U;
  auto minimumLength = USPAC * UFSFACT;
  if (minimumLength > width) {
	minimumLength = width * WIDFACT;
  }
  auto iReference = 0U;
  for (auto iSequence = 1U; iSequence < wrap::toUnsigned(OSequence->size()); ++iSequence) {
	// clang-format off
	auto const seq    = OSequence->operator[](iSequence);
	auto const seqRef = OSequence->operator[](iReference);
	auto const delta  = fPOINT {seq.x - seqRef.x, seq.y - seqRef.y};
	auto const length = hypot(delta.x, delta.y);
	// clang-format on
	if (length > minimumLength) {
	  OSequence->operator[](iOutput++) = seq;
	  iReference                       = iSequence;
	}
  }
  OSequence->resize(iOutput);
}

void form::internal::pbrd(FRMHED const& form) {
  auto const spacing = LineSpacing;
  auto const start   = form::getlast(form);
  LineSpacing        = form.edgeSpacing;
  OSequence->clear();
  auto fillVerticalRect = std::vector<VRCT2> {};
  fillVerticalRect.resize(form.vertexCount);
  auto underlayVerticalRect = std::vector<VRCT2> {};
  underlayVerticalRect.resize(form.vertexCount);
  satin::satout(form, form.borderSize);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	sprct(FormVertices, form.vertexIndex, fillVerticalRect, iVertex, iVertex + 1U);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
  }
  sprct(FormVertices, form.vertexIndex, fillVerticalRect, (form.vertexCount - 1U), 0U);
  spurct(underlayVerticalRect, fillVerticalRect, (form.vertexCount - 1U));
  if ((form.edgeType & EGUND) != 0U) {
	StateMap->reset(StateFlag::SAT1);
	LineSpacing = USPAC;
	StateMap->set(StateFlag::UND);
	auto const width = form.borderSize * URAT;
	satin::satout(form, width);
	StateMap->set(StateFlag::UNDPHAS);
	StateMap->set(StateFlag::FILDIR);
	pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
	StateMap->reset(StateFlag::UNDPHAS);
	StateMap->reset(StateFlag::FILDIR);
	pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
	LineSpacing = form.edgeSpacing;
	prsmal(width);
	StateMap->reset(StateFlag::UND);
  }
  pfn(underlayVerticalRect, fillVerticalRect, start, fillVerticalRect, form.borderSize);
  LineSpacing = spacing;
}

void form::internal::apbrd(FRMHED const& form) {
  auto currentVertex = 0U;
  OSequence->clear();
  auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  OSequence->push_back(*vertexIt);
  for (auto iVertex = 0U; iVertex < form.vertexCount * 2U; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, IniFile.AppStitchLen);
	currentVertex = nextVertex;
  }
}

void form::internal::bhfn(FRMHED const& form, uint32_t start, uint32_t finish) {
  auto const vStart  = wrap::next(FormVertices->cbegin(), form.vertexIndex + start);
  auto const vFinish = wrap::next(FormVertices->cbegin(), form.vertexIndex + finish);
  auto const delta   = fPOINT {(vFinish->x - vStart->x), (vFinish->y - vStart->y)};
  auto const length  = hypot(delta.x, delta.y);
  auto const count   = wrap::round<uint32_t>(length / form.edgeSpacing);
  auto const step    = fPOINT {(delta.x / wrap::toFloat(count)), (delta.y / wrap::toFloat(count))};
  auto       innerPoint    = fPOINT {vStart->x, vStart->y};
  auto const rotationAngle = atan2(-delta.x, delta.y);
  auto const outerStep = fPOINT {form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle)};
  if (count != 0U) {
	for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	  auto const firstPoint = fPOINT {innerPoint.x + step.x, innerPoint.y + step.y};
	  auto const outerPoint = fPOINT {firstPoint.x + outerStep.x, firstPoint.y + outerStep.y};
	  OSequence->push_back(fPOINT {firstPoint});
	  OSequence->push_back(fPOINT {innerPoint});
	  OSequence->push_back(fPOINT {firstPoint});
	  OSequence->push_back(fPOINT {outerPoint});
	  OSequence->push_back(fPOINT {firstPoint});
	  OSequence->push_back(fPOINT {outerPoint});
	  OSequence->push_back(fPOINT {firstPoint});
	  innerPoint.x += step.x;
	  innerPoint.y += step.y;
	}
	auto const firstPoint = fPOINT {innerPoint.x + step.x, innerPoint.y + step.y};
	OSequence->push_back(fPOINT {firstPoint});
	OSequence->push_back(fPOINT {innerPoint});
  }
}

void form::internal::bhcrnr(FRMHED const& form, uint32_t vertex) {
  auto const nextVertex = form::nxt(form, vertex);
  auto const vertexIt   = wrap::next(FormVertices->cbegin(), form.vertexIndex + nextVertex);

  auto* ptr = (StateMap->test(StateFlag::INDIR)) ? OutsidePoints : InsidePoints;
  if (nullptr != ptr) {
	auto delta = fPOINT {ptr->operator[](nextVertex).x - vertexIt->x,
	                     ptr->operator[](nextVertex).y - vertexIt->y};

	auto const length = hypot(delta.x, delta.y);
	auto const ratio  = ButtonholeCornerLength / length;
	delta.x *= ratio;
	delta.y *= ratio;
	auto const point = fPOINT {vertexIt->x + delta.x, vertexIt->y + delta.y};
	OSequence->push_back(*vertexIt);
	OSequence->push_back(fPOINT {point});
	OSequence->push_back(*vertexIt);
	OSequence->push_back(fPOINT {point});
	OSequence->push_back(*vertexIt);
  }
}

void form::internal::bhbrd(FRMHED const& form) {
  auto vertex = form::getlast(form);
  OSequence->clear();
  auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex + vertex);
  OSequence->push_back(*vertexIt);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, vertex);
	bhfn(form, vertex, nextVertex);
	bhcrnr(form, vertex);
	vertex = nextVertex;
  }
}

void form::internal::dubfn(FRMHED const& currentForm) {
  brdfil(currentForm);
  for (auto iBackward = OSequence->size(); iBackward != 0; --iBackward) {
	OSequence->push_back(OSequence->operator[](iBackward - 1U));
  }
}

void form::internal::chkbrd(FRMHED const& form) {
  if (form.edgeType != 0U) {
	switch (form.edgeType & NEGUND) {
	  case EDGELINE: { // Line
		brdfil(form);
		break;
	  }
	  case EDGEBEAN: { // Bean
		bold(form);
		break;
	  }
	  case EDGECLIP: { // Clipboard
		auto clipRect = fRECTANGLE {};
		clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		clip::clpbrd(form, clipRect, form::getlast(form));
		break;
	  }
	  case EDGECLIPX: { // Even Clipboard
		auto clipRect = fRECTANGLE {};
		clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		clip::duxclp(form);
		break;
	  }
	  case EDGEANGSAT: { // Angle Satin
		satin::sbrd(form);
		break;
	  }
	  case EDGEPROPSAT: { // Proportional Satin
		pbrd(form);
		break;
	  }
	  case EDGEAPPL: { // Applique
		apbrd(form);
		ritapbrd();
		satin::sbrd(form);
		break;
	  }
	  case EDGEBHOL: { // BH Buttonhole
		auto const length      = ButtonholeCornerLength;
		ButtonholeCornerLength = form::getblen();
		satin::satout(form, BHWIDTH);
		bhbrd(form);
		ButtonholeCornerLength = length;
		break;
	  }
	  case EDGEPICOT: { // Picot
		auto clipRect = fRECTANGLE {};
		clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		clip::clpic(form, clipRect);
		break;
	  }
	  case EDGEDOUBLE: { // Double
		dubfn(form);
		break;
	  }
	  case EDGELCHAIN: { // Lin Chain
		StateMap->set(StateFlag::LINCHN);
		clip::chnfn(form);
		break;
	  }
	  case EDGEOCHAIN: { // Open Chain
		StateMap->reset(StateFlag::LINCHN);
		clip::chnfn(form);
		break;
	  }
	  default: {
		outDebugString(L"default hit in chkbrd: edgeType [{}]\n", form.edgeType & NEGUND);
		break;
	  }
	}
	ritbrd(form);
  }
}

void form::internal::fnvrt(std::vector<fPOINT>&   currentFillVertices,
                           std::vector<uint32_t>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints) {
  auto const [min, max] =
      std::minmax_element(std::next(currentFillVertices.begin(), 1),
                          currentFillVertices.end(),
                          [](fPOINT const& a, fPOINT const& b) { return a.x < b.x; });
  auto const highX           = max->x;
  auto const lineOffset      = std::floor(min->x / LineSpacing);
  auto const lowX            = LineSpacing * lineOffset;
  auto       fillLineCount   = wrap::floor<uint32_t>((highX - lowX) / LineSpacing + 1.0F);
  auto const step            = (highX - lowX) / wrap::toFloat(fillLineCount);
  auto       currentX        = lowX;
  auto       projectedPoints = std::vector<fPOINTLINE> {};
  projectedPoints.reserve(currentFillVertices.size() + 2U);
  auto const currentVertexCount = wrap::toUnsigned(currentFillVertices.size());
  for (auto iLine = 0U; iLine < fillLineCount; ++iLine) {
	auto iLineCounter = 0U;
	currentX += step;
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  auto const iNextVertex = (iVertex + 1U) % currentVertexCount;
	  auto       point       = fPOINT {};
	  if (projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
		++iLineCounter;
	  }
	}
	fillLineCount += iLineCounter;
  }
  lineEndpoints.reserve(wrap::toSize(fillLineCount) + 1U);
  auto lineGroupIndex = 0U;
  // groupIndex cannot be more than fillLineCount so reserve that amount of memory to reduce re-allocations
  groupIndexSequence.reserve(fillLineCount);
  currentX = lowX;
  for (auto iLine = 0U; iLine < fillLineCount; ++iLine) {
	projectedPoints.clear();
	projectedPoints.reserve(currentVertexCount);
	currentX += step;
	auto iPoint = 0U;
	for (auto iVertex = uint16_t {0U}; iVertex < currentVertexCount; ++iVertex) {
	  auto const iNextVertex = (iVertex + 1U) % currentVertexCount;
	  auto       point       = fPOINT {};
	  if (projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
		auto const a = fPOINTLINE {point.x, point.y, iVertex};
		projectedPoints.push_back(a);
		++iPoint;
	  }
	}
	if (iPoint > 1) {
	  auto const evenPointCount = (iPoint & 0xfffffffe);
	  groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
	  std::sort(projectedPoints.begin(), projectedPoints.end(), fi::fplComp);
	  iPoint                    = 0;
	  auto const savedLineCount = lineEndpoints.size();
	  while (iPoint < evenPointCount) {
		if (lineEndpoints.size() < fillLineCount) {
		  lineEndpoints.push_back(SMALPNTL {projectedPoints[iPoint].line,
		                                    lineGroupIndex,
		                                    projectedPoints[iPoint].x,
		                                    projectedPoints[iPoint].y});
		  ++iPoint;
		  lineEndpoints.push_back(SMALPNTL {projectedPoints[iPoint].line,
		                                    lineGroupIndex,
		                                    projectedPoints[iPoint].x,
		                                    projectedPoints[iPoint].y});
		  ++iPoint;
		}
	  }
	  if (lineEndpoints.size() != savedLineCount) {
		++lineGroupIndex;
	  }
	}
  }
  groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
}

void form::internal::fnang(std::vector<uint32_t>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints,
                           float                  rotationAngle,
                           fPOINT&                rotationCenter,
                           FRMHED&                angledForm,
                           std::vector<fPOINT>&   angledFormVertices) {
  angledForm = FormList->operator[](ClosestFormToCursor);

  auto const& angRect = angledForm.rectangle;
  rotationCenter = fPOINT {wrap::midl(angRect.right, angRect.left), wrap::midl(angRect.top, angRect.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto vertexIt = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*vertexIt);
	++vertexIt;
	thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
  }
  angledForm.vertexIndex = 0;
  fi::fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void form::internal::fnhor(std::vector<uint32_t>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints,
                           float const            rotationAngle,
                           fPOINT&                rotationCenter,
                           FRMHED&                angledForm,
                           std::vector<fPOINT>&   angledFormVertices) {
  angledForm = FormList->operator[](ClosestFormToCursor);

  auto const& angRect = angledForm.rectangle;
  rotationCenter = fPOINT {wrap::midl(angRect.right, angRect.left), wrap::midl(angRect.top, angRect.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto vertexIt = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*vertexIt);
	thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	++vertexIt;
  }
  angledForm.vertexIndex = 0;
  fi::fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void form::internal::prebrd(FRMHED const& form, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices) {
  constexpr auto REDFACT = 0.1F; // reduction factor

  auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto const nextV    = std::next(vertexIt, 1);
  auto       delta    = fPOINT {(nextV->x - vertexIt->x), (nextV->y - vertexIt->y)};
  angledFormVertices.resize(wrap::toSize(form.vertexCount) + 3U);
  auto output = angledFormVertices.begin();
  ++output;
  std::copy(vertexIt, wrap::next(vertexIt, form.vertexCount), output);
  auto ratio = (fabs(delta.x) > fabs(delta.y)) ? fabs(REDFACT / delta.x) : fabs(REDFACT / delta.y);
  angledFormVertices[0].x = vertexIt->x - delta.x * ratio;
  angledFormVertices[0].y = vertexIt->y - delta.y * ratio;
  angledForm              = FormList->operator[](ClosestFormToCursor);
  angledForm.vertexIndex  = 0;
  angledForm.vertexCount += 3;
  auto const lastV = wrap::next(vertexIt, form.vertexCount - 1U);
  auto const prevV = wrap::next(lastV, -1);
  delta.x          = lastV->x - prevV->x;
  delta.y          = lastV->y - prevV->y;
  ratio = (fabs(delta.x) > fabs(delta.y)) ? fabs(REDFACT / delta.x) : fabs(REDFACT / delta.y);
  angledFormVertices[angledForm.vertexCount - 1U].x = lastV->x + delta.x * ratio;
  angledFormVertices[angledForm.vertexCount - 1U].y = lastV->y + delta.y * ratio;
}

void form::internal::plfn(FRMHED const&             form,
                          std::vector<VRCT2> const& underlayVerticalRect,
                          std::vector<VRCT2> const& fillVerticalRect,
                          std::vector<VRCT2> const& prct,
                          float                     width,
                          fPOINT&                   stitchPoint) {
  duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt, stitchPoint);
  duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, 1, 2, width, stitchPoint);
  for (auto iVertex = 2U; iVertex < form.vertexCount - 4U; ++iVertex) {
	duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt, stitchPoint);
	duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1U, width, stitchPoint);
  }
  duromb(prct[form.vertexCount - 4U].bipnt,
         prct[form.vertexCount - 4U].dipnt,
         prct[form.vertexCount - 4U].bopnt,
         prct[form.vertexCount - 4U].dopnt,
         stitchPoint);
}

void form::internal::plbak(uint32_t backPoint) {
  if ((!OSequence->empty()) && (backPoint < (OSequence->size() - 1U))) {
	auto iSequence = wrap::toUnsigned(OSequence->size() - 1U);
	while (iSequence > backPoint) {
	  std::swap(OSequence->operator[](iSequence), OSequence->operator[](backPoint));
	  --iSequence;
	  ++backPoint;
	}
  }
}

void form::internal::plbrd(FRMHED const& form, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices) {
  // Ensure that we have at least 4 array members
  auto fillVerticalRect = std::vector<VRCT2> {};
  fillVerticalRect.resize(wrap::toSize(form.vertexCount) + 3U);
  auto underlayVerticalRect = std::vector<VRCT2> {};
  underlayVerticalRect.resize(wrap::toSize(form.vertexCount) + 3U);
  prebrd(form, angledForm, angledFormVertices);
  satin::satout(angledForm, angledForm.borderSize);
  InsidePoints->push_back(InsidePoints->front());
  OutsidePoints->push_back(OutsidePoints->front());
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount - 1U; ++iVertex) {
	sprct(AngledFormVertices, 0, fillVerticalRect, iVertex, iVertex + 1U);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
  }
  sprct(AngledFormVertices, 0U, fillVerticalRect, (angledForm.vertexCount - 1U), 0U);
  spurct(underlayVerticalRect, fillVerticalRect, (angledForm.vertexCount - 1U));
  auto const vertexIt = wrap::next(angledFormVertices.cbegin(), angledForm.vertexIndex);
  if ((angledForm.attribute & SBLNT) == 0U) {
	auto const val = std::next(vertexIt, 1);

	fillVerticalRect[1].aipnt     = *val;
	fillVerticalRect[1].aopnt     = *val;
	underlayVerticalRect[1].aipnt = *val;
	underlayVerticalRect[1].aopnt = *val;
  }
  if ((angledForm.attribute & FBLNT) == 0U) {
	auto const val = wrap::next(vertexIt, angledForm.vertexCount - 1U);

	fillVerticalRect[angledForm.vertexCount - 4U].dipnt     = *val;
	fillVerticalRect[angledForm.vertexCount - 4U].dopnt     = *val;
	underlayVerticalRect[angledForm.vertexCount - 4U].dipnt = *val;
	underlayVerticalRect[angledForm.vertexCount - 4U].dopnt = *val;
  }
  OSequence->clear();
  auto stitchPoint = *vertexIt;
  if ((angledForm.edgeType & EGUND) != 0U) {
	LineSpacing = USPAC;
	StateMap->set(StateFlag::UND);
	auto const width = angledForm.borderSize * URAT;
	StateMap->set(StateFlag::UNDPHAS);
	StateMap->reset(StateFlag::FILDIR);
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width, stitchPoint);
	auto const savedIndex = wrap::toUnsigned(OSequence->size());
	StateMap->reset(StateFlag::UNDPHAS);
	stitchPoint = *vertexIt;
	StateMap->set(StateFlag::FILDIR);
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width, stitchPoint);
	plbak(savedIndex);
	prsmal(width);
	if (!OSequence->empty()) { // ensure that we can do a valid read from OSequence
	  stitchPoint = OSequence->back();
	}
  }
  StateMap->reset(StateFlag::UND);
  LineSpacing = angledForm.edgeSpacing;
  plfn(angledForm, underlayVerticalRect, fillVerticalRect, fillVerticalRect, angledForm.borderSize, stitchPoint);
  LineSpacing = form.edgeSpacing;
}

void form::internal::lapbrd(FRMHED const& form) {
  auto const savedStitchLength = UserStitchLength;
  OSequence->clear();
  UserStitchLength = IniFile.AppStitchLen;
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	bdrlin(form.vertexIndex, iVertex, iVertex + 1U, IniFile.AppStitchLen);
  }
  for (auto iVertex = form.vertexCount - 1U; iVertex != 0U; --iVertex) {
	bdrlin(form.vertexIndex, iVertex, iVertex - 1U, IniFile.AppStitchLen);
  }
  UserStitchLength = savedStitchLength;
}

void form::internal::blbrd(FRMHED const& form) {
  OSequence->clear();
  auto vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  OSequence->push_back(*vertexIt);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 2U; ++iVertex) {
	bhfn(form, iVertex, iVertex + 1U);
	bhcrnr(form, iVertex);
  }
  bhfn(form, form.vertexCount - 2U, form.vertexCount - 1U);
  vertexIt = wrap::next(vertexIt, form.vertexCount - 1U);
  OSequence->push_back(*vertexIt);
}

void form::internal::contf(FRMHED& form) {
  auto const start  = form.angleOrClipData.guide.start;
  auto const finish = form.angleOrClipData.guide.finish;
  // ToDo - Find a better way to avoid crashing than clamping it if start is after finish
  if (start < finish) {
	auto       vBegin          = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	auto const selectionStart  = wrap::next(vBegin, start);
	auto const lowVertexIndex  = start;
	auto const highVertexIndex = form.vertexCount - start - 1U;
	auto       lowCounts       = std::vector<uint32_t> {};
	lowCounts.resize(lowVertexIndex);
	auto highCounts = std::vector<uint32_t> {};
	highCounts.resize(highVertexIndex);
	auto lowLengths = std::vector<float> {};
	lowLengths.resize(lowVertexIndex);
	auto highLengths = std::vector<float> {};
	highLengths.resize(highVertexIndex);
	auto lowDeltas = std::vector<fPOINT> {};
	lowDeltas.resize(lowVertexIndex);
	auto highDeltas = std::vector<fPOINT> {};
	highDeltas.resize(highVertexIndex);
	auto lowSteps = std::vector<fPOINT> {};
	lowSteps.resize(lowVertexIndex);
	auto highSteps = std::vector<fPOINT> {};
	highSteps.resize(highVertexIndex);
	auto lowVertices = std::vector<fPOINT> {};
	lowVertices.resize(lowVertexIndex);
	auto highVertices = std::vector<fPOINT> {};
	highVertices.resize(highVertexIndex);
	auto lowLength = 0.0F;
	OSequence->clear();
	auto lowIndex = 0U;
	{
	  auto vertex = wrap::next(vBegin, lowVertexIndex);
	  for (auto iVertex = lowVertexIndex; iVertex != 0; --iVertex) {
		lowVertices[lowIndex] = *vertex;
		auto prevVert         = wrap::next(vertex, -1);
		lowDeltas[lowIndex]   = fPOINT {prevVert->x - vertex->x, prevVert->y - vertex->y};
		lowLengths[lowIndex]  = hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
		lowLength += lowLengths[lowIndex];
		++lowIndex;
		--vertex;
	  }
	}
	auto const selectedVertexCount = finish - start;
	auto       polyLines           = std::vector<PVEC> {};
	polyLines.resize(selectedVertexCount);
	{
	  auto selind = 0U;
	  auto vertex = wrap::next(vBegin, start + 1);
	  for (auto iVertex = start + 1U; iVertex <= finish; ++iVertex) {
		auto const delta  = fPOINT {vertex->x - selectionStart->x, vertex->y - selectionStart->y};
		polyLines[selind] = PVEC {atan2(delta.y, delta.x), hypot(delta.x, delta.y)};
		++selind;
		++vertex;
	  }
	}
	auto highIndex  = 0U;
	auto highLength = 0.0F;
	{
	  auto vertex     = wrap::next(vBegin, finish);
	  auto nextVertex = wrap::next(vertex, 1);
	  for (auto iVertex = finish; iVertex < form.vertexCount - 1U; ++iVertex) {
		highVertices[highIndex] = *vertex;
		highDeltas[highIndex]   = fPOINT {nextVertex->x - vertex->x, nextVertex->y - vertex->y};
		highLengths[highIndex]  = hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
		highLength += highLengths[highIndex];
		++highIndex;
		++vertex;
		++nextVertex;
	  }
	}
	auto lowSpacing  = form.fillSpacing;
	auto highSpacing = lowSpacing;
	if (highLength < lowLength) {
	  highSpacing = form.fillSpacing * highLength / lowLength;
	}
	else {
	  lowSpacing = form.fillSpacing * lowLength / highLength;
	}
	for (auto iVertex = 0U; iVertex < lowVertexIndex; ++iVertex) {
	  lowCounts[iVertex] = wrap::round<uint32_t>(lowLengths[iVertex] / lowSpacing);
	  lowSteps[iVertex]  = fPOINT {lowDeltas[iVertex].x / wrap::toFloat(lowCounts[iVertex]),
                                  lowDeltas[iVertex].y / wrap::toFloat(lowCounts[iVertex])};
	}
	for (auto iVertex = 0U; iVertex < highVertexIndex; ++iVertex) {
	  highCounts[iVertex] = wrap::round<uint32_t>(highLengths[iVertex] / highSpacing);
	  highSteps[iVertex]  = fPOINT {highDeltas[iVertex].x / wrap::toFloat(highCounts[iVertex]),
                                   highDeltas[iVertex].y / wrap::toFloat(highCounts[iVertex])};
	}
	lowIndex = highIndex = 0;
	StateMap->reset(StateFlag::FILDIR);
	auto       lowCount   = 0U;
	auto       highCount  = 0U;
	auto       finVertex  = wrap::next(vBegin, finish);
	auto       strtVertex = wrap::next(vBegin, start);
	auto       delta      = fPOINT {finVertex->x - strtVertex->x, finVertex->y - strtVertex->y};
	auto const reference  = PVEC {atan2(delta.y, delta.x), hypot(delta.x, delta.y)};
	auto       lowStep    = fPOINT {};
	auto       lowPoint   = fPOINT {};
	auto       highStep   = fPOINT {};
	auto       highPoint  = fPOINT {};
	while ((lowCount != 0U) || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
	  if (lowCount != 0U) {
		--lowCount;
	  }
	  else {
		if (lowIndex < lowVertexIndex) {
		  lowCount = lowCounts[lowIndex];
		  lowStep  = lowSteps[lowIndex];
		  lowPoint = lowVertices[lowIndex];
		  ++lowIndex;
		}
	  }
	  if (highCount != 0U) {
		--highCount;
	  }
	  else {
		if (highIndex < highVertexIndex) {
		  highCount = highCounts[highIndex];
		  highStep  = highSteps[highIndex];
		  highPoint = highVertices[highIndex];
		  ++highIndex;
		}
	  }
	  delta = fPOINT {highPoint.x - lowPoint.x, highPoint.y - lowPoint.y};

	  constexpr auto REFFACT = 0.9F; // reduction factor for the reference
	  if (reference.length > REFFACT * LineSpacing) {
		auto const polyLine = PVEC {atan2(delta.y, delta.x), hypot(delta.x, delta.y)};
		auto const polyDiff = PVEC {polyLine.angle - reference.angle, polyLine.length / reference.length};
		if (StateMap->testAndFlip(StateFlag::FILDIR)) {
		  OSequence->push_back(fPOINT {lowPoint});
		  for (auto iVertex = 0U; iVertex < (selectedVertexCount - 1); ++iVertex) {
			auto const length = polyLines[iVertex].length * polyDiff.length;
			auto const angle  = polyLines[iVertex].angle + polyDiff.angle;
			OSequence->push_back(fPOINT {lowPoint.x + cos(angle) * length, lowPoint.y + sin(angle) * length});
		  }
		}
		else {
		  OSequence->push_back(fPOINT {highPoint});
		  for (auto iVertex = selectedVertexCount - 1U; iVertex != 0; --iVertex) {
			auto const length = polyLines[iVertex - 1U].length * polyDiff.length;
			auto const angle  = polyLines[iVertex - 1U].angle + polyDiff.angle;
			OSequence->push_back(fPOINT {lowPoint.x + cos(angle) * length, lowPoint.y + sin(angle) * length});
		  }
		}
	  }
	  lowPoint.x += lowStep.x;
	  lowPoint.y += lowStep.y;
	  highPoint.x += highStep.x;
	  highPoint.y += highStep.y;
	}
	if (StateMap->test(StateFlag::FILDIR)) {
	  OSequence->push_back(*vBegin);
	}
	else {
	  auto lastVert = wrap::next(vBegin, form.vertexCount - 1U);
	  OSequence->push_back(*lastVert);
	}
	if (form.lengthOrCount.stitchLength < MinStitchLength) {
	  form.lengthOrCount.stitchLength = MinStitchLength;
	}
  }
}

void form::internal::duflt(float& formOffset, std::vector<fPOINT>& currentFormVertices) {
  constexpr auto SAFOFFST = 0.01F; // factor to ensure that formOffset is not 0

  auto const leftEdge =
      (std::min_element(currentFormVertices.begin(), currentFormVertices.end(), [](fPOINT const& a, fPOINT const& b) {
	    return a.x < b.x;
      }))->x;
  if (leftEdge < ClipRectSize.cx) {
	StateMap->set(StateFlag::WASNEG);
	formOffset = ClipRectSize.cx + fabs(leftEdge) + SAFOFFST;
	for (auto& vertex : currentFormVertices) {
	  vertex.x += formOffset;
	}
	auto& form = FormList->operator[](ClosestFormToCursor);
	form.rectangle.left += formOffset;
	form.rectangle.right += formOffset;
  }
  else {
	StateMap->reset(StateFlag::WASNEG);
  }
}

auto form::internal::leftsid(std::vector<fPOINT> const& currentFormVertices) noexcept -> uint32_t {
  auto leftVertex = 0U;
  auto iVertex    = 0U;
  auto minimumX   = BIGFLOAT;
  for (auto const& vertex : currentFormVertices) {
	if (vertex.x < minimumX) {
	  minimumX   = vertex.x;
	  leftVertex = iVertex;
	}
	++iVertex;
  }
  return leftVertex;
}

auto form::internal::vclpComp(VCLPX const& vclpx1, VCLPX const& vclpx2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (vclpx1.segment < vclpx2.segment) {
	return true;
  }
  if (vclpx2.segment < vclpx1.segment) {
	return false;
  }
  if (vclpx1.vertex < vclpx2.vertex) {
	return true;
  }
  if (vclpx2.vertex < vclpx1.vertex) {
	return false;
  }
  return false;
}

auto form::internal::isin(FRMHED const&              form,
                          std::vector<VCLPX> const&  regionCrossingData,
                          float                      xCoordinate,
                          float                      yCoordinate,
                          uint32_t                   regionCrossingStart,
                          uint32_t                   regionCrossingEnd,
                          fRECTANGLE const&          boundingRect,
                          std::vector<fPOINT> const& currentFormVertices) -> bool {
  auto count = 0U;
  auto point = fPOINT {};
  if (xCoordinate < boundingRect.left) {
	return false;
  }
  if (yCoordinate > boundingRect.top) {
	return false;
  }
  if (xCoordinate > boundingRect.right) {
	return false;
  }
  if (yCoordinate < boundingRect.bottom) {
	return false;
  }
  for (auto iRegion = regionCrossingStart; iRegion < regionCrossingEnd; ++iRegion) {
	auto const iVertex     = regionCrossingData[iRegion].vertex;
	auto const startVertex = wrap::next(currentFormVertices.cbegin(), iVertex);
	auto const endVertex   = wrap::next(currentFormVertices.cbegin(), form::nxt(form, iVertex));
	if (projv(xCoordinate, *startVertex, *endVertex, point)) {
	  if (point.y > yCoordinate) {
		if (startVertex->x != xCoordinate && endVertex->x != xCoordinate) {
		  ++count;
		}
		else {
		  if (startVertex->x < endVertex->x) {
			if (endVertex->x != xCoordinate) {
			  ++count;
			}
		  }
		  else {
			if (startVertex->x != xCoordinate) {
			  ++count;
			}
		  }
		}
	  }
	}
  }
  return (count & 1U) != 0U;
}

void form::internal::inspnt(std::vector<CLIPNT>& clipStitchPoints) {
  auto const clipStitchPoint = CLIPNT {clipStitchPoints.back().x, clipStitchPoints.back().y, 0, 0};
  clipStitchPoints.back().x =
      wrap::midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2U].x);
  clipStitchPoints.back().y =
      wrap::midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2U].y);
  clipStitchPoints.back().flag = 1;
  clipStitchPoints.push_back(clipStitchPoint);
}

auto form::internal::isect(uint32_t                   vertex0,
                           uint32_t                   vertex1,
                           fPOINT&                    intersection,
                           float&                     length,
                           fPOINT const&              lineSegmentStart,
                           fPOINT const&              lineSegmentEnd,
                           std::vector<fPOINT> const& currentFormVertices) -> bool {
  auto const delta =
      fPOINT {(lineSegmentEnd.x - lineSegmentStart.x), (lineSegmentEnd.y - lineSegmentStart.y)};
  auto const point            = lineSegmentStart;
  auto       tempIntersection = fPOINT {};
  auto       flag             = false;
  auto const vertexZero       = wrap::next(currentFormVertices.cbegin(), vertex0);
  auto const vertexOne        = wrap::next(currentFormVertices.cbegin(), vertex1);
  if ((delta.x != 0.0F) && (delta.y != 0.0F)) {
	flag = proj(point, delta.y / delta.x, *vertexZero, *vertexOne, tempIntersection);
  }
  else {
	if (delta.y != 0.0F) {
	  flag = projv(point.x, *vertexZero, *vertexOne, tempIntersection);
	}
	else {
	  if (delta.x != 0.0F) {
		flag = projh(point.y, *vertexZero, *vertexOne, tempIntersection);
	  }
	  else if (vertexZero->y == lineSegmentStart.y && vertexOne->y == lineSegmentStart.y) {
		auto left  = vertexZero->x;
		auto right = vertexOne->x;
		if (vertexZero->x > vertexOne->x) {
		  std::swap(left, right);
		}
		if (lineSegmentStart.x > left && lineSegmentStart.x < right) {
		  tempIntersection = lineSegmentStart;
		  flag             = true;
		}
	  }
	}
  }
  if (tempIntersection.x < TNYFLOAT) {
	tempIntersection.x = 0.0F;
  }
  if (tempIntersection.y < TNYFLOAT) {
	tempIntersection.y = 0.0F;
  }
  intersection = tempIntersection;
  length = hypot(tempIntersection.x - lineSegmentStart.x, tempIntersection.y - lineSegmentStart.y);
  // ToDo - should length be determined from start or end?
  //	 hypot(tempIntersection.x-lineSegmentEnd.x,tempIntersection.y-lineSegmentEnd.y);
  return flag;
}

auto form::internal::clipComp(CLIPSORT const* const arg1, CLIPSORT const* const arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if ((arg1 != nullptr) && (arg2 != nullptr)) {
	auto const local1 = arg1->segmentLength;
	auto const local2 = arg2->segmentLength;
	if (local1 < local2) {
	  return true;
	}
	if (local2 < local1) {
	  return false;
	}
  }

  return false;
}

void form::internal::mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, uint32_t destination, uint32_t source) noexcept {
  if (destination != source) {
	arrayOfClipIntersectData[destination] = arrayOfClipIntersectData[source];
  }
}

auto form::internal::insect(FRMHED const&              form,
                            std::vector<CLIPSORT>&     clipIntersectData,
                            std::vector<VCLPX> const&  regionCrossingData,
                            std::vector<CLIPSORT*>&    arrayOfClipIntersectData,
                            uint32_t                   regionCrossingStart,
                            uint32_t                   regionCrossingEnd,
                            fPOINT const&              lineSegmentStart,
                            fPOINT const&              lineSegmentEnd,
                            std::vector<fPOINT> const& currentFormVertices) -> uint32_t {
  auto iIntersection = 0U;
  auto count         = 0U;
  arrayOfClipIntersectData.clear();
  for (auto iRegions = regionCrossingStart; iRegions < regionCrossingEnd; ++iRegions) {
	auto const currentVertex = regionCrossingData[iRegions].vertex;
	auto const nextVertex    = form::nxt(form, currentVertex);
	if (isect(currentVertex,
	          nextVertex,
	          clipIntersectData[iIntersection].point,
	          clipIntersectData[iIntersection].sideLength,
	          lineSegmentStart,
	          lineSegmentEnd,
	          currentFormVertices)) {
	  auto const  hFlag        = (lineSegmentEnd.x > lineSegmentStart.x);
	  auto const  vFlag        = (lineSegmentEnd.y > lineSegmentStart.y);
	  auto const  left         = hFlag ? lineSegmentStart.x : lineSegmentEnd.x;
	  auto const  right        = hFlag ? lineSegmentEnd.x : lineSegmentStart.x;
	  auto const  top          = vFlag ? lineSegmentEnd.y : lineSegmentStart.y;
	  auto const  bottom       = vFlag ? lineSegmentStart.y : lineSegmentEnd.y;
	  auto const& intersection = clipIntersectData[iIntersection].point;
	  if (intersection.x >= left && intersection.x <= right && intersection.y >= bottom &&
	      intersection.y <= top) {
		clipIntersectData[iIntersection].segmentLength =
		    hypot(clipIntersectData[iIntersection].point.x - lineSegmentStart.x,
		          clipIntersectData[iIntersection].point.y - lineSegmentStart.y);
		clipIntersectData[iIntersection].vertexIndex = currentVertex;
		arrayOfClipIntersectData.push_back(&clipIntersectData[iIntersection]);
		++iIntersection;
		++count;
	  }
	}
  }
  if (count > 1) {
	std::sort(arrayOfClipIntersectData.begin(), arrayOfClipIntersectData.end(), fi::clipComp);
	auto iDestination = 1U;
	for (auto iIntersect = 0U; iIntersect < count - 1U; ++iIntersect) {
	  if (fabs(arrayOfClipIntersectData[iIntersect]->segmentLength -
	           arrayOfClipIntersectData[wrap::toSize(iIntersect) + 1U]->segmentLength) > TNYFLOAT) {
		mvpclp(arrayOfClipIntersectData, iDestination++, iIntersect + 1U);
	  }
	}
	count = iDestination;
  }
  return count;
}

auto form::internal::getlen(std::vector<CLIPNT>&       clipStitchPoints,
                            std::vector<float> const&  lengths,
                            uint32_t                   iPoint,
                            std::vector<fPOINT> const& currentFormVertices) -> float {
  clipStitchPoints[iPoint].vertexIndex %= currentFormVertices.size();
  auto const vertexIt = wrap::next(currentFormVertices.cbegin(), clipStitchPoints[iPoint].vertexIndex);
  return lengths[clipStitchPoints[iPoint].vertexIndex] +
         hypot(vertexIt->x - clipStitchPoints[iPoint].x, vertexIt->y - clipStitchPoints[iPoint].y);
}

auto form::internal::clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
                             std::vector<CLPSEG>&       clipSegments,
                             std::vector<float> const&  lengths,
                             uint32_t                   start,
                             uint32_t                   finish,
                             std::vector<fPOINT> const& currentFormVertices) -> uint32_t {
  auto const clipSegment = CLPSEG {start,
                                   getlen(clipStitchPoints, lengths, start, currentFormVertices),
                                   0U,
                                   clipStitchPoints[start].vertexIndex,
                                   finish,
                                   getlen(clipStitchPoints, lengths, finish, currentFormVertices),
                                   0U,
                                   clipStitchPoints[finish].vertexIndex,
                                   0};
  clipSegments.push_back(clipSegment);
  return finish + 1U;
}

auto form::internal::lenComp(LENINFO const& arg1, LENINFO const& arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (arg1.length < arg2.length) {
	return true;
  }
  if (arg2.length < arg1.length) {
	return false;
  }

  return false;
}

void form::internal::chksid(FRMHED const&              form,
                            uint32_t                   vertexIndex,
                            uint32_t                   clipIntersectSide,
                            std::vector<fPOINT> const& currentFormVertices) {
  if (clipIntersectSide != vertexIndex) {
	auto const vertexIt    = currentFormVertices.cbegin();
	auto const vertexCount = currentFormVertices.size();
	if ((vertexIndex - clipIntersectSide + vertexCount) % vertexCount < (vertexCount / 2)) {
	  auto       iVertex = form::nxt(form, clipIntersectSide);
	  auto const limit   = form::nxt(form, vertexIndex);
	  while (iVertex != limit) {
		auto const vIt = wrap::next(vertexIt, iVertex);
		OSequence->push_back(*vIt);
		iVertex = form::nxt(form, iVertex);
	  }
	}
	else {
	  auto iVertex = clipIntersectSide;
	  while (iVertex != vertexIndex) {
		auto const vIt = wrap::next(vertexIt, iVertex);
		OSequence->push_back(*vIt);
		iVertex = form::prv(form, iVertex);
	  }
	}
  }
}

void form::internal::ritseg(FRMHED const&              form,
                            std::vector<CLIPNT> const& clipStitchPoints,
                            std::vector<CLPSEG>&       clipSegments,
                            uint32_t const             currentSegmentIndex,
                            uint32_t&                  clipIntersectSide,
                            std::vector<fPOINT> const& currentFormVertices) {
  auto isPointedEnd = true;
  if ((form.extendedAttribute & AT_SQR) != 0U) {
	isPointedEnd = false;
  }
  if (StateMap->test(StateFlag::FILDIR)) {
	auto iPoint = clipSegments[currentSegmentIndex].start;
	if (StateMap->test(StateFlag::TXFIL) && isPointedEnd) {
	  ++iPoint;
	}
	chksid(form, clipSegments[currentSegmentIndex].asid, clipIntersectSide, currentFormVertices);
	while (iPoint <= clipSegments[currentSegmentIndex].finish) {
	  OSequence->push_back(fPOINT {clipStitchPoints[iPoint].x, clipStitchPoints[iPoint++].y});
	}
	clipIntersectSide = clipSegments[currentSegmentIndex].zsid;
  }
  else {
	auto iPoint = clipSegments[currentSegmentIndex].finish;
	if (StateMap->test(StateFlag::TXFIL) && isPointedEnd) {
	  --iPoint;
	}
	chksid(form, clipSegments[currentSegmentIndex].zsid, clipIntersectSide, currentFormVertices);
	if (clipSegments[currentSegmentIndex].start != 0U) {
	  while (iPoint >= clipSegments[currentSegmentIndex].start) {
		OSequence->push_back(fPOINT {clipStitchPoints[iPoint].x, clipStitchPoints[iPoint--].y});
	  }
	}
	else {
	  while (iPoint < clipSegments[currentSegmentIndex].start) {
		OSequence->push_back(fPOINT {clipStitchPoints[iPoint].x, clipStitchPoints[iPoint--].y});
	  }
	}
	clipIntersectSide = clipSegments[currentSegmentIndex].asid;
  }
  clipSegments[currentSegmentIndex].dun = 1;
}

auto form::internal::clpnxt(std::vector<CLPSEG> const&  clipSegments,
                            std::vector<LENINFO> const& sortedLengths,
                            uint32_t                    sind,
                            uint32_t&                   outIndex) -> bool {
  auto       index        = 1U;
  auto const indexDoubled = wrap::toUnsigned(clipSegments.size()) * 2U;
  StateMap->reset(StateFlag::FILDIR);
  while (index < clipSegments.size()) {
	if (StateMap->testAndFlip(StateFlag::FILDIR)) {
	  outIndex = (sind + index) % indexDoubled;
	  if (clipSegments[sortedLengths[outIndex].index].dun == 0) {
		return false;
	  }
	  ++index;
	}
	else {
	  outIndex = (sind + indexDoubled - index) % indexDoubled;
	  if (clipSegments[sortedLengths[outIndex].index].dun == 0) {
		return false;
	  }
	}
  }
  return true;
}

auto form::internal::nucseg(std::vector<CLPSEG> const&  clipSegments,
                            std::vector<LENINFO> const& sortedLengths,
                            uint32_t&                   currentSegmentIndex) -> bool {
  auto const index = (StateMap->test(StateFlag::FILDIR)) ? clipSegments[currentSegmentIndex].endIndex
                                                         : clipSegments[currentSegmentIndex].beginIndex;
  auto outIndex = 0U;
  if (clpnxt(clipSegments, sortedLengths, index, outIndex)) {
	return false;
  }
  if (sortedLengths[outIndex].isEnd) {
	StateMap->reset(StateFlag::FILDIR);
  }
  else {
	StateMap->set(StateFlag::FILDIR);
  }
  currentSegmentIndex = sortedLengths[outIndex].index;
  return true;
}

auto form::internal::vscmp(uint32_t index1, uint32_t index2) noexcept -> bool {
  if (OSequence->operator[](index1).x != OSequence->operator[](index2).x) {
	return true;
  }
  return OSequence->operator[](index1).y != OSequence->operator[](index2).y;
}

void form::internal::clpcon(FRMHED& form, std::vector<RNGCNT> const& textureSegments, std::vector<fPOINT>& currentFormVertices) {
  auto const currentVertexCount = currentFormVertices.size();
  auto       formOffset         = 0.0F;
  duflt(formOffset, currentFormVertices);
  auto clipWidth = ClipRectSize.cx + form.fillSpacing;
  if (StateMap->test(StateFlag::ISUND)) {
	clipWidth = form.underlaySpacing;
  }
  auto clipNegative = false;
  if (form.fillSpacing < 0) {
	clipNegative = true;
  }
  if (clipWidth < CLPMINVT) {
	clipWidth = CLPMINVT;
  }
  if (StateMap->test(StateFlag::TXFIL)) {
	if ((!TexturePointsBuffer->empty()) &&
	    form.fillInfo.texture.index + form.fillInfo.texture.count <=
	        gsl::narrow<decltype(form.fillInfo.texture.index)>(TexturePointsBuffer->size())) {
	  clipWidth = form.fillSpacing;
	}
	else {
	  return;
	}
  }
  auto lengths = std::vector<float> {};
  lengths.resize(currentVertexCount);
  auto clipSideLengths = std::vector<float> {}; // lengths of form sides for clipboard fill
  clipSideLengths.resize(currentVertexCount);
  auto clipIntersectData = std::vector<CLIPSORT> {}; // intersect points for clipboard fill
  clipIntersectData.resize(currentVertexCount);
  auto arrayOfClipIntersectData = std::vector<CLIPSORT*> {};
  arrayOfClipIntersectData.reserve(currentVertexCount);
  auto vertex      = leftsid(currentFormVertices);
  auto totalLength = 0.0F;
  vertex           = form::nxt(form, vertex);
  auto vBegin      = currentFormVertices.begin();
  for (auto iVertex = 0U; iVertex <= currentVertexCount; ++iVertex) {
	auto const vertIt       = wrap::next(vBegin, vertex);
	auto const nextVertex   = form::nxt(form, vertex);
	auto const nVertIt      = wrap::next(vBegin, nextVertex);
	lengths[vertex]         = totalLength;
	clipSideLengths[vertex] = hypot(nVertIt->x - vertIt->x, nVertIt->y - vertIt->y);
	totalLength += clipSideLengths[vertex];
	vertex = nextVertex;
  }
  auto vertexIt     = vBegin;
  auto boundingRect = fRECTANGLE {vertexIt->x, vertexIt->y, vertexIt->x, vertexIt->y};
  ++vertexIt;
  for (auto iVertex = 1U; iVertex < currentVertexCount; ++iVertex) {
	if (vertexIt->x < boundingRect.left) {
	  boundingRect.left = vertexIt->x;
	}
	if (vertexIt->y > boundingRect.top) {
	  boundingRect.top = vertexIt->y;
	}
	if (vertexIt->x > boundingRect.right) {
	  boundingRect.right = vertexIt->x;
	}
	if (vertexIt->y < boundingRect.bottom) {
	  boundingRect.bottom = vertexIt->y;
	}
	++vertexIt;
  }
  auto clipGrid = RECT {wrap::floor<int32_t>(boundingRect.left / clipWidth),
                        wrap::ceil<int32_t>(boundingRect.top / ClipRectSize.cy + 1.0F) + 2,
                        wrap::ceil<int32_t>(boundingRect.right / clipWidth),
                        wrap::floor<int32_t>(boundingRect.bottom / ClipRectSize.cy - 1.0F)};

  auto negativeOffset = 0L;
  auto clipGridOffset = 0U;
  if (form.wordParam > 1) {
	clipGridOffset = form.wordParam;
  }
  if (clipGridOffset != 0U) {
	++(clipGrid.top);
	if (form.fillSpacing < 0) {
	  --(clipGrid.bottom);
	  clipGrid.left -= std::lround(ClipRectSize.cx / clipWidth);
	  clipGrid.right += std::lround(ClipRectSize.cx / clipWidth);
	}
  }
  if (clipNegative && (clipGridOffset == 0U)) {
	clipGrid.left -= std::lround(ClipRectSize.cx / clipWidth);
  }
  if (clipGrid.bottom < 0) {
	negativeOffset = 1 - clipGrid.bottom;
	clipGrid.bottom += negativeOffset;
	clipGrid.top += negativeOffset;
	auto const formNegativeOffset = ClipRectSize.cy * wrap::toFloat(negativeOffset);
	auto       vertIt             = vBegin;
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vertIt->y += formNegativeOffset;
	  ++vertIt;
	}
	boundingRect.top += formNegativeOffset;
	boundingRect.bottom += formNegativeOffset;
  }
  auto regionCrossingData = std::vector<VCLPX> {}; // region crossing data for vertical clipboard fills
  // reserve a little more than we need. Determined empirically
  regionCrossingData.reserve(wrap::toSize(clipGrid.right - clipGrid.left) * 5U);
  auto vCurr = vBegin;
  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	auto const vNext  = wrap::next(vBegin, form::nxt(form, iVertex));
	auto       start  = wrap::floor<uint32_t>(vCurr->x / clipWidth);
	auto       finish = wrap::floor<uint32_t>(vNext->x / clipWidth);
	++vCurr;
	if (start > finish) {
	  std::swap(start, finish);
	}
	if (form.fillSpacing < 0) {
	  finish += wrap::round<decltype(finish)>(ClipRectSize.cx / clipWidth);
	}
	auto const right = gsl::narrow<uint32_t>(clipGrid.right);
	if (finish > right) {
	  finish = right;
	}
	if (clipNegative) {
	  start -= wrap::round<uint32_t>(ClipRectSize.cx / clipWidth);
	}
	for (auto iSegment = start; iSegment <= finish; ++iSegment) {
	  regionCrossingData.push_back(VCLPX {iSegment, iVertex});
	}
  }
  std::sort(regionCrossingData.begin(), regionCrossingData.end(), fi::vclpComp);
  auto iclpx = std::vector<uint32_t> {};
  iclpx.reserve(regionCrossingData.size());
  auto regionSegment = regionCrossingData[0].segment;
  iclpx.push_back(0);
  for (auto iSegment = 1U; iSegment < wrap::toUnsigned(regionCrossingData.size()); ++iSegment) {
	if (regionCrossingData[iSegment].segment != regionSegment) {
	  iclpx.push_back(iSegment);
	  regionSegment = regionCrossingData[iSegment].segment;
	}
  }
  iclpx.push_back(wrap::toUnsigned(regionCrossingData.size()));
  auto clipStitchPoints = std::vector<CLIPNT> {};
  // Reserve some memory, but probably not enough
  clipStitchPoints.reserve(1000U);
  auto pasteLocation = fPOINT {};
  auto texture       = TexturePointsBuffer->begin();
  auto iclpxSize     = wrap::toUnsigned(iclpx.size());
  if (iclpxSize != 0U) {
	--iclpxSize;
	bool breakFlag = false;
	for (auto iRegion = 0U; iRegion < iclpxSize; ++iRegion) {
	  auto regionCrossingStart = iclpx[iRegion];
	  auto regionCrossingEnd   = iclpx[wrap::toSize(iRegion) + 1U];
	  pasteLocation.x = clipWidth * (wrap::toFloat(iRegion) + wrap::toFloat(clipGrid.left));
	  auto  clipVerticalOffset = 0.0F;
	  auto  lineSegmentStart   = fPOINT {}; // vertical clipboard line segment start
	  auto  lineSegmentEnd     = fPOINT {}; // vertical clipboard line segment end
	  auto& clipBuffer         = *ClipBuffer;
	  auto  clipStitchCount    = wrap::toUnsigned(ClipBuffer->size());
	  if (StateMap->test(StateFlag::TXFIL)) {
		auto const textureLine =
		    (iRegion + wrap::toUnsigned(clipGrid.left)) % wrap::toUnsigned(form.fillInfo.texture.lines);
		clipStitchCount = wrap::toUnsigned(textureSegments[textureLine].stitchCount);

		texture = wrap::next(TexturePointsBuffer->begin(),
		                     form.fillInfo.texture.index + textureSegments[textureLine].line);

		lineSegmentStart.x = pasteLocation.x;
		if (form.txof != 0.0F) {
		  auto const lineOffset =
		      (gsl::narrow_cast<float>(iRegion) + gsl::narrow_cast<float>(clipGrid.left)) /
		      gsl::narrow_cast<float>(form.fillInfo.texture.lines);
		  clipVerticalOffset = fmod(form.txof * lineOffset, form.fillInfo.texture.height);
		}
	  }
	  else {
		if (clipGridOffset != 0U) {
		  clipVerticalOffset = wrap::toFloat(iRegion % clipGridOffset) /
		                       (wrap::toFloat(clipGridOffset) * ClipRectSize.cy);
		}
		lineSegmentStart.x = pasteLocation.x + clipBuffer[0].x;
	  }
	  lineSegmentStart.y = wrap::toFloat(clipGrid.bottom) * ClipRectSize.cy;
	  if (clipGridOffset != 0U) {
		clipVerticalOffset =
		    wrap::toFloat(iRegion % clipGridOffset) / (wrap::toFloat(clipGridOffset) * ClipRectSize.cy);
	  }
	  for (auto iVerticalGrid = clipGrid.bottom; iVerticalGrid < clipGrid.top; ++iVerticalGrid) {
		pasteLocation.y = wrap::toFloat(iVerticalGrid) * ClipRectSize.cy - clipVerticalOffset;
		if (!clipBuffer.empty()) {
		  lineSegmentEnd.x = pasteLocation.x + clipBuffer[0].x;
		  lineSegmentEnd.y = pasteLocation.y + clipBuffer[0].y;
		}
		else {
		  lineSegmentEnd = pasteLocation;
		}
		if (clipStitchPoints.empty()) {
		  lineSegmentStart = lineSegmentEnd;
		}
		auto textureIt = texture;
		for (auto iStitch = 0U; iStitch < clipStitchCount; ++iStitch) {
		  if (StateMap->test(StateFlag::TXFIL)) {
			lineSegmentEnd = fPOINT {pasteLocation.x, pasteLocation.y + textureIt->y};
			++textureIt;
		  }
		  else {
			lineSegmentEnd = fPOINT {pasteLocation.x + clipBuffer[iStitch].x,
			                         pasteLocation.y + clipBuffer[iStitch].y};
		  }

		  clipStitchPoints.push_back(CLIPNT {lineSegmentStart.x, lineSegmentStart.y, 0, 0});
		  if (isin(form, regionCrossingData, lineSegmentStart.x, lineSegmentStart.y, regionCrossingStart, regionCrossingEnd, boundingRect, currentFormVertices)) {
			if ((clipStitchPoints.size() > 1) && clipStitchPoints[clipStitchPoints.size() - 1U].flag == 2) {
			  inspnt(clipStitchPoints);
			}
			clipStitchPoints.back().flag = 0;
		  }
		  else {
			if ((clipStitchPoints.size() > 1) && (clipStitchPoints[clipStitchPoints.size() - 1U].flag == 0U)) {
			  inspnt(clipStitchPoints);
			}
			clipStitchPoints.back().flag = 2;
		  }
		  auto const count = insect(form,
		                            clipIntersectData,
		                            regionCrossingData,
		                            arrayOfClipIntersectData,
		                            regionCrossingStart,
		                            regionCrossingEnd,
		                            lineSegmentStart,
		                            lineSegmentEnd,
		                            currentFormVertices);
		  if (count != 0U) {
			for (auto index = 0U; index < count; ++index) {
			  clipStitchPoints.push_back({arrayOfClipIntersectData[index]->point.x,
			                              arrayOfClipIntersectData[index]->point.y,
			                              arrayOfClipIntersectData[index]->vertexIndex,
			                              1});
			  if (clipStitchPoints.size() > MAXITEMS << 2U) {
				breakFlag = true;
				break;
			  }
			}
			if (breakFlag) {
			  break;
			}
		  }
		  lineSegmentStart = lineSegmentEnd;
		}
		if (breakFlag) {
		  break;
		}
	  }
	  if (!breakFlag) {
		if (!clipStitchPoints.empty()) {
		  clipStitchPoints[clipStitchPoints.size() - 1U].flag = 2;
		}
	  }
	  else {
		break;
	  }
	}
  }
  if (!clipStitchPoints.empty()) {
	clipStitchPoints[clipStitchPoints.size() - 1U].flag = 2;
  }
  if (negativeOffset != 0) {
	auto const formNegativeOffset = wrap::toFloat(negativeOffset) * ClipRectSize.cy;
	for (auto& clipStitchPoint : clipStitchPoints) {
	  clipStitchPoint.y -= formNegativeOffset;
	}
	auto vOffset = vBegin;
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vOffset->y -= formNegativeOffset;
	  ++vOffset;
	}
  }

#if CLPVU == 0
  StateMap->reset(StateFlag::FILDIR);
  auto clipSegments = std::vector<CLPSEG> {};
  auto endPoint     = wrap::toUnsigned(clipStitchPoints.size());
  if (endPoint != 0U) {
	--endPoint;
	// reserve a reasonable amount but not the full amount potentially required
	clipSegments.reserve(endPoint / 10U);
	auto previousPoint = 0U;
	for (auto iPoint = 0U; iPoint < endPoint; ++iPoint) {
	  switch (clipStitchPoints[iPoint].flag) {
		case 0: { // inside
		  StateMap->set(StateFlag::FILDIR);
		  break;
		}
		case 1: { // line
		  if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			clpnseg(clipStitchPoints, clipSegments, lengths, previousPoint, iPoint, currentFormVertices);
		  }
		  else {
			previousPoint = iPoint;
		  }
		  break;
		}
		case 2: { // outside
		  StateMap->reset(StateFlag::FILDIR);
		  break;
		}
		default: {
		  outDebugString(L"default hit in clpcon: flag [{}]\n", clipStitchPoints[iPoint].flag);
		  break;
		}
	  }
	}
  }

#endif

  if (!clipSegments.empty()) {
	auto sortedLengths = std::vector<LENINFO> {};
	sortedLengths.reserve(clipSegments.size() * 2);
	for (auto iSegment = 0U; iSegment < wrap::toUnsigned(clipSegments.size()); ++iSegment) {
	  sortedLengths.push_back(LENINFO {iSegment, false, clipSegments[iSegment].beginLength});
	  sortedLengths.push_back(LENINFO {iSegment, true, clipSegments[iSegment].endLength});
	}
	std::sort(sortedLengths.begin(), sortedLengths.end(), fi::lenComp);
	for (auto iSorted = 0U; iSorted < wrap::toUnsigned(sortedLengths.size()); ++iSorted) {
	  if (sortedLengths[iSorted].isEnd) {
		clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
	  }
	  else {
		clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
	  }
	}

#if CLPVU == 1

	for (uint32_t iStitch = 0; iStitch < ActivePointIndex; ++iStitch) {
	  StitchBuffer->operator[](iStitch) =
	      fPOINTATTR {ClipStitchPoints[iStitch].x, ClipStitchPoints[iStitch].y, 0};
	}
	PCSHeader.stitchCount = ActivePointIndex;
#endif

#if CLPVU == 2

	auto iStitch = 0;
	for (iSegment = 0; iSegment < ClipSegmentIndex; ++iSegment) {
	  for (iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish;
	       ++iStitchPoint) {
		StitchBuffer->operator[](iStitch) =
		    fPOINTATTR {ClipStitchPoints[iStitchPoint].x, ClipStitchPoints[iStitchPoint].y, iSegment & 0xf};
	  }
	}
	PCSHeader.stitchCount = iStitch;

#endif

#if CLPVU == 0

	auto currentSegmentIndex = 0U;
	StateMap->set(StateFlag::FILDIR);
	OSequence->clear();
	auto clipIntersectSide = clipSegments[0].asid;
	ritseg(form, clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
	while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
	  ritseg(form, clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
	}
	chksid(form, 0, clipIntersectSide, currentFormVertices);
	auto index = 0U;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size()); ++iSequence) {
	  if (vscmp(iSequence, index)) {
		++index;
		OSequence->operator[](index) = OSequence->operator[](iSequence);
	  }
	}
	OSequence->resize(index);
	if (StateMap->test(StateFlag::WASNEG)) {
	  for (auto& iSequence : *OSequence) {
		iSequence.x -= formOffset;
	  }
	  auto vOffset = vBegin;
	  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
		vOffset->x -= formOffset;
		++vOffset;
	  }
	  form.rectangle.left -= formOffset;
	  form.rectangle.right -= formOffset;
	}
#endif
  }
}

void form::internal::angout(FRMHED& angledForm) {
  if (angledForm.vertexCount != 0U) {
	auto* rectangle = &angledForm.rectangle;
	auto  vertexIt  = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
	rectangle->left = rectangle->right = vertexIt->x;
	rectangle->bottom = rectangle->top = vertexIt->y;
	++vertexIt;
	for (auto iVertex = 1U; iVertex < angledForm.vertexCount; ++iVertex) {
	  if (vertexIt->x > rectangle->right) {
		rectangle->right = vertexIt->x;
	  }
	  if (vertexIt->x < rectangle->left) {
		rectangle->left = vertexIt->x;
	  }
	  if (vertexIt->y < rectangle->bottom) {
		rectangle->bottom = vertexIt->y;
	  }
	  if (vertexIt->y > rectangle->top) {
		rectangle->top = vertexIt->y;
	  }
	  ++vertexIt;
	}
  }
}

void form::internal::horclpfn(std::vector<RNGCNT> const& textureSegments,
                              FRMHED&                    angledForm,
                              std::vector<fPOINT>&       angledFormVertices) {
  angledForm                = FormList->operator[](ClosestFormToCursor);
  auto const rotationCenter = fPOINT {wrap::midl(angledForm.rectangle.right, angledForm.rectangle.left),
                                      wrap::midl(angledForm.rectangle.top, angledForm.rectangle.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto vertexIt = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*vertexIt);
	thred::rotflt(angledFormVertices.back(), (PI_FHALF), rotationCenter);
	++vertexIt;
  }
  angledForm.vertexIndex = 0;
  angout(angledForm);
  clpcon(angledForm, textureSegments, angledFormVertices);
  rotbak((-PI_FHALF), rotationCenter);
}

void form::angclpfn(FRMHED const& form, std::vector<RNGCNT> const& textureSegments, std::vector<fPOINT>& angledFormVertices) {
  auto       angledForm    = form;
  auto const rotationAngle = StateMap->test(StateFlag::ISUND)
                                 ? PI_FHALF - angledForm.underlayStitchAngle
                                 : StateMap->test(StateFlag::TXFIL)
                                       ? PI_FHALF - angledForm.angleOrClipData.angle
                                       : PI_FHALF - angledForm.satinOrAngle.angle;
  auto const rotationCenter = fPOINT {wrap::midl(angledForm.rectangle.right, angledForm.rectangle.left),
                                      wrap::midl(angledForm.rectangle.top, angledForm.rectangle.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  if (StateMap->test(StateFlag::ISUND)) {
	auto const& vertexList = xt::insid(angledForm);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	  angledFormVertices.push_back(vertexList[iVertex]);
	  thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	}
  }
  else {
	auto vertexIt = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	  angledFormVertices.push_back(*vertexIt);
	  thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	  ++vertexIt;
	}
  }
  angledForm.vertexIndex = 0;
  fi::angout(angledForm);
  fi::clpcon(angledForm, textureSegments, angledFormVertices);
  fi::rotbak(-rotationAngle, rotationCenter);
}

auto form::internal::spComp(SMALPNTL const* const arg1, SMALPNTL const* const arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if ((arg1 != nullptr) && (arg2 != nullptr)) {
	auto const& lineEnd1 = *arg1;
	auto const& lineEnd2 = *arg2;
	if (lineEnd1.line < lineEnd2.line) {
	  return true;
	}
	if (lineEnd2.line < lineEnd1.line) {
	  return false;
	}
	if (lineEnd1.group < lineEnd2.group) {
	  return true;
	}
	if (lineEnd2.group < lineEnd1.group) {
	  return false;
	}
	if (lineEnd1.y < lineEnd2.y) {
	  return true;
	}
	if (lineEnd2.y < lineEnd1.y) {
	  return false;
	}
  }

  return false;
}

auto form::internal::isclos(SMALPNTL const* const lineEndPoint0,
                            SMALPNTL const* const lineEndPoint1,
                            float                 gapToClosestRegion) noexcept -> bool {
  if ((lineEndPoint0 != nullptr) && (lineEndPoint1 != nullptr)) {
	auto const high0 = lineEndPoint0[1].y + gapToClosestRegion;
	auto const low1  = lineEndPoint1[0].y - gapToClosestRegion;
	if (high0 < low1) {
	  return false;
	}
	auto const high1 = lineEndPoint1[1].y + gapToClosestRegion;
	auto const low0  = lineEndPoint0[0].y - gapToClosestRegion;
	if (high1 < low0) {
	  return false;
	}
  }
  return true;
}

auto form::internal::lnclos(std::vector<uint32_t>& groupIndexSequence,
                            std::vector<SMALPNTL>& lineEndpoints,
                            uint32_t               group0,
                            uint32_t               line0,
                            uint32_t               group1,
                            uint32_t               line1,
                            float                  gapToClosestRegion) noexcept -> bool {
  auto const* lineEndPoint0 = &lineEndpoints[groupIndexSequence[group0]];
  if (group1 > groupIndexSequence.size() - 2U) {
	return false;
  }
  if (group0 == 0) {
	return false;
  }
  if (lineEndPoint0 != nullptr) {
	auto count0 = (groupIndexSequence[wrap::toSize(group0) + 1U] - groupIndexSequence[group0]) / 2U;
	auto index0 = 0U;
	while ((count0 != 0U) && lineEndPoint0[index0].line != line0) {
	  --count0;
	  index0 += 2;
	}
	if (count0 != 0U) {
	  auto count1 = (groupIndexSequence[wrap::toSize(group1) + 1U] - groupIndexSequence[group1]) / 2U;
	  if (auto const* lineEndPoint1 = &lineEndpoints[groupIndexSequence[group1]]) {
		auto index1 = 0U;
		while ((count1 != 0U) && lineEndPoint1[index1].line != line1) {
		  --count1;
		  index1 += 2;
		}
		if (count1 != 0U) {
		  return isclos(&lineEndPoint0[index0], &lineEndPoint1[index1], gapToClosestRegion);
		}
	  }
	}
  }
  return false;
}

auto form::internal::regclos(std::vector<uint32_t>&        groupIndexSequence,
                             std::vector<SMALPNTL>&        lineEndpoints,
                             std::vector<SMALPNTL*> const& sortedLines,
                             uint32_t                      iRegion0,
                             uint32_t                      iRegion1,
                             std::vector<REGION> const&    regionsList,
                             float                         gapToClosestRegion,
                             uint32_t&                     nextGroup) noexcept -> bool {
  auto const* lineEndPoint0Start = sortedLines[regionsList[iRegion0].start];
  auto const* lineEndPoint1Start = sortedLines[regionsList[iRegion1].start];
  auto const  group0Start        = lineEndPoint0Start->group;
  auto const  group1Start        = lineEndPoint1Start->group;
  auto const  startFlag          = group0Start > group1Start;
  auto const  groupStart         = startFlag ? group0Start : group1Start;
  auto const  lineStart          = startFlag ? lineEndPoint0Start->line : lineEndPoint1Start->line;
  auto const  prevLine           = startFlag ? lineEndPoint1Start->line : lineEndPoint0Start->line;
  if ((groupStart != 0U) &&
      lnclos(groupIndexSequence, lineEndpoints, groupStart - 1, prevLine, groupStart, lineStart, gapToClosestRegion)) {
	nextGroup = groupStart;
	return true;
  }
  auto const* lineEndPoint0End = sortedLines[regionsList[iRegion0].end];
  auto const* lineEndPoint1End = sortedLines[regionsList[iRegion1].end];
  auto const  group1End        = lineEndPoint1End->group;
  auto const  group0End        = lineEndPoint0End->group;
  auto const  endFlag          = group0End < group1End;
  auto const  groupEnd         = endFlag ? group0End : group1End;
  auto const  lineEnd          = endFlag ? lineEndPoint0End->line : lineEndPoint1End->line;
  auto const  lastLine         = endFlag ? lineEndPoint1End->line : lineEndPoint0End->line;
  if (lnclos(groupIndexSequence, lineEndpoints, groupEnd, lineEnd, groupEnd + 1, lastLine, gapToClosestRegion)) {
	nextGroup = groupEnd;
	return true;
  }
  if (((group0Start > group1Start) ? (group0Start - group1Start) : (group1Start - group0Start)) < 2) {
	if (isclos(lineEndPoint0Start, lineEndPoint1Start, gapToClosestRegion)) {
	  nextGroup = group0Start;
	  return true;
	}
  }
  if (((group0Start > group1End) ? (group0Start - group1End) : (group1End - group0Start)) < 2) {
	if (isclos(lineEndPoint0Start, lineEndPoint1End, gapToClosestRegion)) {
	  nextGroup = group0Start;
	  return true;
	}
  }
  if (((group0End > group1Start) ? (group0End - group1Start) : (group1Start - group0End)) < 2) {
	if (isclos(lineEndPoint0End, lineEndPoint1Start, gapToClosestRegion)) {
	  nextGroup = group0End;
	  return true;
	}
  }
  if (((group0End > group1End) ? (group0End - group1End) : (group1End - group0End)) < 2) {
	if (isclos(lineEndPoint0End, lineEndPoint1End, gapToClosestRegion)) {
	  nextGroup = group0End;
	  return true;
	}
  }
  return false;
}

auto form::internal::unvis(boost::dynamic_bitset<> const& visitedRegions, int32_t& visitedIndex) -> bool {
  auto const regionCount = gsl::narrow<int32_t>(visitedRegions.size());
  for (visitedIndex = 0; visitedIndex < regionCount; ++visitedIndex) {
	if (!visitedRegions[wrap::toSize(visitedIndex)]) {
	  return true;
	}
  }
  return false;
}

auto form::internal::notdun(std::vector<RGSEQ>&            tempPath,
                            std::vector<RCON> const&       pathMap,
                            std::vector<uint32_t> const&   mapIndexSequence,
                            boost::dynamic_bitset<> const& visitedRegions,
                            ptrdiff_t                      pathLength,
                            uint32_t                       doneRegion,
                            uint32_t                       sequencePathIndex) -> bool {
  auto previousLevel = pathLength;
  if (previousLevel != 0U) {
	--previousLevel;
  }
  auto const regionPath = wrap::next(tempPath.begin(), sequencePathIndex);
  regionPath[0].pcon    = mapIndexSequence[doneRegion];
  regionPath[0].count   = gsl::narrow<int32_t>(
      mapIndexSequence[gsl::narrow_cast<size_t>(doneRegion) + 1U] - regionPath[0].pcon);
  for (auto iPath = ptrdiff_t {1}; iPath < pathLength; ++iPath) {
	regionPath[iPath].pcon  = mapIndexSequence[pathMap[regionPath[iPath - 1].pcon].node];
	regionPath[iPath].count = gsl::narrow<int32_t>(
	    mapIndexSequence[gsl::narrow_cast<size_t>(pathMap[regionPath[iPath - 1].pcon].node) + 1] -
	    regionPath[iPath].pcon);
  }
  while (visitedRegions[pathMap[regionPath[previousLevel].pcon].node]) {
	if (--regionPath[previousLevel].count > 0) {
	  ++(regionPath[previousLevel].pcon);
	}
	else {
	  auto pivot = previousLevel;
	  do {
		if (pivot != 0U) {
		  --pivot;
		}
		else {
		  return true;
		}
		--(regionPath[pivot].count);
		++(regionPath[pivot].pcon);
	  } while (regionPath[pivot].count == 0);
	  ++pivot;
	  while (pivot <= previousLevel) {
		if (pivot != 0U) {
		  regionPath[pivot].pcon  = mapIndexSequence[pathMap[regionPath[pivot - 1].pcon].node];
		  regionPath[pivot].count = gsl::narrow<int32_t>(
		      mapIndexSequence[wrap::toSize(pathMap[regionPath[pivot - 1].pcon].node) + 1U] -
		      regionPath[pivot].pcon);
		}
		else {
		  if (--regionPath[0].count != 0) {
			++(regionPath[0].pcon);
		  }
		  else {
			return true;
		  }
		}
		++pivot;
	  }
	}
  }
  return false;
}

auto form::internal::reglen(std::vector<SMALPNTL*> const&       sortedLines,
                            uint32_t                            iRegion,
                            std::array<fPOINT, SQRCORNS> const& lastRegionCorners,
                            std::vector<REGION> const&          regionsList) noexcept -> float {
  auto lineEndPoints = std::array<SMALPNTL*, SQRCORNS> {};
  lineEndPoints[0]   = sortedLines[regionsList[iRegion].start];
  lineEndPoints[1]   = &sortedLines[regionsList[iRegion].start][1];
  lineEndPoints[2]   = sortedLines[regionsList[iRegion].end];
  lineEndPoints[3]   = &sortedLines[regionsList[iRegion].end][1];
  auto minimumLength = BIGFLOAT;
  for (auto const& corner : lastRegionCorners) {
	for (auto const& point : lineEndPoints) {
	  auto const length = hypot(corner.x - point->x, corner.y - point->y);
	  if (length < minimumLength) {
		minimumLength = length;
	  }
	}
  }
  return minimumLength;
}

void form::internal::nxtrgn(std::vector<RGSEQ>&           tempPath,
                            std::vector<RCON> const&      pathMap,
                            std::vector<uint32_t> const&  mapIndexSequence,
                            boost::dynamic_bitset<>&      visitedRegions,
                            std::vector<SMALPNTL*> const& sortedLines,
                            std::vector<REGION> const&    regionsList,
                            uint32_t&                     doneRegion,
                            uint32_t                      pathMapIndex,
                            uint32_t&                     sequencePathIndex,
                            int32_t                       visitedIndex) {
  auto lastRegionCorners = std::array<fPOINT, SQRCORNS> {};
  auto pathLength        = ptrdiff_t {1}; // length of the path to the region
  while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength, doneRegion, sequencePathIndex)) {
	++pathLength;
	auto maxPathLength = gsl::narrow<ptrdiff_t>(tempPath.size() - sequencePathIndex);

	constexpr auto DEPFACT = 8; // tuneable value. Increasing this increases path discovery time exponentially
	if (maxPathLength > DEPFACT) {
	  maxPathLength = DEPFACT;
	}
	outDebugString(L"nxtrgn: pathLength {}\n", pathLength);
	if (pathLength > maxPathLength) {
	  auto* lineEndPoint = sortedLines[regionsList[doneRegion].start];
	  if (lineEndPoint != nullptr) {
		lastRegionCorners[0] = lineEndPoint[0];
		lastRegionCorners[1] = lineEndPoint[1];
	  }
	  lineEndPoint = sortedLines[regionsList[doneRegion].end];
	  if (lineEndPoint != nullptr) {
		lastRegionCorners[2] = lineEndPoint[0];
		lastRegionCorners[3] = lineEndPoint[1];
	  }
	  auto       newRegion     = 0U;
	  auto       minimumLength = BIGFLOAT;
	  auto const regionCount   = visitedRegions.size();
	  for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
		if (!visitedRegions[iRegion]) {
		  auto const length = reglen(sortedLines, iRegion, lastRegionCorners, regionsList);
		  if (length < minimumLength) {
			minimumLength = length;
			newRegion     = iRegion;
		  }
		}
	  }
	  tempPath[sequencePathIndex].skp = true;
	  for (auto iPath = 0U; iPath < pathMapIndex; ++iPath) {
		if (pathMap[iPath].node == newRegion) {
		  tempPath[sequencePathIndex++].pcon = iPath;
		  visitedRegions.set(newRegion);
		  doneRegion = newRegion;
		  return;
		}
	  }
	  tempPath[sequencePathIndex].count  = visitedIndex;
	  tempPath[sequencePathIndex++].pcon = MAXDWORD;
	  visitedRegions.set(wrap::toSize(visitedIndex));
	  doneRegion = wrap::toUnsigned(visitedIndex);
	  return;
	}
  }
  auto regionPath = wrap::next(tempPath.cbegin(), sequencePathIndex);
  for (auto iPath = ptrdiff_t {0}; iPath < pathLength; ++iPath) {
	tempPath[sequencePathIndex].skp    = false;
	tempPath[sequencePathIndex++].pcon = regionPath->pcon;
	visitedRegions.set(pathMap[regionPath->pcon].node);
	++regionPath;
  }
  --regionPath;
  doneRegion = pathMap[regionPath->pcon].node;
}

void form::internal::nxtseq(std::vector<FSEQ>&           sequencePath,
                            std::vector<RCON> const&     pathMap,
                            std::vector<uint32_t> const& mapIndexSequence,
                            uint32_t                     pathIndex) {
  if ((wrap::toSize(pathIndex) + 1U) < sequencePath.size()) {
	unsigned const nextNode = sequencePath[wrap::toSize(pathIndex) + 1U].node;
	unsigned       iPath    = mapIndexSequence[sequencePath[pathIndex].node];
	while (iPath < mapIndexSequence[wrap::toSize(sequencePath[pathIndex].node) + 1U] &&
	       pathMap[iPath].node != nextNode) {
	  ++iPath;
	}
	if (iPath < pathMap.size()) {
	  sequencePath[pathIndex].nextGroup = gsl::narrow<uint16_t>(pathMap[iPath].nextGroup);
	}
	else {
	  sequencePath[pathIndex].nextGroup = 0;
	}
  }
  else {
	sequencePath[pathIndex].nextGroup = 0;
  }
}

void form::internal::brkdun(std::vector<SMALPNTL*> const& sortedLines,
                            uint32_t                      start,
                            uint32_t                      finish,
                            std::vector<fPOINT>&          workingFormVertices) {
  BSequence->emplace_back(sortedLines[start]->x, sortedLines[start]->y, 0);
  BSequence->emplace_back(sortedLines[finish]->x, sortedLines[finish]->y, 0);
  BSequence->emplace_back(workingFormVertices.operator[](sortedLines[start]->line).x,
                          workingFormVertices.operator[](sortedLines[start]->line).y,
                          0);
  StateMap->set(StateFlag::BRKFIX);
}

void form::internal::duseq1(SMALPNTL const* sequenceLines) {
  if (sequenceLines != nullptr) {
	BSequence->emplace_back(wrap::midl(sequenceLines[1].x, sequenceLines[0].x),
	                        wrap::midl(sequenceLines[1].y, sequenceLines[0].y),
	                        0);
  }
}

void form::internal::movseq(std::vector<SMALPNTL*> const& sortedLines, uint32_t ind) {
  auto* lineEndPoint = sortedLines[ind];
  BSequence->emplace_back(BSEQPNT {lineEndPoint->x, lineEndPoint->y, SEQBOT});
  // Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
  //             and not the next entry in sortedLines
  ++lineEndPoint;
  BSequence->emplace_back(BSEQPNT {lineEndPoint->x, lineEndPoint->y, SEQTOP});
}

void form::internal::brkseq(std::vector<SMALPNTL*> const& sortedLines,
                            uint32_t                      start,
                            uint32_t                      finish,
                            boost::dynamic_bitset<>&      sequenceMap,
                            uint32_t&                     lastGroup,
                            SMALPNTL*                     sequenceLines) {
  StateMap->reset(StateFlag::SEQDUN);
  if (sequenceLines == nullptr) {
	sequenceLines = sortedLines[start];
  }
  if (start > finish) {
	auto savedGroup = sortedLines[start]->group + 1U;
	// This odd construction for iLine is used to ensure
	// loop terminates when finish = 0
	for (auto iLine = start + 1U; iLine != finish; --iLine) {
	  auto const iLineDec = iLine - 1U;
	  --savedGroup;
	  if (sortedLines[iLineDec]->group != savedGroup) {
		BSequence->emplace_back(sequenceLines[0].x, sequenceLines[0].y, 0);
		sequenceLines = sortedLines[iLineDec];
		BSequence->emplace_back(sequenceLines[0].x, sequenceLines[0].y, 0);
		savedGroup = sequenceLines[0].group;
	  }
	  else {
		sequenceLines = sortedLines[iLineDec];
	  }
	  if (sequenceMap.test_set(iLineDec)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  duseq1(sequenceLines);
		}
	  }
	  else {
		movseq(sortedLines, iLineDec);
	  }
	}
	lastGroup = sequenceLines->group;
  }
  else {
	auto savedGroup = sortedLines[start]->group - 1U;
	for (auto iLine = start; iLine <= finish; ++iLine) {
	  ++savedGroup;
	  if (sortedLines[iLine]->group != savedGroup) {
		BSequence->emplace_back(sequenceLines[0].x, sequenceLines[0].y, 0);
		sequenceLines = sortedLines[iLine];
		BSequence->emplace_back(sequenceLines[0].x, sequenceLines[0].y, 0);
		savedGroup = sequenceLines[0].group;
	  }
	  else {
		sequenceLines = sortedLines[iLine];
	  }
	  if (sequenceMap.test_set(iLine)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  duseq1(sequenceLines);
		}
	  }
	  else {
		movseq(sortedLines, iLine);
	  }
	}
	lastGroup = sequenceLines->group;
  }
  if (StateMap->testAndReset(StateFlag::SEQDUN)) {
	duseq1(sequenceLines);
  }
}

void form::internal::dunseq(std::vector<SMALPNTL*> const& sortedLines, uint32_t start, uint32_t finish, uint32_t& lastGroup) {
  auto minimumY = BIGFLOAT;
  for (auto iLine = start; iLine <= finish; ++iLine) {
	auto const deltaY = sortedLines[start][1].y - sortedLines[start][0].y;
	if (deltaY < minimumY) {
	  minimumY = deltaY;
	}
  }
  if (minimumY == BIGFLOAT) {
	minimumY = 0.0F;
  }
  else {
	minimumY /= 2.0F;
  }
  BSequence->emplace_back(sortedLines[start][0].x, sortedLines[start][0].y + minimumY, 0);
  BSequence->emplace_back(sortedLines[finish][0].x, sortedLines[finish][0].y + minimumY, 0);
  lastGroup = sortedLines[finish][0].group;
}

void form::internal::duseq2(SMALPNTL const* sequenceLines) {
  if (sequenceLines != nullptr) {
	BSequence->emplace_back(wrap::midl(sequenceLines[1].x, sequenceLines[0].x),
	                        wrap::midl(sequenceLines[1].y, sequenceLines[0].y),
	                        0);
  }
}

void form::internal::duseq(std::vector<SMALPNTL*> const& sortedLines,
                           uint32_t                      start,
                           uint32_t                      finish,
                           boost::dynamic_bitset<>&      sequenceMap,
                           uint32_t&                     lastGroup,
                           SMALPNTL*                     sequenceLines) {
  auto savedTopLine = sortedLines[start][1].line;
  StateMap->reset(StateFlag::SEQDUN);
  bool flag = false;
  if (start > finish) {
	auto iLine = start + 1U;
	// This odd construction for iLine is used to ensure loop terminates when finish = 0
	for (; iLine != finish; --iLine) {
	  auto const iLineDec = iLine - 1U;
	  if (sequenceMap.test_set(iLineDec)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  flag = true;
		  duseq2(sortedLines[iLineDec]);
		  sequenceLines = sortedLines[iLineDec];
		}
		else {
		  if (savedTopLine != sortedLines[iLineDec][1].line) {
			if (iLineDec != 0U) {
			  duseq2(sortedLines[wrap::toSize(iLineDec) + 1U]);
			}
			flag = true;
			duseq2(sortedLines[iLineDec]);
			sequenceLines = sortedLines[iLineDec];
			savedTopLine  = sequenceLines[1].line;
		  }
		}
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::SEQDUN)) {
		  duseq2(sortedLines[wrap::toSize(iLineDec) + 1U]);
		}
		flag          = true;
		sequenceLines = sortedLines[iLineDec];
		movseq(sortedLines, iLineDec);
	  }
	}
	if (StateMap->testAndReset(StateFlag::SEQDUN)) {
	  flag = true;
	  duseq2(sortedLines[iLine]);
	  sequenceLines = sortedLines[iLine];
	}
	if (flag) {
	  lastGroup = sequenceLines->group;
	}
  }
  else {
	auto iLine = start;
	for (; iLine <= finish; ++iLine) {
	  if (sequenceMap.test_set(iLine)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  flag = true;
		  duseq2(sortedLines[iLine]);
		  sequenceLines = sortedLines[iLine];
		}
		else {
		  if (savedTopLine != sortedLines[iLine][1].line) {
			if (iLine != 0U) {
			  duseq2(sortedLines[iLine - 1U]);
			}
			flag = true;
			duseq2(sortedLines[iLine]);
			sequenceLines = sortedLines[iLine];
			savedTopLine  = sequenceLines[1].line;
		  }
		}
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::SEQDUN)) {
		  if (iLine != 0U) {
			duseq2(sortedLines[iLine - 1U]);
		  }
		}
		flag          = true;
		sequenceLines = sortedLines[iLine];
		movseq(sortedLines, iLine);
	  }
	}
	if (StateMap->testAndReset(StateFlag::SEQDUN)) {
	  if (iLine != 0U) {
		flag = true;
		duseq2(sortedLines[iLine - 1U]);
		sequenceLines = sortedLines[iLine - 1U];
	  }
	}
	if (flag) {
	  lastGroup = sequenceLines->group;
	}
  }
}

void form::internal::durgn(FRMHED const&                 form,
                           std::vector<FSEQ> const&      sequencePath,
                           boost::dynamic_bitset<>&      visitedRegions,
                           std::vector<SMALPNTL*> const& sortedLines,
                           uint32_t                      pthi,
                           uint32_t                      lineCount,
                           std::vector<REGION> const&    regionsList,
                           uint32_t&                     lastGroup,
                           uint32_t                      sequencePathIndex,
                           std::vector<fPOINT>&          workingFormVertices) {
  auto        sequenceMap   = boost::dynamic_bitset<>(lineCount);
  auto        nextGroup     = gsl::narrow_cast<uint32_t>(sequencePath[pthi].nextGroup);
  auto const  iRegion       = sequencePath[pthi].node;
  auto const& currentRegion = regionsList[iRegion];
  auto const  sequenceStart = currentRegion.start;
  auto const  sequenceEnd   = currentRegion.end;
  if (sequencePath[pthi].skp || StateMap->testAndReset(StateFlag::BRKFIX)) {
	auto iter = std::prev(BSequence->end());
	if (iter->attribute != SEQBOT) {
	  --iter;
	  BSequence->emplace_back(iter->x, iter->y, 0);
	}
	// clang-format off
	auto const  firstLine     = sortedLines[sequenceStart]->line;
	auto const& bpnt          = BSequence->back();
	auto        minimumLength = BIGFLOAT;
	auto        mindif        = 0U;
	// clang-format on
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  auto const length = hypot(bpnt.x - workingFormVertices.operator[](iVertex).x,
	                            bpnt.y - workingFormVertices.operator[](iVertex).y);
	  if (length < minimumLength) {
		minimumLength = length;
		mindif        = iVertex;
	  }
	}
	if (minimumLength != 0.0F) {
	  BSequence->emplace_back(
	      workingFormVertices.operator[](mindif).x, workingFormVertices.operator[](mindif).y, 0);
	}
	if (form.vertexCount != 0U) {
	  auto const fdif = (form.vertexCount + firstLine - mindif) % form.vertexCount;
	  auto const bdif = (form.vertexCount - firstLine + mindif) % form.vertexCount;
	  if (fdif < bdif) {
		auto ind = form::nxt(form, mindif);
		while (ind != firstLine) {
		  BSequence->emplace_back(
		      workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
		  ind = form::nxt(form, ind);
		}
		BSequence->emplace_back(
		    workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
	  }
	  else {
		auto ind = form::prv(form, mindif);
		while (ind != firstLine) {
		  BSequence->emplace_back(
		      workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
		  ind = form::prv(form, ind);
		}
		BSequence->emplace_back(
		    workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
	  }
	}
  }
  auto dun = true;
  if (!visitedRegions[iRegion]) {
	dun = false;
	visitedRegions.set(iRegion);
  }
  auto const groupStart = sortedLines[currentRegion.start]->group;
  auto const groupEnd   = sortedLines[currentRegion.end]->group;
  auto       seql       = 0U;
  if (groupEnd != groupStart) {
	auto const intermediate = std::round((gsl::narrow_cast<double>(lastGroup) - groupStart) /
	                                         (gsl::narrow_cast<double>(groupEnd) - groupStart) *
	                                         (gsl::narrow_cast<double>(sequenceEnd) - sequenceStart) +
	                                     sequenceStart);
	wrap::narrow(seql, std::abs(intermediate));
  }
  auto const length = (gsl::narrow_cast<double>(groupEnd) - groupStart) *
                      (gsl::narrow_cast<double>(sequenceEnd) - sequenceStart);
  auto seqn = sequenceEnd;
  if (length != 0.0) {
	if (nextGroup == 0 || nextGroup < groupStart) {
	  nextGroup = groupStart;
	}
	seqn = wrap::round<decltype(seqn)>((gsl::narrow_cast<double>(nextGroup) - groupStart) / length + sequenceStart);
  }
  if (seql < sequenceStart) {
	seql = sequenceStart;
  }
  if (seql > sequenceEnd) {
	seql = sequenceEnd;
  }
  if (seqn < sequenceStart) {
	seqn = sequenceStart;
  }
  if (seqn > sequenceEnd) {
	seqn = sequenceEnd;
  }
  if (sortedLines[seql]->group != lastGroup) {
	if (seql < sequenceEnd && sortedLines[wrap::toSize(seql) + 1U]->group == lastGroup) {
	  ++seql;
	}
	else {
	  if (seql > sequenceStart && sortedLines[seql - 1U]->group == lastGroup) {
		--seql;
	  }
	  else {
		auto mindif = std::numeric_limits<uint32_t>::max();
		for (auto ind = sequenceStart; ind <= sequenceEnd; ++ind) {
		  auto const gdif = ((sortedLines[ind]->group > lastGroup) ? (sortedLines[ind]->group - lastGroup)
		                                                           : (lastGroup - sortedLines[ind]->group));
		  if (gdif < mindif) {
			mindif = gdif;
			seql   = ind;
		  }
		}
	  }
	}
  }
  if (sortedLines[seqn]->group != nextGroup) {
	if (seqn < sequenceEnd && sortedLines[wrap::toSize(seqn) + 1U]->group == nextGroup) {
	  ++seqn;
	}
	else {
	  if (seqn > sequenceStart && sortedLines[seqn - 1U]->group == nextGroup) {
		--seqn;
	  }
	  else {
		auto mindif = std::numeric_limits<uint32_t>::max();
		for (auto ind = sequenceStart; ind <= sequenceEnd; ++ind) {
		  auto const gdif = ((sortedLines[ind]->group > nextGroup) ? (sortedLines[ind]->group - nextGroup)
		                                                           : (nextGroup - sortedLines[ind]->group));
		  if (gdif < mindif) {
			mindif = gdif;
			seqn   = ind;
		  }
		}
	  }
	}
  }
  auto* sequenceLines = gsl::narrow_cast<SMALPNTL*>(nullptr);
  if (currentRegion.breakCount != 0U) {
	if (dun) {
	  brkdun(sortedLines, seql, seqn, workingFormVertices);
	}
	else {
	  if (lastGroup >= groupEnd) {
		brkseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceLines);
		if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
		  brkseq(sortedLines, sequenceStart, seqn, sequenceMap, lastGroup, sequenceLines);
		}
	  }
	  else {
		if (groupStart <= nextGroup) {
		  if (seql != sequenceStart) {
			brkseq(sortedLines, seql, sequenceStart, sequenceMap, lastGroup, sequenceLines);
		  }
		  brkseq(sortedLines, sequenceStart, sequenceEnd, sequenceMap, lastGroup, sequenceLines);
		  if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
			brkseq(sortedLines, sequenceEnd, seqn, sequenceMap, lastGroup, sequenceLines);
		  }
		}
		else {
		  if (seql != sequenceEnd) {
			brkseq(sortedLines, seql, sequenceEnd, sequenceMap, lastGroup, sequenceLines);
		  }
		  brkseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceLines);
		  if (pthi < sequencePathIndex - 1 && sequenceStart != seqn) {
			brkseq(sortedLines, sequenceStart, seqn, sequenceMap, lastGroup, sequenceLines);
		  }
		}
	  }
	}
  }
  else {
	if (dun) {
	  dunseq(sortedLines, seql, seqn, lastGroup);
	}
	else {
	  if (lastGroup >= groupEnd) {
		duseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceLines);
		duseq(sortedLines, sequenceStart, seqn, sequenceMap, lastGroup, sequenceLines);
	  }
	  else {
		if (groupStart <= nextGroup) {
		  if (seql != sequenceStart) {
			duseq(sortedLines, seql, sequenceStart, sequenceMap, lastGroup, sequenceLines);
		  }
		  duseq(sortedLines, sequenceStart, sequenceEnd, sequenceMap, lastGroup, sequenceLines);
		  if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
			duseq(sortedLines, sequenceEnd, seqn, sequenceMap, lastGroup, sequenceLines);
		  }
		}
		else {
		  if (seql != sequenceEnd) {
			duseq(sortedLines, seql, sequenceEnd, sequenceMap, lastGroup, sequenceLines);
		  }
		  duseq(sortedLines, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceLines);
		  if (pthi < sequencePathIndex - 1 && sequenceStart != seqn) {
			duseq(sortedLines, sequenceStart, seqn, sequenceMap, lastGroup, sequenceLines);
		  }
		}
	  }
	}
  }
}

void form::internal::lcon(FRMHED const&          form,
                          std::vector<uint32_t>& groupIndexSequence,
                          std::vector<SMALPNTL>& lineEndpoints,
                          std::vector<fPOINT>&   workingFormVertices) {
#if BUGSEQ
  UNREFERENCED_PARAMETER(groupIndexSequence);
#endif

  if (!lineEndpoints.empty()) {
	auto       sortedLines     = std::vector<SMALPNTL*> {};
	auto const stitchLineCount = lineEndpoints.size();
	sortedLines.reserve(stitchLineCount / 2U);
	for (auto iLine = 0U; iLine < stitchLineCount; iLine += 2U) {
	  sortedLines.push_back(&lineEndpoints[iLine]);
	}
	std::sort(sortedLines.begin(), sortedLines.end(), fi::spComp);
	auto const lineCount = wrap::toUnsigned(sortedLines.size());
	auto       regions   = std::vector<REGION> {};
	regions.emplace_back(0U, 0U, 0U, 0U);
	auto breakLine = sortedLines[0]->line;
	for (auto iLine = 0U; iLine < lineCount; ++iLine) {
	  if (breakLine != sortedLines[iLine]->line) {
		regions.back().end = iLine - 1U;
		regions.emplace_back(iLine, 0U, 0U, 0U);
		breakLine = sortedLines[iLine]->line;
	  }
	}
	regions.back().end            = lineCount - 1U;
	auto const     regionCount    = wrap::toUnsigned(regions.size());
	auto           visitedRegions = boost::dynamic_bitset<>(regionCount);
	constexpr auto STLINE         = 0U;
	for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	  auto count = 0U;
	  if ((regions[iRegion].end - regions[iRegion].start) > 1) {
		auto startGroup = sortedLines[regions[iRegion].start]->group;
		for (auto iLine = regions[iRegion].start + 1U; iLine <= regions[iRegion].end; ++iLine) {
		  ++startGroup;
		  if (sortedLines[iLine]->group != startGroup) {
			if (count == 0U) {
			  regions[iRegion].regionBreak = STLINE;
			}
			++count;
			startGroup = sortedLines[iLine]->group;
		  }
		}
	  }
	  regions[iRegion].breakCount = count;
	}

#if BUGSEQ
	// Note - this debug code only works for vertical fill on a single form
	auto bugColor = 0U;
	for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	  for (auto iLine = regions[iRegion].start; iLine <= regions[iRegion].end; ++iLine) {
		auto const* lineGroupPoint = &*sortedLines[iLine];
		StitchBuffer->push_back(fPOINTATTR {lineGroupPoint[0].x, lineGroupPoint[0].y, bugColor});
		StitchBuffer->push_back(fPOINTATTR {lineGroupPoint[1].x, lineGroupPoint[1].y, bugColor});
	  }
	  ++bugColor;
	  bugColor &= 0xf;
	}
#else
	BSequence->clear();
	auto mapIndexSequence = std::vector<uint32_t> {};
	mapIndexSequence.reserve(wrap::toSize(regionCount) + 1U);
	auto pathMap      = std::vector<RCON> {};
	auto sequencePath = std::vector<FSEQ> {};
	if (regionCount > 1) {
	  auto pathMapIndex = 0U;
	  // use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
	  pathMap.reserve(wrap::toSize((regionCount * (regionCount - 1U)) / 2U) + 2U);
	  for (auto iSequence = 0U; iSequence < regionCount; ++iSequence) {
		mapIndexSequence.push_back(pathMapIndex);
		auto count              = 0;
		auto gapToClosestRegion = 0.0F;
		for (auto iNode = 0U; iNode < regionCount; ++iNode) {
		  if (iSequence != iNode) {
			auto       nextGroup   = 0U;
			auto const isConnected = regclos(
			    groupIndexSequence, lineEndpoints, sortedLines, iSequence, iNode, regions, gapToClosestRegion, nextGroup);
			if (isConnected) {
			  pathMap.push_back(RCON {iNode, isConnected, nextGroup});
			  ++pathMapIndex;
			  ++count;
			}
		  }
		}
		while (count == 0) {
		  gapToClosestRegion += LineSpacing;
		  count = 0;
		  for (auto iNode = 0U; iNode < regionCount; ++iNode) {
			if (iSequence != iNode) {
			  auto       nextGroup   = 0U;
			  auto const isConnected = regclos(
			      groupIndexSequence, lineEndpoints, sortedLines, iSequence, iNode, regions, gapToClosestRegion, nextGroup);
			  if (isConnected) {
				pathMap.push_back(RCON {iNode, isConnected, nextGroup});
				++pathMapIndex;
				++count;
			  }
			}
		  }
		}
	  }
	  mapIndexSequence.push_back(pathMapIndex);
	  // find the leftmost region
	  auto startGroup = std::numeric_limits<uint32_t>::max();
	  auto leftRegion = 0U;
	  for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
		auto const* lineGroupPoint = sortedLines[regions[iRegion].start];
		if (lineGroupPoint->group < startGroup) {
		  startGroup = lineGroupPoint->group;
		  leftRegion = iRegion;
		}
	  }
	  auto tempPath = std::vector<RGSEQ> {};
	  tempPath.resize(wrap::toSize((regionCount * (regionCount - 1U)) / 2U) + 1U);
	  // find the leftmost region in pathMap
	  auto sequencePathIndex = 1U;
	  auto dontSkip          = true;
	  auto inPath            = 0U;
	  for (inPath = 0U; inPath < pathMapIndex; ++inPath) {
		if (pathMap[inPath].node == leftRegion) {
		  dontSkip = false;
		  break;
		}
	  }
	  if (dontSkip) {
		pathMap.push_back(RCON {leftRegion, false, 0});
		inPath = pathMapIndex;
	  }
	  // set the first entry in the temporary path to the leftmost region
	  tempPath[0].pcon  = inPath;
	  tempPath[0].count = 1;
	  tempPath[0].skp   = false;
	  visitedRegions.set(leftRegion);
	  auto doneRegion   = leftRegion; // last region sequenced
	  auto visitedIndex = 0;
	  while (unvis(visitedRegions, visitedIndex)) {
		nxtrgn(tempPath, pathMap, mapIndexSequence, visitedRegions, sortedLines, regions, doneRegion, pathMapIndex, sequencePathIndex, visitedIndex);
	  }
	  auto count = std::numeric_limits<uint32_t>::max();
	  sequencePath.reserve(sequencePathIndex);
	  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
		bool const tmpSkip = tempPath[iPath].skp;
		uint16_t   tmpNode = 0U;
		if (tempPath[iPath].pcon == std::numeric_limits<uint32_t>::max()) {
		  wrap::narrow(tmpNode, tempPath[iPath].count);
		  count = wrap::toUnsigned(tempPath[iPath].count);
		}
		else {
		  if (tempPath[iPath].pcon != count) {
			count = tempPath[iPath].pcon;
			wrap::narrow(tmpNode, pathMap[tempPath[iPath].pcon].node);
		  }
		}
		sequencePath.push_back(FSEQ {tmpNode, 0, tmpSkip});
	  }
	  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
		nxtseq(sequencePath, pathMap, mapIndexSequence, iPath);
	  }
	  visitedRegions.reset();
	  auto lastGroup = 0U;
	  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
		outDebugString(L"iterator {},vrt {},grpn {}\n", iPath, pathMap[iPath].node, pathMap[iPath].nextGroup);
		if (!unvis(visitedRegions, visitedIndex)) {
		  break;
		}
		durgn(form, sequencePath, visitedRegions, sortedLines, iPath, lineCount, regions, lastGroup, sequencePathIndex, workingFormVertices);
	  }
	}
	else {
	  constexpr auto SPATHIDX = 1U; // Sequence path index
	  sequencePath.resize(SPATHIDX);
	  auto lastGroup       = 0U;
	  sequencePath[0].node = 0;
	  wrap::narrow(sequencePath[0].nextGroup, sortedLines[regions[0].end]->group);
	  sequencePath[0].skp = false;
	  durgn(form, sequencePath, visitedRegions, sortedLines, 0, lineCount, regions, lastGroup, SPATHIDX, workingFormVertices);
	}

#endif
  }
}

void form::internal::bakseq() {
#if BUGSEQ
#else
  constexpr auto RITSIZ = 6;

  static constexpr auto seqtab = std::array<int32_t, RITSIZ> {
      12,
      7,
      15,
      11,
      13,
      17,
  };

#if BUGBAK

  for (auto val : *BSequence) {
	OSequence->push_back(fPOINT {val.x, val.y});
  }
  FormList->operator[](ClosestFormToCursor).maxFillStitchLen = 6000;
#else

  auto iSequence = BSequence->size();
  if (iSequence != 0U) {
	--iSequence;
  }
  OSequence->clear();
  StateMap->reset(StateFlag::FILDIR);
  OSequence->push_back(fPOINT {BSequence->operator[](iSequence).x, BSequence->operator[](iSequence).y});
  if (iSequence != 0U) {
	--iSequence;
  }
  while (iSequence > 0) {
	// clang-format off
	auto const rcnt           = iSequence % seqtab.size();
	auto const StitchSpacing2 = LineSpacing * 2;
	auto const rit            = std::lround(BSequence->operator[](iSequence).x / StitchSpacing2);
	auto&      bPrevious      = BSequence->operator[](iSequence - 1U);
	auto&      bCurrent       = BSequence->operator[](iSequence);
	auto&      bNext          = BSequence->operator[](iSequence + 1U);
	auto       delta          = fPOINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};
	auto       slope          = std::numeric_limits<float>::max(); // initialize to the max value i.e. slope when y is zero
	// clang-format on
	if (delta.y != 0.0F) {
	  slope = delta.x / delta.y;
	}
	auto const  UserStitchLength9 = UserStitchLength / MAXSIZ;
	auto const& form              = FormList->operator[](ClosestFormToCursor);
	switch (bCurrent.attribute) {
	  case SEQTOP: {
		if ((form.extendedAttribute & AT_SQR) != 0U) {
		  if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			OSequence->push_back(fPOINT {bPrevious.x, bPrevious.y});
			auto count = wrap::ceil<int32_t>(bCurrent.y / UserStitchLength);
			do {
			  OSequence->push_back(fPOINT {0.0F,
			                               wrap::toFloat(count) * UserStitchLength +
			                                   wrap::toFloat(rit % seqtab[rcnt]) * UserStitchLength9});
			  if (OSequence->back().y > bCurrent.y) {
				break;
			  }
			  OSequence->back().x = bCurrent.x;
			  ++count;
			} while (true);
			OSequence->back() = bCurrent;
		  }
		  else {
			OSequence->push_back(fPOINT {bCurrent.x, bCurrent.y});
			auto count = wrap::floor<int32_t>(bCurrent.y / UserStitchLength);
			do {
			  OSequence->push_back(fPOINT {0.0F,
			                               wrap::toFloat(count) * UserStitchLength -
			                                   wrap::toFloat((rit + 2) % seqtab[rcnt]) * UserStitchLength9});
			  if (OSequence->back().y < bPrevious.y) {
				break;
			  }
			  OSequence->back().x = bCurrent.x;
			  --count;
			} while (true);
			OSequence->back() = bPrevious;
		  }
		}
		else {
		  auto count = wrap::ceil<int32_t>(bNext.y / UserStitchLength);
		  do {
			OSequence->push_back(fPOINT {0.0F,
			                             wrap::toFloat(count) * UserStitchLength +
			                                 wrap::toFloat(rit % seqtab[rcnt]) * UserStitchLength9});
			if (OSequence->back().y > bCurrent.y) {
			  break;
			}
			delta.y             = OSequence->back().y - bNext.y;
			delta.x             = slope * delta.y;
			OSequence->back().x = bNext.x + delta.x;
			++count;
		  } while (true);
		  OSequence->back() = bCurrent;
		}
		break;
	  }
	  case SEQBOT: {
		if ((form.extendedAttribute & AT_SQR) == 0U) {
		  auto count = wrap::floor<int32_t>(bNext.y / UserStitchLength);
		  do {
			OSequence->push_back(fPOINT {0.0F,
			                             wrap::toFloat(count) * UserStitchLength -
			                                 wrap::toFloat((rit + 2) % seqtab[rcnt]) * UserStitchLength9});
			if (OSequence->back().y < bCurrent.y) {
			  break;
			}
			delta.y             = OSequence->back().y - bNext.y;
			delta.x             = slope * delta.y;
			OSequence->back().x = bNext.x + delta.x;
			--count;
		  } while (true);
		  OSequence->back() = bCurrent;
		}
		break;
	  }
	  case 0: {
		delta = fPOINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};
		StateMap->reset(StateFlag::FILDIR);
		auto const length = hypot(delta.x, delta.y);
		if (length != 0.0F) {
		  auto const UserStitchLength2 = UserStitchLength * 2.0F;
		  if (length > UserStitchLength2) {
			auto point = bNext;
			auto count = wrap::round<uint32_t>(length / UserStitchLength - 1.0F);
			if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
			  return;
			}
			auto const fCount = wrap::toFloat(count);
			auto const step   = fPOINT {delta.x / fCount, delta.y / fCount};
			while (count != 0U) {
			  point.x += step.x;
			  point.y += step.y;
			  OSequence->push_back(fPOINT {point.x, point.y});
			  --count;
			}
		  }
		}
		OSequence->push_back(fPOINT {bCurrent.x, bCurrent.y});
		break;
	  }
	  default: {
		outDebugString(L"default hit in bakseq: attribute [{}]\n", bCurrent.attribute);
		break;
	  }
	}
	--iSequence;
  }
#endif // BUGBAK
#endif // BUGSEQ
}

auto form::filinu(fPOINT const& inPoint, fPOINT const& stitchPoint) -> fPOINT {
  auto       point  = stitchPoint;
  auto const delta  = fPOINT {(inPoint.x - stitchPoint.x), (inPoint.y - stitchPoint.y)};
  auto const length = hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>(length / UserStitchLength);
  if (count != 0U) {
	if (StateMap->test(StateFlag::FILDIR)) {
	  ++count;
	}
	auto const fCount = wrap::toFloat(count);
	auto const step   = fPOINT {delta.x / fCount, delta.y / fCount};
	while (count > 0) {
	  point.x += step.x;
	  point.y += step.y;
	  OSequence->push_back(point);
	  --count;
	}
  }
  else {
	OSequence->push_back(inPoint);
  }
  return inPoint;
}

auto form::filin(fPOINT const& currentPoint, fPOINT const& stitchPoint) -> fPOINT {
  auto const delta  = fPOINT {(currentPoint.x - stitchPoint.x), (currentPoint.y - stitchPoint.y)};
  auto       point  = stitchPoint;
  auto const length = hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>(length / UserStitchLength);
  if (count != 0U) {
	if (StateMap->test(StateFlag::FILDIR)) {
	  ++count;
	}
	auto const fCount = wrap::toFloat(count);
	auto const step   = fPOINT {delta.x / fCount, delta.y / fCount};
	while (count > 0) {
	  point.x += step.x;
	  point.y += step.y;
	  OSequence->push_back(point);
	  --count;
	}
  }
  else {
	OSequence->push_back(currentPoint);
  }
  return currentPoint;
}

void form::internal::dudif(fPOINT const& start, fPOINT const& finish, fPOINT& delta) noexcept {
  delta.x = finish.x - start.x;
  delta.y = finish.y - start.y;
}

void form::internal::trfrm(fPOINT const& bottomLeftPoint,
                           fPOINT const& topLeftPoint,
                           fPOINT const& bottomRightPoint,
                           fPOINT const& topRightPoint) {
  auto topDelta    = fPOINT {};
  auto bottomDelta = fPOINT {};
  auto leftDelta   = fPOINT {};
  auto rightDelta  = fPOINT {};
  dudif(topLeftPoint, topRightPoint, topDelta);
  dudif(bottomLeftPoint, bottomRightPoint, bottomDelta);
  dudif(bottomLeftPoint, topLeftPoint, leftDelta);
  dudif(bottomRightPoint, topRightPoint, rightDelta);
  for (auto& clip : *ClipBuffer) {
	auto const clipRatio      = fPOINT {clip.x / ClipRectSize.cx, clip.y / ClipRectSize.cy};
	auto const topMidpoint    = fPOINT {clipRatio.x * (topDelta.x) + topLeftPoint.x,
                                     clipRatio.x * (topDelta.y) + topLeftPoint.y};
	auto const bottomMidpoint = fPOINT {clipRatio.x * (bottomDelta.x) + bottomLeftPoint.x,
	                                    clipRatio.x * (bottomDelta.y) + bottomLeftPoint.y};
	auto       middleDelta    = fPOINT {};
	dudif(bottomMidpoint, topMidpoint, middleDelta);
	OSequence->push_back(fPOINT {clipRatio.y * middleDelta.x + bottomMidpoint.x,
	                             clipRatio.y * middleDelta.y + bottomMidpoint.y});
  }
}

void form::internal::clpfm() {
  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(BSequence->size()) - 2U; iSequence += 2) {
	auto& bSeq0 = BSequence->operator[](iSequence);
	auto& bSeq1 = BSequence->operator[](wrap::toSize(iSequence) + 1U);
	auto& bSeq2 = BSequence->operator[](wrap::toSize(iSequence) + 2U);
	auto& bSeq3 = BSequence->operator[](wrap::toSize(iSequence) + 3U);

	auto const leftLength  = hypot(bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y);
	auto const rightLength = hypot(bSeq3.x - bSeq2.x, bSeq3.y - bSeq2.y);
	auto const leftDelta   = fPOINT {bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y};
	auto const rightDelta  = fPOINT {bSeq2.x - bSeq3.x, bSeq2.y - bSeq3.y};

	auto count = std::round((rightLength > leftLength) ? leftLength / ClipRectSize.cy
	                                                   : rightLength / ClipRectSize.cy);
	if (count == 0.0F) {
	  count = 1.0F;
	}
	auto const leftStep  = fPOINT {leftDelta.x / count, leftDelta.y / count};
	auto const rightStep = fPOINT {rightDelta.x / count, rightDelta.y / count};
	auto       topLeft   = fPOINT {bSeq0.x, bSeq0.y};
	auto       topRight  = fPOINT {bSeq3.x, bSeq3.y};
	for (auto iStep = 0U; iStep < wrap::toUnsigned(count); ++iStep) {
	  auto const bottomLeft  = topLeft;
	  auto const bottomRight = topRight;
	  topLeft.x += leftStep.x;
	  topLeft.y += leftStep.y;
	  topRight.x += rightStep.x;
	  topRight.y += rightStep.y;
	  trfrm(bottomLeft, topLeft, bottomRight, topRight);
	}
  }
}

void form::internal::fmclp(FRMHED& form) {
  auto const savedSpacing = LineSpacing;
  LineSpacing             = ClipRectSize.cx;
  StateMap->set(StateFlag::BARSAT);
  satin::satfil(form);
  StateMap->reset(StateFlag::BARSAT);
  clpfm();
  auto& currentForm    = FormList->operator[](ClosestFormToCursor);
  currentForm.fillType = CLPF;
  LineSpacing          = savedSpacing;
}

void form::refilfn() {
  auto const savedStitchLength   = UserStitchLength;
  auto       angledForm          = FRMHED {};
  auto       workingFormVertices = std::vector<fPOINT> {};
  StateMap->reset(StateFlag::TXFIL);
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	form.underlayIndent = 0;
  }
  if (!(StateMap->test(StateFlag::FUNCLP) || StateMap->test(StateFlag::FUNSCLP))) {
	thred::savdo();
  }
  auto fillStartsData = FILLSTARTS {}; // fill start data for refill
  auto fillStartsMap  = 0U;            // fill starts bitmap
  xt::fdelstch(form, fillStartsData, fillStartsMap);
  StateMap->set(StateFlag::WASREFIL);
  constexpr auto MINSPACE = 0.5F;
  if (form.fillSpacing < MINSPACE && !clip::isclp(form)) {
	form.fillSpacing = MINSPACE;
  }
  if (form.edgeSpacing < MINSPACE) {
	form.edgeSpacing = MINSPACE;
  }
  if (!clip::isclp(form)) {
	UserStitchLength = form.lengthOrCount.stitchLength;
  }
  if (!(StateMap->test(StateFlag::WASDO) || StateMap->test(StateFlag::FUNCLP) ||
        StateMap->test(StateFlag::FUNSCLP))) {
	thred::savdo();
  }
  StateMap->reset(StateFlag::WASDO);
  if (((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U) && form.type == FRMLINE && form.fillType != CONTF) {
	form.type = FRMFPOLY;
  }
  InterleaveSequence->clear();
  InterleaveSequenceIndices->clear();
  StateMap->reset(StateFlag::ISUND);
  auto textureSegments = std::vector<RNGCNT> {};
  textureSegments.resize(wrap::toSize(form.fillInfo.texture.lines));
  switch (form.type) {
	case FRMLINE: {
	  switch (form.edgeType & NEGUND) {
		case EDGELINE: {
		  fi::brdfil(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEBEAN: {
		  fi::bold(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGECLIP: {
		  auto clipRect = fRECTANGLE {};
		  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		  clip::clpout(form.borderSize);
		  clip::clpbrd(form, clipRect, 0);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEANGSAT: {
		  StateMap->reset(StateFlag::SAT1);
		  satin::slbrd(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEPROPSAT: {
		  if (form.vertexCount > 2) {
			StateMap->reset(StateFlag::SAT1);
			fi::plbrd(form, angledForm, *AngledFormVertices);
			fi::ritbrd(form);
		  }
		  break;
		}
		case EDGEAPPL: {
		  fi::lapbrd(form);
		  fi::ritapbrd();
		  StateMap->reset(StateFlag::SAT1);
		  satin::slbrd(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEBHOL: {
		  auto const length      = ButtonholeCornerLength;
		  ButtonholeCornerLength = form::getblen();
		  satin::satout(form, BHWIDTH);
		  fi::blbrd(form);
		  ButtonholeCornerLength = length;
		  fi::ritbrd(form);
		  break;
		}
		case EDGEPICOT: {
		  auto clipRect = fRECTANGLE {};
		  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		  auto const length      = ButtonholeCornerLength;
		  ButtonholeCornerLength = form::getplen();
		  clip::clpic(form, clipRect);
		  ButtonholeCornerLength = length;
		  fi::ritbrd(form);
		  break;
		}
		case EDGEDOUBLE: {
		  fi::dubfn(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGELCHAIN: {
		  StateMap->set(StateFlag::LINCHN);
		  clip::chnfn(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEOCHAIN: {
		  StateMap->reset(StateFlag::LINCHN);
		  clip::chnfn(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGECLIPX: {
		  auto clipRect = fRECTANGLE {};
		  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		  clip::duxclp(form);
		  fi::ritbrd(form);
		  break;
		}
		default: {
		  outDebugString(L"default hit in refilfn 1: edgeType [{}]\n", form.edgeType & NEGUND);
		  break;
		}
	  }
	  if (form.fillType == CONTF && ((form.attribute & FRECONT) != 0)) {
		fi::contf(form);
		fi::ritfil();
	  }
	  break;
	}
	case FRMFPOLY: {
	  xt::chkcwlk(form);
	  xt::chkwlk(form);
	  xt::chkund(form, textureSegments, *AngledFormVertices);
	  StateMap->reset(StateFlag::ISUND);
	  if (form.fillType != 0U) {
		auto const spacing      = LineSpacing;
		LineSpacing             = form.fillSpacing;
		auto lineEndpoints      = std::vector<SMALPNTL> {};
		auto groupIndexSequence = std::vector<uint32_t> {};
		auto rotationCenter     = fPOINT {};
		auto doFill             = true;
		auto rotationAngle      = 0.0F;
		switch (form.fillType) {
		  case VRTF: {
			workingFormVertices.clear();
			workingFormVertices.reserve(form.vertexCount);
			auto startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto endVertex   = wrap::next(startVertex, form.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
			fi::fnvrt(workingFormVertices, groupIndexSequence, lineEndpoints);
			break;
		  }
		  case HORF: {
			rotationAngle = PI_FHALF;
			fi::fnhor(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, *AngledFormVertices);
			workingFormVertices.clear();
			workingFormVertices.reserve(angledForm.vertexCount);
			auto startVertex = wrap::next(AngledFormVertices->cbegin(), angledForm.vertexIndex);
			auto endVertex   = wrap::next(startVertex, angledForm.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
			break;
		  }
		  case ANGF: {
			rotationAngle = PI_FHALF - form.angleOrClipData.angle;
			fi::fnang(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, *AngledFormVertices);
			workingFormVertices.clear();
			workingFormVertices.reserve(angledForm.vertexCount);
			auto startVertex = wrap::next(AngledFormVertices->cbegin(), angledForm.vertexIndex);
			auto endVertex   = wrap::next(startVertex, angledForm.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
			break;
		  }
		  case VCLPF: {
			auto clipRect = fRECTANGLE {};
			clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
			workingFormVertices.clear();
			workingFormVertices.reserve(form.vertexCount);
			auto startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto endVertex   = wrap::next(startVertex, form.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
			fi::clpcon(form, textureSegments, workingFormVertices);
			doFill = false;
			break;
		  }
		  case HCLPF: {
			auto clipRect = fRECTANGLE {};
			clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
			fi::horclpfn(textureSegments, angledForm, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  case ANGCLPF: {
			auto clipRect = fRECTANGLE {};
			clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
			StateMap->reset(StateFlag::ISUND);
			form::angclpfn(form, textureSegments, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  case TXVRTF: {
			texture::setxt(form, textureSegments);
			workingFormVertices.clear();
			workingFormVertices.reserve(form.vertexCount);
			auto startVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto endVertex   = wrap::next(startVertex, form.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
			fi::clpcon(form, textureSegments, workingFormVertices);
			doFill = false;
			break;
		  }
		  case TXHORF: {
			texture::setxt(form, textureSegments);
			fi::horclpfn(textureSegments, angledForm, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  case TXANGF: {
			texture::setxt(form, textureSegments);
			StateMap->reset(StateFlag::ISUND);
			form::angclpfn(form, textureSegments, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  default: {
			outDebugString(L"default hit in refilfn 2: fillType [{}]\n", form.fillType);
			break;
		  }
		}
		if (doFill) {
		  fi::lcon(form, groupIndexSequence, lineEndpoints, workingFormVertices);
		  fi::bakseq();
		  if (form.fillType != VRTF && form.fillType != TXVRTF) {
			rotationAngle = -rotationAngle;
			fi::rotbak(rotationAngle, rotationCenter);
		  }
		}
		fi::ritfil();
		LineSpacing = spacing;
	  }
	  fi::chkbrd(form);
	  break;
	}
	case SAT: {
	  xt::chkcwlk(form);
	  xt::chkwlk(form);
	  xt::chkund(form, textureSegments, *AngledFormVertices);
	  StateMap->reset(StateFlag::ISUND);
	  switch (form.fillType) {
		case SATF: {
		  auto const spacing = LineSpacing;
		  LineSpacing        = form.fillSpacing;
		  UserStitchLength   = form.lengthOrCount.stitchLength;
		  satin::satfil(form);
		  LineSpacing = spacing;
		  fi::ritfil();
		  break;
		}
		case CLPF: {
		  auto clipRect = fRECTANGLE {};
		  clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
		  fi::fmclp(form);
		  fi::ritfil();
		  break;
		}
		case FTHF: {
		  StateMap->set(StateFlag::CNV2FTH);
		  xt::fthrfn(form);
		  break;
		}
		default: {
		  outDebugString(L"default hit in refilfn 3: fillType [{}]\n", form.fillType);
		  break;
		}
	  }
	  fi::chkbrd(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in refilfn 4: type [{}]\n", form.type);
	  break;
	}
  }
  UserStitchLength = savedStitchLength;
  xt::intlv(form, fillStartsData, fillStartsMap);
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  xt::setfchk();
}

void form::refil() {
  if (!UserFlagMap->test(UserFlag::WRNOF)) {
	auto const codedForm = ClosestFormToCursor << FRMSHFT | USMSK;
	if (std::any_of(StitchBuffer->begin(), StitchBuffer->end(), [&codedForm](fPOINTATTR const& m) -> bool {
	      return ((m.attribute & NOTFRM) == 0U) && (m.attribute & (USMSK | FRMSK)) == codedForm;
	    })) {
	  if (FormDataSheet != nullptr) {
		StateMap->set(StateFlag::WASFRMFRM);
	  }
	  thred::undat();
	  displayText::tabmsg(IDS_REFIL);
	  StateMap->set(StateFlag::MOVMSG);
	  displayText::okcan();
	  return;
	}
  }
  form::refilfn();
}

void form::setfpnt() {
  // clang-format off
  auto const& form             = FormList->operator[](ClosestFormToCursor);
  auto const  screenCoordinate = POINT {(Msg.pt.x - StitchWindowOrigin.x), 
										(Msg.pt.y - StitchWindowOrigin.y)};
  auto const vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex + ClosestVertexToCursor);
  // clang-format on
  form::unfrm();
  fi::rats();
  *vertexIt = fi::px2stchf(screenCoordinate);
  form::frmout(ClosestFormToCursor);
  refil();
  StateMap->set(StateFlag::WASFPNT);
  StateMap->reset(StateFlag::SELBOX);
  StateMap->set(StateFlag::FRMPSEL);
  thred::ritfcor(*vertexIt);
  StateMap->set(StateFlag::RESTCH);
}

void form::internal::makpoli() {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (currentForm.type == SAT) {
	if (currentForm.satinGuideCount != 0U) {
	  satin::delsac(ClosestFormToCursor);
	}
  }
  currentForm.type = FRMFPOLY;
}

void form::internal::fsvrt() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type = FRMFPOLY;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType    = VRTF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form::dusqr(form);
  form::refilfn();
}

void form::filvrt() {
  if (displayText::filmsgs(FMM_VRT)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor     = selectedForm;
	  auto const& currentForm = FormList->operator[](ClosestFormToCursor);
	  if (currentForm.type != FRMLINE) {
		fi::fsvrt();
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::fsvrt();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::internal::fshor(FRMHED& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type = FRMFPOLY;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType    = HORF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form.angleOrClipData.angle = PI_FHALF;
  form::dusqr(form);
  form::refil();
}

void form::filhor() {
  if (displayText::filmsgs(FMM_HOR)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](ClosestFormToCursor);
	  if (form.type != FRMLINE) {
		fi::fshor(form);
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  fi::fshor(form);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::internal::fsangl(FRMHED& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type = FRMFPOLY;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType              = ANGF;
  form.angleOrClipData.angle = IniFile.fillAngle;
  form.fillSpacing           = LineSpacing;
  form::fsizpar(form);
  form::dusqr(form);
  form::refil();
}

void form::filangl() {
  if (displayText::filmsgs(FMM_ANG)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  if (form.type != FRMLINE) {
		fi::fsangl(form);
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  fi::fsangl(form);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

auto form::chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  auto const  point = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto const& currentForm = FormList->operator[](ClosestFormToCursor);
  NewFormVertexCount      = currentForm.vertexCount + 1U;
  thred::duzrat();
  auto const rectangle    = form::sRct2px(currentForm.rectangle);
  auto&      formControls = *FormControlPoints;
  formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = rectangle.left;
  formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = rectangle.top;
  formControls[2].x = formControls[3].x = formControls[4].x = rectangle.right;
  formControls[4].y = formControls[5].y = formControls[6].y = rectangle.bottom;
  formControls[1].x = formControls[5].x = std::lround(wrap::midl(rectangle.right, rectangle.left));
  formControls[3].y = formControls[7].y = std::lround(wrap::midl(rectangle.top, rectangle.bottom));

  auto minimumLength    = BIGDBL;
  auto formControlIndex = 0U;
  for (auto iControl : formControls) {
	auto const length = hypot(iControl.x - point.x, iControl.y - point.y);
	if (length < minimumLength) {
	  minimumLength             = length;
	  SelectedFormControlVertex = formControlIndex;
	}
	if (minimumLength < CLOSENUF) {
	  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
	  for (auto iCorner = 0U; iCorner < wrap::toUnsigned(stretchBoxLine.size()); ++iCorner) {
		stretchBoxLine[iCorner] = formControls[wrap::toSize(iCorner) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  thred::strtchbox(stretchBoxLine);
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		StateMap->set(StateFlag::STRTCH);
	  }
	  else {
		StateMap->set(StateFlag::EXPAND);
		xyRatio = (currentForm.rectangle.right - currentForm.rectangle.left) /
		          (currentForm.rectangle.top - currentForm.rectangle.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  StateMap->set(StateFlag::SHOSTRTCH);
	  return true;
	}
	++formControlIndex;
  }
  SelectedFormControlVertex >>= 1U;
  if (point.x >= rectangle.left && point.x <= rectangle.right && point.y >= rectangle.top &&
      point.y <= rectangle.bottom) {
	auto const vertexIt   = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex);
	auto const formOrigin = form::sfCor2px(*vertexIt);
	FormMoveDelta         = fPOINT {formOrigin.x - point.x, formOrigin.y - point.y};
	StateMap->set(StateFlag::FRMOV);
	return true;
  }
  return false;
}

void form::rstfrm() {
  auto const point =
      POINT {(Msg.pt.x + std::lround(FormMoveDelta.x)), (Msg.pt.y + std::lround(FormMoveDelta.y))};

  auto const attribute = ClosestFormToCursor << 4U;
  form::setmfrm();
  StateMap->reset(StateFlag::FRMOV);
  auto const stitchPoint = thred::pxCor2stch(point);
  auto& currentForm      = FormList->operator[](ClosestFormToCursor);
  auto       vertexIt    = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  auto const offset      = fPOINT {stitchPoint.x - vertexIt->x, stitchPoint.y - vertexIt->y};
  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; ++iVertex) {
	vertexIt->x += offset.x;
	vertexIt->y += offset.y;
	++vertexIt;
  }
  currentForm.rectangle.left += offset.x;
  currentForm.rectangle.top += offset.y;
  currentForm.rectangle.right += offset.x;
  currentForm.rectangle.bottom += offset.y;
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & FRMSK) == attribute && ((stitch.attribute & ALTYPMSK) != 0U) &&
	    ((stitch.attribute & NOTFRM) == 0U)) {
	  stitch.x += offset.x;
	  stitch.y += offset.y;
	}
  }
}

void form::clrfills() noexcept {
  for (auto& formIter : *FormList) {
	formIter.clipEntries             = 0;
	formIter.lengthOrCount.clipCount = 0;
	formIter.edgeType                = 0;
	formIter.fillType                = 0;
	formIter.attribute &= NFRECONT;
	formIter.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
  }
  ClipPoints->clear();
}

void form::internal::ducon() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, FormLines->data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::internal::uncon() {
  if (StateMap->testAndReset(StateFlag::SHOCON)) {
	ducon();
  }
}

void form::drwcon() {
  fi::uncon();
  auto& formLines = *FormLines;
  formLines[1].x  = Msg.pt.x - StitchWindowOrigin.x;
  formLines[1].y  = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::SHOCON);
  fi::ducon();
}

void form::duinsf() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::uninsf() {
  if (StateMap->testAndReset(StateFlag::SHOINSF)) {
	form::duinsf();
  }
}

void form::rotfrm(FRMHED& form, uint32_t newStartVertex) {
  auto rotatedVertices = std::vector<fPOINT> {};
  rotatedVertices.resize(form.vertexCount);
  auto iRotated = newStartVertex;
  auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
  std::copy(vertexIt, wrap::next(vertexIt, form.vertexCount), rotatedVertices.begin());
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*vertexIt = rotatedVertices[iRotated];
	iRotated  = form::nxt(form, iRotated);
	++vertexIt;
  }
  auto const gBeginIt  = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
  auto       rotatedIt = gBeginIt;
  if (form.type == SAT) {
	if (form.vertexCount != 0U) {
	  if (form.wordParam != 0U) {
		form.wordParam = (form.wordParam + form.vertexCount - newStartVertex) % form.vertexCount;
	  }
	  auto guideIt = gBeginIt;
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		if (guideIt->start != newStartVertex && guideIt->finish != newStartVertex) {
		  rotatedIt->start = (guideIt->start + form.vertexCount - newStartVertex) % form.vertexCount;
		  rotatedIt->finish = (guideIt->finish + form.vertexCount - newStartVertex) % form.vertexCount;
		  if (rotatedIt->start > rotatedIt->finish) {
			std::swap(rotatedIt->start, rotatedIt->finish);
		  }
		  ++rotatedIt;
		}
		++guideIt;
	  }
	}
  }
  if (rotatedIt != gBeginIt) {
	form.satinGuideCount = wrap::distance<uint32_t>(gBeginIt, rotatedIt);
	std::sort(gBeginIt, rotatedIt, satin::scomp);
  }
  if (form.vertexCount != 0U) {
	if ((form.extendedAttribute & AT_STRT) != 0U) {
	  form.fillStart = (form.fillStart + form.vertexCount - newStartVertex) % form.vertexCount;
	}
	if ((form.extendedAttribute & AT_END) != 0U) {
	  form.fillEnd = (form.fillEnd + form.vertexCount - newStartVertex) % form.vertexCount;
	}
  }
}

void form::internal::filsfn() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.type = SAT;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType    = SATF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form::refilfn();
}

void form::filsat() {
  if (displayText::filmsgs(FMM_FAN)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor     = selectedForm;
	  auto const& currentForm = FormList->operator[](ClosestFormToCursor);
	  if (currentForm.type != FRMLINE) {
		fi::filsfn();
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::filsfn();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

auto form::internal::closat(intersectionStyles& inOutFlag) -> bool {
  auto       minimumLength = BIGFLOAT;
  auto const stitchPoint   = thred::pxCor2stch(Msg.pt);
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& formIter = FormList->operator[](iForm);
	if (formIter.vertexCount != 0U) {
	  auto const layer =
	      gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formIter.attribute & FRMLMSK) >> 1U);
	  if ((ActiveLayer == 0U) || layer == ActiveLayer || ((formIter.attribute & FRMLMSK) == 0U)) {
		auto const lastVertex = (formIter.type == FRMLINE) ? formIter.vertexCount - 1U : formIter.vertexCount;
		// Loop through for all line segments
		auto       length   = 0.0F;
		auto const vertexIt = wrap::next(FormVertices->cbegin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < lastVertex; ++iVertex) {
		  auto const currVert = wrap::next(vertexIt, iVertex);
		  auto const nextVert = wrap::next(vertexIt, form::nxt(formIter, iVertex));
		  auto const param    = findDistanceToSide(*currVert, *nextVert, stitchPoint, length);
		  if ((length < minimumLength)) {
			if ((param < 0.0F) && (iVertex == 0)) {
			  // this should only happen if the Closest vertex is the start of a line (vertex 0)
			  minimumLength         = length;
			  ClosestFormToCursor   = iForm;
			  ClosestVertexToCursor = iVertex;
			  inOutFlag             = intersectionStyles::POINT_BEFORE_LINE;
			}
			else {
			  // return the vertex after the intersection
			  if ((param > 1.0F) && (iVertex == lastVertex - 1)) {
				minimumLength         = length;
				ClosestFormToCursor   = iForm;
				ClosestVertexToCursor = form::nxt(formIter, iVertex);
				inOutFlag             = intersectionStyles::POINT_AFTER_LINE;
			  }
			  else {
				minimumLength         = length;
				ClosestFormToCursor   = iForm;
				ClosestVertexToCursor = form::nxt(formIter, iVertex);
				inOutFlag             = intersectionStyles::POINT_IN_LINE;
			  }
			}
		  }
		}
	  }
	}
	else {
	  outDebugString(L"closat: Form Has no vertices!\n inOutFlag[{}]", inOutFlag);
	}
  }
  return minimumLength != BIGFLOAT;
}

void form::internal::nufpnt(uint32_t vertex, FRMHED& form, fPOINT const& stitchPoint) {
  form::fltspac(vertex + 1U, 1U);
  ++(form.vertexCount);
  auto const vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex + vertex + 1U);
  *vertexIt           = stitchPoint;
  if (form.satinGuideCount != 0U) {
	auto guideIt = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
	for (auto ind = 0U; ind < form.satinGuideCount; ++ind) {
	  if (guideIt->start > vertex) {
		++(guideIt->start);
	  }
	  if (guideIt->finish > vertex) {
		++(guideIt->finish);
	  }
	  ++guideIt;
	}
  }
  if (form.wordParam >= vertex + 1U) {
	++(form.wordParam);
	form.wordParam %= form.vertexCount;
  }
  if (form.fillType == CONTF) {
	if (form.angleOrClipData.guide.start > vertex) {
	  ++(form.angleOrClipData.guide.start);
	}
	if (form.angleOrClipData.guide.finish > vertex) {
	  ++(form.angleOrClipData.guide.finish);
	}
  }
  form::frmlin(form);
}

void form::insat() { // insert a point in a form
  auto inOutFlag = intersectionStyles::POINT_IN_LINE;
  if (fi::closat(inOutFlag)) {
	thred::savdo();
	// clang-format off
	auto&      selectedForm  = FormList->operator[](ClosestFormToCursor);
	auto const stitchPoint = thred::pxCor2stch(Msg.pt);
	auto const lastVertex    = selectedForm.vertexCount - 1U;
	// clang-format on
	if (inOutFlag != intersectionStyles::POINT_IN_LINE) {
	  if (ClosestVertexToCursor == 0 && selectedForm.type == FRMLINE) {
		StateMap->set(StateFlag::PRELIN);
	  }
	  else {
		if (ClosestVertexToCursor != lastVertex && selectedForm.type == FRMLINE) {
		  ClosestVertexToCursor = form::prv(selectedForm, ClosestVertexToCursor);
		}
	  }
	  fi::nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	  auto const vertexIt = wrap::next(FormVertices->begin(), selectedForm.vertexIndex);
	  if (StateMap->testAndReset(StateFlag::PRELIN)) {
		std::swap(vertexIt[0], vertexIt[1]);
	  }
	}
	else {
	  ClosestVertexToCursor = form::prv(selectedForm, ClosestVertexToCursor);
	  fi::nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	}
	form::refil();
  }
  StateMap->set(StateFlag::RESTCH);
}

auto form::chkdel(FRMHED const& currentForm) noexcept -> bool {
  if (currentForm.type == FRMLINE) {
	return currentForm.vertexCount <= 2;
  }

  return currentForm.vertexCount <= 3;
}

void form::unfil() {
  if (displayText::filmsgs(FMX_UNF)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);
	  if ((form.fillType != 0U) || (form.edgeType != 0U)) {
		clip::delclps(selectedForm);
		texture::deltx(selectedForm);
		formMap.set(selectedForm);
		form.fillType = 0;
		form.edgeType = 0;
		form.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	  }
	}
	auto iDestination = StitchBuffer->begin();
	for (auto& stitch : *StitchBuffer) {
	  if (!formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
		*iDestination++ = stitch;
	  }
	}
	StitchBuffer->resize(wrap::distance<size_t>(StitchBuffer->begin(), iDestination));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  if (!StateMap->testAndReset(StateFlag::IGNOR) && !UserFlagMap->test(UserFlag::WRNOF)) {
		auto const codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
		if (std::any_of(StitchBuffer->begin(), StitchBuffer->end(), [&codedForm](fPOINTATTR const& m) -> bool {
		      return ((m.attribute & NOTFRM) == 0U) && (m.attribute & (USMSK | FRMSK)) == codedForm;
		    })) {
		  displayText::tabmsg(IDS_UNFIL);
		  StateMap->set(StateFlag::FILMSG);
		  displayText::okcan();
		  StateMap->set(StateFlag::IGNOR);
		  return;
		}
	  }
	  if (!StitchBuffer->empty()) {
		auto       iDestination = StitchBuffer->begin();
		auto       destCount    = 0U;
		auto const codedForm    = ClosestFormToCursor << FRMSHFT;
		for (auto& stitch : *StitchBuffer) {
		  if ((stitch.attribute & FRMSK) != codedForm || ((stitch.attribute & NOTFRM) != 0U)) {
			*iDestination++ = stitch;
			++destCount;
		  }
		}
		StitchBuffer->resize(destCount);
	  }
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  clip::delclps(ClosestFormToCursor);
	  texture::deltx(ClosestFormToCursor);
	  form.fillType = 0;
	  form.edgeType = 0;
	  form.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	}
  }
}

void form::frm0() {
  if (StateMap->test(StateFlag::FRMPSEL)) {
	auto& form = FormList->operator[](ClosestFormToCursor);
	thred::savdo();
	form::rotfrm(form, ClosestVertexToCursor);
	ClosestVertexToCursor = 0;
	satin::satadj(form);
	form::refil();
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::rinfrm() {
  form::frmlin(*FormForInsert);
  SelectObject(StitchWindowMemDC, FormPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  auto& formLines = *FormLines;
  if ((FormVertexNext != 0U) || FormForInsert->type != FRMLINE) {
	wrap::Polyline(StitchWindowMemDC, &formLines[FormVertexPrev], LNPNTS);
  }
  InsertLine[0]   = formLines[FormVertexPrev];
  InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
  InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::SHOINSF);
  form::duinsf();
}

void form::infrm() { // insert multiple points into a form
  // clang-format off
	auto inOutFlag = intersectionStyles::POINT_IN_LINE;
  // clang-format on
  if (fi::closat(inOutFlag)) {
	FormForInsert = &(FormList->operator[](ClosestFormToCursor));
	if (inOutFlag != intersectionStyles::POINT_IN_LINE) {
	  if ((ClosestVertexToCursor == 0U) && FormForInsert->type == FRMLINE) {
		FormVertexPrev = 0;
		StateMap->set(StateFlag::PRELIN);
	  }
	  else {
		FormVertexPrev = form::prv(*FormForInsert, ClosestVertexToCursor);
		FormVertexNext = ClosestVertexToCursor;
	  }
	}
	else {
	  FormVertexNext = ClosestVertexToCursor;
	  FormVertexPrev = form::prv(*FormForInsert, ClosestVertexToCursor);
	}
	StateMap->set(StateFlag::INSFRM);
	StateMap->set(StateFlag::INIT);
	rinfrm();
  }
}

void form::setins() {
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  fi::nufpnt(FormVertexPrev, *FormForInsert, stitchPoint);
  if (StateMap->test(StateFlag::PRELIN)) {
	auto const vertexIt = wrap::next(FormVertices->begin(), FormForInsert->vertexIndex);
	std::swap(vertexIt[0], vertexIt[1]);
  }
  else {
	FormVertexPrev = form::nxt(*FormForInsert, FormVertexPrev);
	FormVertexNext = form::nxt(*FormForInsert, FormVertexPrev);
  }
  form::frmlin(*FormForInsert);
  InsertLine[0]   = FormLines->operator[](FormVertexPrev);
  InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
  InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::INSFRM);
  duinsf();
  StateMap->set(StateFlag::RESTCH);
}

void form::bsizpar(FRMHED& currentForm) noexcept {
  currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
  currentForm.edgeStitchLen      = UserStitchLength;
  currentForm.minBorderStitchLen = MinStitchLength;
}

void form::internal::sbord(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGELINE;
  form::bsizpar(form);
  form::refilfn();
}

void form::bord() {
  if (displayText::filmsgs(FML_LIN)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  wrap::narrow(form.borderColor, ActiveColor);
	  fi::sbord(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  wrap::narrow(form.borderColor, ActiveColor);
	  fi::sbord(ClosestFormToCursor);
	  thred::coltab();
	  StateMap->set(StateFlag::INIT);
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::internal::fsclp(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  form.edgeType       = EDGECLIP;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = ClipRectSize.cx;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  auto offsetStart = wrap::next(ClipPoints->begin(), form.borderClipData);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  clip::clpout(ClipRectSize.cy / 2);
  form::refilfn();
}

void form::fclp() {
  if (displayText::filmsgs(FML_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::savdo();
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fi::fsclp(selectedForm);
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::fsclp(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::internal::sapliq(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGEAPPL;
  if (UserFlagMap->test(UserFlag::DUND)) {
	form.edgeType |= EGUND;
  }
  form.edgeSpacing = LineSpacing / 2.0F;
  form.borderSize  = IniFile.borderWidth;
  form::bsizpar(form);
  wrap::narrow_cast(form.borderColor, ActiveColor | gsl::narrow_cast<uint8_t>(AppliqueColor << 4U));
  if (form.type != FRMLINE) {
	if (form.fillType == SAT && (form.satinGuideCount != 0U)) {
	  satin::delsac(ClosestFormToCursor);
	}
  }
  form.fillType = 0U;
  form::refilfn();
}

void form::apliq() {
  if (displayText::filmsgs(FML_APLQ)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& currentForm   = FormList->operator[](ClosestFormToCursor);
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::sapliq(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& currentForm = FormList->operator[](ClosestFormToCursor);
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::sapliq(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::setap() {
  AppliqueColor = ActiveColor;
  // NOLINTNEXTLINE (clang-diagnostic-sign-conversion)
  displayText::showMessage(IDS_APCOL, (AppliqueColor + 1U));
}

void form::internal::getbig(fRECTANGLE* allItemsRect) noexcept {
  *allItemsRect = fRECTANGLE {BIGFLOAT, 0.0F, 0.0F, BIGFLOAT};
  for (auto& iForm : *FormList) {
	auto const& trct = iForm.rectangle;
	if (trct.left < allItemsRect->left) {
	  allItemsRect->left = trct.left;
	}
	if (trct.top > allItemsRect->top) {
	  allItemsRect->top = trct.top;
	}
	if (trct.right > allItemsRect->right) {
	  allItemsRect->right = trct.right;
	}
	if (trct.bottom < allItemsRect->bottom) {
	  allItemsRect->bottom = trct.bottom;
	}
  }
  for (auto& stitch : *StitchBuffer) {
	if (stitch.x < allItemsRect->left) {
	  allItemsRect->left = stitch.x;
	}
	if (stitch.y > allItemsRect->top) {
	  allItemsRect->top = stitch.y;
	}
	if (stitch.x > allItemsRect->right) {
	  allItemsRect->right = stitch.x;
	}
	if (stitch.y < allItemsRect->bottom) {
	  allItemsRect->bottom = stitch.y;
	}
  }
}

void form::stchrct2px(fRECTANGLE const* stitchRect, RECT& screenRect) {
  auto stitchCoord = fPOINT {stitchRect->left, stitchRect->top};
  auto screenCoord = POINT {0L, 0L};
  thred::sCor2px(stitchCoord, screenCoord);
  screenRect.left = screenCoord.x;
  screenRect.top  = screenCoord.y;
  stitchCoord.x   = stitchRect->right;
  stitchCoord.y   = stitchRect->bottom;
  thred::sCor2px(stitchCoord, screenCoord);
  screenRect.right  = screenCoord.x;
  screenRect.bottom = screenCoord.y;
}

void form::selal() {
  StateMap->reset(StateFlag::FORMSEL);
  SelectedFormList->clear();
  StateMap->reset(StateFlag::SELBOX);
  StateMap->reset(StateFlag::GRPSEL);
  fi::getbig(AllItemsRect);
  ZoomRect = fRECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.y), wrap::toFloat(UnzoomedRect.x), 0.0F};
  ZoomFactor = 1;
  StateMap->reset(StateFlag::ZUMED);
  thred::movStch();
  NearestCount = 0;
  StateMap->reset(StateFlag::RUNPAT);
  thred::duzrat();
  form::stchrct2px(AllItemsRect, SelectedFormsRect);
  StateMap->set(StateFlag::BIGBOX);
  StateMap->set(StateFlag::RESTCH);
}

void form::setstrtch() {
  auto reference   = 0.0F;
  auto ratio       = 1.0F;
  auto stitchRect  = fRECTANGLE {};
  auto stitchPoint = fPOINT {};
  thred::savdo();
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	stitchRect = SelectedVerticesRect;
  }
  else {
	if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX)) {
	  form::pxrct2stch(SelectedFormsRect, stitchRect);
	  stitchPoint = fPOINT {stitchRect.right, stitchRect.bottom};
	}
	else {
	  stitchPoint = thred::pxCor2stch(Msg.pt);
	}
  }
  switch (SelectedFormControlVertex) {
	case 0: { // top control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.bottom;
		auto const offsetY = Msg.pt.y - StitchWindowOrigin.y;
		ratio              = wrap::toFloat(SelectedFormsRect.bottom - offsetY) /
		        wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  reference          = form.rectangle.bottom;
		  ratio              = (stitchPoint.y - reference) / (form.rectangle.top - reference);
		  form.rectangle.top = stitchPoint.y;
		}
		else {
		  reference = StitchRangeRect.bottom;
		  ratio     = (stitchPoint.y - reference) / (StitchRangeRect.top - reference);
		}
	  }
	  break;
	}
	case 1: { // right control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.left;
		auto const offsetX = Msg.pt.x - StitchWindowOrigin.x;
		ratio              = wrap::toFloat(offsetX - SelectedFormsRect.left) /
		        wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  auto& formRectangle = form.rectangle;
		  reference           = formRectangle.left;
		  ratio               = (stitchPoint.x - reference) / (formRectangle.right - reference);
		  formRectangle.right = stitchPoint.x;
		}
		else {
		  reference = StitchRangeRect.left;
		  ratio     = (stitchPoint.x - reference) / (StitchRangeRect.right - reference);
		}
	  }
	  break;
	}
	case 2: { // bottom control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.top;
		auto const offsetY = Msg.pt.y - StitchWindowOrigin.y;
		ratio              = wrap::toFloat(offsetY - SelectedFormsRect.top) /
		        wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  reference             = form.rectangle.top;
		  ratio                 = (stitchPoint.y - reference) / (form.rectangle.bottom - reference);
		  form.rectangle.bottom = stitchPoint.y;
		}
		else {
		  reference = StitchRangeRect.top;
		  ratio     = (stitchPoint.y - reference) / (StitchRangeRect.bottom - reference);
		}
	  }
	  break;
	}
	case 3: { // left control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.right;
		auto const offsetX = Msg.pt.x - StitchWindowOrigin.x;
		ratio              = wrap::toFloat(SelectedFormsRect.right - offsetX) /
		        wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  reference           = form.rectangle.right;
		  ratio               = (stitchPoint.x - reference) / (form.rectangle.left - reference);
		  form.rectangle.left = stitchPoint.x;
		}
		else {
		  reference = StitchRangeRect.right;
		  ratio     = (stitchPoint.x - reference) / (StitchRangeRect.left - reference);
		}
	  }
	  break;
	}
	default: {
	  throw; // we should never reach here
	}
  }
  if ((SelectedFormControlVertex & 1U) != 0U) { // right or left control selected
	if (StateMap->test(StateFlag::FPSEL)) {
	  auto currentVertex = SelectedFormVertices.start;
	  auto vertexIt      = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const vert = wrap::next(vertexIt, currentVertex);

		vert->x       = (vert->x - reference) * ratio + reference;
		currentVertex = pdir(form, currentVertex);
	  }
	  form::frmout(ClosestFormToCursor);
	  thred::setpsel();
	  refil();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		auto& formIter = FormList->operator[](iForm);
		auto vertexIt  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  vertexIt->x = (vertexIt->x - reference) * ratio + reference;
		  ++vertexIt;
		}
		form::frmout(iForm);
	  }
	  for (auto stitch : *StitchBuffer) {
		stitch.x = (stitch.x - reference) * ratio + reference;
	  }
	  selal();
	  return;
	}

	if (!SelectedFormList->empty()) {
	  for (auto selectedForm : (*SelectedFormList)) {
		auto& formIter = FormList->operator[](selectedForm);
		auto vertexIt  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  vertexIt->x = (vertexIt->x - reference) * ratio + reference;
		  ++vertexIt;
		}
	  }
	}
	else {
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		  vertexIt->x = (vertexIt->x - reference) * ratio + reference;
		  ++vertexIt;
		}
	  }
	  else {
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).x =
		      (StitchBuffer->operator[](iStitch).x - reference) * ratio + reference;
		}
	  }
	}
  }
  else {
	if (StateMap->test(StateFlag::FPSEL)) {
	  auto currentVertex = SelectedFormVertices.start;
	  auto vertexIt      = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const vert = wrap::next(vertexIt, currentVertex);

		vert->y       = (vert->y - reference) * ratio + reference;
		currentVertex = pdir(form, currentVertex);
	  }
	  form::frmout(ClosestFormToCursor);
	  thred::setpsel();
	  refil();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		auto& formIter = FormList->operator[](iForm);
		auto vertexIt  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  vertexIt->y = (vertexIt->y - reference) * ratio + reference;
		  ++vertexIt;
		}
		form::frmout(iForm);
	  }
	  for (auto& stitch : *StitchBuffer) {
		stitch.y = (stitch.y - reference) * ratio + reference;
	  }
	  selal();
	  return;
	}

	if (!SelectedFormList->empty()) {
	  for (auto selectedForm : (*SelectedFormList)) {
		auto& formIter = FormList->operator[](selectedForm);
		auto vertexIt  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  vertexIt->y = (vertexIt->y - reference) * ratio + reference;
		  ++vertexIt;
		}
	  }
	}
	else {
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		  vertexIt->y = (vertexIt->y - reference) * ratio + reference;
		  ++vertexIt;
		}
	  }
	  else {
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).y =
		      (StitchBuffer->operator[](iStitch).y - reference) * ratio + reference;
		}
	  }
	}
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  form::frmout(selectedForm);
	  ClosestFormToCursor = selectedForm;
	  refil();
	}
  }
  else if (StateMap->test(StateFlag::FORMSEL)) {
	refil();
  }
  StateMap->set(StateFlag::RESTCH);
}

void form::setexpand(float xyRatio) {
  auto size0     = fPOINT {};
  auto rectangle = fRECTANGLE {};
  thred::savdo();
  auto stitchPoint = fPOINT {};
  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
	wrap::narrow_cast(rectangle.left, SelectedFormsRect.left);
	wrap::narrow_cast(rectangle.top, SelectedFormsRect.top);
	wrap::narrow_cast(rectangle.right, SelectedFormsRect.right);
	wrap::narrow_cast(rectangle.bottom, SelectedFormsRect.bottom);

	wrap::narrow_cast(stitchPoint.x, Msg.pt.x - StitchWindowOrigin.x);
	wrap::narrow_cast(stitchPoint.y, Msg.pt.y - StitchWindowOrigin.y);
	size0.y = rectangle.bottom - rectangle.top;
  }
  else {
	stitchPoint = thred::pxCor2stch(Msg.pt);
	if (StateMap->test(StateFlag::FORMSEL)) {
	  rectangle = FormList->operator[](ClosestFormToCursor).rectangle;
	}
	else {
	  rectangle = StitchRangeRect;
	}
	size0.y = rectangle.top - rectangle.bottom;
  }
  auto ratio     = fPOINT {1.0F, 1.0F};
  auto reference = fPOINT {};
  size0.x        = rectangle.right - rectangle.left;
  switch (SelectedFormControlVertex) {
	case 0: {
	  reference.x = rectangle.right;
	  reference.y = rectangle.bottom;
	  auto size1  = fPOINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  auto const aspect = size1.x / size1.y;
	  if (aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.left  = rectangle.right - size1.x;
		formRect.top   = rectangle.bottom + size1.y;
	  }
	  break;
	}
	case 1: {
	  reference.x = rectangle.left;
	  reference.y = rectangle.bottom;
	  auto size1  = fPOINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  auto const aspect = size1.x / size1.y;
	  if (aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.right = rectangle.left + size1.x;
		formRect.top   = rectangle.bottom + size1.y;
	  }
	  break;
	}
	case 2: {
	  reference.x = rectangle.left;
	  reference.y = rectangle.top;
	  auto size1  = fPOINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  auto const aspect = size1.x / size1.y;
	  if (aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect  = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.right  = rectangle.left + size1.x;
		formRect.bottom = rectangle.top - size1.y;
	  }
	  break;
	}
	case 3: {
	  reference.x = rectangle.right;
	  reference.y = rectangle.top;
	  auto size1  = fPOINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  auto const aspect = size1.x / size1.y;
	  if (aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect  = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.left   = rectangle.right - size1.x;
		formRect.bottom = rectangle.top - size1.y;
	  }
	  break;
	}
	default: {
	  throw; // we should never reach here
	}
  }
  auto const integerReference = POINT {std::lround(reference.x), std::lround(reference.y)};
  auto const stitchReference  = fi::px2stchf(integerReference);
  if (StateMap->test(StateFlag::FPSEL)) {
	auto vertexIt = wrap::next(FormVertices->begin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
	auto iCurrent = SelectedFormVertices.start;
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const vert = wrap::next(vertexIt, iCurrent);

	  vert->x  = (vert->x - stitchReference.x) * ratio.x + stitchReference.x;
	  vert->y  = (vert->y - stitchReference.y) * ratio.y + stitchReference.y;
	  iCurrent = form::pdir(FormList->operator[](ClosestFormToCursor), iCurrent);
	}
	thred::setpsel();
	form::frmout(ClosestFormToCursor);
	form::refil();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  auto& formIter                      = FormList->operator[](iForm);
	  auto                       vertexIt = wrap::next(FormVertices->begin(), formIter.vertexIndex);
	  auto const&                formVertexCount = formIter.vertexCount;
	  for (auto iVertex = 0U; iVertex < formVertexCount; ++iVertex) {
		vertexIt->x = (vertexIt->x - stitchReference.x) * ratio.x + stitchReference.x;
		vertexIt->y = (vertexIt->y - stitchReference.y) * ratio.y + stitchReference.y;
		++vertexIt;
	  }
	  form::frmout(iForm);
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.x = (stitch.x - stitchReference.x) * ratio.x + stitchReference.x;
	  stitch.y = (stitch.y - stitchReference.y) * ratio.y + stitchReference.y;
	}
	form::selal();
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);

	  auto const& formVertexCount = form.vertexCount;
	  auto        vertexIt        = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < formVertexCount; ++iVertex) {
		vertexIt->x = (vertexIt->x - stitchReference.x) * ratio.x + stitchReference.x;
		vertexIt->y = (vertexIt->y - stitchReference.y) * ratio.y + stitchReference.y;
		++vertexIt;
	  }
	  form::frmout(selectedForm);
	  ClosestFormToCursor = selectedForm;
	  form::refil();
	}
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);

	  auto vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		vertexIt->x = (vertexIt->x - reference.x) * ratio.x + reference.x;
		vertexIt->y = (vertexIt->y - reference.y) * ratio.y + reference.y;
		++vertexIt;
	  }
	  form::refil();
	}
	else {
	  auto stitchIt = wrap::next(StitchBuffer->begin(), GroupStartStitch);
	  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		stitchIt->x = (stitchIt->x - reference.x) * ratio.x + reference.x;
		stitchIt->y = (stitchIt->y - reference.y) * ratio.y + reference.y;
		++stitchIt;
	  }
	}
  }
  StateMap->set(StateFlag::RESTCH);
}

void form::nufilcol(uint8_t color) noexcept {
  // clang-format off
  auto& formColor = FormList->operator[](ClosestFormToCursor).fillColor;
  // clang-format on
  if (formColor != color) {
	formColor            = color;
	auto const attribute = (ClosestFormToCursor << FRMSHFT) | FRMFIL;
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (FRMSK | TYPMSK | FTHMSK)) == attribute) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color;
	  }
	}
  }
}

void form::nufthcol(uint8_t color) noexcept {
  auto& formColor = FormList->operator[](ClosestFormToCursor).fillInfo.feather.color;
  if (formColor != color) {
	formColor            = color;
	auto const attribute = (ClosestFormToCursor << FRMSHFT) | FTHMSK;
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (FRMSK | FTHMSK)) == attribute) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color;
	  }
	}
  }
}

void form::nubrdcol(uint8_t color) noexcept {
  FormList-> operator[](ClosestFormToCursor).borderColor = color;
  auto const attribute = (ClosestFormToCursor << FRMSHFT) | FRMBFIL;
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & (FRMSK | TYPMSK)) == attribute) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= color;
	}
  }
}

void form::nulapcol(uint8_t color) {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (gsl::narrow<decltype(color)>(currentForm.borderColor >> FRMSHFT) != color) {
	currentForm.borderColor &= COLMSK;
	currentForm.borderColor |= gsl::narrow_cast<uint8_t>(color << FRMSHFT);
	auto const attribute = (ClosestFormToCursor << FRMSHFT) | TYPMSK;
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (TYPMSK | FRMSK)) == attribute) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color;
	  }
	}
  }
}

void form::internal::sbold(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGEBEAN;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  form::refilfn();
}

void form::dubold() {
  if (displayText::filmsgs(FML_BLD)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  fi::sbold(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  fi::sbold(ClosestFormToCursor);
	  thred::coltab();
	  StateMap->set(StateFlag::INIT);
	  StateMap->set(StateFlag::RESTCH);
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	}
  }
}

void form::sethup() noexcept {
  if (IniFile.hoopSizeX == LHUPX && IniFile.hoopSizeY == LHUPY) {
	IniFile.hoopType = LARGHUP;
	return;
  }
  if (IniFile.hoopSizeX == SHUPX && IniFile.hoopSizeY == SHUPY) {
	IniFile.hoopType = SMALHUP;
	return;
  }
  if (IniFile.hoopSizeX == HUP100XY && IniFile.hoopSizeY == HUP100XY) {
	IniFile.hoopType = HUP100;
	return;
  }
  IniFile.hoopType = CUSTHUP;
}

void form::dustar(uint32_t starCount, float length) {
  constexpr auto STARMIN = 3U;   // minimum star vertices
  constexpr auto STARMAX = 100U; // maximum star vertices
  if (starCount < STARMIN) {
	starCount = STARMIN;
  }
  if (starCount > STARMAX) {
	starCount = STARMAX;
  }
  auto const stepAngle   = PI_F / wrap::toFloat(starCount);
  auto       angle       = stepAngle / 2.0F + PI_F;
  auto const vertexCount = starCount * 2U;
  auto       newForm     = FRMHED {};
  newForm.vertexIndex    = thred::adflt(vertexCount);
  newForm.vertexCount    = vertexCount;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  newForm.type = FRMFPOLY;
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  auto point          = thred::pxCor2stch(Msg.pt);
  StateMap->set(StateFlag::FILDIR);
  auto vBegin   = wrap::next(FormVertices->begin(), newForm.vertexIndex);
  auto vertexIt = vBegin;
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	vertexIt->x = point.x;
	vertexIt->y = point.y;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle += stepAngle;
	++vertexIt;
  }
  vertexIt          = vBegin;
  auto       vEnd   = wrap::next(vertexIt, starCount);
  auto const center = fPOINT {wrap::midl(vEnd->x, vBegin->x), wrap::midl(vEnd->y, vBegin->y)};
  ++vertexIt;
  for (auto iVertex = 1U; iVertex < starCount; ++iVertex) {
	vertexIt->x = (vertexIt->x - center.x) * StarRatio + center.x;
	vertexIt->y = (vertexIt->y - center.y) * StarRatio + center.y;
	vertexIt += 2;
  }
  vertexIt->x = (vertexIt->x - center.x) * StarRatio + center.x;
  vertexIt->y = (vertexIt->y - center.y) * StarRatio + center.y;
  form::frmout(ClosestFormToCursor);
  FormMoveDelta      = fPOINT {};
  NewFormVertexCount = vertexCount + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::duspir(uint32_t stepCount) {
  constexpr auto STEPMIN = 3U;   // Spiral step minmum
  constexpr auto STEPMAX = 100U; // spiral step maximum
  if (stepCount < STEPMIN) {
	stepCount = STEPMIN;
  }
  if (stepCount > STEPMAX) {
	stepCount = STEPMAX;
  }
  auto const stepAngle = PI_F2 / wrap::toFloat(stepCount);
  auto const length    = 800.0F / wrap::toFloat(stepCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
  auto newForm        = FRMHED {};
  auto vertexCount    = wrap::round<uint32_t>(wrap::toFloat(stepCount) * SpiralWrap);
  newForm.vertexIndex = thred::adflt(vertexCount);
  newForm.vertexCount = vertexCount;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  auto firstSpiral    = std::vector<fPOINT> {};
  firstSpiral.resize(stepCount);
  auto centeredSpiral = std::vector<fPOINT> {};
  centeredSpiral.resize(stepCount);
  auto point = thred::pxCor2stch(Msg.pt);
  auto angle = 0.0F;
  for (auto iStep = 0U; iStep < stepCount; ++iStep) {
	firstSpiral[iStep].x = point.x;
	firstSpiral[iStep].y = point.y;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle += stepAngle;
  }
  auto const center = fPOINT {wrap::midl(firstSpiral[stepCount / 2].x, firstSpiral[0].x),
                              wrap::midl(firstSpiral[stepCount / 2].y, firstSpiral[0].y)};
  for (auto iStep = 0U; iStep < stepCount; ++iStep) {
	centeredSpiral[iStep].x = firstSpiral[iStep].x - center.x;
	centeredSpiral[iStep].y = firstSpiral[iStep].y - center.y;
  }
  auto const stepRatio = 1.0F / wrap::toFloat(vertexCount);
  auto       ratio     = stepRatio;
  auto& currentForm    = FormList->operator[](ClosestFormToCursor);
  auto vertexIt        = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	vertexIt->x = centeredSpiral[iVertex % stepCount].x * ratio + center.x;
	vertexIt->y = centeredSpiral[iVertex % stepCount].y * ratio + center.y;
	++vertexIt;
	ratio += stepRatio;
  }
  currentForm.type = FRMLINE;
  form::frmout(ClosestFormToCursor);
  FormMoveDelta      = fPOINT {};
  NewFormVertexCount = vertexCount + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::duhart(uint32_t sideCount) {
  constexpr auto HARTANG  = 0.28F; // heart angle in radians
  constexpr auto HARTINFL = 0.7F;  // heart inflection point in radians
  constexpr auto HARTSTEP = 4.5F;
  constexpr auto HSIDEMIN = 6U;   // minimum number of heart vertices
  constexpr auto HSIDEMAX = 100U; // maximum number of heart vertices
  if (sideCount > HSIDEMAX) {
	sideCount = HSIDEMAX;
  }
  if (sideCount < HSIDEMIN) {
	sideCount = HSIDEMIN;
  }
  FormList->push_back(FRMHED {});
  auto& currentForm       = FormList->back();
  currentForm.vertexIndex = wrap::toUnsigned(FormVertices->size());
  wrap::narrow(currentForm.attribute, ActiveLayer << 1U);
  FormVertices->reserve(FormVertices->size() + wrap::toSize(sideCount) * 2U - 2U);
  auto const savedVertexIndex = wrap::toUnsigned(FormVertices->size());
  auto       point            = thred::pxCor2stch(Msg.pt);
  auto       stepAngle        = PI_F2 / wrap::toFloat(sideCount);
  auto const length           = 300.0F / wrap::toFloat(sideCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
  auto angle    = PI_F * HARTANG;
  auto iVertex  = ptrdiff_t {0};
  auto maximumX = 0.0F;
  while (angle > -PI_F * HARTINFL) {
	if (point.x > maximumX) {
	  maximumX = point.x;
	}
	FormVertices->push_back(point);
	++iVertex;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle -= stepAngle;
  }
  stepAngle /= HARTSTEP;
  auto       lastVertex = iVertex;
  auto const vBegin     = wrap::next(FormVertices->begin(), savedVertexIndex);
  while (point.x > vBegin->x && iVertex < wrap::ptrdiff(2U * HSIDEMAX)) {
	FormVertices->push_back(fPOINT {point.x, point.y});
	++iVertex;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle -= stepAngle;
  }
  auto const firstVertex = iVertex;

  auto const nVert    = std::next(vBegin, lastVertex - 1);
  auto const ratio    = (nVert->x - vBegin->x) / (nVert->x - vBegin[firstVertex - 1].x);
  auto       vertexIt = std::next(vBegin, lastVertex);
  for (iVertex = lastVertex; iVertex < firstVertex; ++iVertex) {
	vertexIt->x = (vertexIt->x - nVert->x) * ratio + nVert->x;
	++vertexIt;
  }
  auto iDestination = wrap::toUnsigned(iVertex);
  lastVertex        = iVertex;
  iVertex           = lastVertex - 2;
  vertexIt          = std::next(vBegin, iVertex);
  for (; iVertex != 0; --iVertex) {
	FormVertices->push_back(
	    fPOINT {maximumX + maximumX - vertexIt->x - 2 * (maximumX - vBegin->x), vertexIt->y});
	--vertexIt;
	++iDestination;
  }
  NewFormVertexCount      = iDestination + 1U;
  currentForm.vertexCount = iDestination;
  currentForm.type        = FRMFPOLY;
  ClosestFormToCursor     = wrap::toUnsigned(FormList->size() - 1U);
  form::frmout(ClosestFormToCursor);
  FormMoveDelta = fPOINT {};
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::dulens(uint32_t sides) {
  if (sides < 4) {
	sides = 4;
  }
  /*
      if (sides > 48) {
          sides = 48;
      }
  */
  auto const steps = sides >> 1U;
  // Calculate the angle subtended by the arc of the lens which is determined by the aspect ratio
  auto const theta = std::atan(1.0F / IniFile.lensRatio);
  // now calculate the radius of the lens arc and scale by the zoom factor
  auto const radius = (100.0F / std::sin(theta)) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
  // get the angle subtended by each step
  auto const omega = 2.0F * theta / wrap::toFloat(steps);
  // and the interior angle of every segment
  auto const phi = (PI_F - omega) / 2.0F;
  // as well as the length of every segment
  auto const stepLength = 2.0F * radius * std::cos(phi);
  FormList->push_back(FRMHED {});
  auto& currentForm       = FormList->back();
  ClosestFormToCursor     = wrap::toUnsigned(FormList->size() - 1U);
  currentForm.vertexIndex = wrap::toUnsigned(FormVertices->size());
  currentForm.attribute   = gsl::narrow_cast<decltype(currentForm.attribute)>(ActiveLayer << 1U);
  auto const stitchPoint  = thred::pxCor2stch(Msg.pt);
  auto       point        = stitchPoint;
  auto       iVertex      = 0U;
  FormVertices->reserve(FormVertices->size() + wrap::toSize(steps << 1U) + 1U);
  // rho is the angle of this particular segment
  auto rho = PI_F - theta - phi;
  for (auto iStep = 0U; iStep < steps; ++iStep) {
	FormVertices->push_back(point);
	point.x += stepLength * cos(rho);
	point.y += stepLength * sin(rho);
	rho += omega;
	++iVertex;
  }
  FormVertices->push_back(point);
  auto       vertexIt = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex + iVertex - 1U);
  auto const av       = stitchPoint.x;
  for (; iVertex != 1; --iVertex) {
	FormVertices->push_back(fPOINT {av + av - (*vertexIt).x, (*vertexIt).y});
	--vertexIt;
  }
  currentForm.vertexCount = wrap::toUnsigned(FormVertices->size() - currentForm.vertexIndex);
  NewFormVertexCount      = currentForm.vertexCount + 1U;
  currentForm.type        = FRMFPOLY;
  form::frmout(ClosestFormToCursor);
  FormMoveDelta = fPOINT {};
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

constexpr auto form::internal::shreg(float highValue, float reference, float eggRatio) noexcept -> float {
  return (highValue - reference) * eggRatio + reference;
}

void form::dueg(uint32_t sides) {
  constexpr auto ESIDEMIN = 8U; // mimimum number of egg vertices
  if (sides < ESIDEMIN) {
	sides = ESIDEMIN;
  }
  form::durpoli(sides);
  auto const& form      = FormList->back();
  auto        vertexIt  = wrap::next(FormVertices->begin(), form.vertexIndex);
  auto const  reference = wrap::midl(vertexIt[sides / 2].y, vertexIt[0].y);
  auto const  maximumY  = vertexIt[sides >> 2U].y - vertexIt[0].y;
  for (uint32_t iVertex = 0; iVertex < sides; ++iVertex) {
	if (vertexIt->y < reference) {
	  vertexIt->y = reference - (reference - vertexIt->y) * IniFile.eggRatio;
	}
	++vertexIt;
  }
  auto const eggRatio = maximumY / (vertexIt[sides >> 2U].y - vertexIt[0].y);
  vertexIt            = wrap::next(FormVertices->begin(), form.vertexIndex);
  auto const ref      = vertexIt[0];
  for (uint32_t iVertex = 1; iVertex < form.vertexCount; ++iVertex) {
	*vertexIt = fPOINT {fi::shreg(vertexIt->x, ref.x, eggRatio), fi::shreg(vertexIt->y, ref.y, eggRatio)};
	++vertexIt;
  }
}

void form::duzig(uint32_t vertices) {
  constexpr auto ZVERTMIN = 3U;   // mimimum number of zig-zag vertices
  constexpr auto ZVERTMAX = 100U; // maximum number of zig-zag vertices
  if (vertices < ZVERTMIN) {
	vertices = ZVERTMIN;
  }
  if (vertices > ZVERTMAX) {
	vertices = ZVERTMAX;
  }
  auto newForm        = FRMHED {};
  newForm.vertexIndex = thred::adflt(vertices);
  newForm.vertexCount = vertices;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  FormList->push_back(newForm);
  ClosestFormToCursor    = wrap::toUnsigned(FormList->size() - 1U);
  auto       stitchPoint = thred::pxCor2stch(Msg.pt);
  auto const offset      = fPOINT {UnzoomedRect.x / 6.0, UnzoomedRect.y / (6.0 * vertices)};
  auto vertexIt = wrap::next(FormVertices->begin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
  for (auto iVertex = 0U; iVertex < vertices; ++iVertex) {
	*vertexIt = stitchPoint;
	++vertexIt;
	stitchPoint.y -= offset.y;
	if ((iVertex & 1U) != 0U) {
	  stitchPoint.x += offset.x;
	}
	else {
	  stitchPoint.x -= offset.x;
	}
  }
  FormList->back().type = FRMLINE;
  form::frmout(ClosestFormToCursor);
  FormMoveDelta      = fPOINT {};
  NewFormVertexCount = vertices + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::fliph() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	thred::savdo();
	auto const offset        = SelectedVerticesRect.right + SelectedVerticesRect.left;
	auto       currentVertex = SelectedFormVertices.start;
	auto const vBegin        = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const vertexIt = wrap::next(vBegin, currentVertex);
	  vertexIt->x         = offset - vertexIt->x;
	  currentVertex       = form::pdir(form, currentVertex);
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	thred::savdo();
	auto const offset = AllItemsRect->right + AllItemsRect->left;
	for (auto& FormVertice : *FormVertices) {
	  FormVertice.x = offset - FormVertice.x;
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.x = offset - stitch.x;
	}
	for (auto& iForm : *FormList) {
	  auto& rect = iForm.rectangle;
	  rect.left  = offset - rect.left;
	  rect.right = offset - rect.right;
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	auto formMap   = boost::dynamic_bitset<>(FormList->size());
	auto rectangle = fRECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, rectangle);
	auto const offset = rectangle.right + rectangle.left;
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  formMap.set(selectedForm);
	  // clang-format off
	  auto& formIter = FormList->operator[](selectedForm);
	  auto  vertexIt = wrap::next(FormVertices->begin(), formIter.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		vertexIt->x = offset - vertexIt->x;
		++vertexIt;
	  }
	  form::frmout(selectedForm);
	}
	for (auto& stitch : *StitchBuffer) {
	  auto const decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	  if (formMap.test(decodedForm) && ((stitch.attribute & NOTFRM) == 0U)) {
		stitch.x = offset - stitch.x;
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto const& srcRect  = form.rectangle;
	  auto const  offset   = srcRect.right + srcRect.left;
	  auto        vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		vertexIt->x = offset - vertexIt->x;
		++vertexIt;
	  }
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor &&
		    ((stitch.attribute & NOTFRM) == 0U)) {
		  stitch.x = offset - stitch.x;
		}
	  }
	  form::frmout(ClosestFormToCursor);
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto rectangle = fRECTANGLE {};
		thred::selRct(rectangle);
		auto const offset = rectangle.right + rectangle.left;
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).x = offset - StitchBuffer->operator[](iStitch).x;
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::internal::prpsbrd(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  if (form.vertexCount > 2) {
	clip::deleclp(formIndex);
	form.edgeType = EDGEPROPSAT;
	if (UserFlagMap->test(UserFlag::DUND)) {
	  form.edgeType |= EGUND;
	}
	form::bsizpar(form);
	form.borderSize  = BorderWidth;
	form.edgeSpacing = LineSpacing;
	wrap::narrow(form.borderColor, ActiveColor);
	form::refilfn();
  }
}

void form::prpbrd(float borderStitchSpacing) {
  auto const savedSpacing = LineSpacing;
  if (displayText::filmsgs(FML_PRPS)) {
	return;
  }
  LineSpacing = borderStitchSpacing;
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor    = selectedForm;
	  auto& currentForm      = FormList->operator[](ClosestFormToCursor);
	  currentForm.borderSize = LineSpacing;
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::prpsbrd(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& currentForm = FormList->operator[](ClosestFormToCursor);
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::prpsbrd(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
  LineSpacing = savedSpacing;
}

void form::tglfrm() {
  if (StateMap->testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  StateMap->reset(StateFlag::HIDSTCH);
  if (StateMap->testAndFlip(StateFlag::FRMOF)) {
	FormOnOff->assign(displayText::loadStr(IDS_FRMPLUS));
  }
  else {
	FormOnOff->assign(displayText::loadStr(IDS_FRMINUS));
	StateMap->reset(StateFlag::FORMSEL);
	StateMap->reset(StateFlag::FORMIN);
	StateMap->reset(StateFlag::MOVFRM);
	StateMap->reset(StateFlag::FRMPMOV);
	StateMap->reset(StateFlag::FRMOV);
	StateMap->reset(StateFlag::SATIN);
	StateMap->reset(StateFlag::SATPNT);
	StateMap->reset(StateFlag::SATCNKT);
	StateMap->reset(StateFlag::FENDIN);
	StateMap->reset(StateFlag::DELFRM);
	StateMap->reset(StateFlag::FRMPSEL);
	StateMap->reset(StateFlag::INSFRM);
  }
  SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
  StateMap->set(StateFlag::DUMEN);
  StateMap->set(StateFlag::RESTCH);
}

void form::frmon() {
  thred::unbsho();
  StateMap->reset(StateFlag::FRMOF);
  FormOnOff->assign(displayText::loadStr(IDS_FRMPLUS));
  SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
  StateMap->set(StateFlag::DUMEN);
}

void form::internal::fnord() {
  // clang-format off
  auto const& currentForm = FormList->operator[](ClosestFormToCursor);
  auto const  start       = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  auto const  end         = wrap::next(start, currentForm.vertexCount);
  // clang-format on
  std::reverse(start, end);
  form::refil();
}

void form::flpord() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	thred::savdo();
	auto start  = SelectedFormVertices.start;
	auto finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % form.vertexCount;
	auto const vBegin = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= (SelectedFormVertices.vertexCount / 2); ++iVertex) {
	  auto vStart = wrap::next(vBegin, start);
	  auto vEnd   = wrap::next(vBegin, finish);
	  std::swap(*vStart, *vEnd);
	  start = form::pdir(form, start);
	  StateMap->flip(StateFlag::PSELDIR);
	  finish = form::pdir(form, finish);
	  StateMap->flip(StateFlag::PSELDIR);
	}
	form::refil();
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  fi::fnord();
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::fnord();
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto const start = wrap::next(StitchBuffer->begin(), GroupStartStitch);
		auto const end   = wrap::next(StitchBuffer->begin(), GroupEndStitch);
		std::reverse(start, end);
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::internal::filsclp() {
  texture::deltx(ClosestFormToCursor);
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (currentForm.type != SAT) {
	currentForm.wordParam = 0;
  }
  currentForm.type                    = SAT;
  currentForm.fillType                = CLPF;
  currentForm.angleOrClipData.clip    = clip::numclp();
  currentForm.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
  auto offsetStart = wrap::next(ClipPoints->begin(), currentForm.angleOrClipData.clip);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  form::refilfn();
}

void form::clpfil() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN && ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
			  fi::filsclp();
			}
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::filsclp();
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::internal::snpfn(std::vector<uint32_t> const& xPoints, uint32_t start, uint32_t end, uint32_t finish) noexcept {
  if (finish != start) {
	for (auto current = start; current < end; ++current) {
	  auto const reference = xPoints[current];
	  for (auto iPoint = current + 1U; iPoint < finish; ++iPoint) {
		auto const check = xPoints[iPoint];
		auto const CheckLength =
		    hypot(StitchBuffer->operator[](check).x - StitchBuffer->operator[](reference).x,
		          StitchBuffer->operator[](check).y - StitchBuffer->operator[](reference).y);
		if (CheckLength < SnapLength) {
		  StitchBuffer->operator[](check) = StitchBuffer->operator[](reference);
		}
	  }
	}
  }
}

void form::internal::doTimeWindow(float                        rangeX,
                                  std::vector<uint32_t> const& xPoints,
                                  std::vector<uint32_t> const& xHistogram) {
  auto const checkLength = wrap::round<uint32_t>(SnapLength * 2.0F + 1.0F);
  // NOLINTNEXTLINE(hicpp-signed-bitwise,readability-qualified-auto)
  auto const timeWindow = CreateWindow(L"STATIC",
                                       nullptr,
                                       WS_CHILD | WS_VISIBLE | WS_BORDER,
                                       ButtonWidthX3,
                                       0,
                                       StitchWindowSize.x,
                                       ButtonHeight,
                                       ThrEdWindow,
                                       nullptr,
                                       ThrEdInstance,
                                       nullptr);

  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const timeDC       = GetDC(timeWindow);
  auto const timeStep     = wrap::toFloat(StitchWindowSize.x) / rangeX;
  auto       timePosition = 0.0F;
  auto&      formLines    = *FormLines;
  formLines.clear();
  formLines.push_back(POINT {0, 0});
  formLines.push_back(POINT {0, ButtonHeight});
  SelectObject(timeDC, thred::getUserPen(0U));
  for (auto iColumn = 1U; iColumn < wrap::round<uint32_t>(rangeX) - checkLength - 1U; ++iColumn) {
	snpfn(xPoints,
	      xHistogram[iColumn],
	      xHistogram[wrap::toSize(iColumn) + 1U],
	      xHistogram[wrap::toSize(iColumn) + checkLength]);
	wrap::Polyline(timeDC, formLines.data(), LNPNTS);
	timePosition += timeStep;
	formLines[0].x = formLines[1].x = std::lround(timePosition);
  }
  DestroyWindow(timeWindow);
}

void form::internal::snp(uint32_t start, uint32_t finish) {
  auto range = fPOINT {};
  thred::chkrng(range);
  auto xPoints = std::vector<uint32_t> {};
  xPoints.resize(StitchBuffer->size());
  auto xHistogram = std::vector<uint32_t> {};
  xHistogram.resize(wrap::round<size_t>(range.x) + 1U);
  auto const attribute = (ClosestFormToCursor << 4U) & FRMSK;
  if (StateMap->test(StateFlag::FORMSEL)) {
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  if (((StitchBuffer->operator[](iStitch).attribute & NOTFRM) == 0U) &&
	      (StitchBuffer->operator[](iStitch).attribute & FRMSK) == attribute) {
		auto const iColumn = wrap::floor<uint32_t>(StitchBuffer->operator[](iStitch).x);
		++(xHistogram[iColumn]);
	  }
	}
  }
  else {
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  auto const iColumn = wrap::floor<uint32_t>(StitchBuffer->operator[](iStitch).x);
	  ++(xHistogram[iColumn]);
	}
  }
  auto       accumulator = 0U;
  auto const endColumn   = wrap::round<uint32_t>(range.x);
  for (auto iColumn = 0U; iColumn < endColumn; ++iColumn) {
	auto const value    = xHistogram[iColumn];
	xHistogram[iColumn] = accumulator;
	accumulator += value;
  }
  xHistogram[endColumn] = accumulator;
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto iStitch = 0U;
	for (auto& stitch : *StitchBuffer) {
	  if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == attribute) {
		auto const iColumn             = wrap::floor<uint32_t>(stitch.x);
		xPoints[xHistogram[iColumn]++] = iStitch;
	  }
	  ++iStitch;
	}
  }
  else {
	auto iStitch = 0U;
	for (auto& stitch : *StitchBuffer) {
	  auto const iColumn             = wrap::floor<uint32_t>(stitch.x);
	  xPoints[xHistogram[iColumn]++] = iStitch;
	  ++iStitch;
	}
  }
  doTimeWindow(range.x, xPoints, xHistogram);
}

void form::snap() {
  thred::savdo();
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	fi::snp(GroupStartStitch, GroupEndStitch);
  }
  else {
	fi::snp(0, wrap::toUnsigned(StitchBuffer->size()));
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::internal::dufcntr(fPOINT& center) noexcept {
  auto bigRect = FormList->operator[](SelectedFormList->front()).rectangle;
  for (auto selectedForm : (*SelectedFormList)) {
	auto const formRect = FormList->operator[](selectedForm).rectangle;
	if (formRect.left < bigRect.left) {
	  bigRect.left = formRect.left;
	}
	if (formRect.top > bigRect.top) {
	  bigRect.top = formRect.top;
	}
	if (formRect.right > bigRect.right) {
	  bigRect.right = formRect.right;
	}
	if (formRect.bottom < bigRect.bottom) {
	  bigRect.bottom = formRect.bottom;
	}
  }
  center = fPOINT {wrap::midl(bigRect.right, bigRect.left), wrap::midl(bigRect.top, bigRect.bottom)};
}

auto form::rotpar() -> fPOINT {
  do {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  RotationRect = FormList->operator[](ClosestFormToCursor).rectangle;
	  StateMap->set(StateFlag::FRMROT);
	  break;
	}
	if (!SelectedFormList->empty()) {
	  form::pxrct2stch(SelectedFormsRect, RotationRect);
	  StateMap->set(StateFlag::FRMSROT);
	  break;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  RotationRect = *AllItemsRect;
	  break;
	}
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  thred::selRct(RotationRect);
	  break;
	}
	if (StateMap->test(StateFlag::FPSEL)) {
	  RotationRect = SelectedVerticesRect;
	  break;
	}
  } while (false);
  if (StateMap->test(StateFlag::GMRK)) {
	return ZoomMarkPoint;
  }
  return fPOINT {wrap::midl(RotationRect.right, RotationRect.left),
                 wrap::midl(RotationRect.top, RotationRect.bottom)};
}

void form::internal::rotentr(float rotationAngle) {
  displayText::showMessage(IDS_ROTA, (rotationAngle * RADDEGF));
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void form::setrang() {
  fi::rotentr(IniFile.rotationAngle);
  StateMap->set(StateFlag::NUROT);
}

void form::rotcmd() {
  if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL) ||
      !SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
	fi::rotentr(IniFile.rotationAngle);
	StateMap->set(StateFlag::ENTROT);
  }
  else {
	displayText::shoseln(IDS_ALLX, IDS_ROTCMD);
  }
}

void form::internal::fnagain(float rotationAngle) {
  if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL) ||
      !SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
	auto const rotationCenter = form::rotpar();
	thred::rotfn(rotationAngle, rotationCenter);
  }
  else {
	displayText::alrotmsg();
  }
}

void form::rotagain() {
  fi::fnagain(IniFile.rotationAngle);
}

void form::bakagain() {
  fi::fnagain(PI_F2 - IniFile.rotationAngle);
}

void form::rotdup() {
  if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL) || !SelectedFormList->empty()) {
	fi::rotentr(IniFile.rotationAngle);
	StateMap->set(StateFlag::ENTRDUP);
  }
  else {
	displayText::shord();
  }
}

void form::internal::adfrm(uint32_t iForm) {
  FormList->push_back(FormList->operator[](iForm));
  auto& currentForm       = FormList->back();
  auto& srcForm           = FormList->operator[](iForm);
  ClosestFormToCursor     = wrap::toUnsigned(FormList->size() - 1U);
  currentForm.vertexIndex = wrap::toUnsigned(FormVertices->size());
  auto const vertexIt     = wrap::next(FormVertices->cbegin(), srcForm.vertexIndex);
  FormVertices->insert(FormVertices->end(), vertexIt, wrap::next(vertexIt, srcForm.vertexCount));
  if (srcForm.type == SAT && (srcForm.satinGuideCount != 0U)) {
	currentForm.satinOrAngle.guide = wrap::toUnsigned(SatinGuides->size());

	auto const guideStart = wrap::next(SatinGuides->cbegin(), srcForm.satinOrAngle.guide);
	SatinGuides->insert(SatinGuides->end(), guideStart, wrap::next(guideStart, srcForm.satinGuideCount));
  }
  if (clip::iseclpx(srcForm)) {
	currentForm.borderClipData = wrap::toUnsigned(ClipPoints->size());

	auto const srcStart = wrap::next(ClipPoints->cbegin(), srcForm.borderClipData);
	ClipPoints->insert(ClipPoints->end(), srcStart, wrap::next(srcStart, srcForm.clipEntries));
  }
  if (clip::isclpx(srcForm)) {
	currentForm.angleOrClipData.clip = wrap::toUnsigned(ClipPoints->size());

	auto const srcStart = wrap::next(ClipPoints->cbegin(), srcForm.angleOrClipData.clip);
	ClipPoints->insert(ClipPoints->end(), srcStart, wrap::next(srcStart, srcForm.lengthOrCount.clipCount));
  }
}

void form::duprot(float rotationAngle) {
  thred::savdo();
  fi::adfrm(ClosestFormToCursor);
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
  form::refil();
  StateMap->set(StateFlag::FORMSEL);
  StateMap->set(StateFlag::RESTCH);
}

void form::internal::duprotfs(float rotationAngle) {
  auto const rotationCenter = form::rotpar();
  for (auto selectedForm : (*SelectedFormList)) {
	adfrm(selectedForm);
	thred::rotfn(rotationAngle, rotationCenter);
  }
}

void form::internal::duprots(float rotationAngle, fPOINT const& rotationCenter) {
  thred::rngadj();
  ClosestPointIndex   = wrap::toUnsigned(StitchBuffer->size());
  auto       sourceIt = wrap::next(StitchBuffer->cbegin(), GroupStartStitch);
  auto const endPoint = wrap::next(StitchBuffer->cbegin(), GroupEndStitch + 1U);
  while (sourceIt < endPoint) {
	StitchBuffer->push_back(
	    fPOINTATTR {(*sourceIt).x, (*sourceIt).y, (*sourceIt).attribute & (~(FRMSK | TYPMSK))});
	++sourceIt;
  }
  GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
  thred::rngadj();
  thred::rotfn(rotationAngle, rotationCenter);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::internal::cplayfn(uint32_t iForm, uint32_t layer) {
  FormList->push_back(FormList->operator[](iForm));
  // the push_back may invalidate the reference so do it afterwards
  // clang-format off
  auto& srcForm           = FormList->operator[](iForm);
  auto& currentForm       = FormList->back();
  currentForm.vertexIndex = thred::adflt(srcForm.vertexCount);
  auto const vertexIt           = wrap::next(FormVertices->cbegin(), srcForm.vertexIndex);
  // clang-format on
  std::copy(vertexIt,
            wrap::next(vertexIt, srcForm.vertexCount),
            wrap::next(FormVertices->begin(), currentForm.vertexIndex));
  if (currentForm.type == SAT && (currentForm.satinGuideCount != 0U)) {
	currentForm.satinOrAngle.guide = wrap::toUnsigned(SatinGuides->size());

	auto const sourceStart = wrap::next(SatinGuides->cbegin(), srcForm.satinOrAngle.guide);
	auto const sourceEnd   = wrap::next(sourceStart, srcForm.satinGuideCount);
	auto const destination = SatinGuides->end();
	SatinGuides->insert(destination, sourceStart, sourceEnd);
  }
  currentForm.clipEntries             = 0;
  currentForm.fillType                = 0;
  currentForm.lengthOrCount.clipCount = 0;
  currentForm.edgeType                = 0;
  currentForm.fillInfo.texture.index  = 0;
  currentForm.attribute               = srcForm.attribute & NFRMLMSK;
  currentForm.attribute |= layer << FLAYSHFT;
  currentForm.extendedAttribute = 0;
  form::dusqr(currentForm);
}

void form::cpylayr(uint32_t layer) {
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  fi::cplayfn(selectedForm, layer);
	}
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::cplayfn(ClosestFormToCursor, layer);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto const codedStitchLayer = layer << LAYSHFT;
		auto const endStitch        = wrap::next(StitchBuffer->cbegin(), GroupEndStitch + 1U);
		for (auto currentStitch = wrap::next(StitchBuffer->cbegin(), GroupStartStitch); currentStitch < endStitch;
		     ++currentStitch) {
		  StitchBuffer->push_back(fPOINTATTR {
		      (*currentStitch).x, (*currentStitch).y, ((*currentStitch).attribute & NLAYMSK) | codedStitchLayer});
		}
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::movlayr(uint32_t layer) {
  auto const codedStitchLayer = layer << LAYSHFT;
  if (!SelectedFormList->empty()) {
	thred::savdo();
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto selectedForm : (*SelectedFormList)) {
	  auto& formAttr = FormList->operator[](selectedForm).attribute;
	  formAttr = gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formAttr & NFRMLMSK) | (layer << FLAYSHFT));
	  formMap.set(selectedForm);
	}
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) != 0U) {
		auto const iCurrentForm = (stitch.attribute & FRMSK) >> FRMSHFT;
		if (formMap.test(iCurrentForm)) {
		  stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
		}
	  }
	}
	SelectedFormList->clear();
	StateMap->reset(StateFlag::FORMSEL);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto& formAttr = FormList->operator[](ClosestFormToCursor).attribute;
	  wrap::narrow(formAttr, (layer << FLAYSHFT | gsl::narrow_cast<decltype(layer)>(formAttr & NFRMLMSK)));
	  StateMap->reset(StateFlag::FORMSEL);
	  for (auto& stitch : *StitchBuffer) {
		if (((stitch.attribute & ALTYPMSK) != 0U) && ((stitch.attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
		  stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
		}
	  }
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).attribute =
		      (StitchBuffer->operator[](iStitch).attribute & NLAYMSK) | codedStitchLayer;
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::join() {
  auto const savedFormIndex = ClosestFormToCursor;

  auto const& savedform = FormList->operator[](ClosestFormToCursor);
  auto const                        lastVertex =
      wrap::next(FormVertices->cbegin(), savedform.vertexIndex + savedform.vertexCount - 1U);
  StateMap->set(StateFlag::FRMSAM);
  if (FormList->size() > 1 && StateMap->test(StateFlag::FORMSEL) && form::closfrm()) {
	// clang-format off
	auto& form       = FormList->operator[](ClosestFormToCursor);
	auto  vertexList = std::vector<fPOINT> {};
	// clang-format on
	vertexList.reserve(form.vertexCount);
	auto const vertexIt = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	if ((abs(lastVertex->x - vertexIt->x) > TNYFLOAT) || (abs(lastVertex->y - vertexIt->y) > TNYFLOAT)) {
	  auto const vert = wrap::next(vertexIt, ClosestVertexToCursor);
	  vertexList.push_back(*vert);
	}
	ClosestVertexToCursor = form::nxt(form, ClosestVertexToCursor);
	for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	  auto vert = wrap::next(vertexIt, ClosestVertexToCursor);
	  vertexList.push_back(*vert);
	  ClosestVertexToCursor = form::nxt(form, ClosestVertexToCursor);
	}
	StateMap->set(StateFlag::DELTO);
	thred::frmdel();
	if (savedFormIndex > ClosestFormToCursor) {
	  ClosestFormToCursor = savedFormIndex - 1U;
	}
	else {
	  ClosestFormToCursor = savedFormIndex;
	}
	auto& toForm = FormList->operator[](ClosestFormToCursor);

	auto const insertionPoint = toForm.vertexIndex + toForm.vertexCount;
	form::fltspac(toForm.vertexCount, wrap::toUnsigned(vertexList.size()));
	auto const dest = wrap::next(FormVertices->begin(), insertionPoint);
	std::copy(vertexList.cbegin(), vertexList.cend(), dest);
	toForm.vertexCount += wrap::toUnsigned(vertexList.size());
	form::frmout(ClosestFormToCursor);
	form::refil();
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  StateMap->reset(StateFlag::FRMSAM);
}

void form::refilal() {
  auto const savedFormIndex = ClosestFormToCursor;
  thred::savdo();
  for (ClosestFormToCursor = 0; ClosestFormToCursor < wrap::toUnsigned(FormList->size()); ++ClosestFormToCursor) {
	form::refilfn();
  }
  ClosestFormToCursor = savedFormIndex;
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::nufsel() {
  if (!FormList->empty()) {
	StateMap->reset(StateFlag::FORMSEL);
	if (StateMap->testAndReset(StateFlag::WASEL)) {
	  SelectedFormList->push_back(PreviousFormIndex);
	}
	if (std::none_of(SelectedFormList->begin(), SelectedFormList->end(), [](uint32_t i) {
	      return i == ClosestFormToCursor;
	    })) {
	  SelectedFormList->push_back(ClosestFormToCursor);
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::frmadj(uint32_t formIndex) {
  // clang-format off
  auto const& form     = FormList->operator[](formIndex);
  auto        vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
  // clang-format on
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	vertexIt->x += FormMoveDelta.x;
	vertexIt->y -= FormMoveDelta.y;
	++vertexIt;
  }
  form::frmout(formIndex);
}

void form::frmsadj() {
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
  }
  for (auto& stitch : *StitchBuffer) {
	if (((stitch.attribute & ALTYPMSK) != 0U) && formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
	  stitch.x += FormMoveDelta.x;
	  stitch.y -= FormMoveDelta.y;
	}
  }
}

void form::internal::frmpnts(uint32_t type) {
  if (!StitchBuffer->empty()) {
	auto       iStitch = 0U;
	auto const trg     = ((ClosestFormToCursor << 4U) | type);
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (ALTYPMSK | FRMSK)) == trg) {
		break;
	  }
	  ++iStitch;
	}
	ClosestPointIndex = iStitch;
	auto bFlag        = false;
	for (auto it = wrap::next(StitchBuffer->begin(), ClosestPointIndex); it != StitchBuffer->end(); ++it) {
	  if (((*it).attribute & (ALTYPMSK | FRMSK)) != trg) {
		GroupStitchIndex = wrap::distance<uint32_t>(StitchBuffer->begin(), it) - 1U;
		bFlag            = true;
		break;
	  }
	}
	if (!bFlag) {
	  GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
	}
  }
  else {
	ClosestPointIndex = 0;
	GroupStitchIndex  = 0;
  }
}

void form::selfil(uint32_t type) {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	if (type == FRMFIL && (currentForm.fillType == 0U)) {
	  displayText::tabmsg(IDS_FSELF);
	  return;
	}
	if (type == FRMBFIL && (currentForm.edgeType == 0U)) {
	  displayText::tabmsg(IDS_FSELB);
	  return;
	}
	if (type == FRMAPFIL && (currentForm.edgeType & NEGUND) != EDGEAPPL) {
	  displayText::tabmsg(IDS_FSELA);
	  return;
	}
	fi::frmpnts(type);
	StateMap->set(StateFlag::GRPSEL);
	StateMap->reset(StateFlag::FORMSEL);
	thred::rngadj();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_SEL1FRM);
  }
}

auto form::notfstch(uint32_t attribute) noexcept -> bool {
  if ((attribute & NOTFRM) != 0U) {
	return true;
  }
  return ((attribute & FRMSK) >> FRMSHFT) != ClosestFormToCursor;
}

void form::selalfil() {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto const savedIndex = ClosestPointIndex;
	ClosestPointIndex     = 0U;
	while (ClosestPointIndex < StitchBuffer->size() &&
	       form::notfstch(StitchBuffer->operator[](ClosestPointIndex).attribute)) {
	  ++ClosestPointIndex;
	}
	if (ClosestPointIndex != StitchBuffer->size()) {
	  if (ClosestPointIndex != 0U) {
		--ClosestPointIndex;
	  }
	  GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
	  while (GroupStitchIndex > ClosestPointIndex &&
	         form::notfstch(StitchBuffer->operator[](GroupStitchIndex).attribute)) {
		--GroupStitchIndex;
	  }
	  StateMap->set(StateFlag::GRPSEL);
	  StateMap->reset(StateFlag::FORMSEL);
	  thred::rngadj();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  ClosestPointIndex = savedIndex;
	  displayText::tabmsg(IDS_FSELM);
	}
  }
  else {
	displayText::tabmsg(IDS_SEL1FRM);
  }
}

auto form::frmrng(uint32_t iForm, RANGE& range) -> bool {
  auto retval = false;
  if (!StitchBuffer->empty()) {
	// clang-format off
	range.start          = 0;
	auto&      form      = FormList->operator[](iForm);
	auto const saveClose = ClosestFormToCursor;
	ClosestFormToCursor  = iForm;
	// clang-format on
	if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	  while (range.start < StitchBuffer->size() && notfstch(StitchBuffer->operator[](range.start).attribute)) {
		++(range.start);
	  }
	  range.finish = wrap::toUnsigned(StitchBuffer->size() - 1U);
	  while (range.finish > range.start && notfstch(StitchBuffer->operator[](range.finish).attribute)) {
		--(range.finish);
	  }
	  retval = range.finish > range.start;
	}
	else {
	  range.finish = wrap::toUnsigned(StitchBuffer->size());
	}
	ClosestFormToCursor = saveClose;
  }
  return retval;
}

void form::internal::bholbrd(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.borderSize = BorderWidth;
  form::bsizpar(form);
  form.edgeType    = EDGEBHOL;
  form.edgeSpacing = LineSpacing;
  wrap::narrow(form.borderColor, ActiveColor);
  form::savblen(ButtonholeCornerLength);
  form::refilfn();
}

void form::bhol() {
  auto const savedSpacing = LineSpacing;
  if (displayText::filmsgs(FML_BHOL)) {
	return;
  }
  LineSpacing = savedSpacing;
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  fi::bholbrd(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  fi::bholbrd(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
  LineSpacing = savedSpacing;
}

void form::fcntr() {
  if (!SelectedFormList->empty()) {
	thred::savdo();
	// clang-format off
	auto const  firstForm     = SelectedFormList->front();
	auto const& firstRect     = FormList->operator[](firstForm).rectangle;
	auto const  initialCenter = fPOINT {wrap::midl(firstRect.right, firstRect.left), 
										wrap::midl(firstRect.top, firstRect.bottom)};
	// clang-format on
	for (auto selectedForm : (*SelectedFormList)) {
	  // clang-format off
	  auto const& formRect      = FormList->operator[](selectedForm).rectangle;
	  auto const  currentCenter = fPOINT {wrap::midl(formRect.right, formRect.left), 
										  wrap::midl(formRect.top, formRect.bottom)};
	  auto const  delta         = fPOINT {initialCenter.x - currentCenter.x, initialCenter.y - currentCenter.y};
	  auto&       currentForm   = FormList->operator[](selectedForm);
	  auto        vertexIt      = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; ++iVertex) {
		vertexIt->x += delta.x;
		vertexIt->y += delta.y;
		++vertexIt;
	  }
	  form::frmout(selectedForm);
	  auto const codedForm = (selectedForm << FRMSHFT);
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) == codedForm && ((stitch.attribute & NOTFRM) == 0U)) {
		  stitch.x += delta.x;
		  stitch.y += delta.y;
		}
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_SELC);
  }
}

void form::boxsel() {
  StateMap->reset(StateFlag::INSRT);
  if (!StateMap->test(StateFlag::THUMSHO)) {
	StateMap->reset(StateFlag::BZUM);
	StateMap->reset(StateFlag::BZUMIN);
	StateMap->set(StateFlag::VCAPT);
	SetCapture(ThrEdWindow);
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::internal::fspic(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  form.edgeType       = EDGEPICOT;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = PicotSpacing;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  form::savplen(ButtonholeCornerLength);
  auto offsetStart = wrap::next(ClipPoints->begin(), form.borderClipData);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  form::refilfn();
}

void form::picot() {
  if (displayText::filmsgs(FML_PIC)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	thred::savdo();
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fi::fspic(selectedForm);
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::fspic(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

auto form::internal::contsf(uint32_t formIndex) -> bool {
  ClosestFormToCursor = formIndex;
  auto& form          = FormList->operator[](ClosestFormToCursor);
  if (form.vertexCount > 4) {
	clip::delclps(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	form::chkcont();
	form.attribute |= gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
	wrap::narrow(form.fillColor, ActiveColor);
	form.fillSpacing = LineSpacing;
	form::fsizpar(form);
	form::refilfn();
	return true;
  }
  return false;
}

void form::contfil() {
  if (displayText::filmsgs(FML_CONT)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto selectedForm : (*SelectedFormList)) {
	  fi::contsf(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	thred::savdo();
	if (fi::contsf(ClosestFormToCursor)) {
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  displayText::tabmsg(IDS_CONT);
	}
  }
}

void form::internal::dupfn(float rotationAngle) {
  thred::savdo();
  auto const rotationCenter = form::rotpar();
  if (IniFile.rotationAngle != 0.0F) {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form::duprot(rotationAngle);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		duprots(rotationAngle, rotationCenter);
	  }
	  else {
		if (!SelectedFormList->empty()) {
		  duprotfs(rotationAngle);
		}
		else {
		  displayText::shord();
		}
	  }
	}
  }
  else {
	rotentr(rotationAngle);
	StateMap->set(StateFlag::ENTRDUP);
  }
}

void form::redup() {
  fi::dupfn(IniFile.rotationAngle);
}

void form::bakdup() {
  fi::dupfn(PI_F2 - IniFile.rotationAngle);
}

void form::internal::shrnks() {
  auto  clipRect    = fRECTANGLE {};
  auto  lengths     = std::vector<uint32_t> {};
  auto  deltas      = std::vector<fPOINT> {};
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  lengths.reserve(currentForm.vertexCount);
  deltas.reserve(currentForm.vertexCount - 1U);
  clip::oclp(clipRect, currentForm.borderClipData, currentForm.clipEntries);
  auto const vertexIt = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  auto const vMax     = gsl::narrow<ptrdiff_t>(currentForm.vertexCount - 1U);
  for (auto iVertex = ptrdiff_t {0}; iVertex < vMax; ++iVertex) {
	auto& thisVertex = vertexIt[iVertex];
	auto& nextVertex = vertexIt[iVertex + 1];
	deltas.emplace_back(fPOINT {nextVertex.x - thisVertex.x, nextVertex.y - thisVertex.y});
	lengths.emplace_back(hypot(deltas.back().x, deltas.back().y));
  }
  auto length = lengths.begin();
  auto delta  = deltas.begin();
  for (auto iVertex = ptrdiff_t {0}; iVertex < vMax; ++iVertex) {
	auto const fLen       = wrap::toFloat(*length);
	auto const count      = std::floor(fLen / ClipRectSize.cx);
	auto const ratio      = (ClipRectSize.cx * count + 0.004F) / fLen;
	auto&      thisVertex = vertexIt[iVertex];
	auto&      nextVertex = vertexIt[iVertex + 1];
	nextVertex.x          = thisVertex.x + (*delta).x * ratio;
	nextVertex.y          = thisVertex.y + (*delta).y * ratio;
	++length;
	++delta;
  }
  form::frmout(ClosestFormToCursor);
  form::refil();
}

void form::shrnk() {
  if (StateMap->test(StateFlag::FORMSEL) && FormList->operator[](ClosestFormToCursor).edgeType == EDGECLIP) {
	thred::savdo();
	fi::shrnks();
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_FRMCLP, IDS_SHRNK);
  }
}

void form::internal::dufdat(std::vector<fPOINT>& tempClipPoints,
                            std::vector<SATCON>& tempGuides,
                            std::vector<fPOINT>& destFormVertices,
                            std::vector<FRMHED>& destFormList,
                            uint32_t             formIndex,
                            uint32_t&            formRelocationIndex,
                            uint32_t&            formSourceIndex) {
  auto& dest = destFormList[formRelocationIndex];

  destFormList[formRelocationIndex++] = FormList->operator[](formIndex);

  auto const vertexIt = wrap::next(FormVertices->cbegin(), dest.vertexIndex);
  std::copy(vertexIt, wrap::next(vertexIt, dest.vertexCount), wrap::next(destFormVertices.begin(), formSourceIndex));
  dest.vertexIndex = formSourceIndex;
  formSourceIndex += dest.vertexCount;
  if (dest.satinGuideCount != 0U) {
	auto const guideStart = wrap::next(SatinGuides->cbegin(), dest.satinOrAngle.guide);
	auto const guideEnd   = wrap::next(guideStart, dest.satinGuideCount);
	tempGuides.insert(tempGuides.end(), guideStart, guideEnd);
	dest.satinOrAngle.guide = wrap::toUnsigned(tempGuides.size() - dest.satinGuideCount);
  }
  if (clip::iseclpx(formIndex)) {
	auto const sourceStart = wrap::next(ClipPoints->cbegin(), dest.borderClipData);
	auto const sourceEnd   = wrap::next(sourceStart, dest.clipEntries);
	tempClipPoints.insert(tempClipPoints.end(), sourceStart, sourceEnd);
	dest.borderClipData = wrap::toUnsigned(tempClipPoints.size() - dest.clipEntries);
  }
  if (clip::isclpx(formIndex)) {
	auto const sourceStart = wrap::next(ClipPoints->cbegin(), dest.angleOrClipData.clip);
	auto const sourceEnd   = wrap::next(sourceStart, dest.lengthOrCount.clipCount);
	tempClipPoints.insert(tempClipPoints.end(), sourceStart, sourceEnd);
	dest.angleOrClipData.clip = wrap::toUnsigned(tempClipPoints.size() - dest.lengthOrCount.clipCount);
  }
}

void form::internal::stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept {
  attribute &= NFRMSK;
  attribute |= formIndex << FRMSHFT;
}

void form::frmnumfn(uint32_t newFormIndex) {
  thred::savdo();
  if (newFormIndex != ClosestFormToCursor) {
	auto formRelocationIndex = 0U;
	auto sourceForm          = 0U;
	auto tempFormList        = std::vector<FRMHED> {};
	tempFormList.resize(FormList->size());
	auto tempFormVertices = std::vector<fPOINT> {};
	tempFormVertices.resize(FormVertices->size());
	auto tempGuides = std::vector<SATCON> {};
	tempGuides.reserve(SatinGuides->size());
	auto tempClipPoints = std::vector<fPOINT> {};
	tempClipPoints.reserve(ClipPoints->size());
	auto formSourceIndex = 0U;
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  if (iForm == newFormIndex) {
		fi::dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, ClosestFormToCursor, formRelocationIndex, formSourceIndex);
	  }
	  else {
		if (sourceForm == ClosestFormToCursor) {
		  ++sourceForm;
		}
		fi::dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, sourceForm++, formRelocationIndex, formSourceIndex);
	  }
	}
	auto& formList = *FormList;
	std::copy(tempFormList.cbegin(), tempFormList.cend(), formList.begin());
	std::copy(tempFormVertices.cbegin(), tempFormVertices.cend(), FormVertices->begin());
	std::copy(tempGuides.cbegin(), tempGuides.cend(), SatinGuides->begin());
	std::copy(tempClipPoints.cbegin(), tempClipPoints.cend(), ClipPoints->begin());
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & SRTYPMSK) != 0U) {
		auto const decodedFormIndex = (stitch.attribute & FRMSK) >> FRMSHFT;
		if (decodedFormIndex == ClosestFormToCursor) {
		  fi::stchfrm(newFormIndex, stitch.attribute);
		}
		else {
		  auto start  = newFormIndex;
		  auto finish = ClosestFormToCursor;
		  if (ClosestFormToCursor < newFormIndex) {
			std::swap(start, finish);
		  }
		  if (decodedFormIndex >= start && decodedFormIndex <= finish) {
			if (newFormIndex < ClosestFormToCursor) {
			  fi::stchfrm(decodedFormIndex + 1, stitch.attribute);
			}
			else {
			  fi::stchfrm(decodedFormIndex - 1, stitch.attribute);
			}
		  }
		}
	  }
	}
	ClosestFormToCursor = newFormIndex;
	displayText::ritnum(IDS_NUMFRM, ClosestFormToCursor);
  }
}

constexpr auto form::internal::duat(uint32_t attribute) -> uint32_t {
  auto const type = ((attribute >> TYPSHFT) + 1U) & 3U;
  auto const frm  = (attribute & FRMSK) >> 2U;
  return type | frm;
}

void form::internal::srtf(std::vector<fPOINTATTR> const& tempStitchBuffer, uint32_t start, uint32_t finish) {
  if (start != finish) {
	auto stitchHistogram = std::vector<uint32_t> {};
	stitchHistogram.resize(FormList->size() << 2U);
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  ++(stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]);
	}
	auto stitchAccumulator = start;
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size() << 2U); ++iForm) {
	  auto const value       = stitchHistogram[iForm];
	  stitchHistogram[iForm] = stitchAccumulator;
	  stitchAccumulator += value;
	}
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  StitchBuffer->operator[](stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++) =
	      tempStitchBuffer[iStitch];
	}
  }
}

void form::srtbyfrm() {
  auto colorHistogram = std::vector<uint32_t> {};
  colorHistogram.resize(COLORCNT);
  auto color = std::vector<uint32_t> {};
  color.resize(COLORCNT);
  if (!FormList->empty()) {
	thred::savdo();
	std::iota(color.begin(), color.end(), 1);
	color[AppliqueColor]  = 0U;
	auto tempStitchBuffer = std::vector<fPOINTATTR> {};
	tempStitchBuffer.resize(StitchBuffer->size());
	for (auto& stitch : *StitchBuffer) {
	  ++(colorHistogram[color[stitch.attribute & COLORBTS]]);
	}
	auto colorAccumulator = 0U;
	for (auto& iColor : colorHistogram) {
	  auto const value = iColor;
	  iColor           = colorAccumulator;
	  colorAccumulator += value;
	}
	for (auto& stitch : *StitchBuffer) {
	  tempStitchBuffer[colorHistogram[color[stitch.attribute & COLORBTS]]++] = stitch;
	}
	fi::srtf(tempStitchBuffer, 0, colorHistogram[0]);
	for (auto iColor = 0U; iColor < (colorHistogram.size() - 1U); ++iColor) {
	  fi::srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[wrap::toSize(iColor) + 1U]);
	}
  }
  else {
	xt::srtcol();
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::cntrx() {
  auto const markCenter =
      StateMap->test(StateFlag::GMRK)
          ? ZoomMarkPoint
          : fPOINT {wrap::toFloat(UnzoomedRect.x) / 2.0F, wrap::toFloat(UnzoomedRect.y) / 2.0F};
  bool flag = false;
  if (!SelectedFormList->empty()) {
	flag = true;
	thred::savdo();
	auto selectedCenter = fPOINT {};
	fi::dufcntr(selectedCenter);
	FormMoveDelta = fPOINT {markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y};
	if (StateMap->test(StateFlag::CNTRV)) {
	  FormMoveDelta.y = 0.0F;
	}
	if (StateMap->test(StateFlag::CNTRH)) {
	  FormMoveDelta.x = 0.0F;
	}
	for (auto selectedForm : (*SelectedFormList)) {
	  frmadj(selectedForm);
	}
	frmsadj();
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  flag = true;
	  thred::savdo();
	  // clang-format off
	  auto const& formRect       = FormList->operator[](ClosestFormToCursor).rectangle;
	  auto const  selectedCenter = fPOINT {wrap::midl(formRect.right, formRect.left), 
										   wrap::midl(formRect.top, formRect.bottom)};
	  // clang-format on
	  FormMoveDelta = fPOINT {markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y};
	  if (StateMap->test(StateFlag::CNTRV)) {
		FormMoveDelta.y = 0.0F;
	  }
	  if (StateMap->test(StateFlag::CNTRH)) {
		FormMoveDelta.x = 0.0F;
	  }
	  frmadj(ClosestFormToCursor);
	  for (auto& stitch : *StitchBuffer) {
		if (((stitch.attribute & ALTYPMSK) != 0U) && (stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		  stitch.x += FormMoveDelta.x;
		  stitch.y -= FormMoveDelta.y;
		}
	  }
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		flag = true;
		thred::savdo();
		thred::rngadj();
		auto const& startStitch = StitchBuffer->operator[](GroupStartStitch);
		auto groupRect = fRECTANGLE {startStitch.x, startStitch.y, startStitch.x, startStitch.y};
		for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
		  auto const& stitch = StitchBuffer->operator[](iStitch);
		  if (stitch.x < groupRect.left) {
			groupRect.left = stitch.x;
		  }
		  if (stitch.y > groupRect.top) {
			groupRect.top = stitch.y;
		  }
		  if (stitch.x > groupRect.right) {
			groupRect.right = stitch.x;
		  }
		  if (stitch.y < groupRect.bottom) {
			groupRect.bottom = stitch.y;
		  }
		}
		auto const selectedCenter = fPOINT {wrap::midl(groupRect.right, groupRect.left),
		                                    wrap::midl(groupRect.top, groupRect.bottom)};
		FormMoveDelta = fPOINT {markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y};
		if (StateMap->test(StateFlag::CNTRV)) {
		  FormMoveDelta.y = 0.0F;
		}
		if (StateMap->test(StateFlag::CNTRH)) {
		  FormMoveDelta.x = 0.0F;
		}
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).x += FormMoveDelta.x;
		  StitchBuffer->operator[](iStitch).y -= FormMoveDelta.y;
		}
	  }
	  else {
		displayText::shoseln(IDS_FGRPF, IDS_CENT);
	  }
	}
  }
  if (flag) {
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::centir() {
  StateMap->reset(StateFlag::BIGBOX);
  fi::getbig(AllItemsRect);
  auto const itemCenter = fPOINT {wrap::midl(AllItemsRect->right, AllItemsRect->left),
                                  wrap::midl(AllItemsRect->top, AllItemsRect->bottom)};
  auto const hoopCenter = fPOINT {wrap::toFloat(UnzoomedRect.x) / 2.0F, wrap::toFloat(UnzoomedRect.y) / 2.0F};
  auto const delta = fPOINT {hoopCenter.x - itemCenter.x, hoopCenter.y - itemCenter.y};
  for (auto& stitch : *StitchBuffer) {
	stitch.x += delta.x;
	stitch.y += delta.y;
  }
  for (auto& FormVertice : *FormVertices) {
	FormVertice.x += delta.x;
	FormVertice.y += delta.y;
  }
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	form::frmout(iForm);
  }
  xt::setfchk();
  StateMap->set(StateFlag::RESTCH);
}

auto form::internal::bean(uint32_t start, uint32_t finish) -> uint32_t {
  auto const stitchRange      = (finish - start) * 3U; // each bean is 3 stitches
  auto       highStitchBuffer = std::vector<fPOINTATTR> {};
  highStitchBuffer.reserve(stitchRange);
  auto iSourceStitch = start;
  auto beanCount     = 0U;
  for (auto loop = 0; loop < 2; ++loop) {
	auto const& stitch     = StitchBuffer->operator[](iSourceStitch);
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 1U);
	auto const& stitchFwd2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 2U);
	highStitchBuffer.push_back(stitch);
	if (stitchFwd2.x != stitch.x || stitchFwd2.y != stitch.y) {
	  highStitchBuffer.push_back(stitchFwd1);
	  highStitchBuffer.push_back(stitch);
	  beanCount += 2U;
	}
	++iSourceStitch;
  }
  while (iSourceStitch < finish - 1) {
	auto const& stitch      = StitchBuffer->operator[](iSourceStitch);
	auto const& stitchFwd1  = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 1U);
	auto const& stitchFwd2  = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 2U);
	auto const& stitchBack2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) - 2U);
	highStitchBuffer.push_back(stitch);
	if ((stitchFwd2.x != stitch.x || stitchFwd2.y != stitch.y) &&
	    (stitchBack2.x != stitch.x || stitchBack2.y != stitch.y)) {
	  highStitchBuffer.push_back(stitchFwd1);
	  highStitchBuffer.push_back(stitch);
	  beanCount += 2U;
	}
	++iSourceStitch;
  }
  auto const& stitch      = StitchBuffer->operator[](iSourceStitch);
  auto const& stitchFwd1  = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 1U);
  auto const& stitchBack2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) - 2U);
  highStitchBuffer.push_back(stitch);
  if (stitchBack2.x != stitch.x || stitchBack2.y != stitch.y) {
	highStitchBuffer.push_back(stitchFwd1);
	highStitchBuffer.push_back(stitch);
	beanCount += 2U;
  }
  // now copy stitches back up to the end of the original group
  std::copy(highStitchBuffer.cbegin(),
            wrap::next(highStitchBuffer.cbegin(), finish - start),
            wrap::next(StitchBuffer->begin(), start));
  // and then insert the remainder of the new stitches
  StitchBuffer->insert(wrap::next(StitchBuffer->cbegin(), finish),
                       wrap::next(highStitchBuffer.begin(), finish - start),
                       highStitchBuffer.end());
  return beanCount;
}

void form::dubean() {
  if (!StitchBuffer->empty()) {
	thred::savdo();
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  auto const beanCount = fi::bean(GroupStartStitch, GroupEndStitch);
	  if (ClosestPointIndex > GroupStitchIndex) {
		ClosestPointIndex += beanCount;
	  }
	  else {
		GroupStitchIndex += beanCount;
	  }
	  thred::grpAdj();
	}
	else {
	  fi::bean(0, wrap::toUnsigned(StitchBuffer->size() - 1U));
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::internal::unbean(uint32_t start, uint32_t& finish) {
  auto const stitchRange      = ((finish - start) / 3U) + 1U; // each bean is 3 stitches
  auto       highStitchBuffer = std::vector<fPOINTATTR> {};
  highStitchBuffer.reserve(stitchRange);
  auto lastStitch = finish;
  if (lastStitch > wrap::toUnsigned(StitchBuffer->size()) - 3U) {
	lastStitch = wrap::toUnsigned(StitchBuffer->size()) - 3U;
  }
  auto iSourceStitch = start;
  for (; iSourceStitch <= lastStitch; ++iSourceStitch) {
	auto const& stitch     = StitchBuffer->operator[](iSourceStitch);
	auto const& stitchFwd2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 2U);
	highStitchBuffer.push_back(stitch);
	if (stitch.x == stitchFwd2.x && stitch.y == stitchFwd2.y) {
	  iSourceStitch += 2;
	}
  }
  if (finish != lastStitch) {
	while (iSourceStitch != finish + 1U) {
	  highStitchBuffer.push_back(StitchBuffer->operator[](iSourceStitch++));
	}
  }
  if ((finish - start) > highStitchBuffer.size()) {
	auto const stitchStart = wrap::next(StitchBuffer->begin(), start);
	std::copy(highStitchBuffer.begin(), highStitchBuffer.end(), stitchStart);
	StitchBuffer->erase(wrap::next(StitchBuffer->cbegin(), start + highStitchBuffer.size()),
	                    wrap::next(StitchBuffer->cbegin(), finish + 1U));
  }
  finish = start + wrap::toUnsigned(highStitchBuffer.size() - 1U);
}

void form::debean() {
  thred::savdo();
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	fi::unbean(GroupStartStitch, GroupEndStitch);
	if (ClosestPointIndex > GroupStitchIndex) {
	  ClosestPointIndex = GroupEndStitch;
	}
	else {
	  GroupStitchIndex = GroupEndStitch;
	}
	auto const iEndStitch = wrap::toUnsigned(StitchBuffer->size()) - 1U;
	if (ClosestPointIndex > iEndStitch - 1U) {
	  ClosestPointIndex = iEndStitch;
	}
	if (GroupStitchIndex > iEndStitch - 1U) {
	  GroupStitchIndex = iEndStitch;
	}
	thred::grpAdj();
  }
  else {
	if (StitchBuffer->size() > 2U) {
	  auto iEndStitch = wrap::toUnsigned(StitchBuffer->size()) - 1U;
	  fi::unbean(0, iEndStitch);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::clpspac(uint32_t const insertPoint, uint32_t count) {
  auto const sourceStart = wrap::next(ClipPoints->cbegin(), insertPoint);
  auto const sourceEnd   = wrap::next(sourceStart, count);
  ClipPoints->insert(sourceStart, sourceStart, sourceEnd);
}

void form::stchadj() {
  auto const     codedClosest = ClosestFormToCursor << FRMSHFT;
  constexpr auto OFFSET       = 1U << FRMSHFT; // coded next form offset
  for (auto& stitch : *StitchBuffer) {
	auto codedForm = stitch.attribute & FRMSK;
	if (codedForm > codedClosest) {
	  stitch.attribute &= NFRMSK;
	  codedForm += OFFSET;
	  stitch.attribute |= codedForm;
	}
  }
  form::refilfn();
  ++ClosestFormToCursor;
  form::refilfn();
  StateMap->reset(StateFlag::FRMPSEL);
}

auto form::internal::spltlin() -> bool {
  auto& srcForm = FormList->operator[](ClosestFormToCursor);
  if (ClosestVertexToCursor < 1 || srcForm.vertexCount - ClosestVertexToCursor < 2) {
	return false;
  }
  auto const vertexIt = wrap::next(FormVertices->cbegin(), srcForm.vertexIndex + ClosestVertexToCursor);
  auto const stitchPoint = *vertexIt;
  fi::nufpnt(ClosestVertexToCursor, srcForm, stitchPoint);
  form::frmlin(srcForm);
  FormList->insert(wrap::next(FormList->cbegin(), ClosestFormToCursor), 1, srcForm);
  // insert may invalidate the srcForm reference, so build a new one
  auto& form       = FormList->operator[](ClosestFormToCursor);
  auto& dstForm    = FormList->operator[](wrap::toSize(ClosestFormToCursor) + 1U);
  form.vertexCount = ClosestVertexToCursor + 1U;
  dstForm.vertexCount -= form.vertexCount;
  dstForm.vertexIndex = form.vertexIndex + form.vertexCount;
  form::frmout(ClosestFormToCursor);
  form::frmout(ClosestFormToCursor + 1U);
  if (clip::iseclp(form)) {
	form::clpspac(form.borderClipData, form.clipEntries);
	auto const maxForm = FormList->size();
	for (auto iForm = ClosestFormToCursor + 1U; iForm < maxForm; ++iForm) {
	  FormList->operator[](iForm).borderClipData += form.clipEntries;
	}
  }
  form::stchadj();
  return true;
}

void form::spltfrm() {
  if (StateMap->test(StateFlag::FRMPSEL)) {
	thred::savdo();
	auto const& currentForm = FormList->operator[](ClosestFormToCursor);
	if (currentForm.type == SAT) {
	  if (currentForm.satinGuideCount != 0U) {
		auto guideIt = wrap::next(SatinGuides->cbegin(), currentForm.satinOrAngle.guide);
		for (auto guideIndex = 0U; guideIndex < currentForm.satinGuideCount; ++guideIndex) {
		  if (guideIt->start == ClosestVertexToCursor || guideIt->finish == ClosestVertexToCursor) {
			satin::spltsat(guideIndex);
			return;
		  }
		  ++guideIt;
		}
	  }
	}
	else {
	  if (currentForm.type == FRMLINE) {
		if (fi::spltlin()) {
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  displayText::tabmsg(IDS_FRM3);
		}
		return;
	  }
	}
  }
  displayText::spltmsg();
}

void form::stchs2frm() {
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	constexpr auto STITCHMX = 12000U; // maximum number of stitches in a group
	if ((GroupEndStitch - GroupStartStitch) > STITCHMX) {
	  displayText::tabmsg(IDS_STMAX);
	  return;
	}
	auto const vertexCount = GroupEndStitch - GroupStartStitch + 1U;
	FormList->push_back(FRMHED {});
	auto& currentForm       = FormList->back();
	currentForm.type        = FRMLINE;
	currentForm.vertexCount = vertexCount;
	currentForm.vertexIndex = thred::adflt(vertexCount);
	auto vertexIt           = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  *vertexIt = StitchBuffer->operator[](iStitch);
	  ++vertexIt;
	}
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	thred::delstchm();
	StateMap->reset(StateFlag::GRPSEL);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_GRPMSG, IDS_STCH2FRM);
  }
}

void form::vrtsclp() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
  form.angleOrClipData.clip    = clip::numclp();
  form.wordParam               = IniFile.fillPhase;
  fi::makpoli();
  form.fillSpacing = IniFile.clipOffset;
  auto offsetStart = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  form.fillType = VCLPF;
  wrap::narrow(form.fillColor, ActiveColor);
  form.type = FRMFPOLY;
  refilfn();
}

void form::vrtclp() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  StateMap->set(StateFlag::NOCLP);
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
			  vrtsclp();
			}
		  }
		  StateMap->reset(StateFlag::NOCLP);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			vrtsclp();
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::horsclp() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  auto const clipSize          = wrap::toUnsigned(ClipBuffer->size());
  form.lengthOrCount.clipCount = clipSize;
  form.angleOrClipData.clip    = clip::numclp();
  form.lengthOrCount.clipCount = clipSize;
  form.wordParam               = IniFile.fillPhase;
  fi::makpoli();
  form.fillSpacing = IniFile.clipOffset;
  auto offsetStart = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  form.fillType = HCLPF;
  wrap::narrow(form.fillColor, ActiveColor);
  form.type = FRMFPOLY;
  form::refilfn();
}

void form::horclp() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  StateMap->set(StateFlag::NOCLP);
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
			  horsclp();
			}
		  }
		  StateMap->set(StateFlag::NOCLP);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			horsclp();
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::angsclp(FRMHED& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.angleOrClipData.clip    = clip::numclp();
  form.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
  form.wordParam               = IniFile.fillPhase;
  fi::makpoli();
  form.satinOrAngle.angle = IniFile.fillAngle;
  form.fillSpacing        = IniFile.clipOffset;
  auto offsetStart        = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  form.fillType = ANGCLPF;
  wrap::narrow(form.fillColor, ActiveColor);
  form.type = FRMFPOLY;
  form::refilfn();
}

void form::angclp() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  StateMap->set(StateFlag::NOCLP);
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			auto& form          = FormList->operator[](selectedForm);
			if (form.type != FRMLINE) {
			  angsclp(form);
			}
		  }
		  StateMap->reset(StateFlag::NOCLP);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			auto& form = FormList->operator[](ClosestFormToCursor);
			angsclp(form);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::dubsfil(FRMHED& form) {
  clip::deleclp(ClosestFormToCursor);
  form.edgeType = EDGEDOUBLE;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  fi::dubfn(form);
  form::refilfn();
}

void form::dubfil() {
  if (displayText::filmsgs(FML_LIN)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  dubsfil(form);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  dubsfil(form);
	  thred::coltab();
	  StateMap->set(StateFlag::INIT);
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::col2frm() {
  auto colorChangedCount = 0U;
  if (!FormList->empty()) {
	auto const formColorPermutations = FormList->size() << 4U; // total number of form and color combinations
	auto       fillColorHistogram = std::vector<uint32_t> {};
	fillColorHistogram.resize(formColorPermutations);
	auto borderColorHistogram = std::vector<uint32_t> {};
	borderColorHistogram.resize(formColorPermutations);
	auto featherColorHistogram = std::vector<uint32_t> {};
	featherColorHistogram.resize(formColorPermutations);
	auto underlayColorHistogram = std::vector<uint32_t> {};
	underlayColorHistogram.resize(formColorPermutations);
	for (auto& stitch : *StitchBuffer) {
	  auto const formColorCode = stitch.attribute & (COLMSK | FRMSK);
	  if ((stitch.attribute & (WLKMSK | CWLKMSK | UNDMSK)) != 0U) {
		++(underlayColorHistogram[formColorCode]);
	  }
	  else {
		if ((stitch.attribute & FTHMSK) != 0U) {
		  ++(featherColorHistogram[formColorCode]);
		}
		else {
		  switch (stitch.attribute & TYPMSK) {
			case FRMFIL: {
			  ++(fillColorHistogram[formColorCode]);
			  break;
			}
			case FRMBFIL: {
			  ++(borderColorHistogram[formColorCode]);
			  break;
			}
			default: {
			  outDebugString(L"default hit in col2frm: attribute [{}]\n", stitch.attribute & TYPMSK);
			  break;
			}
		  }
		}
	  }
	}
	auto startColorOffset = 0U;
	auto endColorOffset   = COLORCNT;
	for (auto& formIter : *FormList) {
	  if (formIter.fillType != 0U) {
		auto count         = 0U;
		auto majorityColor = 0U;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (fillColorHistogram[iColor] > count) {
			count         = fillColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLORBTS;
		if (formIter.fillColor != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(formIter.fillColor, majorityColor);
		}
		if (formIter.fillType == FTHF && ((formIter.extendedAttribute & AT_FTHBLND) != 0U)) {
		  count = majorityColor = 0;
		  for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
			if (featherColorHistogram[iColor] > count) {
			  count         = fillColorHistogram[iColor];
			  majorityColor = iColor;
			}
		  }
		  majorityColor &= COLORBTS;
		  if (formIter.fillInfo.feather.color != majorityColor) {
			++colorChangedCount;
			wrap::narrow(formIter.fillInfo.feather.color, majorityColor);
		  }
		}
	  }
	  if (formIter.edgeType != 0U) {
		auto count         = 0U;
		auto majorityColor = 0U;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (borderColorHistogram[iColor] > count) {
			count         = borderColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLORBTS;
		if (formIter.borderColor != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(formIter.borderColor, majorityColor);
		}
	  }
	  if ((formIter.extendedAttribute & (AT_WALK | AT_CWLK | AT_UND)) != 0U) {
		auto count         = 0U;
		auto majorityColor = 0U;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (underlayColorHistogram[iColor] > count) {
			count         = borderColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLORBTS;
		if (formIter.underlayColor != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(formIter.underlayColor, majorityColor);
		}
	  }
	  startColorOffset += COLORCNT;
	  endColorOffset += COLORCNT;
	}
  }
  displayText::showMessage(IDS_NCOLCHG, colorChangedCount);
}

void form::chan() {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  wrap::narrow(currentForm.borderColor, ActiveColor);
  currentForm.edgeSpacing        = IniFile.chainSpace;
  currentForm.borderSize         = BorderWidth;
  currentForm.edgeStitchLen      = IniFile.chainRatio;
  currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
  currentForm.minBorderStitchLen = MinStitchLength;
  if (StateMap->test(StateFlag::LINCHN)) {
	currentForm.edgeType = EDGELCHAIN;
  }
  else {
	currentForm.edgeType = EDGEOCHAIN;
  }
  form::refilfn();
}

void form::chain() {
  if (displayText::filmsgs(FML_CHAIN)) {
	return;
  }
  thred::savdo();
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  chan();
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  chan();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::crop() {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	thred::savdo();
	// clang-format off
	auto const& form         = FormList->operator[](ClosestFormToCursor);
	auto        iDestination = StitchBuffer->begin();
	// clang-format on
	for (auto& stitch : *StitchBuffer) {
	  if (form::cisin(form, stitch.x, stitch.y)) {
		*iDestination++ = stitch;
	  }
	}
	StitchBuffer->resize(wrap::distance<size_t>(StitchBuffer->begin(), iDestination));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_FRM1MSG, IDS_CROP);
  }
}

void form::internal::fsclpx(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  form.edgeType       = EDGECLIPX;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = ClipRectSize.cx;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  auto offsetStart = wrap::next(ClipPoints->begin(), form.borderClipData);
  for (auto& clip : *ClipBuffer) {
	*offsetStart = clip;
	++offsetStart;
  }
  clip::duxclp(form);
  form::refilfn();
}

void form::filclpx() {
  if (displayText::filmsgs(FML_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::savdo();
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fi::fsclpx(selectedForm);
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::fsclpx(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP);
	  }
	}
	else {
	  auto const str = std::wstring(L"no clipboard data");
	  displayText::shoMsg(str);
	  CloseClipboard();
	}
  }
}

void form::srtfrm() {
  auto histogram = std::vector<uint32_t> {};
  histogram.resize(FormList->size());
  if (!StitchBuffer->empty()) {
	thred::savdo();
	for (auto& stitch : *StitchBuffer) {
	  auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	  ++(histogram[iForm]);
	}
	auto totalStitches = 0U;
	for (auto& entry : histogram) {
	  auto const formStitchCount = entry;
	  entry                      = totalStitches;
	  totalStitches += formStitchCount;
	}
	auto highStitchBuffer = std::vector<fPOINTATTR> {};
	highStitchBuffer.resize(StitchBuffer->size());
	for (auto& stitch : *StitchBuffer) {
	  auto const iForm              = (stitch.attribute & FRMSK) >> FRMSHFT;
	  auto const iHighStitch        = histogram[iForm]++;
	  highStitchBuffer[iHighStitch] = stitch;
	}
	std::copy(highStitchBuffer.cbegin(), highStitchBuffer.cend(), StitchBuffer->begin());
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}
