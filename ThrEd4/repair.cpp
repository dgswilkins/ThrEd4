#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
//#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
//#include <string>

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "texture.h"
#include "thred.h"

void adbad(std::wstring& repairMessage, unsigned code, size_t count) {
	std::wstring fmtStr;

	loadString(fmtStr, code);
	repairMessage += fmtStr;
	loadString(fmtStr, IDS_NOTREP);
	repairMessage += fmt::format(fmtStr, count);
}

void lodchk() {
	unsigned attribute = 0;

	delinf();
	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		SelectedForm = &FormList[iForm];
		if (!SelectedForm->type)
			SelectedForm->type = FRMFPOLY;
		else {
			if (SelectedForm->type == FRMLINE) {
				if (SelectedForm->fillType != CONTF) {
					SelectedForm->fillType                = 0;
					SelectedForm->lengthOrCount.clipCount = 0;
				}
			}
		}
		frmout(iForm);
		if (!SelectedForm->maxFillStitchLen)
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		if (!SelectedForm->maxBorderStitchLen)
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
	}
	boost::dynamic_bitset<> formMap(FormIndex);
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		attribute = StitchBuffer[iStitch].attribute;
		if ((attribute & TYPMSK) == TYPFRM) {
			const unsigned tform = (attribute & FRMSK) >> FRMSHFT;
			if (tform < formMap.size()) {
				formMap.set(tform);
			}
			else {
				// ToDo - unassign the stitch from any form
				StitchBuffer[iStitch].attribute &= (NFRMSK & NTYPMSK);
			}
		}
	}
	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		if (!formMap.test(iForm))
			FormList[iForm].fillType = 0;
	}
	formMap.reset();
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		attribute = StitchBuffer[iStitch].attribute;
		if (attribute & TYPBRD)
			formMap.set((attribute & FRMSK) >> FRMSHFT);
	}
	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		if (!formMap.test(iForm))
			FormList[iForm].edgeType = 0;
	}
}

void chkclp(const FRMHED* const formHeader, BADCNTS& badData) {
	if (badData.clip == gsl::narrow<size_t>(formHeader->angleOrClipData.clip - ClipPoints))
		badData.clip += formHeader->lengthOrCount.clipCount;
	else
		badData.attribute |= BADCLP;
}

void chkeclp(const FRMHED* const formHeader, BADCNTS& badData) {
	if (badData.clip == gsl::narrow<size_t>(formHeader->borderClipData - ClipPoints))
		badData.clip += formHeader->clipEntries;
	else
		badData.attribute |= BADCLP;
}

unsigned frmchkfn() {
	BADCNTS badData = {};

	if (FormIndex) {
		for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
			const auto* formHeader = &FormList[iForm];
			if (!(badData.attribute & BADFLT)) {
				if (!formHeader->vertexCount)
					badData.attribute |= BADFLT;
				if (badData.flt == gsl::narrow<size_t>(formHeader->vertices - FormVertices))
					badData.flt += formHeader->vertexCount;
				else
					badData.attribute |= BADFLT;
			}
			if (!(badData.attribute & BADCLP)) {
				if (isclp(iForm))
					chkclp(formHeader, badData);
				if (iseclp(iForm))
					chkeclp(formHeader, badData);
			}
			if (formHeader->type == SAT && formHeader->satinGuideCount) {
				if (!(badData.attribute & BADSAT)) {
					if (badData.guideCount == gsl::narrow<size_t>(formHeader->satinOrAngle.guide - SatinGuides))
						badData.guideCount += formHeader->satinGuideCount;
					else
						badData.attribute |= BADSAT;
				}
			}
			if (istx(iForm)) {
				if (!(badData.attribute & BADTX)) {
					if (badData.tx == formHeader->fillInfo.texture.index)
						badData.tx += formHeader->fillInfo.texture.count;
					else
						badData.attribute |= BADTX;
				}
			}
			if (badData.attribute == (BADFLT | BADCLP | BADSAT | BADTX))
				break;
		}
		if (badData.flt != FormVertexIndex)
			badData.attribute |= BADFLT;
		if (badData.clip != ClipPointIndex)
			badData.attribute |= BADCLP;
		if (badData.guideCount != SatinGuideIndex)
			badData.attribute |= BADSAT;
		if (badData.tx != TextureIndex)
			badData.attribute |= BADTX;
	}
	return badData.attribute;
}

void bcup(size_t find, BADCNTS& badData) {
	const auto& formHeader = FormList[find];
	if (isclp(find))
		badData.clip += formHeader.lengthOrCount.clipCount;
	if (iseclp(find))
		badData.clip += formHeader.clipEntries;
	if (formHeader.type == SAT)
		badData.guideCount += formHeader.satinGuideCount;
	if (istx(find))
		badData.tx += formHeader.fillInfo.texture.count;
}

void chkfstch() noexcept {
	const size_t codedFormIndex = FormIndex << FRMSHFT;

	for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) >= codedFormIndex)
			StitchBuffer[iStitch].attribute = NOTFRM;
	}
}

void repflt(std::wstring& repairMessage) {
	size_t  iDestination = 0;
	size_t  vertexCount  = 0;
	BADCNTS badData      = {};

	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		if (FormList[iForm].vertexCount) {
			FormList[iDestination++] = FormList[iForm];
			vertexCount += FormList[iForm].vertexCount;
		}
	}
	FormIndex = iDestination;
	std::vector<fPOINT> vertexPoint;
	size_t              iVertex = 0;
	bool                flag    = true;
	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		const auto formHeader       = &FormList[iForm];
		const size_t vertexDifference = formHeader->vertices - FormVertices;
		if (FormVertexIndex >= vertexDifference + formHeader->vertexCount) {
			vertexPoint.resize(vertexPoint.size() + formHeader->vertexCount);
			auto       sourceStart = formHeader->vertices;
			auto       sourceEnd   = sourceStart + formHeader->vertexCount;
			auto       destination = vertexPoint.begin() + iVertex;
			const auto _           = std::copy(sourceStart, sourceEnd, destination);
			formHeader->vertices   = &FormVertices[iVertex];
			iVertex += formHeader->vertexCount;
			bcup(iForm, badData);
		}
		else {
			if (vertexDifference < FormVertexIndex) {
				formHeader->vertexCount = gsl::narrow<unsigned short>(FormVertexIndex - vertexDifference);
				delsac(iForm);
				// ToDo - do we need to increase the size of vertexPoint?
				// vertexPoint.resize(vertexPoint.size + formHeader->vertexCount);
				auto sourceStart = formHeader->vertices;
				auto sourceEnd   = sourceStart + formHeader->vertexCount;
				auto destination = vertexPoint.begin() + iVertex;
				auto _           = std::copy(sourceStart, sourceEnd, destination);
				bcup(iForm, badData);
			}
			else {
				FormIndex       = iForm;
				ClipPointIndex  = badData.clip;
				SatinGuideIndex = badData.guideCount;
				TextureIndex    = badData.tx;
				chkfstch();
				adbad(repairMessage, IDS_FRMDAT, FormIndex - iForm + 1);
				flag = false;
				break;
			}
		}
	}
	if (flag) {
		FormVertexIndex = iVertex;
	}
	std::copy(vertexPoint.cbegin(), vertexPoint.cend(), FormVertices);
}

void repclp(std::wstring& repairMessage) {
	unsigned            badClipCount = 0;
	size_t              clipCount = 0;
	std::vector<fPOINT> clipPoint;
	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		const auto formHeader = &FormList[iForm];
		size_t clipDifference = 0u;
		if (isclp(iForm)) {
			// ToDo - pointer arithmetic to be fixed
			clipDifference = formHeader->angleOrClipData.clip - ClipPoints;
			if (clipDifference + formHeader->lengthOrCount.clipCount < ClipPointIndex) {
				clipPoint.resize(clipPoint.size() + formHeader->lengthOrCount.clipCount);
				auto sourceStart = formHeader->angleOrClipData.clip;
				auto sourceEnd   = sourceStart + formHeader->lengthOrCount.clipCount;
				auto destination = clipPoint.begin() + clipCount;
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				formHeader->angleOrClipData.clip = &ClipPoints[clipCount];
				clipCount += formHeader->lengthOrCount.clipCount;
			}
			else {
				if (clipDifference < ClipPointIndex) {
					formHeader->lengthOrCount.clipCount = gsl::narrow<unsigned int>(FormVertexIndex - clipDifference);
					clipPoint.resize(clipPoint.size() + formHeader->lengthOrCount.clipCount);
					auto sourceStart = formHeader->angleOrClipData.clip;
					auto sourceEnd   = sourceStart + formHeader->lengthOrCount.clipCount;
					auto destination = clipPoint.begin() + clipCount;
					auto _           = std::copy(sourceStart, sourceEnd, destination);

					formHeader->angleOrClipData.clip = &ClipPoints[clipCount];
					clipCount += formHeader->lengthOrCount.clipCount;
				}
				else {
					badClipCount++;
					formHeader->fillType = 0;
				}
			}
		}
		if (iseclp(iForm)) {
			// ToDo - pointer arithmetic to be fixed
			clipDifference = formHeader->borderClipData - ClipPoints;
			if (clipDifference + formHeader->clipEntries < ClipPointIndex) {
				clipPoint.resize(clipPoint.size() + formHeader->clipEntries);
				auto sourceStart = formHeader->borderClipData;
				auto sourceEnd   = sourceStart + formHeader->clipEntries;
				auto destination = clipPoint.begin() + clipCount;
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				formHeader->borderClipData = &ClipPoints[clipCount];
				clipCount += formHeader->clipEntries;
			}
			else {
				if (clipDifference < ClipPointIndex) {
					formHeader->clipEntries = gsl::narrow<unsigned short>(FormVertexIndex - clipDifference);
					clipPoint.resize(clipPoint.size() + formHeader->clipEntries);
					auto sourceStart = formHeader->borderClipData;
					auto sourceEnd   = sourceStart + formHeader->clipEntries;
					auto destination = clipPoint.begin() + clipCount;
					auto _           = std::copy(sourceStart, sourceEnd, destination);

					formHeader->borderClipData = &ClipPoints[clipCount];
					clipCount += formHeader->clipEntries;
				}
				else {
					badClipCount++;
					formHeader->fillType = 0;
				}
			}
		}
	}
	std::copy(clipPoint.cbegin(), clipPoint.cend(), ClipPoints);
	ClipPointIndex = clipCount;
	if (badClipCount)
		adbad(repairMessage, IDS_CLPDAT, badClipCount);
}

void repsat() {
	size_t   guideCount = 0;
	BADCNTS  badData    = {};

	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		const auto formHeader = &FormList[iForm];
		if (formHeader->type == SAT) {
			// ToDo - pointer arithmetic to be fixed
			const size_t guideDifference = formHeader->satinOrAngle.guide - SatinGuides;
			if (FormVertexIndex > guideDifference + formHeader->vertexCount) {
				auto       sourceStart = formHeader->satinOrAngle.guide;
				auto       sourceEnd   = sourceStart + formHeader->satinGuideCount;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[guideCount], 10000 - guideCount);
				std::copy(sourceStart, sourceEnd, destination);
				formHeader->satinOrAngle.guide = &SatinGuides[guideCount];
				guideCount += formHeader->satinGuideCount;
				bcup(iForm, badData);
			}
			else {
				if (guideDifference < SatinGuideIndex) {
					formHeader->satinGuideCount = gsl::narrow<unsigned short>(SatinGuideIndex - guideDifference);
					auto       sourceStart      = formHeader->satinOrAngle.guide;
					auto       sourceEnd        = sourceStart + formHeader->satinGuideCount;
					const auto destination = stdext::make_checked_array_iterator(&SatinGuides[guideCount], 10000 - guideCount);
					std::copy(sourceStart, sourceEnd, destination);
					bcup(iForm, badData);
				}
				else {
					guideCount                  = badData.guideCount;
					formHeader->satinGuideCount = 0;
				}
			}
		}
	}
	SatinGuideIndex = guideCount;
}

void reptx() {
	size_t  textureCount = 0;
	BADCNTS badData      = {};

	for (size_t iForm = 0u; iForm < FormIndex; iForm++) {
		if (istx(iForm)) {
			const auto formHeader = &FormList[iForm];
			if (gsl::narrow<unsigned short>(TextureIndex)
			    > formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count) {
				auto       sourceStart = &SatinGuides[formHeader->fillInfo.texture.index];
				auto       sourceEnd   = sourceStart + formHeader->fillInfo.texture.count;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
				std::copy(sourceStart, sourceEnd, destination);
				formHeader->fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
				textureCount += formHeader->fillInfo.texture.count;
				bcup(iForm, badData);
			}
			else {
				if (TextureIndex > formHeader->fillInfo.texture.index) {
					formHeader->fillInfo.texture.count
					    = gsl::narrow<unsigned short>(TextureIndex) - formHeader->fillInfo.texture.index;
					auto       sourceStart = &SatinGuides[formHeader->fillInfo.texture.index];
					auto       sourceEnd   = sourceStart + formHeader->fillInfo.texture.count;
					const auto destination
					    = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
					std::copy(sourceStart, sourceEnd, destination);
					formHeader->fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
					bcup(iForm, badData);
					textureCount = badData.tx;
				}
				else
					formHeader->fillType = 0;
			}
		}
	}
	TextureIndex = textureCount;
}

void repar() {
	std::wstring   repairMessage;
	const unsigned repairType = frmchkfn();

	savdo();
	// RepairString = MsgBuffer;
	if (repairType & BADFLT)
		repflt(repairMessage);
	if (repairType & BADCLP)
		repclp(repairMessage);
	if (repairType & BADSAT)
		repsat();
	if (repairType & BADTX)
		reptx();
	lodchk();
	StateMap.set(StateFlag::RESTCH);
	shoMsg(repairMessage);
}

void frmchkx() {
	if (IniFile.dataCheck) {
		const auto code = frmchkfn();
		switch (IniFile.dataCheck) {
		case 1:
			if (code)
				datmsg(code);
			break;
		case 2:
			if (code)
				repar();
			break;
		case 3:
			if (code) {
				repar();
				tabmsg(IDS_DATREP);
			}
		}
	}
}
