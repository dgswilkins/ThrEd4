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
#include <vector>

// clang-format off
// ReSharper disable CppUseAuto
MY_SINGLE*                     Instance                  = nullptr; // main instance
uint8_t                        ActiveColor               = 0U;      // active color selector
uint8_t                        ActiveLayer               = 0U;      // active layer
uint8_t                        AppliqueColor             = COLORCNT - 1U; // underlay color
COLORREF                       BackgroundColor           = 0U;      // stitch window background
float                          BorderWidth               = BRDWID;  // border width for satin borders
int32_t                        ButtonHeight              = 0;       // button height
int32_t                        ButtonWidth               = 0;       // button width
int32_t                        ButtonWidthX3             = 0;       // button width times 3
float                          ButtonholeCornerLength    = IBFCLEN; // buttonhole corner length
uint32_t                       Clip                      = 0U;      // pcs format clipboard handle
HGLOBAL                        ClipMemory                = nullptr; // handle to the clipboard memory
F_LSIZ                         ClipRectSize              = {};      // clipboard rectangle size
uint32_t                       ClosestFormToCursor       = 0U;      // closest form to the cursor
uint32_t                       ClosestPointIndex         = 0U;      // index of closest point
uint32_t                       ClosestVertexToCursor     = 0U;      // formOrigin closest to the cursor
HWND                           DeleteStitchesDialog      = nullptr; // asking user if they want to delete the stitches associated with a form
DRAWITEMSTRUCT*                DrawItem                  = nullptr; // for owner-draw windows
POINT                          EndPointCross             = {};      // point to draw at the end cross for form select points
HWND                           FormDataSheet             = nullptr; // form data sheet
uint32_t                       FormMenuChoice            = 0U;      // data type for form data form numerical entry
F_POINT                        FormMoveDelta             = F_POINT {}; // offset for moving forms
HPEN                           FormPen                   = nullptr; // form pen
HPEN                           FormPen3px                = nullptr; // three-pixel form pen
uint32_t                       GroupEndStitch            = 0U;      // higher end of selected stitches
uint32_t                       GroupStartStitch          = 0U;      // lower end of selected stitches
uint32_t                       GroupStitchIndex          = 0U;      // last point selected in group
INI_FILE                       IniFile                   = {};      // initialization file
std::array<POINT, 3>           InsertLine                = {};      // the insert line
std::vector<F_POINT>*          InsidePoints              = nullptr; // pointer to the list of inside outline points
F_POINT                        LastPoint                 = F_POINT {}; // last formOrigin written by line connect routine
float                          LineSpacing               = DEFSPACE * PFGRAN; // stitch spacing in stitch units
HWND                           MainStitchWin             = nullptr; // stitch window
float                          MinStitchLength           = MINSIZ * PFGRAN; // minimum stitch size
MSG                            WinMsg                    = {};      // main message loop message
RECT                           MsgRect                   = {};      // rectangle containing the text message
HWND                           MsgWindow                 = nullptr; // message window
uint32_t                       NewFormVertexCount        = 0U;      // points in the new form
std::vector<F_POINT>*          OutsidePoints             = nullptr; // pointer to the list of outside outline points
float                          PicotSpacing              = IPICSPAC;// space between border picots
uint32_t                       PseudoRandomValue         = 0U;      // pseudo-random sequence register
F_RECTANGLE                    RotationRect              = F_RECTANGLE {}; // rotation rectangle
uint32_t                       SelectedFormControlVertex = 0U;      // user selected form control formOrigin
FORM_VERTICES                  SelectedFormVertices      = {};      // selected form vertices
RECT                           SelectedFormsRect         = {};      // for multiple selections;
F_POINT                        SelectedFormsSize         = F_POINT {}; // size of multiple select rectangle
RECT                           SelectedPixelsRect        = {};      // display form vertex select rectangle
F_RECTANGLE                    SelectedVerticesRect      = F_RECTANGLE {}; // rectangle enclosing selected form verticess
HWND                           SideMessageWindow         = nullptr; // main side message window
float                          SnapLength                = SNPLEN * PFGRAN;  // snap together length
float                          SpiralWrap                = SPIRWRAP;// number of revolutions in a spiral
float                          StarRatio                 = STARAT;  // star formOrigin to body ratio
F_POINT                        StitchBmpRatio            = F_POINT {}; // stitch hoop to bitmap ratios
F_RECTANGLE                    StitchRangeRect           = F_RECTANGLE {}; // stitch range rectangle
RECT                           StitchWindowClientRect    = {};      // stitch window size,client
HDC                            StitchWindowDC            = nullptr; // stitch window device context
HDC                            StitchWindowMemDC         = nullptr; // stitch window memory device context
POINT                          StitchWindowOrigin        = {};      // offset origin of the stitch window
HINSTANCE                      ThrEdInstance             = {};      // main instance handle
HWND                           ThrEdWindow               = nullptr; // main window handle
POINT                          ThredWindowOrigin         = {};      // offset origin of the main window
uint32_t*                      TraceBitmapData           = nullptr; // trace bitmap data

SIZE                           UnzoomedRect              = {};      // size of the unzoomed stitch window
std::array<COLORREF, COLORCNT> UserColor                 = {};      // user colors
float                          UserStitchLength          = USESIZ * PFGRAN; // user selected stitch size
std::array<POINT, SQPNTS>      ZoomBoxLine               = {};      // the zoom box
F_POINT                        ZoomBoxOrigin             = F_POINT {}; // zoom box origin
float                          ZoomFactor                = 1.0F;    // zoom factor
F_POINT                        ZoomMarkPoint             = F_POINT {}; // stitch coordinates of the zoom mark
F_POINT                        ZoomRatio                 = F_POINT {}; // zoom ratio used to draw stitch window
F_RECTANGLE                    ZoomRect                  = F_RECTANGLE {}; // zoom rectangle
// ReSharper restore CppUseAuto
// clang-format on
