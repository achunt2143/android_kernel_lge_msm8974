<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ALPHA_CMPXCHG_H
#define _ALPHA_CMPXCHG_H

/*
 * Atomic exchange routines.
 */

<<<<<<< HEAD
#define __ASM__MB
#define ____xchg(type, args...)		__xchg ## type ## _local(args)
=======
#define ____xchg(type, args...)		__arch_xchg ## type ## _local(args)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ____cmpxchg(type, args...)	__cmpxchg ## type ## _local(args)
#include <asm/xchg.h>

#define xchg_local(ptr, x)						\
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
<<<<<<< HEAD
	(__typeof__(*(ptr))) __xchg_local((ptr), (unsigned long)_x_,	\
				       sizeof(*(ptr)));			\
})

#define cmpxchg_local(ptr, o, n)					\
=======
	(__typeof__(*(ptr))) __arch_xchg_local((ptr), (unsigned long)_x_,\
					       sizeof(*(ptr)));		\
})

#define arch_cmpxchg_local(ptr, o, n)					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg_local((ptr), (unsigned long)_o_,	\
					  (unsigned long)_n_,		\
					  sizeof(*(ptr)));		\
})

<<<<<<< HEAD
#define cmpxchg64_local(ptr, o, n)					\
=======
#define arch_cmpxchg64_local(ptr, o, n)					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg_local((ptr), (o), (n));					\
})

<<<<<<< HEAD
#ifdef CONFIG_SMP
#undef __ASM__MB
#define __ASM__MB	"\tmb\n"
#endif
#undef ____xchg
#undef ____cmpxchg
#define ____xchg(type, args...)		__xchg ##type(args)
#define ____cmpxchg(type, args...)	__cmpxchg ##type(args)
#include <asm/xchg.h>

#define xchg(ptr, x)							\
({									\
	__typeof__(*(ptr)) _x_ = (x);					\
	(__typeof__(*(ptr))) __xchg((ptr), (unsigned long)_x_,		\
				 sizeof(*(ptr)));			\
})

#define cmpxchg(ptr, o, n)						\
({									\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	(__typeof__(*(ptr))) __cmpxchg((ptr), (unsigned long)_o_,	\
				    (unsigned long)_n_,	sizeof(*(ptr)));\
})

#define cmpxchg64(ptr, o, n)						\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg((ptr), (o), (n));					\
})

#undef __ASM__MB
#undef ____cmpxchg

#define __HAVE_ARCH_CMPXCHG 1

=======
#undef ____xchg
#undef ____cmpxchg
#define ____xchg(type, args...)		__arch_xchg ##type(args)
#define ____cmpxchg(type, args...)	__cmpxchg ##type(args)
#include <asm/xchg.h>

/*
 * The leading and the trailing memory barriers guarantee that these
 * operations are fully ordered.
 */
#define arch_xchg(ptr, x)						\
({									\
	__typeof__(*(ptr)) __ret;					\
	__typeof__(*(ptr)) _x_ = (x);					\
	smp_mb();							\
	__ret = (__typeof__(*(ptr)))					\
		__arch_xchg((ptr), (unsigned long)_x_, sizeof(*(ptr)));	\
	smp_mb();							\
	__ret;								\
})

#define arch_cmpxchg(ptr, o, n)						\
({									\
	__typeof__(*(ptr)) __ret;					\
	__typeof__(*(ptr)) _o_ = (o);					\
	__typeof__(*(ptr)) _n_ = (n);					\
	smp_mb();							\
	__ret = (__typeof__(*(ptr))) __cmpxchg((ptr),			\
		(unsigned long)_o_, (unsigned long)_n_, sizeof(*(ptr)));\
	smp_mb();							\
	__ret;								\
})

#define arch_cmpxchg64(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_cmpxchg((ptr), (o), (n));					\
})

#undef ____cmpxchg

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ALPHA_CMPXCHG_H */
