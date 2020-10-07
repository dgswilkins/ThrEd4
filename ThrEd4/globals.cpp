#include "stdafx.h"

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
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#pragma warning(disable : 26455) // supress warning for library headers
#include <boost/dynamic_bitset.hpp>
#pragma warning(pop)

// Local Headers
#include "ThrEdTypes.h"
#include "EnumMap.h"

namespace fs = std::filesystem;

auto ActiveColor = uint8_t {};                              // active color selector
auto ActiveLayer = uint8_t {};                              // active layer
auto AllItemsRect = gsl::narrow_cast<fRECTANGLE*>(nullptr); // rectangle enclosing all forms and stitches
auto AngledFormVertices     = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // form formOrigin data for angle fills
auto AppliqueColor          = uint8_t {COLORCNT - 1U};                         // underlay color
auto AuxName                = gsl::narrow_cast<fs::path*>(nullptr);             //
auto BSequence              = gsl::narrow_cast<std::vector<BSEQPNT>*>(nullptr); //
auto BackgroundColor        = COLORREF {};    // stitch window background
auto BorderWidth            = float {BRDWID}; // border width for satin borders
auto ButtonHeight           = int32_t {};     // button height
auto ButtonWidth            = int32_t {};     // button width
auto ButtonWidthX3          = int32_t {};     // button width times 3
auto ButtonWin              = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // button windows
auto ButtonholeCornerLength = float {IBFCLEN};                 // buttonhole corner length
auto CancelButton           = gsl::narrow_cast<HWND>(nullptr); // cancel button
auto Clip                   = uint32_t {};                     // pcs format clipboard handle
auto ClipBuffer             = gsl::narrow_cast<std::vector<fPOINTATTR>*>(nullptr); //
auto ClipMemory             = gsl::narrow_cast<HGLOBAL>(nullptr); // handle to the clipboard memory
auto ClipPointer = gsl::narrow_cast<void*>(nullptr); // for memory allocation for clipboard data
auto ClipPoints = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // main clipboard fill points for forms
auto ClipRectSize          = FLSIZ {};                             // clipboard rectangle size
auto ClipStitchData        = gsl::narrow_cast<CLPSTCH*>(nullptr); // for pcs clipboard data
auto ClipTypeMap           = uint32_t {MCLPF | MVCLPF | MHCLPF | MANGCLPF}; // for checking if a fill is a clipboard fill
auto ClosestFormToCursor   = uint32_t {}; // closest form to the cursor
auto ClosestPointIndex     = uint32_t {}; // index of closest point
auto ClosestVertexToCursor = uint32_t {}; // formOrigin closest to the cursor
auto ColorBarSize          = gsl::narrow_cast<int32_t*>(nullptr); // Color bar width scaled for DPI
auto DefaultColorWin       = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // default color windows
auto DeleteStitchesDialog =
    gsl::narrow_cast<HWND>(nullptr); // asking user if they want to delete the sitches associated with a form
auto DesignerName   = gsl::narrow_cast<std::wstring*>(nullptr);   // designer name in clear
auto DiscardButton  = gsl::narrow_cast<HWND>(nullptr);            // discard button
auto DrawItem       = gsl::narrow_cast<DRAWITEMSTRUCT*>(nullptr); // for owner-draw windows
auto EndPointCross  = POINT {}; // point to draw at the end cross for form select points
auto ExtendedHeader = gsl::narrow_cast<STREX*>(nullptr);              // thred file header extension
auto FillMenu       = gsl::narrow_cast<HMENU>(nullptr);               // fill submenu
auto FormAngles     = gsl::narrow_cast<std::vector<float>*>(nullptr); // angles of a form for satin border fills
auto FormControlPoints =
    gsl::narrow_cast<std::vector<POINT>*>(nullptr);   // form control rectangle in pixel coordinates
auto FormDataSheet = gsl::narrow_cast<HWND>(nullptr); // form data sheet
auto FormLines = gsl::narrow_cast<std::vector<POINT>*>(nullptr); // used in the form drawing routines
auto FormList        = gsl::narrow_cast<std::vector<FRMHED>*>(nullptr); // a list of form headers
auto FormMenuChoice  = uint32_t {0U}; // data type for form data form numerical entry
auto FormMoveDelta   = fPOINT {};     // offset for moving forms
auto FormOnOff       = gsl::narrow_cast<std::wstring*>(nullptr);
auto FormPen         = gsl::narrow_cast<HPEN>(nullptr);                 // form pen
auto FormPen3px      = gsl::narrow_cast<HPEN>(nullptr);                 // three-pixel form pen
auto FormSelectedPen = gsl::narrow_cast<HPEN>(nullptr);                 // form select pen
auto FormVertices    = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // form points
auto GeneralNumberInputBox = gsl::narrow_cast<HWND>(nullptr);           // general number input box
auto GridPen               = gsl::narrow_cast<HPEN>(nullptr);           // pen for stitch grid
auto GroupEndStitch        = uint32_t {}; // higher end of selected stitches
auto GroupStartStitch      = uint32_t {}; // lower end of selected stitches
auto GroupStitchIndex      = uint32_t {}; // last point selected in group
auto HorizontalRatio = float {}; // horizontal ratio between the zoom window and the entire stitch space
auto HorizontalScrollBar = gsl::narrow_cast<HWND>(nullptr); // horizontal scroll bar
auto IniFile             = INIFILE {};                      // initialization file
auto InsertLine          = std::array<POINT, 3> {};         // the insert line
auto InsidePointList =
    gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // list of inside outline points for satin or clipboard fills
auto InsidePoints = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // pointer to the list of inside outline points
auto InterleaveSequenceIndices =
    gsl::narrow_cast<std::vector<INSREC>*>(nullptr); // indices into interleave points
auto InterleaveSequence = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // storage for interleave points
auto LabelWindow = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // text handles for the form data sheet
auto LastPoint          = fPOINT {}; // last formOrigin written by line connect routine
auto LineSpacing        = float {DEFSPACE * PFGRAN};        // stitch spacing in stitch units
auto LowerLeftStitch    = fPOINT {};                        // lower left formOrigin in a form
auto MainMenu           = gsl::narrow_cast<HMENU>(nullptr); // main menu
auto MainStitchWin      = gsl::narrow_cast<HWND>(nullptr);  // stitch window
auto MaxStitchLen       = float {};                         // maximum stitch length
auto MenuInfo           = gsl::narrow_cast<MENUITEMINFO*>(nullptr);
auto MinStitchLength    = float {MINSIZ * PFGRAN};         // minimum stitch size
auto Msg                = MSG {};                          // main message loop message
auto MsgBuffer          = std::array<wchar_t, MSGSIZ> {};  // for user messages
auto MsgIndex           = uint32_t {};                     // pointer to the message buffer
auto MsgRect            = RECT {};                         // rectangle containing the text message
auto MsgWindow          = gsl::narrow_cast<HWND>(nullptr); // message window
auto MultiFormPen       = gsl::narrow_cast<HPEN>(nullptr); // multiple selected forms pen
auto NearestCount       = uint32_t {};                     // number of boxes selected
auto NewFormVertexCount = uint32_t {};                     // points in the new form
auto OKButton           = gsl::narrow_cast<HWND>(nullptr); // ok button
auto OSequence = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // temporary storage for sequencing
auto OutLineEverySelectedForm = bool {false}; // When selecting multiple forms, should we outline every form?
auto OutsidePointList =
    gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // list of outside outline points for satin or clipboard fills
auto OutsidePoints         = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // pointer to the list of outside outline points
auto PcdClipFormat         = L"PMust_Format";                                 //
auto PicotSpacing          = float {IPICSPAC}; // space between border picots
auto PreferenceIndex       = uint32_t {0};     // index to the active preference window
auto PreferenceWindowWidth = int32_t {};       // width of the preference window
auto PreferencesWindow     = gsl::narrow_cast<HWND>(nullptr); // preferences window
auto PreviousFormIndex     = uint32_t {};                     // previously selected form
auto PseudoRandomValue     = uint32_t {};                     // pseudo-random sequence register
auto RotationRect          = fRECTANGLE {};                   // rotation rectangle
auto RubberBandLine = gsl::narrow_cast<std::vector<POINT>*>(nullptr); // points to form points to be moved
auto SatinGuides  = gsl::narrow_cast<std::vector<SATCON>*>(nullptr); // satin form connects
auto scRct        = RECT {};                                         //
auto screenDPI    = gsl::narrow_cast<int32_t*>(nullptr);             // screen DPI
auto ScrollSize   = gsl::narrow_cast<int32_t*>(nullptr); // Scroll bar width scaled for DPI
auto SearchLine   = gsl::narrow_cast<std::vector<POINT>*>(nullptr); // stitch select line
auto SelectAllPen = gsl::narrow_cast<HPEN>(nullptr);                // pen for drawing large boxes
auto SelectedFormControlVertex = uint32_t {}; // user selected form control formOrigin
auto SelectedFormList = gsl::narrow_cast<std::vector<uint32_t>*>(nullptr); // a list of selected forms
auto SelectedFormVertices = FORMVERTICES {};                               // selected form vertices
auto SelectedFormsLine    = gsl::narrow_cast<std::vector<POINT>*>(nullptr); // line derived from the big rectangle
auto SelectedFormsRect    = RECT {};   // for multiple selections;
auto SelectedFormsSize    = fPOINT {}; // size of multiple select rectangle
auto SelectedPixelsRect   = RECT {};   // display form vertex select rectangle
auto SelectedPointsLine =
    gsl::narrow_cast<std::vector<POINT>*>(nullptr); // line derived from the formOrigin select rectangle
auto SelectedVerticesRect = fRECTANGLE {};          // rectangle enclosing selected form verticess
auto ShowStitchThreshold  = float {SHOPNTS};        // show stitch grid below this zoom level
auto SideMessageWindow    = gsl::narrow_cast<HWND>(nullptr); // main side message window
auto SideWindowEntryBuffer = std::array<wchar_t, SWBLEN> {}; // buffer for entering form data sheet numbers
auto SideWindow        = std::array<HWND, SWCOUNT> {}; // side message windows
auto SmallStitchLength = float {SMALSIZ * PFGRAN};     // user can remove stitches smaller than this
auto SnapLength        = float {SNPLEN * PFGRAN};      // snap together length
auto SpiralWrap        = float {SPIRWRAP};             // number of revolutions in a spiral
auto StarRatio         = float {STARAT};               // star formOrigin to body ratio
auto StateMap = gsl::narrow_cast<EnumMap<StateFlag>*>(nullptr); // Flags indicating current run state
auto StitchBmpRatio       = fPOINT {};                          // stitch hoop to bitmap ratios
auto StitchBoxesThreshold = float {STCHBOX}; // threshold for drawing stitch boxes
auto StitchBuffer    = gsl::narrow_cast<std::vector<fPOINTATTR>*>(nullptr); // main stitch buffer
auto StitchRangeRect = fRECTANGLE {};                         // stitch range rectangle
auto StitchWindowClientRect = RECT {};                        // stitch window size,client
auto StitchWindowDC         = gsl::narrow_cast<HDC>(nullptr); // stitch window device context
auto StitchWindowMemDC      = gsl::narrow_cast<HDC>(nullptr); // stitch window memory device context
auto StitchWindowOrigin     = POINT {};                       // offset origin of the stitch window
auto StitchWindowSize       = POINT {};                       // size of the stitch window in pixels
auto StringTable            = gsl::narrow_cast<std::vector<std::wstring>*>(nullptr); //
auto TempPolygon            = gsl::narrow_cast<std::vector<fPOINT>*>(nullptr); // temporary storage when user is entering a polygon;
auto TextureHistory         = gsl::narrow_cast<std::array<TXHST, ITXBUFSZ>*>(nullptr); // texture editor history headers
auto TextureInputBuffer = gsl::narrow_cast<std::wstring*>(nullptr); // texture fill number buffer
auto TexturePointsBuffer = gsl::narrow_cast<std::vector<TXPNT>*>(nullptr); // buffer for textured fill points
auto ThreadSizeWin     = std::array<HWND, COLORCNT> {};                    // thread size windows
auto ThrEdClipFormat   = L"threditor";                                     //
auto ThrEdInstance     = HINSTANCE {};                                     // main instance handle
auto ThrEdWindow       = gsl::narrow_cast<HWND>(nullptr);                  // main window handle
auto ThrName           = gsl::narrow_cast<fs::path*>(nullptr);             //
auto ThredWindowOrigin = POINT {};                             // offset origin of the main window
auto TraceBitmapData   = gsl::narrow_cast<uint32_t*>(nullptr); // trace bitmap data
auto TracedEdges = gsl::narrow_cast<boost::dynamic_bitset<>*>(nullptr); // detected edges of trace areas
auto TracedMap = gsl::narrow_cast<boost::dynamic_bitset<>*>(nullptr); // in/out state of trace areas
auto UnzoomedRect     = POINT {};                          // size of the unzoomed stitch window
auto UserColor        = std::array<COLORREF, COLORCNT> {}; // user colors
auto UserColorWin     = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // user color windows
auto UserFlagMap      = gsl::narrow_cast<EnumMap<UserFlag>*>(nullptr); // for storage of persistent binary variables set by the user
auto UserPen          = std::array<HPEN, COLORCNT> {};            // user color pens
auto UserStitchLength = float {USESIZ * PFGRAN};                  // user selected stitch size
auto ValueWindow = gsl::narrow_cast<std::vector<HWND>*>(nullptr); // data handles for the form data sheet
auto VerticalRatio = float {}; // vertical ratio between the zoom window and the entire stitch space
auto VerticalScrollBar = gsl::narrow_cast<HWND>(nullptr);      // vertical scroll bar
auto WorkingFileName   = gsl::narrow_cast<fs::path*>(nullptr); //
auto ZoomBoxLine       = std::array<POINT, SQPNTS> {};         // the zoom box
auto ZoomBoxOrigin     = fPOINT {};                            // zoom box origin
auto ZoomFactor        = float {1.0F};                         // zoom factor
auto ZoomMarkPoint     = fPOINT {};     // stitch coordinates of the zoom mark
auto ZoomRatio         = fPOINT {};     // zoom ratio used to draw stitch window
auto ZoomRect          = fRECTANGLE {}; // zoom rectangle
