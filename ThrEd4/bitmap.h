#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace bitmap {
void assignUBFilename(fs::path const& directory);
void bitbltBitmap() noexcept;
void bfrm(FRM_HEAD const& form);
void chkbit();
void delmap();
void drawBmpBackground();
auto getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP;
auto getBitmapDC() noexcept -> HDC;
auto getBitmapHeight() noexcept -> int;
auto getBitmapSizeinStitches() noexcept -> F_POINT;
auto getBitmapWidth() noexcept -> int;
auto getBmpColor() noexcept -> COLORREF;
auto getBmpBackColor(uint32_t const& index) noexcept -> COLORREF;

auto getBmpNameLength() -> uint32_t;
auto getBmpNameData() noexcept -> char*;

auto getBmpStitchRatio() noexcept -> F_POINT;
auto getrmap() -> uint32_t;
auto getTraceDC() noexcept -> HDC;
auto ismap() noexcept -> bool;
void resetBmpFile(bool reset);
void resetDC() noexcept;
void savmap();
void lodbmp(fs::path const* directory);
void setBBCV(std::vector<COLORREF>* value) noexcept;
void setBmpBackColor();
void setBmpColor();
void setBitmapColor(COLORREF const& newColor) noexcept;
void setBitmapPen(HPEN pen) noexcept;
void setUBfilename(fs::path* fileName) noexcept;
namespace internal {
  void bfil(COLORREF const& backgroundColor);
  auto binv(std::vector<uint8_t> const& monoBitmapData, uint32_t bitmapWidthInBytes) -> bool;
  auto bitar() -> bool;
  void bitlin(uint8_t const* source, uint32_t* destination, uint32_t bitmapWidthBytes, COLORREF foreground, COLORREF background);
  void bitsiz();
  constexpr auto fswap(COLORREF color) noexcept -> COLORREF;
  constexpr auto gudtyp(WORD bitCount) noexcept -> bool;

  void movmap(int cnt, uint8_t* buffer);
  auto nuBit() noexcept -> BOOL;
  auto loadName(fs::path const* directory, fs::path* fileName) -> bool;
  void pxlin(FRM_HEAD const& form, uint32_t start, uint32_t finish);
  auto saveName(fs::path& fileName);
  auto stch2bit(F_POINT& point) -> POINT;
} // namespace internal
} // namespace bitmap
