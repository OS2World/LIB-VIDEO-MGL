/****************************************************************************
*
*					SciTech OS Portability Manager Library
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
*					Portions copyright (C) Josh Vanderhoof
*
* Language:		ANSI C
* Environment:	Any
*
* Description:	Functions to save and restore the VGA hardware state.
*
****************************************************************************/

#include "pmapi.h"

/*--------------------------- Global variables ----------------------------*/

/* VGA index register ports */
#define CRT_I   0x3D4		/* CRT Controller Index 					*/
#define ATT_IW  0x3C0		/* Attribute Controller Index & Data 		*/
#define GRA_I   0x3CE		/* Graphics Controller Index 				*/
#define SEQ_I   0x3C4		/* Sequencer Index 							*/

/* VGA data register ports */
#define CRT_D   0x3D5		/* CRT Controller Data Register				*/
#define ATT_R   0x3C1		/* Attribute Controller Data Read Register 	*/
#define GRA_D   0x3CF		/* Graphics Controller Data Register 		*/
#define SEQ_D   0x3C5		/* Sequencer Data Register 					*/
#define MIS_R   0x3CC		/* Misc Output Read Register 				*/
#define MIS_W   0x3C2		/* Misc Output Write Register 				*/
#define IS1_R   0x3DA		/* Input Status Register 1 					*/
#define PEL_IW  0x3C8		/* PEL Write Index 							*/
#define PEL_IR  0x3C7		/* PEL Read Index 							*/
#define PEL_D   0x3C9		/* PEL Data Register 						*/

/* standard VGA indexes max counts */
#define CRT_C   24			/* 24  CRT Controller Registers 			*/
#define ATT_C   21			/* 21  Attribute Controller Registers 		*/
#define GRA_C   9			/* 9   Graphics Controller Registers 		*/
#define SEQ_C   5			/* 5   Sequencer Registers 					*/
#define MIS_C   1			/* 1   Misc Output Register 				*/
#define	PAL_C	768			/* 768 Paletet Registers					*/

/* VGA registers saving indexes */
#define CRT     0			/* CRT Controller Registers start 			*/
#define ATT     (CRT+CRT_C)	/* Attribute Controller Registers start 	*/
#define GRA     (ATT+ATT_C)	/* Graphics Controller Registers start 		*/
#define SEQ     (GRA+GRA_C)	/* Sequencer Registers 						*/
#define MIS     (SEQ+SEQ_C)	/* General Registers 						*/
#define PAL     (MIS+MIS_C)	/* VGA Palette Registers 					*/

#define	port_out(v,p)	PM_outpb(p,v)
#define	port_outw(v,p)	PM_outpw(p,v)
#define	port_in(p)		PM_inpb(p)

/*----------------------------- Implementation ----------------------------*/

/****************************************************************************
REMARKS:
Returns the size of the VGA state buffer.
****************************************************************************/
int PMAPI PM_getVGAStateSize(void)
{
	return CRT_C + ATT_C + GRA_C + SEQ_C + MIS_C + PAL_C;
}

/****************************************************************************
REMARKS:
Delay for a short period of time.
****************************************************************************/
static void vga_delay(void)
{
	int i;
	for (i = 0; i < 10; i++)
		;
}

/****************************************************************************
REMARKS:
Save the color palette values
****************************************************************************/
static void vga_savepalette(
	uchar *pal)
{
	int i;

    port_out(0, PEL_IR);
    for (i = 0; i < 768; i++) {
		vga_delay();
		*pal++ = port_in(PEL_D);
        }
}

/****************************************************************************
REMARKS:
Restore the color palette values
****************************************************************************/
static void vga_restorepalette(
	const uchar *pal)
{
    int i;

    /* restore saved palette */
	port_out(0, PEL_IW);
	for (i = 0; i < 768; i++) {
		vga_delay();
		port_out(*pal++, PEL_D);
		}
}

/****************************************************************************
REMARKS:
Save the state of all VGA compatible registers
****************************************************************************/
void PMAPI PM_saveVGAState(
	void *stateBuf)
{
	uchar 	*regs = stateBuf;
    int 	i;

	/* Save state of VGA registers */
    for (i = 0; i < CRT_C; i++) {
		port_out(i, CRT_I);
		regs[CRT + i] = port_in(CRT_D);
    	}
	for (i = 0; i < ATT_C; i++) {
		port_in(IS1_R);
		vga_delay();
		port_out(i, ATT_IW);
		vga_delay();
		regs[ATT + i] = port_in(ATT_R);
		vga_delay();
    	}
    for (i = 0; i < GRA_C; i++) {
		port_out(i, GRA_I);
		regs[GRA + i] = port_in(GRA_D);
    	}
    for (i = 0; i < SEQ_C; i++) {
		port_out(i, SEQ_I);
		regs[SEQ + i] = port_in(SEQ_D);
    	}
    regs[MIS] = port_in(MIS_R);

    /* Save the VGA palette values */
    vga_savepalette(&regs[PAL]);

    /* Turn the VGA display back on */
	PM_vgaUnblankDisplay();
}

/****************************************************************************
REMARKS:
Retore the state of all VGA compatible registers
****************************************************************************/
void PMAPI PM_restoreVGAState(
	const void *stateBuf)
{
	const uchar	*regs = stateBuf;
    int 		i;

    /* Restore the state of the VGA compatible registers */
    port_out(regs[MIS], MIS_W);
    port_outw(0x0100,SEQ_I);
    port_out(1, SEQ_I);
    port_out(regs[SEQ + 1] | 0x20, SEQ_D);
    for (i = 2; i < SEQ_C; i++) {
		port_out(i, SEQ_I);
		port_out(regs[SEQ + i], SEQ_D);
    	}
    port_outw(0x0300,SEQ_I);

	/* Deprotect CRT registers 0-7 and write CRTC */
	port_out(0x11, CRT_I);
	port_out(port_in(CRT_D) & 0x7F, CRT_D);
    for (i = 0; i < CRT_C; i++) {
		port_out(i, CRT_I);
		port_out(regs[CRT + i], CRT_D);
    	}
    for (i = 0; i < GRA_C; i++) {
		port_out(i, GRA_I);
		port_out(regs[GRA + i], GRA_D);
    	}
    for (i = 0; i < ATT_C; i++) {
		port_in(IS1_R);		/* reset flip-flop */
		vga_delay();
		port_out(i, ATT_IW);
		vga_delay();
		port_out(regs[ATT + i], ATT_IW);
		vga_delay();
    	}

    /* Restore the VGA palette values */
    vga_restorepalette(&regs[PAL]);

    /* Ensure the VGA screen is turned on */
	PM_vgaUnblankDisplay();
}

/****************************************************************************
REMARKS:
Disables the VGA display for screen output making it blank.
****************************************************************************/
void PMAPI PM_vgaBlankDisplay(void)
{
	/* Turn screen off */
	port_out(0x01, SEQ_I);
	port_out(port_in(SEQ_D) | 0x20, SEQ_D);

	/* Disable video output */
    port_in(IS1_R);
    vga_delay();
     port_out(0x00, ATT_IW);
}

/****************************************************************************
REMARKS:
Enables the VGA display for screen output.
****************************************************************************/
void PMAPI PM_vgaUnblankDisplay(void)
{
	/* Turn screen back on */
	port_out(0x01, SEQ_I);
	port_out(port_in(SEQ_D) & 0xDF, SEQ_D);

	/* Enable video output */
    port_in(IS1_R);
    vga_delay();
    port_out(0x20, ATT_IW);
}
