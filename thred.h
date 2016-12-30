//compile switches
#define		__UseASM__	0
#define		PESACT		1			//compile pes code
#define		BUGBAK		0			//turn bakseq off

#define		TRCMTH		1			//0=brightness compare,1=color compare

#define		RUTVALID	1
#define		COMVALID	2
#define		REGVALID	4

#define		RUTSED		1232323232	
#define		COMSED		1323232323
#define		TIMSED		1313131313
#define		TIMSIG		2211221122		//time signature for trial operation
#define		DAY1		864000000000	//file time ticks in a day
#define		DAY31		26784000000000	//file time ticks in 31 days
#define		DAY400		345600000000000	//file time ticks in 400 days
#define		TRILIM		100				//expired trial clipboard limit
#define		SRTIM		20000000		//sort time limit in 100 ns intervals
//end of trial version codes

//daisy codes
#define		DAZPETS		5			//petals
#define		DAZCNT		10			//petal points
#define		DAZICNT		2			//petal inner points
#define		DAZLEN		15			//diameter
#define		DAZPLEN		20			//petal length
#define		DAZHLEN		5			//hole size
#define		DAZTYP		5			//border type
#define		DAZMCNT		7			//mirror count
//end of daisy codes

#define		TXTRAT		0.95		//texture fill clipboard shrink/grow ratio
#define		MAXMSK		0xffff0000	//for checking for greater than 65536
#define		MAXSEQ		65536		//maximum number of points in the sequence list

#define		DEFBPIX		4			//default form box pixels
#define		MAXWLK		54			//max underlay/edge walk stitch length
#define		MINWLK		2.4			//max underlay/edge walk stitch length
#define		DEFULEN		12;			//default underlay stitch length
#define		DEFUSPAC	6;			//default underlay stitch spacing

#define		IWAVPNTS	36			//default wave points
#define		IWAVSTRT	10			//default wave start
#define		IWAVEND		26			//default wave end
#define		IWAVS		5			//default wave lobes
#define		THRLED0		0x746872	//lead dword value for thred file v 1.0
#define		THRLED1		0x1746872	//lead dword value for thred file v 1.1
#define		MAXFLT		65536		//maximum number of floating point items, form & clipboard points
#define		ZUMFCT		0.65		//zoom factor
#define		PAGSCROL	0.9			//page scroll factor
#define		LINSCROL	0.05		//line scroll factor
#define		TXTSIDS		6			//extra pixels in a text box
#define		MAXPCS		131072		//maximum possible stiches in a pcs file*2
#define		MAXCHNG		10000		//maximum number of color changes
#define		SHUPX		480			//small hoop x size
#define		SHUPY		480			//small hoop y size
#define		LHUPX		719			//large hoop x size
#define		LHUPY		690			//large hoop y size
#define		HUP100XY	600			//100 millimeter hoop size
#define		PFGRAN		6			//pfaf "pixels" per millimeter
#define		TSIZ30		0.3			//#30 thread size in millimeters
#define		TSIZ40		0.2			//#40 thread size in millimeters
#define		TSIZ60		0.05		//#60 thread size in millimeters
#define		SCROLSIZ	12			//width of a scroll bar
#define		COLSIZ		12			//width of the color bar
#define		RIGHTSIZ	24			//SCROLSIZ+COLSIZ
#define		CLOSENUF	10			//mouse click region for select
#define		ZMARGIN		1.25		//zoom margin for select zooms
#define		SMALSIZ		0.25		//default small stitch size
#define		MINSIZ		0.1			//default minimum stitch size
#define		USESIZ		3.5			//user preferred size
#define		MAXSIZ		9.0			//default maximum stitch size
#define		PFAFGRAN	6			//pfaf stitch points per millimeter
#define		RMAPSIZ		65536		//a bit for each stitch
const unsigned long	RMAPBITS = RMAPSIZ << 5;	//a bit for each stitch
#define		MINZUM		5			 //minimum zoom in stitch points
#define		MAXZLEV		9			//maximum levels of zoom
#define		SHOPNTS		0.00		//show stitch points when zoom below this
#define		STCHBOX		0.4226		//show stitch boxes when zoom below this
#define		BITCOL		0xffff00	//default bitmap color
#define		MAXFORMS	1024		//maximum number of forms
#define		FORMFCT		0.05		//new forms part of screen
#define		MAXDELAY	600			//maximum movie delay
#define		MINDELAY	1			//minimum movie delay
#define		MOVITIM		12			//default movie time
#define		DEFSPACE	0.45		//default stitch spacing
#define		JMPSPACE	13			//default jump stitch spacing
#define		DEFANG		0.785398163397448 //default fill angle, 45 degrees
#define		MAXFRMLINS	20000		//maximum lines in a form
#define		MSGSIZ		8192		//size of the message buffer
#define		PI			3.1415926535898
#define		PI2			6.2831853071796
#define		MAXSTCH		54			//maximum permitted stitch length for pfaf in pfaf "stitch pixels"
#define		USPAC		15			//underlay fill spacing
#define		APSPAC		10.8		//applique border spacing
#define		OSEQLEN		262144		//output sequence length
#define		BSEQLEN		OSEQLEN<<1	//reverse sequence length
#define		MAXRAT		3			//maximum number of stitches in one place in satin border
#define		URAT		0.75		//ratio of underlay stitch to satin border size
#define		PURAT		0.6			//for perp satin corners
#define		DIURAT		0.125		//(1-URAT)/2
#define		DOURAT		0.8125		//(1-URAT)/2+URAT
#define		MINRCT		12			//minimum dimension of a form select rectangle
#define		OLDNUM		4			//number of old filenames saved on file menu
#define		OLDVER		4			//number of old file versions kept
#define		TINY		1e-6		//tiny number for floating point stuff
#define		SPEDLIN		30			//speed change for line message on speed scroll bar
#define		SPEDPAG		120			//speed change for page message on speed scroll bar
#define		KNOTLEN		54			//set knots for stitches longer than this
#define		MAXFRMPNTS	65536		//maximum total form points
#define		MAXCLPNTS	65536		//maximum total cliboard storage in forms
#define		MAXSAC		10000		//maximum number of satin guidlins
#define		MAXKNOTS	16384		//maximum nuber of knots
#define		IBFCLEN		4*PFGRAN	//initial buttonhole fill corner length
#define		IPICSPAC	6			//initial picot border space
#define		PRFLINS		28			//number of lines on the prefernece menu
#define		EDGETYPS	12			//number of border fill types
#define		SEED		3037000499	//pseudo-random-sequence seed
#define		FSED		1340007303	//feather seqence seed
#define		NAMSED		2222222222	//trial version psg seed
#define		DSTRAT		0.8333333333333333	//ratio of dst stitch points to PFAFF stitch points
#define		HUPS		5			//number of hoops the user can select
#define		NORDSED		0x5a5a5a5a	//name order seed
#define		NCODSED		0x73ef5a7e	//name encoding seed
#define		NCODOF		80			//name encoding offset
#define		CLPMIN		0.5			//if clipboard data less wide, then don't fill
#define		CLPMINAUT	1.2			//for skinny vertical clips
#define		BRDWID		18			//default satin border size
#define		SNPLEN		0.15		//default snap together length size
#define		STARAT		0.4			//default star ratio
#define		SPIRWRAP	1.52		//default spiral wrap
#define		BALNORM		0x80		//normal balarad stitch
#define		BALJUMP		0x81		//balarad jump stitch
#define		BALSTOP		0			//balarad stop
#define		COLVER		0x776874	//color file version
#define		PESCMSK		0x3f		//pes color mask
#define		REDCOL		0xff		//code for the color red
#define		GRNCOL		0xff00		//code for the color green
#define		BLUCOL		0xff0000	//code for the color blue
#define		REDMSK		0xffff00	//mask for the color red
#define		GRNMSK		0xff00ff	//mask for the color green
#define		BLUMSK		0x00ffff	//mask for the color blue
#define		TRACLEN		1			//initial trace length
#define		TRACRAT		1.00001		//initial trace ratio
#define		CHSDEF		24			//default chain stitch length
#define		CHRDEF		0.25		//default chain stitch ratio
#define		NUGINI		2			//default nudge step
#define		DEFPIX		2			//default nudge pixels
#define		DEFEGRAT	1.5			//default egg ratio
#define		DEFPNTPIX	4			//default form and stitch point pixels
#define		HBUFSIZ		1024		//help buffer size
#define		HIGRD		0xffffff	//grid high color
#define		MEDGRD		0x404040	//grid medium color
#define		DEFGRD		0x202020	//grid default color
#define		REDGRD		0xff2020	//grid red color
#define		BLUGRD		0x20ff20	//grid green color
#define		GRNGRD		0x2020ff	//grid blue color
#define		FDEFRAT		0.6			//default feather ratio
#define		FDEFUP		10			//default feather up count
#define		FDEFDWN		5			//default feather down count
#define		FDEFFLR		9			//default feather floor
#define		FDEFNUM		10			//default feather number
#define		FDEFTYP		FTHPSG		//default feather type
#define		ITXHI		9*PFGRAN	//default texture editor height
#define		ITXWID		9*PFGRAN	//default texture editor width
#define		ITXSPAC		0.40*PFGRAN	//default texture editor spacing
#define		ITXPIX		5			//default texture editor cross pixels

enum {
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
	STR_TRIAL,
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
	STR_FTHDWN,
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
	STR_LEN,	//must be last entry in enum
};

//edge tracing directions
enum {
	TRCU,			//top edge
	TRCR,			//right edge
	TRCD,			//bottom edge
	TRCL			//left edge
};

enum {
	SETCUST = 1,	//set the custom hoop
	SMALHUP = 2,	//pfaf code for small hoop
	LARGHUP,		//pfaf code for large hoop
	HUP100,			//100 millimeter hoop
	CUSTHUP			//user defined hoop size
};

enum	{	//daisy form types
	DSIN,
	DRAMP,
	DSAW,
	DRAG,
	DCOG,
	DHART
};

//bitmap
enum {
	SATIN,		//user is entering a satin stitch form
	SATPNT,		//user is entering points for a satin stitch form
	BOXZUM,		//box zoom select mode
	BZUMIN,		//display the zoom box
	BZUM,		//zoom box displayed on screen
	INSRT,		//insert mode active
	SELPNT,		//user has clicked on a selected point
	SATCNKT,	//user is connecting points in a satin form
	INSFRM,		//user is inserting points in a form
	FORMIN,		//user is selecting a form
	PRFACT,		//user is selecting preferences
	POLIMOV,	//user is placing a pre defined form
	SIZSEL,		//thread size select boxes displayed
	THRDS,		//State of the threads/lines box
	LINED,		//line has been started
	LININ,		//line is in the zoom rectangle
	ZUMED,		//stitch window is zoomed
	COL,		//state of the color/all box
	HID,		//state of the show/hide button
	ISDWN,		//down stitch exists
	ISUP,		//up stitch exists
	WASLIN,		//a move stitch line has been drawn
	CAPT,		//screen capture in effect
	VCAPT,		//keep screen capture as long as the mouse cursor
				// is in the edit window
	SELBOX,		//the select box is showing
	ILIN,		//insert line is showing on the screen
	INIT,		//set when a stitch is entered in the stitch buffer,a form is entered, or the mark is set
	LIN1,		//drawing only one line in stitch insert mode
	ILIN1,		//one insert line showing on screen
	GRPSEL,		//group select mode
	SCROS,		//start group cross has been drawn
	ECROS,		//end group cross has been drawn
	ELIN,		//group select line has been drawn
	BAKEND,		//insert data should be appended to the end of the file
	CLPSHO,		//display the clipboard insert rectangle
	SELSHO,		//display the select rectangle
	ROTAT,		//rotate state
	ROTSHO,		//rotate rectangle is showing
	ROTUSHO,	//rotate indicator line is showing
	ROTCAPT,	//rotate capture mode
	MOVCNTR,	//user is moving the rotate center
	GETMIN,		//user is entering a minimum stitch length
	NUMIN,		//user in entering a number
	MOVFRM,		//user is moving a form point
	MOVMSG,		//user tried to move an edited form
	SHOFRM,		//form is currently written to the screen
	STHR,		//user is entering a new stitch box threshold
	ENTAR,		//user is adjusting a number with the up and down arrow keys
	ENTR30,		//user is entering a new thread size for #30 thread
	ENTR40,		//user is entering a new thread size for #40 thread
	ENTR60,		//user is entering a new thread size for #60 thread
	FORMSEL,	//a form is selected
	FRMPMOV,	//user is moving a point on a form
	IGNOR,		//user may elect to ignore a message about losing edits when resizing a form
	FRMOV,		//user is moving a form
	RUNPAT,		//user is runing a pattern
	WASPAT,		//user ran a pattern, but hasn't done anything else yet
	FILDIR,		//direction lines in angle fill
	SHOSAT,		//the satin stitch form is on the screen
	SHOCON,		//connect point line is visible
	FENDIN,		//user is selecting fill ends
	DELFRM,		//user wants to delete a form
	BAKACT,		//there are entries in the undo buffer
	BAKWRAP,	//undo buffer pointer has wrapped
	BAKING,		//user has called the undo function
	REDUSHO,	//redo menu item is active
	UNDUSHO,	//undo menu item is active
	FUNCLP,		//user is loading a form from the clipboard
	FRMPSEL,	//user has selected a point in a form
	SHOINSF,	//the form insert line has been drawn
	SAT1,		//set when the first stitch is entered in a satin fill
	FILMSG,		//set when user tries to unfil a form with edited stitches
	CLPBAK,		//set when the first clipboard fill item has been entered
	STRTCH,		//user is stretching a form horizontally or vertically
	SHOSTRTCH,	//stretch box is written to the screen
	EXPAND,		//user is expanding a form horizontally and vertically
	SIDACT,		//side message window is active
	BRDACT,		//border data in side message window
	FILTYP,		//0=main fill, 1=border fill, used in side message display
	GMRK,		//draw the mark on the screen
	SIDCOL,		//user is selecting a fill color for a form
	BRDSID,		//user is selecting a border fill color for a form
	ENTRPOL,	//user is entering sides for a regular polygon
	FRMROT,		//user is rotating a form
	DELTO,		//user is deleting a form and the associated stitches
	REDOLD,		//user is opening a file from the old file list
	BAKSHO,		//user is looking at bakup files
	DUBAK,		//user has chosen a backup file to load, but may want to save changes
	IGNAM,		//save the thred file without making backups
	PRGMSG,		//delete all backups message is displayed
	NEWBAK,		//user is starting a new file, but may want to save changes
	SAVEX,		//user is exiting, may want to save changes
	OSAV,		//user wants to load a new file, may want to save changes
	FRMOF,		//forms are not displayed and can't be selected
	SHOMOV,		//user moving form line shown on screen
	BARSAT,		//doing a bare satin fill to be filled in with clipboard data
	ENTRSTAR,	//user in entering the number of points for a star
	ENTROT,		//user is entering a rotate number for command rotate;
	ENTRDUP,	//user is entering a rotate angle for rotate and duplicate
	ENTRSPIR,	//user is entering the number of points for a spiral
	ENTRHART,	//user is entering the number of points for a heart
	ENTRLENS,	//user is entering the number of points for a lens
	SEQDUN,		//last stitch seqenced was already done
	WASEL,		//a form was selected when the user hit control-right-click
	MOVFRMS,	//user is moving a group of forms
	FRMSROT,	//user is rotating group of forms
	FUNSCLP,	//user is pasting a group of forms
	DELSFRMS,	//user is deleting a group of forms
	BIGBOX,		//user has selected all forms and stitches
	MOVSET,		//the move point has been selected
	UPTO,		//show points upto the selected point
	LENSRCH,	//user has hit the max or min button
	BOXSLCT,	//user is making a select box
	THUMSHO,	//thumbnail mode
	INSFIL,		//user is inserting a file
	IGNORINS,	//don't use the open file dialog box to get the insert file name
	RBUT,		//user pressed the right mouse button selecting a thumbnail 
	UND,		//stitching a perp satin underlay
	UNDPHAS,	//perp satin underlay phase
	SAVAS,		//user is saving a file under a new name
	SHOSIZ,		//design size box is displayed
	HUPMSG,		//user is selecting a hoop size
	IGNTHR,		//ignore the close enough threshold for 'C' hotkey
	MONOMAP,	//set if a color bitmap is loaded
	THUMON,		//user is loading a thumnail
	CLPOVR,		//can't fit another clipboard fill on the current line
	CONTIG,		//contiguous point flag
	NUROT,		//user is entering a rotate angle
	APSID,		//user is enter a new applique color
	FRMSAM,		//don't find an already selected form
	ENTRSEG,	//user is entering rotation segments
	ENTRFNUM,	//user is entering a form number
	WASFRMFRM,	//need to restore the form-form
	SAVACT,		//activate the undo save
	CNTRH,		//for centering horizontally
	CNTRV,		//for centering vertically
	RESTCH,		//redraw the stitch window
	WASGRP,		//group of stitches selecter when entering length search
	REFILMSG,	//refill all message is up
	PRELIN,		//user is inserting form points before the 0 point in a line form
	WASRT,		//insert was active when preferences activated
	WASDIR,		//last minimum direction for sequencing
	BRKFIX,		//last line sequenced was from the end of a break of an already done region
	RELAYR,		//need to write the layer in the layer button
	HIDSTCH,	//don't display the stitches
	WASIN,		//placing stitches inside the form for vertical clipboard fill
	LTRACE,		//line trace funtion active
	HUPEX,		//set when points are outside the hoop
	HUPCHNG,	//hoop size has been changed
	SIZED,		//window has been resized
	ROTAL,		//user is rotating all stitches
	ZUMACT,		//zoom to actual size
	WASREFIL,	//last fill was a refil
	DUMEN,		//menu needs to be refilled
	WASNEG,		//form had a point less than the size of the clipboard fill
	FCLOS,		//user is closing a file
	NOTFREE,	//no free space on the drive
	PIXIN,		//user in inputting nudge pixels
	STPXIN,		//user is inputting stitch point pixels
	FRMPXIN,	//user is inputting form point pixels
	WASMRK,		//user has set a mark
	RESIZ,		//need to change the size of the main window
	MSGOF,		//offset the message box
	WASMOV,		//mouse move capture flag
	WASCOL,		//mouse move started in a color box
	ENTREG,		//user is entering an egg form
	BADFIL,		//loaded file is corrupted
	WASTRAC,	//traace bitmap loaded
	LANDSCAP,	//trace bitmap is landscape position
	WASEDG,		//trace edge map was created
	TRCUP,		//trace the up limit
	TRCRED,		//red trace on
	TRCGRN,		//green trace on
	TRCBLU,		//blue trace on
	DUHI,		//paint the high trace rectangle
	DULO,		//paint the low trace rectangle
	WASDIF,		//found edges on bitmap
	WASDSEL,	//color selected bitmap
	TRNIN0,		//trace color number input
	TRNUP,		//trace up number input
	WASTRCOL,	//trace color entered from keyboard
	TRNIN1,		//trace parameter number input
	TRSET,		//trace numbers have been set
	WASESC,		//user hit escape
	HIDMAP,		//hide the bitmap
	WASBLAK,	//bitmap form pixels set to black
	LINCHN,		//chain stich has line in middle
	ENTRZIG,	//user is creating a zig-zag form
	FOLFRM,		//form-form is displaying a follower form
	DUREFRM,	//display the form-form
	SIDX,		//display the extra fills
	WASLED,		//lead form found in frmchk
	FPSEL,		//form points selected
	SHOPSEL,	//form points selected rectangle is showing
	PSELDIR,	//direction of form point select
	FPUNCLP,	//user is retrieving cliped form points into a form
	SHOP,		//clipboard from points are showing
	WASPCDCLP,	//there is pcd data on the clipboard
	FTHR,		//feather fill flag
	FBOTH,		//do both sides of a feather fill
	ENTRFTHUP,	//user is entering the feather up number
	FTHSID,		//user is entering a feather color
	CNV2FTH,	//converting a form to feather ribbon
	WASFPNT,	//user is moving a form point
	WASDO,		//backup has been made before refil
	NOSEL,		//left button down didn't select anything
	WASDEL,		//stitches were deleted
	FLPBLND,	//flip the feather blend bit before refilling
	GTWLKIND,	//get the edge walk/underlay offset
	GTWLKLEN,	//get the edge walk/underlay stitch length
	GTUANG,		//get the underlay angle
	GTUSPAC,	//get the underaly spacing
	BADVER,		//key version does not match
	DUSRT,		//write sort info
	WLKDIR,		//edge walk direction, 0=forward
	INDIR,		//inner border switched with outer
	DIDSTRT,	//start stitches have been written
	REFCNT,		//counting the form-form lines
	UNDCOL,		//user is entrering an underlay color
	FSETULEN,	//user is setting the underlay stitch length for a group of forms
	ISEND,		//writing the end stitches
	FSETUSPAC,	//user is setting the underlay spacing for a group of forms
	FSETUANG,	//user is setting the underlay angle for a group of forms
	FSETFLEN,	//user is setting the fill stitch length for a group of forms
	FSETFSPAC,	//user is setting the fill spacing for a group of forms
	FSETFANG,	//user is setting the fill angle for a group of forms
	FSETUCOL,	//user is setting the underlay color for a group of forms
	FSETFCOL,	//user is setting the fill color for a group of forms
	FSETBCOL,	//user is setting the border color for a group of forms
	FSETBLEN,	//user is setting the border color for a group of forms
	FSETBSPAC,	//user is setting the border spacing for a group of forms
	FSETBMIN,	//user is setting the minimum border stitch length for a group of forms
	FSETBMAX,	//user is setting the maximum border stitch length for a group of forms
	FSETFMIN,	//user is setting the minimum fill stitch length for a group of forms
	FSETFMAX,	//user is setting the maximum fill stitch length for a group of forms
	FSETFWID,	//user is setting the width for a group of forms
	FSETFHI,	//user is setting the height for a group of forms
	TXTRED,		//displaying the texture fill editor
	FRMBOXIN,	//user is setting the form box pixels
	TXTMOV,		//user is moving a texture editor point
	TXTCLP,		//user is setting texture points with a clipboard form
	TXTLIN,		//user is setting texture points in a line
	WASTXBAK,	//texture fill editor history active
	LASTXBAK,	//last key was a texture back key
	TXBDIR,		//texture backup direction
	TXHCNTR,	//texture center on
	TXFIL,		//texture fill in progress
	WASWROT,	//texture clipboard form was written
	DESCHG,		//design size change 0=x, 1=y 
	CMPDO,		//design has been changed
	ISUND,		//doing underlay fill
	CHKTX,		//user has changed the texture fill window size, check the points
	FSETFIND,	//user is setting the indent for a group of forms
	TXBOX,		//user is importing stitches for textured fill
	TXIN,		//last stitch point was in stitch select box
	SCLPSPAC,	//user is setting the clipbard fill spacing
	FCHK,		//check the forms
	NOCLP,		//don't load clipboard data from forms
	NOTHRFIL,	//loaded file wasn't a Thred file

	LASTBIT		//must be the last entry in the enum
};

#define	MAPLEN (LASTBIT>>5)+1 //length of the state bitmap in 32 bit words

#define BZUMINB 1<<BZUMIN
#define BOXZUMB 1<<BOXZUM
#define INSRTB 1<<INSRT
#define SELPNTB 1<<SELPNT
#define SATINB 1<<SATIN
#define SATPNTB 1<<SATPNT
#define SATCNKTB 1<<SATCNKT
#define INSFRMB 1<<INSFRM
#define SELBOXM 1<<SELBOX
#define FORMINB 1<<FORMIN
#define PRFACTB 1<<PRFACT
#define	POLIMOVB 1<<POLIMOV

//user bitmap
enum {
	SQRFIL,		//square ends on fills
	BLUNT,		//blunt ends on satin lines
	NEDOF,		//needle cursor off
	KNOTOF,		//don't show knots
	BSAVOF,		//don't save PCS bitmaps
	LINSPAC,	//evenly space border line points
	DUND,		//underlay in satin borders
	FIL2OF,		//fill to select off
	ROTAUX,		//rotate the aux file when saving
	SAVMAX,		//thred window maximized
	MARQ,		//remove mark when user hits escape only
	FRMX,		//cross cursor for form entry
	DAZHOL,		//daisy hole
	DAZD,		//daisy d-lines
	CHREF,		//refill forms when changing design size
	WRNOF		//warn if edited off
};

enum {			//feather fill types
	FTHSIN = 1,	//sine
	FTHSIN2,	//half sine
	FTHLIN,		//line
	FTHPSG,		//psuedo-random sequence
	FTHRMP,		//sawtooth
	FTHFAZ		//phase
};

#pragma pack(1)

//pcs file header structure
typedef struct _pcsheader {
	TCHAR			leadIn;
	TCHAR			hoopType;
	unsigned short	colorCount;
	COLORREF		colors[16];
	unsigned short	stitchCount;
}PCSHEADER;

//ini file structure
typedef struct _iniFil {
	TCHAR			defaultDirectory[180];	//default directory
	COLORREF		stitchColors[16];		//colors
	COLORREF		stitchPreferredColors[16];	//stich preference colors
	COLORREF		backgroundPreferredColors[16];	//background preference colors
	COLORREF		backgroundColor;		//background color
	COLORREF		bitmapColor;			//bitmap color
	double			minStitchLength;		//minimum stitch length
	double			showStitchThreshold;	//show stitch points 
	double			threadSize30;			//millimeter size of 30 weight thread
	double			threadSize40;			//millimeter size of 40 weight thread
	double			threadSize60;			//millimeter size of 60 weight thread
	double			userStitchLength;		//user stitch length
	double			maxStitchLength;		//maximum stitch length
	double			smallStitchLength;		//small stitch size
	double			stitchBoxesThreshold;	//show sitch box level
	double			stitchSpace;			//stitch spacing between lines of stitches
	double			fillAngle;				//fill angle
	unsigned		binaryVariableBitmap;	//bitmap
	double			borderWidth;			//border width
	unsigned		underlayColor;			//applique color
	TCHAR			prevNames[OLDNUM][_MAX_PATH];	//last file names
	double			snapLength;				//snap together length
	double			starRatio;				//star ratio
	double			spiralWrap;				//spiral wrap
	COLORREF		bitmapBackgroundColors[16];	//bitmap background color preferences
	double			buttonholeFillCornerLength;	//buttonhole fill corner length		
	float			picotSpace;				//space between border picots
	TCHAR			hoopType;				//hoop type
	TCHAR			auxFileType;			//machine file type
	float			hoopSizeX;				//hoop x size
	float			hoopSizeY;				//hoop y size
	double			rotationAngle;			//rotation angle
	float			gridSize;				//grid size
	float			clipboardOffset;		//clipboard offset
	RECT			initialWindowCoords;	//initial window coordinates
	COLORREF		gridColor;				//grid color
	unsigned		fillPhase;				//clipboard fill phase
	float			customHoopX;			//custom hoop width
	float			customHoopY;			//custom hoop height
	float			traceLength;			//lens points
	double			traceRatio;				//trace ratio
	float			chainSpace;				//chain space
	float			chainRatio;				//chain ratio
	float			cursorNudgeStep;		//cursor nudge step
	unsigned short  nudgePixels;			//nudge pixels
	float			eggRatio;				//egg ratio
	unsigned short	stitchSizePixels;		//size of stitch points in pixels
	unsigned short	formPointSizePixels;	//size of form points in pixels
	unsigned short	formSides;				//sides of a created form
	float			tearTailLength;			//length of the tear tail
	float			tearTwistStep;			//tear twist step
	float			tearTwistRatio;			//tear twist ratio
	unsigned short	wavePoints;				//wave points
	unsigned short	waveStart;				//wave strting point
	unsigned short	waveEnd;				//wave ending point;
	unsigned short	waveLobes;				//wave lobes
	unsigned char	featherFillType;		//feather fill type
	unsigned char	featherUpCount;			//feather up count
	unsigned char	featherDownCount;		//feather down count
	unsigned char	featherType;			//feather bits
	float			featherRatio;			//feather ratio
	float			featherMinStitchSize;	//feather floor
	unsigned short	featherCount;			//feather fill psg granularity
	TCHAR			p2cName[_MAX_PATH];		//pes2card file
	float			underlayIndent;			//edge walk/underlay indent
	float			underlayAngle;			//underlay angle
	float			underlaySpacing;		//underlay spacing
	float			underlayStitchLen;		//underlay stitch length
	float			daisyDiameter;			//daisy diameter
	float			daisyPetalLen;			//daisy petal length
	float			daisyHoleDiameter;		//daisy hole diameter
	unsigned		daisyPetalCount;		//daisy petals
	unsigned		daisyPetalPoints;		//daisy petal points
	unsigned		daisyInnerCount;		//daisy inner count
	unsigned char	daisyBorderType;		//daisy border type
	unsigned char	dataCheck;				//data check
	float			textureHeight;			//textured fill height
	float			textureWidth;			//textured fill width
	float			textureSpacing;			//textured fill spacing
	unsigned short	formBoxSizePixels;		//form box pixels
	unsigned short	daisyHeartCount;		//daisy heart count
	unsigned short  textureEditorSizePixels;	//texture editor pixels
	float			clpspc;					//clipboard fill spacing
	TCHAR			designerName[50];		//designer name
}INIFILE;

enum {

	AUXPCS,
	AUXDST,
	AUXPES
};

typedef struct _formPoints {

	unsigned start;
	unsigned pointCount;
	unsigned finish;
	unsigned form;
}FORMPOINTS;

typedef struct _doublePoint {

	double x;
	double y;

}dPOINT;

typedef struct _doublePointLine {

	double			x;
	double			y;
	unsigned short line;

}dPOINTLINE;

typedef struct _doublerectangle {

	double top;
	double bottom;
	double left;
	double right;

}dRECTANGLE;

typedef struct _colChng {

	unsigned short stitchIndex;	//stich index
	unsigned short colorIndex;	//color index
}COLCHNG;

typedef struct _fPointAttribute {

	float		x;
	float		y;
	unsigned	attribute;
}fPOINTATTRIBUTE;

#define COLMSK		0x0000000f
#define NCOLMSK		0xfffffff0
#define	COLSMSK		0x0000ffff
#define FRMSK		0x00003ff0
#define NFRMSK		0xffffc00f
#define UFRMSK		0x80003ff0
#define FRMSHFT		4
#define TYPFMSK		0x20003ff0
#define TYPBMSK		0x40003ff0
#define TYPAPMSK	0x60003ff0
#define	LAYMSK		0x0e000000
#define NLAYMSK		0xf1ffffff
#define LAYSHFT		25
#define TYPMSK		0x60000000
#define TYPFRM		0x20000000
#define TYPBRD		0x40000000
#define NTYPMSK		0x9fffffff
#define TYPSHFT		29
#define USMSK		0x80000000
#define USHFT		31
#define	ATMSK		0x7fffffff
#define NFRM_NTYP	0x9fffc00f
#define TYPATMSK	0x20000000
#define	LASTMSK		0x20003ff0
#define	WLKMSK		0x00200000
#define WLKFMSK		0x00203ff0
#define	CWLKMSK		0x00100000
#define CWLKFMSK	0x00103ff0
#define	UNDMSK		0x00400000
#define	WUNDMSK		0x00600000
#define UNDFMSK		0x00403ff0
#define KNOTMSK		0x00800000
#define NKNOTMSK	0xff7fffff
#define FTHMSK		0x01000000
#define DELMSK		0x61e03ff0
#define ALTYPMSK	0x61f00000
#define	SRTMSK		0x61f03fff
#define SRTYPMSK	0x61700000
#define NOTFRM		0x00080000

/*
bit definitions for fPOINTATTRIBUTE.attribute
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

#define	FRMFIL		0x20000000
#define FRMBFIL		0x40000000
#define FRMAPFIL	0x60000000

typedef struct _floatPoint {

	float x;
	float y;
}fPOINT;

typedef struct _floatRectangle {

	float top;
	float left;
	float right;
	float bottom;
}fRECTANGLE;

typedef struct _forminfo {

	unsigned	type;
	unsigned	attribute;
	unsigned	sideCount;
}FORMINFO;

typedef struct _satcon {

	unsigned short start;
	unsigned short finish;
}SATCON;

typedef union _fangclp {

	float		angle;
	fPOINT*		clip;
	SATCON		sat;
}FANGCLP;

typedef union _flencnt {

	float		stitchLength;
	unsigned	clipCount;
}FLENCNT;

typedef union _satinOrAngle {

	SATCON*	sac;
	float	angle;
}SATINANGLE;

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

#define	BELEN		1
#define BESPAC		2
#define	BESIZ		4
#define	BNCLP		8
#define	BPICSPAC	16
#define	BCNRSIZ		32
#define	BRDEND		64
#define BRDPOS		128
#define BEMAX		256
#define BEMIN		512
#define CHNPOS		1024

#define MEGLIN		BELEN|BEMAX|BEMIN
#define MEGBLD		BELEN|BEMAX|BEMIN
#define MEGCLP		BNCLP|BEMAX|BEMIN
#define MEGSAT		BESPAC|BESIZ|BRDEND|BEMAX|BEMIN
#define MEGAP		BESPAC|BESIZ|BRDEND|BEMAX|BEMIN
#define MEGPRP		BESPAC|BESIZ|BRDEND|BEMAX|BEMIN
#define MEGHOL		BELEN|BESPAC|BESIZ|BCNRSIZ|BEMAX|BEMIN
#define MEGPIC		BELEN|BESIZ|BNCLP|BPICSPAC|BCNRSIZ|BEMAX|BEMIN
#define MEGDUB		BELEN|BEMAX|BEMIN
#define MEGCHNL		BESIZ|BESPAC|BEMAX|BEMIN|CHNPOS
#define MEGCHNH		BESIZ|BESPAC|BEMAX|BEMIN|CHNPOS
#define MEGCLPX		BNCLP|BEMAX|BEMIN

#define	EGLIN_LINS	4
#define EGBLD_LINS	4
#define EGCLP_LINS	3
#define EGSAT_LINS	6
#define EGAP_LINS	7
#define EGPRP_LINS	6
#define EGHOL_LINS	7
#define EGPIC_LINS	7
#define EGCHN_LINS	6

// ToDo - Is this structure valid and should it be used?
typedef struct _frmhedo {

	unsigned char	attribute;		//attribute
	unsigned short	sides;			//number of sides
	unsigned char	type;			//type
	unsigned char	fillColor;		//fill color
	unsigned char	borderColor;	//border color
	unsigned short	clipEntries;	//number of border clipboard entries
	fPOINT*			vertices;		//points
	SATINANGLE		satinOrAngle;	//satin guidlines or angle clipboard fill angle
	fPOINT*			borderClipData;	//border clipboard data
	unsigned short	satinGuideCount;	//number of satin guidlines
	unsigned short	wordParam;		//word parameter
	fRECTANGLE		rectangle;		//rectangle
	unsigned char	fillType;		//fill type
	unsigned char	edgeType;		//edge type
	float			fillSpacing;	//fill spacing
	FLENCNT			lengthOrCount;	//fill stitch length or clipboard count
	FANGCLP			angleOrClipData;	//fill angle or clipboard data pointer
	float			borderSize;		//border size
	float			edgeSpacing;	//edge spacing
	float			edgeStitchLen;	//edge stitch length
	unsigned short	res;			//pico length
}FRMHEDO;

#define	FRMEND		1
#define FRMLMSK		0x0e
#define NFRMLMSK	0xf1

/*form attribute bits

  1		end teflon line
  2-4	layer bits
  5		finish blunt
  6		start blunt
  7		refill bit for contour fill
*/

//blunt bits 
#define FBLNT		0x20
#define SBLNT		0x40
#define	NFBLNT		0xdf
#define NSBLNT		0xbf
#define NOBLNT		0x9f

//contour refil
#define FRECONT		0x80
#define NFRECONT	0x7f

typedef struct _featherInfo {
	unsigned char	fillType;	//feather fill type
	unsigned char	upCount;	//feather up count
	unsigned char	downCount;	//feather down count
	unsigned char	color;		//feather blend color
	float			ratio;		//feather ratio
	float			minStitchSize;	//feather floor
	unsigned short	count;		//feather fill psg granularity
}FTHINFO;

typedef struct _textureInfo {
	short			lines;
	unsigned short	index;
	unsigned short	count;
	float			height;
}TXTRINFO;

typedef union _tfhed
{
	FTHINFO		feather;
	TXTRINFO	texture;
}TFINFO;

typedef struct _frmhed {

	unsigned char	attribute;			//attribute
	unsigned short	sides;				//number of sides
	unsigned char	type;				//type
	unsigned char	fillColor;			//fill color
	unsigned char	borderColor;		//border color
	unsigned short	clipEntries;		//number of border clipboard entries
	fPOINT*			vertices;			//points
	SATINANGLE		satinOrAngle;		//satin guidlines or angle clipboard fill angle
	fPOINT*			borderClipData;		//border clipboard data
	unsigned short	satinGuideCount;	//number of satin guidlines
	unsigned short	wordParam;			//clipboard/textured fill phase or satin connect end
	fRECTANGLE		rectangle;			//rectangle
	unsigned char	fillType;			//fill type
	unsigned char	edgeType;			//edge type
	float			fillSpacing;		//fill spacing
	FLENCNT			lengthOrCount;		//fill stitch length or clipboard count
	FANGCLP			angleOrClipData;	//fill angle or clipboard data pointer
	float			borderSize;			//border size
	float			edgeSpacing;		//edge spacing
	float			edgeStitchLen;		//edge stitch length
	unsigned short	picoLength;			//pico length

	unsigned		extendedAttribute;	//attribute extension
	float			maxFillStitchLen;	//maximum fill stitch length
	float			minFillStitchLen;	//minimum fill stitch length
	float			maxBorderStitchLen;	//maximum border stitch length
	float			minBorderStitchLen;	//minimum border stitch length
	TFINFO			fillInfo;			//feather/texture info
	unsigned short	fillStart;			//fill start point
	unsigned short	fillEnd;			//fill end point
	float			underlaySpacing;	//underlay spacing
	float			underlayStitchLen;	//underlay stitch length
	float			underlayStitchAngle;	//underlay stitch angle
	float			underlayIndent;		//underlay/edge walk indent
	float			txof;				//gradient end density
	unsigned char	underlayColor;		//underlay color
	unsigned char	cres;				//reserved
}FRMHED;

//frmhed extended attribute bits

#define AT_SQR 1		//square ends
#define AT_FTHUP 2		//feather up flag
#define AT_FTHDWN 4		//feather down flag
#define AT_FTHBLND 8	//feather blend flag
#define AT_STRT 16		//user set start flag
#define AT_END 32		//user set end flag
#define AT_UND 64		//underlay flag
#define AT_WALK 128		//edge walk
#define AT_CWLK 256		//center walk

typedef struct _frmclp {
	unsigned	clipType;
	unsigned	reserved;
	FRMHED		form;
}FORMCLIP; //form data clipboard header

typedef struct _frmsclp {
	unsigned		clipType;
	unsigned short	formCount;
	unsigned short	reserved;
}FORMSCLIP; //multiple forms clipboard header

typedef struct _fpclp {
	unsigned		clipType;
	unsigned short	pointCount;
	BOOL			direction;
}FORMPOINTCLIP; // form points clipboard header

typedef struct _strhed {		//thred file header
	unsigned		headerType;
	unsigned		fileLength;		//length of strhed + length of stitch data
	unsigned short	stitchCount;	//number of stitches
	unsigned short	hoopType;		//size of hup
	unsigned short	formCount;		//number of forms
	unsigned short	FormPointsLen;	//points to form points
	unsigned short	pointCount;		//number of form points
	unsigned short	dlineLen;		//points to dline data
	unsigned short	dlineCount;		//dline data count
	unsigned short	clipboardDataLen;	//points to clipboard data
	unsigned short	clipboardDataCount;	//clipboard data count
}STRHED;

typedef struct _txpnt {	//textured fill point
	float	y;
	short	line;
}TXPNT;

typedef struct _strex {				//thred v1.0 file header extension
	float		hoopSizeX;			//hoop size x dimension
	float		hoopSizeY;			//hoop size y dimension
	float		stgran;				//stitches per millimeter
	TCHAR		creatorName[50];	//name of the file creator
	TCHAR		modifierName[50];	//name of last file modifier
	TCHAR		auxFormat;			//auxillary file format
	TCHAR		stres;				//reserved
	unsigned	texturePointCount;	//textured fill point count
	TCHAR		res[26];			//reserved for expansion
}STREX;

typedef struct _dsthed {		//dst file header
	TCHAR desched	[3];	// 0 0		description
	TCHAR desc		[17];	// 3 3
	TCHAR recshed	[3];	// 20 14	redord count
	TCHAR recs		[8];	// 23 17
	TCHAR cohed		[3];	// 31 1F
	TCHAR co		[4];	// 34 22
	TCHAR xplushed	[3];	// 38 26	x+ size
	TCHAR xplus		[6];	// 41 29
	TCHAR xminhed	[3];	// 47 2F	x- size
	TCHAR xmin		[6];	// 50 32
	TCHAR yplushed	[3];	// 56 38
	TCHAR yplus		[6];	// 59 3B	y+ size
	TCHAR yminhed	[3];	// 65 41
	TCHAR ymin		[6];	// 68 44	y- size
	TCHAR axhed		[3];	// 74 4A
	TCHAR ax		[7];	// 77 4D
	TCHAR ayhed		[3];	// 84 54
	TCHAR ay		[7];	// 87 57
	TCHAR mxhed		[3];	// 94 5E
	TCHAR mx		[7];	// 97 61
	TCHAR myhed		[3];	// 104 68
	TCHAR my		[7];	// 107 6B
	TCHAR pdhed		[2];	// 114 72
	TCHAR pd		[7];	// 116 74
	TCHAR eof		[1];	// 123 7B
	TCHAR res		[388];	// 124 7C
}DSTHED;

//dst type masks

#define JMPTYP 0x830000
#define COLTYP 0x630000
#define REGTYP 0x030000

typedef struct _dstrec {	//dst stitch record
	TCHAR	led;
	TCHAR	mid;
	TCHAR	nd;
}DSTREC;

typedef struct _smalpntl {
	unsigned short	line;	//line and group must remain in this order for sort to work
	unsigned short	group;
	float			x;
	float			y;
}SMALPNTL;

typedef struct _pcstch {
	unsigned char	fx;
	short			x;
	unsigned char	nx;
	unsigned char	fy;
	short			y;
	unsigned char	ny;
	unsigned char	typ;
}PCSTCH;

typedef struct _clpstch {
	unsigned short	led;
	unsigned char	fx;
	unsigned short	x;
	unsigned char	spcx;
	unsigned char	fy;
	unsigned short	y;
	unsigned char	spcy;
	unsigned char	myst;
	unsigned char	typ;
}CLPSTCH;

typedef struct _bakhed {
	unsigned	fcnt;
	FRMHED*		frmp;
	unsigned	scnt;
	fPOINTATTRIBUTE*	stch;
	unsigned	fltcnt;
	fPOINT*		flt;
	unsigned	sacnt;
	SATCON*		sac;
	unsigned	nclp;
	fPOINT*		clipData;
	COLORREF*	cols;
	TXPNT*		txp;
	unsigned	ntx;
	POINT		zoomRect;
}BAKHED;

typedef struct _flsiz {
	float	cx;
	float	cy;
}FLSIZ;

typedef struct _range {
	unsigned start;
	unsigned finish;
}RANGE;

typedef struct _region {	//region for sequencing vertical fills
	unsigned start;		//start line of region
	unsigned end;		//end line of region
	unsigned brk;
	unsigned cntbrk;
}REGION;

typedef struct _rcon {		//pathMap: path map for sequencing
	unsigned short	vrt;
	unsigned short	con;
	unsigned short	grpn;
}RCON;

typedef struct _rgseq {		//tmpath: temporary path connections
	unsigned	pcon;		//pointer to pathMap entry
	int			cnt;
	TCHAR		skp;		//path not found
}RGSEQ;

typedef struct _fseq {		//mpath: path of sequenced regions
	unsigned short	vrt;
	unsigned short	grpn;
	TCHAR			skp;	//path not found
}FSEQ;

#define SEQTOP 2
#define SEQBOT 3

typedef struct _bseqpnt {
	float			x;
	float			y;
	char			attribute;
}BSEQPNT;

typedef struct _pvec {
	double	angle;
	double	length;
}PVEC;

typedef struct _vrct2 {
	dPOINT	aipnt;
	dPOINT	aopnt;
	dPOINT	bipnt;
	dPOINT	bopnt;
	dPOINT	cipnt;
	dPOINT	copnt;
	dPOINT	dipnt;
	dPOINT	dopnt;
}VRCT2;

typedef struct _cursorMask {
	unsigned char form[128];
	unsigned char dline[128];
	unsigned char uprightNeedle[128];
	unsigned char leftUpNeedle[128];
	unsigned char leftDownNeedle[128];
	unsigned char rightUpNeedle[128];
	unsigned char rightDownNeedle[128];
}CURSORMASK;

//balarad file header
typedef struct _balhed {
	COLORREF		color[256];
	unsigned		signature;
	unsigned short	version;
	float			hoopSizeX;
	float			hoopSizeY;
	COLORREF		backgroundColor;
	unsigned char	res[1006];
}BALHED;

//balarad stitch
typedef struct _balstch {
	unsigned char	code;
	unsigned char	flag;
	float			x;
	float			y;
}BALSTCH;

typedef struct _clpseg {
	unsigned		start;
	float			borderLength;
	unsigned		bind;
	unsigned short	asid;
	unsigned		finish;
	float			edgeLength;
	unsigned		edgeIndex;
	unsigned short	zsid;
	TCHAR			dun;
}CLPSEG;

typedef struct _clipsort {
	float		seglen;
	float		sidlen;
	unsigned	lin;
	fPOINT		point;
}CLIPSORT;

typedef struct _clipnt {
	float			x;
	float			y;
	unsigned short	side;
	unsigned short	flag;
}CLIPNT;

typedef struct _vclpx {
	unsigned seg;
	unsigned sid;
}VCLPX;

#if PESACT

typedef struct _pesled {
	TCHAR			ver[8];
	unsigned		pec;
}PESLED;

typedef struct _peshed {
	TCHAR	led[8];
	TCHAR	off[3];
	TCHAR	m1[13];
	TCHAR	ce[6];
	TCHAR	m2[47];
	short	xsiz;
	short	ysiz;
	TCHAR	m3[16];
	TCHAR	cs[6];
	TCHAR	m4[3];
	TCHAR	scol;
	TCHAR	m5[3];
}PESHED;

typedef struct _pestch {
	short x;
	short y;
}PESTCH;

#endif

typedef struct _trcpnt {
	SHORT x;
	SHORT y;
}TRCPNT;

// ToDo - Is this struct needed?
typedef struct _txtmsg {
	unsigned	cod;
	TCHAR*		str;
}TXTMSG;

typedef struct _grdcod {
	unsigned id;
	unsigned col;
}GRDCOD;

#pragma pack()

//form types
enum {
	FRMLINE = 1u,	// Line form
	FRMFPOLY,		// Freehand polygon form
	FRMRPOLY,		// Regular polygon form
	FRMSTAR,		// Star form
	FRMSPIRAL,		// Spiral Form
	SAT,			// Used when fill is 'feather', 'fan', 'fan clip' or form is daisy
	FRMHEART,		// Heart Form
	FRMLENS,		// Lens form
	FRMEGG,			// Egg form
	FRMTEAR,		// Teardrop form
	FRMZIGZAG,		// Zig-Zag form
	FRMWAVE,		// Wave Form
	FRMDAISY,		// Daisy form
};

//main menu items
enum {
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
	M_FIL,
	M_ADD,
	M_FRM,
	M_ALL,
	M_1,
	M_2,
	M_3,
	M_4,
	M_HELP,
};

//fill menu items
enum {
	MFIL_SAT,
	MFIL_VERT,
	MFIL_HOR,
	MFIL_ANG,
	MFIL_CLP,
	MFIL_CONT,
	MFIL_BORD,
	MFIL_UNFIL
};

//clipboard data types
enum {
	CLP_STCH = 1,
	CLP_FRM,
	CLP_FRMS,
	CLP_FRMPS,
};

//edge fill types
enum {
	EDGELINE = 1,	// Line
	EDGEBEAN,		// Bean
	EDGECLIP,		// Clipboard
	EDGEANGSAT,		// Angle Satin
	EDGEAPPL,		// Applique
	EDGEPROPSAT,	// Proportional Satin
	EDGEBHOL,		// Buttonhole
	EDGEPICOT,		// Picot
	EDGEDOUBLE,		// Double
	EDGELCHAIN,		// Line Chain
	EDGEOCHAIN,		// Open Chain
	EDGECLIPX,		// Even Clipboard
	EDGELAST		//must always be the last value
};

//edge underlay bit
#define EGUND 0x80
#define NEGUND 0x7f

//form data lines
enum {
	LFRM,			//form 0
	LLAYR,			//layer 1
	LFRMFIL,		//form fill 2	
	LFRMCOL,		//form fill color 3
	LFRMSPAC,		//form fill space 4
	LFRMLEN,		//form stitch length 5
	LFRMANG,		//angle of angle fill 6
	LBRD,			//border 7
	LBRDCOL,		//border color 8
	LBRDSPAC,		//border space 9
	LBRDLEN,		//border stitch length 10
	LBRDSIZ,		//border size 11
	LAPCOL,			//applique color 12
	LBCSIZ,			//buttonhole corner size 13
	LBSTRT,			//form start style 14
	LBFIN,			//form end style 15
	LBRDPIC,		//pico border spacing 16
	LBRDUND,		//border underlay 17
	LSACANG,		//angle clipboard angle 18
	LFRMFAZ,		//clipboard phase 19
	LBRDPOS,		//chain position 20
	LBFILSQR,		//square/pointed fill ends 21
	LMAXFIL,		//maximum fill stitch length 22
	LMINFIL,		//minimum fill stitch length 23
	LMAXBRD,		//maximum border stitch length 24
	LMINBRD,		//minimum border stitch length 25
	LFTHCOL,		//feather color 26
	LFTHTYP,		//feather fill type
	LFTHBLND,		//feather blend
	LFTHDWN,		//feather down
	LFTHUP,			//feather up
	LFTHUPCNT,		//feather up count
	LFTHDWNCNT,		//feather down count
	LFTHSIZ,		//feather size
	LFTHNUM,		//feather number
	LFTHFLR,		//feather floor
	LFSTRT,			//form fill start on/off
	LDSTRT,			//form fill start data
	LFEND,			//form fill end on/off
	LDEND,			//form fill end data
	LCWLK,			//center walk
	LWALK,			//edge walk
	LWLKIND,		//edge walk/underlay indent
	LUND,			//underlay
	LULEN,			//underaly stitch length
	LUANG,			//underlay angle
	LUSPAC,			//underlay spacing
	LUNDCOL,		//underlay color
	LTXOF,			//texture fill spacing
	LASTLIN			//must be the last entry
};

//fill types
enum {
	VRTF = 1,
	HORF,
	ANGF,
	SATF,
	CLPF,
	CONTF,
	VCLPF,
	HCLPF,
	ANGCLPF,
	FTHF,
	TXVRTF,
	TXHORF,
	TXANGF,
};
#define MCLPF		1<<CLPF
#define MVCLPF		1<<VCLPF
#define MHCLPF		1<<HCLPF
#define MANGCLPF	1<<ANGCLPF

//preference window
enum {
	PSPAC,	//0
	PANGL,	//1
	PSQR,	//2
	PSAT,	//3
	PMAX,	//4
	PUSE,	//5
	PMIN,	//6
	PSHO,	//7
	PBOX,	//8
	PSMAL,	//9
	PAP,	//10
	PSNP,	//11
	PSTAR,	//12
	PSPIR,	//13
	PBUT,	//14
	PBLNT,	//15
	PPIC,	//16
	PHUP,	//17
	PHUPX,	//18
	PUND,	//19
	PGRD,	//20
	PCLPOF,	//21
	PFAZ,	//22
	PCHN,	//23
	PCHRAT,	//24
	PNUDG,	//25
	PEG,	//26
	PHUPY,	//27
};

//file menu items
enum {
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

//fill message codes
enum {
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

//button windows
enum {
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

enum	{ //text button windows
	HTXCLR,
	HTXHI,
	HTXWID,
	HTXSPAC,
	HTXVRT,
	HTXHOR,
	HTXANG,
	HTXMIR,
};

#define HLIN  HNUM

typedef struct _orec {
	unsigned 	start;
	unsigned 	finish;
	fPOINTATTRIBUTE*	spnt;
	fPOINTATTRIBUTE*	epnt;
	unsigned 	col;
	unsigned 	typ;
	unsigned 	frm;
	unsigned	otyp;
}OREC;

typedef struct _srtrec	{
	unsigned start;
	unsigned finish;
	unsigned cnt;
	unsigned loc;
	BOOL	 dir;
}SRTREC;

typedef struct _fstrts	{
	unsigned apl;
	unsigned fil;
	unsigned fth;
	unsigned brd;
	unsigned apcol;
	unsigned fcol;
	unsigned fthcol;
	unsigned ecol;
}FSTRTS;

#define M_AP	    2
#define M_CWLK	    4
#define M_WALK	    8
#define M_UND	   16
#define M_FIL	   32
#define M_FTH	   64
#define M_BRD	  128
#define M_APCOL   256
#define M_FCOL	  512
#define M_FTHCOL 1024
#define M_ECOL   2048

typedef struct _frmlim	{
	unsigned apstrt;
	unsigned apend;
	unsigned cstrt;
	unsigned cend;
	unsigned wstrt;
	unsigned wend;
	unsigned ustrt;
	unsigned uend;
	unsigned fstrt;
	unsigned fend;
	unsigned bstrt;
	unsigned bend;
}FRMLIM;

typedef struct _insrec	{
	unsigned cod;
	unsigned col;
	unsigned ind;
	unsigned seq;
}INSREC;

enum	{	//interleave sequence identifiers
	I_AP,
	I_FIL,
	I_FTH,
	I_BRD,
};

typedef struct _intinf {
	unsigned pins;
	unsigned coloc;
	unsigned layerIndex;
	unsigned sloc;
	unsigned oloc;
	fPOINTATTRIBUTE* histch;
}INTINF;

typedef struct _txtscr	{
	int		top;		//pixel top line
	int		bottom;		//pixel bottom line
	int		height;		//pixel height of area
	int		halfHeight;	//pixel middle of area
	float	xOffset;	//edit x offset of area
	float	yOffset;	//edit y offset of area
	int		index;		//index into temporay texture fill point table
	float	areaHeight;	//edit height of area
	float	screenHeight;	//edit height of screen
	float	width;		//edit width of area
	float	spacing;	//edit space between lines
	short	lines;		//number of lines
	double	editToPixelRatio;	//edit to pixel ratio
	fPOINT	formCenter;	//middle of the form
}TXTSCR;


typedef struct _txtrct	{
	short left;
	short right;
	float top;
	float bottom;
}TXTRCT;

typedef struct _txhst	{
	TXPNT*	texturePoint;
	int		count;
	float	height;
	float	width;
	float	spacing;
}TXHST;

typedef struct _rngcnt	{
	int	line;
	int	stitchCount;
}RNGCNT;

#define BADFLT	  1
#define BADCLP	  2
#define BADSAT	  4
#define BADTX	  8

typedef struct _badcnts	{
	unsigned attribute;
	int flt;
	int clip;
	int sat;
	int tx;
}BADCNTS;
