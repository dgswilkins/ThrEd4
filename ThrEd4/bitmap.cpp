#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Local Headers
#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "trace.h"
#include "thred.h"
#include "bitmap.h"

namespace bi = bitmap::internal;

COLORREF  BitmapBackgroundColors[16]; // for the bitmap color dialog box
uint32_t  BitmapColor = BITCOL;       // bitmap color
HDC       BitmapDC;                   // bitmap device context
RECT      BitmapDstRect;              // stitch window destination rectangle for zooomed view
HANDLE    BitmapFileHandle;           // bitmap handle
uint32_t  BitmapHeight;               // bitmap height
HPEN      BitmapPen;                  // bitmap pen
POINT     BitmapPoint;                // a point on the bitmap
fPOINT    BitmapSizeinStitches;       // bitmap end points in stitch points
RECT      BitmapSrcRect;              // bitmap source rectangle for zoomed view
uint32_t  BitmapWidth;                // bitmap width
fPOINT    BmpStitchRatio;             // bitmap to stitch hoop ratios
fs::path* DefaultBMPDirectory;
fs::path* UserBMPFileName; // bitmap file name from user load

BITMAPFILEHEADER BitmapFileHeader;   // bitmap file header
BITMAPV4HEADER   BitmapFileHeaderV4; // bitmap version4 file header
BITMAPINFO       BitmapInfo;         // bitmap info
BITMAPINFOHEADER BitmapInfoHeader;   // bitmap info header

wchar_t const BmpFilter[_MAX_PATH + 1]       = L"Microsoft (BMP)\0*.bmp\0";
wchar_t       CustomBmpFilter[_MAX_PATH + 1] = L"Thredworks (THR)\0*.thr\0";

CHOOSECOLOR BitMapColorStruct;

OPENFILENAME OpenBitmapName = {
    sizeof(OpenBitmapName),      // lStructsize
    nullptr,                     // hwndOwner
    nullptr,                     // hInstance
    std::begin(BmpFilter),       // lpstrFilter
    std::begin(CustomBmpFilter), // lpstrCustomFilter
    _MAX_PATH,                   // nMaxCustFilter
    0,                           // nFilterIndex
    nullptr,                     // lpstrFile
    _MAX_PATH,                   // nMaxFile
    nullptr,                     // lpstrFileTitle
    0,                           // nMaxFileTitle
    nullptr,                     // lpstrInitialDir
    nullptr,                     // lpstrTitle
    OFN_OVERWRITEPROMPT,         // Flags
    0,                           // nFileOffset
    0,                           // nFileExtension
    L"bmp",                      // lpstrDefExt
    0,                           // lCustData
    nullptr,                     // lpfnHook
    nullptr,                     // lpTemplateName
#ifdef _MAC
    nullptr, // lpEditInfo
    0U,      // lpstrPrompt
#endif
#if (_WIN32_WINNT >= 0x0500)
    nullptr, // *pvReserved
    0U,      // dwReserved
    0U       // FlagsEx
#endif
};

COLORREF const DefaultBitmapBackgroundColors[] = {0x00c0d5bf,
                                                  0x00c8dfee,
                                                  0x00708189,
                                                  0x00a5a97a,
                                                  0x00b8d6fe,
                                                  0x008a8371,
                                                  0x004b6cb8,
                                                  0x009cdcc2,
                                                  0x00366d39,
                                                  0x00dcfcfb,
                                                  0x003c4f75,
                                                  0x0095b086,
                                                  0x00c9dcba,
                                                  0x0043377b,
                                                  0x00b799ae,
                                                  0x0054667a};

auto bitmap::internal::fswap(COLORREF color) noexcept -> COLORREF {
  // ToDo - find a better option than _byteswap
  return _byteswap_ulong(color) >> 8U;
}

auto bitmap::getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP {
  if (ppvBits != nullptr) {
#pragma warning(suppress : 26490) // Don't use reinterpret_cast (type.1)
	auto bitmap =
	    CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, reinterpret_cast<void**>(ppvBits), nullptr, 0); // NOLINT
	if (*ppvBits != nullptr) {
	  return bitmap;
	}
	DeleteObject(bitmap);
	throw;
  }

  throw;
}

void bitmap::internal::bfil(COLORREF const& backgroundColor) {
  auto const InverseBackgroundColor = fswap(backgroundColor);
  BitmapFileHandle =
      CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (BitmapFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
	auto fmtStr = std::wstring {};
	displayText::loadString(fmtStr, IDS_UNOPEN);
	displayText::shoMsg(fmt::format(fmtStr, UserBMPFileName->wstring()));
	CloseHandle(BitmapFileHandle);
	BitmapFileHandle  = nullptr;
	PCSBMPFileName[0] = 0;
	return;
  }
  auto bytesRead = DWORD {0};
  ReadFile(BitmapFileHandle, &BitmapFileHeader, 14U, &bytesRead, nullptr);
  constexpr auto MB_Sig = 0x4D42; // check for 'BM' signature in the 1st 2 bytes. Use Big Endian order
  if (BitmapFileHeader.bfType == MB_Sig) {
	auto fileHeaderSize = BitmapFileHeader.bfOffBits - 14U;
	if (fileHeaderSize > sizeof(BITMAPV4HEADER)) {
	  fileHeaderSize = sizeof(BITMAPV4HEADER);
	}
	ReadFile(BitmapFileHandle, &BitmapFileHeaderV4, fileHeaderSize, &bytesRead, nullptr);
  }
  else {
	CloseHandle(BitmapFileHandle);
	BitmapFileHandle  = nullptr;
	PCSBMPFileName[0] = 0;
	return;
  }
  if (gudtyp(BitmapFileHeaderV4.bV4BitCount)) {
	if (!StateMap.testAndReset(StateFlag::WASESC)) {
	  StateMap.reset(StateFlag::TRSET);
	}
	BitmapWidth  = BitmapFileHeaderV4.bV4Width;
	BitmapHeight = BitmapFileHeaderV4.bV4Height;
	StateMap.set(StateFlag::INIT);
	ZoomRect.left   = 0.0F;
	ZoomRect.bottom = 0.0F;
	ZoomRect.right  = UnzoomedRect.x;
	ZoomRect.top    = UnzoomedRect.y;
	BitmapDC        = CreateCompatibleDC(StitchWindowDC);
	if (BitmapFileHeaderV4.bV4BitCount == 1) {
	  StateMap.set(StateFlag::MONOMAP);
	  auto       bitmapWidthBytes = (BitmapWidth >> 5U) << 2U;
	  auto const widthOverflow    = BitmapWidth % 32;
	  if (widthOverflow != 0U) {
		bitmapWidthBytes += 4;
	  }
	  auto const bitmapSizeBytes = bitmapWidthBytes * BitmapHeight;
	  auto       monoBitmapData  = std::vector<uint8_t> {};
	  monoBitmapData.resize(bitmapSizeBytes);
	  ReadFile(BitmapFileHandle, monoBitmapData.data(), bitmapSizeBytes, &bytesRead, nullptr);
	  CloseHandle(BitmapFileHandle);
	  auto const flag       = binv(monoBitmapData, bitmapWidthBytes);
	  auto const foreground = gsl::narrow_cast<COLORREF>(flag ? InverseBackgroundColor : BitmapColor);
	  auto const background = gsl::narrow_cast<COLORREF>(flag ? BitmapColor : InverseBackgroundColor);
	  BitmapInfoHeader      = {};
	  BitmapInfoHeader.biSize        = sizeof(BitmapInfoHeader);
	  BitmapInfoHeader.biWidth       = BitmapWidth;
	  BitmapInfoHeader.biHeight      = BitmapHeight;
	  BitmapInfoHeader.biPlanes      = 1U;
	  BitmapInfoHeader.biBitCount    = 32U;
	  BitmapInfoHeader.biCompression = BI_RGB;
	  BitmapInfo.bmiHeader           = BitmapInfoHeader;
	  auto* bits                     = gsl::narrow_cast<uint32_t*>(nullptr);
	  auto  bitmap                   = bitmap::getBitmap(BitmapDC, &BitmapInfo, &bits);
	  // Synchronize
	  GdiFlush();
	  if (bits != nullptr) {
		for (auto iHeight = 0U; iHeight < BitmapHeight; iHeight++) {
		  bitlin(&monoBitmapData[wrap::toSize(iHeight) * bitmapWidthBytes],
		         &bits[wrap::toSize(iHeight) * BitmapWidth],
		         background,
		         foreground);
		}
	  }
	  auto deviceContext = CreateCompatibleDC(StitchWindowDC);
	  if ((bitmap != nullptr) && (deviceContext != nullptr)) {
		SelectObject(deviceContext, bitmap);
		BitmapFileHandle = CreateCompatibleBitmap(StitchWindowDC, BitmapWidth, BitmapHeight);
		SelectObject(BitmapDC, BitmapFileHandle);
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, deviceContext, 0, 0, SRCCOPY);
		DeleteObject(bitmap);
		DeleteObject(deviceContext);
	  }
	}
	else {
	  CloseHandle(BitmapFileHandle);
	  StateMap.reset(StateFlag::MONOMAP);
	  BitmapFileHandle = LoadImage(
	      ThrEdInstance, UserBMPFileName->wstring().c_str(), IMAGE_BITMAP, BitmapWidth, BitmapHeight, LR_LOADFROMFILE);
	  SelectObject(BitmapDC, BitmapFileHandle);
	  StateMap.set(StateFlag::RESTCH);
	}
	bitsiz();
	trace::initColorRef();
	StateMap.reset(StateFlag::HIDMAP);
  }
  else {
	CloseHandle(BitmapFileHandle);
	BitmapFileHandle  = nullptr;
	PCSBMPFileName[0] = 0;
	displayText::tabmsg(IDS_BMAP);
  }
}
// Get a rough estimate of whether black or white
// is dominant in the monochrome bitmap
auto bitmap::internal::binv(std::vector<uint8_t> const& monoBitmapData, uint32_t bitmapWidthInBytes) -> bool {
  auto       whiteBits = 0U;
  auto       blackBits = 0U;
  auto const byteCount = BitmapWidth >> 3U;
  for (auto iHeight = 0U; iHeight < BitmapHeight; iHeight++) {
	if ((wrap::toSize(bitmapWidthInBytes) * iHeight) < monoBitmapData.size()) {
	  auto const bcpnt = &monoBitmapData[wrap::toSize(bitmapWidthInBytes) * iHeight];
	  for (auto iBytes = 0U; iBytes < byteCount; iBytes++) {
		if (bcpnt[iBytes] == 0U) {
		  blackBits++;
		}
		else {
		  if (bcpnt[iBytes] == 0xff) {
			whiteBits++;
		  }
		}
	  }
	}
	else {
	  throw;
	}
  }
  return whiteBits > blackBits;
}

void bitmap::internal::bitlin(uint8_t const* source, uint32_t* destination, COLORREF foreground, COLORREF background) {
  if ((source != nullptr) && (destination != nullptr)) {
	for (auto i = 0U; i < (BitmapWidth >> 3U); i++) {
	  auto bits = std::bitset<8U>(source[i]);
	  for (auto bitOffset = 0U; bitOffset < 8U; bitOffset++) {
		*destination = bits[bitOffset ^ 7U] ? foreground : background;
		destination++;
	  }
	}
	if (auto const final = (BitmapWidth % 8)) {
	  auto bits = std::bitset<8U>(source[(BitmapWidth >> 3U)]);
	  for (auto bitOffset = final; bitOffset < 8U; bitOffset++) {
		*destination = bits[bitOffset ^ 7U] ? foreground : background;
		destination++;
	  }
	}
  }
}

#pragma warning(suppress : 26440) // suppression required until MSVC /analyze recognizes noexcept(false) used in gsl::narrow
void bitmap::internal::bitsiz() {
  auto const screenAspectRatio =
      gsl::narrow<float>(UnzoomedRect.x) / gsl::narrow<float>(UnzoomedRect.y);
  auto const bitmapAspectRatio = gsl::narrow<float>(BitmapWidth) / gsl::narrow<float>(BitmapHeight);
  if (bitmapAspectRatio > screenAspectRatio) {
	BitmapSizeinStitches.x = gsl::narrow<float>(UnzoomedRect.x);
	BitmapSizeinStitches.y = gsl::narrow<float>(UnzoomedRect.x) / bitmapAspectRatio;
  }
  else {
	BitmapSizeinStitches.x = gsl::narrow<float>(UnzoomedRect.y) * bitmapAspectRatio;
	BitmapSizeinStitches.y = gsl::narrow<float>(UnzoomedRect.y);
  }
  BmpStitchRatio.x = BitmapWidth / BitmapSizeinStitches.x;
  BmpStitchRatio.y = BitmapHeight / BitmapSizeinStitches.y;
  StitchBmpRatio.x = BitmapSizeinStitches.x / BitmapWidth;
  StitchBmpRatio.y = BitmapSizeinStitches.y / BitmapHeight;
}

constexpr auto bitmap::internal::gudtyp(WORD bitCount) noexcept -> bool {
  auto flag = false;
  switch (bitCount) { // NOLINT
	case 1U:
	case 24U:
	case 32U:
	  flag = true;
  }
  return flag;
}

void bitmap::resetBmpFile(bool reset) noexcept {
  if (PCSBMPFileName[0] != 0) {
	DeleteObject(BitmapFileHandle);
	ReleaseDC(ThrEdWindow, BitmapDC);
	if (reset) {
	  PCSBMPFileName[0] = 0;
	}
  }
}

void bitmap::savmap() {
  if (PCSBMPFileName[0] != 0) {
	if (StateMap.test(StateFlag::MONOMAP)) {
	  displayText::tabmsg(IDS_SAVMAP);
	  return;
	}
	if (!StateMap.test(StateFlag::WASTRAC)) {
	  displayText::tabmsg(IDS_MAPCHG);
	  return;
	}
	if (GetSaveFileName(&OpenBitmapName)) {
	  BitmapFileHandle =
	      CreateFile(UserBMPFileName->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	  if (BitmapFileHandle == INVALID_HANDLE_VALUE) { // NOLINT
		displayText::crmsg(*UserBMPFileName);
		return;
	  }
	  auto bytesWritten = DWORD {0};
	  WriteFile(BitmapFileHandle, &BitmapFileHeader, 14U, &bytesWritten, nullptr);
	  WriteFile(BitmapFileHandle, &BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - 14U, &bytesWritten, nullptr);
	  auto buffer = std::vector<uint8_t> {};
	  buffer.resize((wrap::toSize(BitmapWidth) * BitmapHeight * 3U) + 1U);
	  bi::movmap(BitmapWidth * BitmapHeight, buffer.data());
	  WriteFile(BitmapFileHandle, buffer.data(), BitmapWidth * BitmapHeight * 3, &bytesWritten, nullptr);
	  CloseHandle(BitmapFileHandle);
	}
  }
  else {
	displayText::tabmsg(IDS_SHOMAP);
  }
}

// Move unpacked 24BPP data into packed 24BPP data
void bitmap::internal::movmap(uint32_t cnt, uint8_t* buffer) {
  auto source = TraceBitmapData;
  if (source != nullptr) {
	auto destination = buffer;
	for (auto i = 0U; i < cnt; i++) {
	  *(convert_ptr<uint32_t*>(destination)) = *(source++);
	  destination += 3;
	}
  }
}

void bitmap::lodbmp() {
  bitmap::resetBmpFile(false);
  auto fileName = std::vector<wchar_t> {};
  fileName.resize(_MAX_PATH);
  auto dirBuffer = std::vector<wchar_t> {};
  dirBuffer.resize(_MAX_PATH);
  auto workingFileStr = UserBMPFileName->wstring();
  auto dirStr         = DefaultBMPDirectory->wstring();
  std::copy(workingFileStr.cbegin(), workingFileStr.cend(), fileName.begin());
  std::copy(dirStr.cbegin(), dirStr.cend(), dirBuffer.begin());
  OpenBitmapName.lpstrFile       = fileName.data();
  OpenBitmapName.lpstrInitialDir = dirBuffer.data();
  if (GetOpenFileName(&OpenBitmapName)) {
	fileName.resize(wcslen(fileName.data()));
	UserBMPFileName->assign(fileName.cbegin(), fileName.cend());
	trace::untrace();
	auto saveFile = utf::Utf16ToUtf8(UserBMPFileName->filename().wstring());
	// PCS file can only store a 16 character filename?
	// ToDo - give the user a little more info that the bitmap has not been loaded
	if (!saveFile.empty() && saveFile.size() < 16U) {
	  auto const bmpName = gsl::span<char> {PCSBMPFileName};
	  std::copy(saveFile.cbegin(), saveFile.cend(), bmpName.begin());
	  bi::defbNam();
	  bitmap::internal::bfil(BackgroundColor);
	}
	else {
	}
	StateMap.set(StateFlag::RESTCH);
  }
}

void bitmap::internal::defbNam() {
  if (!UserBMPFileName->empty()) {
	*DefaultBMPDirectory = UserBMPFileName->parent_path();
  }
}

auto bitmap::internal::nuBit() noexcept -> COLORREF {
  BitMapColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT; // NOLINT
  BitMapColorStruct.hwndOwner      = ThrEdWindow;
  BitMapColorStruct.lCustData      = 0;
  BitMapColorStruct.lpCustColors   = std::begin(BitmapBackgroundColors);
  BitMapColorStruct.lpfnHook       = nullptr;
  BitMapColorStruct.lpTemplateName = nullptr;
  BitMapColorStruct.rgbResult      = BitmapColor;
  BitMapColorStruct.lStructSize    = sizeof(BitMapColorStruct);
  return ChooseColor(&BitMapColorStruct);
}

void bitmap::setBmpColor() {
  if (bi::nuBit() != 0U) {
	BitmapColor = bi::fswap(BitMapColorStruct.rgbResult);
	if (PCSBMPFileName[0] != 0) {
	  bitmap::internal::bfil(BackgroundColor);
	}
	BitmapPen = thred::nuPen(BitmapPen, 1, BitmapColor);
	thred::zumhom();
  }
}

void bitmap::setUBfilename(fs::path* fileName) noexcept {
  UserBMPFileName = fileName;
}

void bitmap::setDefBmpDir(fs::path* directory) noexcept {
  DefaultBMPDirectory = directory;
}

void bitmap::assignDefBmpDir(std::wstring const& directory) {
  DefaultBMPDirectory->assign(directory);
}

void bitmap::assignUBFilename(fs::path const& directory) {
  fs::current_path(directory);
  auto BMPfileName = utf::Utf8ToUtf16(std::string(std::begin(PCSBMPFileName)));
  UserBMPFileName->assign(BMPfileName);
  bitmap::internal::bfil(BackgroundColor);
}

auto bitmap::getBitmapSizeinStitches() noexcept -> fPOINT {
  return BitmapSizeinStitches;
}

auto bitmap::getBmpBackColor(uint32_t const& index) noexcept -> COLORREF {
  return BitmapBackgroundColors[index];
}

void bitmap::setBmpBackColor() noexcept {
  for (auto iColor = 0U; iColor < 16U; iColor++) {
	BitmapBackgroundColors[iColor] = DefaultBitmapBackgroundColors[iColor];
  }
}

auto bitmap::getBmpColor() noexcept -> COLORREF {
  return BitmapColor;
}

auto bitmap::getBmpStitchRatio() noexcept -> fPOINT {
  return BmpStitchRatio;
}

auto bitmap::getBitmapHeight() noexcept -> uint32_t {
  return BitmapHeight;
}

auto bitmap::getBitmapDC() noexcept -> HDC {
  return BitmapDC;
}

auto bitmap::getBitmapWidth() noexcept -> uint32_t {
  return BitmapWidth;
}

void bitmap::setBitmapColor(COLORREF const& newColor) noexcept {
  BitmapColor = newColor;
}

void bitmap::setBitmapPen(HPEN const& pen) noexcept {
  BitmapPen = pen;
}

void bitmap::drawBmpBackground() {
  auto deviceContext = BitmapDC;
  if (StateMap.test(StateFlag::WASTRAC)) {
	deviceContext = TraceDC;
  }
  if (bi::bitar()) {
	StretchBlt(StitchWindowMemDC,
	           BitmapDstRect.left,
	           BitmapDstRect.top,
	           BitmapDstRect.right - BitmapDstRect.left,
	           BitmapDstRect.bottom - BitmapDstRect.top,
	           deviceContext,
	           BitmapSrcRect.left,
	           BitmapSrcRect.top,
	           BitmapSrcRect.right - BitmapSrcRect.left,
	           BitmapSrcRect.bottom - BitmapSrcRect.top,
	           SRCCOPY);
  }
}

auto bitmap::internal::bitar() -> bool {
  auto const zoomedInRect = fRECTANGLE {
      ZoomRect.left, (UnzoomedRect.y - ZoomRect.top), ZoomRect.right, (UnzoomedRect.y - ZoomRect.bottom)};
  if (zoomedInRect.top > BitmapSizeinStitches.y || zoomedInRect.left > BitmapSizeinStitches.x) {
	return false;
  }
  BitmapSrcRect = {wrap::ceil<int32_t>(ZoomRect.left * BmpStitchRatio.x),
                   wrap::ceil<int32_t>(zoomedInRect.top * BmpStitchRatio.y),
                   wrap::floor<int32_t>(ZoomRect.right * BmpStitchRatio.x),
                   wrap::floor<int32_t>(zoomedInRect.bottom * BmpStitchRatio.y)};
  if (BitmapSrcRect.right > gsl::narrow<int32_t>(BitmapWidth)) {
	BitmapSrcRect.right = BitmapWidth;
	StateMap.reset(StateFlag::LANDSCAP);
  }
  if (BitmapSrcRect.bottom > gsl::narrow<int32_t>(BitmapHeight)) {
	BitmapSrcRect.bottom = BitmapHeight;
	StateMap.set(StateFlag::LANDSCAP);
  }
  auto const backingRect = fRECTANGLE {BitmapSrcRect.left * StitchBmpRatio.x,
                                       BitmapSrcRect.top * StitchBmpRatio.y,
                                       BitmapSrcRect.right * StitchBmpRatio.x,
                                       BitmapSrcRect.bottom * StitchBmpRatio.y};

  auto const differenceRect = fRECTANGLE {backingRect.left - zoomedInRect.left,
                                          backingRect.top - zoomedInRect.top,
                                          zoomedInRect.right - backingRect.right,
                                          zoomedInRect.bottom - backingRect.bottom};
  auto const bitmapStitchRatio =
      fPOINT {gsl::narrow_cast<float>(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left),
              gsl::narrow_cast<float>(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom)};
  BitmapDstRect = {
      wrap::round<int32_t>(differenceRect.left * bitmapStitchRatio.x),
      wrap::round<int32_t>(differenceRect.top * bitmapStitchRatio.y),
      wrap::round<int32_t>(StitchWindowClientRect.right - differenceRect.right * bitmapStitchRatio.x),
      wrap::round<int32_t>(StitchWindowClientRect.bottom - differenceRect.bottom * bitmapStitchRatio.y)};
  return true;
}

auto bitmap::getrmap() -> uint32_t {
  auto header     = BITMAPINFOHEADER {gsl::narrow<DWORD>(sizeof(BITMAPINFOHEADER)),
                                  gsl::narrow_cast<LONG>(BitmapWidth),
                                  gsl::narrow_cast<LONG>(BitmapHeight),
                                  1U,
                                  32U,
                                  BI_RGB,
                                  0U,
                                  0L,
                                  0L,
                                  0U,
                                  0U};
  auto info       = BITMAPINFO {header, {RGBQUAD {0, 0, 0, 0}}};
  TraceBitmap     = bitmap::getBitmap(BitmapDC, &info, &TraceBitmapData);
  TraceDC         = CreateCompatibleDC(StitchWindowDC);
  auto bitmapSize = 0U;
  if ((TraceBitmap != nullptr) && (TraceDC != nullptr)) {
	SelectObject(TraceDC, TraceBitmap);
	BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
	StateMap.set(StateFlag::WASTRAC);
	bitmapSize = (BitmapWidth + 1U) * (BitmapHeight + 1U);
	TracedMap->resize(bitmapSize);
	TracedMap->reset();
	StretchBlt(StitchWindowMemDC,
	           BitmapDstRect.left,
	           BitmapDstRect.top,
	           BitmapDstRect.right - BitmapDstRect.left,
	           BitmapDstRect.bottom - BitmapDstRect.top,
	           BitmapDC,
	           BitmapSrcRect.left,
	           BitmapSrcRect.top,
	           BitmapSrcRect.right - BitmapSrcRect.left,
	           BitmapSrcRect.bottom - BitmapSrcRect.top,
	           SRCCOPY);
  }
  return bitmapSize;
}

void bitmap::bitbltBitmap() noexcept {
  BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
}

auto bitmap::internal::stch2bit(fPOINT& point) -> POINT {
  if (StateMap.test(StateFlag::LANDSCAP)) {
	point.y -= (gsl::narrow_cast<float>(UnzoomedRect.y) - BitmapSizeinStitches.y);
  }
  return POINT {wrap::round<int32_t>(BmpStitchRatio.x * point.x),
                wrap::round<int32_t>(BitmapHeight - BmpStitchRatio.y * point.y)};
}

void bitmap::internal::pxlin(FRMHED const& form, uint32_t start, uint32_t finish) {
  POINT line[2];
  auto  vertexIt = std::next(FormVertices->begin(), form.vertexIndex);
  line[0]        = bi::stch2bit(vertexIt[start]);
  line[1]        = bi::stch2bit(vertexIt[finish]);
  Polyline(BitmapDC, static_cast<POINT const*>(line), 2);
  Polyline(TraceDC, static_cast<POINT const*>(line), 2);
}

void bitmap::bfrm(FRMHED const& form) {
  if (form.vertexCount != 0U) {
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; iVertex++) {
	  bi::pxlin(form, iVertex, iVertex + 1U);
	}
	if (form.type != FRMLINE) {
	  bi::pxlin(form, form.vertexCount - 1U, 0);
	}
  }
}
