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

unsigned                   TextureWindowId;           // id of the window being updated
HWND                       SideWindowButton;          // button side window
RECT                       TexturePixelRect;          // screen selected texture points rectangle
TXTRCT                     TextureRect;               // selected texture points rectangle
POINT                      SelectTexturePointsOrigin; // original location of selected texture points
POINT                      TextureCursorLocation;     // texture editor move cursor location
HPEN                       TextureCrossPen;           // texture editor cross pen
TXHST                      TextureHistory[ITXBUFLEN]; // texture editor history headers
int                        TextureHistoryIndex;       // pointer to the next texture history buffer
std::vector<TXPNT>*        TempTexturePoints;         // temporary storage for textured fill data
std::vector<unsigned int>* SelectedTexturePointsList; // list of selected points
TXTSCR                     TextureScreen;             // texture editor layout parameters

void texture::initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<unsigned int>* ptrTexturePointsList) noexcept {
	TempTexturePoints         = ptrTexturePoints;
	SelectedTexturePointsList = ptrTexturePointsList;
}

bool texture::internal::txnam(wchar_t* name, int sizeName) {
	auto texturePath = fs::path(ArgList[0]);

	texturePath.replace_filename(L"thred.txr");
	return !wcscpy_s(name, sizeName, texturePath.c_str());
}

void texture::txdun() {
	const char signature[4]         = "txh";
	auto       textureHistoryBuffer = std::vector<TXHSTBUF>(ITXBUFLEN);

	if (!TextureHistory[0].texturePoints.empty()) {
		wchar_t name[_MAX_PATH] = { 0 };
		if (txi::txnam(name, sizeof(name) / sizeof(name[0]))) {
			DWORD bytesWritten = 0u;
			auto  handle       = CreateFile(name, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
			if (handle != INVALID_HANDLE_VALUE) {
				WriteFile(handle, &signature, sizeof(signature), &bytesWritten, nullptr);
				WriteFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesWritten, nullptr);
				auto bufferIter = textureHistoryBuffer.begin();
				for (const auto& historyEntry : TextureHistory) {
					auto& bufferEntry       = *bufferIter;
					bufferEntry.placeholder = nullptr;
					bufferEntry.count       = gsl::narrow<unsigned>(historyEntry.texturePoints.size());
					bufferEntry.height      = historyEntry.height;
					bufferEntry.width       = historyEntry.width;
					bufferEntry.spacing     = historyEntry.spacing;
					bufferIter++;
				}
				WriteFileInt(handle,
				             textureHistoryBuffer.data(),
				             gsl::narrow<unsigned int>(textureHistoryBuffer.size() * ITXBUFLEN),
				             &bytesWritten,
				             nullptr);
				for (auto& item : TextureHistory) {
					if (!item.texturePoints.empty()) {
						WriteFileInt(
						    handle,
						    item.texturePoints.data(),
						    gsl::narrow<unsigned int>(item.texturePoints.size() * sizeof(TextureHistory[0].texturePoints[0])),
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
	OutputDebugString(fmt::format(L"retrieving texture history {}\n", TextureHistoryIndex).c_str());
	const auto* textureHistoryItem = &TextureHistory[TextureHistoryIndex];
	if (textureHistoryItem) {
		TextureScreen.areaHeight = textureHistoryItem->height;
		TextureScreen.width      = textureHistoryItem->width;
		TextureScreen.spacing    = textureHistoryItem->spacing;
		if (!textureHistoryItem->texturePoints.empty()) {
			TempTexturePoints->clear();
			TempTexturePoints->reserve(textureHistoryItem->texturePoints.size());
			for (auto texturePoint : textureHistoryItem->texturePoints) {
				TempTexturePoints->push_back(texturePoint);
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void texture::redtx() {
	wchar_t name[_MAX_PATH]      = { 0 };
	auto    textureHistoryBuffer = std::vector<TXHSTBUF>(ITXBUFLEN);

	TextureHistoryIndex = ITXBUFLEN - 1;
	if (txi::txnam(name, sizeof(name) / sizeof(name[0]))) {
		auto handle = CreateFile(name, GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
		if (handle != INVALID_HANDLE_VALUE) {
			DWORD bytesRead = 0;
			char  sig[4]    = { 0 };
			if (ReadFile(handle, &sig, sizeof(sig), &bytesRead, nullptr)) {
				if (!strcmp(sig, "txh")) {
					if (ReadFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesRead, nullptr)) {
						DWORD historyBytesRead = 0;
						if (ReadFileInt(handle,
						                textureHistoryBuffer.data(),
						                gsl::narrow<unsigned int>(textureHistoryBuffer.size() * ITXBUFLEN),
						                &historyBytesRead,
						                nullptr)) {
							for (auto index = 0u; index < (historyBytesRead / sizeof(textureHistoryBuffer[0])); index++) {
								TextureHistory[index].height  = textureHistoryBuffer[index].height;
								TextureHistory[index].width   = textureHistoryBuffer[index].width;
								TextureHistory[index].spacing = textureHistoryBuffer[index].spacing;
								if (textureHistoryBuffer[index].count) {
									TextureHistory[index].texturePoints.resize(textureHistoryBuffer[index].count);
									if (!ReadFileInt(handle,
									                 TextureHistory[index].texturePoints.data(),
									                 sizeof(TextureHistory[0].texturePoints[0])
									                     * textureHistoryBuffer[index].count,
									                 &bytesRead,
									                 nullptr)) {
										TextureHistory[index].texturePoints.clear();
										TextureHistory[index].texturePoints.shrink_to_fit();
									}
								}
							}
						}
					}
					StateMap.set(StateFlag::WASTXBAK);
				}
			}
		}
		CloseHandle(handle);
	}
	txi::redtbak();
}

bool texture::istx(unsigned int iForm) noexcept {
	const auto& pfrm = FormList[iForm];
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

void texture::internal::txrfor() noexcept {
	if (TextureHistoryIndex < (ITXBUFLEN - 1)) {
		TextureHistoryIndex++;
	}
	else {
		TextureHistoryIndex = 0;
	}
}

bool texture::internal::chktxh(_In_ const TXHST* historyItem) {
	if (historyItem) {
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
		for (auto iPoint = 0u; iPoint < TempTexturePoints->size(); iPoint++) {
			if ((*TempTexturePoints)[iPoint].line != historyItem->texturePoints[iPoint].line) {
				return true;
			}
			if ((*TempTexturePoints)[iPoint].y != historyItem->texturePoints[iPoint].y) {
				return true;
			}
		}
	}
	return false;
}

void texture::savtxt() {
	if (!TempTexturePoints->empty()) {
		auto currentHistoryItem = &TextureHistory[TextureHistoryIndex];
		if (txi::chktxh(currentHistoryItem)) {
			StateMap.set(StateFlag::WASTXBAK);
			StateMap.reset(StateFlag::TXBDIR);
			StateMap.reset(StateFlag::LASTXBAK);
			txi::txrfor();
			currentHistoryItem          = &TextureHistory[TextureHistoryIndex];
			currentHistoryItem->height  = TextureScreen.areaHeight;
			currentHistoryItem->width   = TextureScreen.width;
			currentHistoryItem->spacing = TextureScreen.spacing;
			currentHistoryItem->texturePoints.clear();
			currentHistoryItem->texturePoints.reserve(TempTexturePoints->size());
			for (auto texturePoint : *TempTexturePoints) {
				currentHistoryItem->texturePoints.push_back(texturePoint);
			}
		}
	}
}

void texture::internal::txrbak() noexcept {
	if (TextureHistoryIndex > 0) {
		TextureHistoryIndex--;
	}
	else {
		TextureHistoryIndex = ITXBUFLEN - 1;
	}
}

void texture::dutxtfil() {
	if (!IniFile.textureHeight) {
		IniFile.textureHeight = ITXHI;
	}
	if (!IniFile.textureWidth) {
		IniFile.textureWidth = ITXWID;
	}
	if (!IniFile.textureSpacing) {
		IniFile.textureSpacing = static_cast<float>(ITXSPAC);
	}
	if (!IniFile.textureEditorSize) {
		IniFile.textureEditorSize = ITXPIX;
	}
	AngledForm.vertexCount = 0;
	StateMap.set(StateFlag::TXTRED);
	StateMap.set(StateFlag::ZUMED);
	StateMap.reset(StateFlag::WASPAT);
	StateMap.reset(StateFlag::RUNPAT);
	thred::movStch();
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
	SelectedTexturePointsList->clear();
	StateMap.set(StateFlag::INIT);
	SideWindowButton = nullptr;
	if (StateMap.test(StateFlag::WASTXBAK)) {
		txi::redtbak();
		if (!TextureScreen.areaHeight) {
			TextureScreen.areaHeight = IniFile.textureHeight;
		}
		if (!TextureScreen.spacing) {
			TextureScreen.spacing = IniFile.underlaySpacing;
		}
		if (!TextureScreen.width) {
			TextureScreen.width = IniFile.textureWidth;
		}
		StateMap.set(StateFlag::LASTXBAK);
		StateMap.reset(StateFlag::TXBDIR);
	}
	else {
		TempTexturePoints->clear();
		TextureScreen.areaHeight = IniFile.textureHeight;
		TextureScreen.width      = IniFile.textureWidth;
		TextureScreen.spacing    = IniFile.textureSpacing;
	}
	StateMap.set(StateFlag::RESTCH);
}

void texture::internal::txt2pix(const TXPNT& texturePoint, POINT& screenPoint) {
	screenPoint.x = dToL((static_cast<double>(TextureScreen.spacing) * texturePoint.line + TextureScreen.xOffset)
	                     / TextureScreen.editToPixelRatio);
	screenPoint.y
	    = dToL(static_cast<double>(TextureScreen.height)
	           - texturePoint.y / static_cast<double>(TextureScreen.areaHeight) * TextureScreen.height + TextureScreen.top);
}

void texture::internal::txtxfn(const POINT& reference, unsigned short offsetPixels) noexcept {
	POINT line[2] = {};

	line[0] = { reference.x, reference.y - offsetPixels };
	line[1] = { reference.x, reference.y + offsetPixels };
	Polyline(StitchWindowMemDC, line, 2);
	line[0] = { reference.x - offsetPixels, reference.y };
	line[1] = { reference.x + offsetPixels, reference.y };
	Polyline(StitchWindowMemDC, line, 2);
}

void texture::internal::dutxtx(unsigned int index, unsigned short offsetPixels) {
	auto ref = POINT{};

	txi::txt2pix((*TempTexturePoints)[index], ref);
	txi::txtxfn(ref, offsetPixels);
	if (ref.y > TextureScreen.halfHeight) {
		ref.y -= TextureScreen.height;
	}
	else {
		ref.y += TextureScreen.height;
	}
	txi::txtxfn(ref, offsetPixels);
}

void texture::internal::txrct2rct(const TXTRCT& textureRect, RECT& rectangle) {
	auto texturePoint = TXPNT{ textureRect.top, textureRect.left };
	auto point        = POINT{};

	txi::txt2pix(texturePoint, point);
	rectangle.left    = point.x - IniFile.textureEditorSize;
	rectangle.top     = point.y - IniFile.textureEditorSize;
	texturePoint.y    = textureRect.bottom;
	texturePoint.line = textureRect.right;
	txi::txt2pix(texturePoint, point);
	rectangle.right  = point.x + IniFile.textureEditorSize;
	rectangle.bottom = point.y + IniFile.textureEditorSize;
}

void texture::internal::ed2px(const fPOINT& editPoint, POINT& point) {
	point.x = dToL(editPoint.x / TextureScreen.editToPixelRatio);
	point.y = dToL(StitchWindowClientRect.bottom - editPoint.y / TextureScreen.editToPixelRatio);
}

void texture::internal::px2ed(const POINT& point, fPOINT& editPoint) noexcept {
	editPoint.x = point.x * TextureScreen.editToPixelRatio;
	editPoint.y = TextureScreen.screenHeight - point.y * TextureScreen.editToPixelRatio;
}

void texture::internal::chktx() {
	auto tmpTexture = std::vector<TXPNT>();
	for (auto& p : *TempTexturePoints) {
		if (p.line <= TextureScreen.lines && p.y <= TextureScreen.areaHeight) {
			tmpTexture.push_back(p);
		}
	}
	*TempTexturePoints = tmpTexture;
}

void texture::drwtxtr() {
	POINT line[2] = {};

	const auto editSpace = TextureScreen.areaHeight * 2.0 / (TextureScreen.spacing * (TextureScreen.lines + 2.0));

	FillRect(StitchWindowMemDC, &StitchWindowClientRect, BackgroundBrush);
	const auto pixelSpace = (StitchWindowClientRect.bottom * 1.0) / StitchWindowClientRect.right;
	TextureScreen.lines   = gsl::narrow<unsigned short>(std::floor(TextureScreen.width / TextureScreen.spacing));
	const auto extraWidth = TextureScreen.spacing * (TextureScreen.lines + 2);
	if (StateMap.testAndReset(StateFlag::CHKTX)) {
		txi::chktx();
	}
	int yOffset = 0;
	if (pixelSpace > editSpace) {
		TextureScreen.xOffset          = 0;
		TextureScreen.editToPixelRatio = extraWidth / StitchWindowClientRect.bottom;
		yOffset                        = gsl::narrow<decltype(yOffset)>(
            std::round((StitchWindowClientRect.bottom - TextureScreen.areaHeight / TextureScreen.editToPixelRatio) / 2.0));
	}
	else {
		TextureScreen.editToPixelRatio = TextureScreen.areaHeight * 2 / StitchWindowClientRect.bottom;
		yOffset                        = StitchWindowClientRect.bottom >> 2;
		TextureScreen.xOffset          = (TextureScreen.editToPixelRatio * StitchWindowClientRect.right
                                 - static_cast<double>(TextureScreen.spacing) * (static_cast<double>(TextureScreen.lines) + 2.0))
		                        / 2.0;
	}
	TextureScreen.top          = yOffset;
	TextureScreen.bottom       = StitchWindowClientRect.bottom - yOffset;
	TextureScreen.height       = TextureScreen.bottom - TextureScreen.top;
	TextureScreen.halfHeight   = StitchWindowClientRect.bottom >> 1;
	TextureScreen.screenHeight = StitchWindowClientRect.bottom * TextureScreen.editToPixelRatio;
	TextureScreen.yOffset      = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, GridPen);
	auto gridLineCount = gsl::narrow<unsigned int>(std::floor(TextureScreen.areaHeight / IniFile.gridSize + 1));
	auto textureRecord = TXPNT{};
	line[0].x          = 0;
	line[1].x          = StitchWindowClientRect.right;
	auto point         = POINT{};
	for (auto iGrid = 0u; iGrid < gridLineCount; iGrid++) {
		txi::txt2pix(textureRecord, point);
		line[0].y = line[1].y = point.y;
		Polyline(StitchWindowMemDC, line, 2);
		textureRecord.y += IniFile.gridSize;
	}
	DeleteObject(TextureCrossPen);
	TextureCrossPen = CreatePenInt(PS_SOLID, 1, UserColor[ActiveColor]);
	SelectObject(StitchWindowMemDC, TextureCrossPen);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	line[0].y = 0;
	line[1].y = StitchWindowClientRect.bottom;
	for (auto iVertical = 1u; iVertical < TextureScreen.lines + 1u; iVertical++) {
		line[0].x = line[1].x = dToL((static_cast<double>(TextureScreen.spacing) * iVertical + TextureScreen.xOffset)
		                             / TextureScreen.editToPixelRatio);
		Polyline(StitchWindowMemDC, line, 2);
	}
	line[0].x = 0;
	line[1].x = StitchWindowClientRect.right;
	line[0].y = line[1].y = TextureScreen.top;
	Polyline(StitchWindowMemDC, line, 2);
	line[0].y = line[1].y = TextureScreen.bottom;
	Polyline(StitchWindowMemDC, line, 2);
	DeleteObject(TextureCrossPen);
	TextureCrossPen = CreatePenInt(PS_SOLID, 1, 0xffffff);
	SelectObject(StitchWindowMemDC, TextureCrossPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	for (auto index = 0u; index < TempTexturePoints->size(); index++) {
		txi::dutxtx(index, IniFile.textureEditorSize);
	}
	if (!SelectedTexturePointsList->empty()) {
		txi::txrct2rct(TextureRect, TexturePixelRect);
		line[0] = { TexturePixelRect.left, TexturePixelRect.top };
		line[1] = { TexturePixelRect.right, TexturePixelRect.top };
		Polyline(StitchWindowMemDC, line, 2);
		line[1] = { TexturePixelRect.left, TexturePixelRect.bottom };
		Polyline(StitchWindowMemDC, line, 2);
		line[0] = { TexturePixelRect.right, TexturePixelRect.bottom };
		Polyline(StitchWindowMemDC, line, 2);
		line[1] = { TexturePixelRect.right, TexturePixelRect.top };
		Polyline(StitchWindowMemDC, line, 2);
	}
	for (auto selectedPoint : *SelectedTexturePointsList) {
		txi::dutxtx(selectedPoint, IniFile.textureEditorSize);
		txi::dutxtx(selectedPoint, IniFile.textureEditorSize << 1);
	}
	BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
	displayText::drwtxbut(TextureScreen);
}

bool texture::internal::px2txt(const POINT& offset) {
	auto editPoint = fPOINT{};

	txi::px2ed(offset, editPoint);
	auto val = (editPoint.x - TextureScreen.xOffset) / TextureScreen.spacing;
	if (val < -0.5f) {
		val = 0.0f;
	}
	auto tmp = TXPNT{ 0.0f, gsl::narrow<unsigned short>(std::round(val)) };
	if (tmp.line > TextureScreen.lines) {
		return false;
	}
	if (tmp.line < 1) {
		return false;
	}
	if (offset.y > TextureScreen.top) {
		if (offset.y > TextureScreen.bottom) {
			return false;
		}
		tmp.y = TextureScreen.areaHeight
		        - ((static_cast<float>(offset.y) - TextureScreen.top) / TextureScreen.height * TextureScreen.areaHeight);
		TempTexturePoints->push_back(tmp);
		return true;
	}

	return false;
}

void texture::internal::deorg(POINT& point) noexcept {
	point = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
}

bool texture::internal::txbutfn() {
	auto offset = POINT{};

	txi::deorg(offset);
	return txi::px2txt(offset);
}

void texture::rstxt() {
	SelectedTexturePointsList->clear();
	StateMap.reset(StateFlag::TXTMOV);
	StateMap.reset(StateFlag::BZUM);
	StateMap.reset(StateFlag::BZUMIN);
	StateMap.set(StateFlag::RESTCH);
	StateMap.reset(StateFlag::POLIMOV);
	DestroyWindow(SideWindowButton);
	SideWindowButton = nullptr;
	StateMap.set(StateFlag::RESTCH);
}

void texture::txtrbut() {
	if (txi::txbutfn()) {
		texture::savtxt();
		StateMap.set(StateFlag::RESTCH);
	}
	else {
		texture::rstxt();
	}
}

bool texture::internal::txtclos(unsigned int& closestTexturePoint) {
	if (closestTexturePoint) {
		auto minimumLength = 1e99;
		auto reference     = POINT{};
		auto point         = POINT{};

		txi::deorg(reference);
		closestTexturePoint = 0;
		for (auto iPoint = 0u; iPoint < TempTexturePoints->size(); iPoint++) {
			txi::txt2pix((*TempTexturePoints)[iPoint], point);
			const auto length = hypot(point.x - reference.x, point.y - reference.y);
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
	StateMap.set(StateFlag::TXTMOV);
	TextureCursorLocation.x = SelectTexturePointsOrigin.x = Msg.pt.x - StitchWindowOrigin.x;
	TextureCursorLocation.y = SelectTexturePointsOrigin.y = Msg.pt.y - StitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void texture::internal::ritxrct() noexcept {
	auto  offset    = POINT{ (TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                         (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };
	auto  rectangle = RECT{ (TexturePixelRect.left + offset.x),
                           (TexturePixelRect.top + offset.y),
                           (TexturePixelRect.right + offset.x),
                           (TexturePixelRect.bottom + offset.y) };
	POINT line[5]   = {};

	line[0].x = line[1].x = line[4].x = rectangle.left;
	line[2].x = line[3].x = rectangle.right;
	line[0].y = line[3].y = line[4].y = rectangle.top;
	line[1].y = line[2].y = rectangle.bottom;
	Polyline(StitchWindowDC, line, 5);
}

void texture::internal::dutxrct(TXTRCT& textureRect) {
	if (!SelectedTexturePointsList->empty()) {
		auto texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[0]];
		textureRect.left = textureRect.right = texturePoint->line;
		textureRect.top = textureRect.bottom = texturePoint->y;
		for (auto iPoint = 1u; iPoint < SelectedTexturePointsList->size(); iPoint++) {
			texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
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
		textureRect.left = textureRect.right = 0u;
		textureRect.top = textureRect.bottom = 0.0f;
	}
}

fPOINT texture::internal::ed2stch(const fPOINT& point) noexcept {
	return { (point.x - TextureScreen.xOffset), (point.y - TextureScreen.yOffset) };
}

void texture::internal::dutxlin(const fPOINT& point0in, const fPOINT& point1in) {
	const auto point0 = txi::ed2stch(point0in);
	const auto point1 = txi::ed2stch(point1in);

	const auto deltaX = point1.x - point0.x;
	if (fabs(deltaX) < TINY) {
		return;
	}
	const auto slope  = (point1.y - point0.y) / deltaX;
	auto       start  = point0.x;
	auto       finish = point1.x;
	if (start > finish) {
		std::swap(start, finish);
	}
	auto integerStart  = gsl::narrow<int>(std::ceil(start / TextureScreen.spacing));
	auto integerFinish = gsl::narrow<int>(std::floor(finish / TextureScreen.spacing));
	if (integerStart < 1) {
		integerStart = 1;
	}
	if (integerFinish > TextureScreen.lines) {
		integerFinish = TextureScreen.lines;
	}
	const auto lineRange = integerFinish - integerStart;
	if (lineRange >= 0) {
		TempTexturePoints->reserve(TempTexturePoints->size() + lineRange);
		while (integerStart <= integerFinish) {
			const auto yOffset = slope * (-point0.x + integerStart * TextureScreen.spacing) + point0.y;
			if (yOffset > 0 && yOffset < TextureScreen.areaHeight) {
				TempTexturePoints->push_back({ yOffset, gsl::narrow<unsigned short>(integerStart) });
			}
			integerStart++;
		}
	}
}

void texture::internal::setxclp() {
	auto screenOffset = POINT{};
	auto editorOffset = fPOINT{};

	txi::deorg(screenOffset);
	txi::px2ed(screenOffset, editorOffset);
	if (StateMap.testAndReset(StateFlag::TXHCNTR)) {
		editorOffset.x = (TextureScreen.lines * TextureScreen.spacing) / 2 + TextureScreen.xOffset - TextureScreen.formCenter.x
		                 + TextureScreen.spacing / 2;
	}
	else {
		editorOffset.x -= TextureScreen.formCenter.x;
	}
	editorOffset.y -= TextureScreen.formCenter.y;
	for (auto iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x += editorOffset.x;
		AngledFormVertices[iVertex].y += editorOffset.y;
	}
	auto lineCount = AngledForm.vertexCount - 1;
	if (AngledForm.type != FRMLINE) {
		lineCount++;
	}
	VertexCount = AngledForm.vertexCount;
	for (auto iLine = 0u; iLine < lineCount; iLine++) {
		const auto iNextVertex = form::nxt(iLine);
		txi::dutxlin(AngledFormVertices[iLine], AngledFormVertices[iNextVertex]);
	}
}

void texture::internal::stxlin() {
	auto offset = POINT{};
	auto point0 = fPOINT{};
	auto point1 = fPOINT{};

	StateMap.reset(StateFlag::TXTMOV);
	txi::deorg(offset);
	txi::px2ed(offset, point1);
	txi::px2ed(FormLines[0], point0);
	txi::dutxlin(point0, point1);
	StateMap.set(StateFlag::RESTCH);
}

void texture::internal::ed2txp(const POINT& offset, TXPNT& textureRecord) {
	auto point = fPOINT{};

	txi::px2ed(offset, point);
	auto val = (point.x - TextureScreen.xOffset) / TextureScreen.spacing;
	if (val < -0.5f) {
		val = 0.0f;
	}
	textureRecord.line = gsl::narrow<unsigned short>(std::round(val));
	textureRecord.y    = TextureScreen.areaHeight
	                  - ((static_cast<float>(offset.y) - TextureScreen.top) / TextureScreen.height * TextureScreen.areaHeight);
}

void texture::txtrup() {
	auto offset = POINT{};

	if (StateMap.testAndReset(StateFlag::TXTMOV)) {
		texture::savtxt();
		txi::deorg(offset);
		offset.x -= SelectTexturePointsOrigin.x;
		offset.y -= SelectTexturePointsOrigin.y;
		const auto Xmagnitude = abs(offset.x);
		auto       textureOffset
		    = TXOFF{ static_cast<float>(-offset.y) / TextureScreen.height * TextureScreen.areaHeight,
			         gsl::narrow<int>(std::ceil(Xmagnitude * TextureScreen.editToPixelRatio / TextureScreen.spacing)) };
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
			auto& texturePoint = (*TempTexturePoints)[point];
			texturePoint.line += textureOffset.line;
			texturePoint.y += textureOffset.y;
		}
		txi::dutxrct(TextureRect);
	}
	else {
		if (StateMap.testAndReset(StateFlag::BZUMIN)) {
			auto highestTexturePoint = TXPNT{};
			auto lowestTexturePoint  = TXPNT{};
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
			for (auto iPoint = 0u; iPoint < TempTexturePoints->size(); iPoint++) {
				if ((*TempTexturePoints)[iPoint].y < highestTexturePoint.y
				    && (*TempTexturePoints)[iPoint].y > lowestTexturePoint.y
				    && (*TempTexturePoints)[iPoint].line <= highestTexturePoint.line
				    && (*TempTexturePoints)[iPoint].line >= lowestTexturePoint.line) {
					SelectedTexturePointsList->push_back(iPoint);
				}
			}
			txi::dutxrct(TextureRect);
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

void texture::internal::angrct(fRECTANGLE& rectangle) noexcept {
	rectangle.left = rectangle.right = AngledFormVertices[0].x;
	rectangle.bottom = rectangle.top = AngledFormVertices[0].y;
	for (auto iVertex = 1u; iVertex < AngledForm.vertexCount; iVertex++) {
		if (AngledFormVertices[iVertex].x < rectangle.left) {
			rectangle.left = AngledFormVertices[iVertex].x;
		}
		if (AngledFormVertices[iVertex].x > rectangle.right) {
			rectangle.right = AngledFormVertices[iVertex].x;
		}
		if (AngledFormVertices[iVertex].y > rectangle.top) {
			rectangle.top = AngledFormVertices[iVertex].y;
		}
		if (AngledFormVertices[iVertex].y < rectangle.bottom) {
			rectangle.bottom = AngledFormVertices[iVertex].y;
		}
	}
}

void texture::internal::ritxfrm() {
	auto offset = POINT{ (TextureCursorLocation.x - SelectTexturePointsOrigin.x),
		                 (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };

	offset.x = TextureCursorLocation.x - SelectTexturePointsOrigin.x;
	offset.y = TextureCursorLocation.y - SelectTexturePointsOrigin.y;
	for (auto iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		txi::ed2px(AngledFormVertices[iVertex], FormLines[iVertex]);
		FormLines[iVertex].x += offset.x;
		FormLines[iVertex].y += offset.y;
	}
	FormLines[AngledForm.vertexCount] = FormLines[0];
	auto vertexCount                  = AngledForm.vertexCount;
	if (AngledForm.type != FRMLINE) {
		vertexCount++;
	}
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	PolylineInt(StitchWindowDC, FormLines, vertexCount);
}

void texture::internal::setxfrm() {
	auto angleRect = fRECTANGLE{};

	txi::angrct(angleRect);
	for (auto iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x -= angleRect.left;
		AngledFormVertices[iVertex].y -= angleRect.bottom;
	}
	txi::angrct(angleRect);
	const auto height = angleRect.top - angleRect.bottom;
	if (height > TextureScreen.areaHeight) {
		const auto ratio = TextureScreen.areaHeight / height * 0.95;
		for (auto iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex].x *= ratio;
			AngledFormVertices[iVertex].y *= ratio;
		}
		txi::angrct(angleRect);
	}
	TextureScreen.formCenter.x = form::midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = form::midl(angleRect.top, angleRect.bottom);
	txi::ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
}

void texture::internal::txtclp() {
	ThrEdClip  = RegisterClipboardFormat(ThrEdClipFormat);
	ClipMemory = GetClipboardData(ThrEdClip);
	if (ClipMemory) {
		ClipFormHeader = static_cast<FORMCLIP*>(GlobalLock(ClipMemory));
		if (ClipFormHeader) {
			if (ClipFormHeader->clipType == CLP_FRM) {
				SelectedForm     = &ClipFormHeader->form;
				auto vertices    = convert_ptr<fPOINT*>(&SelectedForm[1]);
				AngledForm       = *SelectedForm;
				auto sourceStart = vertices;
				auto sourceEnd   = sourceStart + SelectedForm->vertexCount;
				std::copy(sourceStart, sourceEnd, AngledFormVertices);
				AngledForm.vertices = AngledFormVertices;
				StateMap.reset(StateFlag::TXTLIN);
				StateMap.set(StateFlag::TXTCLP);
				StateMap.set(StateFlag::TXTMOV);
				txi::setxfrm();
				TextureCursorLocation = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
			}
			GlobalUnlock(ClipMemory);
		}
	}
	StateMap.set(StateFlag::RESTCH);
	StateMap.reset(StateFlag::WASWROT);
}

void texture::internal::dutxtlin() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, 2);
}

void texture::txtrmov() {
	if (StateMap.test(StateFlag::TXTLIN)) {
		txi::dutxtlin();
		txi::deorg(FormLines[1]);
		txi::dutxtlin();
		return;
	}
	if (StateMap.test(StateFlag::TXTCLP)) {
		if (StateMap.testAndSet(StateFlag::WASWROT)) {
			txi::ritxfrm();
		}
		TextureCursorLocation = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
		txi::ritxfrm();
	}
	else {
		if (!SelectedTexturePointsList->empty()) {
			txi::ritxrct();
			TextureCursorLocation = { Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y };
			txi::ritxrct();
		}
	}
}

void texture::internal::txtlin() {
	txi::deorg(FormLines[0]);
	txi::deorg(FormLines[1]);
	StateMap.reset(StateFlag::TXTCLP);
	StateMap.set(StateFlag::TXTLIN);
	StateMap.set(StateFlag::TXTMOV);
}

void texture::internal::chktxnum() {
	auto value = 0.0f;

	if (!TextureInputBuffer->empty()) {
		value = std::stof(*TextureInputBuffer);
	}
	if (value) {
		TextureInputBuffer->clear();
		value *= PFGRAN;
		switch (TextureWindowId) {
		case HTXHI: {
			texture::savtxt();
			TextureScreen.areaHeight = value;
			IniFile.textureHeight    = value;
			StateMap.set(StateFlag::CHKTX);
			break;
		}
		case HTXWID: {
			texture::savtxt();
			TextureScreen.width  = value;
			IniFile.textureWidth = value;
			StateMap.set(StateFlag::CHKTX);
			break;
		}
		case HTXSPAC: {
			texture::savtxt();
			TextureScreen.spacing  = value;
			IniFile.textureSpacing = value;
			TextureScreen.width    = value * TextureScreen.lines + value / 2;
			StateMap.set(StateFlag::CHKTX);
			break;
		}
		}
	}
	DestroyWindow(SideWindowButton);
	SideWindowButton = nullptr;
	StateMap.set(StateFlag::RESTCH);
}

void texture::internal::butsid(unsigned windowId) {
	auto buttonRect = RECT{};

	txi::chktxnum();
	TextureWindowId = windowId;
	GetWindowRect((*ButtonWin)[windowId], &buttonRect);
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

bool texture::internal::txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1) noexcept {
	if (texturePoint0.line == texturePoint1.line) {
		return (texturePoint0.y < texturePoint1.y);
	}

	return (texturePoint0.line < texturePoint1.line);
}

void texture::internal::txpar() {
	SelectedForm->type                       = FRMFPOLY;
	SelectedForm->fillInfo.texture.lines     = TextureScreen.lines;
	SelectedForm->fillInfo.texture.height    = TextureScreen.areaHeight;
	SelectedForm->fillSpacing                = TextureScreen.spacing;
	SelectedForm->lengthOrCount.stitchLength = IniFile.userStitchLength;
	SelectedForm->maxFillStitchLen           = IniFile.maxStitchLength;
	SelectedForm->minFillStitchLen           = IniFile.minStitchLength;
	SelectedForm->fillColor                  = gsl::narrow<unsigned char>(ActiveColor);
	form::refilfn();
}

void texture::internal::txvrt() {
	if (!TempTexturePoints->empty()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXVRTF;
			txi::txpar();
		}
	}
}

void texture::internal::txhor() {
	if (!TempTexturePoints->empty()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXHORF;
			txi::txpar();
		}
	}
}

void texture::internal::txang() {
	if (!TempTexturePoints->empty()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			form::fvars(ClosestFormToCursor);
			SelectedForm->fillType              = TXANGF;
			SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
			txi::txpar();
		}
	}
}

void texture::deltx() {
	auto flag = false;

	const auto currentIndex = FormList[ClosestFormToCursor].fillInfo.texture.index;

	if (TextureIndex && texture::istx(ClosestFormToCursor) && SelectedForm->fillInfo.texture.count) {
		// First check to see if the texture is shared between forms
		for (auto iForm = 0u; iForm < ClosestFormToCursor; iForm++) {
			if (texture::istx(iForm)) {
				if (FormList[iForm].fillInfo.texture.index == currentIndex) {
					flag = true;
				}
			}
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			if (texture::istx(iForm)) {
				if (FormList[iForm].fillInfo.texture.index == currentIndex) {
					flag = true;
				}
			}
		}
		// Only if it is not shared, should the texture be deleted
		if (!flag) {
			auto textureBuffer = std::vector<TXPNT>();
			textureBuffer.reserve(TextureIndex);
			auto iBuffer = 0u;
			for (auto iForm = 0u; iForm < ClosestFormToCursor; iForm++) {
				if (texture::istx(iForm)) {
					auto startSource = TexturePointsBuffer->cbegin() + FormList[iForm].fillInfo.texture.index;
					auto endSource   = startSource + FormList[iForm].fillInfo.texture.count;
					textureBuffer.resize(textureBuffer.size() + FormList[iForm].fillInfo.texture.count);
					auto destination                       = textureBuffer.begin() + iBuffer;
					auto _                                 = std::copy(startSource, endSource, destination);
					FormList[iForm].fillInfo.texture.index = iBuffer;
					iBuffer += FormList[iForm].fillInfo.texture.count;
				}
			}
			for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
				if (texture::istx(iForm)) {
					auto startSource = TexturePointsBuffer->cbegin() + FormList[iForm].fillInfo.texture.index;
					auto endSource   = startSource + FormList[iForm].fillInfo.texture.count;
					textureBuffer.resize(textureBuffer.size() + FormList[iForm].fillInfo.texture.count);
					auto destination                       = textureBuffer.begin() + iBuffer;
					auto _                                 = std::copy(startSource, endSource, destination);
					FormList[iForm].fillInfo.texture.index = iBuffer;
					iBuffer += FormList[iForm].fillInfo.texture.count;
				}
			}
			TextureIndex         = iBuffer;
			*TexturePointsBuffer = textureBuffer;
		}
		FormList[ClosestFormToCursor].fillType = 0;
	}
}

void texture::internal::nutx() {
	auto index = 0u;

	std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), txi::txcmp);
	if (FormIndex) {
		if (texture::istx(ClosestFormToCursor)) {
			index = FormList[ClosestFormToCursor].fillInfo.texture.index;
			texture::deltx();
		}
		else {
			for (auto iForm = ClosestFormToCursor; iForm-- > 0;) {
				if (texture::istx(iForm)) {
					const auto& formHeader = FormList[iForm];
					index                  = formHeader.fillInfo.texture.index + formHeader.fillInfo.texture.count;
					break;
				}
			}
		}
	}
	if (!TempTexturePoints->empty()) {
		const auto tempPointCount = gsl::narrow<unsigned int>(TempTexturePoints->size());
		const auto pointCount     = TextureIndex - index;
		TexturePointsBuffer->resize(TexturePointsBuffer->size() + tempPointCount);
		TextureIndex += tempPointCount;
		auto startSource = TexturePointsBuffer->begin() + index;
		if (pointCount) {
			auto endSource   = startSource + pointCount;
			auto destination = startSource + tempPointCount;
			auto _           = std::copy(startSource, endSource, destination);
		}
		for (auto iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			if (texture::istx(iForm)) {
				FormList[iForm].fillInfo.texture.index += gsl::narrow<unsigned short>(tempPointCount);
			}
		}
		auto _ = std::copy(TempTexturePoints->cbegin(), TempTexturePoints->cend(), startSource);

		SelectedForm->fillInfo.texture.index = gsl::narrow<unsigned short>(index);
		SelectedForm->fillInfo.texture.count = gsl::narrow<unsigned short>(tempPointCount);
	}
}

// Ensure all lines in the texture have at least 1 point
void texture::internal::altx() {
	auto txtLines = boost::dynamic_bitset<>(gsl::narrow<size_t>(TextureScreen.lines) + 1);
	if (StateMap.test(StateFlag::FORMSEL)) {
		const auto halfHeight = TextureScreen.areaHeight / 2.0f;
		for (auto& texturePoint : *TempTexturePoints) {
			txtLines.set(texturePoint.line);
		}
		for (unsigned short iLine = 1; iLine <= TextureScreen.lines; iLine++) {
			if (!txtLines.test(iLine)) {
				TempTexturePoints->push_back({ halfHeight, iLine });
			}
		}
	}
}

void texture::txof() {
	displayText::butxt(HBOXSEL, (*StringTable)[STR_BOXSEL]);
	thred::redraw((*ButtonWin)[HHID]);
	if (StateMap.test(StateFlag::UPTO)) {
		displayText::butxt(HUPTO, (*StringTable)[STR_UPON]);
	}
	else {
		displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
	}
	SetWindowText((*ButtonWin)[HTXSPAC], L"");
	texture::savtxt();
	thred::zumhom();
	SelectedTexturePointsList->clear();
	SelectedTexturePointsList->shrink_to_fit();
	StateMap.reset(StateFlag::TXTRED);
}

enum textureStyles
{
	VRTYP,
	HORTYP,
	ANGTYP,
};

void texture::internal::dutxfn(unsigned textureType) {
	if (StateMap.test(StateFlag::FORMSEL)) {
		txi::altx();
		clip::delmclp(ClosestFormToCursor);
		satin::delsac(ClosestFormToCursor);
		texture::deltx();
		texture::savtxt();
		txi::nutx();
		form::dusqr();
		switch (textureType) {
		case VRTYP: {
			txi::txvrt();
			break;
		}
		case HORTYP: {
			txi::txhor();
			break;
		}
		case ANGTYP: {
			txi::txang();
			break;
		}
		}
	}
	texture::txof();
	texture::rstxt();
	if (StateMap.testAndReset(StateFlag::WASFRMFRM)) {
		formForms::refrm();
	}
}

void texture::internal::dutxmir() {
	if (!TempTexturePoints->empty()) {
		const auto centerLine = (TextureScreen.lines + 1u) >> 1;

		texture::savtxt();
		std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), txi::txcmp);

		auto iPoint = TempTexturePoints->size() - 1;
		while ((*TempTexturePoints)[iPoint].line > centerLine && iPoint >= 0) {
			iPoint--;
		}
		const auto iMirrorPoint = iPoint + 1;
		// ToDo - What does this do? iPoint is not used beyond here
		if (TextureScreen.lines & 1) {
			while (iPoint >= 0) {
				if ((*TempTexturePoints)[iPoint].line == centerLine) {
					iPoint--;
				}
				else {
					break;
				}
			}
		}
		TempTexturePoints->resize(iMirrorPoint);
		for (auto index = 0u; index < iMirrorPoint; index++) {
			TempTexturePoints->push_back(
			    { (*TempTexturePoints)[index].y,
			      gsl::narrow<unsigned short>(TextureScreen.lines - (*TempTexturePoints)[index].line + 1) });
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void texture::internal::txdelal() {
	TextureInputBuffer->clear();
	DestroyWindow(SideWindowButton);
	SideWindowButton = nullptr;
	StateMap.set(StateFlag::RESTCH);
	texture::savtxt();
	TempTexturePoints->clear();
	texture::rstxt();
	StateMap.set(StateFlag::RESTCH);
}

bool texture::internal::chkbut() {
	if (Msg.hwnd == (*ButtonWin)[HTXCLR]) {
		txi::txdelal();
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXHI]) {
		txi::butsid(HTXHI);
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXWID]) {
		txi::butsid(HTXWID);
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXSPAC]) {
		txi::butsid(HTXSPAC);
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXVRT]) {
		txi::dutxfn(VRTYP);
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXHOR]) {
		txi::dutxfn(HORTYP);
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXANG]) {
		txi::dutxfn(ANGTYP);
		return true;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXMIR]) {
		txi::dutxmir();
		return true;
	}
	return false;
}

void texture::txtlbut() {
	form::fvars(ClosestFormToCursor);
	if (txi::chkbut()) {
		return;
	}
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		texture::savtxt();
		txi::setxclp();
		return;
	}
	if (StateMap.testAndReset(StateFlag::TXTLIN)) {
		texture::savtxt();
		txi::stxlin();
		return;
	}
	if (!SelectedTexturePointsList->empty()) {
		auto point = POINT{};
		txi::deorg(point);
		if (point.x > TexturePixelRect.left && point.x < TexturePixelRect.right && point.y > TexturePixelRect.top
		    && point.y < TexturePixelRect.bottom) {
			txi::setxmov();
			txi::ritxrct();
			return;
		}
	}
	if (!SelectedTexturePointsList->empty()) {
		if (txi::txtclos((*SelectedTexturePointsList)[0])) {
			SelectedTexturePointsList->resize(1);
			txi::setxmov();
			txi::dutxrct(TextureRect);
			StateMap.set(StateFlag::RESTCH);
			return;
		}
	}
	SelectedTexturePointsList->clear();
	StateMap.set(StateFlag::BZUMIN);
	StateMap.reset(StateFlag::BZUM);
	ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = Msg.pt.x - StitchWindowOrigin.x;
	ZoomBoxLine[0].y = ZoomBoxLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
	ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1;
}

void texture::internal::txbak() {
	if (StateMap.test(StateFlag::WASTXBAK)) {
		SelectedTexturePointsList->clear();
		auto flag = false;
		for (auto iHistory = 0u; iHistory < ITXBUFLEN; iHistory++) {
			if (TextureHistory[TextureHistoryIndex].width) {
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
	if (StateMap.test(StateFlag::WASTXBAK)) {
		auto flag = false;
		for (auto iHistory = 0u; iHistory < ITXBUFLEN; iHistory++) {
			txi::txrfor();
			if (TextureHistory[TextureHistoryIndex].width) {
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
		// Another potential pattern for this:
		// myVector.erase(remove_if(myVector.begin(), myVector.end(), testFunction), myVector.end());
		auto tmpTexture = std::vector<TXPNT>();
		tmpTexture.reserve(TempTexturePoints->size() - SelectedTexturePointsList->size());
		for (auto iSourcePoint = 0u; iSourcePoint < TempTexturePoints->size(); iSourcePoint++) {
			if (!texturePointsMap.test(iSourcePoint)) {
				tmpTexture.push_back((*TempTexturePoints)[iSourcePoint]);
			}
		}
		*TempTexturePoints = tmpTexture;
		SelectedTexturePointsList->clear();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	auto iClosestPoint = 0u;
	if (!TempTexturePoints->empty() && txi::txtclos(iClosestPoint)) {
		auto it = TempTexturePoints->begin();
		std::advance(it, iClosestPoint);
		it = TempTexturePoints->erase(it);
		StateMap.set(StateFlag::RESTCH);
	}
}

void texture::internal::txcntrv() {
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		StateMap.set(StateFlag::TXHCNTR);
		texture::savtxt();
		txi::setxclp();
		StateMap.set(StateFlag::RESTCH);
	}
}

void texture::internal::txsiz(double ratio) {
	txi::ritxfrm();
	for (auto iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x *= ratio;
		AngledFormVertices[iVertex].y *= ratio;
	}
	auto angleRect = fRECTANGLE{};
	txi::angrct(angleRect);
	TextureScreen.formCenter.x = form::midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = form::midl(angleRect.top, angleRect.bottom);
	txi::ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
	txi::ritxfrm();
}

void texture::internal::txshrnk() {
	txi::txsiz(TXTRAT);
}

void texture::internal::txgro() {
	txi::txsiz(1 / TXTRAT);
}

bool texture::internal::txdig(unsigned keyCode, char& character) {
	if (isdigit(keyCode)) {
		character = gsl::narrow<char>(keyCode);
		return true;
	}
	if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
		character = gsl::narrow<char>(keyCode) - VK_NUMPAD0 + 0x30;
		return true;
	}
	if (keyCode == 0xbe || keyCode == 0x6e) {
		character = '.';
		return true;
	}
	return false;
}

void texture::internal::txnudg(int deltaX, float deltaY) {
	if (!SelectedTexturePointsList->empty()) {
		if (deltaY) {
			const auto screenDeltaY = deltaY * TextureScreen.editToPixelRatio;
			for (auto point : *SelectedTexturePointsList) {
				const auto yCoord = (*TempTexturePoints)[point].y + screenDeltaY;
				if (yCoord < 0.0) {
					return;
				}
				if (yCoord > TextureScreen.areaHeight) {
					return;
				}
			}
			for (auto point : *SelectedTexturePointsList) {
				(*TempTexturePoints)[point].y += screenDeltaY;
			}
		}
		else {
			for (auto point : *SelectedTexturePointsList) {
				const auto textureLine = (*TempTexturePoints)[point].line + deltaX;
				if (textureLine < 1) {
					return;
				}
				if (textureLine > TextureScreen.lines) {
					return;
				}
			}
			for (const auto point : *SelectedTexturePointsList) {
				(*TempTexturePoints)[point].line += gsl::narrow<unsigned short>(deltaX);
			}
		}
	}
	txi::dutxrct(TextureRect);
	StateMap.set(StateFlag::RESTCH);
}

void texture::txsnap() {
	const auto txpntSize = TempTexturePoints->size();

	if (txpntSize) {
		texture::savtxt();
		const auto halfGrid = IniFile.gridSize / 2;
		if (!SelectedTexturePointsList->empty()) {
			for (auto iPoint = 0u; iPoint < txpntSize; iPoint++) {
				auto       texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
				const auto yStep        = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y         = yStep * IniFile.gridSize;
			}
		}
		else {
			for (auto iPoint = 0u; iPoint < txpntSize; iPoint++) {
				auto       texturePoint = &(*TempTexturePoints)[iPoint];
				const auto yStep        = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y         = yStep * IniFile.gridSize;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void texture::txtkey(unsigned keyCode) {
	char character = {};

	if (SideWindowButton) {
		auto flag = true;
		switch (keyCode) {
		case VK_RETURN: {
			txi::chktxnum();
			return;
		}
		case VK_ESCAPE: {
			texture::rstxt();
			StateMap.set(StateFlag::RESTCH);
			break;
		}
		case 'Q': {
			texture::rstxt();
			StateMap.set(StateFlag::RESTCH);
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
		}
		if (flag) {
			if (TextureInputBuffer->size() < 8) { // floating point 7 digits of precision + '.'
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
		StateMap.set(StateFlag::RESTCH);
		break;
	}
	case 0xdb: { //[
		txi::txshrnk();
		break;
	}
	case 0xdd: { //]
		txi::txgro();
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
		if (!StateMap.testAndSet(StateFlag::LASTXBAK)) {
			texture::savtxt();
			txi::txrbak();
		}
		else {
			if (StateMap.testAndReset(StateFlag::TXBDIR)) {
				txi::txrbak();
			}
		}
		txi::txbak();
		return;
	}
	case 'V': {
		if (OpenClipboard(ThrEdWindow)) {
			txi::txtclp();
		}
		break;
	}
	case 'N': {
		StateMap.set(StateFlag::LASTXBAK);
		if (!StateMap.testAndSet(StateFlag::TXBDIR)) {
			txi::txrfor();
		}
		txi::nxbak();
		return;
	}
	case 'D':
	case VK_DELETE: {
		if (GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & 0X8000) {
			txi::txdelal();
		}
		else {
			txi::txtdel();
		}
		break;
	}
	case 0xbd: { //-
		txi::txcntrv();
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
	}
	StateMap.reset(StateFlag::LASTXBAK);
}

void texture::setxt(std::vector<RNGCNT>& textureSegments) {
	const auto currentIndex = SelectedForm->fillInfo.texture.index;
	const auto currentCount = SelectedForm->fillInfo.texture.count;

	texture::savtxt();
	SelectedForm->wordParam = 0;
	StateMap.set(StateFlag::TXFIL);
	ClipRectSize.cx = SelectedForm->fillSpacing;
	ClipRectSize.cy = SelectedForm->fillInfo.texture.height;
	if (currentCount) {
		for (auto iTexturePoint = currentCount - 1; iTexturePoint >= 0; iTexturePoint--) {
			const auto currentPoint = TexturePointsBuffer->at(gsl::narrow<size_t>(currentIndex) + iTexturePoint);
			if (currentPoint.line) {
				const auto iSegment = currentPoint.line - 1;

				textureSegments[iSegment].line = iTexturePoint;
				textureSegments[iSegment].stitchCount++;
			}
		}
	}
}

void texture::rtrtx() {
	form::fvars(ClosestFormToCursor);
	TempTexturePoints->clear();

	const auto currentIndex = SelectedForm->fillInfo.texture.index;
	if (TexturePointsBuffer->size() > gsl::narrow<unsigned int>(currentIndex)) {
		auto currentCount = SelectedForm->fillInfo.texture.count;
		if (TexturePointsBuffer->size() < gsl::narrow<unsigned int>(currentIndex + currentCount)) {
			currentCount = gsl::narrow<unsigned short>(TexturePointsBuffer->size()) - currentIndex;
		}
		TempTexturePoints->resize(currentCount);
		auto startSource         = TexturePointsBuffer->cbegin() + currentIndex;
		auto endSource           = startSource + currentCount;
		auto _                   = std::copy(startSource, endSource, TempTexturePoints->begin());
		TextureScreen.areaHeight = SelectedForm->fillInfo.texture.height;
		TextureScreen.spacing    = SelectedForm->fillSpacing;
		TextureScreen.lines      = SelectedForm->fillInfo.texture.lines;
		TextureScreen.width      = TextureScreen.lines * TextureScreen.spacing + TextureScreen.spacing / 2;
		texture::savtxt();
	}
}

bool texture::internal::inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch) noexcept {
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
	auto selectionRect = fRECTANGLE{};

	texture::savtxt();
	thred::unbBox();
	StateMap.reset(StateFlag::BZUMIN);
	thred::pxCor2stch(ZoomBoxLine[0]);
	selectionRect.top  = SelectedPoint.y;
	selectionRect.left = SelectedPoint.x;
	thred::pxCor2stch(ZoomBoxLine[2]);
	selectionRect.bottom = SelectedPoint.y;
	selectionRect.right  = SelectedPoint.x;
	StateMap.reset(StateFlag::TXIN);
	auto line = 1u;
	for (auto iStitch = 0u; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (txi::inrct(selectionRect, StitchBuffer[iStitch])) {
			StateMap.set(StateFlag::TXIN);
			TempTexturePoints->push_back({ (StitchBuffer[iStitch].y - selectionRect.bottom), gsl::narrow<unsigned short>(line) });
		}
		else {
			if (StateMap.testAndReset(StateFlag::TXIN)) {
				line++;
			}
		}
	}
	if (!TempTexturePoints->empty()) {
		line = TempTexturePoints->back().line;
	}
	TextureScreen.spacing    = (selectionRect.right - selectionRect.left) / line;
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width      = TextureScreen.spacing * line + TextureScreen.spacing / 2;
	StateMap.set(StateFlag::TXTRED);
	StateMap.set(StateFlag::RESTCH);
}

void texture::writeScreenWidth(unsigned position) {
	auto fmtStr = std::wstring{};
	displayText::loadString(fmtStr, IDS_TXWID);
	auto scrWidth = std::wstring(fmt::format(fmtStr, (TextureScreen.width / PFGRAN)));
	TextOutInt(DrawItem->hDC, position, 1, scrWidth.c_str(), gsl::narrow<unsigned int>(scrWidth.size()));
}