#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
//#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
//#include <filesystem>
//#include <vector>

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26814)
//#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
//#include "ThrEdTypes.h"

namespace bitmap {
void assignDefBmpDir(std::wstring const& directory);
void assignUBFilename(fs::path const& directory);
void bitbltBitmap() noexcept;
void bfrm(FRMHED const& form);
void drawBmpBackground();
auto getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP;
auto getBitmapDC() noexcept -> HDC;
auto getBitmapHeight() noexcept -> uint32_t;
auto getBitmapSizeinStitches() noexcept -> fPOINT;
auto getBitmapWidth() noexcept -> uint32_t;
auto getBmpColor() noexcept -> COLORREF;
auto getBmpBackColor(uint32_t index) noexcept -> COLORREF;
auto getBmpStitchRatio() noexcept -> fPOINT;
auto getrmap() -> uint32_t;
void resetBmpFile(bool reset) noexcept;
void savmap();
void lodbmp();
void setBmpBackColor() noexcept;
void setBmpColor();
void setBitmapColor(COLORREF newColor) noexcept;
void setBitmapPen(HPEN& pen) noexcept;
void setDefBmpDir(fs::path* directory) noexcept;
void setUBfilename(fs::path* fileName) noexcept;
namespace internal {
  void bfil(COLORREF const& backgroundColor);
  auto binv(std::vector<uint8_t> const& monoBitmapData, uint32_t bitmapWidthInBytes) -> bool;
  auto bitar() -> bool;
  void bitlin(uint8_t const* source, uint32_t* destination, COLORREF foreground, COLORREF background);
  void bitsiz();
  void defbNam();
  auto fswap(COLORREF color) noexcept -> COLORREF;

  constexpr auto gudtyp(WORD bitCount) noexcept -> bool;

  void movmap(uint32_t cnt, uint8_t* buffer);
  auto nuBit() noexcept -> COLORREF;
  void pxlin(FRMHED const& form, uint32_t start, uint32_t finish);
  auto stch2bit(fPOINT& point) -> POINT;
} // namespace internal
} // namespace bitmap
