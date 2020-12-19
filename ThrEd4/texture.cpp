// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
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
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127)  // supress warning for fmt library header
#pragma warning(disable : 6387)  // supress warning for fmt library header
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#include <fmt/format.h>
#pragma warning(pop)

// Local Headers
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

constexpr auto OSCLAMP = -0.5F; // values below this are off screen and should be clamped

static auto TextureWindowId           = uint32_t {}; // id of the window being updated
static auto SideWindowButton          = HWND {};     // button side window
static auto TexturePixelRect          = RECT {};     // screen selected texture points rectangle
static auto TextureRect               = TXTRCT {};   // selected texture points rectangle
static auto SelectTexturePointsOrigin = POINT {};    // original location of selected texture points
static auto TextureCursorLocation     = POINT {};    // texture editor move cursor location
static auto TextureCrossPen           = HPEN {};     // texture editor cross pen
static auto TextureHistory = gsl::narrow_cast<std::vector<TXHST>*>(nullptr); // texture editor history headers
static auto TextureHistoryIndex = uint32_t {}; // pointer to the next texture history buffer
static auto TempTexturePoints   = static_cast<std::vector<TXPNT>*>(nullptr); // temporary storage for textured fill data
static auto SelectedTexturePointsList = static_cast<std::vector<uint32_t>*>(nullptr); // list of selected points
static auto TextureScreen = TXTSCR {}; // texture editor layout parameters

void texture::initTextures(std::vector<TXPNT>*    ptrTexturePoints,
                           std::vector<uint32_t>* ptrTexturePointsList,
                           std::vector<TXHST>*    ptrTextureHistory) noexcept {
  TempTexturePoints         = ptrTexturePoints;
  SelectedTexturePointsList = ptrTexturePointsList;
  TextureHistory            = ptrTextureHistory;
}

auto texture::internal::txnam(std::wstring& name) -> bool {
  auto const* texturePath = thred::getHomeDir();
  if (nullptr != texturePath) {
	auto const textureFile = *texturePath / L"thred.txr";
	name.assign(textureFile.generic_wstring());
	return true;
  }
  return false;
}

void texture::txdun() {
  auto signature            = std::array<char, 4> {"txh"};
  auto textureHistoryBuffer = std::vector<TXHSTBUF> {};
  textureHistoryBuffer.resize(ITXBUFSZ);
  if (!TextureHistory->operator[](0).texturePoints.empty()) {
	auto name = std::wstring {};
	if (txi::txnam(name)) {
	  auto bytesWritten = DWORD {};
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const handle = CreateFile(name.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (handle != INVALID_HANDLE_VALUE) {
		WriteFile(handle, signature.data(), wrap::toUnsigned(signature.size()), &bytesWritten, nullptr);
		WriteFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesWritten, nullptr);
		auto bufferIter = textureHistoryBuffer.begin();
		for (auto const& historyEntry : *TextureHistory) {
		  auto& bufferEntry       = *bufferIter;
		  bufferEntry.placeholder = nullptr;
		  bufferEntry.count       = wrap::toUnsigned(historyEntry.texturePoints.size());
		  bufferEntry.height      = historyEntry.height;
		  bufferEntry.width       = historyEntry.width;
		  bufferEntry.spacing     = historyEntry.spacing;
		  ++bufferIter;
		}
		wrap::WriteFile(handle,
		                textureHistoryBuffer.data(),
		                wrap::toUnsigned(textureHistoryBuffer.size() * ITXBUFSZ),
		                &bytesWritten,
		                nullptr);
		for (auto& item : *TextureHistory) {
		  if (!item.texturePoints.empty()) {
			wrap::WriteFile(
			    handle,
			    item.texturePoints.data(),
			    wrap::toUnsigned(item.texturePoints.size() *
			                     sizeof(decltype(TextureHistory->operator[](0).texturePoints.back()))),
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
  // NOLINTNEXTLINE
  outDebugString(L"retrieving texture history {}\n", TextureHistoryIndex);
  auto const& textureHistoryItem = TextureHistory->operator[](TextureHistoryIndex);
  TextureScreen.areaHeight       = textureHistoryItem.height;
  TextureScreen.width            = textureHistoryItem.width;
  TextureScreen.spacing          = textureHistoryItem.spacing;
  if (!textureHistoryItem.texturePoints.empty()) {
	TempTexturePoints->clear();
	TempTexturePoints->reserve(textureHistoryItem.texturePoints.size());
	TempTexturePoints->insert(TempTexturePoints->begin(),
	                          textureHistoryItem.texturePoints.cbegin(),
	                          textureHistoryItem.texturePoints.cend());
  }
  StateMap->set(StateFlag::RESTCH);
}

void texture::redtx() {
  auto name                 = std::wstring {};
  auto textureHistoryBuffer = std::vector<TXHSTBUF> {};
  textureHistoryBuffer.resize(ITXBUFSZ);
  TextureHistoryIndex = ITXBUFSZ - 1U;
  if (txi::txnam(name)) {
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const handle = CreateFile(name.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (handle != INVALID_HANDLE_VALUE) {
	  auto bytesRead = DWORD {};
	  auto sig       = std::array<char, 4> {};
	  if (ReadFile(handle, sig.data(), wrap::toUnsigned(sig.size()), &bytesRead, nullptr) != 0) {
		if (strcmp(sig.data(), "txh") == 0) {
		  if (ReadFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesRead, nullptr) != 0) {
			auto historyBytesRead = DWORD {};
			if (wrap::ReadFile(handle,
			                   textureHistoryBuffer.data(),
			                   wrap::toUnsigned(textureHistoryBuffer.size() * ITXBUFSZ),
			                   &historyBytesRead,
			                   nullptr)) {
			  for (auto index = 0U;
			       index < (historyBytesRead / wrap::toUnsigned(sizeof(decltype(textureHistoryBuffer.back()))));
			       ++index) {
				TextureHistory->operator[](index).height  = textureHistoryBuffer[index].height;
				TextureHistory->operator[](index).width   = textureHistoryBuffer[index].width;
				TextureHistory->operator[](index).spacing = textureHistoryBuffer[index].spacing;
				if (textureHistoryBuffer[index].count != 0U) {
				  TextureHistory->operator[](index).texturePoints.resize(textureHistoryBuffer[index].count);
				  if (!wrap::ReadFile(handle,
				                      TextureHistory->operator[](index).texturePoints.data(),
				                      sizeof(decltype(TextureHistory->operator[](0).texturePoints.back())) *
				                          textureHistoryBuffer[index].count,
				                      &bytesRead,
				                      nullptr)) {
					TextureHistory->operator[](index).texturePoints.clear();
					TextureHistory->operator[](index).texturePoints.shrink_to_fit();
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
  if (TextureHistoryIndex < (ITXBUFSZ - 1U)) {
	++TextureHistoryIndex;
  }
  else {
	TextureHistoryIndex = 0;
  }
}

auto texture::internal::chktxh(_In_ TXHST const& historyItem) -> bool {
  if (historyItem.texturePoints.size() != TempTexturePoints->size()) {
	return true;
  }
  if (historyItem.height != TextureScreen.areaHeight) {
	return true;
  }
  if (historyItem.spacing != TextureScreen.spacing) {
	return true;
  }
  if (historyItem.width != TextureScreen.width) {
	return true;
  }
  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(TempTexturePoints->size()); ++iPoint) {
	if (TempTexturePoints->operator[](iPoint).line != historyItem.texturePoints[iPoint].line) {
	  return true;
	}
	if (TempTexturePoints->operator[](iPoint).y != historyItem.texturePoints[iPoint].y) {
	  return true;
	}
  }
  return false;
}

void texture::savtxt() {
  if (!TempTexturePoints->empty()) {
	auto& currentHistoryItem = TextureHistory->operator[](TextureHistoryIndex);
	if (txi::chktxh(currentHistoryItem)) {
	  StateMap->set(StateFlag::WASTXBAK);
	  StateMap->reset(StateFlag::TXBDIR);
	  StateMap->reset(StateFlag::LASTXBAK);
	  txi::txrfor();
	  auto& currentItem   = TextureHistory->operator[](TextureHistoryIndex);
	  currentItem.height  = TextureScreen.areaHeight;
	  currentItem.width   = TextureScreen.width;
	  currentItem.spacing = TextureScreen.spacing;
	  currentItem.texturePoints.clear();
	  currentItem.texturePoints.insert(
	      currentItem.texturePoints.end(), TempTexturePoints->begin(), TempTexturePoints->end());
	}
  }
}

void texture::internal::txrbak() noexcept {
  if (TextureHistoryIndex > 0) {
	--TextureHistoryIndex;
  }
  else {
	TextureHistoryIndex = ITXBUFSZ - 1U;
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
  ShowWindow(VerticalScrollBar, SW_HIDE);
  ShowWindow(HorizontalScrollBar, SW_HIDE);
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

void texture::internal::txt2pix(TXPNT const& texturePoint, POINT& screenPoint) noexcept {
  screenPoint.x =
      std::lround(((TextureScreen.spacing) * wrap::toFloat(texturePoint.line) + TextureScreen.xOffset) /
                  TextureScreen.editToPixelRatio);
  screenPoint.y =
      std::lround(wrap::toFloat(TextureScreen.height) -
                  texturePoint.y / TextureScreen.areaHeight * wrap::toFloat(TextureScreen.height) +
                  wrap::toFloat(TextureScreen.top));
}

void texture::internal::txtxfn(POINT const& reference, uint16_t offsetPixels) {
  auto line = std::array<POINT, 2> {};
  line[0]   = POINT {reference.x, reference.y - offsetPixels};
  line[1]   = POINT {reference.x, reference.y + offsetPixels};
  wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  line[0] = POINT {reference.x - offsetPixels, reference.y};
  line[1] = POINT {reference.x + offsetPixels, reference.y};
  wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
}

void texture::internal::dutxtx(uint32_t index, uint16_t offsetPixels) {
  auto ref = POINT {};
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

void texture::internal::txrct2rct(TXTRCT const& textureRect, RECT& rectangle) noexcept {
  auto texturePoint = TXPNT {textureRect.top, textureRect.left};
  auto point        = POINT {};
  txi::txt2pix(texturePoint, point);
  rectangle.left    = point.x - IniFile.textureEditorSize;
  rectangle.top     = point.y - IniFile.textureEditorSize;
  texturePoint.y    = textureRect.bottom;
  texturePoint.line = textureRect.right;
  txi::txt2pix(texturePoint, point);
  rectangle.right  = point.x + IniFile.textureEditorSize;
  rectangle.bottom = point.y + IniFile.textureEditorSize;
}

void texture::internal::ed2px(fPOINT const& editPoint, POINT& point) noexcept {
  point.x = std::lround(editPoint.x / TextureScreen.editToPixelRatio);
  point.y = std::lround(wrap::toFloat(StitchWindowClientRect.bottom) - editPoint.y / TextureScreen.editToPixelRatio);
}

void texture::internal::px2ed(POINT const& point, fPOINT& editPoint) noexcept {
  editPoint.x = wrap::toFloat(point.x) * TextureScreen.editToPixelRatio;
  editPoint.y = TextureScreen.screenHeight - wrap::toFloat(point.y) * TextureScreen.editToPixelRatio;
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
  auto       line      = std::array<POINT, LNPNTS> {};
  auto const editSpace = TextureScreen.areaHeight * 2.0F /
                         (TextureScreen.spacing * (wrap::toFloat(TextureScreen.lines) + 2.0F));
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, thred::getBackGroundBrush());
  auto const pixelSpace =
      wrap::toFloat(StitchWindowClientRect.bottom) / wrap::toFloat(StitchWindowClientRect.right);
  TextureScreen.lines   = wrap::floor<uint16_t>(TextureScreen.width / TextureScreen.spacing);
  auto const extraWidth = TextureScreen.spacing * wrap::toFloat(TextureScreen.lines + 2U);
  if (StateMap->testAndReset(StateFlag::CHKTX)) {
	txi::chktx();
  }
  auto const flag = (pixelSpace > editSpace);
  TextureScreen.editToPixelRatio =
      flag ? extraWidth / wrap::toFloat(StitchWindowClientRect.bottom)
           : TextureScreen.areaHeight * 2.0F / wrap::toFloat(StitchWindowClientRect.bottom);
  TextureScreen.xOffset =
      flag ? 0.0F
           : (TextureScreen.editToPixelRatio * wrap::toFloat(StitchWindowClientRect.right) -
              TextureScreen.spacing * (wrap::toFloat(TextureScreen.lines) + 2.0F)) /
                 2.0F;
  auto const yOffset = flag ? wrap::round<LONG>((wrap::toFloat(StitchWindowClientRect.bottom) -
                                                 TextureScreen.areaHeight / TextureScreen.editToPixelRatio) /
                                                2.0F)
                            : StitchWindowClientRect.bottom / 4;
  TextureScreen.top        = yOffset;
  TextureScreen.bottom     = StitchWindowClientRect.bottom - yOffset;
  TextureScreen.height     = TextureScreen.bottom - TextureScreen.top;
  TextureScreen.halfHeight = StitchWindowClientRect.bottom / 2;
  TextureScreen.screenHeight = wrap::toFloat(StitchWindowClientRect.bottom) * TextureScreen.editToPixelRatio;
  TextureScreen.yOffset = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  SelectObject(StitchWindowMemDC, GridPen);
  auto const gridLineCount = wrap::floor<uint32_t>(TextureScreen.areaHeight / IniFile.gridSize + 1.0F);
  auto       textureRecord = TXPNT {};
  line[0].x                = 0;
  line[1].x                = StitchWindowClientRect.right;
  auto point               = POINT {};
  for (auto iGrid = 0U; iGrid < gridLineCount; ++iGrid) {
	txi::txt2pix(textureRecord, point);
	line[0].y = line[1].y = point.y;
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	textureRecord.y += IniFile.gridSize;
  }
  DeleteObject(TextureCrossPen);
  auto const spUserColor = gsl::span<COLORREF> {UserColor};
  TextureCrossPen        = wrap::CreatePen(PS_SOLID, PENNWID, spUserColor[ActiveColor]);
  SelectObject(StitchWindowMemDC, TextureCrossPen);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  line[0].y = 0;
  line[1].y = StitchWindowClientRect.bottom;
  for (auto iVertical = 1U; iVertical < TextureScreen.lines + 1U; ++iVertical) {
	line[0].x = line[1].x =
	    std::lroundf((TextureScreen.spacing * wrap::toFloat(iVertical) + TextureScreen.xOffset) /
	                 TextureScreen.editToPixelRatio);
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
  line[0].x = 0;
  line[1].x = StitchWindowClientRect.right;
  line[0].y = line[1].y = TextureScreen.top;
  wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  line[0].y = line[1].y = TextureScreen.bottom;
  wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  DeleteObject(TextureCrossPen);
  TextureCrossPen = wrap::CreatePen(PS_SOLID, PENNWID, PENWHITE);
  SelectObject(StitchWindowMemDC, TextureCrossPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  for (auto index = 0U; index < wrap::toUnsigned(TempTexturePoints->size()); ++index) {
	txi::dutxtx(index, IniFile.textureEditorSize);
  }
  if (!SelectedTexturePointsList->empty()) {
	txi::txrct2rct(TextureRect, TexturePixelRect);
	line[0] = {TexturePixelRect.left, TexturePixelRect.top};
	line[1] = {TexturePixelRect.right, TexturePixelRect.top};
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	line[1] = {TexturePixelRect.left, TexturePixelRect.bottom};
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	line[0] = {TexturePixelRect.right, TexturePixelRect.bottom};
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	line[1] = {TexturePixelRect.right, TexturePixelRect.top};
	wrap::Polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
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

  if (line < OSCLAMP) {
	line = 0.0F;
  }
  auto txPoint = TXPNT {0.0F, wrap::round<uint16_t>(line)};
  if ((txPoint.line <= TextureScreen.lines) && (txPoint.line >= 1)) {
	if (offset.y > TextureScreen.top) {
	  if ((offset.y <= TextureScreen.bottom)) {
		txPoint.y = TextureScreen.areaHeight -
		            (wrap::toFloat(offset.y - TextureScreen.top) /
		             (wrap::toFloat(TextureScreen.height) * TextureScreen.areaHeight));
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
  auto offset = POINT {};
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
	auto minimumLength = BIGDBL;
	auto reference     = POINT {};
	auto point         = POINT {};
	txi::deorg(reference);
	closestTexturePoint = 0;
	for (auto iPoint = 0U; iPoint < wrap::toUnsigned(TempTexturePoints->size()); ++iPoint) {
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

void texture::internal::ritxrct() {
  auto const offset = POINT {(TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                             (TextureCursorLocation.y - SelectTexturePointsOrigin.y)};

  auto const rectangle = RECT {(TexturePixelRect.left + offset.x),
                               (TexturePixelRect.top + offset.y),
                               (TexturePixelRect.right + offset.x),
                               (TexturePixelRect.bottom + offset.y)};

  auto line = std::array<POINT, SQPNTS> {};

  line[0].x = line[1].x = line[4].x = rectangle.left;
  line[2].x = line[3].x = rectangle.right;
  line[0].y = line[3].y = line[4].y = rectangle.top;
  line[1].y = line[2].y = rectangle.bottom;
  wrap::Polyline(StitchWindowDC, line.data(), wrap::toUnsigned(line.size()));
}

void texture::internal::dutxrct(TXTRCT& textureRect) {
  if (!SelectedTexturePointsList->empty()) {
	auto* texturePoint = &TempTexturePoints->operator[](SelectedTexturePointsList->front());
	textureRect.left = textureRect.right = texturePoint->line;
	textureRect.top = textureRect.bottom = texturePoint->y;
	for (auto iPoint = 1U; iPoint < wrap::toUnsigned(SelectedTexturePointsList->size()); ++iPoint) {
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
  if (fabs(deltaX) < TNYFLOAT) {
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
  auto const lineRange = wrap::toSize(integerFinish - integerStart);
  TempTexturePoints->reserve(TempTexturePoints->size() + lineRange);
  while (integerStart <= integerFinish) {
	auto const yOffset =
	    slope * (-point0.x + wrap::toFloat(integerStart) * TextureScreen.spacing) + point0.y;
	if (yOffset > 0 && yOffset < TextureScreen.areaHeight) {
	  TempTexturePoints->push_back(TXPNT {yOffset, gsl::narrow<uint16_t>(integerStart)});
	}
	++integerStart;
  }
}

void texture::internal::setxclp(FRMHED const& form) {
  auto screenOffset = POINT {};
  auto editorOffset = fPOINT {};
  txi::deorg(screenOffset);
  txi::px2ed(screenOffset, editorOffset);
  if (StateMap->testAndReset(StateFlag::TXHCNTR)) {
	editorOffset.x = (wrap::toFloat(TextureScreen.lines) * TextureScreen.spacing) / 2.0F +
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
	++lineCount;
  }
  for (auto iLine = 0U; iLine < lineCount; ++iLine) {
	auto const iNextVertex = form::nxt(form, iLine);
	txi::dutxlin(angledFormVertices[iLine], angledFormVertices[iNextVertex]);
  }
}

void texture::internal::stxlin() {
  auto offset = POINT {};
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

  if (val < OSCLAMP) {
	val = 0.0F;
  }
  textureRecord.line = wrap::round<uint16_t>(val);
  textureRecord.y =
      TextureScreen.areaHeight - (wrap::toFloat(offset.y - TextureScreen.top) /
                                  (wrap::toFloat(TextureScreen.height) * TextureScreen.areaHeight));
}

void texture::txtrup() {
  auto offset = POINT {};
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
	  for (auto iPoint = 0U; iPoint < wrap::toUnsigned(TempTexturePoints->size()); ++iPoint) {
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
  for (auto const& vertex : angledFormVertices) {
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
  auto const offset = POINT {(TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                             (TextureCursorLocation.y - SelectTexturePointsOrigin.y)};

  auto& formLines = *FormLines;
  formLines.resize(wrap::toSize(textureForm.vertexCount) + 1U);
  auto&      angledFormVertices = *AngledFormVertices;
  auto const maxVertex          = wrap::toUnsigned(angledFormVertices.size());
  for (auto iVertex = 0U; iVertex < maxVertex; ++iVertex) {
	txi::ed2px(angledFormVertices[iVertex], formLines[iVertex]);
	formLines[iVertex].x += offset.x;
	formLines[iVertex].y += offset.y;
  }
  formLines[maxVertex] = formLines[0];
  auto vertexCount     = maxVertex;
  if (textureForm.type != FRMLINE) {
	++vertexCount;
  }
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::Polyline(StitchWindowDC, formLines.data(), vertexCount);
}

void texture::internal::setxfrm() noexcept {
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
	auto const ratio = TextureScreen.areaHeight / height * 0.95F;
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
  auto const thrEdClip = RegisterClipboardFormat(ThrEdClipFormat);
  ClipMemory           = GetClipboardData(thrEdClip);
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
  wrap::Polyline(StitchWindowDC, FormLines->data(), LNPNTS);
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
		TextureScreen.width    = value * wrap::toFloat(TextureScreen.lines) + value / 2.0F;
		StateMap->set(StateFlag::CHKTX);
		break;
	  }
	  default: {
		outDebugString(L"default hit in chktxnum: TextureWindowId [{}]\n", TextureWindowId);
		break;
	  }
	}
  }
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  StateMap->set(StateFlag::RESTCH);
}

void texture::internal::butsid(uint32_t windowId) {
  auto buttonRect = RECT {};
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
  form.type = FRMFPOLY;
  wrap::narrow(form.fillInfo.texture.lines, TextureScreen.lines);
  form.fillInfo.texture.height    = TextureScreen.areaHeight;
  form.fillSpacing                = TextureScreen.spacing;
  form.lengthOrCount.stitchLength = IniFile.userStitchLength;
  form.maxFillStitchLen           = IniFile.maxStitchLength;
  form.minFillStitchLen           = IniFile.minStitchLength;
  wrap::narrow(form.fillColor, ActiveColor);
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
	  form.angleOrClipData.angle = IniFile.fillAngle;
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
	for (auto iForm = 0U; iForm < formIndex; ++iForm) {
	  if (texture::istx(iForm)) {
		auto& formIter = FormList->operator[](iForm);
		if (formIter.fillInfo.texture.index == currentIndex) {
		  flag = true;
		}
	  }
	}
	for (auto iForm = formIndex + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
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
	  auto iBuffer = uint16_t {};
	  for (auto iForm = 0U; iForm < formIndex; ++iForm) {
		if (texture::istx(iForm)) {
		  auto& fillInfo         = FormList->operator[](iForm).fillInfo;
		  auto const startSource = wrap::next(TexturePointsBuffer->cbegin(), fillInfo.texture.index);
		  auto const endSource   = wrap::next(startSource, fillInfo.texture.count);
		  textureBuffer.resize(textureBuffer.size() + fillInfo.texture.count);
		  auto const destination = std::next(textureBuffer.begin(), iBuffer);
		  std::copy(startSource, endSource, destination);
		  fillInfo.texture.index = iBuffer;
		  iBuffer += fillInfo.texture.count;
		}
	  }
	  for (auto iForm = formIndex + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		if (texture::istx(iForm)) {
		  auto& fillInfo         = FormList->operator[](iForm).fillInfo;
		  auto const startSource = wrap::next(TexturePointsBuffer->cbegin(), fillInfo.texture.index);
		  auto const endSource   = wrap::next(startSource, fillInfo.texture.count);
		  textureBuffer.resize(textureBuffer.size() + fillInfo.texture.count);
		  auto const destination = wrap::next(textureBuffer.begin(), iBuffer);
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
	auto const insertIt       = wrap::next(TexturePointsBuffer->begin(), index);
	TexturePointsBuffer->insert(insertIt, TempTexturePoints->cbegin(), TempTexturePoints->cend());
	for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  if (texture::istx(iForm)) {
		FormList->operator[](iForm).fillInfo.texture.index +=
		    gsl::narrow<decltype(FormList->back().fillInfo.texture.index)>(tempPointCount);
	  }
	}
	wrap::narrow(form.fillInfo.texture.index, index);
	wrap::narrow(form.fillInfo.texture.count, tempPointCount);
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
	for (uint16_t iLine = 1; iLine <= TextureScreen.lines; ++iLine) {
	  if (!txtLines.test(iLine)) {
		TempTexturePoints->push_back(TXPNT {halfHeight, iLine});
	  }
	}
  }
}

void texture::txof() {
  displayText::butxt(HBOXSEL, displayText::loadStr(IDS_BOXSEL));
  thred::redraw(ButtonWin->operator[](HHID));
  if (StateMap->test(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPON));
  }
  else {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
  }
  SetWindowText(ButtonWin->operator[](HTXSPAC), L"");
  texture::savtxt();
  thred::zumhom();
  SelectedTexturePointsList->clear();
  SelectedTexturePointsList->shrink_to_fit();
  StateMap->reset(StateFlag::TXTRED);
}

enum textureStyles { VRTYP, HORTYP, ANGTYP };

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
		outDebugString(L"default hit in dutxfn: textureType [{}]\n", textureType);
		break;
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
	  --iPoint;
	}
	TempTexturePoints->resize(wrap::toSize(iPoint) + 1U);
	auto const iMirrorPoint = iPoint + evenOffset;
	for (auto index = 0U; index < iMirrorPoint; ++index) {
	  auto const newLine =
	      gsl::narrow_cast<uint16_t>(TextureScreen.lines - TempTexturePoints->operator[](index).line + 1U);
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
	auto point = POINT {};
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
  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1L;
}

void texture::internal::txbak() {
  if (StateMap->test(StateFlag::WASTXBAK)) {
	SelectedTexturePointsList->clear();
	auto flag = false;
	for (auto iHistory = 0U; iHistory < ITXBUFSZ; ++iHistory) {
	  if (TextureHistory->operator[](TextureHistoryIndex).width != 0.0F) {
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
	for (auto iHistory = 0U; iHistory < ITXBUFSZ; ++iHistory) {
	  txi::txrfor();
	  if (TextureHistory->operator[](TextureHistoryIndex).width != 0.0F) {
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

auto texture::internal::txdig(wchar_t keyCode, wchar_t& character) noexcept -> bool {
  if (isdigit(keyCode) != 0) {
	character = keyCode;
	return true;
  }
  if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
	character = keyCode - VK_NUMPAD0 + L'0';
	return true;
  }
  if (keyCode == VK_OEM_PERIOD || keyCode == VK_DECIMAL) {
	character = L'.';
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
		if (yCoord < 0.0F) {
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
	  for (auto const& point : *SelectedTexturePointsList) {
		TempTexturePoints->operator[](point).line +=
		    gsl::narrow<decltype(TempTexturePoints->back().line)>(deltaX);
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
	  for (auto iPoint = 0U; iPoint < txpntSize; ++iPoint) {
		auto& texturePoint = TempTexturePoints->operator[](SelectedTexturePointsList->operator[](iPoint));
		auto const yStep = (texturePoint.y + halfGrid) / IniFile.gridSize;
		texturePoint.y   = yStep * IniFile.gridSize;
	  }
	}
	else {
	  for (auto iPoint = 0U; iPoint < txpntSize; ++iPoint) {
		auto& texturePoint = TempTexturePoints->operator[](iPoint);
		auto const yStep   = (texturePoint.y + halfGrid) / IniFile.gridSize;
		texturePoint.y     = yStep * IniFile.gridSize;
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::txtkey(wchar_t keyCode, FRMHED& textureForm) {
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
	  case VK_BACK: // backspace
	  {
		if (!TextureInputBuffer->empty()) {
		  TextureInputBuffer->pop_back();
		}
		flag = false;
		break;
	  }
	  default: {
		outDebugString(L"default hit in txtkey 1: keyCode [{}]\n", keyCode);
		break;
	  }
	}
	if (flag) {
	  constexpr auto BUFFLEN   = 8U; // i.e. floating point 7 digits of precision + '.'
	  auto           character = wchar_t {};
	  if (TextureInputBuffer->size() < BUFFLEN) {
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
	  [[fallthrough]];
	}
	case 'Q': {
	  texture::rstxt();
	  StateMap->set(StateFlag::RESTCH);
	  break;
	}
	case VK_OEM_4: { //  '[{' for US
	  txi::txshrnk(textureForm);
	  break;
	}
	case VK_OEM_6: { //  ']}' for US
	  txi::txgro(textureForm);
	  break;
	}
	case VK_OEM_3: { // '`~' for US
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
	case VK_OEM_MINUS: { // '-' any country
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
	  outDebugString(L"default hit in txtkey 2: keyCode [{}]\n", keyCode);
	  break;
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
	for (auto iTexturePoint = currentCount - 1; iTexturePoint >= 0; --iTexturePoint) {
	  auto const& currentPoint =
	      TexturePointsBuffer->operator[](wrap::toSize(currentIndex) + wrap::toSize(iTexturePoint));
	  if (currentPoint.line != 0U) {
		auto const iSegment            = currentPoint.line - 1U;
		textureSegments[iSegment].line = iTexturePoint;
		++(textureSegments[iSegment].stitchCount);
	  }
	}
  }
}

void texture::rtrtx(FRMHED const& form) {
  TempTexturePoints->clear();
  auto const currentIndex = form.fillInfo.texture.index;
  if (wrap::toUnsigned(TexturePointsBuffer->size()) > gsl::narrow_cast<uint32_t>(currentIndex)) {
	auto currentCount = form.fillInfo.texture.count;
	if (wrap::toUnsigned(TexturePointsBuffer->size()) < gsl::narrow_cast<uint32_t>(currentIndex + currentCount)) {
	  currentCount = gsl::narrow<uint16_t>(TexturePointsBuffer->size()) - currentIndex;
	}
	TempTexturePoints->resize(currentCount);
	auto const startSource = std::next(TexturePointsBuffer->cbegin(), currentIndex);
	auto const endSource   = std::next(startSource, currentCount);
	std::copy(startSource, endSource, TempTexturePoints->begin());
	TextureScreen.areaHeight = form.fillInfo.texture.height;
	TextureScreen.spacing    = form.fillSpacing;
	TextureScreen.lines      = gsl::narrow<uint16_t>(form.fillInfo.texture.lines);
	TextureScreen.width =
	    wrap::toFloat(TextureScreen.lines) * TextureScreen.spacing + TextureScreen.spacing / 2.0F;
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
	texture::savtxt();
	thred::unbBox();
	StateMap->reset(StateFlag::BZUMIN);
	auto const stitchPoint = thred::pxCor2stch(
	    POINT {ZoomBoxLine[2].x + StitchWindowOrigin.x, ZoomBoxLine[2].y + StitchWindowOrigin.y});
	auto const selectionRect =
	    fRECTANGLE {ZoomBoxOrigin.x, ZoomBoxOrigin.y, stitchPoint.x, stitchPoint.y};
	StateMap->reset(StateFlag::TXIN);
	TempTexturePoints->clear();
	auto line = 1U;
	auto TXin = false;
	for (auto& stitch : *StitchBuffer) {
	  if (txi::inrct(selectionRect, stitch)) {
		TXin = true;
		TempTexturePoints->push_back(TXPNT {(stitch.y - selectionRect.bottom), gsl::narrow<uint16_t>(line)});
	  }
	  else {
		if (TXin) {
		  TXin = false;
		  ++line;
		}
	  }
	}
	if (!TempTexturePoints->empty()) {
	  line = TempTexturePoints->back().line;
	}
	if (IniFile.textureEditorSize == 0U) {
	  IniFile.textureEditorSize = ITXPIX;
	}
	TextureScreen.spacing    = (selectionRect.right - selectionRect.left) / wrap::toFloat(line);
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width = TextureScreen.spacing * wrap::toFloat(line) + TextureScreen.spacing / 2.0F;
	StateMap->set(StateFlag::TXTRED);
	StateMap->set(StateFlag::RESTCH);
  }
}

void texture::writeScreenWidth(int32_t position) {
  auto const scrWidth = fmt::format(displayText::loadStr(IDS_TXWID), (TextureScreen.width * IPFGRAN));
  wrap::textOut(DrawItem->hDC, position, 1, scrWidth.c_str(), wrap::toUnsigned(scrWidth.size()));
}
