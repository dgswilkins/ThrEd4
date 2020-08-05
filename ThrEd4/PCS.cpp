#include "stdafx.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
//#include <Windows.h> // Win32 Platform SDK main header

#ifdef ALLOCFAILURE
//#include <new.h>
#endif

// Standard Libraries
#include <cwctype>

// Open Source headers

// Local Headers
#include "switches.h"

#include "Resources/resource.h"
#include "globals.h"
#include "displayText.h"
#include "form.h"
#include "thred.h"
//#include "PES.h"
#include "PCS.h"

#pragma pack(push, 1)

class PCSHEADER // pcs file header structure
{
  public:
  int8_t   leadIn {0};
  int8_t   hoopType {0};
  uint16_t colorCount {0};
  COLORREF colors[COLOR_COUNT] {0}; // NOLINT(modernize-avoid-c-arrays)
  uint16_t stitchCount {0};

  constexpr PCSHEADER() noexcept = default;
  // PCSHEADER(PCSHEADER&&) = default;
  // PCSHEADER& operator=(PCSHEADER const& rhs) = default;
  // PCSHEADER& operator=(PCSHEADER&&) = default;
  //~PCSHEADER() = default;
};

#pragma pack(pop)

namespace pci = PCS::internal;

auto PCSHeader = PCSHEADER {};                         // pcs file header

auto PCS::savePCS(fs::path const* auxName, std::vector<fPOINTATTR>& saveStitches) -> bool {
  auto flag = true;
  if (nullptr != auxName) {
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto fileHandle = CreateFile(
	    auxName->wstring().c_str(), (GENERIC_WRITE | GENERIC_READ), 0, nullptr, CREATE_ALWAYS, 0, nullptr); // NOLINT(hicpp-signed-bitwise)
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
	if (fileHandle == INVALID_HANDLE_VALUE) {
	  displayText::crmsg(*auxName);
	  flag = false;
	}
	else {
	  PCSHeader.leadIn     = 0x32;
	  PCSHeader.colorCount = COLOR_COUNT;
	  auto PCSStitchBuffer = std::vector<PCSTCH> {};
	  wrap::narrow(PCSHeader.stitchCount, StitchBuffer->size());
	  for (auto iColor = 0U; iColor < COLOR_COUNT; ++iColor) {
		PCSHeader.colors[iColor] = UserColor[iColor];
	  }
	  do {
		if (pci::pcshup(saveStitches)) {
		  flag = false;
		  break;
		}
		auto bytesWritten = DWORD {0};
		if (FALSE == WriteFile(fileHandle, &PCSHeader, sizeof(PCSHeader), &bytesWritten, nullptr)) {
		  displayText::riter();
		  flag = false;
		  break;
		}
		auto iPCSstitch = 0U;
		auto savcol     = COLMSK;
		PCSStitchBuffer.resize(StitchBuffer->size() + thred::maxColor() + 2U);
		for (auto& stitch : saveStitches) {
		  if ((stitch.attribute & COLMSK) != savcol) {
			savcol                          = stitch.attribute & COLMSK;
			PCSStitchBuffer[iPCSstitch].tag = 3;
			wrap::narrow(PCSStitchBuffer[iPCSstitch++].fx, savcol);
		  }
		  auto integerPart    = 0.0F;
		  auto fractionalPart = std::modf(stitch.x, &integerPart);
		  PCSStitchBuffer[iPCSstitch].fx =
		      wrap::floor<decltype(PCSStitchBuffer.back().fx)>(fractionalPart * fractionalFactor);
		  wrap::narrow(PCSStitchBuffer[iPCSstitch].x, integerPart);
		  fractionalPart = std::modf(stitch.y, &integerPart);
		  PCSStitchBuffer[iPCSstitch].fy =
		      wrap::floor<decltype(PCSStitchBuffer.back().fy)>(fractionalPart * fractionalFactor);
		  wrap::narrow(PCSStitchBuffer[iPCSstitch++].y, integerPart);
		}
		if (FALSE == WriteFile(fileHandle,
		                       PCSStitchBuffer.data(),
		                       iPCSstitch * sizeof(decltype(PCSStitchBuffer.back())),
		                       &bytesWritten,
		                       nullptr)) {
		  displayText::riter();
		  flag = false;
		  break;
		}
		if (UserFlagMap->test(UserFlag::BSAVOF)) {
		  auto const blank = std::array<char, 14>{};
		  if (FALSE == WriteFile(fileHandle, blank.data(), blank.size(), &bytesWritten, nullptr)) {
			displayText::riter();
			flag = false;
			break;
		  }
		}
		else {
		  if (FALSE ==
		      WriteFile(fileHandle, static_cast<LPCVOID>(PCSBMPFileName), 14, &bytesWritten, nullptr)) {
			displayText::riter();
			flag = false;
			break;
		  }
		}
	  } while (false);
	}
	CloseHandle(fileHandle);
  }
  else {
	flag = false;
  }
  return flag;
}

auto PCS::readPCSFile(fs::path const& newFileName) -> bool {
  auto fileSize = uintmax_t {0};
  if (thred::getFileSize(newFileName, fileSize)) {
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto fileHandle = HANDLE {nullptr};
	if (thred::getFileHandle(newFileName, fileHandle)) {
	  auto bytesRead = DWORD {0};
	  ReadFile(fileHandle, &PCSHeader, sizeof(PCSHeader), &bytesRead, nullptr);
	  if (bytesRead == sizeof(PCSHeader)) {
		if (PCSHeader.leadIn == '2' && PCSHeader.colorCount == COLOR_COUNT) {
		  for (auto iColor = 0U; iColor < COLOR_COUNT; ++iColor) {
			UserColor[iColor] = PCSHeader.colors[iColor];
		  }
		  fileSize -= sizeof(PCSHeader) + 14;
		  auto const pcsStitchCount = fileSize / sizeof(PCSTCH);
		  auto       PCSDataBuffer  = std::vector<PCSTCH> {};
		  PCSDataBuffer.resize(pcsStitchCount);
		  ReadFile(fileHandle, PCSDataBuffer.data(), fileSize, &bytesRead, nullptr);
		  if (bytesRead == fileSize) {
			auto iStitch      = uint16_t {0U};
			auto iColorChange = 0U;
			auto color        = 0U;
			auto iPCSstitch   = 0U;
			StitchBuffer->clear();
			StitchBuffer->reserve(PCSHeader.stitchCount);
			while (iStitch < PCSHeader.stitchCount && iPCSstitch < pcsStitchCount) {
			  auto& stitch = PCSDataBuffer[iPCSstitch];
			  if (stitch.tag == 3) {
				thred::addColor(iStitch, stitch.fx);
				++iColorChange;
				color = NOTFRM | stitch.fx;
			  }
			  else {
				StitchBuffer->push_back(
				    fPOINTATTR {wrap::toFloat(stitch.x) + wrap::toFloat(stitch.fx) / fractionalFactor,
				                wrap::toFloat(stitch.y) + wrap::toFloat(stitch.fy) / fractionalFactor,
				                color});
				++iStitch;
			  }
			  ++iPCSstitch;
			}
			// Grab the bitmap filename
			ReadFile(fileHandle, &PCSBMPFileName, 14, &bytesRead, nullptr);
			if (bytesRead != 14) {
				outDebugString(L"readPCSFile: description bytesRead {}\n", bytesRead);
			}
			IniFile.auxFileType = AUXPCS;
			if (PCSHeader.hoopType != LARGHUP && PCSHeader.hoopType != SMALHUP) {
			  PCSHeader.hoopType = LARGHUP;
			}
			auto stitchRect = fRECTANGLE {};
			thred::sizstch(stitchRect, *StitchBuffer);
			if (stitchRect.left < 0 || stitchRect.right > LHUPY || stitchRect.bottom < 0 ||
			    stitchRect.top > LHUPY) {
			  IniFile.hoopSizeX = LHUPX;
			  IniFile.hoopSizeY = LHUPY;
			  thred::chkhup();
			}
			else {
			  if (PCSHeader.hoopType == LARGHUP) {
				IniFile.hoopType  = LARGHUP;
				IniFile.hoopSizeX = LHUPX;
				IniFile.hoopSizeY = LHUPY;
			  }
			  else {
				if (stitchRect.right > SHUPX || stitchRect.top > SHUPY) {
				  IniFile.hoopType  = LARGHUP;
				  IniFile.hoopSizeX = SHUPX;
				  IniFile.hoopSizeY = SHUPY;
				}
				else {
				  IniFile.hoopType  = SMALHUP;
				  IniFile.hoopSizeX = SHUPX;
				  IniFile.hoopSizeY = SHUPY;
				}
			  }
			}
			CloseHandle(fileHandle);
			return true;
		  }
		}
	  }
	  CloseHandle(fileHandle);
	}
  }
  return false;
}

auto PCS::internal::pcshup(std::vector<fPOINTATTR>& stitches) -> bool {
  auto boundingRect = fRECTANGLE {stitches[0].y, stitches[0].x, stitches[0].x, stitches[0].y};
  for (auto stitch : stitches) {
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
  fPOINT const boundingSize = {boundingRect.right - boundingRect.left,
                               boundingRect.top - boundingRect.bottom};
  if (boundingSize.x > LHUPX || boundingSize.y > LHUPY) {
	displayText::tabmsg(IDS_PFAF2L);
	return true;
  }
  auto hoopSize = fPOINT {};
  if (boundingSize.x > SHUPX || boundingSize.y > SHUPY) {
	PCSHeader.hoopType = LARGHUP;
	hoopSize.x         = LHUPX;
	hoopSize.y         = LHUPY;
  }
  else {
	if (IniFile.hoopSizeX == LHUPX && IniFile.hoopSizeY == LHUPY) {
	  PCSHeader.hoopType = LARGHUP;
	  hoopSize.x         = LHUPX;
	  hoopSize.y         = LHUPY;
	}
	else {
	  PCSHeader.hoopType = SMALHUP;
	  hoopSize.x         = SHUPX;
	  hoopSize.y         = SHUPY;
	}
  }
  auto delta = fPOINT {};
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
  return false;
}

auto PCS::isPCS(fs::path const& path) -> bool {
  auto extention = path.extension().wstring();
  std::transform(extention.begin(), extention.end(), extention.begin(), ::towlower);
  return (extention.compare(0, 4, L".pcs") == 0);
}

auto PCS::insPCS(fs::path const& insertedFile, fRECTANGLE& insertedRectangle) -> bool {
  auto retflag = true;
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle =
      CreateFile(insertedFile.wstring().c_str(), (GENERIC_READ), 0, nullptr, OPEN_EXISTING, 0, nullptr);
#pragma warning(suppress : 26493) // type.4 Don't use C-style casts NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
  if (fileHandle == INVALID_HANDLE_VALUE) {
	displayText::filnopn(IDS_FNOPN, insertedFile);
	retflag = false;
  }
  else {
	auto pcsFileHeader = PCSHEADER {};
	auto bytesRead     = DWORD {0};

	ReadFile(fileHandle, &pcsFileHeader, sizeof(pcsFileHeader), &bytesRead, nullptr);
	if (pcsFileHeader.leadIn == 0x32 && pcsFileHeader.colorCount == COLOR_COUNT) {
	  auto fileSize = uintmax_t {0};
	  thred::getFileSize(insertedFile, fileSize);
	  fileSize -= sizeof(pcsFileHeader) + 14;
	  auto const pcsStitchCount  = fileSize / sizeof(PCSTCH);
	  auto       pcsStitchBuffer = std::vector<PCSTCH> {};
	  pcsStitchBuffer.resize(pcsStitchCount);
	  ReadFile(fileHandle, pcsStitchBuffer.data(), fileSize, &bytesRead, nullptr);
	  if (bytesRead == fileSize) {
		thred::savdo();
		auto insertIndex = StitchBuffer->size();
		StitchBuffer->reserve(StitchBuffer->size() + pcsStitchCount);
		auto newAttribute = 0U;
		for (auto iPCSStitch = 0U; iPCSStitch < pcsStitchCount; ++iPCSStitch) {
		  if (pcsStitchBuffer[iPCSStitch].tag == 3) {
			newAttribute = pcsStitchBuffer[iPCSStitch++].fx | NOTFRM;
		  }
		  else {
			(*StitchBuffer)
			    .emplace_back(fPOINTATTR {gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].x) +
			                                  gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].fx) / fractionalFactor,
			                              gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].y) +
			                                  gsl::narrow_cast<float>(pcsStitchBuffer[iPCSStitch].fy) / fractionalFactor,
			                              newAttribute});
		  }
		}
		auto const newStitchCount = StitchBuffer->size();
		auto const startStitch    = StitchBuffer->operator[](insertIndex);
		for (; insertIndex < newStitchCount; ++insertIndex) {
		  auto const stitch = StitchBuffer->operator[](insertIndex);
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
		retflag             = true;
	  }
	  else {
		StateMap->reset(StateFlag::INIT);
		displayText::tabmsg(IDS_SHRTF);
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
		retflag = false;
	  }
	}
	else {
	  retflag = false;
	}
  }
  CloseHandle(fileHandle);
  return retflag;
}
