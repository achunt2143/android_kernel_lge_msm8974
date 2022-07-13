<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Access to VGA videoram
 *
 *	(c) 1998 Martin Mares <mj@ucw.cz>
 */
#ifndef _ASM_VGA_H
#define _ASM_VGA_H

<<<<<<< HEAD
=======
#include <linux/string.h>
#include <asm/addrspace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/byteorder.h>

/*
 *	On the PC, we can just recalculate addresses and then
 *	access the videoram directly without any black magic.
 */

<<<<<<< HEAD
#define VGA_MAP_MEM(x, s)	(0xb0000000L + (unsigned long)(x))
=======
#define VGA_MAP_MEM(x, s)	CKSEG1ADDR(0x10000000L + (unsigned long)(x))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define vga_readb(x)	(*(x))
#define vga_writeb(x, y)	(*(y) = (x))

#define VT_BUF_HAVE_RW
/*
 *  These are only needed for supporting VGA or MDA text mode, which use little
 *  endian byte ordering.
 *  In other cases, we can optimize by using native byte ordering and
 *  <linux/vt_buffer.h> has already done the right job for us.
 */

#undef scr_writew
#undef scr_readw

static inline void scr_writew(u16 val, volatile u16 *addr)
{
	*addr = cpu_to_le16(val);
}

static inline u16 scr_readw(volatile const u16 *addr)
{
	return le16_to_cpu(*addr);
}

<<<<<<< HEAD
=======
static inline void scr_memsetw(u16 *s, u16 v, unsigned int count)
{
	memset16(s, cpu_to_le16(v), count / 2);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define scr_memcpyw(d, s, c) memcpy(d, s, c)
#define scr_memmovew(d, s, c) memmove(d, s, c)
#define VT_BUF_HAVE_MEMCPYW
#define VT_BUF_HAVE_MEMMOVEW
<<<<<<< HEAD
=======
#define VT_BUF_HAVE_MEMSETW
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_VGA_H */
