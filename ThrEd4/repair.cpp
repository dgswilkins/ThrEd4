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
		if (SelectedForm->type == 0u) {
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
		if (SelectedForm->maxFillStitchLen == 0.0f) {
			SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
		}
		if (SelectedForm->maxBorderStitchLen == 0.0f) {
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
		if ((attribute & TYPBRD) != 0u) {
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
			if ((badData.attribute & BADFLT) == 0u) {
				if (form.vertexCount == 0u) {
					badData.attribute |= BADFLT;
				}
				if (badData.flt == form.vertexIndex) {
					badData.flt += form.vertexCount;
				}
				else {
					badData.attribute |= BADFLT;
				}
			}
			if ((badData.attribute & BADCLP) == 0u) {
				if (clip::isclp(iForm)) {
					ri::chkclp(form, badData);
				}
				if (clip::iseclp(iForm)) {
					ri::chkeclp(form, badData);
				}
			}
			if (form.type == SAT && (form.satinGuideCount != 0u)) {
				if ((badData.attribute & BADSAT) == 0u) {
					if (badData.guideCount == form.satinOrAngle.guide) {
						badData.guideCount += form.satinGuideCount;
					}
					else {
						badData.attribute |= BADSAT;
					}
				}
			}
			if (texture::istx(iForm)) {
				if ((badData.attribute & BADTX) == 0u) {
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
		if (badData.guideCount != SatinGuides->size()) {
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
		if (formList[iForm].vertexCount != 0u) {
			formList[iDestination++] = formList[iForm];
			vertexCount += formList[iForm].vertexCount;
		}
	}
	auto vertexPoint = std::vector<fPOINT>{};
	auto iVertex     = 0u;
	auto flag        = true;
	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form = formList[iForm];
		if (FormVertices->size() >= gsl::narrow_cast<size_t>(form.vertexIndex) + form.vertexCount) {
			vertexPoint.resize(vertexPoint.size() + form.vertexCount);
			auto       sourceStart = std::next(FormVertices->cbegin(), form.vertexIndex);
			auto       sourceEnd   = sourceStart + form.vertexCount;
			auto       destination = std::next(vertexPoint.begin(), iVertex);
			const auto _           = std::copy(sourceStart, sourceEnd, destination);
			form.vertexIndex       = iVertex;
			iVertex += form.vertexCount;
			ri::bcup(iForm, badData);
		}
		else {
			if (form.vertexIndex < FormVertices->size()) {
				form.vertexCount = gsl::narrow<unsigned short>(FormVertices->size() - form.vertexIndex);
				satin::delsac(iForm);
				// ToDo - do we need to increase the size of vertexPoint?
				// vertexPoint.resize(vertexPoint.size + form.vertexCount);
				auto sourceStart = std::next(FormVertices->cbegin(), form.vertexIndex);
				auto sourceEnd = std::next(sourceStart, form.vertexCount);
				vertexPoint.insert(vertexPoint.end(), sourceStart, sourceEnd);
				ri::bcup(iForm, badData);
			}
			else {
				FormList->resize(iForm);
				ClipPoints->resize(badData.clip);
				SatinGuides->resize(badData.guideCount);
				TextureIndex = badData.tx;
				ri::chkfstch();
				ri::adbad(repairMessage, IDS_FRMDAT, gsl::narrow<unsigned int>(FormList->size()));
				flag = false;
				break;
			}
		}
	}
	// ToDo - is this needed?
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
			if (gsl::narrow_cast<size_t>(clipDifference) + form.lengthOrCount.clipCount < ClipPoints->size()) {
				clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
				auto sourceStart = std::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
				auto sourceEnd   = sourceStart + form.lengthOrCount.clipCount;
				auto destination = std::next(clipPoint.begin(), clipCount);
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				form.angleOrClipData.clip = clipCount;
				clipCount += form.lengthOrCount.clipCount;
			}
			else {
				if (clipDifference < ClipPoints->size()) {
					form.lengthOrCount.clipCount = gsl::narrow<unsigned int>(FormVertices->size() - clipDifference);
					clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
					auto sourceStart = std::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
					auto sourceEnd   = sourceStart + form.lengthOrCount.clipCount;
					auto destination = std::next(clipPoint.begin(), clipCount);
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
			if (gsl::narrow_cast<size_t>(clipDifference) + form.clipEntries < ClipPoints->size()) {
				clipPoint.resize(clipPoint.size() + form.clipEntries);
				auto sourceStart = std::next(ClipPoints->cbegin(), form.borderClipData);
				auto sourceEnd   = sourceStart + form.clipEntries;
				auto destination = std::next(clipPoint.begin(), clipCount);
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				form.borderClipData = clipCount;
				clipCount += form.clipEntries;
			}
			else {
				if (clipDifference < ClipPoints->size()) {
					form.clipEntries = gsl::narrow<unsigned short>(FormVertices->size() - clipDifference);
					clipPoint.resize(clipPoint.size() + form.clipEntries);
					auto sourceStart = std::next(ClipPoints->cbegin(), form.borderClipData);
					auto sourceEnd   = sourceStart + form.clipEntries;
					auto destination = std::next(clipPoint.begin(), clipCount);
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
	if (badClipCount != 0u) {
		ri::adbad(repairMessage, IDS_CLPDAT, badClipCount);
	}
}

void repair::internal::repsat() {
	auto guideCount = 0u;
	auto badData    = BADCNTS{};

	for (auto iForm = 0u; iForm < FormList->size(); iForm++) {
		auto& form = (*FormList)[iForm];
		if (form.type == SAT) {
			const auto guideDifference = form.satinOrAngle.guide;
			if (FormVertices->size() > gsl::narrow_cast<size_t>(guideDifference) + form.vertexCount) {
				auto       sourceStart = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
				auto       sourceEnd   = sourceStart + form.satinGuideCount;
				const auto destination = std::next(SatinGuides->begin(), guideCount);
				std::copy(sourceStart, sourceEnd, destination);
				form.satinOrAngle.guide = guideCount;
				guideCount += form.satinGuideCount;
				ri::bcup(iForm, badData);
			}
			else {
				if (guideDifference < SatinGuides->size()) {
					form.satinGuideCount   = gsl::narrow<unsigned short>(SatinGuides->size() - guideDifference);
					auto       sourceStart = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
					auto       sourceEnd   = sourceStart + form.satinGuideCount;
					const auto destination = std::next(SatinGuides->begin(), guideCount);
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
	SatinGuides->resize(guideCount);
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
	if ((repairType & BADFLT) != 0u) {
		ri::repflt(repairMessage);
	}
	if ((repairType & BADCLP) != 0u) {
		ri::repclp(repairMessage);
	}
	if ((repairType & BADSAT) != 0u) {
		ri::repsat();
	}
	if ((repairType & BADTX) != 0u) {
		ri::reptx();
	}
	repair::lodchk();
	StateMap.set(StateFlag::RESTCH);
	displayText::shoMsg(repairMessage);
}

void repair::frmchkx() {
	if (IniFile.dataCheck != 0u) {
		const auto code = ri::frmchkfn();
		switch (IniFile.dataCheck) {
		case 1: {
			if (code != 0u) {
				displayText::datmsg(code);
			}
			break;
		}
		case 2: {
			if (code != 0u) {
				repair::repar();
			}
			break;
		}
		case 3: {
			if (code != 0u) {
				repair::repar();
				displayText::tabmsg(IDS_DATREP);
			}
		}
		}
	}
}
