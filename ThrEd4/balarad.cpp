// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "balarad.h"
#include "DST.h"
#include "globals.h"
#include "point.h"
#include "thred.h"
#include "ThrEdTypes.h"
#include "utf8conv.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/narrow"
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

// Windows Header Files:
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
#include <windef.h> // Win32 header
#include <winnt.h>

// Standard Libraries
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iterator>
#include <vector>

#pragma pack(push, 1)
class BAL_STITCH // balarad stitch
{
  public:
  uint8_t code {};
  uint8_t flag {};
  float   x {};
  float   y {};
};
#pragma pack(pop)

#pragma pack(push, 1)
constexpr auto MAXCOL = 256;  // maximum number of colors in balarad file
constexpr auto BALPAD = 1006; // balarad file header padding

class BAL_HEAD // balarad file header
{
  public:
  std::array<COLORREF, MAXCOL> color {};

  uint32_t signature {};
  uint16_t version {};
  float    hoopSizeX {};
  float    hoopSizeY {};
  COLORREF backgroundColor {};

  std::array<uint8_t, BALPAD> res {};
};
#pragma pack(pop)

namespace bali {
void thr2bal(std::vector<BAL_STITCH>& balaradStitch, uint32_t source, uint8_t code, uint8_t flag);
} // namespace bali

// main variables
namespace {
auto BalaradOffset = F_POINT {};                           // balarad offset
auto BalaradName0  = gsl::narrow_cast<fs::path*>(nullptr); // balarad semaphore file
auto BalaradName1  = gsl::narrow_cast<fs::path*>(nullptr); // balarad data file
auto BalaradName2  = gsl::narrow_cast<fs::path*>(nullptr);
} // namespace

constexpr auto BALJUMP  = uint8_t {0x81U}; // balarad jump stitch
constexpr auto BALNORM  = uint8_t {0x80U}; // normal balarad stitch
constexpr auto BALRATIO = 10.0F / 6.0F;    // Balarad stitch size ration
constexpr auto BALSTOP  = uint8_t {0U};    // balarad stop

auto bal::getBN0() noexcept -> fs::path* {
  return BalaradName0;
}

auto bal::getBN1() noexcept -> fs::path* {
  return BalaradName1;
}

auto bal::getBN2() noexcept -> fs::path* {
  return BalaradName2;
}

void bal::setBN0(fs::path* name) noexcept {
  BalaradName0 = name;
}

void bal::setBN1(fs::path* name) noexcept {
  BalaradName1 = name;
}

void bal::setBN2(fs::path* name) noexcept {
  BalaradName2 = name;
}

void bali::thr2bal(std::vector<BAL_STITCH>& balaradStitch, uint32_t const source, uint8_t const code, uint8_t const flag) {
  balaradStitch.push_back(BAL_STITCH {code,
                                      flag,
                                      (StitchBuffer->operator[](source).x - BalaradOffset.x) * BALRATIO,
                                      (StitchBuffer->operator[](source).y - BalaradOffset.y) * BALRATIO});
}

void bal::redbal() {
  auto fileSize = uintmax_t {};
  if (!thred::getFileSize(*BalaradName2, fileSize)) {
	return;
  }
  auto balaradHeader = BAL_HEAD {};
  StitchBuffer->clear();
  FormList->clear();
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const balaradFile =
      CreateFile(BalaradName2->wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
  if (balaradFile == INVALID_HANDLE_VALUE) {
	return;
  }
  auto bytesRead = DWORD {};
  if (!wrap::readFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesRead, L"ReadFile for balaradHeader in redbal")) {
	return;
  }
  if (bytesRead != sizeof(balaradHeader)) {
	CloseHandle(balaradFile);
	return;
  }
  auto balaradStitch = std::vector<BAL_STITCH> {};

  auto const newSize = (fileSize - sizeof(balaradHeader)) / wrap::sizeofType(balaradStitch);
  balaradStitch.resize(gsl::narrow<size_t>(newSize));
  if (!wrap::readFile(balaradFile, balaradStitch.data(), fileSize - sizeof(balaradHeader), &bytesRead, L"ReadFile for balaradStitch in redbal")) {
	return;
  }
  auto const stitchCount  = bytesRead / wrap::sizeofType(balaradStitch);
  BackgroundColor         = balaradHeader.backgroundColor;
  IniFile.backgroundColor = balaradHeader.backgroundColor;
  thred::initBackPenBrush();
  constexpr auto IBALRAT = 6.0F / 10.0F; // Inverse balarad stitch size ratio
  IniFile.hoopSizeX      = balaradHeader.hoopSizeX * IBALRAT;
  IniFile.hoopSizeY      = balaradHeader.hoopSizeY * IBALRAT;
  UnzoomedRect           = {std::lround(IniFile.hoopSizeX), std::lround(IniFile.hoopSizeY)};
  BalaradOffset          = F_POINT {IniFile.hoopSizeX * HALF, IniFile.hoopSizeY * HALF};
  IniFile.hoopType       = CUSTHUP;
  UserColor.fill(0);
  auto const spBHC = gsl::span {balaradHeader.color};
  auto       iBHC  = spBHC.begin();
  UserColor[0]     = *iBHC;
  auto color       = 0U;
  thred::addColor(0, color);
  for (auto iStitch = 0U; iStitch < stitchCount; ++iStitch) {
	switch (balaradStitch[iStitch].code) {
	  case BALNORM: {
		StitchBuffer->emplace_back(balaradStitch[iStitch].x * IBALRAT + BalaradOffset.x,
		                           balaradStitch[iStitch].y * IBALRAT + BalaradOffset.y,
		                           color);
		break;
	  }
	  case BALSTOP: {
		color = DST::colmatch(*iBHC++);

		auto const currentStitch = wrap::toUnsigned(StitchBuffer->size() - 1U);
		thred::addColor(currentStitch, color);
		break;
	  }
	  default: {
		outDebugString(L"default hit in redbal: code [{}]\n", balaradStitch[iStitch].code);
		break;
	  }
	}
  }
  thred::initPenBrush();
  thred::coltab();
  thred::redrawColorBar();
  StateMap->set(StateFlag::INIT);
  StateMap->set(StateFlag::RESTCH);
  CloseHandle(balaradFile);
}

void bal::ritbal() {
  auto balaradHeader = BAL_HEAD {};
  if (!BalaradName0->empty() && !BalaradName1->empty() && !StitchBuffer->empty()) {
	auto outputName = thred::setFileName();
	outputName.replace_extension(L".thv");
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto balaradFile =
	    CreateFile(outputName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (balaradFile == INVALID_HANDLE_VALUE) {
	  return;
	}
	auto       color  = gsl::narrow_cast<uint8_t>(StitchBuffer->front().attribute & COLMSK);
	auto const spBHC  = gsl::span {balaradHeader.color};
	auto       iBHC   = spBHC.begin();
	auto const bhcEnd = std::next(spBHC.begin(), UserColor.size());
	*iBHC             = UserColor.at(color);
	for (auto const& stitch : *StitchBuffer) {
	  if (auto const stitchColor = gsl::narrow_cast<uint8_t>(stitch.attribute & COLMSK); color != stitchColor) {
		color     = stitchColor;
		*iBHC++ = UserColor.at(color);
		if (iBHC == bhcEnd) {
		  break;
		}
	  }
	}
	constexpr auto DRBM           = 0x6472626d; // integer representation of 'drbm'
	balaradHeader.signature       = DRBM;
	balaradHeader.backgroundColor = BackgroundColor;
	balaradHeader.hoopSizeX       = IniFile.hoopSizeX * BALRATIO;
	balaradHeader.hoopSizeY       = IniFile.hoopSizeY * BALRATIO;
	auto bytesWritten             = DWORD {};
	WriteFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesWritten, nullptr);
	BalaradOffset      = F_POINT {IniFile.hoopSizeX * HALF, IniFile.hoopSizeY * HALF};
	auto balaradStitch = std::vector<BAL_STITCH> {};
	balaradStitch.reserve(StitchBuffer->size() + 2U);
	color = StitchBuffer->front().attribute & COLMSK;
	bali::thr2bal(balaradStitch, 0, BALJUMP, 0);
	for (auto iStitch = 0U; const auto& stitch : *StitchBuffer) {
	  bali::thr2bal(balaradStitch, iStitch, BALNORM, 0);
	  if ((stitch.attribute & COLMSK) != color) {
		color = stitch.attribute & COLMSK;
		bali::thr2bal(balaradStitch, iStitch, BALSTOP, color);
	  }
	  ++iStitch;
	}
	WriteFile(balaradFile, balaradStitch.data(), wrap::sizeofVector(balaradStitch), &bytesWritten, nullptr);
	CloseHandle(balaradFile);
	balaradFile =
	    CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	auto const outString = utf::utf16ToUtf8(outputName.wstring());
	wrap::writeFile(balaradFile, outString.c_str(), wrap::toUnsigned(outputName.wstring().size()) + 1U, &bytesWritten, nullptr);
	CloseHandle(balaradFile);
  }
  else {
	if (!BalaradName1->empty()) {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const balaradFile =
	      CreateFile(BalaradName1->wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	  CloseHandle(balaradFile);
	}
  }
  if (!BalaradName0->empty()) {
	fs::remove(*BalaradName0);
  }
}
