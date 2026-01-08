// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
#ifndef THRED_THRED_TYPES_H
#define THRED_THRED_TYPES_H

// Local Headers
#include "Resources/resource.h"
#include "utf8conv.h"

// Open Source headers
#include "gsl/gsl"
#ifdef _DEBUG
#pragma warning(push)
#pragma warning(disable : 4702) // supress warning for fmt library header
#include "fmt/xchar.h"
#include "fmt/compile.h"
#pragma warning(pop)
#endif

// Standard Libraries
#include <numbers>
#ifdef _DEBUG
#include <source_location>
#else
#include <stdexcept>
#endif

#ifdef _DEBUG
class FMT_WITH_LOC
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  const wchar_t*       strX;
  std::source_location loc;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  // ReSharper disable once CppNonExplicitConvertingConstructor
  // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
  constexpr FMT_WITH_LOC(const wchar_t* fmtString,
                         const std::source_location& fileLoc = std::source_location::current()) noexcept
      : strX(fmtString), loc(fileLoc) {
  }
};

template <typename... Args> constexpr void outDebugString(FMT_WITH_LOC fwl, Args&&... args) {
  auto       name = utf::utf8ToUtf16(std::string(fwl.loc.file_name()));
  auto       line = fwl.loc.line();
  auto const strY = format(FMT_COMPILE(L" {}({}) : {}"), name, line, fwl.strX);
  auto const strZ = fmt::format(fmt::runtime(strY), std::forward<Args>(args)...);
  OutputDebugString(strZ.c_str());
}
#else
template <typename... Args>
constexpr void outDebugString([[maybe_unused]] const wchar_t* value, [[maybe_unused]] Args... args) {
}
#endif

constexpr auto DEFUSPAC = 6.0F;         // default underlay stitch spacing
constexpr auto DEFLRAT  = 0.8F;         // default lens form aspect ratio
constexpr uint32_t MAXITEMS = 0x10000U; // maximum number of stitches, sequence items & clipboard points
constexpr auto SHUPX    = 480.0F;       // small hoop x size
constexpr auto SHUPY    = 480.0F;       // small hoop y size
constexpr auto LHUPX    = 719.0F;       // large hoop x size
constexpr auto LHUPY    = 690.0F;       // large hoop y size
constexpr auto HUP100XY = 600.0F;       // 100 millimeter hoop size
constexpr auto PFGRAN   = 6.0F;         // pfaf stitch points per millimeter
constexpr auto IPFGRAN  = 1.0F / 6.0F; // pfaf millimeters per stitch point
// ToDo - Should this be a configurable parameter?
constexpr LONG ICLOSNUF = 8L; // mouse click region for select
constexpr auto CLOSENUF = double {ICLOSNUF};
constexpr auto FCLOSNUF = float {CLOSENUF};
constexpr auto SMALSIZ  = 0.25F;                     // default small stitch size
constexpr auto MINSIZ   = 0.1F;                      // default minimum stitch size
constexpr auto USESIZ   = 3.5F;                      // user preferred size
constexpr auto MAXSIZ   = 9.0F;                      // default maximum stitch size
constexpr auto SHOPNTS  = 0.00F;                     // show stitch points when zoom below this
constexpr auto STCHBOX  = 0.4226F;                   // show stitch boxes when zoom below this
constexpr auto DEFSPACE = 0.45F;                     // default stitch spacing
constexpr auto PI_F     = std::numbers::pi_v<float>; // PI to single precision
constexpr auto PI_F2    = PI_F * 2.0F;               //
constexpr auto PI_FHALF = PI_F / 2.0F;               //
constexpr auto PI_D     = std::numbers::pi;          // PI to double precision
constexpr auto PI_D2    = PI_D * 2.0;                //
constexpr auto USPAC    = 15.0F;                     // underlay fill spacing
constexpr auto URAT     = 0.75F;         // ratio of underlay stitch to satin border size
constexpr auto QUADRT   = uint32_t {4U}; // split display into quadrants

constexpr auto     IBFCLEN  = 4.0F * PFGRAN; // initial buttonhole fill corner length
constexpr auto     IPICSPAC = 6.0F;          // initial picot border space
constexpr int32_t  PRFLINS  = 30;            // number of lines on the preference menu
constexpr uint32_t SEED     = 3037000499U;   // pseudo-random-sequence seed
constexpr auto     BRDWID   = 18.0F;         // default satin border size
constexpr auto     SNPLEN   = 0.15F;         // default snap together length size
constexpr auto     STARAT   = 0.4F;          // default star ratio
constexpr auto     SPIRWRAP = 1.52F;         // default spiral wrap
constexpr int32_t  HBUFSIZ  = 1024;          // help buffer size
constexpr auto     ITXHI    = 9.0F * PFGRAN; // default texture editor height
constexpr auto     ITXWID   = 9.0F * PFGRAN; // default texture editor width
constexpr auto     ITXSPAC  = 0.4F * PFGRAN; // default texture editor spacing
constexpr uint16_t ITXPIX   = 5U;            // default texture editor cross pixels
constexpr uint32_t BTNCOUNT = 9U;            // Maximum number of buttons below the color buttons
constexpr int32_t  FONTSIZE = 400;           // default font size
constexpr uint32_t B1MASK   = 0x000000ffU;   // mask for the least significant byte
constexpr uint32_t B2MASK   = 0x0000ff00U;   // mask for the middle byte
constexpr uint32_t B3MASK   = 0x00ff0000U;   // mask for the more significant byte
constexpr uint32_t BYTSHFT  = 8U;            // bits required to shift one byte left
constexpr uint32_t WRDSHFT  = 16U;           // bits required to shift two bytes left
constexpr uint32_t TBYTSHFT = 24U;           // bits required to shift three bytes left
constexpr uint32_t HBSHFT   = 31U;           // shift required to reach the MSB of uint32
constexpr uint8_t  NIBMASK  = 0x0fU;         // Nibble mask
constexpr uint8_t  BYTMASK  = 0xffU;         // Byte mask
constexpr uint16_t WRDMASK  = 0xffffU;       // word mask
constexpr uint32_t COLMASK  = 0xffffffU;     // COLORREF mask
constexpr uint8_t  MSK7BITS = 0x7fU;         // mask value to 7 bits
constexpr uint8_t  MSK3BITS = 0x7U;          // mask value to 3 bits

constexpr uint32_t LNPNTS  = 2U;            // number of points required to draw a line
constexpr uint32_t TRIPNTS = 4U;            // number of points required to draw a triangle or arrow
constexpr uint32_t SQPNTS  = 5U;            // number of points required to draw a square
constexpr uint32_t OUTPNTS = 9U;            // number of points required to draw an outline
constexpr auto     RADDEGF = 180.0F / PI_F; // float factor to convert radians to degrees
constexpr auto     DEGRADF = PI_F / 180.0F; // float factor to convert degrees to radians

constexpr auto TNYFLOAT = 1e-9F; // tiny number for single precision float stuff
// large and small constants for multiple types
constexpr auto BIGFLOAT = std::numeric_limits<float>::max();
constexpr auto LOWFLOAT = std::numeric_limits<float>::lowest();
constexpr auto BIGDBL   = std::numeric_limits<double>::max();
constexpr auto LOWDBL   = std::numeric_limits<double>::lowest();
constexpr auto BIGLONG  = std::numeric_limits<LONG>::max();
constexpr auto LOWLONG  = std::numeric_limits<LONG>::lowest();
constexpr auto BIGDWORD = std::numeric_limits<DWORD>::max();
constexpr auto LOWDWORD = std::numeric_limits<DWORD>::lowest();
constexpr auto BIGUINT  = std::numeric_limits<uint32_t>::max();
constexpr auto BGUINT16 = std::numeric_limits<uint16_t>::max();
constexpr auto BIGINT32 = std::numeric_limits<int32_t>::max();
constexpr auto BIGSIZE  = std::numeric_limits<size_t>::max();

constexpr auto PENBLK   = COLORREF {0x000000U}; // Black Pen
constexpr auto PENCHCL  = COLORREF {0x404040U}; // Charcoal pen
constexpr auto PENGRAY  = COLORREF {0x808080U}; // Gray pen
constexpr auto PENLILAC = COLORREF {0xc080c0U}; // Lilac pen
constexpr auto PENLMGRN = COLORREF {0x40c040U}; // Lime green pen
constexpr auto PENOLIVE = COLORREF {0xc0c040U}; // Olive pen
constexpr auto PENPOLIV = COLORREF {0xc0c080U}; // Pale olive pen
constexpr auto PENPRPLE = COLORREF {0x804080U}; // Purple pen
constexpr auto PENROSY  = COLORREF {0xc08080U}; // Rosy pen
constexpr auto PENSEGRN = COLORREF {0x80c080U}; // Sea green pen
constexpr auto PENSILVR = COLORREF {0xc0c0c0U}; // Silver pen
constexpr auto PENTEAL  = COLORREF {0x40c0c0U}; // Teal pen
constexpr auto PENTRQSE = COLORREF {0x80c0c0U}; // Turquoise pen
constexpr auto PENWHITE = COLORREF {0xffffffU}; // White pen
constexpr auto PENNWID  = 1;                    // Narrow pen width
constexpr auto PENMWID  = 3;                    // Medium pen width
constexpr auto PENWWID  = 5;                    // Wide pen width
constexpr auto STDDPI   = int32_t {96};         // Default Display DPI

constexpr auto FRACFACT = 256.0F; // float factor to convert the fractional part to/from the lower byte
constexpr auto HALF   = 0.5F;     // factor for halving
constexpr auto DBLF   = 2.0F;     // factor for doubling
constexpr auto DECRAD = 10;       // decimal radix

// Dialog border offsets
constexpr auto DLGN = 6; // dialog border narrow offset 
constexpr auto DLGW = DLGN * 2; // dialog border left/right or top/bottom combined offset
constexpr auto DLGH = 15; // dialog border huge offset

// rectangle position indices
constexpr auto PTL  = 0; // top left corner       80-1-2
constexpr auto PTM  = 1; // top midpoint           |   |
constexpr auto PTR  = 2; // top right corner       7   3
constexpr auto PRM  = 3; // right midpoint         |   |
constexpr auto PBR  = 4; // bottom right corner    6-5-4
constexpr auto PBM  = 5; // bottom midpoint
constexpr auto PBL  = 6; // bottom left corner
constexpr auto PLM  = 7; // left midpoint
constexpr auto PTLE = 8; // top left endpoint

// main menu items
constexpr auto M_FILE = int8_t {0};
constexpr auto M_VIEW = int8_t {1};
//constexpr auto M_FORM = int8_t {2};
constexpr auto M_EDIT = int8_t {3};
//constexpr auto M_IN   = int8_t {4};
//constexpr auto M_OUT  = int8_t {5};
constexpr auto M_UNDO = int8_t {6};
constexpr auto M_REDO = int8_t {7};
//constexpr auto M_ROT  = int8_t {8};
//constexpr auto M_PREF = int8_t {9};
constexpr auto M_FILL = int8_t {10};
//constexpr auto M_ADD  = int8_t {11};
//constexpr auto M_FRM  = int8_t {12};
constexpr auto M_ALL  = int8_t {13};
//constexpr auto M_1    = int8_t {14};
//constexpr auto M_2    = int8_t {15};
//constexpr auto M_3    = int8_t {16};
//constexpr auto M_4    = int8_t {17};
//constexpr auto M_HELP = int8_t {18};

// fill menu items
//constexpr auto MFIL_SAT  = int8_t {0};
//constexpr auto MFIL_FTH  = int8_t {1};
//constexpr auto MFIL_VERT = int8_t {2};
//constexpr auto MFIL_HOR  = int8_t {3};
//constexpr auto MFIL_ANG  = int8_t {4};
//constexpr auto MFIL_CLP  = int8_t {5};
//constexpr auto MFIL_CONT = int8_t {6};
//constexpr auto MFIL_TED  = int8_t {7};
constexpr auto MFIL_BORD = int8_t {8};
//constexpr auto MFIL_UNFIL= int8_t {9};

// view menu items
//constexpr auto MVW_MOVIE = int8_t {0};
constexpr auto MVW_SET = int8_t {1};
//constexpr auto MVW_BACK = int8_t {2};
//constexpr auto MVW_ZOOM = int8_t {3};
//constexpr auto MVW_THRD = int8_t {4};
//constexpr auto MVW_SHWT = int8_t {5};
//constexpr auto MVW_SHWTCOL = int8_t {6};
//constexpr auto MVW_DSGN = int8_t {7};
//constexpr auto MVW_KNOT = int8_t {8};
//constexpr auto MVW_MARK = int8_t {9};
//constexpr auto MVW_ABOUT = int8_t {10};


enum class HoopSize : int8_t {
  kSetCustom = 1, // set the custom hoop
  kSmall = 2, // pfaf code for small hoop
  kLarge = 3, // pfaf code for large hoop
  kHundredMM  = 4, // 100 millimeter hoop
  kUserDefined = 5  // user defined hoop size
};

// bitmap
enum class StateFlag : uint16_t {
  SATIN,   // user is entering a satin stitch form
  SATPNT,  // user is entering points for a satin stitch form
  BOXZUM,  // box zoom select mode
  BZUMIN,  // display the zoom box
  BZUM,    // zoom box displayed on screen
  INSRT,   // insert mode active
  SELPNT,  // user has clicked on a selected point
  SATCNKT, // user is connecting points in a satin form
  INSFRM,  // user is inserting points in a form
  FORMIN,  // user is selecting a form
  PRFACT,  // user is selecting preferences
  POLIMOV, // user is placing a pre defined form
  SIZSEL,  // thread size select boxes displayed
  THRDS,   // State of the threads/lines box
  LINED,   // line has been started
  LININ,   // line is in the zoom rectangle
  ZUMED,   // stitch window is zoomed
  COL,     // state of the color/all box
  HID,     // state of the show/hide button
  ISDWN,   // down stitch exists
  ISUP,    // up stitch exists
  WASLIN,  // a move stitch line has been drawn
  CAPT,    // screen capture in effect
  VCAPT,   // keep screen capture as long as the mouse cursor
           // is in the edit window
  SELBOX,  // the select box is showing
  ILIN,    // insert line is showing on the screen
  INIT,   // set when a stitch is entered in the stitch buffer,a form is entered, or the mark is set
  LIN1,   // drawing only one line in stitch insert mode
  ILIN1,  // one insert line showing on screen
  GRPSEL, // group select mode
  SCROS,  // start group cross has been drawn
  ECROS,  // end group cross has been drawn
  ELIN,   // group select line has been drawn
  BAKEND, // insert data should be appended to the end of the file
  CLPSHO, // display the clipboard insert rectangle
  SELSHO, // display the select rectangle
  ROTAT,  // rotate state
  ROTSHO, // rotate rectangle is showing
  ROTUSHO,   // rotate indicator line is showing
  ROTCAPT,   // rotate capture mode
  MOVCNTR,   // user is moving the rotate center
  GETMIN,    // user is entering a minimum stitch length
  NUMIN,     // user is entering a number
  MOVFRM,    // user is moving a form vertex
  MOVMSG,    // user tried to move an edited form
  SHOFRM,    // form is currently written to the screen
  STHR,      // user is entering a new stitch box threshold
  ENTAR,     // user is adjusting a number with the up and down arrow keys
  ENTR30,    // user is entering a new thread size for #30 thread
  ENTR40,    // user is entering a new thread size for #40 thread
  ENTR60,    // user is entering a new thread size for #60 thread
  FORMSEL,   // a form is selected
  FRMPMOV,   // user is moving a point on a form
  IGNOR,     // user may elect to ignore a message about losing edits when resizing a form
  FRMOV,     // user is moving a form
  RUNPAT,    // user is running a pattern
  WASPAT,    // user ran a pattern, but hasn't done anything else yet
  FILDIR,    // direction lines in angle fill
  SHOSAT,    // the satin stitch form is on the screen
  SHOCON,    // connect point line is visible
  FENDIN,    // user is selecting fill ends
  DELFRM,    // user wants to delete a form
  BAKACT,    // there are entries in the undo buffer
  BAKWRAP,   // undo buffer pointer has wrapped
  BAKING,    // user has called the undo function
  REDUSHO,   // redo menu item is active
  UNDUSHO,   // undo menu item is active
  FUNCLP,    // user is loading a form from the clipboard
  FRMPSEL,   // user has selected a point in a form
  SHOINSF,   // the form insert line has been drawn
  SAT1,      // set when the first stitch is entered in a satin fill
  FILMSG,    // set when user tries to unfill a form with edited stitches
  CLPBAK,    // set when the first clipboard fill item has been entered
  STRTCH,    // user is stretching a form horizontally or vertically
  SHOSTRTCH, // stretch box is written to the screen
  EXPAND,    // user is expanding a form horizontally and vertically
  SIDACT,    // side message window is active
  BRDACT,    // border data in side message window
  FILTYP,    // 0=main fill, 1=border fill, used in side message display
  GMRK,      // draw the mark on the screen
  SIDCOL,    // user is selecting a fill color for a form
  BRDSID,    // user is selecting a border fill color for a form
  ENTRPOL,   // user is entering sides for a regular polygon
  FRMROT,    // user is rotating a form
  DELTO,     // user is deleting a form and the associated stitches
  REDOLD,    // user is opening a file from the old file list
  BAKSHO,    // user is looking at backup files
  IGNAM,     // save the thred file without making backups
  PRGMSG,    // delete all backups message is displayed
  NEWBAK,    // user is starting a new file, but may want to save changes
  SAVEX,     // user is exiting, may want to save changes
  OSAV,      // user wants to load a new file, but may want to save changes
  FRMOF,     // forms are not displayed and can't be selected
  SHOMOV,    // user moving form line shown on screen
  BARSAT,    // doing a bare satin fill to be filled in with clipboard data
  ENTRSTAR,  // user in entering the number of points for a star
  ENTROT,    // user is entering a rotate number for command rotate;
  ENTRDUP,   // user is entering a rotate angle for rotate and duplicate
  ENTRSPIR,  // user is entering the number of points for a spiral
  ENTRHART,  // user is entering the number of points for a heart
  ENTRLENS,  // user is entering the number of points for a lens
  SEQDUN,    // last stitch sequenced was already done
  WASEL,     // a form was selected when the user hit control-right-click
  MOVFRMS,   // user is moving a group of forms
  FRMSROT,   // user is rotating group of forms
  FUNSCLP,   // user is pasting a group of forms
  DELSFRMS,  // user is deleting a group of forms
  BIGBOX,    // user has selected all forms and stitches
  MOVSET,    // the move point has been selected
  UPTO,      // show points up to the selected point
  LENSRCH,   // user has hit the max or min button
  BOXSLCT,   // user is making a select box
  THUMSHO,   // thumbnail mode
  INSFIL,    // user is inserting a file
  RBUT,      // user pressed the right mouse button selecting a thumbnail
  UND,       // stitching a proportional satin underlay
  UNDPHAS,   // proportional satin underlay phase
  SAVAS,     // user is saving a file under a new name
  SHOSIZ,    // design size box is displayed
  HUPMSG,    // user is selecting a hoop size
  IGNTHR,    // ignore the close enough threshold for 'C' hotkey
  MONOMAP,   // set if a color bitmap is loaded
  THUMON,    // user is loading a thumbnail
  CLPOVR,    // can't fit another clipboard fill on the current line
  CONTIG,    // contiguous point flag
  NUROT,     // user is entering a rotate angle
  APSID,     // user is entering a new applique color
  FRMSAM,    // don't find an already selected form
  ENTRSEG,   // user is entering rotation segments
  ENTRFNUM,  // user is entering a form number
  WASFRMFRM, // need to restore the form-form
  SAVACT,    // activate the undo save
  CNTRH,     // for centering horizontally
  CNTRV,     // for centering vertically
  RESTCH,    // redraw the stitch window
  WASGRP,    // group of stitches selected when entering length search
  REFILMSG,  // refill all message is up
  PRELIN,    // user is inserting form points before the 0 point in a line form
  WASRT,     // insert was active when preferences activated
  WASDIR,    // last minimum direction for sequencing
  BRKFIX,    // last line sequenced was from the end of a break of an already done region
  RELAYR,    // need to write the layer in the layer button
  HIDSTCH,   // don't display the stitches
  WASIN,     // placing stitches inside the form for vertical clipboard fill
  LTRACE,    // line trace funtion active
  HUPEX,     // set when points are outside the hoop
  HUPCHNG,   // hoop size has been changed
  SIZED,     // window has been resized
  ROTAL,     // user is rotating all stitches
  ZUMACT,    // zoom to actual size
  WASREFIL,  // last fill was a refil
  DUMEN,     // menu needs to be refilled
  WASNEG,    // form had a point less than the size of the clipboard fill
  FCLOS,     // user is closing a file
  NOTFREE,   // no free space on the drive
  PIXIN,     // user in inputing nudge pixels
  STPXIN,    // user is inputing stitch point pixels
  FRMPXIN,   // user is inputting form vertex pixels
  WASMRK,    // user has set a mark
  RESIZ,     // need to change the size of the main window
  MSGOF,     // offset the message box
  WASMOV,    // mouse move capture flag
  WASCOL,    // mouse move started in a color box
  ENTREG,    // user is entering an egg form
  BADFIL,    // loaded file is corrupted
  WASTRAC,   // trace bitmap loaded
  LANDSCAP,  // trace bitmap is landscape position
  WASEDG,    // trace edge map was created
  TRCUP,     // trace the up limit
  TRCRED,    // red trace on
  TRCGRN,    // green trace on
  TRCBLU,    // blue trace on
  DUHI,      // paint the high trace rectangle
  DULO,      // paint the low trace rectangle
  WASDIF,    // found edges on bitmap
  WASDSEL,   // color selected bitmap
  TRNIN0,    // trace color number input
  TRNUP,     // trace up number input
  WASTRCOL,  // trace color entered from keyboard
  TRNIN1,    // trace parameter number input
  TRSET,     // trace numbers have been set
  WASESC,    // user hit escape
  HIDMAP,    // hide the bitmap
  WASBLAK,   // bitmap form pixels set to black
  LINCHN,    // chain stitch has line in middle
  ENTRZIG,   // user is creating a zig-zag form
  FOLFRM,    // form-form is displaying a follower form
  DUREFRM,   // display the form-form
  SIDX,      // display the extra fills
  WASLED,    // lead form found in frmchk
  FPSEL,     // form points selected
  SHOPSEL,   // form points selected rectangle is showing
  PSELDIR,   // direction of form vertex select
  FPUNCLP,   // user is retrieving clipped form points into a form
  SHOP,      // clipboard from points are showing
  WASPCDCLP, // there is pcd data on the clipboard
  FTHR,      // feather fill flag
  FBOTH,     // do both sides of a feather fill
  ENTRFTHUP, // user is entering the feather up number
  FTHSID,    // user is entering a feather color
  CNV2FTH,   // converting a form to feather ribbon
  WASFPNT,   // user is moving a form vertex
  WASDO,     // backup has been made before refill
  NOSEL,     // left button down didn't select anything
  WASDEL,    // stitches were deleted
  GTWLKIND,  // get the edge walk/underlay offset
  GTWLKLEN,  // get the edge walk/underlay stitch length
  GTUANG,    // get the underlay angle
  GTUSPAC,   // get the underlay spacing
  BADVER,    // key version does not match
  DUSRT,     // write sort info
  WLKDIR,    // edge walk direction, 0=forward
  INDIR,     // inner border switched with outer
  DIDSTRT,   // start stitches have been written
  REFCNT,    // counting the form-form lines
  UNDCOL,    // user is entering an underlay color
  FSETULEN,  // user is setting the underlay stitch length for a group of forms
  ISEND,     // writing the end stitches
  FSETUSPAC, // user is setting the underlay spacing for a group of forms
  FSETUANG,  // user is setting the underlay angle for a group of forms
  FSETFLEN,  // user is setting the fill stitch length for a group of forms
  FSETFSPAC, // user is setting the fill spacing for a group of forms
  FSETFANG,  // user is setting the fill angle for a group of forms
  FSETUCOL,  // user is setting the underlay color for a group of forms
  FSETFCOL,  // user is setting the fill color for a group of forms
  FSETBCOL,  // user is setting the border color for a group of forms
  FSETBLEN,  // user is setting the border color for a group of forms
  FSETBSPAC, // user is setting the border spacing for a group of forms
  FSETBMIN,  // user is setting the minimum border stitch length for a group of forms
  FSETBMAX,  // user is setting the maximum border stitch length for a group of forms
  FSETFMIN,  // user is setting the minimum fill stitch length for a group of forms
  FSETFMAX,  // user is setting the maximum fill stitch length for a group of forms
  FSETFWID,  // user is setting the width for a group of forms
  FSETFHI,   // user is setting the height for a group of forms
  TXTRED,    // displaying the texture fill editor
  FRMBOXIN,  // user is setting the form box pixels
  TXTMOV,    // user is moving a texture editor point
  TXTCLP,    // user is setting texture points with a clipboard form
  TXTLIN,    // user is setting texture points in a line
  WASTXBAK,  // texture fill editor history active
  LASTXBAK,  // last key was a texture back key
  TXBDIR,    // texture backup direction
  TXHCNTR,   // texture center on
  TXFIL,     // texture fill in progress
  WASWROT,   // texture clipboard form was written
  DESCHG,    // design size change 0=x, 1=y
  CMPDO,     // design has been changed
  ISUND,     // doing underlay fill
  CHKTX,     // user has changed the texture fill window size, check the points
  FSETFIND,  // user is setting the indent for a group of forms
  TXBOX,     // user is importing stitches for textured fill
  TXIN,      // last stitch point was in stitch select box
  SCLPSPAC,  // user is setting the clipbard fill spacing
  FCHK,      // check the forms
  NOCLP,     // don't load clipboard data from forms
  NOTHRFIL,  // loaded file wasn't a Thred file
  ROTCLOCK,  // Indicate direction of rotation

  EnumCount // must be the last entry in the enum
};

// user bitmap
enum class UserFlag : uint8_t {
  SQRFIL,  // square ends on fills
  BLUNT,   // blunt ends on satin lines
  NEDOF,   // needle cursor off
  KNOTOF,  // don't show knots
  BSAVOF,  // don't save PCS bitmaps
  LINSPAC, // evenly space border line points
  DUND,    // underlay in satin borders
  FIL2OF,  // fill to select off
  ROTAUX,  // rotate the aux file when saving
  SAVMAX,  // thred window maximized
  MARQ,    // remove mark when user hits escape only
  FRMX,    // cross cursor for form entry
  DAZHOL,  // daisy hole
  DAZD,    // daisy d-lines
  CHREF,   // refill forms when changing design size
  WRNOF,   // warn if edited off

  EnumCount // must be the last entry in the enum
};

class LIST_TYPE
{
  public:
  uint32_t stringID {}; // String resource ID
  uint8_t  value {};    // value index
};

// Layer values
enum Layers : uint8_t {
  LAYER0, // All Layers
  LAYER1, // Layer 1
  LAYER2, // Layer 2
  LAYER3, // Layer 3
  LAYER4  // Layer 4
};

constexpr auto LAYERMAX = 5U; // number of layers

// Feather types
enum FeatherStyles : uint8_t {
  FTHSIN = 1, // sine
  FTHSIN2,    // half sine
  FTHLIN,     // line
  FTHPSG,     // ragged
  FTHRMP,     // sawtooth
  FTHFAZ      // picket
};

constexpr auto FSTYLMAX = uint8_t {6U}; // count of feather styles

// fill types
enum FillStyles : uint8_t {
  VRTF = 1, // Vertical fill
  HORF,     // Horizontal fill
  ANGF,     // Angle fill
  SATF,     // Fan fill
  CLPF,     // Fan clip Fill
  CONTF,    // Contour fill
  VCLPF,    // Vertical clip fill
  HCLPF,    // Horizontal clip fill
  ANGCLPF,  // Angle clip fill
  FTHF,     // Feather fill
  TXVRTF,   // Vertical texture fill
  TXHORF,   // Horizontal texture fill
  TXANGF    // Angle texture fill
};

constexpr auto FILLTMAX = uint32_t {14U}; // number of fill types

constexpr auto MCLPF    = 1U << static_cast<uint32_t>(CLPF);
constexpr auto MVCLPF   = 1U << static_cast<uint32_t>(VCLPF);
constexpr auto MHCLPF   = 1U << static_cast<uint32_t>(HCLPF);
constexpr auto MANGCLPF = 1U << static_cast<uint32_t>(ANGCLPF);
constexpr auto CLIPTYPEMAP = MCLPF | MVCLPF | MHCLPF | MANGCLPF; // for checking if a fill is a clipboard fill

// edge fill types
enum EdgeFillStyles : uint8_t {
  EDGELINE = 1U, // Line
  EDGEBEAN,      // Bean
  EDGECLIP,      // Clipboard
  EDGEANGSAT,    // Angle Satin
  EDGEAPPL,      // Applique
  EDGEPROPSAT,   // Proportional Satin
  EDGEBHOL,      // Buttonhole
  EDGEPICOT,     // Picot
  EDGEDOUBLE,    // Double
  EDGELCHAIN,    // Line Chain
  EDGEOCHAIN,    // Open Chain
  EDGECLIPX      // Even Clipboard
};

constexpr auto EDGETMAX = uint32_t {13U}; // number of edge fill types

// preference window entries
enum PrefWin : uint8_t {
  PRFAPPCOL, // Applique color
  PRFAPSLEN, // Applique stitchLen
  PRFBRDWID, // Border width
  PRFBCNLEN, // Button corner length
  PRFCHFLEN, // Chain fill length
  PRFCHFPOS, // Chain fill position
  PRFCLPOFF, // Clipboard offset
  PRFCLPPHS, // Clipboard phase
  PRFEGGRAT, // Egg ratio
  PRFFILANG, // Fill angle
  PRFFILEND, // Fill ends
  PRFFILSPC, // Fill spacing
  PRFGRDCUT, // Grid cutoff
  PRFGRDSIZ, // Grid size
  PRFHUPTYP, // Hoop type
  PRFHUPHGT, // Hoop height
  PRFHUPWID, // Hoop width
  PRFLENRAT, // Lens ratio
  PRFNUGSTP, // Nudge
  PRFPCTSPC, // Picot spacing
  PRFSATEND, // Satin form ends
  PRFSATUND, // Satin underlay
  PRFSMLSTH, // Small stitch size
  PRFSNPSIZ, // Snap to size
  PRFSPLWRP, // Spiral wrap
  PRFSTRRAT, // Star ratio
  PRFSBXCUT, // Stitch box cutoff
  PRFSTCMAX, // Stitch length, maximum
  PRFSTCUSR, // Stitch length, user
  PRFSTCMIN  // Stitch length, minimum
};

constexpr auto PREFTMAX = uint32_t {30U}; // number of edge fill types

constexpr auto LAYRLIST = std::array<LIST_TYPE, LAYERMAX> {{{.stringID = IDS_LAY00, .value = LAYER0},
                                                            {.stringID = IDS_LAY01, .value = LAYER1},
                                                            {.stringID = IDS_LAY02, .value = LAYER2},
                                                            {.stringID = IDS_LAY03, .value = LAYER3},
                                                            {.stringID = IDS_LAY04, .value = LAYER4}}};

constexpr auto FTHRLIST = std::array<LIST_TYPE, FSTYLMAX> {{{.stringID = IDS_FTH0, .value = FTHSIN},
                                                            {.stringID = IDS_FTH1, .value = FTHSIN2},
                                                            {.stringID = IDS_FTH2, .value = FTHLIN},
                                                            {.stringID = IDS_FTH3, .value = FTHPSG},
                                                            {.stringID = IDS_FTH4, .value = FTHRMP},
                                                            {.stringID = IDS_FTH5, .value = FTHFAZ}}};

constexpr auto FILLLIST = std::array<LIST_TYPE, FILLTMAX> {{{.stringID = IDS_FIL0, .value = 0},
                                                            {.stringID = IDS_FIL1, .value = VRTF},
                                                            {.stringID = IDS_FIL2, .value = HORF},
                                                            {.stringID = IDS_FIL3, .value = ANGF},
                                                            {.stringID = IDS_FIL4, .value = SATF},
                                                            {.stringID = IDS_FIL5, .value = CLPF},
                                                            {.stringID = IDS_FIL6, .value = CONTF},
                                                            {.stringID = IDS_FIL7, .value = VCLPF},
                                                            {.stringID = IDS_FIL8, .value = HCLPF},
                                                            {.stringID = IDS_FIL9, .value = ANGCLPF},
                                                            {.stringID = IDS_FIL10, .value = FTHF},
                                                            {.stringID = IDS_FIL11, .value = TXVRTF},
                                                            {.stringID = IDS_FIL12, .value = TXHORF},
                                                            {.stringID = IDS_FIL13, .value = TXANGF}}};

constexpr auto EDGELIST =
    std::array<LIST_TYPE, EDGETMAX> {{{.stringID = IDS_EDG0, .value = 0},
                                      {.stringID = IDS_EDG1, .value = EDGELINE},
                                      {.stringID = IDS_EDG2, .value = EDGEBEAN},
                                      {.stringID = IDS_EDG3, .value = EDGECLIP},
                                      {.stringID = IDS_EDG4, .value = EDGEANGSAT},
                                      {.stringID = IDS_EDG5, .value = EDGEAPPL},
                                      {.stringID = IDS_EDG6, .value = EDGEPROPSAT},
                                      {.stringID = IDS_EDG7, .value = EDGEBHOL},
                                      {.stringID = IDS_EDG8, .value = EDGEPICOT},
                                      {.stringID = IDS_EDG9, .value = EDGEDOUBLE},
                                      {.stringID = IDS_EDG10, .value = EDGELCHAIN},
                                      {.stringID = IDS_EDG11, .value = EDGEOCHAIN},
                                      {.stringID = IDS_EDG12, .value = EDGECLIPX}}};

constexpr auto PREFLIST = std::array<LIST_TYPE, PREFTMAX> {
    {{.stringID = IDS_PRF10, .value = PRFAPPCOL}, {.stringID = IDS_PRF29, .value = PRFAPSLEN},
     {.stringID = IDS_PRF3, .value = PRFBRDWID},  {.stringID = IDS_PRF14, .value = PRFBCNLEN},
     {.stringID = IDS_PRF23, .value = PRFCHFLEN}, {.stringID = IDS_PRF24, .value = PRFCHFPOS},
     {.stringID = IDS_PRF21, .value = PRFCLPOFF}, {.stringID = IDS_PRF22, .value = PRFCLPPHS},
     {.stringID = IDS_PRF26, .value = PRFEGGRAT}, {.stringID = IDS_PRF1, .value = PRFFILANG},
     {.stringID = IDS_PRF2, .value = PRFFILEND},  {.stringID = IDS_PRF0, .value = PRFFILSPC},
     {.stringID = IDS_PRF7, .value = PRFGRDCUT},  {.stringID = IDS_PRF20, .value = PRFGRDSIZ},
     {.stringID = IDS_PRF17, .value = PRFHUPTYP}, {.stringID = IDS_PRF27, .value = PRFHUPHGT},
     {.stringID = IDS_PRF18, .value = PRFHUPWID}, {.stringID = IDS_PRF28, .value = PRFLENRAT},
     {.stringID = IDS_PRF25, .value = PRFNUGSTP}, {.stringID = IDS_PRF16, .value = PRFPCTSPC},
     {.stringID = IDS_PRF15, .value = PRFSATEND}, {.stringID = IDS_PRF19, .value = PRFSATUND},
     {.stringID = IDS_PRF9, .value = PRFSMLSTH},  {.stringID = IDS_PRF11, .value = PRFSNPSIZ},
     {.stringID = IDS_PRF13, .value = PRFSPLWRP}, {.stringID = IDS_PRF12, .value = PRFSTRRAT},
     {.stringID = IDS_PRF8, .value = PRFSBXCUT},  {.stringID = IDS_PRF4, .value = PRFSTCMAX},
     {.stringID = IDS_PRF5, .value = PRFSTCUSR},  {.stringID = IDS_PRF6, .value = PRFSTCMIN}}};

// form types
enum FormStyles : int8_t {
  FRMLINE = 1U, // Line form
  FRMFPOLY,     // Freehand polygon form
  FRMRPOLY,     // Regular polygon form
  FRMSTAR,      // Star form
  FRMSPIRAL,    // Spiral Form
  SAT,          // Used when fill is 'feather', 'fan', 'fan clip' or form is daisy
  FRMHEART,     // Heart Form
  FRMLENS,      // Lens form
  FRMEGG,       // Egg form
  FRMTEAR,      // Teardrop form
  FRMZIGZAG,    // Zig-Zag form
  FRMWAVE,      // Wave Form
  FRMDAISY      // Daisy form
};

// clipboard data types
enum ClipDataType : uint8_t { CLP_STCH = 1, CLP_FRM, CLP_FRMS, CLP_FRMPS };

// edge underlay bit
constexpr auto EGUND  = uint8_t {0x80U};
constexpr auto NEGUND = uint8_t {0x7fU};

// form data lines
enum FormData : uint8_t {
  LFRM,       // form 0
  LLAYR,      // layer 1
  LFRMFIL,    // form fill 2
  LFRMCOL,    // form fill color 3
  LFRMSPAC,   // form fill space 4
  LFRMLEN,    // form stitch length 5
  LFRMANG,    // angle of angle fill 6
  LBRD,       // border 7
  LBRDCOL,    // border color 8
  LBRDSPAC,   // border space 9
  LBRDLEN,    // border stitch length 10
  LBRDSIZ,    // border size 11
  LAPCOL,     // applique color 12
  LBCSIZ,     // buttonhole corner size 13
  LBSTRT,     // form start style 14
  LBFIN,      // form end style 15
  LBRDPIC,    // picot border spacing 16
  LBRDUND,    // border underlay 17
  LSACANG,    // angle clipboard angle 18
  LFRMFAZ,    // clipboard phase 19
  LBRDPOS,    // chain position 20
  LBFILSQR,   // square/pointed fill ends 21
  LMAXFIL,    // maximum fill stitch length 22
  LMINFIL,    // minimum fill stitch length 23
  LMAXBRD,    // maximum border stitch length 24
  LMINBRD,    // minimum border stitch length 25
  LFTHCOL,    // feather color 26
  LFTHTYP,    // feather fill type
  LFTHBLND,   // feather blend
  LFTHBTH,    // feather both up and down
  LFTHUP,     // feather up or down
  LFTHUPCNT,  // feather up count
  LFTHDWNCNT, // feather down count
  LFTHSIZ,    // feather size
  LFTHNUM,    // feather number
  LFTHFLR,    // feather floor
  LFSTRT,     // form fill start on/off
  LDSTRT,     // form fill start data
  LFEND,      // form fill end on/off
  LDEND,      // form fill end data
  LCWLK,      // center walk
  LWALK,      // edge walk
  LWLKIND,    // edge walk/underlay indent
  LUND,       // underlay
  LULEN,      // underaly stitch length
  LUANG,      // underlay angle
  LUSPAC,     // underlay spacing
  LUNDCOL,    // underlay color
  LTXOF,      // texture fill spacing
  LASTLIN     // must be the last entry
};

// fill message codes
enum FillMessageCodes : uint16_t {
  FMM_FAN   = 0U,
  FMM_VRT   = 1U,
  FMM_HOR   = 2U,
  FMM_TXTR  = 3U,
  FMM_ANG   = 4U,
  FMM_CLP   = 5U,
  FMM_FTH   = 6U,
  FML_LIN   = 0x100,
  FML_BLD   = 0x101,
  FML_ANGS  = 0x102,
  FML_PRPS  = 0x103,
  FML_APLQ  = 0x104,
  FML_BHOL  = 0x105,
  FML_CLP   = 0x106,
  FML_PIC   = 0x107,
  FMX_UNF   = 0x108,
  FML_CONT  = 0x109,
  FML_CHAIN = 0x10A
};

// button windows. Note that BUTTONCNT should match the number of buttons
enum StitchButtons : uint8_t { HBOXSEL, HUPTO, HHID, HNUM, HTOT, HMINLEN, HMAXLEN, HCOR, HLAYR };

enum TextureButtons : uint8_t { // text button windows
  HTXCLR,
  HTXHI,
  HTXWID,
  HTXSPAC,
  HTXVRT,
  HTXHOR,
  HTXANG,
  HTXMIR
};

enum MachineType : int8_t { AUXPCS, AUXDST, AUXPES };

#ifdef DOUBLE_REQUIRED
class D_RECTANGLE
{
  public:
  double top {};
  double bottom {};
  double left {};
  double right {};

  constexpr D_RECTANGLE() noexcept               = default; // default constructor
  D_RECTANGLE(D_RECTANGLE const&)                = default; // copy constructor
  D_RECTANGLE(D_RECTANGLE&&)                     = default; // move constructor
  D_RECTANGLE& operator=(D_RECTANGLE const& rhs) = default; // copy assignment
  D_RECTANGLE& operator=(D_RECTANGLE&&)          = default; // move assignment
  ~D_RECTANGLE()                                 = default; // destructor
};
#endif

constexpr uint32_t COLMSK    = 0x0000000fU; // 0000 0000 0000 0000 0000 0000 0000 1111
constexpr uint32_t APCOLMSK  = 0x000000f0U; // 0000 0000 0000 0000 0000 0000 1111 0000
constexpr uint32_t NCOLMSK   = 0xfffffff0U; // 1111 1111 1111 1111 1111 1111 1111 0000
constexpr uint32_t COLSMSK   = 0x0000ffffU; // 0000 0000 0000 0000 1111 1111 1111 1111
constexpr uint32_t FRMSK     = 0x00003ff0U; // 0000 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t NFRMSK    = 0xffffc00fU; // 1111 1111 1111 1111 1100 0000 0000 1111
constexpr uint32_t UFRMSK    = 0x80003ff0U; // 1000 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t TYPFMSK   = 0x20003ff0U; // 0010 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t TYPBMSK   = 0x40003ff0U; // 0100 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t TYPAPMSK  = 0x60003ff0U; // 0110 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t LAYMSK    = 0x0e000000U; // 0000 1110 0000 0000 0000 0000 0000 0000
constexpr uint32_t NLAYMSK   = 0xf1ffffffU; // 1111 0001 1111 1111 1111 1111 1111 1111
constexpr uint32_t TYPMSK    = 0x60000000U; // 0110 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t TYPFRM    = 0x20000000U; // 0010 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t TYPBRD    = 0x40000000U; // 0100 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t NTYPMSK   = 0x9fffffffU; // 1001 1111 1111 1111 1111 1111 1111 1111
constexpr uint32_t USMSK     = 0x80000000U; // 1000 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t ATMSK     = 0x7fffffffU; // 0111 1111 1111 1111 1111 1111 1111 1111
constexpr uint32_t NFRM_NTYP = 0x9fffc00fU; // 1001 1111 1111 1111 1100 0000 0000 1111
constexpr uint32_t TYPATMSK  = 0x20000000U; // 0010 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t WLKMSK    = 0x00200000U; // 0000 0000 0010 0000 0000 0000 0000 0000
constexpr uint32_t WLKFMSK   = 0x00203ff0U; // 0000 0000 0010 0000 0011 1111 1111 0000
constexpr uint32_t CWLKMSK   = 0x00100000U; // 0000 0000 0001 0000 0000 0000 0000 0000
constexpr uint32_t CWLKFMSK  = 0x00103ff0U; // 0000 0000 0001 0000 0011 1111 1111 0000
constexpr uint32_t UNDMSK    = 0x00400000U; // 0000 0000 0100 0000 0000 0000 0000 0000
constexpr uint32_t WUNDMSK   = 0x00600000U; // 0000 0000 0110 0000 0000 0000 0000 0000
constexpr uint32_t UNDFMSK   = 0x00403ff0U; // 0000 0000 0100 0000 0011 1111 1111 0000
constexpr uint32_t KNOTMSK   = 0x00800000U; // 0000 0000 1000 0000 0000 0000 0000 0000
constexpr uint32_t NKNOTMSK  = 0xff7fffffU; // 1111 1111 0111 1111 1111 1111 1111 1111
constexpr uint32_t FTHMSK    = 0x01000000U; // 0000 0001 0000 0000 0000 0000 0000 0000
constexpr uint32_t DELMSK    = 0x61e03ff0U; // 0110 0001 1110 0000 0011 1111 1111 0000
constexpr uint32_t ALTYPMSK  = 0x61f00000U; // 0110 0001 1111 0000 0000 0000 0000 0000
constexpr uint32_t SRTMSK    = 0x61f03fffU; // 0110 0001 1111 0000 0011 1111 1111 1111
constexpr uint32_t SRTYPMSK  = 0x61700000U; // 0110 0001 0111 0000 0000 0000 0000 0000
constexpr uint32_t NOTFRM    = 0x00080000U; // 0000 0000 0000 1000 0000 0000 0000 0000
constexpr uint32_t FRMSHFT   = 4U;
constexpr uint32_t LAYSHFT   = 25U; // shift layer data in stitch attribute
constexpr uint32_t FLAYSHFT  = 1U;  // shift layer data in form attribute
constexpr uint32_t TYPSHFT   = 29U; // stitch type if it is not special
constexpr uint32_t USHFT     = 31U; // user edited stitches shift
constexpr uint32_t STSHFT    = 18U; // stitch type definitions begin at 19

/*
bit definitions for F_POINT_ATTR.attribute
0-3		stitch color
4-14	form pointer
15-18	spares
19		not a form stitch
20		center walk stitch
21		edge walk stitch
22		underlay stitch
23		knot	stitch
24		feather stitch
25-27	layer
28		spare
29-30	stitch type 00=not a form stitch, 01=form fill, 10=form border fill, 11=applique stitches
31		set for user edited stitches
*/

constexpr uint32_t FRMFIL   = 0x20000000U; // Form Fill Stitches
constexpr uint32_t FRMBFIL  = 0x40000000U; // Form Border Stitches
constexpr uint32_t FRMAPFIL = 0x60000000U; // Form Applique Stitches

#ifdef NEEDS_FORM_INFO
class FORM_INFO
{
  public:
  uint32_t type {};
  uint32_t attribute {};
  uint32_t sideCount {};

  // constexpr FORM_INFO() noexcept = default
  // FORM_INFO(FORM_INFO const&) = default
  // FORM_INFO(FORM_INFO&&) = default
  // FORM_INFO& operator=(FORM_INFO const& rhs) = default
  // FORM_INFO& operator=(FORM_INFO&&) = default
  //~FORM_INFO() = default
};
#endif

/*form attribute bits

0		end teflon line
1-3		layer bits
5		finish blunt
6		start blunt
7		refill bit for contour fill
*/

constexpr uint8_t FRMEND   = 0x01U; // 0000 0001
constexpr uint8_t NFRMEND  = 0xfeU; // 1111 1110
constexpr uint8_t FRMLMSK  = 0x0eU; // 0000 1110
constexpr uint8_t NFRMLMSK = 0xf1U; // 1111 0001

constexpr auto FRMMMSK = 0xffff0000U;

// blunt bits
constexpr uint8_t FBLNT  = 0x20U; // 0010 0000
constexpr uint8_t SBLNT  = 0x40U; // 0100 0000
constexpr uint8_t NFBLNT = 0xdfU; // 1101 1111
constexpr uint8_t NSBLNT = 0xbfU; // 1011 1111
constexpr uint8_t NOBLNT = 0x9fU; // 1001 1111

// frmhed extended attribute bits

constexpr auto AT_SQR     = 1U;       // square ends
constexpr auto AT_FTHUP   = 1U << 1U; // feather up or down flag
constexpr auto AT_FTHBTH  = 1U << 2U; // feather both up and down flag
constexpr auto AT_FTHBLND = 1U << 3U; // feather blend flag
constexpr auto AT_STRT    = 1U << 4U; // user set start flag
constexpr auto AT_END     = 1U << 5U; // user set end flag
constexpr auto AT_UND     = 1U << 6U; // underlay flag
constexpr auto AT_WALK    = 1U << 7U; // edge walk
constexpr auto AT_CWLK    = 1U << 8U; // center walk

class RANGE
{
  public:
  uint32_t start {};
  uint32_t finish {};
};

class SEARCH_REC
{
  public:
  uint32_t index {};
  float    length {};
};

class INS_REC
{
  public:
  uint32_t code {};
  uint32_t color {};
  uint32_t index {};
  uint32_t seq {};
};

enum InterleaveTypes : uint8_t { // interleave sequence identifiers
  I_AP,
  I_FIL,
  I_FTH,
  I_BRD
};

class RNG_COUNT
{
  public:
  int32_t line {};
  int32_t stitchCount {};
};

class PEC_COLOR
{
  public:
  uint8_t r {};
  uint8_t g {};
  uint8_t b {};
};

// Do the type punning while ensuring that the returned pointer is non_null
// use the encapsulation recommended in I.30
// (https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i30-encapsulate-rule-violations)
template <class T2, class T1> _Ret_notnull_ auto convertFromPtr(T1* pointer) -> T2 {
  if (pointer) {
	return gsl::narrow_cast<T2>(gsl::narrow_cast<void*>(pointer));
  }

  throw std::runtime_error("Could Not cast pointer");
}

#endif // THRED_THRED_TYPES_H
