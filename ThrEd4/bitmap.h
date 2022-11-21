#pragma once

// Local Headers
#include "formHeader.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace bitmap {
void assignUBFilename(fs::path const& directory);
void bfil(COLORREF const& backgroundColor);
void bfrm(FRM_HEAD const& form);
void bitbltBitmap() noexcept;
void chkbit();
void delmap();
void drawBmpBackground();
auto getBitmap(_In_ HDC hdc, _In_ const BITMAPINFO* pbmi, _Outptr_ uint32_t** ppvBits) -> HBITMAP;
auto getBitmapDC() noexcept -> HDC;
auto getBitmapHeight() noexcept -> int;
auto getBitmapSizeinStitches() noexcept -> F_POINT;
auto getBitmapWidth() noexcept -> int;
auto getBmpBackColor(uint32_t const& index) noexcept -> COLORREF;
auto getBmpColor() noexcept -> COLORREF;
auto getBmpNameData() noexcept -> char*;
auto getBmpNameLength() -> uint32_t;
auto getBmpStitchRatio() noexcept -> F_POINT;
auto getTraceDC() noexcept -> HDC;
auto getrmap() -> uint32_t;
auto ismap() noexcept -> bool;
void lodbmp(fs::path const* directory);
void resetBmpFile(bool reset);
void resetDC() noexcept;
void savmap();
void setBBCV(std::vector<COLORREF>* value) noexcept;
void setBitmapColor(COLORREF const& newColor) noexcept;
void setBitmapPen(HPEN pen) noexcept;
void setBmpBackColor();
void setBmpColor();
void setUBfilename(fs::path* fileName) noexcept;
} // namespace bitmap
