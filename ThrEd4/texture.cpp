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
#include <boost/dynamic_bitset.hpp>
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
#include "switches.h"
#include "wrappers.h"

#include "Resources/resource.h"
#include "globals.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formForms.h"
#include "satin.h"
#include "thred.h"
#include "xt.h"
#include "texture.h"

namespace txi = texture::internal;

uint32_t               TextureWindowId;           // id of the window being updated
HWND                   SideWindowButton;          // button side window
RECT                   TexturePixelRect;          // screen selected texture points rectangle
TXTRCT                 TextureRect;               // selected texture points rectangle
POINT                  SelectTexturePointsOrigin; // original location of selected texture points
POINT                  TextureCursorLocation;     // texture editor move cursor location
HPEN                   TextureCrossPen;           // texture editor cross pen
TXHST                  TextureHistory[ITXBUFLEN]; // texture editor history headers
uint32_t               TextureHistoryIndex;       // pointer to the next texture history buffer
std::vector<TXPNT>*    TempTexturePoints;         // temporary storage for textured fill data
std::vector<uint32_t>* SelectedTexturePointsList; // list of selected points
TXTSCR                 TextureScreen;             // texture editor layout parameters

void texture::initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<uint32_t>* ptrTexturePointsList) noexcept {
  TempTexturePoints         = ptrTexturePoints;
  SelectedTexturePointsList = ptrTexturePointsList;
}

auto texture::internal::txnam(wchar_t* name, int32_t sizeName) -> bool {
  auto texturePath = fs::path(ArgList[0]);
  texturePath.replace_filename(L"thred.txr");
  return wcscpy_s(name, sizeName, texturePath.c_str()) == 0;
}

void texture::txdun() {
  auto signature            = std::array<char, 4> {"txh"};
  auto textureHistoryBuffer = std::vector<TXHSTBUF> {};
  textureHistoryBuffer.resize(ITXBUFLEN);
  if (!TextureHistory[0].texturePoints.empty()) {
	wchar_t name[_MAX_PATH] = {0};
	if (txi::txnam(static_cast<wchar_t*>(name), sizeof(name) / sizeof(name[0]))) {
	  auto  bytesWritten = DWORD {0};
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto handle =
	      CreateFile(static_cast<LPCWSTR>(name), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (handle != INVALID_HANDLE_VALUE) {
		WriteFile(handle, signature.data(), gsl::narrow<DWORD>(signature.size()), &bytesWritten, nullptr);
		WriteFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesWritten, nullptr);
		auto bufferIter = textureHistoryBuffer.begin();
		for (auto const& historyEntry : TextureHistory) {
		  auto& bufferEntry       = *bufferIter;
		  bufferEntry.placeholder = nullptr;
		  bufferEntry.count       = gsl::narrow<uint32_t>(historyEntry.texturePoints.size());
		  bufferEntry.height      = historyEntry.height;
		  bufferEntry.width       = historyEntry.width;
		  bufferEntry.spacing     = historyEntry.spacing;
		  bufferIter++;
		}
		wrap::WriteFile(handle,
		                textureHistoryBuffer.data(),
		                wrap::toUnsigned(textureHistoryBuffer.size() * ITXBUFLEN),
		                &bytesWritten,
		                nullptr);
		for (auto& item : TextureHistory) {
		  if (!item.texturePoints.empty()) {
			wrap::WriteFile(handle,
			                item.texturePoints.data(),
			                wrap::toUnsigned(item.texturePoints.size() *
			                                 sizeof(decltype(TextureHistory[0].texturePoints.back()))),
			                &bytesWritten,
			                nullptr);
		  }
		}
	  }
	  CloseHandle(handle);
	}
  }
}

void texture::internal::redtbak() {
  outDebugString(L"retrieving texture history {}\n", TextureHistoryIndex);
  auto const* textureHistoryItem = &TextureHistory[TextureHistoryIndex];
  if (textureHistoryItem != nullptr) {
	TextureScreen.areaHeight = textureHistoryItem->height;
	TextureScreen.width      = textureHistoryItem->width;
	TextureScreen.spacing    = textureHistoryItem->spacing;
	if (!textureHistoryItem->texturePoints.empty()) {
	  TempTexturePoints->clear();
	  TempTexturePoints->reserve(textureHistoryItem->texturePoints.size());
	  TempTexturePoints->insert(TempTexturePoints->begin(),
	                            textureHistoryItem->texturePoints.cbegin(),
	                            textureHistoryItem->texturePoints.cend());
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::redtx() {
  wchar_t name[_MAX_PATH]      = {0};
  auto    textureHistoryBuffer = std::vector<TXHSTBUF> {};
  textureHistoryBuffer.resize(ITXBUFLEN);
  TextureHistoryIndex = ITXBUFLEN - 1U;
  if (txi::txnam(static_cast<wchar_t*>(name), sizeof(name) / sizeof(name[0]))) {
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto handle = CreateFile(static_cast<LPCWSTR>(name), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (handle != INVALID_HANDLE_VALUE) {
	  auto bytesRead = DWORD {0};
	  auto sig       = std::array<char, 4> {0};
	  if (ReadFile(handle, sig.data(), gsl::narrow<DWORD>(sig.size()), &bytesRead, nullptr) != 0) {
		if (strcmp(sig.data(), "txh") == 0) {
		  if (ReadFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesRead, nullptr) != 0) {
			auto historyBytesRead = DWORD {0};
			if (wrap::ReadFile(handle,
			                   textureHistoryBuffer.data(),
			                   wrap::toUnsigned(textureHistoryBuffer.size() * ITXBUFLEN),
			                   &historyBytesRead,
			                   nullptr)) {
			  for (auto index = 0U;
			       index < (historyBytesRead / wrap::toUnsigned(sizeof(decltype(textureHistoryBuffer.back()))));
			       index++) {
				TextureHistory[index].height  = textureHistoryBuffer[index].height;
				TextureHistory[index].width   = textureHistoryBuffer[index].width;
				TextureHistory[index].spacing = textureHistoryBuffer[index].spacing;
				if (textureHistoryBuffer[index].count != 0U) {
				  TextureHistory[index].texturePoints.resize(textureHistoryBuffer[index].count);
				  if (!wrap::ReadFile(handle,
				                      TextureHistory[index].texturePoints.data(),
				                      sizeof(decltype(TextureHistory[0].texturePoints.back())) *
				                          textureHistoryBuffer[index].count,
				                      &bytesRead,
				                      nullptr)) {
					TextureHistory[index].texturePoints.clear();
					TextureHistory[index].texturePoints.shrink_to_fit();
				  }
				}
			  }
			}
		  }
		  StateMap->set(StateFlag::WASTXBAK);
		}
	  }
	}
	CloseHandle(handle);
  }
  txi::redtbak();
}

auto texture::istx(uint32_t iForm) noexcept -> bool {
  auto const& pfrm = FormList->operator[](iForm);
  if (pfrm.fillType == TXVRTF) {
	return true;
  }
  if (pfrm.fillType == TXHORF) {
	return true;
  }
  if (pfrm.fillType == TXANGF) {
	return true;
  }
  return false;
}

auto texture::istx(FRMHED const& form) noexcept -> bool {
  if (form.fillType == TXVRTF) {
	return true;
  }
  if (form.fillType == TXHORF) {
	return true;
  }
  if (form.fillType == TXANGF) {
	return true;
  }
  return false;
}

void texture::internal::txrfor() noexcept {
  if (TextureHistoryIndex < (ITXBUFLEN - 1U)) {
	TextureHistoryIndex++;
  }
  else {
	TextureHistoryIndex = 0;
  }
}

auto texture::internal::chktxh(_In_ TXHST const* historyItem) -> bool {
  if (historyItem != nullptr) {
	if (historyItem->texturePoints.size() != TempTexturePoints->size()) {
	  return true;
	}
	if (historyItem->height != TextureScreen.areaHeight) {
	  return true;
	}
	if (historyItem->spacing != TextureScreen.spacing) {
	  return true;
	}
	if (historyItem->width != TextureScreen.width) {
	  return true;
	}
	for (auto iPoint = 0U; iPoint < wrap::toUnsigned(TempTexturePoints->size()); iPoint++) {
	  if (TempTexturePoints->operator[](iPoint).line != historyItem->texturePoints[iPoint].line) {
		return true;
	  }
	  if (TempTexturePoints->operator[](iPoint).y != historyItem->texturePoints[iPoint].y) {
		return true;
	  }
	}
  }
  return false;
}

void texture::savtxt() {
  if (!TempTexturePoints->empty()) {
	auto* currentHistoryItem = &TextureHistory[TextureHistoryIndex];
	if (txi::chktxh(currentHistoryItem)) {
	  StateMap->set(StateFlag::WASTXBAK);
	  StateMap->reset(StateFlag::TXBDIR);
	  StateMap->reset(StateFlag::LASTXBAK);
	  txi::txrfor();
	  currentHistoryItem          = &TextureHistory[TextureHistoryIndex];
	  currentHistoryItem->height  = TextureScreen.areaHeight;
	  currentHistoryItem->width   = TextureScreen.width;
	  currentHistoryItem->spacing = TextureScreen.spacing;
	  currentHistoryItem->texturePoints.clear();
	  currentHistoryItem->texturePoints.insert(
	      currentHistoryItem->texturePoints.end(), TempTexturePoints->begin(), TempTexturePoints->end());
	}
  }
}

void texture::internal::txrbak() noexcept {
  if (TextureHistoryIndex > 0) {
	TextureHistoryIndex--;
  }
  else {
	TextureHistoryIndex = ITXBUFLEN - 1U;
  }
}

void texture::dutxtfil() {
  if (IniFile.textureHeight == 0.0F) {
	IniFile.textureHeight = ITXHI;
  }
  if (IniFile.textureWidth == 0.0F) {
	IniFile.textureWidth = ITXWID;
  }
  if (IniFile.textureSpacing == 0.0F) {
	IniFile.textureSpacing = ITXSPAC;
  }
  if (IniFile.textureEditorSize == 0U) {
	IniFile.textureEditorSize = ITXPIX;
  }
  StateMap->set(StateFlag::TXTRED);
  StateMap->set(StateFlag::ZUMED);
  StateMap->reset(StateFlag::WASPAT);
  StateMap->reset(StateFlag::RUNPAT);
  thred::movStch();
  ShowWindow(VerticalScrollBar, FALSE);
  ShowWindow(HorizontalScrollBar, FALSE);
  SelectedTexturePointsList->clear();
  StateMap->set(StateFlag::INIT);
  SideWindowButton = nullptr;
  if (StateMap->test(StateFlag::WASTXBAK)) {
	txi::redtbak();
	if (TextureScreen.areaHeight == 0.0F) {
	  TextureScreen.areaHeight = IniFile.textureHeight;
	}
	if (TextureScreen.spacing == 0.0F) {
	  TextureScreen.spacing = IniFile.underlaySpacing;
	}
	if (TextureScreen.width == 0.0F) {
	  TextureScreen.width = IniFile.textureWidth;
	}
	StateMap->set(StateFlag::LASTXBAK);
	StateMap->reset(StateFlag::TXBDIR);
  }
  else {
	TempTexturePoints->clear();
	TextureScreen.areaHeight = IniFile.textureHeight;
	TextureScreen.width      = IniFile.textureWidth;
	TextureScreen.spacing    = IniFile.textureSpacing;
  }
  StateMap->set(StateFlag::RESTCH);
}

void texture::internal::txt2pix(TXPNT const& texturePoint, POINT& screenPoint) {
  screenPoint.x = wrap::round<int32_t>(
      (gsl::narrow_cast<double>(TextureScreen.spacing) * texturePoint.line + TextureScreen.xOffset) /
      TextureScreen.editToPixelRatio);
  screenPoint.y = wrap::round<int32_t>(
      gsl::narrow_cast<double>(TextureScreen.height) -
      texturePoint.y / gsl::narrow_cast<double>(TextureScreen.areaHeight) * TextureScreen.height +
      TextureScreen.top);
}

void texture::internal::txtxfn(POINT const& reference, uint16_t offsetPixels) noexcept {
  POINT line[2] = {};
  line[0]       = POINT {reference.x, reference.y - offsetPixels};
  line[1]       = POINT {reference.x, reference.y + offsetPixels};
  Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
  line[0] = POINT {reference.x - offsetPixels, reference.y};
  line[1] = POINT {reference.x + offsetPixels, reference.y};
  Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
}

void texture::internal::dutxtx(uint32_t index, uint16_t offsetPixels) {
  auto ref = POINT {0L, 0L};
  txi::txt2pix(TempTexturePoints->operator[](index), ref);
  txi::txtxfn(ref, offsetPixels);
  if (ref.y > TextureScreen.halfHeight) {
	ref.y -= TextureScreen.height;
  }
  else {
	ref.y += TextureScreen.height;
  }
  txi::txtxfn(ref, offsetPixels);
}

void texture::internal::txrct2rct(TXTRCT const& textureRect, RECT& rectangle) {
  auto texturePoint = TXPNT {textureRect.top, textureRect.left};
  auto point        = POINT {0L, 0L};
  txi::txt2pix(texturePoint, point);
  rectangle.left    = point.x - IniFile.textureEditorSize;
  rectangle.top     = point.y - IniFile.textureEditorSize;
  texturePoint.y    = textureRect.bottom;
  texturePoint.line = textureRect.right;
  txi::txt2pix(texturePoint, point);
  rectangle.right  = point.x + IniFile.textureEditorSize;
  rectangle.bottom = point.y + IniFile.textureEditorSize;
}

void texture::internal::ed2px(fPOINT const& editPoint, POINT& point) {
  point.x = wrap::round<int32_t>(editPoint.x / TextureScreen.editToPixelRatio);
  point.y = wrap::round<int32_t>(StitchWindowClientRect.bottom - editPoint.y / TextureScreen.editToPixelRatio);
}

void texture::internal::px2ed(POINT const& point, fPOINT& editPoint) noexcept {
  editPoint.x = point.x * TextureScreen.editToPixelRatio;
  editPoint.y = TextureScreen.screenHeight - point.y * TextureScreen.editToPixelRatio;
}

void texture::internal::chktx() {
  TempTexturePoints->erase(std::remove_if(TempTexturePoints->begin(),
                                          TempTexturePoints->end(),
                                          [&](auto const& p) -> bool {
	                                        return !(p.line <= TextureScreen.lines &&
	                                                 p.y <= TextureScreen.areaHeight);
                                          }),
                           TempTexturePoints->end());
}

void texture::drwtxtr() {
  POINT      line[2] = {};
  auto const editSpace =
      TextureScreen.areaHeight * 2.0 / (TextureScreen.spacing * (TextureScreen.lines + 2.0));
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
  auto const pixelSpace = (StitchWindowClientRect.bottom * 1.0) / StitchWindowClientRect.right;
  TextureScreen.lines   = wrap::floor<uint16_t>(TextureScreen.width / TextureScreen.spacing);
  auto const extraWidth = TextureScreen.spacing * gsl::narrow_cast<float>(TextureScreen.lines + 2U);
  if (StateMap->testAndReset(StateFlag::CHKTX)) {
	txi::chktx();
  }
  auto yOffset = 0;
  if (pixelSpace > editSpace) {
	TextureScreen.xOffset          = 0.0F;
	TextureScreen.editToPixelRatio = extraWidth / StitchWindowClientRect.bottom;
	yOffset                        = wrap::round<decltype(yOffset)>(
        (StitchWindowClientRect.bottom - TextureScreen.areaHeight / TextureScreen.editToPixelRatio) / 2.0F);
  }
  else {
	TextureScreen.editToPixelRatio = TextureScreen.areaHeight * 2.0F / StitchWindowClientRect.bottom;
	yOffset                        = StitchWindowClientRect.bottom / 4;
	TextureScreen.xOffset =
	    (TextureScreen.editToPixelRatio * gsl::narrow_cast<float>(StitchWindowClientRect.right) -
	     TextureScreen.spacing * (gsl::narrow_cast<float>(TextureScreen.lines) + 2.0F)) /
	    2.0F;
  }
  TextureScreen.top          = yOffset;
  TextureScreen.bottom       = StitchWindowClientRect.bottom - yOffset;
  TextureScreen.height       = TextureScreen.bottom - TextureScreen.top;
  TextureScreen.halfHeight   = StitchWindowClientRect.bottom / 2;
  TextureScreen.screenHeight = StitchWindowClientRect.bottom * TextureScreen.editToPixelRatio;
  TextureScreen.yOffset      = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  SelectObject(StitchWindowMemDC, GridPen);
  auto gridLineCount = wrap::floor<uint32_t>(TextureScreen.areaHeight / IniFile.gridSize + 1.0F);
  auto textureRecord = TXPNT {};
  line[0].x          = 0;
  line[1].x          = StitchWindowClientRect.right;
  auto point         = POINT {0L, 0L};
  for (auto iGrid = 0U; iGrid < gridLineCount; iGrid++) {
	txi::txt2pix(textureRecord, point);
	line[0].y = line[1].y = point.y;
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
	textureRecord.y += IniFile.gridSize;
  }
  DeleteObject(TextureCrossPen);
  TextureCrossPen = wrap::CreatePen(PS_SOLID, 1, UserColor[ActiveColor]);
  SelectObject(StitchWindowMemDC, TextureCrossPen);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  line[0].y = 0;
  line[1].y = StitchWindowClientRect.bottom;
  for (auto iVertical = 1U; iVertical < TextureScreen.lines + 1U; iVertical++) {
	line[0].x = line[1].x = wrap::round<int32_t>(
	    (gsl::narrow_cast<double>(TextureScreen.spacing) * iVertical + TextureScreen.xOffset) /
	    TextureScreen.editToPixelRatio);
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
  }
  line[0].x = 0;
  line[1].x = StitchWindowClientRect.right;
  line[0].y = line[1].y = TextureScreen.top;
  Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
  line[0].y = line[1].y = TextureScreen.bottom;
  Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
  DeleteObject(TextureCrossPen);
  TextureCrossPen = wrap::CreatePen(PS_SOLID, 1, 0xffffff);
  SelectObject(StitchWindowMemDC, TextureCrossPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  for (auto index = 0U; index < wrap::toUnsigned(TempTexturePoints->size()); index++) {
	txi::dutxtx(index, IniFile.textureEditorSize);
  }
  if (!SelectedTexturePointsList->empty()) {
	txi::txrct2rct(TextureRect, TexturePixelRect);
	line[0] = {TexturePixelRect.left, TexturePixelRect.top};
	line[1] = {TexturePixelRect.right, TexturePixelRect.top};
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
	line[1] = {TexturePixelRect.left, TexturePixelRect.bottom};
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
	line[0] = {TexturePixelRect.right, TexturePixelRect.bottom};
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
	line[1] = {TexturePixelRect.right, TexturePixelRect.top};
	Polyline(StitchWindowMemDC, static_cast<POINT*>(line), 2);
  }
  for (auto selectedPoint : *SelectedTexturePointsList) {
	txi::dutxtx(selectedPoint, IniFile.textureEditorSize);
	txi::dutxtx(selectedPoint, IniFile.textureEditorSize * 2U);
  }
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
  displayText::drwtxbut(TextureScreen);
}

auto texture::internal::px2txt(POINT const& offset) -> bool {
  auto editPoint = fPOINT {};
  auto retval    = false;
  txi::px2ed(offset, editPoint);
  auto line = (editPoint.x - TextureScreen.xOffset) / TextureScreen.spacing;
  if (line < -0.5F) {
	line = 0.0F;
  }
  auto txPoint = TXPNT {0.0F, wrap::round<uint16_t>(line)};
  if ((txPoint.line <= TextureScreen.lines) && (txPoint.line >= 1)) {
	if (offset.y > TextureScreen.top) {
	  if ((offset.y <= TextureScreen.bottom)) {
		txPoint.y = TextureScreen.areaHeight - ((gsl::narrow_cast<float>(offset.y) - TextureScreen.top) /
		                                        TextureScreen.height * TextureScreen.areaHeight);
		TempTexturePoints->push_back(txPoint);
		retval = true;
	  }
	}
  }
  return retval;
}

void texture::internal::deorg(POINT& point) noexcept {
  point = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
}

auto texture::internal::txbutfn() -> bool {
  auto offset = POINT {0L, 0L};
  txi::deorg(offset);
  return txi::px2txt(offset);
}

void texture::rstxt() {
  SelectedTexturePointsList->clear();
  StateMap->reset(StateFlag::TXTMOV);
  StateMap->reset(StateFlag::BZUM);
  StateMap->reset(StateFlag::BZUMIN);
  StateMap->set(StateFlag::RESTCH);
  StateMap->reset(StateFlag::POLIMOV);
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  StateMap->set(StateFlag::RESTCH);
}

void texture::txtrbut() {
  if (txi::txbutfn()) {
	texture::savtxt();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	texture::rstxt();
  }
}

auto texture::internal::txtclos(uint32_t& closestTexturePoint) -> bool {
  if (closestTexturePoint != 0U) {
	auto minimumLength = 1e99;
	auto reference     = POINT {0L, 0L};
	auto point         = POINT {0L, 0L};
	txi::deorg(reference);
	closestTexturePoint = 0;
	for (auto iPoint = 0U; iPoint < wrap::toUnsigned(TempTexturePoints->size()); iPoint++) {
	  txi::txt2pix(TempTexturePoints->operator[](iPoint), point);
	  auto const length = hypot(point.x - reference.x, point.y - reference.y);
	  if (length < minimumLength) {
		minimumLength       = length;
		closestTexturePoint = iPoint;
	  }
	}
	if (minimumLength < CLOSENUF) {
	  return true;
	}
  }
  return false;
}

void texture::internal::setxmov() {
  StateMap->set(StateFlag::TXTMOV);
  TextureCursorLocation.x = SelectTexturePointsOrigin.x = Msg.pt.x - StitchWindowOrigin.x;
  TextureCursorLocation.y = SelectTexturePointsOrigin.y = Msg.pt.y - StitchWindowOrigin.y;
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void texture::internal::ritxrct() noexcept {
  auto const offset = POINT {(TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                             (TextureCursorLocation.y - SelectTexturePointsOrigin.y)};

  auto const rectangle = RECT {(TexturePixelRect.left + offset.x),
                               (TexturePixelRect.top + offset.y),
                               (TexturePixelRect.right + offset.x),
                               (TexturePixelRect.bottom + offset.y)};
  POINT      line[5]   = {};
  line[0].x = line[1].x = line[4].x = rectangle.left;
  line[2].x = line[3].x = rectangle.right;
  line[0].y = line[3].y = line[4].y = rectangle.top;
  line[1].y = line[2].y = rectangle.bottom;
  Polyline(StitchWindowDC, static_cast<POINT*>(line), 5);
}

void texture::internal::dutxrct(TXTRCT& textureRect) {
  if (!SelectedTexturePointsList->empty()) {
	auto* texturePoint = &TempTexturePoints->operator[](SelectedTexturePointsList->front());
	textureRect.left = textureRect.right = texturePoint->line;
	textureRect.top = textureRect.bottom = texturePoint->y;
	for (auto iPoint = 1U; iPoint < wrap::toUnsigned(SelectedTexturePointsList->size()); iPoint++) {
	  texturePoint = &TempTexturePoints->operator[](SelectedTexturePointsList->operator[](iPoint));
	  if (texturePoint->y > textureRect.top) {
		textureRect.top = texturePoint->y;
	  }
	  if (texturePoint->y < textureRect.bottom) {
		textureRect.bottom = texturePoint->y;
	  }
	  if (texturePoint->line < textureRect.left) {
		textureRect.left = texturePoint->line;
	  }
	  if (texturePoint->line > textureRect.right) {
		textureRect.right = texturePoint->line;
	  }
	}
  }
  else {
	textureRect.left = textureRect.right = 0U;
	textureRect.top = textureRect.bottom = 0.0F;
  }
}

auto texture::internal::ed2stch(fPOINT const& point) noexcept -> fPOINT {
  return {(point.x - TextureScreen.xOffset), (point.y - TextureScreen.yOffset)};
}

void texture::internal::dutxlin(fPOINT const& point0in, fPOINT const& point1in) {
  auto const point0 = txi::ed2stch(point0in);
  auto const point1 = txi::ed2stch(point1in);
  auto const deltaX = point1.x - point0.x;
  if (fabs(deltaX) < TINY) {
	return;
  }
  auto const slope  = (point1.y - point0.y) / deltaX;
  auto       start  = point0.x;
  auto       finish = point1.x;
  if (start > finish) {
	std::swap(start, finish);
  }
  auto integerStart  = wrap::ceil<int32_t>(start / TextureScreen.spacing);
  auto integerFinish = wrap::floor<int32_t>(finish / TextureScreen.spacing);
  if (integerStart < 1) {
	integerStart = 1;
  }
  if (integerFinish > TextureScreen.lines) {
	integerFinish = TextureScreen.lines;
  }
  auto const lineRange = integerFinish - integerStart;
  if (lineRange >= 0) {
	TempTexturePoints->reserve(TempTexturePoints->size() + lineRange);
	while (integerStart <= integerFinish) {
	  auto const yOffset = slope * (-point0.x + integerStart * TextureScreen.spacing) + point0.y;
	  if (yOffset > 0 && yOffset < TextureScreen.areaHeight) {
		TempTexturePoints->push_back({yOffset, gsl::narrow<uint16_t>(integerStart)});
	  }
	  integerStart++;
	}
  }
}

void texture::internal::setxclp(FRMHED const& form) {
  auto screenOffset = POINT {0L, 0L};
  auto editorOffset = fPOINT {};
  txi::deorg(screenOffset);
  txi::px2ed(screenOffset, editorOffset);
  if (StateMap->testAndReset(StateFlag::TXHCNTR)) {
	editorOffset.x = (gsl::narrow_cast<float>(TextureScreen.lines) * TextureScreen.spacing) / 2.0F +
	                 TextureScreen.xOffset - TextureScreen.formCenter.x + TextureScreen.spacing / 2.0F;
  }
  else {
	editorOffset.x -= TextureScreen.formCenter.x;
  }
  editorOffset.y -= TextureScreen.formCenter.y;
  auto& angledFormVertices = *AngledFormVertices;
  for (auto& vertex : angledFormVertices) {
	vertex.x += editorOffset.x;
	vertex.y += editorOffset.y;
  }
  auto lineCount = form.vertexCount - 1U;
  if (form.type != FRMLINE) {
	lineCount++;
  }
  for (auto iLine = 0U; iLine < lineCount; iLine++) {
	auto const iNextVertex = form::nxt(form, iLine);
	txi::dutxlin(angledFormVertices[iLine], angledFormVertices[iNextVertex]);
  }
}

void texture::internal::stxlin() {
  auto offset = POINT {0L, 0L};
  auto point0 = fPOINT {};
  auto point1 = fPOINT {};
  StateMap->reset(StateFlag::TXTMOV);
  txi::deorg(offset);
  txi::px2ed(offset, point1);
  txi::px2ed(FormLines->front(), point0);
  txi::dutxlin(point0, point1);
  StateMap->set(StateFlag::RESTCH);
}

void texture::internal::ed2txp(POINT const& offset, TXPNT& textureRecord) {
  auto point = fPOINT {};
  txi::px2ed(offset, point);
  auto val = (point.x - TextureScreen.xOffset) / TextureScreen.spacing;
  if (val < -0.5F) {
	val = 0.0F;
  }
  textureRecord.line = wrap::round<uint16_t>(val);
  textureRecord.y = TextureScreen.areaHeight - ((gsl::narrow_cast<float>(offset.y) - TextureScreen.top) /
                                                TextureScreen.height * TextureScreen.areaHeight);
}

void texture::txtrup() {
  auto offset = POINT {0L, 0L};
  if (StateMap->testAndReset(StateFlag::TXTMOV)) {
	texture::savtxt();
	txi::deorg(offset);
	offset.x -= SelectTexturePointsOrigin.x;
	offset.y -= SelectTexturePointsOrigin.y;
	auto const Xmagnitude = abs(offset.x);
	auto       textureOffset =
	    TXOFF {wrap::toFloat(-offset.y) / TextureScreen.height * TextureScreen.areaHeight,
	           wrap::ceil<int32_t>(Xmagnitude * TextureScreen.editToPixelRatio / TextureScreen.spacing)};
	if (offset.x < 0) {
	  textureOffset.line = -textureOffset.line;
	}
	auto yOffset = TextureRect.top + textureOffset.y - TextureScreen.areaHeight;
	if (yOffset > 0) {
	  textureOffset.y -= yOffset;
	}
	yOffset = TextureRect.bottom + textureOffset.y;
	if (yOffset < 0) {
	  textureOffset.y -= yOffset;
	}
	auto xCoord = TextureRect.left + textureOffset.line - 1;
	if (xCoord < 0) {
	  textureOffset.line -= xCoord;
	}
	xCoord = TextureRect.right + textureOffset.line - TextureScreen.lines;
	if (xCoord > 0) {
	  textureOffset.line -= xCoord;
	}
	for (auto point : *SelectedTexturePointsList) {
	  auto& texturePoint = TempTexturePoints->operator[](point);
	  texturePoint.line += textureOffset.line;
	  texturePoint.y += textureOffset.y;
	}
	txi::dutxrct(TextureRect);
  }
  else {
	if (StateMap->testAndReset(StateFlag::BZUMIN)) {
	  auto highestTexturePoint = TXPNT {};
	  auto lowestTexturePoint  = TXPNT {};
	  txi::deorg(offset);
	  txi::ed2txp(ZoomBoxLine[0], highestTexturePoint);
	  txi::ed2txp(offset, lowestTexturePoint);
	  if (highestTexturePoint.line < lowestTexturePoint.line) {
		std::swap(highestTexturePoint.line, lowestTexturePoint.line);
	  }
	  if (highestTexturePoint.y < lowestTexturePoint.y) {
		std::swap(highestTexturePoint.y, lowestTexturePoint.y);
	  }
	  SelectedTexturePointsList->clear();
	  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(TempTexturePoints->size()); iPoint++) {
		if (TempTexturePoints->operator[](iPoint).y < highestTexturePoint.y &&
		    TempTexturePoints->operator[](iPoint).y > lowestTexturePoint.y &&
		    TempTexturePoints->operator[](iPoint).line <= highestTexturePoint.line &&
		    TempTexturePoints->operator[](iPoint).line >= lowestTexturePoint.line) {
		  SelectedTexturePointsList->push_back(iPoint);
		}
	  }
	  txi::dutxrct(TextureRect);
	}
  }
  StateMap->set(StateFlag::RESTCH);
}

void texture::internal::angrct(fRECTANGLE& rectangle) noexcept {
  auto const& angledFormVertices = *AngledFormVertices;
  rectangle.left                 = angledFormVertices[0].x;
  rectangle.right                = angledFormVertices[0].x;
  rectangle.bottom               = angledFormVertices[0].y;
  rectangle.top                  = angledFormVertices[0].y;
  for (auto vertex : angledFormVertices) {
	if (vertex.x < rectangle.left) {
	  rectangle.left = vertex.x;
	}
	if (vertex.x > rectangle.right) {
	  rectangle.right = vertex.x;
	}
	if (vertex.y > rectangle.top) {
	  rectangle.top = vertex.y;
	}
	if (vertex.y < rectangle.bottom) {
	  rectangle.bottom = vertex.y;
	}
  }
}

void texture::internal::ritxfrm(FRMHED const& textureForm) {
  auto offset = POINT {(TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                       (TextureCursorLocation.y - SelectTexturePointsOrigin.y)};

  offset.x        = TextureCursorLocation.x - SelectTexturePointsOrigin.x;
  offset.y        = TextureCursorLocation.y - SelectTexturePointsOrigin.y;
  auto& formLines = *FormLines;
  formLines.resize(wrap::toSize(textureForm.vertexCount) + 1U);
  auto&      angledFormVertices = *AngledFormVertices;
  auto const maxVertex          = wrap::toUnsigned(angledFormVertices.size());
  for (auto iVertex = 0U; iVertex < maxVertex; iVertex++) {
	txi::ed2px(angledFormVertices[iVertex], formLines[iVertex]);
	formLines[iVertex].x += offset.x;
	formLines[iVertex].y += offset.y;
  }
  formLines[maxVertex] = formLines[0];
  auto vertexCount     = maxVertex;
  if (textureForm.type != FRMLINE) {
	vertexCount++;
  }
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowDC, formLines.data(), vertexCount);
}

void texture::internal::setxfrm() {
  auto angleRect = fRECTANGLE {};
  txi::angrct(angleRect);
  auto& angledFormVertices = *AngledFormVertices;
  for (auto& vertex : angledFormVertices) {
	vertex.x -= angleRect.left;
	vertex.y -= angleRect.bottom;
  }
  txi::angrct(angleRect);
  auto const height = angleRect.top - angleRect.bottom;
  if (height > TextureScreen.areaHeight) {
	auto const ratio = TextureScreen.areaHeight / height * 0.95;
	for (auto& vertex : angledFormVertices) {
	  vertex.x *= ratio;
	  vertex.y *= ratio;
	}
	txi::angrct(angleRect);
  }
  TextureScreen.formCenter.x = wrap::midl(angleRect.right, angleRect.left);
  TextureScreen.formCenter.y = wrap::midl(angleRect.top, angleRect.bottom);
  txi::ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
}

void texture::internal::txtclp(FRMHED& textureForm) {
  ThrEdClip  = RegisterClipboardFormat(ThrEdClipFormat);
  ClipMemory = GetClipboardData(ThrEdClip);
  if (ClipMemory != nullptr) {
	auto* clipFormHeader = gsl::narrow_cast<FORMCLIP*>(GlobalLock(ClipMemory));
	if (clipFormHeader != nullptr) {
	  if (clipFormHeader->clipType == CLP_FRM) {
		auto* clipForm = &clipFormHeader->form;
		if (nullptr != clipForm) {
		  textureForm         = *clipForm;
		  auto*      vertices = convert_ptr<fPOINT*>(&clipForm[1]);
		  auto const source   = gsl::span<fPOINT>(vertices, textureForm.vertexCount);
		  AngledFormVertices->clear();
		  AngledFormVertices->insert(AngledFormVertices->end(), source.begin(), source.end());
		  textureForm.vertexIndex = 0;
		  StateMap->reset(StateFlag::TXTLIN);
		  StateMap->set(StateFlag::TXTCLP);
		  StateMap->set(StateFlag::TXTMOV);
		  txi::setxfrm();
		  TextureCursorLocation = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
		}
	  }
	  GlobalUnlock(ClipMemory);
	}
  }
  StateMap->set(StateFlag::RESTCH);
  StateMap->reset(StateFlag::WASWROT);
}

void texture::internal::dutxtlin() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  Polyline(StitchWindowDC, FormLines->data(), 2);
}

void texture::txtrmov(FRMHED const& textureForm) {
  if (StateMap->test(StateFlag::TXTLIN)) {
	txi::dutxtlin();
	txi::deorg(FormLines->operator[](1));
	txi::dutxtlin();
	return;
  }
  if (StateMap->test(StateFlag::TXTCLP)) {
	if (StateMap->testAndSet(StateFlag::WASWROT)) {
	  txi::ritxfrm(textureForm);
	}
	TextureCursorLocation = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	txi::ritxfrm(textureForm);
  }
  else {
	if (!SelectedTexturePointsList->empty()) {
	  txi::ritxrct();
	  TextureCursorLocation = {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	  txi::ritxrct();
	}
  }
}

void texture::internal::txtlin() {
  auto& formLines = *FormLines;
  formLines.clear();
  formLines.resize(2);
  txi::deorg(formLines[0]);
  txi::deorg(formLines[1]);
  StateMap->reset(StateFlag::TXTCLP);
  StateMap->set(StateFlag::TXTLIN);
  StateMap->set(StateFlag::TXTMOV);
}

void texture::internal::chktxnum() {
  auto value = 0.0F;
  if (!TextureInputBuffer->empty()) {
	value = std::stof(*TextureInputBuffer);
  }
  if (value != 0.0F) {
	TextureInputBuffer->clear();
	value *= PFGRAN;
	switch (TextureWindowId) {
	  case HTXHI: {
		texture::savtxt();
		TextureScreen.areaHeight = value;
		IniFile.textureHeight    = value;
		StateMap->set(StateFlag::CHKTX);
		break;
	  }
	  case HTXWID: {
		texture::savtxt();
		TextureScreen.width  = value;
		IniFile.textureWidth = value;
		StateMap->set(StateFlag::CHKTX);
		break;
	  }
	  case HTXSPAC: {
		texture::savtxt();
		TextureScreen.spacing  = value;
		IniFile.textureSpacing = value;
		TextureScreen.width = value * gsl::narrow_cast<float>(TextureScreen.lines) + value / 2.0F;
		StateMap->set(StateFlag::CHKTX);
		break;
	  }
	  default: {
	  }
	}
  }
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  StateMap->set(StateFlag::RESTCH);
}

void texture::internal::butsid(uint32_t windowId) {
  auto buttonRect = RECT {0L, 0L, 0L, 0L};
  txi::chktxnum();
  TextureWindowId = windowId;
  GetWindowRect(ButtonWin->operator[](windowId), &buttonRect);
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  SideWindowButton = CreateWindow(L"STATIC",
                                  nullptr,
                                  SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                  buttonRect.left + ButtonWidthX3 - StitchWindowOrigin.x,
                                  buttonRect.top - StitchWindowOrigin.y,
                                  ButtonWidthX3,
                                  ButtonHeight,
                                  MainStitchWin,
                                  nullptr,
                                  ThrEdInstance,
                                  nullptr);
  displayText::updateWinFont(MainStitchWin);
}

auto texture::internal::tpComp(TXPNT const& texturePoint0, TXPNT const& texturePoint1) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (texturePoint0.line < texturePoint1.line) {
	return true;
  }
  if (texturePoint1.line < texturePoint0.line) {
	return false;
  }
  if (texturePoint0.y < texturePoint1.y) {
	return true;
  }
  if (texturePoint1.y < texturePoint0.y) {
	return false;
  }

  return false;
}

void texture::internal::txpar(FRMHED& form) {
  form.type                       = FRMFPOLY;
  form.fillInfo.texture.lines     = TextureScreen.lines;
  form.fillInfo.texture.height    = TextureScreen.areaHeight;
  form.fillSpacing                = TextureScreen.spacing;
  form.lengthOrCount.stitchLength = IniFile.userStitchLength;
  form.maxFillStitchLen           = IniFile.maxStitchLength;
  form.minFillStitchLen           = IniFile.minStitchLength;
  form.fillColor                  = gsl::narrow<uint8_t>(ActiveColor);
  form::refilfn();
}

void texture::internal::txvrt(FRMHED& form) {
  if (!TempTexturePoints->empty()) {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form.fillType = TXVRTF;
	  txi::txpar(form);
	}
  }
}

void texture::internal::txhor(FRMHED& form) {
  if (!TempTexturePoints->empty()) {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form.fillType = TXHORF;
	  txi::txpar(form);
	}
  }
}

void texture::internal::txang(FRMHED& form) {
  if (!TempTexturePoints->empty()) {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form.fillType              = TXANGF;
	  form.angleOrClipData.angle = gsl::narrow_cast<float>(IniFile.fillAngle);
	  txi::txpar(form);
	}
  }
}

void texture::deltx(uint32_t formIndex) {
  // clang-format off
  auto&       form         = FormList->operator[](formIndex);
  auto const& currentIndex = form.fillInfo.texture.index;
  // clang-format on
  if ((!TexturePointsBuffer->empty()) && texture::istx(formIndex) && (form.fillInfo.texture.count != 0U)) {
	auto flag = false;
	// First check to see if the texture is shared between forms
	for (auto iForm = 0U; iForm < formIndex; iForm++) {
	  if (texture::istx(iForm)) {
		auto& formIter = FormList->operator[](iForm);
		if (formIter.fillInfo.texture.index == currentIndex) {
		  flag = true;
		}
	  }
	}
	for (auto iForm = formIndex + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	  if (texture::istx(iForm)) {
		auto& formIter = FormList->operator[](iForm);
		if (formIter.fillInfo.texture.index == currentIndex) {
		  flag = true;
		}
	  }
	}
	// Only if it is not shared, should the texture be deleted
	if (!flag) {
	  auto textureBuffer = std::vector<TXPNT> {};
	  textureBuffer.reserve(TexturePointsBuffer->size());
	  auto iBuffer = 0U;
	  for (auto iForm = 0U; iForm < formIndex; iForm++) {
		if (texture::istx(iForm)) {
		  auto& fillInfo   = FormList->operator[](iForm).fillInfo;
		  auto startSource = std::next(TexturePointsBuffer->cbegin(), fillInfo.texture.index);
		  auto endSource   = std::next(startSource, fillInfo.texture.count);
		  textureBuffer.resize(textureBuffer.size() + fillInfo.texture.count);
		  auto destination = std::next(textureBuffer.begin(), iBuffer);
		  std::copy(startSource, endSource, destination);
		  fillInfo.texture.index = iBuffer;
		  iBuffer += fillInfo.texture.count;
		}
	  }
	  for (auto iForm = formIndex + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
		if (texture::istx(iForm)) {
		  auto& fillInfo   = FormList->operator[](iForm).fillInfo;
		  auto startSource = std::next(TexturePointsBuffer->cbegin(), fillInfo.texture.index);
		  auto endSource   = std::next(startSource, fillInfo.texture.count);
		  textureBuffer.resize(textureBuffer.size() + fillInfo.texture.count);
		  auto destination = std::next(textureBuffer.begin(), iBuffer);
		  std::copy(startSource, endSource, destination);
		  fillInfo.texture.index = iBuffer;
		  iBuffer += fillInfo.texture.count;
		}
	  }
	  *TexturePointsBuffer = std::move(textureBuffer);
	}
	form.fillType = 0;
  }
}

void texture::internal::nutx(FRMHED& form) {
  auto index = 0U;
  std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), txi::tpComp);
  if (!FormList->empty()) {
	if (texture::istx(ClosestFormToCursor)) {
	  auto const& texture = form.fillInfo.texture;
	  index               = texture.index;
	  texture::deltx(ClosestFormToCursor);
	}
	else {
	  for (auto iForm = ClosestFormToCursor; iForm-- > 0;) {
		if (texture::istx(iForm)) {
		  auto const& texture = FormList->operator[](iForm).fillInfo.texture;
		  index               = texture.index + texture.count;
		  break;
		}
	  }
	}
  }
  if (!TempTexturePoints->empty()) {
	auto const tempPointCount = wrap::toUnsigned(TempTexturePoints->size());
	auto       insertIt       = std::next(TexturePointsBuffer->begin(), index);
	TexturePointsBuffer->insert(insertIt, TempTexturePoints->cbegin(), TempTexturePoints->cend());
	for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); iForm++) {
	  if (texture::istx(iForm)) {
		FormList->operator[](iForm).fillInfo.texture.index += gsl::narrow<uint16_t>(tempPointCount);
	  }
	}
	form.fillInfo.texture.index = gsl::narrow<uint16_t>(index);
	form.fillInfo.texture.count = gsl::narrow<uint16_t>(tempPointCount);
  }
}

// Ensure all lines in the texture have at least 1 point
void texture::internal::altx() {
  auto txtLines = boost::dynamic_bitset<>(wrap::toSize(TextureScreen.lines) + 1U);
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto const halfHeight = TextureScreen.areaHeight / 2.0F;
	for (auto& texturePoint : *TempTexturePoints) {
	  txtLines.set(texturePoint.line);
	}
	for (uint16_t iLine = 1; iLine <= TextureScreen.lines; iLine++) {
	  if (!txtLines.test(iLine)) {
		TempTexturePoints->push_back({halfHeight, iLine});
	  }
	}
  }
}

void texture::txof() {
  displayText::butxt(HBOXSEL, StringTable->operator[](STR_BOXSEL));
  thred::redraw(ButtonWin->operator[](HHID));
  if (StateMap->test(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, StringTable->operator[](STR_UPON));
  }
  else {
	displayText::butxt(HUPTO, StringTable->operator[](STR_UPOF));
  }
  SetWindowText(ButtonWin->operator[](HTXSPAC), L"");
  texture::savtxt();
  thred::zumhom();
  SelectedTexturePointsList->clear();
  SelectedTexturePointsList->shrink_to_fit();
  StateMap->reset(StateFlag::TXTRED);
}

enum textureStyles {
  VRTYP,
  HORTYP,
  ANGTYP,
};

void texture::internal::dutxfn(uint32_t textureType) {
  if (StateMap->test(StateFlag::FORMSEL)) {
	txi::altx();
	auto& form = FormList->operator[](ClosestFormToCursor);
	clip::delmclp(ClosestFormToCursor);
	if (form.satinGuideCount != 0U) {
	  satin::delsac(ClosestFormToCursor);
	}
	texture::deltx(ClosestFormToCursor);
	texture::savtxt();
	txi::nutx(form);
	form::dusqr(FormList->operator[](ClosestFormToCursor));
	switch (textureType) {
	  case VRTYP: {
		txi::txvrt(form);
		break;
	  }
	  case HORTYP: {
		txi::txhor(form);
		break;
	  }
	  case ANGTYP: {
		txi::txang(form);
		break;
	  }
	  default: {
	  }
	}
  }
  texture::txof();
  texture::rstxt();
  if (StateMap->testAndReset(StateFlag::WASFRMFRM)) {
	formForms::refrm();
  }
}

void texture::internal::dutxmir() {
  if (!TempTexturePoints->empty()) {
	auto const centerLine = (TextureScreen.lines + 1U) / 2;
	auto const evenOffset = 1U - (TextureScreen.lines & 1U);
	texture::savtxt();
	std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), txi::tpComp);
	auto iPoint = wrap::toUnsigned(TempTexturePoints->size()) - 1U;
	while (TempTexturePoints->operator[](iPoint).line > centerLine) {
	  iPoint--;
	}
	TempTexturePoints->resize(wrap::toSize(iPoint) + 1U);
	auto const iMirrorPoint = iPoint + evenOffset;
	for (auto index = 0U; index < iMirrorPoint; index++) {
	  auto newLine =
	      gsl::narrow<uint16_t>(TextureScreen.lines - TempTexturePoints->operator[](index).line + 1U);
	  TempTexturePoints->emplace_back(TXPNT {TempTexturePoints->operator[](index).y, newLine});
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::internal::txdelal() {
  TextureInputBuffer->clear();
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  StateMap->set(StateFlag::RESTCH);
  texture::savtxt();
  TempTexturePoints->clear();
  texture::rstxt();
  StateMap->set(StateFlag::RESTCH);
}

auto texture::internal::chkbut() -> bool {
  if (Msg.hwnd == ButtonWin->operator[](HTXCLR)) {
	txi::txdelal();
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXHI)) {
	txi::butsid(HTXHI);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXWID)) {
	txi::butsid(HTXWID);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXSPAC)) {
	txi::butsid(HTXSPAC);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXVRT)) {
	txi::dutxfn(VRTYP);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXHOR)) {
	txi::dutxfn(HORTYP);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXANG)) {
	txi::dutxfn(ANGTYP);
	return true;
  }
  if (Msg.hwnd == ButtonWin->operator[](HTXMIR)) {
	txi::dutxmir();
	return true;
  }
  return false;
}

void texture::txtlbut(FRMHED const& textureForm) {
  if (txi::chkbut()) {
	return;
  }
  if (StateMap->testAndReset(StateFlag::TXTCLP)) {
	texture::savtxt();
	txi::setxclp(textureForm);
	return;
  }
  if (StateMap->testAndReset(StateFlag::TXTLIN)) {
	texture::savtxt();
	txi::stxlin();
	return;
  }
  if (!SelectedTexturePointsList->empty()) {
	auto point = POINT {0L, 0L};
	txi::deorg(point);
	if (point.x > TexturePixelRect.left && point.x < TexturePixelRect.right &&
	    point.y > TexturePixelRect.top && point.y < TexturePixelRect.bottom) {
	  txi::setxmov();
	  txi::ritxrct();
	  return;
	}
  }
  if (!SelectedTexturePointsList->empty()) {
	if (txi::txtclos(SelectedTexturePointsList->front())) {
	  SelectedTexturePointsList->resize(1);
	  txi::setxmov();
	  txi::dutxrct(TextureRect);
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
  }
  SelectedTexturePointsList->clear();
  StateMap->set(StateFlag::BZUMIN);
  StateMap->reset(StateFlag::BZUM);
  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
  ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1U;
}

void texture::internal::txbak() {
  if (StateMap->test(StateFlag::WASTXBAK)) {
	SelectedTexturePointsList->clear();
	auto flag = false;
	for (auto iHistory = 0U; iHistory < ITXBUFLEN; iHistory++) {
	  if (TextureHistory[TextureHistoryIndex].width != 0.0F) {
		flag = true;
		break;
	  }
	  txi::txrbak();
	}
	if (flag) {
	  txi::redtbak();
	  txi::txrbak();
	}
  }
}

void texture::internal::nxbak() {
  if (StateMap->test(StateFlag::WASTXBAK)) {
	auto flag = false;
	for (auto iHistory = 0U; iHistory < ITXBUFLEN; iHistory++) {
	  txi::txrfor();
	  if (TextureHistory[TextureHistoryIndex].width != 0.0F) {
		flag = true;
		break;
	  }
	}
	if (flag) {
	  txi::redtbak();
	}
  }
}

void texture::internal::txtdel() {
  if (!SelectedTexturePointsList->empty()) {
	texture::savtxt();
	auto texturePointsMap = boost::dynamic_bitset<>(TempTexturePoints->size());
	for (auto& p : *SelectedTexturePointsList) {
	  texturePointsMap.set(p);
	}
	auto index = 0U;
	TempTexturePoints->erase(remove_if(TempTexturePoints->begin(),
	                                   TempTexturePoints->end(),
	                                   [&index, texturePointsMap](auto point) {
	                                     std::ignore = point; // avoid unused parameter warning with code that optimizes out
	                                     return texturePointsMap.test(index++);
	                                   }),
	                         TempTexturePoints->end());
	SelectedTexturePointsList->clear();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  auto iClosestPoint = 0U;
  if (!TempTexturePoints->empty() && txi::txtclos(iClosestPoint)) {
	auto it = TempTexturePoints->cbegin();
	std::advance(it, iClosestPoint);
	it = TempTexturePoints->erase(it);
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::internal::txcntrv(FRMHED const& textureForm) {
  if (StateMap->testAndReset(StateFlag::TXTCLP)) {
	StateMap->set(StateFlag::TXHCNTR);
	texture::savtxt();
	txi::setxclp(textureForm);
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::internal::txsiz(float ratio, FRMHED const& textureForm) {
  txi::ritxfrm(textureForm);
  auto& angledFormVertices = *AngledFormVertices;
  for (auto& vertex : angledFormVertices) {
	vertex.x *= ratio;
	vertex.y *= ratio;
  }
  auto angleRect = fRECTANGLE {};
  txi::angrct(angleRect);
  TextureScreen.formCenter.x = wrap::midl(angleRect.right, angleRect.left);
  TextureScreen.formCenter.y = wrap::midl(angleRect.top, angleRect.bottom);
  txi::ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
  txi::ritxfrm(textureForm);
}

void texture::internal::txshrnk(FRMHED const& textureForm) {
  txi::txsiz(TXTRAT, textureForm);
}

void texture::internal::txgro(FRMHED const& textureForm) {
  txi::txsiz(1.0F / TXTRAT, textureForm);
}

auto texture::internal::txdig(uint32_t keyCode, char& character) -> bool {
  if (isdigit(keyCode) != 0) {
	character = gsl::narrow<char>(keyCode);
	return true;
  }
  if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
	character = gsl::narrow<char>(keyCode - VK_NUMPAD0 + 0x30);
	return true;
  }
  if (keyCode == 0xbe || keyCode == 0x6e) {
	character = '.';
	return true;
  }
  return false;
}

void texture::internal::txnudg(int32_t deltaX, float deltaY) {
  if (!SelectedTexturePointsList->empty()) {
	if (deltaY != 0.0F) {
	  auto const screenDeltaY = deltaY * TextureScreen.editToPixelRatio;
	  for (auto point : *SelectedTexturePointsList) {
		auto const yCoord = TempTexturePoints->operator[](point).y + screenDeltaY;
		if (yCoord < 0.0) {
		  return;
		}
		if (yCoord > TextureScreen.areaHeight) {
		  return;
		}
	  }
	  for (auto point : *SelectedTexturePointsList) {
		TempTexturePoints->operator[](point).y += screenDeltaY;
	  }
	}
	else {
	  for (auto point : *SelectedTexturePointsList) {
		auto const textureLine = TempTexturePoints->operator[](point).line + deltaX;
		if (textureLine < 1) {
		  return;
		}
		if (textureLine > TextureScreen.lines) {
		  return;
		}
	  }
	  for (auto const point : *SelectedTexturePointsList) {
		TempTexturePoints->operator[](point).line += gsl::narrow<uint16_t>(deltaX);
	  }
	}
  }
  txi::dutxrct(TextureRect);
  StateMap->set(StateFlag::RESTCH);
}

void texture::txsnap() {
  auto const txpntSize = TempTexturePoints->size();
  if (txpntSize != 0U) {
	texture::savtxt();
	auto const halfGrid = IniFile.gridSize / 2;
	if (!SelectedTexturePointsList->empty()) {
	  for (auto iPoint = 0U; iPoint < txpntSize; iPoint++) {
		auto& texturePoint = TempTexturePoints->operator[](SelectedTexturePointsList->operator[](iPoint));
		auto const yStep = (texturePoint.y + halfGrid) / IniFile.gridSize;
		texturePoint.y   = yStep * IniFile.gridSize;
	  }
	}
	else {
	  for (auto iPoint = 0U; iPoint < txpntSize; iPoint++) {
		auto& texturePoint = TempTexturePoints->operator[](iPoint);
		auto const yStep   = (texturePoint.y + halfGrid) / IniFile.gridSize;
		texturePoint.y     = yStep * IniFile.gridSize;
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::txtkey(uint32_t keyCode, FRMHED& textureForm) {
  auto character = char {};
  if (SideWindowButton != nullptr) {
	auto flag = true;
	switch (keyCode) {
	  case VK_RETURN: {
		txi::chktxnum();
		return;
	  }
	  case VK_ESCAPE:
	  case 'Q': {
		texture::rstxt();
		StateMap->set(StateFlag::RESTCH);
		break;
	  }
	  case 8: // backspace
	  {
		if (!TextureInputBuffer->empty()) {
		  TextureInputBuffer->pop_back();
		}
		flag = false;
		break;
	  }
	  default: {
	  }
	}
	if (flag) {
	  if (TextureInputBuffer->size() < 8U) { // floating point 7 digits of precision + '.'
		if (txi::txdig(keyCode, character)) {
		  TextureInputBuffer->push_back(character);
		}
	  }
	}
	SetWindowText(SideWindowButton, TextureInputBuffer->c_str());
	return;
  }
  switch (keyCode) {
	case VK_ESCAPE: {
	  texture::txof();
	}
	case 'Q': {
	  texture::rstxt();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	case 0xdb: { //[
	  txi::txshrnk(textureForm);
	  break;
	}
	case 0xdd: { //]
	  txi::txgro(textureForm);
	  break;
	}
	case 192: { //`
	  xt::tst();
	  break;
	}
	case 'R': {
	  txi::dutxfn(VRTYP);
	  break;
	}
	case 'A': {
	  txi::dutxfn(ANGTYP);
	  break;
	}
	case 'H': {
	  txi::dutxfn(HORTYP);
	  break;
	}
	case 'E': {
	  txi::txtlin();
	  break;
	}
	case 'Z':
	case 'B': {
	  if (!StateMap->testAndSet(StateFlag::LASTXBAK)) {
		texture::savtxt();
		txi::txrbak();
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::TXBDIR)) {
		  txi::txrbak();
		}
	  }
	  txi::txbak();
	  return;
	}
	case 'V': {
	  if (OpenClipboard(ThrEdWindow) != 0) {
		txi::txtclp(textureForm);
	  }
	  break;
	}
	case 'N': {
	  StateMap->set(StateFlag::LASTXBAK);
	  if (!StateMap->testAndSet(StateFlag::TXBDIR)) {
		txi::txrfor();
	  }
	  txi::nxbak();
	  return;
	}
	case 'D':
	case VK_DELETE: {
	  if (wrap::pressed(VK_SHIFT) && wrap::pressed(VK_CONTROL)) {
		txi::txdelal();
	  }
	  else {
		txi::txtdel();
	  }
	  break;
	}
	case 0xbd: { //-
	  txi::txcntrv(textureForm);
	  break;
	}
	case VK_LEFT: {
	  txi::txnudg(-1, 0);
	  break;
	}
	case VK_RIGHT: {
	  txi::txnudg(1, 0);
	  break;
	}
	case VK_UP: {
	  txi::txnudg(0, IniFile.cursorNudgeStep);
	  break;
	}
	case VK_DOWN: {
	  txi::txnudg(0, -IniFile.cursorNudgeStep);
	  break;
	}
	case 'S': {
	  texture::txsnap();
	  break;
	}
	default: {
	}
  }
  StateMap->reset(StateFlag::LASTXBAK);
}

void texture::setxt(FRMHED& form, std::vector<RNGCNT>& textureSegments) {
  auto const currentIndex = form.fillInfo.texture.index;
  auto const currentCount = form.fillInfo.texture.count;
  texture::savtxt();
  form.wordParam = 0;
  StateMap->set(StateFlag::TXFIL);
  ClipRectSize.cx = form.fillSpacing;
  ClipRectSize.cy = form.fillInfo.texture.height;
  if (currentCount != 0U) {
	for (auto iTexturePoint = currentCount - 1; iTexturePoint >= 0; iTexturePoint--) {
	  auto const currentPoint = TexturePointsBuffer->at(wrap::toSize(currentIndex) + iTexturePoint);
	  if (currentPoint.line != 0U) {
		auto const iSegment            = currentPoint.line - 1U;
		textureSegments[iSegment].line = iTexturePoint;
		textureSegments[iSegment].stitchCount++;
	  }
	}
  }
}

void texture::rtrtx(FRMHED const& form) {
  TempTexturePoints->clear();
  auto const currentIndex = form.fillInfo.texture.index;
  if (TexturePointsBuffer->size() > gsl::narrow<uint32_t>(currentIndex)) {
	auto currentCount = form.fillInfo.texture.count;
	if (TexturePointsBuffer->size() < gsl::narrow<uint32_t>(currentIndex + currentCount)) {
	  currentCount = gsl::narrow<uint16_t>(TexturePointsBuffer->size()) - currentIndex;
	}
	TempTexturePoints->resize(currentCount);
	auto startSource = std::next(TexturePointsBuffer->cbegin(), currentIndex);
	auto endSource   = std::next(startSource, currentCount);
	std::copy(startSource, endSource, TempTexturePoints->begin());
	TextureScreen.areaHeight = form.fillInfo.texture.height;
	TextureScreen.spacing    = form.fillSpacing;
	TextureScreen.lines      = form.fillInfo.texture.lines;
	TextureScreen.width = gsl::narrow_cast<float>(TextureScreen.lines) * TextureScreen.spacing +
	                      TextureScreen.spacing / 2.0F;
	texture::savtxt();
  }
}

auto texture::internal::inrct(fRECTANGLE const& rectangle, fPOINTATTR const& stitch) noexcept -> bool {
  if (stitch.x < rectangle.left) {
	return false;
  }
  if (stitch.x > rectangle.right) {
	return false;
  }
  if (stitch.y < rectangle.bottom) {
	return false;
  }
  if (stitch.y > rectangle.top) {
	return false;
  }
  return true;
}

void texture::setshft() {
  if (ZoomBoxLine[2].x != 0L && ZoomBoxLine[2].y != 0L) {
	auto selectionRect = fRECTANGLE {};
	texture::savtxt();
	thred::unbBox();
	StateMap->reset(StateFlag::BZUMIN);
	selectionRect.top      = ZoomBoxOrigin.y;
	selectionRect.left     = ZoomBoxOrigin.x;
	auto const stitchPoint = thred::pxCor2stch(
	    POINT {ZoomBoxLine[2].x + StitchWindowOrigin.x, ZoomBoxLine[2].y + StitchWindowOrigin.y});
	selectionRect.bottom = stitchPoint.y;
	selectionRect.right  = stitchPoint.x;
	StateMap->reset(StateFlag::TXIN);
	TempTexturePoints->clear();
	auto line = 1U;
	for (auto& stitch : *StitchBuffer) {
	  if (txi::inrct(selectionRect, stitch)) {
		StateMap->set(StateFlag::TXIN);
		TempTexturePoints->push_back(TXPNT {(stitch.y - selectionRect.bottom), gsl::narrow<uint16_t>(line)});
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::TXIN)) {
		  line++;
		}
	  }
	}
	if (!TempTexturePoints->empty()) {
	  line = TempTexturePoints->back().line;
	}
	if (IniFile.textureEditorSize == 0U) {
	  IniFile.textureEditorSize = ITXPIX;
	}
	TextureScreen.spacing    = (selectionRect.right - selectionRect.left) / line;
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width      = TextureScreen.spacing * line + TextureScreen.spacing / 2;
	StateMap->set(StateFlag::TXTRED);
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::writeScreenWidth(uint32_t position) {
  auto fmtStr = std::wstring {};
  displayText::loadString(fmtStr, IDS_TXWID);
  auto scrWidth = std::wstring(fmt::format(fmtStr, (TextureScreen.width / PFGRAN)));
  wrap::TextOut(DrawItem->hDC, position, 1, scrWidth.c_str(), wrap::toUnsigned(scrWidth.size()));
}
