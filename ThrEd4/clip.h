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

void chnfn();
void clpbrd(size_t startVertex);
void clpcrnr(std::vector<fPOINT>& clipFillData, size_t vertex, const dPOINT& rotationCenter);
void clpout();
void clpic();
bool clpsid(const std::vector<fPOINT>& clipReversedData,
            std::vector<fPOINT>&       clipFillData,
            size_t                     start,
            size_t                     finish,
            const dPOINT&              rotationCenter);
void clpsub(size_t fpnt, size_t cnt) noexcept;
void clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints);
void delclps(size_t iForm);
void deleclp(size_t iForm) noexcept;
void delmclp(size_t iForm);
void duch(std::vector<fPOINT>& chainEndPoints);
void duchfn(const std::vector<fPOINT>& chainEndPoints, size_t start, size_t finish);
void duxclp();
void dufxlen(std::vector<fPOINT>& chainEndPoints);
void dulast(std::vector<fPOINT>& chainEndPoints);
void durev(std::vector<fPOINT>& clipReversedData);

size_t  findclp(size_t formIndex) noexcept;
void    fxlen(std::vector<fPOINT>& chainEndPoints, const std::vector<double>& listSINEs, const std::vector<double>& listCOSINEs);
void    fxlin(std::vector<fPOINT>&       chainEndPoints,
              const std::vector<double>& ListSINEs,
              const std::vector<double>& ListCOSINEs,
              dPOINT&                    moveToCoords,
              unsigned                   currentSide);
void    fxlit(const std::vector<double>& listSINEs,
              const std::vector<double>& listCOSINEs,
              dPOINT&                    moveToCoords,
              unsigned                   currentSide);
bool    fxpnt(const std::vector<double>& listSINEs,
              const std::vector<double>& listCOSINEs,
              dPOINT&                    moveToCoords,
              unsigned                   currentSide);
bool    isclp(size_t iForm) noexcept;
bool    isclpx(size_t iForm) noexcept;
bool    iseclp(size_t iForm) noexcept;
bool    iseclpx(size_t iForm) noexcept;
void    lincrnr(const std::vector<fPOINT>& clipReversedData,
                std::vector<fPOINT>&       clipFillData,
                double                     clipAngle,
                dPOINT&                    moveToCoords,
                const dPOINT&              rotationCenter,
                size_t                     currentSide);
void    linsid(const std::vector<fPOINT>& clipReversedData,
               std::vector<fPOINT>&       clipFillData,
               double                     clipAngle,
               const dPOINT&              vector0,
               const dPOINT&              rotationCenter,
               size_t                     currentSide);
fPOINT* nueclp(size_t currentForm, size_t count) noexcept;
fPOINT* numclp() noexcept;
bool    nupnt(double clipAngle, dPOINT& moveToCoords, size_t currentSide) noexcept;
void    oclp(const fPOINT* const clip, size_t clipEntries);
void    picfn(std::vector<fPOINT>& clipFillData, size_t start, size_t finish, double spacing, const dPOINT& rotationCenter);
bool    ritclp(const std::vector<fPOINT>& clipFillData, const fPOINT& point);
void    setvct(size_t start, size_t finish, double& ClipAngle, dPOINT& vector0) noexcept;
void    xclpfn(const std::vector<fPOINT>& tempClipPoints,
               const std::vector<fPOINT>& chainEndPoints,
               unsigned                   start,
               unsigned                   finish,
               const dPOINT&              rotationCenter);
