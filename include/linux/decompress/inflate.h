<<<<<<< HEAD
#ifndef INFLATE_H
#define INFLATE_H

int gunzip(unsigned char *inbuf, int len,
	   int(*fill)(void*, unsigned int),
	   int(*flush)(void*, unsigned int),
	   unsigned char *output,
	   int *pos,
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_DECOMPRESS_INFLATE_H
#define LINUX_DECOMPRESS_INFLATE_H

int gunzip(unsigned char *inbuf, long len,
	   long (*fill)(void*, unsigned long),
	   long (*flush)(void*, unsigned long),
	   unsigned char *output,
	   long *pos,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	   void(*error_fn)(char *x));
#endif
