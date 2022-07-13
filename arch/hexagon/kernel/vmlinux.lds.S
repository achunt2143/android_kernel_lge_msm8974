<<<<<<< HEAD
/*
 * Linker script for Hexagon kernel
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#define LOAD_OFFSET PAGE_OFFSET

=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Linker script for Hexagon kernel
 *
 * Copyright (c) 2010-2014, The Linux Foundation. All rights reserved.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/vmlinux.lds.h>
#include <asm/asm-offsets.h>	/*  Most of the kernel defines are here  */
#include <asm/mem-layout.h>	/*  except for page_offset  */
#include <asm/cache.h>		/*  and now we're pulling cache line size  */
<<<<<<< HEAD
=======
#include <asm/thread_info.h>	/*  and we need THREAD_SIZE too */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
OUTPUT_ARCH(hexagon)
ENTRY(stext)

jiffies = jiffies_64;

/*
See asm-generic/vmlinux.lds.h for expansion of some of these macros.
See asm-generic/sections.h for seemingly required labels.
*/

#define PAGE_SIZE _PAGE_SIZE

<<<<<<< HEAD
/*  This LOAD_OFFSET is temporary for debugging on the simulator; it may change
    for hypervisor pseudo-physical memory.  */


SECTIONS
{
	. = PAGE_OFFSET + LOAD_ADDRESS;
=======
SECTIONS
{
	. = PAGE_OFFSET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__init_begin = .;
	HEAD_TEXT_SECTION
	INIT_TEXT_SECTION(PAGE_SIZE)
	PERCPU_SECTION(L1_CACHE_BYTES)
	__init_end = .;

        . = ALIGN(_PAGE_SIZE);
	_stext = .;
<<<<<<< HEAD
	.text : AT(ADDR(.text) - LOAD_OFFSET) {
		_text = .;
		TEXT_TEXT
=======
	.text : AT(ADDR(.text)) {
		_text = .;
		TEXT_TEXT
		IRQENTRY_TEXT
		SOFTIRQENTRY_TEXT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		SCHED_TEXT
		LOCK_TEXT
		KPROBES_TEXT
		*(.fixup)
	}
	_etext = .;

	INIT_DATA_SECTION(PAGE_SIZE)

	_sdata = .;
<<<<<<< HEAD
		RW_DATA_SECTION(32,PAGE_SIZE,PAGE_SIZE)
		RO_DATA_SECTION(PAGE_SIZE)
	_edata = .;

	EXCEPTION_TABLE(16)
	NOTES
=======
		RW_DATA(32,PAGE_SIZE,_THREAD_SIZE)
		RO_DATA(PAGE_SIZE)
	_edata = .;

	EXCEPTION_TABLE(16)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BSS_SECTION(_PAGE_SIZE, _PAGE_SIZE, _PAGE_SIZE)

	_end = .;

<<<<<<< HEAD
	/DISCARD/ : {
		EXIT_TEXT
		EXIT_DATA
		EXIT_CALL
	}

	STABS_DEBUG
	DWARF_DEBUG

=======
	STABS_DEBUG
	DWARF_DEBUG
	ELF_DETAILS
	.hexagon.attributes 0 : { *(.hexagon.attributes) }

	DISCARDS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
