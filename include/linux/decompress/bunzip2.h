<<<<<<< HEAD
#ifndef DECOMPRESS_BUNZIP2_H
#define DECOMPRESS_BUNZIP2_H

int bunzip2(unsigned char *inbuf, int len,
	    int(*fill)(void*, unsigned int),
	    int(*flush)(void*, unsigned int),
	    unsigned char *output,
	    int *pos,
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef DECOMPRESS_BUNZIP2_H
#define DECOMPRESS_BUNZIP2_H

int bunzip2(unsigned char *inbuf, long len,
	    long (*fill)(void*, unsigned long),
	    long (*flush)(void*, unsigned long),
	    unsigned char *output,
	    long *pos,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    void(*error)(char *x));
#endif
