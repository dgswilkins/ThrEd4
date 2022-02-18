#pragma once

// Local Headers
#include "formHeader.h"
#include "regionHeader.h"

// Open Source headers
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26814)
#include "boost/dynamic_bitset.hpp"
#pragma warning(pop)

constexpr auto SQRCORNS = 4U; // number of corners in a square

// is intersection of line and cursor in, before or after the line
enum class IntersectionStyles { POINT_IN_LINE = 0, POINT_BEFORE_LINE, POINT_AFTER_LINE };

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

} // namespace form
