// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "clip.h"
#include "displayText.h"
#include "EnumMap.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "Resources/resource.h"
#include "satin.h"
#include "texture.h"
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
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "gsl/narrow"
#include "gsl/util"
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
#include <sal.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cstring>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cctype>
#include <filesystem>
#include <iterator>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

class TX_HIST_BUFF
{
  public:
  uint32_t count {};

  float height {};
  float width {};
  float spacing {};
};

#pragma pack(push, 1)
class TX_OFF // textured fill offset
{
  public:
  float   y {};
  int32_t line {};
};
#pragma pack(pop)

class TXTR_RECT
{
  public:
  uint16_t left {};
  uint16_t right {};
  float    top {};
  float    bottom {};
};

class TXT_SINGLE
{
  public:
  static auto getInstance() noexcept -> TXT_SINGLE* {
	// NOLINTNEXTLINE(clang-diagnostic-exit-time-destructors)
	static TXT_SINGLE instance;
	return &instance;
  }

  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::vector<TX_HIST>  TextureHistory;            // texture editor history headers
  std::vector<TX_PNT>   TempTexturePoints;         // temporary storage for textured fill data
  std::vector<uint32_t> SelectedTexturePointsList; // list of selected points
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  TXT_SINGLE(const TXT_SINGLE&)                    = delete;
  auto operator=(const TXT_SINGLE&) -> TXT_SINGLE& = delete;
  TXT_SINGLE(TXT_SINGLE&&)                         = delete;
  auto operator=(TXT_SINGLE&&) -> TXT_SINGLE&      = delete;

  private:
  TXT_SINGLE() noexcept = default;
  ~TXT_SINGLE()         = default;
};

// texture internal namespace
namespace {
constexpr auto ITXBUFSZ = uint32_t {16U}; // texture buffer depth
constexpr auto OSCLAMP  = -0.5F;          // values below this are off screen and should be clamped
constexpr auto TXTRAT   = 0.95F;          // texture fill clipboard shrink/grow ratio

enum TextureStyles : uint8_t { VRTYP, HORTYP, ANGTYP };

auto SelectTexturePointsOrigin = POINT {};       // original location of selected texture points
auto SideWindowButton          = HWND {};        // button side window
auto TextureCrossPen           = HPEN {};        // texture editor cross pen
auto TextureCursorLocation     = POINT {};       // texture editor move cursor location
auto TextureHistoryIndex       = uint32_t {};    // pointer to the next texture history buffer
auto TexturePixelRect          = RECT {};        // screen selected texture points rectangle
auto TextureRect               = TXTR_RECT {};   // selected texture points rectangle
auto TextureScreen             = TXTR_SCREEN {}; // texture editor layout parameters
auto TextureWindowId           = uint32_t {};    // id of the window being updated

TXT_SINGLE* TextureInstance;

// Definitions
void altx();
void angrct(F_RECTANGLE& rectangle) noexcept;
void butsid(uint32_t windowId);
auto chkbut() -> bool;
auto chktxh(_In_ TX_HIST const& historyItem) noexcept(std::is_same_v<size_t, uint32_t>) -> bool;
void chktxnum();
void deorg(POINT& point) noexcept;
void doTexAdjust(FRM_HEAD& current, std::vector<TX_PNT>& textureBuffer, uint16_t& iBuffer);
void dutxfn(uint32_t textureType);
void dutxlin(F_POINT const& point0in, F_POINT const& point1in);
void dutxmir();
void dutxrct(TXTR_RECT& textureRect) noexcept;
void dutxtlin() noexcept;
void dutxtx(uint32_t index, uint16_t offsetPixels) noexcept(std::is_same_v<size_t, uint32_t>);
void ed2px(F_POINT const& editPoint, POINT& point) noexcept;
auto ed2stch(F_POINT const& point) noexcept -> F_POINT;
void ed2txp(POINT const& offset, TX_PNT& textureRecord);
auto inrct(F_RECTANGLE const& rectangle, F_POINT_ATTR const& stitch) noexcept -> bool;
void nutx(uint32_t formIndex);
void nxbak();
void px2ed(POINT const& point, F_POINT& editPoint) noexcept;
auto px2txt(POINT const& offset) -> bool;
void redtbak();
void ritxfrm(FRM_HEAD const& textureForm);
void ritxrct() noexcept(std::is_same_v<size_t, uint32_t>);
void rollbackTexture(std::vector<TX_HIST>::iterator const& texture);
void setxclp(FRM_HEAD const& form);
void setxmov();
void stxlin();
void txang(FRM_HEAD& form);
void txbak();
auto txbutfn() -> bool;
auto tpComp(TX_PNT const& texturePoint0, TX_PNT const& texturePoint1) noexcept -> bool;
void txcntrv(FRM_HEAD const& textureForm);
void txdelal();
auto txdig(wchar_t keyCode, wchar_t& character) noexcept -> bool;
void txfn(uint32_t textureType, uint32_t formIndex);
void txgro(FRM_HEAD const& textureForm);
void txhor(FRM_HEAD& form);
auto txnam(std::wstring& name) -> bool;
void txnudg(int32_t deltaX, float deltaY);
void txpar(FRM_HEAD& form);
void txrbak() noexcept(std::is_same_v<size_t, uint32_t>);
void txrct2rct(TXTR_RECT const& textureRect, RECT& rectangle) noexcept;
void txrfor() noexcept;
void txshrnk(FRM_HEAD const& textureForm);
void txsiz(float ratio, FRM_HEAD const& textureForm);
void txt2pix(TX_PNT const& texturePoint, POINT& screenPoint) noexcept;
auto txtclos(uint32_t& closestTexturePoint) noexcept(std::is_same_v<size_t, uint32_t>) -> bool;
void txtdel();
void txtlin();
void txtxfn(POINT const& reference, uint16_t offsetPixels) noexcept(std::is_same_v<size_t, uint32_t>);
void txvrt(FRM_HEAD& form);

// Functions
// Ensure all lines in the texture have at least 1 point
void dutxmir() {
  auto& tempTexturePoints = TextureInstance->TempTexturePoints;
  if (tempTexturePoints.empty()) {
	return;
  }
  auto const centerLine = (TextureScreen.lines + 1U) / 2;
  auto const evenOffset = 1U - (TextureScreen.lines & 1U);
  texture::savtxt();
  std::ranges::sort(tempTexturePoints, tpComp);
  auto iPoint = wrap::toUnsigned(tempTexturePoints.size()) - 1U;
  while (tempTexturePoints.operator[](iPoint).line > centerLine) {
	--iPoint;
  }
  tempTexturePoints.resize(wrap::toSize(iPoint) + 1U);
  auto const iMirrorPoint = iPoint + evenOffset;
  for (auto index = 0U; index < iMirrorPoint; ++index) {
	auto const newLine =
	    gsl::narrow_cast<uint16_t>(TextureScreen.lines - tempTexturePoints.operator[](index).line + 1U);
	tempTexturePoints.emplace_back(TX_PNT {.y = tempTexturePoints.operator[](index).y, .line = newLine});
  }
  Instance->StateMap.set(StateFlag::RESTCH);
}

void dutxrct(TXTR_RECT& textureRect) noexcept {
  auto const& selectedTexturePointsList = TextureInstance->SelectedTexturePointsList;
  if (selectedTexturePointsList.empty()) {
	textureRect.left = textureRect.right = 0U;
	textureRect.top = textureRect.bottom = 0.0F;
	return;
  }

  auto minX = BGUINT16;
  auto maxX = uint16_t {0U};
  auto maxY = LOWFLOAT;
  auto minY = BIGFLOAT;
  for (auto const& iPoint : selectedTexturePointsList) {
	auto const& texturePoint = TextureInstance->TempTexturePoints.operator[](iPoint);

	minX = std::min(minX, texturePoint.line);
	maxX = std::max(maxX, texturePoint.line);
	minY = std::min(minY, texturePoint.y);
	maxY = std::max(maxY, texturePoint.y);
  }
  textureRect = TXTR_RECT {.left = minX, .right = maxX, .top = maxY, .bottom = minY};
}

void dutxtlin() noexcept {
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, Instance->formLines.data(), LNPNTS);
}

void dutxtx(uint32_t const index, uint16_t const offsetPixels) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto ref = POINT {};
  txt2pix(TextureInstance->TempTexturePoints.operator[](index), ref);
  txtxfn(ref, offsetPixels);
  if (ref.y > TextureScreen.halfHeight) {
	ref.y -= TextureScreen.height;
  }
  else {
	ref.y += TextureScreen.height;
  }
  txtxfn(ref, offsetPixels);
}

void ed2px(F_POINT const& editPoint, POINT& point) noexcept {
  point = POINT {std::lround(editPoint.x / TextureScreen.editToPixelRatio),
                 std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                             (editPoint.y / TextureScreen.editToPixelRatio))};
}

auto ed2stch(F_POINT const& point) noexcept -> F_POINT {
  return F_POINT {(point.x - TextureScreen.xOffset), (point.y - TextureScreen.yOffset)};
}

void ed2txp(POINT const& offset, TX_PNT& textureRecord) {
  auto point = F_POINT {};
  px2ed(offset, point);
  auto val = (point.x - TextureScreen.xOffset) / TextureScreen.spacing;

  if (val < OSCLAMP) {
	val = 0.0F;
  }
  textureRecord.line = wrap::round<uint16_t>(val);
  textureRecord.y    = TextureScreen.areaHeight -
                    ((wrap::toFloat(offset.y - TextureScreen.top) / wrap::toFloat(TextureScreen.height)) *
                     TextureScreen.areaHeight);
}

auto inrct(F_RECTANGLE const& rectangle, F_POINT_ATTR const& stitch) noexcept -> bool {
  return stitch.x >= rectangle.left && stitch.x <= rectangle.right &&
         stitch.y >= rectangle.bottom && stitch.y <= rectangle.top;
}

void nutx(uint32_t const formIndex) {
  auto& tempTexturePoints = TextureInstance->TempTexturePoints;
  auto& formList          = Instance->FormList;

  if (formList.empty()) {
	return;
  }
  auto& form = formList.operator[](formIndex);

  auto index = gsl::narrow_cast<uint16_t>(0U);
  if (form.isTexture()) {
	index = form.texture.index;
	texture::deltx(formIndex);
  }
  else {
	for (auto spForms = std::ranges::subrange(formList.begin(), wrap::next(formList.begin(), formIndex));
	     auto const& current : spForms | std::views::reverse) {
	  if (current.isTexture()) {
		auto const& texture = current.texture;
		index               = texture.index + texture.count;
		break;
	  }
	}
  }
  if (tempTexturePoints.empty()) {
	return;
  }
  auto const tempPointCount = gsl::narrow<decltype(formList.back().texture.index)>(tempTexturePoints.size());
  std::ranges::sort(tempTexturePoints, tpComp);
  auto const itPoint = wrap::next(Instance->TexturePointsBuffer.begin(), index);
  Instance->TexturePointsBuffer.insert(itPoint, tempTexturePoints.cbegin(), tempTexturePoints.cend());
  for (auto spForms =
           std::ranges::subrange(wrap::next(formList.begin(), formIndex + 1U), Instance->FormList.end());
       auto& current : spForms) {
	if (current.isTexture()) {
	  current.texture.index += tempPointCount;
	}
  }
  form.texture.index = index;
  form.texture.count = tempPointCount;
}

void nxbak() {
  if (!Instance->StateMap.test(StateFlag::WASTXBAK)) {
	return;
  }
  auto       flag = false;
  auto const end  = wrap::toUnsigned(TextureInstance->TextureHistory.size());
  for (auto iHistory = uint32_t {0U}; iHistory < end; ++iHistory) {
	txrfor();
	if (TextureInstance->TextureHistory.operator[](TextureHistoryIndex).width != 0.0F) {
	  flag = true;
	  break;
	}
  }
  if (flag) {
	redtbak();
  }
}

void px2ed(POINT const& point, F_POINT& editPoint) noexcept {
  editPoint = F_POINT {wrap::toFloat(point.x) * TextureScreen.editToPixelRatio,
                       TextureScreen.screenHeight - (wrap::toFloat(point.y) * TextureScreen.editToPixelRatio)};
}

auto px2txt(POINT const& offset) -> bool {
  auto editPoint = F_POINT {};
  px2ed(offset, editPoint);
  auto const line = (editPoint.x - TextureScreen.xOffset) / TextureScreen.spacing;

  if (line < OSCLAMP) {
	return false;
  }
  auto txPoint = TX_PNT {.y = 0.0F, .line = wrap::round<uint16_t>(line)};
  if (txPoint.line > TextureScreen.lines || offset.y <= TextureScreen.top ||
      offset.y > TextureScreen.bottom) {
	return false;
  }
  txPoint.y = TextureScreen.areaHeight -
              (wrap::toFloat(offset.y - TextureScreen.top) / wrap::toFloat(TextureScreen.height)) *
                  TextureScreen.areaHeight;
  TextureInstance->TempTexturePoints.push_back(txPoint);
  return true;
}

void redtbak() {
  outDebugString(L"retrieving texture history {}\n", TextureHistoryIndex);
  auto&       tempTexturePoints  = TextureInstance->TempTexturePoints;
  auto const& textureHistoryItem = TextureInstance->TextureHistory.operator[](TextureHistoryIndex);
  TextureScreen.areaHeight       = textureHistoryItem.height;
  TextureScreen.width            = textureHistoryItem.width;
  TextureScreen.spacing          = textureHistoryItem.spacing;
  if (!textureHistoryItem.texturePoints.empty()) {
	tempTexturePoints.clear();
	tempTexturePoints.reserve(textureHistoryItem.texturePoints.size());
	tempTexturePoints.insert(tempTexturePoints.begin(),
	                         textureHistoryItem.texturePoints.cbegin(),
	                         textureHistoryItem.texturePoints.cend());
  }
  Instance->StateMap.set(StateFlag::RESTCH);
}

void ritxfrm(FRM_HEAD const& textureForm) {
  auto const offset = POINT {(TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                             (TextureCursorLocation.y - SelectTexturePointsOrigin.y)};

  auto& formLines = Instance->formLines;
  formLines.resize(wrap::toSize(textureForm.vertexCount) + 1U);
  auto const& angledFormVertices = Instance->angledFormVertices;
  for (auto formLine = formLines.begin(); auto const& vertex : angledFormVertices) {
	ed2px(vertex, *formLine);
	formLine->x += offset.x;
	formLine->y += offset.y;
	++formLine;
  }
  formLines.back() = formLines.front();
  auto vertexCount = wrap::toUnsigned(angledFormVertices.size());
  if (textureForm.type != FRMLINE) {
	++vertexCount;
  }
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
  wrap::polyline(StitchWindowDC, formLines.data(), vertexCount);
}

void ritxrct() noexcept(std::is_same_v<size_t, uint32_t>) {
  auto const offset = POINT {(TextureCursorLocation.x - SelectTexturePointsOrigin.x),
                             (TextureCursorLocation.y - SelectTexturePointsOrigin.y)};

  auto const rectangle = RECT {(TexturePixelRect.left + offset.x),
                               (TexturePixelRect.top + offset.y),
                               (TexturePixelRect.right + offset.x),
                               (TexturePixelRect.bottom + offset.y)};

  auto line = std::array<POINT, SQPNTS> {};

  line[0].x = line[1].x = line[4].x = rectangle.left;
  line[2].x = line[3].x = rectangle.right;
  line[0].y = line[3].y = line[4].y = rectangle.top;
  line[1].y = line[2].y = rectangle.bottom;
  wrap::polyline(StitchWindowDC, line.data(), wrap::toUnsigned(line.size()));
}

void rollbackTexture(std::vector<TX_HIST>::iterator const& texture) {
  auto dist = wrap::toUnsigned(std::distance(TextureInstance->TextureHistory.begin(), texture));
  TextureHistoryIndex = dist == 0 ? wrap::toUnsigned(TextureInstance->TextureHistory.size() - 1U) : --dist;
  texture->height  = 0;
  texture->width   = 0;
  texture->spacing = 0;
  texture->texturePoints.clear();
  texture->texturePoints.shrink_to_fit();
}

void setxclp(FRM_HEAD const& form) {
  auto screenOffset = POINT {};
  auto editorOffset = F_POINT {};
  deorg(screenOffset);
  px2ed(screenOffset, editorOffset);
  if (Instance->StateMap.testAndReset(StateFlag::TXHCNTR)) {
	editorOffset.x = wrap::toFloat(TextureScreen.lines) * TextureScreen.spacing * HALF +
	                 TextureScreen.xOffset - TextureScreen.formCenter.x + TextureScreen.spacing * HALF;
  }
  else {
	editorOffset.x -= TextureScreen.formCenter.x;
  }
  editorOffset.y -= TextureScreen.formCenter.y;
  auto& angledFormVertices = Instance->angledFormVertices;
  std::ranges::transform(angledFormVertices, angledFormVertices.begin(), [editorOffset](auto& vertex) {
	return vertex + editorOffset;
  });
  auto lineCount = form.vertexCount - 1U;
  if (form.type != FRMLINE) {
	++lineCount;
  }
  for (auto iLine = 0U; iLine < lineCount; ++iLine) {
	auto const iNextVertex = form::nxt(form, iLine);
	dutxlin(angledFormVertices[iLine], angledFormVertices[iNextVertex]);
  }
}

void setxmov() {
  Instance->StateMap.set(StateFlag::TXTMOV);
  TextureCursorLocation.x = SelectTexturePointsOrigin.x = WinMsg.pt.x - StitchWindowOrigin.x;
  TextureCursorLocation.y = SelectTexturePointsOrigin.y = WinMsg.pt.y - StitchWindowOrigin.y;
  SetROP2(StitchWindowDC, R2_NOTXORPEN);
}

void stxlin() {
  auto offset = POINT {};
  auto point0 = F_POINT {};
  auto point1 = F_POINT {};
  Instance->StateMap.reset(StateFlag::TXTMOV);
  deorg(offset);
  px2ed(offset, point1);
  px2ed(Instance->formLines.front(), point0);
  dutxlin(point0, point1);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void txang(FRM_HEAD& form) {
  if (TextureInstance->TempTexturePoints.empty()) {
	return;
  }
  form.fillType  = TXANGF;
  form.fillAngle = IniFile.fillAngle;
  txpar(form);
}

void txbak() {
  if (!Instance->StateMap.test(StateFlag::WASTXBAK)) {
	return;
  }
  TextureInstance->SelectedTexturePointsList.clear();
  auto       flag = false;
  auto const end  = wrap::toUnsigned(TextureInstance->TextureHistory.size());
  for (auto iHistory = 0U; iHistory < end; ++iHistory) {
	if (TextureInstance->TextureHistory.operator[](TextureHistoryIndex).width != 0.0F) {
	  flag = true;
	  break;
	}
	txrbak();
  }
  if (flag) {
	redtbak();
	txrbak();
  }
}

auto txbutfn() -> bool {
  auto offset = POINT {};
  deorg(offset);
  return px2txt(offset);
}

auto tpComp(TX_PNT const& texturePoint0, TX_PNT const& texturePoint1) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (texturePoint0.line < texturePoint1.line) {
	return true;
  }
  if (texturePoint1.line < texturePoint0.line) {
	return false;
  }
  if (texturePoint0.y < texturePoint1.y) {
	return true;
  }
  if (texturePoint1.y < texturePoint0.y) {
	return false;
  }

  return false;
}

void txcntrv(FRM_HEAD const& textureForm) {
  if (Instance->StateMap.testAndReset(StateFlag::TXTCLP)) {
	Instance->StateMap.set(StateFlag::TXHCNTR);
	texture::savtxt();
	setxclp(textureForm);
	Instance->StateMap.set(StateFlag::RESTCH);
  }
}

void txdelal() {
  Instance->textureInputBuffer.clear();
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  Instance->StateMap.set(StateFlag::RESTCH);
  texture::savtxt();
  TextureInstance->TempTexturePoints.clear();
  texture::rstxt();
  Instance->StateMap.set(StateFlag::RESTCH);
}

auto txdig(wchar_t const keyCode, wchar_t& character) noexcept -> bool {
  if (isdigit(keyCode) != 0) {
	character = keyCode;
	return true;
  }
  if (keyCode >= VK_NUMPAD0 && keyCode <= VK_NUMPAD9) {
	character = keyCode - VK_NUMPAD0 + L'0';
	return true;
  }
  if (keyCode == VK_OEM_PERIOD || keyCode == VK_DECIMAL) {
	character = L'.';
	return true;
  }
  return false;
}

void txfn(uint32_t const textureType, uint32_t const formIndex) {
  auto& form = Instance->FormList.operator[](formIndex);
  clip::delmclp(formIndex);
  if (form.satinGuideCount != 0U) {
	satin::delsac(formIndex);
  }
  texture::savtxt();
  nutx(formIndex);
  form.squareEnd(Instance->userFlagMap.test(UserFlag::SQRFIL));
  switch (textureType) {
	case VRTYP: {
	  txvrt(form);
	  break;
	}
	case HORTYP: {
	  txhor(form);
	  break;
	}
	case ANGTYP: {
	  txang(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in dutxfn: textureType [{}]\n", textureType);
	  break;
	}
  }
}

void txgro(FRM_HEAD const& textureForm) {
  txsiz(1.0F / TXTRAT, textureForm);
}

void txhor(FRM_HEAD& form) {
  if (TextureInstance->TempTexturePoints.empty()) {
	return;
  }
  form.fillType = TXHORF;
  txpar(form);
}

auto txnam(std::wstring& name) -> bool {
  auto const& texturePath = thred::getHomeDir();
  auto const  textureFile = texturePath / L"thred.txr";
  name.assign(textureFile.generic_wstring());
  return true;
}

void txnudg(int32_t const deltaX, float const deltaY) {
  auto const& selectedTexturePointsList = TextureInstance->SelectedTexturePointsList;
  auto&       tempTexturePoints         = TextureInstance->TempTexturePoints;
  if (selectedTexturePointsList.empty()) {
	return;
  }
  if (deltaY != 0.0F) {
	auto const screenDeltaY = deltaY * TextureScreen.editToPixelRatio;
	for (auto const& point : selectedTexturePointsList) {
	  if (auto const yCoord = tempTexturePoints.operator[](point).y + screenDeltaY;
	      yCoord < 0.0F || yCoord > TextureScreen.areaHeight) {
		return;
	  }
	}
	for (auto const& point : selectedTexturePointsList) {
	  tempTexturePoints.operator[](point).y += screenDeltaY;
	}
  }
  else {
	for (auto const& point : selectedTexturePointsList) {
	  if (auto const textureLine = tempTexturePoints.operator[](point).line + deltaX;
	      textureLine < 1 || textureLine > TextureScreen.lines) {
		return;
	  }
	}
	for (auto const& point : selectedTexturePointsList) {
	  auto& location = tempTexturePoints.operator[](point).line;
	  location       = gsl::narrow<decltype(tempTexturePoints.back().line)>(location + deltaX);
	}
  }
  dutxrct(TextureRect);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void txpar(FRM_HEAD& form) {
  form.type = FRMFPOLY;
  wrap::narrow(form.texture.lines, TextureScreen.lines);
  form.texture.height   = TextureScreen.areaHeight;
  form.fillSpacing      = TextureScreen.spacing;
  form.stitchLength     = IniFile.userStitchLength;
  form.maxFillStitchLen = IniFile.maxStitchLength;
  form.minFillStitchLen = IniFile.minStitchLength;
  form.fillColor        = ActiveColor;
  form::refilfn(ClosestFormToCursor);
}

void txrbak() noexcept(std::is_same_v<size_t, uint32_t>) {
  if (TextureHistoryIndex == 0) {
	TextureHistoryIndex = wrap::toUnsigned(TextureInstance->TextureHistory.size() - 1U);
	return;
  }
  --TextureHistoryIndex;
}

void txrct2rct(TXTR_RECT const& textureRect, RECT& rectangle) noexcept {
  auto texturePoint = TX_PNT {.y = textureRect.top, .line = textureRect.left};
  auto point        = POINT {};
  txt2pix(texturePoint, point);
  rectangle.left    = point.x - IniFile.textureEditorSize;
  rectangle.top     = point.y - IniFile.textureEditorSize;
  texturePoint.y    = textureRect.bottom;
  texturePoint.line = textureRect.right;
  txt2pix(texturePoint, point);
  rectangle.right  = point.x + IniFile.textureEditorSize;
  rectangle.bottom = point.y + IniFile.textureEditorSize;
}

void txrfor() noexcept {
  if (TextureHistoryIndex == TextureInstance->TextureHistory.size() - 1U) {
	TextureHistoryIndex = 0;
	return;
  }
  ++TextureHistoryIndex;
}

void txshrnk(FRM_HEAD const& textureForm) {
  txsiz(TXTRAT, textureForm);
}

void txsiz(float const ratio, FRM_HEAD const& textureForm) {
  ritxfrm(textureForm);
  for (auto& vertex : Instance->angledFormVertices) {
	vertex *= ratio;
  }
  auto angleRect = F_RECTANGLE {};
  angrct(angleRect);
  TextureScreen.formCenter =
      F_POINT {wrap::midl(angleRect.right, angleRect.left), wrap::midl(angleRect.top, angleRect.bottom)};
  ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
  ritxfrm(textureForm);
}

void txt2pix(TX_PNT const& texturePoint, POINT& screenPoint) noexcept {
  screenPoint.x =
      std::lround((TextureScreen.spacing * wrap::toFloat(texturePoint.line) + TextureScreen.xOffset) /
                  TextureScreen.editToPixelRatio);
  screenPoint.y =
      std::lround(wrap::toFloat(TextureScreen.height) -
                  (texturePoint.y / TextureScreen.areaHeight * wrap::toFloat(TextureScreen.height)) +
                  wrap::toFloat(TextureScreen.top));
}

auto txtclos(uint32_t& closestTexturePoint) noexcept(std::is_same_v<size_t, uint32_t>) -> bool {
  if (closestTexturePoint == 0U) {
	return false;
  }
  auto minimumLength = BIGLONG;
  auto reference     = POINT {};
  auto point         = POINT {};
  deorg(reference);
  closestTexturePoint = 0;
  auto const end      = wrap::toUnsigned(TextureInstance->TextureHistory.size());
  for (auto iPoint = uint32_t {0U}; iPoint < end; ++iPoint) {
	txt2pix(TextureInstance->TempTexturePoints.operator[](iPoint), point);
	auto const deltaX = point.x - reference.x;
	auto const deltaY = point.y - reference.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
	  minimumLength       = length;
	  closestTexturePoint = iPoint;
	}
  }
  return std::sqrtf(wrap::toFloat(minimumLength)) < FCLOSNUF;
}

void txtdel() {
  auto& selectedTexturePointsList = TextureInstance->SelectedTexturePointsList;
  auto& tempTexturePoints         = TextureInstance->TempTexturePoints;
  if (!selectedTexturePointsList.empty()) {
	texture::savtxt();
	auto texturePointsMap = boost::dynamic_bitset(tempTexturePoints.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	for (auto const& point : selectedTexturePointsList) {
	  texturePointsMap.set(point);
	}
	auto index = 0U;
	std::erase_if(tempTexturePoints, [&index, texturePointsMap]([[maybe_unused]] auto point) {
	  return texturePointsMap.test(index++);
	});
	selectedTexturePointsList.clear();
	Instance->StateMap.set(StateFlag::RESTCH);
	return;
  }
  if (auto iClosestPoint = 0U; !tempTexturePoints.empty() && txtclos(iClosestPoint)) {
	auto itPoint = tempTexturePoints.cbegin();
	std::advance(itPoint, iClosestPoint);
	itPoint = tempTexturePoints.erase(itPoint);
	Instance->StateMap.set(StateFlag::RESTCH);
  }
}

void txtlin() {
  auto& formLines = Instance->formLines;
  formLines.clear();
  formLines.resize(2);
  deorg(formLines[0]);
  deorg(formLines[1]);
  Instance->StateMap.reset(StateFlag::TXTCLP);
  Instance->StateMap.set(StateFlag::TXTLIN);
  Instance->StateMap.set(StateFlag::TXTMOV);
}

void txtxfn(POINT const& reference, uint16_t const offsetPixels) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto line = std::array<POINT, 2> {};
  line[0]   = POINT {reference.x, reference.y - offsetPixels};
  line[1]   = POINT {reference.x, reference.y + offsetPixels};
  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  line[0] = POINT {reference.x - offsetPixels, reference.y};
  line[1] = POINT {reference.x + offsetPixels, reference.y};
  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
}

void txvrt(FRM_HEAD& form) {
  if (TextureInstance->TempTexturePoints.empty()) {
	return;
  }
  form.fillType = TXVRTF;
  txpar(form);
}

void altx() {
  auto txtLines = boost::dynamic_bitset(wrap::toSize(TextureScreen.lines) + 1U); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  auto const halfHeight = TextureScreen.areaHeight / 2.0F;
  for (auto const& texturePoint : TextureInstance->TempTexturePoints) {
	txtLines.set(texturePoint.line);
  }
  for (uint16_t iLine = 1; iLine <= TextureScreen.lines; ++iLine) {
	if (!txtLines.test(iLine)) {
	  TextureInstance->TempTexturePoints.push_back(TX_PNT {.y = halfHeight, .line = iLine});
	}
  }
}

void angrct(F_RECTANGLE& rectangle) noexcept {
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;
  for (auto const& vertex : Instance->angledFormVertices) {
	minX = std::min(minX, vertex.x);
	minY = std::min(minY, vertex.y);
	maxX = std::max(maxX, vertex.x);
	maxY = std::max(maxY, vertex.y);
  }
  rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
}

void butsid(uint32_t const windowId) {
  auto buttonRect = RECT {};
  chktxnum();
  TextureWindowId = windowId;
  GetWindowRect(Instance->buttonWin.operator[](windowId), &buttonRect);
  SideWindowButton = CreateWindow(L"STATIC",
                                  nullptr,
                                  SS_NOTIFY | SS_CENTER | WS_CHILD | WS_VISIBLE | WS_BORDER,
                                  buttonRect.left + ButtonWidthX3 - StitchWindowOrigin.x,
                                  buttonRect.top - StitchWindowOrigin.y,
                                  ButtonWidthX3,
                                  ButtonHeight,
                                  MainStitchWin,
                                  nullptr,
                                  ThrEdInstance,
                                  nullptr);
  displayText::updateWinFont(MainStitchWin);
}

auto chkbut() -> bool {
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXCLR)) {
	txdelal();
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXHI)) {
	butsid(HTXHI);
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXWID)) {
	butsid(HTXWID);
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXSPAC)) {
	butsid(HTXSPAC);
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXVRT)) {
	dutxfn(VRTYP);
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXHOR)) {
	dutxfn(HORTYP);
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXANG)) {
	dutxfn(ANGTYP);
	return true;
  }
  if (WinMsg.hwnd == Instance->buttonWin.operator[](HTXMIR)) {
	dutxmir();
	return true;
  }
  return false;
}

auto chktxh(_In_ TX_HIST const& historyItem) noexcept(std::is_same_v<size_t, uint32_t>) -> bool {
  if (historyItem.texturePoints.size() != TextureInstance->TempTexturePoints.size()) {
	return true;
  }
  if (!util::closeEnough(historyItem.height, TextureScreen.areaHeight)) {
	return true;
  }
  if (!util::closeEnough(historyItem.spacing, TextureScreen.spacing)) {
	return true;
  }
  if (!util::closeEnough(historyItem.width, TextureScreen.width)) {
	return true;
  }

  for (auto itHITP = historyItem.texturePoints.cbegin(); auto const& point : TextureInstance->TempTexturePoints) {
	if (point.line != itHITP->line || !util::closeEnough(point.y, itHITP->y)) {
	  return true;
	}
	++itHITP;
  }
  return false;
}

void chktxnum() {
  auto value = 0.0F;
  if (!Instance->textureInputBuffer.empty()) {
	value = std::stof(Instance->textureInputBuffer);
  }
  if (value != 0.0F) {
	Instance->textureInputBuffer.clear();
	value *= PFGRAN;
	switch (TextureWindowId) {
	  case HTXHI: {
		texture::savtxt();
		TextureScreen.areaHeight = value;
		IniFile.textureHeight    = value;
		Instance->StateMap.set(StateFlag::CHKTX);
		break;
	  }
	  case HTXWID: {
		texture::savtxt();
		TextureScreen.width  = value;
		IniFile.textureWidth = value;
		Instance->StateMap.set(StateFlag::CHKTX);
		break;
	  }
	  case HTXSPAC: {
		texture::savtxt();
		TextureScreen.spacing  = value;
		IniFile.textureSpacing = value;
		TextureScreen.width    = value * wrap::toFloat(TextureScreen.lines) + value * HALF;
		Instance->StateMap.set(StateFlag::CHKTX);
		break;
	  }
	  default: {
		outDebugString(L"default hit in chktxnum: TextureWindowId [{}]\n", TextureWindowId);
		break;
	  }
	}
  }
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  Instance->StateMap.set(StateFlag::RESTCH);
}

void deorg(POINT& point) noexcept {
  point = {.x = WinMsg.pt.x - StitchWindowOrigin.x, .y = WinMsg.pt.y - StitchWindowOrigin.y};
}

void doTexAdjust(FRM_HEAD& current, std::vector<TX_PNT>& textureBuffer, uint16_t& iBuffer) {
  textureBuffer.resize(textureBuffer.size() + current.texture.count);
  auto const startSource = wrap::next(Instance->TexturePointsBuffer.cbegin(), current.texture.index);
  auto const endSource   = wrap::next(startSource, current.texture.count);
  auto const destination = wrap::next(textureBuffer.begin(), iBuffer);
  std::copy(startSource, endSource, destination);
  current.texture.index = iBuffer;
  iBuffer += current.texture.count;
}

void dutxfn(uint32_t const textureType) {
  altx();
  if (Instance->StateMap.test(StateFlag::FORMSEL)) {
	txfn(textureType, ClosestFormToCursor);
  }
  else {
	for (auto const& selectedForm : Instance->selectedFormList) {
	  txfn(textureType, selectedForm);
	}
  }
  texture::txof();
  texture::rstxt();
  if (Instance->StateMap.testAndReset(StateFlag::WASFRMFRM)) {
	formForms::refrm();
  }
}

void dutxlin(F_POINT const& point0in, F_POINT const& point1in) {
  auto const point0 = ed2stch(point0in);
  auto const point1 = ed2stch(point1in);
  auto const deltaX = point1.x - point0.x;
  if (fabs(deltaX) < TNYFLOAT) {
	return;
  }
  auto const slope  = (point1.y - point0.y) / deltaX;
  auto       start  = point0.x;
  auto       finish = point1.x;
  if (start > finish) {
	std::swap(start, finish);
  }
  auto integerStart  = wrap::ceil<int32_t>(start / TextureScreen.spacing);
  auto integerFinish = wrap::floor<int32_t>(finish / TextureScreen.spacing);
  if (integerStart > TextureScreen.lines + 1) {
	return;
  }
  if (integerFinish < 0) {
	return;
  }
  integerStart = std::max(integerStart, 1);
  if (integerFinish == 0) {
	++integerFinish;
  }
  if (integerStart == TextureScreen.lines + 1) {
	--integerStart;
  }
  integerFinish        = std::min(integerFinish, gsl::narrow_cast<int32_t>(TextureScreen.lines));
  auto const lineRange = wrap::toSize(integerFinish - integerStart);
  TextureInstance->TempTexturePoints.reserve(TextureInstance->TempTexturePoints.size() + lineRange);
  while (integerStart <= integerFinish) {
	if (auto const yOffset =
	        (slope * (-point0.x + wrap::toFloat(integerStart) * TextureScreen.spacing)) + point0.y;
	    yOffset > 0 && yOffset < TextureScreen.areaHeight) {
	  TextureInstance->TempTexturePoints.push_back(
	      TX_PNT {.y = yOffset, .line = gsl::narrow<uint16_t>(integerStart)});
	}
	++integerStart;
  }
}

} // namespace

void texture::setTxtCurLoc(POINT const location) noexcept {
  TextureCursorLocation = location;
}

void texture::initTextureHistory() {
  TextureInstance->TextureHistory.resize(ITXBUFSZ);
}

void texture::txdun() {
  constexpr auto SIGNATURE            = std::array<char, 4> {"txh"};
  auto           textureHistoryBuffer = std::vector<TX_HIST_BUFF> {};
  textureHistoryBuffer.resize(TextureInstance->TextureHistory.size());
  if (TextureInstance->TextureHistory.operator[](0).texturePoints.empty()) {
	return;
  }
  auto name = std::wstring {};
  if (!txnam(name)) {
	return;
  }
  auto bytesWritten = DWORD {};
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const handle = CreateFile(name.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, nullptr);
  if (handle == INVALID_HANDLE_VALUE) {
	CloseHandle(handle);
	return;
  }
  WriteFile(handle, SIGNATURE.data(), wrap::toUnsigned(SIGNATURE.size()), &bytesWritten, nullptr);
  WriteFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesWritten, nullptr);
  for (auto bufferIter = textureHistoryBuffer.begin(); auto const& historyEntry : TextureInstance->TextureHistory) {
	bufferIter->count   = wrap::toUnsigned(historyEntry.texturePoints.size());
	bufferIter->height  = historyEntry.height;
	bufferIter->width   = historyEntry.width;
	bufferIter->spacing = historyEntry.spacing;
	++bufferIter;
  }
  wrap::writeFile(handle,
                  textureHistoryBuffer.data(),
                  wrap::toUnsigned(textureHistoryBuffer.size() * wrap::sizeofType(textureHistoryBuffer)),
                  &bytesWritten,
                  nullptr);
  for (auto const& item : TextureInstance->TextureHistory) {
	if (item.texturePoints.empty()) {
	  continue;
	}
	wrap::writeFile(handle, item.texturePoints.data(), wrap::sizeofVector(item.texturePoints), &bytesWritten, nullptr);
  }
  CloseHandle(handle);
}

void texture::redtx() {
  auto  name                 = std::wstring {};
  auto  textureHistoryBuffer = std::vector<TX_HIST_BUFF> {};
  auto& textureHistory       = TextureInstance->TextureHistory;
  textureHistoryBuffer.resize(textureHistory.size());
  TextureHistoryIndex = wrap::toUnsigned(textureHistory.size() - 1U);
  while (true) {
	if (!txnam(name)) {
	  break;
	}
	// NOLINTNEXTLINE(readability-qualified-auto)
	auto const handle = CreateFile(name.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (handle == INVALID_HANDLE_VALUE) {
	  CloseHandle(handle);
	  break;
	}
	auto bytesRead = DWORD {};
	auto sig       = std::array<char, 4> {};
	if (!wrap::readFile(handle, sig.data(), sig.size(), &bytesRead, L"ReadFile for sig in redtx")) {
	  break;
	}
	if (strcmp(sig.data(), "txh") != 0) {
	  break;
	}
	if (!wrap::readFile(handle, &TextureHistoryIndex, sizeof(TextureHistoryIndex), &bytesRead, L"ReadFile for TextureHistoryIndex in redtx")) {
	  TextureHistoryIndex = wrap::toUnsigned(textureHistory.size() - 1U);
	  break;
	}
	auto const bytesToRead = textureHistoryBuffer.size() * wrap::sizeofType(textureHistoryBuffer);
	auto       historyBytesRead = DWORD {};
	if (!wrap::readFile(handle, textureHistoryBuffer.data(), bytesToRead, &historyBytesRead, L"ReadFile for textureHistoryBuffer in redtx")) {
	  TextureHistoryIndex = wrap::toUnsigned(textureHistory.size() - 1U);
	  break;
	}
	for (auto texture = textureHistory.begin(); auto const& entry : textureHistoryBuffer) {
	  texture->height  = entry.height;
	  texture->width   = entry.width;
	  texture->spacing = entry.spacing;
	  if (entry.count == 0U) {
		++texture;
		continue;
	  }
	  texture->texturePoints.resize(entry.count);
	  if (auto const bytesToReadTx = wrap::sizeofType(textureHistory.front().texturePoints) * entry.count;
	      !wrap::readFile(handle, texture->texturePoints.data(), bytesToReadTx, &bytesRead, L"ReadFile for texturePoints in redtx")) {
		rollbackTexture(texture);
		break;
	  }
	  ++texture;
	}
	Instance->StateMap.set(StateFlag::WASTXBAK);
	CloseHandle(handle);
	break;
  }
  redtbak();
}

void texture::savtxt() {
  auto& tempTexturePoints = TextureInstance->TempTexturePoints;
  if (tempTexturePoints.empty()) {
	return;
  }
  auto& textureHistory = TextureInstance->TextureHistory;
  if (auto const& currentHistoryItem = textureHistory.operator[](TextureHistoryIndex);
      !chktxh(currentHistoryItem)) {
	return;
  }
  Instance->StateMap.set(StateFlag::WASTXBAK);
  Instance->StateMap.reset(StateFlag::TXBDIR);
  Instance->StateMap.reset(StateFlag::LASTXBAK);
  txrfor();
  auto& currentItem   = textureHistory.operator[](TextureHistoryIndex);
  currentItem.height  = TextureScreen.areaHeight;
  currentItem.width   = TextureScreen.width;
  currentItem.spacing = TextureScreen.spacing;
  currentItem.texturePoints.clear();
  currentItem.texturePoints.insert(
      currentItem.texturePoints.end(), tempTexturePoints.begin(), tempTexturePoints.end());
}

auto texture::dutxtfil() -> bool {
  if (!displayText::filmsgs(FMM_TXTR)) {
	return false;
  }
  if (IniFile.textureHeight == 0.0F) {
	IniFile.textureHeight = ITXHI;
  }
  if (IniFile.textureWidth == 0.0F) {
	IniFile.textureWidth = ITXWID;
  }
  if (IniFile.textureSpacing == 0.0F) {
	IniFile.textureSpacing = ITXSPAC;
  }
  if (IniFile.textureEditorSize == 0U) {
	IniFile.textureEditorSize = ITXPIX;
  }
  Instance->StateMap.set(StateFlag::TXTRED);
  Instance->StateMap.set(StateFlag::ZUMED);
  Instance->StateMap.reset(StateFlag::WASPAT);
  Instance->StateMap.reset(StateFlag::RUNPAT);
  thred::movStch();
  thred::showScrollBars(false);
  TextureInstance->SelectedTexturePointsList.clear();
  Instance->StateMap.set(StateFlag::INIT);
  SideWindowButton = nullptr;
  if (Instance->StateMap.test(StateFlag::WASTXBAK)) {
	redtbak();
	if (TextureScreen.areaHeight == 0.0F) {
	  TextureScreen.areaHeight = IniFile.textureHeight;
	}
	if (TextureScreen.spacing == 0.0F) {
	  TextureScreen.spacing = IniFile.underlaySpacing;
	}
	if (TextureScreen.width == 0.0F) {
	  TextureScreen.width = IniFile.textureWidth;
	}
	Instance->StateMap.set(StateFlag::LASTXBAK);
	Instance->StateMap.reset(StateFlag::TXBDIR);
  }
  else {
	TextureInstance->TempTexturePoints.clear();
	TextureScreen.areaHeight = IniFile.textureHeight;
	TextureScreen.width      = IniFile.textureWidth;
	TextureScreen.spacing    = IniFile.textureSpacing;
  }
  Instance->StateMap.set(StateFlag::RESTCH);
  return true;
}

void texture::drwtxtr() {
  auto       line      = std::array<POINT, LNPNTS> {};
  auto const editSpace = TextureScreen.areaHeight * 2.0F /
                         (TextureScreen.spacing * (wrap::toFloat(TextureScreen.lines) + 2.0F));
  FillRect(StitchWindowMemDC, &StitchWindowClientRect, thred::getBackGroundBrush());
  auto const pixelSpace =
      wrap::toFloat(StitchWindowClientRect.bottom) / wrap::toFloat(StitchWindowClientRect.right);
  TextureScreen.lines   = wrap::floor<uint16_t>(TextureScreen.width / TextureScreen.spacing);
  auto const extraWidth = TextureScreen.spacing * wrap::toFloat(TextureScreen.lines + 2U);
  if (Instance->StateMap.testAndReset(StateFlag::CHKTX)) {
	auto const erased = std::erase_if(TextureInstance->TempTexturePoints, [&](auto const& point) -> bool {
	  return !(point.line <= TextureScreen.lines && point.y <= TextureScreen.areaHeight);
	});
#ifdef _DEBUG
	outDebugString(L"[{}] texture points erased\n", erased);
#else
	// ReSharper disable CppClangTidyClangDiagnosticUnusedValue
	DBG_UNREFERENCED_LOCAL_VARIABLE(erased); // NOLINT(clang-diagnostic-unused-value)
	// ReSharper restore CppClangTidyClangDiagnosticUnusedValue
#endif
  }
  auto const flag = pixelSpace > editSpace;
  TextureScreen.editToPixelRatio =
      flag ? extraWidth / wrap::toFloat(StitchWindowClientRect.bottom)
           : TextureScreen.areaHeight * DBLF / wrap::toFloat(StitchWindowClientRect.bottom);
  constexpr auto BIAS = 2.0F;
  TextureScreen.xOffset =
      flag ? 0.0F
           : (TextureScreen.editToPixelRatio * wrap::toFloat(StitchWindowClientRect.right) -
              TextureScreen.spacing * (wrap::toFloat(TextureScreen.lines) + BIAS)) *
                 HALF;
  auto const yOffset   = flag ? wrap::round<LONG>((wrap::toFloat(StitchWindowClientRect.bottom) -
                                                 TextureScreen.areaHeight / TextureScreen.editToPixelRatio) *
                                                HALF)
                              : StitchWindowClientRect.bottom / 4;
  TextureScreen.top    = yOffset;
  TextureScreen.bottom = StitchWindowClientRect.bottom - yOffset;
  TextureScreen.height = TextureScreen.bottom - TextureScreen.top;
  TextureScreen.halfHeight = StitchWindowClientRect.bottom / 2;
  TextureScreen.screenHeight = wrap::toFloat(StitchWindowClientRect.bottom) * TextureScreen.editToPixelRatio;
  TextureScreen.yOffset = (TextureScreen.screenHeight - TextureScreen.areaHeight) / 2;
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  thred::selectGridPen();
  auto const gridLineCount = wrap::floor<uint32_t>((TextureScreen.areaHeight / IniFile.gridSize) + 1.0F);
  auto textureRecord = TX_PNT {};
  line[0].x          = 0;
  line[1].x          = StitchWindowClientRect.right;
  auto point         = POINT {};
  for (auto iGrid = 0U; iGrid < gridLineCount; ++iGrid) {
	txt2pix(textureRecord, point);
	line[0].y = line[1].y = point.y;
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	textureRecord.y += IniFile.gridSize;
  }
  DeleteObject(TextureCrossPen);
  auto const iUserColor = wrap::next(UserColor.begin(), ActiveColor);
  TextureCrossPen       = wrap::createPen(PS_SOLID, PENNWID, *iUserColor);
  SelectObject(StitchWindowMemDC, TextureCrossPen);
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
  line[0].y = 0;
  line[1].y = StitchWindowClientRect.bottom;
  for (auto iVertical = 1U; iVertical < TextureScreen.lines + 1U; ++iVertical) {
	line[0].x = line[1].x =
	    std::lroundf((TextureScreen.spacing * wrap::toFloat(iVertical) + TextureScreen.xOffset) /
	                 TextureScreen.editToPixelRatio);
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
  line[0].x = 0;
  line[1].x = StitchWindowClientRect.right;
  line[0].y = line[1].y = TextureScreen.top;
  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  line[0].y = line[1].y = TextureScreen.bottom;
  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  DeleteObject(TextureCrossPen);
  TextureCrossPen = wrap::createPen(PS_SOLID, PENNWID, PENWHITE);
  SelectObject(StitchWindowMemDC, TextureCrossPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  auto const end = wrap::toUnsigned(TextureInstance->TempTexturePoints.size());
  for (auto index = uint32_t {0U}; index < end; ++index) {
	dutxtx(index, IniFile.textureEditorSize);
  }
  if (!TextureInstance->SelectedTexturePointsList.empty()) {
	txrct2rct(TextureRect, TexturePixelRect);
	line[0] = {.x = TexturePixelRect.left, .y = TexturePixelRect.top};
	line[1] = {.x = TexturePixelRect.right, .y = TexturePixelRect.top};
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	line[1] = {.x = TexturePixelRect.left, .y = TexturePixelRect.bottom};
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	line[0] = {.x = TexturePixelRect.right, .y = TexturePixelRect.bottom};
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	line[1] = {.x = TexturePixelRect.right, .y = TexturePixelRect.top};
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
  for (auto const& selectedPoint : TextureInstance->SelectedTexturePointsList) {
	dutxtx(selectedPoint, IniFile.textureEditorSize);
	dutxtx(selectedPoint, IniFile.textureEditorSize * 2U);
  }
  BitBlt(StitchWindowDC, 0, 0, StitchWindowClientRect.right, StitchWindowClientRect.bottom, StitchWindowMemDC, 0, 0, SRCCOPY);
  displayText::drwtxbut(TextureScreen);
}

void texture::rstxt() {
  TextureInstance->SelectedTexturePointsList.clear();
  Instance->StateMap.reset(StateFlag::TXTMOV);
  Instance->StateMap.reset(StateFlag::BZUM);
  Instance->StateMap.reset(StateFlag::BZUMIN);
  Instance->StateMap.set(StateFlag::RESTCH);
  Instance->StateMap.reset(StateFlag::POLIMOV);
  DestroyWindow(SideWindowButton);
  SideWindowButton = nullptr;
  Instance->StateMap.set(StateFlag::RESTCH);
}

void texture::txtrbut() {
  if (txbutfn()) {
	savtxt();
	Instance->StateMap.set(StateFlag::RESTCH);
  }
  else {
	rstxt();
  }
}

void texture::txtrup() {
  auto offset = POINT {};
  Instance->StateMap.set(StateFlag::RESTCH);
  auto& tempTexturePoints         = TextureInstance->TempTexturePoints;
  auto& selectedTexturePointsList = TextureInstance->SelectedTexturePointsList;
  if (Instance->StateMap.testAndReset(StateFlag::TXTMOV)) {
	savtxt();
	deorg(offset);
	offset.x -= SelectTexturePointsOrigin.x;
	offset.y -= SelectTexturePointsOrigin.y;
	auto const xMagnitude = abs(offset.x);
	auto       textureOffset =
	    TX_OFF {.y = wrap::toFloat(-offset.y) / wrap::toFloat(TextureScreen.height) * TextureScreen.areaHeight,
	            .line = wrap::ceil<int32_t>(wrap::toFloat(xMagnitude) *
	                                        TextureScreen.editToPixelRatio / TextureScreen.spacing)};
	if (offset.x < 0) {
	  textureOffset.line = -textureOffset.line;
	}
	auto yOffset = TextureRect.top + textureOffset.y - TextureScreen.areaHeight;
	if (yOffset > 0) {
	  textureOffset.y -= yOffset;
	}
	yOffset = TextureRect.bottom + textureOffset.y;
	if (yOffset < 0) {
	  textureOffset.y -= yOffset;
	}
	auto xCoord = TextureRect.left + textureOffset.line - 1;
	if (xCoord < 0) {
	  textureOffset.line -= xCoord;
	}
	xCoord = TextureRect.right + textureOffset.line - TextureScreen.lines;
	if (xCoord > 0) {
	  textureOffset.line -= xCoord;
	}
	for (auto const& point : selectedTexturePointsList) {
	  auto& texturePoint = tempTexturePoints.operator[](point);
	  texturePoint.line  = gsl::narrow<uint16_t>(texturePoint.line + textureOffset.line);
	  texturePoint.y += textureOffset.y;
	}
	dutxrct(TextureRect);
	return;
  }
  if (!Instance->StateMap.testAndReset(StateFlag::BZUMIN)) {
	return;
  }
  auto highestTexturePoint = TX_PNT {};
  auto lowestTexturePoint  = TX_PNT {};
  deorg(offset);
  ed2txp(ZoomBoxLine[0], highestTexturePoint);
  ed2txp(offset, lowestTexturePoint);
  if (highestTexturePoint.line < lowestTexturePoint.line) {
	std::swap(highestTexturePoint.line, lowestTexturePoint.line);
  }
  if (highestTexturePoint.y < lowestTexturePoint.y) {
	std::swap(highestTexturePoint.y, lowestTexturePoint.y);
  }
  selectedTexturePointsList.clear();
  for (auto index = 0U; auto const& iPoint : tempTexturePoints) {
	if (iPoint.y < highestTexturePoint.y && iPoint.y > lowestTexturePoint.y &&
	    iPoint.line <= highestTexturePoint.line && iPoint.line >= lowestTexturePoint.line) {
	  selectedTexturePointsList.push_back(index);
	}
	++index;
  }
  dutxrct(TextureRect);
}

void texture::setxfrm() noexcept {
  auto angleRect = F_RECTANGLE {};
  angrct(angleRect);
  auto& angledFormVertices = Instance->angledFormVertices;
  for (auto& vertex : angledFormVertices) {
	vertex.x -= angleRect.left;
	vertex.y -= angleRect.bottom;
  }
  angrct(angleRect);
  if (auto const height = angleRect.top - angleRect.bottom; height > TextureScreen.areaHeight) {
	for (auto const ratio = TextureScreen.areaHeight / height * 0.95F; auto& vertex : angledFormVertices) {
	  vertex *= ratio;
	}
	angrct(angleRect);
  }
  TextureScreen.formCenter =
      F_POINT {wrap::midl(angleRect.right, angleRect.left), wrap::midl(angleRect.top, angleRect.bottom)};
  ed2px(TextureScreen.formCenter, SelectTexturePointsOrigin);
}

void texture::txtrmov(FRM_HEAD const& textureForm) {
  if (Instance->StateMap.test(StateFlag::TXTLIN)) {
	dutxtlin();
	deorg(Instance->formLines.operator[](1));
	dutxtlin();
	return;
  }
  if (Instance->StateMap.test(StateFlag::TXTCLP)) {
	if (Instance->StateMap.testAndSet(StateFlag::WASWROT)) {
	  ritxfrm(textureForm);
	}
	TextureCursorLocation = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
	                         .y = WinMsg.pt.y - StitchWindowOrigin.y};
	ritxfrm(textureForm);
	return;
  }
  if (TextureInstance->SelectedTexturePointsList.empty()) {
	return;
  }
  ritxrct();
  TextureCursorLocation = {.x = WinMsg.pt.x - StitchWindowOrigin.x,
                           .y = WinMsg.pt.y - StitchWindowOrigin.y};
  ritxrct();
}

void texture::deltx(uint32_t const formIndex) {
  auto& formList = Instance->FormList;

  auto const itForm = wrap::next(formList.begin(), formIndex);
  auto const itNext = std::next(itForm);
  if (Instance->TexturePointsBuffer.empty() || !itForm->isTexture() || itForm->texture.count == 0U) {
	return;
  }
  auto const& currentIndex = itForm->texture.index;
  // First check to see if the texture is shared between forms
  // check forms before and after current
  auto const flagShared =
      std::any_of(formList.begin(),
                  itForm,
                  [currentIndex](const auto& current) {
	                return current.isTexture() && current.texture.index == currentIndex;
                  }) ||
      std::any_of(std::next(itForm), formList.end(), [currentIndex](const auto& current) {
	    return current.isTexture() && current.texture.index == currentIndex;
      });
  // clear the texture info from the form
  itForm->texture = TXTR_INFO {};
  // if it is shared, do not delete texture
  if (flagShared) {
	return;
  }
  auto textureBuffer = std::vector<TX_PNT> {};
  textureBuffer.reserve(Instance->TexturePointsBuffer.size());
  auto iBuffer = uint16_t {};
  // adjust forms before the current form
  for (auto spForms = std::ranges::subrange(formList.begin(), itForm); auto& current : spForms) {
	if (!current.isTexture()) {
	  continue;
	}
	doTexAdjust(current, textureBuffer, iBuffer);
  }
  // adjust forms after the current form
  for (auto spForms = std::ranges::subrange(itNext, formList.end()); auto& current : spForms) {
	if (!current.isTexture()) {
	  continue;
	}
	doTexAdjust(current, textureBuffer, iBuffer);
  }
  Instance->TexturePointsBuffer = std::move(textureBuffer);
}

void texture::txof() {
  displayText::butxt(HBOXSEL, displayText::loadStr(IDS_BOXSEL));
  thred::redraw(Instance->buttonWin.operator[](HHID));
  if (Instance->StateMap.test(StateFlag::UPTO)) {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPON));
  }
  else {
	displayText::butxt(HUPTO, displayText::loadStr(IDS_UPOF));
  }
  SetWindowText(Instance->buttonWin.operator[](HTXSPAC), L"");
  savtxt();
  thred::zumhom();
  TextureInstance->SelectedTexturePointsList.clear();
  TextureInstance->SelectedTexturePointsList.shrink_to_fit();
  Instance->StateMap.reset(StateFlag::TXTRED);
}

void texture::txtlbut(FRM_HEAD const& textureForm) {
  if (chkbut()) {
	return;
  }
  if (Instance->StateMap.testAndReset(StateFlag::TXTCLP)) {
	savtxt();
	setxclp(textureForm);
	return;
  }
  if (Instance->StateMap.testAndReset(StateFlag::TXTLIN)) {
	savtxt();
	stxlin();
	return;
  }
  auto& selectedTexturePointsList = TextureInstance->SelectedTexturePointsList;
  if (!selectedTexturePointsList.empty()) {
	auto point = POINT {};
	deorg(point);
	if (point.x > TexturePixelRect.left && point.x < TexturePixelRect.right &&
	    point.y > TexturePixelRect.top && point.y < TexturePixelRect.bottom) {
	  setxmov();
	  ritxrct();
	  return;
	}
	if (txtclos(selectedTexturePointsList.front())) {
	  selectedTexturePointsList.resize(1);
	  setxmov();
	  dutxrct(TextureRect);
	  Instance->StateMap.set(StateFlag::RESTCH);
	  return;
	}
  }
  selectedTexturePointsList.clear();
  Instance->StateMap.set(StateFlag::BZUMIN);
  Instance->StateMap.reset(StateFlag::BZUM);
  ZoomBoxLine[0].x = ZoomBoxLine[3].x = ZoomBoxLine[4].x = WinMsg.pt.x - StitchWindowOrigin.x;
  ZoomBoxLine[0].y = ZoomBoxLine[1].y = WinMsg.pt.y - StitchWindowOrigin.y;
  ZoomBoxLine[4].y                    = ZoomBoxLine[0].y - 1L;
}

void texture::txsnap() {
  auto&       tempTexturePoints         = TextureInstance->TempTexturePoints;
  auto const& selectedTexturePointsList = TextureInstance->SelectedTexturePointsList;
  if (tempTexturePoints.empty()) {
	return;
  }
  savtxt();
  auto const halfGrid = IniFile.gridSize / 2;
  if (!selectedTexturePointsList.empty()) {
	for (auto const& iPoint : selectedTexturePointsList) {
	  auto&      texturePoint = tempTexturePoints.operator[](iPoint);
	  auto const yStep        = std::floor((texturePoint.y + halfGrid) / IniFile.gridSize);
	  texturePoint.y          = yStep * IniFile.gridSize;
	}
  }
  else {
	for (auto& texturePoint : tempTexturePoints) {
	  auto const yStep = std::floor((texturePoint.y + halfGrid) / IniFile.gridSize);
	  texturePoint.y   = yStep * IniFile.gridSize;
	}
  }
  dutxrct(TextureRect);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void texture::txtkey(wchar_t const keyCode, FRM_HEAD& textureForm) {
  if (SideWindowButton != nullptr) {
	auto flag = true;
	switch (keyCode) {
	  case VK_RETURN: {
		chktxnum();
		return;
	  }
	  case VK_ESCAPE:
	  case 'Q': {
		rstxt();
		Instance->StateMap.set(StateFlag::RESTCH);
		break;
	  }
	  case VK_BACK: // backspace
	  {
		if (!Instance->textureInputBuffer.empty()) {
		  Instance->textureInputBuffer.pop_back();
		}
		flag = false;
		break;
	  }
	  default: {
		outDebugString(L"default hit in txtkey 1: keyCode [{}]\n", keyCode);
		break;
	  }
	}
	if (flag) {
	  auto character = wchar_t {};
	  if (constexpr auto BUFFLEN = 8U; Instance->textureInputBuffer.size() <
	                                   BUFFLEN) { // i.e. floating point 7 digits of precision + '.'
		if (txdig(keyCode, character)) {
		  Instance->textureInputBuffer.push_back(character);
		}
	  }
	}
	SetWindowText(SideWindowButton, Instance->textureInputBuffer.c_str());
	return;
  }
  switch (keyCode) {
	case VK_ESCAPE: {
	  txof();
	  [[fallthrough]];
	}
	case 'Q': {
	  rstxt();
	  Instance->StateMap.set(StateFlag::RESTCH);
	  break;
	}
	case VK_OEM_4: { //  '[{' for US
	  txshrnk(textureForm);
	  break;
	}
	case VK_OEM_6: { //  ']}' for US
	  txgro(textureForm);
	  break;
	}
	case VK_OEM_3: { // '`~' for US
	  thred::tst();
	  break;
	}
	case 'R': {
	  dutxfn(VRTYP);
	  break;
	}
	case 'A': {
	  dutxfn(ANGTYP);
	  break;
	}
	case 'H': {
	  dutxfn(HORTYP);
	  break;
	}
	case 'E': {
	  txtlin();
	  break;
	}
	case 'Z':
	case 'B': {
	  if (!Instance->StateMap.testAndSet(StateFlag::LASTXBAK)) {
		savtxt();
		txrbak();
	  }
	  else {
		if (Instance->StateMap.testAndReset(StateFlag::TXBDIR)) {
		  txrbak();
		}
	  }
	  txbak();
	  return;
	}
	case 'V': {
	  if (OpenClipboard(ThrEdWindow) != 0) {
		tfc::txtclp(textureForm);
	  }
	  break;
	}
	case 'N': {
	  Instance->StateMap.set(StateFlag::LASTXBAK);
	  if (!Instance->StateMap.testAndSet(StateFlag::TXBDIR)) {
		txrfor();
	  }
	  nxbak();
	  return;
	}
	case 'D':
	case VK_DELETE: {
	  if (wrap::pressed(VK_SHIFT) && wrap::pressed(VK_CONTROL)) {
		txdelal();
	  }
	  else {
		txtdel();
	  }
	  break;
	}
	case VK_OEM_MINUS: { // '-' any country
	  txcntrv(textureForm);
	  break;
	}
	case VK_LEFT: {
	  txnudg(-1, 0);
	  break;
	}
	case VK_RIGHT: {
	  txnudg(1, 0);
	  break;
	}
	case VK_UP: {
	  txnudg(0, IniFile.cursorNudgeStep);
	  break;
	}
	case VK_DOWN: {
	  txnudg(0, -IniFile.cursorNudgeStep);
	  break;
	}
	case 'S': {
	  txsnap();
	  break;
	}
	default: {
	  outDebugString(L"default hit in txtkey 2: keyCode [{}]\n", keyCode);
	  break;
	}
  }
  Instance->StateMap.reset(StateFlag::LASTXBAK);
}

void texture::setxt(FRM_HEAD& form, std::vector<RNG_COUNT>& textureSegments) {
  auto const currentIndex = form.texture.index;
  auto const currentCount = form.texture.count;
  savtxt();
  form.wordParam = 0;
  Instance->StateMap.set(StateFlag::TXFIL);
  ClipRectSize.cx = form.fillSpacing;
  ClipRectSize.cy = form.texture.height;
  if (currentCount == 0U) {
	return;
  }
  for (auto iTexturePoint = currentCount - 1; iTexturePoint >= 0; --iTexturePoint) {
	if (auto const& currentPoint = Instance->TexturePointsBuffer.operator[](
	        wrap::toSize(currentIndex) + wrap::toSize(iTexturePoint));
	    currentPoint.line != 0U) {
	  auto const iSegment            = currentPoint.line - 1U;
	  textureSegments[iSegment].line = iTexturePoint;
	  ++textureSegments[iSegment].stitchCount;
	}
  }
}

void texture::rtrtx(FRM_HEAD const& form) {
  auto& tempTexturePoints = TextureInstance->TempTexturePoints;
  tempTexturePoints.clear();
  auto const currentIndex = form.texture.index;
  if (wrap::toUnsigned(Instance->TexturePointsBuffer.size()) < gsl::narrow_cast<uint32_t>(currentIndex)) {
	return;
  }
  auto currentCount = form.texture.count;
  if (wrap::toUnsigned(Instance->TexturePointsBuffer.size()) <
      gsl::narrow_cast<uint32_t>(currentIndex + currentCount)) {
	currentCount = gsl::narrow<uint16_t>(Instance->TexturePointsBuffer.size()) - currentIndex;
  }
  tempTexturePoints.resize(currentCount);
  auto const startSource = std::next(Instance->TexturePointsBuffer.cbegin(), currentIndex);
  auto const endSource   = std::next(startSource, currentCount);
  std::copy(startSource, endSource, tempTexturePoints.begin());
  TextureScreen.areaHeight = form.texture.height;
  TextureScreen.spacing    = form.fillSpacing;
  TextureScreen.lines      = gsl::narrow<uint16_t>(form.texture.lines);
  TextureScreen.width =
      wrap::toFloat(TextureScreen.lines) * TextureScreen.spacing + TextureScreen.spacing * HALF;
  savtxt();
}

void texture::setshft() {
  if (ZoomBoxLine[2].x == 0L || ZoomBoxLine[2].y == 0L) {
	return;
  }
  savtxt();
  thred::unbBox();
  Instance->StateMap.reset(StateFlag::BZUMIN);
  auto const stitchPoint = thred::pxCor2stch(
      POINT {ZoomBoxLine[2].x + StitchWindowOrigin.x, ZoomBoxLine[2].y + StitchWindowOrigin.y});
  auto const selectionRect =
      F_RECTANGLE {ZoomBoxOrigin.x, ZoomBoxOrigin.y, stitchPoint.x, stitchPoint.y};
  Instance->StateMap.reset(StateFlag::TXIN);
  auto& tempTexturePoints = TextureInstance->TempTexturePoints;
  tempTexturePoints.clear();
  auto line = 1U;
  auto txIn = false;
  for (auto const& stitch : Instance->StitchBuffer) {
	if (inrct(selectionRect, stitch)) {
	  txIn = true;
	  tempTexturePoints.push_back(
	      TX_PNT {.y = (stitch.y - selectionRect.bottom), .line = gsl::narrow<uint16_t>(line)});
	  continue;
	}
	if (txIn) {
	  txIn = false;
	  ++line;
	}
  }
  if (!tempTexturePoints.empty()) {
	line = tempTexturePoints.back().line;
  }
  if (IniFile.textureEditorSize == 0U) {
	IniFile.textureEditorSize = ITXPIX;
  }
  TextureScreen.spacing    = (selectionRect.right - selectionRect.left) / wrap::toFloat(line);
  TextureScreen.areaHeight = selectionRect.top - selectionRect.bottom;
  TextureScreen.width = TextureScreen.spacing * wrap::toFloat(line) + TextureScreen.spacing * HALF;
  Instance->StateMap.set(StateFlag::TXTRED);
  Instance->StateMap.set(StateFlag::RESTCH);
}

void texture::writeScreenWidth(int32_t const position) {
  auto const scrWidth = displayText::format(IDS_TXWID, TextureScreen.width * IPFGRAN);
  wrap::textOut(DrawItem->hDC, position, 1, scrWidth.c_str(), wrap::toUnsigned(scrWidth.size()));
}

void texture::textureInit() noexcept {
  TextureInstance = TXT_SINGLE::getInstance();
}
