<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_TRACE_CLOCK_H
#define _LINUX_TRACE_CLOCK_H

/*
 * 3 trace clock variants, with differing scalability/precision
 * tradeoffs:
 *
 *  -   local: CPU-local trace clock
 *  -  medium: scalable global clock with some jitter
 *  -  global: globally monotonic, serialized clock
 */
#include <linux/compiler.h>
#include <linux/types.h>

<<<<<<< HEAD
extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
=======
#include <asm/trace_clock.h>

extern u64 notrace trace_clock_local(void);
extern u64 notrace trace_clock(void);
extern u64 notrace trace_clock_jiffies(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern u64 notrace trace_clock_global(void);
extern u64 notrace trace_clock_counter(void);

#endif /* _LINUX_TRACE_CLOCK_H */
