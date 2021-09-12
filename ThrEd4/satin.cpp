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

// Local Headers
#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "thred.h"
#include "satin.h"

namespace si = satin::internal;

static auto StartPoint = uint32_t {}; // starting formOrigin for a satin stitch guide-line

void satin::delsac(uint32_t formIndex) {
  auto& formList = *FormList;
  if (!SatinGuides->empty()) {
	if (formList[formIndex].type == SAT && (formList[formIndex].satinGuideCount != 0U)) {
	  auto const startGuide = wrap::next(SatinGuides->cbegin(), formList[formIndex].satinOrAngle.guide);
	  auto const endGuide = wrap::next(startGuide, formList[formIndex].satinGuideCount);
	  SatinGuides->erase(startGuide, endGuide);
	  for (auto iForm = formIndex + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
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
  auto const val     = SATCON {};
  auto const itGuide = wrap::next(SatinGuides->cbegin(), startGuide);
  SatinGuides->insert(itGuide, val);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	if (form.type == SAT) {
	  form.satinOrAngle.guide += guideCount;
	}
  }
}

auto satin::internal::nusac(uint32_t formIndex, uint32_t guideCount) -> uint32_t {
  auto        guideIndex = 0U;
  auto const& formList   = *FormList;
  for (auto iForm = 0U; iForm < formIndex; ++iForm) {
	auto const& form = formList[iForm];
	if (form.type == SAT) {
	  guideIndex += form.satinGuideCount;
	}
  }
  si::sacspac(guideIndex, guideCount);
  return guideIndex;
}

void satin::spltsat(uint32_t guideIndex) {
  {
	auto const& currentForm = FormList->operator[](ClosestFormToCursor);
	FormList->insert(wrap::next(FormList->cbegin(), ClosestFormToCursor), currentForm);
  }
  auto& form = FormList->operator[](ClosestFormToCursor); // insert may have invalidated reference
  // We are adding two additional vertices when splitting the form
  auto vertexBuffer = std::vector<fPOINT> {};
  vertexBuffer.resize(wrap::toSize(form.vertexCount) + 2U);
  // clang-format off
  auto const maxForm  = FormList->size();
  auto const lastFormGuide = wrap::next(FormVertices->cbegin(), form.vertexIndex + form.vertexCount);
  // clang-format on
  FormVertices->insert(lastFormGuide, 2, fPOINT {});
  for (auto iForm = ClosestFormToCursor + 2U; iForm < maxForm; ++iForm) {
	FormList->operator[](iForm).vertexIndex += 2;
  }
  auto        iOldVertex    = 0U;
  auto        itGuide       = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
  auto const& currentGuide  = *(wrap::next(itGuide, guideIndex));
  auto const  oldLastVertex = currentGuide.start + (form.vertexCount - currentGuide.finish) + 1U;
  auto        iNewVertex    = oldLastVertex + 1U;
  auto const  itFirstGuide  = wrap::next(FormVertices->begin(), form.vertexIndex);
  auto        itVertex      = itFirstGuide;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	if (iVertex == currentGuide.start || iVertex == currentGuide.finish) {
	  vertexBuffer[iOldVertex++] = *itVertex;
	  if (iVertex == currentGuide.start) {
		vertexBuffer[iNewVertex++] = *itVertex;
	  }
	  else {
		vertexBuffer[oldLastVertex] = *itVertex;
	  }
	}
	else {
	  if (iVertex < currentGuide.start) {
		vertexBuffer[iOldVertex++] = *itVertex;
	  }
	  else {
		if (iVertex < currentGuide.finish) {
		  vertexBuffer[iNewVertex++] = *itVertex;
		}
		else {
		  vertexBuffer[iOldVertex++] = *itVertex;
		}
	  }
	}
	++itVertex;
  }
  itVertex = itFirstGuide;
  for (auto iVertex = 0U; iVertex < iNewVertex; ++iVertex) {
	*(itVertex++) = vertexBuffer[iVertex];
  }
  form.vertexCount     = iOldVertex;
  auto& nextForm       = FormList->operator[](wrap::toSize(ClosestFormToCursor) + 1U);
  nextForm.vertexCount = iNewVertex - iOldVertex;
  nextForm.vertexIndex = form.vertexIndex + iOldVertex;
  form::frmout(ClosestFormToCursor);
  form::frmout(ClosestFormToCursor + 1U);
  auto const iNewGuide = 1U + currentGuide.start - currentGuide.finish;
  for (auto iGuide = 0U; iGuide < guideIndex; ++iGuide) {
	(itGuide++)->finish += iNewGuide;
  }
  if (form.wordParam != 0U) {
	form.wordParam = currentGuide.start;
  }
  for (auto iGuide = guideIndex; iGuide < form.satinGuideCount; ++iGuide) {
	itGuide->start -= (currentGuide.start - 1);
	itGuide->finish -= (currentGuide.start - 1);
	++itGuide;
  }
  if (nextForm.wordParam != 0U) {
	nextForm.wordParam -= (currentGuide.start - 1);
  }
  auto const offset      = form.satinOrAngle.guide + guideIndex;
  auto const itThisGuide = wrap::next(SatinGuides->cbegin(), offset);
  SatinGuides->erase(itThisGuide);
  nextForm.satinOrAngle.guide = form.satinOrAngle.guide + guideIndex;
  nextForm.satinGuideCount    = form.satinGuideCount - guideIndex - 1U;
  form.satinGuideCount        = guideIndex;
  for (auto iForm = ClosestFormToCursor + 2U; iForm < maxForm; ++iForm) {
	auto& formIter = FormList->operator[](iForm);
	if ((formIter.type == SAT) && (formIter.satinGuideCount != 0U) && formIter.satinOrAngle.guide != 0U) {
	  --(formIter.satinOrAngle.guide);
	}
  }
  if (clip::iseclp(form)) {
	form::clpspac(form.borderClipData, form.clipEntries);
	for (auto iForm = ClosestFormToCursor + 1U; iForm < maxForm; ++iForm) {
	  auto& formIter = FormList->operator[](iForm);
	  formIter.borderClipData += form.clipEntries;
	}
  }
  form::stchadj();
}

void satin::internal::satclos() {
  // clang-format off
  auto&      form              = FormList->operator[](ClosestFormToCursor);
  auto const initialGuideCount = form.satinGuideCount;
  auto       minimumLength     = BIGFLOAT;
  // clang-format on
  form::uninsf();
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  {
	auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  auto const deltaX = stitchPoint.x - itVertex->x;
	  auto const deltaY = stitchPoint.y - itVertex->y;
	  auto const length = hypot(deltaX, deltaY);
	  if (length < minimumLength) {
		minimumLength         = length;
		ClosestVertexToCursor = iVertex;
	  }
	  ++itVertex;
	}
  }
  StateMap->reset(StateFlag::SATCNKT);
  if (form.type == FRMLINE) {
	form.fillType      = CONTF;
	auto closestVertex = ClosestVertexToCursor;
	if (StartPoint > closestVertex) {
	  std::swap(closestVertex, StartPoint);
	}
	if (StartPoint == 0U) {
	  ++StartPoint;
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
	  if (closestVertex > form.vertexCount - 2) {
		closestVertex = (form.vertexCount - 2U);
		StartPoint    = closestVertex - 2U;
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
		auto const itGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide + form.satinGuideCount);
		itGuide->start  = closestVertex;
		itGuide->finish = ClosestVertexToCursor;
		++form.satinGuideCount;
		satin::satadj(form);
	  }
	  else {
		form.satinOrAngle.guide = si::nusac(ClosestFormToCursor, 1);
		auto const itGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide + initialGuideCount);
		itGuide->start       = closestVertex;
		itGuide->finish      = ClosestVertexToCursor;
		form.satinGuideCount = 1;
	  }
	}
  }
}

void satin::satknkt() {
  si::satclos();
  form::refil();
  StateMap->set(StateFlag::RESTCH);
}

auto satin::internal::satselfn() -> bool {
  auto       minimumLength = BIGFLOAT;
  auto const stitchPoint   = thred::pxCor2stch(Msg.pt);
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);

	auto const layerCode =
	    gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(form.attribute & FRMLMSK) >> 1U);
	if ((ActiveLayer == 0U) || (layerCode == 0U) || layerCode == ActiveLayer) {
	  auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		auto const deltaX = stitchPoint.x - itVertex->x;
		auto const deltaY = stitchPoint.y - itVertex->y;
		++itVertex;
		auto const length = hypot(deltaX, deltaY);
		if (length < minimumLength) {
		  minimumLength         = length;
		  ClosestFormToCursor   = iForm;
		  ClosestVertexToCursor = iVertex;
		}
	  }
	}
  }
  return minimumLength < FCLOSNUF;
}

void satin::satsel() {
  if (si::satselfn()) {
	auto& form = FormList->operator[](ClosestFormToCursor);
	thred::duzrat();
	StartPoint          = ClosestVertexToCursor;
	auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + ClosestVertexToCursor);
	FormLines->front() = form::sfCor2px(*itVertex);
	StateMap->reset(StateFlag::SHOCON);
	StateMap->set(StateFlag::SATCNKT);
	if (form.type == FRMFPOLY) {
	  form.type = SAT;
	}
  }
}

void satin::internal::satcpy(FRMHED const& form, std::vector<SATCON> const& source, uint32_t size) {
  auto const& currentFormGuides = form.satinOrAngle.guide;
  auto const  startGuide        = wrap::next(SatinGuides->cbegin(), currentFormGuides);
  auto const  endGuide          = wrap::next(startGuide, (form.satinGuideCount - size));
  SatinGuides->erase(startGuide, endGuide);
  auto const itGuide = wrap::next(SatinGuides->begin(), currentFormGuides);
  std::copy(source.cbegin(), source.cend(), itGuide);
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
  auto itFirstGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
  {
	auto itGuide = itFirstGuide;
	for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
	  auto const endCount = (form.vertexCount - 1);
	  if (itGuide->finish > endCount) {
		itGuide->finish = endCount;
	  }
	  if (itGuide->start > endCount) {
		itGuide->start = endCount;
	  }
	  ++itGuide;
	}
  }
  // remove any guides of 0 length
  {
	auto itGuide = itFirstGuide;
	for (auto iSource = 0U; iSource < currentGuidesCount; ++iSource) {
	  if (itGuide->start != itGuide->finish) {
		interiorGuides.push_back(*itGuide);
		++itGuide;
	  }
	}
  }
  auto iDestination = wrap::toUnsigned(interiorGuides.size());
  if (currentGuidesCount > iDestination) {
	// NOLINTNEXTLINE
	outDebugString(L"Removed {} zero distance guides\n", (currentGuidesCount - iDestination));
	si::satcpy(form, interiorGuides, iDestination);
	itFirstGuide       = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide); // iterator may be invalidated by erase
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
	{
	  auto itGuide = itFirstGuide;
	  for (auto iSource = 0U; iSource < currentGuidesCount; ++iSource) {
		if (!satinMap.test(itGuide->start) && !satinMap.test(itGuide->finish)) {
		  interiorGuides.push_back(*itGuide);
		}
		++itGuide;
	  }
	}
	iDestination = wrap::toUnsigned(interiorGuides.size());
	if (currentGuidesCount > iDestination) {
	  outDebugString(L"Removed {} end guides\n", (currentGuidesCount - iDestination));
	  si::satcpy(form, interiorGuides, iDestination);
	  itFirstGuide       = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide); // iterator may be invalidated by erase
	  currentGuidesCount = iDestination;
	}
	// remove any guides that start after the end guide
	if (endGuide != 0U) {
	  interiorGuides.clear();
	  auto itGuide = itFirstGuide;
	  for (auto iSource = 0U; iSource < currentGuidesCount; ++iSource) {
		if (itGuide->start < endGuide) {
		  interiorGuides.push_back(*itGuide);
		}
		++itGuide;
	  }
	  iDestination = wrap::toUnsigned(interiorGuides.size());
	  if (currentGuidesCount > iDestination) {
		outDebugString(L"Removed {} reversed guides\n", (currentGuidesCount - iDestination));
		si::satcpy(form, interiorGuides, iDestination);
		itFirstGuide         = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide); // iterator may be invalidated by erase
		form.satinGuideCount = iDestination;
		currentGuidesCount   = iDestination;
	  }
	}
  }

  if (currentGuidesCount != 0U && endGuide != 0) {
	satinMap.reset();
	{
	  auto itGuide = itFirstGuide;
	  for (auto iGuide = 0U; iGuide < currentGuidesCount; ++iGuide) {
		auto iForward = itGuide->start;
		if (iForward > endGuide - 1) {
		  iForward = endGuide - 1U;
		}
		if (satinMap.test_set(iForward)) {
		  auto iReverse = iForward;
		  if (iReverse != 0U) {
			--iReverse;
		  }
		  while (satinMap.test(iForward) && (iForward < ((endGuide)-1))) {
			++iForward;
		  }
		  while ((iReverse != 0U) && (satinMap.test(iReverse))) {
			--iReverse;
		  }
		  if (satinMap.test(iForward) && satinMap.test(iReverse)) {
			break;
		  }
		  if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
			if (iForward - itGuide->start > itGuide->start - iReverse) {
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
		++itGuide;
	  }
	}
	{
	  auto iVertex = 0U;
	  auto itGuide = itFirstGuide;
	  do {
		iVertex = wrap::toUnsigned(satinMap.getFirst());
		if (iVertex < form.vertexCount) {
		  (itGuide++)->start = iVertex;
		}
	  } while (iVertex < form.vertexCount);
	  currentGuidesCount = wrap::distance<uint32_t>(itFirstGuide, itGuide);
	}
	satinMap.reset();
	auto itGuide = itFirstGuide;
	for (auto iGuide = 0U; iGuide < currentGuidesCount; ++iGuide) {
	  auto iForward = itGuide->finish;
	  auto iReverse = iForward;
	  if (iForward > form.vertexCount - 1U) {
		iForward = form.vertexCount - 1U;
	  }
	  if (satinMap.test_set(iForward)) {
		if (iForward < form.vertexCount - 1U) {
		  ++iForward;
		}
		if (iReverse > (endGuide) + 1U) {
		  --iReverse;
		}
		while (satinMap.test(iForward) && iForward < form.vertexCount - 1U) {
		  ++iForward;
		}
		while (iReverse > (endGuide)-1 && (satinMap.test(iReverse))) {
		  --iReverse;
		}
		if (satinMap.test(iForward) && satinMap.test(iReverse)) {
		  break;
		}
		if (!satinMap.test(iForward) && !satinMap.test(iReverse)) {
		  if (iForward - itGuide->finish > itGuide->finish - iReverse) {
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
	  ++itGuide;
	}
	auto iReverse = 0U;
	itGuide       = itFirstGuide;
	do {
	  iReverse = wrap::toUnsigned(satinMap.getLast());
	  if (iReverse < form.vertexCount) {
		(itGuide++)->finish = iReverse;
	  }
	} while (iReverse < form.vertexCount);
	auto iGuide = wrap::distance<uint32_t>(itFirstGuide, itGuide);
	if (iGuide < currentGuidesCount) {
	  iGuide = currentGuidesCount;
	}
	currentGuidesCount = iGuide;
	auto const end0    = (form.vertexCount - endGuide - 2U);
	auto const end1    = (endGuide - 2U);
	if (currentGuidesCount > end0) {
	  currentGuidesCount = end0;
	}
	if (currentGuidesCount > end1) {
	  currentGuidesCount = end1;
	}
  }
  else {
	// sort the guides
	auto itStartGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
	auto itEndGuide   = wrap::next(itStartGuide, form.satinGuideCount);
	std::sort(itStartGuide, itEndGuide, satin::scomp);
  }
  if (form.satinGuideCount < savedGuideCount) {
	auto const iGuide = savedGuideCount - currentGuidesCount;
	outDebugString(L"Guides adjusted by {}, so updating forms\n", iGuide);
	for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  auto& afterForm = FormList->operator[](iForm);
	  if ((afterForm.type == SAT) && (afterForm.satinOrAngle.guide >= iGuide)) {
		afterForm.satinOrAngle.guide -= iGuide;
	  }
	}
  }
}

void satin::delcon(FRMHED& form, uint32_t GuideIndex) {
  auto const offset  = form.satinOrAngle.guide + GuideIndex;
  auto const itGuide = wrap::next(SatinGuides->cbegin(), offset);
  SatinGuides->erase(itGuide);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& afterForm = FormList->operator[](iForm);
	if ((afterForm.type == SAT) && (afterForm.satinGuideCount != 0U) && (afterForm.satinOrAngle.guide != 0U)) {
	  --(afterForm.satinOrAngle.guide);
	}
  }
  --(form.satinGuideCount);
  if (form.fillType == SATF) {
	form::refil();
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void satin::delspnt() {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (form::chkdel(currentForm)) {
	StateMap->set(StateFlag::DELTO);
	thred::frmdel();
	StateMap->reset(StateFlag::FRMPSEL);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (currentForm.type == SAT) {
	if (ClosestVertexToCursor < currentForm.wordParam) {
	  --(currentForm.wordParam);
	}
	if (currentForm.satinGuideCount != 0U) {
	  auto const itStartGuide = wrap::next(SatinGuides->begin(), currentForm.satinOrAngle.guide);
	  auto       itGuide      = itStartGuide;
	  auto       iGuide       = 0U;
	  while (iGuide < currentForm.satinGuideCount) {
		if (itGuide->start != ClosestVertexToCursor && itGuide->finish != ClosestVertexToCursor) {
		  ++itGuide;
		  ++iGuide;
		}
		else {
		  break;
		}
	  }
	  if (iGuide < currentForm.satinGuideCount &&
	      (itGuide->start == ClosestVertexToCursor || itGuide->finish == ClosestVertexToCursor)) {
		while (iGuide < currentForm.satinGuideCount) {
		  *itGuide = *(std::next(itGuide));
		  ++itGuide;
		  ++iGuide;
		}
		--(currentForm.satinGuideCount);
		for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		  auto& form = FormList->operator[](iForm);
		  if (form.type == SAT && (form.satinGuideCount != 0U)) {
			++(form.satinOrAngle.guide);
		  }
		}
	  }
	  itGuide = itStartGuide;
	  for (iGuide = 0; iGuide < currentForm.satinGuideCount; ++iGuide) {
		if (itGuide->start > ClosestVertexToCursor) {
		  --(itGuide->start);
		}
		if (itGuide->finish > ClosestVertexToCursor) {
		  --(itGuide->finish);
		}
		++itGuide;
	  }
	}
  }
  auto const closestVertexIt =
      wrap::next(FormVertices->cbegin(), currentForm.vertexIndex + ClosestVertexToCursor);
  FormVertices->erase(closestVertexIt);
  --(currentForm.vertexCount);
  if (ClosestVertexToCursor > (currentForm.vertexCount) - 1) {
	ClosestVertexToCursor = currentForm.vertexCount - 1U;
  }
  StateMap->set(StateFlag::FRMPSEL);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	--(form.vertexIndex);
  }
  auto const itVertex = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex + ClosestVertexToCursor);
  thred::ritfcor(*itVertex);
  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
  form::frmout(ClosestFormToCursor);
  if (itVertex->x < ZoomRect.left || itVertex->x > ZoomRect.right ||
      itVertex->y < ZoomRect.bottom || itVertex->y > ZoomRect.top) {
	thred::shft(*itVertex);
  }
  form::refil();
}

void satin::internal::satsbrd(uint32_t formIndex) {
  auto& currentForm = FormList->operator[](formIndex);
  clip::deleclp(ClosestFormToCursor);
  currentForm.edgeType = EDGEANGSAT;
  if (UserFlagMap->test(UserFlag::DUND)) {
	currentForm.edgeType |= EGUND;
  }
  form::bsizpar(currentForm);
  currentForm.borderSize  = BorderWidth;
  currentForm.edgeSpacing = LineSpacing / 2;
  wrap::narrow(currentForm.borderColor, ActiveColor);
  form::refilfn();
}

void satin::satbrd() {
  if (displayText::filmsgs(FML_ANGS)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		form.attribute |= gsl::narrow_cast<decltype(form.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		form.attribute &= NOBLNT;
	  }
	  si::satsbrd(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		form.attribute |= gsl::narrow<decltype(form.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		form.attribute &= NOBLNT;
	  }
	  si::satsbrd(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void satin::internal::satends(FRMHED const& form, uint32_t isBlunt, float width) {
  auto const& vertexIndex = form.vertexIndex;
  auto        itVertex    = wrap::next(FormVertices->cbegin(), vertexIndex);
  if ((isBlunt & SBLNT) != 0U) {
	auto step = fPOINT {sin(FormAngles->front()) * width / 2.0F, cos(FormAngles->front()) * width / 2.0F};
	if (StateMap->test(StateFlag::INDIR)) {
	  step.x = -step.x;
	  step.y = -step.y;
	}
	InsidePoints->front()  = fPOINT {itVertex->x + step.x, itVertex->y - step.y};
	OutsidePoints->front() = fPOINT {itVertex->x - step.x, itVertex->y + step.y};
  }
  else {
	InsidePoints->front() = OutsidePoints->front() = *itVertex;
  }
  auto const& vertexCount = form.vertexCount;
  itVertex                = wrap::next(itVertex, vertexCount - 1U);
  if ((isBlunt & FBLNT) != 0U) {
	auto step = fPOINT {sin(FormAngles->operator[](vertexCount - 2U)) * width / 2.0F,
	                    cos(FormAngles->operator[](vertexCount - 2U)) * width / 2.0F};
	if (StateMap->test(StateFlag::INDIR)) {
	  step.x = -step.x;
	  step.y = -step.y;
	}
	InsidePoints->back()  = fPOINT {itVertex->x + step.x, itVertex->y - step.y};
	OutsidePoints->back() = fPOINT {itVertex->x - step.x, itVertex->y + step.y};
  }
  else {
	InsidePoints->back() = OutsidePoints->back() = *itVertex;
  }
}

void satin::ribon() {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	if (FormList->operator[](ClosestFormToCursor).vertexCount > 2) {
	  thred::savdo();
	  auto const savedFormIndex = ClosestFormToCursor;
	  satin::satout(FormList->operator[](ClosestFormToCursor), BorderWidth);
	  if (!FormList->empty()) {
		auto       newForm            = FRMHED {};
		auto const currentType        = FormList->operator[](ClosestFormToCursor).type;
		auto const currentVertexCount = FormList->operator[](ClosestFormToCursor).vertexCount;
		newForm.maxFillStitchLen = MAXSIZ * PFGRAN;
		newForm.minFillStitchLen = MinStitchLength;
		MaxStitchLen             = MAXSIZ * PFGRAN;
		if (currentType == FRMLINE) {
		  // Set blunt flags
		  auto isBlunt = 0U;
		  if (UserFlagMap->test(UserFlag::BLUNT)) {
			isBlunt = SBLNT | FBLNT;
		  }
		  si::satends(FormList->operator[](ClosestFormToCursor), isBlunt, BorderWidth);
		}
		newForm.vertexIndex = (currentType == FRMLINE)
		                          ? thred::adflt(currentVertexCount * 2U)
		                          : thred::adflt((currentVertexCount * 2U) + 2U);
		auto const startVertex = wrap::next(FormVertices->begin(), newForm.vertexIndex);
		auto       itVertex    = startVertex;
		*(itVertex++)          = OutsidePoints->front();
		if (currentType == FRMLINE) {
		  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
			*(itVertex++) = InsidePoints->operator[](iVertex);
		  }
		  for (auto iVertex = currentVertexCount - 1U; iVertex != 0; --iVertex) {
			*(itVertex++) = OutsidePoints->operator[](iVertex);
		  }
		}
		else {
		  newForm.underlayIndent = IniFile.underlayIndent;
		  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
			*(itVertex++) = InsidePoints->operator[](iVertex);
		  }
		  *(itVertex++) = InsidePoints->front();
		  *(itVertex++) = OutsidePoints->front();
		  for (auto iVertex = currentVertexCount - 1U; iVertex != 0; --iVertex) {
			*(itVertex++) = OutsidePoints->operator[](iVertex);
		  }
		}
		auto const iNewVertex = wrap::distance<uint32_t>(startVertex, itVertex);
		newForm.type          = SAT;
		wrap::narrow(newForm.fillColor, ActiveColor);
		newForm.fillSpacing                = LineSpacing;
		newForm.lengthOrCount.stitchLength = IniFile.maxStitchLength;
		newForm.vertexCount                = iNewVertex;
		newForm.attribute                  = FormList->operator[](ClosestFormToCursor).attribute;
		newForm.attribute &= FRMLMSK;
		newForm.attribute |= FRMEND;
		newForm.wordParam          = iNewVertex / 2;
		newForm.satinGuideCount    = newForm.wordParam - 2U;
		newForm.satinOrAngle.guide = adsatk(newForm.satinGuideCount);
		if (StateMap->test(StateFlag::CNV2FTH)) {
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
		auto itGuide = wrap::next(SatinGuides->begin(), newForm.satinOrAngle.guide);
		for (auto iGuide = 0U; iGuide < newForm.satinGuideCount; ++iGuide) {
		  itGuide->start  = iGuide + 2U;
		  itGuide->finish = newForm.vertexCount - iGuide - 1U;
		  ++itGuide;
		}
		FormList->push_back(newForm);
		ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
		form::frmout(ClosestFormToCursor);
		form::refilfn();
		ClosestFormToCursor = savedFormIndex;
		StateMap->set(StateFlag::DELTO);
		thred::frmdel();
		ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
		thred::coltab();
		StateMap->set(StateFlag::FORMSEL);
		StateMap->set(StateFlag::INIT);
		StateMap->set(StateFlag::RESTCH);
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
  auto       stitchPoint  = fPOINT {};
  OSequence->clear();
  if ((form.edgeType & EGUND) != 0U) {
	auto const width = form.borderSize * URAT;
	satin::satout(form, width);
	si::satends(form, form.attribute, width);
	StateMap->reset(StateFlag::SAT1);
	StateMap->reset(StateFlag::FILDIR);
	LineSpacing = USPAC;
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	  si::sbfn(*InsidePoints, iVertex, iVertex + 1U, stitchPoint);
	}
	StateMap->flip(StateFlag::FILDIR);
	for (auto iVertex = form.vertexCount - 1U; iVertex != 0; --iVertex) {
	  si::sbfn(*InsidePoints, iVertex, iVertex - 1, stitchPoint);
	}
  }
  satin::satout(form, form.borderSize);
  si::satends(form, form.attribute, form.borderSize);
  LineSpacing = form.edgeSpacing;
  StateMap->reset(StateFlag::SAT1);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
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
	auto itFirstVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	if (!StateMap->testAndSet(StateFlag::SAT1)) {
	  if (StateMap->test(StateFlag::FTHR)) {
		if (form.vertexCount != 0U) {
		  auto itVertex = wrap::next(itFirstVertex, line1Start % form.vertexCount);
		  BSequence->emplace_back(BSEQPNT {itVertex->x, itVertex->y, 0});
		}
	  }
	  else {
		if (StateMap->test(StateFlag::BARSAT)) {
		  if (form.vertexCount != 0U) {
			auto itVertex = wrap::next(itFirstVertex, line1Start % form.vertexCount);
			BSequence->emplace_back(BSEQPNT {itVertex->x, itVertex->y, 0});
			itVertex = wrap::next(itFirstVertex, line2Start % form.vertexCount);
			BSequence->emplace_back(BSEQPNT {itVertex->x, itVertex->y, 0});
		  }
		}
		else {
		  auto itVertex = wrap::next(itFirstVertex, line1Start);
		  OSequence->push_back(*itVertex);
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
	for (auto iSegment = 0U; iSegment < line1Segments - 1U; ++iSegment) {
	  auto const nextVertex = form::nxt(form, iVertex);
	  auto const val = wrap::ceil<uint32_t>(((lengths[nextVertex] - lengths[iVertex]) / line1Length) *
	                                        wrap::toFloat(stitchCount));
	  line1StitchCounts.push_back(val);
	  segmentStitchCount += val;
	  iVertex = form::nxt(form, iVertex);
	}
	line1StitchCounts.push_back(stitchCount - segmentStitchCount);
	auto iNextVertex   = line2Start;
	iVertex            = form::prv(form, iNextVertex);
	segmentStitchCount = 0;
	while (iVertex > line2End) {
	  auto const val = wrap::ceil<uint32_t>(((lengths[iNextVertex] - lengths[iVertex]) / line2Length) *
	                                        wrap::toFloat(stitchCount));
	  line2StitchCounts.push_back(val);
	  segmentStitchCount += val;
	  iNextVertex = form::prv(form, iNextVertex);
	  iVertex     = form::prv(form, iNextVertex);
	}
	line2StitchCounts.push_back(stitchCount - segmentStitchCount);
	auto line1Point    = *(wrap::next(itFirstVertex, line1Start));
	auto line1Next     = wrap::next(itFirstVertex, form::nxt(form, line1Start));
	auto line2Previous = wrap::next(itFirstVertex, form::prv(form, line2Start));
	auto line1Count    = line1StitchCounts[0];
	auto line2Count    = line2StitchCounts[0];
	auto iLine1Vertex  = line1Start;
	auto iLine2Vertex  = line2Start;

	auto itLine1Vertex = wrap::next(itFirstVertex, iLine1Vertex);
	auto line1Delta    = fPOINT {line1Next->x - itLine1Vertex->x, line1Next->y - itLine1Vertex->y};
	auto line2Point    = (iLine2Vertex == form.vertexCount) ? *itFirstVertex
	                                                     : *(wrap::next(itFirstVertex, iLine2Vertex));
	auto line2Delta = fPOINT {line2Previous->x - line2Point.x, line2Previous->y - line2Point.y};
	iLine1Vertex    = form::nxt(form, iLine1Vertex);
	iLine2Vertex    = form::prv(form, iLine2Vertex);
	auto line1Step =
	    fPOINT {line1Delta.x / wrap::toFloat(line1Count), line1Delta.y / wrap::toFloat(line1Count)};
	auto line2Step =
	    fPOINT {line2Delta.x / wrap::toFloat(line2Count), line2Delta.y / wrap::toFloat(line2Count)};
	bool flag        = false;
	auto iLine1Count = 1U;
	auto iLine2Count = 1U;
	auto stitchPoint = *(wrap::next(itFirstVertex, line1Start));
	auto loop        = 0U;

	constexpr auto LOOPLIM = 20000U; // limit the iterations

	do {
	  flag = false;
	  ++loop;
	  if (StateMap->test(StateFlag::FTHR)) {
		while ((line1Count != 0U) && (line2Count != 0U)) {
		  line1Point.x += line1Step.x;
		  line1Point.y += line1Step.y;
		  line2Point.x += line2Step.x;
		  line2Point.y += line2Step.y;
		  if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			BSequence->emplace_back(BSEQPNT {line1Point.x, line1Point.y, 0});
		  }
		  else {
			BSequence->emplace_back(BSEQPNT {line2Point.x, line2Point.y, 1});
		  }
		  --line1Count;
		  --line2Count;
		}
	  }
	  else {
		if (StateMap->test(StateFlag::BARSAT)) {
		  while ((line1Count != 0U) && (line2Count != 0U)) {
			line1Point.x += line1Step.x;
			line1Point.y += line1Step.y;
			line2Point.x += line2Step.x;
			line2Point.y += line2Step.y;
			if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			  BSequence->emplace_back(BSEQPNT {line1Point.x, line1Point.y, 0});
			  BSequence->emplace_back(BSEQPNT {line2Point.x, line2Point.y, 1});
			}
			else {
			  BSequence->emplace_back(BSEQPNT {line2Point.x, line2Point.y, 2});
			  BSequence->emplace_back(BSEQPNT {line1Point.x, line1Point.y, 3});
			}
			--line1Count;
			--line2Count;
		  }
		}
		else {
		  while ((line1Count != 0U) && (line2Count != 0U)) {
			line1Point.x += line1Step.x;
			line1Point.y += line1Step.y;
			line2Point.x += line2Step.x;
			line2Point.y += line2Step.y;
			if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			  if (UserFlagMap->test(UserFlag::SQRFIL)) {
				stitchPoint = form::filinu(line2Point, stitchPoint);
			  }
			  stitchPoint = form::filin(line1Point, stitchPoint);
			}
			else {
			  if (UserFlagMap->test(UserFlag::SQRFIL)) {
				stitchPoint = form::filinu(line1Point, stitchPoint);
			  }
			  stitchPoint = form::filin(line2Point, stitchPoint);
			}
			--line1Count;
			--line2Count;
		  }
		}
	  }
	  if ((iLine1Count < line1Segments || iLine2Count < line2Segments)) {
		if ((line1Count == 0U) && iLine1Count < line1StitchCounts.size()) {
		  line1Count           = line1StitchCounts[iLine1Count++];
		  line1Next            = wrap::next(itFirstVertex, form::nxt(form, iLine1Vertex));
		  auto itCurrentVertex = wrap::next(itFirstVertex, iLine1Vertex);
		  line1Delta.x         = line1Next->x - itCurrentVertex->x;
		  line1Delta.y         = line1Next->y - itCurrentVertex->y;
		  iLine1Vertex         = form::nxt(form, iLine1Vertex);
		  line1Step.x          = line1Delta.x / wrap::toFloat(line1Count);
		  line1Step.y          = line1Delta.y / wrap::toFloat(line1Count);
		}
		if ((line2Count == 0U) && iLine2Count < line2StitchCounts.size()) {
		  line2Count           = line2StitchCounts[iLine2Count++];
		  line2Previous        = wrap::next(itFirstVertex, form::prv(form, iLine2Vertex));
		  auto itCurrentVertex = wrap::next(itFirstVertex, iLine2Vertex);
		  line2Delta.x         = line2Previous->x - itCurrentVertex->x;
		  line2Delta.y         = line2Previous->y - itCurrentVertex->y;
		  iLine2Vertex         = form::prv(form, iLine2Vertex);
		  line2Step.x          = line2Delta.x / wrap::toFloat(line2Count);
		  line2Step.y          = line2Delta.y / wrap::toFloat(line2Count);
		}
		if (((line1Count != 0U) || (line2Count != 0U)) && line1Count < MAXITEMS && line2Count < MAXITEMS) {
		  flag = true;
		}
	  }
	} while (flag && (loop < LOOPLIM));
  }
}

void satin::internal::satmf(FRMHED const& form, std::vector<float> const& lengths) {
  auto start = 0U;
  if ((form.attribute & FRMEND) != 0U) {
	start = 1;
  }
  auto itGuide = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
  si::satfn(form, lengths, start, itGuide->start, form.vertexCount, itGuide->finish);
  auto endGuideIndex = form.satinGuideCount;
  if (endGuideIndex != 0U) {
	--endGuideIndex;
  }
  auto const itEndGuide = wrap::next(itGuide, endGuideIndex);
  for (auto iGuide = 0U; iGuide < endGuideIndex; ++iGuide) {
	auto const& thisGuide = *itGuide;
	++itGuide;
	auto const& nextGuide = *itGuide;
	si::satfn(form, lengths, thisGuide.start, nextGuide.start, thisGuide.finish, nextGuide.finish);
  }
  auto const& endGuide = form.wordParam;
  if (endGuide != 0U) {
	si::satfn(form, lengths, itEndGuide->start, endGuide, itEndGuide->finish, endGuide + 1U);
  }
  else {
	if (itEndGuide->finish - itEndGuide->start > 2) {
	  auto const length =
	      (lengths[itEndGuide->finish] - lengths[itEndGuide->start]) / 2 + lengths[itEndGuide->start];
	  auto iVertex = itEndGuide->start;
	  while (length > lengths[iVertex]) {
		++iVertex;
	  }
	  auto const deltaX = lengths[iVertex] - length;
	  auto const deltaY = length - lengths[iVertex - 1U];
	  if (deltaY > deltaX) {
		--iVertex;
	  }
	  si::satfn(form, lengths, itEndGuide->start, iVertex, itEndGuide->finish, iVertex);
	}
	else {
	  si::satfn(
	      form, lengths, itEndGuide->start, itEndGuide->start + 1, itEndGuide->finish, itEndGuide->start + 1U);
	}
  }
}

void satin::satfil(FRMHED& form) {
  auto const savedSpacing = LineSpacing;
  satin::satadj(form);
  LineSpacing /= 2;
  OSequence->clear();
  BSequence->clear();
  StateMap->reset(StateFlag::SAT1);
  StateMap->reset(StateFlag::FILDIR);
  form.fillType = SATF;
  auto lengths  = std::vector<float> {};
  lengths.reserve(wrap::toSize(form.vertexCount) + 1U);
  auto length = 0.0F;
  lengths.push_back(length);
  auto const itFirstVertex    = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto       itPreviousVertex = itFirstVertex;
  auto       itCurrentVertex  = std::next(itPreviousVertex);
  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	auto const delta =
	    fPOINT {itCurrentVertex->x - itPreviousVertex->x, itCurrentVertex->y - itPreviousVertex->y};
	length += hypot(delta.x, delta.y);
	lengths.push_back(length);
	++itPreviousVertex;
	++itCurrentVertex;
  }
  auto const lastDelta =
      fPOINT {itFirstVertex->x - itPreviousVertex->x, itFirstVertex->y - itPreviousVertex->y};
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
	  length       = (length - lengths[1]) / 2.0F;
	  auto iVertex = 1U;
	  if (!StateMap->test(StateFlag::BARSAT)) {
		auto const vNext = std::next(itFirstVertex);
		OSequence->push_back(*vNext);
	  }
	  while ((iVertex < (form.vertexCount + 1U)) && (length > lengths[iVertex])) {
		++iVertex;
	  }
	  auto const deltaA = lengths[iVertex] - length;
	  auto const deltaB = length - lengths[iVertex - 1U];
	  if (deltaB > deltaA) {
		--iVertex;
	  }
	  si::satfn(form, lengths, 1, iVertex, form.vertexCount, iVertex);
	  break;
	}
	if (form.satinGuideCount != 0U) {
	  si::satmf(form, lengths);
	  break;
	}
	length /= 2.0F;
	auto iVertex = 0U;
	if (!StateMap->test(StateFlag::BARSAT) && !StateMap->test(StateFlag::FTHR)) {
	  OSequence->push_back(*itFirstVertex);
	}
	while (length > lengths[iVertex]) {
	  ++iVertex;
	}
	auto const deltaA = lengths[iVertex] - length;
	auto const deltaB = length - lengths[wrap::toSize(iVertex) - 1U];
	if (deltaB > deltaA) {
	  --iVertex;
	}
	si::satfn(form, lengths, 0, iVertex, form.vertexCount, iVertex);
  } while (false);
  LineSpacing = savedSpacing;
}

void satin::satfix() {
  auto const vertexCount = wrap::toUnsigned(TempPolygon->size());
  auto       minSize     = 1U;
  if (FormList->back().type == FRMFPOLY) {
	minSize = 2U;
  }
  if (TempPolygon->size() > minSize) {
	FormList->back().vertexIndex    = thred::adflt(vertexCount);
	auto const itVertex = wrap::next(FormVertices->begin(), FormList->back().vertexIndex);
	std::copy(TempPolygon->cbegin(), TempPolygon->cend(), itVertex);
	TempPolygon->clear();
	FormList->back().vertexCount = vertexCount;
	form::frmout(wrap::toUnsigned(FormList->size() - 1U));
	FormList->back().satinGuideCount = 0;
	StateMap->set(StateFlag::INIT);
  }
  else {
	TempPolygon->clear();
	FormList->pop_back();
  }
  StateMap->reset(StateFlag::SHOSAT);
  StateMap->set(StateFlag::RESTCH);
}

void satin::dusat() noexcept {
  auto const  vertexCount = TempPolygon->size();
  auto const& formLines   = *FormLines;
  auto const* line        = &formLines[vertexCount - 1U];
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::Polyline(StitchWindowDC, line, LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void satin::internal::unsat() {
  if (StateMap->testAndReset(StateFlag::SHOSAT)) {
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
  StateMap->set(StateFlag::SHOSAT);
  satin::dusat();
}

void satin::satpnt0() {
  auto& formLines = *FormLines;
  formLines.clear();
  formLines.push_back(POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y});
  TempPolygon->push_back(thred::pxCor2stch(Msg.pt));
  StateMap->set(StateFlag::SATPNT);
}

void satin::satpnt1() {
  si::unsat();
  auto const vertexCount   = TempPolygon->size();
  auto&      formLines     = *FormLines;
  formLines[vertexCount].x = Msg.pt.x - StitchWindowOrigin.x;
  formLines[vertexCount].y = Msg.pt.y - StitchWindowOrigin.y;
  satin::dusat();
  TempPolygon->push_back(thred::pxCor2stch(Msg.pt));
  StateMap->set(StateFlag::RESTCH);
}

void satin::internal::filinsbw(std::vector<fPOINT>& satinBackup,
                               fPOINT const&        point,
                               uint32_t&            satinBackupIndex,
                               fPOINT&              stitchPoint) {
  satinBackup[satinBackupIndex++] = point;
  satinBackupIndex &= (satinBackup.size() - 1U); // make sure that satinBackupIndex rolls over when it reaches the end of the buffer
  form::filinsb(point, stitchPoint);
}

void satin::internal::sbfn(std::vector<fPOINT> const& insidePoints, uint32_t start, uint32_t finish, fPOINT& stitchPoint) {
  auto const& outsidePoints = *OutsidePoints;
  auto  innerDelta    = fPOINT {(insidePoints[finish].x - insidePoints[start].x),
                            (insidePoints[finish].y - insidePoints[start].y)};
  auto  outerDelta    = fPOINT {(outsidePoints[finish].x - outsidePoints[start].x),
                            (outsidePoints[finish].y - outsidePoints[start].y)};

  auto const innerLength = hypot(innerDelta.x, innerDelta.y);
  auto const outerLength = hypot(outerDelta.x, outerDelta.y);
  auto       innerPoint  = fPOINT {insidePoints[start].x, insidePoints[start].y};
  auto       outerPoint  = fPOINT {outsidePoints[start].x, outsidePoints[start].y};
  auto       innerFlag   = false;
  auto       outerFlag   = false;
  if (!StateMap->testAndSet(StateFlag::SAT1)) {
	stitchPoint = insidePoints[start];
  }
  if (outerLength > innerLength) {
	innerFlag         = true;
	auto intersection = fPOINT {};
	if (form::linx(insidePoints, start, finish, intersection)) {
	  innerDelta = fPOINT {};
	  innerPoint = intersection;
	}
  }
  else {
	outerFlag         = true;
	auto intersection = fPOINT {};
	if (form::linx(insidePoints, start, finish, intersection)) {
	  outerDelta = fPOINT {};
	  outerPoint = intersection;
	}
  }
  auto count = wrap::round<uint32_t>(((outerLength > innerLength) ? outerLength : innerLength) / LineSpacing);
  if (count == 0U) {
	count = 1;
  }
  if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
	return;
  }
  auto satinBackup = std::vector<fPOINT> {}; // backup stitches in satin fills

  constexpr auto SATBUFSZ = 8U; // satin buffer size
  satinBackup.resize(SATBUFSZ);
  std::fill(satinBackup.begin(), satinBackup.end(), fPOINT {BIGFLOAT, BIGFLOAT});
  auto const innerStep = fPOINT {innerDelta.x / wrap::toFloat(count), innerDelta.y / wrap::toFloat(count)};
  auto const outerStep = fPOINT {outerDelta.x / wrap::toFloat(count), outerDelta.y / wrap::toFloat(count)};
  auto satinBackupIndex = 0U;
  for (auto iStep = 0U; iStep < count; ++iStep) {
	innerPoint.x += innerStep.x;
	innerPoint.y += innerStep.y;
	outerPoint.x += outerStep.x;
	outerPoint.y += outerStep.y;
	if (StateMap->testAndFlip(StateFlag::FILDIR)) {
	  if (innerFlag) {
		auto const offsetDelta = fPOINT {innerPoint.x - stitchPoint.x, innerPoint.y - stitchPoint.y};
		auto const offsetLength = hypot(offsetDelta.x, offsetDelta.y);
		auto const offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
		auto const offsetStep   = fPOINT {offsetDelta.x / wrap::toFloat(offsetCount),
                                        offsetDelta.y / wrap::toFloat(offsetCount)};
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
		auto const offsetDelta = fPOINT {outerPoint.x - stitchPoint.x, outerPoint.y - stitchPoint.y};
		auto const offsetLength = hypot(offsetDelta.x, offsetDelta.y);
		auto const offsetCount  = wrap::round<uint32_t>(offsetLength / LineSpacing);
		auto const offsetStep   = fPOINT {offsetDelta.x / wrap::toFloat(offsetCount),
                                        offsetDelta.y / wrap::toFloat(offsetCount)};
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
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, startVertex);
	si::sbfn(*InsidePoints, startVertex, nextVertex, stitchPoint);
	startVertex = nextVertex;
  }
  OSequence->front() = OSequence->back();
}

void satin::satzum() {
  StateMap->reset(StateFlag::SHOSAT);
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

	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	  constexpr auto DEFWIDTH = 0.1F; // default satin width
	  si::outfn(form, iVertex, iVertex + 1, DEFWIDTH);
	}
	auto count = 0U;
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  if (form::cisin(form, InsidePoints->operator[](iVertex).x, InsidePoints->operator[](iVertex).y)) {
		++count;
	  }
	}
	satinWidth /= 2.0F;
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	  si::outfn(form, iVertex, iVertex + 1, satinWidth);
	}
	si::outfn(form, form.vertexCount - 1U, 0, satinWidth);
	StateMap->reset(StateFlag::INDIR);
	if (count < (form.vertexCount / 2U)) {
	  StateMap->set(StateFlag::INDIR);
	  OutsidePoints = InsidePointList;
	  InsidePoints  = OutsidePointList;
	}
  }
}

void satin::sbrd(FRMHED const& form) {
  auto const savedSpacing = LineSpacing;
  auto const start        = form::getlast(form);
  auto       stitchPoint  = fPOINT {};
  StateMap->reset(StateFlag::SAT1);
  StateMap->reset(StateFlag::FILDIR);
  OSequence->clear();
  OSequence->push_back(fPOINT {});
  if ((form.edgeType & EGUND) != 0U) {
	LineSpacing = USPAC;
	satin::satout(form, form.borderSize * URAT);
	si::sfn(form, start, stitchPoint);
	StateMap->set(StateFlag::FILDIR);
	si::sfn(form, start, stitchPoint);
  }
  satin::satout(form, form.borderSize);
  LineSpacing = form.edgeSpacing;
  si::sfn(form, start, stitchPoint);
  LineSpacing = savedSpacing;
}

auto satin::internal::satOffset(const uint32_t& finish, const uint32_t& start, float satinWidth) noexcept
    -> fPOINT {
  constexpr auto SATHRESH = 10.0F;

  auto angle  = (FormAngles->operator[](finish) - FormAngles->operator[](start)) / 2.0F;
  auto factor = 1.0F / cos(angle);
  if (factor < -SATHRESH) {
	factor = -SATHRESH;
  }
  else {
	if (factor > SATHRESH) {
	  factor = SATHRESH;
	}
  }
  auto const length = satinWidth * factor;

  angle += FormAngles->operator[](start) + PI_FHALF;

  auto const retval = fPOINT {length * cos(angle), length * sin(angle)};
  return retval;
}

void satin::internal::outfn(FRMHED const& form, uint32_t start, uint32_t finish, float satinWidth) {
  auto const offset =
      (fabs(FormAngles->operator[](start)) < TNYFLOAT && fabs(FormAngles->operator[](finish)) < TNYFLOAT)
          ? fPOINT {0.0F, satinWidth}
          : satOffset(finish, start, satinWidth);
  auto const itVertex = ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT))
                            ? wrap::next(AngledFormVertices->cbegin(), form.vertexIndex + finish)
                            : wrap::next(FormVertices->cbegin(), form.vertexIndex + finish);
  InsidePoints-> operator[](finish).x = itVertex->x - offset.x;
  InsidePoints-> operator[](finish).y = itVertex->y - offset.y;
  OutsidePoints->operator[](finish).x = itVertex->x + offset.x;
  OutsidePoints->operator[](finish).y = itVertex->y + offset.y;
}

auto satin::internal::chkbak(std::vector<fPOINT> const& satinBackup, fPOINT const& pnt) noexcept -> bool {
  auto const maxSB = satinBackup.size();
  for (auto iBackup = 0U; iBackup < maxSB; ++iBackup) {
	auto const length = hypot(satinBackup[iBackup].x - pnt.x, satinBackup[iBackup].y - pnt.y);
	if (length < LineSpacing) {
	  return true;
	}
  }
  return false;
}

auto satin::adsatk(uint32_t count) -> uint32_t {
  auto const oldSize = wrap::toUnsigned(SatinGuides->size());
  auto const dest    = SatinGuides->end();
  auto const val     = SATCON {};
  SatinGuides->insert(dest, count, val);
  return oldSize;
}
