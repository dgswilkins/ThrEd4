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
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
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

size_t   StartPoint;       // starting formOrigin for a satin stitch guide-line
unsigned SatinBackupIndex; // pointer for backup stitches in satin fills
unsigned SatinIndex;       // pointer to next satin form origin to enter

unsigned satin::internal::satind(const SATCON* const guide) noexcept {
	return guide - SatinGuides;
}

void satin::delsac(size_t formIndex) noexcept {
	size_t iForm  = 0;
	size_t source = 0, destination = 0;

	if (SatinGuideIndex) {
		if (FormList[formIndex].type == SAT && FormList[formIndex].satinGuideCount) {
			destination = satin::internal::satind(FormList[formIndex].satinOrAngle.guide);
			source      = destination + FormList[formIndex].satinGuideCount;
			while (source < SatinGuideIndex) {
				SatinGuides[destination++] = SatinGuides[source++];
			}
			for (iForm = formIndex + 1; iForm < FormIndex; iForm++) {
				if (FormList[iForm].type == SAT && FormList[iForm].satinGuideCount)
					FormList[iForm].satinOrAngle.guide -= FormList[formIndex].satinGuideCount;
			}
			SatinGuideIndex -= FormList[formIndex].satinGuideCount;
		}
	}
	FormList[formIndex].satinGuideCount = 0;
}

void satin::internal::sacspac(const SATCON* const startGuide, unsigned guideCount) noexcept {
	size_t iStartGuide = 0;
	size_t source = 0, destination = 0;

	iStartGuide = satin::internal::satind(startGuide);
	source      = SatinGuideIndex - 1;
	destination = SatinGuideIndex + guideCount - 1;
	while (source >= iStartGuide) {
		SatinGuides[destination--] = SatinGuides[source--];
	}
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			FormList[iForm].satinOrAngle.guide += guideCount;
	}
	SatinGuideIndex += guideCount;
}

SATCON* satin::internal::nusac(size_t formIndex, unsigned guideCount) noexcept {
	size_t guideIndex = 0;

	for (size_t iForm = 0u; iForm < formIndex; iForm++) {
		if (FormList[iForm].type == SAT)
			guideIndex += FormList[iForm].satinGuideCount;
	}
	FormList[formIndex].satinOrAngle.guide = &SatinGuides[guideIndex];
	satin::internal::sacspac(FormList[formIndex].satinOrAngle.guide, guideCount);
	return FormList[formIndex].satinOrAngle.guide;
}

void satin::spltsat(const SATCON& currentGuide) {
	// We are adding two additional vertices when splitting the form
	std::vector<fPOINT> vertexBuffer(VertexCount + 2);
	unsigned            iGuide = 0, iVertex = 0;
	size_t              oldLastVertex = 0, iNewVertex = 0, iOldVertex = 0;

	mvfrmsb(&FormList[FormIndex], &FormList[FormIndex - 1], FormIndex - ClosestFormToCursor);
	FormIndex++;
	if (ClosestFormToCursor < FormIndex - 2) {
		mvfltsb(&FormVertices[FormVertexIndex + 1],
		        &FormVertices[FormVertexIndex - 1],
		        FormVertexIndex - fltind(FormList[ClosestFormToCursor + 2].vertices));
	}
	FormVertexIndex += 2;
	for (auto iForm = ClosestFormToCursor + 2; iForm < FormIndex; iForm++)
		FormList[iForm].vertices += 2;
	iOldVertex    = 0;
	oldLastVertex = currentGuide.start + (VertexCount - currentGuide.finish) + 1;
	iNewVertex    = oldLastVertex + 1;
	for (iVertex = 0; iVertex < VertexCount; iVertex++) {
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
	for (iVertex = 0; iVertex < iNewVertex; iVertex++) {
		CurrentFormVertices[iVertex] = vertexBuffer[iVertex];
	}
	SelectedForm->vertexCount                     = iOldVertex;
	FormList[ClosestFormToCursor + 1].vertexCount = iNewVertex - iOldVertex;
	FormList[ClosestFormToCursor + 1].vertices    = &CurrentFormVertices[iOldVertex];
	frmout(ClosestFormToCursor);
	frmout(ClosestFormToCursor + 1);
	iOldVertex = currentGuide.start + 1 - currentGuide.finish;
	for (iGuide = 0; iGuide < ActivePointIndex; iGuide++)
		SelectedForm->satinOrAngle.guide[iGuide].finish += iOldVertex;
	if (SelectedForm->wordParam)
		SelectedForm->wordParam = currentGuide.start;
	iOldVertex = iGuide + 1;
	while (iGuide < gsl::narrow<unsigned>(SelectedForm->satinGuideCount)) {
		SelectedForm->satinOrAngle.guide[iGuide].start -= (currentGuide.start - 1);
		SelectedForm->satinOrAngle.guide[iGuide].finish -= (currentGuide.start - 1);
		iGuide++;
	}
	if (FormList[ClosestFormToCursor + 1].wordParam)
		FormList[ClosestFormToCursor + 1].wordParam -= (currentGuide.start - 1);
	auto       sourceStart = SelectedForm->satinOrAngle.guide + iOldVertex;
	auto       sourceRange = SatinGuideIndex - satin::internal::satind(&SelectedForm->satinOrAngle.guide[iOldVertex]);
	auto       sourceEnd   = sourceStart + sourceRange;
	const auto destination = stdext::make_checked_array_iterator((sourceStart - 1), sourceRange);
	std::copy(sourceStart, sourceEnd, destination);
	SatinGuideIndex--;
	FormList[ClosestFormToCursor + 1].satinOrAngle.guide = &SelectedForm->satinOrAngle.guide[ActivePointIndex];
	FormList[ClosestFormToCursor + 1].satinGuideCount    = SelectedForm->satinGuideCount - ActivePointIndex - 1;
	SelectedForm->satinGuideCount                        = ActivePointIndex;
	for (auto iForm = ClosestFormToCursor + 2; iForm < FormIndex; iForm++)
		FormList[iForm].satinOrAngle.guide--;
	if (clip::iseclp(ClosestFormToCursor)) {
		clpspac(SelectedForm->borderClipData, SelectedForm->clipEntries);
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
			FormList[iForm].borderClipData += SelectedForm->clipEntries;
	}
	stchadj();
}

void satin::internal::satclos() {
	unsigned iVertex           = 0;
	size_t   closestVertex     = StartPoint;
	size_t   initialGuideCount = SelectedForm->satinGuideCount;
	double   deltaX = 0.0, deltaY = 0.0, length = 0.0, minimumLength = 1e99;

	uninsf();
	px2stch();
	for (iVertex = 0; iVertex < SelectedForm->vertexCount; iVertex++) {
		deltaX = SelectedPoint.x - SelectedForm->vertices[iVertex].x;
		deltaY = SelectedPoint.y - SelectedForm->vertices[iVertex].y;
		length = hypot(deltaX, deltaY);
		if (length < minimumLength) {
			minimumLength         = length;
			ClosestVertexToCursor = iVertex;
		}
	}
	StateMap.reset(StateFlag::SATCNKT);
	if (SelectedForm->type == FRMLINE) {
		SelectedForm->fillType = CONTF;
		closestVertex          = ClosestVertexToCursor;
		if (StartPoint > closestVertex) {
			std::swap(closestVertex, StartPoint);
		}
		if (!StartPoint)
			StartPoint++;
		if (StartPoint == SelectedForm->vertexCount - 2
		    && closestVertex == gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1) {
			StartPoint    = 1;
			closestVertex = SelectedForm->vertexCount - 2;
		}
		if (closestVertex >= gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2) {
			closestVertex = SelectedForm->vertexCount - 2;
			if (StartPoint >= gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2)
				StartPoint = SelectedForm->vertexCount - 2;
		}
		if (closestVertex - StartPoint < 2) {
			closestVertex = StartPoint + 2;
			if (closestVertex > gsl::narrow<unsigned>(SelectedForm->vertexCount) - 2) {
				closestVertex = closestVertex - SelectedForm->vertexCount - 2;
				closestVertex -= closestVertex;
				StartPoint -= closestVertex;
			}
		}
		SelectedForm->angleOrClipData.guide.start  = StartPoint;
		SelectedForm->angleOrClipData.guide.finish = closestVertex;
	}
	else {
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
			if (SelectedForm->attribute & FRMEND)
				SelectedForm->wordParam = closestVertex;
			else {
				if (closestVertex)
					rotfrm(closestVertex);
				SelectedForm->attribute |= FRMEND;
			}
			satin::satadj();
		}
		else {
			if (CurrentFormGuidesCount) {
				satin::internal::sacspac(&SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount], 1);
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount].start    = closestVertex;
				SelectedForm->satinOrAngle.guide[SelectedForm->satinGuideCount++].finish = ClosestVertexToCursor;
				satin::satadj();
			}
			else {
				SelectedForm->satinOrAngle.guide                           = satin::internal::nusac(ClosestFormToCursor, 1);
				SelectedForm->satinOrAngle.guide[initialGuideCount].start  = closestVertex;
				SelectedForm->satinOrAngle.guide[initialGuideCount].finish = ClosestVertexToCursor;
				SelectedForm->satinGuideCount                              = 1;
			}
		}
	}
}

void satin::satknkt() {
	satin::internal::satclos();
	fvars(ClosestFormToCursor);
	refil();
	StateMap.set(StateFlag::RESTCH);
}

bool satin::internal::satselfn() noexcept {
	unsigned iForm = 0, iVertex = 0, layerCode = 0;
	double   length = 0, minimumLength = 1e99;
	double   deltaX = 0.0, deltaY = 0.0;

	px2stch();
	for (iForm = 0; iForm < FormIndex; iForm++) {
		layerCode = (FormList[iForm].attribute & FRMLMSK) >> 1;
		if (!ActiveLayer || !layerCode || layerCode == ActiveLayer) {
			for (iVertex = 0; iVertex < FormList[iForm].vertexCount; iVertex++) {
				deltaX = SelectedPoint.x - FormList[iForm].vertices[iVertex].x;
				deltaY = SelectedPoint.y - FormList[iForm].vertices[iVertex].y;
				length = hypot(deltaX, deltaY);
				if (length < minimumLength) {
					minimumLength         = length;
					ClosestFormToCursor   = iForm;
					ClosestVertexToCursor = iVertex;
				}
			}
		}
	}
	if (minimumLength < CLOSENUF)
		return 1;
	else
		return 0;
}

void satin::satsel() {
	if (satin::internal::satselfn()) {
		fvars(ClosestFormToCursor);
		duzrat();
		StartPoint = ClosestVertexToCursor;
		sfCor2px(SelectedForm->vertices[ClosestVertexToCursor], FormLines[0]);
		StateMap.reset(StateFlag::SHOCON);
		StateMap.set(StateFlag::SATCNKT);
		if (SelectedForm->type == FRMFPOLY)
			SelectedForm->type = SAT;
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
	fvars(ClosestFormToCursor);

	unsigned            iDestination = 0;
	size_t              iVertex = 0, iReverse = 0, iForward = 0, iGuide = 0;
	std::vector<SATCON> interiorGuides(CurrentFormGuidesCount);
	SATCON*             sourceGuide      = nullptr;
	SATCON*             destinationGuide = nullptr;
	size_t              savedGuideCount  = SelectedForm->satinGuideCount;
	ExtendedBitSet<>    satinMap(VertexCount);

	// ensure all guide endpoints are on valid vertices
	for (iGuide = 0u; iGuide < SelectedForm->satinGuideCount; iGuide++) {
		auto endCount = gsl::narrow<unsigned short>(VertexCount - 1);
		if (CurrentFormGuides[iGuide].finish > endCount)
			CurrentFormGuides[iGuide].finish = endCount;
		if (CurrentFormGuides[iGuide].start > endCount)
			CurrentFormGuides[iGuide].start = endCount;
	}

	// remove any guides of 0 length
	iDestination = 0;
	for (auto iSource = 0u; iSource < CurrentFormGuidesCount; iSource++) {
		if (CurrentFormGuides[iSource].start != CurrentFormGuides[iSource].finish) {
			interiorGuides[iDestination++] = CurrentFormGuides[iSource];
		}
	}
	if (CurrentFormGuidesCount != iDestination) {
		OutputDebugString(fmt::format(L"Removed {} zero distance guides\n", (CurrentFormGuidesCount - iDestination)).c_str());
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = iDestination;
		satin::internal::satcpy(CurrentFormGuides, interiorGuides, iDestination);
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
			satinMap.set(SatinEndGuide + 1);
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
			satin::internal::satcpy(CurrentFormGuides, interiorGuides, iDestination);
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
				satin::internal::satcpy(CurrentFormGuides, interiorGuides, iDestination);
			}
		}
	}

	if (CurrentFormGuidesCount) {
		satinMap.reset();
		for (iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			iForward = CurrentFormGuides[iGuide].start;
			if (iForward > gsl::narrow<unsigned>(SatinEndGuide) - 1)
				iForward = SatinEndGuide - 1;
			if (satinMap.test_set(iForward)) {
				iReverse = iForward;
				if (iReverse)
					iReverse--;
				while (satinMap.test(iForward) && (iForward < (gsl::narrow<unsigned>(SatinEndGuide) - 1)))
					iForward++;
				while (iReverse && (satinMap.test(iReverse)))
					iReverse--;
				if (satinMap.test(iForward) && satinMap.test(iReverse))
					break;
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].start > CurrentFormGuides[iGuide].start - iReverse)
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
				else {
					if (!satinMap.test(iForward))
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
			}
		}
		iGuide = 0u;
		do {
			iVertex = satinMap.getFirst();
			if (iVertex < VertexCount)
				CurrentFormGuides[iGuide++].start = gsl::narrow<unsigned short>(iVertex);
		} while (iVertex < VertexCount);

		CurrentFormGuidesCount = SelectedForm->satinGuideCount = gsl::narrow<unsigned short>(iGuide);
		satinMap.reset();
		// Todo - are iForward and iReverse appropriate variable names below?
		for (iGuide = 0u; iGuide < CurrentFormGuidesCount; iGuide++) {
			iForward = iReverse = CurrentFormGuides[iGuide].finish;
			if (iForward > VertexCount - 1)
				iForward = VertexCount - 1;
			if (satinMap.test_set(iForward)) {
				if (iForward < VertexCount - 1)
					iForward++;
				if (iReverse > gsl::narrow<unsigned>(SatinEndGuide) + 1)
					iReverse--;
				while (satinMap.test(iForward) && iForward < VertexCount - 1)
					iForward++;
				while (iReverse > gsl::narrow<unsigned>(SatinEndGuide) - 1 && (satinMap.test(iReverse)))
					iReverse--;
				if (satinMap.test(iForward) && satinMap.test(iReverse))
					break;
				if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
					if (iForward - CurrentFormGuides[iGuide].finish > CurrentFormGuides[iGuide].finish - iReverse)
						satinMap.set(iReverse);
					else
						satinMap.set(iForward);
				}
				else {
					if (!satinMap.test(iForward))
						satinMap.set(iForward);
					else
						satinMap.set(iReverse);
				}
			}
		}
		iGuide = 0u;
		do {
			iReverse = satinMap.getLast();
			if (iReverse < VertexCount)
				CurrentFormGuides[iGuide++].finish = gsl::narrow<unsigned short>(iReverse);
		} while (iReverse < VertexCount);
		if (iGuide < CurrentFormGuidesCount)
			iGuide = CurrentFormGuidesCount;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount = gsl::narrow<unsigned short>(iGuide);
		if (SatinEndGuide) {
			auto end0 = gsl::narrow<unsigned short>(VertexCount - SatinEndGuide - 2);
			auto end1 = gsl::narrow<unsigned short>(SatinEndGuide - 2);
			if (CurrentFormGuidesCount > end0)
				CurrentFormGuidesCount = end0;
			if (CurrentFormGuidesCount > end1)
				CurrentFormGuidesCount = end1;
			SelectedForm->satinGuideCount = CurrentFormGuidesCount;
		}
	}
	if (SelectedForm->satinGuideCount < savedGuideCount) {
		iGuide      = savedGuideCount - CurrentFormGuidesCount;
		sourceGuide = destinationGuide = SelectedForm->satinOrAngle.guide;
		destinationGuide += SelectedForm->satinGuideCount;
		sourceGuide += savedGuideCount;
		std::copy(sourceGuide,
		          (&SatinGuides[SatinGuideIndex] + 1),
		          stdext::make_checked_array_iterator(destinationGuide, 10000 - SatinGuideIndex));
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			const auto formHeader = &FormList[iForm];
			if (formHeader->type == SAT)
				formHeader->satinOrAngle.guide -= iGuide;
		}
		SatinGuideIndex -= iGuide;
	}
}

void satin::delcon(unsigned GuideIndex) {
	// ToDo - Find a better way to calculate the offset into the SatinGuides array
	const unsigned iGuide = &SelectedForm->satinOrAngle.guide[GuideIndex] - SatinGuides;
	SATCON*        guide  = &SelectedForm->satinOrAngle.guide[GuideIndex];

	if (guide) {
		if (SatinGuideIndex > iGuide) {
			// Todo - use std::vector & member erase() ?
			std::copy(
			    guide + 1, guide + (SatinGuideIndex - iGuide + 1), stdext::make_checked_array_iterator(guide, SatinGuideIndex));
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			const auto formHeader = &FormList[iForm];
			if (formHeader->type == SAT && formHeader->satinGuideCount)
				formHeader->satinOrAngle.guide--;
		}
		if (ClosestVertexToCursor < SatinEndGuide)
			SatinEndGuide--;
		SelectedForm->satinGuideCount--;
		SatinGuideIndex--;
		CurrentFormGuidesCount = SelectedForm->satinGuideCount;
		if (SelectedForm->fillType == SATF)
			refil();
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
}

void satin::delspnt() {
	unsigned iGuide = 0;
	SATCON*  guide  = nullptr;
	fPOINT   vertex = {};

	fvars(ClosestFormToCursor);
	if (chkdel()) {
		StateMap.set(StateFlag::DELTO);
		frmdel();
		StateMap.reset(StateFlag::FRMPSEL);
		coltab();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	if (SelectedForm->type == SAT) {
		if (ClosestVertexToCursor < SelectedForm->wordParam)
			SelectedForm->wordParam--;
		if (SelectedForm->satinGuideCount) {
			guide = SelectedForm->satinOrAngle.guide;
			if (guide) {
				iGuide = 0;
				while (guide[iGuide].start != ClosestVertexToCursor && guide[iGuide].finish != ClosestVertexToCursor
				       && iGuide < SelectedForm->satinGuideCount)
					iGuide++;
				if (iGuide < SelectedForm->satinGuideCount
				    && (guide[iGuide].start == ClosestVertexToCursor || guide[iGuide].finish == ClosestVertexToCursor)) {
					while (iGuide < SelectedForm->satinGuideCount) {
						guide[iGuide] = guide[iGuide + 1];
						iGuide++;
					}
					SelectedForm->satinGuideCount--;
					SatinGuideIndex--;
					for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
						const auto formHeader = &FormList[iForm];
						if (formHeader->type == SAT && formHeader->satinGuideCount)
							formHeader->satinOrAngle.guide++;
					}
				}
				for (iGuide = 0; iGuide < SelectedForm->satinGuideCount; iGuide++) {
					if (guide[iGuide].start > ClosestVertexToCursor)
						guide[iGuide].start--;
					if (guide[iGuide].finish > ClosestVertexToCursor)
						guide[iGuide].finish--;
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
	fvars(ClosestFormToCursor);
	if (ClosestVertexToCursor > gsl::narrow<unsigned>(SelectedForm->vertexCount) - 1)
		ClosestVertexToCursor = SelectedForm->vertexCount - 1;
	StateMap.set(StateFlag::FRMPSEL);
	for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++)
		FormList[iForm].vertices--;
	ritfcor(CurrentFormVertices[ClosestVertexToCursor]);
	displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
	frmout(ClosestFormToCursor);
	vertex = SelectedForm->vertices[ClosestVertexToCursor];
	if (vertex.x < ZoomRect.left || vertex.x > ZoomRect.right || vertex.y < ZoomRect.bottom || vertex.y > ZoomRect.top)
		shft(vertex);
	refil();
}

void satin::internal::satsbrd() {
	clip::deleclp(ClosestFormToCursor);
	SelectedForm->edgeType = EDGEANGSAT;
	if (UserFlagMap.test(UserFlag::DUND))
		SelectedForm->edgeType |= EGUND;
	bsizpar();
	SelectedForm->borderSize  = BorderWidth;
	SelectedForm->edgeSpacing = LineSpacing / 2;
	SelectedForm->borderColor = ActiveColor;
	refilfn();
}

void satin::satbrd() {
	if (displayText::filmsgs(FML_ANGS))
		return;
	if (SelectedFormList->size()) {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satin::internal::satsbrd();
		}
		StateMap.set(StateFlag::INIT);
		coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			if (UserFlagMap.test(UserFlag::BLUNT))
				SelectedForm->attribute |= (SBLNT | FBLNT);
			else
				SelectedForm->attribute &= NOBLNT;
			satin::internal::satsbrd();
			StateMap.set(StateFlag::INIT);
			coltab();
			ritot(PCSHeader.stitchCount);
			StateMap.set(StateFlag::RESTCH);
		}
	}
}

void satin::internal::satends(unsigned isBlunt) {
	fPOINT step = {};

	if (isBlunt & SBLNT) {
		step.x = sin((*FormAngles)[0]) * HorizontalLength2 / 2;
		step.y = cos((*FormAngles)[0]) * HorizontalLength2 / 2;
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
		step.x = sin((*FormAngles)[VertexCount - 2]) * HorizontalLength2 / 2;
		step.y = cos((*FormAngles)[VertexCount - 2]) * HorizontalLength2 / 2;
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
	unsigned iNewVertex = 0, isBlunt = 0;
	size_t   savedFormIndex = 0;

	displayText::frm1pnt();
	if (StateMap.test(StateFlag::FORMSEL)) {
		fvars(ClosestFormToCursor);
		if (VertexCount > 2) {
			savdo();
			savedFormIndex = ClosestFormToCursor;
			satin::satout(BorderWidth);

			HorizontalLength2     = BorderWidth / 2;
			const auto formHeader = &FormList[FormIndex];
			if (formHeader) {
				frmclr(formHeader);
				iNewVertex                   = 0;
				formHeader->maxFillStitchLen = 9 * PFGRAN;
				formHeader->minFillStitchLen = MinStitchLength;
				MaxStitchLen                 = 9 * PFGRAN;
				if (SelectedForm->type == FRMLINE) {
					// Set blunt flags
					if (UserFlagMap.test(UserFlag::BLUNT))
						isBlunt = SBLNT | FBLNT;
					else
						isBlunt = 0;
					satin::internal::satends(isBlunt);
					formHeader->vertices                 = adflt(VertexCount << 1);
					formHeader->vertices[0].x            = (*OutsidePoints)[0].x;
					formHeader->vertices[iNewVertex++].y = (*OutsidePoints)[0].y;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						formHeader->vertices[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						formHeader->vertices[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				else {
					formHeader->vertices                 = adflt((VertexCount << 1) + 2);
					formHeader->vertices[0].x            = (*OutsidePoints)[0].x;
					formHeader->vertices[iNewVertex++].y = (*OutsidePoints)[0].y;
					formHeader->underlayIndent           = IniFile.underlayIndent;
					for (auto iVertex = 0u; iVertex < VertexCount; iVertex++) {
						formHeader->vertices[iNewVertex++] = (*InsidePoints)[iVertex];
					}
					formHeader->vertices[iNewVertex++] = (*InsidePoints)[0];
					formHeader->vertices[iNewVertex++] = (*OutsidePoints)[0];
					for (auto iVertex = VertexCount - 1; iVertex != 0; iVertex--) {
						formHeader->vertices[iNewVertex++] = (*OutsidePoints)[iVertex];
					}
				}
				formHeader->type                       = SAT;
				formHeader->fillColor                  = ActiveColor;
				formHeader->fillSpacing                = LineSpacing;
				formHeader->lengthOrCount.stitchLength = IniFile.maxStitchLength;
				formHeader->vertexCount                = iNewVertex;
				formHeader->attribute                  = 1;
				formHeader->wordParam                  = iNewVertex >> 1;
				formHeader->satinGuideCount            = formHeader->wordParam - 2;
				formHeader->satinOrAngle.guide         = adsatk(formHeader->satinGuideCount);
				if (StateMap.test(StateFlag::CNV2FTH)) {
					formHeader->fillType                       = FTHF;
					formHeader->fillInfo.feather.ratio         = IniFile.featherRatio;
					formHeader->fillInfo.feather.upCount       = IniFile.featherUpCount;
					formHeader->fillInfo.feather.downCount     = IniFile.featherDownCount;
					formHeader->fillInfo.feather.fillType      = IniFile.featherFillType;
					formHeader->fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
					// ToDo - should this be ORed in to keep other attributes?
					formHeader->extendedAttribute      = IniFile.featherType;
					formHeader->fillInfo.feather.count = IniFile.featherCount;
					formHeader->fillInfo.feather.color = (ActiveColor + 1) & COLMSK;
				}
				else
					formHeader->fillType = SATF;
				for (auto iGuide = 0u; iGuide < formHeader->satinGuideCount; iGuide++) {
					formHeader->satinOrAngle.guide[iGuide].start  = iGuide + 2;
					formHeader->satinOrAngle.guide[iGuide].finish = formHeader->vertexCount - iGuide - 1;
				}
				FormIndex++;
				frmout(FormIndex - 1);
				ClosestFormToCursor = FormIndex - 1;
				refilfn();
				ClosestFormToCursor = savedFormIndex;
				StateMap.set(StateFlag::DELTO);
				frmdel();
				ClosestFormToCursor = FormIndex - 1;
				StateMap.set(StateFlag::FORMSEL);
				StateMap.set(StateFlag::INIT);
				StateMap.set(StateFlag::RESTCH);
			}
			else {
				throw;
			}
		}
		else
			displayText::tabmsg(IDS_FRM2);
	}
	else
		displayText::shoseln(IDS_FRM1MSG, IDS_CONVRIB);
}

void satin::slbrd() {
	const double spacing = LineSpacing;

	SequenceIndex = 0;
	if (SelectedForm->edgeType & EGUND) {
		HorizontalLength2 = SelectedForm->borderSize * URAT;
		satin::satout(HorizontalLength2);
		satin::internal::satends(SelectedForm->attribute);
		StateMap.reset(StateFlag::SAT1);
		StateMap.reset(StateFlag::FILDIR);
		LineSpacing = USPAC;
		for (size_t iVertex = 0u; iVertex < SelectedForm->vertexCount - 1; iVertex++)
			satin::internal::sbfn(*InsidePoints, iVertex, iVertex + 1);
		StateMap.flip(StateFlag::FILDIR);
		for (size_t iVertex = SelectedForm->vertexCount - 1; iVertex != 0; iVertex--)
			satin::internal::sbfn(*InsidePoints, iVertex, iVertex - 1);
	}
	HorizontalLength2 = SelectedForm->borderSize;
	satin::satout(HorizontalLength2);
	satin::internal::satends(SelectedForm->attribute);
	LineSpacing = SelectedForm->edgeSpacing;
	StateMap.reset(StateFlag::SAT1);
	for (size_t iVertex = 0; iVertex < SelectedForm->vertexCount - 1; iVertex++)
		satin::internal::sbfn(*InsidePoints, iVertex, iVertex + 1);
	LineSpacing = spacing;
}

void satin::internal::satfn(const std::vector<double>& lengths,
                      size_t                     line1Start,
                      size_t                     line1End,
                      size_t                     line2Start,
                      size_t                     line2End) {
	size_t   line1Next = 0, line2Previous = 0;
	unsigned stitchCount = 0;
	unsigned iSegment = 0, line1Count = 0, line2Count = 0;
	unsigned segmentStitchCount = 0;
	size_t   iLine1Vertex       = 0;
	size_t   iNextVertex = 0, iVertex = 0, iLine2Vertex = 0;
	unsigned iLine1Count = 0, iLine2Count = 0;
	size_t   line1Segments = 0, line2Segments = 0;
	double   line1Length = 0.0, line2Length = 0.0;
	dPOINT   line1Point = {}, line2Point = {}, line1Delta = {}, line2Delta = {};
	dPOINT   line1Step = {}, line2Step = {};

	if (line1Start != line1End && line2Start != line2End) {
		if (!StateMap.testAndSet(StateFlag::SAT1)) {
			if (StateMap.test(StateFlag::FTHR)) {
				BSequence[SequenceIndex].attribute = 0;
				ritseq1(line1Start % VertexCount);
			}
			else {
				if (StateMap.test(StateFlag::BARSAT)) {
					ritseq1(line1Start % VertexCount);
					ritseq1(line2Start % VertexCount);
				}
				else {
					SelectedPoint              = CurrentFormVertices[line1Start];
					OSequence[SequenceIndex++] = SelectedPoint;
				}
			}
		}
		line1Length = lengths[line1End] - lengths[line1Start];
		line2Length = lengths[line2Start] - lengths[line2End];
		if (fabs(line1Length) > fabs(line2Length))
			stitchCount = fabs(line2Length) / LineSpacing;
		else
			stitchCount = fabs(line1Length) / LineSpacing;
		line1Segments = ((line1End > line1Start) ? (line1End - line1Start) : (line1Start - line1End));
		line2Segments = ((line2Start > line2End) ? (line2Start - line2End) : (line2End - line2Start));
		std::vector<unsigned> line1StitchCounts;
		line1StitchCounts.reserve(line1Segments);
		std::vector<unsigned> line2StitchCounts;
		line2StitchCounts.reserve(line2Segments);
		iVertex            = line1Start;
		segmentStitchCount = 0;
		for (iSegment = 0; iSegment < line1Segments - 1; iSegment++) {
			iNextVertex        = nxt(iVertex);
			const unsigned val = ((lengths[iNextVertex] - lengths[iVertex]) / line1Length) * stitchCount + 0.5;
			line1StitchCounts.push_back(val);
			segmentStitchCount += val;
			iVertex = nxt(iVertex);
		}
		line1StitchCounts.push_back(stitchCount - segmentStitchCount);
		iNextVertex        = line2Start;
		iVertex            = prv(iNextVertex);
		iSegment           = 0;
		segmentStitchCount = 0;
		while (iVertex > line2End) {
			const unsigned val = ((lengths[iNextVertex] - lengths[iVertex]) / line2Length) * stitchCount + 0.5;
			line2StitchCounts.push_back(val);
			segmentStitchCount += val;
			iNextVertex = prv(iNextVertex);
			iVertex     = prv(iNextVertex);
		}
		line2StitchCounts.push_back(stitchCount - segmentStitchCount);
		line1Point.x  = CurrentFormVertices[line1Start].x;
		line1Point.y  = CurrentFormVertices[line1Start].y;
		line1Next     = nxt(line1Start);
		line2Previous = prv(line2Start);
		line1Count    = line1StitchCounts[iLine1Count++];
		line2Count    = line2StitchCounts[iLine2Count++];
		iLine1Vertex  = line1Start;
		iLine2Vertex  = line2Start;
		line1Delta.x  = CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x;
		line1Delta.y  = CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y;
		if (iLine2Vertex == VertexCount) {
			line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[0].x;
			line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[0].y;
			line2Point   = CurrentFormVertices[0];
		}
		else {
			line2Delta.x = CurrentFormVertices[line2Previous].x - CurrentFormVertices[iLine2Vertex].x;
			line2Delta.y = CurrentFormVertices[line2Previous].y - CurrentFormVertices[iLine2Vertex].y;
			line2Point   = CurrentFormVertices[iLine2Vertex];
		}
		iLine1Vertex = nxt(iLine1Vertex);
		iLine2Vertex = prv(iLine2Vertex);
		line1Step.x  = line1Delta.x / line1Count;
		line1Step.y  = line1Delta.y / line1Count;
		line2Step.x  = line2Delta.x / line2Count;
		line2Step.y  = line2Delta.y / line2Count;
		bool flag    = false;
		do {
			flag = false;
			if (StateMap.test(StateFlag::FTHR)) {
				while (line1Count && line2Count) {
					line1Point.x += line1Step.x;
					line1Point.y += line1Step.y;
					line2Point.x += line2Step.x;
					line2Point.y += line2Step.y;
					if (StateMap.testAndFlip(StateFlag::FILDIR)) {
						BSequence[SequenceIndex].attribute = 0;
						BSequence[SequenceIndex].x         = line1Point.x;
						BSequence[SequenceIndex++].y       = line1Point.y;
					}
					else {
						BSequence[SequenceIndex].attribute = 1;
						BSequence[SequenceIndex].x         = line2Point.x;
						BSequence[SequenceIndex++].y       = line2Point.y;
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
							BSequence[SequenceIndex].attribute = 0;
							BSequence[SequenceIndex].x         = line1Point.x;
							BSequence[SequenceIndex++].y       = line1Point.y;
							BSequence[SequenceIndex].attribute = 1;
							BSequence[SequenceIndex].x         = line2Point.x;
							BSequence[SequenceIndex++].y       = line2Point.y;
						}
						else {
							BSequence[SequenceIndex].attribute = 2;
							BSequence[SequenceIndex].x         = line2Point.x;
							BSequence[SequenceIndex++].y       = line2Point.y;
							BSequence[SequenceIndex].attribute = 3;
							BSequence[SequenceIndex].x         = line1Point.x;
							BSequence[SequenceIndex++].y       = line1Point.y;
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
							if (UserFlagMap.test(UserFlag::SQRFIL))
								filinu(line2Point);
							filin(line1Point);
						}
						else {
							if (UserFlagMap.test(UserFlag::SQRFIL))
								filinu(line1Point);
							filin(line2Point);
						}
						line1Count--;
						line2Count--;
					}
				}
			}
			if ((iLine1Count < line1Segments || iLine2Count < line2Segments)) {
				if (!line1Count && iLine1Count < line1StitchCounts.size()) {
					line1Count   = line1StitchCounts[iLine1Count++];
					line1Next    = nxt(iLine1Vertex);
					line1Delta.x = CurrentFormVertices[line1Next].x - CurrentFormVertices[iLine1Vertex].x;
					line1Delta.y = CurrentFormVertices[line1Next].y - CurrentFormVertices[iLine1Vertex].y;
					iLine1Vertex = nxt(iLine1Vertex);
					line1Step.x  = line1Delta.x / line1Count;
					line1Step.y  = line1Delta.y / line1Count;
				}

				if (!line2Count && iLine2Count < line2StitchCounts.size()) {
					line2Count    = line2StitchCounts[iLine2Count++];
					line2Previous = prv(iLine2Vertex);
					line2Delta.x  = CurrentFormVertices[line2Previous].x - CurrentFormVertices[iLine2Vertex].x;
					line2Delta.y  = CurrentFormVertices[line2Previous].y - CurrentFormVertices[iLine2Vertex].y;
					iLine2Vertex  = prv(iLine2Vertex);
					line2Step.x   = line2Delta.x / line2Count;
					line2Step.y   = line2Delta.y / line2Count;
				}
				if ((line1Count || line2Count) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
					flag = true;
				}
			}
		} while (flag);
	}
}

void satin::internal::satmf(const std::vector<double>& lengths) {
	unsigned iGuide  = 0;
	size_t   iVertex = 0;
	double   length = 0.0, deltaX = 0.0, deltaY = 0.0;

	if (SelectedForm->attribute & FRMEND)
		iGuide = 1;
	satin::internal::satfn(lengths, iGuide, CurrentFormGuides[0].start, VertexCount, CurrentFormGuides[0].finish);
	for (iGuide = 0; iGuide < gsl::narrow<unsigned>(CurrentFormGuidesCount) - 1; iGuide++)
		satin::internal::satfn(lengths,
		                 CurrentFormGuides[iGuide].start,
		                 CurrentFormGuides[iGuide + 1].start,
		                 CurrentFormGuides[iGuide].finish,
		                 CurrentFormGuides[iGuide + 1].finish);
	if (SatinEndGuide)
		satin::internal::satfn(
		    lengths, CurrentFormGuides[iGuide].start, SatinEndGuide, CurrentFormGuides[iGuide].finish, SatinEndGuide + 1);
	else {
		if (CurrentFormGuides[iGuide].finish - CurrentFormGuides[iGuide].start > 2) {
			length = (lengths[CurrentFormGuides[iGuide].finish] - lengths[CurrentFormGuides[iGuide].start]) / 2
			         + lengths[CurrentFormGuides[iGuide].start];
			iVertex = CurrentFormGuides[iGuide].start;
			while (length > lengths[iVertex])
				iVertex++;
			deltaX = lengths[iVertex] - length;
			deltaY = length - lengths[iVertex - 1];
			if (deltaY > deltaX)
				iVertex--;
			satin::internal::satfn(lengths, CurrentFormGuides[iGuide].start, iVertex, CurrentFormGuides[iGuide].finish, iVertex);
		}
		else
			satin::internal::satfn(lengths,
			                 CurrentFormGuides[iGuide].start,
			                 CurrentFormGuides[iGuide].start + 1,
			                 CurrentFormGuides[iGuide].finish,
			                 CurrentFormGuides[iGuide].start + 1);
	}
}

void satin::satfil() {
	unsigned     iVertex = 0;
	double       length = 0.0, deltaX = 0.0, deltaY = 0.0;
	const double spacing = LineSpacing;

	fvars(ClosestFormToCursor);
	satin::satadj();
	LineSpacing /= 2;
	SequenceIndex = 0;
	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SelectedForm->fillType = SATF;
	std::vector<double> lengths;
	lengths.reserve(VertexCount + 1);
	length = 0;
	lengths.push_back(length);
	for (iVertex = 1; iVertex < VertexCount; iVertex++) {
		deltaX = CurrentFormVertices[iVertex].x - CurrentFormVertices[iVertex - 1].x;
		deltaY = CurrentFormVertices[iVertex].y - CurrentFormVertices[iVertex - 1].y;
		length += hypot(deltaX, deltaY);
		lengths.push_back(length);
	}
	deltaX = CurrentFormVertices[0].x - CurrentFormVertices[iVertex - 1].x;
	deltaY = CurrentFormVertices[0].y - CurrentFormVertices[iVertex - 1].y;
	length += hypot(deltaX, deltaY);
	lengths.push_back(length);
	do {
		if (SatinEndGuide) {
			if (CurrentFormGuidesCount) {
				satin::internal::satmf(lengths);
				break;
			}
			else {
				satin::internal::satfn(lengths, 1, SatinEndGuide, VertexCount, SatinEndGuide + 1);
				break;
			}
		}
		if (SelectedForm->attribute & FRMEND) {
			if (CurrentFormGuidesCount) {
				satin::internal::satmf(lengths);
				break;
			}
			else {
				if (VertexCount == 3 && FormList[ClosestFormToCursor].attribute & 1) {
					satin::internal::satfn(lengths, 2, 3, 2, 1);
					break;
					;
				}
				else {
					length  = (length - lengths[1]) / 2;
					iVertex = 1;
					if (!StateMap.test(StateFlag::BARSAT)) {
						OSequence[0] = SelectedPoint = CurrentFormVertices[1];
						SequenceIndex                = 1;
					}
					while ((length > lengths[iVertex]) && (iVertex < (VertexCount + 1))) {
						iVertex++;
					}
					deltaX = lengths[iVertex] - length;
					deltaY = length - lengths[iVertex - 1];
					if (deltaY > deltaX) {
						iVertex--;
					}
					satin::internal::satfn(lengths, 1, iVertex, VertexCount, iVertex);
				}
				break;
			}
		}
		if (CurrentFormGuidesCount) {
			satin::internal::satmf(lengths);
			break;
		}
		length /= 2;
		iVertex = 0;
		if (!StateMap.test(StateFlag::BARSAT) && !StateMap.test(StateFlag::FTHR)) {
			OSequence[0] = SelectedPoint = CurrentFormVertices[0];
			SequenceIndex                = 1;
		}
		while (length > lengths[iVertex]) {
			iVertex++;
		}
		deltaX = lengths[iVertex] - length;
		deltaY = length - lengths[iVertex - 1];
		if (deltaY > deltaX)
			iVertex--;
		satin::internal::satfn(lengths, 0, iVertex, VertexCount, iVertex);
	} while (false);

	LineSpacing = spacing;
}

void satin::satfix() {
	unsigned iVertex = 0;

	if (SatinIndex > 1) {
		FormList[FormIndex].vertices = adflt(SatinIndex);
		for (iVertex = 0; iVertex < SatinIndex; iVertex++) {
			FormList[FormIndex].vertices[iVertex] = (*TempPolygon)[iVertex];
		}
		TempPolygon->clear();
		FormList[FormIndex].vertexCount = SatinIndex;
		frmout(FormIndex);
		FormList[FormIndex].satinGuideCount = 0;
		FormIndex++;
		StateMap.set(StateFlag::INIT);
	}
	StateMap.reset(StateFlag::SHOSAT);
	StateMap.set(StateFlag::RESTCH);
}

void satin::dusat() noexcept {
	const POINT* line = &FormLines[SatinIndex - 1];

	SetROP2(StitchWindowDC, R2_XORPEN);
	SelectObject(StitchWindowDC, FormPen);
	Polyline(StitchWindowDC, line, 2);
	SetROP2(StitchWindowDC, R2_COPYPEN);
}

void satin::internal::unsat() {
	if (StateMap.testAndReset(StateFlag::SHOSAT))
		satin::dusat();
}

void satin::drwsat() {
	satin::internal::unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	StateMap.set(StateFlag::SHOSAT);
	satin::dusat();
}

void satin::satpnt0() {
	px2stch();
	FormLines[0].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[0].y = Msg.pt.y - StitchWindowOrigin.y;
	TempPolygon->push_back(SelectedPoint);
	SatinIndex = 1;
	StateMap.set(StateFlag::SATPNT);
}

void satin::satpnt1() {
	satin::internal::unsat();
	px2stch();
	FormLines[SatinIndex].x = Msg.pt.x - StitchWindowOrigin.x;
	FormLines[SatinIndex].y = Msg.pt.y - StitchWindowOrigin.y;
	satin::dusat();
	TempPolygon->push_back(SelectedPoint);
	SatinIndex++;
	StateMap.set(StateFlag::RESTCH);
}

void filinsbw(std::vector<dPOINT>& satinBackup, const dPOINT& point) {
	satinBackup[SatinBackupIndex++] = point;
	SatinBackupIndex &= (satinBackup.size() - 1);
	filinsb(point);
}

void satin::internal::sbfn(const std::vector<fPOINT>& insidePoints, size_t start, size_t finish) {
	std::vector<dPOINT> satinBackup(8); // backup stitches in satin fills
	dPOINT innerDelta  = { (insidePoints[finish].x - insidePoints[start].x), (insidePoints[finish].y - insidePoints[start].y) };
	double innerLength = hypot(innerDelta.x, innerDelta.y);
	dPOINT outerDelta
	    = { ((*OutsidePoints)[finish].x - (*OutsidePoints)[start].x), ((*OutsidePoints)[finish].y - (*OutsidePoints)[start].y) };
	double   outerLength = hypot(outerDelta.x, outerDelta.y);
	dPOINT   innerPoint  = { insidePoints[start].x, insidePoints[start].y };
	dPOINT   outerPoint  = { (*OutsidePoints)[start].x, (*OutsidePoints)[start].y };
	dPOINT   innerStep = {}, outerStep = {};
	dPOINT   offsetDelta = {}, offsetStep = {}, offset = {};
	dPOINT   intersection = {};
	double   offsetLength = 0.0;
	unsigned count = 0, innerFlag = 0, outerFlag = 0, offsetCount = 0, iStep = 0;
	unsigned intersectFlag = 0;

	if (!StateMap.testAndSet(StateFlag::SAT1)) {
		SelectedPoint = insidePoints[start];
	}
	SatinBackupIndex = 0;
	for (auto& sb : satinBackup) {
		sb = { 1e12f, 1e12f };
	}
	if (outerLength > innerLength) {
		count     = outerLength / LineSpacing;
		innerFlag = 1;
		if (linx(insidePoints, start, finish, intersection)) {
			intersectFlag = 1;
			innerDelta.x = innerDelta.y = innerLength = 0;
			innerPoint                                = intersection;
		}
	}
	else {
		count     = innerLength / LineSpacing;
		outerFlag = 1;
		if (linx(insidePoints, start, finish, intersection)) {
			intersectFlag = 1;
			outerDelta.x = outerDelta.y = outerLength = 0;
			outerPoint                                = intersection;
		}
	}
	if (!count)
		count = 1;
	if (chkmax(count, SequenceIndex))
		return;
	innerStep.x = innerDelta.x / count;
	innerStep.y = innerDelta.y / count;
	outerStep.x = outerDelta.x / count;
	outerStep.y = outerDelta.y / count;
	for (iStep = 0; iStep < count; iStep++) {
		innerPoint.x += innerStep.x;
		innerPoint.y += innerStep.y;
		outerPoint.x += outerStep.x;
		outerPoint.y += outerStep.y;
		if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			if (innerFlag) {
				offsetDelta.x = innerPoint.x - SelectedPoint.x;
				offsetDelta.y = innerPoint.y - SelectedPoint.y;
				offsetLength  = hypot(offsetDelta.x, offsetDelta.y);
				offsetCount   = offsetLength / LineSpacing;
				offsetStep.x  = offsetDelta.x / offsetCount;
				offsetStep.y  = offsetDelta.y / offsetCount;
				offset.x      = innerPoint.x;
				offset.y      = innerPoint.y;
				while (satin::internal::chkbak(satinBackup, offset)) {
					offset.x -= offsetStep.x;
					offset.y -= offsetStep.y;
				}
				filinsbw(satinBackup, offset);
			}
			else
				filinsb(innerPoint);
		}
		else {
			if (outerFlag) {
				offsetDelta.x = outerPoint.x - SelectedPoint.x;
				offsetDelta.y = outerPoint.y - SelectedPoint.y;
				offsetLength  = hypot(offsetDelta.x, offsetDelta.y);
				offsetCount   = offsetLength / LineSpacing;
				offsetStep.x  = offsetDelta.x / offsetCount;
				offsetStep.y  = offsetDelta.y / offsetCount;
				offset.x      = outerPoint.x;
				offset.y      = outerPoint.y;
				while (satin::internal::chkbak(satinBackup, offset)) {
					offset.x -= offsetStep.x;
					offset.y -= offsetStep.y;
				}
				filinsbw(satinBackup, offset);
			}
			else
				filinsb(outerPoint);
		}
	}
}

void satin::internal::sfn(size_t startVertex) {
	size_t nextVertex = 0;

	for (auto iVertex = 0u; iVertex < SelectedForm->vertexCount; iVertex++) {
		nextVertex = nxt(startVertex);
		satin::internal::sbfn(*InsidePoints, startVertex, nextVertex);
		startVertex = nextVertex;
	}
	OSequence[0] = OSequence[SequenceIndex - 1];
	if (SequenceIndex > MAXITEMS - 2)
		SequenceIndex = MAXITEMS - 2;
}

void satin::satzum() {
	StateMap.reset(StateFlag::SHOSAT);
	duzrat();
	VertexCount = SatinIndex;
	frmlin(*TempPolygon);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, FormPen);
	PolylineInt(StitchWindowMemDC, FormLines, SatinIndex);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	satin::drwsat();
}

void satin::satout(double satinWidth) {
	unsigned iVertex = 0;
	unsigned count   = 0;

	if (VertexCount) {
		duangs();
		OutsidePointList->resize(VertexCount);
		InsidePointList->resize(VertexCount);
		OutsidePoints = OutsidePointList;
		InsidePoints  = InsidePointList;
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			satin::internal::outfn(iVertex, iVertex + 1, 0.1);
		count = 0;
		for (iVertex = 0; iVertex < VertexCount; iVertex++) {
			if (cisin((*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y))
				count++;
		}
		satinWidth /= 2;
		for (iVertex = 0; iVertex < VertexCount - 1; iVertex++)
			satin::internal::outfn(iVertex, iVertex + 1, satinWidth);
		satin::internal::outfn(iVertex, 0, satinWidth);
		StateMap.reset(StateFlag::INDIR);
		if (count<VertexCount>> 1) {
			StateMap.set(StateFlag::INDIR);
			OutsidePoints = InsidePointList;
			InsidePoints  = OutsidePointList;
		}
	}
}

void satin::sbrd() {
	const double spacing = LineSpacing;
	const size_t start   = getlast();

	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	SequenceIndex = 1;
	if (SelectedForm->edgeType & EGUND) {
		LineSpacing = USPAC;
		satin::satout(HorizontalLength2 * URAT);
		satin::internal::sfn(start);
		StateMap.set(StateFlag::FILDIR);
		satin::internal::sfn(start);
	}
	fvars(ClosestFormToCursor);
	satin::satout(HorizontalLength2);
	LineSpacing = SelectedForm->edgeSpacing;
	satin::internal::sfn(start);
	LineSpacing = spacing;
}

void satin::internal::outfn(unsigned start, unsigned finish, double satinWidth) {
	double angle   = 0.0;
	double length  = 0.0;
	double xOffset = 0.0, yOffset = 0.0;

	if (fabs((*FormAngles)[start]) < TINY && fabs((*FormAngles)[finish]) < TINY) {
		xOffset = 0.0;
		yOffset = satinWidth;
	}
	else {
#define SATHRESH 10

		angle  = ((*FormAngles)[finish] - (*FormAngles)[start]) / 2;
		length = satinWidth / cos(angle);
		if (length < -satinWidth * SATHRESH)
			length = -satinWidth * SATHRESH;
		if (length > satinWidth * SATHRESH)
			length = satinWidth * SATHRESH;
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
	unsigned   iBackup = 0;
	double     length  = 0.0;
	const auto maxSB   = satinBackup.size();
	for (iBackup = 0; iBackup < maxSB; iBackup++) {
		length = hypot(satinBackup[iBackup].x - pnt.x, satinBackup[iBackup].y - pnt.y);
		if (length < LineSpacing)
			return 1;
	}
	return 0;
}

SATCON* satin::adsatk(size_t count) noexcept {
	auto iSatinConnect = SatinGuideIndex;

	SatinGuideIndex += count;
	return &SatinGuides[iSatinConnect];
}

void satin::cpySat(const FRMHED& formHeader) {
	if (SelectedForm->type == SAT && SelectedForm->satinGuideCount) {
		SelectedForm->satinOrAngle.guide = satin::adsatk(SelectedForm->satinGuideCount);
		auto       sourceStart           = formHeader.satinOrAngle.guide;
		auto       sourceEnd             = sourceStart + SelectedForm->satinGuideCount;
		const auto destination = stdext::make_checked_array_iterator(SelectedForm->satinOrAngle.guide, 10000 - SatinGuideIndex);
		std::copy(sourceStart, sourceEnd, destination);
	}
}

size_t satin::getGuideSize() {
	return SatinGuideIndex;
}

void satin::clearGuideSize() {
	SatinGuideIndex = 0;
}

void satin::setGuideSize(size_t newGuideSize) {
	SatinGuideIndex = newGuideSize;
}

void satin::cpyTmpGuides(std::vector<SATCONOUT> &inSatinGuides)
{
	const auto destination = stdext::make_checked_array_iterator(
		SatinGuides + SatinGuideIndex, (sizeof(SatinGuides) / sizeof(SatinGuides[0])) - SatinGuideIndex);
	std::copy(inSatinGuides.cbegin(), inSatinGuides.cend(), destination);
}

void satin::cpyUndoGuides(const BAKHED& undoData)
{
	if (undoData.guideCount) {
		std::copy(undoData.guide, undoData.guide + undoData.guideCount, SatinGuides);
	}
	SatinGuideIndex = undoData.guideCount;
}


