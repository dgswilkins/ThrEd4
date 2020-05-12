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
void bfrm(FRMHED const& form);
void drawBmpBackground();
auto getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP;
auto getBitmapDC() noexcept -> HDC;
auto getBitmapHeight() noexcept -> uint32_t;
auto getBitmapSizeinStitches() noexcept -> fPOINT;
auto getBitmapWidth() noexcept -> uint32_t;
auto getBmpColor() noexcept -> COLORREF;
auto getBmpBackColor(uint32_t const& index) noexcept -> COLORREF;
auto getBmpStitchRatio() noexcept -> fPOINT;
auto getrmap() -> uint32_t;
void resetBmpFile(bool reset) noexcept;
void savmap();
void lodbmp();
void setBmpBackColor() noexcept;
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
  auto constexpr fswap(COLORREF color) noexcept -> COLORREF;
  auto constexpr gudtyp(WORD bitCount) noexcept -> bool;
  void movmap(uint32_t cnt, uint8_t* buffer);
  auto nuBit() noexcept -> COLORREF;
  auto loadName(fs::path* fileName)->bool;
  void pxlin(FRMHED const& form, uint32_t start, uint32_t finish);
  auto saveName(fs::path& fileName);
  auto stch2bit(fPOINT& point) -> POINT;
} // namespace internal
} // namespace bitmap
