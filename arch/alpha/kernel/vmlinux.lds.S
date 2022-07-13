<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */

#define EMITS_PT_NOTE
#define RO_EXCEPTION_TABLE_ALIGN	16

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/vmlinux.lds.h>
#include <asm/thread_info.h>
#include <asm/cache.h>
#include <asm/page.h>
#include <asm/setup.h>

OUTPUT_FORMAT("elf64-alpha")
OUTPUT_ARCH(alpha)
ENTRY(__start)
<<<<<<< HEAD
PHDRS { kernel PT_LOAD; note PT_NOTE; }
=======
PHDRS { text PT_LOAD; note PT_NOTE; }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
jiffies = jiffies_64;
SECTIONS
{
#ifdef CONFIG_ALPHA_LEGACY_START_ADDRESS
	. = 0xfffffc0000310000;
#else
	. = 0xfffffc0001010000;
#endif

	_text = .;	/* Text and read-only data */
	.text : {
		HEAD_TEXT
		TEXT_TEXT
		SCHED_TEXT
		LOCK_TEXT
		*(.fixup)
		*(.gnu.warning)
<<<<<<< HEAD
	} :kernel
	swapper_pg_dir = SWAPPER_PGD;
	_etext = .;	/* End of text section */

	NOTES :kernel :note
	.dummy : {
		*(.dummy)
	} :kernel

	RODATA
	EXCEPTION_TABLE(16)
=======
	} :text
	swapper_pg_dir = SWAPPER_PGD;
	_etext = .;	/* End of text section */

	RO_DATA(4096)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Will be freed after init */
	__init_begin = ALIGN(PAGE_SIZE);
	INIT_TEXT_SECTION(PAGE_SIZE)
	INIT_DATA_SECTION(16)
	PERCPU_SECTION(L1_CACHE_BYTES)
	/* Align to THREAD_SIZE rather than PAGE_SIZE here so any padding page
	   needed for the THREAD_SIZE aligned init_task gets freed after init */
	. = ALIGN(THREAD_SIZE);
	__init_end = .;
	/* Freed after init ends here */

	_sdata = .;	/* Start of rw data section */
	_data = .;
<<<<<<< HEAD
	RW_DATA_SECTION(L1_CACHE_BYTES, PAGE_SIZE, THREAD_SIZE)
=======
	RW_DATA(L1_CACHE_BYTES, PAGE_SIZE, THREAD_SIZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.got : {
		*(.got)
	}
	.sdata : {
		*(.sdata)
	}
	_edata = .;	/* End of data section */

	BSS_SECTION(0, 0, 0)
	_end = .;

	.mdebug 0 : {
		*(.mdebug)
	}
	.note 0 : {
		*(.note)
	}

	STABS_DEBUG
	DWARF_DEBUG
<<<<<<< HEAD
=======
	ELF_DETAILS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	DISCARDS
}
