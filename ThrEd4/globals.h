#pragma once

// Suppress C4244: conversion from 'type1' to 'type2', possible loss of data
#pragma warning(disable : 4244)
// Suppress C6031: return value ignored
#pragma warning(disable : 6031)

// C++ Code Checker rules - disable them as required
#pragma warning(disable : 26400) //			Do not assign the result of an allocation or a function call with an owner<T>
#pragma warning(disable : 26401) //	i.11	Do not delete a raw pointer that is not an owner<T>
#pragma warning(disable : 26409) //			Avoid calling new and delete explicitly, use std::make_unique<T> instead
#pragma warning(disable : 26423) //			The allocation was not directly assigned to an owner
#pragma warning(disable : 26424) //			Failing to delete or assign ownership of allocation
//#pragma warning(disable : 26425) //			Assigning 'return of new[]' to a static variable
//#pragma warning(disable:26429)	// f.23		Symbol is never tested for nullness, it can be marked as not_null
//#pragma warning(disable:26438)	// es.76	Avoid 'goto'
// We disable 26446 as operator[] is bounds checked for vectors in the debug version
#pragma warning(disable : 26446) // bounds.4 Prefer to use gsl::at() instead of unchecked subscript operator
//#pragma warning(disable : 26451) // io.2		Arithmetic overflow
//#pragma warning(disable:26461)	// con.3	The pointer argument can be marked as a pointer to const
//#pragma warning(disable:26472)	// type.1	Don't use a gsl::narrow_cast for arithmetic conversions
//#pragma warning(disable:26474)	// type.1	Don't cast between pointer types when the conversion could be implicit
#pragma warning(disable : 26481) // bounds.1	Don't use pointer arithmetic. Use span instead
#pragma warning(disable : 26482) // bounds.2	Only index into arrays using constant expressions
#pragma warning(disable : 26485) // bounds.3	No array to pointer decay
#pragma warning(disable : 26486) // lifetime.1	Don't pass a pointer that may be invalid to a function
#pragma warning(disable : 26489) // lifetime.1	Don't dereference a pointer that may be invalid
//#pragma warning(disable:26490)	// type.1	Don't use reinterpret_cast
//#pragma warning(disable:26493)	// type.4	Don't use C-style casts
//#pragma warning(disable:26494)	// type.5	Variable is uninitialized
//#pragma warning(disable:26496)	// Con.4	Value is assigned only once, mark it as a pointer to const
//#pragma warning(disable:26497)	// f.4		This function could be marked constexpr if compile-time evaluation is
// desired
#pragma warning(disable : 26499) //			Could not find any lifetime tracking information

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
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"
#include "EnumMap.h"

namespace fs = std::experimental::filesystem;

extern unsigned   ActiveColor;
extern unsigned   ActiveLayer;
extern unsigned   ActivePointIndex;
extern fRECTANGLE AllItemsRect;

extern std::vector<fPOINT>* AngledFormVertices;

extern unsigned  AppliqueColor;
extern LPWSTR*   ArgList; // command line argument array
extern HCURSOR   ArrowCursor;
extern fs::path* AuxName;

extern std::vector<BSEQPNT>*   BSequence;

extern HBRUSH    BackgroundBrush;
extern unsigned  BeanCount; // number of stitches added by convert to bean
extern HDC       BitmapDC;
extern RECT      BitmapDstRect;
extern unsigned  BitmapHeight;
extern POINT     BitmapPoint;
extern dPOINT    BitmapSizeinStitches;
extern RECT      BitmapSrcRect;
extern dPOINT    BmpStitchRatio;
extern unsigned  BitmapWidth;
extern double    BorderWidth;
extern unsigned  ButtonHeight;
extern unsigned  ButtonWidth;
extern unsigned  ButtonWidthX3;

extern std::vector<HWND>* ButtonWin;

extern float    ButtonholeCornerLength;
extern HWND     CancelButton;
extern unsigned Clip;

extern std::vector<fPOINTATTR>* ClipBuffer;

extern FORMCLIP* ClipFormHeader;
extern HGLOBAL   ClipMemory;
extern void*     ClipPointer;

extern std::vector<fPOINT>* ClipPoints;

extern fRECTANGLE   ClipRect;
extern FLSIZ        ClipRectSize;
extern CLPSTCH*     ClipStitchData;
extern unsigned     ClipTypeMap;
extern unsigned int ClosestFormToCursor;
extern unsigned     ClosestPointIndex;
extern unsigned int ClosestVertexToCursor;
extern int*         ColorBarSize;
extern HCURSOR      CrossCursor;
extern unsigned int CurrentFormGuides;
extern unsigned int CurrentFormGuidesCount;
extern unsigned int CurrentVertexIndex;

extern std::vector<HWND>* DefaultColorWin;

extern HWND DeleteStitchesDialog;

extern std::wstring*   DesignerName;
extern HWND            DiscardButton;
extern DRAWITEMSTRUCT* DrawItem;
extern POINT           EndPointCross;
extern HMENU           FillMenu;

extern std::vector<double>* FormAngles;
extern std::vector<POINT>*  FormControlPoints;

extern HWND         FormDataSheet;

extern std::vector<POINT>*  FormLines;
extern std::vector<FRMHED>* FormList;

extern unsigned int FormMenuChoice;
extern fPOINT       FormMoveDelta;

extern std::wstring* FormOnOff;

extern HPEN FormPen3px;
extern HPEN FormPen;
extern HPEN FormSelectedPen;

extern std::vector<fPOINT>* FormVertices;

extern HWND      GeneralNumberInputBox;
extern HPEN      GridPen;
extern unsigned  GroupEndStitch;
extern unsigned  GroupStartStitch;
extern unsigned  GroupStitchIndex;
extern fs::path* HomeDirectory;
extern double    HorizontalLength;
extern double    HorizontalLength2;
extern double    HorizontalRatio;
extern HWND      HorizontalScrollBar;
extern INIFILE   IniFile;
extern POINT     InsertLine[3];

extern std::vector<fPOINT>* InsidePointList;
extern std::vector<fPOINT>* InsidePoints;

extern unsigned InterleaveSequenceIndex;
extern INSREC   InterleaveSequenceIndices[10];
extern fPOINT   InterleaveSequence[MAXITEMS];

extern std::vector<HWND>* LabelWindow;

extern fPOINT LastPoint;
extern HPEN   LayerPen[6];
extern double LineSpacing;
extern fPOINT LowerLeftStitch;
extern HMENU  MainMenu;
extern HWND   MainStitchWin;
extern float  MaxStitchLen;

extern MENUITEMINFO* MenuInfo;

extern double       MinStitchLength;
extern MSG          Msg;
extern wchar_t      MsgBuffer[MSGSIZ];
extern unsigned     MsgIndex;
extern RECT         MsgRect;
extern HWND         MsgWindow;
extern HPEN         MultiFormPen;
extern unsigned     NearestCount;
extern unsigned int NewFormVertexCount;
extern HWND         OKButton;
extern fPOINT       OSequence[OSEQLEN];
extern bool         OutLineEverySelectedForm;
extern unsigned int OutputIndex;

extern std::vector<fPOINT>* OutsidePointList;
extern std::vector<fPOINT>* OutsidePoints;

extern char           PCSBMPFileName[16];
extern PCSHEADER      PCSHeader;
extern const wchar_t* PcdClipFormat;
extern float          PicotSpacing;
extern unsigned       PreferenceIndex;
extern long           PreferenceWindowWidth;
extern HWND           PreferencesWindow;
extern unsigned int   PreviousFormIndex;
extern unsigned       PseudoRandomValue;
extern fRECTANGLE     RotationRect;

extern std::vector<POINT>* RubberBandLine;

extern unsigned int SatinEndGuide;

extern std::vector<SATCON>* SatinGuides;

extern RECT      scRct;
extern unsigned* screenDPI;
extern int*      ScrollSize;
extern unsigned  SearchLineIndex;
extern HPEN      SelectAllPen;
extern FRMHED*   SelectedForm;
extern unsigned  SelectedFormControlVertex;

extern std::vector<unsigned int>* SelectedFormList;
extern FORMVERTICES               SelectedFormVertices;
extern std::vector<POINT>*        SelectedFormsLine;

extern RECT   SelectedFormsRect;
extern fPOINT SelectedFormsSize;
extern RECT   SelectedPixelsRect;
extern fPOINT SelectedPoint;

extern std::vector<POINT>* SelectedPointsLine;

extern fRECTANGLE   SelectedVerticesRect;
extern unsigned int SequenceIndex;
extern double       ShowStitchThreshold;
extern HWND         SideMessageWindow;
extern wchar_t      SideWindowEntryBuffer[11];
extern HWND         SideWindow[16];
extern double       SmallStitchLength;
extern double       SnapLength;
extern double       SpiralWrap;
extern double       StarRatio;

extern EnumMap<StateFlag> StateMap;

extern dPOINT     StitchBmpRatio;
extern double     StitchBoxesThreshold;
extern fPOINTATTR StitchBuffer[MAXITEMS * 2];
extern POINT      StitchCoordinatesPixels;
extern fRECTANGLE StitchRangeRect;
extern RECT       StitchWindowClientRect;
extern HDC        StitchWindowDC;
extern HDC        StitchWindowMemDC;
extern POINT      StitchWindowOrigin;
extern POINT      StitchWindowSize;

extern std::vector<std::wstring>* StringTable;

extern std::vector<fPOINT>* TempPolygon;
extern unsigned int         TextureIndex;
extern std::wstring*        TextureInputBuffer;
extern std::vector<TXPNT>*  TexturePointsBuffer;
extern HWND                 ThreadSizeWin[16];
extern unsigned             ThrEdClip;
extern const wchar_t*       ThrEdClipFormat;
extern HINSTANCE            ThrEdInstance;
extern HWND                 ThrEdWindow;
extern fs::path*            ThrName;
extern POINT                ThredWindowOrigin;
extern unsigned*            TraceBitmapData;
extern HDC                  TraceDC;

extern boost::dynamic_bitset<>* TracedEdges;
extern boost::dynamic_bitset<>* TracedMap;

extern POINT    UnzoomedRect;
extern COLORREF UserColor[16];
extern HWND     UserColorWin[16];

extern EnumMap<UserFlag> UserFlagMap;

extern HPEN   UserPen[16];
extern double UserStitchLength;

extern std::vector<HWND>* ValueWindow;

extern unsigned int VertexCount;
extern double       VerticalRatio;
extern HWND         VerticalScrollBar;
extern fs::path*    WorkingFileName;
extern POINT        ZoomBoxLine[5];
extern fPOINT       ZoomBoxOrigin;
extern double       ZoomFactor;
extern dPOINT       ZoomMarkPoint;
extern dPOINT       ZoomRatio;
extern dRECTANGLE   ZoomRect;
