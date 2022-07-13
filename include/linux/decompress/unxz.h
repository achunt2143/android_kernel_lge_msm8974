/*
 * Wrapper for decompressing XZ-compressed kernel, initramfs, and initrd
 *
 * Author: Lasse Collin <lasse.collin@tukaani.org>
 *
 * This file has been put into the public domain.
 * You can do whatever you want with this file.
 */

#ifndef DECOMPRESS_UNXZ_H
#define DECOMPRESS_UNXZ_H

<<<<<<< HEAD
int unxz(unsigned char *in, int in_size,
	 int (*fill)(void *dest, unsigned int size),
	 int (*flush)(void *src, unsigned int size),
	 unsigned char *out, int *in_used,
=======
int unxz(unsigned char *in, long in_size,
	 long (*fill)(void *dest, unsigned long size),
	 long (*flush)(void *src, unsigned long size),
	 unsigned char *out, long *in_used,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 void (*error)(char *x));

#endif
