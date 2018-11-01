#pragma once

// Open Source headers
#include "warnings.h"
#pragma warning(push)
#pragma warning(disable : ALL_CPPCORECHECK_WARNINGS)
#include <gsl/gsl>
#pragma warning(pop)

// Local Headers
#include "switches.h"

#define TRCMTH 1 // 0=brightness compare,1=color compare

#define RUTVALID 1
#define COMVALID 2
#define REGVALID 4

#define RUTSED 1232323232
#define COMSED 1323232323
#define TIMSED 1313131313
#define TIMSIG 2211221122      // time signature for trial operation
#define DAY1 864000000000      // file time ticks in a day
#define DAY31 26784000000000   // file time ticks in 31 days
#define DAY400 345600000000000 // file time ticks in 400 days
#define TRILIM 100             // expired trial clipboard limit
#define SRTIM 20000000         // sort time limit in 100 ns intervals
// end of trial version codes

// daisy codes
#define DAZPETS 5  // petals
#define DAZCNT 10  // petal points
#define DAZICNT 2  // petal inner points
#define DAZLEN 15  // diameter
#define DAZPLEN 20 // petal length
#define DAZHLEN 5  // hole size
#define DAZTYP 5   // border type
#define DAZMCNT 7  // mirror count
// end of daisy codes

#define TXTRAT 0.95       // texture fill clipboard shrink/grow ratio
#define MAXMSK 0xffff0000 // for checking for greater than 65536

#define DEFBPIX 4   // default form box pixels
#define MAXWLK 54   // max underlay/edge walk stitch length
#define MINWLK 2.4f // max underlay/edge walk stitch length
#define DEFULEN 12; // default underlay stitch length
#define DEFUSPAC 6; // default underlay stitch spacing

#define IWAVPNTS 36       // default wave points
#define IWAVSTRT 10       // default wave start
#define IWAVEND 26        // default wave end
#define IWAVS 5           // default wave lobes
#define THRLED0 0x746872  // lead dword value for thred file v 1.0
#define THRLED1 0x1746872 // lead dword value for thred file v 1.1
#define ZUMFCT 0.65       // zoom factor
#define PAGSCROL 0.9      // page scroll factor
#define LINSCROL 0.05     // line scroll factor
#define TXTSIDS 6         // extra pixels in a text box
#define MAXITEMS 0x10000u // maximum number of stitches, vertices, texture points, sequence items & clipboard points
#define MAXCHNG 10000     // maximum number of color changes
#define SHUPX 480.0f      // small hoop x size
#define SHUPY 480.0f      // small hoop y size
#define LHUPX 719.0f      // large hoop x size
#define LHUPY 690.0f      // large hoop y size
#define HUP100XY 600.0f   // 100 millimeter hoop size
#define PFGRAN 6          // pfaf "pixels" per millimeter
#define TSIZ30 0.3        //#30 thread size in millimeters
#define TSIZ40 0.2        //#40 thread size in millimeters
#define TSIZ60 0.05       //#60 thread size in millimeters
#define SCROLSIZ 12       // logical pixel width of a scroll bar
#define COLSIZ 12         // logical pixel width of the color bar
// ToDo - Should this be a configurable parameter?
#define CLOSENUF 15              // mouse click region for select
#define ZMARGIN 1.25             // zoom margin for select zooms
#define SMALSIZ 0.25             // default small stitch size
#define MINSIZ 0.1               // default minimum stitch size
#define USESIZ 3.5               // user preferred size
#define MAXSIZ 9.0               // default maximum stitch size
#define PFAFGRAN 6               // pfaf stitch points per millimeter
#define MINZUM 5                 // minimum zoom in stitch points
#define MAXZLEV 9                // maximum levels of zoom
#define SHOPNTS 0.00             // show stitch points when zoom below this
#define STCHBOX 0.4226           // show stitch boxes when zoom below this
#define BITCOL 0xffff00          // default bitmap color
#define MAXFORMS 1024            // maximum number of forms
#define FORMFCT 0.05             // new forms part of screen
#define MAXDELAY 600             // maximum movie time step
#define MINDELAY 1               // minimum movie time step
#define MOVITIM 12               // default movie time
#define DEFSPACE 0.45            // default stitch spacing
#define JMPSPACE 13              // default jump stitch spacing
#define DEFANG 0.785398163397448 // default fill angle, 45 degrees
#define MAXFRMLINS 20000         // maximum lines in a form
#define MSGSIZ 8192              // size of the message buffer
#define PI 3.1415926535898
#define PI2 6.2831853071796
#define MAXSTCH 54                // maximum permitted stitch length for pfaf in pfaf "stitch pixels"
#define USPAC 15                  // underlay fill spacing
#define APSPAC 10.8               // applique border spacing
#define OSEQLEN 0x40000           // output sequence length
#define BSEQLEN OSEQLEN << 1      // reverse sequence length
#define MAXRAT 3                  // maximum number of stitches in one place in satin border
#define URAT 0.75                 // ratio of underlay stitch to satin border size
#define PURAT 0.6                 // for proportional satin corners
#define DIURAT 0.125              //(1-URAT)/2
#define DOURAT 0.8125             //(1-URAT)/2+URAT
#define MINRCT 12                 // minimum dimension of a form select rectangle
#define OLDNUM 4                  // number of old filenames saved on file menu
#define OLDVER 4                  // number of old file versions kept
#define TINY 1e-6                 // tiny number for floating point stuff
#define SPEDLIN 30                // speed change for line message on speed scroll bar
#define SPEDPAG 120               // speed change for page message on speed scroll bar
#define KNOTLEN 54                // set knots for stitches longer than this
#define MAXSAC 10000              // maximum number of satin guidelines
#define MAXKNOTS 16384            // maximum nuber of knots
#define IBFCLEN (4 * PFGRAN)      // initial buttonhole fill corner length
#define IPICSPAC 6                // initial picot border space
#define PRFLINS 28                // number of lines on the preference menu
#define EDGETYPS 12               // number of border fill types
#define SEED 3037000499           // pseudo-random-sequence seed
#define FSED 1340007303           // feather sequence seed
#define NAMSED 2222222222         // trial version psg seed
#define DSTRAT 0.8333333333333333 // ratio of dst stitch points to PFAFF stitch points
#define HUPS 5u                   // number of hoops the user can select
#define NORDSED 0x5a5a5a5a        // name order seed
#define NCODSED 0x73ef5a7e        // name encoding seed
#define NCODOF 80                 // name encoding offset
#define CLPMIN 0.5                // if clipboard data less wide, then don't fill
#define CLPMINAUT 1.2f            // for skinny vertical clips
#define BRDWID 18                 // default satin border size
#define SNPLEN 0.15               // default snap together length size
#define STARAT 0.4                // default star ratio
#define SPIRWRAP 1.52             // default spiral wrap
#define BALNORM 0x80              // normal balarad stitch
#define BALJUMP 0x81              // balarad jump stitch
#define BALSTOP 0                 // balarad stop
#define COLVER 0x776874           // color file version
#define PESCMSK 0x3f              // pes color mask
#define REDCOL 0xff               // code for the color red
#define GRNCOL 0xff00             // code for the color green
#define BLUCOL 0xff0000           // code for the color blue
#define REDMSK 0xffff00           // mask for the color red
#define GRNMSK 0xff00ff           // mask for the color green
#define BLUMSK 0x00ffff           // mask for the color blue
#define TRACLEN 1                 // initial trace length
#define TRACRAT 1.00001           // initial trace ratio
#define CHSDEF 24                 // default chain stitch length
#define CHRDEF 0.25               // default chain stitch ratio
#define NUGINI 2                  // default nudge step
#define DEFPIX 2                  // default nudge pixels
#define DEFEGRAT 1.5              // default egg ratio
#define DEFPNTPIX 4               // default form and stitch point pixels
#define HBUFSIZ 1024              // help buffer size
#define HIGRD 0xffffff            // grid high color
#define MEDGRD 0x404040           // grid medium color
#define DEFGRD 0x202020           // grid default color
#define REDGRD 0xff2020           // grid red color
#define BLUGRD 0x20ff20           // grid green color
#define GRNGRD 0x2020ff           // grid blue color
#define FDEFRAT 0.6f              // default feather ratio
#define FDEFUP 10                 // default feather up count
#define FDEFDWN 5                 // default feather down count
#define FDEFFLR 9                 // default feather floor
#define FDEFNUM 10                // default feather number
#define FDEFTYP FTHPSG            // default feather type
#define ITXHI (9 * PFGRAN)        // default texture editor height
#define ITXWID (9 * PFGRAN)       // default texture editor width
#define ITXSPAC (0.40 * PFGRAN)   // default texture editor spacing
#define ITXPIX 5                  // default texture editor cross pixels
#define ITXBUFLEN 16              // texture buffer depth

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
	VCAPT,     // keep screen capture as long as the mouse cursor
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
	MOVSET,    // the move point has been selected
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
	PIXIN,     // user in inputting nudge pixels
	STPXIN,    // user is inputting stitch point pixels
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
	TXIN,      // last stitch point was in stitch select box
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

#pragma pack(1)

// pcs file header structure
struct _pcsheader {
	char           leadIn;
	char           hoopType;
	unsigned short colorCount;
	COLORREF       colors[16];
	unsigned short stitchCount;
};

using PCSHEADER = struct _pcsheader;

// ini file structure
struct _iniFil {
	char           defaultDirectory[180];         // default directory
	COLORREF       stitchColors[16];              // colors
	COLORREF       stitchPreferredColors[16];     // stitch preference colors
	COLORREF       backgroundPreferredColors[16]; // background preference colors
	COLORREF       backgroundColor;               // background color
	COLORREF       bitmapColor;                   // bitmap color
	double         minStitchLength;               // minimum stitch length
	double         showStitchThreshold;           // show stitch points
	double         threadSize30;                  // millimeter size of 30 weight thread
	double         threadSize40;                  // millimeter size of 40 weight thread
	double         threadSize60;                  // millimeter size of 60 weight thread
	double         userStitchLength;              // user stitch length
	double         maxStitchLength;               // maximum stitch length
	double         smallStitchLength;             // small stitch size
	double         stitchBoxesThreshold;          // show sitch box level
	double         stitchSpace;                   // stitch spacing between lines of stitches
	double         fillAngle;                     // fill angle
	unsigned       userFlagMap;                   // bitmap for user variables
	double         borderWidth;                   // border width
	unsigned       appliqueColor;                 // applique color
	char           prevNames[OLDNUM][_MAX_PATH];  // last file names
	double         snapLength;                    // snap together length
	double         starRatio;                     // star ratio
	double         spiralWrap;                    // spiral wrap
	COLORREF       bitmapBackgroundColors[16];    // bitmap background color preferences
	double         buttonholeCornerLength;        // buttonhole fill corner length
	float          picotSpace;                    // space between border picots
	char           hoopType;                      // hoop type
	char           auxFileType;                   // machine file type
	float          hoopSizeX;                     // hoop x size
	float          hoopSizeY;                     // hoop y size
	double         rotationAngle;                 // rotation angle
	float          gridSize;                      // grid size
	float          clipOffset;                    // clipboard offset
	RECT           initialWindowCoords;           // initial window coordinates
	COLORREF       gridColor;                     // grid color
	unsigned       fillPhase;                     // clipboard fill phase
	float          customHoopX;                   // custom hoop width
	float          customHoopY;                   // custom hoop height
	float          traceLength;                   // lens points
	double         traceRatio;                    // trace ratio
	float          chainSpace;                    // chain space
	float          chainRatio;                    // chain ratio
	float          cursorNudgeStep;               // cursor nudge step
	unsigned short nudgePixels;                   // nudge pixels
	float          eggRatio;                      // egg ratio
	unsigned short stitchSizePixels;              // size of stitch points in pixels
	unsigned short formVertexSizePixels;          // size of form points in pixels
	unsigned short formSides;                     // sides of a created form
	float          tearTailLength;                // length of the tear tail
	float          tearTwistStep;                 // tear twist step
	float          tearTwistRatio;                // tear twist ratio
	unsigned short wavePoints;                    // wave points
	unsigned short waveStart;                     // wave strting point
	unsigned short waveEnd;                       // wave ending point;
	unsigned short waveLobes;                     // wave lobes
	unsigned char  featherFillType;               // feather fill type
	unsigned char  featherUpCount;                // feather up count
	unsigned char  featherDownCount;              // feather down count
	unsigned char  featherType;                   // feather bits
	float          featherRatio;                  // feather ratio
	float          featherMinStitchSize;          // feather floor
	unsigned short featherCount;                  // feather fill psg granularity
	char           p2cName[_MAX_PATH];            // pes2card file
	float          underlayIndent;                // edge walk/underlay indent
	float          underlayAngle;                 // underlay angle
	float          underlaySpacing;               // underlay spacing
	float          underlayStitchLen;             // underlay stitch length
	float          daisyDiameter;                 // daisy diameter
	float          daisyPetalLen;                 // daisy petal length
	float          daisyHoleDiameter;             // daisy hole diameter
	unsigned       daisyPetalCount;               // daisy petals
	unsigned       daisyPetalPoints;              // daisy petal points
	unsigned       daisyInnerCount;               // daisy inner count
	unsigned char  daisyBorderType;               // daisy border type
	unsigned char  dataCheck;                     // data check
	float          textureHeight;                 // textured fill height
	float          textureWidth;                  // textured fill width
	float          textureSpacing;                // textured fill spacing
	unsigned short formBoxSizePixels;             // form box pixels
	unsigned short daisyHeartCount;               // daisy heart count
	unsigned short textureEditorSize;             // texture editor pixels
	float          clpspc;                        // clipboard fill spacing
	char           designerName[50];              // designer name
};

using INIFILE = struct _iniFil;

enum machineType
{
	AUXPCS,
	AUXDST,
	AUXPES
};

struct _formVertices {
	unsigned int start;
	unsigned int vertexCount;
	unsigned int finish;
	unsigned int form;
};

using FORMVERTICES = struct _formVertices;

class fPOINT;
class fPOINTATTR;
class SMALPNTL;
class BSEQPNT;

class dPOINT
{
public:
	double x{ 0.0 };
	double y{ 0.0 };

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
	float x{ 0.0f };
	float y{ 0.0f };

	constexpr fPOINT() noexcept = default;
	inline fPOINT(double rhsX, double rhsY) noexcept;
	inline fPOINT(float rhsX, float rhsY) noexcept;
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
	float    x;
	float    y;
	unsigned attribute;
};

class SMALPNTL
{
public:
	unsigned line; // line and group must remain in this order for sort to work
	unsigned group;
	double   x;
	double   y;
};

class BSEQPNT
{
public:
	float x;
	float y;
	char  attribute;
};

inline bool fPOINT::operator==(const fPOINT& rhs) const noexcept {
	return (x == rhs.x) && (y == rhs.y);
}

inline fPOINT::fPOINT(float rhsX, float rhsY) noexcept
    : x(rhsX)
    , y(rhsY) {
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
	x = rhs.x;
	y = rhs.y;
	return *this;
}

inline fPOINT& fPOINT::operator=(const SMALPNTL& rhs) noexcept {
	x = rhs.x;
	y = rhs.y;
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

struct _doublePointLine {
	double         x;
	double         y;
	unsigned short line;
};

using dPOINTLINE = struct _doublePointLine;

struct _doublerectangle {
	double top;
	double bottom;
	double left;
	double right;
};

using dRECTANGLE = struct _doublerectangle;

struct _colChng {
	unsigned short stitchIndex; // stitch index
	unsigned short colorIndex;  // color index
};

using COLCHNG = struct _colChng;

class FEATHER
{
public:
	unsigned     fillType;          // type of feather fill
	float        ratio;             // feather ratio
	float        minStitch;         // smallest stitch allowed
	float        ratioLocal;        // local feather ratio
	float        formRatio;         // feather ratio from form
	unsigned     extendedAttribute; // extended form attribute
	unsigned     upCount;           // feather up count
	unsigned     downCount;         // feather down count
	unsigned     totalCount;        // up count plus down count
	unsigned int phaseIndex;
	unsigned     phase;
	unsigned     countUp;
	unsigned     countDown;
	float        globalStep;
	float        globalPosition;
	float        globalPhase;
	float        globalUp;
	float        globalDown;
	float        globalRatio;
};

#define COLMSK 0x0000000f    // 0000 0000 0000 0000 0000 0000 0000 1111
#define APCOLMSK 0x000000f0  // 0000 0000 0000 0000 0000 0000 1111 0000
#define NCOLMSK 0xfffffff0   // 1111 1111 1111 1111 1111 1111 1111 0000
#define COLSMSK 0x0000ffff   // 0000 0000 0000 0000 1111 1111 1111 1111
#define FRMSK 0x00003ff0     // 0000 0000 0000 0000 0011 1111 1111 0000
#define NFRMSK 0xffffc00f    // 1111 1111 1111 1111 1100 0000 0000 1111
#define UFRMSK 0x80003ff0    // 1000 0000 0000 0000 0011 1111 1111 0000
#define TYPFMSK 0x20003ff0   // 0010 0000 0000 0000 0011 1111 1111 0000
#define TYPBMSK 0x40003ff0   // 0100 0000 0000 0000 0011 1111 1111 0000
#define TYPAPMSK 0x60003ff0  // 0110 0000 0000 0000 0011 1111 1111 0000
#define LAYMSK 0x0e000000    // 0000 1110 0000 0000 0000 0000 0000 0000
#define NLAYMSK 0xf1ffffff   // 1111 0001 1111 1111 1111 1111 1111 1111
#define TYPMSK 0x60000000    // 0110 0000 0000 0000 0000 0000 0000 0000
#define TYPFRM 0x20000000    // 0010 0000 0000 0000 0000 0000 0000 0000
#define TYPBRD 0x40000000    // 0100 0000 0000 0000 0000 0000 0000 0000
#define NTYPMSK 0x9fffffff   // 1001 1111 1111 1111 1111 1111 1111 1111
#define USMSK 0x80000000u    // 1000 0000 0000 0000 0000 0000 0000 0000
#define ATMSK 0x7fffffff     // 0111 1111 1111 1111 1111 1111 1111 1111
#define NFRM_NTYP 0x9fffc00f // 1001 1111 1111 1111 1100 0000 0000 1111
#define TYPATMSK 0x20000000  // 0010 0000 0000 0000 0000 0000 0000 0000
#define WLKMSK 0x00200000    // 0000 0000 0010 0000 0000 0000 0000 0000
#define WLKFMSK 0x00203ff0   // 0000 0000 0010 0000 0011 1111 1111 0000
#define CWLKMSK 0x00100000   // 0000 0000 0001 0000 0000 0000 0000 0000
#define CWLKFMSK 0x00103ff0  // 0000 0000 0001 0000 0011 1111 1111 0000
#define UNDMSK 0x00400000    // 0000 0000 0100 0000 0000 0000 0000 0000
#define WUNDMSK 0x00600000   // 0000 0000 0110 0000 0000 0000 0000 0000
#define UNDFMSK 0x00403ff0   // 0000 0000 0100 0000 0011 1111 1111 0000
#define KNOTMSK 0x00800000   // 0000 0000 1000 0000 0000 0000 0000 0000
#define NKNOTMSK 0xff7fffff  // 1111 1111 0111 1111 1111 1111 1111 1111
#define FTHMSK 0x01000000    // 0000 0001 0000 0000 0000 0000 0000 0000
#define DELMSK 0x61e03ff0    // 0110 0001 1110 0000 0011 1111 1111 0000
#define ALTYPMSK 0x61f00000  // 0110 0001 1111 0000 0000 0000 0000 0000
#define SRTMSK 0x61f03fff    // 0110 0001 1111 0000 0011 1111 1111 0000
#define SRTYPMSK 0x61700000  // 0110 0001 0111 0000 0000 0000 0000 0000
#define NOTFRM 0x00080000    // 0000 0000 0000 1000 0000 0000 0000 0000
#define FRMSHFT 4u
#define LAYSHFT 25
#define TYPSHFT 29
#define USHFT 31

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

#define FRMFIL 0x20000000
#define FRMBFIL 0x40000000
#define FRMAPFIL 0x60000000

struct _floatRectangle {
	float top;
	float left;
	float right;
	float bottom;
};

using fRECTANGLE = struct _floatRectangle;

struct _forminfo {
	unsigned     type;
	unsigned     attribute;
	unsigned int sideCount;
};

using FORMINFO = struct _forminfo;

class SATCONOUT;

class SATCON
{
public:
	unsigned int start;
	unsigned int finish;

	inline SATCON& operator=(const SATCONOUT& rhs) noexcept;
};

class SATCONOUT
{
public:
	unsigned short start;
	unsigned short finish;

	SATCONOUT() noexcept;
	// SATCONOUT(SATCONOUT&&) = default;
	// SATCONOUT& operator=(const SATCONOUT& rhs) = default;
	// SATCONOUT& operator=(SATCONOUT&&) = default;
	//~SATCONOUT() = default;

	explicit SATCONOUT(const SATCON& rhs);
	inline SATCONOUT& operator=(const SATCON& rhs);
};

inline SATCONOUT::SATCONOUT() noexcept {
	start  = 0u;
	finish = 0u;
}

inline SATCONOUT::SATCONOUT(const SATCON& rhs) {
	start  = gsl::narrow<unsigned short>(rhs.start);
	finish = gsl::narrow<unsigned short>(rhs.finish);
}

inline SATCONOUT& SATCONOUT::operator=(const SATCON& rhs) {
	start  = gsl::narrow<unsigned short>(rhs.start);
	finish = gsl::narrow<unsigned short>(rhs.finish);

	return *this;
}

inline SATCON& SATCON::operator=(const SATCONOUT& rhs) noexcept {
	start  = rhs.start;
	finish = rhs.finish;

	return *this;
}

union FANGCLPOUT;

union FANGCLP {
public:
	float        angle;
	unsigned int clip;
	SATCON       guide;

	inline FANGCLP& operator=(const FANGCLPOUT& rhs) noexcept;
};

union FANGCLPOUT {
public:
	float     angle;
	DWORD     clip{}; // clip pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
	SATCONOUT guide;

	FANGCLPOUT() noexcept;
	// FANGCLPOUT(FANGCLPOUT&&) = default;
	// FANGCLPOUT& operator=(const FANGCLPOUT& rhs) = default;
	// FANGCLPOUT& operator=(FANGCLPOUT&&) = default;
	//~FANGCLPOUT() = default;

	explicit FANGCLPOUT(const FANGCLP& rhs) noexcept;
	inline FANGCLPOUT& operator=(const FANGCLP& rhs) noexcept;
};

inline FANGCLPOUT::FANGCLPOUT() noexcept {
	clip = 0;
}

inline FANGCLPOUT::FANGCLPOUT(const FANGCLP& rhs) noexcept {
	angle = rhs.angle;
}

inline FANGCLPOUT& FANGCLPOUT::operator=(const FANGCLP& rhs) noexcept {
	angle = rhs.angle;
	return *this;
}

inline FANGCLP& FANGCLP::operator=(const FANGCLPOUT& rhs) noexcept {
	angle = rhs.angle;
	return *this;
}

union FLENCNTOUT;

union FLENCNT {
public:
	float        stitchLength;
	unsigned int clipCount;

	inline FLENCNT& operator=(const FLENCNTOUT& rhs) noexcept;
};

union FLENCNTOUT {
public:
	float    stitchLength{};
	unsigned clipCount;

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
	SATCON* guide;
	float   angle;

	inline SATINANGLE& operator=(const SATINANGLEOUT& rhs) noexcept;
};

union SATINANGLEOUT {
public:
	DWORD guide{};
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

#define BELEN 1
#define BESPAC 2
#define BESIZ 4
#define BNCLP 8
#define BPICSPAC 16
#define BCNRSIZ 32
#define BRDEND 64
#define BRDPOS 128
#define BEMAX 256
#define BEMIN 512
#define CHNPOS 1024

#define MEGLIN (BELEN | BEMAX | BEMIN)
#define MEGBLD (BELEN | BEMAX | BEMIN)
#define MEGCLP (BNCLP | BEMAX | BEMIN)
#define MEGSAT (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN)
#define MEGAP (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN)
#define MEGPRP (BESPAC | BESIZ | BRDEND | BEMAX | BEMIN)
#define MEGHOL (BELEN | BESPAC | BESIZ | BCNRSIZ | BEMAX | BEMIN)
#define MEGPIC (BELEN | BESIZ | BNCLP | BPICSPAC | BCNRSIZ | BEMAX | BEMIN)
#define MEGDUB (BELEN | BEMAX | BEMIN)
#define MEGCHNL (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS)
#define MEGCHNH (BESIZ | BESPAC | BEMAX | BEMIN | CHNPOS)
#define MEGCLPX (BNCLP | BEMAX | BEMIN)

#define EGLIN_LINS 4
#define EGBLD_LINS 4
#define EGCLP_LINS 3
#define EGSAT_LINS 6
#define EGAP_LINS 7
#define EGPRP_LINS 6
#define EGHOL_LINS 7
#define EGPIC_LINS 7
#define EGCHN_LINS 6

struct _featherInfo {
	unsigned char  fillType;      // feather fill type
	unsigned char  upCount;       // feather up count
	unsigned char  downCount;     // feather down count
	unsigned char  color;         // feather blend color
	float          ratio;         // feather ratio
	float          minStitchSize; // feather floor
	unsigned short count;         // feather fill psg granularity
};

using FTHINFO = struct _featherInfo;

struct _textureInfo {
	short          lines;
	unsigned short index;
	unsigned short count;
	float          height;
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
	unsigned char  attribute;       // attribute
	unsigned short vertexCount;     // number of sides
	unsigned char  type;            // type
	unsigned char  fillColor;       // fill color
	unsigned char  borderColor;     // border color
	unsigned short clipEntries;     // number of border clipboard entries
	fPOINT*        vertices;        // points
	SATINANGLE     satinOrAngle;    // satin guidelines or angle clipboard fill angle
	unsigned int   borderClipData;  // border clipboard data
	unsigned short satinGuideCount; // number of satin guidelines
	unsigned short wordParam;       // word parameter
	fRECTANGLE     rectangle;       // rectangle
	unsigned char  fillType;        // fill type
	unsigned char  edgeType;        // edge type
	float          fillSpacing;     // fill spacing
	FLENCNT        lengthOrCount;   // fill stitch length or clipboard count
	FANGCLP        angleOrClipData; // fill angle or clipboard data pointer
	float          borderSize;      // border size
	float          edgeSpacing;     // edge spacing
	float          edgeStitchLen;   // edge stitch length
	unsigned short res;             // pico length
};

class FRMHEDOUT;

class FRMHED
{
public:
	unsigned char  attribute;       // attribute
	unsigned int   vertexCount;     // number of sides
	unsigned char  type;            // type
	unsigned char  fillColor;       // fill color
	unsigned char  borderColor;     // border color
	unsigned int   clipEntries;     // number of border clipboard entries
	fPOINT*        vertices;        // points
	SATINANGLE     satinOrAngle;    // satin guidelines or angle clipboard fill angle
	unsigned int   borderClipData;  // border clipboard data
	unsigned int   satinGuideCount; // number of satin guidelines
	unsigned int   wordParam;       // clipboard/textured fill phase or satin end guide
	fRECTANGLE     rectangle;       // rectangle
	unsigned char  fillType;        // fill type
	unsigned char  edgeType;        // edge type
	float          fillSpacing;     // fill spacing
	FLENCNT        lengthOrCount;   // fill stitch length or clipboard count
	FANGCLP        angleOrClipData; // fill angle or clipboard data pointer
	float          borderSize;      // border size
	float          edgeSpacing;     // edge spacing
	float          edgeStitchLen;   // edge stitch length
	unsigned short picoLength;      // pico length

	unsigned      extendedAttribute;   // attribute extension
	float         maxFillStitchLen;    // maximum fill stitch length
	float         minFillStitchLen;    // minimum fill stitch length
	float         maxBorderStitchLen;  // maximum border stitch length
	float         minBorderStitchLen;  // minimum border stitch length
	TFINFO        fillInfo;            // feather/texture info
	unsigned int  fillStart;           // fill start point
	unsigned int  fillEnd;             // fill end point
	float         underlaySpacing;     // underlay spacing
	float         underlayStitchLen;   // underlay stitch length
	float         underlayStitchAngle; // underlay stitch angle
	float         underlayIndent;      // underlay/edge walk indent
	float         txof;                // gradient end density
	unsigned char underlayColor;       // underlay color
	unsigned char cres;                // reserved

	inline FRMHED& operator=(const FRMHEDO& rhs) noexcept;
	inline FRMHED& operator=(const FRMHEDOUT& rhs) noexcept;
};

inline FRMHED& FRMHED::operator=(const FRMHEDO& rhs) noexcept {
	attribute       = rhs.attribute;
	vertexCount     = rhs.vertexCount;
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = rhs.clipEntries;
	vertices        = rhs.vertices; // Todo - Should we be copying this value?
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = rhs.borderClipData; // Todo - Should we be copying this value?
	satinGuideCount = rhs.satinGuideCount;
	wordParam       = rhs.wordParam;
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	angleOrClipData = rhs.angleOrClipData;
	borderSize      = rhs.borderSize;
	edgeSpacing     = rhs.edgeSpacing;
	edgeStitchLen   = rhs.edgeStitchLen;
	picoLength      = rhs.res;
	return *this;
}

class FRMHEDOUT
{
public:
	unsigned char  attribute;    // attribute
	unsigned short vertexCount;  // number of sides
	unsigned char  type;         // type
	unsigned char  fillColor;    // fill color
	unsigned char  borderColor;  // border color
	unsigned short clipEntries;  // number of border clipboard entries
	DWORD          vertices;     // vertices pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
	SATINANGLEOUT  satinOrAngle; // satin guidelines or angle clipboard fill angle
	DWORD borderClipData; // border clipboard data pointer not saved. size is to keep compatibility with v1 & v2 ThrEd files
	unsigned short satinGuideCount; // number of satin guidelines
	unsigned short wordParam;       // clipboard/textured fill phase or satin end guide
	fRECTANGLE     rectangle{};     // rectangle
	unsigned char  fillType;        // fill type
	unsigned char  edgeType;        // edge type
	float          fillSpacing;     // fill spacing
	FLENCNTOUT     lengthOrCount;   // fill stitch length or clipboard count
	FANGCLPOUT     angleOrClipData; // fill angle or clipboard data pointer
	float          borderSize;      // border size
	float          edgeSpacing;     // edge spacing
	float          edgeStitchLen;   // edge stitch length
	unsigned short picoLength;      // pico length

	unsigned int   extendedAttribute;   // attribute extension
	float          maxFillStitchLen;    // maximum fill stitch length
	float          minFillStitchLen;    // minimum fill stitch length
	float          maxBorderStitchLen;  // maximum border stitch length
	float          minBorderStitchLen;  // minimum border stitch length
	TFINFO         fillInfo{};          // feather/texture info
	unsigned short fillStart;           // fill start point
	unsigned short fillEnd;             // fill end point
	float          underlaySpacing;     // underlay spacing
	float          underlayStitchLen;   // underlay stitch length
	float          underlayStitchAngle; // underlay stitch angle
	float          underlayIndent;      // underlay/edge walk indent
	float          txof;                // gradient end density
	unsigned char  underlayColor;       // underlay color
	unsigned char  cres;                // reserved

	FRMHEDOUT() noexcept;
	// FRMHEDOUT(FRMHEDOUT&&) = default;
	// FRMHEDOUT& operator=(const FRMHEDOUT& rhs) = default;
	// FRMHEDOUT& operator=(FRMHEDOUT&&) = default;
	//~FRMHEDOUT() = default;

	explicit inline FRMHEDOUT(const FRMHED& rhs);
	inline FRMHEDOUT& operator=(const FRMHED& rhs);
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
	vertices        = 0;
	borderClipData  = 0;
	satinGuideCount = 0;
	wordParam       = 0;
	rectangle       = {0.0f, 0.0f, 0.0f, 0.0f};
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

inline FRMHEDOUT::FRMHEDOUT(const FRMHED& rhs) {
	attribute       = rhs.attribute;
	vertexCount     = gsl::narrow<unsigned short>(rhs.vertexCount);
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = gsl::narrow<unsigned short>(rhs.clipEntries);
	vertices        = 0; // do not write the pointer value to file
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = 0; // do not write the pointer value to file
	satinGuideCount = gsl::narrow<unsigned short>(rhs.satinGuideCount);
	wordParam       = gsl::narrow<unsigned short>(rhs.wordParam);
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	angleOrClipData = rhs.angleOrClipData;
	borderSize      = rhs.borderSize;
	edgeSpacing     = rhs.edgeSpacing;
	edgeStitchLen   = rhs.edgeStitchLen;
	picoLength      = rhs.picoLength;

	extendedAttribute   = rhs.extendedAttribute;
	maxFillStitchLen    = rhs.maxFillStitchLen;
	minFillStitchLen    = rhs.minFillStitchLen;
	maxBorderStitchLen  = rhs.maxBorderStitchLen;
	minBorderStitchLen  = rhs.minBorderStitchLen;
	fillInfo            = rhs.fillInfo;
	fillStart           = gsl::narrow<unsigned short>(rhs.fillStart);
	fillEnd             = gsl::narrow<unsigned short>(rhs.fillEnd);
	underlaySpacing     = rhs.underlaySpacing;
	underlayStitchLen   = rhs.underlayStitchLen;
	underlayStitchAngle = rhs.underlayStitchAngle;
	underlayIndent      = rhs.underlayIndent;
	txof                = rhs.txof;
	underlayColor       = rhs.underlayColor;
	cres                = rhs.cres;
}

inline FRMHEDOUT& FRMHEDOUT::operator=(const FRMHED& rhs) {
	attribute       = rhs.attribute;
	vertexCount     = gsl::narrow<unsigned short>(rhs.vertexCount);
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = gsl::narrow<unsigned short>(rhs.clipEntries);
	vertices        = 0; // do not write the pointer value to file
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = 0; // do not write the pointer value to file
	satinGuideCount = gsl::narrow<unsigned short>(rhs.satinGuideCount);
	wordParam       = gsl::narrow<unsigned short>(rhs.wordParam);
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	angleOrClipData = rhs.angleOrClipData;
	borderSize      = rhs.borderSize;
	edgeSpacing     = rhs.edgeSpacing;
	edgeStitchLen   = rhs.edgeStitchLen;
	picoLength      = rhs.picoLength;

	extendedAttribute   = rhs.extendedAttribute;
	maxFillStitchLen    = rhs.maxFillStitchLen;
	minFillStitchLen    = rhs.minFillStitchLen;
	maxBorderStitchLen  = rhs.maxBorderStitchLen;
	minBorderStitchLen  = rhs.minBorderStitchLen;
	fillInfo            = rhs.fillInfo;
	fillStart           = gsl::narrow<unsigned short>(rhs.fillStart);
	fillEnd             = gsl::narrow<unsigned short>(rhs.fillEnd);
	underlaySpacing     = rhs.underlaySpacing;
	underlayStitchLen   = rhs.underlayStitchLen;
	underlayStitchAngle = rhs.underlayStitchAngle;
	underlayIndent      = rhs.underlayIndent;
	txof                = rhs.txof;
	underlayColor       = rhs.underlayColor;
	cres                = rhs.cres;

	return *this;
}

inline FRMHED& FRMHED::operator=(const FRMHEDOUT& rhs) noexcept {
	attribute       = rhs.attribute;
	vertexCount     = rhs.vertexCount;
	type            = rhs.type;
	fillColor       = rhs.fillColor;
	borderColor     = rhs.borderColor;
	clipEntries     = rhs.clipEntries;
	vertices        = nullptr; // do not read the pointer from file
	satinOrAngle    = rhs.satinOrAngle;
	borderClipData  = 0u; // do not read the index from file
	satinGuideCount = rhs.satinGuideCount;
	wordParam       = rhs.wordParam;
	rectangle       = rhs.rectangle;
	fillType        = rhs.fillType;
	edgeType        = rhs.edgeType;
	fillSpacing     = rhs.fillSpacing;
	lengthOrCount   = rhs.lengthOrCount;
	angleOrClipData = rhs.angleOrClipData;
	borderSize      = rhs.borderSize;
	edgeSpacing     = rhs.edgeSpacing;
	edgeStitchLen   = rhs.edgeStitchLen;
	picoLength      = rhs.picoLength;

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

#define FRMEND 1u
#define FRMLMSK 0x0eu
#define NFRMLMSK 0xf1u

/*form attribute bits

1		end teflon line
2-4	layer bits
5		finish blunt
6		start blunt
7		refill bit for contour fill
*/

// blunt bits
#define FBLNT 0x20u
#define SBLNT 0x40u
#define NFBLNT 0xdfu
#define NSBLNT 0xbfu
#define NOBLNT 0x9fu

// frmhed extended attribute bits

#define AT_SQR 1     // square ends
#define AT_FTHUP 2   // feather up or down flag
#define AT_FTHBTH 4  // feather both up and down flag
#define AT_FTHBLND 8 // feather blend flag
#define AT_STRT 16   // user set start flag
#define AT_END 32    // user set end flag
#define AT_UND 64    // underlay flag
#define AT_WALK 128  // edge walk
#define AT_CWLK 256  // center walk

// contour refil
#define FRECONT 0x80
#define NFRECONT 0x7f

struct _frmclp {
	unsigned clipType;
	unsigned reserved;
	FRMHED   form;
};

using FORMCLIP = struct _frmclp; // form data clipboard header

struct _frmsclp {
	unsigned       clipType;
	unsigned short formCount;
	unsigned short reserved;
};

using FORMSCLIP = struct _frmsclp; // multiple forms clipboard header

struct _fvclip {
	unsigned     clipType;
	unsigned int vertexCount;
	bool         direction;
};

using FORMVERTEXCLIP = struct _fvclip; // form points clipboard header

struct _strhed { // thred file header
	unsigned       headerType;
	unsigned       fileLength;    // length of strhed + length of stitch data
	unsigned short stitchCount;   // number of stitches
	unsigned short hoopType;      // size of hoop
	unsigned short formCount;     // number of forms
	unsigned short vertexLen;     // points to form points
	unsigned short vertexCount;   // number of form points
	unsigned short dlineLen;      // points to dline data
	unsigned short dlineCount;    // dline data count
	unsigned short clipDataLen;   // points to clipboard data
	unsigned short clipDataCount; // clipboard data count
};

using STRHED = _strhed;

struct _txpnt { // textured fill point
	float          y;
	unsigned short line;
};

using TXPNT = struct _txpnt;

struct _txoff { // textured fill offset
	float y;
	int   line;
};

using TXOFF = struct _txoff;

#define RES_SIZE 26
struct _strex {                 // thred v1.0 file header extension
	float    hoopSizeX;         // hoop size x dimension
	float    hoopSizeY;         // hoop size y dimension
	float    stgran;            // stitches per millimeter
	char     creatorName[50];   // name of the file creator
	char     modifierName[50];  // name of last file modifier
	char     auxFormat;         // auxillary file format
	char     stres;             // reserved
	unsigned texturePointCount; // textured fill point count
	char     res[RES_SIZE];     // reserved for expansion
};

using STREX = struct _strex;

struct _dsthed {      // dst file header
	char desched[3];  // 0 0		description
	char desc[17];    // 3 3
	char recshed[3];  // 20 14	record count
	char recs[8];     // 23 17
	char cohed[3];    // 31 1F
	char co[4];       // 34 22
	char xplushed[3]; // 38 26	x+ size
	char xplus[6];    // 41 29
	char xminhed[3];  // 47 2F	x- size
	char xmin[6];     // 50 32
	char yplushed[3]; // 56 38
	char yplus[6];    // 59 3B	y+ size
	char yminhed[3];  // 65 41
	char ymin[6];     // 68 44	y- size
	char axhed[3];    // 74 4A
	char ax[7];       // 77 4D
	char ayhed[3];    // 84 54
	char ay[7];       // 87 57
	char mxhed[3];    // 94 5E
	char mx[7];       // 97 61
	char myhed[3];    // 104 68
	char my[7];       // 107 6B
	char pdhed[2];    // 114 72
	char pd[7];       // 116 74
	char eof[1];      // 123 7B
	char res[388];    // 124 7C
};

using DSTHED = struct _dsthed;

// dst type masks

#define JMPTYP 0x830000
#define COLTYP 0x630000
#define REGTYP 0x030000

struct _dstrec { // dst stitch record
	unsigned char led;
	unsigned char mid;
	unsigned char nd;
};

using DSTREC = struct _dstrec;

struct _dstoffsets {
	POINT Positive; // plus offset written into the destination file header
	POINT Negative; // minus offset written into the destination file header
};

using DSTOffsets = struct _dstoffsets;

struct _pcstch {
	unsigned char fx; // fractional part of x coord or color if tag = 3
	short         x;  // integer part of x coord
	unsigned char nx;
	unsigned char fy; // fractional part of y coord
	short         y;  // integer part of y coord
	unsigned char ny;
	unsigned char tag;
};

using PCSTCH = struct _pcstch;

struct _clpstch {
	unsigned int   led;  // ????
	unsigned char  fx;   // fractional part of x coord
	unsigned short x;    // integer part of x coord
	unsigned char  spcx; // ToDo - Is this structure member needed?
	unsigned char  fy;   // fractional part of y coord
	unsigned short y;    // integer part of y coord
	unsigned char  spcy; // ToDo - Is this structure member needed?
	unsigned char  myst; // ToDo - Is this structure member needed?
	unsigned char  tag;  // ToDo - Is this structure member needed?
};

using CLPSTCH = struct _clpstch;

struct _bakhed {
	unsigned int formCount;
	FRMHED*      forms;
	unsigned     stitchCount;
	fPOINTATTR*  stitches;
	unsigned int vertexCount;
	fPOINT*      vertices;
	unsigned int guideCount;
	SATCON*      guide;
	unsigned int clipPointCount;
	fPOINT*      clipPoints;
	COLORREF*    colors;
	TXPNT*       texturePoints;
	unsigned int texturePointCount;
	POINT        zoomRect;
};

using BAKHED = struct _bakhed;

struct _flsiz {
	float cx;
	float cy;
};

using FLSIZ = struct _flsiz;

struct _frmrange {
	unsigned int start;
	unsigned int finish;
};

using FRMRANGE = struct _frmrange;

struct _range {
	unsigned start;
	unsigned finish;
};

using RANGE = struct _range;

class REGION // region for sequencing vertical fills
{
public:
	unsigned start{ 0u };       // start line of region
	unsigned end{ 0u };         // end line of region
	unsigned regionBreak{ 0u }; // ToDo - Is this member needed?
	unsigned breakCount{ 0u };

	constexpr REGION() noexcept = default;
	inline REGION(unsigned rhsStart, unsigned rhsEnd, unsigned rhsBreak, unsigned rhsCount) noexcept;
	// REGION(REGION&&) = default;
	// REGION& operator=(const REGION& rhs) = default;
	// REGION& operator=(REGION&&) = default;
	//~REGION() = default;
};

inline REGION::REGION(unsigned rhsStart, unsigned rhsEnd, unsigned rhsBreak, unsigned rhsCount) noexcept
    : start(rhsStart)
    , end(rhsEnd)
    , regionBreak(rhsBreak)
    , breakCount(rhsCount) {
}

struct _rcon { // PathMap: path map for sequencing
	unsigned node;
	bool     isConnected;
	unsigned nextGroup;
};

using RCON = struct _rcon;

struct _rgseq {    // TempPath: temporary path connections
	unsigned pcon; // pointer to PathMap entry
	int      count;
	bool     skp; // path not found
};

using RGSEQ = struct _rgseq;

struct _fseq { // SequencePath: path of sequenced regions
	unsigned short node;
	unsigned short nextGroup;
	bool           skp; // path not found
};

using FSEQ = struct _fseq;

#define SEQTOP 2
#define SEQBOT 3

struct _pvec {
	double angle;
	double length;
};

using PVEC = struct _pvec;

struct _vrct2 {
	dPOINT aipnt;
	dPOINT aopnt;
	dPOINT bipnt;
	dPOINT bopnt;
	dPOINT cipnt;
	dPOINT copnt;
	dPOINT dipnt;
	dPOINT dopnt;
};

using VRCT2 = struct _vrct2;

struct _cursorMask {
	unsigned char form[128];
	unsigned char dline[128];
	unsigned char uprightNeedle[128];
	unsigned char leftUpNeedle[128];
	unsigned char leftDownNeedle[128];
	unsigned char rightUpNeedle[128];
	unsigned char rightDownNeedle[128];
};

using CURSORMASK = struct _cursorMask;

// balarad file header
struct _balhed {
	COLORREF       color[256];
	unsigned       signature;
	unsigned short version;
	float          hoopSizeX;
	float          hoopSizeY;
	COLORREF       backgroundColor;
	unsigned char  res[1006];
};

using BALHED = struct _balhed;

// balarad stitch
struct _balstch {
	unsigned char code;
	unsigned char flag;
	float         x;
	float         y;
};

using BALSTCH = struct _balstch;

struct _clpseg {
	unsigned     start;
	float        beginLength;
	unsigned     beginIndex;
	unsigned int asid;
	unsigned     finish;
	float        endLength;
	unsigned     endIndex;
	unsigned int zsid;
	char         dun;
};

using CLPSEG = struct _clpseg;

class CLIPSORT
{
public:
	float        segmentLength{ 0.0f };
	float        sideLength{ 0.0f };
	unsigned int vertexIndex{ 0 };
	fPOINT       point;

	constexpr CLIPSORT() noexcept = default;
};

struct _clipnt {
	float        x;
	float        y;
	unsigned int vertexIndex;
	unsigned     flag;
};

using CLIPNT = struct _clipnt;

struct _vclpx {
	unsigned segment;
	unsigned vertex;
};

using VCLPX = struct _vclpx;

#if PESACT

struct _pesled {
	char     ver[8];
	unsigned pec;
};

using PESLED = struct _pesled;

struct _peshed {
	char  led[8];
	char  off[3];
	char  m1[13];
	char  ce[6];
	char  m2[47];
	short xsiz;
	short ysiz;
	char  m3[16];
	char  cs[6];
	char  m4[3];
	char  scol;
	char  m5[3];
};

using PESHED = struct _peshed;

struct _pestch {
	short x;
	short y;
};

using PESTCH = struct _pestch;

#endif

struct _trcpnt {
	SHORT x;
	SHORT y;
};

using TRCPNT = struct _trcpnt;

// ToDo - Is this struct needed?
struct _txtmsg {
	unsigned cod;
	char*    str;
};

using TXTMSG = struct _txtmsg;

struct _grdcod {
	unsigned id;
	unsigned col;
};

using GRDCOD = struct _grdcod;

#pragma pack()

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

// edge underlay bit
#define EGUND 0x80u
#define NEGUND 0x7fu

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
#define MCLPF 1 << CLPF
#define MVCLPF 1 << VCLPF
#define MHCLPF 1 << HCLPF
#define MANGCLPF 1 << ANGCLPF

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

#define HLIN HNUM

struct _orec {
	unsigned    start;
	unsigned    finish;
	fPOINTATTR* startStitch;
	fPOINTATTR* endStitch;
	unsigned    color;
	unsigned    type;
	unsigned    form;
	unsigned    otyp;
};

using OREC = struct _orec;

struct _srtrec {
	unsigned start;         // start region index
	unsigned finish;        // finish region index
	unsigned count;         // number of regions in range
	unsigned currentRegion; // current region index
	bool     direction;     // direction of sort
};

using SRTREC = struct _srtrec;

struct _fstrts {
	unsigned applique;
	unsigned fill;
	unsigned feather;
	unsigned border;
	unsigned appliqueColor;
	unsigned fillColor;
	unsigned featherColor;
	unsigned borderColor;
};

using FSTRTS = struct _fstrts;

union _fillStarts {
	FSTRTS       fillNamed;
	unsigned int fillArray[sizeof(FSTRTS) / sizeof(unsigned int)];
};

using FILLSTARTS = union _fillStarts;

#define M_AP 2
#define M_CWLK 4
#define M_WALK 8
#define M_UND 16
#define M_FIL 32
#define M_FTH 64
#define M_BRD 128
#define M_APCOL 256
#define M_FCOL 512
#define M_FTHCOL 1024
#define M_ECOL 2048

struct _insrec {
	unsigned code;
	unsigned color;
	unsigned index;
	unsigned seq;
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
	unsigned     pins;
	unsigned     coloc;
	unsigned int layerIndex;
	unsigned     start;
	unsigned     output;
	fPOINTATTR*  highStitchBuffer;
};

using INTINF = struct _intinf;

class TXTSCR
{
public:
	int            top{};                   // pixel top line
	int            bottom{};                // pixel bottom line
	int            height{};                // pixel height of area
	int            halfHeight{};            // pixel middle of area
	float          xOffset{ 0.0f };         // edit x offset of area
	float          yOffset{ 0.0f };         // edit y offset of area
	float          areaHeight{ 0.0f };      // edit height of area
	float          screenHeight{ 0.0f };    // edit height of screen
	float          width{ 0.0f };           // edit width of area
	float          spacing{ 0.0f };         // edit space between lines
	unsigned short lines{ 0 };              // number of lines
	double         editToPixelRatio{ 0.0 }; // edit to pixel ratio
	fPOINT         formCenter;              // middle of the form

	constexpr TXTSCR() noexcept = default;
};

struct _txtrct {
	unsigned short left;
	unsigned short right;
	float          top;
	float          bottom;
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
	unsigned*    placeholder;
	unsigned int count;
	float        height;
	float        width;
	float        spacing;
};

using TXHSTBUF = struct _txhstbuff;

struct _rngcnt {
	int line;
	int stitchCount;
};

using RNGCNT = struct _rngcnt;

#define BADFLT 1
#define BADCLP 2
#define BADSAT 4
#define BADTX 8

struct _badcnts {
	unsigned     attribute;
	unsigned int flt;
	unsigned int clip;
	unsigned int guideCount; // ToDo - is this an accurate description?
	unsigned int tx;
};

using BADCNTS = struct _badcnts;

struct _findInfo {
	unsigned         count;
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
	unsigned index;
	bool     isEnd;
	float    length;
};

using LENINFO = struct _lengthInfo;

// Do the type punning while ensuring that the returned pointer is non_null
// use the encapsulation recommended in I.30
// (https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#i30-encapsulate-rule-violations)
template <class T2, class T1> inline _Ret_notnull_ T2 convert_ptr(T1* pointer) {
	GSL_SUPPRESS(26474) {
		if (pointer) {
			return static_cast<T2>(static_cast<void*>(pointer));
		}

		throw;
	}
}
