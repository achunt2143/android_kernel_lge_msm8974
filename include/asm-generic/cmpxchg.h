<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic UP xchg and cmpxchg using interrupt disablement.  Does not
 * support SMP.
 */

#ifndef __ASM_GENERIC_CMPXCHG_H
#define __ASM_GENERIC_CMPXCHG_H

#ifdef CONFIG_SMP
#error "Cannot use generic cmpxchg on SMP"
#endif

#include <linux/types.h>
#include <linux/irqflags.h>

<<<<<<< HEAD
#ifndef xchg

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This function doesn't exist, so you'll get a linker error if
 * something tries to do an invalidly-sized xchg().
 */
<<<<<<< HEAD
extern void __xchg_called_with_bad_pointer(void);

static inline
unsigned long __xchg(unsigned long x, volatile void *ptr, int size)
=======
extern void __generic_xchg_called_with_bad_pointer(void);

static inline
unsigned long __generic_xchg(unsigned long x, volatile void *ptr, int size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long ret, flags;

	switch (size) {
	case 1:
#ifdef __xchg_u8
		return __xchg_u8(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u8 *)ptr;
<<<<<<< HEAD
		*(volatile u8 *)ptr = x;
=======
		*(volatile u8 *)ptr = (x & 0xffu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u8 */

	case 2:
#ifdef __xchg_u16
		return __xchg_u16(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u16 *)ptr;
<<<<<<< HEAD
		*(volatile u16 *)ptr = x;
=======
		*(volatile u16 *)ptr = (x & 0xffffu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u16 */

	case 4:
#ifdef __xchg_u32
		return __xchg_u32(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u32 *)ptr;
<<<<<<< HEAD
		*(volatile u32 *)ptr = x;
=======
		*(volatile u32 *)ptr = (x & 0xffffffffu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u32 */

#ifdef CONFIG_64BIT
	case 8:
#ifdef __xchg_u64
		return __xchg_u64(x, ptr);
#else
		local_irq_save(flags);
		ret = *(volatile u64 *)ptr;
		*(volatile u64 *)ptr = x;
		local_irq_restore(flags);
		return ret;
#endif /* __xchg_u64 */
#endif /* CONFIG_64BIT */

	default:
<<<<<<< HEAD
		__xchg_called_with_bad_pointer();
=======
		__generic_xchg_called_with_bad_pointer();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return x;
	}
}

<<<<<<< HEAD
#define xchg(ptr, x) \
	((__typeof__(*(ptr))) __xchg((unsigned long)(x), (ptr), sizeof(*(ptr))))

#endif /* xchg */

/*
 * Atomic compare and exchange.
 *
 * Do not define __HAVE_ARCH_CMPXCHG because we want to use it to check whether
 * a cmpxchg primitive faster than repeated local irq save/restore exists.
 */
#include <asm-generic/cmpxchg-local.h>

#define cmpxchg(ptr, o, n)	cmpxchg_local((ptr), (o), (n))
#define cmpxchg64(ptr, o, n)	cmpxchg64_local((ptr), (o), (n))
=======
#define generic_xchg(ptr, x) ({							\
	((__typeof__(*(ptr)))							\
		__generic_xchg((unsigned long)(x), (ptr), sizeof(*(ptr))));	\
})

/*
 * Atomic compare and exchange.
 */
#include <asm-generic/cmpxchg-local.h>

#define generic_cmpxchg_local(ptr, o, n) ({					\
	((__typeof__(*(ptr)))__generic_cmpxchg_local((ptr), (unsigned long)(o),	\
			(unsigned long)(n), sizeof(*(ptr))));			\
})

#define generic_cmpxchg64_local(ptr, o, n) \
	__generic_cmpxchg64_local((ptr), (o), (n))


#ifndef arch_xchg
#define arch_xchg		generic_xchg
#endif

#ifndef arch_cmpxchg_local
#define arch_cmpxchg_local	generic_cmpxchg_local
#endif

#ifndef arch_cmpxchg64_local
#define arch_cmpxchg64_local	generic_cmpxchg64_local
#endif

#define arch_cmpxchg		arch_cmpxchg_local
#define arch_cmpxchg64		arch_cmpxchg64_local
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_GENERIC_CMPXCHG_H */
