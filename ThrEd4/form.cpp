// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formClip.h"
#include "formForms.h"
#include "formHeader.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "menu.h"
#include "point.h"
#include "Resources/resource.h"
#include "satCon.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "ThrEdTypes.h"
// ReSharper disable CppUnusedIncludeDirective
#include "warnings.h"
// ReSharper restore CppUnusedIncludeDirective
#include "wrappers.h"
#include "xt.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include "boost/dynamic_bitset/dynamic_bitset.hpp"
#include "boost/range/algorithm_ext/iota.hpp"
#include "gsl/narrow"
#include "gsl/pointers"
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
#include <minwindef.h>
#include <windef.h>
#include <wingdi.h>
#include <winnt.h>
#include <WinUser.h>

// Standard Libraries
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

class CLIP_SEG
{
  public:
  uint32_t start {};
  float    beginLength {};
  uint32_t beginIndex {};
  uint32_t asid {};
  uint32_t finish {};
  float    endLength {};
  uint32_t endIndex {};
  uint32_t zsid {};
  int8_t   dun {};
};

class CLIP_SORT
{
  public:
  float    segmentLength {};
  float    sideLength {};
  uint32_t vertexIndex {};
  F_POINT  point;
};

class CLIP_PNT
{
  public:
  float    x {};
  float    y {};
  uint32_t vertexIndex {};
  uint32_t flag {};
};

class F_POINT_LINE
{
  public:
  float    x {};
  float    y {};
  uint16_t line {};
};

class F_SEQ // Forward SequencePath: path of sequenced regions
{
  public:
  uint16_t node {};
  uint16_t nextGroup {};
  bool     skp {}; // path not found
};

class LEN_INFO // Sorted line length info
{
  public:
  uint32_t index {};
  bool     isEnd {};
  float    length {};
};

class R_CON // PathMap: path map for sequencing
{
  public:
  uint32_t node {};
  bool     isConnected {};
  uint32_t nextGroup {};
};

class REGION // region for sequencing vertical fills
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  uint32_t start {}; // start line of region
  uint32_t end {};   // end line of region
  uint32_t breakCount {};
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  inline REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsCount) noexcept;
};

inline REGION::REGION(uint32_t const rhsStart, uint32_t const rhsEnd, uint32_t const rhsCount) noexcept
    : start(rhsStart), end(rhsEnd), breakCount(rhsCount) {
}

class RG_SEQ // TempPath: temporary path connections
{
  public:
  uint32_t pcon {}; // pointer to PathMap entry
  int32_t  count {};
  bool     skp {}; // path not found
};

class V_CLP_X
{
  public:
  uint32_t segment {};
  uint32_t vertex {};
};

class V_RECT_2
{
  public:
  F_POINT aipnt;
  F_POINT aopnt;
  F_POINT bipnt;
  F_POINT bopnt;
  F_POINT cipnt;
  F_POINT copnt;
  F_POINT dipnt;
  F_POINT dopnt;
};

namespace {
constexpr auto BHWIDTH  = 20.0F; // Button hole width
constexpr auto CLPMIN   = 0.5F;  // if clipboard data width less than this, then don't fill
constexpr auto CLPMINVT = 1.2F;  // Minimum clip width for skinny vertical clips
constexpr auto FRECONT  = 0x80U; // 1000 0000 contour refil
constexpr auto NFRECONT = 0x7fU; // 0111 1111 not contour refil
constexpr auto OUTL_ALL = false; // When selecting multiple forms, should we outline every form?
constexpr auto SEQBOT   = int32_t {3};
constexpr auto SEQTOP   = int32_t {2};

auto AllItemsRect       = F_RECTANGLE {};                  // bounding rectangle for all items
auto ButtonholeCornerLength = IBFCLEN;                         // buttonhole corner length
auto FormForInsert      = static_cast<FRM_HEAD*>(nullptr); // insert form vertex in this form
auto FormSelectedPen    = HPEN {};                         // form select pen
auto FormVertexNext     = uint32_t {}; // form vertex storage for form vertex insert
auto FormVertexPrev     = uint32_t {}; // form vertex storage for form vertex insert
auto LayerPen           = std::array<HPEN, LAYERMAX> {}; //
auto MaxStitchLen       = float {};                      // maximum stitch length
auto NewFormVertexCount = uint32_t {};                   // points in the new form

// Definitions
void adfrm(uint32_t iForm);
void angout(FRM_HEAD& angledForm) noexcept(!std::is_same_v<ptrdiff_t, int>);
void apbrd(FRM_HEAD const& form);
void bakseq();
void bdrlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float stitchSize);
auto bean(uint32_t start, uint32_t finish) -> uint32_t;
void bhbrd(FRM_HEAD const& form);
void bhcrnr(FRM_HEAD const& form, uint32_t vertex);
void bhfn(FRM_HEAD const& form, uint32_t start, uint32_t finish);
void bholbrd(uint32_t formIndex);
void blbrd(FRM_HEAD const& form);
void bold(FRM_HEAD const& form);
void boldlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float size);
void brdfil(FRM_HEAD const& form);
void brkdun(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t                       start,
            uint32_t                       finish,
            std::vector<F_POINT>&          workingFormVertices);
void brkseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t                       start,
            uint32_t                       finish,
            boost::dynamic_bitset<>&       sequenceMap,
            uint32_t&                      lastGroup,
            uint32_t&                      sequenceIndex);
void chan(uint32_t formIndex);
void chkbrd(FRM_HEAD const& form);
void chksid(FRM_HEAD const& form, uint32_t vertexIndex, uint32_t clipIntersectSide, std::vector<F_POINT> const& currentFormVertices);
auto clipComp(gsl::not_null<CLIP_SORT const*> arg1, gsl::not_null<CLIP_SORT const*> arg2) noexcept -> bool;
auto closat(IntersectionStyles& inOutFlag) -> bool;
void clpcon(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& currentFormVertices);
void clpfm();
auto clpnseg(std::vector<CLIP_PNT>&      clipStitchPoints,
             std::vector<CLIP_SEG>&      clipSegments,
             std::vector<float> const&   lengths,
             uint32_t                    start,
             uint32_t                    finish,
             std::vector<F_POINT> const& currentFormVertices) -> uint32_t;
auto clpnxt(std::vector<CLIP_SEG> const& clipSegments,
            std::vector<LEN_INFO> const& sortedLengths,
            uint32_t                     sind,
            uint32_t&                    outIndex) -> bool;
auto compLines(std::tuple<uint32_t, SMAL_PNT_L*> const& arg3,
               std::tuple<uint32_t, SMAL_PNT_L*> const& arg4) noexcept -> bool;
void contf(FRM_HEAD& form);
auto contsf(uint32_t formIndex) -> bool;
void cplayfn(uint32_t iForm, uint32_t layer);
void doTimeWindow(float rangeX, std::vector<uint32_t> const& xPoints, std::vector<uint32_t> const& xHistogram);
void drawGuides(FRM_HEAD const& form);
void drawFormBox(const FRM_HEAD& form);
void drawFormsBox();
void drawStartGuide(FRM_HEAD const& form,
                    uint8_t         layer,
                    unsigned int&   lastPoint) noexcept(!std::is_same_v<ptrdiff_t, int>);

constexpr auto duat(uint32_t attribute) -> uint32_t;

void dubfn(FRM_HEAD const& currentForm);
void ducon() noexcept;
void dufcntr(F_POINT& center) noexcept;
void dufdat(std::vector<F_POINT>&  tempClipPoints,
            std::vector<SAT_CON>&  tempGuides,
            std::vector<F_POINT>&  destFormVertices,
            std::vector<FRM_HEAD>& destFormList,
            uint32_t               formIndex,
            uint32_t&              formRelocationIndex,
            uint32_t&              formSourceIndex);
void duflt(float& formOffset, std::vector<F_POINT>& currentFormVertices);
void dunseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t                       start,
            uint32_t                       finish,
            uint32_t&                      lastGroup);
void duprot(float rotationAngle);
void duprots(float rotationAngle, F_POINT const& rotationCenter);
void duprotfs(float rotationAngle);
void durgn(FRM_HEAD const&                form,
           std::vector<F_SEQ> const&      sequencePath,
           boost::dynamic_bitset<>&       visitedRegions,
           std::vector<SMAL_PNT_L> const& lineEndpoints,
           std::vector<uint32_t> const&   sortedLineIndices,
           uint32_t                       pthi,
           uint32_t                       lineCount,
           std::vector<REGION> const&     regionsList,
           uint32_t&                      lastGroup,
           uint32_t                       sequencePathIndex,
           std::vector<F_POINT>&          workingFormVertices);
void duromb(F_POINT const& start0, F_POINT const& finish0, F_POINT const& start1, F_POINT const& finish1, F_POINT& stitchPoint);
void duseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
           std::vector<uint32_t> const&   sortedLineIndices,
           uint32_t                       start,
           uint32_t                       finish,
           boost::dynamic_bitset<>&       sequenceMap,
           uint32_t&                      lastGroup,
           uint32_t&                      sequenceIndex);
void duseq1(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t                       sequenceIndex);
void duspnd(float                        stitchLen,
            std::vector<V_RECT_2> const& underlayVerticalRect,
            std::vector<V_RECT_2> const& fillVerticalRect,
            uint32_t                     start,
            uint32_t                     finish,
            float                        width,
            F_POINT&                     stitchPoint);
void fillSB(const F_POINT& pivot, float angle, float const& radius, F_POINT& stitchPoint, float const& level);
void filsclp();
void filsfn(uint32_t formIndex);
auto findDistanceToSide(F_POINT const& lineStart, F_POINT const& lineEnd, F_POINT const& point, float& distance) noexcept -> float;
void fmclp(FRM_HEAD& form);
void fnagain(float rotationAngle);
void fnang(std::vector<uint32_t>&   groupIndexSequence,
           std::vector<SMAL_PNT_L>& lineEndpoints,
           float                    rotationAngle,
           F_POINT&                 rotationCenter,
           FRM_HEAD&                angledForm,
           std::vector<F_POINT>&    angledFormVertices);
void fnhor(std::vector<uint32_t>&   groupIndexSequence,
           std::vector<SMAL_PNT_L>& lineEndpoints,
           float                    rotationAngle,
           F_POINT&                 rotationCenter,
           FRM_HEAD&                angledForm,
           std::vector<F_POINT>&    angledFormVertices);
void fnord();
void fnvrt(std::vector<F_POINT>&    currentFillVertices,
           std::vector<uint32_t>&   groupIndexSequence,
           std::vector<SMAL_PNT_L>& lineEndpoints);
auto fplComp(F_POINT_LINE const& point1, F_POINT_LINE const& point2) noexcept -> bool;
void frmToLine(FRM_HEAD const& form);
void frmpnts(uint32_t type);
void frmpoly(gsl::span<POINT> const& source) noexcept;
void frmsqr(uint32_t vertexIndex, uint32_t iVertex);
void frmsqr0(POINT const& controlPoint);
void frmx(POINT const& controlPoint, HDC hDC) noexcept(std::is_same_v<size_t, uint32_t>);
void fsangl(FRM_HEAD& form);
void fsclp(uint32_t formIndex);
void fsclpx(uint32_t formIndex);
void fshor(FRM_HEAD& form);
void fspic(uint32_t formIndex);
void fsvrt();
auto getbig(std::vector<FRM_HEAD> const&     formList,
            std::vector<F_POINT_ATTR> const& stitchBuffer) noexcept -> F_RECTANGLE;
auto getlen(std::vector<CLIP_PNT>&    clipStitchPoints,
            std::vector<float> const& lengths,
            uint32_t                  iPoint,
            std::vector<F_POINT> const& currentFormVertices) noexcept(!std::is_same_v<ptrdiff_t, int>) -> float;
auto getLayerPen(uint32_t layer) noexcept(!std::is_same_v<ptrdiff_t, int>) -> HPEN;
auto getLineSortOrder(std::vector<SMAL_PNT_L>& lineEndpoints) -> std::vector<uint32_t>;
auto handleSeq(size_t iSequence, B_SEQ_PNT& bCurrent) -> bool;
void handleSeqBot(size_t iSequence, B_SEQ_PNT const& bCurrent);
void handleSeqTop(size_t iSequence, B_SEQ_PNT const& bCurrent);
void horclpfn(std::vector<RNG_COUNT> const& textureSegments, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices);
auto insect(FRM_HEAD const&             form,
            std::vector<CLIP_SORT>&     clipIntersectData,
            std::vector<V_CLP_X> const& regionCrossingData,
            std::vector<CLIP_SORT*>&    arrayOfClipIntersectData,
            uint32_t                    regionCrossingStart,
            uint32_t                    regionCrossingEnd,
            F_POINT const&              lineSegmentStart,
            F_POINT const&              lineSegmentEnd,
            std::vector<F_POINT> const& currentFormVertices) -> uint32_t;
void inspnt(std::vector<CLIP_PNT>& clipStitchPoints);
auto isclos(std::vector<SMAL_PNT_L> const& lineEndpoints,
            uint32_t                       line0Index,
            uint32_t                       line1Index,
            float                          gapToClosestRegion) noexcept -> bool;
auto isect(uint32_t       vertex0,
           uint32_t       vertex1,
           F_POINT&       intersection,
           float&         length,
           F_POINT const& lineSegmentStart,
           F_POINT const& lineSegmentEnd,
           std::vector<F_POINT> const& currentFormVertices) noexcept(!std::is_same_v<size_t, uint32_t>) -> bool;
auto isin(FRM_HEAD const&             form,
          std::vector<V_CLP_X> const& regionCrossingData,
          float                       xCoordinate,
          float                       yCoordinate,
          uint32_t                    regionCrossingStart,
          uint32_t                    regionCrossingEnd,
          F_RECTANGLE const&          boundingRect,
          std::vector<F_POINT> const& currentFormVertices) noexcept(!std::is_same_v<size_t, uint32_t>) -> bool;
void lapbrd(FRM_HEAD const& form);
void lcon(FRM_HEAD const&              form,
          std::vector<uint32_t> const& groupIndexSequence,
          std::vector<SMAL_PNT_L>&     lineEndpoints,
          std::vector<F_POINT>&        workingFormVertices);
auto leftsid(std::vector<F_POINT> const& currentFormVertices) noexcept -> uint32_t;
auto lenComp(LEN_INFO const& arg1, LEN_INFO const& arg2) noexcept -> bool;
auto lnclos(std::vector<uint32_t> const&   groupIndexSequence,
            std::vector<SMAL_PNT_L> const& lineEndpoints,
            uint32_t                       group0,
            uint32_t                       line0,
            uint32_t                       group1,
            uint32_t                       line1,
            float gapToClosestRegion) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool;
void makpoli();
void movseq(std::vector<SMAL_PNT_L> const& lineEndpoints, std::vector<uint32_t> const& sortedLineIndices, uint32_t ind);
void mvpclp(std::vector<CLIP_SORT*>& arrayOfClipIntersectData, uint32_t destination, uint32_t source) noexcept;
auto notdun(std::vector<RG_SEQ>&           tempPath,
            std::vector<R_CON> const&      pathMap,
            std::vector<uint32_t> const&   mapIndexSequence,
            boost::dynamic_bitset<> const& visitedRegions,
            ptrdiff_t                      pathLength,
            uint32_t                       doneRegion,
            uint32_t                       sequencePathIndex) -> bool;
auto nucseg(std::vector<CLIP_SEG> const& clipSegments,
            std::vector<LEN_INFO> const& sortedLengths,
            uint32_t&                    currentSegmentIndex) -> bool;
void nufpnt(uint32_t vertex, FRM_HEAD& formForInsert, F_POINT stitchPoint);
void nxtrgn(std::vector<RG_SEQ>&           tempPath,
            std::vector<R_CON> const&      pathMap,
            std::vector<uint32_t> const&   mapIndexSequence,
            boost::dynamic_bitset<>&       visitedRegions,
            std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            std::vector<REGION> const&     regionsList,
            uint32_t&                      doneRegion,
            uint32_t                       pathMapIndex,
            uint32_t&                      sequencePathIndex,
            int32_t                        visitedIndex);
void nxtseq(std::vector<F_SEQ>&          sequencePath,
            std::vector<R_CON> const&    pathMap,
            std::vector<uint32_t> const& mapIndexSequence,
            uint32_t                     pathIndex);
void pbrd(FRM_HEAD const& form);
void pfn(std::vector<V_RECT_2> const& underlayVerticalRect,
         std::vector<V_RECT_2> const& fillVerticalRect,
         uint32_t                     startVertex,
         std::vector<V_RECT_2> const& vrct,
         float                        width);
void plbak(uint32_t backPoint) noexcept(std::is_same_v<size_t, uint32_t>);
void plbrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices);
void plfn(FRM_HEAD const&              form,
          std::vector<V_RECT_2> const& underlayVerticalRect,
          std::vector<V_RECT_2> const& fillVerticalRect,
          std::vector<V_RECT_2> const& prct,
          float                        width,
          F_POINT&                     stitchPoint);
void prebrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices);
auto proj(F_POINT const& point, float slope, F_POINT const& point0, F_POINT const& point1, F_POINT& intersectionPoint) noexcept -> bool;
auto projh(float yCoordinate, F_POINT const& point0, F_POINT const& point1, F_POINT& intersection) noexcept -> bool;
auto projv(float xCoordinate, F_POINT const& lowerPoint, F_POINT const& upperPoint, F_POINT& intersection) noexcept -> bool;
void prpsbrd(uint32_t formIndex);
void prsmal(float width);
auto px2stchf(POINT const& screen) noexcept -> F_POINT;
auto regclos(std::vector<uint32_t> const&   groupIndexSequence,
             std::vector<SMAL_PNT_L> const& lineEndpoints,
             std::vector<uint32_t> const&   sortedLineIndices,
             uint32_t                       iRegion0,
             uint32_t                       iRegion1,
             std::vector<REGION> const&     regionsList,
             float                          gapToClosestRegion,
             uint32_t& nextGroup) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool;
auto reglen(std::vector<SMAL_PNT_L> const&       lineEndpoints,
            std::vector<uint32_t> const&         sortedLineIndices,
            uint32_t                             iRegion,
            std::array<F_POINT, SQRCORNS> const& lastRegionCorners,
            std::vector<REGION> const&           regionsList) noexcept -> float;
void resizeBigBox(F_POINT const& stitchReference, F_POINT const& ratio);
void resizeForm(F_POINT const& reference, F_POINT const& ratio);
void resizeFormPoints(F_POINT const& stitchReference, F_POINT const& ratio);
void resizeSelectedForms(F_POINT const& stitchReference, F_POINT const& ratio);
void resizeStitches(F_POINT const& reference, F_POINT const& ratio);
void ritapbrd();
void ritbrd(FRM_HEAD const& form);
void ritfil(FRM_HEAD& form);
auto ritlin(F_POINT const& start, F_POINT const& finish, float userStitchLen) -> bool;
void ritseg(FRM_HEAD const&              form,
            std::vector<CLIP_PNT> const& clipStitchPoints,
            std::vector<CLIP_SEG>&       clipSegments,
            uint32_t                     currentSegmentIndex,
            uint32_t&                    clipIntersectSide,
            std::vector<F_POINT> const&  currentFormVertices);
void rotbak(float rotationAngle, F_POINT const& rotationCenter) noexcept;
void rotentr(float rotationAngle);
void sapliq(uint32_t formIndex);
void sbold(uint32_t formIndex);
void sbord(uint32_t formIndex);
void setSize(F_POINT& size1, float xyRatio) noexcept;
void seteg();
void sethart();
void setlens();
void setlin();
void setpoli();
void setrpoli();
void setspir();
void setstar();
void setzig();

constexpr auto shreg(float highValue, float reference, float eggRatio) noexcept -> float;

void shrnks();
void snp(uint32_t start, uint32_t finish);
void snpfn(std::vector<uint32_t> const& xPoints, uint32_t start, uint32_t end, uint32_t finish) noexcept;
void spend(std::vector<V_RECT_2> const& fillVerticalRect, uint32_t start, uint32_t finish, F_POINT& stitchPoint);
auto spltlin() -> bool;
void sprct(std::vector<F_POINT> const& vertices,
           uint32_t                    vertexIndex,
           std::vector<V_RECT_2>&      fillVerticalRect,
           uint32_t                    start,
           uint32_t                    finish,
           std::vector<F_POINT> const& outsidePoints) noexcept(!std::is_same_v<uint32_t, size_t>);
void spurct(std::vector<V_RECT_2>&       underlayVerticalRect,
            std::vector<V_RECT_2> const& fillVerticalRect,
            uint32_t                     iRect) noexcept;
void spurfn(F_POINT const& innerPoint, F_POINT const& outerPoint, F_POINT& underlayInnerPoint, F_POINT& underlayOuterPoint) noexcept;
void srtf(std::vector<F_POINT_ATTR> const& tempStitchBuffer, uint32_t start, uint32_t finish);
void stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept;
void swEdgeType(FRM_HEAD const& form, FRM_HEAD& angledForm);
void swPolyFillType(FRM_HEAD& form, FRM_HEAD& angledForm, std::vector<RNG_COUNT>& textureSegments);
void swSatFillType(FRM_HEAD& form);
void trfrm(F_POINT const& bottomLeftPoint,
           F_POINT const& topLeftPoint,
           F_POINT const& bottomRightPoint,
           F_POINT const& topRightPoint);
void unbean(uint32_t start, uint32_t& finish);
void uncon();
auto unvis(boost::dynamic_bitset<> const& visitedRegions, int32_t& visitedIndex) -> bool;
auto vclpComp(V_CLP_X const& vclpx1, V_CLP_X const& vclpx2) noexcept -> bool;
auto vscmp(uint32_t index1, uint32_t index2) noexcept -> bool;

// Functions

void adfrm(uint32_t const iForm) {
  auto& formList = Instance->formList;

  auto currentForm = formList.operator[](iForm); // intended copy

  auto const originalVertexIndex = currentForm.vertexIndex;
  currentForm.vertexIndex        = wrap::toUnsigned(Instance->formVertices.size());
  auto const itVertex            = wrap::next(Instance->formVertices.cbegin(), originalVertexIndex);
  Instance->formVertices.insert(
      Instance->formVertices.end(), itVertex, wrap::next(itVertex, currentForm.vertexCount));
  if (currentForm.type == SAT && currentForm.satinGuideCount != 0U) {
	auto const originalGuide    = currentForm.satinGuideIndex;
	currentForm.satinGuideIndex = wrap::toUnsigned(Instance->satinGuides.size());

	auto const itGuides = wrap::next(Instance->satinGuides.cbegin(), originalGuide);
	Instance->satinGuides.insert(
	    Instance->satinGuides.end(), itGuides, wrap::next(itGuides, currentForm.satinGuideCount));
  }
  if (currentForm.isEdgeClipX()) {
	auto const originalBCData  = currentForm.borderClipData;
	currentForm.borderClipData = wrap::toUnsigned(Instance->clipPoints.size());

	auto const itClipPoints = wrap::next(Instance->clipPoints.cbegin(), originalBCData);
	Instance->clipPoints.insert(
	    Instance->clipPoints.end(), itClipPoints, wrap::next(itClipPoints, currentForm.clipEntries));
  }
  if (currentForm.isClipX()) {
	auto const originalClip = currentForm.clipIndex;
	currentForm.clipIndex   = wrap::toUnsigned(Instance->clipPoints.size());

	auto const itClipPoints = wrap::next(Instance->clipPoints.cbegin(), originalClip);
	Instance->clipPoints.insert(
	    Instance->clipPoints.end(), itClipPoints, wrap::next(itClipPoints, currentForm.clipCount));
  }
  formList.push_back(currentForm);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
}

auto fplComp(F_POINT_LINE const& point1, F_POINT_LINE const& point2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (point1.y < point2.y) {
	return true;
  }
  if (point2.y < point1.y) {
	return false;
  }
  if (point1.x < point2.x) {
	return true;
  }
  if (point2.x < point1.x) {
	return false;
  }
  return false;
}

void rotbak(float const rotationAngle, F_POINT const& rotationCenter) noexcept {
  for (auto& iSequence : Instance->oSequence) {
	thred::rotflt(iSequence, rotationAngle, rotationCenter);
  }
}

auto px2stchf(POINT const& screen) noexcept -> F_POINT {
  auto const factorX = wrap::toFloat(screen.x) / wrap::toFloat(StitchWindowClientRect.right);
  auto const factorY = wrap::toFloat(StitchWindowClientRect.bottom - screen.y) /
                       wrap::toFloat(StitchWindowClientRect.bottom);
  return F_POINT {(factorX * (ZoomRect.right - ZoomRect.left)) + ZoomRect.left,
                  (factorY * (ZoomRect.top - ZoomRect.bottom)) + ZoomRect.bottom};
}

void frmToLine(FRM_HEAD const& form) {
  if (form.vertexCount == 0U) {
	return;
  }
  auto& formLines = Instance->formLines;
  formLines.clear();
  formLines.reserve(form.vertexCount);
  auto const itFirstVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);

  auto itCurrentVertex = itFirstVertex; // intentional copy
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	formLines.push_back(POINT {std::lround((itCurrentVertex->x - ZoomRect.left) * ZoomRatio.x),
	                           std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                       ((itCurrentVertex->y - ZoomRect.bottom) * ZoomRatio.y))});
	++itCurrentVertex;
  }
  formLines.push_back(POINT {std::lround((itFirstVertex->x - ZoomRect.left) * ZoomRatio.x),
                             std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                                         ((itFirstVertex->y - ZoomRect.bottom) * ZoomRatio.y))});
}

void frmsqr(uint32_t const vertexIndex, uint32_t const iVertex) {
  auto       line             = std::array<POINT, TRIPNTS> {};
  auto const itCurrentVertex  = wrap::next(Instance->formVertices.cbegin(), vertexIndex + iVertex);
  auto const itPreviousVertex = std::next(itCurrentVertex, -1);
  line[1]                     = thred::stch2pxr(*itCurrentVertex);
  auto const ratio            = wrap::toFloat(thred::duScale(IniFile.formVertexSizePixels)) /
                     wrap::toFloat(StitchWindowClientRect.right);
  constexpr auto DFACT  = 2.0F;
  auto           length = (ZoomRect.right - ZoomRect.left) * ratio * DFACT;
  auto const     delta =
      F_POINT {itPreviousVertex->x - itCurrentVertex->x, itPreviousVertex->y - itCurrentVertex->y};
  auto       angle  = std::atan2(delta.y, delta.x);
  auto       xVal   = length * cos(angle);
  auto       yVal   = length * sin(angle);
  auto       offset = F_POINT {xVal, yVal};
  auto const point  = F_POINT {itCurrentVertex->x + offset.x, itCurrentVertex->y + offset.y};
  angle             = std::atan2(-delta.x, delta.y);
  length *= HALF;
  xVal               = length * cos(angle);
  yVal               = length * sin(angle);
  offset             = F_POINT {xVal, yVal};
  auto adjustedPoint = F_POINT {point.x + offset.x, point.y + offset.y};
  line[0] = line[3] = thred::stch2pxr(adjustedPoint);
  adjustedPoint     = F_POINT {point.x - offset.x, point.y - offset.y};
  line[2]           = thred::stch2pxr(adjustedPoint);
  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
}

void frmsqr0(POINT const& controlPoint) {
  auto line = std::array<POINT, SQPNTS> {};
  if (auto const offset = thred::duScale(gsl::narrow<int32_t>(IniFile.formBoxSizePixels)); offset != 0) {
	line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
	line[0].y = line[1].y = controlPoint.y - offset;
	line[1].x = line[2].x = controlPoint.x + offset + 1;
	line[2].y = line[3].y = controlPoint.y + offset + 1;
	line[4].y             = controlPoint.y - 1;
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
}

// ReSharper disable CppParameterMayBeConst
void frmx(POINT const& controlPoint, HDC hDC) noexcept(std::is_same_v<size_t, uint32_t>) {
  auto       line   = std::array<POINT, LNPNTS> {};
  auto const offset = thred::duScale(8);
  SelectObject(hDC, FormSelectedPen);
  line[0].x = line[1].x = controlPoint.x;
  line[0].y             = controlPoint.y + offset;
  line[1].y             = controlPoint.y - offset;
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
  line[0].y = line[1].y = controlPoint.y;
  line[0].x             = controlPoint.x - offset;
  line[1].x             = controlPoint.x + offset;
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
  SelectObject(hDC, FormPen);
}
// ReSharper restore CppParameterMayBeConst

void frmpoly(gsl::span<POINT> const& source) noexcept {
  for (auto const& iPoint : source) {
	wrap::polyline(StitchWindowMemDC, std::addressof(iPoint), LNPNTS);
  }
}

void drawStartGuide(FRM_HEAD const& form,
                    uint8_t const   layer,
                    unsigned int&   lastPoint) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  SelectObject(StitchWindowMemDC, FormPen);
  auto& formLines = Instance->formLines;
  frmpoly(gsl::span(std::addressof(formLines.operator[](1)), form.wordParam - 1));
  SelectObject(StitchWindowMemDC, FormPen3px);
  wrap::polyline(StitchWindowMemDC, std::addressof(formLines.operator[](form.wordParam)), LNPNTS);
  SelectObject(StitchWindowMemDC, getLayerPen(layer));
  lastPoint = form.wordParam + 1U;
}

void drawGuides(FRM_HEAD const& form) {
  auto       line          = std::array<POINT, 2> {};
  auto const itFirstVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto       itGuide       = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
  auto const maxGuide      = form.satinGuideCount;
  for (auto iGuide = 0U; iGuide < maxGuide; ++iGuide) {
	auto const itStartVertex  = wrap::next(itFirstVertex, itGuide->start);
	auto const itFinishVertex = wrap::next(itFirstVertex, itGuide->finish);
	line[0]                   = form::sfCor2px(*itStartVertex);
	line[1]                   = form::sfCor2px(*itFinishVertex);
	SelectObject(StitchWindowMemDC, FormPen);
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
	++itGuide;
  }
}

void drawFormBox(FRM_HEAD const& form) {
  auto const& formLines = Instance->formLines;
  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	if (iVertex == ClosestVertexToCursor) {
	  frmx(formLines.operator[](iVertex), StitchWindowMemDC);
	}
	else {
	  frmsqr(form.vertexIndex, iVertex);
	}
  }
  if (ClosestVertexToCursor != 0U) {
	frmsqr0(formLines.front());
  }
  else {
	frmx(formLines.front(), StitchWindowMemDC);
  }
  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
}

void drawFormsBox() {
  thred::selectMultiFormPen();
  thred::ratsr();
  SelectedFormsRect = RECT {BIGLONG, LOWLONG, LOWLONG, BIGLONG};
  for (auto const selectedForm : Instance->selectedFormList) {
	form::fselrct(selectedForm);
  }
  wrap::narrow_cast(SelectedFormsSize.x, SelectedFormsRect.right - SelectedFormsRect.left);
  wrap::narrow_cast(SelectedFormsSize.y, SelectedFormsRect.bottom - SelectedFormsRect.top);
  form::dubig();
}

void setpoli() {
  auto form           = FRM_HEAD {};
  form.type           = FRMFPOLY;
  form.attribute      = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  form.underlayIndent = IniFile.underlayIndent;
  Instance->formList.push_back(form);
  Instance->stateMap.set(StateFlag::SATIN);
  Instance->stateMap.set(StateFlag::INIT);
}

void setlin() {
  auto form      = FRM_HEAD {};
  form.type      = FRMLINE;
  form.attribute = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  Instance->formList.push_back(form);
  Instance->stateMap.set(StateFlag::SATIN);
  Instance->stateMap.set(StateFlag::INIT);
}

void setrpoli() {
  thred::unmsg();
  displayText::pntmsg(IDS_REGP);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRPOL);
  thred::numWnd();
}

void setstar() {
  thred::unmsg();
  displayText::pntmsg(IDS_STAR);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRSTAR);
  thred::numWnd();
}

void setspir() {
  thred::unmsg();
  displayText::pntmsg(IDS_SPIR);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRSPIR);
  thred::numWnd();
}

void sethart() {
  thred::unmsg();
  displayText::pntmsg(IDS_HEART);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRHART);
  thred::numWnd();
}

void setlens() {
  thred::unmsg();
  displayText::pntmsg(IDS_LENS);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRLENS);
  thred::numWnd();
}

void seteg() {
  thred::unmsg();
  displayText::pntmsg(IDS_EGG);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTREG);
  thred::numWnd();
}

void setzig() {
  thred::unmsg();
  displayText::pntmsg(IDS_ZIG);
  Instance->stateMap.set(StateFlag::NUMIN);
  Instance->stateMap.set(StateFlag::ENTRZIG);
  thred::numWnd();
}

auto findDistanceToSide(F_POINT const& lineStart, F_POINT const& lineEnd, F_POINT const& point, float& distance) noexcept -> float {
  auto const varA = point.x - lineStart.x;
  auto const varB = point.y - lineStart.y;
  auto const varC = lineEnd.x - lineStart.x;
  auto const varD = lineEnd.y - lineStart.y;
  if (varC == 0.0F && varD == 0.0F) {
	distance = std::sqrt(varA * varB);
	constexpr auto FIRST = -0.1F; // Arbitrarily choose the first point since start and end are the same
	return FIRST;
  }
  auto const dot     = (varA * varC) + (varB * varD);
  auto const lenSqrd = (varC * varC) + (varD * varD);
  auto const param   = dot / lenSqrd;
  // NOLINTBEGIN(readability-avoid-nested-conditional-operator)
  auto const diff =
      param < 0 ? F_POINT {point.x - lineStart.x, point.y - lineStart.y} // Before the first point
      : param > 1
          ? F_POINT {point.x - lineEnd.x, point.y - lineEnd.y} // After the last point
          : F_POINT {point.x - (lineStart.x + param * varC), point.y - (lineStart.y + param * varD)}; // Between endpoints
  // NOLINTEND(readability-avoid-nested-conditional-operator)
  // returning shortest distance
  distance = std::sqrt((diff.x * diff.x) + (diff.y * diff.y));
  return param;
}

auto ritlin(F_POINT const& start, F_POINT const& finish, float userStitchLen) -> bool {
  auto const     delta  = F_POINT {finish.x - start.x, finish.y - start.y};
  auto           length = std::hypot(delta.x, delta.y);
  constexpr auto CLAMP  = 200.0F;
  // This clamp is temporary to avoid overflow when BH corner value is too large. Find a better fix
  length = std::min(length, CLAMP);

  auto& interleaveSequence = Instance->interleaveSequence;
  interleaveSequence.push_back(start);
  if (length > MaxStitchLen) {
	constexpr auto MINSTLEN = 1e-1F; // clamp minimum stitch length

	userStitchLen = std::max(userStitchLen, MINSTLEN);
	auto count    = wrap::ceil<uint32_t>(length / userStitchLen);
	if (count == 0U) {
	  count = 1;
	}
	auto newLength = length / wrap::toFloat(count);
	while (newLength > MaxStitchLen && newLength > MINSTLEN) {
	  ++count;
	  newLength = length / wrap::toFloat(count);
	}
	auto const step  = F_POINT {delta.x / wrap::toFloat(count), delta.y / wrap::toFloat(count)};
	auto       point = F_POINT {start.x + step.x, start.y + step.y};
	for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	  interleaveSequence.push_back(point);
	  point += step;
	}
  }
  return true;
}

void ritbrd(FRM_HEAD const& form) {
  if (Instance->oSequence.empty()) {
	return;
  }
  Instance->interleaveSequenceIndices.emplace_back(
      INS_REC {.code  = TYPBRD,
               .color = gsl::narrow_cast<uint32_t>(form.borderColor) & COLMSK,
               .index = wrap::toUnsigned(Instance->interleaveSequence.size()),
               .seq   = I_BRD});
  form::chkseq(true);
}

void ritapbrd() {
  if (Instance->oSequence.empty()) {
	return;
  }
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);
  Instance->interleaveSequenceIndices.emplace_back(
      INS_REC {.code  = TYPMSK,
               .color = gsl::narrow_cast<uint32_t>(form.borderColor) >> 4U,
               .index = wrap::toUnsigned(Instance->interleaveSequence.size()),
               .seq   = I_AP});
  form::chkseq(true);
}

void ritfil(FRM_HEAD& form) {
  if (Instance->oSequence.empty()) {
	return;
  }
  Instance->interleaveSequenceIndices.emplace_back(
      INS_REC {.code  = TYPFRM,
               .color = gsl::narrow_cast<uint32_t>(form.fillColor),
               .index = wrap::toUnsigned(Instance->interleaveSequence.size()),
               .seq   = I_FIL});
  form::chkseq(false);
}

void bdrlin(uint32_t const vertexIndex, uint32_t const start, uint32_t const finish, float const stitchSize) {
  auto const itStartVertex  = wrap::next(Instance->formVertices.cbegin(), vertexIndex + start);
  auto const itFinishVertex = wrap::next(Instance->formVertices.cbegin(), vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length      = std::hypot(delta.x, delta.y);
  auto       stitchCount = Instance->userFlagMap.test(UserFlag::LINSPAC)
                               ? wrap::ceil<uint32_t>(length / stitchSize)
                               : wrap::round<uint32_t>(((length - stitchSize * HALF) / stitchSize) + 1.0F);
  auto       step        = F_POINT {};
  if (Instance->userFlagMap.test(UserFlag::LINSPAC)) {
	if (stitchCount != 0U) {
	  step = delta / wrap::toFloat(stitchCount);
	}
  }
  else {
	// ToDo - Is this calculation correct?
	auto const angle = std::atan2(delta.y, delta.x);

	step = F_POINT {cos(angle) * stitchSize, sin(angle) * stitchSize};
  }
  if (stitchCount != 0U) {
	auto point = F_POINT {itStartVertex->x + step.x, itStartVertex->y + step.y};
	--stitchCount;
	while (stitchCount != 0U) {
	  Instance->oSequence.push_back(point);
	  point += step;
	  --stitchCount;
	}
  }
  Instance->oSequence.push_back(*itFinishVertex);
}

void brdfil(FRM_HEAD const& form) {
  auto currentVertex = (form.extendedAttribute & AT_STRT) != 0U ? form.fillStart : form::getlast(form);
  Instance->oSequence.clear();
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + currentVertex);
  Instance->oSequence.push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, form.edgeStitchLen);
	currentVertex = nextVertex;
  }
  if (form.type != FRMLINE) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, form.edgeStitchLen);
  }
}

void boldlin(uint32_t const vertexIndex, uint32_t const start, uint32_t const finish, float size) {
  auto const itStartVertex  = wrap::next(Instance->formVertices.cbegin(), vertexIndex + start);
  auto const itFinishVertex = wrap::next(Instance->formVertices.cbegin(), vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length = std::hypot(delta.x, delta.y);

  constexpr auto ESCLAMP = 1e-1F; // edge stitch minimum length clamp

  size       = std::max(size, ESCLAMP);
  auto count = wrap::round<uint32_t>(length / size);
  if (count == 0U) {
	Instance->oSequence.push_back(*itFinishVertex);
	Instance->oSequence.push_back(*itStartVertex);
	Instance->oSequence.push_back(*itFinishVertex);
	return;
  }
  auto const step   = F_POINT {delta.x / wrap::toFloat(count), delta.y / wrap::toFloat(count)};
  auto       point0 = *itStartVertex; // intentional copy
  auto       point1 = F_POINT {point0.x + step.x, point0.y + step.y};
  while (count != 0U) {
	Instance->oSequence.push_back(point1);
	Instance->oSequence.push_back(point0);
	Instance->oSequence.push_back(point1);
	point0 += step;
	point1 += step;
	--count;
  }
  Instance->oSequence.push_back(*itFinishVertex);
}

void bold(FRM_HEAD const& form) {
  auto iOutput   = 0U;
  auto iNextLine = form::getlast(form);
  auto iLine     = iNextLine;
  Instance->oSequence.clear();
  auto const itLineVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + iLine);
  Instance->oSequence.push_back(*itLineVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	iNextLine = form::nxt(form, iLine);
	boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
	iLine = iNextLine;
  }
  if (form.type != FRMLINE) {
	iNextLine = form::nxt(form, iLine);
	boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
  }
  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(Instance->oSequence.size() - 1U); ++iSequence) {
	auto const& sequence     = Instance->oSequence.operator[](iSequence);
	auto const& sequenceFwd1 = Instance->oSequence.operator[](wrap::toSize(iSequence) + 1U);
	auto const  deltaX       = sequenceFwd1.x - sequence.x;
	auto const  deltaY       = sequenceFwd1.y - sequence.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length > TNYFLOAT) {
	  Instance->oSequence.operator[](iOutput++) = sequence;
	}
  }
  auto const itNextVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + iNextLine);
  Instance->oSequence.operator[](iOutput++) = *itNextVertex;
  Instance->oSequence.resize(iOutput);
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
auto projh(float const yCoordinate, F_POINT const& point0, F_POINT const& point1, F_POINT& intersection) noexcept -> bool {
  auto const deltaX = point1.x - point0.x;
  intersection.y    = yCoordinate;
  if (deltaX != 0.0F) {
	if (auto const deltaY = point1.y - point0.y; deltaY != 0.0F) {
	  auto const slope = deltaY / deltaX;
	  intersection.x   = (yCoordinate - point0.y) / slope + point0.x;
	}
	else {
	  return false;
	}
  }
  else {
	intersection.x = point0.x;
  }
  auto leftY  = point0.y;
  auto rightY = point1.y;
  if (leftY > rightY) {
	std::swap(leftY, rightY);
  }
  return yCoordinate >= leftY && yCoordinate <= rightY;
}

void sprct(std::vector<F_POINT> const& vertices,
           uint32_t const              vertexIndex,
           std::vector<V_RECT_2>&      fillVerticalRect,
           uint32_t const              start,
           uint32_t const              finish,
           std::vector<F_POINT> const& outsidePoints) noexcept(!std::is_same_v<uint32_t, size_t>) {
  auto const& opStart        = outsidePoints[start];
  auto const& opFinish       = outsidePoints[finish];
  auto const& ipStart        = InsidePoints->operator[](start);
  auto const& ipFinish       = InsidePoints->operator[](finish);
  auto const  delta          = F_POINT {(opFinish.x - opStart.x), (opFinish.y - opStart.y)};
  auto const  itStartVertex  = wrap::next(vertices.cbegin(), vertexIndex + start);
  auto const  itFinishVertex = wrap::next(vertices.cbegin(), vertexIndex + finish);
  if (delta.x != 0.0F && delta.y != 0.0F) {
	auto const slope        = -delta.x / delta.y;
	auto       point        = *itFinishVertex; // intentional copy
	auto&      verticalRect = fillVerticalRect[start];
	proj(point, slope, opStart, opFinish, verticalRect.dopnt);
	proj(point, slope, ipStart, ipFinish, verticalRect.dipnt);
	point = *itStartVertex;
	proj(point, slope, opStart, opFinish, verticalRect.aopnt);
	proj(point, slope, ipStart, ipFinish, verticalRect.aipnt);
	point = ipStart;
	if (proj(point, slope, opStart, opFinish, verticalRect.bopnt)) {
	  verticalRect.bipnt = ipStart;
	}
	else {
	  verticalRect.bopnt = opStart;
	  point              = opStart;
	  proj(point, slope, ipStart, ipFinish, verticalRect.bipnt);
	}
	point = ipFinish;
	if (proj(point, slope, opStart, opFinish, verticalRect.copnt)) {
	  verticalRect.cipnt = ipFinish;
	}
	else {
	  verticalRect.copnt = opFinish;
	  point              = opFinish;
	  proj(point, slope, ipStart, ipFinish, verticalRect.cipnt);
	}
	return;
  }
  if (delta.x != 0.0F) {
	auto  pointX       = itFinishVertex->x;
	auto& verticalRect = fillVerticalRect[start];
	projv(pointX, opStart, opFinish, verticalRect.dopnt);
	projv(pointX, ipStart, ipFinish, verticalRect.dipnt);
	pointX = itStartVertex->x;
	projv(pointX, opStart, opFinish, verticalRect.aopnt);
	projv(pointX, ipStart, ipFinish, verticalRect.aipnt);
	if (projv(ipStart.x, opStart, opFinish, verticalRect.bopnt)) {
	  verticalRect.bipnt = ipStart;
	}
	else {
	  verticalRect.bopnt = opStart;
	  projv(opStart.x, ipStart, ipFinish, verticalRect.bipnt);
	}
	if (projv(ipFinish.x, opStart, opFinish, verticalRect.copnt)) {
	  verticalRect.cipnt = ipFinish;
	}
	else {
	  verticalRect.copnt = opFinish;
	  projv(opFinish.x, ipStart, ipFinish, verticalRect.cipnt);
	}
	return;
  }
  auto  pointY       = itFinishVertex->y;
  auto& verticalRect = fillVerticalRect[start];
  projh(pointY, opStart, opFinish, verticalRect.dopnt);
  projh(pointY, ipStart, ipFinish, verticalRect.dipnt);
  pointY = itStartVertex->y;
  projh(pointY, opStart, opFinish, verticalRect.aopnt);
  projh(pointY, ipStart, ipFinish, verticalRect.aipnt);
  if (projh(ipStart.y, opStart, opFinish, verticalRect.bopnt)) {
	verticalRect.bipnt = ipStart;
  }
  else {
	verticalRect.bopnt = opStart;
	projh(opStart.y, ipStart, ipFinish, verticalRect.bipnt);
  }
  if (projh(ipFinish.y, opStart, opFinish, verticalRect.copnt)) {
	verticalRect.cipnt = ipFinish;
  }
  else {
	verticalRect.copnt = opFinish;
	projh(opFinish.y, ipStart, ipFinish, verticalRect.cipnt);
  }
}

void spurfn(F_POINT const& innerPoint, F_POINT const& outerPoint, F_POINT& underlayInnerPoint, F_POINT& underlayOuterPoint) noexcept {
  auto const     delta  = F_POINT {outerPoint.x - innerPoint.x, outerPoint.y - innerPoint.y};
  constexpr auto DIURAT = (1.0F - URAT) / 2.0F;
  constexpr auto DOURAT = ((1.0F - URAT) / 2.0F) + URAT;

  underlayInnerPoint = delta * DIURAT + innerPoint;
  underlayOuterPoint = delta * DOURAT + innerPoint;
}

void spurct(std::vector<V_RECT_2>&       underlayVerticalRect,
            std::vector<V_RECT_2> const& fillVerticalRect,
            uint32_t const               iRect) noexcept {
  auto const& fRect  = fillVerticalRect[iRect];
  auto&       ulRect = underlayVerticalRect[iRect];
  spurfn(fRect.aipnt, fRect.aopnt, ulRect.aipnt, ulRect.aopnt);
  spurfn(fRect.bipnt, fRect.bopnt, ulRect.bipnt, ulRect.bopnt);
  spurfn(fRect.cipnt, fRect.copnt, ulRect.cipnt, ulRect.copnt);
  spurfn(fRect.dipnt, fRect.dopnt, ulRect.dipnt, ulRect.dopnt);
}

void duromb(F_POINT const& start0, F_POINT const& finish0, F_POINT const& start1, F_POINT const& finish1, F_POINT& stitchPoint) {
  if (!Instance->stateMap.test(StateFlag::UND)) {
	auto const length0 = std::hypot(stitchPoint.x - start0.x, stitchPoint.y - start0.y);
	if (auto const length1 = std::hypot(stitchPoint.x - start1.x, stitchPoint.y - start1.y); length0 > length1) {
	  Instance->stateMap.set(StateFlag::FILDIR);
	}
	else {
	  Instance->stateMap.reset(StateFlag::FILDIR);
	}
  }
  auto const delta0  = F_POINT {finish0.x - start0.x, finish0.y - start0.y};
  auto const delta1  = F_POINT {finish1.x - start1.x, finish1.y - start1.y};
  auto const length0 = std::hypot(delta0.x, delta0.y);
  auto       count   = wrap::round<uint32_t>(length0 / (LineSpacing * HALF));
  if (count == 0U) {
	++count;
  }
  auto const factor  = wrap::toFloat(count);
  auto const step0   = F_POINT {delta0.x / factor, delta0.y / factor};
  auto const step1   = F_POINT {delta1.x / factor, delta1.y / factor};
  auto       start00 = start0; // intentional copy
  auto       start11 = start1; // intentional copy
  for (auto iStep = 0U; iStep < count; ++iStep) {
	if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
	  form::filinsb(start00, stitchPoint);
	}
	else {
	  form::filinsb(start11, stitchPoint);
	}
	start00 += step0;
	start11 += step1;
  }
}

void pfn(std::vector<V_RECT_2> const& underlayVerticalRect,
         std::vector<V_RECT_2> const& fillVerticalRect,
         uint32_t const               startVertex,
         std::vector<V_RECT_2> const& vrct,
         float const                  width) {
  // clang-format off
  auto const& form          = Instance->formList.operator[](ClosestFormToCursor);
  auto        currentVertex = startVertex;
  auto        nextVertex    = form::nxt(form, currentVertex);
  auto const  itVertex      = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + startVertex);
  auto        stitchPoint   = *itVertex; // intentional copy
  // clang-format on
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	duromb(vrct[currentVertex].bipnt,
	       vrct[currentVertex].cipnt,
	       vrct[currentVertex].bopnt,
	       vrct[currentVertex].copnt,
	       stitchPoint);
	duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, currentVertex, nextVertex, width, stitchPoint);
	currentVertex = nextVertex;
	nextVertex    = form::nxt(form, nextVertex);
  }
}

void prsmal(float const width) {
  constexpr auto UFSFACT = 0.8F; // set the minimum to 80% of the underlay fill spacing

  auto iOutput       = 0U;
  auto minimumLength = USPAC * UFSFACT;
  if (minimumLength > width) {
	constexpr auto WIDFACT = 0.9F; // or 90% of the actual width
	minimumLength          = width * WIDFACT;
  }
  auto       iReference  = 0U;
  auto const lengthCheck = minimumLength * minimumLength;
  for (auto iSequence = 1U; iSequence < wrap::toUnsigned(Instance->oSequence.size()); ++iSequence) {
	auto const& seq    = Instance->oSequence.operator[](iSequence);
	auto const& seqRef = Instance->oSequence.operator[](iReference);
	auto const  deltaX = seq.x - seqRef.x;
	auto const  deltaY = seq.y - seqRef.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length > lengthCheck) {
	  Instance->oSequence.operator[](iOutput++) = seq;

	  iReference = iSequence;
	}
  }
  Instance->oSequence.resize(iOutput);
}

void pbrd(FRM_HEAD const& form) {
  auto const spacing = LineSpacing;
  auto const start   = form::getlast(form);
  LineSpacing        = form.edgeSpacing;
  Instance->oSequence.clear();
  auto fillVerticalRect = std::vector<V_RECT_2> {};
  fillVerticalRect.resize(form.vertexCount);
  auto underlayVerticalRect = std::vector<V_RECT_2> {};
  underlayVerticalRect.resize(form.vertexCount);
  satin::satout(form, form.borderSize);
  auto const& outsidePoints = *OutsidePoints;
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	sprct(Instance->formVertices, form.vertexIndex, fillVerticalRect, iVertex, iVertex + 1U, outsidePoints);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
  }
  sprct(Instance->formVertices, form.vertexIndex, fillVerticalRect, form.vertexCount - 1U, 0U, outsidePoints);
  spurct(underlayVerticalRect, fillVerticalRect, form.vertexCount - 1U);
  if ((form.edgeType & EGUND) != 0U) {
	Instance->stateMap.reset(StateFlag::SAT1);
	LineSpacing = USPAC;
	Instance->stateMap.set(StateFlag::UND);
	auto const width = form.borderSize * URAT;
	satin::satout(form, width);
	Instance->stateMap.set(StateFlag::UNDPHAS);
	Instance->stateMap.set(StateFlag::FILDIR);
	pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
	Instance->stateMap.reset(StateFlag::UNDPHAS);
	Instance->stateMap.reset(StateFlag::FILDIR);
	pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
	LineSpacing = form.edgeSpacing;
	prsmal(width);
	Instance->stateMap.reset(StateFlag::UND);
  }
  pfn(underlayVerticalRect, fillVerticalRect, start, fillVerticalRect, form.borderSize);
  LineSpacing = spacing;
}

void apbrd(FRM_HEAD const& form) {
  auto currentVertex = 0U;
  Instance->oSequence.clear();
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  Instance->oSequence.push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount * 2U; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, IniFile.AppStitchLen);
	currentVertex = nextVertex;
  }
}

void bhfn(FRM_HEAD const& form, uint32_t const start, uint32_t const finish) {
  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + start);
  auto const itFinishVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length = std::hypot(delta.x, delta.y);
  auto const count  = wrap::round<uint32_t>(length / form.edgeSpacing);
  auto const step   = F_POINT {(delta.x / wrap::toFloat(count)), (delta.y / wrap::toFloat(count))};
  auto       innerPoint    = F_POINT {itStartVertex->x, itStartVertex->y};
  auto const rotationAngle = std::atan2(-delta.x, delta.y);
  auto const outerStep = F_POINT {form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle)};
  if (count == 0U) {
	return;
  }
  for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	auto const firstPoint = F_POINT {innerPoint.x + step.x, innerPoint.y + step.y};
	auto const outerPoint = F_POINT {firstPoint.x + outerStep.x, firstPoint.y + outerStep.y};
	Instance->oSequence.emplace_back(firstPoint);
	Instance->oSequence.emplace_back(innerPoint);
	Instance->oSequence.emplace_back(firstPoint);
	Instance->oSequence.emplace_back(outerPoint);
	Instance->oSequence.emplace_back(firstPoint);
	Instance->oSequence.emplace_back(outerPoint);
	Instance->oSequence.emplace_back(firstPoint);
	innerPoint += step;
  }
  auto const firstPoint = F_POINT {innerPoint.x + step.x, innerPoint.y + step.y};
  Instance->oSequence.emplace_back(firstPoint);
  Instance->oSequence.emplace_back(innerPoint);
}

void bhcrnr(FRM_HEAD const& form, uint32_t const vertex) {
  auto const  nextVertex = form::nxt(form, vertex);
  auto const  itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + nextVertex);
  auto const* ptr      = Instance->stateMap.test(StateFlag::INDIR) ? OutsidePoints : InsidePoints;
  if (nullptr == ptr) {
	return;
  }
  auto delta =
      F_POINT {ptr->operator[](nextVertex).x - itVertex->x, ptr->operator[](nextVertex).y - itVertex->y};
  auto const length = std::hypot(delta.x, delta.y);
  auto const ratio  = ButtonholeCornerLength / length;
  delta *= ratio;
  auto const point = F_POINT {itVertex->x + delta.x, itVertex->y + delta.y};
  Instance->oSequence.push_back(*itVertex);
  Instance->oSequence.emplace_back(point);
  Instance->oSequence.push_back(*itVertex);
  Instance->oSequence.emplace_back(point);
  Instance->oSequence.push_back(*itVertex);
}

void bhbrd(FRM_HEAD const& form) {
  auto vertex = form::getlast(form);
  Instance->oSequence.clear();
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex + vertex);
  Instance->oSequence.push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, vertex);
	bhfn(form, vertex, nextVertex);
	bhcrnr(form, vertex);
	vertex = nextVertex;
  }
}

void dubfn(FRM_HEAD const& currentForm) {
  brdfil(currentForm);
  for (auto iBackward = Instance->oSequence.size(); iBackward != 0; --iBackward) {
	Instance->oSequence.push_back(Instance->oSequence.operator[](iBackward - 1U));
  }
}

void chkbrd(FRM_HEAD const& form) {
  if (form.edgeType == 0U) {
	return;
  }
  switch (form.edgeType & NEGUND) {
	case EDGELINE: { // Line
	  brdfil(form);
	  break;
	}
	case EDGEBEAN: { // Bean
	  bold(form);
	  break;
	}
	case EDGECLIP: { // Clipboard
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
	  clip::clpbrd(form, clipRect, form::getlast(form));
	  break;
	}
	case EDGECLIPX: { // Even Clipboard
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
	  clip::duxclp(form);
	  break;
	}
	case EDGEANGSAT: { // Angle Satin
	  satin::sbrd(form);
	  break;
	}
	case EDGEPROPSAT: { // Proportional Satin
	  pbrd(form);
	  break;
	}
	case EDGEAPPL: { // Applique
	  apbrd(form);
	  ritapbrd();
	  satin::sbrd(form);
	  break;
	}
	case EDGEBHOL: { // BH Buttonhole
	  auto const length      = ButtonholeCornerLength;
	  ButtonholeCornerLength = form::getblen();
	  satin::satout(form, BHWIDTH);
	  bhbrd(form);
	  ButtonholeCornerLength = length;
	  break;
	}
	case EDGEPICOT: { // Picot
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
	  clip::clpic(form, clipRect);
	  break;
	}
	case EDGEDOUBLE: { // Double
	  dubfn(form);
	  break;
	}
	case EDGELCHAIN: { // Lin Chain
	  Instance->stateMap.set(StateFlag::LINCHN);
	  clip::chnfn(form);
	  break;
	}
	case EDGEOCHAIN: { // Open Chain
	  Instance->stateMap.reset(StateFlag::LINCHN);
	  clip::chnfn(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in chkbrd: edgeType [{}]\n", form.edgeType & NEGUND);
	  break;
	}
  }
  ritbrd(form);
}

void fnvrt(std::vector<F_POINT>&    currentFillVertices,
           std::vector<uint32_t>&   groupIndexSequence,
           std::vector<SMAL_PNT_L>& lineEndpoints) {
  if (!currentFillVertices.empty()) {
	auto const mmTuple =
	    std::ranges::minmax_element(currentFillVertices, [](F_POINT const& first, F_POINT const& second) {
	      return first.x < second.x;
	    });
	auto const highX           = mmTuple.max->x;
	auto const lineOffset      = std::floor(mmTuple.min->x / LineSpacing);
	auto const lowX            = LineSpacing * lineOffset;
	auto       fillLineCount   = wrap::floor<uint32_t>(((highX - lowX) / LineSpacing) + 1.0F);
	auto const step            = (highX - lowX) / wrap::toFloat(fillLineCount);
	auto       currentX        = lowX;
	auto       projectedPoints = std::vector<F_POINT_LINE> {};
	projectedPoints.reserve(currentFillVertices.size() + 2U);
	auto const currentVertexCount = currentFillVertices.size();
	for (auto iLine = 0U; iLine < fillLineCount; ++iLine) {
	  auto iLineCounter = 0U;
	  currentX += step;
	  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
		// currentVertexCount cannot be 0 so no need to check for division by 0
		auto const iNextVertex = (wrap::toSize(iVertex) + 1U) % currentVertexCount; // NOLINT(clang-analyzer-core.DivideZero)
		if (auto point = F_POINT {};
		    projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
		  ++iLineCounter;
		}
	  }
	  fillLineCount += iLineCounter;
	}
	lineEndpoints.reserve(wrap::toSize(fillLineCount) + 1U);
	auto lineGroupIndex = 0U;
	// groupIndex cannot be more than fillLineCount so reserve that amount of memory to reduce re-allocations
	groupIndexSequence.reserve(fillLineCount);
	currentX = lowX;
	for (auto iLine = 0U; iLine < fillLineCount; ++iLine) {
	  projectedPoints.clear();
	  projectedPoints.reserve(currentVertexCount);
	  currentX += step;
	  auto       iPoint      = 0U;
	  auto const vertexCheck = gsl::narrow<uint16_t>(currentVertexCount);
	  for (auto iVertex = uint16_t {}; iVertex < vertexCheck; ++iVertex) {
		auto const iNextVertex = (wrap::toSize(iVertex) + 1U) % currentVertexCount; // NOLINT(clang-analyzer-core.DivideZero)
		if (auto point = F_POINT {};
		    projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
		  auto const projected = F_POINT_LINE {.x = point.x, .y = point.y, .line = iVertex};
		  projectedPoints.push_back(projected);
		  ++iPoint;
		}
	  }
	  if (iPoint <= 1) {
		continue;
	  }
	  auto const evenPointCount = iPoint & 0xfffffffe;
	  groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
	  std::ranges::sort(projectedPoints, fplComp);
	  iPoint                    = 0;
	  auto const savedLineCount = lineEndpoints.size();
	  while (iPoint < evenPointCount) {
		if (lineEndpoints.size() >= fillLineCount) {
		  continue;
		}
		lineEndpoints.emplace_back(projectedPoints[iPoint].line,
		                           lineGroupIndex,
		                           projectedPoints[iPoint].x,
		                           projectedPoints[iPoint].y);
		++iPoint;
		lineEndpoints.emplace_back(projectedPoints[iPoint].line,
		                           lineGroupIndex,
		                           projectedPoints[iPoint].x,
		                           projectedPoints[iPoint].y);
		++iPoint;
	  }
	  if (lineEndpoints.size() != savedLineCount) {
		++lineGroupIndex;
	  }
	}
	groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
  }
}

void fnang(std::vector<uint32_t>&   groupIndexSequence,
           std::vector<SMAL_PNT_L>& lineEndpoints,
           float const              rotationAngle,
           F_POINT&                 rotationCenter,
           FRM_HEAD&                angledForm,
           std::vector<F_POINT>&    angledFormVertices) {
  angledForm = Instance->formList.operator[](ClosestFormToCursor);

  auto const& angRect = angledForm.rectangle;
  rotationCenter = F_POINT {wrap::midl(angRect.right, angRect.left), wrap::midl(angRect.top, angRect.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*itVertex);
	++itVertex;
	thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
  }
  angledForm.vertexIndex = 0;
  fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void fnhor(std::vector<uint32_t>&   groupIndexSequence,
           std::vector<SMAL_PNT_L>& lineEndpoints,
           float const              rotationAngle,
           F_POINT&                 rotationCenter,
           FRM_HEAD&                angledForm,
           std::vector<F_POINT>&    angledFormVertices) {
  angledForm = Instance->formList.operator[](ClosestFormToCursor);

  auto const& angRect = angledForm.rectangle;
  rotationCenter = F_POINT {wrap::midl(angRect.right, angRect.left), wrap::midl(angRect.top, angRect.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*itVertex);
	thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	++itVertex;
  }
  angledForm.vertexIndex = 0;
  fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void prebrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices) {
  constexpr auto REDFACT = 0.1F; // reduction factor

  auto const itVertex     = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto const itNextVertex = std::next(itVertex);
  auto       delta = F_POINT {(itNextVertex->x - itVertex->x), (itNextVertex->y - itVertex->y)};
  angledFormVertices.resize(wrap::toSize(form.vertexCount) + 3U);
  auto output = angledFormVertices.begin();
  ++output;
  std::copy(itVertex, wrap::next(itVertex, form.vertexCount), output);
  auto ratio = fabs(delta.x) > fabs(delta.y) ? fabs(REDFACT / delta.x) : fabs(REDFACT / delta.y);
  angledFormVertices[0]  = *itVertex - delta * ratio;
  angledForm             = Instance->formList.operator[](ClosestFormToCursor);
  angledForm.vertexIndex = 0;
  angledForm.vertexCount += 3;
  auto const itLastVertex     = wrap::next(itVertex, form.vertexCount - 1U);
  auto const itPreviousVertex = wrap::next(itLastVertex, -1);
  delta                       = *itLastVertex - *itPreviousVertex;
  ratio = fabs(delta.x) > fabs(delta.y) ? fabs(REDFACT / delta.x) : fabs(REDFACT / delta.y);
  auto const prevIndex = angledForm.vertexCount - 1U;

  angledFormVertices[prevIndex] = *itLastVertex + delta * ratio;
}

void plfn(FRM_HEAD const&              form,
          std::vector<V_RECT_2> const& underlayVerticalRect,
          std::vector<V_RECT_2> const& fillVerticalRect,
          std::vector<V_RECT_2> const& prct,
          float const                  width,
          F_POINT&                     stitchPoint) {
  duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt, stitchPoint);
  duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, 1, 2, width, stitchPoint);
  for (auto iVertex = 2U; iVertex < form.vertexCount - 4U; ++iVertex) {
	duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt, stitchPoint);
	duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1U, width, stitchPoint);
  }
  auto const lastVertex = form.vertexCount - 4U;
  duromb(prct[lastVertex].bipnt, prct[lastVertex].dipnt, prct[lastVertex].bopnt, prct[lastVertex].dopnt, stitchPoint);
}

void plbak(uint32_t backPoint) noexcept(std::is_same_v<size_t, uint32_t>) {
  if (Instance->oSequence.empty() || backPoint >= Instance->oSequence.size() - 1U) {
	return;
  }
  auto iSequence = wrap::toUnsigned(Instance->oSequence.size() - 1U);
  while (iSequence > backPoint) {
	std::swap(Instance->oSequence.operator[](iSequence), Instance->oSequence.operator[](backPoint));
	--iSequence;
	++backPoint;
  }
}

void plbrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices) {
  // Ensure that we have at least 4 array members
  auto fillVerticalRect = std::vector<V_RECT_2> {};
  fillVerticalRect.resize(wrap::toSize(form.vertexCount) + 3U);
  auto underlayVerticalRect = std::vector<V_RECT_2> {};
  underlayVerticalRect.resize(wrap::toSize(form.vertexCount) + 3U);
  prebrd(form, angledForm, angledFormVertices);
  satin::satout(angledForm, angledForm.borderSize);
  InsidePoints->push_back(InsidePoints->front());
  OutsidePoints->push_back(OutsidePoints->front());
  auto const& outsidePoints = *OutsidePoints;
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount - 1U; ++iVertex) {
	sprct(Instance->angledFormVertices, 0, fillVerticalRect, iVertex, iVertex + 1U, outsidePoints);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
  }
  sprct(Instance->angledFormVertices, 0U, fillVerticalRect, angledForm.vertexCount - 1U, 0U, outsidePoints);
  spurct(underlayVerticalRect, fillVerticalRect, angledForm.vertexCount - 1U);
  auto const itVertex = wrap::next(angledFormVertices.cbegin(), angledForm.vertexIndex);
  if ((angledForm.attribute & SBLNT) == 0U) {
	auto const val = std::next(itVertex, 1U);

	fillVerticalRect[1].aipnt     = *val;
	fillVerticalRect[1].aopnt     = *val;
	underlayVerticalRect[1].aipnt = *val;
	underlayVerticalRect[1].aopnt = *val;
  }
  if ((angledForm.attribute & FBLNT) == 0U) {
	auto const itLastVertex = wrap::next(itVertex, angledForm.vertexCount - 1U);
	auto const index        = angledForm.vertexCount - 4U;

	fillVerticalRect[index].dipnt     = *itLastVertex;
	fillVerticalRect[index].dopnt     = *itLastVertex;
	underlayVerticalRect[index].dipnt = *itLastVertex;
	underlayVerticalRect[index].dopnt = *itLastVertex;
  }
  Instance->oSequence.clear();
  auto stitchPoint = *itVertex; // intentional copy
  if ((angledForm.edgeType & EGUND) != 0U) {
	LineSpacing = USPAC;
	Instance->stateMap.set(StateFlag::UND);
	auto const width = angledForm.borderSize * URAT;
	Instance->stateMap.set(StateFlag::UNDPHAS);
	Instance->stateMap.reset(StateFlag::FILDIR);
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width, stitchPoint);
	auto const savedIndex = wrap::toUnsigned(Instance->oSequence.size());
	Instance->stateMap.reset(StateFlag::UNDPHAS);
	stitchPoint = *itVertex;
	Instance->stateMap.set(StateFlag::FILDIR);
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width, stitchPoint);
	plbak(savedIndex);
	prsmal(width);
	if (!Instance->oSequence.empty()) { // ensure that we can do a valid read from oSequence
	  stitchPoint = Instance->oSequence.back();
	}
  }
  Instance->stateMap.reset(StateFlag::UND);
  LineSpacing = angledForm.edgeSpacing;
  plfn(angledForm, underlayVerticalRect, fillVerticalRect, fillVerticalRect, angledForm.borderSize, stitchPoint);
  LineSpacing = form.edgeSpacing;
}

void lapbrd(FRM_HEAD const& form) {
  auto const savedStitchLength = UserStitchLength;
  Instance->oSequence.clear();
  UserStitchLength = IniFile.AppStitchLen;
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	bdrlin(form.vertexIndex, iVertex, iVertex + 1U, IniFile.AppStitchLen);
  }
  for (auto iVertex = form.vertexCount - 1U; iVertex != 0U; --iVertex) {
	bdrlin(form.vertexIndex, iVertex, iVertex - 1U, IniFile.AppStitchLen);
  }
  UserStitchLength = savedStitchLength;
}

void blbrd(FRM_HEAD const& form) {
  Instance->oSequence.clear();
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  Instance->oSequence.push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 2U; ++iVertex) {
	bhfn(form, iVertex, iVertex + 1U);
	bhcrnr(form, iVertex);
  }
  bhfn(form, form.vertexCount - 2U, form.vertexCount - 1U);
  itVertex = wrap::next(itVertex, form.vertexCount - 1U);
  Instance->oSequence.push_back(*itVertex);
}

void contf(FRM_HEAD& form) {
  class P_VEC
  {
public:
	float angle {};
	float length {};
  };

  auto const start = form.fillGuide.start;
  // ToDo - Find a better way to avoid crashing than clamping it if start is after finish
  auto const finish = form.fillGuide.finish;
  if (start >= finish) {
	return;
  }
  // setup the form vertices
  auto       itFirstVertex    = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  auto const itSelectedVertex = wrap::next(itFirstVertex, start);
  auto const lowVertexIndex   = start;
  auto const highVertexIndex  = form.vertexCount - start - 1U;
  // setup the storage for the counts, lengths, deltas, steps, and vertices
  auto lowCounts = std::vector<uint32_t> {};
  lowCounts.resize(lowVertexIndex);
  auto highCounts = std::vector<uint32_t> {};
  highCounts.resize(highVertexIndex);
  auto lowLengths = std::vector<float> {};
  lowLengths.resize(lowVertexIndex);
  auto highLengths = std::vector<float> {};
  highLengths.resize(highVertexIndex);
  auto lowDeltas = std::vector<F_POINT> {};
  lowDeltas.resize(lowVertexIndex);
  auto highDeltas = std::vector<F_POINT> {};
  highDeltas.resize(highVertexIndex);
  auto lowSteps = std::vector<F_POINT> {};
  lowSteps.resize(lowVertexIndex);
  auto highSteps = std::vector<F_POINT> {};
  highSteps.resize(highVertexIndex);
  auto lowVertices = std::vector<F_POINT> {};
  lowVertices.resize(lowVertexIndex);
  auto highVertices = std::vector<F_POINT> {};
  highVertices.resize(highVertexIndex);
  auto lowLength = 0.0F;
  Instance->oSequence.clear();
  auto lowIndex = 0U;
  {
	// setup the low side of the contours
	auto itVertex = wrap::next(itFirstVertex, lowVertexIndex);
	for (auto iVertex = lowVertexIndex; iVertex != 0; --iVertex) {
	  lowVertices[lowIndex] = *itVertex;
	  auto itPreviousVertex = wrap::next(itVertex, -1);
	  lowDeltas[lowIndex] =
	      F_POINT {itPreviousVertex->x - itVertex->x, itPreviousVertex->y - itVertex->y};
	  lowLengths[lowIndex] = std::hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
	  lowLength += lowLengths[lowIndex];
	  ++lowIndex;
	  --itVertex;
	}
  }
  auto const selectedVertexCount = finish - start;
  auto       polyLines           = std::vector<P_VEC> {};
  polyLines.resize(selectedVertexCount);
  {
	// setup the polyLines
	auto selind   = 0U;
	auto itVertex = wrap::next(itFirstVertex, start + 1);
	for (auto iVertex = start + 1U; iVertex <= finish; ++iVertex) {
	  auto const delta = F_POINT {itVertex->x - itSelectedVertex->x, itVertex->y - itSelectedVertex->y};
	  polyLines[selind] =
	      P_VEC {.angle = atan2(delta.y, delta.x), .length = std::hypot(delta.x, delta.y)};
	  ++selind;
	  ++itVertex;
	}
  }
  auto highIndex  = 0U;
  auto highLength = 0.0F;
  {
	// setup the high side of the contours
	auto itFinishVertex = wrap::next(itFirstVertex, finish);
	auto itNextVertex   = wrap::next(itFinishVertex, 1);
	for (auto iVertex = finish; iVertex < form.vertexCount - 1U; ++iVertex) {
	  highVertices[highIndex] = *itFinishVertex;
	  highDeltas[highIndex] =
	      F_POINT {itNextVertex->x - itFinishVertex->x, itNextVertex->y - itFinishVertex->y};
	  highLengths[highIndex] = std::hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
	  highLength += highLengths[highIndex];
	  ++highIndex;
	  ++itFinishVertex;
	  ++itNextVertex;
	}
  }
  auto lowSpacing  = form.fillSpacing;
  auto highSpacing = lowSpacing;
  // adjust the spacing depending on the lengths
  if (highLength < lowLength) {
	highSpacing = form.fillSpacing * highLength / lowLength;
  }
  else {
	lowSpacing = form.fillSpacing * lowLength / highLength;
  }
  // setup the counts and steps
  for (auto iVertex = 0U; iVertex < lowVertexIndex; ++iVertex) {
	lowCounts[iVertex] = wrap::round<uint32_t>(lowLengths[iVertex] / lowSpacing);
	lowSteps[iVertex]  = F_POINT {lowDeltas[iVertex].x / wrap::toFloat(lowCounts[iVertex]),
                                 lowDeltas[iVertex].y / wrap::toFloat(lowCounts[iVertex])};
  }
  for (auto iVertex = 0U; iVertex < highVertexIndex; ++iVertex) {
	highCounts[iVertex] = wrap::round<uint32_t>(highLengths[iVertex] / highSpacing);
	highSteps[iVertex]  = F_POINT {highDeltas[iVertex].x / wrap::toFloat(highCounts[iVertex]),
                                  highDeltas[iVertex].y / wrap::toFloat(highCounts[iVertex])};
  }
  // setup the indices and counts
  lowIndex = highIndex = 0;
  Instance->stateMap.reset(StateFlag::FILDIR);
  auto lowCount       = 0U;
  auto highCount      = 0U;
  auto itFinishVertex = wrap::next(itFirstVertex, finish);
  auto itStartVertex  = wrap::next(itFirstVertex, start);
  auto delta = F_POINT {itFinishVertex->x - itStartVertex->x, itFinishVertex->y - itStartVertex->y};
  auto const reference =
      P_VEC {.angle = std::atan2(delta.y, delta.x), .length = std::hypot(delta.x, delta.y)};
  auto lowStep   = F_POINT {};
  auto lowPoint  = F_POINT {};
  auto highStep  = F_POINT {};
  auto highPoint = F_POINT {};
  // build the contours
  while (lowCount != 0U || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
	if (lowCount != 0U) {
	  --lowCount;
	}
	else {
	  if (lowIndex < lowVertexIndex) {
		lowCount = lowCounts[lowIndex];
		lowStep  = lowSteps[lowIndex];
		lowPoint = lowVertices[lowIndex];
		++lowIndex;
	  }
	}
	if (highCount != 0U) {
	  --highCount;
	}
	else {
	  if (highIndex < highVertexIndex) {
		highCount = highCounts[highIndex];
		highStep  = highSteps[highIndex];
		highPoint = highVertices[highIndex];
		++highIndex;
	  }
	}
	delta = F_POINT {highPoint.x - lowPoint.x, highPoint.y - lowPoint.y};
	// check if the reference length is too long
	if (constexpr auto REFFACT = 0.9F; // reduction factor for the reference
	    reference.length > REFFACT * LineSpacing) {
	  // and if not, create the sequence for the contour
	  auto const polyLine =
	      P_VEC {.angle = std::atan2(delta.y, delta.x), .length = std::hypot(delta.x, delta.y)};
	  if (auto const polyDiff = P_VEC {.angle  = polyLine.angle - reference.angle,
	                                   .length = polyLine.length / reference.length};
	      Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
		Instance->oSequence.emplace_back(lowPoint);
		for (auto iVertex = 0U; iVertex < selectedVertexCount - 1; ++iVertex) {
		  auto const length = polyLines[iVertex].length * polyDiff.length;
		  auto const angle  = polyLines[iVertex].angle + polyDiff.angle;
		  Instance->oSequence.emplace_back(lowPoint.x + (cos(angle) * length),
		                                   lowPoint.y + (sin(angle) * length));
		}
	  }
	  else {
		Instance->oSequence.emplace_back(highPoint);
		for (auto iVertex = selectedVertexCount - 1U; iVertex != 0; --iVertex) {
		  auto const prevVertex = iVertex - 1U;
		  auto const length     = polyLines[prevVertex].length * polyDiff.length;
		  auto const angle      = polyLines[prevVertex].angle + polyDiff.angle;
		  Instance->oSequence.emplace_back(lowPoint.x + (cos(angle) * length),
		                                   lowPoint.y + (sin(angle) * length));
		}
	  }
	}
	lowPoint += lowStep;
	highPoint += highStep;
  }
  if (Instance->stateMap.test(StateFlag::FILDIR)) {
	Instance->oSequence.push_back(*itFirstVertex);
  }
  else {
	auto itLastVertex = wrap::next(itFirstVertex, form.vertexCount - 1U);
	Instance->oSequence.push_back(*itLastVertex);
  }
  form.stitchLength = std::max(form.stitchLength, MinStitchLength);
}

void duflt(float& formOffset, std::vector<F_POINT>& currentFormVertices) {
  auto const leftEdge =
      std::ranges::min_element(currentFormVertices, [](F_POINT const& first, F_POINT const& second) {
	    return first.x < second.x;
      })->x;
  if (leftEdge >= ClipRectSize.cx) {
	Instance->stateMap.reset(StateFlag::WASNEG);
	return;
  }
  Instance->stateMap.set(StateFlag::WASNEG);
  constexpr auto SAFOFFST = 0.01F; // factor to ensure that formOffset is not 0
  formOffset              = ClipRectSize.cx + fabs(leftEdge) + SAFOFFST;
  for (auto& vertex : currentFormVertices) {
	vertex.x += formOffset;
  }
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  form.rectangle.left += formOffset;
  form.rectangle.right += formOffset;
}

auto leftsid(std::vector<F_POINT> const& currentFormVertices) noexcept -> uint32_t {
  auto leftVertex = 0U;
  auto iVertex    = 0U;
  auto minimumX   = BIGFLOAT;
  for (auto const& vertex : currentFormVertices) {
	if (vertex.x < minimumX) {
	  minimumX   = vertex.x;
	  leftVertex = iVertex;
	}
	++iVertex;
  }
  return leftVertex;
}

auto vclpComp(V_CLP_X const& vclpx1, V_CLP_X const& vclpx2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (vclpx1.segment < vclpx2.segment) {
	return true;
  }
  if (vclpx2.segment < vclpx1.segment) {
	return false;
  }
  if (vclpx1.vertex < vclpx2.vertex) {
	return true;
  }
  if (vclpx2.vertex < vclpx1.vertex) {
	return false;
  }
  return false;
}

auto isin(FRM_HEAD const&             form,
          std::vector<V_CLP_X> const& regionCrossingData,
          float const                 xCoordinate,
          float const                 yCoordinate,
          uint32_t const              regionCrossingStart,
          uint32_t const              regionCrossingEnd,
          F_RECTANGLE const&          boundingRect,
          std::vector<F_POINT> const& currentFormVertices) noexcept(!std::is_same_v<size_t, uint32_t>) -> bool {
  auto count = 0U;
  auto point = F_POINT {};
  if (xCoordinate < boundingRect.left) {
	return false;
  }
  if (yCoordinate > boundingRect.top) {
	return false;
  }
  if (xCoordinate > boundingRect.right) {
	return false;
  }
  if (yCoordinate < boundingRect.bottom) {
	return false;
  }
  for (auto iRegion = regionCrossingStart; iRegion < regionCrossingEnd; ++iRegion) {
	auto const iVertex       = regionCrossingData[iRegion].vertex;
	auto const itStartVertex = wrap::next(currentFormVertices.cbegin(), iVertex);
	auto const itEndVertex   = wrap::next(currentFormVertices.cbegin(), form::nxt(form, iVertex));
	if (!projv(xCoordinate, *itStartVertex, *itEndVertex, point)) {
	  continue;
	}
	if (point.y <= yCoordinate) {
	  continue;
	}
	if (!util::closeEnough(itStartVertex->x, xCoordinate) && !util::closeEnough(itEndVertex->x, xCoordinate)) {
	  ++count;
	  continue;
	}
	if (itStartVertex->x < itEndVertex->x) {
	  if (!util::closeEnough(itEndVertex->x, xCoordinate)) {
		++count;
	  }
	  continue;
	}
	if (!util::closeEnough(itStartVertex->x, xCoordinate)) {
	  ++count;
	}
  }
  return (count & 1U) != 0U;
}

void inspnt(std::vector<CLIP_PNT>& clipStitchPoints) {
  auto const clipStitchPoint =
      CLIP_PNT {.x = clipStitchPoints.back().x, .y = clipStitchPoints.back().y, .vertexIndex = 0, .flag = 0};
  clipStitchPoints.back().x =
      wrap::midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2U].x);
  clipStitchPoints.back().y =
      wrap::midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2U].y);
  clipStitchPoints.back().flag = 1;
  clipStitchPoints.push_back(clipStitchPoint);
}

auto isect(uint32_t const vertex0,
           uint32_t const vertex1,
           F_POINT&       intersection,
           float&         length,
           F_POINT const& lineSegmentStart,
           F_POINT const& lineSegmentEnd,
           std::vector<F_POINT> const& currentFormVertices) noexcept(!std::is_same_v<size_t, uint32_t>) -> bool {
  auto const delta =
      F_POINT {(lineSegmentEnd.x - lineSegmentStart.x), (lineSegmentEnd.y - lineSegmentStart.y)};
  auto       tempIntersection = F_POINT {};
  auto       flag             = false;
  auto const itVertexZero     = wrap::next(currentFormVertices.cbegin(), vertex0);
  if (auto const itVertexOne = wrap::next(currentFormVertices.cbegin(), vertex1);
      delta.x != 0.0F && delta.y != 0.0F) {
	flag = proj(lineSegmentStart, delta.y / delta.x, *itVertexZero, *itVertexOne, tempIntersection);
  }
  else {
	if (delta.y != 0.0F) {
	  flag = projv(lineSegmentStart.x, *itVertexZero, *itVertexOne, tempIntersection);
	}
	else {
	  if (delta.x != 0.0F) {
		flag = projh(lineSegmentStart.y, *itVertexZero, *itVertexOne, tempIntersection);
	  }
	  else if (util::closeEnough(itVertexZero->y, lineSegmentStart.y) &&
	           util::closeEnough(itVertexOne->y, lineSegmentStart.y)) {
		auto left  = itVertexZero->x;
		auto right = itVertexOne->x;
		if (itVertexZero->x > itVertexOne->x) {
		  std::swap(left, right);
		}
		if (lineSegmentStart.x > left && lineSegmentStart.x < right) {
		  tempIntersection = lineSegmentStart;
		  flag             = true;
		}
	  }
	}
  }
  if (flag) {
	if (abs(tempIntersection.x) < TNYFLOAT) {
	  tempIntersection.x = 0.0F;
	}
	if (abs(tempIntersection.y) < TNYFLOAT) {
	  tempIntersection.y = 0.0F;
	}
	intersection = tempIntersection;
	length =
	    std::hypot(tempIntersection.x - lineSegmentStart.x, tempIntersection.y - lineSegmentStart.y);
	// ToDo - should length be determined from start or end?
	//	 hypot(tempIntersection.x-lineSegmentEnd.x,tempIntersection.y-lineSegmentEnd.y);
  }
  return flag;
}

auto clipComp(const gsl::not_null<CLIP_SORT const*> arg1,
              const gsl::not_null<CLIP_SORT const*> arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  auto const local1 = arg1->segmentLength;
  auto const local2 = arg2->segmentLength;
  if (local1 < local2) {
	return true;
  }
  if (local2 < local1) {
	return false;
  }
  return false;
}

void mvpclp(std::vector<CLIP_SORT*>& arrayOfClipIntersectData, uint32_t const destination, uint32_t const source) noexcept {
  if (destination != source) {
	arrayOfClipIntersectData[destination] = arrayOfClipIntersectData[source];
  }
}

auto insect(FRM_HEAD const&             form,
            std::vector<CLIP_SORT>&     clipIntersectData,
            std::vector<V_CLP_X> const& regionCrossingData,
            std::vector<CLIP_SORT*>&    arrayOfClipIntersectData,
            uint32_t const              regionCrossingStart,
            uint32_t const              regionCrossingEnd,
            F_POINT const&              lineSegmentStart,
            F_POINT const&              lineSegmentEnd,
            std::vector<F_POINT> const& currentFormVertices) -> uint32_t {
  auto iIntersection = 0U;
  auto count         = 0U;
  arrayOfClipIntersectData.clear();
  for (auto iRegions = regionCrossingStart; iRegions < regionCrossingEnd; ++iRegions) {
	auto const currentVertex = regionCrossingData[iRegions].vertex;
	if (auto const nextVertex = form::nxt(form, currentVertex);
	    isect(currentVertex,
	          nextVertex,
	          clipIntersectData[iIntersection].point,
	          clipIntersectData[iIntersection].sideLength,
	          lineSegmentStart,
	          lineSegmentEnd,
	          currentFormVertices)) {
	  auto const hFlag  = lineSegmentEnd.x > lineSegmentStart.x;
	  auto const vFlag  = lineSegmentEnd.y > lineSegmentStart.y;
	  auto const left   = hFlag ? lineSegmentStart.x : lineSegmentEnd.x;
	  auto const right  = hFlag ? lineSegmentEnd.x : lineSegmentStart.x;
	  auto const top    = vFlag ? lineSegmentEnd.y : lineSegmentStart.y;
	  auto const bottom = vFlag ? lineSegmentStart.y : lineSegmentEnd.y;
	  if (auto const& intersection = clipIntersectData[iIntersection].point;
	      intersection.x >= left && intersection.x <= right && intersection.y >= bottom &&
	      intersection.y <= top) {
		clipIntersectData[iIntersection].segmentLength =
		    std::hypot(clipIntersectData[iIntersection].point.x - lineSegmentStart.x,
		               clipIntersectData[iIntersection].point.y - lineSegmentStart.y);
		clipIntersectData[iIntersection].vertexIndex = currentVertex;
		arrayOfClipIntersectData.push_back(&clipIntersectData[iIntersection]);
		++iIntersection;
		++count;
	  }
	}
  }
  if (count > 1) {
	std::ranges::sort(arrayOfClipIntersectData, clipComp);
	auto iDestination = 1U;
	for (auto iIntersect = 0U; iIntersect < count - 1U; ++iIntersect) {
	  if (fabs(arrayOfClipIntersectData[iIntersect]->segmentLength -
	           arrayOfClipIntersectData[wrap::toSize(iIntersect) + 1U]->segmentLength) > TNYFLOAT) {
		mvpclp(arrayOfClipIntersectData, iDestination++, iIntersect + 1U);
	  }
	}
	count = iDestination;
  }
  return count;
}

auto getlen(std::vector<CLIP_PNT>&    clipStitchPoints,
            std::vector<float> const& lengths,
            uint32_t const            iPoint,
            std::vector<F_POINT> const& currentFormVertices) noexcept(!std::is_same_v<ptrdiff_t, int>) -> float {
  clipStitchPoints[iPoint].vertexIndex %= currentFormVertices.size();
  auto const itVertex = wrap::next(currentFormVertices.cbegin(), clipStitchPoints[iPoint].vertexIndex);
  return lengths[clipStitchPoints[iPoint].vertexIndex] +
         std::hypot(itVertex->x - clipStitchPoints[iPoint].x, itVertex->y - clipStitchPoints[iPoint].y);
}

auto clpnseg(std::vector<CLIP_PNT>&      clipStitchPoints,
             std::vector<CLIP_SEG>&      clipSegments,
             std::vector<float> const&   lengths,
             uint32_t const              start,
             uint32_t const              finish,
             std::vector<F_POINT> const& currentFormVertices) -> uint32_t {
  auto const clipSegment =
      CLIP_SEG {.start       = start,
                .beginLength = getlen(clipStitchPoints, lengths, start, currentFormVertices),
                .beginIndex  = 0U,
                .asid        = clipStitchPoints[start].vertexIndex,
                .finish      = finish,
                .endLength   = getlen(clipStitchPoints, lengths, finish, currentFormVertices),
                .endIndex    = 0U,
                .zsid        = clipStitchPoints[finish].vertexIndex,
                .dun         = 0};
  clipSegments.push_back(clipSegment);
  return finish + 1U;
}

auto lenComp(LEN_INFO const& arg1, LEN_INFO const& arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (arg1.length < arg2.length) {
	return true;
  }
  if (arg2.length < arg1.length) {
	return false;
  }

  return false;
}

void ritseg(FRM_HEAD const&              form,
            std::vector<CLIP_PNT> const& clipStitchPoints,
            std::vector<CLIP_SEG>&       clipSegments,
            uint32_t const               currentSegmentIndex,
            uint32_t&                    clipIntersectSide,
            std::vector<F_POINT> const&  currentFormVertices) {
  auto isPointedEnd = true;
  if ((form.extendedAttribute & AT_SQR) != 0U) {
	isPointedEnd = false;
  }
  if (Instance->stateMap.test(StateFlag::FILDIR)) {
	auto iPoint = clipSegments[currentSegmentIndex].start;
	if (Instance->stateMap.test(StateFlag::TXFIL) && isPointedEnd) {
	  ++iPoint;
	}
	chksid(form, clipSegments[currentSegmentIndex].asid, clipIntersectSide, currentFormVertices);
	while (iPoint <= clipSegments[currentSegmentIndex].finish) {
	  Instance->oSequence.emplace_back(clipStitchPoints[iPoint].x, clipStitchPoints[iPoint].y);
	  ++iPoint;
	}
	clipIntersectSide = clipSegments[currentSegmentIndex].zsid;
  }
  else {
	auto iPoint = clipSegments[currentSegmentIndex].finish;
	if (Instance->stateMap.test(StateFlag::TXFIL) && isPointedEnd) {
	  --iPoint;
	}
	chksid(form, clipSegments[currentSegmentIndex].zsid, clipIntersectSide, currentFormVertices);
	if (clipSegments[currentSegmentIndex].start != 0U) {
	  while (iPoint >= clipSegments[currentSegmentIndex].start) {
		Instance->oSequence.emplace_back(clipStitchPoints[iPoint].x, clipStitchPoints[iPoint].y);
		--iPoint;
	  }
	}
	else {
	  // while (iPoint < clipSegments[currentSegmentIndex].start) { // This line makes no sense. Maybe the replacement does?
	  while (iPoint-- != 0U) {
		Instance->oSequence.emplace_back(clipStitchPoints[iPoint].x, clipStitchPoints[iPoint].y);
	  }
	}
	clipIntersectSide = clipSegments[currentSegmentIndex].asid;
  }
  clipSegments[currentSegmentIndex].dun = 1;
}

auto clpnxt(std::vector<CLIP_SEG> const& clipSegments,
            std::vector<LEN_INFO> const& sortedLengths,
            uint32_t const               sind,
            uint32_t&                    outIndex) -> bool {
  auto       index        = 1U;
  auto const indexDoubled = wrap::toUnsigned(clipSegments.size()) * 2U;
  Instance->stateMap.reset(StateFlag::FILDIR);
  while (index < clipSegments.size()) {
	if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
	  outIndex = (sind + index) % indexDoubled;
	  if (clipSegments[sortedLengths[outIndex].index].dun == 0) {
		return false;
	  }
	  ++index;
	  continue;
	}
	outIndex = (sind + indexDoubled - index) % indexDoubled;
	if (clipSegments[sortedLengths[outIndex].index].dun == 0) {
	  return false;
	}
  }
  return true;
}

auto nucseg(std::vector<CLIP_SEG> const& clipSegments,
            std::vector<LEN_INFO> const& sortedLengths,
            uint32_t&                    currentSegmentIndex) -> bool {
  auto const index    = Instance->stateMap.test(StateFlag::FILDIR)
                            ? clipSegments[currentSegmentIndex].endIndex
                            : clipSegments[currentSegmentIndex].beginIndex;
  auto       outIndex = 0U;
  if (clpnxt(clipSegments, sortedLengths, index, outIndex)) {
	return false;
  }
  if (sortedLengths[outIndex].isEnd) {
	Instance->stateMap.reset(StateFlag::FILDIR);
  }
  else {
	Instance->stateMap.set(StateFlag::FILDIR);
  }
  currentSegmentIndex = sortedLengths[outIndex].index;
  return true;
}

auto vscmp(uint32_t const index1, uint32_t const index2) noexcept -> bool {
  return util::closeEnough(Instance->oSequence.operator[](index1).x,
                           Instance->oSequence.operator[](index2).x)
             ? !util::closeEnough(Instance->oSequence.operator[](index1).y,
                                  Instance->oSequence.operator[](index2).y)
             : true;
}

void clpcon(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& currentFormVertices) {
  auto const currentVertexCount = currentFormVertices.size();
  auto       formOffset         = 0.0F;
  duflt(formOffset, currentFormVertices);
  auto clipWidth = ClipRectSize.cx + form.fillSpacing;
  if (Instance->stateMap.test(StateFlag::ISUND)) {
	clipWidth = form.underlaySpacing;
  }
  auto clipNegative = false;
  if (form.fillSpacing < 0) {
	clipNegative = true;
  }
  clipWidth = std::max(clipWidth, CLPMINVT);
  if (Instance->stateMap.test(StateFlag::TXFIL)) {
	if (!Instance->texturePointsBuffer.empty() &&
	    form.texture.index + form.texture.count <=
	        gsl::narrow<decltype(form.texture.index)>(Instance->texturePointsBuffer.size())) {
	  clipWidth = form.fillSpacing;
	}
	else {
	  return;
	}
  }
  auto lengths = std::vector<float> {};
  lengths.resize(currentVertexCount);
  auto clipSideLengths = std::vector<float> {}; // lengths of form sides for clipboard fill
  clipSideLengths.resize(currentVertexCount);
  auto clipIntersectData = std::vector<CLIP_SORT> {}; // intersect points for clipboard fill
  clipIntersectData.resize(currentVertexCount);
  auto arrayOfClipIntersectData = std::vector<CLIP_SORT*> {};
  arrayOfClipIntersectData.reserve(currentVertexCount);
  auto vertex        = leftsid(currentFormVertices);
  auto totalLength   = 0.0F;
  vertex             = form::nxt(form, vertex);
  auto itFirstVertex = currentFormVertices.begin();
  for (auto iVertex = 0U; iVertex <= currentVertexCount; ++iVertex) {
	auto const itVertex     = wrap::next(itFirstVertex, vertex);
	auto const nextVertex   = form::nxt(form, vertex);
	auto const itNextVertex = wrap::next(itFirstVertex, nextVertex);
	lengths[vertex]         = totalLength;
	clipSideLengths[vertex] = std::hypot(itNextVertex->x - itVertex->x, itNextVertex->y - itVertex->y);
	totalLength += clipSideLengths[vertex];
	vertex = nextVertex;
  }
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;
  for (auto const& itVertex : currentFormVertices) {
	minX = std::min(minX, itVertex.x);
	minY = std::min(minY, itVertex.y);
	maxX = std::max(maxX, itVertex.x);
	maxY = std::max(maxY, itVertex.y);
  }
  auto clipGrid     = RECT {wrap::floor<int32_t>(minX / clipWidth),
                        wrap::ceil<int32_t>((maxY / ClipRectSize.cy) + 1.0F) + 2,
                        wrap::ceil<int32_t>(maxX / clipWidth),
                        wrap::floor<int32_t>((minY / ClipRectSize.cy) - 1.0F)};
  auto boundingRect = F_RECTANGLE {minX, maxY, maxX, minY};

  auto negativeOffset = 0L;
  auto clipGridOffset = 0U;
  if (form.wordParam > 1) {
	clipGridOffset = form.wordParam;
  }
  if (clipGridOffset != 0U) {
	++clipGrid.top;
	if (form.fillSpacing < 0) {
	  --clipGrid.bottom;
	  clipGrid.left -= std::lround(ClipRectSize.cx / clipWidth);
	  clipGrid.right += std::lround(ClipRectSize.cx / clipWidth);
	}
  }
  if (clipNegative && clipGridOffset == 0U) {
	clipGrid.left -= std::lround(ClipRectSize.cx / clipWidth);
  }
  if (clipGrid.bottom < 0) {
	negativeOffset = 1 - clipGrid.bottom;
	clipGrid.bottom += negativeOffset;
	clipGrid.top += negativeOffset;
	auto const formNegativeOffset = ClipRectSize.cy * wrap::toFloat(negativeOffset);

	auto vertIt = itFirstVertex; // intentional copy
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vertIt->y += formNegativeOffset;
	  ++vertIt;
	}
	boundingRect.top += formNegativeOffset;
	boundingRect.bottom += formNegativeOffset;
  }
  auto regionCrossingData = std::vector<V_CLP_X> {}; // region crossing data for vertical clipboard fills
  // reserve a little more than we need. Determined empirically
  constexpr auto RESFACT = size_t {5U}; // use this as the reservation factor
  regionCrossingData.reserve(wrap::toSize(clipGrid.right - clipGrid.left) * RESFACT);
  auto vCurr = itFirstVertex; // intentional copy
  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	auto const itNextVertex = wrap::next(itFirstVertex, form::nxt(form, iVertex));
	auto       start        = wrap::floor<uint32_t>(vCurr->x / clipWidth);
	auto       finish       = wrap::floor<uint32_t>(itNextVertex->x / clipWidth);
	++vCurr;
	if (start > finish) {
	  std::swap(start, finish);
	}
	if (form.fillSpacing < 0) {
	  finish += wrap::round<decltype(finish)>(ClipRectSize.cx / clipWidth);
	}
	finish = std::min(finish, gsl::narrow<uint32_t>(clipGrid.right));
	if (clipNegative) {
	  start -= wrap::round<uint32_t>(ClipRectSize.cx / clipWidth);
	}
	for (auto iSegment = start; iSegment <= finish; ++iSegment) {
	  regionCrossingData.push_back(V_CLP_X {.segment = iSegment, .vertex = iVertex});
	}
  }
  std::ranges::sort(regionCrossingData, vclpComp);
  auto iclpx = std::vector<uint32_t> {};
  iclpx.reserve(regionCrossingData.size());
  auto regionSegment = regionCrossingData[0].segment;
  iclpx.push_back(0);
  for (auto iSegment = 1U; iSegment < wrap::toUnsigned(regionCrossingData.size()); ++iSegment) {
	if (regionCrossingData[iSegment].segment != regionSegment) {
	  iclpx.push_back(iSegment);
	  regionSegment = regionCrossingData[iSegment].segment;
	}
  }
  iclpx.push_back(wrap::toUnsigned(regionCrossingData.size()));
  auto clipStitchPoints = std::vector<CLIP_PNT> {};
  // Reserve some memory, but probably not enough
  constexpr auto CSPRES = size_t {1000U};
  clipStitchPoints.reserve(CSPRES);
  auto pasteLocation = F_POINT {};
  auto texture       = Instance->texturePointsBuffer.begin();
  if (auto iclpxSize = wrap::toUnsigned(iclpx.size()); iclpxSize != 0U) {
	--iclpxSize;
	bool  breakFlag  = false;
	auto& clipBuffer = Instance->clipBuffer;

	for (auto iRegion = 0U; iRegion < iclpxSize; ++iRegion) {
	  auto regionCrossingStart = iclpx[iRegion];
	  auto regionCrossingEnd   = iclpx[wrap::toSize(iRegion) + 1U];
	  pasteLocation.x         = clipWidth * (wrap::toFloat(iRegion) + wrap::toFloat(clipGrid.left));
	  auto clipVerticalOffset = 0.0F;
	  auto lineSegmentStart   = F_POINT {}; // vertical clipboard line segment start
	  auto lineSegmentEnd     = F_POINT {}; // vertical clipboard line segment end
	  auto clipStitchCount    = wrap::toUnsigned(clipBuffer.size());
	  if (Instance->stateMap.test(StateFlag::TXFIL)) {
		auto const textureLine =
		    (iRegion + wrap::toUnsigned(clipGrid.left)) % wrap::toUnsigned(form.texture.lines);
		clipStitchCount = wrap::toUnsigned(textureSegments[textureLine].stitchCount);

		texture = wrap::next(Instance->texturePointsBuffer.begin(),
		                     form.texture.index + textureSegments[textureLine].line);

		lineSegmentStart.x = pasteLocation.x;
		if (form.txof != 0.0F) {
		  auto const lineOffset =
		      (gsl::narrow_cast<float>(iRegion) + gsl::narrow_cast<float>(clipGrid.left)) /
		      gsl::narrow_cast<float>(form.texture.lines);
		  clipVerticalOffset = fmod(form.txof * lineOffset, form.texture.height);
		}
	  }
	  else {
		if (clipGridOffset != 0U) {
		  clipVerticalOffset = wrap::toFloat(iRegion % clipGridOffset) /
		                       (wrap::toFloat(clipGridOffset) * ClipRectSize.cy);
		}
		lineSegmentStart.x = pasteLocation.x + clipBuffer.front().x;
	  }
	  lineSegmentStart.y = wrap::toFloat(clipGrid.bottom) * ClipRectSize.cy;
	  if (clipGridOffset != 0U) {
		clipVerticalOffset =
		    wrap::toFloat(iRegion % clipGridOffset) / (wrap::toFloat(clipGridOffset) * ClipRectSize.cy);
	  }
	  for (auto iVerticalGrid = clipGrid.bottom; iVerticalGrid < clipGrid.top; ++iVerticalGrid) {
		pasteLocation.y = wrap::toFloat(iVerticalGrid) * ClipRectSize.cy - clipVerticalOffset;
		if (!clipBuffer.empty()) {
		  lineSegmentEnd = pasteLocation + clipBuffer.front();
		}
		else {
		  lineSegmentEnd = pasteLocation;
		}
		if (clipStitchPoints.empty()) {
		  lineSegmentStart = lineSegmentEnd;
		}
		auto textureIt = texture; // intentional copy
		for (auto iStitch = 0U; iStitch < clipStitchCount; ++iStitch) {
		  if (Instance->stateMap.test(StateFlag::TXFIL)) {
			lineSegmentEnd = F_POINT {pasteLocation.x, pasteLocation.y + textureIt->y};
			++textureIt;
		  }
		  else {
			lineSegmentEnd = F_POINT {pasteLocation.x + clipBuffer.operator[](iStitch).x,
			                          pasteLocation.y + clipBuffer.operator[](iStitch).y};
		  }

		  clipStitchPoints.push_back(
		      CLIP_PNT {.x = lineSegmentStart.x, .y = lineSegmentStart.y, .vertexIndex = 0, .flag = 0});
		  if (isin(form, regionCrossingData, lineSegmentStart.x, lineSegmentStart.y, regionCrossingStart, regionCrossingEnd, boundingRect, currentFormVertices)) {
			if (clipStitchPoints.size() > 1 && clipStitchPoints[clipStitchPoints.size() - 1U].flag == 2) {
			  inspnt(clipStitchPoints);
			}
			clipStitchPoints.back().flag = 0;
		  }
		  else {
			if (clipStitchPoints.size() > 1 && clipStitchPoints[clipStitchPoints.size() - 1U].flag == 0U) {
			  inspnt(clipStitchPoints);
			}
			clipStitchPoints.back().flag = 2;
		  }
		  if (auto const count = insect(form,
		                                clipIntersectData,
		                                regionCrossingData,
		                                arrayOfClipIntersectData,
		                                regionCrossingStart,
		                                regionCrossingEnd,
		                                lineSegmentStart,
		                                lineSegmentEnd,
		                                currentFormVertices);
		      count != 0U) {
			auto constexpr CLIPMAXSIZE = size_t {MAXITEMS << 2U};
			for (auto index = 0U; index < count; ++index) {
			  clipStitchPoints.push_back({.x = arrayOfClipIntersectData[index]->point.x,
			                              .y = arrayOfClipIntersectData[index]->point.y,
			                              .vertexIndex = arrayOfClipIntersectData[index]->vertexIndex,
			                              .flag = 1});
			  if (clipStitchPoints.size() > CLIPMAXSIZE) {
				breakFlag = true;
				break;
			  }
			}
			if (breakFlag) {
			  break;
			}
		  }
		  lineSegmentStart = lineSegmentEnd;
		}
		if (breakFlag) {
		  break;
		}
	  }
	  if (breakFlag) {
		break;
	  }
	  if (!clipStitchPoints.empty()) {
		clipStitchPoints[clipStitchPoints.size() - 1U].flag = 2;
	  }
	}
  }
  if (!clipStitchPoints.empty()) {
	clipStitchPoints[clipStitchPoints.size() - 1U].flag = 2;
  }
  if (negativeOffset != 0) {
	auto const formNegativeOffset = wrap::toFloat(negativeOffset) * ClipRectSize.cy;
	for (auto& clipStitchPoint : clipStitchPoints) {
	  clipStitchPoint.y -= formNegativeOffset;
	}
	auto vOffset = itFirstVertex; // intentional copy
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vOffset->y -= formNegativeOffset;
	  ++vOffset;
	}
  }

  auto clipSegments = std::vector<CLIP_SEG> {};

#if (CLPVU == 0) || (CLPVU == 2)
  Instance->stateMap.reset(StateFlag::FILDIR);
  if (auto endPoint = wrap::toUnsigned(clipStitchPoints.size()); endPoint != 0U) {
	--endPoint;
	// reserve a reasonable amount but not the full amount potentially required
	constexpr auto CSRES = uint32_t {8U};
	clipSegments.reserve(endPoint / CSRES);
	auto previousPoint = 0U;
	for (auto iPoint = 0U; iPoint < endPoint; ++iPoint) {
	  switch (clipStitchPoints[iPoint].flag) {
		case 0: { // inside
		  Instance->stateMap.set(StateFlag::FILDIR);
		  break;
		}
		case 1: { // line
		  if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
			clpnseg(clipStitchPoints, clipSegments, lengths, previousPoint, iPoint, currentFormVertices);
		  }
		  else {
			previousPoint = iPoint;
		  }
		  break;
		}
		case 2: { // outside
		  Instance->stateMap.reset(StateFlag::FILDIR);
		  break;
		}
		default: {
		  outDebugString(L"default hit in clpcon: flag [{}]\n", clipStitchPoints[iPoint].flag);
		  break;
		}
	  }
	}
  }
  if (clipSegments.empty()) {
	return;
  }
#endif

  auto sortedLengths = std::vector<LEN_INFO> {};
  sortedLengths.reserve(clipSegments.size() * 2);
  for (auto iSegment = 0U; iSegment < wrap::toUnsigned(clipSegments.size()); ++iSegment) {
	sortedLengths.push_back(
	    LEN_INFO {.index = iSegment, .isEnd = false, .length = clipSegments[iSegment].beginLength});
	sortedLengths.push_back(
	    LEN_INFO {.index = iSegment, .isEnd = true, .length = clipSegments[iSegment].endLength});
  }
  std::ranges::sort(sortedLengths, lenComp);
  for (auto iSorted = 0U; iSorted < wrap::toUnsigned(sortedLengths.size()); ++iSorted) {
	if (sortedLengths[iSorted].isEnd) {
	  clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
	}
	else {
	  clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
	}
  }

#if CLPVU == 1

  for (uint32_t iStitch = 0; iStitch < clipStitchPoints.size(); ++iStitch) {
	Instance->stitchBuffer.emplace_back(
	    F_POINT_ATTR {clipStitchPoints[iStitch].x, clipStitchPoints[iStitch].y, 0});
  }
#endif

#if CLPVU == 2

  auto iStitch = 0;
  for (auto iSegment = 0U; iSegment < clipSegments.size(); ++iSegment) {
	for (auto iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish;
	     ++iStitchPoint) {
	  Instance->stitchBuffer.emplace_back(F_POINT_ATTR {
	      clipStitchPoints[iStitchPoint].x, clipStitchPoints[iStitchPoint].y, iSegment & 0xf});
	}
  }

#endif

#if CLPVU == 0

  auto currentSegmentIndex = 0U;
  Instance->stateMap.set(StateFlag::FILDIR);
  Instance->oSequence.clear();
  auto clipIntersectSide = clipSegments[0].asid;
  ritseg(form, clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
  while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
	ritseg(form, clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
  }
  chksid(form, 0, clipIntersectSide, currentFormVertices);
  auto index = 0U;
  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(Instance->oSequence.size()); ++iSequence) {
	if (vscmp(iSequence, index)) {
	  ++index;
	  Instance->oSequence.operator[](index) = Instance->oSequence.operator[](iSequence);
	}
  }
  Instance->oSequence.resize(index);
  if (Instance->stateMap.test(StateFlag::WASNEG)) {
	for (auto& iSequence : Instance->oSequence) {
	  iSequence.x -= formOffset;
	}
	auto vOffset = itFirstVertex; // intentional copy
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vOffset->x -= formOffset;
	  ++vOffset;
	}
	form.rectangle.left -= formOffset;
	form.rectangle.right -= formOffset;
  }
#endif
}

void angout(FRM_HEAD& angledForm) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  if (angledForm.vertexCount == 0U) {
	return;
  }
  auto const spVertices =
      gsl::span(Instance->formVertices).subspan(angledForm.vertexIndex, angledForm.vertexCount);
  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;
  for (auto const& itVertex : spVertices) {
	minX = std::min(minX, itVertex.x);
	minY = std::min(minY, itVertex.y);
	maxX = std::max(maxX, itVertex.x);
	maxY = std::max(maxY, itVertex.y);
  }
  angledForm.rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
}

void horclpfn(std::vector<RNG_COUNT> const& textureSegments, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices) {
  angledForm = Instance->formList.operator[](ClosestFormToCursor);
  auto const rotationCenter = F_POINT {wrap::midl(angledForm.rectangle.right, angledForm.rectangle.left),
                                       wrap::midl(angledForm.rectangle.top, angledForm.rectangle.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*itVertex);
	thred::rotflt(angledFormVertices.back(), PI_FHALF, rotationCenter);
	++itVertex;
  }
  angledForm.vertexIndex = 0;
  angout(angledForm);
  clpcon(angledForm, textureSegments, angledFormVertices);
  rotbak(-PI_FHALF, rotationCenter);
}

auto isclos(std::vector<SMAL_PNT_L> const& lineEndpoints,
            uint32_t                       line0Index,
            uint32_t                       line1Index,
            float const                    gapToClosestRegion) noexcept -> bool {
  auto const low0 = lineEndpoints[line0Index].y - gapToClosestRegion;
  auto const low1 = lineEndpoints[line1Index].y - gapToClosestRegion;
  if (auto const high0 = lineEndpoints[++line0Index].y + gapToClosestRegion; high0 < low1) {
	return false;
  }
  if (auto const high1 = lineEndpoints[++line1Index].y + gapToClosestRegion; high1 < low0) {
	return false;
  }
  return true;
}

auto lnclos(std::vector<uint32_t> const&   groupIndexSequence,
            std::vector<SMAL_PNT_L> const& lineEndpoints,
            uint32_t const                 group0,
            uint32_t const                 line0,
            uint32_t const                 group1,
            uint32_t const                 line1,
            float const gapToClosestRegion) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool {
  auto const lineEndPoint0 = wrap::next(lineEndpoints.begin(), groupIndexSequence[group0]);
  if (group1 > groupIndexSequence.size() - 2U) {
	return false;
  }
  if (group0 == 0) {
	return false;
  }
  auto count0 = (groupIndexSequence[wrap::toSize(group0) + 1U] - groupIndexSequence[group0]) / 2U;
  auto index0 = 0U;
  while (count0 != 0U && lineEndPoint0.operator[](wrap::toPtrdiff(index0)).line != line0) {
	--count0;
	index0 += 2;
  }
  if (count0 == 0U) {
	return false;
  }
  auto const lineEndPoint1 = wrap::next(lineEndpoints.begin(), groupIndexSequence[group1]);
  auto       index1        = 0U;
  auto count1 = (groupIndexSequence[wrap::toSize(group1) + 1U] - groupIndexSequence[group1]) / 2U;
  while (count1 != 0U && lineEndPoint1.operator[](wrap::toPtrdiff(index1)).line != line1) {
	--count1;
	index1 += 2;
  }
  if (count1 != 0U) {
	return isclos(lineEndpoints, groupIndexSequence[group0] + index0, groupIndexSequence[group1] + index1, gapToClosestRegion);
  }
  return false;
}

auto regclos(std::vector<uint32_t> const&   groupIndexSequence,
             std::vector<SMAL_PNT_L> const& lineEndpoints,
             std::vector<uint32_t> const&   sortedLineIndices,
             uint32_t const                 iRegion0,
             uint32_t const                 iRegion1,
             std::vector<REGION> const&     regionsList,
             float const                    gapToClosestRegion,
             uint32_t& nextGroup) noexcept(!std::is_same_v<ptrdiff_t, int>) -> bool {
  auto const  lineStart0Index    = sortedLineIndices[regionsList[iRegion0].start];
  auto const  lineStart1Index    = sortedLineIndices[regionsList[iRegion1].start];
  auto const& lineEndPoint0Start = lineEndpoints[lineStart0Index];
  auto const& lineEndPoint1Start = lineEndpoints[lineStart1Index];
  auto const  group0Start        = lineEndPoint0Start.group;
  auto const  group1Start        = lineEndPoint1Start.group;
  auto const  startFlag          = group0Start > group1Start;
  auto const  groupStart         = startFlag ? group0Start : group1Start;
  auto const  lineStart          = startFlag ? lineEndPoint0Start.line : lineEndPoint1Start.line;
  if (auto const prevLine = startFlag ? lineEndPoint1Start.line : lineEndPoint0Start.line;
      groupStart != 0U &&
      lnclos(groupIndexSequence, lineEndpoints, groupStart - 1, prevLine, groupStart, lineStart, gapToClosestRegion)) {
	nextGroup = groupStart;
	return true;
  }
  auto const  lineEnd0Index    = sortedLineIndices[regionsList[iRegion0].end];
  auto const  lineEnd1Index    = sortedLineIndices[regionsList[iRegion1].end];
  auto const& lineEndPoint0End = lineEndpoints[lineEnd0Index];
  auto const& lineEndPoint1End = lineEndpoints[lineEnd1Index];
  auto const  group1End        = lineEndPoint1End.group;
  auto const  group0End        = lineEndPoint0End.group;
  auto const  endFlag          = group0End < group1End;
  auto const  groupEnd         = endFlag ? group0End : group1End;
  auto const  lineEnd          = endFlag ? lineEndPoint0End.line : lineEndPoint1End.line;
  if (auto const lastLine = endFlag ? lineEndPoint1End.line : lineEndPoint0End.line;
      lnclos(groupIndexSequence, lineEndpoints, groupEnd, lineEnd, groupEnd + 1, lastLine, gapToClosestRegion)) {
	nextGroup = groupEnd;
	return true;
  }
  if ((group0Start > group1Start ? group0Start - group1Start : group1Start - group0Start) < 2) {
	if (isclos(lineEndpoints, lineStart0Index, lineStart1Index, gapToClosestRegion)) {
	  nextGroup = group0Start;
	  return true;
	}
  }
  if ((group0Start > group1End ? group0Start - group1End : group1End - group0Start) < 2) {
	if (isclos(lineEndpoints, lineStart0Index, lineEnd1Index, gapToClosestRegion)) {
	  nextGroup = group0Start;
	  return true;
	}
  }
  if ((group0End > group1Start ? group0End - group1Start : group1Start - group0End) < 2) {
	if (isclos(lineEndpoints, lineEnd0Index, lineStart1Index, gapToClosestRegion)) {
	  nextGroup = group0End;
	  return true;
	}
  }
  if ((group0End > group1End ? group0End - group1End : group1End - group0End) < 2) {
	if (isclos(lineEndpoints, lineEnd0Index, lineEnd1Index, gapToClosestRegion)) {
	  nextGroup = group0End;
	  return true;
	}
  }
  return false;
}

auto unvis(boost::dynamic_bitset<> const& visitedRegions, int32_t& visitedIndex) -> bool {
  auto const regionCount = gsl::narrow<int32_t>(visitedRegions.size());
  for (visitedIndex = 0; visitedIndex < regionCount; ++visitedIndex) {
	if (!visitedRegions[wrap::toSize(visitedIndex)]) {
	  return true;
	}
  }
  return false;
}

auto notdun(std::vector<RG_SEQ>&           tempPath,
            std::vector<R_CON> const&      pathMap,
            std::vector<uint32_t> const&   mapIndexSequence,
            boost::dynamic_bitset<> const& visitedRegions,
            ptrdiff_t const                pathLength,
            uint32_t                       doneRegion,
            uint32_t const                 sequencePathIndex) -> bool {
  auto previousLevel = pathLength;
  if (previousLevel != 0) {
	--previousLevel;
  }
  auto const itRegionPath = wrap::next(tempPath.begin(), sequencePathIndex);
  itRegionPath[0].pcon    = mapIndexSequence[doneRegion];
  itRegionPath[0].count   = gsl::narrow<int32_t>(
      mapIndexSequence[gsl::narrow_cast<size_t>(doneRegion) + 1U] - itRegionPath[0].pcon);
  for (auto iPath = ptrdiff_t {1}; iPath < pathLength; ++iPath) {
	auto const prevPath       = iPath - 1;
	itRegionPath[iPath].pcon  = mapIndexSequence[pathMap[itRegionPath[prevPath].pcon].node];
	itRegionPath[iPath].count = gsl::narrow<int32_t>(
	    mapIndexSequence[gsl::narrow_cast<size_t>(pathMap[itRegionPath[prevPath].pcon].node) + 1] -
	    itRegionPath[iPath].pcon);
  }
  while (visitedRegions[pathMap[itRegionPath[previousLevel].pcon].node]) {
	if (--itRegionPath[previousLevel].count > 0) {
	  ++itRegionPath[previousLevel].pcon;
	  continue;
	}
	auto pivot = previousLevel;
	auto flag  = true;
	while (flag) {
	  if (pivot != 0) {
		--pivot;
	  }
	  else {
		return true;
	  }
	  --itRegionPath[pivot].count;
	  ++itRegionPath[pivot].pcon;
	  if (itRegionPath[pivot].count != 0) {
		flag = false;
	  }
	}
	++pivot;
	while (pivot <= previousLevel) {
	  if (pivot != 0) {
		auto const prevPivot      = pivot - 1;
		itRegionPath[pivot].pcon  = mapIndexSequence[pathMap[itRegionPath[prevPivot].pcon].node];
		itRegionPath[pivot].count = gsl::narrow<int32_t>(
		    mapIndexSequence[wrap::toSize(pathMap[itRegionPath[prevPivot].pcon].node) + 1U] -
		    itRegionPath[pivot].pcon);
	  }
	  else {
		if (--itRegionPath[0].count != 0) {
		  ++itRegionPath[0].pcon;
		}
		else {
		  return true;
		}
	  }
	  ++pivot;
	}
  }
  return false;
}

auto reglen(std::vector<SMAL_PNT_L> const&       lineEndpoints,
            std::vector<uint32_t> const&         sortedLineIndices,
            uint32_t const                       iRegion,
            std::array<F_POINT, SQRCORNS> const& lastRegionCorners,
            std::vector<REGION> const&           regionsList) noexcept -> float {
  std::array<SMAL_PNT_L, SQRCORNS> corners;

  auto index = sortedLineIndices[regionsList[iRegion].start];
  corners[0] = lineEndpoints[index];
  corners[1] = lineEndpoints[++index];
  index      = sortedLineIndices[regionsList[iRegion].end];
  corners[2] = lineEndpoints[index];
  corners[3] = lineEndpoints[++index];

  auto minimumLength = BIGFLOAT;
  for (auto const& corner : lastRegionCorners) {
	for (auto const& point : corners) {
	  auto const deltaX = corner.x - point.x;
	  auto const deltaY = corner.y - point.y;
	  auto const length = (deltaX * deltaX) + (deltaY * deltaY);
	  minimumLength     = std::min(minimumLength, length);
	}
  }
  return std::sqrtf(minimumLength);
}

void nxtrgn(std::vector<RG_SEQ>&           tempPath,
            std::vector<R_CON> const&      pathMap,
            std::vector<uint32_t> const&   mapIndexSequence,
            boost::dynamic_bitset<>&       visitedRegions,
            std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            std::vector<REGION> const&     regionsList,
            uint32_t&                      doneRegion,
            uint32_t const                 pathMapIndex,
            uint32_t&                      sequencePathIndex,
            int32_t const                  visitedIndex) {
  std::array<F_POINT, SQRCORNS> lastRegionCorners;

  auto pathLength = ptrdiff_t {1}; // length of the path to the region
  while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength, doneRegion, sequencePathIndex)) {
	++pathLength;
	auto maxPathLength = gsl::narrow<ptrdiff_t>(tempPath.size() - sequencePathIndex);

	constexpr auto DEPFACT =
	    ptrdiff_t {8}; // tuneable value. Increasing this increases path discovery time exponentially
	maxPathLength = std::min(maxPathLength, DEPFACT);
	outDebugString(L"nxtrgn: pathLength {}\n", pathLength);
	if (pathLength <= maxPathLength) {
	  continue;
	}
	auto lineEndPoint        = sortedLineIndices[regionsList[doneRegion].start];
	lastRegionCorners[0]     = lineEndpoints[lineEndPoint];
	lastRegionCorners[1]     = lineEndpoints[++lineEndPoint];
	lineEndPoint             = sortedLineIndices[regionsList[doneRegion].end];
	lastRegionCorners[2]     = lineEndpoints[lineEndPoint];
	lastRegionCorners[3]     = lineEndpoints[++lineEndPoint];
	auto       newRegion     = 0U;
	auto       minimumLength = BIGFLOAT;
	auto const regionCount   = visitedRegions.size();
	for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	  if (visitedRegions[iRegion]) {
		continue;
	  }
	  auto const length = reglen(lineEndpoints, sortedLineIndices, iRegion, lastRegionCorners, regionsList);
	  if (length >= minimumLength) {
		continue;
	  }
	  minimumLength = length;
	  newRegion     = iRegion;
	}
	tempPath[sequencePathIndex].skp = true;
	for (auto iPath = 0U; iPath < pathMapIndex; ++iPath) {
	  if (pathMap[iPath].node != newRegion) {
		continue;
	  }
	  tempPath[sequencePathIndex++].pcon = iPath;
	  visitedRegions.set(newRegion);
	  doneRegion = newRegion;
	  return;
	}
	tempPath[sequencePathIndex].count  = visitedIndex;
	tempPath[sequencePathIndex++].pcon = MAXDWORD;
	visitedRegions.set(wrap::toSize(visitedIndex));
	doneRegion = wrap::toUnsigned(visitedIndex);
	return;
  }
  auto itRegionPath = wrap::next(tempPath.cbegin(), sequencePathIndex);
  for (auto iPath = ptrdiff_t {}; iPath < pathLength; ++iPath) {
	tempPath[sequencePathIndex].skp    = false;
	tempPath[sequencePathIndex++].pcon = itRegionPath->pcon;
	visitedRegions.set(pathMap[itRegionPath->pcon].node);
	++itRegionPath;
  }
  --itRegionPath;
  doneRegion = pathMap[itRegionPath->pcon].node;
}

void nxtseq(std::vector<F_SEQ>&          sequencePath,
            std::vector<R_CON> const&    pathMap,
            std::vector<uint32_t> const& mapIndexSequence,
            uint32_t const               pathIndex) {
  if (wrap::toSize(pathIndex) + 1U >= sequencePath.size()) {
	sequencePath[pathIndex].nextGroup = 0;
	return;
  }
  unsigned const nextNode = sequencePath[wrap::toSize(pathIndex) + 1U].node;
  unsigned       iPath    = mapIndexSequence[sequencePath[pathIndex].node];
  while (iPath < mapIndexSequence[wrap::toSize(sequencePath[pathIndex].node) + 1U] &&
         pathMap[iPath].node != nextNode) {
	++iPath;
  }
  if (iPath >= pathMap.size()) {
	sequencePath[pathIndex].nextGroup = 0;
	return;
  }
  sequencePath[pathIndex].nextGroup = gsl::narrow<uint16_t>(pathMap[iPath].nextGroup);
}

void brkdun(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t const                 start,
            uint32_t const                 finish,
            std::vector<F_POINT>&          workingFormVertices) {
  auto& bSequence = Instance->bSequence;
  bSequence.emplace_back(
      lineEndpoints[sortedLineIndices[start]].x, lineEndpoints[sortedLineIndices[start]].y, 0);
  bSequence.emplace_back(
      lineEndpoints[sortedLineIndices[finish]].x, lineEndpoints[sortedLineIndices[finish]].y, 0);
  bSequence.emplace_back(
      workingFormVertices.operator[](lineEndpoints[sortedLineIndices[start]].line).x,
      workingFormVertices.operator[](lineEndpoints[sortedLineIndices[start]].line).y,
      0);
  Instance->stateMap.set(StateFlag::BRKFIX);
}

void duseq1(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t const                 sequenceIndex) {
  auto        index     = sortedLineIndices[sequenceIndex];
  auto const& sequence0 = lineEndpoints[index];
  auto const& sequence1 = lineEndpoints[++index];
  Instance->bSequence.emplace_back(
      wrap::midl(sequence1.x, sequence0.x), wrap::midl(sequence1.y, sequence0.y), 0);
}

void movseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t const                 ind) {
  auto  lineEndPoint = wrap::next(lineEndpoints.begin(), sortedLineIndices[ind]);
  auto& bSequence    = Instance->bSequence;

  bSequence.emplace_back(lineEndPoint->x, lineEndPoint->y, SEQBOT);
  // Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
  //             and not the next entry in sortedLines
  ++lineEndPoint;
  bSequence.emplace_back(lineEndPoint->x, lineEndPoint->y, SEQTOP);
}

void brkseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t const                 start,
            uint32_t const                 finish,
            boost::dynamic_bitset<>&       sequenceMap,
            uint32_t&                      lastGroup,
            uint32_t&                      sequenceIndex) {
  Instance->stateMap.reset(StateFlag::SEQDUN);
  if (sequenceIndex > lineEndpoints.size()) {
	sequenceIndex = start;
  }
  if (start > finish) {
	auto savedGroup = lineEndpoints[sortedLineIndices[start]].group + 1U;
	// This odd construction for iLine is used to ensure
	// loop terminates when finish = 0
	for (auto iLine = start + 1U; iLine != finish; --iLine) {
	  auto const iLineDec = iLine - 1U;
	  --savedGroup;
	  if (lineEndpoints[sortedLineIndices[iLineDec]].group != savedGroup) {
		auto& bSequence = Instance->bSequence;
		bSequence.emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                       lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                       0);
		sequenceIndex = iLineDec;
		bSequence.emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                       lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                       0);
		savedGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	  }
	  else {
		sequenceIndex = iLineDec;
	  }
	  if (sequenceMap.test_set(iLineDec)) {
		if (!Instance->stateMap.testAndSet(StateFlag::SEQDUN)) {
		  duseq1(lineEndpoints, sortedLineIndices, sequenceIndex);
		}
	  }
	  else {
		movseq(lineEndpoints, sortedLineIndices, iLineDec);
	  }
	}
	lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
  }
  else {
	auto savedGroup = lineEndpoints[sortedLineIndices[start]].group - 1U;
	for (auto iLine = start; iLine <= finish; ++iLine) {
	  ++savedGroup;
	  if (lineEndpoints[sortedLineIndices[iLine]].group != savedGroup) {
		auto& bSequence = Instance->bSequence;
		bSequence.emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                       lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                       0);
		sequenceIndex = iLine;
		bSequence.emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                       lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                       0);
		savedGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	  }
	  else {
		sequenceIndex = iLine;
	  }
	  if (sequenceMap.test_set(iLine)) {
		if (!Instance->stateMap.testAndSet(StateFlag::SEQDUN)) {
		  duseq1(lineEndpoints, sortedLineIndices, sequenceIndex);
		}
	  }
	  else {
		movseq(lineEndpoints, sortedLineIndices, iLine);
	  }
	}
	lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
  }
  if (Instance->stateMap.testAndReset(StateFlag::SEQDUN)) {
	duseq1(lineEndpoints, sortedLineIndices, sequenceIndex);
  }
}

void dunseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
            std::vector<uint32_t> const&   sortedLineIndices,
            uint32_t const                 start,
            uint32_t const                 finish,
            uint32_t&                      lastGroup) {
  auto minimumY = BIGFLOAT;
  for (auto iLine = start; iLine <= finish; ++iLine) {
	auto const index  = wrap::toSize(sortedLineIndices[start]);
	auto const deltaY = lineEndpoints[index + 1U].y - lineEndpoints[index].y;

	minimumY = std::min(deltaY, minimumY);
  }
  if (util::closeEnough(minimumY, BIGFLOAT)) {
	minimumY = 0.0F;
  }
  else {
	minimumY *= HALF;
  }
  auto& bSequence = Instance->bSequence;
  bSequence.emplace_back(
      lineEndpoints[sortedLineIndices[start]].x, lineEndpoints[sortedLineIndices[start]].y + minimumY, 0);
  bSequence.emplace_back(lineEndpoints[sortedLineIndices[finish]].x,
                         lineEndpoints[sortedLineIndices[finish]].y + minimumY,
                         0);
  lastGroup = lineEndpoints[sortedLineIndices[finish]].group;
}

void duseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
           std::vector<uint32_t> const&   sortedLineIndices,
           uint32_t const                 start,
           uint32_t const                 finish,
           boost::dynamic_bitset<>&       sequenceMap,
           uint32_t&                      lastGroup,
           uint32_t&                      sequenceIndex) {
  auto savedTopLine = lineEndpoints[wrap::toSize(sortedLineIndices[start]) + 1U].line;
  Instance->stateMap.reset(StateFlag::SEQDUN);
  bool flag = false;
  if (start > finish) {
	auto iLine = start + 1U;
	// This odd construction for iLine is used to ensure loop terminates when finish = 0
	for (; iLine != finish; --iLine) {
	  auto const iLineDec = iLine - 1U;
	  if (sequenceMap.test_set(iLineDec)) {
		if (!Instance->stateMap.testAndSet(StateFlag::SEQDUN)) {
		  duseq1(lineEndpoints, sortedLineIndices, iLineDec);
		  sequenceIndex = iLineDec;
		  flag          = true;
		  continue;
		}
		if (savedTopLine != lineEndpoints[wrap::toSize(sortedLineIndices[iLineDec]) + 1U].line) {
		  if (iLineDec != 0U) {
			duseq1(lineEndpoints, sortedLineIndices, iLineDec + 1U);
		  }
		  duseq1(lineEndpoints, sortedLineIndices, iLineDec);
		  sequenceIndex = iLineDec;
		  flag          = true;
		  savedTopLine  = lineEndpoints[wrap::toSize(sortedLineIndices[iLineDec]) + 1U].line;
		}
		continue;
	  }
	  if (Instance->stateMap.testAndReset(StateFlag::SEQDUN)) {
		duseq1(lineEndpoints, sortedLineIndices, iLineDec + 1U);
	  }
	  sequenceIndex = iLineDec;
	  flag          = true;
	  movseq(lineEndpoints, sortedLineIndices, iLineDec);
	}
	if (Instance->stateMap.testAndReset(StateFlag::SEQDUN)) {
	  duseq1(lineEndpoints, sortedLineIndices, iLine);
	  sequenceIndex = iLine;
	  flag          = true;
	}
	if (flag) { // flag is set if sequenceLines has been set above
	  lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	}
	return;
  }
  auto iLine = start;
  for (; iLine <= finish; ++iLine) {
	if (sequenceMap.test_set(iLine)) {
	  if (!Instance->stateMap.testAndSet(StateFlag::SEQDUN)) {
		flag = true;
		duseq1(lineEndpoints, sortedLineIndices, iLine);
		sequenceIndex = iLine;
		continue;
	  }
	  if (savedTopLine != lineEndpoints[wrap::toSize(sortedLineIndices[iLine]) + 1U].line) {
		if (iLine != 0U) {
		  duseq1(lineEndpoints, sortedLineIndices, iLine - 1U);
		}
		flag = true;
		duseq1(lineEndpoints, sortedLineIndices, iLine);
		sequenceIndex = iLine;
		savedTopLine  = lineEndpoints[wrap::toSize(sortedLineIndices[iLine]) + 1U].line;
	  }
	  continue;
	}
	if (Instance->stateMap.testAndReset(StateFlag::SEQDUN)) {
	  if (iLine != 0U) {
		duseq1(lineEndpoints, sortedLineIndices, iLine - 1U);
	  }
	}
	flag          = true;
	sequenceIndex = iLine;
	movseq(lineEndpoints, sortedLineIndices, iLine);
  }
  if (Instance->stateMap.testAndReset(StateFlag::SEQDUN)) {
	flag = true;
	duseq1(lineEndpoints, sortedLineIndices, iLine - 1U);
	sequenceIndex = iLine - 1U;
  }
  if (flag) {
	lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
  }
}

void durgn(FRM_HEAD const&                form,
           std::vector<F_SEQ> const&      sequencePath,
           boost::dynamic_bitset<>&       visitedRegions,
           std::vector<SMAL_PNT_L> const& lineEndpoints,
           std::vector<uint32_t> const&   sortedLineIndices,
           uint32_t const                 pthi,
           uint32_t const                 lineCount,
           std::vector<REGION> const&     regionsList,
           uint32_t&                      lastGroup,
           uint32_t const                 sequencePathIndex,
           std::vector<F_POINT>&          workingFormVertices) {
  // ReSharper disable once CppTemplateArgumentsCanBeDeduced
  auto        sequenceMap   = boost::dynamic_bitset<>(lineCount);
  auto        nextGroup     = gsl::narrow_cast<uint32_t>(sequencePath[pthi].nextGroup);
  auto const  iRegion       = sequencePath[pthi].node;
  auto const& currentRegion = regionsList[iRegion];
  auto const  sequenceStart = currentRegion.start;
  auto const  sequenceEnd   = currentRegion.end;
  auto&       bSequence     = Instance->bSequence;

  if (sequencePath[pthi].skp || Instance->stateMap.testAndReset(StateFlag::BRKFIX)) {
	if (auto iter = std::prev(bSequence.end()); iter->attribute != SEQBOT) {
	  --iter;
	  bSequence.emplace_back(iter->x, iter->y, 0);
	}
	// clang-format off
	auto const  firstLine     = lineEndpoints[sortedLineIndices[sequenceStart]].line;
	auto const& bpnt          = bSequence.back();
	auto        minimumLength = BIGFLOAT;
	auto        mindif        = 0U;
	// clang-format on
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  auto const deltaX = bpnt.x - workingFormVertices.operator[](iVertex).x;
	  auto const deltaY = bpnt.y - workingFormVertices.operator[](iVertex).y;

	  if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
		minimumLength = length;
		mindif        = iVertex;
	  }
	}
	if (minimumLength != 0.0F) {
	  bSequence.emplace_back(
	      workingFormVertices.operator[](mindif).x, workingFormVertices.operator[](mindif).y, 0);
	}
	if (form.vertexCount != 0U) {
	  auto const fdif = (form.vertexCount + firstLine - mindif) % form.vertexCount;
	  if (auto const bdif = (form.vertexCount - firstLine + mindif) % form.vertexCount; fdif < bdif) {
		auto ind = form::nxt(form, mindif);
		while (ind != firstLine) {
		  bSequence.emplace_back(
		      workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
		  ind = form::nxt(form, ind);
		}
		bSequence.emplace_back(
		    workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
	  }
	  else {
		auto ind = form::prv(form, mindif);
		while (ind != firstLine) {
		  bSequence.emplace_back(
		      workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
		  ind = form::prv(form, ind);
		}
		bSequence.emplace_back(
		    workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
	  }
	}
  }
  auto dun = true;
  if (!visitedRegions[iRegion]) {
	dun = false;
	visitedRegions.set(iRegion);
  }
  auto const groupStart = lineEndpoints[sortedLineIndices[currentRegion.start]].group;
  auto const groupEnd   = lineEndpoints[sortedLineIndices[currentRegion.end]].group;
  auto       seql       = 0U;
  if (groupEnd != groupStart) {
	auto const intermediate =
	    std::round(((wrap::toDouble(lastGroup) - groupStart) / (wrap::toDouble(groupEnd) - groupStart) *
	                (wrap::toDouble(sequenceEnd) - sequenceStart)) +
	               sequenceStart);
	wrap::narrow(seql, std::abs(intermediate));
  }
  auto const length = (wrap::toDouble(groupEnd) - groupStart) * (wrap::toDouble(sequenceEnd) - sequenceStart);
  auto seqn = sequenceEnd;
  if (length != 0.0) {
	if (nextGroup == 0 || nextGroup < groupStart) {
	  nextGroup = groupStart;
	}
	seqn = wrap::round<decltype(seqn)>(((wrap::toDouble(nextGroup) - groupStart) / length) + sequenceStart);
  }
  seql = std::max(seql, sequenceStart);
  seql = std::min(seql, sequenceEnd);
  seqn = std::max(seqn, sequenceStart);
  seqn = std::min(seqn, sequenceEnd);
  if (lineEndpoints[sortedLineIndices[seql]].group != lastGroup) {
	if (seql < sequenceEnd && lineEndpoints[sortedLineIndices[wrap::toSize(seql) + 1U]].group == lastGroup) {
	  ++seql;
	}
	else {
	  if (seql > sequenceStart && lineEndpoints[sortedLineIndices[seql - 1U]].group == lastGroup) {
		--seql;
	  }
	  else {
		auto mindif = BIGUINT;
		for (auto ind = sequenceStart; ind <= sequenceEnd; ++ind) {
		  if (auto const gdif = lineEndpoints[sortedLineIndices[ind]].group > lastGroup
		                            ? lineEndpoints[sortedLineIndices[ind]].group - lastGroup
		                            : lastGroup - lineEndpoints[sortedLineIndices[ind]].group;
		      gdif < mindif) {
			mindif = gdif;
			seql   = ind;
		  }
		}
	  }
	}
  }
  if (lineEndpoints[sortedLineIndices[seqn]].group != nextGroup) {
	if (seqn < sequenceEnd && lineEndpoints[sortedLineIndices[wrap::toSize(seqn) + 1U]].group == nextGroup) {
	  ++seqn;
	}
	else {
	  if (seqn > sequenceStart && lineEndpoints[sortedLineIndices[seqn - 1U]].group == nextGroup) {
		--seqn;
	  }
	  else {
		auto mindif = BIGUINT;
		for (auto ind = sequenceStart; ind <= sequenceEnd; ++ind) {
		  if (auto const gdif = lineEndpoints[sortedLineIndices[ind]].group > nextGroup
		                            ? lineEndpoints[sortedLineIndices[ind]].group - nextGroup
		                            : nextGroup - lineEndpoints[sortedLineIndices[ind]].group;
		      gdif < mindif) {
			mindif = gdif;
			seqn   = ind;
		  }
		}
	  }
	}
  }
  auto sequenceIndex = wrap::toUnsigned(lineEndpoints.size() + 1U);
  if (currentRegion.breakCount != 0U) {
	if (dun) {
	  brkdun(lineEndpoints, sortedLineIndices, seql, seqn, workingFormVertices);
	  return;
	}
	if (lastGroup >= groupEnd) {
	  brkseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
	  if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
		brkseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
	  }
	  return;
	}
	if (groupStart <= nextGroup) {
	  if (seql != sequenceStart) {
		brkseq(lineEndpoints, sortedLineIndices, seql, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
	  }
	  brkseq(lineEndpoints, sortedLineIndices, sequenceStart, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
	  if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
		brkseq(lineEndpoints, sortedLineIndices, sequenceEnd, seqn, sequenceMap, lastGroup, sequenceIndex);
	  }
	  return;
	}
	if (seql != sequenceEnd) {
	  brkseq(lineEndpoints, sortedLineIndices, seql, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
	}
	brkseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
	if (pthi < sequencePathIndex - 1 && sequenceStart != seqn) {
	  brkseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
	}
	return;
  }
  if (dun) {
	dunseq(lineEndpoints, sortedLineIndices, seql, seqn, lastGroup);
	return;
  }
  if (lastGroup >= groupEnd) {
	duseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
	duseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
	return;
  }
  if (groupStart <= nextGroup) {
	if (seql != sequenceStart) {
	  duseq(lineEndpoints, sortedLineIndices, seql, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
	}
	duseq(lineEndpoints, sortedLineIndices, sequenceStart, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
	if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
	  duseq(lineEndpoints, sortedLineIndices, sequenceEnd, seqn, sequenceMap, lastGroup, sequenceIndex);
	}
	return;
  }
  if (seql != sequenceEnd) {
	duseq(lineEndpoints, sortedLineIndices, seql, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
  }
  duseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
  if (pthi < sequencePathIndex - 1 && sequenceStart != seqn) {
	duseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
  }
}

auto compLines(std::tuple<uint32_t, SMAL_PNT_L*> const& arg3,
               std::tuple<uint32_t, SMAL_PNT_L*> const& arg4) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  auto const* arg1 = std::get<SMAL_PNT_L*>(arg3);
  auto const* arg2 = std::get<SMAL_PNT_L*>(arg4);
  if (arg1->line < arg2->line) {
	return true;
  }
  if (arg2->line < arg1->line) {
	return false;
  }
  if (arg1->group < arg2->group) {
	return true;
  }
  if (arg2->group < arg1->group) {
	return false;
  }
  if (arg1->y < arg2->y) {
	return true;
  }
  if (arg2->y < arg1->y) {
	return false;
  }
  return false;
}

auto getLineSortOrder(std::vector<SMAL_PNT_L>& lineEndpoints) -> std::vector<uint32_t> {
  auto       sorted          = std::vector<std::tuple<uint32_t, SMAL_PNT_L*>> {};
  auto const stitchLineCount = lineEndpoints.size();
  sorted.reserve(stitchLineCount / 2U);
  for (auto iLine = 0U; iLine < stitchLineCount; iLine += 2U) {
	sorted.emplace_back(iLine, &lineEndpoints[iLine]);
  }
  std::ranges::sort(sorted, compLines);
  auto retVal = std::vector<uint32_t> {};
  retVal.reserve(stitchLineCount / 2U);
  for (auto& line : sorted) {
	retVal.push_back(std::get<uint32_t>(line));
  }
  return retVal;
}

void lcon(FRM_HEAD const&              form,
          std::vector<uint32_t> const& groupIndexSequence,
          std::vector<SMAL_PNT_L>&     lineEndpoints,
          std::vector<F_POINT>&        workingFormVertices) {
#if BUGSEQ
  UNREFERENCED_PARAMETER(groupIndexSequence);
#endif

  if (lineEndpoints.empty()) {
	return;
  }
  auto const sortedLineIndices = getLineSortOrder(lineEndpoints);
  auto const lineCount         = wrap::toUnsigned(sortedLineIndices.size());
  auto       regions           = std::vector<REGION> {};
  regions.emplace_back(0U, 0U, 0U);
  auto breakLine = lineEndpoints[sortedLineIndices[0]].line;
  for (auto iLine = 0U; iLine < lineCount; ++iLine) {
	if (breakLine == lineEndpoints[sortedLineIndices[iLine]].line) {
	  continue;
	}
	regions.back().end = iLine - 1U;
	regions.emplace_back(iLine, 0U, 0U);
	breakLine = lineEndpoints[sortedLineIndices[iLine]].line;
  }
  regions.back().end     = lineCount - 1U;
  auto const regionCount = wrap::toUnsigned(regions.size());
  // ReSharper disable once CppTemplateArgumentsCanBeDeduced
  auto visitedRegions = boost::dynamic_bitset<>(regionCount);
  for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	auto count = 0U;
	if (regions[iRegion].end - regions[iRegion].start <= 1) {
	  continue;
	}
	auto startGroup = lineEndpoints[sortedLineIndices[regions[iRegion].start]].group;
	for (auto iLine = regions[iRegion].start + 1U; iLine <= regions[iRegion].end; ++iLine) {
	  ++startGroup;
	  if (lineEndpoints[sortedLineIndices[iLine]].group == startGroup) {
		continue;
	  }
	  ++count;
	  startGroup = lineEndpoints[sortedLineIndices[iLine]].group;
	}
	regions[iRegion].breakCount = count;
  }

#if BUGSEQ
  // Note - this debug code only works for vertical fill on a single form
  auto bugColor = 0U;
  for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	for (auto iLine = regions[iRegion].start; iLine <= regions[iRegion].end; ++iLine) {
	  auto const* lineGroupPoint = &lineEndpoints[sortedLineIndices[iLine]];
	  Instance->stitchBuffer.push_back(F_POINT_ATTR {lineGroupPoint[0].x, lineGroupPoint[0].y, bugColor});
	  Instance->stitchBuffer.push_back(F_POINT_ATTR {lineGroupPoint[1].x, lineGroupPoint[1].y, bugColor});
	}
	++bugColor;
	bugColor &= 0xf;
  }
#else
  Instance->bSequence.clear();
  auto mapIndexSequence = std::vector<uint32_t> {};
  mapIndexSequence.reserve(wrap::toSize(regionCount) + 1U);
  auto pathMap      = std::vector<R_CON> {};
  auto sequencePath = std::vector<F_SEQ> {};
  if (regionCount <= 1) {
	constexpr auto SPATHIDX = 1U; // Sequence path index
	sequencePath.resize(SPATHIDX);
	auto lastGroup       = 0U;
	sequencePath[0].node = 0;
	wrap::narrow(sequencePath[0].nextGroup, lineEndpoints[sortedLineIndices[regions[0].end]].group);
	sequencePath[0].skp = false;
	durgn(form, sequencePath, visitedRegions, lineEndpoints, sortedLineIndices, 0, lineCount, regions, lastGroup, SPATHIDX, workingFormVertices);
	return;
  }
  auto pathMapIndex = 0U;
  // use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
  pathMap.reserve(wrap::toSize(regionCount * (regionCount - 1U) / 2U) + 2U);
  for (auto iSequence = 0U; iSequence < regionCount; ++iSequence) {
	mapIndexSequence.push_back(pathMapIndex);
	auto count              = 0;
	auto gapToClosestRegion = 0.0F;
	for (auto iNode = 0U; iNode < regionCount; ++iNode) {
	  if (iSequence == iNode) {
		continue;
	  }
	  auto nextGroup = 0U;
	  if (auto const isConnected = regclos(
	          groupIndexSequence, lineEndpoints, sortedLineIndices, iSequence, iNode, regions, gapToClosestRegion, nextGroup);
	      isConnected) {
		pathMap.push_back(R_CON {.node = iNode, .isConnected = isConnected, .nextGroup = nextGroup});
		++pathMapIndex;
		++count;
	  }
	}
	while (count == 0) {
	  gapToClosestRegion += LineSpacing;
	  count = 0;
	  for (auto iNode = 0U; iNode < regionCount; ++iNode) {
		if (iSequence == iNode) {
		  continue;
		}
		auto nextGroup = 0U;
		if (auto const isConnected = regclos(
		        groupIndexSequence, lineEndpoints, sortedLineIndices, iSequence, iNode, regions, gapToClosestRegion, nextGroup);
		    isConnected) {
		  pathMap.push_back(R_CON {.node = iNode, .isConnected = isConnected, .nextGroup = nextGroup});
		  ++pathMapIndex;
		  ++count;
		}
	  }
	}
  }
  mapIndexSequence.push_back(pathMapIndex);
  // find the leftmost region
  auto startGroup = BIGUINT;
  auto leftRegion = 0U;
  for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	if (auto const& lineGroupPoint = lineEndpoints[sortedLineIndices[regions[iRegion].start]];
	    lineGroupPoint.group < startGroup) {
	  startGroup = lineGroupPoint.group;
	  leftRegion = iRegion;
	}
  }
  auto tempPath = std::vector<RG_SEQ> {};
  tempPath.resize(wrap::toSize(regionCount * (regionCount - 1U) / 2U) + 1U);
  // find the leftmost region in pathMap
  auto sequencePathIndex = 1U;
  auto dontSkip          = true;
  auto inPath            = 0U;
  for (; inPath < pathMapIndex; ++inPath) {
	if (pathMap[inPath].node == leftRegion) {
	  dontSkip = false;
	  break;
	}
  }
  if (dontSkip) {
	pathMap.push_back(R_CON {.node = leftRegion, .isConnected = false, .nextGroup = 0});
	inPath = pathMapIndex;
  }
  // set the first entry in the temporary path to the leftmost region
  tempPath[0].pcon  = inPath;
  tempPath[0].count = 1;
  tempPath[0].skp   = false;
  visitedRegions.set(leftRegion);
  auto doneRegion   = leftRegion; // last region sequenced
  auto visitedIndex = 0;
  while (unvis(visitedRegions, visitedIndex)) {
	nxtrgn(tempPath, pathMap, mapIndexSequence, visitedRegions, lineEndpoints, sortedLineIndices, regions, doneRegion, pathMapIndex, sequencePathIndex, visitedIndex);
  }
  auto count = BIGUINT;
  sequencePath.reserve(sequencePathIndex);
  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
	bool const tmpSkip = tempPath[iPath].skp;
	uint16_t   tmpNode = 0U;
	if (tempPath[iPath].pcon == BIGUINT) {
	  wrap::narrow(tmpNode, tempPath[iPath].count);
	  count = wrap::toUnsigned(tempPath[iPath].count);
	}
	else {
	  if (tempPath[iPath].pcon != count) {
		count = tempPath[iPath].pcon;
		wrap::narrow(tmpNode, pathMap[tempPath[iPath].pcon].node);
	  }
	}
	sequencePath.push_back(F_SEQ {.node = tmpNode, .nextGroup = 0, .skp = tmpSkip});
  }
  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
	nxtseq(sequencePath, pathMap, mapIndexSequence, iPath);
  }
  visitedRegions.reset();
  auto lastGroup = 0U;
  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
	outDebugString(L"iterator {},vrt {},grpn {}\n", iPath, pathMap[iPath].node, pathMap[iPath].nextGroup);
	if (!unvis(visitedRegions, visitedIndex)) {
	  break;
	}
	durgn(form, sequencePath, visitedRegions, lineEndpoints, sortedLineIndices, iPath, lineCount, regions, lastGroup, sequencePathIndex, workingFormVertices);
  }

#endif
}

void handleSeqTop(size_t const iSequence, B_SEQ_PNT const& bCurrent) {
  static constexpr auto SEQ_TABLE = std::array {
      12,
      7,
      15,
      11,
      13,
      17,
  };

  auto const& bSequence = Instance->bSequence;

  auto const& bNext     = bSequence.operator[](iSequence + 1U);
  auto const& bPrevious = bSequence.operator[](iSequence - 1U);
  auto        delta     = F_POINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};
  auto const& form      = Instance->formList.operator[](ClosestFormToCursor);
  auto const  rcnt      = iSequence % SEQ_TABLE.size();
  auto        slope     = BIGFLOAT; // initialize to the max value i.e. slope when y is zero

  auto const stitchSpacing2    = LineSpacing * 2;
  auto const userStitchLength9 = UserStitchLength / MAXSIZ;

  auto const rit = std::lround(bSequence.operator[](iSequence).x / stitchSpacing2);

  if (delta.y != 0.0F) {
	slope = delta.x / delta.y;
  }
  if ((form.extendedAttribute & AT_SQR) != 0U) {
	if (Instance->stateMap.testAndFlip(StateFlag::FILDIR)) {
	  Instance->oSequence.emplace_back(bPrevious.x, bPrevious.y);
	  auto yVal = (wrap::toFloat(wrap::ceil<int32_t>(bCurrent.y / UserStitchLength)) * UserStitchLength) +
	              (wrap::toFloat(std::abs(rit % SEQ_TABLE.at(rcnt))) * userStitchLength9);
	  while (true) {
		Instance->oSequence.emplace_back(0.0F, yVal);
		if (yVal > bCurrent.y) {
		  break;
		}
		Instance->oSequence.back().x = bCurrent.x;
		yVal += UserStitchLength;
	  }
	  Instance->oSequence.back() = bCurrent;
	  return;
	}
	Instance->oSequence.emplace_back(bCurrent.x, bCurrent.y);
	auto yVal = (wrap::toFloat(wrap::floor<int32_t>(bCurrent.y / UserStitchLength)) * UserStitchLength) -
	            (wrap::toFloat(std::abs((rit + 2) % SEQ_TABLE.at(rcnt))) * userStitchLength9);
	while (true) {
	  Instance->oSequence.emplace_back(0.0F, yVal);
	  if (yVal < bPrevious.y) {
		break;
	  }
	  Instance->oSequence.back().x = bCurrent.x;
	  yVal -= UserStitchLength;
	}
	Instance->oSequence.back() = bPrevious;
	return;
  }
  auto yVal = (wrap::toFloat(wrap::ceil<int32_t>(bNext.y / UserStitchLength)) * UserStitchLength) +
              (wrap::toFloat(std::abs(rit % SEQ_TABLE.at(rcnt))) * userStitchLength9);
  while (true) {
	Instance->oSequence.emplace_back(0.0F, yVal);
	if (yVal > bCurrent.y) {
	  break;
	}
	delta.y = Instance->oSequence.back().y - bNext.y;
	delta.x = slope * delta.y;

	Instance->oSequence.back().x = bNext.x + delta.x;
	yVal += UserStitchLength;
  }
  Instance->oSequence.back() = bCurrent;
}

void handleSeqBot(size_t const iSequence, B_SEQ_PNT const& bCurrent) {
  static constexpr auto SEQ_TABLE = std::array {
      12,
      7,
      15,
      11,
      13,
      17,
  };

  auto const& bSequence = Instance->bSequence;

  auto const& bNext = bSequence.operator[](iSequence + 1U);
  auto        delta = F_POINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};
  auto const& form  = Instance->formList.operator[](ClosestFormToCursor);
  auto const  rcnt  = iSequence % SEQ_TABLE.size();
  auto        slope = BIGFLOAT; // initialize to the max value i.e. slope when y is zero

  auto const stitchSpacing2    = LineSpacing * 2;
  auto const userStitchLength9 = UserStitchLength / MAXSIZ;

  auto const rit = std::lround(bSequence.operator[](iSequence).x / stitchSpacing2);

  if (delta.y != 0.0F) {
	slope = delta.x / delta.y;
  }
  if ((form.extendedAttribute & AT_SQR) != 0U) {
	return;
  }
  auto yVal = (wrap::toFloat(wrap::floor<int32_t>(bNext.y / UserStitchLength)) * UserStitchLength) -
              (wrap::toFloat(std::abs((rit + 2) % SEQ_TABLE.at(rcnt))) * userStitchLength9);
  while (true) {
	Instance->oSequence.emplace_back(0.0F, yVal);
	if (yVal < bCurrent.y) {
	  break;
	}
	delta.y = Instance->oSequence.back().y - bNext.y;
	delta.x = slope * delta.y;

	Instance->oSequence.back().x = bNext.x + delta.x;
	yVal -= UserStitchLength;
  }
  Instance->oSequence.back() = bCurrent;
}

auto handleSeq(size_t const iSequence, B_SEQ_PNT& bCurrent) -> bool {
  auto const& bNext = Instance->bSequence.operator[](iSequence + 1U);
  auto const  delta = F_POINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};

  Instance->stateMap.reset(StateFlag::FILDIR);
  auto const length = std::hypot(delta.x, delta.y);
  if (length == 0.0F) {
	Instance->oSequence.emplace_back(bCurrent.x, bCurrent.y);
	return true;
  }
  if (auto const userStitchLength2 = UserStitchLength * 2.0F; length <= userStitchLength2) {
	Instance->oSequence.emplace_back(bCurrent.x, bCurrent.y);
	return true;
  }
  auto point = bNext; // intended copy
  auto count = wrap::round<uint32_t>((length / UserStitchLength) - 1.0F);
  if (form::chkmax(count, wrap::toUnsigned(Instance->oSequence.size()))) {
	return false;
  }
  auto const fCount = wrap::toFloat(count);
  auto const step   = F_POINT {delta.x / fCount, delta.y / fCount};
  while (count != 0U) {
	point.x += step.x;
	point.y += step.y;
	Instance->oSequence.emplace_back(point.x, point.y);
	--count;
  }
  Instance->oSequence.emplace_back(bCurrent.x, bCurrent.y);
  return true;
}

void bakseq() {
  auto& bSequence = Instance->bSequence;

#if BUGSEQ
#else
#if BUGBAK

  for (auto val : bSequence) {
	Instance->oSequence.push_back(F_POINT {val.x, val.y});
  }
  Instance->formList.operator[](ClosestFormToCursor).maxFillStitchLen = 6000;
#else

  auto iSequence = bSequence.size();
  if (iSequence != 0U) {
	--iSequence;
  }
  Instance->oSequence.clear();
  Instance->stateMap.reset(StateFlag::FILDIR);
  Instance->oSequence.emplace_back(bSequence.operator[](iSequence).x,
                                   bSequence.operator[](iSequence).y);
  if (iSequence != 0U) {
	--iSequence;
  }
  while (iSequence > 0) {
	switch (auto& bCurrent = bSequence.operator[](iSequence); bCurrent.attribute) {
	  case SEQTOP:
		handleSeqTop(iSequence, bCurrent);
		break;
	  case SEQBOT:
		handleSeqBot(iSequence, bCurrent);
		break;
	  case 0: {
		if (!handleSeq(iSequence, bCurrent)) {
		  return;
		}
		break;
	  }
	  default: {
		outDebugString(L"default hit in bakseq: attribute [{}]\n", bCurrent.attribute);
		break;
	  }
	}
	--iSequence;
  }
#endif // BUGBAK
#endif // BUGSEQ
}

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
auto projv(float const xCoordinate, F_POINT const& lowerPoint, F_POINT const& upperPoint, F_POINT& intersection) noexcept -> bool {
  auto const deltaX = upperPoint.x - lowerPoint.x;
  if (deltaX == 0.0F) {
	return false;
  }
  auto const slope = (upperPoint.y - lowerPoint.y) / deltaX;
  intersection     = F_POINT {xCoordinate, ((xCoordinate - lowerPoint.x) * slope) + lowerPoint.y};
  auto lower       = lowerPoint.x;
  auto upper       = upperPoint.x;
  if (lower > upper) {
	std::swap(lower, upper);
  }
  return xCoordinate >= lower && xCoordinate <= upper;
}

auto proj(F_POINT const& point,
          float const    slope,
          F_POINT const& point0,
          F_POINT const& point1,
          F_POINT&       intersectionPoint) noexcept -> bool {
  auto const delta = F_POINT {point1.x - point0.x, point1.y - point0.y};
  if (delta.x != 0.0F) {
	auto const sideSlope     = delta.y / delta.x;
	auto const sideConstant  = point0.y - (sideSlope * point0.x);
	auto const pointConstant = point.y - (slope * point.x);
	auto const newX          = (sideConstant - pointConstant) / (slope - sideSlope);
	intersectionPoint        = F_POINT {newX, (newX * slope) + pointConstant};
  }
  else {
	auto const pointConstant = point.y - (slope * point.x);
	auto const newX          = point0.x;
	intersectionPoint        = F_POINT {newX, (newX * slope) + pointConstant};
  }
  auto const xMinimum = std::min(point0.x, point1.x);
  auto const xMaximum = std::max(point0.x, point1.x);
  if (delta.y != 0.0F) {
	auto const yMinimum = std::min(point0.y, point1.y);
	auto const yMaximum = std::max(point0.y, point1.y);
	return intersectionPoint.x >= xMinimum && intersectionPoint.x <= xMaximum &&
	       intersectionPoint.y >= yMinimum && intersectionPoint.y <= yMaximum;
  }
  if (delta.x == 0.0F) {
	return false;
  }
  return intersectionPoint.x >= xMinimum && intersectionPoint.x <= xMaximum;
}

void fillSB(const F_POINT& pivot, float const angle, float const& radius, F_POINT& stitchPoint, float const& level) {
  auto const outerPoint = F_POINT {pivot.x + (cos(angle) * radius), pivot.y + (sin(angle) * radius)};
  form::filinsb(outerPoint, stitchPoint);
  auto const innerRadius = radius * level * 0.4F;
  auto const innerPoint =
      F_POINT {pivot.x + (cos(angle) * innerRadius), pivot.y + (sin(angle) * innerRadius)};
  form::filinsb(innerPoint, stitchPoint);
}

void spend(std::vector<V_RECT_2> const& fillVerticalRect, uint32_t const start, uint32_t const finish, F_POINT& stitchPoint) {
  // clang-format off
  static constexpr auto LEVEL00 = std::array { 0.0F };
  static constexpr auto LEVEL01 = std::array { 1.0F };
  static constexpr auto LEVEL02 = std::array { 0.0F/2.0F , 1.0F/2.0F  };
  static constexpr auto LEVEL03 = std::array { 1.0F/3.0F , 0.0F/3.0F , 2.0F/3.0F  };
  static constexpr auto LEVEL04 = std::array { 1.0F/4.0F , 3.0F/4.0F , 0.0F/4.0F , 2.0F/4.0F  };
  static constexpr auto LEVEL05 = std::array { 2.0F/5.0F , 0.0F/5.0F , 3.0F/5.0F , 1.0F/5.0F , 4.0F/5.0F  };
  static constexpr auto LEVEL06 = std::array { 2.0F/6.0F , 3.0F/6.0F , 1.0F/6.0F , 4.0F/6.0F , 0.0F/6.0F , 5.0F/6.0F  };
  static constexpr auto LEVEL07 = std::array { 3.0F/7.0F , 2.0F/7.0F , 4.0F/7.0F , 1.0F/7.0F , 5.0F/7.0F , 0.0F/7.0F ,  6.0F/7.0F  };
  static constexpr auto LEVEL08 = std::array { 4.0F/8.0F , 3.0F/8.0F , 5.0F/8.0F , 2.0F/8.0F , 6.0F/8.0F , 1.0F/8.0F ,  7.0F/8.0F , 0.0F/8.0F  };
  static constexpr auto LEVEL09 = std::array { 4.0F/9.0F , 3.0F/9.0F , 5.0F/9.0F , 2.0F/9.0F , 6.0F/9.0F , 1.0F/9.0F ,  7.0F/9.0F , 0.0F/9.0F ,  8.0F/9.0F  };
  static constexpr auto LEVEL10 = std::array { 5.0F/10.0F, 4.0F/10.0F, 6.0F/10.0F, 3.0F/10.0F, 7.0F/10.0F, 2.0F/10.0F,  8.0F/10.0F, 1.0F/10.0F,  9.0F/10.0F,  0.0F/10.0F };
  static constexpr auto LEVEL11 = std::array { 5.0F/11.0F, 4.0F/11.0F, 6.0F/11.0F, 3.0F/11.0F, 7.0F/11.0F, 2.0F/11.0F,  8.0F/11.0F, 1.0F/11.0F,  9.0F/11.0F,  0.0F/11.0F, 10.0F/11.0F };
  static constexpr auto LEVEL12 = std::array { 6.0F/12.0F, 5.0F/12.0F, 7.0F/12.0F, 4.0F/12.0F, 8.0F/12.0F, 3.0F/12.0F,  9.0F/12.0F, 2.0F/12.0F, 10.0F/12.0F,  1.0F/12.0F, 11.0F/12.0F,  0.0F/12.0F};
  static constexpr auto LEVEL13 = std::array { 6.0F/13.0F, 5.0F/13.0F, 7.0F/13.0F, 4.0F/13.0F, 8.0F/13.0F, 3.0F/13.0F,  9.0F/13.0F, 2.0F/13.0F, 10.0F/13.0F,  1.0F/13.0F, 11.0F/13.0F,  0.0F/13.0F, 12.0F/13.0F};
  static constexpr auto LEVEL14 = std::array { 7.0F/14.0F, 6.0F/14.0F, 8.0F/14.0F, 5.0F/14.0F, 9.0F/14.0F, 4.0F/14.0F, 10.0F/14.0F, 3.0F/14.0F, 11.0F/14.0F,  2.0F/14.0F, 12.0F/14.0F,  1.0F/14.0F, 13.0F/14.0F,  0.0F/14.0F };
  static constexpr auto LEVEL15 = std::array { 7.0F/15.0F, 6.0F/15.0F, 8.0F/15.0F, 5.0F/15.0F, 9.0F/15.0F, 4.0F/15.0F, 10.0F/15.0F, 3.0F/15.0F, 11.0F/15.0F,  2.0F/15.0F, 12.0F/15.0F,  1.0F/15.0F, 13.0F/15.0F,  0.0F/15.0F, 14.0F/15.0F};
  // clang-format on

  constexpr auto LEVELSMAX = uint32_t {15U}; // max number of levels defined in the table

  auto constexpr LEVELS = std::array {LEVEL00.data(),
                                      LEVEL01.data(),
                                      LEVEL02.data(),
                                      LEVEL03.data(),
                                      LEVEL04.data(),
                                      LEVEL05.data(),
                                      LEVEL06.data(),
                                      LEVEL07.data(),
                                      LEVEL08.data(),
                                      LEVEL09.data(),
                                      LEVEL10.data(),
                                      LEVEL11.data(),
                                      LEVEL12.data(),
                                      LEVEL13.data(),
                                      LEVEL14.data(),
                                      LEVEL15.data()};

  auto const innerDelta = F_POINT {(fillVerticalRect[finish].cipnt.x - fillVerticalRect[start].bipnt.x),
                                   (fillVerticalRect[finish].cipnt.y - fillVerticalRect[start].bipnt.y)};
  auto const innerLength = std::hypot(innerDelta.x, innerDelta.y);
  auto const outerDelta = F_POINT {(fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
                                   (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y)};
  auto const outerLength = std::hypot(outerDelta.x, outerDelta.y);
  auto const flag        = outerLength > innerLength;
  auto const pivot       = flag ? fillVerticalRect[start].cipnt : fillVerticalRect[start].copnt;

  auto const startDelta  = flag ? F_POINT {fillVerticalRect[start].copnt.x - pivot.x,
                                          fillVerticalRect[start].copnt.y - pivot.y}
                                : F_POINT {fillVerticalRect[start].cipnt.x - pivot.x,
                                          fillVerticalRect[start].cipnt.y - pivot.y};
  auto const finishDelta = flag ? F_POINT {fillVerticalRect[finish].bopnt.x - pivot.x,
                                           fillVerticalRect[finish].bopnt.y - pivot.y}
                                : F_POINT {fillVerticalRect[finish].bipnt.x - pivot.x,
                                           fillVerticalRect[finish].bipnt.y - pivot.y};
  // ToDo is this correct? hypot rather than tan ?
  if (std::hypot(stitchPoint.x - pivot.x, stitchPoint.y - pivot.y) > PI_F2) {
	form::filinsb(pivot, stitchPoint);
  }
  auto       startAngle  = std::atan2(startDelta.y, startDelta.x);
  auto const finishAngle = std::atan2(finishDelta.y, finishDelta.x);
  auto       deltaAngle  = finishAngle - startAngle;
  if (deltaAngle > PI_F) {
	deltaAngle -= PI_F2;
  }
  if (deltaAngle < -PI_F) {
	deltaAngle += PI_F2;
  }
  auto const radius    = std::hypot(startDelta.x, startDelta.y);
  auto const arc       = fabs(radius * deltaAngle);
  auto       count     = wrap::round<uint32_t>(arc / LineSpacing);
  auto const stepAngle = deltaAngle / wrap::toFloat(count);
  if (count == 0U) {
	count = 1U;
  }
  if (count > LEVELSMAX) {
	for (auto iCount = 0U; iCount < count; ++iCount) {
	  auto const level = wrap::toFloat(form::psg() % count) / wrap::toFloat(count);
	  fillSB(pivot, startAngle, radius, stitchPoint, level);
	  startAngle += stepAngle;
	}
	return;
  }
  for (auto const spLevels = gsl::span {LEVELS.operator[](count), count}; auto const& level : spLevels) {
	fillSB(pivot, startAngle, radius, stitchPoint, level);
	startAngle += stepAngle;
  }
}

void duspnd(float const                  stitchLen,
            std::vector<V_RECT_2> const& underlayVerticalRect,
            std::vector<V_RECT_2> const& fillVerticalRect,
            uint32_t const               start,
            uint32_t const               finish,
            float const                  width,
            F_POINT&                     stitchPoint) {
  if (!Instance->stateMap.test(StateFlag::UND)) {
	spend(fillVerticalRect, start, finish, stitchPoint);
	return;
  }
  if (Instance->stateMap.test(StateFlag::UNDPHAS)) {
	form::filinsb(underlayVerticalRect[start].copnt, stitchPoint);
	form::filinsb(underlayVerticalRect[start].cipnt, stitchPoint);
	auto const delta =
	    F_POINT {underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x,
	             underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y};
	if (auto const length = std::hypot(delta.x, delta.y); length > stitchLen) {
	  auto const angle =
	      std::atan2(InsidePoints->operator[](finish).y - OutsidePoints->operator[](finish).y,
	                 InsidePoints->operator[](finish).x - OutsidePoints->operator[](finish).x);
	  auto const point = F_POINT {underlayVerticalRect[finish].bopnt.x + (cos(angle) * width),
	                              underlayVerticalRect[finish].bopnt.y + (sin(angle) * width)};
	  form::filinsb(point, stitchPoint);
	}
	form::filinsb(underlayVerticalRect[finish].bipnt, stitchPoint);
	form::filinsb(underlayVerticalRect[finish].bopnt, stitchPoint);
	return;
  }
  form::filinsb(underlayVerticalRect[start].cipnt, stitchPoint);
  form::filinsb(underlayVerticalRect[start].copnt, stitchPoint);
  auto const delta =
      F_POINT {underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x,
               underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y};
  if (auto const length = std::hypot(delta.x, delta.y); length > stitchLen) {
	auto const angle =
	    std::atan2(OutsidePoints->operator[](finish).y - InsidePoints->operator[](finish).y,
	               OutsidePoints->operator[](finish).x - InsidePoints->operator[](finish).x);
	auto const point = F_POINT {underlayVerticalRect[finish].bipnt.x + (cos(angle) * width),
	                            underlayVerticalRect[finish].bipnt.y + (sin(angle) * width)};
	form::filinsb(point, stitchPoint);
  }
  form::filinsb(underlayVerticalRect[finish].bopnt, stitchPoint);
  form::filinsb(underlayVerticalRect[finish].bipnt, stitchPoint);
}

void trfrm(F_POINT const& bottomLeftPoint,
           F_POINT const& topLeftPoint,
           F_POINT const& bottomRightPoint,
           F_POINT const& topRightPoint) {
  auto const topDelta    = topRightPoint - topLeftPoint;
  auto const bottomDelta = bottomRightPoint - bottomLeftPoint;
  for (auto const& clip : Instance->clipBuffer) {
	auto const clipRatio   = F_POINT {clip.x / ClipRectSize.cx, clip.y / ClipRectSize.cy};
	auto const topMidpoint = F_POINT {(clipRatio.x * topDelta.x) + topLeftPoint.x,
	                                  (clipRatio.x * topDelta.y) + topLeftPoint.y};

	auto const bottomMidpoint = F_POINT {(clipRatio.x * bottomDelta.x) + bottomLeftPoint.x,
	                                     (clipRatio.x * bottomDelta.y) + bottomLeftPoint.y};

	auto const middleDelta = topMidpoint - bottomMidpoint;
	Instance->oSequence.emplace_back((clipRatio.y * middleDelta.x) + bottomMidpoint.x,
	                                 (clipRatio.y * middleDelta.y) + bottomMidpoint.y);
  }
}

void clpfm() {
  auto const& bSequence = Instance->bSequence;
  auto const  endIt     = wrap::toUnsigned(bSequence.size()) - 2U;

  for (auto iSequence = 0U; iSequence < endIt; iSequence += 2) {
	auto const& bSeq0 = bSequence.operator[](iSequence);
	auto const& bSeq1 = bSequence.operator[](wrap::toSize(iSequence) + 1U);
	auto const& bSeq2 = bSequence.operator[](wrap::toSize(iSequence) + 2U);
	auto const& bSeq3 = bSequence.operator[](wrap::toSize(iSequence) + 3U);

	auto const leftLength  = std::hypot(bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y);
	auto const rightLength = std::hypot(bSeq3.x - bSeq2.x, bSeq3.y - bSeq2.y);
	auto const leftDelta   = F_POINT {bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y};
	auto const rightDelta  = F_POINT {bSeq2.x - bSeq3.x, bSeq2.y - bSeq3.y};

	auto count = std::round(rightLength > leftLength ? leftLength / ClipRectSize.cy
	                                                 : rightLength / ClipRectSize.cy);
	if (count == 0.0F) {
	  count = 1.0F;
	}
	auto const leftStep  = F_POINT {leftDelta.x / count, leftDelta.y / count};
	auto const rightStep = F_POINT {rightDelta.x / count, rightDelta.y / count};
	auto       topLeft   = F_POINT {bSeq0.x, bSeq0.y};
	auto       topRight  = F_POINT {bSeq3.x, bSeq3.y};
	for (auto iStep = 0U; iStep < wrap::toUnsigned(count); ++iStep) {
	  auto const bottomLeft  = topLeft;
	  auto const bottomRight = topRight;
	  topLeft += leftStep;
	  topRight += rightStep;
	  trfrm(bottomLeft, topLeft, bottomRight, topRight);
	}
  }
}

void fmclp(FRM_HEAD& form) {
  auto const savedSpacing = LineSpacing;
  LineSpacing             = ClipRectSize.cx;
  Instance->stateMap.set(StateFlag::BARSAT);
  satin::satfil(form);
  Instance->stateMap.reset(StateFlag::BARSAT);
  clpfm();
  auto& currentForm    = Instance->formList.operator[](ClosestFormToCursor);
  currentForm.fillType = CLPF;
  LineSpacing          = savedSpacing;
}

void swEdgeType(FRM_HEAD const& form, FRM_HEAD& angledForm) {
  switch (form.edgeType & NEGUND) {
	case EDGELINE: {
	  brdfil(form);
	  ritbrd(form);
	  break;
	}
	case EDGEBEAN: {
	  bold(form);
	  ritbrd(form);
	  break;
	}
	case EDGECLIP: {
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
	  clip::clpout(form.borderSize);
	  clip::clpbrd(form, clipRect, 0);
	  ritbrd(form);
	  break;
	}
	case EDGEANGSAT: {
	  Instance->stateMap.reset(StateFlag::SAT1);
	  satin::slbrd(form);
	  ritbrd(form);
	  break;
	}
	case EDGEPROPSAT: {
	  if (form.vertexCount > 2) {
		Instance->stateMap.reset(StateFlag::SAT1);
		plbrd(form, angledForm, Instance->angledFormVertices);
		ritbrd(form);
	  }
	  break;
	}
	case EDGEAPPL: {
	  lapbrd(form);
	  ritapbrd();
	  Instance->stateMap.reset(StateFlag::SAT1);
	  satin::slbrd(form);
	  ritbrd(form);
	  break;
	}
	case EDGEBHOL: {
	  auto const length      = ButtonholeCornerLength;
	  ButtonholeCornerLength = form::getblen();
	  satin::satout(form, BHWIDTH);
	  blbrd(form);
	  ButtonholeCornerLength = length;
	  ritbrd(form);
	  break;
	}
	case EDGEPICOT: {
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
	  auto const length      = ButtonholeCornerLength;
	  ButtonholeCornerLength = form::getplen();
	  clip::clpic(form, clipRect);
	  ButtonholeCornerLength = length;
	  ritbrd(form);
	  break;
	}
	case EDGEDOUBLE: {
	  dubfn(form);
	  ritbrd(form);
	  break;
	}
	case EDGELCHAIN: {
	  Instance->stateMap.set(StateFlag::LINCHN);
	  clip::chnfn(form);
	  ritbrd(form);
	  break;
	}
	case EDGEOCHAIN: {
	  Instance->stateMap.reset(StateFlag::LINCHN);
	  clip::chnfn(form);
	  ritbrd(form);
	  break;
	}
	case EDGECLIPX: {
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
	  clip::duxclp(form);
	  ritbrd(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in refilfn 1: edgeType [{}]\n", form.edgeType & NEGUND);
	  break;
	}
  }
}

void swPolyFillType(FRM_HEAD& form, FRM_HEAD& angledForm, std::vector<RNG_COUNT>& textureSegments) {
  auto lineEndpoints       = std::vector<SMAL_PNT_L> {};
  auto groupIndexSequence  = std::vector<uint32_t> {};
  auto workingFormVertices = std::vector<F_POINT> {};
  auto rotationCenter      = F_POINT {};
  auto doFill              = true;
  auto rotationAngle       = 0.0F;
  // use fillType to determine which function to call
  switch (form.fillType) {
	case VRTF: { // vertical fill
	  workingFormVertices.clear();
	  workingFormVertices.reserve(form.vertexCount);
	  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	  auto const itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
	  workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
	  fnvrt(workingFormVertices, groupIndexSequence, lineEndpoints);
	  break;
	}
	case HORF: { // horizontal fill
	  rotationAngle = PI_FHALF;
	  fnhor(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, Instance->angledFormVertices);
	  workingFormVertices.clear();
	  workingFormVertices.reserve(angledForm.vertexCount);
	  auto const itStartVertex = wrap::next(Instance->angledFormVertices.cbegin(), angledForm.vertexIndex);
	  auto const itEndVertex = wrap::next(itStartVertex, angledForm.vertexCount);
	  workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
	  break;
	}
	case ANGF: { // angled fill
	  rotationAngle = PI_FHALF - form.fillAngle;
	  fnang(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, Instance->angledFormVertices);
	  workingFormVertices.clear();
	  workingFormVertices.reserve(angledForm.vertexCount);
	  auto const itStartVertex = wrap::next(Instance->angledFormVertices.cbegin(), angledForm.vertexIndex);
	  auto const itEndVertex = wrap::next(itStartVertex, angledForm.vertexCount);
	  workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
	  break;
	}
	case VCLPF: { // vertical clip fill
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.clipIndex, form.clipCount);
	  workingFormVertices.clear();
	  workingFormVertices.reserve(form.vertexCount);
	  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	  auto const itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
	  workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
	  clpcon(form, textureSegments, workingFormVertices);
	  doFill = false;
	  break;
	}
	case HCLPF: { // horizontal clip fill
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.clipIndex, form.clipCount);
	  horclpfn(textureSegments, angledForm, Instance->angledFormVertices);
	  doFill = false;
	  break;
	}
	case ANGCLPF: { // angled clip fill
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.clipIndex, form.clipCount);
	  Instance->stateMap.reset(StateFlag::ISUND);
	  form::angclpfn(form, textureSegments, Instance->angledFormVertices);
	  doFill = false;
	  break;
	}
	case TXVRTF: { // vertical fill with texture
	  texture::setxt(form, textureSegments);
	  workingFormVertices.clear();
	  workingFormVertices.reserve(form.vertexCount);
	  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
	  auto const itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
	  workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
	  clpcon(form, textureSegments, workingFormVertices);
	  doFill = false;
	  break;
	}
	case TXHORF: { // horizontal fill with texture
	  texture::setxt(form, textureSegments);
	  horclpfn(textureSegments, angledForm, Instance->angledFormVertices);
	  doFill = false;
	  break;
	}
	case TXANGF: { // angled fill with texture
	  texture::setxt(form, textureSegments);
	  Instance->stateMap.reset(StateFlag::ISUND);
	  form::angclpfn(form, textureSegments, Instance->angledFormVertices);
	  doFill = false;
	  break;
	}
	default: {
	  outDebugString(L"default hit in refilfn 2: fillType [{}]\n", form.fillType);
	  break;
	}
  }
  // fill the form if it is a vertical, horizontal, or angled fill
  if (doFill) {
	lcon(form, groupIndexSequence, lineEndpoints, workingFormVertices);
	bakseq();
	if (form.fillType != VRTF && form.fillType != TXVRTF) {
	  rotationAngle = -rotationAngle;
	  rotbak(rotationAngle, rotationCenter);
	}
  }
}

void swSatFillType(FRM_HEAD& form) {
  switch (form.fillType) {
	case SATF: {
	  auto const spacing = LineSpacing;
	  LineSpacing        = form.fillSpacing;
	  UserStitchLength   = form.stitchLength;
	  satin::satfil(form);
	  LineSpacing = spacing;
	  ritfil(form);
	  break;
	}
	case CLPF: {
	  auto clipRect = F_RECTANGLE {};
	  clip::oclp(clipRect, form.clipIndex, form.clipCount);
	  fmclp(form);
	  ritfil(form);
	  break;
	}
	case FTHF: {
	  Instance->stateMap.set(StateFlag::CNV2FTH);
	  xt::fthrfn(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in refilfn 3: fillType [{}]\n", form.fillType);
	  break;
	}
  }
}

void makpoli() {
  auto& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  if (currentForm.type == SAT && currentForm.satinGuideCount != 0U) {
	satin::delsac(ClosestFormToCursor);
  }
  currentForm.type = FRMFPOLY;
}

void fsvrt() {
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type        = FRMFPOLY;
  form.fillColor   = ActiveColor;
  form.fillType    = VRTF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form.squareEnd(Instance->userFlagMap.test(UserFlag::SQRFIL));
  form::refilfn(ClosestFormToCursor);
}

void fshor(FRM_HEAD& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type        = FRMFPOLY;
  form.fillColor   = ActiveColor;
  form.fillType    = HORF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form.fillAngle = PI_FHALF;
  form.squareEnd(Instance->userFlagMap.test(UserFlag::SQRFIL));
  form::refil(ClosestFormToCursor);
}

void fsangl(FRM_HEAD& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type        = FRMFPOLY;
  form.fillColor   = ActiveColor;
  form.fillType    = ANGF;
  form.fillAngle   = IniFile.fillAngle;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form.squareEnd(Instance->userFlagMap.test(UserFlag::SQRFIL));
  form::refil(ClosestFormToCursor);
}

void ducon() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, Instance->formLines.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void uncon() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOCON)) {
	ducon();
  }
}

void filsfn(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::delmclp(formIndex);
  texture::deltx(formIndex);
  form.type        = SAT;
  form.fillColor   = ActiveColor;
  form.fillType    = SATF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form::refilfn(formIndex);
}

auto closat(IntersectionStyles& inOutFlag) -> bool {
  auto       minimumLength = BIGFLOAT;
  auto const stitchPoint   = thred::pxCor2stch(WinMsg.pt);

  auto const& formList = Instance->formList;
  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
	auto const& formIter = formList.operator[](iForm);
	if (formIter.vertexCount == 0U) {
	  outDebugString(L"closat: Form Has no vertices!\n inOutFlag[{}]", gsl::narrow_cast<int>(inOutFlag));
	}
	if (auto const layer =
	        gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formIter.attribute & FRMLMSK) >> 1U);
	    ActiveLayer != 0U && layer != ActiveLayer && (formIter.attribute & FRMLMSK) != 0U) {
	  continue;
	}
	auto const lastVertex = formIter.type == FRMLINE ? formIter.vertexCount - 1U : formIter.vertexCount;
	// Loop through for all line segments
	auto       length   = 0.0F;
	auto const itVertex = wrap::next(Instance->formVertices.cbegin(), formIter.vertexIndex);
	for (auto iVertex = 0U; iVertex < lastVertex; ++iVertex) {
	  auto const itCurrentVertex = wrap::next(itVertex, iVertex);
	  auto const itNextVertex    = wrap::next(itVertex, form::nxt(formIter, iVertex));
	  auto const param = findDistanceToSide(*itCurrentVertex, *itNextVertex, stitchPoint, length);
	  if (length >= minimumLength) {
		continue;
	  }
	  if (param < 0.0F && iVertex == 0) {
		// this should only happen if the Closest vertex is the start of a line (vertex 0)
		minimumLength         = length;
		ClosestFormToCursor   = iForm;
		ClosestVertexToCursor = iVertex;
		inOutFlag             = IntersectionStyles::POINT_BEFORE_LINE;
		continue;
	  }
	  // return the vertex after the intersection
	  if (param > 1.0F && iVertex == lastVertex - 1) {
		minimumLength         = length;
		ClosestFormToCursor   = iForm;
		ClosestVertexToCursor = form::nxt(formIter, iVertex);
		inOutFlag             = IntersectionStyles::POINT_AFTER_LINE;
		continue;
	  }
	  minimumLength         = length;
	  ClosestFormToCursor   = iForm;
	  ClosestVertexToCursor = form::nxt(formIter, iVertex);
	  inOutFlag             = IntersectionStyles::POINT_IN_LINE;
	}
  }
  return !util::closeEnough(minimumLength, BIGFLOAT);
}

void nufpnt(uint32_t const vertex, FRM_HEAD& formForInsert, F_POINT const stitchPoint) {
  form::fltspac(vertex + 1U, 1U);
  ++formForInsert.vertexCount;
  auto const itVertex = wrap::next(Instance->formVertices.begin(), formForInsert.vertexIndex + vertex + 1U);
  *itVertex = stitchPoint;
  if (formForInsert.satinGuideCount != 0U) {
	auto itGuide = wrap::next(Instance->satinGuides.begin(), formForInsert.satinGuideIndex);
	for (auto ind = 0U; ind < formForInsert.satinGuideCount; ++ind) {
	  if (itGuide->start > vertex) {
		++itGuide->start;
	  }
	  if (itGuide->finish > vertex) {
		++itGuide->finish;
	  }
	  ++itGuide;
	}
  }
  if (formForInsert.wordParam >= vertex + 1U) {
	++formForInsert.wordParam;
	formForInsert.wordParam %= formForInsert.vertexCount;
  }
  if (formForInsert.fillType == CONTF) {
	if (formForInsert.fillGuide.start > vertex) {
	  ++formForInsert.fillGuide.start;
	}
	if (formForInsert.fillGuide.finish > vertex) {
	  ++formForInsert.fillGuide.finish;
	}
  }
  frmToLine(formForInsert);
}

void sbord(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGELINE;
  form::bsizpar(form);
  form::refilfn(formIndex);
}

void fsclp(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  auto const& clipBuffer = Instance->clipBuffer;

  auto const clipSize = wrap::toUnsigned(clipBuffer.size());
  form.edgeType       = EDGECLIP;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(formIndex, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = ClipRectSize.cx;
  form.borderColor    = ActiveColor;
  form::bsizpar(form);
  auto itClipPoint = wrap::next(Instance->clipPoints.begin(), form.borderClipData);
  for (auto const& clip : clipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  clip::clpout(ClipRectSize.cy / 2);
  form::refilfn(formIndex);
}

void sapliq(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGEAPPL;
  if (Instance->userFlagMap.test(UserFlag::DUND)) {
	form.edgeType |= EGUND;
  }
  form.edgeSpacing = LineSpacing * HALF;
  form.borderSize  = IniFile.borderWidth;
  form::bsizpar(form);
  wrap::narrow_cast(form.borderColor, ActiveColor | gsl::narrow_cast<uint8_t>(AppliqueColor << 4U));
  if (form.type != FRMLINE) {
	if (form.fillType == SAT && form.satinGuideCount != 0U) {
	  satin::delsac(ClosestFormToCursor);
	}
  }
  form.fillType = 0U;
  form::refilfn(formIndex);
}

auto getbig(std::vector<FRM_HEAD> const&     formList,
            std::vector<F_POINT_ATTR> const& stitchBuffer) noexcept -> F_RECTANGLE {
  auto allItemsRect = F_RECTANGLE {BIGFLOAT, 0.0F, 0.0F, BIGFLOAT};
  for (auto const& iForm : formList) {
	auto const& trct    = iForm.rectangle;
	allItemsRect.left   = std::min(trct.left, allItemsRect.left);
	allItemsRect.top    = std::max(trct.top, allItemsRect.top);
	allItemsRect.right  = std::max(trct.right, allItemsRect.right);
	allItemsRect.bottom = std::min(trct.bottom, allItemsRect.bottom);
  }
  for (auto const& stitch : stitchBuffer) {
	allItemsRect.left   = std::min(stitch.x, allItemsRect.left);
	allItemsRect.top    = std::max(stitch.y, allItemsRect.top);
	allItemsRect.right  = std::max(stitch.x, allItemsRect.right);
	allItemsRect.bottom = std::min(stitch.y, allItemsRect.bottom);
  }
  return allItemsRect;
}

void setSize(F_POINT& size1, float const xyRatio) noexcept {
  // we need to maintain the aspect ratio
  if (auto const aspect = size1.x / size1.y; aspect < xyRatio) {
	size1.x = size1.y * xyRatio;
  }
  else {
	size1.y = size1.x / xyRatio;
  }
}

void resizeFormPoints(const F_POINT& stitchReference, F_POINT const& ratio) {
  auto&      formList = Instance->formList;
  auto const itVertex =
      wrap::next(Instance->formVertices.begin(), formList.operator[](ClosestFormToCursor).vertexIndex);
  auto iCurrent = SelectedFormVertices.start;
  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	auto const itCurrentVertex = wrap::next(itVertex, iCurrent);

	itCurrentVertex->x = (itCurrentVertex->x - stitchReference.x) * ratio.x + stitchReference.x;
	itCurrentVertex->y = (itCurrentVertex->y - stitchReference.y) * ratio.y + stitchReference.y;
	iCurrent           = form::pdir(formList.operator[](ClosestFormToCursor), iCurrent);
  }
  thred::setpsel();
  formList.operator[](ClosestFormToCursor).outline();
  form::refil(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void resizeBigBox(F_POINT const& stitchReference, F_POINT const& ratio) {
  auto& formList = Instance->formList;

  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
	auto&       formIter        = formList.operator[](iForm);
	auto        itVertex        = wrap::next(Instance->formVertices.begin(), formIter.vertexIndex);
	auto const& formVertexCount = formIter.vertexCount;
	for (auto iVertex = 0U; iVertex < formVertexCount; ++iVertex) {
	  itVertex->x = (itVertex->x - stitchReference.x) * ratio.x + stitchReference.x;
	  itVertex->y = (itVertex->y - stitchReference.y) * ratio.y + stitchReference.y;
	  ++itVertex;
	}
	formIter.outline();
  }
  for (auto& stitch : Instance->stitchBuffer) {
	stitch.x = (stitch.x - stitchReference.x) * ratio.x + stitchReference.x;
	stitch.y = (stitch.y - stitchReference.y) * ratio.y + stitchReference.y;
  }
  form::selal();
}

void resizeSelectedForms(F_POINT const& stitchReference, F_POINT const& ratio) {
  for (auto const selectedForm : Instance->selectedFormList) {
	auto& form = Instance->formList.operator[](selectedForm);

	auto const& formVertexCount = form.vertexCount;
	auto        itVertex        = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < formVertexCount; ++iVertex) {
	  itVertex->x = (itVertex->x - stitchReference.x) * ratio.x + stitchReference.x;
	  itVertex->y = (itVertex->y - stitchReference.y) * ratio.y + stitchReference.y;
	  ++itVertex;
	}
	form.outline();
	ClosestFormToCursor = selectedForm;
	form::refil(ClosestFormToCursor);
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void resizeForm(F_POINT const& reference, F_POINT const& ratio) {
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);

  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	itVertex->x = (itVertex->x - reference.x) * ratio.x + reference.x;
	itVertex->y = (itVertex->y - reference.y) * ratio.y + reference.y;
	++itVertex;
  }
  form::refil(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void resizeStitches(F_POINT const& reference, F_POINT const& ratio) {
  auto itStitch = wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch);
  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	itStitch->x = (itStitch->x - reference.x) * ratio.x + reference.x;
	itStitch->y = (itStitch->y - reference.y) * ratio.y + reference.y;
	++itStitch;
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void sbold(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType    = EDGEBEAN;
  form.borderColor = ActiveColor;
  form::bsizpar(form);
  form::refilfn(formIndex);
}

constexpr auto shreg(float const highValue, float const reference, float const eggRatio) noexcept -> float {
  return ((highValue - reference) * eggRatio) + reference;
}

void prpsbrd(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  if (form.vertexCount <= 2) {
	return;
  }
  clip::deleclp(formIndex);
  form.edgeType = EDGEPROPSAT;
  if (Instance->userFlagMap.test(UserFlag::DUND)) {
	form.edgeType |= EGUND;
  }
  form::bsizpar(form);
  form.borderSize  = Instance->borderWidth;
  form.edgeSpacing = LineSpacing;
  form.borderColor = ActiveColor;
  form::refilfn(formIndex);
}

void fnord() {
  // clang-format off
  auto const& currentForm   = Instance->formList.operator[](ClosestFormToCursor);
  auto const  itStartVertex = wrap::next(Instance->formVertices.begin(), currentForm.vertexIndex);
  auto const  itEndVertex   = wrap::next(itStartVertex, currentForm.vertexCount);
  // clang-format on
  std::reverse(itStartVertex, itEndVertex);
  form::refil(ClosestFormToCursor);
}

void filsclp() {
  texture::deltx(ClosestFormToCursor);
  auto& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  if (currentForm.type != SAT) {
	currentForm.wordParam = 0;
  }
  currentForm.type       = SAT;
  currentForm.fillType   = CLPF;
  currentForm.clipIndex  = clip::numclp(ClosestFormToCursor);
  auto const& clipBuffer = Instance->clipBuffer;

  currentForm.clipCount = wrap::toUnsigned(clipBuffer.size());
  auto itClipPoints     = wrap::next(Instance->clipPoints.begin(), currentForm.clipIndex);
  for (auto const& clip : clipBuffer) {
	*itClipPoints = clip;
	++itClipPoints;
  }
  form::refilfn(ClosestFormToCursor);
}

void snpfn(std::vector<uint32_t> const& xPoints, uint32_t const start, uint32_t const end, uint32_t const finish) noexcept {
  if (finish == start) {
	return;
  }
  auto const checkLength = Instance->SnapLength * Instance->SnapLength;
  for (auto current = start; current < end; ++current) {
	auto const& referencePoint = Instance->stitchBuffer.operator[](xPoints[current]);
	for (auto iPoint = current + 1U; iPoint < finish; ++iPoint) {
	  auto& checkPoint = Instance->stitchBuffer.operator[](xPoints[iPoint]);

	  auto const deltaX = checkPoint.x - referencePoint.x;
	  auto const deltaY = checkPoint.y - referencePoint.y;
	  if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < checkLength) {
		checkPoint = referencePoint;
	  }
	}
  }
}

void doTimeWindow(float const rangeX, std::vector<uint32_t> const& xPoints, std::vector<uint32_t> const& xHistogram) {
  auto const checkLength   = wrap::round<uint32_t>((Instance->SnapLength * 2.0F) + 1.0F);
  auto const stitchWindowX = thred::getStitchWindowX();
  // NOLINTNEXTLINE(readability-qualified-auto)
  auto timeWindow = CreateWindowEx(
      0, L"STATIC", nullptr, WS_CHILD | WS_VISIBLE | WS_BORDER, ButtonWidthX3, 0, stitchWindowX, ButtonHeight, ThrEdWindow, nullptr, ThrEdInstance, nullptr);

  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const timeDC       = GetDC(timeWindow);
  auto const timeStep     = wrap::toFloat(stitchWindowX) / rangeX;
  auto       timePosition = 0.0F;
  auto&      formLines    = Instance->formLines;
  formLines.clear();
  formLines.push_back(POINT {});
  formLines.push_back(POINT {0, ButtonHeight});
  SelectObject(timeDC, thred::getUserPen(0U));
  for (auto iColumn = 1U; iColumn < wrap::round<uint32_t>(rangeX) - checkLength - 1U; ++iColumn) {
	snpfn(xPoints,
	      xHistogram[iColumn],
	      xHistogram[wrap::toSize(iColumn) + 1U],
	      xHistogram[wrap::toSize(iColumn) + checkLength]);
	wrap::polyline(timeDC, formLines.data(), LNPNTS);
	timePosition += timeStep;
	formLines[0].x = formLines[1].x = std::lround(timePosition);
  }
  if (nullptr == timeWindow) {
	return;
  }
  DestroyWindow(timeWindow);
  timeWindow = nullptr;
}

void snp(uint32_t const start, uint32_t const finish) {
  auto range = F_POINT {};
  thred::chkrng(range);
  auto xPoints = std::vector<uint32_t> {};
  xPoints.resize(Instance->stitchBuffer.size());
  auto xHistogram = std::vector<uint32_t> {};
  xHistogram.resize(wrap::round<size_t>(range.x) + 1U);
  auto const attribute = (ClosestFormToCursor << 4U) & FRMSK;
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  if ((Instance->stitchBuffer.operator[](iStitch).attribute & NOTFRM) == 0U &&
	      (Instance->stitchBuffer.operator[](iStitch).attribute & FRMSK) == attribute) {
		auto const iColumn = wrap::floor<uint32_t>(Instance->stitchBuffer.operator[](iStitch).x);
		++xHistogram[iColumn];
	  }
	}
  }
  else {
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  auto const iColumn = wrap::floor<uint32_t>(Instance->stitchBuffer.operator[](iStitch).x);
	  ++xHistogram[iColumn];
	}
  }
  auto       accumulator = 0U;
  auto const endColumn   = wrap::round<uint32_t>(range.x);
  for (auto iColumn = 0U; iColumn < endColumn; ++iColumn) {
	auto const value    = xHistogram[iColumn];
	xHistogram[iColumn] = accumulator;
	accumulator += value;
  }
  xHistogram[endColumn] = accumulator;
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto iStitch = 0U;
	for (auto const& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & NOTFRM) == 0U && (stitch.attribute & FRMSK) == attribute) {
		auto const iColumn             = wrap::floor<uint32_t>(stitch.x);
		xPoints[xHistogram[iColumn]++] = iStitch;
	  }
	  ++iStitch;
	}
  }
  else {
	auto iStitch = 0U;
	for (auto const& stitch : Instance->stitchBuffer) {
	  auto const iColumn             = wrap::floor<uint32_t>(stitch.x);
	  xPoints[xHistogram[iColumn]++] = iStitch;
	  ++iStitch;
	}
  }
  doTimeWindow(range.x, xPoints, xHistogram);
}

void dufcntr(F_POINT& center) noexcept {
  auto const& formList = Instance->formList;

  auto bigRect = formList.operator[](Instance->selectedFormList.front()).rectangle;
  for (auto const selectedForm : Instance->selectedFormList) {
	auto const formRect = formList.operator[](selectedForm).rectangle;
	bigRect.left        = std::min(formRect.left, bigRect.left);
	bigRect.top         = std::max(formRect.top, bigRect.top);
	bigRect.right       = std::max(formRect.right, bigRect.right);
	bigRect.bottom      = std::min(formRect.bottom, bigRect.bottom);
  }
  center = F_POINT {wrap::midl(bigRect.right, bigRect.left), wrap::midl(bigRect.top, bigRect.bottom)};
}

void rotentr(float const rotationAngle) {
  displayText::showMessage(IDS_ROTA, rotationAngle * RADDEGF);
  Instance->stateMap.set(StateFlag::NUMIN);
  thred::numWnd();
}

void fnagain(float const rotationAngle) {
  if (!Instance->stateMap.test(StateFlag::FORMSEL) && !Instance->stateMap.test(StateFlag::GRPSEL) &&
      Instance->selectedFormList.empty() && !Instance->stateMap.test(StateFlag::BIGBOX) &&
      !Instance->stateMap.test(StateFlag::FPSEL)) {
	displayText::alrotmsg();
	return;
  }
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
}

void duprot(float const rotationAngle) {
  thred::savdo();
  adfrm(ClosestFormToCursor);
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
  form::refil(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::FORMSEL);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void duprotfs(float const rotationAngle) {
  auto const rotationCenter = form::rotpar();
  for (auto const selectedForm : Instance->selectedFormList) {
	adfrm(selectedForm);
	form::refil(wrap::toUnsigned(Instance->formList.size() - 1U));
  }
  thred::rotfn(rotationAngle, rotationCenter);
}

void duprots(float const rotationAngle, F_POINT const& rotationCenter) {
  thred::rngadj();
  ClosestPointIndex = wrap::toUnsigned(Instance->stitchBuffer.size());
  Instance->stitchBuffer.reserve(ClosestPointIndex + (wrap::toSize(GroupEndStitch) + 1U - GroupStartStitch));
  Instance->stitchBuffer.insert(Instance->stitchBuffer.end(),
                                wrap::next(Instance->stitchBuffer.cbegin(), GroupStartStitch),
                                wrap::next(Instance->stitchBuffer.cbegin(), GroupEndStitch + 1U));
  for (auto currentStitch = wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex);
       currentStitch != Instance->stitchBuffer.end();
       ++currentStitch) {
	currentStitch->attribute &= ~(FRMSK | TYPMSK);
  }
  GroupStitchIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
  thred::rngadj();
  thred::rotfn(rotationAngle, rotationCenter);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void cplayfn(uint32_t const iForm, uint32_t const layer) {
  auto& formList = Instance->formList;

  auto currentForm = formList.operator[](iForm); // intended copy

  auto const originalVertexIndex = currentForm.vertexIndex;
  currentForm.vertexIndex        = thred::adflt(currentForm.vertexCount);
  auto const itVertex            = wrap::next(Instance->formVertices.cbegin(), originalVertexIndex);
  std::copy(itVertex,
            wrap::next(itVertex, currentForm.vertexCount),
            wrap::next(Instance->formVertices.begin(), currentForm.vertexIndex));
  if (currentForm.type == SAT && currentForm.satinGuideCount != 0U) {
	auto const originalGuide    = currentForm.satinGuideIndex;
	currentForm.satinGuideIndex = wrap::toUnsigned(Instance->satinGuides.size());

	auto const itStartGuide = wrap::next(Instance->satinGuides.cbegin(), originalGuide);
	auto const itEndGuide   = wrap::next(itStartGuide, currentForm.satinGuideCount);
	auto const destination  = Instance->satinGuides.end();
	Instance->satinGuides.insert(destination, itStartGuide, itEndGuide);
  }
  currentForm.clipEntries   = 0;
  currentForm.fillType      = 0;
  currentForm.clipCount     = 0;
  currentForm.edgeType      = 0;
  currentForm.texture.index = 0;
  currentForm.attribute &= NFRMLMSK;
  currentForm.attribute |= layer << FLAYSHFT;
  currentForm.extendedAttribute = 0;
  currentForm.squareEnd(Instance->userFlagMap.test(UserFlag::SQRFIL));
  formList.push_back(currentForm);
}

void frmpnts(uint32_t const type) {
  if (Instance->stitchBuffer.empty()) {
	ClosestPointIndex = 0;
	GroupStitchIndex  = 0;
	return;
  }
  auto       iStitch = 0U;
  auto const trg     = (ClosestFormToCursor << 4U) | type;
  for (auto const& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & (ALTYPMSK | FRMSK)) == trg) {
	  break;
	}
	++iStitch;
  }
  ClosestPointIndex = iStitch;
  auto bFlag        = false;
  for (auto itStitch = wrap::next(Instance->stitchBuffer.begin(), ClosestPointIndex);
       itStitch != Instance->stitchBuffer.end();
       ++itStitch) {
	if ((itStitch->attribute & (ALTYPMSK | FRMSK)) != trg) {
	  GroupStitchIndex = wrap::distance<uint32_t>(Instance->stitchBuffer.begin(), itStitch) - 1U;
	  bFlag            = true;
	  break;
	}
  }
  if (!bFlag) {
	GroupStitchIndex = wrap::toUnsigned(Instance->stitchBuffer.size() - 1U);
  }
}

void bholbrd(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  form.borderSize = Instance->borderWidth;
  form::bsizpar(form);
  form.edgeType    = EDGEBHOL;
  form.edgeSpacing = LineSpacing;
  form.borderColor = ActiveColor;
  form::savblen(ButtonholeCornerLength);
  form::refilfn(formIndex);
}

void fspic(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  auto const& clipBuffer = Instance->clipBuffer;

  auto const clipSize = wrap::toUnsigned(clipBuffer.size());
  form.edgeType       = EDGEPICOT;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = thred::getPicotSpacing();
  form.borderColor    = ActiveColor;
  form::bsizpar(form);
  form::savplen(ButtonholeCornerLength);
  auto itClipPoints = wrap::next(Instance->clipPoints.begin(), form.borderClipData);
  for (auto const& clip : clipBuffer) {
	*itClipPoints = clip;
	++itClipPoints;
  }
  form::refilfn(formIndex);
}

auto contsf(uint32_t const formIndex) -> bool {
  if (auto& form = Instance->formList.operator[](formIndex); form.vertexCount > 4) {
	clip::delclps(formIndex);
	texture::deltx(formIndex);
	form::chkcont();
	form.attribute |= gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
	form.fillColor   = ActiveColor;
	form.fillSpacing = LineSpacing;
	form::fsizpar(form);
	form::refilfn(formIndex);
	return true;
  }
  return false;
}

void shrnks() {
  auto  clipRect    = F_RECTANGLE {};
  auto  lengths     = std::vector<float> {};
  auto  deltas      = std::vector<F_POINT> {};
  auto& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  lengths.reserve(currentForm.vertexCount);
  auto const deltaCount = currentForm.vertexCount - 1U;
  deltas.reserve(deltaCount);
  clip::oclp(clipRect, currentForm.borderClipData, currentForm.clipEntries);
  auto const itVertex = wrap::next(Instance->formVertices.begin(), currentForm.vertexIndex);
  auto const vMax     = gsl::narrow<ptrdiff_t>(currentForm.vertexCount - 1U);
  for (auto iVertex = ptrdiff_t {}; iVertex < vMax; ++iVertex) {
	auto const& thisVertex = itVertex[iVertex];
	auto const& nextVertex = itVertex[iVertex + 1];
	deltas.emplace_back(nextVertex.x - thisVertex.x, nextVertex.y - thisVertex.y);
	lengths.emplace_back(std::hypot(deltas.back().x, deltas.back().y));
  }
  auto length = lengths.begin();
  auto delta  = deltas.begin();
  for (auto iVertex = ptrdiff_t {}; iVertex < vMax; ++iVertex) {
	auto const  count      = std::floor(*length / ClipRectSize.cx);
	auto const  ratio      = (ClipRectSize.cx * count + 0.004F) / *length;
	auto const& thisVertex = itVertex[iVertex];
	auto&       nextVertex = itVertex[iVertex + 1];
	nextVertex             = thisVertex + *delta * ratio;
	++length;
	++delta;
  }
  currentForm.outline();
  form::refil(ClosestFormToCursor);
}

void dufdat(std::vector<F_POINT>&  tempClipPoints,
            std::vector<SAT_CON>&  tempGuides,
            std::vector<F_POINT>&  destFormVertices,
            std::vector<FRM_HEAD>& destFormList,
            uint32_t const         formIndex,
            uint32_t&              formRelocationIndex,
            uint32_t&              formSourceIndex) {
  auto& dest = destFormList.operator[](formRelocationIndex);

  auto const& formList = Instance->formList;

  dest = formList.operator[](formIndex);
  ++formRelocationIndex;
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), dest.vertexIndex);
  std::copy(itVertex, wrap::next(itVertex, dest.vertexCount), wrap::next(destFormVertices.begin(), formSourceIndex));
  dest.vertexIndex = formSourceIndex;
  formSourceIndex += dest.vertexCount;
  if (dest.satinGuideCount != 0U) {
	auto const itStartGuide = wrap::next(Instance->satinGuides.cbegin(), dest.satinGuideIndex);
	auto const itEndGuide   = wrap::next(itStartGuide, dest.satinGuideCount);
	tempGuides.insert(tempGuides.end(), itStartGuide, itEndGuide);
	dest.satinGuideIndex = wrap::toUnsigned(tempGuides.size() - dest.satinGuideCount);
  }
  auto const& form = formList.operator[](formIndex);
  if (form.isEdgeClipX()) {
	auto const itStartClip = wrap::next(Instance->clipPoints.cbegin(), dest.borderClipData);
	auto const itEndClip   = wrap::next(itStartClip, dest.clipEntries);
	tempClipPoints.insert(tempClipPoints.end(), itStartClip, itEndClip);
	dest.borderClipData = wrap::toUnsigned(tempClipPoints.size() - dest.clipEntries);
  }
  if (form.isClipX()) {
	auto const itStartClip = wrap::next(Instance->clipPoints.cbegin(), dest.clipIndex);
	auto const itEndClip   = wrap::next(itStartClip, dest.clipCount);
	tempClipPoints.insert(tempClipPoints.end(), itStartClip, itEndClip);
	dest.clipIndex = wrap::toUnsigned(tempClipPoints.size() - dest.clipCount);
  }
}

void stchfrm(uint32_t const formIndex, uint32_t& attribute) noexcept {
  attribute &= NFRMSK;
  attribute |= formIndex << FRMSHFT;
}

void srtf(std::vector<F_POINT_ATTR> const& tempStitchBuffer, uint32_t const start, uint32_t const finish) {
  if (start == finish) {
	return;
  }
  auto const& formList = Instance->formList;

  auto stitchHistogram = std::vector<uint32_t> {};
  stitchHistogram.resize(formList.size() << 2U);
  for (auto iStitch = start; iStitch < finish; ++iStitch) {
	++stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)];
  }
  auto stitchAccumulator = start;
  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size() << 2U); ++iForm) {
	auto const value       = stitchHistogram[iForm];
	stitchHistogram[iForm] = stitchAccumulator;
	stitchAccumulator += value;
  }
  for (auto iStitch = start; iStitch < finish; ++iStitch) {
	Instance->stitchBuffer.operator[](stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++) =
	    tempStitchBuffer[iStitch];
  }
}

constexpr auto duat(uint32_t const attribute) -> uint32_t {
  auto const type = ((attribute >> TYPSHFT) + 1U) & 3U;
  auto const frm  = (attribute & FRMSK) >> 2U;
  return type | frm;
}

auto bean(uint32_t const start, uint32_t const finish) -> uint32_t {
  auto const stitchRange      = (finish - start) * 3U; // each bean is 3 stitches
  auto       highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.reserve(stitchRange);
  auto iSourceStitch = start;
  auto beanCount     = 0U;
  for (auto loop = 0; loop < 2; ++loop) {
	auto const& stitch     = Instance->stitchBuffer.operator[](iSourceStitch);
	auto const& stitchFwd1 = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) + 1U);
	auto const& stitchFwd2 = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) + 2U);
	highStitchBuffer.push_back(stitch);
	if (!util::closeEnough(stitchFwd2.x, stitch.x) || !util::closeEnough(stitchFwd2.y, stitch.y)) {
	  highStitchBuffer.push_back(stitchFwd1);
	  highStitchBuffer.push_back(stitch);
	  beanCount += 2U;
	}
	++iSourceStitch;
  }
  while (iSourceStitch < finish - 1) {
	auto const& stitch      = Instance->stitchBuffer.operator[](iSourceStitch);
	auto const& stitchFwd1  = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) + 1U);
	auto const& stitchFwd2  = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) + 2U);
	auto const& stitchBack2 = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) - 2U);
	highStitchBuffer.push_back(stitch);
	if ((!util::closeEnough(stitchFwd2.x, stitch.x) || !util::closeEnough(stitchFwd2.y, stitch.y)) &&
	    (!util::closeEnough(stitchBack2.x, stitch.x) || !util::closeEnough(stitchBack2.y, stitch.y))) {
	  highStitchBuffer.push_back(stitchFwd1);
	  highStitchBuffer.push_back(stitch);
	  beanCount += 2U;
	}
	++iSourceStitch;
  }
  auto const& stitch      = Instance->stitchBuffer.operator[](iSourceStitch);
  auto const& stitchFwd1  = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) + 1U);
  auto const& stitchBack2 = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) - 2U);
  highStitchBuffer.push_back(stitch);
  if (!util::closeEnough(stitchBack2.x, stitch.x) || !util::closeEnough(stitchBack2.y, stitch.y)) {
	highStitchBuffer.push_back(stitchFwd1);
	highStitchBuffer.push_back(stitch);
	beanCount += 2U;
  }
  // now copy stitches back up to the end of the original group
  std::copy(highStitchBuffer.cbegin(),
            wrap::next(highStitchBuffer.cbegin(), finish - start),
            wrap::next(Instance->stitchBuffer.begin(), start));
  // and then insert the remainder of the new stitches
  Instance->stitchBuffer.insert(wrap::next(Instance->stitchBuffer.cbegin(), finish),
                                wrap::next(highStitchBuffer.begin(), finish - start),
                                highStitchBuffer.end());
  return beanCount;
}

void unbean(uint32_t const start, uint32_t& finish) {
  auto const stitchRange      = ((finish - start) / 3U) + 1U; // each bean is 3 stitches
  auto       highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.reserve(stitchRange);
  auto lastStitch    = finish;
  lastStitch         = std::min(lastStitch, wrap::toUnsigned(Instance->stitchBuffer.size()) - 3U);
  auto iSourceStitch = start;
  for (auto skip = 0U; iSourceStitch <= lastStitch; ++iSourceStitch) {
	if (skip != 0U) {
	  --skip;
	  continue;
	}
	auto const& stitch     = Instance->stitchBuffer.operator[](iSourceStitch);
	auto const& stitchFwd2 = Instance->stitchBuffer.operator[](wrap::toSize(iSourceStitch) + 2U);
	highStitchBuffer.push_back(stitch);
	if (util::closeEnough(stitch.x, stitchFwd2.x) && util::closeEnough(stitch.y, stitchFwd2.y)) {
	  skip = 2U; // skip the next two stitches
	}
  }
  if (finish != lastStitch) {
	while (iSourceStitch != finish + 1U) {
	  highStitchBuffer.push_back(Instance->stitchBuffer.operator[](iSourceStitch++));
	}
  }
  if (wrap::toSize(finish) - start > highStitchBuffer.size()) {
	auto const itStartStitch = wrap::next(Instance->stitchBuffer.begin(), start);
	std::ranges::copy(highStitchBuffer, itStartStitch);
	Instance->stitchBuffer.erase(
	    wrap::next(Instance->stitchBuffer.cbegin(), start + highStitchBuffer.size()),
	    wrap::next(Instance->stitchBuffer.cbegin(), finish + 1U));
  }
  finish = start + wrap::toUnsigned(highStitchBuffer.size() - 1U);
}

auto spltlin() -> bool {
  auto& formList = Instance->formList;

  auto& srcForm = formList.operator[](ClosestFormToCursor);
  if (ClosestVertexToCursor < 1 || srcForm.vertexCount - ClosestVertexToCursor < 2) {
	return false;
  }
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), srcForm.vertexIndex + ClosestVertexToCursor);
  nufpnt(ClosestVertexToCursor, srcForm, *itVertex);
  frmToLine(srcForm);
  formList.insert(wrap::next(formList.cbegin(), ClosestFormToCursor), 1, srcForm);
  // insert may invalidate the srcForm reference, so build a new one
  auto& form       = formList.operator[](ClosestFormToCursor);
  auto& dstForm    = formList.operator[](wrap::toSize(ClosestFormToCursor) + 1U);
  form.vertexCount = ClosestVertexToCursor + 1U;
  dstForm.vertexCount -= form.vertexCount;
  dstForm.vertexIndex = form.vertexIndex + form.vertexCount;
  form.outline();
  dstForm.outline();
  if (form.isEdgeClip()) {
	form::clpspac(form.borderClipData, form.clipEntries);
	auto const maxForm = formList.size();
	for (auto iForm = ClosestFormToCursor + 1U; iForm < maxForm; ++iForm) {
	  formList.operator[](iForm).borderClipData += form.clipEntries;
	}
  }
  form::stchadj();
  return true;
}

void chan(uint32_t const formIndex) {
  auto& currentForm              = Instance->formList.operator[](formIndex);
  currentForm.borderColor        = ActiveColor;
  currentForm.edgeSpacing        = IniFile.chainSpace;
  currentForm.borderSize         = Instance->borderWidth;
  currentForm.edgeStitchLen      = IniFile.chainRatio;
  currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
  currentForm.minBorderStitchLen = MinStitchLength;
  currentForm.edgeType = Instance->stateMap.test(StateFlag::LINCHN) ? EDGELCHAIN : EDGEOCHAIN;
  form::refilfn(formIndex);
}

void fsclpx(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::deleclp(formIndex);
  auto const& clipBuffer = Instance->clipBuffer;

  auto const clipSize = wrap::toUnsigned(clipBuffer.size());
  form.edgeType       = EDGECLIPX;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(formIndex, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = ClipRectSize.cx;
  form.borderColor    = ActiveColor;
  form::bsizpar(form);
  auto itClipPoint = wrap::next(Instance->clipPoints.begin(), form.borderClipData);
  for (auto const& clip : clipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  clip::duxclp(form);
  form::refilfn(formIndex);
}

auto getLayerPen(uint32_t const layer) noexcept(!std::is_same_v<ptrdiff_t, int>) -> HPEN {
  auto const itLayerPen = wrap::next(LayerPen.begin(), layer);
  return *itLayerPen;
}

void chksid(FRM_HEAD const&             form,
            uint32_t const              vertexIndex,
            uint32_t const              clipIntersectSide,
            std::vector<F_POINT> const& currentFormVertices) {
  if (clipIntersectSide == vertexIndex) {
	return;
  }
  auto const itVertex = currentFormVertices.cbegin();
  if (auto const vertexCount = currentFormVertices.size();
      (wrap::toSize(vertexIndex) - clipIntersectSide + vertexCount) % vertexCount < vertexCount / 2) {
	auto       iVertex = form::nxt(form, clipIntersectSide);
	auto const limit   = form::nxt(form, vertexIndex);
	while (iVertex != limit) {
	  auto const itThisVertex = wrap::next(itVertex, iVertex);
	  Instance->oSequence.push_back(*itThisVertex);
	  iVertex = form::nxt(form, iVertex);
	}
	return;
  }
  auto iVertex = clipIntersectSide;
  while (iVertex != vertexIndex) {
	auto const itThisVertex = wrap::next(itVertex, iVertex);
	Instance->oSequence.push_back(*itThisVertex);
	iVertex = form::prv(form, iVertex);
  }
}
} // namespace

auto form::chkmax(uint32_t const arg0, uint32_t const arg1) noexcept -> bool {
  constexpr auto MAXMSK = 0xffff0000U; // for checking for greater than 65536
  if ((arg0 & MAXMSK) != 0U) {
	return true;
  }
  if ((arg1 & MAXMSK) != 0U) {
	return true;
  }
  if (((arg1 + arg0) & MAXMSK) != 0U) {
	return true;
  }
  return false;
}

void form::fltspac(uint32_t const vertexOffset, uint32_t const count) {
  auto& formList = Instance->formList;

  auto const currentVertex = formList.operator[](ClosestFormToCursor).vertexIndex + vertexOffset;
  auto const startVertex   = wrap::next(Instance->formVertices.cbegin(), currentVertex);
  auto constexpr VAL       = F_POINT {};
  Instance->formVertices.insert(startVertex, count, VAL);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
	auto& form = formList.operator[](iForm);
	form.vertexIndex += count;
  }
}

void form::delflt(uint32_t const formIndex) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto& formList = Instance->formList;

  auto itForm = wrap::next(formList.begin(), formIndex);
  if (itForm->vertexCount == 0U) {
	return;
  }
  auto const vertexCount   = itForm->vertexCount;
  auto const itStartVertex = wrap::next(Instance->formVertices.cbegin(), itForm->vertexIndex);
  auto const itEndVertex   = wrap::next(itStartVertex, itForm->vertexCount);
  Instance->formVertices.erase(itStartVertex, itEndVertex);
  for (++itForm; itForm < formList.end(); ++itForm) {
	itForm->vertexIndex -= vertexCount;
  }
}

void form::delmfil(uint32_t const formIndex) {
  if (Instance->formList.operator[](formIndex).isTexture()) {
	texture::deltx(formIndex);
  }
  clip::delmclp(formIndex);
  // find the first stitch to delete
  auto const codedForm = formIndex << FRMSHFT;
  if (auto const firstStitch = std::ranges::find_if(Instance->stitchBuffer,
                                                    [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                                                  return (stitch.attribute & (FRMSK | NOTFRM)) == codedForm &&
	                                                         (stitch.attribute & (TYPFRM | FTHMSK)) != 0U;
                                                    });
      firstStitch != Instance->stitchBuffer.end()) {
	// we found the first stitch, so now delete the stitches in the form
	Instance->stitchBuffer.erase(std::remove_if(firstStitch,
	                                            Instance->stitchBuffer.end(),
	                                            [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                                              return (stitch.attribute & FRMSK) == codedForm;
	                                            }),
	                             Instance->stitchBuffer.end());
  }
}

void form::fsizpar(FRM_HEAD& form) noexcept {
  form.stitchLength     = UserStitchLength;
  form.maxFillStitchLen = IniFile.maxStitchLength;
  form.minFillStitchLen = MinStitchLength;
}

void form::chkcont() {
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  fsizpar(form);
  if (form.fillType != CONTF) {
	if (form.satinGuideCount != 0U) {
	  auto shortestGuideIndex = 0U;
	  auto minimumLength      = MAXDWORD;
	  auto itGuide            = wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		if (auto const length = itGuide->finish - itGuide->start; length < minimumLength) {
		  minimumLength      = length;
		  shortestGuideIndex = iGuide;
		}
		++itGuide;
	  }
	  auto const shortestGuide =
	      wrap::next(Instance->satinGuides.cbegin(), form.satinGuideIndex + shortestGuideIndex);

	  form.fillGuide = *shortestGuide;
	  satin::delsac(ClosestFormToCursor);
	}
	else {
	  form.fillGuide = SAT_CON {1U, form.vertexCount - 2U};
	}
  }
  form.attribute |= FRECONT;
  form.type     = FRMLINE;
  form.fillType = CONTF;
}

void form::ispcdclp() {
  Instance->stateMap.reset(StateFlag::WASPCDCLP);
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  if (auto const* clipMemory = tfc::getPCDClipMemory(); clipMemory != nullptr) {
	Instance->stateMap.set(StateFlag::WASPCDCLP);
  }
  CloseClipboard();
}

auto form::sfCor2px(F_POINT const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    ((stitchPoint.y - ZoomRect.bottom) * ZoomRatio.y))};
}

void form::frmlin(std::vector<F_POINT> const& vertices) {
  auto const vertexMax = vertices.size();
  if (vertexMax == 0U) {
	return;
  }
  auto& formLines = Instance->formLines;
  formLines.clear();
  formLines.reserve(vertexMax);
  for (auto iVertex = 0U; iVertex < vertexMax; ++iVertex) {
	formLines.push_back(POINT {std::lround((vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x),
	                           std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                       ((vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y))});
  }
  formLines.push_back(POINT {std::lround((vertices[0].x - ZoomRect.left) * ZoomRatio.x),
                             std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
                                         ((vertices[0].y - ZoomRect.bottom) * ZoomRatio.y))});
}

void form::dufrm() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, Instance->formLines.data(), NewFormVertexCount);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::unfrm() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOFRM)) {
	dufrm();
  }
}

void form::mdufrm() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  auto const& formLines = Instance->formLines;
  if (Instance->formList.operator[](ClosestFormToCursor).type == FRMLINE) {
	wrap::polyline(StitchWindowDC, formLines.data(), NewFormVertexCount - 1);
  }
  else {
	wrap::polyline(StitchWindowDC, formLines.data(), NewFormVertexCount);
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void form::munfrm() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOFRM)) {
	mdufrm();
  }
}

void form::setfrm() {
  auto& formList = Instance->formList;

  if (formList.empty()) {
	return;
  }
  thred::rats();
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  auto const point    = px2stchf(Instance->formLines.front());
  auto const newCount = NewFormVertexCount - 1U; // -1 to account for the last point being the same as the first
  auto const spVertices = gsl::span(Instance->formVertices).subspan(formList.back().vertexIndex, newCount);
  auto const delta = F_POINT {point.x - spVertices.front().x, point.y - spVertices.front().y};

  auto minX = BIGFLOAT;
  auto minY = BIGFLOAT;
  auto maxX = LOWFLOAT;
  auto maxY = LOWFLOAT;
  for (auto& vertex : spVertices) {
	vertex += delta;
	minX = std::min(minX, vertex.x);
	minY = std::min(minY, vertex.y);
	maxX = std::max(maxX, vertex.x);
	maxY = std::max(maxY, vertex.y);
  }
  formList.back().rectangle = F_RECTANGLE {minX, maxY, maxX, minY};
  Instance->stateMap.reset(StateFlag::FORMIN);
  Instance->stateMap.set(StateFlag::INIT);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::form() {
  displayText::shoMsg(displayText::loadStr(IDS_FMEN), true);
  Instance->stateMap.set(StateFlag::FORMIN);
  Instance->stateMap.reset(StateFlag::INSRT);
  thred::duzrat();
}

// ReSharper disable CppParameterMayBeConst
void form::selsqr(POINT const& controlPoint, HDC hDC) {
  auto line = std::array<POINT, SQPNTS> {};

  auto const offset = thred::duScale(gsl::narrow<int32_t>(IniFile.formVertexSizePixels));
  line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
  line[0].y = line[1].y = controlPoint.y - offset;
  line[1].x = line[2].x = controlPoint.x + offset;
  line[2].y = line[3].y = controlPoint.y + offset;
  line[4].y             = controlPoint.y - offset;
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
}
// ReSharper restore CppParameterMayBeConst

// ReSharper disable CppParameterMayBeConst
void form::ritfrct(uint32_t const iForm, HDC hDC) {
  auto pixelOutline = std::array<POINT, OUTPNTS> {};

  std::array<F_POINT, OUTPNTS> formOutline;
  thred::ratsr();
  SelectObject(StitchWindowDC, FormPen);
  SetROP2(StitchWindowDC, R2_XORPEN);
  auto const& rectangle = Instance->formList.operator[](iForm).rectangle;
  SelectObject(hDC, FormSelectedPen);
  formOutline[PTL].x = formOutline[PBL].x = formOutline[PLM].x = formOutline[PTLE].x = rectangle.left;
  formOutline[PTL].y = formOutline[PTM].y = formOutline[PTR].y = formOutline[PTLE].y = rectangle.top;
  formOutline[PTR].x = formOutline[PRM].x = formOutline[PBR].x = rectangle.right;
  formOutline[PBR].y = formOutline[PBM].y = formOutline[PBL].y = rectangle.bottom;
  formOutline[PTM].x = formOutline[PBM].x = wrap::midl(rectangle.right, rectangle.left);
  formOutline[PRM].y = formOutline[PLM].y = wrap::midl(rectangle.top, rectangle.bottom);

  auto ipixelOutline = pixelOutline.begin();
  for (auto& controlPoint : formOutline) {
	*ipixelOutline = sfCor2px(controlPoint);
	++ipixelOutline;
  }
  wrap::polyline(hDC, pixelOutline.data(), wrap::toUnsigned(pixelOutline.size()));
  auto const spOutline = gsl::span {pixelOutline};
  // drawing the boxes on corners and sides, so don't overwrite the first box which will "erase" it
  for (auto const subsp = spOutline.subspan(0, spOutline.size() - 1); auto const& controlPoint : subsp) {
	selsqr(controlPoint, hDC);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  if (Instance->stateMap.testAndReset(StateFlag::GRPSEL)) {
	Instance->stateMap.reset(StateFlag::SELSHO);
	Instance->searchLine.clear();
	Instance->searchLine.shrink_to_fit();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}
// ReSharper restore CppParameterMayBeConst

void form::delfrms() {
  thred::savdo();
  Instance->formVertices.clear();
  Instance->clipPoints.clear();
  Instance->formList.clear();
  Instance->satinGuides.clear();
  for (auto& stitch : Instance->stitchBuffer) {
	stitch.attribute &= NFRM_NTYP;
	stitch.attribute |= NOTFRM;
  }
}

void form::fselrct(uint32_t const iForm) noexcept(std::is_same_v<size_t, uint32_t>) {
  // clang-format off
  auto const& form = Instance->formList.operator[](iForm);
  std::array<F_POINT, SQPNTS> formOutline;
  auto line        = std::array<POINT, SQPNTS> {};
  // clang-format on
  formOutline[0].x = formOutline[3].x = formOutline[4].x = form.rectangle.left;
  formOutline[0].y = formOutline[1].y = formOutline[4].y = form.rectangle.top;
  formOutline[1].x = formOutline[2].x = form.rectangle.right;
  formOutline[2].y = formOutline[3].y = form.rectangle.bottom;

  auto iFormOutline = formOutline.begin();

  auto minX = SelectedFormsRect.left;
  auto maxY = SelectedFormsRect.top;
  auto maxX = SelectedFormsRect.right;
  auto minY = SelectedFormsRect.bottom;
  for (auto& point : line) {
	point.x = thred::scaleHorizontal(iFormOutline->x - ZoomRect.left);
	point.y = thred::scaleVertical(ZoomRect.top - iFormOutline->y);
	minX    = std::min(minX, point.x);
	minY    = std::min(minY, point.y);
	maxX    = std::max(maxX, point.x);
	maxY    = std::max(maxY, point.y);
	++iFormOutline;
  }

  SelectedFormsRect = RECT {minX, maxY, maxX, minY};
  // ReSharper disable once CppUnreachableCode
  if (OUTL_ALL) {
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
}

void form::rct2sel(RECT const& rectangle, std::vector<POINT>& line) noexcept {
  line[PTL].x = line[PBL].x = line[PLM].x = line[PTLE].x = rectangle.left;
  line[PTL].y = line[PTM].y = line[PTR].y = line[PTLE].y = rectangle.top;
  line[PTR].x = line[PRM].x = line[PBR].x = rectangle.right;
  line[PBR].y = line[PBM].y = line[PBL].y = rectangle.bottom;
  line[PTM].x = line[PBM].x = ((rectangle.right - rectangle.left) / 2) + rectangle.left;
  line[PRM].y = line[PLM].y = ((rectangle.bottom - rectangle.top) / 2) + rectangle.top;
}

void form::dubig() {
  rct2sel(SelectedFormsRect, Instance->selectedFormsLine);
  thred::selectAllPen();
  wrap::polyline(StitchWindowMemDC,
                 Instance->selectedFormsLine.data(),
                 wrap::toUnsigned(Instance->selectedFormsLine.size()));
  for (auto iPoint = 0U; iPoint < Instance->selectedFormsLine.size() - 1U; ++iPoint) {
	selsqr(Instance->selectedFormsLine.operator[](iPoint), StitchWindowMemDC);
  }
}

// ReSharper disable CppParameterMayBeConst
void form::dupsel(HDC hDC) {
  SelectObject(hDC, FormPen);
  SetROP2(hDC, R2_XORPEN);
  wrap::polyline(
      hDC, Instance->selectedPointsLine.data(), wrap::toUnsigned(Instance->selectedPointsLine.size()));
  for (auto iPoint = 0U; iPoint < Instance->selectedPointsLine.size() - 1U; ++iPoint) {
	selsqr(Instance->selectedPointsLine.operator[](iPoint), hDC);
  }
  frmx(EndPointCross, hDC);
}
// ReSharper restore CppParameterMayBeConst

void form::unpsel() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOPSEL)) {
	dupsel(StitchWindowDC);
  }
}

auto form::sRct2px(F_RECTANGLE const& stitchRect) -> RECT {
  return RECT {wrap::ceil<int32_t>((stitchRect.left - ZoomRect.left) * ZoomRatio.x),
               wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                   ((stitchRect.top - ZoomRect.bottom) * ZoomRatio.y)),
               wrap::ceil<int32_t>((stitchRect.right - ZoomRect.left) * ZoomRatio.x),
               wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                   ((stitchRect.bottom - ZoomRect.bottom) * ZoomRatio.y))};
}

void form::drwfrm() {
  Instance->stateMap.reset(StateFlag::SHOMOV);
  Instance->stateMap.reset(StateFlag::SHOPSEL);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  thred::ratsr();
  thred::duzrat();
  auto const maxForm = Instance->formList.size();
  for (auto iForm = 0U; iForm < maxForm; ++iForm) {
	auto&       formLines = Instance->formLines;
	auto const& form      = Instance->formList.operator[](iForm);
	frmToLine(form);
	if (formLines.empty()) {
	  continue;
	}
	// NOLINTNEXTLINE(hicpp-signed-bitwise)
	auto const layer = gsl::narrow_cast<uint8_t>((form.attribute & FRMLMSK) >> 1U);
	if (ActiveLayer != 0U && layer != 0U && layer != ActiveLayer) {
	  continue;
	}
	auto lastPoint = 0U;
	if (form.type == SAT) {
	  if ((form.attribute & FRMEND) != 0U) { // if the form has an end guide, draw it
		SelectObject(StitchWindowMemDC, FormPen3px);
		wrap::polyline(StitchWindowMemDC, formLines.data(), LNPNTS);
		lastPoint = 1;
	  }
	  if (form.wordParam != 0U) { // if the form has a start guide, draw it
		drawStartGuide(form, layer, lastPoint);
	  }
	  if (form.satinGuideCount != 0U) { // if the form has satin guides, draw them
		drawGuides(form);
	  }
	}
	SelectObject(StitchWindowMemDC, getLayerPen(layer));
	if (form.type == FRMLINE) { // if the form is a line, draw it
	  if (form.vertexCount > 0) {
		frmpoly(gsl::span(formLines.data(), form.vertexCount - 1));
		if (form.fillType == CONTF) { // if the form is a contour fill, draw the fill guide
		  auto       line           = std::array<POINT, 2> {};
		  auto const itFirstVertex  = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
		  auto const itStartVertex  = wrap::next(itFirstVertex, form.fillGuide.start);
		  auto const itFinishVertex = wrap::next(itFirstVertex, form.fillGuide.finish);
		  thred::sCor2px(*itStartVertex, line[0]);
		  thred::sCor2px(*itFinishVertex, line[1]);
		  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
		}
	  }
	}
	else { // if the form is a polygon, draw it
	  if (form.vertexCount > lastPoint) {
		frmpoly(gsl::span(std::addressof(formLines.operator[](lastPoint)), form.vertexCount - lastPoint));
	  }
	}
	if (ClosestFormToCursor == iForm &&
	    Instance->stateMap.test(StateFlag::FRMPSEL)) { // if the form is selected, draw the selection box
	  drawFormBox(form);
	}
	else { // if the form is not selected, draw the form
	  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
		frmsqr(form.vertexIndex, iVertex);
	  }
	  SelectObject(StitchWindowMemDC, FormSelectedPen);
	  frmsqr0(formLines[0]);
	}
	if (Instance->stateMap.test(StateFlag::FPSEL) &&
	    ClosestFormToCursor == iForm) { // if form points are selected, draw the selection box
	  dupsel(StitchWindowMemDC);
	  SelectedPixelsRect = sRct2px(SelectedVerticesRect);
	  rct2sel(SelectedPixelsRect, Instance->selectedPointsLine);
	  Instance->stateMap.set(StateFlag::SHOPSEL);
	  dupsel(StitchWindowMemDC);
	}
  }
  if (!Instance->selectedFormList.empty()) { // if there are selected forms, draw the selection box
	drawFormsBox();
  }
  else { // if there are no selected forms, erase the selection box
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  ritfrct(ClosestFormToCursor, StitchWindowMemDC);
	}
	if (Instance->stateMap.test(StateFlag::FRMPMOV)) { // if the user is moving a form point
	  thred::ritmov(ClosestFormToCursor);
	  Instance->rubberBandLine.operator[](1) =
	      POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
	  Instance->stateMap.set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	}
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void form::setmfrm(uint32_t const formIndex) {
  auto const& closeForm = Instance->formList.operator[](formIndex);

  auto       itVertex = wrap::next(Instance->formVertices.cbegin(), closeForm.vertexIndex);
  auto       point    = sfCor2px(itVertex[0]);
  auto const offset =
      POINT {WinMsg.pt.x - StitchWindowOrigin.x - point.x + std::lround(FormMoveDelta.x),
             WinMsg.pt.y - StitchWindowOrigin.y - point.y + std::lround(FormMoveDelta.y)};
  auto& formLines = Instance->formLines;
  formLines.resize(wrap::toSize(closeForm.vertexCount) + 1U);
  for (auto iForm = 0U; iForm < closeForm.vertexCount; ++iForm) {
	point            = sfCor2px(*itVertex);
	formLines[iForm] = POINT {point.x + offset.x, point.y + offset.y};
	++itVertex;
  }
  formLines[closeForm.vertexCount] = formLines[0];
}

void form::durpoli(uint32_t vertexCount) {
  vertexCount          = std::max(vertexCount, 3U);
  auto const stepAngle = PI_F2 / wrap::toFloat(vertexCount);
  // 500 gives us a reasonably sized default
  auto const length = 500.0F / wrap::toFloat(vertexCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  auto newForm        = FRM_HEAD {};
  newForm.vertexIndex = thred::adflt(vertexCount);
  newForm.vertexCount = vertexCount;
  newForm.attribute   = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  newForm.type        = FRMFPOLY;
  auto point          = thred::pxCor2stch(WinMsg.pt);
  auto angle          = 0.0F;
  auto itVertex       = wrap::next(Instance->formVertices.begin(), newForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	*itVertex = point;
	point += F_POINT {length * cos(angle), length * sin(angle)};
	angle += stepAngle;
	++itVertex;
  }
  newForm.outline();
  auto& formList = Instance->formList;

  formList.push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  Instance->stateMap.set(StateFlag::POLIMOV);
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::SHOFRM);
  mdufrm();
}

auto form::nxt(FRM_HEAD const& form, uint32_t iVertex) noexcept -> uint32_t {
  ++iVertex;
  if (iVertex > form.vertexCount - 1U) {
	iVertex = 0;
  }
  return iVertex;
}

auto form::prv(FRM_HEAD const& form, uint32_t iVertex) noexcept -> uint32_t {
  if (iVertex != 0U) {
	--iVertex;
  }
  else {
	iVertex = form.vertexCount - 1U;
  }
  return iVertex;
}

auto form::pdir(FRM_HEAD const& form, uint32_t const vertex) -> uint32_t {
  if (Instance->stateMap.test(StateFlag::PSELDIR)) {
	return nxt(form, vertex);
  }

  return prv(form, vertex);
}

void form::pxrct2stch(RECT const& screenRect, F_RECTANGLE& stitchRect) noexcept {
  auto corner = POINT {screenRect.left + StitchWindowOrigin.x, screenRect.top + StitchWindowOrigin.y};
  auto stitchPoint = thred::pxCor2stch(corner);
  stitchRect.left  = stitchPoint.x;
  stitchRect.top   = stitchPoint.y;
  corner = POINT {screenRect.right + StitchWindowOrigin.x, screenRect.bottom + StitchWindowOrigin.y};
  stitchPoint       = thred::pxCor2stch(corner);
  stitchRect.right  = stitchPoint.x;
  stitchRect.bottom = stitchPoint.y;
}

void form::flipv() {
  auto& formList = Instance->formList;

  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	auto const& form = formList.operator[](ClosestFormToCursor);

	auto const offset        = SelectedVerticesRect.top + SelectedVerticesRect.bottom;
	auto       currentVertex = SelectedFormVertices.start;
	auto const itFirstVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const itVertex = wrap::next(itFirstVertex, currentVertex);
	  itVertex->y         = offset - itVertex->y;
	  currentVertex       = pdir(form, currentVertex);
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	auto const offset = AllItemsRect.top + AllItemsRect.bottom;
	for (auto& formVertice : Instance->formVertices) {
	  formVertice.y = offset - formVertice.y;
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  stitch.y = offset - stitch.y;
	}
	for (auto& iForm : formList) {
	  iForm.rectangle.bottom = offset - iForm.rectangle.bottom;
	  iForm.rectangle.top    = offset - iForm.rectangle.top;
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	auto formMap = boost::dynamic_bitset {formList.size()}; // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	auto rectangle = F_RECTANGLE {};
	pxrct2stch(SelectedFormsRect, rectangle);
	auto const offset = rectangle.top + rectangle.bottom;
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& iForm = formList.operator[](selectedForm);
	  formMap.set(selectedForm);
	  auto itVertex = wrap::next(Instance->formVertices.begin(), iForm.vertexIndex);
	  for (auto iVertex = 0U; iVertex < iForm.vertexCount; ++iVertex) {
		itVertex->y = offset - itVertex->y;
		++itVertex;
	  }
	  iForm.outline();
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  if (auto const decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	      formMap.test(decodedForm) && (stitch.attribute & NOTFRM) == 0U) {
		stitch.y = offset - stitch.y;
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto& form = formList.operator[](ClosestFormToCursor);

	auto const offset   = form.rectangle.top + form.rectangle.bottom;
	auto       itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  itVertex->y = offset - itVertex->y;
	  ++itVertex;
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor && (stitch.attribute & NOTFRM) == 0U) {
		stitch.y = offset - stitch.y;
	  }
	}
	form.outline();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	auto rectangle = F_RECTANGLE {};
	thred::selRct(rectangle);
	auto const offset = rectangle.top + rectangle.bottom;
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  Instance->stitchBuffer.operator[](iStitch).y =
	      offset - Instance->stitchBuffer.operator[](iStitch).y;
	}
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::duform(int32_t const formType) {
  switch (formType + 1) {
	case FRMLINE:
	  thred::savdo();
	  setlin();
	  break;
	case FRMFPOLY:
	  thred::savdo();
	  setpoli();
	  break;
	case FRMRPOLY:
	  setrpoli();
	  break;
	case FRMSTAR:
	  setstar();
	  break;
	case FRMSPIRAL:
	  setspir();
	  break;
	case FRMHEART - 1:
	  sethart();
	  break;
	case FRMLENS - 1:
	  setlens();
	  break;
	case FRMEGG - 1:
	  seteg();
	  break;
	case FRMTEAR - 1:
	  formForms::setear();
	  break;
	case FRMZIGZAG - 1:
	  setzig();
	  break;
	case FRMWAVE - 1:
	  formForms::wavfrm();
	  break;
	case FRMDAISY - 1:
	  formForms::dasyfrm();
	  break;
	default:
	  outDebugString(L"default hit in duform: formType [{}]\n", formType);
	  break;
  }
}

auto form::closfrm(uint32_t& formIndex) -> bool {
  auto const& formList = Instance->formList;

  if (formList.empty()) {
	return false;
  }
  auto const screenCoordinate =
      POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
  thred::rats();
  auto       closestForm   = 0U;
  auto       closestVertex = 0U;
  auto       minimumLength = BIGFLOAT;
  auto const point         = px2stchf(screenCoordinate);
  auto const layerCoded    = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  auto const maxForm       = wrap::toUnsigned(formList.size());
  for (auto iForm = 0U; iForm < maxForm; ++iForm) {
	if (Instance->stateMap.test(StateFlag::FRMSAM) && iForm == formIndex) {
	  continue;
	}
	auto const& currentForm = formList.operator[](iForm);
	if (auto const formLayer = gsl::narrow_cast<uint8_t>(currentForm.attribute & FRMLMSK);
	    ActiveLayer != 0U && formLayer != 0U && formLayer != layerCoded) {
	  continue;
	}
	auto const itFirstVertex = wrap::next(Instance->formVertices.cbegin(), currentForm.vertexIndex);
	auto       itVertex      = itFirstVertex; // intentional copy
	// find the closest line first and then find the closest vertex on that line
	auto       length    = 0.0F;
	auto const sideCount = currentForm.vertexCount;
	for (auto iVertex = 0U; iVertex < sideCount; ++iVertex) {
	  auto const itNextVertex = wrap::next(itFirstVertex, nxt(currentForm, iVertex));
	  auto const param        = findDistanceToSide(*itVertex, *itNextVertex, point, length);
	  if (length >= minimumLength || length < 0.0F) {
		++itVertex;
		continue;
	  }
	  minimumLength = length;
	  closestForm   = iForm;
	  if (param < HALF) {
		closestVertex = iVertex;
	  }
	  else {
		closestVertex = nxt(currentForm, iVertex);
	  }
	  ++itVertex;
	}
  }

  auto const& vertex = Instance->formVertices.operator[](
      wrap::toSize(formList.operator[](closestForm).vertexIndex) + closestVertex);
  auto const stitchCoordsInPixels = thred::stch2pxr(vertex);

  minimumLength = std::hypot(wrap::toFloat(stitchCoordsInPixels.x - screenCoordinate.x),
                             wrap::toFloat(stitchCoordsInPixels.y - screenCoordinate.y));
  if (minimumLength < FCLOSNUF) {
	formIndex             = closestForm;
	ClosestVertexToCursor = closestVertex;
	Instance->stateMap.set(StateFlag::RELAYR);
	return true;
  }
  return false;
}

void form::frmovlin() {
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);
  thred::ratsr();
  if (form.type == FRMLINE) {
	NewFormVertexCount = form.vertexCount;
  }
  else {
	NewFormVertexCount = form.vertexCount + 1U;
  }
  frmToLine(form);
  auto        previousPoint = prv(form, ClosestVertexToCursor);
  auto const& formLines     = Instance->formLines;
  for (auto iPoint = 0U; iPoint < 3U; ++iPoint) {
	Instance->rubberBandLine.operator[](iPoint) = formLines[previousPoint];

	previousPoint = nxt(form, previousPoint);
  }
  thred::ritmov(ClosestFormToCursor);
}

auto form::closflt(FRM_HEAD const& form,
                   float const     xCoordinate,
                   float const yCoordinate) noexcept(!std::is_same_v<ptrdiff_t, int>) -> uint32_t {
  auto closestVertex = 0U;
  auto minimumLength = BIGFLOAT;
  auto itVertex      = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const deltaX = xCoordinate - itVertex->x;
	auto const deltaY = yCoordinate - itVertex->y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
	  closestVertex = iVertex;
	  minimumLength = length;
	}
	++itVertex;
  }
  return closestVertex;
}

void form::chkseq(bool border) {
#if BUGBAK
  UNREFERENCED_PARAMETER(border);

  for (auto val : Instance->oSequence) {
	Instance->interleaveSequence.push_back(val);
  }
#else
  auto& interleaveSequence = Instance->interleaveSequence;

  auto const savedIndex = interleaveSequence.size();
  auto&      form       = Instance->formList.operator[](ClosestFormToCursor);

  // ReSharper disable once CppInitializedValueIsAlwaysRewritten
  auto userStitchLen = 0.0F;
  if (border) {
	userStitchLen = form.edgeType == EDGELCHAIN || form.edgeType == EDGEOCHAIN ? MAXSIZ * PFGRAN
	                                                                           : form.edgeStitchLen;
  }
  else {
	userStitchLen = form.isClip() ? form.maxFillStitchLen : form.stitchLength;
  }
  auto const minimumStitchLength = border ? form.minBorderStitchLen : form.minFillStitchLen;
  if (border) {
	if (form.maxBorderStitchLen == 0.0F) {
	  form.maxBorderStitchLen = IniFile.maxStitchLength;
	}
  }
  else {
	if (form.maxFillStitchLen == 0.0F) {
	  form.maxFillStitchLen = IniFile.maxStitchLength;
	}
  }
  MaxStitchLen  = border ? form.maxBorderStitchLen : form.maxFillStitchLen;
  userStitchLen = std::min(userStitchLen, MaxStitchLen);
  if (!Instance->oSequence.empty()) {
	bool flag = true;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(Instance->oSequence.size()) - 1U; ++iSequence) {
	  if (!ritlin(Instance->oSequence.operator[](iSequence),
	              Instance->oSequence.operator[](wrap::toSize(iSequence) + 1U),
	              userStitchLen)) {
		flag = false;
		break;
	  }
	}
	if (flag) {
	  interleaveSequence.push_back(Instance->oSequence.back());
	}
  }
  else {
	outDebugString(L"chkseq: oSequence empty! border [{}]\n", border);
  }
  if (minimumStitchLength == 0.0F) {
	return;
  }
  auto       destination = wrap::toUnsigned(savedIndex + 1U);
  auto const lengthCheck = minimumStitchLength * minimumStitchLength;
  for (auto iSequence = savedIndex + 1U; iSequence < interleaveSequence.size(); ++iSequence) {
	auto const& seq      = interleaveSequence.operator[](iSequence);
	auto const& seqBack1 = interleaveSequence.operator[](iSequence - 1U);
	auto const  deltaX   = seq.x - seqBack1.x;
	auto const  deltaY   = seq.y - seqBack1.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length > lengthCheck) {
	  interleaveSequence.operator[](destination) = seq;
	  ++destination;
	}
  }
  interleaveSequence.resize(destination);
#endif
}

auto form::lastch() noexcept -> bool {
  if (Instance->interleaveSequence.empty()) {
	return false;
  }
  LastPoint = Instance->interleaveSequence.back();
  return true;
}

auto form::getlast(FRM_HEAD const& form) noexcept(!std::is_same_v<ptrdiff_t, int>) -> uint32_t {
  if (form.fillType == 0U) {
	return 0;
  }
  lastch();
  auto minimumLength = BIGFLOAT;
  auto closestVertex = 0U;
  auto itVertex      = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const deltaX = LastPoint.x - itVertex->x;
	auto const deltaY = LastPoint.y - itVertex->y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
	  minimumLength = length;
	  closestVertex = iVertex;
	}
	++itVertex;
  }
  return closestVertex;
}

void form::filinsb(F_POINT const& point, F_POINT& stitchPoint) {
  constexpr auto MAXSTCH = 54.0F; // maximum permitted stitch length for pfaf in pfaf "stitch pixels"
  auto const delta  = F_POINT {(point.x - stitchPoint.x), (point.y - stitchPoint.y)};
  auto const length = std::hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>((length / MAXSTCH) + 1.0F);
  auto const step   = F_POINT {(delta.x / wrap::toFloat(count)), (delta.y / wrap::toFloat(count))};
  if (length > MAXSTCH) {
	--count;
	if (chkmax(count, wrap::toUnsigned(Instance->oSequence.size()))) {
	  return;
	}
	while (count != 0U) {
	  stitchPoint += step;
	  Instance->oSequence.push_back(stitchPoint);
	  --count;
	}
  }
  Instance->oSequence.push_back(point);
  stitchPoint = point;
}

auto form::getAllItemsRect() noexcept -> F_RECTANGLE& {
  return AllItemsRect;
}

auto form::getblen() noexcept -> float {
  // clang-format off
  auto const& form    = Instance->formList.operator[](ClosestFormToCursor);
  auto const  iLength = (form.clipEntries << WRDSHFT) | form.picoLength;
  // clang-format on
  return wrap::toFloat(iLength);
}

void form::savblen(float const fLength) {
  auto const iLength = wrap::round<uint32_t>(fLength);
  auto&      form    = Instance->formList.operator[](ClosestFormToCursor);
  form.clipEntries   = iLength >> WRDSHFT;
  form.picoLength    = iLength & WRDMASK;
}

auto form::getplen() noexcept -> float {
  // clang-format off
  auto const& form  = Instance->formList.operator[](ClosestFormToCursor);
  auto const  value = form.picoLength;
  // clang-format on
  return wrap::toFloat(value >> BYTSHFT) + (wrap::toFloat(value & BYTMASK) / FRACFACT);
}

void form::savplen(float length) {
  auto integerPart = 0.0F;

  constexpr auto PCLAMP     = 255.0F;
  length                    = std::min(length, PCLAMP);
  auto const fractionalPart = std::modf(length, &integerPart);
  auto const frByte         = wrap::floor<uint16_t>(fractionalPart * FRACFACT);
  auto const num            = gsl::narrow<uint32_t>(integerPart);
  Instance->formList.operator[](ClosestFormToCursor).picoLength = ((num << BYTSHFT) & B2MASK) | frByte;
}

void form::duangs(FRM_HEAD const& form) {
  auto& formAngles = Instance->formAngles;
  formAngles.clear();
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  if (form.type == FRMLINE && (form.edgeType & NEGUND) == EDGEPROPSAT) {
	itVertex = wrap::next(Instance->angledFormVertices.cbegin(), form.vertexIndex);
  }
  auto const vMax = gsl::narrow<ptrdiff_t>(form.vertexCount - 1U);
  for (auto iVertex = ptrdiff_t {}; iVertex < vMax; ++iVertex) {
	auto const& thisVertex = itVertex[iVertex];
	auto const& nextVertex = itVertex[iVertex + 1];
	formAngles.push_back(std::atan2(nextVertex.y - thisVertex.y, nextVertex.x - thisVertex.x));
  }
  formAngles.push_back(std::atan2(itVertex[0].y - itVertex[vMax].y, itVertex[0].x - itVertex[vMax].x));
}

auto form::cisin(FRM_HEAD const& form,
                 float const     xCoordinate,
                 float const     yCoordinate) noexcept(!std::is_same_v<size_t, uint32_t>) -> bool {
  if (auto const& rectangle = form.rectangle;
      xCoordinate < rectangle.left || xCoordinate > rectangle.right ||
      yCoordinate < rectangle.bottom || yCoordinate > rectangle.top) {
	return false;
  }
  auto count        = 0U;
  auto intersection = F_POINT {};
  auto itVertex     = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  if (form.type == FRMLINE && (form.edgeType & NEGUND) == EDGEPROPSAT) {
	itVertex = wrap::next(Instance->angledFormVertices.cbegin(), form.vertexIndex);
  }
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const itThisVertex = wrap::next(itVertex, iVertex);
	auto const itNextVertex = wrap::next(itVertex, nxt(form, iVertex));
	if (!projv(xCoordinate, *itThisVertex, *itNextVertex, intersection)) {
	  continue;
	}
	if (intersection.y < yCoordinate) {
	  continue;
	}
	if (!util::closeEnough(itThisVertex->x, xCoordinate) && !util::closeEnough(itNextVertex->x, xCoordinate)) {
	  ++count;
	  continue;
	}
	if (itThisVertex->x < itNextVertex->x) {
	  if (!util::closeEnough(itNextVertex->x, xCoordinate)) {
		++count;
	  }
	  continue;
	}
	if (!util::closeEnough(itThisVertex->x, xCoordinate)) {
	  ++count;
	}
  }
  return (count & 1U) != 0U;
}

/* find the intersection of two lines, one defined by point and slope, the other by the coordinates
   of the endpoints. */
auto form::linx(std::vector<F_POINT> const& points,
                uint32_t const              start,
                uint32_t const              finish,
                F_POINT&                    intersection,
                std::vector<F_POINT> const& outsidePoints) noexcept -> bool {
  auto const delta =
      F_POINT {(outsidePoints[start].x - points[start].x), (outsidePoints[start].y - points[start].y)};
  auto const& point = points[start];

  if (delta.x == 0.0F && delta.y == 0.0F) {
	return false;
  }
  if (delta.x != 0.0F) {
	return proj(point, delta.y / delta.x, outsidePoints[finish], points[finish], intersection);
  }
  return projv(point.x, points[finish], outsidePoints[finish], intersection);
}

auto form::psg() noexcept -> uint32_t {
  if (PseudoRandomValue == 0U) {
	PseudoRandomValue = SEED;
  }
  auto const temp = PseudoRandomValue & 0x48000000U;
  PseudoRandomValue <<= 1U;
  if (constexpr auto BITS = std::array {0x8000000U, 0x40000000U}; temp == BITS[1] || temp == BITS[0]) {
	++PseudoRandomValue;
  }
  return PseudoRandomValue;
}

void form::angclpfn(FRM_HEAD const&               form,
                    std::vector<RNG_COUNT> const& textureSegments,
                    std::vector<F_POINT>&         angledFormVertices) {
  auto angledForm = form; // intentional copy
  // NOLINTBEGIN(readability-avoid-nested-conditional-operator)
  auto const rotationAngle =
      Instance->stateMap.test(StateFlag::ISUND)   ? PI_FHALF - angledForm.underlayStitchAngle
      : Instance->stateMap.test(StateFlag::TXFIL) ? PI_FHALF - angledForm.fillAngle
                                                  : PI_FHALF - angledForm.clipFillAngle;
  // NOLINTEND(readability-avoid-nested-conditional-operator)
  auto const rotationCenter = F_POINT {wrap::midl(angledForm.rectangle.right, angledForm.rectangle.left),
                                       wrap::midl(angledForm.rectangle.top, angledForm.rectangle.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  if (Instance->stateMap.test(StateFlag::ISUND)) {
	auto const& vertexList = xt::insid(angledForm);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	  angledFormVertices.push_back(vertexList[iVertex]);
	  thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	}
  }
  else {
	auto itVertex = wrap::next(Instance->formVertices.cbegin(), angledForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	  angledFormVertices.push_back(*itVertex);
	  thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	  ++itVertex;
	}
  }
  angledForm.vertexIndex = 0;
  angout(angledForm);
  clpcon(angledForm, textureSegments, angledFormVertices);
  rotbak(-rotationAngle, rotationCenter);
}

void form::filinu(F_POINT const& inPoint, F_POINT const& stitchPoint) {
  auto       point  = stitchPoint; // intentional copy
  auto const delta  = F_POINT {(inPoint.x - stitchPoint.x), (inPoint.y - stitchPoint.y)};
  auto const length = std::hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>(length / UserStitchLength);
  if (count == 0U) {
	Instance->oSequence.push_back(inPoint);
	return;
  }

  if (Instance->stateMap.test(StateFlag::FILDIR)) {
	++count;
  }
  auto const fCount = wrap::toFloat(count);
  auto const step   = F_POINT {delta.x / fCount, delta.y / fCount};
  while (count > 0) {
	point += step;
	Instance->oSequence.push_back(point);
	--count;
  }
}

void form::filin(F_POINT const& currentPoint, F_POINT const& stitchPoint) {
  auto const delta  = F_POINT {(currentPoint.x - stitchPoint.x), (currentPoint.y - stitchPoint.y)};
  auto       point  = stitchPoint; // intentional copy
  auto const length = std::hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>(length / UserStitchLength);
  if (count == 0U) {
	Instance->oSequence.push_back(currentPoint);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FILDIR)) {
	++count;
  }
  auto const fCount = wrap::toFloat(count);
  auto const step   = F_POINT {delta.x / fCount, delta.y / fCount};
  while (count > 0) {
	point += step;
	Instance->oSequence.push_back(point);
	--count;
  }
}

void form::refilfn(uint32_t const formIndex) {
  auto const savedStitchLength = UserStitchLength;
  auto       angledForm        = FRM_HEAD {};
  Instance->stateMap.reset(StateFlag::TXFIL);
  auto& form = Instance->formList.operator[](formIndex);
  if (form.type == FRMLINE) {
	form.underlayIndent = 0;
  }
  if (!(Instance->stateMap.test(StateFlag::FUNCLP) || Instance->stateMap.test(StateFlag::FUNSCLP))) {
	thred::savdo();
  }
  auto fillStartsData = FillStartsDataType {}; // fill start data for refill
  auto fillStartsMap  = 0U;                    // fill starts bitmap
  xt::fdelstch(formIndex, fillStartsData, fillStartsMap);
  Instance->stateMap.set(StateFlag::WASREFIL);
  constexpr auto MINSPACE = 0.5F;
  if (form.fillSpacing < MINSPACE && !form.isClip()) {
	form.fillSpacing = MINSPACE;
  }
  form.edgeSpacing = std::max(form.edgeSpacing, MINSPACE);
  if (!form.isClip()) {
	UserStitchLength = form.stitchLength;
  }
  if (!(Instance->stateMap.test(StateFlag::WASDO) || Instance->stateMap.test(StateFlag::FUNCLP) ||
        Instance->stateMap.test(StateFlag::FUNSCLP))) {
	thred::savdo();
  }
  Instance->stateMap.reset(StateFlag::WASDO);
  if ((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U && form.type == FRMLINE && form.fillType != CONTF) {
	form.type = FRMFPOLY;
  }
  Instance->interleaveSequence.clear();
  Instance->interleaveSequenceIndices.clear();
  Instance->stateMap.reset(StateFlag::ISUND);
  auto textureSegments = std::vector<RNG_COUNT> {};
  textureSegments.resize(wrap::toSize(form.texture.lines));
  switch (form.type) {
	case FRMLINE: {
	  swEdgeType(form, angledForm);
	  if (form.fillType == CONTF && (form.attribute & FRECONT) != 0) {
		contf(form);
		ritfil(form);
	  }
	  break;
	}
	case FRMFPOLY: {
	  xt::chkcwlk(formIndex);
	  xt::chkwlk(formIndex);
	  xt::chkund(formIndex, textureSegments, Instance->angledFormVertices);
	  Instance->stateMap.reset(StateFlag::ISUND);
	  if (form.fillType == 0U) {
		chkbrd(form);
		break;
	  }
	  auto const spacing = LineSpacing;
	  LineSpacing        = form.fillSpacing;
	  swPolyFillType(form, angledForm, textureSegments);
	  ritfil(form);
	  LineSpacing = spacing;
	  chkbrd(form);
	  break;
	}
	case SAT: {
	  xt::chkcwlk(formIndex);
	  xt::chkwlk(formIndex);
	  xt::chkund(formIndex, textureSegments, Instance->angledFormVertices);
	  Instance->stateMap.reset(StateFlag::ISUND);
	  swSatFillType(form);
	  chkbrd(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in refilfn 4: type [{}]\n", form.type);
	  break;
	}
  }
  UserStitchLength = savedStitchLength;
  xt::intlv(formIndex, fillStartsData, fillStartsMap);
  thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
  xt::setfchk();
}

void form::refil(uint32_t const formIndex) {
  if (Instance->userFlagMap.test(UserFlag::WRNOF)) {
	refilfn(formIndex);
	return;
  }
  if (auto const codedForm = formIndex << FRMSHFT | USMSK;
      !std::ranges::any_of(Instance->stitchBuffer, [&codedForm](F_POINT_ATTR const& stitch) -> bool {
	    return (stitch.attribute & NOTFRM) == 0U && (stitch.attribute & (USMSK | FRMSK)) == codedForm;
      })) {
	refilfn(formIndex);
	return;
  }
  if (formForms::inFormDataSheet()) {
	Instance->stateMap.set(StateFlag::WASFRMFRM);
  }
  formForms::destroyFormDataSheet();
  if (nullptr == MsgWindow) {
	displayText::tabmsg(IDS_REFIL, true);
	displayText::okcan();
  }
  Instance->stateMap.set(StateFlag::MOVMSG);
}

void form::setfpnt() {
  // clang-format off
  auto&      form             = Instance->formList.operator[](ClosestFormToCursor);
  auto const screenCoordinate = POINT {(WinMsg.pt.x - StitchWindowOrigin.x), 
									   (WinMsg.pt.y - StitchWindowOrigin.y)};
  auto const itVertex         = wrap::next(Instance->formVertices.begin(), form.vertexIndex + ClosestVertexToCursor);
  // clang-format on
  unfrm();
  thred::rats();
  *itVertex = px2stchf(screenCoordinate);
  form.outline();
  refil(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::WASFPNT);
  Instance->stateMap.reset(StateFlag::SELBOX);
  Instance->stateMap.set(StateFlag::FRMPSEL);
  thred::ritfcor(*itVertex);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::filvrt() {
  if (!displayText::filmsgs(FMM_VRT)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  if (auto const& currentForm = Instance->formList.operator[](ClosestFormToCursor);
	      currentForm.type != FRMLINE) {
		fsvrt();
	  }
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	fsvrt();
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::filhor() {
  if (!displayText::filmsgs(FMM_HOR)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  if (auto& form = Instance->formList.operator[](ClosestFormToCursor); form.type != FRMLINE) {
		fshor(form);
	  }
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto& form = Instance->formList.operator[](ClosestFormToCursor);
	fshor(form);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::filangl() {
  auto& formList = Instance->formList;
  if (!displayText::filmsgs(FMM_ANG)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  if (auto& form = formList.operator[](selectedForm); form.type != FRMLINE) {
		fsangl(form);
	  }
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	auto& form = formList.operator[](ClosestFormToCursor);
	fsangl(form);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

auto form::chkfrm(std::vector<POINT>& formControlPoints, std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool {
  auto const point = POINT {(WinMsg.pt.x - StitchWindowOrigin.x), (WinMsg.pt.y - StitchWindowOrigin.y)};
  auto const& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  NewFormVertexCount      = currentForm.vertexCount + 1U;
  thred::duzrat();
  auto const rectangle    = sRct2px(currentForm.rectangle);
  auto&      formControls = formControlPoints;
  rct2sel(rectangle, formControls);

  auto minimumLength      = BIGLONG;
  auto constexpr LENCHECK = ICLOSNUF * ICLOSNUF;
  auto formControlIndex   = 0U;
  for (auto const& iControl : formControls) {
	auto const deltaX = iControl.x - point.x;
	auto const deltaY = iControl.y - point.y;
	if (auto const length = (deltaX * deltaX) + (deltaY * deltaY); length < minimumLength) {
	  minimumLength             = length;
	  SelectedFormControlVertex = formControlIndex;
	}
	if (minimumLength < LENCHECK) {
	  ritfrct(ClosestFormToCursor, StitchWindowDC);
	  for (auto iCorner = 0U; iCorner < wrap::toUnsigned(stretchBoxLine.size()); ++iCorner) {
		stretchBoxLine[iCorner] = formControls[wrap::toSize(iCorner) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  thred::strtchbox(stretchBoxLine);
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		Instance->stateMap.set(StateFlag::STRTCH);
	  }
	  else {
		Instance->stateMap.set(StateFlag::EXPAND);
		xyRatio = (currentForm.rectangle.right - currentForm.rectangle.left) /
		          (currentForm.rectangle.top - currentForm.rectangle.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  Instance->stateMap.set(StateFlag::SHOSTRTCH);
	  return true;
	}
	++formControlIndex;
  }
  SelectedFormControlVertex >>= 1U;
  if (point.x < rectangle.left || point.x > rectangle.right || point.y < rectangle.top ||
      point.y > rectangle.bottom) {
	return false;
  }
  auto const itVertex   = wrap::next(Instance->formVertices.cbegin(), currentForm.vertexIndex);
  auto const formOrigin = sfCor2px(*itVertex);
  FormMoveDelta         = F_POINT {formOrigin.x - point.x, formOrigin.y - point.y};
  Instance->stateMap.set(StateFlag::FRMOV);
  return true;
}

void form::rstfrm() {
  auto const point =
      POINT {(WinMsg.pt.x + std::lround(FormMoveDelta.x)), (WinMsg.pt.y + std::lround(FormMoveDelta.y))};

  auto const attribute = ClosestFormToCursor << 4U;
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.reset(StateFlag::FRMOV);
  auto const stitchPoint = thred::pxCor2stch(point);
  auto&      currentForm = Instance->formList.operator[](ClosestFormToCursor);
  auto       itVertex    = wrap::next(Instance->formVertices.begin(), currentForm.vertexIndex);
  auto const offset      = F_POINT {stitchPoint.x - itVertex->x, stitchPoint.y - itVertex->y};
  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; ++iVertex) {
	*itVertex += offset;
	++itVertex;
  }
  currentForm.rectangle.left += offset.x;
  currentForm.rectangle.top += offset.y;
  currentForm.rectangle.right += offset.x;
  currentForm.rectangle.bottom += offset.y;
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & FRMSK) == attribute && (stitch.attribute & ALTYPMSK) != 0U &&
	    (stitch.attribute & NOTFRM) == 0U) {
	  stitch += offset;
	}
  }
}

void form::clrfills() noexcept {
  for (auto& iForm : Instance->formList) {
	iForm.clipEntries = 0;
	iForm.clipCount   = 0;
	iForm.edgeType    = 0;
	iForm.fillType    = 0;
	iForm.attribute &= NFRECONT;
	iForm.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
  }
  Instance->clipPoints.clear();
}

void form::drwcon() {
  uncon();
  auto& formLines = Instance->formLines;
  formLines[1]    = POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
  Instance->stateMap.set(StateFlag::SHOCON);
  ducon();
}

void form::duinsf() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::uninsf() {
  if (Instance->stateMap.testAndReset(StateFlag::SHOINSF)) {
	duinsf();
  }
}

void form::rotfrm(FRM_HEAD& form, uint32_t const newStartVertex) {
  auto rotatedVertices = std::vector<F_POINT> {};
  rotatedVertices.resize(form.vertexCount);
  auto iRotated = newStartVertex;
  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  std::copy(itVertex, wrap::next(itVertex, form.vertexCount), rotatedVertices.begin());
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex = rotatedVertices[iRotated];
	iRotated  = nxt(form, iRotated);
	++itVertex;
  }
  if (form.type == SAT) {
	if (form.vertexCount != 0U) {
	  if (form.wordParam != 0U) {
		form.wordParam = (form.wordParam + form.vertexCount - newStartVertex) % form.vertexCount;
	  }
	  if (form.satinGuideCount != 0U) {
		auto const itStartGuide = wrap::next(Instance->satinGuides.begin(), form.satinGuideIndex);
		auto       rotatedIt    = itStartGuide; // intended copy
		auto       itGuide      = itStartGuide; // intended copy
		for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		  if (itGuide->start != newStartVertex && itGuide->finish != newStartVertex) {
			rotatedIt->start = (itGuide->start + form.vertexCount - newStartVertex) % form.vertexCount;
			rotatedIt->finish = (itGuide->finish + form.vertexCount - newStartVertex) % form.vertexCount;
			if (rotatedIt->start > rotatedIt->finish) {
			  std::swap(rotatedIt->start, rotatedIt->finish);
			}
			++rotatedIt;
		  }
		  ++itGuide;
		}
		if (rotatedIt != itStartGuide) {
		  form.satinGuideCount = wrap::distance<uint32_t>(itStartGuide, rotatedIt);
		  std::sort(itStartGuide, rotatedIt, satin::scomp);
		}
	  }
	}
  }
  if (form.vertexCount == 0U) {
	return;
  }
  if ((form.extendedAttribute & AT_STRT) != 0U) {
	form.fillStart = (form.fillStart + form.vertexCount - newStartVertex) % form.vertexCount;
  }
  if ((form.extendedAttribute & AT_END) != 0U) {
	form.fillEnd = (form.fillEnd + form.vertexCount - newStartVertex) % form.vertexCount;
  }
}

void form::filsat() {
  if (!displayText::filmsgs(FMM_FAN)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	for (auto const selectedForm : Instance->selectedFormList) {
	  if (auto const& currentForm = Instance->formList.operator[](selectedForm); currentForm.type != FRMLINE) {
		filsfn(selectedForm);
	  }
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	filsfn(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::insat() { // insert a point in a form
  auto inOutFlag = IntersectionStyles::POINT_IN_LINE;
  if (!closat(inOutFlag)) {
	return;
  }
  thred::savdo();
  auto& selectedForm = Instance->formList.operator[](ClosestFormToCursor);

  auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
  switch (inOutFlag) {
	case IntersectionStyles::POINT_BEFORE_LINE: {
	  nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	  auto const itVertex = wrap::next(Instance->formVertices.begin(), selectedForm.vertexIndex);
	  std::swap(itVertex[0], itVertex[1]);
	  break;
	}
	case IntersectionStyles::POINT_IN_LINE: {
	  ClosestVertexToCursor = prv(selectedForm, ClosestVertexToCursor);
	  nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	  break;
	}
	case IntersectionStyles::POINT_AFTER_LINE: {
	  nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	  break;
	}
	// ReSharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  throw std::runtime_error("Invalid Intersection Flag in insat");
	}
  }
  refil(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto form::chkdel(FRM_HEAD const& currentForm) noexcept -> bool {
  if (currentForm.type == FRMLINE) {
	return currentForm.vertexCount <= 2;
  }

  return currentForm.vertexCount <= 3;
}

void form::unfil() {
  if (!displayText::filmsgs(FMX_UNF)) {
	return;
  }
  auto& formList = Instance->formList;

  if (!Instance->selectedFormList.empty()) {
	auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	for (auto const selectedForm : Instance->selectedFormList) {
	  if (auto& form = formList.operator[](selectedForm); form.fillType != 0U || form.edgeType != 0U) {
		clip::delclps(selectedForm);
		texture::deltx(selectedForm);
		formMap.set(selectedForm);
		form.fillType = 0;
		form.edgeType = 0;
		form.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	  }
	}
	auto iDestination = Instance->stitchBuffer.begin();
	for (auto const& stitch : Instance->stitchBuffer) {
	  if (!formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
		*iDestination++ = stitch;
	  }
	}
	Instance->stitchBuffer.resize(wrap::distance<size_t>(Instance->stitchBuffer.begin(), iDestination));
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	if (!Instance->stateMap.testAndReset(StateFlag::IGNOR) && !Instance->userFlagMap.test(UserFlag::WRNOF)) {
	  if (auto const codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
	      std::ranges::any_of(Instance->stitchBuffer, [&codedForm](F_POINT_ATTR const& stitch) -> bool {
		    return (stitch.attribute & NOTFRM) == 0U && (stitch.attribute & (USMSK | FRMSK)) == codedForm;
	      })) {
		displayText::tabmsg(IDS_UNFIL, true);
		Instance->stateMap.set(StateFlag::FILMSG);
		displayText::okcan();
		Instance->stateMap.set(StateFlag::IGNOR);
		return;
	  }
	}
	if (!Instance->stitchBuffer.empty()) {
	  auto       iDestination = Instance->stitchBuffer.begin();
	  auto       destCount    = 0U;
	  auto const codedForm    = ClosestFormToCursor << FRMSHFT;
	  for (auto const& stitch : Instance->stitchBuffer) {
		if ((stitch.attribute & FRMSK) != codedForm || (stitch.attribute & NOTFRM) != 0U) {
		  *iDestination++ = stitch;
		  ++destCount;
		}
	  }
	  Instance->stitchBuffer.resize(destCount);
	}
	auto& form = formList.operator[](ClosestFormToCursor);
	clip::delclps(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	form.fillType = 0;
	form.edgeType = 0;
	form.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::frm0() {
  if (!Instance->stateMap.test(StateFlag::FRMPSEL)) {
	return;
  }
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  thred::savdo();
  rotfrm(form, ClosestVertexToCursor);
  ClosestVertexToCursor = 0;
  satin::satadj(form);
  refil(ClosestFormToCursor);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::rinfrm() {
  frmToLine(*FormForInsert);
  SelectObject(StitchWindowMemDC, FormPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  auto const& formLines = Instance->formLines;
  if (FormVertexNext != 0U || FormForInsert->type != FRMLINE) {
	wrap::polyline(StitchWindowMemDC, &formLines[FormVertexPrev], LNPNTS);
  }
  InsertLine[0] = formLines[FormVertexPrev];
  InsertLine[1] = POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
  Instance->stateMap.set(StateFlag::SHOINSF);
  duinsf();
}

void form::infrm() { // insert multiple points into a form
  auto inOutFlag = IntersectionStyles::POINT_IN_LINE;
  if (!closat(inOutFlag)) {
	return;
  }
  FormForInsert = std::addressof(Instance->formList.operator[](ClosestFormToCursor));
  switch (inOutFlag) {
	case IntersectionStyles::POINT_BEFORE_LINE: {
	  FormVertexPrev = 0;
	  Instance->stateMap.set(StateFlag::PRELIN);
	  break;
	}
	case IntersectionStyles::POINT_IN_LINE: {
	  FormVertexPrev = prv(*FormForInsert, ClosestVertexToCursor);
	  FormVertexNext = ClosestVertexToCursor;
	  break;
	}
	case IntersectionStyles::POINT_AFTER_LINE: {
	  FormVertexPrev = ClosestVertexToCursor;
	  break;
	}
	// ReSharper disable once CppClangTidyClangDiagnosticCoveredSwitchDefault
	default: { // NOLINT(clang-diagnostic-covered-switch-default)
	  throw std::runtime_error("Invalid Intersection Flag in infrm");
	}
  }
  Instance->stateMap.set(StateFlag::INSFRM);
  Instance->stateMap.set(StateFlag::INIT);
  rinfrm();
}

void form::setins() {
  auto const stitchPoint = thred::pxCor2stch(WinMsg.pt);
  nufpnt(FormVertexPrev, *FormForInsert, stitchPoint);
  if (Instance->stateMap.test(StateFlag::PRELIN)) {
	auto const itVertex = wrap::next(Instance->formVertices.begin(), FormForInsert->vertexIndex);
	std::swap(itVertex[0], itVertex[1]);
  }
  else {
	FormVertexPrev = nxt(*FormForInsert, FormVertexPrev);
	FormVertexNext = nxt(*FormForInsert, FormVertexPrev);
  }
  frmToLine(*FormForInsert);
  InsertLine[0] = Instance->formLines.operator[](FormVertexPrev);
  InsertLine[1] = POINT {WinMsg.pt.x - StitchWindowOrigin.x, WinMsg.pt.y - StitchWindowOrigin.y};
  Instance->stateMap.set(StateFlag::INSFRM);
  duinsf();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::bsizpar(FRM_HEAD& currentForm) noexcept {
  currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
  currentForm.edgeStitchLen      = UserStitchLength;
  currentForm.minBorderStitchLen = MinStitchLength;
}

void form::bord() {
  if (!displayText::filmsgs(FML_LIN)) {
	return;
  }
  auto& formList = Instance->formList;

  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& form       = formList.operator[](selectedForm);
	  form.borderColor = ActiveColor;
	  sbord(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto& form       = formList.operator[](ClosestFormToCursor);
	form.borderColor = ActiveColor;
	sbord(ClosestFormToCursor);
	thred::coltab();
	Instance->stateMap.set(StateFlag::INIT);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::fclp() {
  if (!displayText::filmsgs(FML_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	CloseClipboard();
	return;
  }
  thred::savdo();
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cx <= CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  fsclp(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	fsclp(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::apliq() {
  if (!displayText::filmsgs(FML_APLQ)) {
	return;
  }
  auto& formList = Instance->formList;

  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  if (auto& currentForm = formList.operator[](selectedForm); Instance->userFlagMap.test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  sapliq(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	if (auto& currentForm = formList.operator[](ClosestFormToCursor);
	    Instance->userFlagMap.test(UserFlag::BLUNT)) {
	  currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	}
	else {
	  currentForm.attribute &= NOBLNT;
	}
	sapliq(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::setap() {
  AppliqueColor = ActiveColor;
  displayText::showMessage(IDS_APCOL, AppliqueColor + 1U);
}

void form::stchrct2px(F_RECTANGLE const& stitchRect, RECT& screenRect) {
  auto stitchCoord = F_POINT {stitchRect.left, stitchRect.top};
  auto screenCoord = POINT {};
  thred::sCor2px(stitchCoord, screenCoord);
  screenRect.left = screenCoord.x;
  screenRect.top  = screenCoord.y;
  stitchCoord     = F_POINT {stitchRect.right, stitchRect.bottom};
  thred::sCor2px(stitchCoord, screenCoord);
  screenRect.right  = screenCoord.x;
  screenRect.bottom = screenCoord.y;
}

void form::selal() {
  Instance->stateMap.reset(StateFlag::FORMSEL);
  Instance->selectedFormList.clear();
  Instance->stateMap.reset(StateFlag::SELBOX);
  Instance->stateMap.reset(StateFlag::GRPSEL);
  AllItemsRect = getbig(Instance->formList, Instance->stitchBuffer);
  ZoomRect = F_RECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.cy), wrap::toFloat(UnzoomedRect.cx), 0.0F};
  ZoomFactor = 1;
  Instance->stateMap.reset(StateFlag::ZUMED);
  thred::movStch();
  thred::resetNearest();
  Instance->stateMap.reset(StateFlag::RUNPAT);
  thred::duzrat();
  stchrct2px(AllItemsRect, SelectedFormsRect);
  Instance->stateMap.set(StateFlag::BIGBOX);
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::setstrtch() {
  auto reference   = 0.0F;
  auto ratio       = 1.0F;
  auto stitchRect  = F_RECTANGLE {};
  auto stitchPoint = F_POINT {};
  thred::savdo();
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	stitchRect = SelectedVerticesRect;
  }
  else {
	if (!Instance->selectedFormList.empty() || Instance->stateMap.test(StateFlag::BIGBOX)) {
	  pxrct2stch(SelectedFormsRect, stitchRect);
	  stitchPoint = F_POINT {stitchRect.right, stitchRect.bottom};
	}
	else {
	  stitchPoint = thred::pxCor2stch(WinMsg.pt);
	}
  }
  switch (SelectedFormControlVertex) {
	case 0: { // top control
	  if (!Instance->selectedFormList.empty() || Instance->stateMap.test(StateFlag::BIGBOX) ||
	      Instance->stateMap.test(StateFlag::FPSEL)) {
		reference = stitchRect.bottom;

		auto const offsetY = WinMsg.pt.y - StitchWindowOrigin.y;

		ratio = wrap::toFloat(SelectedFormsRect.bottom - offsetY) /
		        wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
		reference          = form.rectangle.bottom;
		ratio              = (stitchPoint.y - reference) / (form.rectangle.top - reference);
		form.rectangle.top = stitchPoint.y;
		break;
	  }
	  reference = StitchRangeRect.bottom;
	  ratio     = (stitchPoint.y - reference) / (StitchRangeRect.top - reference);
	  break;
	}
	case 1: { // right control
	  if (!Instance->selectedFormList.empty() || Instance->stateMap.test(StateFlag::BIGBOX) ||
	      Instance->stateMap.test(StateFlag::FPSEL)) {
		reference = stitchRect.left;

		auto const offsetX = WinMsg.pt.x - StitchWindowOrigin.x;

		ratio = wrap::toFloat(offsetX - SelectedFormsRect.left) /
		        wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left);
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
		auto& formRectangle = form.rectangle;
		reference           = formRectangle.left;
		ratio               = (stitchPoint.x - reference) / (formRectangle.right - reference);
		formRectangle.right = stitchPoint.x;
		break;
	  }
	  reference = StitchRangeRect.left;
	  ratio     = (stitchPoint.x - reference) / (StitchRangeRect.right - reference);
	  break;
	}
	case 2: { // bottom control
	  if (!Instance->selectedFormList.empty() || Instance->stateMap.test(StateFlag::BIGBOX) ||
	      Instance->stateMap.test(StateFlag::FPSEL)) {
		reference = stitchRect.top;

		auto const offsetY = WinMsg.pt.y - StitchWindowOrigin.y;

		ratio = wrap::toFloat(offsetY - SelectedFormsRect.top) /
		        wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
		reference             = form.rectangle.top;
		ratio                 = (stitchPoint.y - reference) / (form.rectangle.bottom - reference);
		form.rectangle.bottom = stitchPoint.y;
		break;
	  }
	  reference = StitchRangeRect.top;
	  ratio     = (stitchPoint.y - reference) / (StitchRangeRect.bottom - reference);
	  break;
	}
	case 3: { // left control
	  if (!Instance->selectedFormList.empty() || Instance->stateMap.test(StateFlag::BIGBOX) ||
	      Instance->stateMap.test(StateFlag::FPSEL)) {
		reference = stitchRect.right;

		auto const offsetX = WinMsg.pt.x - StitchWindowOrigin.x;

		ratio = wrap::toFloat(SelectedFormsRect.right - offsetX) /
		        wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left);
		break;
	  }
	  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
		reference           = form.rectangle.right;
		ratio               = (stitchPoint.x - reference) / (form.rectangle.left - reference);
		form.rectangle.left = stitchPoint.x;
		break;
	  }
	  reference = StitchRangeRect.right;
	  ratio     = (stitchPoint.x - reference) / (StitchRangeRect.left - reference);
	  break;
	}
	default: {
	  throw std::runtime_error("no valid control selected"); // we should never reach here
	}
  }
  auto& formList = Instance->formList;
  if ((SelectedFormControlVertex & 1U) != 0U) { // right or left control selected
	if (Instance->stateMap.test(StateFlag::FPSEL)) {
	  auto currentVertex = SelectedFormVertices.start;
	  auto itVertex      = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const itCurrentVertex = wrap::next(itVertex, currentVertex);

		itCurrentVertex->x = (itCurrentVertex->x - reference) * ratio + reference;
		currentVertex      = pdir(form, currentVertex);
	  }
	  form.outline();
	  thred::setpsel();
	  refil(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
		auto& formIter = formList.operator[](iForm);
		auto  itVertex = wrap::next(Instance->formVertices.begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->x = (itVertex->x - reference) * ratio + reference;
		  ++itVertex;
		}
		formIter.outline();
	  }
	  for (auto& stitch : Instance->stitchBuffer) {
		stitch.x = (stitch.x - reference) * ratio + reference;
	  }
	  selal();
	  return;
	}
	if (!Instance->selectedFormList.empty()) {
	  for (auto selectedForm : Instance->selectedFormList) {
		auto& formIter = formList.operator[](selectedForm);
		auto  itVertex = wrap::next(Instance->formVertices.begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->x = (itVertex->x - reference) * ratio + reference;
		  ++itVertex;
		}
	  }
	  for (auto selectedForm : Instance->selectedFormList) {
		formList.operator[](selectedForm).outline();
		ClosestFormToCursor = selectedForm;
		refil(ClosestFormToCursor);
	  }
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		itVertex->x = (itVertex->x - reference) * ratio + reference;
		++itVertex;
	  }
	  refil(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		Instance->stitchBuffer.operator[](iStitch).x =
		    (Instance->stitchBuffer.operator[](iStitch).x - reference) * ratio + reference;
	  }
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
  }
  else {
	if (Instance->stateMap.test(StateFlag::FPSEL)) {
	  auto currentVertex = SelectedFormVertices.start;
	  auto itVertex      = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const itCurrentVertex = wrap::next(itVertex, currentVertex);

		itCurrentVertex->y = (itCurrentVertex->y - reference) * ratio + reference;
		currentVertex      = pdir(form, currentVertex);
	  }
	  formList.operator[](ClosestFormToCursor).outline();
	  thred::setpsel();
	  refil(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
		auto& formIter = formList.operator[](iForm);
		auto  itVertex = wrap::next(Instance->formVertices.begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->y = (itVertex->y - reference) * ratio + reference;
		  ++itVertex;
		}
		formIter.outline();
	  }
	  for (auto& stitch : Instance->stitchBuffer) {
		stitch.y = (stitch.y - reference) * ratio + reference;
	  }
	  selal();
	  return;
	}
	if (!Instance->selectedFormList.empty()) {
	  for (auto selectedForm : Instance->selectedFormList) {
		auto& formIter = formList.operator[](selectedForm);
		auto  itVertex = wrap::next(Instance->formVertices.begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->y = (itVertex->y - reference) * ratio + reference;
		  ++itVertex;
		}
	  }
	  for (auto selectedForm : Instance->selectedFormList) {
		formList.operator[](selectedForm).outline();
		ClosestFormToCursor = selectedForm;
		refil(selectedForm);
	  }
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		itVertex->y = (itVertex->y - reference) * ratio + reference;
		++itVertex;
	  }
	  refil(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::RESTCH);
	  return;
	}
	if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		Instance->stitchBuffer.operator[](iStitch).y =
		    (Instance->stitchBuffer.operator[](iStitch).y - reference) * ratio + reference;
	  }
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
  }
}

void form::setexpand(float const xyRatio) {
  // ReSharper disable once CppInitializedValueIsAlwaysRewritten
  auto size0     = F_POINT {};
  auto rectangle = F_RECTANGLE {};
  thred::savdo();
  auto stitchPoint = F_POINT {};

  auto& formList = Instance->formList;
  if (!Instance->selectedFormList.empty() || Instance->stateMap.test(StateFlag::BIGBOX) ||
      Instance->stateMap.test(StateFlag::FPSEL)) {
	// if we have multiple forms selected, or we are in big box mode, or we are in form point select
	// mode we need to use the selected forms rectangle
	wrap::narrow_cast(rectangle.left, SelectedFormsRect.left);
	wrap::narrow_cast(rectangle.top, SelectedFormsRect.top);
	wrap::narrow_cast(rectangle.right, SelectedFormsRect.right);
	wrap::narrow_cast(rectangle.bottom, SelectedFormsRect.bottom);

	wrap::narrow_cast(stitchPoint.x, WinMsg.pt.x - StitchWindowOrigin.x);
	wrap::narrow_cast(stitchPoint.y, WinMsg.pt.y - StitchWindowOrigin.y);
	size0.y = rectangle.bottom - rectangle.top;
  }
  else {
	// if we are in form select mode, we need to use the form rectangle or if not, the stitch range rectangle
	stitchPoint = thred::pxCor2stch(WinMsg.pt);
	rectangle   = Instance->stateMap.test(StateFlag::FORMSEL)
	                  ? formList.operator[](ClosestFormToCursor).rectangle
	                  : StitchRangeRect;
	size0.y     = rectangle.top - rectangle.bottom;
  }
  // ReSharper disable CppInitializedValueIsAlwaysRewritten
  auto ratio     = F_POINT {1.0F, 1.0F};
  auto reference = F_POINT {};
  // ReSharper restore CppInitializedValueIsAlwaysRewritten
  size0.x = rectangle.right - rectangle.left;
  // we need to determine the reference point and the ratio for the expansion
  switch (SelectedFormControlVertex) {
	case 0: { // top control
	  reference  = F_POINT {rectangle.right, rectangle.bottom};
	  auto size1 = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  setSize(size1, xyRatio);
	  ratio = size1 / size0;
	  if (Instance->selectedFormList.empty() && Instance->stateMap.test(StateFlag::FORMSEL)) {
		auto& formRect = formList.operator[](ClosestFormToCursor).rectangle;
		formRect.left  = rectangle.right - size1.x;
		formRect.top   = rectangle.bottom + size1.y;
	  }
	  break;
	}
	case 1: { // right control
	  reference  = F_POINT {rectangle.left, rectangle.bottom};
	  auto size1 = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  setSize(size1, xyRatio);
	  ratio = size1 / size0;
	  if (Instance->selectedFormList.empty() && Instance->stateMap.test(StateFlag::FORMSEL)) {
		auto& formRect = formList.operator[](ClosestFormToCursor).rectangle;
		formRect.right = rectangle.left + size1.x;
		formRect.top   = rectangle.bottom + size1.y;
	  }
	  break;
	}
	case 2: { // bottom control
	  reference  = F_POINT {rectangle.left, rectangle.top};
	  auto size1 = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  setSize(size1, xyRatio);
	  ratio = size1 / size0;
	  if (Instance->selectedFormList.empty() && Instance->stateMap.test(StateFlag::FORMSEL)) {
		auto& formRect  = formList.operator[](ClosestFormToCursor).rectangle;
		formRect.right  = rectangle.left + size1.x;
		formRect.bottom = rectangle.top - size1.y;
	  }
	  break;
	}
	case 3: { // left control
	  reference  = F_POINT {rectangle.right, rectangle.top};
	  auto size1 = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  setSize(size1, xyRatio);
	  ratio = size1 / size0;
	  // if we are in form select mode, we need to update the form rectangle
	  if (Instance->selectedFormList.empty() && Instance->stateMap.test(StateFlag::FORMSEL)) {
		auto& formRect  = formList.operator[](ClosestFormToCursor).rectangle;
		formRect.left   = rectangle.right - size1.x;
		formRect.bottom = rectangle.top - size1.y;
	  }
	  break;
	}
	default: {
	  throw std::runtime_error("no valid control vertex"); // we should never reach here
	}
  }
  auto const integerReference = POINT {std::lround(reference.x), std::lround(reference.y)};
  auto const stitchReference  = px2stchf(integerReference);
  if (Instance->stateMap.test(StateFlag::FPSEL)) { // form point selected
	resizeFormPoints(stitchReference, ratio);
	return;
  }
  if (Instance->stateMap.test(StateFlag::BIGBOX)) { // everything selected
	resizeBigBox(stitchReference, ratio);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	resizeSelectedForms(stitchReference, ratio);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) { // single form selected
	resizeForm(reference, ratio);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) { // stitch group selected
	resizeStitches(reference, ratio);
  }
}

void form::nufilcol(uint8_t const color) noexcept {
  auto& formColor = Instance->formList.operator[](ClosestFormToCursor).fillColor;
  if (formColor == color) {
	return;
  }
  formColor = color;

  auto const attribute = (ClosestFormToCursor << FRMSHFT) | FRMFIL;
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & (FRMSK | TYPMSK | FTHMSK)) == attribute) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= color;
	}
  }
}

void form::nufthcol(uint8_t const color) noexcept {
  auto& formColor = Instance->formList.operator[](ClosestFormToCursor).feather.color;
  if (formColor == color) {
	return;
  }
  formColor            = color;
  auto const attribute = (ClosestFormToCursor << FRMSHFT) | FTHMSK;
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & (FRMSK | FTHMSK)) == attribute) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= color;
	}
  }
}

void form::nubrdcol(uint8_t const color) noexcept {
  Instance->formList.operator[](ClosestFormToCursor).borderColor = color;
  auto const attribute = (ClosestFormToCursor << FRMSHFT) | FRMBFIL;
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & (FRMSK | TYPMSK)) == attribute) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= color;
	}
  }
}

void form::nulapcol(uint8_t color) {
  auto& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  if (gsl::narrow<decltype(color)>(currentForm.borderColor >> FRMSHFT) == color) {
	return;
  }
  currentForm.borderColor &= COLMSK;
  currentForm.borderColor |= gsl::narrow_cast<uint8_t>(color << FRMSHFT);
  auto const attribute = (ClosestFormToCursor << FRMSHFT) | TYPMSK;
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & (TYPMSK | FRMSK)) == attribute) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= color;
	}
  }
}

void form::dubold() {
  if (!displayText::filmsgs(FML_BLD)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  sbold(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	sbold(ClosestFormToCursor);
	thred::coltab();
	Instance->stateMap.set(StateFlag::INIT);
	thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::sethup() noexcept {
  if (util::closeEnough(IniFile.hoopSizeX, LHUPX) && util::closeEnough(IniFile.hoopSizeY, LHUPY)) {
	IniFile.hoopType = LARGHUP;
	return;
  }
  if (util::closeEnough(IniFile.hoopSizeX, SHUPX) && util::closeEnough(IniFile.hoopSizeY, SHUPY)) {
	IniFile.hoopType = SMALHUP;
	return;
  }
  if (util::closeEnough(IniFile.hoopSizeX, HUP100XY) && util::closeEnough(IniFile.hoopSizeY, HUP100XY)) {
	IniFile.hoopType = HUP100;
	return;
  }
  IniFile.hoopType = CUSTHUP;
}

void form::dustar(uint32_t starCount, float const starRatio, float const length) {
  constexpr auto STARMIN = 3U;   // minimum star vertices
  constexpr auto STARMAX = 100U; // maximum star vertices

  starCount = std::clamp(starCount, STARMIN, STARMAX);

  auto const stepAngle   = PI_F / wrap::toFloat(starCount);
  auto       angle       = (stepAngle * HALF) + PI_F;
  auto const vertexCount = starCount * 2U;
  auto       newForm     = FRM_HEAD {};
  newForm.vertexIndex    = thred::adflt(vertexCount);
  newForm.vertexCount    = vertexCount;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  newForm.type = FRMFPOLY;
  auto point   = thred::pxCor2stch(WinMsg.pt);
  Instance->stateMap.set(StateFlag::FILDIR);
  auto const itFirstVertex = wrap::next(Instance->formVertices.begin(), newForm.vertexIndex);
  auto       itVertex      = itFirstVertex;
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	itVertex->x = point.x;
	itVertex->y = point.y;
	point += F_POINT {length * cos(angle), length * sin(angle)};
	angle += stepAngle;
	++itVertex;
  }
  itVertex               = itFirstVertex;
  auto const itEndVertex = wrap::next(itVertex, starCount);
  auto const center      = F_POINT {wrap::midl(itEndVertex->x, itFirstVertex->x),
                               wrap::midl(itEndVertex->y, itFirstVertex->y)};
  ++itVertex;
  for (auto iVertex = 1U; iVertex < starCount; ++iVertex) {
	*itVertex = (*itVertex - center) * starRatio + center;
	itVertex += 2;
  }
  *itVertex = (*itVertex - center) * starRatio + center;
  newForm.outline();
  auto& formList = Instance->formList;

  formList.push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  Instance->stateMap.set(StateFlag::POLIMOV);
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::SHOFRM);
  mdufrm();
}

void form::duspir(uint32_t stepCount) {
  constexpr auto STEPMIN = 3U;   // Spiral step minimum
  constexpr auto STEPMAX = 100U; // spiral step maximum

  stepCount = std::clamp(stepCount, STEPMIN, STEPMAX);

  auto const stepAngle = PI_F2 / wrap::toFloat(stepCount);
  auto const length    = 800.0F / wrap::toFloat(stepCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  auto newForm           = FRM_HEAD {};
  newForm.type           = FRMLINE;
  auto const vertexCount = wrap::round<uint32_t>(wrap::toFloat(stepCount) * thred::getSpiralWrap());
  newForm.vertexIndex    = thred::adflt(vertexCount);
  newForm.vertexCount    = vertexCount;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  auto firstSpiral = std::vector<F_POINT> {};
  firstSpiral.resize(stepCount);
  auto centeredSpiral = std::vector<F_POINT> {};
  centeredSpiral.resize(stepCount);
  auto point = thred::pxCor2stch(WinMsg.pt);
  auto angle = 0.0F;
  for (auto iStep = 0U; iStep < stepCount; ++iStep) {
	firstSpiral[iStep] = point;
	point += F_POINT {length * cos(angle), length * sin(angle)};
	angle += stepAngle;
  }
  auto const center = F_POINT {wrap::midl(firstSpiral[stepCount / 2].x, firstSpiral[0].x),
                               wrap::midl(firstSpiral[stepCount / 2].y, firstSpiral[0].y)};
  for (auto iStep = 0U; iStep < stepCount; ++iStep) {
	centeredSpiral[iStep] = firstSpiral[iStep] - center;
  }
  auto const stepRatio = 1.0F / wrap::toFloat(vertexCount);
  auto       ratio     = stepRatio;
  auto       itVertex  = wrap::next(Instance->formVertices.begin(), newForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	*itVertex = centeredSpiral[iVertex % stepCount] * ratio + center;
	++itVertex;
	ratio += stepRatio;
  }
  newForm.outline();
  auto& formList = Instance->formList;

  formList.push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  Instance->stateMap.set(StateFlag::POLIMOV);
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::SHOFRM);
  mdufrm();
}

void form::duhart(uint32_t sideCount) {
  constexpr auto HARTANG  = 0.28F; // heart angle in radians
  constexpr auto HARTINFL = 0.7F;  // heart inflection point in radians
  constexpr auto HARTSTEP = 4.5F;
  constexpr auto HSIDEMIN = 6U;   // minimum number of heart vertices
  constexpr auto HSIDEMAX = 100U; // maximum number of heart vertices

  sideCount = std::clamp(sideCount, HSIDEMIN, HSIDEMAX);

  auto currentForm        = FRM_HEAD {};
  currentForm.vertexIndex = wrap::toUnsigned(Instance->formVertices.size());
  wrap::narrow(currentForm.attribute, ActiveLayer << 1U);
  Instance->formVertices.reserve(Instance->formVertices.size() + (wrap::toSize(sideCount) * 2U) - 2U);
  auto const savedVertexIndex = wrap::toUnsigned(Instance->formVertices.size());
  auto       point            = thred::pxCor2stch(WinMsg.pt);
  auto       stepAngle        = PI_F2 / wrap::toFloat(sideCount);
  auto const length           = 300.0F / wrap::toFloat(sideCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  auto angle    = PI_F * HARTANG;
  auto iVertex  = ptrdiff_t {};
  auto maximumX = 0.0F;
  while (angle > -PI_F * HARTINFL) {
	maximumX = std::max(point.x, maximumX);
	Instance->formVertices.push_back(point);
	++iVertex;
	point += F_POINT {length * cos(angle), length * sin(angle)};
	angle -= stepAngle;
  }
  stepAngle /= HARTSTEP;
  auto       lastVertex    = iVertex;
  auto const itFirstVertex = wrap::next(Instance->formVertices.begin(), savedVertexIndex);
  while (point.x > itFirstVertex->x && iVertex < wrap::toPtrdiff(2U * HSIDEMAX)) {
	Instance->formVertices.emplace_back(point.x, point.y);
	++iVertex;
	point += F_POINT {length * cos(angle), length * sin(angle)};
	angle -= stepAngle;
  }
  auto const firstVertex = iVertex;

  auto const nVert = std::next(itFirstVertex, lastVertex - 1);
  auto const ratio = (nVert->x - itFirstVertex->x) / (nVert->x - itFirstVertex[firstVertex - 1].x);
  auto       itVertex = std::next(itFirstVertex, lastVertex);
  for (iVertex = lastVertex; iVertex < firstVertex; ++iVertex) {
	itVertex->x = (itVertex->x - nVert->x) * ratio + nVert->x;
	++itVertex;
  }
  auto iDestination = wrap::toUnsigned(iVertex);
  lastVertex        = iVertex;
  iVertex           = lastVertex - 2;
  itVertex          = std::next(itFirstVertex, iVertex);
  for (; iVertex != 0; --iVertex) {
	Instance->formVertices.emplace_back(
	    maximumX + maximumX - itVertex->x - (2 * (maximumX - itFirstVertex->x)), itVertex->y);
	--itVertex;
	++iDestination;
  }
  NewFormVertexCount      = iDestination + 1U;
  currentForm.vertexCount = iDestination;
  currentForm.type        = FRMFPOLY;
  currentForm.outline();
  auto& formList = Instance->formList;

  formList.push_back(currentForm);
  ClosestFormToCursor = wrap::toUnsigned(formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  Instance->stateMap.set(StateFlag::POLIMOV);
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::SHOFRM);
  mdufrm();
}

void form::dulens(uint32_t sides) {
  sides = std::max(sides, 4U);
#ifdef LENS48
  sides = std::min(sides, 48U);
#endif
  auto const steps = sides >> 1U;
  // Calculate the angle subtended by the arc of the lens which is determined by the aspect ratio
  auto const theta = std::atan(1.0F / IniFile.lensRatio);
  // now calculate the radius of the lens arc and scale by the zoom factor
  auto const radius = (100.0F / std::sin(theta)) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  // get the angle subtended by each step
  auto const omega = 2.0F * theta / wrap::toFloat(steps);
  // and the interior angle of every segment
  auto const phi = (PI_F - omega) / 2.0F;
  // as well as the length of every segment
  auto const stepLength   = 2.0F * radius * std::cos(phi);
  auto       currentForm  = FRM_HEAD {};
  currentForm.vertexIndex = wrap::toUnsigned(Instance->formVertices.size());
  currentForm.attribute   = gsl::narrow_cast<decltype(currentForm.attribute)>(ActiveLayer << 1U);
  auto const stitchPoint  = thred::pxCor2stch(WinMsg.pt);
  auto       point        = stitchPoint;
  auto       iVertex      = 0U;
  Instance->formVertices.reserve(Instance->formVertices.size() + wrap::toSize(steps << 1U) + 1U);
  // rho is the angle of this particular segment
  auto rho = PI_F - theta - phi;
  for (auto iStep = 0U; iStep < steps; ++iStep) {
	Instance->formVertices.push_back(point);
	point += F_POINT {stepLength * cos(rho), stepLength * sin(rho)};
	rho += omega;
	++iVertex;
  }
  Instance->formVertices.push_back(point);
  auto itVertex = wrap::next(Instance->formVertices.cbegin(), currentForm.vertexIndex + iVertex - 1U);
  auto const xCoord = stitchPoint.x;
  for (; iVertex != 1; --iVertex) {
	Instance->formVertices.emplace_back(xCoord + xCoord - itVertex->x, itVertex->y);
	--itVertex;
  }
  currentForm.vertexCount = wrap::toUnsigned(Instance->formVertices.size() - currentForm.vertexIndex);
  NewFormVertexCount = currentForm.vertexCount + 1U;
  currentForm.type   = FRMFPOLY;
  currentForm.outline();
  Instance->formList.push_back(currentForm);
  ClosestFormToCursor = wrap::toUnsigned(Instance->formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  Instance->stateMap.set(StateFlag::POLIMOV);
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::SHOFRM);
  mdufrm();
}

void form::dueg(uint32_t sides) {
  constexpr auto ESIDEMIN = 8U; // minimum number of egg vertices

  sides = std::max(sides, ESIDEMIN);
  durpoli(sides);
  auto const  halfSides    = gsl::narrow<ptrdiff_t>(sides >> 1U);
  auto const  quarterSides = gsl::narrow<ptrdiff_t>(sides >> 2U);
  auto const& form         = Instance->formList.back();
  auto        itVertex     = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  auto const  reference    = wrap::midl(itVertex[halfSides].y, itVertex[0].y);
  auto const  maximumY     = itVertex[quarterSides].y - itVertex[0].y;
  for (uint32_t iVertex = 0; iVertex < sides; ++iVertex) {
	if (itVertex->y < reference) {
	  itVertex->y = reference - (reference - itVertex->y) * IniFile.eggRatio;
	}
	++itVertex;
  }
  itVertex             = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  auto const  eggRatio = maximumY / (itVertex[quarterSides].y - itVertex[0].y);
  auto const& ref      = *itVertex;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex = F_POINT {shreg(itVertex->x, ref.x, eggRatio), shreg(itVertex->y, ref.y, eggRatio)};
	++itVertex;
  }
}

void form::duzig(uint32_t vertices) {
  constexpr auto ZVERTMIN = 3U;   // minimum number of zig-zag vertices
  constexpr auto ZVERTMAX = 100U; // maximum number of zig-zag vertices
  vertices                = std::clamp(vertices, ZVERTMIN, ZVERTMAX);
  auto newForm            = FRM_HEAD {};
  newForm.vertexIndex     = thred::adflt(vertices);
  newForm.vertexCount     = vertices;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  auto       stitchPoint = thred::pxCor2stch(WinMsg.pt);
  auto const offset      = F_POINT {UnzoomedRect.cx / 6.0, UnzoomedRect.cy / (6.0 * vertices)};
  auto       itVertex    = wrap::next(Instance->formVertices.begin(), newForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertices; ++iVertex) {
	*itVertex = stitchPoint;
	++itVertex;
	stitchPoint.y -= offset.y;
	if ((iVertex & 1U) != 0U) {
	  stitchPoint.x += offset.x;
	}
	else {
	  stitchPoint.x -= offset.x;
	}
  }
  newForm.type = FRMLINE;
  newForm.outline();
  Instance->formList.push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(Instance->formList.size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertices + 1U;
  Instance->stateMap.set(StateFlag::POLIMOV);
  setmfrm(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::SHOFRM);
  mdufrm();
}

void form::fliph() {
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	thred::savdo();
	auto const offset        = SelectedVerticesRect.right + SelectedVerticesRect.left;
	auto       currentVertex = SelectedFormVertices.start;
	auto const itFirstVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const itVertex = wrap::next(itFirstVertex, currentVertex);
	  itVertex->x         = offset - itVertex->x;
	  currentVertex       = pdir(form, currentVertex);
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  auto& formList = Instance->formList;

  if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	thred::savdo();
	auto const offset = AllItemsRect.right + AllItemsRect.left;
	for (auto& formVertice : Instance->formVertices) {
	  formVertice.x = offset - formVertice.x;
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  stitch.x = offset - stitch.x;
	}
	for (auto& iForm : formList) {
	  auto& rect = iForm.rectangle;
	  rect.left  = offset - rect.left;
	  rect.right = offset - rect.right;
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	auto rectangle = F_RECTANGLE {};
	pxrct2stch(SelectedFormsRect, rectangle);
	auto const offset = rectangle.right + rectangle.left;
	for (auto const selectedForm : Instance->selectedFormList) {
	  formMap.set(selectedForm);
	  // clang-format off
	  auto& formIter = formList.operator[](selectedForm);
	  auto  itVertex = wrap::next(Instance->formVertices.begin(), formIter.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		itVertex->x = offset - itVertex->x;
		++itVertex;
	  }
	  formIter.outline();
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  if (auto const decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	      formMap.test(decodedForm) && (stitch.attribute & NOTFRM) == 0U) {
		stitch.x = offset - stitch.x;
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	auto const& srcRect  = form.rectangle;
	auto const  offset   = srcRect.right + srcRect.left;
	auto        itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  itVertex->x = offset - itVertex->x;
	  ++itVertex;
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor && (stitch.attribute & NOTFRM) == 0U) {
		stitch.x = offset - stitch.x;
	  }
	}
	form.outline();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	auto rectangle = F_RECTANGLE {};
	thred::selRct(rectangle);
	auto const offset = rectangle.right + rectangle.left;
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  Instance->stitchBuffer.operator[](iStitch).x =
	      offset - Instance->stitchBuffer.operator[](iStitch).x;
	}
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::prpbrd(float const borderStitchSpacing) {
  if (!displayText::filmsgs(FML_PRPS)) {
	return;
  }
  auto const savedSpacing = LineSpacing;
  LineSpacing             = borderStitchSpacing;
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& currentForm      = Instance->formList.operator[](selectedForm);
	  currentForm.borderSize = LineSpacing;
	  if (Instance->userFlagMap.test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  prpsbrd(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	LineSpacing = savedSpacing;
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	if (auto& currentForm = Instance->formList.operator[](ClosestFormToCursor);
	    Instance->userFlagMap.test(UserFlag::BLUNT)) {
	  currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	}
	else {
	  currentForm.attribute &= NOBLNT;
	}
	prpsbrd(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
	LineSpacing = savedSpacing;
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::tglfrm() {
  if (Instance->stateMap.testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  Instance->stateMap.reset(StateFlag::HIDSTCH);
  menu::flipFormOnOff();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::frmon() {
  thred::unbsho();
  Instance->stateMap.reset(StateFlag::FRMOF);
  menu::resetFormOnOff();
}

void form::flpord() {
  auto const& form = Instance->formList.operator[](ClosestFormToCursor);
  if (Instance->stateMap.test(StateFlag::FPSEL)) {
	thred::savdo();
	auto start = SelectedFormVertices.start;
	auto finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % form.vertexCount;
	auto const itFirstVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount / 2; ++iVertex) {
	  auto itStartVertex = wrap::next(itFirstVertex, start);
	  auto itEndVertex   = wrap::next(itFirstVertex, finish);
	  std::swap(*itStartVertex, *itEndVertex);
	  start = pdir(form, start);
	  Instance->stateMap.flip(StateFlag::PSELDIR);
	  finish = pdir(form, finish);
	  Instance->stateMap.flip(StateFlag::PSELDIR);
	}
	refil(ClosestFormToCursor);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  fnord();
	}
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	fnord();
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	auto const start = wrap::next(Instance->stitchBuffer.begin(), GroupStartStitch);
	auto const end   = wrap::next(Instance->stitchBuffer.begin(), GroupEndStitch);
	std::reverse(start, end);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::clpfil() {
  if (!displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	CloseClipboard();
	return;
  }
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cx <= CLPMIN || ClipRectSize.cy <= CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  if (Instance->formList.operator[](ClosestFormToCursor).type != FRMLINE) {
		filsclp();
	  }
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	filsclp();
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::snap() {
  thred::savdo();
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	snp(GroupStartStitch, GroupEndStitch);
  }
  else {
	snp(0, wrap::toUnsigned(Instance->stitchBuffer.size()));
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto form::rotpar() -> F_POINT {
  while (true) {
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  RotationRect = Instance->formList.operator[](ClosestFormToCursor).rectangle;
	  Instance->stateMap.set(StateFlag::FRMROT);
	  break;
	}
	if (!Instance->selectedFormList.empty()) {
	  pxrct2stch(SelectedFormsRect, RotationRect);
	  Instance->stateMap.set(StateFlag::FRMSROT);
	  break;
	}
	if (Instance->stateMap.test(StateFlag::BIGBOX)) {
	  RotationRect = AllItemsRect;
	  break;
	}
	if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  thred::selRct(RotationRect);
	  break;
	}
	if (Instance->stateMap.test(StateFlag::FPSEL)) {
	  RotationRect = SelectedVerticesRect;
	  break;
	}
	break;
  }
  if (Instance->stateMap.test(StateFlag::GMRK)) {
	return thred::getZoomMarkPoint();
  }
  return F_POINT {wrap::midl(RotationRect.right, RotationRect.left),
                  wrap::midl(RotationRect.top, RotationRect.bottom)};
}

void form::setrang() {
  rotentr(IniFile.rotationAngle);
  Instance->stateMap.set(StateFlag::NUROT);
}

void form::rotcmd() {
  if (!Instance->stateMap.test(StateFlag::FORMSEL) && !Instance->stateMap.test(StateFlag::GRPSEL) &&
      Instance->selectedFormList.empty() && !Instance->stateMap.test(StateFlag::BIGBOX) &&
      !Instance->stateMap.test(StateFlag::FPSEL)) {
	displayText::shoseln(IDS_ALLX, IDS_ROTCMD);
	return;
  }
  rotentr(IniFile.rotationAngle);
  Instance->stateMap.set(StateFlag::ENTROT);
}

void form::rotagain() {
  fnagain(IniFile.rotationAngle);
}

void form::bakagain() {
  fnagain(PI_F2 - IniFile.rotationAngle);
}

void form::rotdup() {
  if (!Instance->stateMap.test(StateFlag::FORMSEL) && !Instance->stateMap.test(StateFlag::GRPSEL) &&
      Instance->selectedFormList.empty()) {
	displayText::shord();
	return;
  }
  rotentr(IniFile.rotationAngle);
  Instance->stateMap.set(StateFlag::ENTRDUP);
}

void form::cpylayr(uint32_t const layer) {
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	for (auto const selectedForm : Instance->selectedFormList) {
	  cplayfn(selectedForm, layer);
	}
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	cplayfn(ClosestFormToCursor, layer);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	auto const oldSize = Instance->stitchBuffer.size();
	Instance->stitchBuffer.reserve(oldSize + (wrap::toSize(GroupEndStitch) + 1U - GroupStartStitch));
	Instance->stitchBuffer.insert(Instance->stitchBuffer.end(),
	                              wrap::next(Instance->stitchBuffer.cbegin(), GroupStartStitch),
	                              wrap::next(Instance->stitchBuffer.cbegin(), GroupEndStitch + 1U));
	auto const codedStitchLayer = layer << LAYSHFT;
	for (auto currentStitch = wrap::next(Instance->stitchBuffer.begin(), oldSize);
	     currentStitch != Instance->stitchBuffer.end();
	     ++currentStitch) {
	  currentStitch->attribute &= NLAYMSK;
	  currentStitch->attribute |= codedStitchLayer;
	}
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::movlayr(uint32_t layer) {
  auto const codedStitchLayer = layer << LAYSHFT;

  auto& formList = Instance->formList;
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	auto formMap = boost::dynamic_bitset(formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& formAttr = formList.operator[](selectedForm).attribute;
	  formAttr = gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formAttr & NFRMLMSK) | layer << FLAYSHFT);
	  formMap.set(selectedForm);
	}
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) != 0U) {
		if (auto const iCurrentForm = (stitch.attribute & FRMSK) >> FRMSHFT; formMap.test(iCurrentForm)) {
		  stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
		}
	  }
	}
	Instance->selectedFormList.clear();
	Instance->stateMap.reset(StateFlag::FORMSEL);
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	auto& formAttr = formList.operator[](ClosestFormToCursor).attribute;
	wrap::narrow(formAttr, layer << FLAYSHFT | gsl::narrow_cast<decltype(layer)>(formAttr & NFRMLMSK));
	Instance->stateMap.reset(StateFlag::FORMSEL);
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) != 0U && (stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; ++iStitch) {
	  Instance->stitchBuffer.operator[](iStitch).attribute =
	      (Instance->stitchBuffer.operator[](iStitch).attribute & NLAYMSK) | codedStitchLayer;
	}
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::join() {
  auto const savedFormIndex = ClosestFormToCursor;

  auto& formList = Instance->formList;

  auto const& savedform = formList.operator[](ClosestFormToCursor);
  auto const  lastVertex =
      wrap::next(Instance->formVertices.cbegin(), savedform.vertexIndex + savedform.vertexCount - 1U);
  Instance->stateMap.set(StateFlag::FRMSAM);
  if (formList.size() <= 1 || !Instance->stateMap.test(StateFlag::FORMSEL) || !closfrm(ClosestFormToCursor)) {
	Instance->stateMap.reset(StateFlag::FRMSAM);
	return;
  }
  auto const& form = formList.operator[](ClosestFormToCursor);

  auto vertexList = std::vector<F_POINT> {};
  vertexList.reserve(form.vertexCount);
  auto const itVertex = wrap::next(Instance->formVertices.cbegin(), form.vertexIndex);
  if (abs(lastVertex->x - itVertex->x) > TNYFLOAT || abs(lastVertex->y - itVertex->y) > TNYFLOAT) {
	auto const itThisVertex = wrap::next(itVertex, ClosestVertexToCursor);
	vertexList.push_back(*itThisVertex);
  }
  ClosestVertexToCursor = nxt(form, ClosestVertexToCursor);
  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	auto itThisVertex = wrap::next(itVertex, ClosestVertexToCursor);
	vertexList.push_back(*itThisVertex);
	ClosestVertexToCursor = nxt(form, ClosestVertexToCursor);
  }
  Instance->stateMap.set(StateFlag::DELTO);
  thred::frmdel();
  if (savedFormIndex > ClosestFormToCursor) {
	ClosestFormToCursor = savedFormIndex - 1U;
  }
  else {
	ClosestFormToCursor = savedFormIndex;
  }
  auto& toForm = formList.operator[](ClosestFormToCursor);

  auto const insertionPoint = toForm.vertexIndex + toForm.vertexCount;
  fltspac(toForm.vertexCount, wrap::toUnsigned(vertexList.size()));
  auto const dest = wrap::next(Instance->formVertices.begin(), insertionPoint);
  std::ranges::copy(vertexList, dest);
  toForm.vertexCount += wrap::toUnsigned(vertexList.size());
  toForm.outline();
  refil(ClosestFormToCursor);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
  Instance->stateMap.reset(StateFlag::FRMSAM);
}

void form::refilal() {
  auto const savedFormIndex = ClosestFormToCursor;
  thred::savdo();
  for (ClosestFormToCursor = 0; ClosestFormToCursor < wrap::toUnsigned(Instance->formList.size());
       ++ClosestFormToCursor) {
	refilfn(ClosestFormToCursor);
  }
  ClosestFormToCursor = savedFormIndex;
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::nufsel() {
  if (Instance->formList.empty()) {
	return;
  }
  Instance->stateMap.reset(StateFlag::FORMSEL);
  if (Instance->stateMap.testAndReset(StateFlag::WASEL)) {
	thred::pushPreviousForm();
  }
  if (std::ranges::none_of(Instance->selectedFormList,
                           [](uint32_t const intX) { return intX == ClosestFormToCursor; })) {
	Instance->selectedFormList.push_back(ClosestFormToCursor);
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::frmadj(uint32_t const formIndex) noexcept(!std::is_same_v<ptrdiff_t, int>) {
  auto& form = Instance->formList.operator[](formIndex);

  auto itVertex = wrap::next(Instance->formVertices.begin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex += FormMoveDelta;
	++itVertex;
  }
  form.outline();
}

void form::frmsadj() {
  auto formMap = boost::dynamic_bitset(Instance->formList.size()); // NOLINT(clang-diagnostic-ctad-maybe-unsupported)
  for (auto const selectedForm : Instance->selectedFormList) {
	formMap.set(selectedForm);
  }
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & ALTYPMSK) != 0U && formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
	  stitch += FormMoveDelta;
	}
  }
}

void form::selfil(uint32_t const type) {
  displayText::frm1pnt();
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::tabmsg(IDS_SEL1FRM, false);
	return;
  }
  auto const& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  if (type == FRMFIL && currentForm.fillType == 0U) {
	displayText::tabmsg(IDS_FSELF, false);
	return;
  }
  if (type == FRMBFIL && currentForm.edgeType == 0U) {
	displayText::tabmsg(IDS_FSELB, false);
	return;
  }
  if (type == FRMAPFIL && (currentForm.edgeType & NEGUND) != EDGEAPPL) {
	displayText::tabmsg(IDS_FSELA, false);
	return;
  }
  frmpnts(type);
  Instance->stateMap.set(StateFlag::GRPSEL);
  Instance->stateMap.reset(StateFlag::FORMSEL);
  thred::rngadj();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::selalfil() {
  displayText::frm1pnt();
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::tabmsg(IDS_SEL1FRM, false);
	return;
  }
  auto const savedIndex = ClosestPointIndex;
  ClosestPointIndex     = thred::findFirstStitch(ClosestFormToCursor);
  if (ClosestPointIndex == Instance->stitchBuffer.size()) {
	ClosestPointIndex = savedIndex;
	displayText::tabmsg(IDS_FSELM, false);
	return;
  }
  if (ClosestPointIndex != 0U) {
	--ClosestPointIndex;
  }
  GroupStitchIndex = thred::findLastStitch(ClosestFormToCursor);
  Instance->stateMap.set(StateFlag::GRPSEL);
  Instance->stateMap.reset(StateFlag::FORMSEL);
  thred::rngadj();
  Instance->stateMap.set(StateFlag::RESTCH);
}

auto form::frmrng(uint32_t const iForm, RANGE& range) -> bool {
  if (Instance->stitchBuffer.empty()) {
	return false;
  }
  range.start      = 0;
  auto const& form = Instance->formList.operator[](iForm);

  ClosestFormToCursor = iForm;
  if (form.fillType != 0U || form.edgeType != 0U) {
	range.start  = thred::findFirstStitch(iForm);
	range.finish = thred::findLastStitch(iForm);
	return range.finish > range.start;
  }
  range.finish = wrap::toUnsigned(Instance->stitchBuffer.size());
  return false;
}

void form::bhol() {
  auto const savedSpacing = LineSpacing;
  if (!displayText::filmsgs(FML_BHOL)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  bholbrd(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	LineSpacing = savedSpacing;
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	bholbrd(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
	Instance->stateMap.set(StateFlag::RESTCH);
	LineSpacing = savedSpacing;
  }
}

void form::fcntr() {
  if (Instance->selectedFormList.empty()) {
	displayText::tabmsg(IDS_SELC, false);
	return;
  }
  thred::savdo();
  auto const firstForm = Instance->selectedFormList.front();
  auto&      formList  = Instance->formList;

  auto const& firstRect = formList.operator[](firstForm).rectangle;

  auto const initialCenter =
      F_POINT {wrap::midl(firstRect.right, firstRect.left), wrap::midl(firstRect.top, firstRect.bottom)};
  for (auto const selectedForm : Instance->selectedFormList) {
	auto& currentForm = formList.operator[](selectedForm);

	auto const& formRect = currentForm.rectangle;
	auto const  currentCenter =
	    F_POINT {wrap::midl(formRect.right, formRect.left), wrap::midl(formRect.top, formRect.bottom)};
	auto const delta = F_POINT {initialCenter.x - currentCenter.x, initialCenter.y - currentCenter.y};
	auto itVertex = wrap::next(Instance->formVertices.begin(), currentForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < currentForm.vertexCount; ++iVertex) {
	  *itVertex += delta;
	  ++itVertex;
	}
	currentForm.outline();
	auto const codedForm = selectedForm << FRMSHFT;
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & FRMSK) == codedForm && (stitch.attribute & NOTFRM) == 0U) {
		stitch += delta;
	  }
	}
  }
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::boxsel() {
  Instance->stateMap.reset(StateFlag::INSRT);
  if (!Instance->stateMap.test(StateFlag::THUMSHO)) {
	Instance->stateMap.reset(StateFlag::BZUM);
	Instance->stateMap.reset(StateFlag::BZUMIN);
	Instance->stateMap.set(StateFlag::VCAPT);
	SetCapture(ThrEdWindow);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::picot() {
  if (!displayText::filmsgs(FML_PIC)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == 0) {
	return;
  }
  thred::savdo();
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	CloseClipboard();
	return;
  }
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cx <= CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  fspic(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	fspic(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::contfil() {
  if (!displayText::filmsgs(FML_CONT)) {
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	for (auto const selectedForm : Instance->selectedFormList) {
	  contsf(selectedForm);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  thred::savdo();
  if (contsf(ClosestFormToCursor)) {
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  displayText::tabmsg(IDS_CONT, false);
}

void form::dupfn(float const rotationAngle) {
  thred::savdo();
  auto usedAngle = rotationAngle;
  if (Instance->stateMap.test(StateFlag::ROTCLOCK) && rotationAngle != 0.0F) {
	usedAngle = PI_F2 - rotationAngle;
  }
  auto const rotationCenter = rotpar();
  if (usedAngle == 0.0F) {
	rotentr(usedAngle);
	Instance->stateMap.set(StateFlag::ENTRDUP);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	duprot(usedAngle);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	duprots(usedAngle, rotationCenter);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	duprotfs(usedAngle);
	return;
  }
  displayText::shord();
}

void form::shrnk() {
  if (!Instance->stateMap.test(StateFlag::FORMSEL) ||
      Instance->formList.operator[](ClosestFormToCursor).edgeType != EDGECLIP) {
	displayText::shoseln(IDS_FRMCLP, IDS_SHRNK);
	return;
  }
  thred::savdo();
  shrnks();
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::frmnumfn(uint32_t& oldFormIndex, uint32_t const newFormIndex) {
  thred::savdo();
  if (newFormIndex == oldFormIndex) {
	return;
  }
  auto  formRelocationIndex = 0U;
  auto  sourceForm          = 0U;
  auto  tempFormList        = std::vector<FRM_HEAD> {};
  auto& formList            = Instance->formList;

  tempFormList.resize(formList.size());
  auto tempFormVertices = std::vector<F_POINT> {};
  tempFormVertices.resize(Instance->formVertices.size());
  auto tempGuides = std::vector<SAT_CON> {};
  tempGuides.reserve(Instance->satinGuides.size());
  auto tempClipPoints = std::vector<F_POINT> {};
  tempClipPoints.reserve(Instance->clipPoints.size());
  auto formSourceIndex = 0U;
  for (auto iForm = 0U; iForm < wrap::toUnsigned(formList.size()); ++iForm) {
	if (iForm == newFormIndex) {
	  dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, oldFormIndex, formRelocationIndex, formSourceIndex);
	  continue;
	}
	if (sourceForm == oldFormIndex) {
	  ++sourceForm;
	}
	dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, sourceForm++, formRelocationIndex, formSourceIndex);
  }
  std::ranges::copy(tempFormList, formList.begin());
  std::ranges::copy(tempFormVertices, Instance->formVertices.begin());
  std::ranges::copy(tempGuides, Instance->satinGuides.begin());
  std::ranges::copy(tempClipPoints, Instance->clipPoints.begin());
  for (auto& stitch : Instance->stitchBuffer) {
	if ((stitch.attribute & SRTYPMSK) == 0U) {
	  continue;
	}
	auto const decodedFormIndex = (stitch.attribute & FRMSK) >> FRMSHFT;
	if (decodedFormIndex == oldFormIndex) {
	  stchfrm(newFormIndex, stitch.attribute);
	  continue;
	}
	auto start  = newFormIndex;
	auto finish = oldFormIndex;
	if (oldFormIndex < newFormIndex) {
	  std::swap(start, finish);
	}
	if (decodedFormIndex < start || decodedFormIndex > finish) {
	  continue;
	}
	if (newFormIndex < oldFormIndex) {
	  stchfrm(decodedFormIndex + 1, stitch.attribute);
	  continue;
	}
	stchfrm(decodedFormIndex - 1, stitch.attribute);
  }
  oldFormIndex = newFormIndex;
  displayText::ritnum(IDS_NUMFORM, oldFormIndex);
}

void form::srtbyfrm() {
  auto colorHistogram = std::vector<uint32_t> {};
  colorHistogram.resize(COLORCNT);
  auto color = std::vector<uint32_t> {};
  color.resize(COLORCNT);
  if (!Instance->formList.empty()) {
	thred::savdo();
	boost::range::iota(color, 1);
	color[AppliqueColor]  = 0U;
	auto tempStitchBuffer = std::vector<F_POINT_ATTR> {};
	tempStitchBuffer.resize(Instance->stitchBuffer.size());
	for (auto const& stitch : Instance->stitchBuffer) {
	  ++colorHistogram[color[stitch.attribute & COLMSK]];
	}
	auto colorAccumulator = 0U;
	for (auto& iColor : colorHistogram) {
	  auto const value = iColor;
	  iColor           = colorAccumulator;
	  colorAccumulator += value;
	}
	for (auto const& stitch : Instance->stitchBuffer) {
	  tempStitchBuffer[colorHistogram[color[stitch.attribute & COLMSK]]++] = stitch;
	}
	srtf(tempStitchBuffer, 0, colorHistogram[0]);
	for (auto iColor = 0U; iColor < colorHistogram.size() - 1U; ++iColor) {
	  srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[wrap::toSize(iColor) + 1U]);
	}
  }
  else {
	xt::srtcol();
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::cntrx() {
  auto const markCenter =
      Instance->stateMap.test(StateFlag::GMRK)
          ? thred::getZoomMarkPoint()
          : F_POINT {wrap::toFloat(UnzoomedRect.cx) / 2.0F, wrap::toFloat(UnzoomedRect.cy) / 2.0F};
  if (!Instance->selectedFormList.empty()) {
	thred::savdo();
	auto selectedCenter = F_POINT {};
	dufcntr(selectedCenter);
	FormMoveDelta = F_POINT {markCenter.x - selectedCenter.x, markCenter.y - selectedCenter.y};
	if (Instance->stateMap.test(StateFlag::CNTRV)) {
	  FormMoveDelta.y = 0.0F;
	}
	if (Instance->stateMap.test(StateFlag::CNTRH)) {
	  FormMoveDelta.x = 0.0F;
	}
	for (auto const selectedForm : Instance->selectedFormList) {
	  frmadj(selectedForm);
	}
	frmsadj();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	thred::savdo();
	// clang-format off
	  auto const& formRect       = Instance->formList.operator[](ClosestFormToCursor).rectangle;
	  auto const  selectedCenter = F_POINT {wrap::midl(formRect.right, formRect.left), 
										    wrap::midl(formRect.top, formRect.bottom)};
	// clang-format on
	FormMoveDelta = F_POINT {markCenter.x - selectedCenter.x, markCenter.y - selectedCenter.y};
	if (Instance->stateMap.test(StateFlag::CNTRV)) {
	  FormMoveDelta.y = 0.0F;
	}
	if (Instance->stateMap.test(StateFlag::CNTRH)) {
	  FormMoveDelta.x = 0.0F;
	}
	frmadj(ClosestFormToCursor);
	for (auto& stitch : Instance->stitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) != 0U && (stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		stitch += FormMoveDelta;
	  }
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	auto const& startStitch = Instance->stitchBuffer.operator[](GroupStartStitch);
	auto groupRect = F_RECTANGLE {startStitch.x, startStitch.y, startStitch.x, startStitch.y};
	for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
	  auto const& stitch = Instance->stitchBuffer.operator[](iStitch);
	  groupRect.left     = std::min(stitch.x, groupRect.left);
	  groupRect.top      = std::max(stitch.y, groupRect.top);
	  groupRect.right    = std::max(stitch.x, groupRect.right);
	  groupRect.bottom   = std::min(stitch.y, groupRect.bottom);
	}
	auto const selectedCenter = F_POINT {wrap::midl(groupRect.right, groupRect.left),
	                                     wrap::midl(groupRect.top, groupRect.bottom)};
	FormMoveDelta = F_POINT {markCenter.x - selectedCenter.x, markCenter.y - selectedCenter.y};
	if (Instance->stateMap.test(StateFlag::CNTRV)) {
	  FormMoveDelta.y = 0.0F;
	}
	if (Instance->stateMap.test(StateFlag::CNTRH)) {
	  FormMoveDelta.x = 0.0F;
	}
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  Instance->stitchBuffer.operator[](iStitch) += FormMoveDelta;
	}
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  displayText::shoseln(IDS_FGRPF, IDS_CENT);
}

void form::centir() {
  Instance->stateMap.reset(StateFlag::BIGBOX);
  auto& formList        = Instance->formList;
  AllItemsRect          = getbig(formList, Instance->stitchBuffer);
  auto const itemCenter = F_POINT {wrap::midl(AllItemsRect.right, AllItemsRect.left),
                                   wrap::midl(AllItemsRect.top, AllItemsRect.bottom)};
  auto const hoopCenter =
      F_POINT {wrap::toFloat(UnzoomedRect.cx) / 2.0F, wrap::toFloat(UnzoomedRect.cy) / 2.0F};
  auto const delta = F_POINT {hoopCenter.x - itemCenter.x, hoopCenter.y - itemCenter.y};
  for (auto& stitch : Instance->stitchBuffer) {
	stitch += delta;
  }
  for (auto& formVertice : Instance->formVertices) {
	formVertice += delta;
  }
  for (auto& iForm : formList) {
	iForm.outline();
  }
  xt::setfchk();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::dubean() {
  if (Instance->stitchBuffer.empty()) {
	return;
  }
  thred::savdo();
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	if (auto const beanCount = bean(GroupStartStitch, GroupEndStitch); ClosestPointIndex > GroupStitchIndex) {
	  ClosestPointIndex += beanCount;
	}
	else {
	  GroupStitchIndex += beanCount;
	}
	thred::grpAdj();
  }
  else {
	bean(0, wrap::toUnsigned(Instance->stitchBuffer.size() - 1U));
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::debean() {
  thred::savdo();
  if (Instance->stateMap.test(StateFlag::GRPSEL)) {
	thred::rngadj();
	unbean(GroupStartStitch, GroupEndStitch);
	if (ClosestPointIndex > GroupStitchIndex) {
	  ClosestPointIndex = GroupEndStitch;
	}
	else {
	  GroupStitchIndex = GroupEndStitch;
	}
	auto const iEndStitch = wrap::toUnsigned(Instance->stitchBuffer.size()) - 1U;
	if (ClosestPointIndex > iEndStitch - 1U) {
	  ClosestPointIndex = iEndStitch;
	}
	if (GroupStitchIndex > iEndStitch - 1U) {
	  GroupStitchIndex = iEndStitch;
	}
	thred::grpAdj();
  }
  else {
	if (Instance->stitchBuffer.size() > 2U) {
	  auto iEndStitch = wrap::toUnsigned(Instance->stitchBuffer.size()) - 1U;
	  unbean(0, iEndStitch);
	}
  }
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::clpspac(uint32_t const insertPoint, uint32_t const count) {
  auto const itStartClip = wrap::next(Instance->clipPoints.cbegin(), insertPoint);
  auto const itEndClip   = wrap::next(itStartClip, count);
  Instance->clipPoints.insert(itStartClip, itStartClip, itEndClip);
}

void form::stchadj() {
  auto const     codedClosest = ClosestFormToCursor << FRMSHFT;
  constexpr auto OFFSET       = 1U << FRMSHFT; // coded next form offset
  for (auto& stitch : Instance->stitchBuffer) {
	if (auto codedForm = stitch.attribute & FRMSK; codedForm > codedClosest) {
	  stitch.attribute &= NFRMSK;
	  codedForm += OFFSET;
	  stitch.attribute |= codedForm;
	}
  }
  refilfn(ClosestFormToCursor);
  ++ClosestFormToCursor;
  refilfn(ClosestFormToCursor);
  Instance->stateMap.reset(StateFlag::FRMPSEL);
}

void form::spltfrm() {
  if (!Instance->stateMap.test(StateFlag::FRMPSEL)) {
	displayText::spltmsg();
	return;
  }
  thred::savdo();
  auto const& currentForm = Instance->formList.operator[](ClosestFormToCursor);
  if (currentForm.type == SAT) {
	if (currentForm.satinGuideCount == 0U) {
	  return;
	}
	auto itGuide = wrap::next(Instance->satinGuides.cbegin(), currentForm.satinGuideIndex);
	for (auto guideIndex = 0U; guideIndex < currentForm.satinGuideCount; ++guideIndex) {
	  if (itGuide->start == ClosestVertexToCursor || itGuide->finish == ClosestVertexToCursor) {
		satin::spltsat(guideIndex);
		return;
	  }
	  ++itGuide;
	}
	return;
  }
  if (currentForm.type == FRMLINE) {
	if (!spltlin()) {
	  displayText::tabmsg(IDS_FRM3, false);
	  return;
	}
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::stchs2frm() {
  if (!Instance->stateMap.test(StateFlag::GRPSEL)) {
	displayText::shoseln(IDS_GRPMSG, IDS_STCH2FRM);
	return;
  }
  thred::savdo();
  thred::rngadj();
  // ToDo - Is this still a valid restriction?
  if (constexpr auto STITCHMX = 12000U; // maximum number of stitches in a group
      GroupEndStitch - GroupStartStitch > STITCHMX) {
	displayText::tabmsg(IDS_STMAX, false);
	return;
  }
  auto const vertexCount  = GroupEndStitch - GroupStartStitch + 1U;
  auto       currentForm  = FRM_HEAD {};
  currentForm.type        = FRMLINE;
  currentForm.vertexCount = vertexCount;
  currentForm.vertexIndex = thred::adflt(vertexCount);
  auto itVertex           = wrap::next(Instance->formVertices.begin(), currentForm.vertexIndex);
  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	*itVertex = Instance->stitchBuffer.operator[](iStitch);
	++itVertex;
  }
  currentForm.outline();
  Instance->formList.push_back(currentForm);
  thred::delstchm();
  Instance->stateMap.reset(StateFlag::GRPSEL);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::vrtsclp(uint32_t const formIndex) {
  auto& form = Instance->formList.operator[](formIndex);
  clip::delmclp(formIndex);
  texture::deltx(formIndex);
  auto const& clipBuffer = Instance->clipBuffer;

  form.clipCount = wrap::toUnsigned(clipBuffer.size());
  form.clipIndex = clip::numclp(formIndex);
  form.wordParam = IniFile.fillPhase;
  makpoli();
  form.fillSpacing = IniFile.clipOffset;
  auto itClipPoint = wrap::next(Instance->clipPoints.begin(), form.clipIndex);
  for (auto const& clip : clipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  form.fillType  = VCLPF;
  form.fillColor = ActiveColor;
  form.type      = FRMFPOLY;
  refilfn(formIndex);
}

void form::vrtclp() {
  if (!displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == FALSE) {
	return;
  }
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	CloseClipboard();
	return;
  }
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cy <= CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	Instance->stateMap.set(StateFlag::NOCLP);
	for (auto const selectedForm : Instance->selectedFormList) {
	  if (Instance->formList.operator[](selectedForm).type != FRMLINE) {
		vrtsclp(selectedForm);
	  }
	}
	Instance->stateMap.reset(StateFlag::NOCLP);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	vrtsclp(ClosestFormToCursor);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::horsclp() {
  auto& form = Instance->formList.operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  auto const& clipBuffer = Instance->clipBuffer;

  auto const clipSize = wrap::toUnsigned(clipBuffer.size());
  form.clipCount      = clipSize;
  form.clipIndex      = clip::numclp(ClosestFormToCursor);
  // ToDo - should this be clipSize + 1?
  form.clipCount = clipSize;
  form.wordParam = IniFile.fillPhase;
  makpoli();
  form.fillSpacing = IniFile.clipOffset;
  auto itClipPoint = wrap::next(Instance->clipPoints.begin(), form.clipIndex);
  for (auto const& clip : clipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  form.fillType  = HCLPF;
  form.fillColor = ActiveColor;
  form.type      = FRMFPOLY;
  refilfn(ClosestFormToCursor);
}

void form::horclp() {
  if (!displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == FALSE) {
	return;
  }
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	CloseClipboard();
	return;
  }
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cy > CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  if (!Instance->selectedFormList.empty()) {
	Instance->stateMap.set(StateFlag::NOCLP);
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  if (Instance->formList.operator[](ClosestFormToCursor).type != FRMLINE) {
		horsclp();
	  }
	}
	Instance->stateMap.set(StateFlag::NOCLP);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
  else {
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  horsclp();
	  Instance->stateMap.set(StateFlag::INIT);
	  thred::coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
  }
}

void form::angsclp(FRM_HEAD& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.clipIndex         = clip::numclp(ClosestFormToCursor);
  auto const& clipBuffer = Instance->clipBuffer;

  form.clipCount = wrap::toUnsigned(clipBuffer.size());
  form.wordParam = IniFile.fillPhase;
  makpoli();
  form.clipFillAngle = IniFile.fillAngle;
  form.fillSpacing   = IniFile.clipOffset;
  auto itClipPoint   = wrap::next(Instance->clipPoints.begin(), form.clipIndex);
  for (auto const& clip : clipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  form.fillType  = ANGCLPF;
  form.fillColor = ActiveColor;
  form.type      = FRMFPOLY;
  refilfn(ClosestFormToCursor);
}

void form::angclp() {
  if (!displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == FALSE) {
	return;
  }
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	CloseClipboard();
	return;
  }
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cy <= CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  auto& formList = Instance->formList;

  if (!Instance->selectedFormList.empty()) {
	Instance->stateMap.set(StateFlag::NOCLP);
	for (auto const selectedForm : Instance->selectedFormList) {
	  ClosestFormToCursor = selectedForm;
	  if (auto& form = formList.operator[](selectedForm); form.type != FRMLINE) {
		angsclp(form);
	  }
	}
	Instance->stateMap.reset(StateFlag::NOCLP);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto& form = formList.operator[](ClosestFormToCursor);
	angsclp(form);
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::dubsfil(FRM_HEAD& form) {
  clip::deleclp(ClosestFormToCursor);
  form.edgeType    = EDGEDOUBLE;
  form.borderColor = ActiveColor;
  bsizpar(form);
  dubfn(form);
  refilfn(ClosestFormToCursor);
}

void form::dubfil() {
  if (!displayText::filmsgs(FML_LIN)) {
	return;
  }
  auto& formList = Instance->formList;

  if (!Instance->selectedFormList.empty()) {
	for (auto const selectedForm : Instance->selectedFormList) {
	  auto& form = formList.operator[](selectedForm);
	  dubsfil(form);
	}
	Instance->stateMap.set(StateFlag::INIT);
	thred::coltab();
	Instance->stateMap.set(StateFlag::RESTCH);
	return;
  }
  if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	auto& form = formList.operator[](ClosestFormToCursor);
	dubsfil(form);
	thred::coltab();
	Instance->stateMap.set(StateFlag::INIT);
	Instance->stateMap.set(StateFlag::RESTCH);
  }
}

void form::col2frm() {
  auto  colorChangedCount = 0U;
  auto& formList          = Instance->formList;

  if (formList.empty()) { // no forms to change
	displayText::showMessage(IDS_NCOLCHG, colorChangedCount);
	return;
  }
  auto const formColorPermutations = formList.size() << 4U; // total number of form and color combinations
  auto fillColorHistogram     = std::vector<uint32_t> {};
  auto borderColorHistogram   = std::vector<uint32_t> {};
  auto featherColorHistogram  = std::vector<uint32_t> {};
  auto underlayColorHistogram = std::vector<uint32_t> {};
  // prefer resizing to be explicit
  featherColorHistogram.resize(formColorPermutations);
  fillColorHistogram.resize(formColorPermutations);
  borderColorHistogram.resize(formColorPermutations);
  underlayColorHistogram.resize(formColorPermutations);
  // initialize the histograms
  for (auto const& stitch : Instance->stitchBuffer) {
	auto const formColorCode = stitch.attribute & (COLMSK | FRMSK);
	if ((stitch.attribute & (WLKMSK | CWLKMSK | UNDMSK)) != 0U) {
	  ++underlayColorHistogram[formColorCode];
	  continue;
	}
	if ((stitch.attribute & FTHMSK) != 0U) {
	  ++featherColorHistogram[formColorCode];
	  continue;
	}
	switch (stitch.attribute & TYPMSK) {
	  case FRMFIL: {
		++fillColorHistogram[formColorCode];
		break;
	  }
	  case FRMBFIL: {
		++borderColorHistogram[formColorCode];
		break;
	  }
	  default: {
		outDebugString(L"default hit in col2frm: attribute [{}]\n", stitch.attribute & TYPMSK);
		break;
	  }
	}
  }
  auto startColorOffset = 0U;
  auto endColorOffset   = COLORCNT;
  for (auto& iForm : formList) {
	if (iForm.fillType != 0U) { // form has a fill
	  auto count         = 0U;
	  auto majorityColor = 0U;
	  // find the most common color in the fill
	  for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		if (fillColorHistogram[iColor] > count) {
		  count         = fillColorHistogram[iColor];
		  majorityColor = iColor;
		}
	  }
	  majorityColor &= COLMSK;
	  if (iForm.fillColor != majorityColor) {
		++colorChangedCount;
		wrap::narrow(iForm.fillColor, majorityColor);
	  }
	  // if the form has feathering and the feathering color is not the same as the fill color
	  if (iForm.fillType == FTHF && (iForm.extendedAttribute & AT_FTHBLND) != 0U) {
		count = majorityColor = 0;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (featherColorHistogram[iColor] > count) {
			count         = fillColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLMSK;
		if (iForm.feather.color != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(iForm.feather.color, majorityColor);
		}
	  }
	}
	if (iForm.edgeType != 0U) { // form has a border
	  auto count         = 0U;
	  auto majorityColor = 0U;
	  // find the most common color in the border
	  for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		if (borderColorHistogram[iColor] > count) {
		  count         = borderColorHistogram[iColor];
		  majorityColor = iColor;
		}
	  }
	  majorityColor &= COLMSK;
	  if (iForm.borderColor != majorityColor) {
		++colorChangedCount;
		wrap::narrow(iForm.borderColor, majorityColor);
	  }
	}
	if ((iForm.extendedAttribute & (AT_WALK | AT_CWLK | AT_UND)) != 0U) { // form has an underlay
	  auto count         = 0U;
	  auto majorityColor = 0U;
	  // find the most common color in the underlay
	  for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		if (underlayColorHistogram[iColor] > count) {
		  count         = borderColorHistogram[iColor];
		  majorityColor = iColor;
		}
	  }
	  majorityColor &= COLMSK;
	  if (iForm.underlayColor != majorityColor) {
		++colorChangedCount;
		wrap::narrow(iForm.underlayColor, majorityColor);
	  }
	}
	startColorOffset += COLORCNT;
	endColorOffset += COLORCNT;
  }
}

void form::chain() {
  if (!displayText::filmsgs(FML_CHAIN)) {
	return;
  }
  thred::savdo();
  if (Instance->selectedFormList.empty()) {
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  chan(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(Instance->stitchBuffer.size()));
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
	return;
  }
  for (auto const selectedForm : Instance->selectedFormList) {
	chan(selectedForm);
  }
  Instance->stateMap.set(StateFlag::INIT);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::crop() {
  displayText::frm1pnt();
  if (!Instance->stateMap.test(StateFlag::FORMSEL)) {
	displayText::shoseln(IDS_FRM1MSG, IDS_CROP);
	return;
  }
  thred::savdo();
  // clang-format off
	auto const& form         = Instance->formList.operator[](ClosestFormToCursor);
	auto        iDestination = Instance->stitchBuffer.begin();
  // clang-format on
  for (auto const& stitch : Instance->stitchBuffer) {
	if (cisin(form, stitch.x, stitch.y)) {
	  *iDestination++ = stitch;
	}
  }
  Instance->stitchBuffer.resize(wrap::distance<size_t>(Instance->stitchBuffer.begin(), iDestination));
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::filclpx() {
  if (!displayText::filmsgs(FML_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) == FALSE) {
	return;
  }
  auto* clipMemory = tfc::getPCDClipMemory();
  if (clipMemory == nullptr) {
	auto const str = std::wstring(L"no clipboard data");
	displayText::shoMsg(str, false);
	CloseClipboard();
	return;
  }
  thred::savdo();
  thred::redclp(clipMemory);
  CloseClipboard();
  if (ClipRectSize.cx <= CLPMIN) {
	displayText::tabmsg(IDS_CLP, false);
	return;
  }
  if (Instance->selectedFormList.empty()) {
	if (Instance->stateMap.test(StateFlag::FORMSEL)) {
	  fsclpx(ClosestFormToCursor);
	  Instance->stateMap.set(StateFlag::INIT);
	  thred::coltab();
	  Instance->stateMap.set(StateFlag::RESTCH);
	}
	return;
  }
  for (auto const selectedForm : Instance->selectedFormList) {
	fsclpx(selectedForm);
  }
  Instance->stateMap.set(StateFlag::INIT);
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::srtfrm() {
  auto const& formList = Instance->formList;

  if (formList.empty() || Instance->stitchBuffer.empty()) {
	return;
  }
  auto histogram = std::vector<uint32_t> {};
  histogram.resize(formList.size());
  thred::savdo();
  for (auto const& stitch : Instance->stitchBuffer) {
	auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	++histogram[iForm];
  }
  auto totalStitches = 0U;
  for (auto& entry : histogram) {
	auto const formStitchCount = entry;
	entry                      = totalStitches;
	totalStitches += formStitchCount;
  }
  auto highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.resize(Instance->stitchBuffer.size());
  for (auto const& stitch : Instance->stitchBuffer) {
	auto const iForm              = (stitch.attribute & FRMSK) >> FRMSHFT;
	auto const iHighStitch        = histogram[iForm]++;
	highStitchBuffer[iHighStitch] = stitch;
  }
  std::ranges::copy(highStitchBuffer, Instance->stitchBuffer.begin());
  thred::coltab();
  Instance->stateMap.set(StateFlag::RESTCH);
}

void form::createLayerPens() noexcept {
  LayerPen[0] = wrap::createPen(PS_SOLID, PENNWID, PENSILVR);
  LayerPen[1] = wrap::createPen(PS_SOLID, PENNWID, PENTRQSE);
  LayerPen[2] = wrap::createPen(PS_SOLID, PENNWID, PENLILAC);
  LayerPen[3] = wrap::createPen(PS_SOLID, PENNWID, PENPOLIV);
  LayerPen[4] = wrap::createPen(PS_SOLID, PENNWID, PENTEAL);
}

void form::createFormSelectedPen() noexcept {
  FormSelectedPen = wrap::createPen(PS_SOLID, PENNWID, PENSEGRN);
}

void form::setNewFormVertexCount(uint32_t const vertexCount) noexcept {
  NewFormVertexCount = vertexCount;
}

void form::setButtonholeCornerLength(float const length) noexcept {
  ButtonholeCornerLength = length;
}

auto form::getButtonholeCornerLength() noexcept -> float {
  return ButtonholeCornerLength;
}
