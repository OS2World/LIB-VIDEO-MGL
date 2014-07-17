/*
 * Copyright 1999 Egbert Eich
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is hereby granted without fee, provided that
 * the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of the authors not be used in
 * advertising or publicity pertaining to distribution of the software without
 * specific, written prior permission.  The authors makes no representations
 * about the suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 *
 * THE AUTHORS DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT SHALL THE AUTHORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
#include "debug.h"

#define IF_MASK		0x00000200
#define VIF_MASK	0x00080000	/* virtual interrupt flag */
#define VIP_MASK	0x00100000	/* virtual interrupt pending */

#include </usr/include/unistd.h>
#include <errno.h>
#include <asm/unistd.h>
//#include <syscall-list.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#ifdef __alpha__
#include <sys/io.h>
#endif
#include "v86bios.h"
#include "AsmMacros.h"
#if defined(PRINT_IP) || defined(TRACE)
# define DEBUG
#endif
#include "x86emu.h"
#undef DEBUG
#ifndef PRINT_PORT
#define p_inb inb
#define p_inw inw
#define p_outb outb
#define p_outw outw
#endif
#if !defined(PRINT_PORT) && !defined(NEED_PCI_IO)
#define p_inl inl
#define p_outl outl
#endif

#define M            _X86EMU_env
#define CPU_REG(reg) M.x86.R_##reg

void
x86emu_do_int(int num)
{
	struct regs86 regs;
		
	i_printf("int 0x%x received: ax:0x%x\n",num,CPU_REG(AX));

	/* try to run bios interrupt */
	
	/* if not installed fall back */
#define COPY(x,y) regs.y = M.x86.x
#define COPY_R(x,y) M.x86.x = regs.y
	
	COPY(R_EAX,eax);
	COPY(R_EBX,ebx);
	COPY(R_ECX,ecx);
	COPY(R_EDX,edx);
	COPY(R_ESI,esi);
	COPY(R_EDI,edi);
	COPY(R_EBP,ebp);
	COPY(R_EIP,eip);
	COPY(R_ESP,esp);
	COPY(R_CS,cs);
	COPY(R_SS,ss);
	COPY(R_DS,ds);
	COPY(R_ES,es);
	COPY(R_FS,fs);
	COPY(R_GS,gs);
	COPY(R_EFLG,eflags);

	if (!(int_handler(num,&regs))) {
		if (!run_bios_int(num,&regs))
			goto unknown_int;
		else
			return;
	}
	
	COPY_R(R_EAX,eax);
	COPY_R(R_EBX,ebx);
	COPY_R(R_ECX,ecx);
	COPY_R(R_EDX,edx);
	COPY_R(R_ESI,esi);
	COPY_R(R_EDI,edi);
	COPY_R(R_EBP,ebp);
	COPY_R(R_EIP,eip);
	COPY_R(R_ESP,esp);
	COPY_R(R_CS,cs);
	COPY_R(R_SS,ss);
	COPY_R(R_DS,ds);
	COPY_R(R_ES,es);
	COPY_R(R_FS,fs);
	COPY_R(R_GS,gs);
	COPY_R(R_EFLG,eflags);
	return;

 unknown_int:
	fprintf(stderr,"\nUnknown vm86_int: %X\n\n",num);
	X86EMU_halt_sys();
	return;
	
#undef COPY
#undef COPY_R
}

void
setup_x86emu(unsigned long bios_start, CARD16 ax)
{
	int i;
	CARD32 eip;
	CARD16 cs;
	X86EMU_intrFuncs intFuncs[256];

#ifdef PRINT_PORT
	X86EMU_pioFuncs pioFuncs = {
		(u8(*)(u16))p_inb,
		(u16(*)(u16))p_inw,
		(u32(*)(u16))p_inl,
		(void(*)(u16,u8))p_outb,
		(void(*)(u16,u16))p_outw,
		(void(*)(u16,u32))p_outl
	};
#else
	X86EMU_pioFuncs pioFuncs = {
		(u8(*)(u16))inb,
		(u16(*)(u16))inw,
		(u32(*)(u16))inl,
		(void(*)(u16,u8))outb,
		(void(*)(u16,u16))outw,
		(void(*)(u16,u32))outl
	};
#endif
#ifdef __alpha__
	X86EMU_memFuncs memFuncs = {
	  (u8(*)(u32))mem_rb,
	  (u16(*)(u32))mem_rw,
	  (u32(*)(u32))mem_rl,
	  (void(*)(u32,u8))mem_wb,
	  (void(*)(u32,u16))mem_ww,
	  (void(*)(u32,u32))mem_wl
	};
#endif
	M.mem_base = 0;
	M.mem_size = 1024*1024 + 1024;
	//  M.x86.debug = DEBUG_DISASSEMBLE_F | DEBUG_TRACE_F | DEBUG_DECODE_F;
	//  M.x86.debug |= DEBUG_DECODE_F |  DEBUG_TRACE_F;
/*
 * For single step tracing compile x86emu with option -DDEBUG
 * and define TRACE
 */
	M.x86.debug = 0;
#ifdef PRINT_IP
	M.x86.debug = DEBUG_SAVE_CS_IP;
#endif
#ifdef TRACE
	X86EMU_trace_on();
#endif
	X86EMU_setupPioFuncs(&pioFuncs);
#ifdef __alpha__
	X86EMU_setupMemFuncs(&memFuncs);
#endif
	for (i=0;i<256;i++)
		intFuncs[i] = x86emu_do_int;
	X86EMU_setupIntrFuncs(intFuncs);

	eip = bios_start & 0xFFFF;
	cs = (bios_start & 0xFF0000) >> 4;
	
	CPU_REG(EAX) = (CARD32) ax;
	CPU_REG(EBX) = 0;
	CPU_REG(ECX) = 0;
	CPU_REG(EDX) = 0;
	CPU_REG(ESI) = 0;
	CPU_REG(EDI) = 0;
	CPU_REG(EBP) = 0;
	CPU_REG(EIP) = eip;
	CPU_REG(CS) = cs;               
	CPU_REG(SP) = 0x100;
	CPU_REG(SS) = 0x30;               /* This is the standard pc bios stack */
	CPU_REG(ES) = 0;                  /* standard pc es */
	CPU_REG(DS) = 0x40;               /* standard pc ds */
	CPU_REG(FS) = 0;
	CPU_REG(GS) = 0;
	CPU_REG(EFLG) |= (VIF_MASK | VIP_MASK | IF_MASK | 0x2);
}

void
do_x86emu(void)
{
	X86EMU_exec();
}

void
do_x86(unsigned long bios_start, CARD16 ax)
{
	setup_x86emu(bios_start,ax);
	do_x86emu();
}

int
run_bios_int(int num, struct regs86 *regs)
{
#ifdef V86BIOS_DEBUG
	static int firsttime = 1;
#endif
	/* check if bios vector is initialized */
	if (((CARD16*)0)[(num<<1)+1] == 0xF000) { /* SYS_BIOS_SEG ?*/
#ifdef V86BIOS_DEBUG
		i_printf("card BIOS not loaded\n");
#endif
		return 0;
	}
	
#ifdef V86BIOS_DEBUG
	if (firsttime) {
		dprint(0,0x3D0);
		firsttime = 0;
	}
#endif
	
	i_printf("calling card BIOS at: ");
	i_printf("0x%x:%x\n",((CARD16 *) 0)[(num << 1) + 1],
			 (CARD32)((CARD16 *) 0)[num << 1]);
	X86EMU_prepareForInt(num);
	
	return 1;
}

CARD32
getIntVect(int num)
{
  return ((CARD32*)0)[num];
}

void
printk(const char *fmt, ...)
{
	va_list argptr;
	va_start(argptr, fmt);
	vfprintf(stdout, fmt, argptr);
	fflush(stdout);
	va_end(argptr);
}

#ifdef PRINT_IP
CARD32
getIP(void)
{
    return (M.x86.saved_cs << 4) + M.x86.saved_ip;
}
#endif
