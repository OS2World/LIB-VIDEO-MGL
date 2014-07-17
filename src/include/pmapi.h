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
* Language:		ANSI C
* Environment:	Any
*
* Description:	Header file for the OS Portability Manager Library, which
*				contains functions to implement OS specific services in a
*				generic, cross platform API. Porting the OS Portability
*				Manager library is the first step to porting any SciTech
*				products to a new platform.
*
****************************************************************************/

#ifndef	__PMAPI_H
#define	__PMAPI_H

#include "scitech.h"
#include "pcilib.h"
#include "ztimerc.h"

/*--------------------------- Macros and Typedefs -------------------------*/

/* You will need to define one of the following before you compile this
 * library for it to work correctly with the DOS extender that you are
 * using when compiling for extended DOS:
 *
 *		TNT			- Phar Lap TNT DOS Extender
 *		DOS4GW		- Rational DOS/4GW, DOS/4GW Pro, Causeway and PMODE/W
 *		DJGPP		- DJGPP port of GNU C++
 *
 * If none is specified, we will automatically determine which operating
 * system is being targetted and the following will be defined (provided by
 * scitech.h header file):
 *
 *		__MSDOS16__		- Default for 16 bit MSDOS mode
 *		__MSDOS32__		- Default for 32 bit MSDOS
 *		__WINDOWS16__	- Default for 16 bit Windows
 *		__WINDOWS32__	- Default for 32 bit Windows
 *
 * One of the following will be defined automatically for you to select
 * which memory model is in effect:
 *
 *		REALMODE	- 16 bit real mode (large memory model)
 *		PM286		- 16 protected mode (large memory model)
 *		PM386		- 32 protected mode (flat memory model)
 */

#if defined(__UNIX__) && !defined(_MAX_PATH)
#define _MAX_PATH 256
#endif

#if defined(TNT) || defined(DOSX) || defined(X32VM) || defined(DPMI32)		\
	|| defined(DOS4GW) || defined(DJGPP) || defined(__WINDOWS32__)			\
	|| defined(__MSDOS32__) || defined(__UNIX__) || defined(__WIN32_VXD__) \
	|| defined(__32BIT__) || defined(__SMX32__) || defined(__RTTARGET__)
#define	PM386
#elif defined(DPMI16) || defined(__WINDOWS16__)
#define	PM286
#else
#define	REALMODE
#endif

#pragma pack(1)

/* Provide the typedefs for the PM_int386 functions, which issue native
 * interrupts in real or protected mode and can pass extended registers
 * around.
 */

struct _PMDWORDREGS {
	ulong	eax,ebx,ecx,edx,esi,edi,cflag;
	};

struct _PMWORDREGS {
	ushort 	ax,ax_hi;
	ushort 	bx,bx_hi;
	ushort 	cx,cx_hi;
	ushort 	dx,dx_hi;
	ushort 	si,si_hi;
	ushort 	di,di_hi;
	ushort	cflag,cflag_hi;
	};

struct _PMBYTEREGS {
	uchar	al, ah;	ushort ax_hi;
	uchar	bl, bh;	ushort bx_hi;
	uchar	cl, ch; ushort cx_hi;
	uchar	dl, dh; ushort dx_hi;
	};

typedef union {
	struct  _PMDWORDREGS e;
	struct  _PMWORDREGS  x;
	struct  _PMBYTEREGS  h;
	} PMREGS;

typedef struct {
	ushort  es;
	ushort  cs;
	ushort  ss;
	ushort	ds;
	ushort	fs;
	ushort	gs;
	} PMSREGS;

/* Provide definitions for the real mode register structures passed to
 * the PM_int86() and PM_int86x() routines. Note that we provide our own
 * functions to do this for 16-bit code that calls the PM_int386 functions.
 */

typedef PMREGS 	RMREGS;
typedef PMSREGS	RMSREGS;

typedef struct {
	long	edi;
	long	esi;
	long	ebp;
	long	reserved;
	long	ebx;
	long	edx;
	long	ecx;
	long	eax;
	short	flags;
	short	es,ds,fs,gs,ip,cs,sp,ss;
	} DPMI_regs;

#ifdef	__MSDOS__
/* Register structure passed to PM_VxDCall function */
typedef struct {
	ulong	eax;
	ulong	ebx;
	ulong	ecx;
	ulong	edx;
	ulong	esi;
	ulong	edi;
	ushort	ds,es;
	} VXD_regs;
#endif

#define	PM_MAX_DRIVE				3
#define	PM_MAX_PATH					256
#define	PM_FILE_INVALID				(void*)0xFFFFFFFF

/* Structure for generic directory traversal and management. Also the same
 * values are passed to PM_setFileAttr to change the file attributes.
 */

typedef struct {
	ulong	dwSize;
	ulong	attrib;
	ulong	sizeLo;
	ulong	sizeHi;
	char	name[PM_MAX_PATH];
	} PM_findData;

/* Flags stored in the PM_findData structure, and also values passed to
 * PM_setFileAttr to change the file attributes.
 */

#define PM_FILE_NORMAL				0x00000000
#define PM_FILE_READONLY         	0x00000001
#define PM_FILE_DIRECTORY        	0x00000002
#define PM_FILE_ARCHIVE				0x00000004
#define PM_FILE_HIDDEN   			0x00000008
#define PM_FILE_SYSTEM   			0x00000010

/* Flags returned by the PM_splitpath function */

#define PM_HAS_WILDCARDS 0x01
#define PM_HAS_EXTENSION 0x02
#define PM_HAS_FILENAME  0x04
#define PM_HAS_DIRECTORY 0x08
#define PM_HAS_DRIVE     0x10

/* Structure passed to the PM_setFileTime functions */
typedef struct {
	short	sec;     	/* Seconds */
	short	min;     	/* Minutes */
	short	hour;    	/* Hour (0--23) */
	short	day;     	/* Day of month (1--31) */
	short	mon;      	/* Month (0--11) */
	short	year;    	/* Year (calendar year minus 1900) */
	} PM_time;

/* Define a macro for creating physical base addresses from segment:offset */

#define MK_PHYS(s,o)  (((ulong)(s) << 4) + (ulong)(o))

/* Define the different types of modes supported. This is a global variable
 * that can be used to determine the type at runtime which will contain
 * one of these values.
 */

typedef enum {
	PM_realMode,
	PM_286,
	PM_386
	} PM_mode_enum;

/* Define types passed to PM_enableWriteCombine */

#define PM_MTRR_UNCACHABLE 	0
#define PM_MTRR_WRCOMB     	1
#define PM_MTRR_WRTHROUGH  	4
#define PM_MTRR_WRPROT     	5
#define PM_MTRR_WRBACK    	6
#define PM_MTRR_MAX    		6

/* Error codes returned by PM_enableWriteCombine */

#define PM_MTRR_ERR_OK					0
#define PM_MTRR_NOT_SUPPORTED           -1
#define PM_MTRR_ERR_PARAMS              -2
#define PM_MTRR_ERR_NOT_4KB_ALIGNED     -3
#define PM_MTRR_ERR_BELOW_1MB           -4
#define PM_MTRR_ERR_NOT_ALIGNED         -5
#define PM_MTRR_ERR_OVERLAP             -6
#define PM_MTRR_ERR_TYPE_MISMATCH       -7
#define PM_MTRR_ERR_NONE_FREE           -8
#define	PM_MTRR_ERR_NOWRCOMB			-9
#define	PM_MTRR_ERR_NO_OS_SUPPORT		-10

/* Values passed to the PM_DMACProgram function */

#define PM_DMA_READ_ONESHOT  	0x44	/* One-shot DMA read 		*/
#define PM_DMA_WRITE_ONESHOT	0x48    /* One-shot DMA write		*/
#define PM_DMA_READ_AUTOINIT  	0x54	/* Auto-init DMA read		*/
#define PM_DMA_WRITE_AUTOINIT	0x58	/* Auto-init DMA write		*/

/* Flags passed to suspend application callback */

#define PM_DEACTIVATE		1
#define	PM_REACTIVATE  		2

/* Return codes that the application can return from the suspend application
 * callback registered with the PM library. See the MGL documentation for
 * more details.
 */
#define	PM_NO_DEACTIVATE   	0
#define	PM_SUSPEND_APP    	1
#define	PM_NO_SUSPEND_APP	2

#if		defined(__WIN32_VXD__)
#define	PMAPI	_ASMAPI			/* 'C' calling conventions for VxD's	*/
#define	PMAPIP	_ASMAPIP
#elif	defined(__WINDOWS32__)
#define	PMAPI	_DLLASM			/* 'C' calling conventions for Win32	*/
#define	PMAPIP	_DLLASMP
#elif	defined(__WINDOWS16__)
#define	PMAPI	_DLLAPI			/* Pascal calling conventions for Win16	*/
#define	PMAPIP	_DLLAPIP
#elif   defined(__BEOS__)
#define PMAPI _ASMAPI
#define PMAPIP _ASMAPIP
#else
#define	PMAPI	_ASMAPI			/* 'C' calling conventions otherwise	*/
#define	PMAPIP	_ASMAPIP
#endif

/* Internal typedef to override DPMI_int86 handler */

typedef ibool (PMAPIP DPMI_handler_t)(DPMI_regs *regs);
void PMAPI DPMI_setInt10Handler(DPMI_handler_t handler);

/* Type definitions for a window handle for console modes */

#if		defined(__DRIVER__) || defined(__WIN32_VXD__)
typedef	void		*PM_HWND;	/* Pointer for portable drivers 		*/
typedef	void		*PM_MODULE;	/* Module handle for portable drivers	*/
#elif	defined(__WINDOWS__)
#ifdef	DECLARE_HANDLE
typedef	HWND		PM_HWND;	/* Real window handle 					*/
typedef	HINSTANCE	PM_MODULE;	/* Win32 DLL handle						*/
#else
typedef	void		*PM_HWND;	/* Place holder if windows.h not included */
typedef	void		*PM_MODULE;	/* Place holder if windows.h not included */
#endif
#elif	defined(__USE_X11__)
typedef	struct {
	Window		*window;
	Display		*display;
	} PM_HWND;					/* X11 window handle */
#elif	defined(__OS2__)
typedef	void	*PM_HWND;
typedef	void	*PM_MODULE;
#elif	defined(__LINUX__)
typedef	int		PM_HWND;		/* Console id for fullscreen Linux */
typedef	void	*PM_MODULE;
#elif	defined(__QNX__)
typedef	int		PM_HWND;		/* Console id for fullscreen QNX */
typedef	void	*PM_MODULE;
#elif	defined(__RTTARGET__)
typedef	int		PM_HWND;		/* Placeholder for RTTarget-32 */
typedef	void	*PM_MODULE;
#elif	defined(__REALDOS__)
typedef	int		PM_HWND;		/* Placeholder for fullscreen DOS */
typedef	void	*PM_MODULE;		/* Placeholder for fullscreen DOS */
#elif	defined(__SMX32__)
typedef	int		PM_HWND;		/* Placeholder for fullscreen SMX */
typedef	void	*PM_MODULE;
#else
#error	PM library not ported to this platform yet!
#endif

/* Type definition for code pointers */

typedef void (*__codePtr)();

/* Type definition for a C based interrupt handler */

typedef void (PMAPIP PM_intHandler)(void);
typedef ibool (PMAPIP PM_irqHandler)(void);

/* Hardware IRQ handle used to save and restore the hardware IRQ */

typedef void *PM_IRQHandle;

/* Type definition for the fatal cleanup handler */

typedef void (PMAPIP PM_fatalCleanupHandler)(void);

/* Type defifinition for save state callback function */

typedef int (PMAPIP PM_saveState_cb)(int flags);

/* Type definintion for enum write combined callback function */

typedef void (PMAPIP PM_enumWriteCombine_t)(ulong base,ulong length,uint type);

/* Structure defining all the PM API functions as exported to
 * the binary portable DLL's.
 */

typedef struct {
	ulong	dwSize;
	int 	(PMAPIP PM_getModeType)(void);
	void *	(PMAPIP PM_getBIOSPointer)(void);
	void * 	(PMAPIP PM_getA0000Pointer)(void);
	void * 	(PMAPIP PM_mapPhysicalAddr)(ulong base,ulong limit,ibool isCached);
	void *	(PMAPIP PM_mallocShared)(long size);
	int		(PMAPIP PM_mapShared)(void *ptr);
	void	(PMAPIP PM_freeShared)(void *ptr);
	void *	(PMAPIP PM_mapToProcess)(void *linear,ulong limit);
	void *	(PMAPIP PM_mapRealPointer)(uint r_seg,uint r_off);
	void *	(PMAPIP PM_allocRealSeg)(uint size,uint *r_seg,uint *r_off);
	void	(PMAPIP PM_freeRealSeg)(void *mem);
	void *	(PMAPIP PM_allocLockedMem)(uint size,ulong *physAddr,ibool below16Meg);
	void	(PMAPIP PM_freeLockedMem)(void *p,uint size);
	void 	(PMAPIP PM_callRealMode)(uint seg,uint off, RMREGS *regs,RMSREGS *sregs);
	int 	(PMAPIP PM_int86)(int intno, RMREGS *in, RMREGS *out);
	int 	(PMAPIP PM_int86x)(int intno, RMREGS *in, RMREGS *out,RMSREGS *sregs);
	void	(PMAPIP DPMI_int86)(int intno, DPMI_regs *regs);
	void	(PMAPIP PM_availableMemory)(ulong *physical,ulong *total);
	void *	(PMAPIP PM_getVESABuf)(uint *len,uint *rseg,uint *roff);
	long	(PMAPIP PM_getOSType)(void);
	void	(PMAPIP PM_fatalError)(const char *msg);
	void	(PMAPIP PM_setBankA)(int bank);
	void	(PMAPIP PM_setBankAB)(int bank);
	void 	(PMAPIP PM_setCRTStart)(int x,int y,int waitVRT);
	char *	(PMAPIP PM_getCurrentPath)(char *path,int maxLen);
	const char * (PMAPIP PM_getVBEAFPath)(void);
	const char * (PMAPIP PM_getNucleusPath)(void);
	const char * (PMAPIP PM_getNucleusConfigPath)(void);
	const char * (PMAPIP PM_getUniqueID)(void);
	const char * (PMAPIP PM_getMachineName)(void);
	ibool 	(PMAPIP VF_available)(void);
	void * 	(PMAPIP VF_init)(ulong baseAddr,int bankSize,int codeLen,void *bankFunc);
	void 	(PMAPIP VF_exit)(void);
	PM_HWND	(PMAPIP PM_openConsole)(PM_HWND hwndUser,int device,int xRes,int yRes,int bpp,ibool fullScreen);
	int		(PMAPIP PM_getConsoleStateSize)(void);
	void	(PMAPIP PM_saveConsoleState)(void *stateBuf,PM_HWND hwndConsole);
	void	(PMAPIP PM_restoreConsoleState)(const void *stateBuf,PM_HWND hwndConsole);
	void	(PMAPIP PM_closeConsole)(PM_HWND hwndConsole);
	void 	(PMAPIP PM_setOSCursorLocation)(int x,int y);
	void 	(PMAPIP PM_setOSScreenWidth)(int width,int height);
	int		(PMAPIP PM_enableWriteCombine)(ulong base,ulong length,uint type);
	void	(PMAPIP PM_backslash)(char *filename);
	int 	(PMAPIP PM_lockDataPages)(void *p,uint len);
	int 	(PMAPIP PM_unlockDataPages)(void *p,uint len);
	int 	(PMAPIP PM_lockCodePages)(__codePtr p,uint len);
	int 	(PMAPIP PM_unlockCodePages)(__codePtr p,uint len);
	ibool	(PMAPIP PM_setRealTimeClockHandler)(PM_intHandler ih,int frequency);
	void	(PMAPIP PM_setRealTimeClockFrequency)(int frequency);
	void 	(PMAPIP PM_restoreRealTimeClockHandler)(void);
	ibool 	(PMAPIP PM_doBIOSPOST)(ushort axVal,ulong BIOSPhysAddr,void *BIOSPtr,ulong BIOSLen);
	char 	(PMAPIP PM_getBootDrive)(void);
	void  	(PMAPIP PM_freePhysicalAddr)(void *ptr,ulong limit);
	uchar	(PMAPIP PM_inpb)(int port);
	ushort	(PMAPIP PM_inpw)(int port);
	ulong	(PMAPIP PM_inpd)(int port);
	void	(PMAPIP PM_outpb)(int port,uchar val);
	void	(PMAPIP PM_outpw)(int port,ushort val);
	void	(PMAPIP PM_outpd)(int port,ulong val);
	void	(PMAPIP PM_changeResolution)(PM_HWND hwndConsole,int xRes,int yRes,int bpp);
	void	(PMAPIP PM_setSuspendAppCallback)(int (_ASMAPIP saveState)(int flags));
	ibool	(PMAPIP PM_haveBIOSAccess)(void);
	int		(PMAPIP PM_kbhit)(void);
	int		(PMAPIP PM_getch)(void);
	ibool 	(PMAPIP PM_findBPD)(const char *dllname,char *bpdpath);
	ulong 	(PMAPIP PM_getPhysicalAddr)(void *p);
	void	(PMAPIP PM_sleep)(ulong milliseconds);
	int		(PMAPIP PM_getCOMPort)(int port);
	int		(PMAPIP PM_getLPTPort)(int port);
	PM_MODULE (PMAPIP PM_loadLibrary)(const char *szDLLName);
	void * 	(PMAPIP PM_getProcAddress)(PM_MODULE hModule,const char *szProcName);
	void	(PMAPIP PM_freeLibrary)(PM_MODULE hModule);
	int		(PMAPIP PCI_enumerate)(PCIDeviceInfo info[],int maxDevices);
	ulong 	(PMAPIP PCI_accessReg)(int index,ulong value,int func,PCIDeviceInfo *info);
	ibool	(PMAPIP PCI_setHardwareIRQ)(PCIDeviceInfo *info,uint intPin,uint IRQ);
	void	(PMAPIP PCI_generateSpecialCyle)(uint bus,ulong specialCycleData);
	void	(PMAPIP PCI_mapShared)(void);
	ulong 	(PMAPIP PCIBIOS_getEntry)(void);
	uint	(PMAPIP CPU_getProcessorType)(void);
	ibool	(PMAPIP CPU_haveMMX)(void);
	ibool	(PMAPIP CPU_have3DNow)(void);
	ibool	(PMAPIP CPU_haveSSE)(void);
	ibool	(PMAPIP CPU_haveRDTSC)(void);
	ulong	(PMAPIP CPU_getProcessorSpeed)(ibool accurate);
	void	(PMAPIP ZTimerInit)(void);
	void 	(PMAPIP LZTimerOn)(void);
	ulong	(PMAPIP LZTimerLap)(void);
	void 	(PMAPIP LZTimerOff)(void);
	ulong 	(PMAPIP LZTimerCount)(void);
	void 	(PMAPIP LZTimerOnExt)(LZTimerObject *tm);
	ulong	(PMAPIP LZTimerLapExt)(LZTimerObject *tm);
	void 	(PMAPIP LZTimerOffExt)(LZTimerObject *tm);
	ulong 	(PMAPIP LZTimerCountExt)(LZTimerObject *tm);
	void	(PMAPIP ULZTimerOn)(void);
	ulong	(PMAPIP ULZTimerLap)(void);
	void	(PMAPIP ULZTimerOff)(void);
	ulong	(PMAPIP ULZTimerCount)(void);
	ulong	(PMAPIP ULZReadTime)(void);
	ulong	(PMAPIP ULZElapsedTime)(ulong start,ulong finish);
	void	(PMAPIP ULZTimerResolution)(float *resolution);
	void *	(PMAPIP PM_findFirstFile)(const char *filename,PM_findData *findData);
	ibool	(PMAPIP PM_findNextFile)(void *handle,PM_findData *findData);
	void	(PMAPIP PM_findClose)(void *handle);
	void 	(PMAPIP PM_makepath)(char *p,const char *drive,const char *dir,const char *name,const char *ext);
	int		(PMAPIP PM_splitpath)(const char *fn,char *drive,char *dir,char *name,char *ext);
	ibool 	(PMAPIP PM_driveValid)(char drive);
	void 	(PMAPIP PM_getdcwd)(int drive,char *dir,int len);
	void 	(PMAPIP PM_setFileAttr)(const char *filename,uint attrib);
	ibool	(PMAPIP PM_mkdir)(const char *filename);
	ibool	(PMAPIP PM_rmdir)(const char *filename);
	uint	(PMAPIP PM_getFileAttr)(const char *filename);
	void	(PMAPIP PM_getFileTime)(const char *filename,ibool gmtTime,PM_time *time);
	void 	(PMAPIP PM_setFileTime)(const char *filename,ibool gmtTime,PM_time *time);
	char *	(PMAPIP CPU_getProcessorName)(void);
	int 	(PMAPIP PM_getVGAStateSize)(void);
	void 	(PMAPIP PM_saveVGAState)(void *stateBuf);
	void 	(PMAPIP PM_restoreVGAState)(const void *stateBuf);
	void 	(PMAPIP PM_vgaBlankDisplay)(void);
	void 	(PMAPIP PM_vgaUnblankDisplay)(void);
	} PM_imports;

#pragma pack()

/*---------------------------- Global variables ---------------------------*/

#ifdef  __cplusplus
extern "C" {            /* Use "C" linkage when in C++ mode */
#endif

#ifdef	__WIN32_VXD__
#define	VESA_BUF_SIZE 1024
extern uchar *_PM_rmBufAddr;
#endif

/* {secret} Pointer to global exports structure.
 * Should not be used by application programs.
 */
extern PM_imports _VARAPI _PM_imports;

/*--------------------------- Function Prototypes -------------------------*/

/* Routine to initialise the host side PM library. Note used from DLL's */

void	PMAPI PM_init(void);

/* Routine to return either PM_realMode, PM_286 or PM_386 */

int 	PMAPI PM_getModeType(void);

/* Routine to return a selector to the BIOS data area at segment 0x40 */

void *	PMAPI PM_getBIOSPointer(void);

/* Routine to return a linear pointer to the VGA frame buffer memory */

void * 	PMAPI PM_getA0000Pointer(void);

/* Routines to map/free physical memory into the current DS segment. In
 * some environments (32-bit DOS is one), after the mapping has been
 * allocated, it cannot be freed. Hence you should only allocate the
 * mapping once and cache the value for use by other parts of your
 * application. If the mapping cannot be createed, this function will
 * return a NULL pointer.
 *
 * This routine will also work for memory addresses below 1Mb, but the
 * mapped address cannot cross the 1Mb boundary.
 */

void * 	PMAPI PM_mapPhysicalAddr(ulong base,ulong limit,ibool isCached);
void  	PMAPI PM_freePhysicalAddr(void *ptr,ulong limit);

/* Routine to determine the physical address of a linear address. It is
 * up to the caller to ensure the entire address range for a linear
 * block of memory is page aligned if that is required.
 */

ulong 	PMAPI PM_getPhysicalAddr(void *p);

/* Routine to allocate a memory block in the global shared region that
 * is common to all tasks and accessible from ring 0 code.
 */

void *	PMAPI PM_mallocShared(long size);

/* Routine to map a shared memory block into the current process */

int		PMAPI PM_mapShared(void *ptr);

/* Routine to free the allocated shared memory block */

void	PMAPI PM_freeShared(void *ptr);

/* Attach a previously allocated linear mapping to a new process */

void *	PMAPI PM_mapToProcess(void *linear,ulong limit);

/* Macros to extract byte, word and long values from a char pointer */

#define	PM_getByte(p)		*((volatile uchar*)(p))
#define	PM_getWord(p)		*((volatile ushort*)(p))
#define	PM_getLong(p)		*((volatile ulong*)(p))
#define	PM_setByte(p,v)		PM_getByte(p) = (v)
#define	PM_setWord(p,v)		PM_getWord(p) = (v)
#define	PM_setLong(p,v)		PM_getLong(p) = (v)

/* Routine for accessing a low 1Mb memory block. You dont need to free this
 * pointer, but in 16 bit protected mode the selector allocated will be
 * re-used the next time this routine is called.
 */

void *	PMAPI PM_mapRealPointer(uint r_seg,uint r_off);

/* Routine to allocate a block of conventional memory below the 1Mb
 * limit so that it can be accessed from real mode. Ensure that you free
 * the segment when you are done with it.
 *
 * This routine returns a selector and offset to the segment that has been
 * allocated, and also returns the real mode segment and offset which can
 * be passed to real mode routines. Will return 0 if memory could not be
 * allocated.
 *
 * Please note that with some DOS extenders, memory allocated with the
 * following function cannot be freed, hence it will be allocated for the
 * life of your program. Thus if you need to call a bunch of different
 * real-mode routines in your program, allocate a single large buffer at
 * program startup that can be re-used throughout the program execution.
 */

void *	PMAPI PM_allocRealSeg(uint size,uint *r_seg,uint *r_off);
void	PMAPI PM_freeRealSeg(void *mem);

/* Routine to allocate a block of locked memory, and return both the
 * linear and physical addresses of the memory.
 */

void *	PMAPI PM_allocLockedMem(uint size,ulong *physAddr,ibool below16Meg);
void	PMAPI PM_freeLockedMem(void *p,uint size);

/* Routine to return true if we have access to the BIOS on the host OS */

ibool	PMAPI PM_haveBIOSAccess(void);

/* Routine to call a real mode assembly language procedure. Register
 * values are passed in and out in the 'regs' and 'sregs' structures. We
 * do not provide any method of copying data from the protected mode stack
 * to the real mode stack, so if you need to pass data to real mode, you will
 * need to write a real mode assembly language hook to recieve the values
 * in registers, and to pass the data through a real mode block allocated
 * with the PM_allocRealSeg() routine.
 */

void 	PMAPI PM_callRealMode(uint seg,uint off, RMREGS *regs,RMSREGS *sregs);

/* Routines to generate real mode interrupts using the same interface that
 * is used by int86() and int86x() in realmode. This routine is need to
 * call certain BIOS and DOS functions that are not supported by some
 * DOS extenders. No translation is done on any of the register values,
 * so they must be correctly set up and translated by the calling program.
 *
 * Normally the DOS extenders will allow you to use the normal int86()
 * function directly and will pass on unhandled calls to real mode to be
 * handled by the real mode handler. However calls to int86x() with real
 * mode segment values to be loaded will cause a GPF if used with the
 * standard int86x(), so you should use these routines if you know you
 * want to call a real mode handler.
 */

int 	PMAPI PM_int86(int intno, RMREGS *in, RMREGS *out);
int 	PMAPI PM_int86x(int intno, RMREGS *in, RMREGS *out,RMSREGS *sregs);

/* Routine to generate a real mode interrupt. This is identical to the
 * above function, but takes a DPMI_regs structure for the registers
 * which has a lot more information. It is only available from 32-bit
 * protected mode.
 */

void	PMAPI DPMI_int86(int intno, DPMI_regs *regs);

/* Function to return the amount of available physical and total memory.
 * The results of this function are *only* valid before you have made any
 * calls to malloc() and free(). If you need to keep track of exactly how
 * much memory is currently allocated, you need to call this function to
 * get the total amount of memory available and then keep track of
 * the available memory every time you call malloc() and free().
 */

void	PMAPI PM_availableMemory(ulong *physical,ulong *total);

/* Return the address of a global VESA real mode transfer buffer for use
 * by applications.
 */

void *	PMAPI PM_getVESABuf(uint *len,uint *rseg,uint *roff);

/* Handle fatal error conditions */

void 	PMAPI PM_fatalError(const char *msg);

/* Function to set a cleanup error handler called when PM_fatalError
 * is called. This allows us to the console back into a normal state
 * if we get a failure from deep inside a BPD file. This function is
 * not exported to BPD files, and is only used by code compiled for the
 * OS.
 */

void 	PMAPI PM_setFatalErrorCleanup(PM_fatalCleanupHandler cleanup);

/* Return the OS type flag as defined in <drvlib/os/os.h> */

long	PMAPI PM_getOSType(void);

/* Functions to set a VBE bank via an Int 10h */

void 	PMAPI PM_setBankA(int bank);
void	PMAPI PM_setBankAB(int bank);
void 	PMAPI PM_setCRTStart(int x,int y,int waitVRT);

/* Return the current working directory */

char *	PMAPI PM_getCurrentPath(char *path,int maxLen);

/* Return paths to the VBE/AF and Nucleus directories */

const char * PMAPI PM_getVBEAFPath(void);
const char * PMAPI PM_getNucleusPath(void);
const char * PMAPI PM_getNucleusConfigPath(void);

/* Find the path to a binary portable DLL */

ibool 	PMAPI PM_findBPD(const char *dllname,char *bpdpath);

/* Returns the drive letter of the boot drive for DOS, OS/2 and Windows */

char 	PMAPI PM_getBootDrive(void);

/* Return a network unique machine identifier as a string */

const char * PMAPI PM_getUniqueID(void);

/* Return the network machine name as a string */

const char * PMAPI PM_getMachineName(void);

/* Functions to install and remove the virtual linear framebuffer
 * emulation code. For unsupported DOS extenders and when running under
 * a DPMI host like Windows or OS/2, this function will return a NULL.
 */

ibool 	PMAPI VF_available(void);
void * 	PMAPI VF_init(ulong baseAddr,int bankSize,int codeLen,void *bankFunc);
void 	PMAPI VF_exit(void);

/* Functions to wait for a keypress and read a key for command line
 * environments such as DOS, Win32 console and Unix.
 */

int		PMAPI PM_kbhit(void);
int		PMAPI PM_getch(void);

/* Functions to create either a fullscreen or windowed console on the
 * desktop, and to allow the resolution of fullscreen consoles to be
 * changed on the fly without closing the console. For non-windowed
 * environments (such as a Linux or OS/2 fullscreen console), these
 * functions enable console graphics mode and restore console text mode.
 *
 * The suspend application callback is used to allow the application to
 * save the state of the fullscreen console mode to allow temporary
 * switching to another console or back to the regular GUI desktop. It
 * is also called to restore the fullscreen graphics state after the
 * fullscreen console regains the focus.
 *
 * The device parameter allows for the console to be opened on a different
 * display controllers (0 is always the primary controller).
 */

PM_HWND	PMAPI PM_openConsole(PM_HWND hwndUser,int device,int xRes,int yRes,int bpp,ibool fullScreen);
int		PMAPI PM_getConsoleStateSize(void);
void	PMAPI PM_saveConsoleState(void *stateBuf,PM_HWND hwndConsole);
void	PMAPI PM_changeResolution(PM_HWND hwndConsole,int xRes,int yRes,int bpp);
void	PMAPI PM_setSuspendAppCallback(PM_saveState_cb saveState);
void	PMAPI PM_restoreConsoleState(const void *stateBuf,PM_HWND hwndConsole);
void	PMAPI PM_closeConsole(PM_HWND hwndConsole);

/* Functions to modify OS console information */

void 	PMAPI PM_setOSCursorLocation(int x,int y);
void 	PMAPI PM_setOSScreenWidth(int width,int height);

/* Function to emable Intel PPro/PII write combining */

int		PMAPI PM_enableWriteCombine(ulong base,ulong length,uint type);
int 	PMAPI PM_enumWriteCombine(PM_enumWriteCombine_t callback);

/* Function to add a path separator to the end of a filename (if not present) */

void	PMAPI PM_backslash(char *filename);

/* Routines to lock and unlock regions of memory under a virtual memory
 * environment. These routines _must_ be used to lock all hardware
 * and mouse interrupt handlers installed, _AND_ any global data that
 * these handler manipulate, so that they will always be present in memory
 * to handle the incoming interrupts.
 *
 * Note that it is important to call the correct routine depending on
 * whether the area being locked is code or data, so that under 32 bit
 * PM we will get the selector value correct.
 */

int 	PMAPI PM_lockDataPages(void *p,uint len);
int 	PMAPI PM_unlockDataPages(void *p,uint len);
int 	PMAPI PM_lockCodePages(__codePtr p,uint len);
int 	PMAPI PM_unlockCodePages(__codePtr p,uint len);

/* Routines to install and remove Real Time Clock interrupt handlers. The
 * frequency of the real time clock can be changed by calling
 * PM_setRealTimeClockFrequeny, and the value can be any power of 2 value
 * from 2Hz to 8192Hz.
 *
 * Note that you _must_ lock the memory containing the interrupt
 * handlers with the PM_lockPages() function otherwise you may encounter
 * problems in virtual memory environments.
 *
 * NOTE: User space versions of the PM library should fail these functions.
 */

ibool	PMAPI PM_setRealTimeClockHandler(PM_intHandler ih,int frequency);
void	PMAPI PM_setRealTimeClockFrequency(int frequency);
void 	PMAPI PM_restoreRealTimeClockHandler(void);

/* Routines to install and remove hardware interrupt handlers.
 *
 * Note that you _must_ lock the memory containing the interrupt
 * handlers with the PM_lockPages() function otherwise you may encounter
 * problems in virtual memory environments.
 *
 * NOTE: User space versions of the PM library should fail these functions.
 */

PM_IRQHandle PMAPI PM_setIRQHandler(int IRQ,PM_irqHandler ih);
void 	PMAPI PM_restoreIRQHandler(PM_IRQHandle irqHandle);

/* Functions to program DMA using the legacy ISA DMA controller */

void	PMAPI PM_DMACEnable(int channel);
void	PMAPI PM_DMACDisable(int channel);
void	PMAPI PM_DMACProgram(int channel,int mode,ulong bufferPhys,int count);
ulong	PMAPI PM_DMACPosition(int channel);

/* Function to post secondary graphics controllers using the BIOS */

ibool 	PMAPI PM_doBIOSPOST(ushort axVal,ulong BIOSPhysAddr,void *mappedBIOS,ulong BIOSLen);

/* Functions to do I/O port manipulation directly from C code. These
 * functions are portable and will work on any processor architecture
 * to access I/O space registers on PCI devices.
 */

uchar	PMAPI PM_inpb(int port);
ushort	PMAPI PM_inpw(int port);
ulong	PMAPI PM_inpd(int port);
void	PMAPI PM_outpb(int port,uchar val);
void	PMAPI PM_outpw(int port,ushort val);
void	PMAPI PM_outpd(int port,ulong val);

/* Functions to determine the I/O port locations for COM and LPT ports.
 * The functions are zero based, so for COM1 or LPT1 pass in a value of 0,
 * for COM2 or LPT2 pass in a value of 1 etc.
 */

int		PMAPI PM_getCOMPort(int port);
int		PMAPI PM_getLPTPort(int port);

/* Internal functions that need prototypes */

void 	PMAPI _PM_getRMvect(int intno, long *realisr);
void 	PMAPI _PM_setRMvect(int intno, long realisr);
void 	PMAPI _PM_freeMemoryMappings(void);

/* Function to override the default debug log file location */

void 	PMAPI PM_setDebugLog(const char *logFilePath);

/* Function to put the process to sleep for the specified milliseconds */

void	PMAPI PM_sleep(ulong milliseconds);

/* Functions for directory traversal and management */

void *	PMAPI PM_findFirstFile(const char *filename,PM_findData *findData);
ibool	PMAPI PM_findNextFile(void *handle,PM_findData *findData);
void	PMAPI PM_findClose(void *handle);
void 	PMAPI PM_makepath(char *p,const char *drive,const char *dir,const char *name,const char *ext);
int		PMAPI PM_splitpath(const char *fn,char *drive,char *dir,char *name,char *ext);
ibool 	PMAPI PM_driveValid(char drive);
void 	PMAPI PM_getdcwd(int drive,char *dir,int len);
uint	PMAPI PM_getFileAttr(const char *filename);
void 	PMAPI PM_setFileAttr(const char *filename,uint attrib);
void	PMAPI PM_getFileTime(const char *filename,ibool gmTime,PM_time *time);
void 	PMAPI PM_setFileTime(const char *filename,ibool gmTime,PM_time *time);
ibool	PMAPI PM_mkdir(const char *filename);
ibool	PMAPI PM_rmdir(const char *filename);

/* Functions to handle loading OS specific shared libraries */

PM_MODULE PMAPI PM_loadLibrary(const char *szDLLName);
void * 	PMAPI PM_getProcAddress(PM_MODULE hModule,const char *szProcName);
void	PMAPI PM_freeLibrary(PM_MODULE hModule);

/* Function to enable IOPL access if required */

int 	PMAPI PM_setIOPL(int iopl);

/* DOS specific fucntions */

#ifdef	__MSDOS__
uint 	PMAPI PMHELP_getVersion(void);
void 	PMAPI PM_VxDCall(VXD_regs *regs);
#endif

/* Functions to save and restore the VGA hardware state */

int 	PMAPI PM_getVGAStateSize(void);
void 	PMAPI PM_saveVGAState(void *stateBuf);
void 	PMAPI PM_restoreVGAState(const void *stateBuf);
void 	PMAPI PM_vgaBlankDisplay(void);
void 	PMAPI PM_vgaUnblankDisplay(void);

/* Call the X86 emulator or the real BIOS in our test harness */

#ifdef	TEST_HARNESS
#define	PM_mapRealPointer(r_seg,r_off)		_PM_imports.PM_mapRealPointer(r_seg,r_off)
#define	PM_getVESABuf(len,rseg,roff)		_PM_imports.PM_getVESABuf(len,rseg,roff)
#define	PM_callRealMode(seg,off,regs,sregs)	_PM_imports.PM_callRealMode(seg,off,regs,sregs)
#define	PM_int86(intno,in,out)				_PM_imports.PM_int86(intno,in,out)
#define	PM_int86x(intno,in,out,sregs)		_PM_imports.PM_int86x(intno,in,out,sregs)
#endif

#ifdef	__cplusplus
}						/* End of "C" linkage for C++	*/
#endif

/* Include OS extensions for interrupt handling */

#if defined(__REALDOS__) || defined(__SMX32__)
#include "pmint.h"
#endif

#endif /* __PMAPI_H */
