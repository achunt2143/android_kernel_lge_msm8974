<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* 32-bit atomic xchg() and cmpxchg() definitions.
 *
 * Copyright (C) 1996 David S. Miller (davem@davemloft.net)
 * Copyright (C) 2000 Anton Blanchard (anton@linuxcare.com.au)
 * Copyright (C) 2007 Kyle McMartin (kyle@parisc-linux.org)
 *
 * Additions by Keith M Wesolowski (wesolows@foobazco.org) based
 * on asm-parisc/atomic.h Copyright (C) 2000 Philipp Rumpf <prumpf@tux.org>.
 */

#ifndef __ARCH_SPARC_CMPXCHG__
#define __ARCH_SPARC_CMPXCHG__

<<<<<<< HEAD
#include <asm/btfixup.h>

/* This has special calling conventions */
#ifndef CONFIG_SMP
BTFIXUPDEF_CALL(void, ___xchg32, void)
#endif

static inline unsigned long xchg_u32(__volatile__ unsigned long *m, unsigned long val)
{
#ifdef CONFIG_SMP
	__asm__ __volatile__("swap [%2], %0"
			     : "=&r" (val)
			     : "0" (val), "r" (m)
			     : "memory");
	return val;
#else
	register unsigned long *ptr asm("g1");
	register unsigned long ret asm("g2");

	ptr = (unsigned long *) m;
	ret = val;

	/* Note: this is magic and the nop there is
	   really needed. */
	__asm__ __volatile__(
	"mov	%%o7, %%g4\n\t"
	"call	___f____xchg32\n\t"
	" nop\n\t"
	: "=&r" (ret)
	: "0" (ret), "r" (ptr)
	: "g3", "g4", "g7", "memory", "cc");

	return ret;
#endif
}

extern void __xchg_called_with_bad_pointer(void);

static inline unsigned long __xchg(unsigned long x, __volatile__ void * ptr, int size)
{
	switch (size) {
	case 4:
		return xchg_u32(ptr, x);
=======
unsigned long __xchg_u32(volatile u32 *m, u32 new);
void __xchg_called_with_bad_pointer(void);

static __always_inline unsigned long __arch_xchg(unsigned long x, __volatile__ void * ptr, int size)
{
	switch (size) {
	case 4:
		return __xchg_u32(ptr, x);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	__xchg_called_with_bad_pointer();
	return x;
}

<<<<<<< HEAD
#define xchg(ptr,x) ((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))
=======
#define arch_xchg(ptr,x) ({(__typeof__(*(ptr)))__arch_xchg((unsigned long)(x),(ptr),sizeof(*(ptr)));})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Emulate cmpxchg() the same way we emulate atomics,
 * by hashing the object address and indexing into an array
 * of spinlocks to get a bit of performance...
 *
 * See arch/sparc/lib/atomic32.c for implementation.
 *
 * Cribbed from <asm-parisc/atomic.h>
 */
<<<<<<< HEAD
#define __HAVE_ARCH_CMPXCHG	1

/* bug catcher for when unsupported size is used - won't link */
extern void __cmpxchg_called_with_bad_pointer(void);
/* we only need to support cmpxchg of a u32 on sparc */
extern unsigned long __cmpxchg_u32(volatile u32 *m, u32 old, u32 new_);
=======

/* bug catcher for when unsupported size is used - won't link */
void __cmpxchg_called_with_bad_pointer(void);
/* we only need to support cmpxchg of a u32 on sparc */
unsigned long __cmpxchg_u32(volatile u32 *m, u32 old, u32 new_);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* don't worry...optimizer will get rid of most of this */
static inline unsigned long
__cmpxchg(volatile void *ptr, unsigned long old, unsigned long new_, int size)
{
	switch (size) {
	case 4:
		return __cmpxchg_u32((u32 *)ptr, (u32)old, (u32)new_);
	default:
		__cmpxchg_called_with_bad_pointer();
		break;
	}
	return old;
}

<<<<<<< HEAD
#define cmpxchg(ptr, o, n)						\
=======
#define arch_cmpxchg(ptr, o, n)						\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg((ptr), (unsigned long)_o_,	\
			(unsigned long)_n_, sizeof(*(ptr)));		\
})

<<<<<<< HEAD
=======
u64 __cmpxchg_u64(u64 *ptr, u64 old, u64 new);
#define arch_cmpxchg64(ptr, old, new)	__cmpxchg_u64(ptr, old, new)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/cmpxchg-local.h>

/*
 * cmpxchg_local and cmpxchg64_local are atomic wrt current CPU. Always make
 * them available.
 */
<<<<<<< HEAD
#define cmpxchg_local(ptr, o, n)				  	       \
	((__typeof__(*(ptr)))__cmpxchg_local_generic((ptr), (unsigned long)(o),\
			(unsigned long)(n), sizeof(*(ptr))))
#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))
=======
#define arch_cmpxchg_local(ptr, o, n)				  	       \
	((__typeof__(*(ptr)))__generic_cmpxchg_local((ptr), (unsigned long)(o),\
			(unsigned long)(n), sizeof(*(ptr))))
#define arch_cmpxchg64_local(ptr, o, n) __generic_cmpxchg64_local((ptr), (o), (n))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ARCH_SPARC_CMPXCHG__ */
