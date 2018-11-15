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

unsigned int StartPoint; // starting formOrigin for a satin stitch guide-line

unsigned satin::internal::satind(const SATCON* const guide) noexcept {
	// ToDo - find a better way than pointer arithmetic
	return guide - SatinGuides;
}

void satin::delsac(unsigned int formIndex) {
	auto& formList = *FormList;
	if (SatinGuideIndex) {
		if (formList[formIndex].type == SAT && formList[formIndex].satinGuideCount) {
			auto destination = si::satind(formList[formIndex].satinOrAngle.guide);
			auto source      = destination + formList[formIndex].satinGuideCount;
			while (source < SatinGuideIndex) {
				SatinGuides[destination++] = SatinGuides[source++];
			}
			for (auto iForm = formIndex + 1; iForm < FormIndex; iForm++) {
				if (formList[iForm].type == SAT && formList[iForm].satinGuideCount) {
					formList[iForm].satinOrAngle.guide -= formList[formIndex].satinGuideCount;
				}
			}
			SatinGuideIndex -= formList[formIndex].satinGuideCount;
		}
	}
	formList[formIndex].satinGuideCount = 0;
}

void satin::internal::sacspac(const SATCON* const startGuide, unsigned guideCount) {
	const auto iStartGuide = si::satind(startGuide);
	if (SatinGuideIndex) {
		auto source      = SatinGuideIndex - 1;
		auto destination = SatinGuideIndex + guideCount - 1;
		while (source >= iStartGuide) {
			SatinGuides[destination--] = SatinGuides[source--];
		}
	}
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		auto& form = (*FormList)[iForm];
		if (form.type == SAT) {
			form.satinOrAngle.guide += guideCount;
		}
	}
	SatinGuideIndex += guideCount;
}

SATCON* satin::internal::nusac(unsigned int formIndex, unsigned guideCount) {
	auto  guideIndex = 0u;
	auto& formList   = *FormList;

	for (auto iForm = 0u; iForm < formIndex; iForm++) {
		auto& form = formList[iForm];
		if (form.type == SAT) {
			guideIndex += form.satinGuideCount;
		}
	}
	si::sacspac(&SatinGuides[guideIndex], guideCount);
	return &SatinGuides[guideIndex];
}

void satin::spltsat(const SATCON& currentGuide) {
	// We are adding two additional vertices when splitting the form
	auto  vertexBuffer = std::vector<fPOINT>(gsl::narrow<size_t>(VertexCount) + 2);
	auto& formList     = *FormList;

	auto& srcForm = formList[ClosestFormToCursor];
	auto  it      = formList.begin() + ClosestFormToCursor;
	it            = formList.insert(it, srcForm);
	FormIndex++;
	auto maxForm = formList.size();
	if (ClosestFormToCursor < maxForm - 2) {
		form::mvfltsb(&FormVertices[FormVertexIndex + 1],
		              &FormVertices[FormVertexIndex - 1],
		              FormVertexIndex - form::fltind(formList[ClosestFormToCursor + 2].vertices));
	}
	FormVertexIndex += 2;
	for (auto iForm = ClosestFormToCursor + 2; iForm < maxForm; iForm++) {
		formList[iForm].vertices += 2;
	}
	auto       iOldVertex    = 0u;
	const auto oldLastVertex = currentGuide.start + (VertexCount - currentGuide.finish) + 1;
	auto       iNewVertex    = oldLastVertex + 1;
	for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
		if (iVertex == currentGuide.start || iVertex == currentGuide.finish) {
			vertexBuffer[iOldVertex++] = CurrentFormVertices[iVertex];
			if (iVertex == currentGuide.start) {
				vertexBuffer[iNewVertex++] = CurrentFormVertices[iVertex];
			}
			else {
				vertexBuffer[oldLastVertex] = CurrentFormVertices[iVertex];
			}
		}
		else {
			if (iVertex < currentGuide.start) {
				vertexBuffer[iOldVertex++] = CurrentFormVertices[iVertex];
			}
			else {
				if (iVertex < currentGuide.finish) {
					vertexBuffer[iNewVertex++] = CurrentFormVertices[iVertex];
				}
				else {
					vertexBuffer[iOldVertex++] = CurrentFormVertices[iVertex];
				}
			}
		}
	}
	for (auto iVertex = 0u; iVertex < iNewVertex; iVertex++) {
		CurrentFormVertices[iVertex] = vertexBuffer[iVertex];
	}
	SelectedForm->vertexCount = iOldVertex;
	auto& nextForm            = formList[ClosestFormToCursor + 1];
	nextForm.vertexCount      = iNewVertex - iOldVertex;
	nextForm.vertices         = &CurrentFormVertices[iOldVertex];
	form::frmout(ClosestFormToCursor);
	form::frmout(ClosestFormToCursor + 1);
	iOldVertex  = currentGuide.start + 1 - currentGuide.finish;
	auto iGuide = 0u;
	for (iGuide = 0u; iGuide < ActivePointIndex; iGuide++) {
		SelectedForm->satinOrAngle.guide[iGuide].finish += iOldVertex;
	}
	if (SelectedForm->wordParam) {
		SelectedForm->wordParam = currentGuide.start;
	}
	iOldVertex = iGuide + 1;
	while (iGuide < (SelectedForm->satinGuideCount)) {
		SelectedForm->satinOrAngle.guide[iGuide].start -= (currentGuide.start - 1);
		SelectedForm->satinOrAngle.guide[iGuide].finish -= (currentGuide.start - 1);
		iGuide++;
	}
	if (nextForm.wordParam) {
		nextForm.wordParam -= (currentGuide.start - 1);
	}
	auto       sourceStart = SelectedForm->satinOrAngle.guide + iOldVertex;
	auto       sourceRange = SatinGuideIndex - si::satind(&SelectedForm->satinOrAngle.guide[iOldVertex]);
	auto       sourceEnd   = sourceStart + sourceRange;
	const auto destination = stdext::make_checked_array_iterator((sourceStart - 1), sourceRange);
	std::copy(sourceStart, sourceEnd, destination);
	SatinGuideIndex--;
	nextForm.satinOrAngle.guide   = &SelectedForm->satinOrAngle.guide[ActivePointIndex];
	nextForm.satinGuideCount      = SelectedForm->satinGuideCount - ActivePointIndex - 1;
	SelectedForm->satinGuideCount = ActivePointIndex;
	for (auto iForm = ClosestFormToCursor + 2; iForm < maxForm; iForm++) {
		auto& formIter = formList[iForm];
		formIter.satinOrAngle.guide--;
	}
	if (clip::iseclp(ClosestFormToCursor)) {
		form::clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (auto iForm = ClosestFormToCursor + 1; iForm < maxForm; iForm++) {
			auto& formIter = formList[iForm];
			formIter.borderClipData += SelectedForm->clipEntries;
		}
	}
	form::stchadj();
}

void satin::internal::satclos() {
	auto initialGuideCount = SelectedForm->satinGuideCount;
	auto minimumLength     = 1e99;

	form::uninsf();
	thred::px2stch();
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		const auto deltaX = SelectedPoint.x - SelectedForm->vertices[iVertex].x;
		const auto deltaY = SelectedPoint.y - SelectedForm->vertices[iVertex].y;
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
		if (!StartPoint) {
			StartPoint++;
		}
		if (StartPoint == SelectedForm->vertexCount - 2 && closestVertex == (SelectedForm->vertexCount) - 1) {
			StartPoint    = 1;
			closestVertex = SelectedForm->vertexCount - 2;
		}
		if (closestVertex >= (SelectedForm->vertexCount) - 2) {
			closestVertex = SelectedForm->vertexCount - 2;
			if (StartPoint >= (SelectedForm->vertexCount) - 2) {
				StartPoint = SelectedForm->vertexCount - 2;
			}
		}
		if (closestVertex - StartPoint < 2) {
			closestVertex = StartPoint + 2;
			if (closestVertex > (SelectedForm->vertexCount) - 2) {
				closestVertex = closestVertex - SelectedForm->vertexCount - 2;
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
			closestVertex         = VertexCount - 1;
			ClosestVertexToCursor = VertexCount;
		}
		if (closestVertex == 1 && ClosestVertexToCursor == VertexCount) {
			closestVertex         = 0;
			ClosestVertexToCursor = 1;
		}
		if (ClosestVertexToCursor - closestVertex == 1) {
			if (SelectedForm->attribute & FRMEND) {
				SelectedForm->wordParam = closestVertex;
			}
			else {
				if (closestVertex) {
					form::rotfrm(closestVertex);
				}
				SelectedForm->attribute |= FRMEND;
			}
			satin::satadj();
		}
		else {
			if (CurrentFormGuidesCount) {
				si::sacspac(&SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount].start    = closestVertex;
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satin::satadj();
			}
			else {
				SelectedForm->satinOrAngle.guide                           = si::nusac(ClosestFormToCursor, 1);
				SelectedForm->satinOrAngle.guide[initialGuideCount].start  = closestVertex;
				SelectedForm->satinOrAngle.guide[initialGuideCount].finish = ClosestVertexToCursor;
				SelectedForm->satinGuideCount                              = 1;
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
	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		auto&      form      = (*FormList)[iForm];
		const auto layerCode = (form.attribute & FRMLMSK) >> 1;
		if (!ActiveLayer || !layerCode || layerCode == ActiveLayer) {
			for (auto iVertex = 0u; iVertex < form.vertexCount; iVertex++) {
				const auto deltaX = SelectedPoint.x - form.vertices[iVertex].x;
				const auto deltaY = SelectedPoint.y - form.vertices[iVertex].y;
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
		StartPoint = ClosestVertexToCursor;
		form::sfCor2px(SelectedForm->vertices[ClosestVertexToCursor], (*FormLines)[0]);
		StateMap.reset(StateFlag::SHOCON);
		StateMap.set(StateFlag::SATCNKT);
		if (SelectedForm->type == FRMFPOLY) {
			SelectedForm->type = SAT;
		}
	}
}

void satin::internal::satcpy(SATCON* const destination, const std::vector<SATCON> source, unsigned int size) {
	if (destination) {
		for (unsigned int iSource = 0; iSource < size; iSource++) {
			destination[iSource] = source[iSource];
		}
	}
}

void satin::satadj() {
	form::fvars(ClosestFormToCursor);

	auto interiorGuides  = std::vector<SATCON>(CurrentFormGuidesCount);
	auto savedGuideCount = SelectedForm->satinGuideCount;
	auto satinMap        = ExtendedBitSet<>(VertexCount);

	// ensure all guide endpoints are on valid vertices
	for (auto iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
		const auto endCount = (VertexCount - 1);
		if (CurrentFormGuides[iGuide].finish > endCount) {
			CurrentFormGuides[iGuide].finish = endCount;
		}
		if (CurrentFormGuides[iGuide].start > endCount) {
			CurrentFormGuides[iGuide].start = endCount;
		}
	}

	// remove any guides of 0 length
	auto iDestination = 0u;
	for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
		if (CurrentFormGuides[iSource].start != CurrentFormGuides[iSource].finish) {
			interiorGuides[iDestination++] = CurrentFormGuides[iSource];
		}
	}
	if (CurrentFormGuidesCount != iDestination) {
		OutputDebugString(fmt::format(L"Removed {} zero distance guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
		si::satcpy(CurrentFormGuides, interiorGuides, iDestination);
	}
	if (SatinEndGuide || SelectedForm->attribute & FRMEND) {
		// there are end guides so set the satinMap for the next step
		satinMap.reset();
		if (SelectedForm->attribute & FRMEND) {
			satinMap.set(0);
			satinMap.set(1);
		}
		if (SatinEndGuide) {
			satinMap.set(SatinEndGuide);
			satinMap.set(gsl::narrow<size_t>(SatinEndGuide) + 1);
		}
		// check to see if any of the current guides are end guides and add to interiorGuides if not
		iDestination = 0;
		for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
			if (!satinMap.test(CurrentFormGuides[iSource].start) && !satinMap.test(CurrentFormGuides[iSource].finish)) {
				interiorGuides[iDestination++] = CurrentFormGuides[iSource];
			}
		}
		if (CurrentFormGuidesCount != iDestination) {
			OutputDebugString(fmt::format(L"Removed {} end guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
			CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
			si::satcpy(CurrentFormGuides, interiorGuides, iDestination);
		}
		// remove any guides that start after the end guide
		if (SatinEndGuide) {
			iDestination = 0;
			for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
				if (CurrentFormGuides[iSource].start < SatinEndGuide) {
					interiorGuides[iDestination++] = CurrentFormGuides[iSource];
				}
			}
			if (CurrentFormGuidesCount != iDestination) {
				OutputDebugString(fmt::format(L"Removed {} reversed guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
				CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
				si::satcpy(CurrentFormGuides, interiorGuides, iDestination);
			}
		}
	}

	if (CurrentFormGuidesCount) {
		satinMap.reset();
		for (auto iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			auto iForward = CurrentFormGuides[iGuide].start;
			if (iForward > (SatinEndGuide)-1) {
				iForward = SatinEndGuide - 1;
			}
			if (satinMap.test_set(iForward)) {
				auto iReverse = iForward;
				if (iReverse) {
					iReverse--;
				}
				while (satinMap.test(iForward) && (iForward < ((SatinEndGuide)-1))) {
					iForward++;
				}
				while (iReverse && (satinMap.test(iReverse))) {
					iReverse--;
				}
				if (satinMap.test(iForward) && satinMap.test(iReverse)) {
					break;
				}
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].start > CurrentFormGuides[iGuide].start - iReverse) {
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
			iVertex = gsl::narrow<unsigned int>(satinMap.getFirst());
			if (iVertex < VertexCount) {
				CurrentFormGuides[iGuide++].start = iVertex;
			}
		} while (iVertex < VertexCount);

		CurrentFormGuidesCount = SelectedForm->satinGuideCount = (iGuide);
		satinMap.reset();
		// Todo - are iForward and iReverse appropriate variable names below?
		for (iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			auto iForward = CurrentFormGuides[iGuide].finish;
			auto iReverse = iForward;
			if (iForward > VertexCount - 1) {
				iForward = VertexCount - 1;
			}
			if (satinMap.test_set(iForward)) {
				if (iForward < VertexCount - 1) {
					iForward++;
				}
				if (iReverse > (SatinEndGuide) + 1) {
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
					if (iForward - CurrentFormGuides[iGuide].finish > CurrentFormGuides[iGuide].finish - iReverse) {
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
			iReverse = gsl::narrow<unsigned int>(satinMap.getLast());
			if (iReverse < VertexCount) {
				CurrentFormGuides[iGuide++].finish = iReverse;
			}
		} while (iReverse < VertexCount);
		if (iGuide < CurrentFormGuidesCount) {
			iGuide = CurrentFormGuidesCount;
		}
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = (iGuide);
		if (SatinEndGuide) {
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
		auto iGuide           = savedGuideCount - CurrentFormGuidesCount;
		auto sourceGuide      = SelectedForm->satinOrAngle.guide;
		auto destinationGuide = sourceGuide;
		destinationGuide += SelectedForm->satinGuideCount;
		sourceGuide += savedGuideCount;
		std::copy(sourceGuide,
		          (&SatinGuides[SatinGuideIndex] + 1),
		          stdext::make_checked_array_iterator(destinationGuide, 10000 - SatinGuideIndex));
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			auto& form = (*FormList)[iForm];
			if (form.type == SAT) {
				form.satinOrAngle.guide -= iGuide;
			}
		}
		SatinGuideIndex -= iGuide;
	}
}

void satin::delcon(unsigned GuideIndex) {
	// ToDo - Find a better way to calculate the offset into the SatinGuides array
	const auto iGuide = gsl::narrow<unsigned int>(&SelectedForm->satinOrAngle.guide[GuideIndex] - SatinGuides);
	auto       guide  = &SelectedForm->satinOrAngle.guide[GuideIndex];

	if (guide) {
		if (SatinGuideIndex > iGuide) {
			// Todo - use std::vector & member erase() ?
			std::copy(guide + 1,
			          guide + gsl::narrow<size_t>(SatinGuideIndex - iGuide + 1u),
			          stdext::make_checked_array_iterator(guide, SatinGuideIndex));
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			auto& form = (*FormList)[iForm];
			if (form.type == SAT && form.satinGuideCount) {
				form.satinOrAngle.guide--;
			}
		}
		if (ClosestVertexToCursor < SatinEndGuide) {
			SatinEndGuide--;
		}
		SelectedForm->satinGuideCount--;
		SatinGuideIndex--;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount;
		if (SelectedForm->fillType == SATF) {
			form::refil();
		}
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
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
		if (SelectedForm->satinGuideCount) {
			auto guide = SelectedForm->satinOrAngle.guide;
			if (guide) {
				auto iGuide = 0u;
				while (guide[iGuide].start != ClosestVertexToCursor && guide[iGuide].finish != ClosestVertexToCursor
				       && iGuide < SelectedForm->satinGuideCount) {
					iGuide++;
				}
				if (iGuide < SelectedForm->satinGuideCount
				    && (guide[iGuide].start == ClosestVertexToCursor || guide[iGuide].finish == ClosestVertexToCursor)) {
					while (iGuide < SelectedForm->satinGuideCount) {
						guide[iGuide] = guide[iGuide + 1];
						iGuide++;
					}
					SelectedForm->satinGuideCount--;
					SatinGuideIndex--;
					for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
						auto& form = (*FormList)[iForm];
						if (form.type == SAT && form.satinGuideCount) {
							form.satinOrAngle.guide++;
						}
					}
				}
				for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
					if (guide[iGuide].start > ClosestVertexToCursor) {
						guide[iGuide].start--;
					}
					if (guide[iGuide].finish > ClosestVertexToCursor) {
						guide[iGuide].finish--;
					}
				}
			}
		}
	}
	// ToDo - convert to vector and use erase()
	std::copy(SelectedForm->vertices + ClosestVertexToCursor + 1,
	          SelectedForm->vertices + FormVertexIndex,
	          stdext::make_checked_array_iterator(SelectedForm->vertices + ClosestVertexToCursor,
	                                              FormVertexIndex - (ClosestVertexToCursor + 1)));
	SelectedForm->vertexCount--;
	FormVertexIndex--;
	form::fvars(ClosestFormToCursor);
	if (ClosestVertexToCursor > (SelectedForm->vertexCount) - 1) {
		ClosestVertexToCursor = SelectedForm->vertexCount - 1;
	}
	StateMap.set(StateFlag::FRMPSEL);
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		auto& form = (*FormList)[iForm];
		form.vertices--;
	}
	thred::ritfcor(CurrentFormVertices[ClosestVertexToCursor]);
	displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
	form::frmout(ClosestFormToCursor);
	const auto vertex = SelectedForm->vertices[ClosestVertexToCursor];
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
	SelectedForm->borderColor = gsl::narrow<unsigned char>(ActiveColor);
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
			thred::ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void satin::internal::satends(unsigned isBlunt) {
	if (isBlunt & SBLNT) {
		auto step = fPOINT{ sin((*FormAngles)[0]) * HorizontalLength2 / 2.0, cos((*FormAngles)[0]) * HorizontalLength2 / 2.0 };
		if (StateMap.test(StateFlag::INDIR)) {
			step.x = -step.x;
			step.y = -step.y;
		}
		(*InsidePoints)[0].x  = SelectedForm->vertices[0].x + step.x;
		(*InsidePoints)[0].y  = SelectedForm->vertices[0].y - step.y;
		(*OutsidePoints)[0].x = SelectedForm->vertices[0].x - step.x;
		(*OutsidePoints)[0].y = SelectedForm->vertices[0].y + step.y;
	}
	else {
		(*InsidePoints)[0] = (*OutsidePoints)[0] = CurrentFormVertices[0];
	}
	if (isBlunt & FBLNT) {
		auto step = fPOINT{ sin((*FormAngles)[VertexCount - 2]) * HorizontalLength2 / 2.0,
			                cos((*FormAngles)[VertexCount - 2]) * HorizontalLength2 / 2.0 };
		if (StateMap.test(StateFlag::INDIR)) {
			step.x = -step.x;
			step.y = -step.y;
		}
		(*InsidePoints)[VertexCount - 1].x  = SelectedForm->vertices[VertexCount - 1].x + step.x;
		(*InsidePoints)[VertexCount - 1].y  = SelectedForm->vertices[VertexCount - 1].y - step.y;
		(*OutsidePoints)[VertexCount - 1].x = SelectedForm->vertices[VertexCount - 1].x - step.x;
		(*OutsidePoints)[VertexCount - 1].y = SelectedForm->vertices[VertexCount - 1].y + step.y;
	}
	else {
		(*InsidePoints)[VertexCount - 1] = (*OutsidePoints)[VertexCount - 1] = CurrentFormVertices[VertexCount - 1];
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
			HorizontalLength2 = BorderWidth / 2;
			if (!(*FormList).empty()) {
				(*FormList).emplace_back(FRMHED{});
				auto& form = (*FormList).back();
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
					form.vertices                 = thred::adflt(VertexCount << 1);
					form.vertices[0].x            = (*OutsidePoints)[0].x;
					form.vertices[iNewVertex++].y = (*OutsidePoints)[0].y;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						form.vertices[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						form.vertices[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				else {
					form.vertices                 = thred::adflt((VertexCount << 1) + 2);
					form.vertices[0].x            = (*OutsidePoints)[0].x;
					form.vertices[iNewVertex++].y = (*OutsidePoints)[0].y;
					form.underlayIndent           = IniFile.underlayIndent;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						form.vertices[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					form.vertices[iNewVertex++] = (*InsidePoints)[0];
					form.vertices[iNewVertex++] = (*OutsidePoints)[0];
					for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						form.vertices[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				form.type                       = SAT;
				form.fillColor                  = gsl::narrow<unsigned char>(ActiveColor);
				form.fillSpacing                = LineSpacing;
				form.lengthOrCount.stitchLength = IniFile.maxStitchLength;
				form.vertexCount                = iNewVertex;
				form.attribute                  = 1;
				form.wordParam                  = iNewVertex >> 1;
				form.satinGuideCount            = form.wordParam - 2;
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
					form.fillInfo.feather.color = (ActiveColor + 1) & COLMSK;
				}
				else {
					form.fillType = SATF;
				}
				for (auto iGuide = 0u; iGuide < form.satinGuideCount; iGuide++) {
					form.satinOrAngle.guide[iGuide].start  = iGuide + 2;
					form.satinOrAngle.guide[iGuide].finish = form.vertexCount - iGuide - 1;
				}
				FormIndex++;
				ClosestFormToCursor = (*FormList).size() - 1;
				form::frmout(ClosestFormToCursor);
				form::refilfn();
				ClosestFormToCursor = savedFormIndex;
				StateMap.set(StateFlag::DELTO);
				thred::frmdel();
				ClosestFormToCursor = (*FormList).size() - 1;
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

	SequenceIndex = 0;
	if (SelectedForm->edgeType & EGUND) {
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		satin::satout(HorizontalLength2);
		si::satends(SelectedForm->attribute);
		StateMap.reset(StateFlag::SAT1);
		StateMap.reset(StateFlag::FILDIR);
		LineSpacing = USPAC;
		for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount - 1; iVertex++) {
			si::sbfn(*InsidePoints, iVertex, iVertex + 1);
		}
		StateMap.flip(StateFlag::FILDIR);
		for (auto iVertex = SelectedForm->vertexCount - 1; iVertex != 0; iVertex--) {
			si::sbfn(*InsidePoints, iVertex, iVertex - 1);
		}
	}
	HorizontalLength2 = SelectedForm->borderSize;
	satin::satout(HorizontalLength2);
	si::satends(SelectedForm->attribute);
	LineSpacing = SelectedForm->edgeSpacing;
	StateMap.reset(StateFlag::SAT1);
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount - 1; iVertex++) {
		si::sbfn(*InsidePoints, iVertex, iVertex + 1);
	}
	LineSpacing = savedSpacing;
}

void satin::internal::satfn(const std::vector<double>& lengths,
                            unsigned int               line1Start,
                            unsigned int               line1End,
                            unsigned int               line2Start,
                            unsigned int               line2End) {
	if (line1Start != line1End && line2Start != line2End) {
		if (!StateMap.testAndSet(StateFlag::SAT1)) {
			if (StateMap.test(StateFlag::FTHR)) {
				BSequence[SequenceIndex].attribute = 0;
				form::ritseq1(line1Start % VertexCount);
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					form::ritseq1(line1Start % VertexCount);
					form::ritseq1(line2Start % VertexCount);
				}
				else {
					SelectedPoint              = CurrentFormVertices[line1Start];
					OSequence[SequenceIndex++] = SelectedPoint;
				}
			}
		}
		auto line1Length = lengths[line1End] - lengths[line1Start];
		auto line2Length = lengths[line2Start] - lengths[line2End];
		auto stitchCount = 0u;
		if (fabs(line1Length) > fabs(line2Length)) {
			stitchCount = dToUI(fabs(line2Length) / LineSpacing);
		}
		else {
			stitchCount = dToUI(fabs(line1Length) / LineSpacing);
		}
		const auto line1Segments     = ((line1End > line1Start) ? (line1End - line1Start) : (line1Start - line1End));
		const auto line2Segments     = ((line2Start > line2End) ? (line2Start - line2End) : (line2End - line2Start));
		auto       line1StitchCounts = std::vector<unsigned>();
		line1StitchCounts.reserve(line1Segments);
		auto line2StitchCounts = std::vector<unsigned>();
		line2StitchCounts.reserve(line2Segments);
		auto iVertex            = line1Start;
		auto segmentStitchCount = 0u;
		for (auto iSegment = 0u; iSegment < line1Segments - 1; iSegment++) {
			const auto nextVertex = form::nxt(iVertex);
			const auto val        = gsl::narrow<unsigned int>(
                std::round(((lengths[nextVertex] - lengths[iVertex]) / line1Length) * stitchCount + 0.5));
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
			const auto val = gsl::narrow<unsigned int>(
			    std::round(((lengths[iNextVertex] - lengths[iVertex]) / line2Length) * stitchCount + 0.5));
			line2StitchCounts.push_back(val);
			segmentStitchCount += val;
			iNextVertex = form::prv(iNextVertex);
			iVertex     = form::prv(iNextVertex);
		}
		line2StitchCounts.push_back(stitchCount - segmentStitchCount);
		auto line1Point    = dPOINT(CurrentFormVertices[line1Start]);
		auto line1Next     = form::nxt(line1Start);
		auto line2Previous = form::prv(line2Start);
		auto line1Count    = line1StitchCounts[0];
		auto line2Count    = line2StitchCounts[0];
		auto iLine1Vertex  = line1Start;
		auto iLine2Vertex  = line2Start;
		auto line1Delta    = dPOINT{ CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x,
                                  CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y };
		auto line2Delta    = dPOINT{};
		auto line2Point    = dPOINT{};
		if (iLine2Vertex == VertexCount) {
			line2Delta.x = static_cast<double>(CurrentFormVertices[line2Previous].x) - CurrentFormVertices[0].x;
			line2Delta.y = static_cast<double>(CurrentFormVertices[line2Previous].y) - CurrentFormVertices[0].y;
			line2Point   = CurrentFormVertices[0];
		}
		else {
			line2Delta.x = static_cast<double>(CurrentFormVertices[line2Previous].x) - CurrentFormVertices[iLine2Vertex].x;
			line2Delta.y = static_cast<double>(CurrentFormVertices[line2Previous].y) - CurrentFormVertices[iLine2Vertex].y;
			line2Point   = CurrentFormVertices[iLine2Vertex];
		}
		iLine1Vertex     = form::nxt(iLine1Vertex);
		iLine2Vertex     = form::prv(iLine2Vertex);
		auto line1Step   = dPOINT{ line1Delta.x / line1Count, line1Delta.y / line1Count };
		auto line2Step   = dPOINT{ line2Delta.x / line2Count, line2Delta.y / line2Count };
		bool flag        = false;
		auto iLine1Count = 1u;
		auto iLine2Count = 1u;
		do {
			flag = false;
			if (StateMap.test(StateFlag::FTHR)) {
				while (line1Count && line2Count) {
					line1Point.x += line1Step.x;
					line1Point.y += line1Step.y;
					line2Point.x += line2Step.x;
					line2Point.y += line2Step.y;
					if (StateMap.testAndFlip(StateFlag::FILDIR)) {
						BSequence[SequenceIndex++] = { static_cast<float>(line1Point.x), static_cast<float>(line1Point.y), 0 };
					}
					else {
						BSequence[SequenceIndex++] = { static_cast<float>(line2Point.x), static_cast<float>(line2Point.y), 1 };
					}
					if (SequenceIndex > MAXITEMS - 6) {
						SequenceIndex = MAXITEMS - 6;
						return;
					}
					line1Count--;
					line2Count--;
				}
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					while (line1Count && line2Count) {
						line1Point.x += line1Step.x;
						line1Point.y += line1Step.y;
						line2Point.x += line2Step.x;
						line2Point.y += line2Step.y;
						if (StateMap.testAndFlip(StateFlag::FILDIR)) {
							BSequence[SequenceIndex++]
							    = { static_cast<float>(line1Point.x), static_cast<float>(line1Point.y), 0 };
							BSequence[SequenceIndex++]
							    = { static_cast<float>(line2Point.x), static_cast<float>(line2Point.y), 1 };
						}
						else {
							BSequence[SequenceIndex++]
							    = { static_cast<float>(line2Point.x), static_cast<float>(line2Point.y), 2 };
							BSequence[SequenceIndex++]
							    = { static_cast<float>(line1Point.x), static_cast<float>(line1Point.y), 3 };
						}
						if (SequenceIndex > MAXITEMS - 6) {
							SequenceIndex = MAXITEMS - 6;
							return;
						}
						line1Count--;
						line2Count--;
					}
				}
				else {
					while (line1Count && line2Count) {
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
				if (!line1Count && iLine1Count < line1StitchCounts.size()) {
					line1Count   = line1StitchCounts[iLine1Count++];
					line1Next    = form::nxt(iLine1Vertex);
					line1Delta.x = static_cast<double>(CurrentFormVertices[line1Next].x) - CurrentFormVertices[iLine1Vertex].x;
					line1Delta.y = static_cast<double>(CurrentFormVertices[line1Next].y) - CurrentFormVertices[iLine1Vertex].y;
					iLine1Vertex = form::nxt(iLine1Vertex);
					line1Step.x  = line1Delta.x / line1Count;
					line1Step.y  = line1Delta.y / line1Count;
				}

				if (!line2Count && iLine2Count < line2StitchCounts.size()) {
					line2Count    = line2StitchCounts[iLine2Count++];
					line2Previous = form::prv(iLine2Vertex);
					line2Delta.x
					    = static_cast<double>(CurrentFormVertices[line2Previous].x) - CurrentFormVertices[iLine2Vertex].x;
					line2Delta.y
					    = static_cast<double>(CurrentFormVertices[line2Previous].y) - CurrentFormVertices[iLine2Vertex].y;
					iLine2Vertex = form::prv(iLine2Vertex);
					line2Step.x  = line2Delta.x / line2Count;
					line2Step.y  = line2Delta.y / line2Count;
				}
				if ((line1Count || line2Count) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
					flag = true;
				}
			}
		} while (flag);
	}
}

void satin::internal::satmf(const std::vector<double>& lengths) {
	auto start = 0u;

	if (SelectedForm->attribute & FRMEND) {
		start = 1;
	}
	si::satfn(lengths, start, CurrentFormGuides[0].start, VertexCount, CurrentFormGuides[0].finish);
	auto endGuide = CurrentFormGuidesCount;
	if (endGuide) {
		endGuide--;
	}
	for (auto iGuide = 0u; iGuide < endGuide; iGuide++) {
		si::satfn(lengths,
		          CurrentFormGuides[iGuide].start,
		          CurrentFormGuides[iGuide + 1].start,
		          CurrentFormGuides[iGuide].finish,
		          CurrentFormGuides[iGuide + 1].finish);
	}
	if (SatinEndGuide) {
		si::satfn(
		    lengths, CurrentFormGuides[endGuide].start, SatinEndGuide, CurrentFormGuides[endGuide].finish, SatinEndGuide + 1);
	}
	else {
		if (CurrentFormGuides[endGuide].finish - CurrentFormGuides[endGuide].start > 2) {
			auto length = (lengths[CurrentFormGuides[endGuide].finish] - lengths[CurrentFormGuides[endGuide].start]) / 2
			              + lengths[CurrentFormGuides[endGuide].start];
			auto iVertex = CurrentFormGuides[endGuide].start;
			while (length > lengths[iVertex]) {
				iVertex++;
			}
			auto deltaX = lengths[iVertex] - length;
			auto deltaY = length - lengths[iVertex - 1];
			if (deltaY > deltaX) {
				iVertex--;
			}
			si::satfn(lengths, CurrentFormGuides[endGuide].start, iVertex, CurrentFormGuides[endGuide].finish, iVertex);
		}
		else {
			si::satfn(lengths,
			          CurrentFormGuides[endGuide].start,
			          CurrentFormGuides[endGuide].start + 1,
			          CurrentFormGuides[endGuide].finish,
			          CurrentFormGuides[endGuide].start + 1);
		}
	}
}

void satin::satfil() {
	const auto savedSpacing = LineSpacing;

	form::fvars(ClosestFormToCursor);
	satin::satadj();
	LineSpacing /= 2;
	SequenceIndex = 0;
	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SelectedForm->fillType = SATF;
	auto lengths           = std::vector<double>();
	lengths.reserve(gsl::narrow<size_t>(VertexCount) + 1);
	auto length = 0.0;
	lengths.push_back(length);
	for (auto iVertex = 1u; iVertex < VertexCount; iVertex++) {
		const auto delta = dPOINT{ CurrentFormVertices[iVertex].x - CurrentFormVertices[iVertex - 1].x,
			                       CurrentFormVertices[iVertex].y - CurrentFormVertices[iVertex - 1].y };
		length += hypot(delta.x, delta.y);
		lengths.push_back(length);
	}
	const auto lastDelta = dPOINT{ CurrentFormVertices[0].x - CurrentFormVertices[VertexCount - 1].x,
		                           CurrentFormVertices[0].y - CurrentFormVertices[VertexCount - 1].y };
	length += hypot(lastDelta.x, lastDelta.y);
	lengths.push_back(length);
	do {
		if (SatinEndGuide) {
			if (CurrentFormGuidesCount) {
				si::satmf(lengths);
				break;
			}

			si::satfn(lengths, 1, SatinEndGuide, VertexCount, SatinEndGuide + 1);
			break;
		}
		if (SelectedForm->attribute & FRMEND) {
			if (CurrentFormGuidesCount) {
				si::satmf(lengths);
				break;
			}

			if (VertexCount == 3 && (*FormList)[ClosestFormToCursor].attribute & 1) {
				si::satfn(lengths, 2, 3, 2, 1);
				break;
			}

			length       = (length - lengths[1]) / 2;
			auto iVertex = 1u;
			if (!StateMap.test(StateFlag::BARSAT)) {
				OSequence[0] = SelectedPoint = CurrentFormVertices[1];
				SequenceIndex                = 1;
			}
			while ((iVertex < (VertexCount + 1)) && (length > lengths[iVertex])) {
				iVertex++;
			}
			const auto deltaA = lengths[iVertex] - length;
			const auto deltaB = length - lengths[iVertex - 1];
			if (deltaB > deltaA) {
				iVertex--;
			}
			si::satfn(lengths, 1, iVertex, VertexCount, iVertex);

			break;
		}
		if (CurrentFormGuidesCount) {
			si::satmf(lengths);
			break;
		}
		length /= 2;
		auto iVertex = 0;
		if (!StateMap.test(StateFlag::BARSAT) && !StateMap.test(StateFlag::FTHR)) {
			OSequence[0] = SelectedPoint = CurrentFormVertices[0];
			SequenceIndex                = 1;
		}
		while (length > lengths[iVertex]) {
			iVertex++;
		}
		const auto deltaA = lengths[iVertex] - length;
		const auto deltaB = length - lengths[static_cast<size_t>(iVertex) - 1u];
		if (deltaB > deltaA) {
			iVertex--;
		}
		si::satfn(lengths, 0, iVertex, VertexCount, iVertex);
	} while (false);

	LineSpacing = savedSpacing;
}

void satin::satfix() {
	const auto vertexCount = gsl::narrow<unsigned int>(TempPolygon->size());
	auto&      form        = (*FormList).back();
	auto       minSize     = 1u;
	if (form.type == FRMFPOLY) {
		minSize = 2u;
	}
	if (TempPolygon->size() > minSize) {
		form.vertices = thred::adflt(vertexCount);
		for (auto iVertex = 0u; iVertex < vertexCount; iVertex++) {
			form.vertices[iVertex] = (*TempPolygon)[iVertex];
		}
		TempPolygon->clear();
		form.vertexCount = vertexCount;
		form::frmout((*FormList).size() - 1);
		form.satinGuideCount = 0;
		FormIndex++;
		StateMap.set(StateFlag::INIT);
	}
	else {
		TempPolygon->clear();
		(*FormList).pop_back();
	}
	StateMap.reset(StateFlag::SHOSAT);
	StateMap.set(StateFlag::RESTCH);
}

void satin::dusat() {
	const auto  vertexCount = TempPolygon->size();
	auto&       formLines   = *FormLines;
	const auto* line        = &formLines[vertexCount - 1];

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
	formLines.resize(vertexCount + 1);
	formLines[vertexCount].x = Msg.pt.x - StitchWindowOrigin.x;
	formLines[vertexCount].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOSAT);
	satin::dusat();
}

void satin::satpnt0() {
	thred::px2stch();
	auto& formLines = *FormLines;
	formLines.clear();
	formLines.emplace_back(POINT{ Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y });
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

void satin::internal::filinsbw(std::vector<dPOINT>& satinBackup, const dPOINT& point, unsigned& satinBackupIndex) {
	satinBackup[satinBackupIndex++] = point;
	satinBackupIndex &= (satinBackup.size() - 1);
	form::filinsb(point);
}

void satin::internal::sbfn(const std::vector<fPOINT>& insidePoints, unsigned int start, unsigned int finish) {
	auto& outsidePoints = *OutsidePoints;
	auto  satinBackup   = std::vector<dPOINT>(8); // backup stitches in satin fills
	auto  innerDelta
	    = dPOINT{ (insidePoints[finish].x - insidePoints[start].x), (insidePoints[finish].y - insidePoints[start].y) };
	auto innerLength = hypot(innerDelta.x, innerDelta.y);
	auto outerDelta
	    = dPOINT{ (outsidePoints[finish].x - outsidePoints[start].x), (outsidePoints[finish].y - outsidePoints[start].y) };
	auto outerLength = hypot(outerDelta.x, outerDelta.y);
	auto innerPoint  = dPOINT{ insidePoints[start].x, insidePoints[start].y };
	auto outerPoint  = dPOINT{ outsidePoints[start].x, outsidePoints[start].y };

	auto innerFlag = false;
	auto outerFlag = false;
	auto count     = 0u;

	if (!StateMap.testAndSet(StateFlag::SAT1)) {
		SelectedPoint = insidePoints[start];
	}
	for (auto& sb : satinBackup) {
		sb.x = 1e12;
		sb.y = 1e12;
	}
	if (outerLength > innerLength) {
		count     = gsl::narrow<unsigned int>(std::round(outerLength / LineSpacing));
		innerFlag = true;

		dPOINT intersection = {};
		if (form::linx(insidePoints, start, finish, intersection)) {
			innerDelta.x = 0.0;
			innerDelta.y = 0.0;
			innerLength  = 0;
			innerPoint   = intersection;
		}
	}
	else {
		count     = gsl::narrow<unsigned int>(std::round(innerLength / LineSpacing));
		outerFlag = true;

		dPOINT intersection = {};
		if (form::linx(insidePoints, start, finish, intersection)) {
			outerDelta.x = 0.0;
			outerDelta.y = 0.0;
			outerLength  = 0;
			outerPoint   = intersection;
		}
	}
	if (!count) {
		count = 1;
	}
	if (form::chkmax(count, SequenceIndex)) {
		return;
	}
	const auto innerStep        = dPOINT{ innerDelta.x / count, innerDelta.y / count };
	const auto outerStep        = dPOINT{ outerDelta.x / count, outerDelta.y / count };
	auto       satinBackupIndex = 0u;
	for (auto iStep = 0u; iStep < count; iStep++) {
		innerPoint.x += innerStep.x;
		innerPoint.y += innerStep.y;
		outerPoint.x += outerStep.x;
		outerPoint.y += outerStep.y;
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			if (innerFlag) {
				const auto offsetDelta  = dPOINT{ innerPoint.x - SelectedPoint.x, innerPoint.y - SelectedPoint.y };
				const auto offsetLength = hypot(offsetDelta.x, offsetDelta.y);
				auto       offsetCount  = gsl::narrow<unsigned int>(std::round(offsetLength / LineSpacing));
				const auto offsetStep   = dPOINT{ offsetDelta.x / offsetCount, offsetDelta.y / offsetCount };
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
				const auto offsetDelta  = dPOINT{ outerPoint.x - SelectedPoint.x, outerPoint.y - SelectedPoint.y };
				const auto offsetLength = hypot(offsetDelta.x, offsetDelta.y);
				auto       offsetCount  = gsl::narrow<unsigned int>(std::round(offsetLength / LineSpacing));
				const auto offsetStep   = dPOINT{ offsetDelta.x / offsetCount, offsetDelta.y / offsetCount };
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

void satin::internal::sfn(unsigned int startVertex) {
	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		const auto nextVertex = form::nxt(startVertex);
		si::sbfn(*InsidePoints, startVertex, nextVertex);
		startVertex = nextVertex;
	}
	OSequence[0] = OSequence[SequenceIndex - 1];
	if (SequenceIndex > MAXITEMS - 2) {
		SequenceIndex = MAXITEMS - 2;
	}
}

void satin::satzum() {
	StateMap.reset(StateFlag::SHOSAT);
	thred::duzrat();
	const auto vertexCount = gsl::narrow<unsigned int>(TempPolygon->size());
	VertexCount            = vertexCount;
	form::frmlin(*TempPolygon);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	PolylineInt(StitchWindowMemDC, FormLines->data(), vertexCount);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	satin::drwsat();
}

void satin::satout(double satinWidth) {
	if (VertexCount) {
		form::duangs();
		OutsidePointList->resize(VertexCount);
		InsidePointList->resize(VertexCount);
		OutsidePoints = OutsidePointList;
		InsidePoints  = InsidePointList;
		for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
			si::outfn(iVertex, iVertex + 1, 0.1);
		}
		auto count = 0u;
		for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
			if (form::cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y)) {
				count++;
			}
		}
		satinWidth /= 2;
		for (auto iVertex = 0u; iVertex < VertexCount - 1; iVertex++) {
			si::outfn(iVertex, iVertex + 1, satinWidth);
		}
		si::outfn(VertexCount - 1, 0, satinWidth);
		StateMap.reset(StateFlag::INDIR);
		if (count<VertexCount>> 1) {
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
	SequenceIndex = 1;
	if (SelectedForm->edgeType & EGUND) {
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

void satin::internal::outfn(unsigned start, unsigned finish, double satinWidth) {
	auto xOffset = 0.0;
	auto yOffset = 0.0;

	if (fabs((*FormAngles)[start]) < TINY && fabs((*FormAngles)[finish]) < TINY) {
		xOffset = 0.0;
		yOffset = satinWidth;
	}
	else {
		constexpr auto SATHRESH = 10.0;

		auto angle  = ((*FormAngles)[finish] - (*FormAngles)[start]) / 2.0;
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
	(*InsidePoints)[finish].x  = CurrentFormVertices[finish].x - xOffset;
	(*InsidePoints)[finish].y  = CurrentFormVertices[finish].y - yOffset;
	(*OutsidePoints)[finish].x = CurrentFormVertices[finish].x + xOffset;
	(*OutsidePoints)[finish].y = CurrentFormVertices[finish].y + yOffset;
}

bool satin::internal::chkbak(const std::vector<dPOINT>& satinBackup, const dPOINT& pnt) {
	const auto maxSB = satinBackup.size();
	for (auto iBackup = 0u; iBackup < maxSB; iBackup++) {
		const auto length = hypot(satinBackup[iBackup].x - pnt.x, satinBackup[iBackup].y - pnt.y);
		if (length < LineSpacing) {
			return true;
		}
	}
	return false;
}

SATCON* satin::adsatk(unsigned int count) noexcept {
	auto iSatinConnect = SatinGuideIndex;

	SatinGuideIndex += count;
	return &SatinGuides[iSatinConnect];
}

void satin::cpySat(const FRMHED& formHeader) {
	if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);

		auto       sourceStart = formHeader.satinOrAngle.guide;
		auto       sourceEnd   = sourceStart + SelectedForm->satinGuideCount;
		const auto destination = stdext::make_checked_array_iterator(SelectedForm->satinOrAngle.guide, 10000 - SatinGuideIndex);
		std::copy(sourceStart, sourceEnd, destination);
	}
}

unsigned int satin::getGuideSize() noexcept {
	return SatinGuideIndex;
}

void satin::clearGuideSize() noexcept {
	SatinGuideIndex = 0;
}

void satin::setGuideSize(unsigned int newGuideSize) noexcept {
	SatinGuideIndex = newGuideSize;
}

void satin::cpyTmpGuides(const std::vector<SATCONOUT>& inSatinGuides) {
	const auto destination = stdext::make_checked_array_iterator(
	    SatinGuides + SatinGuideIndex, (sizeof(SatinGuides) / sizeof(SatinGuides[0])) - SatinGuideIndex);
	std::copy(inSatinGuides.cbegin(), inSatinGuides.cend(), destination);
}

void satin::cpyUndoGuides(const BAKHED& undoData) {
	if (undoData.guideCount) {
		std::copy(undoData.guide, undoData.guide + undoData.guideCount, SatinGuides);
	}
	SatinGuideIndex = undoData.guideCount;
}
