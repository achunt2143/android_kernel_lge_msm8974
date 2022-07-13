<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * include/asm-parisc/cache.h
 */

#ifndef __ARCH_PARISC_CACHE_H
#define __ARCH_PARISC_CACHE_H

<<<<<<< HEAD

/*
 * PA 2.0 processors have 64-byte cachelines; PA 1.1 processors have
 * 32-byte cachelines.  The default configuration is not for SMP anyway,
 * so if you're building for SMP, you should select the appropriate
 * processor type.  There is a potential livelock danger when running
 * a machine with this value set too small, but it's more probable you'll
 * just ruin performance.
 */
#ifdef CONFIG_PA20
#define L1_CACHE_BYTES 64
#define L1_CACHE_SHIFT 6
#else
#define L1_CACHE_BYTES 32
#define L1_CACHE_SHIFT 5
#endif
=======
#include <asm/alternative.h>

/*
 * PA 2.0 processors have 64 and 128-byte L2 cachelines; PA 1.1 processors
 * have 32-byte cachelines.  The L1 length appears to be 16 bytes but this
 * is not clearly documented.
 */
#define L1_CACHE_BYTES 16
#define L1_CACHE_SHIFT 4
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef __ASSEMBLY__

#define SMP_CACHE_BYTES L1_CACHE_BYTES

#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

<<<<<<< HEAD
#define __read_mostly __attribute__((__section__(".data..read_mostly")))
=======
#define __read_mostly __section(".data..read_mostly")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void parisc_cache_init(void);	/* initializes cache-flushing */
void disable_sr_hashing_asm(int); /* low level support for above */
void disable_sr_hashing(void);   /* turns off space register hashing */
void free_sid(unsigned long);
unsigned long alloc_sid(void);

struct seq_file;
extern void show_cache_info(struct seq_file *m);

extern int split_tlb;
extern int dcache_stride;
extern int icache_stride;
extern struct pdc_cache_info cache_info;
<<<<<<< HEAD
void parisc_setup_cache_timing(void);

#define pdtlb(addr)         asm volatile("pdtlb 0(%%sr1,%0)" : : "r" (addr));
#define pitlb(addr)         asm volatile("pitlb 0(%%sr1,%0)" : : "r" (addr));
#define pdtlb_kernel(addr)  asm volatile("pdtlb 0(%0)" : : "r" (addr));
=======
extern struct pdc_btlb_info btlb_info;
void parisc_setup_cache_timing(void);

#define pdtlb(sr, addr)	asm volatile("pdtlb 0(%%sr%0,%1)" \
			ALTERNATIVE(ALT_COND_NO_SMP, INSN_PxTLB) \
			: : "i"(sr), "r" (addr) : "memory")
#define pitlb(sr, addr)	asm volatile("pitlb 0(%%sr%0,%1)" \
			ALTERNATIVE(ALT_COND_NO_SMP, INSN_PxTLB) \
			ALTERNATIVE(ALT_COND_NO_SPLIT_TLB, INSN_NOP) \
			: : "i"(sr), "r" (addr) : "memory")

#define asm_io_fdc(addr) asm volatile("fdc %%r0(%0)" \
			ALTERNATIVE(ALT_COND_NO_DCACHE, INSN_NOP) \
			ALTERNATIVE(ALT_COND_NO_IOC_FDC, INSN_NOP) \
			: : "r" (addr) : "memory")
#define asm_io_sync()	asm volatile("sync" \
			ALTERNATIVE(ALT_COND_NO_DCACHE, INSN_NOP) \
			ALTERNATIVE(ALT_COND_NO_IOC_FDC, INSN_NOP) :::"memory")
#define asm_syncdma()	asm volatile("syncdma" :::"memory")
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* ! __ASSEMBLY__ */

/* Classes of processor wrt: disabling space register hashing */

#define SRHASH_PCXST    0   /* pcxs, pcxt, pcxt_ */
#define SRHASH_PCXL     1   /* pcxl */
#define SRHASH_PA20     2   /* pcxu, pcxu_, pcxw, pcxw_ */

#endif
