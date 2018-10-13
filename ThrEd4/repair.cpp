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
#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "repair.h"

namespace ri = repair::internal;

void repair::internal::adbad(std::wstring& repairMessage, unsigned code, unsigned int count) {
	std::wstring fmtStr;

	displayText::loadString(fmtStr, code);
	repairMessage += fmtStr;
	displayText::loadString(fmtStr, IDS_NOTREP);
	repairMessage += fmt::format(fmtStr, count);
}

void repair::lodchk() {
	thred::delinf();
	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		SelectedForm = &FormList[iForm];
		if (!SelectedForm->type) {
			SelectedForm->type = FRMFPOLY;
		}
		else {
			if (SelectedForm->type == FRMLINE) {
				if (SelectedForm->fillType != CONTF) {
					SelectedForm->fillType                = 0;
					SelectedForm->lengthOrCount.clipCount = 0;
				}
			}
		}
		form::frmout(iForm);
		if (!SelectedForm->maxFillStitchLen) {
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		}
		if (!SelectedForm->maxBorderStitchLen) {
			SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
		}
	}
	boost::dynamic_bitset<> formMap(FormIndex);
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		const auto attribute = StitchBuffer[iStitch].attribute;
		if ((attribute & TYPMSK) == TYPFRM) {
			const auto tform = (attribute & FRMSK) >> FRMSHFT;
			if (tform < formMap.size()) {
				formMap.set(tform);
			}
			else {
				// ToDo - unassign the stitch from any form
				StitchBuffer[iStitch].attribute &= (NFRMSK & NTYPMSK);
			}
		}
	}
	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		if (!formMap.test(iForm)) {
			FormList[iForm].fillType = 0;
		}
	}
	formMap.reset();
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		const auto attribute = StitchBuffer[iStitch].attribute;
		if (attribute & TYPBRD) {
			formMap.set((attribute & FRMSK) >> FRMSHFT);
		}
	}
	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		if (!formMap.test(iForm)) {
			FormList[iForm].edgeType = 0;
		}
	}
}

void repair::internal::chkclp(const FRMHED* const formHeader, BADCNTS& badData) {
	if (badData.clip == gsl::narrow<unsigned int>(formHeader->angleOrClipData.clip - ClipPoints)) {
		badData.clip += formHeader->lengthOrCount.clipCount;
	}
	else {
		badData.attribute |= BADCLP;
	}
}

void repair::internal::chkeclp(const FRMHED* const formHeader, BADCNTS& badData) {
	if (badData.clip == gsl::narrow<unsigned int>(formHeader->borderClipData - ClipPoints)) {
		badData.clip += formHeader->clipEntries;
	}
	else {
		badData.attribute |= BADCLP;
	}
}

unsigned repair::internal::frmchkfn() {
	BADCNTS badData = {};

	if (FormIndex) {
		for (auto iForm = 0u; iForm < FormIndex; iForm++) {
			const auto* formHeader = &FormList[iForm];
			if (!(badData.attribute & BADFLT)) {
				if (!formHeader->vertexCount) {
					badData.attribute |= BADFLT;
				}
				if (badData.flt == gsl::narrow<unsigned int>(formHeader->vertices - FormVertices)) {
					badData.flt += formHeader->vertexCount;
				}
				else {
					badData.attribute |= BADFLT;
				}
			}
			if (!(badData.attribute & BADCLP)) {
				if (clip::isclp(iForm)) {
					ri::chkclp(formHeader, badData);
				}
				if (clip::iseclp(iForm)) {
					ri::chkeclp(formHeader, badData);
				}
			}
			if (formHeader->type == SAT && formHeader->satinGuideCount) {
				if (!(badData.attribute & BADSAT)) {
					if (badData.guideCount == gsl::narrow<unsigned int>(formHeader->satinOrAngle.guide - SatinGuides)) {
						badData.guideCount += formHeader->satinGuideCount;
					}
					else {
						badData.attribute |= BADSAT;
					}
				}
			}
			if (texture::istx(iForm)) {
				if (!(badData.attribute & BADTX)) {
					if (badData.tx == formHeader->fillInfo.texture.index) {
						badData.tx += formHeader->fillInfo.texture.count;
					}
					else {
						badData.attribute |= BADTX;
					}
				}
			}
			if (badData.attribute == (BADFLT | BADCLP | BADSAT | BADTX)) {
				break;
			}
		}
		if (badData.flt != FormVertexIndex) {
			badData.attribute |= BADFLT;
		}
		if (badData.clip != ClipPointIndex) {
			badData.attribute |= BADCLP;
		}
		if (badData.guideCount != satin::getGuideSize()) {
			badData.attribute |= BADSAT;
		}
		if (badData.tx != TextureIndex) {
			badData.attribute |= BADTX;
		}
	}
	return badData.attribute;
}

void repair::internal::bcup(unsigned int find, BADCNTS& badData) noexcept {
	const auto& formHeader = FormList[find];
	if (clip::isclp(find)) {
		badData.clip += formHeader.lengthOrCount.clipCount;
	}
	if (clip::iseclp(find)) {
		badData.clip += formHeader.clipEntries;
	}
	if (formHeader.type == SAT) {
		badData.guideCount += formHeader.satinGuideCount;
	}
	if (texture::istx(find)) {
		badData.tx += formHeader.fillInfo.texture.count;
	}
}

void repair::internal::chkfstch() noexcept {
	const auto codedFormIndex = FormIndex << FRMSHFT;

	for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) >= codedFormIndex) {
			StitchBuffer[iStitch].attribute = NOTFRM;
		}
	}
}

void repair::internal::repflt(std::wstring& repairMessage) {
	auto    iDestination = 0u;
	auto    vertexCount  = 0u;
	BADCNTS badData      = {};

	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		if (FormList[iForm].vertexCount) {
			FormList[iDestination++] = FormList[iForm];
			vertexCount += FormList[iForm].vertexCount;
		}
	}
	FormIndex = iDestination;
	std::vector<fPOINT> vertexPoint;

	auto iVertex = 0u;
	auto flag    = true;
	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		const auto formHeader = &FormList[iForm];
		// ToDo - find a better way than pointer arithmetic
		const auto vertexDifference = gsl::narrow<unsigned int>(formHeader->vertices - FormVertices);
		if (FormVertexIndex >= vertexDifference + formHeader->vertexCount) {
			vertexPoint.resize(vertexPoint.size() + formHeader->vertexCount);
			auto       sourceStart = formHeader->vertices;
			auto       sourceEnd   = sourceStart + formHeader->vertexCount;
			auto       destination = vertexPoint.begin() + iVertex;
			const auto _           = std::copy(sourceStart, sourceEnd, destination);
			formHeader->vertices   = &FormVertices[iVertex];
			iVertex += formHeader->vertexCount;
			ri::bcup(iForm, badData);
		}
		else {
			if (vertexDifference < FormVertexIndex) {
				formHeader->vertexCount = gsl::narrow<unsigned short>(FormVertexIndex - vertexDifference);
				satin::delsac(iForm);
				// ToDo - do we need to increase the size of vertexPoint?
				// vertexPoint.resize(vertexPoint.size + formHeader->vertexCount);
				auto sourceStart = formHeader->vertices;
				auto sourceEnd   = sourceStart + formHeader->vertexCount;
				auto destination = vertexPoint.begin() + iVertex;
				auto _           = std::copy(sourceStart, sourceEnd, destination);
				ri::bcup(iForm, badData);
			}
			else {
				FormIndex      = iForm;
				ClipPointIndex = badData.clip;
				satin::setGuideSize(badData.guideCount);
				TextureIndex = badData.tx;
				ri::chkfstch();
				ri::adbad(repairMessage, IDS_FRMDAT, FormIndex - iForm + 1);
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

void repair::internal::repclp(std::wstring& repairMessage) {
	auto badClipCount = 0u;
	auto clipCount    = 0u;

	std::vector<fPOINT> clipPoint;
	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		const auto formHeader     = &FormList[iForm];
		auto       clipDifference = 0u;
		if (clip::isclp(iForm)) {
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
		if (clip::iseclp(iForm)) {
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
	if (badClipCount) {
		ri::adbad(repairMessage, IDS_CLPDAT, badClipCount);
	}
}

void repair::internal::repsat() {
	auto    guideCount = 0u;
	BADCNTS badData    = {};

	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		const auto formHeader = &FormList[iForm];
		if (formHeader->type == SAT) {
			// ToDo - pointer arithmetic to be fixed
			const auto guideDifference = gsl::narrow<unsigned int>(formHeader->satinOrAngle.guide - SatinGuides);
			if (FormVertexIndex > guideDifference + formHeader->vertexCount) {
				auto       sourceStart = formHeader->satinOrAngle.guide;
				auto       sourceEnd   = sourceStart + formHeader->satinGuideCount;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[guideCount], 10000 - guideCount);
				std::copy(sourceStart, sourceEnd, destination);
				formHeader->satinOrAngle.guide = &SatinGuides[guideCount];
				guideCount += formHeader->satinGuideCount;
				ri::bcup(iForm, badData);
			}
			else {
				if (guideDifference < satin::getGuideSize()) {
					formHeader->satinGuideCount = gsl::narrow<unsigned short>(satin::getGuideSize() - guideDifference);
					auto       sourceStart      = formHeader->satinOrAngle.guide;
					auto       sourceEnd        = sourceStart + formHeader->satinGuideCount;
					const auto destination = stdext::make_checked_array_iterator(&SatinGuides[guideCount], 10000 - guideCount);
					std::copy(sourceStart, sourceEnd, destination);
					ri::bcup(iForm, badData);
				}
				else {
					guideCount                  = badData.guideCount;
					formHeader->satinGuideCount = 0;
				}
			}
		}
	}
	satin::setGuideSize(guideCount);
}

void repair::internal::reptx() {
	auto    textureCount = 0u;
	BADCNTS badData      = {};

	for (auto iForm = 0u; iForm < FormIndex; iForm++) {
		if (texture::istx(iForm)) {
			const auto formHeader = &FormList[iForm];
			if (gsl::narrow<unsigned short>(TextureIndex)
			    > formHeader->fillInfo.texture.index + formHeader->fillInfo.texture.count) {
				auto       sourceStart = &SatinGuides[formHeader->fillInfo.texture.index];
				auto       sourceEnd   = sourceStart + formHeader->fillInfo.texture.count;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
				std::copy(sourceStart, sourceEnd, destination);
				formHeader->fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
				textureCount += formHeader->fillInfo.texture.count;
				ri::bcup(iForm, badData);
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
					ri::bcup(iForm, badData);
					textureCount = badData.tx;
				}
				else {
					formHeader->fillType = 0;
				}
			}
		}
	}
	TextureIndex = textureCount;
}

void repair::repar() {
	std::wstring   repairMessage;
	const unsigned repairType = ri::frmchkfn();

	thred::savdo();
	// RepairString = MsgBuffer;
	if (repairType & BADFLT) {
		ri::repflt(repairMessage);
	}
	if (repairType & BADCLP) {
		ri::repclp(repairMessage);
	}
	if (repairType & BADSAT) {
		ri::repsat();
	}
	if (repairType & BADTX) {
		ri::reptx();
	}
	repair::lodchk();
	StateMap.set(StateFlag::RESTCH);
	displayText::shoMsg(repairMessage);
}

void repair::frmchkx() {
	if (IniFile.dataCheck) {
		const auto code = ri::frmchkfn();
		switch (IniFile.dataCheck) {
		case 1: {
			if (code) {
				displayText::datmsg(code);
			}
			break;
		}
		case 2: {
			if (code) {
				repair::repar();
			}
			break;
		}
		case 3: {
			if (code) {
				repair::repar();
				displayText::tabmsg(IDS_DATREP);
			}
		}
		}
	}
}
