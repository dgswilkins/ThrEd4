#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:

// Standard Libraries
#include <vector>

// Open Source headers

// Local Headers
#include "ThrEdTypes.h"

namespace clip {

void chnfn(FRM_HEAD const& form);
void clpbrd(FRM_HEAD const& form, F_RECTANGLE const& clipRect, uint32_t startVertex);
void clpic(FRM_HEAD const& form, F_RECTANGLE const& clipRect);
void clpout(float width);
void delclps(uint32_t formIndex);
void deleclp(uint32_t formIndex);
void delmclp(uint32_t formIndex);
void duxclp(FRM_HEAD const& form);
auto isclp(FRM_HEAD const& form) noexcept -> bool;
auto isclpx(FRM_HEAD const& form) noexcept -> bool;
auto iseclp(FRM_HEAD const& form) noexcept -> bool;
auto iseclpx(FRM_HEAD const& form) noexcept -> bool;
auto nueclp(uint32_t currentForm, uint32_t count) -> uint32_t;
auto numclp() -> uint32_t;
void oclp(F_RECTANGLE& clipRect, uint32_t clipIndex, uint32_t clipEntries);

namespace internal {

  void clpcrnr(FRM_HEAD const&       form,
               F_RECTANGLE const&    clipRect,
               std::vector<F_POINT>& clipFillData,
               uint32_t              vertex,
               F_POINT const&        rotationCenter);
  auto clpsid(uint32_t                    vertexIndex,
              F_RECTANGLE const&          clipRect,
              std::vector<F_POINT> const& clipReversedData,
              std::vector<F_POINT>&       clipFillData,
              uint32_t                    start,
              uint32_t                    finish,
              F_POINT const&              rotationCenter) -> bool;
  void clpsub(uint32_t fpnt, uint32_t cnt);
  void clpxadj(std::vector<F_POINT>& tempClipPoints, std::vector<F_POINT>& chainEndPoints);
  void duch(std::vector<F_POINT> const& chainEndPoints);
  void duchfn(std::vector<F_POINT> const& chainEndPoints, uint32_t start, uint32_t finish);
  void dufxlen(FRM_HEAD const& form, std::vector<F_POINT>& chainEndPoints);
  void dulast(std::vector<F_POINT>& chainEndPoints);
  void durev(F_RECTANGLE const& clipRect, std::vector<F_POINT>& clipReversedData) noexcept;
  auto findclp(uint32_t formIndex) noexcept -> uint32_t;
  void fxlen(FRM_HEAD const&           form,
             std::vector<F_POINT>&     chainEndPoints,
             std::vector<float> const& listSINEs,
             std::vector<float> const& listCOSINEs);
  void fxlin(uint32_t                  vertexIndex,
             std::vector<F_POINT>&     chainEndPoints,
             std::vector<float> const& ListSINEs,
             std::vector<float> const& ListCOSINEs,
             F_POINT&                  moveToCoords,
             uint32_t                  currentSide,
             F_POINT&                  stitchPoint,
             float                     adjustedSpace,
             uint32_t                  nextStart);
  void fxlit(uint32_t                  vertexIndex,
             std::vector<float> const& listSINEs,
             std::vector<float> const& listCOSINEs,
             F_POINT&                  moveToCoords,
             uint32_t                  currentSide,
             F_POINT&                  stitchPoint,
             uint32_t&                 adjCount,
             float                     adjustedSpace,
             uint32_t                  nextStart);
  auto fxpnt(uint32_t                  vertexIndex,
             std::vector<float> const& listSINEs,
             std::vector<float> const& listCOSINEs,
             F_POINT&                  moveToCoords,
             uint32_t                  currentSide,
             F_POINT const&            stitchPoint,
             float                     adjustedSpace,
             uint32_t                  nextStart) -> bool;
  void lincrnr(uint32_t                    vertexIndex,
               std::vector<F_POINT> const& clipReversedData,
               std::vector<F_POINT>&       clipFillData,
               float                       clipAngle,
               F_POINT const&              rotationCenter,
               uint32_t                    currentSide,
               F_POINT&                    stitchPoint,
               F_POINT const&              borderClipReference);
  void linsid(uint32_t                    vertexIndex,
              std::vector<F_POINT> const& clipReversedData,
              std::vector<F_POINT>&       clipFillData,
              float                       clipAngle,
              F_POINT const&              vector0,
              F_POINT const&              rotationCenter,
              uint32_t                    currentSide,
              F_POINT&                    stitchPoint,
              F_POINT const&              borderClipReference);
  auto nupnt(float clipAngle, F_POINT& moveToCoords, F_POINT const& stitchPoint) noexcept -> bool;
  void picfn(FRM_HEAD const&       form,
             F_RECTANGLE const&    clipRect,
             std::vector<F_POINT>& clipFillData,
             uint32_t              start,
             uint32_t              finish,
             float                 spacing,
             F_POINT const&        rotationCenter);
  auto ritclp(std::vector<F_POINT> const& clipFillData, F_POINT const& point) -> bool;
  void setvct(uint32_t vertexIndex, uint32_t start, uint32_t finish, float& clipAngle, F_POINT& vector0);
  void xclpfn(std::vector<F_POINT> const& tempClipPoints,
              std::vector<F_POINT> const& chainEndPoints,
              uint32_t                    start,
              uint32_t                    finish,
              F_POINT const&              rotationCenter);
} // namespace internal
} // namespace clip
