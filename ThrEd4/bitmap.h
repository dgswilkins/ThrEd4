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
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <cstdint>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace bitmap {
void assignUBFilename(fs::path const& directory);
void bitbltBitmap() noexcept;
void bfil(COLORREF const& backgroundColor);
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
} // namespace bitmap
