#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <filesystem>

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
#include "EnumMap.h"
#include "utf8conv.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "hlp.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

namespace xi = xt::internal;

fPOINT   DesignSize;     // design size
uint32_t ColorOrder[16]; // color order adjusted for applique

enum stitchStyles
{
	TYPE_APPLIQUE = 1, // applique
	TYPE_CWALK,        // center walk
	TYPE_EWALK,        // edge walk
	TYPE_UNDERLAY,     // underlay
	TYPE_FTHR,         // feather
	TYPE_FILL,         // fill
	TYPE_BORDER        // border
};

char StitchTypes[] = {
	0,             // 0 no type
	TYPE_APPLIQUE, // 1 applique
	TYPE_CWALK,    // 2 center walk
	TYPE_EWALK,    // 3 edge walk
	TYPE_UNDERLAY, // 4 underlay
	0,             // 5 knot
	TYPE_FTHR,     // 6 feather
	0,             // 7 layer
	0,             // 8 layer
	0,             // 9 layer
	0,             // 10 reserved
	TYPE_FILL,     // 11 fill
	TYPE_BORDER,   // 12 border
};

void xt::setfchk() {
	if (IniFile.dataCheck != 0U) {
		StateMap.set(StateFlag::FCHK);
	}
}

#ifdef _DEBUG

void xt::internal::prbug() noexcept {
	OutputDebugString(&MsgBuffer[0]);
}
#endif

void xt::internal::fthvars(const FRMHED& form, FEATHER& feather) {
	StateMap.reset(StateFlag::BARSAT);
	StateMap.reset(StateFlag::FTHR);

	feather.fillType          = form.fillInfo.feather.fillType;
	feather.formRatio         = form.fillInfo.feather.ratio;
	feather.minStitch         = form.fillInfo.feather.minStitchSize;
	feather.totalCount        = form.fillInfo.feather.count;
	feather.extendedAttribute = form.extendedAttribute;
	feather.countUp           = form.fillInfo.feather.upCount;
	feather.upCount           = feather.countUp;
	feather.countDown         = form.fillInfo.feather.downCount;
	feather.downCount         = feather.countDown;
	feather.phaseIndex        = feather.upCount + feather.downCount;
	if ((feather.extendedAttribute & AT_FTHBLND) != 0U) {
		StateMap.set(StateFlag::BARSAT);
	}
	else {
		StateMap.set(StateFlag::FTHR);
	}
}

constexpr auto xt::internal::durat(float start, float finish, float featherRatio) -> float {
	return (finish - start) * featherRatio + start;
}

void xt::internal::duxrats(uint32_t start, uint32_t finish, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = durat((*BSequence)[finish].x, (*BSequence)[start].x, featherRatioLocal);
	point.y = durat((*BSequence)[finish].y, (*BSequence)[start].y, featherRatioLocal);
}

void xt::internal::durats(uint32_t iSequence, std::vector<fPOINT>* sequence, FEATHER& feather) {
	if (sequence != nullptr) {
		auto&      bCurrent     = (*BSequence)[iSequence];
		auto&      bNext        = (*BSequence)[wrap::toSize(iSequence) + 1U];
		const auto stitchLength = hypot(bNext.x - bCurrent.x, bNext.y - bCurrent.y);

		if (stitchLength < feather.minStitch) {
			sequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
		}
		else {
			feather.ratioLocal = feather.minStitch / stitchLength;
			const auto adjustedPoint
			    = fPOINT { durat(bNext.x, bCurrent.x, feather.ratioLocal), durat(bNext.y, bCurrent.y, feather.ratioLocal) };

			sequence->push_back(
			    fPOINT { durat(adjustedPoint.x, bCurrent.x, feather.ratio), durat(adjustedPoint.y, bCurrent.y, feather.ratio) });
		}
	}
}

auto xt::internal::bpsg() noexcept -> uint32_t {
	auto testValue = 0U;

	if (PseudoRandomValue == 0U) {
		PseudoRandomValue = FSED;
	}
	testValue = PseudoRandomValue & 0x40000008U;
	PseudoRandomValue >>= 1U;
	if (testValue == 0x8U || testValue == 0x40000000U) {
		PseudoRandomValue |= 0x40000000U;
	}
	return PseudoRandomValue;
}

void xt::internal::nurat(FEATHER& feather) noexcept {
	const auto remainder = fmodf(feather.globalPosition, 1.0F);
	switch (feather.fillType) {
	case FTHPSG: {
		if (feather.upCount != 0U) {
			if (feather.countUp != 0U) {
				feather.ratio
				    = (gsl::narrow_cast<float>(feather.totalCount) - gsl::narrow_cast<float>(form::psg() % feather.totalCount))
				      / gsl::narrow_cast<float>(feather.totalCount);
				feather.countUp--;
			}
			else {
				feather.ratio
				    = (gsl::narrow_cast<float>(feather.totalCount) - gsl::narrow_cast<float>(bpsg() % feather.totalCount))
				      / gsl::narrow_cast<float>(feather.totalCount);
				if (feather.countDown != 0U) {
					feather.countDown--;
				}
				else {
					PseudoRandomValue = FSED;
					feather.countUp   = feather.upCount;
					feather.countDown = feather.downCount;
				}
			}
		}
		else {
			feather.ratio
			    = (gsl::narrow_cast<float>(feather.totalCount) - gsl::narrow_cast<float>(form::psg() % feather.totalCount))
			      / gsl::narrow_cast<float>(feather.totalCount);
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHFAZ: {
		if (feather.phase >= feather.upCount) {
			feather.ratio = 1;
		}
		else {
			feather.ratio = feather.formRatio;
		}
		break;
	}
	case FTHSIN: {
		if (remainder > feather.globalRatio) {
			feather.ratio = sin((1.0F - remainder) / (1.0F - feather.globalRatio) * PI_F + PI_F) * 0.5F + 0.5F;
		}
		else {
			feather.ratio = sin(remainder / feather.globalRatio * PI_F) * 0.5F + 0.5F;
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHSIN2: {
		if (remainder > feather.globalRatio) {
			feather.ratio = sin((1.0F - remainder) / (1.0F - feather.globalRatio) * PI_F);
		}
		else {
			feather.ratio = sin(remainder / feather.globalRatio * PI_F);
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHRMP: {
		if (remainder > feather.globalRatio) {
			feather.ratio = (1.0F - remainder) / (1.0F - feather.globalRatio);
		}
		else {
			feather.ratio = remainder / feather.globalRatio;
		}
		feather.ratio *= feather.formRatio;
		break;
	}
	case FTHLIN:
	default: {
		feather.ratio = feather.formRatio;
		break;
	}
	}
	++feather.phase %= feather.phaseIndex;
	feather.globalPosition += feather.globalStep;
}

void xt::internal::fthfn(uint32_t iSequence, FEATHER& feather) {
	nurat(feather);
	durats(iSequence, OSequence, feather);
}

void xt::internal::ratpnt(uint32_t iPoint, uint32_t iNextPoint, fPOINT& point, float featherRatio) noexcept {
	point.x = ((*BSequence)[iNextPoint].x - (*BSequence)[iPoint].x) * featherRatio + (*BSequence)[iPoint].x;
	point.y = ((*BSequence)[iNextPoint].y - (*BSequence)[iPoint].y) * featherRatio + (*BSequence)[iPoint].y;
}

auto xt::internal::midpnt(const fPOINT& startPoint, const fPOINT& endPoint) noexcept -> fPOINT {
	return fPOINT { form::midl(endPoint.x, startPoint.x), form::midl(endPoint.y, startPoint.y) };
}

void xt::internal::xratf(const fPOINT& startPoint, const fPOINT& endPoint, fPOINT& point, float featherRatioLocal) noexcept {
	point.x = (endPoint.x - startPoint.x) * featherRatioLocal + startPoint.x;
	point.y = (endPoint.y - startPoint.y) * featherRatioLocal + startPoint.y;
}

void xt::internal::fthrbfn(uint32_t iSequence, FEATHER& feather, std::vector<fPOINT>& featherSequence) {
	auto       currentPoint = fPOINT {};
	auto       nextPoint    = fPOINT {};
	auto       midPoint     = fPOINT {};
	auto&      bCurrent     = (*BSequence)[iSequence];
	auto&      bNext        = (*BSequence)[wrap::toSize(iSequence) + 1U];
	const auto length       = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);

	nurat(feather);
	if (length < (2.0F * feather.minStitch)) {
		feather.ratio = 0.5F;
		ratpnt(iSequence, iSequence + 1, currentPoint, feather.ratio);
		ratpnt(iSequence + 3, iSequence + 2, nextPoint, feather.ratio);
	}
	else {
		auto currentHighPoint = fPOINT {};
		auto currentLowPoint  = fPOINT {};
		auto nextHighPoint    = fPOINT {};
		auto nextLowPoint     = fPOINT {};
		feather.ratioLocal    = feather.minStitch / length;
		duxrats(iSequence, iSequence + 1, currentLowPoint, feather.ratioLocal);
		duxrats(iSequence + 3, iSequence + 2, nextLowPoint, feather.ratioLocal);
		feather.ratioLocal = 1 - feather.ratioLocal;
		duxrats(iSequence, iSequence + 1, currentHighPoint, feather.ratioLocal);
		duxrats(iSequence + 3, iSequence + 2, nextHighPoint, feather.ratioLocal);
		feather.ratioLocal = feather.ratio;
		xratf(currentLowPoint, currentHighPoint, currentPoint, feather.ratioLocal);
		xratf(nextLowPoint, nextHighPoint, nextPoint, feather.ratioLocal);
	}
	midPoint = midpnt(currentPoint, nextPoint);
	OSequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
	OutputIndex++;
	OSequence->push_back(midPoint);
	OutputIndex++;
	featherSequence.emplace_back(bNext.x, bNext.y);
	featherSequence.push_back(midPoint);
}

void xt::internal::fthdfn(uint32_t iSequence, FEATHER& feather) {
	auto&      bCurrent = (*BSequence)[iSequence];
	auto&      bNext    = (*BSequence)[wrap::toSize(iSequence) + 1U];
	const auto length   = hypot(bNext.y - bCurrent.y, bNext.x - bCurrent.x);

	nurat(feather);
	OSequence->push_back(fPOINT { bCurrent.x, bCurrent.y });
	OSequence->push_back(fPOINT { bNext.x, bNext.y });
	if (length > feather.minStitch) {
		auto adjustedPoint = fPOINT {};
		auto currentPoint  = fPOINT {};
		auto nextPoint     = fPOINT {};
		feather.ratioLocal = 0.5F;
		duxrats(iSequence + 1, iSequence, adjustedPoint, feather.ratioLocal);
		feather.ratioLocal = feather.minStitch / length / 2;
		xratf(adjustedPoint, (*OSequence)[iSequence], currentPoint, feather.ratioLocal);
		xratf(adjustedPoint, (*OSequence)[wrap::toSize(iSequence) + 1U], nextPoint, feather.ratioLocal);
		feather.ratioLocal = feather.ratio;
		xratf(currentPoint, (*OSequence)[iSequence], (*OSequence)[iSequence], feather.ratioLocal);
		xratf(nextPoint,
		      (*OSequence)[wrap::toSize(iSequence) + 1U],
		      (*OSequence)[wrap::toSize(iSequence) + 1U],
		      feather.ratioLocal);
	}
}

void xt::internal::fritfil(const FRMHED& form, std::vector<fPOINT>& featherSequence) {
	if (!OSequence->empty()) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { TYPFRM, form.fillColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		form::chkseq(false);
		if (((form.extendedAttribute & AT_FTHBLND) != 0U)
		    && ~(form.extendedAttribute & (AT_FTHUP | AT_FTHBTH)) != (AT_FTHUP | AT_FTHBTH)) {
			InterleaveSequenceIndices->emplace_back(
			    INSREC { FTHMSK, form.fillInfo.feather.color, wrap::toUnsigned(InterleaveSequence->size()), I_FTH });

			const auto sequenceMax      = wrap::toUnsigned(featherSequence.size());
			auto       iReverseSequence = sequenceMax - 1U;
			for (auto iSequence = 0U; iSequence < sequenceMax; iSequence++) {
				(*OSequence)[iSequence] = featherSequence[iReverseSequence];
				iReverseSequence--;
			}
			OSequence->resize(sequenceMax);
			form::chkseq(false);
		}
	}
}

void xt::fthrfn(FRMHED& form) {
	const auto savedSpacing = LineSpacing;
	auto       feather      = FEATHER {};

	auto featherSequence = std::vector<fPOINT> {};

	// ToDo - what does this function do
	PseudoRandomValue = FSED;
	xi::fthvars(form, feather);
	LineSpacing = form.fillSpacing;
	satin::satfil(form);
	BSequence->front().attribute = 0;
	(*BSequence)[1].attribute    = 1;
	if (feather.phaseIndex == 0U) {
		feather.phaseIndex = 1U;
	}
	auto       ind = gsl::narrow_cast<uint32_t>(BSequence->size()) / (feather.phaseIndex << 2U);
	const auto res = gsl::narrow_cast<uint32_t>(BSequence->size()) % (feather.phaseIndex << 2U);
	if (res > (feather.phaseIndex << 1U)) {
		ind++;
	}
	feather.globalPosition = 0.0F;
	feather.globalStep     = 4.0F / gsl::narrow_cast<float>(BSequence->size() * ind);
	feather.globalPhase    = gsl::narrow_cast<float>(BSequence->size()) / ind;
	feather.globalRatio    = gsl::narrow_cast<float>(feather.countUp) / feather.phaseIndex;
	feather.globalUp       = feather.globalPhase * feather.globalRatio;
	feather.globalDown     = feather.globalPhase - feather.globalUp;
	form.fillType          = FTHF;
	feather.phase          = 1U;
	BSequence->push_back((*BSequence)[BSequence->size() - 2U]);
	BSequence->push_back((*BSequence)[BSequence->size() - 1U]);
	if ((feather.extendedAttribute & AT_FTHBLND) != 0U) {
		OutputIndex = 0U;
		for (ind = 0U; ind < wrap::toUnsigned(BSequence->size()) - 2U; ind++) {
			if ((*BSequence)[ind].attribute == 0) {
				xi::fthrbfn(ind, feather, featherSequence);
			}
		}
	}
	else {
		if ((form.extendedAttribute & AT_FTHBTH) != 0U) {
			for (ind = 0U; ind <= wrap::toUnsigned(BSequence->size()) - 2U; ind++) {
				if ((*BSequence)[ind].attribute == 0) {
					xi::fthdfn(ind, feather);
				}
			}
		}
		else {
			for (ind = 0U; ind <= wrap::toUnsigned(BSequence->size()) - 2U; ind++) {
				if ((*BSequence)[ind].attribute != 0) {
					if ((feather.extendedAttribute & AT_FTHUP) != 0U) {
						xi::fthfn(ind, feather);
					}
					else {
						OSequence->push_back(fPOINT { (*BSequence)[ind].x, (*BSequence)[ind].y });
					}
				}
				else {
					if ((feather.extendedAttribute & AT_FTHUP) != 0U) {
						OSequence->push_back(fPOINT { (*BSequence)[ind].x, (*BSequence)[ind].y });
					}
					else {
						xi::fthfn(ind, feather);
					}
				}
			}
		}
		OutputIndex = ind;
	}
	StateMap.reset(StateFlag::FTHR);
	StateMap.reset(StateFlag::BARSAT);
	LineSpacing = savedSpacing;
	xi::fritfil(form, featherSequence);
}

void xt::fethrf() {
	if (!FormList->empty()) {
		auto& form = FormList->operator[](ClosestFormToCursor);
		clip::delmclp(ClosestFormToCursor);
		texture::deltx(ClosestFormToCursor);
		form.type                           = SAT;
		form.fillInfo.feather.ratio         = IniFile.featherRatio;
		form.fillInfo.feather.upCount       = IniFile.featherUpCount;
		form.fillInfo.feather.downCount     = IniFile.featherDownCount;
		form.fillInfo.feather.fillType      = IniFile.featherFillType;
		form.fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
		form.extendedAttribute &= ~(AT_FTHUP | AT_FTHBTH | AT_FTHBLND);
		form.extendedAttribute |= IniFile.featherType;
		form.fillInfo.feather.count     = IniFile.featherCount;
		form.lengthOrCount.stitchLength = UserStitchLength;
		form.fillSpacing                = LineSpacing;
		form.fillColor                  = gsl::narrow<uint8_t>(ActiveColor);
		form.fillInfo.feather.color     = (ActiveColor + 1U) & COLMSK;
		form.fillType                   = FTHF;
		form::refilfn();
	}
}

void xt::fethr() {
	if (displayText::filmsgs(FMM_FTH)) {
		return;
	}
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xt::fethrf();
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			xt::fethrf();
		}
	}
	StateMap.set(StateFlag::INIT);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

constexpr auto xt::internal::tim2int(FILETIME time) noexcept -> ULARGE_INTEGER {
	auto op = ULARGE_INTEGER { { 0, 0 } };

	op.LowPart  = time.dwLowDateTime;
	op.HighPart = time.dwHighDateTime;
	return op;
}

#pragma warning(push)
#pragma warning(disable : 26487)
auto xt::insid(const FRMHED& form) -> std::vector<fPOINT>& {
	satin::satout(form, fabs(form.underlayIndent));
	if (form.underlayIndent > 0) {
		auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
			if (!form::cisin(form, (*InsidePoints)[iVertex].x, (*InsidePoints)[iVertex].y)) {
				(*InsidePoints)[iVertex] = vertexIt[iVertex];
			}
		}
		return *InsidePoints;
	}

	return *OutsidePoints;
}
#pragma warning(pop)

void xt::internal::delwlk(uint32_t code) {
	if (!StitchBuffer->empty()) {
		auto highStitchBuffer = std::vector<fPOINTATTR> {};
		highStitchBuffer.reserve(StitchBuffer->size());
		for (auto& stitch : *StitchBuffer) {
			if ((stitch.attribute & WLKFMSK) != code) {
				highStitchBuffer.push_back(stitch);
			}
		}
		if (highStitchBuffer.size() != StitchBuffer->size()) {
			StitchBuffer->resize(highStitchBuffer.size());
			std::copy(highStitchBuffer.cbegin(), highStitchBuffer.cend(), StitchBuffer->begin());
		}
	}
}

void xt::internal::chkuseq(FRMHED& form) {
#if BUGBAK
	uint32_t index;

	for (index = 0; index < OutputIndex; index++) {
		InterleaveSequence->push_back((*OSequence)[index]);
	}
	InterleaveSequenceIndices->back().color = form.underlayColor;
#else

	if (OutputIndex != 0U) {
		if (form.underlayStitchLen < MINWLK) {
			form.underlayStitchLen = MINWLK;
		}
		if (form.underlayStitchLen > MAXWLK) {
			form.underlayStitchLen = MAXWLK;
		}
		const auto underlayStitchLength = form.underlayStitchLen;
		for (auto iSequence = 0U; iSequence < OutputIndex - 1U; iSequence++) {
			const auto delta       = fPOINT { (*OSequence)[wrap::toSize(iSequence) + 1U].x - (*OSequence)[iSequence].x,
                                        (*OSequence)[wrap::toSize(iSequence) + 1U].y - (*OSequence)[iSequence].y };
			const auto length      = hypot(delta.x, delta.y);
			const auto stitchCount = wrap::round<uint32_t>(length / underlayStitchLength);
			if (stitchCount != 0U) {
				const auto step  = fPOINT { delta.x / stitchCount, delta.y / stitchCount };
				auto       point = (*OSequence)[iSequence];
				for (auto index = 0U; index < stitchCount; index++) {
					InterleaveSequence->push_back(point);
					point.x += step.x;
					point.y += step.y;
				}
			}
			else {
				InterleaveSequence->push_back((*OSequence)[iSequence]);
			}
		}
		InterleaveSequence->push_back((*OSequence)[OutputIndex - 1U]);
		// ToDo - should this be front or (back - 1) ?
		InterleaveSequenceIndices->front().color = form.underlayColor;
	}
#endif
}

void xt::internal::ritwlk(FRMHED& form) {
	if (OutputIndex != 0U) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { WLKMSK, form.underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		chkuseq(form);
	}
}

void xt::internal::ritcwlk(FRMHED& form) {
	if (OutputIndex != 0U) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { CWLKMSK, form.underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		chkuseq(form);
	}
}

auto xt::internal::gucon(const FRMHED&            form,
                         std::vector<fPOINTATTR>& buffer,
                         const fPOINT&            start,
                         const fPOINT&            finish,
                         uint32_t                 destination,
                         uint32_t                 code) -> uint32_t {
	auto length      = hypot(finish.x - start.x, finish.y - start.y);
	auto startVertex = form::closflt(form, start.x, start.y);

	const auto endVertex = form::closflt(form, finish.x, finish.y);

	auto up   = 0U;
	auto down = 0U;

	if (length < 5.0F) {
		return 0;
	}
	if (startVertex == endVertex) {
		return 0;
	}
	const auto& indentedPoint = xt::insid(form);
	up = down = startVertex;
	do {
		if (up == endVertex) {
			StateMap.reset(StateFlag::WLKDIR);
			break;
		}
		if (down == endVertex) {
			StateMap.set(StateFlag::WLKDIR);
			break;
		}
		up   = form::nxt(form, up);
		down = form::prv(form, down);
	} while (true);
	auto iStitch = destination;
	while (startVertex != endVertex) {
		if (iStitch != 0U) {
			if (buffer[iStitch - 1U].x != indentedPoint[startVertex].x
			    || buffer[iStitch - 1U].y != indentedPoint[startVertex].y) {
				buffer.emplace_back(fPOINTATTR { indentedPoint[startVertex].x, indentedPoint[startVertex].y, code });
				iStitch++;
			}
		}
		else {
			buffer.emplace_back(fPOINTATTR { indentedPoint[startVertex].x, indentedPoint[startVertex].y, code });
			iStitch++;
		}
		auto intermediateVertex = 0U;
		if (StateMap.test(StateFlag::WLKDIR)) {
			intermediateVertex = form::prv(form, startVertex);
		}
		else {
			intermediateVertex = form::nxt(form, startVertex);
		}
		const auto delta = fPOINT { indentedPoint[intermediateVertex].x - indentedPoint[startVertex].x,
			                        indentedPoint[intermediateVertex].y - indentedPoint[startVertex].y };
		length           = hypot(delta.x, delta.y);

		const auto stitchCount = wrap::round<uint32_t>(length / UserStitchLength);
		if (stitchCount > 1) {
			const auto step       = fPOINT { delta.x / stitchCount, delta.y / stitchCount };
			auto       localPoint = fPOINT { indentedPoint[startVertex].x + step.x, indentedPoint[startVertex].y + step.y };
			for (auto iStep = 0U; iStep < stitchCount - 1U; iStep++) {
				buffer.emplace_back(fPOINTATTR { localPoint.x, localPoint.y, code });
				iStitch++;
				localPoint.x += step.x;
				localPoint.y += step.y;
			}
		}
		if (StateMap.test(StateFlag::WLKDIR)) {
			startVertex = form::prv(form, startVertex);
		}
		else {
			startVertex = form::nxt(form, startVertex);
		}
	}
	buffer.emplace_back(fPOINTATTR { indentedPoint[startVertex].x, indentedPoint[startVertex].y, code });
	iStitch++;
	return iStitch - destination;
}

void xt::internal::fnwlk(FRMHED& form) {
	if (form.type == FRMLINE) {
		form.type = FRMFPOLY;
	}
	auto start = 0U;
	if (((form.extendedAttribute & AT_STRT) != 0U) && form.type != FRMLINE) {
		start = form.fillStart;
	}
	auto count = form.vertexCount;
	if (form.type != FRMLINE) {
		count++;
	}
	const auto& walkPoints = xt::insid(form);
	OutputIndex            = 0;
	while (count != 0U) {
		OSequence->push_back(walkPoints[start]);
		OutputIndex++;
		start = form::nxt(form, start);
		count--;
	}
	ritwlk(form);
}

void xt::internal::ritund(FRMHED& form) {
	if (!OSequence->empty()) {
		InterleaveSequenceIndices->emplace_back(
		    INSREC { UNDMSK, form.underlayColor, wrap::toUnsigned(InterleaveSequence->size()), I_FIL });
		chkuseq(form);
	}
}

void xt::internal::undclp(const FRMHED& form) {
	// ToDo - Is it better to initialize individually?
	auto& clipBuffer = *ClipBuffer;
	clipBuffer.clear();
	clipBuffer.reserve(2);
	ClipRectSize = FLSIZ { 0, form.underlayStitchLen };
	clipBuffer.emplace_back(0.0F, 00.0F, 0U);
	clipBuffer.emplace_back(0.0F, form.underlayStitchLen, 0U);
}

void xt::internal::fncwlk(FRMHED& form) {
	OutputIndex = 0;
	OSequence->clear();
	form.extendedAttribute |= AT_CWLK;
	if (form.satinGuideCount != 0U) {
		auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
		if (form.wordParam != 0U) {
			const auto iVertex    = form.wordParam;
			auto&      thisVertex = vertexIt[iVertex];
			auto&      nextVertex = vertexIt[wrap::toSize(iVertex) + 1U];
			OSequence->push_back(fPOINT { form::midl(thisVertex.x, nextVertex.x), form::midl(thisVertex.y, nextVertex.y) });
			OutputIndex++;
		}
		auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
		for (auto iGuide = form.satinGuideCount; iGuide != 0; iGuide--) {
			OSequence->push_back(
			    fPOINT { form::midl(vertexIt[guideIt[iGuide - 1U].finish].x, vertexIt[guideIt[iGuide - 1U].start].x),
			             form::midl(vertexIt[guideIt[iGuide - 1U].finish].y, vertexIt[guideIt[iGuide - 1U].start].y) });
			OutputIndex++;
		}
		if ((form.attribute & FRMEND) != 0U) {
			OSequence->push_back(fPOINT { form::midl(vertexIt[0].x, vertexIt[1].x), form::midl(vertexIt[0].y, vertexIt[1].y) });
			OutputIndex++;
		}
	}
	else {
		auto start = 0U;
		if ((form.extendedAttribute & AT_STRT) != 0U) {
			start = form.fillStart;
		}
		auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
		OSequence->push_back(vertexIt[start]);
		OutputIndex++;
		auto finish = form::prv(form, start);
		start       = form::nxt(form, start);
		for (auto iGuide = 1U; iGuide < (form.vertexCount / 2U); iGuide++) {
			OSequence->push_back(
			    fPOINT { form::midl(vertexIt[finish].x, vertexIt[start].x), form::midl(vertexIt[finish].y, vertexIt[start].y) });
			if (form::cisin(form, OSequence->back().x, OSequence->back().y)) {
				OutputIndex++;
			}
			start  = form::nxt(form, start);
			finish = form::prv(form, finish);
		}
		OSequence->push_back(vertexIt[start]);
		OutputIndex++;
	}
	ritcwlk(form);
}

void xt::srtcol() {
	constexpr auto colorSize = 16;
	auto           histogram = std::vector<uint32_t> {};
	histogram.resize(colorSize);
	auto colorStartStitch = std::vector<uint32_t> {};
	colorStartStitch.resize(colorSize);

	for (auto& stitch : *StitchBuffer) {
		histogram[stitch.attribute & COLMSK]++;
	}
	auto startStitch = 0U;
	auto it          = histogram.cbegin();
	for (auto& stitchColor : colorStartStitch) {
		stitchColor = startStitch;
		startStitch += *it;
		++it;
	}
	auto highStitchBuffer = std::vector<fPOINTATTR> {};
	highStitchBuffer.resize(StitchBuffer->size());
	for (auto& stitch : *StitchBuffer) {
		highStitchBuffer[colorStartStitch[stitch.attribute & COLMSK]++] = stitch;
	}
	std::copy(highStitchBuffer.cbegin(), highStitchBuffer.cend(), StitchBuffer->begin());
}

void xt::dubit(FRMHED& form, uint32_t bit) {
	thred::savdo();
	StateMap.set(StateFlag::WASDO);
	if (form.type == FRMLINE) {
		form.type = FRMFPOLY;
	}
	if (((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) == 0U) && ((bit & (AT_UND | AT_WALK | AT_CWLK)) != 0U)) {
		if (form.fillType != 0U) {
			form.underlayColor = form.fillColor;
		}
		else {
			form.underlayColor = gsl::narrow<uint8_t>(ActiveColor);
		}
		form.underlayStitchLen = IniFile.underlayStitchLen;
	}
	if (((form.extendedAttribute & AT_UND) == 0U) && ((bit & AT_UND) != 0U)) {
		form.underlayStitchAngle = IniFile.underlayAngle;
		form.underlaySpacing     = IniFile.underlaySpacing;
	}
	const auto code = form.extendedAttribute & bit;
	if (code != 0U) {
		form.extendedAttribute &= ~(bit);
	}
	else {
		form.extendedAttribute |= bit;
	}
	form::refil();
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setuang() {
	StateMap.set(StateFlag::GTUANG);
	displayText::msgflt(IDS_UANG, IniFile.underlayAngle / PI_F * 180.0F);
}

void xt::setuspac() {
	StateMap.set(StateFlag::GTUSPAC);
	displayText::msgflt(IDS_USPAC, IniFile.underlaySpacing / PFGRAN);
}

void xt::setwlkind() {
	StateMap.set(StateFlag::GTWLKIND);
	displayText::msgflt(IDS_WLKIND, IniFile.underlayIndent / PFGRAN);
}

void xt::setulen() {
	StateMap.set(StateFlag::GTWLKLEN);
	displayText::msgflt(IDS_WLKLEN, IniFile.underlayStitchLen / PFGRAN);
}

void xt::chkcwlk(FRMHED& form) {
	if ((form.extendedAttribute & AT_CWLK) != 0U) {
		xi::fncwlk(form);
	}
	else {
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | CWLKMSK);
	}
}

void xt::chkwlk(FRMHED& form) {
	if ((form.extendedAttribute & AT_WALK) != 0U) {
		xi::fnwlk(form);
	}
	else {
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | WLKMSK);
	}
}

void xt::internal::fnund(FRMHED& form, const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices) {
	const auto savedStitchSize = UserStitchLength;

	UserStitchLength = 1e38F;
	if (form.underlaySpacing == 0.0F) {
		form.underlaySpacing = IniFile.underlaySpacing;
	}
	if (form.underlayStitchLen == 0.0F) {
		form.underlayStitchLen = IniFile.underlayStitchLen;
	}
	undclp(form);
	StateMap.set(StateFlag::ISUND);
	form::angclpfn(form, textureSegments, angledFormVertices);
	OutputIndex = wrap::toUnsigned(OSequence->size());
	ritund(form);
	UserStitchLength = savedStitchSize;
}

void xt::chkund(FRMHED& form, const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices) {
	if ((form.extendedAttribute & AT_UND) != 0U) {
		xi::fnund(form, textureSegments, angledFormVertices);
	}
	else {
		xi::delwlk((ClosestFormToCursor << FRMSHFT) | UNDMSK);
	}
}

void xt::selalfrm() {
	SelectedFormList->reserve(FormList->size());
	for (auto formIndex = 0U; formIndex < wrap::toUnsigned(FormList->size()); formIndex++) {
		SelectedFormList->push_back(formIndex);
	}
	StateMap.set(StateFlag::RESTCH);
}

auto xt::internal::dutyp(uint32_t attribute) noexcept -> uint32_t {
	auto       result          = uint8_t { 0 };
	auto       bit             = DWORD { 0 };
	const auto maskedAttribute = attribute & SRTYPMSK;

	// ToDo - replace
	_BitScanReverse(&bit, maskedAttribute);

	if (bit == 0) {
		return 0U;
	}

	result = ((bit & 0xffU) - 18U);

	if ((result != 12U) || ((maskedAttribute & TYPATMSK) == 0)) {
		return result & 0xfU;
	}

	return 1U;
}

void xt::internal::durec(OREC& record) noexcept {
	const auto stitchIt = std::next(StitchBuffer->begin(), record.start);

	record.type          = StitchTypes[dutyp(stitchIt->attribute)];
	const auto attribute = stitchIt->attribute & SRTMSK;
	record.color         = attribute & 0xfU;
	record.form          = (attribute & FRMSK) >> FRMSHFT;
}

auto xt::internal::orComp(const OREC* record1, const OREC* record2) noexcept -> bool {
	// make sure the comparison obeys strict weak ordering for stable sorting
	if (record1 != nullptr && record2 != nullptr) {
		if (ColorOrder[record1->color] < ColorOrder[record2->color]) {
			return true;
		}
		if (ColorOrder[record2->color] < ColorOrder[record1->color]) {
			return false;
		}
		if (record1->form < record2->form) {
			return true;
		}
		if (record2->form < record1->form) {
			return false;
		}
		if (record1->type < record2->type) {
			return true;
		}
		if (record2->type < record1->type) {
			return false;
		}
		if (record1->start < record2->start) {
			return true;
		}
		if (record2->start < record1->start) {
			return false;
		}
	}

	return false;
}

auto xt::internal::orfComp(const OREC* record1, const OREC* record2) noexcept -> bool {
	// make sure the comparison obeys strict weak ordering for stable sorting
	if (record1 != nullptr && record2 != nullptr) {
		if (record1->form < record2->form) {
			return true;
		}
		if (record2->form < record1->form) {
			return false;
		}
		if (record1->type < record2->type) {
			return true;
		}
		if (record2->type < record1->type) {
			return false;
		}
	}

	return false;
}

auto xt::internal::chkrdun(const std::vector<uint32_t>& formFillCounter,
                           const std::vector<OREC*>&    pRecs,
                           const SRTREC&                stitchRecord) noexcept -> bool {
	for (auto iStitch = stitchRecord.start; iStitch < stitchRecord.finish; iStitch++) {
		if (pRecs[iStitch]->otyp == formFillCounter[pRecs[iStitch]->form]) {
			return true;
		}
	}
	return false;
}

auto xt::internal::precjmps(std::vector<fPOINTATTR>& stitchBuffer, const std::vector<OREC*>& pRecs, const SRTREC& sortRecord)
    -> double {
	auto currentRegion = sortRecord.currentRegion;
	auto stitchIt = StitchBuffer->begin();
	auto direction     = sortRecord.direction;

	auto formFillCounter = std::vector<uint32_t> {};
	formFillCounter.resize((FormList->size() + 2U) << 2U);
	auto totalJumps = 0U;
	while (chkrdun(formFillCounter, pRecs, sortRecord)) {
		double minimumLength = 1e9;
		if (direction) {
			stitchIt = std::next(StitchBuffer->begin(), pRecs[currentRegion]->finish - 1U);
		}
		else {
			stitchIt = std::next(StitchBuffer->begin(), pRecs[currentRegion]->start);
		}
		for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
			if (pRecs[iRegion]->otyp == formFillCounter[pRecs[iRegion]->form]) {
				auto& startStitch = StitchBuffer->operator[](pRecs[iRegion]->startStitch);
				auto length = hypot(startStitch.x - stitchIt->x, startStitch.y - stitchIt->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction     = false;
					currentRegion = iRegion;
				}
				auto& endStitch = StitchBuffer->operator[](pRecs[iRegion]->endStitch);
				length = hypot(endStitch.x - stitchIt->x, endStitch.y - stitchIt->y);
				if (length < minimumLength) {
					minimumLength = length;
					direction     = true;
					currentRegion = iRegion;
				}
			}
		}
		if (minimumLength > 9.0 * PFGRAN) {
			totalJumps++;
		}
		formFillCounter[pRecs[currentRegion]->form]++;
		if (StateMap.test(StateFlag::DUSRT)) {
			if (direction) {
				if (pRecs[currentRegion]->start != 0U) {
					for (auto iRegion = pRecs[currentRegion]->finish - 1U; iRegion >= pRecs[currentRegion]->start; iRegion--) {
						stitchBuffer.push_back(StitchBuffer->operator[](iRegion));
					}
				}
				else {
					auto iRegion = pRecs[currentRegion]->finish;
					while (iRegion != 0U) {
						stitchBuffer.push_back(StitchBuffer->operator[](--iRegion));
					}
				}
			}
			else {
				for (auto iRegion = pRecs[currentRegion]->start; iRegion < pRecs[currentRegion]->finish; iRegion++) {
					stitchBuffer.push_back(StitchBuffer->operator[](iRegion));
				}
			}
		}
	}
	return totalJumps;
}

auto xt::internal::duprecs(std::vector<fPOINTATTR>& stitchBuffer, const std::vector<OREC*>& pRecs, SRTREC& sortRecord)
    -> uint32_t {
	sortRecord.direction = false;
	const auto jumps0    = wrap::round<uint32_t>(precjmps(stitchBuffer, pRecs, sortRecord));

	sortRecord.direction = true;
	const auto jumps1    = wrap::round<uint32_t>(precjmps(stitchBuffer, pRecs, sortRecord));

	if (jumps0 < jumps1) {
		sortRecord.direction = false;
		return jumps0;
	}
	sortRecord.direction = true;
	return jumps1;
}

#ifdef _DEBUG

void xt::internal::dmprec(const std::vector<OREC*>& stitchRegion, uint32_t count) {
	for (auto iRegion = 0U; iRegion < count; iRegion++) {
		OutputDebugString(fmt::format(L"{:4d} attrb: 0x{:08x} form: {:4d} type: {} color: {:2d} start: {:5d} finish: {:5d}\n",
		                              iRegion,
		                              StitchBuffer->operator[](stitchRegion[iRegion]->start).attribute,
		                              stitchRegion[iRegion]->form,
		                              stitchRegion[iRegion]->type,
		                              stitchRegion[iRegion]->color,
		                              stitchRegion[iRegion]->start,
		                              stitchRegion[iRegion]->finish)
		                      .c_str());
	}
}
#endif

auto xt::internal::srtchk(const std::vector<OREC*>& stitchRegion, uint32_t count, uint32_t& badForm) noexcept -> bool {
	auto formIndex = stitchRegion[0]->form;
	auto color     = stitchRegion[0]->color;

	for (auto iRegion = 1U; iRegion < count; iRegion++) {
		if (stitchRegion[iRegion]->form == formIndex) {
			if (ColorOrder[stitchRegion[iRegion]->color] < ColorOrder[color]) {
				auto& form = FormList->operator[](formIndex);
				if (form.fillType == FTHF && ((form.extendedAttribute & AT_FTHBLND) != 0U)
				    && stitchRegion[iRegion]->color == form.fillColor) {
					continue;
				}
				badForm = iRegion;
				return false;
			}

			color = stitchRegion[iRegion]->color;
		}
		else {
			color     = stitchRegion[iRegion]->color;
			formIndex = stitchRegion[iRegion]->form;
		}
	}
	return true;
}

void xt::fsort() {
	if (!StitchBuffer->empty()) {
		auto attribute = StitchBuffer->front().attribute & SRTMSK;

		auto stitchRegion = std::vector<OREC> {};
		stitchRegion.reserve(100U);

		// ToDo - fsort does not appear to be capable of handling the case where the underlay, fill and border colors
		//        in a single form are not in ascending order already.
		thred::savdo();
		stitchRegion.emplace_back(OREC {});
		stitchRegion.back().startStitch = 0;
		ColorOrder[AppliqueColor]       = 0;
		for (auto iColor = 0U; iColor < 16U; iColor++) {
			if (iColor != AppliqueColor) {
				ColorOrder[iColor] = iColor + 1U;
			}
		}
		for (auto iStitch = 1U; iStitch < wrap::toUnsigned(StitchBuffer->size()); iStitch++) {
			if ((StitchBuffer->operator[](iStitch).attribute & SRTMSK) != attribute) {
				stitchRegion.back().finish    = iStitch;
				stitchRegion.back().endStitch = iStitch - 1U;
				stitchRegion.emplace_back(OREC {});
				stitchRegion.back().start       = iStitch;
				stitchRegion.back().startStitch = iStitch;
				attribute                       = StitchBuffer->operator[](iStitch).attribute & SRTMSK;
			}
		}
		stitchRegion.back().finish    = gsl::narrow<decltype(stitchRegion.back().finish)>(StitchBuffer->size());
		stitchRegion.back().endStitch = StitchBuffer->size() - 1U;
		const auto lastRegion         = wrap::toUnsigned(stitchRegion.size());
		auto       pRecs              = std::vector<OREC*> {};
		pRecs.reserve(lastRegion);
		auto pFRecs = std::vector<OREC*> {};
		pFRecs.reserve(lastRegion);
		for (auto& region : stitchRegion) {
			xi::durec(region);
			pRecs.push_back(&region);
			pFRecs.push_back(&region);
		}
		std::sort(pRecs.begin(), pRecs.end(), xi::orComp);
		std::sort(pFRecs.begin(), pFRecs.end(), xi::orfComp);
#ifdef _DEBUG
		xi::dmprec(pRecs, lastRegion);
#endif
		auto badForm = 0U;
		if (xi::srtchk(pFRecs, lastRegion, badForm)) {
			auto stitchRange = std::vector<RANGE> {};
			stitchRange.resize(lastRegion);
			stitchRange[0].start = 0;
			attribute            = pRecs[0]->color;
			auto currentForm     = 0xffffffffU;
			auto typeCount       = 0U;
			auto iRange          = 0U;
			for (auto iRegion = 0U; iRegion < lastRegion; iRegion++) {
				auto srtskp = true;
				if (attribute != pRecs[iRegion]->color) {
					stitchRange[iRange++].finish = iRegion;
					stitchRange[iRange].start    = iRegion;
					attribute                    = pRecs[iRegion]->color;
					currentForm                  = pRecs[iRegion]->form;
					typeCount                    = 0;
					srtskp                       = false;
				}
				if (srtskp) {
					if (pRecs[iRegion]->form == currentForm) {
						typeCount++;
					}
					else {
						typeCount   = 0;
						currentForm = pRecs[iRegion]->form;
					}
				}
				pRecs[iRegion]->otyp = typeCount;
			}
			stitchRange[iRange].finish  = lastRegion;
			const auto lastRange        = ++iRange;
			auto       tempStitchBuffer = std::vector<fPOINTATTR> {};
			tempStitchBuffer.reserve(StitchBuffer->size());
			for (iRange = 0; iRange < lastRange; iRange++) {
				StateMap.reset(StateFlag::DUSRT);
				auto sortRecord   = SRTREC {};
				sortRecord.start  = stitchRange[iRange].start;
				sortRecord.finish = stitchRange[iRange].finish;
				sortRecord.count  = sortRecord.finish - sortRecord.start;
				auto minimumJumps = 0xffffffffU;
				// timeout used to put an upper bound on the number of sorting permutations checked
				auto fileTime = FILETIME { 0U, 0U };
				GetSystemTimeAsFileTime(&fileTime);
				const auto startTime        = xi::tim2int(fileTime);
				auto       minimumIndex     = 0U;
				auto       minimumDirection = 0U;
				for (auto iRegion = sortRecord.start; iRegion < sortRecord.finish; iRegion++) {
					sortRecord.currentRegion = iRegion;
					if (pRecs[iRegion]->otyp == 0U) {
						const auto jumps = xi::duprecs(tempStitchBuffer, pRecs, sortRecord);
						if (jumps < minimumJumps) {
							minimumJumps     = jumps;
							minimumIndex     = iRegion;
							minimumDirection = gsl::narrow_cast<uint32_t>(sortRecord.direction);
						}
					}
					GetSystemTimeAsFileTime(&fileTime);
					const auto nextTime = xi::tim2int(fileTime);
					if (nextTime.QuadPart - startTime.QuadPart > SRTIM) {
						break;
					}
				}
				StateMap.set(StateFlag::DUSRT);
				sortRecord.currentRegion = minimumIndex;
				sortRecord.direction     = (minimumDirection != 0U);
				xi::precjmps(tempStitchBuffer, pRecs, sortRecord);
			}
			StitchBuffer->resize(tempStitchBuffer.size());
			std::copy(tempStitchBuffer.cbegin(), tempStitchBuffer.cend(), StitchBuffer->begin());
			thred::coltab();
			StateMap.set(StateFlag::RESTCH);
		}
		else {
			auto str = std::wstring {};
			displayText::loadString(str, IDS_SRTER);
			displayText::shoMsg(fmt::format(str, pFRecs[badForm]->form));
		}
	}
}

#ifdef _DEBUG

class ATFLD
{
public:
	uint32_t color { 0U };
	uint32_t form { 0U };
	uint32_t type { 0U };
	uint32_t layer { 0U };
	uint32_t user { 0U };

	constexpr ATFLD() noexcept = default;
	// ATFLD(ATFLD&&) = default;
	// ATFLD& operator=(const ATFLD& rhs) = default;
	// ATFLD& operator=(ATFLD&&) = default;
	//~ATFLD() = default;
};

void xt::internal::duatf(uint32_t ind) {
	const auto attribute = StitchBuffer->operator[](ind).attribute;

	auto attributeFields = ATFLD { (attribute & COLMSK),
		                           ((attribute & FRMSK) >> FRMSHFT),
		                           gsl::narrow<uint32_t>(StitchTypes[dutyp(attribute)]),
		                           ((attribute >> LAYSHFT) & 7U),
		                           0 };

	if ((attribute & 0x80000000) != 0U) {
		attributeFields.user = 1;
	}
	else {
		attributeFields.user = 0;
	}
	OutputDebugString(fmt::format(L"{:5d}: color: {:2d} form: {:5d} type: {:2d}, layer: {:1d}: user: {:1d} attrib: 0x{:08x}\n",
	                              ind,
	                              attributeFields.color,
	                              attributeFields.form,
	                              attributeFields.type,
	                              attributeFields.layer,
	                              attributeFields.user,
	                              attribute)
	                      .c_str());
}

void xt::dmpat() {
	auto attribute = StitchBuffer->front().attribute;
	auto iStitch   = 0U;
	xi::duatf(iStitch);
	for (auto& stitch : *StitchBuffer) {
		if (attribute != stitch.attribute) {
			xi::duatf(iStitch);
			attribute = stitch.attribute;
		}
		iStitch++;
	}
}
#endif

void xt::fdelstch(const FRMHED& form, FILLSTARTS& fillStartsData, uint32_t& fillStartsMap) {
	auto iDestinationStitch = 0U;
	auto tmap               = 0U;
	// ToDo - Still not sure what this function does?
	//        I suspect the fillStartsData members are not correctly named
	const auto codedFormIndex = (ClosestFormToCursor << FRMSHFT);
	auto       bordercolor    = gsl::narrow<uint32_t>(form.borderColor & COLMSK);

	auto appliqueColor = gsl::narrow<uint32_t>(form.borderColor >> FRMSHFT);
	for (auto iSourceStitch = 0U; iSourceStitch < wrap::toUnsigned(StitchBuffer->size()); iSourceStitch++) {
		if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX) && iSourceStitch == ClosestPointIndex) {
			ClosestPointIndex = iDestinationStitch;
		}
		const auto attribute = StitchBuffer->operator[](iSourceStitch).attribute;
		if (codedFormIndex == (attribute & (FRMSK | NOTFRM))) {
			const auto type = StitchTypes[xi::dutyp(attribute)];
			switch (type) {
			case TYPE_APPLIQUE: {
				if ((tmap & M_AP) == 0U) {
					tmap |= M_AP;
					fillStartsData.fillNamed.applique = iDestinationStitch;
				}
				break;
			}
			case TYPE_FTHR: {
				if ((tmap & M_FTH) == 0U) {
					tmap |= M_FTH;
					fillStartsData.fillNamed.feather = iDestinationStitch;
				}
				break;
			}
			case TYPE_FILL: {
				if ((tmap & M_FIL) == 0U) {
					tmap |= M_FIL;
					fillStartsData.fillNamed.fill = iDestinationStitch;
				}
				break;
			}
			case TYPE_BORDER: {
				if ((tmap & M_BRD) == 0U) {
					tmap |= M_BRD;
					fillStartsData.fillNamed.border = iDestinationStitch;
				}
				break;
			}
			default: {
				if ((form.fillType != 0U) && ((tmap & M_FIL) == 0U)) {
					tmap |= M_FIL;
					fillStartsData.fillNamed.fill = iDestinationStitch;
				}
				break;
			}
			}
		}
		else {
			// ToDo - there is a potential problem here when the active color is 0
			//        because the default color is 0 as well. Should the default color
			//        be e.g. -1 ?
			const auto color = attribute & COLMSK;
			if (color == form.fillColor) {
				tmap |= M_FCOL;
				fillStartsData.fillNamed.fillColor = iDestinationStitch;
			}
			if (color == form.fillInfo.feather.color) {
				tmap |= M_FTHCOL;
				fillStartsData.fillNamed.featherColor = iDestinationStitch;
			}
			if (color == bordercolor) {
				tmap |= M_ECOL;
				fillStartsData.fillNamed.borderColor = iDestinationStitch;
			}
			if (color == appliqueColor) {
				tmap |= M_APCOL;
				fillStartsData.fillNamed.appliqueColor = iDestinationStitch;
			}
			StitchBuffer->operator[](iDestinationStitch) = StitchBuffer->operator[](iSourceStitch);
			iDestinationStitch++;
		}
	}
	fillStartsData.fillNamed.fillColor++;
	fillStartsData.fillNamed.featherColor++;
	fillStartsData.fillNamed.borderColor++;
	fillStartsData.fillNamed.appliqueColor++;
	fillStartsMap = tmap;
	StitchBuffer->resize(iDestinationStitch);
	if ((tmap & M_ECOL) == 0U) {
		fillStartsData.fillNamed.borderColor = gsl::narrow<decltype(fillStartsData.fillNamed.borderColor)>(StitchBuffer->size());
	}
	if ((tmap & M_FTHCOL) == 0U) {
		fillStartsData.fillNamed.featherColor
		    = gsl::narrow<decltype(fillStartsData.fillNamed.featherColor)>(StitchBuffer->size());
	}
	if ((tmap & M_FCOL) == 0U) {
		fillStartsData.fillNamed.fillColor = gsl::narrow<decltype(fillStartsData.fillNamed.fillColor)>(StitchBuffer->size());
	}
	if (form.edgeType != 0U) {
		if (form.edgeType == EDGEAPPL) {
			if ((tmap & M_AP) == 0U) {
				if ((tmap & M_APCOL) != 0U) {
					fillStartsData.fillNamed.applique = fillStartsData.fillNamed.appliqueColor + 1U;
				}
				else {
					fillStartsData.fillNamed.applique
					    = gsl::narrow<decltype(fillStartsData.fillNamed.applique)>(StitchBuffer->size());
				}
			}
		}
		if ((tmap & M_BRD) == 0U) {
			if ((tmap & M_ECOL) != 0U) {
				fillStartsData.fillNamed.border = fillStartsData.fillNamed.borderColor + 1U;
			}
			else {
				fillStartsData.fillNamed.border = gsl::narrow<decltype(fillStartsData.fillNamed.border)>(StitchBuffer->size());
			}
		}
	}
	if ((form.fillType != 0U) || ((tmap & (M_WALK | M_UND | M_CWLK)) != 0U)) {
		if ((tmap & M_FIL) == 0U) {
			if ((tmap & M_FCOL) != 0U) {
				fillStartsData.fillNamed.fill = fillStartsData.fillNamed.fillColor + 1U;
			}
			else {
				fillStartsData.fillNamed.fill = gsl::narrow<decltype(fillStartsData.fillNamed.fill)>(StitchBuffer->size());
			}
		}
	}
	if (form.fillType == FTHF) {
		if ((tmap & M_FTH) == 0U) {
			if ((tmap & M_FTHCOL) != 0U) {
				fillStartsData.fillNamed.feather = fillStartsData.fillNamed.featherColor + 1U;
			}
			else {
				fillStartsData.fillNamed.feather = gsl::narrow<decltype(fillStartsData.fillNamed.feather)>(StitchBuffer->size());
			}
		}
	}
	for (auto ind = 3U; ind != 0U; ind--) {
		iDestinationStitch = ind - 1U;
		while (iDestinationStitch < ind) {
			if (fillStartsData.fillArray[iDestinationStitch] > fillStartsData.fillArray[ind]) {
				fillStartsData.fillArray[ind] = fillStartsData.fillArray[iDestinationStitch];
			}
			iDestinationStitch--;
		}
	}
	if (!UserFlagMap.test(UserFlag::FIL2OF) && StateMap.test(StateFlag::SELBOX)) {
		for (auto& startPoint : fillStartsData.fillArray) {
			startPoint = ClosestPointIndex;
		}
	}
}

auto xt::internal::lastcol(uint32_t index, fPOINT& point) noexcept -> bool {
	const auto color = InterleaveSequenceIndices->operator[](index).color;
	while (index != 0U) {
		index--;
		if (InterleaveSequenceIndices->operator[](index).color == color) {
			point = (*InterleaveSequence)[InterleaveSequenceIndices->operator[](wrap::toSize(index) + 1U).index - 1U];
			return true;
		}
	}
	return false;
}

void xt::internal::duint(const FRMHED& form, std::vector<fPOINTATTR>& buffer, uint32_t code, INTINF& ilData) {
	if (ilData.coloc > ilData.start) {
		const auto count       = ilData.coloc - ilData.start;
		auto       sourceStart = std::next(StitchBuffer->begin(), ilData.start);
		auto       sourceEnd   = sourceStart + count;

		const auto destination = std::next(buffer.begin(), ilData.output);
		buffer.insert(destination, sourceStart, sourceEnd);
		ilData.start += count;
		ilData.output += count;
	}
	if ((form.extendedAttribute & AT_STRT) != 0U) {
		if (!StateMap.testAndSet(StateFlag::DIDSTRT)) {
			auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
			ilData.output += gucon(form,
			                       buffer,
			                       vertexIt[form.fillStart],
			                       (*InterleaveSequence)[InterleaveSequenceIndices->operator[](ilData.pins).index],
			                       ilData.output,
			                       code);
		}
	}
	auto point = fPOINT {};
	if (lastcol(ilData.pins, point)) {
		ilData.output += gucon(
		    form, buffer, point, (*InterleaveSequence)[InterleaveSequenceIndices->operator[](ilData.pins).index], ilData.output, code);
	}
	for (auto iSequence = InterleaveSequenceIndices->operator[](ilData.pins).index;
	     iSequence < InterleaveSequenceIndices->operator[](wrap::toSize(ilData.pins) + 1U).index;
	     iSequence++) {
		if (ilData.output > 0) {
			if ((*InterleaveSequence)[iSequence].x != buffer[ilData.output - 1U].x
			    || (*InterleaveSequence)[iSequence].y != buffer[ilData.output - 1U].y) {
				buffer.emplace_back(fPOINTATTR { (*InterleaveSequence)[iSequence].x, (*InterleaveSequence)[iSequence].y, code });
				ilData.output++;
			}
		}
		else {
			buffer.emplace_back(fPOINTATTR { (*InterleaveSequence)[iSequence].x, (*InterleaveSequence)[iSequence].y, code });
			ilData.output++;
		}
	}
}

auto xt::internal::isfil(const FRMHED& form) noexcept -> bool {
	if (form.fillType != 0U) {
		return true;
	}
	if (form.edgeType != 0U) {
		return true;
	}
	if ((form.extendedAttribute & (AT_CWLK | AT_WALK | AT_UND)) != 0U) {
		return true;
	}
	return false;
}

void xt::internal::chkend(const FRMHED& form, std::vector<fPOINTATTR>& buffer, uint32_t code, INTINF& ilData) {
	if (isfil(form)) {
		StateMap.set(StateFlag::ISEND);
		if ((form.extendedAttribute & AT_END) != 0U) {
			auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
			ilData.output += gucon(form, buffer, InterleaveSequence->back(), vertexIt[form.fillEnd], ilData.output, code);
		}
	}
}

void xt::intlv(const FRMHED& form, const FILLSTARTS& fillStartsData, uint32_t fillStartsMap) {
	auto ilData = INTINF {};

	StateMap.reset(StateFlag::ISEND);
	InterleaveSequenceIndices->emplace_back(INSREC { 0, 0, wrap::toUnsigned(InterleaveSequence->size()), 0 });
	ilData.layerIndex = (gsl::narrow<uint32_t>(form.attribute & FRMLMSK) << (LAYSHFT - 1)) | (ClosestFormToCursor << FRMSHFT);
	StateMap.reset(StateFlag::DIDSTRT);
	if (!StitchBuffer->empty()) {
		auto highStitchBuffer = std::vector<fPOINTATTR> {};
		auto code             = 0U;
		for (auto iSequence = 0U; iSequence < wrap::toUnsigned(InterleaveSequenceIndices->size() - 1U); iSequence++) {
			ilData.pins = iSequence;
			switch (InterleaveSequenceIndices->operator[](iSequence).seq) {
			case I_AP: {
				if (((fillStartsMap & M_FIL) != 0U) && fillStartsData.fillNamed.applique >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.applique;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.appliqueColor;
					if (ilData.coloc == 1) {
						ilData.coloc = 0;
					}
				}
				break;
			}
			case I_FIL: {
				if (((fillStartsMap & M_FIL) != 0U) && fillStartsData.fillNamed.fill >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.fill;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.fillColor;
				}
				break;
			}
			case I_FTH: {
				if (((fillStartsMap & M_FIL) != 0U) && fillStartsData.fillNamed.feather >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.feather;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.featherColor;
				}
				break;
			}
			case I_BRD: {
				if (((fillStartsMap & M_BRD) != 0U) && fillStartsData.fillNamed.border >= ilData.coloc) {
					ilData.coloc = fillStartsData.fillNamed.border;
				}
				else {
					ilData.coloc = fillStartsData.fillNamed.borderColor;
				}
				break;
			}
			}
			code = gsl::narrow<uint32_t>(ilData.layerIndex | InterleaveSequenceIndices->operator[](ilData.pins).code
			                             | InterleaveSequenceIndices->operator[](ilData.pins).color);
			xi::duint(form, highStitchBuffer, code, ilData);
		}
		xi::chkend(form, highStitchBuffer, code, ilData);
		if ((!StitchBuffer->empty()) && ilData.start < gsl::narrow<decltype(ilData.start)>(StitchBuffer->size() - 1U)) {
			const auto ine         = wrap::toUnsigned(StitchBuffer->size() - ilData.start);
			auto       sourceStart = std::next(StitchBuffer->begin(), ilData.start);
			auto       sourceEnd   = StitchBuffer->end();

			const auto destination = std::next(highStitchBuffer.begin(), ilData.output);
			highStitchBuffer.insert(destination, sourceStart, sourceEnd);
			ilData.output += ine;
		}
		auto sourceStart = highStitchBuffer.begin();
		auto sourceEnd   = sourceStart + ilData.output;
		StitchBuffer->clear();
		StitchBuffer->insert(StitchBuffer->begin(), sourceStart, sourceEnd);
	}
	else { // no stitches added so far
		auto code     = 0U;
		auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
		for (auto iSequence = 0U; iSequence < wrap::toUnsigned(InterleaveSequenceIndices->size() - 1U); iSequence++) {
			code = gsl::narrow<uint32_t>(ilData.layerIndex | InterleaveSequenceIndices->operator[](iSequence).code
			                             | InterleaveSequenceIndices->operator[](iSequence).color);
			if ((form.extendedAttribute & AT_STRT) != 0U) {
				if (!StateMap.testAndSet(StateFlag::DIDSTRT)) {
					ilData.output += xi::gucon(form,
					                           *StitchBuffer,
					                           vertexIt[form.fillStart],
					                           (*InterleaveSequence)[InterleaveSequenceIndices->operator[](ilData.pins).index],
					                           ilData.output,
					                           code);
				}
			}
			auto colpnt = fPOINT {};
			if (xi::lastcol(iSequence, colpnt)) {
				ilData.output += xi::gucon(form,
				                           *StitchBuffer,
				                           colpnt,
				                           (*InterleaveSequence)[InterleaveSequenceIndices->operator[](iSequence).index],
				                           ilData.output,
				                           code);
			}
			for (auto ine = InterleaveSequenceIndices->operator[](iSequence).index;
			     ine < InterleaveSequenceIndices->operator[](wrap::toSize(iSequence) + 1U).index;
			     ine++) {
				if (ilData.output > 0) {
					if ((*InterleaveSequence)[ine].x != StitchBuffer->operator[](ilData.output - 1U).x
					    || (*InterleaveSequence)[ine].y != StitchBuffer->operator[](ilData.output - 1U).y) {
						StitchBuffer->push_back({ (*InterleaveSequence)[ine].x, (*InterleaveSequence)[ine].y, code });
						ilData.output++;
					}
				}
				else {
					StitchBuffer->push_back({ (*InterleaveSequence)[ine].x, (*InterleaveSequence)[ine].y, code });
					ilData.output++;
				}
			}
		}
		xi::chkend(form, *StitchBuffer, code, ilData);
	}
	thred::coltab();
}

void xt::internal::setundfn(uint32_t code) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		auto& form = FormList->operator[](ClosestFormToCursor);
		if (form.type != FRMLINE) {
			const auto savedAttribute = form.extendedAttribute;
			form.extendedAttribute |= code;
			if (savedAttribute != form.extendedAttribute) {
				form::refilfn();
			}
		}
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			auto& form = FormList->operator[](selectedForm);
			if (form.type == FRMLINE) {
				continue;
			}
			const auto savedAttribute = form.extendedAttribute;
			form.extendedAttribute |= code;
			if (savedAttribute != form.extendedAttribute) {
				form::refilfn();
			}
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setund() {
	xi::setundfn(AT_UND);
}

void xt::setwlk() {
	xi::setundfn(AT_WALK);
}

void xt::setcwlk() {
	xi::setundfn(AT_CWLK);
}

void xt::internal::notundfn(uint32_t code) {
	thred::savdo();
	code = ~code;
	if (StateMap.test(StateFlag::FORMSEL)) {
		auto& form = FormList->operator[](ClosestFormToCursor);
		if (form.type != FRMLINE) {
			const auto savedAttribute = form.extendedAttribute;
			form.extendedAttribute &= code;
			if (savedAttribute != form.extendedAttribute) {
				form::refilfn();
			}
		}
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			auto& form = FormList->operator[](selectedForm);
			if (form.type == FRMLINE) {
				continue;
			}
			const auto savedAttribute = form.extendedAttribute;
			form.extendedAttribute &= code;
			if (savedAttribute != form.extendedAttribute) {
				form::refilfn();
			}
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::notund() {
	xi::notundfn(AT_UND);
}

void xt::notwlk() {
	xi::notundfn(AT_WALK);
}

void xt::notcwlk() {
	xi::notundfn(AT_CWLK);
}

void xt::internal::ulenfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
		form.underlayStitchLen = length;
		form::refilfn();
	}
}

void xt::dusulen(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::ulenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::ulenfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::undlen() {
	displayText::tabmsg(IDS_SETULEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETULEN);
	displayText::numWnd();
}

void xt::internal::uspacfn(uint32_t formNumber, float spacing) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if ((form.extendedAttribute & AT_UND) != 0U) {
		form.underlaySpacing = spacing;
		form::refilfn();
	}
}

void xt::duspac(float spacing) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::uspacfn(ClosestFormToCursor, spacing);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::uspacfn(selectedForm, spacing);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::uspac() {
	displayText::tabmsg(IDS_SETUSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUSPAC);
	displayText::numWnd();
}

void xt::internal::uangfn(uint32_t formNumber, float angle) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if ((form.extendedAttribute & AT_UND) != 0U) {
		form.underlayStitchAngle = angle;
		form::refilfn();
	}
}

void xt::dufang(float angle) {
	thred::savdo();
	angle *= PI_F / 180.0F;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::uangfn(ClosestFormToCursor, angle);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::uangfn(selectedForm, angle);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::sfuang() {
	displayText::tabmsg(IDS_SETUANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUANG);
	displayText::numWnd();
}

void xt::internal::flenfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if ((form.fillType != 0U) && !clip::isclp(form)) {
		form.lengthOrCount.stitchLength = length;
		form::refilfn();
	}
}

void xt::duflen(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::flenfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::flenfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setflen() {
	displayText::tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFLEN);
	displayText::numWnd();
}

void xt::internal::fspacfn(uint32_t formNumber, float spacing) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.fillType != 0U) {
		if (spacing < 0) {
			if (!clip::isclp(form)) {
				return;
			}
		}
		form.fillSpacing = spacing;
		form::refilfn();
	}
}

void xt::dufspac(float spacing) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fspacfn(ClosestFormToCursor, spacing);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fspacfn(selectedForm, spacing);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfspac() {
	displayText::tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFSPAC);
	displayText::numWnd();
}

void xt::internal::findfn(uint32_t formNumber, float indent) {
	ClosestFormToCursor = formNumber;
	auto& form          = FormList->operator[](formNumber);
	form.underlayIndent = indent;
	if ((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U) {
		form::refilfn();
	}
}

void xt::dufind(float indent) {
	thred::savdo();
	indent *= PFGRAN;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::findfn(ClosestFormToCursor, indent);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::findfn(selectedForm, indent);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::internal::fangfn(uint32_t formNumber, float angle) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.type == FRMFPOLY && (form.fillType != 0U)) {
		switch (form.fillType) {
		case VRTF:
		case HORF:
		case ANGF: {
			form.fillType              = ANGF;
			form.angleOrClipData.angle = angle;
			break;
		}
		case VCLPF:
		case HCLPF:
		case ANGCLPF: {
			form.fillType           = ANGCLPF;
			form.satinOrAngle.angle = angle;
			break;
		}
		case TXVRTF:
		case TXHORF:
		case TXANGF: {
			form.fillType              = TXANGF;
			form.angleOrClipData.angle = angle;
			break;
		}
		}
		form::refilfn();
	}
}

void xt::dufxang(float angle) {
	thred::savdo();
	angle *= PI_F / 180.0F;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fangfn(ClosestFormToCursor, angle);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fangfn(selectedForm, angle);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfang() {
	displayText::tabmsg(IDS_SETFANG);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFANG);
	displayText::numWnd();
}

void xt::internal::ucolfn(uint32_t formNumber, uint32_t color) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if ((form.extendedAttribute & (AT_UND | AT_WALK | AT_CWLK)) != 0U) {
		form.underlayColor = gsl::narrow<uint8_t>(color);
		form::refilfn();
	}
}

void xt::dundcol(uint32_t color) {
	thred::savdo();
	if (color != 0U) {
		color--;
	}
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::ucolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::ucolfn(selectedForm, color);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setucol() {
	displayText::tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETUCOL);
	displayText::numWnd();
}

void xt::internal::fcolfn(uint32_t formNumber, uint32_t color) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.fillType != 0U) {
		form.fillColor = gsl::narrow<uint8_t>(color);
		form::refilfn();
	}
}

void xt::dufcol(uint32_t color) {
	thred::savdo();
	if (color != 0U) {
		color--;
	}
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fcolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fcolfn(selectedForm, color);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfcol() {
	displayText::tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFCOL);
	displayText::numWnd();
}

void xt::internal::bcolfn(uint32_t formNumber, uint32_t color) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.edgeType != 0U) {
		form.borderColor = gsl::narrow<uint8_t>(color);
		form::refilfn();
	}
}

void xt::dubcol(uint32_t color) {
	thred::savdo();
	if (color != 0U) {
		color--;
	}
	color &= COLMSK;
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bcolfn(ClosestFormToCursor, color);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bcolfn(selectedForm, color);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbcol() {
	displayText::tabmsg(IDS_COL);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBCOL);
	displayText::numWnd();
}

void xt::internal::blenfn(FRMHED& form, float length) {
	if ((form.edgeType != 0U) && !clip::iseclp(form)) {
		form.lengthOrCount.stitchLength = length;
		form::refilfn();
	}
}

void xt::dublen(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		auto& form = FormList->operator[](ClosestFormToCursor);
		xi::blenfn(form, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			auto& form = FormList->operator[](selectedForm);
			xi::blenfn(form, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setblen() {
	displayText::tabmsg(IDS_SETFLEN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBLEN);
	displayText::numWnd();
}

void xt::internal::bspacfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.edgeType != 0U) {
		form.edgeSpacing = length;
		form::refilfn();
	}
}

void xt::dubspac(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bspacfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bspacfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbspac() {
	displayText::tabmsg(IDS_SETFSPAC);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBSPAC);
	displayText::numWnd();
}

void xt::internal::bminfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.edgeType != 0U) {
		form.minBorderStitchLen = length;
		form::refilfn();
	}
}

void xt::dubmin(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bminfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bminfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbmin() {
	displayText::tabmsg(IDS_TXT23);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMIN);
	displayText::numWnd();
}

void xt::internal::bmaxfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.edgeType != 0U) {
		form.maxBorderStitchLen = length;
		form::refilfn();
	}
}

void xt::dubmax(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::bmaxfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::bmaxfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setbmax() {
	displayText::tabmsg(IDS_TXT22);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETBMAX);
	displayText::numWnd();
}

void xt::internal::fminfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.fillType != 0U) {
		form.minFillStitchLen = length;
		form::refilfn();
	}
}

void xt::dufmin(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fminfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fminfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfmin() {
	displayText::tabmsg(IDS_TXT21);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMIN);
	displayText::numWnd();
}

void xt::internal::fmaxfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form = FormList->operator[](formNumber);
	if (form.fillType != 0U) {
		form.maxFillStitchLen = length;
		form::refilfn();
	}
}

void xt::dufmax(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fmaxfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fmaxfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfmax() {
	displayText::tabmsg(IDS_TXT20);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFMAX);
	displayText::numWnd();
}

void xt::internal::fwidfn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form                       = FormList->operator[](formNumber);
	const auto             reference = form.rectangle.left;
	const auto             ratio     = length / (form.rectangle.right - reference);
	auto                   vertexIt  = std::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		vertexIt[iVertex].x = (vertexIt[iVertex].x - reference) * ratio + reference;
	}
	form::frmout(formNumber);
	form::refilfn();
}

void xt::dufwid(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fwidfn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fwidfn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfwid() {
	displayText::tabmsg(IDS_WID);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFWID);
	displayText::numWnd();
}

void xt::setfind() {
	displayText::tabmsg(IDS_UWLKIND);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFIND);
	displayText::numWnd();
}

void xt::internal::fhifn(uint32_t formNumber, float length) {
	ClosestFormToCursor = formNumber;
	auto& form                       = FormList->operator[](formNumber);
	const auto             reference = form.rectangle.bottom;
	const auto             ratio     = length / (form.rectangle.top - reference);
	auto                   vertexIt  = std::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		vertexIt[iVertex].y = (vertexIt[iVertex].y - reference) * ratio + reference;
	}
	form::frmout(formNumber);
	form::refilfn();
}

void xt::dufhi(float length) {
	thred::savdo();
	if (StateMap.test(StateFlag::FORMSEL)) {
		xi::fhifn(ClosestFormToCursor, length);
	}
	else {
		for (auto selectedForm : (*SelectedFormList)) {
			xi::fhifn(selectedForm, length);
		}
	}
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
}

void xt::setfhi() {
	displayText::tabmsg(IDS_HI);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::FSETFHI);
	displayText::numWnd();
}

void xt::setfilstrt() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		auto& form     = FormList->operator[](ClosestFormToCursor);
		form.fillStart = gsl::narrow<uint16_t>(ClosestVertexToCursor);
		form.extendedAttribute |= AT_STRT;
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_FORMP, IDS_FSTRT);
	}
}

void xt::setfilend() {
	if (StateMap.test(StateFlag::FRMPSEL)) {
		auto& form   = FormList->operator[](ClosestFormToCursor);
		form.fillEnd = gsl::narrow<uint16_t>(ClosestVertexToCursor);
		form.extendedAttribute |= AT_END;
		form::refil();
		thred::coltab();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		displayText::shoseln(IDS_FORMP, IDS_FEND);
	}
}

void xt::duauxnam() {
	*AuxName = *WorkingFileName;
	switch (IniFile.auxFileType) {
	case AUXDST: {
		AuxName->replace_extension(".dst");
		break;
	}
#if PESACT
	case AUXPES: {
		AuxName->replace_extension(".pes");
		break;
	}
#endif
	default: {
		AuxName->replace_extension("pcs");
		break;
	}
	}
}

void xt::internal::rtrclpfn(const FRMHED& form) {
	if (OpenClipboard(ThrEdWindow) != 0) {
		auto count    = 0U;
		auto clipRect = fRECTANGLE {};
		if (clip::iseclp(form)) {
			count = form.clipEntries;
			clip::oclp(clipRect, form.borderClipData, count);
		}
		else {
			if (clip::isclp(form)) {
				count = form.lengthOrCount.clipCount;
				clip::oclp(clipRect, form.angleOrClipData.clip, count);
			}
		}
		if (count != 0U) {
			LowerLeftStitch.x = 0.0F;
			LowerLeftStitch.y = 0.0F;
			EmptyClipboard();
			Clip        = RegisterClipboardFormat(PcdClipFormat);
			ClipPointer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, count * sizeof(CLPSTCH) + 2U); // NOLINT
			if (ClipPointer != nullptr) {
				ClipStitchData = *(gsl::narrow_cast<CLPSTCH**>(ClipPointer));
				thred::rtclpfn(0, 0);
				ClipStitchData[0].led = count;
				for (auto iStitch = 1U; iStitch < count; iStitch++) {
					thred::rtclpfn(iStitch, iStitch);
				}
				SetClipboardData(Clip, ClipPointer);
			}
			CloseClipboard();
		}
	}
}

void xt::rtrclp() {
	if (StateMap.test(StateFlag::FORMSEL)) {
		const auto& form = FormList->operator[](ClosestFormToCursor);
		if (texture::istx(ClosestFormToCursor)) {
			texture::rtrtx(form);
		}
		else {
			xi::rtrclpfn(form);
		}
	}
}

void xt::internal::setstxt(uint32_t stringIndex, float value, HWND dialog) {
	SetWindowText(GetDlgItem(dialog, stringIndex), fmt::format(L"{:.2f}", (value / PFGRAN)).c_str());
}

auto xt::internal::getstxt(uint32_t stringIndex, HWND dialog) -> float {
	// ToDo - This is not great code.
	wchar_t buffer[16] = {};
	GetWindowText(GetDlgItem(dialog, stringIndex), static_cast<LPTSTR>(buffer), sizeof(buffer) / sizeof(buffer[0]));
	return wrap::bufToFloat(&buffer[0]) * PFGRAN;
}

auto xt::internal::chkasp(fPOINT& point, float aspectRatio, HWND dialog) -> bool {
	point.x = getstxt(IDC_DESWID, dialog);
	point.y = getstxt(IDC_DESHI, dialog);
	// ToDo - should this have a range? aspectRatio +/- %
	return (point.y / point.x) == aspectRatio;
}

auto CALLBACK xt::internal::setsprc(HWND hwndlg, UINT umsg, WPARAM wparam, LPARAM lparam) -> BOOL {
	UNREFERENCED_PARAMETER(lparam);

	auto designSizeDialog = hwndlg; // change design size dialog window

	switch (umsg) {
	case WM_INITDIALOG: {
		SendMessage(hwndlg, WM_SETFOCUS, 0, 0);
		setstxt(IDC_DESWID, DesignSize.x, designSizeDialog);
		setstxt(IDC_DESHI, DesignSize.y, designSizeDialog);
		CheckDlgButton(hwndlg, IDC_REFILF, gsl::narrow_cast<UINT>(UserFlagMap.test(UserFlag::CHREF)));
		break;
	}
	case WM_COMMAND: {
		switch (LOWORD(wparam)) { // NOLINT
		case IDCANCEL: {
			EndDialog(hwndlg, 0);
			return TRUE;
		}
		case IDOK: {
			DesignSize.x = getstxt(IDC_DESWID, designSizeDialog);
			DesignSize.y = getstxt(IDC_DESHI, designSizeDialog);
			if (IsDlgButtonChecked(hwndlg, IDC_REFILF) != 0U) {
				UserFlagMap.set(UserFlag::CHREF);
			}
			else {
				UserFlagMap.reset(UserFlag::CHREF);
			}
			EndDialog(hwndlg, 1);
			return TRUE;
		}
		case IDC_DESWID: {
			if ((wparam >> 16U) == EN_CHANGE) {
				StateMap.reset(StateFlag::DESCHG);
			}
			break;
		}
		case IDC_DESHI: {
			if ((wparam >> 16U) == EN_CHANGE) {
				StateMap.set(StateFlag::DESCHG);
			}
			break;
		}
		case IDC_DUASP: {
			const auto designAspectRatio = DesignSize.y / DesignSize.x;
			auto       designSize        = fPOINT {};
			if (!chkasp(designSize, designAspectRatio, designSizeDialog)) {
				if (StateMap.test(StateFlag::DESCHG)) {
					setstxt(IDC_DESWID, designSize.y / designAspectRatio, designSizeDialog);
				}
				else {
					setstxt(IDC_DESHI, designSize.x * designAspectRatio, designSizeDialog);
				}
			}
			break;
		}
		}
	}
	default: {
	}
	}
	return 0;
}

void xt::internal::sadj(fPOINTATTR& stitch, const fPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	stitch.x = (stitch.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	stitch.y = (stitch.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xt::internal::sadj(fPOINT& point, const fPOINT& designSizeRatio, const fRECTANGLE& designSizeRect) noexcept {
	point.x = (point.x - designSizeRect.left) * designSizeRatio.x + designSizeRect.left;
	point.y = (point.y - designSizeRect.bottom) * designSizeRatio.y + designSizeRect.bottom;
}

void xt::internal::nudfn(const fRECTANGLE& designSizeRect) {
	const auto newSize = fPOINT { (designSizeRect.right - designSizeRect.left), (designSizeRect.top - designSizeRect.bottom) };
	const auto designSizeRatio = fPOINT { (DesignSize.x / newSize.x), (DesignSize.y / newSize.y) };
	for (auto& stitch : *StitchBuffer) {
		sadj(stitch, designSizeRatio, designSizeRect);
	}
	for (auto& FormVertice : *FormVertices) {
		sadj(FormVertice, designSizeRatio, designSizeRect);
	}
	form::frmout(ClosestFormToCursor);
}

void xt::nudsiz() {
	auto flag           = 0;
	auto designSizeRect = fRECTANGLE {}; // design size rectangle

	thred::savdo();
	flag = 0;
	if (!StitchBuffer->empty()) {
		thred::stchrct(designSizeRect);
		flag = 1;
	}
	else {
		if (!FormList->empty()) {
			thred::frmrct(designSizeRect);
			flag = 2;
		}
	}
	if (flag != 0) {
		DesignSize.x = designSizeRect.right - designSizeRect.left;
		DesignSize.y = designSizeRect.top - designSizeRect.bottom;
		if (DialogBox(ThrEdInstance, MAKEINTRESOURCE(IDD_SIZ), ThrEdWindow, reinterpret_cast<DLGPROC>(xi::setsprc))) { // NOLINT
			flag = 0;
			if (DesignSize.x > IniFile.hoopSizeX) {
				IniFile.hoopSizeX = DesignSize.x * 1.05F;
				UnzoomedRect.x    = wrap::round<int32_t>(IniFile.hoopSizeX);
				flag              = 1;
			}
			if (DesignSize.y > IniFile.hoopSizeY) {
				IniFile.hoopSizeY = DesignSize.y * 1.05F;
				UnzoomedRect.y    = wrap::round<int32_t>(IniFile.hoopSizeY);
				flag              = 1;
			}
			xi::nudfn(designSizeRect);
			if (UserFlagMap.test(UserFlag::CHREF)) {
				form::refilal();
			}
			if (flag != 0) {
				thred::movStch();
				thred::zumhom();
				displayText::hsizmsg();
			}
			form::centir();
			for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
				form::frmout(iForm);
			}
		}
	}
}

void xt::dushft() {
	//	StateMap.set(StateFlag::BOXSLCT);
	StateMap.set(StateFlag::BZUMIN);
	//	StateMap.set(StateFlag::NOSEL);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
	thred::px2stch();
	ZoomBoxOrigin = SelectedPoint;
}

void xt::mvshft() {
	if (StateMap.test(StateFlag::BOXZUM) && StateMap.testAndSet(StateFlag::VCAPT)) {
		SetCapture(ThrEdWindow);
	}
	if (StateMap.test(StateFlag::BZUMIN)) {
		if ((Msg.wParam & MK_LBUTTON) != 0U) { // NOLINT
			if (StateMap.testAndSet(StateFlag::VCAPT)) {
				SetCapture(ThrEdWindow);
			}
			thred::unbBox();
			ZoomBoxLine[1].x = ZoomBoxLine[2].x = Msg.pt.x - StitchWindowOrigin.x;
			ZoomBoxLine[2].y = ZoomBoxLine[3].y = Msg.pt.y - StitchWindowOrigin.y;
			StateMap.set(StateFlag::BZUM);
			thred::bBox();
		}
	}
}

void xt::setclpspac() {
	displayText::msgflt(IDS_CLPSPAC, IniFile.clipOffset / PFGRAN);
	StateMap.set(StateFlag::NUMIN);
	StateMap.set(StateFlag::SCLPSPAC);
	displayText::numWnd();
}

auto CALLBACK xt::internal::enumch(HWND hwnd, LPARAM lParam) noexcept -> BOOL {
	UNREFERENCED_PARAMETER(lParam);

	DestroyWindow(hwnd);
	return TRUE;
}

void xt::clrstch() noexcept {
	while (EnumChildWindows(MainStitchWin, xi::enumch, 0) != 0) {
		;
	}
}

void xt::chgwrn() {
	UserFlagMap.flip(UserFlag::WRNOF);
	thred::wrnmen();
	StateMap.set(StateFlag::DUMEN);
}

void xt::chgchk(uint8_t code) {
	IniFile.dataCheck = code;
	thred::chkmen();
	StateMap.set(StateFlag::DUMEN);
}

void xt::tst() {
	DesignerName->assign(L"Coder");
	ThrName->assign(*DesignerName);
	StateMap.set(StateFlag::RESTCH);
}
