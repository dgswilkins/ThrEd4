// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "bitmap.h"
#include "displayText.h"
#include "formHeader.h"
#include "globals.h"
#include "iniFile.h"
#include "Resources/resource.h"
#include "satCon.h"
#include "thred.h"
#include "thredFile.h"
#include "ThrEdTypes.h"
#include "utf8conv.h"
//  ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
//  ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "gsl/span"
#include "gsl/util"
#pragma warning(pop)

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
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
#include <cstring>
// ReSharper restore CppUnusedIncludeDirective
#include <filesystem>
#include <vector>

auto thredFile::readTHRFile(std::filesystem::path const&    newFileName,
                            THR_HEAD_EX&                    extendedHeader,
                            COLORREF&                       backgroundColor,
                            std::array<COLORREF, COLORCNT>& customThreadColor,
                            std::array<wchar_t, COLORCNT>&  threadSizeArray) -> bool {
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto fileHandle = HANDLE {nullptr};
  if (!thred::getFileHandle(newFileName, fileHandle)) {
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto bytesRead   = DWORD {};
  auto thredHeader = THR_HEAD {};
  if (!wrap::readFile(fileHandle, &thredHeader, sizeof(thredHeader), &bytesRead, L"ReadFile for ThrEd Header in readTHRFile")) { // check is we even have a file
	return false;
  }
  if (!thredHeader.isValid()) { // does the file have the correct signature
	thred::prtred(fileHandle, IDS_NOTHR);
	return false;
  }
  if (bytesRead != sizeof(thredHeader)) { // is there enough data in the file to read the header
	thred::prtred(fileHandle, IDS_SHRTF);
	return false;
  }
  auto const version = thredHeader.getVersion();
  thred::setDesignerName(IniFile.designerName);
  switch (version) { // handle the different versions of the file format
	case 0: {
	  if (thredHeader.hoopType == wrap::toIntegralType(HoopSize::kSmall)) {
		IniFile.hoopSizeX = SHUPX;
		IniFile.hoopSizeY = SHUPY;
		UnzoomedRect = SIZE {gsl::narrow_cast<int32_t>(SHUPX), gsl::narrow_cast<int32_t>(SHUPY)};
	  }
	  else {
		IniFile.hoopSizeX = LHUPX;
		IniFile.hoopSizeY = LHUPY;
		UnzoomedRect = SIZE {gsl::narrow_cast<int32_t>(LHUPX), gsl::narrow_cast<int32_t>(LHUPY)};
	  }
	  thred::ritfnam(thred::getDesignerName());
	  auto const spModifierName = gsl::span {extendedHeader.modifierName};
	  auto const spIDN          = gsl::span {IniFile.designerName};
	  // NOLINTNEXTLINE(clang-diagnostic-unsafe-buffer-usage-in-libc-call)
	  std::copy(spIDN.begin(),
	            wrap::next(spIDN.begin(), std::strlen(spIDN.data()) + 1U),
	            spModifierName.begin());
	  break;
	}
	case 1:
	case 2: {
	  if (!wrap::readFile(fileHandle, &extendedHeader, sizeof(extendedHeader), &bytesRead, L"ReadFile for extendedHeader in readTHRFile")) {
		return false;
	  }
	  if (bytesRead != sizeof(extendedHeader)) { // is there enough data in the file to read the extended header
		thred::prtred(fileHandle, IDS_SHRTF);
		return false;
	  }
	  IniFile.hoopSizeX = extendedHeader.hoopSizeX;
	  IniFile.hoopSizeY = extendedHeader.hoopSizeY;

	  UnzoomedRect = {.cx = std::lround(extendedHeader.hoopSizeX), .cy = std::lround(extendedHeader.hoopSizeY)};
	  thred::redfnam(thred::getDesignerName());
	  break;
	}
	default: {
	  thred::prtred(fileHandle, IDS_NOTVER);
	  return false;
	}
  }
  ZoomRect     = F_RECTANGLE {0.0F, IniFile.hoopSizeY, IniFile.hoopSizeX, 0.0F};
  UnzoomedRect = {.cx = std::lround(IniFile.hoopSizeX), .cy = std::lround(IniFile.hoopSizeY)};
  Instance->stitchBuffer.resize(thredHeader.stitchCount);
  if (thredHeader.stitchCount != 0U) { // read the stitch buffer if there are stitches
	auto const bytesToRead = thredHeader.stitchCount * wrap::sizeofType(Instance->stitchBuffer);
	if (!wrap::readFile(fileHandle, Instance->stitchBuffer.data(), bytesToRead, &bytesRead, L"ReadFile for stitchBuffer in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  thred::prtred(fileHandle, IDS_PRT);
	  return false;
	}
  }
  Instance->stitchBuffer.shrink_to_fit();
  auto bytesToRead = bitmap::getBmpNameLength();
  if (!wrap::readFile(fileHandle, bitmap::getBmpNameData(), bytesToRead, &bytesRead, L"ReadFile for getBmpNameData in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the bitmap name, reset the bitmap and return
	bitmap::resetBmpFile(true);
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  bytesToRead = sizeof(backgroundColor);
  if (!wrap::readFile(fileHandle, &backgroundColor, bytesToRead, &bytesRead, L"ReadFile for backgroundColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the background color, default it and return
	backgroundColor = IniFile.backgroundColor;
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  thred::initBackgroundBrush(backgroundColor);
  bytesToRead = sizeof(UserColor);
  if (!wrap::readFile(fileHandle, UserColor.data(), bytesToRead, &bytesRead, L"ReadFile for UserColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the user colors, default them and return
	UserColor = thred::getDefaultThreadColors();
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  bytesToRead = sizeof(customThreadColor);
  if (!wrap::readFile(fileHandle, customThreadColor.data(), bytesToRead, &bytesRead, L"ReadFile for customThreadColor in readTHRFile")) {
	return false;
  }
  if (bytesRead != bytesToRead) { // if we can't read the custom colors, default them and return
	customThreadColor = thred::getDefaultThreadColors();
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto msgBuffer = std::array<char, COLORCNT> {}; // buffer for thread size string
  if (!wrap::readFile(fileHandle, msgBuffer.data(), sizeof(msgBuffer), &bytesRead, L"ReadFile for msgBuffer in readTHRFile")) {
	return false;
  }
  if (bytesRead != COLORCNT) { // if we can't read the thread size buffer, bail
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  auto const threadSizebuf  = std::string(msgBuffer.data(), msgBuffer.size());
  auto       threadSizeBufW = utf::utf8ToUtf16(threadSizebuf);
  std::ranges::generate(threadSizeArray, [tsBuffer = threadSizeBufW.begin()]() mutable noexcept -> wchar_t {
	return *tsBuffer++;
  });
  if (thredHeader.formCount == 0) {
	CloseHandle(fileHandle);
	return true;
  }
  Instance->stateMap.reset(StateFlag::BADFIL);
  auto& formList = Instance->formList;

  if (version < 2) {
	auto formListOriginal = std::vector<FRM_HEAD_O> {};
	formListOriginal.resize(thredHeader.formCount);
	bytesToRead = thredHeader.formCount * wrap::sizeofType(formListOriginal);
	if (!wrap::readFile(fileHandle, formListOriginal.data(), bytesToRead, &bytesRead, L"ReadFile for formListOriginal in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) { // if the header and available data don't match, resize the form list with the data we do have but set the bad file flag
	  wrap::narrow(thredHeader.formCount, bytesRead / wrap::sizeofType(formListOriginal));
	  formListOriginal.resize(thredHeader.formCount);
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	formList.reserve(formListOriginal.size());
	formList.insert(formList.end(), formListOriginal.begin(), formListOriginal.end());
  }
  else {
	auto inFormList = std::vector<FRM_HEAD_OUT> {};
	inFormList.resize(thredHeader.formCount);
	bytesToRead = thredHeader.formCount * wrap::sizeofType(inFormList);
	if (!wrap::readFile(fileHandle, inFormList.data(), bytesToRead, &bytesRead, L"ReadFile for inFormList in readTHRFile")) { // read the form list
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  wrap::narrow(thredHeader.formCount, bytesRead / wrap::sizeofType(inFormList));
	  inFormList.resize(thredHeader.formCount);
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	formList.reserve(inFormList.size());
	formList.insert(formList.end(), inFormList.begin(), inFormList.end());
  }
  formList.shrink_to_fit();
  if (thredHeader.vertexCount != 0U) { // read the form vertices
	Instance->formVertices.resize(thredHeader.vertexCount);
	bytesToRead = thredHeader.vertexCount * wrap::sizeofType(Instance->formVertices);
	if (!wrap::readFile(fileHandle, Instance->formVertices.data(), bytesToRead, &bytesRead, L"ReadFile for formVertices in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  Instance->formVertices.resize(bytesRead / wrap::sizeofType(Instance->formVertices));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
  }
  else {
	// We have forms but no vertices - blow up the read
	thred::prtred(fileHandle, IDS_PRT);
	return false;
  }
  Instance->formVertices.shrink_to_fit();
  if (thredHeader.dlineCount != 0U) { // read the satin guide list
	auto inGuideList = std::vector<SAT_CON_OUT>(thredHeader.dlineCount);
	bytesToRead      = thredHeader.dlineCount * wrap::sizeofType(inGuideList);
	if (!wrap::readFile(fileHandle, inGuideList.data(), bytesToRead, &bytesRead, L"ReadFile for inGuideList in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  inGuideList.resize(bytesRead / wrap::sizeofType(inGuideList));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
	Instance->satinGuides.reserve(inGuideList.size());
	Instance->satinGuides.insert(Instance->satinGuides.end(), inGuideList.begin(), inGuideList.end());
  }
  Instance->satinGuides.shrink_to_fit();
  if (thredHeader.clipDataCount != 0U) { // read the clip points
	Instance->clipPoints.resize(thredHeader.clipDataCount);
	bytesToRead = thredHeader.clipDataCount * wrap::sizeofType(Instance->clipPoints);
	if (!wrap::readFile(fileHandle, Instance->clipPoints.data(), bytesToRead, &bytesRead, L"ReadFile for clipPoints in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  Instance->clipPoints.resize(bytesRead / wrap::sizeofType(Instance->clipPoints));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
  }
  Instance->clipPoints.shrink_to_fit();
  if (extendedHeader.texturePointCount != 0U) { // read the texture points
	Instance->texturePointsBuffer.resize(extendedHeader.texturePointCount);
	bytesToRead = extendedHeader.texturePointCount * wrap::sizeofType(Instance->texturePointsBuffer);
	if (!wrap::readFile(fileHandle, Instance->texturePointsBuffer.data(), bytesToRead, &bytesRead, L"ReadFile for texturePointsBuffer in readTHRFile")) {
	  return false;
	}
	if (bytesRead != bytesToRead) {
	  Instance->texturePointsBuffer.resize(bytesRead / wrap::sizeofType(Instance->texturePointsBuffer));
	  Instance->stateMap.set(StateFlag::BADFIL);
	}
  }
  else {
	Instance->texturePointsBuffer.clear();
  }
  Instance->texturePointsBuffer.shrink_to_fit();
  if (Instance->stateMap.testAndReset(StateFlag::BADFIL)) {
	displayText::bfilmsg();
  }
  // now re-create all the pointers/indexes in the form data
  auto clipOffset   = 0U;
  auto vertexOffset = 0U;
  auto guideOffset  = 0U;
  for (auto& iForm : formList) {
	iForm.vertexIndex = vertexOffset;
	vertexOffset += iForm.vertexCount;
	if (iForm.type == FormStyles::kSatin && iForm.satinGuideCount != 0U) {
	  iForm.satinGuideIndex = guideOffset;
	  guideOffset += iForm.satinGuideCount;
	}
	// ToDo - do we still need to do this in v3? (we can store the offset safely in v3
	// where we could not store the pointer in v2)
	if (iForm.isClip()) {
	  iForm.clipIndex = clipOffset;
	  clipOffset += iForm.clipCount;
	}
	if (iForm.isEdgeClipX()) {
	  iForm.borderClipData = clipOffset;
	  clipOffset += iForm.clipEntries;
	}
  }
  xt::setfchk();
  CloseHandle(fileHandle);
  return true;
}
