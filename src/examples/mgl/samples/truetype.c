/****************************************************************************
*
*					SciTech Multi-platform Graphics Library
*
*  ========================================================================
*
*    The contents of this file are subject to the SciTech MGL Public
*    License Version 1.0 (the "License"); you may not use this file
*    except in compliance with the License. You may obtain a copy of
*    the License at http://www.scitechsoft.com/mgl-license.txt
*
*    Software distributed under the License is distributed on an
*    "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
*    implied. See the License for the specific language governing
*    rights and limitations under the License.
*
*    The Original Code is Copyright (C) 1991-1998 SciTech Software, Inc.
*
*    The Initial Developer of the Original Code is SciTech Software, Inc.
*    All Rights Reserved.
*
*  ========================================================================
*
* Language:		ANSI C
* Environment:	Any
*
* Description:  Demo showing how to use the text functions to draw text to
*				the screen.
*
****************************************************************************/

#include "mglsamp.h"
#include <string.h>
#include <stdlib.h>

char *FontLibname[] = {
"win2x/verdana.ttf",
"truetype/1stgrade.ttf",   "truetype/flange-l.ttf",
"truetype/1979rg__.ttf",   "truetype/finalnew.ttf", 
"truetype/39smooth.ttf",   "truetype/fletcher.ttf",
"truetype/7hours.ttf",     "truetype/flora_bo.ttf", 
"truetype/aarco1.ttf",     "truetype/flora_no.ttf",
"truetype/abbey_m1.ttf",   "truetype/florenc_.ttf", 
"truetype/abbey_m4.ttf",   "truetype/florence.ttf", 
"truetype/abbey_me.ttf",   "truetype/foxtrot.ttf",
"truetype/abctec04.ttf",   "truetype/frankenf.ttf", 
"truetype/ac.ttf",         "truetype/freehan1.ttf", 
"truetype/adlib.ttf",      "truetype/freehand.ttf", 
"truetype/adlib_ex.ttf",   "truetype/freeport.ttf", 
"truetype/adlib_th.ttf",   "truetype/froman.ttf", 
"truetype/adlib_wd.ttf",   "truetype/fundrunk.ttf", 
"truetype/advert.ttf",     "truetype/fusion.ttf", 
"truetype/african.ttf",    "truetype/fusion_n.ttf", 
"truetype/agate_no.ttf",   "truetype/g_cut1.ttf", 
"truetype/aircut3.ttf",    "truetype/galleria.ttf", 
"truetype/aleia.ttf",      "truetype/garton.ttf", 
"truetype/alex_ant.ttf",   "truetype/ges_____.ttf", 
"truetype/alexandr.ttf",   "truetype/gismonda.ttf", 
"truetype/alfr_ho.ttf",    "truetype/gocmp___.ttf", 
"truetype/alfredo_.ttf",   "truetype/gordon__.ttf", 
"truetype/alison__.ttf",   "truetype/gustavus.ttf", 
"truetype/allegro.ttf",    "truetype/hancock.ttf", 
"truetype/allheart.ttf",   "truetype/handel.ttf", 
"truetype/alpine.ttf",     "truetype/hangem.ttf", 
"truetype/ambrosia.ttf",   "truetype/harri.ttf", 
"truetype/ametz___.ttf",   "truetype/hasty.ttf", 
"truetype/amtyp__n.ttf",   "truetype/hbo.ttf", 
"truetype/angular.ttf",    "truetype/helna.ttf",   
"truetype/annifont.ttf",   "truetype/hobo_ho.ttf", 
"truetype/annstone.ttf",   "truetype/hotshot.ttf", 
"truetype/anson.ttf",      "truetype/howard.ttf", 
"truetype/ant883.ttf",     "truetype/iglesia_.ttf", 
"truetype/antique_.ttf",   "truetype/igloocap.ttf", 
"truetype/antiquer.ttf",   "truetype/imperium.ttf", 
"truetype/aquilla_.ttf",   "truetype/impress.ttf", 
"truetype/arabian_.ttf",   "truetype/informal.ttf", 
"truetype/architex.ttf",   "truetype/italian.ttf", 
"truetype/architic.ttf",   "truetype/japan.ttf", 
"truetype/architxb.ttf",   "truetype/jazz.ttf", 
"truetype/architxi.ttf",   "truetype/jester.ttf", 
"truetype/architxt.ttf",   "truetype/jobber.ttf", 
"truetype/archtura.ttf",   "truetype/junipr.ttf", 
"truetype/arctic2.ttf",    "truetype/kabel.ttf",
"truetype/arenski.ttf",    "truetype/kennon.ttf", 
"truetype/ariston1.ttf",   "truetype/klingon.ttf", 
"truetype/arroy.ttf",      "truetype/lansbury.ttf",
"truetype/asimov.ttf",     "truetype/leftycas.ttf", 
"truetype/atlainln.ttf",   "truetype/lichtnr.ttf", 
"truetype/aubauer.ttf",    "truetype/light_bl.ttf", 
"truetype/aukoloss.ttf",   "truetype/lightou.ttf", 
"truetype/aurora.ttf",     "truetype/lithogrb.ttf", 
"truetype/author.ttf",     "truetype/livingby.ttf", 
"truetype/avatar.ttf",     "truetype/lucas.ttf", 
"truetype/avion.ttf",      "truetype/m.ttf", 
"truetype/badfilms.ttf",   "truetype/machum12.ttf", 
"truetype/balleten.ttf",   "truetype/magde.ttf", 
"truetype/balloon1.ttf",   "truetype/mardigr1.ttf", 
"truetype/baltar01.ttf",   "truetype/mariage4.ttf", 
"truetype/basque_l.ttf",   "truetype/mazam.ttf", 
"truetype/bear.ttf",       "truetype/memo_.ttf", 
"truetype/beatsvil.ttf",   "truetype/mengrav2.ttf", 
"truetype/bebl____.ttf",   "truetype/merlinn.ttf", 
"truetype/bebop.ttf",      "truetype/metalord.ttf", 
"truetype/becker_m.ttf",   "truetype/mfigaro.ttf", 
"truetype/bed870.ttf",     "truetype/mickey.ttf", 
"truetype/benguia1.ttf",   "truetype/microtie.ttf", 
"truetype/benjami1.ttf",   "truetype/minya.ttf", 
"truetype/benqu.ttf",      "truetype/mira____.ttf", 
"truetype/benqu_b.ttf",    "truetype/mlsgd.ttf", 
"truetype/berns.ttf",      "truetype/mrkflthn.ttf", 
"truetype/bert____.ttf",   "truetype/mrkfltwd.ttf", 
"truetype/bisque.ttf",     "truetype/mystic.ttf",  
"truetype/blackcha.ttf",   "truetype/n_e_b.ttf", 
"truetype/blippo-h.ttf",   "truetype/n_e_b_b.ttf", 
"truetype/bodidlyb.ttf",   "truetype/nas966.ttf", 
"truetype/bolide.ttf",     "truetype/neil____.ttf", 
"truetype/bonzai.ttf",     "truetype/neuvares.ttf", 
"truetype/bookjack.ttf",   "truetype/ninjp___.ttf", 
"truetype/boomeran.ttf",   "truetype/nitemare.ttf", 
"truetype/boomi___.ttf",   "truetype/no.ttf", 
"truetype/borzoire.ttf",   "truetype/nocontro.ttf", 
"truetype/bridge.ttf",     "truetype/nuancei.ttf", 
"truetype/brook.ttf",      "truetype/oregndr.ttf", 
"truetype/bucephtt.ttf",   "truetype/oregnwe.ttf",
"truetype/budangul.ttf",   "truetype/oxfrd.ttf", 
"truetype/caligula.ttf",   "truetype/paganini.ttf", 
"truetype/calligra.ttf",   "truetype/pares.ttf",
"truetype/camberic.ttf",   "truetype/pares_o.ttf", 
"truetype/camelot_.ttf",   "truetype/pcrypt.ttf", 
"truetype/canaith.ttf",    "truetype/pepl_exp.ttf", 
"truetype/caraway_.ttf",   "truetype/pepl_o.ttf", 
"truetype/carleton.ttf",   "truetype/peplum.ttf", 
"truetype/cascad__.ttf",   "truetype/phaedri_.ttf", 
"truetype/casscond.ttf",   "truetype/phoenix_.ttf", 
"truetype/chaucer.ttf",    "truetype/pompeii.ttf", 
"truetype/child.ttf",      "truetype/postanti.ttf", 
"truetype/chltc_b.ttf",    "truetype/premi.ttf", 
"truetype/chm_____.ttf",   "truetype/presents.ttf", 
"truetype/chopinop.ttf",   "truetype/prsent.ttf", 
"truetype/ciagoth.ttf",    "truetype/quill_s.ttf", 
"truetype/civitype.ttf",   "truetype/rabear.ttf", 
"truetype/clarendo.ttf",   "truetype/rechtman.ttf", 
"truetype/clearfa2.ttf",   "truetype/redfont.ttf", 
"truetype/cloister.ttf",   "truetype/reed.ttf", 
"truetype/cobb.ttf",       "truetype/ren&stim.ttf", 
"truetype/coronet-.ttf",   "truetype/revival_.ttf", 
"truetype/croisant.ttf",   "truetype/rhyol___.ttf", 
"truetype/csd_chal.ttf",   "truetype/richardm.ttf", 
"truetype/cuckoo.ttf",     "truetype/roissy.ttf", 
"truetype/cuneifor.ttf",   "truetype/roundinf.ttf", 
"truetype/dali____.ttf",   "truetype/rsgraphi.ttf", 
"truetype/debussy.ttf",    "truetype/rsjacks.ttf", 
"truetype/dicken.ttf",     "truetype/sacc_o.ttf", 
"truetype/dingdong.ttf",   "truetype/saccule.ttf", 
"truetype/dixon.ttf",      "truetype/sadh_o.ttf", 
"truetype/domcasua.ttf",   "truetype/sadhu.ttf", 
"truetype/dorcla__.ttf",   "truetype/saff_too.ttf", 
"truetype/dow_____.ttf",   "truetype/sanlite.ttf", 
"truetype/drg.ttf",        "truetype/sketch.ttf", 
"truetype/dubiel.ttf",     "truetype/slogan_n.ttf", 
"truetype/eastside.ttf",   "truetype/squire.ttf", 
"truetype/eccentri.ttf",   "truetype/steelwlf.ttf", 
"truetype/eckmann_.ttf",   "truetype/stonehen.ttf", 
"truetype/edition_.ttf",   "truetype/stymiel1.ttf", 
"truetype/eklekti3.ttf",   "truetype/styrofoa.ttf",
"truetype/ekletic.ttf",    "truetype/terminat.ttf", 
"truetype/elastic.ttf",    "truetype/tgscript.ttf", 
"truetype/eldbjtt.ttf",    "truetype/thomas.ttf",
"truetype/elzevier.ttf",   "truetype/toonzscr.ttf", 
"truetype/english1.ttf",   "truetype/tt0626m_.ttf", 
"truetype/english_.ttf",   "truetype/tt0727m_.ttf", 
"truetype/englisht.ttf",   "truetype/unicorn.ttf", 
"truetype/engross.ttf",    "truetype/upwest.ttf", 
"truetype/enp_____.ttf",   "truetype/vietfont.ttf", 
"truetype/erie_li1.ttf",   "truetype/vikingn.ttf", 
"truetype/erie_lig.ttf",   "truetype/vireofn.ttf", 
"truetype/eterna.ttf",     "truetype/vivala.ttf", 
"truetype/ex1472.ttf",     "truetype/vogue_no.ttf", 
"truetype/exlogo.ttf",     "truetype/walrod__.ttf", 
"truetype/featherl.ttf",   "truetype/westend.ttf", 
"truetype/fecm____.ttf",   "truetype/wildwst.ttf", 
"truetype/feltmari.ttf",   "truetype/wilhlmk.ttf", 
"truetype/feltmark.ttf",   "truetype/zirkle.ttf", 
"truetype/fettefra.ttf", 
	};
#define NUM_FontLibS   (sizeof(FontLibname) / sizeof(FontLibname[0]))

/*----------------------------- Global Variables --------------------------*/

char demoName[] = "textdemo";

/*------------------------------ Implementation ---------------------------*/

#define MYRED MGL_rgbColor(dc, 255, 0, 0)
#define MYBLUE MGL_rgbColor(dc, 0, 0, 255)
#define MYGREEN MGL_rgbColor(dc, 0, 255, 0)
#define MYWHITE MGL_rgbColor(dc, 255, 255, 255)
#define MYBLACK MGL_rgbColor(dc, 0, 0, 0)

ibool textLibDump(MGLDC *dc)
/****************************************************************************
*
* Function:     textDump
* Parameters:   dc  - Device context
*
* Description:  Displays all the characters in each of the fonts in
*               the distribution.
*
****************************************************************************/
{
    int         i,j,maxx,pointSize = 24;
	font_t      *font;
	font_lib_t  *fontLib;
	char        buf[80];

	mainWindow(dc,"Loading font...");
	maxx = MGL_maxx();
	for (i = 0; i < NUM_FontLibS; i++) {   /* For each available font      */
		/* Attempt to load the font file from disk. If it cannot be
		 * found, then go onto the next one.
		 */
		strcpy(buf,FontLibname[i]);
		if ((fontLib = MGL_openFontLib(buf)) == NULL)
			continue;
        if ((font = MGL_loadFontInstance(fontLib,pointSize,0,0,true)) == NULL) {
			MGL_closeFontLib(fontLib);
			continue;
			}
		switch (fontLib->fontLibType) {
			case MGL_BITMAPFONT_LIB:
				sprintf(buf,"Bitmap Font Library: %s - %d Point", fontLib->name, font->pointSize);
				break;
			case MGL_TRUETYPEFONT_LIB:
				sprintf(buf,"TrueType Font Library: %s - %d Point", fontLib->name, font->pointSize);
				break;
			case MGL_TYPE1FONT_LIB:
				sprintf(buf,"Type1 Font Library: %s - %d Point", fontLib->name, font->pointSize);
				break;
			}
		mainWindow(dc,buf);

		/* Display all characters on the screen */
		MGL_useFont(font);
		MGL_setFontBlendMode(MGL_AA_RGBBLEND);
		if (MGL_getBitsPerPixel(dc) <= 8) {
			MGL_setFontAntiAliasPalette(
				MGL_rgbColor(dc, 255, 255, 255),
				MGL_rgbColor(dc, 192, 192, 192),
				MGL_rgbColor(dc, 128, 128, 128),
				MGL_rgbColor(dc, 64, 64, 64),
				MGL_rgbColor(dc, 0, 0, 0));
			}
		MGL_setColor(MYWHITE);
        MGL_setBackColor(MYBLACK);
        buf[1] = '\0';
		for (j = ' '; j < 256; j++) {
			buf[0] = j;
            MGL_drawStr(buf);
			if (MGL_getX() + MGL_maxCharWidth() > maxx) {
				MGL_moveToCoord(0,MGL_getY() + MGL_textHeight());
				}
			}

		/* Unload the font from memory */
		MGL_useFont(defFont);
		MGL_unloadFont(font);
		MGL_closeFontLib(fontLib);
		if (!pause())
			return false;
		mainWindow(dc,"Loading font...");
		}
	defaultAttributes(dc);
	return true;
}

ibool textDir(MGLDC *dc)
/****************************************************************************
*
* Function:     textDir
* Parameters:   dc  - Device context
*
* Description:  Display text in varying directions on the screen.
*
****************************************************************************/
{
	int         maxx,maxy;
	int         horiz,vert;
	char        *str;
	font_t      *font = NULL;
	font_lib_t  *fontLib;
	char        buf[80];
    ibool       cont = true;
	maxx = MGL_maxx();
	maxy = MGL_maxy();

	strcpy(buf,FontLibname[0]);
	if ((fontLib = MGL_openFontLib(buf)) == NULL)
		return FALSE;
    if ((font = MGL_loadFontInstance(fontLib,18,0,0,true)) == NULL) {
		MGL_closeFontLib(fontLib);
		return FALSE;
		}

	sprintf(buf,"Character set (%s)", font->name);
    MGL_clearDevice();
	mainWindow(dc,buf);
    
	MGL_useFont(font);
    for (horiz = MGL_LEFT_TEXT; horiz <= MGL_RIGHT_TEXT && cont; horiz++)
		for (vert = MGL_TOP_TEXT; vert <= MGL_BASELINE_TEXT; vert++) {
			MGL_clearDevice();
            mainWindow(dc,buf);
            
            MGL_useFont(font);
            MGL_setColor(MYRED);
			MGL_lineCoord(0,maxy/2,maxx,maxy/2);
			MGL_lineCoord(maxx/2,0,maxx/2,maxy);
			MGL_setTextJustify(MGL_LEFT_TEXT,MGL_TOP_TEXT);
			MGL_setTextDirection(MGL_RIGHT_DIR);
			MGL_setColor(MYGREEN);
			MGL_setFontBlendMode(MGL_AA_RGBBLEND);
			if (MGL_getBitsPerPixel(dc) <= 8) {
				MGL_setFontAntiAliasPalette(
					MGL_rgbColor(dc, 0, 255, 0),
					MGL_rgbColor(dc, 0, 192, 0),
					MGL_rgbColor(dc, 0, 128, 0),
					MGL_rgbColor(dc, 0, 64, 0),
					MGL_rgbColor(dc, 0, 0, 0));
				}
			MGL_setBackColor(MYBLACK);

			MGL_moveToCoord(0,0);
			MGL_drawStr("horiz: ");
			switch (horiz) {
				case MGL_LEFT_TEXT:
					str = "LEFT";
					break;
				case MGL_CENTER_TEXT:
					str = "CENTER";
					break;
				case MGL_RIGHT_TEXT:
					str = "RIGHT";
					break;
				default:
					str = "UNKNOWN";
				}
			MGL_drawStr(str);
			MGL_moveToCoord(0,MGL_textHeight());
			MGL_drawStr("vert:  ");
			switch (vert) {
				case MGL_TOP_TEXT:
					str = "TOP";
					break;
				case MGL_CENTER_TEXT:
					str = "CENTER";
					break;
				case MGL_BOTTOM_TEXT:
					str = "BOTTOM";
					break;
				case MGL_BASELINE_TEXT:
					str = "BASELINE";
					break;
				default:
					str = "UNKNOWN";
				}
			MGL_drawStr(str);
			MGL_makeCurrentDC(dc);

			MGL_setTextJustify(horiz,vert);
			MGL_useFont(font);

            MGL_setColor(MYBLUE);
			MGL_lineCoord(0,maxy/2+MGL_textHeight()-1,maxx,maxy/2+MGL_textHeight()-1);
			MGL_lineCoord(maxx/2+MGL_textHeight()-1,0,maxx/2+MGL_textHeight()-1,maxy);

            MGL_setColor(MYWHITE);
            MGL_setBackColor(MYBLACK);

			if (MGL_getBitsPerPixel(dc) <= 8) {
				MGL_setFontAntiAliasPalette(
					MGL_rgbColor(dc, 255, 255, 255),
					MGL_rgbColor(dc, 192, 192, 192),
					MGL_rgbColor(dc, 128, 128, 128),
					MGL_rgbColor(dc, 64, 64, 64),
					MGL_rgbColor(dc, 0, 0, 0));
				}

			MGL_setTextDirection(MGL_LEFT_DIR);
			MGL_drawStrXY(maxx/2,maxy/2,"This text goes left");

			MGL_setTextDirection(MGL_DOWN_DIR);
			MGL_drawStrXY(maxx/2,maxy/2,"This text goes down");

			MGL_setTextDirection(MGL_UP_DIR);
			MGL_drawStrXY(maxx/2,maxy/2,"This text goes up");

			MGL_setTextDirection(MGL_RIGHT_DIR);
			MGL_drawStrXY(maxx/2,maxy/2,"This text goes right");
            if (!pause()) {
                cont = false;
				break;
                }
            }

	/* Unload the font from memory */
	MGL_useFont(defFont);
	MGL_unloadFont(font);
	MGL_closeFontLib(fontLib);
	defaultAttributes(dc);
    return cont;
}

ibool textBlend(MGLDC *dc)
/****************************************************************************
*
* Function:     textBlend
* Parameters:   dc  - Device context
*
* Description:  Blend text on the screen.
*
****************************************************************************/
{
	int         maxx,maxy;
	font_t      *font = NULL;
	font_lib_t  *fontLib;
	char        buf[80];

	maxx = MGL_maxx();
	maxy = MGL_maxy();

	strcpy(buf,FontLibname[0]);
	if ((fontLib = MGL_openFontLib(buf)) == NULL)
		return FALSE;
    if ((font = MGL_loadFontInstance(fontLib,22,0,0,true)) == NULL) {
		MGL_closeFontLib(fontLib);
		return FALSE;
		}

	sprintf(buf,"Character set (%s)", font->name);
    MGL_clearDevice();
	mainWindow(dc,buf);
    
	MGL_useFont(font);

	MGL_setTextJustify(MGL_LEFT_TEXT,MGL_TOP_TEXT);
	MGL_setTextDirection(MGL_RIGHT_DIR);
    MGL_setColor(MYGREEN);
	MGL_loadPNGIntoDC(dc, "cow-24.png", (maxx/2)-160, (maxy/2)-120, false);

    MGL_setTextJustify(MGL_CENTER_TEXT,MGL_CENTER_TEXT);
    MGL_setFontBlendMode(MGL_AA_RGBBLEND);

	if (MGL_getBitsPerPixel(dc) <= 8) {
		MGL_setFontAntiAliasPalette(
			MGL_rgbColor(dc, 0, 255, 0),
			MGL_rgbColor(dc, 0, 192, 0),
			MGL_rgbColor(dc, 0, 128, 0),
			MGL_rgbColor(dc, 0, 64, 0),
			MGL_rgbColor(dc, 0, 0, 0));
		}

    MGL_setColor(MGL_rgbColor(dc, 255, 255, 255));
    MGL_moveToCoord(maxx/2-1, (maxy/2)-(MGL_textHeight()*2)-1);
    MGL_drawStr("This is a test of anti-aliased font blending.");

    MGL_setColor(MGL_rgbColor(dc, 0, 100, 0));
    MGL_moveToCoord(maxx/2+1, (maxy/2)-(MGL_textHeight()*2)+1);
    MGL_drawStr("This is a test of anti-aliased font blending.");

    MGL_setColor(MGL_rgbColor(dc, 0, 220, 0));
    MGL_moveToCoord(maxx/2, (maxy/2)-(MGL_textHeight()*2));
    MGL_drawStr("This is a test of anti-aliased font blending.");


    MGL_setColor(MGL_rgbColor(dc, 255, 0, 255));
    MGL_moveToCoord(maxx/2, (maxy/2)-(MGL_textHeight()));

    MGL_drawStr("Note: this test will not draw with");
    MGL_setColor(MGL_rgbColor(dc, 128, 128, 128));
    MGL_moveToCoord(maxx/2, maxy/2);
    MGL_drawStr("blending in 8 bit or lower modes");
    

    MGL_setColor(MGL_rgbColor(dc, 255, 255, 255));
    MGL_moveToCoord(maxx/2, (maxy/2)+(MGL_textHeight()));
    MGL_drawStr("press any key to continue");

    MGL_setColor(MGL_rgbColor(dc, 128, 128, 255));
    MGL_moveToCoord(maxx/2-1, (maxy/2)+(MGL_textHeight()*2)-1);
    MGL_drawStr("**************");

    MGL_setColor(MGL_rgbColor(dc, 0, 0, 128));
    MGL_moveToCoord(maxx/2+1, (maxy/2)+(MGL_textHeight()*2)+1);
    MGL_drawStr("**************");

    MGL_setColor(MGL_rgbColor(dc, 0, 0, 0));
    MGL_moveToCoord(maxx/2, (maxy/2)+(MGL_textHeight()*2));
    MGL_drawStr("**************");

    pause();

	/* Unload the font from memory */
	MGL_useFont(defFont);
	MGL_unloadFont(font);
	MGL_closeFontLib(fontLib);
	defaultAttributes(dc);
	return true;
}

void demo(MGLDC *dc)
{
    if (!textBlend(dc)) return;
    if (!textDir(dc)) return;
    if (!textLibDump(dc)) return;
}
