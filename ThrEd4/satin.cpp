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
	if (formList[formIndex].type == SAT && (formList[formIndex].satinGuideCount != 0U)) {
	  auto eraseStart = std::next(SatinGuides->cbegin(), formList[formIndex].satinOrAngle.guide);
	  auto eraseEnd   = std::next(eraseStart, formList[formIndex].satinGuideCount);
	  SatinGuides->erase(eraseStart, eraseEnd);
	  for (auto iForm = formIndex + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		if (formList[iForm].type == SAT && (formList[iForm].satinGuideCount != 0U) &&
		    (formList[iForm].satinOrAngle.guide >= formList[formIndex].satinGuideCount)) {
		  formList[iForm].satinOrAngle.guide -= formList[formIndex].satinGuideCount;
		}
	  }
	}
  }
  formList[formIndex].satinGuideCount = 0;
}

void satin::internal::sacspac(uint32_t startGuide, uint32_t guideCount) {
  auto const val = SATCON {};
  auto       pos = std::next(SatinGuides->cbegin(), startGuide);
  SatinGuides->insert(pos, val);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	auto& form = FormList->operator[](iForm);
	if (form.type == SAT) {
	  form.satinOrAngle.guide += guideCount;
	}
  }
}

auto satin::internal::nusac(uint32_t formIndex, uint32_t guideCount) -> uint32_t {
  auto        guideIndex = 0U;
  auto const& formList   = *FormList;

  for (auto iForm = 0U; iForm < formIndex; iForm++) {
	auto& form = formList[iForm];
	if (form.type == SAT) {
	  guideIndex += form.satinGuideCount;
	}
  }
  si::sacspac(guideIndex, guideCount);
  return guideIndex;
}

void satin::spltsat(uint32_t guideIndex) {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  auto guideIt      = std::next(SatinGuides->begin(), currentForm.satinOrAngle.guide);
  // We are adding two additional vertices when splitting the form
  auto vertexBuffer = std::vector<fPOINT> {};
  vertexBuffer.resize(wrap::toSize(currentForm.vertexCount) + 2U);

  FormList->insert(std::next(FormList->cbegin(), ClosestFormToCursor), currentForm);
  auto& form = FormList->operator[](ClosestFormToCursor); // insert may have invalidated
  auto const             maxForm = FormList->size();
  auto position = std::next(FormVertices->cbegin(), wrap::toSize(form.vertexIndex) + form.vertexCount);
  FormVertices->insert(position, 2, fPOINT {});
  for (auto iForm = ClosestFormToCursor + 2U; iForm < maxForm; iForm++) {
	FormList->operator[](iForm).vertexIndex += 2;
  }
  auto       iOldVertex   = 0U;
  auto       currentGuide = guideIt[guideIndex];
  auto const oldLastVertex = currentGuide.start + (currentForm.vertexCount - currentGuide.finish) + 1U;
  auto       iNewVertex    = oldLastVertex + 1U;
  auto       vertexIt      = std::next(FormVertices->begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; iVertex++) {
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
  for (auto iVertex = 0U; iVertex < iNewVertex; iVertex++) {
	vertexIt[iVertex] = vertexBuffer[iVertex];
  }
  form.vertexCount     = iOldVertex;
  auto& nextForm       = FormList->operator[](wrap::toSize(ClosestFormToCursor) + 1U);
  nextForm.vertexCount = iNewVertex - iOldVertex;
  nextForm.vertexIndex = form.vertexIndex + iOldVertex;
  form::frmout(ClosestFormToCursor);
  form::frmout(ClosestFormToCursor + 1U);
  auto iNewGuide = 1U;
  iNewGuide += currentGuide.start - currentGuide.finish;
  auto iGuide = 0U;
  for (iGuide = 0U; iGuide < guideIndex; iGuide++) {
	guideIt[iGuide].finish += iNewGuide;
  }
  if (form.wordParam != 0U) {
	form.wordParam = currentGuide.start;
  }
  iNewGuide = iGuide + 1U;
  while (iGuide < (form.satinGuideCount)) {
	guideIt[iGuide].start -= (currentGuide.start - 1);
	guideIt[iGuide].finish -= (currentGuide.start - 1);
	iGuide++;
  }
  if (nextForm.wordParam != 0U) {
	nextForm.wordParam -= (currentGuide.start - 1);
  }
  auto const offset = form.satinOrAngle.guide + iNewGuide - 1U;
  auto       pos    = std::next(SatinGuides->cbegin(), offset);
  SatinGuides->erase(pos);
  nextForm.satinOrAngle.guide = form.satinOrAngle.guide + guideIndex;
  nextForm.satinGuideCount    = form.satinGuideCount - guideIndex - 1U;
  form.satinGuideCount        = guideIndex;
  for (auto iForm = ClosestFormToCursor + 2U; iForm < maxForm; iForm++) {
	auto& formIter = FormList->operator[](iForm);
	if ((formIter.type == SAT) && (formIter.satinGuideCount != 0U) && formIter.satinOrAngle.guide != 0U) {
	  formIter.satinOrAngle.guide--;
	}
  }
  if (clip::iseclp(form)) {
	form::clpspac(form.borderClipData, form.clipEntries);
	for (auto iForm = ClosestFormToCursor + 1U; iForm < maxForm; iForm++) {
	  auto& formIter = FormList->operator[](iForm);
	  formIter.borderClipData += form.clipEntries;
	}
  }
  form::stchadj();
}

void satin::internal::satclos() {
  auto& form                               = FormList->operator[](ClosestFormToCursor);
  auto const             initialGuideCount = form.satinGuideCount;
  auto                   minimumLength     = 1e99;

  form::uninsf();
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	auto const deltaX = stitchPoint.x - vertexIt[iVertex].x;
	auto const deltaY = stitchPoint.y - vertexIt[iVertex].y;
	auto const length = hypot(deltaX, deltaY);
	if (length < minimumLength) {
	  minimumLength         = length;
	  ClosestVertexToCursor = iVertex;
	}
  }
  StateMap.reset(StateFlag::SATCNKT);
  if (form.type == FRMLINE) {
	form.fillType      = CONTF;
	auto closestVertex = ClosestVertexToCursor;
	if (StartPoint > closestVertex) {
	  std::swap(closestVertex, StartPoint);
	}
	if (StartPoint == 0U) {
	  StartPoint++;
	}
	if (StartPoint == form.vertexCount - 2 && closestVertex == (form.vertexCount) - 1) {
	  StartPoint    = 1;
	  closestVertex = form.vertexCount - 2U;
	}
	if (closestVertex >= (form.vertexCount) - 2U) {
	  closestVertex = form.vertexCount - 2U;
	  if (StartPoint >= (form.vertexCount) - 2U) {
		StartPoint = form.vertexCount - 2U;
	  }
	}
	if (closestVertex - StartPoint < 2U) {
	  closestVertex = StartPoint + 2U;
	  if (closestVertex > (form.vertexCount) - 2) {
		closestVertex = closestVertex - form.vertexCount - 2U;
		closestVertex -= closestVertex;
		StartPoint -= closestVertex;
	  }
	}
	form.angleOrClipData.guide.start  = StartPoint;
	form.angleOrClipData.guide.finish = closestVertex;
  }
  else {
	auto closestVertex = StartPoint;

	if (ClosestVertexToCursor < closestVertex) {
	  std::swap(ClosestVertexToCursor, closestVertex);
	}
	if (closestVertex == 0 && ClosestVertexToCursor == form.vertexCount - 1) {
	  closestVertex         = form.vertexCount - 1U;
	  ClosestVertexToCursor = form.vertexCount;
	}
	if (closestVertex == 1 && ClosestVertexToCursor == form.vertexCount) {
	  closestVertex         = 0;
	  ClosestVertexToCursor = 1;
	}
	if (ClosestVertexToCursor - closestVertex == 1) {
	  if ((form.attribute & FRMEND) != 0U) {
		form.wordParam = closestVertex;
	  }
	  else {
		if (closestVertex != 0U) {
		  form::rotfrm(form, closestVertex);
		}
		form.attribute |= FRMEND;
	  }
	  satin::satadj(form);
	}
	else {
	  if (form.satinGuideCount != 0U) {
		si::sacspac(form.satinOrAngle.guide + form.satinGuideCount, 1);
		auto guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide);

		guideIt[form.satinGuideCount].start    = closestVertex;
		guideIt[form.satinGuideCount++].finish = ClosestVertexToCursor;
		satin::satadj(form);
	  }
	  else {
		form.satinOrAngle.guide = si::nusac(ClosestFormToCursor, 1);

		auto guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide);

		guideIt[initialGuideCount].start = closestVertex;
		// cppcheck-suppress unreadVariable
		guideIt[initialGuideCount].finish = ClosestVertexToCursor;
		form.satinGuideCount              = 1;
	  }
	}
  }
}

void satin::satknkt() {
  si::satclos();
  form::refil();
  StateMap.set(StateFlag::RESTCH);
}

auto satin::internal::satselfn() -> bool {
  auto minimumLength = 1e99;

  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	auto& form = FormList->operator[](iForm);
	auto const             layerCode =
	    gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(form.attribute & FRMLMSK) >> 1U);
	if ((ActiveLayer == 0U) || (layerCode == 0U) || layerCode == ActiveLayer) {
	  auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
		auto const deltaX = stitchPoint.x - vertexIt[iVertex].x;
		auto const deltaY = stitchPoint.y - vertexIt[iVertex].y;
		auto const length = hypot(deltaX, deltaY);
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
	auto& form = FormList->operator[](ClosestFormToCursor);
	thred::duzrat();
	StartPoint    = ClosestVertexToCursor;
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	FormLines->front() = form::sfCor2px(vertexIt[ClosestVertexToCursor]);
	StateMap.reset(StateFlag::SHOCON);
	StateMap.set(StateFlag::SATCNKT);
	if (form.type == FRMFPOLY) {
	  form.type = SAT;
	}
  }
}

void satin::internal::satcpy(FRMHED const& form, std::vector<SATCON> const& source, uint32_t size) {
  auto const& currentFormGuides = form.satinOrAngle.guide;
  auto        eraseStart        = std::next(SatinGuides->cbegin(), currentFormGuides);
  auto        eraseEnd          = std::next(eraseStart, (form.satinGuideCount - size));
  SatinGuides->erase(eraseStart, eraseEnd);
  auto guideIt = std::next(SatinGuides->begin(), currentFormGuides); // iterator may be invalidated by erase
  std::copy(source.cbegin(), source.cend(), guideIt);
}

auto satin::scomp(SATCON const& arg1, SATCON const& arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (arg1.start < arg2.start) {
	return true;
  }
  if (arg2.start < arg1.start) {
	return false;
  }
  if (arg1.finish < arg2.finish) {
	return true;
  }
  if (arg2.finish < arg1.finish) {
	return false;
  }
  return false;
}

void satin::satadj(FRMHED& form) {
  auto& currentGuidesCount = form.satinGuideCount;
  auto  interiorGuides     = std::vector<SATCON> {};
  interiorGuides.reserve(currentGuidesCount);
  auto const savedGuideCount = form.satinGuideCount;
  auto       satinMap        = ExtendedBitSet<>(form.vertexCount);

  // ensure all guide endpoints are on valid vertices
  auto guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
  for (auto iGuide = 0U; iGuide < form.satinGuideCount; iGuide++) {
	auto const endCount = (form.vertexCount - 1);
	if (guideIt[iGuide].finish > endCount) {
	  guideIt[iGuide].finish = endCount;
	}
	if (guideIt[iGuide].start > endCount) {
	  guideIt[iGuide].start = endCount;
	}
  }

  // remove any guides of 0 length
  for (auto iSource = 0U; iSource < currentGuidesCount; iSource++) {
	if (guideIt[iSource].start != guideIt[iSource].finish) {
	  interiorGuides.push_back(guideIt[iSource]);
	}
  }
  auto iDestination = wrap::toUnsigned(interiorGuides.size());
  if (currentGuidesCount > iDestination) {
	OutputDebugString(
	    fmt::format(L"Removed {} zero distance guides\n", (currentGuidesCount - iDestination)).c_str());
	si::satcpy(form, interiorGuides, iDestination);
	currentGuidesCount = iDestination;
  }
  auto const& endGuide = form.wordParam;
  if ((endGuide != 0U) || ((form.attribute & FRMEND) != 0U)) {
	// there are end guides so set the satinMap for the next step
	satinMap.reset();
	if ((form.attribute & FRMEND) != 0U) {
	  satinMap.set(0);
	  satinMap.set(1);
	}
	if (endGuide != 0U) {
	  satinMap.set(endGuide);
	  satinMap.set((wrap::toSize(endGuide) + 1U) % form.vertexCount);
	}
	// check to see if any of the current guides are end guides and add to interiorGuides if not
	interiorGuides.clear();
	guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
	for (auto iSource = 0U; iSource < currentGuidesCount; iSource++) {
	  if (!satinMap.test(guideIt[iSource].start) && !satinMap.test(guideIt[iSource].finish)) {
		interiorGuides.push_back(guideIt[iSource]);
	  }
	}
	iDestination = gsl::narrow<decltype(iDestination)>(interiorGuides.size());
	if (currentGuidesCount > iDestination) {
	  OutputDebugString(fmt::format(L"Removed {} end guides\n", (currentGuidesCount - iDestination)).c_str());
	  si::satcpy(form, interiorGuides, iDestination);
	  currentGuidesCount = iDestination;
	}
	// remove any guides that start after the end guide
	if (endGuide != 0U) {
	  interiorGuides.clear();
	  guideIt = std::next(SatinGuides->begin(), form.satinOrAngle.guide); // iterator may be invalidated by erase
	  for (auto iSource = 0U; iSource < currentGuidesCount; iSource++) {
		if (guideIt[iSource].start < endGuide) {
		  interiorGuides.push_back(guideIt[iSource]);
		}
	  }
	  iDestination = gsl::narrow<decltype(iDestination)>(interiorGuides.size());
	  if (currentGuidesCount > iDestination) {
		OutputDebugString(
		    fmt::format(L"Removed {} reversed guides\n", (currentGuidesCount - iDestination)).c_str());
		si::satcpy(form, interiorGuides, iDestination);
		form.satinGuideCount = iDestination;
		currentGuidesCount   = iDestination;
	  }
	}
  }

  if (currentGuidesCount != 0U && endGuide != 0) {
	satinMap.reset();
	for (auto iGuide = 0U; iGuide < currentGuidesCount; iGuide++) {
	  auto iForward = guideIt[iGuide].start;
	  if (iForward > endGuide - 1) {
		iForward = endGuide - 1U;
	  }
	  if (satinMap.test_set(iForward)) {
		auto iReverse = iForward;
		if (iReverse != 0U) {
		  iReverse--;
		}
		while (satinMap.test(iForward) && (iForward < ((endGuide)-1))) {
		  iForward++;
		}
		while ((iReverse != 0U) && (satinMap.test(iReverse))) {
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
	auto iGuide  = 0U;
	auto iVertex = 0U;
	do {
	  iVertex = wrap::toUnsigned(satinMap.getFirst());
	  if (iVertex < form.vertexCount) {
		guideIt[iGuide++].start = iVertex;
	  }
	} while (iVertex < form.vertexCount);

	currentGuidesCount = iGuide;
	satinMap.reset();
	for (iGuide = 0U; iGuide < currentGuidesCount; iGuide++) {
	  auto iForward = guideIt[iGuide].finish;
	  auto iReverse = iForward;
	  if (iForward > form.vertexCount - 1U) {
		iForward = form.vertexCount - 1U;
	  }
	  if (satinMap.test_set(iForward)) {
		if (iForward < form.vertexCount - 1U) {
		  iForward++;
		}
		if (iReverse > (endGuide) + 1U) {
		  iReverse--;
		}
		while (satinMap.test(iForward) && iForward < form.vertexCount - 1U) {
		  iForward++;
		}
		while (iReverse > (endGuide)-1 && (satinMap.test(iReverse))) {
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
	iGuide        = 0U;
	auto iReverse = 0U;
	do {
	  iReverse = wrap::toUnsigned(satinMap.getLast());
	  if (iReverse < form.vertexCount) {
		guideIt[iGuide++].finish = iReverse;
	  }
	} while (iReverse < form.vertexCount);
	if (iGuide < currentGuidesCount) {
	  iGuide = currentGuidesCount;
	}
	currentGuidesCount = iGuide;
	if (endGuide != 0U) {
	  auto const end0 = (form.vertexCount - endGuide - 2U);
	  auto const end1 = (endGuide - 2U);
	  if (currentGuidesCount > end0) {
		currentGuidesCount = end0;
	  }
	  if (currentGuidesCount > end1) {
		currentGuidesCount = end1;
	  }
	}
  }
  else {
	// sort the guides
	auto start = std::next(SatinGuides->begin(), form.satinOrAngle.guide);
	auto end   = std::next(start, form.satinGuideCount);
	std::sort(start, end, satin::scomp);
  }
  if (form.satinGuideCount < savedGuideCount) {
	auto const iGuide = savedGuideCount - currentGuidesCount;
	OutputDebugString(fmt::format(L"Guides adjusted by {}, so updating forms\n", iGuide).c_str());
	for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	  auto& afterForm = FormList->operator[](iForm);
	  if ((afterForm.type == SAT) && (afterForm.satinOrAngle.guide >= iGuide)) {
		afterForm.satinOrAngle.guide -= iGuide;
	  }
	}
  }
}

void satin::delcon(FRMHED& form, uint32_t GuideIndex) {
  auto const offset = form.satinOrAngle.guide + GuideIndex;
  auto       guide  = std::next(SatinGuides->cbegin(), offset);

  SatinGuides->erase(guide);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	auto& afterForm = FormList->operator[](iForm);
	if ((afterForm.type == SAT) && (afterForm.satinGuideCount != 0U) && (afterForm.satinOrAngle.guide != 0U)) {
	  afterForm.satinOrAngle.guide--;
	}
  }
  form.satinGuideCount--;
  if (form.fillType == SATF) {
	form::refil();
  }
  thred::coltab();
  StateMap.set(StateFlag::RESTCH);
}

void satin::delspnt() {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (form::chkdel(currentForm)) {
	StateMap.set(StateFlag::DELTO);
	thred::frmdel();
	StateMap.reset(StateFlag::FRMPSEL);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (currentForm.type == SAT) {
	if (ClosestVertexToCursor < currentForm.wordParam) {
	  currentForm.wordParam--;
	}
	if (currentForm.satinGuideCount != 0U) {
	  auto guideIt = std::next(SatinGuides->begin(), currentForm.satinOrAngle.guide);
	  auto iGuide  = 0U;
	  while (iGuide < currentForm.satinGuideCount) {
		if (guideIt[iGuide].start != ClosestVertexToCursor && guideIt[iGuide].finish != ClosestVertexToCursor) {
		  iGuide++;
		}
		else {
		  break;
		}
	  }
	  if (iGuide < currentForm.satinGuideCount && (guideIt[iGuide].start == ClosestVertexToCursor ||
	                                               guideIt[iGuide].finish == ClosestVertexToCursor)) {
		while (iGuide < currentForm.satinGuideCount) {
		  guideIt[iGuide] = guideIt[wrap::toSize(iGuide) + 1U];
		  iGuide++;
		}
		currentForm.satinGuideCount--;
		for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		  auto& form = FormList->operator[](iForm);
		  if (form.type == SAT && (form.satinGuideCount != 0U)) {
			form.satinOrAngle.guide++;
		  }
		}
	  }
	  for (iGuide = 0; iGuide < currentForm.satinGuideCount; iGuide++) {
		if (guideIt[iGuide].start > ClosestVertexToCursor) {
		  guideIt[iGuide].start--;
		}
		if (guideIt[iGuide].finish > ClosestVertexToCursor) {
		  guideIt[iGuide].finish--;
		}
	  }
	}
  }
  auto closestVertexIt =
      std::next(FormVertices->cbegin(), wrap::toSize(currentForm.vertexIndex) + ClosestVertexToCursor);
  FormVertices->erase(closestVertexIt);
  currentForm.vertexCount--;
  if (ClosestVertexToCursor > (currentForm.vertexCount) - 1) {
	ClosestVertexToCursor = currentForm.vertexCount - 1U;
  }
  StateMap.set(StateFlag::FRMPSEL);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	auto& form = FormList->operator[](iForm);
	form.vertexIndex--;
  }
  auto vertexIt = std::next(FormVertices->cbegin(), currentForm.vertexIndex);
  thred::ritfcor(vertexIt[ClosestVertexToCursor]);
  displayText::ritnum(STR_NUMPNT, ClosestVertexToCursor);
  form::frmout(ClosestFormToCursor);
  auto const vertex = vertexIt[ClosestVertexToCursor];
  if (vertex.x < ZoomRect.left || vertex.x > ZoomRect.right || vertex.y < ZoomRect.bottom ||
      vertex.y > ZoomRect.top) {
	thred::shft(vertex);
  }
  form::refil();
}

void satin::internal::satsbrd(uint32_t formIndex) {
  auto& currentForm = FormList->operator[](formIndex);
  clip::deleclp(ClosestFormToCursor);
  currentForm.edgeType = EDGEANGSAT;
  if (UserFlagMap.test(UserFlag::DUND)) {
	currentForm.edgeType |= EGUND;
  }
  form::bsizpar(currentForm);
  currentForm.borderSize  = BorderWidth;
  currentForm.edgeSpacing = LineSpacing / 2;
  currentForm.borderColor = gsl::narrow<uint8_t>(ActiveColor);
  form::refilfn();
}

void satin::satbrd() {
  if (displayText::filmsgs(FML_ANGS)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  if (UserFlagMap.test(UserFlag::BLUNT)) {
		form.attribute |= gsl::narrow_cast<decltype(form.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		form.attribute &= NOBLNT;
	  }
	  si::satsbrd(selectedForm);
	}
	StateMap.set(StateFlag::INIT);
	thred::coltab();
	StateMap.set(StateFlag::RESTCH);
  }
  else {
	if (StateMap.test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  if (UserFlagMap.test(UserFlag::BLUNT)) {
		form.attribute |= gsl::narrow<decltype(form.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		form.attribute &= NOBLNT;
	  }
	  si::satsbrd(ClosestFormToCursor);
	  StateMap.set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap.set(StateFlag::RESTCH);
	}
  }
}

void satin::internal::satends(FRMHED const& form, uint32_t isBlunt, float width) {
  auto const& vertexIndex = form.vertexIndex;
  auto        vertexIt    = std::next(FormVertices->cbegin(), vertexIndex);
  if ((isBlunt & SBLNT) != 0U) {
	auto step = fPOINT {sin(FormAngles->front()) * width / 2.0F, cos(FormAngles->front()) * width / 2.0F};
	if (StateMap.test(StateFlag::INDIR)) {
	  step.x = -step.x;
	  step.y = -step.y;
	}
	InsidePoints->front()  = fPOINT {vertexIt->x + step.x, vertexIt->y - step.y};
	OutsidePoints->front() = fPOINT {vertexIt->x - step.x, vertexIt->y + step.y};
  }
  else {
	InsidePoints->front() = OutsidePoints->front() = *vertexIt;
  }
  auto const& vertexCount = form.vertexCount;
  vertexIt += vertexCount - 1U;
  if ((isBlunt & FBLNT) != 0U) {
	auto step = fPOINT {sin(FormAngles->operator[](vertexCount - 2U)) * width / 2.0F,
	                    cos(FormAngles->operator[](vertexCount - 2U)) * width / 2.0F};
	if (StateMap.test(StateFlag::INDIR)) {
	  step.x = -step.x;
	  step.y = -step.y;
	}
	InsidePoints->back()  = fPOINT {vertexIt->x + step.x, vertexIt->y - step.y};
	OutsidePoints->back() = fPOINT {vertexIt->x - step.x, vertexIt->y + step.y};
  }
  else {
	InsidePoints->back() = OutsidePoints->back() = *vertexIt;
  }
}

void satin::ribon() {
  displayText::frm1pnt();
  if (StateMap.test(StateFlag::FORMSEL)) {
	auto& currentForm = FormList->operator[](ClosestFormToCursor);
	if (currentForm.vertexCount > 2) {
	  thred::savdo();
	  auto const savedFormIndex = ClosestFormToCursor;
	  satin::satout(currentForm, BorderWidth);
	  if (!FormList->empty()) {
		FormList->push_back(FRMHED {});
		auto& newForm = FormList->back();
		// reset vars as push_back may invalidate references
		currentForm                        = FormList->operator[](ClosestFormToCursor);
		auto                    iNewVertex = 0U;
		newForm.maxFillStitchLen           = 9.0F * PFGRAN;
		newForm.minFillStitchLen           = MinStitchLength;
		MaxStitchLen                       = 9.0F * PFGRAN;
		if (currentForm.type == FRMLINE) {
		  // Set blunt flags
		  auto isBlunt = 0U;
		  if (UserFlagMap.test(UserFlag::BLUNT)) {
			isBlunt = SBLNT | FBLNT;
		  }
		  si::satends(currentForm, isBlunt, BorderWidth);
		  newForm.vertexIndex      = thred::adflt(currentForm.vertexCount * 2U);
		  auto vertexIt            = std::next(FormVertices->begin(), newForm.vertexIndex);
		  vertexIt[0].x            = OutsidePoints->front().x;
		  vertexIt[iNewVertex++].y = OutsidePoints->front().y;
		  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; iVertex++) {
			vertexIt[iNewVertex++] = InsidePoints->operator[](iVertex);
		  }
		  for (auto iVertex = currentForm.vertexCount - 1U; iVertex != 0; iVertex--) {
			vertexIt[iNewVertex++] = OutsidePoints->operator[](iVertex);
		  }
		}
		else {
		  newForm.vertexIndex      = thred::adflt((currentForm.vertexCount * 2U) + 2U);
		  auto vertexIt            = std::next(FormVertices->begin(), newForm.vertexIndex);
		  vertexIt[0].x            = OutsidePoints->front().x;
		  vertexIt[iNewVertex++].y = OutsidePoints->front().y;
		  newForm.underlayIndent   = IniFile.underlayIndent;
		  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; iVertex++) {
			vertexIt[iNewVertex++] = InsidePoints->operator[](iVertex);
		  }
		  vertexIt[iNewVertex++] = InsidePoints->front();
		  vertexIt[iNewVertex++] = OutsidePoints->front();
		  for (auto iVertex = currentForm.vertexCount - 1U; iVertex != 0; iVertex--) {
			vertexIt[iNewVertex++] = OutsidePoints->operator[](iVertex);
		  }
		}
		newForm.type                       = SAT;
		newForm.fillColor                  = gsl::narrow<uint8_t>(ActiveColor);
		newForm.fillSpacing                = LineSpacing;
		newForm.lengthOrCount.stitchLength = IniFile.maxStitchLength;
		newForm.vertexCount                = iNewVertex;
		newForm.attribute                  = currentForm.attribute;
		newForm.attribute &= FRMLMSK;
		newForm.attribute |= FRMEND;
		newForm.wordParam          = iNewVertex / 2;
		newForm.satinGuideCount    = newForm.wordParam - 2U;
		newForm.satinOrAngle.guide = adsatk(newForm.satinGuideCount);
		if (StateMap.test(StateFlag::CNV2FTH)) {
		  newForm.fillType                       = FTHF;
		  newForm.fillInfo.feather.ratio         = IniFile.featherRatio;
		  newForm.fillInfo.feather.upCount       = IniFile.featherUpCount;
		  newForm.fillInfo.feather.downCount     = IniFile.featherDownCount;
		  newForm.fillInfo.feather.fillType      = IniFile.featherFillType;
		  newForm.fillInfo.feather.minStitchSize = IniFile.featherMinStitchSize;
		  newForm.extendedAttribute              = IniFile.featherType;
		  newForm.fillInfo.feather.count         = IniFile.featherCount;
		  newForm.fillInfo.feather.color         = (ActiveColor + 1U) & COLMSK;
		}
		else {
		  newForm.fillType = SATF;
		}
		auto guideIt = std::next(SatinGuides->begin(), newForm.satinOrAngle.guide);
		for (auto iGuide = 0U; iGuide < newForm.satinGuideCount; iGuide++) {
		  guideIt[iGuide].start  = iGuide + 2U;
		  guideIt[iGuide].finish = newForm.vertexCount - iGuide - 1U;
		}
		ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
		form::frmout(ClosestFormToCursor);
		form::refilfn();
		ClosestFormToCursor = savedFormIndex;
		StateMap.set(StateFlag::DELTO);
		thred::frmdel();
		ClosestFormToCursor = gsl::narrow<decltype(ClosestFormToCursor)>(FormList->size() - 1U);
		thred::coltab();
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

void satin::slbrd(FRMHED const& form) {
  auto const savedSpacing = LineSpacing;
  auto stitchPoint = fPOINT {};

  OSequence->clear();
  if ((form.edgeType & EGUND) != 0U) {
	auto const width = form.borderSize * URAT;
	satin::satout(form, width);
	si::satends(form, form.attribute, width);
	StateMap.reset(StateFlag::SAT1);
	StateMap.reset(StateFlag::FILDIR);
	LineSpacing = USPAC;
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; iVertex++) {
	  si::sbfn(*InsidePoints, iVertex, iVertex + 1U, stitchPoint);
	}
	StateMap.flip(StateFlag::FILDIR);
	for (auto iVertex = form.vertexCount - 1U; iVertex != 0; iVertex--) {
	  si::sbfn(*InsidePoints, iVertex, iVertex - 1, stitchPoint);
	}
  }
  satin::satout(form, form.borderSize);
  si::satends(form, form.attribute, form.borderSize);
  LineSpacing = form.edgeSpacing;
  StateMap.reset(StateFlag::SAT1);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; iVertex++) {
	si::sbfn(*InsidePoints, iVertex, iVertex + 1U, stitchPoint);
  }
  LineSpacing = savedSpacing;
}

void satin::internal::satfn(FRMHED const&             form,
                            std::vector<float> const& lengths,
                            uint32_t                  line1Start,
                            uint32_t                  line1End,
                            uint32_t                  line2Start,
                            uint32_t                  line2End) {
  if (line1Start != line1End && line2Start != line2End) {
	auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
	if (!StateMap.testAndSet(StateFlag::SAT1)) {
	  if (StateMap.test(StateFlag::FTHR)) {
		if (form.vertexCount != 0U) {
		  auto vertex = vertexIt[line1Start % form.vertexCount];
		  BSequence->emplace_back(BSEQPNT {vertex.x, vertex.y, 0});
		}
	  }
	  else {
		if (StateMap.test(StateFlag::BARSAT)) {
		  if (form.vertexCount != 0U) {
			auto vertex = vertexIt[line1Start % form.vertexCount];
			BSequence->emplace_back(BSEQPNT {vertex.x, vertex.y, 0});
			vertex = vertexIt[line2Start % form.vertexCount];
			BSequence->emplace_back(BSEQPNT {vertex.x, vertex.y, 0});
		  }
		}
		else {
		  OSequence->push_back(vertexIt[line1Start]);
		}
	  }
	}
	auto line1Length = lengths[line1End] - lengths[line1Start];
	auto line2Length = lengths[line2Start] - lengths[line2End];
	auto stitchCount = 0U;
	if (fabs(line1Length) > fabs(line2Length)) {
	  stitchCount = wrap::round<uint32_t>(fabs(line2Length) / LineSpacing);
	}
	else {
	  stitchCount = wrap::round<uint32_t>(fabs(line1Length) / LineSpacing);
	}
	auto const line1Segments = ((line1End > line1Start) ? (line1End - line1Start) : (line1Start - line1End));
	auto const line2Segments = ((line2Start > line2End) ? (line2Start - line2End) : (line2End - line2Start));
	auto line1StitchCounts = std::vector<uint32_t> {};
	line1StitchCounts.reserve(line1Segments);
	auto line2StitchCounts = std::vector<uint32_t> {};
	line2StitchCounts.reserve(line2Segments);
	auto iVertex            = line1Start;
	auto segmentStitchCount = 0U;
	for (auto iSegment = 0U; iSegment < line1Segments - 1U; iSegment++) {
	  auto const nextVertex = form::nxt(form, iVertex);
	  auto const val =
	      wrap::ceil<uint32_t>(((lengths[nextVertex] - lengths[iVertex]) / line1Length) * stitchCount);
	  line1StitchCounts.push_back(val);
	  segmentStitchCount += val;
	  iVertex = form::nxt(form, iVertex);
	}
	line1StitchCounts.push_back(stitchCount - segmentStitchCount);
	auto iNextVertex = line2Start;
	iVertex          = form::prv(form, iNextVertex);
	segmentStitchCount = 0;
	while (iVertex > line2End) {
	  auto const val =
	      wrap::ceil<uint32_t>(((lengths[iNextVertex] - lengths[iVertex]) / line2Length) * stitchCount);
	  line2StitchCounts.push_back(val);
	  segmentStitchCount += val;
	  iNextVertex = form::prv(form, iNextVertex);
	  iVertex     = form::prv(form, iNextVertex);
	}
	line2StitchCounts.push_back(stitchCount - segmentStitchCount);
	auto line1Point    = vertexIt[line1Start];
	auto line1Next     = form::nxt(form, line1Start);
	auto line2Previous = form::prv(form, line2Start);
	auto line1Count    = line1StitchCounts[0];
	auto line2Count    = line2StitchCounts[0];
	auto iLine1Vertex  = line1Start;
	auto iLine2Vertex  = line2Start;
	auto line1Delta    = fPOINT {vertexIt[line1Next].x - vertexIt[iLine1Vertex].x,
                              vertexIt[line1Next].y - vertexIt[iLine1Vertex].y};
	auto line2Point    = (iLine2Vertex == form.vertexCount) ? vertexIt[0] : vertexIt[iLine2Vertex];
	auto line2Delta =
	    fPOINT {vertexIt[line2Previous].x - line2Point.x, vertexIt[line2Previous].y - line2Point.y};
	iLine1Vertex     = form::nxt(form, iLine1Vertex);
	iLine2Vertex     = form::prv(form, iLine2Vertex);
	auto line1Step   = fPOINT {line1Delta.x / line1Count, line1Delta.y / line1Count};
	auto line2Step   = fPOINT {line2Delta.x / line2Count, line2Delta.y / line2Count};
	bool flag        = false;
	auto loop        = 0U;
	auto iLine1Count = 1U;
	auto iLine2Count   = 1U;
	auto stitchPoint = vertexIt[line1Start];
	do {
	  flag = false;
	  loop++;
	  if (StateMap.test(StateFlag::FTHR)) {
		while ((line1Count != 0U) && (line2Count != 0U)) {
		  line1Point.x += line1Step.x;
		  line1Point.y += line1Step.y;
		  line2Point.x += line2Step.x;
		  line2Point.y += line2Step.y;
		  if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			BSequence->emplace_back(BSEQPNT {line1Point.x, line1Point.y, 0});
		  }
		  else {
			BSequence->emplace_back(BSEQPNT {line2Point.x, line2Point.y, 1});
		  }
		  line1Count--;
		  line2Count--;
		}
	  }
	  else {
		if (StateMap.test(StateFlag::BARSAT)) {
		  while ((line1Count != 0U) && (line2Count != 0U)) {
			line1Point.x += line1Step.x;
			line1Point.y += line1Step.y;
			line2Point.x += line2Step.x;
			line2Point.y += line2Step.y;
			if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			  BSequence->emplace_back(BSEQPNT {line1Point.x, line1Point.y, 0});
			  BSequence->emplace_back(BSEQPNT {line2Point.x, line2Point.y, 1});
			}
			else {
			  BSequence->emplace_back(BSEQPNT {line2Point.x, line2Point.y, 2});
			  BSequence->emplace_back(BSEQPNT {line1Point.x, line1Point.y, 3});
			}
			line1Count--;
			line2Count--;
		  }
		}
		else {
		  while ((line1Count != 0U) && (line2Count != 0U)) {
			line1Point.x += line1Step.x;
			line1Point.y += line1Step.y;
			line2Point.x += line2Step.x;
			line2Point.y += line2Step.y;
			if (StateMap.testAndFlip(StateFlag::FILDIR)) {
			  if (UserFlagMap.test(UserFlag::SQRFIL)) {
				stitchPoint = form::filinu(line2Point, stitchPoint);
			  }
			  stitchPoint = form::filin(line1Point, stitchPoint);
			}
			else {
			  if (UserFlagMap.test(UserFlag::SQRFIL)) {
				stitchPoint = form::filinu(line1Point, stitchPoint);
			  }
			  stitchPoint = form::filin(line2Point, stitchPoint);
			}
			line1Count--;
			line2Count--;
		  }
		}
	  }
	  if ((iLine1Count < line1Segments || iLine2Count < line2Segments)) {
		if ((line1Count == 0U) && iLine1Count < line1StitchCounts.size()) {
		  line1Count   = line1StitchCounts[iLine1Count++];
		  line1Next    = form::nxt(form, iLine1Vertex);
		  line1Delta.x = vertexIt[line1Next].x - vertexIt[iLine1Vertex].x;
		  line1Delta.y = vertexIt[line1Next].y - vertexIt[iLine1Vertex].y;
		  iLine1Vertex = form::nxt(form, iLine1Vertex);
		  line1Step.x  = line1Delta.x / line1Count;
		  line1Step.y  = line1Delta.y / line1Count;
		}

		if ((line2Count == 0U) && iLine2Count < line2StitchCounts.size()) {
		  line2Count    = line2StitchCounts[iLine2Count++];
		  line2Previous = form::prv(form, iLine2Vertex);
		  line2Delta.x  = vertexIt[line2Previous].x - vertexIt[iLine2Vertex].x;
		  line2Delta.y  = vertexIt[line2Previous].y - vertexIt[iLine2Vertex].y;
		  iLine2Vertex  = form::prv(form, iLine2Vertex);
		  line2Step.x   = line2Delta.x / line2Count;
		  line2Step.y   = line2Delta.y / line2Count;
		}
		if (((line1Count != 0U) || (line2Count != 0U)) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
		  flag = true;
		}
	  }
	} while (flag && (loop < 20000U));
  }
}

void satin::internal::satmf(FRMHED const& form, std::vector<float> const& lengths) {
  auto start = 0U;

  if ((form.attribute & FRMEND) != 0U) {
	start = 1;
  }
  auto guideIt = std::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
  si::satfn(form, lengths, start, guideIt[0].start, form.vertexCount, guideIt[0].finish);
  auto endGuideIndex = form.satinGuideCount;
  if (endGuideIndex != 0U) {
	endGuideIndex--;
  }
  for (auto iGuide = 0U; iGuide < endGuideIndex; iGuide++) {
	auto& thisGuide = guideIt[iGuide];
	auto& nextGuide = guideIt[wrap::toSize(iGuide) + 1U];
	si::satfn(form, lengths, thisGuide.start, nextGuide.start, thisGuide.finish, nextGuide.finish);
  }
  auto const& endGuide = form.wordParam;
  if (endGuide != 0U) {
	si::satfn(form, lengths, guideIt[endGuideIndex].start, endGuide, guideIt[endGuideIndex].finish, endGuide + 1U);
  }
  else {
	if (guideIt[endGuideIndex].finish - guideIt[endGuideIndex].start > 2) {
	  auto length = (lengths[guideIt[endGuideIndex].finish] - lengths[guideIt[endGuideIndex].start]) / 2 +
	                lengths[guideIt[endGuideIndex].start];
	  auto iVertex = guideIt[endGuideIndex].start;
	  while (length > lengths[iVertex]) {
		iVertex++;
	  }
	  auto deltaX = lengths[iVertex] - length;
	  auto deltaY = length - lengths[iVertex - 1U];
	  if (deltaY > deltaX) {
		iVertex--;
	  }
	  si::satfn(form, lengths, guideIt[endGuideIndex].start, iVertex, guideIt[endGuideIndex].finish, iVertex);
	}
	else {
	  si::satfn(form,
	            lengths,
	            guideIt[endGuideIndex].start,
	            guideIt[endGuideIndex].start + 1,
	            guideIt[endGuideIndex].finish,
	            guideIt[endGuideIndex].start + 1U);
	}
  }
}

void satin::satfil(FRMHED& form) {
  auto const savedSpacing = LineSpacing;

  satin::satadj(form);
  LineSpacing /= 2;
  OSequence->clear();
  BSequence->clear();
  StateMap.reset(StateFlag::SAT1);
  StateMap.reset(StateFlag::FILDIR);
  form.fillType = SATF;
  auto lengths  = std::vector<float> {};
  lengths.reserve(wrap::toSize(form.vertexCount) + 1U);
  auto length = 0.0;
  lengths.push_back(length);
  auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
  for (auto iVertex = 1U; iVertex < form.vertexCount; iVertex++) {
	auto const delta = fPOINT {vertexIt[iVertex].x - vertexIt[iVertex - 1U].x,
	                           vertexIt[iVertex].y - vertexIt[iVertex - 1U].y};
	length += hypot(delta.x, delta.y);
	lengths.push_back(length);
  }
  auto const lastDelta = fPOINT {vertexIt[0].x - vertexIt[form.vertexCount - 1U].x,
                                 vertexIt[0].y - vertexIt[form.vertexCount - 1U].y};
  length += hypot(lastDelta.x, lastDelta.y);
  lengths.push_back(length);
  do {
	auto const& endGuide = form.wordParam;
	if (endGuide != 0U) {
	  if (form.satinGuideCount != 0U) {
		si::satmf(form, lengths);
		break;
	  }

	  si::satfn(form, lengths, 1, endGuide, form.vertexCount, endGuide + 1U);
	  break;
	}
	if ((form.attribute & FRMEND) != 0U) {
	  if (form.satinGuideCount != 0U) {
		si::satmf(form, lengths);
		break;
	  }

	  if (form.vertexCount == 3 && ((form.attribute & 1U) != 0)) {
		si::satfn(form, lengths, 2, 3, 2, 1);
		break;
	  }

	  length       = (length - lengths[1]) / 2.0;
	  auto iVertex = 1U;
	  if (!StateMap.test(StateFlag::BARSAT)) {
		OSequence->push_back(vertexIt[1]);
	  }
	  while ((iVertex < (form.vertexCount + 1U)) && (length > lengths[iVertex])) {
		iVertex++;
	  }
	  auto const deltaA = lengths[iVertex] - length;
	  auto const deltaB = length - lengths[iVertex - 1U];
	  if (deltaB > deltaA) {
		iVertex--;
	  }
	  si::satfn(form, lengths, 1, iVertex, form.vertexCount, iVertex);

	  break;
	}
	if (form.satinGuideCount != 0U) {
	  si::satmf(form, lengths);
	  break;
	}
	length /= 2.0;
	auto iVertex = 0;
	if (!StateMap.test(StateFlag::BARSAT) && !StateMap.test(StateFlag::FTHR)) {
	  OSequence->push_back(vertexIt[0]);
	}
	while (length > lengths[iVertex]) {
	  iVertex++;
	}
	auto const deltaA = lengths[iVertex] - length;
	auto const deltaB = length - lengths[wrap::toSize(iVertex) - 1U];
	if (deltaB > deltaA) {
	  iVertex--;
	}
	si::satfn(form, lengths, 0, iVertex, form.vertexCount, iVertex);
  } while (false);

  LineSpacing = savedSpacing;
}

void satin::satfix() {
  auto const vertexCount = wrap::toUnsigned(TempPolygon->size());
  auto&      form        = FormList->back();
  auto       minSize     = 1U;
  if (form.type == FRMFPOLY) {
	minSize = 2U;
  }
  if (TempPolygon->size() > minSize) {
	form.vertexIndex = thred::adflt(vertexCount);
	auto vertexIt    = std::next(FormVertices->begin(), form.vertexIndex);
	std::copy(TempPolygon->cbegin(), TempPolygon->cend(), vertexIt);
	TempPolygon->clear();
	form.vertexCount = vertexCount;
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
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

void satin::dusat() noexcept {
  auto const  vertexCount = TempPolygon->size();
  auto&       formLines   = *FormLines;
  auto const* line        = &formLines[vertexCount - 1U];

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
  auto const vertexCount = TempPolygon->size();
  auto&      formLines   = *FormLines;
  formLines.resize(vertexCount + 1U);
  formLines[vertexCount].x = Msg.pt.x - StitchWindowOrigin.x;
  formLines[vertexCount].y = Msg.pt.y - StitchWindowOrigin.y;
  StateMap.set(StateFlag::SHOSAT);
  satin::dusat();
}

void satin::satpnt0() {
  auto& formLines = *FormLines;
  formLines.clear();
  formLines.push_back(POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y});
  TempPolygon->push_back(thred::pxCor2stch(Msg.pt));
  StateMap.set(StateFlag::SATPNT);
}

void satin::satpnt1() {
  si::unsat();
  auto const vertexCount   = TempPolygon->size();
  auto&      formLines     = *FormLines;
  formLines[vertexCount].x = Msg.pt.x - StitchWindowOrigin.x;
  formLines[vertexCount].y = Msg.pt.y - StitchWindowOrigin.y;
  satin::dusat();
  TempPolygon->push_back(thred::pxCor2stch(Msg.pt));
  StateMap.set(StateFlag::RESTCH);
}

void satin::internal::filinsbw(std::vector<fPOINT>& satinBackup, fPOINT const& point, uint32_t& satinBackupIndex, fPOINT& stitchPoint) {
  satinBackup[satinBackupIndex++] = point;
  satinBackupIndex &= (satinBackup.size() - 1U); // make sure that satinBackupIndex rolls over when it reaches the end of the buffer
  form::filinsb(point, stitchPoint);
}

void satin::internal::sbfn(std::vector<fPOINT> const& insidePoints, uint32_t start, uint32_t finish, fPOINT& stitchPoint) {
  auto& outsidePoints = *OutsidePoints;
  auto  satinBackup   = std::vector<fPOINT> {}; // backup stitches in satin fills
  satinBackup.resize(8U);
  auto       innerDelta  = fPOINT {(insidePoints[finish].x - insidePoints[start].x),
                            (insidePoints[finish].y - insidePoints[start].y)};
  auto const innerLength = hypot(innerDelta.x, innerDelta.y);
  auto       outerDelta  = fPOINT {(outsidePoints[finish].x - outsidePoints[start].x),
                            (outsidePoints[finish].y - outsidePoints[start].y)};
  auto const outerLength = hypot(outerDelta.x, outerDelta.y);
  auto       innerPoint  = fPOINT {insidePoints[start].x, insidePoints[start].y};
  auto       outerPoint  = fPOINT {outsidePoints[start].x, outsidePoints[start].y};

  auto innerFlag = false;
  auto outerFlag = false;
  auto count     = 0U;

  if (!StateMap.testAndSet(StateFlag::SAT1)) {
	stitchPoint = insidePoints[start];
  }
  for (auto& sb : satinBackup) {
	sb.x = 1e12F;
	sb.y = 1e12F;
  }
  if (outerLength > innerLength) {
	count     = wrap::round<uint32_t>(outerLength / LineSpacing);
	innerFlag = true;

	fPOINT intersection = {};
	if (form::linx(insidePoints, start, finish, intersection)) {
	  innerDelta.x = 0.0F;
	  innerDelta.y = 0.0F;
	  innerPoint   = intersection;
	}
  }
  else {
	count     = wrap::round<uint32_t>(innerLength / LineSpacing);
	outerFlag = true;

	fPOINT intersection = {};
	if (form::linx(insidePoints, start, finish, intersection)) {
	  outerDelta.x = 0.0F;
	  outerDelta.y = 0.0F;
	  outerPoint   = intersection;
	}
  }
  if (count == 0U) {
	count = 1;
  }
  if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
	return;
  }
  auto const innerStep        = fPOINT {innerDelta.x / count, innerDelta.y / count};
  auto const outerStep        = fPOINT {outerDelta.x / count, outerDelta.y / count};
  auto       satinBackupIndex = 0U;
  for (auto iStep = 0U; iStep < count; iStep++) {
	innerPoint.x += innerStep.x;
	innerPoint.y += innerStep.y;
	outerPoint.x += outerStep.x;
	outerPoint.y += outerStep.y;
	if (StateMap.testAndFlip(StateFlag::FILDIR)) {
	  if (innerFlag) {
		auto const offsetDelta =
		    fPOINT {innerPoint.x - stitchPoint.x, innerPoint.y - stitchPoint.y};
		auto const offsetLength = hypot(offsetDelta.x, offsetDelta.y);
		auto       offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
		auto const offsetStep   = fPOINT {offsetDelta.x / offsetCount, offsetDelta.y / offsetCount};
		auto       offset       = innerPoint;
		while (si::chkbak(satinBackup, offset)) {
		  offset.x -= offsetStep.x;
		  offset.y -= offsetStep.y;
		}
		si::filinsbw(satinBackup, offset, satinBackupIndex, stitchPoint);
	  }
	  else {
		form::filinsb(innerPoint, stitchPoint);
	  }
	}
	else {
	  if (outerFlag) {
		auto const offsetDelta =
		    fPOINT {outerPoint.x - stitchPoint.x, outerPoint.y - stitchPoint.y};
		auto const offsetLength = hypot(offsetDelta.x, offsetDelta.y);
		auto       offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
		auto const offsetStep   = fPOINT {offsetDelta.x / offsetCount, offsetDelta.y / offsetCount};
		auto       offset       = outerPoint;
		while (si::chkbak(satinBackup, offset)) {
		  offset.x -= offsetStep.x;
		  offset.y -= offsetStep.y;
		}
		si::filinsbw(satinBackup, offset, satinBackupIndex, stitchPoint);
	  }
	  else {
		form::filinsb(outerPoint, stitchPoint);
	  }
	}
  }
}

void satin::internal::sfn(FRMHED const& form, uint32_t startVertex, fPOINT& stitchPoint) {
  for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	auto const nextVertex = form::nxt(form, startVertex);
	si::sbfn(*InsidePoints, startVertex, nextVertex, stitchPoint);
	startVertex = nextVertex;
  }
  OSequence->front() = OSequence->back();
}

void satin::satzum() {
  StateMap.reset(StateFlag::SHOSAT);
  thred::duzrat();
  auto const vertexCount = wrap::toUnsigned(TempPolygon->size());
  form::frmlin(*TempPolygon);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  SelectObject(StitchWindowMemDC, FormPen);
  wrap::Polyline(StitchWindowMemDC, FormLines->data(), vertexCount);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  satin::drwsat();
}

void satin::satout(FRMHED const& form, float satinWidth) {
  if (form.vertexCount != 0U) {
	form::duangs(form);
	OutsidePointList->resize(form.vertexCount);
	InsidePointList->resize(form.vertexCount);
	OutsidePoints = OutsidePointList;
	InsidePoints  = InsidePointList;
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; iVertex++) {
	  si::outfn(form, iVertex, iVertex + 1, 0.1F);
	}
	auto count = 0U;
	for (auto iVertex = 0U; iVertex < form.vertexCount; iVertex++) {
	  if (form::cisin(form, InsidePoints->operator[](iVertex).x, InsidePoints->operator[](iVertex).y)) {
		count++;
	  }
	}
	satinWidth /= 2.0F;
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; iVertex++) {
	  si::outfn(form, iVertex, iVertex + 1, satinWidth);
	}
	si::outfn(form, form.vertexCount - 1U, 0, satinWidth);
	StateMap.reset(StateFlag::INDIR);
	if (count < (form.vertexCount / 2U)) {
	  StateMap.set(StateFlag::INDIR);
	  OutsidePoints = InsidePointList;
	  InsidePoints  = OutsidePointList;
	}
  }
}

void satin::sbrd(FRMHED const& form) {
  auto const savedSpacing = LineSpacing;
  auto const start        = form::getlast(form);
  auto stitchPoint = fPOINT {};
  StateMap.reset(StateFlag::SAT1);
  StateMap.reset(StateFlag::FILDIR);
  OSequence->clear();
  OSequence->push_back(fPOINT {0.0, 0.0});
  if ((form.edgeType & EGUND) != 0U) {
	LineSpacing = USPAC;
	satin::satout(form, form.borderSize * URAT);
	si::sfn(form, start, stitchPoint);
	StateMap.set(StateFlag::FILDIR);
	si::sfn(form, start, stitchPoint);
  }
  satin::satout(form, form.borderSize);
  LineSpacing = form.edgeSpacing;
  si::sfn(form, start, stitchPoint);
  LineSpacing = savedSpacing;
}

void satin::internal::outfn(FRMHED const& form, uint32_t start, uint32_t finish, float satinWidth) {
  auto xOffset = 0.0F;
  auto yOffset = 0.0F;

  if (fabs(FormAngles->operator[](start)) < TINY && fabs(FormAngles->operator[](finish)) < TINY) {
	xOffset = 0.0F;
	yOffset = satinWidth;
  }
  else {
	constexpr auto SATHRESH = 10.0;

	auto angle  = (FormAngles->operator[](finish) - FormAngles->operator[](start)) / 2.0F;
	auto length = satinWidth / cos(angle);
	if (length < -satinWidth * SATHRESH) {
	  length = -satinWidth * SATHRESH;
	}
	if (length > satinWidth * SATHRESH) {
	  length = satinWidth * SATHRESH;
	}
	angle += FormAngles->operator[](start) + PI_F / 2.0F;
	xOffset = length * cos(angle);
	yOffset = length * sin(angle);
  }
  auto vertexIt = std::next(FormVertices->cbegin(), form.vertexIndex);
  if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
	vertexIt = std::next(AngledFormVertices->cbegin(), form.vertexIndex);
  }
  InsidePoints-> operator[](finish).x = vertexIt[finish].x - xOffset;
  InsidePoints-> operator[](finish).y = vertexIt[finish].y - yOffset;
  OutsidePoints->operator[](finish).x = vertexIt[finish].x + xOffset;
  OutsidePoints->operator[](finish).y = vertexIt[finish].y + yOffset;
}

auto satin::internal::chkbak(std::vector<fPOINT> const& satinBackup, fPOINT const& pnt) noexcept -> bool {
  auto const maxSB = satinBackup.size();
  for (auto iBackup = 0U; iBackup < maxSB; iBackup++) {
	auto const length = hypot(satinBackup[iBackup].x - pnt.x, satinBackup[iBackup].y - pnt.y);
	if (length < LineSpacing) {
	  return true;
	}
  }
  return false;
}

auto satin::adsatk(uint32_t count) -> uint32_t {
  auto const iSatinConnect = wrap::toUnsigned(SatinGuides->size());
  auto const it            = SatinGuides->end();
  auto const val           = SATCON {};
  SatinGuides->insert(it, count, val);

  return iSatinConnect;
}
