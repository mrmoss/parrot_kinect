/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the mingw-w64 runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#if (defined(SCHEMA_STRINGS)) || (! defined(TMSCHEMA_H))

#if (_WIN32_WINNT >= 0x0600 || defined __VSSYM32_H__)
#warning <tmschema.h> is obsolete, use <vssym32.h> instead.
#endif

#define TMSCHEMA_H

#include "schemadef.h"

#define THEMEMGR_VERSION 1

BEGIN_TM_SCHEMA(ThemeMgrSchema)

BEGIN_TM_ENUM(BGTYPE)
TM_ENUM(0,BT,IMAGEFILE)
TM_ENUM(1,BT,BORDERFILL)
TM_ENUM(2,BT,NONE)
END_TM_ENUM()

BEGIN_TM_ENUM(IMAGELAYOUT)
TM_ENUM(0,IL,VERTICAL)
TM_ENUM(1,IL,HORIZONTAL)
END_TM_ENUM()

BEGIN_TM_ENUM(BORDERTYPE)
TM_ENUM(0,BT,RECT)
TM_ENUM(1,BT,ROUNDRECT)
TM_ENUM(2,BT,ELLIPSE)
END_TM_ENUM()

BEGIN_TM_ENUM(FILLTYPE)
TM_ENUM(0,FT,SOLID)
TM_ENUM(1,FT,VERTGRADIENT)
TM_ENUM(2,FT,HORZGRADIENT)
TM_ENUM(3,FT,RADIALGRADIENT)
TM_ENUM(4,FT,TILEIMAGE)
END_TM_ENUM()

BEGIN_TM_ENUM(SIZINGTYPE)
TM_ENUM(0,ST,TRUESIZE)
TM_ENUM(1,ST,STRETCH)
TM_ENUM(2,ST,TILE)
END_TM_ENUM()

BEGIN_TM_ENUM(HALIGN)
TM_ENUM(0,HA,LEFT)
TM_ENUM(1,HA,CENTER)
TM_ENUM(2,HA,RIGHT)
END_TM_ENUM()

BEGIN_TM_ENUM(CONTENTALIGNMENT)
TM_ENUM(0,CA,LEFT)
TM_ENUM(1,CA,CENTER)
TM_ENUM(2,CA,RIGHT)
END_TM_ENUM()

BEGIN_TM_ENUM(VALIGN)
TM_ENUM(0,VA,TOP)
TM_ENUM(1,VA,CENTER)
TM_ENUM(2,VA,BOTTOM)
END_TM_ENUM()

BEGIN_TM_ENUM(OFFSETTYPE)
TM_ENUM(0,OT,TOPLEFT)
TM_ENUM(1,OT,TOPRIGHT)
TM_ENUM(2,OT,TOPMIDDLE)
TM_ENUM(3,OT,BOTTOMLEFT)
TM_ENUM(4,OT,BOTTOMRIGHT)
TM_ENUM(5,OT,BOTTOMMIDDLE)
TM_ENUM(6,OT,MIDDLELEFT)
TM_ENUM(7,OT,MIDDLERIGHT)
TM_ENUM(8,OT,LEFTOFCAPTION)
TM_ENUM(9,OT,RIGHTOFCAPTION)
TM_ENUM(10,OT,LEFTOFLASTBUTTON)
TM_ENUM(11,OT,RIGHTOFLASTBUTTON)
TM_ENUM(12,OT,ABOVELASTBUTTON)
TM_ENUM(13,OT,BELOWLASTBUTTON)
END_TM_ENUM()

BEGIN_TM_ENUM(ICONEFFECT)
TM_ENUM(0,ICE,NONE)
TM_ENUM(1,ICE,GLOW)
TM_ENUM(2,ICE,SHADOW)
TM_ENUM(3,ICE,PULSE)
TM_ENUM(4,ICE,ALPHA)
END_TM_ENUM()

BEGIN_TM_ENUM(TEXTSHADOWTYPE)
TM_ENUM(0,TST,NONE)
TM_ENUM(1,TST,SINGLE)
TM_ENUM(2,TST,CONTINUOUS)
END_TM_ENUM()

BEGIN_TM_ENUM(GLYPHTYPE)
TM_ENUM(0,GT,NONE)
TM_ENUM(1,GT,IMAGEGLYPH)
TM_ENUM(2,GT,FONTGLYPH)
END_TM_ENUM()

BEGIN_TM_ENUM(IMAGESELECTTYPE)
TM_ENUM(0,IST,NONE)
TM_ENUM(1,IST,SIZE)
TM_ENUM(2,IST,DPI)
END_TM_ENUM()

BEGIN_TM_ENUM(TRUESIZESCALINGTYPE)
TM_ENUM(0,TSST,NONE)
TM_ENUM(1,TSST,SIZE)
TM_ENUM(2,TSST,DPI)
END_TM_ENUM()

BEGIN_TM_ENUM(GLYPHFONTSIZINGTYPE)
TM_ENUM(0,GFST,NONE)
TM_ENUM(1,GFST,SIZE)
TM_ENUM(2,GFST,DPI)
END_TM_ENUM()

BEGIN_TM_PROPS()

TM_PROP(201,TMT,STRING,STRING)
TM_PROP(202,TMT,INT,INT)
TM_PROP(203,TMT,WINBOOL,WINBOOL)
TM_PROP(204,TMT,COLOR,COLOR)
TM_PROP(205,TMT,MARGINS,MARGINS)
TM_PROP(206,TMT,FILENAME,FILENAME)
TM_PROP(207,TMT,SIZE,SIZE)
TM_PROP(208,TMT,POSITION,POSITION)
TM_PROP(209,TMT,RECT,RECT)
TM_PROP(210,TMT,FONT,FONT)
TM_PROP(211,TMT,INTLIST,INTLIST)

TM_PROP(401,TMT,COLORSCHEMES,STRING)
TM_PROP(402,TMT,SIZES,STRING)
TM_PROP(403,TMT,CHARSET,INT)

#define TMT_FIRST_RCSTRING_NAME TMT_DISPLAYNAME
#define TMT_LAST_RCSTRING_NAME TMT_DESCRIPTION

TM_PROP(601,TMT,DISPLAYNAME,STRING)
TM_PROP(602,TMT,TOOLTIP,STRING)
TM_PROP(603,TMT,COMPANY,STRING)
TM_PROP(604,TMT,AUTHOR,STRING)
TM_PROP(605,TMT,COPYRIGHT,STRING)
TM_PROP(606,TMT,URL,STRING)
TM_PROP(607,TMT,VERSION,STRING)
TM_PROP(608,TMT,DESCRIPTION,STRING)

#define TMT_FIRSTFONT TMT_CAPTIONFONT
#define TMT_LASTFONT TMT_ICONTITLEFONT

TM_PROP(801,TMT,CAPTIONFONT,FONT)
TM_PROP(802,TMT,SMALLCAPTIONFONT,FONT)
TM_PROP(803,TMT,MENUFONT,FONT)
TM_PROP(804,TMT,STATUSFONT,FONT)
TM_PROP(805,TMT,MSGBOXFONT,FONT)
TM_PROP(806,TMT,ICONTITLEFONT,FONT)

#define TMT_FIRSTBOOL TMT_FLATMENUS
#define TMT_LASTBOOL TMT_FLATMENUS

TM_PROP(1001,TMT,FLATMENUS,WINBOOL)

#define TMT_FIRSTSIZE TMT_SIZINGBORDERWIDTH
#define TMT_LASTSIZE TMT_MENUBARHEIGHT

TM_PROP(1201,TMT,SIZINGBORDERWIDTH,SIZE)
TM_PROP(1202,TMT,SCROLLBARWIDTH,SIZE)
TM_PROP(1203,TMT,SCROLLBARHEIGHT,SIZE)
TM_PROP(1204,TMT,CAPTIONBARWIDTH,SIZE)
TM_PROP(1205,TMT,CAPTIONBARHEIGHT,SIZE)
TM_PROP(1206,TMT,SMCAPTIONBARWIDTH,SIZE)
TM_PROP(1207,TMT,SMCAPTIONBARHEIGHT,SIZE)
TM_PROP(1208,TMT,MENUBARWIDTH,SIZE)
TM_PROP(1209,TMT,MENUBARHEIGHT,SIZE)

#define TMT_FIRSTINT TMT_MINCOLORDEPTH
#define TMT_LASTINT TMT_MINCOLORDEPTH

TM_PROP(1301,TMT,MINCOLORDEPTH,INT)

#define TMT_FIRSTSTRING TMT_CSSNAME
#define TMT_LASTSTRING TMT_XMLNAME

TM_PROP(1401,TMT,CSSNAME,STRING)
TM_PROP(1402,TMT,XMLNAME,STRING)

#define TMT_FIRSTCOLOR TMT_SCROLLBAR
#define TMT_LASTCOLOR TMT_MENUBAR

TM_PROP(1601,TMT,SCROLLBAR,COLOR)
TM_PROP(1602,TMT,BACKGROUND,COLOR)
TM_PROP(1603,TMT,ACTIVECAPTION,COLOR)
TM_PROP(1604,TMT,INACTIVECAPTION,COLOR)
TM_PROP(1605,TMT,MENU,COLOR)
TM_PROP(1606,TMT,WINDOW,COLOR)
TM_PROP(1607,TMT,WINDOWFRAME,COLOR)
TM_PROP(1608,TMT,MENUTEXT,COLOR)
TM_PROP(1609,TMT,WINDOWTEXT,COLOR)
TM_PROP(1610,TMT,CAPTIONTEXT,COLOR)
TM_PROP(1611,TMT,ACTIVEBORDER,COLOR)
TM_PROP(1612,TMT,INACTIVEBORDER,COLOR)
TM_PROP(1613,TMT,APPWORKSPACE,COLOR)
TM_PROP(1614,TMT,HIGHLIGHT,COLOR)
TM_PROP(1615,TMT,HIGHLIGHTTEXT,COLOR)
TM_PROP(1616,TMT,BTNFACE,COLOR)
TM_PROP(1617,TMT,BTNSHADOW,COLOR)
TM_PROP(1618,TMT,GRAYTEXT,COLOR)
TM_PROP(1619,TMT,BTNTEXT,COLOR)
TM_PROP(1620,TMT,INACTIVECAPTIONTEXT,COLOR)
TM_PROP(1621,TMT,BTNHIGHLIGHT,COLOR)
TM_PROP(1622,TMT,DKSHADOW3D,COLOR)
TM_PROP(1623,TMT,LIGHT3D,COLOR)
TM_PROP(1624,TMT,INFOTEXT,COLOR)
TM_PROP(1625,TMT,INFOBK,COLOR)
TM_PROP(1626,TMT,BUTTONALTERNATEFACE,COLOR)
TM_PROP(1627,TMT,HOTTRACKING,COLOR)
TM_PROP(1628,TMT,GRADIENTACTIVECAPTION,COLOR)
TM_PROP(1629,TMT,GRADIENTINACTIVECAPTION,COLOR)
TM_PROP(1630,TMT,MENUHILIGHT,COLOR)
TM_PROP(1631,TMT,MENUBAR,COLOR)

TM_PROP(1801,TMT,FROMHUE1,INT)
TM_PROP(1802,TMT,FROMHUE2,INT)
TM_PROP(1803,TMT,FROMHUE3,INT)
TM_PROP(1804,TMT,FROMHUE4,INT)
TM_PROP(1805,TMT,FROMHUE5,INT)
TM_PROP(1806,TMT,TOHUE1,INT)
TM_PROP(1807,TMT,TOHUE2,INT)
TM_PROP(1808,TMT,TOHUE3,INT)
TM_PROP(1809,TMT,TOHUE4,INT)
TM_PROP(1810,TMT,TOHUE5,INT)

TM_PROP(2001,TMT,FROMCOLOR1,COLOR)
TM_PROP(2002,TMT,FROMCOLOR2,COLOR)
TM_PROP(2003,TMT,FROMCOLOR3,COLOR)
TM_PROP(2004,TMT,FROMCOLOR4,COLOR)
TM_PROP(2005,TMT,FROMCOLOR5,COLOR)
TM_PROP(2006,TMT,TOCOLOR1,COLOR)
TM_PROP(2007,TMT,TOCOLOR2,COLOR)
TM_PROP(2008,TMT,TOCOLOR3,COLOR)
TM_PROP(2009,TMT,TOCOLOR4,COLOR)
TM_PROP(2010,TMT,TOCOLOR5,COLOR)

TM_PROP(2201,TMT,TRANSPARENT,WINBOOL)
TM_PROP(2202,TMT,AUTOSIZE,WINBOOL)
TM_PROP(2203,TMT,BORDERONLY,WINBOOL)
TM_PROP(2204,TMT,COMPOSITED,WINBOOL)
TM_PROP(2205,TMT,BGFILL,WINBOOL)
TM_PROP(2206,TMT,GLYPHTRANSPARENT,WINBOOL)
TM_PROP(2207,TMT,GLYPHONLY,WINBOOL)
TM_PROP(2208,TMT,ALWAYSSHOWSIZINGBAR,WINBOOL)
TM_PROP(2209,TMT,MIRRORIMAGE,WINBOOL)
TM_PROP(2210,TMT,UNIFORMSIZING,WINBOOL)
TM_PROP(2211,TMT,INTEGRALSIZING,WINBOOL)
TM_PROP(2212,TMT,SOURCEGROW,WINBOOL)
TM_PROP(2213,TMT,SOURCESHRINK,WINBOOL)

TM_PROP(2401,TMT,IMAGECOUNT,INT)
TM_PROP(2402,TMT,ALPHALEVEL,INT)
TM_PROP(2403,TMT,BORDERSIZE,INT)
TM_PROP(2404,TMT,ROUNDCORNERWIDTH,INT)
TM_PROP(2405,TMT,ROUNDCORNERHEIGHT,INT)
TM_PROP(2406,TMT,GRADIENTRATIO1,INT)
TM_PROP(2407,TMT,GRADIENTRATIO2,INT)
TM_PROP(2408,TMT,GRADIENTRATIO3,INT)
TM_PROP(2409,TMT,GRADIENTRATIO4,INT)
TM_PROP(2410,TMT,GRADIENTRATIO5,INT)
TM_PROP(2411,TMT,PROGRESSCHUNKSIZE,INT)
TM_PROP(2412,TMT,PROGRESSSPACESIZE,INT)
TM_PROP(2413,TMT,SATURATION,INT)
TM_PROP(2414,TMT,TEXTBORDERSIZE,INT)
TM_PROP(2415,TMT,ALPHATHRESHOLD,INT)
TM_PROP(2416,TMT,WIDTH,SIZE)
TM_PROP(2417,TMT,HEIGHT,SIZE)
TM_PROP(2418,TMT,GLYPHINDEX,INT)
TM_PROP(2419,TMT,TRUESIZESTRETCHMARK,INT)
TM_PROP(2420,TMT,MINDPI1,INT)
TM_PROP(2421,TMT,MINDPI2,INT)
TM_PROP(2422,TMT,MINDPI3,INT)
TM_PROP(2423,TMT,MINDPI4,INT)
TM_PROP(2424,TMT,MINDPI5,INT)

TM_PROP(2601,TMT,GLYPHFONT,FONT)

TM_PROP(3001,TMT,IMAGEFILE,FILENAME)
TM_PROP(3002,TMT,IMAGEFILE1,FILENAME)
TM_PROP(3003,TMT,IMAGEFILE2,FILENAME)
TM_PROP(3004,TMT,IMAGEFILE3,FILENAME)
TM_PROP(3005,TMT,IMAGEFILE4,FILENAME)
TM_PROP(3006,TMT,IMAGEFILE5,FILENAME)
TM_PROP(3007,TMT,STOCKIMAGEFILE,FILENAME)
TM_PROP(3008,TMT,GLYPHIMAGEFILE,FILENAME)

TM_PROP(3201,TMT,TEXT,STRING)

TM_PROP(3401,TMT,OFFSET,POSITION)
TM_PROP(3402,TMT,TEXTSHADOWOFFSET,POSITION)
TM_PROP(3403,TMT,MINSIZE,POSITION)
TM_PROP(3404,TMT,MINSIZE1,POSITION)
TM_PROP(3405,TMT,MINSIZE2,POSITION)
TM_PROP(3406,TMT,MINSIZE3,POSITION)
TM_PROP(3407,TMT,MINSIZE4,POSITION)
TM_PROP(3408,TMT,MINSIZE5,POSITION)
TM_PROP(3409,TMT,NORMALSIZE,POSITION)

TM_PROP(3601,TMT,SIZINGMARGINS,MARGINS)
TM_PROP(3602,TMT,CONTENTMARGINS,MARGINS)
TM_PROP(3603,TMT,CAPTIONMARGINS,MARGINS)

TM_PROP(3801,TMT,BORDERCOLOR,COLOR)
TM_PROP(3802,TMT,FILLCOLOR,COLOR)
TM_PROP(3803,TMT,TEXTCOLOR,COLOR)
TM_PROP(3804,TMT,EDGELIGHTCOLOR,COLOR)
TM_PROP(3805,TMT,EDGEHIGHLIGHTCOLOR,COLOR)
TM_PROP(3806,TMT,EDGESHADOWCOLOR,COLOR)
TM_PROP(3807,TMT,EDGEDKSHADOWCOLOR,COLOR)
TM_PROP(3808,TMT,EDGEFILLCOLOR,COLOR)
TM_PROP(3809,TMT,TRANSPARENTCOLOR,COLOR)
TM_PROP(3810,TMT,GRADIENTCOLOR1,COLOR)
TM_PROP(3811,TMT,GRADIENTCOLOR2,COLOR)
TM_PROP(3812,TMT,GRADIENTCOLOR3,COLOR)
TM_PROP(3813,TMT,GRADIENTCOLOR4,COLOR)
TM_PROP(3814,TMT,GRADIENTCOLOR5,COLOR)
TM_PROP(3815,TMT,SHADOWCOLOR,COLOR)
TM_PROP(3816,TMT,GLOWCOLOR,COLOR)
TM_PROP(3817,TMT,TEXTBORDERCOLOR,COLOR)
TM_PROP(3818,TMT,TEXTSHADOWCOLOR,COLOR)
TM_PROP(3819,TMT,GLYPHTEXTCOLOR,COLOR)
TM_PROP(3820,TMT,GLYPHTRANSPARENTCOLOR,COLOR)
TM_PROP(3821,TMT,FILLCOLORHINT,COLOR)
TM_PROP(3822,TMT,BORDERCOLORHINT,COLOR)
TM_PROP(3823,TMT,ACCENTCOLORHINT,COLOR)

TM_PROP(4001,TMT,BGTYPE,ENUM)
TM_PROP(4002,TMT,BORDERTYPE,ENUM)
TM_PROP(4003,TMT,FILLTYPE,ENUM)
TM_PROP(4004,TMT,SIZINGTYPE,ENUM)
TM_PROP(4005,TMT,HALIGN,ENUM)
TM_PROP(4006,TMT,CONTENTALIGNMENT,ENUM)
TM_PROP(4007,TMT,VALIGN,ENUM)
TM_PROP(4008,TMT,OFFSETTYPE,ENUM)
TM_PROP(4009,TMT,ICONEFFECT,ENUM)
TM_PROP(4010,TMT,TEXTSHADOWTYPE,ENUM)
TM_PROP(4011,TMT,IMAGELAYOUT,ENUM)
TM_PROP(4012,TMT,GLYPHTYPE,ENUM)
TM_PROP(4013,TMT,IMAGESELECTTYPE,ENUM)
TM_PROP(4014,TMT,GLYPHFONTSIZINGTYPE,ENUM)
TM_PROP(4015,TMT,TRUESIZESCALINGTYPE,ENUM)

TM_PROP(5001,TMT,USERPICTURE,WINBOOL)
TM_PROP(5002,TMT,DEFAULTPANESIZE,RECT)
TM_PROP(5003,TMT,BLENDCOLOR,COLOR)

END_TM_PROPS()

BEGIN_TM_CLASS_PARTS(WINDOW)
TM_PART(1,WP,CAPTION)
TM_PART(2,WP,SMALLCAPTION)
TM_PART(3,WP,MINCAPTION)
TM_PART(4,WP,SMALLMINCAPTION)
TM_PART(5,WP,MAXCAPTION)
TM_PART(6,WP,SMALLMAXCAPTION)
TM_PART(7,WP,FRAMELEFT)
TM_PART(8,WP,FRAMERIGHT)
TM_PART(9,WP,FRAMEBOTTOM)
TM_PART(10,WP,SMALLFRAMELEFT)
TM_PART(11,WP,SMALLFRAMERIGHT)
TM_PART(12,WP,SMALLFRAMEBOTTOM)

TM_PART(13,WP,SYSBUTTON)
TM_PART(14,WP,MDISYSBUTTON)
TM_PART(15,WP,MINBUTTON)
TM_PART(16,WP,MDIMINBUTTON)
TM_PART(17,WP,MAXBUTTON)
TM_PART(18,WP,CLOSEBUTTON)
TM_PART(19,WP,SMALLCLOSEBUTTON)
TM_PART(20,WP,MDICLOSEBUTTON)
TM_PART(21,WP,RESTOREBUTTON)
TM_PART(22,WP,MDIRESTOREBUTTON)
TM_PART(23,WP,HELPBUTTON)
TM_PART(24,WP,MDIHELPBUTTON)

TM_PART(25,WP,HORZSCROLL)
TM_PART(26,WP,HORZTHUMB)
TM_PART(27,WP,VERTSCROLL)
TM_PART(28,WP,VERTTHUMB)

TM_PART(29,WP,DIALOG)

TM_PART(30,WP,CAPTIONSIZINGTEMPLATE)
TM_PART(31,WP,SMALLCAPTIONSIZINGTEMPLATE)
TM_PART(32,WP,FRAMELEFTSIZINGTEMPLATE)
TM_PART(33,WP,SMALLFRAMELEFTSIZINGTEMPLATE)
TM_PART(34,WP,FRAMERIGHTSIZINGTEMPLATE)
TM_PART(35,WP,SMALLFRAMERIGHTSIZINGTEMPLATE)
TM_PART(36,WP,FRAMEBOTTOMSIZINGTEMPLATE)
TM_PART(37,WP,SMALLFRAMEBOTTOMSIZINGTEMPLATE)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(FRAME)
TM_STATE(1,FS,ACTIVE)
TM_STATE(2,FS,INACTIVE)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(CAPTION)
TM_STATE(1,CS,ACTIVE)
TM_STATE(2,CS,INACTIVE)
TM_STATE(3,CS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(MAXCAPTION)
TM_STATE(1,MXCS,ACTIVE)
TM_STATE(2,MXCS,INACTIVE)
TM_STATE(3,MXCS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(MINCAPTION)
TM_STATE(1,MNCS,ACTIVE)
TM_STATE(2,MNCS,INACTIVE)
TM_STATE(3,MNCS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HORZSCROLL)
TM_STATE(1,HSS,NORMAL)
TM_STATE(2,HSS,HOT)
TM_STATE(3,HSS,PUSHED)
TM_STATE(4,HSS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HORZTHUMB)
TM_STATE(1,HTS,NORMAL)
TM_STATE(2,HTS,HOT)
TM_STATE(3,HTS,PUSHED)
TM_STATE(4,HTS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(VERTSCROLL)
TM_STATE(1,VSS,NORMAL)
TM_STATE(2,VSS,HOT)
TM_STATE(3,VSS,PUSHED)
TM_STATE(4,VSS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(VERTTHUMB)
TM_STATE(1,VTS,NORMAL)
TM_STATE(2,VTS,HOT)
TM_STATE(3,VTS,PUSHED)
TM_STATE(4,VTS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(SYSBUTTON)
TM_STATE(1,SBS,NORMAL)
TM_STATE(2,SBS,HOT)
TM_STATE(3,SBS,PUSHED)
TM_STATE(4,SBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(MINBUTTON)
TM_STATE(1,MINBS,NORMAL)
TM_STATE(2,MINBS,HOT)
TM_STATE(3,MINBS,PUSHED)
TM_STATE(4,MINBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(MAXBUTTON)
TM_STATE(1,MAXBS,NORMAL)
TM_STATE(2,MAXBS,HOT)
TM_STATE(3,MAXBS,PUSHED)
TM_STATE(4,MAXBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(RESTOREBUTTON)
TM_STATE(1,RBS,NORMAL)
TM_STATE(2,RBS,HOT)
TM_STATE(3,RBS,PUSHED)
TM_STATE(4,RBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HELPBUTTON)
TM_STATE(1,HBS,NORMAL)
TM_STATE(2,HBS,HOT)
TM_STATE(3,HBS,PUSHED)
TM_STATE(4,HBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(CLOSEBUTTON)
TM_STATE(1,CBS,NORMAL)
TM_STATE(2,CBS,HOT)
TM_STATE(3,CBS,PUSHED)
TM_STATE(4,CBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(BUTTON)
TM_PART(1,BP,PUSHBUTTON)
TM_PART(2,BP,RADIOBUTTON)
TM_PART(3,BP,CHECKBOX)
TM_PART(4,BP,GROUPBOX)
TM_PART(5,BP,USERBUTTON)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(PUSHBUTTON)
TM_STATE(1,PBS,NORMAL)
TM_STATE(2,PBS,HOT)
TM_STATE(3,PBS,PRESSED)
TM_STATE(4,PBS,DISABLED)
TM_STATE(5,PBS,DEFAULTED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(RADIOBUTTON)
TM_STATE(1,RBS,UNCHECKEDNORMAL)
TM_STATE(2,RBS,UNCHECKEDHOT)
TM_STATE(3,RBS,UNCHECKEDPRESSED)
TM_STATE(4,RBS,UNCHECKEDDISABLED)
TM_STATE(5,RBS,CHECKEDNORMAL)
TM_STATE(6,RBS,CHECKEDHOT)
TM_STATE(7,RBS,CHECKEDPRESSED)
TM_STATE(8,RBS,CHECKEDDISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(CHECKBOX)
TM_STATE(1,CBS,UNCHECKEDNORMAL)
TM_STATE(2,CBS,UNCHECKEDHOT)
TM_STATE(3,CBS,UNCHECKEDPRESSED)
TM_STATE(4,CBS,UNCHECKEDDISABLED)
TM_STATE(5,CBS,CHECKEDNORMAL)
TM_STATE(6,CBS,CHECKEDHOT)
TM_STATE(7,CBS,CHECKEDPRESSED)
TM_STATE(8,CBS,CHECKEDDISABLED)
TM_STATE(9,CBS,MIXEDNORMAL)
TM_STATE(10,CBS,MIXEDHOT)
TM_STATE(11,CBS,MIXEDPRESSED)
TM_STATE(12,CBS,MIXEDDISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(GROUPBOX)
TM_STATE(1,GBS,NORMAL)
TM_STATE(2,GBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(REBAR)
TM_PART(1,RP,GRIPPER)
TM_PART(2,RP,GRIPPERVERT)
TM_PART(3,RP,BAND)
TM_PART(4,RP,CHEVRON)
TM_PART(5,RP,CHEVRONVERT)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(CHEVRON)
TM_STATE(1,CHEVS,NORMAL)
TM_STATE(2,CHEVS,HOT)
TM_STATE(3,CHEVS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(TOOLBAR)
TM_PART(1,TP,BUTTON)
TM_PART(2,TP,DROPDOWNBUTTON)
TM_PART(3,TP,SPLITBUTTON)
TM_PART(4,TP,SPLITBUTTONDROPDOWN)
TM_PART(5,TP,SEPARATOR)
TM_PART(6,TP,SEPARATORVERT)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(TOOLBAR)
TM_STATE(1,TS,NORMAL)
TM_STATE(2,TS,HOT)
TM_STATE(3,TS,PRESSED)
TM_STATE(4,TS,DISABLED)
TM_STATE(5,TS,CHECKED)
TM_STATE(6,TS,HOTCHECKED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(STATUS)
TM_PART(1,SP,PANE)
TM_PART(2,SP,GRIPPERPANE)
TM_PART(3,SP,GRIPPER)
END_TM_CLASS_PARTS()

BEGIN_TM_CLASS_PARTS(MENU)
TM_PART(1,MP,MENUITEM)
TM_PART(2,MP,MENUDROPDOWN)
TM_PART(3,MP,MENUBARITEM)
TM_PART(4,MP,MENUBARDROPDOWN)
TM_PART(5,MP,CHEVRON)
TM_PART(6,MP,SEPARATOR)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(MENU)
TM_STATE(1,MS,NORMAL)
TM_STATE(2,MS,SELECTED)
TM_STATE(3,MS,DEMOTED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(LISTVIEW)
TM_PART(1,LVP,LISTITEM)
TM_PART(2,LVP,LISTGROUP)
TM_PART(3,LVP,LISTDETAIL)
TM_PART(4,LVP,LISTSORTEDDETAIL)
TM_PART(5,LVP,EMPTYTEXT)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(LISTITEM)
TM_STATE(1,LIS,NORMAL)
TM_STATE(2,LIS,HOT)
TM_STATE(3,LIS,SELECTED)
TM_STATE(4,LIS,DISABLED)
TM_STATE(5,LIS,SELECTEDNOTFOCUS)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(HEADER)
TM_PART(1,HP,HEADERITEM)
TM_PART(2,HP,HEADERITEMLEFT)
TM_PART(3,HP,HEADERITEMRIGHT)
TM_PART(4,HP,HEADERSORTARROW)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(HEADERITEM)
TM_STATE(1,HIS,NORMAL)
TM_STATE(2,HIS,HOT)
TM_STATE(3,HIS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HEADERITEMLEFT)
TM_STATE(1,HILS,NORMAL)
TM_STATE(2,HILS,HOT)
TM_STATE(3,HILS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HEADERITEMRIGHT)
TM_STATE(1,HIRS,NORMAL)
TM_STATE(2,HIRS,HOT)
TM_STATE(3,HIRS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HEADERSORTARROW)
TM_STATE(1,HSAS,SORTEDUP)
TM_STATE(2,HSAS,SORTEDDOWN)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(PROGRESS)
TM_PART(1,PP,BAR)
TM_PART(2,PP,BARVERT)
TM_PART(3,PP,CHUNK)
TM_PART(4,PP,CHUNKVERT)
END_TM_CLASS_PARTS()

BEGIN_TM_CLASS_PARTS(TAB)
TM_PART(1,TABP,TABITEM)
TM_PART(2,TABP,TABITEMLEFTEDGE)
TM_PART(3,TABP,TABITEMRIGHTEDGE)
TM_PART(4,TABP,TABITEMBOTHEDGE)
TM_PART(5,TABP,TOPTABITEM)
TM_PART(6,TABP,TOPTABITEMLEFTEDGE)
TM_PART(7,TABP,TOPTABITEMRIGHTEDGE)
TM_PART(8,TABP,TOPTABITEMBOTHEDGE)
TM_PART(9,TABP,PANE)
TM_PART(10,TABP,BODY)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(TABITEM)
TM_STATE(1,TIS,NORMAL)
TM_STATE(2,TIS,HOT)
TM_STATE(3,TIS,SELECTED)
TM_STATE(4,TIS,DISABLED)
TM_STATE(5,TIS,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TABITEMLEFTEDGE)
TM_STATE(1,TILES,NORMAL)
TM_STATE(2,TILES,HOT)
TM_STATE(3,TILES,SELECTED)
TM_STATE(4,TILES,DISABLED)
TM_STATE(5,TILES,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TABITEMRIGHTEDGE)
TM_STATE(1,TIRES,NORMAL)
TM_STATE(2,TIRES,HOT)
TM_STATE(3,TIRES,SELECTED)
TM_STATE(4,TIRES,DISABLED)
TM_STATE(5,TIRES,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TABITEMBOTHEDGES)
TM_STATE(1,TIBES,NORMAL)
TM_STATE(2,TIBES,HOT)
TM_STATE(3,TIBES,SELECTED)
TM_STATE(4,TIBES,DISABLED)
TM_STATE(5,TIBES,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TOPTABITEM)
TM_STATE(1,TTIS,NORMAL)
TM_STATE(2,TTIS,HOT)
TM_STATE(3,TTIS,SELECTED)
TM_STATE(4,TTIS,DISABLED)
TM_STATE(5,TTIS,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TOPTABITEMLEFTEDGE)
TM_STATE(1,TTILES,NORMAL)
TM_STATE(2,TTILES,HOT)
TM_STATE(3,TTILES,SELECTED)
TM_STATE(4,TTILES,DISABLED)
TM_STATE(5,TTILES,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TOPTABITEMRIGHTEDGE)
TM_STATE(1,TTIRES,NORMAL)
TM_STATE(2,TTIRES,HOT)
TM_STATE(3,TTIRES,SELECTED)
TM_STATE(4,TTIRES,DISABLED)
TM_STATE(5,TTIRES,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TOPTABITEMBOTHEDGES)
TM_STATE(1,TTIBES,NORMAL)
TM_STATE(2,TTIBES,HOT)
TM_STATE(3,TTIBES,SELECTED)
TM_STATE(4,TTIBES,DISABLED)
TM_STATE(5,TTIBES,FOCUSED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(TRACKBAR)
TM_PART(1,TKP,TRACK)
TM_PART(2,TKP,TRACKVERT)
TM_PART(3,TKP,THUMB)
TM_PART(4,TKP,THUMBBOTTOM)
TM_PART(5,TKP,THUMBTOP)
TM_PART(6,TKP,THUMBVERT)
TM_PART(7,TKP,THUMBLEFT)
TM_PART(8,TKP,THUMBRIGHT)
TM_PART(9,TKP,TICS)
TM_PART(10,TKP,TICSVERT)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(TRACKBAR)
TM_STATE(1,TKS,NORMAL)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TRACK)
TM_STATE(1,TRS,NORMAL)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TRACKVERT)
TM_STATE(1,TRVS,NORMAL)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(THUMB)
TM_STATE(1,TUS,NORMAL)
TM_STATE(2,TUS,HOT)
TM_STATE(3,TUS,PRESSED)
TM_STATE(4,TUS,FOCUSED)
TM_STATE(5,TUS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(THUMBBOTTOM)
TM_STATE(1,TUBS,NORMAL)
TM_STATE(2,TUBS,HOT)
TM_STATE(3,TUBS,PRESSED)
TM_STATE(4,TUBS,FOCUSED)
TM_STATE(5,TUBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(THUMBTOP)
TM_STATE(1,TUTS,NORMAL)
TM_STATE(2,TUTS,HOT)
TM_STATE(3,TUTS,PRESSED)
TM_STATE(4,TUTS,FOCUSED)
TM_STATE(5,TUTS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(THUMBVERT)
TM_STATE(1,TUVS,NORMAL)
TM_STATE(2,TUVS,HOT)
TM_STATE(3,TUVS,PRESSED)
TM_STATE(4,TUVS,FOCUSED)
TM_STATE(5,TUVS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(THUMBLEFT)
TM_STATE(1,TUVLS,NORMAL)
TM_STATE(2,TUVLS,HOT)
TM_STATE(3,TUVLS,PRESSED)
TM_STATE(4,TUVLS,FOCUSED)
TM_STATE(5,TUVLS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(THUMBRIGHT)
TM_STATE(1,TUVRS,NORMAL)
TM_STATE(2,TUVRS,HOT)
TM_STATE(3,TUVRS,PRESSED)
TM_STATE(4,TUVRS,FOCUSED)
TM_STATE(5,TUVRS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TICS)
TM_STATE(1,TSS,NORMAL)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(TICSVERT)
TM_STATE(1,TSVS,NORMAL)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(TOOLTIP)
TM_PART(1,TTP,STANDARD)
TM_PART(2,TTP,STANDARDTITLE)
TM_PART(3,TTP,BALLOON)
TM_PART(4,TTP,BALLOONTITLE)
TM_PART(5,TTP,CLOSE)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(CLOSE)
TM_STATE(1,TTCS,NORMAL)
TM_STATE(2,TTCS,HOT)
TM_STATE(3,TTCS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(STANDARD)
TM_STATE(1,TTSS,NORMAL)
TM_STATE(2,TTSS,LINK)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(BALLOON)
TM_STATE(1,TTBS,NORMAL)
TM_STATE(2,TTBS,LINK)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(TREEVIEW)
TM_PART(1,TVP,TREEITEM)
TM_PART(2,TVP,GLYPH)
TM_PART(3,TVP,BRANCH)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(TREEITEM)
TM_STATE(1,TREIS,NORMAL)
TM_STATE(2,TREIS,HOT)
TM_STATE(3,TREIS,SELECTED)
TM_STATE(4,TREIS,DISABLED)
TM_STATE(5,TREIS,SELECTEDNOTFOCUS)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(GLYPH)
TM_STATE(1,GLPS,CLOSED)
TM_STATE(2,GLPS,OPENED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(SPIN)
TM_PART(1,SPNP,UP)
TM_PART(2,SPNP,DOWN)
TM_PART(3,SPNP,UPHORZ)
TM_PART(4,SPNP,DOWNHORZ)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(UP)
TM_STATE(1,UPS,NORMAL)
TM_STATE(2,UPS,HOT)
TM_STATE(3,UPS,PRESSED)
TM_STATE(4,UPS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(DOWN)
TM_STATE(1,DNS,NORMAL)
TM_STATE(2,DNS,HOT)
TM_STATE(3,DNS,PRESSED)
TM_STATE(4,DNS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(UPHORZ)
TM_STATE(1,UPHZS,NORMAL)
TM_STATE(2,UPHZS,HOT)
TM_STATE(3,UPHZS,PRESSED)
TM_STATE(4,UPHZS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(DOWNHORZ)
TM_STATE(1,DNHZS,NORMAL)
TM_STATE(2,DNHZS,HOT)
TM_STATE(3,DNHZS,PRESSED)
TM_STATE(4,DNHZS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(PAGE)
TM_PART(1,PGRP,UP)
TM_PART(2,PGRP,DOWN)
TM_PART(3,PGRP,UPHORZ)
TM_PART(4,PGRP,DOWNHORZ)
END_TM_CLASS_PARTS()

BEGIN_TM_CLASS_PARTS(SCROLLBAR)
TM_PART(1,SBP,ARROWBTN)
TM_PART(2,SBP,THUMBBTNHORZ)
TM_PART(3,SBP,THUMBBTNVERT)
TM_PART(4,SBP,LOWERTRACKHORZ)
TM_PART(5,SBP,UPPERTRACKHORZ)
TM_PART(6,SBP,LOWERTRACKVERT)
TM_PART(7,SBP,UPPERTRACKVERT)
TM_PART(8,SBP,GRIPPERHORZ)
TM_PART(9,SBP,GRIPPERVERT)
TM_PART(10,SBP,SIZEBOX)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(ARROWBTN)
TM_STATE(1,ABS,UPNORMAL)
TM_STATE(2,ABS,UPHOT)
TM_STATE(3,ABS,UPPRESSED)
TM_STATE(4,ABS,UPDISABLED)
TM_STATE(5,ABS,DOWNNORMAL)
TM_STATE(6,ABS,DOWNHOT)
TM_STATE(7,ABS,DOWNPRESSED)
TM_STATE(8,ABS,DOWNDISABLED)
TM_STATE(9,ABS,LEFTNORMAL)
TM_STATE(10,ABS,LEFTHOT)
TM_STATE(11,ABS,LEFTPRESSED)
TM_STATE(12,ABS,LEFTDISABLED)
TM_STATE(13,ABS,RIGHTNORMAL)
TM_STATE(14,ABS,RIGHTHOT)
TM_STATE(15,ABS,RIGHTPRESSED)
TM_STATE(16,ABS,RIGHTDISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(SCROLLBAR)
TM_STATE(1,SCRBS,NORMAL)
TM_STATE(2,SCRBS,HOT)
TM_STATE(3,SCRBS,PRESSED)
TM_STATE(4,SCRBS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(SIZEBOX)
TM_STATE(1,SZB,RIGHTALIGN)
TM_STATE(2,SZB,LEFTALIGN)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(EDIT)
TM_PART(1,EP,EDITTEXT)
TM_PART(2,EP,CARET)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(EDITTEXT)
TM_STATE(1,ETS,NORMAL)
TM_STATE(2,ETS,HOT)
TM_STATE(3,ETS,SELECTED)
TM_STATE(4,ETS,DISABLED)
TM_STATE(5,ETS,FOCUSED)
TM_STATE(6,ETS,READONLY)
TM_STATE(7,ETS,ASSIST)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(COMBOBOX)
TM_PART(1,CP,DROPDOWNBUTTON)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(COMBOBOX)
TM_STATE(1,CBXS,NORMAL)
TM_STATE(2,CBXS,HOT)
TM_STATE(3,CBXS,PRESSED)
TM_STATE(4,CBXS,DISABLED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(CLOCK)
TM_PART(1,CLP,TIME)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(CLOCK)
TM_STATE(1,CLS,NORMAL)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(TRAYNOTIFY)
TM_PART(1,TNP,BACKGROUND)
TM_PART(2,TNP,ANIMBACKGROUND)
END_TM_CLASS_PARTS()

BEGIN_TM_CLASS_PARTS(TASKBAR)
TM_PART(1,TBP,BACKGROUNDBOTTOM)
TM_PART(2,TBP,BACKGROUNDRIGHT)
TM_PART(3,TBP,BACKGROUNDTOP)
TM_PART(4,TBP,BACKGROUNDLEFT)
TM_PART(5,TBP,SIZINGBARBOTTOM)
TM_PART(6,TBP,SIZINGBARRIGHT)
TM_PART(7,TBP,SIZINGBARTOP)
TM_PART(8,TBP,SIZINGBARLEFT)
END_TM_CLASS_PARTS()

BEGIN_TM_CLASS_PARTS(TASKBAND)
TM_PART(1,TDP,GROUPCOUNT)
TM_PART(2,TDP,FLASHBUTTON)
TM_PART(3,TDP,FLASHBUTTONGROUPMENU)
END_TM_CLASS_PARTS()

BEGIN_TM_CLASS_PARTS(STARTPANEL)
TM_PART(1,SPP,USERPANE)
TM_PART(2,SPP,MOREPROGRAMS)
TM_PART(3,SPP,MOREPROGRAMSARROW)
TM_PART(4,SPP,PROGLIST)
TM_PART(5,SPP,PROGLISTSEPARATOR)
TM_PART(6,SPP,PLACESLIST)
TM_PART(7,SPP,PLACESLISTSEPARATOR)
TM_PART(8,SPP,LOGOFF)
TM_PART(9,SPP,LOGOFFBUTTONS)
TM_PART(10,SPP,USERPICTURE)
TM_PART(11,SPP,PREVIEW)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(MOREPROGRAMSARROW)
TM_STATE(1,SPS,NORMAL)
TM_STATE(2,SPS,HOT)
TM_STATE(3,SPS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(LOGOFFBUTTONS)
TM_STATE(1,SPLS,NORMAL)
TM_STATE(2,SPLS,HOT)
TM_STATE(3,SPLS,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(EXPLORERBAR)
TM_PART(1,EBP,HEADERBACKGROUND)
TM_PART(2,EBP,HEADERCLOSE)
TM_PART(3,EBP,HEADERPIN)
TM_PART(4,EBP,IEBARMENU)
TM_PART(5,EBP,NORMALGROUPBACKGROUND)
TM_PART(6,EBP,NORMALGROUPCOLLAPSE)
TM_PART(7,EBP,NORMALGROUPEXPAND)
TM_PART(8,EBP,NORMALGROUPHEAD)
TM_PART(9,EBP,SPECIALGROUPBACKGROUND)
TM_PART(10,EBP,SPECIALGROUPCOLLAPSE)
TM_PART(11,EBP,SPECIALGROUPEXPAND)
TM_PART(12,EBP,SPECIALGROUPHEAD)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(HEADERCLOSE)
TM_STATE(1,EBHC,NORMAL)
TM_STATE(2,EBHC,HOT)
TM_STATE(3,EBHC,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(HEADERPIN)
TM_STATE(1,EBHP,NORMAL)
TM_STATE(2,EBHP,HOT)
TM_STATE(3,EBHP,PRESSED)
TM_STATE(4,EBHP,SELECTEDNORMAL)
TM_STATE(5,EBHP,SELECTEDHOT)
TM_STATE(6,EBHP,SELECTEDPRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(IEBARMENU)
TM_STATE(1,EBM,NORMAL)
TM_STATE(2,EBM,HOT)
TM_STATE(3,EBM,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(NORMALGROUPCOLLAPSE)
TM_STATE(1,EBNGC,NORMAL)
TM_STATE(2,EBNGC,HOT)
TM_STATE(3,EBNGC,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(NORMALGROUPEXPAND)
TM_STATE(1,EBNGE,NORMAL)
TM_STATE(2,EBNGE,HOT)
TM_STATE(3,EBNGE,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(SPECIALGROUPCOLLAPSE)
TM_STATE(1,EBSGC,NORMAL)
TM_STATE(2,EBSGC,HOT)
TM_STATE(3,EBSGC,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_PART_STATES(SPECIALGROUPEXPAND)
TM_STATE(1,EBSGE,NORMAL)
TM_STATE(2,EBSGE,HOT)
TM_STATE(3,EBSGE,PRESSED)
END_TM_PART_STATES()

BEGIN_TM_CLASS_PARTS(MENUBAND)
TM_PART(1,MDP,NEWAPPBUTTON)
TM_PART(2,MDP,SEPERATOR)
END_TM_CLASS_PARTS()

BEGIN_TM_PART_STATES(MENUBAND)
TM_STATE(1,MDS,NORMAL)
TM_STATE(2,MDS,HOT)
TM_STATE(3,MDS,PRESSED)
TM_STATE(4,MDS,DISABLED)
TM_STATE(5,MDS,CHECKED)
TM_STATE(6,MDS,HOTCHECKED)
END_TM_PART_STATES()

END_TM_SCHEMA(ThemeMgrSchema)
#endif
