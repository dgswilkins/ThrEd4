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

void chnfn();
void clpbrd(unsigned int startVertex);
void clpic();
void clpout();
void delclps(unsigned int iForm);
void deleclp(unsigned int iForm);
void delmclp(unsigned int iForm);
void duxclp();
bool isclp(unsigned int iForm);
bool isclp(const FRMHED& form) noexcept;
bool isclpx(unsigned int iForm);
bool isclpx(const FRMHED& form) noexcept;
bool iseclp(unsigned int iForm);
bool iseclp(const FRMHED& form) noexcept;
bool iseclpx(unsigned int iForm);
bool iseclpx(const FRMHED& form) noexcept;

unsigned int nueclp(unsigned int currentForm, unsigned int count);
unsigned int numclp();

void oclp(unsigned int clipIndex, unsigned int clipEntries);

namespace internal {

	void clpcrnr(std::vector<fPOINT>& clipFillData, unsigned int vertex, const dPOINT& rotationCenter);
	bool clpsid(const std::vector<fPOINT>& clipReversedData,
	            std::vector<fPOINT>&       clipFillData,
	            unsigned int               start,
	            unsigned int               finish,
	            const dPOINT&              rotationCenter);
	void clpsub(unsigned int fpnt, unsigned int cnt);
	void clpxadj(std::vector<fPOINT>& tempClipPoints, std::vector<fPOINT>& chainEndPoints);
	void duch(std::vector<fPOINT>& chainEndPoints);
	void duchfn(const std::vector<fPOINT>& chainEndPoints, unsigned int start, unsigned int finish);
	void dufxlen(std::vector<fPOINT>& chainEndPoints);
	void dulast(std::vector<fPOINT>& chainEndPoints);
	void durev(std::vector<fPOINT>& clipReversedData);

	unsigned int findclp(unsigned int formIndex);
	void fxlen(std::vector<fPOINT>& chainEndPoints, const std::vector<double>& listSINEs, const std::vector<double>& listCOSINEs);
	void fxlin(std::vector<fPOINT>&       chainEndPoints,
	           const std::vector<double>& ListSINEs,
	           const std::vector<double>& ListCOSINEs,
	           dPOINT&                    moveToCoords,
	           unsigned                   currentSide);
	void fxlit(const std::vector<double>& listSINEs,
	           const std::vector<double>& listCOSINEs,
	           dPOINT&                    moveToCoords,
	           unsigned                   currentSide);
	bool fxpnt(const std::vector<double>& listSINEs,
	           const std::vector<double>& listCOSINEs,
	           dPOINT&                    moveToCoords,
	           unsigned                   currentSide);
	void lincrnr(const std::vector<fPOINT>& clipReversedData,
	             std::vector<fPOINT>&       clipFillData,
	             double                     clipAngle,
	             dPOINT&                    moveToCoords,
	             const dPOINT&              rotationCenter,
	             unsigned int               currentSide);
	void linsid(const std::vector<fPOINT>& clipReversedData,
	            std::vector<fPOINT>&       clipFillData,
	            double                     clipAngle,
	            const dPOINT&              vector0,
	            const dPOINT&              rotationCenter,
	            unsigned int               currentSide);
	bool nupnt(double clipAngle, dPOINT& moveToCoords, unsigned int currentSide);
	void picfn(std::vector<fPOINT>& clipFillData,
	           unsigned int         start,
	           unsigned int         finish,
	           double               spacing,
	           const dPOINT&        rotationCenter);
	bool ritclp(const std::vector<fPOINT>& clipFillData, const fPOINT& point);
	void setvct(unsigned int start, unsigned int finish, double& clipAngle, dPOINT& vector0);
	void xclpfn(const std::vector<fPOINT>& tempClipPoints,
	            const std::vector<fPOINT>& chainEndPoints,
	            unsigned                   start,
	            unsigned                   finish,
	            const dPOINT&              rotationCenter);
} // namespace internal
} // namespace clip