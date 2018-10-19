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
void         angclpfn(const std::vector<RNGCNT>& textureSegments);
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
unsigned     chkfrm(std::vector<POINT>& stretchBoxLine, double& xyRatio);
bool         chkmax(unsigned int arg0, unsigned int arg1) noexcept;
void         chkseq(bool border);
bool         cisin(float xCoordinate, float yCoordinate) noexcept;
unsigned int closflt(float xCoordinate, float yCoordinate) noexcept;
bool         closfrm();
void         clpfil();
void         clpspac(unsigned int insertPoint, unsigned int count) noexcept;
void         clrfills() noexcept;
void         cntrx();
void         col2frm();
void         contfil();
void         cpylayr(unsigned codedLayer);
void         crop();
void         debean();
void         delflt(unsigned int formIndex) noexcept;
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
void         dufrm();
void         duhart(unsigned sideCount);
void         dulens(unsigned sides);
void         duinsf() noexcept;
void         duprot(double rotationAngle, const dPOINT& rotationCenter);
void         dupsel(HDC dc);
void         durpoli(unsigned vertexCount);
void         duspir(unsigned stepCount);
void         dusqr();
void         dustar(unsigned starCount, double length);
void         duzig(unsigned vertices);
void         fclp();
void         fcntr();
void         filangl();
void         filclpx();
void         filhor();
void         filin(dPOINT currentPoint);
void         filinsb(const dPOINT& point);
void         filinsbw(std::vector<dPOINT>& satinBackup, const dPOINT& point);
void         filinu(const dPOINT& inPoint);
void         filsat();
void         filvrt();
unsigned     find1st();
void         fliph();
void         flipv();
void         flpord();
unsigned     fltind(const fPOINT* point) noexcept;
void         fltspac(const fPOINT* start, unsigned int count) noexcept;
void         form();
void         frm0();
void         frmadj(unsigned int formIndex) noexcept;
void         frmclr(FRMHED* destination) noexcept;
void         frmlin(const fPOINT* vertices, unsigned int vertexCount);
void         frmlin(const std::vector<fPOINT>& vertices);
void         frmnumfn(unsigned newFormIndex);
void         frmon();
void         frmout(unsigned int formIndex) noexcept;
void         frmovlin();
bool         frmrng(unsigned int iForm, RANGE& range) noexcept;
void         frmsadj();
void         fselrct(unsigned int iForm);
void         fsizpar() noexcept;
void         fvars(unsigned int iForm) noexcept;

float getblen() noexcept;

unsigned int getlast() noexcept;

float getplen() noexcept;

void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
bool lastch() noexcept;
bool linx(const std::vector<fPOINT>& points, unsigned int start, unsigned int finish, dPOINT& intersection);
void makspac(unsigned start, unsigned count);
void mdufrm();

constexpr float midl(float high, float low);

void movlayr(unsigned codedLayer);
void munfrm();
void mvfltsb(fPOINT* destination, const fPOINT* source, unsigned int count) noexcept;
void mvfrmsb(FRMHED* destination, const FRMHED* source, unsigned int count) noexcept;
bool notfstch(unsigned attribute) noexcept;
void nubrdcol(unsigned color);
void nufilcol(unsigned color);
void nufsel();
void nufthcol(unsigned color);
void nulapcol(unsigned color);

unsigned int nxt(unsigned int iVertex) noexcept;

unsigned int pdir(unsigned int vertex);

void picot();
void prpbrd(double borderStitchSpacing);

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
void     ritseq1(unsigned int ind) noexcept;
void     rotagain();
void     rotcmd(dPOINT& rotationCenter);
void     rotdup();
void     rotfrm(unsigned int newStartVertex);
void     rstfrm();
void     sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
void     savblen(float fLength);
void     savplen(float length);
void     selal();
void     selalfil();
void     selfil(unsigned type);
void     selsqr(const POINT& controlPoint, HDC dc);
void     setap();
void     setexpand(double xyRatio);
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

	void     adfrm(unsigned int iForm);
	void     angout() noexcept;
	void     apbrd();
	void     bakseq();
	void     bdrlin(unsigned int start, unsigned int finish, double stitchSize);
	void     bean(unsigned start, unsigned finish);
	void     bhbrd(double spacing);
	void     bhcrnr(unsigned int vertex);
	void     bhfn(unsigned int start, unsigned int finish, double spacing);
	void     bholbrd();
	void     blbrd(double spacing);
	void     bold(double size);
	void     boldlin(unsigned int start, unsigned int finish, double size);
	void     brdfil(double pd_Size);
	void     brkdun(const std::vector<SMALPNTL*>& sortedLines, unsigned int start, unsigned int finish);
	void     brkseq(const std::vector<SMALPNTL*>& sortedLines,
	                unsigned int                  start,
	                unsigned int                  finish,
	                boost::dynamic_bitset<>&      sequenceMap);
	bool     chk2of();
	void     chkbrd(unsigned& interleaveSequenceIndex2);
	void     chksid(unsigned int vertexIndex, unsigned clipIntersectSide) noexcept;
	bool     closat(intersectionStyles& inOutFlag) noexcept;
	bool     clpcmp(const VCLPX& vclpx1, const VCLPX& vclpx2) noexcept;
	void     clpcon(const std::vector<RNGCNT>& textureSegments);
	void     clpfm();
	unsigned clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
	                 std::vector<CLPSEG>&       clipSegments,
	                 const std::vector<double>& lengths,
	                 unsigned                   start,
	                 unsigned                   finish);
	bool     clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned sind);
	bool     comp(const dPOINTLINE& point1, const dPOINTLINE& point2) noexcept;
	void     contf();
	bool     contsf(unsigned int formIndex);
	void     cplayfn(unsigned int iForm, unsigned play);
	void     delfil(unsigned attribute);

	constexpr unsigned duat(unsigned attribute);

	void  dubfn();
	void  ducon() noexcept;
	void  dudif(const dPOINT& start, const dPOINT& finish, dPOINT& delta) noexcept;
	void  dufcntr(dPOINT& center);
	void  dufdat(std::vector<fPOINT>& tempClipPoints,
	             std::vector<SATCON>& tempGuides,
	             std::vector<fPOINT>& destinationFormVertices,
	             std::vector<FRMHED>& destinationFormList,
	             unsigned int         formIndex,
	             unsigned int&        FormRelocationIndex);
	void  duflt();
	void  duinf(const FRMHED& formHeader) noexcept;
	void  dunseq(const std::vector<SMALPNTL*>& sortedLines, unsigned int start, unsigned int finish);
	void  dupfn(double rotationAngle);
	void  duprotfs(double rotationAngle);
	void  duprots(double rotationAngle, const dPOINT& rotationCenter);
	void  durgn(const std::vector<FSEQ>&      sequencePath,
	            boost::dynamic_bitset<>&      visitedRegions,
	            const std::vector<SMALPNTL*>& sortedLines,
	            unsigned                      pthi,
	            unsigned int                  lineCount,
	            std::vector<REGION>&          regionsList);
	void  duromb(const dPOINT& start0, const dPOINT& finish0, const dPOINT& start1, const dPOINT& finish1);
	void  duseq(const std::vector<SMALPNTL*>& sortedLines,
	            unsigned int                  start,
	            unsigned int                  finish,
	            boost::dynamic_bitset<>&      sequenceMap);
	void  duseq1() noexcept;
	void  duseq2(const std::vector<SMALPNTL*>& sortedLines, unsigned int iLine);
	void  duspnd(const std::vector<VRCT2>& underlayVerticalRect,
	             const std::vector<VRCT2>& fillVerticalRect,
	             unsigned int              start,
	             unsigned int              finish);
	void  filsclp();
	void  filsfn();
	float findDistanceToSide(const fPOINT& lineStart, const fPOINT& lineEnd, const fPOINT& point, double& distance) noexcept;
	void  fmclp();
	void  fnagain(double rotationAngle);
	void  fnang(std::vector<unsigned>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            double                 rotationAngle,
	            dPOINT&                rotationCenter);
	void  fnhor(std::vector<unsigned>& groupIndexSequence,
	            std::vector<SMALPNTL>& lineEndpoints,
	            double                 rotationAngle,
	            dPOINT&                rotationCenter);
	void  fnord();
	void  fnvrt(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
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
	void  getfinfo(unsigned int iForm) noexcept;
	float getlen(std::vector<CLIPNT>& clipStitchPoints, const std::vector<double>& lengths, unsigned iPoint);
	void  horclpfn(const std::vector<RNGCNT>& textureSegments);

	unsigned insect(std::vector<CLIPSORT>&    clipIntersectData,
	                const std::vector<VCLPX>& regionCrossingData,
	                std::vector<CLIPSORT*>&   arrayOfClipIntersectData,
	                unsigned                  regionCrossingStart,
	                unsigned                  regionCrossingEnd);
	void     inspnt(std::vector<CLIPNT>& clipStitchPoints);

	bool isclos(const SMALPNTL* lineEndPoint0, const SMALPNTL* lineEndPoint1, double gapToClosestRegion) noexcept;

	bool isect(unsigned int vertex0, unsigned int vertex1, fPOINT& intersection, float& length) noexcept;
	bool isin(std::vector<VCLPX> regionCrossingData,
	          float              xCoordinate,
	          float              yCoordinate,
	          unsigned           regionCrossingStart,
	          unsigned           regionCrossingEnd);
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
	void makpoli() noexcept;
	void movseq(const std::vector<SMALPNTL*>& sortedLines, unsigned int ind);
	void mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, unsigned destination, unsigned source);
	bool notdun(std::vector<RGSEQ>&            tempPath,
	            const std::vector<RCON>&       pathMap,
	            const std::vector<unsigned>&   mapIndexSequence,
	            const boost::dynamic_bitset<>& VisitedRegions,
	            unsigned                       level,
	            unsigned                       doneRegion);
	bool notsel() noexcept;
	bool
	       nucseg(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned& currentSegmentIndex);
	void   nufpnt(unsigned int vertex);
	void   nutim(double size) noexcept;
	void   nxtim();
	void   nxtrgn(std::vector<RGSEQ>&           tempPath,
	              const std::vector<RCON>&      pathMap,
	              const std::vector<unsigned>&  mapIndexSequence,
	              boost::dynamic_bitset<>&      visitedRegions,
	              const std::vector<SMALPNTL*>& sortedLines,
	              const std::vector<REGION>&    regionsList,
	              unsigned&                     doneRegion,
	              unsigned                      pathMapIndex);
	void   nxtseq(std::vector<FSEQ>&           sequencePath,
	              const std::vector<RCON>&     pathMap,
	              const std::vector<unsigned>& mapIndexSequence,
	              unsigned                     pathIndex,
	              unsigned&                    pathCount);
	double p2p(const fPOINT& point0, const fPOINT& point1) noexcept;
	void   pbrd(double edgeSpacing);
	void   pfn(const std::vector<VRCT2>& underlayVerticalRect,
	           const std::vector<VRCT2>& fillVerticalRect,
	           unsigned int              startVertex,
	           const std::vector<VRCT2>& vrct);
	void   plbak(unsigned int backPoint) noexcept;
	void   plbrd(double edgeSpacing);
	void   plfn(const std::vector<VRCT2>& underlayVerticalRect,
	            const std::vector<VRCT2>& fillVerticalRect,
	            const std::vector<VRCT2>& prct);
	void   prebrd();
	bool proj(const dPOINT& point, double slope, const fPOINT& point0, const fPOINT& point1, dPOINT& intersectionPoint) noexcept;
	bool projh(double yCoordinate, const fPOINT& point0, const fPOINT& point1, dPOINT& intersection) noexcept;
	bool projv(double xCoordinate, const fPOINT& lowerPoint, const fPOINT& upperPoint, dPOINT& intersection) noexcept;
	void prpsbrd();
	void prsmal() noexcept;
	void px2stchf(const POINT& screen, fPOINT& stitchPoint) noexcept;
	void rats();
	bool regclos(std::vector<unsigned>&        groupIndexSequence,
	             std::vector<SMALPNTL>&        lineEndpoints,
	             const std::vector<SMALPNTL*>& sortedLines,
	             unsigned                      iRegion0,
	             unsigned                      iRegion1,
	             const std::vector<REGION>&    regionsList,
	             double                        gapToClosestRegion,
	             unsigned&                     NextGroup);
	double reglen(const std::vector<SMALPNTL*>& sortedLines,
	              unsigned                      iRegion,
	              const std::vector<fPOINT>&    lastRegionCorners,
	              const std::vector<REGION>&    regionsList);
	void   ritapbrd(unsigned& interleaveSequenceIndex2);
	void   ritbrd(unsigned& interleaveSequenceIndex2);
	void   ritfil(unsigned& interleaveSequenceIndex2);
	bool   ritlin(const fPOINT& start, const fPOINT& finish);
	void   ritseg(const std::vector<CLIPNT>& clipStitchPoints,
	              std::vector<CLPSEG>&       clipSegments,
	              unsigned                   currentSegmentIndex,
	              unsigned&                  clipIntersectSide);
	void   rotbak(double rotationAngle, const dPOINT& rotationCenter) noexcept;
	void   rotentr(double rotationAngle);
	void   rotpar(dPOINT& rotationCenter);
	void   rspnt(float xCoordinate, float yCoordinate) noexcept;
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

	constexpr float shreg(float highValue, float reference, double EggRatio) noexcept;

	void shrnks();
	void snp(unsigned start, unsigned finish);
	void snpfn(const std::vector<unsigned>& xPoints, unsigned start, unsigned end, unsigned finish);
	void spend(const std::vector<VRCT2>& fillVerticalRect, unsigned int start, unsigned int finish);
	bool spltlin();
	void sprct(std::vector<VRCT2>& fillVerticalRect, unsigned start, unsigned finish);
	void spurct(std::vector<VRCT2>& underlayVerticalRect, const std::vector<VRCT2>& fillVerticalRect, unsigned iRect);
	void
	     spurfn(const dPOINT& innerPoint, const dPOINT& outerPoint, dPOINT& underlayInnerPoint, dPOINT& underlayOuterPoint) noexcept;
	bool sqcomp(const SMALPNTL* arg1, const SMALPNTL* arg2) noexcept;
	void srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, unsigned start, unsigned finish);
	void stchfrm(unsigned formIndex, unsigned* attribute) noexcept;
	void trfrm(const dPOINT& bottomLeftPoint,
	           const dPOINT& topLeftPoint,
	           const dPOINT& bottomRightPoint,
	           const dPOINT& topRightPoint) noexcept;
	void unbean(unsigned start, unsigned finish);
	void uncon();

	unsigned leftsid() noexcept;

	bool unvis(const boost::dynamic_bitset<>& visitedRegions);
	bool vscmp(unsigned index1, unsigned index2) noexcept;
} // namespace internal
} // namespace form