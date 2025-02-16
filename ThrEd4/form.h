#pragma once

// Local Headers
#include "formHeader.h"

constexpr auto SQRCORNS = 4U; // number of corners in a square

// is intersection of line and cursor in, before or after the line
enum class IntersectionStyles : uint8_t { POINT_IN_LINE = 0, POINT_BEFORE_LINE, POINT_AFTER_LINE };

namespace form {

void angclp();
void angclpfn(FRM_HEAD const& form, std::vector<RNG_COUNT> const& textureSegments, std::vector<F_POINT>& angledFormVertices);
void angsclp(FRM_HEAD& form);
void apliq();
void bakagain();
void bhol();
void boxsel();
void bord();
void bsizpar(FRM_HEAD& currentForm) noexcept;
void centir();
void chain();
void chkcont();
auto chkdel(FRM_HEAD const& currentForm) noexcept -> bool;
auto chkfrm(std::vector<POINT>& formControlPoints, std::vector<POINT>& stretchBoxLine, float& xyRatio) -> bool;
auto chkmax(uint32_t arg0, uint32_t arg1) noexcept -> bool;
void chkseq(bool border);
auto cisin(FRM_HEAD const& form,
           float           xCoordinate,
           float           yCoordinate) noexcept(!std::is_same_v<size_t, uint32_t>) -> bool;
auto closflt(FRM_HEAD const& form,
             float           xCoordinate,
             float           yCoordinate) noexcept(!std::is_same_v<ptrdiff_t, int>) -> uint32_t;
auto closfrm(uint32_t& formIndex) -> bool;
void clpfil();
void clpspac(uint32_t insertPoint, uint32_t count);
void clrfills() noexcept;
void cntrx();
void col2frm();
void contfil();
void cpylayr(uint32_t layer);
void createFormSelectedPen() noexcept;
void createLayerPens() noexcept;
void crop();
void debean();
void delflt(uint32_t formIndex) noexcept(!std::is_same_v<ptrdiff_t, int>);
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
void dupfn(float rotationAngle);
void dupsel(HDC hDC);
void durpoli(uint32_t vertexCount);
void duspir(uint32_t stepCount);
void dustar(uint32_t starCount, float starRatio, float length);
void duzig(uint32_t vertices);
void fclp();
void fcntr();
void filangl();
void filclpx();
void filhor();
void filin(F_POINT const& currentPoint, F_POINT const& stitchPoint);
void filinsb(F_POINT const& point, F_POINT& stitchPoint);
void filinu(F_POINT const& inPoint, F_POINT const& stitchPoint);
void filsat();
void filvrt();
void fliph();
void flipv();
void flpord();
void fltspac(uint32_t vertexOffset, uint32_t count);
void form();
void frm0();
void frmadj(uint32_t formIndex) noexcept(!std::is_same_v<ptrdiff_t, int>);
void frmlin(std::vector<F_POINT> const& vertices);
void frmnumfn(uint32_t& oldFormIndex, uint32_t newFormIndex);
void frmon();
void frmovlin();
auto frmrng(uint32_t iForm, RANGE& range) -> bool;
void frmsadj();
void fselrct(uint32_t iForm) noexcept(std::is_same_v<size_t, uint32_t>);
void fsizpar(FRM_HEAD& form) noexcept;
auto getAllItemsRect() noexcept -> F_RECTANGLE&;
auto getblen() noexcept -> float;
auto getlast(FRM_HEAD const& form) noexcept(!std::is_same_v<ptrdiff_t, int>) -> uint32_t;
auto getplen() noexcept -> float;
void horclp();
void horsclp();
void infrm();
void insat();
void ispcdclp();
void join();
auto lastch() noexcept -> bool;
auto linx(std::vector<F_POINT> const& points,
          uint32_t                    start,
          uint32_t                    finish,
          F_POINT&                    intersection,
          std::vector<F_POINT> const& outsidePoints) noexcept -> bool;
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
void rct2sel(RECT const& rectangle, std::vector<POINT>& line) noexcept;
void refil(uint32_t formIndex);
void refilal();
void refilfn(uint32_t formIndex);
void rinfrm();
void ritfrct(uint32_t iForm, HDC hDC);
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
void selsqr(POINT const& controlPoint, HDC hDC);
void setap();
void setexpand(float xyRatio);
void setfpnt();
void setfrm();
void sethup() noexcept;
void setins();
void setmfrm(uint32_t formIndex);
void setNewFormVertexCount(uint32_t vertexCount) noexcept;
void setrang();
void setstrtch();
auto sfCor2px(F_POINT const& stitchPoint) -> POINT;
void shrnk();
void snap();
void spltfrm();
void srtbyfrm();
void srtfrm();
void stchadj();
void stchrct2px(F_RECTANGLE const& stitchRect, RECT& screenRect);
void stchs2frm();
void tglfrm();
void unfil();
void unfrm();
void uninsf();
void unpsel();
void vrtclp();
void vrtsclp(uint32_t formIndex);

} // namespace form
