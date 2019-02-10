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
	auto fmtStr = std::wstring{};

	displayText::loadString(fmtStr, code);
	repairMessage += fmtStr;
	displayText::loadString(fmtStr, IDS_NOTREP);
	repairMessage += fmt::format(fmtStr, count);
}

void repair::lodchk() {
	thred::delinf();
	auto& formList = *FormList;
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		SelectedForm = &formList[iForm];
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
	auto formMap = boost::dynamic_bitset<>(FormList->size());
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
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		if (!formMap.test(iForm)) {
			form.fillType = 0;
		}
	}
	formMap.reset();
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		const auto attribute = StitchBuffer[iStitch].attribute;
		if (attribute & TYPBRD) {
			formMap.set((attribute & FRMSK) >> FRMSHFT);
		}
	}
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		if (!formMap.test(iForm)) {
			form.edgeType = 0;
		}
	}
}

void repair::internal::chkclp(const FRMHED& formHeader, BADCNTS& badData) noexcept {
	if (badData.clip == formHeader.angleOrClipData.clip) {
		badData.clip += formHeader.lengthOrCount.clipCount;
	}
	else {
		badData.attribute |= BADCLP;
	}
}

void repair::internal::chkeclp(const FRMHED& formHeader, BADCNTS& badData) noexcept {
	if (badData.clip == formHeader.borderClipData) {
		badData.clip += formHeader.clipEntries;
	}
	else {
		badData.attribute |= BADCLP;
	}
}

unsigned repair::internal::frmchkfn() {
	auto badData = BADCNTS{};

	if (!FormList->empty()) {
		for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
			const auto& form = (*FormList)[iForm];
			if (!(badData.attribute & BADFLT)) {
				if (!form.vertexCount) {
					badData.attribute |= BADFLT;
				}
				if (badData.flt == form.vertexIndex) {
					badData.flt += form.vertexCount;
				}
				else {
					badData.attribute |= BADFLT;
				}
			}
			if (!(badData.attribute & BADCLP)) {
				if (clip::isclp(iForm)) {
					ri::chkclp(form, badData);
				}
				if (clip::iseclp(iForm)) {
					ri::chkeclp(form, badData);
				}
			}
			if (form.type == SAT && form.satinGuideCount) {
				if (!(badData.attribute & BADSAT)) {
					if (badData.guideCount == form.satinOrAngle.guide) {
						badData.guideCount += form.satinGuideCount;
					}
					else {
						badData.attribute |= BADSAT;
					}
				}
			}
			if (texture::istx(iForm)) {
				if (!(badData.attribute & BADTX)) {
					if (badData.tx == form.fillInfo.texture.index) {
						badData.tx += form.fillInfo.texture.count;
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
		if (badData.flt != FormVertices->size()) {
			badData.attribute |= BADFLT;
		}
		if (badData.clip != ClipPoints->size()) {
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

void repair::internal::bcup(unsigned int find, BADCNTS& badData) {
	const auto& form = (*FormList)[find];
	if (clip::isclp(find)) {
		badData.clip += form.lengthOrCount.clipCount;
	}
	if (clip::iseclp(find)) {
		badData.clip += form.clipEntries;
	}
	if (form.type == SAT) {
		badData.guideCount += form.satinGuideCount;
	}
	if (texture::istx(find)) {
		badData.tx += form.fillInfo.texture.count;
	}
}

void repair::internal::chkfstch() noexcept {
	const auto codedFormIndex = FormList->size() << FRMSHFT;

	for (auto iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if ((StitchBuffer[iStitch].attribute & FRMSK) >= codedFormIndex) {
			StitchBuffer[iStitch].attribute = NOTFRM;
		}
	}
}

void repair::internal::repflt(std::wstring& repairMessage) {
	auto  iDestination = 0u;
	auto  vertexCount  = 0u;
	auto  badData      = BADCNTS{};
	auto& formList     = *FormList;

	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		if (formList[iForm].vertexCount) {
			formList[iDestination++] = formList[iForm];
			vertexCount += formList[iForm].vertexCount;
		}
	}
	FormIndex        = iDestination;
	auto vertexPoint = std::vector<fPOINT>{};
	auto iVertex     = 0u;
	auto flag        = true;
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form = formList[iForm];
		// ToDo - find a better way than pointer arithmetic
		const auto vertexDifference = form.vertexIndex;
		if (FormVertices->size() >= vertexDifference + form.vertexCount) {
			vertexPoint.resize(vertexPoint.size() + form.vertexCount);
			auto       sourceStart = FormVertices->begin() + form.vertexIndex;
			auto       sourceEnd   = sourceStart + form.vertexCount;
			auto       destination = vertexPoint.begin() + iVertex;
			const auto _           = std::copy(sourceStart, sourceEnd, destination);
			form.vertexIndex          = iVertex;
			iVertex += form.vertexCount;
			ri::bcup(iForm, badData);
		}
		else {
			if (vertexDifference < FormVertices->size()) {
				form.vertexCount = gsl::narrow<unsigned short>(FormVertices->size() - vertexDifference);
				satin::delsac(iForm);
				// ToDo - do we need to increase the size of vertexPoint?
				// vertexPoint.resize(vertexPoint.size + form.vertexCount);
				auto sourceStart = FormVertices->begin() + form.vertexIndex;
				auto sourceEnd   = sourceStart + form.vertexCount;
				auto destination = vertexPoint.begin() + iVertex;
				auto _           = std::copy(sourceStart, sourceEnd, destination);
				ri::bcup(iForm, badData);
			}
			else {
				FormIndex = iForm;
				ClipPoints->resize(badData.clip);
				satin::setGuideSize(badData.guideCount);
				TextureIndex = badData.tx;
				ri::chkfstch();
				ri::adbad(repairMessage, IDS_FRMDAT, FormIndex - iForm + 1);
				flag = false;
				break;
			}
		}
	}
	//ToDo - is this needed?
	if (flag) {
		FormVertices->resize(iVertex);
	}
	std::copy(vertexPoint.cbegin(), vertexPoint.cend(), FormVertices->begin());
}

void repair::internal::repclp(std::wstring& repairMessage) {
	auto badClipCount = 0u;
	auto clipCount    = 0u;

	auto clipPoint = std::vector<fPOINT>{};
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form           = (*FormList)[iForm];
		auto  clipDifference = 0u;
		if (clip::isclp(iForm)) {
			// ToDo - pointer arithmetic to be fixed
			clipDifference = form.angleOrClipData.clip;
			if (clipDifference + form.lengthOrCount.clipCount < ClipPoints->size()) {
				clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
				auto sourceStart = ClipPoints->begin() + form.angleOrClipData.clip;
				auto sourceEnd   = sourceStart + form.lengthOrCount.clipCount;
				auto destination = clipPoint.begin() + clipCount;
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				form.angleOrClipData.clip = clipCount;
				clipCount += form.lengthOrCount.clipCount;
			}
			else {
				if (clipDifference < ClipPoints->size()) {
					form.lengthOrCount.clipCount = gsl::narrow<unsigned int>(FormVertices->size() - clipDifference);
					clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
					auto sourceStart = ClipPoints->begin() + form.angleOrClipData.clip;
					auto sourceEnd   = sourceStart + form.lengthOrCount.clipCount;
					auto destination = clipPoint.begin() + clipCount;
					auto _           = std::copy(sourceStart, sourceEnd, destination);

					form.angleOrClipData.clip = clipCount;
					clipCount += form.lengthOrCount.clipCount;
				}
				else {
					badClipCount++;
					form.fillType = 0;
				}
			}
		}
		if (clip::iseclp(iForm)) {
			clipDifference = form.borderClipData;
			if (clipDifference + form.clipEntries < ClipPoints->size()) {
				clipPoint.resize(clipPoint.size() + form.clipEntries);
				auto sourceStart = ClipPoints->begin() + form.borderClipData;
				auto sourceEnd   = sourceStart + form.clipEntries;
				auto destination = clipPoint.begin() + clipCount;
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				form.borderClipData = clipCount;
				clipCount += form.clipEntries;
			}
			else {
				if (clipDifference < ClipPoints->size()) {
					form.clipEntries = gsl::narrow<unsigned short>(FormVertices->size() - clipDifference);
					clipPoint.resize(clipPoint.size() + form.clipEntries);
					auto sourceStart = ClipPoints->begin() + form.borderClipData;
					auto sourceEnd   = sourceStart + form.clipEntries;
					auto destination = clipPoint.begin() + clipCount;
					auto _           = std::copy(sourceStart, sourceEnd, destination);

					form.borderClipData = clipCount;
					clipCount += form.clipEntries;
				}
				else {
					badClipCount++;
					form.fillType = 0;
				}
			}
		}
	}
	std::copy(clipPoint.cbegin(), clipPoint.cend(), ClipPoints->begin());
	if (badClipCount) {
		ri::adbad(repairMessage, IDS_CLPDAT, badClipCount);
	}
}

void repair::internal::repsat() {
	auto guideCount = 0u;
	auto badData    = BADCNTS{};

	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		if (form.type == SAT) {
			// ToDo - pointer arithmetic to be fixed
			const auto guideDifference = form.satinOrAngle.guide;
			if (FormVertices->size() > guideDifference + form.vertexCount) {
				auto       sourceStart = SatinGuides->begin() + form.satinOrAngle.guide;
				auto       sourceEnd = sourceStart + form.satinGuideCount;
				const auto destination = SatinGuides->begin() + guideCount;
				std::copy(sourceStart, sourceEnd, destination);
				form.satinOrAngle.guide = guideCount;
				guideCount += form.satinGuideCount;
				ri::bcup(iForm, badData);
			}
			else {
				if (guideDifference < SatinGuides->size()) {
					form.satinGuideCount = gsl::narrow<unsigned short>(SatinGuides->size() - guideDifference);
					auto       sourceStart = SatinGuides->begin() + form.satinOrAngle.guide;
					auto       sourceEnd = sourceStart + form.satinGuideCount;
					const auto destination = SatinGuides->begin() + guideCount;
					std::copy(sourceStart, sourceEnd, destination);
					ri::bcup(iForm, badData);
				}
				else {
					guideCount           = badData.guideCount;
					form.satinGuideCount = 0;
				}
			}
		}
	}
	satin::setGuideSize(guideCount);
}

void repair::internal::reptx() {
	auto textureCount = 0u;
	auto badData      = BADCNTS{};

	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		if (texture::istx(iForm)) {
			auto& form = (*FormList)[iForm];
			if (gsl::narrow<unsigned short>(TextureIndex) > form.fillInfo.texture.index + form.fillInfo.texture.count) {
				auto       sourceStart = &SatinGuides[form.fillInfo.texture.index];
				auto       sourceEnd   = sourceStart + form.fillInfo.texture.count;
				const auto destination = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
				std::copy(sourceStart, sourceEnd, destination);
				form.fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
				textureCount += form.fillInfo.texture.count;
				ri::bcup(iForm, badData);
			}
			else {
				if (TextureIndex > form.fillInfo.texture.index) {
					form.fillInfo.texture.count = gsl::narrow<unsigned short>(TextureIndex) - form.fillInfo.texture.index;
					auto       sourceStart      = &SatinGuides[form.fillInfo.texture.index];
					auto       sourceEnd        = sourceStart + form.fillInfo.texture.count;
					const auto destination
					    = stdext::make_checked_array_iterator(&SatinGuides[textureCount], 10000 - textureCount);
					std::copy(sourceStart, sourceEnd, destination);
					form.fillInfo.texture.index = gsl::narrow<unsigned short>(textureCount);
					ri::bcup(iForm, badData);
					textureCount = badData.tx;
				}
				else {
					form.fillType = 0;
				}
			}
		}
	}
	TextureIndex = textureCount;
}

void repair::repar() {
	auto       repairMessage = std::wstring{};
	const auto repairType    = ri::frmchkfn();

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
