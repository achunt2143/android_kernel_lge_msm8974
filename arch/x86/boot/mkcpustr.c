<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2008 rPath, Inc. - All Rights Reserved
 *
<<<<<<< HEAD
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2 or (at your
 *   option) any later version; incorporated herein by reference.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ----------------------------------------------------------------------- */

/*
 * This is a host program to preprocess the CPU strings into a
 * compact format suitable for the setup code.
 */

#include <stdio.h>

<<<<<<< HEAD
=======
#include "../include/asm/required-features.h"
#include "../include/asm/disabled-features.h"
#include "../include/asm/cpufeatures.h"
#include "../include/asm/vmxfeatures.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "../kernel/cpu/capflags.c"

int main(void)
{
	int i, j;
	const char *str;

	printf("static const char x86_cap_strs[] =\n");

	for (i = 0; i < NCAPINTS; i++) {
		for (j = 0; j < 32; j++) {
			str = x86_cap_flags[i*32+j];

			if (i == NCAPINTS-1 && j == 31) {
				/* The last entry must be unconditional; this
				   also consumes the compiler-added null
				   character */
				if (!str)
					str = "";
				printf("\t\"\\x%02x\\x%02x\"\"%s\"\n",
				       i, j, str);
			} else if (str) {
				printf("#if REQUIRED_MASK%d & (1 << %d)\n"
				       "\t\"\\x%02x\\x%02x\"\"%s\\0\"\n"
				       "#endif\n",
				       i, j, i, j, str);
			}
		}
	}
	printf("\t;\n");
	return 0;
}
