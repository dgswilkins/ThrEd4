// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "bitmap.h"
#include "displayText.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "reporting.h"
#include "Resources/resource.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "trace.h"
#include "utf8conv.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/narrow"
#include "gsl/pointers"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <algorithm>
#include <combaseapi.h>
#include <commdlg.h>
#include <errhandlingapi.h>
#include <fileapi.h>
#include <handleapi.h>
#include <minwindef.h>
#include <ShObjIdl_core.h>
#include <shtypes.h>
#include <windef.h>
#include <winerror.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <array>
#include <bitset>
#include <cmath>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
// ReSharper disable CppUnusedIncludeDirective
#include <type_traits>
// ReSharper restore CppUnusedIncludeDirective
#include <vector>

constexpr auto BITCOL  = uint32_t {0xffff00U}; // default bitmap color
constexpr auto SZBMPNM = 17U; // THR spec for BMP filename length (+ 1 for zero terminator)
constexpr auto FLTBMP  = COMDLG_FILTERSPEC {L"Bitmap Files", L"*.bmp"}; // filter specifications
constexpr auto FLTALL  = COMDLG_FILTERSPEC {L"All files", L"*.*"};      // filter specifications
constexpr auto BPB     = 8U;                                            // bits per byte
constexpr auto BPP24   = DWORD {24U};                                   // 24 bits per pixel
constexpr auto BPP32   = DWORD {32U};                                   // 32 bits per pixel

// bitmap internal namespace
namespace {
auto BitmapBackgroundColors = gsl::narrow_cast<std::vector<COLORREF>*>(nullptr); // for the bitmap color dialog box
auto BitmapColor          = BITCOL;                                              // bitmap color
auto BitMapColorStruct    = CHOOSECOLOR {};
auto BitmapDC             = HDC {};    // bitmap device context
auto BitmapDstRect        = RECT {};   // stitch window destination rectangle for zooomed view
auto BitmapFileHandle     = HANDLE {}; // bitmap handle
auto BitmapFileHeader     = BITMAPFILEHEADER {}; // bitmap file header
auto BitmapFileHeaderV4   = BITMAPV4HEADER {};   // bitmap version4 file header
auto BitmapHeight         = LONG {};             // bitmap height
auto BitmapInfo           = BITMAPINFO {};       // bitmap info
auto BitmapInfoHeader     = BITMAPINFOHEADER {}; // bitmap info header
auto BitmapPen            = HPEN {};             // bitmap pen
auto BitmapSizeinStitches = F_POINT {};          // bitmap end points in stitch points
auto BitmapSrcRect        = RECT {};             // bitmap source rectangle for zoomed view
auto BitmapWidth          = LONG {};             // bitmap width
auto BmpStitchRatio       = F_POINT {};          // bitmap to stitch hoop ratios
auto TraceBitmap          = HBITMAP {};          // trace bitmap
auto TraceDC              = HDC {};              // trace device context
auto UTF16BMPname         = gsl::narrow_cast<fs::path*>(nullptr); // bitmap file name from user load
auto UTF8BMPname          = std::array<char, SZBMPNM> {};         // bitmap file name from pcs file

// Definitions
auto binv(std::vector<uint8_t> const& monoBitmapData) noexcept -> bool;
auto bitar() -> bool;
void bitlin(gsl::span<uint8_t> const&  source,
            gsl::span<uint32_t> const& destination,
            COLORREF                   foreground,
            COLORREF                   background) noexcept;
void bitsiz();

constexpr auto fswap(COLORREF color) noexcept -> COLORREF;
auto getBitmap(HDC hdc, const BITMAPINFO* pbmi, gsl::not_null<uint32_t**> ppvBits) -> HBITMAP;
constexpr auto gudtyp(WORD bitCount) noexcept -> bool;

auto loadName(fs::path const& directory, fs::path& fileName) -> bool;
void movmap(gsl::span<uint32_t> const& src, std::vector<uint8_t>& buffer) noexcept;
auto nuBit() noexcept -> BOOL;
void pxlin(FRM_HEAD const& form, uint32_t start, uint32_t finish);
auto saveName(fs::path& fileName);
auto stch2bit(F_POINT& point) -> POINT;

// Functions
// Get a rough estimate of whether black or white
// is dominant in the monochrome bitmap
auto binv(const std::vector<uint8_t>& monoBitmapData) noexcept -> bool {
  auto const whiteBits = gsl::narrow_cast<size_t>(std::ranges::count(monoBitmapData, UCHAR_MAX));
  return whiteBits > monoBitmapData.size() >> 1U;
}

auto bitar() -> bool {
  auto const zoomedInRect = F_RECTANGLE {ZoomRect.left,
                                         (wrap::toFloat(UnzoomedRect.cy) - ZoomRect.top),
                                         ZoomRect.right,
                                         (wrap::toFloat(UnzoomedRect.cy) - ZoomRect.bottom)};
  if (zoomedInRect.top > BitmapSizeinStitches.y || zoomedInRect.left > BitmapSizeinStitches.x) {
	return false;
  }
  BitmapSrcRect = {.left   = wrap::ceil<int32_t>(ZoomRect.left * BmpStitchRatio.x),
                   .top    = wrap::ceil<int32_t>(zoomedInRect.top * BmpStitchRatio.y),
                   .right  = wrap::floor<int32_t>(ZoomRect.right * BmpStitchRatio.x),
                   .bottom = wrap::floor<int32_t>(zoomedInRect.bottom * BmpStitchRatio.y)};
  if (BitmapSrcRect.right > gsl::narrow<int32_t>(BitmapWidth)) {
	BitmapSrcRect.right = BitmapWidth;
	Instance->StateMap.reset(StateFlag::LANDSCAP);
  }
  if (BitmapSrcRect.bottom > BitmapHeight) {
	BitmapSrcRect.bottom = BitmapHeight;
	Instance->StateMap.set(StateFlag::LANDSCAP);
  }
  auto const backingRect = F_RECTANGLE {wrap::toFloat(BitmapSrcRect.left) * StitchBmpRatio.x,
                                        wrap::toFloat(BitmapSrcRect.top) * StitchBmpRatio.y,
                                        wrap::toFloat(BitmapSrcRect.right) * StitchBmpRatio.x,
                                        wrap::toFloat(BitmapSrcRect.bottom) * StitchBmpRatio.y};

  auto const differenceRect = F_RECTANGLE {backingRect.left - zoomedInRect.left,
                                           backingRect.top - zoomedInRect.top,
                                           zoomedInRect.right - backingRect.right,
                                           zoomedInRect.bottom - backingRect.bottom};
  auto const bitmapStitchRatio =
      F_POINT {wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left),
               wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom)};
  BitmapDstRect = {.left   = std::lround(differenceRect.left * bitmapStitchRatio.x),
                   .top    = std::lround(differenceRect.top * bitmapStitchRatio.y),
                   .right  = std::lround(wrap::toFloat(StitchWindowClientRect.right) -
                                        (differenceRect.right * bitmapStitchRatio.x)),
                   .bottom = std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                                         (differenceRect.bottom * bitmapStitchRatio.y))};
  return true;
}

void bitlin(gsl::span<uint8_t> const&  source,
            gsl::span<uint32_t> const& destination,
            COLORREF const             foreground,
            COLORREF const             background) noexcept {
  auto dst = destination.begin();
  for (auto const subSource = source.subspan(0, source.size() - 1U); auto const& src : subSource) {
	auto bits = std::bitset<CHAR_BIT>(src);
	for (auto bitOffset = 0U; bitOffset < CHAR_BIT; ++bitOffset) {
	  auto const offset = bitOffset ^ (CHAR_BIT - 1U);
	  *dst              = bits[offset] ? foreground : background;
	  ++dst;
	}
  }
  if (auto const final = destination.size() % CHAR_BIT) {
	auto const bits = std::bitset<CHAR_BIT>(source.back());
	for (auto bitOffset = 0U; bitOffset < final; ++bitOffset) {
	  auto const offset = bitOffset ^ (CHAR_BIT - 1U);
	  *dst              = bits[offset] ? foreground : background;
	  ++dst;
	}
  }
}

void bitsiz() {
  auto const screenAspectRatio =
      gsl::narrow<float>(UnzoomedRect.cx) / gsl::narrow<float>(UnzoomedRect.cy);
  if (auto const bitmapAspectRatio = gsl::narrow<float>(BitmapWidth) / gsl::narrow<float>(BitmapHeight);
      bitmapAspectRatio > screenAspectRatio) {
	BitmapSizeinStitches = F_POINT {gsl::narrow<float>(UnzoomedRect.cx),
	                                (gsl::narrow<float>(UnzoomedRect.cx) / bitmapAspectRatio)};
  }
  else {
	BitmapSizeinStitches = F_POINT {(gsl::narrow<float>(UnzoomedRect.cy) * bitmapAspectRatio),
	                                gsl::narrow<float>(UnzoomedRect.cy)};
  }
  BmpStitchRatio = F_POINT {(wrap::toFloat(BitmapWidth) / BitmapSizeinStitches.x),
                            (wrap::toFloat(BitmapHeight) / BitmapSizeinStitches.y)};
  StitchBmpRatio = F_POINT {(BitmapSizeinStitches.x / wrap::toFloat(BitmapWidth)),
                            (BitmapSizeinStitches.y / wrap::toFloat(BitmapHeight))};
}

constexpr auto fswap(COLORREF const color) noexcept -> COLORREF {
  // this code compiles to the same assembly as _byteswap_ulong(color) >> 8U, making
  // it a portable version
  auto const swapped = ((color & 0x000000FFU) << 24U) | ((color & 0x0000FF00U) << 8U) |
                       ((color & 0x00FF0000U) >> 8U) | ((color & 0xFF000000U) >> 24U);
  return swapped >> BPB;
}

// ReSharper disable CppParameterMayBeConst
auto getBitmap(HDC hdc, const BITMAPINFO* pbmi, gsl::not_null<uint32_t**> const ppvBits) -> HBITMAP {
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast NOLINTNEXTLINE(readability-qualified-auto)
  auto const bitmap =
      CreateDIBSection(hdc, pbmi, DIB_RGB_COLORS, reinterpret_cast<void**>(ppvBits.get()), nullptr, 0); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (*ppvBits != nullptr) {
	return bitmap;
  }
  throw std::runtime_error("CreateDIBSection failed");
}
// ReSharper restore CppParameterMayBeConst

constexpr auto gudtyp(WORD const bitCount) noexcept -> bool {
  switch (bitCount) {
	case 1U:
	case BPP24:
	case BPP32:
	  return true;
	default:
	  return false;
  }
}

auto loadName(fs::path const& directory, fs::path& fileName) -> bool {
  auto* pFileOpen = gsl::narrow_cast<IFileOpenDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hResult = CoCreateInstance(
      CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (FAILED(hResult) || nullptr == pFileOpen) {
	return false;
  }
  auto dwOptions = DWORD {};
  hResult        = pFileOpen->GetOptions(&dwOptions);
  if (FAILED(hResult)) {
	return false;
  }
  static constexpr auto FILTER_FILE_TYPES = std::array {FLTBMP, FLTALL};
  // NOLINTNEXTLINE(hicpp-signed-bitwise)
  hResult = pFileOpen->SetOptions(dwOptions | FOS_DONTADDTORECENT);
  hResult += pFileOpen->SetFileTypes(wrap::toUnsigned(FILTER_FILE_TYPES.size()), FILTER_FILE_TYPES.data());
  hResult += pFileOpen->SetTitle(L"Open ThrEd File");
#if USE_DEFBDIR
  // If we want to, we can set the default directory rather than using the OS mechanism for last used
  auto* psiFrom = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult += SHCreateItemFromParsingName(directory.wstring().data(), nullptr, IID_PPV_ARGS(&psiFrom));
  hResult += pFileOpen->SetFolder(psiFrom);
  if (nullptr != psiFrom) {
	psiFrom->Release();
  }
#else
  UNREFERENCED_PARAMETER(directory);
#endif
  if (FAILED(hResult)) {
	return false;
  }
  hResult = pFileOpen->Show(nullptr);
  if (FAILED(hResult)) {
	return false;
  }
  auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult     = pFileOpen->GetResult(&pItem);
  if (FAILED(hResult) || nullptr == pItem) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto pszFilePath = PWSTR {nullptr};
  hResult          = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
  if (FAILED(hResult)) {
	return false;
  }
  fileName.assign(pszFilePath);
  CoTaskMemFree(pszFilePath);
  return true;
}

// Move unpacked 24BPP data into packed 24BPP data
void movmap(gsl::span<uint32_t> const& src, std::vector<uint8_t>& buffer) noexcept {
  auto destination = buffer.begin();
  for (auto const& iTBD : src) {
	*gsl::narrow_cast<uint32_t*>(gsl::narrow_cast<void*>(&(*destination))) = iTBD;
	destination += 3;
  }
}

auto nuBit() noexcept -> BOOL {
  BitMapColorStruct.Flags          = CC_ANYCOLOR | CC_RGBINIT;
  BitMapColorStruct.hwndOwner      = ThrEdWindow;
  BitMapColorStruct.lCustData      = 0;
  BitMapColorStruct.lpCustColors   = BitmapBackgroundColors->data();
  BitMapColorStruct.lpfnHook       = nullptr;
  BitMapColorStruct.lpTemplateName = nullptr;
  BitMapColorStruct.rgbResult      = BitmapColor;
  BitMapColorStruct.lStructSize    = sizeof(BitMapColorStruct);
  return ChooseColor(&BitMapColorStruct);
}

void pxlin(FRM_HEAD const& form, uint32_t const start, uint32_t const finish) {
  auto       line     = std::array<POINT, 2> {};
  auto const vertexIt = wrap::next(Instance->FormVertices.begin(), form.vertexIndex);
  auto const vStart   = wrap::next(vertexIt, start);
  auto const vFinish  = wrap::next(vertexIt, finish);

  line[0] = stch2bit(*vStart);
  line[1] = stch2bit(*vFinish);
  wrap::polyline(BitmapDC, line.data(), wrap::toUnsigned(line.size()));
  wrap::polyline(TraceDC, line.data(), wrap::toUnsigned(line.size()));
}

auto saveName(fs::path& fileName) {
  auto* pFileSave = gsl::narrow_cast<IFileSaveDialog*>(nullptr);
#pragma warning(suppress : 26490) // type.1 Don't use reinterpret_cast
  auto hResult = CoCreateInstance(
      CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave)); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
  if (FAILED(hResult) || nullptr == pFileSave) {
	return false;
  }
  constexpr auto FILTER_FILE_TYPES = std::array {FLTBMP, FLTALL};
  hResult = pFileSave->SetFileTypes(wrap::toUnsigned(FILTER_FILE_TYPES.size()), FILTER_FILE_TYPES.data());
  hResult += pFileSave->SetFileTypeIndex(0);
  hResult += pFileSave->SetTitle(L"Save Bitmap");
  auto const bmpName = UTF16BMPname->filename().wstring();
  hResult += pFileSave->SetFileName(bmpName.c_str());
  hResult += pFileSave->SetDefaultExtension(L"bmp");
  if (FAILED(hResult)) {
	return false;
  }
  hResult = pFileSave->Show(nullptr);
  if (FAILED(hResult)) {
	return false;
  }
  auto* pItem = gsl::narrow_cast<IShellItem*>(nullptr);
  hResult     = pFileSave->GetResult(&pItem);
  if (FAILED(hResult) || nullptr == pItem) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto pszFilePath = PWSTR {nullptr};
  hResult          = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
  if (FAILED(hResult)) {
	return false;
  }
  fileName.assign(pszFilePath);
  CoTaskMemFree(pszFilePath);
  return true;
}

auto stch2bit(F_POINT& point) -> POINT {
  if (Instance->StateMap.test(StateFlag::LANDSCAP)) {
	point.y -= wrap::toFloat(UnzoomedRect.cy) - BitmapSizeinStitches.y;
  }
  return POINT {wrap::round<LONG>(BmpStitchRatio.x * point.x),
                wrap::round<LONG>(wrap::toFloat(BitmapHeight) - (BmpStitchRatio.y * point.y))};
}
} // namespace

void bitmap::bfil(COLORREF const& backgroundColor) {
  auto const inverseBackgroundColor = fswap(backgroundColor);
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto hBitmapFile =
      CreateFile(UTF16BMPname->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (hBitmapFile == INVALID_HANDLE_VALUE) {
	displayText::showMessage(IDS_UNOPEN, UTF16BMPname->wstring());
	CloseHandle(hBitmapFile);
	resetBmpFile(true);
	return;
  }
  auto bytesRead = DWORD {};
  if (!wrap::readFile(hBitmapFile, &BitmapFileHeader, sizeof(BitmapFileHeader), &bytesRead, L"ReadFile for BitmapFileHeader in bfil")) {
	auto errorCode = GetLastError();
	CloseHandle(hBitmapFile);
	resetBmpFile(true);
	rpt::reportError(L"ReadFile for BitmapFileHeader in bfil", errorCode);
	return;
  }
  // check for 'BM' signature in the 1st 2 bytes. Use Big Endian order
  if (constexpr auto MB_SIG = 0x4D42; BitmapFileHeader.bfType != MB_SIG) {
	resetBmpFile(true);
	return;
  }
  auto fileHeaderSize = wrap::toSize(BitmapFileHeader.bfOffBits) - sizeof(BitmapFileHeader);
  fileHeaderSize      = std::min(fileHeaderSize, sizeof(BITMAPV4HEADER));
  if (!wrap::readFile(hBitmapFile, &BitmapFileHeaderV4, fileHeaderSize, &bytesRead, L"ReadFile for BitmapFileHeaderV4 in bfil")) {
	return;
  }
  if (!gudtyp(BitmapFileHeaderV4.bV4BitCount)) {
	CloseHandle(hBitmapFile);
	resetBmpFile(true);
	displayText::tabmsg(IDS_BMAP, false);
	return;
  }
  if (!Instance->StateMap.testAndReset(StateFlag::WASESC)) {
	Instance->StateMap.reset(StateFlag::TRSET);
  }
  BitmapWidth  = BitmapFileHeaderV4.bV4Width;
  BitmapHeight = BitmapFileHeaderV4.bV4Height;
  Instance->StateMap.set(StateFlag::INIT);
  ZoomRect.left   = 0.0F;
  ZoomRect.bottom = 0.0F;
  wrap::narrow_cast(ZoomRect.right, UnzoomedRect.cx);
  wrap::narrow_cast(ZoomRect.top, UnzoomedRect.cy);
  BitmapDC = CreateCompatibleDC(StitchWindowDC);
  if (BitmapFileHeaderV4.bV4BitCount == 1) {
	Instance->StateMap.set(StateFlag::MONOMAP);
	constexpr auto SR5 = uint8_t {5}; // Shift Right

	auto bitmapWidthBytes = (gsl::narrow_cast<uint32_t>(BitmapWidth) >> SR5) << 2U;
	if (auto const widthOverflow = BitmapWidth % 32; widthOverflow != 0U) {
	  bitmapWidthBytes += 4U;
	}
	auto const bitmapSizeBytes = bitmapWidthBytes * gsl::narrow<decltype(bitmapWidthBytes)>(BitmapHeight);
	auto monoBitmapData = std::vector<uint8_t> {};
	monoBitmapData.resize(bitmapSizeBytes);
	if (!wrap::readFile(hBitmapFile, monoBitmapData.data(), bitmapSizeBytes, &bytesRead, L"ReadFile for monoBitmapData in bfil")) {
	  return;
	}
	CloseHandle(hBitmapFile);
	auto const flag       = binv(monoBitmapData);
	auto const foreground = gsl::narrow_cast<COLORREF>(flag ? inverseBackgroundColor : BitmapColor);
	auto const background = gsl::narrow_cast<COLORREF>(flag ? BitmapColor : inverseBackgroundColor);
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
	auto const bitmap = getBitmap(BitmapDC, &BitmapInfo, &bits);
	// Synchronize
	GdiFlush();
	auto const spMBD     = gsl::span(monoBitmapData);
	auto const spBits    = gsl::span(bits, wrap::toSize(BitmapWidth * BitmapHeight));
	auto       srcOffset = 0U;
	auto       dstOffset = 0U;
	auto const srcWidth  = (wrap::toUnsigned(BitmapWidth) >> 3U) + 1U;
	for (auto iHeight = 0; iHeight < BitmapHeight; ++iHeight) {
	  auto const spLineSrc = spMBD.subspan(srcOffset, srcWidth);
	  auto const spLineDst = spBits.subspan(dstOffset, wrap::toSize(BitmapWidth));
	  bitlin(spLineSrc, spLineDst, foreground, background);
	  srcOffset += bitmapWidthBytes;
	  dstOffset += wrap::toUnsigned(BitmapWidth);
	}
	// NOLINTNEXTLINE(readability-qualified-auto)
	if (auto const deviceContext = CreateCompatibleDC(StitchWindowDC); deviceContext != nullptr) {
	  SelectObject(deviceContext, bitmap);
	  hBitmapFile = CreateCompatibleBitmap(StitchWindowDC, BitmapWidth, BitmapHeight);
	  SelectObject(BitmapDC, hBitmapFile);
	  BitBlt(BitmapDC, 0, 0, BitmapWidth, BitmapHeight, deviceContext, 0, 0, SRCCOPY);
	  DeleteObject(bitmap);
	  DeleteObject(deviceContext);
	}
  }
  else {
	CloseHandle(hBitmapFile);
	Instance->StateMap.reset(StateFlag::MONOMAP);
	hBitmapFile = LoadImage(
	    ThrEdInstance, UTF16BMPname->wstring().c_str(), IMAGE_BITMAP, BitmapWidth, BitmapHeight, LR_LOADFROMFILE);
	SelectObject(BitmapDC, hBitmapFile);
	Instance->StateMap.set(StateFlag::RESTCH);
  }
  bitsiz();
  trace::initColorRef();
  Instance->StateMap.reset(StateFlag::HIDMAP);
}

void bitmap::resetBmpFile(bool const reset) {
  if (!ismap()) {
	return;
  }
  DeleteObject(BitmapFileHandle);
  ReleaseDC(ThrEdWindow, BitmapDC);
  if (reset) {
	UTF8BMPname.fill(0);
  }
}

void bitmap::savmap() {
  if (ismap() && nullptr != TraceBitmapData) {
	displayText::tabmsg(IDS_SHOMAP, false);
	return;
  }
  if (Instance->StateMap.test(StateFlag::MONOMAP)) {
	displayText::tabmsg(IDS_SAVMAP, false);
	return;
  }
  if (!Instance->StateMap.test(StateFlag::WASTRAC)) {
	displayText::tabmsg(IDS_MAPCHG, false);
	return;
  }
  auto fileName = fs::path {};
  if (!saveName(fileName)) {
	return;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const hBitmap =
      CreateFile(fileName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (hBitmap == INVALID_HANDLE_VALUE) {
	displayText::crmsg(*UTF16BMPname);
	return;
  }
  auto bytesWritten = DWORD {};
  WriteFile(hBitmap, &BitmapFileHeader, sizeof(BitmapFileHeader), &bytesWritten, nullptr);
  WriteFile(hBitmap, &BitmapFileHeaderV4, BitmapFileHeader.bfOffBits - sizeof(BitmapFileHeader), &bytesWritten, nullptr);
  auto buffer = std::vector<uint8_t> {};
  buffer.resize((wrap::toSize(BitmapWidth) * wrap::toUnsigned(BitmapHeight) * 3U) + 1U);
  auto const spTrace = gsl::span(TraceBitmapData, wrap::toSize(BitmapWidth * BitmapHeight));
  movmap(spTrace, buffer);
  WriteFile(hBitmap, buffer.data(), gsl::narrow<DWORD>(BitmapWidth * BitmapHeight * 3), &bytesWritten, nullptr);
  CloseHandle(hBitmap);
}

void bitmap::lodbmp(fs::path const& directory) {
  if (!loadName(directory, *UTF16BMPname)) {
	return;
  }
  resetBmpFile(false);
  trace::untrace();
#if USE_SHORT_NAME
  auto const pleng = GetShortPathName(UTF16BMPname->wstring().c_str(), nullptr, 0);
  auto       dest  = std::vector<wchar_t> {};
  dest.resize(pleng);
  GetShortPathName(UTF16BMPname->wstring().c_str(), dest.data(), wrap::toUnsigned(dest.size()));
  auto const filePart = fs::path {dest.data()};
  auto       saveFile = utf::utf16ToUtf8(filePart.filename().wstring());
#else
  auto const saveFile = utf::utf16ToUtf8(UTF16BMPname->filename().wstring());
#endif
  if (!saveFile.empty() && saveFile.size() < UTF8BMPname.size()) {
	std::ranges::copy(saveFile, UTF8BMPname.begin());
	bfil(BackgroundColor);
  }
  else {
	// THR version 2 file can only store a 16 character filename
	// Give the user a little more info why the bitmap has not been loaded
	displayText::showMessage(IDS_BMPLONG, ThrName->wstring());
  }
  Instance->StateMap.set(StateFlag::RESTCH);
}

void bitmap::setBmpColor() {
  if (nuBit() == 0U) {
	return;
  }
  BitmapColor = fswap(BitMapColorStruct.rgbResult);
  if (ismap()) {
	bfil(BackgroundColor);
  }
  thred::nuPen(BitmapPen, 1, BitmapColor);
  thred::zumhom();
}

void bitmap::setBBCV(std::vector<COLORREF>* value) noexcept {
  BitmapBackgroundColors = value;
}

void bitmap::setUBfilename(fs::path* fileName) noexcept {
  UTF16BMPname = fileName;
}

void bitmap::assignUBFilename(fs::path const& directory) {
  current_path(directory);
  auto const bmpFileName = utf::utf8ToUtf16(std::string(UTF8BMPname.data()));
  auto const fullPath    = directory / bmpFileName;
  UTF16BMPname->assign(fullPath);
  bfil(BackgroundColor);
}

auto bitmap::getBitmapSizeinStitches() noexcept -> F_POINT {
  return BitmapSizeinStitches;
}

auto bitmap::getBmpBackColor(uint32_t const& index) noexcept -> COLORREF {
  return BitmapBackgroundColors->operator[](index);
}

void bitmap::setBmpBackColor() {
  constexpr auto DEFAULT_COLORS = std::array<COLORREF, COLORCNT> {0x00c0d5bf,
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
  BitmapBackgroundColors->clear();
  BitmapBackgroundColors->resize(DEFAULT_COLORS.size());
  std::ranges::copy(DEFAULT_COLORS, BitmapBackgroundColors->begin());
}

auto bitmap::getBmpColor() noexcept -> COLORREF {
  return BitmapColor;
}

auto bitmap::getBmpStitchRatio() noexcept -> F_POINT {
  return BmpStitchRatio;
}

auto bitmap::getBitmapHeight() noexcept -> LONG {
  return BitmapHeight;
}

auto bitmap::getBitmapDC() noexcept -> HDC {
  return BitmapDC;
}

auto bitmap::getTraceDC() noexcept -> HDC {
  return TraceDC;
}

auto bitmap::getBitmapWidth() noexcept -> LONG {
  return BitmapWidth;
}

void bitmap::setBitmapColor(COLORREF const& newColor) noexcept {
  BitmapColor = newColor;
}

// ReSharper disable once CppParameterMayBeConst
void bitmap::setBitmapPen(HPEN pen) noexcept {
  BitmapPen = pen;
}
// ReSharper restore CppParameterMayBeConst

void bitmap::resetDC() noexcept {
  DeleteObject(TraceBitmap);
  DeleteObject(TraceDC);
}

auto bitmap::getBmpNameLength() noexcept(std::is_same_v<size_t, uint32_t>) -> uint32_t {
  return wrap::toUnsigned(UTF8BMPname.size() - 1); // -1 to account for zero termination
}

auto bitmap::getBmpNameData() noexcept -> char* {
  return UTF8BMPname.data();
}

auto bitmap::ismap() noexcept -> bool {
  return UTF8BMPname[0] != 0;
}

void bitmap::chkbit() {
  if (ismap() && (Instance->StateMap.test(StateFlag::WASDIF) || Instance->StateMap.test(StateFlag::WASDSEL) ||
                  Instance->StateMap.test(StateFlag::WASBLAK))) {
	Instance->StateMap.set(StateFlag::WASESC);
	bfil(BackgroundColor);
  }
}

void bitmap::delmap() {
  UTF8BMPname.fill(0);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void bitmap::drawBmpBackground() {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto deviceContext = BitmapDC;
  if (Instance->StateMap.test(StateFlag::WASTRAC)) {
	deviceContext = TraceDC;
  }
  if (!bitar()) {
	return;
  }
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

  auto const info = BITMAPINFO {header, {RGBQUAD {0, 0, 0, 0}}};
  TraceBitmap     = getBitmap(BitmapDC, &info, &TraceBitmapData);
  TraceDC         = CreateCompatibleDC(StitchWindowDC);
  auto bitmapSize = 0U;
  if (TraceDC != nullptr) {
	SelectObject(TraceDC, TraceBitmap);
	BitBlt(TraceDC, 0, 0, BitmapWidth, BitmapHeight, BitmapDC, 0, 0, SRCCOPY);
	Instance->StateMap.set(StateFlag::WASTRAC);
	bitmapSize = (wrap::toUnsigned(BitmapWidth) + 1U) * (wrap::toUnsigned(BitmapHeight) + 1U);
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

void bitmap::bfrm(FRM_HEAD const& form) {
  if (form.vertexCount == 0U) {
	return;
  }
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	pxlin(form, iVertex, iVertex + 1U);
  }
  if (form.type != FRMLINE) {
	pxlin(form, form.vertexCount - 1U, 0);
  }
}
