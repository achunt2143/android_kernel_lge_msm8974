<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OpenRISC vmlinux.lds.S
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
<<<<<<< HEAD
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ld script for OpenRISC architecture
 */

/* TODO
 *		- clean up __offset & stuff
<<<<<<< HEAD
 *		- change all 8192 aligment to PAGE !!!
 *		- recheck if all aligments are really needed
=======
 *		- change all 8192 alignment to PAGE !!!
 *		- recheck if all alignments are really needed
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#  define LOAD_OFFSET  PAGE_OFFSET
#  define LOAD_BASE    PAGE_OFFSET

#include <asm/page.h>
#include <asm/cache.h>
<<<<<<< HEAD
#include <asm-generic/vmlinux.lds.h>

OUTPUT_FORMAT("elf32-or32", "elf32-or32", "elf32-or32")
=======
#include <asm/thread_info.h>
#include <asm-generic/vmlinux.lds.h>

#ifdef __OR1K__
#define __OUTPUT_FORMAT        "elf32-or1k"
#else
#define __OUTPUT_FORMAT        "elf32-or32"
#endif

OUTPUT_FORMAT(__OUTPUT_FORMAT, __OUTPUT_FORMAT, __OUTPUT_FORMAT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
jiffies = jiffies_64 + 4;

SECTIONS
{
        /* Read-only sections, merged into text segment: */
        . = LOAD_BASE ;

<<<<<<< HEAD
=======
	_text = .;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* _s_kernel_ro must be page aligned */
	. = ALIGN(PAGE_SIZE);
	_s_kernel_ro = .;

        .text                   : AT(ADDR(.text) - LOAD_OFFSET)
	{
          _stext = .;
	  TEXT_TEXT
	  SCHED_TEXT
	  LOCK_TEXT
	  KPROBES_TEXT
	  IRQENTRY_TEXT
<<<<<<< HEAD
=======
	  SOFTIRQENTRY_TEXT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	  *(.fixup)
	  *(.text.__*)
	  _etext = .;
	}
	/* TODO: Check if fixup and text.__* are really necessary
	 * fixup is definitely necessary
	 */

	_sdata = .;

<<<<<<< HEAD
	/* Page alignment required for RO_DATA_SECTION */
	RO_DATA_SECTION(PAGE_SIZE)
=======
	/* Page alignment required for RO_DATA */
	RO_DATA(PAGE_SIZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	_e_kernel_ro = .;

	/* Whatever comes after _e_kernel_ro had better be page-aligend, too */

	/* 32 here is cacheline size... recheck this */
<<<<<<< HEAD
	RW_DATA_SECTION(32, PAGE_SIZE, PAGE_SIZE)
=======
	RW_DATA(32, PAGE_SIZE, PAGE_SIZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

        _edata  =  .;

	EXCEPTION_TABLE(4)
<<<<<<< HEAD
	NOTES
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Init code and data */
	. = ALIGN(PAGE_SIZE);
	__init_begin = .;

	HEAD_TEXT_SECTION

	/* Page aligned */
	INIT_TEXT_SECTION(PAGE_SIZE)

	/* Align __setup_start on 16 byte boundary */
	INIT_DATA_SECTION(16)

	PERCPU_SECTION(L1_CACHE_BYTES)

        __init_end = .;

<<<<<<< HEAD
	. = ALIGN(PAGE_SIZE);
	.initrd			: AT(ADDR(.initrd) - LOAD_OFFSET)
	{
		__initrd_start = .;
		*(.initrd)
		__initrd_end = .;
		FILL (0);
                . = ALIGN (PAGE_SIZE);
	}

        __vmlinux_end = .;            /* last address of the physical file */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BSS_SECTION(0, 0, 0x20)

        _end = .;

	/* Throw in the debugging sections */
	STABS_DEBUG
	DWARF_DEBUG
<<<<<<< HEAD
=======
	ELF_DETAILS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

        /* Sections to be discarded -- must be last */
	DISCARDS
}
