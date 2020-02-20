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
void angclpfn(FRMHED const& form, std::vector<RNGCNT> const& textureSegments, std::vector<fPOINT>& angledFormVertices);
void angsclp(FRMHED& form);
void apliq();
void bakagain();
void bakdup();
void bhol();
void boxsel();
void bord();
void bsizpar(FRMHED& currentForm) noexcept;
void centir();
void chain();
void chan();
void chkcont();
auto chkdel(FRMHED const& currentForm) noexcept -> bool;
auto chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
auto chkmax(uint32_t arg0, uint32_t arg1) noexcept -> bool;
void chkseq(bool border);
auto cisin(FRMHED const& form, float xCoordinate, float yCoordinate) -> bool;
auto closflt(FRMHED const& form, float xCoordinate, float yCoordinate) -> uint32_t;
auto closfrm() -> bool;
void clpfil();
void clpspac(uint32_t insertPoint, uint32_t count);
void clrfills() noexcept;
void cntrx();
void col2frm();
void contfil();
void cpylayr(uint32_t codedLayer);
void crop();
void debean();
void delflt(uint32_t formIndex);
void delfrms();
void delmfil(uint32_t formIndex);
void drwcon();
void drwfrm();
void duangs(FRMHED const& form);
void dubean();
void dubfil();
void dubig();
void dubold();
void dubsfil(FRMHED& form);
void dueg(uint32_t sides);
void duform(uint32_t formType);
void dufrm() noexcept;
void duhart(uint32_t sideCount);
void dulens(uint32_t sides);
void duinsf() noexcept;
void duprot(float rotationAngle);
void dupsel(HDC dc);
void durpoli(uint32_t vertexCount);
void duspir(uint32_t stepCount);
void dusqr(FRMHED& form);
void dustar(uint32_t starCount, float length);
void duzig(uint32_t vertices);
void fclp();
void fcntr();
void filangl();
void filclpx();
void filhor();
void filin(fPOINT currentPoint);
void filinsb(fPOINT const& point);
void filinu(fPOINT const& inPoint);
void filsat();
void filvrt();
auto find1st() -> uint32_t;
void fliph();
void flipv();
void flpord();
void fltspac(uint32_t vertexOffset, uint32_t count);
void form();
void frm0();
void frmadj(uint32_t formIndex);
void frmclr(FRMHED& destination) noexcept;
void frmlin(FRMHED const& form);
void frmlin(std::vector<fPOINT> const& vertices);
void frmnumfn(uint32_t newFormIndex);
void frmon();
void frmout(uint32_t formIndex);
void frmovlin();
auto frmrng(uint32_t iForm, RANGE& range) -> bool;
void frmsadj();
void fselrct(uint32_t iForm);
void fsizpar(FRMHED& form) noexcept;
auto getblen() noexcept -> float;
auto getlast(FRMHED const& form) -> uint32_t;
auto getplen() noexcept -> float;
void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
auto lastch() noexcept -> bool;
auto linx(std::vector<fPOINT> const& points, uint32_t start, uint32_t finish, fPOINT& intersection) noexcept
    -> bool;
void mdufrm() noexcept;

// pragma required until MSVC /analyze recognizes noexcept(false)
#pragma warning(push)
#pragma warning(disable : 26440)
template <class inType> float midl(inType high, inType low) noexcept {
  return (gsl::narrow_cast<float>(high) - gsl::narrow_cast<float>(low)) / 2.0F + gsl::narrow_cast<float>(low);
}
#pragma warning(pop)

void movlayr(uint32_t codedLayer);
void munfrm();
auto notfstch(uint32_t attribute) noexcept -> bool;
void nubrdcol(uint32_t color);
void nufilcol(uint32_t color);
void nufsel();
void nufthcol(uint32_t color);
void nulapcol(uint32_t color);
auto nxt(FRMHED const& form, uint32_t iVertex) noexcept -> uint32_t;
auto pdir(FRMHED const& form, uint32_t vertex) -> uint32_t;
void picot();
void prpbrd(float borderStitchSpacing);
auto prv(FRMHED const& form, uint32_t iVertex) noexcept -> uint32_t;
auto psg() noexcept -> uint32_t;
void pxrct2stch(RECT const& screenRect, fRECTANGLE& stitchRect) noexcept;
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
void rotfrm(FRMHED& form, uint32_t newStartVertex);
auto rotpar() -> fPOINT;
void rstfrm();
void sRct2px(fRECTANGLE const& stitchRect, RECT& screenRect);
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
auto sfCor2px(fPOINT const& stitchPoint) -> POINT;
void shrnk();
void snap();
void spltfrm();
void srtbyfrm();
void srtfrm();
void stchadj();
void stchrct2px(fRECTANGLE const& stitchRect, RECT& screenRect);
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
  void angout(FRMHED& angledForm);
  void apbrd(FRMHED const& form);
  void bakseq();
  void bdrlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, float stitchSize);
  void bean(uint32_t start, uint32_t finish);
  void bhbrd(FRMHED const& form);
  void bhcrnr(FRMHED const& form, uint32_t vertex);
  void bhfn(FRMHED const& form, uint32_t start, uint32_t finish);
  void bholbrd(uint32_t formIndex);
  void blbrd(FRMHED const& form);
  void bold(FRMHED const& form);
  void boldlin(uint32_t vertexIndex, uint32_t start, uint32_t finish, double size);
  void brdfil(FRMHED const& form);
  void brkdun(std::vector<SMALPNTL*> const& sortedLines, uint32_t start, uint32_t finish);
  void brkseq(std::vector<SMALPNTL*> const& sortedLines,
              uint32_t                      start,
              uint32_t                      finish,
              boost::dynamic_bitset<>&      sequenceMap,
              uint32_t&                     lastGroup,
              SMALPNTL*                     sequenceLines);
  auto chk2of() -> bool;
  void chkbrd(FRMHED const& form);
  void chksid(FRMHED const& form, uint32_t vertexIndex, uint32_t clipIntersectSide, std::vector<fPOINT> const& currentFormVertices);
  auto closat(intersectionStyles& inOutFlag) -> bool;
  auto vclpComp(VCLPX const& vclpx1, VCLPX const& vclpx2) noexcept -> bool;
  void clpcon(FRMHED& form, std::vector<RNGCNT> const& textureSegments, std::vector<fPOINT>& currentFormVertices);
  void clpfm();
  auto clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
               std::vector<CLPSEG>&       clipSegments,
               std::vector<double> const& lengths,
               uint32_t                   start,
               uint32_t                   finish,
               std::vector<fPOINT> const& currentFormVertices) -> uint32_t;

  auto clpnxt(std::vector<CLPSEG> const& clipSegments, std::vector<LENINFO> const& sortedLengths, uint32_t sind)
      -> bool;
  auto fplComp(fPOINTLINE const& point1, fPOINTLINE const& point2) noexcept -> bool;
  void contf(FRMHED& form);
  auto contsf(uint32_t formIndex) -> bool;
  void cplayfn(uint32_t iForm, uint32_t play);

  constexpr auto duat(uint32_t attribute) -> uint32_t;

  void dubfn(FRMHED const& currentForm);
  void ducon() noexcept;
  void dudif(fPOINT const& start, fPOINT const& finish, fPOINT& delta) noexcept;
  void dufcntr(fPOINT& center) noexcept;
  void dufdat(std::vector<fPOINT>& tempClipPoints,
              std::vector<SATCON>& tempGuides,
              std::vector<fPOINT>& destFormVertices,
              std::vector<FRMHED>& destFormList,
              uint32_t             formIndex,
              uint32_t&            formRelocationIndex,
              uint32_t&            formSourceIndex);
  void duflt(float& formOffset, std::vector<fPOINT>& currentFormVertices);
  void dunseq(std::vector<SMALPNTL*> const& sortedLines, uint32_t start, uint32_t finish, uint32_t& lastGroup);
  void dupfn(float rotationAngle);
  void duprotfs(float rotationAngle);
  void duprots(float rotationAngle, fPOINT const& rotationCenter);
  void durgn(FRMHED const&                 form,
             std::vector<FSEQ> const&      sequencePath,
             boost::dynamic_bitset<>&      visitedRegions,
             std::vector<SMALPNTL*> const& sortedLines,
             uint32_t                      pthi,
             uint32_t                      lineCount,
             std::vector<REGION> const&    regionsList,
             uint32_t&                     lastGroup,
             uint32_t                      sequencePathIndex);
  void duromb(fPOINT const& start0, fPOINT const& finish0, fPOINT const& start1, fPOINT const& finish1);
  void duseq(std::vector<SMALPNTL*> const& sortedLines,
             uint32_t                      start,
             uint32_t                      finish,
             boost::dynamic_bitset<>&      sequenceMap,
             uint32_t&                     lastGroup,
             SMALPNTL*                     sequenceLines);
  void duseq1(SMALPNTL const* sequenceLines);
  void duseq2(SMALPNTL const* sequenceLines);
  void duspnd(uint32_t                  stitchLen,
              std::vector<VRCT2> const& underlayVerticalRect,
              std::vector<VRCT2> const& fillVerticalRect,
              uint32_t                  start,
              uint32_t                  finish,
              float                     width);
  void filsclp();
  void filsfn();
  auto findDistanceToSide(fPOINT const& lineStart, fPOINT const& lineEnd, fPOINT const& point, double& distance) noexcept
      -> float;
  void fmclp(FRMHED& form);
  void fnagain(float rotationAngle);
  void fnang(std::vector<uint32_t>& groupIndexSequence,
             std::vector<SMALPNTL>& lineEndpoints,
             float                  rotationAngle,
             fPOINT&                rotationCenter,
             FRMHED&                angledForm,
             std::vector<fPOINT>&   angledFormVertices);
  void fnhor(std::vector<uint32_t>& groupIndexSequence,
             std::vector<SMALPNTL>& lineEndpoints,
             float                  rotationAngle,
             fPOINT&                rotationCenter,
             FRMHED&                angledForm,
             std::vector<fPOINT>&   angledFormVertices);
  void fnord();
  void fnvrt(std::vector<fPOINT>&   currentFillVertices,
             std::vector<uint32_t>& groupIndexSequence,
             std::vector<SMALPNTL>& lineEndpoints);
  void frmpnts(uint32_t type);
  void frmpoly(POINT const* line, uint32_t count) noexcept;
  void frmsqr(uint32_t vertexIndex, uint32_t iVertex);
  void frmsqr0(POINT const& controlPoint);
  void frmx(POINT const& controlPoint, HDC dc);
  void fsangl(FRMHED& form);
  void fsclp(uint32_t formIndex);
  void fsclpx(uint32_t formIndex);
  void fshor(FRMHED& form);
  void fspic(uint32_t formIndex);
  void fsvrt();
  void getbig() noexcept;
  auto getlen(std::vector<CLIPNT>&       clipStitchPoints,
              std::vector<double> const& lengths,
              uint32_t                   iPoint,
              std::vector<fPOINT> const& currentFormVertices) -> float;
  void horclpfn(std::vector<RNGCNT> const& textureSegments, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
  auto insect(FRMHED const&              form,
              std::vector<CLIPSORT>&     clipIntersectData,
              std::vector<VCLPX> const&  regionCrossingData,
              std::vector<CLIPSORT*>&    arrayOfClipIntersectData,
              uint32_t                   regionCrossingStart,
              uint32_t                   regionCrossingEnd,
              fPOINT const&              lineSegmentStart,
              fPOINT const&              lineSegmentEnd,
              std::vector<fPOINT> const& currentFormVertices) -> uint32_t;
  void inspnt(std::vector<CLIPNT>& clipStitchPoints);
  auto isclos(SMALPNTL const* lineEndPoint0, SMALPNTL const* lineEndPoint1, double gapToClosestRegion) noexcept
      -> bool;
  auto isect(uint32_t                   vertex0,
             uint32_t                   vertex1,
             fPOINT&                    intersection,
             float&                     length,
             fPOINT const&              lineSegmentStart,
             fPOINT const&              lineSegmentEnd,
             std::vector<fPOINT> const& currentFormVertices) -> bool;
  auto isin(FRMHED const&              form,
            std::vector<VCLPX> const&  regionCrossingData,
            float                      xCoordinate,
            float                      yCoordinate,
            uint32_t                   regionCrossingStart,
            uint32_t                   regionCrossingEnd,
            fRECTANGLE const&          boundingRect,
            std::vector<fPOINT> const& currentFormVertices) -> bool;
  void lapbrd(FRMHED const& form);
  void lcon(FRMHED const& form, std::vector<uint32_t>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
  auto lenComp(LENINFO const& arg1, LENINFO const& arg2) noexcept -> bool;
  auto clipComp(CLIPSORT const* arg1, CLIPSORT const* arg2) noexcept -> bool;
  auto lnclos(std::vector<uint32_t>& groupIndexSequence,
              std::vector<SMALPNTL>& lineEndpoints,
              uint32_t               group0,
              uint32_t               line0,
              uint32_t               group1,
              uint32_t               line1,
              double                 gapToClosestRegion) noexcept -> bool;
  void makpoli();
  void movseq(std::vector<SMALPNTL*> const& sortedLines, uint32_t ind);
  void mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, uint32_t destination, uint32_t source) noexcept;
  auto notdun(std::vector<RGSEQ>&            tempPath,
              std::vector<RCON> const&       pathMap,
              std::vector<uint32_t> const&   mapIndexSequence,
              boost::dynamic_bitset<> const& visitedRegions,
              uint32_t                       pathLength,
              uint32_t                       doneRegion,
              uint32_t                       sequencePathIndex) -> bool;
  auto nucseg(std::vector<CLPSEG> const&  clipSegments,
              std::vector<LENINFO> const& sortedLengths,
              uint32_t&                   currentSegmentIndex) -> bool;
  void nufpnt(uint32_t vertex, FRMHED& formForInsert);
  void doTimeWindow(float rangeX, std::vector<uint32_t> const& xPoints, std::vector<uint32_t> const& xHistogram);
  void nxtrgn(std::vector<RGSEQ>&           tempPath,
              std::vector<RCON> const&      pathMap,
              std::vector<uint32_t> const&  mapIndexSequence,
              boost::dynamic_bitset<>&      visitedRegions,
              std::vector<SMALPNTL*> const& sortedLines,
              std::vector<REGION> const&    regionsList,
              uint32_t&                     doneRegion,
              uint32_t                      pathMapIndex,
              uint32_t&                     sequencePathIndex,
              uint32_t                      visitedIndex);
  void nxtseq(std::vector<FSEQ>&           sequencePath,
              std::vector<RCON> const&     pathMap,
              std::vector<uint32_t> const& mapIndexSequence,
              uint32_t                     pathIndex,
              uint32_t&                    pathCount);
  void pbrd(FRMHED const& form);
  void pfn(std::vector<VRCT2> const& underlayVerticalRect,
           std::vector<VRCT2> const& fillVerticalRect,
           uint32_t                  startVertex,
           std::vector<VRCT2> const& vrct,
           float                     width);
  void plbak(uint32_t backPoint);
  void plbrd(FRMHED const& form, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
  void plfn(FRMHED const&             form,
            std::vector<VRCT2> const& underlayVerticalRect,
            std::vector<VRCT2> const& fillVerticalRect,
            std::vector<VRCT2> const& prct,
            float                     width);
  void prebrd(FRMHED const& form, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
  auto proj(fPOINT const& point, float slope, fPOINT const& point0, fPOINT const& point1, fPOINT& intersectionPoint) noexcept
      -> bool;
  auto projh(float yCoordinate, fPOINT const& point0, fPOINT const& point1, fPOINT& intersection) noexcept -> bool;
  auto projv(float xCoordinate, fPOINT const& lowerPoint, fPOINT const& upperPoint, fPOINT& intersection) noexcept
      -> bool;
  void prpsbrd(uint32_t formIndex);
  void prsmal(float width);
  void px2stchf(POINT const& screen, fPOINT& stitchPoint) noexcept;
  void rats();
  auto regclos(std::vector<uint32_t>&        groupIndexSequence,
               std::vector<SMALPNTL>&        lineEndpoints,
               std::vector<SMALPNTL*> const& sortedLines,
               uint32_t                      iRegion0,
               uint32_t                      iRegion1,
               std::vector<REGION> const&    regionsList,
               double                        gapToClosestRegion,
               uint32_t&                     nextGroup) noexcept -> bool;
  auto reglen(std::vector<SMALPNTL*> const& sortedLines,
              uint32_t                      iRegion,
              std::vector<fPOINT> const&    lastRegionCorners,
              std::vector<REGION> const&    regionsList) -> double;
  void ritapbrd();
  void ritbrd(FRMHED const& form);
  void ritfil();
  auto ritlin(fPOINT const& start, fPOINT const& finish, float userStitchLen) -> bool;
  void ritseg(FRMHED const&              form,
              std::vector<CLIPNT> const& clipStitchPoints,
              std::vector<CLPSEG>&       clipSegments,
              uint32_t                   currentSegmentIndex,
              uint32_t&                  clipIntersectSide,
              std::vector<fPOINT> const& currentFormVertices);
  void rotbak(float rotationAngle, fPOINT const& rotationCenter) noexcept;
  void rotentr(double rotationAngle);
  void rspnt(float xCoordinate, float yCoordinate);
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
  void spend(std::vector<VRCT2> const& fillVerticalRect, uint32_t start, uint32_t finish);
  auto spltlin() -> bool;
  void sprct(std::vector<fPOINT> const* vertices,
             uint32_t                   vertexIndex,
             std::vector<VRCT2>&        fillVerticalRect,
             uint32_t                   start,
             uint32_t                   finish);
  void spurct(std::vector<VRCT2>& underlayVerticalRect, std::vector<VRCT2> const& fillVerticalRect, uint32_t iRect) noexcept;
  void spurfn(fPOINT const& innerPoint, fPOINT const& outerPoint, fPOINT& underlayInnerPoint, fPOINT& underlayOuterPoint) noexcept;
  auto spComp(SMALPNTL const* arg1, SMALPNTL const* arg2) noexcept -> bool;
  void srtf(std::vector<fPOINTATTR> const& tempStitchBuffer, uint32_t start, uint32_t finish);
  void stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept;
  void trfrm(fPOINT const& bottomLeftPoint,
             fPOINT const& topLeftPoint,
             fPOINT const& bottomRightPoint,
             fPOINT const& topRightPoint);
  void unbean(uint32_t start, uint32_t& finish);
  void uncon();
  auto leftsid(std::vector<fPOINT> const& currentFormVertices) -> uint32_t;
  auto unvis(boost::dynamic_bitset<> const& visitedRegions, uint32_t& visitedIndex) -> bool;
  auto vscmp(uint32_t index1, uint32_t index2) noexcept -> bool;
} // namespace internal
} // namespace form
