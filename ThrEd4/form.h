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
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

namespace form {

void     angclp();
void     angclpfn(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices);
void     angsclp();
void     apliq();
void     bakagain();
void     bakdup();
void     bhol();
void     boxsel();
void     bord();
void     bsizpar() noexcept;
void     centir();
void     chain();
void     chan();
void     chkcont();
bool     chkdel() noexcept;
bool     chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio);
bool     chkmax(uint32_t arg0, uint32_t arg1) noexcept;
void     chkseq(bool border);
bool     cisin(float xCoordinate, float yCoordinate);
uint32_t closflt(float xCoordinate, float yCoordinate);
bool     closfrm();
void     clpfil();
void     clpspac(uint32_t insertPoint, uint32_t count);
void     clrfills() noexcept;
void     cntrx();
void     col2frm();
void     contfil();
void     cpylayr(uint32_t codedLayer);
void     crop();
void     debean();
void     delflt(uint32_t formIndex);
void     delfrms();
void     delmfil();
void     drwcon();
void     drwfrm();
void     duangs();
void     dubean();
void     dubfil();
void     dubig();
void     dubold();
void     dubsfil();
void     dueg(uint32_t sides);
void     duform(uint32_t formType);
void     dufrm() noexcept;
void     duhart(uint32_t sideCount);
void     dulens(uint32_t sides);
void     duinsf() noexcept;
void     duprot(float rotationAngle);
void     dupsel(HDC dc);
void     durpoli(uint32_t vertexCount);
void     duspir(uint32_t stepCount);
void     dusqr();
void     dustar(uint32_t starCount, float length);
void     duzig(uint32_t vertices);
void     fclp();
void     fcntr();
void     filangl();
void     filclpx();
void     filhor();
void     filin(fPOINT currentPoint);
void     filinsb(const fPOINT& point);
void     filinu(const fPOINT& inPoint);
void     filsat();
void     filvrt();
uint32_t find1st();
void     fliph();
void     flipv();
void     flpord();
void     fltspac(uint32_t vertexOffset, uint32_t count);
void     form();
void     frm0();
void     frmadj(uint32_t formIndex);
void     frmclr(FRMHED& destination) noexcept;
void     frmlin(uint32_t vertices, uint32_t vertexCount);
void     frmlin(const std::vector<fPOINT>& vertices);
void     frmnumfn(uint32_t newFormIndex);
void     frmon();
void     frmout(uint32_t formIndex);
void     frmovlin();
bool     frmrng(uint32_t iForm, RANGE& range);
void     frmsadj();
void     fselrct(uint32_t iForm);
void     fsizpar() noexcept;
void     fvars(uint32_t iForm);

float getblen();

uint32_t getlast();

float getplen() noexcept;

void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
bool lastch();
bool linx(const std::vector<fPOINT>& points, uint32_t start, uint32_t finish, fPOINT& intersection);
void mdufrm();

// pragma required until MSVC /analyze recognizes noexcept(false)
#pragma warning(push)
#pragma warning(disable : 26440)
template <class inType> float midl(inType high, inType low) noexcept {
	return (gsl::narrow_cast<float>(high) - gsl::narrow_cast<float>(low)) / 2.0f + gsl::narrow_cast<float>(low);
}
#pragma warning(pop)

void movlayr(uint32_t codedLayer);
void munfrm();
bool notfstch(uint32_t attribute) noexcept;
void nubrdcol(uint32_t color);
void nufilcol(uint32_t color);
void nufsel();
void nufthcol(uint32_t color);
void nulapcol(uint32_t color);

uint32_t nxt(uint32_t iVertex) noexcept;

uint32_t pdir(uint32_t vertex);

void picot();
void prpbrd(float borderStitchSpacing);

constexpr uint32_t prv(uint32_t iVertex);

uint32_t psg() noexcept;
void     pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect) noexcept;
void     ratsr();
void     rct2sel(const RECT& rectangle, std::vector<POINT>& line);
void     redup();
void     refil();
void     refilal();
void     refilfn();
void     rinfrm();
void     ritfrct(uint32_t iForm, HDC dc);
void     rotagain();
void     rotcmd();
void     rotdup();
void     rotfrm(uint32_t newStartVertex);
fPOINT   rotpar();
void     rstfrm();
void     sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
void     savblen(float fLength);
void     savplen(float length);
void     selal();
void     selalfil();
void     selfil(uint32_t type);
void     selsqr(const POINT& controlPoint, HDC dc);
void     setap();
void     setexpand(float xyRatio);
void     setfpnt();
void     setfrm();
void     sethup() noexcept;
void     setins();
void     setmfrm();
void     setrang();
void     setstrtch();
void     sfCor2px(const fPOINT& stitchPoint, POINT& screen);
void     shrnk();
void     snap();
void     spltfrm();
void     srtbyfrm();
void     srtfrm();
void     stchadj();
void     stchrct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
void     stchs2frm();
void     tglfrm();
void     unfil();
void     unfrm();
void     uninsf();
void     unpsel();
void     vrtclp();
void     vrtsclp();

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
	bool chk2of();
	void chkbrd();
	void chksid(uint32_t vertexIndex, uint32_t clipIntersectSide, const std::vector<fPOINT>& currentFormVertices);
	bool closat(intersectionStyles& inOutFlag);
	bool vclpComp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept;
	void clpcon(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& currentFormVertices);
	void clpfm();

	uint32_t clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
	                 std::vector<CLPSEG>&       clipSegments,
	                 const std::vector<double>& lengths,
	                 uint32_t                   start,
	                 uint32_t                   finish,
	                 const std::vector<fPOINT>& currentFormVertices);

	bool clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, uint32_t sind);
	bool fplComp(const fPOINTLINE& point1, const fPOINTLINE& point2) noexcept;
	void contf();
	bool contsf(uint32_t formIndex);
	void cplayfn(uint32_t iForm, uint32_t play);

	constexpr uint32_t duat(uint32_t attribute);

	void dubfn();
	void ducon() noexcept;
	void dudif(const fPOINT& start, const fPOINT& finish, fPOINT& delta) noexcept;
	void dufcntr(fPOINT& center);
	void dufdat(std::vector<fPOINT>& tempClipPoints,
	            std::vector<SATCON>& tempGuides,
	            std::vector<fPOINT>& destinationFormVertices,
	            std::vector<FRMHED>& destinationFormList,
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

	SMALPNTL* duseq2(SMALPNTL* sequenceLines);

	void  duspnd(const std::vector<VRCT2>& underlayVerticalRect,
	             const std::vector<VRCT2>& fillVerticalRect,
	             uint32_t                  start,
	             uint32_t                  finish);
	void  filsclp();
	void  filsfn();
	float findDistanceToSide(const fPOINT& lineStart, const fPOINT& lineEnd, const fPOINT& point, double& distance) noexcept;
	void  fmclp();
	void  fnagain(float rotationAngle);
	void  fnang(std::vector<uint32_t>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            float                  rotationAngle,
	            fPOINT&                rotationCenter,
	            FRMHED&                angledForm,
	            std::vector<fPOINT>&   angledFormVertices);
	void  fnhor(std::vector<uint32_t>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            float                  rotationAngle,
	            fPOINT&                rotationCenter,
	            FRMHED&                angledForm,
	            std::vector<fPOINT>&   angledFormVertices);
	void  fnord();
	void  fnvrt(std::vector<fPOINT>&   currentFillVertices,
	            std::vector<uint32_t>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints);
	void  frmpnts(uint32_t type);
	void  frmpoly(const POINT* line, uint32_t count) noexcept;
	void  frmsqr(uint32_t iVertex);
	void  frmsqr0(const POINT& controlPoint);
	void  frmx(const POINT& controlPoint, HDC dc);
	void  fsangl();
	void  fsclp();
	void  fsclpx();
	void  fshor();
	void  fspic();
	void  fsvrt();
	void  getbig() noexcept;
	float getlen(std::vector<CLIPNT>&       clipStitchPoints,
	             const std::vector<double>& lengths,
	             uint32_t                   iPoint,
	             const std::vector<fPOINT>& currentFormVertices);
	void  horclpfn(const std::vector<RNGCNT>& textureSegments, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);

	uint32_t insect(std::vector<CLIPSORT>&     clipIntersectData,
	                const std::vector<VCLPX>&  regionCrossingData,
	                std::vector<CLIPSORT*>&    arrayOfClipIntersectData,
	                uint32_t                   regionCrossingStart,
	                uint32_t                   regionCrossingEnd,
	                const fPOINT&              lineSegmentStart,
	                const fPOINT&              lineSegmentEnd,
	                const std::vector<fPOINT>& currentFormVertices);

	void inspnt(std::vector<CLIPNT>& clipStitchPoints);
	bool isclos(const SMALPNTL* lineEndPoint0, const SMALPNTL* lineEndPoint1, double gapToClosestRegion) noexcept;

	bool isect(uint32_t                   vertex0,
	           uint32_t                   vertex1,
	           fPOINT&                    intersection,
	           float&                     length,
	           const fPOINT&              lineSegmentStart,
	           const fPOINT&              lineSegmentEnd,
	           const std::vector<fPOINT>& currentFormVertices);
	bool isin(std::vector<VCLPX>&        regionCrossingData,
	          float                      xCoordinate,
	          float                      yCoordinate,
	          uint32_t                   regionCrossingStart,
	          uint32_t                   regionCrossingEnd,
	          const fRECTANGLE&          boundingRect,
	          const std::vector<fPOINT>& currentFormVertices);
	void lapbrd();
	void lcon(std::vector<uint32_t>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
	bool lenComp(const LENINFO& arg1, const LENINFO& arg2) noexcept;
	bool clipComp(const CLIPSORT* const arg1, const CLIPSORT* const arg2) noexcept;
	void linrutb(uint32_t start);
	bool lnclos(std::vector<uint32_t>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            uint32_t               group0,
	            uint32_t               line0,
	            uint32_t               group1,
	            uint32_t               line1,
	            double                 gapToClosestRegion);
	void makpoli();
	void movseq(const std::vector<SMALPNTL*>& sortedLines, uint32_t ind);
	void mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, uint32_t destination, uint32_t source);
	bool notdun(std::vector<RGSEQ>&            tempPath,
	            const std::vector<RCON>&       pathMap,
	            const std::vector<uint32_t>&   mapIndexSequence,
	            const boost::dynamic_bitset<>& visitedRegions,
	            uint32_t                       pathLength,
	            uint32_t                       doneRegion,
	            uint32_t                       sequencePathIndex);
	bool notsel() noexcept;
	bool
	     nucseg(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, uint32_t& currentSegmentIndex);
	void nufpnt(uint32_t vertex, FRMHED* formForInsert);
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

	void   pbrd(float edgeSpacing);
	void   pfn(const std::vector<VRCT2>& underlayVerticalRect,
	           const std::vector<VRCT2>& fillVerticalRect,
	           uint32_t                  startVertex,
	           const std::vector<VRCT2>& vrct);
	void   plbak(uint32_t backPoint);
	void   plbrd(float edgeSpacing, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
	void   plfn(const std::vector<VRCT2>& underlayVerticalRect,
	            const std::vector<VRCT2>& fillVerticalRect,
	            const std::vector<VRCT2>& prct);
	void   prebrd(FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);
	bool   proj(const fPOINT& point, float slope, const fPOINT& point0, const fPOINT& point1, fPOINT& intersectionPoint) noexcept;
	bool   projh(float yCoordinate, const fPOINT& point0, const fPOINT& point1, fPOINT& intersection) noexcept;
	bool   projv(float xCoordinate, const fPOINT& lowerPoint, const fPOINT& upperPoint, fPOINT& intersection) noexcept;
	void   prpsbrd();
	void   prsmal();
	void   px2stchf(const POINT& screen, fPOINT& stitchPoint) noexcept;
	void   rats();
	bool   regclos(std::vector<uint32_t>&        groupIndexSequence,
	               std::vector<SMALPNTL>&        lineEndpoints,
	               const std::vector<SMALPNTL*>& sortedLines,
	               uint32_t                      iRegion0,
	               uint32_t                      iRegion1,
	               const std::vector<REGION>&    regionsList,
	               double                        gapToClosestRegion,
	               uint32_t&                     nextGroup);
	double reglen(const std::vector<SMALPNTL*>& sortedLines,
	              uint32_t                      iRegion,
	              const std::vector<fPOINT>&    lastRegionCorners,
	              const std::vector<REGION>&    regionsList);
	void   ritapbrd();
	void   ritbrd();
	void   ritfil();
	bool   ritlin(const fPOINT& start, const fPOINT& finish, float userStitchLen);
	void   ritseg(const std::vector<CLIPNT>& clipStitchPoints,
	              std::vector<CLPSEG>&       clipSegments,
	              uint32_t                   currentSegmentIndex,
	              uint32_t&                  clipIntersectSide,
	              const std::vector<fPOINT>& currentFormVertices);
	void   rotbak(float rotationAngle, const fPOINT& rotationCenter) noexcept;
	void   rotentr(double rotationAngle);
	void   rspnt(float xCoordinate, float yCoordinate);
	void   sapliq();

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

	constexpr float shreg(float highValue, float reference, float eggRatio) noexcept;

	void shrnks();
	void snp(uint32_t start, uint32_t finish);
	void snpfn(const std::vector<uint32_t>& xPoints, uint32_t start, uint32_t end, uint32_t finish);
	void spend(const std::vector<VRCT2>& fillVerticalRect, uint32_t start, uint32_t finish);
	bool spltlin();
	void sprct(std::vector<VRCT2>& fillVerticalRect, uint32_t start, uint32_t finish);
	void spurct(std::vector<VRCT2>& underlayVerticalRect, const std::vector<VRCT2>& fillVerticalRect, uint32_t iRect);
	void
	     spurfn(const fPOINT& innerPoint, const fPOINT& outerPoint, fPOINT& underlayInnerPoint, fPOINT& underlayOuterPoint) noexcept;
	bool spComp(const SMALPNTL* const arg1, const SMALPNTL* const arg2) noexcept;
	void srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, uint32_t start, uint32_t finish);
	void stchfrm(uint32_t formIndex, uint32_t& attribute) noexcept;
	void
	     trfrm(const fPOINT& bottomLeftPoint, const fPOINT& topLeftPoint, const fPOINT& bottomRightPoint, const fPOINT& topRightPoint);
	void unbean(uint32_t start, uint32_t& finish);
	void uncon();

	uint32_t leftsid(const std::vector<fPOINT>& currentFormVertices);

	bool unvis(const boost::dynamic_bitset<>& visitedRegions, uint32_t& visitedIndex);
	bool vscmp(uint32_t index1, uint32_t index2);
} // namespace internal
} // namespace form