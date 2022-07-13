<<<<<<< HEAD
#ifndef _ASM_X86_CMPXCHG_64_H
#define _ASM_X86_CMPXCHG_64_H

static inline void set_64bit(volatile u64 *ptr, u64 val)
{
	*ptr = val;
}

#define __HAVE_ARCH_CMPXCHG 1

#define cmpxchg64(ptr, o, n)						\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg((ptr), (o), (n));					\
})

#define cmpxchg64_local(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	cmpxchg_local((ptr), (o), (n));					\
})

#define system_has_cmpxchg_double() cpu_has_cx16
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_CMPXCHG_64_H
#define _ASM_X86_CMPXCHG_64_H

#define arch_cmpxchg64(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_cmpxchg((ptr), (o), (n));					\
})

#define arch_cmpxchg64_local(ptr, o, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_cmpxchg_local((ptr), (o), (n));				\
})

#define arch_try_cmpxchg64(ptr, po, n)					\
({									\
	BUILD_BUG_ON(sizeof(*(ptr)) != 8);				\
	arch_try_cmpxchg((ptr), (po), (n));				\
})

union __u128_halves {
	u128 full;
	struct {
		u64 low, high;
	};
};

#define __arch_cmpxchg128(_ptr, _old, _new, _lock)			\
({									\
	union __u128_halves o = { .full = (_old), },			\
			    n = { .full = (_new), };			\
									\
	asm volatile(_lock "cmpxchg16b %[ptr]"				\
		     : [ptr] "+m" (*(_ptr)),				\
		       "+a" (o.low), "+d" (o.high)			\
		     : "b" (n.low), "c" (n.high)			\
		     : "memory");					\
									\
	o.full;								\
})

static __always_inline u128 arch_cmpxchg128(volatile u128 *ptr, u128 old, u128 new)
{
	return __arch_cmpxchg128(ptr, old, new, LOCK_PREFIX);
}
#define arch_cmpxchg128 arch_cmpxchg128

static __always_inline u128 arch_cmpxchg128_local(volatile u128 *ptr, u128 old, u128 new)
{
	return __arch_cmpxchg128(ptr, old, new,);
}
#define arch_cmpxchg128_local arch_cmpxchg128_local

#define __arch_try_cmpxchg128(_ptr, _oldp, _new, _lock)			\
({									\
	union __u128_halves o = { .full = *(_oldp), },			\
			    n = { .full = (_new), };			\
	bool ret;							\
									\
	asm volatile(_lock "cmpxchg16b %[ptr]"				\
		     CC_SET(e)						\
		     : CC_OUT(e) (ret),					\
		       [ptr] "+m" (*ptr),				\
		       "+a" (o.low), "+d" (o.high)			\
		     : "b" (n.low), "c" (n.high)			\
		     : "memory");					\
									\
	if (unlikely(!ret))						\
		*(_oldp) = o.full;					\
									\
	likely(ret);							\
})

static __always_inline bool arch_try_cmpxchg128(volatile u128 *ptr, u128 *oldp, u128 new)
{
	return __arch_try_cmpxchg128(ptr, oldp, new, LOCK_PREFIX);
}
#define arch_try_cmpxchg128 arch_try_cmpxchg128

static __always_inline bool arch_try_cmpxchg128_local(volatile u128 *ptr, u128 *oldp, u128 new)
{
	return __arch_try_cmpxchg128(ptr, oldp, new,);
}
#define arch_try_cmpxchg128_local arch_try_cmpxchg128_local

#define system_has_cmpxchg128()		boot_cpu_has(X86_FEATURE_CX16)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_X86_CMPXCHG_64_H */
