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

// using the OS mechanism for last used directory NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define USE_DEFBDIR 0
// Use the 8.3 version of the bitmap file when saving the name into the ThrEd file NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define USE_SHORT_NAME 0 

namespace bi = bitmap::internal;

static auto BitMapColorStruct = CHOOSECOLOR {};

static auto BitmapBackgroundColors = std::array<COLORREF, COLORCNT> {}; // for the bitmap color dialog box

static auto BitmapColor          = BITCOL;  // bitmap color
static auto BitmapDC             = HDC {};  // bitmap device context
static auto BitmapDstRect        = RECT {}; // stitch window destination rectangle for zooomed view
static auto BitmapFileHandle     = HANDLE {};           // bitmap handle
static auto BitmapFileHeader     = BITMAPFILEHEADER {}; // bitmap file header
static auto BitmapFileHeaderV4   = BITMAPV4HEADER {};   // bitmap version4 file header
static auto BitmapHeight         = int {};              // bitmap height
static auto BitmapInfo           = BITMAPINFO {};       // bitmap info
static auto BitmapInfoHeader     = BITMAPINFOHEADER {}; // bitmap info header
static auto BitmapPen            = HPEN {};             // bitmap pen
static auto BitmapSizeinStitches = fPOINT {};           // bitmap end points in stitch points
static auto BitmapSrcRect        = RECT {};             // bitmap source rectangle for zoomed view
static auto BitmapWidth          = int {};              // bitmap width
static auto BmpStitchRatio       = fPOINT {};           // bitmap to stitch hoop ratios
static auto TraceBitmap          = HBITMAP {};          // trace bitmap
static auto TraceDC              = HDC {};              // trace device context
static auto UTF16BMPname = static_cast<fs::path*>(nullptr); // bitmap file name from user load
static auto UTF8BMPname  = std::array<char, SZBMPNM> {};    // bitmap file name from pcs file

constexpr auto BPB   = 8U;          // bits per byte
constexpr auto BPP24 = DWORD {24U}; // 24 bits per pixel
constexpr auto BPP32 = DWORD {32U}; // 32 bits per pixel

constexpr auto bitmap::internal::fswap(COLORREF color) noexcept -> COLORREF {
  // this code compiles to the same assembly as _byteswap_ulong(color) >> 8U, making
  // it a portable version
  auto const a = ((color & 0x000000FFU) << 24U) | ((color & 0x0000FF00U) << 8U) |
                 ((color & 0x00FF0000U) >> 8U) | ((color & 0xFF000000U) >> 24U);
  return a >> BPB;
}

auto bitmap::getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP {
  if (ppvBits != nullptr) {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(readability-qualified-auto)
	auto const bitmap =
	    CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, reinterpret_cast<void**>(ppvBits), nullptr, 0); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
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
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto hBitmapFile =
      CreateFile(UTF16BMPname->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (hBitmapFile == INVALID_HANDLE_VALUE) {
	auto fmtStr = std::wstring {};
	displayText::loadString(fmtStr, IDS_UNOPEN);
	// NOLINTNEXTLINE
	displayText::shoMsg(fmt::format(fmtStr, UTF16BMPname->wstring()));
	CloseHandle(hBitmapFile);
	bitmap::resetBmpFile(true);
	return;
  }
  auto bytesRead = DWORD {0};
  ReadFile(hBitmapFile, &BitmapFileHeader, sizeof(BitmapFileHeader), &bytesRead, nullptr);
  constexpr auto MB_SIG = 0x4D42; // check for 'BM' signature in the 1st 2 bytes. Use Big Endian order
  if (BitmapFileHeader.bfType == MB_SIG) {
	auto fileHeaderSize = wrap::toSize(BitmapFileHeader.bfOffBits) - sizeof(BitmapFileHeader);
	if (fileHeaderSize > sizeof(BITMAPV4HEADER)) {
	  fileHeaderSize = sizeof(BITMAPV4HEADER);
	}
	ReadFile(hBitmapFile, &BitmapFileHeaderV4, gsl::narrow<DWORD>(fileHeaderSize), &bytesRead, nullptr);
  }
  else {
	CloseHandle(hBitmapFile);
	bitmap::resetBmpFile(true);
	return;
  }
  if (gudtyp(BitmapFileHeaderV4.bV4BitCount)) {
	if (!StateMap->testAndReset(StateFlag::WASESC)) {
	  StateMap->reset(StateFlag::TRSET);
	}
	BitmapWidth  = BitmapFileHeaderV4.bV4Width;
	BitmapHeight = BitmapFileHeaderV4.bV4Height;
	StateMap->set(StateFlag::INIT);
	ZoomRect.left   = 0.0F;
	ZoomRect.bottom = 0.0F;
	wrap::narrow_cast(ZoomRect.right, UnzoomedRect.x);
	wrap::narrow_cast(ZoomRect.top, UnzoomedRect.y);
	BitmapDC = CreateCompatibleDC(StitchWindowDC);
	if (BitmapFileHeaderV4.bV4BitCount == 1) {
	  StateMap->set(StateFlag::MONOMAP);
	  // NOLINTNEXTLINE(readability-magic-numbers)
	  auto       bitmapWidthBytes = gsl::narrow_cast<uint32_t>(BitmapWidth) >> 5U << 2U;
	  auto const widthOverflow    = BitmapWidth % 32;
	  if (widthOverflow != 0U) {
		bitmapWidthBytes += 4U;
	  }
	  auto const bitmapSizeBytes = bitmapWidthBytes * gsl::narrow<decltype(bitmapWidthBytes)>(BitmapHeight);
	  auto monoBitmapData = std::vector<uint8_t> {};
	  monoBitmapData.resize(bitmapSizeBytes);
	  ReadFile(hBitmapFile, monoBitmapData.data(), bitmapSizeBytes, &bytesRead, nullptr);
	  CloseHandle(hBitmapFile);
	  auto const flag       = binv(monoBitmapData, bitmapWidthBytes);
	  auto const foreground = gsl::narrow_cast<COLORREF>(flag ? InverseBackgroundColor : BitmapColor);
	  auto const background = gsl::narrow_cast<COLORREF>(flag ? BitmapColor : InverseBackgroundColor);
	  BitmapInfoHeader      = {};
	  BitmapInfoHeader.biSize        = sizeof(BitmapInfoHeader);
	  BitmapInfoHeader.biWidth       = BitmapWidth;
	  BitmapInfoHeader.biHeight      = BitmapHeight;
	  BitmapInfoHeader.biPlanes      = 1U;
	  BitmapInfoHeader.biBitCount    = BPP32;
	  BitmapInfoHeader.biCompression = BI_RGB;
	  BitmapInfo.bmiHeader           = BitmapInfoHeader;
	  auto* bits                     = gsl::narrow_cast<uint32_t*>(nullptr);
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const bitmap = bitmap::getBitmap(BitmapDC, &BitmapInfo, &bits);
	  // Synchronize
	  GdiFlush();
	  if (bits != nullptr) {
		for (auto iHeight = 0; iHeight < BitmapHeight; ++iHeight) {
		  bitlin(&monoBitmapData[wrap::toSize(iHeight) * bitmapWidthBytes],
		         &bits[wrap::toSize(iHeight * BitmapWidth)],
		         bitmapWidthBytes,
		         background,
		         foreground);
		}
	  }
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const deviceContext = CreateCompatibleDC(StitchWindowDC);
	  if ((bitmap != nullptr) && (deviceContext != nullptr)) {
		SelectObject(deviceContext, bitmap);
		hBitmapFile = CreateCompatibleBitmap(StitchWindowDC, BitmapWidth, BitmapHeight);
		SelectObject(BitmapDC, hBitmapFile);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, deviceContext, 0, 0, SRCCOPY);
		DeleteObject(bitmap);
		DeleteObject(deviceContext);
	  }
	}
	else {
	  CloseHandle(hBitmapFile);
	  StateMap->reset(StateFlag::MONOMAP);
	  hBitmapFile = LoadImage(
	      ThrEdInstance, UTF16BMPname->wstring().c_str(), IMAGE_BITMAP, BitmapWidth, BitmapHeight, LR_LOADFROMFILE);
	  SelectObject(BitmapDC, hBitmapFile);
	  StateMap->set(StateFlag::RESTCH);
	}
	bitsiz();
	trace::initColorRef();
	StateMap->reset(StateFlag::HIDMAP);
  }
  else {
	CloseHandle(hBitmapFile);
	bitmap::resetBmpFile(true);
	displayText::tabmsg(IDS_BMAP);
  }
}

// Get a rough estimate of whether black or white
// is dominant in the monochrome bitmap
auto bitmap::internal::binv(std::vector<uint8_t> const& monoBitmapData, uint32_t bitmapWidthInBytes) -> bool {
  auto whiteBits = 0U;
  auto blackBits = 0U;
  for (auto iHeight = 0; iHeight < BitmapHeight; ++iHeight) {
	if ((wrap::toSize(iHeight) * bitmapWidthInBytes) < monoBitmapData.size()) {
	  auto const* bcpnt = &monoBitmapData[wrap::toSize(iHeight) * bitmapWidthInBytes];
	  for (auto iBytes = 0U; iBytes < bitmapWidthInBytes; ++iBytes) {
		if (bcpnt[iBytes] == 0U) {
		  ++blackBits;
		}
		else {
		  if (bcpnt[iBytes] == UCHAR_MAX) {
			++whiteBits;
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

void bitmap::internal::bitlin(uint8_t const* source,
                              uint32_t*      destination,
                              uint32_t       bitmapWidthBytes,
                              COLORREF       foreground,
                              COLORREF       background) {
  if ((source != nullptr) && (destination != nullptr)) {
	for (auto i = 0U; i < bitmapWidthBytes; ++i) {
	  auto bits = std::bitset<CHAR_BIT>(source[i]);
	  for (auto bitOffset = 0U; bitOffset < CHAR_BIT; ++bitOffset) {
		*destination = bits[bitOffset ^ (CHAR_BIT - 1U)] ? foreground : background;
		++destination;
	  }
	}
	if (auto const final = (gsl::narrow<uint32_t>(BitmapWidth) % CHAR_BIT)) {
	  auto bits = std::bitset<CHAR_BIT>(source[bitmapWidthBytes]);
	  for (auto bitOffset = final; bitOffset < CHAR_BIT; ++bitOffset) {
		*destination = bits[bitOffset ^ (CHAR_BIT - 1U)] ? foreground : background;
		++destination;
	  }
	}
  }
}

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
  BmpStitchRatio.x = wrap::toFloat(BitmapWidth) / BitmapSizeinStitches.x;
  BmpStitchRatio.y = wrap::toFloat(BitmapHeight) / BitmapSizeinStitches.y;
  StitchBmpRatio.x = BitmapSizeinStitches.x / wrap::toFloat(BitmapWidth);
  StitchBmpRatio.y = BitmapSizeinStitches.y / wrap::toFloat(BitmapHeight);
}

constexpr auto bitmap::internal::gudtyp(WORD bitCount) noexcept -> bool {
  switch (bitCount) {
	case 1U:
	case BPP24:
	case BPP32:
	  return true;
	default:
	  return false;
  }
}

void bitmap::resetBmpFile(bool reset) {
  if (bitmap::ismap()) {
	DeleteObject(BitmapFileHandle);
	ReleaseDC(ThrEdWindow, BitmapDC);
	if (reset) {
	  UTF8BMPname.fill(0);
	}
  }
}

auto bitmap::internal::saveName(fs::path& fileName) {
  auto* pFileSave = gsl::narrow_cast<IFileSaveDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hr = CoCreateInstance(
      CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (SUCCEEDED(hr) && (nullptr != pFileSave)) {
	COMDLG_FILTERSPEC const filterFileTypes[] = {{L"Bitmap Files", L"*.bmp"}, {L"All files", L"*.*"}};
	constexpr auto          FILTERCNT = (sizeof(filterFileTypes) / sizeof(filterFileTypes[0])); // File filter count
	hr = pFileSave->SetFileTypes(FILTERCNT, static_cast<COMDLG_FILTERSPEC const*>(filterFileTypes));
	hr += pFileSave->SetFileTypeIndex(0);
	hr += pFileSave->SetTitle(L"Save Bitmap");
	auto const bmpName = UTF16BMPname->filename().wstring();
	hr += pFileSave->SetFileName(bmpName.c_str());
	hr += pFileSave->SetDefaultExtension(L"bmp");
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (SUCCEEDED(hr)) {
	  hr = pFileSave->Show(nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (SUCCEEDED(hr)) {
		auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
		hr          = pFileSave->GetResult(&pItem);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		if (SUCCEEDED(hr) && (nullptr != pItem)) {
		  // NOLINTNEXTLINE(readability-qualified-auto)
		  auto pszFilePath = PWSTR {nullptr};
		  hr               = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		  if (SUCCEEDED(hr)) {
			fileName.assign(pszFilePath);
			CoTaskMemFree(pszFilePath);
			return true;
		  }
		}
	  }
	}
  }
  return false;
}

void bitmap::savmap() {
  if (bitmap::ismap()) {
	if (StateMap->test(StateFlag::MONOMAP)) {
	  displayText::tabmsg(IDS_SAVMAP);
	  return;
	}
	if (!StateMap->test(StateFlag::WASTRAC)) {
	  displayText::tabmsg(IDS_MAPCHG);
	  return;
	}
	auto fileName = fs::path {};
	if (bi::saveName(fileName)) {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const hBitmap =
	      CreateFile(fileName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (hBitmap == INVALID_HANDLE_VALUE) {
		displayText::crmsg(*UTF16BMPname);
		return;
	  }
	  auto bytesWritten = DWORD {0};
	  WriteFile(hBitmap, &BitmapFileHeader, sizeof(BitmapFileHeader), &bytesWritten, nullptr);
	  WriteFile(hBitmap, &BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - sizeof(BitmapFileHeader), &bytesWritten, nullptr);
	  auto buffer = std::vector<uint8_t> {};
	  buffer.resize((wrap::toSize(BitmapWidth) * wrap::toUnsigned(BitmapHeight) * 3U) + 1U);
	  bi::movmap(BitmapWidth * BitmapHeight, buffer.data());
	  WriteFile(hBitmap, buffer.data(), gsl::narrow<DWORD>(BitmapWidth * BitmapHeight * 3), &bytesWritten, nullptr);
	  CloseHandle(hBitmap);
	}
  }
  else {
	displayText::tabmsg(IDS_SHOMAP);
  }
}

// Move unpacked 24BPP data into packed 24BPP data
void bitmap::internal::movmap(int cnt, uint8_t* buffer) {
  auto* source = TraceBitmapData;
  if (source != nullptr) {
	auto* destination = buffer;
	for (auto i = 0; i < cnt; ++i) {
	  *(convert_ptr<uint32_t*>(destination)) = *(source++);
	  destination += 3;
	}
  }
}

auto bitmap::internal::loadName(fs::path const* directory, fs::path* fileName) -> bool {
  if ((nullptr != fileName) && (nullptr != directory)) {
	auto* pFileOpen = gsl::narrow_cast<IFileOpenDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
	auto hr = CoCreateInstance(
	    CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast,hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (SUCCEEDED(hr) && (nullptr != pFileOpen)) {
	  auto dwOptions = DWORD {};
	  hr             = pFileOpen->GetOptions(&dwOptions);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	  if (SUCCEEDED(hr)) {
		COMDLG_FILTERSPEC const filterFileTypes[] = {{L"Bitmap Files", L"*.bmp"}, {L"All files", L"*.*"}};
		constexpr auto          FILTERCNT         = (sizeof(filterFileTypes) / sizeof(filterFileTypes[0])); // Filter file types count
		// NOLINTNEXTLINE(hicpp-signed-bitwise)
		hr = pFileOpen->SetOptions(dwOptions | FOS_DONTADDTORECENT);
		hr += pFileOpen->SetFileTypes(FILTERCNT, static_cast<COMDLG_FILTERSPEC const*>(filterFileTypes));
		hr += pFileOpen->SetTitle(L"Open Thred File");
#if USE_DEFBDIR
		// If we want to, we can set the default directory rather than using the OS mechanism for last used
		auto* psiFrom = gsl::narrow_cast<IShellItem*>(nullptr);
		// NOLINTNEXTLINE(clang-diagnostic-language-extension-token)
		hr += SHCreateItemFromParsingName(directory->wstring().data(), nullptr, IID_PPV_ARGS(&psiFrom));
		hr += pFileOpen->SetFolder(psiFrom);
		if (nullptr != psiFrom) {
		  psiFrom->Release();
		}
#else
		UNREFERENCED_PARAMETER(directory);
#endif
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		if (SUCCEEDED(hr)) {
		  hr = pFileOpen->Show(nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
		  if (SUCCEEDED(hr)) {
			auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
			hr          = pFileOpen->GetResult(&pItem);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
			if (SUCCEEDED(hr) && (nullptr != pItem)) {
			  // NOLINTNEXTLINE(readability-qualified-auto)
			  auto pszFilePath = PWSTR {nullptr};
			  hr               = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
			  if (SUCCEEDED(hr)) {
				fileName->assign(pszFilePath);
				CoTaskMemFree(pszFilePath);
				return true;
			  }
			}
		  }
		}
	  }
	}
  }
  return false;
}

void bitmap::lodbmp(fs::path const* directory) {
  if (bi::loadName(directory, UTF16BMPname)) {
	bitmap::resetBmpFile(false);
	trace::untrace();
#if USE_SHORT_NAME
	auto const pleng = GetShortPathName(UTF16BMPname->wstring().c_str(), NULL, 0);
	auto       dest  = std::vector<wchar_t> {};
	dest.resize(pleng);
	GetShortPathName(UTF16BMPname->wstring().c_str(), dest.data(), wrap::toUnsigned(dest.size()));
	auto filePart = fs::path {dest.data()};
	auto saveFile = utf::Utf16ToUtf8(filePart.filename().wstring());
#else
	auto const saveFile = utf::Utf16ToUtf8(UTF16BMPname->filename().wstring());
#endif
	if (!saveFile.empty() && saveFile.size() < UTF8BMPname.size()) {
	  std::copy(saveFile.cbegin(), saveFile.cend(), UTF8BMPname.begin());
	  bitmap::internal::bfil(BackgroundColor);
	}
	else {
	  // THR version 2 file can only store a 16 character filename
	  // Give the user a little more info why the bitmap has not been loaded
	  auto fmtStr = std::wstring {};
	  displayText::loadString(fmtStr, IDS_BMPLONG);
	  displayText::shoMsg(fmt::format(fmtStr, ThrName->wstring()));
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

auto bitmap::internal::nuBit() noexcept -> BOOL {
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  BitMapColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
  BitMapColorStruct.hwndOwner      = ThrEdWindow;
  BitMapColorStruct.lCustData      = 0;
  BitMapColorStruct.lpCustColors   = BitmapBackgroundColors.data();
  BitMapColorStruct.lpfnHook       = nullptr;
  BitMapColorStruct.lpTemplateName = nullptr;
  BitMapColorStruct.rgbResult      = BitmapColor;
  BitMapColorStruct.lStructSize    = sizeof(BitMapColorStruct);
  return ChooseColor(&BitMapColorStruct);
}

void bitmap::setBmpColor() {
  if (bi::nuBit() != 0U) {
	BitmapColor = bi::fswap(BitMapColorStruct.rgbResult);
	if (bitmap::ismap()) {
	  bitmap::internal::bfil(BackgroundColor);
	}
	BitmapPen = thred::nuPen(BitmapPen, 1, BitmapColor);
	thred::zumhom();
  }
}

void bitmap::setUBfilename(fs::path* fileName) noexcept {
  UTF16BMPname = fileName;
}

void bitmap::assignUBFilename(fs::path const& directory) {
  fs::current_path(directory);
  auto const BMPfileName = utf::Utf8ToUtf16(std::string(UTF8BMPname.data()));
  auto const fullPath    = directory / BMPfileName;
  UTF16BMPname->assign(fullPath);
  bitmap::internal::bfil(BackgroundColor);
}

auto bitmap::getBitmapSizeinStitches() noexcept -> fPOINT {
  return BitmapSizeinStitches;
}

auto bitmap::getBmpBackColor(uint32_t const& index) noexcept -> COLORREF {
  return BitmapBackgroundColors[index];
}

void bitmap::setBmpBackColor() noexcept {
  constexpr auto defaultColors = std::array<COLORREF, COLORCNT> {0x00c0d5bf,
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

  BitmapBackgroundColors = defaultColors;
}

auto bitmap::getBmpColor() noexcept -> COLORREF {
  return BitmapColor;
}

auto bitmap::getBmpStitchRatio() noexcept -> fPOINT {
  return BmpStitchRatio;
}

auto bitmap::getBitmapHeight() noexcept -> int {
  return BitmapHeight;
}

auto bitmap::getBitmapDC() noexcept -> HDC {
  return BitmapDC;
}

auto bitmap::getTraceDC() noexcept -> HDC {
  return TraceDC;
}

auto bitmap::getBitmapWidth() noexcept -> int {
  return BitmapWidth;
}

void bitmap::setBitmapColor(COLORREF const& newColor) noexcept {
  BitmapColor = newColor;
}

void bitmap::setBitmapPen(HPEN pen) noexcept {
  BitmapPen = pen;
}

void bitmap::resetDC() noexcept {
  DeleteObject(TraceBitmap);
  DeleteObject(TraceDC);
}

auto bitmap::getBmpNameLength() -> uint32_t {
  return wrap::toUnsigned(UTF8BMPname.size());
}

auto bitmap::getBmpNameData() noexcept -> char* {
  return UTF8BMPname.data();
}

auto bitmap::ismap() noexcept -> bool {
  return (UTF8BMPname[0] != 0);
}

void bitmap::chkbit() {
  if (bitmap::ismap() && (StateMap->test(StateFlag::WASDIF) || StateMap->test(StateFlag::WASDSEL) ||
                          StateMap->test(StateFlag::WASBLAK))) {
	StateMap->set(StateFlag::WASESC);
	bitmap::internal::bfil(BackgroundColor);
  }
}

void bitmap::delmap() {
  UTF8BMPname.fill(0);
  StateMap->set(StateFlag::RESTCH);
}

void bitmap::drawBmpBackground() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto deviceContext = BitmapDC;
  if (StateMap->test(StateFlag::WASTRAC)) {
	deviceContext = TraceDC;
  }
  if (bi::bitar()) {
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
  auto const zoomedInRect = fRECTANGLE {ZoomRect.left,
                                        (wrap::toFloat(UnzoomedRect.y) - ZoomRect.top),
                                        ZoomRect.right,
                                        (wrap::toFloat(UnzoomedRect.y) - ZoomRect.bottom)};
  if (zoomedInRect.top > BitmapSizeinStitches.y || zoomedInRect.left > BitmapSizeinStitches.x) {
	return false;
  }
  BitmapSrcRect = {wrap::ceil<int32_t>(ZoomRect.left * BmpStitchRatio.x),
                   wrap::ceil<int32_t>(zoomedInRect.top * BmpStitchRatio.y),
                   wrap::floor<int32_t>(ZoomRect.right * BmpStitchRatio.x),
                   wrap::floor<int32_t>(zoomedInRect.bottom * BmpStitchRatio.y)};
  if (BitmapSrcRect.right > gsl::narrow<int32_t>(BitmapWidth)) {
	BitmapSrcRect.right = BitmapWidth;
	StateMap->reset(StateFlag::LANDSCAP);
  }
  if (BitmapSrcRect.bottom > BitmapHeight) {
	BitmapSrcRect.bottom = BitmapHeight;
	StateMap->set(StateFlag::LANDSCAP);
  }
  auto const backingRect = fRECTANGLE {wrap::toFloat(BitmapSrcRect.left) * StitchBmpRatio.x,
                                       wrap::toFloat(BitmapSrcRect.top) * StitchBmpRatio.y,
                                       wrap::toFloat(BitmapSrcRect.right) * StitchBmpRatio.x,
                                       wrap::toFloat(BitmapSrcRect.bottom) * StitchBmpRatio.y};

  auto const differenceRect = fRECTANGLE {backingRect.left - zoomedInRect.left,
                                          backingRect.top - zoomedInRect.top,
                                          zoomedInRect.right - backingRect.right,
                                          zoomedInRect.bottom - backingRect.bottom};
  auto const bitmapStitchRatio =
      fPOINT {wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left),
              wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom)};
  BitmapDstRect = {std::lround(differenceRect.left * bitmapStitchRatio.x),
                   std::lround(differenceRect.top * bitmapStitchRatio.y),
                   std::lround(wrap::toFloat(StitchWindowClientRect.right) -
                               differenceRect.right * bitmapStitchRatio.x),
                   std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                               differenceRect.bottom * bitmapStitchRatio.y)};
  return true;
}

auto bitmap::getrmap() -> uint32_t {
  auto const header = BITMAPINFOHEADER {wrap::toUnsigned(sizeof(BITMAPINFOHEADER)),
                                        gsl::narrow_cast<LONG>(BitmapWidth),
                                        gsl::narrow_cast<LONG>(BitmapHeight),
                                        1U,
                                        BPP32,
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
	StateMap->set(StateFlag::WASTRAC);
	bitmapSize = (wrap::toUnsigned(BitmapWidth) + 1U) * (wrap::toUnsigned(BitmapHeight) + 1U);
	TracedMap->resize(bitmapSize);
	TracedMap->reset();
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
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
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, TraceDC, 0, 0, SRCCOPY);
}

auto bitmap::internal::stch2bit(fPOINT& point) -> POINT {
  if (StateMap->test(StateFlag::LANDSCAP)) {
	point.y -= (wrap::toFloat(UnzoomedRect.y) - BitmapSizeinStitches.y);
  }
  return POINT {wrap::round<LONG>(BmpStitchRatio.x * point.x),
                wrap::round<LONG>(BitmapHeight - BmpStitchRatio.y * point.y)};
}

void bitmap::internal::pxlin(FRMHED const& form, uint32_t start, uint32_t finish) {
  auto line     = std::array<POINT, 2> {};
  auto const vertexIt = wrap::next(FormVertices->begin(), form.vertexIndex);
  auto const vStart   = wrap::next(vertexIt, start);
  auto const vFinish  = wrap::next(vertexIt, finish);
  line[0]       = bi::stch2bit(*vStart);
  line[1]       = bi::stch2bit(*vFinish);
  wrap::Polyline(BitmapDC, line.data(), wrap::toUnsigned(line.size()));
  wrap::Polyline(TraceDC, line.data(), wrap::toUnsigned(line.size()));
}

void bitmap::bfrm(FRMHED const& form) {
  if (form.vertexCount != 0U) {
	for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	  bi::pxlin(form, iVertex, iVertex + 1U);
	}
	if (form.type != FRMLINE) {
	  bi::pxlin(form, form.vertexCount - 1U, 0);
	}
  }
}
