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
//#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
//#include "switches.h"
//#include "ExtendedBitSet.h"
//#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
//#include "formForms.h"
//#include "hlp.h"
//#include "texture.h"
#include "thred.h"
//#include "xt.h"
#include "satin.h"

namespace si = satin::internal;

uint32_t StartPoint; // starting formOrigin for a satin stitch guide-line

void satin::delsac(uint32_t formIndex) {
	auto& formList = *FormList;
	if (!SatinGuides->empty()) {
		if (formList[formIndex].type == SAT && (formList[formIndex].satinGuideCount != 0u)) {
			auto eraseStart = std::next(SatinGuides->cbegin(), formList[formIndex].satinOrAngle.guide);
			auto eraseEnd   = std::next(eraseStart, formList[formIndex].satinGuideCount);
			SatinGuides->erase(eraseStart, eraseEnd);
			for (auto iForm = formIndex + 1u; iForm < FormList->size(); iForm++) {
				if (formList[iForm].type == SAT && (formList[iForm].satinGuideCount != 0u)) {
					formList[iForm].satinOrAngle.guide -= formList[formIndex].satinGuideCount;
				}
			}
		}
	}
	formList[formIndex].satinGuideCount = 0;
}

void satin::internal::sacspac(uint32_t startGuide, uint32_t guideCount) {
	const auto val = SATCON {};
	auto       pos = std::next(SatinGuides->cbegin(), startGuide);
	SatinGuides->insert(pos, val);
	for (auto iForm = ClosestFormToCursor + 1u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		if (form.type == SAT) {
			form.satinOrAngle.guide += guideCount;
		}
	}
}

uint32_t satin::internal::nusac(uint32_t formIndex, uint32_t guideCount) {
	auto  guideIndex = 0u;
	auto& formList   = *FormList;

	for (auto iForm = 0u; iForm < formIndex; iForm++) {
		auto& form = formList[iForm];
		if (form.type == SAT) {
			guideIndex += form.satinGuideCount;
		}
	}
	si::sacspac(guideIndex, guideCount);
	return guideIndex;
}

void satin::spltsat(uint32_t guideIndex) {
	auto guideIt = std::next(SatinGuides->begin(), SelectedForm->satinOrAngle.guide);
	// We are adding two additional vertices when splitting the form
	auto vertexBuffer = std::vector<fPOINT> {};
	vertexBuffer.resize(wrap::toSize(VertexCount) + 2u);
	auto& formList = *FormList;

	auto& srcForm = formList[ClosestFormToCursor];
	auto  dest    = std::next(formList.cbegin(), ClosestFormToCursor);
	formList.insert(dest, srcForm);
	form::fvars(ClosestFormToCursor);
	const auto maxForm  = formList.size();
	auto       position = std::next(FormVertices->cbegin(), wrap::toSize(CurrentVertexIndex) + VertexCount);
	FormVertices->insert(position, 2, fPOINT {});
	for (auto iForm = ClosestFormToCursor + 2u; iForm < maxForm; iForm++) {
		formList[iForm].vertexIndex += 2;
	}
	auto       iOldVertex    = 0u;
	auto       currentGuide  = guideIt[guideIndex];
	const auto oldLastVertex = currentGuide.start + (VertexCount - currentGuide.finish) + 1u;
	auto       iNewVertex    = oldLastVertex + 1u;
	auto       vertexIt      = std::next(FormVertices->begin(), CurrentVertexIndex);
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		if (iVertex == currentGuide.start || iVertex == currentGuide.finish) {
			vertexBuffer[iOldVertex++] = vertexIt[iVertex];
			if (iVertex == currentGuide.start) {
				vertexBuffer[iNewVertex++] = vertexIt[iVertex];
			}
			else {
				vertexBuffer[oldLastVertex] = vertexIt[iVertex];
			}
		}
		else {
			if (iVertex < currentGuide.start) {
				vertexBuffer[iOldVertex++] = vertexIt[iVertex];
			}
			else {
				if (iVertex < currentGuide.finish) {
					vertexBuffer[iNewVertex++] = vertexIt[iVertex];
				}
				else {
					vertexBuffer[iOldVertex++] = vertexIt[iVertex];
				}
			}
		}
	}
	for (auto iVertex = 0u; iVertex < iNewVertex; iVertex++) {
		vertexIt[iVertex] = vertexBuffer[iVertex];
	}
	SelectedForm->vertexCount = iOldVertex;
	auto& nextForm            = formList[wrap::toSize(ClosestFormToCursor) + 1u];
	nextForm.vertexCount      = iNewVertex - iOldVertex;
	nextForm.vertexIndex      = CurrentVertexIndex + iOldVertex;
	form::frmout(ClosestFormToCursor);
	form::frmout(ClosestFormToCursor + 1u);
	auto iNewGuide = 1u;
	iNewGuide += currentGuide.start - currentGuide.finish;
	auto iGuide = 0u;
	for (iGuide = 0u; iGuide < guideIndex; iGuide++) {
		guideIt[iGuide].finish += iNewGuide;
	}
	if (SelectedForm->wordParam != 0u) {
		SelectedForm->wordParam = currentGuide.start;
	}
	iNewGuide = iGuide + 1u;
	while (iGuide < (SelectedForm->satinGuideCount)) {
		guideIt[iGuide].start -= (currentGuide.start - 1);
		guideIt[iGuide].finish -= (currentGuide.start - 1);
		iGuide++;
	}
	if (nextForm.wordParam != 0u) {
		nextForm.wordParam -= (currentGuide.start - 1);
	}
	const auto offset = SelectedForm->satinOrAngle.guide + iNewGuide - 1u;
	auto       pos    = std::next(SatinGuides->cbegin(), offset);
	SatinGuides->erase(pos);
	nextForm.satinOrAngle.guide   = SelectedForm->satinOrAngle.guide + guideIndex;
	nextForm.satinGuideCount      = SelectedForm->satinGuideCount - guideIndex - 1u;
	SelectedForm->satinGuideCount = guideIndex;
	for (auto iForm = ClosestFormToCursor + 2u; iForm < maxForm; iForm++) {
		auto& formIter = formList[iForm];
		formIter.satinOrAngle.guide--;
	}
	if (clip::iseclp(ClosestFormToCursor)) {
		form::clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (auto iForm = ClosestFormToCursor + 1u; iForm < maxForm; iForm++) {
			auto& formIter = formList[iForm];
			formIter.borderClipData += SelectedForm->clipEntries;
		}
	}
	form::stchadj();
}

void satin::internal::satclos() {
	const auto initialGuideCount = SelectedForm->satinGuideCount;
	auto       minimumLength     = 1e99;

	form::uninsf();
	thred::px2stch();
	auto vertexIt = std::next(FormVertices->cbegin(), SelectedForm->vertexIndex);
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		const auto deltaX = SelectedPoint.x - vertexIt[iVertex].x;
		const auto deltaY = SelectedPoint.y - vertexIt[iVertex].y;
		const auto length = hypot(deltaX, deltaY);
		if (length < minimumLength) {
			minimumLength         = length;
			ClosestVertexToCursor = iVertex;
		}
	}
	StateMap.reset(StateFlag::SATCNKT);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->fillType = CONTF;
		auto closestVertex     = ClosestVertexToCursor;
		if (StartPoint > closestVertex) {
			std::swap(closestVertex, StartPoint);
		}
		if (StartPoint == 0u) {
			StartPoint++;
		}
		if (StartPoint == SelectedForm->vertexCount - 2 && closestVertex == (SelectedForm->vertexCount) - 1) {
			StartPoint    = 1;
			closestVertex = SelectedForm->vertexCount - 2u;
		}
		if (closestVertex >= (SelectedForm->vertexCount) - 2u) {
			closestVertex = SelectedForm->vertexCount - 2u;
			if (StartPoint >= (SelectedForm->vertexCount) - 2u) {
				StartPoint = SelectedForm->vertexCount - 2u;
			}
		}
		if (closestVertex - StartPoint < 2u) {
			closestVertex = StartPoint + 2u;
			if (closestVertex > (SelectedForm->vertexCount) - 2) {
				closestVertex = closestVertex - SelectedForm->vertexCount - 2u;
				closestVertex -= closestVertex;
				StartPoint -= closestVertex;
			}
		}
		SelectedForm->angleOrClipData.guide.start  = StartPoint;
		SelectedForm->angleOrClipData.guide.finish = closestVertex;
	}
	else {
		auto closestVertex = StartPoint;

		if (ClosestVertexToCursor < closestVertex) {
			std::swap(ClosestVertexToCursor, closestVertex);
		}
		if (closestVertex == 0 && ClosestVertexToCursor == VertexCount - 1) {
			closestVertex         = VertexCount - 1u;
			ClosestVertexToCursor = VertexCount;
		}
		if (closestVertex == 1 && ClosestVertexToCursor == VertexCount) {
			closestVertex         = 0;
			ClosestVertexToCursor = 1;
		}
		if (ClosestVertexToCursor - closestVertex == 1) {
			if ((SelectedForm->attribute & FRMEND) != 0u) {
				SelectedForm->wordParam = closestVertex;
			}
			else {
				if (closestVertex != 0u) {
					form::rotfrm(closestVertex);
				}
				SelectedForm->attribute |= FRMEND;
			}
			satin::satadj();
		}
		else {
			if (CurrentFormGuidesCount != 0u) {
				si::sacspac(SelectedForm->satinOrAngle.guide + SelectedForm->satinGuideCount, 1);
				auto guideIt = std::next(SatinGuides->begin(), SelectedForm->satinOrAngle.guide);

				guideIt[SelectedForm->satinGuideCount].start    = closestVertex;
				guideIt[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satin::satadj();
			}
			else {
				SelectedForm->satinOrAngle.guide = si::nusac(ClosestFormToCursor, 1);

				auto guideIt = std::next(SatinGuides->begin(), SelectedForm->satinOrAngle.guide);

				guideIt[initialGuideCount].start  = closestVertex;
				guideIt[initialGuideCount].finish = ClosestVertexToCursor;
				SelectedForm->satinGuideCount     = 1;
			}
		}
	}
}

void satin::satknkt() {
	si::satclos();
	form::fvars(ClosestFormToCursor);
	form::refil();
	StateMap.set(StateFlag::RESTCH);
}

bool satin::internal::satselfn() {
	auto minimumLength = 1e99;

	thred::px2stch();
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto&      form      = (*FormList)[iForm];
		const auto layerCode = gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(form.attribute & FRMLMSK) >> 1u);
		if ((ActiveLayer == 0u) || (layerCode == 0u) || layerCode == ActiveLayer) {
			auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
			for (auto iVertex = 0u; iVertex < form.vertexCount; iVertex++) {
				const auto deltaX = SelectedPoint.x - vertexIt[iVertex].x;
				const auto deltaY = SelectedPoint.y - vertexIt[iVertex].y;
				const auto length = hypot(deltaX, deltaY);
				if (length < minimumLength) {
					minimumLength         = length;
					ClosestFormToCursor   = iForm;
					ClosestVertexToCursor = iVertex;
				}
			}
		}
	}
	return minimumLength < CLOSENUF;
}

void satin::satsel() {
	if (si::satselfn()) {
		form::fvars(ClosestFormToCursor);
		thred::duzrat();
		StartPoint    = ClosestVertexToCursor;
		auto vertexIt = std::next(FormVertices->cbegin(), SelectedForm->vertexIndex);
		form::sfCor2px(vertexIt[ClosestVertexToCursor], (*FormLines)[0]);
		StateMap.reset(StateFlag::SHOCON);
		StateMap.set(StateFlag::SATCNKT);
		if (SelectedForm->type == FRMFPOLY) {
			SelectedForm->type = SAT;
		}
	}
}

void satin::internal::satcpy(const std::vector<SATCON>& source, uint32_t size) {
	auto eraseStart = std::next(SatinGuides->cbegin(), CurrentFormGuides);
	auto eraseEnd   = std::next(eraseStart, (CurrentFormGuidesCount - size));
	SatinGuides->erase(eraseStart, eraseEnd);
	auto guideIt = std::next(SatinGuides->begin(), CurrentFormGuides); // iterator may be invalidated by erase
	std::copy(source.cbegin(), source.cend(), guideIt);
}

void satin::satadj() {
	form::fvars(ClosestFormToCursor);

	auto interiorGuides = std::vector<SATCON> {};
	interiorGuides.reserve(CurrentFormGuidesCount);
	const auto savedGuideCount = SelectedForm->satinGuideCount;
	auto       satinMap        = ExtendedBitSet<>(VertexCount);

	// ensure all guide endpoints are on valid vertices
	auto guideIt = std::next(SatinGuides->begin(), CurrentFormGuides);
	for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
		const auto endCount = (VertexCount - 1);
		if (guideIt[iGuide].finish > endCount) {
			guideIt[iGuide].finish = endCount;
		}
		if (guideIt[iGuide].start > endCount) {
			guideIt[iGuide].start = endCount;
		}
	}

	// remove any guides of 0 length
	for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
		if (guideIt[iSource].start != guideIt[iSource].finish) {
			interiorGuides.push_back(guideIt[iSource]);
		}
	}
	auto iDestination = wrap::toUnsigned(interiorGuides.size());
	if (CurrentFormGuidesCount > iDestination) {
		OutputDebugString(fmt::format(L"Removed {} zero distance guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
		si::satcpy(interiorGuides, iDestination);
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
	}
	if ((SatinEndGuide != 0u) || ((SelectedForm->attribute & FRMEND) != 0u)) {
		// there are end guides so set the satinMap for the next step
		satinMap.reset();
		if ((SelectedForm->attribute & FRMEND) != 0u) {
			satinMap.set(0);
			satinMap.set(1);
		}
		if (SatinEndGuide != 0u) {
			satinMap.set(SatinEndGuide);
			satinMap.set(wrap::toSize(SatinEndGuide) + 1u);
		}
		// check to see if any of the current guides are end guides and add to interiorGuides if not
		interiorGuides.clear();
		guideIt = std::next(SatinGuides->begin(), CurrentFormGuides);
		for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
			if (!satinMap.test(guideIt[iSource].start) && !satinMap.test(guideIt[iSource].finish)) {
				interiorGuides.push_back(guideIt[iSource]);
			}
		}
		iDestination = gsl::narrow<decltype(iDestination)>(interiorGuides.size());
		if (CurrentFormGuidesCount > iDestination) {
			OutputDebugString(fmt::format(L"Removed {} end guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
			si::satcpy(interiorGuides, iDestination);
			CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
		}
		// remove any guides that start after the end guide
		if (SatinEndGuide != 0u) {
			interiorGuides.clear();
			guideIt = std::next(SatinGuides->begin(), CurrentFormGuides); // iterator may be invalidated by erase
			for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
				if (guideIt[iSource].start < SatinEndGuide) {
					interiorGuides.push_back(guideIt[iSource]);
				}
			}
			iDestination = gsl::narrow<decltype(iDestination)>(interiorGuides.size());
			if (CurrentFormGuidesCount > iDestination) {
				OutputDebugString(fmt::format(L"Removed {} reversed guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
				si::satcpy(interiorGuides, iDestination);
				SelectedForm->satinGuideCount = iDestination;
				CurrentFormGuidesCount        = iDestination;
			}
		}
	}

	if (CurrentFormGuidesCount != 0u && SatinEndGuide != 0) {
		satinMap.reset();
		for (auto iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			auto iForward = guideIt[iGuide].start;
			if (iForward > SatinEndGuide - 1) {
				iForward = SatinEndGuide - 1u;
			}
			if (satinMap.test_set(iForward)) {
				auto iReverse = iForward;
				if (iReverse != 0u) {
					iReverse--;
				}
				while (satinMap.test(iForward) && (iForward < ((SatinEndGuide)-1))) {
					iForward++;
				}
				while ((iReverse != 0u) && (satinMap.test(iReverse))) {
					iReverse--;
				}
				if (satinMap.test(iForward) && satinMap.test(iReverse)) {
					break;
				}
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - guideIt[iGuide].start > guideIt[iGuide].start - iReverse) {
						satinMap.set(iReverse);
					}
					else {
						satinMap.set(iForward);
					}
				}
				else {
					if (!satinMap.test(iForward)) {
						satinMap.set(iReverse);
					}
					else {
						satinMap.set(iForward);
					}
				}
			}
		}
		auto iGuide  = 0u;
		auto iVertex = 0u;
		do {
			iVertex = wrap::toUnsigned(satinMap.getFirst());
			if (iVertex < VertexCount) {
				guideIt[iGuide++].start = iVertex;
			}
		} while (iVertex < VertexCount);

		CurrentFormGuidesCount = SelectedForm->satinGuideCount = (iGuide);
		satinMap.reset();
		// Todo - are iForward and iReverse appropriate variable names below?
		for (iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			auto iForward = guideIt[iGuide].finish;
			auto iReverse = iForward;
			if (iForward > VertexCount - 1) {
				iForward = VertexCount - 1u;
			}
			if (satinMap.test_set(iForward)) {
				if (iForward < VertexCount - 1) {
					iForward++;
				}
				if (iReverse > (SatinEndGuide) + 1u) {
					iReverse--;
				}
				while (satinMap.test(iForward) && iForward < VertexCount - 1) {
					iForward++;
				}
				while (iReverse > (SatinEndGuide)-1 && (satinMap.test(iReverse))) {
					iReverse--;
				}
				if (satinMap.test(iForward) && satinMap.test(iReverse)) {
					break;
				}
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - guideIt[iGuide].finish > guideIt[iGuide].finish - iReverse) {
						satinMap.set(iReverse);
					}
					else {
						satinMap.set(iForward);
					}
				}
				else {
					if (!satinMap.test(iForward)) {
						satinMap.set(iForward);
					}
					else {
						satinMap.set(iReverse);
					}
				}
			}
		}
		iGuide        = 0u;
		auto iReverse = 0u;
		do {
			iReverse = wrap::toUnsigned(satinMap.getLast());
			if (iReverse < VertexCount) {
				guideIt[iGuide++].finish = iReverse;
			}
		} while (iReverse < VertexCount);
		if (iGuide < CurrentFormGuidesCount) {
			iGuide = CurrentFormGuidesCount;
		}
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iGuide;
		if (SatinEndGuide != 0u) {
			const auto end0 = (VertexCount - SatinEndGuide - 2);
			const auto end1 = (SatinEndGuide - 2);
			if (CurrentFormGuidesCount > end0) {
				CurrentFormGuidesCount = end0;
			}
			if (CurrentFormGuidesCount > end1) {
				CurrentFormGuidesCount = end1;
			}
			SelectedForm->satinGuideCount = CurrentFormGuidesCount;
		}
	}
	if (SelectedForm->satinGuideCount < savedGuideCount) {
		const auto iGuide = savedGuideCount - CurrentFormGuidesCount;
		OutputDebugString(fmt::format(L"Guides adjusted by {}, so updating forms\n", iGuide).c_str());
		for (auto iForm = ClosestFormToCursor + 1u; iForm < FormList->size(); iForm++) {
			auto& form = (*FormList)[iForm];
			if (form.type == SAT) {
				form.satinOrAngle.guide -= iGuide;
			}
		}
	}
}

void satin::delcon(uint32_t GuideIndex) {
	const auto offset = SelectedForm->satinOrAngle.guide + GuideIndex;
	auto       guide  = std::next(SatinGuides->cbegin(), offset);

	SatinGuides->erase(guide);
	for (auto iForm = ClosestFormToCursor + 1u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		if (form.type == SAT && (form.satinGuideCount != 0u)) {
			form.satinOrAngle.guide--;
		}
	}
	if (ClosestVertexToCursor < SatinEndGuide) {
		SatinEndGuide--;
	}
	SelectedForm->satinGuideCount--;
	CurrentFormGuidesCount = SelectedForm->satinGuideCount;
	if (SelectedForm->fillType == SATF) {
		form::refil();
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void satin::delspnt() {
	form::fvars(ClosestFormToCursor);
	if (form::chkdel()) {
		StateMap.set(StateFlag::DELTO);
		thred::frmdel();
		StateMap.reset(StateFlag::FRMPSEL);
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedForm->type == SAT) {
		if (ClosestVertexToCursor < SelectedForm->wordParam) {
			SelectedForm->wordParam--;
		}
		if (SelectedForm->satinGuideCount != 0u) {
			auto guideIt = std::next(SatinGuides->begin(), SelectedForm->satinOrAngle.guide);
			auto iGuide  = 0u;
			while (guideIt[iGuide].start != ClosestVertexToCursor && guideIt[iGuide].finish != ClosestVertexToCursor
			       && iGuide < SelectedForm->satinGuideCount) {
				iGuide++;
			}
			if (iGuide < SelectedForm->satinGuideCount
			    && (guideIt[iGuide].start == ClosestVertexToCursor || guideIt[iGuide].finish == ClosestVertexToCursor)) {
				while (iGuide < SelectedForm->satinGuideCount) {
					guideIt[iGuide] = guideIt[wrap::toSize(iGuide) + 1u];
					iGuide++;
				}
				SelectedForm->satinGuideCount--;
				for (auto iForm = ClosestFormToCursor + 1u; iForm < FormList->size(); iForm++) {
					auto& form = (*FormList)[iForm];
					if (form.type == SAT && (form.satinGuideCount != 0u)) {
						form.satinOrAngle.guide++;
					}
				}
			}
			for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
				if (guideIt[iGuide].start > ClosestVertexToCursor) {
					guideIt[iGuide].start--;
				}
				if (guideIt[iGuide].finish > ClosestVertexToCursor) {
					guideIt[iGuide].finish--;
				}
			}
		}
	}
	auto closestVertexIt = std::next(FormVertices->cbegin(), wrap::toSize(SelectedForm->vertexIndex) + ClosestVertexToCursor);
	FormVertices->erase(closestVertexIt);
	SelectedForm->vertexCount--;
	form::fvars(ClosestFormToCursor);
	if (ClosestVertexToCursor > (SelectedForm->vertexCount) - 1) {
		ClosestVertexToCursor = SelectedForm->vertexCount - 1u;
	}
	StateMap.set(StateFlag::FRMPSEL);
	for (auto iForm = ClosestFormToCursor + 1u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		form.vertexIndex--;
	}
	auto vertexIt = std::next(FormVertices->cbegin(), SelectedForm->vertexIndex);
	thred::ritfcor(vertexIt[ClosestVertexToCursor]);
	displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
	form::frmout(ClosestFormToCursor);
	const auto vertex = vertexIt[ClosestVertexToCursor];
	if (vertex.x < ZoomRect.left || vertex.x > ZoomRect.right || vertex.y < ZoomRect.bottom || vertex.y > ZoomRect.top) {
		thred::shft(vertex);
	}
	form::refil();
}

void satin::internal::satsbrd() {
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEANGSAT;
	if (UserFlagMap.test(UserFlag::DUND)) {
		SelectedForm->edgeType |= EGUND;
	}
	form::bsizpar();
	SelectedForm->borderSize  = BorderWidth;
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderColor = gsl::narrow<uint8_t>(ActiveColor);
	form::refilfn();
}

void satin::satbrd() {
	if (displayText::filmsgs(FML_ANGS)) {
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
			si::satsbrd();
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
			si::satsbrd();
			StateMap.set(StateFlag::INIT);
			thred::coltab();
			thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void satin::internal::satends(uint32_t isBlunt) {
	if ((isBlunt & SBLNT) != 0u) {
		auto step = fPOINT { sin((*FormAngles)[0]) * HorizontalLength2 / 2.0f, cos((*FormAngles)[0]) * HorizontalLength2 / 2.0f };
		if (StateMap.test(StateFlag::INDIR)) {
			step.x = -step.x;
			step.y = -step.y;
		}
		auto vertexIt         = std::next(FormVertices->cbegin(), SelectedForm->vertexIndex);
		(*InsidePoints)[0].x  = vertexIt[0].x + step.x;
		(*InsidePoints)[0].y  = vertexIt[0].y - step.y;
		(*OutsidePoints)[0].x = vertexIt[0].x - step.x;
		(*OutsidePoints)[0].y = vertexIt[0].y + step.y;
	}
	else {
		auto vertexIt      = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		(*InsidePoints)[0] = (*OutsidePoints)[0] = vertexIt[0];
	}
	if ((isBlunt & FBLNT) != 0u) {
		auto step = fPOINT { sin((*FormAngles)[VertexCount - 2u]) * HorizontalLength2 / 2.0f,
			                 cos((*FormAngles)[VertexCount - 2u]) * HorizontalLength2 / 2.0f };
		if (StateMap.test(StateFlag::INDIR)) {
			step.x = -step.x;
			step.y = -step.y;
		}
		auto vertexIt                       = std::next(FormVertices->cbegin(), SelectedForm->vertexIndex);
		(*InsidePoints)[VertexCount - 1u].x  = vertexIt[VertexCount - 1u].x + step.x;
		(*InsidePoints)[VertexCount - 1u].y  = vertexIt[VertexCount - 1u].y - step.y;
		(*OutsidePoints)[VertexCount - 1u].x = vertexIt[VertexCount - 1u].x - step.x;
		(*OutsidePoints)[VertexCount - 1u].y = vertexIt[VertexCount - 1u].y + step.y;
	}
	else {
		auto vertexIt                    = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		(*InsidePoints)[VertexCount - 1u] = (*OutsidePoints)[VertexCount - 1u] = vertexIt[VertexCount - 1u];
	}
}

void satin::ribon() {
	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		form::fvars(ClosestFormToCursor);
		if (VertexCount > 2) {
			thred::savdo();
			const auto savedFormIndex = ClosestFormToCursor;
			satin::satout(BorderWidth);
			form::fvars(ClosestFormToCursor);
			HorizontalLength2 = BorderWidth / 2.0f;
			if (!FormList->empty()) {
				FormList->push_back(FRMHED {});
				auto& form = FormList->back();
				// reset vars as emplace may invalidate pointers
				form::fvars(ClosestFormToCursor);
				auto iNewVertex       = 0u;
				form.maxFillStitchLen = 9 * PFGRAN;
				form.minFillStitchLen = MinStitchLength;
				MaxStitchLen          = 9 * PFGRAN;
				if (SelectedForm->type == FRMLINE) {
					// Set blunt flags
					auto isBlunt = 0u;
					if (UserFlagMap.test(UserFlag::BLUNT)) {
						isBlunt = SBLNT | FBLNT;
					}
					else {
						isBlunt = 0;
					}
					si::satends(isBlunt);
					form.vertexIndex         = thred::adflt(VertexCount * 2u);
					auto vertexIt            = std::next(FormVertices->begin(), form.vertexIndex);
					vertexIt[0].x            = (*OutsidePoints)[0].x;
					vertexIt[iNewVertex++].y = (*OutsidePoints)[0].y;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						vertexIt[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					for (auto iVertex = VertexCount - 1u; iVertex != 0; iVertex--) {
						vertexIt[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				else {
					form.vertexIndex         = thred::adflt((VertexCount * 2u) + 2u);
					auto vertexIt            = std::next(FormVertices->begin(), form.vertexIndex);
					vertexIt[0].x            = (*OutsidePoints)[0].x;
					vertexIt[iNewVertex++].y = (*OutsidePoints)[0].y;
					form.underlayIndent      = IniFile.underlayIndent;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						vertexIt[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					vertexIt[iNewVertex++] = (*InsidePoints)[0];
					vertexIt[iNewVertex++] = (*OutsidePoints)[0];
					for (auto iVertex = VertexCount - 1u; iVertex != 0; iVertex--) {
						vertexIt[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				form.type                       = SAT;
				form.fillColor                  = gsl::narrow<uint8_t>(ActiveColor);
				form.fillSpacing                = LineSpacing;
				form.lengthOrCount.stitchLength = IniFile.maxStitchLength;
				form.vertexCount                = iNewVertex;
				form.attribute                  = 1;
				form.wordParam                  = iNewVertex / 2;
				form.satinGuideCount            = form.wordParam - 2u;
				form.satinOrAngle.guide         = adsatk(form.satinGuideCount);
				if (StateMap.test(StateFlag::CNV2FTH)) {
					form.fillType                       = FTHF;
					form.fillInfo.feather.ratio         = IniFile.featherRatio;
					form.fillInfo.feather.upCount       = IniFile.featherUpCount;
					form.fillInfo.feather.downCount     = IniFile.featherDownCount;
					form.fillInfo.feather.fillType      = IniFile.featherFillType;
					form.fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
					// ToDo - should this be ORed in to keep other attributes?
					form.extendedAttribute      = IniFile.featherType;
					form.fillInfo.feather.count = IniFile.featherCount;
					form.fillInfo.feather.color = (ActiveColor + 1u) & COLMSK;
				}
				else {
					form.fillType = SATF;
				}
				auto guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
				for (auto iGuide = 0u; iGuide < form.satinGuideCount; iGuide++) {
					guideIt[iGuide].start  = iGuide + 2u;
					guideIt[iGuide].finish = form.vertexCount - iGuide - 1u;
				}
				ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
				form::frmout(ClosestFormToCursor);
				form::refilfn();
				ClosestFormToCursor = savedFormIndex;
				StateMap.set(StateFlag::DELTO);
				thred::frmdel();
				ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1u);
				StateMap.set(StateFlag::FORMSEL);
				StateMap.set(StateFlag::INIT);
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				throw;
			}
		}
		else {
			displayText::tabmsg(IDS_FRM2);
		}
	}
	else {
		displayText::shoseln(IDS_FRM1MSG, IDS_CONVRIB);
	}
}

void satin::slbrd() {
	const auto savedSpacing = LineSpacing;

	OSequence->clear();
	if ((SelectedForm->edgeType & EGUND) != 0u) {
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		satin::satout(HorizontalLength2);
		si::satends(SelectedForm->attribute);
		StateMap.reset(StateFlag::SAT1);
		StateMap.reset(StateFlag::FILDIR);
		LineSpacing = USPAC;
		for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount - 1u; iVertex++) {
			si::sbfn(*InsidePoints, iVertex, iVertex + 1u);
		}
		StateMap.flip(StateFlag::FILDIR);
		for (auto iVertex = SelectedForm->vertexCount - 1u; iVertex != 0; iVertex--) {
			si::sbfn(*InsidePoints, iVertex, iVertex - 1);
		}
	}
	HorizontalLength2 = SelectedForm->borderSize;
	satin::satout(HorizontalLength2);
	si::satends(SelectedForm->attribute);
	LineSpacing = SelectedForm->edgeSpacing;
	StateMap.reset(StateFlag::SAT1);
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount - 1u; iVertex++) {
		si::sbfn(*InsidePoints, iVertex, iVertex + 1u);
	}
	LineSpacing = savedSpacing;
}

void satin::internal::satfn(const std::vector<float>& lengths,
                            uint32_t                  line1Start,
                            uint32_t                  line1End,
                            uint32_t                  line2Start,
                            uint32_t                  line2End) {
	if (line1Start != line1End && line2Start != line2End) {
		auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
		if (!StateMap.testAndSet(StateFlag::SAT1)) {
			if (StateMap.test(StateFlag::FTHR)) {
				auto vertex = vertexIt[line1Start % VertexCount];
				BSequence->emplace_back(BSEQPNT { vertex.x, vertex.y, 0 });
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					if (VertexCount != 0u) {
						auto vertex = vertexIt[line1Start % VertexCount];
						BSequence->emplace_back(BSEQPNT { vertex.x, vertex.y, 0 });
						vertex = vertexIt[line2Start % VertexCount];
						BSequence->emplace_back(BSEQPNT { vertex.x, vertex.y, 0 });
					}
				}
				else {
					SelectedPoint = vertexIt[line1Start];
					OSequence->push_back(SelectedPoint);
				}
			}
		}
		auto line1Length = lengths[line1End] - lengths[line1Start];
		auto line2Length = lengths[line2Start] - lengths[line2End];
		auto stitchCount = 0u;
		if (fabs(line1Length) > fabs(line2Length)) {
			stitchCount = wrap::round<uint32_t>(fabs(line2Length) / LineSpacing);
		}
		else {
			stitchCount = wrap::round<uint32_t>(fabs(line1Length) / LineSpacing);
		}
		const auto line1Segments     = ((line1End > line1Start) ? (line1End - line1Start) : (line1Start - line1End));
		const auto line2Segments     = ((line2Start > line2End) ? (line2Start - line2End) : (line2End - line2Start));
		auto       line1StitchCounts = std::vector<uint32_t> {};
		line1StitchCounts.reserve(line1Segments);
		auto line2StitchCounts = std::vector<uint32_t> {};
		line2StitchCounts.reserve(line2Segments);
		auto iVertex            = line1Start;
		auto segmentStitchCount = 0u;
		for (auto iSegment = 0u; iSegment < line1Segments - 1u; iSegment++) {
			const auto nextVertex = form::nxt(iVertex);
			const auto val = wrap::round<uint32_t>(((lengths[nextVertex] - lengths[iVertex]) / line1Length) * stitchCount + 0.5f);
			line1StitchCounts.push_back(val);
			segmentStitchCount += val;
			iVertex = form::nxt(iVertex);
		}
		line1StitchCounts.push_back(stitchCount - segmentStitchCount);
		auto iNextVertex = line2Start;
		iVertex          = form::prv(iNextVertex);
		// auto iSegment           = 0;
		segmentStitchCount = 0;
		while (iVertex > line2End) {
			const auto val
			    = wrap::round<uint32_t>(((lengths[iNextVertex] - lengths[iVertex]) / line2Length) * stitchCount + 0.5f);
			line2StitchCounts.push_back(val);
			segmentStitchCount += val;
			iNextVertex = form::prv(iNextVertex);
			iVertex     = form::prv(iNextVertex);
		}
		line2StitchCounts.push_back(stitchCount - segmentStitchCount);
		auto line1Point    = vertexIt[line1Start];
		auto line1Next     = form::nxt(line1Start);
		auto line2Previous = form::prv(line2Start);
		auto line1Count    = line1StitchCounts[0];
		auto line2Count    = line2StitchCounts[0];
		auto iLine1Vertex  = line1Start;
		auto iLine2Vertex  = line2Start;
		auto line1Delta
		    = fPOINT { vertexIt[line1Next].x - vertexIt[iLine1Vertex].x, vertexIt[line1Next].y - vertexIt[iLine1Vertex].y };
		auto line2Delta = fPOINT {};
		auto line2Point = fPOINT {};
		if (iLine2Vertex == VertexCount) {
			line2Delta.x = vertexIt[line2Previous].x - vertexIt[0].x;
			line2Delta.y = vertexIt[line2Previous].y - vertexIt[0].y;
			line2Point   = vertexIt[0];
		}
		else {
			line2Delta.x = vertexIt[line2Previous].x - vertexIt[iLine2Vertex].x;
			line2Delta.y = vertexIt[line2Previous].y - vertexIt[iLine2Vertex].y;
			line2Point   = vertexIt[iLine2Vertex];
		}
		iLine1Vertex     = form::nxt(iLine1Vertex);
		iLine2Vertex     = form::prv(iLine2Vertex);
		auto line1Step   = fPOINT { line1Delta.x / line1Count, line1Delta.y / line1Count };
		auto line2Step   = fPOINT { line2Delta.x / line2Count, line2Delta.y / line2Count };
		bool flag        = false;
		auto loop        = 0u;
		auto iLine1Count = 1u;
		auto iLine2Count = 1u;
		do {
			flag = false;
			loop++;
			if (StateMap.test(StateFlag::FTHR)) {
				while ((line1Count != 0u) && (line2Count != 0u)) {
					line1Point.x += line1Step.x;
					line1Point.y += line1Step.y;
					line2Point.x += line2Step.x;
					line2Point.y += line2Step.y;
					if (StateMap.testAndFlip(StateFlag::FILDIR)) {
						BSequence->emplace_back(BSEQPNT { line1Point.x, line1Point.y, 0 });
					}
					else {
						BSequence->emplace_back(BSEQPNT { line2Point.x, line2Point.y, 1 });
					}
					line1Count--;
					line2Count--;
				}
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					while ((line1Count != 0u) && (line2Count != 0u)) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							BSequence->emplace_back(BSEQPNT { line1Point.x, line1Point.y, 0 });
							BSequence->emplace_back(BSEQPNT { line2Point.x, line2Point.y, 1 });
						}
						else {
							BSequence->emplace_back(BSEQPNT { line2Point.x, line2Point.y, 2 });
							BSequence->emplace_back(BSEQPNT { line1Point.x, line1Point.y, 3 });
						}
						line1Count--;
						line2Count--;
					}
				}
				else {
					while ((line1Count != 0u) && (line2Count != 0u)) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							if (UserFlagMap.test(UserFlag::SQRFIL)) {
								form::filinu(line2Point);
							}
							form::filin(line1Point);
						}
						else {
							if (UserFlagMap.test(UserFlag::SQRFIL)) {
								form::filinu(line1Point);
							}
							form::filin(line2Point);
						}
						line1Count--;
						line2Count--;
					}
				}
			}
			if ((iLine1Count < line1Segments || iLine2Count < line2Segments)) {
				if ((line1Count == 0u) && iLine1Count < line1StitchCounts.size()) {
					line1Count   = line1StitchCounts[iLine1Count++];
					line1Next    = form::nxt(iLine1Vertex);
					line1Delta.x = vertexIt[line1Next].x - vertexIt[iLine1Vertex].x;
					line1Delta.y = vertexIt[line1Next].y - vertexIt[iLine1Vertex].y;
					iLine1Vertex = form::nxt(iLine1Vertex);
					line1Step.x  = line1Delta.x / line1Count;
					line1Step.y  = line1Delta.y / line1Count;
				}

				if ((line2Count == 0u) && iLine2Count < line2StitchCounts.size()) {
					line2Count    = line2StitchCounts[iLine2Count++];
					line2Previous = form::prv(iLine2Vertex);
					line2Delta.x  = vertexIt[line2Previous].x - vertexIt[iLine2Vertex].x;
					line2Delta.y  = vertexIt[line2Previous].y - vertexIt[iLine2Vertex].y;
					iLine2Vertex  = form::prv(iLine2Vertex);
					line2Step.x   = line2Delta.x / line2Count;
					line2Step.y   = line2Delta.y / line2Count;
				}
				if (((line1Count != 0u) || (line2Count != 0u)) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
					flag = true;
				}
			}
		} while (flag && (loop < 20000u));
	}
}

void satin::internal::satmf(const std::vector<float>& lengths) {
	auto start = 0u;

	if ((SelectedForm->attribute & FRMEND) != 0u) {
		start = 1;
	}
	auto guideIt = std::next(SatinGuides->cbegin(), CurrentFormGuides);
	si::satfn(lengths, start, guideIt[0].start, VertexCount, guideIt[0].finish);
	auto endGuide = CurrentFormGuidesCount;
	if (endGuide != 0u) {
		endGuide--;
	}
	for (auto iGuide = 0u; iGuide < endGuide; iGuide++) {
		auto& thisGuide = guideIt[iGuide];
		auto& nextGuide = guideIt[wrap::toSize(iGuide) + 1u];
		si::satfn(lengths, thisGuide.start, nextGuide.start, thisGuide.finish, nextGuide.finish);
	}
	if (SatinEndGuide != 0u) {
		si::satfn(lengths, guideIt[endGuide].start, SatinEndGuide, guideIt[endGuide].finish, SatinEndGuide + 1u);
	}
	else {
		if (guideIt[endGuide].finish - guideIt[endGuide].start > 2) {
			auto length
			    = (lengths[guideIt[endGuide].finish] - lengths[guideIt[endGuide].start]) / 2 + lengths[guideIt[endGuide].start];
			auto iVertex = guideIt[endGuide].start;
			while (length > lengths[iVertex]) {
				iVertex++;
			}
			auto deltaX = lengths[iVertex] - length;
			auto deltaY = length - lengths[iVertex - 1u];
			if (deltaY > deltaX) {
				iVertex--;
			}
			si::satfn(lengths, guideIt[endGuide].start, iVertex, guideIt[endGuide].finish, iVertex);
		}
		else {
			si::satfn(lengths,
			          guideIt[endGuide].start,
			          guideIt[endGuide].start + 1,
			          guideIt[endGuide].finish,
			          guideIt[endGuide].start + 1u);
		}
	}
}

void satin::satfil() {
	const auto savedSpacing = LineSpacing;

	form::fvars(ClosestFormToCursor);
	satin::satadj();
	LineSpacing /= 2;
	OSequence->clear();
	BSequence->clear();
	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SelectedForm->fillType = SATF;
	auto lengths           = std::vector<float> {};
	lengths.reserve(wrap::toSize(VertexCount) + 1u);
	auto length = 0.0f;
	lengths.push_back(length);
	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	for (auto iVertex = 1u; iVertex < VertexCount; iVertex++) {
		const auto delta
		    = fPOINT { vertexIt[iVertex].x - vertexIt[iVertex - 1u].x, vertexIt[iVertex].y - vertexIt[iVertex - 1u].y };
		length += hypot(delta.x, delta.y);
		lengths.push_back(length);
	}
	const auto lastDelta = fPOINT { vertexIt[0].x - vertexIt[VertexCount - 1u].x, vertexIt[0].y - vertexIt[VertexCount - 1u].y };
	length += hypot(lastDelta.x, lastDelta.y);
	lengths.push_back(length);
	do {
		if (SatinEndGuide != 0u) {
			if (CurrentFormGuidesCount != 0u) {
				si::satmf(lengths);
				break;
			}

			si::satfn(lengths, 1, SatinEndGuide, VertexCount, SatinEndGuide + 1u);
			break;
		}
		if ((SelectedForm->attribute & FRMEND) != 0u) {
			if (CurrentFormGuidesCount != 0u) {
				si::satmf(lengths);
				break;
			}

			if (VertexCount == 3 && (((*FormList)[ClosestFormToCursor].attribute & 1u) != 0)) {
				si::satfn(lengths, 2, 3, 2, 1);
				break;
			}

			length       = (length - lengths[1]) / 2;
			auto iVertex = 1u;
			if (!StateMap.test(StateFlag::BARSAT)) {
				SelectedPoint = vertexIt[1];
				OSequence->push_back(SelectedPoint);
			}
			while ((iVertex < (VertexCount + 1u)) && (length > lengths[iVertex])) {
				iVertex++;
			}
			const auto deltaA = lengths[iVertex] - length;
			const auto deltaB = length - lengths[iVertex - 1u];
			if (deltaB > deltaA) {
				iVertex--;
			}
			si::satfn(lengths, 1, iVertex, VertexCount, iVertex);

			break;
		}
		if (CurrentFormGuidesCount != 0u) {
			si::satmf(lengths);
			break;
		}
		length /= 2.0f;
		auto iVertex = 0;
		if (!StateMap.test(StateFlag::BARSAT) && !StateMap.test(StateFlag::FTHR)) {
			SelectedPoint = vertexIt[0];
			OSequence->push_back(SelectedPoint);
		}
		while (length > lengths[iVertex]) {
			iVertex++;
		}
		const auto deltaA = lengths[iVertex] - length;
		const auto deltaB = length - lengths[wrap::toSize(iVertex) - 1u];
		if (deltaB > deltaA) {
			iVertex--;
		}
		si::satfn(lengths, 0, iVertex, VertexCount, iVertex);
	} while (false);

	LineSpacing = savedSpacing;
}

void satin::satfix() {
	const auto vertexCount = wrap::toUnsigned(TempPolygon->size());
	auto&      form        = FormList->back();
	auto       minSize     = 1u;
	if (form.type == FRMFPOLY) {
		minSize = 2u;
	}
	if (TempPolygon->size() > minSize) {
		form.vertexIndex = thred::adflt(vertexCount);
		auto vertexIt    = std::next(FormVertices->begin(), form.vertexIndex);
		for (auto iVertex = 0u; iVertex < vertexCount; iVertex++) {
			vertexIt[iVertex] = (*TempPolygon)[iVertex];
		}
		TempPolygon->clear();
		form.vertexCount = vertexCount;
		form::frmout(wrap::toUnsigned(FormList->size() - 1u));
		form.satinGuideCount = 0;
		StateMap.set(StateFlag::INIT);
	}
	else {
		TempPolygon->clear();
		FormList->pop_back();
	}
	StateMap.reset(StateFlag::SHOSAT);
	StateMap.set(StateFlag::RESTCH);
}

void satin::dusat() {
	const auto  vertexCount = TempPolygon->size();
	auto&       formLines   = *FormLines;
	const auto* line        = &formLines[vertexCount - 1u];

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, line, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void satin::internal::unsat() {
	if (StateMap.testAndReset(StateFlag::SHOSAT)) {
		satin::dusat();
	}
}

void satin::drwsat() {
	si::unsat();
	thred::px2stch();
	const auto vertexCount = TempPolygon->size();
	auto&      formLines   = *FormLines;
	formLines.resize(vertexCount + 1u);
	formLines[vertexCount].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[vertexCount].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOSAT);
	satin::dusat();
}

void satin::satpnt0() {
	thred::px2stch();
	auto& formLines = *FormLines;
	formLines.clear();
	formLines.push_back(POINT { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y });
	TempPolygon->push_back(SelectedPoint);
	StateMap.set(StateFlag::SATPNT);
}

void satin::satpnt1() {
	si::unsat();
	thred::px2stch();
	const auto vertexCount   = TempPolygon->size();
	auto&      formLines     = *FormLines;
	formLines[vertexCount].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[vertexCount].y = Msg.pt.y - StitchWindowOrigin.y;
	satin::dusat();
	TempPolygon->push_back(SelectedPoint);
	StateMap.set(StateFlag::RESTCH);
}

void satin::internal::filinsbw(std::vector<fPOINT>& satinBackup, const fPOINT& point, uint32_t& satinBackupIndex) {
	satinBackup[satinBackupIndex++] = point;
	satinBackupIndex &= (satinBackup.size() - 1);
	form::filinsb(point);
}

void satin::internal::sbfn(const std::vector<fPOINT>& insidePoints, uint32_t start, uint32_t finish) {
	auto& outsidePoints = *OutsidePoints;
	auto  satinBackup   = std::vector<fPOINT> {}; // backup stitches in satin fills
	satinBackup.resize(8);
	auto innerDelta
	    = fPOINT { (insidePoints[finish].x - insidePoints[start].x), (insidePoints[finish].y - insidePoints[start].y) };
	const auto innerLength = hypot(innerDelta.x, innerDelta.y);
	auto       outerDelta
	    = fPOINT { (outsidePoints[finish].x - outsidePoints[start].x), (outsidePoints[finish].y - outsidePoints[start].y) };
	const auto outerLength = hypot(outerDelta.x, outerDelta.y);
	auto       innerPoint  = fPOINT { insidePoints[start].x, insidePoints[start].y };
	auto       outerPoint  = fPOINT { outsidePoints[start].x, outsidePoints[start].y };

	auto innerFlag = false;
	auto outerFlag = false;
	auto count     = 0u;

	if (!StateMap.testAndSet(StateFlag::SAT1)) {
		SelectedPoint = insidePoints[start];
	}
	for (auto& sb : satinBackup) {
		sb.x = 1e12f;
		sb.y = 1e12f;
	}
	if (outerLength > innerLength) {
		count     = wrap::round<uint32_t>(outerLength / LineSpacing);
		innerFlag = true;

		fPOINT intersection = {};
		if (form::linx(insidePoints, start, finish, intersection)) {
			innerDelta.x = 0.0f;
			innerDelta.y = 0.0f;
			innerPoint   = intersection;
		}
	}
	else {
		count     = wrap::round<uint32_t>(innerLength / LineSpacing);
		outerFlag = true;

		fPOINT intersection = {};
		if (form::linx(insidePoints, start, finish, intersection)) {
			outerDelta.x = 0.0f;
			outerDelta.y = 0.0f;
			outerPoint   = intersection;
		}
	}
	if (count == 0u) {
		count = 1;
	}
	if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
		return;
	}
	const auto innerStep        = fPOINT { innerDelta.x / count, innerDelta.y / count };
	const auto outerStep        = fPOINT { outerDelta.x / count, outerDelta.y / count };
	auto       satinBackupIndex = 0u;
	for (auto iStep = 0u; iStep < count; iStep++) {
		innerPoint.x += innerStep.x;
		innerPoint.y += innerStep.y;
		outerPoint.x += outerStep.x;
		outerPoint.y += outerStep.y;
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			if (innerFlag) {
				const auto offsetDelta  = fPOINT { innerPoint.x - SelectedPoint.x, innerPoint.y - SelectedPoint.y };
				const auto offsetLength = hypot(offsetDelta.x, offsetDelta.y);
				auto       offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
				const auto offsetStep   = fPOINT { offsetDelta.x / offsetCount, offsetDelta.y / offsetCount };
				auto       offset       = innerPoint;
				while (si::chkbak(satinBackup, offset)) {
					offset.x -= offsetStep.x;
					offset.y -= offsetStep.y;
				}
				si::filinsbw(satinBackup, offset, satinBackupIndex);
			}
			else {
				form::filinsb(innerPoint);
			}
		}
		else {
			if (outerFlag) {
				const auto offsetDelta  = fPOINT { outerPoint.x - SelectedPoint.x, outerPoint.y - SelectedPoint.y };
				const auto offsetLength = hypot(offsetDelta.x, offsetDelta.y);
				auto       offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
				const auto offsetStep   = fPOINT { offsetDelta.x / offsetCount, offsetDelta.y / offsetCount };
				auto       offset       = outerPoint;
				while (si::chkbak(satinBackup, offset)) {
					offset.x -= offsetStep.x;
					offset.y -= offsetStep.y;
				}
				si::filinsbw(satinBackup, offset, satinBackupIndex);
			}
			else {
				form::filinsb(outerPoint);
			}
		}
	}
}

void satin::internal::sfn(uint32_t startVertex) {
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		const auto nextVertex = form::nxt(startVertex);
		si::sbfn(*InsidePoints, startVertex, nextVertex);
		startVertex = nextVertex;
	}
	(*OSequence)[0] = OSequence->back();
}

void satin::satzum() {
	StateMap.reset(StateFlag::SHOSAT);
	thred::duzrat();
	const auto vertexCount = wrap::toUnsigned(TempPolygon->size());
	VertexCount            = vertexCount;
	form::frmlin(*TempPolygon);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	wrap::Polyline(StitchWindowMemDC, FormLines->data(), vertexCount);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	satin::drwsat();
}

void satin::satout(float satinWidth) {
	if (VertexCount != 0u) {
		form::duangs();
		OutsidePointList->resize(VertexCount);
		InsidePointList->resize(VertexCount);
		OutsidePoints = OutsidePointList;
		InsidePoints  = InsidePointList;
		for (auto iVertex = 0u; iVertex < VertexCount - 1u; iVertex++) {
			si::outfn(iVertex, iVertex + 1, 0.1f);
		}
		auto count = 0u;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			if (form::cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y)) {
				count++;
			}
		}
		satinWidth /= 2.0f;
		for (auto iVertex = 0u; iVertex < VertexCount - 1u; iVertex++) {
			si::outfn(iVertex, iVertex + 1, satinWidth);
		}
		si::outfn(VertexCount - 1, 0, satinWidth);
		StateMap.reset(StateFlag::INDIR);
		if (count < (VertexCount / 2)) {
			StateMap.set(StateFlag::INDIR);
			OutsidePoints = InsidePointList;
			InsidePoints  = OutsidePointList;
		}
	}
}

void satin::sbrd() {
	const auto savedSpacing = LineSpacing;
	const auto start        = form::getlast();

	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	OSequence->clear();
	OSequence->push_back(fPOINT { 0.0, 0.0 });
	if ((SelectedForm->edgeType & EGUND) != 0u) {
		LineSpacing = USPAC;
		satin::satout(HorizontalLength2 * URAT);
		si::sfn(start);
		StateMap.set(StateFlag::FILDIR);
		si::sfn(start);
	}
	form::fvars(ClosestFormToCursor);
	satin::satout(HorizontalLength2);
	LineSpacing = SelectedForm->edgeSpacing;
	si::sfn(start);
	LineSpacing = savedSpacing;
}

void satin::internal::outfn(uint32_t start, uint32_t finish, float satinWidth) {
	auto xOffset = 0.0f;
	auto yOffset = 0.0f;

	if (fabs((*FormAngles)[start]) < TINY && fabs((*FormAngles)[finish]) < TINY) {
		xOffset = 0.0f;
		yOffset = satinWidth;
	}
	else {
		constexpr auto SATHRESH = 10.0;

		auto angle  = ((*FormAngles)[finish] - (*FormAngles)[start]) / 2.0f;
		auto length = satinWidth / cos(angle);
		if (length < -satinWidth * SATHRESH) {
			length = -satinWidth * SATHRESH;
		}
		if (length > satinWidth * SATHRESH) {
			length = satinWidth * SATHRESH;
		}
		angle += (*FormAngles)[start] + PI / 2;
		xOffset = length * cos(angle);
		yOffset = length * sin(angle);
	}
	auto vertexIt = std::next(FormVertices->cbegin(), CurrentVertexIndex);
	if ((SelectedForm->type == FRMLINE) && ((SelectedForm->edgeType & NEGUND) == EDGEPROPSAT)) {
		vertexIt = std::next(AngledFormVertices->cbegin(), CurrentVertexIndex);
	}
	(*InsidePoints)[finish].x  = vertexIt[finish].x - xOffset;
	(*InsidePoints)[finish].y  = vertexIt[finish].y - yOffset;
	(*OutsidePoints)[finish].x = vertexIt[finish].x + xOffset;
	(*OutsidePoints)[finish].y = vertexIt[finish].y + yOffset;
}

bool satin::internal::chkbak(const std::vector<fPOINT>& satinBackup, const fPOINT& pnt) {
	const auto maxSB = satinBackup.size();
	for (auto iBackup = 0u; iBackup < maxSB; iBackup++) {
		const auto length = hypot(satinBackup[iBackup].x - pnt.x, satinBackup[iBackup].y - pnt.y);
		if (length < LineSpacing) {
			return true;
		}
	}
	return false;
}

uint32_t satin::adsatk(uint32_t count) {
	const auto iSatinConnect = wrap::toUnsigned(SatinGuides->size());
	const auto it            = SatinGuides->end();
	const auto val           = SATCON {};
	SatinGuides->insert(it, count, val);

	return iSatinConnect;
}

void satin::cpySat(const FRMHED& formHeader) {
	if (SelectedForm->type == SAT && (SelectedForm->satinGuideCount != 0u)) {
		SelectedForm->satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);

		auto       sourceStart = std::next(SatinGuides->cbegin(), formHeader.satinOrAngle.guide);
		auto       sourceEnd   = std::next(sourceStart, SelectedForm->satinGuideCount);
		const auto destination = std::next(SatinGuides->begin(), SelectedForm->satinOrAngle.guide);
		std::copy(sourceStart, sourceEnd, destination);
	}
}
