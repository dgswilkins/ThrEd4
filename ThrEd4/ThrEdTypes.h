#pragma once

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "switches.h"

constexpr int32_t RES_SIZE    = 26;       // reserved for expansion in the ThrEd v1.0 header
constexpr int32_t NAME_LEN    = 50;       // Length of the name fields in ThrEd headers
constexpr int32_t COLOR_COUNT = 16;       // Number of colors in arrays
constexpr int32_t SRTIM       = 20000000; // sort time limit in 100 ns intervals

// daisy codes
constexpr uint32_t DAZPETS = 5u;  // petals
constexpr uint32_t DAZCNT  = 10u; // petal points
constexpr uint32_t DAZICNT = 2u;  // petal inner points
constexpr uint32_t DAZLEN  = 15u; // diameter
constexpr uint32_t DAZPLEN = 20u; // petal length
constexpr uint32_t DAZHLEN = 5u;  // hole size
constexpr uint32_t DAZTYP  = 5u;  // border type
constexpr uint32_t DAZMCNT = 7u;  // mirror count
// end of daisy codes

constexpr float    TXTRAT   = 0.95f;       // texture fill clipboard shrink/grow ratio
constexpr uint32_t MAXMSK   = 0xffff0000u; // for checking for greater than 65536
constexpr uint16_t DEFBPIX  = 4u;          // default form box pixels
constexpr float    MAXWLK   = 54.0f;       // max underlay/edge walk stitch length
constexpr float    MINWLK   = 2.4f;        // max underlay/edge walk stitch length
constexpr float    DEFULEN  = 12.0f;       // default underlay stitch length
constexpr float    DEFUSPAC = 6.0f;        // default underlay stitch spacing
constexpr uint16_t IWAVPNTS = 36u;         // default wave points
constexpr uint16_t IWAVSTRT = 10u;         // default wave start
constexpr uint16_t IWAVEND  = 26u;         // default wave end
constexpr uint16_t IWAVS    = 5u;          // default wave lobes
constexpr uint32_t THRLED0  = 0x746872u;   // lead dword value for thred file v 1.0
constexpr uint32_t THRLED1  = 0x1746872u;  // lead dword value for thred file v 1.1
constexpr float    ZUMFCT   = 0.65f;       // zoom factor
constexpr float    PAGSCROL = 0.9f;        // page scroll factor
constexpr float    LINSCROL = 0.05f;       // line scroll factor
constexpr uint32_t TXTSIDS  = 6u;          // extra pixels in a text box
constexpr uint32_t MAXITEMS = 0x10000u;    // maximum number of stitches, sequence items & clipboard points
constexpr uint32_t MAXCHNG  = 10000u;      // maximum number of color changes
constexpr float    SHUPX    = 480.0f;      // small hoop x size
constexpr float    SHUPY    = 480.0f;      // small hoop y size
constexpr float    LHUPX    = 719.0f;      // large hoop x size
constexpr float    LHUPY    = 690.0f;      // large hoop y size
constexpr float    HUP100XY = 600.0f;      // 100 millimeter hoop size
constexpr float    PFGRAN   = 6.0f;        // pfaff "pixels" per millimeter
constexpr double   TSIZ30   = 0.3;         // #30 thread size in millimeters
constexpr double   TSIZ40   = 0.2;         // #40 thread size in millimeters
constexpr double   TSIZ60   = 0.05;        // #60 thread size in millimeters
constexpr int32_t  SCROLSIZ = 12;          // logical pixel width of a scroll bar
constexpr int32_t  COLSIZ   = 12;          // logical pixel width of the color bar
// ToDo - Should this be a configurable parameter?
constexpr double   CLOSENUF   = 15.0;              // mouse click region for select
constexpr float    ZMARGIN    = 1.25f;             // zoom margin for select zooms
constexpr float    SMALSIZ    = 0.25f;              // default small stitch size
constexpr float    MINSIZ     = 0.1f;              // default minimum stitch size
constexpr float    USESIZ     = 3.5f;              // user preferred size
constexpr double   MAXSIZ     = 9.0;               // default maximum stitch size
constexpr float    PFAFGRAN   = 6.0f;              // pfaf stitch points per millimeter
constexpr int32_t  MINZUM     = 5;                 // minimum zoom in stitch points
constexpr double   SHOPNTS    = 0.00;              // show stitch points when zoom below this
constexpr double   STCHBOX    = 0.4226;            // show stitch boxes when zoom below this
constexpr uint32_t BITCOL     = 0xffff00u;         // default bitmap color
constexpr uint32_t MAXFORMS   = 1024u;             // maximum number of forms
constexpr double   FORMFCT    = 0.05;              // new forms part of screen
constexpr int32_t  MAXDELAY   = 600;               // maximum movie time step
constexpr int32_t  MINDELAY   = 1;                 // minimum movie time step
constexpr int32_t  MOVITIM    = 12;                // default movie time
constexpr float    DEFSPACE   = 0.45f;             // default stitch spacing
constexpr float    DEFANG     = 0.7853981f;        // default fill angle, 45 degrees
constexpr uint32_t MAXFRMLINS = 20000u;            // maximum lines in a form
constexpr uint32_t MSGSIZ     = 8192;              // size of the message buffer
constexpr float    PI_F       = 3.1415927f;        // PI to single precision
constexpr double   PI         = 3.141592653589793; // PI to double precision
constexpr double   PI2        = PI * 2.0;          //
constexpr double   MAXSTCH    = 54.0;              // maximum permitted stitch length for pfaf in pfaf "stitch pixels"
constexpr double   USPAC      = 15.0;              // underlay fill spacing
constexpr float    APSPAC     = 10.8f;             // applique border spacing
constexpr uint32_t OSEQLEN    = 0x40000u;          // output sequence length
constexpr uint32_t BSEQLEN    = OSEQLEN * 2u;      // reverse sequence length
constexpr float    URAT       = 0.75f;             // ratio of underlay stitch to satin border size
constexpr double   PURAT      = 0.6;               // for proportional satin corners

constexpr float DIURAT = (1.0f - URAT) / 2.0f;        //(1-URAT)/2
constexpr float DOURAT = (1.0f - URAT) / 2.0f + URAT; //(1-URAT)/2+URAT

constexpr float    MINRCT    = 12.0f;           // minimum dimension of a form select rectangle
constexpr uint32_t OLDNUM    = 4u;              // number of old filenames saved on file menu
constexpr int8_t   OLDVER    = 4;               // number of old file versions kept
constexpr float    TINY      = 1e-6f;           // tiny number for floating point stuff
constexpr int32_t  SPEDLIN   = 30;              // speed change for line message on speed scroll bar
constexpr int32_t  SPEDPAG   = 120;             // speed change for page message on speed scroll bar
constexpr int32_t  KNOTLEN   = 54;              // set knots for stitches longer than this
constexpr int32_t  MAXKNOTS  = 16384;           // maximum number of knots
constexpr float    IBFCLEN   = (4.0f * PFGRAN); // initial buttonhole fill corner length
constexpr float    IPICSPAC  = 6.0f;            // initial picot border space
constexpr int32_t  PRFLINS   = 28;              // number of lines on the preference menu
constexpr uint32_t EDGETYPS  = 12u;             // number of border fill types
constexpr uint32_t SEED      = 3037000499;      // pseudo-random-sequence seed
constexpr uint32_t FSED      = 1340007303;      // feather sequence seed
constexpr double   HUPS      = 5u;              // number of hoops the user can select
constexpr uint32_t NORDSED   = 0x5a5a5a5au;     // name order seed
constexpr uint32_t NCODSED   = 0x73ef5a7eu;     // name encoding seed
constexpr uint8_t  NCODOF    = 80u;             // name encoding offset
constexpr double   CLPMIN    = 0.5;             // if clipboard data less wide, then don't fill
constexpr float    CLPMINAUT = 1.2f;            // for skinny vertical clips
constexpr float    BRDWID    = 18.0f;           // default satin border size
constexpr float    SNPLEN    = 0.15f;            // default snap together length size
constexpr float    STARAT    = 0.4f;            // default star ratio
constexpr float    SPIRWRAP  = 1.52f;           // default spiral wrap
constexpr uint32_t BALNORM   = 0x80u;           // normal balarad stitch
constexpr uint32_t BALJUMP   = 0x81u;           // balarad jump stitch
constexpr uint32_t BALSTOP   = 0;               // balarad stop
constexpr uint32_t COLVER    = 0x776874u;       // color file version
constexpr uint32_t REDCOL    = 0x0000ffu;       // code for the color red
constexpr uint32_t GRNCOL    = 0x00ff00u;       // code for the color green
constexpr uint32_t BLUCOL    = 0xff0000u;       // code for the color blue
constexpr uint32_t REDMSK    = 0xffff00u;       // mask for the color red
constexpr uint32_t GRNMSK    = 0xff00ffu;       // mask for the color green
constexpr uint32_t BLUMSK    = 0x00ffffu;       // mask for the color blue
constexpr float    TRACLEN   = 1.0f;            // initial trace length
constexpr double   TRACRAT   = 1.00001;         // initial trace ratio
constexpr float    CHSDEF    = 24.0f;           // default chain stitch length
constexpr double   CHRDEF    = 0.25;            // default chain stitch ratio
constexpr float    NUGINI    = 2.0f;            // default nudge step
constexpr uint16_t DEFPIX    = 2u;              // default nudge pixels
constexpr double   DEFEGRAT  = 1.5;             // default egg ratio
constexpr uint16_t DEFPNTPIX = 4u;              // default form and stitch point32_t pixels
constexpr int32_t  HBUFSIZ   = 1024;            // help buffer size
constexpr uint32_t HIGRD     = 0xffffffu;       // grid high color
constexpr uint32_t MEDGRD    = 0x404040u;       // grid medium color
constexpr uint32_t DEFGRD    = 0x202020u;       // grid default color
constexpr uint32_t REDGRD    = 0xff2020u;       // grid red color
constexpr uint32_t BLUGRD    = 0x20ff20u;       // grid green color
constexpr uint32_t GRNGRD    = 0x2020ffu;       // grid blue color
constexpr float    FDEFRAT   = 0.6f;            // default feather ratio
constexpr uint8_t  FDEFUP    = 10u;             // default feather up count
constexpr uint8_t  FDEFDWN   = 5;               // default feather down count
constexpr float    FDEFFLR   = 9.0f;            // default feather floor
constexpr uint16_t FDEFNUM   = 10u;             // default feather number
constexpr float    ITXHI     = (9.0f * PFGRAN); // default texture editor height
constexpr float    ITXWID    = (9.0f * PFGRAN); // default texture editor width
constexpr float    ITXSPAC   = (0.4f * PFGRAN); // default texture editor spacing
constexpr uint16_t ITXPIX    = 5u;              // default texture editor cross pixels
constexpr uint32_t ITXBUFLEN = 16u;             // texture buffer depth

enum stringLabel
{
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
enum traceDirection
{
	TRCU, // top edge
	TRCR, // right edge
	TRCD, // bottom edge
	TRCL  // left edge
};

enum hoopSize
{
	SETCUST = 1, // set the custom hoop
	SMALHUP = 2, // pfaf code for small hoop
	LARGHUP,     // pfaf code for large hoop
	HUP100,      // 100 millimeter hoop
	CUSTHUP      // user defined hoop size
};

enum daisyStyles
{          // daisy form types
	DSIN,  // Sine shape
	DRAMP, // Ramp shape
	DSAW,  // Sawtooth shape
	DRAG,  // Ragged shape
	DCOG,  // Cog shape
	DHART  // Heart shape
};

// bitmap
enum class StateFlag
{
	SATIN,     // user is entering a satin stitch form
	SATPNT,    // user is entering points for a satin stitch form
	BOXZUM,    // box zoom select mode
	BZUMIN,    // display the zoom box
	BZUM,      // zoom box displayed on screen
	INSRT,     // insert mode active
	SELPNT,    // user has clicked on a selected point
	SATCNKT,   // user is connecting points in a satin form
	INSFRM,    // user is inserting points in a form
	FORMIN,    // user is selecting a form
	PRFACT,    // user is selecting preferences
	POLIMOV,   // user is placing a pre defined form
	SIZSEL,    // thread size select boxes displayed
	THRDS,     // State of the threads/lines box
	LINED,     // line has been started
	LININ,     // line is in the zoom rectangle
	ZUMED,     // stitch window is zoomed
	COL,       // state of the color/all box
	HID,       // state of the show/hide button
	ISDWN,     // down stitch exists
	ISUP,      // up stitch exists
	WASLIN,    // a move stitch line has been drawn
	CAPT,      // screen capture in effect
	VCAPT,     // keep screen capture as int32_t as the mouse cursor
	           // is in the edit window
	SELBOX,    // the select box is showing
	ILIN,      // insert line is showing on the screen
	INIT,      // set when a stitch is entered in the stitch buffer,a form is entered, or the mark is set
	LIN1,      // drawing only one line in stitch insert mode
	ILIN1,     // one insert line showing on screen
	GRPSEL,    // group select mode
	SCROS,     // start group cross has been drawn
	ECROS,     // end group cross has been drawn
	ELIN,      // group select line has been drawn
	BAKEND,    // insert data should be appended to the end of the file
	CLPSHO,    // display the clipboard insert rectangle
	SELSHO,    // display the select rectangle
	ROTAT,     // rotate state
	ROTSHO,    // rotate rectangle is showing
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
	FRMPMOV,   // user is moving a point32_t on a form
	IGNOR,     // user may elect to ignore a message about losing edits when resizing a form
	FRMOV,     // user is moving a form
	RUNPAT,    // user is running a pattern
	WASPAT,    // user ran a pattern, but hasn't done anything else yet
	FILDIR,    // direction lines in angle fill
	SHOSAT,    // the satin stitch form is on the screen
	SHOCON,    // connect point32_t line is visible
	FENDIN,    // user is selecting fill ends
	DELFRM,    // user wants to delete a form
	BAKACT,    // there are entries in the undo buffer
	BAKWRAP,   // undo buffer pointer has wrapped
	BAKING,    // user has called the undo function
	REDUSHO,   // redo menu item is active
	UNDUSHO,   // undo menu item is active
	FUNCLP,    // user is loading a form from the clipboard
	FRMPSEL,   // user has selected a point32_t in a form
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
	DUBAK,     // user has chosen a backup file to load, but may want to save changes
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
	MOVSET,    // the move point32_t has been selected
	UPTO,      // show points up to the selected point
	LENSRCH,   // user has hit the max or min button
	BOXSLCT,   // user is making a select box
	THUMSHO,   // thumbnail mode
	INSFIL,    // user is inserting a file
	IGNORINS,  // don't use the open file dialog box to get the insert file name
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
	CONTIG,    // contiguous point32_t flag
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
	PRELIN,    // user is inserting form points before the 0 point32_t in a line form
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
	WASNEG,    // form had a point32_t less than the size of the clipboard fill
	FCLOS,     // user is closing a file
	NOTFREE,   // no free space on the drive
	PIXIN,     // user in inputting nudge pixels
	STPXIN,    // user is inputting stitch point32_t pixels
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
	DUHI,      // paint32_t the high trace rectangle
	DULO,      // paint32_t the low trace rectangle
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
	UNDCOL,    // user is entrering an underlay color
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
	TXIN,      // last stitch point32_t was in stitch select box
	SCLPSPAC,  // user is setting the clipbard fill spacing
	FCHK,      // check the forms
	NOCLP,     // don't load clipboard data from forms
	NOTHRFIL,  // loaded file wasn't a Thred file

	EnumCount // must be the last entry in the enum
};

// user bitmap
enum class UserFlag
{
	SQRFIL,   // square ends on fills
	BLUNT,    // blunt ends on satin lines
	NEDOF,    // needle cursor off
	KNOTOF,   // don't show knots
	BSAVOF,   // don't save PCS bitmaps
	LINSPAC,  // evenly space border line points
	DUND,     // underlay in satin borders
	FIL2OF,   // fill to select off
	ROTAUX,   // rotate the aux file when saving
	SAVMAX,   // thred window maximized
	MARQ,     // remove mark when user hits escape only
	FRMX,     // cross cursor for form entry
	DAZHOL,   // daisy hole
	DAZD,     // daisy d-lines
	CHREF,    // refill forms when changing design size
	WRNOF,    // warn if edited off
	EnumCount // must be the last entry in the enum
};

enum featherStyles
{               // feather fill types
	FTHSIN = 1, // sine
	FTHSIN2,    // half sine
	FTHLIN,     // line
	FTHPSG,     // psuedo-random sequence
	FTHRMP,     // sawtooth
	FTHFAZ      // phase
};

constexpr int32_t FDEFTYP = FTHPSG; // default feather type

// edge fill types
enum edgeFillStyles
{
	EDGELINE = 1, // Line
	EDGEBEAN,     // Bean
	EDGECLIP,     // Clipboard
	EDGEANGSAT,   // Angle Satin
	EDGEAPPL,     // Applique
	EDGEPROPSAT,  // Proportional Satin
	EDGEBHOL,     // Buttonhole
	EDGEPICOT,    // Picot
	EDGEDOUBLE,   // Double
	EDGELCHAIN,   // Line Chain
	EDGEOCHAIN,   // Open Chain
	EDGECLIPX,    // Even Clipboard
	EDGELAST      // must always be the last value
};

// form types
enum formStyles
{
	FRMLINE = 1u, // Line form
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
enum mainMenuItems
{
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
enum viewMenuItems
{
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
enum fillMenuItems
{
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
enum clipDataType
{
	CLP_STCH = 1,
	CLP_FRM,
	CLP_FRMS,
	CLP_FRMPS,
};

// edge underlay bit
constexpr uint32_t EGUND  = 0x80u;
constexpr uint32_t NEGUND = 0x7fu;

// form data lines
enum formData
{
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
enum fillStyles
{
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
constexpr uint32_t MCLPF    = 1 << CLPF;
constexpr uint32_t MVCLPF   = 1 << VCLPF;
constexpr uint32_t MHCLPF   = 1 << HCLPF;
constexpr uint32_t MANGCLPF = 1 << ANGCLPF;

// preference window
enum preferenceItems
{
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
};

// file menu items
enum fileMenuItems
{
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
enum fillMessageCodes
{
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
enum stitchButtons
{
	HBOXSEL,
	HUPTO,
	HHID,
	HNUM,
	HTOT,
	HMINLEN,
	HMAXLEN,
	HCOR,
	HLAYR
};

enum textureButtons
{ // text button windows
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

#pragma pack(1)

// pcs file header structure
struct _pcsheader {
	int8_t   leadIn;
	int8_t   hoopType;
	uint16_t colorCount;
	COLORREF colors[COLOR_COUNT]; // NOLINT
	uint16_t stitchCount;
};

using PCSHEADER = struct _pcsheader;

// ini file structure
struct _iniFil {
	char     defaultDirectory[180];                  // default directory                   NOLINT
	COLORREF stitchColors[COLOR_COUNT];              // colors                              NOLINT
	COLORREF stitchPreferredColors[COLOR_COUNT];     // stitch preference colors            NOLINT
	COLORREF backgroundPreferredColors[COLOR_COUNT]; // background preference colors        NOLINT
	COLORREF backgroundColor;                        // background color
	COLORREF bitmapColor;                            // bitmap color
	float    minStitchLength;                        // minimum stitch length
	double   showStitchThreshold;                    // show stitch points
	double   threadSize30;                           // millimeter size of 30 weight thread
	double   threadSize40;                           // millimeter size of 40 weight thread
	double   threadSize60;                           // millimeter size of 60 weight thread
	float    userStitchLength;                       // user stitch length
	float    maxStitchLength;                        // maximum stitch length
	double   smallStitchLength;                      // small stitch size
	double   stitchBoxesThreshold;                   // show sitch box level
	float    stitchSpace;                            // stitch spacing between lines of stitches
	float    fillAngle;                              // fill angle
	uint32_t userFlagMap;                            // bitmap for user variables
	float    borderWidth;                            // border width
	uint32_t appliqueColor;                          // applique color
	char     prevNames[OLDNUM][_MAX_PATH];           // last file names                     NOLINT
	double   snapLength;                             // snap together length
	float    starRatio;                              // star ratio
	float    spiralWrap;                             // spiral wrap
	COLORREF bitmapBackgroundColors[COLOR_COUNT];    // bitmap background color preferences NOLINT
	float    buttonholeCornerLength;                 // buttonhole fill corner length
	float    picotSpace;                             // space between border picots
	int8_t   hoopType;                               // hoop type
	int8_t   auxFileType;                            // machine file type
	float    hoopSizeX;                              // hoop x size
	float    hoopSizeY;                              // hoop y size
	float    rotationAngle;                          // rotation angle
	float    gridSize;                               // grid size
	float    clipOffset;                             // clipboard offset
	RECT     initialWindowCoords;                    // initial window coordinates
	COLORREF gridColor;                              // grid color
	uint32_t fillPhase;                              // clipboard fill phase
	float    customHoopX;                            // custom hoop width
	float    customHoopY;                            // custom hoop height
	float    traceLength;                            // lens points
	double   traceRatio;                             // trace ratio
	float    chainSpace;                             // chain space
	float    chainRatio;                             // chain ratio
	float    cursorNudgeStep;                        // cursor nudge step
	uint16_t nudgePixels;                            // nudge pixels
	float    eggRatio;                               // egg ratio
	uint16_t stitchSizePixels;                       // size of stitch points in pixels
	uint16_t formVertexSizePixels;                   // size of form points in pixels
	uint16_t formSides;                              // sides of a created form
	float    tearTailLength;                         // length of the tear tail
	float    tearTwistStep;                          // tear twist step
	float    tearTwistRatio;                         // tear twist ratio
	uint16_t wavePoints;                             // wave points
	uint16_t waveStart;                              // wave strting point
	uint16_t waveEnd;                                // wave ending point;
	uint16_t waveLobes;                              // wave lobes
	uint8_t  featherFillType;                        // feather fill type
	uint8_t  featherUpCount;                         // feather up count
	uint8_t  featherDownCount;                       // feather down count
	uint8_t  featherType;                            // feather bits
	float    featherRatio;                           // feather ratio
	float    featherMinStitchSize;                   // feather floor
	uint16_t featherCount;                           // feather fill psg granularity
	char     p2cName[MAX_PATH];                      // pes2card file                       NOLINT
	float    underlayIndent;                         // edge walk/underlay indent
	float    underlayAngle;                          // underlay angle
	float    underlaySpacing;                        // underlay spacing
	float    underlayStitchLen;                      // underlay stitch length
	float    daisyDiameter;                          // daisy diameter
	float    daisyPetalLen;                          // daisy petal length
	float    daisyHoleDiameter;                      // daisy hole diameter
	uint32_t daisyPetalCount;                        // daisy petals
	uint32_t daisyPetalPoints;                       // daisy petal points
	uint32_t daisyInnerCount;                        // daisy inner count
	uint8_t  daisyBorderType;                        // daisy border type
	uint8_t  dataCheck;                              // data check
	float    textureHeight;                          // textured fill height
	float    textureWidth;                           // textured fill width
	float    textureSpacing;                         // textured fill spacing
	uint16_t formBoxSizePixels;                      // form box pixels
	uint16_t daisyHeartCount;                        // daisy heart count
	uint16_t textureEditorSize;                      // texture editor pixels
	float    clpspc;                                 // clipboard fill spacing
	char     designerName[NAME_LEN];                 // designer name                       NOLINT
};

using INIFILE = struct _iniFil;

enum machineType
{
	AUXPCS,
	AUXDST,
	AUXPES
};

struct _formVertices {
	uint32_t start;
	uint32_t vertexCount;
	uint32_t finish;
	uint32_t form;
};

using FORMVERTICES = struct _formVertices;

class fPOINT;
class fPOINTATTR;
class SMALPNTL;
class BSEQPNT;

class dPOINT
{
public:
	double x { 0.0 };
	double y { 0.0 };

	constexpr dPOINT() noexcept = default;
	explicit inline dPOINT(const fPOINT& rhs) noexcept;
	inline dPOINT(float rhsX, float rhsY) noexcept;
	inline dPOINT(double rhsX, double rhsY) noexcept;
	inline bool    operator==(const dPOINT& rhs) const noexcept;
	inline dPOINT& operator=(const fPOINT& rhs) noexcept;
	inline dPOINT& operator=(const BSEQPNT& rhs) noexcept;
};

class fPOINT
{
public:
	float x { 0.0f };
	float y { 0.0f };

	constexpr fPOINT() noexcept = default;
	inline fPOINT(double rhsX, double rhsY) noexcept;
	inline fPOINT(float rhsX, float rhsY) noexcept;
	inline fPOINT(int32_t rhsX, int32_t rhsY) noexcept;
	explicit inline fPOINT(const dPOINT& rhs) noexcept;
	inline bool    operator==(const fPOINT& rhs) const noexcept;
	inline fPOINT& operator=(const dPOINT& rhs) noexcept;
	inline fPOINT& operator=(const fPOINTATTR& rhs) noexcept;
	inline fPOINT& operator=(const SMALPNTL& rhs) noexcept;
	inline fPOINT& operator=(const BSEQPNT& rhs) noexcept;
};

class fPOINTATTR
{
public:
	float    x { 0.0f };
	float    y { 0.0f };
	uint32_t attribute { 0u };

	constexpr fPOINTATTR() noexcept = default;
	inline fPOINTATTR(double rhsX, double rhsY, uint32_t rhsA) noexcept;
	inline fPOINTATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept;
	// fPOINTATTR(fPOINTATTR&&) = default;
	// fPOINTATTR& operator=(const fPOINTATTR& rhs) = default;
	// fPOINTATTR& operator=(fPOINTATTR&&) = default;
	//~fPOINTATTR() = default;
};

class SMALPNTL
{
public:
	uint32_t line; // line and group must remain in this order for sort to work
	uint32_t group;
	float    x;
	float    y;
};

class BSEQPNT
{
public:
	float  x;
	float  y;
	int8_t attribute;

	inline BSEQPNT() noexcept = default;
	inline BSEQPNT(double rhsX, double rhsY, int32_t rhsAttr);
	inline BSEQPNT(float rhsX, float rhsY, int32_t rhsAttr);
	// BSEQPNT(BSEQPNT&&) = default;
	// BSEQPNT& operator=(const BSEQPNT& rhs) = default;
	// BSEQPNT& operator=(BSEQPNT&&) = default;
	//~BSEQPNT() = default;
};

// pragma required until MSVC /analyze recognizes noexcept(false)
#pragma warning(push)
#pragma warning(disable : 26440)
inline BSEQPNT::BSEQPNT(double rhsX, double rhsY, int32_t rhsAttr) {
	x         = gsl::narrow<float>(rhsX);
	y         = gsl::narrow<float>(rhsY);
	attribute = gsl::narrow<int8_t>(rhsAttr);
}

inline BSEQPNT::BSEQPNT(float rhsX, float rhsY, int32_t rhsAttr)
    : x(rhsX)
    , y(rhsY) {
	attribute = gsl::narrow<int8_t>(rhsAttr);
}
#pragma warning(pop)

inline bool fPOINT::operator==(const fPOINT& rhs) const noexcept {
	return (x == rhs.x) && (y == rhs.y);
}

inline fPOINT::fPOINT(float rhsX, float rhsY) noexcept
    : x(rhsX)
    , y(rhsY) {
}
inline fPOINT::fPOINT(int32_t rhsX, int32_t rhsY) noexcept {
	x = gsl::narrow_cast<float>(rhsX);
	y = gsl::narrow_cast<float>(rhsY);
}

#pragma warning(push)
#pragma warning(disable : 4244)
inline fPOINT::fPOINT(double rhsX, double rhsY) noexcept {
	x = gsl::narrow_cast<float>(rhsX);
	y = gsl::narrow_cast<float>(rhsY);
}

inline fPOINT::fPOINT(const dPOINT& rhs) noexcept {
	x = gsl::narrow_cast<float>(rhs.x);
	y = gsl::narrow_cast<float>(rhs.y);
}

inline fPOINT& fPOINT::operator=(const dPOINT& rhs) noexcept {
	x = gsl::narrow_cast<float>(rhs.x);
	y = gsl::narrow_cast<float>(rhs.y);
	return *this;
}

inline fPOINT& fPOINT::operator=(const SMALPNTL& rhs) noexcept {
	x = gsl::narrow_cast<float>(rhs.x);
	y = gsl::narrow_cast<float>(rhs.y);
	return *this;
}
#pragma warning(pop)

inline fPOINT& fPOINT::operator=(const fPOINTATTR& rhs) noexcept {
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline fPOINT& fPOINT::operator=(const BSEQPNT& rhs) noexcept {
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline fPOINTATTR::fPOINTATTR(float rhsX, float rhsY, uint32_t rhsA) noexcept
    : x(rhsX)
    , y(rhsY)
    , attribute(rhsA) {
}

inline fPOINTATTR::fPOINTATTR(double rhsX, double rhsY, uint32_t rhsA) noexcept {
	x         = gsl::narrow_cast<float>(rhsX);
	y         = gsl::narrow_cast<float>(rhsY);
	attribute = rhsA;
}

inline dPOINT::dPOINT(const fPOINT& rhs) noexcept
    : x(rhs.x)
    , y(rhs.y) {
}

inline dPOINT::dPOINT(float rhsX, float rhsY) noexcept
    : x(rhsX)
    , y(rhsY) {
}

inline dPOINT::dPOINT(double rhsX, double rhsY) noexcept
    : x(rhsX)
    , y(rhsY) {
}
inline bool dPOINT::operator==(const dPOINT& rhs) const noexcept {
	return (x == rhs.x) && (y == rhs.y);
}

inline dPOINT& dPOINT::operator=(const fPOINT& rhs) noexcept {
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline dPOINT& dPOINT::operator=(const BSEQPNT& rhs) noexcept {
	x = rhs.x;
	y = rhs.y;
	return *this;
}

struct _floatPointLine {
	float    x;
	float    y;
	uint16_t line;
};

using fPOINTLINE = struct _floatPointLine;

struct _doublerectangle {
	double top;
	double bottom;
	double left;
	double right;
};

using dRECTANGLE = struct _doublerectangle;

struct _colChng {
	uint16_t stitchIndex; // stitch index
	uint16_t colorIndex;  // color index
};

using COLCHNG = struct _colChng;

class FEATHER
{
public:
	uint32_t fillType;          // type of feather fill
	float    ratio;             // feather ratio
	float    minStitch;         // smallest stitch allowed
	float    ratioLocal;        // local feather ratio
	float    formRatio;         // feather ratio from form
	uint32_t extendedAttribute; // extended form attribute
	uint32_t upCount;           // feather up count
	uint32_t downCount;         // feather down count
	uint32_t totalCount;        // up count plus down count
	uint32_t phaseIndex;
	uint32_t phase;
	uint32_t countUp;
	uint32_t countDown;
	float    globalStep;
	float    globalPosition;
	float    globalPhase;
	float    globalUp;
	float    globalDown;
	float    globalRatio;
};

constexpr uint32_t COLMSK    = 0x0000000fu; // 0000 0000 0000 0000 0000 0000 0000 1111
constexpr uint32_t APCOLMSK  = 0x000000f0u; // 0000 0000 0000 0000 0000 0000 1111 0000
constexpr uint32_t NCOLMSK   = 0xfffffff0u; // 1111 1111 1111 1111 1111 1111 1111 0000
constexpr uint32_t COLSMSK   = 0x0000ffffu; // 0000 0000 0000 0000 1111 1111 1111 1111
constexpr uint32_t FRMSK     = 0x00003ff0u; // 0000 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t NFRMSK    = 0xffffc00fu; // 1111 1111 1111 1111 1100 0000 0000 1111
constexpr uint32_t UFRMSK    = 0x80003ff0u; // 1000 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t TYPFMSK   = 0x20003ff0u; // 0010 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t TYPBMSK   = 0x40003ff0u; // 0100 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t TYPAPMSK  = 0x60003ff0u; // 0110 0000 0000 0000 0011 1111 1111 0000
constexpr uint32_t LAYMSK    = 0x0e000000u; // 0000 1110 0000 0000 0000 0000 0000 0000
constexpr uint32_t NLAYMSK   = 0xf1ffffffu; // 1111 0001 1111 1111 1111 1111 1111 1111
constexpr uint32_t TYPMSK    = 0x60000000u; // 0110 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t TYPFRM    = 0x20000000u; // 0010 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t TYPBRD    = 0x40000000u; // 0100 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t NTYPMSK   = 0x9fffffffu; // 1001 1111 1111 1111 1111 1111 1111 1111
constexpr uint32_t USMSK     = 0x80000000u; // 1000 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t ATMSK     = 0x7fffffffu; // 0111 1111 1111 1111 1111 1111 1111 1111
constexpr uint32_t NFRM_NTYP = 0x9fffc00fu; // 1001 1111 1111 1111 1100 0000 0000 1111
constexpr uint32_t TYPATMSK  = 0x20000000u; // 0010 0000 0000 0000 0000 0000 0000 0000
constexpr uint32_t WLKMSK    = 0x00200000u; // 0000 0000 0010 0000 0000 0000 0000 0000
constexpr uint32_t WLKFMSK   = 0x00203ff0u; // 0000 0000 0010 0000 0011 1111 1111 0000
constexpr uint32_t CWLKMSK   = 0x00100000u; // 0000 0000 0001 0000 0000 0000 0000 0000
constexpr uint32_t CWLKFMSK  = 0x00103ff0u; // 0000 0000 0001 0000 0011 1111 1111 0000
constexpr uint32_t UNDMSK    = 0x00400000u; // 0000 0000 0100 0000 0000 0000 0000 0000
constexpr uint32_t WUNDMSK   = 0x00600000u; // 0000 0000 0110 0000 0000 0000 0000 0000
constexpr uint32_t UNDFMSK   = 0x00403ff0u; // 0000 0000 0100 0000 0011 1111 1111 0000
constexpr uint32_t KNOTMSK   = 0x00800000u; // 0000 0000 1000 0000 0000 0000 0000 0000
constexpr uint32_t NKNOTMSK  = 0xff7fffffu; // 1111 1111 0111 1111 1111 1111 1111 1111
constexpr uint32_t FTHMSK    = 0x01000000u; // 0000 0001 0000 0000 0000 0000 0000 0000
constexpr uint32_t DELMSK    = 0x61e03ff0u; // 0110 0001 1110 0000 0011 1111 1111 0000
constexpr uint32_t ALTYPMSK  = 0x61f00000u; // 0110 0001 1111 0000 0000 0000 0000 0000
constexpr uint32_t SRTMSK    = 0x61f03fffu; // 0110 0001 1111 0000 0011 1111 1111 0000
constexpr uint32_t SRTYPMSK  = 0x61700000u; // 0110 0001 0111 0000 0000 0000 0000 0000
constexpr uint32_t NOTFRM    = 0x00080000u; // 0000 0000 0000 1000 0000 0000 0000 0000
constexpr uint32_t FRMSHFT   = 4;
constexpr uint32_t LAYSHFT   = 25;
constexpr uint32_t TYPSHFT   = 29;
constexpr uint32_t USHFT     = 31;

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

constexpr uint32_t FRMFIL   = 0x20000000u;
constexpr uint32_t FRMBFIL  = 0x40000000u;
constexpr uint32_t FRMAPFIL = 0x60000000u;

struct _floatRectangle {
	float left;
	float top;
	float right;
	float bottom;
};

using fRECTANGLE = struct _floatRectangle;

struct _forminfo {
	uint32_t type;
	uint32_t attribute;
	uint32_t sideCount;
};

using FORMINFO = struct _forminfo;

class SATCONOUT;

class SATCON
{
public:
	uint32_t start;
	uint32_t finish;

	SATCON() noexcept;
	// SATCON(SATCON&&) = default;
	// SATCON& operator=(const SATCON& rhs) = default;
	// SATCON& operator=(SATCON&&) = default;
	//~SATCON() = default;

	explicit SATCON(const SATCONOUT& rhs) noexcept;
	inline SATCON(uint32_t rStart, uint32_t rFinish) noexcept;
	inline SATCON& operator=(const SATCONOUT& rhs) noexcept;
};

inline SATCON::SATCON() noexcept {
	start  = 0u;
	finish = 0u;
}

inline SATCON::SATCON(uint32_t rStart, uint32_t rFinish) noexcept
    : start(rStart)
    , finish(rFinish) {
}

class SATCONOUT
{
public:
	uint16_t start;
	uint16_t finish;

	SATCONOUT() noexcept;
	// SATCONOUT(SATCONOUT&&) = default;
	// SATCONOUT& operator=(const SATCONOUT& rhs) = default;
	// SATCONOUT& operator=(SATCONOUT&&) = default;
	//~SATCONOUT() = default;

	explicit SATCONOUT(const SATCON& rhs) noexcept;
	inline SATCONOUT& operator=(const SATCON& rhs) noexcept;
};

inline SATCONOUT::SATCONOUT() noexcept {
	start  = 0u;
	finish = 0u;
}

inline SATCONOUT::SATCONOUT(const SATCON& rhs) noexcept {
	start  = gsl::narrow<uint16_t>(rhs.start);
	finish = gsl::narrow<uint16_t>(rhs.finish);
}

inline SATCONOUT& SATCONOUT::operator=(const SATCON& rhs) noexcept {
	start  = gsl::narrow<uint16_t>(rhs.start);
	finish = gsl::narrow<uint16_t>(rhs.finish);

	return *this;
}

inline SATCON::SATCON(const SATCONOUT& rhs) noexcept {
	start  = rhs.start;
	finish = rhs.finish;
}

inline SATCON& SATCON::operator=(const SATCONOUT& rhs) noexcept {
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
	// FANGCLP& operator=(const FANGCLP& rhs) = default;
	// FANGCLP& operator=(FANGCLP&&) = default;
	//~FANGCLP() = default;
};

inline FANGCLP::FANGCLP() noexcept { // NOLINT
	guide.start  = 0u;
	guide.finish = 0u;
}

union FANGCLPOUT {
public:
	float     angle;
	DWORD     clip {}; // clip pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
	SATCONOUT guide;

	FANGCLPOUT() noexcept;
	// FANGCLPOUT(FANGCLPOUT&&) = default;
	// FANGCLPOUT& operator=(const FANGCLPOUT& rhs) = default;
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

	inline FLENCNT& operator=(const FLENCNTOUT& rhs) noexcept;
};

union FLENCNTOUT {
public:
	float    stitchLength {};
	uint32_t clipCount;

	FLENCNTOUT() noexcept;
	// FLENCNTOUT(FLENCNTOUT&&) = default;
	// FLENCNTOUT& operator=(const FLENCNTOUT& rhs) = default;
	// FLENCNTOUT& operator=(FLENCNTOUT&&) = default;
	//~FLENCNTOUT() = default;

	explicit FLENCNTOUT(const FLENCNT& rhs) noexcept;
	inline FLENCNTOUT& operator=(const FLENCNT& rhs) noexcept;
};

inline FLENCNTOUT::FLENCNTOUT() noexcept {
	clipCount = 0u;
}

inline FLENCNTOUT::FLENCNTOUT(const FLENCNT& rhs) noexcept {
	clipCount = rhs.clipCount;
}

inline FLENCNTOUT& FLENCNTOUT::operator=(const FLENCNT& rhs) noexcept {
	clipCount = rhs.clipCount;

	return *this;
}

inline FLENCNT& FLENCNT::operator=(const FLENCNTOUT& rhs) noexcept {
	clipCount = rhs.clipCount;

	return *this;
}

union SATINANGLEOUT;

union SATINANGLE {
public:
	uint32_t guide;
	float    angle;

	inline SATINANGLE& operator=(const SATINANGLEOUT& rhs) noexcept;
};

union SATINANGLEOUT {
public:
	DWORD guide {};
	float angle;

	SATINANGLEOUT() noexcept;
	// SATINANGLEOUT(SATINANGLEOUT&&) = default;
	// SATINANGLEOUT& operator=(const SATINANGLEOUT& rhs) = default;
	// SATINANGLEOUT& operator=(SATINANGLEOUT&&) = default;
	//~SATINANGLEOUT() = default;

	explicit SATINANGLEOUT(const SATINANGLE& rhs) noexcept;
	inline SATINANGLEOUT& operator=(const SATINANGLE& rhs) noexcept;
};

inline SATINANGLEOUT::SATINANGLEOUT() noexcept {
	angle = 0.0;
}

inline SATINANGLEOUT::SATINANGLEOUT(const SATINANGLE& rhs) noexcept {
	angle = rhs.angle;
}

inline SATINANGLEOUT& SATINANGLEOUT::operator=(const SATINANGLE& rhs) noexcept {
	angle = rhs.angle;

	return *this;
}

inline SATINANGLE& SATINANGLE::operator=(const SATINANGLEOUT& rhs) noexcept {
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

constexpr uint32_t BELEN    = 0x001u;
constexpr uint32_t BESPAC   = 0x002u;
constexpr uint32_t BESIZ    = 0x004u;
constexpr uint32_t BNCLP    = 0x008u;
constexpr uint32_t BPICSPAC = 0x010u;
constexpr uint32_t BCNRSIZ  = 0x020u;
constexpr uint32_t BRDEND   = 0x040u;
constexpr uint32_t BRDPOS   = 0x080u;
constexpr uint32_t BEMAX    = 0x100u;
constexpr uint32_t BEMIN    = 0x200u;
constexpr uint32_t CHNPOS   = 0x400u;

constexpr uint32_t MEGLIN  = (BELEN | BEMAX | BEMIN);
constexpr uint32_t MEGBLD  = (BELEN | BEMAX | BEMIN);
constexpr uint32_t MEGCLP  = (BNCLP | BEMAX | BEMIN);
constexpr uint32_t MEGSAT  = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr uint32_t MEGAP   = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr uint32_t MEGPRP  = (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN);
constexpr uint32_t MEGHOL  = (BELEN | BESPAC | BESIZ | BCNRSIZ | BEMAX | BEMIN);
constexpr uint32_t MEGPIC  = (BELEN | BESIZ | BNCLP | BPICSPAC | BCNRSIZ | BEMAX | BEMIN);
constexpr uint32_t MEGDUB  = (BELEN | BEMAX | BEMIN);
constexpr uint32_t MEGCHNL = (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS);
constexpr uint32_t MEGCHNH = (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS);
constexpr uint32_t MEGCLPX = (BNCLP | BEMAX | BEMIN);

constexpr uint32_t EGLIN_LINS = 4;
constexpr uint32_t EGBLD_LINS = 4;
constexpr uint32_t EGCLP_LINS = 3;
constexpr uint32_t EGSAT_LINS = 6;
constexpr uint32_t EGAP_LINS  = 7;
constexpr uint32_t EGPRP_LINS = 6;
constexpr uint32_t EGHOL_LINS = 7;
constexpr uint32_t EGPIC_LINS = 7;
constexpr uint32_t EGCHN_LINS = 6;

struct _featherInfo {
	uint8_t  fillType;      // feather fill type
	uint8_t  upCount;       // feather up count
	uint8_t  downCount;     // feather down count
	uint8_t  color;         // feather blend color
	float    ratio;         // feather ratio
	float    minStitchSize; // feather floor
	uint16_t count;         // feather fill psg granularity
};

using FTHINFO = struct _featherInfo;

struct _textureInfo {
	int16_t  lines;
	uint16_t index;
	uint16_t count;
	float    height;
};

using TXTRINFO = struct _textureInfo;

union _tfhed {
	FTHINFO  feather;
	TXTRINFO texture;
};

using TFINFO = union _tfhed;

// Original form header used prior to version 2
class FRMHEDO
{
public:
	uint8_t       attribute;       // attribute
	uint16_t      vertexCount;     // number of sides
	uint8_t       type;            // type
	uint8_t       fillColor;       // fill color
	uint8_t       borderColor;     // border color
	uint16_t      clipEntries;     // number of border clipboard entries
	DWORD         vertices;        // points
	SATINANGLEOUT satinOrAngle;    // satin guidelines or angle clipboard fill angle
	DWORD         borderClipData;  // border clipboard data
	uint16_t      satinGuideCount; // number of satin guidelines
	uint16_t      wordParam;       // word parameter
	fRECTANGLE    rectangle {};    // rectangle
	uint8_t       fillType;        // fill type
	uint8_t       edgeType;        // edge type
	float         fillSpacing;     // fill spacing
	FLENCNTOUT    lengthOrCount;   // fill stitch length or clipboard count
	FANGCLPOUT    angleOrClipData; // fill angle or clipboard data pointer
	float         borderSize;      // border size
	float         edgeSpacing;     // edge spacing
	float         edgeStitchLen;   // edge stitch length
	uint16_t      res;             // pico length

	FRMHEDO() noexcept;
};

inline FRMHEDO::FRMHEDO() noexcept
    : satinOrAngle()
    , lengthOrCount()
    , angleOrClipData() {
	attribute       = 0;
	vertexCount     = 0;
	type            = 0;
	fillColor       = 0;
	borderColor     = 0;
	clipEntries     = 0;
	vertices        = 0;
	borderClipData  = 0;
	satinGuideCount = 0;
	wordParam       = 0;
	rectangle       = { 0.0f, 0.0f, 0.0f, 0.0f };
	fillType        = 0;
	edgeType        = 0;
	fillSpacing     = 0;
	borderSize      = 0;
	edgeSpacing     = 0;
	edgeStitchLen   = 0;
	res             = 0;
}

class FRMHEDOUT;

class FRMHED
{
public:
	uint8_t    attribute;          // attribute
	uint32_t   vertexCount;        // number of sides
	uint8_t    type;               // type
	uint8_t    fillColor;          // fill color
	uint8_t    borderColor;        // border color
	uint32_t   clipEntries;        // number of border clipboard entries
	uint32_t   vertexIndex;        // index into FormVertices
	SATINANGLE satinOrAngle {};    // satin guidelines or angle clipboard fill angle
	uint32_t   borderClipData;     // pointer to start of border clipboard data
	uint32_t   satinGuideCount;    // number of satin guidelines
	uint32_t   wordParam;          // clipboard/textured fill phase or satin end guide
	fRECTANGLE rectangle {};       // rectangle
	uint8_t    fillType;           // fill type
	uint8_t    edgeType;           // edge type
	float      fillSpacing;        // fill spacing
	FLENCNT    lengthOrCount {};   // fill stitch length or clipboard count
	FANGCLP    angleOrClipData {}; // fill angle or clipboard data pointer
	float      borderSize;         // border size
	float      edgeSpacing;        // edge spacing
	float      edgeStitchLen;      // edge stitch length
	uint16_t   picoLength;         // pico length

	uint32_t extendedAttribute;   // attribute extension
	float    maxFillStitchLen;    // maximum fill stitch length
	float    minFillStitchLen;    // minimum fill stitch length
	float    maxBorderStitchLen;  // maximum border stitch length
	float    minBorderStitchLen;  // minimum border stitch length
	TFINFO   fillInfo {};         // feather/texture info
	uint32_t fillStart;           // fill start point
	uint32_t fillEnd;             // fill end point
	float    underlaySpacing;     // underlay spacing
	float    underlayStitchLen;   // underlay stitch length
	float    underlayStitchAngle; // underlay stitch angle
	float    underlayIndent;      // underlay/edge walk indent
	float    txof;                // gradient end density
	uint8_t  underlayColor;       // underlay color
	uint8_t  cres;                // reserved

	FRMHED() noexcept;
	// FRMHED(FRMHED&&) = default;
	// FRMHED& operator=(const FRMHED& rhs) = default;
	// FRMHED& operator=(FRMHED&&) = default;
	//~FRMHED() = default;

	explicit inline FRMHED(const FRMHEDO& rhs) noexcept;
	explicit inline FRMHED(const FRMHEDOUT& rhs) noexcept;

	inline FRMHED& operator=(const FRMHEDO& rhs) noexcept;
	inline FRMHED& operator=(const FRMHEDOUT& rhs) noexcept;
};

inline FRMHED::FRMHED() noexcept
    : satinOrAngle()
    , lengthOrCount()
    , angleOrClipData() {
	attribute       = 0;
	vertexCount     = 0;
	type            = 0;
	fillColor       = 0;
	borderColor     = 0;
	clipEntries     = 0;
	vertexIndex     = 0;
	borderClipData  = 0;
	satinGuideCount = 0;
	wordParam       = 0;
	rectangle       = { 0.0f, 0.0f, 0.0f, 0.0f };
	fillType        = 0;
	edgeType        = 0;
	fillSpacing     = 0;
	borderSize      = 0;
	edgeSpacing     = 0;
	edgeStitchLen   = 0;
	picoLength      = 0;

	maxFillStitchLen    = 0;
	minFillStitchLen    = 0;
	extendedAttribute   = 0;
	maxBorderStitchLen  = 0;
	minBorderStitchLen  = 0;
	fillInfo            = {};
	fillStart           = 0;
	fillEnd             = 0;
	underlaySpacing     = 0;
	underlayStitchLen   = 0;
	underlayStitchAngle = 0;
	underlayIndent      = 0;
	txof                = 0;
	underlayColor       = 0;
	cres                = 0;
}

inline FRMHED::FRMHED(const FRMHEDO& rhs) noexcept {
	attribute       = rhs.attribute;
	vertexCount     = rhs.vertexCount;
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = rhs.clipEntries;
	vertexIndex     = 0;
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
	satinGuideCount = rhs.satinGuideCount;
	wordParam       = rhs.wordParam;
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0u))
	    || ((((1 << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0u))) {
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

	extendedAttribute   = 0;
	maxFillStitchLen    = 0;
	minFillStitchLen    = 0;
	maxBorderStitchLen  = 0;
	minBorderStitchLen  = 0;
	fillInfo            = {};
	fillStart           = 0;
	fillEnd             = 0;
	underlaySpacing     = 0;
	underlayStitchLen   = 0;
	underlayStitchAngle = 0;
	underlayIndent      = 0;
	txof                = 0;
	underlayColor       = 0;
	cres                = 0;
}

inline FRMHED& FRMHED::operator=(const FRMHEDO& rhs) noexcept {
	attribute       = rhs.attribute;
	vertexCount     = rhs.vertexCount;
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = rhs.clipEntries;
	vertexIndex     = 0;
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
	satinGuideCount = rhs.satinGuideCount;
	wordParam       = rhs.wordParam;
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0u))
	    || ((((1 << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0u))) {
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
	uint8_t       attribute;    // attribute
	uint16_t      vertexCount;  // number of sides
	uint8_t       type;         // type
	uint8_t       fillColor;    // fill color
	uint8_t       borderColor;  // border color
	uint16_t      clipEntries;  // number of border clipboard entries
	DWORD         vertexIndex;  // vertex index not saved in v1 or v2. size is to keep compatibility with v1 & v2 ThrEd files
	SATINANGLEOUT satinOrAngle; // satin guidelines or angle clipboard fill angle
	DWORD      borderClipData;  // border clipboard data pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
	uint16_t   satinGuideCount; // number of satin guidelines
	uint16_t   wordParam;       // clipboard/textured fill phase or satin end guide
	fRECTANGLE rectangle {};    // rectangle
	uint8_t    fillType;        // fill type
	uint8_t    edgeType;        // edge type
	float      fillSpacing;     // fill spacing
	FLENCNTOUT lengthOrCount;   // fill stitch length or clipboard count
	FANGCLPOUT angleOrClipData; // fill angle or clipboard data pointer
	float      borderSize;      // border size
	float      edgeSpacing;     // edge spacing
	float      edgeStitchLen;   // edge stitch length
	uint16_t   picoLength;      // pico length

	uint32_t extendedAttribute;   // attribute extension
	float    maxFillStitchLen;    // maximum fill stitch length
	float    minFillStitchLen;    // minimum fill stitch length
	float    maxBorderStitchLen;  // maximum border stitch length
	float    minBorderStitchLen;  // minimum border stitch length
	TFINFO   fillInfo {};         // feather/texture info
	uint16_t fillStart;           // fill start point
	uint16_t fillEnd;             // fill end point
	float    underlaySpacing;     // underlay spacing
	float    underlayStitchLen;   // underlay stitch length
	float    underlayStitchAngle; // underlay stitch angle
	float    underlayIndent;      // underlay/edge walk indent
	float    txof;                // gradient end density
	uint8_t  underlayColor;       // underlay color
	uint8_t  cres;                // reserved

	FRMHEDOUT() noexcept;
	// FRMHEDOUT(FRMHEDOUT&&) = default;
	// FRMHEDOUT& operator=(const FRMHEDOUT& rhs) = default;
	// FRMHEDOUT& operator=(FRMHEDOUT&&) = default;
	//~FRMHEDOUT() = default;

	explicit inline FRMHEDOUT(const FRMHED& rhs) noexcept;
	inline FRMHEDOUT& operator=(const FRMHED& rhs) noexcept;
};

inline FRMHEDOUT::FRMHEDOUT() noexcept
    : satinOrAngle()
    , lengthOrCount()
    , angleOrClipData() {
	attribute       = 0;
	vertexCount     = 0;
	type            = 0;
	fillColor       = 0;
	borderColor     = 0;
	clipEntries     = 0;
	vertexIndex     = 0;
	borderClipData  = 0;
	satinGuideCount = 0;
	wordParam       = 0;
	rectangle       = { 0.0f, 0.0f, 0.0f, 0.0f };
	fillType        = 0;
	edgeType        = 0;
	fillSpacing     = 0;
	borderSize      = 0;
	edgeSpacing     = 0;
	edgeStitchLen   = 0;
	picoLength      = 0;

	maxFillStitchLen    = 0;
	minFillStitchLen    = 0;
	extendedAttribute   = 0;
	maxBorderStitchLen  = 0;
	minBorderStitchLen  = 0;
	fillInfo            = {};
	fillStart           = 0;
	fillEnd             = 0;
	underlaySpacing     = 0;
	underlayStitchLen   = 0;
	underlayStitchAngle = 0;
	underlayIndent      = 0;
	txof                = 0;
	underlayColor       = 0;
	cres                = 0;
}

inline FRMHEDOUT::FRMHEDOUT(const FRMHED& rhs) noexcept {
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
	if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0u))
	    || ((((1 << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0u))) {
		angleOrClipData.clip = rhs.angleOrClipData.clip;
	}
	else {
		if (type == FRMLINE) {
			if (fillType == CONTF) {
				angleOrClipData.guide.start = gsl::narrow<decltype(angleOrClipData.guide.start)>(rhs.angleOrClipData.guide.start);
				angleOrClipData.guide.finish
				    = gsl::narrow<decltype(angleOrClipData.guide.finish)>(rhs.angleOrClipData.guide.finish);
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

inline FRMHEDOUT& FRMHEDOUT::operator=(const FRMHED& rhs) noexcept {
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
	if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0u))
	    || ((((1 << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0u))) {
		angleOrClipData.clip = rhs.angleOrClipData.clip;
	}
	else {
		if (type == FRMLINE) {
			if (fillType == CONTF) {
				angleOrClipData.guide.start = gsl::narrow<decltype(angleOrClipData.guide.start)>(rhs.angleOrClipData.guide.start);
				angleOrClipData.guide.finish
				    = gsl::narrow<decltype(angleOrClipData.guide.finish)>(rhs.angleOrClipData.guide.finish);
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

inline FRMHED::FRMHED(const FRMHEDOUT& rhs) noexcept {
	attribute       = rhs.attribute;
	vertexCount     = rhs.vertexCount;
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = rhs.clipEntries;
	vertexIndex     = 0u; // do not read the index from a v2 file
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = 0u; // do not read the index from a v2 file
	satinGuideCount = rhs.satinGuideCount;
	wordParam       = rhs.wordParam;
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0u))
	    || ((((1 << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0u))) {
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

inline FRMHED& FRMHED::operator=(const FRMHEDOUT& rhs) noexcept {
	attribute       = rhs.attribute;
	vertexCount     = rhs.vertexCount;
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = rhs.clipEntries;
	vertexIndex     = 0u; // do not read the index from a v2 file
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = 0u; // do not read the index from a v2 file
	satinGuideCount = rhs.satinGuideCount;
	wordParam       = rhs.wordParam;
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	if (((edgeType == EDGECLIP || edgeType == EDGEPICOT || edgeType == EDGECLIPX) && (clipEntries != 0u))
	    || ((((1 << fillType) & (MCLPF | MVCLPF | MHCLPF | MANGCLPF)) != 0) && (lengthOrCount.clipCount != 0u))) {
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

constexpr uint32_t FRMEND   = 0x01u;
constexpr uint32_t FRMLMSK  = 0x0eu;
constexpr uint32_t NFRMLMSK = 0xf1u;

/*form attribute bits

1		end teflon line
2-4	layer bits
5		finish blunt
6		start blunt
7		refill bit for contour fill
*/

// blunt bits
constexpr uint32_t FBLNT  = 0x20u;
constexpr uint32_t SBLNT  = 0x40u;
constexpr uint32_t NFBLNT = 0xdfu;
constexpr uint32_t NSBLNT = 0xbfu;
constexpr uint32_t NOBLNT = 0x9fu;

// frmhed extended attribute bits

constexpr uint32_t AT_SQR     = 0x001u; // square ends
constexpr uint32_t AT_FTHUP   = 0x002u; // feather up or down flag
constexpr uint32_t AT_FTHBTH  = 0x004u; // feather both up and down flag
constexpr uint32_t AT_FTHBLND = 0x008u; // feather blend flag
constexpr uint32_t AT_STRT    = 0x010u; // user set start flag
constexpr uint32_t AT_END     = 0x020u; // user set end flag
constexpr uint32_t AT_UND     = 0x040u; // underlay flag
constexpr uint32_t AT_WALK    = 0x080u; // edge walk
constexpr uint32_t AT_CWLK    = 0x100u; // center walk

// contour refil
constexpr uint32_t FRECONT  = 0x80u;
constexpr uint32_t NFRECONT = 0x7fu;

struct _frmclp {
	uint32_t clipType {};
	uint32_t reserved {};
	FRMHED   form;
};

using FORMCLIP = struct _frmclp; // form data clipboard header

struct _frmsclp {
	uint32_t clipType;
	uint16_t formCount;
	uint16_t reserved;
};

using FORMSCLIP = struct _frmsclp; // multiple forms clipboard header

struct _fvclip {
	uint32_t clipType;
	uint32_t vertexCount;
	bool     direction;
};

using FORMVERTEXCLIP = struct _fvclip; // form points clipboard header

struct _strhed { // thred file header
	uint32_t headerType;
	uint32_t fileLength;    // length of strhed + length of stitch data
	uint16_t stitchCount;   // number of stitches
	uint16_t hoopType;      // size of hoop
	uint16_t formCount;     // number of forms
	uint16_t vertexLen;     // points to form points
	uint16_t vertexCount;   // number of form points
	uint16_t dlineLen;      // points to dline data
	uint16_t dlineCount;    // dline data count
	uint16_t clipDataLen;   // points to clipboard data
	uint16_t clipDataCount; // clipboard data count
};

using STRHED = _strhed;

struct _txpnt { // textured fill point
	float    y;
	uint16_t line;
};

using TXPNT = struct _txpnt;

struct _txoff { // textured fill offset
	float   y;
	int32_t line;
};

using TXOFF = struct _txoff;

struct _strex {                      // thred v1.0 file header extension
	float    hoopSizeX;              // hoop size x dimension
	float    hoopSizeY;              // hoop size y dimension
	float    stgran;                 // stitches per millimeter
	char     creatorName[NAME_LEN];  // name of the file creator NOLINT(modernize-avoid-c-arrays)
	char     modifierName[NAME_LEN]; // name of last file modifier NOLINT(modernize-avoid-c-arrays)
	int8_t   auxFormat;              // auxillary file format
	int8_t   stres;                  // reserved
	uint32_t texturePointCount;      // textured fill point32_t count
	int8_t   res[RES_SIZE];          // reserved for expansion NOLINT(modernize-avoid-c-arrays)
};

using STREX = struct _strex;

struct _dsthed {      // dst file header
	char desched[3];  // 0 0		description NOLINT
	char desc[17];    // 3 3                    NOLINT
	char recshed[3];  // 20 14	record count    NOLINT
	char recs[8];     // 23 17                  NOLINT
	char cohed[3];    // 31 1F                  NOLINT
	char co[4];       // 34 22                  NOLINT
	char xplushed[3]; // 38 26	x+ size         NOLINT
	char xplus[6];    // 41 29                  NOLINT
	char xminhed[3];  // 47 2F	x- size         NOLINT
	char xmin[6];     // 50 32                  NOLINT
	char yplushed[3]; // 56 38                  NOLINT
	char yplus[6];    // 59 3B	y+ size         NOLINT
	char yminhed[3];  // 65 41                  NOLINT
	char ymin[6];     // 68 44	y- size         NOLINT
	char axhed[3];    // 74 4A                  NOLINT
	char ax[7];       // 77 4D                  NOLINT
	char ayhed[3];    // 84 54                  NOLINT
	char ay[7];       // 87 57                  NOLINT
	char mxhed[3];    // 94 5E                  NOLINT
	char mx[7];       // 97 61                  NOLINT
	char myhed[3];    // 104 68                 NOLINT
	char my[7];       // 107 6B                 NOLINT
	char pdhed[2];    // 114 72                 NOLINT
	char pd[7];       // 116 74                 NOLINT
	char eof[1];      // 123 7B                 NOLINT
	char res[388];    // 124 7C                 NOLINT
};

using DSTHED = struct _dsthed;

// dst type masks

constexpr uint32_t JMPTYP = 0x830000u;
constexpr uint32_t COLTYP = 0x630000u;
constexpr uint32_t REGTYP = 0x030000u;

struct _dstrec { // dst stitch record
	uint8_t led;
	uint8_t mid;
	uint8_t nd;
};

using DSTREC = struct _dstrec;

struct _dstoffsets {
	POINT Positive; // plus offset written into the destination file header
	POINT Negative; // minus offset written into the destination file header
};

using DSTOffsets = struct _dstoffsets;

struct _pcstch {
	uint8_t fx; // fractional part of x coord or color if tag = 3
	int16_t x;  // integer part of x coord
	uint8_t nx;
	uint8_t fy; // fractional part of y coord
	int16_t y;  // integer part of y coord
	uint8_t ny;
	uint8_t tag;
};

using PCSTCH = struct _pcstch;

struct _clpstch {
	uint32_t led;  // ????
	uint8_t  fx;   // fractional part of x coord
	uint16_t x;    // integer part of x coord
	uint8_t  spcx; // ToDo - Is this structure member needed?
	uint8_t  fy;   // fractional part of y coord
	uint16_t y;    // integer part of y coord
	uint8_t  spcy; // ToDo - Is this structure member needed?
	uint8_t  myst; // ToDo - Is this structure member needed?
	uint8_t  tag;  // ToDo - Is this structure member needed?
};

using CLPSTCH = struct _clpstch;

struct _bakhed {
	uint32_t    formCount;
	FRMHED*     forms;
	uint32_t    stitchCount;
	fPOINTATTR* stitches;
	uint32_t    vertexCount;
	fPOINT*     vertices;
	uint32_t    guideCount;
	SATCON*     guide;
	uint32_t    clipPointCount;
	fPOINT*     clipPoints;
	COLORREF*   colors;
	TXPNT*      texturePoints;
	uint32_t    texturePointCount;
	POINT       zoomRect;
};

using BAKHED = struct _bakhed;

struct _flsiz {
	float cx;
	float cy;
};

using FLSIZ = struct _flsiz;

struct _frmrange {
	uint32_t start;
	uint32_t finish;
};

using FRMRANGE = struct _frmrange;

struct _range {
	uint32_t start;
	uint32_t finish;
};

using RANGE = struct _range;

class REGION // region for sequencing vertical fills
{
public:
	uint32_t start { 0u };       // start line of region
	uint32_t end { 0u };         // end line of region
	uint32_t regionBreak { 0u }; // ToDo - Is this member needed?
	uint32_t breakCount { 0u };

	constexpr REGION() noexcept = default;
	inline REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept;
	// REGION(REGION&&) = default;
	// REGION& operator=(const REGION& rhs) = default;
	// REGION& operator=(REGION&&) = default;
	//~REGION() = default;
};

inline REGION::REGION(uint32_t rhsStart, uint32_t rhsEnd, uint32_t rhsBreak, uint32_t rhsCount) noexcept
    : start(rhsStart)
    , end(rhsEnd)
    , regionBreak(rhsBreak)
    , breakCount(rhsCount) {
}

struct _rcon { // PathMap: path map for sequencing
	uint32_t node;
	bool     isConnected;
	uint32_t nextGroup;
};

using RCON = struct _rcon;

struct _rgseq {    // TempPath: temporary path connections
	uint32_t pcon; // pointer to PathMap entry
	int32_t  count;
	bool     skp; // path not found
};

using RGSEQ = struct _rgseq;

struct _fseq { // SequencePath: path of sequenced regions
	uint16_t node;
	uint16_t nextGroup;
	bool     skp; // path not found
};

using FSEQ = struct _fseq;

constexpr uint32_t SEQTOP = 2;
constexpr uint32_t SEQBOT = 3;

struct _pvec {
	double angle;
	double length;
};

using PVEC = struct _pvec;

struct _vrct2 {
	fPOINT aipnt;
	fPOINT aopnt;
	fPOINT bipnt;
	fPOINT bopnt;
	fPOINT cipnt;
	fPOINT copnt;
	fPOINT dipnt;
	fPOINT dopnt;
};

using VRCT2 = struct _vrct2;

struct _cursorMask {
	uint8_t form[128];            // NOLINT
	uint8_t dline[128];           // NOLINT
	uint8_t uprightNeedle[128];   // NOLINT
	uint8_t leftUpNeedle[128];    // NOLINT
	uint8_t leftDownNeedle[128];  // NOLINT
	uint8_t rightUpNeedle[128];   // NOLINT
	uint8_t rightDownNeedle[128]; // NOLINT
};

using CURSORMASK = struct _cursorMask;

// balarad file header
struct _balhed {
	COLORREF color[256]; // NOLINT
	uint32_t signature;
	uint16_t version;
	float    hoopSizeX;
	float    hoopSizeY;
	COLORREF backgroundColor;
	uint8_t  res[1006]; // NOLINT
};

using BALHED = struct _balhed;

// balarad stitch
struct _balstch {
	uint8_t code;
	uint8_t flag;
	float   x;
	float   y;
};

using BALSTCH = struct _balstch;

struct _clpseg {
	uint32_t start;
	float    beginLength;
	uint32_t beginIndex;
	uint32_t asid;
	uint32_t finish;
	float    endLength;
	uint32_t endIndex;
	uint32_t zsid;
	int8_t   dun;
};

using CLPSEG = struct _clpseg;

class CLIPSORT
{
public:
	float    segmentLength { 0.0f };
	float    sideLength { 0.0f };
	uint32_t vertexIndex { 0 };
	fPOINT   point;

	constexpr CLIPSORT() noexcept = default;
};

struct _clipnt {
	float    x;
	float    y;
	uint32_t vertexIndex;
	uint32_t flag;
};

using CLIPNT = struct _clipnt;

struct _vclpx {
	uint32_t segment;
	uint32_t vertex;
};

using VCLPX = struct _vclpx;

#if PESACT

struct _pesled {
	int8_t   ver[8]; // NOLINT
	uint32_t pec;
};

using PESLED = struct _pesled;

struct _peshed {
	char     led[8];  //   0-7  Identification and version (#PES0001)         NOLINT
	uint32_t off;     //   8-b  Absolute PEC section byte offset
	uint16_t hpsz;    //   c,d  Hoopsize (0), 0 = 100x100mm, 1 = 130x180mm
	uint16_t usdn;    //   e,f  Use existing design area (1)
	uint16_t blct;    // 10,11  CSewSeg segment block count (1)
	char     hnd1[4]; // 12-15  header end (FF FF 00 00)                      NOLINT
	uint16_t celn;    // 16,17  Length of following string (7)
	char     ce[7];   // 18-1e  CEmbOne identification (CEmbOne)              NOLINT
	int16_t  xlft;    // 1f,20  Extent left
	int16_t  xtop;    // 21,22  Extent top
	int16_t  xrht;    // 23,24  Extent right
	int16_t  xbot;    // 25,26  Extent bottom
	int16_t  plft;    // 27,28  Extent left position
	int16_t  ptop;    // 29,2a  Extent top position
	int16_t  prht;    // 2b,2c  Extent right position
	int16_t  pbot;    // 2d,2e  Extent bottom position
	float    atfm1;   // 2f-32  Affine transform Scale X (1.0f) (00 00 80 3f)
	float    atfm2;   // 33-36  Affine transform Skew X (0.0f) (00 00 00 00)
	float    atfm3;   // 37-3a  Affine transform Skew Y (0.0f) (00 00 00 00)
	float    atfm4;   // 3b-3e  Affine transform Scale Y (1.0f) (00 00 80 3f)
	float    atfm5;   // 3f-42  Affine transform Left_Pos
	float    atfm6;   // 43-46  Affine transform Bottom_Pos
	uint16_t ukn1;    // 47,48  unknown (1)
	int16_t  xtrn;    // 49,4a  CSewSeg x coordinate translation (0)
	int16_t  ytrn;    // 4b,4c  CSewSeg y coordinate translation (0)
	int16_t  xsiz;    // 4d,4e  CSewSeg width
	int16_t  ysiz;    // 4f,50  CSewSeg height
	int8_t   ukn2[8]; // 51,58  unknown (0)                                   NOLINT
	uint16_t bcnt;    // 59,5a  CSewSeg block count (segments + (2*colorChanges))
	int8_t   hnd2[4]; // 5b-5e  header end (FF FF 00 00)                      NOLINT
	uint16_t cslen;   // 5f,60  CSewSeg length (7)
	char     cs[7];   // 61-67  CSewSeg identification (CSewSeg)              NOLINT
	                  // uint16_t styp1;   // 68,69  Stitch type (0)
	                  // uint16_t scol;    // 6a,6b  Stitch Palette thread index
};

using PESHED = struct _peshed;

struct _pestch {
	int16_t x;
	int16_t y;
};

using PESTCH = struct _pestch;

struct _pesstitchlist {
	uint16_t stitchtype;
	uint16_t threadIndex;
	uint16_t stitchcount;
};

using PESSTCHLST = struct _pesstitchlist;

struct _pecheader {
	int8_t   label[19];  // Label string prefixed with "LA:" and padded with space (0x20)                         NOLINT
	int8_t   labnd;      // carriage return character
	uint8_t  ukn1[11];   // Unknown (' ')                                                                         NOLINT
	uint8_t  ukn2;       // Unknown
	uint16_t hnd1;       // Unknown (0x00ff)
	uint8_t  thumbWidth; // Thumbnail image width in bytes (6) , with 8 bit pixels per byte
	                     // Thus, 6 would mean 6×8 = 48 pixels per line
	uint8_t thumbHeight; // Thumbnail image height in pixels (38)
	uint8_t ukn3[12];    // Unknown, usually 20 20 20 20 64 20 00 20 00 20 20 20                                  NOLINT
	uint8_t colorCount;  // Number of colors minus one, 0xFF means 0 colors
	uint8_t pad[463];    // Pad bytes up to 512. This will hold the pallette indices for thread colors and 0x20   NOLINT
};

using PECHDR = struct _pecheader;

static const uint8_t ThumbHeight = 38u;
static const uint8_t ThumbWidth  = 48u;

struct _pecheader2 {
	uint16_t unknown1;        // typical 0x0000
	uint16_t thumbnailOffset; // Offset to thumbnail image subsection relative to the PEC section offset plus 512 bytes
	uint16_t unknown2;        // typical 0x3100
	uint16_t unknown3;        // typical 0xf0ff
	int16_t  width;           // Width
	int16_t  height;          // height
	uint16_t unknown4;        // typical 0x01e0
	uint16_t unknown5;        // typical 0x01b0
	uint16_t xMin;            // starting X val (Bigendian)
	uint16_t yMin;            // starting Y val (Bigendian)
};

using PECHDR2 = struct _pecheader2;

#endif

struct _trcpnt {
	int16_t x;
	int16_t y;
};

using TRCPNT = struct _trcpnt;

// ToDo - Is this struct needed?
struct _txtmsg {
	uint32_t cod;
	int8_t*  str;
};

using TXTMSG = struct _txtmsg;

struct _grdcod {
	uint32_t id;
	uint32_t col;
};

using GRDCOD = struct _grdcod;

#pragma pack()

struct _orec {
	uint32_t    start;
	uint32_t    finish;
	fPOINTATTR* startStitch;
	fPOINTATTR* endStitch;
	uint32_t    color;
	uint32_t    type;
	uint32_t    form;
	uint32_t    otyp;
};

using OREC = struct _orec;

struct _srtrec {
	uint32_t start;         // start region index
	uint32_t finish;        // finish region index
	uint32_t count;         // number of regions in range
	uint32_t currentRegion; // current region index
	bool     direction;     // direction of sort
};

using SRTREC = struct _srtrec;

struct _fstrts {
	uint32_t applique;
	uint32_t fill;
	uint32_t feather;
	uint32_t border;
	uint32_t appliqueColor;
	uint32_t fillColor;
	uint32_t featherColor;
	uint32_t borderColor;
};

using FSTRTS = struct _fstrts;

union _fillStarts {
	FSTRTS   fillNamed;
	uint32_t fillArray[sizeof(FSTRTS) / sizeof(uint32_t)]; // NOLINT
};

using FILLSTARTS = union _fillStarts;

constexpr uint32_t M_AP     = 0x002u;
constexpr uint32_t M_CWLK   = 0x004u;
constexpr uint32_t M_WALK   = 0x008u;
constexpr uint32_t M_UND    = 0x010u;
constexpr uint32_t M_FIL    = 0x020u;
constexpr uint32_t M_FTH    = 0x040u;
constexpr uint32_t M_BRD    = 0x080u;
constexpr uint32_t M_APCOL  = 0x100u;
constexpr uint32_t M_FCOL   = 0x200u;
constexpr uint32_t M_FTHCOL = 0x400u;
constexpr uint32_t M_ECOL   = 0x800u;

struct _insrec {
	uint32_t code;
	uint32_t color;
	uint32_t index;
	uint32_t seq;
};

using INSREC = struct _insrec;

enum interleaveTypes
{ // interleave sequence identifiers
	I_AP,
	I_FIL,
	I_FTH,
	I_BRD,
};

struct _intinf {
	uint32_t    pins;
	uint32_t    coloc;
	uint32_t    layerIndex;
	uint32_t    start;
	uint32_t    output;
	fPOINTATTR* highStitchBuffer;
};

using INTINF = struct _intinf;

class TXTSCR
{
public:
	int32_t  top {};                    // pixel top line
	int32_t  bottom {};                 // pixel bottom line
	int32_t  height {};                 // pixel height of area
	int32_t  halfHeight {};             // pixel middle of area
	float    xOffset { 0.0f };          // edit x offset of area
	float    yOffset { 0.0f };          // edit y offset of area
	float    areaHeight { 0.0f };       // edit height of area
	float    screenHeight { 0.0f };     // edit height of screen
	float    width { 0.0f };            // edit width of area
	float    spacing { 0.0f };          // edit space between lines
	uint16_t lines { 0 };               // number of lines
	float    editToPixelRatio { 0.0f }; // edit to pixel ratio
	fPOINT   formCenter;                // middle of the form

	constexpr TXTSCR() noexcept = default;
};

struct _txtrct {
	uint16_t left;
	uint16_t right;
	float    top;
	float    bottom;
};

using TXTRCT = struct _txtrct;

struct _txhst {
	std::vector<TXPNT> texturePoints;
	float              height  = 0.0;
	float              width   = 0.0;
	float              spacing = 0.0;
};

using TXHST = struct _txhst;

struct _txhstbuff {
	uint32_t* placeholder;
	uint32_t  count;
	float     height;
	float     width;
	float     spacing;
};

using TXHSTBUF = struct _txhstbuff;

struct _rngcnt {
	int32_t line;
	int32_t stitchCount;
};

using RNGCNT = struct _rngcnt;

constexpr uint32_t BADFLT = 1;
constexpr uint32_t BADCLP = 2;
constexpr uint32_t BADSAT = 4;
constexpr uint32_t BADTX  = 8;

struct _badcnts {
	uint32_t attribute;
	uint32_t flt;
	uint32_t clip;
	uint32_t guideCount; // ToDo - is this an accurate description?
	uint32_t tx;
};

using BADCNTS = struct _badcnts;

struct _findInfo {
	uint32_t         count;
	WIN32_FIND_DATA* data;
};

using FINDINFO = struct _findInfo;

// is intersection of line and cursor in, before or after the line
enum intersectionStyles
{
	POINT_IN_LINE = 0,
	POINT_BEFORE_LINE,
	POINT_AFTER_LINE
};

// Sorted line length info
struct _lengthInfo {
	uint32_t index;
	bool     isEnd;
	float    length;
};

using LENINFO = struct _lengthInfo;

struct _pecColor {
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

using PECCOLOR = struct _pecColor;

struct _colorchange {
	uint16_t blockIndex;
	uint16_t colorIndex;
};

using PESCOLORLIST = struct _colorchange;

struct _thread {
	PECCOLOR    color;
	const char* description;
	const char* catalogNumber;
};

using THREAD = struct _thread;

// Do the type punning while ensuring that the returned pointer is non_null
// use the encapsulation recommended in I.30
// (https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i30-encapsulate-rule-violations)
template <class T2, class T1> inline _Ret_notnull_ T2 convert_ptr(T1* pointer) {
	GSL_SUPPRESS(26474) {
		if (pointer) {
			return gsl::narrow_cast<T2>(gsl::narrow_cast<void*>(pointer));
		}

		throw;
	}
}
