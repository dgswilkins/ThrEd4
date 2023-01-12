// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
// Local Headers
#include "stdafx.h"
#include "switches.h"
#include "clip.h"
#include "displayText.h"
#include "form.h"
#include "formForms.h"
#include "globals.h"
#include "menu.h"
#include "satin.h"
#include "texture.h"
#include "thred.h"
#include "xt.h"

// Standard Libraries
#include <numeric>

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

  // constexpr CLIP_SEG() noexcept = default;
  // CLIP_SEG(CLIP_SEG const&) = default;
  // CLIP_SEG(CLIP_SEG&&) = default;
  // CLIP_SEG& operator=(CLIP_SEG const& rhs) = default;
  // CLIP_SEG& operator=(CLIP_SEG&&) = default;
  //~CLIP_SEG() = default;
};

class CLIP_SORT
{
  public:
  float    segmentLength {};
  float    sideLength {};
  uint32_t vertexIndex {};
  F_POINT  point;

  // constexpr CLIP_SORT() noexcept = default;
  // CLIP_SORT(CLIP_SORT const&) = default;
  // CLIP_SORT(CLIP_SORT&&) = default;
  // CLIP_SORT& operator=(CLIP_SORT const& rhs) = default;
  // CLIP_SORT& operator=(CLIP_SORT&&) = default;
  //~CLIP_SORT() = default;
};

class CLIP_PNT
{
  public:
  float    x {};
  float    y {};
  uint32_t vertexIndex {};
  uint32_t flag {};

  // constexpr CLIP_PNT() noexcept = default;
  // CLIP_PNT(CLIP_PNT const&) = default;
  // CLIP_PNT(CLIP_PNT&&) = default;
  // CLIP_PNT& operator=(CLIP_PNT const& rhs) = default;
  // CLIP_PNT& operator=(CLIP_PNT&&) = default;
  //~CLIP_PNT() = default;
};

class F_POINT_LINE
{
  public:
  float    x {};
  float    y {};
  uint16_t line {};

  // constexpr F_POINT_LINE() noexcept = default;
  // F_POINT_LINE(F_POINT_LINE const&) = default;
  // F_POINT_LINE(F_POINT_LINE&&) = default;
  // F_POINT_LINE& operator=(F_POINT_LINE const& rhs) = default;
  // F_POINT_LINE& operator=(F_POINT_LINE&&) = default;
  //~F_POINT_LINE() = default;
};

class F_SEQ // Forward SequencePath: path of sequenced regions
{
  public:
  uint16_t node {};
  uint16_t nextGroup {};
  bool     skp {}; // path not found

  // constexpr F_SEQ() noexcept = default;
  // F_SEQ(F_SEQ const&) = default;
  // F_SEQ(F_SEQ&&) = default;
  // F_SEQ& operator=(F_SEQ const& rhs) = default;
  // F_SEQ& operator=(F_SEQ&&) = default;
  //~F_SEQ() = default;
};

class LEN_INFO // Sorted line length info
{
  public:
  uint32_t index {};
  bool     isEnd {};
  float    length {};

  // constexpr LEN_INFO() noexcept = default;
  // LEN_INFO(LEN_INFO const&) = default;
  // LEN_INFO(LEN_INFO&&) = default;
  // LEN_INFO& operator=(LEN_INFO const& rhs) = default;
  // LEN_INFO& operator=(LEN_INFO&&) = default;
  //~LEN_INFO() = default;
};

class R_CON // PathMap: path map for sequencing
{
  public:
  uint32_t node {};
  bool     isConnected {};
  uint32_t nextGroup {};

  // constexpr R_CON() noexcept = default;
  // R_CON(R_CON const&) = default;
  // R_CON(R_CON&&) = default;
  // R_CON& operator=(R_CON const& rhs) = default;
  // R_CON& operator=(R_CON&&) = default;
  //~R_CON() = default;
};

class REGION // region for sequencing vertical fills
{
  public:
  uint32_t start {};       // start line of region
  uint32_t end {};         // end line of region
  uint32_t regionBreak {}; // ToDo - Is this member needed?
  uint32_t breakCount {};

  // constexpr REGION() noexcept = default;
  inline REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept;
  // REGION(REGION const&) = default;
  // REGION(REGION&&) = default;
  // REGION& operator=(REGION const& rhs) = default;
  // REGION& operator=(REGION&&) = default;
  //~REGION() = default;
};

inline REGION::REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept :
    start(rhsStart), end(rhsEnd), regionBreak(rhsBreak), breakCount(rhsCount) {
}

class RG_SEQ // TempPath: temporary path connections
{
  public:
  uint32_t pcon {}; // pointer to PathMap entry
  int32_t  count {};
  bool     skp {}; // path not found

  // constexpr RG_SEQ() noexcept = default;
  // RG_SEQ(RG_SEQ const&) = default;
  // RG_SEQ(RG_SEQ&&) = default;
  // RG_SEQ& operator=(RG_SEQ const& rhs) = default;
  // RG_SEQ& operator=(RG_SEQ&&) = default;
  //~RG_SEQ() = default;
};

class V_CLP_X
{
  public:
  uint32_t segment {};
  uint32_t vertex {};

  // constexpr V_CLP_X() noexcept = default;
  // V_CLP_X(V_CLP_X const&) = default;
  // V_CLP_X(V_CLP_X&&) = default;
  // V_CLP_X& operator=(V_CLP_X const& rhs) = default;
  // V_CLP_X& operator=(V_CLP_X&&) = default;
  //~V_CLP_X() = default;
};

class V_RECT_2
{
  public:
  F_POINT aipnt {};
  F_POINT aopnt {};
  F_POINT bipnt {};
  F_POINT bopnt {};
  F_POINT cipnt {};
  F_POINT copnt {};
  F_POINT dipnt {};
  F_POINT dopnt {};

  // constexpr V_RECT_2() noexcept = default;
  // V_RECT_2(V_RECT_2 const&) = default;
  // V_RECT_2(V_RECT_2&&) = default;
  // V_RECT_2& operator=(V_RECT_2 const& rhs) = default;
  // V_RECT_2& operator=(V_RECT_2&&) = default;
  //~V_RECT_2() = default;
};

constexpr auto BHWIDTH  = 20.0F; // Button hole width
constexpr auto CLPMIN   = 0.5F;  // if clipboard data width less than this, then don't fill
constexpr auto CLPMINVT = 1.2F;  // Minimum clip width for skinny vertical clips
constexpr auto FRECONT  = 0x80U; // 1000 0000 contour refil
constexpr auto NFRECONT = 0x7fU; // 0111 1111 contour refil
constexpr auto SEQTOP   = int32_t {2};
constexpr auto SEQBOT   = int32_t {3};

namespace fi {
void adfrm(uint32_t iForm);
void angout(FRM_HEAD& angledForm);
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

constexpr auto duat(uint32_t attribute) -> uint32_t;

void dubfn(FRM_HEAD const& currentForm);
void ducon() noexcept;
void dudif(F_POINT const& start, F_POINT const& finish, F_POINT& delta) noexcept;
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
void filsfn();
auto findDistanceToSide(F_POINT const& lineStart, F_POINT const& lineEnd, F_POINT const& point, float& distance) noexcept
    -> float;
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
void frmpnts(uint32_t type);
void frmpoly(gsl::span<POINT> const& source) noexcept;
void frmsqr(uint32_t vertexIndex, uint32_t iVertex);
void frmsqr0(POINT const& controlPoint);
void frmx(POINT const& controlPoint, HDC hDC);
void fsangl(FRM_HEAD& form);
void fsclp(uint32_t formIndex);
void fsclpx(uint32_t formIndex);
void fshor(FRM_HEAD& form);
void fspic(uint32_t formIndex);
void fsvrt();
auto getbig(std::vector<FRM_HEAD> const& formList, std::vector<F_POINT_ATTR> const& stitchBuffer) noexcept
    -> F_RECTANGLE;
auto getlen(std::vector<CLIP_PNT>&      clipStitchPoints,
            std::vector<float> const&   lengths,
            uint32_t                    iPoint,
            std::vector<F_POINT> const& currentFormVertices) -> float;
auto getLineSortOrder(std::vector<SMAL_PNT_L>& lineEndpoints) -> std::vector<uint32_t>;
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
auto isclos(std::vector<SMAL_PNT_L> const& lineEndpoints, uint32_t line0Index, uint32_t line1Index, float gapToClosestRegion) noexcept
    -> bool;
auto isect(uint32_t                    vertex0,
           uint32_t                    vertex1,
           F_POINT&                    intersection,
           float&                      length,
           F_POINT const&              lineSegmentStart,
           F_POINT const&              lineSegmentEnd,
           std::vector<F_POINT> const& currentFormVertices) -> bool;
auto isin(FRM_HEAD const&             form,
          std::vector<V_CLP_X> const& regionCrossingData,
          float                       xCoordinate,
          float                       yCoordinate,
          uint32_t                    regionCrossingStart,
          uint32_t                    regionCrossingEnd,
          F_RECTANGLE const&          boundingRect,
          std::vector<F_POINT> const& currentFormVertices) -> bool;
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
            float                          gapToClosestRegion) noexcept -> bool;
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
void plbak(uint32_t backPoint);
void plbrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices);
void plfn(FRM_HEAD const&              form,
          std::vector<V_RECT_2> const& underlayVerticalRect,
          std::vector<V_RECT_2> const& fillVerticalRect,
          std::vector<V_RECT_2> const& prct,
          float                        width,
          F_POINT&                     stitchPoint);
void prebrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices);
auto proj(F_POINT const& point, float slope, F_POINT const& point0, F_POINT const& point1, F_POINT& intersectionPoint) noexcept
    -> bool;
auto projh(float yCoordinate, F_POINT const& point0, F_POINT const& point1, F_POINT& intersection) noexcept
    -> bool;
auto projv(float xCoordinate, F_POINT const& lowerPoint, F_POINT const& upperPoint, F_POINT& intersection) noexcept
    -> bool;
void prpsbrd(uint32_t formIndex);
void prsmal(float width);
auto px2stchf(POINT const& screen) noexcept -> F_POINT;
void rats();
auto regclos(std::vector<uint32_t> const&   groupIndexSequence,
             std::vector<SMAL_PNT_L> const& lineEndpoints,
             std::vector<uint32_t> const&   sortedLineIndices,
             uint32_t                       iRegion0,
             uint32_t                       iRegion1,
             std::vector<REGION> const&     regionsList,
             float                          gapToClosestRegion,
             uint32_t&                      nextGroup) noexcept -> bool;
auto reglen(std::vector<SMAL_PNT_L> const&       lineEndpoints,
            std::vector<uint32_t> const&         sortedLineIndices,
            uint32_t                             iRegion,
            std::array<F_POINT, SQRCORNS> const& lastRegionCorners,
            std::vector<REGION> const&           regionsList) noexcept -> float;
void ritapbrd();
void ritbrd(FRM_HEAD const& form);
void ritfil();
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
auto spComp(gsl::not_null<SMAL_PNT_L const*> arg1, gsl::not_null<SMAL_PNT_L const*> arg2) noexcept -> bool;
void spend(std::vector<V_RECT_2> const& fillVerticalRect, uint32_t start, uint32_t finish, F_POINT& stitchPoint);
auto spltlin() -> bool;
void sprct(std::vector<F_POINT> const& vertices,
           uint32_t                    vertexIndex,
           std::vector<V_RECT_2>&      fillVerticalRect,
           uint32_t                    start,
           uint32_t                    finish);
void spurct(std::vector<V_RECT_2>&       underlayVerticalRect,
            std::vector<V_RECT_2> const& fillVerticalRect,
            uint32_t                     iRect) noexcept;
void spurfn(F_POINT const& innerPoint, F_POINT const& outerPoint, F_POINT& underlayInnerPoint, F_POINT& underlayOuterPoint) noexcept;
void srtf(std::vector<F_POINT_ATTR> const& tempStitchBuffer, uint32_t start, uint32_t finish);
void stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept;
void trfrm(F_POINT const& bottomLeftPoint,
           F_POINT const& topLeftPoint,
           F_POINT const& bottomRightPoint,
           F_POINT const& topRightPoint);
void unbean(uint32_t start, uint32_t& finish);
void uncon();
auto unvis(boost::dynamic_bitset<> const& visitedRegions, int32_t& visitedIndex) -> bool;
auto vclpComp(V_CLP_X const& vclpx1, V_CLP_X const& vclpx2) noexcept -> bool;
auto vscmp(uint32_t index1, uint32_t index2) noexcept -> bool;
} // namespace fi

static auto FormForInsert  = static_cast<FRM_HEAD*>(nullptr); // insert form vertex in this form
static auto FormVertexNext = uint32_t {}; // form vertex storage for form vertex insert
static auto FormVertexPrev = uint32_t {}; // form vertex storage for form vertex insert

auto fi::fplComp(F_POINT_LINE const& point1, F_POINT_LINE const& point2) noexcept -> bool {
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

auto form::chkmax(uint32_t arg0, uint32_t arg1) noexcept -> bool {
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

void form::fltspac(uint32_t vertexOffset, uint32_t count) {
  auto const currentVertex = FormList->operator[](ClosestFormToCursor).vertexIndex + vertexOffset;
  auto const startVertex   = wrap::next(FormVertices->cbegin(), currentVertex);
  auto constexpr VAL       = F_POINT {};
  FormVertices->insert(startVertex, count, VAL);
  for (auto iForm = ClosestFormToCursor + 1U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	auto& form = FormList->operator[](iForm);
	form.vertexIndex += count;
  }
}

void form::delflt(uint32_t formIndex) {
  if (auto itForm = wrap::next(FormList->begin(), formIndex); itForm->vertexCount != 0U) {
	auto const vertexCount   = itForm->vertexCount;
	auto const itStartVertex = wrap::next(FormVertices->cbegin(), itForm->vertexIndex);
	auto const itEndVertex   = wrap::next(itStartVertex, itForm->vertexCount);
	FormVertices->erase(itStartVertex, itEndVertex);
	for (++itForm; itForm < FormList->end(); ++itForm) {
	  itForm->vertexIndex -= vertexCount;
	}
  }
}

void fi::rotbak(float rotationAngle, F_POINT const& rotationCenter) noexcept {
  for (auto& iSequence : *OSequence) {
	thred::rotflt(iSequence, rotationAngle, rotationCenter);
  }
}

void form::delmfil(uint32_t formIndex) {
  if (FormList->operator[](formIndex).isTexture()) {
	texture::deltx(formIndex);
  }
  clip::delmclp(formIndex);
  // find the first stitch to delete
  auto const codedForm = formIndex << FRMSHFT;
  if (auto const firstStitch =
          std::ranges::find_if(*StitchBuffer,
                               [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                             return ((stitch.attribute & (FRMSK | NOTFRM)) == codedForm) &&
	                                    ((stitch.attribute & (TYPFRM | FTHMSK)) != 0U);
                               });
      firstStitch != StitchBuffer->end()) {
	// we found the first stitch, so now delete the stitches in the form
	StitchBuffer->erase(std::remove_if(firstStitch,
	                                   StitchBuffer->end(),
	                                   [codedForm](F_POINT_ATTR const& stitch) -> bool {
	                                     return (stitch.attribute & FRMSK) == codedForm;
	                                   }),
	                    StitchBuffer->end());
  }
}

void form::fsizpar(FRM_HEAD& form) noexcept {
  form.lengthOrCount.stitchLength = UserStitchLength;
  form.maxFillStitchLen           = IniFile.maxStitchLength;
  form.minFillStitchLen           = MinStitchLength;
}

void form::chkcont() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  fsizpar(form);
  if (form.fillType != CONTF) {
	if (form.satinGuideCount != 0U) {
	  auto shortestGuideIndex = 0U;
	  auto minimumLength      = MAXDWORD;
	  auto itGuide            = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
	  for (auto iGuide = 0U; iGuide < form.satinGuideCount; ++iGuide) {
		if (auto const length = itGuide->finish - itGuide->start; length < minimumLength) {
		  minimumLength      = length;
		  shortestGuideIndex = iGuide;
		}
		++itGuide;
	  }
	  auto const shortestGuide = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide + shortestGuideIndex);

	  form.angleOrClipData.guide = *shortestGuide;
	  satin::delsac(ClosestFormToCursor);
	}
	else {
	  form.angleOrClipData.guide.start  = 1;
	  form.angleOrClipData.guide.finish = form.vertexCount - 2U;
	}
  }
  form.attribute |= FRECONT;
  form.type     = FRMLINE;
  form.fillType = CONTF;
}

void form::ispcdclp() {
  StateMap->reset(StateFlag::WASPCDCLP);
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  StateMap->set(StateFlag::WASPCDCLP);
	}
  }
  CloseClipboard();
}

auto form::sfCor2px(F_POINT const& stitchPoint) -> POINT {
  return POINT {wrap::ceil<int32_t>((stitchPoint.x - ZoomRect.left) * ZoomRatio.x),
                wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                    (stitchPoint.y - ZoomRect.bottom) * ZoomRatio.y)};
}

auto fi::px2stchf(POINT const& screen) noexcept -> F_POINT {
  auto const factorX = wrap::toFloat(screen.x) / wrap::toFloat(StitchWindowClientRect.right);
  auto const factorY = wrap::toFloat(StitchWindowClientRect.bottom - screen.y) /
                       wrap::toFloat(StitchWindowClientRect.bottom);
  return F_POINT {factorX * (ZoomRect.right - ZoomRect.left) + ZoomRect.left,
                  factorY * (ZoomRect.top - ZoomRect.bottom) + ZoomRect.bottom};
}

void form::frmlin(FRM_HEAD const& form) {
  if (form.vertexCount != 0U) {
	FormLines->clear();
	FormLines->reserve(form.vertexCount);
	auto const itFirstVertex   = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	auto       itCurrentVertex = itFirstVertex;
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  FormLines->push_back(POINT {std::lround((itCurrentVertex->x - ZoomRect.left) * ZoomRatio.x),
	                              std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                          (itCurrentVertex->y - ZoomRect.bottom) * ZoomRatio.y)});
	  ++itCurrentVertex;
	}
	FormLines->push_back(POINT {std::lround((itFirstVertex->x - ZoomRect.left) * ZoomRatio.x),
	                            std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                        (itFirstVertex->y - ZoomRect.bottom) * ZoomRatio.y)});
  }
}

void form::frmlin(std::vector<F_POINT> const& vertices) {
  if (auto const vertexMax = vertices.size(); vertexMax != 0U) {
	auto& formLines = *FormLines;
	formLines.clear();
	formLines.reserve(vertexMax);
	for (auto iVertex = 0U; iVertex < vertexMax; ++iVertex) {
	  formLines.push_back(POINT {std::lround((vertices[iVertex].x - ZoomRect.left) * ZoomRatio.x),
	                             std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                         (vertices[iVertex].y - ZoomRect.bottom) * ZoomRatio.y)});
	}
	formLines.push_back(POINT {std::lround((vertices[0].x - ZoomRect.left) * ZoomRatio.x),
	                           std::lround(wrap::toFloat(StitchWindowClientRect.bottom) -
	                                       (vertices[0].y - ZoomRect.bottom) * ZoomRatio.y)});
  }
}

void form::dufrm() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::unfrm() {
  if (StateMap->testAndReset(StateFlag::SHOFRM)) {
	dufrm();
  }
}

void form::mdufrm() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  if (FormList->operator[](ClosestFormToCursor).type == FRMLINE) {
	wrap::polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount - 1);
  }
  else {
	wrap::polyline(StitchWindowDC, FormLines->data(), NewFormVertexCount);
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void form::munfrm() {
  if (StateMap->testAndReset(StateFlag::SHOFRM)) {
	form::mdufrm();
  }
}

void fi::rats() {
  if (StateMap->test(StateFlag::ZUMED)) {
	HorizontalRatio = (ZoomRect.right - ZoomRect.left) / wrap::toFloat(StitchWindowClientRect.right);
	VerticalRatio = (ZoomRect.top - ZoomRect.bottom) / wrap::toFloat(StitchWindowClientRect.bottom);
  }
  else {
	HorizontalRatio = wrap::toFloat(UnzoomedRect.cx) / wrap::toFloat(StitchWindowClientRect.right);
	VerticalRatio   = wrap::toFloat(UnzoomedRect.cy) / wrap::toFloat(StitchWindowClientRect.bottom);
  }
}

void form::setfrm() {
  if (!FormList->empty()) {
	fi::rats();
	ClosestFormToCursor  = wrap::toUnsigned(FormList->size() - 1U);
	auto const point     = fi::px2stchf(FormLines->front());
	auto       itVertex  = wrap::next(FormVertices->begin(), FormList->back().vertexIndex);
	auto const delta     = F_POINT {point.x - itVertex->x, point.y - itVertex->y};
	auto&      rectangle = FormList->back().rectangle;
	rectangle            = F_RECTANGLE {BIGFLOAT, 0.0F, 0.0F, BIGFLOAT};
	for (auto iVertex = 0U; iVertex < NewFormVertexCount - 1U; ++iVertex) {
	  itVertex->x += delta.x;
	  itVertex->y += delta.y;
	  if (itVertex->x < rectangle.left) {
		rectangle.left = itVertex->x;
	  }
	  if (itVertex->x > rectangle.right) {
		rectangle.right = itVertex->x;
	  }
	  if (itVertex->y > rectangle.top) {
		rectangle.top = itVertex->y;
	  }
	  if (itVertex->y < rectangle.bottom) {
		rectangle.bottom = itVertex->y;
	  }
	  ++itVertex;
	}
	StateMap->reset(StateFlag::FORMIN);
	StateMap->set(StateFlag::INIT);
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::form() {
  displayText::shoMsg(displayText::loadStr(IDS_FMEN), true);
  StateMap->set(StateFlag::FORMIN);
  StateMap->reset(StateFlag::INSRT);
  thred::duzrat();
}

void fi::frmsqr(uint32_t vertexIndex, uint32_t iVertex) {
  auto       line             = std::array<POINT, TRIPNTS> {};
  auto const itCurrentVertex  = wrap::next(FormVertices->cbegin(), vertexIndex + iVertex);
  auto const itPreviousVertex = std::next(itCurrentVertex, -1);
  line[1]                     = thred::stch2pxr(*itCurrentVertex);
  auto const ratio = wrap::toFloat(MulDiv(IniFile.formVertexSizePixels, *ScreenDPI, STDDPI)) /
                     wrap::toFloat(StitchWindowClientRect.right);
  auto       length = (ZoomRect.right - ZoomRect.left) * ratio * 2.0F;
  auto const delta =
      F_POINT {itPreviousVertex->x - itCurrentVertex->x, itPreviousVertex->y - itCurrentVertex->y};
  auto       angle  = atan2(delta.y, delta.x);
  auto       xVal   = length * cos(angle);
  auto       yVal   = length * sin(angle);
  auto       offset = F_POINT {xVal, yVal};
  auto const point  = F_POINT {itCurrentVertex->x + offset.x, itCurrentVertex->y + offset.y};
  angle             = atan2(-delta.x, delta.y);
  length /= 2.0F;
  xVal               = length * cos(angle);
  yVal               = length * sin(angle);
  offset             = F_POINT {xVal, yVal};
  auto adjustedPoint = F_POINT {point.x + offset.x, point.y + offset.y};
  line[0] = line[3] = thred::stch2pxr(adjustedPoint);
  adjustedPoint     = F_POINT {point.x - offset.x, point.y - offset.y};
  line[2]           = thred::stch2pxr(adjustedPoint);
  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
}

void form::selsqr(POINT const& controlPoint, HDC hDC) {
  auto line = std::array<POINT, SQPNTS> {};
  auto const offset = MulDiv(gsl::narrow<int32_t>(IniFile.formVertexSizePixels), *ScreenDPI, STDDPI);
  line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
  line[0].y = line[1].y = controlPoint.y - offset;
  line[1].x = line[2].x = controlPoint.x + offset;
  line[2].y = line[3].y = controlPoint.y + offset;
  line[4].y             = controlPoint.y - offset;
  wrap::polyline(hDC, line.data(), wrap::toUnsigned(line.size()));
}

void fi::frmsqr0(POINT const& controlPoint) {
  auto line = std::array<POINT, SQPNTS> {};
  if (auto const offset = MulDiv(gsl::narrow<int32_t>(IniFile.formBoxSizePixels), *ScreenDPI, STDDPI);
      offset != 0) {
	line[0].x = line[3].x = line[4].x = controlPoint.x - offset;
	line[0].y = line[1].y = controlPoint.y - offset;
	line[1].x = line[2].x = controlPoint.x + offset + 1;
	line[2].y = line[3].y = controlPoint.y + offset + 1;
	line[4].y             = controlPoint.y - 1;
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
}

void fi::frmx(POINT const& controlPoint, HDC hDC) {
  auto       line   = std::array<POINT, LNPNTS> {};
  auto const offset = MulDiv(8, *ScreenDPI, STDDPI);
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

void form::ratsr() {
  if (StateMap->test(StateFlag::ZUMED)) {
	HorizontalRatio = wrap::toFloat(StitchWindowClientRect.right) / (ZoomRect.right - ZoomRect.left);
	VerticalRatio = wrap::toFloat(StitchWindowClientRect.bottom) / (ZoomRect.top - ZoomRect.bottom);
  }
  else {
	HorizontalRatio = wrap::toFloat(StitchWindowClientRect.right) / wrap::toFloat(UnzoomedRect.cx);
	VerticalRatio   = wrap::toFloat(StitchWindowClientRect.bottom) / wrap::toFloat(UnzoomedRect.cy);
  }
}

void form::ritfrct(uint32_t iForm, HDC hDC) {
  auto pixelOutline = std::array<POINT, OUTPNTS> {};
  auto formOutline  = std::array<F_POINT, OUTPNTS> {};
  ratsr();
  SelectObject(StitchWindowDC, FormPen);
  SetROP2(StitchWindowDC, R2_XORPEN);
  auto const& rectangle = FormList->operator[](iForm).rectangle;
  SelectObject(hDC, FormSelectedPen);
  formOutline[0].x = formOutline[6].x = formOutline[7].x = formOutline[8].x = rectangle.left;
  formOutline[0].y = formOutline[1].y = formOutline[2].y = formOutline[8].y = rectangle.top;
  formOutline[2].x = formOutline[3].x = formOutline[4].x = rectangle.right;
  formOutline[4].y = formOutline[5].y = formOutline[6].y = rectangle.bottom;
  formOutline[1].x = formOutline[5].x = wrap::midl(rectangle.right, rectangle.left);
  formOutline[3].y = formOutline[7].y = wrap::midl(rectangle.top, rectangle.bottom);

  auto ipixelOutline = pixelOutline.begin();
  for (auto& controlPoint : formOutline) {
	*ipixelOutline = form::sfCor2px(controlPoint);
	++ipixelOutline;
  }
  wrap::polyline(hDC, pixelOutline.data(), wrap::toUnsigned(pixelOutline.size()));
  auto const spOutline = gsl::span {pixelOutline};
  // drawing the boxes on corners and sides, so don't overwrite the first box which will "erase" it
  auto const subsp = spOutline.subspan(0, spOutline.size() - 1);
  for (auto const& controlPoint : subsp) {
	form::selsqr(controlPoint, hDC);
  }
  SetROP2(StitchWindowDC, R2_COPYPEN);
  if (StateMap->testAndReset(StateFlag::GRPSEL)) {
	StateMap->reset(StateFlag::SELSHO);
	SearchLine->clear();
	SearchLine->shrink_to_fit();
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::delfrms() {
  thred::savdo();
  FormVertices->clear();
  ClipPoints->clear();
  FormList->clear();
  SatinGuides->clear();
  for (auto& stitch : *StitchBuffer) {
	stitch.attribute &= NFRM_NTYP;
	stitch.attribute |= NOTFRM;
  }
}

void form::fselrct(uint32_t iForm) {
  // clang-format off
  auto const& form = FormList->operator[](iForm);
  auto formOutline = std::array<F_POINT, SQPNTS> {};
  auto line        = std::array<POINT, SQPNTS> {};
  // clang-format on
  formOutline[0].x = formOutline[3].x = formOutline[4].x = form.rectangle.left;
  formOutline[0].y = formOutline[1].y = formOutline[4].y = form.rectangle.top;
  formOutline[1].x = formOutline[2].x = form.rectangle.right;
  formOutline[2].y = formOutline[3].y = form.rectangle.bottom;
  auto iFormOutline                   = formOutline.begin();
  for (auto& point : line) {
	point.x = std::lround((iFormOutline->x - ZoomRect.left) * HorizontalRatio);
	point.y = std::lround((ZoomRect.top - iFormOutline->y) * VerticalRatio);
	if (point.x < SelectedFormsRect.left) {
	  SelectedFormsRect.left = point.x;
	}
	if (point.y < SelectedFormsRect.top) {
	  SelectedFormsRect.top = point.y;
	}
	if (point.x > SelectedFormsRect.right) {
	  SelectedFormsRect.right = point.x;
	}
	if (point.y > SelectedFormsRect.bottom) {
	  SelectedFormsRect.bottom = point.y;
	}
	++iFormOutline;
  }
  auto const last = POINT {std::lround((formOutline[0].x - ZoomRect.left) * HorizontalRatio),
                           std::lround((ZoomRect.top - formOutline[0].y) * VerticalRatio)};
  if (last.x < SelectedFormsRect.left) {
	SelectedFormsRect.left = last.x;
  }
  if (last.y < SelectedFormsRect.top) {
	SelectedFormsRect.top = last.y;
  }
  if (last.x > SelectedFormsRect.right) {
	SelectedFormsRect.right = last.x;
  }
  if (last.y > SelectedFormsRect.bottom) {
	SelectedFormsRect.bottom = last.y;
  }
  if (OutLineEverySelectedForm) {
	wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
  }
}

void form::rct2sel(RECT const& rectangle, std::vector<POINT>& line) noexcept {
  line[0].x = line[6].x = line[7].x = line[8].x = rectangle.left;
  line[0].y = line[1].y = line[2].y = line[8].y = rectangle.top;
  line[2].x = line[3].x = line[4].x = rectangle.right;
  line[4].y = line[5].y = line[6].y = rectangle.bottom;
  line[1].x = line[5].x = ((rectangle.right - rectangle.left) / 2) + rectangle.left;
  line[3].y = line[7].y = ((rectangle.bottom - rectangle.top) / 2) + rectangle.top;
}

void form::dubig() {
  form::rct2sel(SelectedFormsRect, *SelectedFormsLine);
  SelectObject(StitchWindowMemDC, SelectAllPen);
  wrap::polyline(StitchWindowMemDC, SelectedFormsLine->data(), wrap::toUnsigned(SelectedFormsLine->size()));
  for (auto iPoint = 0U; iPoint < (SelectedFormsLine->size() - 1U); ++iPoint) {
	form::selsqr(SelectedFormsLine->operator[](iPoint), StitchWindowMemDC);
  }
}

void fi::frmpoly(gsl::span<POINT> const& source) noexcept {
  for (auto const& iPoint : source) {
	wrap::polyline(StitchWindowMemDC, std::addressof(iPoint), LNPNTS);
  }
}

void form::dupsel(HDC hDC) {
  SelectObject(hDC, FormPen);
  SetROP2(hDC, R2_XORPEN);
  wrap::polyline(hDC, SelectedPointsLine->data(), wrap::toUnsigned(SelectedPointsLine->size()));
  // iPoint = SelectedFormVertices.start;
  for (auto iPoint = 0U; iPoint < (SelectedPointsLine->size() - 1U); ++iPoint) {
	form::selsqr(SelectedPointsLine->operator[](iPoint), hDC);
  }
  fi::frmx(EndPointCross, hDC);
}

void form::unpsel() {
  if (StateMap->testAndReset(StateFlag::SHOPSEL)) {
	form::dupsel(StitchWindowDC);
  }
}

auto form::sRct2px(F_RECTANGLE const& stitchRect) -> RECT {
  return RECT {wrap::ceil<int32_t>((stitchRect.left - ZoomRect.left) * ZoomRatio.x),
               wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                   (stitchRect.top - ZoomRect.bottom) * ZoomRatio.y),
               wrap::ceil<int32_t>((stitchRect.right - ZoomRect.left) * ZoomRatio.x),
               wrap::ceil<int32_t>(wrap::toFloat(StitchWindowClientRect.bottom) -
                                   (stitchRect.bottom - ZoomRect.bottom) * ZoomRatio.y)};
}

void form::drwfrm() {
  StateMap->reset(StateFlag::SHOMOV);
  StateMap->reset(StateFlag::SHOPSEL);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  ratsr();
  thred::duzrat();
  auto const maxForm = FormList->size();
  for (auto iForm = 0U; iForm < maxForm; ++iForm) {
	auto const& form = FormList->operator[](iForm);
	form::frmlin(form);
	if (!FormLines->empty()) {
	  // NOLINTNEXTLINE(hicpp-signed-bitwise)
	  if (auto const layer = gsl::narrow_cast<uint8_t>((form.attribute & FRMLMSK) >> 1U);
	      (ActiveLayer == 0U) || (layer == 0U) || layer == ActiveLayer) {
		auto line      = std::array<POINT, 2> {};
		auto lastPoint = 0U;
		if (form.type == SAT) {
		  if ((form.attribute & FRMEND) != 0U) {
			SelectObject(StitchWindowMemDC, FormPen3px);
			wrap::polyline(StitchWindowMemDC, FormLines->data(), LNPNTS);
			lastPoint = 1;
		  }
		  if (form.wordParam != 0U) {
			SelectObject(StitchWindowMemDC, FormPen);
			fi::frmpoly(gsl::span<POINT>(std::addressof(FormLines->operator[](1)), form.wordParam - 1));
			SelectObject(StitchWindowMemDC, FormPen3px);
			wrap::polyline(StitchWindowMemDC, std::addressof(FormLines->operator[](form.wordParam)), LNPNTS);
			SelectObject(StitchWindowMemDC, thred::getLayerPen(layer));
			lastPoint = form.wordParam + 1U;
		  }
		  auto const maxGuide      = FormList->operator[](iForm).satinGuideCount;
		  auto const itFirstVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
		  auto       itGuide       = wrap::next(SatinGuides->cbegin(), form.satinOrAngle.guide);
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
		SelectObject(StitchWindowMemDC, thred::getLayerPen(layer));
		if (form.type == FRMLINE) {
		  if (form.vertexCount > 0) {
			fi::frmpoly(gsl::span<POINT>(FormLines->data(), form.vertexCount - 1));
			if (form.fillType == CONTF) {
			  auto const itFirstVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			  auto const itStartVertex = wrap::next(itFirstVertex, form.angleOrClipData.guide.start);
			  auto const itFinishVertex = wrap::next(itFirstVertex, form.angleOrClipData.guide.finish);
			  thred::sCor2px(*itStartVertex, line[0]);
			  thred::sCor2px(*itFinishVertex, line[1]);
			  wrap::polyline(StitchWindowMemDC, line.data(), wrap::toUnsigned(line.size()));
			}
		  }
		}
		else {
		  if (form.vertexCount > lastPoint) {
			fi::frmpoly(gsl::span<POINT>(std::addressof(FormLines->operator[](lastPoint)),
			                             form.vertexCount - lastPoint));
		  }
		}
		if (ClosestFormToCursor == iForm && StateMap->test(StateFlag::FRMPSEL)) {
		  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
			if (iVertex == ClosestVertexToCursor) {
			  fi::frmx(FormLines->operator[](iVertex), StitchWindowMemDC);
			}
			else {
			  fi::frmsqr(form.vertexIndex, iVertex);
			}
		  }
		  if (ClosestVertexToCursor != 0U) {
			fi::frmsqr0(FormLines->front());
		  }
		  else {
			fi::frmx(FormLines->front(), StitchWindowMemDC);
		  }
		  displayText::ritnum(IDS_NUMPNT, ClosestVertexToCursor);
		}
		else {
		  auto& formLines = *FormLines;
		  for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
			fi::frmsqr(form.vertexIndex, iVertex);
		  }
		  SelectObject(StitchWindowMemDC, FormSelectedPen);
		  fi::frmsqr0(formLines[0]);
		}
		if (StateMap->test(StateFlag::FPSEL) && ClosestFormToCursor == iForm) {
		  SelectedPixelsRect = form::sRct2px(SelectedVerticesRect);
		  form::rct2sel(SelectedPixelsRect, *SelectedPointsLine);
		  StateMap->set(StateFlag::SHOPSEL);
		  form::dupsel(StitchWindowMemDC);
		}
	  }
	}
  }
  if (!SelectedFormList->empty()) {
	SelectObject(StitchWindowMemDC, MultiFormPen);
	ratsr();
	SelectedFormsRect.top = SelectedFormsRect.left =
	    std::numeric_limits<decltype(SelectedFormsRect.top)>::max();
	SelectedFormsRect.bottom = SelectedFormsRect.right = 0;
	for (auto const selectedForm : (*SelectedFormList)) {
	  fselrct(selectedForm);
	}
	wrap::narrow_cast(SelectedFormsSize.x, SelectedFormsRect.right - SelectedFormsRect.left);
	wrap::narrow_cast(SelectedFormsSize.y, SelectedFormsRect.bottom - SelectedFormsRect.top);
	dubig();
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form::ritfrct(ClosestFormToCursor, StitchWindowMemDC);
	}
	if (StateMap->test(StateFlag::FRMPMOV)) {
	  thred::ritmov(ClosestFormToCursor);
	  RubberBandLine->operator[](1).x = Msg.pt.x - StitchWindowOrigin.x;
	  RubberBandLine->operator[](1).y = Msg.pt.y - StitchWindowOrigin.y;
	  StateMap->set(StateFlag::SHOMOV);
	  thred::ritmov(ClosestFormToCursor);
	}
  }
  SetROP2(StitchWindowMemDC, R2_COPYPEN);
}

void fi::setpoli() {
  auto form           = FRM_HEAD {};
  form.type           = FRMFPOLY;
  form.attribute      = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  form.underlayIndent = IniFile.underlayIndent;
  FormList->push_back(form);
  StateMap->set(StateFlag::SATIN);
  StateMap->set(StateFlag::INIT);
}

void fi::setlin() {
  auto form      = FRM_HEAD {};
  form.type      = FRMLINE;
  form.attribute = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  FormList->push_back(form);
  StateMap->set(StateFlag::SATIN);
  StateMap->set(StateFlag::INIT);
}

void fi::setrpoli() {
  thred::unmsg();
  displayText::pntmsg(IDS_REGP);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRPOL);
  displayText::numWnd();
}

void fi::setstar() {
  thred::unmsg();
  displayText::pntmsg(IDS_STAR);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRSTAR);
  displayText::numWnd();
}

void fi::setspir() {
  thred::unmsg();
  displayText::pntmsg(IDS_SPIR);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRSPIR);
  displayText::numWnd();
}

void fi::sethart() {
  thred::unmsg();
  displayText::pntmsg(IDS_HEART);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRHART);
  displayText::numWnd();
}

void fi::setlens() {
  thred::unmsg();
  displayText::pntmsg(IDS_LENS);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRLENS);
  displayText::numWnd();
}

void fi::seteg() {
  thred::unmsg();
  displayText::pntmsg(IDS_EGG);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTREG);
  displayText::numWnd();
}

void fi::setzig() {
  thred::unmsg();
  displayText::pntmsg(IDS_ZIG);
  StateMap->set(StateFlag::NUMIN);
  StateMap->set(StateFlag::ENTRZIG);
  displayText::numWnd();
}

void form::setmfrm() {
  // clang-format off
  auto const& closeForm = FormList->operator[](ClosestFormToCursor);
  auto        itVertex  = wrap::next(FormVertices->cbegin(), closeForm.vertexIndex);
  auto        point     = form::sfCor2px(itVertex[0]);
  auto const  offset    =
      POINT {Msg.pt.x - StitchWindowOrigin.x - point.x + std::lround(FormMoveDelta.x),
             Msg.pt.y - StitchWindowOrigin.y - point.y + std::lround(FormMoveDelta.y)};
  auto&       formLines = *FormLines;
  // clang-format on
  formLines.resize(wrap::toSize(closeForm.vertexCount) + 1U);
  for (auto iForm = 0U; iForm < closeForm.vertexCount; ++iForm) {
	point              = form::sfCor2px(*itVertex);
	formLines[iForm].x = point.x + offset.x;
	formLines[iForm].y = point.y + offset.y;
	++itVertex;
  }
  formLines[closeForm.vertexCount] = formLines[0];
}

void form::durpoli(uint32_t vertexCount) {
  if (vertexCount < 3) {
	vertexCount = 3;
  }
  auto const stepAngle = PI_F2 / wrap::toFloat(vertexCount);
  // 500 gives us a reasonably sized default
  auto const length = 500.0F / wrap::toFloat(vertexCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  auto newForm        = FRM_HEAD {};
  newForm.vertexIndex = thred::adflt(vertexCount);
  newForm.vertexCount = vertexCount;
  newForm.attribute   = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
  newForm.type        = FRMFPOLY;
  auto point          = thred::pxCor2stch(Msg.pt);
  auto angle          = 0.0F;
  auto itVertex       = wrap::next(FormVertices->begin(), newForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	*itVertex = point;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle += stepAngle;
	++itVertex;
  }
  newForm.outline();
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  StateMap->set(StateFlag::POLIMOV);
  setmfrm();
  StateMap->set(StateFlag::SHOFRM);
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

auto form::pdir(FRM_HEAD const& form, uint32_t vertex) -> uint32_t {
  if (StateMap->test(StateFlag::PSELDIR)) {
	return form::nxt(form, vertex);
  }

  return form::prv(form, vertex);
}

void form::pxrct2stch(RECT const& screenRect, F_RECTANGLE& stitchRect) noexcept {
  auto corner = POINT {screenRect.left + StitchWindowOrigin.x, screenRect.top + StitchWindowOrigin.y};
  auto stitchPoint  = thred::pxCor2stch(corner);
  stitchRect.left   = stitchPoint.x;
  stitchRect.top    = stitchPoint.y;
  corner.x          = screenRect.right + StitchWindowOrigin.x;
  corner.y          = screenRect.bottom + StitchWindowOrigin.y;
  stitchPoint       = thred::pxCor2stch(corner);
  stitchRect.right  = stitchPoint.x;
  stitchRect.bottom = stitchPoint.y;
}

void form::flipv() {
  if (StateMap->test(StateFlag::FPSEL)) {
	auto const& form = FormList->operator[](ClosestFormToCursor);

	auto const offset        = SelectedVerticesRect.top + SelectedVerticesRect.bottom;
	auto       currentVertex = SelectedFormVertices.start;
	auto const itFirstVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const itVertex = wrap::next(itFirstVertex, currentVertex);
	  itVertex->y         = offset - itVertex->y;
	  currentVertex       = pdir(form, currentVertex);
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	auto const offset = AllItemsRect->top + AllItemsRect->bottom;
	for (auto& formVertice : *FormVertices) {
	  formVertice.y = offset - formVertice.y;
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.y = offset - stitch.y;
	}
	for (auto& iForm : *FormList) {
	  iForm.rectangle.bottom = offset - iForm.rectangle.bottom;
	  iForm.rectangle.top    = offset - iForm.rectangle.top;
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (!SelectedFormList->empty()) {
	auto formMap   = boost::dynamic_bitset<> {FormList->size()};
	auto rectangle = F_RECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, rectangle);
	auto const offset = rectangle.top + rectangle.bottom;
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& iForm         = FormList->operator[](ClosestFormToCursor);
	  formMap.set(ClosestFormToCursor);
	  auto itVertex = wrap::next(FormVertices->begin(), iForm.vertexIndex);
	  for (auto iVertex = 0U; iVertex < iForm.vertexCount; ++iVertex) {
		itVertex->y = offset - itVertex->y;
		++itVertex;
	  }
	  iForm.outline();
	}
	for (auto& stitch : *StitchBuffer) {
	  if (auto const decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	      formMap.test(decodedForm) && ((stitch.attribute & NOTFRM) == 0U)) {
		stitch.y = offset - stitch.y;
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);

	  auto const offset   = form.rectangle.top + form.rectangle.bottom;
	  auto       itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		itVertex->y = offset - itVertex->y;
		++itVertex;
	  }
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor &&
		    ((stitch.attribute & NOTFRM) == 0U)) {
		  stitch.y = offset - stitch.y;
		}
	  }
	  form.outline();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto rectangle = F_RECTANGLE {};
		thred::selRct(rectangle);
		auto const offset = rectangle.top + rectangle.bottom;
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).y = offset - StitchBuffer->operator[](iStitch).y;
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::duform(int32_t formType) {
  switch (formType + 1) {
	case FRMLINE:
	  thred::savdo();
	  fi::setlin();
	  break;
	case FRMFPOLY:
	  thred::savdo();
	  fi::setpoli();
	  break;
	case FRMRPOLY:
	  fi::setrpoli();
	  break;
	case FRMSTAR:
	  fi::setstar();
	  break;
	case FRMSPIRAL:
	  fi::setspir();
	  break;
	case FRMHEART - 1:
	  fi::sethart();
	  break;
	case FRMLENS - 1:
	  fi::setlens();
	  break;
	case FRMEGG - 1:
	  fi::seteg();
	  break;
	case FRMTEAR - 1:
	  formForms::setear();
	  break;
	case FRMZIGZAG - 1:
	  fi::setzig();
	  break;
	case FRMWAVE - 1:
	  formForms::wavfrm();
	  break;
	case FRMDAISY - 1:
	  formForms::dasyfrm();
	  break;
	default:
	  // NOLINTNEXTLINE(clang-diagnostic-sign-conversion) ToDo - why is this neccessary?
	  outDebugString(L"default hit in duform: formType [{}]\n", formType);
	  break;
  }
}

auto fi::findDistanceToSide(F_POINT const& lineStart, F_POINT const& lineEnd, F_POINT const& point, float& distance) noexcept
    -> float {
  auto const varA = point.x - lineStart.x;
  auto const varB = point.y - lineStart.y;
  auto const varC = lineEnd.x - lineStart.x;
  auto const varD = lineEnd.y - lineStart.y;
  if ((varC == 0.0F) && (varD == 0.0F)) {
	distance = sqrt(varA * varB);
	// Arbitrarily choose the first point since start and end are the same
	return -0.1F;
  }
  auto const dot     = varA * varC + varB * varD;
  auto const lenSqrd = varC * varC + varD * varD;
  auto const param   = dot / lenSqrd;
  // param < 0 = before the first point
  // param > 1 = after last point
  // 0 < param < 1 = between endpoints
  auto const diff = (param < 0)   ? F_POINT {point.x - lineStart.x, point.y - lineStart.y}
                    : (param > 1) ? F_POINT {point.x - lineEnd.x, point.y - lineEnd.y}
                                  : F_POINT {point.x - (lineStart.x + param * varC),
                                             point.y - (lineStart.y + param * varD)};
  // returning shortest distance
  distance = sqrt(diff.x * diff.x + diff.y * diff.y);
  return param;
}

auto form::closfrm() -> bool {
  if (!FormList->empty()) {
	auto const screenCoordinate =
	    POINT {Msg.pt.x - StitchWindowOrigin.x, Msg.pt.y - StitchWindowOrigin.y};
	fi::rats();
	auto       closestForm   = 0U;
	auto       closestVertex = 0U;
	auto       minimumLength = BIGFLOAT;
	auto const point         = fi::px2stchf(screenCoordinate);
	auto const layerCoded    = gsl::narrow_cast<uint8_t>(ActiveLayer << 1U);
	auto const maxForm       = wrap::toUnsigned(FormList->size());
	for (auto iForm = 0U; iForm < maxForm; ++iForm) {
	  if (StateMap->test(StateFlag::FRMSAM) && iForm == ClosestFormToCursor) {
		continue;
	  }
	  auto& currentForm = FormList->operator[](iForm);
	  if (auto const formLayer = gsl::narrow_cast<uint8_t>(currentForm.attribute & FRMLMSK);
	      (ActiveLayer == 0U) || (formLayer == 0U) || formLayer == layerCoded) {
		auto const itFirstVertex = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex);
		auto       itVertex      = itFirstVertex;
		// find the closest line first and then find the closest vertex on that line
		auto       length    = 0.0F;
		auto const sideCount = currentForm.vertexCount;
		for (auto iVertex = 0U; iVertex < sideCount; ++iVertex) {
		  auto const itNextVertex = wrap::next(itFirstVertex, form::nxt(currentForm, iVertex));
		  auto const param        = fi::findDistanceToSide(*itVertex, *itNextVertex, point, length);
		  if ((length < minimumLength) && (length >= 0.0F)) {
			minimumLength = length;
			closestForm   = iForm;
			if (param < 0.5F) {
			  closestVertex = iVertex;
			}
			else {
			  closestVertex = form::nxt(currentForm, iVertex);
			}
		  }
		  ++itVertex;
		}
	  }
	}

	auto const& vertex =
	    FormVertices->operator[](wrap::toSize(FormList->operator[](closestForm).vertexIndex) + closestVertex);
	auto const stitchCoordsInPixels = thred::stch2pxr(vertex);

	minimumLength = std::hypot(wrap::toFloat(stitchCoordsInPixels.x - screenCoordinate.x),
	                           wrap::toFloat(stitchCoordsInPixels.y - screenCoordinate.y));
	if (minimumLength < FCLOSNUF) {
	  ClosestFormToCursor   = closestForm;
	  ClosestVertexToCursor = closestVertex;
	  StateMap->set(StateFlag::RELAYR);
	  return true;
	}
	return false;
  }
  return false;
}

void form::frmovlin() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  ratsr();
  if (form.type == FRMLINE) {
	NewFormVertexCount = form.vertexCount;
  }
  else {
	NewFormVertexCount = form.vertexCount + 1U;
  }
  form::frmlin(form);
  auto        previousPoint = form::prv(form, ClosestVertexToCursor);
  auto const& formLines     = *FormLines;
  for (auto iPoint = 0U; iPoint < 3U; ++iPoint) {
	RubberBandLine->operator[](iPoint) = formLines[previousPoint];
	previousPoint                      = form::nxt(form, previousPoint);
  }
  thred::ritmov(ClosestFormToCursor);
}

auto fi::ritlin(F_POINT const& start, F_POINT const& finish, float userStitchLen) -> bool {
  auto const delta  = F_POINT {finish.x - start.x, finish.y - start.y};
  auto       length = hypot(delta.x, delta.y);
  // This clamp is temporary to avoid overflow when BH corner value is too large. Find a better fix
  if (constexpr auto CLAMP = 200.0F; length > CLAMP) {
	length = CLAMP;
  }

  InterleaveSequence->push_back(start);
  if (length > MaxStitchLen) {
	constexpr auto MINSTLEN = 1e-1F; // clamp minimum stitch length
	if (userStitchLen < MINSTLEN) {
	  userStitchLen = MINSTLEN;
	}
	auto count = wrap::ceil<uint32_t>(length / userStitchLen);
	if (count == 0U) {
	  count = 1;
	}
	auto newLength = length / wrap::toFloat(count);
	while ((newLength > MaxStitchLen) && (newLength > MINSTLEN)) {
	  ++count;
	  newLength = length / wrap::toFloat(count);
	}
	auto const step  = F_POINT {delta.x / wrap::toFloat(count), delta.y / wrap::toFloat(count)};
	auto       point = F_POINT {start.x + step.x, start.y + step.y};
	for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	  InterleaveSequence->push_back(point);
	  point.x += step.x;
	  point.y += step.y;
	}
  }
  return true;
}

auto form::closflt(FRM_HEAD const& form, float xCoordinate, float yCoordinate) -> uint32_t {
  auto closestVertex = 0U;
  auto minimumLength = BIGFLOAT;
  auto itVertex      = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	if (auto const length = hypot(xCoordinate - itVertex->x, yCoordinate - itVertex->y); length < minimumLength) {
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

  for (auto val : *OSequence) {
	InterleaveSequence->push_back(val);
  }
#else
  auto const savedIndex = InterleaveSequence->size();
  auto& form            = FormList->operator[](ClosestFormToCursor);

  auto userStitchLen =
      border ? (form.edgeType == EDGELCHAIN || form.edgeType == EDGEOCHAIN) ? MAXSIZ * PFGRAN : form.edgeStitchLen
      : (form.isClip()) ? MaxStitchLen
                       : form.lengthOrCount.stitchLength;
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
  MaxStitchLen = border ? form.maxBorderStitchLen : form.maxFillStitchLen;
  if (userStitchLen > MaxStitchLen) {
	userStitchLen = MaxStitchLen;
  }
  if (!OSequence->empty()) {
	bool flag = true;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size()) - 1U; ++iSequence) {
	  if (!fi::ritlin(OSequence->operator[](iSequence), OSequence->operator[](wrap::toSize(iSequence) + 1U), userStitchLen)) {
		flag = false;
		break;
	  }
	}
	if (flag) {
	  InterleaveSequence->push_back(OSequence->back());
	}
  }
  else {
	outDebugString(L"chkseq: OSequence empty! border [{}]\n", border);
  }
  if (minimumStitchLength == 0.0F) {
	return;
  }
  auto destination = wrap::toUnsigned(savedIndex + 1U);
  for (auto iSequence = savedIndex + 1U; iSequence < InterleaveSequence->size(); ++iSequence) {
	auto const& seq      = InterleaveSequence->operator[](iSequence);
	auto const& seqBack1 = InterleaveSequence->operator[](iSequence - 1U);
	if (auto const len = hypot(seq.x - seqBack1.x, seq.y - seqBack1.y); len > minimumStitchLength) {
	  InterleaveSequence->operator[](destination) = seq;
	  ++destination;
	}
  }
  InterleaveSequence->resize(destination);
#endif
}

void fi::ritbrd(FRM_HEAD const& form) {
  if (!OSequence->empty()) {
	InterleaveSequenceIndices->emplace_back(INS_REC {TYPBRD,
	                                                 gsl::narrow_cast<uint32_t>(form.borderColor) & COLMSK,
	                                                 wrap::toUnsigned(InterleaveSequence->size()),
	                                                 I_BRD});
	form::chkseq(true);
  }
}

void fi::ritapbrd() {
  if (!OSequence->empty()) {
	auto const& form = FormList->operator[](ClosestFormToCursor);
	InterleaveSequenceIndices->emplace_back(INS_REC {TYPMSK,
	                                                 gsl::narrow_cast<uint32_t>(form.borderColor) >> 4U,
	                                                 wrap::toUnsigned(InterleaveSequence->size()),
	                                                 I_AP});
	form::chkseq(true);
  }
}

void fi::ritfil() {
  if (!OSequence->empty()) {
	auto const& form = FormList->operator[](ClosestFormToCursor);
	InterleaveSequenceIndices->emplace_back(INS_REC {
	    TYPFRM, gsl::narrow_cast<uint32_t>(form.fillColor), wrap::toUnsigned(InterleaveSequence->size()), I_FIL});
	form::chkseq(false);
  }
}

auto form::lastch() noexcept -> bool {
  if (!InterleaveSequence->empty()) {
	LastPoint = InterleaveSequence->back();
	return true;
  }
  { return false; }
}

auto form::getlast(FRM_HEAD const& form) -> uint32_t {
  if (form.fillType != 0U) {
	form::lastch();
	auto minimumLength = BIGFLOAT;
	auto closestVertex = 0U;
	auto itVertex      = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  if (auto const length = hypot(LastPoint.x - itVertex->x, LastPoint.y - itVertex->y); length < minimumLength) {
		minimumLength = length;
		closestVertex = iVertex;
	  }
	  ++itVertex;
	}
	return closestVertex;
  }
  { return 0; }
}

void form::filinsb(F_POINT const& point, F_POINT& stitchPoint) {
  constexpr auto MAXSTCH = 54.0F; // maximum permitted stitch length for pfaf in pfaf "stitch pixels"
  auto const delta  = F_POINT {(point.x - stitchPoint.x), (point.y - stitchPoint.y)};
  auto const length = hypot(delta.x, delta.y);
  auto       count  = wrap::round<uint32_t>(length / MAXSTCH + 1.0F);
  auto const step   = F_POINT {(delta.x / wrap::toFloat(count)), (delta.y / wrap::toFloat(count))};
  if (length > MAXSTCH) {
	--count;
	if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
	  return;
	}
	while (count != 0U) {
	  stitchPoint.x += step.x;
	  stitchPoint.y += step.y;
	  OSequence->push_back(stitchPoint);
	  --count;
	}
  }
  OSequence->push_back(point);
  stitchPoint.x = point.x;
  stitchPoint.y = point.y;
}

auto form::getblen() noexcept -> float {
  // clang-format off
  auto const& form    = FormList->operator[](ClosestFormToCursor);
  auto const  iLength = (form.clipEntries << WRDSHFT) | form.picoLength;
  // clang-format on
  return wrap::toFloat(iLength);
}

void form::savblen(float fLength) {
  auto const iLength = wrap::round<uint32_t>(fLength);
  auto& form         = FormList->operator[](ClosestFormToCursor);
  form.clipEntries   = iLength >> WRDSHFT;
  form.picoLength    = iLength & WRDMASK;
}

auto form::getplen() noexcept -> float {
  // clang-format off
  auto const& form  = FormList->operator[](ClosestFormToCursor);
  auto const  value = form.picoLength;
  // clang-format on
  return (wrap::toFloat(value >> BYTSHFT) + wrap::toFloat(value & BYTMASK) / FRACFACT);
}

void form::savplen(float length) {
  auto integerPart = 0.0F;

  if (constexpr auto PCLAMP = 255.0F; length > PCLAMP) {
	length = PCLAMP;
  }
  auto const fractionalPart = std::modf(length, &integerPart);
  auto const frByte         = wrap::floor<uint16_t>(fractionalPart * FRACFACT);
  auto const num            = gsl::narrow<uint32_t>(integerPart);
  FormList-> operator[](ClosestFormToCursor).picoLength = ((num << BYTSHFT) & B2MASK) | frByte;
}

void fi::bdrlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float stitchSize) {
  auto const itStartVertex  = wrap::next(FormVertices->cbegin(), vertexIndex + start);
  auto const itFinishVertex = wrap::next(FormVertices->cbegin(), vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length      = hypot(delta.x, delta.y);
  auto       stitchCount = (UserFlagMap->test(UserFlag::LINSPAC))
                               ? wrap::ceil<uint32_t>(length / stitchSize)
                               : wrap::round<uint32_t>((length - stitchSize / 2.0F) / stitchSize + 1.0F);
  auto       step        = F_POINT {};
  if (UserFlagMap->test(UserFlag::LINSPAC)) {
	if (stitchCount != 0U) {
	  step.x = delta.x / wrap::toFloat(stitchCount);
	  step.y = delta.y / wrap::toFloat(stitchCount);
	}
  }
  else {
	// ToDo - Is this calculation correct?
	auto const angle = atan2(delta.y, delta.x);
	step.x           = cos(angle) * stitchSize;
	step.y           = sin(angle) * stitchSize;
  }
  if (stitchCount != 0U) {
	auto point = F_POINT {itStartVertex->x + step.x, itStartVertex->y + step.y};
	--stitchCount;
	while (stitchCount != 0U) {
	  OSequence->push_back(point);
	  point.x += step.x;
	  point.y += step.y;
	  --stitchCount;
	}
  }
  OSequence->push_back(*itFinishVertex);
}

void fi::brdfil(FRM_HEAD const& form) {
  auto currentVertex = ((form.extendedAttribute & AT_STRT) != 0U) ? form.fillStart : form::getlast(form);
  OSequence->clear();
  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + currentVertex);
  OSequence->push_back(*itVertex);
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

void fi::boldlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float size) {
  auto const itStartVertex  = wrap::next(FormVertices->cbegin(), vertexIndex + start);
  auto const itFinishVertex = wrap::next(FormVertices->cbegin(), vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length = hypot(delta.x, delta.y);

  if (constexpr auto ESCLAMP = 1e-1F; // edge stitch minimum length clamp
      size < ESCLAMP) {
	size = ESCLAMP;
  }
  if (auto count = wrap::round<uint32_t>(length / size); count != 0U) {
	auto const step   = F_POINT {delta.x / wrap::toFloat(count), delta.y / wrap::toFloat(count)};
	auto       point0 = *itStartVertex;
	auto       point1 = F_POINT {point0.x + step.x, point0.y + step.y};
	while (count != 0U) {
	  OSequence->push_back(point1);
	  OSequence->push_back(point0);
	  OSequence->push_back(point1);
	  point0.x += step.x;
	  point0.y += step.y;
	  point1.x += step.x;
	  point1.y += step.y;
	  --count;
	}
	OSequence->push_back(*itFinishVertex);
  }
  else {
	OSequence->push_back(*itFinishVertex);
	OSequence->push_back(*itStartVertex);
	OSequence->push_back(*itFinishVertex);
  }
}

void fi::bold(FRM_HEAD const& form) {
  auto iOutput   = 0U;
  auto iNextLine = form::getlast(form);
  auto iLine     = iNextLine;
  OSequence->clear();
  auto const itLineVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + iLine);
  OSequence->push_back(*itLineVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	iNextLine = form::nxt(form, iLine);
	boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
	iLine = iNextLine;
  }
  if (form.type != FRMLINE) {
	iNextLine = form::nxt(form, iLine);
	boldlin(form.vertexIndex, iLine, iNextLine, form.edgeStitchLen);
  }
  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size() - 1U); ++iSequence) {
	auto const& sequence     = OSequence->operator[](iSequence);
	auto const& sequenceFwd1 = OSequence->operator[](wrap::toSize(iSequence) + 1U);
	if (auto const length = hypot(sequenceFwd1.x - sequence.x, sequenceFwd1.y - sequence.y); length > TNYFLOAT) {
	  OSequence->operator[](iOutput++) = sequence;
	}
  }
  auto const itNextVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + iNextLine);
  OSequence->operator[](iOutput++) = *itNextVertex;
  OSequence->resize(iOutput);
}

void form::duangs(FRM_HEAD const& form) {
  FormAngles->clear();
  auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
	itVertex = wrap::next(AngledFormVertices->cbegin(), form.vertexIndex);
  }
  auto const vMax = gsl::narrow<ptrdiff_t>(form.vertexCount - 1U);
  for (auto iVertex = ptrdiff_t {}; iVertex < vMax; ++iVertex) {
	auto const& thisVertex = itVertex[iVertex];
	auto const& nextVertex = itVertex[iVertex + 1];
	FormAngles->push_back(atan2(nextVertex.y - thisVertex.y, nextVertex.x - thisVertex.x));
  }
  FormAngles->push_back(atan2(itVertex[0].y - itVertex[vMax].y, itVertex[0].x - itVertex[vMax].x));
}

// find the intersection of a line defined by it's endpoints and a vertical line defined by it's x coordinate
auto fi::projv(float xCoordinate, F_POINT const& lowerPoint, F_POINT const& upperPoint, F_POINT& intersection) noexcept
    -> bool {
  auto const deltaX = upperPoint.x - lowerPoint.x;
  intersection.x    = xCoordinate;
  if (deltaX != 0.0F) {
	auto const slope = (upperPoint.y - lowerPoint.y) / deltaX;
	intersection.y   = (xCoordinate - lowerPoint.x) * slope + lowerPoint.y;
	auto lower       = lowerPoint.x;
	auto upper       = upperPoint.x;
	if (lower > upper) {
	  std::swap(lower, upper);
	}
	return xCoordinate >= lower && xCoordinate <= upper;
  }
  return false;
}

auto form::cisin(FRM_HEAD const& form, float xCoordinate, float yCoordinate) -> bool {
  auto const& rectangle = form.rectangle;
  if (xCoordinate >= rectangle.right) {
	return false;
  }
  if (xCoordinate <= rectangle.left) {
	return false;
  }
  if (yCoordinate >= rectangle.top) {
	return false;
  }
  if (yCoordinate <= rectangle.bottom) {
	return false;
  }
  auto count        = 0U;
  auto intersection = F_POINT {};
  auto itVertex     = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  if ((form.type == FRMLINE) && ((form.edgeType & NEGUND) == EDGEPROPSAT)) {
	itVertex = wrap::next(AngledFormVertices->cbegin(), form.vertexIndex);
  }
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const itThisVertex = wrap::next(itVertex, iVertex);
	if (auto const itNextVertex = wrap::next(itVertex, nxt(form, iVertex));
	    fi::projv(xCoordinate, *itThisVertex, *itNextVertex, intersection)) {
	  if (intersection.y >= yCoordinate) {
		if (!util::closeEnough(itThisVertex->x, xCoordinate) && !util::closeEnough(itNextVertex->x, xCoordinate)) {
		  ++count;
		}
		else {
		  if (itThisVertex->x < itNextVertex->x) {
			if (!util::closeEnough(itNextVertex->x, xCoordinate)) {
			  ++count;
			}
		  }
		  else {
			if (!util::closeEnough(itThisVertex->x, xCoordinate)) {
			  ++count;
			}
		  }
		}
	  }
	}
  }
  return (count & 1U) != 0U;
}

/* find the intersection of two lines, one defined by point and slope, the other by the coordinates
   of the endpoints. */
auto fi::proj(F_POINT const& point, float slope, F_POINT const& point0, F_POINT const& point1, F_POINT& intersectionPoint) noexcept
    -> bool {
  auto const delta     = D_POINT {point1.x - point0.x, point1.y - point0.y};
  auto       intersect = D_POINT {intersectionPoint};
  if (delta.x != 0.0) {
	auto const sideSlope    = delta.y / delta.x;
	auto const sideConstant = wrap::toDouble(point0.y) - sideSlope * wrap::toDouble(point0.x);
	auto const pointConstant =
	    wrap::toDouble(point.y) - wrap::toDouble(slope) * wrap::toDouble(point.x);
	intersect.x = (sideConstant - pointConstant) / (wrap::toDouble(slope) - sideSlope);
	intersect.y = intersect.x * wrap::toDouble(slope) + pointConstant;
  }
  else {
	intersect.x = wrap::toDouble(point0.x);
	auto const pointConstant =
	    wrap::toDouble(point.y) - wrap::toDouble(slope) * wrap::toDouble(point.x);
	intersect.y = intersect.x * wrap::toDouble(slope) + pointConstant;
  }
  auto xMinimum = wrap::toDouble(point0.x);
  auto xMaximum = wrap::toDouble(point1.x);
  if (xMinimum > xMaximum) {
	std::swap(xMinimum, xMaximum);
  }
  if (delta.y != 0.0) {
	auto yMinimum = wrap::toDouble(point0.y);
	auto yMaximum = wrap::toDouble(point1.y);
	if (yMinimum > yMaximum) {
	  std::swap(yMinimum, yMaximum);
	}
	intersectionPoint = intersect;
	return intersect.x >= xMinimum && intersect.x <= xMaximum && intersect.y >= yMinimum &&
	       intersect.y <= yMaximum;
  }
  intersectionPoint = intersect;
  return intersect.x >= xMinimum && intersect.x <= xMaximum;
}

auto form::linx(std::vector<F_POINT> const& points, uint32_t start, uint32_t finish, F_POINT& intersection) noexcept
    -> bool {
  if (OutsidePoints != nullptr) {
	auto const  delta = F_POINT {(OutsidePoints->operator[](start).x - points[start].x),
                                (OutsidePoints->operator[](start).y - points[start].y)};
	auto const& point = points[start];

	if ((delta.x == 0.0F) && (delta.y == 0.0F)) {
	  return false;
	}
	if (delta.x != 0.0F) {
	  return fi::proj(point, delta.y / delta.x, OutsidePoints->operator[](finish), points[finish], intersection);
	}

	return fi::projv(point.x, points[finish], OutsidePoints->operator[](finish), intersection);
  }

  return false;
}

// find the intersection of a line defined by it's endpoints and a horizontal line defined by it's y coordinate
auto fi::projh(float yCoordinate, F_POINT const& point0, F_POINT const& point1, F_POINT& intersection) noexcept
    -> bool {
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

void fi::sprct(std::vector<F_POINT> const& vertices,
               uint32_t                    vertexIndex,
               std::vector<V_RECT_2>&      fillVerticalRect,
               uint32_t                    start,
               uint32_t                    finish) {
  auto const& opStart      = OutsidePoints->operator[](start);
  auto const& opFinish     = OutsidePoints->operator[](finish);
  auto const& ipStart      = InsidePoints->operator[](start);
  auto const& ipFinish     = InsidePoints->operator[](finish);
  auto const delta         = F_POINT {(opFinish.x - opStart.x), (opFinish.y - opStart.y)};
  auto const itStartVertex = wrap::next(vertices.cbegin(), vertexIndex + start);
  if (auto const itFinishVertex = wrap::next(vertices.cbegin(), vertexIndex + finish);
      (delta.x != 0.0F) && (delta.y != 0.0F)) {
	auto const slope        = -delta.x / delta.y;
	auto       point        = *itFinishVertex;
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
  }
  else {
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
	}
	else {
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
  }
}

void fi::spurfn(F_POINT const& innerPoint,
                F_POINT const& outerPoint,
                F_POINT&       underlayInnerPoint,
                F_POINT&       underlayOuterPoint) noexcept {
  auto const     delta  = F_POINT {outerPoint.x - innerPoint.x, outerPoint.y - innerPoint.y};
  constexpr auto DIURAT = (1.0F - URAT) / 2.0F;        //(1-URAT)/2
  constexpr auto DOURAT = (1.0F - URAT) / 2.0F + URAT; //(1-URAT)/2+URAT

  underlayInnerPoint.x = delta.x * DIURAT + innerPoint.x;
  underlayInnerPoint.y = delta.y * DIURAT + innerPoint.y;
  underlayOuterPoint.x = delta.x * DOURAT + innerPoint.x;
  underlayOuterPoint.y = delta.y * DOURAT + innerPoint.y;
}

void fi::spurct(std::vector<V_RECT_2>&       underlayVerticalRect,
                std::vector<V_RECT_2> const& fillVerticalRect,
                uint32_t                     iRect) noexcept {
  auto const& fRect  = fillVerticalRect[iRect];
  auto&       ulRect = underlayVerticalRect[iRect];
  spurfn(fRect.aipnt, fRect.aopnt, ulRect.aipnt, ulRect.aopnt);
  spurfn(fRect.bipnt, fRect.bopnt, ulRect.bipnt, ulRect.bopnt);
  spurfn(fRect.cipnt, fRect.copnt, ulRect.cipnt, ulRect.copnt);
  spurfn(fRect.dipnt, fRect.dopnt, ulRect.dipnt, ulRect.dopnt);
}

void fi::duromb(F_POINT const& start0, F_POINT const& finish0, F_POINT const& start1, F_POINT const& finish1, F_POINT& stitchPoint) {
  if (!StateMap->test(StateFlag::UND)) {
	auto const length0 = hypot(stitchPoint.x - start0.x, stitchPoint.y - start0.y);
	if (auto const length1 = hypot(stitchPoint.x - start1.x, stitchPoint.y - start1.y); length0 > length1) {
	  StateMap->set(StateFlag::FILDIR);
	}
	else {
	  StateMap->reset(StateFlag::FILDIR);
	}
  }
  auto const delta0  = F_POINT {finish0.x - start0.x, finish0.y - start0.y};
  auto const delta1  = F_POINT {finish1.x - start1.x, finish1.y - start1.y};
  auto const length0 = hypot(delta0.x, delta0.y);
  auto       count   = wrap::round<uint32_t>(length0 / (LineSpacing / 2.0F));
  if (count == 0U) {
	++count;
  }
  auto const factor  = wrap::toFloat(count);
  auto const step0   = F_POINT {delta0.x / factor, delta0.y / factor};
  auto const step1   = F_POINT {delta1.x / factor, delta1.y / factor};
  auto       start00 = start0;
  auto       start11 = start1;
  for (auto iStep = 0U; iStep < count; ++iStep) {
	if (StateMap->testAndFlip(StateFlag::FILDIR)) {
	  form::filinsb(start00, stitchPoint);
	}
	else {
	  form::filinsb(start11, stitchPoint);
	}
	start00.x += step0.x;
	start00.y += step0.y;
	start11.x += step1.x;
	start11.y += step1.y;
  }
}

auto form::psg() noexcept -> uint32_t {
  if (PseudoRandomValue == 0U) {
	PseudoRandomValue = SEED;
  }
  auto const temp = PseudoRandomValue & 0x48000000U;
  PseudoRandomValue <<= 1U;
  if (temp == 0x40000000U || temp == 0x8000000U) {
	++PseudoRandomValue;
  }
  return PseudoRandomValue;
}

void fi::fillSB(const F_POINT& pivot, float angle, float const& radius, F_POINT& stitchPoint, float const& level) {
  auto const outerPoint = F_POINT {pivot.x + cos(angle) * radius, pivot.y + sin(angle) * radius};
  form::filinsb(outerPoint, stitchPoint);
  auto const innerRadius = radius * level * 0.4F;
  auto const innerPoint = F_POINT {pivot.x + cos(angle) * innerRadius, pivot.y + sin(angle) * innerRadius};
  form::filinsb(innerPoint, stitchPoint);
}

void fi::spend(std::vector<V_RECT_2> const& fillVerticalRect, uint32_t start, uint32_t finish, F_POINT& stitchPoint) {
  // clang-format off
  static constexpr auto LEVEL00 = std::array<float,  1U>{ 0.0F };
  static constexpr auto LEVEL01 = std::array<float,  1U>{ 1.0F };
  static constexpr auto LEVEL02 = std::array<float,  2U>{ 0.0F/2.0F , 1.0F/2.0F  };
  static constexpr auto LEVEL03 = std::array<float,  3U>{ 1.0F/3.0F , 0.0F/3.0F , 2.0F/3.0F  };
  static constexpr auto LEVEL04 = std::array<float,  4U>{ 1.0F/4.0F , 3.0F/4.0F , 0.0F/4.0F , 2.0F/4.0F  };
  static constexpr auto LEVEL05 = std::array<float,  5U>{ 2.0F/5.0F , 0.0F/5.0F , 3.0F/5.0F , 1.0F/5.0F , 4.0F/5.0F  };
  static constexpr auto LEVEL06 = std::array<float,  6U>{ 2.0F/6.0F , 3.0F/6.0F , 1.0F/6.0F , 4.0F/6.0F , 0.0F/6.0F , 5.0F/6.0F  };
  static constexpr auto LEVEL07 = std::array<float,  7U>{ 3.0F/7.0F , 2.0F/7.0F , 4.0F/7.0F , 1.0F/7.0F , 5.0F/7.0F , 0.0F/7.0F ,  6.0F/7.0F  };
  static constexpr auto LEVEL08 = std::array<float,  8U>{ 4.0F/8.0F , 3.0F/8.0F , 5.0F/8.0F , 2.0F/8.0F , 6.0F/8.0F , 1.0F/8.0F ,  7.0F/8.0F , 0.0F/8.0F  };
  static constexpr auto LEVEL09 = std::array<float,  9U>{ 4.0F/9.0F , 3.0F/9.0F , 5.0F/9.0F , 2.0F/9.0F , 6.0F/9.0F , 1.0F/9.0F ,  7.0F/9.0F , 0.0F/9.0F ,  8.0F/9.0F  };
  static constexpr auto LEVEL10 = std::array<float, 10U>{ 5.0F/10.0F, 4.0F/10.0F, 6.0F/10.0F, 3.0F/10.0F, 7.0F/10.0F, 2.0F/10.0F,  8.0F/10.0F, 1.0F/10.0F,  9.0F/10.0F,  0.0F/10.0F };
  static constexpr auto LEVEL11 = std::array<float, 11U>{ 5.0F/11.0F, 4.0F/11.0F, 6.0F/11.0F, 3.0F/11.0F, 7.0F/11.0F, 2.0F/11.0F,  8.0F/11.0F, 1.0F/11.0F,  9.0F/11.0F,  0.0F/11.0F, 10.0F/11.0F };
  static constexpr auto LEVEL12 = std::array<float, 12U>{ 6.0F/12.0F, 5.0F/12.0F, 7.0F/12.0F, 4.0F/12.0F, 8.0F/12.0F, 3.0F/12.0F,  9.0F/12.0F, 2.0F/12.0F, 10.0F/12.0F,  1.0F/12.0F, 11.0F/12.0F,  0.0F/12.0F};
  static constexpr auto LEVEL13 = std::array<float, 13U>{ 6.0F/13.0F, 5.0F/13.0F, 7.0F/13.0F, 4.0F/13.0F, 8.0F/13.0F, 3.0F/13.0F,  9.0F/13.0F, 2.0F/13.0F, 10.0F/13.0F,  1.0F/13.0F, 11.0F/13.0F,  0.0F/13.0F, 12.0F/13.0F};
  static constexpr auto LEVEL14 = std::array<float, 14U>{ 7.0F/14.0F, 6.0F/14.0F, 8.0F/14.0F, 5.0F/14.0F, 9.0F/14.0F, 4.0F/14.0F, 10.0F/14.0F, 3.0F/14.0F, 11.0F/14.0F,  2.0F/14.0F, 12.0F/14.0F,  1.0F/14.0F, 13.0F/14.0F,  0.0F/14.0F };
  static constexpr auto LEVEL15 = std::array<float, 15U>{ 7.0F/15.0F, 6.0F/15.0F, 8.0F/15.0F, 5.0F/15.0F, 9.0F/15.0F, 4.0F/15.0F, 10.0F/15.0F, 3.0F/15.0F, 11.0F/15.0F,  2.0F/15.0F, 12.0F/15.0F,  1.0F/15.0F, 13.0F/15.0F,  0.0F/15.0F, 14.0F/15.0F};
  // clang-format on

  static auto levels = std::array<float const*, 16> {LEVEL00.data(),
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
  auto const innerLength = hypot(innerDelta.x, innerDelta.y);
  auto const outerDelta = F_POINT {(fillVerticalRect[finish].copnt.x - fillVerticalRect[start].bopnt.x),
                                   (fillVerticalRect[finish].copnt.y - fillVerticalRect[start].bopnt.y)};
  auto const outerLength = hypot(outerDelta.x, outerDelta.y);
  auto const flag        = (outerLength > innerLength);
  auto const pivot       = flag ? fillVerticalRect[start].cipnt : fillVerticalRect[start].copnt;

  auto const startDelta  = flag ? F_POINT {fillVerticalRect[start].copnt.x - pivot.x,
                                          fillVerticalRect[start].copnt.y - pivot.y}
                                : F_POINT {fillVerticalRect[start].cipnt.x - pivot.x,
                                          fillVerticalRect[start].cipnt.y - pivot.y};
  auto const finishDelta = flag ? F_POINT {fillVerticalRect[finish].bopnt.x - pivot.x,
                                           fillVerticalRect[finish].bopnt.y - pivot.y}
                                : F_POINT {fillVerticalRect[finish].bipnt.x - pivot.x,
                                           fillVerticalRect[finish].bipnt.y - pivot.y};
  if (hypot(stitchPoint.x - pivot.x, stitchPoint.y - pivot.y) > PI_F2) {
	form::filinsb(pivot, stitchPoint);
  }
  auto       startAngle  = atan2(startDelta.y, startDelta.x);
  auto const finishAngle = atan2(finishDelta.y, finishDelta.x);
  auto       deltaAngle  = finishAngle - startAngle;
  if (deltaAngle > PI_F) {
	deltaAngle -= PI_F2;
  }
  if (deltaAngle < -PI_F) {
	deltaAngle += PI_F2;
  }
  auto const radius    = hypot(startDelta.x, startDelta.y);
  auto const arc       = fabs(radius * deltaAngle);
  auto       count     = wrap::round<uint32_t>(arc / LineSpacing);
  auto const stepAngle = deltaAngle / wrap::toFloat(count);
  if (count == 0U) {
	count = 1U;
  }
  if (count > 15U) {
	for (auto iCount = 0U; iCount < count; ++iCount) {
	  auto const level = wrap::toFloat(form::psg() % count) / wrap::toFloat(count);
	  fi::fillSB(pivot, startAngle, radius, stitchPoint, level);
	  startAngle += stepAngle;
	}
  }
  else {
	auto const spLevels = gsl::span(levels.operator[](count), count);
	for (auto const& level : spLevels) {
	  fi::fillSB(pivot, startAngle, radius, stitchPoint, level);
	  startAngle += stepAngle;
	}
  }
}

void fi::duspnd(float                        stitchLen,
                std::vector<V_RECT_2> const& underlayVerticalRect,
                std::vector<V_RECT_2> const& fillVerticalRect,
                uint32_t                     start,
                uint32_t                     finish,
                float                        width,
                F_POINT&                     stitchPoint) {
  if (StateMap->test(StateFlag::UND)) {
	if (StateMap->test(StateFlag::UNDPHAS)) {
	  form::filinsb(underlayVerticalRect[start].copnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[start].cipnt, stitchPoint);
	  auto const delta =
	      F_POINT {underlayVerticalRect[finish].bipnt.x - underlayVerticalRect[start].cipnt.x,
	               underlayVerticalRect[finish].bipnt.y - underlayVerticalRect[start].cipnt.y};
	  if (auto const length = hypot(delta.x, delta.y); length > stitchLen) {
		auto const angle =
		    atan2(InsidePoints->operator[](finish).y - OutsidePoints->operator[](finish).y,
		          InsidePoints->operator[](finish).x - OutsidePoints->operator[](finish).x);
		auto const point = F_POINT {underlayVerticalRect[finish].bopnt.x + cos(angle) * width,
		                            underlayVerticalRect[finish].bopnt.y + sin(angle) * width};
		form::filinsb(point, stitchPoint);
	  }
	  form::filinsb(underlayVerticalRect[finish].bipnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[finish].bopnt, stitchPoint);
	}
	else {
	  form::filinsb(underlayVerticalRect[start].cipnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[start].copnt, stitchPoint);
	  auto const delta =
	      F_POINT {underlayVerticalRect[finish].bopnt.x - underlayVerticalRect[start].copnt.x,
	               underlayVerticalRect[finish].bopnt.y - underlayVerticalRect[start].copnt.y};
	  if (auto const length = hypot(delta.x, delta.y); length > stitchLen) {
		auto const angle =
		    atan2(OutsidePoints->operator[](finish).y - InsidePoints->operator[](finish).y,
		          OutsidePoints->operator[](finish).x - InsidePoints->operator[](finish).x);
		auto const point = F_POINT {underlayVerticalRect[finish].bipnt.x + cos(angle) * width,
		                            underlayVerticalRect[finish].bipnt.y + sin(angle) * width};
		form::filinsb(point, stitchPoint);
	  }
	  form::filinsb(underlayVerticalRect[finish].bopnt, stitchPoint);
	  form::filinsb(underlayVerticalRect[finish].bipnt, stitchPoint);
	}
  }
  else {
	spend(fillVerticalRect, start, finish, stitchPoint);
  }
}

void fi::pfn(std::vector<V_RECT_2> const& underlayVerticalRect,
             std::vector<V_RECT_2> const& fillVerticalRect,
             uint32_t                     startVertex,
             std::vector<V_RECT_2> const& vrct,
             float                        width) {
  // clang-format off
  auto const& form          = FormList->operator[](ClosestFormToCursor);
  auto        currentVertex = startVertex;
  auto        nextVertex    = form::nxt(form, currentVertex);
  auto const  itVertex      = wrap::next(FormVertices->cbegin(), form.vertexIndex + startVertex);
  auto        stitchPoint   = *itVertex;
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

void fi::prsmal(float width) {
  constexpr auto UFSFACT = 0.8F; // set the minimum to 80% of the underlay fill spacing

  auto iOutput       = 0U;
  auto minimumLength = USPAC * UFSFACT;
  if (minimumLength > width) {
	constexpr auto WIDFACT = 0.9F; // or 90% of the actual width
	minimumLength          = width * WIDFACT;
  }
  auto iReference = 0U;
  for (auto iSequence = 1U; iSequence < wrap::toUnsigned(OSequence->size()); ++iSequence) {
	auto const& seq    = OSequence->operator[](iSequence);
	auto const& seqRef = OSequence->operator[](iReference);
	if (auto const length = hypot(seq.x - seqRef.x, seq.y - seqRef.y); length > minimumLength) {
	  OSequence->operator[](iOutput++) = seq;
	  iReference                       = iSequence;
	}
  }
  OSequence->resize(iOutput);
}

void fi::pbrd(FRM_HEAD const& form) {
  auto const spacing = LineSpacing;
  auto const start   = form::getlast(form);
  LineSpacing        = form.edgeSpacing;
  OSequence->clear();
  auto fillVerticalRect = std::vector<V_RECT_2> {};
  fillVerticalRect.resize(form.vertexCount);
  auto underlayVerticalRect = std::vector<V_RECT_2> {};
  underlayVerticalRect.resize(form.vertexCount);
  satin::satout(form, form.borderSize);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	sprct(*FormVertices, form.vertexIndex, fillVerticalRect, iVertex, iVertex + 1U);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
  }
  sprct(*FormVertices, form.vertexIndex, fillVerticalRect, (form.vertexCount - 1U), 0U);
  spurct(underlayVerticalRect, fillVerticalRect, (form.vertexCount - 1U));
  if ((form.edgeType & EGUND) != 0U) {
	StateMap->reset(StateFlag::SAT1);
	LineSpacing = USPAC;
	StateMap->set(StateFlag::UND);
	auto const width = form.borderSize * URAT;
	satin::satout(form, width);
	StateMap->set(StateFlag::UNDPHAS);
	StateMap->set(StateFlag::FILDIR);
	pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
	StateMap->reset(StateFlag::UNDPHAS);
	StateMap->reset(StateFlag::FILDIR);
	pfn(underlayVerticalRect, fillVerticalRect, start, underlayVerticalRect, width);
	LineSpacing = form.edgeSpacing;
	prsmal(width);
	StateMap->reset(StateFlag::UND);
  }
  pfn(underlayVerticalRect, fillVerticalRect, start, fillVerticalRect, form.borderSize);
  LineSpacing = spacing;
}

void fi::apbrd(FRM_HEAD const& form) {
  auto currentVertex = 0U;
  OSequence->clear();
  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  OSequence->push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount * 2U; ++iVertex) {
	auto const nextVertex = form::nxt(form, currentVertex);
	bdrlin(form.vertexIndex, currentVertex, nextVertex, IniFile.AppStitchLen);
	currentVertex = nextVertex;
  }
}

void fi::bhfn(FRM_HEAD const& form, uint32_t start, uint32_t finish) {
  auto const itStartVertex  = wrap::next(FormVertices->cbegin(), form.vertexIndex + start);
  auto const itFinishVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + finish);
  auto const delta =
      F_POINT {(itFinishVertex->x - itStartVertex->x), (itFinishVertex->y - itStartVertex->y)};
  auto const length = hypot(delta.x, delta.y);
  auto const count  = wrap::round<uint32_t>(length / form.edgeSpacing);
  auto const step   = F_POINT {(delta.x / wrap::toFloat(count)), (delta.y / wrap::toFloat(count))};
  auto       innerPoint    = F_POINT {itStartVertex->x, itStartVertex->y};
  auto const rotationAngle = atan2(-delta.x, delta.y);
  auto const outerStep = F_POINT {form.borderSize * cos(rotationAngle), form.borderSize * sin(rotationAngle)};
  if (count != 0U) {
	for (auto iStep = 0U; iStep < count - 1U; ++iStep) {
	  auto const firstPoint = F_POINT {innerPoint.x + step.x, innerPoint.y + step.y};
	  auto const outerPoint = F_POINT {firstPoint.x + outerStep.x, firstPoint.y + outerStep.y};
	  OSequence->push_back(F_POINT {firstPoint});
	  OSequence->push_back(F_POINT {innerPoint});
	  OSequence->push_back(F_POINT {firstPoint});
	  OSequence->push_back(F_POINT {outerPoint});
	  OSequence->push_back(F_POINT {firstPoint});
	  OSequence->push_back(F_POINT {outerPoint});
	  OSequence->push_back(F_POINT {firstPoint});
	  innerPoint.x += step.x;
	  innerPoint.y += step.y;
	}
	auto const firstPoint = F_POINT {innerPoint.x + step.x, innerPoint.y + step.y};
	OSequence->push_back(F_POINT {firstPoint});
	OSequence->push_back(F_POINT {innerPoint});
  }
}

void fi::bhcrnr(FRM_HEAD const& form, uint32_t vertex) {
  auto const nextVertex = form::nxt(form, vertex);
  auto const itVertex   = wrap::next(FormVertices->cbegin(), form.vertexIndex + nextVertex);
  if (auto const* ptr = (StateMap->test(StateFlag::INDIR)) ? OutsidePoints : InsidePoints; nullptr != ptr) {
	auto delta = F_POINT {ptr->operator[](nextVertex).x - itVertex->x,
	                      ptr->operator[](nextVertex).y - itVertex->y};

	auto const length = hypot(delta.x, delta.y);
	auto const ratio  = ButtonholeCornerLength / length;
	delta.x *= ratio;
	delta.y *= ratio;
	auto const point = F_POINT {itVertex->x + delta.x, itVertex->y + delta.y};
	OSequence->push_back(*itVertex);
	OSequence->push_back(F_POINT {point});
	OSequence->push_back(*itVertex);
	OSequence->push_back(F_POINT {point});
	OSequence->push_back(*itVertex);
  }
}

void fi::bhbrd(FRM_HEAD const& form) {
  auto vertex = form::getlast(form);
  OSequence->clear();
  auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex + vertex);
  OSequence->push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	auto const nextVertex = form::nxt(form, vertex);
	bhfn(form, vertex, nextVertex);
	bhcrnr(form, vertex);
	vertex = nextVertex;
  }
}

void fi::dubfn(FRM_HEAD const& currentForm) {
  brdfil(currentForm);
  for (auto iBackward = OSequence->size(); iBackward != 0; --iBackward) {
	OSequence->push_back(OSequence->operator[](iBackward - 1U));
  }
}

void fi::chkbrd(FRM_HEAD const& form) {
  if (form.edgeType != 0U) {
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
		StateMap->set(StateFlag::LINCHN);
		clip::chnfn(form);
		break;
	  }
	  case EDGEOCHAIN: { // Open Chain
		StateMap->reset(StateFlag::LINCHN);
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
}

void fi::fnvrt(std::vector<F_POINT>&    currentFillVertices,
               std::vector<uint32_t>&   groupIndexSequence,
               std::vector<SMAL_PNT_L>& lineEndpoints) {
  auto const [min, max] = std::minmax_element(
      currentFillVertices.begin(),
      currentFillVertices.end(),
      [](F_POINT const& first, F_POINT const& second) { return first.x < second.x; });
  auto const highX           = max->x;
  auto const lineOffset      = std::floor(min->x / LineSpacing);
  auto const lowX            = LineSpacing * lineOffset;
  auto       fillLineCount   = wrap::floor<uint32_t>((highX - lowX) / LineSpacing + 1.0F);
  auto const step            = (highX - lowX) / wrap::toFloat(fillLineCount);
  auto       currentX        = lowX;
  auto       projectedPoints = std::vector<F_POINT_LINE> {};
  projectedPoints.reserve(currentFillVertices.size() + 2U);
  auto const currentVertexCount = wrap::toUnsigned(currentFillVertices.size());
  for (auto iLine = 0U; iLine < fillLineCount; ++iLine) {
	auto iLineCounter = 0U;
	currentX += step;
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  auto const iNextVertex = (iVertex + 1U) % currentVertexCount;
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
	auto iPoint = 0U;
	for (auto iVertex = uint16_t {}; iVertex < currentVertexCount; ++iVertex) {
	  auto const iNextVertex = (iVertex + 1U) % currentVertexCount;
	  if (auto point = F_POINT {};
	      projv(currentX, currentFillVertices[iVertex], currentFillVertices[iNextVertex], point)) {
		auto const projected = F_POINT_LINE {point.x, point.y, iVertex};
		projectedPoints.push_back(projected);
		++iPoint;
	  }
	}
	if (iPoint > 1) {
	  auto const evenPointCount = (iPoint & 0xfffffffe);
	  groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
	  std::ranges::sort(projectedPoints, fi::fplComp);
	  iPoint                    = 0;
	  auto const savedLineCount = lineEndpoints.size();
	  while (iPoint < evenPointCount) {
		if (lineEndpoints.size() < fillLineCount) {
		  lineEndpoints.push_back(SMAL_PNT_L {projectedPoints[iPoint].line,
		                                      lineGroupIndex,
		                                      projectedPoints[iPoint].x,
		                                      projectedPoints[iPoint].y});
		  ++iPoint;
		  lineEndpoints.push_back(SMAL_PNT_L {projectedPoints[iPoint].line,
		                                      lineGroupIndex,
		                                      projectedPoints[iPoint].x,
		                                      projectedPoints[iPoint].y});
		  ++iPoint;
		}
	  }
	  if (lineEndpoints.size() != savedLineCount) {
		++lineGroupIndex;
	  }
	}
  }
  groupIndexSequence.push_back(wrap::toUnsigned(lineEndpoints.size()));
}

void fi::fnang(std::vector<uint32_t>&   groupIndexSequence,
               std::vector<SMAL_PNT_L>& lineEndpoints,
               float                    rotationAngle,
               F_POINT&                 rotationCenter,
               FRM_HEAD&                angledForm,
               std::vector<F_POINT>&    angledFormVertices) {
  angledForm = FormList->operator[](ClosestFormToCursor);

  auto const& angRect = angledForm.rectangle;
  rotationCenter = F_POINT {wrap::midl(angRect.right, angRect.left), wrap::midl(angRect.top, angRect.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto itVertex = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*itVertex);
	++itVertex;
	thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
  }
  angledForm.vertexIndex = 0;
  fi::fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void fi::fnhor(std::vector<uint32_t>&   groupIndexSequence,
               std::vector<SMAL_PNT_L>& lineEndpoints,
               float const              rotationAngle,
               F_POINT&                 rotationCenter,
               FRM_HEAD&                angledForm,
               std::vector<F_POINT>&    angledFormVertices) {
  angledForm = FormList->operator[](ClosestFormToCursor);

  auto const& angRect = angledForm.rectangle;
  rotationCenter = F_POINT {wrap::midl(angRect.right, angRect.left), wrap::midl(angRect.top, angRect.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto itVertex = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*itVertex);
	thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	++itVertex;
  }
  angledForm.vertexIndex = 0;
  fi::fnvrt(angledFormVertices, groupIndexSequence, lineEndpoints);
}

void fi::prebrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices) {
  constexpr auto REDFACT = 0.1F; // reduction factor

  auto const itVertex     = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  auto const itNextVertex = std::next(itVertex);
  auto       delta = F_POINT {(itNextVertex->x - itVertex->x), (itNextVertex->y - itVertex->y)};
  angledFormVertices.resize(wrap::toSize(form.vertexCount) + 3U);
  auto output = angledFormVertices.begin();
  ++output;
  std::copy(itVertex, wrap::next(itVertex, form.vertexCount), output);
  auto ratio = (fabs(delta.x) > fabs(delta.y)) ? fabs(REDFACT / delta.x) : fabs(REDFACT / delta.y);
  angledFormVertices[0].x = itVertex->x - delta.x * ratio;
  angledFormVertices[0].y = itVertex->y - delta.y * ratio;
  angledForm              = FormList->operator[](ClosestFormToCursor);
  angledForm.vertexIndex  = 0;
  angledForm.vertexCount += 3;
  auto const itLastVertex     = wrap::next(itVertex, form.vertexCount - 1U);
  auto const itPreviousVertex = wrap::next(itLastVertex, -1);
  delta.x                     = itLastVertex->x - itPreviousVertex->x;
  delta.y                     = itLastVertex->y - itPreviousVertex->y;
  ratio = (fabs(delta.x) > fabs(delta.y)) ? fabs(REDFACT / delta.x) : fabs(REDFACT / delta.y);
  angledFormVertices[angledForm.vertexCount - 1U].x = itLastVertex->x + delta.x * ratio;
  angledFormVertices[angledForm.vertexCount - 1U].y = itLastVertex->y + delta.y * ratio;
}

void fi::plfn(FRM_HEAD const&              form,
              std::vector<V_RECT_2> const& underlayVerticalRect,
              std::vector<V_RECT_2> const& fillVerticalRect,
              std::vector<V_RECT_2> const& prct,
              float                        width,
              F_POINT&                     stitchPoint) {
  duromb(prct[1].aipnt, prct[1].cipnt, prct[1].aopnt, prct[1].copnt, stitchPoint);
  duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, 1, 2, width, stitchPoint);
  for (auto iVertex = 2U; iVertex < form.vertexCount - 4U; ++iVertex) {
	duromb(prct[iVertex].bipnt, prct[iVertex].cipnt, prct[iVertex].bopnt, prct[iVertex].copnt, stitchPoint);
	duspnd(form.edgeStitchLen, underlayVerticalRect, fillVerticalRect, iVertex, iVertex + 1U, width, stitchPoint);
  }
  duromb(prct[form.vertexCount - 4U].bipnt,
         prct[form.vertexCount - 4U].dipnt,
         prct[form.vertexCount - 4U].bopnt,
         prct[form.vertexCount - 4U].dopnt,
         stitchPoint);
}

void fi::plbak(uint32_t backPoint) {
  if ((!OSequence->empty()) && (backPoint < (OSequence->size() - 1U))) {
	auto iSequence = wrap::toUnsigned(OSequence->size() - 1U);
	while (iSequence > backPoint) {
	  std::swap(OSequence->operator[](iSequence), OSequence->operator[](backPoint));
	  --iSequence;
	  ++backPoint;
	}
  }
}

void fi::plbrd(FRM_HEAD const& form, FRM_HEAD& angledForm, std::vector<F_POINT>& angledFormVertices) {
  // Ensure that we have at least 4 array members
  auto fillVerticalRect = std::vector<V_RECT_2> {};
  fillVerticalRect.resize(wrap::toSize(form.vertexCount) + 3U);
  auto underlayVerticalRect = std::vector<V_RECT_2> {};
  underlayVerticalRect.resize(wrap::toSize(form.vertexCount) + 3U);
  prebrd(form, angledForm, angledFormVertices);
  satin::satout(angledForm, angledForm.borderSize);
  InsidePoints->push_back(InsidePoints->front());
  OutsidePoints->push_back(OutsidePoints->front());
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount - 1U; ++iVertex) {
	sprct(*AngledFormVertices, 0, fillVerticalRect, iVertex, iVertex + 1U);
	spurct(underlayVerticalRect, fillVerticalRect, iVertex);
  }
  sprct(*AngledFormVertices, 0U, fillVerticalRect, (angledForm.vertexCount - 1U), 0U);
  spurct(underlayVerticalRect, fillVerticalRect, (angledForm.vertexCount - 1U));
  auto const itVertex = wrap::next(angledFormVertices.cbegin(), angledForm.vertexIndex);
  if ((angledForm.attribute & SBLNT) == 0U) {
	auto const val = std::next(itVertex, 1);

	fillVerticalRect[1].aipnt     = *val;
	fillVerticalRect[1].aopnt     = *val;
	underlayVerticalRect[1].aipnt = *val;
	underlayVerticalRect[1].aopnt = *val;
  }
  if ((angledForm.attribute & FBLNT) == 0U) {
	auto const itLastVertex = wrap::next(itVertex, angledForm.vertexCount - 1U);

	fillVerticalRect[angledForm.vertexCount - 4U].dipnt     = *itLastVertex;
	fillVerticalRect[angledForm.vertexCount - 4U].dopnt     = *itLastVertex;
	underlayVerticalRect[angledForm.vertexCount - 4U].dipnt = *itLastVertex;
	underlayVerticalRect[angledForm.vertexCount - 4U].dopnt = *itLastVertex;
  }
  OSequence->clear();
  auto stitchPoint = *itVertex;
  if ((angledForm.edgeType & EGUND) != 0U) {
	LineSpacing = USPAC;
	StateMap->set(StateFlag::UND);
	auto const width = angledForm.borderSize * URAT;
	StateMap->set(StateFlag::UNDPHAS);
	StateMap->reset(StateFlag::FILDIR);
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width, stitchPoint);
	auto const savedIndex = wrap::toUnsigned(OSequence->size());
	StateMap->reset(StateFlag::UNDPHAS);
	stitchPoint = *itVertex;
	StateMap->set(StateFlag::FILDIR);
	plfn(angledForm, underlayVerticalRect, fillVerticalRect, underlayVerticalRect, width, stitchPoint);
	plbak(savedIndex);
	prsmal(width);
	if (!OSequence->empty()) { // ensure that we can do a valid read from OSequence
	  stitchPoint = OSequence->back();
	}
  }
  StateMap->reset(StateFlag::UND);
  LineSpacing = angledForm.edgeSpacing;
  plfn(angledForm, underlayVerticalRect, fillVerticalRect, fillVerticalRect, angledForm.borderSize, stitchPoint);
  LineSpacing = form.edgeSpacing;
}

void fi::lapbrd(FRM_HEAD const& form) {
  auto const savedStitchLength = UserStitchLength;
  OSequence->clear();
  UserStitchLength = IniFile.AppStitchLen;
  for (auto iVertex = 0U; iVertex < form.vertexCount - 1U; ++iVertex) {
	bdrlin(form.vertexIndex, iVertex, iVertex + 1U, IniFile.AppStitchLen);
  }
  for (auto iVertex = form.vertexCount - 1U; iVertex != 0U; --iVertex) {
	bdrlin(form.vertexIndex, iVertex, iVertex - 1U, IniFile.AppStitchLen);
  }
  UserStitchLength = savedStitchLength;
}

void fi::blbrd(FRM_HEAD const& form) {
  OSequence->clear();
  auto itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
  OSequence->push_back(*itVertex);
  for (auto iVertex = 0U; iVertex < form.vertexCount - 2U; ++iVertex) {
	bhfn(form, iVertex, iVertex + 1U);
	bhcrnr(form, iVertex);
  }
  bhfn(form, form.vertexCount - 2U, form.vertexCount - 1U);
  itVertex = wrap::next(itVertex, form.vertexCount - 1U);
  OSequence->push_back(*itVertex);
}

void fi::contf(FRM_HEAD& form) {
  class P_VEC
  {
public:
	float angle {};
	float length {};

	// constexpr P_VEC() noexcept = default;
	// P_VEC(P_VEC const&) = default;
	// P_VEC(P_VEC&&) = default;
	// P_VEC& operator=(P_VEC const& rhs) = default;
	// P_VEC& operator=(P_VEC&&) = default;
	//~P_VEC() = default;
  };

  auto const start = form.angleOrClipData.guide.start;
  // ToDo - Find a better way to avoid crashing than clamping it if start is after finish
  if (auto const finish = form.angleOrClipData.guide.finish; start < finish) {
	auto       itFirstVertex    = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	auto const itSelectedVertex = wrap::next(itFirstVertex, start);
	auto const lowVertexIndex   = start;
	auto const highVertexIndex  = form.vertexCount - start - 1U;
	auto       lowCounts        = std::vector<uint32_t> {};
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
	OSequence->clear();
	auto lowIndex = 0U;
	{
	  auto itVertex = wrap::next(itFirstVertex, lowVertexIndex);
	  for (auto iVertex = lowVertexIndex; iVertex != 0; --iVertex) {
		lowVertices[lowIndex] = *itVertex;
		auto itPreviousVertex = wrap::next(itVertex, -1);
		lowDeltas[lowIndex] =
		    F_POINT {itPreviousVertex->x - itVertex->x, itPreviousVertex->y - itVertex->y};
		lowLengths[lowIndex] = hypot(lowDeltas[lowIndex].x, lowDeltas[lowIndex].y);
		lowLength += lowLengths[lowIndex];
		++lowIndex;
		--itVertex;
	  }
	}
	auto const selectedVertexCount = finish - start;
	auto       polyLines           = std::vector<P_VEC> {};
	polyLines.resize(selectedVertexCount);
	{
	  auto selind   = 0U;
	  auto itVertex = wrap::next(itFirstVertex, start + 1);
	  for (auto iVertex = start + 1U; iVertex <= finish; ++iVertex) {
		auto const delta =
		    F_POINT {itVertex->x - itSelectedVertex->x, itVertex->y - itSelectedVertex->y};
		polyLines[selind] = P_VEC {atan2(delta.y, delta.x), hypot(delta.x, delta.y)};
		++selind;
		++itVertex;
	  }
	}
	auto highIndex  = 0U;
	auto highLength = 0.0F;
	{
	  auto itFinishVertex = wrap::next(itFirstVertex, finish);
	  auto itNextVertex   = wrap::next(itFinishVertex, 1);
	  for (auto iVertex = finish; iVertex < form.vertexCount - 1U; ++iVertex) {
		highVertices[highIndex] = *itFinishVertex;
		highDeltas[highIndex] =
		    F_POINT {itNextVertex->x - itFinishVertex->x, itNextVertex->y - itFinishVertex->y};
		highLengths[highIndex] = hypot(highDeltas[highIndex].x, highDeltas[highIndex].y);
		highLength += highLengths[highIndex];
		++highIndex;
		++itFinishVertex;
		++itNextVertex;
	  }
	}
	auto lowSpacing  = form.fillSpacing;
	auto highSpacing = lowSpacing;
	if (highLength < lowLength) {
	  highSpacing = form.fillSpacing * highLength / lowLength;
	}
	else {
	  lowSpacing = form.fillSpacing * lowLength / highLength;
	}
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
	lowIndex = highIndex = 0;
	StateMap->reset(StateFlag::FILDIR);
	auto lowCount       = 0U;
	auto highCount      = 0U;
	auto itFinishVertex = wrap::next(itFirstVertex, finish);
	auto itStartVertex  = wrap::next(itFirstVertex, start);
	auto delta = F_POINT {itFinishVertex->x - itStartVertex->x, itFinishVertex->y - itStartVertex->y};
	auto const reference = P_VEC {atan2(delta.y, delta.x), hypot(delta.x, delta.y)};
	auto       lowStep   = F_POINT {};
	auto       lowPoint  = F_POINT {};
	auto       highStep  = F_POINT {};
	auto       highPoint = F_POINT {};
	while ((lowCount != 0U) || (lowIndex < lowVertexIndex && highIndex < highVertexIndex)) {
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

	  if (constexpr auto REFFACT = 0.9F; // reduction factor for the reference
	      reference.length > REFFACT * LineSpacing) {
		auto const polyLine = P_VEC {atan2(delta.y, delta.x), hypot(delta.x, delta.y)};
		if (auto const polyDiff =
		        P_VEC {polyLine.angle - reference.angle, polyLine.length / reference.length};
		    StateMap->testAndFlip(StateFlag::FILDIR)) {
		  OSequence->push_back(F_POINT {lowPoint});
		  for (auto iVertex = 0U; iVertex < (selectedVertexCount - 1); ++iVertex) {
			auto const length = polyLines[iVertex].length * polyDiff.length;
			auto const angle  = polyLines[iVertex].angle + polyDiff.angle;
			OSequence->push_back(F_POINT {lowPoint.x + cos(angle) * length, lowPoint.y + sin(angle) * length});
		  }
		}
		else {
		  OSequence->push_back(F_POINT {highPoint});
		  for (auto iVertex = selectedVertexCount - 1U; iVertex != 0; --iVertex) {
			auto const length = polyLines[iVertex - 1U].length * polyDiff.length;
			auto const angle  = polyLines[iVertex - 1U].angle + polyDiff.angle;
			OSequence->push_back(F_POINT {lowPoint.x + cos(angle) * length, lowPoint.y + sin(angle) * length});
		  }
		}
	  }
	  lowPoint.x += lowStep.x;
	  lowPoint.y += lowStep.y;
	  highPoint.x += highStep.x;
	  highPoint.y += highStep.y;
	}
	if (StateMap->test(StateFlag::FILDIR)) {
	  OSequence->push_back(*itFirstVertex);
	}
	else {
	  auto itLastVertex = wrap::next(itFirstVertex, form.vertexCount - 1U);
	  OSequence->push_back(*itLastVertex);
	}
	if (form.lengthOrCount.stitchLength < MinStitchLength) {
	  form.lengthOrCount.stitchLength = MinStitchLength;
	}
  }
}

void fi::duflt(float& formOffset, std::vector<F_POINT>& currentFormVertices) {
  if (auto const leftEdge = (std::ranges::min_element(currentFormVertices,
                                                      [](F_POINT const& first, F_POINT const& second) {
	                                                    return first.x < second.x;
                                                      }))
                                ->x;
      leftEdge < ClipRectSize.cx) {
	StateMap->set(StateFlag::WASNEG);
	constexpr auto SAFOFFST = 0.01F; // factor to ensure that formOffset is not 0
	formOffset              = ClipRectSize.cx + fabs(leftEdge) + SAFOFFST;
	for (auto& vertex : currentFormVertices) {
	  vertex.x += formOffset;
	}
	auto& form = FormList->operator[](ClosestFormToCursor);
	form.rectangle.left += formOffset;
	form.rectangle.right += formOffset;
  }
  else {
	StateMap->reset(StateFlag::WASNEG);
  }
}

auto fi::leftsid(std::vector<F_POINT> const& currentFormVertices) noexcept -> uint32_t {
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

auto fi::vclpComp(V_CLP_X const& vclpx1, V_CLP_X const& vclpx2) noexcept -> bool {
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

auto fi::isin(FRM_HEAD const&             form,
              std::vector<V_CLP_X> const& regionCrossingData,
              float                       xCoordinate,
              float                       yCoordinate,
              uint32_t                    regionCrossingStart,
              uint32_t                    regionCrossingEnd,
              F_RECTANGLE const&          boundingRect,
              std::vector<F_POINT> const& currentFormVertices) -> bool {
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
	if (auto const itEndVertex = wrap::next(currentFormVertices.cbegin(), form::nxt(form, iVertex));
	    projv(xCoordinate, *itStartVertex, *itEndVertex, point)) {
	  if (point.y > yCoordinate) {
		if (!util::closeEnough(itStartVertex->x, xCoordinate) && !util::closeEnough(itEndVertex->x, xCoordinate)) {
		  ++count;
		}
		else {
		  if (itStartVertex->x < itEndVertex->x) {
			if (!util::closeEnough(itEndVertex->x, xCoordinate)) {
			  ++count;
			}
		  }
		  else {
			if (!util::closeEnough(itStartVertex->x, xCoordinate)) {
			  ++count;
			}
		  }
		}
	  }
	}
  }
  return (count & 1U) != 0U;
}

void fi::inspnt(std::vector<CLIP_PNT>& clipStitchPoints) {
  auto const clipStitchPoint = CLIP_PNT {clipStitchPoints.back().x, clipStitchPoints.back().y, 0, 0};
  clipStitchPoints.back().x =
      wrap::midl(clipStitchPoint.x, clipStitchPoints[clipStitchPoints.size() - 2U].x);
  clipStitchPoints.back().y =
      wrap::midl(clipStitchPoint.y, clipStitchPoints[clipStitchPoints.size() - 2U].y);
  clipStitchPoints.back().flag = 1;
  clipStitchPoints.push_back(clipStitchPoint);
}

auto fi::isect(uint32_t                    vertex0,
               uint32_t                    vertex1,
               F_POINT&                    intersection,
               float&                      length,
               F_POINT const&              lineSegmentStart,
               F_POINT const&              lineSegmentEnd,
               std::vector<F_POINT> const& currentFormVertices) -> bool {
  auto const delta =
      F_POINT {(lineSegmentEnd.x - lineSegmentStart.x), (lineSegmentEnd.y - lineSegmentStart.y)};
  auto       tempIntersection = F_POINT {};
  auto       flag             = false;
  auto const itVertexZero     = wrap::next(currentFormVertices.cbegin(), vertex0);
  if (auto const itVertexOne = wrap::next(currentFormVertices.cbegin(), vertex1);
      (delta.x != 0.0F) && (delta.y != 0.0F)) {
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
  if (tempIntersection.x < TNYFLOAT) {
	tempIntersection.x = 0.0F;
  }
  if (tempIntersection.y < TNYFLOAT) {
	tempIntersection.y = 0.0F;
  }
  intersection = tempIntersection;
  length = hypot(tempIntersection.x - lineSegmentStart.x, tempIntersection.y - lineSegmentStart.y);
  // ToDo - should length be determined from start or end?
  //	 hypot(tempIntersection.x-lineSegmentEnd.x,tempIntersection.y-lineSegmentEnd.y);
  return flag;
}

auto fi::clipComp(const gsl::not_null<CLIP_SORT const*> arg1,
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

void fi::mvpclp(std::vector<CLIP_SORT*>& arrayOfClipIntersectData, uint32_t destination, uint32_t source) noexcept {
  if (destination != source) {
	arrayOfClipIntersectData[destination] = arrayOfClipIntersectData[source];
  }
}

auto fi::insect(FRM_HEAD const&             form,
                std::vector<CLIP_SORT>&     clipIntersectData,
                std::vector<V_CLP_X> const& regionCrossingData,
                std::vector<CLIP_SORT*>&    arrayOfClipIntersectData,
                uint32_t                    regionCrossingStart,
                uint32_t                    regionCrossingEnd,
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
	  auto const hFlag  = (lineSegmentEnd.x > lineSegmentStart.x);
	  auto const vFlag  = (lineSegmentEnd.y > lineSegmentStart.y);
	  auto const left   = hFlag ? lineSegmentStart.x : lineSegmentEnd.x;
	  auto const right  = hFlag ? lineSegmentEnd.x : lineSegmentStart.x;
	  auto const top    = vFlag ? lineSegmentEnd.y : lineSegmentStart.y;
	  auto const bottom = vFlag ? lineSegmentStart.y : lineSegmentEnd.y;
	  if (auto const& intersection = clipIntersectData[iIntersection].point;
	      intersection.x >= left && intersection.x <= right && intersection.y >= bottom &&
	      intersection.y <= top) {
		clipIntersectData[iIntersection].segmentLength =
		    hypot(clipIntersectData[iIntersection].point.x - lineSegmentStart.x,
		          clipIntersectData[iIntersection].point.y - lineSegmentStart.y);
		clipIntersectData[iIntersection].vertexIndex = currentVertex;
		arrayOfClipIntersectData.push_back(&clipIntersectData[iIntersection]);
		++iIntersection;
		++count;
	  }
	}
  }
  if (count > 1) {
	std::ranges::sort(arrayOfClipIntersectData, fi::clipComp);
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

auto fi::getlen(std::vector<CLIP_PNT>&      clipStitchPoints,
                std::vector<float> const&   lengths,
                uint32_t                    iPoint,
                std::vector<F_POINT> const& currentFormVertices) -> float {
  clipStitchPoints[iPoint].vertexIndex %= currentFormVertices.size();
  auto const itVertex = wrap::next(currentFormVertices.cbegin(), clipStitchPoints[iPoint].vertexIndex);
  return lengths[clipStitchPoints[iPoint].vertexIndex] +
         hypot(itVertex->x - clipStitchPoints[iPoint].x, itVertex->y - clipStitchPoints[iPoint].y);
}

auto fi::clpnseg(std::vector<CLIP_PNT>&      clipStitchPoints,
                 std::vector<CLIP_SEG>&      clipSegments,
                 std::vector<float> const&   lengths,
                 uint32_t                    start,
                 uint32_t                    finish,
                 std::vector<F_POINT> const& currentFormVertices) -> uint32_t {
  auto const clipSegment = CLIP_SEG {start,
                                     getlen(clipStitchPoints, lengths, start, currentFormVertices),
                                     0U,
                                     clipStitchPoints[start].vertexIndex,
                                     finish,
                                     getlen(clipStitchPoints, lengths, finish, currentFormVertices),
                                     0U,
                                     clipStitchPoints[finish].vertexIndex,
                                     0};
  clipSegments.push_back(clipSegment);
  return finish + 1U;
}

auto fi::lenComp(LEN_INFO const& arg1, LEN_INFO const& arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
  if (arg1.length < arg2.length) {
	return true;
  }
  if (arg2.length < arg1.length) {
	return false;
  }

  return false;
}

void fi::chksid(FRM_HEAD const&             form,
                uint32_t                    vertexIndex,
                uint32_t                    clipIntersectSide,
                std::vector<F_POINT> const& currentFormVertices) {
  if (clipIntersectSide != vertexIndex) {
	auto const itVertex = currentFormVertices.cbegin();
	if (auto const vertexCount = currentFormVertices.size();
	    (vertexIndex - clipIntersectSide + vertexCount) % vertexCount < (vertexCount / 2)) {
	  auto       iVertex = form::nxt(form, clipIntersectSide);
	  auto const limit   = form::nxt(form, vertexIndex);
	  while (iVertex != limit) {
		auto const itThisVertex = wrap::next(itVertex, iVertex);
		OSequence->push_back(*itThisVertex);
		iVertex = form::nxt(form, iVertex);
	  }
	}
	else {
	  auto iVertex = clipIntersectSide;
	  while (iVertex != vertexIndex) {
		auto const itThisVertex = wrap::next(itVertex, iVertex);
		OSequence->push_back(*itThisVertex);
		iVertex = form::prv(form, iVertex);
	  }
	}
  }
}

void fi::ritseg(FRM_HEAD const&              form,
                std::vector<CLIP_PNT> const& clipStitchPoints,
                std::vector<CLIP_SEG>&       clipSegments,
                uint32_t const               currentSegmentIndex,
                uint32_t&                    clipIntersectSide,
                std::vector<F_POINT> const&  currentFormVertices) {
  auto isPointedEnd = true;
  if ((form.extendedAttribute & AT_SQR) != 0U) {
	isPointedEnd = false;
  }
  if (StateMap->test(StateFlag::FILDIR)) {
	auto iPoint = clipSegments[currentSegmentIndex].start;
	if (StateMap->test(StateFlag::TXFIL) && isPointedEnd) {
	  ++iPoint;
	}
	chksid(form, clipSegments[currentSegmentIndex].asid, clipIntersectSide, currentFormVertices);
	while (iPoint <= clipSegments[currentSegmentIndex].finish) {
	  OSequence->push_back(F_POINT {clipStitchPoints[iPoint].x, clipStitchPoints[iPoint++].y});
	}
	clipIntersectSide = clipSegments[currentSegmentIndex].zsid;
  }
  else {
	auto iPoint = clipSegments[currentSegmentIndex].finish;
	if (StateMap->test(StateFlag::TXFIL) && isPointedEnd) {
	  --iPoint;
	}
	chksid(form, clipSegments[currentSegmentIndex].zsid, clipIntersectSide, currentFormVertices);
	if (clipSegments[currentSegmentIndex].start != 0U) {
	  while (iPoint >= clipSegments[currentSegmentIndex].start) {
		OSequence->push_back(F_POINT {clipStitchPoints[iPoint].x, clipStitchPoints[iPoint--].y});
	  }
	}
	else {
	  // while (iPoint < clipSegments[currentSegmentIndex].start) { // This line makes no sense. Maybe the replacement does?
	  while (iPoint-- != 0U) {
		OSequence->push_back(F_POINT {clipStitchPoints[iPoint].x, clipStitchPoints[iPoint].y});
	  }
	}
	clipIntersectSide = clipSegments[currentSegmentIndex].asid;
  }
  clipSegments[currentSegmentIndex].dun = 1;
}

auto fi::clpnxt(std::vector<CLIP_SEG> const& clipSegments,
                std::vector<LEN_INFO> const& sortedLengths,
                uint32_t                     sind,
                uint32_t&                    outIndex) -> bool {
  auto       index        = 1U;
  auto const indexDoubled = wrap::toUnsigned(clipSegments.size()) * 2U;
  StateMap->reset(StateFlag::FILDIR);
  while (index < clipSegments.size()) {
	if (StateMap->testAndFlip(StateFlag::FILDIR)) {
	  outIndex = (sind + index) % indexDoubled;
	  if (clipSegments[sortedLengths[outIndex].index].dun == 0) {
		return false;
	  }
	  ++index;
	}
	else {
	  outIndex = (sind + indexDoubled - index) % indexDoubled;
	  if (clipSegments[sortedLengths[outIndex].index].dun == 0) {
		return false;
	  }
	}
  }
  return true;
}

auto fi::nucseg(std::vector<CLIP_SEG> const& clipSegments,
                std::vector<LEN_INFO> const& sortedLengths,
                uint32_t&                    currentSegmentIndex) -> bool {
  auto const index = (StateMap->test(StateFlag::FILDIR)) ? clipSegments[currentSegmentIndex].endIndex
                                                         : clipSegments[currentSegmentIndex].beginIndex;
  auto outIndex = 0U;
  if (clpnxt(clipSegments, sortedLengths, index, outIndex)) {
	return false;
  }
  if (sortedLengths[outIndex].isEnd) {
	StateMap->reset(StateFlag::FILDIR);
  }
  else {
	StateMap->set(StateFlag::FILDIR);
  }
  currentSegmentIndex = sortedLengths[outIndex].index;
  return true;
}

auto fi::vscmp(uint32_t index1, uint32_t index2) noexcept -> bool {
  return util::closeEnough(OSequence->operator[](index1).x, OSequence->operator[](index2).x)
             ? !util::closeEnough(OSequence->operator[](index1).y, OSequence->operator[](index2).y)
             : true;
}

void fi::clpcon(FRM_HEAD& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& currentFormVertices) {
  auto const currentVertexCount = currentFormVertices.size();
  auto       formOffset         = 0.0F;
  duflt(formOffset, currentFormVertices);
  auto clipWidth = ClipRectSize.cx + form.fillSpacing;
  if (StateMap->test(StateFlag::ISUND)) {
	clipWidth = form.underlaySpacing;
  }
  auto clipNegative = false;
  if (form.fillSpacing < 0) {
	clipNegative = true;
  }
  if (clipWidth < CLPMINVT) {
	clipWidth = CLPMINVT;
  }
  if (StateMap->test(StateFlag::TXFIL)) {
	if ((!TexturePointsBuffer->empty()) &&
	    form.fillInfo.texture.index + form.fillInfo.texture.count <=
	        gsl::narrow<decltype(form.fillInfo.texture.index)>(TexturePointsBuffer->size())) {
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
	clipSideLengths[vertex] = hypot(itNextVertex->x - itVertex->x, itNextVertex->y - itVertex->y);
	totalLength += clipSideLengths[vertex];
	vertex = nextVertex;
  }
  auto itVertex     = itFirstVertex;
  auto boundingRect = F_RECTANGLE {itVertex->x, itVertex->y, itVertex->x, itVertex->y};
  ++itVertex;
  for (auto iVertex = 1U; iVertex < currentVertexCount; ++iVertex) {
	if (itVertex->x < boundingRect.left) {
	  boundingRect.left = itVertex->x;
	}
	if (itVertex->y > boundingRect.top) {
	  boundingRect.top = itVertex->y;
	}
	if (itVertex->x > boundingRect.right) {
	  boundingRect.right = itVertex->x;
	}
	if (itVertex->y < boundingRect.bottom) {
	  boundingRect.bottom = itVertex->y;
	}
	++itVertex;
  }
  auto clipGrid = RECT {wrap::floor<int32_t>(boundingRect.left / clipWidth),
                        wrap::ceil<int32_t>(boundingRect.top / ClipRectSize.cy + 1.0F) + 2,
                        wrap::ceil<int32_t>(boundingRect.right / clipWidth),
                        wrap::floor<int32_t>(boundingRect.bottom / ClipRectSize.cy - 1.0F)};

  auto negativeOffset = 0L;
  auto clipGridOffset = 0U;
  if (form.wordParam > 1) {
	clipGridOffset = form.wordParam;
  }
  if (clipGridOffset != 0U) {
	++(clipGrid.top);
	if (form.fillSpacing < 0) {
	  --(clipGrid.bottom);
	  clipGrid.left -= std::lround(ClipRectSize.cx / clipWidth);
	  clipGrid.right += std::lround(ClipRectSize.cx / clipWidth);
	}
  }
  if (clipNegative && (clipGridOffset == 0U)) {
	clipGrid.left -= std::lround(ClipRectSize.cx / clipWidth);
  }
  if (clipGrid.bottom < 0) {
	negativeOffset = 1 - clipGrid.bottom;
	clipGrid.bottom += negativeOffset;
	clipGrid.top += negativeOffset;
	auto const formNegativeOffset = ClipRectSize.cy * wrap::toFloat(negativeOffset);
	auto       vertIt             = itFirstVertex;
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vertIt->y += formNegativeOffset;
	  ++vertIt;
	}
	boundingRect.top += formNegativeOffset;
	boundingRect.bottom += formNegativeOffset;
  }
  auto regionCrossingData = std::vector<V_CLP_X> {}; // region crossing data for vertical clipboard fills
  // reserve a little more than we need. Determined empirically
  regionCrossingData.reserve(wrap::toSize(clipGrid.right - clipGrid.left) * 5U);
  auto vCurr = itFirstVertex;
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
	if (auto const right = gsl::narrow<uint32_t>(clipGrid.right); finish > right) {
	  finish = right;
	}
	if (clipNegative) {
	  start -= wrap::round<uint32_t>(ClipRectSize.cx / clipWidth);
	}
	for (auto iSegment = start; iSegment <= finish; ++iSegment) {
	  regionCrossingData.push_back(V_CLP_X {iSegment, iVertex});
	}
  }
  std::ranges::sort(regionCrossingData, fi::vclpComp);
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
  clipStitchPoints.reserve(1000U);
  auto pasteLocation = F_POINT {};
  auto texture       = TexturePointsBuffer->begin();
  if (auto iclpxSize = wrap::toUnsigned(iclpx.size()); iclpxSize != 0U) {
	--iclpxSize;
	bool breakFlag = false;
	for (auto iRegion = 0U; iRegion < iclpxSize; ++iRegion) {
	  auto regionCrossingStart = iclpx[iRegion];
	  auto regionCrossingEnd   = iclpx[wrap::toSize(iRegion) + 1U];
	  pasteLocation.x         = clipWidth * (wrap::toFloat(iRegion) + wrap::toFloat(clipGrid.left));
	  auto clipVerticalOffset = 0.0F;
	  auto lineSegmentStart   = F_POINT {}; // vertical clipboard line segment start
	  auto lineSegmentEnd     = F_POINT {}; // vertical clipboard line segment end
	  auto clipStitchCount    = wrap::toUnsigned(ClipBuffer->size());
	  if (StateMap->test(StateFlag::TXFIL)) {
		auto const textureLine =
		    (iRegion + wrap::toUnsigned(clipGrid.left)) % wrap::toUnsigned(form.fillInfo.texture.lines);
		clipStitchCount = wrap::toUnsigned(textureSegments[textureLine].stitchCount);

		texture = wrap::next(TexturePointsBuffer->begin(),
		                     form.fillInfo.texture.index + textureSegments[textureLine].line);

		lineSegmentStart.x = pasteLocation.x;
		if (form.txof != 0.0F) {
		  auto const lineOffset =
		      (gsl::narrow_cast<float>(iRegion) + gsl::narrow_cast<float>(clipGrid.left)) /
		      gsl::narrow_cast<float>(form.fillInfo.texture.lines);
		  clipVerticalOffset = fmod(form.txof * lineOffset, form.fillInfo.texture.height);
		}
	  }
	  else {
		if (clipGridOffset != 0U) {
		  clipVerticalOffset = wrap::toFloat(iRegion % clipGridOffset) /
		                       (wrap::toFloat(clipGridOffset) * ClipRectSize.cy);
		}
		lineSegmentStart.x = pasteLocation.x + ClipBuffer->front().x;
	  }
	  lineSegmentStart.y = wrap::toFloat(clipGrid.bottom) * ClipRectSize.cy;
	  if (clipGridOffset != 0U) {
		clipVerticalOffset =
		    wrap::toFloat(iRegion % clipGridOffset) / (wrap::toFloat(clipGridOffset) * ClipRectSize.cy);
	  }
	  for (auto iVerticalGrid = clipGrid.bottom; iVerticalGrid < clipGrid.top; ++iVerticalGrid) {
		pasteLocation.y = wrap::toFloat(iVerticalGrid) * ClipRectSize.cy - clipVerticalOffset;
		if (!ClipBuffer->empty()) {
		  lineSegmentEnd.x = pasteLocation.x + ClipBuffer->front().x;
		  lineSegmentEnd.y = pasteLocation.y + ClipBuffer->front().y;
		}
		else {
		  lineSegmentEnd = pasteLocation;
		}
		if (clipStitchPoints.empty()) {
		  lineSegmentStart = lineSegmentEnd;
		}
		auto textureIt = texture;
		for (auto iStitch = 0U; iStitch < clipStitchCount; ++iStitch) {
		  if (StateMap->test(StateFlag::TXFIL)) {
			lineSegmentEnd = F_POINT {pasteLocation.x, pasteLocation.y + textureIt->y};
			++textureIt;
		  }
		  else {
			lineSegmentEnd = F_POINT {pasteLocation.x + ClipBuffer->operator[](iStitch).x,
			                          pasteLocation.y + ClipBuffer->operator[](iStitch).y};
		  }

		  clipStitchPoints.push_back(CLIP_PNT {lineSegmentStart.x, lineSegmentStart.y, 0, 0});
		  if (isin(form, regionCrossingData, lineSegmentStart.x, lineSegmentStart.y, regionCrossingStart, regionCrossingEnd, boundingRect, currentFormVertices)) {
			if ((clipStitchPoints.size() > 1) && clipStitchPoints[clipStitchPoints.size() - 1U].flag == 2) {
			  inspnt(clipStitchPoints);
			}
			clipStitchPoints.back().flag = 0;
		  }
		  else {
			if ((clipStitchPoints.size() > 1) && (clipStitchPoints[clipStitchPoints.size() - 1U].flag == 0U)) {
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
			for (auto index = 0U; index < count; ++index) {
			  clipStitchPoints.push_back({arrayOfClipIntersectData[index]->point.x,
			                              arrayOfClipIntersectData[index]->point.y,
			                              arrayOfClipIntersectData[index]->vertexIndex,
			                              1});
			  if (clipStitchPoints.size() > MAXITEMS << 2U) {
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
	  if (!breakFlag) {
		if (!clipStitchPoints.empty()) {
		  clipStitchPoints[clipStitchPoints.size() - 1U].flag = 2;
		}
	  }
	  else {
		break;
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
	auto vOffset = itFirstVertex;
	for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
	  vOffset->y -= formNegativeOffset;
	  ++vOffset;
	}
  }

#if CLPVU == 0
  StateMap->reset(StateFlag::FILDIR);
  auto clipSegments = std::vector<CLIP_SEG> {};
  if (auto endPoint = wrap::toUnsigned(clipStitchPoints.size()); endPoint != 0U) {
	--endPoint;
	// reserve a reasonable amount but not the full amount potentially required
	clipSegments.reserve(endPoint / 10U);
	auto previousPoint = 0U;
	for (auto iPoint = 0U; iPoint < endPoint; ++iPoint) {
	  switch (clipStitchPoints[iPoint].flag) {
		case 0: { // inside
		  StateMap->set(StateFlag::FILDIR);
		  break;
		}
		case 1: { // line
		  if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			clpnseg(clipStitchPoints, clipSegments, lengths, previousPoint, iPoint, currentFormVertices);
		  }
		  else {
			previousPoint = iPoint;
		  }
		  break;
		}
		case 2: { // outside
		  StateMap->reset(StateFlag::FILDIR);
		  break;
		}
		default: {
		  outDebugString(L"default hit in clpcon: flag [{}]\n", clipStitchPoints[iPoint].flag);
		  break;
		}
	  }
	}
  }

#endif

  if (!clipSegments.empty()) {
	auto sortedLengths = std::vector<LEN_INFO> {};
	sortedLengths.reserve(clipSegments.size() * 2);
	for (auto iSegment = 0U; iSegment < wrap::toUnsigned(clipSegments.size()); ++iSegment) {
	  sortedLengths.push_back(LEN_INFO {iSegment, false, clipSegments[iSegment].beginLength});
	  sortedLengths.push_back(LEN_INFO {iSegment, true, clipSegments[iSegment].endLength});
	}
	std::ranges::sort(sortedLengths, fi::lenComp);
	for (auto iSorted = 0U; iSorted < wrap::toUnsigned(sortedLengths.size()); ++iSorted) {
	  if (sortedLengths[iSorted].isEnd) {
		clipSegments[sortedLengths[iSorted].index].endIndex = iSorted;
	  }
	  else {
		clipSegments[sortedLengths[iSorted].index].beginIndex = iSorted;
	  }
	}

#if CLPVU == 1

	for (uint32_t iStitch = 0; iStitch < ActivePointIndex; ++iStitch) {
	  StitchBuffer->operator[](iStitch) =
	      F_POINT_ATTR {ClipStitchPoints[iStitch].x, ClipStitchPoints[iStitch].y, 0};
	}
	PCSHeader.stitchCount = ActivePointIndex;
#endif

#if CLPVU == 2

	auto iStitch = 0;
	for (iSegment = 0; iSegment < ClipSegmentIndex; ++iSegment) {
	  for (iStitchPoint = clipSegments[iSegment].start; iStitchPoint <= clipSegments[iSegment].finish;
	       ++iStitchPoint) {
		StitchBuffer->operator[](iStitch) =
		    F_POINT_ATTR {ClipStitchPoints[iStitchPoint].x, ClipStitchPoints[iStitchPoint].y, iSegment & 0xf};
	  }
	}
	PCSHeader.stitchCount = iStitch;

#endif

#if CLPVU == 0

	auto currentSegmentIndex = 0U;
	StateMap->set(StateFlag::FILDIR);
	OSequence->clear();
	auto clipIntersectSide = clipSegments[0].asid;
	ritseg(form, clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
	while (nucseg(clipSegments, sortedLengths, currentSegmentIndex)) {
	  ritseg(form, clipStitchPoints, clipSegments, currentSegmentIndex, clipIntersectSide, currentFormVertices);
	}
	chksid(form, 0, clipIntersectSide, currentFormVertices);
	auto index = 0U;
	for (auto iSequence = 0U; iSequence < wrap::toUnsigned(OSequence->size()); ++iSequence) {
	  if (vscmp(iSequence, index)) {
		++index;
		OSequence->operator[](index) = OSequence->operator[](iSequence);
	  }
	}
	OSequence->resize(index);
	if (StateMap->test(StateFlag::WASNEG)) {
	  for (auto& iSequence : *OSequence) {
		iSequence.x -= formOffset;
	  }
	  auto vOffset = itFirstVertex;
	  for (auto iVertex = 0U; iVertex < currentVertexCount; ++iVertex) {
		vOffset->x -= formOffset;
		++vOffset;
	  }
	  form.rectangle.left -= formOffset;
	  form.rectangle.right -= formOffset;
	}
#endif
  }
}

void fi::angout(FRM_HEAD& angledForm) {
  if (angledForm.vertexCount != 0U) {
	auto* rectangle = &angledForm.rectangle;
	auto  itVertex  = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
	rectangle->left = rectangle->right = itVertex->x;
	rectangle->bottom = rectangle->top = itVertex->y;
	++itVertex;
	for (auto iVertex = 1U; iVertex < angledForm.vertexCount; ++iVertex) {
	  if (itVertex->x > rectangle->right) {
		rectangle->right = itVertex->x;
	  }
	  if (itVertex->x < rectangle->left) {
		rectangle->left = itVertex->x;
	  }
	  if (itVertex->y < rectangle->bottom) {
		rectangle->bottom = itVertex->y;
	  }
	  if (itVertex->y > rectangle->top) {
		rectangle->top = itVertex->y;
	  }
	  ++itVertex;
	}
  }
}

void fi::horclpfn(std::vector<RNG_COUNT> const& textureSegments,
                  FRM_HEAD&                     angledForm,
                  std::vector<F_POINT>&         angledFormVertices) {
  angledForm = FormList->operator[](ClosestFormToCursor);
  auto const rotationCenter = F_POINT {wrap::midl(angledForm.rectangle.right, angledForm.rectangle.left),
                                       wrap::midl(angledForm.rectangle.top, angledForm.rectangle.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  auto itVertex = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	angledFormVertices.push_back(*itVertex);
	thred::rotflt(angledFormVertices.back(), (PI_FHALF), rotationCenter);
	++itVertex;
  }
  angledForm.vertexIndex = 0;
  angout(angledForm);
  clpcon(angledForm, textureSegments, angledFormVertices);
  rotbak((-PI_FHALF), rotationCenter);
}

void form::angclpfn(FRM_HEAD const&               form,
                    std::vector<RNG_COUNT> const& textureSegments,
                    std::vector<F_POINT>&         angledFormVertices) {
  auto angledForm = form;
  auto const rotationAngle = StateMap->test(StateFlag::ISUND) ? PI_FHALF - angledForm.underlayStitchAngle
                             : StateMap->test(StateFlag::TXFIL)
                                 ? PI_FHALF - angledForm.angleOrClipData.angle
                                 : PI_FHALF - angledForm.satinOrAngle.angle;
  auto const rotationCenter = F_POINT {wrap::midl(angledForm.rectangle.right, angledForm.rectangle.left),
                                       wrap::midl(angledForm.rectangle.top, angledForm.rectangle.bottom)};
  angledFormVertices.clear();
  angledFormVertices.reserve(angledForm.vertexCount);
  if (StateMap->test(StateFlag::ISUND)) {
	auto const& vertexList = xt::insid(angledForm);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	  angledFormVertices.push_back(vertexList[iVertex]);
	  thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	}
  }
  else {
	auto itVertex = wrap::next(FormVertices->cbegin(), angledForm.vertexIndex);
	for (auto iVertex = 0U; iVertex < angledForm.vertexCount; ++iVertex) {
	  angledFormVertices.push_back(*itVertex);
	  thred::rotflt(angledFormVertices.back(), rotationAngle, rotationCenter);
	  ++itVertex;
	}
  }
  angledForm.vertexIndex = 0;
  fi::angout(angledForm);
  fi::clpcon(angledForm, textureSegments, angledFormVertices);
  fi::rotbak(-rotationAngle, rotationCenter);
}

auto fi::spComp(const gsl::not_null<SMAL_PNT_L const*> arg1,
                const gsl::not_null<SMAL_PNT_L const*> arg2) noexcept -> bool {
  // make sure the comparison obeys strict weak ordering for stable sorting
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

auto fi::isclos(std::vector<SMAL_PNT_L> const& lineEndpoints,
                uint32_t                       line0Index,
                uint32_t                       line1Index,
                float                          gapToClosestRegion) noexcept -> bool {
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

auto fi::lnclos(std::vector<uint32_t> const&   groupIndexSequence,
                std::vector<SMAL_PNT_L> const& lineEndpoints,
                uint32_t                       group0,
                uint32_t                       line0,
                uint32_t                       group1,
                uint32_t                       line1,
                float                          gapToClosestRegion) noexcept -> bool {
  auto const lineEndPoint0 = std::next(lineEndpoints.begin(), groupIndexSequence[group0]);
  if (group1 > groupIndexSequence.size() - 2U) {
	return false;
  }
  if (group0 == 0) {
	return false;
  }
  auto count0 = (groupIndexSequence[wrap::toSize(group0) + 1U] - groupIndexSequence[group0]) / 2U;
  auto index0 = 0U;
  while ((count0 != 0U) && lineEndPoint0[index0].line != line0) {
	--count0;
	index0 += 2;
  }
  if (count0 != 0U) {
	auto const lineEndPoint1 = std::next(lineEndpoints.begin(), groupIndexSequence[group1]);
	auto       index1        = 0U;
	auto count1 = (groupIndexSequence[wrap::toSize(group1) + 1U] - groupIndexSequence[group1]) / 2U;
	while ((count1 != 0U) && lineEndPoint1[index1].line != line1) {
	  --count1;
	  index1 += 2;
	}
	if (count1 != 0U) {
	  return isclos(lineEndpoints, groupIndexSequence[group0] + index0, groupIndexSequence[group1] + index1, gapToClosestRegion);
	}
  }
  return false;
}

auto fi::regclos(std::vector<uint32_t> const&   groupIndexSequence,
                 std::vector<SMAL_PNT_L> const& lineEndpoints,
                 std::vector<uint32_t> const&   sortedLineIndices,
                 uint32_t                       iRegion0,
                 uint32_t                       iRegion1,
                 std::vector<REGION> const&     regionsList,
                 float                          gapToClosestRegion,
                 uint32_t&                      nextGroup) noexcept -> bool {
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
      (groupStart != 0U) &&
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
  if (((group0Start > group1Start) ? (group0Start - group1Start) : (group1Start - group0Start)) < 2) {
	if (isclos(lineEndpoints, lineStart0Index, lineStart1Index, gapToClosestRegion)) {
	  nextGroup = group0Start;
	  return true;
	}
  }
  if (((group0Start > group1End) ? (group0Start - group1End) : (group1End - group0Start)) < 2) {
	if (isclos(lineEndpoints, lineStart0Index, lineEnd1Index, gapToClosestRegion)) {
	  nextGroup = group0Start;
	  return true;
	}
  }
  if (((group0End > group1Start) ? (group0End - group1Start) : (group1Start - group0End)) < 2) {
	if (isclos(lineEndpoints, lineEnd0Index, lineStart1Index, gapToClosestRegion)) {
	  nextGroup = group0End;
	  return true;
	}
  }
  if (((group0End > group1End) ? (group0End - group1End) : (group1End - group0End)) < 2) {
	if (isclos(lineEndpoints, lineEnd0Index, lineEnd1Index, gapToClosestRegion)) {
	  nextGroup = group0End;
	  return true;
	}
  }
  return false;
}

auto fi::unvis(boost::dynamic_bitset<> const& visitedRegions, int32_t& visitedIndex) -> bool {
  auto const regionCount = gsl::narrow<int32_t>(visitedRegions.size());
  for (visitedIndex = 0; visitedIndex < regionCount; ++visitedIndex) {
	if (!visitedRegions[wrap::toSize(visitedIndex)]) {
	  return true;
	}
  }
  return false;
}

auto fi::notdun(std::vector<RG_SEQ>&           tempPath,
                std::vector<R_CON> const&      pathMap,
                std::vector<uint32_t> const&   mapIndexSequence,
                boost::dynamic_bitset<> const& visitedRegions,
                ptrdiff_t                      pathLength,
                uint32_t                       doneRegion,
                uint32_t                       sequencePathIndex) -> bool {
  auto previousLevel = pathLength;
  if (previousLevel != 0U) {
	--previousLevel;
  }
  auto const itRegionPath = wrap::next(tempPath.begin(), sequencePathIndex);
  itRegionPath[0].pcon    = mapIndexSequence[doneRegion];
  itRegionPath[0].count   = gsl::narrow<int32_t>(
      mapIndexSequence[gsl::narrow_cast<size_t>(doneRegion) + 1U] - itRegionPath[0].pcon);
  for (auto iPath = ptrdiff_t {1}; iPath < pathLength; ++iPath) {
	itRegionPath[iPath].pcon  = mapIndexSequence[pathMap[itRegionPath[iPath - 1].pcon].node];
	itRegionPath[iPath].count = gsl::narrow<int32_t>(
	    mapIndexSequence[gsl::narrow_cast<size_t>(pathMap[itRegionPath[iPath - 1].pcon].node) + 1] -
	    itRegionPath[iPath].pcon);
  }
  while (visitedRegions[pathMap[itRegionPath[previousLevel].pcon].node]) {
	if (--itRegionPath[previousLevel].count > 0) {
	  ++(itRegionPath[previousLevel].pcon);
	}
	else {
	  auto pivot = previousLevel;
	  do {
		if (pivot != 0U) {
		  --pivot;
		}
		else {
		  return true;
		}
		--(itRegionPath[pivot].count);
		++(itRegionPath[pivot].pcon);
	  } while (itRegionPath[pivot].count == 0);
	  ++pivot;
	  while (pivot <= previousLevel) {
		if (pivot != 0U) {
		  itRegionPath[pivot].pcon  = mapIndexSequence[pathMap[itRegionPath[pivot - 1].pcon].node];
		  itRegionPath[pivot].count = gsl::narrow<int32_t>(
		      mapIndexSequence[wrap::toSize(pathMap[itRegionPath[pivot - 1].pcon].node) + 1U] -
		      itRegionPath[pivot].pcon);
		}
		else {
		  if (--itRegionPath[0].count != 0) {
			++(itRegionPath[0].pcon);
		  }
		  else {
			return true;
		  }
		}
		++pivot;
	  }
	}
  }
  return false;
}

auto fi::reglen(std::vector<SMAL_PNT_L> const&       lineEndpoints,
                std::vector<uint32_t> const&         sortedLineIndices,
                uint32_t                             iRegion,
                std::array<F_POINT, SQRCORNS> const& lastRegionCorners,
                std::vector<REGION> const&           regionsList) noexcept -> float {
  auto corners = std::array<SMAL_PNT_L, SQRCORNS> {};
  auto index   = sortedLineIndices[regionsList[iRegion].start];
  corners[0]   = lineEndpoints[index];
  corners[1]   = lineEndpoints[++index];
  index        = sortedLineIndices[regionsList[iRegion].end];
  corners[2]   = lineEndpoints[index];
  corners[3]   = lineEndpoints[++index];

  auto minimumLength = BIGFLOAT;
  for (auto const& corner : lastRegionCorners) {
	for (auto const& point : corners) {
	  if (auto const length = hypot(corner.x - point.x, corner.y - point.y); length < minimumLength) {
		minimumLength = length;
	  }
	}
  }
  return minimumLength;
}

void fi::nxtrgn(std::vector<RG_SEQ>&           tempPath,
                std::vector<R_CON> const&      pathMap,
                std::vector<uint32_t> const&   mapIndexSequence,
                boost::dynamic_bitset<>&       visitedRegions,
                std::vector<SMAL_PNT_L> const& lineEndpoints,
                std::vector<uint32_t> const&   sortedLineIndices,
                std::vector<REGION> const&     regionsList,
                uint32_t&                      doneRegion,
                uint32_t                       pathMapIndex,
                uint32_t&                      sequencePathIndex,
                int32_t                        visitedIndex) {
  auto lastRegionCorners = std::array<F_POINT, SQRCORNS> {};
  auto pathLength        = ptrdiff_t {1}; // length of the path to the region
  while (notdun(tempPath, pathMap, mapIndexSequence, visitedRegions, pathLength, doneRegion, sequencePathIndex)) {
	++pathLength;
	auto maxPathLength = gsl::narrow<ptrdiff_t>(tempPath.size() - sequencePathIndex);

	if (constexpr auto DEPFACT = 8; // tuneable value. Increasing this increases path discovery time exponentially
	    maxPathLength > DEPFACT) {
	  maxPathLength = DEPFACT;
	}
	outDebugString(L"nxtrgn: pathLength {}\n", pathLength);
	if (pathLength > maxPathLength) {
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
		if (!visitedRegions[iRegion]) {
		  if (auto const length = reglen(lineEndpoints, sortedLineIndices, iRegion, lastRegionCorners, regionsList);
		      length < minimumLength) {
			minimumLength = length;
			newRegion     = iRegion;
		  }
		}
	  }
	  tempPath[sequencePathIndex].skp = true;
	  for (auto iPath = 0U; iPath < pathMapIndex; ++iPath) {
		if (pathMap[iPath].node == newRegion) {
		  tempPath[sequencePathIndex++].pcon = iPath;
		  visitedRegions.set(newRegion);
		  doneRegion = newRegion;
		  return;
		}
	  }
	  tempPath[sequencePathIndex].count  = visitedIndex;
	  tempPath[sequencePathIndex++].pcon = MAXDWORD;
	  visitedRegions.set(wrap::toSize(visitedIndex));
	  doneRegion = wrap::toUnsigned(visitedIndex);
	  return;
	}
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

void fi::nxtseq(std::vector<F_SEQ>&          sequencePath,
                std::vector<R_CON> const&    pathMap,
                std::vector<uint32_t> const& mapIndexSequence,
                uint32_t                     pathIndex) {
  if ((wrap::toSize(pathIndex) + 1U) < sequencePath.size()) {
	unsigned const nextNode = sequencePath[wrap::toSize(pathIndex) + 1U].node;
	unsigned       iPath    = mapIndexSequence[sequencePath[pathIndex].node];
	while (iPath < mapIndexSequence[wrap::toSize(sequencePath[pathIndex].node) + 1U] &&
	       pathMap[iPath].node != nextNode) {
	  ++iPath;
	}
	if (iPath < pathMap.size()) {
	  sequencePath[pathIndex].nextGroup = gsl::narrow<uint16_t>(pathMap[iPath].nextGroup);
	}
	else {
	  sequencePath[pathIndex].nextGroup = 0;
	}
  }
  else {
	sequencePath[pathIndex].nextGroup = 0;
  }
}

void fi::brkdun(std::vector<SMAL_PNT_L> const& lineEndpoints,
                std::vector<uint32_t> const&   sortedLineIndices,
                uint32_t                       start,
                uint32_t                       finish,
                std::vector<F_POINT>&          workingFormVertices) {
  BSequence->emplace_back(
      lineEndpoints[sortedLineIndices[start]].x, lineEndpoints[sortedLineIndices[start]].y, 0);
  BSequence->emplace_back(
      lineEndpoints[sortedLineIndices[finish]].x, lineEndpoints[sortedLineIndices[finish]].y, 0);
  BSequence->emplace_back(
      workingFormVertices.operator[](lineEndpoints[sortedLineIndices[start]].line).x,
      workingFormVertices.operator[](lineEndpoints[sortedLineIndices[start]].line).y,
      0);
  StateMap->set(StateFlag::BRKFIX);
}

void fi::duseq1(std::vector<SMAL_PNT_L> const& lineEndpoints,
                std::vector<uint32_t> const&   sortedLineIndices,
                uint32_t                       sequenceIndex) {
  auto        index     = sortedLineIndices[sequenceIndex];
  auto const& sequence0 = lineEndpoints[index];
  auto const& sequence1 = lineEndpoints[++index];
  BSequence->emplace_back(wrap::midl(sequence1.x, sequence0.x), wrap::midl(sequence1.y, sequence0.y), 0);
}

void fi::movseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
                std::vector<uint32_t> const&   sortedLineIndices,
                uint32_t                       ind) {
  auto lineEndPoint = std::next(lineEndpoints.begin(), sortedLineIndices[ind]);
  BSequence->emplace_back(B_SEQ_PNT {lineEndPoint->x, lineEndPoint->y, SEQBOT});
  // Be careful - this makes lineEndPoint point to the next entry in LineEndPoints
  //             and not the next entry in sortedLines
  ++lineEndPoint;
  BSequence->emplace_back(B_SEQ_PNT {lineEndPoint->x, lineEndPoint->y, SEQTOP});
}

void fi::brkseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
                std::vector<uint32_t> const&   sortedLineIndices,
                uint32_t                       start,
                uint32_t                       finish,
                boost::dynamic_bitset<>&       sequenceMap,
                uint32_t&                      lastGroup,
                uint32_t&                      sequenceIndex) {
  StateMap->reset(StateFlag::SEQDUN);
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
		BSequence->emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                        lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                        0);
		sequenceIndex = iLineDec;
		BSequence->emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                        lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                        0);
		savedGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	  }
	  else {
		sequenceIndex = iLineDec;
	  }
	  if (sequenceMap.test_set(iLineDec)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
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
		BSequence->emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                        lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                        0);
		sequenceIndex = iLine;
		BSequence->emplace_back(lineEndpoints[sortedLineIndices[sequenceIndex]].x,
		                        lineEndpoints[sortedLineIndices[sequenceIndex]].y,
		                        0);
		savedGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	  }
	  else {
		sequenceIndex = iLine;
	  }
	  if (sequenceMap.test_set(iLine)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  duseq1(lineEndpoints, sortedLineIndices, sequenceIndex);
		}
	  }
	  else {
		movseq(lineEndpoints, sortedLineIndices, iLine);
	  }
	}
	lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
  }
  if (StateMap->testAndReset(StateFlag::SEQDUN)) {
	duseq1(lineEndpoints, sortedLineIndices, sequenceIndex);
  }
}

void fi::dunseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
                std::vector<uint32_t> const&   sortedLineIndices,
                uint32_t                       start,
                uint32_t                       finish,
                uint32_t&                      lastGroup) {
  auto minimumY = BIGFLOAT;
  for (auto iLine = start; iLine <= finish; ++iLine) {
	auto const index = wrap::toSize(sortedLineIndices[start]);
	if (auto const deltaY = lineEndpoints[index + 1U].y - lineEndpoints[index].y; deltaY < minimumY) {
	  minimumY = deltaY;
	}
  }
  if (util::closeEnough(minimumY, BIGFLOAT)) {
	minimumY = 0.0F;
  }
  else {
	minimumY /= 2.0F;
  }
  BSequence->emplace_back(
      lineEndpoints[sortedLineIndices[start]].x, lineEndpoints[sortedLineIndices[start]].y + minimumY, 0);
  BSequence->emplace_back(lineEndpoints[sortedLineIndices[finish]].x,
                          lineEndpoints[sortedLineIndices[finish]].y + minimumY,
                          0);
  lastGroup = lineEndpoints[sortedLineIndices[finish]].group;
}

void fi::duseq(std::vector<SMAL_PNT_L> const& lineEndpoints,
               std::vector<uint32_t> const&   sortedLineIndices,
               uint32_t                       start,
               uint32_t                       finish,
               boost::dynamic_bitset<>&       sequenceMap,
               uint32_t&                      lastGroup,
               uint32_t&                      sequenceIndex) {
  auto savedTopLine = lineEndpoints[wrap::toSize(sortedLineIndices[start]) + 1U].line;
  StateMap->reset(StateFlag::SEQDUN);
  if (bool flag = false; start > finish) {
	auto iLine = start + 1U;
	// This odd construction for iLine is used to ensure loop terminates when finish = 0
	for (; iLine != finish; --iLine) {
	  if (auto const iLineDec = iLine - 1U; sequenceMap.test_set(iLineDec)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  duseq1(lineEndpoints, sortedLineIndices, iLineDec);
		  sequenceIndex = iLineDec;
		  flag          = true;
		}
		else {
		  if (savedTopLine != lineEndpoints[wrap::toSize(sortedLineIndices[iLineDec]) + 1U].line) {
			if (iLineDec != 0U) {
			  duseq1(lineEndpoints, sortedLineIndices, iLineDec + 1U);
			}
			duseq1(lineEndpoints, sortedLineIndices, iLineDec);
			sequenceIndex = iLineDec;
			flag          = true;
			savedTopLine  = lineEndpoints[wrap::toSize(sortedLineIndices[iLineDec]) + 1U].line;
		  }
		}
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::SEQDUN)) {
		  duseq1(lineEndpoints, sortedLineIndices, iLineDec + 1U);
		}
		sequenceIndex = iLineDec;
		flag          = true;
		movseq(lineEndpoints, sortedLineIndices, iLineDec);
	  }
	}
	if (StateMap->testAndReset(StateFlag::SEQDUN)) {
	  duseq1(lineEndpoints, sortedLineIndices, iLine);
	  sequenceIndex = iLine;
	  flag          = true;
	}
	if (flag) { // flag is set if sequenceLines has been set above
	  lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	}
  }
  else {
	auto iLine = start;
	for (; iLine <= finish; ++iLine) {
	  if (sequenceMap.test_set(iLine)) {
		if (!StateMap->testAndSet(StateFlag::SEQDUN)) {
		  flag = true;
		  duseq1(lineEndpoints, sortedLineIndices, iLine);
		  sequenceIndex = iLine;
		}
		else {
		  if (savedTopLine != lineEndpoints[wrap::toSize(sortedLineIndices[iLine]) + 1U].line) {
			if (iLine != 0U) {
			  duseq1(lineEndpoints, sortedLineIndices, iLine - 1U);
			}
			flag = true;
			duseq1(lineEndpoints, sortedLineIndices, iLine);
			sequenceIndex = iLine;
			savedTopLine  = lineEndpoints[wrap::toSize(sortedLineIndices[iLine]) + 1U].line;
		  }
		}
	  }
	  else {
		if (StateMap->testAndReset(StateFlag::SEQDUN)) {
		  if (iLine != 0U) {
			duseq1(lineEndpoints, sortedLineIndices, iLine - 1U);
		  }
		}
		flag          = true;
		sequenceIndex = iLine;
		movseq(lineEndpoints, sortedLineIndices, iLine);
	  }
	}
	if (StateMap->testAndReset(StateFlag::SEQDUN)) {
	  if (iLine != 0U) {
		flag = true;
		duseq1(lineEndpoints, sortedLineIndices, iLine - 1U);
		sequenceIndex = iLine - 1U;
	  }
	}
	if (flag) {
	  lastGroup = lineEndpoints[sortedLineIndices[sequenceIndex]].group;
	}
  }
}

void fi::durgn(FRM_HEAD const&                form,
               std::vector<F_SEQ> const&      sequencePath,
               boost::dynamic_bitset<>&       visitedRegions,
               std::vector<SMAL_PNT_L> const& lineEndpoints,
               std::vector<uint32_t> const&   sortedLineIndices,
               uint32_t                       pthi,
               uint32_t                       lineCount,
               std::vector<REGION> const&     regionsList,
               uint32_t&                      lastGroup,
               uint32_t                       sequencePathIndex,
               std::vector<F_POINT>&          workingFormVertices) {
  auto        sequenceMap   = boost::dynamic_bitset<>(lineCount);
  auto        nextGroup     = gsl::narrow_cast<uint32_t>(sequencePath[pthi].nextGroup);
  auto const  iRegion       = sequencePath[pthi].node;
  auto const& currentRegion = regionsList[iRegion];
  auto const  sequenceStart = currentRegion.start;
  auto const  sequenceEnd   = currentRegion.end;
  if (sequencePath[pthi].skp || StateMap->testAndReset(StateFlag::BRKFIX)) {
	if (auto iter = std::prev(BSequence->end()); iter->attribute != SEQBOT) {
	  --iter;
	  BSequence->emplace_back(iter->x, iter->y, 0);
	}
	// clang-format off
	auto const  firstLine     = lineEndpoints[sortedLineIndices[sequenceStart]].line;
	auto const& bpnt          = BSequence->back();
	auto        minimumLength = BIGFLOAT;
	auto        mindif        = 0U;
	// clang-format on
	for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	  if (auto const length = hypot(bpnt.x - workingFormVertices.operator[](iVertex).x,
	                                bpnt.y - workingFormVertices.operator[](iVertex).y);
	      length < minimumLength) {
		minimumLength = length;
		mindif        = iVertex;
	  }
	}
	if (minimumLength != 0.0F) {
	  BSequence->emplace_back(
	      workingFormVertices.operator[](mindif).x, workingFormVertices.operator[](mindif).y, 0);
	}
	if (form.vertexCount != 0U) {
	  auto const fdif = (form.vertexCount + firstLine - mindif) % form.vertexCount;
	  if (auto const bdif = (form.vertexCount - firstLine + mindif) % form.vertexCount; fdif < bdif) {
		auto ind = form::nxt(form, mindif);
		while (ind != firstLine) {
		  BSequence->emplace_back(
		      workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
		  ind = form::nxt(form, ind);
		}
		BSequence->emplace_back(
		    workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
	  }
	  else {
		auto ind = form::prv(form, mindif);
		while (ind != firstLine) {
		  BSequence->emplace_back(
		      workingFormVertices.operator[](ind).x, workingFormVertices.operator[](ind).y, 0);
		  ind = form::prv(form, ind);
		}
		BSequence->emplace_back(
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
	    std::round((wrap::toDouble(lastGroup) - groupStart) / (wrap::toDouble(groupEnd) - groupStart) *
	                   (wrap::toDouble(sequenceEnd) - sequenceStart) +
	               sequenceStart);
	wrap::narrow(seql, std::abs(intermediate));
  }
  auto const length = (wrap::toDouble(groupEnd) - groupStart) * (wrap::toDouble(sequenceEnd) - sequenceStart);
  auto seqn = sequenceEnd;
  if (length != 0.0) {
	if (nextGroup == 0 || nextGroup < groupStart) {
	  nextGroup = groupStart;
	}
	seqn = wrap::round<decltype(seqn)>((wrap::toDouble(nextGroup) - groupStart) / length + sequenceStart);
  }
  if (seql < sequenceStart) {
	seql = sequenceStart;
  }
  if (seql > sequenceEnd) {
	seql = sequenceEnd;
  }
  if (seqn < sequenceStart) {
	seqn = sequenceStart;
  }
  if (seqn > sequenceEnd) {
	seqn = sequenceEnd;
  }
  if (lineEndpoints[sortedLineIndices[seql]].group != lastGroup) {
	if (seql < sequenceEnd && lineEndpoints[sortedLineIndices[wrap::toSize(seql) + 1U]].group == lastGroup) {
	  ++seql;
	}
	else {
	  if (seql > sequenceStart && lineEndpoints[sortedLineIndices[seql - 1U]].group == lastGroup) {
		--seql;
	  }
	  else {
		auto mindif = std::numeric_limits<uint32_t>::max();
		for (auto ind = sequenceStart; ind <= sequenceEnd; ++ind) {
		  if (auto const gdif = ((lineEndpoints[sortedLineIndices[ind]].group > lastGroup)
		                             ? (lineEndpoints[sortedLineIndices[ind]].group - lastGroup)
		                             : (lastGroup - lineEndpoints[sortedLineIndices[ind]].group));
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
		auto mindif = std::numeric_limits<uint32_t>::max();
		for (auto ind = sequenceStart; ind <= sequenceEnd; ++ind) {
		  if (auto const gdif = ((lineEndpoints[sortedLineIndices[ind]].group > nextGroup)
		                             ? (lineEndpoints[sortedLineIndices[ind]].group - nextGroup)
		                             : (nextGroup - lineEndpoints[sortedLineIndices[ind]].group));
		      gdif < mindif) {
			mindif = gdif;
			seqn   = ind;
		  }
		}
	  }
	}
  }
  if (auto sequenceIndex = wrap::toUnsigned(lineEndpoints.size() + 1U); currentRegion.breakCount != 0U) {
	if (dun) {
	  brkdun(lineEndpoints, sortedLineIndices, seql, seqn, workingFormVertices);
	}
	else {
	  if (lastGroup >= groupEnd) {
		brkseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
		if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
		  brkseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
		}
	  }
	  else {
		if (groupStart <= nextGroup) {
		  if (seql != sequenceStart) {
			brkseq(lineEndpoints, sortedLineIndices, seql, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
		  }
		  brkseq(lineEndpoints, sortedLineIndices, sequenceStart, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
		  if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
			brkseq(lineEndpoints, sortedLineIndices, sequenceEnd, seqn, sequenceMap, lastGroup, sequenceIndex);
		  }
		}
		else {
		  if (seql != sequenceEnd) {
			brkseq(lineEndpoints, sortedLineIndices, seql, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
		  }
		  brkseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
		  if (pthi < sequencePathIndex - 1 && sequenceStart != seqn) {
			brkseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
		  }
		}
	  }
	}
  }
  else {
	if (dun) {
	  dunseq(lineEndpoints, sortedLineIndices, seql, seqn, lastGroup);
	}
	else {
	  if (lastGroup >= groupEnd) {
		duseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
		duseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
	  }
	  else {
		if (groupStart <= nextGroup) {
		  if (seql != sequenceStart) {
			duseq(lineEndpoints, sortedLineIndices, seql, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
		  }
		  duseq(lineEndpoints, sortedLineIndices, sequenceStart, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
		  if (pthi < sequencePathIndex - 1 && sequenceEnd != seqn) {
			duseq(lineEndpoints, sortedLineIndices, sequenceEnd, seqn, sequenceMap, lastGroup, sequenceIndex);
		  }
		}
		else {
		  if (seql != sequenceEnd) {
			duseq(lineEndpoints, sortedLineIndices, seql, sequenceEnd, sequenceMap, lastGroup, sequenceIndex);
		  }
		  duseq(lineEndpoints, sortedLineIndices, sequenceEnd, sequenceStart, sequenceMap, lastGroup, sequenceIndex); // NOLINT(readability-suspicious-call-argument)
		  if (pthi < sequencePathIndex - 1 && sequenceStart != seqn) {
			duseq(lineEndpoints, sortedLineIndices, sequenceStart, seqn, sequenceMap, lastGroup, sequenceIndex);
		  }
		}
	  }
	}
  }
}

auto fi::compLines(std::tuple<uint32_t, SMAL_PNT_L*> const& arg3,
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

auto fi::getLineSortOrder(std::vector<SMAL_PNT_L>& lineEndpoints) -> std::vector<uint32_t> {
  auto       sorted          = std::vector<std::tuple<uint32_t, SMAL_PNT_L*>> {};
  auto const stitchLineCount = lineEndpoints.size();
  sorted.reserve(stitchLineCount / 2U);
  for (auto iLine = 0U; iLine < stitchLineCount; iLine += 2U) {
	sorted.emplace_back(iLine, &lineEndpoints[iLine]);
  }
  std::ranges::sort(sorted, fi::compLines);
  auto retVal = std::vector<uint32_t> {};
  retVal.reserve(stitchLineCount / 2U);
  for (auto& line : sorted) {
	retVal.push_back(std::get<uint32_t>(line));
  }
  return retVal;
}

void fi::lcon(FRM_HEAD const&              form,
              std::vector<uint32_t> const& groupIndexSequence,
              std::vector<SMAL_PNT_L>&     lineEndpoints,
              std::vector<F_POINT>&        workingFormVertices) {
#if BUGSEQ
  UNREFERENCED_PARAMETER(groupIndexSequence);
#endif

  if (!lineEndpoints.empty()) {
	auto       sortedLineIndices = fi::getLineSortOrder(lineEndpoints);
	auto const lineCount         = wrap::toUnsigned(sortedLineIndices.size());
	auto       regions           = std::vector<REGION> {};
	regions.emplace_back(0U, 0U, 0U, 0U);
	auto breakLine = lineEndpoints[sortedLineIndices[0]].line;
	for (auto iLine = 0U; iLine < lineCount; ++iLine) {
	  if (breakLine != lineEndpoints[sortedLineIndices[iLine]].line) {
		regions.back().end = iLine - 1U;
		regions.emplace_back(iLine, 0U, 0U, 0U);
		breakLine = lineEndpoints[sortedLineIndices[iLine]].line;
	  }
	}
	regions.back().end        = lineCount - 1U;
	auto const regionCount    = wrap::toUnsigned(regions.size());
	auto       visitedRegions = boost::dynamic_bitset<>(regionCount);
	for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	  auto count = 0U;
	  if ((regions[iRegion].end - regions[iRegion].start) > 1) {
		auto startGroup = lineEndpoints[sortedLineIndices[regions[iRegion].start]].group;
		for (auto iLine = regions[iRegion].start + 1U; iLine <= regions[iRegion].end; ++iLine) {
		  ++startGroup;
		  if (lineEndpoints[sortedLineIndices[iLine]].group != startGroup) {
			if (constexpr auto STLINE = 0U; count == 0U) {
			  regions[iRegion].regionBreak = STLINE;
			}
			++count;
			startGroup = lineEndpoints[sortedLineIndices[iLine]].group;
		  }
		}
	  }
	  regions[iRegion].breakCount = count;
	}

#if BUGSEQ
	// Note - this debug code only works for vertical fill on a single form
	auto bugColor = 0U;
	for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
	  for (auto iLine = regions[iRegion].start; iLine <= regions[iRegion].end; ++iLine) {
		auto const* lineGroupPoint = &*sortedLines[iLine];
		StitchBuffer->push_back(F_POINT_ATTR {lineGroupPoint[0].x, lineGroupPoint[0].y, bugColor});
		StitchBuffer->push_back(F_POINT_ATTR {lineGroupPoint[1].x, lineGroupPoint[1].y, bugColor});
	  }
	  ++bugColor;
	  bugColor &= 0xf;
	}
#else
	BSequence->clear();
	auto mapIndexSequence = std::vector<uint32_t> {};
	mapIndexSequence.reserve(wrap::toSize(regionCount) + 1U);
	auto pathMap = std::vector<R_CON> {};
	if (auto sequencePath = std::vector<F_SEQ> {}; regionCount > 1) {
	  auto pathMapIndex = 0U;
	  // use the number of possible pairs of nodes n(n - 1)/2 and account for RegionCount possibly being odd
	  pathMap.reserve(wrap::toSize((regionCount * (regionCount - 1U)) / 2U) + 2U);
	  for (auto iSequence = 0U; iSequence < regionCount; ++iSequence) {
		mapIndexSequence.push_back(pathMapIndex);
		auto count              = 0;
		auto gapToClosestRegion = 0.0F;
		for (auto iNode = 0U; iNode < regionCount; ++iNode) {
		  if (iSequence != iNode) {
			auto nextGroup = 0U;
			if (auto const isConnected = regclos(
			        groupIndexSequence, lineEndpoints, sortedLineIndices, iSequence, iNode, regions, gapToClosestRegion, nextGroup);
			    isConnected) {
			  pathMap.push_back(R_CON {iNode, isConnected, nextGroup});
			  ++pathMapIndex;
			  ++count;
			}
		  }
		}
		while (count == 0) {
		  gapToClosestRegion += LineSpacing;
		  count = 0;
		  for (auto iNode = 0U; iNode < regionCount; ++iNode) {
			if (iSequence != iNode) {
			  auto nextGroup = 0U;
			  if (auto const isConnected = regclos(
			          groupIndexSequence, lineEndpoints, sortedLineIndices, iSequence, iNode, regions, gapToClosestRegion, nextGroup);
			      isConnected) {
				pathMap.push_back(R_CON {iNode, isConnected, nextGroup});
				++pathMapIndex;
				++count;
			  }
			}
		  }
		}
	  }
	  mapIndexSequence.push_back(pathMapIndex);
	  // find the leftmost region
	  auto startGroup = std::numeric_limits<uint32_t>::max();
	  auto leftRegion = 0U;
	  for (auto iRegion = 0U; iRegion < regionCount; ++iRegion) {
		if (auto const& lineGroupPoint = lineEndpoints[sortedLineIndices[regions[iRegion].start]];
		    lineGroupPoint.group < startGroup) {
		  startGroup = lineGroupPoint.group;
		  leftRegion = iRegion;
		}
	  }
	  auto tempPath = std::vector<RG_SEQ> {};
	  tempPath.resize(wrap::toSize((regionCount * (regionCount - 1U)) / 2U) + 1U);
	  // find the leftmost region in pathMap
	  auto sequencePathIndex = 1U;
	  auto dontSkip          = true;
	  auto inPath            = 0U;
	  for (inPath = 0U; inPath < pathMapIndex; ++inPath) {
		if (pathMap[inPath].node == leftRegion) {
		  dontSkip = false;
		  break;
		}
	  }
	  if (dontSkip) {
		pathMap.push_back(R_CON {leftRegion, false, 0});
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
		nxtrgn(tempPath,
		       pathMap,
		       mapIndexSequence,
		       visitedRegions,
		       lineEndpoints,
		       sortedLineIndices,
		       regions,
		       doneRegion,
		       pathMapIndex,
		       sequencePathIndex,
		       visitedIndex);
	  }
	  auto count = std::numeric_limits<uint32_t>::max();
	  sequencePath.reserve(sequencePathIndex);
	  for (auto iPath = 0U; iPath < sequencePathIndex; ++iPath) {
		bool const tmpSkip = tempPath[iPath].skp;
		uint16_t   tmpNode = 0U;
		if (tempPath[iPath].pcon == std::numeric_limits<uint32_t>::max()) {
		  wrap::narrow(tmpNode, tempPath[iPath].count);
		  count = wrap::toUnsigned(tempPath[iPath].count);
		}
		else {
		  if (tempPath[iPath].pcon != count) {
			count = tempPath[iPath].pcon;
			wrap::narrow(tmpNode, pathMap[tempPath[iPath].pcon].node);
		  }
		}
		sequencePath.push_back(F_SEQ {tmpNode, 0, tmpSkip});
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
	}
	else {
	  constexpr auto SPATHIDX = 1U; // Sequence path index
	  sequencePath.resize(SPATHIDX);
	  auto lastGroup       = 0U;
	  sequencePath[0].node = 0;
	  wrap::narrow(sequencePath[0].nextGroup, lineEndpoints[sortedLineIndices[regions[0].end]].group);
	  sequencePath[0].skp = false;
	  durgn(form, sequencePath, visitedRegions, lineEndpoints, sortedLineIndices, 0, lineCount, regions, lastGroup, SPATHIDX, workingFormVertices);
	}

#endif
  }
}

void fi::bakseq() {
#if BUGSEQ
#else
  constexpr auto RITSIZ = 6;

  static constexpr auto SEQ_TABLE = std::array<int32_t, RITSIZ> {
      12,
      7,
      15,
      11,
      13,
      17,
  };

#if BUGBAK

  for (auto val : *BSequence) {
	OSequence->push_back(F_POINT {val.x, val.y});
  }
  FormList->operator[](ClosestFormToCursor).maxFillStitchLen = 6000;
#else

  auto iSequence = BSequence->size();
  if (iSequence != 0U) {
	--iSequence;
  }
  OSequence->clear();
  StateMap->reset(StateFlag::FILDIR);
  OSequence->push_back(F_POINT {BSequence->operator[](iSequence).x, BSequence->operator[](iSequence).y});
  if (iSequence != 0U) {
	--iSequence;
  }
  while (iSequence > 0) {
	// clang-format off
	auto const  rcnt           = iSequence % SEQ_TABLE.size();
	auto const  stitchSpacing2 = LineSpacing * 2;
	auto const  rit            = std::lround(BSequence->operator[](iSequence).x / stitchSpacing2);
	auto const& bPrevious      = BSequence->operator[](iSequence - 1U);
	auto&       bCurrent       = BSequence->operator[](iSequence);
	auto const& bNext          = BSequence->operator[](iSequence + 1U);
	auto        delta          = F_POINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};
	auto        slope          = std::numeric_limits<float>::max(); // initialize to the max value i.e. slope when y is zero
	// clang-format on
	if (delta.y != 0.0F) {
	  slope = delta.x / delta.y;
	}
	auto const  userStitchLength9 = UserStitchLength / MAXSIZ;
	auto const& form              = FormList->operator[](ClosestFormToCursor);
	switch (bCurrent.attribute) {
	  case SEQTOP: {
		if ((form.extendedAttribute & AT_SQR) != 0U) {
		  if (StateMap->testAndFlip(StateFlag::FILDIR)) {
			OSequence->push_back(F_POINT {bPrevious.x, bPrevious.y});
			auto yVal = wrap::toFloat(wrap::ceil<int32_t>(bCurrent.y / UserStitchLength)) * UserStitchLength +
			            wrap::toFloat(std::abs(rit % SEQ_TABLE[rcnt])) * userStitchLength9;
			do {
			  OSequence->push_back(F_POINT {0.0F, yVal});
			  if (yVal > bCurrent.y) {
				break;
			  }
			  OSequence->back().x = bCurrent.x;
			  yVal += UserStitchLength;
			} while (true);
			OSequence->back() = bCurrent;
		  }
		  else {
			OSequence->push_back(F_POINT {bCurrent.x, bCurrent.y});
			auto yVal = wrap::toFloat(wrap::floor<int32_t>(bCurrent.y / UserStitchLength)) * UserStitchLength -
			            wrap::toFloat(std::abs((rit + 2) % SEQ_TABLE[rcnt])) * userStitchLength9;
			do {
			  OSequence->push_back(F_POINT {0.0F, yVal});
			  if (yVal < bPrevious.y) {
				break;
			  }
			  OSequence->back().x = bCurrent.x;
			  yVal -= UserStitchLength;
			} while (true);
			OSequence->back() = bPrevious;
		  }
		}
		else {
		  auto yVal = wrap::toFloat(wrap::ceil<int32_t>(bNext.y / UserStitchLength)) * UserStitchLength +
		              wrap::toFloat(std::abs(rit % SEQ_TABLE[rcnt])) * userStitchLength9;
		  do {
			OSequence->push_back(F_POINT {0.0F, yVal});
			if (yVal > bCurrent.y) {
			  break;
			}
			delta.y             = OSequence->back().y - bNext.y;
			delta.x             = slope * delta.y;
			OSequence->back().x = bNext.x + delta.x;
			yVal += UserStitchLength;
		  } while (true);
		  OSequence->back() = bCurrent;
		}
		break;
	  }
	  case SEQBOT: {
		if ((form.extendedAttribute & AT_SQR) == 0U) {
		  auto yVal = wrap::toFloat(wrap::floor<int32_t>(bNext.y / UserStitchLength)) * UserStitchLength -
		              wrap::toFloat(std::abs((rit + 2) % SEQ_TABLE[rcnt])) * userStitchLength9;
		  do {
			OSequence->push_back(F_POINT {0.0F, yVal});
			if (yVal < bCurrent.y) {
			  break;
			}
			delta.y             = OSequence->back().y - bNext.y;
			delta.x             = slope * delta.y;
			OSequence->back().x = bNext.x + delta.x;
			yVal -= UserStitchLength;
		  } while (true);
		  OSequence->back() = bCurrent;
		}
		break;
	  }
	  case 0: {
		delta = F_POINT {bCurrent.x - bNext.x, bCurrent.y - bNext.y};
		StateMap->reset(StateFlag::FILDIR);
		if (auto const length = hypot(delta.x, delta.y); length != 0.0F) {
		  if (auto const userStitchLength2 = UserStitchLength * 2.0F; length > userStitchLength2) {
			auto point = bNext;
			auto count = wrap::round<uint32_t>(length / UserStitchLength - 1.0F);
			if (form::chkmax(count, wrap::toUnsigned(OSequence->size()))) {
			  return;
			}
			auto const fCount = wrap::toFloat(count);
			auto const step   = F_POINT {delta.x / fCount, delta.y / fCount};
			while (count != 0U) {
			  point.x += step.x;
			  point.y += step.y;
			  OSequence->push_back(F_POINT {point.x, point.y});
			  --count;
			}
		  }
		}
		OSequence->push_back(F_POINT {bCurrent.x, bCurrent.y});
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

auto form::filinu(F_POINT const& inPoint, F_POINT const& stitchPoint) -> F_POINT {
  auto       point  = stitchPoint;
  auto const delta  = F_POINT {(inPoint.x - stitchPoint.x), (inPoint.y - stitchPoint.y)};
  auto const length = hypot(delta.x, delta.y);
  if (auto count = wrap::round<uint32_t>(length / UserStitchLength); count != 0U) {
	if (StateMap->test(StateFlag::FILDIR)) {
	  ++count;
	}
	auto const fCount = wrap::toFloat(count);
	auto const step   = F_POINT {delta.x / fCount, delta.y / fCount};
	while (count > 0) {
	  point.x += step.x;
	  point.y += step.y;
	  OSequence->push_back(point);
	  --count;
	}
  }
  else {
	OSequence->push_back(inPoint);
  }
  return inPoint;
}

auto form::filin(F_POINT const& currentPoint, F_POINT const& stitchPoint) -> F_POINT {
  auto const delta  = F_POINT {(currentPoint.x - stitchPoint.x), (currentPoint.y - stitchPoint.y)};
  auto       point  = stitchPoint;
  auto const length = hypot(delta.x, delta.y);
  if (auto count = wrap::round<uint32_t>(length / UserStitchLength); count != 0U) {
	if (StateMap->test(StateFlag::FILDIR)) {
	  ++count;
	}
	auto const fCount = wrap::toFloat(count);
	auto const step   = F_POINT {delta.x / fCount, delta.y / fCount};
	while (count > 0) {
	  point.x += step.x;
	  point.y += step.y;
	  OSequence->push_back(point);
	  --count;
	}
  }
  else {
	OSequence->push_back(currentPoint);
  }
  return currentPoint;
}

void fi::dudif(F_POINT const& start, F_POINT const& finish, F_POINT& delta) noexcept {
  delta.x = finish.x - start.x;
  delta.y = finish.y - start.y;
}

void fi::trfrm(F_POINT const& bottomLeftPoint,
               F_POINT const& topLeftPoint,
               F_POINT const& bottomRightPoint,
               F_POINT const& topRightPoint) {
  auto topDelta    = F_POINT {};
  auto bottomDelta = F_POINT {};
  auto leftDelta   = F_POINT {};
  auto rightDelta  = F_POINT {};
  dudif(topLeftPoint, topRightPoint, topDelta);
  dudif(bottomLeftPoint, bottomRightPoint, bottomDelta);
  dudif(bottomLeftPoint, topLeftPoint, leftDelta);
  dudif(bottomRightPoint, topRightPoint, rightDelta);
  for (auto const& clip : *ClipBuffer) {
	auto const clipRatio      = F_POINT {clip.x / ClipRectSize.cx, clip.y / ClipRectSize.cy};
	auto const topMidpoint    = F_POINT {clipRatio.x * (topDelta.x) + topLeftPoint.x,
                                      clipRatio.x * (topDelta.y) + topLeftPoint.y};
	auto const bottomMidpoint = F_POINT {clipRatio.x * (bottomDelta.x) + bottomLeftPoint.x,
	                                     clipRatio.x * (bottomDelta.y) + bottomLeftPoint.y};
	auto       middleDelta    = F_POINT {};
	dudif(bottomMidpoint, topMidpoint, middleDelta);
	OSequence->push_back(F_POINT {clipRatio.y * middleDelta.x + bottomMidpoint.x,
	                              clipRatio.y * middleDelta.y + bottomMidpoint.y});
  }
}

void fi::clpfm() {
  for (auto iSequence = 0U; iSequence < wrap::toUnsigned(BSequence->size()) - 2U; iSequence += 2) {
	auto const& bSeq0 = BSequence->operator[](iSequence);
	auto const& bSeq1 = BSequence->operator[](wrap::toSize(iSequence) + 1U);
	auto const& bSeq2 = BSequence->operator[](wrap::toSize(iSequence) + 2U);
	auto const& bSeq3 = BSequence->operator[](wrap::toSize(iSequence) + 3U);

	auto const leftLength  = hypot(bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y);
	auto const rightLength = hypot(bSeq3.x - bSeq2.x, bSeq3.y - bSeq2.y);
	auto const leftDelta   = F_POINT {bSeq1.x - bSeq0.x, bSeq1.y - bSeq0.y};
	auto const rightDelta  = F_POINT {bSeq2.x - bSeq3.x, bSeq2.y - bSeq3.y};

	auto count = std::round((rightLength > leftLength) ? leftLength / ClipRectSize.cy
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
	  topLeft.x += leftStep.x;
	  topLeft.y += leftStep.y;
	  topRight.x += rightStep.x;
	  topRight.y += rightStep.y;
	  trfrm(bottomLeft, topLeft, bottomRight, topRight);
	}
  }
}

void fi::fmclp(FRM_HEAD& form) {
  auto const savedSpacing = LineSpacing;
  LineSpacing             = ClipRectSize.cx;
  StateMap->set(StateFlag::BARSAT);
  satin::satfil(form);
  StateMap->reset(StateFlag::BARSAT);
  clpfm();
  auto& currentForm    = FormList->operator[](ClosestFormToCursor);
  currentForm.fillType = CLPF;
  LineSpacing          = savedSpacing;
}

void form::refilfn() {
  auto const savedStitchLength   = UserStitchLength;
  auto       angledForm          = FRM_HEAD {};
  auto       workingFormVertices = std::vector<F_POINT> {};
  StateMap->reset(StateFlag::TXFIL);
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (form.type == FRMLINE) {
	form.underlayIndent = 0;
  }
  if (!(StateMap->test(StateFlag::FUNCLP) || StateMap->test(StateFlag::FUNSCLP))) {
	thred::savdo();
  }
  auto fillStartsData = FILL_STARTS {}; // fill start data for refill
  auto fillStartsMap  = 0U;             // fill starts bitmap
  xt::fdelstch(form, fillStartsData, fillStartsMap);
  StateMap->set(StateFlag::WASREFIL);
  constexpr auto MINSPACE = 0.5F;
  if (form.fillSpacing < MINSPACE && !form.isClip()) {
	form.fillSpacing = MINSPACE;
  }
  if (form.edgeSpacing < MINSPACE) {
	form.edgeSpacing = MINSPACE;
  }
  if (!form.isClip()) {
	UserStitchLength = form.lengthOrCount.stitchLength;
  }
  if (!(StateMap->test(StateFlag::WASDO) || StateMap->test(StateFlag::FUNCLP) ||
        StateMap->test(StateFlag::FUNSCLP))) {
	thred::savdo();
  }
  StateMap->reset(StateFlag::WASDO);
  if (((form.extendedAttribute & (AT_UND | AT_WALK)) != 0U) && form.type == FRMLINE && form.fillType != CONTF) {
	form.type = FRMFPOLY;
  }
  InterleaveSequence->clear();
  InterleaveSequenceIndices->clear();
  StateMap->reset(StateFlag::ISUND);
  auto textureSegments = std::vector<RNG_COUNT> {};
  textureSegments.resize(wrap::toSize(form.fillInfo.texture.lines));
  switch (form.type) {
	case FRMLINE: {
	  switch (form.edgeType & NEGUND) {
		case EDGELINE: {
		  fi::brdfil(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEBEAN: {
		  fi::bold(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGECLIP: {
		  auto clipRect = F_RECTANGLE {};
		  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		  clip::clpout(form.borderSize);
		  clip::clpbrd(form, clipRect, 0);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEANGSAT: {
		  StateMap->reset(StateFlag::SAT1);
		  satin::slbrd(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEPROPSAT: {
		  if (form.vertexCount > 2) {
			StateMap->reset(StateFlag::SAT1);
			fi::plbrd(form, angledForm, *AngledFormVertices);
			fi::ritbrd(form);
		  }
		  break;
		}
		case EDGEAPPL: {
		  fi::lapbrd(form);
		  fi::ritapbrd();
		  StateMap->reset(StateFlag::SAT1);
		  satin::slbrd(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEBHOL: {
		  auto const length      = ButtonholeCornerLength;
		  ButtonholeCornerLength = form::getblen();
		  satin::satout(form, BHWIDTH);
		  fi::blbrd(form);
		  ButtonholeCornerLength = length;
		  fi::ritbrd(form);
		  break;
		}
		case EDGEPICOT: {
		  auto clipRect = F_RECTANGLE {};
		  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		  auto const length      = ButtonholeCornerLength;
		  ButtonholeCornerLength = form::getplen();
		  clip::clpic(form, clipRect);
		  ButtonholeCornerLength = length;
		  fi::ritbrd(form);
		  break;
		}
		case EDGEDOUBLE: {
		  fi::dubfn(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGELCHAIN: {
		  StateMap->set(StateFlag::LINCHN);
		  clip::chnfn(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGEOCHAIN: {
		  StateMap->reset(StateFlag::LINCHN);
		  clip::chnfn(form);
		  fi::ritbrd(form);
		  break;
		}
		case EDGECLIPX: {
		  auto clipRect = F_RECTANGLE {};
		  clip::oclp(clipRect, form.borderClipData, form.clipEntries);
		  clip::duxclp(form);
		  fi::ritbrd(form);
		  break;
		}
		default: {
		  outDebugString(L"default hit in refilfn 1: edgeType [{}]\n", form.edgeType & NEGUND);
		  break;
		}
	  }
	  if (form.fillType == CONTF && ((form.attribute & FRECONT) != 0)) {
		fi::contf(form);
		fi::ritfil();
	  }
	  break;
	}
	case FRMFPOLY: {
	  xt::chkcwlk(form);
	  xt::chkwlk(form);
	  xt::chkund(form, textureSegments, *AngledFormVertices);
	  StateMap->reset(StateFlag::ISUND);
	  if (form.fillType != 0U) {
		auto const spacing      = LineSpacing;
		LineSpacing             = form.fillSpacing;
		auto lineEndpoints      = std::vector<SMAL_PNT_L> {};
		auto groupIndexSequence = std::vector<uint32_t> {};
		auto rotationCenter     = F_POINT {};
		auto doFill             = true;
		auto rotationAngle      = 0.0F;
		switch (form.fillType) {
		  case VRTF: {
			workingFormVertices.clear();
			workingFormVertices.reserve(form.vertexCount);
			auto itStartVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
			fi::fnvrt(workingFormVertices, groupIndexSequence, lineEndpoints);
			break;
		  }
		  case HORF: {
			rotationAngle = PI_FHALF;
			fi::fnhor(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, *AngledFormVertices);
			workingFormVertices.clear();
			workingFormVertices.reserve(angledForm.vertexCount);
			auto itStartVertex = wrap::next(AngledFormVertices->cbegin(), angledForm.vertexIndex);
			auto itEndVertex   = wrap::next(itStartVertex, angledForm.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
			break;
		  }
		  case ANGF: {
			rotationAngle = PI_FHALF - form.angleOrClipData.angle;
			fi::fnang(groupIndexSequence, lineEndpoints, rotationAngle, rotationCenter, angledForm, *AngledFormVertices);
			workingFormVertices.clear();
			workingFormVertices.reserve(angledForm.vertexCount);
			auto itStartVertex = wrap::next(AngledFormVertices->cbegin(), angledForm.vertexIndex);
			auto itEndVertex   = wrap::next(itStartVertex, angledForm.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
			break;
		  }
		  case VCLPF: {
			auto clipRect = F_RECTANGLE {};
			clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
			workingFormVertices.clear();
			workingFormVertices.reserve(form.vertexCount);
			auto itStartVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
			fi::clpcon(form, textureSegments, workingFormVertices);
			doFill = false;
			break;
		  }
		  case HCLPF: {
			auto clipRect = F_RECTANGLE {};
			clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
			fi::horclpfn(textureSegments, angledForm, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  case ANGCLPF: {
			auto clipRect = F_RECTANGLE {};
			clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
			StateMap->reset(StateFlag::ISUND);
			form::angclpfn(form, textureSegments, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  case TXVRTF: {
			texture::setxt(form, textureSegments);
			workingFormVertices.clear();
			workingFormVertices.reserve(form.vertexCount);
			auto itStartVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
			auto itEndVertex   = wrap::next(itStartVertex, form.vertexCount);
			workingFormVertices.insert(workingFormVertices.end(), itStartVertex, itEndVertex);
			fi::clpcon(form, textureSegments, workingFormVertices);
			doFill = false;
			break;
		  }
		  case TXHORF: {
			texture::setxt(form, textureSegments);
			fi::horclpfn(textureSegments, angledForm, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  case TXANGF: {
			texture::setxt(form, textureSegments);
			StateMap->reset(StateFlag::ISUND);
			form::angclpfn(form, textureSegments, *AngledFormVertices);
			doFill = false;
			break;
		  }
		  default: {
			outDebugString(L"default hit in refilfn 2: fillType [{}]\n", form.fillType);
			break;
		  }
		}
		if (doFill) {
		  fi::lcon(form, groupIndexSequence, lineEndpoints, workingFormVertices);
		  fi::bakseq();
		  if (form.fillType != VRTF && form.fillType != TXVRTF) {
			rotationAngle = -rotationAngle;
			fi::rotbak(rotationAngle, rotationCenter);
		  }
		}
		fi::ritfil();
		LineSpacing = spacing;
	  }
	  fi::chkbrd(form);
	  break;
	}
	case SAT: {
	  xt::chkcwlk(form);
	  xt::chkwlk(form);
	  xt::chkund(form, textureSegments, *AngledFormVertices);
	  StateMap->reset(StateFlag::ISUND);
	  switch (form.fillType) {
		case SATF: {
		  auto const spacing = LineSpacing;
		  LineSpacing        = form.fillSpacing;
		  UserStitchLength   = form.lengthOrCount.stitchLength;
		  satin::satfil(form);
		  LineSpacing = spacing;
		  fi::ritfil();
		  break;
		}
		case CLPF: {
		  auto clipRect = F_RECTANGLE {};
		  clip::oclp(clipRect, form.angleOrClipData.clip, form.lengthOrCount.clipCount);
		  fi::fmclp(form);
		  fi::ritfil();
		  break;
		}
		case FTHF: {
		  StateMap->set(StateFlag::CNV2FTH);
		  xt::fthrfn(form);
		  break;
		}
		default: {
		  outDebugString(L"default hit in refilfn 3: fillType [{}]\n", form.fillType);
		  break;
		}
	  }
	  fi::chkbrd(form);
	  break;
	}
	default: {
	  outDebugString(L"default hit in refilfn 4: type [{}]\n", form.type);
	  break;
	}
  }
  UserStitchLength = savedStitchLength;
  xt::intlv(form, fillStartsData, fillStartsMap);
  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
  xt::setfchk();
}

void form::refil() {
  if (!UserFlagMap->test(UserFlag::WRNOF)) {
	if (auto const codedForm = ClosestFormToCursor << FRMSHFT | USMSK;
	    std::ranges::any_of(*StitchBuffer, [&codedForm](F_POINT_ATTR const& stitch) -> bool {
	      return ((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & (USMSK | FRMSK)) == codedForm;
	    })) {
	  if (FormDataSheet != nullptr) {
		StateMap->set(StateFlag::WASFRMFRM);
	  }
	  thred::undat();
	  if (nullptr == MsgWindow) {
		displayText::tabmsg(IDS_REFIL, true);
		displayText::okcan();
	  }
	  StateMap->set(StateFlag::MOVMSG);
	  return;
	}
  }
  form::refilfn();
}

void form::setfpnt() {
  // clang-format off
  auto&      form             = FormList->operator[](ClosestFormToCursor);
  auto const screenCoordinate = POINT {(Msg.pt.x - StitchWindowOrigin.x), 
									   (Msg.pt.y - StitchWindowOrigin.y)};
  auto const itVertex         = wrap::next(FormVertices->begin(), form.vertexIndex + ClosestVertexToCursor);
  // clang-format on
  form::unfrm();
  fi::rats();
  *itVertex = fi::px2stchf(screenCoordinate);
  form.outline();
  refil();
  StateMap->set(StateFlag::WASFPNT);
  StateMap->reset(StateFlag::SELBOX);
  StateMap->set(StateFlag::FRMPSEL);
  thred::ritfcor(*itVertex);
  StateMap->set(StateFlag::RESTCH);
}

void fi::makpoli() {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (currentForm.type == SAT) {
	if (currentForm.satinGuideCount != 0U) {
	  satin::delsac(ClosestFormToCursor);
	}
  }
  currentForm.type = FRMFPOLY;
}

void fi::fsvrt() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type = FRMFPOLY;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType    = VRTF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form.squareEnd(UserFlagMap->test(UserFlag::SQRFIL));
  form::refilfn();
}

void form::filvrt() {
  if (displayText::filmsgs(FMM_VRT)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  if (auto const& currentForm = FormList->operator[](ClosestFormToCursor); currentForm.type != FRMLINE) {
		fi::fsvrt();
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::fsvrt();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void fi::fshor(FRM_HEAD& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type = FRMFPOLY;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType    = HORF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form.angleOrClipData.angle = PI_FHALF;
  form.squareEnd(UserFlagMap->test(UserFlag::SQRFIL));
  form::refil();
}

void form::filhor() {
  if (displayText::filmsgs(FMM_HOR)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  if (auto& form = FormList->operator[](ClosestFormToCursor); form.type != FRMLINE) {
		fi::fshor(form);
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  fi::fshor(form);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void fi::fsangl(FRM_HEAD& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  makpoli();
  form.type = FRMFPOLY;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType              = ANGF;
  form.angleOrClipData.angle = IniFile.fillAngle;
  form.fillSpacing           = LineSpacing;
  form::fsizpar(form);
  form.squareEnd(UserFlagMap->test(UserFlag::SQRFIL));
  form::refil();
}

void form::filangl() {
  if (displayText::filmsgs(FMM_ANG)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  if (auto& form = FormList->operator[](selectedForm); form.type != FRMLINE) {
		fi::fsangl(form);
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  fi::fsangl(form);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

auto form::chkfrm(gsl::not_null<std::vector<POINT>*> formControlPoints,
                  std::vector<POINT>&                stretchBoxLine,
                  float&                             xyRatio) -> bool {
  auto const  point = POINT {(Msg.pt.x - StitchWindowOrigin.x), (Msg.pt.y - StitchWindowOrigin.y)};
  auto const& currentForm = FormList->operator[](ClosestFormToCursor);
  NewFormVertexCount      = currentForm.vertexCount + 1U;
  thred::duzrat();
  auto const rectangle    = form::sRct2px(currentForm.rectangle);
  auto&      formControls = *formControlPoints;
  formControls[0].x = formControls[6].x = formControls[7].x = formControls[8].x = rectangle.left;
  formControls[0].y = formControls[1].y = formControls[2].y = formControls[8].y = rectangle.top;
  formControls[2].x = formControls[3].x = formControls[4].x = rectangle.right;
  formControls[4].y = formControls[5].y = formControls[6].y = rectangle.bottom;
  formControls[1].x = formControls[5].x = std::lround(wrap::midl(rectangle.right, rectangle.left));
  formControls[3].y = formControls[7].y = std::lround(wrap::midl(rectangle.top, rectangle.bottom));

  auto minimumLength    = BIGDBL;
  auto formControlIndex = 0U;
  for (auto const& iControl : formControls) {
	if (auto const length = hypot(iControl.x - point.x, iControl.y - point.y); length < minimumLength) {
	  minimumLength             = length;
	  SelectedFormControlVertex = formControlIndex;
	}
	if (minimumLength < CLOSENUF) {
	  form::ritfrct(ClosestFormToCursor, StitchWindowDC);
	  for (auto iCorner = 0U; iCorner < wrap::toUnsigned(stretchBoxLine.size()); ++iCorner) {
		stretchBoxLine[iCorner] = formControls[wrap::toSize(iCorner) * 2U];
	  }
	  stretchBoxLine.back() = stretchBoxLine.front();
	  thred::strtchbox(stretchBoxLine);
	  if ((SelectedFormControlVertex & 1U) != 0U) {
		StateMap->set(StateFlag::STRTCH);
	  }
	  else {
		StateMap->set(StateFlag::EXPAND);
		xyRatio = (currentForm.rectangle.right - currentForm.rectangle.left) /
		          (currentForm.rectangle.top - currentForm.rectangle.bottom);
	  }
	  SelectedFormControlVertex >>= 1U;
	  StateMap->set(StateFlag::SHOSTRTCH);
	  return true;
	}
	++formControlIndex;
  }
  SelectedFormControlVertex >>= 1U;
  if (point.x >= rectangle.left && point.x <= rectangle.right && point.y >= rectangle.top &&
      point.y <= rectangle.bottom) {
	auto const itVertex   = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex);
	auto const formOrigin = form::sfCor2px(*itVertex);
	FormMoveDelta         = F_POINT {formOrigin.x - point.x, formOrigin.y - point.y};
	StateMap->set(StateFlag::FRMOV);
	return true;
  }
  return false;
}

void form::rstfrm() {
  auto const point =
      POINT {(Msg.pt.x + std::lround(FormMoveDelta.x)), (Msg.pt.y + std::lround(FormMoveDelta.y))};

  auto const attribute = ClosestFormToCursor << 4U;
  form::setmfrm();
  StateMap->reset(StateFlag::FRMOV);
  auto const stitchPoint = thred::pxCor2stch(point);
  auto& currentForm      = FormList->operator[](ClosestFormToCursor);
  auto       itVertex    = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  auto const offset      = F_POINT {stitchPoint.x - itVertex->x, stitchPoint.y - itVertex->y};
  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; ++iVertex) {
	itVertex->x += offset.x;
	itVertex->y += offset.y;
	++itVertex;
  }
  currentForm.rectangle.left += offset.x;
  currentForm.rectangle.top += offset.y;
  currentForm.rectangle.right += offset.x;
  currentForm.rectangle.bottom += offset.y;
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & FRMSK) == attribute && ((stitch.attribute & ALTYPMSK) != 0U) &&
	    ((stitch.attribute & NOTFRM) == 0U)) {
	  stitch.x += offset.x;
	  stitch.y += offset.y;
	}
  }
}

void form::clrfills() noexcept {
  for (auto& formIter : *FormList) {
	formIter.clipEntries             = 0;
	formIter.lengthOrCount.clipCount = 0;
	formIter.edgeType                = 0;
	formIter.fillType                = 0;
	formIter.attribute &= NFRECONT;
	formIter.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
  }
  ClipPoints->clear();
}

void fi::ducon() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, FormLines->data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void fi::uncon() {
  if (StateMap->testAndReset(StateFlag::SHOCON)) {
	ducon();
  }
}

void form::drwcon() {
  fi::uncon();
  auto& formLines = *FormLines;
  formLines[1].x  = Msg.pt.x - StitchWindowOrigin.x;
  formLines[1].y  = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::SHOCON);
  fi::ducon();
}

void form::duinsf() noexcept {
  SetROP2(StitchWindowDC, R2_XORPEN);
  SelectObject(StitchWindowDC, FormPen);
  wrap::polyline(StitchWindowDC, InsertLine.data(), LNPNTS);
  SetROP2(StitchWindowDC, R2_COPYPEN);
}

void form::uninsf() {
  if (StateMap->testAndReset(StateFlag::SHOINSF)) {
	form::duinsf();
  }
}

void form::rotfrm(FRM_HEAD& form, uint32_t newStartVertex) {
  auto rotatedVertices = std::vector<F_POINT> {};
  rotatedVertices.resize(form.vertexCount);
  auto iRotated = newStartVertex;
  auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
  std::copy(itVertex, wrap::next(itVertex, form.vertexCount), rotatedVertices.begin());
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex = rotatedVertices[iRotated];
	iRotated  = form::nxt(form, iRotated);
	++itVertex;
  }
  auto const itStartGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
  auto       rotatedIt    = itStartGuide;
  if (form.type == SAT) {
	if (form.vertexCount != 0U) {
	  if (form.wordParam != 0U) {
		form.wordParam = (form.wordParam + form.vertexCount - newStartVertex) % form.vertexCount;
	  }
	  auto itGuide = itStartGuide;
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
	}
  }
  if (rotatedIt != itStartGuide) {
	form.satinGuideCount = wrap::distance<uint32_t>(itStartGuide, rotatedIt);
	std::sort(itStartGuide, rotatedIt, satin::scomp);
  }
  if (form.vertexCount != 0U) {
	if ((form.extendedAttribute & AT_STRT) != 0U) {
	  form.fillStart = (form.fillStart + form.vertexCount - newStartVertex) % form.vertexCount;
	}
	if ((form.extendedAttribute & AT_END) != 0U) {
	  form.fillEnd = (form.fillEnd + form.vertexCount - newStartVertex) % form.vertexCount;
	}
  }
}

void fi::filsfn() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.type = SAT;
  wrap::narrow(form.fillColor, ActiveColor);
  form.fillType    = SATF;
  form.fillSpacing = LineSpacing;
  form::fsizpar(form);
  form::refilfn();
}

void form::filsat() {
  if (displayText::filmsgs(FMM_FAN)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  if (auto const& currentForm = FormList->operator[](ClosestFormToCursor); currentForm.type != FRMLINE) {
		fi::filsfn();
	  }
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::filsfn();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

auto fi::closat(IntersectionStyles& inOutFlag) -> bool {
  auto       minimumLength = BIGFLOAT;
  auto const stitchPoint   = thred::pxCor2stch(Msg.pt);
  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	if (auto& formIter = FormList->operator[](iForm); formIter.vertexCount != 0U) {
	  if (auto const layer =
	          gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formIter.attribute & FRMLMSK) >> 1U);
	      (ActiveLayer == 0U) || layer == ActiveLayer || ((formIter.attribute & FRMLMSK) == 0U)) {
		auto const lastVertex = (formIter.type == FRMLINE) ? formIter.vertexCount - 1U : formIter.vertexCount;
		// Loop through for all line segments
		auto       length   = 0.0F;
		auto const itVertex = wrap::next(FormVertices->cbegin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < lastVertex; ++iVertex) {
		  auto const itCurrentVertex = wrap::next(itVertex, iVertex);
		  auto const itNextVertex    = wrap::next(itVertex, form::nxt(formIter, iVertex));
		  auto const param = findDistanceToSide(*itCurrentVertex, *itNextVertex, stitchPoint, length);
		  if ((length < minimumLength)) {
			if ((param < 0.0F) && (iVertex == 0)) {
			  // this should only happen if the Closest vertex is the start of a line (vertex 0)
			  minimumLength         = length;
			  ClosestFormToCursor   = iForm;
			  ClosestVertexToCursor = iVertex;
			  inOutFlag             = IntersectionStyles::POINT_BEFORE_LINE;
			}
			else {
			  // return the vertex after the intersection
			  if ((param > 1.0F) && (iVertex == lastVertex - 1)) {
				minimumLength         = length;
				ClosestFormToCursor   = iForm;
				ClosestVertexToCursor = form::nxt(formIter, iVertex);
				inOutFlag             = IntersectionStyles::POINT_AFTER_LINE;
			  }
			  else {
				minimumLength         = length;
				ClosestFormToCursor   = iForm;
				ClosestVertexToCursor = form::nxt(formIter, iVertex);
				inOutFlag             = IntersectionStyles::POINT_IN_LINE;
			  }
			}
		  }
		}
	  }
	}
	else {
	  outDebugString(L"closat: Form Has no vertices!\n inOutFlag[{}]", gsl::narrow_cast<int>(inOutFlag));
	}
  }
  return !util::closeEnough(minimumLength, BIGFLOAT);
}

void fi::nufpnt(uint32_t vertex, FRM_HEAD& form, F_POINT stitchPoint) {
  form::fltspac(vertex + 1U, 1U);
  ++(form.vertexCount);
  auto const itVertex = wrap::next(FormVertices->begin(), form.vertexIndex + vertex + 1U);
  *itVertex           = stitchPoint;
  if (form.satinGuideCount != 0U) {
	auto itGuide = wrap::next(SatinGuides->begin(), form.satinOrAngle.guide);
	for (auto ind = 0U; ind < form.satinGuideCount; ++ind) {
	  if (itGuide->start > vertex) {
		++(itGuide->start);
	  }
	  if (itGuide->finish > vertex) {
		++(itGuide->finish);
	  }
	  ++itGuide;
	}
  }
  if (form.wordParam >= vertex + 1U) {
	++(form.wordParam);
	form.wordParam %= form.vertexCount;
  }
  if (form.fillType == CONTF) {
	if (form.angleOrClipData.guide.start > vertex) {
	  ++(form.angleOrClipData.guide.start);
	}
	if (form.angleOrClipData.guide.finish > vertex) {
	  ++(form.angleOrClipData.guide.finish);
	}
  }
  form::frmlin(form);
}

void form::insat() { // insert a point in a form
  if (auto inOutFlag = IntersectionStyles::POINT_IN_LINE; fi::closat(inOutFlag)) {
	thred::savdo();
	// clang-format off
	auto&      selectedForm = FormList->operator[](ClosestFormToCursor);
	auto const stitchPoint  = thred::pxCor2stch(Msg.pt);
	auto const lastVertex   = selectedForm.vertexCount - 1U;
	// clang-format on
	if (inOutFlag != IntersectionStyles::POINT_IN_LINE) {
	  if (ClosestVertexToCursor == 0 && selectedForm.type == FRMLINE) {
		StateMap->set(StateFlag::PRELIN);
	  }
	  else {
		if (ClosestVertexToCursor != lastVertex && selectedForm.type == FRMLINE) {
		  ClosestVertexToCursor = form::prv(selectedForm, ClosestVertexToCursor);
		}
	  }
	  fi::nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	  auto const itVertex = wrap::next(FormVertices->begin(), selectedForm.vertexIndex);
	  if (StateMap->testAndReset(StateFlag::PRELIN)) {
		std::swap(itVertex[0], itVertex[1]);
	  }
	}
	else {
	  ClosestVertexToCursor = form::prv(selectedForm, ClosestVertexToCursor);
	  fi::nufpnt(ClosestVertexToCursor, selectedForm, stitchPoint);
	}
	form::refil();
  }
  StateMap->set(StateFlag::RESTCH);
}

auto form::chkdel(FRM_HEAD const& currentForm) noexcept -> bool {
  if (currentForm.type == FRMLINE) {
	return currentForm.vertexCount <= 2;
  }

  return currentForm.vertexCount <= 3;
}

void form::unfil() {
  if (displayText::filmsgs(FMX_UNF)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto const selectedForm : (*SelectedFormList)) {
	  if (auto& form = FormList->operator[](selectedForm); (form.fillType != 0U) || (form.edgeType != 0U)) {
		clip::delclps(selectedForm);
		texture::deltx(selectedForm);
		formMap.set(selectedForm);
		form.fillType = 0;
		form.edgeType = 0;
		form.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	  }
	}
	auto iDestination = StitchBuffer->begin();
	for (auto const& stitch : *StitchBuffer) {
	  if (!formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
		*iDestination++ = stitch;
	  }
	}
	StitchBuffer->resize(wrap::distance<size_t>(StitchBuffer->begin(), iDestination));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  if (!StateMap->testAndReset(StateFlag::IGNOR) && !UserFlagMap->test(UserFlag::WRNOF)) {
		if (auto const codedForm = (ClosestFormToCursor << FRMSHFT) | USMSK;
		    std::ranges::any_of(*StitchBuffer, [&codedForm](F_POINT_ATTR const& stitch) -> bool {
		      return ((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & (USMSK | FRMSK)) == codedForm;
		    })) {
		  displayText::tabmsg(IDS_UNFIL, true);
		  StateMap->set(StateFlag::FILMSG);
		  displayText::okcan();
		  StateMap->set(StateFlag::IGNOR);
		  return;
		}
	  }
	  if (!StitchBuffer->empty()) {
		auto       iDestination = StitchBuffer->begin();
		auto       destCount    = 0U;
		auto const codedForm    = ClosestFormToCursor << FRMSHFT;
		for (auto const& stitch : *StitchBuffer) {
		  if ((stitch.attribute & FRMSK) != codedForm || ((stitch.attribute & NOTFRM) != 0U)) {
			*iDestination++ = stitch;
			++destCount;
		  }
		}
		StitchBuffer->resize(destCount);
	  }
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  clip::delclps(ClosestFormToCursor);
	  texture::deltx(ClosestFormToCursor);
	  form.fillType = 0;
	  form.edgeType = 0;
	  form.extendedAttribute &= ~(AT_UND | AT_CWLK | AT_WALK);
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	}
  }
}

void form::frm0() {
  if (StateMap->test(StateFlag::FRMPSEL)) {
	auto& form = FormList->operator[](ClosestFormToCursor);
	thred::savdo();
	form::rotfrm(form, ClosestVertexToCursor);
	ClosestVertexToCursor = 0;
	satin::satadj(form);
	form::refil();
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::rinfrm() {
  form::frmlin(*FormForInsert);
  SelectObject(StitchWindowMemDC, FormPen);
  SetROP2(StitchWindowMemDC, R2_XORPEN);
  auto const& formLines = *FormLines;
  if ((FormVertexNext != 0U) || FormForInsert->type != FRMLINE) {
	wrap::polyline(StitchWindowMemDC, &formLines[FormVertexPrev], LNPNTS);
  }
  InsertLine[0]   = formLines[FormVertexPrev];
  InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
  InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::SHOINSF);
  form::duinsf();
}

void form::infrm() { // insert multiple points into a form
  if (auto inOutFlag = IntersectionStyles::POINT_IN_LINE; fi::closat(inOutFlag)) {
	FormForInsert = &(FormList->operator[](ClosestFormToCursor));
	if (inOutFlag != IntersectionStyles::POINT_IN_LINE) {
	  if ((ClosestVertexToCursor == 0U) && FormForInsert->type == FRMLINE) {
		FormVertexPrev = 0;
		StateMap->set(StateFlag::PRELIN);
	  }
	  else {
		FormVertexPrev = form::prv(*FormForInsert, ClosestVertexToCursor);
		FormVertexNext = ClosestVertexToCursor;
	  }
	}
	else {
	  FormVertexNext = ClosestVertexToCursor;
	  FormVertexPrev = form::prv(*FormForInsert, ClosestVertexToCursor);
	}
	StateMap->set(StateFlag::INSFRM);
	StateMap->set(StateFlag::INIT);
	rinfrm();
  }
}

void form::setins() {
  auto const stitchPoint = thred::pxCor2stch(Msg.pt);
  fi::nufpnt(FormVertexPrev, *FormForInsert, stitchPoint);
  if (StateMap->test(StateFlag::PRELIN)) {
	auto const itVertex = wrap::next(FormVertices->begin(), FormForInsert->vertexIndex);
	std::swap(itVertex[0], itVertex[1]);
  }
  else {
	FormVertexPrev = form::nxt(*FormForInsert, FormVertexPrev);
	FormVertexNext = form::nxt(*FormForInsert, FormVertexPrev);
  }
  form::frmlin(*FormForInsert);
  InsertLine[0]   = FormLines->operator[](FormVertexPrev);
  InsertLine[1].x = Msg.pt.x - StitchWindowOrigin.x;
  InsertLine[1].y = Msg.pt.y - StitchWindowOrigin.y;
  StateMap->set(StateFlag::INSFRM);
  duinsf();
  StateMap->set(StateFlag::RESTCH);
}

void form::bsizpar(FRM_HEAD& currentForm) noexcept {
  currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
  currentForm.edgeStitchLen      = UserStitchLength;
  currentForm.minBorderStitchLen = MinStitchLength;
}

void fi::sbord(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGELINE;
  form::bsizpar(form);
  form::refilfn();
}

void form::bord() {
  if (displayText::filmsgs(FML_LIN)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  wrap::narrow(form.borderColor, ActiveColor);
	  fi::sbord(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  wrap::narrow(form.borderColor, ActiveColor);
	  fi::sbord(ClosestFormToCursor);
	  thred::coltab();
	  StateMap->set(StateFlag::INIT);
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void fi::fsclp(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  form.edgeType       = EDGECLIP;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = ClipRectSize.cx;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  auto itClipPoint = wrap::next(ClipPoints->begin(), form.borderClipData);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  clip::clpout(ClipRectSize.cy / 2);
  form::refilfn();
}

void form::fclp() {
  if (displayText::filmsgs(FML_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::savdo();
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fi::fsclp(selectedForm);
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::fsclp(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void fi::sapliq(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGEAPPL;
  if (UserFlagMap->test(UserFlag::DUND)) {
	form.edgeType |= EGUND;
  }
  form.edgeSpacing = LineSpacing / 2.0F;
  form.borderSize  = IniFile.borderWidth;
  form::bsizpar(form);
  wrap::narrow_cast(form.borderColor, ActiveColor | gsl::narrow_cast<uint8_t>(AppliqueColor << 4U));
  if (form.type != FRMLINE) {
	if (form.fillType == SAT && (form.satinGuideCount != 0U)) {
	  satin::delsac(ClosestFormToCursor);
	}
  }
  form.fillType = 0U;
  form::refilfn();
}

void form::apliq() {
  if (displayText::filmsgs(FML_APLQ)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  if (auto& currentForm = FormList->operator[](ClosestFormToCursor); UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::sapliq(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  if (auto& currentForm = FormList->operator[](ClosestFormToCursor); UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::sapliq(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::setap() {
  AppliqueColor = ActiveColor;
  // NOLINTNEXTLINE(clang-diagnostic-sign-conversion)
  displayText::showMessage(IDS_APCOL, (AppliqueColor + 1U));
}

auto fi::getbig(std::vector<FRM_HEAD> const& formList, std::vector<F_POINT_ATTR> const& stitchBuffer) noexcept
    -> F_RECTANGLE {
  auto allItemsRect = F_RECTANGLE {BIGFLOAT, 0.0F, 0.0F, BIGFLOAT};
  for (auto const& iForm : formList) {
	auto const& trct = iForm.rectangle;
	if (trct.left < allItemsRect.left) {
	  allItemsRect.left = trct.left;
	}
	if (trct.top > allItemsRect.top) {
	  allItemsRect.top = trct.top;
	}
	if (trct.right > allItemsRect.right) {
	  allItemsRect.right = trct.right;
	}
	if (trct.bottom < allItemsRect.bottom) {
	  allItemsRect.bottom = trct.bottom;
	}
  }
  for (auto const& stitch : stitchBuffer) {
	if (stitch.x < allItemsRect.left) {
	  allItemsRect.left = stitch.x;
	}
	if (stitch.y > allItemsRect.top) {
	  allItemsRect.top = stitch.y;
	}
	if (stitch.x > allItemsRect.right) {
	  allItemsRect.right = stitch.x;
	}
	if (stitch.y < allItemsRect.bottom) {
	  allItemsRect.bottom = stitch.y;
	}
  }
  return allItemsRect;
}

void form::stchrct2px(gsl::not_null<F_RECTANGLE const*> stitchRect, RECT& screenRect) {
  auto stitchCoord = F_POINT {stitchRect->left, stitchRect->top};
  auto screenCoord = POINT {};
  thred::sCor2px(stitchCoord, screenCoord);
  screenRect.left = screenCoord.x;
  screenRect.top  = screenCoord.y;
  stitchCoord.x   = stitchRect->right;
  stitchCoord.y   = stitchRect->bottom;
  thred::sCor2px(stitchCoord, screenCoord);
  screenRect.right  = screenCoord.x;
  screenRect.bottom = screenCoord.y;
}

void form::selal() {
  StateMap->reset(StateFlag::FORMSEL);
  SelectedFormList->clear();
  StateMap->reset(StateFlag::SELBOX);
  StateMap->reset(StateFlag::GRPSEL);
  *AllItemsRect = fi::getbig(*FormList, *StitchBuffer);
  ZoomRect = F_RECTANGLE {0.0F, wrap::toFloat(UnzoomedRect.cy), wrap::toFloat(UnzoomedRect.cx), 0.0F};
  ZoomFactor = 1;
  StateMap->reset(StateFlag::ZUMED);
  thred::movStch();
  NearestCount = 0;
  StateMap->reset(StateFlag::RUNPAT);
  thred::duzrat();
  form::stchrct2px(AllItemsRect, SelectedFormsRect);
  StateMap->set(StateFlag::BIGBOX);
  StateMap->set(StateFlag::RESTCH);
}

void form::setstrtch() {
  auto reference   = 0.0F;
  auto ratio       = 1.0F;
  auto stitchRect  = F_RECTANGLE {};
  auto stitchPoint = F_POINT {};
  thred::savdo();
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	stitchRect = SelectedVerticesRect;
  }
  else {
	if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX)) {
	  form::pxrct2stch(SelectedFormsRect, stitchRect);
	  stitchPoint = F_POINT {stitchRect.right, stitchRect.bottom};
	}
	else {
	  stitchPoint = thred::pxCor2stch(Msg.pt);
	}
  }
  switch (SelectedFormControlVertex) {
	case 0: { // top control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.bottom;
		auto const offsetY = Msg.pt.y - StitchWindowOrigin.y;
		ratio              = wrap::toFloat(SelectedFormsRect.bottom - offsetY) /
		        wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  reference          = form.rectangle.bottom;
		  ratio              = (stitchPoint.y - reference) / (form.rectangle.top - reference);
		  form.rectangle.top = stitchPoint.y;
		}
		else {
		  reference = StitchRangeRect.bottom;
		  ratio     = (stitchPoint.y - reference) / (StitchRangeRect.top - reference);
		}
	  }
	  break;
	}
	case 1: { // right control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.left;
		auto const offsetX = Msg.pt.x - StitchWindowOrigin.x;
		ratio              = wrap::toFloat(offsetX - SelectedFormsRect.left) /
		        wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  auto& formRectangle = form.rectangle;
		  reference           = formRectangle.left;
		  ratio               = (stitchPoint.x - reference) / (formRectangle.right - reference);
		  formRectangle.right = stitchPoint.x;
		}
		else {
		  reference = StitchRangeRect.left;
		  ratio     = (stitchPoint.x - reference) / (StitchRangeRect.right - reference);
		}
	  }
	  break;
	}
	case 2: { // bottom control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.top;
		auto const offsetY = Msg.pt.y - StitchWindowOrigin.y;
		ratio              = wrap::toFloat(offsetY - SelectedFormsRect.top) /
		        wrap::toFloat(SelectedFormsRect.bottom - SelectedFormsRect.top);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  reference             = form.rectangle.top;
		  ratio                 = (stitchPoint.y - reference) / (form.rectangle.bottom - reference);
		  form.rectangle.bottom = stitchPoint.y;
		}
		else {
		  reference = StitchRangeRect.top;
		  ratio     = (stitchPoint.y - reference) / (StitchRangeRect.bottom - reference);
		}
	  }
	  break;
	}
	case 3: { // left control
	  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
		reference          = stitchRect.right;
		auto const offsetX = Msg.pt.x - StitchWindowOrigin.x;
		ratio              = wrap::toFloat(SelectedFormsRect.right - offsetX) /
		        wrap::toFloat(SelectedFormsRect.right - SelectedFormsRect.left);
	  }
	  else {
		if (StateMap->test(StateFlag::FORMSEL)) {
		  reference           = form.rectangle.right;
		  ratio               = (stitchPoint.x - reference) / (form.rectangle.left - reference);
		  form.rectangle.left = stitchPoint.x;
		}
		else {
		  reference = StitchRangeRect.right;
		  ratio     = (stitchPoint.x - reference) / (StitchRangeRect.left - reference);
		}
	  }
	  break;
	}
	default: {
	  throw std::runtime_error("no valid control selected"); // we should never reach here
	}
  }
  if ((SelectedFormControlVertex & 1U) != 0U) { // right or left control selected
	if (StateMap->test(StateFlag::FPSEL)) {
	  auto currentVertex = SelectedFormVertices.start;
	  auto itVertex      = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const itCurrentVertex = wrap::next(itVertex, currentVertex);

		itCurrentVertex->x = (itCurrentVertex->x - reference) * ratio + reference;
		currentVertex      = pdir(form, currentVertex);
	  }
	  form.outline();
	  thred::setpsel();
	  refil();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		auto& formIter = FormList->operator[](iForm);
		auto itVertex  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->x = (itVertex->x - reference) * ratio + reference;
		  ++itVertex;
		}
		formIter.outline();
	  }
	  for (auto& stitch : *StitchBuffer) {
		stitch.x = (stitch.x - reference) * ratio + reference;
	  }
	  selal();
	  return;
	}

	if (!SelectedFormList->empty()) {
	  for (auto selectedForm : (*SelectedFormList)) {
		auto& formIter = FormList->operator[](selectedForm);
		auto itVertex  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->x = (itVertex->x - reference) * ratio + reference;
		  ++itVertex;
		}
	  }
	}
	else {
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		  itVertex->x = (itVertex->x - reference) * ratio + reference;
		  ++itVertex;
		}
	  }
	  else {
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).x =
		      (StitchBuffer->operator[](iStitch).x - reference) * ratio + reference;
		}
	  }
	}
  }
  else {
	if (StateMap->test(StateFlag::FPSEL)) {
	  auto currentVertex = SelectedFormVertices.start;
	  auto itVertex      = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
		auto const itCurrentVertex = wrap::next(itVertex, currentVertex);

		itCurrentVertex->y = (itCurrentVertex->y - reference) * ratio + reference;
		currentVertex      = pdir(form, currentVertex);
	  }
	  FormList->operator[](ClosestFormToCursor).outline();
	  thred::setpsel();
	  refil();
	  StateMap->set(StateFlag::RESTCH);
	  return;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
		auto& formIter = FormList->operator[](iForm);
		auto itVertex  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->y = (itVertex->y - reference) * ratio + reference;
		  ++itVertex;
		}
		formIter.outline();
	  }
	  for (auto& stitch : *StitchBuffer) {
		stitch.y = (stitch.y - reference) * ratio + reference;
	  }
	  selal();
	  return;
	}

	if (!SelectedFormList->empty()) {
	  for (auto selectedForm : (*SelectedFormList)) {
		auto& formIter = FormList->operator[](selectedForm);
		auto itVertex  = wrap::next(FormVertices->begin(), formIter.vertexIndex);
		for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		  itVertex->y = (itVertex->y - reference) * ratio + reference;
		  ++itVertex;
		}
	  }
	}
	else {
	  if (StateMap->test(StateFlag::FORMSEL)) {
		auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
		for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		  itVertex->y = (itVertex->y - reference) * ratio + reference;
		  ++itVertex;
		}
	  }
	  else {
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).y =
		      (StitchBuffer->operator[](iStitch).y - reference) * ratio + reference;
		}
	  }
	}
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  FormList->operator[](selectedForm).outline();
	  ClosestFormToCursor = selectedForm;
	  refil();
	}
  }
  else if (StateMap->test(StateFlag::FORMSEL)) {
	refil();
  }
  StateMap->set(StateFlag::RESTCH);
}

void form::setexpand(float xyRatio) {
  auto size0     = F_POINT {};
  auto rectangle = F_RECTANGLE {};
  thred::savdo();
  auto stitchPoint = F_POINT {};
  if (!SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
	wrap::narrow_cast(rectangle.left, SelectedFormsRect.left);
	wrap::narrow_cast(rectangle.top, SelectedFormsRect.top);
	wrap::narrow_cast(rectangle.right, SelectedFormsRect.right);
	wrap::narrow_cast(rectangle.bottom, SelectedFormsRect.bottom);

	wrap::narrow_cast(stitchPoint.x, Msg.pt.x - StitchWindowOrigin.x);
	wrap::narrow_cast(stitchPoint.y, Msg.pt.y - StitchWindowOrigin.y);
	size0.y = rectangle.bottom - rectangle.top;
  }
  else {
	stitchPoint = thred::pxCor2stch(Msg.pt);
	if (StateMap->test(StateFlag::FORMSEL)) {
	  rectangle = FormList->operator[](ClosestFormToCursor).rectangle;
	}
	else {
	  rectangle = StitchRangeRect;
	}
	size0.y = rectangle.top - rectangle.bottom;
  }
  auto ratio     = F_POINT {1.0F, 1.0F};
  auto reference = F_POINT {};
  size0.x        = rectangle.right - rectangle.left;
  switch (SelectedFormControlVertex) {
	case 0: {
	  reference.x = rectangle.right;
	  reference.y = rectangle.bottom;
	  auto size1  = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  if (auto const aspect = size1.x / size1.y; aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.left  = rectangle.right - size1.x;
		formRect.top   = rectangle.bottom + size1.y;
	  }
	  break;
	}
	case 1: {
	  reference.x = rectangle.left;
	  reference.y = rectangle.bottom;
	  auto size1  = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  if (auto const aspect = size1.x / size1.y; aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.right = rectangle.left + size1.x;
		formRect.top   = rectangle.bottom + size1.y;
	  }
	  break;
	}
	case 2: {
	  reference.x = rectangle.left;
	  reference.y = rectangle.top;
	  auto size1  = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  if (auto const aspect = size1.x / size1.y; aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect  = FormList->operator[](ClosestFormToCursor).rectangle;
		formRect.right  = rectangle.left + size1.x;
		formRect.bottom = rectangle.top - size1.y;
	  }
	  break;
	}
	case 3: {
	  reference.x = rectangle.right;
	  reference.y = rectangle.top;
	  auto size1  = F_POINT {fabs(stitchPoint.x - reference.x), fabs(stitchPoint.y - reference.y)};
	  if (auto const aspect = size1.x / size1.y; aspect < xyRatio) {
		size1.x = size1.y * xyRatio;
	  }
	  else {
		size1.y = size1.x / xyRatio;
	  }
	  ratio.x = size1.x / size0.x;
	  ratio.y = size1.y / size0.y;
	  if (SelectedFormList->empty() && StateMap->test(StateFlag::FORMSEL)) {
		auto& formRect  = FormList->operator[](ClosestFormToCursor).rectangle;
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
  auto const stitchReference  = fi::px2stchf(integerReference);
  if (StateMap->test(StateFlag::FPSEL)) {
	auto itVertex = wrap::next(FormVertices->begin(), FormList->operator[](ClosestFormToCursor).vertexIndex);
	auto iCurrent = SelectedFormVertices.start;
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const itCurrentVertex = wrap::next(itVertex, iCurrent);

	  itCurrentVertex->x = (itCurrentVertex->x - stitchReference.x) * ratio.x + stitchReference.x;
	  itCurrentVertex->y = (itCurrentVertex->y - stitchReference.y) * ratio.y + stitchReference.y;
	  iCurrent           = form::pdir(FormList->operator[](ClosestFormToCursor), iCurrent);
	}
	thred::setpsel();
	FormList->operator[](ClosestFormToCursor).outline();
	form::refil();
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  auto& formIter                      = FormList->operator[](iForm);
	  auto                       itVertex = wrap::next(FormVertices->begin(), formIter.vertexIndex);
	  auto const&                formVertexCount = formIter.vertexCount;
	  for (auto iVertex = 0U; iVertex < formVertexCount; ++iVertex) {
		itVertex->x = (itVertex->x - stitchReference.x) * ratio.x + stitchReference.x;
		itVertex->y = (itVertex->y - stitchReference.y) * ratio.y + stitchReference.y;
		++itVertex;
	  }
	  formIter.outline();
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.x = (stitch.x - stitchReference.x) * ratio.x + stitchReference.x;
	  stitch.y = (stitch.y - stitchReference.y) * ratio.y + stitchReference.y;
	}
	form::selal();
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto selectedForm : (*SelectedFormList)) {
	  auto& form = FormList->operator[](selectedForm);

	  auto const& formVertexCount = form.vertexCount;
	  auto        itVertex        = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < formVertexCount; ++iVertex) {
		itVertex->x = (itVertex->x - stitchReference.x) * ratio.x + stitchReference.x;
		itVertex->y = (itVertex->y - stitchReference.y) * ratio.y + stitchReference.y;
		++itVertex;
	  }
	  form.outline();
	  ClosestFormToCursor = selectedForm;
	  form::refil();
	}
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);

	  auto itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		itVertex->x = (itVertex->x - reference.x) * ratio.x + reference.x;
		itVertex->y = (itVertex->y - reference.y) * ratio.y + reference.y;
		++itVertex;
	  }
	  form::refil();
	}
	else {
	  auto itStitch = wrap::next(StitchBuffer->begin(), GroupStartStitch);
	  for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		itStitch->x = (itStitch->x - reference.x) * ratio.x + reference.x;
		itStitch->y = (itStitch->y - reference.y) * ratio.y + reference.y;
		++itStitch;
	  }
	}
  }
  StateMap->set(StateFlag::RESTCH);
}

void form::nufilcol(uint8_t color) noexcept {
  if (auto& formColor = FormList->operator[](ClosestFormToCursor).fillColor; formColor != color) {
	formColor = color;

	auto const attribute = (ClosestFormToCursor << FRMSHFT) | FRMFIL;
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (FRMSK | TYPMSK | FTHMSK)) == attribute) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color;
	  }
	}
  }
}

void form::nufthcol(uint8_t color) noexcept {
  if (auto& formColor = FormList->operator[](ClosestFormToCursor).fillInfo.feather.color; formColor != color) {
	formColor            = color;
	auto const attribute = (ClosestFormToCursor << FRMSHFT) | FTHMSK;
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (FRMSK | FTHMSK)) == attribute) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color;
	  }
	}
  }
}

void form::nubrdcol(uint8_t color) noexcept {
  FormList-> operator[](ClosestFormToCursor).borderColor = color;
  auto const attribute = (ClosestFormToCursor << FRMSHFT) | FRMBFIL;
  for (auto& stitch : *StitchBuffer) {
	if ((stitch.attribute & (FRMSK | TYPMSK)) == attribute) {
	  stitch.attribute &= NCOLMSK;
	  stitch.attribute |= color;
	}
  }
}

void form::nulapcol(uint8_t color) {
  if (auto& currentForm = FormList->operator[](ClosestFormToCursor);
      gsl::narrow<decltype(color)>(currentForm.borderColor >> FRMSHFT) != color) {
	currentForm.borderColor &= COLMSK;
	currentForm.borderColor |= gsl::narrow_cast<uint8_t>(color << FRMSHFT);
	auto const attribute = (ClosestFormToCursor << FRMSHFT) | TYPMSK;
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (TYPMSK | FRMSK)) == attribute) {
		stitch.attribute &= NCOLMSK;
		stitch.attribute |= color;
	  }
	}
  }
}

void fi::sbold(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.edgeType = EDGEBEAN;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  form::refilfn();
}

void form::dubold() {
  if (displayText::filmsgs(FML_BLD)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  fi::sbold(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  fi::sbold(ClosestFormToCursor);
	  thred::coltab();
	  StateMap->set(StateFlag::INIT);
	  StateMap->set(StateFlag::RESTCH);
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	}
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

void form::dustar(uint32_t starCount, float length) {
  if (constexpr auto STARMIN = 3U; // minimum star vertices
      starCount < STARMIN) {
	starCount = STARMIN;
  }
  if (constexpr auto STARMAX = 100U; // maximum star vertices
      starCount > STARMAX) {
	starCount = STARMAX;
  }
  auto const stepAngle   = PI_F / wrap::toFloat(starCount);
  auto       angle       = stepAngle / 2.0F + PI_F;
  auto const vertexCount = starCount * 2U;
  auto       newForm     = FRM_HEAD {};
  newForm.vertexIndex    = thred::adflt(vertexCount);
  newForm.vertexCount    = vertexCount;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  newForm.type = FRMFPOLY;
  auto point   = thred::pxCor2stch(Msg.pt);
  StateMap->set(StateFlag::FILDIR);
  auto itFirstVertex = wrap::next(FormVertices->begin(), newForm.vertexIndex);
  auto itVertex      = itFirstVertex;
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	itVertex->x = point.x;
	itVertex->y = point.y;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle += stepAngle;
	++itVertex;
  }
  itVertex               = itFirstVertex;
  auto       itEndVertex = wrap::next(itVertex, starCount);
  auto const center      = F_POINT {wrap::midl(itEndVertex->x, itFirstVertex->x),
                               wrap::midl(itEndVertex->y, itFirstVertex->y)};
  ++itVertex;
  for (auto iVertex = 1U; iVertex < starCount; ++iVertex) {
	itVertex->x = (itVertex->x - center.x) * StarRatio + center.x;
	itVertex->y = (itVertex->y - center.y) * StarRatio + center.y;
	itVertex += 2;
  }
  itVertex->x = (itVertex->x - center.x) * StarRatio + center.x;
  itVertex->y = (itVertex->y - center.y) * StarRatio + center.y;
  newForm.outline();
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::duspir(uint32_t stepCount) {
  if (constexpr auto STEPMIN = 3U; // Spiral step minmum
      stepCount < STEPMIN) {
	stepCount = STEPMIN;
  }
  if (constexpr auto STEPMAX = 100U; // spiral step maximum
      stepCount > STEPMAX) {
	stepCount = STEPMAX;
  }
  auto const stepAngle = PI_F2 / wrap::toFloat(stepCount);
  auto const length    = 800.0F / wrap::toFloat(stepCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  auto newForm        = FRM_HEAD {};
  newForm.type        = FRMLINE;
  auto vertexCount    = wrap::round<uint32_t>(wrap::toFloat(stepCount) * SpiralWrap);
  newForm.vertexIndex = thred::adflt(vertexCount);
  newForm.vertexCount = vertexCount;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  auto firstSpiral = std::vector<F_POINT> {};
  firstSpiral.resize(stepCount);
  auto centeredSpiral = std::vector<F_POINT> {};
  centeredSpiral.resize(stepCount);
  auto point = thred::pxCor2stch(Msg.pt);
  auto angle = 0.0F;
  for (auto iStep = 0U; iStep < stepCount; ++iStep) {
	firstSpiral[iStep].x = point.x;
	firstSpiral[iStep].y = point.y;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle += stepAngle;
  }
  auto const center = F_POINT {wrap::midl(firstSpiral[stepCount / 2].x, firstSpiral[0].x),
                               wrap::midl(firstSpiral[stepCount / 2].y, firstSpiral[0].y)};
  for (auto iStep = 0U; iStep < stepCount; ++iStep) {
	centeredSpiral[iStep].x = firstSpiral[iStep].x - center.x;
	centeredSpiral[iStep].y = firstSpiral[iStep].y - center.y;
  }
  auto const stepRatio = 1.0F / wrap::toFloat(vertexCount);
  auto       ratio     = stepRatio;
  auto       itVertex  = wrap::next(FormVertices->begin(), newForm.vertexIndex);
  for (auto iVertex = 0U; iVertex < vertexCount; ++iVertex) {
	itVertex->x = centeredSpiral[iVertex % stepCount].x * ratio + center.x;
	itVertex->y = centeredSpiral[iVertex % stepCount].y * ratio + center.y;
	++itVertex;
	ratio += stepRatio;
  }
  newForm.outline();
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertexCount + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::duhart(uint32_t sideCount) {
  constexpr auto HARTANG  = 0.28F; // heart angle in radians
  constexpr auto HARTINFL = 0.7F;  // heart inflection point in radians
  constexpr auto HARTSTEP = 4.5F;

  constexpr auto HSIDEMAX = 100U; // maximum number of heart vertices
  if (sideCount > HSIDEMAX) {
	sideCount = HSIDEMAX;
  }
  if (constexpr auto HSIDEMIN = 6U; // minimum number of heart vertices
      sideCount < HSIDEMIN) {
	sideCount = HSIDEMIN;
  }
  auto currentForm        = FRM_HEAD {};
  currentForm.vertexIndex = wrap::toUnsigned(FormVertices->size());
  wrap::narrow(currentForm.attribute, ActiveLayer << 1U);
  FormVertices->reserve(FormVertices->size() + wrap::toSize(sideCount) * 2U - 2U);
  auto const savedVertexIndex = wrap::toUnsigned(FormVertices->size());
  auto       point            = thred::pxCor2stch(Msg.pt);
  auto       stepAngle        = PI_F2 / wrap::toFloat(sideCount);
  auto const length           = 300.0F / wrap::toFloat(sideCount) * ZoomFactor *
                      wrap::toFloat(UnzoomedRect.cx + UnzoomedRect.cy) / (LHUPX + LHUPY);
  auto angle    = PI_F * HARTANG;
  auto iVertex  = ptrdiff_t {};
  auto maximumX = 0.0F;
  while (angle > -PI_F * HARTINFL) {
	if (point.x > maximumX) {
	  maximumX = point.x;
	}
	FormVertices->push_back(point);
	++iVertex;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
	angle -= stepAngle;
  }
  stepAngle /= HARTSTEP;
  auto       lastVertex    = iVertex;
  auto const itFirstVertex = wrap::next(FormVertices->begin(), savedVertexIndex);
  while (point.x > itFirstVertex->x && iVertex < wrap::ptrdiff(2U * HSIDEMAX)) {
	FormVertices->push_back(F_POINT {point.x, point.y});
	++iVertex;
	point.x += length * cos(angle);
	point.y += length * sin(angle);
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
	FormVertices->push_back(
	    F_POINT {maximumX + maximumX - itVertex->x - 2 * (maximumX - itFirstVertex->x), itVertex->y});
	--itVertex;
	++iDestination;
  }
  NewFormVertexCount      = iDestination + 1U;
  currentForm.vertexCount = iDestination;
  currentForm.type        = FRMFPOLY;
  currentForm.outline();
  FormList->push_back(currentForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::dulens(uint32_t sides) {
  if (sides < 4) {
	sides = 4;
  }
  /*
      if (sides > 48) {
          sides = 48;
      }
  */
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
  currentForm.vertexIndex = wrap::toUnsigned(FormVertices->size());
  currentForm.attribute   = gsl::narrow_cast<decltype(currentForm.attribute)>(ActiveLayer << 1U);
  auto const stitchPoint  = thred::pxCor2stch(Msg.pt);
  auto       point        = stitchPoint;
  auto       iVertex      = 0U;
  FormVertices->reserve(FormVertices->size() + wrap::toSize(steps << 1U) + 1U);
  // rho is the angle of this particular segment
  auto rho = PI_F - theta - phi;
  for (auto iStep = 0U; iStep < steps; ++iStep) {
	FormVertices->push_back(point);
	point.x += stepLength * cos(rho);
	point.y += stepLength * sin(rho);
	rho += omega;
	++iVertex;
  }
  FormVertices->push_back(point);
  auto       itVertex = wrap::next(FormVertices->cbegin(), currentForm.vertexIndex + iVertex - 1U);
  auto const xCoord   = stitchPoint.x;
  for (; iVertex != 1; --iVertex) {
	FormVertices->push_back(F_POINT {xCoord + xCoord - (*itVertex).x, (*itVertex).y});
	--itVertex;
  }
  currentForm.vertexCount = wrap::toUnsigned(FormVertices->size() - currentForm.vertexIndex);
  NewFormVertexCount      = currentForm.vertexCount + 1U;
  currentForm.type        = FRMFPOLY;
  currentForm.outline();
  FormList->push_back(currentForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

constexpr auto fi::shreg(float highValue, float reference, float eggRatio) noexcept -> float {
  return (highValue - reference) * eggRatio + reference;
}

void form::dueg(uint32_t sides) {
  if (constexpr auto ESIDEMIN = 8U; // mimimum number of egg vertices
      sides < ESIDEMIN) {
	sides = ESIDEMIN;
  }
  form::durpoli(sides);
  auto const  halfSides    = gsl::narrow<ptrdiff_t>(sides >> 1U);
  auto const  quarterSides = gsl::narrow<ptrdiff_t>(sides >> 2U);
  auto const& form         = FormList->back();
  auto        itVertex     = wrap::next(FormVertices->begin(), form.vertexIndex);
  auto const  reference    = wrap::midl(itVertex[halfSides].y, itVertex[0].y);
  auto const  maximumY     = itVertex[quarterSides].y - itVertex[0].y;
  for (uint32_t iVertex = 0; iVertex < sides; ++iVertex) {
	if (itVertex->y < reference) {
	  itVertex->y = reference - (reference - itVertex->y) * IniFile.eggRatio;
	}
	++itVertex;
  }
  itVertex            = wrap::next(FormVertices->begin(), form.vertexIndex);
  auto const eggRatio = maximumY / (itVertex[quarterSides].y - itVertex[0].y);
  auto const ref      = *itVertex;
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	*itVertex = F_POINT {fi::shreg(itVertex->x, ref.x, eggRatio), fi::shreg(itVertex->y, ref.y, eggRatio)};
	++itVertex;
  }
}

void form::duzig(uint32_t vertices) {
  if (constexpr auto ZVERTMIN = 3U; // mimimum number of zig-zag vertices
      vertices < ZVERTMIN) {
	vertices = ZVERTMIN;
  }
  if (constexpr auto ZVERTMAX = 100U; // maximum number of zig-zag vertices
      vertices > ZVERTMAX) {
	vertices = ZVERTMAX;
  }
  auto newForm        = FRM_HEAD {};
  newForm.vertexIndex = thred::adflt(vertices);
  newForm.vertexCount = vertices;
  wrap::narrow(newForm.attribute, ActiveLayer << 1U);
  auto       stitchPoint = thred::pxCor2stch(Msg.pt);
  auto const offset      = F_POINT {UnzoomedRect.cx / 6.0, UnzoomedRect.cy / (6.0 * vertices)};
  auto       itVertex    = wrap::next(FormVertices->begin(), newForm.vertexIndex);
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
  FormList->push_back(newForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
  FormMoveDelta       = F_POINT {};
  NewFormVertexCount  = vertices + 1U;
  StateMap->set(StateFlag::POLIMOV);
  form::setmfrm();
  StateMap->set(StateFlag::SHOFRM);
  form::mdufrm();
}

void form::fliph() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	thred::savdo();
	auto const offset        = SelectedVerticesRect.right + SelectedVerticesRect.left;
	auto       currentVertex = SelectedFormVertices.start;
	auto const itFirstVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= SelectedFormVertices.vertexCount; ++iVertex) {
	  auto const itVertex = wrap::next(itFirstVertex, currentVertex);
	  itVertex->x         = offset - itVertex->x;
	  currentVertex       = form::pdir(form, currentVertex);
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (StateMap->test(StateFlag::BIGBOX)) {
	thred::savdo();
	auto const offset = AllItemsRect->right + AllItemsRect->left;
	for (auto& formVertice : *FormVertices) {
	  formVertice.x = offset - formVertice.x;
	}
	for (auto& stitch : *StitchBuffer) {
	  stitch.x = offset - stitch.x;
	}
	for (auto& iForm : *FormList) {
	  auto& rect = iForm.rectangle;
	  rect.left  = offset - rect.left;
	  rect.right = offset - rect.right;
	}
	StateMap->set(StateFlag::RESTCH);
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	auto formMap   = boost::dynamic_bitset<>(FormList->size());
	auto rectangle = F_RECTANGLE {};
	form::pxrct2stch(SelectedFormsRect, rectangle);
	auto const offset = rectangle.right + rectangle.left;
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  formMap.set(selectedForm);
	  // clang-format off
	  auto& formIter = FormList->operator[](selectedForm);
	  auto  itVertex = wrap::next(FormVertices->begin(), formIter.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < formIter.vertexCount; ++iVertex) {
		itVertex->x = offset - itVertex->x;
		++itVertex;
	  }
	  formIter.outline();
	}
	for (auto& stitch : *StitchBuffer) {
	  if (auto const decodedForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	      formMap.test(decodedForm) && ((stitch.attribute & NOTFRM) == 0U)) {
		stitch.x = offset - stitch.x;
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto const& srcRect  = form.rectangle;
	  auto const  offset   = srcRect.right + srcRect.left;
	  auto        itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
		itVertex->x = offset - itVertex->x;
		++itVertex;
	  }
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor &&
		    ((stitch.attribute & NOTFRM) == 0U)) {
		  stitch.x = offset - stitch.x;
		}
	  }
	  form.outline();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto rectangle = F_RECTANGLE {};
		thred::selRct(rectangle);
		auto const offset = rectangle.right + rectangle.left;
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).x = offset - StitchBuffer->operator[](iStitch).x;
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void fi::prpsbrd(uint32_t formIndex) {
  if (auto& form = FormList->operator[](formIndex); form.vertexCount > 2) {
	clip::deleclp(formIndex);
	form.edgeType = EDGEPROPSAT;
	if (UserFlagMap->test(UserFlag::DUND)) {
	  form.edgeType |= EGUND;
	}
	form::bsizpar(form);
	form.borderSize  = BorderWidth;
	form.edgeSpacing = LineSpacing;
	wrap::narrow(form.borderColor, ActiveColor);
	form::refilfn();
  }
}

void form::prpbrd(float borderStitchSpacing) {
  auto const savedSpacing = LineSpacing;
  if (displayText::filmsgs(FML_PRPS)) {
	return;
  }
  LineSpacing = borderStitchSpacing;
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor    = selectedForm;
	  auto& currentForm      = FormList->operator[](ClosestFormToCursor);
	  currentForm.borderSize = LineSpacing;
	  if (UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::prpsbrd(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  if (auto& currentForm = FormList->operator[](ClosestFormToCursor); UserFlagMap->test(UserFlag::BLUNT)) {
		currentForm.attribute |= gsl::narrow_cast<decltype(currentForm.attribute)>(SBLNT | FBLNT);
	  }
	  else {
		currentForm.attribute &= NOBLNT;
	  }
	  fi::prpsbrd(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
  LineSpacing = savedSpacing;
}

void form::tglfrm() {
  if (StateMap->testAndReset(StateFlag::SATPNT)) {
	satin::satfix();
  }
  StateMap->reset(StateFlag::HIDSTCH);
  menu::flipFormOnOff();
  StateMap->set(StateFlag::RESTCH);
}

void form::frmon() {
  thred::unbsho();
  StateMap->reset(StateFlag::FRMOF);
  menu::resetFormOnOff();
}

void fi::fnord() {
  // clang-format off
  auto const& currentForm   = FormList->operator[](ClosestFormToCursor);
  auto const  itStartVertex = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  auto const  itEndVertex   = wrap::next(itStartVertex, currentForm.vertexCount);
  // clang-format on
  std::reverse(itStartVertex, itEndVertex);
  form::refil();
}

void form::flpord() {
  auto const& form = FormList->operator[](ClosestFormToCursor);
  if (StateMap->test(StateFlag::FPSEL)) {
	thred::savdo();
	auto start = SelectedFormVertices.start;
	auto finish = (SelectedFormVertices.start + SelectedFormVertices.vertexCount) % form.vertexCount;
	auto const itFirstVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
	for (auto iVertex = 0U; iVertex <= (SelectedFormVertices.vertexCount / 2); ++iVertex) {
	  auto itStartVertex = wrap::next(itFirstVertex, start);
	  auto itEndVertex   = wrap::next(itFirstVertex, finish);
	  std::swap(*itStartVertex, *itEndVertex);
	  start = form::pdir(form, start);
	  StateMap->flip(StateFlag::PSELDIR);
	  finish = form::pdir(form, finish);
	  StateMap->flip(StateFlag::PSELDIR);
	}
	form::refil();
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  fi::fnord();
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::fnord();
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto const start = wrap::next(StitchBuffer->begin(), GroupStartStitch);
		auto const end   = wrap::next(StitchBuffer->begin(), GroupEndStitch);
		std::reverse(start, end);
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void fi::filsclp() {
  texture::deltx(ClosestFormToCursor);
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  if (currentForm.type != SAT) {
	currentForm.wordParam = 0;
  }
  currentForm.type                    = SAT;
  currentForm.fillType                = CLPF;
  currentForm.angleOrClipData.clip    = clip::numclp(ClosestFormToCursor);
  currentForm.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
  auto itClipPoints = wrap::next(ClipPoints->begin(), currentForm.angleOrClipData.clip);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoints = clip;
	++itClipPoints;
  }
  form::refilfn();
}

void form::clpfil() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN && ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
			  fi::filsclp();
			}
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::filsclp();
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void fi::snpfn(std::vector<uint32_t> const& xPoints, uint32_t start, uint32_t end, uint32_t finish) noexcept {
  if (finish != start) {
	for (auto current = start; current < end; ++current) {
	  auto const reference = xPoints[current];
	  for (auto iPoint = current + 1U; iPoint < finish; ++iPoint) {
		auto const check = xPoints[iPoint];
		if (auto const checkLength =
		        hypot(StitchBuffer->operator[](check).x - StitchBuffer->operator[](reference).x,
		              StitchBuffer->operator[](check).y - StitchBuffer->operator[](reference).y);
		    checkLength < SnapLength) {
		  StitchBuffer->operator[](check) = StitchBuffer->operator[](reference);
		}
	  }
	}
  }
}

void fi::doTimeWindow(float rangeX, std::vector<uint32_t> const& xPoints, std::vector<uint32_t> const& xHistogram) {
  auto const checkLength = wrap::round<uint32_t>(SnapLength * 2.0F + 1.0F);
  // NOLINTNEXTLINE(hicpp-signed-bitwise,readability-qualified-auto)
  auto const timeWindow = CreateWindow(L"STATIC",
                                       nullptr,
                                       WS_CHILD | WS_VISIBLE | WS_BORDER,
                                       ButtonWidthX3,
                                       0,
                                       StitchWindowSize.cx,
                                       ButtonHeight,
                                       ThrEdWindow,
                                       nullptr,
                                       ThrEdInstance,
                                       nullptr);

  // NOLINTNEXTLINE(readability-qualified-auto)
  auto const timeDC       = GetDC(timeWindow);
  auto const timeStep     = wrap::toFloat(StitchWindowSize.cx) / rangeX;
  auto       timePosition = 0.0F;
  auto&      formLines    = *FormLines;
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
  DestroyWindow(timeWindow);
}

void fi::snp(uint32_t start, uint32_t finish) {
  auto range = F_POINT {};
  thred::chkrng(range);
  auto xPoints = std::vector<uint32_t> {};
  xPoints.resize(StitchBuffer->size());
  auto xHistogram = std::vector<uint32_t> {};
  xHistogram.resize(wrap::round<size_t>(range.x) + 1U);
  auto const attribute = (ClosestFormToCursor << 4U) & FRMSK;
  if (StateMap->test(StateFlag::FORMSEL)) {
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  if (((StitchBuffer->operator[](iStitch).attribute & NOTFRM) == 0U) &&
	      (StitchBuffer->operator[](iStitch).attribute & FRMSK) == attribute) {
		auto const iColumn = wrap::floor<uint32_t>(StitchBuffer->operator[](iStitch).x);
		++(xHistogram[iColumn]);
	  }
	}
  }
  else {
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  auto const iColumn = wrap::floor<uint32_t>(StitchBuffer->operator[](iStitch).x);
	  ++(xHistogram[iColumn]);
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
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto iStitch = 0U;
	for (auto const& stitch : *StitchBuffer) {
	  if (((stitch.attribute & NOTFRM) == 0U) && (stitch.attribute & FRMSK) == attribute) {
		auto const iColumn             = wrap::floor<uint32_t>(stitch.x);
		xPoints[xHistogram[iColumn]++] = iStitch;
	  }
	  ++iStitch;
	}
  }
  else {
	auto iStitch = 0U;
	for (auto const& stitch : *StitchBuffer) {
	  auto const iColumn             = wrap::floor<uint32_t>(stitch.x);
	  xPoints[xHistogram[iColumn]++] = iStitch;
	  ++iStitch;
	}
  }
  doTimeWindow(range.x, xPoints, xHistogram);
}

void form::snap() {
  thred::savdo();
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	fi::snp(GroupStartStitch, GroupEndStitch);
  }
  else {
	fi::snp(0, wrap::toUnsigned(StitchBuffer->size()));
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void fi::dufcntr(F_POINT& center) noexcept {
  auto bigRect = FormList->operator[](SelectedFormList->front()).rectangle;
  for (auto const selectedForm : (*SelectedFormList)) {
	auto const formRect = FormList->operator[](selectedForm).rectangle;
	if (formRect.left < bigRect.left) {
	  bigRect.left = formRect.left;
	}
	if (formRect.top > bigRect.top) {
	  bigRect.top = formRect.top;
	}
	if (formRect.right > bigRect.right) {
	  bigRect.right = formRect.right;
	}
	if (formRect.bottom < bigRect.bottom) {
	  bigRect.bottom = formRect.bottom;
	}
  }
  center = F_POINT {wrap::midl(bigRect.right, bigRect.left), wrap::midl(bigRect.top, bigRect.bottom)};
}

auto form::rotpar() -> F_POINT {
  do {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  RotationRect = FormList->operator[](ClosestFormToCursor).rectangle;
	  StateMap->set(StateFlag::FRMROT);
	  break;
	}
	if (!SelectedFormList->empty()) {
	  form::pxrct2stch(SelectedFormsRect, RotationRect);
	  StateMap->set(StateFlag::FRMSROT);
	  break;
	}
	if (StateMap->test(StateFlag::BIGBOX)) {
	  RotationRect = *AllItemsRect;
	  break;
	}
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  thred::selRct(RotationRect);
	  break;
	}
	if (StateMap->test(StateFlag::FPSEL)) {
	  RotationRect = SelectedVerticesRect;
	  break;
	}
  } while (false);
  if (StateMap->test(StateFlag::GMRK)) {
	return ZoomMarkPoint;
  }
  return F_POINT {wrap::midl(RotationRect.right, RotationRect.left),
                  wrap::midl(RotationRect.top, RotationRect.bottom)};
}

void fi::rotentr(float rotationAngle) {
  displayText::showMessage(IDS_ROTA, (rotationAngle * RADDEGF));
  StateMap->set(StateFlag::NUMIN);
  displayText::numWnd();
}

void form::setrang() {
  fi::rotentr(IniFile.rotationAngle);
  StateMap->set(StateFlag::NUROT);
}

void form::rotcmd() {
  if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL) ||
      !SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
	fi::rotentr(IniFile.rotationAngle);
	StateMap->set(StateFlag::ENTROT);
  }
  else {
	displayText::shoseln(IDS_ALLX, IDS_ROTCMD);
  }
}

void fi::fnagain(float rotationAngle) {
  if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL) ||
      !SelectedFormList->empty() || StateMap->test(StateFlag::BIGBOX) || StateMap->test(StateFlag::FPSEL)) {
	auto const rotationCenter = form::rotpar();
	thred::rotfn(rotationAngle, rotationCenter);
  }
  else {
	displayText::alrotmsg();
  }
}

void form::rotagain() {
  fi::fnagain(IniFile.rotationAngle);
}

void form::bakagain() {
  fi::fnagain(PI_F2 - IniFile.rotationAngle);
}

void form::rotdup() {
  if (StateMap->test(StateFlag::FORMSEL) || StateMap->test(StateFlag::GRPSEL) || !SelectedFormList->empty()) {
	fi::rotentr(IniFile.rotationAngle);
	StateMap->set(StateFlag::ENTRDUP);
  }
  else {
	displayText::shord();
  }
}

void fi::adfrm(uint32_t iForm) {
  auto currentForm                                 = FormList->operator[](iForm);
  auto const                   originalVertexIndex = currentForm.vertexIndex;
  currentForm.vertexIndex                          = wrap::toUnsigned(FormVertices->size());
  auto const itVertex = wrap::next(FormVertices->cbegin(), originalVertexIndex);
  FormVertices->insert(FormVertices->end(), itVertex, wrap::next(itVertex, currentForm.vertexCount));
  if (currentForm.type == SAT && (currentForm.satinGuideCount != 0U)) {
	auto const originalGuide       = currentForm.satinOrAngle.guide;
	currentForm.satinOrAngle.guide = wrap::toUnsigned(SatinGuides->size());

	auto const itGuides = wrap::next(SatinGuides->cbegin(), originalGuide);
	SatinGuides->insert(SatinGuides->end(), itGuides, wrap::next(itGuides, currentForm.satinGuideCount));
  }
  if (currentForm.isEdgeClipX()) {
	auto const originalBCData  = currentForm.borderClipData;
	currentForm.borderClipData = wrap::toUnsigned(ClipPoints->size());

	auto const itClipPoints = wrap::next(ClipPoints->cbegin(), originalBCData);
	ClipPoints->insert(ClipPoints->end(), itClipPoints, wrap::next(itClipPoints, currentForm.clipEntries));
  }
  if (currentForm.isClipX()) {
	auto const originalClip          = currentForm.angleOrClipData.clip;
	currentForm.angleOrClipData.clip = wrap::toUnsigned(ClipPoints->size());

	auto const itClipPoints = wrap::next(ClipPoints->cbegin(), originalClip);
	ClipPoints->insert(
	    ClipPoints->end(), itClipPoints, wrap::next(itClipPoints, currentForm.lengthOrCount.clipCount));
  }
  FormList->push_back(currentForm);
  ClosestFormToCursor = wrap::toUnsigned(FormList->size() - 1U);
}

void form::duprot(float rotationAngle) {
  thred::savdo();
  fi::adfrm(ClosestFormToCursor);
  auto const rotationCenter = form::rotpar();
  thred::rotfn(rotationAngle, rotationCenter);
  form::refil();
  StateMap->set(StateFlag::FORMSEL);
  StateMap->set(StateFlag::RESTCH);
}

void fi::duprotfs(float rotationAngle) {
  auto const rotationCenter = form::rotpar();
  for (auto const selectedForm : (*SelectedFormList)) {
	adfrm(selectedForm);
	thred::rotfn(rotationAngle, rotationCenter);
  }
}

void fi::duprots(float rotationAngle, F_POINT const& rotationCenter) {
  thred::rngadj();
  ClosestPointIndex = wrap::toUnsigned(StitchBuffer->size());
  StitchBuffer->reserve(ClosestPointIndex + (wrap::toSize(GroupEndStitch) + 1U - GroupStartStitch));
  StitchBuffer->insert(StitchBuffer->end(),
                       wrap::next(StitchBuffer->cbegin(), GroupStartStitch),
                       wrap::next(StitchBuffer->cbegin(), GroupEndStitch + 1U));
  for (auto currentStitch = wrap::next(StitchBuffer->begin(), ClosestPointIndex);
       currentStitch != StitchBuffer->end();
       ++currentStitch) {
	currentStitch->attribute &= (~(FRMSK | TYPMSK));
  }
  GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
  thred::rngadj();
  thred::rotfn(rotationAngle, rotationCenter);
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void fi::cplayfn(uint32_t iForm, uint32_t layer) {
  auto currentForm = FormList->operator[](iForm);

  auto const originalVertexIndex = currentForm.vertexIndex;
  currentForm.vertexIndex        = thred::adflt(currentForm.vertexCount);
  auto const itVertex            = wrap::next(FormVertices->cbegin(), originalVertexIndex);
  std::copy(itVertex,
            wrap::next(itVertex, currentForm.vertexCount),
            wrap::next(FormVertices->begin(), currentForm.vertexIndex));
  if (currentForm.type == SAT && (currentForm.satinGuideCount != 0U)) {
	auto const originalGuide       = currentForm.satinOrAngle.guide;
	currentForm.satinOrAngle.guide = wrap::toUnsigned(SatinGuides->size());

	auto const itStartGuide = wrap::next(SatinGuides->cbegin(), originalGuide);
	auto const itEndGuide   = wrap::next(itStartGuide, currentForm.satinGuideCount);
	auto const destination  = SatinGuides->end();
	SatinGuides->insert(destination, itStartGuide, itEndGuide);
  }
  currentForm.clipEntries             = 0;
  currentForm.fillType                = 0;
  currentForm.lengthOrCount.clipCount = 0;
  currentForm.edgeType                = 0;
  currentForm.fillInfo.texture.index  = 0;
  currentForm.attribute &= NFRMLMSK;
  currentForm.attribute |= layer << FLAYSHFT;
  currentForm.extendedAttribute = 0;
  currentForm.squareEnd(UserFlagMap->test(UserFlag::SQRFIL));
  FormList->push_back(currentForm);
}

void form::cpylayr(uint32_t layer) {
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto const selectedForm : (*SelectedFormList)) {
	  fi::cplayfn(selectedForm, layer);
	}
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  fi::cplayfn(ClosestFormToCursor, layer);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		auto const oldSize = StitchBuffer->size();
		StitchBuffer->reserve(oldSize + (wrap::toSize(GroupEndStitch) + 1U - GroupStartStitch));
		StitchBuffer->insert(StitchBuffer->end(),
		                     wrap::next(StitchBuffer->cbegin(), GroupStartStitch),
		                     wrap::next(StitchBuffer->cbegin(), GroupEndStitch + 1U));
		auto const codedStitchLayer = layer << LAYSHFT;
		for (auto currentStitch = wrap::next(StitchBuffer->begin(), oldSize);
		     currentStitch != StitchBuffer->end();
		     ++currentStitch) {
		  currentStitch->attribute &= NLAYMSK;
		  currentStitch->attribute |= codedStitchLayer;
		}
		thred::coltab();
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::movlayr(uint32_t layer) {
  if (auto const codedStitchLayer = layer << LAYSHFT; !SelectedFormList->empty()) {
	thred::savdo();
	auto formMap = boost::dynamic_bitset<>(FormList->size());
	for (auto const selectedForm : (*SelectedFormList)) {
	  auto& formAttr = FormList->operator[](selectedForm).attribute;
	  formAttr = gsl::narrow_cast<uint8_t>(gsl::narrow_cast<uint8_t>(formAttr & NFRMLMSK) | (layer << FLAYSHFT));
	  formMap.set(selectedForm);
	}
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & ALTYPMSK) != 0U) {
		if (auto const iCurrentForm = (stitch.attribute & FRMSK) >> FRMSHFT; formMap.test(iCurrentForm)) {
		  stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
		}
	  }
	}
	SelectedFormList->clear();
	StateMap->reset(StateFlag::FORMSEL);
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  thred::savdo();
	  auto& formAttr = FormList->operator[](ClosestFormToCursor).attribute;
	  wrap::narrow(formAttr, (layer << FLAYSHFT | gsl::narrow_cast<decltype(layer)>(formAttr & NFRMLMSK)));
	  StateMap->reset(StateFlag::FORMSEL);
	  for (auto& stitch : *StitchBuffer) {
		if (((stitch.attribute & ALTYPMSK) != 0U) && ((stitch.attribute & FRMSK) >> FRMSHFT) == ClosestFormToCursor) {
		  stitch.attribute = (stitch.attribute & NLAYMSK) | codedStitchLayer;
		}
	  }
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		thred::savdo();
		thred::rngadj();
		for (auto iStitch = GroupStartStitch; iStitch < GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).attribute =
		      (StitchBuffer->operator[](iStitch).attribute & NLAYMSK) | codedStitchLayer;
		}
		StateMap->set(StateFlag::RESTCH);
	  }
	}
  }
}

void form::join() {
  auto const savedFormIndex = ClosestFormToCursor;

  auto const& savedform = FormList->operator[](ClosestFormToCursor);
  auto const                        lastVertex =
      wrap::next(FormVertices->cbegin(), savedform.vertexIndex + savedform.vertexCount - 1U);
  StateMap->set(StateFlag::FRMSAM);
  if (FormList->size() > 1 && StateMap->test(StateFlag::FORMSEL) && form::closfrm()) {
	// clang-format off
	auto const& form       = FormList->operator[](ClosestFormToCursor);
	auto        vertexList = std::vector<F_POINT> {};
	// clang-format on
	vertexList.reserve(form.vertexCount);
	auto const itVertex = wrap::next(FormVertices->cbegin(), form.vertexIndex);
	if ((abs(lastVertex->x - itVertex->x) > TNYFLOAT) || (abs(lastVertex->y - itVertex->y) > TNYFLOAT)) {
	  auto const itThisVertex = wrap::next(itVertex, ClosestVertexToCursor);
	  vertexList.push_back(*itThisVertex);
	}
	ClosestVertexToCursor = form::nxt(form, ClosestVertexToCursor);
	for (auto iVertex = 1U; iVertex < form.vertexCount; ++iVertex) {
	  auto itThisVertex = wrap::next(itVertex, ClosestVertexToCursor);
	  vertexList.push_back(*itThisVertex);
	  ClosestVertexToCursor = form::nxt(form, ClosestVertexToCursor);
	}
	StateMap->set(StateFlag::DELTO);
	thred::frmdel();
	if (savedFormIndex > ClosestFormToCursor) {
	  ClosestFormToCursor = savedFormIndex - 1U;
	}
	else {
	  ClosestFormToCursor = savedFormIndex;
	}
	auto& toForm = FormList->operator[](ClosestFormToCursor);

	auto const insertionPoint = toForm.vertexIndex + toForm.vertexCount;
	form::fltspac(toForm.vertexCount, wrap::toUnsigned(vertexList.size()));
	auto const dest = wrap::next(FormVertices->begin(), insertionPoint);
	std::ranges::copy(vertexList, dest);
	toForm.vertexCount += wrap::toUnsigned(vertexList.size());
	toForm.outline();
	form::refil();
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  StateMap->reset(StateFlag::FRMSAM);
}

void form::refilal() {
  auto const savedFormIndex = ClosestFormToCursor;
  thred::savdo();
  for (ClosestFormToCursor = 0; ClosestFormToCursor < wrap::toUnsigned(FormList->size()); ++ClosestFormToCursor) {
	form::refilfn();
  }
  ClosestFormToCursor = savedFormIndex;
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::nufsel() {
  if (!FormList->empty()) {
	StateMap->reset(StateFlag::FORMSEL);
	if (StateMap->testAndReset(StateFlag::WASEL)) {
	  SelectedFormList->push_back(PreviousFormIndex);
	}
	if (std::ranges::none_of(*SelectedFormList,
	                         [](uint32_t intX) { return intX == ClosestFormToCursor; })) {
	  SelectedFormList->push_back(ClosestFormToCursor);
	}
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::frmadj(uint32_t formIndex) {
  // clang-format off
  auto& form     = FormList->operator[](formIndex);
  auto  itVertex = wrap::next(FormVertices->begin(), form.vertexIndex);
  // clang-format on
  for (auto iVertex = 0U; iVertex < form.vertexCount; ++iVertex) {
	itVertex->x += FormMoveDelta.x;
	itVertex->y -= FormMoveDelta.y;
	++itVertex;
  }
  form.outline();
}

void form::frmsadj() {
  auto formMap = boost::dynamic_bitset<>(FormList->size());
  for (auto const selectedForm : (*SelectedFormList)) {
	formMap.set(selectedForm);
  }
  for (auto& stitch : *StitchBuffer) {
	if (((stitch.attribute & ALTYPMSK) != 0U) && formMap.test((stitch.attribute & FRMSK) >> FRMSHFT)) {
	  stitch.x += FormMoveDelta.x;
	  stitch.y -= FormMoveDelta.y;
	}
  }
}

void fi::frmpnts(uint32_t type) {
  if (!StitchBuffer->empty()) {
	auto       iStitch = 0U;
	auto const trg     = ((ClosestFormToCursor << 4U) | type);
	for (auto const& stitch : *StitchBuffer) {
	  if ((stitch.attribute & (ALTYPMSK | FRMSK)) == trg) {
		break;
	  }
	  ++iStitch;
	}
	ClosestPointIndex = iStitch;
	auto bFlag        = false;
	for (auto itStitch = wrap::next(StitchBuffer->begin(), ClosestPointIndex);
	     itStitch != StitchBuffer->end();
	     ++itStitch) {
	  if (((*itStitch).attribute & (ALTYPMSK | FRMSK)) != trg) {
		GroupStitchIndex = wrap::distance<uint32_t>(StitchBuffer->begin(), itStitch) - 1U;
		bFlag            = true;
		break;
	  }
	}
	if (!bFlag) {
	  GroupStitchIndex = wrap::toUnsigned(StitchBuffer->size() - 1U);
	}
  }
  else {
	ClosestPointIndex = 0;
	GroupStitchIndex  = 0;
  }
}

void form::selfil(uint32_t type) {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto const& currentForm = FormList->operator[](ClosestFormToCursor);
	if (type == FRMFIL && (currentForm.fillType == 0U)) {
	  displayText::tabmsg(IDS_FSELF, false);
	  return;
	}
	if (type == FRMBFIL && (currentForm.edgeType == 0U)) {
	  displayText::tabmsg(IDS_FSELB, false);
	  return;
	}
	if (type == FRMAPFIL && (currentForm.edgeType & NEGUND) != EDGEAPPL) {
	  displayText::tabmsg(IDS_FSELA, false);
	  return;
	}
	fi::frmpnts(type);
	StateMap->set(StateFlag::GRPSEL);
	StateMap->reset(StateFlag::FORMSEL);
	thred::rngadj();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_SEL1FRM, false);
  }
}

void form::selalfil() {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	auto const savedIndex = ClosestPointIndex;
	ClosestPointIndex     = thred::findFirstStitch(ClosestFormToCursor);
	if (ClosestPointIndex != StitchBuffer->size()) {
	  if (ClosestPointIndex != 0U) {
		--ClosestPointIndex;
	  }
	  GroupStitchIndex = thred::findLastStitch(ClosestFormToCursor);
	  StateMap->set(StateFlag::GRPSEL);
	  StateMap->reset(StateFlag::FORMSEL);
	  thred::rngadj();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  ClosestPointIndex = savedIndex;
	  displayText::tabmsg(IDS_FSELM, false);
	}
  }
  else {
	displayText::tabmsg(IDS_SEL1FRM, false);
  }
}

auto form::frmrng(uint32_t iForm, RANGE& range) -> bool {
  auto retval = false;
  if (!StitchBuffer->empty()) {
	range.start = 0;
	// clang-format off
	auto const& form      = FormList->operator[](iForm);
	auto const  saveClose = ClosestFormToCursor;
	// clang-format on
	ClosestFormToCursor = iForm;
	if ((form.fillType != 0U) || (form.edgeType != 0U)) {
	  range.start  = thred::findFirstStitch(ClosestFormToCursor);
	  range.finish = thred::findLastStitch(ClosestFormToCursor);
	  retval       = range.finish > range.start;
	}
	else {
	  range.finish = wrap::toUnsigned(StitchBuffer->size());
	}
	ClosestFormToCursor = saveClose;
  }
  return retval;
}

void fi::bholbrd(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  form.borderSize = BorderWidth;
  form::bsizpar(form);
  form.edgeType    = EDGEBHOL;
  form.edgeSpacing = LineSpacing;
  wrap::narrow(form.borderColor, ActiveColor);
  form::savblen(ButtonholeCornerLength);
  form::refilfn();
}

void form::bhol() {
  auto const savedSpacing = LineSpacing;
  if (displayText::filmsgs(FML_BHOL)) {
	return;
  }
  LineSpacing = savedSpacing;
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  fi::bholbrd(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  fi::bholbrd(ClosestFormToCursor);
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
  LineSpacing = savedSpacing;
}

void form::fcntr() {
  if (!SelectedFormList->empty()) {
	thred::savdo();
	// clang-format off
	auto const  firstForm     = SelectedFormList->front();
	auto const& firstRect     = FormList->operator[](firstForm).rectangle;
	auto const  initialCenter = F_POINT {wrap::midl(firstRect.right, firstRect.left), 
										 wrap::midl(firstRect.top, firstRect.bottom)};
	// clang-format on
	for (auto const selectedForm : (*SelectedFormList)) {
	  // clang-format off
	  auto&       currentForm   = FormList->operator[](selectedForm);
	  auto const& formRect      = currentForm.rectangle;
	  auto const  currentCenter = F_POINT {wrap::midl(formRect.right, formRect.left), 
		                                   wrap::midl(formRect.top, formRect.bottom)};
	  auto const  delta         = F_POINT {initialCenter.x - currentCenter.x, initialCenter.y - currentCenter.y};
	  auto        itVertex      = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
	  // clang-format on
	  for (auto iVertex = 0U; iVertex < currentForm.vertexCount; ++iVertex) {
		itVertex->x += delta.x;
		itVertex->y += delta.y;
		++itVertex;
	  }
	  currentForm.outline();
	  auto const codedForm = (selectedForm << FRMSHFT);
	  for (auto& stitch : *StitchBuffer) {
		if ((stitch.attribute & FRMSK) == codedForm && ((stitch.attribute & NOTFRM) == 0U)) {
		  stitch.x += delta.x;
		  stitch.y += delta.y;
		}
	  }
	}
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::tabmsg(IDS_SELC, false);
  }
}

void form::boxsel() {
  StateMap->reset(StateFlag::INSRT);
  if (!StateMap->test(StateFlag::THUMSHO)) {
	StateMap->reset(StateFlag::BZUM);
	StateMap->reset(StateFlag::BZUMIN);
	StateMap->set(StateFlag::VCAPT);
	SetCapture(ThrEdWindow);
	StateMap->set(StateFlag::RESTCH);
  }
}

void fi::fspic(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  form.edgeType       = EDGEPICOT;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = PicotSpacing;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  form::savplen(ButtonholeCornerLength);
  auto itClipPoints = wrap::next(ClipPoints->begin(), form.borderClipData);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoints = clip;
	++itClipPoints;
  }
  form::refilfn();
}

void form::picot() {
  if (displayText::filmsgs(FML_PIC)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	thred::savdo();
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fi::fspic(selectedForm);
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::fspic(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

auto fi::contsf(uint32_t formIndex) -> bool {
  ClosestFormToCursor = formIndex;
  if (auto& form = FormList->operator[](ClosestFormToCursor); form.vertexCount > 4) {
	clip::delclps(ClosestFormToCursor);
	texture::deltx(ClosestFormToCursor);
	form::chkcont();
	form.attribute |= gsl::narrow<decltype(form.attribute)>(ActiveLayer << 1U);
	wrap::narrow(form.fillColor, ActiveColor);
	form.fillSpacing = LineSpacing;
	form::fsizpar(form);
	form::refilfn();
	return true;
  }
  return false;
}

void form::contfil() {
  if (displayText::filmsgs(FML_CONT)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	thred::savdo();
	for (auto const selectedForm : (*SelectedFormList)) {
	  fi::contsf(selectedForm);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	thred::savdo();
	if (fi::contsf(ClosestFormToCursor)) {
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  StateMap->set(StateFlag::RESTCH);
	}
	else {
	  displayText::tabmsg(IDS_CONT, false);
	}
  }
}

void form::dupfn(float rotationAngle) {
  thred::savdo();
  auto usedAngle = rotationAngle;
  if (StateMap->test(StateFlag::ROTCLOCK) && (rotationAngle != 0.0F)) {
	usedAngle = PI_F2 - rotationAngle;
  }
  auto const rotationCenter = form::rotpar();
  if (usedAngle != 0.0F) {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  form::duprot(usedAngle);
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		fi::duprots(usedAngle, rotationCenter);
	  }
	  else {
		if (!SelectedFormList->empty()) {
		  fi::duprotfs(usedAngle);
		}
		else {
		  displayText::shord();
		}
	  }
	}
  }
  else {
	fi::rotentr(usedAngle);
	StateMap->set(StateFlag::ENTRDUP);
  }
}

void fi::shrnks() {
  auto  clipRect    = F_RECTANGLE {};
  auto  lengths     = std::vector<float> {};
  auto  deltas      = std::vector<F_POINT> {};
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  lengths.reserve(currentForm.vertexCount);
  deltas.reserve(currentForm.vertexCount - 1U);
  clip::oclp(clipRect, currentForm.borderClipData, currentForm.clipEntries);
  auto const itVertex = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
  auto const vMax     = gsl::narrow<ptrdiff_t>(currentForm.vertexCount - 1U);
  for (auto iVertex = ptrdiff_t {}; iVertex < vMax; ++iVertex) {
	auto const& thisVertex = itVertex[iVertex];
	auto const& nextVertex = itVertex[iVertex + 1];
	deltas.emplace_back(nextVertex.x - thisVertex.x, nextVertex.y - thisVertex.y);
	lengths.emplace_back(hypot(deltas.back().x, deltas.back().y));
  }
  auto length = lengths.begin();
  auto delta  = deltas.begin();
  for (auto iVertex = ptrdiff_t {}; iVertex < vMax; ++iVertex) {
	auto const  count      = std::floor(*length / ClipRectSize.cx);
	auto const  ratio      = (ClipRectSize.cx * count + 0.004F) / *length;
	auto const& thisVertex = itVertex[iVertex];
	auto&       nextVertex = itVertex[iVertex + 1];
	nextVertex.x           = thisVertex.x + delta->x * ratio;
	nextVertex.y           = thisVertex.y + delta->y * ratio;
	++length;
	++delta;
  }
  currentForm.outline();
  form::refil();
}

void form::shrnk() {
  if (StateMap->test(StateFlag::FORMSEL) && FormList->operator[](ClosestFormToCursor).edgeType == EDGECLIP) {
	thred::savdo();
	fi::shrnks();
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_FRMCLP, IDS_SHRNK);
  }
}

void fi::dufdat(std::vector<F_POINT>&  tempClipPoints,
                std::vector<SAT_CON>&  tempGuides,
                std::vector<F_POINT>&  destFormVertices,
                std::vector<FRM_HEAD>& destFormList,
                uint32_t               formIndex,
                uint32_t&              formRelocationIndex,
                uint32_t&              formSourceIndex) {
  auto& dest = destFormList.operator[](formRelocationIndex);

  dest = FormList->operator[](formIndex);
  ++formRelocationIndex;
  auto const itVertex = wrap::next(FormVertices->cbegin(), dest.vertexIndex);
  std::copy(itVertex, wrap::next(itVertex, dest.vertexCount), wrap::next(destFormVertices.begin(), formSourceIndex));
  dest.vertexIndex = formSourceIndex;
  formSourceIndex += dest.vertexCount;
  if (dest.satinGuideCount != 0U) {
	auto const itStartGuide = wrap::next(SatinGuides->cbegin(), dest.satinOrAngle.guide);
	auto const itEndGuide   = wrap::next(itStartGuide, dest.satinGuideCount);
	tempGuides.insert(tempGuides.end(), itStartGuide, itEndGuide);
	dest.satinOrAngle.guide = wrap::toUnsigned(tempGuides.size() - dest.satinGuideCount);
  }
  auto const& form = FormList->operator[](formIndex);
  if (form.isEdgeClipX()) {
	auto const itStartClip = wrap::next(ClipPoints->cbegin(), dest.borderClipData);
	auto const itEndClip   = wrap::next(itStartClip, dest.clipEntries);
	tempClipPoints.insert(tempClipPoints.end(), itStartClip, itEndClip);
	dest.borderClipData = wrap::toUnsigned(tempClipPoints.size() - dest.clipEntries);
  }
  if (form.isClipX()) {
	auto const itStartClip = wrap::next(ClipPoints->cbegin(), dest.angleOrClipData.clip);
	auto const itEndClip   = wrap::next(itStartClip, dest.lengthOrCount.clipCount);
	tempClipPoints.insert(tempClipPoints.end(), itStartClip, itEndClip);
	dest.angleOrClipData.clip = wrap::toUnsigned(tempClipPoints.size() - dest.lengthOrCount.clipCount);
  }
}

void fi::stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept {
  attribute &= NFRMSK;
  attribute |= formIndex << FRMSHFT;
}

void form::frmnumfn(uint32_t newFormIndex) {
  thred::savdo();
  if (newFormIndex != ClosestFormToCursor) {
	auto formRelocationIndex = 0U;
	auto sourceForm          = 0U;
	auto tempFormList        = std::vector<FRM_HEAD> {};
	tempFormList.resize(FormList->size());
	auto tempFormVertices = std::vector<F_POINT> {};
	tempFormVertices.resize(FormVertices->size());
	auto tempGuides = std::vector<SAT_CON> {};
	tempGuides.reserve(SatinGuides->size());
	auto tempClipPoints = std::vector<F_POINT> {};
	tempClipPoints.reserve(ClipPoints->size());
	auto formSourceIndex = 0U;
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size()); ++iForm) {
	  if (iForm == newFormIndex) {
		fi::dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, ClosestFormToCursor, formRelocationIndex, formSourceIndex);
	  }
	  else {
		if (sourceForm == ClosestFormToCursor) {
		  ++sourceForm;
		}
		fi::dufdat(tempClipPoints, tempGuides, tempFormVertices, tempFormList, sourceForm++, formRelocationIndex, formSourceIndex);
	  }
	}
	auto& formList = *FormList;
	std::ranges::copy(tempFormList, formList.begin());
	std::ranges::copy(tempFormVertices, FormVertices->begin());
	std::ranges::copy(tempGuides, SatinGuides->begin());
	std::ranges::copy(tempClipPoints, ClipPoints->begin());
	for (auto& stitch : *StitchBuffer) {
	  if ((stitch.attribute & SRTYPMSK) != 0U) {
		if (auto const decodedFormIndex = (stitch.attribute & FRMSK) >> FRMSHFT;
		    decodedFormIndex == ClosestFormToCursor) {
		  fi::stchfrm(newFormIndex, stitch.attribute);
		}
		else {
		  auto start  = newFormIndex;
		  auto finish = ClosestFormToCursor;
		  if (ClosestFormToCursor < newFormIndex) {
			std::swap(start, finish);
		  }
		  if (decodedFormIndex >= start && decodedFormIndex <= finish) {
			if (newFormIndex < ClosestFormToCursor) {
			  fi::stchfrm(decodedFormIndex + 1, stitch.attribute);
			}
			else {
			  fi::stchfrm(decodedFormIndex - 1, stitch.attribute);
			}
		  }
		}
	  }
	}
	ClosestFormToCursor = newFormIndex;
	displayText::ritnum(IDS_NUMFORM, ClosestFormToCursor);
  }
}

constexpr auto fi::duat(uint32_t attribute) -> uint32_t {
  auto const type = ((attribute >> TYPSHFT) + 1U) & 3U;
  auto const frm  = (attribute & FRMSK) >> 2U;
  return type | frm;
}

void fi::srtf(std::vector<F_POINT_ATTR> const& tempStitchBuffer, uint32_t start, uint32_t finish) {
  if (start != finish) {
	auto stitchHistogram = std::vector<uint32_t> {};
	stitchHistogram.resize(FormList->size() << 2U);
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  ++(stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]);
	}
	auto stitchAccumulator = start;
	for (auto iForm = 0U; iForm < wrap::toUnsigned(FormList->size() << 2U); ++iForm) {
	  auto const value       = stitchHistogram[iForm];
	  stitchHistogram[iForm] = stitchAccumulator;
	  stitchAccumulator += value;
	}
	for (auto iStitch = start; iStitch < finish; ++iStitch) {
	  StitchBuffer->operator[](stitchHistogram[duat(tempStitchBuffer[iStitch].attribute)]++) =
	      tempStitchBuffer[iStitch];
	}
  }
}

void form::srtbyfrm() {
  auto colorHistogram = std::vector<uint32_t> {};
  colorHistogram.resize(COLORCNT);
  auto color = std::vector<uint32_t> {};
  color.resize(COLORCNT);
  if (!FormList->empty()) {
	thred::savdo();
	std::iota(color.begin(), color.end(), 1);
	color[AppliqueColor]  = 0U;
	auto tempStitchBuffer = std::vector<F_POINT_ATTR> {};
	tempStitchBuffer.resize(StitchBuffer->size());
	for (auto const& stitch : *StitchBuffer) {
	  ++(colorHistogram[color[stitch.attribute & COLMSK]]);
	}
	auto colorAccumulator = 0U;
	for (auto& iColor : colorHistogram) {
	  auto const value = iColor;
	  iColor           = colorAccumulator;
	  colorAccumulator += value;
	}
	for (auto const& stitch : *StitchBuffer) {
	  tempStitchBuffer[colorHistogram[color[stitch.attribute & COLMSK]]++] = stitch;
	}
	fi::srtf(tempStitchBuffer, 0, colorHistogram[0]);
	for (auto iColor = 0U; iColor < (colorHistogram.size() - 1U); ++iColor) {
	  fi::srtf(tempStitchBuffer, colorHistogram[iColor], colorHistogram[wrap::toSize(iColor) + 1U]);
	}
  }
  else {
	xt::srtcol();
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::cntrx() {
  auto const markCenter =
      StateMap->test(StateFlag::GMRK)
          ? ZoomMarkPoint
          : F_POINT {wrap::toFloat(UnzoomedRect.cx) / 2.0F, wrap::toFloat(UnzoomedRect.cy) / 2.0F};
  bool flag = false;
  if (!SelectedFormList->empty()) {
	flag = true;
	thred::savdo();
	auto selectedCenter = F_POINT {};
	fi::dufcntr(selectedCenter);
	FormMoveDelta = F_POINT {markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y};
	if (StateMap->test(StateFlag::CNTRV)) {
	  FormMoveDelta.y = 0.0F;
	}
	if (StateMap->test(StateFlag::CNTRH)) {
	  FormMoveDelta.x = 0.0F;
	}
	for (auto const selectedForm : (*SelectedFormList)) {
	  frmadj(selectedForm);
	}
	frmsadj();
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  flag = true;
	  thred::savdo();
	  // clang-format off
	  auto const& formRect       = FormList->operator[](ClosestFormToCursor).rectangle;
	  auto const  selectedCenter = F_POINT {wrap::midl(formRect.right, formRect.left), 
										    wrap::midl(formRect.top, formRect.bottom)};
	  // clang-format on
	  FormMoveDelta = F_POINT {markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y};
	  if (StateMap->test(StateFlag::CNTRV)) {
		FormMoveDelta.y = 0.0F;
	  }
	  if (StateMap->test(StateFlag::CNTRH)) {
		FormMoveDelta.x = 0.0F;
	  }
	  frmadj(ClosestFormToCursor);
	  for (auto& stitch : *StitchBuffer) {
		if (((stitch.attribute & ALTYPMSK) != 0U) && (stitch.attribute & FRMSK) >> FRMSHFT == ClosestFormToCursor) {
		  stitch.x += FormMoveDelta.x;
		  stitch.y -= FormMoveDelta.y;
		}
	  }
	}
	else {
	  if (StateMap->test(StateFlag::GRPSEL)) {
		flag = true;
		thred::savdo();
		thred::rngadj();
		auto const& startStitch = StitchBuffer->operator[](GroupStartStitch);
		auto groupRect = F_RECTANGLE {startStitch.x, startStitch.y, startStitch.x, startStitch.y};
		for (auto iStitch = GroupStartStitch + 1U; iStitch <= GroupEndStitch; ++iStitch) {
		  auto const& stitch = StitchBuffer->operator[](iStitch);
		  if (stitch.x < groupRect.left) {
			groupRect.left = stitch.x;
		  }
		  if (stitch.y > groupRect.top) {
			groupRect.top = stitch.y;
		  }
		  if (stitch.x > groupRect.right) {
			groupRect.right = stitch.x;
		  }
		  if (stitch.y < groupRect.bottom) {
			groupRect.bottom = stitch.y;
		  }
		}
		auto const selectedCenter = F_POINT {wrap::midl(groupRect.right, groupRect.left),
		                                     wrap::midl(groupRect.top, groupRect.bottom)};
		FormMoveDelta = F_POINT {markCenter.x - selectedCenter.x, -markCenter.y + selectedCenter.y};
		if (StateMap->test(StateFlag::CNTRV)) {
		  FormMoveDelta.y = 0.0F;
		}
		if (StateMap->test(StateFlag::CNTRH)) {
		  FormMoveDelta.x = 0.0F;
		}
		for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
		  StitchBuffer->operator[](iStitch).x += FormMoveDelta.x;
		  StitchBuffer->operator[](iStitch).y -= FormMoveDelta.y;
		}
	  }
	  else {
		displayText::shoseln(IDS_FGRPF, IDS_CENT);
	  }
	}
  }
  if (flag) {
	StateMap->set(StateFlag::RESTCH);
  }
}

void form::centir() {
  StateMap->reset(StateFlag::BIGBOX);
  *AllItemsRect         = fi::getbig(*FormList, *StitchBuffer);
  auto const itemCenter = F_POINT {wrap::midl(AllItemsRect->right, AllItemsRect->left),
                                   wrap::midl(AllItemsRect->top, AllItemsRect->bottom)};
  auto const hoopCenter =
      F_POINT {wrap::toFloat(UnzoomedRect.cx) / 2.0F, wrap::toFloat(UnzoomedRect.cy) / 2.0F};
  auto const delta = F_POINT {hoopCenter.x - itemCenter.x, hoopCenter.y - itemCenter.y};
  for (auto& stitch : *StitchBuffer) {
	stitch.x += delta.x;
	stitch.y += delta.y;
  }
  for (auto& formVertice : *FormVertices) {
	formVertice.x += delta.x;
	formVertice.y += delta.y;
  }
  for (auto& form : *FormList) {
	form.outline();
  }
  xt::setfchk();
  StateMap->set(StateFlag::RESTCH);
}

auto fi::bean(uint32_t start, uint32_t finish) -> uint32_t {
  auto const stitchRange      = (finish - start) * 3U; // each bean is 3 stitches
  auto       highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.reserve(stitchRange);
  auto iSourceStitch = start;
  auto beanCount     = 0U;
  for (auto loop = 0; loop < 2; ++loop) {
	auto const& stitch     = StitchBuffer->operator[](iSourceStitch);
	auto const& stitchFwd1 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 1U);
	auto const& stitchFwd2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 2U);
	highStitchBuffer.push_back(stitch);
	if (!util::closeEnough(stitchFwd2.x, stitch.x) || !util::closeEnough(stitchFwd2.y, stitch.y)) {
	  highStitchBuffer.push_back(stitchFwd1);
	  highStitchBuffer.push_back(stitch);
	  beanCount += 2U;
	}
	++iSourceStitch;
  }
  while (iSourceStitch < finish - 1) {
	auto const& stitch      = StitchBuffer->operator[](iSourceStitch);
	auto const& stitchFwd1  = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 1U);
	auto const& stitchFwd2  = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 2U);
	auto const& stitchBack2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) - 2U);
	highStitchBuffer.push_back(stitch);
	if ((!util::closeEnough(stitchFwd2.x, stitch.x) || !util::closeEnough(stitchFwd2.y, stitch.y)) &&
	    (!util::closeEnough(stitchBack2.x, stitch.x) || !util::closeEnough(stitchBack2.y, stitch.y))) {
	  highStitchBuffer.push_back(stitchFwd1);
	  highStitchBuffer.push_back(stitch);
	  beanCount += 2U;
	}
	++iSourceStitch;
  }
  auto const& stitch      = StitchBuffer->operator[](iSourceStitch);
  auto const& stitchFwd1  = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 1U);
  auto const& stitchBack2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) - 2U);
  highStitchBuffer.push_back(stitch);
  if (!util::closeEnough(stitchBack2.x, stitch.x) || !util::closeEnough(stitchBack2.y, stitch.y)) {
	highStitchBuffer.push_back(stitchFwd1);
	highStitchBuffer.push_back(stitch);
	beanCount += 2U;
  }
  // now copy stitches back up to the end of the original group
  std::copy(highStitchBuffer.cbegin(),
            wrap::next(highStitchBuffer.cbegin(), finish - start),
            wrap::next(StitchBuffer->begin(), start));
  // and then insert the remainder of the new stitches
  StitchBuffer->insert(wrap::next(StitchBuffer->cbegin(), finish),
                       wrap::next(highStitchBuffer.begin(), finish - start),
                       highStitchBuffer.end());
  return beanCount;
}

void form::dubean() {
  if (!StitchBuffer->empty()) {
	thred::savdo();
	if (StateMap->test(StateFlag::GRPSEL)) {
	  thred::rngadj();
	  if (auto const beanCount = fi::bean(GroupStartStitch, GroupEndStitch); ClosestPointIndex > GroupStitchIndex) {
		ClosestPointIndex += beanCount;
	  }
	  else {
		GroupStitchIndex += beanCount;
	  }
	  thred::grpAdj();
	}
	else {
	  fi::bean(0, wrap::toUnsigned(StitchBuffer->size() - 1U));
	}
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}

void fi::unbean(uint32_t start, uint32_t& finish) {
  auto const stitchRange      = ((finish - start) / 3U) + 1U; // each bean is 3 stitches
  auto       highStitchBuffer = std::vector<F_POINT_ATTR> {};
  highStitchBuffer.reserve(stitchRange);
  auto lastStitch = finish;
  if (lastStitch > wrap::toUnsigned(StitchBuffer->size()) - 3U) {
	lastStitch = wrap::toUnsigned(StitchBuffer->size()) - 3U;
  }
  auto iSourceStitch = start;
  for (; iSourceStitch <= lastStitch; ++iSourceStitch) {
	auto const& stitch     = StitchBuffer->operator[](iSourceStitch);
	auto const& stitchFwd2 = StitchBuffer->operator[](wrap::toSize(iSourceStitch) + 2U);
	highStitchBuffer.push_back(stitch);
	if (util::closeEnough(stitch.x, stitchFwd2.x) && util::closeEnough(stitch.y, stitchFwd2.y)) {
	  iSourceStitch += 2;
	}
  }
  if (finish != lastStitch) {
	while (iSourceStitch != finish + 1U) {
	  highStitchBuffer.push_back(StitchBuffer->operator[](iSourceStitch++));
	}
  }
  if ((finish - start) > highStitchBuffer.size()) {
	auto const itStartStitch = wrap::next(StitchBuffer->begin(), start);
	std::ranges::copy(highStitchBuffer, itStartStitch);
	StitchBuffer->erase(wrap::next(StitchBuffer->cbegin(), start + highStitchBuffer.size()),
	                    wrap::next(StitchBuffer->cbegin(), finish + 1U));
  }
  finish = start + wrap::toUnsigned(highStitchBuffer.size() - 1U);
}

void form::debean() {
  thred::savdo();
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::rngadj();
	fi::unbean(GroupStartStitch, GroupEndStitch);
	if (ClosestPointIndex > GroupStitchIndex) {
	  ClosestPointIndex = GroupEndStitch;
	}
	else {
	  GroupStitchIndex = GroupEndStitch;
	}
	auto const iEndStitch = wrap::toUnsigned(StitchBuffer->size()) - 1U;
	if (ClosestPointIndex > iEndStitch - 1U) {
	  ClosestPointIndex = iEndStitch;
	}
	if (GroupStitchIndex > iEndStitch - 1U) {
	  GroupStitchIndex = iEndStitch;
	}
	thred::grpAdj();
  }
  else {
	if (StitchBuffer->size() > 2U) {
	  auto iEndStitch = wrap::toUnsigned(StitchBuffer->size()) - 1U;
	  fi::unbean(0, iEndStitch);
	}
  }
  thred::coltab();
  StateMap->set(StateFlag::RESTCH);
}

void form::clpspac(uint32_t const insertPoint, uint32_t count) {
  auto const itStartClip = wrap::next(ClipPoints->cbegin(), insertPoint);
  auto const itEndClip   = wrap::next(itStartClip, count);
  ClipPoints->insert(itStartClip, itStartClip, itEndClip);
}

void form::stchadj() {
  auto const     codedClosest = ClosestFormToCursor << FRMSHFT;
  constexpr auto OFFSET       = 1U << FRMSHFT; // coded next form offset
  for (auto& stitch : *StitchBuffer) {
	if (auto codedForm = stitch.attribute & FRMSK; codedForm > codedClosest) {
	  stitch.attribute &= NFRMSK;
	  codedForm += OFFSET;
	  stitch.attribute |= codedForm;
	}
  }
  form::refilfn();
  ++ClosestFormToCursor;
  form::refilfn();
  StateMap->reset(StateFlag::FRMPSEL);
}

auto fi::spltlin() -> bool {
  auto& srcForm = FormList->operator[](ClosestFormToCursor);
  if (ClosestVertexToCursor < 1 || srcForm.vertexCount - ClosestVertexToCursor < 2) {
	return false;
  }
  auto const itVertex = wrap::next(FormVertices->cbegin(), srcForm.vertexIndex + ClosestVertexToCursor);
  fi::nufpnt(ClosestVertexToCursor, srcForm, *itVertex);
  form::frmlin(srcForm);
  FormList->insert(wrap::next(FormList->cbegin(), ClosestFormToCursor), 1, srcForm);
  // insert may invalidate the srcForm reference, so build a new one
  auto& form       = FormList->operator[](ClosestFormToCursor);
  auto& dstForm    = FormList->operator[](wrap::toSize(ClosestFormToCursor) + 1U);
  form.vertexCount = ClosestVertexToCursor + 1U;
  dstForm.vertexCount -= form.vertexCount;
  dstForm.vertexIndex = form.vertexIndex + form.vertexCount;
  form.outline();
  dstForm.outline();
  if (form.isEdgeClip()) {
	form::clpspac(form.borderClipData, form.clipEntries);
	auto const maxForm = FormList->size();
	for (auto iForm = ClosestFormToCursor + 1U; iForm < maxForm; ++iForm) {
	  FormList->operator[](iForm).borderClipData += form.clipEntries;
	}
  }
  form::stchadj();
  return true;
}

void form::spltfrm() {
  if (StateMap->test(StateFlag::FRMPSEL)) {
	thred::savdo();
	if (auto const& currentForm = FormList->operator[](ClosestFormToCursor); currentForm.type == SAT) {
	  if (currentForm.satinGuideCount != 0U) {
		auto itGuide = wrap::next(SatinGuides->cbegin(), currentForm.satinOrAngle.guide);
		for (auto guideIndex = 0U; guideIndex < currentForm.satinGuideCount; ++guideIndex) {
		  if (itGuide->start == ClosestVertexToCursor || itGuide->finish == ClosestVertexToCursor) {
			satin::spltsat(guideIndex);
			return;
		  }
		  ++itGuide;
		}
	  }
	}
	else {
	  if (currentForm.type == FRMLINE) {
		if (fi::spltlin()) {
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  displayText::tabmsg(IDS_FRM3, false);
		}
		return;
	  }
	}
  }
  displayText::spltmsg();
}

void form::stchs2frm() {
  if (StateMap->test(StateFlag::GRPSEL)) {
	thred::savdo();
	thred::rngadj();
	// ToDo - Is this still a valid restriction?
	if (constexpr auto STITCHMX = 12000U; // maximum number of stitches in a group
	    (GroupEndStitch - GroupStartStitch) > STITCHMX) {
	  displayText::tabmsg(IDS_STMAX, false);
	  return;
	}
	auto const vertexCount  = GroupEndStitch - GroupStartStitch + 1U;
	auto       currentForm  = FRM_HEAD {};
	currentForm.type        = FRMLINE;
	currentForm.vertexCount = vertexCount;
	currentForm.vertexIndex = thred::adflt(vertexCount);
	auto itVertex           = wrap::next(FormVertices->begin(), currentForm.vertexIndex);
	for (auto iStitch = GroupStartStitch; iStitch <= GroupEndStitch; ++iStitch) {
	  *itVertex = StitchBuffer->operator[](iStitch);
	  ++itVertex;
	}
	currentForm.outline();
	FormList->push_back(currentForm);
	thred::delstchm();
	StateMap->reset(StateFlag::GRPSEL);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_GRPMSG, IDS_STCH2FRM);
  }
}

void form::vrtsclp(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::delmclp(formIndex);
  texture::deltx(formIndex);
  form.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
  form.angleOrClipData.clip    = clip::numclp(formIndex);
  form.wordParam               = IniFile.fillPhase;
  fi::makpoli();
  form.fillSpacing = IniFile.clipOffset;
  auto itClipPoint = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  form.fillType = VCLPF;
  wrap::narrow(form.fillColor, ActiveColor);
  form.type = FRMFPOLY;
  refilfn();
}

void form::vrtclp() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  StateMap->set(StateFlag::NOCLP);
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
			  vrtsclp(ClosestFormToCursor);
			}
		  }
		  StateMap->reset(StateFlag::NOCLP);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			vrtsclp(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::horsclp() {
  auto& form = FormList->operator[](ClosestFormToCursor);
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  auto const clipSize          = wrap::toUnsigned(ClipBuffer->size());
  form.lengthOrCount.clipCount = clipSize;
  form.angleOrClipData.clip    = clip::numclp(ClosestFormToCursor);
  form.lengthOrCount.clipCount = clipSize;
  form.wordParam               = IniFile.fillPhase;
  fi::makpoli();
  form.fillSpacing = IniFile.clipOffset;
  auto itClipPoint = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  form.fillType = HCLPF;
  wrap::narrow(form.fillColor, ActiveColor);
  form.type = FRMFPOLY;
  form::refilfn();
}

void form::horclp() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  StateMap->set(StateFlag::NOCLP);
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (FormList->operator[](ClosestFormToCursor).type != FRMLINE) {
			  horsclp();
			}
		  }
		  StateMap->set(StateFlag::NOCLP);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			horsclp();
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::angsclp(FRM_HEAD& form) {
  clip::delmclp(ClosestFormToCursor);
  texture::deltx(ClosestFormToCursor);
  form.angleOrClipData.clip    = clip::numclp(ClosestFormToCursor);
  form.lengthOrCount.clipCount = wrap::toUnsigned(ClipBuffer->size());
  form.wordParam               = IniFile.fillPhase;
  fi::makpoli();
  form.satinOrAngle.angle = IniFile.fillAngle;
  form.fillSpacing        = IniFile.clipOffset;
  auto itClipPoint        = wrap::next(ClipPoints->begin(), form.angleOrClipData.clip);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  form.fillType = ANGCLPF;
  wrap::narrow(form.fillColor, ActiveColor);
  form.type = FRMFPOLY;
  form::refilfn();
}

void form::angclp() {
  if (displayText::filmsgs(FMM_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cy > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  StateMap->set(StateFlag::NOCLP);
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			if (auto& form = FormList->operator[](selectedForm); form.type != FRMLINE) {
			  angsclp(form);
			}
		  }
		  StateMap->reset(StateFlag::NOCLP);
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			auto& form = FormList->operator[](ClosestFormToCursor);
			angsclp(form);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  CloseClipboard();
	}
  }
}

void form::dubsfil(FRM_HEAD& form) {
  clip::deleclp(ClosestFormToCursor);
  form.edgeType = EDGEDOUBLE;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  fi::dubfn(form);
  form::refilfn();
}

void form::dubfil() {
  if (displayText::filmsgs(FML_LIN)) {
	return;
  }
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  auto& form          = FormList->operator[](selectedForm);
	  dubsfil(form);
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  auto& form = FormList->operator[](ClosestFormToCursor);
	  dubsfil(form);
	  thred::coltab();
	  StateMap->set(StateFlag::INIT);
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::col2frm() {
  auto colorChangedCount = 0U;
  if (!FormList->empty()) {
	auto const formColorPermutations = FormList->size() << 4U; // total number of form and color combinations
	auto fillColorHistogram = std::vector<uint32_t> {};
	fillColorHistogram.resize(formColorPermutations);
	auto borderColorHistogram = std::vector<uint32_t> {};
	borderColorHistogram.resize(formColorPermutations);
	auto featherColorHistogram = std::vector<uint32_t> {};
	featherColorHistogram.resize(formColorPermutations);
	auto underlayColorHistogram = std::vector<uint32_t> {};
	underlayColorHistogram.resize(formColorPermutations);
	for (auto const& stitch : *StitchBuffer) {
	  if (auto const formColorCode = stitch.attribute & (COLMSK | FRMSK);
	      (stitch.attribute & (WLKMSK | CWLKMSK | UNDMSK)) != 0U) {
		++(underlayColorHistogram[formColorCode]);
	  }
	  else {
		if ((stitch.attribute & FTHMSK) != 0U) {
		  ++(featherColorHistogram[formColorCode]);
		}
		else {
		  switch (stitch.attribute & TYPMSK) {
			case FRMFIL: {
			  ++(fillColorHistogram[formColorCode]);
			  break;
			}
			case FRMBFIL: {
			  ++(borderColorHistogram[formColorCode]);
			  break;
			}
			default: {
			  outDebugString(L"default hit in col2frm: attribute [{}]\n", stitch.attribute & TYPMSK);
			  break;
			}
		  }
		}
	  }
	}
	auto startColorOffset = 0U;
	auto endColorOffset   = COLORCNT;
	for (auto& formIter : *FormList) {
	  if (formIter.fillType != 0U) {
		auto count         = 0U;
		auto majorityColor = 0U;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (fillColorHistogram[iColor] > count) {
			count         = fillColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLMSK;
		if (formIter.fillColor != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(formIter.fillColor, majorityColor);
		}
		if (formIter.fillType == FTHF && ((formIter.extendedAttribute & AT_FTHBLND) != 0U)) {
		  count = majorityColor = 0;
		  for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
			if (featherColorHistogram[iColor] > count) {
			  count         = fillColorHistogram[iColor];
			  majorityColor = iColor;
			}
		  }
		  majorityColor &= COLMSK;
		  if (formIter.fillInfo.feather.color != majorityColor) {
			++colorChangedCount;
			wrap::narrow(formIter.fillInfo.feather.color, majorityColor);
		  }
		}
	  }
	  if (formIter.edgeType != 0U) {
		auto count         = 0U;
		auto majorityColor = 0U;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (borderColorHistogram[iColor] > count) {
			count         = borderColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLMSK;
		if (formIter.borderColor != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(formIter.borderColor, majorityColor);
		}
	  }
	  if ((formIter.extendedAttribute & (AT_WALK | AT_CWLK | AT_UND)) != 0U) {
		auto count         = 0U;
		auto majorityColor = 0U;
		for (auto iColor = startColorOffset; iColor < endColorOffset; ++iColor) {
		  if (underlayColorHistogram[iColor] > count) {
			count         = borderColorHistogram[iColor];
			majorityColor = iColor;
		  }
		}
		majorityColor &= COLMSK;
		if (formIter.underlayColor != majorityColor) {
		  ++colorChangedCount;
		  wrap::narrow(formIter.underlayColor, majorityColor);
		}
	  }
	  startColorOffset += COLORCNT;
	  endColorOffset += COLORCNT;
	}
  }
  displayText::showMessage(IDS_NCOLCHG, colorChangedCount);
}

void form::chan() {
  auto& currentForm = FormList->operator[](ClosestFormToCursor);
  wrap::narrow(currentForm.borderColor, ActiveColor);
  currentForm.edgeSpacing        = IniFile.chainSpace;
  currentForm.borderSize         = BorderWidth;
  currentForm.edgeStitchLen      = IniFile.chainRatio;
  currentForm.maxBorderStitchLen = IniFile.maxStitchLength;
  currentForm.minBorderStitchLen = MinStitchLength;
  if (StateMap->test(StateFlag::LINCHN)) {
	currentForm.edgeType = EDGELCHAIN;
  }
  else {
	currentForm.edgeType = EDGEOCHAIN;
  }
  form::refilfn();
}

void form::chain() {
  if (displayText::filmsgs(FML_CHAIN)) {
	return;
  }
  thred::savdo();
  if (!SelectedFormList->empty()) {
	for (auto const selectedForm : (*SelectedFormList)) {
	  ClosestFormToCursor = selectedForm;
	  chan();
	}
	StateMap->set(StateFlag::INIT);
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	if (StateMap->test(StateFlag::FORMSEL)) {
	  chan();
	  StateMap->set(StateFlag::INIT);
	  thred::coltab();
	  thred::ritot(wrap::toUnsigned(StitchBuffer->size()));
	  StateMap->set(StateFlag::RESTCH);
	}
  }
}

void form::crop() {
  displayText::frm1pnt();
  if (StateMap->test(StateFlag::FORMSEL)) {
	thred::savdo();
	// clang-format off
	auto const& form         = FormList->operator[](ClosestFormToCursor);
	auto        iDestination = StitchBuffer->begin();
	// clang-format on
	for (auto const& stitch : *StitchBuffer) {
	  if (form::cisin(form, stitch.x, stitch.y)) {
		*iDestination++ = stitch;
	  }
	}
	StitchBuffer->resize(wrap::distance<size_t>(StitchBuffer->begin(), iDestination));
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
  else {
	displayText::shoseln(IDS_FRM1MSG, IDS_CROP);
  }
}

void fi::fsclpx(uint32_t formIndex) {
  auto& form = FormList->operator[](formIndex);
  clip::deleclp(formIndex);
  auto const clipSize = wrap::toUnsigned(ClipBuffer->size());
  form.edgeType       = EDGECLIPX;
  form.clipEntries    = clipSize;
  form.borderClipData = clip::nueclp(ClosestFormToCursor, clipSize);
  form.borderSize     = ClipRectSize.cy;
  form.edgeSpacing    = ClipRectSize.cx;
  wrap::narrow(form.borderColor, ActiveColor);
  form::bsizpar(form);
  auto itClipPoint = wrap::next(ClipPoints->begin(), form.borderClipData);
  for (auto const& clip : *ClipBuffer) {
	*itClipPoint = clip;
	++itClipPoint;
  }
  clip::duxclp(form);
  form::refilfn();
}

void form::filclpx() {
  if (displayText::filmsgs(FML_CLP)) {
	return;
  }
  if (OpenClipboard(ThrEdWindow) != 0) {
	Clip       = RegisterClipboardFormat(PcdClipFormat);
	ClipMemory = GetClipboardData(Clip);
	if (ClipMemory != nullptr) {
	  thred::savdo();
	  thred::redclp();
	  CloseClipboard();
	  if (ClipRectSize.cx > CLPMIN) {
		if (!SelectedFormList->empty()) {
		  for (auto const selectedForm : (*SelectedFormList)) {
			ClosestFormToCursor = selectedForm;
			fi::fsclpx(selectedForm);
		  }
		  StateMap->set(StateFlag::INIT);
		  thred::coltab();
		  StateMap->set(StateFlag::RESTCH);
		}
		else {
		  if (StateMap->test(StateFlag::FORMSEL)) {
			fi::fsclpx(ClosestFormToCursor);
			StateMap->set(StateFlag::INIT);
			thred::coltab();
			StateMap->set(StateFlag::RESTCH);
		  }
		}
	  }
	  else {
		displayText::tabmsg(IDS_CLP, false);
	  }
	}
	else {
	  auto const str = std::wstring(L"no clipboard data");
	  displayText::shoMsg(str, false);
	  CloseClipboard();
	}
  }
}

void form::srtfrm() {
  auto histogram = std::vector<uint32_t> {};
  histogram.resize(FormList->size());
  if (!StitchBuffer->empty()) {
	thred::savdo();
	for (auto const& stitch : *StitchBuffer) {
	  auto const iForm = (stitch.attribute & FRMSK) >> FRMSHFT;
	  ++(histogram[iForm]);
	}
	auto totalStitches = 0U;
	for (auto& entry : histogram) {
	  auto const formStitchCount = entry;
	  entry                      = totalStitches;
	  totalStitches += formStitchCount;
	}
	auto highStitchBuffer = std::vector<F_POINT_ATTR> {};
	highStitchBuffer.resize(StitchBuffer->size());
	for (auto const& stitch : *StitchBuffer) {
	  auto const iForm              = (stitch.attribute & FRMSK) >> FRMSHFT;
	  auto const iHighStitch        = histogram[iForm]++;
	  highStitchBuffer[iHighStitch] = stitch;
	}
	std::ranges::copy(highStitchBuffer, StitchBuffer->begin());
	thred::coltab();
	StateMap->set(StateFlag::RESTCH);
  }
}
