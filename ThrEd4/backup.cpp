// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "backup.h"
#include "globals.h"
#include "menu.h"
#include "thred.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

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

  // constexpr BACK_HEAD() noexcept = default;
  // BACK_HEAD(CLIP_STITCH const&) = default;
  // BACK_HEAD(CLIP_STITCH&&) = default;
  // BACK_HEAD& operator=(BACK_HEAD const& rhs) = default;
  // BACK_HEAD& operator=(BACK_HEAD&&) = default;
  //~BACK_HEAD() = default;
};

// Main Variables
namespace {
auto UndoBufferWriteIndex = uint32_t {}; // undo storage pointer
auto UndoBufferReadIndex  = uint32_t {}; // undo retrieval pointers
} // namespace

// Supress bounds.1 	Don't use pointer arithmetic. Use span instead
#pragma warning(push)
#pragma warning(disable : 26481)
void backup::dudat() {
  auto& undoBuffer = *UndoBuffer;
  undoBuffer[UndoBufferWriteIndex].reset(nullptr);
  auto const formCount = wrap::toUnsigned(FormList->size());

  auto const size = wrap::sizeofVector(*FormList) + wrap::sizeofVector(StitchBuffer) +
                    wrap::sizeofVector(FormVertices) + wrap::sizeofVector(ClipPoints) +
                    wrap::sizeofVector(SatinGuides) + wrap::sizeofVector(TexturePointsBuffer) +
                    wrap::toUnsigned(sizeof(BACK_HEAD)) + wrap::toUnsigned(sizeof(UserColor));
  undoBuffer[UndoBufferWriteIndex] = std::make_unique<uint32_t[]>(size); // NOLINT(modernize-avoid-c-arrays)
  auto* backupData = convertFromPtr<BACK_HEAD*>(undoBuffer[UndoBufferWriteIndex].get());
  if (backupData == nullptr) {
	return;
  }
  backupData->zoomRect  = UnzoomedRect;
  backupData->formCount = formCount;
  backupData->forms     = convertFromPtr<FRM_HEAD*>(&backupData[1]);
  if (formCount != 0) {
	auto const spForms = gsl::span {backupData->forms, FormList->size()};
	std::copy(FormList->cbegin(), FormList->cend(), spForms.begin());
  }
  backupData->stitchCount = wrap::toUnsigned(StitchBuffer->size());
  backupData->stitches    = convertFromPtr<F_POINT_ATTR*>(&backupData->forms[formCount]);
  if (!StitchBuffer->empty()) {
	auto const spStitches = gsl::span {backupData->stitches, StitchBuffer->size()};
	std::copy(StitchBuffer->begin(), StitchBuffer->end(), spStitches.begin());
  }
  backupData->vertexCount = wrap::toUnsigned(FormVertices->size());
  backupData->vertices    = convertFromPtr<F_POINT*>(&backupData->stitches[StitchBuffer->size()]);
  if (!FormVertices->empty()) {
	auto const spVertices = gsl::span {backupData->vertices, FormVertices->size()};
	std::copy(FormVertices->cbegin(), FormVertices->cend(), spVertices.begin());
  }
  backupData->guideCount = wrap::toUnsigned(SatinGuides->size());
  backupData->guide      = convertFromPtr<SAT_CON*>(&backupData->vertices[FormVertices->size()]);
  if (!SatinGuides->empty()) {
	auto const spGuides = gsl::span {backupData->guide, backupData->guideCount};
	std::copy(SatinGuides->cbegin(), SatinGuides->cend(), spGuides.begin());
  }
  backupData->clipPointCount = wrap::toUnsigned(ClipPoints->size());
  backupData->clipPoints     = convertFromPtr<F_POINT*>(&backupData->guide[SatinGuides->size()]);
  if (!ClipPoints->empty()) {
	auto const spClipPoints = gsl::span {backupData->clipPoints, backupData->clipPointCount};
	std::copy(ClipPoints->cbegin(), ClipPoints->cend(), spClipPoints.begin());
  }
  backupData->colors = convertFromPtr<COLORREF*>(&backupData->clipPoints[ClipPoints->size()]);
  {
	auto const spColors = gsl::span {backupData->colors, COLORCNT};
	std::copy(std::begin(UserColor), std::end(UserColor), spColors.begin());
  }
  backupData->texturePoints     = convertFromPtr<TX_PNT*>(&backupData->colors[COLORCNT]);
  backupData->texturePointCount = wrap::toUnsigned(TexturePointsBuffer->size());
  if (!TexturePointsBuffer->empty()) {
	auto const spTexturePoints = gsl::span {backupData->texturePoints, backupData->texturePointCount};
	std::copy(TexturePointsBuffer->cbegin(), TexturePointsBuffer->cend(), spTexturePoints.begin());
  }
}
#pragma warning(pop)

void backup::deldu() {
  for (auto& undo : *UndoBuffer) {
	undo.reset(nullptr);
  }
  UndoBufferWriteIndex = 0;
  StateMap->reset(StateFlag::BAKWRAP);
  StateMap->reset(StateFlag::BAKACT);
}

void backup::redbak() {
  auto const* undoData = convertFromPtr<BACK_HEAD*>(UndoBuffer->operator[](UndoBufferWriteIndex).get());
  if (undoData == nullptr) {
	return;
  }
  StitchBuffer->clear();
  if (undoData->stitchCount != 0U) {
	auto const span = gsl::span {undoData->stitches, undoData->stitchCount};
	StitchBuffer->insert(StitchBuffer->end(), span.begin(), span.end());
  }
  else {
	StateMap->reset(StateFlag::INIT);
  }
  UnzoomedRect = undoData->zoomRect;
  FormList->clear();
  if (undoData->formCount != 0U) {
	auto const span = gsl::span {undoData->forms, undoData->formCount};
	FormList->insert(FormList->end(), span.begin(), span.end());
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
  ClipPoints->clear();
  if (undoData->clipPointCount != 0U) {
	auto const span = gsl::span {undoData->clipPoints, undoData->clipPointCount};
	ClipPoints->insert(ClipPoints->end(), span.begin(), span.end());
  }
  // ToDo - add field in BACK_HEAD to keep track of number of colors
  constexpr auto UCOLSIZE     = UserColor.size();
  auto const     spUndoColors = gsl::span {undoData->colors, gsl::narrow<ptrdiff_t>(UCOLSIZE)};
  auto const     spUserColors = gsl::span {UserColor};
  std::copy(spUndoColors.begin(), spUndoColors.end(), spUserColors.begin());
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
  UndoBufferWriteIndex &= (UNDOLEN - 1U);
  auto const nextBufferIndex = (UndoBufferWriteIndex + 1U) & (UNDOLEN - 1U);
  if (nextBufferIndex == UndoBufferReadIndex) {
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
	UndoBufferWriteIndex &= (UNDOLEN - 1U);
	auto const previousBufferIndex = UndoBufferWriteIndex - 1U;
	if (previousBufferIndex == UndoBufferReadIndex) {
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
  redbak();
}

void backup::updateWriteIndex() {
  ++UndoBufferWriteIndex;
  UndoBufferWriteIndex &= (UNDOLEN - 1U);
  if (UndoBufferWriteIndex == 0U) {
	StateMap->set(StateFlag::BAKWRAP);
  }
}