// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "backup.h"
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
#include <WinUser.h>

// Standard Libraries
#include <cstddef>
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

// Main Variables
namespace {
auto UndoBufferWriteIndex = uint32_t {}; // undo storage pointer
auto UndoBufferReadIndex  = uint32_t {}; // undo retrieval pointers
} // namespace

namespace bui {
void redbak();
} // namespace bui

#pragma warning(push)
void backup::dudat() {
  auto& bufferElement = UndoBuffer->at(UndoBufferWriteIndex);
  bufferElement.clear();
  auto const& formList = Instance->FormList;
  auto const formCount = wrap::toUnsigned(formList.size());

  auto const size = wrap::sizeofVector(formList) + wrap::sizeofVector(StitchBuffer) +
                    wrap::sizeofVector(FormVertices) + wrap::sizeofVector(Instance->ClipPoints) +
                    wrap::sizeofVector(SatinGuides) + wrap::sizeofVector(TexturePointsBuffer) +
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
	std::ranges::copy(formList.cbegin(), formList.cend(), spForms.begin());
  }
  backupData->stitchCount = wrap::toUnsigned(StitchBuffer->size());
  backupData->stitches =
      convertFromPtr<F_POINT_ATTR*>(std::next(backupData->forms, wrap::toPtrdiff(formCount)));
  if (!StitchBuffer->empty()) {
	auto const spStitches = gsl::span {backupData->stitches, StitchBuffer->size()};
	std::ranges::copy(StitchBuffer->begin(), StitchBuffer->end(), spStitches.begin());
  }
  backupData->vertexCount = wrap::toUnsigned(FormVertices->size());
  backupData->vertices =
      convertFromPtr<F_POINT*>(std::next(backupData->stitches, wrap::toPtrdiff(StitchBuffer->size())));
  if (!FormVertices->empty()) {
	auto const spVertices = gsl::span {backupData->vertices, FormVertices->size()};
	std::ranges::copy(FormVertices->cbegin(), FormVertices->cend(), spVertices.begin());
  }
  backupData->guideCount = wrap::toUnsigned(SatinGuides->size());
  backupData->guide =
      convertFromPtr<SAT_CON*>(std::next(backupData->vertices, wrap::toPtrdiff(FormVertices->size())));
  if (!SatinGuides->empty()) {
	auto const spGuides = gsl::span {backupData->guide, backupData->guideCount};
	std::ranges::copy(SatinGuides->cbegin(), SatinGuides->cend(), spGuides.begin());
  }
  backupData->clipPointCount = wrap::toUnsigned(Instance->ClipPoints.size());
  backupData->clipPoints =
      convertFromPtr<F_POINT*>(std::next(backupData->guide, wrap::toPtrdiff(SatinGuides->size())));
  if (!Instance->ClipPoints.empty()) {
	auto const spClipPoints = gsl::span {backupData->clipPoints, backupData->clipPointCount};
	std::ranges::copy(Instance->ClipPoints.cbegin(), Instance->ClipPoints.cend(), spClipPoints.begin());
  }
  backupData->colors =
      convertFromPtr<COLORREF*>(std::next(backupData->clipPoints, wrap::toPtrdiff(Instance->ClipPoints.size())));
  {
	auto const spColors = gsl::span {backupData->colors, COLORCNT};
	std::ranges::copy(std::begin(UserColor), std::end(UserColor), spColors.begin());
  }
  backupData->texturePoints     = convertFromPtr<TX_PNT*>(std::next(backupData->colors, COLORCNT));
  backupData->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
  if (!TexturePointsBuffer->empty()) {
	auto const spTexturePoints = gsl::span {backupData->texturePoints, backupData->texturePointCount};
	std::ranges::copy(TexturePointsBuffer->cbegin(), TexturePointsBuffer->cend(), spTexturePoints.begin());
  }
}
#pragma warning(pop)

void backup::deldu() {
  for (auto& bufferElement : *UndoBuffer) {
	bufferElement.clear();
  }
  UndoBufferWriteIndex = 0;
  StateMap->reset(StateFlag::BAKWRAP);
  StateMap->reset(StateFlag::BAKACT);
}

void bui::redbak() {
  auto bufferElement = UndoBuffer->at(UndoBufferWriteIndex);
  if (bufferElement.empty()) {
	return;
  }
  auto const* undoData = convertFromPtr<BACK_HEAD*>(bufferElement.data());
  StitchBuffer->clear();
  if (undoData->stitchCount != 0U) {
	auto const span = gsl::span {undoData->stitches, undoData->stitchCount};
	StitchBuffer->insert(StitchBuffer->end(), span.begin(), span.end());
	StateMap->set(StateFlag::INIT);
  }
  else {
	StateMap->reset(StateFlag::INIT);
  }
  UnzoomedRect = undoData->zoomRect;

  auto& formList = Instance->FormList;
  formList.clear();
  if (undoData->formCount != 0U) {
	auto const span = gsl::span {undoData->forms, undoData->formCount};
	formList.insert(formList.end(), span.begin(), span.end());
  }
  FormVertices->clear();
  if (undoData->vertexCount != 0U) {
	auto const span = gsl::span {undoData->vertices, undoData->vertexCount};
	FormVertices->insert(FormVertices->end(), span.begin(), span.end());
  }
  SatinGuides->clear();
  if (undoData->guideCount != 0U) {
	auto const span = gsl::span {undoData->guide, undoData->guideCount};
	SatinGuides->insert(SatinGuides->end(), span.begin(), span.end());
  }
  Instance->ClipPoints.clear();
  if (undoData->clipPointCount != 0U) {
	auto const span = gsl::span {undoData->clipPoints, undoData->clipPointCount};
	Instance->ClipPoints.insert(Instance->ClipPoints.end(), span.begin(), span.end());
  }
  constexpr auto UCOLSIZE     = UserColor.size();
  auto const     spUndoColors = gsl::span {undoData->colors, gsl::narrow<ptrdiff_t>(UCOLSIZE)};
  auto const     spUserColors = gsl::span {UserColor};
  std::ranges::copy(spUndoColors.begin(), spUndoColors.end(), spUserColors.begin());
  thred::refreshColors();
  TexturePointsBuffer->clear();
  if (undoData->texturePointCount != 0U) {
	auto const span = gsl::span {undoData->texturePoints, undoData->texturePointCount};
	TexturePointsBuffer->insert(TexturePointsBuffer->end(), span.begin(), span.end());
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
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
  bui::redbak();
}

void backup::bak() {
  thred::unmsg();
  StateMap->reset(StateFlag::FPSEL);
  StateMap->reset(StateFlag::FRMPSEL);
  StateMap->reset(StateFlag::BIGBOX);
  SelectedFormList->clear();
  thred::undat();
  if (StateMap->testAndReset(StateFlag::PRFACT)) {
	StateMap->reset(StateFlag::WASRT);
	DestroyWindow(PreferencesWindow);
	PreferenceIndex = 0;
	thred::unsid();
  }
  if (!StateMap->testAndSet(StateFlag::BAKING)) {
	dudat();
	UndoBufferReadIndex = UndoBufferWriteIndex + 1U;
  }
  if (StateMap->test(StateFlag::BAKWRAP)) {
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
  StateMap->reset(StateFlag::FORMSEL);
  StateMap->reset(StateFlag::GRPSEL);
  StateMap->reset(StateFlag::SCROS);
  StateMap->reset(StateFlag::ECROS);
  bui::redbak();
}

void backup::updateWriteIndex() {
  ++UndoBufferWriteIndex;
  UndoBufferWriteIndex &= UNDOLEN - 1U;
  if (UndoBufferWriteIndex == 0U) {
	StateMap->set(StateFlag::BAKWRAP);
  }
}
