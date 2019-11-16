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
#include <boost/dynamic_bitset.hpp>
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
#include "formForms.h"
#include "hlp.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"
#include "form.h"

namespace fi = form::internal;

FRMHED*  FormForInsert;  // insert form vertex in this form 
uint32_t FormVertexNext; // form vertex storage for form vertex insert
uint32_t FormVertexPrev; // form vertex storage for form vertex insert

std::vector<fPOINT>* WorkingFormVertices; // form points for angle fills

// clang-format off
uint8_t Level00   = 0U;
uint8_t Level01   = 1U;
uint8_t Level02[] = { 0U, 1U };
uint8_t Level03[] = { 1U, 0U, 2U };
uint8_t Level04[] = { 1U, 3U, 0U, 2U };
uint8_t Level05[] = { 2U, 0U, 3U, 1U, 4U };
uint8_t Level06[] = { 3U, 0U, 2U, 4U, 1U, 5U };
uint8_t Level07[] = { 3U, 0U, 4U, 1U, 6U, 2U,  5U };
uint8_t Level08[] = { 4U, 0U, 5U, 1U, 3U, 6U,  2U, 7U };
uint8_t Level09[] = { 4U, 0U, 5U, 1U, 6U, 2U,  7U, 3U,  8U };
uint8_t Level10[] = { 5U, 0U, 6U, 1U, 7U, 2U,  8U, 3U,  9U,  4U };
uint8_t Level11[] = { 5U, 0U, 6U, 1U, 7U, 2U,  8U, 3U,  9U, 10U };
uint8_t Level12[] = { 6U, 0U, 7U, 1U, 8U, 2U,  9U, 3U, 10U,  4U, 11U };
uint8_t Level13[] = { 6U, 0U, 1U, 7U, 2U, 8U,  3U, 9U,  4U, 10U,  5U, 11U,  6U, 12U };
uint8_t Level14[] = { 7U, 0U, 8U, 1U, 9U, 2U, 10U, 3U, 11U,  4U, 12U,  5U, 13U,  6U };
uint8_t Level15[] = { 7U, 0U, 8U, 1U, 9U, 2U, 10U, 3U, 11U,  4U, 12U,  5U, 13U,  6U, 14U, 7U, 15U };
// clang-format on

uint8_t* Levels[] = { &Level00,    &Level01,    &Level02[0], &Level03[0], &Level04[0], &Level05[0], &Level06[0], &Level07[0],
	                  &Level08[0], &Level09[0], &Level10[0], &Level11[0], &Level12[0], &Level13[0], &Level14[0], &Level15[0] };

void form::frmclr(FRMHED& destination) noexcept {
	auto head   = FRMHED {};
	destination = head;
}

auto form::internal::fplComp(const fPOINTLINE& point1, const fPOINTLINE& point2) noexcept -> bool {
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
	if (UserFlagMap.test(UserFlag::SQRFIL)) {
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
	const auto currentVertex = FormList->operator[](ClosestFormToCursor).vertexIndex + vertexOffset;
	auto       sourceStart   = std::next(FormVertices->cbegin(), currentVertex);
	auto       val           = fPOINT {};
	FormVertices->insert(sourceStart, count, val);

	for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		auto& form = FormList->operator[](iForm);
		form.vertexIndex += count;
	}
}

void form::delflt(uint32_t formIndex) {
	auto form = std::next(FormList->begin(), formIndex);
	if (form->vertexCount != 0U) {
		const auto vertexCount = form->vertexCount;
		auto       eraseStart  = std::next(FormVertices->cbegin(), form->vertexIndex);
		auto       eraseEnd    = std::next(eraseStart, form->vertexCount);
		FormVertices->erase(eraseStart, eraseEnd);
		for (form++; form < FormList->end(); form++) {
			form->vertexIndex -= vertexCount;
		}
	}
}

auto form::internal::chk2of() -> bool {
	if (!StateMap.test(StateFlag::SELBOX)) {
		return false;
	}
	if (UserFlagMap.test(UserFlag::FIL2OF)) {
		return false;
	}
	return true;
}

void form::internal::rotbak(float rotationAngle, const fPOINT& rotationCenter) noexcept {
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
	const auto codedForm   = ClosestFormToCursor << FRMSHFT;
	auto       firstStitch = std::find_if(StitchBuffer->begin(), StitchBuffer->end(), [codedForm](const fPOINTATTR& m) -> bool {
        return ((m.attribute & (FRMSK | NOTFRM)) == codedForm) && ((m.attribute & (TYPFRM | FTHMSK)) != 0U);
    });
	if (firstStitch != StitchBuffer->end()) {
		// we found the first stitch, so now delete the stitches in the form 
		StitchBuffer->erase(
		    std::remove_if(firstStitch,
		                   StitchBuffer->end(),
		                   [codedForm](const fPOINTATTR& m) -> bool { return (m.attribute & FRMSK) == codedForm; }),
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
		auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
		if (form.satinGuideCount != 0U) {
			auto shortestGuideIndex = 0U;
			auto minimumLength      = 10000U;
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
				const auto length = guideIt[iGuide].finish - guideIt[iGuide].start;
				if (length < minimumLength) {
					minimumLength      = length;
					shortestGuideIndex = iGuide;
				}
			}
			form.angleOrClipData.guide.start  = guideIt[shortestGuideIndex].start;
			form.angleOrClipData.guide.finish = guideIt[shortestGuideIndex].finish;
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
	const auto attribute = ClosestFormToCursor << 4U;
	auto       iStitch   = 0U;
	for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) == attribute) {
			if ((stitch.attribute & NOTFRM) == 0U) {
				break;
			}
		}
		iStitch++;
	}
	return iStitch;
}

void form::ispcdclp() {
	StateMap.reset(StateFlag::WASPCDCLP);
	if (OpenClipboard(ThrEdWindow) != 0) {
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory != nullptr) {
			StateMap.set(StateFlag::WASPCDCLP);
		}
	}
	CloseClipboard();
}

void form::frmout(uint32_t formIndex) {
	auto& form = FormList->operator[](formIndex);
	if (form.vertexCount != 0U) {
		auto& rectangle    = form.rectangle;
		CurrentVertexIndex = form.vertexIndex;
		auto vertexIt      = std::next(FormVertices->cbegin(), form.vertexIndex);
		rectangle          = fRECTANGLE { vertexIt->x, vertexIt->y, vertexIt->x, vertexIt->y };
		for (auto iVertex = 1U; iVertex < form.vertexCount; iVertex++) {
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
			const auto offset = (MINRCT - (rectangle.top - rectangle.bottom)) / 2;
			rectangle.top += offset;
			rectangle.bottom -= offset;
		}
		if (rectangle.right - rectangle.left < MINRCT) {
			const auto offset = (MINRCT - (rectangle.right - rectangle.left)) / 2;
			rectangle.left -= offset;
			rectangle.right += offset;
		}
	}
}

void form::sfCor2px(const fPOINT& stitchPoint, POINT& screen) {
	screen.x = wrap::round<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x + 0.5F);
	screen.y = wrap::round<int32_t>(StitchWindowClientRect.bottom - (stitchPoint.y - ZoomRect.bottom) * ZoomRatio.y + 0.5F);
}

void form::internal::px2stchf(const POINT& screen, fPOINT& stitchPoint) noexcept {
	const auto factorX = gsl::narrow_cast<float>(screen.x) / gsl::narrow_cast<float>(StitchWindowClientRect.right);
	stitchPoint.x      = factorX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	const auto factorY = (gsl::narrow_cast<float>(StitchWindowClientRect.bottom) - screen.y) / StitchWindowClientRect.bottom;
	stitchPoint.y      = factorY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

void form::frmlin(uint32_t vertices, uint32_t vertexCount) {
	if (VertexCount != 0U) {
		auto& formLines = *FormLines;
		formLines.clear();
		formLines.reserve(vertexCount);
		auto vertexIt = std::next(FormVertices->cbegin(), vertices);
		for (auto iVertex = 0U; iVertex < vertexCount; iVertex++) {
			formLines.push_back(POINT {
			    wrap::round<int32_t>((vertexIt[iVertex].x - ZoomRect.left) * ZoomRatio.x),
			    wrap::round<int32_t>(StitchWindowClientRect.bottom - (vertexIt[iVertex].y - ZoomRect.bottom) * ZoomRatio.y) });
		}
		formLines.push_back(
		    POINT { wrap::round<int32_t>((vertexIt[0].x - ZoomRect.left) * ZoomRatio.x),
		            wrap::round<int32_t>(StitchWindowClientRect.bottom - (vertexIt[0].y - ZoomRect.bottom) * ZoomRatio.y) });
	}
}

void form::frmlin(const std::vector<fPOINT>& vertices) {
	const auto vertexMax = vertices.size();

	if (vertexMax != 0U) {
		auto& formLines = *FormLines;
		formLines.clear();
		formLines.reserve(vertexMax);
		for (auto iVertex = 0U; iVertex < vertexMax; iVertex++) {
			formLines.push_back(POINT {
			    wrap::round<int32_t>((vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x),
			    wrap::round<int32_t>(StitchWindowClientRect.bottom - (vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y) });
		}
		formLines.push_back(
		    POINT { wrap::round<int32_t>((vertices[0].x - ZoomRect.left) * ZoomRatio.x),
		            wrap::round<int32_t>(StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom) * ZoomRatio.y) });
	}
}

void form::dufrm() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	wrap::Polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::unfrm() {
	if (StateMap.testAndReset(StateFlag::SHOFRM)) {
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
	if (StateMap.testAndReset(StateFlag::SHOFRM)) {
		form::mdufrm();
	}
}

void form::internal::rats() {
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = (ZoomRect.right - ZoomRect.left) / StitchWindowClientRect.right;
		VerticalRatio   = (ZoomRect.top - ZoomRect.bottom) / StitchWindowClientRect.bottom;
	}
	else {
		HorizontalRatio = gsl::narrow_cast<float>(UnzoomedRect.x) / StitchWindowClientRect.right;
		VerticalRatio   = gsl::narrow_cast<float>(UnzoomedRect.y) / StitchWindowClientRect.bottom;
	}
}

void form::fvars(uint32_t iForm) noexcept {
	if (!FormList->empty() && iForm < FormList->size()) {
		auto& form             = FormList->operator[](iForm);
		CurrentVertexIndex     = form.vertexIndex;
		VertexCount            = form.vertexCount;
	}
}

void form::setfrm() {
	if (!FormList->empty()) {
		auto point = fPOINT {};

		fi::rats();
		ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
		form::fvars(ClosestFormToCursor);
		auto& form = FormList->back();
		fi::px2stchf(FormLines->front(), point);
		auto       vertexIt     = std::next(FormVertices->begin(), form.vertexIndex);
		const auto delta        = fPOINT { point.x - vertexIt->x, point.y - vertexIt->y };
		auto& rectangle = form.rectangle;
		rectangle = fRECTANGLE { 1e30F, 0.0F, 0.0F, 1e30F };
		for (auto iVertex = 0U; iVertex < NewFormVertexCount - 1U; iVertex++) {
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
		StateMap.reset(StateFlag::FORMIN);
		StateMap.set(StateFlag::INIT);
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::form() {
	displayText::shoMsg((*StringTable)[STR_FMEN]);
	StateMap.set(StateFlag::FORMIN);
	StateMap.reset(StateFlag::INSRT);
	thred::duzrat();
}

void form::internal::frmsqr(uint32_t vertexIndex, uint32_t iVertex) {
	POINT line[4] = {};

	auto vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
	thred::stch2pxr(vertexIt[iVertex]);
	line[1]          = StitchCoordinatesPixels;
	const auto ratio = gsl::narrow_cast<float>(MulDiv(IniFile.formVertexSizePixels, gsl::narrow<int32_t>(*screenDPI), 96))
	                   / StitchWindowClientRect.right;
	auto       length = (ZoomRect.right - ZoomRect.left) * ratio * 2.0F;
	const auto delta  = fPOINT { vertexIt[iVertex - 1U].x - vertexIt[iVertex].x, vertexIt[iVertex - 1U].y - vertexIt[iVertex].y };
	auto       angle  = atan2(delta.y, delta.x);
	auto       offset = fPOINT { length * cos(angle), length * sin(angle) };
	const auto point  = fPOINT { vertexIt[iVertex].x + offset.x, vertexIt[iVertex].y + offset.y };
	angle             = atan2(-delta.x, delta.y);
	length /= 2.0F;
	offset             = fPOINT { length * cos(angle), length * sin(angle) };
	auto adjustedPoint = fPOINT { point.x + offset.x, point.y + offset.y };
	thred::stch2pxr(adjustedPoint);
	line[0] = line[3] = StitchCoordinatesPixels;
	adjustedPoint     = fPOINT { point.x - offset.x, point.y - offset.y };
	thred::stch2pxr(adjustedPoint);
	line[2] = StitchCoordinatesPixels;
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 4);
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::selsqr(const POINT& controlPoint, HDC dc) {
	POINT      line[5] = {};
	const auto offset  = MulDiv(gsl::narrow<int32_t>(IniFile.formVertexSizePixels), gsl::narrow<int32_t>(*screenDPI), 96);

	line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
	line[0].y = line[1].y = controlPoint.y - offset;
	line[1].x = line[2].x = controlPoint.x + offset;
	line[2].y = line[3].y = controlPoint.y + offset;
	line[4].y             = controlPoint.y - offset;
	Polyline(dc, static_cast<POINT*>(line), 5);
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::internal::frmsqr0(const POINT& controlPoint) {
	POINT      line[5] = {};
	const auto offset  = MulDiv(gsl::narrow<int32_t>(IniFile.formBoxSizePixels), gsl::narrow<int32_t>(*screenDPI), 96);

	if (offset != 0) {
		line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
		line[0].y = line[1].y = controlPoint.y - offset;
		line[1].x = line[2].x = controlPoint.x + offset + 1;
		line[2].y = line[3].y = controlPoint.y + offset + 1;
		line[4].y             = controlPoint.y - 1;
		Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 5);
	}
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::internal::frmx(const POINT& controlPoint, HDC dc) {
	POINT      line[2] = {};
	const auto offset  = MulDiv(8, gsl::narrow<int32_t>(*screenDPI), 96);

	SelectObject(dc, FormSelectedPen);
	line[0].x = line[1].x = controlPoint.x;
	line[0].y             = controlPoint.y + offset;
	line[1].y             = controlPoint.y - offset;
	Polyline(dc, static_cast<POINT*>(line), 2);
	line[0].y = line[1].y = controlPoint.y;
	line[0].x             = controlPoint.x - offset;
	line[1].x             = controlPoint.x + offset;
	Polyline(dc, static_cast<POINT*>(line), 2);
	SelectObject(dc, FormPen);
}

void form::ratsr() {
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = gsl::narrow_cast<float>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
		VerticalRatio   = gsl::narrow_cast<float>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		HorizontalRatio = gsl::narrow_cast<float>(StitchWindowClientRect.right) / UnzoomedRect.x;
		VerticalRatio   = gsl::narrow_cast<float>(StitchWindowClientRect.bottom) / UnzoomedRect.y;
	}
}

void form::ritfrct(uint32_t iForm, HDC dc) {
	POINT  pixelOutline[10] = {};
	fPOINT formOutline[10]  = {};

	ratsr();
	SelectObject(StitchWindowDC, FormPen);
	SetROP2(StitchWindowDC, R2_XORPEN);
	const auto& rectangle = FormList->operator[](iForm).rectangle;
	SelectObject(dc, FormSelectedPen);
	formOutline[0].x = formOutline[6].x = formOutline[7].x = formOutline[8].x = rectangle.left;
	formOutline[0].y = formOutline[1].y = formOutline[2].y = formOutline[8].y = rectangle.top;
	formOutline[2].x = formOutline[3].x = formOutline[4].x = rectangle.right;
	formOutline[4].y = formOutline[5].y = formOutline[6].y = rectangle.bottom;
	formOutline[1].x = formOutline[5].x = form::midl(rectangle.right, rectangle.left);
	formOutline[3].y = formOutline[7].y = form::midl(rectangle.top, rectangle.bottom);
	for (auto controlPoint = 0U; controlPoint < 8U; controlPoint++) {
		form::sfCor2px(formOutline[controlPoint], pixelOutline[controlPoint]);
	}
	form::sfCor2px(formOutline[0], pixelOutline[8]);
	Polyline(dc, static_cast<POINT*>(pixelOutline), 9);
	for (auto controlPoint = 0U; controlPoint < 8U; controlPoint++) {
		form::selsqr(pixelOutline[controlPoint], dc);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		StateMap.reset(StateFlag::SELSHO);
		SearchLine->clear();
		SearchLine->shrink_to_fit();
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::delfrms() {
	thred::savdo();
	FormVertices->clear();
	ClipPoints->clear();
	FormList->clear();
	SatinGuides->clear();
	for (auto stitch : *StitchBuffer) {
		stitch.attribute &= NFRM_NTYP;
		stitch.attribute |= NOTFRM;
	}
}

void form::fselrct(uint32_t iForm) {
	const auto& form           = FormList->operator[](iForm);
	fPOINT      formOutline[5] = {};
	POINT       line[6]        = {};

	formOutline[0].x = formOutline[3].x = formOutline[4].x = form.rectangle.left;
	formOutline[0].y = formOutline[1].y = formOutline[4].y = form.rectangle.top;
	formOutline[1].x = formOutline[2].x = form.rectangle.right;
	formOutline[2].y = formOutline[3].y = form.rectangle.bottom;
	for (auto iPoint = 0U; iPoint < 5U; iPoint++) {
		line[iPoint].x = wrap::round<int32_t>((formOutline[iPoint].x - ZoomRect.left) * HorizontalRatio);
		line[iPoint].y = wrap::round<int32_t>((ZoomRect.top - formOutline[iPoint].y) * VerticalRatio);
		if (line[iPoint].x < SelectedFormsRect.left) {
			SelectedFormsRect.left = line[iPoint].x;
		}
		if (line[iPoint].y < SelectedFormsRect.top) {
			SelectedFormsRect.top = line[iPoint].y;
		}
		if (line[iPoint].x > SelectedFormsRect.right) {
			SelectedFormsRect.right = line[iPoint].x;
		}
		if (line[iPoint].y > SelectedFormsRect.bottom) {
			SelectedFormsRect.bottom = line[iPoint].y;
		}
	}
	line[5].x = wrap::round<int32_t>((formOutline[0].x - ZoomRect.left) * HorizontalRatio);
	line[5].y = wrap::round<int32_t>((ZoomRect.top - formOutline[0].y) * VerticalRatio);
	if (line[5].x < SelectedFormsRect.left) {
		SelectedFormsRect.left = line[5].x;
	}
	if (line[5].y < SelectedFormsRect.top) {
		SelectedFormsRect.top = line[5].y;
	}
	if (line[5].x > SelectedFormsRect.right) {
		SelectedFormsRect.right = line[5].x;
	}
	if (line[5].y > SelectedFormsRect.bottom) {
		SelectedFormsRect.bottom = line[5].y;
	}
	if (OutLineEverySelectedForm) {
		Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 5);
	}
}

void form::rct2sel(const RECT& rectangle, std::vector<POINT>& line) noexcept {
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
	Polyline(StitchWindowMemDC, SelectedFormsLine->data(), 9);
	for (auto iPoint = 0U; iPoint < 8U; iPoint++) {
		form::selsqr((*SelectedFormsLine)[iPoint], StitchWindowMemDC);
	}
}

void form::internal::frmpoly(const POINT* const line, uint32_t count) noexcept {
	if (line != nullptr) {
		if (count != 0U) {
			for (auto iPoint = 0U; iPoint < count - 1U; iPoint++) {
				Polyline(StitchWindowMemDC, &line[iPoint], 2);
			}
		}
	}
}

void form::dupsel(HDC dc) {
	SelectObject(dc, FormPen);
	SetROP2(dc, R2_XORPEN);
	Polyline(dc, SelectedPointsLine->data(), 9);
	// iPoint = SelectedFormVertices.start;
	for (auto iPoint = 0U; iPoint < 8U; iPoint++) {
		form::selsqr((*SelectedPointsLine)[iPoint], dc);
	}
	fi::frmx(EndPointCross, dc);
}

void form::unpsel() {
	if (StateMap.testAndReset(StateFlag::SHOPSEL)) {
		form::dupsel(StitchWindowDC);
	}
}

void form::sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect) {
	screenRect.left = wrap::round<int32_t>((stitchRect.left - ZoomRect.left) * ZoomRatio.x + 0.5F);
	screenRect.top
	    = wrap::round<int32_t>((StitchWindowClientRect.bottom) - (stitchRect.top - ZoomRect.bottom) * ZoomRatio.y + 0.5F);
	screenRect.right = wrap::round<int32_t>((stitchRect.right - ZoomRect.left) * ZoomRatio.x + 0.5F);
	screenRect.bottom
	    = wrap::round<int32_t>((StitchWindowClientRect.bottom) - (stitchRect.bottom - ZoomRect.bottom) * ZoomRatio.y + 0.5F);
}

void form::drwfrm() {
	StateMap.reset(StateFlag::SHOMOV);
	StateMap.reset(StateFlag::SHOPSEL);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	ratsr();
	thred::duzrat();
	const auto maxForm = FormList->size();
	for (auto iForm = 0U; iForm < maxForm; iForm++) {
		form::fvars(iForm);
		const auto& form = FormList->operator[](iForm);
		form::frmlin(form.vertexIndex, VertexCount);
		if (!FormLines->empty()) {
			auto layer = gsl::narrow_cast<uint32_t>(gsl::narrow_cast<uint8_t>(form.attribute & FRMLMSK) >> 1U);
			if ((ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
				POINT line[2]   = {};
				auto  lastPoint = 0U;
				auto  vertexIt  = std::next(FormVertices->cbegin(), form.vertexIndex);
				if (form.type == SAT) {
					if ((form.attribute & FRMEND) != 0U) {
						SelectObject(StitchWindowMemDC, FormPen3px);
						Polyline(StitchWindowMemDC, FormLines->data(), 2);
						lastPoint = 1;
					}
					if (form.wordParam != 0U) {
						SelectObject(StitchWindowMemDC, FormPen);
						fi::frmpoly(&((*FormLines)[1]), form.wordParam);
						SelectObject(StitchWindowMemDC, FormPen3px);
						Polyline(StitchWindowMemDC, &((*FormLines)[form.wordParam]), 2);
						SelectObject(StitchWindowMemDC, LayerPen[layer]);
						lastPoint = form.wordParam + 1U;
					}
					const auto maxGuide = FormList->operator[](iForm).satinGuideCount;
					auto       guideIt  = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
					for (auto iGuide = 0U; iGuide < maxGuide; iGuide++) {
						form::sfCor2px(vertexIt[guideIt[iGuide].start], line[0]);
						form::sfCor2px(vertexIt[guideIt[iGuide].finish], line[1]);
						SelectObject(StitchWindowMemDC, FormPen);
						Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
					}
				}
				SelectObject(StitchWindowMemDC, LayerPen[layer]);
				if (form.type == FRMLINE) {
					fi::frmpoly(FormLines->data(), VertexCount);
					if (form.fillType == CONTF) {
						thred::sCor2px(vertexIt[form.angleOrClipData.guide.start], line[0]);
						thred::sCor2px(vertexIt[form.angleOrClipData.guide.finish], line[1]);
						Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
					}
				}
				else {
					fi::frmpoly(&((*FormLines)[lastPoint]), VertexCount + 1 - lastPoint);
				}
				if (ClosestFormToCursor == iForm && StateMap.test(StateFlag::FRMPSEL)) {
					auto& formLines = *FormLines;
					for (auto iVertex = 1U; iVertex < VertexCount; iVertex++) {
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
					displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
				}
				else {
					auto& formLines = *FormLines;
					for (auto iVertex = 1U; iVertex < VertexCount; iVertex++) {
						fi::frmsqr(form.vertexIndex, iVertex);
					}
					SelectObject(StitchWindowMemDC, FormSelectedPen);
					fi::frmsqr0(formLines[0]);
				}
				if (StateMap.test(StateFlag::FPSEL) && ClosestFormToCursor == iForm) {
					form::sRct2px(SelectedVerticesRect, SelectedPixelsRect);
					form::rct2sel(SelectedPixelsRect, *SelectedPointsLine);
					StateMap.set(StateFlag::SHOPSEL);
					form::dupsel(StitchWindowMemDC);
				}
			}
		}
	}
	if (!SelectedFormList->empty()) {
		SelectObject(StitchWindowMemDC, MultiFormPen);
		ratsr();
		SelectedFormsRect.top = SelectedFormsRect.left = LONG_MAX;
		SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
		for (auto selectedForm : (*SelectedFormList)) {
			fselrct(selectedForm);
		}
		SelectedFormsSize.x = gsl::narrow<decltype(SelectedFormsSize.x)>(SelectedFormsRect.right - SelectedFormsRect.left);
		SelectedFormsSize.y = gsl::narrow<decltype(SelectedFormsSize.x)>(SelectedFormsRect.bottom - SelectedFormsRect.top);
		dubig();
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::ritfrct(ClosestFormToCursor, StitchWindowMemDC);
		}
		if (StateMap.test(StateFlag::FRMPMOV)) {
			thred::ritmov(ClosestFormToCursor);
			(*RubberBandLine)[1].x = Msg.pt.x - StitchWindowOrigin.x;
			(*RubberBandLine)[1].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::SHOMOV);
			thred::ritmov(ClosestFormToCursor);
		}
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	form::fvars(ClosestFormToCursor);
}

void form::internal::setpoli() {
	auto form           = FRMHED {};
	form.type           = FRMFPOLY;
	form.attribute      = gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
	form.underlayIndent = IniFile.underlayIndent;
	FormList->push_back(form);
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void form::internal::setlin() {
	auto form      = FRMHED {};
	form.type      = FRMLINE;
	form.attribute = gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
	FormList->push_back(form);
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void form::internal::setrpoli() {
	thred::unmsg();
	displayText::pntmsg(IDS_REGP);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRPOL);
	displayText::numWnd();
}

void form::internal::setstar() {
	thred::unmsg();
	displayText::pntmsg(IDS_STAR);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRSTAR);
	displayText::numWnd();
}

void form::internal::setspir() {
	thred::unmsg();
	displayText::pntmsg(IDS_SPIR);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRSPIR);
	displayText::numWnd();
}

void form::internal::sethart() {
	thred::unmsg();
	displayText::pntmsg(IDS_HEART);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRHART);
	displayText::numWnd();
}

void form::internal::setlens() {
	thred::unmsg();
	displayText::pntmsg(IDS_LENS);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRLENS);
	displayText::numWnd();
}

void form::internal::seteg() {
	thred::unmsg();
	displayText::pntmsg(IDS_EGG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTREG);
	displayText::numWnd();
}

void form::internal::setzig() {
	thred::unmsg();
	displayText::pntmsg(IDS_ZIG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::ENTRZIG);
	displayText::numWnd();
}

void form::setmfrm() {
	auto  point     = POINT { 0L, 0L };
	auto& closeForm = FormList->operator[](ClosestFormToCursor);
	auto  vertexIt  = std::next(FormVertices->cbegin(), closeForm.vertexIndex);
	form::sfCor2px(vertexIt[0], point);
	const auto offset    = POINT { Msg.pt.x - StitchWindowOrigin.x - point.x + wrap::round<int32_t>(FormMoveDelta.x),
                                Msg.pt.y - StitchWindowOrigin.y - point.y + wrap::round<int32_t>(FormMoveDelta.y) };
	auto&      formLines = *FormLines;
	formLines.resize(wrap::toSize(closeForm.vertexCount) + 1U);
	for (auto iForm = 0U; iForm < closeForm.vertexCount; iForm++) {
		form::sfCor2px(vertexIt[iForm], point);
		formLines[iForm].x = point.x + offset.x;
		formLines[iForm].y = point.y + offset.y;
	}
	formLines[closeForm.vertexCount] = formLines[0];
}

void form::durpoli(uint32_t vertexCount) {
	if (vertexCount < 3) {
		vertexCount = 3;
	}
	const auto stepAngle = PI_F * 2.0F / gsl::narrow_cast<float>(vertexCount);
	// 500 gives us a reasonably sized default
	const auto length = 500.0F / gsl::narrow_cast<float>(vertexCount) * ZoomFactor
	                    * gsl::narrow_cast<float>(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	auto newForm        = FRMHED {};
	newForm.vertexIndex = thred::adflt(vertexCount);
	newForm.vertexCount = vertexCount;
	newForm.attribute   = gsl::narrow<decltype(newForm.attribute)>(ActiveLayer << 1U);
	newForm.type        = FRMFPOLY;
	FormList->push_back(newForm);
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point    = SelectedPoint;
	auto angle    = 0.0F;
	auto vertexIt = std::next(FormVertices->begin(), newForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < vertexCount; iVertex++) {
		vertexIt[iVertex].x = point.x;
		vertexIt[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	form::frmout(ClosestFormToCursor);
	FormMoveDelta      = fPOINT {};
	NewFormVertexCount = vertexCount + 1U;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

auto form::nxt(uint32_t iVertex) noexcept -> uint32_t {
	iVertex++;
	if (iVertex > VertexCount - 1) {
		iVertex = 0;
	}
	return iVertex;
}

auto form::prv(uint32_t iVertex) noexcept -> uint32_t {
	if (iVertex != 0U) {
		iVertex--;
	}
	else {
		iVertex = VertexCount - 1U;
	}
	return iVertex;
}

auto form::pdir(uint32_t vertex) -> uint32_t {
	if (StateMap.test(StateFlag::PSELDIR)) {
		return form::nxt(vertex);
	}

	return form::prv(vertex);
}

void form::pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect) noexcept {
	auto corner = POINT { screenRect.left + StitchWindowOrigin.x, screenRect.top + StitchWindowOrigin.y };
	thred::pxCor2stch(corner);
	stitchRect.left = SelectedPoint.x;
	stitchRect.top  = SelectedPoint.y;
	corner.x        = screenRect.right + StitchWindowOrigin.x;
	corner.y        = screenRect.bottom + StitchWindowOrigin.y;
	thred::pxCor2stch(corner);
	stitchRect.right  = SelectedPoint.x;
	stitchRect.bottom = SelectedPoint.y;
}

void form::flipv() {
	form::fvars(ClosestFormToCursor);
	auto& form = FormList->operator[](ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		const auto offset        = SelectedVerticesRect.top + SelectedVerticesRect.bottom;
		auto       currentVertex = SelectedFormVertices.start;
		auto       vertexIt      = std::next(FormVertices->begin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexIt[currentVertex].y = offset - vertexIt[currentVertex].y;
			currentVertex             = pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		const auto offset = AllItemsRect.top + AllItemsRect.bottom;
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
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (!SelectedFormList->empty()) {
		auto formMap = boost::dynamic_bitset<> { FormList->size() };

		auto rectangle = fRECTANGLE {};
		form::pxrct2stch(SelectedFormsRect, rectangle);
		const auto offset = rectangle.top + rectangle.bottom;
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			const auto& iForm    = FormList->operator[](ClosestFormToCursor);
			formMap.set(ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			auto vertexIt = std::next(FormVertices->begin(), iForm.vertexIndex);
			for (auto iVertex = 0U; iVertex < iForm.vertexCount; iVertex++) {
				vertexIt[iVertex].y = offset - vertexIt[iVertex].y;
			}
			form::frmout(ClosestFormToCursor);
		}
		for (auto& stitch : *StitchBuffer) {
			const auto decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && ((stitch.attribute & NOTFRM) == 0U)) {
				stitch.y = offset - stitch.y;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			const auto  offset   = form.rectangle.top + form.rectangle.bottom;
			auto       vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
			for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].y = offset - vertexIt[iVertex].y;
			}
			for (auto& stitch : *StitchBuffer) {
				if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor && ((stitch.attribute & NOTFRM) == 0U)) {
					stitch.y = offset - stitch.y;
				}
			}
			form::frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				auto rectangle = fRECTANGLE {};
				thred::selRct(rectangle);
				const auto offset = rectangle.top + rectangle.bottom;
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					(*StitchBuffer)[iStitch].y = offset - (*StitchBuffer)[iStitch].y;
				}
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::duform(uint32_t formType) {
	switch (formType + 1U) {
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
	}
}

auto form::internal::findDistanceToSide(const fPOINT& lineStart,
                                        const fPOINT& lineEnd,
                                        const fPOINT& point,
                                        double&       distance) noexcept -> float {
	const auto A     = gsl::narrow_cast<double>(point.x) - lineStart.x;
	const auto B     = gsl::narrow_cast<double>(point.y) - lineStart.y;
	const auto C     = gsl::narrow_cast<double>(lineEnd.x) - lineStart.x;
	const auto D     = gsl::narrow_cast<double>(lineEnd.y) - lineStart.y;
	auto       diffX = 0.0;
	auto       diffY = 0.0;

	if ((C == 0) && (D == 0)) {
		diffX    = A;
		diffY    = B;
		distance = sqrt(diffX * diffX + diffY * diffY);
		// Arbitrarily choose the first point since start and end are the same
		return -0.1F;
	}

	const auto dot    = A * C + B * D;
	const auto len_sq = C * C + D * D;
	const auto param  = gsl::narrow_cast<float>(dot / len_sq);

	if (param < 0) {
		// point is nearest to the first point i.e lineStart.x and lineStart.y
		diffX = gsl::narrow_cast<double>(point.x) - lineStart.x;
		diffY = gsl::narrow_cast<double>(point.y) - lineStart.y;
	}
	else if (param > 1) {
		// point is nearest to the end point i.e lineEnd.x and lineEnd.y
		diffX = gsl::narrow_cast<double>(point.x) - lineEnd.x;
		diffY = gsl::narrow_cast<double>(point.y) - lineEnd.y;
	}
	else {
		// if perpendicular line intersect the line segment.
		diffX = point.x - (lineStart.x + param * C);
		diffY = point.y - (lineStart.y + param * D);
	}

	// returning shortest distance
	distance = sqrt(diffX * diffX + diffY * diffY);
	return param;
}

auto form::closfrm() -> bool {
	if (!FormList->empty()) {
		const auto screenCoordinate = POINT { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
		fi::rats();
		auto closestForm   = 0U;
		auto closestVertex = 0U;
		auto point         = fPOINT {};
		auto minimumLength = 1e99;
		fi::px2stchf(screenCoordinate, point);
		const auto layerCoded  = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
		const auto maxForm     = wrap::toUnsigned(FormList->size());
		const auto savedVertex = VertexCount;
		for (auto iForm = 0U; iForm < maxForm; iForm++) {
			if (StateMap.test(StateFlag::FRMSAM) && iForm == ClosestFormToCursor) {
				continue;
			}
			auto& currentForm    = FormList->operator[](iForm);
			VertexCount          = currentForm.vertexCount;
			const auto formLayer = gsl::narrow_cast<uint8_t>(currentForm.attribute & FRMLMSK);
			if ((ActiveLayer == 0U) || (formLayer == 0U) || formLayer == layerCoded) {
				auto vertexIt = std::next(FormVertices->cbegin(), currentForm.vertexIndex);
				// find the closest line first and then find the closest vertex on that line
				auto       length    = 0.0;
				const auto sideCount = currentForm.vertexCount;
				for (auto iVertex = 0U; iVertex < sideCount; iVertex++) {
					const auto param = fi::findDistanceToSide(vertexIt[iVertex], vertexIt[form::nxt(iVertex)], point, length);
					if ((length < minimumLength) && (length >= 0.0)) {
						minimumLength = length;
						closestForm   = iForm;
						if (param < 0.5F) {
							closestVertex = iVertex;
						}
						else {
							closestVertex = form::nxt(iVertex);
						}
					}
				}
			}
		}
		VertexCount  = savedVertex;
		auto& vertex = (*FormVertices)[wrap::toSize(FormList->operator[](closestForm).vertexIndex) + closestVertex];
		thred::stch2pxr(vertex);
		minimumLength = hypot(StitchCoordinatesPixels.x - screenCoordinate.x, StitchCoordinatesPixels.y - screenCoordinate.y);
		if (minimumLength < CLOSENUF) {
			ClosestFormToCursor   = closestForm;
			ClosestVertexToCursor = closestVertex;
			form::fvars(ClosestFormToCursor);
			StateMap.set(StateFlag::RELAYR);
			return true;
		}
		return false;
	}

	return false;
}

void form::frmovlin() {
	form::fvars(ClosestFormToCursor);
	const auto& form = FormList->operator[](ClosestFormToCursor);
	ratsr();
	if (form.type == FRMLINE) {
		NewFormVertexCount = form.vertexCount;
	}
	else {
		NewFormVertexCount = form.vertexCount + 1U;
	}
	form::frmlin(form.vertexIndex, form.vertexCount);
	auto  previousPoint = form::prv(ClosestVertexToCursor);
	auto& formLines     = *FormLines;
	for (auto iPoint = 0; iPoint < 3; iPoint++) {
		(*RubberBandLine)[iPoint] = formLines[previousPoint];
		previousPoint             = form::nxt(previousPoint);
	}
	thred::ritmov(ClosestFormToCursor);
}

auto form::internal::ritlin(const fPOINT& start, const fPOINT& finish, float userStitchLen) -> bool {
	const auto delta  = fPOINT { finish.x - start.x, finish.y - start.y };
	auto       length = hypot(delta.x, delta.y);
	// This clamp is temporary to avoid overflow when BH corner value is too large. Find a better fix
	if (length > 200.0F) {
		length = 200.0F;
	}

	InterleaveSequence->push_back(start);
	if (length > MaxStitchLen) {
		if (userStitchLen < 1e-1f) {
			userStitchLen = 1e-1f;
		}
		auto count = wrap::ceil<uint32_t>(length / userStitchLen);
		if (count == 0U) {
			count = 1;
		}
		while (length / count > MaxStitchLen) {
			count++;
		}
		const auto step  = fPOINT { delta.x / count, delta.y / count };
		auto       point = fPOINT { start.x + step.x, start.y + step.y };
		for (auto iStep = 0U; iStep < count - 1U; iStep++) {
			InterleaveSequence->push_back(point);
			point.x += step.x;
			point.y += step.y;
		}
	}
	return true;
}

auto form::closflt(const FRMHED& form, float xCoordinate, float yCoordinate) -> uint32_t {
	auto closestVertex = 0U;

	auto minimumLength = 1e99;
	auto vertexIt      = std::next(FormVertices->cbegin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
		const auto length = hypot(xCoordinate - vertexIt[iVertex].x, yCoordinate - vertexIt[iVertex].y);
		if (length < minimumLength) {
			closestVertex = iVertex;
			minimumLength = length;
		}
	}
	return closestVertex;
}

void form::chkseq(bool border) {
#if BUGBAK
	UNREFERENCED_PARAMETER(border);

	for (auto index = 0U; index < SequenceIndex; index++) {
		InterleaveSequence->push_back((*OSequence)[index]);
	}
#else

	auto minimumStitchLength = 0.0F;
	auto userStitchLen       = 0.0F;

	const auto savedIndex = InterleaveSequence->size();
	if (border) {
		auto& form = FormList->operator[](ClosestFormToCursor);
		if (form.maxBorderStitchLen == 0.0F) {
			form.maxBorderStitchLen = IniFile.maxStitchLength;
		}
		MaxStitchLen = form.maxBorderStitchLen;
		if (form.edgeType == EDGELCHAIN || form.edgeType == EDGEOCHAIN) {
			userStitchLen = 9.0F * PFGRAN;
		}
		else {
			userStitchLen = form.edgeStitchLen;
		}
		minimumStitchLength = form.minBorderStitchLen;
	}
	else {
		auto& form = FormList->operator[](ClosestFormToCursor);
		if (form.maxFillStitchLen == 0.0F) {
			form.maxFillStitchLen = IniFile.maxStitchLength;
		}
		MaxStitchLen = form.maxFillStitchLen;
		if (clip::isclp(form)) {
			userStitchLen = MaxStitchLen;
		}
		else {
			userStitchLen = form.lengthOrCount.stitchLength;
		}
		minimumStitchLength = form.minFillStitchLen;
	}
	if (userStitchLen > MaxStitchLen) {
		userStitchLen = MaxStitchLen;
	}
	bool flag = true;
	if (!OSequence->empty()) {
		for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size()) - 1U; iSequence++) {
			if (!fi::ritlin((*OSequence)[iSequence], (*OSequence)[wrap::toSize(iSequence) + 1U], userStitchLen)) {
				flag = false;
				break;
			}
		}
		if (flag) {
			InterleaveSequence->push_back(OSequence->back());
		}
	}
	else {
		OutputDebugString(L"OSequence empty!");
	}
	if (minimumStitchLength == 0.0F) {
		return;
	}
	auto destination = wrap::toUnsigned(savedIndex + 1U);
	for (auto iSequence = savedIndex + 1U; iSequence < InterleaveSequence->size(); iSequence++) {
		const auto len = hypot((*InterleaveSequence)[iSequence].x - (*InterleaveSequence)[iSequence - 1U].x,
			(*InterleaveSequence)[iSequence].y - (*InterleaveSequence)[iSequence - 1U].y);
		if (len > minimumStitchLength) {
			(*InterleaveSequence)[destination] = (*InterleaveSequence)[iSequence];
			++destination;
		}
	}
	InterleaveSequence->resize(destination);
#endif
}

// ToDo - move form variable into calling function
void form::internal::ritbrd(const FRMHED& form) {
	if (!OSequence->empty()) {
		InterleaveSequenceIndices->emplace_back(INSREC { TYPBRD,
		                                                 gsl::narrow<uint32_t>(form.borderColor) & COLMSK,
		                                                 wrap::toUnsigned(InterleaveSequence->size()),
		                                                 I_BRD });
		form::chkseq(true);
	}
}

void form::internal::ritapbrd() {
	if (!OSequence->empty()) {
		const auto& form = FormList->operator[](ClosestFormToCursor);
		InterleaveSequenceIndices->emplace_back(INSREC {
		    TYPMSK, gsl::narrow<uint32_t>(form.borderColor) >> 4U, wrap::toUnsigned(InterleaveSequence->size()), I_AP });
		form::chkseq(true);
	}
}

void form::internal::ritfil() {
	if (!OSequence->empty()) {
		const auto& form = FormList->operator[](ClosestFormToCursor);
		InterleaveSequenceIndices->emplace_back(
		    INSREC { TYPFRM, form.fillColor, gsl::narrow<uint32_t>(InterleaveSequence->size()), I_FIL });
		form::chkseq(false);
	}
}

auto form::lastch() -> bool {
	if (!InterleaveSequence->empty()) {
		LastPoint = InterleaveSequence->back();
		return true;
	}
	{ return false; }
}

auto form::getlast(const FRMHED& form) -> uint32_t {
	if (form.fillType != 0U) {
		form::lastch();
		auto minimumLength = 1e99;
		auto closestVertex = 0U;
		auto vertexIt      = std::next(FormVertices->cbegin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
			const auto dx     = LastPoint.x - vertexIt[iVertex].x;
			const auto dy     = LastPoint.y - vertexIt[iVertex].y;
			const auto length = hypot(dx, dy);
			if (length < minimumLength) {
				minimumLength = length;
				closestVertex = iVertex;
			}
		}
		return closestVertex;
	}
	{ return 0; }
}

void form::filinsb(const fPOINT& point) {
	const auto delta  = fPOINT { (point.x - SelectedPoint.x), (point.y - SelectedPoint.y) };
	const auto length = hypot(delta.x, delta.y);
	auto       count  = wrap::round<uint32_t>(length / MAXSTCH + 1.0F);
	const auto step   = fPOINT { (delta.x / count), (delta.y / count) };

	if (length > MAXSTCH) {
		count--;
		if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
			return;
		}
		while (count != 0U) {
			SelectedPoint.x += step.x;
			SelectedPoint.y += step.y;
			OSequence->push_back(SelectedPoint);
			count--;
		}
	}
	OSequence->push_back(point);
	SelectedPoint.x = point.x;
	SelectedPoint.y = point.y;
}

auto form::getblen() noexcept -> float {
	auto&      form    = FormList->operator[](ClosestFormToCursor);
	const auto iLength = (form.clipEntries << 16U) | form.picoLength;
	return gsl::narrow_cast<float>(iLength);
}

void form::savblen(float fLength) {
	auto  iLength = wrap::round<uint32_t>(fLength);
	auto& form    = FormList->operator[](ClosestFormToCursor);

	form.clipEntries = iLength >> 16U;
	form.picoLength  = iLength & 0xFFFFU;
}

auto form::getplen() noexcept -> float {
	const auto& form = FormList->operator[](ClosestFormToCursor);
	const auto value = form.picoLength;

	return (gsl::narrow_cast<float>(value >> 8U) + gsl::narrow_cast<float>(value & 0xffU) / 256.0F);
}

void form::savplen(float length) {
	auto integerPart = 0.0;

	if (length > 255.0F) {
		length = 255.0F;
	}
	const auto fractionalPart = std::modf(length, &integerPart);
	auto       fr             = wrap::floor<uint16_t>(fractionalPart * 256.0);
	auto       num            = gsl::narrow<uint32_t>(integerPart);

	FormList->operator[](ClosestFormToCursor).picoLength = ((num << 8U) & 0xff00U) | fr;
}

void form::internal::bdrlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float stitchSize) {
	auto       vertexIt    = std::next(FormVertices->cbegin(), vertexIndex);
	const auto delta       = fPOINT { (vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y) };
	const auto length      = hypot(delta.x, delta.y);
	auto       stitchCount = 0U;
	auto       step        = fPOINT {};

	if (UserFlagMap.test(UserFlag::LINSPAC)) {
		stitchCount = wrap::round<uint32_t>(length / stitchSize + 0.5F);
		if (stitchCount != 0U) {
			step.x = delta.x / stitchCount;
			step.y = delta.y / stitchCount;
		}
	}
	else {
		stitchCount      = wrap::round<uint32_t>((length - stitchSize / 2.0F) / stitchSize + 1.0F);
		const auto angle = atan2(delta.y, delta.x);
		step.x           = cos(angle) * stitchSize;
		step.y           = sin(angle) * stitchSize;
	}
	if (stitchCount != 0U) {
		auto point = fPOINT { vertexIt[start].x + step.x, vertexIt[start].y + step.y };
		stitchCount--;
		while (stitchCount != 0U) {
			OSequence->push_back(point);
			point.x += step.x;
			point.y += step.y;
			stitchCount--;
		}
	}
	OSequence->push_back(vertexIt[finish]);
}

void form::internal::brdfil(const FRMHED& form) {
	auto currentVertex = 0U;

	if ((form.extendedAttribute & AT_STRT) != 0U) {
		currentVertex = form.fillStart;
	}
	else {
		currentVertex = form::getlast(form);
	}
	OSequence->clear();
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	OSequence->push_back(vertexIt[currentVertex]);
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		const auto nextVertex = form::nxt(currentVertex);
		bdrlin(form.vertexIndex, currentVertex, nextVertex, form.edgeStitchLen);
		currentVertex = nextVertex;
	}
	if (form.type != FRMLINE) {
		const auto nextVertex = form::nxt(currentVertex);
		bdrlin(form.vertexIndex, currentVertex, nextVertex, form.edgeStitchLen);
	}
}

void form::internal::boldlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, double size) {
	auto       vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
	const auto delta    = fPOINT { (vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y) };
	const auto length   = hypot(delta.x, delta.y);
	if (size < 1e-1) {
		size = 1e-1;
	}
	auto       count    = wrap::round<uint32_t>(length / size);

	if (count != 0U) {
		const auto step   = fPOINT { delta.x / count, delta.y / count };
		auto       point0 = vertexIt[start];
		auto       point1 = fPOINT { point0.x + step.x, point0.y + step.y };
		while (count != 0U) {
			OSequence->push_back(point1);
			OSequence->push_back(point0);
			OSequence->push_back(point1);
			point0.x += step.x;
			point0.y += step.y;
			point1.x += step.x;
			point1.y += step.y;
			count--;
		}
		OSequence->push_back(vertexIt[finish]);
	}
	else {
		OSequence->push_back(vertexIt[finish]);
		OSequence->push_back(vertexIt[start]);
		OSequence->push_back(vertexIt[finish]);
	}
}

void form::internal::bold(const FRMHED& form) {
	auto iOutput   = 0U;
	auto iNextLine = form::getlast(form);
	auto iLine     = iNextLine;

	OSequence->clear();
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	OSequence->push_back(vertexIt[iLine]);
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		iNextLine = form::nxt(iLine);
		boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
		iLine = iNextLine;
	}
	if (form.type != FRMLINE) {
		iNextLine = form::nxt(iLine);
		boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
	}
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size() - 1U); iSequence++) {
		const auto length = hypot((*OSequence)[wrap::toSize(iSequence) + 1U].x - (*OSequence)[iSequence].x,
		                          (*OSequence)[wrap::toSize(iSequence) + 1U].y - (*OSequence)[iSequence].y);
		if (length > TINY) {
			(*OSequence)[iOutput++] = (*OSequence)[iSequence];
		}
	}
	(*OSequence)[iOutput++] = vertexIt[iNextLine];
	OSequence->resize(iOutput);
}

void form::duangs(const FRMHED& form) {
	FormAngles->clear();
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
		vertexIt = std::next(AngledFormVertices->cbegin(), form.vertexIndex);
	}
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		auto& thisVertex = vertexIt[iVertex];
		auto& nextVertex = vertexIt[wrap::toSize(iVertex) + 1U];
		FormAngles->push_back(atan2(nextVertex.y - thisVertex.y, nextVertex.x - thisVertex.x));
	}
	FormAngles->push_back(atan2(vertexIt[0].y - vertexIt[VertexCount - 1U].y, vertexIt[0].x - vertexIt[VertexCount - 1U].x));
}

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
auto form::internal::projv(float xCoordinate, const fPOINT& lowerPoint, const fPOINT& upperPoint, fPOINT& intersection) noexcept
    -> bool {
	const auto deltaX = upperPoint.x - lowerPoint.x;

	intersection.x = xCoordinate;

	if (deltaX != 0.0) {
		const auto slope = (upperPoint.y - lowerPoint.y) / deltaX;
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

auto form::cisin(const FRMHED& form, float xCoordinate, float yCoordinate) -> bool {
	const auto& rectangle = form.rectangle;

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
	auto vertexIt     = std::next(FormVertices->cbegin(), form.vertexIndex);
	if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
		vertexIt = std::next(AngledFormVertices->cbegin(), form.vertexIndex);
	}
	for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
		const auto nextVertex = nxt(iVertex);
		if (fi::projv(xCoordinate, vertexIt[iVertex], vertexIt[nextVertex], intersection)) {
			if (intersection.y >= yCoordinate) {
				if (vertexIt[iVertex].x != xCoordinate && vertexIt[nextVertex].x != xCoordinate) {
					count++;
				}
				else {
					if (vertexIt[iVertex].x < vertexIt[nextVertex].x) {
						if (vertexIt[nextVertex].x != xCoordinate) {
							count++;
						}
					}
					else {
						if (vertexIt[iVertex].x != xCoordinate) {
							count++;
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
auto form::internal::proj(const fPOINT& point,
                          float         slope,
                          const fPOINT& point0,
                          const fPOINT& point1,
                          fPOINT&       intersectionPoint) noexcept -> bool {
	const auto delta = fPOINT { point1.x - point0.x, point1.y - point0.y };
	if (delta.x != 0.0F) {
		const auto sideSlope     = delta.y / delta.x;
		const auto sideConstant  = point0.y - sideSlope * point0.x;
		const auto pointConstant = point.y - slope * point.x;
		intersectionPoint.x      = (sideConstant - pointConstant) / (slope - sideSlope);
		intersectionPoint.y      = intersectionPoint.x * slope + pointConstant;
	}
	else {
		intersectionPoint.x      = point0.x;
		const auto pointConstant = point.y - slope * point.x;
		intersectionPoint.y      = intersectionPoint.x * slope + pointConstant;
	}
	auto xMinimum = point0.x;
	auto xMaximum = point1.x;
	if (xMinimum > xMaximum) {
		std::swap(xMinimum, xMaximum);
	}
	if (delta.y != 0.0) {
		auto yMinimum = point0.y;
		auto yMaximum = point1.y;
		if (yMinimum > yMaximum) {
			std::swap(yMinimum, yMaximum);
		}
		return !(intersectionPoint.x <= xMinimum || intersectionPoint.x > xMaximum || intersectionPoint.y < yMinimum
		         || intersectionPoint.y > yMaximum);
	}

	return !(intersectionPoint.x <= xMinimum || intersectionPoint.x > xMaximum);
}

auto form::linx(const std::vector<fPOINT>& points, uint32_t start, uint32_t finish, fPOINT& intersection) noexcept -> bool {
	if (OutsidePoints != nullptr) {
		const auto delta
		    = fPOINT { ((*OutsidePoints)[start].x - points[start].x), ((*OutsidePoints)[start].y - points[start].y) };
		const auto point = points[start];

		if ((delta.x == 0.0F) && (delta.y == 0.0F)) {
			return false;
		}
		if (delta.x != 0.0F) {
			return fi::proj(point, delta.y / delta.x, (*OutsidePoints)[finish], points[finish], intersection);
		}

		return fi::projv(point.x, points[finish], (*OutsidePoints)[finish], intersection);
	}

	return false;
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
auto form::internal::projh(float yCoordinate, const fPOINT& point0, const fPOINT& point1, fPOINT& intersection) noexcept -> bool {
	const auto deltaX = point1.x - point0.x;

	intersection.y = yCoordinate;
	if (deltaX != 0.0F) {
		const auto deltaY = point1.y - point0.y;
		if (deltaY != 0.0F) {
			const auto slope = deltaY / deltaX;
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

void form::internal::sprct(std::vector<VRCT2>& fillVerticalRect, uint32_t start, uint32_t finish) {
	auto&      opStart      = (*OutsidePoints)[start];
	auto&      opFinish     = (*OutsidePoints)[finish];
	auto&      ipStart      = (*InsidePoints)[start];
	auto&      ipFinish     = (*InsidePoints)[finish];
	const auto delta        = fPOINT { (opFinish.x - opStart.x), (opFinish.y - opStart.y) };
	auto       point        = fPOINT {};
	auto&      verticalRect = fillVerticalRect[start];

	const auto& form = FormList->operator[](ClosestFormToCursor);
	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
		vertexIt = std::next(AngledFormVertices->cbegin(), CurrentVertexIndex);
	}
	if ((delta.x != 0.0F) && (delta.y != 0.0F)) {
		const auto slope = -delta.x / delta.y;
		point            = vertexIt[finish];
		proj(point, slope, opStart, opFinish, verticalRect.dopnt);
		proj(point, slope, ipStart, ipFinish, verticalRect.dipnt);
		point = vertexIt[start];
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
		if (delta.x != 0.0) {
			point.x = vertexIt[finish].x;
			projv(point.x, opStart, opFinish, verticalRect.dopnt);
			projv(point.x, ipStart, ipFinish, verticalRect.dipnt);
			point.x = vertexIt[start].x;
			projv(point.x, opStart, opFinish, verticalRect.aopnt);
			projv(point.x, ipStart, ipFinish, verticalRect.aipnt);
			point.x = ipStart.x;
			if (projv(point.x, opStart, opFinish, verticalRect.bopnt)) {
				verticalRect.bipnt = ipStart;
			}
			else {
				verticalRect.bopnt = opStart;
				point.x            = opStart.x;
				projv(point.x, ipStart, ipFinish, verticalRect.bipnt);
			}
			point.x = ipFinish.x;
			if (projv(point.x, opStart, opFinish, verticalRect.copnt)) {
				verticalRect.cipnt = ipFinish;
			}
			else {
				verticalRect.copnt = opFinish;
				point.x            = opFinish.x;
				projv(point.x, ipStart, ipFinish, verticalRect.cipnt);
			}
		}
		else {
			point.y = vertexIt[finish].y;
			projh(point.y, opStart, opFinish, verticalRect.dopnt);
			projh(point.y, ipStart, ipFinish, verticalRect.dipnt);
			point.y = vertexIt[start].y;
			projh(point.y, opStart, opFinish, verticalRect.aopnt);
			projh(point.y, ipStart, ipFinish, verticalRect.aipnt);
			point.y = ipStart.y;
			if (projh(point.y, opStart, opFinish, verticalRect.bopnt)) {
				verticalRect.bipnt = ipStart;
			}
			else {
				verticalRect.bopnt = opStart;
				point.y            = opStart.y;
				projh(point.y, ipStart, ipFinish, verticalRect.bipnt);
			}
			point.y = ipFinish.y;
			if (projh(point.y, opStart, opFinish, verticalRect.copnt)) {
				verticalRect.cipnt = ipFinish;
			}
			else {
				verticalRect.copnt = opFinish;
				point.y            = opFinish.y;
				projh(opFinish.y, ipStart, ipFinish, verticalRect.cipnt);
			}
		}
	}
}

void form::internal::spurfn(const fPOINT& innerPoint,
                            const fPOINT& outerPoint,
                            fPOINT&       underlayInnerPoint,
                            fPOINT&       underlayOuterPoint) noexcept {
	const auto delta     = fPOINT { outerPoint.x - innerPoint.x, outerPoint.y - innerPoint.y };
	underlayInnerPoint.x = delta.x * DIURAT + innerPoint.x;
	underlayInnerPoint.y = delta.y * DIURAT + innerPoint.y;
	underlayOuterPoint.x = delta.x * DOURAT + innerPoint.x;
	underlayOuterPoint.y = delta.y * DOURAT + innerPoint.y;
}

void form::internal::spurct(std::vector<VRCT2>&       underlayVerticalRect,
                            const std::vector<VRCT2>& fillVerticalRect,
                            uint32_t                  iRect) noexcept {
	auto& fRect  = fillVerticalRect[iRect];
	auto& ulRect = underlayVerticalRect[iRect];
	spurfn(fRect.aipnt, fRect.aopnt, ulRect.aipnt, ulRect.aopnt);
	spurfn(fRect.bipnt, fRect.bopnt, ulRect.bipnt, ulRect.bopnt);
	spurfn(fRect.cipnt, fRect.copnt, ulRect.cipnt, ulRect.copnt);
	spurfn(fRect.dipnt, fRect.dopnt, ulRect.dipnt, ulRect.dopnt);
}

void form::internal::duromb(const fPOINT& start0, const fPOINT& finish0, const fPOINT& start1, const fPOINT& finish1) {
	if (!StateMap.test(StateFlag::UND)) {
		const auto delta0  = fPOINT { SelectedPoint.x - start0.x, SelectedPoint.y - start0.y };
		const auto delta1  = fPOINT { SelectedPoint.x - start1.x, SelectedPoint.y - start1.y };
		const auto length0 = hypot(delta0.x, delta0.y);
		const auto length1 = hypot(delta1.x, delta1.y);
		if (length0 > length1) {
			StateMap.set(StateFlag::FILDIR);
		}
		else {
			StateMap.reset(StateFlag::FILDIR);
		}
	}
	const auto delta0  = fPOINT { finish0.x - start0.x, finish0.y - start0.y };
	const auto delta1  = fPOINT { finish1.x - start1.x, finish1.y - start1.y };
	const auto length0 = hypot(delta0.x, delta0.y);
	auto       count   = wrap::round<uint32_t>(length0 / (LineSpacing / 2.0F));
	if (count == 0U) {
		count++;
	}
	const auto step0 = fPOINT { delta0.x / count, delta0.y / count };
	const auto step1 = fPOINT { delta1.x / count, delta1.y / count };

	auto start00 = start0;
	auto start11 = start1;
	for (auto iStep = 0U; iStep < count; iStep++) {
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			form::filinsb(start00);
		}
		else {
			form::filinsb(start11);
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
	const auto temp = PseudoRandomValue & 0x48000000U;
	PseudoRandomValue <<= 1U;
	if (temp == 0x40000000U || temp == 0x8000000U) {
		PseudoRandomValue++;
	}
	return PseudoRandomValue;
}

void form::internal::spend(const std::vector<VRCT2>& fillVerticalRect, uint32_t start, uint32_t finish) {
	const auto innerDelta  = fPOINT { (fillVerticalRect[finish].cipnt.x - fillVerticalRect[start].bipnt.x),
                                     (fillVerticalRect[finish].cipnt.y - fillVerticalRect[start].bipnt.y) };
	const auto innerLength = hypot(innerDelta.x, innerDelta.y);
	const auto outerDelta  = fPOINT { (fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
                                     (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y) };
	const auto outerLength = hypot(outerDelta.x, outerDelta.y);
	auto       pivot       = fPOINT {};
	auto       startDelta  = fPOINT {};
	auto       finishDelta = fPOINT {};

	if (outerLength > innerLength) {
		pivot         = fillVerticalRect[start].cipnt;
		startDelta.x  = fillVerticalRect[start].copnt.x - pivot.x;
		startDelta.y  = fillVerticalRect[start].copnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bopnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bopnt.y - pivot.y;
	}
	else {
		pivot         = fillVerticalRect[start].copnt;
		startDelta.x  = fillVerticalRect[start].cipnt.x - pivot.x;
		startDelta.y  = fillVerticalRect[start].cipnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bipnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bipnt.y - pivot.y;
	}
	if (hypot(SelectedPoint.x - pivot.x, SelectedPoint.y - pivot.y) > 2.0F * PI_F) {
		form::filinsb(pivot);
	}
	auto       startAngle  = atan2(startDelta.y, startDelta.x);
	const auto finishAngle = atan2(finishDelta.y, finishDelta.x);
	auto       deltaAngle  = finishAngle - startAngle;
	if (deltaAngle > PI_F) {
		deltaAngle -= 2.0F * PI_F;
	}
	if (deltaAngle < -PI_F) {
		deltaAngle += 2.0F * PI_F;
	}
	const auto radius    = hypot(startDelta.x, startDelta.y);
	const auto arc       = fabs(radius * deltaAngle);
	auto       count     = wrap::round<uint32_t>(arc / LineSpacing);
	const auto stepAngle = deltaAngle / count;
	if (count == 0U) {
		count = 1;
	}
	for (auto ind = 0U; ind < count; ind++) {
		const auto outerPoint = fPOINT { pivot.x + cos(startAngle) * radius, pivot.y + sin(startAngle) * radius };
		form::filinsb(outerPoint);
		auto level = 0U;
		if ((count & 0xfffffff0) != 0U) {
			level = form::psg() % count;
		}
		else {
			level = Levels[count][ind];
		}
		const auto innerRadius = radius * level / count * 0.4;
		const auto innerPoint  = fPOINT { pivot.x + cos(startAngle) * innerRadius, pivot.y + sin(startAngle) * innerRadius };
		form::filinsb(innerPoint);
		startAngle += stepAngle;
	}
}

void form::internal::duspnd(const FRMHED&             form,
                            const std::vector<VRCT2>& underlayVerticalRect,
                            const std::vector<VRCT2>& fillVerticalRect,
                            uint32_t                  start,
                            uint32_t                  finish,
                            float                     width) {
	if (StateMap.test(StateFlag::UND)) {
		if (StateMap.test(StateFlag::UNDPHAS)) {
			form::filinsb(underlayVerticalRect[start].copnt);
			form::filinsb(underlayVerticalRect[start].cipnt);
			const auto delta  = fPOINT { underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x,
                                        underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y };
			const auto length = hypot(delta.x, delta.y);
			if (length > form.edgeStitchLen) {
				const auto angle = atan2((*InsidePoints)[finish].y - (*OutsidePoints)[finish].y,
				                         (*InsidePoints)[finish].x - (*OutsidePoints)[finish].x);
				const auto point = fPOINT { underlayVerticalRect[finish].bopnt.x + cos(angle) * width,
					                        underlayVerticalRect[finish].bopnt.y + sin(angle) * width };
				form::filinsb(point);
			}
			form::filinsb(underlayVerticalRect[finish].bipnt);
			form::filinsb(underlayVerticalRect[finish].bopnt);
		}
		else {
			form::filinsb(underlayVerticalRect[start].cipnt);
			form::filinsb(underlayVerticalRect[start].copnt);
			const auto delta  = fPOINT { underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x,
                                        underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y };
			const auto length = hypot(delta.x, delta.y);
			if (length > form.edgeStitchLen) {
				const auto angle = atan2((*OutsidePoints)[finish].y - (*InsidePoints)[finish].y,
				                         (*OutsidePoints)[finish].x - (*InsidePoints)[finish].x);
				const auto point = fPOINT { underlayVerticalRect[finish].bipnt.x + cos(angle) * width,
					                        underlayVerticalRect[finish].bipnt.y + sin(angle) * width };
				form::filinsb(point);
			}
			form::filinsb(underlayVerticalRect[finish].bopnt);
			form::filinsb(underlayVerticalRect[finish].bipnt);
		}
	}
	else {
		spend(fillVerticalRect, start, finish);
	}
}

void form::internal::pfn(const std::vector<VRCT2>& underlayVerticalRect,
                         const std::vector<VRCT2>& fillVerticalRect,
                         uint32_t                  startVertex,
                         const std::vector<VRCT2>& vrct,
                         float                     width) {
	auto currentVertex = startVertex;
	auto nextVertex    = form::nxt(currentVertex);

	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	const auto& form = FormList->operator[](ClosestFormToCursor);
	SelectedPoint = vertexIt[startVertex];
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		duromb(vrct[currentVertex].bipnt, vrct[currentVertex].cipnt, vrct[currentVertex].bopnt, vrct[currentVertex].copnt);
		duspnd(form, underlayVerticalRect, fillVerticalRect, currentVertex, nextVertex, width);
		currentVertex = nextVertex;
		nextVertex    = form::nxt(nextVertex);
	}
}

void form::internal::prsmal(float width) {
	auto iOutput       = 0U;
	auto minimumLength = USPAC * 0.8F;

	if (minimumLength > width) {
		minimumLength = width * 0.9F;
	}
	auto iReference = 0U;
	for (auto iSequence = 1U; iSequence < wrap::toUnsigned(OSequence->size()); iSequence++) {
		const auto delta  = fPOINT { (*OSequence)[iSequence].x - (*OSequence)[iReference].x,
                                    (*OSequence)[iSequence].y - (*OSequence)[iReference].y };
		const auto length = hypot(delta.x, delta.y);
		if (length > minimumLength) {
			(*OSequence)[iOutput++] = (*OSequence)[iSequence];
			iReference              = iSequence;
		}
	}
	OSequence->resize(iOutput);
}

void form::internal::pbrd(const FRMHED& form) {
	const auto spacing = LineSpacing;
	const auto start   = form::getlast(form);

	LineSpacing = form.edgeSpacing;
	OSequence->clear();
	auto fillVerticalRect = std::vector<VRCT2> {};
	fillVerticalRect.resize(VertexCount);
	auto underlayVerticalRect = std::vector<VRCT2> {};
	underlayVerticalRect.resize(VertexCount);
	satin::satout(form, form.borderSize);
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1U);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, (VertexCount - 1), 0);
	spurct(underlayVerticalRect, fillVerticalRect, (VertexCount - 1));
	if ((form.edgeType & EGUND) != 0U) {
		StateMap.reset(StateFlag::SAT1);
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		const auto width = form.borderSize * URAT;
		satin::satout(form, width);
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.set(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
		StateMap.reset(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
		LineSpacing = form.edgeSpacing;
		prsmal(width);
		StateMap.reset(StateFlag::UND);
	}
	pfn(underlayVerticalRect, fillVerticalRect, start, fillVerticalRect, form.borderSize);
	LineSpacing = spacing;
}

void form::internal::apbrd(uint32_t vertexIndex) {
	auto currentVertex = 0U;

	OSequence->clear();
	auto vertexIt = std::next(FormVertices->cbegin(), vertexIndex);
	OSequence->push_back(vertexIt[currentVertex]);
	for (auto iVertex = 0U; iVertex < VertexCount * 2U; iVertex++) {
		const auto nextVertex = form::nxt(currentVertex);
		bdrlin(vertexIndex, currentVertex, nextVertex, IniFile.AppStitchLen);
		currentVertex = nextVertex;
	}
}

void form::internal::bhfn(const FRMHED& form, uint32_t start, uint32_t finish) {
	auto       vertexIt   = std::next(FormVertices->cbegin(), form.vertexIndex);
	const auto delta      = fPOINT { (vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y) };
	const auto length     = hypot(delta.x, delta.y);
	const auto count      = wrap::round<uint32_t>(length / form.edgeSpacing);
	const auto step       = fPOINT { (delta.x / count), (delta.y / count) };
	auto       innerPoint = fPOINT { vertexIt[start].x, vertexIt[start].y };

	const auto rotationAngle = atan2(-delta.x, delta.y);
	const auto outerStep
	    = fPOINT { form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle) };
	if (count != 0U) {
		for (auto iStep = 0U; iStep < count - 1U; iStep++) {
			const auto firstPoint = fPOINT { innerPoint.x + step.x, innerPoint.y + step.y };
			const auto outerPoint = fPOINT { firstPoint.x + outerStep.x, firstPoint.y + outerStep.y };
			OSequence->push_back(fPOINT { firstPoint });
			OSequence->push_back(fPOINT { innerPoint });
			OSequence->push_back(fPOINT { firstPoint });
			OSequence->push_back(fPOINT { outerPoint });
			OSequence->push_back(fPOINT { firstPoint });
			OSequence->push_back(fPOINT { outerPoint });
			OSequence->push_back(fPOINT { firstPoint });
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		const auto firstPoint = fPOINT { innerPoint.x + step.x, innerPoint.y + step.y };
		OSequence->push_back(fPOINT { firstPoint });
		OSequence->push_back(fPOINT { innerPoint });
	}
}

void form::internal::bhcrnr(uint32_t vertex) {
	const auto nextVertex = form::nxt(vertex);
	auto       delta      = fPOINT {};

	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	if (StateMap.test(StateFlag::INDIR)) {
		delta.x = (*OutsidePoints)[nextVertex].x - vertexIt[nextVertex].x;
		delta.y = (*OutsidePoints)[nextVertex].y - vertexIt[nextVertex].y;
	}
	else {
		delta.x = (*InsidePoints)[nextVertex].x - vertexIt[nextVertex].x;
		delta.y = (*InsidePoints)[nextVertex].y - vertexIt[nextVertex].y;
	}
	const auto length = hypot(delta.x, delta.y);
	const auto ratio  = ButtonholeCornerLength / length;
	delta.x *= ratio;
	delta.y *= ratio;
	const auto point = fPOINT { vertexIt[nextVertex].x + delta.x, vertexIt[nextVertex].y + delta.y };
	OSequence->push_back(vertexIt[nextVertex]);
	OSequence->push_back(fPOINT { point });
	OSequence->push_back(vertexIt[nextVertex]);
	OSequence->push_back(fPOINT { point });
	OSequence->push_back(vertexIt[nextVertex]);
}

void form::internal::bhbrd(const FRMHED& form) {
	auto vertex = form::getlast(form);

	OSequence->clear();
	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	OSequence->push_back(vertexIt[vertex]);
	for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
		const auto nextVertex = form::nxt(vertex);
		bhfn(form, vertex, nextVertex);
		bhcrnr(vertex);
		vertex = nextVertex;
	}
}

void form::internal::dubfn(const FRMHED& currentForm) {
	brdfil(currentForm);
	for (auto iBackward = OSequence->size(); iBackward != 0; iBackward--) {
		OSequence->push_back((*OSequence)[iBackward - 1U]);
	}
}

void form::internal::chkbrd(const FRMHED& form) {
	form::fvars(ClosestFormToCursor);
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
			auto clipRect = fRECTANGLE{};
			clip::oclp(clipRect, form.borderClipData, form.clipEntries);
			clip::clpbrd(form, clipRect, form::getlast(form));
			break;
		}
		case EDGECLIPX: { // Even Clipboard
			auto clipRect = fRECTANGLE{};
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
			apbrd(form.vertexIndex);
			ritapbrd();
			satin::sbrd(form);
			break;
		}
		case EDGEBHOL: { // BH Buttonhole
			const auto length = ButtonholeCornerLength;
			ButtonholeCornerLength = form::getblen();
			satin::satout(form, 20);
			bhbrd(form);
			ButtonholeCornerLength = length;
			break;
		}
		case EDGEPICOT: { // Picot
			auto clipRect = fRECTANGLE{};
			clip::oclp(clipRect, form.borderClipData, form.clipEntries);
			clip::clpic(form, clipRect);
			break;
		}
		case EDGEDOUBLE: { // Double
			dubfn(form);
			break;
		}
		case EDGELCHAIN: { // Lin Chain
			StateMap.set(StateFlag::LINCHN);
			clip::chnfn(form);
			break;
		}
		case EDGEOCHAIN: { // Open Chain
			StateMap.reset(StateFlag::LINCHN);
			clip::chnfn(form);
			break;
		}
		}
		ritbrd(form);
	}
}

void form::internal::fnvrt(std::vector<fPOINT>&   currentFillVertices,
                           std::vector<uint32_t>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints) {
	auto highX  = currentFillVertices[0].x;
	auto lowX   = currentFillVertices[0].x;
	VertexCount = gsl::narrow<decltype(VertexCount)>(currentFillVertices.size());
	for (auto fillVertex = std::next(currentFillVertices.cbegin(), 1); fillVertex < currentFillVertices.end(); fillVertex++) {
		if (fillVertex->x > highX) {
			highX = fillVertex->x;
		}
		if (fillVertex->x < lowX) {
			lowX = fillVertex->x;
		}
	}

	auto lineOffset            = wrap::floor<int32_t>(lowX / LineSpacing);
	lowX                       = LineSpacing * lineOffset;
	auto       fillLineCount   = wrap::floor<uint32_t>((gsl::narrow_cast<double>(highX) - lowX) / LineSpacing + 1.0F);
	const auto step            = (highX - lowX) / fillLineCount;
	auto       currentX        = lowX;
	auto       projectedPoints = std::vector<fPOINTLINE> {};
	projectedPoints.reserve(wrap::toSize(VertexCount) + 2U);
	for (auto iLine = 0U; iLine < fillLineCount; iLine++) {
		auto iLineCounter = 0U;
		currentX += step;
		for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
			const auto iNextVertex = (iVertex + 1U) % VertexCount;
			auto       point       = fPOINT {};
			if (projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
				iLineCounter++;
			}
		}
		fillLineCount += iLineCounter;
	}
	lineEndpoints.reserve(wrap::toSize(fillLineCount) + 1U);
	auto lineGroupIndex = 0U;
	// groupIndex cannot be more than fillLineCount so reserve that amount of memory to reduce re-allocations
	groupIndexSequence.reserve(fillLineCount);
	currentX = lowX;
	for (auto iLine = 0U; iLine < fillLineCount; iLine++) {
		projectedPoints.clear();
		projectedPoints.reserve(VertexCount);
		currentX += step;
		auto iPoint = 0U;
		for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
			const auto iNextVertex = (iVertex + 1U) % VertexCount;
			auto       point       = fPOINT {};
			if (projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
				const auto a = fPOINTLINE { point.x, point.y, gsl::narrow<uint16_t>(iVertex) };
				projectedPoints.push_back(a);
				iPoint++;
			}
		}
		if (iPoint > 1) {
			const auto evenPointCount = iPoint &= 0xfffffffe;
			groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
			std::sort(projectedPoints.begin(), projectedPoints.end(), fi::fplComp);
			iPoint                    = 0;
			const auto savedLineCount = lineEndpoints.size();
			while (iPoint < evenPointCount) {
				if (lineEndpoints.size() < fillLineCount) {
					lineEndpoints.push_back(
					    { projectedPoints[iPoint].line, lineGroupIndex, projectedPoints[iPoint].x, projectedPoints[iPoint].y });
					iPoint++;
					lineEndpoints.push_back(
					    { projectedPoints[iPoint].line, lineGroupIndex, projectedPoints[iPoint].x, projectedPoints[iPoint].y });
					iPoint++;
				}
			}
			if (lineEndpoints.size() != savedLineCount) {
				lineGroupIndex++;
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
	const auto& angRect = angledForm.rectangle;
	rotationCenter      = fPOINT { form::midl(angRect.right, angRect.left), form::midl(angRect.top, angRect.bottom) };
	angledFormVertices.clear();
	angledFormVertices.reserve(angledForm.vertexCount);
	auto vertexIt = std::next(FormVertices->cbegin(), angledForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; iVertex++) {
		angledFormVertices.push_back(vertexIt[iVertex]);
		thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	}
	angledForm.vertexIndex = 0;
	fi::fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void form::internal::fnhor(std::vector<uint32_t>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints,
                           const float            rotationAngle,
                           fPOINT&                rotationCenter,
                           FRMHED&                angledForm,
                           std::vector<fPOINT>&   angledFormVertices) {
	angledForm = FormList->operator[](ClosestFormToCursor);
	const auto& angRect = angledForm.rectangle;
	rotationCenter      = fPOINT { form::midl(angRect.right, angRect.left), form::midl(angRect.top, angRect.bottom) };
	angledFormVertices.clear();
	angledFormVertices.reserve(angledForm.vertexCount);
	auto vertexIt = std::next(FormVertices->cbegin(), angledForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; iVertex++) {
		angledFormVertices.push_back(vertexIt[iVertex]);
		thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	}
	angledForm.vertexIndex = 0;
	fi::fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void form::internal::prebrd(FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices) {
	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	auto delta    = fPOINT { (vertexIt[1].x - vertexIt[0].x), (vertexIt[1].y - vertexIt[0].y) };
	auto ratio    = 0.0F;
	angledFormVertices.resize(wrap::toSize(VertexCount) + 3U);
	auto output = angledFormVertices.begin();
	output++;
	std::copy(vertexIt, vertexIt + VertexCount, output);
	if (fabs(delta.x) > fabs(delta.y)) {
		ratio = fabs(0.1F / delta.x);
	}
	else {
		ratio = fabs(0.1F / delta.y);
	}
	angledFormVertices[0].x = vertexIt[0].x - delta.x * ratio;
	angledFormVertices[0].y = vertexIt[0].y - delta.y * ratio;
	angledForm              = FormList->operator[](ClosestFormToCursor);
	angledForm.vertexIndex  = 0;
	angledForm.vertexCount += 3;
	delta.x = vertexIt[VertexCount - 1U].x - vertexIt[VertexCount - 2U].x;
	delta.y = vertexIt[VertexCount - 1U].y - vertexIt[VertexCount - 2U].y;
	if (fabs(delta.x) > fabs(delta.y)) {
		ratio = fabs(0.1 / delta.x);
	}
	else {
		ratio = fabs(0.1F / delta.y);
	}
	angledFormVertices[angledForm.vertexCount - 1U].x = vertexIt[VertexCount - 1U].x + delta.x * ratio;
	angledFormVertices[angledForm.vertexCount - 1U].y = vertexIt[VertexCount - 1U].y + delta.y * ratio;
	VertexCount                                       = angledForm.vertexCount;
	CurrentVertexIndex                                = angledForm.vertexIndex;
}

void form::internal::plfn(const FRMHED&             form,
                          const std::vector<VRCT2>& underlayVerticalRect,
                          const std::vector<VRCT2>& fillVerticalRect,
                          const std::vector<VRCT2>& prct,
                          float                     width) {
	duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt);
	duspnd(form, underlayVerticalRect, fillVerticalRect, 1, 2, width);
	for (auto iVertex = 2U; iVertex < VertexCount - 4U; iVertex++) {
		duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt);
		duspnd(form, underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1U, width);
	}
	duromb(
	    prct[VertexCount - 4U].bipnt, prct[VertexCount - 4U].dipnt, prct[VertexCount - 4U].bopnt, prct[VertexCount - 4U].dopnt);
}

void form::internal::plbak(uint32_t backPoint) {
	if ((!OSequence->empty()) && (backPoint < (OSequence->size() - 1U))) {
		auto iSequence = wrap::toUnsigned(OSequence->size() - 1U);
		while (iSequence > backPoint) {
			std::swap((*OSequence)[iSequence], (*OSequence)[backPoint]);
			iSequence--;
			backPoint++;
		}
	}
}

void form::internal::plbrd(float edgeSpacing, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices) {
	// Ensure that we have at least 4 array members
	auto fillVerticalRect = std::vector<VRCT2> {};
	fillVerticalRect.resize(wrap::toSize(VertexCount) + 3U);
	auto underlayVerticalRect = std::vector<VRCT2> {};
	underlayVerticalRect.resize(wrap::toSize(VertexCount) + 3U);
	prebrd(angledForm, angledFormVertices);
	satin::satout(angledForm, angledForm.borderSize);
	InsidePoints->push_back(InsidePoints->front());
	OutsidePoints->push_back(OutsidePoints->front());
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1U);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, (VertexCount - 1), 0);
	spurct(underlayVerticalRect, fillVerticalRect, (VertexCount - 1));
	auto vertexIt = std::next(angledFormVertices.cbegin(), angledForm.vertexIndex);
	if ((angledForm.attribute & SBLNT) == 0U) {
		const auto& val = vertexIt[1];

		fillVerticalRect[1].aipnt     = val;
		fillVerticalRect[1].aopnt     = val;
		underlayVerticalRect[1].aipnt = val;
		underlayVerticalRect[1].aopnt = val;
	}
	if ((angledForm.attribute & FBLNT) == 0U) {
		const auto& val = vertexIt[VertexCount - 1U];

		fillVerticalRect[VertexCount - 4U].dipnt     = val;
		fillVerticalRect[VertexCount - 4U].dopnt     = val;
		underlayVerticalRect[VertexCount - 4U].dipnt = val;
		underlayVerticalRect[VertexCount - 4U].dopnt = val;
	}
	OSequence->clear();
	SelectedPoint = vertexIt[0];
	if ((angledForm.edgeType & EGUND) != 0U) {
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		const auto width = angledForm.borderSize * URAT;
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width);
		const auto savedIndex = wrap::toUnsigned(OSequence->size());
		StateMap.reset(StateFlag::UNDPHAS);
		SelectedPoint = vertexIt[0];
		StateMap.set(StateFlag::FILDIR);
		plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width);
		plbak(savedIndex);
		prsmal(width);
		if (!OSequence->empty()) { // ensure that we can do a valid read from OSequence
			SelectedPoint = OSequence->back();
		}
	}
	StateMap.reset(StateFlag::UND);
	LineSpacing = angledForm.edgeSpacing;
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, fillVerticalRect, angledForm.borderSize);
	LineSpacing = edgeSpacing;
	form::fvars(ClosestFormToCursor);
}

void form::internal::lapbrd(uint32_t vertexIndex) {
	const auto savedStitchLength = UserStitchLength;

	OSequence->clear();
	UserStitchLength = IniFile.AppStitchLen;
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		bdrlin(vertexIndex, iVertex, iVertex + 1U, IniFile.AppStitchLen);
	}
	for (auto iVertex = VertexCount - 1U; iVertex != 0U; iVertex--) {
		bdrlin(vertexIndex, iVertex, iVertex - 1U, IniFile.AppStitchLen);
	}
	UserStitchLength = savedStitchLength;
}

void form::internal::blbrd(const FRMHED& form) {
	OSequence->clear();
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	OSequence->push_back(vertexIt[0]);
	for (auto iVertex = 0U; iVertex < VertexCount - 2U; iVertex++) {
		bhfn(form, iVertex, iVertex + 1U);
		bhcrnr(iVertex);
	}
	bhfn(form, VertexCount - 2U, VertexCount - 1U);
	OSequence->push_back(vertexIt[VertexCount - 1U]);
}

void form::internal::contf() {
	auto& form                 = FormList->operator[](ClosestFormToCursor);
	const auto start           = form.angleOrClipData.guide.start;
	auto       vertexIt        = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	auto&      selectionStart  = vertexIt[start];
	const auto finish          = form.angleOrClipData.guide.finish;
	const auto lowVertexIndex  = start;
	const auto highVertexIndex = VertexCount - start - 1U;

	auto lowCounts = std::vector<uint32_t> {};
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
	for (auto iVertex = lowVertexIndex; iVertex != 0; iVertex--) {
		lowVertices[lowIndex] = vertexIt[iVertex];
		lowDeltas[lowIndex]  = fPOINT { vertexIt[iVertex - 1U].x - vertexIt[iVertex].x, vertexIt[iVertex - 1U].y - vertexIt[iVertex].y };
		lowLengths[lowIndex]  = hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
		lowLength += lowLengths[lowIndex];
		lowIndex++;
	}
	auto       selind              = 0U;
	const auto selectedVertexCount = finish - start;
	auto       polyLines           = std::vector<PVEC> {};
	polyLines.resize(selectedVertexCount);
	for (auto iVertex = start + 1U; iVertex <= finish; iVertex++) {
		const auto delta  = fPOINT { vertexIt[iVertex].x - selectionStart.x, vertexIt[iVertex].y - selectionStart.y };
		polyLines[selind] = { atan2(delta.y, delta.x), hypot(delta.x, delta.y) };
		selind++;
	}
	auto highIndex  = 0U;
	auto highLength = 0.0F;
	for (auto iVertex = finish; iVertex < VertexCount - 1U; iVertex++) {
		auto& thisVertex          = vertexIt[iVertex];
		auto& nextVertex          = vertexIt[wrap::toSize(iVertex) + 1U];
		highVertices[highIndex].x = thisVertex.x;
		highVertices[highIndex].y = thisVertex.y;
		highDeltas[highIndex].x   = nextVertex.x - thisVertex.x;
		highDeltas[highIndex].y   = nextVertex.y - thisVertex.y;
		highLengths[highIndex]    = hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
		highLength += highLengths[highIndex];
		highIndex++;
	}
	auto highSpacing = form.fillSpacing;
	auto lowSpacing  = form.fillSpacing * lowLength / highLength;
	if (highLength < lowLength) {
		std::swap(highSpacing, lowSpacing);
	}
	for (auto iVertex = 0U; iVertex < lowVertexIndex; iVertex++) {
		lowCounts[iVertex]  = wrap::round<uint32_t>(lowLengths[iVertex] / lowSpacing);
		lowSteps[iVertex].x = lowDeltas[iVertex].x / lowCounts[iVertex];
		lowSteps[iVertex].y = lowDeltas[iVertex].y / lowCounts[iVertex];
	}
	for (auto iVertex = 0U; iVertex < highVertexIndex; iVertex++) {
		highCounts[iVertex]  = wrap::round<uint32_t>(highLengths[iVertex] / highSpacing);
		highSteps[iVertex].x = highDeltas[iVertex].x / highCounts[iVertex];
		highSteps[iVertex].y = highDeltas[iVertex].y / highCounts[iVertex];
	}
	lowIndex = highIndex = 0;
	StateMap.reset(StateFlag::FILDIR);
	auto       lowCount  = 0U;
	auto       highCount = 0U;
	auto       delta     = fPOINT { vertexIt[finish].x - vertexIt[start].x, vertexIt[finish].y - vertexIt[start].y };
	const auto reference = PVEC { atan2(delta.y, delta.x), hypot(delta.x, delta.y) };
	auto       lowStep   = fPOINT {};
	auto       lowPoint  = fPOINT {};
	auto       highStep  = fPOINT {};
	auto       highPoint = fPOINT {};
	while ((lowCount != 0U) || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
		if (lowCount != 0U) {
			lowCount--;
		}
		else {
			if (lowIndex < lowVertexIndex) {
				lowCount = lowCounts[lowIndex];
				lowStep  = lowSteps[lowIndex];
				lowPoint = lowVertices[lowIndex];
				lowIndex++;
			}
		}
		if (highCount != 0U) {
			highCount--;
		}
		else {
			if (highIndex < highVertexIndex) {
				highCount = highCounts[highIndex];
				highStep  = highSteps[highIndex];
				highPoint = highVertices[highIndex];
				highIndex++;
			}
		}
		delta.x = highPoint.x - lowPoint.x;
		delta.y = highPoint.y - lowPoint.y;
		if (reference.length > 0.9 * LineSpacing) {
			const auto polyLine = PVEC { atan2(delta.y, delta.x), hypot(delta.x, delta.y) };
			const auto polyDiff = PVEC { polyLine.angle - reference.angle, polyLine.length / reference.length };
			if (StateMap.testAndFlip(StateFlag::FILDIR)) {
				OSequence->push_back(fPOINT { lowPoint });
				for (auto iVertex = 0U; iVertex < (selectedVertexCount - 1); iVertex++) {
					const auto length = polyLines[iVertex].length * polyDiff.length;
					const auto angle  = polyLines[iVertex].angle + polyDiff.angle;
					OSequence->push_back(fPOINT { lowPoint.x + cos(angle) * length, lowPoint.y + sin(angle) * length });
				}
			}
			else {
				OSequence->push_back(fPOINT { highPoint });
				for (auto iVertex = selectedVertexCount - 1U; iVertex != 0; iVertex--) {
					const auto length = polyLines[iVertex - 1U].length * polyDiff.length;
					const auto angle  = polyLines[iVertex - 1U].angle + polyDiff.angle;
					OSequence->push_back(fPOINT { lowPoint.x + cos(angle) * length, lowPoint.y + sin(angle) * length });
				}
			}
		}
		lowPoint.x += lowStep.x;
		lowPoint.y += lowStep.y;
		highPoint.x += highStep.x;
		highPoint.y += highStep.y;
	}
	if (StateMap.test(StateFlag::FILDIR)) {
		OSequence->push_back(vertexIt[0]);
	}
	else {
		OSequence->push_back(vertexIt[VertexCount - 1U]);
	}
	if (form.lengthOrCount.stitchLength < MinStitchLength) {
		form.lengthOrCount.stitchLength = MinStitchLength;
	}
}

void form::internal::duflt(float& formOffset, std::vector<fPOINT>& currentFormVertices) {
	auto leftEdge = 1e9F;

	for (auto vertex : currentFormVertices) {
		if (vertex.x < leftEdge) {
			leftEdge = vertex.x;
		}
	}
	if (leftEdge < ClipRectSize.cx) {
		StateMap.set(StateFlag::WASNEG);
		formOffset = ClipRectSize.cx + fabs(leftEdge) + .01F;
		for (auto& vertex : currentFormVertices) {
			vertex.x += formOffset;
		}
		auto& form = FormList->operator[](ClosestFormToCursor);
		form.rectangle.left += formOffset;
		form.rectangle.right += formOffset;
	}
	else {
		StateMap.reset(StateFlag::WASNEG);
	}
}

auto form::internal::leftsid(const std::vector<fPOINT>& currentFormVertices) -> uint32_t {
	auto leftVertex = 0U;
	auto iVertex    = 0U;
	auto minimumX   = 1e9F;

	auto vertexIt = currentFormVertices.cbegin();
	for (iVertex = 0; iVertex < wrap::toUnsigned(currentFormVertices.size()); iVertex++) {
		if (vertexIt[iVertex].x < minimumX) {
			minimumX   = vertexIt[iVertex].x;
			leftVertex = iVertex;
		}
	}
	return leftVertex;
}

auto form::internal::vclpComp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept -> bool {
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

auto form::internal::isin(std::vector<VCLPX>&        regionCrossingData,
                          float                      xCoordinate,
                          float                      yCoordinate,
                          uint32_t                   regionCrossingStart,
                          uint32_t                   regionCrossingEnd,
                          const fRECTANGLE&          boundingRect,
                          const std::vector<fPOINT>& currentFormVertices) -> bool {
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
	auto vertexIt = currentFormVertices.cbegin();
	for (auto iRegion = regionCrossingStart; iRegion < regionCrossingEnd; iRegion++) {
		const auto startVertex = regionCrossingData[iRegion].vertex;
		const auto endVertex   = form::nxt(startVertex);
		if (projv(xCoordinate, vertexIt[startVertex], vertexIt[endVertex], point)) {
			if (point.y > yCoordinate) {
				if (vertexIt[startVertex].x != xCoordinate && vertexIt[endVertex].x != xCoordinate) {
					count++;
				}
				else {
					if (vertexIt[startVertex].x < vertexIt[endVertex].x) {
						if (vertexIt[endVertex].x != xCoordinate) {
							count++;
						}
					}
					else {
						if (vertexIt[startVertex].x != xCoordinate) {
							count++;
						}
					}
				}
			}
		}
	}
	return (count & 1U) != 0U;
}

void form::internal::inspnt(std::vector<CLIPNT>& clipStitchPoints) {
	CLIPNT clipStitchPoint { clipStitchPoints.back().x, clipStitchPoints.back().y, 0, 0 };
	clipStitchPoints.back().x    = form::midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2U].x);
	clipStitchPoints.back().y    = form::midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2U].y);
	clipStitchPoints.back().flag = 1;
	clipStitchPoints.push_back(clipStitchPoint);
}

auto form::internal::isect(uint32_t                   vertex0,
                           uint32_t                   vertex1,
                           fPOINT&                    intersection,
                           float&                     length,
                           const fPOINT&              lineSegmentStart,
                           const fPOINT&              lineSegmentEnd,
                           const std::vector<fPOINT>& currentFormVertices) -> bool {
	const auto delta            = fPOINT { (lineSegmentEnd.x - lineSegmentStart.x), (lineSegmentEnd.y - lineSegmentStart.y) };
	const auto point            = lineSegmentStart;
	auto       tempIntersection = fPOINT {};
	auto       flag             = false;
	auto       left             = 0.0F;
	auto       right            = 0.0F;

	auto vertexIt = currentFormVertices.cbegin();
	if ((delta.x != 0.0F) && (delta.y != 0.0F)) {
		flag = proj(point, delta.y / delta.x, vertexIt[vertex0], vertexIt[vertex1], tempIntersection);
	}
	else {
		if (delta.y != 0.0F) {
			flag = projv(point.x, vertexIt[vertex0], vertexIt[vertex1], tempIntersection);
		}
		else {
			if (delta.x != 0.0F) {
				flag = projh(point.y, vertexIt[vertex0], vertexIt[vertex1], tempIntersection);
			}
			else if (vertexIt[vertex0].y == lineSegmentStart.y && vertexIt[vertex1].y == lineSegmentStart.y) {
				if (vertexIt[vertex0].x < vertexIt[vertex1].x) {
					left  = vertexIt[vertex0].x;
					right = vertexIt[vertex1].x;
				}
				else {
					left  = vertexIt[vertex1].x;
					right = vertexIt[vertex0].x;
				}
				if (lineSegmentStart.x > left && lineSegmentStart.x < right) {
					intersection.x = lineSegmentStart.x;
					intersection.y = lineSegmentStart.y;
					length         = 0.0F;
					return true;
				}
				return false;
			}
			else {
				return false;
			}
		}
	}
	if (tempIntersection.x < TINY) {
		tempIntersection.x = 0.0F;
	}
	if (tempIntersection.y < TINY) {
		tempIntersection.y = 0.0F;
	}
	intersection.x = gsl::narrow_cast<float>(tempIntersection.x);
	intersection.y = gsl::narrow_cast<float>(tempIntersection.y);
	length         = hypot(tempIntersection.x - lineSegmentStart.x, tempIntersection.y - lineSegmentStart.y);
	// ToDo - should length be determined from start or end?
	//	 hypot(tempIntersection.x-lineSegmentEnd.x,tempIntersection.y-lineSegmentEnd.y);
	return flag;
}

auto form::internal::clipComp(const CLIPSORT* const arg1, const CLIPSORT* const arg2) noexcept -> bool {
	// make sure the comparison obeys strict weak ordering for stable sorting
	if ((arg1 != nullptr) && (arg2 != nullptr)) {
		const auto local1 = arg1->segmentLength;
		const auto local2 = arg2->segmentLength;
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

auto form::internal::insect(std::vector<CLIPSORT>&     clipIntersectData,
                            const std::vector<VCLPX>&  regionCrossingData,
                            std::vector<CLIPSORT*>&    arrayOfClipIntersectData,
                            uint32_t                   regionCrossingStart,
                            uint32_t                   regionCrossingEnd,
                            const fPOINT&              lineSegmentStart,
                            const fPOINT&              lineSegmentEnd,
                            const std::vector<fPOINT>& currentFormVertices) -> uint32_t {
	auto  iRegions        = 0U;
	auto  iIntersection   = 0U;
	auto  count           = 0U;
	auto  lineSegmentRect = fRECTANGLE {};
	auto* intersection    = gsl::narrow_cast<fPOINT*>(nullptr);

	if (lineSegmentEnd.x > lineSegmentStart.x) {
		lineSegmentRect.left  = lineSegmentStart.x;
		lineSegmentRect.right = lineSegmentEnd.x;
	}
	else {
		lineSegmentRect.left  = lineSegmentEnd.x;
		lineSegmentRect.right = lineSegmentStart.x;
	}
	if (lineSegmentEnd.y > lineSegmentStart.y) {
		lineSegmentRect.top    = lineSegmentEnd.y;
		lineSegmentRect.bottom = lineSegmentStart.y;
	}
	else {
		lineSegmentRect.top    = lineSegmentStart.y;
		lineSegmentRect.bottom = lineSegmentEnd.y;
	}
	iIntersection = count = 0;
	arrayOfClipIntersectData.clear();
	for (iRegions = regionCrossingStart; iRegions < regionCrossingEnd; iRegions++) {
		const auto currentVertex = regionCrossingData[iRegions].vertex;
		const auto nextVertex    = form::nxt(currentVertex);
		if (isect(currentVertex,
		          nextVertex,
		          clipIntersectData[iIntersection].point,
		          clipIntersectData[iIntersection].sideLength,
		          lineSegmentStart,
		          lineSegmentEnd,
		          currentFormVertices)) {
			intersection = &clipIntersectData[iIntersection].point;
			if (intersection->x >= lineSegmentRect.left && intersection->x <= lineSegmentRect.right
			    && intersection->y >= lineSegmentRect.bottom && intersection->y <= lineSegmentRect.top) {
				clipIntersectData[iIntersection].segmentLength
				    = hypot(clipIntersectData[iIntersection].point.x - lineSegmentStart.x,
				            clipIntersectData[iIntersection].point.y - lineSegmentStart.y);
				clipIntersectData[iIntersection].vertexIndex = currentVertex;
				arrayOfClipIntersectData.push_back(&clipIntersectData[iIntersection]);
				iIntersection++;
				count++;
			}
		}
	}
	if (count > 1) {
		std::sort(arrayOfClipIntersectData.begin(), arrayOfClipIntersectData.end(), fi::clipComp);
		auto iDestination = 1U;
		for (iIntersection = 0; iIntersection < count - 1U; iIntersection++) {
			if (fabs(gsl::narrow_cast<double>(arrayOfClipIntersectData[iIntersection]->segmentLength)
			         - arrayOfClipIntersectData[wrap::toSize(iIntersection) + 1U]->segmentLength)
			    > TINY) {
				mvpclp(arrayOfClipIntersectData, iDestination++, iIntersection + 1U);
			}
		}
		count = iDestination;
	}
	return count;
}

auto form::internal::getlen(std::vector<CLIPNT>&       clipStitchPoints,
                            const std::vector<double>& lengths,
                            uint32_t                   iPoint,
                            const std::vector<fPOINT>& currentFormVertices) -> float {
	auto retval = 0.0F;
	clipStitchPoints[iPoint].vertexIndex %= currentFormVertices.size();
	auto vertexIt = currentFormVertices.cbegin();
	retval        = lengths[clipStitchPoints[iPoint].vertexIndex]
	         + hypot(vertexIt[clipStitchPoints[iPoint].vertexIndex].x - clipStitchPoints[iPoint].x,
	                 vertexIt[clipStitchPoints[iPoint].vertexIndex].y - clipStitchPoints[iPoint].y);
	return retval;
}

auto form::internal::clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
                             std::vector<CLPSEG>&       clipSegments,
                             const std::vector<double>& lengths,
                             uint32_t                   start,
                             uint32_t                   finish,
                             const std::vector<fPOINT>& currentFormVertices) -> uint32_t {
	const auto clipSegment = CLPSEG { start,  getlen(clipStitchPoints, lengths, start, currentFormVertices),
		                              0U,     clipStitchPoints[start].vertexIndex,
		                              finish, getlen(clipStitchPoints, lengths, finish, currentFormVertices),
		                              0U,     clipStitchPoints[finish].vertexIndex,
		                              0 };
	clipSegments.push_back(clipSegment);
	return finish + 1U;
}

auto form::internal::lenComp(const LENINFO& arg1, const LENINFO& arg2) noexcept -> bool {
	// make sure the comparison obeys strict weak ordering for stable sorting
	if (arg1.length < arg2.length) {
		return true;
	}
	if (arg2.length < arg1.length) {
		return false;
	}

	return false;
}

void form::internal::chksid(uint32_t vertexIndex, uint32_t clipIntersectSide, const std::vector<fPOINT>& currentFormVertices) {
	if (clipIntersectSide != vertexIndex) {
		auto       vertexIt    = currentFormVertices.cbegin();
		const auto vertexCount = currentFormVertices.size();
		if ((vertexIndex - clipIntersectSide + vertexCount) % vertexCount < (vertexCount / 2)) {
			auto       iVertex = form::nxt(clipIntersectSide);
			const auto limit   = form::nxt(vertexIndex);
			while (iVertex != limit) {
				OSequence->push_back(vertexIt[iVertex]);
				iVertex = form::nxt(iVertex);
			}
		}
		else {
			auto iVertex = clipIntersectSide;
			while (iVertex != vertexIndex) {
				OSequence->push_back(vertexIt[iVertex]);
				iVertex = form::prv(iVertex);
			}
		}
	}
}

void form::internal::ritseg(const std::vector<CLIPNT>& clipStitchPoints,
                            std::vector<CLPSEG>&       clipSegments,
                            const uint32_t             currentSegmentIndex,
                            uint32_t&                  clipIntersectSide,
                            const std::vector<fPOINT>& currentFormVertices) {
	auto isPointedEnd = true;

	const auto& form = FormList->operator[](ClosestFormToCursor);
	if ((form.extendedAttribute & AT_SQR) != 0U) {
		isPointedEnd = false;
	}
	if (StateMap.test(StateFlag::FILDIR)) {
		auto iPoint = clipSegments[currentSegmentIndex].start;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd) {
			iPoint++;
		}
		chksid(clipSegments[currentSegmentIndex].asid, clipIntersectSide, currentFormVertices);
		while (iPoint <= clipSegments[currentSegmentIndex].finish) {
			OSequence->push_back(fPOINT { clipStitchPoints[iPoint].x, clipStitchPoints[iPoint++].y });
		}
		clipIntersectSide = clipSegments[currentSegmentIndex].zsid;
	}
	else {
		auto iPoint = clipSegments[currentSegmentIndex].finish;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd) {
			iPoint--;
		}
		chksid(clipSegments[currentSegmentIndex].zsid, clipIntersectSide, currentFormVertices);
		if (clipSegments[currentSegmentIndex].start != 0U) {
			while (iPoint >= clipSegments[currentSegmentIndex].start) {
				OSequence->push_back(fPOINT { clipStitchPoints[iPoint].x, clipStitchPoints[iPoint--].y });
			}
		}
		else {
			while (iPoint < clipSegments[currentSegmentIndex].start) {
				OSequence->push_back(fPOINT { clipStitchPoints[iPoint].x, clipStitchPoints[iPoint--].y });
			}
		}
		clipIntersectSide = clipSegments[currentSegmentIndex].asid;
	}
	clipSegments[currentSegmentIndex].dun = 1;
}

auto form::internal::clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, uint32_t sind)
    -> bool {
	auto       index        = 1U;
	const auto indexDoubled = wrap::toUnsigned(clipSegments.size()) * 2U;

	StateMap.reset(StateFlag::FILDIR);
	while (index < clipSegments.size()) {
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			OutputIndex = (sind + index) % indexDoubled;
			if (clipSegments[sortedLengths[OutputIndex].index].dun == 0) {
				return false;
			}
			index++;
		}
		else {
			OutputIndex = (sind + indexDoubled - index) % indexDoubled;
			if (clipSegments[sortedLengths[OutputIndex].index].dun == 0) {
				return false;
			}
		}
	}
	return true;
}

auto form::internal::nucseg(const std::vector<CLPSEG>&  clipSegments,
                            const std::vector<LENINFO>& sortedLengths,
                            uint32_t&                   currentSegmentIndex) -> bool {
	auto index = 0U;

	if (StateMap.test(StateFlag::FILDIR)) {
		index = clipSegments[currentSegmentIndex].endIndex;
	}
	else {
		index = clipSegments[currentSegmentIndex].beginIndex;
	}
	if (clpnxt(clipSegments, sortedLengths, index)) {
		return false;
	}
	if (sortedLengths[OutputIndex].isEnd) {
		StateMap.reset(StateFlag::FILDIR);
	}
	else {
		StateMap.set(StateFlag::FILDIR);
	}
	currentSegmentIndex = sortedLengths[OutputIndex].index;
	return true;
}

auto form::internal::vscmp(uint32_t index1, uint32_t index2) noexcept -> bool {
	if ((*OSequence)[index1].x != (*OSequence)[index2].x) {
		return true;
	}

	return (*OSequence)[index1].y != (*OSequence)[index2].y;
}

void form::internal::clpcon(FRMHED& form, const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& currentFormVertices) {
	const auto currentVertexCount = currentFormVertices.size();
	auto       formOffset         = 0.0F;
	duflt(formOffset, currentFormVertices);
	auto clipWidth = ClipRectSize.cx + form.fillSpacing;
	if (StateMap.test(StateFlag::ISUND)) {
		clipWidth = form.underlaySpacing;
	}
	auto clipNegative = false;
	if (form.fillSpacing < 0) {
		clipNegative = true;
	}
	if (clipWidth < CLPMINAUT) {
		clipWidth = CLPMINAUT;
	}
	if (StateMap.test(StateFlag::TXFIL)) {
		if ((!TexturePointsBuffer->empty())
		    && form.fillInfo.texture.index + form.fillInfo.texture.count
		           <= gsl::narrow<uint16_t>(TexturePointsBuffer->size())) {
			clipWidth = form.fillSpacing;
		}
		else {
			return;
		}
	}
	auto lengths = std::vector<double> {};
	lengths.resize(currentVertexCount);
	auto clipSideLengths = std::vector<double> {}; // lengths of form sides for clipboard fill
	clipSideLengths.resize(currentVertexCount);
	auto clipIntersectData = std::vector<CLIPSORT> {}; // intersect points for clipboard fill
	clipIntersectData.resize(currentVertexCount);
	auto arrayOfClipIntersectData = std::vector<CLIPSORT*> {};
	arrayOfClipIntersectData.reserve(currentVertexCount);
	auto vertex      = leftsid(currentFormVertices);
	auto totalLength = 0.0;
	vertex           = form::nxt(vertex);
	auto vertexIt    = currentFormVertices.begin();
	for (auto iVertex = 0U; iVertex <= currentVertexCount; iVertex++) {
		const auto nextVertex   = form::nxt(vertex);
		lengths[vertex]         = totalLength;
		clipSideLengths[vertex] = hypot(vertexIt[nextVertex].x - vertexIt[vertex].x, vertexIt[nextVertex].y - vertexIt[vertex].y);
		totalLength += clipSideLengths[vertex];
		vertex = nextVertex;
	}
	auto boundingRect = fRECTANGLE { vertexIt[0].x, vertexIt[0].y, vertexIt[0].x, vertexIt[0].y };
	for (auto iVertex = 1U; iVertex < currentVertexCount; iVertex++) {
		if (vertexIt[iVertex].x < boundingRect.left) {
			boundingRect.left = vertexIt[iVertex].x;
		}
		if (vertexIt[iVertex].y > boundingRect.top) {
			boundingRect.top = vertexIt[iVertex].y;
		}
		if (vertexIt[iVertex].x > boundingRect.right) {
			boundingRect.right = vertexIt[iVertex].x;
		}
		if (vertexIt[iVertex].y < boundingRect.bottom) {
			boundingRect.bottom = vertexIt[iVertex].y;
		}
	}
	auto clipGrid = RECT { wrap::floor<int32_t>(boundingRect.left / clipWidth),
		                   wrap::ceil<int32_t>(boundingRect.top / ClipRectSize.cy + 1.0F) + 2,
		                   wrap::ceil<int32_t>(boundingRect.right / clipWidth),
		                   wrap::floor<int32_t>(boundingRect.bottom / ClipRectSize.cy - 1.0F) };

	auto negativeOffset = 0L;
	auto clipGridOffset = 0U;
	if (form.wordParam > 1) {
		clipGridOffset = form.wordParam;
	}
	if (clipGridOffset != 0U) {
		clipGrid.top++;
		if (form.fillSpacing < 0) {
			clipGrid.bottom--;
			clipGrid.left -= wrap::round<int32_t>(ClipRectSize.cx / clipWidth);
			clipGrid.right += wrap::round<int32_t>(ClipRectSize.cx / clipWidth);
		}
	}
	if (clipNegative && (clipGridOffset == 0U)) {
		clipGrid.left -= wrap::round<int32_t>(ClipRectSize.cx / clipWidth);
	}
	if (clipGrid.bottom < 0) {
		negativeOffset = 1 - clipGrid.bottom;
		clipGrid.bottom += negativeOffset;
		clipGrid.top += negativeOffset;
		const auto formNegativeOffset = ClipRectSize.cy * negativeOffset;
		for (auto iVertex = 0U; iVertex < currentVertexCount; iVertex++) {
			vertexIt[iVertex].y += formNegativeOffset;
		}
		boundingRect.top += formNegativeOffset;
		boundingRect.bottom += formNegativeOffset;
	}
	auto regionCrossingData = std::vector<VCLPX> {}; // region crossing data for vertical clipboard fills
	regionCrossingData.reserve(MAXFRMLINS);
	for (auto iVertex = 0U; iVertex < currentVertexCount; iVertex++) {
		auto start  = wrap::floor<uint32_t>(vertexIt[iVertex].x / clipWidth);
		auto finish = wrap::floor<uint32_t>(vertexIt[form::nxt(iVertex)].x / clipWidth);
		if (start > finish) {
			std::swap(start, finish);
		}
		if (form.fillSpacing < 0) {
			finish += wrap::round<decltype(finish)>(ClipRectSize.cx / clipWidth);
		}
		if (finish > gsl::narrow<uint32_t>(clipGrid.right)) {
			finish = gsl::narrow<uint32_t>(clipGrid.right);
		}
		if (clipNegative) {
			start -= wrap::round<uint32_t>(ClipRectSize.cx / clipWidth);
		}
		for (auto iSegment = start; iSegment <= finish; iSegment++) {
			regionCrossingData.push_back({ iSegment, iVertex });
		}
	}
	std::sort(regionCrossingData.begin(), regionCrossingData.end(), fi::vclpComp);
	auto iclpx = std::vector<uint32_t> {};
	iclpx.reserve(regionCrossingData.size());
	auto regionSegment = regionCrossingData[0].segment;
	iclpx.push_back(0);
	for (auto iSegment = 1U; iSegment < wrap::toUnsigned(regionCrossingData.size()); iSegment++) {
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
	auto texture       = gsl::narrow_cast<TXPNT*>(nullptr);
	auto iclpxSize     = wrap::toUnsigned(iclpx.size());
	if (iclpxSize != 0U) {
		iclpxSize--;
		bool breakFlag = false;
		for (auto iRegion = 0U; iRegion < iclpxSize; iRegion++) {
			auto regionCrossingStart = iclpx[iRegion];
			auto regionCrossingEnd   = iclpx[wrap::toSize(iRegion) + 1U];
			pasteLocation.x          = clipWidth * gsl::narrow_cast<float>(iRegion + clipGrid.left);
			auto  clipVerticalOffset = 0.0F;
			auto  lineSegmentStart   = fPOINT {}; // vertical clipboard line segment start
			auto  lineSegmentEnd     = fPOINT {}; // vertical clipboard line segment end
			auto& clipBuffer         = *ClipBuffer;
			auto  clipStitchCount    = wrap::toUnsigned(ClipBuffer->size());
			if (StateMap.test(StateFlag::TXFIL)) {
				const auto textureLine = (iRegion + clipGrid.left) % form.fillInfo.texture.lines;
				clipStitchCount        = textureSegments[textureLine].stitchCount;
				texture                = &TexturePointsBuffer->at(wrap::toSize(form.fillInfo.texture.index)
                                                   + textureSegments[textureLine].line);
				lineSegmentStart.x     = pasteLocation.x;
				if (form.txof != 0.0F) {
					const auto lineOffset = (iRegion + clipGrid.left) / form.fillInfo.texture.lines;
					clipVerticalOffset    = fmod(form.txof * lineOffset, form.fillInfo.texture.height);
				}
			}
			else {
				if (clipGridOffset != 0U) {
					clipVerticalOffset = gsl::narrow_cast<float>(iRegion % clipGridOffset) / (clipGridOffset * ClipRectSize.cy);
				}
				lineSegmentStart.x = pasteLocation.x + clipBuffer[0].x;
			}
			lineSegmentStart.y = clipGrid.bottom * ClipRectSize.cy;
			if (clipGridOffset != 0U) {
				clipVerticalOffset = gsl::narrow_cast<float>(iRegion % clipGridOffset) / (clipGridOffset * ClipRectSize.cy);
			}
			for (auto iVerticalGrid = clipGrid.bottom; iVerticalGrid < clipGrid.top; iVerticalGrid++) {
				pasteLocation.y = iVerticalGrid * ClipRectSize.cy - clipVerticalOffset;
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
				for (auto iStitch = 0U; iStitch < clipStitchCount; iStitch++) {
					if (StateMap.test(StateFlag::TXFIL)) {
						if (texture != nullptr) {
							lineSegmentEnd = fPOINT { pasteLocation.x, pasteLocation.y + texture[iStitch].y };
						}
					}
					else {
						lineSegmentEnd
						    = fPOINT { pasteLocation.x + clipBuffer[iStitch].x, pasteLocation.y + clipBuffer[iStitch].y };
					}

					clipStitchPoints.push_back({ lineSegmentStart.x, lineSegmentStart.y, 0, 0 });
					if (isin(regionCrossingData,
					         lineSegmentStart.x,
					         lineSegmentStart.y,
					         regionCrossingStart,
					         regionCrossingEnd,
					         boundingRect,
					         currentFormVertices)) {
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
					const auto count = insect(clipIntersectData,
					                          regionCrossingData,
					                          arrayOfClipIntersectData,
					                          regionCrossingStart,
					                          regionCrossingEnd,
					                          lineSegmentStart,
					                          lineSegmentEnd,
					                          currentFormVertices);
					if (count != 0U) {
						for (auto index = 0U; index < count; index++) {
							clipStitchPoints.push_back({ arrayOfClipIntersectData[index]->point.x,
							                             arrayOfClipIntersectData[index]->point.y,
							                             arrayOfClipIntersectData[index]->vertexIndex,
							                             1 });
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
				};
			}
			else {
				break;
			}
		}
	}
	if (!clipStitchPoints.empty()) {
		clipStitchPoints[clipStitchPoints.size() - 1U].flag = 2;
	};
	if (negativeOffset != 0) {
		const auto formNegativeOffset = negativeOffset * ClipRectSize.cy;
		for (auto& clipStitchPoint : clipStitchPoints) {
			clipStitchPoint.y -= formNegativeOffset;
		}
		for (auto iVertex = 0U; iVertex < currentVertexCount; iVertex++) {
			vertexIt[iVertex].y -= formNegativeOffset;
		}
	}

#if CLPVU == 0
	StateMap.reset(StateFlag::FILDIR);
	auto previousPoint = 0U;
	auto clipSegments  = std::vector<CLPSEG> {};
	auto endPoint      = wrap::toUnsigned(clipStitchPoints.size());
	if (endPoint != 0U) {
		endPoint--;
		// reserve a reasonable amount but not the full amount potentially required
		clipSegments.reserve(endPoint / 10U);
		for (auto iPoint = 0U; iPoint < endPoint; iPoint++) {
			switch (clipStitchPoints[iPoint].flag) {
			case 0: { // inside
				StateMap.set(StateFlag::FILDIR);
				break;
			}
			case 1: { // line
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					clpnseg(clipStitchPoints, clipSegments, lengths, previousPoint, iPoint, currentFormVertices);
				}
				else {
					previousPoint = iPoint;
				}
				break;
			}
			case 2: { // outside
				StateMap.reset(StateFlag::FILDIR);
				break;
			}
			}
		}
	}

#endif

	if (!clipSegments.empty()) {
		auto sortedLengths = std::vector<LENINFO> {};
		sortedLengths.reserve(clipSegments.size() * 2);
		for (auto iSegment = 0U; iSegment < wrap::toUnsigned(clipSegments.size()); iSegment++) {
			sortedLengths.push_back({ iSegment, false, clipSegments[iSegment].beginLength });
			sortedLengths.push_back({ iSegment, true, clipSegments[iSegment].endLength });
		}
		std::sort(sortedLengths.begin(), sortedLengths.end(), fi::lenComp);
		for (auto iSorted = 0U; iSorted < wrap::toUnsigned(sortedLengths.size()); iSorted++) {
			if (sortedLengths[iSorted].isEnd) {
				clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
			}
			else {
				clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
			}
		}

#if CLPVU == 1

		for (uint32_t iStitch = 0; iStitch < ActivePointIndex; iStitch++) {
			(*StitchBuffer)[iStitch].x         = ClipStitchPoints[iStitch].x;
			(*StitchBuffer)[iStitch].y         = ClipStitchPoints[iStitch].y;
			(*StitchBuffer)[iStitch].attribute = 0;
		}
		PCSHeader.stitchCount = ActivePointIndex;
#endif

#if CLPVU == 2

		auto iStitch = 0;
		for (iSegment = 0; iSegment < ClipSegmentIndex; iSegment++) {
			for (iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish; iStitchPoint++) {
				(*StitchBuffer)[iStitch].x           = ClipStitchPoints[iStitchPoint].x;
				(*StitchBuffer)[iStitch].y           = ClipStitchPoints[iStitchPoint].y;
				(*StitchBuffer)[iStitch++].attribute = iSegment & 0xf;
			}
		}
		PCSHeader.stitchCount = iStitch;

#endif

#if CLPVU == 0

		auto currentSegmentIndex = 0U;
		StateMap.set(StateFlag::FILDIR);
		OSequence->clear();
		auto clipIntersectSide = clipSegments[0].asid;
		ritseg(clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
		while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
			ritseg(clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
		}
		chksid(0, clipIntersectSide, currentFormVertices);
		auto index = 0U;
		for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size()); iSequence++) {
			if (vscmp(iSequence, index)) {
				index++;
				(*OSequence)[index] = (*OSequence)[iSequence];
			}
		}
		OSequence->resize(index);
		if (StateMap.test(StateFlag::WASNEG)) {
			for (auto& iSequence : *OSequence) {
				iSequence.x -= formOffset;
			}
			for (auto iVertex = 0U; iVertex < currentVertexCount; iVertex++) {
				vertexIt[iVertex].x -= formOffset;
			}
			form.rectangle.left -= formOffset;
			form.rectangle.right -= formOffset;
		}
#endif
	}
}

void form::internal::angout(FRMHED& angledForm) {
	if (angledForm.vertexCount != 0U) {
		auto rectangle     = &angledForm.rectangle;
		CurrentVertexIndex = angledForm.vertexIndex;
		auto vertexIt      = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		rectangle->left = rectangle->right = vertexIt[0].x;
		rectangle->bottom = rectangle->top = vertexIt[0].y;
		for (auto iVertex = 1U; iVertex < angledForm.vertexCount; iVertex++) {
			if (vertexIt[iVertex].x > rectangle->right) {
				rectangle->right = vertexIt[iVertex].x;
			}
			if (vertexIt[iVertex].x < rectangle->left) {
				rectangle->left = vertexIt[iVertex].x;
			}
			if (vertexIt[iVertex].y < rectangle->bottom) {
				rectangle->bottom = vertexIt[iVertex].y;
			}
			if (vertexIt[iVertex].y > rectangle->top) {
				rectangle->top = vertexIt[iVertex].y;
			}
		}
	}
}

void form::internal::horclpfn(const std::vector<RNGCNT>& textureSegments,
                              FRMHED&                    angledForm,
                              std::vector<fPOINT>&       angledFormVertices) {
	angledForm                = FormList->operator[](ClosestFormToCursor);
	const auto rotationCenter = fPOINT { form::midl(angledForm.rectangle.right, angledForm.rectangle.left),
		                                form::midl(angledForm.rectangle.top, angledForm.rectangle.bottom) };
	angledFormVertices.clear();
	angledFormVertices.reserve(angledForm.vertexCount);
	auto vertexIt = std::next(FormVertices->cbegin(), angledForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; iVertex++) {
		angledFormVertices.push_back(vertexIt[iVertex]);
		thred::rotflt(angledFormVertices.back(), (PI_F / 2.0F), rotationCenter);
	}
	angledForm.vertexIndex = 0;
	angout(angledForm);
	CurrentVertexIndex = angledForm.vertexIndex;
	clpcon(angledForm, textureSegments, angledFormVertices);
	rotbak((-PI_F / 2.0F), rotationCenter);
}

void form::angclpfn(const FRMHED& form, const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices) {
	auto rotationAngle = 0.0F;

	auto       angledForm     = form;
	const auto rotationCenter = fPOINT { form::midl(angledForm.rectangle.right, angledForm.rectangle.left),
		                                form::midl(angledForm.rectangle.top, angledForm.rectangle.bottom) };
	angledFormVertices.clear();
	angledFormVertices.reserve(angledForm.vertexCount);
	if (StateMap.test(StateFlag::ISUND)) {
		rotationAngle = PI_F / 2.0F - angledForm.underlayStitchAngle;

		const auto& vertexList = xt::insid(angledForm);
		for (auto iVertex = 0U; iVertex < angledForm.vertexCount; iVertex++) {
			angledFormVertices.push_back(vertexList[iVertex]);
			thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
		}
	}
	else {
		if (StateMap.test(StateFlag::TXFIL)) {
			rotationAngle = PI_F / 2.0F - angledForm.angleOrClipData.angle;
		}
		else {
			rotationAngle = PI_F / 2.0F - angledForm.satinOrAngle.angle;
		}
		auto vertexIt = std::next(FormVertices->cbegin(), angledForm.vertexIndex);
		for (auto iVertex = 0U; iVertex < angledForm.vertexCount; iVertex++) {
			angledFormVertices.push_back(vertexIt[iVertex]);
			thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
		}
	}
	angledForm.vertexIndex = 0;
	fi::angout(angledForm);
	CurrentVertexIndex = 0;
	fi::clpcon(angledForm, textureSegments, angledFormVertices);
	fi::rotbak(-rotationAngle, rotationCenter);
}

auto form::internal::spComp(const SMALPNTL* const arg1, const SMALPNTL* const arg2) noexcept -> bool {
	// make sure the comparison obeys strict weak ordering for stable sorting
	if ((arg1 != nullptr) && (arg2 != nullptr)) {
		const auto& lineEnd1 = *arg1;
		const auto& lineEnd2 = *arg2;
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

auto form::internal::isclos(const SMALPNTL* const lineEndPoint0,
                            const SMALPNTL* const lineEndPoint1,
                            double                gapToClosestRegion) noexcept -> bool {
	if ((lineEndPoint0 != nullptr) && (lineEndPoint1 != nullptr)) {
		const auto low0  = lineEndPoint0[0].y - gapToClosestRegion;
		const auto high0 = lineEndPoint0[1].y + gapToClosestRegion;
		const auto low1  = lineEndPoint1[0].y - gapToClosestRegion;
		const auto high1 = lineEndPoint1[1].y + gapToClosestRegion;

		if (high0 < low1) {
			return false;
		}
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
                            double                 gapToClosestRegion) noexcept -> bool {
	const auto lineEndPoint0 = &lineEndpoints[groupIndexSequence[group0]];

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
			count0--;
			index0 += 2;
		}
		if (count0 != 0U) {
			auto count1 = (groupIndexSequence[wrap::toSize(group1) + 1U] - groupIndexSequence[group1]) / 2U;
			auto index1 = 0U;
			if (const auto lineEndPoint1 = &lineEndpoints[groupIndexSequence[group1]]) {
				while ((count1 != 0U) && lineEndPoint1[index1].line != line1) {
					count1--;
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
                             const std::vector<SMALPNTL*>& sortedLines,
                             uint32_t                      iRegion0,
                             uint32_t                      iRegion1,
                             const std::vector<REGION>&    regionsList,
                             double                        gapToClosestRegion,
                             uint32_t&                     nextGroup) noexcept -> bool {
	const auto lineEndPoint0Start = sortedLines[regionsList[iRegion0].start];
	auto       lineEndPoint0End   = gsl::narrow_cast<SMALPNTL*>(nullptr);
	const auto lineEndPoint1Start = sortedLines[regionsList[iRegion1].start];
	auto       lineEndPoint1End   = gsl::narrow_cast<SMALPNTL*>(nullptr);
	const auto group0Start        = lineEndPoint0Start->group;
	auto       group0End          = 0U;
	const auto group1Start        = lineEndPoint1Start->group;
	auto       group1End          = 0U;
	auto       groupStart         = 0U;
	auto       lineStart          = 0U;
	auto       prevLine           = 0U;

	if (group0Start > group1Start) {
		groupStart = group0Start;
		lineStart  = lineEndPoint0Start->line;
		prevLine   = lineEndPoint1Start->line;
	}
	else {
		groupStart = group1Start;
		lineStart  = lineEndPoint1Start->line;
		prevLine   = lineEndPoint0Start->line;
	}
	if ((groupStart != 0U)
	    && lnclos(groupIndexSequence, lineEndpoints, groupStart - 1, prevLine, groupStart, lineStart, gapToClosestRegion)) {
		nextGroup = groupStart;
		return true;
	}

	lineEndPoint0End = sortedLines[regionsList[iRegion0].end];
	lineEndPoint1End = sortedLines[regionsList[iRegion1].end];
	group1End        = lineEndPoint1End->group;
	group0End        = lineEndPoint0End->group;
	auto groupEnd    = 0U;
	auto lineEnd     = 0U;
	auto lastLine    = 0U;
	if (group0End < group1End) {
		groupEnd = group0End;
		lineEnd  = lineEndPoint0End->line;
		lastLine = lineEndPoint1End->line;
	}
	else {
		groupEnd = group1End;
		lineEnd  = lineEndPoint1End->line;
		lastLine = lineEndPoint0End->line;
	}
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

auto form::internal::unvis(const boost::dynamic_bitset<>& visitedRegions, uint32_t& visitedIndex) -> bool {
	const auto regionCount = visitedRegions.size();
	for (visitedIndex = 0; visitedIndex < regionCount; visitedIndex++) {
		if (!visitedRegions[visitedIndex]) {
			return true;
		}
	}
	return false;
}

auto form::internal::notdun(std::vector<RGSEQ>&            tempPath,
                            const std::vector<RCON>&       pathMap,
                            const std::vector<uint32_t>&   mapIndexSequence,
                            const boost::dynamic_bitset<>& visitedRegions,
                            uint32_t                       pathLength,
                            uint32_t                       doneRegion,
                            uint32_t                       sequencePathIndex) -> bool {
	auto previousLevel = pathLength;
	if (previousLevel != 0U) {
		previousLevel--;
	}

	const auto regionPath = std::next(tempPath.begin(), sequencePathIndex);
	regionPath[0].pcon    = mapIndexSequence[doneRegion];
	regionPath[0].count   = mapIndexSequence[wrap::toSize(doneRegion) + 1U] - regionPath[0].pcon;
	for (auto iPath = 1U; iPath < pathLength; iPath++) {
		regionPath[iPath].pcon  = mapIndexSequence[pathMap[regionPath[iPath - 1U].pcon].node];
		regionPath[iPath].count = mapIndexSequence[wrap::toSize(pathMap[regionPath[wrap::toSize(iPath) - 1U].pcon].node) + 1U]
		                          - regionPath[iPath].pcon;
	}
	while (visitedRegions[pathMap[regionPath[previousLevel].pcon].node] && previousLevel >= 0) {
		if (--regionPath[previousLevel].count > 0) {
			regionPath[previousLevel].pcon++;
		}
		else {
			auto pivot = previousLevel;
			do {
				if (pivot != 0U) {
					pivot--;
				}
				else {
					return true;
				}
				regionPath[pivot].count--;
				regionPath[pivot].pcon++;
			} while (regionPath[pivot].count == 0);
			pivot++;
			while (pivot <= previousLevel) {
				if (pivot != 0U) {
					regionPath[pivot].pcon = mapIndexSequence[pathMap[regionPath[pivot - 1U].pcon].node];
					regionPath[pivot].count
					    = mapIndexSequence[wrap::toSize(pathMap[regionPath[wrap::toSize(pivot) - 1U].pcon].node) + 1U]
					      - regionPath[pivot].pcon;
				}
				else {
					if (--regionPath[0].count != 0) {
						regionPath[0].pcon++;
					}
					else {
						return true;
					}
				}
				pivot++;
			}
		}
	}
	return false;
}

auto form::internal::reglen(const std::vector<SMALPNTL*>& sortedLines,
                            uint32_t                      iRegion,
                            const std::vector<fPOINT>&    lastRegionCorners,
                            const std::vector<REGION>&    regionsList) -> double {
	auto lineEndPoints = std::vector<SMALPNTL*> {};
	lineEndPoints.resize(4);

	lineEndPoints[0]   = sortedLines[regionsList[iRegion].start];
	lineEndPoints[1]   = &sortedLines[regionsList[iRegion].start][1];
	lineEndPoints[2]   = sortedLines[regionsList[iRegion].end];
	lineEndPoints[3]   = &sortedLines[regionsList[iRegion].end][1];
	auto minimumLength = 1e99;
	for (auto iCorner = 0U; iCorner < 4; iCorner++) {
		for (auto iPoint = 0U; iPoint < 4; iPoint++) {
			const auto length = hypot(lastRegionCorners[iCorner].x - lineEndPoints[iPoint]->x,
			                          lastRegionCorners[iCorner].y - lineEndPoints[iPoint]->y);
			if (length < minimumLength) {
				minimumLength = length;
			}
		}
	}
	return minimumLength;
}

void form::internal::nxtrgn(std::vector<RGSEQ>&           tempPath,
                            const std::vector<RCON>&      pathMap,
                            const std::vector<uint32_t>&  mapIndexSequence,
                            boost::dynamic_bitset<>&      visitedRegions,
                            const std::vector<SMALPNTL*>& sortedLines,
                            const std::vector<REGION>&    regionsList,
                            uint32_t&                     doneRegion,
                            uint32_t                      pathMapIndex,
                            uint32_t&                     sequencePathIndex,
                            uint32_t                      visitedIndex) {
	auto lastRegionCorners = std::vector<fPOINT> {};
	lastRegionCorners.resize(4); // corners of last region sequenced

	auto pathLength = 1U; // length of the path to the region
	while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength, doneRegion, sequencePathIndex)) {
		pathLength++;
		if (pathLength > tempPath.size() - sequencePathIndex) {
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
			auto       minimumLength = 1e99;
			const auto regionCount   = visitedRegions.size();
			for (auto iRegion = 0U; iRegion < regionCount; iRegion++) {
				if (!visitedRegions[iRegion]) {
					const auto length = reglen(sortedLines, iRegion, lastRegionCorners, regionsList);
					if (length < minimumLength) {
						minimumLength = length;
						newRegion     = iRegion;
					}
				}
			}
			tempPath[sequencePathIndex].skp = true;
			for (auto iPath = 0U; iPath < pathMapIndex; iPath++) {
				if (pathMap[iPath].node == newRegion) {
					tempPath[sequencePathIndex++].pcon = iPath;
					visitedRegions.set(newRegion);
					doneRegion = newRegion;
					return;
				}
			}
			tempPath[sequencePathIndex].count  = visitedIndex;
			tempPath[sequencePathIndex++].pcon = 0xffffffff;
			visitedRegions.set(visitedIndex);
			doneRegion = visitedIndex;
			return;
		}
	}
	auto regionPath = std::next(tempPath.begin(), sequencePathIndex);
	for (auto iPath = 0U; iPath < pathLength; iPath++) {
		tempPath[sequencePathIndex].skp    = false;
		tempPath[sequencePathIndex++].pcon = regionPath[iPath].pcon;
		visitedRegions.set(pathMap[regionPath[iPath].pcon].node);
	}
	doneRegion = pathMap[regionPath[pathLength - 1U].pcon].node;
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::internal::nxtseq(std::vector<FSEQ>&           sequencePath,
                            const std::vector<RCON>&     pathMap,
                            const std::vector<uint32_t>& mapIndexSequence,
                            uint32_t                     pathIndex,
                            uint32_t&                    pathCount) {
	auto iPath = mapIndexSequence[sequencePath[pathIndex].node];

	if ((wrap::toSize(pathIndex) + 1U) < sequencePath.size()) {
		const auto nextNode = sequencePath[wrap::toSize(pathIndex) + 1U].node;
		while (iPath < mapIndexSequence[wrap::toSize(sequencePath[pathIndex].node) + 1U] && pathMap[iPath].node != nextNode) {
			iPath++;
		}
		if (iPath < pathMap.size()) {
			sequencePath[pathCount++].nextGroup = gsl::narrow<uint16_t>(pathMap[iPath].nextGroup);
		}
		else {
			sequencePath[pathCount++].nextGroup = 0;
		}
	}
	else {
		sequencePath[pathCount++].nextGroup = 0;
	}
}

void form::internal::rspnt(float xCoordinate, float yCoordinate) {
	OutputDebugString(fmt::format(L"rspnt: x {},y {}\n", xCoordinate, yCoordinate).c_str());
	BSequence->emplace_back(xCoordinate, yCoordinate, 0);
	OutputIndex++;
}

void form::internal::brkdun(const std::vector<SMALPNTL*>& sortedLines, uint32_t start, uint32_t finish) {
	rspnt(sortedLines[start]->x, sortedLines[start]->y);
	rspnt(sortedLines[finish]->x, sortedLines[finish]->y);
	rspnt((*WorkingFormVertices)[sortedLines[start]->line].x, (*WorkingFormVertices)[sortedLines[start]->line].y);
	StateMap.set(StateFlag::BRKFIX);
}

void form::internal::duseq1(const SMALPNTL* sequenceLines) {
	if (sequenceLines != nullptr) {
		rspnt(form::midl(sequenceLines[1].x, sequenceLines[0].x), form::midl(sequenceLines[1].y, sequenceLines[0].y));
	}
}

void form::internal::movseq(const std::vector<SMALPNTL*>& sortedLines, uint32_t ind) {
	auto lineEndPoint = sortedLines[ind];

	BSequence->emplace_back(BSEQPNT { lineEndPoint->x, lineEndPoint->y, SEQBOT });
	OutputIndex++;
	// Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
	//             and not the next entry in sortedLines
	lineEndPoint++;
	BSequence->emplace_back(BSEQPNT { lineEndPoint->x, lineEndPoint->y, SEQTOP });
	OutputIndex++;
}

void form::internal::brkseq(const std::vector<SMALPNTL*>& sortedLines,
                            uint32_t                      start,
                            uint32_t                      finish,
                            boost::dynamic_bitset<>&      sequenceMap,
                            uint32_t&                     lastGroup,
                            SMALPNTL*                     sequenceLines) {
	StateMap.reset(StateFlag::SEQDUN);
	if (sequenceLines == nullptr) {
		sequenceLines = sortedLines[start];
	}
	if (start > finish) {
		auto savedGroup = sortedLines[start]->group + 1U;
		// This odd construction for iLine is used to ensure
		// loop terminates when finish = 0
		for (auto iLine = start + 1U; iLine != finish; iLine--) {
			const auto iLineDec = iLine - 1U;
			savedGroup--;
			if (sortedLines[iLineDec]->group != savedGroup) {
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				sequenceLines = sortedLines[iLineDec];
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				savedGroup = sequenceLines[0].group;
			}
			else {
				sequenceLines = sortedLines[iLineDec];
			}
			if (sequenceMap.test_set(iLineDec)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
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
		for (auto iLine = start; iLine <= finish; iLine++) {
			savedGroup++;
			if (sortedLines[iLine]->group != savedGroup) {
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				sequenceLines = sortedLines[iLine];
				rspnt(sequenceLines[0].x, sequenceLines[0].y);
				savedGroup = sequenceLines[0].group;
			}
			else {
				sequenceLines = sortedLines[iLine];
			}
			if (sequenceMap.test_set(iLine)) {
				if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
					duseq1(sequenceLines);
				}
			}
			else {
				movseq(sortedLines, iLine);
			}
		}
		lastGroup = sequenceLines->group;
	}
	if (StateMap.testAndReset(StateFlag::SEQDUN)) {
		duseq1(sequenceLines);
	}
}

void form::internal::dunseq(const std::vector<SMALPNTL*>& sortedLines, uint32_t start, uint32_t finish, uint32_t& lastGroup) {
	auto minimumY = 1e30F;

	for (auto iLine = start; iLine <= finish; iLine++) {
		const auto deltaY = sortedLines[start][1].y - sortedLines[start][0].y;
		if (deltaY < minimumY) {
			minimumY = deltaY;
		}
	}
	if (minimumY == 1e30F) {
		minimumY = 0.0F;
	}
	else {
		minimumY /= 2.0F;
	}
	rspnt(sortedLines[start][0].x, sortedLines[start][0].y + minimumY);
	rspnt(sortedLines[finish][0].x, sortedLines[finish][0].y + minimumY);
	lastGroup = sortedLines[finish][0].group;
}

auto form::internal::duseq2(SMALPNTL* sequenceLines) -> SMALPNTL* {
	if (sequenceLines != nullptr) {
		rspnt(form::midl(sequenceLines[1].x, sequenceLines[0].x), form::midl(sequenceLines[1].y, sequenceLines[0].y));
	}
	return sequenceLines;
}

void form::internal::duseq(const std::vector<SMALPNTL*>& sortedLines,
                           uint32_t                      start,
                           uint32_t                      finish,
                           boost::dynamic_bitset<>&      sequenceMap,
                           uint32_t&                     lastGroup,
                           SMALPNTL*                     sequenceLines) {
	auto savedTopLine = sortedLines[start][1].line;
	bool flag         = false;

	sequenceLines = sortedLines[start];
	if (sequenceLines != nullptr) {
		StateMap.reset(StateFlag::SEQDUN);
		if (start > finish) {
			auto iLine = start + 1U;
			// This odd construction for iLine is used to ensure loop terminates when finish = 0
			for (; iLine != finish; iLine--) {
				const auto iLineDec = iLine - 1U;
				if (sequenceMap.test_set(iLineDec)) {
					if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
						flag          = true;
						sequenceLines = duseq2(sortedLines[iLineDec]);
					}
					else {
						if (savedTopLine != sortedLines[iLineDec][1].line) {
							if (iLineDec != 0U) {
								sequenceLines = duseq2(sortedLines[wrap::toSize(iLineDec) + 1U]); // NOLINT
							}
							flag          = true;
							sequenceLines = duseq2(sortedLines[iLineDec]);
							savedTopLine  = sequenceLines[1].line;
						}
					}
				}
				else {
					if (StateMap.testAndReset(StateFlag::SEQDUN)) {
						sequenceLines = duseq2(sortedLines[wrap::toSize(iLineDec) + 1U]); // NOLINT
					}
					flag          = true;
					sequenceLines = sortedLines[iLineDec];
					movseq(sortedLines, iLineDec);
				}
			}
			if (StateMap.testAndReset(StateFlag::SEQDUN)) {
				flag          = true;
				sequenceLines = duseq2(sortedLines[iLine]);
			}
			if (flag) {
				lastGroup = sequenceLines->group;
			}
		}
		else {
			auto iLine = start;
			for (iLine = start; iLine <= finish; iLine++) {
				if (sequenceMap.test_set(iLine)) {
					if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
						flag          = true;
						sequenceLines = duseq2(sortedLines[iLine]);
					}
					else {
						if (savedTopLine != sortedLines[iLine][1].line) {
							if (iLine != 0U) {
								sequenceLines = duseq2(sortedLines[iLine - 1U]); // NOLINT
							}
							flag          = true;
							sequenceLines = duseq2(sortedLines[iLine]);
							savedTopLine  = sequenceLines[1].line;
						}
					}
				}
				else {
					if (StateMap.testAndReset(StateFlag::SEQDUN)) {
						if (iLine != 0U) {
							sequenceLines = duseq2(sortedLines[iLine - 1U]); // NOLINT
						}
					}
					flag          = true;
					sequenceLines = sortedLines[iLine];
					movseq(sortedLines, iLine);
				}
			}
			if (StateMap.testAndReset(StateFlag::SEQDUN)) {
				if (iLine != 0U) {
					flag          = true;
					sequenceLines = duseq2(sortedLines[iLine - 1U]);
				}
			}
			if (flag) {
				lastGroup = sequenceLines->group;
			}
		}
	}
}

void form::internal::durgn(const std::vector<FSEQ>&      sequencePath,
                           boost::dynamic_bitset<>&      visitedRegions,
                           const std::vector<SMALPNTL*>& sortedLines,
                           uint32_t                      pthi,
                           uint32_t                      lineCount,
                           std::vector<REGION>&          regionsList,
                           uint32_t&                     lastGroup,
                           uint32_t                      sequencePathIndex) {
	auto        sequenceMap   = boost::dynamic_bitset<>(lineCount);
	auto        nextGroup     = sequencePath[pthi].nextGroup;
	const auto  iRegion       = sequencePath[pthi].node;
	const auto& currentRegion = regionsList[iRegion];
	const auto  sequenceStart = currentRegion.start;
	const auto  sequenceEnd   = currentRegion.end;
	if (sequencePath[pthi].skp || StateMap.testAndReset(StateFlag::BRKFIX)) {
		if ((*BSequence)[OutputIndex - 1U].attribute != SEQBOT) {
			rspnt((*BSequence)[OutputIndex - 2U].x, (*BSequence)[OutputIndex - 2U].y);
		}
		const auto  firstLine     = sortedLines[sequenceStart]->line;
		const auto& bpnt          = (*BSequence)[OutputIndex - 1U];
		auto        minimumLength = 1e99;
		auto        mindif        = 0U;
		for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
			const auto length = hypot(bpnt.x - (*WorkingFormVertices)[iVertex].x, bpnt.y - (*WorkingFormVertices)[iVertex].y);
			if (length < minimumLength) {
				minimumLength = length;
				mindif        = iVertex;
			}
		}
		if (minimumLength != 0.0) {
			rspnt((*WorkingFormVertices)[mindif].x, (*WorkingFormVertices)[mindif].y);
		}
		if (VertexCount != 0U) {
			const auto fdif = (VertexCount + firstLine - mindif) % VertexCount;
			const auto bdif = (VertexCount - firstLine + mindif) % VertexCount;
			if (fdif < bdif) {
				auto ind = form::nxt(mindif);
				while (ind != firstLine) {
					rspnt((*WorkingFormVertices)[ind].x, (*WorkingFormVertices)[ind].y);
					ind = form::nxt(ind);
				}
				rspnt((*WorkingFormVertices)[ind].x, (*WorkingFormVertices)[ind].y);
			}
			else {
				auto ind = form::prv(mindif);
				while (ind != firstLine) {
					rspnt((*WorkingFormVertices)[ind].x, (*WorkingFormVertices)[ind].y);
					ind = form::prv(ind);
				}
				rspnt((*WorkingFormVertices)[ind].x, (*WorkingFormVertices)[ind].y);
			}
		}
	}
	auto dun = true;
	if (!visitedRegions[iRegion]) {
		dun = false;
		visitedRegions.set(iRegion);
	}
	const auto groupStart = sortedLines[currentRegion.start]->group;
	const auto groupEnd   = sortedLines[currentRegion.end]->group;
	auto       seql       = 0U;
	if (groupEnd != groupStart) {
		const auto intermediate
		    = std::round((gsl::narrow_cast<double>(lastGroup) - groupStart) / (gsl::narrow_cast<double>(groupEnd) - groupStart)
		                     * (gsl::narrow_cast<double>(sequenceEnd) - sequenceStart)
		                 + sequenceStart);
		seql = gsl::narrow<decltype(seql)>(std::abs(intermediate));
	}
	const auto length
	    = (gsl::narrow_cast<double>(groupEnd) - groupStart) * (gsl::narrow_cast<double>(sequenceEnd) - sequenceStart);
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
			seql++;
		}
		else {
			if (seql > sequenceStart && sortedLines[seql - 1U]->group == lastGroup) {
				seql--;
			}
			else {
				auto mindif = 0xffffffffU;
				for (auto ind = sequenceStart; ind <= sequenceEnd; ind++) {
					const auto gdif = ((sortedLines[ind]->group > lastGroup) ? (sortedLines[ind]->group - lastGroup)
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
			seqn++;
		}
		else {
			if (seqn > sequenceStart && sortedLines[seqn - 1U]->group == nextGroup) {
				seqn--;
			}
			else {
				auto mindif = 0xffffffffU;
				for (auto ind = sequenceStart; ind <= sequenceEnd; ind++) {
					const auto gdif = ((sortedLines[ind]->group > nextGroup) ? (sortedLines[ind]->group - nextGroup)
					                                                         : (nextGroup - sortedLines[ind]->group));
					if (gdif < mindif) {
						mindif = gdif;
						seqn   = ind;
					}
				}
			}
		}
	}
	auto sequenceLines = gsl::narrow_cast<SMALPNTL*>(nullptr);
	if (currentRegion.breakCount != 0U) {
		if (dun) {
			brkdun(sortedLines, seql, seqn);
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

void form::internal::lcon(std::vector<uint32_t>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints) {
#if BUGSEQ
	UNREFERENCED_PARAMETER(groupIndexSequence);
#endif

	if (!lineEndpoints.empty()) {
		auto       sortedLines     = std::vector<SMALPNTL*> {};
		const auto stitchLineCount = lineEndpoints.size();
		sortedLines.reserve(stitchLineCount / 2);
		for (auto iLine = 0U; iLine < stitchLineCount; iLine += 2) {
			sortedLines.push_back(&lineEndpoints[iLine]);
		}
		std::sort(sortedLines.begin(), sortedLines.end(), fi::spComp);
		const auto lineCount = wrap::toUnsigned(sortedLines.size());
		auto       regions   = std::vector<REGION> {};
		regions.emplace_back(0U, 0U, 0U, 0U);
		auto breakLine = sortedLines[0]->line;
		for (auto iLine = 0U; iLine < lineCount; iLine++) {
			if (breakLine != sortedLines[iLine]->line) {
				regions.back().end = iLine - 1U;
				regions.emplace_back(iLine, 0U, 0U, 0U);
				breakLine = sortedLines[iLine]->line;
			}
		}
		regions.back().end            = lineCount - 1U;
		const auto     regionCount    = wrap::toUnsigned(regions.size());
		auto           visitedRegions = boost::dynamic_bitset<>(regionCount);
		constexpr auto iStartLine     = 0U;
		for (auto iRegion = 0U; iRegion < regionCount; iRegion++) {
			auto count = 0U;
			if ((regions[iRegion].end - regions[iRegion].start) > 1) {
				auto startGroup = sortedLines[regions[iRegion].start]->group;
				for (auto iLine = regions[iRegion].start + 1U; iLine <= regions[iRegion].end; iLine++) {
					startGroup++;
					if (sortedLines[iLine]->group != startGroup) {
						if (count == 0U) {
							regions[iRegion].regionBreak = iStartLine;
						}
						count++;
						startGroup = sortedLines[iLine]->group;
					}
				}
			}
			regions[iRegion].breakCount = count;
		}

#if BUGSEQ
		// Note - this debug code only works for vertical fill on a single form
		auto bugColor = 0U;
		for (auto iRegion = 0U; iRegion < regionCount; iRegion++) {
			for (auto iLine = regions[iRegion].start; iLine <= regions[iRegion].end; iLine++) {
				const auto* lineGroupPoint = &*sortedLines[iLine];
				StitchBuffer->push_back(fPOINTATTR { lineGroupPoint[0].x, lineGroupPoint[0].y, bugColor });
				StitchBuffer->push_back(fPOINTATTR { lineGroupPoint[1].x, lineGroupPoint[1].y, bugColor });
			}
			bugColor++;
			bugColor &= 0xf;
		}
#else
		OutputIndex = 0;
		BSequence->clear();

		auto mapIndexSequence = std::vector<uint32_t> {};
		mapIndexSequence.reserve(wrap::toSize(regionCount) + 1U);
		auto pathMap      = std::vector<RCON> {};
		auto sequencePath = std::vector<FSEQ> {};

		if (regionCount > 1) {
			auto pathMapIndex = 0U;
			// use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
			pathMap.reserve(wrap::toSize((regionCount * (regionCount - 1U)) / 2U) + 2U);
			for (auto iSequence = 0U; iSequence < regionCount; iSequence++) {
				mapIndexSequence.push_back(pathMapIndex);
				auto count              = 0;
				auto gapToClosestRegion = 0.0;
				for (auto iNode = 0U; iNode < regionCount; iNode++) {
					if (iSequence != iNode) {
						auto       nextGroup   = 0U;
						const auto isConnected = regclos(groupIndexSequence,
						                                 lineEndpoints,
						                                 sortedLines,
						                                 iSequence,
						                                 iNode,
						                                 regions,
						                                 gapToClosestRegion,
						                                 nextGroup);
						if (isConnected) {
							pathMap.push_back({ iNode, isConnected, nextGroup });
							pathMapIndex++;
							count++;
						}
					}
				}
				while (count == 0) {
					gapToClosestRegion += LineSpacing;
					count = 0;
					for (auto iNode = 0U; iNode < regionCount; iNode++) {
						if (iSequence != iNode) {
							auto       nextGroup   = 0U;
							const auto isConnected = regclos(groupIndexSequence,
							                                 lineEndpoints,
							                                 sortedLines,
							                                 iSequence,
							                                 iNode,
							                                 regions,
							                                 gapToClosestRegion,
							                                 nextGroup);
							if (isConnected) {
								pathMap.push_back({ iNode, isConnected, nextGroup });
								pathMapIndex++;
								count++;
							}
						}
					}
				}
			}
			mapIndexSequence.push_back(pathMapIndex);
			// find the leftmost region
			auto startGroup = 0xffffffffU;
			auto leftRegion = 0U;
			for (auto iRegion = 0U; iRegion < regionCount; iRegion++) {
				auto lineGroupPoint = sortedLines[regions[iRegion].start];
				if (lineGroupPoint->group < startGroup) {
					startGroup = lineGroupPoint->group;
					leftRegion = iRegion;
				}
			}
			OutputIndex   = 0;
			auto tempPath = std::vector<RGSEQ> {};
			tempPath.resize(wrap::toSize((regionCount * (regionCount - 1U)) / 2U) + 1U);

			// find the leftmost region in pathMap
			auto sequencePathIndex = 1U;
			auto dontSkip          = true;
			auto inPath            = 0U;
			for (inPath = 0U; inPath < pathMapIndex; inPath++) {
				if (pathMap[inPath].node == leftRegion) {
					dontSkip = false;
					break;
				}
			}
			if (dontSkip) {
				pathMap.push_back(RCON { leftRegion, false, 0 });
				inPath = pathMapIndex;
			}
			// set the first entry in the temporary path to the leftmost region
			tempPath[0].pcon  = inPath;
			tempPath[0].count = 1;
			tempPath[0].skp   = false;
			visitedRegions.set(leftRegion);
			auto doneRegion   = leftRegion; // last region sequenced
			auto visitedIndex = 0U;
			while (unvis(visitedRegions, visitedIndex)) {
				nxtrgn(tempPath,
				       pathMap,
				       mapIndexSequence,
				       visitedRegions,
				       sortedLines,
				       regions,
				       doneRegion,
				       pathMapIndex,
				       sequencePathIndex,
				       visitedIndex);
			}
			auto count = 0xffffffffU;
			sequencePath.reserve(sequencePathIndex);
			for (auto iPath = 0U; iPath < sequencePathIndex; iPath++) {
				const bool tmpSkip = tempPath[iPath].skp;
				uint16_t   tmpNode = 0U;
				if (tempPath[iPath].pcon == 0xffffffff) {
					tmpNode = gsl::narrow<decltype(tmpNode)>(tempPath[iPath].count);
					count   = tempPath[iPath].count;
				}
				else {
					if (tempPath[iPath].pcon != count) {
						count   = tempPath[iPath].pcon;
						tmpNode = gsl::narrow<decltype(tmpNode)>(pathMap[tempPath[iPath].pcon].node);
					}
				}
				sequencePath.push_back({ tmpNode, 0, tmpSkip });
			}
			auto pathCount = 0U;
			for (auto iPath = 0U; iPath < sequencePathIndex; iPath++) {
				nxtseq(sequencePath, pathMap, mapIndexSequence, iPath, pathCount);
			}
			visitedRegions.reset();
			auto lastGroup = 0U;
			for (auto iPath = 0U; iPath < pathCount; iPath++) {
				OutputDebugString(
				    fmt::format(L"iterator {},vrt {},grpn {}\n", iPath, pathMap[iPath].node, pathMap[iPath].nextGroup).c_str());
				if (!unvis(visitedRegions, visitedIndex)) {
					break;
				}
				durgn(sequencePath, visitedRegions, sortedLines, iPath, lineCount, regions, lastGroup, sequencePathIndex);
			}
		}
		else {
			constexpr auto sequencePathIndex = 1U;
			sequencePath.resize(sequencePathIndex);
			auto lastGroup            = 0U;
			sequencePath[0].node      = 0;
			sequencePath[0].nextGroup = gsl::narrow<uint16_t>(sortedLines[regions[0].end]->group);
			sequencePath[0].skp       = false;
			durgn(sequencePath, visitedRegions, sortedLines, 0, lineCount, regions, lastGroup, sequencePathIndex);
		}

#endif
	}
}

void form::internal::bakseq() {
#if BUGSEQ
#else
	constexpr auto RITSIZ = 6;

	const uint32_t seqtab[] = {
		12, 7, 15, 11, 13, 17,
	};

#if BUGBAK

	for (SequenceIndex = 0; SequenceIndex < OutputIndex; SequenceIndex++) {
		(*OSequence)[SequenceIndex].x = (*BSequence)[SequenceIndex].x;
		(*OSequence)[SequenceIndex].y = (*BSequence)[SequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	auto iSequence = OutputIndex;
	if (iSequence != 0U) {
		iSequence--;
	}

	OSequence->clear();
	StateMap.reset(StateFlag::FILDIR);
	OSequence->push_back(fPOINT { (*BSequence)[iSequence].x, (*BSequence)[iSequence].y });
	SelectedPoint = (*BSequence)[iSequence];
	if (iSequence != 0U) {
		iSequence--;
	}
	while (iSequence > 0) {
		const auto rcnt           = iSequence % RITSIZ;
		const auto StitchSpacing2 = LineSpacing * 2;
		const auto rit            = wrap::round<int32_t>((*BSequence)[iSequence].x / StitchSpacing2);
		auto&      bPrevious      = (*BSequence)[iSequence - 1U];
		auto&      bCurrent       = (*BSequence)[iSequence];
		auto&      bNext          = (*BSequence)[wrap::toSize(iSequence) + 1U];
		auto       delta          = fPOINT { bCurrent.x - bNext.x, bCurrent.y - bNext.y };
		auto       slope          = 1e38F;
		if (delta.y != 0.0) {
			slope = delta.x / delta.y;
		}

		const auto UserStitchLength9 = UserStitchLength / 9;
		const auto& form = FormList->operator[](ClosestFormToCursor);
		switch (bCurrent.attribute) {
		case SEQTOP: {
			if ((form.extendedAttribute & AT_SQR) != 0U) {
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					OSequence->push_back(fPOINT { bPrevious.x, bPrevious.y });
					auto count = wrap::ceil<int32_t>(bCurrent.y / UserStitchLength);
					do {
						OSequence->push_back(fPOINT {
						    0.0F, count * UserStitchLength + gsl::narrow_cast<float>(rit % seqtab[rcnt]) * UserStitchLength9 });
						if (OSequence->back().y > bCurrent.y) {
							break;
						}
						delta.y             = OSequence->back().y - bCurrent.y;
						OSequence->back().x = bCurrent.x;
						count++;
					} while (true);
					OSequence->back() = bCurrent;
				}
				else {
					OSequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
					auto count = wrap::floor<int32_t>(bCurrent.y / UserStitchLength);
					do {
						OSequence->push_back(fPOINT {
						    0.0F,
						    count * UserStitchLength - gsl::narrow_cast<float>((rit + 2) % seqtab[rcnt]) * UserStitchLength9 });
						if (OSequence->back().y < bPrevious.y) {
							break;
						}
						delta.y             = OSequence->back().y - bPrevious.y;
						OSequence->back().x = bCurrent.x;
						count--;
					} while (true);
					OSequence->back() = bPrevious;
				}
			}
			else {
				auto count = wrap::ceil<int32_t>(bNext.y / UserStitchLength);
				do {
					OSequence->push_back(fPOINT {
					    0.0F, count * UserStitchLength + gsl::narrow_cast<float>(rit % seqtab[rcnt]) * UserStitchLength9 });
					if (OSequence->back().y > bCurrent.y) {
						break;
					}
					delta.y             = OSequence->back().y - bNext.y;
					delta.x             = slope * delta.y;
					OSequence->back().x = bNext.x + delta.x;
					count++;
				} while (true);
				OSequence->back() = bCurrent;
			}
			break;
		}
		case SEQBOT: {
			if ((form.extendedAttribute & AT_SQR) == 0U) {
				auto count = wrap::floor<int32_t>(bNext.y / UserStitchLength);
				do {
					OSequence->push_back(fPOINT {
					    0.0F, count * UserStitchLength - gsl::narrow_cast<float>((rit + 2) % seqtab[rcnt]) * UserStitchLength9 });
					if (OSequence->back().y < bCurrent.y) {
						break;
					}
					delta.y             = OSequence->back().y - bNext.y;
					delta.x             = slope * delta.y;
					OSequence->back().x = bNext.x + delta.x;
					count--;
				} while (true);
				OSequence->back() = bCurrent;
			}
			break;
		}
		case 0: {
			delta = fPOINT { bCurrent.x - bNext.x, bCurrent.y - bNext.y };
			StateMap.reset(StateFlag::FILDIR);
			const auto length = hypot(delta.x, delta.y);
			if (length != 0.0F) {
				const auto UserStitchLength2 = UserStitchLength * 2.0F;
				if (length > UserStitchLength2) {
					auto point = bNext;
					auto count = wrap::round<uint32_t>(length / UserStitchLength - 1.0F);
					if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
						return;
					}
					const auto step = fPOINT { delta.x / count, delta.y / count };
					while (count != 0U) {
						point.x += step.x;
						point.y += step.y;
						OSequence->push_back(fPOINT { point.x, point.y });
						count--;
					}
				}
			}
			OSequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
		}
		}
		iSequence--;
	}
#endif // BUGBAK
#endif // BUGSEQ
}

void form::filinu(const fPOINT& inPoint) {
	auto       point  = SelectedPoint;
	const auto delta  = fPOINT { (inPoint.x - SelectedPoint.x), (inPoint.y - SelectedPoint.y) };
	const auto length = hypot(delta.x, delta.y);
	auto       count  = wrap::round<uint32_t>(length / UserStitchLength);

	if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
		return;
	}
	if (count != 0U) {
		if (StateMap.test(StateFlag::FILDIR)) {
			count++;
		}
		const auto step = fPOINT { delta.x / count, delta.y / count };
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence->push_back(point);
			count--;
		}
	}
	else {
		OSequence->push_back(fPOINT { inPoint.x, inPoint.y });
	}
	SelectedPoint = inPoint;
}

void form::filin(fPOINT currentPoint) {
	const auto delta  = fPOINT { (currentPoint.x - SelectedPoint.x), (currentPoint.y - SelectedPoint.y) };
	auto       point  = SelectedPoint;
	const auto length = hypot(delta.x, delta.y);
	auto       count  = wrap::round<uint32_t>(length / UserStitchLength);

	if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
		return;
	}
	if (count != 0U) {
		if (StateMap.test(StateFlag::FILDIR)) {
			count++;
		}
		const auto step = fPOINT { delta.x / count, delta.y / count };
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence->push_back(point);
			count--;
		}
	}
	else {
		OSequence->push_back(fPOINT { currentPoint });
	}
	SelectedPoint = currentPoint;
}

void form::internal::dudif(const fPOINT& start, const fPOINT& finish, fPOINT& delta) noexcept {
	delta.x = finish.x - start.x;
	delta.y = finish.y - start.y;
}

void form::internal::trfrm(const fPOINT& bottomLeftPoint,
                           const fPOINT& topLeftPoint,
                           const fPOINT& bottomRightPoint,
                           const fPOINT& topRightPoint) {
	auto topDelta    = fPOINT {};
	auto bottomDelta = fPOINT {};
	auto leftDelta   = fPOINT {};
	auto rightDelta  = fPOINT {};

	dudif(topLeftPoint, topRightPoint, topDelta);
	dudif(bottomLeftPoint, bottomRightPoint, bottomDelta);
	dudif(bottomLeftPoint, topLeftPoint, leftDelta);
	dudif(bottomRightPoint, topRightPoint, rightDelta);
	for (auto& clip : *ClipBuffer) {
		const auto clipRatio = fPOINT { clip.x / ClipRectSize.cx, clip.y / ClipRectSize.cy };
		const auto topMidpoint
		    = fPOINT { clipRatio.x * (topDelta.x) + topLeftPoint.x, clipRatio.x * (topDelta.y) + topLeftPoint.y };
		const auto bottomMidpoint
		    = fPOINT { clipRatio.x * (bottomDelta.x) + bottomLeftPoint.x, clipRatio.x * (bottomDelta.y) + bottomLeftPoint.y };
		auto middleDelta = fPOINT {};
		dudif(bottomMidpoint, topMidpoint, middleDelta);
		OSequence->push_back(
		    fPOINT { clipRatio.y * middleDelta.x + bottomMidpoint.x, clipRatio.y * middleDelta.y + bottomMidpoint.y });
	}
}

void form::internal::clpfm() {
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(BSequence->size()) - 2U; iSequence += 2) {
		auto&      bSeq0       = (*BSequence)[iSequence];
		auto&      bSeq1       = (*BSequence)[wrap::toSize(iSequence) + 1U];
		auto&      bSeq2       = (*BSequence)[wrap::toSize(iSequence) + 2U];
		auto&      bSeq3       = (*BSequence)[wrap::toSize(iSequence) + 3U];
		const auto leftLength  = hypot(bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y);
		const auto rightLength = hypot(bSeq3.x - bSeq2.x, bSeq3.y - bSeq2.y);
		const auto leftDelta   = fPOINT { bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y };
		const auto rightDelta  = fPOINT { bSeq2.x - bSeq3.x, bSeq2.y - bSeq3.y };
		auto       count       = 0U;
		if (rightLength > leftLength) {
			count = wrap::round<decltype(count)>(leftLength / ClipRectSize.cy);
		}
		else {
			count = wrap::round<decltype(count)>(rightLength / ClipRectSize.cy);
		}
		if (count == 0U) {
			count = 1;
		}
		const auto leftStep  = fPOINT { leftDelta.x / count, leftDelta.y / count };
		const auto rightStep = fPOINT { rightDelta.x / count, rightDelta.y / count };
		auto       topLeft   = fPOINT { bSeq0.x, bSeq0.y };
		auto       topRight  = fPOINT { bSeq3.x, bSeq3.y };
		for (auto iStep = 0U; iStep < count; iStep++) {
			const auto bottomLeft  = topLeft;
			const auto bottomRight = topRight;
			topLeft.x += leftStep.x;
			topLeft.y += leftStep.y;
			topRight.x += rightStep.x;
			topRight.y += rightStep.y;
			trfrm(bottomLeft, topLeft, bottomRight, topRight);
		}
	}
}

void form::internal::fmclp(FRMHED& form) {
	const auto savedSpacing = LineSpacing;
	LineSpacing = ClipRectSize.cx;
	StateMap.set(StateFlag::BARSAT);
	satin::satfil(form);
	StateMap.reset(StateFlag::BARSAT);
	clpfm();
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	currentForm.fillType = CLPF;
	LineSpacing            = savedSpacing;
}

void form::refilfn() {
	const auto savedStitchLength   = UserStitchLength;
	auto       angledForm          = FRMHED {};
	auto       workingFormVertices = std::vector<fPOINT> {};
	WorkingFormVertices            = &workingFormVertices;

	StateMap.reset(StateFlag::TXFIL);
	form::fvars(ClosestFormToCursor);
	auto& form = FormList->operator[](ClosestFormToCursor);
	if (form.type == FRMLINE) {
		form.underlayIndent = 0;
	}
	if (!(StateMap.test(StateFlag::FUNCLP) || StateMap.test(StateFlag::FUNSCLP))) {
		thred::savdo();
	}
	auto fillStartsData = FILLSTARTS {}; // fill start data for refill
	auto fillStartsMap  = 0U;            // fill starts bitmap
	xt::fdelstch(form, fillStartsData, fillStartsMap);
	StateMap.set(StateFlag::WASREFIL);
	if (form.fillSpacing < 0.5F && !clip::isclp(form)) {
		form.fillSpacing = 0.5F;
	}
	if (form.edgeSpacing < 0.5F) {
		form.edgeSpacing = 0.5F;
	}
	if (!clip::isclp(form)) {
		UserStitchLength = form.lengthOrCount.stitchLength;
	}
	if (!(StateMap.test(StateFlag::WASDO) || StateMap.test(StateFlag::FUNCLP) || StateMap.test(StateFlag::FUNSCLP))) {
		thred::savdo();
	}
	StateMap.reset(StateFlag::WASDO);
	if (((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U) && form.type == FRMLINE
	    && form.fillType != CONTF) {
		form.type = FRMFPOLY;
	}
	InterleaveSequence->clear();
	InterleaveSequenceIndices->clear();
	StateMap.reset(StateFlag::ISUND);
	auto textureSegments = std::vector<RNGCNT> {};
	textureSegments.resize(form.fillInfo.texture.lines);
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
			StateMap.reset(StateFlag::SAT1);
			satin::slbrd(form);
			fi::ritbrd(form);
			break;
		}
		case EDGEPROPSAT: {
			if (form.vertexCount > 2) {
				StateMap.reset(StateFlag::SAT1);
				fi::plbrd(form.edgeSpacing, angledForm, *AngledFormVertices);
				fi::ritbrd(form);
			}
			break;
		}
		case EDGEAPPL: {
			fi::lapbrd(form.vertexIndex);
			fi::ritapbrd();
			StateMap.reset(StateFlag::SAT1);
			satin::slbrd(form);
			fi::ritbrd(form);
			break;
		}
		case EDGEBHOL: {
			const auto length      = ButtonholeCornerLength;
			ButtonholeCornerLength = form::getblen();
			satin::satout(form, 20);
			fi::blbrd(form);
			ButtonholeCornerLength = length;
			fi::ritbrd(form);
			break;
		}
		case EDGEPICOT: {
			auto clipRect = fRECTANGLE {};
			clip::oclp(clipRect, form.borderClipData, form.clipEntries);
			const auto length      = ButtonholeCornerLength;
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
			StateMap.set(StateFlag::LINCHN);
			clip::chnfn(form);
			fi::ritbrd(form);
			break;
		}
		case EDGEOCHAIN: {
			StateMap.reset(StateFlag::LINCHN);
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
		}
		if (form.fillType == CONTF && ((form.attribute & FRECONT) != 0)) {
			fi::contf();
			fi::ritfil();
		}
		break;
	}
	case FRMFPOLY: {
		xt::chkcwlk(form);
		xt::chkwlk(form);
		xt::chkund(form, textureSegments, *AngledFormVertices);
		StateMap.reset(StateFlag::ISUND);
		if (form.fillType != 0U) {
			const auto spacing      = LineSpacing;
			LineSpacing             = form.fillSpacing;
			auto lineEndpoints      = std::vector<SMALPNTL> {};
			auto groupIndexSequence = std::vector<uint32_t> {};
			auto rotationCenter     = fPOINT {};
			auto doFill             = true;
			auto rotationAngle      = 0.0;
			switch (form.fillType) {
			case VRTF: {
				workingFormVertices.clear();
				workingFormVertices.reserve(form.vertexCount);
				auto startVertex = std::next(FormVertices->cbegin(), form.vertexIndex);
				auto endVertex   = std::next(startVertex, form.vertexCount);
				workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
				fi::fnvrt(workingFormVertices, groupIndexSequence, lineEndpoints);
				break;
			}
			case HORF: {
				rotationAngle = PI / 2;
				fi::fnhor(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, *AngledFormVertices);
				workingFormVertices.clear();
				workingFormVertices.reserve(angledForm.vertexCount);
				auto startVertex = std::next(AngledFormVertices->cbegin(), angledForm.vertexIndex);
				auto endVertex   = std::next(startVertex, angledForm.vertexCount);
				workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
				break;
			}
			case ANGF: {
				rotationAngle = PI / 2 - form.angleOrClipData.angle;
				fi::fnang(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, *AngledFormVertices);
				workingFormVertices.clear();
				workingFormVertices.reserve(angledForm.vertexCount);
				auto startVertex = std::next(AngledFormVertices->cbegin(), angledForm.vertexIndex);
				auto endVertex   = std::next(startVertex, angledForm.vertexCount);
				workingFormVertices.insert(workingFormVertices.end(), startVertex, endVertex);
				break;
			}
			case VCLPF: {
				auto clipRect = fRECTANGLE {};
				clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
				form::fvars(ClosestFormToCursor);
				workingFormVertices.clear();
				workingFormVertices.reserve(form.vertexCount);
				auto startVertex = std::next(FormVertices->cbegin(), form.vertexIndex);
				auto endVertex   = std::next(startVertex, form.vertexCount);
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
				StateMap.reset(StateFlag::ISUND);
				form::angclpfn(form, textureSegments, *AngledFormVertices);
				doFill = false;
				break;
			}
			case TXVRTF: {
				texture::setxt(form, textureSegments);
				workingFormVertices.clear();
				workingFormVertices.reserve(form.vertexCount);
				auto startVertex = std::next(FormVertices->cbegin(), form.vertexIndex);
				auto endVertex   = std::next(startVertex, form.vertexCount);
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
				StateMap.reset(StateFlag::ISUND);
				form::angclpfn(form, textureSegments, *AngledFormVertices);
				doFill = false;
				break;
			}
			}
			if (doFill) {
				fi::lcon(groupIndexSequence, lineEndpoints);
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
		StateMap.reset(StateFlag::ISUND);
		switch (form.fillType) {
		case SATF: {
			const auto spacing = LineSpacing;
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
			StateMap.set(StateFlag::CNV2FTH);
			xt::fthrfn(form);
			break;
		}
		}
		fi::chkbrd(form);
	}
	}
	UserStitchLength = savedStitchLength;
	xt::intlv(form, fillStartsData, fillStartsMap);
	thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	xt::setfchk();
}

void form::refil() {
	if (!UserFlagMap.test(UserFlag::WRNOF)) {
		const auto codedForm = ClosestFormToCursor << FRMSHFT | USMSK;
		for (auto stitch : *StitchBuffer) {
			if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & (USMSK | FRMSK)) == codedForm) {
				if (FormDataSheet != nullptr) {
					StateMap.set(StateFlag::WASFRMFRM);
				}
				thred::undat();
				displayText::tabmsg(IDS_REFIL);
				StateMap.set(StateFlag::MOVMSG);
				displayText::okcan();
				return;
			}
		}
	}
	form::refilfn();
}

void form::setfpnt() {
	form::fvars(ClosestFormToCursor);

	const auto screenCoordinate = POINT { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };
	auto       vertexIt         = std::next(FormVertices->begin(), CurrentVertexIndex);

	form::unfrm();
	fi::rats();
	fi::px2stchf(screenCoordinate, vertexIt[ClosestVertexToCursor]);
	form::frmout(ClosestFormToCursor);
	refil();
	StateMap.set(StateFlag::WASFPNT);
	StateMap.reset(StateFlag::SELBOX);
	StateMap.set(StateFlag::FRMPSEL);
	thred::ritfcor(vertexIt[ClosestVertexToCursor]);
	StateMap.set(StateFlag::RESTCH);
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
	form.fillColor = gsl::narrow<decltype(form.fillColor)>(ActiveColor);
	form.fillType  = VRTF;
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
			ClosestFormToCursor = selectedForm;
			const auto& currentForm = FormList->operator[](ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			if (currentForm.type != FRMLINE) {
				fi::fsvrt();
			}
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			fi::fsvrt();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::internal::fshor(FRMHED& form) {
	clip::delmclp(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	makpoli();
	form.type = FRMFPOLY;
	form.fillColor = gsl::narrow<decltype(form.fillColor)>(ActiveColor);
	form.fillType  = HORF;
	form.fillSpacing           = LineSpacing;
	form::fsizpar(form);
	form.angleOrClipData.angle = gsl::narrow_cast<float>(PI) / 2;
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
			auto& form = FormList->operator[](ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			if (form.type != FRMLINE) {
				fi::fshor(form);
			}
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			auto& form = FormList->operator[](ClosestFormToCursor);
			fi::fshor(form);
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::internal::fsangl(FRMHED& form) {
	clip::delmclp(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	makpoli();
	form.type = FRMFPOLY;
	form.fillColor = gsl::narrow<decltype(form.fillColor)>(ActiveColor);
	form.fillType              = ANGF;
	form.angleOrClipData.angle = gsl::narrow_cast<float>(IniFile.fillAngle);
	form.fillSpacing = LineSpacing;
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
			auto& form = FormList->operator[](selectedForm);
			form::fvars(ClosestFormToCursor);
			if (form.type != FRMLINE) {
				fi::fsangl(form);
			}
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			auto& form = FormList->operator[](ClosestFormToCursor);
			fi::fsangl(form);
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

auto form::chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
	form::fvars(ClosestFormToCursor);
	const auto point = POINT { (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

	const auto& currentForm = FormList->operator[](ClosestFormToCursor);
	NewFormVertexCount = currentForm.vertexCount + 1U;
	thred::duzrat();
	auto rectangle = RECT { 0L, 0L, 0L, 0L };
	form::sRct2px(currentForm.rectangle, rectangle);
	auto& formControls = *FormControlPoints;
	formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = rectangle.left;
	formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = rectangle.top;
	formControls[2].x = formControls[3].x = formControls[4].x = rectangle.right;
	formControls[4].y = formControls[5].y = formControls[6].y = rectangle.bottom;
	formControls[1].x = formControls[5].x = wrap::round<int32_t>(form::midl(rectangle.right, rectangle.left));
	formControls[3].y = formControls[7].y = wrap::round<int32_t>(form::midl(rectangle.top, rectangle.bottom));

	auto minimumLength    = 1e99;
	auto formControlIndex = 0U;
	for (auto iControl : formControls) {
		const auto length = hypot(iControl.x - point.x, iControl.y - point.y);
		if (length < minimumLength) {
			minimumLength             = length;
			SelectedFormControlVertex = formControlIndex;
		}
		if (minimumLength < CLOSENUF) {
			form::ritfrct(ClosestFormToCursor, StitchWindowDC);
			for (auto iCorner = 0U; iCorner < wrap::toUnsigned(stretchBoxLine.size()); iCorner++) {
				stretchBoxLine[iCorner] = formControls[wrap::toSize(iCorner) * 2U];
			}
			stretchBoxLine.back() = stretchBoxLine.front();
			thred::strtchbox(stretchBoxLine);
			if ((SelectedFormControlVertex & 1U) != 0U) {
				StateMap.set(StateFlag::STRTCH);
			}
			else {
				StateMap.set(StateFlag::EXPAND);
				xyRatio = (currentForm.rectangle.right - currentForm.rectangle.left)
				          / (currentForm.rectangle.top - currentForm.rectangle.bottom);
			}
			SelectedFormControlVertex >>= 1U;
			StateMap.set(StateFlag::SHOSTRTCH);
			return true;
		}
		formControlIndex++;
	}
	SelectedFormControlVertex >>= 1U;
	if (point.x >= rectangle.left && point.x <= rectangle.right && point.y >= rectangle.top && point.y <= rectangle.bottom) {
		auto formOrigin = POINT { 0L, 0L };
		auto vertexIt   = std::next(FormVertices->cbegin(), currentForm.vertexIndex);
		form::sfCor2px(vertexIt[0], formOrigin);
		FormMoveDelta = fPOINT { formOrigin.x - point.x, formOrigin.y - point.y };
		StateMap.set(StateFlag::FRMOV);
		return true;
	}
	return false;
}

void form::rstfrm() {
	const auto point
	    = POINT { (Msg.pt.x + wrap::round<int32_t>(FormMoveDelta.x)), (Msg.pt.y + wrap::round<int32_t>(FormMoveDelta.y)) };
	const auto attribute = ClosestFormToCursor << 4U;

	form::setmfrm();
	StateMap.reset(StateFlag::FRMOV);
	thred::pxCor2stch(point);
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	auto       vertexIt = std::next(FormVertices->begin(), currentForm.vertexIndex);
	const auto offset   = fPOINT { SelectedPoint.x - vertexIt[0].x, SelectedPoint.y - vertexIt[0].y };
	for (auto iVertex = 0U; iVertex < currentForm.vertexCount; iVertex++) {
		vertexIt[iVertex].x += offset.x;
		vertexIt[iVertex].y += offset.y;
	}
	currentForm.rectangle.left += offset.x;
	currentForm.rectangle.top += offset.y;
	currentForm.rectangle.right += offset.x;
	currentForm.rectangle.bottom += offset.y;
	for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) == attribute && ((stitch.attribute & ALTYPMSK) != 0U)
		    && ((stitch.attribute & NOTFRM) == 0U)) {
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
	Polyline(StitchWindowDC, FormLines->data(), 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::internal::uncon() {
	if (StateMap.testAndReset(StateFlag::SHOCON)) {
		ducon();
	}
}

void form::drwcon() {
	fi::uncon();
	auto& formLines = *FormLines;
	formLines[1].x  = Msg.pt.x - StitchWindowOrigin.x;
	formLines[1].y  = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOCON);
	fi::ducon();
}

void form::duinsf() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, static_cast<POINT*>(InsertLine), 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::uninsf() {
	if (StateMap.testAndReset(StateFlag::SHOINSF)) {
		form::duinsf();
	}
}

void form::rotfrm(FRMHED& form, uint32_t newStartVertex) {
	form::fvars(ClosestFormToCursor);

	auto vertexIt        = std::next(FormVertices->begin(), CurrentVertexIndex);
	auto rotatedVertices = std::vector<fPOINT> {};
	rotatedVertices.resize(VertexCount);
	auto iRotatedGuide = 0U;
	auto iRotated      = newStartVertex;

	std::copy(vertexIt, std::next(vertexIt, VertexCount), rotatedVertices.begin());
	for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex] = rotatedVertices[iRotated];
		iRotated          = form::nxt(iRotated);
	}
	iRotatedGuide = 0;
	auto guideIt  = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
	if (form.type == SAT) {
		if (form.wordParam != 0U) {
			form.wordParam
			    = (form.wordParam + form.vertexCount - newStartVertex) % form.vertexCount;
		}
		if (VertexCount != 0U) {
			for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
				if (guideIt[iGuide].start != newStartVertex && guideIt[iGuide].finish != newStartVertex) {
					guideIt[iRotatedGuide].start  = (guideIt[iGuide].start + VertexCount - newStartVertex) % VertexCount;
					guideIt[iRotatedGuide].finish = (guideIt[iGuide].finish + VertexCount - newStartVertex) % VertexCount;
					if (guideIt[iRotatedGuide].start > guideIt[iRotatedGuide].finish) {
						std::swap(guideIt[iRotatedGuide].start, guideIt[iRotatedGuide].finish);
					}
					iRotatedGuide++;
				}
			}
		}
	}
	if (iRotatedGuide != 0U) {
		form.satinGuideCount = iRotatedGuide;
		std::sort(guideIt, std::next(guideIt, iRotatedGuide), satin::scomp);
	}
	if (VertexCount != 0U) {
		if ((form.extendedAttribute & AT_STRT) != 0U) {
			form.fillStart = (form.fillStart + VertexCount - newStartVertex) % VertexCount;
		}
		if ((form.extendedAttribute & AT_END) != 0U) {
			form.fillEnd = (form.fillEnd + VertexCount - newStartVertex) % VertexCount;
		}
	}
}

void form::internal::filsfn() {
	auto& form = FormList->operator[](ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	form.type = SAT;
	form.fillColor = gsl::narrow<uint8_t>(ActiveColor);
	form.fillType = SATF;
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
			ClosestFormToCursor = selectedForm;
			const auto& currentForm = FormList->operator[](ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			if (currentForm.type != FRMLINE) {
				fi::filsfn();
			}
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			fi::filsfn();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

auto form::internal::closat(intersectionStyles& inOutFlag) -> bool {
	auto minimumLength = 1e99;

	thred::px2stch();
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		auto& formIter = FormList->operator[](iForm);
		if (formIter.vertexCount != 0U) {
			auto layer = gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formIter.attribute & FRMLMSK) >> 1U);
			if ((ActiveLayer == 0U) || layer == ActiveLayer || ((formIter.attribute & FRMLMSK) == 0U)) {
				CurrentVertexIndex     = formIter.vertexIndex;
				const auto savedVertex = VertexCount;
				VertexCount            = formIter.vertexCount;
				auto lastVertex        = 0U;
				if (formIter.type == FRMLINE) {
					lastVertex = VertexCount - 1U;
				}
				else {
					lastVertex = VertexCount;
				}
				// Loop through for all line segments
				auto length   = 0.0;
				auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < lastVertex; iVertex++) {
					const auto param = findDistanceToSide(vertexIt[iVertex], vertexIt[form::nxt(iVertex)], SelectedPoint, length);
					if ((length < minimumLength)) {
						if ((param < 0.0) && (iVertex == 0)) {
							// this should only happen if the Closest vertex is the start of a line (vertex 0)
							minimumLength         = length;
							ClosestFormToCursor   = iForm;
							ClosestVertexToCursor = iVertex;
							inOutFlag             = intersectionStyles::POINT_BEFORE_LINE;
						}
						else {
							// return the vertex after the intersection
							if ((param > 1.0) && (iVertex == lastVertex - 1)) {
								minimumLength         = length;
								ClosestFormToCursor   = iForm;
								ClosestVertexToCursor = form::nxt(iVertex);
								inOutFlag             = intersectionStyles::POINT_AFTER_LINE;
							}
							else {
								minimumLength         = length;
								ClosestFormToCursor   = iForm;
								ClosestVertexToCursor = form::nxt(iVertex);
								inOutFlag             = intersectionStyles::POINT_IN_LINE;
							}
						}
					}
				}
				VertexCount = savedVertex;
			}
		}
		else {
			OutputDebugString(L"Form Has no vertices!");
		}
	}
	return minimumLength != 1e99;
}

void form::internal::nufpnt(uint32_t vertex, FRMHED& formForInsert) {
	form::fltspac(vertex + 1U, 1U);
	formForInsert.vertexCount++;
	auto vertexIt = std::next(FormVertices->begin(), gsl::narrow_cast<ptrdiff_t>(formForInsert.vertexIndex) + vertex + 1U);
	*vertexIt     = SelectedPoint;
	if (formForInsert.satinGuideCount != 0U) {
		auto guideIt = std::next(SatinGuides->begin(), formForInsert.satinOrAngle.guide);
		for (auto ind = 0U; ind < formForInsert.satinGuideCount; ind++) {
			if (guideIt->start > vertex) {
				guideIt->start++;
			}
			if (guideIt->finish > vertex) {
				guideIt->finish++;
			}
			guideIt++;
		}
	}
	if (formForInsert.wordParam >= vertex + 1U) {
		formForInsert.wordParam++;
		formForInsert.wordParam %= VertexCount;
	}
	if (formForInsert.fillType == CONTF) {
		if (formForInsert.angleOrClipData.guide.start > vertex) {
			formForInsert.angleOrClipData.guide.start++;
		}
		if (formForInsert.angleOrClipData.guide.finish > vertex) {
			formForInsert.angleOrClipData.guide.finish++;
		}
	}
	form::frmlin(formForInsert.vertexIndex, formForInsert.vertexCount);
}

void form::insat() { // insert a point in a form
	// clang-format off
	auto inOutFlag = intersectionStyles::POINT_IN_LINE;
	// clang-format on
	if (fi::closat(inOutFlag)) {
		thred::savdo();
		auto&      selectedForm = FormList->operator[](ClosestFormToCursor);
		const auto lastVertex   = selectedForm.vertexCount - 1U;
		form::fvars(ClosestFormToCursor);
		if (inOutFlag != intersectionStyles::POINT_IN_LINE) {
			if (ClosestVertexToCursor == 0 && selectedForm.type == FRMLINE) {
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				if (ClosestVertexToCursor != lastVertex && selectedForm.type == FRMLINE) {
					ClosestVertexToCursor = form::prv(ClosestVertexToCursor);
				}
			}
			fi::nufpnt(ClosestVertexToCursor, selectedForm);
			auto vertexIt = std::next(FormVertices->begin(), selectedForm.vertexIndex);
			if (StateMap.testAndReset(StateFlag::PRELIN)) {
				std::swap(vertexIt[0], vertexIt[1]);
			}
		}
		else {
			ClosestVertexToCursor = form::prv(ClosestVertexToCursor);
			fi::nufpnt(ClosestVertexToCursor, selectedForm);
		}
		form::refil();
	}
	StateMap.set(StateFlag::RESTCH);
}

auto form::chkdel(const FRMHED& currentForm) noexcept -> bool {
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
		StitchBuffer->resize(&(*iDestination) - &(StitchBuffer->front()));
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (!StateMap.testAndReset(StateFlag::IGNOR) && !UserFlagMap.test(UserFlag::WRNOF)) {
				const auto codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
				for (auto stitch : *StitchBuffer) {
					if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & (USMSK | FRMSK)) == codedForm) {
						displayText::tabmsg(IDS_UNFIL);
						StateMap.set(StateFlag::FILMSG);
						displayText::okcan();
						StateMap.set(StateFlag::IGNOR);
						return;
					}
				}
			}
			const auto codedForm = ClosestFormToCursor << FRMSHFT;
			if (!StitchBuffer->empty()) {
				auto iDestination = StitchBuffer->begin();
				auto destCount    = 0U;
				for (auto& stitch : *StitchBuffer) {
					if ((stitch.attribute & FRMSK) != codedForm || ((stitch.attribute & NOTFRM) != 0U)) {
						*iDestination++ = stitch;
						destCount++;
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
	if (StateMap.test(StateFlag::FRMPSEL)) {
		auto& form = FormList->operator[](ClosestFormToCursor);
		thred::savdo();
		form::rotfrm(form, ClosestVertexToCursor);
		ClosestVertexToCursor = 0;
		satin::satadj(form);
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::rinfrm() {
	form::frmlin(FormForInsert->vertexIndex, FormForInsert->vertexCount);
	SelectObject(StitchWindowMemDC, FormPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	auto& formLines = *FormLines;
	if ((FormVertexNext != 0U) || FormForInsert->type != FRMLINE) {
		Polyline(StitchWindowMemDC, &formLines[FormVertexPrev], 2);
	}
	InsertLine[0]   = formLines[FormVertexPrev];
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOINSF);
	form::duinsf();
}

void form::infrm() { // insert multiple points into a form
	// clang-format off
	auto inOutFlag = intersectionStyles::POINT_IN_LINE;
	// clang-format on
	if (fi::closat(inOutFlag)) {
		FormForInsert = &(FormList->operator[](ClosestFormToCursor));
		form::fvars(ClosestFormToCursor);
		if (inOutFlag != intersectionStyles::POINT_IN_LINE) {
			if ((ClosestVertexToCursor == 0U) && FormForInsert->type == FRMLINE) {
				FormVertexPrev = 0;
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				FormVertexPrev = form::prv(ClosestVertexToCursor);
				FormVertexNext = ClosestVertexToCursor;
			}
		}
		else {
			FormVertexNext = ClosestVertexToCursor;
			FormVertexPrev = form::prv(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::INSFRM);
		StateMap.set(StateFlag::INIT);
		rinfrm();
	}
}

void form::setins() {
	thred::px2stch();
	fi::nufpnt(FormVertexPrev, *FormForInsert);
	if (StateMap.test(StateFlag::PRELIN)) {
		auto vertexIt = std::next(FormVertices->begin(), FormForInsert->vertexIndex);
		std::swap(vertexIt[0], vertexIt[1]);
	}
	else {
		VertexCount    = FormForInsert->vertexCount;
		FormVertexPrev = form::nxt(FormVertexPrev);
		FormVertexNext = form::nxt(FormVertexPrev);
	}
	form::frmlin(FormForInsert->vertexIndex, FormForInsert->vertexCount);
	InsertLine[0]   = (*FormLines)[FormVertexPrev];
	InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
	InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::INSFRM);
	duinsf();
	StateMap.set(StateFlag::RESTCH);
}

void form::bsizpar(FRMHED& currentForm) noexcept {
	currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
	currentForm.edgeStitchLen      = UserStitchLength;
	currentForm.minBorderStitchLen = MinStitchLength;
}

void form::internal::sbord(uint32_t formIndex) {
	form::fvars(ClosestFormToCursor);
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
			form::fvars(ClosestFormToCursor);
			auto& form = FormList->operator[](selectedForm);
			form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
			fi::sbord(selectedForm);
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			auto& form = FormList->operator[](ClosestFormToCursor);
			form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
			fi::sbord(ClosestFormToCursor);
			thred::coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::internal::fsclp(uint32_t formIndex) {
	auto& form = FormList->operator[](formIndex);
	clip::deleclp(formIndex);
	const auto clipSize          = wrap::toUnsigned(ClipBuffer->size());
	form.edgeType       = EDGECLIP;
	form.clipEntries    = clipSize;
	form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
	form.borderSize     = ClipRectSize.cy;
	form.edgeSpacing    = ClipRectSize.cx;
	form.borderColor    = gsl::narrow<uint8_t>(ActiveColor);
	form::bsizpar(form);
	auto offsetStart = std::next(ClipPoints->begin(), form.borderClipData);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	clip::clpout(ClipRectSize.cy / 2);
	form::refilfn();
}

void form::fclp() {
	if (displayText::filmsgs(FML_CLP)) {
		return;
	}
	if (OpenClipboard(ThrEdWindow) != 0) {
		form::fvars(ClosestFormToCursor);
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
						form::fvars(ClosestFormToCursor);
						fi::fsclp(selectedForm);
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::fsclp(ClosestFormToCursor);
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
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
	form::fvars(ClosestFormToCursor);
	auto& form = FormList->operator[](formIndex);
	clip::deleclp(formIndex);
	form.edgeType = EDGEAPPL;
	if (UserFlagMap.test(UserFlag::DUND)) {
		form.edgeType |= EGUND;
	}
	form.edgeSpacing = LineSpacing / 2.0F;
	form.borderSize  = IniFile.borderWidth;
	form::bsizpar(form);
	form.borderColor = gsl::narrow<uint8_t>(ActiveColor | (AppliqueColor << 4U));
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
			form::fvars(ClosestFormToCursor);
			auto& currentForm = FormList->operator[](ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
			}
			else {
				currentForm.attribute &= NOBLNT;
			}
			fi::sapliq(selectedForm);
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			auto& currentForm = FormList->operator[](ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
			}
			else {
				currentForm.attribute &= NOBLNT;
			}
			fi::sapliq(ClosestFormToCursor);
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::setap() {
	std::wstring fmtStr;
	displayText::loadString(fmtStr, IDS_APCOL);

	AppliqueColor = ActiveColor;
	displayText::shoMsg(fmt::format(fmtStr, (AppliqueColor + 1U)));
}

void form::internal::getbig() noexcept {
	AllItemsRect = fRECTANGLE { 1e9F, 0.0F, 0.0F, 1e9F };
	for (auto& iForm : *FormList) {
		const auto& trct = iForm.rectangle;
		if (trct.left < AllItemsRect.left) {
			AllItemsRect.left = trct.left;
		}
		if (trct.top > AllItemsRect.top) {
			AllItemsRect.top = trct.top;
		}
		if (trct.right > AllItemsRect.right) {
			AllItemsRect.right = trct.right;
		}
		if (trct.bottom < AllItemsRect.bottom) {
			AllItemsRect.bottom = trct.bottom;
		}
	}
	for (auto stitch : *StitchBuffer) {
		if (stitch.x < AllItemsRect.left) {
			AllItemsRect.left = stitch.x;
		}
		if (stitch.y > AllItemsRect.top) {
			AllItemsRect.top = stitch.y;
		}
		if (stitch.x > AllItemsRect.right) {
			AllItemsRect.right = stitch.x;
		}
		if (stitch.y < AllItemsRect.bottom) {
			AllItemsRect.bottom = stitch.y;
		}
	}
}

void form::stchrct2px(const fRECTANGLE& stitchRect, RECT& screenRect) {
	auto stitchCoord = fPOINT { stitchRect.left, stitchRect.top };
	auto screenCoord = POINT { 0L, 0L };

	thred::sCor2px(stitchCoord, screenCoord);
	screenRect.left = screenCoord.x;
	screenRect.top  = screenCoord.y;
	stitchCoord.x   = stitchRect.right;
	stitchCoord.y   = stitchRect.bottom;
	thred::sCor2px(stitchCoord, screenCoord);
	screenRect.right  = screenCoord.x;
	screenRect.bottom = screenCoord.y;
}

void form::selal() {
	StateMap.reset(StateFlag::FORMSEL);
	SelectedFormList->clear();
	StateMap.reset(StateFlag::SELBOX);
	StateMap.reset(StateFlag::GRPSEL);
	fi::getbig();
	ZoomRect   = fRECTANGLE { 0.0F, gsl::narrow_cast<float>(UnzoomedRect.y), gsl::narrow_cast<float>(UnzoomedRect.x), 0.0F };
	ZoomFactor = 1;
	StateMap.reset(StateFlag::ZUMED);
	thred::movStch();
	NearestCount = 0;
	StateMap.reset(StateFlag::RUNPAT);
	thred::duzrat();
	form::stchrct2px(AllItemsRect, SelectedFormsRect);
	StateMap.set(StateFlag::BIGBOX);
	StateMap.set(StateFlag::RESTCH);
}

void form::setstrtch() {
	auto reference  = 0.0F;
	auto ratio      = 1.0F;
	auto stitchRect = fRECTANGLE {};

	thred::savdo();
	if (StateMap.test(StateFlag::FPSEL)) {
		stitchRect = SelectedVerticesRect;
	}
	else {
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX)) {
			form::pxrct2stch(SelectedFormsRect, stitchRect);
		}
		else {
			form::fvars(ClosestFormToCursor);
			thred::px2stch();
			stitchRect = fRECTANGLE { 0.0F, 0.0F, 0.0F, 0.0F };
		}
	}
	switch (SelectedFormControlVertex) {
	case 0: { // top control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.bottom;
			const auto offsetY = Msg.pt.y - StitchWindowOrigin.y;
			ratio              = gsl::narrow_cast<float>(SelectedFormsRect.bottom - offsetY)
			        / gsl::narrow_cast<float>(SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
				reference                   = formRectangle.bottom;
				ratio                       = (SelectedPoint.y - reference) / (formRectangle.top - reference);
				formRectangle.top = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.bottom;
				ratio     = (SelectedPoint.y - reference) / (StitchRangeRect.top - reference);
			}
		}
		break;
	}
	case 1: { // right control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.left;
			const auto offsetX = Msg.pt.x - StitchWindowOrigin.x;
			ratio              = gsl::narrow_cast<float>(offsetX - SelectedFormsRect.left)
			        / gsl::narrow_cast<float>(SelectedFormsRect.right - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
				reference                     = formRectangle.left;
				ratio                         = (SelectedPoint.x - reference) / (formRectangle.right - reference);
				formRectangle.right = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.left;
				ratio     = (SelectedPoint.x - reference) / (StitchRangeRect.right - reference);
			}
		}
		break;
	}
	case 2: { // bottom control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.top;
			const auto offsetY = Msg.pt.y - StitchWindowOrigin.y;
			ratio              = gsl::narrow_cast<float>(offsetY - SelectedFormsRect.top)
			        / gsl::narrow_cast<float>(SelectedFormsRect.bottom - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
				reference                      = formRectangle.top;
				ratio                          = (SelectedPoint.y - reference) / (formRectangle.bottom - reference);
				formRectangle.bottom = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.top;
				ratio     = (SelectedPoint.y - reference) / (StitchRangeRect.bottom - reference);
			}
		}
		break;
	}
	case 3: { // left control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.right;
			const auto offsetX = Msg.pt.x - StitchWindowOrigin.x;
			ratio              = gsl::narrow_cast<float>(SelectedFormsRect.right - offsetX)
			        / gsl::narrow_cast<float>(SelectedFormsRect.right - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
				reference                    = formRectangle.right;
				ratio                        = (SelectedPoint.x - reference) / (formRectangle.left - reference);
				formRectangle.left = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.right;
				ratio     = (SelectedPoint.x - reference) / (StitchRangeRect.left - reference);
			}
		}
		break;
	}
	default: {
		throw; // we should never reach here
	}
	}
	if ((SelectedFormControlVertex & 1U) != 0U) { // right or left control selected
		if (StateMap.test(StateFlag::FPSEL)) {
			form::fvars(ClosestFormToCursor);
			auto currentVertex = SelectedFormVertices.start;
			auto vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
			for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
				vertexIt[currentVertex].x = (vertexIt[currentVertex].x - reference) * ratio + reference;
				currentVertex             = pdir(currentVertex);
			}
			form::frmout(ClosestFormToCursor);
			thred::setpsel();
			refil();
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
				auto& formIter     = FormList->operator[](iForm);
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
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
				auto& formIter     = FormList->operator[](selectedForm);
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
					vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
				}
			}
			else {
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					(*StitchBuffer)[iStitch].x = ((*StitchBuffer)[iStitch].x - reference) * ratio + reference;
				}
			}
		}
	}
	else {
		if (StateMap.test(StateFlag::FPSEL)) {
			form::fvars(ClosestFormToCursor);
			auto currentVertex = SelectedFormVertices.start;
			auto vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
			for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
				vertexIt[currentVertex].y = (vertexIt[currentVertex].y - reference) * ratio + reference;
				currentVertex             = pdir(currentVertex);
			}
			form::frmout(ClosestFormToCursor);
			thred::setpsel();
			refil();
			StateMap.set(StateFlag::RESTCH);
			return;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
				auto& formIter     = FormList->operator[](iForm);
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
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
				auto& formIter     = FormList->operator[](selectedForm);
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
				for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
					vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
				}
			}
			else {
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					(*StitchBuffer)[iStitch].y = ((*StitchBuffer)[iStitch].y - reference) * ratio + reference;
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
	else if (StateMap.test(StateFlag::FORMSEL)) {
		refil();
	}
	StateMap.set(StateFlag::RESTCH);
}

void form::setexpand(float xyRatio) {
	auto size0     = fPOINT {};
	auto rectangle = fRECTANGLE {};

	thred::savdo();
	if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		rectangle       = fRECTANGLE { gsl::narrow_cast<float>(SelectedFormsRect.left),
                                 gsl::narrow_cast<float>(SelectedFormsRect.top),
                                 gsl::narrow_cast<float>(SelectedFormsRect.right),
                                 gsl::narrow_cast<float>(SelectedFormsRect.bottom) };
		SelectedPoint.x = gsl::narrow_cast<float>(Msg.pt.x - StitchWindowOrigin.x);
		SelectedPoint.y = gsl::narrow_cast<float>(Msg.pt.y - StitchWindowOrigin.y);
		size0.y         = rectangle.bottom - rectangle.top;
	}
	else {
		thred::px2stch();
		form::fvars(ClosestFormToCursor);
		if (StateMap.test(StateFlag::FORMSEL)) {
			rectangle = FormList->operator[](ClosestFormToCursor).rectangle;
		}
		else {
			rectangle = StitchRangeRect;
		}
		size0.y = rectangle.top - rectangle.bottom;
	}
	auto ratio     = fPOINT { 1.0F, 1.0F };
	auto reference = fPOINT {};
	size0.x        = rectangle.right - rectangle.left;
	switch (SelectedFormControlVertex) {
	case 0: {
		reference.x       = rectangle.right;
		reference.y       = rectangle.bottom;
		auto       size1  = fPOINT { fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
		const auto aspect = size1.x / size1.y;
		if (aspect < xyRatio) {
			size1.x = size1.y * xyRatio;
		}
		else {
			size1.y = size1.x / xyRatio;
		}
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (SelectedFormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
			auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
			formRectangle.left = rectangle.right - size1.x;
			formRectangle.top  = rectangle.bottom + size1.y;
		}
		break;
	}
	case 1: {
		reference.x       = rectangle.left;
		reference.y       = rectangle.bottom;
		auto       size1  = fPOINT { fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
		const auto aspect = size1.x / size1.y;
		if (aspect < xyRatio) {
			size1.x = size1.y * xyRatio;
		}
		else {
			size1.y = size1.x / xyRatio;
		}
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (SelectedFormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
			auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
			formRectangle.right = rectangle.left + size1.x;
			formRectangle.top   = rectangle.bottom + size1.y;
		}
		break;
	}
	case 2: {
		reference.x       = rectangle.left;
		reference.y       = rectangle.top;
		auto       size1  = fPOINT { fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
		const auto aspect = size1.x / size1.y;
		if (aspect < xyRatio) {
			size1.x = size1.y * xyRatio;
		}
		else {
			size1.y = size1.x / xyRatio;
		}
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (SelectedFormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
			auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
			formRectangle.right  = rectangle.left + size1.x;
			formRectangle.bottom = rectangle.top - size1.y;
		}
		break;
	}
	case 3: {
		reference.x       = rectangle.right;
		reference.y       = rectangle.top;
		auto       size1  = fPOINT { fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
		const auto aspect = size1.x / size1.y;
		if (aspect < xyRatio) {
			size1.x = size1.y * xyRatio;
		}
		else {
			size1.y = size1.x / xyRatio;
		}
		ratio.x = size1.x / size0.x;
		ratio.y = size1.y / size0.y;
		if (SelectedFormList->empty() && StateMap.test(StateFlag::FORMSEL)) {
			auto& formRectangle = FormList->operator[](ClosestFormToCursor).rectangle;
			formRectangle.left   = rectangle.right - size1.x;
			formRectangle.bottom = rectangle.top - size1.y;
		}
		break;
	}
	default: {
		throw; // we should never reach here
	}
	}
	const auto integerReference = POINT { wrap::round<int32_t>(reference.x), wrap::round<int32_t>(reference.y) };
	auto       stitchReference  = fPOINT {};
	fi::px2stchf(integerReference, stitchReference);
	if (StateMap.test(StateFlag::FPSEL)) {
		form::fvars(ClosestFormToCursor);
		auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
		auto iCurrent = SelectedFormVertices.start;
		for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexIt[iCurrent].x = (vertexIt[iCurrent].x - stitchReference.x) * ratio.x + stitchReference.x;
			vertexIt[iCurrent].y = (vertexIt[iCurrent].y - stitchReference.y) * ratio.y + stitchReference.y;
			iCurrent             = form::pdir(iCurrent);
		}
		thred::setpsel();
		form::frmout(ClosestFormToCursor);
		form::refil();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			form::fvars(iForm);
			auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
			const auto& formVertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
			for (auto iVertex = 0U; iVertex < formVertexCount; iVertex++) {
				vertexIt[iVertex].x = (vertexIt[iVertex].x - stitchReference.x) * ratio.x + stitchReference.x;
				vertexIt[iVertex].y = (vertexIt[iVertex].y - stitchReference.y) * ratio.y + stitchReference.y;
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
			form::fvars(selectedForm);
			auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
			const auto& formVertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
			for (auto iVertex = 0U; iVertex < formVertexCount; iVertex++) {
				vertexIt[iVertex].x = (vertexIt[iVertex].x - stitchReference.x) * ratio.x + stitchReference.x;
				vertexIt[iVertex].y = (vertexIt[iVertex].y - stitchReference.y) * ratio.y + stitchReference.y;
			}
			form::frmout(selectedForm);
			ClosestFormToCursor = selectedForm;
			form::refil();
		}
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
			for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].x = (vertexIt[iVertex].x - reference.x) * ratio.x + reference.x;
				vertexIt[iVertex].y = (vertexIt[iVertex].y - reference.y) * ratio.y + reference.y;
			}
			form::refil();
		}
		else {
			for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
				(*StitchBuffer)[iStitch].x = ((*StitchBuffer)[iStitch].x - reference.x) * ratio.x + reference.x;
				(*StitchBuffer)[iStitch].y = ((*StitchBuffer)[iStitch].y - reference.y) * ratio.y + reference.y;
			}
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::nufilcol(uint32_t color) {
	auto& formColor = FormList->operator[](ClosestFormToCursor).fillColor;
	auto newColor = gsl::narrow<uint8_t>(color);
	if (formColor != newColor) {
		formColor = newColor;
		const auto attribute    = (ClosestFormToCursor << FRMSHFT) | FRMFIL;
		for (auto& stitch : *StitchBuffer) {
			if ((stitch.attribute & (FRMSK | TYPMSK | FTHMSK)) == attribute) {
				stitch.attribute &= NCOLMSK;
				stitch.attribute |= color;
			}
		}
	}
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::nufthcol(uint32_t color) {
	auto& formColor = FormList->operator[](ClosestFormToCursor).fillInfo.feather.color;
	if (formColor != gsl::narrow<uint8_t>(color)) {
		formColor = gsl::narrow<uint8_t>(color);
		const auto attribute                 = (ClosestFormToCursor << FRMSHFT) | FTHMSK;
		for (auto& stitch : *StitchBuffer) {
			if ((stitch.attribute & (FRMSK | FTHMSK)) == attribute) {
				stitch.attribute &= NCOLMSK;
				stitch.attribute |= color;
			}
		}
	}
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::nubrdcol(uint32_t color) {
	FormList->operator[](ClosestFormToCursor).borderColor = gsl::narrow<uint8_t>(color);
	const auto attribute      = (ClosestFormToCursor << FRMSHFT) | FRMBFIL;
	for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & (FRMSK | TYPMSK)) == attribute) {
			stitch.attribute &= NCOLMSK;
			stitch.attribute |= color;
		}
	}
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::nulapcol(uint32_t color) {
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	if (gsl::narrow<uint32_t>(currentForm.borderColor >> FRMSHFT) != color) {
		currentForm.borderColor &= COLMSK;
		currentForm.borderColor |= color << FRMSHFT;
		const auto attribute = (ClosestFormToCursor << FRMSHFT) | TYPMSK;
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
	form::fvars(ClosestFormToCursor);
	clip::deleclp(formIndex);
	form.edgeType    = EDGEBEAN;
	form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
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
			form::fvars(ClosestFormToCursor);
			fi::sbold(selectedForm);
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fi::sbold(ClosestFormToCursor);
			thred::coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
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
	if (starCount < 3) {
		starCount = 3;
	}
	if (starCount > 100U) {
		starCount = 100U;
	}
	const auto stepAngle   = PI_F / gsl::narrow_cast<float>(starCount);
	auto       angle       = stepAngle / 2.0F + PI_F;
	const auto vertexCount = starCount * 2U;
	auto       newForm     = FRMHED {};
	newForm.vertexIndex    = thred::adflt(vertexCount);
	newForm.vertexCount    = vertexCount;
	newForm.attribute      = gsl::narrow<uint8_t>(ActiveLayer << 1U);
	newForm.type           = FRMFPOLY;
	FormList->push_back(newForm);
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point = SelectedPoint;
	StateMap.set(StateFlag::FILDIR);
	auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0U; iVertex < vertexCount; iVertex++) {
		vertexIt[iVertex].x = point.x;
		vertexIt[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	const auto center
	    = fPOINT { form::midl(vertexIt[starCount].x, vertexIt[0].x), form::midl(vertexIt[starCount].y, vertexIt[0].y) };
	for (auto iVertex = 1U; iVertex < vertexCount; iVertex += 2) {
		vertexIt[iVertex].x = (vertexIt[iVertex].x - center.x) * StarRatio + center.x;
		vertexIt[iVertex].y = (vertexIt[iVertex].y - center.y) * StarRatio + center.y;
	}
	form::frmout(ClosestFormToCursor);
	FormMoveDelta      = fPOINT {};
	NewFormVertexCount = vertexCount + 1U;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::duspir(uint32_t stepCount) {
	if (stepCount < 3) {
		stepCount = 3;
	}
	if (stepCount > 100U) {
		stepCount = 100U;
	}
	const auto stepAngle = PI_F * 2.0F / gsl::narrow_cast<float>(stepCount);
	const auto length    = 800.0F / gsl::narrow_cast<float>(stepCount) * ZoomFactor
	                    * gsl::narrow_cast<float>(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	auto newForm        = FRMHED {};
	auto vertexCount    = wrap::round<uint32_t>(gsl::narrow_cast<float>(stepCount) * SpiralWrap);
	newForm.vertexIndex = thred::adflt(vertexCount);
	newForm.vertexCount = vertexCount;
	newForm.attribute   = gsl::narrow<uint8_t>(ActiveLayer << 1U);
	FormList->push_back(newForm);
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	auto firstSpiral    = std::vector<fPOINT> {};
	firstSpiral.resize(stepCount);
	auto centeredSpiral = std::vector<fPOINT> {};
	centeredSpiral.resize(stepCount);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point = SelectedPoint;
	auto angle = 0.0F;
	for (auto iStep = 0U; iStep < stepCount; iStep++) {
		firstSpiral[iStep].x = point.x;
		firstSpiral[iStep].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	const auto center = fPOINT { form::midl(firstSpiral[stepCount / 2].x, firstSpiral[0].x),
		                        form::midl(firstSpiral[stepCount / 2].y, firstSpiral[0].y) };
	for (auto iStep = 0U; iStep < stepCount; iStep++) {
		centeredSpiral[iStep].x = firstSpiral[iStep].x - center.x;
		centeredSpiral[iStep].y = firstSpiral[iStep].y - center.y;
	}
	const auto stepRatio = 1.0F / vertexCount;
	auto       ratio     = stepRatio;
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	auto       vertexIt  = std::next(FormVertices->begin(), currentForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < vertexCount; iVertex++) {
		vertexIt[iVertex].x = centeredSpiral[iVertex % stepCount].x * ratio + center.x;
		vertexIt[iVertex].y = centeredSpiral[iVertex % stepCount].y * ratio + center.y;
		ratio += stepRatio;
	}
	currentForm.type = FRMLINE;
	form::frmout(ClosestFormToCursor);
	FormMoveDelta      = fPOINT {};
	NewFormVertexCount = vertexCount + 1U;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::duhart(uint32_t sideCount) {
	if (sideCount > 100U) {
		sideCount = 100U;
	}
	if (sideCount < 6U) {
		sideCount = 6U;
	}
	FormList->push_back(FRMHED {});
	auto& currentForm         = FormList->back();
	currentForm.vertexIndex = gsl::narrow<decltype(currentForm.vertexIndex)>(FormVertices->size());
	currentForm.attribute   = gsl::narrow<uint8_t>(ActiveLayer << 1U);
	FormVertices->reserve(FormVertices->size() + wrap::toSize(sideCount) * 2U - 2U);
	CurrentVertexIndex = gsl::narrow<decltype(CurrentVertexIndex)>(FormVertices->size());
	thred::px2stch();
	auto       point     = SelectedPoint;
	auto       stepAngle = PI_F * 2.0F / gsl::narrow_cast<float>(sideCount);
	const auto length    = 300.0F / gsl::narrow_cast<float>(sideCount) * ZoomFactor
	                    * gsl::narrow_cast<float>(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	auto angle    = PI_F * 0.28F;
	auto iVertex  = 0U;
	auto maximumX = 0.0F;
	while (angle > -PI_F * 0.7F) {
		if (point.x > maximumX) {
			maximumX = point.x;
		}
		FormVertices->push_back(point);
		iVertex++;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	stepAngle /= 4.5F;
	auto lastVertex = iVertex;
	auto vertexIt   = std::next(FormVertices->begin(), CurrentVertexIndex);
	while (point.x > vertexIt[0].x && iVertex < 200U) {
		FormVertices->push_back(fPOINT { point.x, point.y });
		iVertex++;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	const auto firstVertex = iVertex;
	const auto ratio
	    = (vertexIt[lastVertex - 1U].x - vertexIt[0].x) / (vertexIt[lastVertex - 1U].x - vertexIt[firstVertex - 1U].x);
	for (iVertex = lastVertex; iVertex < firstVertex; iVertex++) {
		vertexIt[iVertex].x = (vertexIt[iVertex].x - vertexIt[lastVertex - 1U].x) * ratio + vertexIt[lastVertex - 1U].x;
	}
	auto iDestination = iVertex;
	lastVertex        = iVertex;
	for (iVertex = lastVertex - 2U; iVertex != 0; iVertex--) {
		FormVertices->push_back(
		    fPOINT { maximumX + maximumX - vertexIt[iVertex].x - 2 * (maximumX - vertexIt[0].x), vertexIt[iVertex].y });
		iDestination++;
	}
	NewFormVertexCount        = iDestination + 1U;
	currentForm.vertexCount = iDestination;
	currentForm.type        = FRMFPOLY;
	ClosestFormToCursor       = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	form::frmout(ClosestFormToCursor);
	FormMoveDelta = fPOINT {};
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
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
	const auto steps = sides >> 1U;
	// Calculate the angle subtended by the arc of the lens which is determined by the aspect ratio
	const auto theta = std::atan(1.0F / IniFile.lensRatio);
	// now calculate the radius of the lens arc and scale by the zoom factor
	const auto radius
	    = (100.0F / std::sin(theta)) * ZoomFactor * gsl::narrow_cast<float>(UnzoomedRect.x + UnzoomedRect.y) / (LHUPX + LHUPY);
	// get the angle subtended by each step
	const auto omega = 2.0F * theta / steps;
	// and the interior angle of every segment
	const auto phi = (PI_F - omega) / 2.0F;
	// as well as the length of every segment
	const auto stepLength = 2.0F * radius * std::cos(phi);
	FormList->push_back(FRMHED {});
	auto& currentForm         = FormList->back();
	ClosestFormToCursor       = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	currentForm.vertexIndex = gsl::narrow<decltype(currentForm.vertexIndex)>(FormVertices->size());
	currentForm.attribute   = ActiveLayer << 1U;
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point   = SelectedPoint;
	auto iVertex = 0U;
	FormVertices->reserve(FormVertices->size() + wrap::toSize(steps << 1U) + 1U);
	// rho is the angle of this particular segment
	auto rho = PI_F - theta - phi;
	for (auto iStep = 0U; iStep < steps; iStep++) {
		FormVertices->push_back(point);
		point.x += stepLength * cos(rho);
		point.y += stepLength * sin(rho);
		rho += omega;
		iVertex++;
	}
	FormVertices->push_back(point);
	auto       vertexIt = std::next(FormVertices->begin(), gsl::narrow_cast<ptrdiff_t>(currentForm.vertexIndex + iVertex - 1U));
	const auto av       = SelectedPoint.x;
	for (; iVertex != 1; iVertex--) {
		FormVertices->push_back(fPOINT { av + av - (*vertexIt).x, (*vertexIt).y });
		vertexIt--;
	}
	currentForm.vertexCount
	    = gsl::narrow<decltype(currentForm.vertexCount)>(FormVertices->size() - currentForm.vertexIndex);
	NewFormVertexCount = currentForm.vertexCount + 1U;
	currentForm.type = FRMFPOLY;
	form::frmout(ClosestFormToCursor);
	FormMoveDelta = fPOINT {};
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

constexpr auto form::internal::shreg(float highValue, float reference, float eggRatio) noexcept -> float {
	return (highValue - reference) * eggRatio + reference;
}

void form::dueg(uint32_t sides) {
	if (sides < 8U) {
		sides = 8U;
	}
	form::durpoli(sides);
	auto& form = FormList->back();

	auto       vertexIt  = std::next(FormVertices->begin(), form.vertexIndex);
	const auto reference = form::midl(vertexIt[sides / 2].y, vertexIt[0].y);
	const auto maximumY  = vertexIt[sides >> 2U].y - vertexIt[0].y;

	for (uint32_t iVertex = 0; iVertex < sides; iVertex++) {
		if (vertexIt[iVertex].y < reference) {
			vertexIt[iVertex].y = reference - (reference - vertexIt[iVertex].y) * IniFile.eggRatio;
		}
	}
	const auto eggRatio = maximumY / (vertexIt[sides >> 2U].y - vertexIt[0].y);
	for (uint32_t iVertex = 1; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex].x = fi::shreg(vertexIt[iVertex].x, vertexIt[0].x, eggRatio);
		vertexIt[iVertex].y = fi::shreg(vertexIt[iVertex].y, vertexIt[0].y, eggRatio);
	}
}

void form::duzig(uint32_t vertices) {
	if (vertices < 3) {
		vertices = 3;
	}
	if (vertices > 100U) {
		vertices = 100U;
	}
	auto newForm        = FRMHED {};
	newForm.vertexIndex = thred::adflt(vertices);
	newForm.vertexCount = vertices;
	newForm.attribute   = gsl::narrow<uint8_t>(ActiveLayer << 1U);
	FormList->push_back(newForm);
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	const auto offset   = fPOINT { UnzoomedRect.x / 6.0, UnzoomedRect.y / (6.0 * vertices) };
	auto       vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0U; iVertex < vertices; iVertex++) {
		vertexIt[iVertex] = SelectedPoint;
		SelectedPoint.y -= offset.y;
		if ((iVertex & 1U) != 0U) {
			SelectedPoint.x += offset.x;
		}
		else {
			SelectedPoint.x -= offset.x;
		}
	}
	FormList->back().type = FRMLINE;
	form::frmout(ClosestFormToCursor);
	FormMoveDelta      = fPOINT {};
	NewFormVertexCount = vertices + 1U;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::fliph() {
	form::fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		thred::savdo();
		const auto offset        = SelectedVerticesRect.right + SelectedVerticesRect.left;
		auto       currentVertex = SelectedFormVertices.start;
		auto       vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
		for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexIt[currentVertex].x = offset - vertexIt[currentVertex].x;
			currentVertex             = form::pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		const auto offset = AllItemsRect.right + AllItemsRect.left;
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
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (!SelectedFormList->empty()) {
		thred::savdo();
		auto formMap = boost::dynamic_bitset<>(FormList->size());

		auto rectangle = fRECTANGLE {};
		form::pxrct2stch(SelectedFormsRect, rectangle);
		const auto offset = rectangle.right + rectangle.left;
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			formMap.set(ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
			for (auto iVertex = 0U; iVertex < FormList->operator[](ClosestFormToCursor).vertexCount; iVertex++) {
				vertexIt[iVertex].x = offset - vertexIt[iVertex].x;
			}
			form::frmout(ClosestFormToCursor);
		}
		for (auto& stitch : *StitchBuffer) {
			const auto decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && ((stitch.attribute & NOTFRM) == 0U)) {
				stitch.x = offset - stitch.x;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			const auto& srcRect = FormList->operator[](ClosestFormToCursor).rectangle;
			const auto offset   = srcRect.right + srcRect.left;
			auto       vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
			for (auto iVertex = 0U; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].x = offset - vertexIt[iVertex].x;
			}
			for (auto& stitch : *StitchBuffer) {
				if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor && ((stitch.attribute & NOTFRM) == 0U)) {
					stitch.x = offset - stitch.x;
				}
			}
			form::frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				auto rectangle = fRECTANGLE {};
				thred::selRct(rectangle);
				const auto offset = rectangle.right + rectangle.left;
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					(*StitchBuffer)[iStitch].x = offset - (*StitchBuffer)[iStitch].x;
				}
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::internal::prpsbrd(uint32_t formIndex) {
	auto& form = FormList->operator[](formIndex);
	if (form.vertexCount > 2) {
		clip::deleclp(formIndex);
		form.edgeType = EDGEPROPSAT;
		if (UserFlagMap.test(UserFlag::DUND)) {
			form.edgeType |= EGUND;
		}
		form::bsizpar(form);
		form.borderSize  = BorderWidth;
		form.edgeSpacing = LineSpacing;
		form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
		form::refilfn();
	}
}

void form::prpbrd(float borderStitchSpacing) {
	const auto savedSpacing = LineSpacing;

	if (displayText::filmsgs(FML_PRPS)) {
		return;
	}
	LineSpacing = borderStitchSpacing;
	if (!SelectedFormList->empty()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			auto& currentForm = FormList->operator[](ClosestFormToCursor);
			currentForm.borderSize = LineSpacing;
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
			}
			else {
				currentForm.attribute &= NOBLNT;
			}
			fi::prpsbrd(selectedForm);
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			auto& currentForm = FormList->operator[](ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				currentForm.attribute |= gsl::narrow<decltype(currentForm.attribute)>(SBLNT | FBLNT);
			}
			else {
				currentForm.attribute &= NOBLNT;
			}
			fi::prpsbrd(ClosestFormToCursor);
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void form::tglfrm() {
	if (StateMap.testAndReset(StateFlag::SATPNT)) {
		satin::satfix();
	}
	StateMap.reset(StateFlag::HIDSTCH);
	if (StateMap.testAndFlip(StateFlag::FRMOF)) {
		FormOnOff->assign((*StringTable)[STR_FRMPLUS]);
	}
	else {
		FormOnOff->assign((*StringTable)[STR_FRMINUS]);
		StateMap.reset(StateFlag::FORMSEL);
		StateMap.reset(StateFlag::FORMIN);
		StateMap.reset(StateFlag::MOVFRM);
		StateMap.reset(StateFlag::FRMPMOV);
		StateMap.reset(StateFlag::FRMOV);
		StateMap.reset(StateFlag::SATIN);
		StateMap.reset(StateFlag::SATPNT);
		StateMap.reset(StateFlag::SATCNKT);
		StateMap.reset(StateFlag::FENDIN);
		StateMap.reset(StateFlag::DELFRM);
		StateMap.reset(StateFlag::FRMPSEL);
		StateMap.reset(StateFlag::INSFRM);
	}
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
	StateMap.set(StateFlag::DUMEN);
	StateMap.set(StateFlag::RESTCH);
}

void form::frmon() {
	thred::unbsho();
	StateMap.reset(StateFlag::FRMOF);
	FormOnOff->assign((*StringTable)[STR_FRMPLUS]);
	SetMenuItemInfo(MainMenu, ID_FRMOF, FALSE, MenuInfo);
	StateMap.set(StateFlag::DUMEN);
}

void form::internal::fnord() {
	form::fvars(ClosestFormToCursor);
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	auto vertexIt = std::next(FormVertices->begin(), currentForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < (currentForm.vertexCount / 2); iVertex++) {
		std::swap(vertexIt[iVertex], vertexIt[currentForm.vertexCount - iVertex - 1U]);
	}
	form::refil();
}

void form::flpord() {
	auto iVertex  = 0U;
	auto iForward = 0U;
	auto start    = 0U;
	auto finish   = 0U;

	form::fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		thred::savdo();
		start         = SelectedFormVertices.start;
		finish        = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
		auto vertexIt = std::next(FormVertices->begin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
		for (iVertex = 0; iVertex <= (SelectedFormVertices.vertexCount / 2); iVertex++) {
			std::swap(vertexIt[start], vertexIt[finish]);
			start = form::pdir(start);
			StateMap.flip(StateFlag::PSELDIR);
			finish = form::pdir(finish);
			StateMap.flip(StateFlag::PSELDIR);
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
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			fi::fnord();
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				iForward             = GroupStartStitch;
				const auto endStitch = ((GroupEndStitch - GroupStartStitch) / 2U) + 1U;
				for (auto iStitch = 0U; iStitch < endStitch; iStitch++) {
					std::swap((*StitchBuffer)[iForward], (*StitchBuffer)[GroupEndStitch - iStitch]);
					iForward++;
				}
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::internal::filsclp() {
	texture::deltx(ClosestFormToCursor);
	form::fvars(ClosestFormToCursor);
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	if (currentForm.type != SAT) {
		currentForm.wordParam = 0;
	}
	currentForm.type                    = SAT;
	currentForm.fillType                = CLPF;
	currentForm.angleOrClipData.clip    = clip::numclp();
	currentForm.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
	auto offsetStart                      = std::next(ClipPoints->begin(), currentForm.angleOrClipData.clip);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
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
						form::fvars(ClosestFormToCursor);
						if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
							fi::filsclp();
						}
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::filsclp();
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
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

void form::internal::snpfn(const std::vector<uint32_t>& xPoints, uint32_t start, uint32_t end, uint32_t finish) noexcept {
	if (finish != start) {
		for (auto current = start; current < end; current++) {
			auto reference = xPoints[current];
			for (auto iPoint = current + 1U; iPoint < finish; iPoint++) {
				auto       check       = xPoints[iPoint];
				const auto CheckLength = hypot((*StitchBuffer)[check].x - (*StitchBuffer)[reference].x,
				                               (*StitchBuffer)[check].y - (*StitchBuffer)[reference].y);
				if (CheckLength < SnapLength) {
					(*StitchBuffer)[check] = (*StitchBuffer)[reference];
				}
			}
		}
	}
}

void form::internal::doTimeWindow(float rangeX, const std::vector<uint32_t>& xPoints, const std::vector<uint32_t>& xHistogram) {
	auto checkLength = wrap::round<uint32_t>(SnapLength * 2.0F + 1.0F);

	auto timeWindow = CreateWindow(L"STATIC", // NOLINT
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

	auto       timeDC       = GetDC(timeWindow);
	const auto timeStep     = gsl::narrow_cast<double>(StitchWindowSize.x) / rangeX;
	auto       timePosition = 0.0;
	auto&      formLines    = *FormLines;
	formLines.clear();
	formLines.push_back(POINT { 0, 0 });
	formLines.push_back(POINT { 0, ButtonHeight });
	SelectObject(timeDC, UserPen[0]);
	for (auto iColumn = 1U; iColumn < wrap::round<uint32_t>(rangeX) - checkLength - 1U; iColumn++) {
		snpfn(xPoints,
		      xHistogram[iColumn],
		      xHistogram[wrap::toSize(iColumn) + 1U],
		      xHistogram[wrap::toSize(iColumn) + checkLength]);
		Polyline(timeDC, formLines.data(), 2);
		timePosition += timeStep;
		formLines[0].x = formLines[1].x = wrap::round<int32_t>(timePosition);
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

	const auto attribute = (ClosestFormToCursor << 4U) & FRMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			if ((((*StitchBuffer)[iStitch].attribute & NOTFRM) == 0U)
			    && ((*StitchBuffer)[iStitch].attribute & FRMSK) == attribute) {
				auto iColumn = wrap::floor<uint32_t>((*StitchBuffer)[iStitch].x);
				xHistogram[iColumn]++;
			}
		}
	}
	else {
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			auto iColumn = wrap::floor<uint32_t>((*StitchBuffer)[iStitch].x);
			xHistogram[iColumn]++;
		}
	}
	auto accumulator = 0U;
	auto endColumn   = wrap::round<uint32_t>(range.x);
	for (auto iColumn = 0U; iColumn < endColumn; iColumn++) {
		auto value          = xHistogram[iColumn];
		xHistogram[iColumn] = accumulator;
		accumulator += value;
	}
	xHistogram[endColumn] = accumulator;
	if (StateMap.test(StateFlag::FORMSEL)) {
		auto iStitch = 0U;
		for (auto& stitch : *StitchBuffer) {
			if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == attribute) {
				auto iColumn                   = wrap::floor<uint32_t>(stitch.x);
				xPoints[xHistogram[iColumn]++] = iStitch;
			}
			iStitch++;
		}
	}
	else {
		auto iStitch = 0U;
		for (auto& stitch : *StitchBuffer) {
			auto iColumn                   = wrap::floor<uint32_t>(stitch.x);
			xPoints[xHistogram[iColumn]++] = iStitch;
			iStitch++;
		}
	}
	doTimeWindow(range.x, xPoints, xHistogram);
}

void form::snap() {
	thred::savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		fi::snp(GroupStartStitch, GroupEndStitch);
	}
	else {
		fi::snp(0, wrap::toUnsigned(StitchBuffer->size()));
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::dufcntr(fPOINT& center) {
	auto bigRect = FormList->operator[](SelectedFormList->front()).rectangle;
	for (auto selectedForm : (*SelectedFormList)) {
		const auto formRect = FormList->operator[](selectedForm).rectangle;
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
	center = fPOINT { form::midl(bigRect.right, bigRect.left), form::midl(bigRect.top, bigRect.bottom) };
}

auto form::rotpar() -> fPOINT {
	auto rotationCenter = fPOINT {};
	do {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			RotationRect = FormList->operator[](ClosestFormToCursor).rectangle;
			StateMap.set(StateFlag::FRMROT);
			break;
		}
		if (!SelectedFormList->empty()) {
			form::pxrct2stch(SelectedFormsRect, RotationRect);
			StateMap.set(StateFlag::FRMSROT);
			break;
		}
		if (StateMap.test(StateFlag::BIGBOX)) {
			RotationRect = AllItemsRect;
			break;
		}
		if (StateMap.test(StateFlag::GRPSEL)) {
			thred::rngadj();
			thred::selRct(RotationRect);
			break;
		}
		if (StateMap.test(StateFlag::FPSEL)) {
			RotationRect = SelectedVerticesRect;
			break;
		}
	} while (false);
	if (StateMap.test(StateFlag::GMRK)) {
		rotationCenter = ZoomMarkPoint;
	}
	else {
		rotationCenter
			= fPOINT{ form::midl(RotationRect.right, RotationRect.left), form::midl(RotationRect.top, RotationRect.bottom) };
	}
	return rotationCenter;
}

void form::internal::rotentr(double rotationAngle) {
	auto fmtStr = std::wstring {};

	displayText::loadString(fmtStr, IDS_ROTA);
	displayText::shoMsg(fmt::format(fmtStr, (rotationAngle / PI * 180.0)));
	StateMap.set(StateFlag::NUMIN);
	displayText::numWnd();
}

void form::setrang() {
	fi::rotentr(IniFile.rotationAngle);
	StateMap.set(StateFlag::NUROT);
}

void form::rotcmd() {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || !SelectedFormList->empty()
	    || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		fi::rotentr(IniFile.rotationAngle);
		StateMap.set(StateFlag::ENTROT);
	}
	else {
		displayText::shoseln(IDS_ALLX, IDS_ROTCMD);
	}
}

void form::internal::fnagain(float rotationAngle) {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || !SelectedFormList->empty()
	    || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		const auto rotationCenter = form::rotpar();
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
	fi::fnagain(2.0F * PI_F - IniFile.rotationAngle);
}

void form::rotdup() {
	if (StateMap.test(StateFlag::FORMSEL) || StateMap.test(StateFlag::GRPSEL) || !SelectedFormList->empty()) {
		fi::rotentr(IniFile.rotationAngle);
		StateMap.set(StateFlag::ENTRDUP);
	}
	else {
		displayText::shord();
	}
}

void form::internal::adfrm(uint32_t iForm) {
	FormList->push_back(FormList->operator[](iForm));

	auto& currentForm = FormList->back();
	auto& srcForm     = FormList->operator[](iForm);

	ClosestFormToCursor    = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
	currentForm.vertexIndex = thred::adflt(srcForm.vertexCount);
	auto vertexIt          = std::next(FormVertices->cbegin(), srcForm.vertexIndex);
	std::copy(vertexIt, std::next(vertexIt, srcForm.vertexCount), std::next(FormVertices->begin(), currentForm.vertexIndex));
	currentForm.vertexCount = srcForm.vertexCount;
	if (srcForm.type == SAT && (srcForm.satinGuideCount != 0U)) {
		currentForm.satinOrAngle.guide = satin::adsatk(srcForm.satinGuideCount);

		auto guideStart               = std::next(SatinGuides->cbegin(), srcForm.satinOrAngle.guide);
		auto guideEnd                 = std::next(guideStart, srcForm.satinGuideCount);
		auto guideDest                = std::next(SatinGuides->begin(), currentForm.satinOrAngle.guide);
		std::copy(guideStart, guideEnd, guideDest);
	}
	if (clip::iseclpx(srcForm)) {
		currentForm.borderClipData = thred::adclp(srcForm.clipEntries);

		auto offsetStart          = std::next(ClipPoints->cbegin(), srcForm.borderClipData);
		auto destination          = std::next(ClipPoints->begin(), currentForm.borderClipData);
		std::copy(offsetStart, std::next(offsetStart, srcForm.clipEntries), destination);
	}
	if (clip::isclpx(srcForm)) {
		currentForm.angleOrClipData.clip = thred::adclp(srcForm.lengthOrCount.clipCount);

		auto       sourceStart          = std::next(ClipPoints->cbegin(), srcForm.angleOrClipData.clip);
		auto       sourceEnd            = std::next(sourceStart, srcForm.lengthOrCount.clipCount);
		const auto destination          = std::next(ClipPoints->begin(), currentForm.angleOrClipData.clip);
		std::copy(sourceStart, sourceEnd, destination);
	}
}

void form::duprot(float rotationAngle) {
	thred::savdo();
	fi::adfrm(ClosestFormToCursor);
	const auto rotationCenter = form::rotpar();
	thred::rotfn(rotationAngle, rotationCenter);
	form::refil();
	StateMap.set(StateFlag::FORMSEL);
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::duprotfs(float rotationAngle) {
	const auto rotationCenter = form::rotpar();
	for (auto selectedForm : (*SelectedFormList)) {
		adfrm(selectedForm);
		thred::rotfn(rotationAngle, rotationCenter);
	}
}

void form::internal::duprots(float rotationAngle, const fPOINT& rotationCenter) {
	thred::rngadj();
	ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size());
	auto sourceIt     = std::next(StitchBuffer->begin(), GroupStartStitch);
	auto endPoint     = std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U);
	while (sourceIt < endPoint) {
		StitchBuffer->push_back(fPOINTATTR { (*sourceIt).x, (*sourceIt).y, (*sourceIt).attribute & (~(FRMSK | TYPMSK)) });
		sourceIt++;
	}
	GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
	thred::rngadj();
	thred::rotfn(rotationAngle, rotationCenter);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::cplayfn(uint32_t iForm, uint32_t play) {
	FormList->push_back(FormList->operator[](iForm));
	// the push_back may invalidate the reference so do it afterwards
	auto& srcForm = FormList->operator[](iForm);
	auto& currentForm = FormList->back();
	currentForm.vertexIndex = thred::adflt(srcForm.vertexCount);
	auto vertexIt             = std::next(FormVertices->cbegin(), srcForm.vertexIndex);
	std::copy(vertexIt, std::next(vertexIt, srcForm.vertexCount), std::next(FormVertices->begin(), currentForm.vertexIndex));
	if (currentForm.type == SAT && (currentForm.satinGuideCount != 0U)) {
		currentForm.satinOrAngle.guide = satin::adsatk(srcForm.satinGuideCount);

		auto       sourceStart = std::next(SatinGuides->cbegin(), srcForm.satinOrAngle.guide);
		auto       sourceEnd = std::next(sourceStart, srcForm.satinGuideCount);
		const auto destination = std::next(SatinGuides->begin(), currentForm.satinOrAngle.guide);
		std::copy(sourceStart, sourceEnd, destination);
	}
	currentForm.clipEntries             = 0;
	currentForm.fillType                = 0;
	currentForm.lengthOrCount.clipCount = 0;
	currentForm.edgeType                = 0;
	currentForm.fillInfo.texture.index  = 0;
	currentForm.attribute               = srcForm.attribute & NFRMLMSK;
	currentForm.attribute |= play;
	currentForm.extendedAttribute = 0;
	form::dusqr(currentForm);
}

void form::cpylayr(uint32_t codedLayer) {
	if (!SelectedFormList->empty()) {
		thred::savdo();
		for (auto selectedForm : (*SelectedFormList)) {
			fi::cplayfn(selectedForm, codedLayer);
		}
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			fi::cplayfn(ClosestFormToCursor, codedLayer);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				const auto codedStitchLayer = codedLayer << (LAYSHFT - 1);
				auto       endStitch        = std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(GroupEndStitch) + 1U);
				for (auto currentStitch = std::next(StitchBuffer->begin(), GroupStartStitch); currentStitch < endStitch;
				     currentStitch++) {
					StitchBuffer->push_back(fPOINTATTR {
					    (*currentStitch).x, (*currentStitch).y, ((*currentStitch).attribute & NLAYMSK) | codedStitchLayer });
				}
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::movlayr(uint32_t codedLayer) {
	const auto codedStitchLayer = codedLayer << (LAYSHFT - 1);

	if (!SelectedFormList->empty()) {
		thred::savdo();
		auto formMap = boost::dynamic_bitset<>(FormList->size());
		for (auto selectedForm : (*SelectedFormList)) {
			auto& formAttr = FormList->operator[](selectedForm).attribute;
			formAttr       = gsl::narrow<uint8_t>(codedLayer | gsl::narrow_cast<decltype(codedLayer)>(formAttr & NFRMLMSK));
			formMap.set(selectedForm);
		}
		for (auto& stitch : *StitchBuffer) {
			if ((stitch.attribute & ALTYPMSK) != 0U) {
				const auto iCurrentForm = (stitch.attribute & FRMSK) >> FRMSHFT;
				if (formMap.test(iCurrentForm)) {
					stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
				}
			}
		}
		SelectedFormList->clear();
		StateMap.reset(StateFlag::FORMSEL);
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			auto& formAttr = FormList->operator[](ClosestFormToCursor).attribute;
			formAttr       = gsl::narrow<uint8_t>(codedLayer | gsl::narrow_cast<decltype(codedLayer)>(formAttr & NFRMLMSK));
			StateMap.reset(StateFlag::FORMSEL);
			for (auto& stitch : *StitchBuffer) {
				if (((stitch.attribute & ALTYPMSK) != 0U) && ((stitch.attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
					stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
				}
			}
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
					(*StitchBuffer)[iStitch].attribute = ((*StitchBuffer)[iStitch].attribute & NLAYMSK) | codedStitchLayer;
				}
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::join() {
	const auto savedFormIndex = ClosestFormToCursor;
	auto       srcForm        = std::next(FormList->cbegin(), ClosestFormToCursor);
	auto       lastVertex
	    = std::next(FormVertices->begin(), gsl::narrow_cast<ptrdiff_t>(srcForm->vertexIndex) + srcForm->vertexCount - 1U);
	StateMap.set(StateFlag::FRMSAM);
	if (FormList->size() > 1 && StateMap.test(StateFlag::FORMSEL) && form::closfrm()) {
		auto formIter   = std::next(FormList->cbegin(), ClosestFormToCursor);
		auto vertexList = std::vector<fPOINT> {};
		vertexList.reserve(formIter->vertexCount);
		auto vertexIt = std::next(FormVertices->cbegin(), formIter->vertexIndex);
		if ((abs(lastVertex->x - vertexIt->x) > TINY) || (abs(lastVertex->y - vertexIt->y) > TINY)) {
			vertexList.push_back(vertexIt[ClosestVertexToCursor]);
		}
		ClosestVertexToCursor = form::nxt(ClosestVertexToCursor);
		for (auto iVertex = 1U; iVertex < formIter->vertexCount; iVertex++) {
			vertexList.push_back(vertexIt[ClosestVertexToCursor]);
			ClosestVertexToCursor = form::nxt(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::DELTO);
		thred::frmdel();
		if (savedFormIndex > ClosestFormToCursor) {
			ClosestFormToCursor = savedFormIndex - 1U;
		}
		else {
			ClosestFormToCursor = savedFormIndex;
		}
		formIter                  = std::next(FormList->cbegin(), ClosestFormToCursor); // points to the first form
		const auto insertionPoint = formIter->vertexIndex + formIter->vertexCount;
		form::fltspac(formIter->vertexCount, wrap::toUnsigned(vertexList.size()));
		auto dest = std::next(FormVertices->begin(), insertionPoint);
		std::copy(vertexList.cbegin(), vertexList.cend(), dest);
		auto& currentForm = FormList->operator[](ClosestFormToCursor);
		vertexIt     = std::next(FormVertices->cbegin(), currentForm.vertexIndex);
		currentForm.vertexCount += gsl::narrow<decltype(currentForm.vertexCount)>(vertexList.size());
		form::frmout(ClosestFormToCursor);
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	StateMap.reset(StateFlag::FRMSAM);
}

void form::refilal() {
	const auto savedFormIndex = ClosestFormToCursor;

	thred::savdo();
	for (ClosestFormToCursor = 0; ClosestFormToCursor < wrap::toUnsigned(FormList->size()); ClosestFormToCursor++) {
		form::refilfn();
	}
	ClosestFormToCursor = savedFormIndex;
	form::fvars(ClosestFormToCursor);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

auto form::internal::notsel() noexcept -> bool {
	for (auto selectedForm : (*SelectedFormList)) {
		if (selectedForm == ClosestFormToCursor) {
			return false;
		}
	}
	return true;
}

void form::nufsel() {
	if (!FormList->empty()) {
		StateMap.reset(StateFlag::FORMSEL);
		if (StateMap.testAndReset(StateFlag::WASEL)) {
			SelectedFormList->push_back(PreviousFormIndex);
		}
		if (fi::notsel()) {
			SelectedFormList->push_back(ClosestFormToCursor);
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::frmadj(uint32_t formIndex) {
	form::fvars(formIndex);
	auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0U; iVertex < FormList->operator[](ClosestFormToCursor).vertexCount; iVertex++) {
		vertexIt[iVertex].x += FormMoveDelta.x;
		vertexIt[iVertex].y -= FormMoveDelta.y;
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
		const auto trg     = ((ClosestFormToCursor << 4U) | type);

		for (auto stitch : *StitchBuffer) {
			if ((stitch.attribute & (ALTYPMSK | FRMSK)) == trg) {
				break;
			}
			iStitch++;
		}
		ClosestPointIndex = iStitch;
		auto bFlag        = false;
		for (auto it = std::next(StitchBuffer->begin(), ClosestPointIndex); it != StitchBuffer->end(); it++) {
			if (((*it).attribute & (ALTYPMSK | FRMSK)) != trg) {
				GroupStitchIndex = std::distance(StitchBuffer->begin(), it) - 1U;
				bFlag            = true;
				break;
			}
		}
		if (!bFlag) {
			GroupStitchIndex = gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U);
		}
	}
	else {
		ClosestPointIndex = 0;
		GroupStitchIndex  = 0;
	}
}

void form::selfil(uint32_t type) {
	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
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
		StateMap.set(StateFlag::GRPSEL);
		StateMap.reset(StateFlag::FORMSEL);
		thred::rngadj();
		StateMap.set(StateFlag::RESTCH);
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
	if (StateMap.test(StateFlag::FORMSEL)) {
		const auto savedIndex = ClosestPointIndex;
		ClosestPointIndex     = 0U;
		while (ClosestPointIndex < StitchBuffer->size() && form::notfstch((*StitchBuffer)[ClosestPointIndex].attribute)) {
			ClosestPointIndex++;
		}
		if (ClosestPointIndex != StitchBuffer->size()) {
			if (ClosestPointIndex != 0U) {
				ClosestPointIndex--;
			}
			GroupStitchIndex = gsl::narrow<decltype(GroupStitchIndex)>(StitchBuffer->size() - 1U);
			while (GroupStitchIndex > ClosestPointIndex && form::notfstch((*StitchBuffer)[GroupStitchIndex].attribute)) {
				GroupStitchIndex--;
			}
			StateMap.set(StateFlag::GRPSEL);
			StateMap.reset(StateFlag::FORMSEL);
			thred::rngadj();
			StateMap.set(StateFlag::RESTCH);
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
		range.start          = 0;
		auto&      form      = FormList->operator[](iForm);
		const auto saveClose = ClosestFormToCursor;
		ClosestFormToCursor  = iForm;
		if ((form.fillType != 0U) || (form.edgeType != 0U)) {
			while (range.start < StitchBuffer->size() && notfstch((*StitchBuffer)[range.start].attribute)) {
				range.start++;
			}
			range.finish = wrap::toUnsigned(StitchBuffer->size() - 1U);
			while (range.finish > range.start && notfstch((*StitchBuffer)[range.finish].attribute)) {
				range.finish--;
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
	form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
	form::savblen(ButtonholeCornerLength);
	form::refilfn();
}

void form::bhol() {
	const auto savedSpacing = LineSpacing;

	if (displayText::filmsgs(FML_BHOL)) {
		return;
	}
	LineSpacing = savedSpacing;
	if (!SelectedFormList->empty()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			fi::bholbrd(selectedForm);
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			fi::bholbrd(ClosestFormToCursor);
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void form::fcntr() {
	if (!SelectedFormList->empty()) {
		thred::savdo();
		auto        firstForm = SelectedFormList->front();
		const auto& firstRect = FormList->operator[](firstForm).rectangle;
		const auto  initialCenter
		    = fPOINT { form::midl(firstRect.right, firstRect.left), form::midl(firstRect.top, firstRect.bottom) };
		for (auto selectedForm : (*SelectedFormList)) {
			const auto& formRect = FormList->operator[](selectedForm).rectangle;
			const auto  currentCenter
			    = fPOINT { form::midl(formRect.right, formRect.left), form::midl(formRect.top, formRect.bottom) };
			const auto delta = fPOINT { initialCenter.x - currentCenter.x, initialCenter.y - currentCenter.y };
			auto& currentForm     = FormList->operator[](selectedForm);
			auto vertexIt    = std::next(FormVertices->begin(), currentForm.vertexIndex);
			for (auto iVertex = 0U; iVertex < currentForm.vertexCount; iVertex++) {
				vertexIt[iVertex].x += delta.x;
				vertexIt[iVertex].y += delta.y;
			}
			form::frmout(selectedForm);
			auto codedForm = (selectedForm << FRMSHFT);
			for (auto& stitch : *StitchBuffer) {
				if ((stitch.attribute & FRMSK) == codedForm && ((stitch.attribute & NOTFRM) == 0U)) {
					stitch.x += delta.x;
					stitch.y += delta.y;
				}
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::tabmsg(IDS_SELC);
	}
}

void form::boxsel() {
	StateMap.reset(StateFlag::INSRT);
	if (!StateMap.test(StateFlag::THUMSHO)) {
		StateMap.reset(StateFlag::BZUM);
		StateMap.reset(StateFlag::BZUMIN);
		StateMap.set(StateFlag::VCAPT);
		SetCapture(ThrEdWindow);
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::internal::fspic(uint32_t formIndex) {
	auto& form = FormList->operator[](formIndex);
	clip::deleclp(formIndex);
	const auto clipSize          = wrap::toUnsigned(ClipBuffer->size());
	form.edgeType       = EDGEPICOT;
	form.clipEntries    = clipSize;
	form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
	form.borderSize     = ClipRectSize.cy;
	form.edgeSpacing    = PicotSpacing;
	form.borderColor    = gsl::narrow<uint8_t>(ActiveColor);
	form::bsizpar(form);
	form::savplen(ButtonholeCornerLength);
	auto offsetStart = std::next(ClipPoints->begin(), form.borderClipData);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	form::refilfn();
}

void form::picot() {
	if (displayText::filmsgs(FML_PIC)) {
		return;
	}
	if (OpenClipboard(ThrEdWindow) != 0) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		Clip       = RegisterClipboardFormat(PcdClipFormat);
		ClipMemory = GetClipboardData(Clip);
		if (ClipMemory != nullptr) {
			thred::redclp();
			CloseClipboard();
			if (ClipRectSize.cx > CLPMIN) {
				if (!SelectedFormList->empty()) {
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						form::fvars(ClosestFormToCursor);
						fi::fspic(selectedForm);
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::fspic(ClosestFormToCursor);
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
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
	auto& form = FormList->operator[](ClosestFormToCursor);
	form::fvars(formIndex);
	if (form.vertexCount > 4) {
		clip::delclps(ClosestFormToCursor);
		texture::deltx(ClosestFormToCursor);
		form::chkcont();
		form.attribute |= gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
		form.fillColor = gsl::narrow<uint8_t>(ActiveColor);
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
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		form::fvars(ClosestFormToCursor);
		thred::savdo();
		if (fi::contsf(ClosestFormToCursor)) {
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			displayText::tabmsg(IDS_CONT);
		}
	}
}

void form::internal::dupfn(float rotationAngle) {
	thred::savdo();
	const auto rotationCenter = form::rotpar();
	if (IniFile.rotationAngle != 0.0) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::duprot(rotationAngle);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
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
		StateMap.set(StateFlag::ENTRDUP);
	}
}

void form::redup() {
	fi::dupfn(IniFile.rotationAngle);
}

void form::bakdup() {
	fi::dupfn(2.0F * PI_F - IniFile.rotationAngle);
}

void form::internal::shrnks() {
	auto clipRect = fRECTANGLE {};
	auto lengths  = std::vector<uint32_t> {};
	auto deltas   = std::vector<fPOINT> {};
	lengths.reserve(VertexCount);
	deltas.reserve(VertexCount - 1U);
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	clip::oclp(clipRect, currentForm.borderClipData, currentForm.clipEntries);
	auto vertexIt = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		auto& thisVertex = vertexIt[iVertex];
		auto& nextVertex = vertexIt[wrap::toSize(iVertex) + 1U];
		deltas.emplace_back(fPOINT { nextVertex.x - thisVertex.x, nextVertex.y - thisVertex.y });
		lengths.emplace_back(hypot(deltas.back().x, deltas.back().y));
	}
	auto length = lengths.begin();
	auto delta  = deltas.begin();
	for (auto iVertex = 0U; iVertex < VertexCount - 1U; iVertex++) {
		const auto count      = std::floor(*length / ClipRectSize.cx);
		const auto ratio      = (ClipRectSize.cx * count + 0.004F) / *length;
		auto&      thisVertex = vertexIt[iVertex];
		auto&      nextVertex = vertexIt[wrap::toSize(iVertex) + 1U];
		nextVertex.x          = thisVertex.x + (*delta).x * ratio;
		nextVertex.y          = thisVertex.y + (*delta).y * ratio;
		length++;
		delta++;
	}
	form::frmout(ClosestFormToCursor);
	form::refil();
}

void form::shrnk() {
	form::fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FORMSEL) && FormList->operator[](ClosestFormToCursor).edgeType == EDGECLIP) {
		thred::savdo();
		fi::shrnks();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
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

	auto vertexIt = std::next(FormVertices->cbegin(), dest.vertexIndex);
	std::copy(vertexIt, std::next(vertexIt, dest.vertexCount), std::next(destFormVertices.begin(), formSourceIndex));
	dest.vertexIndex = formSourceIndex;
	formSourceIndex += dest.vertexCount;
	if (dest.satinGuideCount != 0U) {
		auto guideStart = std::next(SatinGuides->cbegin(), dest.satinOrAngle.guide);
		auto guideEnd   = guideStart + dest.satinGuideCount;
		tempGuides.insert(tempGuides.end(), guideStart, guideEnd);
		dest.satinOrAngle.guide = gsl::narrow<decltype(dest.satinOrAngle.guide)>(tempGuides.size() - dest.satinGuideCount);
	}
	if (clip::iseclpx(formIndex)) {
		const auto sourceStart = std::next(ClipPoints->cbegin(), dest.borderClipData);
		auto       sourceEnd   = std::next(sourceStart, dest.clipEntries);
		tempClipPoints.insert(tempClipPoints.end(), sourceStart, sourceEnd);
		dest.borderClipData = gsl::narrow<decltype(dest.borderClipData)>(tempClipPoints.size() - dest.clipEntries);
	}
	if (clip::isclpx(formIndex)) {
		auto sourceStart = std::next(ClipPoints->cbegin(), dest.angleOrClipData.clip);
		auto sourceEnd   = std::next(sourceStart, dest.lengthOrCount.clipCount);
		tempClipPoints.insert(tempClipPoints.end(), sourceStart, sourceEnd);
		dest.angleOrClipData.clip
		    = gsl::narrow<decltype(dest.angleOrClipData.clip)>(tempClipPoints.size() - dest.lengthOrCount.clipCount);
	}
}

void form::internal::stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept {
	attribute &= NFRMSK;
	attribute |= formIndex << FRMSHFT;
}

void form::frmnumfn(uint32_t newFormIndex) {
	auto start  = 0U;
	auto finish = 0U;

	thred::savdo();
	if (newFormIndex != ClosestFormToCursor) {
		if (ClosestFormToCursor > newFormIndex) {
			start  = newFormIndex;
			finish = ClosestFormToCursor;
		}
		else {
			start  = ClosestFormToCursor;
			finish = newFormIndex;
		}
		auto formRelocationIndex = 0U;
		auto sourceForm          = 0U;

		auto tempFormList = std::vector<FRMHED> {};
		tempFormList.resize(FormList->size());
		auto tempFormVertices = std::vector<fPOINT> {};
		tempFormVertices.resize(FormVertices->size());
		auto tempGuides = std::vector<SATCON> {};
		tempGuides.reserve(SatinGuides->size());
		auto tempClipPoints = std::vector<fPOINT> {};
		tempClipPoints.reserve(ClipPoints->size());

		auto formSourceIndex = 0U;
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			if (iForm == newFormIndex) {
				fi::dufdat(tempClipPoints,
				           tempGuides,
				           tempFormVertices,
				           tempFormList,
				           ClosestFormToCursor,
				           formRelocationIndex,
				           formSourceIndex);
			}
			else {
				if (sourceForm == ClosestFormToCursor) {
					sourceForm++;
				}
				fi::dufdat(tempClipPoints,
				           tempGuides,
				           tempFormVertices,
				           tempFormList,
				           sourceForm++,
				          formRelocationIndex,
				          formSourceIndex);
			}
		}
		auto& formList = *FormList;
		std::copy(tempFormList.cbegin(), tempFormList.cend(), formList.begin());
		std::copy(tempFormVertices.cbegin(), tempFormVertices.cend(), FormVertices->begin());
		std::copy(tempGuides.cbegin(), tempGuides.cend(), SatinGuides->begin());
		std::copy(tempClipPoints.cbegin(), tempClipPoints.cend(), ClipPoints->begin());
		for (auto& stitch : *StitchBuffer) {
			if ((stitch.attribute & SRTYPMSK) != 0U) {
				const auto decodedFormIndex = (stitch.attribute & FRMSK) >> FRMSHFT;
				if (decodedFormIndex == ClosestFormToCursor) {
					fi::stchfrm(newFormIndex, stitch.attribute);
				}
				else {
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
		displayText::ritnum(STR_NUMFRM, ClosestFormToCursor);
	}
}

constexpr auto form::internal::duat(uint32_t attribute) -> uint32_t {
	const auto type = ((attribute >> TYPSHFT) + 1U) & 3U;
	const auto frm  = (attribute & FRMSK) >> 2U;

	return type | frm;
}

void form::internal::srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, uint32_t start, uint32_t finish) {
	if (start != finish) {
		auto stitchHistogram = std::vector<uint32_t> {};
		stitchHistogram.resize(FormList->size() << 2U);
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++;
		}
		auto stitchAccumulator = start;
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size() << 2U); iForm++) {
			auto value             = stitchHistogram[iForm];
			stitchHistogram[iForm] = stitchAccumulator;
			stitchAccumulator += value;
		}
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			(*StitchBuffer)[stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++] = tempStitchBuffer[iStitch];
		}
	}
}

void form::srtbyfrm() {
	auto colorHistogram = std::vector<uint32_t> {};
	colorHistogram.resize(16U);
	auto color = std::vector<uint32_t> {};
	color.resize(16U);

	if (!FormList->empty()) {
		thred::savdo();
		color[AppliqueColor] = 0U;
		for (auto iColor = 0U; iColor < 16U; iColor++) {
			if (iColor != AppliqueColor) {
				color[iColor] = iColor + 1U;
			}
		}
		auto tempStitchBuffer = std::vector<fPOINTATTR> {};
		tempStitchBuffer.resize(StitchBuffer->size());
		for (auto& stitch : *StitchBuffer) {
			colorHistogram[color[stitch.attribute & 0xfU]]++;
		}
		auto colorAccumulator = 0U;
		for (auto iColor = 0U; iColor < 16U; iColor++) {
			auto value             = colorHistogram[iColor];
			colorHistogram[iColor] = colorAccumulator;
			colorAccumulator += value;
		}
		for (auto& stitch : *StitchBuffer) {
			tempStitchBuffer[colorHistogram[color[stitch.attribute & 0xfU]]++] = stitch;
		}
		fi::srtf(tempStitchBuffer, 0, colorHistogram[0]);
		for (auto iColor = 0U; iColor < 15U; iColor++) {
			fi::srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[wrap::toSize(iColor) + 1U]);
		}
	}
	else {
		xt::srtcol();
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::cntrx() {
	auto markCenter = fPOINT {};
	bool flag       = false;

	if (StateMap.test(StateFlag::GMRK)) {
		markCenter = ZoomMarkPoint;
	}
	else {
		markCenter.x = UnzoomedRect.x / 2.0F;
		markCenter.y = UnzoomedRect.y / 2.0F;
	}
	if (!SelectedFormList->empty()) {
		flag = true;
		thred::savdo();
		auto selectedCenter = fPOINT {};
		fi::dufcntr(selectedCenter);
		FormMoveDelta = fPOINT { markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y };
		if (StateMap.test(StateFlag::CNTRV)) {
			FormMoveDelta.y = 0.0F;
		}
		if (StateMap.test(StateFlag::CNTRH)) {
			FormMoveDelta.x = 0.0F;
		}
		for (auto selectedForm : (*SelectedFormList)) {
			frmadj(selectedForm);
		}
		frmsadj();
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			flag = true;
			thred::savdo();
			const auto& formRect = FormList->operator[](ClosestFormToCursor).rectangle;

			const auto selectedCenter
			    = fPOINT { form::midl(formRect.right, formRect.left), form::midl(formRect.top, formRect.bottom) };
			FormMoveDelta = fPOINT { markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y };
			if (StateMap.test(StateFlag::CNTRV)) {
				FormMoveDelta.y = 0.0F;
			}
			if (StateMap.test(StateFlag::CNTRH)) {
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
			if (StateMap.test(StateFlag::GRPSEL)) {
				flag = true;
				thred::savdo();
				thred::rngadj();
				auto groupRect = fRECTANGLE { (*StitchBuffer)[GroupStartStitch].x,
					                          (*StitchBuffer)[GroupStartStitch].y,
					                          (*StitchBuffer)[GroupStartStitch].x,
					                          (*StitchBuffer)[GroupStartStitch].y };
				for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; iStitch++) {
					if ((*StitchBuffer)[iStitch].x < groupRect.left) {
						groupRect.left = (*StitchBuffer)[iStitch].x;
					}
					if ((*StitchBuffer)[iStitch].y > groupRect.top) {
						groupRect.top = (*StitchBuffer)[iStitch].y;
					}
					if ((*StitchBuffer)[iStitch].x > groupRect.right) {
						groupRect.right = (*StitchBuffer)[iStitch].x;
					}
					if ((*StitchBuffer)[iStitch].y < groupRect.bottom) {
						groupRect.bottom = (*StitchBuffer)[iStitch].y;
					}
				}
				const auto selectedCenter
				    = fPOINT { form::midl(groupRect.right, groupRect.left), form::midl(groupRect.top, groupRect.bottom) };
				FormMoveDelta = fPOINT { markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y };
				if (StateMap.test(StateFlag::CNTRV)) {
					FormMoveDelta.y = 0.0F;
				}
				if (StateMap.test(StateFlag::CNTRH)) {
					FormMoveDelta.x = 0.0F;
				}
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					(*StitchBuffer)[iStitch].x += FormMoveDelta.x;
					(*StitchBuffer)[iStitch].y -= FormMoveDelta.y;
				}
			}
			else {
				displayText::shoseln(IDS_FGRPF, IDS_CENT);
			}
		}
	}
	if (flag) {
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::centir() {
	StateMap.reset(StateFlag::BIGBOX);
	fi::getbig();
	const auto itemCenter
	    = fPOINT { form::midl(AllItemsRect.right, AllItemsRect.left), form::midl(AllItemsRect.top, AllItemsRect.bottom) };
	const auto hoopCenter = fPOINT { UnzoomedRect.x / 2.0F, UnzoomedRect.y / 2.0F };
	const auto delta      = fPOINT { hoopCenter.x - itemCenter.x, hoopCenter.y - itemCenter.y };
	for (auto& stitch : *StitchBuffer) {
		stitch.x += delta.x;
		stitch.y += delta.y;
	}
	for (auto& FormVertice : *FormVertices) {
		FormVertice.x += delta.x;
		FormVertice.y += delta.y;
	}
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		form::frmout(iForm);
	}
	xt::setfchk();
	StateMap.set(StateFlag::RESTCH);
}

// suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
GSL_SUPPRESS(26440) void form::internal::bean(uint32_t start, uint32_t finish) {
	auto highStitchBuffer = std::vector<fPOINTATTR> {};
	auto iSourceStitch    = start;

	BeanCount = 0;
	highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
	if ((*StitchBuffer)[wrap::toSize(iSourceStitch) + 2U].x != (*StitchBuffer)[iSourceStitch].x
	    || (*StitchBuffer)[wrap::toSize(iSourceStitch) + 2U].y != (*StitchBuffer)[iSourceStitch].y) {
		highStitchBuffer.push_back((*StitchBuffer)[wrap::toSize(iSourceStitch) + 1U]);
		highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
		BeanCount += 2;
	}
	iSourceStitch++;
	highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
	if ((*StitchBuffer)[wrap::toSize(iSourceStitch) + 2U].x != (*StitchBuffer)[iSourceStitch].x
	    || (*StitchBuffer)[wrap::toSize(iSourceStitch) + 2U].y != (*StitchBuffer)[iSourceStitch].y) {
		highStitchBuffer.push_back((*StitchBuffer)[wrap::toSize(iSourceStitch) + 1U]);
		highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < finish - 1) {
		highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
		if (((*StitchBuffer)[wrap::toSize(iSourceStitch) + 2U].x != (*StitchBuffer)[iSourceStitch].x
		     || (*StitchBuffer)[wrap::toSize(iSourceStitch) + 2U].y != (*StitchBuffer)[iSourceStitch].y)
		    && ((*StitchBuffer)[wrap::toSize(iSourceStitch) - 2U].x != (*StitchBuffer)[iSourceStitch].x
		        || (*StitchBuffer)[wrap::toSize(iSourceStitch) - 2U].y != (*StitchBuffer)[iSourceStitch].y)) {
			highStitchBuffer.push_back((*StitchBuffer)[wrap::toSize(iSourceStitch) + 1U]);
			highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
			BeanCount += 2;
		}
		iSourceStitch++;
	}
	highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
	if (((*StitchBuffer)[wrap::toSize(iSourceStitch) - 2U].x != (*StitchBuffer)[iSourceStitch].x
	     || (*StitchBuffer)[wrap::toSize(iSourceStitch) - 2U].y != (*StitchBuffer)[iSourceStitch].y)) {
		highStitchBuffer.push_back((*StitchBuffer)[wrap::toSize(iSourceStitch) + 1U]);
		highStitchBuffer.push_back((*StitchBuffer)[iSourceStitch]);
		BeanCount += 2;
	}
	// now copy stitches back up to the end of the original group
	std::copy(
	    highStitchBuffer.begin(), std::next(highStitchBuffer.begin(), finish - start), std::next(StitchBuffer->begin(), start));
	// and then insert the remainder of the new stitches
	StitchBuffer->insert(
	    std::next(StitchBuffer->begin(), finish), std::next(highStitchBuffer.begin(), finish - start), highStitchBuffer.end());
}

void form::dubean() {
	if (!StitchBuffer->empty()) {
		thred::savdo();
		if (StateMap.test(StateFlag::GRPSEL)) {
			thred::rngadj();
			fi::bean(GroupStartStitch, GroupEndStitch);
			if (ClosestPointIndex > GroupStitchIndex) {
				ClosestPointIndex += BeanCount;
			}
			else {
				GroupStitchIndex += BeanCount;
			}
			thred::grpAdj();
		}
		else {
			fi::bean(0, wrap::toUnsigned(StitchBuffer->size() - 1U));
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::internal::unbean(uint32_t start, uint32_t& finish) {
	auto highStitchBuffer = std::vector<fPOINTATTR> {};
	auto lastStitch       = finish;
	if (lastStitch > wrap::toUnsigned(StitchBuffer->size()) - 3U) {
		lastStitch = wrap::toUnsigned(StitchBuffer->size()) - 3U;
	}
	auto iSource = start;
	for (; iSource <= lastStitch; iSource++) {
		highStitchBuffer.push_back((*StitchBuffer)[iSource]);
		if ((*StitchBuffer)[iSource].x == (*StitchBuffer)[wrap::toSize(iSource) + 2U].x
		    && (*StitchBuffer)[iSource].y == (*StitchBuffer)[wrap::toSize(iSource) + 2U].y) {
			iSource += 2;
		}
	}
	if (finish != lastStitch) {
		while (iSource != finish + 1U) {
			highStitchBuffer.push_back((*StitchBuffer)[iSource++]);
		}
	}
	if ((finish - start) > highStitchBuffer.size()) {
		auto stitchStart = std::next(StitchBuffer->begin(), start);
		std::copy(highStitchBuffer.begin(), highStitchBuffer.end(), stitchStart);
		StitchBuffer->erase(std::next(StitchBuffer->begin(), start + highStitchBuffer.size()),
		                    std::next(StitchBuffer->begin(), gsl::narrow_cast<ptrdiff_t>(finish) + 1U));
	}
	finish = start + wrap::toUnsigned(highStitchBuffer.size() - 1U);
}

void form::debean() {
	thred::savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		fi::unbean(GroupStartStitch, GroupEndStitch);
		if (ClosestPointIndex > GroupStitchIndex) {
			ClosestPointIndex = GroupEndStitch;
		}
		else {
			GroupStitchIndex = GroupEndStitch;
		}
		const auto iEndStitch = wrap::toUnsigned(StitchBuffer->size()) - 1U;
		if (ClosestPointIndex > gsl::narrow<uint32_t>(iEndStitch) - 1) {
			ClosestPointIndex = iEndStitch;
		}
		if (GroupStitchIndex > gsl::narrow<uint32_t>(iEndStitch) - 1) {
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
	StateMap.set(StateFlag::RESTCH);
}

void form::clpspac(const uint32_t insertPoint, uint32_t count) {
	auto sourceStart = std::next(ClipPoints->cbegin(), insertPoint);
	auto sourceEnd   = std::next(sourceStart, count);
	ClipPoints->insert(sourceStart, sourceStart, sourceEnd);
}

void form::stchadj() {
	const auto     codedClosest = ClosestFormToCursor << FRMSHFT;
	constexpr auto offset       = 1U << FRMSHFT;
	for (auto& stitch : *StitchBuffer) {
		auto codedForm = stitch.attribute & FRMSK;
		if (codedForm > codedClosest) {
			stitch.attribute &= NFRMSK;
			codedForm += offset;
			stitch.attribute |= codedForm;
		}
	}
	form::refilfn();
	ClosestFormToCursor++;
	form::refilfn();
	StateMap.reset(StateFlag::FRMPSEL);
}

auto form::internal::spltlin() -> bool {
	auto& form = FormList->operator[](ClosestFormToCursor);
	if (ClosestVertexToCursor < 1 || form.vertexCount - ClosestVertexToCursor < 2) {
		return false;
	}
	fi::nufpnt(ClosestVertexToCursor, form);
	auto vertexIt
	    = std::next(FormVertices->begin(), gsl::narrow_cast<ptrdiff_t>(form.vertexIndex) + ClosestVertexToCursor);
	vertexIt[1U] = vertexIt[0U];
	form::frmlin(form.vertexIndex, form.vertexCount);

	FormList->insert(std::next(FormList->cbegin(), ClosestFormToCursor), 1, form);
	auto& dstForm = FormList->operator[](wrap::toSize(ClosestFormToCursor) + 1U);

	form.vertexCount = ClosestVertexToCursor + 1U;
	dstForm.vertexCount -= form.vertexCount;
	dstForm.vertexIndex = form.vertexIndex + form.vertexCount;
	form::frmout(ClosestFormToCursor);
	form::frmout(ClosestFormToCursor + 1U);
	if (clip::iseclp(form)) {
		form::clpspac(form.borderClipData, form.clipEntries);
		const auto maxForm = FormList->size();
		for (auto iForm = ClosestFormToCursor + 1U; iForm < maxForm; iForm++) {
			FormList->operator[](iForm).borderClipData += form.clipEntries;
		}
	}
	form::stchadj();
	return true;
}

void form::spltfrm() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		const auto& currentForm = FormList->operator[](ClosestFormToCursor);
		if (currentForm.type == SAT) {
			if (currentForm.satinGuideCount != 0U) {
				auto guideIt = std::next(SatinGuides->cbegin(), currentForm.satinOrAngle.guide);
				for (auto guideIndex = 0U; guideIndex < currentForm.satinGuideCount; guideIndex++) {
					if (guideIt[guideIndex].start == ClosestVertexToCursor
					    || guideIt[guideIndex].finish == ClosestVertexToCursor) {
						satin::spltsat(guideIndex);
						return;
					}
				}
			}
		}
		else {
			if (currentForm.type == FRMLINE) {
				if (fi::spltlin()) {
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
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
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		if ((GroupEndStitch - GroupStartStitch) > 12000U) {
			displayText::tabmsg(IDS_STMAX);
			return;
		}
		const auto vertexCount = GroupEndStitch - GroupStartStitch + 1U;
		FormList->push_back(FRMHED {});
		auto& currentForm       = FormList->back();
		currentForm.type        = FRMLINE;
		currentForm.vertexCount = vertexCount;
		currentForm.vertexIndex = thred::adflt(vertexCount);
		auto iVertex              = 0U;
		auto vertexIt             = std::next(FormVertices->begin(), currentForm.vertexIndex);
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
			vertexIt[iVertex].x   = (*StitchBuffer)[iStitch].x;
			vertexIt[iVertex++].y = (*StitchBuffer)[iStitch].y;
		}
		form::frmout(wrap::toUnsigned(FormList->size() - 1U));
		if (ClosestPointIndex > GroupStitchIndex) {
			if (ClosestPointIndex < gsl::narrow<uint32_t>(StitchBuffer->size() - 1U)) {
				ClosestPointIndex++;
			}
		}
		else {
			if (GroupStitchIndex < gsl::narrow<uint32_t>(StitchBuffer->size() - 1U)) {
				GroupStitchIndex++;
			}
		}
		thred::delstchm();
		StateMap.reset(StateFlag::GRPSEL);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_GRPMSG, IDS_STCH2FRM);
	}
}

void form::vrtsclp() {
	form::fvars(ClosestFormToCursor);
	auto& form = FormList->operator[](ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	form.lengthOrCount.clipCount = gsl::narrow<decltype(form.lengthOrCount.clipCount)>(ClipBuffer->size());
	form.angleOrClipData.clip    = clip::numclp();
	form.wordParam               = IniFile.fillPhase;
	fi::makpoli();
	form.fillSpacing = IniFile.clipOffset;
	auto offsetStart          = std::next(ClipPoints->begin(), form.angleOrClipData.clip);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	form.fillType  = VCLPF;
	form.fillColor = gsl::narrow<uint8_t>(ActiveColor);
	form.type      = FRMFPOLY;
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
					StateMap.set(StateFlag::NOCLP);
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						form::fvars(ClosestFormToCursor);
						if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
							vrtsclp();
						}
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						form::fvars(ClosestFormToCursor);
						vrtsclp();
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
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
	form::fvars(ClosestFormToCursor);
	auto& form = FormList->operator[](ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	const auto clipSize                   = wrap::toUnsigned(ClipBuffer->size());
	form.lengthOrCount.clipCount = clipSize;
	form.angleOrClipData.clip    = clip::numclp();
	form.lengthOrCount.clipCount = clipSize;
	form.wordParam               = IniFile.fillPhase;
	fi::makpoli();
	form.fillSpacing = IniFile.clipOffset;
	auto offsetStart          = std::next(ClipPoints->begin(), form.angleOrClipData.clip);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	form.fillType  = HCLPF;
	form.fillColor = gsl::narrow<uint8_t>(ActiveColor);
	form.type      = FRMFPOLY;
	CurrentVertexIndex      = form.vertexIndex;
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
					StateMap.set(StateFlag::NOCLP);
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						form::fvars(ClosestFormToCursor);
						if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
							horsclp();
						}
					}
					StateMap.set(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						form::fvars(ClosestFormToCursor);
						horsclp();
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
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
	form::fvars(ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	form.angleOrClipData.clip    = clip::numclp();
	form.lengthOrCount.clipCount = gsl::narrow<decltype(form.lengthOrCount.clipCount)>(ClipBuffer->size());
	form.wordParam               = IniFile.fillPhase;
	fi::makpoli();
	form.satinOrAngle.angle = IniFile.fillAngle;
	form.fillSpacing        = IniFile.clipOffset;
	auto offsetStart                 = std::next(ClipPoints->begin(), form.angleOrClipData.clip);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	form.fillType  = ANGCLPF;
	form.fillColor = gsl::narrow<uint8_t>(ActiveColor);
	form.type      = FRMFPOLY;
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
					StateMap.set(StateFlag::NOCLP);
					for (auto selectedForm : (*SelectedFormList)) {
						ClosestFormToCursor = selectedForm;
						auto& form = FormList->operator[](selectedForm);
						form::fvars(ClosestFormToCursor);
						if (form.type != FRMLINE) {
							angsclp(form);
						}
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						auto& form = FormList->operator[](ClosestFormToCursor);
						angsclp(form);
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
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
	form::fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	form.edgeType    = EDGEDOUBLE;
	form.borderColor = gsl::narrow<uint8_t>(ActiveColor);
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
			form::fvars(ClosestFormToCursor);
			auto& form = FormList->operator[](selectedForm);
			dubsfil(form);
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			auto& form = FormList->operator[](ClosestFormToCursor);
			dubsfil(form);
			thred::coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::col2frm() {
	auto colorChangedCount = 0U;

	if (!FormList->empty()) {
		const auto formColorPermutations = FormList->size() << 4U; // total number of form and color combinations
		auto       fillColorHistogram    = std::vector<uint32_t> {};
		fillColorHistogram.resize(formColorPermutations);
		auto borderColorHistogram = std::vector<uint32_t> {};
		borderColorHistogram.resize(formColorPermutations);
		auto featherColorHistogram = std::vector<uint32_t> {};
		featherColorHistogram.resize(formColorPermutations);
		auto underlayColorHistogram = std::vector<uint32_t> {};
		underlayColorHistogram.resize(formColorPermutations);
		for (auto& stitch : *StitchBuffer) {
			const auto formColorCode = stitch.attribute & (COLMSK | FRMSK);
			if ((stitch.attribute & (WLKMSK | CWLKMSK | UNDMSK)) != 0U) {
				underlayColorHistogram[formColorCode]++;
			}
			else {
				if ((stitch.attribute & FTHMSK) != 0U) {
					featherColorHistogram[formColorCode]++;
				}
				else {
					switch (stitch.attribute & TYPMSK) {
					case FRMFIL: {
						fillColorHistogram[formColorCode]++;
						break;
					}
					case FRMBFIL: {
						borderColorHistogram[formColorCode]++;
						break;
					}
					}
				}
			}
		}
		auto startColorOffset = 0U;
		auto endColorOffset   = 16U;
		for (auto& formIter : *FormList) {
			if (formIter.fillType != 0U) {
				auto count         = 0U;
				auto majorityColor = 0U;
				for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (fillColorHistogram[iColor] > count) {
						count         = fillColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xfU;
				if (formIter.fillColor != majorityColor) {
					colorChangedCount++;
					formIter.fillColor = gsl::narrow<uint8_t>(majorityColor);
				}
				if (formIter.fillType == FTHF && ((formIter.extendedAttribute & AT_FTHBLND) != 0U)) {
					count = majorityColor = 0;
					for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
						if (featherColorHistogram[iColor] > count) {
							count         = fillColorHistogram[iColor];
							majorityColor = iColor;
						}
					}
					majorityColor &= 0xfU;
					if (formIter.fillInfo.feather.color != majorityColor) {
						colorChangedCount++;
						formIter.fillInfo.feather.color = gsl::narrow<uint8_t>(majorityColor);
					}
				}
			}
			if (formIter.edgeType != 0U) {
				auto count         = 0U;
				auto majorityColor = 0U;
				for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (borderColorHistogram[iColor] > count) {
						count         = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xfU;
				if (formIter.borderColor != majorityColor) {
					colorChangedCount++;
					formIter.borderColor = gsl::narrow<uint8_t>(majorityColor);
				}
			}
			if ((formIter.extendedAttribute & (AT_WALK | AT_CWLK | AT_UND)) != 0U) {
				auto count         = 0U;
				auto majorityColor = 0U;
				for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (underlayColorHistogram[iColor] > count) {
						count         = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xfU;
				if (formIter.underlayColor != majorityColor) {
					colorChangedCount++;
					formIter.underlayColor = gsl::narrow<uint8_t>(majorityColor);
				}
			}
			startColorOffset += 16U;
			endColorOffset += 16U;
		}
	}
	auto fmtStr = std::wstring {};
	displayText::loadString(fmtStr, IDS_NCOLCHG);
	displayText::shoMsg(fmt::format(fmtStr, colorChangedCount));
}

void form::chan() {
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	currentForm.borderColor        = gsl::narrow<uint8_t>(ActiveColor);
	currentForm.edgeSpacing        = IniFile.chainSpace;
	currentForm.borderSize         = BorderWidth;
	currentForm.edgeStitchLen      = IniFile.chainRatio;
	currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
	currentForm.minBorderStitchLen = MinStitchLength;
	if (StateMap.test(StateFlag::LINCHN)) {
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
			form::fvars(ClosestFormToCursor);
			chan();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			chan();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::crop() {
	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		const auto& form = FormList->operator[](ClosestFormToCursor);
		auto iDestination = StitchBuffer->begin();
		for (auto& stitch : *StitchBuffer) {
			if (form::cisin(form, stitch.x, stitch.y)) {
				*iDestination++ = stitch;
			}
		}
		StitchBuffer->resize(&(*iDestination) - &(StitchBuffer->front()));
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_FRM1MSG, IDS_CROP);
	}
}

void form::internal::fsclpx(uint32_t formIndex) {
	auto& form = FormList->operator[](formIndex);
	clip::deleclp(formIndex);
	const auto clipSize          = wrap::toUnsigned(ClipBuffer->size());
	form.edgeType       = EDGECLIPX;
	form.clipEntries    = clipSize;
	form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
	form.borderSize     = ClipRectSize.cy;
	form.edgeSpacing    = ClipRectSize.cx;
	form.borderColor    = gsl::narrow<uint8_t>(ActiveColor);
	form::bsizpar(form);
	auto offsetStart = std::next(ClipPoints->begin(), form.borderClipData);
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	clip::duxclp(form);
	form::refilfn();
}

void form::filclpx() {
	if (displayText::filmsgs(FML_CLP)) {
		return;
	}
	if (OpenClipboard(ThrEdWindow) != 0) {
		form::fvars(ClosestFormToCursor);
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
						form::fvars(ClosestFormToCursor);
						fi::fsclpx(selectedForm);
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::fsclpx(ClosestFormToCursor);
						StateMap.set(StateFlag::INIT);
						thred::coltab();
						StateMap.set(StateFlag::RESTCH);
					}
				}
			}
			else {
				displayText::tabmsg(IDS_CLP);
			}
		}
		else {
			auto str = std::wstring(L"no clipboard data");
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
			const auto iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
			histogram[iForm]++;
		}
		auto totalStitches = 0U;
		for (auto& entry : histogram) {
			auto formStitchCount = entry;
			entry                = totalStitches;
			totalStitches += formStitchCount;
		}
		auto highStitchBuffer = std::vector<fPOINTATTR> {};
		highStitchBuffer.resize(StitchBuffer->size());
		for (auto& stitch : *StitchBuffer) {
			const auto iForm              = (stitch.attribute & FRMSK) >> FRMSHFT;
			auto       iHighStitch        = histogram[iForm]++;
			highStitchBuffer[iHighStitch] = stitch;
		}
		std::copy(highStitchBuffer.cbegin(), highStitchBuffer.cend(), StitchBuffer->begin());
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}
