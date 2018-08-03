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
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"

//void adfrm(size_t iForm);
void angclp();
void angclpfn(const std::vector<RNGCNT>& textureSegments);
//void angout();
void angsclp();
//void apbrd();
void apliq();
void bakagain();
void bakdup();
//void bdrlin(size_t start, size_t finish, double stitchSize);
//void bean(unsigned start, unsigned finish);
//void bhbrd(double spacing);
//void bhcrnr(size_t vertex);
//void bhfn(size_t start, size_t finish, double spacing);
void bhol();
//void bholbrd();
//void blbrd(double spacing);
//void bold(double size);
//void boldlin(size_t start, size_t finish, double size);
void bord();
void boxsel();
//void brdfil(double pd_Size);
//void brkdun(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish);
//void brkseq(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish, boost::dynamic_bitset<>& sequenceMap);
void bsizpar();
void centir();
void chain();
void chan();

//bool               chk2of();
//bool               chkbak(const std::vector<dPOINT>& satinBackup, const dPOINT& pnt);
//void               chkbrd(unsigned& interleaveSequenceIndex2);
void               chkcont();
//bool               chkdel();
unsigned           chkfrm(std::vector<POINT>& stretchBoxLine, double& xyRatio);
bool               chkmax(size_t arg0, size_t arg1);
void               chkseq(bool border) noexcept;
//void               chksid(size_t vertexIndex);
bool               cisin(float xCoordinate, float yCoordinate);
//unsigned           closat();
size_t             closflt(float xCoordinate, float yCoordinate);
unsigned           closfrm();
//bool               clpcmp(const VCLPX& vclpx1, const VCLPX& vclpx2);
//void               clpcon(const std::vector<RNGCNT>& textureSegments);
void               clpfil();
//void               clpfm();
//unsigned           clpind(const fPOINT* const point);
//unsigned           clpnseg(std::vector<CLIPNT>&       clipStitchPoints,
//                           std::vector<CLPSEG>&       clipSegments,
//                           const std::vector<double>& lengths,
//                           unsigned                   start,
//                           unsigned                   finish);
//bool               clpnxt(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned sind);
//void               clpspac(const fPOINT* const insertPoint, size_t count);
void               clrfills();
void               cntrx();
void               col2frm();
//bool               comp(const dPOINTLINE& point1, const dPOINTLINE& point2) noexcept;
//void               contf();
void               contfil();
//bool               contsf(size_t formIndex);
//void               cplayfn(size_t iForm, unsigned play);
void               cpylayr(unsigned codedLayer);
void               crop();
void               debean();
void               delcon(unsigned GuideIndex);
//void               delfil(unsigned attribute);
void               delflt(size_t formIndex);
void               delfrms();
void               delmfil();
void               delsac(size_t formIndex);
void               delspnt();
void               drwcon();
void               drwfrm();
void               drwsat();
void               duangs();
//constexpr unsigned duat(unsigned attribute);
void               dubean();
void               dubfil();
//void               dubfn();
void               dubig();
void               dubold();
void               dubsfil();
//void               ducon();
//void               dudif(const dPOINT& start, const dPOINT& finish, dPOINT* const delta);
void               dueg(unsigned sides);
//void               dufcntr(dPOINT& center);
//void               dufdat(std::vector<fPOINT>& tempClipPoints,
//                          std::vector<SATCON>& tempGuides,
//                          std::vector<fPOINT>& destinationFormVertices,
//                          std::vector<FRMHED>& destinationFormList,
//                          size_t               formIndex);
//void               duflt();
void               duform(unsigned formType);
void               dufrm();
void               duhart(unsigned sideCount);
//void               duinf(const FRMHED& formHeader) noexcept;
void               duinsf();
void               dulens(unsigned sides);
//void               dunseq(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish);
//void               dupfn(double rotationAngle);
void               duprot(double rotationAngle, const dPOINT& rotationCenter);
//void               duprotfs(double rotationAngle);
//void               duprots(double rotationAngle, const dPOINT& rotationCenter);
void               dupsel(HDC dc);
//void               durgn(const std::vector<FSEQ>&      sequencePath,
//                         boost::dynamic_bitset<>&      visitedRegions,
//                         const std::vector<SMALPNTL*>& sortedLines,
//                         unsigned                      pthi,
//                         size_t                        lineCount,
//                         std::vector<REGION>&          regionsList);
//void               duromb(const dPOINT& start0, const dPOINT& finish0, const dPOINT& start1, const dPOINT& finish1);
void               durpoli(unsigned vertexCount);
void               dusat();

//void duseq(const std::vector<SMALPNTL*>& sortedLines, size_t start, size_t finish, boost::dynamic_bitset<>& sequenceMap);

//void     duseq1();
//void     duseq2(const std::vector<SMALPNTL*>& sortedLines, size_t iLine);
void     duspir(unsigned stepCount);
//void     duspnd(const std::vector<VRCT2>& underlayVerticalRect,
//                const std::vector<VRCT2>& fillVerticalRect,
//                size_t                    start,
//                size_t                    finish);
void     dusqr();
void     dustar(unsigned starCount, double length);
void     duzig(unsigned vertices);
void     fclp();
void     fcntr();
void     filangl();
void     filclpx();
void     filhor();
//void     filin(dPOINT currentPoint);
//void     filinsb(const dPOINT& point);
//void     filinsbw(std::vector<dPOINT>& satinBackup, const dPOINT& point);
//void     filinu(const dPOINT& inPoint);
void     filsat();
//void     filsclp();
//void     filsfn();
void     filvrt();
unsigned find1st();
//float    findDistanceToSide(const fPOINT& lineStart, const fPOINT& lineEnd, const fPOINT& point, double& distance);
void     fliph();
void     flipv();
void     flpord();
unsigned fltind(const fPOINT* const point);
void     fltspac(const fPOINT* const start, size_t count);
//void     fmclp();
//void     fnagain(double rotationAngle);
//void     fnang(std::vector<unsigned>& groupIndexSequence,
//               std::vector<SMALPNTL>& lineEndpoints,
//               double                 rotationAngle,
//               dPOINT&                rotationCenter);
//void     fnhor(std::vector<unsigned>& groupIndexSequence,
//               std::vector<SMALPNTL>& lineEndpoints,
//               const double           rotationAngle,
//               dPOINT&                rotationCenter);
//void     fnord();
//void     fnvrt(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
void     form();
void     frm0();
void     frmadj(size_t formIndex);
void     frmclr(FRMHED* const destination) noexcept;
//void     frmlin(const fPOINT* const vertices, size_t vertexCount);
//void     frmlin(const std::vector<fPOINT>& vertices);
void     frmnumfn(unsigned newFormIndex);
void     frmon();
void     frmout(size_t formIndex);
void     frmovlin();
//void     frmpnts(unsigned type);
//void     frmpoly(const POINT* const line, size_t count);
bool     frmrng(size_t iForm, RANGE& range);
void     frmsadj();
//void     frmsqr(unsigned iVertex);
//void     frmsqr0(const POINT& controlPoint);
//void     frmx(const POINT& controlPoint, HDC dc);
//void     fsangl();
//void     fsclp();
//void     fsclpx();
void     fselrct(size_t iForm);
//void     fshor();
void     fsizpar();
//void     fspic();
//void     fsvrt();
void     fvars(size_t iForm);
//void     getbig();

constexpr float getblen();

//void   getfinfo(size_t iForm);
//size_t getlast();
//float  getlen(std::vector<CLIPNT>& clipStitchPoints, const std::vector<double>& lengths, unsigned iPoint);

constexpr float getplen();

void            horclp();
//void            horclpfn(const std::vector<RNGCNT>& textureSegments);
void            horsclp();
void            infrm();
void            insat();
//unsigned        insect(std::vector<CLIPSORT>&    clipIntersectData,
//                       const std::vector<VCLPX>& regionCrossingData,
//                       std::vector<CLIPSORT*>&   arrayOfClipIntersectData,
//                       unsigned                  regionCrossingStart,
//                       unsigned                  regionCrossingEnd);
//void            inspnt(std::vector<CLIPNT>& clipStitchPoints);
//unsigned short  isclos(const SMALPNTL* const lineEndPoint0, const SMALPNTL* const lineEndPoint1);
//bool            isect(size_t vertex0, size_t vertex1, fPOINT& intersection, float& length);
//bool            isin(const std::vector<VCLPX> regionCrossingData,
//                     float                    xCoordinate,
//                     float                    yCoordinate,
//                     unsigned                 regionCrossingStart,
//                     unsigned                 regionCrossingEnd);
void            ispcdclp();
void            join();
//void            lapbrd();
bool            lastch();
//void            lcon(std::vector<unsigned>& groupIndexSequence, std::vector<SMALPNTL>& lineEndpoints);
//bool            lencmp(const LENINFO& arg1, const LENINFO& arg2);
//bool            lencmpa(const CLIPSORT* const arg1, const CLIPSORT* const arg2);
//void            linrutb(unsigned start);
//bool            linx(const std::vector<fPOINT>& points, size_t start, size_t finish, dPOINT& intersection);
//bool            lnclos(std::vector<unsigned>& groupIndexSequence,
//                       std::vector<SMALPNTL>& lineEndpoints,
//                       unsigned               group0,
//                       unsigned               line0,
//                       unsigned               group1,
//                       unsigned               line1);
//void            makpoli();
void            makspac(unsigned start, unsigned count);
void            mdufrm();
constexpr float midl(float high, float low);
void            movlayr(unsigned codedLayer);
//void            movseq(const std::vector<SMALPNTL*>& sortedLines, size_t ind);
void            munfrm();
//void            mvfltsb(fPOINT* const destination, const fPOINT* const source, size_t count);
//void            mvfrmsb(FRMHED* const destination, const FRMHED* const source, size_t count);
//void            mvpclp(std::vector<CLIPSORT*>& arrayOfClipIntersectData, unsigned destination, unsigned source);
//unsigned        notdun(std::vector<RGSEQ>&            tempPath,
//                       const std::vector<RCON>&       pathMap,
//                       const std::vector<unsigned>&   mapIndexSequence,
//                       const boost::dynamic_bitset<>& VisitedRegions,
//                       unsigned                       level);
bool            notfstch(unsigned attribute);
//bool            notsel();
void            nubrdcol(unsigned color);

//bool nucseg(const std::vector<CLPSEG>& clipSegments, const std::vector<LENINFO>& sortedLengths, unsigned& currentSegmentIndex);

void             nufilcol(unsigned color);
//void             nufpnt(size_t vertex);
void             nufsel();
void             nufthcol(unsigned color);
void             nulapcol(unsigned color);
//SATCON*          nusac(size_t formIndex, unsigned guideCount);
//void             nutim(double size);
constexpr size_t nxt(size_t iVertex);
//void             nxtim();
//void             nxtrgn(std::vector<RGSEQ>&           tempPath,
//                        const std::vector<RCON>&      pathMap,
//                        const std::vector<unsigned>&  mapIndexSequence,
//                        boost::dynamic_bitset<>&      visitedRegions,
//                        const std::vector<SMALPNTL*>& sortedLines,
//                        const std::vector<REGION>&    regionsList);
//void             nxtseq(std::vector<FSEQ>&           sequencePath,
//                        const std::vector<RCON>&     pathMap,
//                        const std::vector<unsigned>& mapIndexSequence,
//                        unsigned                     pathIndex);
//void             outfn(unsigned start, unsigned finish, double satinWidth);
//double           p2p(const fPOINT& point0, const fPOINT& point1);
//void             pbrd(double edgeSpacing);
size_t           pdir(size_t vertex);
//void             pfn(const std::vector<VRCT2>& underlayVerticalRect,
//                     const std::vector<VRCT2>& fillVerticalRect,
//                     size_t                    startVertex,
//                     const std::vector<VRCT2>& vrct);

void             picot();
//void             plbak(size_t backPoint);
//void             plbrd(double edgeSpacing);
//void             plfn(const std::vector<VRCT2>& underlayVerticalRect,
//                      const std::vector<VRCT2>& fillVerticalRect,
//                      const std::vector<VRCT2>& prct);
//void             prebrd();
//bool             proj(const dPOINT& point, double slope, const fPOINT& point0, const fPOINT& point1, dPOINT& intersectionPoint);
//bool             projh(double yCoordinate, const fPOINT& point0, const fPOINT& point1, dPOINT& intersection);
//bool             projv(double xCoordinate, const fPOINT& lowerPoint, const fPOINT& upperPoint, dPOINT& intersection);
void             prpbrd(double borderStitchSpacing);
//void             prpsbrd();
//void             prsmal();
constexpr size_t prv(size_t iVertex);
unsigned         psg();
//void             px2stchf(const POINT& screen, fPOINT& stitchPoint);
void             pxrct2stch(const RECT& screenRect, fRECTANGLE& stitchRect);
//void             rats();
void             ratsr();
void             rct2sel(const RECT& rectangle, std::vector<POINT>& line);
void             redup();
void             refil();
void             refilal();
void             refilfn();
//bool             regclos(std::vector<unsigned>&        groupIndexSequence,
//                         std::vector<SMALPNTL>&        lineEndpoints,
//                         const std::vector<SMALPNTL*>& sortedLines,
//                         unsigned                      iRegion0,
//                         unsigned                      iRegion1,
//                         const std::vector<REGION>&    regionsList);
//double           reglen(const std::vector<SMALPNTL*>& sortedLines,
//                        unsigned                      iRegion,
//                        const std::vector<fPOINT>&    lastRegionCorners,
//                        const std::vector<REGION>&    regionsList);
void             ribon();
void             rinfrm();
//void             ritapbrd(unsigned& interleaveSequenceIndex2) noexcept;
//void             ritbrd(unsigned& interleaveSequenceIndex2) noexcept;
//void             ritfil(unsigned& interleaveSequenceIndex2) noexcept;
void             ritfrct(size_t iForm, HDC dc);
//bool             ritlin(const fPOINT& start, const fPOINT& finish);

//void ritseg(const std::vector<CLIPNT>& clipStitchPoints, std::vector<CLPSEG>& clipSegments, const unsigned currentSegmentIndex);

//void            ritseq1(size_t ind);
void            rotagain();
//void            rotbak(double rotationAngle, const dPOINT& rotationCenter);
void            rotcmd(dPOINT& rotationCenter);
void            rotdup();
//void            rotentr(double rotationAngle);
//void            rotfrm(size_t newStartVertex);
//void            rotpar(dPOINT& rotationCenter);
//void            rspnt(float xCoordinate, float yCoordinate);
void            rstfrm();
void            sRct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
//void            sacspac(const SATCON* const startGuide, unsigned guideCount);
//void            sapliq();
void            satadj();
void            satbrd();
//void            satclos();
//void            satcpy(SATCON* const destination, const std::vector<SATCON> source, unsigned int size);
//void            satends(unsigned isBlunt);
void            satfil();
void            satfix();
//void            satfn(const std::vector<double>& lengths, size_t line1Start, size_t line1End, size_t line2Start, size_t line2End);
//unsigned        satind(const SATCON* const guide);
void            satknkt();
//void            satmf(const std::vector<double>& lengths);
void            satout(double satinWidth);
void            satpnt0();
void            satpnt1();
//void            satsbrd();
void            satsel();
//bool            satselfn();
void            satzum();
void            savblen(float fLength);
void            savplen(float length);
//void            sbfn(const std::vector<fPOINT>& insidePoints, size_t start, size_t finish);
//void            sbold();
//void            sbord();
//void            sbrd();
//bool            scomp(const SATCON& arg1, const SATCON& arg2);
void            selal();
void            selalfil();
void            selfil(unsigned type);
void            selsqr(const POINT& controlPoint, HDC dc);
void            setap();
//void            seteg();
void            setexpand(double xyRatio);
void            setfpnt();
void            setfrm();
//void            sethart();
void            sethup();
void            setins();
//void            setlens();
//void            setlin();
void            setmfrm();
//void            setpoli();
void            setrang();
//void            setrpoli();
//void            setspir();
//void            setstar();
void            setstrtch();
//void            setzig();
void            sfCor2px(const fPOINT& stitchPoint, POINT& screen);
//void            sfn(size_t startVertex);
//constexpr float shreg(float highValue, float reference);
void            shrnk();
//void            shrnks();
//void            slbrd();
void            snap();
//void            snp(unsigned start, unsigned finish);
//void            snpfn(const std::vector<unsigned>& xPoints, unsigned start, unsigned end, unsigned finish);
//void            spend(const std::vector<VRCT2>& fillVerticalRect, size_t start, size_t finish);
void            spltfrm();
//bool            spltlin();
//void            spltsat(const SATCON& currentGuide);
//void            sprct(std::vector<VRCT2>& fillVerticalRect, unsigned start, unsigned finish);
//void            spurct(std::vector<VRCT2>& underlayVerticalRect, const std::vector<VRCT2>& fillVerticalRect, unsigned iRect);

//void spurfn(const dPOINT& innerPoint, const dPOINT& outerPoint, dPOINT& underlayInnerPoint, dPOINT& underlayOuterPoint);

//bool sqcomp(const SMALPNTL* arg1, const SMALPNTL* arg2);
void srtbyfrm();
//void srtf(const std::vector<fPOINTATTR>& tempStitchBuffer, unsigned start, unsigned finish);
void srtfrm();
//void stchadj();
//void stchfrm(unsigned formIndex, unsigned* attribute);
void stchrct2px(const fRECTANGLE& stitchRect, RECT& screenRect);
void stchs2frm();


void               tglfrm();
//void               trfrm(const dPOINT& bottomLeftPoint,
//                         const dPOINT& topLeftPoint,
//                         const dPOINT& bottomRightPoint,
//                         const dPOINT& topRightPoint);
//void               unbean(unsigned start, unsigned finish);
//void               uncon();
void               unfil();
void               unfrm();
void               uninsf();
void               unpsel();
//void               unsat();
//constexpr unsigned leftsid();
//bool               unvis(const boost::dynamic_bitset<>& visitedRegions);
void               vrtclp();
void               vrtsclp();
//bool               vscmp(unsigned index1, unsigned index2);

extern unsigned   BeanCount;