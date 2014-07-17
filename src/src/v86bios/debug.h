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
//#define V86BIOS_DEBUG

/*
 * uncomment the following if needed
 * should be command line options
 */

//#define PRINT_PORT
//#define PRINT_IRQ
//#define PRINT_PCI
//#define PRINT_IP    /* print IP address with PIO information */
//#define TRACE       /* turn on debugger in x86emu            */
                      /* requires x86emu compiled with -DDEBUG */

/*
 * these should not be here.
 * Should be converted to command line options.
 */
#define CONFIG_ACTIVE_DEVICE 0
#define SAVE_BIOS 0
#define MAP_SYS_BIOS 1
#define RESORT 1
#define FIX_ROM 0

//#define V_BIOS 0xe0000
//#define V_BIOS 0xe4000

#if defined(PRINT_IO) && !defined(PRINT_PORT)
# undef PRINT_IO
#endif
