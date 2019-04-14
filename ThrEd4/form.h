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

void         angclp();
void         angclpfn(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& angledFormVertices);
void         angsclp();
void         apliq();
void         bakagain();
void         bakdup();
void         bhol();
void         boxsel();
void         bord();
void         bsizpar() noexcept;
void         centir();
void         chain();
void         chan();
void         chkcont();
bool         chkdel() noexcept;
bool         chkfrm(std::vector<POINT>& stretchBoxLine, float& xyRatio);
bool         chkmax(unsigned int arg0, unsigned int arg1) noexcept;
void         chkseq(bool border);
bool         cisin(float xCoordinate, float yCoordinate);
unsigned int closflt(float xCoordinate, float yCoordinate);
bool         closfrm();
void         clpfil();
void         clpspac(unsigned insertPoint, unsigned int count);
void         clrfills() noexcept;
void         cntrx();
void         col2frm();
void         contfil();
void         cpylayr(unsigned codedLayer);
void         crop();
void         debean();
void         delflt(unsigned int formIndex);
void         delfrms();
void         delmfil();
void         drwcon();
void         drwfrm();
void         duangs();
void         dubean();
void         dubfil();
void         dubig();
void         dubold();
void         dubsfil();
void         dueg(unsigned sides);
void         duform(unsigned formType);
void         dufrm() noexcept;
void         duhart(unsigned sideCount);
void         dulens(unsigned sides);
void         duinsf() noexcept;
void         duprot(float rotationAngle);
void         dupsel(HDC dc);
void         durpoli(unsigned vertexCount);
void         duspir(unsigned stepCount);
void         dusqr();
void         dustar(unsigned starCount, float length);
void         duzig(unsigned vertices);
void         fclp();
void         fcntr();
void         filangl();
void         filclpx();
void         filhor();
void         filin(fPOINT currentPoint);
void         filinsb(const fPOINT& point);
void         filinu(const fPOINT& inPoint);
void         filsat();
void         filvrt();
unsigned     find1st();
void         fliph();
void         flipv();
void         flpord();
void         fltspac(unsigned int vertexOffset, unsigned int count);
void         form();
void         frm0();
void         frmadj(unsigned int formIndex);
void         frmclr(FRMHED& destination) noexcept;
void         frmlin(unsigned int vertices, unsigned int vertexCount);
void         frmlin(const std::vector<fPOINT>& vertices);
void         frmnumfn(unsigned newFormIndex);
void         frmon();
void         frmout(unsigned int formIndex);
void         frmovlin();
bool         frmrng(unsigned int iForm, RANGE& range);
void         frmsadj();
void         fselrct(unsigned int iForm);
void         fsizpar() noexcept;
void         fvars(unsigned int iForm);

float getblen();

unsigned int getlast();

float getplen() noexcept;

void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
bool lastch();
bool linx(const std::vector<fPOINT>& points, unsigned int start, unsigned int finish, fPOINT& intersection);
void makspac(unsigned start, unsigned count);
void mdufrm();

// pragma required until MSVC /analyze recognizes noexcept(false)
#pragma warning(push)
#pragma warning(disable : 26440)
template <class inType> float midl(inType high, inType low) noexcept {
	return (gsl::narrow_cast<float>(high) - gsl::narrow_cast<float>(low)) / 2.0f + gsl::narrow_cast<float>(low);
}
#pragma warning(pop)

void movlayr(unsigned codedLayer);
void munfrm();
bool notfstch(unsigned attribute) noexcept;
void nubrdcol(unsigned color);
void nufilcol(unsigned color);
void nufsel();
void nufthcol(unsigned color);
void nulapcol(unsigned color);

unsigned int nxt(unsigned int iVertex) noexcept;

unsigned int pdir(unsigned int vertex);

void picot();
void prpbrd(float borderStitchSpacing);

constexpr unsigned int prv(unsigned int iVertex);

unsigned psg() noexcept;
void     pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect) noexcept;
void     ratsr();
void     rct2sel(const RECT& rectangle, std::vector<POINT>& line);
void     redup();
void     refil();
void     refilal();
void     refilfn();
void     rinfrm();
void     ritfrct(unsigned int iForm, HDC dc);
void     rotagain();
void     rotcmd();
void     rotdup();
void     rotfrm(unsigned int newStartVertex);
fPOINT   rotpar();
void     rstfrm();
void     sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
void     savblen(float fLength);
void     savplen(float length);
void     selal();
void     selalfil();
void     selfil(unsigned type);
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

	void adfrm(unsigned int iForm);
	void angout(FRMHED& angledForm);
	void apbrd();
	void bakseq();
	void bdrlin(unsigned int start, unsigned int finish, float stitchSize);
	void bean(unsigned start, unsigned finish);
	void bhbrd(double spacing);
	void bhcrnr(unsigned int vertex);
	void bhfn(unsigned int start, unsigned int finish, double spacing);
	void bholbrd();
	void blbrd(double spacing);
	void bold(double size);
	void boldlin(unsigned int start, unsigned int finish, double size);
	void brdfil(float pd_Size);
	void brkdun(const std::vector<SMALPNTL*>& sortedLines, unsigned int start, unsigned int finish);
	void brkseq(const std::vector<SMALPNTL*>& sortedLines,
	            unsigned int                  start,
	            unsigned int                  finish,
	            boost::dynamic_bitset<>&      sequenceMap,
	            unsigned&                     lastGroup,
	            SMALPNTL*                     sequenceLines);
	bool chk2of();
	void chkbrd();
	void chksid(unsigned int vertexIndex, unsigned clipIntersectSide, const std::vector<fPOINT>& currentFormVertices);
	bool closat(intersectionStyles& inOutFlag);
	bool clpcmp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept;
	void clpcon(const std::vector<RNGCNT>& textureSegments, std::vector<fPOINT>& currentFormVertices);
	void clpfm();

	unsigned clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
	                 std::vector<CLPSEG>&       clipSegments,
	                 const std::vector<double>& lengths,
	                 unsigned                   start,
	                 unsigned                   finish,
	                 const std::vector<fPOINT>& currentFormVertices);

	bool clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned sind);
	bool comp(const fPOINTLINE& point1, const fPOINTLINE& point2) noexcept;
	void contf();
	bool contsf(unsigned int formIndex);
	void cplayfn(unsigned int iForm, unsigned play);
	void delfil(unsigned attribute);

	constexpr unsigned duat(unsigned attribute);

	void dubfn();
	void ducon() noexcept;
	void dudif(const fPOINT& start, const fPOINT& finish, fPOINT& delta) noexcept;
	void dufcntr(fPOINT& center);
	void dufdat(std::vector<fPOINT>& tempClipPoints,
	            std::vector<SATCON>& tempGuides,
	            std::vector<fPOINT>& destinationFormVertices,
	            std::vector<FRMHED>& destinationFormList,
	            unsigned int         formIndex,
	            unsigned int&        formRelocationIndex,
	            unsigned int&        formSourceIndex);
	void duflt(float& formOffset, std::vector<fPOINT>& currentFormVertices);
	void dunseq(const std::vector<SMALPNTL*>& sortedLines, unsigned int start, unsigned int finish, unsigned& lastGroup);
	void dupfn(float rotationAngle);
	void duprotfs(float rotationAngle);
	void duprots(float rotationAngle, const fPOINT& rotationCenter);
	void durgn(const std::vector<FSEQ>&      sequencePath,
	           boost::dynamic_bitset<>&      visitedRegions,
	           const std::vector<SMALPNTL*>& sortedLines,
	           unsigned                      pthi,
	           unsigned int                  lineCount,
	           std::vector<REGION>&          regionsList,
	           unsigned&                     lastGroup,
	           unsigned                      sequencePathIndex);
	void duromb(const fPOINT& start0, const fPOINT& finish0, const fPOINT& start1, const fPOINT& finish1);
	void duseq(const std::vector<SMALPNTL*>& sortedLines,
	           unsigned int                  start,
	           unsigned int                  finish,
	           boost::dynamic_bitset<>&      sequenceMap,
	           unsigned&                     lastGroup,
	           SMALPNTL*                     sequenceLines);
	void duseq1(const SMALPNTL* sequenceLines);

	SMALPNTL* duseq2(SMALPNTL* sequenceLines);

	void  duspnd(const std::vector<VRCT2>& underlayVerticalRect,
	             const std::vector<VRCT2>& fillVerticalRect,
	             unsigned int              start,
	             unsigned int              finish);
	void  filsclp();
	void  filsfn();
	float findDistanceToSide(const fPOINT& lineStart, const fPOINT& lineEnd, const fPOINT& point, double& distance) noexcept;
	void  fmclp();
	void  fnagain(float rotationAngle);
	void  fnang(std::vector<unsigned>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            float                  rotationAngle,
	            fPOINT&                rotationCenter,
	            FRMHED&                angledForm,
	            std::vector<fPOINT>&   angledFormVertices);
	void  fnhor(std::vector<unsigned>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            float                  rotationAngle,
	            fPOINT&                rotationCenter,
	            FRMHED&                angledForm,
	            std::vector<fPOINT>&   angledFormVertices);
	void  fnord();
	void  fnvrt(std::vector<fPOINT>&   currentFillVertices,
	            std::vector<unsigned>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints);
	void  frmpnts(unsigned type) noexcept;
	void  frmpoly(const POINT* line, unsigned int count) noexcept;
	void  frmsqr(unsigned iVertex);
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
	             unsigned                   iPoint,
	             const std::vector<fPOINT>& currentFormVertices);
	void  horclpfn(const std::vector<RNGCNT>& textureSegments, FRMHED& angledForm, std::vector<fPOINT>& angledFormVertices);

	unsigned insect(std::vector<CLIPSORT>&     clipIntersectData,
	                const std::vector<VCLPX>&  regionCrossingData,
	                std::vector<CLIPSORT*>&    arrayOfClipIntersectData,
	                unsigned                   regionCrossingStart,
	                unsigned                   regionCrossingEnd,
	                const fPOINT&              lineSegmentStart,
	                const fPOINT&              lineSegmentEnd,
	                const std::vector<fPOINT>& currentFormVertices);

	void inspnt(std::vector<CLIPNT>& clipStitchPoints);
	bool isclos(const SMALPNTL* lineEndPoint0, const SMALPNTL* lineEndPoint1, double gapToClosestRegion) noexcept;

	bool isect(unsigned int               vertex0,
	           unsigned int               vertex1,
	           fPOINT&                    intersection,
	           float&                     length,
	           const fPOINT&              lineSegmentStart,
	           const fPOINT&              lineSegmentEnd,
	           const std::vector<fPOINT>& currentFormVertices);
	bool isin(std::vector<VCLPX>&        regionCrossingData,
	          float                      xCoordinate,
	          float                      yCoordinate,
	          unsigned                   regionCrossingStart,
	          unsigned                   regionCrossingEnd,
	          const fRECTANGLE&          boundingRect,
	          const std::vector<fPOINT>& currentFormVertices);
	void lapbrd();
	void lcon(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
	bool lencmp(const LENINFO& arg1, const LENINFO& arg2) noexcept;
	bool lencmpa(const CLIPSORT* arg1, const CLIPSORT* arg2) noexcept;
	void linrutb(unsigned start);
	bool lnclos(std::vector<unsigned>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            unsigned               group0,
	            unsigned               line0,
	            unsigned               group1,
	            unsigned               line1,
	            double                 gapToClosestRegion);
	void makpoli();
	void movseq(const std::vector<SMALPNTL*>& sortedLines, unsigned int ind);
	void mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, unsigned destination, unsigned source);
	bool notdun(std::vector<RGSEQ>&            tempPath,
	            const std::vector<RCON>&       pathMap,
	            const std::vector<unsigned>&   mapIndexSequence,
	            const boost::dynamic_bitset<>& visitedRegions,
	            unsigned                       pathLength,
	            unsigned                       doneRegion,
	            unsigned                       sequencePathIndex);
	bool notsel() noexcept;
	bool
	     nucseg(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned& currentSegmentIndex);
	void nufpnt(unsigned int vertex, FRMHED* formForInsert);
	void doTimeWindow(float rangeX, const std::vector<unsigned>& xPoints, const std::vector<unsigned>& xHistogram);
	void nxtrgn(std::vector<RGSEQ>&           tempPath,
	            const std::vector<RCON>&      pathMap,
	            const std::vector<unsigned>&  mapIndexSequence,
	            boost::dynamic_bitset<>&      visitedRegions,
	            const std::vector<SMALPNTL*>& sortedLines,
	            const std::vector<REGION>&    regionsList,
	            unsigned&                     doneRegion,
	            unsigned                      pathMapIndex,
	            unsigned&                     sequencePathIndex,
	            unsigned                      visitedIndex);
	void nxtseq(std::vector<FSEQ>&           sequencePath,
	            const std::vector<RCON>&     pathMap,
	            const std::vector<unsigned>& mapIndexSequence,
	            unsigned                     pathIndex,
	            unsigned&                    pathCount);

	double p2p(const fPOINT& point0, const fPOINT& point1) noexcept;

	void   pbrd(float edgeSpacing);
	void   pfn(const std::vector<VRCT2>& underlayVerticalRect,
	           const std::vector<VRCT2>& fillVerticalRect,
	           unsigned int              startVertex,
	           const std::vector<VRCT2>& vrct);
	void   plbak(unsigned int backPoint);
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
	bool   regclos(std::vector<unsigned>&        groupIndexSequence,
	               std::vector<SMALPNTL>&        lineEndpoints,
	               const std::vector<SMALPNTL*>& sortedLines,
	               unsigned                      iRegion0,
	               unsigned                      iRegion1,
	               const std::vector<REGION>&    regionsList,
	               double                        gapToClosestRegion,
	               unsigned&                     nextGroup);
	double reglen(const std::vector<SMALPNTL*>& sortedLines,
	              unsigned                      iRegion,
	              const std::vector<fPOINT>&    lastRegionCorners,
	              const std::vector<REGION>&    regionsList);
	void   ritapbrd();
	void   ritbrd();
	void   ritfil();
	bool   ritlin(const fPOINT& start, const fPOINT& finish, float userStitchLen);
	void   ritseg(const std::vector<CLIPNT>& clipStitchPoints,
	              std::vector<CLPSEG>&       clipSegments,
	              unsigned                   currentSegmentIndex,
	              unsigned&                  clipIntersectSide,
	              const std::vector<fPOINT>& currentFormVertices);
	void   rotbak(float rotationAngle, const fPOINT& rotationCenter) noexcept;
	void   rotentr(double rotationAngle);
	void   rspnt(float xCoordinate, float yCoordinate);
	void   sapliq();

	void sbold();
	void sbord();
	bool scomp(const SATCON& arg1, const SATCON& arg2) noexcept;
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
	void snp(unsigned start, unsigned finish);
	void snpfn(const std::vector<unsigned>& xPoints, unsigned start, unsigned end, unsigned finish);
	void spend(const std::vector<VRCT2>& fillVerticalRect, unsigned int start, unsigned int finish);
	bool spltlin();
	void sprct(std::vector<VRCT2>& fillVerticalRect, unsigned start, unsigned finish);
	void spurct(std::vector<VRCT2>& underlayVerticalRect, const std::vector<VRCT2>& fillVerticalRect, unsigned iRect);
	void
	     spurfn(const fPOINT& innerPoint, const fPOINT& outerPoint, fPOINT& underlayInnerPoint, fPOINT& underlayOuterPoint) noexcept;
	bool sqcomp(const SMALPNTL* arg1, const SMALPNTL* arg2) noexcept;
	void srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, unsigned start, unsigned finish);
	void stchfrm(unsigned formIndex, unsigned* attribute) noexcept;
	void
	     trfrm(const fPOINT& bottomLeftPoint, const fPOINT& topLeftPoint, const fPOINT& bottomRightPoint, const fPOINT& topRightPoint);
	void unbean(unsigned start, unsigned finish);
	void uncon();

	unsigned leftsid(const std::vector<fPOINT>& currentFormVertices);

	bool unvis(const boost::dynamic_bitset<>& visitedRegions, unsigned& visitedIndex);
	bool vscmp(unsigned index1, unsigned index2);
} // namespace internal
} // namespace form