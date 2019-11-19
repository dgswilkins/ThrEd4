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

namespace clip {

void chnfn(const FRMHED& form);
void clpbrd(const FRMHED& form, const fRECTANGLE& clipRect, uint32_t startVertex);
void clpic(const FRMHED& form, const fRECTANGLE& clipRect);
void clpout(float width);
void delclps(uint32_t formIndex);
void deleclp(uint32_t formIndex);
void delmclp(uint32_t formIndex);
void duxclp(const FRMHED& form);
auto isclp(const FRMHED& form) noexcept -> bool;
auto isclpx(uint32_t iForm) noexcept -> bool;
auto isclpx(const FRMHED& form) noexcept -> bool;
auto iseclp(uint32_t iForm) noexcept -> bool;
auto iseclp(const FRMHED& form) noexcept -> bool;
auto iseclpx(uint32_t iForm) noexcept -> bool;
auto iseclpx(const FRMHED& form) noexcept -> bool;
auto nueclp(uint32_t currentForm, uint32_t count) -> uint32_t;
auto numclp() -> uint32_t;
void oclp(fRECTANGLE& clipRect, uint32_t clipIndex, uint32_t clipEntries);

namespace internal {

	void clpcrnr(uint32_t             vertexIndex,
	             const fRECTANGLE&    clipRect,
	             std::vector<fPOINT>& clipFillData,
	             uint32_t             vertex,
	             const fPOINT&        rotationCenter);
	auto clpsid(uint32_t                   vertexIndex,
	            const fRECTANGLE&          clipRect,
	            const std::vector<fPOINT>& clipReversedData,
	            std::vector<fPOINT>&       clipFillData,
	            uint32_t                   start,
	            uint32_t                   finish,
	            const fPOINT&              rotationCenter) -> bool;
	void clpsub(uint32_t fpnt, uint32_t cnt);
	void clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints);
	void duch(std::vector<fPOINT>& chainEndPoints);
	void duchfn(const std::vector<fPOINT>& chainEndPoints, uint32_t start, uint32_t finish);
	void dufxlen(const FRMHED& form, std::vector<fPOINT>& chainEndPoints);
	void dulast(std::vector<fPOINT>& chainEndPoints);
	void durev(const fRECTANGLE& clipRect, std::vector<fPOINT>& clipReversedData);
	auto findclp(uint32_t formIndex) noexcept -> uint32_t;
	void fxlen(CONST FRMHED&             form,
	           std::vector<fPOINT>&      chainEndPoints,
	           const std::vector<float>& listSINEs,
	           const std::vector<float>& listCOSINEs);
	void fxlin(uint32_t                  vertexIndex,
	           std::vector<fPOINT>&      chainEndPoints,
	           const std::vector<float>& ListSINEs,
	           const std::vector<float>& ListCOSINEs,
	           fPOINT&                   moveToCoords,
	           uint32_t                  currentSide);
	void fxlit(uint32_t                  vertexIndex,
	           const std::vector<float>& listSINEs,
	           const std::vector<float>& listCOSINEs,
	           fPOINT&                   moveToCoords,
	           uint32_t                  currentSide);
	auto fxpnt(uint32_t                  vertexIndex,
	           const std::vector<float>& listSINEs,
	           const std::vector<float>& listCOSINEs,
	           fPOINT&                   moveToCoords,
	           uint32_t                  currentSide) -> bool;
	void lincrnr(uint32_t                   vertexIndex,
	             const std::vector<fPOINT>& clipReversedData,
	             std::vector<fPOINT>&       clipFillData,
	             float                      clipAngle,
	             const fPOINT&              rotationCenter,
	             uint32_t                   currentSide);
	void linsid(uint32_t                   vertexIndex,
	            const std::vector<fPOINT>& clipReversedData,
	            std::vector<fPOINT>&       clipFillData,
	            float                      clipAngle,
	            const fPOINT&              vector0,
	            const fPOINT&              rotationCenter,
	            uint32_t                   currentSide);
	auto nupnt(float clipAngle, fPOINT& moveToCoords) noexcept -> bool;
	void picfn(const FRMHED&        form,
	           const fRECTANGLE&    clipRect,
	           std::vector<fPOINT>& clipFillData,
	           uint32_t             start,
	           uint32_t             finish,
	           float                spacing,
	           const fPOINT&        rotationCenter);
	auto ritclp(const std::vector<fPOINT>& clipFillData, const fPOINT& point) -> bool;
	void setvct(uint32_t vertexIndex, uint32_t start, uint32_t finish, float& clipAngle, fPOINT& vector0);
	void xclpfn(const std::vector<fPOINT>& tempClipPoints,
	            const std::vector<fPOINT>& chainEndPoints,
	            uint32_t                   start,
	            uint32_t                   finish,
	            const fPOINT&              rotationCenter);
} // namespace internal
} // namespace clip
