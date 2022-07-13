<<<<<<< HEAD
#ifndef __LINUX_COMPILER_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_COMPILER_TYPES_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#error "Please don't include <linux/compiler-gcc.h> directly, include <linux/compiler.h> instead."
#endif

/*
 * Common definitions for all gcc versions go here.
 */
<<<<<<< HEAD
#define GCC_VERSION (__GNUC__ * 10000 \
		   + __GNUC_MINOR__ * 100 \
		   + __GNUC_PATCHLEVEL__)


/* Optimization barrier */
/* The "volatile" is due to gcc bugs */
#define barrier() __asm__ __volatile__("": : :"memory")
=======
#define GCC_VERSION (__GNUC__ * 10000		\
		     + __GNUC_MINOR__ * 100	\
		     + __GNUC_PATCHLEVEL__)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This macro obfuscates arithmetic on a variable address so that gcc
 * shouldn't recognize the original var, and make assumptions about it.
 *
 * This is needed because the C standard makes it undefined to do
 * pointer arithmetic on "objects" outside their boundaries and the
 * gcc optimizers assume this is the case. In particular they
 * assume such arithmetic does not wrap.
 *
 * A miscompilation has been observed because of this on PPC.
 * To work around it we hide the relationship of the pointer and the object
 * using this macro.
 *
 * Versions of the ppc64 compiler before 4.1 had a bug where use of
 * RELOC_HIDE could trash r30. The bug can be worked around by changing
 * the inline assembly constraint from =g to =r, in this particular
 * case either is valid.
 */
<<<<<<< HEAD
#define RELOC_HIDE(ptr, off)					\
  ({ unsigned long __ptr;					\
    __asm__ ("" : "=r"(__ptr) : "0"(ptr));		\
    (typeof(ptr)) (__ptr + (off)); })

/* Make the optimizer believe the variable can be manipulated arbitrarily. */
#define OPTIMIZER_HIDE_VAR(var) __asm__ ("" : "=r" (var) : "0" (var))

#ifdef __CHECKER__
#define __must_be_array(arr) 0
#else
/* &a[0] degrades to a pointer: a different type from an array */
#define __must_be_array(a) BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))
#endif

/*
 * Force always-inline if the user requests it so via the .config,
 * or if gcc is too old:
 */
#if !defined(CONFIG_ARCH_SUPPORTS_OPTIMIZED_INLINING) || \
    !defined(CONFIG_OPTIMIZE_INLINING) || (__GNUC__ < 4)
# define inline		inline		__attribute__((always_inline))
# define __inline__	__inline__	__attribute__((always_inline))
# define __inline	__inline	__attribute__((always_inline))
#else
/* A lot of inline functions can cause havoc with function tracing */
# define inline		inline		notrace
# define __inline__	__inline__	notrace
# define __inline	__inline	notrace
#endif

#define __deprecated			__attribute__((deprecated))
#define __packed			__attribute__((packed))
#define __weak				__attribute__((weak))

/*
 * it doesn't make sense on ARM (currently the only user of __naked) to trace
 * naked functions because then mcount is called without stack and frame pointer
 * being set up and there is no chance to restore the lr register to the value
 * before mcount was called.
 *
 * The asm() bodies of naked functions often depend on standard calling conventions,
 * therefore they must be noinline and noclone.  GCC 4.[56] currently fail to enforce
 * this, so we must do so ourselves.  See GCC PR44290.
 */
#define __naked				__attribute__((naked)) noinline __noclone notrace

#define __noreturn			__attribute__((noreturn))

/*
 * From the GCC manual:
 *
 * Many functions have no effects except the return value and their
 * return value depends only on the parameters and/or global
 * variables.  Such a function can be subject to common subexpression
 * elimination and loop optimization just as an arithmetic operator
 * would be.
 * [...]
 */
#define __pure				__attribute__((pure))
#define __aligned(x)			__attribute__((aligned(x)))
#define __printf(a, b)			__attribute__((format(printf, a, b)))
#define __scanf(a, b)			__attribute__((format(scanf, a, b)))
#define  noinline			__attribute__((noinline))
#define __attribute_const__		__attribute__((__const__))
#define __maybe_unused			__attribute__((unused))
#define __always_unused			__attribute__((unused))

#define __gcc_header(x) #x
#define _gcc_header(x) __gcc_header(linux/compiler-gcc##x.h)
#define gcc_header(x) _gcc_header(x)
#include gcc_header(__GNUC__)

#if !defined(__noclone)
#define __noclone	/* not needed */
#endif

/*
 * A trick to suppress uninitialized variable warning without generating any
 * code
 */
#define uninitialized_var(x) x = x

#define __always_inline		inline __attribute__((always_inline))
=======
#define RELOC_HIDE(ptr, off)						\
({									\
	unsigned long __ptr;						\
	__asm__ ("" : "=r"(__ptr) : "0"(ptr));				\
	(typeof(ptr)) (__ptr + (off));					\
})

#ifdef CONFIG_MITIGATION_RETPOLINE
#define __noretpoline __attribute__((__indirect_branch__("keep")))
#endif

#if defined(LATENT_ENTROPY_PLUGIN) && !defined(__CHECKER__)
#define __latent_entropy __attribute__((latent_entropy))
#endif

/*
 * calling noreturn functions, __builtin_unreachable() and __builtin_trap()
 * confuse the stack allocation in gcc, leading to overly large stack
 * frames, see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=82365
 *
 * Adding an empty inline assembly before it works around the problem
 */
#define barrier_before_unreachable() asm volatile("")

/*
 * Mark a position in code as unreachable.  This can be used to
 * suppress control flow warnings after asm blocks that transfer
 * control elsewhere.
 */
#define unreachable() \
	do {					\
		annotate_unreachable();		\
		barrier_before_unreachable();	\
		__builtin_unreachable();	\
	} while (0)

/*
 * GCC 'asm goto' with outputs miscompiles certain code sequences:
 *
 *   https://gcc.gnu.org/bugzilla/show_bug.cgi?id=113921
 *
 * Work around it via the same compiler barrier quirk that we used
 * to use for the old 'asm goto' workaround.
 *
 * Also, always mark such 'asm goto' statements as volatile: all
 * asm goto statements are supposed to be volatile as per the
 * documentation, but some versions of gcc didn't actually do
 * that for asms with outputs:
 *
 *    https://gcc.gnu.org/bugzilla/show_bug.cgi?id=98619
 */
#ifdef CONFIG_GCC_ASM_GOTO_OUTPUT_WORKAROUND
#define asm_goto_output(x...) \
	do { asm volatile goto(x); asm (""); } while (0)
#endif

#if defined(CONFIG_ARCH_USE_BUILTIN_BSWAP)
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__
#define __HAVE_BUILTIN_BSWAP16__
#endif /* CONFIG_ARCH_USE_BUILTIN_BSWAP */

#if GCC_VERSION >= 70000
#define KASAN_ABI_VERSION 5
#else
#define KASAN_ABI_VERSION 4
#endif

#ifdef CONFIG_SHADOW_CALL_STACK
#define __noscs __attribute__((__no_sanitize__("shadow-call-stack")))
#endif

#define __no_sanitize_address __attribute__((__no_sanitize_address__))

#if defined(__SANITIZE_THREAD__)
#define __no_sanitize_thread __attribute__((__no_sanitize_thread__))
#else
#define __no_sanitize_thread
#endif

#define __no_sanitize_undefined __attribute__((__no_sanitize_undefined__))

/*
 * Only supported since gcc >= 12
 */
#if defined(CONFIG_KCOV) && __has_attribute(__no_sanitize_coverage__)
#define __no_sanitize_coverage __attribute__((__no_sanitize_coverage__))
#else
#define __no_sanitize_coverage
#endif

/*
 * Treat __SANITIZE_HWADDRESS__ the same as __SANITIZE_ADDRESS__ in the kernel,
 * matching the defines used by Clang.
 */
#ifdef __SANITIZE_HWADDRESS__
#define __SANITIZE_ADDRESS__
#endif

/*
 * GCC does not support KMSAN.
 */
#define __no_sanitize_memory
#define __no_kmsan_checks

/*
 * Turn individual warnings and errors on and off locally, depending
 * on version.
 */
#define __diag_GCC(version, severity, s) \
	__diag_GCC_ ## version(__diag_GCC_ ## severity s)

/* Severity used in pragma directives */
#define __diag_GCC_ignore	ignored
#define __diag_GCC_warn		warning
#define __diag_GCC_error	error

#define __diag_str1(s)		#s
#define __diag_str(s)		__diag_str1(s)
#define __diag(s)		_Pragma(__diag_str(GCC diagnostic s))

#if GCC_VERSION >= 80000
#define __diag_GCC_8(s)		__diag(s)
#else
#define __diag_GCC_8(s)
#endif

#define __diag_ignore_all(option, comment) \
	__diag(__diag_GCC_ignore option)

/*
 * Prior to 9.1, -Wno-alloc-size-larger-than (and therefore the "alloc_size"
 * attribute) do not work, and must be disabled.
 */
#if GCC_VERSION < 90100
#undef __alloc_size__
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
