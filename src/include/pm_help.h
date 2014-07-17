/****************************************************************************
*
*                          SciTech Display Doctor
*
*               Copyright (C) 1991-1998 SciTech Software, Inc.
*                            All rights reserved.             
*
*  ======================================================================
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  |                                                                    |
*  |This copyrighted computer code is a proprietary trade secret of     |
*  |SciTech Software, Inc., located at 505 Wall Street, Chico, CA 95928 |
*  |USA (www.scitechsoft.com).  ANY UNAUTHORIZED POSSESSION, USE,       |
*  |VIEWING, COPYING, MODIFICATION OR DISSEMINATION OF THIS CODE IS     |
*  |STRICTLY PROHIBITED BY LAW.  Unless you have current, express       |
*  |written authorization from SciTech to possess or use this code, you |
*  |may be subject to civil and/or criminal penalties.                  |
*  |                                                                    |
*  |If you received this code in error or you would like to report      |
*  |improper use, please immediately contact SciTech Software, Inc. at  |
*  |530-894-8400.                                                       |
*  |                                                                    |
*  |REMOVAL OR MODIFICATION OF THIS HEADER IS STRICTLY PROHIBITED BY LAW|
*  ======================================================================
*
* Language:		ANSI C
* Environment:	Win32, OS/2
*
* Description:	Include file for the SciTech Portability Manager 32-bit
*				helper VxD for Windows 9x for and the 16-bit ring 0
*				helper device driver for OS/2.
*
*				This file documents all the public services used by the
*				SciTech Portability Manager library and SciTech Nucleus
*				loader library.
*
****************************************************************************/

#ifndef	__PMHELP_H
#define	__PMHELP_H

/* Include version information */

#include "sdd/sddver.h"
#define PMHELP_Major			SDD_RELEASE_MAJOR
#define PMHELP_Minor			SDD_RELEASE_MINOR
#define	PMHELP_VERSION			((PMHELP_Major << 8) | PMHELP_Minor)

#ifdef	__OS2__

/****************************************************************************
* Public OS/2 Support functions
****************************************************************************/

#include "scitech.h"
#include "nucleus/graphics.h"

#define	PMHELP_NAME				(PSZ)"sddhelp$"
#define	PMHELP_IOCTL			0x0080	/* Used by PM library 		*/
#define	PMHELP_GETVER			0x0001	/* Used by PM library 		*/
#define	PMHELP_MAPPHYS			0x0002	/* Used by PM library 		*/
#define	PMHELP_ALLOCLOCKED		0x0003	/* Used by PM library 		*/
#define	PMHELP_FREELOCKED		0x0004	/* Used by PM library 		*/
#define PMHELP_GETGDT32			0x0005	/* Used by PM library 		*/
#define	PMHELP_ALLOCSHARED		0x0007	/* Used by PM library 		*/
#define	PMHELP_FREESHARED		0x0008	/* Used by PM library 		*/
#define	PMHELP_MAPTOPROCESS		0x0009	/* Used by PM library 		*/
#define	PMHELP_FREEPHYS			0x000A	/* Used by PM library 		*/
#define	PMHELP_FLUSHTLB        	0x000B  /* Used by PM library		*/
#define	PMHELP_SAVECR4         	0x000C  /* Used by PM library		*/
#define	PMHELP_RESTORECR4      	0x000D  /* Used by PM library		*/
#define	PMHELP_READMSR         	0x000E  /* Used by PM library		*/
#define	PMHELP_WRITEMSR        	0x000F  /* Used by PM library		*/
#define	PMHELP_GETSHAREDINFO	0x0040	/* Used by Nucleus library	*/
#define	PMHELP_SETSHAREDINFO	0x0041	/* Used by Nucleus library	*/

#else	/* !__OS2__ */

/****************************************************************************
* Public DOS/Windows Support functions
****************************************************************************/

#ifdef	DEVICE_MAIN
#include <vtoolsc.h>
#define PMHELP_Init_Order	(VDD_INIT_ORDER-1)
#define	RETURN_LONGS(n)		*p->dioc_bytesret = (n) * sizeof(ulong)
#endif	/* DEVICE_MAIN */
#include "scitech.h"
#include "nucleus/graphics.h"

/* We connect to the SDDHELP.VXD module if it is staticly loaded (as part
 * of SciTech Display Doctor), otherwise we dynamically load the PMHELP.VXD
 * public helper VxD.
 */

#define PMHELP_DeviceID    		0x0000
#define	SDDHELP_DeviceID		0x3DF8
#define VXDLDR_DeviceID 		0x0027
#define	SDDHELP_MODULE			"SDDHELP"
#define	SDDHELP_NAME			"SDDHELP.VXD"
#define	PMHELP_NAME				"PMHELP.VXD"
#define PMHELP_DDBNAME			"pmhelp  "
#define	PMHELP_MODULE			"PMHELP"
#define	SDDHELP_VXD_MODULE		"\\\\.\\" SDDHELP_MODULE
#define	PMHELP_VXD_NAME			"\\\\.\\" PMHELP_NAME

/* V86 and PM entry point services */

#define	PMHELP_GETVER						0x0000	/* Used by PM/WinDirect		*/
#define	PMHELP_RDREGB  						0x0003	/* 16-bit DOS TSR (obsolete)*/
#define	PMHELP_WRREGB						0x0004	/* 16-bit DOS TSR (obsolete)*/
#define	PMHELP_RDREGW  						0x0005  /* 16-bit DOS TSR (obsolete)*/
#define	PMHELP_WRREGW						0x0006  /* 16-bit DOS TSR (obsolete)*/
#define	PMHELP_RDREGL  						0x0008  /* 16-bit DOS TSR (obsolete)*/
#define	PMHELP_WRREGL						0x0009  /* 16-bit DOS TSR (obsolete)*/
#define	PMHELP_MAPPHYS						0x000F  /* Used by PM/WinDirect		*/
#define	PMHELP_GETVESABUF					0x0013  /* Used by PM/WinDirect		*/
#define	PMHELP_DPMIINT86					0x0014	/* Used by PM library		*/
#define	PMHELP_INT86						0x0015	/* Used by PM library 		*/
#define	PMHELP_INT86X						0x0016	/* Used by PM library		*/
#define	PMHELP_CALLREALMODE					0x0017	/* Used by PM library		*/
#define	PMHELP_ALLOCLOCKED					0x0018	/* Used by PM library		*/
#define	PMHELP_FREELOCKED					0x0019	/* Used by PM library		*/
#define	PMHELP_ENABLELFBCOMB				0x001A	/* Used by PM library		*/
#define	PMHELP_GETPHYSICALADDR				0x001B	/* Used by PM library		*/
#define	PMHELP_MALLOCSHARED					0x001D	/* Used by PM library		*/
#define	PMHELP_MAPSHARED					0x001E	/* Used by PM library		*/
#define	PMHELP_FREESHARED					0x001F	/* Used by PM library		*/
#define	PMHELP_LOCKDATAPAGES				0x0020	/* Used by PM library		*/
#define	PMHELP_UNLOCKDATAPAGES				0x0021	/* Used by PM library		*/
#define	PMHELP_LOCKCODEPAGES				0x0022	/* Used by PM library		*/
#define	PMHELP_UNLOCKCODEPAGES				0x0023	/* Used by PM library		*/
#define	PMHELP_GETCALLGATE					0x0024	/* Used by PM library		*/
#define	PMHELP_SETCNTPATH					0x0025	/* Used by PM library		*/

#define	PMHELP_GETSHAREDINFO				0x0200  /* Used by Nucleus library 	*/
#define	PMHELP_GETMEMSIZE					0x0201  /* Used by Nucleus library 	*/
#define	PMHELP_GASTATUS						0x0202  /* Used by Nucleus library 	*/
#define	PMHELP_GAERRORMSG					0x0203  /* Used by Nucleus library 	*/
#define	PMHELP_GAGETDAYSLEFT				0x0204  /* Used by Nucleus library 	*/
#define	PMHELP_GAREGISTERLICENSE			0x0205  /* Used by Nucleus library 	*/
#define	PMHELP_GAFORCEMEMSIZE				0x0206  /* Used by Nucleus library 	*/
#define	PMHELP_GAENUMERATEDEVICES			0x0207  /* Used by Nucleus library 	*/
#define	PMHELP_GALOADDRIVER					0x0208  /* Used by Nucleus library 	*/
#define	PMHELP_GASETACTIVEDEVICE			0x0209  /* Used by Nucleus library 	*/
#define PMHELP_GAUNLOADDRIVER				0x020A  /* Used by Nucleus library 	*/
#define PMHELP_GALOADREF2D					0x020B  /* Used by Nucleus library 	*/
#define PMHELP_GAUNLOADREF2D				0x020C  /* Used by Nucleus library 	*/
#define PMHELP_GASTEREOINIT      			0x020D  /* Used by Nucleus library 	*/
#define	PMHELP_GASTEREOON        			0x020E  /* Used by Nucleus library 	*/
#define	PMHELP_GASTEREOFLIP      			0x020F  /* Used by Nucleus library 	*/
#define	PMHELP_GASTEREOFLIPSTATUS			0x0210  /* Used by Nucleus library 	*/
#define	PMHELP_GASTEREOWAITFLIP				0x0211  /* Used by Nucleus library 	*/
#define	PMHELP_GASTEREOOFF					0x0212  /* Used by Nucleus library 	*/
#define	PMHELP_GASTEREOEXIT					0x0213  /* Used by Nucleus library 	*/
#define	PMHELP_GASAVEMODEPROFILE			0x0214  /* Used by Nucleus library 	*/
#define	PMHELP_GASAVEOPTIONS        		0x0215  /* Used by Nucleus library 	*/
#define	PMHELP_GASAVECRTCTIMINGS    		0x0216  /* Used by Nucleus library 	*/
#define	PMHELP_GARESTORECRTCTIMINGS 		0x0217  /* Used by Nucleus library 	*/
#define	PMHELP_DDCINIT						0x0218  /* Used by Nucleus library 	*/
#define	PMHELP_DDCREADEDID              	0x0219  /* Used by Nucleus library 	*/
#define	PMHELP_EDIDPARSE                	0x021A  /* Used by Nucleus library 	*/
#define	PMHELP_MCSBEGIN                 	0x021B  /* Used by Nucleus library 	*/
#define	PMHELP_MCSGETCAPABILITIESSTRING 	0x021C  /* Used by Nucleus library 	*/
#define	PMHELP_MCSISCONTROLSUPPORTED    	0x021D  /* Used by Nucleus library 	*/
#define	PMHELP_MCSENABLECONTROL         	0x021E  /* Used by Nucleus library 	*/
#define	PMHELP_MCSGETCONTROLMAX         	0x021F  /* Used by Nucleus library 	*/
#define	PMHELP_MCSGETCONTROLVALUE       	0x0220  /* Used by Nucleus library 	*/
#define	PMHELP_MCSGETCONTROLVALUES      	0x0221  /* Used by Nucleus library 	*/
#define	PMHELP_MCSSETCONTROLVALUE       	0x0222  /* Used by Nucleus library 	*/
#define	PMHELP_MCSSETCONTROLVALUES      	0x0223  /* Used by Nucleus library 	*/
#define	PMHELP_MCSRESETCONTROL          	0x0224  /* Used by Nucleus library 	*/
#define	PMHELP_MCSSAVECURRENTSETTINGS   	0x0225  /* Used by Nucleus library 	*/
#define	PMHELP_MCSGETTIMINGREPORT       	0x0226  /* Used by Nucleus library 	*/
#define	PMHELP_MCSGETSELFTESTREPORT     	0x0227  /* Used by Nucleus library 	*/
#define	PMHELP_MCSEND                   	0x0228  /* Used by Nucleus library 	*/

#define PMHELP_GAGETOPTIONS                	0x0310	/* Nucleus API thunks */
#define PMHELP_GASETOPTIONS                	0x0311	/* Nucleus API thunks */
#define	PMHELP_GAGETVIDEOMODEINFO			0x0312
#define PMHELP_GASETVIDEOMODE              	0x0313
#define	PMHELP_GAGETVIDEOMODE				0x0314
#define	PMHELP_GAGETCUSTOMVIDEOMODE			0x0315
#define PMHELP_GASETCUSTOMVIDEOMODE        	0x0316
#define PMHELP_GASETCRTCTIMINGS            	0x0317
#define	PMHELP_GASAVERESTORESTATE			0x0318
#define PMHELP_GASETDISPLAYOUTPUT          	0x0319
#define PMHELP_GAGETDISPLAYOUTPUT          	0x031A

#define PMHELP_GASETBANK					0x0330
#define PMHELP_GASETDISPLAYSTART			0x0331
#define PMHELP_GASETDISPLAYSTARTXY         	0x0332
#define PMHELP_GASETSTEREODISPLAYSTART     	0x0333
#define PMHELP_GAGETDISPLAYSTARTSTATUS     	0x0334
#define PMHELP_GAENABLESTEREOMODE          	0x0335
#define PMHELP_GAISVSYNC                   	0x0336
#define PMHELP_GAWAITVSYNC                 	0x0337
#define PMHELP_GASETVSYNCWIDTH             	0x0338
#define PMHELP_GAGETVSYNCWIDTH             	0x0339
#define PMHELP_GASETPALETTEDATA            	0x033A
#define PMHELP_GAGETPALETTEDATA            	0x033B
#define PMHELP_GASETGAMMACORRECTDATA       	0x033C
#define PMHELP_GAGETGAMMACORRECTDATA       	0x033D
#define PMHELP_GAGETCURRENTSCANLINE        	0x033E
#define PMHELP_GASETPALETTEDATAEXT          0x033F
#define PMHELP_GAGETPALETTEDATAEXT          0x0340
#define PMHELP_GASETGAMMACORRECTDATAEXT     0x0341
#define PMHELP_GAGETGAMMACORRECTDATAEXT   	0x0342

/* Win32 DeviceIoControl entry point services */

#define	PMHELP_GETVER32						(0x80000000UL | PMHELP_GETVER)
#define	PMHELP_MAPPHYS32					(0x80000000UL | PMHELP_MAPPHYS)
#define	PMHELP_GETVESABUF32					(0x80000000UL | PMHELP_GETVESABUF)
#define	PMHELP_DPMIINT8632         			(0x80000000UL | PMHELP_DPMIINT86)
#define	PMHELP_INT8632             			(0x80000000UL | PMHELP_INT86)
#define	PMHELP_INT86X32            			(0x80000000UL | PMHELP_INT86X)
#define	PMHELP_CALLREALMODE32      			(0x80000000UL | PMHELP_CALLREALMODE)
#define	PMHELP_ALLOCLOCKED32       			(0x80000000UL | PMHELP_ALLOCLOCKED)
#define	PMHELP_FREELOCKED32					(0x80000000UL | PMHELP_FREELOCKED)
#define	PMHELP_ENABLELFBCOMB32				(0x80000000UL | PMHELP_ENABLELFBCOMB)
#define PMHELP_GETPHYSICALADDR32			(0x80000000UL | PMHELP_GETPHYSICALADDR)
#define	PMHELP_MALLOCSHARED32				(0x80000000UL | PMHELP_MALLOCSHARED)
#define	PMHELP_MAPSHARED32                  (0x80000000UL | PMHELP_MAPSHARED)
#define	PMHELP_FREESHARED32                 (0x80000000UL | PMHELP_FREESHARED)
#define	PMHELP_LOCKDATAPAGES32              (0x80000000UL | PMHELP_LOCKDATAPAGES)
#define	PMHELP_UNLOCKDATAPAGES32            (0x80000000UL | PMHELP_UNLOCKDATAPAGES)
#define	PMHELP_LOCKCODEPAGES32              (0x80000000UL | PMHELP_LOCKCODEPAGES)
#define	PMHELP_UNLOCKCODEPAGES32            (0x80000000UL | PMHELP_UNLOCKCODEPAGES)
#define	PMHELP_GETCALLGATE32				(0x80000000UL | PMHELP_GETCALLGATE)
#define	PMHELP_SETCNTPATH32					(0x80000000UL | PMHELP_SETCNTPATH)

#define	PMHELP_GETSHAREDINFO32				(0x80000000UL | PMHELP_GETSHAREDINFO)
#define	PMHELP_GETMEMSIZE32					(0x80000000UL | PMHELP_GETMEMSIZE)
#define PMHELP_GASTATUS32					(0x80000000UL | PMHELP_GASTATUS)
#define PMHELP_GAERRORMSG32                 (0x80000000UL | PMHELP_GAERRORMSG)
#define PMHELP_GAGETDAYSLEFT32              (0x80000000UL | PMHELP_GAGETDAYSLEFT)
#define PMHELP_GAREGISTERLICENSE32          (0x80000000UL | PMHELP_GAREGISTERLICENSE)
#define PMHELP_GAFORCEMEMSIZE32             (0x80000000UL | PMHELP_GAFORCEMEMSIZE)
#define PMHELP_GAENUMERATEDEVICES32         (0x80000000UL | PMHELP_GAENUMERATEDEVICES)
#define PMHELP_GALOADDRIVER32               (0x80000000UL | PMHELP_GALOADDRIVER)
#define PMHELP_GASETACTIVEDEVICE32          (0x80000000UL | PMHELP_GASETACTIVEDEVICE)
#define PMHELP_GAUNLOADDRIVER32             (0x80000000UL | PMHELP_GAUNLOADDRIVER)
#define PMHELP_GALOADREF2D32                (0x80000000UL | PMHELP_GALOADREF2D)
#define PMHELP_GAUNLOADREF2D32              (0x80000000UL | PMHELP_GAUNLOADREF2D)
#define PMHELP_GASTEREOINIT32               (0x80000000UL | PMHELP_GASTEREOINIT)
#define PMHELP_GASTEREOON32                 (0x80000000UL | PMHELP_GASTEREOON)
#define PMHELP_GASTEREOFLIP32               (0x80000000UL | PMHELP_GASTEREOFLIP)
#define PMHELP_GASTEREOFLIPSTATUS32         (0x80000000UL | PMHELP_GASTEREOFLIPSTATUS)
#define PMHELP_GASTEREOWAITFLIP32           (0x80000000UL | PMHELP_GASTEREOWAITFLIP)
#define PMHELP_GASTEREOOFF32                (0x80000000UL | PMHELP_GASTEREOOFF)
#define PMHELP_GASTEREOEXIT32               (0x80000000UL | PMHELP_GASTEREOEXIT)
#define PMHELP_GASAVEMODEPROFILE32          (0x80000000UL | PMHELP_GASAVEMODEPROFILE)
#define PMHELP_GASAVEOPTIONS32              (0x80000000UL | PMHELP_GASAVEOPTIONS)
#define PMHELP_GASAVECRTCTIMINGS32          (0x80000000UL | PMHELP_GASAVECRTCTIMINGS)
#define PMHELP_GARESTORECRTCTIMINGS32		(0x80000000UL | PMHELP_GARESTORECRTCTIMINGS)
#define PMHELP_DDCINIT32					(0x80000000UL | PMHELP_DDCINIT)
#define PMHELP_DDCREADEDID32                (0x80000000UL | PMHELP_DDCREADEDID)
#define PMHELP_EDIDPARSE32                  (0x80000000UL | PMHELP_EDIDPARSE)
#define PMHELP_MCSBEGIN32                   (0x80000000UL | PMHELP_MCSBEGIN)
#define PMHELP_MCSGETCAPABILITIESSTRING32   (0x80000000UL | PMHELP_MCSGETCAPABILITIESSTRING)
#define PMHELP_MCSISCONTROLSUPPORTED32      (0x80000000UL | PMHELP_MCSISCONTROLSUPPORTED)
#define PMHELP_MCSENABLECONTROL32           (0x80000000UL | PMHELP_MCSENABLECONTROL)
#define PMHELP_MCSGETCONTROLMAX32           (0x80000000UL | PMHELP_MCSGETCONTROLMAX)
#define PMHELP_MCSGETCONTROLVALUE32         (0x80000000UL | PMHELP_MCSGETCONTROLVALUE)
#define PMHELP_MCSGETCONTROLVALUES32        (0x80000000UL | PMHELP_MCSGETCONTROLVALUES)
#define PMHELP_MCSSETCONTROLVALUE32         (0x80000000UL | PMHELP_MCSSETCONTROLVALUE)
#define PMHELP_MCSSETCONTROLVALUES32        (0x80000000UL | PMHELP_MCSSETCONTROLVALUES)
#define PMHELP_MCSRESETCONTROL32            (0x80000000UL | PMHELP_MCSRESETCONTROL)
#define PMHELP_MCSSAVECURRENTSETTINGS32     (0x80000000UL | PMHELP_MCSSAVECURRENTSETTINGS)
#define PMHELP_MCSGETTIMINGREPORT32         (0x80000000UL | PMHELP_MCSGETTIMINGREPORT)
#define PMHELP_MCSGETSELFTESTREPORT32       (0x80000000UL | PMHELP_MCSGETSELFTESTREPORT)
#define PMHELP_MCSEND32                     (0x80000000UL | PMHELP_MCSEND)

#define PMHELP_GASETOPTIONS32              	(0x80000000UL | PMHELP_GASETOPTIONS)
#define PMHELP_GASETVIDEOMODE32            	(0x80000000UL | PMHELP_GASETVIDEOMODE)
#define PMHELP_GASETCUSTOMVIDEOMODE32      	(0x80000000UL | PMHELP_GASETCUSTOMVIDEOMODE)
#define PMHELP_GASETCRTCTIMINGS32          	(0x80000000UL | PMHELP_GASETCRTCTIMINGS)
#define	PMHELP_GASAVERESTORESTATE32			(0x80000000UL | PMHELP_GASAVERESTORESTATE)
#define PMHELP_GASETDISPLAYOUTPUT32        	(0x80000000UL | PMHELP_GASETDISPLAYOUTPUT)

#define PMHELP_GASETBANK32					(0x80000000UL | PMHELP_GASETBANK)
#define PMHELP_GASETDISPLAYSTART32         	(0x80000000UL | PMHELP_GASETDISPLAYSTART)
#define PMHELP_GASETDISPLAYSTARTXY32       	(0x80000000UL | PMHELP_GASETDISPLAYSTARTXY)
#define PMHELP_GASETSTEREODISPLAYSTART32   	(0x80000000UL | PMHELP_GASETSTEREODISPLAYSTART)
#define PMHELP_GAGETDISPLAYSTARTSTATUS32   	(0x80000000UL | PMHELP_GAGETDISPLAYSTARTSTATUS)
#define PMHELP_GAENABLESTEREOMODE32        	(0x80000000UL | PMHELP_GAENABLESTEREOMODE)
#define PMHELP_GAISVSYNC32                 	(0x80000000UL | PMHELP_GAISVSYNC)
#define PMHELP_GAWAITVSYNC32               	(0x80000000UL | PMHELP_GAWAITVSYNC)
#define PMHELP_GASETVSYNCWIDTH32           	(0x80000000UL | PMHELP_GASETVSYNCWIDTH)
#define PMHELP_GAGETVSYNCWIDTH32           	(0x80000000UL | PMHELP_GAGETVSYNCWIDTH)
#define PMHELP_GASETPALETTEDATA32          	(0x80000000UL | PMHELP_GASETPALETTEDATA)
#define PMHELP_GAGETPALETTEDATA32          	(0x80000000UL | PMHELP_GAGETPALETTEDATA)
#define PMHELP_GASETGAMMACORRECTDATA32     	(0x80000000UL | PMHELP_GASETGAMMACORRECTDATA)
#define PMHELP_GAGETGAMMACORRECTDATA32     	(0x80000000UL | PMHELP_GAGETGAMMACORRECTDATA)
#define PMHELP_GAGETCURRENTSCANLINE32      	(0x80000000UL | PMHELP_GAGETCURRENTSCANLINE)
#define PMHELP_GASETPALETTEDATAEXT32        (0x80000000UL | PMHELP_GASETPALETTEDATAEXT)
#define PMHELP_GAGETPALETTEDATAEXT32        (0x80000000UL | PMHELP_GAGETPALETTEDATAEXT)
#define PMHELP_GASETGAMMACORRECTDATAEXT32   (0x80000000UL | PMHELP_GASETGAMMACORRECTDATAEXT)
#define PMHELP_GAGETGAMMACORRECTDATAEXT32  	(0x80000000UL | PMHELP_GAGETGAMMACORRECTDATAEXT)

/* For real mode VxD calls, we put the function number into the high
 * order word of EAX, and a value of 0x4FFF in AX. This allows our
 * VxD handler which is set up to handle Int 10's to recognise a native
 * SDDHELP API call from a real mode DOS program.
 */

#ifdef	REALMODE
#define	API_NUM(num)	(((ulong)(num) << 16) | 0x4FFF)
#else
#define	API_NUM(num)	(num)
#endif

/* Function prototypes for Nucleus function pointers */

typedef GA_devCtx * (NAPIP GA_loadDriver_t)(N_int32 deviceIndex,N_int32 shared);
typedef ibool		(NAPIP QueryFunctions_t)(N_uint32 id,N_int32 safetyLevel,void _FAR_ *funcs);

#endif	/* !__OS2__ */

#endif  /* __PMHELP_H */
