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
* Description:	Header file for the disk based resource file structures
*				used by the MGL.
*
****************************************************************************/

#ifndef	__RESOURCE_H
#define	__RESOURCE_H

/*---------------------- Macro and type definitions -----------------------*/

#pragma pack(1)				/* Pack structures to byte granularity		*/

/* Windows .CUR header and file structures */

typedef struct {
	uchar	bWidth;
	uchar	bHeight;
	uchar	bColorCount;
	uchar	bReserved;
	uchar	wXHotSpot[2];
	uchar	wYHotSpot[2];
	uchar	dwBytesInRes[4];
	uchar	dwImageOffset[4];
	} winCURSORDIRENTRY;

typedef struct {
	uchar				chReserved[2];
	uchar				chType[2];
	uchar				chCount[2];
	winCURSORDIRENTRY	chEntries[1];
	} winCURSORHEADER;

/* Windows .BMP header and file structures */

typedef struct {
	uchar   bfType[2];
	uchar	bfSize[4];
	uchar   bfRes[4];
	uchar	bfOffBits[4];
	} winBITMAPFILEHEADER;

typedef struct {
	uchar	biSize[4];
	uchar	biWidth[4];
	uchar	biHeight[4];
	uchar   biPlanes[2];
	uchar	biBitCount[2];
	uchar	biCompression[4];
	uchar	biSizeImage[4];
	uchar	biXPelsPerMeter[4];
	uchar	biYPelsPerMeter[4];
	uchar	biClrUsed[4];
	uchar	biClrImportant[4];
	} winBITMAPINFOHEADER;

typedef struct {
	ulong   biSize;
	long    biWidth;
	long    biHeight;
	ushort  biPlanes;
	ushort  biBitCount;
	ulong   biCompression;
	ulong   biSizeImage;
	long    biXPelsPerMeter;
	long    biYPelsPerMeter;
	ulong   biClrUsed;
	ulong	biClrImportant;
	} winBITMAPINFOHEADER_full;

typedef struct {
	uchar	rgbBlue;
	uchar   rgbGreen;
	uchar   rgbRed;
	uchar  	rgbReserved;
	} winRGBQUAD;

typedef struct {
	winBITMAPINFOHEADER_full	header;
	winRGBQUAD					colors[256];
	} winBITMAPINFO;

typedef struct {
	winBITMAPINFOHEADER_full	header;
	ulong						maskRed;
	ulong						maskGreen;
	ulong						maskBlue;
	} winBITMAPINFOEXT;

/* Constants for the biCompression field */

#define winBI_RGB      	0L
#define winBI_RLE8     	1L
#define winBI_RLE4     	2L
#define	winBI_BITFIELDS	3L

/* Windows .ICO header and file structures */

typedef struct {
	uchar	bWidth;
	uchar	bHeight;
	uchar	bColorCount;
	uchar	bRes;
	uchar	wRes1[2];
	uchar	wRes2[2];
	uchar	dwBytesInRes[4];
	uchar	dwImageOffset[4];
	} winICONDIRECTORY;

typedef struct {
	uchar				ihRes[2];
	uchar				ihType[2];
	uchar				ihCount[2];
	winICONDIRECTORY	ihEntries[1];
	} winICONHEADER;

/* Windows 2.x .FNT header and file structures */

typedef struct {
	uchar	dfVersion[2];
	uchar	dfSize[4];
	uchar	dfCopyright[60];
	uchar	dfType[2];
	} winFONTHEADER;

typedef struct {
	uchar 	dfPoints[2];
	uchar 	dfVertRes[2];
	uchar   dfHorizRes[2];
	uchar   dfAscent[2];
	uchar   dfInternalLeading[2];
	uchar 	dfExternalLeading[2];
	} winFONTSPACING;

typedef struct {
	uchar	dfItalic;
	uchar   dfUnderline;
	uchar   dfStrikeOut;
	uchar   dfWeight[2];
	uchar   dfCharSet;
	uchar 	dfPixWidth[2];
	uchar 	dfPixHeight[2];
	uchar	dfPitchAndFamily;
	uchar 	dfAvgWidth[2];
	uchar 	dfMaxWidth[2];
	} winFONTSTYLE;

typedef struct {
	uchar	dfFirstChar;
	uchar	dfLastChar;
	uchar	dfDefaultChar;
	uchar	dfBreakChar;
	} winFONTCHAR;

typedef struct {
	uchar   dfWidthBytes[2];
	uchar 	dfDevice[4];
	uchar 	dfFace[4];
	uchar 	dfBitsPointer[4];
	uchar 	dfBitsOffset[4];
	uchar  	dfRes1;
	/* Variable length font info resides in here */
	} winFONTMISC;

typedef struct {
	winFONTHEADER	fh;
	winFONTSPACING	fs;
	winFONTSTYLE	ft;
	winFONTCHAR		fc;
	winFONTMISC		fm;
	} winFONTINFO;

typedef struct {
	uchar	gWidth[2];
	uchar	gOffset[2];
	} winGLYPH;

/* Executable File Format Information */

typedef struct {
	uchar	exSignature[2];
	uchar	exExtraBytes[2];
	uchar	exPages[2];
	uchar	exRelocItems[2];
	uchar	exHeaderSize[2];
	uchar	exMinAlloc[2];
	uchar	exMaxAlloc[2];
	uchar	exInitSS[2];
	uchar	exInitSP[2];
	uchar	exCheckSum[2];
	uchar	exInitIP[2];
	uchar	exInitCS[2];
	uchar	exRelocTable[2];
	uchar	exOverlay[2];
	uchar	reserved1[4];
	uchar	reserved2[28];
	uchar	winInfoOffset[2];
	uchar	wreserved[2];
	uchar	msdosStub[1];
	} winOLDHEADER;

typedef struct {
	uchar			signature[2];
	uchar			linkerVersion;
	uchar			linkerRevision;
	uchar			entryTabOffset[2];
	uchar			entryTabLen[2];
	uchar			reserved1[4];
	uchar			exeFlags[2];
	uchar			dataSegNum[2];
	uchar			localHeapSize[2];
	uchar			stackSize[2];
	uchar			cs_ip[4];
	uchar			ss_sp[4];
	uchar			segTabEntries[2];
	uchar			modTabEntries[2];
	uchar			nonResTabSize[2];
	uchar			segTabOffset[2];
	uchar			resTabOffset[2];
	uchar			resNameTabOffset[2];
	uchar			modTabOffset[2];
	uchar			impTabOffset[2];
	uchar			nonResTabOffset[2];
	uchar			reserved2[2];
	uchar			numEntryPoints[2];
	uchar			numResourceSegs[2];
	uchar			targetOS;
	uchar			miscFlags;
	uchar			fastLoadOffset[2];
	uchar			fastLoadSize[2];
	uchar			reserved3[2];
	uchar			winRevision;
	uchar			winVersion;
	} winWININFO;

typedef struct {
	uchar			rnOffset[2];
	uchar			rnLength[2];
	uchar			rnFlags[2];
	uchar			rnID[2];
	uchar			reserved[4];
	} winNAMEINFO;

typedef struct {
	uchar			rtTypeID[2];
	uchar			rtResourceCount[2];
	uchar			rtReserved[4];
	/* Array of winNAMEINFO Structures */
	} winTYPEINFO;

/* MGL 1.x font file header structure */

typedef struct {
	char	ident[21];			/* Font file identification string		*/
	char	name[_MGL_FNAMESIZE];/* Font's name							*/
	char	copyright[175];		/* Copyright notice etc					*/
	char	majorversion;		/* Major version number					*/
	char	minorversion;		/* Minor version number					*/
	} font_header;

/* Identification flag for font files */

#define	FONT_IDENT	"MegaGraph Font File:"

#define	_MAXVEC		256			/* Number of characters in font defn	*/

/* MGL 1.x Vector Font structure */

typedef struct {
	char			name[_MGL_FNAMESIZE];/* Name of the font			*/
	short			fontType;		/* Type of font						*/
	short			maxWidth;		/* Maximum character width			*/
	short			maxKern;		/* Maximum character kern			*/
	short			fontWidth;		/* Font width						*/
	short			fontHeight;		/* Font height						*/
	short			ascent;			/* Font ascent value				*/
	short			descent;		/* Font descent value				*/
	short			leading;		/* Font leading value				*/
	uchar			width[_MAXVEC];	/* Character width table			*/
	short			offset[_MAXVEC];/* Offset table into vector defn.	*/
	uchar			def[1];			/* Vector definitions for font		*/
	} vec_font;

/* Missing symbol for vector fonts is index 0 */

#define	VEC_MISSINGSYMBOL(f) 0

/* MGL 1.x bitmap font structure */

typedef struct {
	char			name[_MGL_FNAMESIZE];/* Name of the font			*/
	short			fontType;		/* Type of font						*/
	short			maxWidth;		/* Maximum character width			*/
	short			maxKern;		/* Maximum character kern			*/
	short			fontWidth;		/* Font width						*/
	short			fontHeight;		/* Font height						*/
	short			ascent;			/* Font ascent value				*/
	short			descent;		/* Font descent value				*/
	short			leading;		/* Font leading value				*/
	uchar			width[_MAXVEC];	/* Character width table			*/
	short			offset[_MAXVEC];/* Offset table into character defn.*/
	uchar			iwidth[_MAXVEC];/* Character image width table		*/
	uchar			loc[_MAXVEC];	/* Character location table			*/
	uchar			def[1];			/* Bitmap definitions for font		*/
	} bitmap_font_old;

/* PCX file format header information */

typedef struct {
	char	format;						/* Always 10 for PCX		*/
	char	version;					/* Version info				*/
	char    rleflag;					/* Set to 1					*/
	char	bitpx;						/* Bits per pixel			*/
	uchar	x1[2],y1[2],x2[2],y2[2];	/* Image bounds in pixels	*/
	uchar	hres[2],vres[2];			/* Image resolution in dpi	*/
	uchar	colors[48];					/* Palette					*/
	char	vmode;						/* (ignored)				*/
	char	nplanes;					/* Plane count (v2.5=0)		*/
	uchar  	bplin[2];					/* Bytes per scanline		*/
	uchar	paltype;					/* 1 for color, 2 for gray	*/
	uchar	res1;						/* Reserved (0)				*/
	uchar 	scrnw[2],scrnh[2];			/* Screen size in pixels	*/
	char	extra[54];					/* Pad to 128 bytes			*/
	} PCXHEADER;

#define	PCX_RUN_MASK	0xC0			/* Mask top 2 bits			*/
#define	PCX_CNT_MASK	0x3F			/* Mask out pixel count		*/
#define	PCX_MAX_RUN		0x3F			/* Maxium repeat count		*/

/* Macros for extracting values from resource file structures in little
 * endian or big endian format. We define the following macros:
 *
 *	getLEShort()	- Get a short in little endian format
 *	getLELong()		- Get a long in little endian format
 *	getBEShort()	- Get a short in big endian format
 *	getBESHory()	- Get a long in big endian format
 *
 *	putLEShort()	- Put a short in little endian format
 *	putLELong()		- Put a long in little endian format
 *	putBEShort()	- Put a short in big endian format
 *	putBESHory()	- Put a long in big endian format
 *
 * Note that all of these macros expect the input argument to be an
 * array of characters, although the macros will work with the argument
 * as a native short or long.
 */

#define	_TOB(v)	((uchar*)&(v))
#define	_TOS(v)	((ushort*)&(v))
#define	_TOL(v)	((ushort*)&(v))

#ifdef	__BIG_ENDIAN__
#define	getBEShort(v)	(*_TOS(v))
#define	getBELong(v)	(*_TOL(v))
#define	getLEShort(v)	((_TOS(v)[1] << 8) | _TOS(v)[0])
#define	getLELong(v)    ((_TOL(v)[3] << 24) | (_TOL(v)[2] << 16) | (_TOL(v)[1] << 8) | _TOL(v)[0])
#define	putLEShort(p,v)	(_TOB(p)[0] = (v) & 0xFF),(_TOB(p)[1] = ((v) >> 8) & 0xFF)
#define	putLELong(p,v)	(_TOB(p)[0] = (v) & 0xFF),(_TOB(p)[1] = ((v) >> 8) & 0xFF),(_TOB(p)[2] = ((v) >> 16) & 0xFF),(_TOB(p)[3] = ((v) >> 24) & 0xFF)
#define	putBEShort(p,v)	(*_TOS(p)) = (v)
#define	putBELong(p,v)	(*_TOL(p)) = (v)
#else
#define	getBEShort(v)	((_TOS(v)[1] << 8) | _TOS(v)[0])
#define	getBELong(v)    ((_TOL(v)[3] << 24) | (_TOL(v)[2] << 16) | (_TOL(v)[1] << 8) | _TOL(v)[0])
#define	getLEShort(v)	(*_TOS(v))
#define	getLELong(v)	(*_TOL(v))
#define	putLEShort(p,v)	(*_TOS(p)) = (v)
#define	putLELong(p,v)	(*_TOL(p)) = (v)
#define	putBEShort(p,v)	(_TOB(p)[0] = (v) & 0xFF),(_TOB(p)[1] = ((v) >> 8) & 0xFF)
#define	putBELong(p,v)	(_TOB(p)[0] = (v) & 0xFF),(_TOB(p)[1] = ((v) >> 8) & 0xFF),(_TOB(p)[2] = ((v) >> 16) & 0xFF),(_TOB(p)[3] = ((v) >> 24) & 0xFF)
#endif

/* Macros to convert short's and longs in place in a structure */

#define	convLEShort(v)	(v = getLEShort(v))
#define	convLELong(v)	(v = getLELong(v))

#pragma pack()				/* Return to default packing				*/

#endif	/* __RESOURCE_H */
