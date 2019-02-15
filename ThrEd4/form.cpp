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

FRMHED*      FormForInsert;  // insert form vertex in this form
unsigned int FormVertexNext; // form vertex storage for form vertex insert
unsigned int FormVertexPrev; // form vertex storage for form vertex insert

std::vector<fPOINT>* WorkingFormVertices; // form points for angle fills

unsigned char Level00   = 0;
unsigned char Level01   = 1;
unsigned char Level02[] = { 0, 1 };
unsigned char Level03[] = { 1, 0, 2 };
unsigned char Level04[] = { 1, 3, 0, 2 };
unsigned char Level05[] = { 2, 0, 3, 1, 4 };
unsigned char Level06[] = { 3, 0, 2, 4, 1, 5 };
unsigned char Level07[] = { 3, 0, 4, 1, 6, 2, 5 };
unsigned char Level08[] = { 4, 0, 5, 1, 3, 6, 2, 7 };
unsigned char Level09[] = { 4, 0, 5, 1, 6, 2, 7, 3, 8 };
unsigned char Level10[] = { 5, 0, 6, 1, 7, 2, 8, 3, 9, 4 };
unsigned char Level11[] = { 5, 0, 6, 1, 7, 2, 8, 3, 9, 10 };
unsigned char Level12[] = { 6, 0, 7, 1, 8, 2, 9, 3, 10, 4, 11 };
unsigned char Level13[] = { 6, 0, 1, 7, 2, 8, 3, 9, 4, 10, 5, 11, 6, 12 };
unsigned char Level14[] = { 7, 0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6 };
unsigned char Level15[] = { 7, 0, 8, 1, 9, 2, 10, 3, 11, 4, 12, 5, 13, 6, 14, 7, 15 };

unsigned char* Levels[]
    = { &Level00,    &Level01,    &Level02[0], &Level03[0], &Level04[0], &Level05[0], &Level06[0], &Level07[0],
	    &Level08[0], &Level09[0], &Level10[0], &Level11[0], &Level12[0], &Level13[0], &Level14[0], &Level15[0] };

void form::frmclr(FRMHED& destination) noexcept {
	auto head   = FRMHED{};
	destination = head;
}

bool form::internal::comp(const dPOINTLINE& point1, const dPOINTLINE& point2) noexcept {
	if (point1.y < point2.y) {
		return true;
	}
	if (point1.x < point2.x) {
		return true;
	}
	return false;
}

void form::dusqr() {
	if (UserFlagMap.test(UserFlag::SQRFIL)) {
		SelectedForm->extendedAttribute |= AT_SQR;
	}
	else {
		SelectedForm->extendedAttribute &= ~(AT_SQR);
	}
}

bool form::chkmax(unsigned int arg0, unsigned int arg1) noexcept {
	if ((arg0 & MAXMSK) != 0u) {
		return true;
	}
	if ((arg1 & MAXMSK) != 0u) {
		return true;
	}
	if (((arg1 + arg0) & MAXMSK) != 0u) {
		return true;
	}
	return false;
}

void form::fltspac(unsigned int vertexOffset, unsigned int count) {
	const auto currentVertex = (*FormList)[ClosestFormToCursor].vertexIndex + vertexOffset;
	auto       sourceStart   = std::next(FormVertices->cbegin(), currentVertex);
	auto       val           = fPOINT{};
	FormVertices->insert(sourceStart, count, val);

	for (auto iForm = ClosestFormToCursor + 1; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		form.vertexIndex += count;
	}
}

void form::delflt(unsigned int formIndex) {
	auto form = std::next(FormList->begin(), formIndex);
	if (form->vertexCount != 0u) {
		const auto vertexCount = form->vertexCount;
		auto       eraseStart  = std::next(FormVertices->cbegin(), form->vertexIndex);
		auto       eraseEnd    = std::next(eraseStart, form->vertexCount);
		FormVertices->erase(eraseStart, eraseEnd);
		for (form++; form < FormList->end(); form++) {
			form->vertexIndex -= vertexCount;
		}
	}
}

bool form::internal::chk2of() {
	if (!StateMap.test(StateFlag::SELBOX)) {
		return false;
	}
	if (UserFlagMap.test(UserFlag::FIL2OF)) {
		return false;
	}
	return true;
}

void form::internal::rotbak(double rotationAngle, const dPOINT& rotationCenter) noexcept {
	for (auto iSequence = 0u; iSequence < SequenceIndex; iSequence++) {
		thred::rotflt(OSequence[iSequence], rotationAngle, rotationCenter);
	}
}

void form::internal::delfil(unsigned attribute) {
	auto iSource = 0u;

	attribute &= TYPMSK | FRMSK;
	StateMap.reset(StateFlag::WASDEL);
	while ((StitchBuffer[iSource].attribute & (TYPMSK | FRMSK)) != attribute && iSource < PCSHeader.stitchCount) {
		iSource++;
	}
	auto iDestination = iSource;
	if (iSource < PCSHeader.stitchCount) {
		StateMap.set(StateFlag::WASDEL);
	}
	while (iSource < PCSHeader.stitchCount) {
		if ((StitchBuffer[iSource].attribute & (TYPMSK | FRMSK)) != attribute) {
			StitchBuffer[iDestination++] = StitchBuffer[iSource];
		}
		iSource++;
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestination);
}

void form::delmfil() {
	if (texture::istx(ClosestFormToCursor)) {
		texture::deltx();
	}
	clip::delmclp(ClosestFormToCursor);
	auto iDestination = 0;
	for (auto iSource = 0u; iSource < PCSHeader.stitchCount; iSource++) {
		const auto stitchAttribute = StitchBuffer[iSource].attribute;
		if ((stitchAttribute & NOTFRM) == 0u) {
			const auto attribute = ClosestFormToCursor << FRMSHFT;
			if (!((stitchAttribute & FRMSK) == attribute && ((stitchAttribute & (TYPFRM | FTHMSK)) != 0u))) {
				thred::mvstch(iDestination++, iSource);
			}
		}
		else {
			thred::mvstch(iDestination++, iSource);
		}
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestination);
}

void form::fsizpar() noexcept {
	SelectedForm->maxFillStitchLen           = IniFile.maxStitchLength;
	SelectedForm->lengthOrCount.stitchLength = UserStitchLength;
	SelectedForm->minFillStitchLen           = MinStitchLength;
}

void form::chkcont() {
	fsizpar();
	if (SelectedForm->fillType != CONTF) {
		auto guideIt = SatinGuides->begin() + SelectedForm->satinOrAngle.guide;
		if (CurrentFormGuidesCount != 0u) {
			auto shortestGuideIndex = 0u;
			auto minimumLength      = 10000u;
			for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				const auto length = guideIt[iGuide].finish - guideIt[iGuide].start;
				if (length < minimumLength) {
					minimumLength      = length;
					shortestGuideIndex = iGuide;
				}
			}
			SelectedForm->angleOrClipData.guide.start  = guideIt[shortestGuideIndex].start;
			SelectedForm->angleOrClipData.guide.finish = guideIt[shortestGuideIndex].finish;
			satin::delsac(ClosestFormToCursor);
		}
		else {
			SelectedForm->angleOrClipData.guide.start  = 1;
			SelectedForm->angleOrClipData.guide.finish = SelectedForm->vertexCount - 2;
		}
	}
	SelectedForm->type     = FRMLINE;
	SelectedForm->fillType = CONTF;
	SelectedForm->attribute |= FRECONT;
}

unsigned form::find1st() {
	if (fi::chk2of()) {
		return ClosestPointIndex;
	}
	const auto attribute = ClosestFormToCursor << 4;
	auto       iStitch   = 0u;
	while (iStitch < PCSHeader.stitchCount) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
			if ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u) {
				return iStitch;
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

void form::frmout(unsigned int formIndex) {
	auto& form = (*FormList)[formIndex];
	if (form.vertexCount != 0u) {
		auto& rectangle    = form.rectangle;
		CurrentVertexIndex = form.vertexIndex;
		auto vertexIt      = FormVertices->begin() + form.vertexIndex;
		rectangle.left = rectangle.right = vertexIt[0].x;
		rectangle.bottom = rectangle.top = vertexIt[0].y;
		for (auto iVertex = 1u; iVertex < form.vertexCount; iVertex++) {
			if (vertexIt[iVertex].x > rectangle.right) {
				rectangle.right = vertexIt[iVertex].x;
			}
			if (vertexIt[iVertex].x < rectangle.left) {
				rectangle.left = vertexIt[iVertex].x;
			}
			if (vertexIt[iVertex].y < rectangle.bottom) {
				rectangle.bottom = vertexIt[iVertex].y;
			}
			if (vertexIt[iVertex].y > rectangle.top) {
				rectangle.top = vertexIt[iVertex].y;
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

void form::sfCor2px(const fPOINT& stitchPoint, POINT& screen) noexcept {
	screen.x = dToL((stitchPoint.x - ZoomRect.left) * ZoomRatio.x + 0.5);
	screen.y = dToL(StitchWindowClientRect.bottom - (stitchPoint.y - ZoomRect.bottom) * ZoomRatio.y + 0.5);
}

void form::internal::px2stchf(const POINT& screen, fPOINT& stitchPoint) noexcept {
	const auto factorX = static_cast<double>(screen.x) / static_cast<double>(StitchWindowClientRect.right);
	stitchPoint.x      = factorX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left;
	const auto factorY = (static_cast<double>(StitchWindowClientRect.bottom) - screen.y) / StitchWindowClientRect.bottom;
	stitchPoint.y      = factorY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom;
}

void form::frmlin(unsigned int vertices, unsigned int vertexCount) {
	if (VertexCount != 0u) {
		auto& formLines = *FormLines;
		formLines.clear();
		formLines.reserve(vertexCount);
		auto vertexIt = FormVertices->begin() + vertices;
		for (auto iVertex = 0u; iVertex < vertexCount; iVertex++) {
			formLines.push_back(
			    POINT{ dToL((vertexIt[iVertex].x - ZoomRect.left) * ZoomRatio.x),
			           dToL(StitchWindowClientRect.bottom - (vertexIt[iVertex].y - ZoomRect.bottom) * ZoomRatio.y) });
		}
		formLines.push_back(POINT{ dToL((vertexIt[0].x - ZoomRect.left) * ZoomRatio.x),
		                           dToL(StitchWindowClientRect.bottom - (vertexIt[0].y - ZoomRect.bottom) * ZoomRatio.y) });
	}
}

void form::frmlin(const std::vector<fPOINT>& vertices) {
	const auto vertexMax = vertices.size();

	if (vertexMax != 0u) {
		auto& formLines = *FormLines;
		formLines.clear();
		formLines.reserve(vertexMax);
		for (auto iVertex = 0u; iVertex < vertexMax; iVertex++) {
			formLines.push_back(
			    POINT{ dToL((vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x),
			           dToL(StitchWindowClientRect.bottom - (vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y) });
		}
		formLines.push_back(POINT{ dToL((vertices[0].x - ZoomRect.left) * ZoomRatio.x),
		                           dToL(StitchWindowClientRect.bottom - (vertices[0].y - ZoomRect.bottom) * ZoomRatio.y) });
	}
}

void form::dufrm() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	PolylineInt(StitchWindowDC, FormLines->data(), NewFormVertexCount);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::unfrm() {
	if (StateMap.testAndReset(StateFlag::SHOFRM)) {
		dufrm();
	}
}

void form::mdufrm() {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	if ((*FormList)[ClosestFormToCursor].type == FRMLINE) {
		PolylineInt(StitchWindowDC, FormLines->data(), NewFormVertexCount - 1);
	}
	else {
		PolylineInt(StitchWindowDC, FormLines->data(), NewFormVertexCount);
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
		HorizontalRatio = static_cast<double>(UnzoomedRect.x) / StitchWindowClientRect.right;
		VerticalRatio   = static_cast<double>(UnzoomedRect.y) / StitchWindowClientRect.bottom;
	}
}

void form::fvars(unsigned int iForm) {
	if (!FormList->empty() && iForm < FormList->size()) {
		auto& form             = (*FormList)[iForm];
		SelectedForm           = &form;
		CurrentVertexIndex     = form.vertexIndex;
		VertexCount            = form.vertexCount;
		CurrentFormGuides      = form.satinOrAngle.guide;
		CurrentFormGuidesCount = form.satinGuideCount;
		SatinEndGuide          = form.wordParam;
	}
}

void form::setfrm() {
	auto point = fPOINT{};

	fi::rats();
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::fvars(ClosestFormToCursor);
	fi::px2stchf((*FormLines)[0], point);
	auto       vertexIt          = FormVertices->begin() + CurrentVertexIndex;
	auto&      firstVertex       = vertexIt[0];
	const auto delta             = fPOINT{ point.x - firstVertex.x, point.y - firstVertex.y };
	SelectedForm->rectangle.left = SelectedForm->rectangle.bottom = static_cast<float>(1e30);
	SelectedForm->rectangle.right = SelectedForm->rectangle.top = 0;
	for (auto iVertex = 0u; iVertex < NewFormVertexCount - 1; iVertex++) {
		vertexIt[iVertex].x += delta.x;
		vertexIt[iVertex].y += delta.y;
		if (vertexIt[iVertex].x < SelectedForm->rectangle.left) {
			SelectedForm->rectangle.left = vertexIt[iVertex].x;
		}
		if (vertexIt[iVertex].x > SelectedForm->rectangle.right) {
			SelectedForm->rectangle.right = vertexIt[iVertex].x;
		}
		if (vertexIt[iVertex].y > SelectedForm->rectangle.top) {
			SelectedForm->rectangle.top = vertexIt[iVertex].y;
		}
		if (vertexIt[iVertex].y < SelectedForm->rectangle.bottom) {
			SelectedForm->rectangle.bottom = vertexIt[iVertex].y;
		}
	}
	FormIndex++;
	StateMap.reset(StateFlag::FORMIN);
	StateMap.set(StateFlag::INIT);
	StateMap.set(StateFlag::RESTCH);
}

void form::form() {
	displayText::shoMsg((*StringTable)[STR_FMEN]);
	StateMap.set(StateFlag::FORMIN);
	StateMap.reset(StateFlag::INSRT);
	thred::duzrat();
}

void form::internal::frmsqr(unsigned iVertex) {
	POINT line[4] = {};

	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	thred::stch2pxr(vertexIt[iVertex]);
	line[1]           = StitchCoordinatesPixels;
	const auto ratio  = static_cast<double>(MulDiv(IniFile.formVertexSizePixels, *screenDPI, 96)) / StitchWindowClientRect.right;
	auto       length = (ZoomRect.right - ZoomRect.left) * ratio * 2;
	const auto delta  = dPOINT{ vertexIt[iVertex - 1].x - vertexIt[iVertex].x, vertexIt[iVertex - 1].y - vertexIt[iVertex].y };
	auto       angle  = atan2(delta.y, delta.x);
	auto       offset = dPOINT{ length * cos(angle), length * sin(angle) };
	const auto point  = fPOINT{ vertexIt[iVertex].x + offset.x, vertexIt[iVertex].y + offset.y };
	angle             = atan2(-delta.x, delta.y);
	length /= 2.0;
	offset             = dPOINT{ length * cos(angle), length * sin(angle) };
	auto adjustedPoint = fPOINT{ point.x + offset.x, point.y + offset.y };
	thred::stch2pxr(adjustedPoint);
	line[0] = line[3] = StitchCoordinatesPixels;
	adjustedPoint     = fPOINT{ point.x - offset.x, point.y - offset.y };
	thred::stch2pxr(adjustedPoint);
	line[2] = StitchCoordinatesPixels;
	Polyline(StitchWindowMemDC, line, 4);
}

void form::selsqr(const POINT& controlPoint, HDC dc) noexcept {
	POINT      line[5] = {};
	const auto offset  = gsl::narrow<LONG>(MulDiv(IniFile.formVertexSizePixels, *screenDPI, 96));

	line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
	line[0].y = line[1].y = controlPoint.y - offset;
	line[1].x = line[2].x = controlPoint.x + offset;
	line[2].y = line[3].y = controlPoint.y + offset;
	line[4].y             = controlPoint.y - offset;
	Polyline(dc, line, 5);
}

void form::internal::frmsqr0(const POINT& controlPoint) noexcept {
	POINT      line[5] = {};
	const auto offset  = gsl::narrow<LONG>(MulDiv(IniFile.formBoxSizePixels, *screenDPI, 96));

	if (offset != 0) {
		line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
		line[0].y = line[1].y = controlPoint.y - offset;
		line[1].x = line[2].x = controlPoint.x + offset + 1;
		line[2].y = line[3].y = controlPoint.y + offset + 1;
		line[4].y             = controlPoint.y - 1;
		Polyline(StitchWindowMemDC, line, 5);
	}
}

void form::internal::frmx(const POINT& controlPoint, HDC dc) noexcept {
	POINT      line[2] = {};
	const auto offset  = gsl::narrow<LONG>(MulDiv(8, *screenDPI, 96));

	SelectObject(dc, FormSelectedPen);
	line[0].x = line[1].x = controlPoint.x;
	line[0].y             = controlPoint.y + offset;
	line[1].y             = controlPoint.y - offset;
	Polyline(dc, line, 2);
	line[0].y = line[1].y = controlPoint.y;
	line[0].x             = controlPoint.x - offset;
	line[1].x             = controlPoint.x + offset;
	Polyline(dc, line, 2);
	SelectObject(dc, FormPen);
}

void form::ratsr() {
	if (StateMap.test(StateFlag::ZUMED)) {
		HorizontalRatio = static_cast<double>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
		VerticalRatio   = static_cast<double>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
	}
	else {
		HorizontalRatio = static_cast<double>(StitchWindowClientRect.right) / UnzoomedRect.x;
		VerticalRatio   = static_cast<double>(StitchWindowClientRect.bottom) / UnzoomedRect.y;
	}
}

constexpr float form::midl(float high, float low) {
	return (high - low) / 2.0f + low;
}

void form::ritfrct(unsigned int iForm, HDC dc) {
	POINT  pixelOutline[10] = {};
	fPOINT formOutline[10]  = {};

	ratsr();
	SelectObject(StitchWindowDC, FormPen);
	SetROP2(StitchWindowDC, R2_XORPEN);
	const auto& rectangle = (*FormList)[iForm].rectangle;
	SelectObject(dc, FormSelectedPen);
	formOutline[0].x = formOutline[6].x = formOutline[7].x = formOutline[8].x = rectangle.left;
	formOutline[1].x = formOutline[5].x = form::midl(rectangle.right, rectangle.left);
	formOutline[0].y = formOutline[1].y = formOutline[2].y = formOutline[8].y = rectangle.top;
	formOutline[3].y = formOutline[7].y = form::midl(rectangle.top, rectangle.bottom);
	formOutline[4].y = formOutline[5].y = formOutline[6].y = rectangle.bottom;
	formOutline[2].x = formOutline[3].x = formOutline[4].x = rectangle.right;
	for (auto controlPoint = 0u; controlPoint < 8; controlPoint++) {
		form::sfCor2px(formOutline[controlPoint], pixelOutline[controlPoint]);
	}
	form::sfCor2px(formOutline[0], pixelOutline[8]);
	Polyline(dc, pixelOutline, 9);
	for (auto controlPoint = 0u; controlPoint < 8; controlPoint++) {
		form::selsqr(pixelOutline[controlPoint], dc);
	}
	SetROP2(StitchWindowDC, R2_COPYPEN);
	if (StateMap.testAndReset(StateFlag::GRPSEL)) {
		StateMap.reset(StateFlag::SELSHO);
		SearchLineIndex = 0;
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::delfrms() {
	thred::savdo();
	FormIndex = 0;
	FormVertices->clear();
	ClipPoints->clear();
	FormList->clear();
	SatinGuides->clear();
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].attribute &= NFRM_NTYP;
		StitchBuffer[iStitch].attribute |= NOTFRM;
	}
}

void form::fselrct(unsigned int iForm) {
	const auto& form           = (*FormList)[iForm];
	fPOINT      formOutline[5] = {};
	POINT       line[6]        = {};

	formOutline[0].x = formOutline[3].x = formOutline[4].x = form.rectangle.left;
	formOutline[1].x = formOutline[2].x = form.rectangle.right;
	formOutline[0].y = formOutline[1].y = formOutline[4].y = form.rectangle.top;
	formOutline[2].y = formOutline[3].y = form.rectangle.bottom;
	for (auto iPoint = 0u; iPoint < 5; iPoint++) {
		line[iPoint].x = dToL((formOutline[iPoint].x - ZoomRect.left) * HorizontalRatio);
		line[iPoint].y = dToL((ZoomRect.top - formOutline[iPoint].y) * VerticalRatio);
		if (line[iPoint].x < SelectedFormsRect.left) {
			SelectedFormsRect.left = line[iPoint].x;
		}
		if (line[iPoint].x > SelectedFormsRect.right) {
			SelectedFormsRect.right = line[iPoint].x;
		}
		if (line[iPoint].y < SelectedFormsRect.top) {
			SelectedFormsRect.top = line[iPoint].y;
		}
		if (line[iPoint].y > SelectedFormsRect.bottom) {
			SelectedFormsRect.bottom = line[iPoint].y;
		}
	}
	line[5].x = dToL((formOutline[0].x - ZoomRect.left) * HorizontalRatio);
	line[5].y = dToL((ZoomRect.top - formOutline[0].y) * VerticalRatio);
	if (line[5].x < SelectedFormsRect.left) {
		SelectedFormsRect.left = line[5].x;
	}
	if (line[5].x > SelectedFormsRect.right) {
		SelectedFormsRect.right = line[5].x;
	}
	if (line[5].y < SelectedFormsRect.top) {
		SelectedFormsRect.top = line[5].y;
	}
	if (line[5].y > SelectedFormsRect.bottom) {
		SelectedFormsRect.bottom = line[5].y;
	}
	Polyline(StitchWindowMemDC, line, 5);
}

void form::rct2sel(const RECT& rectangle, std::vector<POINT>& line) {
	line[0].x = line[6].x = line[7].x = line[8].x = rectangle.left;
	line[1].x = line[5].x = ((rectangle.right - rectangle.left) >> 1) + rectangle.left;
	line[2].x = line[3].x = line[4].x = rectangle.right;
	line[0].y = line[1].y = line[2].y = line[8].y = rectangle.top;
	line[3].y = line[7].y = ((rectangle.bottom - rectangle.top) >> 1) + rectangle.top;
	line[4].y = line[5].y = line[6].y = rectangle.bottom;
}

void form::dubig() {
	form::rct2sel(SelectedFormsRect, *SelectedFormsLine);
	SelectObject(StitchWindowMemDC, SelectAllPen);
	Polyline(StitchWindowMemDC, SelectedFormsLine->data(), 9);
	for (auto iPoint = 0u; iPoint < 8; iPoint++) {
		form::selsqr((*SelectedFormsLine)[iPoint], StitchWindowMemDC);
	}
}

void form::internal::frmpoly(const POINT* const line, unsigned int count) noexcept {
	if (line != nullptr) {
		if (count != 0u) {
			for (auto iPoint = 0u; iPoint < count - 1; iPoint++) {
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
	for (auto iPoint = 0u; iPoint < 8; iPoint++) {
		form::selsqr((*SelectedPointsLine)[iPoint], dc);
	}
	fi::frmx(EndPointCross, dc);
}

void form::unpsel() {
	if (StateMap.testAndReset(StateFlag::SHOPSEL)) {
		form::dupsel(StitchWindowDC);
	}
}

void form::sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect) noexcept {
	screenRect.left   = dToL((stitchRect.left - ZoomRect.left) * ZoomRatio.x + 0.5);
	screenRect.right  = dToL((stitchRect.right - ZoomRect.left) * ZoomRatio.x + 0.5);
	screenRect.top    = dToL((StitchWindowClientRect.bottom) - (stitchRect.top - ZoomRect.bottom) * ZoomRatio.y + 0.5);
	screenRect.bottom = dToL((StitchWindowClientRect.bottom) - (stitchRect.bottom - ZoomRect.bottom) * ZoomRatio.y + 0.5);
}

void form::drwfrm() {
	StateMap.reset(StateFlag::SHOMOV);
	StateMap.reset(StateFlag::SHOPSEL);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	ratsr();
	thred::duzrat();
	const auto maxForm = FormList->size();
	for (auto iForm = 0u; iForm < maxForm; iForm++) {
		form::fvars(iForm);
		form::frmlin(SelectedForm->vertexIndex, VertexCount);
		if (!FormLines->empty()) {
			auto layer = ((SelectedForm->attribute & FRMLMSK) >> 1);
			if ((ActiveLayer == 0u) || (layer == 0u) || layer == ActiveLayer) {
				POINT line[2]   = {};
				auto  lastPoint = 0u;
				auto  vertexIt  = FormVertices->begin() + CurrentVertexIndex;
				if (SelectedForm->type == SAT) {
					if ((SelectedForm->attribute & FRMEND) != 0u) {
						SelectObject(StitchWindowMemDC, FormPen3px);
						Polyline(StitchWindowMemDC, FormLines->data(), 2);
						lastPoint = 1;
					}
					if (SelectedForm->wordParam != 0u) {
						SelectObject(StitchWindowMemDC, FormPen);
						fi::frmpoly(&((*FormLines)[1]), SelectedForm->wordParam);
						SelectObject(StitchWindowMemDC, FormPen3px);
						Polyline(StitchWindowMemDC, &((*FormLines)[SelectedForm->wordParam]), 2);
						SelectObject(StitchWindowMemDC, LayerPen[layer]);
						lastPoint = SelectedForm->wordParam + 1;
					}
					const auto maxGuide = (*FormList)[iForm].satinGuideCount;
					auto       guideIt  = SatinGuides->begin() + CurrentFormGuides;
					for (auto iGuide = 0u; iGuide < maxGuide; iGuide++) {
						form::sfCor2px(vertexIt[guideIt[iGuide].start], line[0]);
						form::sfCor2px(vertexIt[guideIt[iGuide].finish], line[1]);
						SelectObject(StitchWindowMemDC, FormPen);
						Polyline(StitchWindowMemDC, line, 2);
					}
				}
				SelectObject(StitchWindowMemDC, LayerPen[layer]);
				if (SelectedForm->type == FRMLINE) {
					fi::frmpoly(FormLines->data(), VertexCount);
					if (SelectedForm->fillType == CONTF) {
						dPOINT point(vertexIt[SelectedForm->angleOrClipData.guide.start]);
						thred::sCor2px(point, line[0]);
						point = vertexIt[SelectedForm->angleOrClipData.guide.finish];
						thred::sCor2px(point, line[1]);
						Polyline(StitchWindowMemDC, line, 2);
					}
				}
				else {
					fi::frmpoly(&((*FormLines)[lastPoint]), VertexCount + 1 - lastPoint);
				}
				if (ClosestFormToCursor == iForm && StateMap.test(StateFlag::FRMPSEL)) {
					auto& formLines = *FormLines;
					for (auto iVertex = 1u; iVertex < VertexCount; iVertex++) {
						if (iVertex == ClosestVertexToCursor) {
							fi::frmx(formLines[iVertex], StitchWindowMemDC);
						}
						else {
							fi::frmsqr(iVertex);
						}
					}
					if (ClosestVertexToCursor != 0u) {
						fi::frmsqr0(formLines[0]);
					}
					else {
						fi::frmx(formLines[0], StitchWindowMemDC);
					}
					displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
				}
				else {
					auto& formLines = *FormLines;
					for (auto iVertex = 1u; iVertex < VertexCount; iVertex++) {
						fi::frmsqr(iVertex);
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
		SelectedFormsRect.top = SelectedFormsRect.left = 0x7fffffff;
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
			thred::ritmov();
			(*RubberBandLine)[1].x = Msg.pt.x - StitchWindowOrigin.x;
			(*RubberBandLine)[1].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::SHOMOV);
			thred::ritmov();
		}
	}
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	form::fvars(ClosestFormToCursor);
}

void form::internal::setpoli() {
	auto form           = FRMHED{};
	form.type           = FRMFPOLY;
	form.attribute      = gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1);
	form.underlayIndent = IniFile.underlayIndent;
	FormList->push_back(form);
	StateMap.set(StateFlag::SATIN);
	StateMap.set(StateFlag::INIT);
}

void form::internal::setlin() {
	auto form      = FRMHED{};
	form.type      = FRMLINE;
	form.attribute = gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1);
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
	auto  point     = POINT{};
	auto& closeForm = (*FormList)[ClosestFormToCursor];
	auto  vertexIt  = FormVertices->begin() + closeForm.vertexIndex;
	form::sfCor2px(vertexIt[0], point);
	const auto offset    = POINT{ Msg.pt.x - StitchWindowOrigin.x - point.x + dToL(FormMoveDelta.x),
                               Msg.pt.y - StitchWindowOrigin.y - point.y + dToL(FormMoveDelta.y) };
	auto&      formLines = *FormLines;
	formLines.resize(gsl::narrow_cast<size_t>(closeForm.vertexCount) + 1);
	for (auto iForm = 0u; iForm < closeForm.vertexCount; iForm++) {
		form::sfCor2px(vertexIt[iForm], point);
		formLines[iForm].x = point.x + offset.x;
		formLines[iForm].y = point.y + offset.y;
	}
	formLines[closeForm.vertexCount] = formLines[0];
}

void form::durpoli(unsigned vertexCount) {
	if (vertexCount < 3) {
		vertexCount = 3;
	}
	const auto stepAngle = PI * 2 / vertexCount;
	// ToDo - why 500?
	const auto length = 500.0 / vertexCount * ZoomFactor * (static_cast<double>(UnzoomedRect.x) + UnzoomedRect.y)
	                    / (static_cast<double>(LHUPX) + LHUPY);
	auto newForm        = FRMHED{};
	newForm.vertexIndex = thred::adflt(vertexCount);
	newForm.vertexCount = vertexCount;
	newForm.attribute   = gsl::narrow<decltype(SelectedForm->attribute)>(ActiveLayer << 1);
	newForm.type        = FRMFPOLY;
	FormList->push_back(newForm);
	SelectedForm        = &FormList->back();
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point    = dPOINT{ SelectedPoint.x, SelectedPoint.y };
	auto angle    = 0.0;
	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex].x = point.x;
		vertexIt[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	// ToDo do I need to assign again?
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::frmout(ClosestFormToCursor);
	FormMoveDelta.x    = 0;
	FormMoveDelta.y    = 0;
	NewFormVertexCount = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	mdufrm();
}

unsigned int form::nxt(unsigned int iVertex) noexcept {
	iVertex++;
	if (iVertex > VertexCount - 1) {
		iVertex = 0;
	}
	return iVertex;
}

constexpr unsigned int form::prv(unsigned int iVertex) {
	if (iVertex != 0u) {
		iVertex--;
	}
	else {
		iVertex = VertexCount - 1;
	}
	return iVertex;
}

unsigned int form::pdir(unsigned int vertex) {
	if (StateMap.test(StateFlag::PSELDIR)) {
		return form::nxt(vertex);
	}

	return form::prv(vertex);
}

void form::pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect) noexcept {
	auto corner = POINT{ screenRect.left + StitchWindowOrigin.x, screenRect.top + StitchWindowOrigin.y };
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
	if (StateMap.test(StateFlag::FPSEL)) {
		const auto midpoint      = (SelectedVerticesRect.top - SelectedVerticesRect.bottom) * 0.5f + SelectedVerticesRect.bottom;
		auto       currentVertex = SelectedFormVertices.start;
		auto       vertexIt      = FormVertices->begin() + CurrentVertexIndex;
		for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexIt[currentVertex].y = midpoint + midpoint - vertexIt[currentVertex].y;
			currentVertex             = pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		const auto midpoint = (AllItemsRect.top - AllItemsRect.bottom) / 2.0f + AllItemsRect.bottom;
		for (auto & FormVertice : *FormVertices) {
			FormVertice.y = midpoint + midpoint - FormVertice.y;
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
		}
		for (auto & formIter : *FormList) {
				formIter.rectangle.bottom = midpoint + midpoint - formIter.rectangle.bottom;
			formIter.rectangle.top    = midpoint + midpoint - formIter.rectangle.top;
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (!SelectedFormList->empty()) {
		auto formMap = boost::dynamic_bitset<>{ FormList->size() };

		auto rectangle = fRECTANGLE{};
		form::pxrct2stch(SelectedFormsRect, rectangle);
		const auto midpoint = (rectangle.top - rectangle.bottom) / 2.0f + rectangle.bottom;
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			formMap.set(ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
				vertexIt[iVertex].y = midpoint + midpoint - vertexIt[iVertex].y;
			}
			form::frmout(ClosestFormToCursor);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			const auto decodedForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u)) {
				StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			const auto midpoint
			    = (SelectedForm->rectangle.top - SelectedForm->rectangle.bottom) / 2.0f + SelectedForm->rectangle.bottom;
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].y = midpoint + midpoint - vertexIt[iVertex].y;
			}
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor
				    && ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u)) {
					StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
				}
			}
			form::frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				auto rectangle = fRECTANGLE{};
				thred::selRct(rectangle);
				const auto midpoint = (rectangle.top - rectangle.bottom) / 2.0f + rectangle.bottom;
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].y = midpoint + midpoint - StitchBuffer[iStitch].y;
				}
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::duform(unsigned formType) {
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
	}
}

float form::internal::findDistanceToSide(const fPOINT& lineStart,
                                         const fPOINT& lineEnd,
                                         const fPOINT& point,
                                         double&       distance) noexcept {
	const auto A     = static_cast<double>(point.x) - lineStart.x;
	const auto B     = static_cast<double>(point.y) - lineStart.y;
	const auto C     = static_cast<double>(lineEnd.x) - lineStart.x;
	const auto D     = static_cast<double>(lineEnd.y) - lineStart.y;
	auto       diffX = 0.0;
	auto       diffY = 0.0;

	if ((C == 0) && (D == 0)) {
		diffX    = A;
		diffY    = B;
		distance = sqrt(diffX * diffX + diffY * diffY);
		// Arbitrarily choose the first point since start and end are the same
		return -0.1f;
	}

	const auto dot    = A * C + B * D;
	const auto len_sq = C * C + D * D;
	const auto param  = static_cast<float>(dot / len_sq);

	if (param < 0) {
		// point is nearest to the first point i.e lineStart.x and lineStart.y
		diffX = static_cast<double>(point.x) - lineStart.x;
		diffY = static_cast<double>(point.y) - lineStart.y;
	}
	else if (param > 1) {
		// point is nearest to the end point i.e lineEnd.x and lineEnd.y
		diffX = static_cast<double>(point.x) - lineEnd.x;
		diffY = static_cast<double>(point.y) - lineEnd.y;
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

bool form::closfrm() {
	if (!FormList->empty()) {
		const auto screenCoordinate = POINT{ Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
		fi::rats();
		auto closestForm   = 0u;
		auto closestVertex = 0u;
		auto point         = fPOINT{};
		auto minimumLength = 1e99;
		fi::px2stchf(screenCoordinate, point);
		const auto layerCoded  = ActiveLayer << 1;
		const auto maxForm     = FormList->size();
		const auto savedVertex = VertexCount;
		for (auto iForm = 0u; iForm < maxForm; iForm++) {
			if (StateMap.test(StateFlag::FRMSAM) && iForm == ClosestFormToCursor) {
				continue;
			}
			auto& currentForm    = (*FormList)[iForm];
			VertexCount          = currentForm.vertexCount;
			const auto formLayer = currentForm.attribute & FRMLMSK;
			if ((ActiveLayer == 0u) || (formLayer == 0u) || formLayer == layerCoded) {
				auto vertexIt = FormVertices->begin() + currentForm.vertexIndex;
				// find the closest line first and then find the closest vertex on that line
				auto       length    = 0.0;
				const auto sideCount = currentForm.vertexCount;
				for (auto iVertex = 0u; iVertex < sideCount; iVertex++) {
					const auto param = fi::findDistanceToSide(vertexIt[iVertex], vertexIt[form::nxt(iVertex)], point, length);
					if ((length < minimumLength) && (length >= 0.0)) {
						minimumLength = length;
						closestForm   = iForm;
						if (param < 0.5f) {
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
		auto& vertex = (*FormVertices)[gsl::narrow_cast<size_t>((*FormList)[closestForm].vertexIndex) + closestVertex];
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
	ratsr();
	if (SelectedForm->type == FRMLINE) {
		NewFormVertexCount = SelectedForm->vertexCount;
	}
	else {
		NewFormVertexCount = SelectedForm->vertexCount + 1;
	}
	form::frmlin(SelectedForm->vertexIndex, SelectedForm->vertexCount);
	auto  previousPoint = form::prv(ClosestVertexToCursor);
	auto& formLines     = *FormLines;
	for (auto iPoint = 0; iPoint < 3; iPoint++) {
		(*RubberBandLine)[iPoint] = formLines[previousPoint];
		previousPoint             = form::nxt(previousPoint);
	}
	thred::ritmov();
}

void form::makspac(unsigned start, unsigned count) {
	if (!form::chkmax(PCSHeader.stitchCount, count)) {
		std::copy(StitchBuffer + start,
		          StitchBuffer + PCSHeader.stitchCount,
		          stdext::make_checked_array_iterator(StitchBuffer + start + count, MAXITEMS - (start + count)));
		PCSHeader.stitchCount += gsl::narrow<decltype(PCSHeader.stitchCount)>(count);
	}
}

bool form::internal::ritlin(const fPOINT& start, const fPOINT& finish, float userStitchLen) noexcept {
	const auto delta  = dPOINT{ finish.x - start.x, finish.y - start.y };
	const auto length = hypot(delta.x, delta.y);

	InterleaveSequence[InterleaveSequenceIndex++] = start;
	if (length > MaxStitchLen) {
		auto count = gsl::narrow<unsigned int>(std::ceil(length / userStitchLen));
		if (count == 0u) {
			count = 1;
		}
		while (length / count > MaxStitchLen) {
			count++;
		}
		if (!form::chkmax(InterleaveSequenceIndex, count)) {
			const auto step  = dPOINT{ delta.x / count, delta.y / count };
			auto       point = fPOINT{ start.x + step.x, start.y + step.y };
			for (auto iStep = 0u; iStep < count - 1; iStep++) {
				if ((InterleaveSequenceIndex & MAXMSK) != 0u) {
					InterleaveSequenceIndex = MAXITEMS - 2;
					return false;
				}
				InterleaveSequence[InterleaveSequenceIndex++] = point;
				point.x += step.x;
				point.y += step.y;
			}
		}
		else {
			SequenceIndex = MAXITEMS - 2;
			return false;
		}
	}
	return true;
}

unsigned int form::closflt(float xCoordinate, float yCoordinate) {
	auto closestVertex = 0u;

	auto minimumLength = 1e99;
	auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
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

	for (auto index = 0u; index < SequenceIndex; index++) {
		InterleaveSequence[index] = OSequence[index];
	}
	InterleaveSequenceIndex = SequenceIndex;
#else

	auto minimumStitchLength = 0.0f;
	auto userStitchLen       = 0.0f;

	const auto savedIndex = InterleaveSequenceIndex;
	if (border) {
		if (SelectedForm->maxBorderStitchLen == 0.0f) {
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		}
		MaxStitchLen = SelectedForm->maxBorderStitchLen;
		if (SelectedForm->edgeType == EDGELCHAIN || SelectedForm->edgeType == EDGEOCHAIN) {
			userStitchLen = 9.0f * PFGRAN;
		}
		else {
			userStitchLen = SelectedForm->edgeStitchLen;
		}
		minimumStitchLength = SelectedForm->minBorderStitchLen;
	}
	else {
		if (SelectedForm->maxFillStitchLen == 0.0f) {
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		}
		MaxStitchLen = SelectedForm->maxFillStitchLen;
		if (clip::isclp(ClosestFormToCursor)) {
			userStitchLen = MaxStitchLen;
		}
		else {
			userStitchLen = SelectedForm->lengthOrCount.stitchLength;
		}
		minimumStitchLength = SelectedForm->minFillStitchLen;
	}
	if (userStitchLen > MaxStitchLen) {
		userStitchLen = MaxStitchLen;
	}
	bool flag = true;
	for (auto iSequence = 0u; iSequence < SequenceIndex - 1; iSequence++) {
		if (!fi::ritlin(OSequence[iSequence], OSequence[iSequence + 1], userStitchLen)) {
			flag = false;
			break;
		}
	}
	if (flag) {
		InterleaveSequence[InterleaveSequenceIndex++] = OSequence[SequenceIndex - 1];
	}
	if (minimumStitchLength == 0.0f) {
		return;
	}
	auto destination = savedIndex + 1;
	for (auto iSequence = savedIndex + 1; iSequence < InterleaveSequenceIndex; iSequence++) {
		const auto len = hypot(InterleaveSequence[iSequence].x - InterleaveSequence[iSequence - 1].x,
		                       InterleaveSequence[iSequence].y - InterleaveSequence[iSequence - 1].y);
		if (len > minimumStitchLength) {
			InterleaveSequence[destination] = InterleaveSequence[iSequence];
			destination++;
		}
	}
	InterleaveSequenceIndex = destination;
#endif
}

void form::internal::ritbrd(unsigned& interleaveSequenceIndex2) {
	if (SequenceIndex != 0u) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_BRD;
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPBRD;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->borderColor & COLMSK;
		form::chkseq(true);
		interleaveSequenceIndex2++;
	}
}

void form::internal::ritapbrd(unsigned& interleaveSequenceIndex2) {
	if (SequenceIndex != 0u) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_AP;
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPMSK;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->borderColor >> 4;
		form::chkseq(true);
		interleaveSequenceIndex2++;
	}
}

void form::internal::ritfil(unsigned& interleaveSequenceIndex2) {
	if (SequenceIndex != 0u) {
		InterleaveSequenceIndices[interleaveSequenceIndex2].index = InterleaveSequenceIndex;
		InterleaveSequenceIndices[interleaveSequenceIndex2].seq   = I_FIL;
		InterleaveSequenceIndices[interleaveSequenceIndex2].code  = TYPFRM;
		InterleaveSequenceIndices[interleaveSequenceIndex2].color = SelectedForm->fillColor;
		form::chkseq(false);
		interleaveSequenceIndex2++;
	}
}

bool form::lastch() noexcept {
	if (InterleaveSequenceIndex != 0u) {
		LastPoint = InterleaveSequence[InterleaveSequenceIndex - 1];
		return true;
	}
	{ return false; }
}

unsigned int form::getlast() {
	if (SelectedForm->fillType != 0u) {
		form::lastch();
		auto minimumLength = 1e99;
		auto closestVertex = 0u;
		auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
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

void form::filinsb(const dPOINT& point) noexcept {
	const auto delta  = dPOINT{ (point.x - SelectedPoint.x), (point.y - SelectedPoint.y) };
	const auto length = hypot(delta.x, delta.y);
	auto       count  = gsl::narrow<unsigned int>(std::round(length / MAXSTCH + 1));
	const auto step   = dPOINT{ (delta.x / count), (delta.y / count) };

	if (length > MAXSTCH) {
		count--;
		if (form::chkmax(count, SequenceIndex)) {
			return;
		}
		while (count != 0u) {
			SelectedPoint.x += step.x;
			SelectedPoint.y += step.y;
			OSequence[SequenceIndex++] = SelectedPoint;
			count--;
		}
	}
	if ((SequenceIndex & 0xffff0000) != 0u) {
		return;
	}
	OSequence[SequenceIndex].x   = point.x;
	OSequence[SequenceIndex++].y = point.y;
	SelectedPoint.x              = point.x;
	SelectedPoint.y              = point.y;
}

void form::internal::linrutb(unsigned start) {
	const auto spacing = LineSpacing;

	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = start + 1; iVertex < VertexCount; iVertex++) {
		const auto temp = dPOINT{ vertexIt[iVertex] };
		form::filinsb(temp);
	}
	const auto temp = dPOINT{ vertexIt[0] };
	form::filinsb(temp);
	LineSpacing = spacing;
}

float form::getblen() {
	auto&      form    = (*FormList)[ClosestFormToCursor];
	const auto iLength = (form.clipEntries << 16) | form.picoLength;
	return static_cast<float>(iLength);
}

void form::savblen(float fLength) {
	auto  iLength = gsl::narrow<unsigned int>(std::round(fLength));
	auto& form    = (*FormList)[ClosestFormToCursor];

	form.clipEntries = iLength >> 16;
	form.picoLength  = iLength & 0xFFFF;
}

float form::getplen() noexcept {
	const auto value = SelectedForm->picoLength;

	return (static_cast<float>(value >> 8) + (value & 0xff) / 256.0f);
}

void form::savplen(float length) {
	auto integerPart = 0.0;

	if (length > 255.0) {
		length = 255.0;
	}
	const auto fractionalPart = std::modf(length, &integerPart);
	auto       fr             = gsl::narrow<unsigned short>(std::floor(fractionalPart * 256));
	auto       num            = gsl::narrow<unsigned short>(integerPart);

	(*FormList)[ClosestFormToCursor].picoLength = (num << 8) | fr;
}

void form::internal::bdrlin(unsigned int start, unsigned int finish, double stitchSize) {
	auto       vertexIt    = FormVertices->begin() + CurrentVertexIndex;
	const auto delta       = dPOINT{ (vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y) };
	const auto length      = hypot(delta.x, delta.y);
	auto       stitchCount = 0u;
	auto       step        = dPOINT{};

	if (UserFlagMap.test(UserFlag::LINSPAC)) {
		stitchCount = gsl::narrow<unsigned>(std::round(length / stitchSize + 0.5));
		if (stitchCount != 0u) {
			step.x = delta.x / stitchCount;
			step.y = delta.y / stitchCount;
		}
	}
	else {
		stitchCount      = gsl::narrow<unsigned>(std::round((length - stitchSize / 2.0) / stitchSize + 1.0));
		const auto angle = atan2(delta.y, delta.x);
		step.x           = cos(angle) * stitchSize;
		step.y           = sin(angle) * stitchSize;
	}
	if (stitchCount != 0u) {
		auto point = dPOINT{ vertexIt[start].x + step.x, vertexIt[start].y + step.y };
		stitchCount--;
		while (stitchCount != 0u) {
			OSequence[SequenceIndex++] = point;
			point.x += step.x;
			point.y += step.y;
			stitchCount--;
		}
	}
	OSequence[SequenceIndex++] = vertexIt[finish];
}

void form::internal::brdfil(double pd_Size) {
	auto currentVertex = 0u;

	if ((SelectedForm->extendedAttribute & AT_STRT) != 0u) {
		currentVertex = SelectedForm->fillStart;
	}
	else {
		currentVertex = form::getlast();
	}
	SequenceIndex              = 0;
	auto vertexIt              = FormVertices->begin() + CurrentVertexIndex;
	OSequence[SequenceIndex++] = vertexIt[currentVertex];
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		const auto nextVertex = form::nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, pd_Size);
		currentVertex = nextVertex;
	}
	if (SelectedForm->type != FRMLINE) {
		const auto nextVertex = form::nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, pd_Size);
	}
}

void form::internal::boldlin(unsigned int start, unsigned int finish, double size) {
	auto       vertexIt = FormVertices->begin() + CurrentVertexIndex;
	const auto delta    = dPOINT{ (vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y) };
	const auto length   = hypot(delta.x, delta.y);
	auto       count    = gsl::narrow<unsigned>(std::round(length / size));

	if (count != 0u) {
		const auto step   = fPOINT{ delta.x / count, delta.y / count };
		auto       point0 = vertexIt[start];
		auto       point1 = fPOINT{ point0.x + step.x, point0.y + step.y };
		while (count != 0u) {
			OSequence[SequenceIndex++] = point1;
			OSequence[SequenceIndex++] = point0;
			OSequence[SequenceIndex++] = point1;
			point0.x += step.x;
			point0.y += step.y;
			point1.x += step.x;
			point1.y += step.y;
			count--;
		}
		OSequence[SequenceIndex++] = vertexIt[finish];
	}
	else {
		OSequence[SequenceIndex++] = vertexIt[finish];
		OSequence[SequenceIndex++] = vertexIt[start];
		OSequence[SequenceIndex++] = vertexIt[finish];
	}
}

void form::internal::bold(double size) {
	auto iOutput   = 0u;
	auto iNextLine = form::getlast();
	auto iLine     = iNextLine;

	SequenceIndex              = 0;
	auto vertexIt              = FormVertices->begin() + CurrentVertexIndex;
	OSequence[SequenceIndex++] = vertexIt[iLine];
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		iNextLine = form::nxt(iLine);
		boldlin(iLine, iNextLine, size);
		iLine = iNextLine;
	}
	if (SelectedForm->type != FRMLINE) {
		iNextLine = form::nxt(iLine);
		boldlin(iLine, iNextLine, size);
	}
	for (auto iSequence = 0u; iSequence < SequenceIndex - 1; iSequence++) {
		const auto length
		    = hypot(OSequence[iSequence + 1].x - OSequence[iSequence].x, OSequence[iSequence + 1].y - OSequence[iSequence].y);
		if (length > TINY) {
			OSequence[iOutput++] = OSequence[iSequence];
		}
	}
	OSequence[iOutput++] = vertexIt[iNextLine];
	SequenceIndex        = iOutput;
}

void form::duangs() {
	FormAngles->clear();
	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		auto& thisVertex = vertexIt[iVertex];
		auto& nextVertex = vertexIt[gsl::narrow_cast<size_t>(iVertex) + 1u];
		FormAngles->push_back(atan2(nextVertex.y - thisVertex.y, nextVertex.x - thisVertex.x));
	}
	FormAngles->push_back(atan2(vertexIt[0].y - vertexIt[VertexCount - 1].y, vertexIt[0].x - vertexIt[VertexCount - 1].x));
}

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
bool form::internal::projv(double        xCoordinate,
                           const fPOINT& lowerPoint,
                           const fPOINT& upperPoint,
                           dPOINT&       intersection) noexcept {
	const auto deltaX = static_cast<double>(upperPoint.x) - lowerPoint.x;

	intersection.x = xCoordinate;

	if (deltaX != 0.0) {
		const auto slope = (static_cast<double>(upperPoint.y) - lowerPoint.y) / deltaX;
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

bool form::cisin(float xCoordinate, float yCoordinate) {
	const auto* rectangle = &SelectedForm->rectangle;

	if (xCoordinate >= rectangle->right) {
		return false;
	}
	if (xCoordinate <= rectangle->left) {
		return false;
	}
	if (yCoordinate >= rectangle->top) {
		return false;
	}
	if (yCoordinate <= rectangle->bottom) {
		return false;
	}
	auto count        = 0u;
	auto intersection = dPOINT{};
	auto vertexIt     = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
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
	return (count & 1) != 0u;
}

/* find the intersection of two lines, one defined by point and slope, the other by the coordinates
   of the endpoints. */
bool form::internal::proj(const dPOINT& point,
                          double        slope,
                          const fPOINT& point0,
                          const fPOINT& point1,
                          dPOINT&       intersectionPoint) noexcept {
	const auto delta = dPOINT{ point1.x - point0.x, point1.y - point0.y };
	if (delta.x != 0.0) {
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

bool form::linx(const std::vector<fPOINT>& points, unsigned int start, unsigned int finish, dPOINT& intersection) {
	if (OutsidePoints != nullptr) {
		const auto delta = dPOINT{ ((*OutsidePoints)[start].x - points[start].x), ((*OutsidePoints)[start].y - points[start].y) };
		const auto point = dPOINT{ (points[start].x), (points[start].y) };

		if ((delta.x == 0.0) && (delta.y == 0.0)) {
			return false;
		}
		if (delta.x != 0.0) {
			return fi::proj(point, delta.y / delta.x, (*OutsidePoints)[finish], points[finish], intersection);
		}

		return fi::projv(point.x, points[finish], (*OutsidePoints)[finish], intersection);
	}

	return false;
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
bool form::internal::projh(double yCoordinate, const fPOINT& point0, const fPOINT& point1, dPOINT& intersection) noexcept {
	const auto deltaX = point1.x - point0.x;

	intersection.y = yCoordinate;
	if (deltaX != 0.0f) {
		const auto deltaY = point1.y - point0.y;
		if (deltaY != 0.0f) {
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

void form::internal::sprct(std::vector<VRCT2>& fillVerticalRect, unsigned start, unsigned finish) {
	const auto delta        = dPOINT{ ((*OutsidePoints)[finish].x - (*OutsidePoints)[start].x),
                               ((*OutsidePoints)[finish].y - (*OutsidePoints)[start].y) };
	auto       point        = dPOINT{};
	auto*      verticalRect = &fillVerticalRect[start];

	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	if ((delta.x != 0.0) && (delta.y != 0.0)) {
		const auto slope = -delta.x / delta.y;
		point            = vertexIt[finish];
		proj(point, slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->dopnt);
		proj(point, slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->dipnt);
		point = vertexIt[start];
		proj(point, slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->aopnt);
		proj(point, slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->aipnt);
		point = (*InsidePoints)[start];
		if (proj(point, slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->bopnt)) {
			verticalRect->bipnt = (*InsidePoints)[start];
		}
		else {
			verticalRect->bopnt = (*OutsidePoints)[start];
			point               = (*OutsidePoints)[start];
			proj(point, slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->bipnt);
		}
		point = (*InsidePoints)[finish];
		if (proj(point, slope, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->copnt)) {
			verticalRect->cipnt = (*InsidePoints)[finish];
		}
		else {
			verticalRect->copnt = (*OutsidePoints)[finish];
			point               = (*OutsidePoints)[finish];
			proj(point, slope, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->cipnt);
		}
	}
	else {
		if (delta.x != 0.0) {
			point.x = vertexIt[finish].x;
			projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->dopnt);
			projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->dipnt);
			point.x = vertexIt[start].x;
			projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->aopnt);
			projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->aipnt);
			point.x = (*InsidePoints)[start].x;
			if (projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->bopnt)) {
				verticalRect->bipnt = (*InsidePoints)[start];
			}
			else {
				verticalRect->bopnt = (*OutsidePoints)[start];
				point.x             = (*OutsidePoints)[start].x;
				projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->bipnt);
			}
			point.x = (*InsidePoints)[finish].x;
			if (projv(point.x, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->copnt)) {
				verticalRect->cipnt = (*InsidePoints)[finish];
			}
			else {
				verticalRect->copnt = (*OutsidePoints)[finish];
				point.x             = (*OutsidePoints)[finish].x;
				projv(point.x, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->cipnt);
			}
		}
		else {
			point.y = vertexIt[finish].y;
			projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->dopnt);
			projh(point.y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->dipnt);
			point.y = vertexIt[start].y;
			projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->aopnt);
			projh(point.y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->aipnt);
			point.y = (*InsidePoints)[start].y;
			if (projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->bopnt)) {
				verticalRect->bipnt = (*InsidePoints)[start];
			}
			else {
				verticalRect->bopnt = (*OutsidePoints)[start];
				point.y             = (*OutsidePoints)[start].y;
				projh(point.y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->bipnt);
			}
			point.y = (*InsidePoints)[finish].y;
			if (projh(point.y, (*OutsidePoints)[start], (*OutsidePoints)[finish], verticalRect->copnt)) {
				verticalRect->cipnt = (*InsidePoints)[finish];
			}
			else {
				verticalRect->copnt = (*OutsidePoints)[finish];
				point.y             = (*OutsidePoints)[finish].y;
				projh((*OutsidePoints)[finish].y, (*InsidePoints)[start], (*InsidePoints)[finish], verticalRect->cipnt);
			}
		}
	}
}

void form::internal::spurfn(const dPOINT& innerPoint,
                            const dPOINT& outerPoint,
                            dPOINT&       underlayInnerPoint,
                            dPOINT&       underlayOuterPoint) noexcept {
	const auto delta     = dPOINT{ outerPoint.x - innerPoint.x, outerPoint.y - innerPoint.y };
	underlayInnerPoint.x = delta.x * DIURAT + innerPoint.x;
	underlayInnerPoint.y = delta.y * DIURAT + innerPoint.y;
	underlayOuterPoint.x = delta.x * DOURAT + innerPoint.x;
	underlayOuterPoint.y = delta.y * DOURAT + innerPoint.y;
}

void form::internal::spurct(std::vector<VRCT2>&       underlayVerticalRect,
                            const std::vector<VRCT2>& fillVerticalRect,
                            unsigned                  iRect) {
	spurfn(fillVerticalRect[iRect].aipnt,
	       fillVerticalRect[iRect].aopnt,
	       underlayVerticalRect[iRect].aipnt,
	       underlayVerticalRect[iRect].aopnt);
	spurfn(fillVerticalRect[iRect].bipnt,
	       fillVerticalRect[iRect].bopnt,
	       underlayVerticalRect[iRect].bipnt,
	       underlayVerticalRect[iRect].bopnt);
	spurfn(fillVerticalRect[iRect].cipnt,
	       fillVerticalRect[iRect].copnt,
	       underlayVerticalRect[iRect].cipnt,
	       underlayVerticalRect[iRect].copnt);
	spurfn(fillVerticalRect[iRect].dipnt,
	       fillVerticalRect[iRect].dopnt,
	       underlayVerticalRect[iRect].dipnt,
	       underlayVerticalRect[iRect].dopnt);
}

void form::internal::duromb(const dPOINT& start0, const dPOINT& finish0, const dPOINT& start1, const dPOINT& finish1) {
	if (!StateMap.test(StateFlag::UND)) {
		const auto delta0  = dPOINT{ SelectedPoint.x - start0.x, SelectedPoint.y - start0.y };
		const auto delta1  = dPOINT{ SelectedPoint.x - start1.x, SelectedPoint.y - start1.y };
		const auto length0 = hypot(delta0.x, delta0.y);
		const auto length1 = hypot(delta1.x, delta1.y);
		if (length0 > length1) {
			StateMap.set(StateFlag::FILDIR);
		}
		else {
			StateMap.reset(StateFlag::FILDIR);
		}
	}
	const auto delta0  = dPOINT{ finish0.x - start0.x, finish0.y - start0.y };
	const auto delta1  = dPOINT{ finish1.x - start1.x, finish1.y - start1.y };
	const auto length0 = hypot(delta0.x, delta0.y);
	auto       count   = gsl::narrow<unsigned int>(std::round(length0 / (LineSpacing / 2.0)));
	if (count == 0u) {
		count++;
	}
	const auto step0 = dPOINT{ delta0.x / count, delta0.y / count };
	const auto step1 = dPOINT{ delta1.x / count, delta1.y / count };

	auto start00 = start0;
	auto start11 = start1;
	for (auto iStep = 0u; iStep < count; iStep++) {
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

unsigned form::psg() noexcept {
	if (PseudoRandomValue == 0u) {
		PseudoRandomValue = SEED;
	}
	const auto temp = PseudoRandomValue & 0x48000000;
	PseudoRandomValue <<= 1;
	if (temp == 0x40000000 || temp == 0x8000000) {
		PseudoRandomValue++;
	}
	return PseudoRandomValue;
}

void form::internal::spend(const std::vector<VRCT2>& fillVerticalRect, unsigned int start, unsigned int finish) {
	const auto innerDelta  = dPOINT{ (fillVerticalRect[finish].cipnt.x - fillVerticalRect[start].bipnt.x),
                                    (fillVerticalRect[finish].cipnt.y - fillVerticalRect[start].bipnt.y) };
	const auto innerLength = hypot(innerDelta.x, innerDelta.y);
	const auto outerDelta  = dPOINT{ (fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
                                    (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y) };
	const auto outerLength = hypot(outerDelta.x, outerDelta.y);
	auto       pivot       = dPOINT{};
	auto       startDelta  = dPOINT{};
	auto       finishDelta = dPOINT{};

	if (outerLength > innerLength) {
		pivot.x       = fillVerticalRect[start].cipnt.x;
		pivot.y       = fillVerticalRect[start].cipnt.y;
		startDelta.x  = fillVerticalRect[start].copnt.x - pivot.x;
		startDelta.y  = fillVerticalRect[start].copnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bopnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bopnt.y - pivot.y;
	}
	else {
		pivot.x       = fillVerticalRect[start].copnt.x;
		pivot.y       = fillVerticalRect[start].copnt.y;
		startDelta.x  = fillVerticalRect[start].cipnt.x - pivot.x;
		startDelta.y  = fillVerticalRect[start].cipnt.y - pivot.y;
		finishDelta.x = fillVerticalRect[finish].bipnt.x - pivot.x;
		finishDelta.y = fillVerticalRect[finish].bipnt.y - pivot.y;
	}
	if (hypot(SelectedPoint.x - pivot.x, SelectedPoint.y - pivot.y) > 2 * PI) {
		form::filinsb(pivot);
	}
	auto       startAngle  = atan2(startDelta.y, startDelta.x);
	const auto finishAngle = atan2(finishDelta.y, finishDelta.x);
	auto       deltaAngle  = finishAngle - startAngle;
	if (deltaAngle > PI) {
		deltaAngle -= 2 * PI;
	}
	if (deltaAngle < -PI) {
		deltaAngle += 2 * PI;
	}
	const auto radius    = hypot(startDelta.x, startDelta.y);
	const auto arc       = fabs(radius * deltaAngle);
	auto       count     = gsl::narrow<unsigned int>(std::round(arc / LineSpacing));
	const auto stepAngle = deltaAngle / count;
	if (count == 0u) {
		count = 1;
	}
	for (auto ind = 0u; ind < count; ind++) {
		const auto outerPoint = dPOINT{ pivot.x + cos(startAngle) * radius, pivot.y + sin(startAngle) * radius };
		form::filinsb(outerPoint);
		auto level = 0u;
		if ((count & 0xfffffff0) != 0u) {
			level = form::psg() % count;
		}
		else {
			level = Levels[count][ind];
		}
		const auto innerRadius = radius * level / count * 0.4;
		const auto innerPoint  = dPOINT{ pivot.x + cos(startAngle) * innerRadius, pivot.y + sin(startAngle) * innerRadius };
		form::filinsb(innerPoint);
		startAngle += stepAngle;
	}
}

void form::internal::duspnd(const std::vector<VRCT2>& underlayVerticalRect,
                            const std::vector<VRCT2>& fillVerticalRect,
                            unsigned int              start,
                            unsigned int              finish) {
	if (StateMap.test(StateFlag::UND)) {
		if (StateMap.test(StateFlag::UNDPHAS)) {
			form::filinsb(underlayVerticalRect[start].copnt);
			form::filinsb(underlayVerticalRect[start].cipnt);
			const auto delta  = dPOINT{ underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x,
                                       underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y };
			const auto length = hypot(delta.x, delta.y);
			if (length > SelectedForm->edgeStitchLen) {
				const auto angle = atan2((*InsidePoints)[finish].y - (*OutsidePoints)[finish].y,
				                         (*InsidePoints)[finish].x - (*OutsidePoints)[finish].x);
				const auto point = dPOINT{ underlayVerticalRect[finish].bopnt.x + cos(angle) * HorizontalLength2,
					                       underlayVerticalRect[finish].bopnt.y + sin(angle) * HorizontalLength2 };
				form::filinsb(point);
			}
			form::filinsb(underlayVerticalRect[finish].bipnt);
			form::filinsb(underlayVerticalRect[finish].bopnt);
		}
		else {
			form::filinsb(underlayVerticalRect[start].cipnt);
			form::filinsb(underlayVerticalRect[start].copnt);
			const auto delta  = dPOINT{ underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x,
                                       underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y };
			const auto length = hypot(delta.x, delta.y);
			if (length > SelectedForm->edgeStitchLen) {
				const auto angle = atan2((*OutsidePoints)[finish].y - (*InsidePoints)[finish].y,
				                         (*OutsidePoints)[finish].x - (*InsidePoints)[finish].x);
				const auto point = dPOINT{ underlayVerticalRect[finish].bipnt.x + cos(angle) * HorizontalLength2,
					                       underlayVerticalRect[finish].bipnt.y + sin(angle) * HorizontalLength2 };
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
                         unsigned int              startVertex,
                         const std::vector<VRCT2>& vrct) {
	auto currentVertex = startVertex;
	auto nextVertex    = form::nxt(currentVertex);

	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	SelectedPoint = vertexIt[startVertex];
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		duromb(vrct[currentVertex].bipnt, vrct[currentVertex].cipnt, vrct[currentVertex].bopnt, vrct[currentVertex].copnt);
		duspnd(underlayVerticalRect, fillVerticalRect, currentVertex, nextVertex);
		currentVertex = nextVertex;
		nextVertex    = form::nxt(nextVertex);
	}
}

void form::internal::prsmal() noexcept {
	auto iOutput       = 0u;
	auto minimumLength = USPAC * 0.8;

	if (minimumLength > HorizontalLength2) {
		minimumLength = HorizontalLength2 * 0.9;
	}
	auto iReference = 0u;
	for (auto iSequence = 1u; iSequence < SequenceIndex; iSequence++) {
		const auto delta
		    = dPOINT{ OSequence[iSequence].x - OSequence[iReference].x, OSequence[iSequence].y - OSequence[iReference].y };
		const auto length = hypot(delta.x, delta.y);
		if (length > minimumLength) {
			OSequence[iOutput++] = OSequence[iSequence];
			iReference           = iSequence;
		}
	}
	SequenceIndex = iOutput;
}

void form::internal::pbrd(double edgeSpacing) {
	const auto spacing = LineSpacing;
	const auto start   = form::getlast();

	LineSpacing           = SelectedForm->edgeSpacing;
	SequenceIndex         = 0u;
	auto fillVerticalRect = std::vector<VRCT2>{};
	fillVerticalRect.resize(VertexCount);
	auto underlayVerticalRect = std::vector<VRCT2>{};
	underlayVerticalRect.resize(VertexCount);
	satin::satout(SelectedForm->borderSize);
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, (VertexCount - 1), 0);
	spurct(underlayVerticalRect, fillVerticalRect, (VertexCount - 1));
	if ((SelectedForm->edgeType & EGUND) != 0u) {
		StateMap.reset(StateFlag::SAT1);
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		satin::satout(HorizontalLength2);
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.set(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect);
		StateMap.reset(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect);
		LineSpacing = edgeSpacing;
		prsmal();
		HorizontalLength2 = SelectedForm->borderSize;
		StateMap.reset(StateFlag::UND);
	}
	pfn(underlayVerticalRect, fillVerticalRect, start, fillVerticalRect);
	LineSpacing = spacing;
}

void form::internal::apbrd() {
	auto currentVertex = 0u;

	SequenceIndex              = 0;
	auto vertexIt              = FormVertices->begin() + CurrentVertexIndex;
	OSequence[SequenceIndex++] = vertexIt[currentVertex];
	for (auto iVertex = 0u; iVertex < VertexCount << 1; iVertex++) {
		const auto nextVertex = form::nxt(currentVertex);
		bdrlin(currentVertex, nextVertex, APSPAC);
		currentVertex = nextVertex;
	}
}

void form::internal::bhfn(unsigned int start, unsigned int finish, double spacing) {
	auto       vertexIt   = FormVertices->begin() + CurrentVertexIndex;
	const auto delta      = dPOINT{ (vertexIt[finish].x - vertexIt[start].x), (vertexIt[finish].y - vertexIt[start].y) };
	const auto length     = hypot(delta.x, delta.y);
	const auto count      = gsl::narrow<unsigned int>(std::round(length / spacing));
	const auto step       = dPOINT{ (delta.x / count), (delta.y / count) };
	auto       innerPoint = dPOINT{ vertexIt[start].x, vertexIt[start].y };

	const double rotationAngle = atan2(-delta.x, delta.y);
	const auto outerStep = dPOINT{ SelectedForm->borderSize * cos(rotationAngle), SelectedForm->borderSize * sin(rotationAngle) };
	if (count != 0u) {
		for (auto iStep = 0u; iStep < count - 1; iStep++) {
			const auto firstPoint      = dPOINT{ innerPoint.x + step.x, innerPoint.y + step.y };
			const auto outerPoint      = dPOINT{ firstPoint.x + outerStep.x, firstPoint.y + outerStep.y };
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = innerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = outerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			OSequence[SequenceIndex++] = outerPoint;
			OSequence[SequenceIndex++] = firstPoint;
			innerPoint.x += step.x;
			innerPoint.y += step.y;
		}
		const auto firstPoint      = dPOINT{ innerPoint.x + step.x, innerPoint.y + step.y };
		OSequence[SequenceIndex++] = firstPoint;
		OSequence[SequenceIndex++] = innerPoint;
	}
}

void form::internal::bhcrnr(unsigned int vertex) {
	const auto nextVertex = form::nxt(vertex);
	auto       delta      = dPOINT{};

	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	if (StateMap.test(StateFlag::INDIR)) {
		delta.x = static_cast<double>((*OutsidePoints)[nextVertex].x) - vertexIt[nextVertex].x;
		delta.y = static_cast<double>((*OutsidePoints)[nextVertex].y) - vertexIt[nextVertex].y;
	}
	else {
		delta.x = static_cast<double>((*InsidePoints)[nextVertex].x) - vertexIt[nextVertex].x;
		delta.y = static_cast<double>((*InsidePoints)[nextVertex].y) - vertexIt[nextVertex].y;
	}
	const auto length = hypot(delta.x, delta.y);
	const auto ratio  = ButtonholeCornerLength / length;
	delta.x *= ratio;
	delta.y *= ratio;
	const auto point           = dPOINT{ vertexIt[nextVertex].x + delta.x, vertexIt[nextVertex].y + delta.y };
	OSequence[SequenceIndex++] = vertexIt[nextVertex];
	OSequence[SequenceIndex++] = point;
	OSequence[SequenceIndex++] = vertexIt[nextVertex];
	OSequence[SequenceIndex++] = point;
	OSequence[SequenceIndex++] = vertexIt[nextVertex];
}

void form::internal::bhbrd(double spacing) {
	auto vertex = form::getlast();

	SequenceIndex              = 0;
	auto vertexIt              = FormVertices->begin() + CurrentVertexIndex;
	OSequence[SequenceIndex++] = vertexIt[vertex];
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		const auto nextVertex = form::nxt(vertex);
		bhfn(vertex, nextVertex, spacing);
		bhcrnr(vertex);
		vertex = nextVertex;
	}
}

void form::internal::dubfn() {
	brdfil(SelectedForm->edgeStitchLen);
	auto iForward = SequenceIndex;
	for (auto iBackward = SequenceIndex; iBackward != 0; iBackward--) {
		OSequence[iForward++] = OSequence[iBackward - 1];
	}
	SequenceIndex = iForward;
}

void form::internal::chkbrd(unsigned& interleaveSequenceIndex2) {
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->edgeType != 0u) {
		switch (SelectedForm->edgeType & NEGUND) {
		case EDGELINE: { // Line
			brdfil(SelectedForm->edgeStitchLen);
			break;
		}
		case EDGEBEAN: { // Bean
			bold(SelectedForm->edgeStitchLen);
			break;
		}
		case EDGECLIP: { // Clipboard
			clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clip::clpbrd(form::getlast());
			break;
		}
		case EDGECLIPX: { // Even Clipboard
			clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clip::duxclp();
			break;
		}
		case EDGEANGSAT: { // Angle Satin
			HorizontalLength2 = SelectedForm->borderSize;
			satin::sbrd();
			break;
		}
		case EDGEPROPSAT: { // Proportional Satin
			HorizontalLength2 = SelectedForm->borderSize;
			pbrd(SelectedForm->edgeSpacing);
			break;
		}
		case EDGEAPPL: { // Applique
			SequenceIndex = 0;
			apbrd();
			ritapbrd(interleaveSequenceIndex2);
			HorizontalLength2 = SelectedForm->borderSize;
			satin::sbrd();
			break;
		}
		case EDGEBHOL: { // BH Buttonhole
			HorizontalLength2      = SelectedForm->borderSize * 2.0;
			const auto length      = ButtonholeCornerLength;
			ButtonholeCornerLength = form::getblen();
			satin::satout(20);
			bhbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = length;
			break;
		}
		case EDGEPICOT: { // Picot
			clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clip::clpic();
			break;
		}
		case EDGEDOUBLE: { // Double
			dubfn();
			break;
		}
		case EDGELCHAIN: { // Lin Chain
			StateMap.set(StateFlag::LINCHN);
			clip::chnfn();
			break;
		}
		case EDGEOCHAIN: { // Open Chain
			StateMap.reset(StateFlag::LINCHN);
			clip::chnfn();
			break;
		}
		}
		ritbrd(interleaveSequenceIndex2);
	}
}

void form::internal::fnvrt(std::vector<fPOINT>*   currentFillVertices,
                           std::vector<unsigned>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints) {
	auto highX  = (*currentFillVertices)[0].x;
	auto lowX   = (*currentFillVertices)[0].x;
	VertexCount = gsl::narrow<decltype(VertexCount)>(currentFillVertices->size());
	for (auto fillVertex = std::next(currentFillVertices->begin(), 1); fillVertex < currentFillVertices->end(); fillVertex++) {
		if (fillVertex->x > highX) {
			highX = fillVertex->x;
		}
		if (fillVertex->x < lowX) {
			lowX = fillVertex->x;
		}
	}

	auto lineOffset            = gsl::narrow<int>(std::floor(lowX / LineSpacing));
	lowX                       = LineSpacing * lineOffset;
	auto       fillLineCount   = gsl::narrow<unsigned int>(std::floor((static_cast<double>(highX) - lowX) / LineSpacing + 1));
	const auto step            = (highX - lowX) / fillLineCount;
	auto       currentX        = lowX;
	auto       projectedPoints = std::vector<dPOINTLINE>{};
	projectedPoints.reserve(gsl::narrow_cast<size_t>(VertexCount) + 2);
	for (auto iLine = 0u; iLine < fillLineCount; iLine++) {
		auto iLineCounter = 0u;
		currentX += step;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			const auto iNextVertex = (iVertex + 1) % VertexCount;
			auto       point       = dPOINT{};
			if (projv(currentX, (*currentFillVertices)[iVertex], (*currentFillVertices)[iNextVertex], point)) {
				iLineCounter++;
			}
		}
		fillLineCount += iLineCounter;
	}
	lineEndpoints.reserve(gsl::narrow_cast<size_t>(fillLineCount) + 1);
	auto lineGroupIndex = 0u;
	// groupIndex cannot be more than fillLineCount so reserve that amount of memory to reduce re-allocations
	groupIndexSequence.reserve(fillLineCount);
	currentX = lowX;
	for (auto iLine = 0u; iLine < fillLineCount; iLine++) {
		projectedPoints.clear();
		projectedPoints.reserve(VertexCount);
		currentX += step;
		auto iPoint = 0u;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			const auto iNextVertex = (iVertex + 1) % VertexCount;
			auto       point       = dPOINT{};
			if (projv(currentX, (*currentFillVertices)[iVertex], (*currentFillVertices)[iNextVertex], point)) {
				const auto a = dPOINTLINE{ point.x, point.y, gsl::narrow<unsigned short>(iVertex) };
				projectedPoints.push_back(a);
				iPoint++;
			}
		}
		if (iPoint > 1) {
			const auto evenPointCount = iPoint &= 0xfffffffe;
			groupIndexSequence.push_back(gsl::narrow<unsigned int>(lineEndpoints.size()));
			std::sort(projectedPoints.begin(), projectedPoints.end(), comp);
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
	groupIndexSequence.push_back(gsl::narrow<unsigned int>(lineEndpoints.size()));
}

void form::internal::fnang(std::vector<unsigned>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints,
                           double                 rotationAngle,
                           dPOINT&                rotationCenter,
                           FRMHED&                angledForm) {
	angledForm          = (*FormList)[ClosestFormToCursor];
	const auto& angRect = angledForm.rectangle;
	rotationCenter.x    = ((static_cast<double>(angRect.right) - angRect.left) / 2.0 + angRect.left);
	rotationCenter.y    = ((static_cast<double>(angRect.top) - angRect.bottom) / 2.0 + angRect.bottom);
	AngledFormVertices->clear();
	AngledFormVertices->reserve(angledForm.vertexCount);
	auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
	for (auto iVertex = 0u; iVertex < angledForm.vertexCount; iVertex++) {
		AngledFormVertices->push_back(vertexIt[iVertex]);
		thred::rotflt(AngledFormVertices->back(), rotationAngle, rotationCenter);
	}
	angledForm.vertexIndex = 0;
	SelectedForm           = &angledForm;
	fi::fnvrt(AngledFormVertices, groupIndexSequence, lineEndpoints);
	SelectedForm = &((*FormList)[ClosestFormToCursor]);
}

void form::internal::fnhor(std::vector<unsigned>& groupIndexSequence,
                           std::vector<SMALPNTL>& lineEndpoints,
                           const double           rotationAngle,
                           dPOINT&                rotationCenter,
                           FRMHED&                angledForm) {
	angledForm          = (*FormList)[ClosestFormToCursor];
	const auto& angRect = angledForm.rectangle;
	rotationCenter.x    = ((static_cast<double>(angRect.right) - angRect.left) / 2.0 + angRect.left);
	rotationCenter.y    = ((static_cast<double>(angRect.top) - angRect.bottom) / 2.0 + angRect.bottom);
	AngledFormVertices->clear();
	AngledFormVertices->reserve(angledForm.vertexCount);
	auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
	for (auto iVertex = 0u; iVertex < angledForm.vertexCount; iVertex++) {
		AngledFormVertices->push_back(vertexIt[iVertex]);
		thred::rotflt(AngledFormVertices->back(), rotationAngle, rotationCenter);
	}
	angledForm.vertexIndex = 0;
	SelectedForm           = &angledForm;
	fi::fnvrt(AngledFormVertices, groupIndexSequence, lineEndpoints);
	SelectedForm = &((*FormList)[ClosestFormToCursor]);
}

void form::internal::prebrd(FRMHED& angledForm) {
	auto  vertexIt           = FormVertices->begin() + CurrentVertexIndex;
	auto  delta              = fPOINT{ (vertexIt[1].x - vertexIt[0].x), (vertexIt[1].y - vertexIt[0].y) };
	auto  ratio              = 0.0;
	auto& angledFormVertices = *AngledFormVertices;
	angledFormVertices.resize(gsl::narrow_cast<size_t>(VertexCount) + 3u);
	auto output = angledFormVertices.begin();
	output++;
	std::copy(vertexIt, vertexIt + VertexCount, output);
	if (fabs(delta.x) > fabs(delta.y)) {
		ratio = fabs(0.1 / delta.x);
	}
	else {
		ratio = fabs(0.1 / delta.y);
	}
	angledFormVertices[0].x = vertexIt[0].x - delta.x * ratio;
	angledFormVertices[0].y = vertexIt[0].y - delta.y * ratio;
	angledForm              = *SelectedForm;
	angledForm.vertexIndex  = 0;
	angledForm.vertexCount += 3;
	delta.x = vertexIt[VertexCount - 1].x - vertexIt[VertexCount - 2].x;
	delta.y = vertexIt[VertexCount - 1].y - vertexIt[VertexCount - 2].y;
	if (fabs(delta.x) > fabs(delta.y)) {
		ratio = fabs(0.1 / delta.x);
	}
	else {
		ratio = fabs(0.1 / delta.y);
	}
	angledFormVertices[angledForm.vertexCount - 1].x = vertexIt[VertexCount - 1].x + delta.x * ratio;
	angledFormVertices[angledForm.vertexCount - 1].y = vertexIt[VertexCount - 1].y + delta.y * ratio;
	SelectedForm                                     = &angledForm;
	VertexCount                                      = angledForm.vertexCount;
	CurrentVertexIndex                               = angledForm.vertexIndex;
}

void form::internal::plfn(const std::vector<VRCT2>& underlayVerticalRect,
                          const std::vector<VRCT2>& fillVerticalRect,
                          const std::vector<VRCT2>& prct) {
	duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt);
	duspnd(underlayVerticalRect, fillVerticalRect, 1, 2);
	for (auto iVertex = 2u; iVertex < VertexCount - 4; iVertex++) {
		duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt);
		duspnd(underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1);
	}
	duromb(prct[VertexCount - 4].bipnt, prct[VertexCount - 4].dipnt, prct[VertexCount - 4].bopnt, prct[VertexCount - 4].dopnt);
}

void form::internal::plbak(unsigned int backPoint) noexcept {
	if (backPoint < ((sizeof(OSequence) / sizeof(OSequence[0])) - 1)) {
		auto iSequence = SequenceIndex;
		if (iSequence != 0u) {
			iSequence--;
		}
		while (iSequence > backPoint) {
			std::swap(OSequence[iSequence], OSequence[backPoint]);
			iSequence--;
			backPoint++;
		}
	}
}

void form::internal::plbrd(double edgeSpacing, FRMHED& angledForm) {
	// Ensure that we have at least 4 array members
	auto fillVerticalRect = std::vector<VRCT2>{};
	fillVerticalRect.resize(gsl::narrow_cast<size_t>(VertexCount) + 3);
	auto underlayVerticalRect = std::vector<VRCT2>{};
	underlayVerticalRect.resize(gsl::narrow_cast<size_t>(VertexCount) + 3);
	prebrd(angledForm);
	satin::satout(SelectedForm->borderSize);
	InsidePoints->push_back((*InsidePoints)[0]);
	OutsidePoints->push_back((*OutsidePoints)[0]);
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		sprct(fillVerticalRect, iVertex, iVertex + 1);
		spurct(underlayVerticalRect, fillVerticalRect, iVertex);
	}
	sprct(fillVerticalRect, (VertexCount - 1), 0);
	spurct(underlayVerticalRect, fillVerticalRect, (VertexCount - 1));
	auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
	if ((SelectedForm->attribute & SBLNT) == 0u) {
		const auto& val = vertexIt[1];

		fillVerticalRect[1].aipnt     = val;
		fillVerticalRect[1].aopnt     = val;
		underlayVerticalRect[1].aipnt = val;
		underlayVerticalRect[1].aopnt = val;
	}
	if ((SelectedForm->attribute & FBLNT) == 0u) {
		const auto& val = vertexIt[VertexCount - 1];

		fillVerticalRect[VertexCount - 4].dipnt     = val;
		fillVerticalRect[VertexCount - 4].dopnt     = val;
		underlayVerticalRect[VertexCount - 4].dipnt = val;
		underlayVerticalRect[VertexCount - 4].dopnt = val;
	}
	SequenceIndex = 0;
	vertexIt      = FormVertices->begin() + CurrentVertexIndex;
	SelectedPoint = vertexIt[0];
	if ((SelectedForm->edgeType & EGUND) != 0u) {
		LineSpacing = USPAC;
		StateMap.set(StateFlag::UND);
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		StateMap.set(StateFlag::UNDPHAS);
		StateMap.reset(StateFlag::FILDIR);
		plfn(underlayVerticalRect, fillVerticalRect, underlayVerticalRect);
		const auto savedIndex = SequenceIndex;
		StateMap.reset(StateFlag::UNDPHAS);
		SelectedPoint = vertexIt[0];
		StateMap.set(StateFlag::FILDIR);
		plfn(underlayVerticalRect, fillVerticalRect, underlayVerticalRect);
		plbak(savedIndex);
		prsmal();
		if (SequenceIndex != 0u) { // ensure that we can do a valid read from OSequence
			SelectedPoint = OSequence[SequenceIndex - 1];
		}
	}
	StateMap.reset(StateFlag::UND);
	LineSpacing = SelectedForm->edgeSpacing;
	plfn(underlayVerticalRect, fillVerticalRect, fillVerticalRect);
	LineSpacing = edgeSpacing;
	form::fvars(ClosestFormToCursor);
}

void form::internal::lapbrd() {
	const auto savedStitchLength = UserStitchLength;

	SequenceIndex = 0;
	// ToDo - Should APSPAC be a configurable variable instead?
	UserStitchLength = APSPAC;
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		bdrlin(iVertex, iVertex + 1, APSPAC);
	}
	for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
		bdrlin(iVertex, iVertex - 1, APSPAC);
	}
	UserStitchLength = savedStitchLength;
}

void form::internal::blbrd(double spacing) {
	SequenceIndex              = 0;
	auto vertexIt              = FormVertices->begin() + CurrentVertexIndex;
	OSequence[SequenceIndex++] = vertexIt[0];
	for (auto iVertex = 0u; iVertex < VertexCount - 2; iVertex++) {
		bhfn(iVertex, iVertex + 1, spacing);
		bhcrnr(iVertex);
	}
	bhfn(VertexCount - 2, VertexCount - 1, spacing);
	OSequence[SequenceIndex++] = vertexIt[VertexCount - 1];
}

void form::internal::contf() {
	const auto start           = SelectedForm->angleOrClipData.guide.start;
	auto       vertexIt        = FormVertices->begin() + CurrentVertexIndex;
	auto&      selectionStart  = vertexIt[start];
	const auto finish          = SelectedForm->angleOrClipData.guide.finish;
	const auto lowVertexIndex  = start;
	const auto highVertexIndex = VertexCount - start - 1;

	auto lowCounts = std::vector<unsigned>{};
	lowCounts.resize(lowVertexIndex);
	auto highCounts = std::vector<unsigned>{};
	highCounts.resize(highVertexIndex);
	auto lowLengths = std::vector<float>{};
	lowLengths.resize(lowVertexIndex);
	auto highLengths = std::vector<float>{};
	highLengths.resize(highVertexIndex);
	auto lowDeltas = std::vector<fPOINT>{};
	lowDeltas.resize(lowVertexIndex);
	auto highDeltas = std::vector<fPOINT>{};
	highDeltas.resize(highVertexIndex);
	auto lowSteps = std::vector<dPOINT>{};
	lowSteps.resize(lowVertexIndex);
	auto highSteps = std::vector<dPOINT>{};
	highSteps.resize(highVertexIndex);
	auto lowVertices = std::vector<fPOINT>{};
	lowVertices.resize(lowVertexIndex);
	auto highVertices = std::vector<dPOINT>{};
	highVertices.resize(highVertexIndex);

	auto lowLength = 0.0f;
	// ToDo - rename pols, polref, polin & poldif

	SequenceIndex = 0;
	auto lowIndex = 0u;
	for (auto iVertex = lowVertexIndex; iVertex != 0; iVertex--) {
		lowVertices[lowIndex] = vertexIt[iVertex];
		lowDeltas[lowIndex].x = vertexIt[iVertex - 1].x - vertexIt[iVertex].x;
		lowDeltas[lowIndex].y = vertexIt[iVertex - 1].y - vertexIt[iVertex].y;
		lowLengths[lowIndex]  = hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
		lowLength += lowLengths[lowIndex];
		lowIndex++;
	}
	auto       selind              = 0u;
	const auto selectedVertexCount = finish - start;
	auto       pols                = std::vector<PVEC>{};
	pols.resize(selectedVertexCount);
	for (auto iVertex = start + 1; iVertex <= finish; iVertex++) {
		const auto delta = dPOINT{ vertexIt[iVertex].x - selectionStart.x, vertexIt[iVertex].y - selectionStart.y };
		pols[selind]     = { atan2(delta.y, delta.x), hypot(delta.x, delta.y) };
		selind++;
	}
	auto highIndex  = 0u;
	auto highLength = 0.0f;
	for (auto iVertex = finish; iVertex < VertexCount - 1; iVertex++) {
		auto& thisVertex          = vertexIt[iVertex];
		auto& nextVertex          = vertexIt[gsl::narrow_cast<size_t>(iVertex) + 1u];
		highVertices[highIndex].x = thisVertex.x;
		highVertices[highIndex].y = thisVertex.y;
		highDeltas[highIndex].x   = nextVertex.x - thisVertex.x;
		highDeltas[highIndex].y   = nextVertex.y - thisVertex.y;
		highLengths[highIndex]    = hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
		highLength += highLengths[highIndex];
		highIndex++;
	}
	auto highSpacing = SelectedForm->fillSpacing;
	auto lowSpacing  = SelectedForm->fillSpacing * lowLength / highLength;
	if (highLength < lowLength) {
		std::swap(highSpacing, lowSpacing);
	}
	for (auto iVertex = 0u; iVertex < lowVertexIndex; iVertex++) {
		lowCounts[iVertex]  = gsl::narrow<unsigned int>(std::round(lowLengths[iVertex] / lowSpacing));
		lowSteps[iVertex].x = lowDeltas[iVertex].x / lowCounts[iVertex];
		lowSteps[iVertex].y = lowDeltas[iVertex].y / lowCounts[iVertex];
	}
	for (auto iVertex = 0u; iVertex < highVertexIndex; iVertex++) {
		highCounts[iVertex]  = gsl::narrow<unsigned int>(std::round(highLengths[iVertex] / highSpacing));
		highSteps[iVertex].x = highDeltas[iVertex].x / highCounts[iVertex];
		highSteps[iVertex].y = highDeltas[iVertex].y / highCounts[iVertex];
	}
	lowIndex = highIndex = 0;
	StateMap.reset(StateFlag::FILDIR);
	auto       lowCount  = 0u;
	auto       highCount = 0u;
	auto       delta     = dPOINT{ vertexIt[finish].x - vertexIt[start].x, vertexIt[finish].y - vertexIt[start].y };
	const auto polref    = PVEC{ atan2(delta.y, delta.x), hypot(delta.x, delta.y) };
	auto       lowStep   = dPOINT{};
	auto       lowPoint  = dPOINT{};
	auto       highStep  = dPOINT{};
	auto       highPoint = dPOINT{};
	while ((lowCount != 0u) || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
		if (lowCount != 0u) {
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
		if (highCount != 0u) {
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
		delta.x          = highPoint.x - lowPoint.x;
		delta.y          = highPoint.y - lowPoint.y;
		const auto polin = PVEC{ atan2(delta.y, delta.x), hypot(delta.x, delta.y) };
		if (polref.length > 0.9 * LineSpacing) {
			const auto poldif = PVEC{ polin.angle - polref.angle, polin.length / polref.length };
			if (StateMap.testAndFlip(StateFlag::FILDIR)) {
				OSequence[SequenceIndex].x = lowPoint.x;
				OSequence[SequenceIndex].y = lowPoint.y;
				SequenceIndex++;
				for (auto iVertex = 0u; iVertex < (selectedVertexCount - 1); iVertex++) {
					const auto length          = pols[iVertex].length * poldif.length;
					const auto angle           = pols[iVertex].angle + poldif.angle;
					OSequence[SequenceIndex].x = lowPoint.x + cos(angle) * length;
					OSequence[SequenceIndex].y = lowPoint.y + sin(angle) * length;
					SequenceIndex++;
				}
			}
			else {
				OSequence[SequenceIndex].x = highPoint.x;
				OSequence[SequenceIndex].y = highPoint.y;
				SequenceIndex++;
				for (auto iVertex = selectedVertexCount - 1; iVertex != 0; iVertex--) {
					const auto length          = pols[iVertex - 1].length * poldif.length;
					const auto angle           = pols[iVertex - 1].angle + poldif.angle;
					OSequence[SequenceIndex].x = lowPoint.x + cos(angle) * length;
					OSequence[SequenceIndex].y = lowPoint.y + sin(angle) * length;
					SequenceIndex++;
				}
			}
		}
		lowPoint.x += lowStep.x;
		lowPoint.y += lowStep.y;
		highPoint.x += highStep.x;
		highPoint.y += highStep.y;
	}
	if (StateMap.test(StateFlag::FILDIR)) {
		OSequence[SequenceIndex++] = vertexIt[0];
	}
	else {
		OSequence[SequenceIndex++] = vertexIt[VertexCount - 1];
	}
	if (SelectedForm->lengthOrCount.stitchLength < MinStitchLength) {
		SelectedForm->lengthOrCount.stitchLength = MinStitchLength;
	}
}

void form::internal::duflt(float& formOffset, std::vector<fPOINT>* currentFormVertices) {
	if (currentFormVertices != nullptr) {
		auto iVertex  = 0u;
		auto leftEdge = 1e9f;

		auto vertexIt = currentFormVertices->begin();
		for (iVertex = 0; iVertex < currentFormVertices->size(); iVertex++) {
			if (vertexIt[iVertex].x < leftEdge) {
				leftEdge = vertexIt[iVertex].x;
			}
		}
		if (leftEdge < ClipRectSize.cx) {
			StateMap.set(StateFlag::WASNEG);
			formOffset = ClipRectSize.cx + fabs(leftEdge) + .05;
			for (iVertex = 0; iVertex < currentFormVertices->size(); iVertex++) {
				vertexIt[iVertex].x += formOffset;
			}
			SelectedForm->rectangle.left += formOffset;
			SelectedForm->rectangle.right += formOffset;
		}
		else {
			StateMap.reset(StateFlag::WASNEG);
		}
	}
}

unsigned form::internal::leftsid(std::vector<fPOINT>* currentFormVertices) {
	auto leftVertex = 0u;
	if (currentFormVertices != nullptr) {
		auto iVertex = 0u;
		auto minimumX = 1e9f;

		auto vertexIt = currentFormVertices->begin();
		for (iVertex = 0; iVertex < currentFormVertices->size(); iVertex++) {
			if (vertexIt[iVertex].x < minimumX) {
				minimumX = vertexIt[iVertex].x;
				leftVertex = iVertex;
			}
		}
	}
	return leftVertex;
}

bool form::internal::clpcmp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept {
	return (vclpx1.segment < vclpx2.segment);
}

bool form::internal::isin(std::vector<VCLPX>&  regionCrossingData,
                          float                xCoordinate,
                          float                yCoordinate,
                          unsigned             regionCrossingStart,
                          unsigned             regionCrossingEnd,
                          const fRECTANGLE&    boundingRect,
                          std::vector<fPOINT>* currentFormVertices) {
	if (currentFormVertices != nullptr) {
		auto count = 0u;
		auto point = dPOINT{};

		if (xCoordinate < boundingRect.left) {
			return false;
		}
		if (xCoordinate > boundingRect.right) {
			return false;
		}
		if (yCoordinate < boundingRect.bottom) {
			return false;
		}
		if (yCoordinate > boundingRect.top) {
			return false;
		}
		auto vertexIt = currentFormVertices->begin();
		for (auto iRegion = regionCrossingStart; iRegion < regionCrossingEnd; iRegion++) {
			const auto startVertex = regionCrossingData[iRegion].vertex;
			const auto endVertex = form::nxt(startVertex);
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
		return (count & 1) != 0u;
	}
	
		return false;
	
}

void form::internal::inspnt(std::vector<CLIPNT>& clipStitchPoints) {
	CLIPNT clipStitchPoint{ clipStitchPoints.back().x, clipStitchPoints.back().y, 0, 0 };
	clipStitchPoints.back().x    = form::midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2].x);
	clipStitchPoints.back().y    = form::midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2].y);
	clipStitchPoints.back().flag = 1;
	clipStitchPoints.push_back(clipStitchPoint);
}

bool form::internal::isect(unsigned int         vertex0,
                           unsigned int         vertex1,
                           fPOINT&              intersection,
                           float&               length,
                           const fPOINT&        lineSegmentStart,
                           const fPOINT&        lineSegmentEnd,
                           std::vector<fPOINT>* currentFormVertices) {
	if (currentFormVertices != nullptr) {
		const auto delta = dPOINT{ (lineSegmentEnd.x - lineSegmentStart.x), (lineSegmentEnd.y - lineSegmentStart.y) };
		const auto point = dPOINT{ (lineSegmentStart.x), (lineSegmentStart.y) };
		auto       tempIntersection = dPOINT{};
		auto       flag = false;
		auto       left = 0.0f;
		auto       right = 0.0f;

		auto vertexIt = currentFormVertices->begin();
		if ((delta.x != 0.0) && (delta.y != 0.0)) {
			flag = proj(point, delta.y / delta.x, vertexIt[vertex0], vertexIt[vertex1], tempIntersection);
		}
		else {
			if (delta.y != 0.0) {
				flag = projv(point.x, vertexIt[vertex0], vertexIt[vertex1], tempIntersection);
			}
			else {
				if (delta.x != 0.0) {
					flag = projh(point.y, vertexIt[vertex0], vertexIt[vertex1], tempIntersection);
				}
				else if (vertexIt[vertex0].y == lineSegmentStart.y && vertexIt[vertex1].y == lineSegmentStart.y) {
					if (vertexIt[vertex0].x < vertexIt[vertex1].x) {
						left = vertexIt[vertex0].x;
						right = vertexIt[vertex1].x;
					}
					else {
						left = vertexIt[vertex1].x;
						right = vertexIt[vertex0].x;
					}
					if (lineSegmentStart.x > left && lineSegmentStart.x < right) {
						intersection.x = lineSegmentStart.x;
						intersection.y = lineSegmentStart.y;
						length = 0;
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
			tempIntersection.x = 0;
		}
		if (tempIntersection.y < TINY) {
			tempIntersection.y = 0;
		}
		intersection.x = static_cast<float>(tempIntersection.x);
		intersection.y = static_cast<float>(tempIntersection.y);
		length = hypot(tempIntersection.x - lineSegmentStart.x, tempIntersection.y - lineSegmentStart.y);
		// ToDo - should length be determined from start or end?
		//	 hypot(tempIntersection.x-lineSegmentEnd.x,tempIntersection.y-lineSegmentEnd.y);
		return flag;
	}
	
		return false;
	
}

bool form::internal::lencmpa(const CLIPSORT* const arg1, const CLIPSORT* const arg2) noexcept {
	const auto local1 = arg1->segmentLength;
	const auto local2 = arg2->segmentLength;

	return (local1 < local2);
}

void form::internal::mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, unsigned destination, unsigned source) {
	if (destination != source) {
		arrayOfClipIntersectData[destination] = arrayOfClipIntersectData[source];
	}
}

unsigned form::internal::insect(std::vector<CLIPSORT>&    clipIntersectData,
                                const std::vector<VCLPX>& regionCrossingData,
                                std::vector<CLIPSORT*>&   arrayOfClipIntersectData,
                                unsigned                  regionCrossingStart,
                                unsigned                  regionCrossingEnd,
                                const fPOINT&             lineSegmentStart,
                                const fPOINT&             lineSegmentEnd,
                                std::vector<fPOINT>*      currentFormVertices) {
	auto  iRegions        = 0u;
	auto  iIntersection   = 0u;
	auto  count           = 0u;
	auto  lineSegmentRect = fRECTANGLE{};
	auto* intersection    = static_cast<fPOINT*>(nullptr);

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
		std::sort(arrayOfClipIntersectData.begin(), arrayOfClipIntersectData.end(), lencmpa);
		auto iDestination = 1u;
		for (iIntersection = 0; iIntersection < count - 1; iIntersection++) {
			if (fabs(static_cast<double>(arrayOfClipIntersectData[iIntersection]->segmentLength)
			         - arrayOfClipIntersectData[static_cast<size_t>(iIntersection) + 1]->segmentLength)
			    > TINY) {
				mvpclp(arrayOfClipIntersectData, iDestination++, iIntersection + 1);
			}
		}
		count = iDestination;
	}
	return count;
}

float form::internal::getlen(std::vector<CLIPNT>&       clipStitchPoints,
                             const std::vector<double>& lengths,
                             unsigned                   iPoint,
                             std::vector<fPOINT>*       currentFormVertices) {
	auto retval = 0.0f;
	if (currentFormVertices != nullptr) {
		clipStitchPoints[iPoint].vertexIndex %= currentFormVertices->size();
		auto vertexIt = currentFormVertices->begin();
		retval = lengths[clipStitchPoints[iPoint].vertexIndex]
		         + hypot(vertexIt[clipStitchPoints[iPoint].vertexIndex].x - clipStitchPoints[iPoint].x,
		                 vertexIt[clipStitchPoints[iPoint].vertexIndex].y - clipStitchPoints[iPoint].y);
	}
	return retval;
}

unsigned form::internal::clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
                                 std::vector<CLPSEG>&       clipSegments,
                                 const std::vector<double>& lengths,
                                 unsigned                   start,
                                 unsigned                   finish,
                                 std::vector<fPOINT>*       currentFormVertices) {
	const auto clipSegment = CLPSEG{ start,  getlen(clipStitchPoints, lengths, start, currentFormVertices),
		                             0u,     clipStitchPoints[start].vertexIndex,
		                             finish, getlen(clipStitchPoints, lengths, finish, currentFormVertices),
		                             0u,     clipStitchPoints[finish].vertexIndex,
		                             0 };
	clipSegments.push_back(clipSegment);
	return finish + 1;
}

bool form::internal::lencmp(const LENINFO& arg1, const LENINFO& arg2) noexcept {
	return (arg1.length < arg2.length);
}

void form::internal::chksid(unsigned int vertexIndex, unsigned clipIntersectSide, std::vector<fPOINT>* currentFormVertices) {
	if (currentFormVertices != nullptr) {
		if (clipIntersectSide != vertexIndex) {
			auto vertexIt = currentFormVertices->begin();
			const auto vertexCount = currentFormVertices->size();
			if ((vertexIndex - clipIntersectSide + vertexCount) % vertexCount < (vertexCount >> 1)) {
				auto       iVertex = form::nxt(clipIntersectSide);
				const auto limit = form::nxt(vertexIndex);
				while (iVertex != limit) {
					OSequence[SequenceIndex++] = vertexIt[iVertex];
					iVertex = form::nxt(iVertex);
				}
			}
			else {
				auto iVertex = clipIntersectSide;
				while (iVertex != vertexIndex) {
					OSequence[SequenceIndex++] = vertexIt[iVertex];
					iVertex = form::prv(iVertex);
				}
			}
		}
	}
}

void form::internal::ritseg(const std::vector<CLIPNT>& clipStitchPoints,
                            std::vector<CLPSEG>&       clipSegments,
                            const unsigned             currentSegmentIndex,
                            unsigned&                  clipIntersectSide,
                            std::vector<fPOINT>*       currentFormVertices) {
	auto isPointedEnd = true;

	if ((SelectedForm->extendedAttribute & AT_SQR) != 0u) {
		isPointedEnd = false;
	}
	if (StateMap.test(StateFlag::FILDIR)) {
		auto iPoint = clipSegments[currentSegmentIndex].start;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd) {
			iPoint++;
		}
		chksid(clipSegments[currentSegmentIndex].asid, clipIntersectSide, currentFormVertices);
		while (iPoint <= clipSegments[currentSegmentIndex].finish) {
			OSequence[SequenceIndex].x   = clipStitchPoints[iPoint].x;
			OSequence[SequenceIndex++].y = clipStitchPoints[iPoint++].y;
		}
		clipIntersectSide = clipSegments[currentSegmentIndex].zsid;
	}
	else {
		auto iPoint = clipSegments[currentSegmentIndex].finish;
		if (StateMap.test(StateFlag::TXFIL) && isPointedEnd) {
			iPoint--;
		}
		chksid(clipSegments[currentSegmentIndex].zsid, clipIntersectSide, currentFormVertices);
		if (clipSegments[currentSegmentIndex].start != 0u) {
			while (iPoint >= clipSegments[currentSegmentIndex].start) {
				OSequence[SequenceIndex].x   = clipStitchPoints[iPoint].x;
				OSequence[SequenceIndex++].y = clipStitchPoints[iPoint--].y;
			}
		}
		else {
			while (iPoint < clipSegments[currentSegmentIndex].start) {
				OSequence[SequenceIndex].x   = clipStitchPoints[iPoint].x;
				OSequence[SequenceIndex++].y = clipStitchPoints[iPoint--].y;
			}
		}
		clipIntersectSide = clipSegments[currentSegmentIndex].asid;
	}
	clipSegments[currentSegmentIndex].dun = 1;
}

bool form::internal::clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned sind) {
	auto       index        = 1u;
	const auto indexDoubled = gsl::narrow<unsigned int>(clipSegments.size()) << 1;

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

bool form::internal::nucseg(const std::vector<CLPSEG>&  clipSegments,
                            const std::vector<LENINFO>& sortedLengths,
                            unsigned&                   currentSegmentIndex) {
	auto index = 0u;

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

bool form::internal::vscmp(unsigned index1, unsigned index2) noexcept {
	if (OSequence[index1].x != OSequence[index2].x) {
		return true;
	}

	return OSequence[index1].y != OSequence[index2].y;
}

void form::internal::clpcon(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>* currentFormVertices) {
	const auto currentVertexCount = currentFormVertices->size();
	auto formOffset         = 0.0f;
	duflt(formOffset, currentFormVertices);
	auto clipWidth = ClipRectSize.cx + SelectedForm->fillSpacing;
	if (StateMap.test(StateFlag::ISUND)) {
		clipWidth = SelectedForm->underlaySpacing;
	}
	auto clipNegative = false;
	if (SelectedForm->fillSpacing < 0) {
		clipNegative = true;
	}
	if (clipWidth < CLPMINAUT) {
		clipWidth = CLPMINAUT;
	}
	if (StateMap.test(StateFlag::TXFIL)) {
		if ((TextureIndex != 0u)
		    && SelectedForm->fillInfo.texture.index + SelectedForm->fillInfo.texture.count
		           <= gsl::narrow<unsigned short>(TextureIndex)) {
			clipWidth = SelectedForm->fillSpacing;
		}
		else {
			return;
		}
	}
	auto lengths = std::vector<double>{};
	lengths.resize(currentVertexCount);
	auto clipSideLengths = std::vector<double>{}; // lengths of form sides for clipboard fill
	clipSideLengths.resize(currentVertexCount);
	auto clipIntersectData = std::vector<CLIPSORT>{}; // intersect points for clipboard fill
	clipIntersectData.resize(currentVertexCount);
	auto arrayOfClipIntersectData = std::vector<CLIPSORT*>{};
	arrayOfClipIntersectData.reserve(currentVertexCount);
	auto vertex      = leftsid(currentFormVertices);
	auto totalLength = 0.0;
	vertex           = form::nxt(vertex);
	auto vertexIt    = currentFormVertices->begin();
	for (auto iVertex = 0u; iVertex <= currentVertexCount; iVertex++) {
		const auto nextVertex   = form::nxt(vertex);
		lengths[vertex]         = totalLength;
		clipSideLengths[vertex] = hypot(vertexIt[nextVertex].x - vertexIt[vertex].x, vertexIt[nextVertex].y - vertexIt[vertex].y);
		totalLength += clipSideLengths[vertex];
		vertex = nextVertex;
	}
	auto boundingRect = fRECTANGLE{ vertexIt[0].y, vertexIt[0].x, vertexIt[0].x, vertexIt[0].y };
	for (auto iVertex = 1u; iVertex < currentVertexCount; iVertex++) {
		if (vertexIt[iVertex].x > boundingRect.right) {
			boundingRect.right = vertexIt[iVertex].x;
		}
		if (vertexIt[iVertex].x < boundingRect.left) {
			boundingRect.left = vertexIt[iVertex].x;
		}
		if (vertexIt[iVertex].y > boundingRect.top) {
			boundingRect.top = vertexIt[iVertex].y;
		}
		if (vertexIt[iVertex].y < boundingRect.bottom) {
			boundingRect.bottom = vertexIt[iVertex].y;
		}
	}
	auto clipGrid = RECT{ gsl::narrow<LONG>(std::floor(boundingRect.left / clipWidth)),
		                  gsl::narrow<LONG>(std::ceil(boundingRect.top / ClipRectSize.cy + 1) + 2),
		                  gsl::narrow<LONG>(std::ceil(boundingRect.right / clipWidth)),
		                  gsl::narrow<LONG>(std::floor(boundingRect.bottom / ClipRectSize.cy - 1)) };

	auto negativeOffset = 0l;
	auto clipGridOffset = 0u;
	if (SelectedForm->wordParam > 1) {
		clipGridOffset = SelectedForm->wordParam;
	}
	if (clipGridOffset != 0u) {
		clipGrid.top++;
		if (SelectedForm->fillSpacing < 0) {
			clipGrid.bottom--;
			clipGrid.left -= dToL(ClipRectSize.cx / clipWidth);
			clipGrid.right += dToL(ClipRectSize.cx / clipWidth);
		}
	}
	if (clipNegative && (clipGridOffset == 0u)) {
		clipGrid.left -= dToL(ClipRectSize.cx / clipWidth);
	}
	if (clipGrid.bottom < 0) {
		negativeOffset = 1 - clipGrid.bottom;
		clipGrid.bottom += negativeOffset;
		clipGrid.top += negativeOffset;
		const auto formNegativeOffset = ClipRectSize.cy * negativeOffset;
		for (auto iVertex = 0u; iVertex < currentVertexCount; iVertex++) {
			vertexIt[iVertex].y += formNegativeOffset;
		}
	}
	auto regionCrossingData = std::vector<VCLPX>{}; // region crossing data for vertical clipboard fills
	regionCrossingData.reserve(MAXFRMLINS);
	for (auto iVertex = 0u; iVertex < currentVertexCount; iVertex++) {
		auto start  = gsl::narrow<unsigned int>(std::floor(vertexIt[iVertex].x / clipWidth));
		auto finish = gsl::narrow<unsigned int>(std::floor((vertexIt[form::nxt(iVertex)].x) / clipWidth));
		if (start > finish) {
			std::swap(start, finish);
		}
		if (SelectedForm->fillSpacing < 0) {
			finish += gsl::narrow<decltype(finish)>(std::round(ClipRectSize.cx / clipWidth));
		}
		if (finish > gsl::narrow<unsigned int>(clipGrid.right)) {
			finish = gsl::narrow<unsigned int>(clipGrid.right);
		}
		if (clipNegative) {
			start -= dToUI(ClipRectSize.cx / clipWidth);
		}
		for (auto iSegment = start; iSegment <= finish; iSegment++) {
			regionCrossingData.push_back({ iSegment, iVertex });
		}
	}
	std::sort(regionCrossingData.begin(), regionCrossingData.end(), clpcmp);
	auto iclpx = std::vector<unsigned>{};
	iclpx.reserve(regionCrossingData.size());
	auto regionSegment = regionCrossingData[0].segment;
	iclpx.push_back(0);
	for (auto iSegment = 1u; iSegment < regionCrossingData.size(); iSegment++) {
		if (regionCrossingData[iSegment].segment != regionSegment) {
			iclpx.push_back(iSegment);
			regionSegment = regionCrossingData[iSegment].segment;
		}
	}
	iclpx.push_back(gsl::narrow<unsigned int>(regionCrossingData.size()));
	ActivePointIndex      = 0;
	auto clipStitchPoints = std::vector<CLIPNT>{};
	// Reserve some memory, but probably not enough
	clipStitchPoints.reserve(1000);
	auto pasteLocation = fPOINT{};
	auto texture       = static_cast<TXPNT*>(nullptr);
	auto iclpxSize     = iclpx.size();
	if (iclpxSize != 0u) {
		iclpxSize--;
		bool breakFlag = false;
		for (auto iRegion = 0u; iRegion < iclpxSize; iRegion++) {
			auto regionCrossingStart = iclpx[iRegion];
			auto regionCrossingEnd   = iclpx[static_cast<size_t>(iRegion) + 1];
			pasteLocation.x          = clipWidth * (iRegion + clipGrid.left);
			auto  clipVerticalOffset = 0.0f;
			auto  lineSegmentStart   = fPOINT{}; // vertical clipboard line segment start
			auto  lineSegmentEnd     = fPOINT{}; // vertical clipboard line segment end
			auto& clipBuffer         = *ClipBuffer;
			auto  clipStitchCount    = ClipBuffer->size();
			if (StateMap.test(StateFlag::TXFIL)) {
				const auto textureLine = (iRegion + clipGrid.left) % SelectedForm->fillInfo.texture.lines;
				clipStitchCount        = textureSegments[textureLine].stitchCount;
				texture                = &TexturePointsBuffer->at(gsl::narrow_cast<size_t>(SelectedForm->fillInfo.texture.index)
                                                   + textureSegments[textureLine].line);
				lineSegmentStart.x     = pasteLocation.x;
				if (SelectedForm->txof != 0.0f) {
					const auto lineOffset = (iRegion + clipGrid.left) / SelectedForm->fillInfo.texture.lines;
					clipVerticalOffset    = fmod(SelectedForm->txof * lineOffset, SelectedForm->fillInfo.texture.height);
				}
			}
			else {
				if (clipGridOffset != 0u) {
					clipVerticalOffset = static_cast<float>(iRegion % clipGridOffset) / (clipGridOffset * ClipRectSize.cy);
				}
				lineSegmentStart.x = pasteLocation.x + clipBuffer[0].x;
			}
			lineSegmentStart.y = clipGrid.bottom * ClipRectSize.cy;
			if (clipGridOffset != 0u) {
				clipVerticalOffset = static_cast<float>(iRegion % clipGridOffset) / (clipGridOffset * ClipRectSize.cy);
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
				for (auto iStitch = 0u; iStitch < clipStitchCount; iStitch++) {
					if (StateMap.test(StateFlag::TXFIL)) {
						if (texture != nullptr) {
							lineSegmentEnd = fPOINT{ pasteLocation.x, pasteLocation.y + texture[iStitch].y };
						}
					}
					else {
						lineSegmentEnd
						    = fPOINT{ pasteLocation.x + clipBuffer[iStitch].x, pasteLocation.y + clipBuffer[iStitch].y };
					}

					clipStitchPoints.push_back({ lineSegmentStart.x, lineSegmentStart.y, 0, 0 });
					if (isin(regionCrossingData,
					         lineSegmentStart.x,
					         lineSegmentStart.y,
					         regionCrossingStart,
					         regionCrossingEnd,
					         boundingRect,
					         currentFormVertices)) {
						if ((clipStitchPoints.size() > 1) && clipStitchPoints[clipStitchPoints.size() - 1].flag == 2) {
							inspnt(clipStitchPoints);
						}
						clipStitchPoints.back().flag = 0;
					}
					else {
						if ((clipStitchPoints.size() > 1) && (clipStitchPoints[clipStitchPoints.size() - 1].flag == 0u)) {
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
					if (count != 0u) {
						for (auto index = 0u; index < count; index++) {
							clipStitchPoints.push_back({ arrayOfClipIntersectData[index]->point.x,
							                             arrayOfClipIntersectData[index]->point.y,
							                             arrayOfClipIntersectData[index]->vertexIndex,
							                             1 });
							if (clipStitchPoints.size() > MAXITEMS << 2) {
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
					clipStitchPoints[clipStitchPoints.size() - 1].flag = 2;
				};
			}
			else {
				break;
			}
		}
	}
	if (!clipStitchPoints.empty()) {
		clipStitchPoints[clipStitchPoints.size() - 1].flag = 2;
	};
	if (negativeOffset != 0) {
		const auto formNegativeOffset = negativeOffset * ClipRectSize.cy;
		for (auto& clipStitchPoint : clipStitchPoints) {
			clipStitchPoint.y -= formNegativeOffset;
		}
		for (auto iVertex = 0u; iVertex < currentVertexCount; iVertex++) {
			vertexIt[iVertex].y -= formNegativeOffset;
		}
	}

#define CLPVU 0

#if CLPVU == 0
	StateMap.reset(StateFlag::FILDIR);
	auto previousPoint = 0u;
	auto clipSegments  = std::vector<CLPSEG>{};
	auto endPoint      = clipStitchPoints.size();
	if (endPoint != 0u) {
		endPoint--;
		// reserve a reasonable amount but not the full amount potentially required
		clipSegments.reserve(endPoint / 10);
		for (auto iPoint = 0u; iPoint < endPoint; iPoint++) {
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
		auto sortedLengths = std::vector<LENINFO>{};
		sortedLengths.reserve(clipSegments.size() * 2);
		for (auto iSegment = 0u; iSegment < clipSegments.size(); iSegment++) {
			sortedLengths.push_back({ iSegment, false, clipSegments[iSegment].beginLength });
			sortedLengths.push_back({ iSegment, true, clipSegments[iSegment].endLength });
		}
		std::sort(sortedLengths.begin(), sortedLengths.end(), lencmp);
		for (auto iSorted = 0u; iSorted < sortedLengths.size(); iSorted++) {
			if (sortedLengths[iSorted].isEnd) {
				clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
			}
			else {
				clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
			}
		}

#if CLPVU == 1

		for (unsigned iStitch = 0; iStitch < ActivePointIndex; iStitch++) {
			StitchBuffer[iStitch].x         = ClipStitchPoints[iStitch].x;
			StitchBuffer[iStitch].y         = ClipStitchPoints[iStitch].y;
			StitchBuffer[iStitch].attribute = 0;
		}
		PCSHeader.stitchCount = ActivePointIndex;
#endif

#if CLPVU == 2

		auto iStitch = 0;
		for (iSegment = 0; iSegment < ClipSegmentIndex; iSegment++) {
			for (iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish; iStitchPoint++) {
				StitchBuffer[iStitch].x           = ClipStitchPoints[iStitchPoint].x;
				StitchBuffer[iStitch].y           = ClipStitchPoints[iStitchPoint].y;
				StitchBuffer[iStitch++].attribute = iSegment & 0xf;
			}
		}
		PCSHeader.stitchCount = iStitch;

#endif

#if CLPVU == 0

		auto currentSegmentIndex = 0u;
		StateMap.set(StateFlag::FILDIR);
		SequenceIndex          = 0;
		auto clipIntersectSide = clipSegments[0].asid;
		ritseg(clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
		while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
			if (SequenceIndex > MAXITEMS - 3) {
				break;
			}
			ritseg(clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
		}
		chksid(0, clipIntersectSide, currentFormVertices);
		if (SequenceIndex > MAXITEMS - 100) {
			SequenceIndex = MAXITEMS - 100;
		}
		auto index = 0u;
		for (auto iSequence = 0u; iSequence < SequenceIndex; iSequence++) {
			if (vscmp(iSequence, index)) {
				index++;
				OSequence[index] = OSequence[iSequence];
			}
		}
		SequenceIndex = index;
		if (StateMap.test(StateFlag::WASNEG)) {
			for (auto iSequence = 0u; iSequence < SequenceIndex; iSequence++) {
				OSequence[iSequence].x -= formOffset;
			}
			for (auto iVertex = 0u; iVertex < currentVertexCount; iVertex++) {
				vertexIt[iVertex].x -= formOffset;
			}
			SelectedForm->rectangle.left -= formOffset;
			SelectedForm->rectangle.right -= formOffset;
		}
#endif
	}
}

void form::internal::angout(FRMHED& angledForm) {
	if (angledForm.vertexCount != 0u) {
		auto rectangle     = &angledForm.rectangle;
		CurrentVertexIndex = angledForm.vertexIndex;
		auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
		rectangle->left = rectangle->right = vertexIt[0].x;
		rectangle->bottom = rectangle->top = vertexIt[0].y;
		for (auto iVertex = 1u; iVertex < angledForm.vertexCount; iVertex++) {
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

void form::internal::horclpfn(const std::vector<RNGCNT>& textureSegments, FRMHED& angledForm) {
	angledForm                = (*FormList)[ClosestFormToCursor];
	const auto rotationCenter = dPOINT{
		((static_cast<double>(angledForm.rectangle.right) - angledForm.rectangle.left) / 2.0 + angledForm.rectangle.left),
		((static_cast<double>(angledForm.rectangle.top) - angledForm.rectangle.bottom) / 2.0 + angledForm.rectangle.bottom)
	};
	AngledFormVertices->clear();
	AngledFormVertices->reserve(angledForm.vertexCount);
	auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
	for (auto iVertex = 0u; iVertex < angledForm.vertexCount; iVertex++) {
		AngledFormVertices->push_back(vertexIt[iVertex]);
		thred::rotflt(AngledFormVertices->back(), (PI / 2), rotationCenter);
	}
	angledForm.vertexIndex = 0;
	angout(angledForm);
	SelectedForm       = &angledForm;
	CurrentVertexIndex = angledForm.vertexIndex;
	clpcon(textureSegments, AngledFormVertices);
	rotbak((-PI / 2), rotationCenter);
	form::fvars(ClosestFormToCursor);
}

void form::angclpfn(const std::vector<RNGCNT>& textureSegments) {
	auto rotationAngle = 0.0;

	auto       angledForm     = (*FormList)[ClosestFormToCursor];
	const auto rotationCenter = dPOINT{
		((static_cast<double>(angledForm.rectangle.right) - angledForm.rectangle.left) / 2.0 + angledForm.rectangle.left),
		((static_cast<double>(angledForm.rectangle.top) - angledForm.rectangle.bottom) / 2.0 + angledForm.rectangle.bottom)
	};
	AngledFormVertices->clear();
	AngledFormVertices->reserve(angledForm.vertexCount);
	if (StateMap.test(StateFlag::ISUND)) {
		rotationAngle = PI / 2 - SelectedForm->underlayStitchAngle;

		const auto& vertexList = xt::insid();
		for (auto iVertex = 0u; iVertex < angledForm.vertexCount; iVertex++) {
			AngledFormVertices->push_back(vertexList[iVertex]);
			thred::rotflt(AngledFormVertices->back(), rotationAngle, rotationCenter);
		}
	}
	else {
		if (StateMap.test(StateFlag::TXFIL)) {
			rotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
		}
		else {
			rotationAngle = PI / 2 - SelectedForm->satinOrAngle.angle;
		}
		auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
		for (auto iVertex = 0u; iVertex < angledForm.vertexCount; iVertex++) {
			AngledFormVertices->push_back(vertexIt[iVertex]);
			thred::rotflt(AngledFormVertices->back(), rotationAngle, rotationCenter);
		}
	}
	angledForm.vertexIndex = 0;
	fi::angout(angledForm);
	SelectedForm       = &angledForm;
	CurrentVertexIndex = 0;
	fi::clpcon(textureSegments, AngledFormVertices);
	fi::rotbak(-rotationAngle, rotationCenter);
	form::fvars(ClosestFormToCursor);
}

bool form::internal::sqcomp(const SMALPNTL* arg1, const SMALPNTL* arg2) noexcept {
	if ((arg1 != nullptr) && (arg2 != nullptr)) {
		const auto& lineEnd1 = *arg1;
		const auto& lineEnd2 = *arg2;

		if (lineEnd1.line == lineEnd2.line) {
			if (lineEnd1.group == lineEnd2.group) {
				if (lineEnd1.y == lineEnd2.y) {
					return false;
				}

				return (lineEnd1.y < lineEnd2.y);
			}

			return (lineEnd1.group < lineEnd2.group);
		}

		return (lineEnd1.line < lineEnd2.line);
	}
	return false;
}

bool form::internal::isclos(const SMALPNTL* const lineEndPoint0,
                            const SMALPNTL* const lineEndPoint1,
                            double                gapToClosestRegion) noexcept {
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

bool form::internal::lnclos(std::vector<unsigned>& groupIndexSequence,
                            std::vector<SMALPNTL>& lineEndpoints,
                            unsigned               group0,
                            unsigned               line0,
                            unsigned               group1,
                            unsigned               line1,
                            double                 gapToClosestRegion) {
	const auto lineEndPoint0 = &lineEndpoints[groupIndexSequence[group0]];

	if (group1 > groupIndexSequence.size() - 2) {
		return false;
	}
	if (group0 == 0) {
		return false;
	}
	if (lineEndPoint0 != nullptr) {
		auto count0 = (groupIndexSequence[static_cast<size_t>(group0) + 1] - groupIndexSequence[group0]) >> 1;
		auto index0 = 0u;
		while ((count0 != 0u) && lineEndPoint0[index0].line != line0) {
			count0--;
			index0 += 2;
		}
		if (count0 != 0u) {
			auto count1 = (groupIndexSequence[static_cast<size_t>(group1) + 1] - groupIndexSequence[group1]) >> 1;
			auto index1 = 0u;
			if (const auto lineEndPoint1 = &lineEndpoints[groupIndexSequence[group1]]) {
				while ((count1 != 0u) && lineEndPoint1[index1].line != line1) {
					count1--;
					index1 += 2;
				}
				if (count1 != 0u) {
					return isclos(&lineEndPoint0[index0], &lineEndPoint1[index1], gapToClosestRegion);
				}
			}
		}
	}
	return false;
}

bool form::internal::regclos(std::vector<unsigned>&        groupIndexSequence,
                             std::vector<SMALPNTL>&        lineEndpoints,
                             const std::vector<SMALPNTL*>& sortedLines,
                             unsigned                      iRegion0,
                             unsigned                      iRegion1,
                             const std::vector<REGION>&    regionsList,
                             double                        gapToClosestRegion,
                             unsigned&                     nextGroup) {
	const auto lineEndPoint0Start = sortedLines[regionsList[iRegion0].start];
	auto       lineEndPoint0End   = static_cast<SMALPNTL*>(nullptr);
	const auto lineEndPoint1Start = sortedLines[regionsList[iRegion1].start];
	auto       lineEndPoint1End   = static_cast<SMALPNTL*>(nullptr);
	const auto group0Start        = lineEndPoint0Start->group;
	auto       group0End          = 0u;
	const auto group1Start        = lineEndPoint1Start->group;
	auto       group1End          = 0u;
	auto       groupStart         = 0u;
	auto       lineStart          = 0u;
	auto       prevLine           = 0u;

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
	if ((groupStart != 0u)
	    && lnclos(groupIndexSequence, lineEndpoints, groupStart - 1, prevLine, groupStart, lineStart, gapToClosestRegion)) {
		nextGroup = groupStart;
		return true;
	}

	lineEndPoint0End = sortedLines[regionsList[iRegion0].end];
	lineEndPoint1End = sortedLines[regionsList[iRegion1].end];
	group1End        = lineEndPoint1End->group;
	group0End        = lineEndPoint0End->group;
	auto groupEnd    = 0u;
	auto lineEnd     = 0u;
	auto lastLine    = 0u;
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

bool form::internal::unvis(const boost::dynamic_bitset<>& visitedRegions, unsigned& visitedIndex) {
	const auto regionCount = visitedRegions.size();
	for (visitedIndex = 0; visitedIndex < regionCount; visitedIndex++) {
		if (!visitedRegions[visitedIndex]) {
			return true;
		}
	}
	return false;
}

bool form::internal::notdun(std::vector<RGSEQ>&            tempPath,
                            const std::vector<RCON>&       pathMap,
                            const std::vector<unsigned>&   mapIndexSequence,
                            const boost::dynamic_bitset<>& visitedRegions,
                            unsigned                       level,
                            unsigned                       doneRegion,
                            unsigned                       sequencePathIndex) {
	auto previousLevel = level;
	if (previousLevel != 0u) {
		previousLevel--;
	}

	const auto regionPath = &tempPath[sequencePathIndex];
	regionPath[0].pcon    = mapIndexSequence[doneRegion];
	regionPath[0].count   = mapIndexSequence[static_cast<size_t>(doneRegion) + 1] - regionPath[0].pcon;
	for (auto iPath = 1u; iPath < level; iPath++) {
		regionPath[iPath].pcon = mapIndexSequence[pathMap[regionPath[iPath - 1].pcon].node];
		regionPath[iPath].count
		    = mapIndexSequence[static_cast<size_t>(pathMap[regionPath[static_cast<size_t>(iPath) - 1].pcon].node) + 1]
		      - regionPath[iPath].pcon;
	}
	while (visitedRegions[pathMap[regionPath[previousLevel].pcon].node] && previousLevel >= 0) {
		if (--regionPath[previousLevel].count > 0) {
			regionPath[previousLevel].pcon++;
		}
		else {
			auto pivot = previousLevel;
			do {
				if (pivot != 0u) {
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
				if (pivot != 0u) {
					regionPath[pivot].pcon = mapIndexSequence[pathMap[regionPath[pivot - 1].pcon].node];
					regionPath[pivot].count
					    = mapIndexSequence[static_cast<size_t>(pathMap[regionPath[static_cast<size_t>(pivot) - 1].pcon].node) + 1]
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

double form::internal::reglen(const std::vector<SMALPNTL*>& sortedLines,
                              unsigned                      iRegion,
                              const std::vector<fPOINT>&    lastRegionCorners,
                              const std::vector<REGION>&    regionsList) {
	auto lineEndPoints = std::vector<SMALPNTL*>{};
	lineEndPoints.resize(4);

	lineEndPoints[0]   = sortedLines[regionsList[iRegion].start];
	lineEndPoints[1]   = &sortedLines[regionsList[iRegion].start][1];
	lineEndPoints[2]   = sortedLines[regionsList[iRegion].end];
	lineEndPoints[3]   = &sortedLines[regionsList[iRegion].end][1];
	auto minimumLength = 1e99;
	for (auto iCorner = 0u; iCorner < 4; iCorner++) {
		for (auto iPoint = 0u; iPoint < 4; iPoint++) {
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
                            const std::vector<unsigned>&  mapIndexSequence,
                            boost::dynamic_bitset<>&      visitedRegions,
                            const std::vector<SMALPNTL*>& sortedLines,
                            const std::vector<REGION>&    regionsList,
                            unsigned&                     doneRegion,
                            unsigned                      pathMapIndex,
                            unsigned&                     sequencePathIndex,
                            unsigned                      visitedIndex) {
	auto lastRegionCorners = std::vector<fPOINT>{};
	lastRegionCorners.resize(4); // corners of last region sequenced

	auto pathLength = 1u; // length of the path to the region
	while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength, doneRegion, sequencePathIndex)) {
		pathLength++;
		if (pathLength > 8) {
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
			auto       newRegion     = 0u;
			auto       minimumLength = 1e99;
			const auto regionCount   = visitedRegions.size();
			for (auto iRegion = 0u; iRegion < regionCount; iRegion++) {
				if (!visitedRegions[iRegion]) {
					const auto length = reglen(sortedLines, iRegion, lastRegionCorners, regionsList);
					if (length < minimumLength) {
						minimumLength = length;
						newRegion     = iRegion;
					}
				}
			}
			tempPath[sequencePathIndex].skp = true;
			for (auto iPath = 0u; iPath < pathMapIndex; iPath++) {
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
	const auto regionPath = &tempPath[sequencePathIndex];
	for (auto iPath = 0u; iPath < pathLength; iPath++) {
		tempPath[sequencePathIndex].skp    = false;
		tempPath[sequencePathIndex++].pcon = regionPath[iPath].pcon;
		visitedRegions.set(pathMap[regionPath[iPath].pcon].node);
	}
	doneRegion = pathMap[regionPath[pathLength - 1].pcon].node;
}

void form::internal::nxtseq(std::vector<FSEQ>&           sequencePath,
                            const std::vector<RCON>&     pathMap,
                            const std::vector<unsigned>& mapIndexSequence,
                            unsigned                     pathIndex,
                            unsigned&                    pathCount) {
	auto iPath = mapIndexSequence[sequencePath[pathIndex].node];

	if ((gsl::narrow_cast<size_t>(pathIndex) + 1) < sequencePath.size()) {
		const auto nextNode = sequencePath[static_cast<size_t>(pathIndex) + 1].node;
		while (iPath < mapIndexSequence[static_cast<size_t>(sequencePath[pathIndex].node) + 1]
		       && pathMap[iPath].node != nextNode) {
			iPath++;
		}
		sequencePath[pathCount++].nextGroup = gsl::narrow<unsigned short>(pathMap[iPath].nextGroup);
	}
	else {
		sequencePath[pathCount++].nextGroup = 0;
	}
}

void form::internal::rspnt(float xCoordinate, float yCoordinate) noexcept {
	BSequence[OutputIndex].x           = xCoordinate;
	BSequence[OutputIndex].y           = yCoordinate;
	BSequence[OutputIndex++].attribute = 0;
}

void form::internal::brkdun(const std::vector<SMALPNTL*>& sortedLines, unsigned int start, unsigned int finish) {
	rspnt(sortedLines[start]->x, sortedLines[start]->y);
	rspnt(sortedLines[finish]->x, sortedLines[finish]->y);
	rspnt((*WorkingFormVertices)[sortedLines[start]->line].x, (*WorkingFormVertices)[sortedLines[start]->line].y);
	StateMap.set(StateFlag::BRKFIX);
}

void form::internal::duseq1(const SMALPNTL* sequenceLines) noexcept {
	if (sequenceLines != nullptr) {
		rspnt((sequenceLines[1].x - sequenceLines[0].x) / 2 + sequenceLines[0].x,
		      (sequenceLines[1].y - sequenceLines[0].y) / 2 + sequenceLines[0].y);
	}
}

void form::internal::movseq(const std::vector<SMALPNTL*>& sortedLines, unsigned int ind) {
	auto lineEndPoint = sortedLines[ind];

	BSequence[OutputIndex].attribute = SEQBOT;
	BSequence[OutputIndex].x         = lineEndPoint->x;
	BSequence[OutputIndex].y         = lineEndPoint->y;
	OutputIndex++;
	// Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
	//             and not the next entry in sortedLines
	lineEndPoint++;
	BSequence[OutputIndex].attribute = SEQTOP;
	BSequence[OutputIndex].x         = lineEndPoint->x;
	BSequence[OutputIndex].y         = lineEndPoint->y;
	OutputIndex++;
}

void form::internal::brkseq(const std::vector<SMALPNTL*>& sortedLines,
                            unsigned int                  start,
                            unsigned int                  finish,
                            boost::dynamic_bitset<>&      sequenceMap,
                            unsigned&                     lastGroup,
                            SMALPNTL*                     sequenceLines) {
	StateMap.reset(StateFlag::SEQDUN);
	if (sequenceLines == nullptr) {
		sequenceLines = sortedLines[start];
	}
	if (start > finish) {
		auto savedGroup = sortedLines[start]->group + 1;
		// This odd construction for iLine is used to ensure
		// loop terminates when finish = 0
		for (auto iLine = start + 1; iLine != finish; iLine--) {
			const auto iLineDec = iLine - 1;
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
		auto savedGroup = sortedLines[start]->group - 1;
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

void form::internal::dunseq(const std::vector<SMALPNTL*>& sortedLines,
                            unsigned int                  start,
                            unsigned int                  finish,
                            unsigned&                     lastGroup) {
	auto minimumY = 1e30;

	for (auto iLine = start; iLine <= finish; iLine++) {
		const double deltaY = sortedLines[start][1].y - sortedLines[start][0].y;
		if (deltaY < minimumY) {
			minimumY = deltaY;
		}
	}
	if (minimumY == 1e30) {
		minimumY = 0;
	}
	else {
		minimumY /= 2;
	}
	rspnt(sortedLines[start][0].x, sortedLines[start][0].y + minimumY);
	rspnt(sortedLines[finish][0].x, sortedLines[finish][0].y + minimumY);
	lastGroup = sortedLines[finish][0].group;
}

SMALPNTL* form::internal::duseq2(SMALPNTL* sequenceLines) noexcept {
	if (sequenceLines != nullptr) {
		rspnt((sequenceLines[1].x - sequenceLines[0].x) / 2 + sequenceLines[0].x,
		      (sequenceLines[1].y - sequenceLines[0].y) / 2 + sequenceLines[0].y);
	}
	return sequenceLines;
}

void form::internal::duseq(const std::vector<SMALPNTL*>& sortedLines,
                           unsigned int                  start,
                           unsigned int                  finish,
                           boost::dynamic_bitset<>&      sequenceMap,
                           unsigned&                     lastGroup,
                           SMALPNTL*                     sequenceLines) {
	auto savedTopLine = sortedLines[start][1].line;
	bool flag         = false;

	sequenceLines = sortedLines[start];
	if (sequenceLines != nullptr) {
		StateMap.reset(StateFlag::SEQDUN);
		if (start > finish) {
			auto iLine = start + 1;
			// This odd construction for iLine is used to ensure loop terminates when finish = 0
			for (; iLine != finish; iLine--) {
				const auto iLineDec = iLine - 1;
				if (sequenceMap.test_set(iLineDec)) {
					if (!StateMap.testAndSet(StateFlag::SEQDUN)) {
						flag          = true;
						sequenceLines = duseq2(sortedLines[iLineDec]);
					}
					else {
						if (savedTopLine != sortedLines[iLineDec][1].line) {
							if (iLineDec != 0u) {
								sequenceLines = duseq2(sortedLines[static_cast<size_t>(iLineDec) + 1]);
							}
							flag          = true;
							sequenceLines = duseq2(sortedLines[iLineDec]);
							savedTopLine  = sequenceLines[1].line;
						}
					}
				}
				else {
					if (StateMap.testAndReset(StateFlag::SEQDUN)) {
						sequenceLines = duseq2(sortedLines[static_cast<size_t>(iLineDec) + 1]);
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
							if (iLine != 0u) {
								sequenceLines = duseq2(sortedLines[iLine - 1]);
							}
							flag          = true;
							sequenceLines = duseq2(sortedLines[iLine]);
							savedTopLine  = sequenceLines[1].line;
						}
					}
				}
				else {
					if (StateMap.testAndReset(StateFlag::SEQDUN)) {
						if (iLine != 0u) {
							sequenceLines = duseq2(sortedLines[iLine - 1]);
						}
					}
					flag          = true;
					sequenceLines = sortedLines[iLine];
					movseq(sortedLines, iLine);
				}
			}
			if (StateMap.testAndReset(StateFlag::SEQDUN)) {
				if (iLine != 0u) {
					flag          = true;
					sequenceLines = duseq2(sortedLines[iLine - 1]);
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
                           unsigned                      pthi,
                           unsigned int                  lineCount,
                           std::vector<REGION>&          regionsList,
                           unsigned&                     lastGroup,
                           unsigned                      sequencePathIndex) {
	auto        sequenceMap   = boost::dynamic_bitset<>(lineCount);
	auto        nextGroup     = sequencePath[pthi].nextGroup;
	const auto  iRegion       = sequencePath[pthi].node;
	const auto* currentRegion = &regionsList[iRegion];
	const auto  sequenceStart = currentRegion->start;
	const auto  sequenceEnd   = currentRegion->end;
	if (sequencePath[pthi].skp || StateMap.testAndReset(StateFlag::BRKFIX)) {
		if (BSequence[OutputIndex - 1].attribute != SEQBOT) {
			rspnt(BSequence[OutputIndex - 2].x, BSequence[OutputIndex - 2].y);
		}
		const auto  firstLine     = sortedLines[sequenceStart]->line;
		const auto* bpnt          = &BSequence[OutputIndex - 1];
		auto        minimumLength = 1e99;
		auto        mindif        = 0u;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			const auto length = hypot(bpnt->x - (*WorkingFormVertices)[iVertex].x, bpnt->y - (*WorkingFormVertices)[iVertex].y);
			if (length < minimumLength) {
				minimumLength = length;
				mindif        = iVertex;
			}
		}
		if (minimumLength != 0.0) {
			rspnt((*WorkingFormVertices)[mindif].x, (*WorkingFormVertices)[mindif].y);
		}
		if (VertexCount != 0u) {
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
	const auto groupStart = sortedLines[currentRegion->start]->group;
	const auto groupEnd   = sortedLines[currentRegion->end]->group;
	auto       seql       = 0u;
	if (groupEnd != groupStart) {
		const auto intermediate
		    = std::round((static_cast<double>(lastGroup) - groupStart) / (static_cast<double>(groupEnd) - groupStart)
		                     * (static_cast<double>(sequenceEnd) - sequenceStart)
		                 + sequenceStart);
		// ToDo - is this guard still needed now that intermediate is calculated correctly?
		if (intermediate > lineCount) {
			seql = 0;
		}
		else {
			// ToDo - is abs the correct choice when intermediate < 0 or should seql be set to 0?
			seql = gsl::narrow<decltype(seql)>(std::abs(intermediate));
		}
	}
	const auto length = (static_cast<double>(groupEnd) - groupStart) * (static_cast<double>(sequenceEnd) - sequenceStart);
	auto       seqn   = sequenceEnd;
	if (length != 0.0) {
		// ToDo - Is this the best fix for this issue?
		if (nextGroup == 0 || nextGroup < groupStart) {
			nextGroup = groupStart;
		}
		seqn = gsl::narrow<decltype(seqn)>(std::round((static_cast<double>(nextGroup) - groupStart) / length + sequenceStart));
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
		if (seql < sequenceEnd && sortedLines[static_cast<size_t>(seql) + 1]->group == lastGroup) {
			seql++;
		}
		else {
			if (seql > sequenceStart && sortedLines[seql - 1]->group == lastGroup) {
				seql--;
			}
			else {
				auto mindif = 0xffffffffu;
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
		if (seqn < sequenceEnd && sortedLines[static_cast<size_t>(seqn) + 1]->group == nextGroup) {
			seqn++;
		}
		else {
			if (seqn > sequenceStart && sortedLines[seqn - 1]->group == nextGroup) {
				seqn--;
			}
			else {
				auto mindif = 0xffffffffu;
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
	auto sequenceLines = static_cast<SMALPNTL*>(nullptr);
	if (currentRegion->breakCount != 0u) {
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

#define BUGSEQ 0

void form::internal::lcon(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints) {
#if BUGSEQ
	UNREFERENCED_PARAMETER(groupIndexSequence);
#endif

	if (!lineEndpoints.empty()) {
		auto       sortedLines     = std::vector<SMALPNTL*>{};
		const auto stitchLineCount = lineEndpoints.size();
		sortedLines.reserve(stitchLineCount >> 1);
		for (auto iLine = 0u; iLine < stitchLineCount; iLine += 2) {
			sortedLines.push_back(&lineEndpoints[iLine]);
		}
		std::sort(sortedLines.begin(), sortedLines.end(), sqcomp);
		const auto lineCount = gsl::narrow<unsigned int>(sortedLines.size());
		auto       regions   = std::vector<REGION>{};
		regions.emplace_back( 0u, 0u, 0u, 0u );
		auto breakLine = sortedLines[0]->line;
		for (auto iLine = 0u; iLine < lineCount; iLine++) {
			if (breakLine != sortedLines[iLine]->line) {
				regions.back().end = iLine - 1;
				regions.emplace_back( iLine, 0u, 0u, 0u );
				breakLine = sortedLines[iLine]->line;
			}
		}
		regions.back().end = lineCount - 1;
		auto regionCount   = regions.size();
		auto regionsList   = std::vector<REGION>{};
		regionsList.resize(regionCount);
		auto visitedRegions = boost::dynamic_bitset<>(regionCount);
		for (auto iRegion = 0u; iRegion < regionCount; iRegion++) {
			regionsList[iRegion].start      = regions[iRegion].start;
			regionsList[iRegion].end        = regions[iRegion].end;
			regionsList[iRegion].breakCount = 0;
		}
		const auto iStartLine = 0u;
		for (auto iRegion = 0u; iRegion < regionCount; iRegion++) {
			auto count = 0u;
			if ((regionsList[iRegion].end - regionsList[iRegion].start) > 1) {
				auto startGroup = sortedLines[regionsList[iRegion].start]->group;
				for (auto iLine = regionsList[iRegion].start + 1; iLine <= regionsList[iRegion].end; iLine++) {
					startGroup++;
					if (sortedLines[iLine]->group != startGroup) {
						if (count == 0u) {
							regionsList[iRegion].regionBreak = iStartLine;
						}
						count++;
						startGroup = sortedLines[iLine]->group;
					}
				}
			}
			regionsList[iRegion].breakCount = count;
		}

#if BUGSEQ
		// Note - this debug code only works for vertical fill on a single form
		auto bugColor = 0u;
		auto index    = 0u;
		for (auto iRegion = 0u; iRegion < RegionCount; iRegion++) {
			for (auto iLine = regionsList[iRegion].start; iLine <= regionsList[iRegion].end; iLine++) {
				auto lineGroupPoint           = &*sortedLines[iLine];
				StitchBuffer[index].attribute = bugColor;
				StitchBuffer[index].x         = lineGroupPoint[0].x;
				StitchBuffer[index++].y       = lineGroupPoint[0].y;
				StitchBuffer[index].attribute = bugColor;
				StitchBuffer[index].x         = lineGroupPoint[1].x;
				StitchBuffer[index++].y       = lineGroupPoint[1].y;
			}
			bugColor++;
			bugColor &= 0xf;
		}
		PCSHeader.stitchCount = gsl::narrow<decltype(PCSHeader.stitchCount)>(index);
#else
		OutputIndex = 0;

		auto mapIndexSequence = std::vector<unsigned>{};
		mapIndexSequence.reserve(gsl::narrow_cast<size_t>(regionCount) + 1);
		auto pathMap      = std::vector<RCON>{};
		auto sequencePath = std::vector<FSEQ>{};

		if (regionCount > 1) {
			auto pathMapIndex = 0u;
			// use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
			pathMap.reserve(gsl::narrow_cast<size_t>((regionCount * (regionCount - 1)) / 2) + 2);
			for (auto iSequence = 0u; iSequence < regionCount; iSequence++) {
				mapIndexSequence.push_back(pathMapIndex);
				auto count              = 0;
				auto gapToClosestRegion = 0.0;
				for (auto iNode = 0u; iNode < regionCount; iNode++) {
					if (iSequence != iNode) {
						auto       nextGroup   = 0u;
						const auto isConnected = regclos(groupIndexSequence,
						                                 lineEndpoints,
						                                 sortedLines,
						                                 iSequence,
						                                 iNode,
						                                 regionsList,
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
					for (auto iNode = 0u; iNode < regionCount; iNode++) {
						if (iSequence != iNode) {
							auto       nextGroup   = 0u;
							const auto isConnected = regclos(groupIndexSequence,
							                                 lineEndpoints,
							                                 sortedLines,
							                                 iSequence,
							                                 iNode,
							                                 regionsList,
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
			auto startGroup = 0xffffffffu;
			auto leftRegion = 0u;
			for (auto iRegion = 0u; iRegion < regionCount; iRegion++) {
				auto lineGroupPoint = sortedLines[regionsList[iRegion].start];
				if (lineGroupPoint->group < startGroup) {
					startGroup = lineGroupPoint->group;
					leftRegion = iRegion;
				}
			}
			OutputIndex   = 0;
			auto tempPath = std::vector<RGSEQ>{};
			tempPath.resize(gsl::narrow_cast<size_t>((regionCount * (regionCount - 1)) / 2) + 1);

			// find the leftmost region in pathMap
			auto sequencePathIndex = 1u;
			auto dontSkip          = true;
			auto inPath            = 0u;
			for (inPath = 0u; inPath < pathMapIndex; inPath++) {
				if (pathMap[inPath].node == leftRegion) {
					dontSkip = false;
					break;
				}
			}
			if (dontSkip) {
				pathMap[pathMapIndex].node      = leftRegion;
				pathMap[pathMapIndex].nextGroup = 0;
				inPath                          = pathMapIndex;
			}
			// set the first entry in the temporary path to the leftmost region
			tempPath[0].pcon  = inPath;
			tempPath[0].count = 1;
			tempPath[0].skp   = false;
			visitedRegions.set(leftRegion);
			auto doneRegion   = leftRegion; // last region sequenced
			auto visitedIndex = 0u;
			while (unvis(visitedRegions, visitedIndex)) {
				nxtrgn(tempPath,
				       pathMap,
				       mapIndexSequence,
				       visitedRegions,
				       sortedLines,
				       regionsList,
				       doneRegion,
				       pathMapIndex,
				       sequencePathIndex,
				       visitedIndex);
			}
			auto count = 0xffffffffu;
			sequencePath.reserve(sequencePathIndex);
			for (auto iPath = 0u; iPath < sequencePathIndex; iPath++) {
				const bool     tmpSkip = tempPath[iPath].skp;
				unsigned short tmpNode = 0u;
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
			auto pathCount = 0u;
			for (auto iPath = 0u; iPath < sequencePathIndex; iPath++) {
				nxtseq(sequencePath, pathMap, mapIndexSequence, iPath, pathCount);
			}
			visitedRegions.reset();
			auto lastGroup = 0u;
			for (auto iPath = 0u; iPath < pathCount; iPath++) {
				OutputDebugString(
				    fmt::format(L"iterator {},vrt {},grpn {}\n", iPath, pathMap[iPath].node, pathMap[iPath].nextGroup).c_str());
				if (!unvis(visitedRegions, visitedIndex)) {
					break;
				}
				durgn(sequencePath, visitedRegions, sortedLines, iPath, lineCount, regionsList, lastGroup, sequencePathIndex);
			}
		}
		else {
			const auto sequencePathIndex = 1u;
			sequencePath.resize(sequencePathIndex);
			auto lastGroup            = 0u;
			sequencePath[0].node      = 0;
			sequencePath[0].nextGroup = gsl::narrow<unsigned short>(sortedLines[regionsList[0].end]->group);
			sequencePath[0].skp       = false;
			durgn(sequencePath, visitedRegions, sortedLines, 0, lineCount, regionsList, lastGroup, sequencePathIndex);
		}

#endif
	}
}

void form::internal::bakseq() {
	constexpr auto RITSIZ = 6;

	const unsigned seqtab[] = {
		12, 7, 15, 11, 13, 17,
	};

#if BUGBAK

	for (SequenceIndex = 0; SequenceIndex < OutputIndex; SequenceIndex++) {
		OSequence[SequenceIndex].x = BSequence[SequenceIndex].x;
		OSequence[SequenceIndex].y = BSequence[SequenceIndex].y;
	}
	SelectedForm->maxFillStitchLen = 6000;
#else

	auto iSequence = OutputIndex;
	if (iSequence != 0u) {
		iSequence--;
	}

	SequenceIndex = 0;
	StateMap.reset(StateFlag::FILDIR);
	OSequence[SequenceIndex] = BSequence[iSequence];
	SequenceIndex++;
	SelectedPoint = BSequence[iSequence];
	if (iSequence != 0u) {
		iSequence--;
	}
	while (iSequence > 0) {
		const auto rcnt = iSequence % RITSIZ;
		if (SequenceIndex > MAXITEMS) {
			SequenceIndex = MAXITEMS - 1;
			return;
		}
		const auto StitchSpacing2 = LineSpacing * 2;
		const auto rit            = gsl::narrow<unsigned int>(std::round(BSequence[iSequence].x / StitchSpacing2));
		auto       delta
		    = dPOINT{ BSequence[iSequence].x - BSequence[iSequence + 1].x, BSequence[iSequence].y - BSequence[iSequence + 1].y };
		auto slope = 1e99;
		if (delta.y != 0.0) {
			slope = delta.x / delta.y;
		}

		const auto UserStitchLength9 = UserStitchLength / 9;

		switch (BSequence[iSequence].attribute) {
		case SEQTOP: {
			if ((SelectedForm->extendedAttribute & AT_SQR) != 0u) {
				if (StateMap.testAndFlip(StateFlag::FILDIR)) {
					OSequence[SequenceIndex++] = BSequence[iSequence - 1];
					auto count                 = ceil(BSequence[iSequence].y / UserStitchLength);
					do {
						OSequence[SequenceIndex].y = count * UserStitchLength + (rit % seqtab[rcnt]) * UserStitchLength9;
						if (OSequence[SequenceIndex].y > BSequence[iSequence].y) {
							break;
						}
						delta.y                      = static_cast<double>(OSequence[SequenceIndex].y) - BSequence[iSequence].y;
						OSequence[SequenceIndex++].x = BSequence[iSequence].x;
						count++;
					} while (true);
					OSequence[SequenceIndex++] = BSequence[iSequence];
				}
				else {
					OSequence[SequenceIndex++] = BSequence[iSequence];
					auto count                 = gsl::narrow<unsigned int>(std::floor(BSequence[iSequence].y / UserStitchLength));
					do {
						OSequence[SequenceIndex].y = count * UserStitchLength - ((rit + 2) % seqtab[rcnt]) * UserStitchLength9;
						if (OSequence[SequenceIndex].y < BSequence[iSequence - 1].y) {
							break;
						}
						delta.y = static_cast<double>(OSequence[SequenceIndex].y) - BSequence[iSequence - 1].y;
						OSequence[SequenceIndex++].x = BSequence[iSequence].x;
						count--;
					} while (true);
					OSequence[SequenceIndex++] = BSequence[iSequence - 1];
				}
			}
			else {
				auto count = gsl::narrow<unsigned int>(std::ceil(BSequence[iSequence + 1].y / UserStitchLength));
				do {
					OSequence[SequenceIndex].y = count * UserStitchLength + (rit % seqtab[rcnt]) * UserStitchLength9;
					if (OSequence[SequenceIndex].y > BSequence[iSequence].y) {
						break;
					}
					delta.y                      = static_cast<double>(OSequence[SequenceIndex].y) - BSequence[iSequence + 1].y;
					delta.x                      = slope * delta.y;
					OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
					count++;
				} while (true);
				OSequence[SequenceIndex++] = BSequence[iSequence];
			}
			break;
		}
		case SEQBOT: {
			if ((SelectedForm->extendedAttribute & AT_SQR) == 0u) {
				auto count = gsl::narrow<unsigned int>(std::floor(BSequence[iSequence + 1].y / UserStitchLength));
				do {
					OSequence[SequenceIndex].y = count * UserStitchLength - ((rit + 2) % seqtab[rcnt]) * UserStitchLength9;
					if (OSequence[SequenceIndex].y < BSequence[iSequence].y) {
						break;
					}
					delta.y                      = static_cast<double>(OSequence[SequenceIndex].y) - BSequence[iSequence + 1].y;
					delta.x                      = slope * delta.y;
					OSequence[SequenceIndex++].x = BSequence[iSequence + 1].x + delta.x;
					count--;
				} while (true);
				OSequence[SequenceIndex++] = BSequence[iSequence];
			}
			break;
		}
		case 0: {
			delta = dPOINT{ static_cast<double>(BSequence[iSequence].x) - BSequence[iSequence + 1].x,
				            static_cast<double>(BSequence[iSequence].y) - BSequence[iSequence + 1].y };
			StateMap.reset(StateFlag::FILDIR);
			const auto length = hypot(delta.x, delta.y);
			if (length != 0.0) {
				const auto UserStitchLength2 = UserStitchLength * 2;
				if (length > UserStitchLength2) {
					auto point = BSequence[iSequence + 1];
					auto count = gsl::narrow<unsigned int>(std::round(length / UserStitchLength - 1));
					if (form::chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXITEMS - 3) {
						return;
					}
					const auto step = dPOINT{ delta.x / count, delta.y / count };
					while (count != 0u) {
						point.x += step.x;
						point.y += step.y;
						OSequence[SequenceIndex++] = point;
						count--;
					}
				}
			}
			OSequence[SequenceIndex++] = BSequence[iSequence];
		}
		}
		iSequence--;
	}
#endif
}

void form::ritseq1(unsigned int ind) {
	auto vertexIt              = FormVertices->begin() + CurrentVertexIndex;
	BSequence[SequenceIndex].x = vertexIt[ind].x;
	BSequence[SequenceIndex].y = vertexIt[ind].y;
	SequenceIndex++;
}

void form::filinu(const dPOINT& inPoint) {
	auto       point  = dPOINT{ SelectedPoint.x, SelectedPoint.y };
	const auto delta  = dPOINT{ (inPoint.x - SelectedPoint.x), (inPoint.y - SelectedPoint.y) };
	const auto length = hypot(delta.x, delta.y);
	auto       count  = gsl::narrow<unsigned int>(std::round(length / UserStitchLength));

	if (form::chkmax(count, SequenceIndex) || count + SequenceIndex > MAXITEMS - 3) {
		return;
	}
	if (count != 0u) {
		if (StateMap.test(StateFlag::FILDIR)) {
			count++;
		}
		const auto step = dPOINT{ delta.x / count, delta.y / count };
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence[SequenceIndex].x   = point.x;
			OSequence[SequenceIndex++].y = point.y;
			count--;
		}
	}
	else {
		OSequence[SequenceIndex].x   = inPoint.x;
		OSequence[SequenceIndex++].y = inPoint.y;
	}
	SelectedPoint = inPoint;
}

void form::filin(dPOINT currentPoint) {
	const auto delta  = dPOINT{ (currentPoint.x - SelectedPoint.x), (currentPoint.y - SelectedPoint.y) };
	auto       point  = dPOINT{ SelectedPoint.x, SelectedPoint.y };
	const auto length = hypot(delta.x, delta.y);
	auto       count  = gsl::narrow<unsigned int>(std::round(length / UserStitchLength));

	if (form::chkmax(count, SequenceIndex) || (count + SequenceIndex) > MAXITEMS - 3) {
		return;
	}
	if (count != 0u) {
		if (StateMap.test(StateFlag::FILDIR)) {
			count++;
		}
		const auto step = dPOINT{ delta.x / count, delta.y / count };
		while (count > 0) {
			point.x += step.x;
			point.y += step.y;
			OSequence[SequenceIndex].x   = point.x;
			OSequence[SequenceIndex++].y = point.y;
			count--;
		}
	}
	else {
		OSequence[SequenceIndex].x   = currentPoint.x;
		OSequence[SequenceIndex++].y = currentPoint.y;
	}
	SelectedPoint = currentPoint;
}

void form::internal::dudif(const dPOINT& start, const dPOINT& finish, dPOINT& delta) noexcept {
	delta.x = finish.x - start.x;
	delta.y = finish.y - start.y;
}

void form::internal::trfrm(const dPOINT& bottomLeftPoint,
                           const dPOINT& topLeftPoint,
                           const dPOINT& bottomRightPoint,
                           const dPOINT& topRightPoint) noexcept {
	auto topDelta    = dPOINT{};
	auto bottomDelta = dPOINT{};
	auto leftDelta   = dPOINT{};
	auto rightDelta  = dPOINT{};

	dudif(topLeftPoint, topRightPoint, topDelta);
	dudif(bottomLeftPoint, bottomRightPoint, bottomDelta);
	dudif(bottomLeftPoint, topLeftPoint, leftDelta);
	dudif(bottomRightPoint, topRightPoint, rightDelta);
	for (auto& clip : *ClipBuffer) {
		const auto clipRatio = dPOINT{ clip.x / ClipRectSize.cx, clip.y / ClipRectSize.cy };
		const auto topMidpoint
		    = dPOINT{ clipRatio.x * (topDelta.x) + topLeftPoint.x, clipRatio.x * (topDelta.y) + topLeftPoint.y };
		const auto bottomMidpoint
		    = dPOINT{ clipRatio.x * (bottomDelta.x) + bottomLeftPoint.x, clipRatio.x * (bottomDelta.y) + bottomLeftPoint.y };
		auto middleDelta = dPOINT{};
		dudif(bottomMidpoint, topMidpoint, middleDelta);
		OSequence[ActivePointIndex].x = clipRatio.y * middleDelta.x + bottomMidpoint.x;
		OSequence[ActivePointIndex].y = clipRatio.y * middleDelta.y + bottomMidpoint.y;
		ActivePointIndex++;
	}
}

void form::internal::clpfm() noexcept {
	ActivePointIndex = 0;
	for (auto iSequence = 0u; iSequence < SequenceIndex - 2; iSequence += 2) {
		const auto leftLength
		    = hypot(BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y);
		const auto rightLength = hypot(BSequence[iSequence + 3].x - BSequence[iSequence + 2].x,
		                               BSequence[iSequence + 3].y - BSequence[iSequence + 2].y);
		const auto leftDelta
		    = dPOINT{ BSequence[iSequence + 1].x - BSequence[iSequence].x, BSequence[iSequence + 1].y - BSequence[iSequence].y };
		const auto rightDelta = dPOINT{ BSequence[iSequence + 2].x - BSequence[iSequence + 3].x,
			                            BSequence[iSequence + 2].y - BSequence[iSequence + 3].y };
		auto       count      = 0u;
		if (rightLength > leftLength) {
			count = gsl::narrow<decltype(count)>(std::round(leftLength / ClipRectSize.cy));
		}
		else {
			count = gsl::narrow<decltype(count)>(std::round(rightLength / ClipRectSize.cy));
		}
		if (count == 0u) {
			count = 1;
		}
		const auto leftStep  = dPOINT{ leftDelta.x / count, leftDelta.y / count };
		const auto rightStep = dPOINT{ rightDelta.x / count, rightDelta.y / count };
		auto       topLeft   = dPOINT{ BSequence[iSequence].x, BSequence[iSequence].y };
		auto       topRight  = dPOINT{ BSequence[iSequence + 3].x, BSequence[iSequence + 3].y };
		for (auto iStep = 0u; iStep < count; iStep++) {
			const auto bottomLeft  = topLeft;
			const auto bottomRight = topRight;
			topLeft.x += leftStep.x;
			topLeft.y += leftStep.y;
			topRight.x += rightStep.x;
			topRight.y += rightStep.y;
			if (ActivePointIndex > MAXITEMS - ClipBuffer->size() - 1) {
				return;
			}
			trfrm(bottomLeft, topLeft, bottomRight, topRight);
		}
	}
}

void form::internal::fmclp() {
	const auto savedSpacing = LineSpacing;

	LineSpacing = ClipRectSize.cx;
	StateMap.set(StateFlag::BARSAT);
	satin::satfil();
	StateMap.reset(StateFlag::BARSAT);
	clpfm();
	SelectedForm->fillType = CLPF;
	SequenceIndex          = ActivePointIndex;
	LineSpacing            = savedSpacing;
}

void form::refilfn() {
	const auto savedStitchLength   = UserStitchLength;
	auto       angledForm          = FRMHED{};
	auto       workingFormVertices = std::vector<fPOINT>{};
	WorkingFormVertices            = &workingFormVertices;

	StateMap.reset(StateFlag::TXFIL);
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->underlayIndent = 0;
	}
	thred::savdo();
	auto fillStartsData = FILLSTARTS{}; // fill start data for refill
	auto fillStartsMap  = 0u;           // fill starts bitmap
	xt::fdelstch(fillStartsData, fillStartsMap);
	StateMap.set(StateFlag::WASREFIL);
	if (SelectedForm->fillSpacing < 0.5 && !clip::isclp(ClosestFormToCursor)) {
		SelectedForm->fillSpacing = 0.5;
	}
	if (SelectedForm->edgeSpacing < 0.5) {
		SelectedForm->edgeSpacing = 0.5;
	}
	if (!clip::isclp(ClosestFormToCursor)) {
		UserStitchLength = SelectedForm->lengthOrCount.stitchLength;
	}
	if (!StateMap.test(StateFlag::WASDO)) {
		thred::savdo();
	}
	StateMap.reset(StateFlag::WASDO);
	if (((SelectedForm->extendedAttribute & (AT_UND | AT_WALK)) != 0u) && SelectedForm->type == FRMLINE
	    && SelectedForm->fillType != CONTF) {
		SelectedForm->type = FRMFPOLY;
	}
	InterleaveSequenceIndex       = 0;
	auto interleaveSequenceIndex2 = 0u; // index into interleave indices
	StateMap.reset(StateFlag::ISUND);
	auto textureSegments = std::vector<RNGCNT>{};
	textureSegments.resize(SelectedForm->fillInfo.texture.lines);
	switch (SelectedForm->type) {
	case FRMLINE: {
		HorizontalLength2 = SelectedForm->borderSize;
		switch (SelectedForm->edgeType & NEGUND) {
		case EDGELINE: {
			fi::brdfil(SelectedForm->edgeStitchLen);
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEBEAN: {
			fi::bold(SelectedForm->edgeStitchLen);
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGECLIP: {
			clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clip::clpout();
			SequenceIndex = 0;
			clip::clpbrd(0);
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEANGSAT: {
			StateMap.reset(StateFlag::SAT1);
			satin::slbrd();
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEPROPSAT: {
			if (SelectedForm->vertexCount > 2) {
				StateMap.reset(StateFlag::SAT1);
				fi::plbrd(SelectedForm->edgeSpacing, angledForm);
				fi::ritbrd(interleaveSequenceIndex2);
			}
			break;
		}
		case EDGEAPPL: {
			fi::lapbrd();
			fi::ritapbrd(interleaveSequenceIndex2);
			StateMap.reset(StateFlag::SAT1);
			satin::slbrd();
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEBHOL: {
			HorizontalLength2      = 2.0 * SelectedForm->borderSize;
			const auto length      = ButtonholeCornerLength;
			ButtonholeCornerLength = form::getblen();
			satin::satout(20);
			fi::blbrd(SelectedForm->edgeSpacing);
			ButtonholeCornerLength = length;
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEPICOT: {
			clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			SequenceIndex          = 0;
			const auto length      = ButtonholeCornerLength;
			ButtonholeCornerLength = form::getplen();
			clip::clpic();
			ButtonholeCornerLength = length;
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEDOUBLE: {
			fi::dubfn();
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGELCHAIN: {
			StateMap.set(StateFlag::LINCHN);
			clip::chnfn();
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGEOCHAIN: {
			StateMap.reset(StateFlag::LINCHN);
			clip::chnfn();
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		case EDGECLIPX: {
			clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
			clip::duxclp();
			fi::ritbrd(interleaveSequenceIndex2);
			break;
		}
		}
		if (SelectedForm->fillType == CONTF && ((SelectedForm->attribute & FRECONT) != 0)) {
			fi::contf();
			fi::ritfil(interleaveSequenceIndex2);
		}
		break;
	}
	case FRMFPOLY: {
		xt::chkcwlk(interleaveSequenceIndex2);
		xt::chkwlk(interleaveSequenceIndex2);
		xt::chkund(textureSegments, interleaveSequenceIndex2);
		StateMap.reset(StateFlag::ISUND);
		if (SelectedForm->fillType != 0u) {
			const auto spacing      = LineSpacing;
			LineSpacing             = SelectedForm->fillSpacing;
			auto lineEndpoints      = std::vector<SMALPNTL>{};
			auto groupIndexSequence = std::vector<unsigned>{};
			auto rotationCenter     = dPOINT{};
			auto doFill             = true;
			auto rotationAngle      = 0.0;
			switch (SelectedForm->fillType) {
			case VRTF: {
				WorkingFormVertices->clear();
				WorkingFormVertices->reserve(SelectedForm->vertexCount);
				auto startVertex = FormVertices->begin() + SelectedForm->vertexIndex;
				auto endVertex   = startVertex + SelectedForm->vertexCount;
				WorkingFormVertices->insert(WorkingFormVertices->end(), startVertex, endVertex);
				fi::fnvrt(WorkingFormVertices, groupIndexSequence, lineEndpoints);
				break;
			}
			case HORF: {
				rotationAngle = PI / 2;
				fi::fnhor(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm);
				WorkingFormVertices->clear();
				WorkingFormVertices->reserve(angledForm.vertexCount);
				auto startVertex = AngledFormVertices->begin() + angledForm.vertexIndex;
				auto endVertex   = startVertex + angledForm.vertexCount;
				WorkingFormVertices->insert(WorkingFormVertices->end(), startVertex, endVertex);
				break;
			}
			case ANGF: {
				rotationAngle = PI / 2 - SelectedForm->angleOrClipData.angle;
				fi::fnang(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm);
				WorkingFormVertices->clear();
				WorkingFormVertices->reserve(angledForm.vertexCount);
				auto startVertex = AngledFormVertices->begin() + angledForm.vertexIndex;
				auto endVertex   = startVertex + angledForm.vertexCount;
				WorkingFormVertices->insert(WorkingFormVertices->end(), startVertex, endVertex);
				break;
			}
			case VCLPF: {
				clip::oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				form::fvars(ClosestFormToCursor);
				WorkingFormVertices->clear();
				WorkingFormVertices->reserve(SelectedForm->vertexCount);
				auto startVertex = FormVertices->begin() + SelectedForm->vertexIndex;
				auto endVertex   = startVertex + SelectedForm->vertexCount;
				WorkingFormVertices->insert(WorkingFormVertices->end(), startVertex, endVertex);
				fi::clpcon(textureSegments, WorkingFormVertices);
				doFill = false;
				break;
			}
			case HCLPF: {
				clip::oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				fi::horclpfn(textureSegments, angledForm);
				doFill = false;
				break;
			}
			case ANGCLPF: {
				clip::oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
				StateMap.reset(StateFlag::ISUND);
				form::angclpfn(textureSegments);
				doFill = false;
				break;
			}
			case TXVRTF: {
				texture::setxt(textureSegments);
				WorkingFormVertices->clear();
				WorkingFormVertices->reserve(SelectedForm->vertexCount);
				auto startVertex = FormVertices->begin() + SelectedForm->vertexIndex;
				auto endVertex   = startVertex + SelectedForm->vertexCount;
				WorkingFormVertices->insert(WorkingFormVertices->end(), startVertex, endVertex);
				fi::clpcon(textureSegments, WorkingFormVertices);
				doFill = false;
				break;
			}
			case TXHORF: {
				texture::setxt(textureSegments);
				fi::horclpfn(textureSegments, angledForm);
				doFill = false;
				break;
			}
			case TXANGF: {
				texture::setxt(textureSegments);
				StateMap.reset(StateFlag::ISUND);
				form::angclpfn(textureSegments);
				doFill = false;
				break;
			}
			}
			if (doFill) {
				fi::lcon(groupIndexSequence, lineEndpoints);
				fi::bakseq();
				if (SelectedForm->fillType != VRTF && SelectedForm->fillType != TXVRTF) {
					rotationAngle = -rotationAngle;
					fi::rotbak(rotationAngle, rotationCenter);
				}
			}
			fi::ritfil(interleaveSequenceIndex2);
			LineSpacing = spacing;
		}
		fi::chkbrd(interleaveSequenceIndex2);
		break;
	}
	case SAT: {
		xt::chkcwlk(interleaveSequenceIndex2);
		xt::chkwlk(interleaveSequenceIndex2);
		xt::chkund(textureSegments, interleaveSequenceIndex2);
		StateMap.reset(StateFlag::ISUND);
		switch (SelectedForm->fillType) {
		case SATF: {
			const auto spacing = LineSpacing;
			LineSpacing        = SelectedForm->fillSpacing;
			UserStitchLength   = SelectedForm->lengthOrCount.stitchLength;
			satin::satfil();
			LineSpacing = spacing;
			fi::ritfil(interleaveSequenceIndex2);
			break;
		}
		case CLPF: {
			clip::oclp(SelectedForm->angleOrClipData.clip, SelectedForm->lengthOrCount.clipCount);
			fi::fmclp();
			fi::ritfil(interleaveSequenceIndex2);
			break;
		}
		case FTHF: {
			StateMap.set(StateFlag::CNV2FTH);
			xt::fthrfn(interleaveSequenceIndex2);
			break;
		}
		}
		fi::chkbrd(interleaveSequenceIndex2);
	}
	}
	UserStitchLength = savedStitchLength;
	xt::intlv(fillStartsData, fillStartsMap, interleaveSequenceIndex2);
	thred::ritot(PCSHeader.stitchCount);
	xt::setfchk();
}

void form::refil() {
	if (!UserFlagMap.test(UserFlag::WRNOF)) {
		const auto codedForm = ClosestFormToCursor << FRMSHFT | USMSK;
		for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			const auto attribute = StitchBuffer[iStitch].attribute;
			if (((attribute & NOTFRM) == 0u) && (attribute & (USMSK | FRMSK)) == codedForm) {
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

	const auto screenCoordinate = POINT{ (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };
	auto       vertexIt         = FormVertices->begin() + CurrentVertexIndex;

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
	if (SelectedForm->type == SAT) {
		if (SelectedForm->satinGuideCount != 0u) {
			satin::delsac(ClosestFormToCursor);
		}
	}
	SelectedForm->type = FRMFPOLY;
}

void form::internal::fsvrt() {
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	makpoli();
	SelectedForm->fillType  = VRTF;
	SelectedForm->type      = FRMFPOLY;
	SelectedForm->fillColor = gsl::narrow<decltype(SelectedForm->fillColor)>(ActiveColor);
	form::fsizpar();
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type        = FRMFPOLY;
	form::dusqr();
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
			form::fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE) {
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

void form::internal::fshor() {
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	makpoli();
	SelectedForm->fillType  = HORF;
	SelectedForm->fillColor = gsl::narrow<decltype(SelectedForm->fillColor)>(ActiveColor);
	form::fsizpar();
	SelectedForm->fillSpacing           = LineSpacing;
	SelectedForm->angleOrClipData.angle = static_cast<float>(PI) / 2;
	SelectedForm->type                  = FRMFPOLY;
	form::dusqr();
	form::refil();
}

void form::filhor() {
	if (displayText::filmsgs(FMM_HOR)) {
		return;
	}
	if (!SelectedFormList->empty()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE) {
				fi::fshor();
			}
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fi::fshor();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::internal::fsangl() {
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	makpoli();
	SelectedForm->fillType              = ANGF;
	SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
	SelectedForm->fillColor             = gsl::narrow<decltype(SelectedForm->fillColor)>(ActiveColor);
	form::fsizpar();
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type        = FRMFPOLY;
	form::dusqr();
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
			form::fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE) {
				fi::fsangl();
			}
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			fi::fsangl();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

bool form::chkfrm(std::vector<POINT>& stretchBoxLine, double& xyRatio) {
	form::fvars(ClosestFormToCursor);
	const auto point = POINT{ (Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y) };

	NewFormVertexCount = SelectedForm->vertexCount + 1;
	thred::duzrat();
	auto rectangle = RECT{};
	form::sRct2px(SelectedForm->rectangle, rectangle);
	auto& formControls = *FormControlPoints;
	formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = rectangle.left;
	formControls[1].x = formControls[5].x = dToL(form::midl(rectangle.right, rectangle.left));
	formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = rectangle.top;
	formControls[3].y = formControls[7].y = dToL(form::midl(rectangle.top, rectangle.bottom));
	formControls[4].y = formControls[5].y = formControls[6].y = rectangle.bottom;
	formControls[2].x = formControls[3].x = formControls[4].x = rectangle.right;

	auto minimumLength = 1e99;
	for (auto iControl = 0u; iControl < 10; iControl++) {
		const auto length = hypot(formControls[iControl].x - point.x, formControls[iControl].y - point.y);
		if (length < minimumLength) {
			minimumLength             = length;
			SelectedFormControlVertex = iControl;
		}
		if (minimumLength < CLOSENUF) {
			form::ritfrct(ClosestFormToCursor, StitchWindowDC);
			for (auto iCorner = 0u; iCorner < 4; iCorner++) {
				stretchBoxLine[iCorner] = formControls[gsl::narrow<size_t>(iCorner) << 1];
			}
			stretchBoxLine[4] = stretchBoxLine[0];
			thred::strtchbox(stretchBoxLine);
			if ((SelectedFormControlVertex & 1) != 0u) {
				StateMap.set(StateFlag::STRTCH);
			}
			else {
				StateMap.set(StateFlag::EXPAND);
				xyRatio = (static_cast<double>(SelectedForm->rectangle.right) - SelectedForm->rectangle.left)
				          / (static_cast<double>(SelectedForm->rectangle.top) - SelectedForm->rectangle.bottom);
			}
			SelectedFormControlVertex >>= 1;
			StateMap.set(StateFlag::SHOSTRTCH);
			return true;
		}
	}
	if (point.x >= rectangle.left && point.x <= rectangle.right && point.y >= rectangle.top && point.y <= rectangle.bottom) {
		auto formOrigin = POINT{};
		auto vertexIt   = FormVertices->begin() + SelectedForm->vertexIndex;
		form::sfCor2px(vertexIt[0], formOrigin);
		FormMoveDelta.x = gsl::narrow<float>(formOrigin.x - point.x);
		FormMoveDelta.y = gsl::narrow<float>(formOrigin.y - point.y);
		StateMap.set(StateFlag::FRMOV);
		return true;
	}
	return false;
}

void form::rstfrm() {
	const auto point     = POINT{ (Msg.pt.x + dToL(FormMoveDelta.x)), (Msg.pt.y + dToL(FormMoveDelta.y)) };
	const auto attribute = ClosestFormToCursor << 4;

	form::setmfrm();
	StateMap.reset(StateFlag::FRMOV);
	thred::pxCor2stch(point);
	auto       vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
	const auto offset   = fPOINT{ SelectedPoint.x - vertexIt[0].x, SelectedPoint.y - vertexIt[0].y };
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		vertexIt[iVertex].x += offset.x;
		vertexIt[iVertex].y += offset.y;
	}
	SelectedForm->rectangle.bottom += offset.y;
	SelectedForm->rectangle.top += offset.y;
	SelectedForm->rectangle.left += offset.x;
	SelectedForm->rectangle.right += offset.x;
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) == attribute && ((StitchBuffer[iStitch].attribute & ALTYPMSK) != 0u)
		    && ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u)) {
			StitchBuffer[iStitch].x += offset.x;
			StitchBuffer[iStitch].y += offset.y;
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

bool form::internal::scomp(const SATCON& arg1, const SATCON& arg2) noexcept {
	return (arg1.start < arg2.start);
}

void form::duinsf() noexcept {
	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, InsertLine, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::uninsf() {
	if (StateMap.testAndReset(StateFlag::SHOINSF)) {
		form::duinsf();
	}
}

void form::rotfrm(unsigned int newStartVertex) {
	form::fvars(ClosestFormToCursor);

	auto vertexIt        = FormVertices->begin() + CurrentVertexIndex;
	auto rotatedVertices = std::vector<fPOINT>{};
	rotatedVertices.resize(VertexCount);
	auto iVertex       = 0u;
	auto iGuide        = 0u;
	auto iRotatedGuide = 0u;
	auto iRotated      = newStartVertex;
	auto tlin          = 0u;

	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		rotatedVertices[iVertex] = vertexIt[iVertex];
	}
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex] = rotatedVertices[iRotated];
		iRotated          = form::nxt(iRotated);
	}
	iRotatedGuide = 0;
	auto guideIt  = SatinGuides->begin() + CurrentFormGuides;
	if (SelectedForm->type == SAT) {
		if (SelectedForm->wordParam != 0u) {
			SelectedForm->wordParam
			    = (SelectedForm->wordParam + SelectedForm->vertexCount - newStartVertex) % SelectedForm->vertexCount;
		}
		if (VertexCount != 0u) {
			for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				if (guideIt[iGuide].start != newStartVertex && guideIt[iGuide].finish != newStartVertex) {
					guideIt[iRotatedGuide].start  = (guideIt[iGuide].start + VertexCount - newStartVertex) % VertexCount;
					guideIt[iRotatedGuide].finish = (guideIt[iGuide].finish + VertexCount - newStartVertex) % VertexCount;
					if (guideIt[iRotatedGuide].start > guideIt[iRotatedGuide].finish) {
						tlin                         = guideIt[iRotatedGuide].start;
						guideIt[iRotatedGuide].start = guideIt[iRotatedGuide].finish;
						guideIt[iGuide].finish       = tlin;
					}
					iRotatedGuide++;
				}
			}
		}
	}
	if (iRotatedGuide != 0u) {
		SelectedForm->satinGuideCount = iRotatedGuide;
		// ToDo - Can we do the sort in place?
		auto rotatedGuides = std::vector<SATCON>{};
		rotatedGuides.resize(iRotatedGuide);
		for (iGuide = 0; iGuide < iRotatedGuide; iGuide++) {
			rotatedGuides[iGuide] = guideIt[iGuide];
		}
		std::sort(rotatedGuides.begin(), rotatedGuides.end(), fi::scomp);
		for (iGuide = 0; iGuide < iRotatedGuide; iGuide++) {
			guideIt[iGuide] = rotatedGuides[iGuide];
		}
	}
	if (VertexCount != 0u) {
		if ((SelectedForm->extendedAttribute & AT_STRT) != 0u) {
			SelectedForm->fillStart = (SelectedForm->fillStart + VertexCount - newStartVertex) % VertexCount;
		}
		if ((SelectedForm->extendedAttribute & AT_END) != 0u) {
			SelectedForm->fillEnd = (SelectedForm->fillEnd + VertexCount - newStartVertex) % VertexCount;
		}
	}
}

void form::internal::filsfn() {
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	SelectedForm->type = SAT;
	form::fsizpar();
	SelectedForm->fillType    = SATF;
	SelectedForm->fillColor   = gsl::narrow<unsigned char>(ActiveColor);
	SelectedForm->fillSpacing = LineSpacing;
	SelectedForm->type        = SAT;
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
			form::fvars(ClosestFormToCursor);
			if (SelectedForm->type != FRMLINE) {
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

bool form::internal::closat(intersectionStyles& inOutFlag) {
	auto minimumLength = 1e99;

	thred::px2stch();
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& formIter = (*FormList)[iForm];
		if ((ActiveLayer == 0u) || ((formIter.attribute & FRMLMSK) >> 1) == ActiveLayer || ((formIter.attribute & FRMLMSK) == 0u)) {
			CurrentVertexIndex     = formIter.vertexIndex;
			const auto savedVertex = VertexCount;
			VertexCount            = formIter.vertexCount;
			auto lastVertex        = 0u;
			if (formIter.type == FRMLINE) {
				lastVertex = VertexCount - 1;
			}
			else {
				lastVertex = VertexCount;
			}
			// Loop through for all line segments
			auto length   = 0.0;
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < lastVertex; iVertex++) {
				const auto param = findDistanceToSide(vertexIt[iVertex], vertexIt[form::nxt(iVertex)], SelectedPoint, length);
				if ((length < minimumLength)) {
					if ((param < 0.0) && (iVertex == 0)) {
						// this should only happen if the Closest vertex is the start of a line (vertex 0)
						minimumLength         = length;
						ClosestFormToCursor   = iForm;
						ClosestVertexToCursor = iVertex;
						inOutFlag             = POINT_BEFORE_LINE;
					}
					else {
						// return the vertex after the intersection
						if ((param > 1.0) && (iVertex == lastVertex - 1)) {
							minimumLength         = length;
							ClosestFormToCursor   = iForm;
							ClosestVertexToCursor = form::nxt(iVertex);
							inOutFlag             = POINT_AFTER_LINE;
						}
						else {
							minimumLength         = length;
							ClosestFormToCursor   = iForm;
							ClosestVertexToCursor = form::nxt(iVertex);
							inOutFlag             = POINT_IN_LINE;
						}
					}
				}
			}
			VertexCount = savedVertex;
		}
	}
	return minimumLength != 1e99;
}

void form::internal::nufpnt(unsigned int vertex, FRMHED* formForInsert) {
	if (formForInsert != nullptr) {
		form::fltspac(vertex + 1, 1);
		formForInsert->vertexCount++;
		auto vertexIt        = FormVertices->begin() + formForInsert->vertexIndex;
		vertexIt[gsl::narrow_cast<size_t>(vertex) + 1u] = SelectedPoint;
		auto guideIt         = SatinGuides->begin() + formForInsert->satinOrAngle.guide;
		for (auto ind = 0u; ind < formForInsert->satinGuideCount; ind++) {
			if (guideIt[ind].start > vertex) {
				guideIt[ind].start++;
			}
			if (guideIt[ind].finish > vertex) {
				guideIt[ind].finish++;
			}
		}
		if (formForInsert->wordParam >= vertex + 1) {
			formForInsert->wordParam++;
			formForInsert->wordParam %= VertexCount;
		}
		if (formForInsert->fillType == CONTF) {
			if (formForInsert->angleOrClipData.guide.start > vertex) {
				formForInsert->angleOrClipData.guide.start++;
			}
			if (formForInsert->angleOrClipData.guide.finish > vertex) {
				formForInsert->angleOrClipData.guide.finish++;
			}
		}
		form::frmlin(formForInsert->vertexIndex, formForInsert->vertexCount);
	}
}

void form::insat() { // insert a point in a form
	// clang-format off
	auto inOutFlag = POINT_IN_LINE;
	// clang-format on
	if (fi::closat(inOutFlag)) {
		thred::savdo();
		auto*      selectedForm = &((*FormList)[ClosestFormToCursor]);
		const auto lastVertex   = selectedForm->vertexCount - 1;
		form::fvars(ClosestFormToCursor);
		if (inOutFlag != POINT_IN_LINE) {
			if (ClosestVertexToCursor == 0 && selectedForm->type == FRMLINE) {
				StateMap.set(StateFlag::PRELIN);
			}
			else {
				if (ClosestVertexToCursor != lastVertex && selectedForm->type == FRMLINE) {
					ClosestVertexToCursor = form::prv(ClosestVertexToCursor);
				}
			}
			fi::nufpnt(ClosestVertexToCursor, selectedForm);
			auto vertexIt = FormVertices->begin() + selectedForm->vertexIndex;
			if (StateMap.testAndReset(StateFlag::PRELIN)) {
				SelectedPoint = vertexIt[0];
				vertexIt[0]   = vertexIt[1];
				vertexIt[1]   = SelectedPoint;
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

bool form::chkdel() noexcept {
	if (SelectedForm->type == FRMLINE) {
		return SelectedForm->vertexCount <= 2;
	}

	return SelectedForm->vertexCount <= 3;
}

void form::unfil() {
	if (displayText::filmsgs(FMX_UNF)) {
		return;
	}
	if (!SelectedFormList->empty()) {
		auto formMap = boost::dynamic_bitset<>(FormList->size());
		for (auto selectedForm : (*SelectedFormList)) {
			SelectedForm = &((*FormList)[selectedForm]);
			if ((SelectedForm->fillType != 0u) || (SelectedForm->edgeType != 0u)) {
				clip::delclps(selectedForm);
				texture::deltx();
				formMap.set(selectedForm);
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				SelectedForm->extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
			}
		}
		auto iDestination = 0u;
		for (auto iSource = 0u; iSource < PCSHeader.stitchCount; iSource++) {
			if (!formMap.test((StitchBuffer[iSource].attribute & FRMSK) >> FRMSHFT)) {
				StitchBuffer[iDestination++] = StitchBuffer[iSource];
			}
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestination);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (!StateMap.testAndReset(StateFlag::IGNOR) && !UserFlagMap.test(UserFlag::WRNOF)) {
				const auto codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
				for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
					const auto attribute = StitchBuffer[iStitch].attribute;
					if (((attribute & NOTFRM) == 0u) && (attribute & (USMSK | FRMSK)) == codedForm) {
						displayText::tabmsg(IDS_UNFIL);
						StateMap.set(StateFlag::FILMSG);
						displayText::okcan();
						StateMap.set(StateFlag::IGNOR);
						return;
					}
				}
			}
			const auto codedForm    = ClosestFormToCursor << FRMSHFT;
			auto       iDestination = 0u;
			for (auto iSource = 0u; iSource < PCSHeader.stitchCount; iSource++) {
				if ((StitchBuffer[iSource].attribute & FRMSK) != codedForm || ((StitchBuffer[iSource].attribute & NOTFRM) != 0u)) {
					StitchBuffer[iDestination++] = StitchBuffer[iSource];
				}
			}
			clip::delclps(ClosestFormToCursor);
			texture::deltx();
			SelectedForm->fillType = 0;
			SelectedForm->edgeType = 0;
			SelectedForm->extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
			PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestination);
			thred::ritot(PCSHeader.stitchCount);
		}
	}
}

void form::frm0() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		thred::savdo();
		form::rotfrm(ClosestVertexToCursor);
		ClosestVertexToCursor = 0;
		satin::satadj();
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
	if ((FormVertexNext != 0u) || FormForInsert->type != FRMLINE) {
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
	auto inOutFlag = POINT_IN_LINE;
	// clang-format on
	if (fi::closat(inOutFlag)) {
		FormForInsert = &((*FormList)[ClosestFormToCursor]);
		form::fvars(ClosestFormToCursor);
		if (inOutFlag != POINT_IN_LINE) {
			if ((ClosestVertexToCursor == 0u) && FormForInsert->type == FRMLINE) {
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
	fi::nufpnt(FormVertexPrev, FormForInsert);
	if (StateMap.test(StateFlag::PRELIN)) {
		auto vertexIt = FormVertices->begin() + FormForInsert->vertexIndex;
		SelectedPoint = vertexIt[0];
		vertexIt[0]   = vertexIt[1];
		vertexIt[1]   = SelectedPoint;
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

void form::bsizpar() noexcept {
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->edgeStitchLen      = UserStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
}

void form::internal::sbord() {
	form::fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGELINE;
	form::bsizpar();
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
			SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
			fi::sbord();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
			fi::sbord();
			thred::coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::internal::fsclp() {
	clip::deleclp(ClosestFormToCursor);
	const auto clipSize          = gsl::narrow<unsigned int>(ClipBuffer->size());
	SelectedForm->edgeType       = EDGECLIP;
	SelectedForm->clipEntries    = clipSize;
	SelectedForm->borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
	SelectedForm->borderSize     = ClipRectSize.cy;
	SelectedForm->edgeSpacing    = ClipRectSize.cx;
	SelectedForm->borderColor    = gsl::narrow<unsigned char>(ActiveColor);
	form::bsizpar();
	auto offsetStart = ClipPoints->begin() + SelectedForm->borderClipData;
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
	clip::clpout();
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
						fi::fsclp();
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::fsclp();
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

void form::internal::sapliq() {
	form::fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEAPPL;
	if (UserFlagMap.test(UserFlag::DUND)) {
		SelectedForm->edgeType |= EGUND;
	}
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderSize  = IniFile.borderWidth;
	form::bsizpar();
	SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor | (AppliqueColor << 4));
	if (SelectedForm->type != FRMLINE) {
		if (SelectedForm->fillType == SAT && (SelectedForm->satinGuideCount != 0u)) {
			satin::delsac(ClosestFormToCursor);
		}
	}
	SelectedForm->fillType = 0;
	SelectedForm->type     = FRMFPOLY;
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
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				SelectedForm->attribute |= (SBLNT | FBLNT);
			}
			else {
				SelectedForm->attribute &= NOBLNT;
			}
			fi::sapliq();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				SelectedForm->attribute |= (SBLNT | FBLNT);
			}
			else {
				SelectedForm->attribute &= NOBLNT;
			}
			fi::sapliq();
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
	displayText::shoMsg(fmt::format(fmtStr, (AppliqueColor + 1)));
}

void form::internal::getbig() noexcept {
	AllItemsRect.bottom = AllItemsRect.left = 1e9;
	AllItemsRect.top = AllItemsRect.right = 0;
	for (auto & iForm : *FormList) {
		const auto& trct = iForm.rectangle;
		if (trct.bottom < AllItemsRect.bottom) {
			AllItemsRect.bottom = trct.bottom;
		}
		if (trct.left < AllItemsRect.left) {
			AllItemsRect.left = trct.left;
		}
		if (trct.right > AllItemsRect.right) {
			AllItemsRect.right = trct.right;
		}
		if (trct.top > AllItemsRect.top) {
			AllItemsRect.top = trct.top;
		}
	}
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (StitchBuffer[iStitch].x < AllItemsRect.left) {
			AllItemsRect.left = StitchBuffer[iStitch].x;
		}
		if (StitchBuffer[iStitch].x > AllItemsRect.right) {
			AllItemsRect.right = StitchBuffer[iStitch].x;
		}
		if (StitchBuffer[iStitch].y < AllItemsRect.bottom) {
			AllItemsRect.bottom = StitchBuffer[iStitch].y;
		}
		if (StitchBuffer[iStitch].y > AllItemsRect.top) {
			AllItemsRect.top = StitchBuffer[iStitch].y;
		}
	}
}

void form::stchrct2px(const fRECTANGLE& stitchRect, RECT& screenRect) noexcept {
	auto stitchCoord = dPOINT{ stitchRect.left, stitchRect.top };
	auto screenCoord = POINT{};

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
	ZoomRect.bottom = 0;
	ZoomRect.left   = 0;
	ZoomRect.right  = UnzoomedRect.x;
	ZoomRect.top    = UnzoomedRect.y;
	ZoomFactor      = 1;
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
	auto reference  = 0.0f;
	auto ratio      = 1.0;
	auto stitchRect = fRECTANGLE{};

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
			stitchRect.bottom = stitchRect.left = stitchRect.right = stitchRect.top = 0;
		}
	}
	switch (SelectedFormControlVertex) {
	case 0: { // top control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.bottom;
			const auto offsetY = Msg.pt.y - StitchWindowOrigin.y;
			ratio              = (static_cast<double>(SelectedFormsRect.bottom) - offsetY)
			        / (static_cast<double>(SelectedFormsRect.bottom) - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.bottom;
				ratio     = (static_cast<double>(SelectedPoint.y) - reference)
				        / (static_cast<double>(SelectedForm->rectangle.top) - reference);
				SelectedForm->rectangle.top = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.bottom;
				ratio
				    = (static_cast<double>(SelectedPoint.y) - reference) / (static_cast<double>(StitchRangeRect.top) - reference);
			}
		}
		break;
	}
	case 1: { // right control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.left;
			const auto offsetX = Msg.pt.x - StitchWindowOrigin.x;
			ratio              = (static_cast<double>(offsetX) - SelectedFormsRect.left)
			        / (static_cast<double>(SelectedFormsRect.right) - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.left;
				ratio     = (static_cast<double>(SelectedPoint.x) - reference)
				        / (static_cast<double>(SelectedForm->rectangle.right) - reference);
				SelectedForm->rectangle.right = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.left;
				ratio     = (static_cast<double>(SelectedPoint.x) - reference)
				        / (static_cast<double>(StitchRangeRect.right) - reference);
			}
		}
		break;
	}
	case 2: { // bottom control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.top;
			const auto offsetY = Msg.pt.y - StitchWindowOrigin.y;
			ratio              = (static_cast<double>(offsetY) - SelectedFormsRect.top)
			        / (static_cast<double>(SelectedFormsRect.bottom) - SelectedFormsRect.top);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.top;
				ratio     = (static_cast<double>(SelectedPoint.y) - reference)
				        / (static_cast<double>(SelectedForm->rectangle.bottom) - reference);
				SelectedForm->rectangle.bottom = SelectedPoint.y;
			}
			else {
				reference = StitchRangeRect.top;
				ratio     = (static_cast<double>(SelectedPoint.y) - reference)
				        / (static_cast<double>(StitchRangeRect.bottom) - reference);
			}
		}
		break;
	}
	case 3: { // left control
		if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
			reference          = stitchRect.right;
			const auto offsetX = Msg.pt.x - StitchWindowOrigin.x;
			ratio              = (static_cast<double>(SelectedFormsRect.right) - offsetX)
			        / (static_cast<double>(SelectedFormsRect.right) - SelectedFormsRect.left);
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				reference = SelectedForm->rectangle.right;
				ratio     = (static_cast<double>(SelectedPoint.x) - reference)
				        / (static_cast<double>(SelectedForm->rectangle.left) - reference);
				SelectedForm->rectangle.left = SelectedPoint.x;
			}
			else {
				reference = StitchRangeRect.right;
				ratio     = (static_cast<double>(SelectedPoint.x) - reference)
				        / (static_cast<double>(StitchRangeRect.left) - reference);
			}
		}
		break;
	}
	}
	if ((SelectedFormControlVertex & 1) != 0u) { // right or left control selected
		if (StateMap.test(StateFlag::FPSEL)) {
			form::fvars(ClosestFormToCursor);
			auto currentVertex = SelectedFormVertices.start;
			auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
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
			for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
				auto& formIter     = (*FormList)[iForm];
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
				for (auto iVertex = 0u; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
				}
				form::frmout(iForm);
			}
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference) * ratio + reference;
			}
			selal();
			return;
		}

		if (!SelectedFormList->empty()) {
			for (auto selectedForm : (*SelectedFormList)) {
				auto& formIter     = (*FormList)[selectedForm];
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
				for (auto iVertex = 0u; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
				for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
					vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
				}
			}
			else {
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference) * ratio + reference;
				}
			}
		}
	}
	else {
		if (StateMap.test(StateFlag::FPSEL)) {
			form::fvars(ClosestFormToCursor);
			auto currentVertex = SelectedFormVertices.start;
			auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
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
			for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
				auto& formIter     = (*FormList)[iForm];
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
				for (auto iVertex = 0u; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
				}
				form::frmout(iForm);
			}
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference) * ratio + reference;
			}
			selal();
			return;
		}

		if (!SelectedFormList->empty()) {
			for (auto selectedForm : (*SelectedFormList)) {
				auto& formIter     = (*FormList)[selectedForm];
				CurrentVertexIndex = formIter.vertexIndex;
				auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
				for (auto iVertex = 0u; iVertex < formIter.vertexCount; iVertex++) {
					vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::FORMSEL)) {
				auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
				for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
					vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
				}
			}
			else {
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference) * ratio + reference;
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

void form::setexpand(double xyRatio) {
	auto size0     = dPOINT{};
	auto rectangle = fRECTANGLE{};

	thred::savdo();
	if (!SelectedFormList->empty() || StateMap.test(StateFlag::BIGBOX) || StateMap.test(StateFlag::FPSEL)) {
		rectangle.bottom = SelectedFormsRect.bottom;
		rectangle.left   = SelectedFormsRect.left;
		rectangle.right  = SelectedFormsRect.right;
		rectangle.top    = SelectedFormsRect.top;
		SelectedPoint.x  = Msg.pt.x - StitchWindowOrigin.x;
		SelectedPoint.y  = Msg.pt.y - StitchWindowOrigin.y;
		size0.y          = static_cast<double>(rectangle.bottom) - rectangle.top;
	}
	else {
		thred::px2stch();
		form::fvars(ClosestFormToCursor);
		if (StateMap.test(StateFlag::FORMSEL)) {
			rectangle = SelectedForm->rectangle;
		}
		else {
			rectangle.bottom = StitchRangeRect.bottom;
			rectangle.top    = StitchRangeRect.top;
			rectangle.right  = StitchRangeRect.right;
			rectangle.left   = StitchRangeRect.left;
		}
		size0.y = static_cast<double>(rectangle.top) - rectangle.bottom;
	}
	auto ratio     = dPOINT{ 1.0, 1.0 };
	auto reference = dPOINT{};
	size0.x        = static_cast<double>(rectangle.right) - rectangle.left;
	switch (SelectedFormControlVertex) {
	case 0: {
		reference.x       = rectangle.right;
		reference.y       = rectangle.bottom;
		auto       size1  = dPOINT{ fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
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
			SelectedForm->rectangle.left = rectangle.right - size1.x;
			SelectedForm->rectangle.top  = rectangle.bottom + size1.y;
		}
		break;
	}
	case 1: {
		reference.x       = rectangle.left;
		reference.y       = rectangle.bottom;
		auto       size1  = dPOINT{ fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
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
			SelectedForm->rectangle.right = rectangle.left + size1.x;
			SelectedForm->rectangle.top   = rectangle.bottom + size1.y;
		}
		break;
	}
	case 2: {
		reference.x       = rectangle.left;
		reference.y       = rectangle.top;
		auto       size1  = dPOINT{ fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
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
			SelectedForm->rectangle.right  = rectangle.left + size1.x;
			SelectedForm->rectangle.bottom = rectangle.top - size1.y;
		}
		break;
	}
	case 3: {
		reference.x       = rectangle.right;
		reference.y       = rectangle.top;
		auto       size1  = dPOINT{ fabs(SelectedPoint.x - reference.x), fabs(SelectedPoint.y - reference.y) };
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
			SelectedForm->rectangle.left   = rectangle.right - size1.x;
			SelectedForm->rectangle.bottom = rectangle.top - size1.y;
		}
		break;
	}
	}
	const auto integerReference = POINT{ dToL(reference.x), dToL(reference.y) };
	auto       stitchReference  = fPOINT{};
	fi::px2stchf(integerReference, stitchReference);
	if (StateMap.test(StateFlag::FPSEL)) {
		form::fvars(ClosestFormToCursor);
		auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
		auto iCurrent = SelectedFormVertices.start;
		for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
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
		for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
			form::fvars(iForm);
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
				vertexIt[iVertex].x = (vertexIt[iVertex].x - stitchReference.x) * ratio.x + stitchReference.x;
				vertexIt[iVertex].y = (vertexIt[iVertex].y - stitchReference.y) * ratio.y + stitchReference.y;
			}
			form::frmout(iForm);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - stitchReference.x) * ratio.x + stitchReference.x;
			StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - stitchReference.y) * ratio.y + stitchReference.y;
		}
		form::selal();
		return;
	}

	if (!SelectedFormList->empty()) {
		for (auto selectedForm : (*SelectedFormList)) {
			form::fvars(selectedForm);
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
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
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].x = (vertexIt[iVertex].x - reference.x) * ratio.x + reference.x;
				vertexIt[iVertex].y = (vertexIt[iVertex].y - reference.y) * ratio.y + reference.y;
			}
			form::refil();
		}
		else {
			for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
				StitchBuffer[iStitch].x = (StitchBuffer[iStitch].x - reference.x) * ratio.x + reference.x;
				StitchBuffer[iStitch].y = (StitchBuffer[iStitch].y - reference.y) * ratio.y + reference.y;
			}
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

void form::nufilcol(unsigned color) noexcept {
	if (SelectedForm->fillColor != gsl::narrow<unsigned char>(color)) {
		SelectedForm->fillColor = gsl::narrow<unsigned char>(color);
		const auto attribute    = (ClosestFormToCursor << 4) | FRMFIL;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (FRMSK | TYPMSK | FTHMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void form::nufthcol(unsigned color) noexcept {
	if (SelectedForm->fillInfo.feather.color != gsl::narrow<unsigned char>(color)) {
		SelectedForm->fillInfo.feather.color = gsl::narrow<unsigned char>(color);
		const auto attribute                 = (ClosestFormToCursor << 4) | FTHMSK;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (FRMSK | FTHMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void form::nubrdcol(unsigned color) noexcept {
	SelectedForm->borderColor = gsl::narrow<unsigned char>(color);
	const auto attribute      = (ClosestFormToCursor << 4) | FRMBFIL;
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & (FRMSK | TYPMSK)) == attribute) {
			StitchBuffer[iStitch].attribute &= 0xfffffff0;
			StitchBuffer[iStitch].attribute |= color;
		}
	}
}

void form::nulapcol(unsigned color) noexcept {
	if (gsl::narrow<unsigned>(SelectedForm->borderColor >> 4) != color) {
		SelectedForm->borderColor &= COLMSK;
		SelectedForm->borderColor |= color << 4;
		const auto attribute = (ClosestFormToCursor << 4) | TYPMSK;
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & (TYPMSK | FRMSK)) == attribute) {
				StitchBuffer[iStitch].attribute &= 0xfffffff0;
				StitchBuffer[iStitch].attribute |= color;
			}
		}
	}
}

void form::internal::sbold() {
	form::fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->edgeType    = EDGEBEAN;
	SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
	form::bsizpar();
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
			fi::sbold();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fi::sbold();
			thred::coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
			thred::ritot(PCSHeader.stitchCount);
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

void form::dustar(unsigned starCount, double length) {
	if (starCount < 3) {
		starCount = 3;
	}
	if (starCount > 100) {
		starCount = 100;
	}
	const auto stepAngle   = PI / starCount;
	auto       angle       = stepAngle / 2.0 + PI;
	const auto vertexCount = starCount << 1;
	auto       newForm     = FRMHED{};
	newForm.vertexIndex    = thred::adflt(vertexCount);
	newForm.vertexCount    = vertexCount;
	newForm.attribute      = gsl::narrow<unsigned char>(ActiveLayer << 1);
	newForm.type           = FRMFPOLY;
	FormList->push_back(newForm);
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point = dPOINT{ SelectedPoint };
	StateMap.set(StateFlag::FILDIR);
	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < vertexCount; iVertex++) {
		vertexIt[iVertex].x = point.x;
		vertexIt[iVertex].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	const auto center = dPOINT{ (static_cast<double>(vertexIt[starCount].x) - vertexIt[0].x) / 2.0 + vertexIt[0].x,
		                        (static_cast<double>(vertexIt[starCount].y) - vertexIt[0].y) / 2.0 + vertexIt[0].y };
	for (auto iVertex = 1u; iVertex < vertexCount; iVertex += 2) {
		vertexIt[iVertex].x = (vertexIt[iVertex].x - center.x) * StarRatio + center.x;
		vertexIt[iVertex].y = (vertexIt[iVertex].y - center.y) * StarRatio + center.y;
	}
	form::frmout(gsl::narrow<unsigned int>(FormList->size() - 1u));
	FormMoveDelta      = fPOINT{ 0.0f, 0.0f };
	NewFormVertexCount = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::duspir(unsigned stepCount) {
	if (stepCount < 3) {
		stepCount = 3;
	}
	if (stepCount > 100) {
		stepCount = 100;
	}
	const auto stepAngle = PI * 2.0 / stepCount;
	// ToDo - Why 800?
	const auto length = 800.0 / stepCount * ZoomFactor * (static_cast<double>(UnzoomedRect.x) + UnzoomedRect.y)
	                    / (static_cast<double>(LHUPX) + LHUPY);
	auto newForm        = FRMHED{};
	auto vertexCount    = gsl::narrow<unsigned int>(std::round(stepCount * SpiralWrap));
	newForm.vertexIndex = thred::adflt(vertexCount);
	newForm.vertexCount = vertexCount;
	newForm.attribute   = gsl::narrow<unsigned char>(ActiveLayer << 1);
	FormList->push_back(newForm);
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	auto firstSpiral    = std::vector<fPOINT>{};
	firstSpiral.resize(stepCount);
	auto centeredSpiral = std::vector<fPOINT>{};
	centeredSpiral.resize(stepCount);
	form::fvars(gsl::narrow<unsigned int>(FormList->size() - 1u));
	thred::px2stch();
	auto point = dPOINT{ SelectedPoint };
	auto angle = 0.0;
	for (auto iStep = 0u; iStep < stepCount; iStep++) {
		firstSpiral[iStep].x = point.x;
		firstSpiral[iStep].y = point.y;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
	}
	const auto center = dPOINT{ (firstSpiral[stepCount >> 1].x - firstSpiral[0].x) / 2 + firstSpiral[0].x,
		                        (firstSpiral[stepCount >> 1].y - firstSpiral[0].y) / 2 + firstSpiral[0].y };
	for (auto iStep = 0u; iStep < stepCount; iStep++) {
		centeredSpiral[iStep].x = firstSpiral[iStep].x - center.x;
		centeredSpiral[iStep].y = firstSpiral[iStep].y - center.y;
	}
	const auto stepRatio = 1.0 / vertexCount;
	auto       ratio     = stepRatio;
	auto       vertexIt  = FormVertices->begin() + SelectedForm->vertexIndex;
	for (auto iVertex = 0u; iVertex < vertexCount; iVertex++) {
		vertexIt[iVertex].x = centeredSpiral[iVertex % stepCount].x * ratio + center.x;
		vertexIt[iVertex].y = centeredSpiral[iVertex % stepCount].y * ratio + center.y;
		ratio += stepRatio;
	}
	SelectedForm->type = FRMLINE;
	form::frmout(gsl::narrow<unsigned int>(FormList->size() - 1u));
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = vertexCount + 1;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::duhart(unsigned sideCount) {
	if (sideCount > 100) {
		sideCount = 100;
	}
	if (sideCount < 6) {
		sideCount = 6;
	}
	FormList->push_back(FRMHED{});
	SelectedForm              = &(FormList->back());
	SelectedForm->vertexIndex = gsl::narrow<decltype(SelectedForm->vertexIndex)>(FormVertices->size());
	SelectedForm->attribute   = gsl::narrow<unsigned char>(ActiveLayer << 1);
	FormVertices->reserve(FormVertices->size() + gsl::narrow_cast<size_t>(sideCount) * 2u - 2u);
	CurrentVertexIndex = gsl::narrow<decltype(CurrentVertexIndex)>(FormVertices->size());
	thred::px2stch();
	auto       point     = dPOINT{ SelectedPoint };
	auto       stepAngle = PI * 2.0 / sideCount;
	const auto length    = 300.0 / sideCount * ZoomFactor * (static_cast<double>(UnzoomedRect.x) + UnzoomedRect.y)
	                    / (static_cast<double>(LHUPX) + LHUPY);
	auto angle    = PI * 0.28;
	auto iVertex  = 0u;
	auto maximumX = 0.0f;
	while (angle > -PI * 0.7) {
		if (point.x > maximumX) {
			maximumX = point.x;
		}
		FormVertices->push_back(fPOINT{ point.x, point.y });
		iVertex++;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	stepAngle /= 4.5;
	auto lastVertex = iVertex;
	auto vertexIt   = std::next(FormVertices->begin(), CurrentVertexIndex);
	while (point.x > vertexIt[0].x && iVertex < 200) {
		FormVertices->push_back(fPOINT{ point.x, point.y });
		iVertex++;
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle -= stepAngle;
	}
	const auto firstVertex = iVertex;
	const auto ratio = (vertexIt[lastVertex - 1].x - vertexIt[0].x) / (vertexIt[lastVertex - 1].x - vertexIt[firstVertex - 1].x);
	for (iVertex = lastVertex; iVertex < firstVertex; iVertex++) {
		vertexIt[iVertex].x = (vertexIt[iVertex].x - vertexIt[lastVertex - 1].x) * ratio + vertexIt[lastVertex - 1].x;
	}
	auto iDestination = iVertex;
	lastVertex        = iVertex;
	for (iVertex = lastVertex - 2; iVertex != 0; iVertex--) {
		FormVertices->push_back(
		    fPOINT{ maximumX + maximumX - vertexIt[iVertex].x - 2 * (maximumX - vertexIt[0].x), vertexIt[iVertex].y });
		iDestination++;
	}
	NewFormVertexCount        = iDestination + 1;
	SelectedForm->vertexCount = iDestination;
	SelectedForm->type        = FRMFPOLY;
	ClosestFormToCursor       = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::frmout(ClosestFormToCursor);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::dulens(unsigned sides) {
	// ToDo - this does not produce a consistent size of lens
	//        or the correct number of sides
	if (sides < 6) {
		sides = 6;
	}
	if (sides > 48) {
		sides = 48;
	}
	const auto steps     = sides << 1;
	const auto stepAngle = PI * 2.0 / steps;
	auto       count     = gsl::narrow<unsigned int>(std::round(steps / 2.0 * 0.3));
	auto       angle     = count * stepAngle;
	const auto length    = 500.0 / steps * ZoomFactor * (gsl::narrow_cast<double>(UnzoomedRect.x) + UnzoomedRect.y)
	                    / (gsl::narrow_cast<double>(LHUPX) + LHUPY);
	FormList->push_back(FRMHED{});
	SelectedForm              = &(FormList->back());
	ClosestFormToCursor       = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	SelectedForm->vertexIndex = gsl::narrow<decltype(SelectedForm->vertexIndex)>(FormVertices->size());
	SelectedForm->attribute   = gsl::narrow<unsigned char>(ActiveLayer << 1);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	auto point   = dPOINT{ SelectedPoint };
	auto iVertex = 0u;
	SelectedPoint.x -= 0.0001f;
	while (point.x >= SelectedPoint.x) {
		FormVertices->push_back(fPOINT{ point });
		point.x += length * cos(angle);
		point.y += length * sin(angle);
		angle += stepAngle;
		iVertex++;
	}
	auto lastVertex = iVertex;
	if (lastVertex != 0u) {
		lastVertex--;
	}
	auto       vertexIt = std::next(FormVertices->cbegin(), SelectedForm->vertexIndex);
	const auto av       = vertexIt[0].x;
	for (iVertex = lastVertex; iVertex != 1; iVertex--) {
		FormVertices->push_back(fPOINT{ av + av - vertexIt[iVertex - 1].x, vertexIt[iVertex - 1].y });
	}
	SelectedForm->vertexCount
	    = gsl::narrow<decltype(SelectedForm->vertexCount)>(FormVertices->size() - SelectedForm->vertexIndex);
	NewFormVertexCount        = SelectedForm->vertexCount + 1;
	SelectedForm->type        = FRMFPOLY;
	form::frmout(ClosestFormToCursor);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

constexpr float form::internal::shreg(float highValue, float reference, double eggRatio) noexcept {
	return (highValue - reference) * eggRatio + reference;
}

void form::dueg(unsigned sides) {
	if (sides < 8) {
		sides = 8;
	}
	form::durpoli(sides);

	auto       vertexIt  = FormVertices->begin() + CurrentVertexIndex;
	const auto reference = form::midl(vertexIt[sides / 2].y, vertexIt[0].y);
	const auto maximumY  = static_cast<double>(vertexIt[sides >> 2].y) - vertexIt[0].y;

	for (unsigned iVertex = 0; iVertex < sides; iVertex++) {
		if (vertexIt[iVertex].y < reference) {
			vertexIt[iVertex].y = reference - (reference - vertexIt[iVertex].y) * IniFile.eggRatio;
		}
	}
	const auto eggRatio = maximumY / (static_cast<double>(vertexIt[sides >> 2].y) - vertexIt[0].y);
	for (unsigned iVertex = 1; iVertex < VertexCount; iVertex++) {
		vertexIt[iVertex].x = fi::shreg(vertexIt[iVertex].x, vertexIt[0].x, eggRatio);
		vertexIt[iVertex].y = fi::shreg(vertexIt[iVertex].y, vertexIt[0].y, eggRatio);
	}
}

void form::duzig(unsigned vertices) {
	if (vertices < 3) {
		vertices = 3;
	}
	if (vertices > 100) {
		vertices = 100;
	}
	auto newForm        = FRMHED{};
	newForm.vertexIndex = thred::adflt(vertices);
	newForm.vertexCount = vertices;
	newForm.attribute   = gsl::narrow<unsigned char>(ActiveLayer << 1);
	FormList->push_back(newForm);
	SelectedForm        = &(FormList->back());
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::fvars(ClosestFormToCursor);
	thred::px2stch();
	const auto offset   = fPOINT{ UnzoomedRect.x / 6.0, UnzoomedRect.y / (6.0 * vertices) };
	auto       vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < vertices; iVertex++) {
		vertexIt[iVertex] = SelectedPoint;
		SelectedPoint.y -= offset.y;
		if ((iVertex & 1) != 0u) {
			SelectedPoint.x += offset.x;
		}
		else {
			SelectedPoint.x -= offset.x;
		}
	}
	SelectedForm->type = FRMLINE;
	// ToDo do I need to assign again?
	ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	form::frmout(ClosestFormToCursor);
	FormMoveDelta.x = FormMoveDelta.y = 0;
	NewFormVertexCount                = vertices + 1;
	StateMap.set(StateFlag::POLIMOV);
	form::setmfrm();
	StateMap.set(StateFlag::SHOFRM);
	form::mdufrm();
}

void form::fliph() {
	form::fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		thred::savdo();
		const auto midpoint
		    = (static_cast<double>(SelectedVerticesRect.right) - SelectedVerticesRect.left) * 0.5 + SelectedVerticesRect.left;
		auto currentVertex = SelectedFormVertices.start;
		auto vertexIt      = FormVertices->begin() + CurrentVertexIndex;
		for (auto iVertex = 0u; iVertex <= SelectedFormVertices.vertexCount; iVertex++) {
			vertexIt[currentVertex].x = midpoint + midpoint - vertexIt[currentVertex].x;
			currentVertex             = form::pdir(currentVertex);
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		const auto midpoint = (AllItemsRect.right - AllItemsRect.left) / 2 + AllItemsRect.left;
		for (auto & FormVertice : *FormVertices) {
			FormVertice.x = midpoint + midpoint - FormVertice.x;
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
		}
		for (auto & iForm : *FormList) {
			auto& rect = iForm.rectangle;
			rect.left  = midpoint + midpoint - rect.left;
			rect.right = midpoint + midpoint - rect.right;
		}
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (!SelectedFormList->empty()) {
		thred::savdo();
		auto formMap = boost::dynamic_bitset<>(FormList->size());

		auto rectangle = fRECTANGLE{};
		form::pxrct2stch(SelectedFormsRect, rectangle);
		const auto midpoint = (rectangle.right - rectangle.left) / 2 + rectangle.left;
		auto       vertexIt = FormVertices->begin() + CurrentVertexIndex;
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			formMap.set(ClosestFormToCursor);
			form::fvars(ClosestFormToCursor);
			for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
				vertexIt[iVertex].x = midpoint + midpoint - vertexIt[iVertex].x;
			}
			form::frmout(ClosestFormToCursor);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			const auto decodedForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			if (formMap.test(decodedForm) && ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u)) {
				StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			thred::savdo();
			const auto midpoint
			    = (SelectedForm->rectangle.right - SelectedForm->rectangle.left) / 2.0f + SelectedForm->rectangle.left;
			auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
			for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
				vertexIt[iVertex].x = midpoint + midpoint - vertexIt[iVertex].x;
			}
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor
				    && ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u)) {
					StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
				}
			}
			form::frmout(ClosestFormToCursor);
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				auto rectangle = fRECTANGLE{};
				thred::selRct(rectangle);
				const auto midpoint = (rectangle.right - rectangle.left) / 2 + rectangle.left;
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x = midpoint + midpoint - StitchBuffer[iStitch].x;
				}
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::internal::prpsbrd() {
	if (SelectedForm->vertexCount > 2) {
		clip::deleclp(ClosestFormToCursor);
		SelectedForm->edgeType = EDGEPROPSAT;
		if (UserFlagMap.test(UserFlag::DUND)) {
			SelectedForm->edgeType |= EGUND;
		}
		form::bsizpar();
		SelectedForm->borderSize  = BorderWidth;
		SelectedForm->edgeSpacing = LineSpacing;
		SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
		form::refilfn();
	}
}

void form::prpbrd(double borderStitchSpacing) {
	const auto savedSpacing = LineSpacing;

	if (displayText::filmsgs(FML_PRPS)) {
		return;
	}
	LineSpacing = borderStitchSpacing;
	if (!SelectedFormList->empty()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			form::fvars(ClosestFormToCursor);
			SelectedForm->borderSize = LineSpacing;
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				SelectedForm->attribute |= (SBLNT | FBLNT);
			}
			else {
				SelectedForm->attribute &= NOBLNT;
			}
			fi::prpsbrd();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT)) {
				SelectedForm->attribute |= (SBLNT | FBLNT);
			}
			else {
				SelectedForm->attribute &= NOBLNT;
			}
			fi::prpsbrd();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			thred::ritot(PCSHeader.stitchCount);
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
	SelectedForm  = &((*FormList)[ClosestFormToCursor]);
	auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
	for (auto iVertex = 0u; iVertex < (SelectedForm->vertexCount >> 1); iVertex++) {
		std::swap(vertexIt[iVertex], vertexIt[SelectedForm->vertexCount - iVertex - 1]);
	}
	form::refil();
}

void form::flpord() {
	auto iVertex  = 0u;
	auto iForward = 0u;
	auto start    = 0u;
	auto finish   = 0u;

	form::fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FPSEL)) {
		thred::savdo();
		start         = SelectedFormVertices.start;
		finish        = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % VertexCount;
		auto vertexIt = FormVertices->begin() + SelectedForm->vertexIndex;
		for (iVertex = 0; iVertex <= SelectedFormVertices.vertexCount >> 1; iVertex++) {
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
				const auto endStitch = ((GroupEndStitch - GroupStartStitch) >> 1) + 1;
				for (auto iStitch = 0u; iStitch < endStitch; iStitch++) {
					std::swap(StitchBuffer[iForward], StitchBuffer[GroupEndStitch - iStitch]);
					iForward++;
				}
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::internal::filsclp() {
	texture::deltx();
	form::fvars(ClosestFormToCursor);
	if (SelectedForm->type != SAT) {
		SelectedForm->wordParam = 0;
	}
	SelectedForm->type                    = SAT;
	SelectedForm->fillType                = CLPF;
	SelectedForm->angleOrClipData.clip    = clip::numclp();
	SelectedForm->lengthOrCount.clipCount = gsl::narrow<unsigned int>(ClipBuffer->size());
	auto offsetStart                      = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
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
						if (SelectedForm->type != FRMLINE) {
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

void form::internal::snpfn(const std::vector<unsigned>& xPoints, unsigned start, unsigned end, unsigned finish) {
	if (finish != start) {
		for (auto current = start; current < end; current++) {
			auto reference = xPoints[current];
			for (auto iPoint = current + 1; iPoint < finish; iPoint++) {
				auto       check = xPoints[iPoint];
				const auto CheckLength
				    = hypot(StitchBuffer[check].x - StitchBuffer[reference].x, StitchBuffer[check].y - StitchBuffer[reference].y);
				if (CheckLength < SnapLength) {
					StitchBuffer[check] = StitchBuffer[reference];
				}
			}
		}
	}
}

void form::internal::doTimeWindow(float rangeX, const std::vector<unsigned>& xPoints, const std::vector<unsigned>& xHistogram) {
	auto checkLength = gsl::narrow<unsigned int>(std::round(SnapLength * 2.0 + 1.0));

	auto timeWindow = CreateWindow(L"STATIC",
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
	const auto timeStep     = static_cast<double>(StitchWindowSize.x) / rangeX;
	auto       timePosition = 0.0;
	auto&      formLines    = *FormLines;
	formLines.clear();
	formLines.push_back(POINT{ 0, 0 });
	formLines.push_back(POINT{ 0, gsl::narrow<LONG>(ButtonHeight) });
	SelectObject(timeDC, UserPen[0]);
	for (auto iColumn = 1u; iColumn < rangeX - checkLength - 1; iColumn++) {
		snpfn(xPoints,
		      xHistogram[iColumn],
		      xHistogram[static_cast<size_t>(iColumn) + 1],
		      xHistogram[static_cast<size_t>(iColumn) + checkLength]);
		Polyline(timeDC, formLines.data(), 2);
		timePosition += timeStep;
		formLines[0].x = formLines[1].x = dToL(timePosition);
	}
	DestroyWindow(timeWindow);
}

void form::internal::snp(unsigned start, unsigned finish) {
	auto range = fPOINT{};

	thred::chkrng(range);
	auto xPoints = std::vector<unsigned>{};
	xPoints.resize(PCSHeader.stitchCount);
	auto xHistogram = std::vector<unsigned>{};
	xHistogram.resize(gsl::narrow<size_t>(std::round(range.x)) + 1);

	const auto attribute = (ClosestFormToCursor << 4) & FRMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			if (((StitchBuffer[iStitch].attribute & NOTFRM) == 0u) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				auto iColumn = gsl::narrow<unsigned>(std::floor(StitchBuffer[iStitch].x));
				xHistogram[iColumn]++;
			}
		}
	}
	else {
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			auto iColumn = gsl::narrow<unsigned>(std::floor(StitchBuffer[iStitch].x));
			xHistogram[iColumn]++;
		}
	}
	auto accumulator = 0u;
	auto endColumn   = gsl::narrow<unsigned int>(std::round(range.x));
	for (auto iColumn = 0u; iColumn < endColumn; iColumn++) {
		auto value          = xHistogram[iColumn];
		xHistogram[iColumn] = accumulator;
		accumulator += value;
	}
	xHistogram[endColumn] = accumulator;
	if (StateMap.test(StateFlag::FORMSEL)) {
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if (((StitchBuffer[iStitch].attribute & NOTFRM) == 0u) && (StitchBuffer[iStitch].attribute & FRMSK) == attribute) {
				auto iColumn = gsl::narrow<unsigned>(std::floor(StitchBuffer[iStitch].x));

				xPoints[xHistogram[iColumn]++] = iStitch;
			}
		}
	}
	else {
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			auto iColumn                   = gsl::narrow<unsigned>(std::floor(StitchBuffer[iStitch].x));
			xPoints[xHistogram[iColumn]++] = iStitch;
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
		fi::snp(0, PCSHeader.stitchCount);
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::dufcntr(dPOINT& center) {
	auto bigRect = (*FormList)[SelectedFormList->front()].rectangle;
	for (auto selectedForm : (*SelectedFormList)) {
		const auto formRect = (*FormList)[selectedForm].rectangle;
		if (formRect.left < bigRect.left) {
			bigRect.left = formRect.left;
		}
		if (formRect.right > bigRect.right) {
			bigRect.right = formRect.right;
		}
		if (formRect.bottom < bigRect.bottom) {
			bigRect.bottom = formRect.bottom;
		}
		if (formRect.top > bigRect.top) {
			bigRect.top = formRect.top;
		}
	}
	center.x = (static_cast<double>(bigRect.right) - bigRect.left) / 2.0 + bigRect.left;
	center.y = (static_cast<double>(bigRect.top) - bigRect.bottom) / 2.0 + bigRect.bottom;
}

dPOINT form::rotpar() {
	auto rotationCenter = dPOINT{};
	if (StateMap.test(StateFlag::FPSEL)) {
		rotationCenter.x = form::midl(SelectedVerticesRect.right, SelectedVerticesRect.left);
		rotationCenter.y = form::midl(SelectedVerticesRect.top, SelectedVerticesRect.bottom);
		return rotationCenter;
	}
	if (StateMap.test(StateFlag::BIGBOX)) {
		rotationCenter.x = form::midl(AllItemsRect.right, AllItemsRect.left);
		rotationCenter.y = form::midl(AllItemsRect.top, AllItemsRect.bottom);
		return rotationCenter;
	}
	if (!SelectedFormList->empty()) {
		if (StateMap.test(StateFlag::GMRK)) {
			rotationCenter.x = ZoomMarkPoint.x;
			rotationCenter.y = ZoomMarkPoint.y;
		}
		else {
			fi::dufcntr(rotationCenter);
		}
		StateMap.set(StateFlag::FRMSROT);
		return rotationCenter;
	}
	if (StateMap.test(StateFlag::FORMSEL)) {
		RotationRect = SelectedForm->rectangle;
		if (!StateMap.test(StateFlag::GMRK)) {
			rotationCenter.x = (static_cast<double>(RotationRect.right) - RotationRect.left) / 2.0 + RotationRect.left;
			rotationCenter.y = (static_cast<double>(RotationRect.top) - RotationRect.bottom) / 2.0 + RotationRect.bottom;
		}
		StateMap.set(StateFlag::FRMROT);
	}
	else {
		if (StateMap.test(StateFlag::GRPSEL)) {
			thred::rngadj();
			thred::selRct(RotationRect);
		}
	}
	if (StateMap.test(StateFlag::GMRK)) {
		rotationCenter = ZoomMarkPoint;
	}
	else {
		rotationCenter.x = (static_cast<double>(RotationRect.right) - RotationRect.left) / 2.0 + RotationRect.left;
		rotationCenter.y = (static_cast<double>(RotationRect.top) - RotationRect.bottom) / 2.0 + RotationRect.bottom;
	}
	return rotationCenter;
}

void form::internal::rotentr(double rotationAngle) {
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, IDS_ROTA);
	displayText::shoMsg(fmt::format(fmtStr, (rotationAngle / PI * 180)));
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

void form::internal::fnagain(double rotationAngle) {
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
	fi::fnagain(2 * PI - IniFile.rotationAngle);
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

void form::internal::adfrm(unsigned int iForm) {
	FormList->push_back((*FormList)[iForm]);
	FormIndex++;

	auto& formHeader = FormList->back();

	SelectedForm           = &((*FormList)[iForm]);
	ClosestFormToCursor    = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
	formHeader.vertexIndex = thred::adflt(SelectedForm->vertexCount);
	auto vertexIt          = FormVertices->begin() + SelectedForm->vertexIndex;
	std::copy(vertexIt, vertexIt + SelectedForm->vertexCount, FormVertices->begin() + formHeader.vertexIndex);
	formHeader.vertexCount = SelectedForm->vertexCount;
	if (SelectedForm->type == SAT && (SelectedForm->satinGuideCount != 0u)) {
		formHeader.satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);
		auto guideStart               = SatinGuides->begin() + SelectedForm->satinOrAngle.guide;
		auto guideEnd                 = guideStart + SelectedForm->satinGuideCount;
		auto guideDest                = SatinGuides->begin() + formHeader.satinOrAngle.guide;
		std::copy(guideStart, guideEnd, guideDest);
	}
	if (clip::iseclpx(iForm)) {
		formHeader.borderClipData = thred::adclp(SelectedForm->clipEntries);
		auto offsetStart          = ClipPoints->begin() + SelectedForm->borderClipData;
		auto destination          = ClipPoints->begin() + formHeader.borderClipData;
		std::copy(offsetStart, offsetStart + SelectedForm->clipEntries, destination);
	}
	if (clip::isclpx(iForm)) {
		formHeader.angleOrClipData.clip = thred::adclp(SelectedForm->lengthOrCount.clipCount);
		auto       sourceStart          = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
		auto       sourceEnd            = sourceStart + SelectedForm->lengthOrCount.clipCount;
		const auto destination          = ClipPoints->begin() + formHeader.angleOrClipData.clip;
		std::copy(sourceStart, sourceEnd, destination);
	}
}

void form::duprot(double rotationAngle) {
	thred::savdo();
	fi::adfrm(ClosestFormToCursor);
	const auto rotationCenter = form::rotpar();
	thred::rotfn(rotationAngle, rotationCenter);
	form::refil();
	StateMap.set(StateFlag::FORMSEL);
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::duprotfs(double rotationAngle) {
	const auto rotationCenter = form::rotpar();
	for (auto selectedForm : (*SelectedFormList)) {
		adfrm(selectedForm);
		thred::rotfn(rotationAngle, rotationCenter);
	}
	FormIndex = gsl::narrow<unsigned int>(FormList->size());
}

void form::internal::duprots(double rotationAngle, const dPOINT& rotationCenter) {
	auto destination = PCSHeader.stitchCount;

	thred::rngadj();

	for (auto source = GroupStartStitch; source <= GroupEndStitch; source++) {
		StitchBuffer[destination].x           = StitchBuffer[source].x;
		StitchBuffer[destination].y           = StitchBuffer[source].y;
		StitchBuffer[destination++].attribute = StitchBuffer[source].attribute & (~(FRMSK | TYPMSK));
	}
	ClosestPointIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount += gsl::narrow<unsigned short>(GroupEndStitch - GroupStartStitch);
	GroupStitchIndex = PCSHeader.stitchCount;
	PCSHeader.stitchCount++;
	thred::rngadj();
	thred::rotfn(rotationAngle, rotationCenter);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::cplayfn(unsigned int iForm, unsigned play) {
	FormList->push_back((*FormList)[iForm]);
	const auto lastForm = gsl::narrow<unsigned int>(FormList->size() - 1u);
	form::fvars(lastForm);
	SelectedForm->vertexIndex = thred::adflt(SelectedForm->vertexCount);
	auto vertexIt             = FormVertices->begin() + (*FormList)[iForm].vertexIndex;
	std::copy(vertexIt, vertexIt + VertexCount, FormVertices->begin() + SelectedForm->vertexIndex);
	satin::cpySat((*FormList)[iForm]);
	SelectedForm->clipEntries             = 0;
	SelectedForm->fillType                = 0;
	SelectedForm->lengthOrCount.clipCount = 0;
	SelectedForm->edgeType                = 0;
	SelectedForm->fillInfo.texture.index  = 0;
	SelectedForm->attribute               = (*FormList)[lastForm].attribute & NFRMLMSK;
	SelectedForm->attribute |= play;
	SelectedForm->extendedAttribute = 0;
	form::dusqr();
	FormIndex++;
}

void form::cpylayr(unsigned codedLayer) {
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
				auto       iCurrentStitch   = PCSHeader.stitchCount;
				for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
					StitchBuffer[iCurrentStitch].x           = StitchBuffer[iStitch].x;
					StitchBuffer[iCurrentStitch].y           = StitchBuffer[iStitch].y;
					StitchBuffer[iCurrentStitch++].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
				}
				PCSHeader.stitchCount = iCurrentStitch;
				thred::coltab();
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::movlayr(unsigned codedLayer) {
	const auto codedStitchLayer = codedLayer << (LAYSHFT - 1);

	if (!SelectedFormList->empty()) {
		thred::savdo();
		auto formMap = boost::dynamic_bitset<>(FormList->size());
		for (auto selectedForm : (*SelectedFormList)) {
			auto& formAttr = (*FormList)[selectedForm].attribute;
			formAttr       = gsl::narrow<unsigned char>((formAttr & NFRMLMSK) | codedLayer);
			formMap.set(selectedForm);
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & ALTYPMSK) != 0u) {
				const auto iCurrentForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
				if (formMap.test(iCurrentForm)) {
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
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
			auto& formAttr = (*FormList)[ClosestFormToCursor].attribute;
			formAttr       = gsl::narrow<unsigned char>((formAttr & NFRMLMSK) | codedLayer);
			StateMap.reset(StateFlag::FORMSEL);
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (((StitchBuffer[iStitch].attribute & ALTYPMSK) != 0u)
				    && ((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
				}
			}
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				thred::savdo();
				thred::rngadj();
				for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].attribute = StitchBuffer[iStitch].attribute & NLAYMSK | codedStitchLayer;
				}
				StateMap.set(StateFlag::RESTCH);
			}
		}
	}
}

void form::join() {
	const auto savedFormIndex = ClosestFormToCursor;

	StateMap.set(StateFlag::FRMSAM);
	if (FormList->size() > 1 && StateMap.test(StateFlag::FORMSEL) && form::closfrm()) {
		auto formIter   = FormList->begin() + ClosestFormToCursor;
		auto vertexList = std::vector<fPOINT>{};
		vertexList.resize(formIter->vertexCount);
		auto vertexIt = FormVertices->cbegin() + formIter->vertexIndex;
		for (auto iVertex = 0u; iVertex < formIter->vertexCount; iVertex++) {
			vertexList[iVertex]   = vertexIt[ClosestVertexToCursor];
			ClosestVertexToCursor = form::nxt(ClosestVertexToCursor);
		}
		StateMap.set(StateFlag::DELTO);
		thred::frmdel();
		if (savedFormIndex > ClosestFormToCursor) {
			ClosestFormToCursor = savedFormIndex - 1;
		}
		else {
			ClosestFormToCursor = savedFormIndex;
		}
		formIter                  = FormList->begin() + ClosestFormToCursor; // formIter possibly invalidated by frmdel
		const auto insertionPoint = formIter->vertexIndex + formIter->vertexCount;
		form::fltspac(formIter->vertexCount, gsl::narrow<unsigned int>(vertexList.size()));
		auto dest = FormVertices->begin() + insertionPoint;
		std::copy(vertexList.begin(), vertexList.end(), dest);
		SelectedForm = &((*FormList)[ClosestFormToCursor]);
		vertexIt     = FormVertices->cbegin() + SelectedForm->vertexIndex;
		SelectedForm->vertexCount += gsl::narrow<decltype(SelectedForm->vertexCount)>(vertexList.size());
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
	for (ClosestFormToCursor = 0; ClosestFormToCursor < FormList->size(); ClosestFormToCursor++) {
		form::refilfn();
	}
	ClosestFormToCursor = savedFormIndex;
	form::fvars(ClosestFormToCursor);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

bool form::internal::notsel() noexcept {
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

void form::frmadj(unsigned int formIndex) {
	form::fvars(formIndex);
	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
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
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (((StitchBuffer[iStitch].attribute & ALTYPMSK) != 0u) && formMap.test((StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT)) {
			StitchBuffer[iStitch].x += FormMoveDelta.x;
			StitchBuffer[iStitch].y -= FormMoveDelta.y;
		}
	}
}

void form::internal::frmpnts(unsigned type) noexcept {
	auto       iStitch = 0u;
	const auto trg     = ((ClosestFormToCursor << 4) | type);

	while (iStitch < PCSHeader.stitchCount && (StitchBuffer[iStitch].attribute & (ALTYPMSK | FRMSK)) != trg) {
		iStitch++;
	}
	ClosestPointIndex = iStitch;
	if (PCSHeader.stitchCount > 0) {
		iStitch = PCSHeader.stitchCount - 1;
	}
	else {
		iStitch = 0;
	}
	while (iStitch > ClosestPointIndex && (StitchBuffer[iStitch].attribute & (ALTYPMSK | FRMSK)) != trg) {
		iStitch--;
	}
	GroupStitchIndex = iStitch;
}

void form::selfil(unsigned type) {
	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		auto& form = (*FormList)[ClosestFormToCursor];
		if (type == FRMFIL && (form.fillType == 0u)) {
			displayText::tabmsg(IDS_FSELF);
			return;
		}
		if (type == FRMBFIL && (form.edgeType == 0u)) {
			displayText::tabmsg(IDS_FSELB);
			return;
		}
		if (type == FRMAPFIL && (form.edgeType & NEGUND) != EDGEAPPL) {
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

bool form::notfstch(unsigned attribute) noexcept {
	if ((attribute & NOTFRM) != 0u) {
		return true;
	}
	return ((attribute & FRMSK) >> FRMSHFT) != ClosestFormToCursor;
}

void form::selalfil() {
	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		ClosestPointIndex = 0;
		while (ClosestPointIndex < PCSHeader.stitchCount && form::notfstch(StitchBuffer[ClosestPointIndex].attribute)) {
			ClosestPointIndex++;
		}
		if (ClosestPointIndex != PCSHeader.stitchCount) {
			if (ClosestPointIndex != 0u) {
				ClosestPointIndex--;
			}
			GroupStitchIndex = PCSHeader.stitchCount - 1;
			while (GroupStitchIndex > ClosestPointIndex && form::notfstch(StitchBuffer[GroupStitchIndex].attribute)) {
				GroupStitchIndex--;
			}
			StateMap.set(StateFlag::GRPSEL);
			StateMap.reset(StateFlag::FORMSEL);
			thred::rngadj();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			displayText::tabmsg(IDS_FSELM);
		}
	}
	else {
		displayText::tabmsg(IDS_SEL1FRM);
	}
}

bool form::frmrng(unsigned int iForm, RANGE& range) {
	if (PCSHeader.stitchCount != 0u) {
		range.start  = 0;
		range.finish = PCSHeader.stitchCount;
		auto& form   = (*FormList)[iForm];
		if ((form.fillType != 0u) || (form.edgeType != 0u)) {
			while (range.start < PCSHeader.stitchCount && notfstch(StitchBuffer[range.start].attribute)) {
				range.start++;
			}
			range.finish = PCSHeader.stitchCount - 1;
			while (range.finish > range.start && notfstch(StitchBuffer[range.finish].attribute)) {
				range.finish--;
			}
			return range.finish > range.start;
		}

		return false;
	}

	return false;
}

void form::internal::bholbrd() {
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->borderSize = BorderWidth;
	form::bsizpar();
	SelectedForm->edgeType    = EDGEBHOL;
	SelectedForm->edgeSpacing = LineSpacing;
	SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
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
			fi::bholbrd();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			fi::bholbrd();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			thred::ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
	LineSpacing = savedSpacing;
}

void form::fcntr() {
	if (!SelectedFormList->empty()) {
		thred::savdo();
		auto        firstForm     = SelectedFormList->front();
		const auto& firstRect     = (*FormList)[firstForm].rectangle;
		const auto  initialCenter = dPOINT{ (static_cast<double>(firstRect.right) - firstRect.left) / 2.0 + firstRect.left,
                                           (static_cast<double>(firstRect.top) - firstRect.bottom) / 2.0 + firstRect.bottom };
		for (auto selectedForm : (*SelectedFormList)) {
			const auto& formRect      = (*FormList)[selectedForm].rectangle;
			const auto  currentCenter = dPOINT{ (static_cast<double>(formRect.right) - formRect.left) / 2.0 + formRect.left,
                                               (static_cast<double>(formRect.top) - formRect.bottom) / 2.0 + formRect.bottom };
			const auto  delta         = dPOINT{ initialCenter.x - currentCenter.x, initialCenter.y - currentCenter.y };
			SelectedForm              = &((*FormList)[selectedForm]);
			auto vertexIt             = FormVertices->begin() + SelectedForm->vertexIndex;
			for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
				vertexIt[iVertex].x += delta.x;
				vertexIt[iVertex].y += delta.y;
			}
			form::frmout(selectedForm);
			auto codedForm = (selectedForm << FRMSHFT);
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				if ((StitchBuffer[iStitch].attribute & FRMSK) == codedForm && ((StitchBuffer[iStitch].attribute & NOTFRM) == 0u)) {
					StitchBuffer[iStitch].x += delta.x;
					StitchBuffer[iStitch].y += delta.y;
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

void form::internal::fspic() {
	clip::deleclp(ClosestFormToCursor);
	const auto clipSize          = gsl::narrow<unsigned int>(ClipBuffer->size());
	SelectedForm->edgeType       = EDGEPICOT;
	SelectedForm->clipEntries    = clipSize;
	SelectedForm->borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
	SelectedForm->borderSize     = ClipRectSize.cy;
	SelectedForm->edgeSpacing    = PicotSpacing;
	SelectedForm->borderColor    = gsl::narrow<unsigned char>(ActiveColor);
	form::bsizpar();
	form::savplen(ButtonholeCornerLength);
	auto offsetStart = ClipPoints->begin() + SelectedForm->borderClipData;
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	HorizontalLength2 = ClipRectSize.cy / 2;
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
						fi::fspic();
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::fspic();
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

bool form::internal::contsf(unsigned int formIndex) {
	ClosestFormToCursor = formIndex;
	form::fvars(formIndex);
	if (SelectedForm->vertexCount > 4) {
		clip::delclps(ClosestFormToCursor);
		texture::deltx();
		form::chkcont();
		SelectedForm->fillSpacing = LineSpacing;
		SelectedForm->fillColor   = gsl::narrow<unsigned char>(ActiveColor);
		form::fsizpar();
		SelectedForm->attribute |= (ActiveLayer << 1);
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

void form::internal::dupfn(double rotationAngle) {
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
	fi::dupfn(2 * PI - IniFile.rotationAngle);
}

void form::internal::shrnks() {
	clip::oclp(SelectedForm->borderClipData, SelectedForm->clipEntries);
	auto vertexIt = FormVertices->begin() + CurrentVertexIndex;
	for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
		auto&      thisVertex = vertexIt[iVertex];
		auto&      nextVertex = vertexIt[gsl::narrow_cast<size_t>(iVertex) + 1u];
		const auto vDelta     = dPOINT{ nextVertex.x - thisVertex.x, nextVertex.y - thisVertex.y };

		const auto length = hypot(vDelta.x, vDelta.y);
		const auto count  = std::round(length / ClipRectSize.cx);
		const auto ratio  = (ClipRectSize.cx * count + 0.004) / length;

		nextVertex.x = thisVertex.x + vDelta.x * ratio;
		nextVertex.y = thisVertex.y + vDelta.y * ratio;
	}
	const auto lastDelta     = dPOINT{ vertexIt[0].x - vertexIt[1].x, vertexIt[0].y - vertexIt[1].y };
	const auto length        = hypot(lastDelta.x, lastDelta.y);
	auto       rotationAngle = atan2(lastDelta.y, lastDelta.x);
	// ToDo - what does this loop do?
	// ToDo - move constant calculations out of the loop
	for (auto ine = 0u; ine < 5; ine++) {
		auto delta = dPOINT{ vertexIt[0].x - vertexIt[VertexCount - 1].x, vertexIt[0].y - vertexIt[VertexCount - 1].y };

		const auto deltaLength     = hypot(delta.x, delta.y);
		const auto count           = deltaLength / ClipRectSize.cx;
		const auto truncationDelta = deltaLength - count * ClipRectSize.cx;
		rotationAngle -= truncationDelta / length;
		delta.x       = cos(rotationAngle) * length;
		delta.y       = sin(rotationAngle) * length;
		vertexIt[0].x = vertexIt[1].x + delta.x;
		vertexIt[0].y = vertexIt[1].y + delta.y;
	}
	form::refil();
}

void form::shrnk() {
	form::fvars(ClosestFormToCursor);
	if (StateMap.test(StateFlag::FORMSEL) && SelectedForm->edgeType == EDGECLIP) {
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
                            std::vector<fPOINT>& destinationFormVertices,
                            std::vector<FRMHED>& destinationFormList,
                            unsigned int         formIndex,
                            unsigned int&        formRelocationIndex,
                            unsigned int&        formSourceIndex) {
	auto& destination = destinationFormList[formRelocationIndex];

	destinationFormList[formRelocationIndex++] = (*FormList)[formIndex];

	auto       vertexIt = FormVertices->begin() + destination.vertexIndex;
	const auto res = std::copy(vertexIt, vertexIt + destination.vertexCount, destinationFormVertices.begin() + formSourceIndex);
	destination.vertexIndex = formSourceIndex;
	formSourceIndex += destination.vertexCount;
	if (destination.satinGuideCount != 0u) {
		auto guideStart = SatinGuides->cbegin() + destination.satinOrAngle.guide;
		auto guideEnd   = guideStart + destination.satinGuideCount;
		tempGuides.insert(tempGuides.end(), guideStart, guideEnd);

		destination.satinOrAngle.guide
		    = gsl::narrow<decltype(destination.satinOrAngle.guide)>(tempGuides.size() - destination.satinGuideCount);
	}
	if (clip::iseclpx(formIndex)) {
		const auto sourceStart = ClipPoints->begin() + destination.borderClipData;
		auto       sourceEnd   = sourceStart + destination.clipEntries;
		tempClipPoints.insert(tempClipPoints.end(), sourceStart, sourceEnd);

		destination.borderClipData
		    = gsl::narrow<decltype(destination.borderClipData)>(tempClipPoints.size() - destination.clipEntries);
	}
	if (clip::isclpx(formIndex)) {
		auto sourceStart = ClipPoints->begin() + destination.angleOrClipData.clip;
		auto sourceEnd   = sourceStart + destination.lengthOrCount.clipCount;
		tempClipPoints.insert(tempClipPoints.end(), sourceStart, sourceEnd);

		destination.angleOrClipData.clip = gsl::narrow<decltype(destination.angleOrClipData.clip)>(
		    tempClipPoints.size() - destination.lengthOrCount.clipCount);
	}
}

void form::internal::stchfrm(unsigned formIndex, unsigned* attribute) noexcept {
	if (attribute != nullptr) {
		*attribute &= NFRMSK;
		*attribute |= formIndex << FRMSHFT;
	}
}

void form::frmnumfn(unsigned newFormIndex) {
	auto start  = 0u;
	auto finish = 0u;

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
		auto formRelocationIndex = 0u;
		auto sourceForm          = 0u;

		auto tempFormList = std::vector<FRMHED>{};
		tempFormList.resize(FormList->size());
		auto tempFormVertices = std::vector<fPOINT>{};
		tempFormVertices.resize(FormVertices->size());
		auto tempGuides = std::vector<SATCON>{};
		tempGuides.reserve(SatinGuides->size());
		auto tempClipPoints = std::vector<fPOINT>{};
		tempClipPoints.reserve(ClipPoints->size());

		auto formSourceIndex = 0u;
		for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
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
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			if ((StitchBuffer[iStitch].attribute & SRTYPMSK) != 0u) {
				const auto decodedFormIndex = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
				if (decodedFormIndex == ClosestFormToCursor) {
					fi::stchfrm(newFormIndex, &StitchBuffer[iStitch].attribute);
				}
				else {
					if (decodedFormIndex >= start && decodedFormIndex <= finish) {
						if (newFormIndex < ClosestFormToCursor) {
							fi::stchfrm(decodedFormIndex + 1, &StitchBuffer[iStitch].attribute);
						}
						else {
							fi::stchfrm(decodedFormIndex - 1, &StitchBuffer[iStitch].attribute);
						}
					}
				}
			}
		}
		ClosestFormToCursor = newFormIndex;
		displayText::ritnum(STR_NUMFRM, ClosestFormToCursor);
	}
}

constexpr unsigned form::internal::duat(unsigned attribute) {
	const auto type = ((attribute >> TYPSHFT) + 1) & 3;
	const auto frm  = (attribute & FRMSK) >> 2;

	return type | frm;
}

void form::internal::srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, unsigned start, unsigned finish) {
	if (start != finish) {
		auto stitchHistogram = std::vector<unsigned>{};
		stitchHistogram.resize(gsl::narrow<size_t>(FormList->size()) << 2);
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++;
		}
		auto stitchAccumulator = start;
		for (auto iForm = 0u; iForm < FormList->size() << 2; iForm++) {
			auto value             = stitchHistogram[iForm];
			stitchHistogram[iForm] = stitchAccumulator;
			stitchAccumulator += value;
		}
		for (auto iStitch = start; iStitch < finish; iStitch++) {
			StitchBuffer[stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++] = tempStitchBuffer[iStitch];
		}
	}
}

void form::srtbyfrm() {
	auto colorHistogram = std::vector<unsigned>{};
	colorHistogram.resize(16);
	auto color = std::vector<unsigned>{};
	color.resize(16);

	if (!FormList->empty()) {
		thred::savdo();
		color[AppliqueColor] = 0u;
		for (auto iColor = 0u; iColor < 16; iColor++) {
			if (iColor != AppliqueColor) {
				color[iColor] = iColor + 1;
			}
		}
		auto tempStitchBuffer = std::vector<fPOINTATTR>{};
		tempStitchBuffer.resize(PCSHeader.stitchCount);
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			colorHistogram[color[StitchBuffer[iStitch].attribute & 0xf]]++;
		}
		auto colorAccumulator = 0u;
		for (auto iColor = 0u; iColor < 16; iColor++) {
			auto value             = colorHistogram[iColor];
			colorHistogram[iColor] = colorAccumulator;
			colorAccumulator += value;
		}
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			tempStitchBuffer[colorHistogram[color[StitchBuffer[iStitch].attribute & 0xf]]++] = StitchBuffer[iStitch];
		}
		fi::srtf(tempStitchBuffer, 0, colorHistogram[0]);
		for (auto iColor = 0u; iColor < 15; iColor++) {
			fi::srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[static_cast<size_t>(iColor) + 1]);
		}
	}
	else {
		xt::srtcol();
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::cntrx() {
	auto markCenter = dPOINT{};
	bool flag       = false;

	if (StateMap.test(StateFlag::GMRK)) {
		markCenter = ZoomMarkPoint;
	}
	else {
		markCenter.x = UnzoomedRect.x / 2.0;
		markCenter.y = UnzoomedRect.y / 2.0;
	}
	if (!SelectedFormList->empty()) {
		flag = true;
		thred::savdo();
		auto selectedCenter = dPOINT{};
		fi::dufcntr(selectedCenter);
		FormMoveDelta.x = markCenter.x - selectedCenter.x;
		FormMoveDelta.y = -markCenter.y + selectedCenter.y;
		if (StateMap.test(StateFlag::CNTRV)) {
			FormMoveDelta.y = 0;
		}
		if (StateMap.test(StateFlag::CNTRH)) {
			FormMoveDelta.x = 0;
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
			const auto& formRect = (*FormList)[ClosestFormToCursor].rectangle;

			const auto selectedCenter = dPOINT{ (static_cast<double>(formRect.right) - formRect.left) / 2.0 + formRect.left,
				                                (static_cast<double>(formRect.top) - formRect.bottom) / 2.0 + formRect.bottom };
			FormMoveDelta.x           = markCenter.x - selectedCenter.x;
			FormMoveDelta.y           = -markCenter.y + selectedCenter.y;
			if (StateMap.test(StateFlag::CNTRV)) {
				FormMoveDelta.y = 0;
			}
			if (StateMap.test(StateFlag::CNTRH)) {
				FormMoveDelta.x = 0;
			}
			frmadj(ClosestFormToCursor);
			for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
				if (((StitchBuffer[iStitch].attribute & ALTYPMSK) != 0u)
				    && (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
					StitchBuffer[iStitch].x += FormMoveDelta.x;
					StitchBuffer[iStitch].y -= FormMoveDelta.y;
				}
			}
		}
		else {
			if (StateMap.test(StateFlag::GRPSEL)) {
				flag = true;
				thred::savdo();
				thred::rngadj();
				auto groupRect = fRECTANGLE{ StitchBuffer[GroupStartStitch].y,
					                         StitchBuffer[GroupStartStitch].x,
					                         StitchBuffer[GroupStartStitch].x,
					                         StitchBuffer[GroupStartStitch].y };
				for (auto iStitch = GroupStartStitch + 1; iStitch <= GroupEndStitch; iStitch++) {
					if (StitchBuffer[iStitch].x < groupRect.left) {
						groupRect.left = StitchBuffer[iStitch].x;
					}
					if (StitchBuffer[iStitch].x > groupRect.right) {
						groupRect.right = StitchBuffer[iStitch].x;
					}
					if (StitchBuffer[iStitch].y < groupRect.bottom) {
						groupRect.bottom = StitchBuffer[iStitch].y;
					}
					if (StitchBuffer[iStitch].y > groupRect.top) {
						groupRect.top = StitchBuffer[iStitch].y;
					}
				}
				const auto selectedCenter
				    = dPOINT{ (static_cast<double>(groupRect.right) - groupRect.left) / 2.0 + groupRect.left,
					          (static_cast<double>(groupRect.top) - groupRect.bottom) / 2.0 + groupRect.bottom };
				FormMoveDelta.x = markCenter.x - selectedCenter.x;
				FormMoveDelta.y = -markCenter.y + selectedCenter.y;
				if (StateMap.test(StateFlag::CNTRV)) {
					FormMoveDelta.y = 0;
				}
				if (StateMap.test(StateFlag::CNTRH)) {
					FormMoveDelta.x = 0;
				}
				for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
					StitchBuffer[iStitch].x += FormMoveDelta.x;
					StitchBuffer[iStitch].y -= FormMoveDelta.y;
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
	const auto itemCenter = dPOINT{ (static_cast<double>(AllItemsRect.right) - AllItemsRect.left) / 2.0 + AllItemsRect.left,
		                            (static_cast<double>(AllItemsRect.top) - AllItemsRect.bottom) / 2.0 + AllItemsRect.bottom };
	const auto hoopCenter = dPOINT{ UnzoomedRect.x / 2.0, UnzoomedRect.y / 2.0 };
	const auto delta      = dPOINT{ hoopCenter.x - itemCenter.x, hoopCenter.y - itemCenter.y };
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		StitchBuffer[iStitch].x += delta.x;
		StitchBuffer[iStitch].y += delta.y;
	}
	for (auto & FormVertice : *FormVertices) {
		FormVertice.x += delta.x;
		FormVertice.y += delta.y;
	}
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		form::frmout(iForm);
	}
	xt::setfchk();
	StateMap.set(StateFlag::RESTCH);
}

void form::internal::bean(unsigned start, unsigned finish) noexcept {
	auto iSourceStitch = start;
	auto iCopyStitch   = MAXITEMS;

	BeanCount = 0;
	thred::mvstch(iCopyStitch++, iSourceStitch);
	if (StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x
	    || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) {
		thred::mvstch(iCopyStitch++, iSourceStitch + 1);
		thred::mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	thred::mvstch(iCopyStitch++, iSourceStitch);
	if (StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x
	    || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y) {
		thred::mvstch(iCopyStitch++, iSourceStitch + 1);
		thred::mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < finish - 2) {
		thred::mvstch(iCopyStitch++, iSourceStitch);
		if ((StitchBuffer[iSourceStitch + 2].x != StitchBuffer[iSourceStitch].x
		     || StitchBuffer[iSourceStitch + 2].y != StitchBuffer[iSourceStitch].y)
		    && (StitchBuffer[iSourceStitch - 2].x != StitchBuffer[iSourceStitch].x
		        || StitchBuffer[iSourceStitch - 2].y != StitchBuffer[iSourceStitch].y)) {
			thred::mvstch(iCopyStitch++, iSourceStitch + 1);
			thred::mvstch(iCopyStitch++, iSourceStitch);
			BeanCount += 2;
		}
		iSourceStitch++;
	}
	thred::mvstch(iCopyStitch++, iSourceStitch);
	if ((StitchBuffer[iSourceStitch - 2].x != StitchBuffer[iSourceStitch].x
	     || StitchBuffer[iSourceStitch - 2].y != StitchBuffer[iSourceStitch].y)) {
		thred::mvstch(iCopyStitch++, iSourceStitch + 1);
		thred::mvstch(iCopyStitch++, iSourceStitch);
		BeanCount += 2;
	}
	iSourceStitch++;
	while (iSourceStitch < PCSHeader.stitchCount) {
		thred::mvstch(iCopyStitch++, iSourceStitch++);
	}
	auto iOutputStitch = start;
	for (iSourceStitch = MAXITEMS; iSourceStitch < iCopyStitch; iSourceStitch++) {
		thred::mvstch(iOutputStitch++, iSourceStitch);
	}
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(iOutputStitch);
}

void form::dubean() {
	if (PCSHeader.stitchCount != 0u) {
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
			fi::bean(0, PCSHeader.stitchCount - 1);
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void form::internal::unbean(unsigned start, unsigned finish) {
	auto iSource = start;
	auto iCopy   = MAXITEMS;

	BeanCount = 0;
	for (iSource = start; iSource <= finish; iSource++) {
		thred::mvstch(iCopy++, iSource);
		if (StitchBuffer[iSource].x == StitchBuffer[iSource + 2].x && StitchBuffer[iSource].y == StitchBuffer[iSource + 2].y) {
			iSource += 2;
			BeanCount += 2;
		}
	}
	if (BeanCount != 0u) {
		BeanCount -= 2;
	}
	if (iSource > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
		iSource = PCSHeader.stitchCount - 1;
	}
	while (iSource < PCSHeader.stitchCount) {
		thred::mvstch(iCopy++, iSource++);
	}
	thred::mvstchs(start, MAXITEMS, iCopy);
	PCSHeader.stitchCount = gsl::narrow<unsigned short>(start + iCopy);
}

void form::debean() {
	thred::savdo();
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		fi::unbean(GroupStartStitch, GroupEndStitch);
		if (ClosestPointIndex > GroupStitchIndex) {
			ClosestPointIndex -= BeanCount;
		}
		else {
			GroupStitchIndex -= BeanCount;
		}
		if (ClosestPointIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			ClosestPointIndex = PCSHeader.stitchCount - 1;
		}
		if (GroupStitchIndex > gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
			GroupStitchIndex = PCSHeader.stitchCount - 1;
		}
		thred::grpAdj();
	}
	else {
		fi::unbean(0, PCSHeader.stitchCount - 1);
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void form::clpspac(const unsigned insertPoint, unsigned int count) {
	auto sourceStart = ClipPoints->begin() + insertPoint;
	auto sourceEnd   = sourceStart + count;
	ClipPoints->insert(sourceStart, sourceStart, sourceEnd);
}

void form::stchadj() {
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		auto       high = StitchBuffer[iStitch].attribute & FRMSK;
		const auto low  = high >> FRMSHFT;
		if (low > ClosestFormToCursor) {
			StitchBuffer[iStitch].attribute &= NFRMSK;
			high += 1 << FRMSHFT;
			StitchBuffer[iStitch].attribute |= high;
		}
	}
	form::refilfn();
	const auto low = ClosestFormToCursor << FRMSHFT;
	for (auto iStitch = PCSHeader.stitchCount; iStitch != 0; iStitch--) {
		if ((StitchBuffer[iStitch - 1].attribute & FRMSK) == low) {
			break;
		}
	}
	ClosestFormToCursor++;
	form::refilfn();
	StateMap.reset(StateFlag::FRMPSEL);
}

bool form::internal::spltlin() {
	if (ClosestVertexToCursor < 2 || SelectedForm->vertexCount - ClosestVertexToCursor < 2) {
		return false;
	}
	auto& srcForm = (*FormList)[ClosestFormToCursor];
	auto  it      = FormList->begin() + ClosestFormToCursor;
	it            = FormList->insert(it, srcForm);
	FormIndex++;
	srcForm                   = (*FormList)[ClosestFormToCursor];
	auto& dstForm             = (*FormList)[gsl::narrow_cast<size_t>(ClosestFormToCursor) + 1u];

	SelectedForm->vertexCount = ClosestVertexToCursor;
	dstForm.vertexCount -= ClosestVertexToCursor;
	dstForm.vertexIndex = srcForm.vertexIndex + ClosestVertexToCursor;
	form::frmout(ClosestFormToCursor);
	form::frmout(ClosestFormToCursor + 1);
	if (clip::iseclp(ClosestFormToCursor)) {
		form::clpspac(srcForm.borderClipData, srcForm.clipEntries);
		const auto maxForm = FormList->size();
		for (auto iForm = ClosestFormToCursor + 1; iForm < maxForm; iForm++) {
			(*FormList)[iForm].borderClipData += SelectedForm->clipEntries;
		}
	}
	form::stchadj();
	return true;
}

void form::spltfrm() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		if (SelectedForm->type == SAT) {
			if (SelectedForm->satinGuideCount != 0u) {
				/*
				// ToDo - Not sure why the fill, border and stitches are being deleted. Commented out of now
				thred::delfstchs();
				SelectedForm->fillType = 0;
				SelectedForm->edgeType = 0;
				*/
				auto guideIt = SatinGuides->begin() + SelectedForm->satinOrAngle.guide;
				for (ActivePointIndex = 0; ActivePointIndex < SelectedForm->satinGuideCount; ActivePointIndex++) {
					if (guideIt[ActivePointIndex].start == ClosestVertexToCursor
					    || guideIt[ActivePointIndex].finish == ClosestVertexToCursor) {
						satin::spltsat(guideIt[ActivePointIndex]);
						return;
					}
				}
				displayText::spltmsg();
			}
			else {
				displayText::spltmsg();
			}
		}
		else {
			if (SelectedForm->type == FRMLINE) {
				if (fi::spltlin()) {
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					displayText::tabmsg(IDS_FRM3);
				}
				return;
			}
			displayText::spltmsg();
		}
	}
	displayText::spltmsg();
}

void form::stchs2frm() {
	if (StateMap.test(StateFlag::GRPSEL)) {
		thred::rngadj();
		if ((GroupEndStitch - GroupStartStitch) > 12000) {
			displayText::tabmsg(IDS_STMAX);
			return;
		}
		const auto vertexCount = GroupEndStitch - GroupStartStitch + 1;
		FormList->push_back(FRMHED{});
		SelectedForm              = &(FormList->back());
		SelectedForm->type        = FRMLINE;
		SelectedForm->vertexCount = vertexCount;
		SelectedForm->vertexIndex = thred::adflt(vertexCount);
		auto iVertex              = 0u;
		auto vertexIt             = FormVertices->begin() + SelectedForm->vertexIndex;
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; iStitch++) {
			vertexIt[iVertex].x   = StitchBuffer[iStitch].x;
			vertexIt[iVertex++].y = StitchBuffer[iStitch].y;
		}
		form::frmout(gsl::narrow<unsigned int>(FormList->size() - 1u));
		FormIndex++;
		if (ClosestPointIndex > GroupStitchIndex) {
			if (ClosestPointIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
				ClosestPointIndex++;
			}
		}
		else {
			if (GroupStitchIndex < gsl::narrow<unsigned>(PCSHeader.stitchCount) - 1) {
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
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	SelectedForm->lengthOrCount.clipCount = gsl::narrow<decltype(SelectedForm->lengthOrCount.clipCount)>(ClipBuffer->size());
	SelectedForm->angleOrClipData.clip    = clip::numclp();
	SelectedForm->wordParam               = IniFile.fillPhase;
	fi::makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	auto offsetStart          = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	SelectedForm->fillType  = VCLPF;
	SelectedForm->fillColor = gsl::narrow<unsigned char>(ActiveColor);
	SelectedForm->type      = FRMFPOLY;
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
						if (SelectedForm->type != FRMLINE) {
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
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	const auto clipSize                   = gsl::narrow<unsigned int>(ClipBuffer->size());
	SelectedForm->lengthOrCount.clipCount = clipSize;
	SelectedForm->angleOrClipData.clip    = clip::numclp();
	SelectedForm->lengthOrCount.clipCount = clipSize;
	SelectedForm->wordParam               = IniFile.fillPhase;
	fi::makpoli();
	SelectedForm->fillSpacing = IniFile.clipOffset;
	auto offsetStart          = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	SelectedForm->fillType  = HCLPF;
	SelectedForm->fillColor = gsl::narrow<unsigned char>(ActiveColor);
	SelectedForm->type      = FRMFPOLY;
	CurrentVertexIndex      = SelectedForm->vertexIndex;
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
						if (SelectedForm->type != FRMLINE) {
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

void form::angsclp() {
	form::fvars(ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	texture::deltx();
	SelectedForm->angleOrClipData.clip    = clip::numclp();
	SelectedForm->lengthOrCount.clipCount = gsl::narrow<decltype(SelectedForm->lengthOrCount.clipCount)>(ClipBuffer->size());
	SelectedForm->wordParam               = IniFile.fillPhase;
	fi::makpoli();
	SelectedForm->satinOrAngle.angle = IniFile.fillAngle;
	SelectedForm->fillSpacing        = IniFile.clipOffset;
	auto offsetStart                 = ClipPoints->begin() + SelectedForm->angleOrClipData.clip;
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	SelectedForm->fillType  = ANGCLPF;
	SelectedForm->fillColor = gsl::narrow<unsigned char>(ActiveColor);
	SelectedForm->type      = FRMFPOLY;
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
						form::fvars(ClosestFormToCursor);
						if (SelectedForm->type != FRMLINE) {
							angsclp();
						}
					}
					StateMap.reset(StateFlag::NOCLP);
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						SelectedForm = &((*FormList)[ClosestFormToCursor]);
						angsclp();
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

void form::dubsfil() {
	form::fvars(ClosestFormToCursor);
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->edgeType    = EDGEDOUBLE;
	SequenceIndex             = 0;
	SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
	form::bsizpar();
	fi::dubfn();
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
			dubsfil();
		}
		StateMap.set(StateFlag::INIT);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			dubsfil();
			thred::coltab();
			StateMap.set(StateFlag::INIT);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::col2frm() {
	auto colorChangedCount = 0u;

	if (!FormList->empty()) {
		const auto formColorPermutations = FormList->size() << 4; // total number of form and color combinations
		auto       fillColorHistogram    = std::vector<unsigned>{};
		fillColorHistogram.resize(formColorPermutations);
		auto borderColorHistogram = std::vector<unsigned>{};
		borderColorHistogram.resize(formColorPermutations);
		auto featherColorHistogram = std::vector<unsigned>{};
		featherColorHistogram.resize(formColorPermutations);
		auto underlayColorHistogram = std::vector<unsigned>{};
		underlayColorHistogram.resize(formColorPermutations);
		for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
			const auto formColorCode = StitchBuffer[iStitch].attribute & 0x3fffu;
			if ((StitchBuffer[iStitch].attribute & (WLKMSK | CWLKMSK | UNDMSK)) != 0u) {
				underlayColorHistogram[formColorCode]++;
			}
			else {
				if ((StitchBuffer[iStitch].attribute & FTHMSK) != 0u) {
					featherColorHistogram[formColorCode]++;
				}
				else {
					switch (StitchBuffer[iStitch].attribute & TYPMSK) {
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
		auto startColorOffset = 0u;
		auto endColorOffset   = 16u;
		for (auto & formIter : *FormList) {
				if (formIter.fillType != 0u) {
				auto count         = 0u;
				auto majorityColor = 0u;
				for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (fillColorHistogram[iColor] > count) {
						count         = fillColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (formIter.fillColor != majorityColor) {
					colorChangedCount++;
					formIter.fillColor = gsl::narrow<unsigned char>(majorityColor);
				}
				if (formIter.fillType == FTHF && ((formIter.extendedAttribute & AT_FTHBLND) != 0u)) {
					count = majorityColor = 0;
					for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
						if (featherColorHistogram[iColor] > count) {
							count         = fillColorHistogram[iColor];
							majorityColor = iColor;
						}
					}
					majorityColor &= 0xf;
					if (formIter.fillInfo.feather.color != majorityColor) {
						colorChangedCount++;
						formIter.fillInfo.feather.color = gsl::narrow<unsigned char>(majorityColor);
					}
				}
			}
			if (formIter.edgeType != 0u) {
				auto count         = 0u;
				auto majorityColor = 0u;
				for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (borderColorHistogram[iColor] > count) {
						count         = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (formIter.borderColor != majorityColor) {
					colorChangedCount++;
					formIter.borderColor = gsl::narrow<unsigned char>(majorityColor);
				}
			}
			if ((formIter.extendedAttribute & (AT_WALK | AT_CWLK | AT_UND)) != 0u) {
				auto count         = 0u;
				auto majorityColor = 0u;
				for (auto iColor = startColorOffset; iColor < endColorOffset; iColor++) {
					if (underlayColorHistogram[iColor] > count) {
						count         = borderColorHistogram[iColor];
						majorityColor = iColor;
					}
				}
				majorityColor &= 0xf;
				if (formIter.underlayColor != majorityColor) {
					colorChangedCount++;
					formIter.underlayColor = gsl::narrow<unsigned char>(majorityColor);
				}
			}
			startColorOffset += 16;
			endColorOffset += 16;
		}
	}
	auto fmtStr = std::wstring{};
	displayText::loadString(fmtStr, IDS_NCOLCHG);
	displayText::shoMsg(fmt::format(fmtStr, colorChangedCount));
}

void form::chan() {
	SelectedForm->borderColor        = gsl::narrow<unsigned char>(ActiveColor);
	SelectedForm->edgeSpacing        = IniFile.chainSpace;
	SelectedForm->borderSize         = BorderWidth;
	SelectedForm->edgeStitchLen      = IniFile.chainRatio;
	SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	SelectedForm->minBorderStitchLen = MinStitchLength;
	if (StateMap.test(StateFlag::LINCHN)) {
		SelectedForm->edgeType = EDGELCHAIN;
	}
	else {
		SelectedForm->edgeType = EDGEOCHAIN;
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
			thred::ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void form::crop() {
	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		thred::savdo();
		form::fvars(ClosestFormToCursor);
		auto iDestination = 0u;
		for (auto iSource = 0u; iSource < PCSHeader.stitchCount; iSource++) {
			if (form::cisin(StitchBuffer[iSource].x, StitchBuffer[iSource].y)) {
				StitchBuffer[iDestination++] = StitchBuffer[iSource];
			}
		}
		PCSHeader.stitchCount = gsl::narrow<unsigned short>(iDestination);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_FRM1MSG, IDS_CROP);
	}
}

void form::internal::fsclpx() {
	clip::deleclp(ClosestFormToCursor);
	const auto clipSize          = gsl::narrow<unsigned int>(ClipBuffer->size());
	SelectedForm->edgeType       = EDGECLIPX;
	SelectedForm->clipEntries    = clipSize;
	SelectedForm->borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
	SelectedForm->borderSize     = ClipRectSize.cy;
	SelectedForm->edgeSpacing    = ClipRectSize.cx;
	SelectedForm->borderColor    = gsl::narrow<unsigned char>(ActiveColor);
	form::bsizpar();
	auto offsetStart = ClipPoints->begin() + SelectedForm->borderClipData;
	for (auto& clip : *ClipBuffer) {
		*offsetStart = clip;
		offsetStart++;
	}
	clip::duxclp();
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
						fi::fsclpx();
					}
					StateMap.set(StateFlag::INIT);
					thred::coltab();
					StateMap.set(StateFlag::RESTCH);
				}
				else {
					if (StateMap.test(StateFlag::FORMSEL)) {
						fi::fsclpx();
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
	auto histogram = std::vector<unsigned>{};
	histogram.resize(FormList->size());

	if (PCSHeader.stitchCount != 0u) {
		thred::savdo();
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			const auto iForm = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			histogram[iForm]++;
		}
		auto totalStitches = 0u;
		for (auto& entry : histogram) {
			auto formStitchCount = entry;
			entry                = totalStitches;
			totalStitches += formStitchCount;
		}
		auto highStitchBuffer = std::vector<fPOINTATTR>{};
		highStitchBuffer.resize(PCSHeader.stitchCount);
		for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
			const auto iForm              = (StitchBuffer[iStitch].attribute & FRMSK) >> FRMSHFT;
			auto       iHighStitch        = histogram[iForm]++;
			highStitchBuffer[iHighStitch] = StitchBuffer[iStitch];
		}
		std::copy(
		    highStitchBuffer.cbegin(), highStitchBuffer.cend(), stdext::make_checked_array_iterator(StitchBuffer, MAXITEMS));
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}
