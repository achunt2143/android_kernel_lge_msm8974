<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This file is part of the Linux kernel.
 *
 * Copyright (c) 2011, Intel Corporation
 * Authors: Fenghua Yu <fenghua.yu@intel.com>,
 *          H. Peter Anvin <hpa@linux.intel.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
=======
 */
#include <linux/printk.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/processor.h>
#include <asm/archrandom.h>
#include <asm/sections.h>

<<<<<<< HEAD
static int __init x86_rdrand_setup(char *s)
{
	setup_clear_cpu_cap(X86_FEATURE_RDRAND);
	return 1;
}
__setup("nordrand", x86_rdrand_setup);

/* We can't use arch_get_random_long() here since alternatives haven't run */
static inline int rdrand_long(unsigned long *v)
{
	int ok;
	asm volatile("1: " RDRAND_LONG "\n\t"
		     "jc 2f\n\t"
		     "decl %0\n\t"
		     "jnz 1b\n\t"
		     "2:"
		     : "=r" (ok), "=a" (*v)
		     : "0" (RDRAND_RETRY_LOOPS));
	return ok;
}

/*
 * Force a reseed cycle; we are architecturally guaranteed a reseed
 * after no more than 512 128-bit chunks of random data.  This also
 * acts as a test of the CPU capability.
 */
#define RESEED_LOOP ((512*128)/sizeof(unsigned long))

void __cpuinit x86_init_rdrand(struct cpuinfo_x86 *c)
{
#ifdef CONFIG_ARCH_RANDOM
	unsigned long tmp;
	int i, count, ok;

	if (!cpu_has(c, X86_FEATURE_RDRAND))
		return;		/* Nothing to do */

	for (count = i = 0; i < RESEED_LOOP; i++) {
		ok = rdrand_long(&tmp);
		if (ok)
			count++;
	}

	if (count != RESEED_LOOP)
		clear_cpu_cap(c, X86_FEATURE_RDRAND);
#endif
=======
/*
 * RDRAND has Built-In-Self-Test (BIST) that runs on every invocation.
 * Run the instruction a few times as a sanity check. Also make sure
 * it's not outputting the same value over and over, which has happened
 * as a result of past CPU bugs.
 *
 * If it fails, it is simple to disable RDRAND and RDSEED here.
 */

void x86_init_rdrand(struct cpuinfo_x86 *c)
{
	enum { SAMPLES = 8, MIN_CHANGE = 5 };
	unsigned long sample, prev;
	bool failure = false;
	size_t i, changed;

	if (!cpu_has(c, X86_FEATURE_RDRAND))
		return;

	for (changed = 0, i = 0; i < SAMPLES; ++i) {
		if (!rdrand_long(&sample)) {
			failure = true;
			break;
		}
		changed += i && sample != prev;
		prev = sample;
	}
	if (changed < MIN_CHANGE)
		failure = true;

	if (failure) {
		clear_cpu_cap(c, X86_FEATURE_RDRAND);
		clear_cpu_cap(c, X86_FEATURE_RDSEED);
		pr_emerg("RDRAND is not reliable on this platform; disabling.\n");
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
