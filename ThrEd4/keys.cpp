// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "backup.h"
#include "displayText.h"
#include "EnumMap.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "hlp.h"
#include "iniFile.h"
#include "keys.h"
#include "menu.h"
#include "mouse.h"
#include "point.h"
#include "Resources/resource.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "trace.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/narrow"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
// ReSharper disable CppUnusedIncludeDirective
#include <cstdint>
// ReSharper restore CppUnusedIncludeDirective
#include <iterator>
#include <string>
#include <vector>

// keys
namespace {
auto MoveAnchor       = uint32_t {}; // for resequencing stitches
auto LastFormSelected = uint32_t {}; // end point of selected range of forms

// Definitions
constexpr auto byteSwap(uint32_t data) noexcept -> uint32_t;
void           dufsel();
auto           handleEndKey(int32_t& retflag) -> bool;
auto           handleHomeKey(bool& retflag) -> bool;
auto           handleLeftKey(bool& retflag) -> bool;
auto           handleRightKey(bool& retflag) -> bool;
void           handleShiftedLeftKey();
void           handleShiftedRightKey();
void           istch();
void           mark();
auto           movstchs(uint32_t destination, uint32_t start, uint32_t finish) -> bool;
void           ritcur();
void           seldwn();
void           selfrm0();
void           selfrmx();
void           selup();
void           setMoveAnchor(uint32_t source) noexcept;

// Functions
constexpr auto byteSwap(uint32_t const data) noexcept -> uint32_t {
  auto const swapped = ((data & 0x000000FFU) << 24U) | ((data & 0x0000FF00U) << 8U) |
                       ((data & 0x00FF0000U) >> 8U) | ((data & 0xFF000000U) >> 24U);
  return swapped;
}

void dufsel() {
  auto start = LastFormSelected > ClosestFormToCursor ? ClosestFormToCursor : LastFormSelected;
  auto const finish = LastFormSelected > ClosestFormToCursor ? LastFormSelected : ClosestFormToCursor;
  Instance->selectedFormList.clear();
  Instance->selectedFormList.reserve(wrap::toSize(finish) - start + 1U);
  while (start <= finish) {
	Instance->selectedFormList.push_back(start);
	++start;
  }
}

auto handleEndKey(int32_t& retflag) -> bool {
  retflag = 1;
  if (wrap::pressed(VK_SHIFT) && wrap::pressed(VK_CONTROL)) {
	if (!Instance->stitchBuffer.empty()) {
	  if (Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
		GroupStitchIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
		Instance->stateMap.set(StateFlag::GRPSEL);
	  }
	  else {
		if (Instance->stateMap.test(StateFlag::GRPSEL)) {
		  if (GroupStitchIndex > ClosestPointIndex) {
			GroupStitchIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
		  }
		  else {
			ClosestPointIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
		  }
		}
		else {
		  Instance->stateMap.set(StateFlag::SELBOX);
		  ClosestPointIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
		  Instance->stateMap.set(StateFlag::RESTCH);
		  return true;
		}
	  }
	  thred::grpAdj();
	  thred::redrawColorBar();
	  retflag = 2;
	  return true;
	}
	return false;
  }
  if (wrap::pressed(VK_SHIFT)) {
	auto const iColor = Instance->stateMap.testAndReset(StateFlag::SELBOX)
	                        ? thred::pt2colInd(ClosestPointIndex)
	                        : thred::pt2colInd(GroupStitchIndex);
	if (Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
	  Instance->stateMap.set(StateFlag::GRPSEL);
	}
	GroupStitchIndex = thred::getColorChangeIndex(iColor) - 1U;
	thred::grpAdj();
	thred::redrawColorBar();
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  auto stitchCoordsInPixels = POINT {0L, StitchWindowClientRect.bottom};
	  if (!Instance->stitchBuffer.empty()) {
		stitchCoordsInPixels = thred::stch2px1(wrap::toUnsigned(Instance->stitchBuffer.size() - 1U));
	  }
	  thred::endpnt(stitchCoordsInPixels);
	  Instance->stateMap.set(StateFlag::BAKEND);
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
	else {
	  if (Instance->stateMap.test(StateFlag::SELBOX)) {
		auto const iColor = thred::pt2colInd(ClosestPointIndex);
		ClosestPointIndex = thred::getColorChangeIndex(iColor) - 1U;
	  }
	  else {
		if (Instance->stateMap.test(StateFlag::LENSRCH)) {
		  thred::setSrchLargest();
		  return true;
		}
		ClosestPointIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
	  }
	  thred::movbox();
	}
  }
  retflag = 0;
  return false;
}

auto handleHomeKey(bool& retflag) -> bool {
  retflag = true;
  if (wrap::pressed(VK_SHIFT) && wrap::pressed(VK_CONTROL)) {
	if (Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
	  GroupStitchIndex = 0;
	  Instance->stateMap.set(StateFlag::GRPSEL);
	}
	else {
	  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
		if (GroupStitchIndex > ClosestPointIndex) {
		  ClosestPointIndex = 0;
		}
		else {
		  GroupStitchIndex = 0;
		}
	  }
	  else {
		Instance->stateMap.set(StateFlag::SELBOX);
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	}
	thred::grpAdj();
	thred::redrawColorBar();
	return true;
  }
  if (wrap::pressed(VK_SHIFT)) {
	auto iColor = Instance->stateMap.testAndReset(StateFlag::SELBOX)
	                  ? thred::pt2colInd(ClosestPointIndex)
	                  : thred::pt2colInd(GroupStitchIndex);
	if (Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
	  Instance->stateMap.set(StateFlag::GRPSEL);
	}
	if (iColor != 0U) {
	  --iColor;
	}
	GroupStitchIndex = thred::getColorChangeIndex(iColor);
	thred::grpAdj();
	thred::redrawColorBar();
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  auto const stitchCoordsInPixels = thred::stch2px1(0);
	  thred::endpnt(stitchCoordsInPixels);
	  Instance->stateMap.reset(StateFlag::BAKEND);
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
	else {
	  if (Instance->stateMap.test(StateFlag::SELBOX)) {
		auto iColor = thred::pt2colInd(ClosestPointIndex);
		if (iColor != 0U) {
		  --iColor;
		}
		ClosestPointIndex = thred::getColorChangeIndex(iColor);
	  }
	  else {
		if (Instance->stateMap.test(StateFlag::LENSRCH)) {
		  thred::setSrchSmallest();
		  return true;
		}
		ClosestPointIndex = 0;
	  }
	  thred::movbox();
	}
  }
  retflag = false;
  return {};
}

auto handleLeftKey(bool& retflag) -> bool {
  retflag = true;
  if (wrap::pressed(VK_SHIFT)) {
	handleShiftedLeftKey();
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  thred::nudgfn(-IniFile.cursorNudgeStep, 0);
	}
	else {
	  if (Instance->stateMap.test(StateFlag::LENSRCH)) {
		thred::nextSortedStitch(false);
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	  else {
		if (Instance->stateMap.test(StateFlag::FRMPSEL)) {
		  auto const& form      = Instance->formList.operator[](ClosestFormToCursor);
		  ClosestVertexToCursor = form::prv(form, ClosestVertexToCursor);
		  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
		  auto const itVertex =
		      wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + ClosestVertexToCursor);
		  thred::ritfcor(*itVertex);
		  thred::shftflt(*itVertex);
		  Instance->stateMap.set(StateFlag::RESTCH);
		}
		else {
		  if (Instance->stateMap.test(StateFlag::SELBOX)) {
			if (ClosestPointIndex != 0U) {
			  --ClosestPointIndex;
			}
			thred::movbox();
			return true;
		  }
		  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
			if (GroupStitchIndex != 0U) {
			  --GroupStitchIndex;
			  thred::grpAdj();
			  thred::redrawColorBar();
			}
		  }
		}
	  }
	}
  }
  retflag = false;
  return {};
}

auto handleRightKey(bool& retflag) -> bool {
  retflag = true;
  if (wrap::pressed(VK_SHIFT)) {
	handleShiftedRightKey();
  }
  else {
	if (wrap::pressed(VK_CONTROL)) {
	  thred::nudgfn(IniFile.cursorNudgeStep, 0);
	}
	else {
	  if (Instance->stateMap.test(StateFlag::LENSRCH)) {
		thred::nextSortedStitch(true);
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	  else {
		if (Instance->stateMap.test(StateFlag::FRMPSEL)) {
		  auto const& formList = Instance->formList;

		  ClosestVertexToCursor = form::nxt(formList.operator[](ClosestFormToCursor), ClosestVertexToCursor);
		  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
		  auto const itVertex =
		      wrap::next(Instance->formVertices.cbegin(),
		                 formList.operator[](ClosestFormToCursor).vertexIndex + ClosestVertexToCursor);
		  thred::ritfcor(*itVertex);
		  thred::shftflt(*itVertex);
		  Instance->stateMap.set(StateFlag::RESTCH);
		}
		else {
		  if (Instance->stateMap.test(StateFlag::SELBOX)) {
			if (ClosestPointIndex < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
			  ++ClosestPointIndex;
			}
			thred::movbox();
			return true;
		  }
		  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
			if (GroupStitchIndex < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
			  ++GroupStitchIndex;
			  thred::grpAdj();
			  thred::redrawColorBar();
			}
		  }
		}
	  }
	}
  }
  retflag = false;
  return {};
}

void handleShiftedLeftKey() {
  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	auto const& vertexCount = Instance->formList.operator[](ClosestFormToCursor).vertexCount;
	if (!Instance->stateMap.test(StateFlag::PSELDIR)) {
	  ++SelectedFormVertices.vertexCount %= vertexCount;
	  SelectedFormVertices.finish =
	      (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
	}
	else {
	  if (SelectedFormVertices.vertexCount != 0U) {
		--SelectedFormVertices.vertexCount;
		SelectedFormVertices.finish =
		    (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
	  }
	  else {
		SelectedFormVertices.vertexCount = 1;
		SelectedFormVertices.finish = (SelectedFormVertices.start + vertexCount - 1) % vertexCount;
		Instance->stateMap.reset(StateFlag::PSELDIR);
	  }
	}
	thred::setpsel();
  }
  else {
	if (Instance->stateMap.testAndReset(StateFlag::FRMPSEL)) {
	  form::unpsel();
	  SelectedFormVertices.start       = ClosestVertexToCursor;
	  SelectedFormVertices.vertexCount = 1;
	  Instance->stateMap.reset(StateFlag::PSELDIR);
	  thred::setpsel();
	}
	else {
	  Instance->stateMap.reset(StateFlag::LENSRCH);
	  Instance->stateMap.reset(StateFlag::FORMSEL);
	  if (Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
		Instance->stateMap.set(StateFlag::GRPSEL);
		GroupStitchIndex = ClosestPointIndex - 1U;
	  }
	  else if (GroupStitchIndex != 0U) {
		--GroupStitchIndex;
		thred::nuAct(GroupStitchIndex);
	  }
	  thred::grpAdj();
	  thred::redrawColorBar();
	}
  }
}

void handleShiftedRightKey() {
  auto const& formList = Instance->formList;

  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	auto const& vertexCount = formList.operator[](ClosestFormToCursor).vertexCount;
	if (Instance->stateMap.test(StateFlag::PSELDIR)) {
	  ++SelectedFormVertices.vertexCount %= vertexCount;
	  SelectedFormVertices.finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % vertexCount;
	}
	else {
	  if (SelectedFormVertices.vertexCount != 0U) {
		--SelectedFormVertices.vertexCount;
		SelectedFormVertices.finish =
		    (SelectedFormVertices.start + vertexCount - SelectedFormVertices.vertexCount) % vertexCount;
	  }
	  else {
		SelectedFormVertices.vertexCount = 1;
		Instance->stateMap.set(StateFlag::PSELDIR);
		SelectedFormVertices.finish = (SelectedFormVertices.start + 1U) % vertexCount;
	  }
	}
	thred::setpsel();
  }
  else {
	if (Instance->stateMap.testAndReset(StateFlag::FRMPSEL)) {
	  auto const& vertexCount = formList.operator[](ClosestFormToCursor).vertexCount;
	  form::unpsel();
	  SelectedFormVertices.start       = ClosestVertexToCursor;
	  SelectedFormVertices.form        = ClosestFormToCursor;
	  SelectedFormVertices.vertexCount = 1;
	  SelectedFormVertices.finish      = (SelectedFormVertices.start + 1U) % vertexCount;
	  Instance->stateMap.set(StateFlag::PSELDIR);
	  thred::setpsel();
	}
	else {
	  Instance->stateMap.reset(StateFlag::LENSRCH);
	  Instance->stateMap.reset(StateFlag::FORMSEL);
	  if (Instance->stateMap.testAndReset(StateFlag::SELBOX)) {
		if (!Instance->stitchBuffer.empty()) {
		  if (auto const lastStitch = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
		      ClosestPointIndex < lastStitch) {
			Instance->stateMap.set(StateFlag::GRPSEL);
			GroupStitchIndex = ClosestPointIndex + 1U;
		  }
		}
	  }
	  else {
		if (!Instance->stitchBuffer.empty()) {
		  if (auto const lastStitch = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
		      GroupStitchIndex < lastStitch) {
			++GroupStitchIndex;
			thred::nuAct(GroupStitchIndex);
		  }
		}
	  }
	  thred::grpAdj();
	  thred::redrawColorBar();
	}
  }
}

void istch() {
  thred::xlin();
  thred::xlin1();
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	if (ClosestPointIndex != 0U && ClosestPointIndex != wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
	  auto const prvStitch   = wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex - 1U);
	  auto const stitch      = std::next(prvStitch);
	  auto const angb        = std::atan2(stitch->y - prvStitch->y, stitch->x - prvStitch->x);
	  auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
	  auto const angt        = std::atan2(stitch->y - stitchPoint.y, stitch->x - stitchPoint.x);
	  auto const nxtStitch   = std::next(stitch);
	  if (auto const angf = std::atan2(stitch->y - nxtStitch->y, stitch->x - nxtStitch->x);
	      fabs(angf - angt) > fabs(angb - angt)) {
		--ClosestPointIndex;
	  }
	}
	else {
	  if (ClosestPointIndex == wrap::toUnsigned(Instance->stitchBuffer.size() - 1U)) {
		--ClosestPointIndex;
	  }
	}
  }
  else {
	ClosestPointIndex = thred::closlin();
  }
  if (ClosestPointIndex == gsl::narrow_cast<decltype(ClosestPointIndex)>(-1)) {
	Instance->stateMap.reset(StateFlag::INSRT);
  }
  else {
	Instance->stateMap.set(StateFlag::INSRT);
	thred::duIns();
	SetCapture(ThrEdWindow);
	displayText::ritnum(IDS_NUMSEL, ClosestPointIndex);
	thred::nuAct(ClosestPointIndex);
  }
}

void mark() {
  if (!thred::closPnt1(ClosestPointIndex) && !Instance->stateMap.test(StateFlag::SELBOX) &&
      !Instance->stateMap.test(StateFlag::INSRT)) {
	return;
  }
  thred::unbox();
  thred::xlin();
  Instance->stateMap.reset(StateFlag::CAPT);
  Instance->stateMap.reset(StateFlag::SELBOX);
  Instance->stateMap.reset(StateFlag::INSRT);
  Instance->stateMap.reset(StateFlag::SCROS);
  Instance->stateMap.reset(StateFlag::ECROS);
  GroupStitchIndex = ClosestPointIndex;
  Instance->stateMap.set(StateFlag::GRPSEL);
}

auto movstchs(uint32_t destination, uint32_t start, uint32_t finish) -> bool {
  auto tempStitchBuffer = std::vector<F_POINT_ATTR> {};
  if (destination + 1U < wrap::toUnsigned(Instance->stitchBuffer.size())) {
	++destination;
  }
  if (start > finish) {
	std::swap(start, finish);
  }
  if (destination >= start && destination <= finish) {
	displayText::tabmsg(IDS_DST1, false);
	return false;
  }
  if (destination < start) {
	auto const bufSize = finish - destination;
	tempStitchBuffer.resize(bufSize);
	std::copy(wrap::next(Instance->stitchBuffer.begin(), start),
	          wrap::next(Instance->stitchBuffer.begin(), finish),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(Instance->stitchBuffer.begin(), destination),
	          wrap::next(Instance->stitchBuffer.begin(), start),
	          wrap::next(tempStitchBuffer.begin(), finish - start));
	std::ranges::copy(tempStitchBuffer, wrap::next(Instance->stitchBuffer.begin(), destination));
  }
  else {
	auto const bufSize = destination - start;
	tempStitchBuffer.resize(bufSize);
	std::copy(wrap::next(Instance->stitchBuffer.begin(), finish),
	          wrap::next(Instance->stitchBuffer.begin(), destination),
	          tempStitchBuffer.begin());
	std::copy(wrap::next(Instance->stitchBuffer.begin(), start),
	          wrap::next(Instance->stitchBuffer.begin(), finish),
	          wrap::next(tempStitchBuffer.begin(), destination - finish));
	std::ranges::copy(tempStitchBuffer, wrap::next(Instance->stitchBuffer.begin(), start));
  }
  return true;
}

void ritcur() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const currentCursor = GetCursor();
  if (currentCursor == nullptr) {
	return;
  }
  auto iconInfo = ICONINFO {FALSE, 0U, 0U, nullptr, nullptr};
  GetIconInfo(currentCursor, &iconInfo);
  auto cursorPosition = POINT {};
  GetCursorPos(&cursorPosition);
  cursorPosition.x -= StitchWindowOrigin.x + gsl::narrow_cast<LONG>(iconInfo.xHotspot);
  cursorPosition.y -= StitchWindowOrigin.y + gsl::narrow_cast<LONG>(iconInfo.yHotspot);
  // ToDo - replace with GetDIBits
  constexpr auto ICONSIZE = 64U; // size in bytes of an icon bitmap
  constexpr auto ICONROWS = 32;  // rows in the icon

  auto bitmapBits = std::array<uint8_t, ICONSIZE> {};
  auto iBMB       = bitmapBits.begin();
  auto iIBMB      = std::next(bitmapBits.begin(), ICONROWS);
  GetBitmapBits(iconInfo.hbmMask, gsl::narrow<LONG>(bitmapBits.size()), bitmapBits.data());
  if (currentCursor != mouse::getArrowCursor()) {
	for (auto iRow = 0; iRow < ICONROWS; ++iRow) {
	  auto const     bitmapInverse = byteSwap(*iIBMB++);
	  auto           bitMask       = uint32_t {1U} << HBSHFT;
	  constexpr auto BPINT         = 32; // bits in an uint32_t
	  for (auto iPixel = 0; iPixel < BPINT; ++iPixel) {
		if ((bitMask & bitmapInverse) != 0U) {
		  constexpr auto ICOLMASK = 0xffffffU;
		  SetPixel(StitchWindowDC,
		           cursorPosition.x + iPixel,
		           cursorPosition.y + iRow,
		           GetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow) ^ ICOLMASK);
		}
		bitMask >>= 1U;
	  }
	}
	return;
  }
  for (auto iRow = 0; iRow < ICONROWS; ++iRow) {
	auto const     mask          = byteSwap(*iBMB++);
	auto const     bitmapInverse = byteSwap(*iIBMB++);
	auto           bitMask       = uint32_t {1U} << HBSHFT;
	constexpr auto BPINT         = 32; // bits in an uint32_t
	for (auto iPixel = 0; iPixel < BPINT; ++iPixel) {
	  if ((bitMask & mask) == 0U) {
		auto const pixelColor = (bitMask & bitmapInverse) != 0U ? PENWHITE : PENBLK;
		SetPixel(StitchWindowDC, cursorPosition.x + iPixel, cursorPosition.y + iRow, pixelColor);
	  }
	  bitMask >>= 1U;
	}
  }
}

void seldwn() {
  if (wrap::pressed(VK_SHIFT)) {
	Instance->stateMap.reset(StateFlag::SELBOX);
	if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
	  if (ClosestFormToCursor != 0U) {
		Instance->selectedFormList.push_back(ClosestFormToCursor);
		LastFormSelected = ClosestFormToCursor - 1U;
		Instance->selectedFormList.push_back(LastFormSelected);
	  }
	  else {
		return;
	  }
	}
	else {
	  if (!Instance->selectedFormList.empty()) {
		if (LastFormSelected != 0U) {
		  --LastFormSelected;
		  dufsel();
		}
	  }
	  else {
		Instance->stateMap.set(StateFlag::FORMSEL);
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	thred::unbox();
	auto const attribute = Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & ATMSK;
	while (ClosestPointIndex != 0U &&
	       (Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & ATMSK) == attribute) {
	  --ClosestPointIndex;
	}
	auto stitchCoordsInPixels = POINT {};
	thred::stch2px(ClosestPointIndex, stitchCoordsInPixels);
	thred::dubox(stitchCoordsInPixels);
	return;
  }

  if (auto const& formList = Instance->formList; !formList.empty()) {
	if (Instance->stateMap.testAndSet(StateFlag::FORMSEL)) {
	  if (ClosestFormToCursor != 0U) {
		--ClosestFormToCursor;
	  }
	}
	else {
	  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
	}
	displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void selfrm0() {
  Instance->stateMap.reset(StateFlag::GRPSEL);
  if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
	Instance->stateMap.set(StateFlag::FRMPSEL);
	ClosestVertexToCursor = 0;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void selfrmx() {
  Instance->stateMap.reset(StateFlag::GRPSEL);
  if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
	Instance->stateMap.set(StateFlag::FRMPSEL);
	ClosestVertexToCursor = Instance->formList.operator[](ClosestFormToCursor).vertexCount - 1U;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void selup() {
  auto const& formList = Instance->formList;
  if (wrap::pressed(VK_SHIFT)) {
	Instance->stateMap.reset(StateFlag::SELBOX);
	if (Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
	  if (ClosestFormToCursor < formList.size() - 1U) {
		Instance->selectedFormList.push_back(ClosestFormToCursor);
		LastFormSelected = ClosestFormToCursor + 1U;
		Instance->selectedFormList.push_back(LastFormSelected);
	  }
	  else {
		return;
	  }
	}
	else {
	  if (!Instance->selectedFormList.empty()) {
		if (LastFormSelected < formList.size() - 1U) {
		  ++LastFormSelected;
		  dufsel();
		}
	  }
	  else {
		Instance->stateMap.set(StateFlag::FORMSEL);
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	thred::unbox();
	auto const attribute = Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & ATMSK;
	while (ClosestPointIndex < wrap::toUnsigned(Instance->stitchBuffer.size() - 1U) &&
	       (Instance->stitchBuffer.operator[](ClosestPointIndex).attribute & ATMSK) == attribute) {
	  ++ClosestPointIndex;
	}
	auto stitchCoordsInPixels = POINT {};
	thred::stch2px(ClosestPointIndex, stitchCoordsInPixels);
	thred::dubox(stitchCoordsInPixels);
	return;
  }
  if (!formList.empty()) {
	if (Instance->stateMap.testAndSet(StateFlag::FORMSEL)) {
	  if (ClosestFormToCursor < wrap::toUnsigned(formList.size()) - 1U) {
		++ClosestFormToCursor;
	  }
	}
	else {
	  ClosestFormToCursor = 0;
	}
	displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void setMoveAnchor(uint32_t const source) noexcept {
  MoveAnchor = source;
}
} // namespace

auto keys::getMoveAnchor() noexcept -> uint32_t {
  return MoveAnchor;
}

void keys::movmrk() {
  if (!Instance->stateMap.test(StateFlag::MOVSET)) {
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	if (movstchs(MoveAnchor, GroupStartStitch, GroupEndStitch)) {
	  thred::coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
	return;
  }
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	if (movstchs(MoveAnchor, ClosestPointIndex, ClosestPointIndex + 1U)) {
	  Instance->stateMap.set(StateFlag::RESTCH);
	  thred::coltab();
	}
  }
}

void keys::setmov() {
  if (Instance->stateMap.test(StateFlag::SELBOX)) {
	setMoveAnchor(ClosestPointIndex);
	Instance->stateMap.set(StateFlag::MOVSET);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void keys::ungrplo() {
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupStartStitch;
	Instance->stateMap.set(StateFlag::SELBOX);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::grpmsg();
	return;
  }
  auto flag    = true;
  auto iStitch = 0U;
  for (auto const& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & NOTFRM) == 0U && (stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
	  ClosestPointIndex = iStitch;
	  Instance->stateMap.set(StateFlag::SELBOX);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  flag = false;
	  break;
	}
	++iStitch;
  }
  if (flag) {
	displayText::grpmsg1();
  }
}

void keys::ungrphi() {
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	thred::rngadj();
	ClosestPointIndex = GroupEndStitch;
	Instance->stateMap.set(StateFlag::SELBOX);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::grpmsg();
	return;
  }
  auto flag = true;
  for (auto iStitch = wrap::toUnsigned(Instance->stitchBuffer.size()); iStitch != 0; --iStitch) {
	if (auto const prevStitch = iStitch - 1U;
	    (Instance->stitchBuffer.operator[](prevStitch).attribute & NOTFRM) == 0U &&
	    (Instance->stitchBuffer.operator[](prevStitch).attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
	  ClosestPointIndex = iStitch - 1U;
	  Instance->stateMap.set(StateFlag::SELBOX);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  flag = false;
	  break;
	}
  }
  if (flag) {
	displayText::grpmsg1();
  }
}

void keys::desiz() {
  constexpr auto MMTOINCH = 1 / 25.4F; // conversion factor for millimeters to inches

  auto rectangle = F_RECTANGLE {};
  auto info      = std::wstring {};
  if (!Instance->stitchBuffer.empty()) {
	thred::stchrct(rectangle);
	auto const xSize = (rectangle.right - rectangle.left) * IPFGRAN;
	auto const ySize = (rectangle.top - rectangle.bottom) * IPFGRAN;
	if (rectangle.left < 0 || rectangle.bottom < 0 || rectangle.right > IniFile.hoopSizeX ||
	    rectangle.top > IniFile.hoopSizeY) {
	  info += displayText::loadStr(IDS_STCHOUT);
	}
	info += displayText::format5(
	    IDS_STCHS, wrap::toUnsigned(Instance->stitchBuffer.size()), xSize, xSize * MMTOINCH, ySize, ySize * MMTOINCH);
  }

  if (auto const& formList = Instance->formList; !formList.empty()) {
	thred::frmrct(rectangle);
	auto const xSize = (rectangle.right - rectangle.left) * IPFGRAN;
	auto const ySize = (rectangle.top - rectangle.bottom) * IPFGRAN;
	info += displayText::format5(IDS_FORMS, formList.size(), xSize, xSize * MMTOINCH, ySize, ySize * MMTOINCH);
  }
  info += displayText::format2(IDS_HUPWID, IniFile.hoopSizeX * IPFGRAN, IniFile.hoopSizeY * IPFGRAN);
  if (!Instance->stitchBuffer.empty()) {
	info += thred::getDesignerFormatted();
  }
  displayText::shoMsg(info, true);
}

auto keys::handleMainWinKeys(wchar_t const&            code,
                             F_POINT&                  rotationCenter,
                             std::vector<POINT> const& stretchBoxLine) -> bool {
  switch (code) {
	case VK_ESCAPE: {
	  thred::esccode();
	  [[fallthrough]];
	}
	case L'Q': {
	  if (wrap::pressed(VK_SHIFT)) {
		ritcur();
		return true;
	  }
	  thred::qcode();
	  return true;
	}
	case VK_OEM_3: {                                             // '`~' for US
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_MENU)) { // CTRL + ALT
		thred::updateBackground();
		Instance->stateMap.set(StateFlag::RESTCH);
		break;
	  }
	  thred::tst();
	  break;
	}
	case VK_PRIOR: { // page up
	  if (wrap::pressed(VK_SHIFT)) {
		Instance->stateMap.reset(StateFlag::ROTCLOCK);
		form::dupfn(IniFile.rotationAngle);
		break;
	  }
	  form::rotagain();
	  break;
	}
	case VK_NEXT: { // page down
	  if (wrap::pressed(VK_SHIFT)) {
		Instance->stateMap.set(StateFlag::ROTCLOCK);
		form::dupfn(IniFile.rotationAngle);
		break;
	  }
	  form::bakagain();
	  break;
	}
	case VK_SUBTRACT: { // keypad -
	  thred::toglup();
	  break;
	}
	case VK_OEM_MINUS: { //-
	  Instance->stateMap.reset(StateFlag::CNTRH);
	  Instance->stateMap.reset(StateFlag::CNTRV);
	  if (wrap::pressed(VK_SHIFT)) {
		Instance->stateMap.set(StateFlag::CNTRV);
	  }
	  else {
		if (wrap::pressed(VK_CONTROL)) {
		  Instance->stateMap.set(StateFlag::CNTRH);
		}
	  }
	  form::cntrx();
	  break;
	}
	case VK_OEM_1: { // ';:' for US
	  movmrk();
	  break;
	}
	case VK_OEM_PLUS: { // '+' any country
	  form::shrnk();
	  break;
	}
	case VK_OEM_COMMA: { // ',' any country
	  if (wrap::pressed(VK_SHIFT)) {
		xt::setfilstrt();
		break;
	  }
	  thred::savdo();
	  form::join();
	  break;
	}
	case VK_OEM_PERIOD: { // '.' any country
	  if (wrap::pressed(VK_SHIFT)) {
		xt::setfilend();
		break;
	  }
	  setmov();
	  break;
	}
	case VK_OEM_2: { // '/?' for US
	  formForms::frmnum();
	  break;
	}
	case VK_OEM_4: { //  '[{' for US
	  if (wrap::pressed(VK_SHIFT)) {
		selfrm0();
		break;
	  }
	  ungrplo();
	  break;
	}
	case VK_OEM_6: { //  ']}' for US
	  if (wrap::pressed(VK_SHIFT)) {
		selfrmx();
		break;
	  }
	  ungrphi();
	  break;
	}
	case VK_OEM_7: { //  ''"' for US
	  desiz();
	  break;
	}
	case VK_F1: {
	  hlp::help();
	  break;
	}
	case VK_F2: {
	  form::snap();
	  break;
	}
	case VK_F3: {
	  if (wrap::pressed(VK_CONTROL)) {
		form::srtfrm();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		form::srtbyfrm();
		break;
	  }
	  xt::fsort();
	  break;
	}
	case VK_F4: {
	  tfc::rtrclp();
	  break;
	}
	case VK_F5: {
	  thred::filfrms();
	  break;
	}
	case VK_F6: {
	  menu::vuthrds();
	  break;
	}
	case VK_F7: {
	  thred::save();
	  break;
	}
	case VK_F8: {
	  thred::savAs();
	  break;
	}
	case VK_F9: {
	  thred::selCol();
	  break;
	}
	case VK_F11: {
	  if (wrap::pressed(VK_SHIFT)) {
		thred::rembig();
		break;
	  }
	  thred::duzero();
	  break;
	}
	case VK_F12: {
	  if (wrap::pressed(VK_CONTROL)) {
		if (wrap::pressed(VK_MENU)) {
		  thred::reldun();
		  break;
		}
#ifdef _DEBUG
		if (OpenClipboard(ThrEdWindow) != 0) {
		  EmptyClipboard();
		}
		CloseClipboard();
		break;
#endif
	  }
#ifdef _DEBUG
	  if (wrap::pressed(VK_SHIFT)) {
		xt::dmpat();
		break;
	  }
#endif
	  thred::dun();
	  break;
	}
	case VK_INSERT: {
	  thred::chkInsCol();
	  break;
	}
	case VK_DELETE: {
	  thred::chkDelCol();
	  break;
	}
	case VK_TAB: {
	  thred::rot(rotationCenter);
	  break;
	}
	case VK_SPACE: {
	  if (Instance->stateMap.testAndFlip(StateFlag::INSRT)) {
		ReleaseCapture();
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	  else {
		istch();
	  }
	  thred::unbox();
	  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL) ||
	      Instance->stateMap.testAndReset(StateFlag::FORMSEL)) {
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	  break;
	}
	case VK_HOME: {
	  auto       homeFlag = true;
	  auto const retval   = handleHomeKey(homeFlag);
	  if (homeFlag) {
		return retval;
	  }
	  break;
	}
	case VK_END: {
	  auto       endFlag = 1;
	  auto const retval  = handleEndKey(endFlag);
	  if (endFlag == 2) {
		break;
	  }
	  if (endFlag == 1) {
		return retval;
	  }
	  break;
	}
	case VK_DOWN: {
	  if (wrap::pressed(VK_CONTROL)) {
		thred::nudgfn(0, -IniFile.cursorNudgeStep);
		break;
	  }
	  seldwn();
	  break;
	}
	case VK_UP: {
	  if (wrap::pressed(VK_CONTROL)) {
		thred::nudgfn(0, IniFile.cursorNudgeStep);
		break;
	  }
	  selup();
	  break;
	}
	case VK_RIGHT: {
	  auto       rightFlag = true;
	  auto const retval    = handleRightKey(rightFlag);
	  if (rightFlag) {
		return retval;
	  }
	  break;
	}
	case VK_LEFT: {
	  auto       leftFlag = true;
	  auto const retval   = handleLeftKey(leftFlag);
	  if (leftFlag) {
		return retval;
	  }
	  break;
	}
	case L'A': {
	  if (wrap::pressed(VK_CONTROL)) {
		thred::selalstch();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		form::selal();
		break;
	  }
	  thred::zumout();
	  break;
	}
	case L'B': {
	  if (wrap::pressed(VK_CONTROL)) {
		if (Instance->stateMap.test(StateFlag::BAKACT) && Instance->stateMap.test(StateFlag::REDUSHO)) {
		  backup::redo();
		}
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		thred::bakmrk();
		break;
	  }
	  backup::bak();
	  break;
	}
	case L'C': {
	  if (wrap::pressed(VK_CONTROL)) {
		tfc::duclip();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		Instance->stateMap.reset(StateFlag::CNV2FTH);
		satin::ribon();
		break;
	  }
	  if (!thred::unselectAll()) {
		Instance->stateMap.reset(StateFlag::HIDSTCH);
		Instance->stateMap.set(StateFlag::IGNTHR);
		thred::rebox();
		Instance->stateMap.reset(StateFlag::IGNTHR);
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	  break;
	}
	case L'D': {
	  if (wrap::pressed(VK_SHIFT)) {
		if (Instance->stateMap.test(StateFlag::FORMSEL)) {
		  PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
		  keybd_event('F', 0, 0, 0);
		}
		break;
	  }
	  satin::satsel();
	  break;
	}
	case L'E': {
	  if (wrap::pressed(VK_SHIFT)) {
		texture::dutxtfil();
		break;
	  }
	  form::infrm();
	  break;
	}
	case L'F': {
	  if (!Instance->selectedFormList.empty()) {
		PostMessage(ThrEdWindow, WM_SYSCOMMAND, SC_KEYMENU, 'E');
		keybd_event('F', 0, 0, 0);
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		Instance->stateMap.set(StateFlag::CNV2FTH);
		satin::ribon();
		break;
	  }
	  form::frmon();
	  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
		formForms::refrm();
		break;
	  }
	  form::form();
	  break;
	}
	case L'G': {
	  mark();
	  break;
	}
	case L'H': {
	  if (wrap::pressed(VK_SHIFT)) {
		thred::pglft();
		break;
	  }
	  if (wrap::pressed(VK_CONTROL)) {
		trace::trcsel();
		break;
	  }
	  trace::blak();
	  break;
	}
	case L'J': {
	  if (wrap::pressed(VK_SHIFT)) {
		thred::pgrit();
		break;
	  }
	  form::refilal();
	  break;
	}
	case L'K': {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_MENU)) { // CTRL + ALT
		thred::setknots();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		thred::tglhid();
		break;
	  }
	  if (wrap::pressed(VK_CONTROL)) {
		thred::set1knot();
		break;
	  }
	  form::tglfrm();
	  break;
	}
	case L'L': {
	  if (wrap::pressed(VK_SHIFT)) {
		thred::delstch();
		break;
	  }
	  form::fcntr();
	  break;
	}
	case L'M': {
	  if (wrap::pressed(VK_CONTROL)) {
		thred::pntmrk();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		thred::dumrk(wrap::toFloat(UnzoomedRect.cx) * HALF, wrap::toFloat(UnzoomedRect.cy) * HALF);
		Instance->stateMap.set(StateFlag::RESTCH);
		break;
	  }
	  if (thred::inStitchWin()) {
		auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
		thred::dumrk(stitchPoint.x, stitchPoint.y);
		Instance->stateMap.set(StateFlag::RESTCH);
	  }
	  break;
	}
	case L'N': {
	  if (wrap::pressed(VK_CONTROL)) {
		xt::nudsiz();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		thred::pgdwn();
		break;
	  }
	  Instance->stateMap.set(StateFlag::TRCUP);
	  trace::trace();
	  break;
	}
	case L'O': {
	  thred::fop();
	  break;
	}
	case L'P': {
	  formForms::prfmsg();
	  break;
	}
	case L'R': {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_SHIFT)) {
		thred::movStch();
		thred::unbox();
		Instance->stateMap.set(StateFlag::RESTCH);
		break;
	  }
	  if (!formForms::inFormDataSheet()) {
		if (wrap::pressed(VK_CONTROL)) {
		  form::setrang();
		  return true;
		}
		if (wrap::pressed(VK_SHIFT)) {
		  thred::rotmrk();
		  return true;
		}
		thred::rotseg();
	  }
	  break;
	}
	case L'S': {
	  if (wrap::pressed(VK_CONTROL)) {
		thred::colchk();
		thred::save();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		thred::gsnap();
		break;
	  }
	  thred::zumshft();
	  break;
	}
	case L'T': {
	  if (wrap::pressed(VK_SHIFT)) {
		thred::retrac();
		break;
	  }
	  if (wrap::pressed(VK_CONTROL)) {
		trace::trinit();
		break;
	  }
	  thred::thumnail();
	  break;
	}
	case L'U': {
	  if (wrap::pressed(VK_SHIFT)) {
		thred::pgup();
		break;
	  }
	  if (wrap::pressed(VK_CONTROL)) {
		trace::trdif();
		break;
	  }
	  Instance->stateMap.reset(StateFlag::TRCUP);
	  trace::trace();
	  break;
	}
	case L'V': {
	  if (wrap::pressed(VK_CONTROL) && OpenClipboard(ThrEdWindow) != 0) {
		auto       pasteFlag = true;
		auto const retval    = tfc::doPaste(stretchBoxLine, pasteFlag);
		if (pasteFlag) {
		  return retval;
		}
		break;
	  }
	  thred::closPnt();
	  break;
	}
	case L'W': {
	  if (wrap::pressed(VK_CONTROL) && wrap::pressed(VK_MENU)) { // CTRL + ALT
		thred::contract();
		break;
	  }
	  if (wrap::pressed(VK_CONTROL)) {
		thred::stretch();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		form::crop();
		break;
	  }
	  form::insat();
	  break;
	}
	case L'X': {
	  if (wrap::pressed(VK_CONTROL)) {
		thred::cut();
		break;
	  }
	  if (wrap::pressed(VK_SHIFT)) {
		thred::hidbit();
		break;
	  }
	  thred::zumhom();
	  break;
	}
	case L'Y': {
	  if (form::closfrm(ClosestFormToCursor) && !thred::unselectAll()) {
		thred::selfpnt();
	  }
	  break;
	}
	case L'Z': {
	  if (wrap::pressed(VK_SHIFT)) {
		ZoomFactor = thred::getZoomMin();
		thred::zumin();
		break;
	  }
	  if (wrap::pressed(VK_CONTROL)) {
		backup::bak();
		break;
	  }
	  thred::zumin();
	  break;
	}
	default: {
	  outDebugString(L"default hit in handleMainWinKeys: code [{}]\n", code);
	  break;
	}
  }
  return false;
}

auto keys::createFormType(wchar_t const code) -> bool {
  switch (code) {
	case L'E': {
	  Instance->stateMap.reset(StateFlag::FORMIN);
	  thred::unmsg();
	  form::duform(FormStyles::kLine);
	  return true;
	}
	case L'F': {
	  Instance->stateMap.reset(StateFlag::FORMIN);
	  thred::unmsg();
	  form::duform(FormStyles::kFreehand);
	  return true;
	}
	case L'R': {
	  form::duform(FormStyles::kRegular);
	  return true;
	}
	case L'S': {
	  form::duform(FormStyles::kStar);
	  return true;
	}
	case L'A': {
	  form::duform(FormStyles::kSpiral);
	  return true;
	}
	case L'H': {
	  form::duform(FormStyles::kHeart);
	  return true;
	}
	case L'L': {
	  form::duform(FormStyles::kLens);
	  return true;
	}
	case L'G': {
	  form::duform(FormStyles::kEgg);
	  return true;
	}
	case L'T': {
	  form::duform(FormStyles::kTear);
	  return true;
	}
	case L'Z': {
	  form::duform(FormStyles::kZigzag);
	  return true;
	}
	case L'W': {
	  form::duform(FormStyles::kWave);
	  return true;
	}
	case L'D': {
	  form::duform(FormStyles::kDaisy);
	  return true;
	}
	default: {
	}
  }
  return false;
}

void keys::navigateThumbnails(wchar_t const code) {
  switch (code) {
	case VK_ESCAPE:
	case L'Q': {
	  thred::thumbQuit();
	  break;
	}
	case VK_DOWN:
	case VK_NEXT: { // page down
	  thred::nuthsel();
	  break;
	}
	case VK_UP:
	case VK_PRIOR: { // page up
	  thred::nuthbak(QUADRT + QUADRT);
	  break;
	}
	case VK_HOME: {
	  thred::thumbHome();
	  break;
	}
	case VK_END: {
	  thred::thumbEnd();
	  break;
	}
	case VK_BACK: // backspace
	case VK_LEFT: {
	  thred::thumbBack();
	  break;
	}
	default: {
	  TranslateMessage(&WinMsg);
	}
  }
}

auto keys::otherKeys(wchar_t const code) -> bool {
  switch (code) {
	case VK_DECIMAL:      // numpad period
	case VK_OEM_PERIOD: { // period
	  thred::otherPeriod();
	  return true;
	}
	case VK_BACK: { // backspace
	  thred::otherBack();
	  return true;
	}
	case VK_RETURN: {
	  thred::otherReturn();
	  return true;
	}
	default: {
	}
  }
  return false;
}
