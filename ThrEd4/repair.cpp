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

void repair::internal::adbad(std::wstring& repairMessage, uint32_t code, uint32_t count) {
	auto fmtStr = std::wstring {};

	displayText::loadString(fmtStr, code);
	repairMessage += fmtStr;
	displayText::loadString(fmtStr, IDS_NOTREP);
	repairMessage += fmt::format(fmtStr, count);
}

void repair::lodchk() {
	thred::delinf();
	if (!FormList->empty()) {
		auto& formList = *FormList;
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			SelectedForm = &formList[iForm];
			if (SelectedForm->type == 0U) {
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
			if (SelectedForm->maxFillStitchLen == 0.0F) {
				SelectedForm->maxFillStitchLen = IniFile.maxStitchLength;
			}
			if (SelectedForm->maxBorderStitchLen == 0.0F) {
				SelectedForm->maxBorderStitchLen = IniFile.maxStitchLength;
			}
		}
		auto formMap = boost::dynamic_bitset<>(FormList->size());
		for (auto& stitch : *StitchBuffer) {
			const auto attribute = stitch.attribute;
			if ((attribute & TYPMSK) == TYPFRM) {
				const auto tform = (attribute & FRMSK) >> FRMSHFT;
				if (tform < formMap.size()) {
					formMap.set(tform);
				}
				else {
					stitch.attribute &= (NFRMSK & NTYPMSK);
				}
			}
		}
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			auto& form = (*FormList)[iForm];
			if (!formMap.test(iForm)) {
				form.fillType = 0;
			}
		}
		formMap.reset();
		for (auto& stitch : *StitchBuffer) {
			const auto attribute = stitch.attribute;
			if ((attribute & TYPBRD) != 0U) {
				formMap.set((attribute & FRMSK) >> FRMSHFT);
			}
		}
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			auto& form = (*FormList)[iForm];
			if (!formMap.test(iForm)) {
				form.edgeType = 0;
			}
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

void repair::internal::chkVrtx(const FRMHED& form, BADCNTS& badData) noexcept {
	if (badData.flt == form.vertexIndex) {
		badData.flt += form.vertexCount;
	}
	else {
		badData.attribute |= BADFLT;
	}
}

void repair::internal::chkSat(const FRMHED& form, BADCNTS& badData) noexcept {
	if (badData.guideCount == form.satinOrAngle.guide) {
		badData.guideCount += form.satinGuideCount;
	}
	else {
		badData.attribute |= BADSAT;
	}
}

void repair::internal::chkTxt(const FRMHED& form, BADCNTS& badData) noexcept {
	if (badData.tx == form.fillInfo.texture.index) {
		badData.tx += form.fillInfo.texture.count;
	}
	else {
		badData.attribute |= BADTX;
	}
}

uint32_t repair::internal::frmchkfn() {
	auto badData = BADCNTS {};

	if (!FormList->empty()) {
		for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
			const auto& form = (*FormList)[iForm];
			if ((badData.attribute & BADFLT) == 0U) {
				if (form.vertexCount == 0U) {
					badData.attribute |= BADFLT;
				}
				ri::chkVrtx(form, badData);
			}
			if ((badData.attribute & BADCLP) == 0U) {
				if (clip::isclp(iForm)) {
					ri::chkclp(form, badData);
				}
				if (clip::iseclp(iForm)) {
					ri::chkeclp(form, badData);
				}
			}
			if ((badData.attribute & BADSAT) == 0U) {
				if (form.type == SAT && (form.satinGuideCount != 0U)) {
					ri::chkSat(form, badData);
				}
			}
			if ((badData.attribute & BADTX) == 0U) {
				if (texture::istx(iForm)) {
					ri::chkTxt(form, badData);
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
		if (badData.tx != TexturePointsBuffer->size()) {
			badData.attribute |= BADTX;
		}
	}
	return badData.attribute;
}

void repair::internal::bcup(uint32_t find, BADCNTS& badData) noexcept {
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

	for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >= codedFormIndex) {
			stitch.attribute = NOTFRM;
		}
	}
}

void repair::internal::repflt(std::wstring& repairMessage) {
	auto  iDestination = 0U;
	auto  vertexCount  = 0U;
	auto  badData      = BADCNTS {};
	auto& formList     = *FormList;

	for (auto form : *FormList) {
		if (form.vertexCount != 0U) {
			formList[iDestination++] = form;
			vertexCount += form.vertexCount;
		}
	}
	formList.resize(iDestination);
	auto vertexPoint = std::vector<fPOINT> {};
	auto iVertex     = 0U;
	auto flag        = true;
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		auto& form = formList[iForm];
		if (FormVertices->size() >= wrap::toSize(form.vertexIndex) + form.vertexCount) {
			auto sourceStart = std::next(FormVertices->cbegin(), form.vertexIndex);
			auto sourceEnd   = std::next(sourceStart, form.vertexCount);
			vertexPoint.insert(vertexPoint.end(), sourceStart, sourceEnd);
			form.vertexIndex = iVertex;
			iVertex += form.vertexCount;
			ri::bcup(iForm, badData);
		}
		else {
			if (form.vertexIndex < FormVertices->size()) {
				form.vertexCount = gsl::narrow<uint16_t>(FormVertices->size() - form.vertexIndex);
				satin::delsac(iForm);
				auto sourceStart = std::next(FormVertices->cbegin(), form.vertexIndex);
				auto sourceEnd   = std::next(sourceStart, form.vertexCount);
				vertexPoint.insert(vertexPoint.end(), sourceStart, sourceEnd);
				ri::bcup(iForm, badData);
			}
			else {
				FormList->resize(iForm);
				ClipPoints->resize(badData.clip);
				SatinGuides->resize(badData.guideCount);
				TexturePointsBuffer->resize(badData.tx);
				ri::chkfstch();
				ri::adbad(repairMessage, IDS_FRMDAT, wrap::toUnsigned(FormList->size()));
				flag = false;
				break;
			}
		}
	}
	*FormVertices = std::move(vertexPoint);
}

void repair::internal::repclp(std::wstring& repairMessage) {
	auto badClipCount = 0U;
	auto clipCount    = 0U;

	auto clipPoint = std::vector<fPOINT> {};
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		auto& form           = (*FormList)[iForm];
		auto  clipDifference = 0U;
		if (clip::isclp(iForm)) {
			clipDifference = form.angleOrClipData.clip;
			if (wrap::toSize(clipDifference) + form.lengthOrCount.clipCount < ClipPoints->size()) {
				clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
				auto sourceStart = std::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
				auto sourceEnd   = std::next(sourceStart, form.lengthOrCount.clipCount);
				auto destination = std::next(clipPoint.begin(), clipCount);
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				form.angleOrClipData.clip = clipCount;
				clipCount += form.lengthOrCount.clipCount;
			}
			else {
				if (clipDifference < ClipPoints->size()) {
					form.lengthOrCount.clipCount = wrap::toUnsigned(FormVertices->size() - clipDifference);
					clipPoint.resize(clipPoint.size() + form.lengthOrCount.clipCount);
					auto sourceStart = std::next(ClipPoints->cbegin(), form.angleOrClipData.clip);
					auto sourceEnd   = std::next(sourceStart, form.lengthOrCount.clipCount);
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
			if (wrap::toSize(clipDifference) + form.clipEntries < ClipPoints->size()) {
				clipPoint.resize(clipPoint.size() + form.clipEntries);
				auto sourceStart = std::next(ClipPoints->cbegin(), form.borderClipData);
				auto sourceEnd   = std::next(sourceStart, form.clipEntries);
				auto destination = std::next(clipPoint.begin(), clipCount);
				auto _           = std::copy(sourceStart, sourceEnd, destination);

				form.borderClipData = clipCount;
				clipCount += form.clipEntries;
			}
			else {
				if (clipDifference < ClipPoints->size()) {
					form.clipEntries = gsl::narrow<uint16_t>(FormVertices->size() - clipDifference);
					clipPoint.resize(clipPoint.size() + form.clipEntries);
					auto sourceStart = std::next(ClipPoints->cbegin(), form.borderClipData);
					auto sourceEnd   = std::next(sourceStart, form.clipEntries);
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
	if (badClipCount != 0U) {
		ri::adbad(repairMessage, IDS_CLPDAT, badClipCount);
	}
}

void repair::internal::repsat() {
	auto guideCount = 0U;
	auto badData    = BADCNTS {};

	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		auto& form = (*FormList)[iForm];
		if (form.type == SAT) {
			const auto guideDifference = form.satinOrAngle.guide;
			if (FormVertices->size() > wrap::toSize(guideDifference) + form.vertexCount) {
				auto       sourceStart = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
				auto       sourceEnd   = std::next(sourceStart, form.satinGuideCount);
				const auto destination = std::next(SatinGuides->begin(), guideCount);
				std::copy(sourceStart, sourceEnd, destination);
				form.satinOrAngle.guide = guideCount;
				guideCount += form.satinGuideCount;
				ri::bcup(iForm, badData);
			}
			else {
				if (guideDifference < SatinGuides->size()) {
					form.satinGuideCount   = gsl::narrow<uint16_t>(SatinGuides->size() - guideDifference);
					auto       sourceStart = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
					auto       sourceEnd   = std::next(sourceStart, form.satinGuideCount);
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
	auto textureCount = 0U;
	auto badData      = BADCNTS {};

	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		if (texture::istx(iForm)) {
			auto& form = (*FormList)[iForm];
			if (wrap::toUnsigned(TexturePointsBuffer->size())
			    > wrap::toUnsigned(form.fillInfo.texture.index) + form.fillInfo.texture.count) {
				auto sourceStart = std::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
				auto sourceEnd   = std::next(sourceStart, form.fillInfo.texture.count);
				auto destination = std::next(TexturePointsBuffer->begin(), textureCount);
				std::copy(sourceStart, sourceEnd, destination);
				form.fillInfo.texture.index = gsl::narrow<uint16_t>(textureCount);
				textureCount += form.fillInfo.texture.count;
				ri::bcup(iForm, badData);
			}
			else {
				if (TexturePointsBuffer->size() > form.fillInfo.texture.index) {
					form.fillInfo.texture.count
					    = gsl::narrow<uint16_t>(TexturePointsBuffer->size()) - form.fillInfo.texture.index;
					auto       sourceStart = std::next(TexturePointsBuffer->cbegin(), form.fillInfo.texture.index);
					auto       sourceEnd   = std::next(sourceStart, form.fillInfo.texture.count);
					const auto destination = std::next(TexturePointsBuffer->begin(), textureCount);
					std::copy(sourceStart, sourceEnd, destination);
					form.fillInfo.texture.index = gsl::narrow<uint16_t>(textureCount);
					ri::bcup(iForm, badData);
					textureCount = badData.tx;
				}
				else {
					form.fillType = 0;
				}
			}
		}
	}
	TexturePointsBuffer->resize(textureCount);
}

void repair::repar() {
	auto       repairMessage = std::wstring {};
	const auto repairType    = ri::frmchkfn();

	thred::savdo();
	// RepairString = MsgBuffer;
	if ((repairType & BADFLT) != 0U) {
		ri::repflt(repairMessage);
	}
	if ((repairType & BADCLP) != 0U) {
		ri::repclp(repairMessage);
	}
	if ((repairType & BADSAT) != 0U) {
		ri::repsat();
	}
	if ((repairType & BADTX) != 0U) {
		ri::reptx();
	}
	repair::lodchk();
	StateMap.set(StateFlag::RESTCH);
	displayText::shoMsg(repairMessage);
}

void repair::frmchkx() {
	if (IniFile.dataCheck != 0U) {
		const auto code = ri::frmchkfn();
		switch (IniFile.dataCheck) {
		case 1: {
			if (code != 0U) {
				displayText::datmsg(code);
			}
			break;
		}
		case 2: {
			if (code != 0U) {
				repair::repar();
			}
			break;
		}
		case 3: {
			if (code != 0U) {
				repair::repar();
				displayText::tabmsg(IDS_DATREP);
			}
		}
		}
	}
}
