#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <vector>

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26814)
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace form {

void angclp();
void angclpfn(FRM_HEAD const& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
void angsclp(FRM_HEAD& form);
void apliq();
void bakagain();
void bakdup();
void bhol();
void boxsel();
void bord();
void bsizpar(FRM_HEAD& currentForm) noexcept;
void centir();
void chain();
void chan();
void chkcont();
auto chkdel(FRM_HEAD const& currentForm) noexcept -> bool;
auto chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
auto chkmax(uint32_t arg0, uint32_t arg1) noexcept -> bool;
void chkseq(bool border);
auto cisin(FRM_HEAD const& form, float xCoordinate, float yCoordinate) -> bool;
auto closflt(FRM_HEAD const& form, float xCoordinate, float yCoordinate) -> uint32_t;
auto closfrm() -> bool;
void clpfil();
void clpspac(uint32_t insertPoint, uint32_t count);
void clrfills() noexcept;
void cntrx();
void col2frm();
void contfil();
void cpylayr(uint32_t layer);
void crop();
void debean();
void delflt(uint32_t formIndex);
void delfrms();
void delmfil(uint32_t formIndex);
void drwcon();
void drwfrm();
void duangs(FRM_HEAD const& form);
void dubean();
void dubfil();
void dubig();
void dubold();
void dubsfil(FRM_HEAD& form);
void dueg(uint32_t sides);
void duform(int32_t formType);
void dufrm() noexcept;
void duhart(uint32_t sideCount);
void dulens(uint32_t sides);
void duinsf() noexcept;
void duprot(float rotationAngle);
void dupsel(HDC dc);
void durpoli(uint32_t vertexCount);
void duspir(uint32_t stepCount);
void dustar(uint32_t starCount, float length);
void duzig(uint32_t vertices);
void fclp();
void fcntr();
void filangl();
void filclpx();
void filhor();
auto filin(F_POINT const& currentPoint, F_POINT const& stitchPoint) -> F_POINT;
void filinsb(F_POINT const& point, F_POINT& stitchPoint);
auto filinu(F_POINT const& inPoint, F_POINT const& stitchPoint) -> F_POINT;
void filsat();
void filvrt();
void fliph();
void flipv();
void flpord();
void fltspac(uint32_t vertexOffset, uint32_t count);
void form();
void frm0();
void frmadj(uint32_t formIndex);
void frmlin(FRM_HEAD const& form);
void frmlin(std::vector<F_POINT> const& vertices);
void frmnumfn(uint32_t newFormIndex);
void frmon();
void frmovlin();
auto frmrng(uint32_t iForm, RANGE& range) -> bool;
void frmsadj();
void fselrct(uint32_t iForm);
void fsizpar(FRM_HEAD& form) noexcept;
auto getblen() noexcept -> float;
auto getlast(FRM_HEAD const& form) -> uint32_t;
auto getplen() noexcept -> float;
void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
auto lastch() noexcept -> bool;
auto linx(std::vector<F_POINT> const& points, uint32_t start, uint32_t finish, F_POINT& intersection) noexcept
    -> bool;
void mdufrm() noexcept;
void movlayr(uint32_t layer);
void munfrm();
void nubrdcol(uint8_t color) noexcept;
void nufilcol(uint8_t color) noexcept;
void nufsel();
void nufthcol(uint8_t color) noexcept;
void nulapcol(uint8_t color);
auto nxt(FRM_HEAD const& form, uint32_t iVertex) noexcept -> uint32_t;
auto pdir(FRM_HEAD const& form, uint32_t vertex) -> uint32_t;
void picot();
void prpbrd(float borderStitchSpacing);
auto prv(FRM_HEAD const& form, uint32_t iVertex) noexcept -> uint32_t;
auto psg() noexcept -> uint32_t;
void pxrct2stch(RECT const& screenRect, F_RECTANGLE& stitchRect) noexcept;
void ratsr();
void rct2sel(RECT const& rectangle, std::vector<POINT>& line) noexcept;
void redup();
void refil();
void refilal();
void refilfn();
void rinfrm();
void ritfrct(uint32_t iForm, HDC dc);
void rotagain();
void rotcmd();
void rotdup();
void rotfrm(FRM_HEAD& form, uint32_t newStartVertex);
auto rotpar() -> F_POINT;
void rstfrm();
auto sRct2px(F_RECTANGLE const& stitchRect) -> RECT;
void savblen(float fLength);
void savplen(float length);
void selal();
void selalfil();
void selfil(uint32_t type);
void selsqr(POINT const& controlPoint, HDC dc);
void setap();
void setexpand(float xyRatio);
void setfpnt();
void setfrm();
void sethup() noexcept;
void setins();
void setmfrm();
void setrang();
void setstrtch();
auto sfCor2px(F_POINT const& stitchPoint) -> POINT;
void shrnk();
void snap();
void spltfrm();
void srtbyfrm();
void srtfrm();
void stchadj();
void stchrct2px(F_RECTANGLE const* stitchRect, RECT& screenRect);
void stchs2frm();
void tglfrm();
void unfil();
void unfrm();
void uninsf();
void unpsel();
void vrtclp();
void vrtsclp();

namespace internal {

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
  void brkdun(std::vector<SMAL_PNT_L*> const& sortedLines,
              uint32_t                        start,
              uint32_t                        finish,
              std::vector<F_POINT>&           workingFormVertices);
  void brkseq(std::vector<SMAL_PNT_L*> const& sortedLines,
              uint32_t                        start,
              uint32_t                        finish,
              boost::dynamic_bitset<>&        sequenceMap,
              uint32_t&                       lastGroup,
              SMAL_PNT_L*                     sequenceLines);
  void chkbrd(FRM_HEAD const& form);
  void chksid(FRM_HEAD const& form, uint32_t vertexIndex, uint32_t clipIntersectSide, std::vector<F_POINT> const& currentFormVertices);
  auto closat(IntersectionStyles& inOutFlag) -> bool;
  auto vclpComp(V_CLP_X const& vclpx1, V_CLP_X const& vclpx2) noexcept -> bool;
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
  auto fplComp(F_POINT_LINE const& point1, F_POINT_LINE const& point2) noexcept -> bool;
  void contf(FRM_HEAD& form);
  auto contsf(uint32_t formIndex) -> bool;
  void cplayfn(uint32_t iForm, uint32_t layer);

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
  void dunseq(std::vector<SMAL_PNT_L*> const& sortedLines, uint32_t start, uint32_t finish, uint32_t& lastGroup);
  void dupfn(float rotationAngle);
  void duprotfs(float rotationAngle);
  void duprots(float rotationAngle, F_POINT const& rotationCenter);
  void durgn(FRM_HEAD const&                 form,
             std::vector<F_SEQ> const&       sequencePath,
             boost::dynamic_bitset<>&        visitedRegions,
             std::vector<SMAL_PNT_L*> const& sortedLines,
             uint32_t                        pthi,
             uint32_t                        lineCount,
             std::vector<REGION> const&      regionsList,
             uint32_t&                       lastGroup,
             uint32_t                        sequencePathIndex,
             std::vector<F_POINT>&           workingFormVertices);
  void duromb(F_POINT const& start0, F_POINT const& finish0, F_POINT const& start1, F_POINT const& finish1, F_POINT& stitchPoint);
  void duseq(std::vector<SMAL_PNT_L*> const& sortedLines,
             uint32_t                        start,
             uint32_t                        finish,
             boost::dynamic_bitset<>&        sequenceMap,
             uint32_t&                       lastGroup,
             SMAL_PNT_L const*               sequenceLines);
  void duseq1(SMAL_PNT_L const* sequenceLines);
  void duseq2(SMAL_PNT_L const* sequenceLines);
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
  void frmpnts(uint32_t type);
  void frmpoly(POINT const* line, uint32_t count) noexcept;
  void frmsqr(uint32_t vertexIndex, uint32_t iVertex);
  void frmsqr0(POINT const& controlPoint);
  void frmx(POINT const& controlPoint, HDC dc);
  void fsangl(FRM_HEAD& form);
  void fsclp(uint32_t formIndex);
  void fsclpx(uint32_t formIndex);
  void fshor(FRM_HEAD& form);
  void fspic(uint32_t formIndex);
  void fsvrt();
  void getbig(F_RECTANGLE* allItemsRect) noexcept;
  auto getlen(std::vector<CLIP_PNT>&      clipStitchPoints,
              std::vector<float> const&   lengths,
              uint32_t                    iPoint,
              std::vector<F_POINT> const& currentFormVertices) -> float;
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
  auto isclos(SMAL_PNT_L const* lineEndPoint0, SMAL_PNT_L const* lineEndPoint1, float gapToClosestRegion) noexcept
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
  auto lenComp(LEN_INFO const& arg1, LEN_INFO const& arg2) noexcept -> bool;
  auto clipComp(CLIP_SORT const* arg1, CLIP_SORT const* arg2) noexcept -> bool;
  auto lnclos(std::vector<uint32_t> const&   groupIndexSequence,
              std::vector<SMAL_PNT_L> const& lineEndpoints,
              uint32_t                       group0,
              uint32_t                       line0,
              uint32_t                       group1,
              uint32_t                       line1,
              float                          gapToClosestRegion) noexcept -> bool;
  void makpoli();
  void movseq(std::vector<SMAL_PNT_L*> const& sortedLines, uint32_t ind);
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
  void doTimeWindow(float rangeX, std::vector<uint32_t> const& xPoints, std::vector<uint32_t> const& xHistogram);
  void nxtrgn(std::vector<RG_SEQ>&            tempPath,
              std::vector<R_CON> const&       pathMap,
              std::vector<uint32_t> const&    mapIndexSequence,
              boost::dynamic_bitset<>&        visitedRegions,
              std::vector<SMAL_PNT_L*> const& sortedLines,
              std::vector<REGION> const&      regionsList,
              uint32_t&                       doneRegion,
              uint32_t                        pathMapIndex,
              uint32_t&                       sequencePathIndex,
              int32_t                         visitedIndex);
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
  auto regclos(std::vector<uint32_t> const&    groupIndexSequence,
               std::vector<SMAL_PNT_L> const&  lineEndpoints,
               std::vector<SMAL_PNT_L*> const& sortedLines,
               uint32_t                        iRegion0,
               uint32_t                        iRegion1,
               std::vector<REGION> const&      regionsList,
               float                           gapToClosestRegion,
               uint32_t&                       nextGroup) noexcept -> bool;
  auto reglen(std::vector<SMAL_PNT_L*> const&      sortedLines,
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
  void spend(std::vector<V_RECT_2> const& fillVerticalRect, uint32_t start, uint32_t finish, F_POINT& stitchPoint);
  auto spltlin() -> bool;
  void sprct(std::vector<F_POINT> const* vertices,
             uint32_t                    vertexIndex,
             std::vector<V_RECT_2>&      fillVerticalRect,
             uint32_t                    start,
             uint32_t                    finish);
  void spurct(std::vector<V_RECT_2>&       underlayVerticalRect,
              std::vector<V_RECT_2> const& fillVerticalRect,
              uint32_t                     iRect) noexcept;
  void spurfn(F_POINT const& innerPoint, F_POINT const& outerPoint, F_POINT& underlayInnerPoint, F_POINT& underlayOuterPoint) noexcept;
  auto spComp(SMAL_PNT_L const* arg1, SMAL_PNT_L const* arg2) noexcept -> bool;
  void srtf(std::vector<F_POINT_ATTR> const& tempStitchBuffer, uint32_t start, uint32_t finish);
  void stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept;
  void trfrm(F_POINT const& bottomLeftPoint,
             F_POINT const& topLeftPoint,
             F_POINT const& bottomRightPoint,
             F_POINT const& topRightPoint);
  void unbean(uint32_t start, uint32_t& finish);
  void uncon();
  auto leftsid(std::vector<F_POINT> const& currentFormVertices) noexcept -> uint32_t;
  auto unvis(boost::dynamic_bitset<> const& visitedRegions, int32_t& visitedIndex) -> bool;
  auto vscmp(uint32_t index1, uint32_t index2) noexcept -> bool;
} // namespace internal
} // namespace form
