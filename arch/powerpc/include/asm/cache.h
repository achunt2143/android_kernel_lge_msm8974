<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_CACHE_H
#define _ASM_POWERPC_CACHE_H

#ifdef __KERNEL__


/* bytes per L1 cache line */
<<<<<<< HEAD
#if defined(CONFIG_8xx) || defined(CONFIG_403GCX)
#define L1_CACHE_SHIFT		4
#define MAX_COPY_PREFETCH	1
#elif defined(CONFIG_PPC_E500MC)
#define L1_CACHE_SHIFT		6
#define MAX_COPY_PREFETCH	4
#elif defined(CONFIG_PPC32)
#define MAX_COPY_PREFETCH	4
=======
#if defined(CONFIG_PPC_8xx)
#define L1_CACHE_SHIFT		4
#define MAX_COPY_PREFETCH	1
#define IFETCH_ALIGN_SHIFT	2
#elif defined(CONFIG_PPC_E500MC)
#define L1_CACHE_SHIFT		6
#define MAX_COPY_PREFETCH	4
#define IFETCH_ALIGN_SHIFT	3
#elif defined(CONFIG_PPC32)
#define MAX_COPY_PREFETCH	4
#define IFETCH_ALIGN_SHIFT	3	/* 603 fetches 2 insn at a time */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if defined(CONFIG_PPC_47x)
#define L1_CACHE_SHIFT		7
#else
#define L1_CACHE_SHIFT		5
#endif
#else /* CONFIG_PPC64 */
#define L1_CACHE_SHIFT		7
<<<<<<< HEAD
=======
#define IFETCH_ALIGN_SHIFT	4 /* POWER8,9 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#define	L1_CACHE_BYTES		(1 << L1_CACHE_SHIFT)

#define	SMP_CACHE_BYTES		L1_CACHE_BYTES

<<<<<<< HEAD
#if defined(__powerpc64__) && !defined(__ASSEMBLY__)
struct ppc64_caches {
	u32	dsize;			/* L1 d-cache size */
	u32	dline_size;		/* L1 d-cache line size	*/
	u32	log_dline_size;
	u32	dlines_per_page;
	u32	isize;			/* L1 i-cache size */
	u32	iline_size;		/* L1 i-cache line size	*/
	u32	log_iline_size;
	u32	ilines_per_page;
};

extern struct ppc64_caches ppc64_caches;
#endif /* __powerpc64__ && ! __ASSEMBLY__ */

#if !defined(__ASSEMBLY__)

#define __read_mostly __attribute__((__section__(".data..read_mostly")))

#ifdef CONFIG_6xx
=======
#define IFETCH_ALIGN_BYTES	(1 << IFETCH_ALIGN_SHIFT)

#ifdef CONFIG_NOT_COHERENT_CACHE
#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES
#endif

#if !defined(__ASSEMBLY__)
#ifdef CONFIG_PPC64

struct ppc_cache_info {
	u32 size;
	u32 line_size;
	u32 block_size;	/* L1 only */
	u32 log_block_size;
	u32 blocks_per_page;
	u32 sets;
	u32 assoc;
};

struct ppc64_caches {
	struct ppc_cache_info l1d;
	struct ppc_cache_info l1i;
	struct ppc_cache_info l2;
	struct ppc_cache_info l3;
};

extern struct ppc64_caches ppc64_caches;

static inline u32 l1_dcache_shift(void)
{
	return ppc64_caches.l1d.log_block_size;
}

static inline u32 l1_dcache_bytes(void)
{
	return ppc64_caches.l1d.block_size;
}

static inline u32 l1_icache_shift(void)
{
	return ppc64_caches.l1i.log_block_size;
}

static inline u32 l1_icache_bytes(void)
{
	return ppc64_caches.l1i.block_size;
}
#else
static inline u32 l1_dcache_shift(void)
{
	return L1_CACHE_SHIFT;
}

static inline u32 l1_dcache_bytes(void)
{
	return L1_CACHE_BYTES;
}

static inline u32 l1_icache_shift(void)
{
	return L1_CACHE_SHIFT;
}

static inline u32 l1_icache_bytes(void)
{
	return L1_CACHE_BYTES;
}

#endif

#define __read_mostly __section(".data..read_mostly")

#ifdef CONFIG_PPC_BOOK3S_32
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern long _get_L2CR(void);
extern long _get_L3CR(void);
extern void _set_L2CR(unsigned long);
extern void _set_L3CR(unsigned long);
#else
#define _get_L2CR()	0L
#define _get_L3CR()	0L
#define _set_L2CR(val)	do { } while(0)
#define _set_L3CR(val)	do { } while(0)
#endif

<<<<<<< HEAD
extern void cacheable_memzero(void *p, unsigned int nb);
extern void *cacheable_memcpy(void *, const void *, unsigned int);
=======
static inline void dcbz(void *addr)
{
	__asm__ __volatile__ ("dcbz 0, %0" : : "r"(addr) : "memory");
}

static inline void dcbi(void *addr)
{
	__asm__ __volatile__ ("dcbi 0, %0" : : "r"(addr) : "memory");
}

static inline void dcbf(void *addr)
{
	__asm__ __volatile__ ("dcbf 0, %0" : : "r"(addr) : "memory");
}

static inline void dcbst(void *addr)
{
	__asm__ __volatile__ ("dcbst 0, %0" : : "r"(addr) : "memory");
}

static inline void icbi(void *addr)
{
	asm volatile ("icbi 0, %0" : : "r"(addr) : "memory");
}

static inline void iccci(void *addr)
{
	asm volatile ("iccci 0, %0" : : "r"(addr) : "memory");
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* !__ASSEMBLY__ */
#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_CACHE_H */
