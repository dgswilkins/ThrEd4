
#pragma once

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "switches.h"

#ifdef _DEBUG
#define outDebugString(X, ...) OutputDebugString(fmt::format(X, __VA_ARGS__).c_str())
#else
#define outDebugString(X, ...)
#endif

constexpr auto RES_SIZE = 26; // reserved for expansion in the ThrEd v1.0 header
constexpr auto NAME_LEN = 50; // Length of the name fields in ThrEd headers
constexpr auto COLORCNT = gsl::narrow_cast<uint8_t>(16U);           // Number of colors in arrays
constexpr auto COLORMAX = gsl::narrow_cast<uint8_t>(COLORCNT - 1U); // max index in color arrays
constexpr auto COLORBTS = 0xfU;     // Number of bits to cover the number of the colors in arrays
constexpr auto SRTIM    = 20000000; // sort time limit in 100 ns intervals

// daisy codes
constexpr auto DAZPETS = 5U;  // petals
constexpr auto DAZCNT  = 10U; // petal points
constexpr auto DAZICNT = 2U;  // petal inner points
constexpr auto DAZLEN  = 15U; // diameter
constexpr auto DAZPLEN = 20U; // petal length
constexpr auto DAZHLEN = 5U;  // hole size
constexpr auto DAZTYP  = 5U;  // border type
constexpr auto DAZMCNT = 7U;  // mirror count
// end of daisy codes

constexpr float    TXTRAT   = 0.95F;       // texture fill clipboard shrink/grow ratio
constexpr uint32_t MAXMSK   = 0xffff0000U; // for checking for greater than 65536
constexpr uint16_t DEFBPIX  = 4U;          // default form box pixels
constexpr float    MAXWLK   = 54.0F;       // max underlay/edge walk stitch length
constexpr float    MINWLK   = 2.4F;        // max underlay/edge walk stitch length
constexpr float    DEFULEN  = 12.0F;       // default underlay stitch length
constexpr float    DEFUSPAC = 6.0F;        // default underlay stitch spacing
constexpr float    DEFLRAT  = 0.8F;        // default lens form aspect ratio
constexpr uint16_t IWAVPNTS = 36U;         // default wave points
constexpr uint16_t IWAVSTRT = 10U;         // default wave start
constexpr uint16_t IWAVEND  = 26U;         // default wave end
constexpr uint16_t IWAVS    = 5U;          // default wave lobes
constexpr uint32_t THRLED0  = 0x746872U;   // lead dword value for thred file v 1.0
constexpr uint32_t THRLED1  = 0x1746872U;  // lead dword value for thred file v 1.1
constexpr float    ZUMFCT   = 0.65F;       // zoom factor
constexpr float    PAGSCROL = 0.9F;        // page scroll factor
constexpr float    LINSCROL = 0.05F;       // line scroll factor
constexpr uint32_t MAXITEMS = 0x10000U; // maximum number of stitches, sequence items & clipboard points
constexpr uint32_t MAXCHNG  = 10000U;      // maximum number of color changes
constexpr float    SHUPX    = 480.0F;      // small hoop x size
constexpr float    SHUPY    = 480.0F;      // small hoop y size
constexpr float    LHUPX    = 719.0F;      // large hoop x size
constexpr float    LHUPY    = 690.0F;      // large hoop y size
constexpr float    HUP100XY = 600.0F;      // 100 millimeter hoop size
constexpr float    PFGRAN   = 6.0F;        // pfaf stitch points per millimeter
constexpr float    IPFGRAN  = 1.0F / 6.0F; // pfaf millimeters per stitch point
constexpr float    TSIZ30   = 0.3F;        // #30 thread size in millimeters
constexpr float    TSIZ40   = 0.2F;        // #40 thread size in millimeters
constexpr float    TSIZ60   = 0.05F;       // #60 thread size in millimeters
constexpr int32_t  SCROLSIZ = 12;          // logical pixel width of a scroll bar
constexpr int32_t  COLSIZ   = 12;          // logical pixel width of the color bar
// ToDo - Should this be a configurable parameter?
constexpr double   CLOSENUF = 15.0; // mouse click region for select
constexpr auto     FCLOSNUF = gsl::narrow_cast<float>(CLOSENUF);
constexpr float    ZMARGIN  = 1.25F;             // zoom margin for select zooms
constexpr float    SMALSIZ  = 0.25F;             // default small stitch size
constexpr float    MINSIZ   = 0.1F;              // default minimum stitch size
constexpr float    USESIZ   = 3.5F;              // user preferred size
constexpr auto     MAXSIZ   = 9.0F;              // default maximum stitch size
constexpr int32_t  MINZUM   = 5;                 // minimum zoom in stitch points
constexpr float    SHOPNTS  = 0.00F;             // show stitch points when zoom below this
constexpr float    STCHBOX  = 0.4226F;           // show stitch boxes when zoom below this
constexpr uint32_t BITCOL   = 0xffff00U;         // default bitmap color
constexpr uint32_t MAXFORMS = 1024U;             // maximum number of forms
constexpr double   FORMFCT  = 0.05;              // new forms part of screen
constexpr int32_t  MAXDELAY = 600;               // maximum movie time step
constexpr int32_t  MINDELAY = 1;                 // minimum movie time step
constexpr int32_t  MOVITIM  = 12;                // default movie time
constexpr float    DEFSPACE = 0.45F;             // default stitch spacing
constexpr float    DEFANG   = 0.7853981F;        // default fill angle, 45 degrees
constexpr uint32_t MSGSIZ   = 8192U;             // size of the message buffer
constexpr float    PI_F     = 3.1415927F;        // PI to single precision
constexpr float    PI_F2    = PI_F * 2.0F;       //
constexpr float    PI_FHALF = PI_F / 2.0F;       //
constexpr double   PI       = 3.141592653589793; // PI to double precision
constexpr double   PI2      = PI * 2.0;          //
constexpr float    USPAC    = 15.0F;             // underlay fill spacing
constexpr float    APSPAC   = 10.8F;             // applique border spacing
constexpr uint32_t OSEQLEN  = 0x40000U;          // output sequence length
constexpr uint32_t BSEQLEN  = OSEQLEN * 2U;      // reverse sequence length
constexpr float    URAT     = 0.75F;             // ratio of underlay stitch to satin border size
constexpr double   PURAT    = 0.6;               // for proportional satin corners

constexpr float DIURAT = (1.0F - URAT) / 2.0F;        //(1-URAT)/2
constexpr float DOURAT = (1.0F - URAT) / 2.0F + URAT; //(1-URAT)/2+URAT

constexpr float    MINRCT   = 12.0F;           // minimum dimension of a form select rectangle
constexpr uint32_t OLDNUM   = 4U;              // number of old filenames saved on file menu
constexpr float    TNYFLOAT = 1e-9F;           // tiny number for single precision float stuff
constexpr float    BIGFLOAT = 1e9F;            // large number for single precision float stuff
constexpr double   BIGDBL   = 1e99;            // large number for double precision float stuff
constexpr int32_t  SPEDLIN  = 30;              // speed change for line message on speed scroll bar
constexpr int32_t  SPEDPAG  = 120;             // speed change for page message on speed scroll bar
constexpr int32_t  KNOTLEN  = 54;              // set knots for stitches longer than this
constexpr int32_t  MAXKNOTS = 16384;           // maximum number of knots
constexpr float    IBFCLEN  = (4.0F * PFGRAN); // initial buttonhole fill corner length
constexpr float    IPICSPAC = 6.0F;            // initial picot border space
constexpr int32_t  PRFLINS  = 30;              // number of lines on the preference menu
constexpr uint32_t SEED     = 3037000499U;     // pseudo-random-sequence seed
constexpr uint32_t FSED     = 1340007303U;     // feather sequence seed
constexpr uint32_t NORDSED  = 0x5a5a5a5aU;     // name order seed
constexpr uint32_t NCODSED  = 0x73ef5a7eU;     // name encoding seed
constexpr uint8_t  NCODOF   = 80U;             // name encoding offset
constexpr auto     CLPMIN   = 0.5F;      // if clipboard data width less than this, then don't fill
constexpr float    CLPMINVT = 1.2F;      // Minimum clip width for skinny vertical clips
constexpr float    BRDWID   = 18.0F;     // default satin border size
constexpr float    SNPLEN   = 0.15F;     // default snap together length size
constexpr float    STARAT   = 0.4F;      // default star ratio
constexpr float    SPIRWRAP = 1.52F;     // default spiral wrap
constexpr uint8_t  BALNORM  = 0x80U;     // normal balarad stitch
constexpr uint8_t  BALJUMP  = 0x81U;     // balarad jump stitch
constexpr uint8_t  BALSTOP  = 0U;        // balarad stop
constexpr uint32_t COLVER   = 0x776874U; // color file version
constexpr uint32_t REDCOL   = 0x0000ffU; // code for the color red
constexpr uint32_t GRNCOL   = 0x00ff00U; // code for the color green
constexpr uint32_t BLUCOL   = 0xff0000U; // code for the color blue
constexpr uint32_t REDMSK   = 0xffff00U; // mask for the color red
constexpr uint32_t GRNMSK   = 0xff00ffU; // mask for the color green
constexpr uint32_t BLUMSK   = 0x00ffffU; // mask for the color blue
constexpr float    TRACLEN  = 1.0F;      // initial trace length
constexpr float    TRACRAT  = 1.00001F;  // initial trace ratio
constexpr float    CHSDEF   = 24.0F;     // default chain stitch length
constexpr double   CHRDEF   = 0.25;      // default chain stitch ratio
constexpr float    NUGINI   = 2.0F;      // default nudge step
constexpr uint16_t DEFPIX   = 2U;        // default nudge pixels
constexpr double   DEFEGRAT = 1.5;       // default egg ratio
constexpr uint16_t DEFPNTPX = 4U;        // default form and stitch point pixels
constexpr int32_t  HBUFSIZ  = 1024;      // help buffer size
constexpr uint32_t HIGRD    = 0xffffffU; // grid high color
constexpr uint32_t MEDGRD   = 0x404040U; // grid medium color
constexpr uint32_t DEFGRD   = 0x202020U; // grid default color
constexpr uint32_t REDGRD   = 0xff2020U; // grid red color
constexpr uint32_t BLUGRD   = 0x20ff20U; // grid green color
constexpr uint32_t GRNGRD   = 0x2020ffU; // grid blue color
constexpr float    FDEFRAT  = 0.6F;      // default feather ratio
constexpr uint8_t  FDEFUP   = 10U;       // default feather up count
constexpr uint8_t  FDEFDWN  = 5;         // default feather down count
constexpr float    FDEFFLR  = 9.0F;      // default feather floor
constexpr uint16_t FDEFNUM  = 10U;       // default feather number
constexpr float    ITXHI    = (9.0F * PFGRAN); // default texture editor height
constexpr float    ITXWID   = (9.0F * PFGRAN); // default texture editor width
constexpr float    ITXSPAC  = (0.4F * PFGRAN); // default texture editor spacing
constexpr uint16_t ITXPIX   = 5U;              // default texture editor cross pixels
constexpr uint32_t ITXBUFSZ = 16U;             // texture buffer depth
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

constexpr auto SWBLEN  = 11U; // Side Window buffer length including the zero terminator
constexpr auto SWCOUNT = 16U; // number of side windows to create/track

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
constexpr auto MAXLAYER = 5U;                   // number of layers
constexpr auto SQRCORNS = 4U;                   // number of corners in a square

constexpr auto FRACFACT = 256.0F; // float factor to convert the fractional part to/from the lower byte
constexpr auto SZBMPNM = 16U;     // THR spec for BMP filename length

enum stringLabel {
  STR_PIKOL,
  STR_UPON,
  STR_UPOF,
  STR_AUXTXT,
  STR_HUP0,
  STR_HUP1,
  STR_HUP2,
  STR_HUP3,
  STR_HUP4,
  STR_TRC0,
  STR_TRC1S,
  STR_TRC2,
  STR_TRC3,
  STR_TRC4,
  STR_TRC1H,
  STR_NUMPNT,
  STR_NUMFRM,
  STR_NUMSCH,
  STR_NUMFORM,
  STR_NUMSEL,
  STR_BOXSEL,
  STR_CLOS,
  STR_TOT,
  STR_LAYR,
  STR_OVRLOK,
  STR_OVRIT,
  STR_THRED,
  STR_NUFIL,
  STR_EMB,
  STR_ON,
  STR_OFF,
  STR_FMEN,
  STR_TXT0,
  STR_TXT1,
  STR_TXT2,
  STR_TXT3,
  STR_TXT4,
  STR_TXT5,
  STR_TXT6,
  STR_TXT7,
  STR_TXT8,
  STR_TXT9,
  STR_TXT10,
  STR_TXT11,
  STR_TXT12,
  STR_TXT13,
  STR_TXT14,
  STR_TXT15,
  STR_TXT16,
  STR_TXT17,
  STR_TXT18,
  STR_TXT19,
  STR_TXT20,
  STR_TXT21,
  STR_TXT22,
  STR_TXT23,
  STR_FIL0,
  STR_FIL1,
  STR_FIL2,
  STR_FIL3,
  STR_FIL4,
  STR_FIL5,
  STR_FIL6,
  STR_FIL7,
  STR_FIL8,
  STR_FIL9,
  STR_FIL10,
  STR_FIL11,
  STR_FIL12,
  STR_FIL13,
  STR_EDG0,
  STR_EDG1,
  STR_EDG2,
  STR_EDG3,
  STR_EDG4,
  STR_EDG5,
  STR_EDG6,
  STR_EDG7,
  STR_EDG8,
  STR_EDG9,
  STR_EDG10,
  STR_EDG11,
  STR_EDG12,
  STR_PRF0,
  STR_PRF1,
  STR_PRF2,
  STR_PRF3,
  STR_PRF4,
  STR_PRF5,
  STR_PRF6,
  STR_PRF7,
  STR_PRF8,
  STR_PRF9,
  STR_PRF10,
  STR_PRF11,
  STR_PRF12,
  STR_PRF13,
  STR_PRF14,
  STR_PRF15,
  STR_PRF16,
  STR_PRF17,
  STR_PRF18,
  STR_PRF19,
  STR_PRF20,
  STR_PRF21,
  STR_PRF22,
  STR_PRF23,
  STR_PRF24,
  STR_PRF25,
  STR_PRF26,
  STR_PRF27,
  STR_PRF28,
  STR_PRF29,
  STR_FRMPLUS,
  STR_FRMINUS,
  STR_OKENT,
  STR_CANCEL,
  STR_FREH,
  STR_BLUNT,
  STR_TAPR,
  STR_PNTD,
  STR_SQR,
  STR_DELST2,
  STR_THRDBY,
  STR_STCHOUT,
  STR_STCHS,
  STR_FORMS,
  STR_HUPWID,
  STR_CREATBY,
  STR_CUSTHUP,
  STR_STCHP,
  STR_FRMP,
  STR_ENTROT,
  STR_NUDG,
  STR_ALLX,
  STR_FTHCOL,
  STR_FTHTYP,
  STR_FTH0,
  STR_FTH1,
  STR_FTH2,
  STR_FTH3,
  STR_FTH4,
  STR_FTH5,
  STR_FTHBLND,
  STR_FTHUP,
  STR_FTHBOTH,
  STR_FTHUPCNT,
  STR_FTHDWNCNT,
  STR_FTHSIZ,
  STR_FTHNUM,
  STR_FTHFLR,
  STR_FSTRT,
  STR_FEND,
  STR_WALK,
  STR_UWLKIND,
  STR_UND,
  STR_ULEN,
  STR_FUANG,
  STR_FUSPAC,
  STR_CWLK,
  STR_UNDCOL,
  STR_FRMBOX,
  STR_TXOF,
  STR_LEN, // must be last entry in enum
};

// edge tracing directions
enum traceDirection {
  TRCU, // top edge
  TRCR, // right edge
  TRCD, // bottom edge
  TRCL  // left edge
};

enum hoopSize {
  SETCUST = 1, // set the custom hoop
  SMALHUP = 2, // pfaf code for small hoop
  LARGHUP,     // pfaf code for large hoop
  HUP100,      // 100 millimeter hoop
  CUSTHUP      // user defined hoop size
};

enum daisyStyles { // daisy form types
  DSIN,            // Sine shape
  DRAMP,           // Ramp shape
  DSAW,            // Sawtooth shape
  DRAG,            // Ragged shape
  DCOG,            // Cog shape
  DHART            // Heart shape
};

// bitmap
enum class StateFlag : uint32_t {
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
  THUMON,    // user is loading a thumnail
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

  EnumCount // must be the last entry in the enum
};

// user bitmap
enum class UserFlag : uint32_t {
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

enum featherStyles { // feather fill types
  FTHSIN = 1,        // sine
  FTHSIN2,           // half sine
  FTHLIN,            // line
  FTHPSG,            // psuedo-random sequence
  FTHRMP,            // sawtooth
  FTHFAZ             // phase
};

constexpr int32_t FDEFTYP = FTHPSG; // default feather type

// edge fill types
enum edgeFillStyles : uint8_t {
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
  EDGECLIPX,     // Even Clipboard
  EDGELAST       // must always be the last value
};

// form types
enum formStyles {
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
  FRMDAISY,     // Daisy form
};

// main menu items
enum mainMenuItems {
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
  M_HELP,
};

// view menu items
enum viewMenuItems {
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
  MVW_ABOUT,
};

// fill menu items
enum fillMenuItems {
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

// clipboard data types
enum clipDataType {
  CLP_STCH = 1,
  CLP_FRM,
  CLP_FRMS,
  CLP_FRMPS,
};

// edge underlay bit
constexpr auto EGUND  = uint8_t {0x80U};
constexpr auto NEGUND = uint8_t {0x7fU};

// form data lines
enum formData {
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

// fill types
enum fillStyles {
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
  TXANGF,   // Angle texture fill
};

constexpr auto MCLPF    = 1U << static_cast<uint32_t>(CLPF);
constexpr auto MVCLPF   = 1U << static_cast<uint32_t>(VCLPF);
constexpr auto MHCLPF   = 1U << static_cast<uint32_t>(HCLPF);
constexpr auto MANGCLPF = 1U << static_cast<uint32_t>(ANGCLPF);

// preference window
enum preferenceItems {
  PSPAC,  // 0
  PANGL,  // 1
  PSQR,   // 2
  PSAT,   // 3
  PMAX,   // 4
  PUSE,   // 5
  PMIN,   // 6
  PSHO,   // 7
  PBOX,   // 8
  PSMAL,  // 9
  PAP,    // 10
  PSNP,   // 11
  PSTAR,  // 12
  PSPIR,  // 13
  PBUT,   // 14
  PBLNT,  // 15
  PPIC,   // 16
  PHUP,   // 17
  PHUPX,  // 18
  PUND,   // 19
  PGRD,   // 20
  PCLPOF, // 21
  PFAZ,   // 22
  PCHN,   // 23
  PCHRAT, // 24
  PNUDG,  // 25
  PEG,    // 26
  PHUPY,  // 27
  PLRAT,  // 28
  PAPSL,  // 29
};

// file menu items
enum fileMenuItems {
  FM_NEW,
  FM_OPEN,
  FM_CLOS,
  FM_THUMB,
  FM_OPNPCS,
  FM_INSRT,
  FM_OVRLAY,
  FM_SAV,
  FM_SAVAS,
  FM_LODBIT,
  FM_SAVBIT,
  FM_HIDBIT,
  FM_RMVBIT,
  FM_PURG,
  FM_LOCK,
  FM_ONAM0,
  FM_ONAM1,
  FM_ONAM2,
  FM_ONAM3
};

// fill message codes
enum fillMessageCodes {
  FMM_FAN,
  FMM_VRT,
  FMM_HOR,
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
  FML_CHAIN,
};

// button windows
enum stitchButtons { HBOXSEL, HUPTO, HHID, HNUM, HTOT, HMINLEN, HMAXLEN, HCOR, HLAYR };

enum textureButtons { // text button windows
  HTXCLR,
  HTXHI,
  HTXWID,
  HTXSPAC,
  HTXVRT,
  HTXHOR,
  HTXANG,
  HTXMIR,
};

constexpr uint32_t HLIN = HNUM;

#pragma pack(push, 1)

class INIFILE // ini file structure
{
  public:
  char     defaultDirectory[180] {0};                // default directory
  COLORREF stitchColors[COLORCNT] {0U};              // colors
  COLORREF stitchPreferredColors[COLORCNT] {0U};     // stitch preference colors
  COLORREF backgroundPreferredColors[COLORCNT] {0U}; // background preference colors
  COLORREF backgroundColor {0U};                     // background color
  COLORREF bitmapColor {0U};                         // bitmap color
  float    minStitchLength {0.0F};                   // minimum stitch length
  float    showStitchThreshold {0.0F};               // show stitch points
  float    threadSize30 {0.0F};                      // millimeter size of 30 weight thread
  float    threadSize40 {0.0F};                      // millimeter size of 40 weight thread
  float    threadSize60 {0.0F};                      // millimeter size of 60 weight thread
  float    userStitchLength {0.0F};                  // user stitch length
  float    maxStitchLength {0.0F};                   // maximum stitch length
  float    smallStitchLength {0.0F};                 // small stitch size
  float    stitchBoxesThreshold {0.0F};              // show sitch box level
  float    stitchSpace {0.0F};                       // stitch spacing between lines of stitches
  float    fillAngle {0.0F};                         // fill angle
  uint32_t userFlagMap {0U};                         // bitmap for user variables
  float    borderWidth {0.0F};                       // border width
  uint32_t appliqueColor {0U};                       // applique color
  char     prevNames[OLDNUM][_MAX_PATH] {{0}};       // last file names
  float    snapLength {0.0F};                        // snap together length
  float    starRatio {0.0F};                         // star ratio
  float    spiralWrap {0.0F};                        // spiral wrap
  COLORREF bitmapBackgroundColors[COLORCNT] {0U}; // bitmap background color preferences NOLINT(modernize-avoid-c-arrays)
  float    buttonholeCornerLength {0.0F};    // buttonhole fill corner length
  float    picotSpace {0.0F};                // space between border picots
  int8_t   hoopType {0};                     // hoop type
  int8_t   auxFileType {0};                  // machine file type
  float    hoopSizeX {0.0F};                 // hoop x size
  float    hoopSizeY {0.0F};                 // hoop y size
  float    rotationAngle {0.0F};             // rotation angle
  float    gridSize {0.0F};                  // grid size
  float    clipOffset {0.0F};                // clipboard offset
  RECT     initialWindowCoords {0, 0, 0, 0}; // initial window coordinates
  COLORREF gridColor {0U};                   // grid color
  uint32_t fillPhase {0U};                   // clipboard fill phase
  float    customHoopX {0.0F};               // custom hoop width
  float    customHoopY {0.0F};               // custom hoop height
  float    traceLength {0.0F};               // lens points
  float    traceRatio {0.0F};                // trace ratio
  float    chainSpace {0.0F};                // chain space
  float    chainRatio {0.0F};                // chain ratio
  float    lensRatio {0.0F};                 // lens aspect ratio
  float    AppStitchLen {0.0F};              // Applique stitch length
  float    cursorNudgeStep {0.0F};           // cursor nudge step
  uint16_t nudgePixels {0U};                 // nudge pixels
  float    eggRatio {0.0F};                  // egg ratio
  uint16_t stitchSizePixels {0U};            // size of stitch points in pixels
  uint16_t formVertexSizePixels {0U};        // size of form points in pixels
  uint16_t formSides {0U};                   // sides of a created form
  float    tearTailLength {0.0F};            // length of the tear tail
  float    tearTwistStep {0.0F};             // tear twist step
  float    tearTwistRatio {0.0F};            // tear twist ratio
  uint16_t wavePoints {0U};                  // wave points
  uint16_t waveStart {0U};                   // wave strting point
  uint16_t waveEnd {0U};                     // wave ending point;
  uint16_t waveLobes {0U};                   // wave lobes
  uint8_t  featherFillType {0U};             // feather fill type
  uint8_t  featherUpCount {0U};              // feather up count
  uint8_t  featherDownCount {0U};            // feather down count
  uint8_t  featherType {0U};                 // feather bits
  float    featherRatio {0.0F};              // feather ratio
  float    featherMinStitchSize {0.0F};      // feather floor
  uint16_t featherCount {0U};                // feather fill psg granularity
  char p2cName[MAX_PATH] {0}; // pes2card file                       NOLINT(modernize-avoid-c-arrays)
  float    underlayIndent {0.0F};    // edge walk/underlay indent
  float    underlayAngle {0.0F};     // underlay angle
  float    underlaySpacing {0.0F};   // underlay spacing
  float    underlayStitchLen {0.0F}; // underlay stitch length
  float    daisyDiameter {0.0F};     // daisy diameter
  float    daisyPetalLen {0.0F};     // daisy petal length
  float    daisyHoleDiameter {0.0F}; // daisy hole diameter
  uint32_t daisyPetalCount {0U};     // daisy petals
  uint32_t daisyPetalPoints {0U};    // daisy petal points
  uint32_t daisyInnerCount {0U};     // daisy inner count
  uint8_t  daisyBorderType {0U};     // daisy border type
  uint8_t  dataCheck {0U};           // data check
  float    textureHeight {0.0F};     // textured fill height
  float    textureWidth {0.0F};      // textured fill width
  float    textureSpacing {0.0F};    // textured fill spacing
  uint16_t formBoxSizePixels {0U};   // form box pixels
  uint16_t daisyHeartCount {0U};     // daisy heart count
  uint16_t textureEditorSize {0U};   // texture editor pixels
  float    clpspc {0.0F};            // clipboard fill spacing
  char designerName[NAME_LEN] {0}; // designer name                       NOLINT(modernize-avoid-c-arrays)

  constexpr INIFILE() noexcept = default;
  // INIFILE(INIFILE&&) = default;
  // INIFILE& operator=(INIFILE const& rhs) = default;
  // INIFILE& operator=(INIFILE&&) = default;
  //~INIFILE() = default;
};

enum machineType { AUXPCS, AUXDST, AUXPES };

class FORMVERTICES
{
  public:
  uint32_t start {0U};
  uint32_t vertexCount {0U};
  uint32_t finish {0U};
  uint32_t form {0U};

  constexpr FORMVERTICES() noexcept = default;
  // FORMVERTICES(FORMVERTICES&&) = default;
  // FORMVERTICES& operator=(FORMVERTICES const& rhs) = default;
  // FORMVERTICES& operator=(FORMVERTICES&&) = default;
  //~FORMVERTICES() = default;
};

class fPOINT;
class fPOINTATTR;
class SMALPNTL;
class BSEQPNT;

class dPOINT
{
  public:
  double x {0.0};
  double y {0.0};

  constexpr dPOINT() noexcept = default;
  explicit inline dPOINT(fPOINT const& rhs) noexcept;
  inline dPOINT(float rhsX, float rhsY) noexcept;
  inline dPOINT(double rhsX, double rhsY) noexcept;
  inline auto operator==(dPOINT const& rhs) const noexcept -> bool;
  inline auto operator=(fPOINT const& rhs) noexcept -> dPOINT&;
  inline auto operator=(BSEQPNT const& rhs) noexcept -> dPOINT&;
  // dPOINT(dPOINT&&) = default;
  // dPOINT& operator=(dPOINT const& rhs) = default;
  // dPOINT& operator=(dPOINT&&) = default;
  //~dPOINT() = default;
};

class fPOINT
{
  public:
  float x {0.0F};
  float y {0.0F};

  constexpr fPOINT() noexcept = default;
  inline fPOINT(double rhsX, double rhsY) noexcept;
  inline fPOINT(float rhsX, float rhsY) noexcept;
  inline fPOINT(int32_t rhsX, int32_t rhsY) noexcept;
  inline fPOINT(LONG rhsX, LONG rhsY) noexcept;
  explicit inline fPOINT(dPOINT const& rhs) noexcept;
  inline auto operator==(fPOINT const& rhs) const noexcept -> bool;
  inline auto operator=(dPOINT const& rhs) noexcept -> fPOINT&;
  inline auto operator=(fPOINTATTR const& rhs) noexcept -> fPOINT&;
  inline auto operator=(SMALPNTL const& rhs) noexcept -> fPOINT&;
  inline auto operator=(BSEQPNT const& rhs) noexcept -> fPOINT&;
  // fPOINT(fPOINT&&) = default;
  // fPOINT& operator=(fPOINT const& rhs) = default;
  // fPOINT& operator=(fPOINT&&) = default;
  //~fPOINT() = default;
};

class fPOINTATTR
{
  public:
  float    x {0.0F};
  float    y {0.0F};
  uint32_t attribute {0U};

  constexpr fPOINTATTR() noexcept = default;
  inline fPOINTATTR(double rhsX, double rhsY, uint32_t rhsA) noexcept;
  inline fPOINTATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept;
  // fPOINTATTR(fPOINTATTR&&) = default;
  // fPOINTATTR& operator=(fPOINTATTR const& rhs) = default;
  // fPOINTATTR& operator=(fPOINTATTR&&) = default;
  //~fPOINTATTR() = default;
};

class SMALPNTL
{
  public:
  uint32_t line {0U}; // line and group must remain in this order for sort to work
  uint32_t group {0U};
  float    x {0.0F};
  float    y {0.0F};

  // constexpr SMALPNTL() noexcept = default;
  // SMALPNTL(SMALPNTL&&) = default;
  // SMALPNTL& operator=(SMALPNTL const& rhs) = default;
  // SMALPNTL& operator=(SMALPNTL&&) = default;
  //~SMALPNTL() = default;
};

class BSEQPNT
{
  public:
  float  x {0.0F};
  float  y {0.0F};
  int8_t attribute {0U};

  inline BSEQPNT() noexcept = default;
  inline BSEQPNT(double rhsX, double rhsY, int32_t rhsAttr);
  inline BSEQPNT(float rhsX, float rhsY, int32_t rhsAttr);
  // BSEQPNT(BSEQPNT&&) = default;
  // BSEQPNT& operator=(BSEQPNT const& rhs) = default;
  // BSEQPNT& operator=(BSEQPNT&&) = default;
  //~BSEQPNT() = default;
};

inline BSEQPNT::BSEQPNT(double rhsX, double rhsY, int32_t rhsAttr) {
  x         = gsl::narrow<float>(rhsX);
  y         = gsl::narrow<float>(rhsY);
  attribute = gsl::narrow<int8_t>(rhsAttr);
}

inline BSEQPNT::BSEQPNT(float rhsX, float rhsY, int32_t rhsAttr) : x(rhsX), y(rhsY) {
  attribute = gsl::narrow<int8_t>(rhsAttr);
}

inline auto fPOINT::operator==(fPOINT const& rhs) const noexcept -> bool {
  return (x == rhs.x) && (y == rhs.y);
}

inline fPOINT::fPOINT(float rhsX, float rhsY) noexcept : x(rhsX), y(rhsY) {
}

inline fPOINT::fPOINT(int32_t rhsX, int32_t rhsY) noexcept {
  x = gsl::narrow_cast<float>(rhsX);
  y = gsl::narrow_cast<float>(rhsY);
}

inline fPOINT::fPOINT(LONG rhsX, LONG rhsY) noexcept {
  x = gsl::narrow_cast<float>(rhsX);
  y = gsl::narrow_cast<float>(rhsY);
}

inline fPOINT::fPOINT(double rhsX, double rhsY) noexcept {
  x = gsl::narrow_cast<float>(rhsX);
  y = gsl::narrow_cast<float>(rhsY);
}

inline fPOINT::fPOINT(dPOINT const& rhs) noexcept {
  x = gsl::narrow_cast<float>(rhs.x);
  y = gsl::narrow_cast<float>(rhs.y);
}

inline auto fPOINT::operator=(dPOINT const& rhs) noexcept -> fPOINT& {
  x = gsl::narrow_cast<float>(rhs.x);
  y = gsl::narrow_cast<float>(rhs.y);
  return *this;
}

inline auto fPOINT::operator=(SMALPNTL const& rhs) noexcept -> fPOINT& {
  x = gsl::narrow_cast<float>(rhs.x);
  y = gsl::narrow_cast<float>(rhs.y);
  return *this;
}

inline auto fPOINT::operator=(fPOINTATTR const& rhs) noexcept -> fPOINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

inline auto fPOINT::operator=(BSEQPNT const& rhs) noexcept -> fPOINT& {
  x = rhs.x;
  y = rhs.y;
  return *this;
}

inline fPOINTATTR::fPOINTATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept :
    x(rhsX), y(rhsY), attribute(rhsA) {
}

inline fPOINTATTR::fPOINTATTR(double rhsX, double rhsY, uint32_t rhsA) noexcept {
  x         = gsl::narrow_cast<float>(rhsX);
  y         = gsl::narrow_cast<float>(rhsY);
  attribute = rhsA;
}

inline dPOINT::dPOINT(fPOINT const& rhs) noexcept :
    x(gsl::narrow_cast<double>(rhs.x)), y(gsl::narrow_cast<double>(rhs.y)) {
}

inline dPOINT::dPOINT(float rhsX, float rhsY) noexcept :
    x(gsl::narrow_cast<double>(rhsX)), y(gsl::narrow_cast<double>(rhsY)) {
}

inline dPOINT::dPOINT(double rhsX, double rhsY) noexcept : x(rhsX), y(rhsY) {
}
inline auto dPOINT::operator==(dPOINT const& rhs) const noexcept -> bool {
  return (x == rhs.x) && (y == rhs.y);
}

inline auto dPOINT::operator=(fPOINT const& rhs) noexcept -> dPOINT& {
  x = gsl::narrow_cast<double>(rhs.x);
  y = gsl::narrow_cast<double>(rhs.y);
  return *this;
}

inline auto dPOINT::operator=(BSEQPNT const& rhs) noexcept -> dPOINT& {
  x = gsl::narrow_cast<double>(rhs.x);
  y = gsl::narrow_cast<double>(rhs.y);
  return *this;
}

class fPOINTLINE
{
  public:
  float    x {0.0F};
  float    y {0.0F};
  uint16_t line {0U};

  // constexpr fPOINTLINE() noexcept = default;
  // fPOINTLINE(fPOINTLINE&&) = default;
  // fPOINTLINE& operator=(fPOINTLINE const& rhs) = default;
  // fPOINTLINE& operator=(fPOINTLINE&&) = default;
  //~fPOINTLINE() = default;
};

class dRECTANGLE
{
  public:
  double top {0.0};
  double bottom {0.0};
  double left {0.0};
  double right {0.0};

  constexpr dRECTANGLE() noexcept = default;
  // dRECTANGLE(dRECTANGLE&&) = default;
  // dRECTANGLE& operator=(dRECTANGLE const& rhs) = default;
  // dRECTANGLE& operator=(dRECTANGLE&&) = default;
  //~dRECTANGLE() = default;
};

class COLCHNG
{
  public:
  uint16_t stitchIndex {0U}; // stitch index
  uint16_t colorIndex {0U};  // color index

  // constexpr COLCHNG() noexcept = default;
  // COLCHNG(COLCHNG&&) = default;
  // COLCHNG& operator=(COLCHNG const& rhs) = default;
  // COLCHNG& operator=(COLCHNG&&) = default;
  //~COLCHNG() = default;
};

class FEATHER
{
  public:
  uint32_t fillType {0U};          // type of feather fill
  float    ratio {0.0F};           // feather ratio
  float    minStitch {0.0F};       // smallest stitch allowed
  float    ratioLocal {0.0F};      // local feather ratio
  float    formRatio {0.0F};       // feather ratio from form
  uint32_t extendedAttribute {0U}; // extended form attribute
  uint32_t upCount {0U};           // feather up count
  uint32_t downCount {0U};         // feather down count
  uint32_t totalCount {0U};        // up count plus down count
  uint32_t phaseIndex {0U};
  uint32_t phase {0U};
  uint32_t countUp {0U};
  uint32_t countDown {0U};
  float    globalStep {0.0F};
  float    globalPosition {0.0F};
  float    globalPhase {0.0F};
  float    globalUp {0.0F};
  float    globalDown {0.0F};
  float    globalRatio {0.0F};

  constexpr FEATHER() noexcept = default;
  // FEATHER(FEATHER&&) = default;
  // FEATHER& operator=(FEATHER const& rhs) = default;
  // FEATHER& operator=(FEATHER&&) = default;
  //~FEATHER() = default;
};

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
constexpr uint32_t TYPSHFT   = 29U;
constexpr uint32_t USHFT     = 31U; // user edited stitches shift

/*
bit definitions for fPOINTATTR.attribute
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

constexpr uint32_t FRMFIL   = 0x20000000U;
constexpr uint32_t FRMBFIL  = 0x40000000U;
constexpr uint32_t FRMAPFIL = 0x60000000U;

class fRECTANGLE
{
  public:
  float left {0.0F};
  float top {0.0F};
  float right {0.0F};
  float bottom {0.0F};

  constexpr fRECTANGLE() noexcept = default;
  // fRECTANGLE(fRECTANGLE&&) = default;
  // fRECTANGLE& operator=(fRECTANGLE const& rhs) = default;
  // fRECTANGLE& operator=(fRECTANGLE&&) = default;
  //~fRECTANGLE() = default;

  inline fRECTANGLE(float rhsLeft, float rhsTop, float rhsRight, float rhsBottom) noexcept;
};

inline fRECTANGLE::fRECTANGLE(float rhsLeft, float rhsTop, float rhsRight, float rhsBottom) noexcept :
    left(rhsLeft), top(rhsTop), right(rhsRight), bottom(rhsBottom) {
}

class FORMINFO
{
  public:
  uint32_t type {0U};
  uint32_t attribute {0U};
  uint32_t sideCount {0U};

  constexpr FORMINFO() noexcept = default;
  // FORMINFO(FORMINFO&&) = default;
  // FORMINFO& operator=(FORMINFO const& rhs) = default;
  // FORMINFO& operator=(FORMINFO&&) = default;
  //~FORMINFO() = default;
};

class SATCONOUT;

class SATCON
{
  public:
  uint32_t start {0U};
  uint32_t finish {0U};

  constexpr SATCON() noexcept = default;
  // SATCON(SATCON&&) = default;
  // SATCON& operator=(SATCON const& rhs) = default;
  // SATCON& operator=(SATCON&&) = default;
  //~SATCON() = default;

  explicit SATCON(SATCONOUT const& rhs) noexcept;
  inline SATCON(uint32_t rStart, uint32_t rFinish) noexcept;
  inline auto operator=(SATCONOUT const& rhs) noexcept -> SATCON&;
};

inline SATCON::SATCON(uint32_t rStart, uint32_t rFinish) noexcept : start(rStart), finish(rFinish) {
}

class SATCONOUT
{
  public:
  uint16_t start {0U};
  uint16_t finish {0U};

  constexpr SATCONOUT() noexcept = default;
  // SATCONOUT(SATCONOUT&&) = default;
  // SATCONOUT& operator=(SATCONOUT const& rhs) = default;
  // SATCONOUT& operator=(SATCONOUT&&) = default;
  //~SATCONOUT() = default;

  explicit SATCONOUT(SATCON const& rhs);
  inline auto operator=(SATCON const& rhs) -> SATCONOUT&;
};

inline SATCONOUT::SATCONOUT(SATCON const& rhs) {
  start  = gsl::narrow<uint16_t>(rhs.start);
  finish = gsl::narrow<uint16_t>(rhs.finish);
}

inline auto SATCONOUT::operator=(SATCON const& rhs) -> SATCONOUT& {
  start  = gsl::narrow<uint16_t>(rhs.start);
  finish = gsl::narrow<uint16_t>(rhs.finish);

  return *this;
}

inline SATCON::SATCON(SATCONOUT const& rhs) noexcept {
  start  = rhs.start;
  finish = rhs.finish;
}

inline auto SATCON::operator=(SATCONOUT const& rhs) noexcept -> SATCON& {
  start  = rhs.start;
  finish = rhs.finish;

  return *this;
}

union FANGCLPOUT;

union FANGCLP {
  public:
  float    angle;
  uint32_t clip; // pointer to start of fill clipboard data
  SATCON   guide;

  FANGCLP() noexcept;
  // FANGCLP(FANGCLP&&) = default;
  // FANGCLP& operator=(FANGCLP const& rhs) = default;
  // FANGCLP& operator=(FANGCLP&&) = default;
  //~FANGCLP() = default;
};

inline FANGCLP::FANGCLP() noexcept {
  guide.start  = 0U;
  guide.finish = 0U;
}

union FANGCLPOUT {
  public:
  float angle;
  DWORD clip {}; // clip pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
  SATCONOUT guide;

  FANGCLPOUT() noexcept;
  // FANGCLPOUT(FANGCLPOUT&&) = default;
  // FANGCLPOUT& operator=(FANGCLPOUT const& rhs) = default;
  // FANGCLPOUT& operator=(FANGCLPOUT&&) = default;
  //~FANGCLPOUT() = default;
};

inline FANGCLPOUT::FANGCLPOUT() noexcept {
  clip = 0;
}

union FLENCNTOUT;

union FLENCNT {
  public:
  float    stitchLength;
  uint32_t clipCount; // number of points in fill clipboard data

  FLENCNT() noexcept;
  // FLENCNT(FLENCNT&&) = default;
  // FLENCNT& operator=(FLENCNT const& rhs) = default;
  // FLENCNT& operator=(FLENCNT&&) = default;
  //~FLENCNT() = default;

  explicit FLENCNT(FLENCNTOUT const& rhs) noexcept;
  inline auto operator=(FLENCNTOUT const& rhs) noexcept -> FLENCNT&;
};

inline FLENCNT::FLENCNT() noexcept {
  clipCount = 0U;
}

union FLENCNTOUT {
  public:
  float    stitchLength {};
  uint32_t clipCount;

  FLENCNTOUT() noexcept;
  // FLENCNTOUT(FLENCNTOUT&&) = default;
  // FLENCNTOUT& operator=(FLENCNTOUT const& rhs) = default;
  // FLENCNTOUT& operator=(FLENCNTOUT&&) = default;
  //~FLENCNTOUT() = default;

  explicit FLENCNTOUT(FLENCNT const& rhs) noexcept;
  inline auto operator=(FLENCNT const& rhs) noexcept -> FLENCNTOUT&;
};

inline FLENCNTOUT::FLENCNTOUT() noexcept {
  clipCount = 0U;
}

inline FLENCNTOUT::FLENCNTOUT(FLENCNT const& rhs) noexcept {
  clipCount = rhs.clipCount;
}

inline auto FLENCNTOUT::operator=(FLENCNT const& rhs) noexcept -> FLENCNTOUT& {
  clipCount = rhs.clipCount;

  return *this;
}

inline FLENCNT::FLENCNT(FLENCNTOUT const& rhs) noexcept { // NOLINT(cppcoreguidelines-pro-type-member-init)
  clipCount = rhs.clipCount;
}

inline auto FLENCNT::operator=(FLENCNTOUT const& rhs) noexcept -> FLENCNT& {
  clipCount = rhs.clipCount;

  return *this;
}

union SATINANGLEOUT;

union SATINANGLE {
  public:
  uint32_t guide;
  float    angle;

  SATINANGLE() noexcept;
  // SATINANGLE(SATINANGLE&&) = default;
  // SATINANGLE& operator=(SATINANGLE const& rhs) = default;
  // SATINANGLE& operator=(SATINANGLE&&) = default;
  //~SATINANGLE() = default;

  explicit SATINANGLE(SATINANGLEOUT const& rhs) noexcept;
  inline auto operator=(SATINANGLEOUT const& rhs) noexcept -> SATINANGLE&;
};

inline SATINANGLE::SATINANGLE() noexcept {
  angle = 0.0;
}

union SATINANGLEOUT {
  public:
  DWORD guide {};
  float angle;

  SATINANGLEOUT() noexcept;
  // SATINANGLEOUT(SATINANGLEOUT&&) = default;
  // SATINANGLEOUT& operator=(SATINANGLEOUT const& rhs) = default;
  // SATINANGLEOUT& operator=(SATINANGLEOUT&&) = default;
  //~SATINANGLEOUT() = default;

  explicit SATINANGLEOUT(SATINANGLE const& rhs) noexcept;
  inline auto operator=(SATINANGLE const& rhs) noexcept -> SATINANGLEOUT&;
};

inline SATINANGLEOUT::SATINANGLEOUT() noexcept {
  angle = 0.0;
}

inline SATINANGLEOUT::SATINANGLEOUT(SATINANGLE const& rhs) noexcept {
  angle = rhs.angle;
}

inline auto SATINANGLEOUT::operator=(SATINANGLE const& rhs) noexcept -> SATINANGLEOUT& {
  angle = rhs.angle;

  return *this;
}

inline SATINANGLE::SATINANGLE(SATINANGLEOUT const& rhs) noexcept { // NOLINT(cppcoreguidelines-pro-type-member-init)
  angle = rhs.angle;
}

inline auto SATINANGLE::operator=(SATINANGLEOUT const& rhs) noexcept -> SATINANGLE& {
  angle = rhs.angle;

  return *this;
}

/*
fill	edgeStitchLen	edgeSpacing	borderSize	clipCount	picspac		crnrsiz		brdend

EDGELINE	edgeStitchLen
EDGEBEAN	edgeStitchLen
EDGECLIP											clipCount
EDGEANGSAT	edgeStitchLen	edgeSpacing	borderSize									attribute
EDGEAPPL	edgeStitchLen	edgeSpacing	borderSize									attribute
EDGEPROPSAT	edgeStitchLen	edgeSpacing	borderSize									attribute
EDGEBHOL	edgeStitchLen	edgeSpacing	borderSize	clipCount,res
EDGEPICOT	edgeStitchLen				borderSize	clipCount	edgeSpacing		res
*/

constexpr auto BELEN    = 1U;
constexpr auto BESPAC   = 1U << 1U;
constexpr auto BESIZ    = 1U << 2U;
constexpr auto BNCLP    = 1U << 3U;
constexpr auto BPICSPAC = 1U << 4U;
constexpr auto BCNRSIZ  = 1U << 5U;
constexpr auto BRDEND   = 1U << 6U;
constexpr auto BRDPOS   = 1U << 7U;
constexpr auto BEMAX    = 1U << 8U;
constexpr auto BEMIN    = 1U << 9U;
constexpr auto CHNPOS   = 1U << 10U;

constexpr auto MEGLIN  = (BELEN | BEMAX | BEMIN);
constexpr auto MEGBLD  = (BELEN | BEMAX | BEMIN);
constexpr auto MEGCLP  = (BNCLP | BEMAX | BEMIN);
constexpr auto MEGSAT  = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr auto MEGAP   = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr auto MEGPRP  = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr auto MEGHOL  = (BELEN | BESPAC | BESIZ | BCNRSIZ | BEMAX | BEMIN);
constexpr auto MEGPIC  = (BELEN | BESIZ | BNCLP | BPICSPAC | BCNRSIZ | BEMAX | BEMIN);
constexpr auto MEGDUB  = (BELEN | BEMAX | BEMIN);
constexpr auto MEGCHNL = (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS);
constexpr auto MEGCHNH = (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS);
constexpr auto MEGCLPX = (BNCLP | BEMAX | BEMIN);

constexpr auto EGLIN_LINS = 4U;
constexpr auto EGBLD_LINS = 4U;
constexpr auto EGCLP_LINS = 3U;
constexpr auto EGSAT_LINS = 6U;
constexpr auto EGAP_LINS  = 7U;
constexpr auto EGPRP_LINS = 6U;
constexpr auto EGHOL_LINS = 7U;
constexpr auto EGPIC_LINS = 7U;
constexpr auto EGCHN_LINS = 6U;

class FTHINFO
{
  public:
  uint8_t  fillType {0U};        // feather fill type
  uint8_t  upCount {0U};         // feather up count
  uint8_t  downCount {0U};       // feather down count
  uint8_t  color {0U};           // feather blend color
  float    ratio {0.0F};         // feather ratio
  float    minStitchSize {0.0F}; // feather floor
  uint16_t count {0U};           // feather fill psg granularity

  constexpr FTHINFO() noexcept = default;
  // FTHINFO(FTHINFO&&) = default;
  // FTHINFO& operator=(FTHINFO const& rhs) = default;
  // FTHINFO& operator=(FTHINFO&&) = default;
  //~FTHINFO() = default;
};

class TXTRINFO
{
  public:
  int16_t  lines {0};
  uint16_t index {0U};
  uint16_t count {0U};
  float    height {0.0F};

  constexpr TXTRINFO() noexcept = default;
  // TXTRINFO(TXTRINFO&&) = default;
  // TXTRINFO& operator=(TXTRINFO const& rhs) = default;
  // TXTRINFO& operator=(TXTRINFO&&) = default;
  //~TXTRINFO() = default;
};

union TFINFO {
  public:
  FTHINFO  feather;
  TXTRINFO texture;

  TFINFO() noexcept;
  // TFINFO(TFINFO&&) = default;
  // TFINFO& operator=(TFINFO const& rhs) = default;
  // TFINFO& operator=(TFINFO&&) = default;
  //~TFINFO() = default;
};

inline TFINFO::TFINFO() noexcept : feather() {
}

class FRMHEDO // Original form header used prior to version 2
{
  public:
  uint8_t  attribute;   // attribute
  uint16_t vertexCount; // number of sides
  uint8_t  type;        // type
  uint8_t  fillColor;   // fill color
  uint8_t  borderColor; // border color
  uint16_t clipEntries; // number of border clipboard entries
  DWORD    vertices;    // points

  SATINANGLEOUT satinOrAngle; // satin guidelines or angle clipboard fill angle

  DWORD      borderClipData;  // border clipboard data
  uint16_t   satinGuideCount; // number of satin guidelines
  uint16_t   wordParam;       // word parameter
  fRECTANGLE rectangle {};    // rectangle
  uint8_t    fillType;        // fill type
  uint8_t    edgeType;        // edge type
  float      fillSpacing;     // fill spacing
  FLENCNTOUT lengthOrCount;   // fill stitch length or clipboard count
  FANGCLPOUT angleOrClipData; // fill angle or clipboard data pointer
  float      borderSize;      // border size
  float      edgeSpacing;     // edge spacing
  float      edgeStitchLen;   // edge stitch length
  uint16_t   res;             // pico length

  FRMHEDO() noexcept;
  // FRMHEDO(FRMHEDO&&) = default;
  // FRMHEDO& operator=(FRMHEDO const& rhs) = default;
  // FRMHEDO& operator=(FRMHEDO&&) = default;
  //~FRMHEDO() = default;
};

inline FRMHEDO::FRMHEDO() noexcept : satinOrAngle(), lengthOrCount(), angleOrClipData() {
  attribute       = 0U;
  vertexCount     = 0U;
  type            = 0U;
  fillColor       = 0U;
  borderColor     = 0U;
  clipEntries     = 0U;
  vertices        = 0U;
  borderClipData  = 0U;
  satinGuideCount = 0U;
  wordParam       = 0U;
  fillType        = 0U;
  edgeType        = 0U;
  fillSpacing     = 0.0F;
  borderSize      = 0.0F;
  edgeSpacing     = 0.0F;
  edgeStitchLen   = 0.0F;
  res             = 0U;
}

class FRMHEDOUT;

class FRMHED
{
  public:
  uint8_t    attribute;           // attribute
  uint32_t   vertexCount;         // number of sides
  uint8_t    type;                // type
  uint8_t    fillColor;           // fill color
  uint8_t    borderColor;         // border color
  uint32_t   clipEntries;         // number of border clipboard entries
  uint32_t   vertexIndex;         // index into FormVertices
  SATINANGLE satinOrAngle {};     // satin guidelines or angle clipboard fill angle
  uint32_t   borderClipData;      // pointer to start of border clipboard data
  uint32_t   satinGuideCount;     // number of satin guidelines
  uint32_t   wordParam;           // clipboard/textured fill phase or satin end guide
  fRECTANGLE rectangle {};        // rectangle
  uint8_t    fillType;            // fill type
  uint8_t    edgeType;            // edge type
  float      fillSpacing;         // fill spacing
  FLENCNT    lengthOrCount {};    // fill stitch length or clipboard count
  FANGCLP    angleOrClipData {};  // fill angle or clipboard data pointer
  float      borderSize;          // border size
  float      edgeSpacing;         // edge spacing
  float      edgeStitchLen;       // edge stitch length
  uint16_t   picoLength;          // pico length
  uint32_t   extendedAttribute;   // attribute extension
  float      maxFillStitchLen;    // maximum fill stitch length
  float      minFillStitchLen;    // minimum fill stitch length
  float      maxBorderStitchLen;  // maximum border stitch length
  float      minBorderStitchLen;  // minimum border stitch length
  TFINFO     fillInfo {};         // feather/texture info
  uint32_t   fillStart;           // fill start point
  uint32_t   fillEnd;             // fill end point
  float      underlaySpacing;     // underlay spacing
  float      underlayStitchLen;   // underlay stitch length
  float      underlayStitchAngle; // underlay stitch angle
  float      underlayIndent;      // underlay/edge walk indent
  float      txof;                // gradient end density
  uint8_t    underlayColor;       // underlay color
  uint8_t    cres;                // reserved

  FRMHED() noexcept;
  // FRMHED(FRMHED&&) = default;
  // FRMHED& operator=(FRMHED const& rhs) = default;
  // FRMHED& operator=(FRMHED&&) = default;
  //~FRMHED() = default;

  explicit inline FRMHED(FRMHEDO const& rhs) noexcept;
  explicit inline FRMHED(FRMHEDOUT const& rhs) noexcept;

  inline auto operator=(FRMHEDO const& rhs) noexcept -> FRMHED&;
  inline auto operator=(FRMHEDOUT const& rhs) noexcept -> FRMHED&;
};

inline FRMHED::FRMHED() noexcept : satinOrAngle(), lengthOrCount(), angleOrClipData(), fillInfo() {
  attribute       = 0U;
  vertexCount     = 0U;
  type            = 0U;
  fillColor       = 0U;
  borderColor     = 0U;
  clipEntries     = 0U;
  vertexIndex     = 0U;
  borderClipData  = 0U;
  satinGuideCount = 0U;
  wordParam       = 0U;
  fillType        = 0U;
  edgeType        = 0U;
  fillSpacing     = 0.0F;
  borderSize      = 0.0F;
  edgeSpacing     = 0.0F;
  edgeStitchLen   = 0.0F;
  picoLength      = 0U;

  maxFillStitchLen    = 0.0F;
  minFillStitchLen    = 0.0F;
  extendedAttribute   = 0U;
  maxBorderStitchLen  = 0.0F;
  minBorderStitchLen  = 0.0F;
  fillStart           = 0U;
  fillEnd             = 0U;
  underlaySpacing     = 0.0F;
  underlayStitchLen   = 0.0F;
  underlayStitchAngle = 0.0F;
  underlayIndent      = 0.0F;
  txof                = 0.0F;
  underlayColor       = 0U;
  cres                = 0U;
}

inline FRMHED::FRMHED(FRMHEDO const& rhs) noexcept :
    satinOrAngle(rhs.satinOrAngle), rectangle(rhs.rectangle), lengthOrCount(rhs.lengthOrCount),
    fillInfo() {
  attribute       = rhs.attribute;
  vertexCount     = rhs.vertexCount;
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = rhs.clipEntries;
  vertexIndex     = 0;
  borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.res;

  extendedAttribute   = 0U;
  maxFillStitchLen    = 0.0F;
  minFillStitchLen    = 0.0F;
  maxBorderStitchLen  = 0.0F;
  minBorderStitchLen  = 0.0F;
  fillStart           = 0U;
  fillEnd             = 0U;
  underlaySpacing     = 0.0F;
  underlayStitchLen   = 0.0F;
  underlayStitchAngle = 0.0F;
  underlayIndent      = 0.0F;
  txof                = 0.0F;
  underlayColor       = 0U;
  cres                = 0U;
}

inline auto FRMHED::operator=(FRMHEDO const& rhs) noexcept -> FRMHED& {
  attribute       = rhs.attribute;
  vertexCount     = rhs.vertexCount;
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = rhs.clipEntries;
  vertexIndex     = 0U;
  satinOrAngle    = rhs.satinOrAngle;
  borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  lengthOrCount   = rhs.lengthOrCount;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.res;
  return *this;
}

class FRMHEDOUT
{
  public:
  uint8_t  attribute;      // attribute
  uint16_t vertexCount {}; // number of sides
  uint8_t  type;           // type
  uint8_t  fillColor;      // fill color
  uint8_t  borderColor;    // border color
  uint16_t clipEntries {}; // number of border clipboard entries
  DWORD    vertexIndex; // vertex index not saved in v1 or v2. size is to keep compatibility with v1 & v2 ThrEd files

  SATINANGLEOUT satinOrAngle; // satin guidelines or angle clipboard fill angle

  DWORD      borderClipData; // border clipboard data pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
  uint16_t   satinGuideCount {}; // number of satin guidelines
  uint16_t   wordParam {};       // clipboard/textured fill phase or satin end guide
  fRECTANGLE rectangle {};       // rectangle
  uint8_t    fillType;           // fill type
  uint8_t    edgeType;           // edge type
  float      fillSpacing;        // fill spacing
  FLENCNTOUT lengthOrCount;      // fill stitch length or clipboard count
  FANGCLPOUT angleOrClipData;    // fill angle or clipboard data pointer

  float    borderSize;          // border size
  float    edgeSpacing;         // edge spacing
  float    edgeStitchLen;       // edge stitch length
  uint16_t picoLength;          // pico length
  uint32_t extendedAttribute;   // attribute extension
  float    maxFillStitchLen;    // maximum fill stitch length
  float    minFillStitchLen;    // minimum fill stitch length
  float    maxBorderStitchLen;  // maximum border stitch length
  float    minBorderStitchLen;  // minimum border stitch length
  TFINFO   fillInfo {};         // feather/texture info
  uint16_t fillStart {};        // fill start point
  uint16_t fillEnd {};          // fill end point
  float    underlaySpacing;     // underlay spacing
  float    underlayStitchLen;   // underlay stitch length
  float    underlayStitchAngle; // underlay stitch angle
  float    underlayIndent;      // underlay/edge walk indent
  float    txof;                // gradient end density
  uint8_t  underlayColor;       // underlay color
  uint8_t  cres;                // reserved

  inline FRMHEDOUT() noexcept;
  // FRMHEDOUT(FRMHEDOUT&&) = default;
  // FRMHEDOUT& operator=(FRMHEDOUT const& rhs) = default;
  // FRMHEDOUT& operator=(FRMHEDOUT&&) = default;
  //~FRMHEDOUT() = default;

  explicit inline FRMHEDOUT(FRMHED const& rhs);
  inline auto operator=(FRMHED const& rhs) -> FRMHEDOUT&;
};

inline FRMHEDOUT::FRMHEDOUT() noexcept :
    satinOrAngle(), lengthOrCount(), angleOrClipData(), fillInfo() {
  attribute       = 0U;
  vertexCount     = 0U;
  type            = 0U;
  fillColor       = 0U;
  borderColor     = 0U;
  clipEntries     = 0U;
  vertexIndex     = 0U;
  borderClipData  = 0U;
  satinGuideCount = 0U;
  wordParam       = 0U;
  fillType        = 0U;
  edgeType        = 0U;
  fillSpacing     = 0.0F;
  borderSize      = 0.0F;
  edgeSpacing     = 0.0F;
  edgeStitchLen   = 0.0F;
  picoLength      = 0U;

  maxFillStitchLen    = 0.0F;
  minFillStitchLen    = 0.0F;
  extendedAttribute   = 0U;
  maxBorderStitchLen  = 0.0F;
  minBorderStitchLen  = 0.0F;
  fillStart           = 0U;
  fillEnd             = 0U;
  underlaySpacing     = 0.0F;
  underlayStitchLen   = 0.0F;
  underlayStitchAngle = 0.0F;
  underlayIndent      = 0.0F;
  txof                = 0.0F;
  underlayColor       = 0U;
  cres                = 0U;
}

inline FRMHEDOUT::FRMHEDOUT(FRMHED const& rhs) :
    satinOrAngle(rhs.satinOrAngle), rectangle(rhs.rectangle), lengthOrCount(rhs.lengthOrCount) {
  attribute       = rhs.attribute;
  vertexCount     = gsl::narrow<uint16_t>(rhs.vertexCount);
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = gsl::narrow<uint16_t>(rhs.clipEntries);
  vertexIndex     = 0U; // do not write the pointer value to file
  borderClipData  = 0U; // do not write the pointer value to file
  satinGuideCount = gsl::narrow<uint16_t>(rhs.satinGuideCount);
  wordParam       = gsl::narrow<uint16_t>(rhs.wordParam);
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start =
		    gsl::narrow<decltype(angleOrClipData.guide.start)>(rhs.angleOrClipData.guide.start);
		angleOrClipData.guide.finish =
		    gsl::narrow<decltype(angleOrClipData.guide.finish)>(rhs.angleOrClipData.guide.finish);
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.picoLength;

  extendedAttribute   = rhs.extendedAttribute;
  maxFillStitchLen    = rhs.maxFillStitchLen;
  minFillStitchLen    = rhs.minFillStitchLen;
  maxBorderStitchLen  = rhs.maxBorderStitchLen;
  minBorderStitchLen  = rhs.minBorderStitchLen;
  fillInfo            = rhs.fillInfo;
  fillStart           = gsl::narrow<uint16_t>(rhs.fillStart);
  fillEnd             = gsl::narrow<uint16_t>(rhs.fillEnd);
  underlaySpacing     = rhs.underlaySpacing;
  underlayStitchLen   = rhs.underlayStitchLen;
  underlayStitchAngle = rhs.underlayStitchAngle;
  underlayIndent      = rhs.underlayIndent;
  txof                = rhs.txof;
  underlayColor       = rhs.underlayColor;
  cres                = rhs.cres;
}

inline auto FRMHEDOUT::operator=(FRMHED const& rhs) -> FRMHEDOUT& {
  attribute       = rhs.attribute;
  vertexCount     = gsl::narrow<uint16_t>(rhs.vertexCount);
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = gsl::narrow<uint16_t>(rhs.clipEntries);
  vertexIndex     = 0; // do not write the pointer value to file
  satinOrAngle    = rhs.satinOrAngle;
  borderClipData  = 0; // do not write the pointer value to file
  satinGuideCount = gsl::narrow<uint16_t>(rhs.satinGuideCount);
  wordParam       = gsl::narrow<uint16_t>(rhs.wordParam);
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  lengthOrCount   = rhs.lengthOrCount;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start =
		    gsl::narrow<decltype(angleOrClipData.guide.start)>(rhs.angleOrClipData.guide.start);
		angleOrClipData.guide.finish =
		    gsl::narrow<decltype(angleOrClipData.guide.finish)>(rhs.angleOrClipData.guide.finish);
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.picoLength;

  extendedAttribute   = rhs.extendedAttribute;
  maxFillStitchLen    = rhs.maxFillStitchLen;
  minFillStitchLen    = rhs.minFillStitchLen;
  maxBorderStitchLen  = rhs.maxBorderStitchLen;
  minBorderStitchLen  = rhs.minBorderStitchLen;
  fillInfo            = rhs.fillInfo;
  fillStart           = gsl::narrow<uint16_t>(rhs.fillStart);
  fillEnd             = gsl::narrow<uint16_t>(rhs.fillEnd);
  underlaySpacing     = rhs.underlaySpacing;
  underlayStitchLen   = rhs.underlayStitchLen;
  underlayStitchAngle = rhs.underlayStitchAngle;
  underlayIndent      = rhs.underlayIndent;
  txof                = rhs.txof;
  underlayColor       = rhs.underlayColor;
  cres                = rhs.cres;

  return *this;
}

inline FRMHED::FRMHED(FRMHEDOUT const& rhs) noexcept :
    satinOrAngle(rhs.satinOrAngle), rectangle(rhs.rectangle), lengthOrCount(rhs.lengthOrCount) {
  attribute       = rhs.attribute;
  vertexCount     = rhs.vertexCount;
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = rhs.clipEntries;
  vertexIndex     = 0U; // do not read the index from a v2 file
  borderClipData  = 0U; // do not read the index from a v2 file
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.picoLength;

  extendedAttribute   = rhs.extendedAttribute;
  maxFillStitchLen    = rhs.maxFillStitchLen;
  minFillStitchLen    = rhs.minFillStitchLen;
  maxBorderStitchLen  = rhs.maxBorderStitchLen;
  minBorderStitchLen  = rhs.minBorderStitchLen;
  fillInfo            = rhs.fillInfo;
  fillStart           = rhs.fillStart;
  fillEnd             = rhs.fillEnd;
  underlaySpacing     = rhs.underlaySpacing;
  underlayStitchLen   = rhs.underlayStitchLen;
  underlayStitchAngle = rhs.underlayStitchAngle;
  underlayIndent      = rhs.underlayIndent;
  txof                = rhs.txof;
  underlayColor       = rhs.underlayColor;
  cres                = rhs.cres;
}

inline auto FRMHED::operator=(FRMHEDOUT const& rhs) noexcept -> FRMHED& {
  attribute       = rhs.attribute;
  vertexCount     = rhs.vertexCount;
  type            = rhs.type;
  fillColor       = rhs.fillColor;
  borderColor     = rhs.borderColor;
  clipEntries     = rhs.clipEntries;
  vertexIndex     = 0U; // do not read the index from a v2 file
  satinOrAngle    = rhs.satinOrAngle;
  borderClipData  = 0U; // do not read the index from a v2 file
  satinGuideCount = rhs.satinGuideCount;
  wordParam       = rhs.wordParam;
  rectangle       = rhs.rectangle;
  fillType        = rhs.fillType;
  edgeType        = rhs.edgeType;
  fillSpacing     = rhs.fillSpacing;
  lengthOrCount   = rhs.lengthOrCount;
  if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0U)) ||
      ((((1U << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0U))) {
	angleOrClipData.clip = rhs.angleOrClipData.clip;
  }
  else {
	if (type == FRMLINE) {
	  if (fillType == CONTF) {
		angleOrClipData.guide.start  = rhs.angleOrClipData.guide.start;
		angleOrClipData.guide.finish = rhs.angleOrClipData.guide.finish;
	  }
	}
	else {
	  angleOrClipData.angle = rhs.angleOrClipData.angle;
	}
  }

  borderSize    = rhs.borderSize;
  edgeSpacing   = rhs.edgeSpacing;
  edgeStitchLen = rhs.edgeStitchLen;
  picoLength    = rhs.picoLength;

  extendedAttribute   = rhs.extendedAttribute;
  maxFillStitchLen    = rhs.maxFillStitchLen;
  minFillStitchLen    = rhs.minFillStitchLen;
  maxBorderStitchLen  = rhs.maxBorderStitchLen;
  minBorderStitchLen  = rhs.minBorderStitchLen;
  fillInfo            = rhs.fillInfo;
  fillStart           = rhs.fillStart;
  fillEnd             = rhs.fillEnd;
  underlaySpacing     = rhs.underlaySpacing;
  underlayStitchLen   = rhs.underlayStitchLen;
  underlayStitchAngle = rhs.underlayStitchAngle;
  underlayIndent      = rhs.underlayIndent;
  txof                = rhs.txof;
  underlayColor       = rhs.underlayColor;
  cres                = rhs.cres;

  return *this;
}

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

// blunt bits
constexpr uint8_t FBLNT  = 0x20U; // 0010 0000
constexpr uint8_t SBLNT  = 0x40U; // 0100 0000
constexpr uint8_t NFBLNT = 0xdfU; // 1101 1111
constexpr uint8_t NSBLNT = 0xbfU; // 1011 1111
constexpr uint8_t NOBLNT = 0x9fU; // 1001 1111

// contour refil
constexpr auto FRECONT  = 0x80U; // 1000 0000
constexpr auto NFRECONT = 0x7fU; // 0111 1111

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

class FORMCLIP // form data clipboard header
{
  public:
  uint32_t clipType {0U};
  uint32_t reserved {0U};
  FRMHED   form {};

  constexpr FORMCLIP() noexcept = default;
  // FORMCLIP(FORMCLIP&&) = default;
  // FORMCLIP& operator=(FORMCLIP const& rhs) = default;
  // FORMCLIP& operator=(FORMCLIP&&) = default;
  //~FORMCLIP() = default;
};

class FORMSCLIP // multiple forms clipboard header
{
  public:
  uint32_t clipType {0U};
  uint16_t formCount {0U};
  uint16_t reserved {0U};

  constexpr FORMSCLIP() noexcept = default;
  // FORMSCLIP(FORMSCLIP&&) = default;
  // FORMSCLIP& operator=(FORMSCLIP const& rhs) = default;
  // FORMSCLIP& operator=(FORMSCLIP&&) = default;
  //~FORMSCLIP() = default;
};

class FORMVERTEXCLIP // form points clipboard header
{
  public:
  uint32_t clipType {0U};
  uint32_t vertexCount {0U};
  bool     direction {false};

  constexpr FORMVERTEXCLIP() noexcept = default;
  // FORMVERTEXCLIP(FORMVERTEXCLIP&&) = default;
  // FORMVERTEXCLIP& operator=(FORMVERTEXCLIP const& rhs) = default;
  // FORMVERTEXCLIP& operator=(FORMVERTEXCLIP&&) = default;
  //~FORMVERTEXCLIP() = default;
};

class STRHED // thred file header
{
  public:
  uint32_t headerType {0U};
  uint32_t fileLength {0U};    // length of strhed + length of stitch data
  uint16_t stitchCount {0U};   // number of stitches
  uint16_t hoopType {0U};      // size of hoop
  uint16_t formCount {0U};     // number of forms
  uint16_t vertexLen {0U};     // points to form points
  uint16_t vertexCount {0U};   // number of form points
  uint16_t dlineLen {0U};      // points to dline data
  uint16_t dlineCount {0U};    // dline data count
  uint16_t clipDataLen {0U};   // points to clipboard data
  uint16_t clipDataCount {0U}; // clipboard data count

  constexpr STRHED() noexcept = default;
  // STRHED(STRHED&&) = default;
  // STRHED& operator=(STRHED const& rhs) = default;
  // STRHED& operator=(STRHED&&) = default;
  //~STRHED() = default;
};

class TXPNT // textured fill point
{
  public:
  float    y {0.0F};
  uint16_t line {0U};

  // constexpr TXPNT() noexcept = default;
  // TXPNT(TXPNT&&) = default;
  // TXPNT& operator=(TXPNT const& rhs) = default;
  // TXPNT& operator=(TXPNT&&) = default;
  //~TXPNT() = default;
};

class TXOFF // textured fill offset
{
  public:
  float   y {0.0F};
  int32_t line {0};

  // constexpr TXOFF() noexcept = default;
  // TXOFF(TXOFF&&) = default;
  // TXOFF& operator=(TXOFF const& rhs) = default;
  // TXOFF& operator=(TXOFF&&) = default;
  //~TXOFF() = default;
};

class STREX // thred v1.0 file header extension
{
  public:
  float  hoopSizeX {0.0F};           // hoop size x dimension
  float  hoopSizeY {0.0F};           // hoop size y dimension
  float  stgran {0.0F};              // stitches per millimeter
  char   creatorName[NAME_LEN] {0};  // name of the file creator NOLINT(modernize-avoid-c-arrays)
  char   modifierName[NAME_LEN] {0}; // name of last file modifier NOLINT(modernize-avoid-c-arrays)
  int8_t auxFormat {0};              // auxillary file format
  int8_t stres {0};                  // reserved
  uint32_t texturePointCount {0U};   // textured fill point count
  int8_t   res[RES_SIZE] {0};        // reserved for expansion NOLINT(modernize-avoid-c-arrays)

  constexpr STREX() noexcept = default;
  // STREX(STREX&&) = default;
  // STREX& operator=(STREX const& rhs) = default;
  // STREX& operator=(STREX&&) = default;
  //~STREX() = default;
};

class PCSTCH
{
  public:
  uint8_t fx {0U}; // fractional part of x coord or color if tag = 3
  int16_t x {0};   // integer part of x coord
  uint8_t nx {0U};
  uint8_t fy {0U}; // fractional part of y coord
  int16_t y {0U};  // integer part of y coord
  uint8_t ny {0U};
  uint8_t tag {0U};

  constexpr PCSTCH() noexcept = default;
  // PCSTCH(PCSTCH&&) = default;
  // PCSTCH& operator=(PCSTCH const& rhs) = default;
  // PCSTCH& operator=(PCSTCH&&) = default;
  //~PCSTCH() = default;
};

class CLPSTCH
{
  public:
  uint32_t led {0U};  // stitchcount in the first record and color otherwise
  uint8_t  fx {0U};   // fractional part of x coord
  uint16_t x {0U};    // integer part of x coord
  uint8_t  spcx {0U}; // ToDo - Is this structure member needed?
  uint8_t  fy {0U};   // fractional part of y coord
  uint16_t y {0U};    // integer part of y coord
  uint8_t  spcy {0U}; // ToDo - Is this structure member needed?
  uint8_t  myst {0U}; // ToDo - Is this structure member needed?
  uint8_t  tag {0U};  // ToDo - Is this structure member needed?

  constexpr CLPSTCH() noexcept = default;
  // CLPSTCH(CLPSTCH&&) = default;
  // CLPSTCH& operator=(CLPSTCH const& rhs) = default;
  // CLPSTCH& operator=(CLPSTCH&&) = default;
  //~CLPSTCH() = default;
};

class BAKHED
{
  public:
  uint32_t    formCount {0U};
  FRMHED*     forms {nullptr};
  uint32_t    stitchCount {0U};
  fPOINTATTR* stitches {nullptr};
  uint32_t    vertexCount {0U};
  fPOINT*     vertices {nullptr};
  uint32_t    guideCount {0U};
  SATCON*     guide {nullptr};
  uint32_t    clipPointCount {0U};
  fPOINT*     clipPoints {nullptr};
  COLORREF*   colors {nullptr};
  TXPNT*      texturePoints {nullptr};
  uint32_t    texturePointCount {0U};
  POINT       zoomRect {0, 0};

  constexpr BAKHED() noexcept = default;
  // BAKHED(CLPSTCH&&) = default;
  // BAKHED& operator=(BAKHED const& rhs) = default;
  // BAKHED& operator=(BAKHED&&) = default;
  //~BAKHED() = default;
};

class FLSIZ
{
  public:
  float cx {0.0F};
  float cy {0.0F};

  // constexpr FLSIZ() noexcept = default;
  // FLSIZ(FLSIZ&&) = default;
  // FLSIZ& operator=(FLSIZ const& rhs) = default;
  // FLSIZ& operator=(FLSIZ&&) = default;
  //~FLSIZ() = default;
};

class FRMRANGE
{
  public:
  uint32_t start {0U};
  uint32_t finish {0U};

  constexpr FRMRANGE() noexcept = default;
  // FRMRANGE(FRMRANGE&&) = default;
  // FRMRANGE& operator=(FRMRANGE const& rhs) = default;
  // FRMRANGE& operator=(FRMRANGE&&) = default;
  //~FRMRANGE() = default;
};

class RANGE
{
  public:
  uint32_t start {0U};
  uint32_t finish {0U};

  // constexpr RANGE() noexcept = default;
  // RANGE(RANGE&&) = default;
  // RANGE& operator=(RANGE const& rhs) = default;
  // RANGE& operator=(RANGE&&) = default;
  //~RANGE() = default;
};

class REGION // region for sequencing vertical fills
{
  public:
  uint32_t start {0U};       // start line of region
  uint32_t end {0U};         // end line of region
  uint32_t regionBreak {0U}; // ToDo - Is this member needed?
  uint32_t breakCount {0U};

  constexpr REGION() noexcept = default;
  inline REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept;
  // REGION(REGION&&) = default;
  // REGION& operator=(REGION const& rhs) = default;
  // REGION& operator=(REGION&&) = default;
  //~REGION() = default;
};

inline REGION::REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept :
    start(rhsStart), end(rhsEnd), regionBreak(rhsBreak), breakCount(rhsCount) {
}

class RCON // PathMap: path map for sequencing
{
  public:
  uint32_t node {0U};
  bool     isConnected {false};
  uint32_t nextGroup {0U};

  // constexpr RCON() noexcept = default;
  // RCON(RCON&&) = default;
  // RCON& operator=(RCON const& rhs) = default;
  // RCON& operator=(RCON&&) = default;
  //~RCON() = default;
};

class RGSEQ // TempPath: temporary path connections
{
  public:
  uint32_t pcon {0U}; // pointer to PathMap entry
  int32_t  count {0};
  bool     skp {false}; // path not found

  constexpr RGSEQ() noexcept = default;
  // RGSEQ(RGSEQ&&) = default;
  // RGSEQ& operator=(RGSEQ const& rhs) = default;
  // RGSEQ& operator=(RGSEQ&&) = default;
  //~RGSEQ() = default;
};

class FSEQ // SequencePath: path of sequenced regions
{
  public:
  uint16_t node {0U};
  uint16_t nextGroup {0U};
  bool     skp {false}; // path not found

  // constexpr FSEQ() noexcept = default;
  // FSEQ(FSEQ&&) = default;
  // FSEQ& operator=(FSEQ const& rhs) = default;
  // FSEQ& operator=(FSEQ&&) = default;
  //~FSEQ() = default;
};

constexpr auto SEQTOP = 2;
constexpr auto SEQBOT = 3;

class PVEC
{
  public:
  float angle {0.0F};
  float length {0.0F};

  // constexpr PVEC() noexcept = default;
  // PVEC(PVEC&&) = default;
  // PVEC& operator=(PVEC const& rhs) = default;
  // PVEC& operator=(PVEC&&) = default;
  //~PVEC() = default;
};

class VRCT2
{
  public:
  fPOINT aipnt {};
  fPOINT aopnt {};
  fPOINT bipnt {};
  fPOINT bopnt {};
  fPOINT cipnt {};
  fPOINT copnt {};
  fPOINT dipnt {};
  fPOINT dopnt {};

  constexpr VRCT2() noexcept = default;
  // VRCT2(VRCT2&&) = default;
  // VRCT2& operator=(VRCT2 const& rhs) = default;
  // VRCT2& operator=(VRCT2&&) = default;
  //~VRCT2() = default;
};

class CURSORMASK
{
  public:
  uint8_t form[128] {0U};            // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint8_t dline[128] {0U};           // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint8_t uprightNeedle[128] {0U};   // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint8_t leftUpNeedle[128] {0U};    // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint8_t leftDownNeedle[128] {0U};  // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint8_t rightUpNeedle[128] {0U};   // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint8_t rightDownNeedle[128] {0U}; // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)

  constexpr CURSORMASK() noexcept = default;
  // CURSORMASK(CURSORMASK&&) = default;
  // CURSORMASK& operator=(CURSORMASK const& rhs) = default;
  // CURSORMASK& operator=(CURSORMASK&&) = default;
  //~CURSORMASK() = default;
};

class BALHED // balarad file header
{
  public:
  COLORREF color[256] {0U}; // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)
  uint32_t signature {0U};
  uint16_t version {0U};
  float    hoopSizeX {0.0F};
  float    hoopSizeY {0.0F};
  COLORREF backgroundColor {0U};
  uint8_t  res[1006] {0U}; // NOLINT(modernize-avoid-c-arrays, readability-magic-numbers)

  constexpr BALHED() noexcept = default;
  // BALHED(BALHED&&) = default;
  // BALHED& operator=(BALHED const& rhs) = default;
  // BALHED& operator=(BALHED&&) = default;
  //~BALHED() = default;
};

class BALSTCH // balarad stitch
{
  public:
  uint8_t code {0U};
  uint8_t flag {0U};
  float   x {0.0F};
  float   y {0.0F};

  // constexpr BALSTCH() noexcept = default;
  // BALSTCH(BALSTCH&&) = default;
  // BALSTCH& operator=(BALSTCH const& rhs) = default;
  // BALSTCH& operator=(BALSTCH&&) = default;
  //~BALSTCH() = default;
};

class CLPSEG
{
  public:
  uint32_t start {0U};
  float    beginLength {0.0F};
  uint32_t beginIndex {0U};
  uint32_t asid {0U};
  uint32_t finish {0U};
  float    endLength {0.0F};
  uint32_t endIndex {0U};
  uint32_t zsid {0U};
  int8_t   dun {0};

  // constexpr CLPSEG() noexcept = default;
  // CLPSEG(CLPSEG&&) = default;
  // CLPSEG& operator=(CLPSEG const& rhs) = default;
  // CLPSEG& operator=(CLPSEG&&) = default;
  //~CLPSEG() = default;
};

class CLIPSORT
{
  public:
  float    segmentLength {0.0F};
  float    sideLength {0.0F};
  uint32_t vertexIndex {0};
  fPOINT   point;

  constexpr CLIPSORT() noexcept = default;
  // CLIPSORT(CLIPSORT&&) = default;
  // CLIPSORT& operator=(CLIPSORT const& rhs) = default;
  // CLIPSORT& operator=(CLIPSORT&&) = default;
  //~CLIPSORT() = default;
};

class CLIPNT
{
  public:
  float    x {0.0F};
  float    y {0.0F};
  uint32_t vertexIndex {0U};
  uint32_t flag {0U};

  // constexpr CLIPNT() noexcept = default;
  // CLIPNT(CLIPNT&&) = default;
  // CLIPNT& operator=(CLIPNT const& rhs) = default;
  // CLIPNT& operator=(CLIPNT&&) = default;
  //~CLIPNT() = default;
};

class VCLPX
{
  public:
  uint32_t segment {0U};
  uint32_t vertex {0U};

  // constexpr VCLPX() noexcept = default;
  // VCLPX(VCLPX&&) = default;
  // VCLPX& operator=(VCLPX const& rhs) = default;
  // VCLPX& operator=(VCLPX&&) = default;
  //~VCLPX() = default;
};

class TRCPNT
{
  public:
  int16_t x {0};
  int16_t y {0};

  // constexpr TRCPNT() noexcept = default;
  // TRCPNT(TRCPNT&&) = default;
  // TRCPNT& operator=(TRCPNT const& rhs) = default;
  // TRCPNT& operator=(TRCPNT&&) = default;
  //~TRCPNT() = default;
};

class GRDCOD
{
  public:
  uint32_t id {0U};
  uint32_t col {0U};

  // constexpr GRDCOD() noexcept = default;
  // GRDCOD(GRDCOD&&) = default;
  // GRDCOD& operator=(GRDCOD const& rhs) = default;
  // GRDCOD& operator=(GRDCOD&&) = default;
  //~GRDCOD() = default;
};

#pragma pack(pop)

class OREC
{
  public:
  uint32_t start {0U};
  uint32_t finish {0U};
  uint32_t startStitch {0U};
  uint32_t endStitch {0U};
  uint32_t color {0U};
  uint32_t type {0U};
  uint32_t form {0U};
  uint32_t otyp {0U};

  constexpr OREC() noexcept = default;
  // OREC(OREC&&) = default;
  // OREC& operator=(OREC const& rhs) = default;
  // OREC& operator=(OREC&&) = default;
  //~OREC() = default;
};

class SRTREC
{
  public:
  uint32_t start {0U};         // start region index
  uint32_t finish {0U};        // finish region index
  uint32_t count {0U};         // number of regions in range
  uint32_t currentRegion {0U}; // current region index
  bool     direction = false;  // direction of sort

  constexpr SRTREC() noexcept = default;
  // SRTREC(SRTREC&&) = default;
  // SRTREC& operator=(SRTREC const& rhs) = default;
  // SRTREC& operator=(SRTREC&&) = default;
  //~SRTREC() = default;
};

class FSTRTS
{
  public:
  uint32_t applique {0U};
  uint32_t fill {0U};
  uint32_t feather {0U};
  uint32_t border {0U};
  uint32_t appliqueColor {0U};
  uint32_t fillColor {0U};
  uint32_t featherColor {0U};
  uint32_t borderColor {0U};

  constexpr FSTRTS() noexcept = default;
  // FSTRTS(FSTRTS&&) = default;
  // FSTRTS& operator=(FSTRTS const& rhs) = default;
  // FSTRTS& operator=(FSTRTS&&) = default;
  //~FSTRTS() = default;
};

union FILLSTARTS {
  public:
  FSTRTS   fillNamed;
  uint32_t fillArray[sizeof(FSTRTS) / sizeof(uint32_t)];

  FILLSTARTS() noexcept;
  // FILLSTARTS(FILLSTARTS&&) = default;
  // FILLSTARTS& operator=(FILLSTARTS const& rhs) = default;
  // FILLSTARTS& operator=(FILLSTARTS&&) = default;
  //~FILLSTARTS() = default;
};

inline FILLSTARTS::FILLSTARTS() noexcept : fillNamed() {
}

constexpr auto M_AP     = 1U << 1U;
constexpr auto M_CWLK   = 1U << 2U;
constexpr auto M_WALK   = 1U << 3U;
constexpr auto M_UND    = 1U << 4U;
constexpr auto M_FIL    = 1U << 5U;
constexpr auto M_FTH    = 1U << 6U;
constexpr auto M_BRD    = 1U << 7U;
constexpr auto M_APCOL  = 1U << 8U;
constexpr auto M_FCOL   = 1U << 9U;
constexpr auto M_FTHCOL = 1U << 10U;
constexpr auto M_ECOL   = 1U << 11U;

class INSREC
{
  public:
  uint32_t code {0U};
  uint32_t color {0U};
  uint32_t index {0U};
  uint32_t seq {0U};

  // constexpr INSREC() noexcept = default;
  // INSREC(INSREC&&) = default;
  // INSREC& operator=(INSREC const& rhs) = default;
  // INSREC& operator=(INSREC&&) = default;
  //~INSREC() = default;
};

enum interleaveTypes { // interleave sequence identifiers
  I_AP,
  I_FIL,
  I_FTH,
  I_BRD,
};

class INTINF
{
  public:
  uint32_t    pins {0U};
  uint32_t    coloc {0U};
  uint32_t    layerIndex {0U};
  uint32_t    start {0U};
  uint32_t    output {0U};
  fPOINTATTR* highStitchBuffer {nullptr};

  constexpr INTINF() noexcept = default;
  // INTINF(INTINF&&) = default;
  // INTINF& operator=(INTINF const& rhs) = default;
  // INTINF& operator=(INTINF&&) = default;
  //~INTINF() = default;
};

class TXTSCR
{
  public:
  int32_t  top {0};                 // pixel top line
  int32_t  bottom {0};              // pixel bottom line
  int32_t  height {0};              // pixel height of area
  int32_t  halfHeight {0};          // pixel middle of area
  float    xOffset {0.0F};          // edit x offset of area
  float    yOffset {0.0F};          // edit y offset of area
  float    areaHeight {0.0F};       // edit height of area
  float    screenHeight {0.0F};     // edit height of screen
  float    width {0.0F};            // edit width of area
  float    spacing {0.0F};          // edit space between lines
  uint16_t lines {0U};              // number of lines
  float    editToPixelRatio {0.0F}; // edit to pixel ratio
  fPOINT   formCenter;              // middle of the form

  constexpr TXTSCR() noexcept = default;
  // TXTSCR(TXTSCR&&) = default;
  // TXTSCR& operator=(TXTSCR const& rhs) = default;
  // TXTSCR& operator=(TXTSCR&&) = default;
  //~TXTSCR() = default;
};

class TXTRCT
{
  public:
  uint16_t left {0U};
  uint16_t right {0U};
  float    top {0.0F};
  float    bottom {0.0F};

  constexpr TXTRCT() noexcept = default;
  // TXTRCT(TXTRCT&&) = default;
  // TXTRCT& operator=(TXTRCT const& rhs) = default;
  // TXTRCT& operator=(TXTRCT&&) = default;
  //~TXTRCT() = default;
};

class TXHST
{
  public:
  std::vector<TXPNT> texturePoints {};

  float height {0.0F};
  float width {0.0F};
  float spacing {0.0F};

  constexpr TXHST() noexcept = default;
  // TXHST(TXHST&&) = default;
  // TXHST& operator=(TXHST const& rhs) = default;
  // TXHST& operator=(TXHST&&) = default;
  //~TXHST() = default;
};

class TXHSTBUF
{
  public:
  uint32_t* placeholder {nullptr};
  uint32_t  count {0U};

  float height {0.0F};
  float width {0.0F};
  float spacing {0.0F};

  constexpr TXHSTBUF() noexcept = default;
  // TXHSTBUF(TXHSTBUF&&) = default;
  // TXHSTBUF& operator=(TXHSTBUF const& rhs) = default;
  // TXHSTBUF& operator=(TXHSTBUF&&) = default;
  //~TXHSTBUF() = default;
};

class RNGCNT
{
  public:
  int32_t line {0};
  int32_t stitchCount {0};

  constexpr RNGCNT() noexcept = default;
  // RNGCNT(RNGCNT&&) = default;
  // RNGCNT& operator=(RNGCNT const& rhs) = default;
  // RNGCNT& operator=(RNGCNT&&) = default;
  //~RNGCNT() = default;
};

constexpr auto BADFLT = 1U;
constexpr auto BADCLP = 1U << 1U;
constexpr auto BADSAT = 1U << 2U;
constexpr auto BADTX  = 1U << 3U;

class BADCNTS
{
  public:
  uint32_t attribute {0U};
  uint32_t flt {0U};
  uint32_t clip {0U};
  uint32_t guideCount {0U}; // ToDo - is this an accurate description?
  uint32_t tx {0U};

  constexpr BADCNTS() noexcept = default;
  // BADCNTS(BADCNTS&&) = default;
  // BADCNTS& operator=(BADCNTS const& rhs) = default;
  // BADCNTS& operator=(BADCNTS&&) = default;
  //~BADCNTS() = default;
};

class FINDINFO
{
  public:
  uint32_t         count {0U};
  WIN32_FIND_DATA* data {nullptr};

  constexpr FINDINFO() noexcept = default;
  // FINDINFO(FINDINFO&&) = default;
  // FINDINFO& operator=(FINDINFO const& rhs) = default;
  // FINDINFO& operator=(FINDINFO&&) = default;
  //~FINDINFO() = default;
};

// is intersection of line and cursor in, before or after the line
enum class intersectionStyles { POINT_IN_LINE = 0, POINT_BEFORE_LINE, POINT_AFTER_LINE };

class LENINFO // Sorted line length info
{
  public:
  uint32_t index {0U};
  bool     isEnd {false};
  float    length {0.0F};

  // constexpr LENINFO() noexcept = default;
  // LENINFO(LENINFO&&) = default;
  // LENINFO& operator=(LENINFO const& rhs) = default;
  // LENINFO& operator=(LENINFO&&) = default;
  //~LENINFO() = default;
};

class PECCOLOR
{
  public:
  uint8_t r {0U};
  uint8_t g {0U};
  uint8_t b {0U};

  // constexpr PECCOLOR() noexcept = default;
  // PECCOLOR(PECCOLOR&&) = default;
  // PECCOLOR& operator=(PECCOLOR const& rhs) = default;
  // PECCOLOR& operator=(PECCOLOR&&) = default;
  //~PECCOLOR() = default;
};

class PESCOLORLIST
{
  public:
  uint16_t blockIndex {0U};
  uint16_t colorIndex {0U};

  // constexpr PESCOLORLIST() noexcept = default;
  // PESCOLORLIST(PESCOLORLIST&&) = default;
  // PESCOLORLIST& operator=(PECCOLORLIST const& rhs) = default;
  // PESCOLORLIST& operator=(PESCOLORLIST&&) = default;
  //~PESCOLORLIST() = default;
};

class THREAD
{
  public:
  PECCOLOR    color {};
  char const* description {nullptr};
  char const* catalogNumber {nullptr};

  // constexpr THREAD() noexcept = default;
  // THREAD(THREAD&&) = default;
  // THREAD& operator=(THREAD const& rhs) = default;
  // THREAD& operator=(THREAD&&) = default;
  //~THREAD() = default;
};

// Do the type punning while ensuring that the returned pointer is non_null
// use the encapsulation recommended in I.30
// (https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i30-encapsulate-rule-violations)
template <class T2, class T1> inline _Ret_notnull_ auto convert_ptr(T1* pointer) -> T2 {
  if (pointer) {
	return gsl::narrow_cast<T2>(gsl::narrow_cast<void*>(pointer));
  }

  throw;
}
