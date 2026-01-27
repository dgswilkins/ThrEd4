// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "bitmap.h"
#include "displayText.h"
#include "EnumMap.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "PCS.h"
#include "point.h"
#include "Resources/resource.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/narrow"
#include "gsl/span"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <fileapi.h>
#include <handleapi.h>
#include <minwindef.h>
#include <windef.h>
#include <winnt.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
// ReSharper disable CppUnusedIncludeDirective
#include <cstdint>
// ReSharper restore CppUnusedIncludeDirective
#include <cwctype>
#include <filesystem>
#include <string>
#include <vector>

#pragma pack(push, 1)

namespace {
constexpr auto PCSBMPNSZ = 14;            // Bitmap filename maximum length in PCS spec
constexpr auto LEADIN    = int8_t {0x32}; // LEADIN marker for PCS file

constexpr auto LARGE_HOOP = F_POINT {LHUPX, LHUPY}; // Large hoop size
constexpr auto SMALL_HOOP = F_POINT {SHUPX, SHUPY}; // Small hoop size

} // namespace

class PCSHEADER // pcs file header structure
{
  public:
  constexpr PCSHEADER() noexcept = default;

  void writeHeader(std::array<COLORREF, COLORCNT> userColor) {
	m_leadIn     = LEADIN;
	m_colorCount = COLORCNT;
	wrap::narrow(m_stitchCount, Instance->stitchBuffer.size());
	auto const spColors = gsl::span {m_colors};
	std::ranges::copy(userColor, spColors.begin());
  }

  constexpr void readHeader(std::array<COLORREF, COLORCNT>& userColor) {
	std::ranges::copy(m_colors, userColor.begin());
  }

  [[nodiscard]] auto isValid() const noexcept -> bool {
	return m_leadIn == LEADIN && m_colorCount == COLORCNT;
  }

  constexpr void setHoopType(bool const flag) noexcept {
	m_hoopType = wrap::toIntegralType(flag ? HoopSize::kLarge : HoopSize::kSmall);
  }

  [[nodiscard]] constexpr auto getStitchCount() const noexcept -> uint16_t {
	return m_stitchCount;
  }

  [[nodiscard]] constexpr auto getHoopType() const noexcept -> HoopSize {
	if (m_hoopType < wrap::toIntegralType(HoopSize::kSetCustom) ||
	    m_hoopType > wrap::toIntegralType(HoopSize::kUserDefined)) {
	  return HoopSize::kSmall;
	}
	return wrap::toEnumType<HoopSize>(m_hoopType);
  }

  private:
  int8_t   m_leadIn {};
  int8_t   m_hoopType {};
  uint16_t m_colorCount {};

  std::array<COLORREF, COLORCNT> m_colors {};

  uint16_t m_stitchCount {};
};
#pragma pack(pop)

#pragma pack(push, 1)
class PCS_STITCH
{
  public:
  uint8_t fx {}; // fractional part of x coord or color if tag = 3
  int16_t x {};  // integer part of x coord
  uint8_t nx {};
  uint8_t fy {}; // fractional part of y coord
  int16_t y {};  // integer part of y coord
  uint8_t ny {};
  uint8_t tag {};
};
#pragma pack(pop)

namespace {
auto PCSHeader = PCSHEADER {}; // pcs file header

// Definitions
auto pcshup(std::vector<F_POINT_ATTR>& stitches) -> bool;

// Functions
auto pcshup(std::vector<F_POINT_ATTR>& stitches) -> bool {
  auto minX = stitches[0].x;
  auto minY = stitches[0].y;
  auto maxX = stitches[0].x;
  auto maxY = stitches[0].y;

  for (const auto& stitch : stitches) {
	minX = std::min(minX, stitch.x);
	minY = std::min(minY, stitch.y);
	maxX = std::max(maxX, stitch.x);
	maxY = std::max(maxY, stitch.y);
  }

  auto const boundingSize = F_POINT {maxX - minX, maxY - minY};
  if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {
	displayText::tabmsg(IDS_PFAF2L, false);
	return false;
  }
  auto const largeFlag =
      boundingSize.x > SHUPX || boundingSize.y > SHUPY ||
      (util::closeEnough(IniFile.hoopSizeX, LHUPX) && util::closeEnough(IniFile.hoopSizeY, LHUPY));
  auto const hoopSize = largeFlag ? LARGE_HOOP : SMALL_HOOP;
  PCSHeader.setHoopType(largeFlag);
  auto delta = F_POINT {};
  if (maxX > hoopSize.x) {
	delta.x = hoopSize.x - maxX;
  }
  if (maxY > hoopSize.y) {
	delta.y = hoopSize.y - maxY;
  }
  if (minX < 0) {
	delta.x = -minX;
  }
  if (minY < 0) {
	delta.y = -minY;
  }
  if (delta.x != 0.0F || delta.y != 0.0F) {
	for (auto& offsetStitch : stitches) {
	  offsetStitch += delta;
	}
  }
  return true;
}

} // namespace

auto PCS::savePCS(fs::path const& auxName, std::vector<F_POINT_ATTR>& saveStitches) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const fileHandle = CreateFile(
      auxName.wstring().c_str(), GENERIC_WRITE | GENERIC_READ, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(auxName);
	return false;
  }
  auto pcsStitchBuffer = std::vector<PCS_STITCH> {};
  PCSHeader.writeHeader(UserColor);
  if (!pcshup(saveStitches)) {
	CloseHandle(fileHandle);
	return false;
  }
  auto bytesWritten = DWORD {};
  if (FALSE == WriteFile(fileHandle, &PCSHeader, sizeof(PCSHeader), &bytesWritten, nullptr)) {
	displayText::riter();
	CloseHandle(fileHandle);
	return false;
  }
  auto savcol = COLMSK;
  pcsStitchBuffer.reserve(Instance->stitchBuffer.size() + thred::maxColor());
  for (auto const& stitch : saveStitches) {
	if ((stitch.attribute & COLMSK) != savcol) {
	  savcol      = stitch.attribute & COLMSK;
	  auto colRec = PCS_STITCH {};
	  colRec.tag  = 3;
	  wrap::narrow(colRec.fx, savcol);
	  pcsStitchBuffer.push_back(colRec);
	}
	auto stitchRec      = PCS_STITCH {};
	auto integerPart    = 0.0F;
	auto fractionalPart = std::modf(stitch.x, &integerPart);
	stitchRec.fx        = wrap::floor<decltype(stitchRec.fx)>(fractionalPart * FRACFACT);
	wrap::narrow(stitchRec.x, integerPart);
	fractionalPart = std::modf(stitch.y, &integerPart);
	stitchRec.fy   = wrap::floor<decltype(stitchRec.fy)>(fractionalPart * FRACFACT);
	wrap::narrow(stitchRec.y, integerPart);
	pcsStitchBuffer.push_back(stitchRec);
  }
  if (FALSE == WriteFile(fileHandle, pcsStitchBuffer.data(), wrap::sizeofVector(pcsStitchBuffer), &bytesWritten, nullptr)) {
	displayText::riter();
	CloseHandle(fileHandle);
	return false;
  }
  if (Instance->userFlagMap.test(UserFlag::BSAVOF)) {
	constexpr auto BLANK = std::array<char, PCSBMPNSZ> {};
	if (FALSE == WriteFile(fileHandle, BLANK.data(), wrap::toUnsigned(BLANK.size()), &bytesWritten, nullptr)) {
	  displayText::riter();
	  CloseHandle(fileHandle);
	  return false;
	}
	CloseHandle(fileHandle);
	return true;
  }
  if (FALSE == WriteFile(fileHandle, bitmap::getBmpNameData(), PCSBMPNSZ, &bytesWritten, nullptr)) {
	displayText::riter();
	CloseHandle(fileHandle);
	return false;
  }
  CloseHandle(fileHandle);
  return true;
}

auto PCS::readPCSFile(fs::path const& newFileName) -> bool {
  auto fileSize = uintmax_t {};
  if (!thred::getFileSize(newFileName, fileSize)) {
	return false;
  }
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	return false;
  }
  auto bytesRead = DWORD {};
  if (!wrap::readFile(fileHandle, &PCSHeader, sizeof(PCSHeader), &bytesRead, L"ReadFile for PCSHeader in readPCSFile")) {
	return false;
  }
  if (bytesRead != sizeof(PCSHeader)) {
	CloseHandle(fileHandle);
	return false;
  }
  if (!PCSHeader.isValid()) {
	CloseHandle(fileHandle);
	return false;
  }
  PCSHeader.readHeader(UserColor);
  fileSize -= uintmax_t {PCSBMPNSZ} + sizeof(PCSHeader);
  auto const pcsStitchCount = wrap::toSize(fileSize / sizeof(PCS_STITCH));
  auto       pcsDataBuffer  = std::vector<PCS_STITCH> {};
  pcsDataBuffer.resize(pcsStitchCount);
  if (!wrap::readFile(fileHandle, pcsDataBuffer.data(), fileSize, &bytesRead, L"ReadFile for pcsDataBuffer in readPCSFile")) {
	return false;
  }
  if (bytesRead != gsl::narrow<DWORD>(fileSize)) {
	CloseHandle(fileHandle);
	return false;
  }
  auto iStitch = uint16_t {};
  auto color   = 0U;
  Instance->stitchBuffer.clear();
  Instance->stitchBuffer.reserve(PCSHeader.getStitchCount());
  for (auto const& stitch : pcsDataBuffer) {
	if (stitch.tag == 3) {
	  thred::addColor(iStitch, stitch.fx);
	  color = NOTFRM | stitch.fx;
	  continue;
	}
	Instance->stitchBuffer.emplace_back(wrap::toFloat(stitch.x) + (wrap::toFloat(stitch.fx) / FRACFACT),
	                                    wrap::toFloat(stitch.y) + (wrap::toFloat(stitch.fy) / FRACFACT),
	                                    color);
	if (iStitch++ >= PCSHeader.getStitchCount()) {
	  break;
	}
  }
  // Grab the bitmap filename
  if (!wrap::readFile(fileHandle, bitmap::getBmpNameData(), PCSBMPNSZ, &bytesRead, L"ReadFile for getBmpNameData in readPCSFile")) {
	return false;
  }
  if (bytesRead != PCSBMPNSZ) {
	outDebugString(L"readPCSFile: description bytesRead {}\n", bytesRead);
	return false;
  }
  IniFile.auxFileType = Machine::kPfaff;
  auto hoopSize       = PCSHeader.getHoopType();
  if (hoopSize != HoopSize::kLarge && hoopSize != HoopSize::kSmall) {
	hoopSize = HoopSize::kLarge;
  }
  auto stitchRect = F_RECTANGLE {};
  thred::stchrct(stitchRect);
  if (stitchRect.left < 0 || stitchRect.right > LHUPY || stitchRect.bottom < 0 || stitchRect.top > LHUPY) {
	IniFile.hoopSizeX = LHUPX;
	IniFile.hoopSizeY = LHUPY;
	thred::chkhup();
	CloseHandle(fileHandle);
	return true;
  }
  if (hoopSize == HoopSize::kLarge) {
	IniFile.hoopType  = HoopSize::kLarge;
	IniFile.hoopSizeX = LHUPX;
	IniFile.hoopSizeY = LHUPY;
	CloseHandle(fileHandle);
	return true;
  }
  if (stitchRect.right > SHUPX || stitchRect.top > SHUPY) {
	IniFile.hoopType  = HoopSize::kLarge;
	IniFile.hoopSizeX = SHUPX;
	IniFile.hoopSizeY = SHUPY;
	CloseHandle(fileHandle);
	return true;
  }
  IniFile.hoopType  = HoopSize::kSmall;
  IniFile.hoopSizeX = SHUPX;
  IniFile.hoopSizeY = SHUPY;
  CloseHandle(fileHandle);
  return true;
}

auto PCS::isPCS(fs::path const& path) -> bool {
  auto extention = path.extension().wstring();
  std::ranges::transform(extention, extention.begin(), towlower);
  return extention.starts_with(L".pcs");
}

auto PCS::insPCS(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const fileHandle =
      CreateFile(insertedFile.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::filnopn(IDS_FNOPN, insertedFile);
	return false;
  }
  auto pcsFileHeader = PCSHEADER {};
  auto bytesRead     = DWORD {};

  if (!wrap::readFile(fileHandle, &pcsFileHeader, sizeof(pcsFileHeader), &bytesRead, L"ReadFile for pcsFileHeader in insPCS")) {
	return false;
  }
  if (!pcsFileHeader.isValid()) {
	// ToDo - Add error message
	CloseHandle(fileHandle);
	return false;
  }
  auto fileSize = uintmax_t {};
  thred::getFileSize(insertedFile, fileSize);
  fileSize -= sizeof(pcsFileHeader) + PCSBMPNSZ;
  auto const pcsStitchCount  = wrap::toSize(fileSize / sizeof(PCS_STITCH));
  auto       pcsStitchBuffer = std::vector<PCS_STITCH> {};
  pcsStitchBuffer.resize(pcsStitchCount);
  if (!wrap::readFile(fileHandle, pcsStitchBuffer.data(), fileSize, &bytesRead, L"ReadFile for pcsStitchBuffer in insPCS")) {
	return false;
  }
  if (bytesRead != fileSize) {
	Instance->stateMap.reset(StateFlag::INIT);
	displayText::tabmsg(IDS_SHRTF, false);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	CloseHandle(fileHandle);
	return false;
  }
  thred::savdo();
  Instance->stitchBuffer.reserve(Instance->stitchBuffer.size() + pcsStitchCount);
  auto newAttribute = 0U;

  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;
  for (auto skipStitch = false; auto const& stitch : pcsStitchBuffer) {
	if (skipStitch) {
	  skipStitch = false;
	  continue;
	}
	if (stitch.tag == 3) {
	  newAttribute = stitch.fx | NOTFRM;
	  skipStitch   = true;
	  continue;
	}
	auto xVal = wrap::toFloat(stitch.x) + (wrap::toFloat(stitch.fx) / FRACFACT);
	auto yVal = wrap::toFloat(stitch.y) + (wrap::toFloat(stitch.fy) / FRACFACT);

	minX = std::min(minX, xVal);
	minY = std::min(minY, yVal);
	maxX = std::max(maxX, xVal);
	maxY = std::max(maxY, yVal);
	Instance->stitchBuffer.emplace_back(xVal, yVal, newAttribute);
  }
  insertedRectangle = F_RECTANGLE {minX, maxY, maxX, minY};
  CloseHandle(fileHandle);
  return true;
}
