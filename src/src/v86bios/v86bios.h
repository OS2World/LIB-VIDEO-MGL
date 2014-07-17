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
#ifndef V86_BIOS_H
#define V86_BIOS_H

#include <stdio.h>

#ifdef V86BIOS_DEBUG
#undef PRINT_PORT
#define PRINT_PORT
#undef PRINT_PCI
#define PRINT_PCI
#undef PRINT_IRQ
#define PRINT_IRQ
#endif

#ifdef PRINT_PORT
#define p_printf(f,a...) fprintf(stderr,f,##a)
#else
#define p_printf(f,a...)
#endif
#ifdef PRINT_IRQ
#define i_printf(f,a...) fprintf(stderr,f,##a)
#else
#define i_printf(f,a...)
#endif
#ifdef PRINT_PCI
#define P_printf(f,a...) fprintf(stderr,f,##a)
#else
#define P_printf(f,a...)
#endif

typedef unsigned char CARD8;
typedef unsigned short CARD16;
typedef unsigned int CARD32;

struct regs86 {
		long ebx;
		long ecx;
		long edx;
		long esi;
		long edi;
		long ebp;
		long eax;
		long eip;
		long esp;
		unsigned short cs;
		unsigned short ss;
		unsigned short es;
		unsigned short ds;
		unsigned short fs;
		unsigned short gs;
	long eflags;
};

typedef struct {
	int fd;
	int vt;
} console;

#ifdef __alpha__
unsigned long _bus_base(void);
extern void* vram_map;
extern int sparse_shift;
#endif

extern void do_x86(unsigned long bios_start,CARD16 ax);
extern int run_bios_int(int num, struct regs86 *regs);
extern CARD32 getIntVect(int num);
CARD32 getIP(void);

extern int port_rep_inb(CARD16 port, CARD8 *base, int d_f, CARD32 count);
extern int port_rep_inw(CARD16 port, CARD16 *base, int d_f, CARD32 count);
extern int port_rep_inl(CARD16 port, CARD32 *base, int d_f, CARD32 count);
extern int port_rep_outb(CARD16 port, CARD8 *base, int d_f, CARD32 count);
extern int port_rep_outw(CARD16 port, CARD16 *base, int d_f, CARD32 count);
extern int port_rep_outl(CARD16 port, CARD32 *base, int d_f, CARD32 count);
extern CARD8 p_inb(CARD16 port);
extern CARD16 p_inw(CARD16 port);
extern CARD32 p_inl(CARD16 port);
extern void p_outb(CARD16 port, CARD8 val);
extern void p_outw(CARD16 port, CARD16 val);
extern void p_outl(CARD16 port, CARD32 val);

#ifdef __alpha__
CARD8 mem_rb(CARD32 addr);
CARD16 mem_rw(CARD32 addr);
CARD32 mem_rl(CARD32 addr);
void mem_wb(CARD32 addr, CARD8 val);
void mem_ww(CARD32 addr, CARD16 val);
void mem_wl(CARD32 addr, CARD32 val);
#endif

extern void io_statistics(void);

extern int int_handler(int num, struct regs86 *regs);

extern console open_console(void);
extern void close_console(console);

extern void dprint(unsigned long start, unsigned long size);

#define MEM_FILE "/dev/mem"
#define DEFAULT_V_BIOS 0xc0000
#ifndef V_BIOS
#define V_BIOS DEFAULT_V_BIOS
#endif

#ifdef __alpha__
#define NEED_PCI_IO
#endif

#endif

