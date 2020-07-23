#pragma once

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable : 4244)
// Suppress C6031: return value ignored
#pragma warning(disable : 6031)

// clang-format off
// C++ Code Checker rules - disable them as required
#pragma warning(disable : 26400)    //		    	Do not assign the result of an allocation or a function call with an owner<T>
#pragma warning(disable : 26401)    //	i.11    	Do not delete a raw pointer that is not an owner<T>
#pragma warning(disable : 26409)    //		    	Avoid calling new and delete explicitly, use std::make_unique<T> instead
#pragma warning(disable : 26423)    //	    		The allocation was not directly assigned to an owner
#pragma warning(disable : 26424)    //	    		Failing to delete or assign ownership of allocation
//#pragma warning(disable : 26425)  //	    		Assigning 'return of new[]' to a static variable
//#pragma warning(disable:26429)	// f.23 		Symbol is never tested for nullness, it can be marked as not_null
//#pragma warning(disable:26438)	// es.76    	Avoid 'goto'

// We disable 26446 as operator[] is bounds checked for vectors in the debug version
#pragma warning(disable : 26446)    // bounds.4     Prefer to use gsl::at() instead of unchecked subscript operator

//#pragma warning(disable : 26451)  // io.2  		Arithmetic overflow
//#pragma warning(disable:26461)	// con.3    	The pointer argument can be marked as a pointer to const
//#pragma warning(disable:26472)	// type.1   	Don't use a gsl::narrow_cast for arithmetic conversions
//#pragma warning(disable:26474)	// type.1   	Don't cast between pointer types when the conversion could be implicit
#pragma warning(disable : 26476)	// type.7   	Don't use a naked union 'union ' with multiple type pointers: Use variant instead
#pragma warning(disable : 26481)    // bounds.1 	Don't use pointer arithmetic. Use span instead
#pragma warning(disable : 26482)    // bounds.2 	Only index into arrays using constant expressions
//#pragma warning(disable : 26485)  // bounds.3 	No array to pointer decay
#pragma warning(disable : 26486)    // lifetime.1	Don't pass a pointer that may be invalid to a function
#pragma warning(disable : 26489)    // lifetime.1	Don't dereference a pointer that may be invalid
//#pragma warning(disable:26490)	// type.1   	Don't use reinterpret_cast
//#pragma warning(disable:26493)	// type.4   	Don't use C-style casts
//#pragma warning(disable:26494)	// type.5   	Variable is uninitialized
//#pragma warning(disable:26496)	// Con.4    	Value is assigned only once, mark it as a pointer to const
//#pragma warning(disable:26497)	// f.4  		This function could be marked constexpr if compile-time evaluation is desired
#pragma warning(disable : 26499)    //			    Could not find any lifetime tracking information
// clang-format on

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

// Windows Header Files:
#include <Windows.h> // Win32 Platform SDK main header

// Standard Libraries
#include <filesystem>

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#pragma warning(disable : 26814)
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"
#include "EnumMap.h"

namespace fs = std::filesystem;

extern uint32_t    ActiveColor;
extern uint8_t     ActiveLayer;
extern fRECTANGLE* AllItemsRect;

extern std::vector<fPOINT>* AngledFormVertices;

extern uint32_t  AppliqueColor;
extern LPTSTR*   ArgList; // command line argument array
extern HCURSOR   ArrowCursor;
extern fs::path* AuxName;

extern std::vector<BSEQPNT>* BSequence;

extern HBRUSH   BackgroundBrush;
extern COLORREF BackgroundColor;
extern uint32_t BeanCount; // number of stitches added by convert to bean
extern float    BorderWidth;
extern int32_t  ButtonHeight;
extern int32_t  ButtonWidth;
extern int32_t  ButtonWidthX3;

extern std::vector<HWND>* ButtonWin;

extern float    ButtonholeCornerLength;
extern HWND     CancelButton;
extern uint32_t Clip;

extern std::vector<fPOINTATTR>* ClipBuffer;

extern HGLOBAL ClipMemory;
extern void*   ClipPointer;

extern std::vector<fPOINT>* ClipPoints;

extern FLSIZ    ClipRectSize;
extern CLPSTCH* ClipStitchData;
extern uint32_t ClipTypeMap;
extern uint32_t ClosestFormToCursor;
extern uint32_t ClosestPointIndex;
extern uint32_t ClosestVertexToCursor;
extern int32_t* ColorBarSize;
extern HCURSOR  CrossCursor;

extern std::vector<HWND>* DefaultColorWin;

extern HWND DeleteStitchesDialog;

extern std::wstring*   DesignerName;
extern HWND            DiscardButton;
extern DRAWITEMSTRUCT* DrawItem;
extern POINT           EndPointCross;
extern STREX*          ExtendedHeader;
extern HMENU           FillMenu;

extern std::vector<float>* FormAngles;
extern std::vector<POINT>* FormControlPoints;

extern HWND FormDataSheet;

extern std::vector<POINT>*  FormLines;
extern std::vector<FRMHED>* FormList;

extern uint32_t FormMenuChoice;
extern fPOINT   FormMoveDelta;

extern std::wstring* FormOnOff;

extern HPEN FormPen3px;
extern HPEN FormPen;
extern HPEN FormSelectedPen;

extern std::vector<fPOINT>* FormVertices;

extern HWND      GeneralNumberInputBox;
extern HPEN      GridPen;
extern uint32_t  GroupEndStitch;
extern uint32_t  GroupStartStitch;
extern uint32_t  GroupStitchIndex;
extern fs::path* HomeDirectory;
extern double    HorizontalLength;
extern float     HorizontalRatio;
extern HWND      HorizontalScrollBar;
extern INIFILE   IniFile;

extern std::array<POINT, 3> InsertLine;
extern std::vector<fPOINT>* InsidePointList;
extern std::vector<fPOINT>* InsidePoints;
extern std::vector<INSREC>* InterleaveSequenceIndices;
extern std::vector<fPOINT>* InterleaveSequence;
extern std::vector<HWND>*   LabelWindow;

extern fPOINT LastPoint;
extern HPEN   LayerPen[6];
extern float  LineSpacing;
extern fPOINT LowerLeftStitch;
extern HMENU  MainMenu;
extern HWND   MainStitchWin;
extern float  MaxStitchLen;

extern MENUITEMINFO* MenuInfo;

extern float    MinStitchLength;
extern MSG      Msg;
extern wchar_t  MsgBuffer[MSGSIZ];
extern uint32_t MsgIndex;
extern RECT     MsgRect;
extern HWND     MsgWindow;
extern HPEN     MultiFormPen;
extern uint32_t NearestCount;
extern uint32_t NewFormVertexCount;
extern HWND     OKButton;

extern std::vector<fPOINT>* OSequence;

extern bool OutLineEverySelectedForm;

extern std::vector<fPOINT>* OutsidePointList;
extern std::vector<fPOINT>* OutsidePoints;

extern char           PCSBMPFileName[16];
extern PCSHEADER      PCSHeader;
extern wchar_t const* PcdClipFormat;
extern float          PicotSpacing;
extern uint32_t       PreferenceIndex;
extern int32_t        PreferenceWindowWidth;
extern HWND           PreferencesWindow;
extern uint32_t       PreviousFormIndex;
extern uint32_t       PseudoRandomValue;
extern fRECTANGLE     RotationRect;

extern std::vector<POINT>* RubberBandLine;

extern std::vector<SATCON>* SatinGuides;

extern RECT      scRct;
extern uint32_t* screenDPI;
extern int32_t*  ScrollSize;

extern std::vector<POINT>* SearchLine;

extern HPEN     SelectAllPen;
extern uint32_t SelectedFormControlVertex;

extern std::vector<uint32_t>* SelectedFormList;
extern FORMVERTICES           SelectedFormVertices;
extern std::vector<POINT>*    SelectedFormsLine;

extern RECT   SelectedFormsRect;
extern fPOINT SelectedFormsSize;
extern RECT   SelectedPixelsRect;

extern std::vector<POINT>* SelectedPointsLine;

extern fRECTANGLE SelectedVerticesRect;
extern double     ShowStitchThreshold;
extern HWND       SideMessageWindow;

extern std::array<wchar_t, SWBLEN> SideWindowEntryBuffer;

extern HWND  SideWindow[16];
extern float SmallStitchLength;
extern float SnapLength;
extern float SpiralWrap;
extern float StarRatio;

extern EnumMap<StateFlag>* StateMap;

extern fPOINT StitchBmpRatio;
extern double StitchBoxesThreshold;

extern std::vector<fPOINTATTR>* StitchBuffer;

extern fRECTANGLE StitchRangeRect;
extern RECT       StitchWindowClientRect;
extern HDC        StitchWindowDC;
extern HDC        StitchWindowMemDC;
extern POINT      StitchWindowOrigin;
extern POINT      StitchWindowSize;

extern std::vector<std::wstring>* StringTable;

extern std::vector<fPOINT>* TempPolygon;
extern std::wstring*        TextureInputBuffer;
extern std::vector<TXPNT>*  TexturePointsBuffer;
extern HWND                 ThreadSizeWin[COLOR_COUNT];
extern uint32_t             ThrEdClip;
extern wchar_t const*       ThrEdClipFormat;
extern HINSTANCE            ThrEdInstance;
extern HWND                 ThrEdWindow;
extern fs::path*            ThrName;
extern POINT                ThredWindowOrigin;
extern HBITMAP              TraceBitmap;
extern uint32_t*            TraceBitmapData;
extern HDC                  TraceDC;

extern boost::dynamic_bitset<>* TracedEdges;
extern boost::dynamic_bitset<>* TracedMap;

extern POINT UnzoomedRect;

extern std::array<COLORREF, COLOR_COUNT> UserColor;

extern std::vector<HWND>* UserColorWin;
extern EnumMap<UserFlag>* UserFlagMap;

extern HPEN  UserPen[COLOR_COUNT];
extern float UserStitchLength;

extern std::vector<HWND>* ValueWindow;

extern float      VerticalRatio;
extern HWND       VerticalScrollBar;
extern fs::path*  WorkingFileName;

extern std::array<POINT, SQPNTS> ZoomBoxLine;

extern fPOINT     ZoomBoxOrigin;
extern float      ZoomFactor;
extern fPOINT     ZoomMarkPoint;
extern fPOINT     ZoomRatio;
extern fRECTANGLE ZoomRect;
