// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "bitmap.h"
#include "displayText.h"
#include "globals.h"
#include "PCS.h"
#include "thred.h"

#pragma pack(push, 1)

class PCSHEADER // pcs file header structure
{
  public:
  int8_t   leadIn {};
  int8_t   hoopType {};
  uint16_t colorCount {};
  COLORREF colors[COLORCNT] {}; // NOLINT(modernize-avoid-c-arrays)
  uint16_t stitchCount {};

  constexpr PCSHEADER() noexcept = default;
  // PCSHEADER(PCSHEADER&&) = default;
  // PCSHEADER& operator=(PCSHEADER const& rhs) = default;
  // PCSHEADER& operator=(PCSHEADER&&) = default;
  //~PCSHEADER() = default;
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

  // constexpr PCS_STITCH() noexcept = default;
  // PCS_STITCH(PCS_STITCH const&) = default;
  // PCS_STITCH(PCS_STITCH&&) = default;
  // PCS_STITCH& operator=(PCS_STITCH const& rhs) = default;
  // PCS_STITCH& operator=(PCS_STITCH&&) = default;
  //~PCS_STITCH() = default;
};
#pragma pack(pop)

constexpr auto LARGE_HOOP = F_POINT {LHUPX, LHUPY};
constexpr auto SMALL_HOOP = F_POINT {SHUPX, SHUPY};

// PCS internal namespace
namespace pci {
auto pcshup(std::vector<F_POINT_ATTR>& stitches) -> bool;
} // namespace pci

namespace {
auto PCSHeader = PCSHEADER {}; // pcs file header
} // namespace

auto PCS::savePCS(fs::path const& auxName, std::vector<F_POINT_ATTR>& saveStitches) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const fileHandle = CreateFile(
      auxName.wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT(hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::crmsg(auxName);
	return false;
  }
  PCSHeader.leadIn     = 0x32;
  PCSHeader.colorCount = COLORCNT;
  auto pcsStitchBuffer = std::vector<PCS_STITCH> {};
  wrap::narrow(PCSHeader.stitchCount, StitchBuffer->size());
  auto const spColors = gsl::span {PCSHeader.colors};
  std::ranges::copy(UserColor, spColors.begin());
  if (!pci::pcshup(saveStitches)) {
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
  pcsStitchBuffer.reserve(StitchBuffer->size() + thred::maxColor());
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
  if (UserFlagMap->test(UserFlag::BSAVOF)) {
	constexpr auto BLANK = std::array<char, 14> {};
	if (FALSE == WriteFile(fileHandle, BLANK.data(), wrap::toUnsigned(BLANK.size()), &bytesWritten, nullptr)) {
	  displayText::riter();
	  CloseHandle(fileHandle);
	  return false;
	}
	CloseHandle(fileHandle);
	return true;
  }
  if (FALSE == WriteFile(fileHandle, bitmap::getBmpNameData(), 14, &bytesWritten, nullptr)) {
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
  if (PCSHeader.leadIn != '2' || PCSHeader.colorCount != COLORCNT) {
	CloseHandle(fileHandle);
	return false;
  }
  std::ranges::copy(PCSHeader.colors, UserColor.begin());
  fileSize -= uintmax_t {14U} + sizeof(PCSHeader);
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
  StitchBuffer->clear();
  StitchBuffer->reserve(PCSHeader.stitchCount);
  for (auto const& stitch : pcsDataBuffer) {
	if (stitch.tag == 3) {
	  thred::addColor(iStitch, stitch.fx);
	  color = NOTFRM | stitch.fx;
	  continue;
	}
	StitchBuffer->push_back(F_POINT_ATTR {wrap::toFloat(stitch.x) + wrap::toFloat(stitch.fx) / FRACFACT,
	                                      wrap::toFloat(stitch.y) + wrap::toFloat(stitch.fy) / FRACFACT,
	                                      color});
	if (iStitch++ >= PCSHeader.stitchCount) {
	  break;
	}
  }
  // Grab the bitmap filename
  if (!wrap::readFile(fileHandle, bitmap::getBmpNameData(), 14, &bytesRead, L"ReadFile for getBmpNameData in readPCSFile")) {
	return false;
  }
  if (bytesRead != 14) {
	// NOLINTNEXTLINE
	outDebugString(L"readPCSFile: description bytesRead {}\n", bytesRead);
	return false;
  }
  IniFile.auxFileType = AUXPCS;
  if (PCSHeader.hoopType != LARGHUP && PCSHeader.hoopType != SMALHUP) {
	PCSHeader.hoopType = LARGHUP;
  }
  auto stitchRect = F_RECTANGLE {};
  thred::sizstch(stitchRect, *StitchBuffer);
  if (stitchRect.left < 0 || stitchRect.right > LHUPY || stitchRect.bottom < 0 || stitchRect.top > LHUPY) {
	IniFile.hoopSizeX = LHUPX;
	IniFile.hoopSizeY = LHUPY;
	thred::chkhup();
	CloseHandle(fileHandle);
	return true;
  }
  if (PCSHeader.hoopType == LARGHUP) {
	IniFile.hoopType  = LARGHUP;
	IniFile.hoopSizeX = LHUPX;
	IniFile.hoopSizeY = LHUPY;
	CloseHandle(fileHandle);
	return true;
  }
  if (stitchRect.right > SHUPX || stitchRect.top > SHUPY) {
	IniFile.hoopType  = LARGHUP;
	IniFile.hoopSizeX = SHUPX;
	IniFile.hoopSizeY = SHUPY;
	CloseHandle(fileHandle);
	return true;
  }
  IniFile.hoopType  = SMALHUP;
  IniFile.hoopSizeX = SHUPX;
  IniFile.hoopSizeY = SHUPY;
  CloseHandle(fileHandle);
  return true;
}

auto pci::pcshup(std::vector<F_POINT_ATTR>& stitches) -> bool {
  auto boundingRect = F_RECTANGLE {stitches[0].y, stitches[0].x, stitches[0].x, stitches[0].y};
  for (auto const& stitch : stitches) {
	if (stitch.x < boundingRect.left) {
	  boundingRect.left = stitch.x;
	}
	if (stitch.x > boundingRect.right) {
	  boundingRect.right = stitch.x;
	}
	if (stitch.y < boundingRect.bottom) {
	  boundingRect.bottom = stitch.y;
	}
	if (stitch.y > boundingRect.top) {
	  boundingRect.top = stitch.y;
	}
  }
  F_POINT const boundingSize = {boundingRect.right - boundingRect.left,
                                boundingRect.top - boundingRect.bottom};
  if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {
	displayText::tabmsg(IDS_PFAF2L, false);
	return false;
  }
  auto const largeFlag =
      (boundingSize.x > SHUPX || boundingSize.y > SHUPY) ||
      (util::closeEnough(IniFile.hoopSizeX, LHUPX) && util::closeEnough(IniFile.hoopSizeY, LHUPY));
  auto const hoopSize = largeFlag ? LARGE_HOOP : SMALL_HOOP;
  PCSHeader.hoopType  = largeFlag ? LARGHUP : SMALHUP;
  auto delta          = F_POINT {};
  if (boundingRect.right > hoopSize.x) {
	delta.x = hoopSize.x - boundingRect.right;
  }
  if (boundingRect.top > hoopSize.y) {
	delta.y = hoopSize.y - boundingRect.top;
  }
  if (boundingRect.left < 0) {
	delta.x = -boundingRect.left;
  }
  if (boundingRect.bottom < 0) {
	delta.y = -boundingRect.bottom;
  }
  if ((delta.x != 0.0F) || (delta.y != 0.0F)) {
	for (auto& offsetStitch : stitches) {
	  offsetStitch.x += delta.x;
	  offsetStitch.y += delta.y;
	}
  }
  return true;
}

auto PCS::isPCS(fs::path const& path) -> bool {
  auto extention = path.extension().wstring();
  std::ranges::transform(extention, extention.begin(), ::towlower);
  return (extention.compare(0, 4, L".pcs") == 0);
}

auto PCS::insPCS(fs::path const& insertedFile, F_RECTANGLE& insertedRectangle) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const fileHandle =
      CreateFile(insertedFile.wstring().c_str(), (GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast, performance-no-int-to-ptr)
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::filnopn(IDS_FNOPN, insertedFile);
	return false;
  }
  auto pcsFileHeader = PCSHEADER {};
  auto bytesRead     = DWORD {};

  if (!wrap::readFile(fileHandle, &pcsFileHeader, sizeof(pcsFileHeader), &bytesRead, L"ReadFile for pcsFileHeader in insPCS")) {
	return false;
  }
  if (pcsFileHeader.leadIn != 0x32 || pcsFileHeader.colorCount != COLORCNT) {
	// ToDo - Add error message
	CloseHandle(fileHandle);
	return false;
  }
  auto fileSize = uintmax_t {};
  thred::getFileSize(insertedFile, fileSize);
  fileSize -= sizeof(pcsFileHeader) + 14;
  auto const pcsStitchCount  = wrap::toSize(fileSize / sizeof(PCS_STITCH));
  auto       pcsStitchBuffer = std::vector<PCS_STITCH> {};
  pcsStitchBuffer.resize(pcsStitchCount);
  if (!wrap::readFile(fileHandle, pcsStitchBuffer.data(), fileSize, &bytesRead, L"ReadFile for pcsStitchBuffer in insPCS")) {
	return false;
  }
  if (bytesRead != fileSize) {
	StateMap->reset(StateFlag::INIT);
	displayText::tabmsg(IDS_SHRTF, false);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
	CloseHandle(fileHandle);
	return false;
  }
  thred::savdo();
  auto insertIndex = StitchBuffer->size();
  StitchBuffer->reserve(StitchBuffer->size() + pcsStitchCount);
  auto newAttribute = 0U;
  for (auto iPCSStitch = 0U; iPCSStitch < pcsStitchCount; ++iPCSStitch) {
	if (pcsStitchBuffer[iPCSStitch].tag == 3) {
	  newAttribute = pcsStitchBuffer[iPCSStitch++].fx | NOTFRM;
	  continue;
	}
	auto xVal = wrap::toFloat(pcsStitchBuffer[iPCSStitch].x) +
	            wrap::toFloat(pcsStitchBuffer[iPCSStitch].fx) / FRACFACT;
	auto yVal = wrap::toFloat(pcsStitchBuffer[iPCSStitch].y) +
	            wrap::toFloat(pcsStitchBuffer[iPCSStitch].fy) / FRACFACT;
	(*StitchBuffer).emplace_back(xVal, yVal, newAttribute);
  }
  auto const newStitchCount = StitchBuffer->size();
  for (; insertIndex < newStitchCount; ++insertIndex) {
	auto const& stitch = StitchBuffer->operator[](insertIndex);
	if (stitch.x < insertedRectangle.left) {
	  insertedRectangle.left = stitch.x;
	}
	if (stitch.x > insertedRectangle.right) {
	  insertedRectangle.right = stitch.x;
	}
	if (stitch.y < insertedRectangle.bottom) {
	  insertedRectangle.bottom = stitch.y;
	}
	if (stitch.y > insertedRectangle.top) {
	  insertedRectangle.top = stitch.y;
	}
  }
  CloseHandle(fileHandle);
  return true;
}
