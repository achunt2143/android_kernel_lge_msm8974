<<<<<<< HEAD
#ifndef _ASM_X86_SETUP_H
#define _ASM_X86_SETUP_H

#ifdef __KERNEL__

#define COMMAND_LINE_SIZE 2048

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SETUP_H
#define _ASM_X86_SETUP_H

#include <uapi/asm/setup.h>

#define COMMAND_LINE_SIZE 2048

#include <linux/linkage.h>
#include <asm/page_types.h>
#include <asm/ibt.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef __i386__

#include <linux/pfn.h>
/*
 * Reserved space for vmalloc and iomap - defined in asm/page.h
 */
#define MAXMEM_PFN	PFN_DOWN(MAXMEM)
#define MAX_NONPAE_PFN	(1 << 20)

#endif /* __i386__ */

#define PARAM_SIZE 4096		/* sizeof(struct boot_params) */

#define OLD_CL_MAGIC		0xA33F
#define OLD_CL_ADDRESS		0x020	/* Relative to real mode data */
#define NEW_CL_POINTER		0x228	/* Relative to real mode data */

#ifndef __ASSEMBLY__
#include <asm/bootparam.h>
#include <asm/x86_init.h>

/* Interrupt control for vSMPowered x86_64 systems */
#ifdef CONFIG_X86_64
void vsmp_init(void);
#else
static inline void vsmp_init(void) { }
#endif

<<<<<<< HEAD
void setup_bios_corruption_check(void);

#ifdef CONFIG_X86_VISWS
extern void visws_early_detect(void);
#else
static inline void visws_early_detect(void) { }
#endif
=======
struct pt_regs;

void setup_bios_corruption_check(void);
void early_platform_quirks(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern unsigned long saved_video_mode;

extern void reserve_standard_io_resources(void);
extern void i386_reserve_resources(void);
<<<<<<< HEAD
extern void setup_default_timer_irq(void);

#ifdef CONFIG_X86_INTEL_MID
extern void x86_mrst_early_setup(void);
#else
static inline void x86_mrst_early_setup(void) { }
=======
extern unsigned long __startup_64(unsigned long physaddr, struct boot_params *bp);
extern void startup_64_setup_gdt_idt(void);
extern void early_setup_idt(void);
extern void __init do_early_exception(struct pt_regs *regs, int trapnr);

#ifdef CONFIG_X86_INTEL_MID
extern void x86_intel_mid_early_setup(void);
#else
static inline void x86_intel_mid_early_setup(void) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_X86_INTEL_CE
extern void x86_ce4100_early_setup(void);
#else
static inline void x86_ce4100_early_setup(void) { }
#endif

#ifndef _SETUP

#include <asm/espfix.h>
<<<<<<< HEAD
=======
#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is set up by the setup-routine at boot-time
 */
extern struct boot_params boot_params;
<<<<<<< HEAD
=======
extern char _text[];

static inline bool kaslr_enabled(void)
{
	return IS_ENABLED(CONFIG_RANDOMIZE_MEMORY) &&
		!!(boot_params.hdr.loadflags & KASLR_FLAG);
}

/*
 * Apply no randomization if KASLR was disabled at boot or if KASAN
 * is enabled. KASAN shadow mappings rely on regions being PGD aligned.
 */
static inline bool kaslr_memory_enabled(void)
{
	return kaslr_enabled() && !IS_ENABLED(CONFIG_KASAN);
}

static inline unsigned long kaslr_offset(void)
{
	return (unsigned long)&_text - __START_KERNEL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Do NOT EVER look at the BIOS memory size location.
 * It does not work on many machines.
 */
#define LOWMEMSIZE()	(0x9f000)

/* exceedingly early brk-like allocator */
extern unsigned long _brk_end;
void *extend_brk(size_t size, size_t align);

/*
<<<<<<< HEAD
 * Reserve space in the brk section.  The name must be unique within
 * the file, and somewhat descriptive.  The size is in bytes.  Must be
 * used at file scope.
 *
 * (This uses a temp function to wrap the asm so we can pass it the
 * size parameter; otherwise we wouldn't be able to.  We can't use a
 * "section" attribute on a normal variable because it always ends up
 * being @progbits, which ends up allocating space in the vmlinux
 * executable.)
 */
#define RESERVE_BRK(name,sz)						\
	static void __section(.discard.text) __used notrace		\
	__brk_reservation_fn_##name##__(void) {				\
		asm volatile (						\
			".pushsection .brk_reservation,\"aw\",@nobits;" \
			".brk." #name ":"				\
			" 1:.skip %c0;"					\
			" .size .brk." #name ", . - 1b;"		\
			" .popsection"					\
			: : "i" (sz));					\
	}

/* Helper for reserving space for arrays of things */
#define RESERVE_BRK_ARRAY(type, name, entries)		\
	type *name;					\
	RESERVE_BRK(name, sizeof(type) * entries)

extern void probe_roms(void);
#ifdef __i386__

void __init i386_start_kernel(void);

#else
void __init x86_64_start_kernel(char *real_mode);
void __init x86_64_start_reservations(char *real_mode_data);

#endif /* __i386__ */
#endif /* _SETUP */
#else
#define RESERVE_BRK(name,sz)				\
	.pushsection .brk_reservation,"aw",@nobits;	\
.brk.name:						\
1:	.skip sz;					\
	.size .brk.name,.-1b;				\
	.popsection
#endif /* __ASSEMBLY__ */
#endif  /*  __KERNEL__  */
=======
 * Reserve space in the .brk section, which is a block of memory from which the
 * caller is allowed to allocate very early (before even memblock is available)
 * by calling extend_brk().  All allocated memory will be eventually converted
 * to memblock.  Any leftover unallocated memory will be freed.
 *
 * The size is in bytes.
 */
#define RESERVE_BRK(name, size)					\
	__section(".bss..brk") __aligned(1) __used	\
	static char __brk_##name[size]

extern void probe_roms(void);

void clear_bss(void);

#ifdef __i386__

asmlinkage void __init __noreturn i386_start_kernel(void);
void __init mk_early_pgtbl_32(void);

#else
asmlinkage void __init __noreturn x86_64_start_kernel(char *real_mode);
asmlinkage void __init __noreturn x86_64_start_reservations(char *real_mode_data);

#endif /* __i386__ */
#endif /* _SETUP */

#else  /* __ASSEMBLY */

.macro __RESERVE_BRK name, size
	.pushsection .bss..brk, "aw"
SYM_DATA_START(__brk_\name)
	.skip \size
SYM_DATA_END(__brk_\name)
	.popsection
.endm

#define RESERVE_BRK(name, size) __RESERVE_BRK name, size

#endif /* __ASSEMBLY__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_SETUP_H */
