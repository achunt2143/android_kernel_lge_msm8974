<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * hvconsole.h
 * Copyright (C) 2004 Ryan S Arnold, IBM Corporation
 *
 * LPAR console support.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _PPC64_HVCONSOLE_H
#define _PPC64_HVCONSOLE_H
#ifdef __KERNEL__

/*
 * PSeries firmware will only send/recv up to 16 bytes of character data per
 * hcall.
 */
#define MAX_VIO_PUT_CHARS	16
#define SIZE_VIO_GET_CHARS	16

/*
 * Vio firmware always attempts to fetch MAX_VIO_GET_CHARS chars.  The 'count'
 * parm is included to conform to put_chars() function pointer template
 */
<<<<<<< HEAD
extern int hvc_get_chars(uint32_t vtermno, char *buf, int count);
extern int hvc_put_chars(uint32_t vtermno, const char *buf, int count);
=======
extern ssize_t hvc_get_chars(uint32_t vtermno, u8 *buf, size_t count);
extern ssize_t hvc_put_chars(uint32_t vtermno, const u8 *buf, size_t count);

/* Provided by HVC VIO */
void hvc_vio_init_early(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __KERNEL__ */
#endif /* _PPC64_HVCONSOLE_H */
