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

void chnfn(FRMHED const& form);
void clpbrd(FRMHED const& form, fRECTANGLE const& clipRect, uint32_t startVertex);
void clpic(FRMHED const& form, fRECTANGLE const& clipRect);
void clpout(float width);
void delclps(uint32_t formIndex);
void deleclp(uint32_t formIndex);
void delmclp(uint32_t formIndex);
void duxclp(FRMHED const& form);
auto isclp(FRMHED const& form) noexcept -> bool;
auto isclpx(uint32_t iForm) noexcept -> bool;
auto isclpx(FRMHED const& form) noexcept -> bool;
auto iseclp(uint32_t iForm) noexcept -> bool;
auto iseclp(FRMHED const& form) noexcept -> bool;
auto iseclpx(uint32_t iForm) noexcept -> bool;
auto iseclpx(FRMHED const& form) noexcept -> bool;
auto nueclp(uint32_t currentForm, uint32_t count) -> uint32_t;
auto numclp() -> uint32_t;
void oclp(fRECTANGLE& clipRect, uint32_t clipIndex, uint32_t clipEntries);

namespace internal {

  void clpcrnr(FRMHED const&        form,
               fRECTANGLE const&    clipRect,
               std::vector<fPOINT>& clipFillData,
               uint32_t             vertex,
               fPOINT const&        rotationCenter);
  auto clpsid(uint32_t                   vertexIndex,
              fRECTANGLE const&          clipRect,
              std::vector<fPOINT> const& clipReversedData,
              std::vector<fPOINT>&       clipFillData,
              uint32_t                   start,
              uint32_t                   finish,
              fPOINT const&              rotationCenter) -> bool;
  void clpsub(uint32_t fpnt, uint32_t cnt);
  void clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints);
  void duch(std::vector<fPOINT>& chainEndPoints);
  void duchfn(std::vector<fPOINT> const& chainEndPoints, uint32_t start, uint32_t finish);
  void dufxlen(FRMHED const& form, std::vector<fPOINT>& chainEndPoints);
  void dulast(std::vector<fPOINT>& chainEndPoints);
  void durev(fRECTANGLE const& clipRect, std::vector<fPOINT>& clipReversedData);
  auto findclp(uint32_t formIndex) noexcept -> uint32_t;
  void fxlen(FRMHED const&             form,
             std::vector<fPOINT>&      chainEndPoints,
             std::vector<float> const& listSINEs,
             std::vector<float> const& listCOSINEs);
  void fxlin(uint32_t                  vertexIndex,
             std::vector<fPOINT>&      chainEndPoints,
             std::vector<float> const& ListSINEs,
             std::vector<float> const& ListCOSINEs,
             fPOINT&                   moveToCoords,
             uint32_t                  currentSide,
             fPOINT&                   stitchPoint,
             float                     adjustedSpace,
             uint32_t                  nextStart);
  void fxlit(uint32_t                  vertexIndex,
             std::vector<float> const& listSINEs,
             std::vector<float> const& listCOSINEs,
             fPOINT&                   moveToCoords,
             uint32_t                  currentSide,
             fPOINT&                   stitchPoint,
             uint32_t&                 adjCount,
             float                     adjustedSpace,
             uint32_t                  nextStart);
  auto fxpnt(uint32_t                  vertexIndex,
             std::vector<float> const& listSINEs,
             std::vector<float> const& listCOSINEs,
             fPOINT&                   moveToCoords,
             uint32_t                  currentSide,
             fPOINT const&             stitchPoint,
             float                     adjustedSpace,
             uint32_t                  nextStart) -> bool;
  void lincrnr(uint32_t                   vertexIndex,
               std::vector<fPOINT> const& clipReversedData,
               std::vector<fPOINT>&       clipFillData,
               float                      clipAngle,
               fPOINT const&              rotationCenter,
               uint32_t                   currentSide,
               fPOINT&                    stitchPoint,
               fPOINT const&              borderClipReference);
  void linsid(uint32_t                   vertexIndex,
              std::vector<fPOINT> const& clipReversedData,
              std::vector<fPOINT>&       clipFillData,
              float                      clipAngle,
              fPOINT const&              vector0,
              fPOINT const&              rotationCenter,
              uint32_t                   currentSide,
              fPOINT&                    stitchPoint,
              fPOINT const&              borderClipReference);
  auto nupnt(float clipAngle, fPOINT& moveToCoords, fPOINT const& stitchPoint) noexcept -> bool;
  void picfn(FRMHED const&        form,
             fRECTANGLE const&    clipRect,
             std::vector<fPOINT>& clipFillData,
             uint32_t             start,
             uint32_t             finish,
             float                spacing,
             fPOINT const&        rotationCenter);
  auto ritclp(std::vector<fPOINT> const& clipFillData, fPOINT const& point) -> bool;
  void setvct(uint32_t vertexIndex, uint32_t start, uint32_t finish, float& clipAngle, fPOINT& vector0);
  void xclpfn(std::vector<fPOINT> const& tempClipPoints,
              std::vector<fPOINT> const& chainEndPoints,
              uint32_t                   start,
              uint32_t                   finish,
              fPOINT const&              rotationCenter);
} // namespace internal
} // namespace clip
