/*
 * arch/xtensa/kernel/vmlinux.lds.S
 *
 * Xtensa linker script
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2008 Tensilica Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Chris Zankel <chris@zankel.net>
 * Marc Gauthier <marc@tensilica.com, marc@alumni.uwaterloo.ca>
 * Joe Taylor <joe@tensilica.com, joetylr@yahoo.com>
 */

<<<<<<< HEAD
=======
#define RO_EXCEPTION_TABLE_ALIGN	16

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/vmlinux.lds.h>
#include <asm/page.h>
#include <asm/thread_info.h>

<<<<<<< HEAD
#include <variant/core.h>
#include <platform/hardware.h>
=======
#include <asm/core.h>
#include <asm/vectors.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
OUTPUT_ARCH(xtensa)
ENTRY(_start)

#ifdef __XTENSA_EB__
jiffies = jiffies_64 + 4;
#else
jiffies = jiffies_64;
#endif

<<<<<<< HEAD
#ifndef KERNELOFFSET
#define KERNELOFFSET 0xd0001000
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Note: In the following macros, it would be nice to specify only the
   vector name and section kind and construct "sym" and "section" using
   CPP concatenation, but that does not work reliably.  Concatenating a
   string with "." produces an invalid token.  CPP will not print a
   warning because it thinks this is an assembly file, but it leaves
   them as multiple tokens and there may or may not be whitespace
   between them.  */

/* Macro for a relocation entry */

#define RELOCATE_ENTRY(sym, section)		\
	LONG(sym ## _start);			\
	LONG(sym ## _end);			\
	LONG(LOADADDR(section))

<<<<<<< HEAD
/* Macro to define a section for a vector.
 *
 * Use of the MIN function catches the types of errors illustrated in
 * the following example:
 *
 * Assume the section .DoubleExceptionVector.literal is completely
 * full.  Then a programmer adds code to .DoubleExceptionVector.text
 * that produces another literal.  The final literal position will
 * overlay onto the first word of the adjacent code section
 * .DoubleExceptionVector.text.  (In practice, the literals will
 * overwrite the code, and the first few instructions will be
 * garbage.)
 */

#define SECTION_VECTOR(sym, section, addr, max_prevsec_size, prevsec)       \
  section addr : AT((MIN(LOADADDR(prevsec) + max_prevsec_size,		    \
		         LOADADDR(prevsec) + SIZEOF(prevsec)) + 3) & ~ 3)   \
=======
#if !defined(CONFIG_VECTORS_ADDR) && XCHAL_HAVE_VECBASE
#define MERGED_VECTORS 1
#else
#define MERGED_VECTORS 0
#endif

/*
 * Macro to define a section for a vector. When MERGED_VECTORS is 0
 * code for every vector is located with other init data. At startup
 * time head.S copies code for every vector to its final position according
 * to description recorded in the corresponding RELOCATE_ENTRY.
 */

#define SECTION_VECTOR4(sym, section, addr, prevsec)			    \
  section addr : AT(((LOADADDR(prevsec) + SIZEOF(prevsec)) + 3) & ~ 3)      \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  {									    \
    . = ALIGN(4);							    \
    sym ## _start = ABSOLUTE(.);		 			    \
    *(section)								    \
    sym ## _end = ABSOLUTE(.);						    \
  }

<<<<<<< HEAD
=======
#define SECTION_VECTOR2(section, addr)					    \
  . = addr;								    \
  *(section)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Mapping of input sections to output sections when linking.
 */

SECTIONS
{
  . = KERNELOFFSET;
  /* .text section */

  _text = .;
  _stext = .;

  .text :
  {
    /* The HEAD_TEXT section must be the first section! */
    HEAD_TEXT
<<<<<<< HEAD
    TEXT_TEXT
    VMLINUX_SYMBOL(__sched_text_start) = .;
    *(.sched.literal .sched.text)
    VMLINUX_SYMBOL(__sched_text_end) = .;
    VMLINUX_SYMBOL(__lock_text_start) = .;
    *(.spinlock.literal .spinlock.text)
    VMLINUX_SYMBOL(__lock_text_end) = .;

=======

#if MERGED_VECTORS
    . = ALIGN(PAGE_SIZE);
    _vecbase = .;

#ifdef SUPPORT_WINDOWED
    SECTION_VECTOR2 (.WindowVectors.text, WINDOW_VECTORS_VADDR)
#endif
#if XCHAL_EXCM_LEVEL >= 2
    SECTION_VECTOR2 (.Level2InterruptVector.text, INTLEVEL2_VECTOR_VADDR)
#endif
#if XCHAL_EXCM_LEVEL >= 3
    SECTION_VECTOR2 (.Level3InterruptVector.text, INTLEVEL3_VECTOR_VADDR)
#endif
#if XCHAL_EXCM_LEVEL >= 4
    SECTION_VECTOR2 (.Level4InterruptVector.text, INTLEVEL4_VECTOR_VADDR)
#endif
#if XCHAL_EXCM_LEVEL >= 5
    SECTION_VECTOR2 (.Level5InterruptVector.text, INTLEVEL5_VECTOR_VADDR)
#endif
#if XCHAL_EXCM_LEVEL >= 6
    SECTION_VECTOR2 (.Level6InterruptVector.text, INTLEVEL6_VECTOR_VADDR)
#endif
    SECTION_VECTOR2 (.DebugInterruptVector.text, DEBUG_VECTOR_VADDR)
    SECTION_VECTOR2 (.KernelExceptionVector.text, KERNEL_VECTOR_VADDR)
    SECTION_VECTOR2 (.UserExceptionVector.text, USER_VECTOR_VADDR)
    SECTION_VECTOR2 (.DoubleExceptionVector.text, DOUBLEEXC_VECTOR_VADDR)

    *(.exception.text)
    *(.xiptext)
#endif

    IRQENTRY_TEXT
    SOFTIRQENTRY_TEXT
    ENTRY_TEXT
    TEXT_TEXT
    SCHED_TEXT
    LOCK_TEXT
    *(.fixup)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  }
  _etext = .;
  PROVIDE (etext = .);

  . = ALIGN(16);

<<<<<<< HEAD
  RODATA

  /*  Relocation table */

  .fixup   : { *(.fixup) }

  EXCEPTION_TABLE(16)
  /* Data section */

  _sdata = .;
  RW_DATA_SECTION(XCHAL_ICACHE_LINESIZE, PAGE_SIZE, THREAD_SIZE)
=======
  RO_DATA(4096)

  /* Data section */

#ifdef CONFIG_XIP_KERNEL
  INIT_TEXT_SECTION(PAGE_SIZE)
#else
  _sdata = .;
  RW_DATA(XCHAL_ICACHE_LINESIZE, PAGE_SIZE, THREAD_SIZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  _edata = .;

  /* Initialization code and data: */

  . = ALIGN(PAGE_SIZE);
  __init_begin = .;
  INIT_TEXT_SECTION(PAGE_SIZE)

  .init.data :
  {
    INIT_DATA
<<<<<<< HEAD
=======
  }
#endif

  .init.rodata :
  {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    . = ALIGN(0x4);
    __tagtable_begin = .;
    *(.taglist)
    __tagtable_end = .;

    . = ALIGN(16);
    __boot_reloc_table_start = ABSOLUTE(.);

<<<<<<< HEAD
    RELOCATE_ENTRY(_WindowVectors_text,
		   .WindowVectors.text);
=======
#if !MERGED_VECTORS
#ifdef SUPPORT_WINDOWED
    RELOCATE_ENTRY(_WindowVectors_text,
		   .WindowVectors.text);
#endif
#if XCHAL_EXCM_LEVEL >= 2
    RELOCATE_ENTRY(_Level2InterruptVector_text,
		   .Level2InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 3
    RELOCATE_ENTRY(_Level3InterruptVector_text,
		   .Level3InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 4
    RELOCATE_ENTRY(_Level4InterruptVector_text,
		   .Level4InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 5
    RELOCATE_ENTRY(_Level5InterruptVector_text,
		   .Level5InterruptVector.text);
#endif
#if XCHAL_EXCM_LEVEL >= 6
    RELOCATE_ENTRY(_Level6InterruptVector_text,
		   .Level6InterruptVector.text);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    RELOCATE_ENTRY(_KernelExceptionVector_text,
		   .KernelExceptionVector.text);
    RELOCATE_ENTRY(_UserExceptionVector_text,
		   .UserExceptionVector.text);
<<<<<<< HEAD
    RELOCATE_ENTRY(_DoubleExceptionVector_literal,
		   .DoubleExceptionVector.literal);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    RELOCATE_ENTRY(_DoubleExceptionVector_text,
		   .DoubleExceptionVector.text);
    RELOCATE_ENTRY(_DebugInterruptVector_text,
		   .DebugInterruptVector.text);
<<<<<<< HEAD
  
=======
    RELOCATE_ENTRY(_exception_text,
		   .exception.text);
#ifdef CONFIG_XIP_KERNEL
    RELOCATE_ENTRY(_xip_text, .xiptext);
#endif
#endif
#ifdef CONFIG_XIP_KERNEL
    RELOCATE_ENTRY(_xip_data, .data);
    RELOCATE_ENTRY(_xip_init_data, .init.data);
#endif
#if defined(CONFIG_SECONDARY_RESET_VECTOR)
    RELOCATE_ENTRY(_SecondaryResetVector_text,
		   .SecondaryResetVector.text);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    __boot_reloc_table_end = ABSOLUTE(.) ;

    INIT_SETUP(XCHAL_ICACHE_LINESIZE)
    INIT_CALLS
    CON_INITCALL
<<<<<<< HEAD
    SECURITY_INITCALL
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
    INIT_RAM_FS
  }

  PERCPU_SECTION(XCHAL_ICACHE_LINESIZE)

  /* We need this dummy segment here */

  . = ALIGN(4);
  .dummy : { LONG(0) }

<<<<<<< HEAD
  /* The vectors are relocated to the real position at startup time */

  SECTION_VECTOR (_WindowVectors_text,
		  .WindowVectors.text,
		  XCHAL_WINDOW_VECTORS_VADDR, 4,
		  .dummy)
  SECTION_VECTOR (_DebugInterruptVector_literal,
		  .DebugInterruptVector.literal,
		  XCHAL_DEBUG_VECTOR_VADDR - 4,
		  SIZEOF(.WindowVectors.text),
		  .WindowVectors.text)
  SECTION_VECTOR (_DebugInterruptVector_text,
		  .DebugInterruptVector.text,
		  XCHAL_DEBUG_VECTOR_VADDR,
		  4,
		  .DebugInterruptVector.literal)
  SECTION_VECTOR (_KernelExceptionVector_literal,
		  .KernelExceptionVector.literal,
		  XCHAL_KERNEL_VECTOR_VADDR - 4,
		  SIZEOF(.DebugInterruptVector.text),
		  .DebugInterruptVector.text)
  SECTION_VECTOR (_KernelExceptionVector_text,
		  .KernelExceptionVector.text,
		  XCHAL_KERNEL_VECTOR_VADDR,
		  4,
		  .KernelExceptionVector.literal)
  SECTION_VECTOR (_UserExceptionVector_literal,
		  .UserExceptionVector.literal,
		  XCHAL_USER_VECTOR_VADDR - 4,
		  SIZEOF(.KernelExceptionVector.text),
		  .KernelExceptionVector.text)
  SECTION_VECTOR (_UserExceptionVector_text,
		  .UserExceptionVector.text,
		  XCHAL_USER_VECTOR_VADDR,
		  4,
		  .UserExceptionVector.literal)
  SECTION_VECTOR (_DoubleExceptionVector_literal,
		  .DoubleExceptionVector.literal,
		  XCHAL_DOUBLEEXC_VECTOR_VADDR - 16,
		  SIZEOF(.UserExceptionVector.text),
		  .UserExceptionVector.text)
  SECTION_VECTOR (_DoubleExceptionVector_text,
		  .DoubleExceptionVector.text,
		  XCHAL_DOUBLEEXC_VECTOR_VADDR,
		  32,
		  .DoubleExceptionVector.literal)

  . = (LOADADDR( .DoubleExceptionVector.text ) + SIZEOF( .DoubleExceptionVector.text ) + 3) & ~ 3;
  . = ALIGN(PAGE_SIZE);

  __init_end = .;

  BSS_SECTION(0, 8192, 0)

  _end = .;

  /* only used by the boot loader  */

  . = ALIGN(0x10);
  .bootstrap : { *(.bootstrap.literal .bootstrap.text .bootstrap.data) }

  . = ALIGN(0x1000);
  __initrd_start = .;
  .initrd : { *(.initrd) }
  __initrd_end = .;

  .ResetVector.text XCHAL_RESET_VECTOR_VADDR :
  {
    *(.ResetVector.text)
  }

  .xt.lit : { *(.xt.lit) }
  .xt.prop : { *(.xt.prop) }

  .debug  0 :  { *(.debug) }
  .line  0 :  { *(.line) }
  .debug_srcinfo  0 :  { *(.debug_srcinfo) }
  .debug_sfnames  0 :  { *(.debug_sfnames) }
  .debug_aranges  0 :  { *(.debug_aranges) }
  .debug_pubnames  0 :  { *(.debug_pubnames) }
  .debug_info  0 :  { *(.debug_info) }
  .debug_abbrev  0 :  { *(.debug_abbrev) }
  .debug_line  0 :  { *(.debug_line) }
  .debug_frame  0 :  { *(.debug_frame) }
  .debug_str  0 :  { *(.debug_str) }
  .debug_loc  0 :  { *(.debug_loc) }
  .debug_macinfo  0 :  { *(.debug_macinfo) }
  .debug_weaknames  0 :  { *(.debug_weaknames) }
  .debug_funcnames  0 :  { *(.debug_funcnames) }
  .debug_typenames  0 :  { *(.debug_typenames) }
  .debug_varnames  0 :  { *(.debug_varnames) }

  .xt.insn 0 :
  {
    *(.xt.insn)
    *(.gnu.linkonce.x*)
  }

  .xt.lit 0 :
  {
    *(.xt.lit)
    *(.gnu.linkonce.p*)
  }

  /* Sections to be discarded */
  DISCARDS
  /DISCARD/ : { *(.exit.literal) }
=======
#undef LAST
#define LAST	.dummy

#if !MERGED_VECTORS
  /* The vectors are relocated to the real position at startup time */

#ifdef SUPPORT_WINDOWED
  SECTION_VECTOR4 (_WindowVectors_text,
		  .WindowVectors.text,
		  WINDOW_VECTORS_VADDR,
		  LAST)
#undef LAST
#define LAST	.WindowVectors.text
#endif
  SECTION_VECTOR4 (_DebugInterruptVector_text,
		  .DebugInterruptVector.text,
		  DEBUG_VECTOR_VADDR,
		  LAST)
#undef LAST
#define LAST	.DebugInterruptVector.text
#if XCHAL_EXCM_LEVEL >= 2
  SECTION_VECTOR4 (_Level2InterruptVector_text,
		  .Level2InterruptVector.text,
		  INTLEVEL2_VECTOR_VADDR,
		  LAST)
# undef LAST
# define LAST	.Level2InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 3
  SECTION_VECTOR4 (_Level3InterruptVector_text,
		  .Level3InterruptVector.text,
		  INTLEVEL3_VECTOR_VADDR,
		  LAST)
# undef LAST
# define LAST	.Level3InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 4
  SECTION_VECTOR4 (_Level4InterruptVector_text,
		  .Level4InterruptVector.text,
		  INTLEVEL4_VECTOR_VADDR,
		  LAST)
# undef LAST
# define LAST	.Level4InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 5
  SECTION_VECTOR4 (_Level5InterruptVector_text,
		  .Level5InterruptVector.text,
		  INTLEVEL5_VECTOR_VADDR,
		  LAST)
# undef LAST
# define LAST	.Level5InterruptVector.text
#endif
#if XCHAL_EXCM_LEVEL >= 6
  SECTION_VECTOR4 (_Level6InterruptVector_text,
		  .Level6InterruptVector.text,
		  INTLEVEL6_VECTOR_VADDR,
		  LAST)
# undef LAST
# define LAST	.Level6InterruptVector.text
#endif
  SECTION_VECTOR4 (_KernelExceptionVector_text,
		  .KernelExceptionVector.text,
		  KERNEL_VECTOR_VADDR,
		  LAST)
#undef LAST
  SECTION_VECTOR4 (_UserExceptionVector_text,
		  .UserExceptionVector.text,
		  USER_VECTOR_VADDR,
		  .KernelExceptionVector.text)
  SECTION_VECTOR4 (_DoubleExceptionVector_text,
		  .DoubleExceptionVector.text,
		  DOUBLEEXC_VECTOR_VADDR,
		  .UserExceptionVector.text)
#define LAST .DoubleExceptionVector.text

#endif
#if defined(CONFIG_SECONDARY_RESET_VECTOR)

  SECTION_VECTOR4 (_SecondaryResetVector_text,
		  .SecondaryResetVector.text,
		  RESET_VECTOR1_VADDR,
		  LAST)
#undef LAST
#define LAST .SecondaryResetVector.text

#endif
#if !MERGED_VECTORS
  SECTION_VECTOR4 (_exception_text,
		  .exception.text,
		  ,
		  LAST)
#undef LAST
#define LAST .exception.text
  SECTION_VECTOR4 (_xip_text,
		  .xiptext,
		  ,
		  LAST)
#undef LAST
#define LAST .xiptext
#endif
  . = (LOADADDR(LAST) + SIZEOF(LAST) + 3) & ~ 3;

  .dummy1 : AT(ADDR(.dummy1)) { LONG(0) }
  . = ALIGN(PAGE_SIZE);

#ifndef CONFIG_XIP_KERNEL
  __init_end = .;

  BSS_SECTION(0, 8192, 0)
#endif

  _end = .;

#ifdef CONFIG_XIP_KERNEL
  . = CONFIG_XIP_DATA_ADDR;

  _xip_start = .;

#undef LOAD_OFFSET
#define LOAD_OFFSET \
  (CONFIG_XIP_DATA_ADDR - (LOADADDR(.dummy1) + SIZEOF(.dummy1) + 3) & ~ 3)

  _xip_data_start = .;
  _sdata = .;
  RW_DATA(XCHAL_ICACHE_LINESIZE, PAGE_SIZE, THREAD_SIZE)
  _edata = .;
  _xip_data_end = .;

  /* Initialization data: */

  STRUCT_ALIGN();

  _xip_init_data_start = .;
  __init_begin = .;
  .init.data :
  {
    INIT_DATA
  }
  _xip_init_data_end = .;
  __init_end = .;
  BSS_SECTION(0, 8192, 0)

  _xip_end = .;

#undef LOAD_OFFSET
#endif

  DWARF_DEBUG

  .xt.prop 0 : { KEEP(*(.xt.prop .xt.prop.* .gnu.linkonce.prop.*)) }
  .xt.insn 0 : { KEEP(*(.xt.insn .xt.insn.* .gnu.linkonce.x*)) }
  .xt.lit  0 : { KEEP(*(.xt.lit  .xt.lit.*  .gnu.linkonce.p*)) }

  /* Sections to be discarded */
  DISCARDS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
