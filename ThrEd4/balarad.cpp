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

class BAL_SINGLE
{
  public:
  static auto getInstance() noexcept -> BAL_SINGLE* {
	// NOLINTNEXTLINE(clang-diagnostic-exit-time-destructors)
	static BAL_SINGLE instance;
	return &instance;
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  fs::path BalaradName0; // balarad semaphore file
  fs::path BalaradName1; // balarad data file
  fs::path BalaradName2;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  BAL_SINGLE(const BAL_SINGLE&)                    = delete;
  auto operator=(const BAL_SINGLE&) -> BAL_SINGLE& = delete;
  BAL_SINGLE(BAL_SINGLE&&)                         = delete;
  auto operator=(BAL_SINGLE&&) -> BAL_SINGLE&      = delete;

  private:
  BAL_SINGLE() noexcept = default;
  ~BAL_SINGLE()         = default;
};

// balarad namespace
namespace {
constexpr auto BALJUMP  = uint8_t {0x81U}; // balarad jump stitch
constexpr auto BALNORM  = uint8_t {0x80U}; // normal balarad stitch
constexpr auto BALRATIO = 10.0F / 6.0F;    // Balarad stitch size ration
constexpr auto BALSTOP  = uint8_t {0U};    // balarad stop

auto BalaradOffset = F_POINT {}; // balarad offset

BAL_SINGLE* BalInstance;

// Functions
void thr2bal(std::vector<BAL_STITCH>& balaradStitch, uint32_t const source, uint8_t const code, uint8_t const flag) {
  balaradStitch.push_back(
      BAL_STITCH {.code = code,
                  .flag = flag,
                  .x = (Instance->stitchBuffer.operator[](source).x - BalaradOffset.x) * BALRATIO,
                  .y = (Instance->stitchBuffer.operator[](source).y - BalaradOffset.y) * BALRATIO});
}
} // namespace

auto bal::getBN0() noexcept -> fs::path& {
  return BalInstance->BalaradName0;
}

auto bal::getBN1() noexcept -> fs::path& {
  return BalInstance->BalaradName1;
}

auto bal::getBN2() noexcept -> fs::path& {
  return BalInstance->BalaradName2;
}

void bal::redbal() {
  auto fileSize = uintmax_t {};
  if (!thred::getFileSize(BalInstance->BalaradName2, fileSize)) {
	return;
  }
  auto balaradHeader = BAL_HEAD {};
  Instance->stitchBuffer.clear();
  Instance->formList.clear();
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const balaradFile = CreateFile(
      BalInstance->BalaradName2.wstring().c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
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
  auto const stitchCount = bytesRead / wrap::sizeofType(balaradStitch);
  thred::setBackgroundColor(balaradHeader.backgroundColor);
  IniFile.backgroundColor = balaradHeader.backgroundColor;
  thred::initBackPenBrush();
  constexpr auto IBALRAT = 6.0F / 10.0F; // Inverse balarad stitch size ratio
  IniFile.hoopSizeX      = balaradHeader.hoopSizeX * IBALRAT;
  IniFile.hoopSizeY      = balaradHeader.hoopSizeY * IBALRAT;
  UnzoomedRect     = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
  BalaradOffset    = F_POINT {IniFile.hoopSizeX * HALF, IniFile.hoopSizeY * HALF};
  IniFile.hoopType = CUSTHUP;
  UserColor.fill(0);
  auto const spBHC = gsl::span {balaradHeader.color};
  auto       iBHC  = spBHC.begin();
  UserColor[0]     = *iBHC;
  auto color       = 0U;
  thred::addColor(0, color);
  for (auto iStitch = 0U; iStitch < stitchCount; ++iStitch) {
	switch (balaradStitch[iStitch].code) {
	  case BALNORM: {
		Instance->stitchBuffer.emplace_back((balaradStitch[iStitch].x * IBALRAT) + BalaradOffset.x,
		                                    (balaradStitch[iStitch].y * IBALRAT) + BalaradOffset.y,
		                                    color);
		break;
	  }
	  case BALSTOP: {
		color = DST::colmatch(*iBHC++);

		auto const currentStitch = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
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
  Instance->stateMap.set(StateFlag::INIT);
  Instance->stateMap.set(StateFlag::RESTCH);
  CloseHandle(balaradFile);
}

void bal::ritbal() {
  auto balaradHeader = BAL_HEAD {};
  if (!BalInstance->BalaradName0.empty() && !BalInstance->BalaradName1.empty() &&
      !Instance->stitchBuffer.empty()) {
	auto outputName = thred::setFileName();
	outputName.replace_extension(L".thv");
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto balaradFile =
	    CreateFile(outputName.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	if (balaradFile == INVALID_HANDLE_VALUE) {
	  return;
	}
	auto       color = gsl::narrow_cast<uint8_t>(Instance->stitchBuffer.front().attribute & COLMSK);
	auto const spBHC = gsl::span {balaradHeader.color};
	auto       iBHC  = spBHC.begin();
	auto const bhcEnd = std::next(spBHC.begin(), UserColor.size());
	*iBHC             = UserColor.at(color);
	for (auto const& stitch : Instance->stitchBuffer) {
	  if (auto const stitchColor = gsl::narrow_cast<uint8_t>(stitch.attribute & COLMSK); color != stitchColor) {
		color   = stitchColor;
		*iBHC++ = UserColor.at(color);
		if (iBHC == bhcEnd) {
		  break;
		}
	  }
	}
	constexpr auto DRBM           = 0x6472626d; // integer representation of 'drbm'
	balaradHeader.signature       = DRBM;
	balaradHeader.backgroundColor = thred::getBackgroundColor();
	balaradHeader.hoopSizeX       = IniFile.hoopSizeX * BALRATIO;
	balaradHeader.hoopSizeY       = IniFile.hoopSizeY * BALRATIO;
	auto bytesWritten             = DWORD {};
	WriteFile(balaradFile, &balaradHeader, sizeof(balaradHeader), &bytesWritten, nullptr);
	BalaradOffset      = F_POINT {IniFile.hoopSizeX * HALF, IniFile.hoopSizeY * HALF};
	auto balaradStitch = std::vector<BAL_STITCH> {};
	balaradStitch.reserve(Instance->stitchBuffer.size() + 2U);
	color = Instance->stitchBuffer.front().attribute & COLMSK;
	thr2bal(balaradStitch, 0, BALJUMP, 0);
	for (auto iStitch = 0U; const auto& stitch : Instance->stitchBuffer) {
	  thr2bal(balaradStitch, iStitch, BALNORM, 0);
	  if ((stitch.attribute & COLMSK) != color) {
		color = stitch.attribute & COLMSK;
		thr2bal(balaradStitch, iStitch, BALSTOP, color);
	  }
	  ++iStitch;
	}
	WriteFile(balaradFile, balaradStitch.data(), wrap::sizeofVector(balaradStitch), &bytesWritten, nullptr);
	CloseHandle(balaradFile);
	balaradFile = CreateFile(
	    BalInstance->BalaradName1.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	auto const outString = utf::utf16ToUtf8(outputName.wstring());
	wrap::writeFile(balaradFile, outString.c_str(), wrap::toUnsigned(outputName.wstring().size()) + 1U, &bytesWritten, nullptr);
	CloseHandle(balaradFile);
  }
  else {
	if (!BalInstance->BalaradName1.empty()) {
	  // NOLINTNEXTLINE(readability-qualified-auto)
	  auto const balaradFile = CreateFile(
	      BalInstance->BalaradName1.wstring().c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
	  CloseHandle(balaradFile);
	}
  }
  if (!BalInstance->BalaradName0.empty()) {
	fs::remove(BalInstance->BalaradName0);
  }
}

void bal::balInit() noexcept {
  BalInstance = BAL_SINGLE::getInstance();
}
