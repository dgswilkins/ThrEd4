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
void angclpfn(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices);
void angsclp();
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
auto chkdel(const FRMHED& currentForm) noexcept -> bool;
auto chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
auto chkmax(uint32_t arg0, uint32_t arg1) noexcept -> bool;
void chkseq(bool border);
auto cisin(float xCoordinate, float yCoordinate) -> bool;
auto closflt(float xCoordinate, float yCoordinate) -> uint32_t;
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
void delmfil();
void drwcon();
void drwfrm();
void duangs();
void dubean();
void dubfil();
void dubig();
void dubold();
void dubsfil();
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
void dusqr();
void dustar(uint32_t starCount, float length);
void duzig(uint32_t vertices);
void fclp();
void fcntr();
void filangl();
void filclpx();
void filhor();
void filin(fPOINT currentPoint);
void filinsb(const fPOINT& point);
void filinu(const fPOINT& inPoint);
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
void frmlin(uint32_t vertices, uint32_t vertexCount);
void frmlin(const std::vector<fPOINT>& vertices);
void frmnumfn(uint32_t newFormIndex);
void frmon();
void frmout(uint32_t formIndex);
void frmovlin();
auto frmrng(uint32_t iForm, RANGE& range) -> bool;
void frmsadj();
void fselrct(uint32_t iForm);
void fsizpar(FRMHED& form) noexcept;
void fvars(uint32_t iForm) noexcept;
auto getblen() noexcept -> float;
auto getlast() -> uint32_t;
auto getplen() noexcept -> float;
void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
auto lastch() -> bool;
auto linx(const std::vector<fPOINT>& points, uint32_t start, uint32_t finish, fPOINT& intersection) noexcept -> bool;
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
auto nxt(uint32_t iVertex) noexcept -> uint32_t;
auto pdir(uint32_t vertex) -> uint32_t;
void picot();
void prpbrd(float borderStitchSpacing);
auto prv(uint32_t iVertex) noexcept -> uint32_t;
auto psg() noexcept -> uint32_t;
void pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect) noexcept;
void ratsr();
void rct2sel(const RECT& rectangle, std::vector<POINT>& line) noexcept;
void redup();
void refil();
void refilal();
void refilfn();
void rinfrm();
void ritfrct(uint32_t iForm, HDC dc);
void rotagain();
void rotcmd();
void rotdup();
void rotfrm(uint32_t newStartVertex);
auto rotpar() -> fPOINT;
void rstfrm();
void sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
void savblen(float fLength);
void savplen(float length);
void selal();
void selalfil();
void selfil(uint32_t type);
void selsqr(const POINT& controlPoint, HDC dc);
void setap();
void setexpand(float xyRatio);
void setfpnt();
void setfrm();
void sethup() noexcept;
void setins();
void setmfrm();
void setrang();
void setstrtch();
void sfCor2px(const fPOINT& stitchPoint, POINT& screen);
void shrnk();
void snap();
void spltfrm();
void srtbyfrm();
void srtfrm();
void stchadj();
void stchrct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
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
	void apbrd();
	void bakseq();
	void bdrlin(uint32_t start, uint32_t finish, float stitchSize);
	void bean(uint32_t start, uint32_t finish);
	void bhbrd(double spacing);
	void bhcrnr(uint32_t vertex);
	void bhfn(uint32_t start, uint32_t finish, double spacing);
	void bholbrd();
	void blbrd(double spacing);
	void bold(double size);
	void boldlin(uint32_t start, uint32_t finish, double size);
	void brdfil(float pd_Size);
	void brkdun(const std::vector<SMALPNTL*>& sortedLines, uint32_t start, uint32_t finish);
	void brkseq(const std::vector<SMALPNTL*>& sortedLines,
	            uint32_t                      start,
	            uint32_t                      finish,
	            boost::dynamic_bitset<>&      sequenceMap,
	            uint32_t&                     lastGroup,
	            SMALPNTL*                     sequenceLines);
	auto chk2of() -> bool;
	void chkbrd();
	void chksid(uint32_t vertexIndex, uint32_t clipIntersectSide, const std::vector<fPOINT>& currentFormVertices);
	auto closat(intersectionStyles& inOutFlag) -> bool;
	auto vclpComp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept -> bool;
	void clpcon(FRMHED& form,const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& currentFormVertices);
	void clpfm();
	auto clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
	             std::vector<CLPSEG>&       clipSegments,
	             const std::vector<double>& lengths,
	             uint32_t                   start,
	             uint32_t                   finish,
	             const std::vector<fPOINT>& currentFormVertices) -> uint32_t;

	auto clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, uint32_t sind) -> bool;
	auto fplComp(const fPOINTLINE& point1, const fPOINTLINE& point2) noexcept -> bool;
	void contf();
	auto contsf(uint32_t formIndex) -> bool;
	void cplayfn(uint32_t iForm, uint32_t play);

	constexpr auto duat(uint32_t attribute) -> uint32_t;

	void dubfn(const FRMHED& currentForm);
	void ducon() noexcept;
	void dudif(const fPOINT& start, const fPOINT& finish, fPOINT& delta) noexcept;
	void dufcntr(fPOINT& center);
	void dufdat(std::vector<fPOINT>& tempClipPoints,
	            std::vector<SATCON>& tempGuides,
	            std::vector<fPOINT>& destFormVertices,
	            std::vector<FRMHED>& destFormList,
	            uint32_t             formIndex,
	            uint32_t&            formRelocationIndex,
	            uint32_t&            formSourceIndex);
	void duflt(float& formOffset, std::vector<fPOINT>& currentFormVertices);
	void dunseq(const std::vector<SMALPNTL*>& sortedLines, uint32_t start, uint32_t finish, uint32_t& lastGroup);
	void dupfn(float rotationAngle);
	void duprotfs(float rotationAngle);
	void duprots(float rotationAngle, const fPOINT& rotationCenter);
	void durgn(const std::vector<FSEQ>&      sequencePath,
	           boost::dynamic_bitset<>&      visitedRegions,
	           const std::vector<SMALPNTL*>& sortedLines,
	           uint32_t                      pthi,
	           uint32_t                      lineCount,
	           std::vector<REGION>&          regionsList,
	           uint32_t&                     lastGroup,
	           uint32_t                      sequencePathIndex);
	void duromb(const fPOINT& start0, const fPOINT& finish0, const fPOINT& start1, const fPOINT& finish1);
	void duseq(const std::vector<SMALPNTL*>& sortedLines,
	           uint32_t                      start,
	           uint32_t                      finish,
	           boost::dynamic_bitset<>&      sequenceMap,
	           uint32_t&                     lastGroup,
	           SMALPNTL*                     sequenceLines);
	void duseq1(const SMALPNTL* sequenceLines);
	auto duseq2(SMALPNTL* sequenceLines) -> SMALPNTL*;
	void duspnd(const std::vector<VRCT2>& underlayVerticalRect,
	            const std::vector<VRCT2>& fillVerticalRect,
	            uint32_t                  start,
	            uint32_t                  finish,
	            float                     width);
	void filsclp();
	void filsfn();
	auto findDistanceToSide(const fPOINT& lineStart, const fPOINT& lineEnd, const fPOINT& point, double& distance) noexcept
	    -> float;
	void fmclp();
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
	void frmpoly(const POINT* line, uint32_t count) noexcept;
	void frmsqr(uint32_t iVertex);
	void frmsqr0(const POINT& controlPoint);
	void frmx(const POINT& controlPoint, HDC dc);
	void fsangl();
	void fsclp();
	void fsclpx();
	void fshor();
	void fspic();
	void fsvrt();
	void getbig() noexcept;
	auto getlen(std::vector<CLIPNT>&       clipStitchPoints,
	            const std::vector<double>& lengths,
	            uint32_t                   iPoint,
	            const std::vector<fPOINT>& currentFormVertices) -> float;
	void horclpfn(const std::vector<RNGCNT>& textureSegments, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
	auto insect(std::vector<CLIPSORT>&     clipIntersectData,
	            const std::vector<VCLPX>&  regionCrossingData,
	            std::vector<CLIPSORT*>&    arrayOfClipIntersectData,
	            uint32_t                   regionCrossingStart,
	            uint32_t                   regionCrossingEnd,
	            const fPOINT&              lineSegmentStart,
	            const fPOINT&              lineSegmentEnd,
	            const std::vector<fPOINT>& currentFormVertices) -> uint32_t;
	void inspnt(std::vector<CLIPNT>& clipStitchPoints);
	auto isclos(const SMALPNTL* lineEndPoint0, const SMALPNTL* lineEndPoint1, double gapToClosestRegion) noexcept -> bool;
	auto isect(uint32_t                   vertex0,
	           uint32_t                   vertex1,
	           fPOINT&                    intersection,
	           float&                     length,
	           const fPOINT&              lineSegmentStart,
	           const fPOINT&              lineSegmentEnd,
	           const std::vector<fPOINT>& currentFormVertices) -> bool;
	auto isin(std::vector<VCLPX>&        regionCrossingData,
	          float                      xCoordinate,
	          float                      yCoordinate,
	          uint32_t                   regionCrossingStart,
	          uint32_t                   regionCrossingEnd,
	          const fRECTANGLE&          boundingRect,
	          const std::vector<fPOINT>& currentFormVertices) -> bool;
	void lapbrd();
	void lcon(std::vector<uint32_t>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
	auto lenComp(const LENINFO& arg1, const LENINFO& arg2) noexcept -> bool;
	auto clipComp(const CLIPSORT* arg1, const CLIPSORT* arg2) noexcept -> bool;
	void linrutb(uint32_t start);
	auto lnclos(std::vector<uint32_t>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            uint32_t               group0,
	            uint32_t               line0,
	            uint32_t               group1,
	            uint32_t               line1,
	            double                 gapToClosestRegion) noexcept -> bool;
	void makpoli();
	void movseq(const std::vector<SMALPNTL*>& sortedLines, uint32_t ind);
	void mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, uint32_t destination, uint32_t source) noexcept;
	auto notdun(std::vector<RGSEQ>&            tempPath,
	            const std::vector<RCON>&       pathMap,
	            const std::vector<uint32_t>&   mapIndexSequence,
	            const boost::dynamic_bitset<>& visitedRegions,
	            uint32_t                       pathLength,
	            uint32_t                       doneRegion,
	            uint32_t                       sequencePathIndex) -> bool;
	auto notsel() noexcept -> bool;
	auto nucseg(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, uint32_t& currentSegmentIndex)
	    -> bool;
	void nufpnt(uint32_t vertex, FRMHED& formForInsert);
	void doTimeWindow(float rangeX, const std::vector<uint32_t>& xPoints, const std::vector<uint32_t>& xHistogram);
	void nxtrgn(std::vector<RGSEQ>&           tempPath,
	            const std::vector<RCON>&      pathMap,
	            const std::vector<uint32_t>&  mapIndexSequence,
	            boost::dynamic_bitset<>&      visitedRegions,
	            const std::vector<SMALPNTL*>& sortedLines,
	            const std::vector<REGION>&    regionsList,
	            uint32_t&                     doneRegion,
	            uint32_t                      pathMapIndex,
	            uint32_t&                     sequencePathIndex,
	            uint32_t                      visitedIndex);
	void nxtseq(std::vector<FSEQ>&           sequencePath,
	            const std::vector<RCON>&     pathMap,
	            const std::vector<uint32_t>& mapIndexSequence,
	            uint32_t                     pathIndex,
	            uint32_t&                    pathCount);
	void pbrd(float edgeSpacing);
	void pfn(const std::vector<VRCT2>& underlayVerticalRect,
	         const std::vector<VRCT2>& fillVerticalRect,
	         uint32_t                  startVertex,
	         const std::vector<VRCT2>& vrct,
	         float                     width);
	void plbak(uint32_t backPoint);
	void plbrd(float edgeSpacing, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
	void plfn(const std::vector<VRCT2>& underlayVerticalRect,
	          const std::vector<VRCT2>& fillVerticalRect,
	          const std::vector<VRCT2>& prct,
	          float                     width);
	void prebrd(FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
	auto proj(const fPOINT& point, float slope, const fPOINT& point0, const fPOINT& point1, fPOINT& intersectionPoint) noexcept
	    -> bool;
	auto projh(float yCoordinate, const fPOINT& point0, const fPOINT& point1, fPOINT& intersection) noexcept -> bool;
	auto projv(float xCoordinate, const fPOINT& lowerPoint, const fPOINT& upperPoint, fPOINT& intersection) noexcept -> bool;
	void prpsbrd(FRMHED& currentForm);
	void prsmal(float width);
	void px2stchf(const POINT& screen, fPOINT& stitchPoint) noexcept;
	void rats();
	auto regclos(std::vector<uint32_t>&        groupIndexSequence,
	             std::vector<SMALPNTL>&        lineEndpoints,
	             const std::vector<SMALPNTL*>& sortedLines,
	             uint32_t                      iRegion0,
	             uint32_t                      iRegion1,
	             const std::vector<REGION>&    regionsList,
	             double                        gapToClosestRegion,
	             uint32_t&                     nextGroup) noexcept -> bool;
	auto reglen(const std::vector<SMALPNTL*>& sortedLines,
	            uint32_t                      iRegion,
	            const std::vector<fPOINT>&    lastRegionCorners,
	            const std::vector<REGION>&    regionsList) -> double;
	void ritapbrd();
	void ritbrd();
	void ritfil();
	auto ritlin(const fPOINT& start, const fPOINT& finish, float userStitchLen) -> bool;
	void ritseg(const std::vector<CLIPNT>& clipStitchPoints,
	            std::vector<CLPSEG>&       clipSegments,
	            uint32_t                   currentSegmentIndex,
	            uint32_t&                  clipIntersectSide,
	            const std::vector<fPOINT>& currentFormVertices);
	void rotbak(float rotationAngle, const fPOINT& rotationCenter) noexcept;
	void rotentr(double rotationAngle);
	void rspnt(float xCoordinate, float yCoordinate);
	void sapliq();
	void sbold();
	void sbord();
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
	void snpfn(const std::vector<uint32_t>& xPoints, uint32_t start, uint32_t end, uint32_t finish) noexcept;
	void spend(const std::vector<VRCT2>& fillVerticalRect, uint32_t start, uint32_t finish);
	auto spltlin() -> bool;
	void sprct(std::vector<VRCT2>& fillVerticalRect, uint32_t start, uint32_t finish);
	void spurct(std::vector<VRCT2>& underlayVerticalRect, const std::vector<VRCT2>& fillVerticalRect, uint32_t iRect) noexcept;
	void
	     spurfn(const fPOINT& innerPoint, const fPOINT& outerPoint, fPOINT& underlayInnerPoint, fPOINT& underlayOuterPoint) noexcept;
	auto spComp(const SMALPNTL* arg1, const SMALPNTL* arg2) noexcept -> bool;
	void srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, uint32_t start, uint32_t finish);
	void stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept;
	void
	     trfrm(const fPOINT& bottomLeftPoint, const fPOINT& topLeftPoint, const fPOINT& bottomRightPoint, const fPOINT& topRightPoint);
	void unbean(uint32_t start, uint32_t& finish);
	void uncon();
	auto leftsid(const std::vector<fPOINT>& currentFormVertices) -> uint32_t;
	auto unvis(const boost::dynamic_bitset<>& visitedRegions, uint32_t& visitedIndex) -> bool;
	auto vscmp(uint32_t index1, uint32_t index2) noexcept -> bool;
} // namespace internal
} // namespace form
