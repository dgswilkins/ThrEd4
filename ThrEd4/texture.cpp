#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// C RunTime Header Files

// Open Source headers
#include <CppCoreCheck\warnings.h>
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 4127) // supress warning for fmt library header
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
#include "satin.h"
#include "thred.h"
#include "texture.h"

unsigned             TextureWindowId;           // id of the window being updated
HWND                 SideWindowButton;          // button side window
RECT                 TexturePixelRect;          // screen selected texture points rectangle
TXTRCT               TextureRect;               // selected texture points rectangle
POINT                SelectTexturePointsOrigin; // original location of selected texture points
POINT                TextureCursorLocation;     // texture editor move cursor location
HPEN                 TextureCrossPen;           // texture editor cross pen
TXHST                TextureHistory[ITXBUFLEN]; // texture editor history headers
int                  TextureHistoryIndex;       // pointer to the next texture history buffer
std::vector<TXPNT>*  TempTexturePoints;         // temporary storage for textured fill data
std::vector<size_t>* SelectedTexturePointsList; // list of selected points
TXTSCR               TextureScreen;             // texture editor layout parameters

void texture::initTextures(std::vector<TXPNT>* ptrTexturePoints, std::vector<size_t>* ptrTexturePointsList) noexcept {
	TempTexturePoints         = ptrTexturePoints;
	SelectedTexturePointsList = ptrTexturePointsList;
}

bool texturePriv::txnam(wchar_t* name, int sizeName) {
	fs::path texturePath(ArgList[0]);

	texturePath.replace_filename(L"thred.txr");
	if (wcscpy_s(name, sizeName, texturePath.c_str())) {
		return false;
	}
	else {
		return true;
	}
}

void texture::txdun() {
	wchar_t               name[_MAX_PATH] = { 0 };
	const char            signature[4]    = "txh";
	std::vector<TXHSTBUF> textureHistoryBuffer(ITXBUFLEN);

	if (TextureHistory[0].texturePoint.size()) {
		if (texturePriv::txnam(name, sizeof(name) / sizeof(name[0]))) {
			DWORD bytesWritten = 0;
			auto  handle       = CreateFile(name, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
			if (handle != INVALID_HANDLE_VALUE) {
				WriteFile(handle, &signature, sizeof(signature), &bytesWritten, 0);
				WriteFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesWritten, 0);
				for (auto i = 0; i < ITXBUFLEN; i++) {
					auto&       bufferEntry  = textureHistoryBuffer[i];
					const auto& historyEntry = TextureHistory[i];
					bufferEntry.placeholder  = nullptr;
					bufferEntry.count        = historyEntry.texturePoint.size();
					bufferEntry.height       = historyEntry.height;
					bufferEntry.width        = historyEntry.width;
					bufferEntry.spacing      = historyEntry.spacing;
				}
				WriteFileInt(handle, textureHistoryBuffer.data(), textureHistoryBuffer.size() * ITXBUFLEN, &bytesWritten, 0);
				for (size_t iHistory = 0; iHistory < ITXBUFLEN; iHistory++) {
					if (TextureHistory[iHistory].texturePoint.size())
						WriteFileInt(handle,
						             TextureHistory[iHistory].texturePoint.data(),
						             TextureHistory[iHistory].texturePoint.size() * sizeof(TextureHistory[0].texturePoint[0]),
						             &bytesWritten,
						             0);
				}
			}
			CloseHandle(handle);
		}
	}
}

void texturePriv::redtbak() {
	OutputDebugString(fmt::format(L"retrieving texture history {}\n", TextureHistoryIndex).c_str());
	const TXHST* textureHistoryItem = &TextureHistory[TextureHistoryIndex];
	if (textureHistoryItem) {
		TextureScreen.areaHeight = textureHistoryItem->height;
		TextureScreen.width      = textureHistoryItem->width;
		TextureScreen.spacing    = textureHistoryItem->spacing;
		if (textureHistoryItem->texturePoint.size()) {
			TempTexturePoints->clear();
			TempTexturePoints->reserve(textureHistoryItem->texturePoint.size());
			for (auto i = 0u; i < textureHistoryItem->texturePoint.size(); i++) {
				TempTexturePoints->push_back(textureHistoryItem->texturePoint[i]);
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void texture::redtx() {
	wchar_t               name[_MAX_PATH] = { 0 };
	std::vector<TXHSTBUF> textureHistoryBuffer(ITXBUFLEN);

	TextureHistoryIndex = ITXBUFLEN - 1;
	if (texturePriv::txnam(name, sizeof(name) / sizeof(name[0]))) {
		auto handle = CreateFile(name, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);
		if (handle != INVALID_HANDLE_VALUE) {
			DWORD bytesRead = 0;
			char  sig[4]    = { 0 };
			if (ReadFile(handle, &sig, sizeof(sig), &bytesRead, 0)) {
				if (!strcmp(sig, "txh")) {
					if (ReadFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesRead, 0)) {
						DWORD historyBytesRead = 0;
						if (ReadFileInt(handle,
						                textureHistoryBuffer.data(),
						                textureHistoryBuffer.size() * ITXBUFLEN,
						                &historyBytesRead,
						                0)) {
							for (size_t index = 0; index < (historyBytesRead / sizeof(textureHistoryBuffer[0])); index++) {
								TextureHistory[index].height  = textureHistoryBuffer[index].height;
								TextureHistory[index].width   = textureHistoryBuffer[index].width;
								TextureHistory[index].spacing = textureHistoryBuffer[index].spacing;
								if (textureHistoryBuffer[index].count) {
									TextureHistory[index].texturePoint.resize(textureHistoryBuffer[index].count);
									if (!ReadFileInt(handle,
									                 TextureHistory[index].texturePoint.data(),
									                 sizeof(TextureHistory[0].texturePoint[0])
									                     * textureHistoryBuffer[index].count,
									                 &bytesRead,
									                 0)) {
										TextureHistory[index].texturePoint.clear();
										TextureHistory[index].texturePoint.shrink_to_fit();
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
	texturePriv::redtbak();
}

bool texture::istx(size_t iForm) noexcept {
	const auto& pfrm = FormList[iForm];
	if (pfrm.fillType == TXVRTF)
		return 1;
	if (pfrm.fillType == TXHORF)
		return 1;
	if (pfrm.fillType == TXANGF)
		return 1;
	return 0;
}

void texturePriv::txrfor() noexcept {
	if (TextureHistoryIndex < (ITXBUFLEN - 1)) {
		TextureHistoryIndex++;
	}
	else {
		TextureHistoryIndex = 0;
	}
}

bool texturePriv::chktxh(_In_ const TXHST* historyItem) {
	if (historyItem) {
		if (historyItem->texturePoint.size() != TempTexturePoints->size())
			return true;
		if (historyItem->height != TextureScreen.areaHeight)
			return true;
		if (historyItem->spacing != TextureScreen.spacing)
			return true;
		if (historyItem->width != TextureScreen.width)
			return true;
		for (size_t iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
			if ((*TempTexturePoints)[iPoint].line != historyItem->texturePoint[iPoint].line)
				return true;
			if ((*TempTexturePoints)[iPoint].y != historyItem->texturePoint[iPoint].y)
				return true;
		}
	}
	return false;
}

void texture::savtxt() {
	if (TempTexturePoints->size()) {
		auto currentHistoryItem = &TextureHistory[TextureHistoryIndex];
		if (texturePriv::chktxh(currentHistoryItem)) {
			StateMap.set(StateFlag::WASTXBAK);
			StateMap.reset(StateFlag::TXBDIR);
			StateMap.reset(StateFlag::LASTXBAK);
			texturePriv::txrfor();
			currentHistoryItem          = &TextureHistory[TextureHistoryIndex];
			currentHistoryItem->height  = TextureScreen.areaHeight;
			currentHistoryItem->width   = TextureScreen.width;
			currentHistoryItem->spacing = TextureScreen.spacing;
			currentHistoryItem->texturePoint.clear();
			currentHistoryItem->texturePoint.reserve(TempTexturePoints->size());
			for (auto i = 0u; i < TempTexturePoints->size(); i++) {
				currentHistoryItem->texturePoint.push_back((*TempTexturePoints)[i]);
			}
		}
	}
}

void texturePriv::txrbak() noexcept {
	if (TextureHistoryIndex > 0) {
		TextureHistoryIndex--;
	}
	else {
		TextureHistoryIndex = ITXBUFLEN - 1;
	}
}

void texture::dutxtfil() {
	if (!IniFile.textureHeight)
		IniFile.textureHeight = ITXHI;
	if (!IniFile.textureWidth)
		IniFile.textureWidth = ITXWID;
	if (!IniFile.textureSpacing)
		IniFile.textureSpacing = static_cast<float>(ITXSPAC);
	if (!IniFile.textureEditorSize)
		IniFile.textureEditorSize = ITXPIX;
	AngledForm.vertexCount = 0;
	StateMap.set(StateFlag::TXTRED);
	StateMap.set(StateFlag::ZUMED);
	StateMap.reset(StateFlag::WASPAT);
	StateMap.reset(StateFlag::RUNPAT);
	movStch();
	ShowWindow(VerticalScrollBar, FALSE);
	ShowWindow(HorizontalScrollBar, FALSE);
	SelectedTexturePointsList->clear();
	StateMap.set(StateFlag::INIT);
	SideWindowButton = 0;
	if (StateMap.test(StateFlag::WASTXBAK)) {
		texturePriv::redtbak();
		if (!TextureScreen.areaHeight)
			TextureScreen.areaHeight = IniFile.textureHeight;
		if (!TextureScreen.spacing)
			TextureScreen.spacing = IniFile.underlaySpacing;
		if (!TextureScreen.width)
			TextureScreen.width = IniFile.textureWidth;
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

void texturePriv::txt2pix(const TXPNT& texturePoint, POINT& screenPoint) noexcept {
	screenPoint.y = TextureScreen.height - texturePoint.y / TextureScreen.areaHeight * TextureScreen.height + TextureScreen.top;
	screenPoint.x = (texturePoint.line * TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
}

void texturePriv::txtxfn(const POINT& reference, unsigned short offsetPixels) noexcept {
	POINT line[2];

	line[0].x = line[1].x = reference.x;
	line[0].y             = reference.y - offsetPixels;
	line[1].y             = reference.y + offsetPixels;
	Polyline(StitchWindowMemDC, line, 2);
	line[0].y = line[1].y = reference.y;
	line[0].x             = reference.x - offsetPixels;
	line[1].x             = reference.x + offsetPixels;
	Polyline(StitchWindowMemDC, line, 2);
}

void texturePriv::dutxtx(size_t index, unsigned short offsetPixels) {
	POINT ref;

	texturePriv::txt2pix((*TempTexturePoints)[index], ref);
	texturePriv::txtxfn(ref, offsetPixels);
	if (ref.y > TextureScreen.halfHeight)
		ref.y -= TextureScreen.height;
	else
		ref.y += TextureScreen.height;
	texturePriv::txtxfn(ref, offsetPixels);
}

void texturePriv::txrct2rct(const TXTRCT& textureRect, RECT& rectangle) noexcept {
	TXPNT texturePoint = { textureRect.top, textureRect.left };
	POINT point        = {};

	texturePriv::txt2pix(texturePoint, point);
	rectangle.left    = point.x - IniFile.textureEditorSize;
	rectangle.top     = point.y - IniFile.textureEditorSize;
	texturePoint.y    = textureRect.bottom;
	texturePoint.line = textureRect.right;
	texturePriv::txt2pix(texturePoint, point);
	rectangle.right  = point.x + IniFile.textureEditorSize;
	rectangle.bottom = point.y + IniFile.textureEditorSize;
}

void texturePriv::ed2px(const fPOINT& editPoint, POINT& point) noexcept {
	point.x = editPoint.x / TextureScreen.editToPixelRatio;
	point.y = StitchWindowClientRect.bottom - editPoint.y / TextureScreen.editToPixelRatio;
}

void texturePriv::px2ed(const POINT& point, fPOINT& editPoint) noexcept {
	editPoint.x = point.x * TextureScreen.editToPixelRatio;
	editPoint.y = TextureScreen.screenHeight - point.y * TextureScreen.editToPixelRatio;
}

void texturePriv::chktx() {
	std::vector<TXPNT> tmpTexture;
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
	TextureScreen.lines   = floor(TextureScreen.width / TextureScreen.spacing);
	const auto extraWidth = TextureScreen.spacing * (TextureScreen.lines + 2);
	if (StateMap.testAndReset(StateFlag::CHKTX))
		texturePriv::chktx();
	int yOffset = 0;
	if (pixelSpace > editSpace) {
		TextureScreen.xOffset          = 0;
		TextureScreen.editToPixelRatio = extraWidth / StitchWindowClientRect.bottom;
		yOffset = (StitchWindowClientRect.bottom - TextureScreen.areaHeight / TextureScreen.editToPixelRatio) / 2;
	}
	else {
		TextureScreen.editToPixelRatio = TextureScreen.areaHeight * 2 / StitchWindowClientRect.bottom;
		yOffset                        = StitchWindowClientRect.bottom >> 2;
		TextureScreen.xOffset
		    = (StitchWindowClientRect.right * TextureScreen.editToPixelRatio - (TextureScreen.lines + 2) * TextureScreen.spacing)
		      / 2;
	}
	TextureScreen.top          = yOffset;
	TextureScreen.bottom       = StitchWindowClientRect.bottom - yOffset;
	TextureScreen.height       = TextureScreen.bottom - TextureScreen.top;
	TextureScreen.halfHeight   = StitchWindowClientRect.bottom >> 1;
	TextureScreen.screenHeight = StitchWindowClientRect.bottom * TextureScreen.editToPixelRatio;
	TextureScreen.yOffset      = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	SelectObject(StitchWindowMemDC, GridPen);
	auto  gridLineCount = gsl::narrow<size_t>(floor(TextureScreen.areaHeight / IniFile.gridSize + 1));
	TXPNT textureRecord = {};
	line[0].x           = 0;
	line[1].x           = StitchWindowClientRect.right;
	POINT point         = {};
	for (size_t iGrid = 0; iGrid < gridLineCount; iGrid++) {
		texturePriv::txt2pix(textureRecord, point);
		line[0].y = line[1].y = point.y;
		Polyline(StitchWindowMemDC, line, 2);
		textureRecord.y += IniFile.gridSize;
	}
	DeleteObject(TextureCrossPen);
	TextureCrossPen = CreatePen(PS_SOLID, 1, UserColor[ActiveColor]);
	SelectObject(StitchWindowMemDC, TextureCrossPen);
	SetROP2(StitchWindowMemDC, R2_COPYPEN);
	line[0].y = 0;
	line[1].y = StitchWindowClientRect.bottom;
	for (size_t iVertical = 1; iVertical < TextureScreen.lines + 1u; iVertical++) {
		line[0].x = line[1].x = (iVertical * TextureScreen.spacing + TextureScreen.xOffset) / TextureScreen.editToPixelRatio;
		Polyline(StitchWindowMemDC, line, 2);
	}
	line[0].x = 0;
	line[1].x = StitchWindowClientRect.right;
	line[0].y = line[1].y = TextureScreen.top;
	Polyline(StitchWindowMemDC, line, 2);
	line[0].y = line[1].y = TextureScreen.bottom;
	Polyline(StitchWindowMemDC, line, 2);
	DeleteObject(TextureCrossPen);
	TextureCrossPen = CreatePen(PS_SOLID, 1, 0xffffff);
	SelectObject(StitchWindowMemDC, TextureCrossPen);
	SetROP2(StitchWindowMemDC, R2_XORPEN);
	for (size_t index = 0; index < TempTexturePoints->size(); index++) {
		texturePriv::dutxtx(index, IniFile.textureEditorSize);
	}
	if (SelectedTexturePointsList->size()) {
		texturePriv::txrct2rct(TextureRect, TexturePixelRect);
		line[0].y = line[1].y = TexturePixelRect.top;
		line[0].x             = TexturePixelRect.left;
		line[1].x             = TexturePixelRect.right;
		Polyline(StitchWindowMemDC, line, 2);
		line[1].y = TexturePixelRect.bottom;
		line[1].x = TexturePixelRect.left;
		Polyline(StitchWindowMemDC, line, 2);
		line[0].x = TexturePixelRect.right;
		line[0].y = TexturePixelRect.bottom;
		Polyline(StitchWindowMemDC, line, 2);
		line[1].x = TexturePixelRect.right;
		line[1].y = TexturePixelRect.top;
		Polyline(StitchWindowMemDC, line, 2);
	}
	for (size_t iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
		texturePriv::dutxtx((*SelectedTexturePointsList)[iPoint], IniFile.textureEditorSize);
		texturePriv::dutxtx((*SelectedTexturePointsList)[iPoint], IniFile.textureEditorSize << 1);
	}
	BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
	displayText::drwtxbut(TextureScreen);
}

bool texturePriv::px2txt(const POINT& offset) {
	fPOINT editPoint;

	texturePriv::px2ed(offset, editPoint);
	TXPNT tmp = { 0, gsl::narrow<unsigned short>(floor((editPoint.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5)) };
	if (tmp.line > TextureScreen.lines)
		return false;
	if (tmp.line < 1)
		return false;
	if (offset.y > TextureScreen.top) {
		if (offset.y > TextureScreen.bottom)
			return false;
		else {
			tmp.y = TextureScreen.areaHeight
			        - (static_cast<float>(offset.y - TextureScreen.top) / TextureScreen.height * TextureScreen.areaHeight);
			TempTexturePoints->push_back(tmp);
			return true;
		}
	}
	else {
		return false;
	}
}

void texturePriv::deorg(POINT& point) noexcept {
	point.x = Msg.pt.x - StitchWindowOrigin.x;
	point.y = Msg.pt.y - StitchWindowOrigin.y;
}

bool texturePriv::txbutfn() {
	POINT offset;

	texturePriv::deorg(offset);
	return texturePriv::px2txt(offset);
}

void texture::rstxt() {
	SelectedTexturePointsList->clear();
	StateMap.reset(StateFlag::TXTMOV);
	StateMap.reset(StateFlag::BZUM);
	StateMap.reset(StateFlag::BZUMIN);
	StateMap.set(StateFlag::RESTCH);
	StateMap.reset(StateFlag::POLIMOV);
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	StateMap.set(StateFlag::RESTCH);
}

void texture::txtrbut() {
	if (texturePriv::txbutfn()) {
		texture::savtxt();
		StateMap.set(StateFlag::RESTCH);
	}
	else
		texture::rstxt();
}

bool texturePriv::txtclos(size_t& closestTexturePoint) {
	if (closestTexturePoint) {
		auto  minimumLength = 1e99;
		POINT reference = {}, point = {};

		texturePriv::deorg(reference);
		closestTexturePoint = 0;
		for (size_t iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
			texturePriv::txt2pix((*TempTexturePoints)[iPoint], point);
			const auto length = hypot(point.x - reference.x, point.y - reference.y);
			if (length < minimumLength) {
				minimumLength       = length;
				closestTexturePoint = iPoint;
			}
		}
		if (minimumLength < CLOSENUF)
			return true;
	}
	return false;
}

void texturePriv::setxmov() {
	StateMap.set(StateFlag::TXTMOV);
	TextureCursorLocation.x = SelectTexturePointsOrigin.x = Msg.pt.x - StitchWindowOrigin.x;
	TextureCursorLocation.y = SelectTexturePointsOrigin.y = Msg.pt.y - StitchWindowOrigin.y;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void texturePriv::ritxrct() noexcept {
	POINT offset
	    = { (TextureCursorLocation.x - SelectTexturePointsOrigin.x), (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };
	RECT  rectangle = { (TexturePixelRect.left + offset.x),
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

void texturePriv::dutxrct(TXTRCT& textureRect) {
	if (SelectedTexturePointsList->size()) {
		auto texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[0]];
		textureRect.left = textureRect.right = texturePoint->line;
		textureRect.top = textureRect.bottom = texturePoint->y;
		for (size_t iPoint = 1; iPoint < SelectedTexturePointsList->size(); iPoint++) {
			texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
			if (texturePoint->y > textureRect.top)
				textureRect.top = texturePoint->y;
			if (texturePoint->y < textureRect.bottom)
				textureRect.bottom = texturePoint->y;
			if (texturePoint->line < textureRect.left)
				textureRect.left = texturePoint->line;
			if (texturePoint->line > textureRect.right)
				textureRect.right = texturePoint->line;
		}
	}
	else
		textureRect.left = textureRect.right = textureRect.top = textureRect.bottom = 0;
}

fPOINT texturePriv::ed2stch(const fPOINT& point) noexcept {
	return { (point.x - TextureScreen.xOffset), (point.y - TextureScreen.yOffset) };
}

void texturePriv::dutxlin(const fPOINT& point0in, const fPOINT& point1in) {
	const auto point0 = texturePriv::ed2stch(point0in);
	const auto point1 = texturePriv::ed2stch(point1in);

	const auto deltaX = point1.x - point0.x;
	if (fabs(deltaX) < TINY)
		return;
	const auto slope  = (point1.y - point0.y) / deltaX;
	auto       start  = point0.x;
	auto       finish = point1.x;
	if (start > finish) {
		std::swap(start, finish);
	}
	int integerStart  = ceil(start / TextureScreen.spacing);
	int integerFinish = floor(finish / TextureScreen.spacing);
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

void texturePriv::setxclp() {
	POINT  screenOffset = {};
	fPOINT editorOffset = {};

	texturePriv::deorg(screenOffset);
	texturePriv::px2ed(screenOffset, editorOffset);
	if (StateMap.testAndReset(StateFlag::TXHCNTR))
		editorOffset.x = (TextureScreen.lines * TextureScreen.spacing) / 2 + TextureScreen.xOffset - TextureScreen.formCenter.x
		                 + TextureScreen.spacing / 2;
	else
		editorOffset.x -= TextureScreen.formCenter.x;
	editorOffset.y -= TextureScreen.formCenter.y;
	for (size_t iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x += editorOffset.x;
		AngledFormVertices[iVertex].y += editorOffset.y;
	}
	auto lineCount = AngledForm.vertexCount - 1;
	if (AngledForm.type != FRMLINE)
		lineCount++;
	VertexCount        = AngledForm.vertexCount;
	size_t iNextVertex = 0;
	for (size_t iLine = 0u; iLine < lineCount; iLine++) {
		iNextVertex = nxt(iLine);
		texturePriv::dutxlin(AngledFormVertices[iLine], AngledFormVertices[iNextVertex]);
	}
}

void texturePriv::stxlin() {
	POINT  offset;
	fPOINT point0;
	fPOINT point1;

	StateMap.reset(StateFlag::TXTMOV);
	texturePriv::deorg(offset);
	texturePriv::px2ed(offset, point1);
	texturePriv::px2ed(FormLines[0], point0);
	texturePriv::dutxlin(point0, point1);
	StateMap.set(StateFlag::RESTCH);
}

void texturePriv::ed2txp(const POINT& offset, TXPNT& textureRecord) noexcept {
	fPOINT point;

	texturePriv::px2ed(offset, point);
	textureRecord.line = (point.x - TextureScreen.xOffset) / TextureScreen.spacing + 0.5;
	textureRecord.y    = TextureScreen.areaHeight
	                  - (static_cast<float>(offset.y - TextureScreen.top) / TextureScreen.height * TextureScreen.areaHeight);
}

void texture::txtrup() {
	POINT offset = {};
	if (StateMap.testAndReset(StateFlag::TXTMOV)) {
		TXPNT textureOffset = {};
		texture::savtxt();
		texturePriv::deorg(offset);
		offset.x -= SelectTexturePointsOrigin.x;
		offset.y -= SelectTexturePointsOrigin.y;
		const auto Xmagnitude = abs(offset.x);
		textureOffset.line    = std::round(Xmagnitude * TextureScreen.editToPixelRatio / TextureScreen.spacing);
		if (offset.x < 0)
			textureOffset.line = -textureOffset.line;
		textureOffset.y = (-offset.y) / TextureScreen.height * TextureScreen.areaHeight;
		auto yOffset    = TextureRect.top + textureOffset.y - TextureScreen.areaHeight;
		if (yOffset > 0)
			textureOffset.y -= yOffset;
		yOffset = TextureRect.bottom + textureOffset.y;
		if (yOffset < 0)
			textureOffset.y -= yOffset;
		auto xCoord = TextureRect.left + textureOffset.line - 1;
		if (xCoord < 0)
			textureOffset.line -= xCoord;
		xCoord = TextureRect.right + textureOffset.line - TextureScreen.lines;
		if (xCoord > 0)
			textureOffset.line -= xCoord;
		for (size_t iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
			auto texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
			texturePoint->line += textureOffset.line;
			texturePoint->y += textureOffset.y;
		}
		texturePriv::dutxrct(TextureRect);
	}
	else {
		if (StateMap.testAndReset(StateFlag::BZUMIN)) {
			TXPNT lowestTexturePoint  = {};
			TXPNT highestTexturePoint = {};
			texturePriv::deorg(offset);
			texturePriv::ed2txp(ZoomBoxLine[0], highestTexturePoint);
			texturePriv::ed2txp(offset, lowestTexturePoint);
			if (highestTexturePoint.line < lowestTexturePoint.line) {
				std::swap(highestTexturePoint.line, lowestTexturePoint.line);
			}
			if (highestTexturePoint.y < lowestTexturePoint.y) {
				std::swap(highestTexturePoint.y, lowestTexturePoint.y);
			}
			SelectedTexturePointsList->clear();
			for (size_t iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
				if ((*TempTexturePoints)[iPoint].y < highestTexturePoint.y
				    && (*TempTexturePoints)[iPoint].y > lowestTexturePoint.y
				    && (*TempTexturePoints)[iPoint].line <= highestTexturePoint.line
				    && (*TempTexturePoints)[iPoint].line >= lowestTexturePoint.line) {
					SelectedTexturePointsList->push_back(iPoint);
				}
			}
			texturePriv::dutxrct(TextureRect);
		}
	}
	StateMap.set(StateFlag::RESTCH);
}

void texturePriv::angrct(fRECTANGLE& rectangle) noexcept {
	rectangle.left = rectangle.right = AngledFormVertices[0].x;
	rectangle.bottom = rectangle.top = AngledFormVertices[0].y;
	for (size_t iVertex = 1; iVertex < AngledForm.vertexCount; iVertex++) {
		if (AngledFormVertices[iVertex].x < rectangle.left)
			rectangle.left = AngledFormVertices[iVertex].x;
		if (AngledFormVertices[iVertex].x > rectangle.right)
			rectangle.right = AngledFormVertices[iVertex].x;
		if (AngledFormVertices[iVertex].y > rectangle.top)
			rectangle.top = AngledFormVertices[iVertex].y;
		if (AngledFormVertices[iVertex].y < rectangle.bottom)
			rectangle.bottom = AngledFormVertices[iVertex].y;
	}
}

void texturePriv::ritxfrm() {
	POINT offset
	    = { (TextureCursorLocation.x - SelectTexturePointsOrigin.x), (TextureCursorLocation.y - SelectTexturePointsOrigin.y) };

	offset.x = TextureCursorLocation.x - SelectTexturePointsOrigin.x;
	offset.y = TextureCursorLocation.y - SelectTexturePointsOrigin.y;
	for (size_t iVertex = 0u; iVertex < AngledForm.vertexCount; iVertex++) {
		texturePriv::ed2px(AngledFormVertices[iVertex], FormLines[iVertex]);
		FormLines[iVertex].x += offset.x;
		FormLines[iVertex].y += offset.y;
	}
	FormLines[AngledForm.vertexCount] = FormLines[0];
	auto vertexCount                  = AngledForm.vertexCount;
	if (AngledForm.type != FRMLINE)
		vertexCount++;
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	PolylineInt(StitchWindowDC, FormLines, vertexCount);
}

void texturePriv::setxfrm() noexcept {
	fRECTANGLE angleRect = {};

	texturePriv::angrct(angleRect);
	for (size_t iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x -= angleRect.left;
		AngledFormVertices[iVertex].y -= angleRect.bottom;
	}
	texturePriv::angrct(angleRect);
	const auto height = angleRect.top - angleRect.bottom;
	if (height > TextureScreen.areaHeight) {
		const auto ratio = TextureScreen.areaHeight / height * 0.95;
		for (size_t iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
			AngledFormVertices[iVertex].x *= ratio;
			AngledFormVertices[iVertex].y *= ratio;
		}
		texturePriv::angrct(angleRect);
	}
	TextureScreen.formCenter.x = midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = midl(angleRect.top, angleRect.bottom);
	texturePriv::ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
}

void texturePriv::txtclp() {
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
				texturePriv::setxfrm();
				TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
				TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			}
			GlobalUnlock(ClipMemory);
		}
	}
	StateMap.set(StateFlag::RESTCH);
	StateMap.reset(StateFlag::WASWROT);
}

void texturePriv::dutxtlin() noexcept {
	SetROP2(StitchWindowDC, R2_NOTXORPEN);
	Polyline(StitchWindowDC, FormLines, 2);
}

void texture::txtrmov() {
	if (StateMap.test(StateFlag::TXTLIN)) {
		texturePriv::dutxtlin();
		texturePriv::deorg(FormLines[1]);
		texturePriv::dutxtlin();
		return;
	}
	if (StateMap.test(StateFlag::TXTCLP)) {
		if (StateMap.testAndSet(StateFlag::WASWROT))
			texturePriv::ritxfrm();
		TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
		TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
		texturePriv::ritxfrm();
	}
	else {
		if (SelectedTexturePointsList->size()) {
			texturePriv::ritxrct();
			TextureCursorLocation.x = Msg.pt.x - StitchWindowOrigin.x;
			TextureCursorLocation.y = Msg.pt.y - StitchWindowOrigin.y;
			texturePriv::ritxrct();
		}
	}
}

void texturePriv::txtlin() {
	texturePriv::deorg(FormLines[0]);
	texturePriv::deorg(FormLines[1]);
	StateMap.reset(StateFlag::TXTCLP);
	StateMap.set(StateFlag::TXTLIN);
	StateMap.set(StateFlag::TXTMOV);
}

void texturePriv::chktxnum() {
	float  value    = 0.0f;
	size_t lastChar = 0;

	if (TextureInputBuffer->size()) {
		value = stof(*TextureInputBuffer, &lastChar);
	}
	if (value) {
		TextureInputBuffer->clear();
		value *= PFGRAN;
		switch (TextureWindowId) {
		case HTXHI:
			texture::savtxt();
			TextureScreen.areaHeight = value;
			IniFile.textureHeight    = value;
			StateMap.set(StateFlag::CHKTX);
			break;
		case HTXWID:
			texture::savtxt();
			TextureScreen.width  = value;
			IniFile.textureWidth = value;
			StateMap.set(StateFlag::CHKTX);
			break;
		case HTXSPAC:
			texture::savtxt();
			TextureScreen.spacing  = value;
			IniFile.textureSpacing = value;
			TextureScreen.width    = value * TextureScreen.lines + value / 2;
			StateMap.set(StateFlag::CHKTX);
			break;
		}
	}
	DestroyWindow(SideWindowButton);
	SideWindowButton = 0;
	StateMap.set(StateFlag::RESTCH);
}

void texturePriv::butsid(unsigned windowId) {
	RECT buttonRect = {};

	texturePriv::chktxnum();
	TextureWindowId = windowId;
	GetWindowRect((*ButtonWin)[windowId], &buttonRect);
	SideWindowButton = CreateWindow(L"STATIC",
	                                0,
	                                SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
	                                buttonRect.left + ButtonWidthX3 - StitchWindowOrigin.x,
	                                buttonRect.top - StitchWindowOrigin.y,
	                                ButtonWidthX3,
	                                ButtonHeight,
	                                MainStitchWin,
	                                NULL,
	                                ThrEdInstance,
	                                NULL);
}

bool texturePriv::txcmp(const TXPNT& texturePoint0, const TXPNT& texturePoint1) noexcept {
	if (texturePoint0.line == texturePoint1.line) {
		return (texturePoint0.y < texturePoint1.y);
	}
	else {
		return (texturePoint0.line < texturePoint1.line);
	}
}

void texturePriv::txpar() {
	SelectedForm->type                       = FRMFPOLY;
	SelectedForm->fillInfo.texture.lines     = TextureScreen.lines;
	SelectedForm->fillInfo.texture.height    = TextureScreen.areaHeight;
	SelectedForm->fillSpacing                = TextureScreen.spacing;
	SelectedForm->lengthOrCount.stitchLength = IniFile.userStitchLength;
	SelectedForm->maxFillStitchLen           = IniFile.maxStitchLength;
	SelectedForm->minFillStitchLen           = IniFile.minStitchLength;
	SelectedForm->fillColor                  = ActiveColor;
	refilfn();
}

void texturePriv::txvrt() {
	if (TempTexturePoints->size()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXVRTF;
			texturePriv::txpar();
		}
	}
}

void texturePriv::txhor() {
	if (TempTexturePoints->size()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType = TXHORF;
			texturePriv::txpar();
		}
	}
}

void texturePriv::txang() {
	if (TempTexturePoints->size()) {
		if (StateMap.test(StateFlag::FORMSEL)) {
			fvars(ClosestFormToCursor);
			SelectedForm->fillType              = TXANGF;
			SelectedForm->angleOrClipData.angle = static_cast<float>(IniFile.fillAngle);
			texturePriv::txpar();
		}
	}
}

void texture::deltx() {
	bool flag = false;

	const auto currentIndex = FormList[ClosestFormToCursor].fillInfo.texture.index;

	if (TextureIndex && texture::istx(ClosestFormToCursor) && SelectedForm->fillInfo.texture.count) {
		// First check to see if the texture is shared between forms
		for (size_t iForm = 0; iForm < ClosestFormToCursor; iForm++) {
			if (texture::istx(iForm)) {
				if (FormList[iForm].fillInfo.texture.index == currentIndex) {
					flag = true;
				}
			}
		}
		for (size_t iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
			if (texture::istx(iForm)) {
				if (FormList[iForm].fillInfo.texture.index == currentIndex) {
					flag = true;
				}
			}
		}
		// Only if it is not shared, should the texture be deleted
		if (!flag) {
			std::vector<TXPNT> textureBuffer;
			textureBuffer.reserve(TextureIndex);
			unsigned short iBuffer = 0;
			for (size_t iForm = 0; iForm < ClosestFormToCursor; iForm++) {
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
			for (size_t iForm = ClosestFormToCursor + 1; iForm < FormIndex; iForm++) {
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

void texturePriv::nutx() {
	unsigned index = 0;

	std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), texturePriv::txcmp);
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
	if (TempTexturePoints->size()) {
		const auto tempPointCount = TempTexturePoints->size();
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
			if (texture::istx(iForm))
				FormList[iForm].fillInfo.texture.index += gsl::narrow<unsigned short>(tempPointCount);
		}
		auto _ = std::copy(TempTexturePoints->cbegin(), TempTexturePoints->cend(), startSource);

		SelectedForm->fillInfo.texture.index = index;
		SelectedForm->fillInfo.texture.count = gsl::narrow<unsigned short>(tempPointCount);
	}
}

// Ensure all lines in the texture have at least 1 point
void texturePriv::altx() {
	boost::dynamic_bitset<> txtLines(TextureScreen.lines + 1);
	if (StateMap.test(StateFlag::FORMSEL)) {
		const auto halfHeight = TextureScreen.areaHeight / 2.0f;
		for (size_t iPoint = 0; iPoint < TempTexturePoints->size(); iPoint++) {
			txtLines.set((*TempTexturePoints)[iPoint].line);
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
	redraw((*ButtonWin)[HHID]);
	if (StateMap.test(StateFlag::UPTO))
		displayText::butxt(HUPTO, (*StringTable)[STR_UPON]);
	else
		displayText::butxt(HUPTO, (*StringTable)[STR_UPOF]);
	SetWindowText((*ButtonWin)[HTXSPAC], L"");
	texture::savtxt();
	zumhom();
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

void texturePriv::dutxfn(unsigned textureType) {
	if (StateMap.test(StateFlag::FORMSEL)) {
		texturePriv::altx();
		clip::delmclp(ClosestFormToCursor);
		satin::delsac(ClosestFormToCursor);
		texture::deltx();
		texture::savtxt();
		texturePriv::nutx();
		dusqr();
		switch (textureType) {
		case VRTYP:
			texturePriv::txvrt();
			break;
		case HORTYP:
			texturePriv::txhor();
			break;
		case ANGTYP:
			texturePriv::txang();
			break;
		}
	}
	texture::txof();
	texture::rstxt();
}

void texturePriv::dutxmir() {
	const auto centerLine = (TextureScreen.lines + 1u) >> 1;

	texture::savtxt();
	std::sort(TempTexturePoints->begin(), TempTexturePoints->end(), texturePriv::txcmp);

	auto iPoint = TempTexturePoints->size() - 1;
	while ((*TempTexturePoints)[iPoint].line > centerLine && iPoint >= 0)
		iPoint--;
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
	for (size_t index = 0; index < iMirrorPoint; index++) {
		TempTexturePoints->push_back({ (*TempTexturePoints)[index].y,
		                               gsl::narrow<unsigned short>(TextureScreen.lines - (*TempTexturePoints)[index].line + 1) });
	}
	StateMap.set(StateFlag::RESTCH);
}

void texturePriv::txdelal() {
	texturePriv::chktxnum();
	texture::savtxt();
	TempTexturePoints->clear();
	texture::rstxt();
	StateMap.set(StateFlag::RESTCH);
}

bool texturePriv::chkbut() {
	if (Msg.hwnd == (*ButtonWin)[HTXCLR]) {
		texturePriv::txdelal();
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXHI]) {
		texturePriv::butsid(HTXHI);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXWID]) {
		texturePriv::butsid(HTXWID);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXSPAC]) {
		texturePriv::butsid(HTXSPAC);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXVRT]) {
		texturePriv::dutxfn(VRTYP);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXHOR]) {
		texturePriv::dutxfn(HORTYP);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXANG]) {
		texturePriv::dutxfn(ANGTYP);
		return 1;
	}
	if (Msg.hwnd == (*ButtonWin)[HTXMIR]) {
		texturePriv::dutxmir();
		return 1;
	}
	return 0;
}

void texture::txtlbut() {
	fvars(ClosestFormToCursor);
	if (texturePriv::chkbut())
		return;
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		texture::savtxt();
		texturePriv::setxclp();
		return;
	}
	if (StateMap.testAndReset(StateFlag::TXTLIN)) {
		texture::savtxt();
		texturePriv::stxlin();
		return;
	}
	if (SelectedTexturePointsList->size()) {
		POINT point = {};
		texturePriv::deorg(point);
		if (point.x > TexturePixelRect.left && point.x < TexturePixelRect.right && point.y > TexturePixelRect.top
		    && point.y < TexturePixelRect.bottom) {
			texturePriv::setxmov();
			texturePriv::ritxrct();
			return;
		}
	}
	if (SelectedTexturePointsList->size()) {
		if (texturePriv::txtclos((*SelectedTexturePointsList)[0])) {
			SelectedTexturePointsList->resize(1);
			texturePriv::setxmov();
			texturePriv::dutxrct(TextureRect);
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

void texturePriv::txbak() {
	if (StateMap.test(StateFlag::WASTXBAK)) {
		SelectedTexturePointsList->clear();
		bool flag = false;
		for (size_t iHistory = 0; iHistory < ITXBUFLEN; iHistory++) {
			if (TextureHistory[TextureHistoryIndex].width) {
				flag = true;
				break;
			}
			texturePriv::txrbak();
		}
		if (flag) {
			texturePriv::redtbak();
			texturePriv::txrbak();
		}
	}
}

void texturePriv::nxbak() {
	if (StateMap.test(StateFlag::WASTXBAK)) {
		bool flag = false;
		for (size_t iHistory = 0; iHistory < ITXBUFLEN; iHistory++) {
			texturePriv::txrfor();
			if (TextureHistory[TextureHistoryIndex].width) {
				flag = true;
				break;
			}
		}
		if (flag) {
			texturePriv::redtbak();
		}
	}
}

void texturePriv::txtdel() {
	if (SelectedTexturePointsList->size()) {
		texture::savtxt();
		boost::dynamic_bitset<> texturePointsMap(TempTexturePoints->size());
		for (auto& p : *SelectedTexturePointsList) {
			texturePointsMap.set(p);
		}
		// Another potential pattern for this:
		// myVector.erase(remove_if(myVector.begin(), myVector.end(), testFunction), myVector.end());
		std::vector<TXPNT> tmpTexture;
		tmpTexture.reserve(TempTexturePoints->size() - SelectedTexturePointsList->size());
		for (size_t iSourcePoint = 0; iSourcePoint < TempTexturePoints->size(); iSourcePoint++) {
			if (!texturePointsMap.test(iSourcePoint)) {
				tmpTexture.push_back((*TempTexturePoints)[iSourcePoint]);
			}
		}
		*TempTexturePoints = tmpTexture;
		SelectedTexturePointsList->clear();
		StateMap.set(StateFlag::RESTCH);
		return;
	}
	size_t iClosestPoint = 0;
	if (TempTexturePoints->size() && texturePriv::txtclos(iClosestPoint)) {
		auto it = TempTexturePoints->begin();
		std::advance(it, iClosestPoint);
		it = TempTexturePoints->erase(it);
		StateMap.set(StateFlag::RESTCH);
	}
}

void texturePriv::txcntrv() {
	if (StateMap.testAndReset(StateFlag::TXTCLP)) {
		StateMap.set(StateFlag::TXHCNTR);
		texture::savtxt();
		texturePriv::setxclp();
		StateMap.set(StateFlag::RESTCH);
	}
}

void texturePriv::txsiz(double ratio) {
	texturePriv::ritxfrm();
	for (size_t iVertex = 0; iVertex < AngledForm.vertexCount; iVertex++) {
		AngledFormVertices[iVertex].x *= ratio;
		AngledFormVertices[iVertex].y *= ratio;
	}
	fRECTANGLE angleRect = {};
	texturePriv::angrct(angleRect);
	TextureScreen.formCenter.x = midl(angleRect.right, angleRect.left);
	TextureScreen.formCenter.y = midl(angleRect.top, angleRect.bottom);
	texturePriv::ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
	texturePriv::ritxfrm();
}

void texturePriv::txshrnk() {
	texturePriv::txsiz(TXTRAT);
}

void texturePriv::txgro() {
	texturePriv::txsiz(1 / TXTRAT);
}

bool texturePriv::txdig(unsigned keyCode, char& character) {
	if (isdigit(keyCode)) {
		character = gsl::narrow<char>(keyCode);
		return 1;
	}
	if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
		character = gsl::narrow<char>(keyCode) - VK_NUMPAD0 + 0x30;
		return 1;
	}
	if (keyCode == 0xbe || keyCode == 0x6e) {
		character = '.';
		return 1;
	}
	return 0;
}

void texturePriv::txnudg(int deltaX, float deltaY) {
	if (SelectedTexturePointsList->size()) {
		if (deltaY) {
			const auto screenDeltaY = deltaY * TextureScreen.editToPixelRatio;
			for (size_t iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
				const auto yCoord = (*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].y + screenDeltaY;
				if (yCoord < 0.0)
					return;
				if (yCoord > TextureScreen.areaHeight)
					return;
			}
			for (size_t iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++)
				(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].y += screenDeltaY;
		}
		else {
			for (size_t iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++) {
				const auto textureLine = (*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].line + deltaX;
				if (textureLine < 1)
					return;
				if (textureLine > TextureScreen.lines)
					return;
			}
			for (size_t iPoint = 0; iPoint < SelectedTexturePointsList->size(); iPoint++)
				(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]].line += deltaX;
		}
	}
	texturePriv::dutxrct(TextureRect);
	StateMap.set(StateFlag::RESTCH);
}

void texture::txsnap() {
	const auto txpntSize = TempTexturePoints->size();

	if (txpntSize) {
		texture::savtxt();
		const auto halfGrid = IniFile.gridSize / 2;
		if (SelectedTexturePointsList->size()) {
			for (size_t iPoint = 0; iPoint < txpntSize; iPoint++) {
				auto       texturePoint = &(*TempTexturePoints)[(*SelectedTexturePointsList)[iPoint]];
				const auto yStep        = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y         = yStep * IniFile.gridSize;
			}
		}
		else {
			for (size_t iPoint = 0; iPoint < txpntSize; iPoint++) {
				auto       texturePoint = &(*TempTexturePoints)[iPoint];
				const auto yStep        = (texturePoint->y + halfGrid) / IniFile.gridSize;
				texturePoint->y         = yStep * IniFile.gridSize;
			}
		}
		StateMap.set(StateFlag::RESTCH);
	}
}

void tst() {
	DesignerName->assign(L"Coder");
	ThrName->assign(*DesignerName);
	StateMap.set(StateFlag::RESTCH);
}

void texture::txtkey(unsigned keyCode) {
	char character = {};
	bool flag      = true;

	if (SideWindowButton) {
		switch (keyCode) {
		case VK_RETURN:
			texturePriv::chktxnum();
			return;
		case VK_ESCAPE:
			texture::rstxt();
			StateMap.set(StateFlag::RESTCH);
			break;
		case 'Q':
			texture::rstxt();
			StateMap.set(StateFlag::RESTCH);
			break;
		case 8: // backspace
			if (TextureInputBuffer->size())
				TextureInputBuffer->pop_back();
			flag = false;
			break;
		}
		if (flag) {
			if (TextureInputBuffer->size() < 8) { // floating point 7 digits of precision + '.'
				if (texturePriv::txdig(keyCode, character)) {
					TextureInputBuffer->push_back(character);
				}
			}
		}
		SetWindowText(SideWindowButton, TextureInputBuffer->c_str());
		return;
	}
	switch (keyCode) {
	case VK_ESCAPE:
		texture::txof();
	case 'Q':
		texture::rstxt();
		StateMap.set(StateFlag::RESTCH);
		break;
	case 0xdb: //[
		texturePriv::txshrnk();
		break;
	case 0xdd: //]
		texturePriv::txgro();
		break;
	case 192: //`
		tst();
		break;
	case 'R':
		texturePriv::dutxfn(VRTYP);
		break;
	case 'A':
		texturePriv::dutxfn(ANGTYP);
		break;
	case 'H':
		texturePriv::dutxfn(HORTYP);
		break;
	case 'E':
		texturePriv::txtlin();
		break;
	case 'Z':
	case 'B':
		if (!StateMap.testAndSet(StateFlag::LASTXBAK)) {
			texture::savtxt();
			texturePriv::txrbak();
		}
		else {
			if (StateMap.testAndReset(StateFlag::TXBDIR))
				texturePriv::txrbak();
		}
		texturePriv::txbak();
		return;
	case 'V':
		if (OpenClipboard(ThrEdWindow))
			texturePriv::txtclp();
		break;
	case 'N':
		StateMap.set(StateFlag::LASTXBAK);
		if (!StateMap.testAndSet(StateFlag::TXBDIR))
			texturePriv::txrfor();
		texturePriv::nxbak();
		return;
	case 'D':
	case VK_DELETE:
		if (GetKeyState(VK_SHIFT) & GetKeyState(VK_CONTROL) & 0X8000)
			texturePriv::txdelal();
		else
			texturePriv::txtdel();
		break;
	case 0xbd: //-
		texturePriv::txcntrv();
		break;
	case VK_LEFT:
		texturePriv::txnudg(-1, 0);
		break;
	case VK_RIGHT:
		texturePriv::txnudg(1, 0);
		break;
	case VK_UP:
		texturePriv::txnudg(0, IniFile.cursorNudgeStep);
		break;
	case VK_DOWN:
		texturePriv::txnudg(0, -IniFile.cursorNudgeStep);
		break;
	case 'S':
		texture::txsnap();
		break;
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
			const auto currentPoint = TexturePointsBuffer->at(currentIndex + iTexturePoint);
			if (currentPoint.line) {
				const auto iSegment = currentPoint.line - 1;

				textureSegments[iSegment].line = iTexturePoint;
				textureSegments[iSegment].stitchCount++;
			}
		}
	}
}

void texture::rtrtx() {
	fvars(ClosestFormToCursor);
	TempTexturePoints->clear();

	const auto currentIndex = SelectedForm->fillInfo.texture.index;
	if (TexturePointsBuffer->size() > gsl::narrow<size_t>(currentIndex)) {
		auto currentCount = SelectedForm->fillInfo.texture.count;
		if (TexturePointsBuffer->size() < gsl::narrow<size_t>(currentIndex + currentCount)) {
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

bool texturePriv::inrct(const fRECTANGLE& rectangle, const fPOINTATTR& stitch) noexcept {
	if (stitch.x < rectangle.left)
		return 0;
	if (stitch.x > rectangle.right)
		return 0;
	if (stitch.y < rectangle.bottom)
		return 0;
	if (stitch.y > rectangle.top)
		return 0;
	return 1;
}

void texture::setshft() {
	fRECTANGLE selectionRect = {};

	texture::savtxt();
	unbBox();
	StateMap.reset(StateFlag::BZUMIN);
	pxCor2stch(ZoomBoxLine[0]);
	selectionRect.top  = SelectedPoint.y;
	selectionRect.left = SelectedPoint.x;
	pxCor2stch(ZoomBoxLine[2]);
	selectionRect.bottom = SelectedPoint.y;
	selectionRect.right  = SelectedPoint.x;
	StateMap.reset(StateFlag::TXIN);
	auto line = 1u;
	for (size_t iStitch = 0; iStitch < PCSHeader.stitchCount; iStitch++) {
		if (texturePriv::inrct(selectionRect, StitchBuffer[iStitch])) {
			StateMap.set(StateFlag::TXIN);
			TempTexturePoints->push_back({ (StitchBuffer[iStitch].y - selectionRect.bottom), gsl::narrow<unsigned short>(line) });
		}
		else {
			if (StateMap.testAndReset(StateFlag::TXIN))
				line++;
		}
	}
	if (TempTexturePoints->size()) {
		line = TempTexturePoints->back().line;
	}
	TextureScreen.spacing    = (selectionRect.right - selectionRect.left) / line;
	TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
	TextureScreen.width      = TextureScreen.spacing * line + TextureScreen.spacing / 2;
	StateMap.set(StateFlag::TXTRED);
	StateMap.set(StateFlag::RESTCH);
}

void texture::writeScreenWidth(unsigned position) {
	std::wstring fmtStr;
	displayText::loadString(fmtStr, IDS_TXWID);
	std::wstring scrWidth(fmt::format(fmtStr, (TextureScreen.width / PFGRAN)));
	TextOutInt(DrawItem->hDC, position, 1, scrWidth.c_str(), scrWidth.size());
}