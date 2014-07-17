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
* Environment:	32-bit Windows VxD
*
* Description:	Implementation for the OS Portability Manager Library, which
*				contains functions to implement OS specific services in a
*				generic, cross platform API. Porting the OS Portability
*				Manager library is the first step to porting any SciTech
*				products to a new platform.
*
****************************************************************************/

#include "pmapi.h"
#include "drvlib/os/os.h"
#include "sdd/sddhelp.h"
#include "mtrr.h"

/*--------------------------- Global variables ----------------------------*/

#define	MAX_MEMORY_SHARED			100
#define	MAX_MEMORY_MAPPINGS			100

typedef struct {
	void	*linear;
	ulong	global;
	ulong	length;
	int		npages;
	} memshared;

typedef struct {
	ulong	physical;
	ulong	linear;
	ulong	length;
	int		npages;
	ibool	isCached;
	} mmapping;

static int 			numMappings = 0;
static memshared	shared[MAX_MEMORY_MAPPINGS] = {0};
static mmapping		maps[MAX_MEMORY_MAPPINGS];
char				_PM_cntPath[PM_MAX_PATH] = "";
uchar				*_PM_rmBufAddr = NULL;
ushort _VARAPI		_PM_savedDS = 0;
static uchar		_PM_oldCMOSRegA;
static uchar		_PM_oldCMOSRegB;
PM_intHandler		_PM_rtcHandler = NULL;
IRQHANDLE 			RTCIRQHandle = 0;
VPICD_HWInt_THUNK 	RTCInt_Thunk;

static char *szWindowsKey		= "Software\\Microsoft\\Windows\\CurrentVersion";
static char *szSystemRoot		= "SystemRoot";
static char *szMachineNameKey 	= "System\\CurrentControlSet\\control\\ComputerName\\ComputerName";
static char *szMachineName 		= "ComputerName";
static void (PMAPIP fatalErrorCleanup)(void) = NULL;

/*----------------------------- Implementation ----------------------------*/

/* Functions to read and write CMOS registers */

ulong	PMAPI _PM_getPDB(void);
uchar 	PMAPI _PM_readCMOS(int index);
void 	PMAPI _PM_writeCMOS(int index,uchar value);

void PMAPI PM_init(void)
{ MTRR_init(); }

ibool PMAPI PM_haveBIOSAccess(void)
{ return true; }

long PMAPI PM_getOSType(void)
{ return _OS_WIN32VXD; }

int PMAPI PM_getModeType(void)
{ return PM_386; }

void PMAPI PM_backslash(char *s)
{
	uint pos = strlen(s);
	if (s[pos-1] != '\\') {
		s[pos] = '\\';
		s[pos+1] = '\0';
		}
}

void PMAPI PM_setFatalErrorCleanup(
	void (PMAPIP cleanup)(void))
{
	fatalErrorCleanup = cleanup;
}

void PMAPI PM_fatalError(const char *msg)
{
	if (fatalErrorCleanup)
		fatalErrorCleanup();
	Fatal_Error_Handler(msg,0);
}

/****************************************************************************
PARAMETERS:
len		- Place to store the length of the buffer
rseg	- Place to store the real mode segment of the buffer
roff	- Place to store the real mode offset of the buffer

REMARKS:
This function returns the address and length of the global VESA transfer
buffer that is used for communicating with the VESA BIOS functions from
Win16 and Win32 programs under Windows.
****************************************************************************/
void * PMAPI PM_getVESABuf(
	uint *len,
	uint *rseg,
	uint *roff)
{
	/* If the VxD is dynamically loaded we will not have a real mode
	 * transfer buffer to return, so we fail the call.
	 */
	if (_PM_rmBufAddr) {
		*len = VESA_BUF_SIZE;
		*rseg = (ulong)(_PM_rmBufAddr) >> 4;
		*roff = (ulong)(_PM_rmBufAddr) & 0xF;
		return _PM_rmBufAddr;
		}
	return NULL;
}

int PMAPI PM_int386(int intno, PMREGS *in, PMREGS *out)
{
	/* Unused in VxDs */
	return 0;
}

void PMAPI _PM_getRMvect(int intno, long *realisr)
{
	WORD	seg;
	DWORD	off;

	Get_V86_Int_Vector(intno,&seg,&off);
	*realisr = ((long)seg << 16) | (off & 0xFFFF);
}

void PMAPI _PM_setRMvect(int intno, long realisr)
{
	Set_V86_Int_Vector(intno,realisr >> 16,realisr & 0xFFFF);
}

char * PMAPI PM_getCurrentPath(char *path,int maxLen)
{
	strncpy(path,_PM_cntPath,maxLen);
	path[maxLen-1] = 0;
	return path;
}

char PMAPI PM_getBootDrive(void)
{ return 'C'; }

const char * PMAPI PM_getVBEAFPath(void)
{ return "c:\\"; }

/****************************************************************************
PARAMETERS:
szKey		- Key to query (can contain version number formatting)
szValue		- Value to get information for
value		- Place to store the registry key data read
size		- Size of the string buffer to read into

RETURNS:
true if the key was found, false if not.
****************************************************************************/
static ibool REG_queryString(
	char *szKey,
	char *szValue,
	char *value,
	ulong size)
{
	HKEY 	hKey;
	ulong	type;
	ibool	status = false;

	memset(value,0,sizeof(value));
	if (RegOpenKey(HKEY_LOCAL_MACHINE,szKey,&hKey) == ERROR_SUCCESS) {
		if (RegQueryValueEx(hKey,(PCHAR)szValue,(ulong*)NULL,(ulong*)&type,value,(ulong*)&size) == ERROR_SUCCESS)
			status = true;
		RegCloseKey(hKey);
		}
	return status;
}

const char * PMAPI PM_getNucleusPath(void)
{
	static char path[256];
	if (!REG_queryString(szWindowsKey,szSystemRoot,path,sizeof(path)))
		strcpy(path,"c:\\windows");
	PM_backslash(path);
	strcat(path,"system\\nucleus");
	return path;
}

const char * PMAPI PM_getNucleusConfigPath(void)
{
	static char path[256];
	strcpy(path,PM_getNucleusPath());
	PM_backslash(path);
	strcat(path,"config");
	return path;
}

const char * PMAPI PM_getUniqueID(void)
{ return PM_getMachineName(); }

const char * PMAPI PM_getMachineName(void)
{
	static char name[256];
	if (REG_queryString(szMachineNameKey,szMachineName,name,sizeof(name)))
		return name;
	return "Unknown";
}

int PMAPI PM_kbhit(void)
{ return 1; }

int PMAPI PM_getch(void)
{ return 0; }

PM_HWND	PMAPI PM_openConsole(PM_HWND hwndUser,int device,int xRes,int yRes,int bpp,ibool fullScreen)
{
	/* Unused in VxDs */
	return NULL;
}

int PMAPI PM_getConsoleStateSize(void)
{
	/* Unused in VxDs */
	return 1;
}

void PMAPI PM_saveConsoleState(void *stateBuf,PM_HWND hwndConsole)
{
	/* Unused in VxDs */
}

void PMAPI PM_changeResolution(PM_HWND hwndConsole,int xRes,int yRes,int bpp)
{
	/* Unused in VxDs */
}

void PMAPI PM_setSuspendAppCallback(int (_ASMAPIP saveState)(int flags))
{
	/* Unused in VxDs */
}

void PMAPI PM_restoreConsoleState(const void *stateBuf,PM_HWND hwndConsole)
{
	/* Unused in VxDs */
}

void PMAPI PM_closeConsole(PM_HWND hwndConsole)
{
	/* Unused in VxDs */
}

void PM_setOSCursorLocation(int x,int y)
{
	uchar *_biosPtr = PM_getBIOSPointer();
	PM_setByte(_biosPtr+0x50,x);
	PM_setByte(_biosPtr+0x51,y);
}

void PM_setOSScreenWidth(int width,int height)
{
	uchar *_biosPtr = PM_getBIOSPointer();
	PM_setByte(_biosPtr+0x4A,width);
	PM_setByte(_biosPtr+0x84,height-1);
}

/****************************************************************************
REMARKS:
Allocate a block of shared memory. For Win9x we allocate shared memory
as locked, global memory that is accessible from any memory context
(including interrupt time context), which allows us to load our important
data structure and code such that we can access it directly from a ring
0 interrupt context.
****************************************************************************/
void * PMAPI PM_mallocShared(long size)
{
	MEMHANDLE	hMem;
	DWORD 		pgNum,nPages = (size + 0xFFF) >> 12;
	int			i;

	/* First find a free slot in our shared memory table */
	for (i = 0; i < MAX_MEMORY_SHARED; i++) {
		if (shared[i].linear == 0)
			break;
		}
	if (i < MAX_MEMORY_SHARED) {
		PageAllocate(nPages,PG_SYS,0,0,0,0,NULL,0,&hMem,&shared[i].linear);
		shared[i].npages = nPages;
		pgNum = (ulong)shared[i].linear >> 12;
		shared[i].global = LinPageLock(pgNum,nPages,PAGEMAPGLOBAL);
		return (void*)shared[i].global;
		}
	return NULL;
}

/****************************************************************************
REMARKS:
We don't need to do anything to share the memory, since it is system
global and everyone can see it.
****************************************************************************/
int PMAPI PM_mapShared(void *ptr)
{
	return 0;
}

/****************************************************************************
REMARKS:
Free a block of shared memory
****************************************************************************/
void PMAPI PM_freeShared(void *p)
{
	int	i;

	/* Find a shared memory block in our table and free it */
	for (i = 0; i < MAX_MEMORY_SHARED; i++) {
		if (shared[i].global == (ulong)p) {
			LinPageUnLock(shared[i].global >> 12,shared[i].npages,PAGEMAPGLOBAL);
			PageFree((ulong)shared[i].linear,0);
			shared[i].linear = 0;
			break;
			}
		}
}

void * PMAPI PM_mapToProcess(void *base,ulong limit)
{ return (void*)base; }

ibool PMAPI PM_doBIOSPOST(
	ushort axVal,
	ulong BIOSPhysAddr,
	void *mappedBIOS,
	ulong BIOSLen)
{
	// TODO: Figure out how to do this
	return false;
}

void * PMAPI PM_getBIOSPointer(void)
{ return (void*)0x400; }

void * PMAPI PM_getA0000Pointer(void)
{ return PM_mapPhysicalAddr(0xA0000,0xFFFF,true); }

/****************************************************************************
PARAMETERS:
base		- Physical base address of the memory to maps in
limit		- Limit of physical memory to region to maps in

RETURNS:
Linear address of the newly mapped memory.

REMARKS:
Maps a physical memory range to a linear memory range.
****************************************************************************/
ulong MapPhysicalToLinear(
	ulong base,
	ulong limit,
	int *npages)
{
	ulong	linear,length = limit+1;
	int		i,ppage,flags;

	ppage = base >> 12;
	*npages = (length + (base & 0xFFF) + 4095) >> 12;
	flags = PR_FIXED | PR_STATIC;
	if (base == 0xA0000) {
		/* We require the linear address to be aligned to a 64Kb boundary
		 * for mapping the banked framebuffer (so we can do efficient
		 * carry checking for bank changes in the assembler code). The only
		 * way to ensure this is to force the linear address to be aligned
		 * to a 4Mb boundary.
		 */
		flags |= PR_4MEG;
		}
	if ((linear = (ulong)PageReserve(PR_SYSTEM,*npages,flags)) == (ulong)-1)
		return 0;
	if (!PageCommitPhys(linear >> 12,*npages,ppage,PC_INCR | PC_USER | PC_WRITEABLE))
		return 0;
	return linear + (base & 0xFFF);
}

/****************************************************************************
PARAMETERS:
base		- Physical base address of the memory to maps in
limit		- Limit of physical memory to region to maps in
isCached	- True if the memory should be cached, false if not

RETURNS:
Linear address of the newly mapped memory.

REMARKS:
This function maps physical memory to linear memory, which can then be used
to create a selector or used directly from 32-bit protected mode programs.
This is better than DPMI 0x800, since it allows you to maps physical
memory below 1Mb, which gets this memory out of the way of the Windows VDD's
sticky paws.

NOTE:	If the memory is not expected to be cached, this function will
		directly re-program the PCD (Page Cache Disable) bit in the
		page tables. There does not appear to be a mechanism in the VMM
		to control this bit via the regular interface.
****************************************************************************/
void * PMAPI PM_mapPhysicalAddr(
	ulong base,
	ulong limit,
	ibool isCached)
{
	ulong	linear,length = limit+1;
	int		i,npages;
	ulong	PDB,*pPDB;

	/* Search table of existing mappings to see if we have already mapped
	 * a region of memory that will serve this purpose.
	 */
	for (i = 0; i < numMappings; i++) {
		if (maps[i].physical == base && maps[i].length == length && maps[i].isCached == isCached)
			return (void*)maps[i].linear;
		}
	if (numMappings == MAX_MEMORY_MAPPINGS)
		return NULL;

	/* We did not find any previously mapped memory region, so maps it in.
	 * Note that we do not use MapPhysToLinear, since this function appears
	 * to have problems mapping memory in the 1Mb physical address space.
	 * Hence we use PageReserve and PageCommitPhys.
	 */
	if ((linear = MapPhysicalToLinear(base,limit,&npages)) == 0)
		return NULL;
	maps[numMappings].physical = base;
	maps[numMappings].length = length;
	maps[numMappings].linear = linear;
	maps[numMappings].npages = npages;
	maps[numMappings].isCached = isCached;
	numMappings++;

	/* Finally disable caching where necessary */
	if (!isCached && (PDB = _PM_getPDB()) != 0) {
		int 	startPDB,endPDB,iPDB,startPage,endPage,start,end,iPage;
		ulong 	pageTable,*pPageTable;

		if (PDB >= 0x100000)
			pPDB = (ulong*)MapPhysicalToLinear(PDB,0xFFF,&npages);
		else
			pPDB = (ulong*)PDB;
		if (pPDB) {
			startPDB = (linear >> 22) & 0x3FF;
			startPage = (linear >> 12) & 0x3FF;
			endPDB = ((linear+limit) >> 22) & 0x3FF;
			endPage = ((linear+limit) >> 12) & 0x3FF;
			for (iPDB = startPDB; iPDB <= endPDB; iPDB++) {
				pageTable = pPDB[iPDB] & ~0xFFF;
				if (pageTable >= 0x100000)
					pPageTable = (ulong*)MapPhysicalToLinear(pageTable,0xFFF,&npages);
				else
					pPageTable = (ulong*)pageTable;
				start = (iPDB == startPDB) ? startPage : 0;
				end = (iPDB == endPDB) ? endPage : 0x3FF;
				for (iPage = start; iPage <= end; iPage++)
					pPageTable[iPage] |= 0x10;
				PageFree((ulong)pPageTable,PR_STATIC);
				}
			PageFree((ulong)pPDB,PR_STATIC);
			}
		}
	return (void*)linear;
}

void PMAPI PM_freePhysicalAddr(void *ptr,ulong limit)
{
	/* We never free the mappings */
}

void PMAPI PM_sleep(ulong milliseconds)
{
	/* We never sleep in a VxD */
}

int PMAPI PM_getCOMPort(int port)
{
	// TODO: Re-code this to determine real values using the Plug and Play
	//		 manager for the OS.
	switch (port) {
		case 0:	return 0x3F8;
		case 1:	return 0x2F8;
		}
	return 0;
}

int PMAPI PM_getLPTPort(int port)
{
	// TODO: Re-code this to determine real values using the Plug and Play
	//		 manager for the OS.
	switch (port) {
		case 0:	return 0x3BC;
		case 1: return 0x378;
		case 2:	return 0x278;
		}
	return 0;
}

ulong PMAPI PM_getPhysicalAddr(void *p)
{
	DWORD	pte;
	CopyPageTable(((DWORD)p) >> 12, 1, (PVOID*)&pte, 0);
	return (pte & ~0xFFF) | (((DWORD)p) & 0xFFF);
}

void PMAPI _PM_freeMemoryMappings(void)
{
	int	i;
	for (i = 0; i < numMappings; i++)
		PageFree(maps[i].linear,PR_STATIC);
}

void * PMAPI PM_mapRealPointer(uint r_seg,uint r_off)
{ return (void*)MK_PHYS(r_seg,r_off); }

void * PMAPI PM_allocRealSeg(uint size,uint *r_seg,uint *r_off)
{ return NULL; }

void PMAPI PM_freeRealSeg(void *mem)
{ }

void PMAPI DPMI_int86(int intno, DPMI_regs *regs)
{
	/* Unsed in VxD's */
}

/****************************************************************************
REMARKS:
Load the V86 registers in the client state, and save the original state
before loading the registers.
****************************************************************************/
static void LoadV86Registers(
	CLIENT_STRUCT *saveRegs,
	RMREGS *in,
	RMSREGS *sregs)
{
	CLIENT_STRUCT	newRegs;

	Save_Client_State(saveRegs);
	newRegs = *saveRegs;
	newRegs.CRS.Client_EAX = in->e.eax;
	newRegs.CRS.Client_EBX = in->e.ebx;
	newRegs.CRS.Client_ECX = in->e.ecx;
	newRegs.CRS.Client_EDX = in->e.edx;
	newRegs.CRS.Client_ESI = in->e.esi;
	newRegs.CRS.Client_EDI = in->e.edi;
	newRegs.CRS.Client_ES = sregs->es;
	newRegs.CRS.Client_DS = sregs->ds;
	Restore_Client_State(&newRegs);
}

/****************************************************************************
REMARKS:
Read the V86 registers from the client state and restore the original state.
****************************************************************************/
static void ReadV86Registers(
	CLIENT_STRUCT *saveRegs,
	RMREGS *out,
	RMSREGS *sregs)
{
	CLIENT_STRUCT	newRegs;

	Save_Client_State(&newRegs);
	out->e.eax = newRegs.CRS.Client_EAX;
	out->e.ebx = newRegs.CRS.Client_EBX;
	out->e.ecx = newRegs.CRS.Client_ECX;
	out->e.edx = newRegs.CRS.Client_EDX;
	out->e.esi = newRegs.CRS.Client_ESI;
	out->e.edi = newRegs.CRS.Client_EDI;
	sregs->es = newRegs.CRS.Client_ES;
	sregs->ds = newRegs.CRS.Client_DS;
	Restore_Client_State(saveRegs);
}

/****************************************************************************
REMARKS:
Call a V86 real mode function with the specified register values
loaded before the call. The call returns with a far ret.
****************************************************************************/
void PMAPI PM_callRealMode(
	uint seg,
	uint off,
	RMREGS *regs,
	RMSREGS *sregs)
{
	CLIENT_STRUCT saveRegs;

	TRACE("SDDHELP: Entering PM_callRealMode()\n");
	Begin_Nest_V86_Exec();
	LoadV86Registers(&saveRegs,regs,sregs);
	Simulate_Far_Call(seg, off);
	Resume_Exec();
	ReadV86Registers(&saveRegs,regs,sregs);
	End_Nest_Exec();
	TRACE("SDDHELP: Exiting PM_callRealMode()\n");
}

/****************************************************************************
REMARKS:
Issue a V86 real mode interrupt with the specified register values
loaded before the interrupt.
****************************************************************************/
int PMAPI PM_int86(
	int intno,
	RMREGS *in,
	RMREGS *out)
{
	RMSREGS			sregs = {0};
	CLIENT_STRUCT	saveRegs;
	ushort			oldDisable;

	TRACE("SDDHELP: Entering PM_int86()\n");

	// Disable pass-up to our VxD handler so we directly call BIOS
	if (disableTSRFlag) {
		oldDisable = *disableTSRFlag;
		*disableTSRFlag = 0;
		}

	Begin_Nest_V86_Exec();
	LoadV86Registers(&saveRegs,in,&sregs);
	Exec_Int(intno);
	ReadV86Registers(&saveRegs,out,&sregs);
	End_Nest_Exec();

	// Re-enable pass-up to our VxD handler if previously enabled
	if (disableTSRFlag)
		*disableTSRFlag = oldDisable;

	TRACE("SDDHELP: Exiting PM_int86()\n");
	return out->x.ax;
}

/****************************************************************************
REMARKS:
Issue a V86 real mode interrupt with the specified register values
loaded before the interrupt.
****************************************************************************/
int PMAPI PM_int86x(
	int intno,
	RMREGS *in,
	RMREGS *out,
	RMSREGS *sregs)
{
	CLIENT_STRUCT	saveRegs;
	ushort			oldDisable;

	TRACE("SDDHELP: Entering PM_int86x()\n");

	// Disable pass-up to our VxD handler so we directly call BIOS
	if (disableTSRFlag) {
		oldDisable = *disableTSRFlag;
		*disableTSRFlag = 0;
		}

	Begin_Nest_V86_Exec();
	LoadV86Registers(&saveRegs,in,sregs);
	Exec_Int(intno);
	ReadV86Registers(&saveRegs,out,sregs);
	End_Nest_Exec();

	// Re-enable pass-up to our VxD handler if previously enabled
	if (disableTSRFlag)
		*disableTSRFlag = oldDisable;

	TRACE("SDDHELP: Exiting PM_int86x()\n");
	return out->x.ax;
}

void PMAPI PM_availableMemory(ulong *physical,ulong *total)
{ *physical = *total = 0; }

/****************************************************************************
REMARKS:
Allocates a block of locked physical memory.
****************************************************************************/
void *	PMAPI PM_allocLockedMem(uint size,ulong *physAddr,ibool below16M)
{
	MEMHANDLE	hMem;
	DWORD 		nPages = (size + 0xFFF) >> 12;
	void		*p;

	PageAllocate(nPages,PG_SYS,0,0,0,below16M ? 0x00FFFFFF : 0xFFFFFFFF,
		physAddr, PAGECONTIG | PAGEFIXED | PAGEUSEALIGN,&hMem,&p);
	return p;
}

/****************************************************************************
REMARKS:
Frees a block of locked physical memory.
****************************************************************************/
void PMAPI PM_freeLockedMem(void *p,uint size)
{
	if (p)
		PageFree((ulong)p,0);
}

/****************************************************************************
REMARKS:
Lock linear memory so it won't be paged.
****************************************************************************/
int PMAPI PM_lockDataPages(void *p,uint len)
{
	DWORD pgNum = (ulong)p >> 12;
	DWORD nPages = (len + (ulong)p - (pgNum << 12) + 0xFFF) >> 12;
	return LinPageLock(pgNum,nPages,0);
}

/****************************************************************************
REMARKS:
Unlock linear memory so it won't be paged.
****************************************************************************/
int PMAPI PM_unlockDataPages(void *p,uint len)
{
	DWORD pgNum = (ulong)p >> 12;
	DWORD nPages = (len + (ulong)p - (pgNum << 12) + 0xFFF) >> 12;
	return LinPageUnLock(pgNum,nPages,0);
}

/****************************************************************************
REMARKS:
Lock linear memory so it won't be paged.
****************************************************************************/
int PMAPI PM_lockCodePages(void (*p)(),uint len)
{
	return PM_lockDataPages((void*)p,len);
}

/****************************************************************************
REMARKS:
Unlock linear memory so it won't be paged.
****************************************************************************/
int PMAPI PM_unlockCodePages(void (*p)(),uint len)
{
	return PM_unlockDataPages((void*)p,len);
}

/****************************************************************************
REMARKS:
Real time clock interrupt handler, which calls the user registered C code.
****************************************************************************/
static BOOL __stdcall RTCInt_Handler(
	VMHANDLE hVM,
	IRQHANDLE hIRQ)
{
	static char inside = 0;

	/* Clear priority interrupt controller and re-enable interrupts so we
	 * dont lock things up for long.
	 */
	VPICD_Phys_EOI(hIRQ);

	/* Clear real-time clock timeout */
	_PM_readCMOS(0x0C);

	/* Now call the C based interrupt handler (but check for mutual
	 * exclusion since we may still be servicing an old interrupt when a
	 * new one comes along; if that happens we ignore the old one).
	 */
	if (!inside) {
		inside = 1;
		enable();
		_PM_rtcHandler();
		inside = 0;
		}
	return TRUE;
}

/****************************************************************************
REMARKS:
Set the real time clock handler (used for software stereo modes).
****************************************************************************/
ibool PMAPI PM_setRealTimeClockHandler(
	PM_intHandler ih,
	int frequency)
{
	struct VPICD_IRQ_Descriptor	IRQdesc;

	/* Save the old CMOS real time clock values */
	_PM_oldCMOSRegA = _PM_readCMOS(0x0A);
	_PM_oldCMOSRegB = _PM_readCMOS(0x0B);

	/* Set the real time clock interrupt handler */
	CHECK(ih != NULL);
	_PM_rtcHandler = ih;
	IRQdesc.VID_IRQ_Number 		= 0x8;
	IRQdesc.VID_Options 		= 0;
	IRQdesc.VID_Hw_Int_Proc 	= (DWORD)VPICD_Thunk_HWInt(RTCInt_Handler, &RTCInt_Thunk);
	IRQdesc.VID_EOI_Proc 		= 0;
	IRQdesc.VID_Virt_Int_Proc 	= 0;
	IRQdesc.VID_Mask_Change_Proc= 0;
	IRQdesc.VID_IRET_Proc 		= 0;
	IRQdesc.VID_IRET_Time_Out 	= 500;
	if ((RTCIRQHandle = VPICD_Virtualize_IRQ(&IRQdesc)) == 0)
		return false;

	/* Program the real time clock default frequency */
	PM_setRealTimeClockFrequency(frequency);

	/* Unmask IRQ8 in the PIC */
	VPICD_Physically_Unmask(RTCIRQHandle);
	return true;
}

/****************************************************************************
REMARKS:
Set the real time clock frequency (for stereo modes).
****************************************************************************/
void PMAPI PM_setRealTimeClockFrequency(
	int frequency)
{
	static short convert[] = {
		8192,
		4096,
		2048,
		1024,
		512,
		256,
		128,
		64,
		32,
		16,
		8,
		4,
		2,
		-1,
		};
	int i;

	/* First clear any pending RTC timeout if not cleared */
	_PM_readCMOS(0x0C);
	if (frequency == 0) {
		/* Disable RTC timout */
		_PM_writeCMOS(0x0A,_PM_oldCMOSRegA);
		_PM_writeCMOS(0x0B,_PM_oldCMOSRegB & 0x0F);
		}
	else {
		/* Convert frequency value to RTC clock indexes */
		for (i = 0; convert[i] != -1; i++) {
			if (convert[i] == frequency)
				break;
			}

		/* Set RTC timout value and enable timeout */
		_PM_writeCMOS(0x0A,(_PM_oldCMOSRegA & 0xF0) | (i+3));
		_PM_writeCMOS(0x0B,(_PM_oldCMOSRegB & 0x0F) | 0x40);
		}
}

/****************************************************************************
REMARKS:
Restore the original real time clock handler.
****************************************************************************/
void PMAPI PM_restoreRealTimeClockHandler(void)
{
	if (RTCIRQHandle) {
		/* Restore CMOS registers and mask RTC clock */
		_PM_writeCMOS(0x0A,_PM_oldCMOSRegA);
		_PM_writeCMOS(0x0B,_PM_oldCMOSRegB);

		/* Restore the interrupt vector */
		VPICD_Set_Auto_Masking(RTCIRQHandle);
		VPICD_Force_Default_Behavior(RTCIRQHandle);
		RTCIRQHandle = 0;
		}
}

/****************************************************************************
REMARKS:
OS specific shared libraries not supported inside a VxD
****************************************************************************/
PM_MODULE PMAPI PM_loadLibrary(
	const char *szDLLName)
{
	(void)szDLLName;
	return NULL;
}

/****************************************************************************
REMARKS:
OS specific shared libraries not supported inside a VxD
****************************************************************************/
void * PMAPI PM_getProcAddress(
	PM_MODULE hModule,
	const char *szProcName)
{
	(void)hModule;
	(void)szProcName;
	return NULL;
}

/****************************************************************************
REMARKS:
OS specific shared libraries not supported inside a VxD
****************************************************************************/
void PMAPI PM_freeLibrary(
	PM_MODULE hModule)
{
	(void)hModule;
}

/****************************************************************************
REMARKS:
Function to find the first file matching a search criteria in a directory.
****************************************************************************/
void *PMAPI PM_findFirstFile(
	const char *filename,
	PM_findData *findData)
{
	// TODO: This function should start a directory enumeration search
	//       given the filename (with wildcards). The data should be
	//		 converted and returned in the findData standard form.
	(void)filename;
	(void)findData;
	return PM_FILE_INVALID;
}

/****************************************************************************
REMARKS:
Function to find the next file matching a search criteria in a directory.
****************************************************************************/
ibool PMAPI PM_findNextFile(
	void *handle,
	PM_findData *findData)
{
	// TODO: This function should find the next file in directory enumeration
	//		 search given the search criteria defined in the call to
	//		 PM_findFirstFile. The data should be converted and returned
	//	 	 in the findData standard form.
	(void)handle;
	(void)findData;
	return false;
}

/****************************************************************************
REMARKS:
Function to close the find process
****************************************************************************/
void PMAPI PM_findClose(
	void *handle)
{
	// TODO: This function should close the find process. This may do
	//		 nothing for some OS'es.
	(void)handle;
}

/****************************************************************************
REMARKS:
Function to determine if a drive is a valid drive or not. Under Unix this
function will return false for anything except a value of 3 (considered
the root drive, and equivalent to C: for non-Unix systems). The drive
numbering is:

	1	- Drive A:
	2 	- Drive B:
	3	- Drive C:
	etc

****************************************************************************/
ibool PMAPI PM_driveValid(
	char drive)
{
	// Not supported in a VxD
	(void)drive;
	return false;
}

/****************************************************************************
REMARKS:
Function to get the current working directory for the specififed drive.
Under Unix this will always return the current working directory regardless
of what the value of 'drive' is.
****************************************************************************/
void PMAPI PM_getdcwd(
	int drive,
	char *dir,
	int len)
{
	// Not supported in a VxD
	(void)drive;
	(void)dir;
	(void)len;
}

/****************************************************************************
PARAMETERS:
base	- The starting physical base address of the region
size	- The size in bytes of the region
type	- Type to place into the MTRR register

RETURNS:
Error code describing the result.

REMARKS:
Function to enable write combining for the specified region of memory.
****************************************************************************/
int PMAPI PM_enableWriteCombine(
	ulong base,
	ulong size,
	uint type)
{
	return MTRR_enableWriteCombine(base,size,type);
}

/****************************************************************************
REMARKS:
Function to change the file attributes for a specific file.
****************************************************************************/
void PMAPI PM_setFileAttr(
	const char *filename,
	uint attrib)
{
	// TODO: Implement this
	(void)filename;
	(void)attrib;
	PM_fatalError("PM_setFileAttr not implemented yet!");
}

/****************************************************************************
REMARKS:
Function to get the file attributes for a specific file.
****************************************************************************/
uint PMAPI PM_getFileAttr(
	const char *filename)
{
	// TODO: Implement this
	(void)filename;
	PM_fatalError("PM_getFileAttr not implemented yet!");
	return 0;
}

/****************************************************************************
REMARKS:
Function to create a directory.
****************************************************************************/
ibool PMAPI PM_mkdir(
	const char *filename)
{
	// TODO: Implement this
	(void)filename;
	PM_fatalError("PM_mkdir not implemented yet!");
	return false;
}

/****************************************************************************
REMARKS:
Function to remove a directory.
****************************************************************************/
ibool PMAPI PM_rmdir(
	const char *filename)
{
	// TODO: Implement this
	(void)filename;
	PM_fatalError("PM_rmdir not implemented yet!");
	return false;
}

/****************************************************************************
REMARKS:
Function to get the file time and date for a specific file.
****************************************************************************/
void PMAPI PM_getFileTime(
	const char *filename,
	ibool gmTime,
	PM_time *time)
{
	// TODO: Implement this!
	(void)filename;
	(void)gmTime;
	(void)time;
	PM_fatalError("PM_getFileTime not implemented yet!");
}

/****************************************************************************
REMARKS:
Function to set the file time and date for a specific file.
****************************************************************************/
void PMAPI PM_setFileTime(
	const char *filename,
	ibool gmTime,
	PM_time *time)
{
	// TODO: Implement this!
	(void)filename;
	(void)gmTime;
	(void)time;
	PM_fatalError("PM_setFileTime not implemented yet!");
}
