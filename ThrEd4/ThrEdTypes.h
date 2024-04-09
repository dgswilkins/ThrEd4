// ReSharper disable CppClangTidyClangDiagnosticFloatEqual
#pragma once

// Local Headers
#include "Resources\resource.h"
#include "utf8conv.h"

// Open Source headers
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
#endif

#ifdef _DEBUG
class FMT_WITH_LOC
{
  public:
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  const wchar_t*       strX;
  std::source_location loc;
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  // NOLINTNEXTLINE(google-explicit-constructor,hicpp-explicit-conversions)
  constexpr FMT_WITH_LOC(const wchar_t* fmtString,
                         const std::source_location& fileLoc = std::source_location::current()) noexcept :
      strX(fmtString),
      loc(fileLoc) {
  }
};

template <typename... Args> constexpr void outDebugString(FMT_WITH_LOC fwl, Args&&... args) {
  auto       name = utf::utf8ToUtf16(std::string(fwl.loc.file_name()));
  auto       line = fwl.loc.line();
  auto const strY = fmt::format(FMT_COMPILE(L" {}({}) : {}"), name, line, fwl.strX);
  auto const strZ = fmt::format(fmt::runtime(strY), std::forward<Args>(args)...);
  OutputDebugString(strZ.c_str());
}
#else
template <typename... Args>
constexpr void outDebugString([[maybe_unused]] const wchar_t* value, [[maybe_unused]] Args... args) {
}
#endif

constexpr float DEFUSPAC = 6.0F;        // default underlay stitch spacing
constexpr float DEFLRAT  = 0.8F;        // default lens form aspect ratio
constexpr uint32_t MAXITEMS = 0x10000U; // maximum number of stitches, sequence items & clipboard points
constexpr float SHUPX    = 480.0F;      // small hoop x size
constexpr float SHUPY    = 480.0F;      // small hoop y size
constexpr float LHUPX    = 719.0F;      // large hoop x size
constexpr float LHUPY    = 690.0F;      // large hoop y size
constexpr float HUP100XY = 600.0F;      // 100 millimeter hoop size
constexpr float PFGRAN   = 6.0F;        // pfaf stitch points per millimeter
constexpr float IPFGRAN  = 1.0F / 6.0F; // pfaf millimeters per stitch point
// ToDo - Should this be a configurable parameter?
constexpr double CLOSENUF = 15.0; // mouse click region for select
constexpr auto   FCLOSNUF = float {CLOSENUF};
constexpr float  SMALSIZ  = 0.25F;                     // default small stitch size
constexpr float  MINSIZ   = 0.1F;                      // default minimum stitch size
constexpr float  USESIZ   = 3.5F;                      // user preferred size
constexpr auto   MAXSIZ   = 9.0F;                      // default maximum stitch size
constexpr float  SHOPNTS  = 0.00F;                     // show stitch points when zoom below this
constexpr float  STCHBOX  = 0.4226F;                   // show stitch boxes when zoom below this
constexpr float  DEFSPACE = 0.45F;                     // default stitch spacing
constexpr float  PI_F     = std::numbers::pi_v<float>; // PI to single precision
constexpr float  PI_F2    = PI_F * 2.0F;               //
constexpr float  PI_FHALF = PI_F / 2.0F;               //
constexpr double PI_D     = std::numbers::pi;          // PI to double precision
constexpr double PI_D2    = PI_D * 2.0;                //
constexpr float  USPAC    = 15.0F;                     // underlay fill spacing
constexpr float  URAT     = 0.75F; // ratio of underlay stitch to satin border size

constexpr float    TNYFLOAT = 1e-9F;           // tiny number for single precision float stuff
constexpr float    BIGFLOAT = 1e9F;            // large number for single precision float stuff
constexpr double   BIGDBL   = 1e99;            // large number for double precision float stuff
constexpr float    IBFCLEN  = (4.0F * PFGRAN); // initial buttonhole fill corner length
constexpr float    IPICSPAC = 6.0F;            // initial picot border space
constexpr int32_t  PRFLINS  = 30;              // number of lines on the preference menu
constexpr uint32_t SEED     = 3037000499U;     // pseudo-random-sequence seed
constexpr float    BRDWID   = 18.0F;           // default satin border size
constexpr float    SNPLEN   = 0.15F;           // default snap together length size
constexpr float    STARAT   = 0.4F;            // default star ratio
constexpr float    SPIRWRAP = 1.52F;           // default spiral wrap
constexpr int32_t  HBUFSIZ  = 1024;            // help buffer size
constexpr float    ITXHI    = (9.0F * PFGRAN); // default texture editor height
constexpr float    ITXWID   = (9.0F * PFGRAN); // default texture editor width
constexpr float    ITXSPAC  = (0.4F * PFGRAN); // default texture editor spacing
constexpr uint16_t ITXPIX   = 5U;              // default texture editor cross pixels
constexpr uint32_t BTNCOUNT = 9U;              // Maximum number of buttons
constexpr int32_t  FONTSIZE = 400;             // default font size
constexpr uint32_t B1MASK   = 0x000000ffU;     // mask for the least significant byte
constexpr uint32_t B2MASK   = 0x0000ff00U;     // mask for the middle byte
constexpr uint32_t B3MASK   = 0x00ff0000U;     // mask for the more significant byte
constexpr uint32_t BYTSHFT  = 8U;              // bits required to shift one byte left
constexpr uint32_t WRDSHFT  = 16U;             // bits required to shift two bytes left
constexpr uint32_t TBYTSHFT = 24U;             // bits required to shift three bytes left
constexpr uint32_t HBSHFT   = 31U;             // shift required to reach the MSB of uint32
constexpr uint8_t  NIBMASK  = 0x0fU;           // Nibble mask
constexpr uint8_t  BYTMASK  = 0xffU;           // Byte mask
constexpr uint16_t WRDMASK  = 0xffffU;         // word mask
constexpr uint32_t COLMASK  = 0xffffffU;       // COLORREF mask
constexpr uint8_t  MSK7BITS = 0x7fU;           // mask value to 7 bits
constexpr uint8_t  MSK3BITS = 0x7U;            // mask value to 3 bits

constexpr uint32_t LNPNTS  = 2U;            // number of points required to draw a line
constexpr uint32_t TRIPNTS = 4U;            // number of points required to draw a triangle or arrow
constexpr uint32_t SQPNTS  = 5U;            // number of points required to draw a square
constexpr uint32_t OUTPNTS = 9U;            // number of points required to draw an outline
constexpr auto     RADDEGF = 180.0F / PI_F; // float factor to convert radians to degrees
constexpr auto     DEGRADF = PI_F / 180.0F; // float factor to convert degrees to radians

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
constexpr auto STDDPI   = int32_t {96};         // Default DPI

constexpr auto FRACFACT = 256.0F; // float factor to convert the fractional part to/from the lower byte
constexpr auto HALF   = 0.5F;     // factor for halving
constexpr auto DBLF   = 2.0F;     // factor for doubling
constexpr auto DECRAD = 10;       // decimal radix

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
enum MainMenuItems : int8_t {
  M_FILE,
  M_VIEW,
  M_FORM,
  M_EDIT,
  M_IN,
  M_OUT,
  M_UNDO,
  M_REDO,
  M_ROT,
  M_PREF,
  M_FILL,
  M_ADD,
  M_FRM,
  M_ALL,
  M_1,
  M_2,
  M_3,
  M_4,
  M_HELP
};

// fill menu items
enum FillMenuItems : int8_t {
  MFIL_SAT,
  MFIL_FTH,
  MFIL_VERT,
  MFIL_HOR,
  MFIL_ANG,
  MFIL_CLP,
  MFIL_CONT,
  MFIL_TED,
  MFIL_BORD,
  MFIL_UNFIL
};

// view menu items
enum ViewMenuItems : int8_t {
  MVW_MOVIE,
  MVW_SET,
  MVW_BACK,
  MVW_ZOOM,
  MVW_THRD,
  MVW_SHWT,
  MVW_SHWTCOL,
  MVW_DSGN,
  MVW_KNOT,
  MVW_MARK,
  MVW_ABOUT
};

enum HoopSize : int8_t {
  SETCUST = 1, // set the custom hoop
  SMALHUP = 2, // pfaf code for small hoop
  LARGHUP,     // pfaf code for large hoop
  HUP100,      // 100 millimeter hoop
  CUSTHUP      // user defined hoop size
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

  // LIST_TYPE() noexcept = default;
  // LIST_TYPE(LIST_TYPE const&) = default;
  // LIST_TYPE(LIST_TYPE&&) = default;
  // LIST_TYPE& operator=(LIST_TYPE const& rhs) = default;
  // LIST_TYPE& operator=(LIST_TYPE&&) = default;
  //~LIST_TYPE() = default;
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
  SATF,     // Fan fill ?
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

// clang-format off
constexpr auto LAYRLIST = std::array<LIST_TYPE, LAYERMAX> {{{IDS_LAY00, LAYER0}, 
														  {IDS_LAY01, LAYER1}, 
														  {IDS_LAY02, LAYER2}, 
														  {IDS_LAY03, LAYER3},
														  {IDS_LAY04, LAYER4}}};

constexpr auto FTHRLIST = std::array<LIST_TYPE, FSTYLMAX> {{{IDS_FTH0, FTHSIN}, 
														  {IDS_FTH1, FTHSIN2}, 
														  {IDS_FTH2, FTHLIN}, 
														  {IDS_FTH3, FTHPSG}, 
														  {IDS_FTH4, FTHRMP}, 
														  {IDS_FTH5, FTHFAZ}}};

constexpr auto FILLLIST = std::array<LIST_TYPE, FILLTMAX> {{{IDS_FIL0,  0},
                                                          {IDS_FIL1,  VRTF},
                                                          {IDS_FIL2,  HORF},
                                                          {IDS_FIL3,  ANGF},
                                                          {IDS_FIL4,  SATF},
                                                          {IDS_FIL5,  CLPF},
                                                          {IDS_FIL6,  CONTF},
                                                          {IDS_FIL7,  VCLPF},
                                                          {IDS_FIL8,  HCLPF},
                                                          {IDS_FIL9,  ANGCLPF},
                                                          {IDS_FIL10, FTHF},
                                                          {IDS_FIL11, TXVRTF},
                                                          {IDS_FIL12, TXHORF},
                                                          {IDS_FIL13, TXANGF}}};

constexpr auto EDGELIST = std::array<LIST_TYPE, EDGETMAX> {{{IDS_EDG0,  0},
                                                          {IDS_EDG1,  EDGELINE},
                                                          {IDS_EDG2,  EDGEBEAN},
                                                          {IDS_EDG3,  EDGECLIP},
                                                          {IDS_EDG4,  EDGEANGSAT},
                                                          {IDS_EDG5,  EDGEAPPL},
                                                          {IDS_EDG6,  EDGEPROPSAT},
                                                          {IDS_EDG7,  EDGEBHOL},
                                                          {IDS_EDG8,  EDGEPICOT},
                                                          {IDS_EDG9,  EDGEDOUBLE},
                                                          {IDS_EDG10, EDGELCHAIN},
                                                          {IDS_EDG11, EDGEOCHAIN},
                                                          {IDS_EDG12, EDGECLIPX}}};

constexpr auto PREFLIST = std::array<LIST_TYPE, PREFTMAX> {{{IDS_PRF10, PRFAPPCOL},
														  {IDS_PRF29, PRFAPSLEN},
														  {IDS_PRF3,  PRFBRDWID},
														  {IDS_PRF14, PRFBCNLEN},
														  {IDS_PRF23, PRFCHFLEN},
														  {IDS_PRF24, PRFCHFPOS},
														  {IDS_PRF21, PRFCLPOFF},
														  {IDS_PRF22, PRFCLPPHS},
														  {IDS_PRF26, PRFEGGRAT},
														  {IDS_PRF1,  PRFFILANG},
														  {IDS_PRF2,  PRFFILEND},
														  {IDS_PRF0,  PRFFILSPC},
														  {IDS_PRF7,  PRFGRDCUT},
														  {IDS_PRF20, PRFGRDSIZ},
														  {IDS_PRF17, PRFHUPTYP},
														  {IDS_PRF27, PRFHUPHGT},
														  {IDS_PRF18, PRFHUPWID},
														  {IDS_PRF28, PRFLENRAT},
														  {IDS_PRF25, PRFNUGSTP},
														  {IDS_PRF16, PRFPCTSPC},
														  {IDS_PRF15, PRFSATEND},
														  {IDS_PRF19, PRFSATUND},
														  {IDS_PRF9,  PRFSMLSTH},
														  {IDS_PRF11, PRFSNPSIZ},
														  {IDS_PRF13, PRFSPLWRP},
														  {IDS_PRF12, PRFSTRRAT},
														  {IDS_PRF8,  PRFSBXCUT},
														  {IDS_PRF4,  PRFSTCMAX},
														  {IDS_PRF5,  PRFSTCUSR},
														  {IDS_PRF6,  PRFSTCMIN}}};
// clang-format on

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
  LBRDPIC,    // pico border spacing 16
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
  FMM_FAN,
  FMM_VRT,
  FMM_HOR,
  FMM_TXTR,
  FMM_ANG,
  FMM_CLP,
  FMM_FTH,
  FML_LIN = 0x100,
  FML_BLD,
  FML_ANGS,
  FML_PRPS,
  FML_APLQ,
  FML_BHOL,
  FML_CLP,
  FML_PIC,
  FMX_UNF,
  FML_CONT,
  FML_CHAIN
};

// button windows
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

/*
class D_RECTANGLE
{
  public:
  double top {};
  double bottom {};
  double left {};
  double right {};

  // constexpr D_RECTANGLE() noexcept = default;
  // D_RECTANGLE(D_RECTANGLE const&) = default;
  // D_RECTANGLE(D_RECTANGLE&&) = default;
  // D_RECTANGLE& operator=(D_RECTANGLE const& rhs) = default;
  // D_RECTANGLE& operator=(D_RECTANGLE&&) = default;
  //~D_RECTANGLE() = default;
};
*/

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

/*
class FORM_INFO
{
  public:
  uint32_t type {};
  uint32_t attribute {};
  uint32_t sideCount {};

  // constexpr FORM_INFO() noexcept = default;
  // FORM_INFO(FORM_INFO const&) = default;
  // FORM_INFO(FORM_INFO&&) = default;
  // FORM_INFO& operator=(FORM_INFO const& rhs) = default;
  // FORM_INFO& operator=(FORM_INFO&&) = default;
  //~FORM_INFO() = default;
};
*/

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

  // constexpr RANGE() noexcept = default;
  // RANGE(RANGE const&) = default;
  // RANGE(RANGE&&) = default;
  // RANGE& operator=(RANGE const& rhs) = default;
  // RANGE& operator=(RANGE&&) = default;
  //~RANGE() = default;
};

class INS_REC
{
  public:
  uint32_t code {};
  uint32_t color {};
  uint32_t index {};
  uint32_t seq {};

  // constexpr INS_REC() noexcept = default;
  // INS_REC(INS_REC const&) = default;
  // INS_REC(INS_REC&&) = default;
  // INS_REC& operator=(INS_REC const& rhs) = default;
  // INS_REC& operator=(INS_REC&&) = default;
  //~INS_REC() = default;
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

  // constexpr RNG_COUNT() noexcept = default;
  // RNG_COUNT(RNG_COUNT const&) = default;
  // RNG_COUNT(RNG_COUNT&&) = default;
  // RNG_COUNT& operator=(RNG_COUNT const& rhs) = default;
  // RNG_COUNT& operator=(RNG_COUNT&&) = default;
  //~RNG_COUNT() = default;
};

class PEC_COLOR
{
  public:
  uint8_t r {};
  uint8_t g {};
  uint8_t b {};

  // constexpr PEC_COLOR() noexcept = default;
  // PEC_COLOR(PEC_COLOR const&) = default;
  // PEC_COLOR(PEC_COLOR&&) = default;
  // PEC_COLOR& operator=(PEC_COLOR const& rhs) = default;
  // PEC_COLOR& operator=(PEC_COLOR&&) = default;
  //~PEC_COLOR() = default;
};

// Do the type punning while ensuring that the returned pointer is non_null
// use the encapsulation recommended in I.30
// (https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i30-encapsulate-rule-violations)
template <class T2, class T1> inline _Ret_notnull_ auto convertFromPtr(T1* pointer) -> T2 {
  if (pointer) {
	return gsl::narrow_cast<T2>(gsl::narrow_cast<void*>(pointer));
  }

  throw std::runtime_error("Could Not cast pointer");
}
