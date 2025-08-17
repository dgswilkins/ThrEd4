// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "backup.h"
#include "formForms.h"
#include "formHeader.h"
#include "globals.h"
#include "iniFile.h"
#include "menu.h"
#include "point.h"
#include "satCon.h"
#include "textureHeader.h"
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
#include <windef.h> // Win32 header

// Standard Libraries
// ReSharper disable CppUnusedIncludeDirective
#include <algorithm>
#include <cstddef>
// ReSharper restore CppUnusedIncludeDirective
#include <cstdint>
#include <iterator>

class BACK_HEAD // Backup header
{
  public:
  uint32_t      formCount {};
  FRM_HEAD*     forms {};
  uint32_t      stitchCount {};
  F_POINT_ATTR* stitches {};
  uint32_t      vertexCount {};
  F_POINT*      vertices {};
  uint32_t      guideCount {};
  SAT_CON*      guide {};
  uint32_t      clipPointCount {};
  F_POINT*      clipPoints {};
  COLORREF*     colors {};
  TX_PNT*       texturePoints {};
  uint32_t      texturePointCount {};
  SIZE          zoomRect {};
};

namespace {
// Main Variables
auto UndoBufferWriteIndex = uint32_t {}; // undo storage pointer
auto UndoBufferReadIndex  = uint32_t {}; // undo retrieval pointers
// Definitions
void redbak();

// Functions
void redbak() {
  auto bufferElement = Instance->undoBuffer.at(UndoBufferWriteIndex);
  if (bufferElement.empty()) {
	return;
  }
  auto const* undoData = convertFromPtr<BACK_HEAD*>(bufferElement.data());
  Instance->stitchBuffer.clear();
  if (undoData->stitchCount != 0U) {
	auto const span = gsl::span {undoData->stitches, undoData->stitchCount};
	Instance->stitchBuffer.insert(Instance->stitchBuffer.end(), span.begin(), span.end());
	Instance->stateMap.set(StateFlag::INIT);
  }
  else {
	Instance->stateMap.reset(StateFlag::INIT);
  }
  UnzoomedRect = undoData->zoomRect;

  auto& formList = Instance->formList;
  formList.clear();
  if (undoData->formCount != 0U) {
	auto const span = gsl::span {undoData->forms, undoData->formCount};
	formList.insert(formList.end(), span.begin(), span.end());
  }
  Instance->formVertices.clear();
  if (undoData->vertexCount != 0U) {
	auto const span = gsl::span {undoData->vertices, undoData->vertexCount};
	Instance->formVertices.insert(Instance->formVertices.end(), span.begin(), span.end());
  }
  Instance->satinGuides.clear();
  if (undoData->guideCount != 0U) {
	auto const span = gsl::span {undoData->guide, undoData->guideCount};
	Instance->satinGuides.insert(Instance->satinGuides.end(), span.begin(), span.end());
  }
  Instance->clipPoints.clear();
  if (undoData->clipPointCount != 0U) {
	auto const span = gsl::span {undoData->clipPoints, undoData->clipPointCount};
	Instance->clipPoints.insert(Instance->clipPoints.end(), span.begin(), span.end());
  }
  constexpr auto UCOLSIZE     = UserColor.size();
  auto const     spUndoColors = gsl::span {undoData->colors, gsl::narrow<ptrdiff_t>(UCOLSIZE)};
  auto const     spUserColors = gsl::span {UserColor};
  std::ranges::copy(spUndoColors.begin(), spUndoColors.end(), spUserColors.begin());
  thred::refreshColors();
  Instance->texturePointsBuffer.clear();
  if (undoData->texturePointCount != 0U) {
	auto const span = gsl::span {undoData->texturePoints, undoData->texturePointCount};
	Instance->texturePointsBuffer.insert(Instance->texturePointsBuffer.end(), span.begin(), span.end());
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

} // namespace

#pragma warning(push)
void backup::dudat() {
  auto& bufferElement = Instance->undoBuffer.at(UndoBufferWriteIndex);
  bufferElement.clear();
  auto const& formList  = Instance->formList;
  auto const  formCount = wrap::toUnsigned(formList.size());

  auto const size =
      wrap::sizeofVector(formList) + wrap::sizeofVector(Instance->stitchBuffer) +
      wrap::sizeofVector(Instance->formVertices) + wrap::sizeofVector(Instance->clipPoints) +
      wrap::sizeofVector(Instance->satinGuides) + wrap::sizeofVector(Instance->texturePointsBuffer) +
      wrap::toUnsigned(sizeof(BACK_HEAD)) + wrap::toUnsigned(sizeof(UserColor));
  bufferElement.resize(size);
  auto* backupData = convertFromPtr<BACK_HEAD*>(bufferElement.data());
  if (size != bufferElement.size()) {
	return;
  }
  backupData->zoomRect  = UnzoomedRect;
  backupData->formCount = formCount;
  backupData->forms     = convertFromPtr<FRM_HEAD*>(std::next(backupData, 1));
  if (formCount != 0) {
	auto const spForms = gsl::span {backupData->forms, formList.size()};
	std::ranges::copy(formList, spForms.begin());
  }
  backupData->stitchCount = wrap::toUnsigned(Instance->stitchBuffer.size());
  backupData->stitches =
      convertFromPtr<F_POINT_ATTR*>(std::next(backupData->forms, wrap::toPtrdiff(formCount)));
  if (!Instance->stitchBuffer.empty()) {
	auto const spStitches = gsl::span {backupData->stitches, Instance->stitchBuffer.size()};
	std::ranges::copy(Instance->stitchBuffer, spStitches.begin());
  }
  backupData->vertexCount = wrap::toUnsigned(Instance->formVertices.size());
  backupData->vertices    = convertFromPtr<F_POINT*>(
      std::next(backupData->stitches, wrap::toPtrdiff(Instance->stitchBuffer.size())));
  if (!Instance->formVertices.empty()) {
	auto const spVertices = gsl::span {backupData->vertices, Instance->formVertices.size()};
	std::ranges::copy(Instance->formVertices, spVertices.begin());
  }
  backupData->guideCount = wrap::toUnsigned(Instance->satinGuides.size());
  backupData->guide      = convertFromPtr<SAT_CON*>(
      std::next(backupData->vertices, wrap::toPtrdiff(Instance->formVertices.size())));
  if (!Instance->satinGuides.empty()) {
	auto const spGuides = gsl::span {backupData->guide, backupData->guideCount};
	std::ranges::copy(Instance->satinGuides, spGuides.begin());
  }
  backupData->clipPointCount = wrap::toUnsigned(Instance->clipPoints.size());
  backupData->clipPoints =
      convertFromPtr<F_POINT*>(std::next(backupData->guide, wrap::toPtrdiff(Instance->satinGuides.size())));
  if (!Instance->clipPoints.empty()) {
	auto const spClipPoints = gsl::span {backupData->clipPoints, backupData->clipPointCount};
	std::ranges::copy(Instance->clipPoints, spClipPoints.begin());
  }
  backupData->colors = convertFromPtr<COLORREF*>(
      std::next(backupData->clipPoints, wrap::toPtrdiff(Instance->clipPoints.size())));
  {
	auto const spColors = gsl::span {backupData->colors, COLORCNT};
	std::ranges::copy(UserColor, spColors.begin());
  }
  backupData->texturePoints     = convertFromPtr<TX_PNT*>(std::next(backupData->colors, COLORCNT));
  backupData->texturePointCount = wrap::toUnsigned(Instance->texturePointsBuffer.size());
  if (!Instance->texturePointsBuffer.empty()) {
	auto const spTexturePoints = gsl::span {backupData->texturePoints, backupData->texturePointCount};
	std::ranges::copy(Instance->texturePointsBuffer, spTexturePoints.begin());
  }
}
#pragma warning(pop)

void backup::deldu() {
  for (auto& bufferElement : Instance->undoBuffer) {
	bufferElement.clear();
	bufferElement.shrink_to_fit();
  }
  UndoBufferWriteIndex = 0;
  Instance->stateMap.reset(StateFlag::BAKWRAP);
  Instance->stateMap.reset(StateFlag::BAKACT);
}

void backup::redo() {
  ++UndoBufferWriteIndex;
  UndoBufferWriteIndex &= UNDOLEN - 1U;
  if (auto const nextBufferIndex = (UndoBufferWriteIndex + 1U) & (UNDOLEN - 1U);
      nextBufferIndex == UndoBufferReadIndex) {
	menu::disableRedo();
  }
  else {
	menu::enableRedo();
  }
  menu::enableUndo();
  redbak();
}

void backup::bak() {
  thred::unmsg();
  Instance->stateMap.reset(StateFlag::FPSEL);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
  Instance->stateMap.reset(StateFlag::BIGBOX);
  Instance->selectedFormList.clear();
  formForms::destroyFormDataSheet();
  if (Instance->stateMap.testAndReset(StateFlag::PRFACT)) {
	Instance->stateMap.reset(StateFlag::WASRT);
	formForms::destroyPreferencesWindow();
	thred::resetPrefIndex();
	thred::unsid(true);
  }
  if (!Instance->stateMap.testAndSet(StateFlag::BAKING)) {
	dudat();
	UndoBufferReadIndex = UndoBufferWriteIndex + 1U;
  }
  if (Instance->stateMap.test(StateFlag::BAKWRAP)) {
	--UndoBufferWriteIndex;
	UndoBufferWriteIndex &= UNDOLEN - 1U;
	if (auto const previousBufferIndex = UndoBufferWriteIndex - 1U; previousBufferIndex == UndoBufferReadIndex) {
	  menu::disableRedo();
	}
  }
  else {
	if (UndoBufferWriteIndex != 0U) {
	  --UndoBufferWriteIndex;
	}
	if (UndoBufferWriteIndex == 0U) {
	  menu::disableUndo();
	}
  }
  menu::enableRedo();
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->stateMap.reset(StateFlag::GRPSEL);
  Instance->stateMap.reset(StateFlag::SCROS);
  Instance->stateMap.reset(StateFlag::ECROS);
  redbak();
}

void backup::updateWriteIndex() {
  ++UndoBufferWriteIndex;
  UndoBufferWriteIndex &= UNDOLEN - 1U;
  if (UndoBufferWriteIndex == 0U) {
	Instance->stateMap.set(StateFlag::BAKWRAP);
  }
}
