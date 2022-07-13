<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_PERCPU_H
#define _ASM_X86_PERCPU_H

#ifdef CONFIG_X86_64
#define __percpu_seg		gs
<<<<<<< HEAD
#define __percpu_mov_op		movq
#else
#define __percpu_seg		fs
#define __percpu_mov_op		movl
=======
#define __percpu_rel		(%rip)
#else
#define __percpu_seg		fs
#define __percpu_rel
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef __ASSEMBLY__

<<<<<<< HEAD
/*
 * PER_CPU finds an address of a per-cpu variable.
 *
 * Args:
 *    var - variable name
 *    reg - 32bit register
 *
 * The resulting address is stored in the "reg" argument.
 *
 * Example:
 *    PER_CPU(cpu_gdt_descr, %ebx)
 */
#ifdef CONFIG_SMP
#define PER_CPU(var, reg)						\
	__percpu_mov_op %__percpu_seg:this_cpu_off, reg;		\
	lea var(reg), reg
#define PER_CPU_VAR(var)	%__percpu_seg:var
#else /* ! SMP */
#define PER_CPU(var, reg)	__percpu_mov_op $var, reg
#define PER_CPU_VAR(var)	var
#endif	/* SMP */
=======
#ifdef CONFIG_SMP
#define __percpu		%__percpu_seg:
#else
#define __percpu
#endif

#define PER_CPU_VAR(var)	__percpu(var)__percpu_rel
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_X86_64_SMP
#define INIT_PER_CPU_VAR(var)  init_per_cpu__##var
#else
#define INIT_PER_CPU_VAR(var)  var
#endif

#else /* ...!ASSEMBLY */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/stringify.h>

#ifdef CONFIG_SMP
#define __percpu_prefix		"%%"__stringify(__percpu_seg)":"
#define __my_cpu_offset		percpu_read(this_cpu_off)

=======
#include <linux/build_bug.h>
#include <linux/stringify.h>
#include <asm/asm.h>

#ifdef CONFIG_SMP

#ifdef CONFIG_CC_HAS_NAMED_AS

#ifdef __CHECKER__
#define __seg_gs		__attribute__((address_space(__seg_gs)))
#define __seg_fs		__attribute__((address_space(__seg_fs)))
#endif

#ifdef CONFIG_X86_64
#define __percpu_seg_override	__seg_gs
#else
#define __percpu_seg_override	__seg_fs
#endif

#define __percpu_prefix		""

#else /* CONFIG_CC_HAS_NAMED_AS */

#define __percpu_seg_override
#define __percpu_prefix		"%%"__stringify(__percpu_seg)":"

#endif /* CONFIG_CC_HAS_NAMED_AS */

#define __force_percpu_prefix	"%%"__stringify(__percpu_seg)":"
#define __my_cpu_offset		this_cpu_read(this_cpu_off)

#ifdef CONFIG_USE_X86_SEG_SUPPORT
/*
 * Efficient implementation for cases in which the compiler supports
 * named address spaces.  Allows the compiler to perform additional
 * optimizations that can save more instructions.
 */
#define arch_raw_cpu_ptr(ptr)					\
({								\
	unsigned long tcp_ptr__;				\
	tcp_ptr__ = __raw_cpu_read(, this_cpu_off);		\
								\
	tcp_ptr__ += (unsigned long)(ptr);			\
	(typeof(*(ptr)) __kernel __force *)tcp_ptr__;		\
})
#else /* CONFIG_USE_X86_SEG_SUPPORT */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Compared to the generic __my_cpu_offset version, the following
 * saves one instruction and avoids clobbering a temp register.
 */
<<<<<<< HEAD
#define __this_cpu_ptr(ptr)				\
({							\
	unsigned long tcp_ptr__;			\
	__verify_pcpu_ptr(ptr);				\
	asm volatile("add " __percpu_arg(1) ", %0"	\
		     : "=r" (tcp_ptr__)			\
		     : "m" (this_cpu_off), "0" (ptr));	\
	(typeof(*(ptr)) __kernel __force *)tcp_ptr__;	\
})
#else
#define __percpu_prefix		""
#endif

#define __percpu_arg(x)		__percpu_prefix "%P" #x
=======
#define arch_raw_cpu_ptr(ptr)					\
({								\
	unsigned long tcp_ptr__;				\
	asm ("mov " __percpu_arg(1) ", %0"			\
	     : "=r" (tcp_ptr__)					\
	     : "m" (__my_cpu_var(this_cpu_off)));		\
								\
	tcp_ptr__ += (unsigned long)(ptr);			\
	(typeof(*(ptr)) __kernel __force *)tcp_ptr__;		\
})
#endif /* CONFIG_USE_X86_SEG_SUPPORT */

#define PER_CPU_VAR(var)	%__percpu_seg:(var)__percpu_rel

#else /* CONFIG_SMP */
#define __percpu_seg_override
#define __percpu_prefix		""
#define __force_percpu_prefix	""

#define PER_CPU_VAR(var)	(var)__percpu_rel

#endif /* CONFIG_SMP */

#define __my_cpu_type(var)	typeof(var) __percpu_seg_override
#define __my_cpu_ptr(ptr)	(__my_cpu_type(*ptr) *)(uintptr_t)(ptr)
#define __my_cpu_var(var)	(*__my_cpu_ptr(&var))
#define __percpu_arg(x)		__percpu_prefix "%" #x
#define __force_percpu_arg(x)	__force_percpu_prefix "%" #x
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Initialized pointers to per-cpu variables needed for the boot
 * processor need to use these macros to get the proper address
 * offset from __per_cpu_load on SMP.
 *
 * There also must be an entry in vmlinux_64.lds.S
 */
#define DECLARE_INIT_PER_CPU(var) \
       extern typeof(var) init_per_cpu_var(var)

#ifdef CONFIG_X86_64_SMP
#define init_per_cpu_var(var)  init_per_cpu__##var
#else
#define init_per_cpu_var(var)  var
#endif

/* For arch-specific code, we can use direct single-insn ops (they
 * don't give an lvalue though). */
<<<<<<< HEAD
extern void __bad_percpu_size(void);

#define percpu_to_op(op, var, val)			\
do {							\
	typedef typeof(var) pto_T__;			\
	if (0) {					\
		pto_T__ pto_tmp__;			\
		pto_tmp__ = (val);			\
		(void)pto_tmp__;			\
	}						\
	switch (sizeof(var)) {				\
	case 1:						\
		asm(op "b %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "qi" ((pto_T__)(val)));		\
		break;					\
	case 2:						\
		asm(op "w %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "ri" ((pto_T__)(val)));		\
		break;					\
	case 4:						\
		asm(op "l %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "ri" ((pto_T__)(val)));		\
		break;					\
	case 8:						\
		asm(op "q %1,"__percpu_arg(0)		\
		    : "+m" (var)			\
		    : "re" ((pto_T__)(val)));		\
		break;					\
	default: __bad_percpu_size();			\
	}						\
} while (0)

=======

#define __pcpu_type_1 u8
#define __pcpu_type_2 u16
#define __pcpu_type_4 u32
#define __pcpu_type_8 u64

#define __pcpu_cast_1(val) ((u8)(((unsigned long) val) & 0xff))
#define __pcpu_cast_2(val) ((u16)(((unsigned long) val) & 0xffff))
#define __pcpu_cast_4(val) ((u32)(((unsigned long) val) & 0xffffffff))
#define __pcpu_cast_8(val) ((u64)(val))

#define __pcpu_op1_1(op, dst) op "b " dst
#define __pcpu_op1_2(op, dst) op "w " dst
#define __pcpu_op1_4(op, dst) op "l " dst
#define __pcpu_op1_8(op, dst) op "q " dst

#define __pcpu_op2_1(op, src, dst) op "b " src ", " dst
#define __pcpu_op2_2(op, src, dst) op "w " src ", " dst
#define __pcpu_op2_4(op, src, dst) op "l " src ", " dst
#define __pcpu_op2_8(op, src, dst) op "q " src ", " dst

#define __pcpu_reg_1(mod, x) mod "q" (x)
#define __pcpu_reg_2(mod, x) mod "r" (x)
#define __pcpu_reg_4(mod, x) mod "r" (x)
#define __pcpu_reg_8(mod, x) mod "r" (x)

#define __pcpu_reg_imm_1(x) "qi" (x)
#define __pcpu_reg_imm_2(x) "ri" (x)
#define __pcpu_reg_imm_4(x) "ri" (x)
#define __pcpu_reg_imm_8(x) "re" (x)

#define percpu_to_op(size, qual, op, _var, _val)			\
do {									\
	__pcpu_type_##size pto_val__ = __pcpu_cast_##size(_val);	\
	if (0) {		                                        \
		typeof(_var) pto_tmp__;					\
		pto_tmp__ = (_val);					\
		(void)pto_tmp__;					\
	}								\
	asm qual(__pcpu_op2_##size(op, "%[val]", __percpu_arg([var]))	\
	    : [var] "+m" (__my_cpu_var(_var))				\
	    : [val] __pcpu_reg_imm_##size(pto_val__));			\
} while (0)

#define percpu_unary_op(size, qual, op, _var)				\
({									\
	asm qual (__pcpu_op1_##size(op, __percpu_arg([var]))		\
	    : [var] "+m" (__my_cpu_var(_var)));				\
})

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generate a percpu add to memory instruction and optimize code
 * if one is added or subtracted.
 */
<<<<<<< HEAD
#define percpu_add_op(var, val)						\
do {									\
	typedef typeof(var) pao_T__;					\
	const int pao_ID__ = (__builtin_constant_p(val) &&		\
			      ((val) == 1 || (val) == -1)) ? (val) : 0;	\
	if (0) {							\
		pao_T__ pao_tmp__;					\
		pao_tmp__ = (val);					\
		(void)pao_tmp__;					\
	}								\
	switch (sizeof(var)) {						\
	case 1:								\
		if (pao_ID__ == 1)					\
			asm("incb "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decb "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addb %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "qi" ((pao_T__)(val)));			\
		break;							\
	case 2:								\
		if (pao_ID__ == 1)					\
			asm("incw "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decw "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addw %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "ri" ((pao_T__)(val)));			\
		break;							\
	case 4:								\
		if (pao_ID__ == 1)					\
			asm("incl "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decl "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addl %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "ri" ((pao_T__)(val)));			\
		break;							\
	case 8:								\
		if (pao_ID__ == 1)					\
			asm("incq "__percpu_arg(0) : "+m" (var));	\
		else if (pao_ID__ == -1)				\
			asm("decq "__percpu_arg(0) : "+m" (var));	\
		else							\
			asm("addq %1, "__percpu_arg(0)			\
			    : "+m" (var)				\
			    : "re" ((pao_T__)(val)));			\
		break;							\
	default: __bad_percpu_size();					\
	}								\
} while (0)

#define percpu_from_op(op, var, constraint)		\
({							\
	typeof(var) pfo_ret__;				\
	switch (sizeof(var)) {				\
	case 1:						\
		asm(op "b "__percpu_arg(1)",%0"		\
		    : "=q" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	case 2:						\
		asm(op "w "__percpu_arg(1)",%0"		\
		    : "=r" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	case 4:						\
		asm(op "l "__percpu_arg(1)",%0"		\
		    : "=r" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	case 8:						\
		asm(op "q "__percpu_arg(1)",%0"		\
		    : "=r" (pfo_ret__)			\
		    : constraint);			\
		break;					\
	default: __bad_percpu_size();			\
	}						\
	pfo_ret__;					\
})

#define percpu_unary_op(op, var)			\
({							\
	switch (sizeof(var)) {				\
	case 1:						\
		asm(op "b "__percpu_arg(0)		\
		    : "+m" (var));			\
		break;					\
	case 2:						\
		asm(op "w "__percpu_arg(0)		\
		    : "+m" (var));			\
		break;					\
	case 4:						\
		asm(op "l "__percpu_arg(0)		\
		    : "+m" (var));			\
		break;					\
	case 8:						\
		asm(op "q "__percpu_arg(0)		\
		    : "+m" (var));			\
		break;					\
	default: __bad_percpu_size();			\
	}						\
=======
#define percpu_add_op(size, qual, var, val)				\
do {									\
	const int pao_ID__ = (__builtin_constant_p(val) &&		\
			      ((val) == 1 || (val) == -1)) ?		\
				(int)(val) : 0;				\
	if (0) {							\
		typeof(var) pao_tmp__;					\
		pao_tmp__ = (val);					\
		(void)pao_tmp__;					\
	}								\
	if (pao_ID__ == 1)						\
		percpu_unary_op(size, qual, "inc", var);		\
	else if (pao_ID__ == -1)					\
		percpu_unary_op(size, qual, "dec", var);		\
	else								\
		percpu_to_op(size, qual, "add", var, val);		\
} while (0)

#define percpu_from_op(size, qual, op, _var)				\
({									\
	__pcpu_type_##size pfo_val__;					\
	asm qual (__pcpu_op2_##size(op, __percpu_arg([var]), "%[val]")	\
	    : [val] __pcpu_reg_##size("=", pfo_val__)			\
	    : [var] "m" (__my_cpu_var(_var)));				\
	(typeof(_var))(unsigned long) pfo_val__;			\
})

#define percpu_stable_op(size, op, _var)				\
({									\
	__pcpu_type_##size pfo_val__;					\
	asm(__pcpu_op2_##size(op, __force_percpu_arg(a[var]), "%[val]")	\
	    : [val] __pcpu_reg_##size("=", pfo_val__)			\
	    : [var] "i" (&(_var)));					\
	(typeof(_var))(unsigned long) pfo_val__;			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
})

/*
 * Add return operation
 */
<<<<<<< HEAD
#define percpu_add_return_op(var, val)					\
({									\
	typeof(var) paro_ret__ = val;					\
	switch (sizeof(var)) {						\
	case 1:								\
		asm("xaddb %0, "__percpu_arg(1)				\
			    : "+q" (paro_ret__), "+m" (var)		\
			    : : "memory");				\
		break;							\
	case 2:								\
		asm("xaddw %0, "__percpu_arg(1)				\
			    : "+r" (paro_ret__), "+m" (var)		\
			    : : "memory");				\
		break;							\
	case 4:								\
		asm("xaddl %0, "__percpu_arg(1)				\
			    : "+r" (paro_ret__), "+m" (var)		\
			    : : "memory");				\
		break;							\
	case 8:								\
		asm("xaddq %0, "__percpu_arg(1)				\
			    : "+re" (paro_ret__), "+m" (var)		\
			    : : "memory");				\
		break;							\
	default: __bad_percpu_size();					\
	}								\
	paro_ret__ += val;						\
	paro_ret__;							\
=======
#define percpu_add_return_op(size, qual, _var, _val)			\
({									\
	__pcpu_type_##size paro_tmp__ = __pcpu_cast_##size(_val);	\
	asm qual (__pcpu_op2_##size("xadd", "%[tmp]",			\
				     __percpu_arg([var]))		\
		  : [tmp] __pcpu_reg_##size("+", paro_tmp__),		\
		    [var] "+m" (__my_cpu_var(_var))			\
		  : : "memory");					\
	(typeof(_var))(unsigned long) (paro_tmp__ + _val);		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
})

/*
 * xchg is implemented using cmpxchg without a lock prefix. xchg is
 * expensive due to the implied lock prefix.  The processor cannot prefetch
 * cachelines if xchg is used.
 */
<<<<<<< HEAD
#define percpu_xchg_op(var, nval)					\
({									\
	typeof(var) pxo_ret__;						\
	typeof(var) pxo_new__ = (nval);					\
	switch (sizeof(var)) {						\
	case 1:								\
		asm("\n\tmov "__percpu_arg(1)",%%al"			\
		    "\n1:\tcmpxchgb %2, "__percpu_arg(1)		\
		    "\n\tjnz 1b"					\
			    : "=&a" (pxo_ret__), "+m" (var)		\
			    : "q" (pxo_new__)				\
			    : "memory");				\
		break;							\
	case 2:								\
		asm("\n\tmov "__percpu_arg(1)",%%ax"			\
		    "\n1:\tcmpxchgw %2, "__percpu_arg(1)		\
		    "\n\tjnz 1b"					\
			    : "=&a" (pxo_ret__), "+m" (var)		\
			    : "r" (pxo_new__)				\
			    : "memory");				\
		break;							\
	case 4:								\
		asm("\n\tmov "__percpu_arg(1)",%%eax"			\
		    "\n1:\tcmpxchgl %2, "__percpu_arg(1)		\
		    "\n\tjnz 1b"					\
			    : "=&a" (pxo_ret__), "+m" (var)		\
			    : "r" (pxo_new__)				\
			    : "memory");				\
		break;							\
	case 8:								\
		asm("\n\tmov "__percpu_arg(1)",%%rax"			\
		    "\n1:\tcmpxchgq %2, "__percpu_arg(1)		\
		    "\n\tjnz 1b"					\
			    : "=&a" (pxo_ret__), "+m" (var)		\
			    : "r" (pxo_new__)				\
			    : "memory");				\
		break;							\
	default: __bad_percpu_size();					\
	}								\
	pxo_ret__;							\
=======
#define percpu_xchg_op(size, qual, _var, _nval)				\
({									\
	__pcpu_type_##size pxo_old__;					\
	__pcpu_type_##size pxo_new__ = __pcpu_cast_##size(_nval);	\
	asm qual (__pcpu_op2_##size("mov", __percpu_arg([var]),		\
				    "%[oval]")				\
		  "\n1:\t"						\
		  __pcpu_op2_##size("cmpxchg", "%[nval]",		\
				    __percpu_arg([var]))		\
		  "\n\tjnz 1b"						\
		  : [oval] "=&a" (pxo_old__),				\
		    [var] "+m" (__my_cpu_var(_var))			\
		  : [nval] __pcpu_reg_##size(, pxo_new__)		\
		  : "memory");						\
	(typeof(_var))(unsigned long) pxo_old__;			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
})

/*
 * cmpxchg has no such implied lock semantics as a result it is much
 * more efficient for cpu local operations.
 */
<<<<<<< HEAD
#define percpu_cmpxchg_op(var, oval, nval)				\
({									\
	typeof(var) pco_ret__;						\
	typeof(var) pco_old__ = (oval);					\
	typeof(var) pco_new__ = (nval);					\
	switch (sizeof(var)) {						\
	case 1:								\
		asm("cmpxchgb %2, "__percpu_arg(1)			\
			    : "=a" (pco_ret__), "+m" (var)		\
			    : "q" (pco_new__), "0" (pco_old__)		\
			    : "memory");				\
		break;							\
	case 2:								\
		asm("cmpxchgw %2, "__percpu_arg(1)			\
			    : "=a" (pco_ret__), "+m" (var)		\
			    : "r" (pco_new__), "0" (pco_old__)		\
			    : "memory");				\
		break;							\
	case 4:								\
		asm("cmpxchgl %2, "__percpu_arg(1)			\
			    : "=a" (pco_ret__), "+m" (var)		\
			    : "r" (pco_new__), "0" (pco_old__)		\
			    : "memory");				\
		break;							\
	case 8:								\
		asm("cmpxchgq %2, "__percpu_arg(1)			\
			    : "=a" (pco_ret__), "+m" (var)		\
			    : "r" (pco_new__), "0" (pco_old__)		\
			    : "memory");				\
		break;							\
	default: __bad_percpu_size();					\
	}								\
	pco_ret__;							\
})

/*
 * percpu_read() makes gcc load the percpu variable every time it is
 * accessed while percpu_read_stable() allows the value to be cached.
 * percpu_read_stable() is more efficient and can be used if its value
 * is guaranteed to be valid across cpus.  The current users include
 * get_current() and get_thread_info() both of which are actually
 * per-thread variables implemented as per-cpu variables and thus
 * stable for the duration of the respective task.
 */
#define percpu_read(var)		percpu_from_op("mov", var, "m" (var))
#define percpu_read_stable(var)		percpu_from_op("mov", var, "p" (&(var)))
#define percpu_write(var, val)		percpu_to_op("mov", var, val)
#define percpu_add(var, val)		percpu_add_op(var, val)
#define percpu_sub(var, val)		percpu_add_op(var, -(val))
#define percpu_and(var, val)		percpu_to_op("and", var, val)
#define percpu_or(var, val)		percpu_to_op("or", var, val)
#define percpu_xor(var, val)		percpu_to_op("xor", var, val)
#define percpu_inc(var)		percpu_unary_op("inc", var)

#define __this_cpu_read_1(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define __this_cpu_read_2(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define __this_cpu_read_4(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))

#define __this_cpu_write_1(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_write_2(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_write_4(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_add_1(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_add_2(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_add_4(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_and_1(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_and_2(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_and_4(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_or_1(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_or_2(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_or_4(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_xor_1(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_xor_2(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_xor_4(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_xchg_1(pcp, val)	percpu_xchg_op(pcp, val)
#define __this_cpu_xchg_2(pcp, val)	percpu_xchg_op(pcp, val)
#define __this_cpu_xchg_4(pcp, val)	percpu_xchg_op(pcp, val)

#define this_cpu_read_1(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_read_2(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_read_4(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_write_1(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_write_2(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_write_4(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_add_1(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_add_2(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_add_4(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_and_1(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_and_2(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_and_4(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_or_1(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_or_2(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_or_4(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_xor_1(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_xor_2(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_xor_4(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_xchg_1(pcp, nval)	percpu_xchg_op(pcp, nval)
#define this_cpu_xchg_2(pcp, nval)	percpu_xchg_op(pcp, nval)
#define this_cpu_xchg_4(pcp, nval)	percpu_xchg_op(pcp, nval)

#ifndef CONFIG_M386
#define __this_cpu_add_return_1(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_add_return_2(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_add_return_4(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define __this_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define __this_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#define this_cpu_add_return_1(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_add_return_2(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_add_return_4(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define this_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)
#define this_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#endif /* !CONFIG_M386 */

#ifdef CONFIG_X86_CMPXCHG64
#define percpu_cmpxchg8b_double(pcp1, pcp2, o1, o2, n1, n2)		\
({									\
	bool __ret;							\
	typeof(pcp1) __o1 = (o1), __n1 = (n1);				\
	typeof(pcp2) __o2 = (o2), __n2 = (n2);				\
	asm volatile("cmpxchg8b "__percpu_arg(1)"\n\tsetz %0\n\t"	\
		    : "=a" (__ret), "+m" (pcp1), "+m" (pcp2), "+d" (__o2) \
		    :  "b" (__n1), "c" (__n2), "a" (__o1));		\
	__ret;								\
})

#define __this_cpu_cmpxchg_double_4	percpu_cmpxchg8b_double
#define this_cpu_cmpxchg_double_4	percpu_cmpxchg8b_double
#endif /* CONFIG_X86_CMPXCHG64 */
=======
#define percpu_cmpxchg_op(size, qual, _var, _oval, _nval)		\
({									\
	__pcpu_type_##size pco_old__ = __pcpu_cast_##size(_oval);	\
	__pcpu_type_##size pco_new__ = __pcpu_cast_##size(_nval);	\
	asm qual (__pcpu_op2_##size("cmpxchg", "%[nval]",		\
				    __percpu_arg([var]))		\
		  : [oval] "+a" (pco_old__),				\
		    [var] "+m" (__my_cpu_var(_var))			\
		  : [nval] __pcpu_reg_##size(, pco_new__)		\
		  : "memory");						\
	(typeof(_var))(unsigned long) pco_old__;			\
})

#define percpu_try_cmpxchg_op(size, qual, _var, _ovalp, _nval)		\
({									\
	bool success;							\
	__pcpu_type_##size *pco_oval__ = (__pcpu_type_##size *)(_ovalp); \
	__pcpu_type_##size pco_old__ = *pco_oval__;			\
	__pcpu_type_##size pco_new__ = __pcpu_cast_##size(_nval);	\
	asm qual (__pcpu_op2_##size("cmpxchg", "%[nval]",		\
				    __percpu_arg([var]))		\
		  CC_SET(z)						\
		  : CC_OUT(z) (success),				\
		    [oval] "+a" (pco_old__),				\
		    [var] "+m" (__my_cpu_var(_var))			\
		  : [nval] __pcpu_reg_##size(, pco_new__)		\
		  : "memory");						\
	if (unlikely(!success))						\
		*pco_oval__ = pco_old__;				\
	likely(success);						\
})

#if defined(CONFIG_X86_32) && !defined(CONFIG_UML)
#define percpu_cmpxchg64_op(size, qual, _var, _oval, _nval)		\
({									\
	union {								\
		u64 var;						\
		struct {						\
			u32 low, high;					\
		};							\
	} old__, new__;							\
									\
	old__.var = _oval;						\
	new__.var = _nval;						\
									\
	asm qual (ALTERNATIVE("call this_cpu_cmpxchg8b_emu",		\
			      "cmpxchg8b " __percpu_arg([var]), X86_FEATURE_CX8) \
		  : [var] "+m" (__my_cpu_var(_var)),			\
		    "+a" (old__.low),					\
		    "+d" (old__.high)					\
		  : "b" (new__.low),					\
		    "c" (new__.high),					\
		    "S" (&(_var))					\
		  : "memory");						\
									\
	old__.var;							\
})

#define raw_cpu_cmpxchg64(pcp, oval, nval)	percpu_cmpxchg64_op(8,         , pcp, oval, nval)
#define this_cpu_cmpxchg64(pcp, oval, nval)	percpu_cmpxchg64_op(8, volatile, pcp, oval, nval)

#define percpu_try_cmpxchg64_op(size, qual, _var, _ovalp, _nval)	\
({									\
	bool success;							\
	u64 *_oval = (u64 *)(_ovalp);					\
	union {								\
		u64 var;						\
		struct {						\
			u32 low, high;					\
		};							\
	} old__, new__;							\
									\
	old__.var = *_oval;						\
	new__.var = _nval;						\
									\
	asm qual (ALTERNATIVE("call this_cpu_cmpxchg8b_emu",		\
			      "cmpxchg8b " __percpu_arg([var]), X86_FEATURE_CX8) \
		  CC_SET(z)						\
		  : CC_OUT(z) (success),				\
		    [var] "+m" (__my_cpu_var(_var)),			\
		    "+a" (old__.low),					\
		    "+d" (old__.high)					\
		  : "b" (new__.low),					\
		    "c" (new__.high),					\
		    "S" (&(_var))					\
		  : "memory");						\
	if (unlikely(!success))						\
		*_oval = old__.var;					\
	likely(success);						\
})

#define raw_cpu_try_cmpxchg64(pcp, ovalp, nval)		percpu_try_cmpxchg64_op(8,         , pcp, ovalp, nval)
#define this_cpu_try_cmpxchg64(pcp, ovalp, nval)	percpu_try_cmpxchg64_op(8, volatile, pcp, ovalp, nval)
#endif

#ifdef CONFIG_X86_64
#define raw_cpu_cmpxchg64(pcp, oval, nval)	percpu_cmpxchg_op(8,         , pcp, oval, nval);
#define this_cpu_cmpxchg64(pcp, oval, nval)	percpu_cmpxchg_op(8, volatile, pcp, oval, nval);

#define raw_cpu_try_cmpxchg64(pcp, ovalp, nval)		percpu_try_cmpxchg_op(8,         , pcp, ovalp, nval);
#define this_cpu_try_cmpxchg64(pcp, ovalp, nval)	percpu_try_cmpxchg_op(8, volatile, pcp, ovalp, nval);

#define percpu_cmpxchg128_op(size, qual, _var, _oval, _nval)		\
({									\
	union {								\
		u128 var;						\
		struct {						\
			u64 low, high;					\
		};							\
	} old__, new__;							\
									\
	old__.var = _oval;						\
	new__.var = _nval;						\
									\
	asm qual (ALTERNATIVE("call this_cpu_cmpxchg16b_emu",		\
			      "cmpxchg16b " __percpu_arg([var]), X86_FEATURE_CX16) \
		  : [var] "+m" (__my_cpu_var(_var)),			\
		    "+a" (old__.low),					\
		    "+d" (old__.high)					\
		  : "b" (new__.low),					\
		    "c" (new__.high),					\
		    "S" (&(_var))					\
		  : "memory");						\
									\
	old__.var;							\
})

#define raw_cpu_cmpxchg128(pcp, oval, nval)	percpu_cmpxchg128_op(16,         , pcp, oval, nval)
#define this_cpu_cmpxchg128(pcp, oval, nval)	percpu_cmpxchg128_op(16, volatile, pcp, oval, nval)

#define percpu_try_cmpxchg128_op(size, qual, _var, _ovalp, _nval)	\
({									\
	bool success;							\
	u128 *_oval = (u128 *)(_ovalp);					\
	union {								\
		u128 var;						\
		struct {						\
			u64 low, high;					\
		};							\
	} old__, new__;							\
									\
	old__.var = *_oval;						\
	new__.var = _nval;						\
									\
	asm qual (ALTERNATIVE("call this_cpu_cmpxchg16b_emu",		\
			      "cmpxchg16b " __percpu_arg([var]), X86_FEATURE_CX16) \
		  CC_SET(z)						\
		  : CC_OUT(z) (success),				\
		    [var] "+m" (__my_cpu_var(_var)),			\
		    "+a" (old__.low),					\
		    "+d" (old__.high)					\
		  : "b" (new__.low),					\
		    "c" (new__.high),					\
		    "S" (&(_var))					\
		  : "memory");						\
	if (unlikely(!success))						\
		*_oval = old__.var;					\
	likely(success);						\
})

#define raw_cpu_try_cmpxchg128(pcp, ovalp, nval)	percpu_try_cmpxchg128_op(16,         , pcp, ovalp, nval)
#define this_cpu_try_cmpxchg128(pcp, ovalp, nval)	percpu_try_cmpxchg128_op(16, volatile, pcp, ovalp, nval)
#endif

/*
 * this_cpu_read() makes gcc load the percpu variable every time it is
 * accessed while this_cpu_read_stable() allows the value to be cached.
 * this_cpu_read_stable() is more efficient and can be used if its value
 * is guaranteed to be valid across cpus.  The current users include
 * pcpu_hot.current_task and pcpu_hot.top_of_stack, both of which are
 * actually per-thread variables implemented as per-CPU variables and
 * thus stable for the duration of the respective task.
 */
#define this_cpu_read_stable_1(pcp)	percpu_stable_op(1, "mov", pcp)
#define this_cpu_read_stable_2(pcp)	percpu_stable_op(2, "mov", pcp)
#define this_cpu_read_stable_4(pcp)	percpu_stable_op(4, "mov", pcp)
#define this_cpu_read_stable_8(pcp)	percpu_stable_op(8, "mov", pcp)
#define this_cpu_read_stable(pcp)	__pcpu_size_call_return(this_cpu_read_stable_, pcp)

#ifdef CONFIG_USE_X86_SEG_SUPPORT

#define __raw_cpu_read(qual, pcp)					\
({									\
	*(qual __my_cpu_type(pcp) *)__my_cpu_ptr(&(pcp));		\
})

#define __raw_cpu_write(qual, pcp, val)					\
do {									\
	*(qual __my_cpu_type(pcp) *)__my_cpu_ptr(&(pcp)) = (val);	\
} while (0)

#define raw_cpu_read_1(pcp)		__raw_cpu_read(, pcp)
#define raw_cpu_read_2(pcp)		__raw_cpu_read(, pcp)
#define raw_cpu_read_4(pcp)		__raw_cpu_read(, pcp)
#define raw_cpu_write_1(pcp, val)	__raw_cpu_write(, pcp, val)
#define raw_cpu_write_2(pcp, val)	__raw_cpu_write(, pcp, val)
#define raw_cpu_write_4(pcp, val)	__raw_cpu_write(, pcp, val)

#define this_cpu_read_1(pcp)		__raw_cpu_read(volatile, pcp)
#define this_cpu_read_2(pcp)		__raw_cpu_read(volatile, pcp)
#define this_cpu_read_4(pcp)		__raw_cpu_read(volatile, pcp)
#define this_cpu_write_1(pcp, val)	__raw_cpu_write(volatile, pcp, val)
#define this_cpu_write_2(pcp, val)	__raw_cpu_write(volatile, pcp, val)
#define this_cpu_write_4(pcp, val)	__raw_cpu_write(volatile, pcp, val)

#ifdef CONFIG_X86_64
#define raw_cpu_read_8(pcp)		__raw_cpu_read(, pcp)
#define raw_cpu_write_8(pcp, val)	__raw_cpu_write(, pcp, val)

#define this_cpu_read_8(pcp)		__raw_cpu_read(volatile, pcp)
#define this_cpu_write_8(pcp, val)	__raw_cpu_write(volatile, pcp, val)
#endif

#define this_cpu_read_const(pcp)	__raw_cpu_read(, pcp)
#else /* CONFIG_USE_X86_SEG_SUPPORT */

#define raw_cpu_read_1(pcp)		percpu_from_op(1, , "mov", pcp)
#define raw_cpu_read_2(pcp)		percpu_from_op(2, , "mov", pcp)
#define raw_cpu_read_4(pcp)		percpu_from_op(4, , "mov", pcp)
#define raw_cpu_write_1(pcp, val)	percpu_to_op(1, , "mov", (pcp), val)
#define raw_cpu_write_2(pcp, val)	percpu_to_op(2, , "mov", (pcp), val)
#define raw_cpu_write_4(pcp, val)	percpu_to_op(4, , "mov", (pcp), val)

#define this_cpu_read_1(pcp)		percpu_from_op(1, volatile, "mov", pcp)
#define this_cpu_read_2(pcp)		percpu_from_op(2, volatile, "mov", pcp)
#define this_cpu_read_4(pcp)		percpu_from_op(4, volatile, "mov", pcp)
#define this_cpu_write_1(pcp, val)	percpu_to_op(1, volatile, "mov", (pcp), val)
#define this_cpu_write_2(pcp, val)	percpu_to_op(2, volatile, "mov", (pcp), val)
#define this_cpu_write_4(pcp, val)	percpu_to_op(4, volatile, "mov", (pcp), val)

#ifdef CONFIG_X86_64
#define raw_cpu_read_8(pcp)		percpu_from_op(8, , "mov", pcp)
#define raw_cpu_write_8(pcp, val)	percpu_to_op(8, , "mov", (pcp), val)

#define this_cpu_read_8(pcp)		percpu_from_op(8, volatile, "mov", pcp)
#define this_cpu_write_8(pcp, val)	percpu_to_op(8, volatile, "mov", (pcp), val)
#endif

/*
 * The generic per-cpu infrastrucutre is not suitable for
 * reading const-qualified variables.
 */
#define this_cpu_read_const(pcp)	({ BUILD_BUG(); (typeof(pcp))0; })
#endif /* CONFIG_USE_X86_SEG_SUPPORT */

#define raw_cpu_add_1(pcp, val)		percpu_add_op(1, , (pcp), val)
#define raw_cpu_add_2(pcp, val)		percpu_add_op(2, , (pcp), val)
#define raw_cpu_add_4(pcp, val)		percpu_add_op(4, , (pcp), val)
#define raw_cpu_and_1(pcp, val)		percpu_to_op(1, , "and", (pcp), val)
#define raw_cpu_and_2(pcp, val)		percpu_to_op(2, , "and", (pcp), val)
#define raw_cpu_and_4(pcp, val)		percpu_to_op(4, , "and", (pcp), val)
#define raw_cpu_or_1(pcp, val)		percpu_to_op(1, , "or", (pcp), val)
#define raw_cpu_or_2(pcp, val)		percpu_to_op(2, , "or", (pcp), val)
#define raw_cpu_or_4(pcp, val)		percpu_to_op(4, , "or", (pcp), val)

/*
 * raw_cpu_xchg() can use a load-store since it is not required to be
 * IRQ-safe.
 */
#define raw_percpu_xchg_op(var, nval)					\
({									\
	typeof(var) pxo_ret__ = raw_cpu_read(var);			\
	raw_cpu_write(var, (nval));					\
	pxo_ret__;							\
})

#define raw_cpu_xchg_1(pcp, val)	raw_percpu_xchg_op(pcp, val)
#define raw_cpu_xchg_2(pcp, val)	raw_percpu_xchg_op(pcp, val)
#define raw_cpu_xchg_4(pcp, val)	raw_percpu_xchg_op(pcp, val)

#define this_cpu_add_1(pcp, val)	percpu_add_op(1, volatile, (pcp), val)
#define this_cpu_add_2(pcp, val)	percpu_add_op(2, volatile, (pcp), val)
#define this_cpu_add_4(pcp, val)	percpu_add_op(4, volatile, (pcp), val)
#define this_cpu_and_1(pcp, val)	percpu_to_op(1, volatile, "and", (pcp), val)
#define this_cpu_and_2(pcp, val)	percpu_to_op(2, volatile, "and", (pcp), val)
#define this_cpu_and_4(pcp, val)	percpu_to_op(4, volatile, "and", (pcp), val)
#define this_cpu_or_1(pcp, val)		percpu_to_op(1, volatile, "or", (pcp), val)
#define this_cpu_or_2(pcp, val)		percpu_to_op(2, volatile, "or", (pcp), val)
#define this_cpu_or_4(pcp, val)		percpu_to_op(4, volatile, "or", (pcp), val)
#define this_cpu_xchg_1(pcp, nval)	percpu_xchg_op(1, volatile, pcp, nval)
#define this_cpu_xchg_2(pcp, nval)	percpu_xchg_op(2, volatile, pcp, nval)
#define this_cpu_xchg_4(pcp, nval)	percpu_xchg_op(4, volatile, pcp, nval)

#define raw_cpu_add_return_1(pcp, val)		percpu_add_return_op(1, , pcp, val)
#define raw_cpu_add_return_2(pcp, val)		percpu_add_return_op(2, , pcp, val)
#define raw_cpu_add_return_4(pcp, val)		percpu_add_return_op(4, , pcp, val)
#define raw_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(1, , pcp, oval, nval)
#define raw_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(2, , pcp, oval, nval)
#define raw_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(4, , pcp, oval, nval)
#define raw_cpu_try_cmpxchg_1(pcp, ovalp, nval)	percpu_try_cmpxchg_op(1, , pcp, ovalp, nval)
#define raw_cpu_try_cmpxchg_2(pcp, ovalp, nval)	percpu_try_cmpxchg_op(2, , pcp, ovalp, nval)
#define raw_cpu_try_cmpxchg_4(pcp, ovalp, nval)	percpu_try_cmpxchg_op(4, , pcp, ovalp, nval)

#define this_cpu_add_return_1(pcp, val)		percpu_add_return_op(1, volatile, pcp, val)
#define this_cpu_add_return_2(pcp, val)		percpu_add_return_op(2, volatile, pcp, val)
#define this_cpu_add_return_4(pcp, val)		percpu_add_return_op(4, volatile, pcp, val)
#define this_cpu_cmpxchg_1(pcp, oval, nval)	percpu_cmpxchg_op(1, volatile, pcp, oval, nval)
#define this_cpu_cmpxchg_2(pcp, oval, nval)	percpu_cmpxchg_op(2, volatile, pcp, oval, nval)
#define this_cpu_cmpxchg_4(pcp, oval, nval)	percpu_cmpxchg_op(4, volatile, pcp, oval, nval)
#define this_cpu_try_cmpxchg_1(pcp, ovalp, nval)	percpu_try_cmpxchg_op(1, volatile, pcp, ovalp, nval)
#define this_cpu_try_cmpxchg_2(pcp, ovalp, nval)	percpu_try_cmpxchg_op(2, volatile, pcp, ovalp, nval)
#define this_cpu_try_cmpxchg_4(pcp, ovalp, nval)	percpu_try_cmpxchg_op(4, volatile, pcp, ovalp, nval)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Per cpu atomic 64 bit operations are only available under 64 bit.
 * 32 bit must fall back to generic operations.
 */
#ifdef CONFIG_X86_64
<<<<<<< HEAD
#define __this_cpu_read_8(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define __this_cpu_write_8(pcp, val)	percpu_to_op("mov", (pcp), val)
#define __this_cpu_add_8(pcp, val)	percpu_add_op((pcp), val)
#define __this_cpu_and_8(pcp, val)	percpu_to_op("and", (pcp), val)
#define __this_cpu_or_8(pcp, val)	percpu_to_op("or", (pcp), val)
#define __this_cpu_xor_8(pcp, val)	percpu_to_op("xor", (pcp), val)
#define __this_cpu_add_return_8(pcp, val) percpu_add_return_op(pcp, val)
#define __this_cpu_xchg_8(pcp, nval)	percpu_xchg_op(pcp, nval)
#define __this_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

#define this_cpu_read_8(pcp)		percpu_from_op("mov", (pcp), "m"(pcp))
#define this_cpu_write_8(pcp, val)	percpu_to_op("mov", (pcp), val)
#define this_cpu_add_8(pcp, val)	percpu_add_op((pcp), val)
#define this_cpu_and_8(pcp, val)	percpu_to_op("and", (pcp), val)
#define this_cpu_or_8(pcp, val)		percpu_to_op("or", (pcp), val)
#define this_cpu_xor_8(pcp, val)	percpu_to_op("xor", (pcp), val)
#define this_cpu_add_return_8(pcp, val)	percpu_add_return_op(pcp, val)
#define this_cpu_xchg_8(pcp, nval)	percpu_xchg_op(pcp, nval)
#define this_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(pcp, oval, nval)

/*
 * Pretty complex macro to generate cmpxchg16 instruction.  The instruction
 * is not supported on early AMD64 processors so we must be able to emulate
 * it in software.  The address used in the cmpxchg16 instruction must be
 * aligned to a 16 byte boundary.
 */
#define percpu_cmpxchg16b_double(pcp1, pcp2, o1, o2, n1, n2)		\
({									\
	bool __ret;							\
	typeof(pcp1) __o1 = (o1), __n1 = (n1);				\
	typeof(pcp2) __o2 = (o2), __n2 = (n2);				\
	alternative_io("leaq %P1,%%rsi\n\tcall this_cpu_cmpxchg16b_emu\n\t", \
		       "cmpxchg16b " __percpu_arg(1) "\n\tsetz %0\n\t",	\
		       X86_FEATURE_CX16,				\
		       ASM_OUTPUT2("=a" (__ret), "+m" (pcp1),		\
				   "+m" (pcp2), "+d" (__o2)),		\
		       "b" (__n1), "c" (__n2), "a" (__o1) : "rsi");	\
	__ret;								\
})

#define __this_cpu_cmpxchg_double_8	percpu_cmpxchg16b_double
#define this_cpu_cmpxchg_double_8	percpu_cmpxchg16b_double

#endif

/* This is not atomic against other CPUs -- CPU preemption needs to be off */
#define x86_test_and_clear_bit_percpu(bit, var)				\
({									\
	int old__;							\
	asm volatile("btr %2,"__percpu_arg(1)"\n\tsbbl %0,%0"		\
		     : "=r" (old__), "+m" (var)				\
		     : "dIr" (bit));					\
	old__;								\
})

static __always_inline int x86_this_cpu_constant_test_bit(unsigned int nr,
                        const unsigned long __percpu *addr)
{
	unsigned long __percpu *a = (unsigned long *)addr + nr / BITS_PER_LONG;

	return ((1UL << (nr % BITS_PER_LONG)) & percpu_read(*a)) != 0;
}

static inline int x86_this_cpu_variable_test_bit(int nr,
                        const unsigned long __percpu *addr)
{
	int oldbit;

	asm volatile("bt "__percpu_arg(2)",%1\n\t"
			"sbb %0,%0"
			: "=r" (oldbit)
			: "m" (*(unsigned long *)addr), "Ir" (nr));
=======
#define raw_cpu_add_8(pcp, val)			percpu_add_op(8, , (pcp), val)
#define raw_cpu_and_8(pcp, val)			percpu_to_op(8, , "and", (pcp), val)
#define raw_cpu_or_8(pcp, val)			percpu_to_op(8, , "or", (pcp), val)
#define raw_cpu_add_return_8(pcp, val)		percpu_add_return_op(8, , pcp, val)
#define raw_cpu_xchg_8(pcp, nval)		raw_percpu_xchg_op(pcp, nval)
#define raw_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(8, , pcp, oval, nval)
#define raw_cpu_try_cmpxchg_8(pcp, ovalp, nval)	percpu_try_cmpxchg_op(8, , pcp, ovalp, nval)

#define this_cpu_add_8(pcp, val)		percpu_add_op(8, volatile, (pcp), val)
#define this_cpu_and_8(pcp, val)		percpu_to_op(8, volatile, "and", (pcp), val)
#define this_cpu_or_8(pcp, val)			percpu_to_op(8, volatile, "or", (pcp), val)
#define this_cpu_add_return_8(pcp, val)		percpu_add_return_op(8, volatile, pcp, val)
#define this_cpu_xchg_8(pcp, nval)		percpu_xchg_op(8, volatile, pcp, nval)
#define this_cpu_cmpxchg_8(pcp, oval, nval)	percpu_cmpxchg_op(8, volatile, pcp, oval, nval)
#define this_cpu_try_cmpxchg_8(pcp, ovalp, nval)	percpu_try_cmpxchg_op(8, volatile, pcp, ovalp, nval)
#endif

static __always_inline bool x86_this_cpu_constant_test_bit(unsigned int nr,
                        const unsigned long __percpu *addr)
{
	unsigned long __percpu *a =
		(unsigned long __percpu *)addr + nr / BITS_PER_LONG;

#ifdef CONFIG_X86_64
	return ((1UL << (nr % BITS_PER_LONG)) & raw_cpu_read_8(*a)) != 0;
#else
	return ((1UL << (nr % BITS_PER_LONG)) & raw_cpu_read_4(*a)) != 0;
#endif
}

static inline bool x86_this_cpu_variable_test_bit(int nr,
                        const unsigned long __percpu *addr)
{
	bool oldbit;

	asm volatile("btl "__percpu_arg(2)",%1"
			CC_SET(c)
			: CC_OUT(c) (oldbit)
			: "m" (*__my_cpu_ptr((unsigned long __percpu *)(addr))), "Ir" (nr));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return oldbit;
}

#define x86_this_cpu_test_bit(nr, addr)			\
	(__builtin_constant_p((nr))			\
	 ? x86_this_cpu_constant_test_bit((nr), (addr))	\
	 : x86_this_cpu_variable_test_bit((nr), (addr)))


#include <asm-generic/percpu.h>

/* We can use this directly for local CPU (faster). */
<<<<<<< HEAD
DECLARE_PER_CPU(unsigned long, this_cpu_off);
=======
DECLARE_PER_CPU_READ_MOSTLY(unsigned long, this_cpu_off);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* !__ASSEMBLY__ */

#ifdef CONFIG_SMP

/*
 * Define the "EARLY_PER_CPU" macros.  These are used for some per_cpu
 * variables that are initialized and accessed before there are per_cpu
 * areas allocated.
 */

#define	DEFINE_EARLY_PER_CPU(_type, _name, _initvalue)			\
	DEFINE_PER_CPU(_type, _name) = _initvalue;			\
	__typeof__(_type) _name##_early_map[NR_CPUS] __initdata =	\
				{ [0 ... NR_CPUS-1] = _initvalue };	\
	__typeof__(_type) *_name##_early_ptr __refdata = _name##_early_map

<<<<<<< HEAD
=======
#define DEFINE_EARLY_PER_CPU_READ_MOSTLY(_type, _name, _initvalue)	\
	DEFINE_PER_CPU_READ_MOSTLY(_type, _name) = _initvalue;		\
	__typeof__(_type) _name##_early_map[NR_CPUS] __initdata =	\
				{ [0 ... NR_CPUS-1] = _initvalue };	\
	__typeof__(_type) *_name##_early_ptr __refdata = _name##_early_map

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define EXPORT_EARLY_PER_CPU_SYMBOL(_name)			\
	EXPORT_PER_CPU_SYMBOL(_name)

#define DECLARE_EARLY_PER_CPU(_type, _name)			\
	DECLARE_PER_CPU(_type, _name);				\
	extern __typeof__(_type) *_name##_early_ptr;		\
	extern __typeof__(_type)  _name##_early_map[]

<<<<<<< HEAD
=======
#define DECLARE_EARLY_PER_CPU_READ_MOSTLY(_type, _name)		\
	DECLARE_PER_CPU_READ_MOSTLY(_type, _name);		\
	extern __typeof__(_type) *_name##_early_ptr;		\
	extern __typeof__(_type)  _name##_early_map[]

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	early_per_cpu_ptr(_name) (_name##_early_ptr)
#define	early_per_cpu_map(_name, _idx) (_name##_early_map[_idx])
#define	early_per_cpu(_name, _cpu) 				\
	*(early_per_cpu_ptr(_name) ?				\
		&early_per_cpu_ptr(_name)[_cpu] :		\
		&per_cpu(_name, _cpu))

#else	/* !CONFIG_SMP */
#define	DEFINE_EARLY_PER_CPU(_type, _name, _initvalue)		\
	DEFINE_PER_CPU(_type, _name) = _initvalue

<<<<<<< HEAD
=======
#define DEFINE_EARLY_PER_CPU_READ_MOSTLY(_type, _name, _initvalue)	\
	DEFINE_PER_CPU_READ_MOSTLY(_type, _name) = _initvalue

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define EXPORT_EARLY_PER_CPU_SYMBOL(_name)			\
	EXPORT_PER_CPU_SYMBOL(_name)

#define DECLARE_EARLY_PER_CPU(_type, _name)			\
	DECLARE_PER_CPU(_type, _name)

<<<<<<< HEAD
=======
#define DECLARE_EARLY_PER_CPU_READ_MOSTLY(_type, _name)		\
	DECLARE_PER_CPU_READ_MOSTLY(_type, _name)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	early_per_cpu(_name, _cpu) per_cpu(_name, _cpu)
#define	early_per_cpu_ptr(_name) NULL
/* no early_per_cpu_map() */

#endif	/* !CONFIG_SMP */

#endif /* _ASM_X86_PERCPU_H */
