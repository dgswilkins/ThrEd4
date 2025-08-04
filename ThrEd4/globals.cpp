// Local Headers
#include "stdafx.h"
#include "fRectangle.h"
#include "globals.h"
#include "iniFile.h"
#include "point.h"
#include "ThrEdTypes.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <minwindef.h>
#include <windef.h>
#include <WinUser.h>

// Standard Libraries
#include <array>
#include <cstdint>
#include <cstdlib>
#include <exception>
#include <new>
#include <vector>

// clang-format off
// ReSharper disable CppUseAuto
MY_SINGLE*            Instance                  = nullptr;           // main instance
uint8_t               ActiveColor               = 0U;                // active color selector
uint8_t               ActiveLayer               = 0U;                // active layer
uint8_t               AppliqueColor             = COLORCNT - 1U;     // underlay color
int32_t               ButtonHeight              = 0;                 // button height
int32_t               ButtonWidth               = 0;                 // button width
int32_t               ButtonWidthX3             = 0;                 // button width times 3
F_LSIZ                ClipRectSize              = {};                // clipboard rectangle size
uint32_t              ClosestFormToCursor       = 0U;                // closest form to the cursor
uint32_t              ClosestPointIndex         = 0U;                // index of closest point
uint32_t              ClosestVertexToCursor     = 0U;                // formOrigin closest to the cursor
POINT                 EndPointCross             = {};                // point to draw at the end cross for form select points
F_POINT               FormMoveDelta             = F_POINT {};        // offset for moving forms
HPEN                  FormPen                   = nullptr;           // form pen
HPEN                  FormPen3px                = nullptr;           // three-pixel form pen
uint32_t              GroupEndStitch            = 0U;                // higher end of selected stitches
uint32_t              GroupStitchIndex          = 0U;                // last point selected in group
uint32_t              GroupStartStitch          = 0U;                // lower end of selected stitches
INI_FILE              IniFile                   = {};                // initialization file
std::array<POINT, 3>  InsertLine                = {};                // the insert line
std::vector<F_POINT>* InsidePoints              = nullptr;           // pointer to the list of inside outline points
F_POINT               LastPoint                 = F_POINT {};        // last formOrigin written by line connect routine
float                 LineSpacing               = DEFSPACE * PFGRAN; // stitch spacing in stitch units
HWND                  MainStitchWin             = nullptr;           // stitch window
float                 MinStitchLength           = MINSIZ * PFGRAN;   // minimum stitch size
MSG                   WinMsg                    = {};                // main message loop message
HWND                  MsgWindow                 = nullptr;           // message window
std::vector<F_POINT>* OutsidePoints             = nullptr;           // pointer to the list of outside outline points
uint32_t              PseudoRandomValue         = 0U;                // pseudo-random sequence register
F_RECTANGLE           RotationRect              = F_RECTANGLE {};    // rotation rectangle
uint32_t              SelectedFormControlVertex = 0U;                // user selected form control formOrigin
FORM_VERTICES         SelectedFormVertices      = {};                // selected form vertices
RECT                  SelectedFormsRect         = {};                // for multiple selections;
F_POINT               SelectedFormsSize         = F_POINT {};        // size of multiple select rectangle
RECT                  SelectedPixelsRect        = {};                // display form vertex select rectangle
F_RECTANGLE           SelectedVerticesRect      = F_RECTANGLE {};    // rectangle enclosing selected form verticess
F_RECTANGLE           StitchRangeRect           = F_RECTANGLE {};    // stitch range rectangle
RECT                  StitchWindowClientRect    = {};                // stitch window size,client
HDC                   StitchWindowDC            = nullptr;           // stitch window device context
HDC                   StitchWindowMemDC         = nullptr;           // stitch window memory device context
POINT                 StitchWindowOrigin        = {};                // offset origin of the stitch window
HINSTANCE             ThrEdInstance             = {};                // main instance handle
HWND                  ThrEdWindow               = nullptr;           // main window handle
POINT                 ThredWindowOrigin         = {};                // offset origin of the main window
uint32_t*             TraceBitmapData           = nullptr;           // trace bitmap data

SIZE                           UnzoomedRect     = {};                // size of the unzoomed stitch window
std::array<COLORREF, COLORCNT> UserColor        = {};                // user colors
float                          UserStitchLength = USESIZ * PFGRAN;   // user selected stitch size
std::array<POINT, SQPNTS>      ZoomBoxLine      = {};                // the zoom box
F_POINT                        ZoomBoxOrigin    = F_POINT {};        // zoom box origin
float                          ZoomFactor       = 1.0F;              // zoom factor
F_POINT                        ZoomRatio        = F_POINT {};        // zoom ratio used to draw stitch window
F_RECTANGLE                    ZoomRect         = F_RECTANGLE {};    // zoom rectangle
// ReSharper restore CppUseAuto
// clang-format on

auto globals::globalsInit() -> uint32_t {
  Instance = MY_SINGLE::getInstance();

  try {
	Instance->initialize();
  }
  catch (std::bad_alloc const&) {
	outDebugString(L"Memory allocation failure in Instance\n");
	return EXIT_FAILURE;
  }
  catch (std::exception const& e) {
	outDebugString(L"Exception caught in Instance: {}\n", static_cast<const void*>(e.what()));
	return EXIT_FAILURE;
  }
  catch (...) {
	outDebugString(L"Unknown exception caught in Instance\n");
	return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
