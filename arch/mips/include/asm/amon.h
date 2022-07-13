/*
<<<<<<< HEAD
 * Amon support
 */

int amon_cpu_avail(int);
void amon_cpu_start(int, unsigned long, unsigned long,
		    unsigned long, unsigned long);
=======
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2013 Imagination Technologies Ltd.
 *
 * Arbitrary Monitor Support (AMON)
 */
int amon_cpu_avail(int cpu);
int amon_cpu_start(int cpu, unsigned long pc, unsigned long sp,
		   unsigned long gp, unsigned long a0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
