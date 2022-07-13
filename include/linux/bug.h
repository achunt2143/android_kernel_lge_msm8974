<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_BUG_H
#define _LINUX_BUG_H

#include <asm/bug.h>
<<<<<<< HEAD
=======
#include <linux/compiler.h>
#include <linux/build_bug.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum bug_trap_type {
	BUG_TRAP_TYPE_NONE = 0,
	BUG_TRAP_TYPE_WARN = 1,
	BUG_TRAP_TYPE_BUG = 2,
};

struct pt_regs;

#ifdef __CHECKER__
<<<<<<< HEAD
#define BUILD_BUG_ON_NOT_POWER_OF_2(n)
#define BUILD_BUG_ON_ZERO(e) (0)
#define BUILD_BUG_ON_NULL(e) ((void*)0)
#define BUILD_BUG_ON(condition)
#define BUILD_BUG() (0)
#else /* __CHECKER__ */

/* Force a compilation error if a constant expression is not a power of 2 */
#define BUILD_BUG_ON_NOT_POWER_OF_2(n)			\
	BUILD_BUG_ON((n) == 0 || (((n) & ((n) - 1)) != 0))

/* Force a compilation error if condition is true, but also produce a
   result (of value 0 and type size_t), so the expression can be used
   e.g. in a structure initializer (or where-ever else comma expressions
   aren't permitted). */
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int:-!!(e); }))
#define BUILD_BUG_ON_NULL(e) ((void *)sizeof(struct { int:-!!(e); }))

/**
 * BUILD_BUG_ON - break compile if a condition is true.
 * @condition: the condition which the compiler should know is false.
 *
 * If you have some code which relies on certain constants being equal, or
 * other compile-time-evaluated condition, you should use BUILD_BUG_ON to
 * detect if someone changes it.
 *
 * The implementation uses gcc's reluctance to create a negative array, but
 * gcc (as of 4.4) only emits that error for obvious cases (eg. not arguments
 * to inline functions).  So as a fallback we use the optimizer; if it can't
 * prove the condition is false, it will cause a link error on the undefined
 * "__build_bug_on_failed".  This error message can be harder to track down
 * though, hence the two different methods.
 */
#ifndef __OPTIMIZE__
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))
#else
extern int __build_bug_on_failed;
#define BUILD_BUG_ON(condition)					\
	do {							\
		((void)sizeof(char[1 - 2*!!(condition)]));	\
		if (condition) __build_bug_on_failed = 1;	\
	} while(0)
#endif

/**
 * BUILD_BUG - break compile if used.
 *
 * If you have some code that you expect the compiler to eliminate at
 * build time, you should use BUILD_BUG to detect if it is
 * unexpectedly used.
 */
#define BUILD_BUG()						\
	do {							\
		extern void __build_bug_failed(void)		\
			__linktime_error("BUILD_BUG failed");	\
		__build_bug_failed();				\
=======
#define MAYBE_BUILD_BUG_ON(cond) (0)
#else /* __CHECKER__ */

#define MAYBE_BUILD_BUG_ON(cond)			\
	do {						\
		if (__builtin_constant_p((cond)))       \
			BUILD_BUG_ON(cond);             \
		else                                    \
			BUG_ON(cond);                   \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (0)

#endif	/* __CHECKER__ */

#ifdef CONFIG_GENERIC_BUG
#include <asm-generic/bug.h>

static inline int is_warning_bug(const struct bug_entry *bug)
{
	return bug->flags & BUGFLAG_WARNING;
}

<<<<<<< HEAD
const struct bug_entry *find_bug(unsigned long bugaddr);
=======
void bug_get_file_line(struct bug_entry *bug, const char **file,
		       unsigned int *line);

struct bug_entry *find_bug(unsigned long bugaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum bug_trap_type report_bug(unsigned long bug_addr, struct pt_regs *regs);

/* These are defined by the architecture */
int is_valid_bugaddr(unsigned long addr);

<<<<<<< HEAD
#else	/* !CONFIG_GENERIC_BUG */

=======
void generic_bug_clear_once(void);

#else	/* !CONFIG_GENERIC_BUG */

static inline void *find_bug(unsigned long bugaddr)
{
	return NULL;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline enum bug_trap_type report_bug(unsigned long bug_addr,
					    struct pt_regs *regs)
{
	return BUG_TRAP_TYPE_BUG;
}

<<<<<<< HEAD
#endif	/* CONFIG_GENERIC_BUG */

#ifdef CONFIG_PANIC_ON_DATA_CORRUPTION
#define PANIC_CORRUPTION 1
#else
#define PANIC_CORRUPTION 0
#endif  /* CONFIG_PANIC_ON_DATA_CORRUPTION */
=======
struct bug_entry;
static inline void bug_get_file_line(struct bug_entry *bug, const char **file,
				     unsigned int *line)
{
	*file = NULL;
	*line = 0;
}

static inline void generic_bug_clear_once(void) {}

#endif	/* CONFIG_GENERIC_BUG */

/*
 * Since detected data corruption should stop operation on the affected
 * structures. Return value must be checked and sanely acted on by caller.
 */
static inline __must_check bool check_data_corruption(bool v) { return v; }
#define CHECK_DATA_CORRUPTION(condition, fmt, ...)			 \
	check_data_corruption(({					 \
		bool corruption = unlikely(condition);			 \
		if (corruption) {					 \
			if (IS_ENABLED(CONFIG_BUG_ON_DATA_CORRUPTION)) { \
				pr_err(fmt, ##__VA_ARGS__);		 \
				BUG();					 \
			} else						 \
				WARN(1, fmt, ##__VA_ARGS__);		 \
		}							 \
		corruption;						 \
	}))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _LINUX_BUG_H */
