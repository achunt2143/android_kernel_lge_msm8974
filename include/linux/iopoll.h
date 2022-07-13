<<<<<<< HEAD
/*
 * Copyright (c) 2012-2014 The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2012-2014 The Linux Foundation. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _LINUX_IOPOLL_H
#define _LINUX_IOPOLL_H

#include <linux/kernel.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <linux/hrtimer.h>
#include <linux/delay.h>
#include <asm-generic/errno.h>
#include <asm/io.h>

/**
 * readl_poll_timeout - Periodically poll an address until a condition is met or a timeout occurs
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @sleep_us: Maximum time to sleep between reads in uS (0 tight-loops)
 * @timeout_us: Timeout in uS, 0 means never timeout
=======
#include <linux/ktime.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/io.h>

/**
 * read_poll_timeout - Periodically poll an address until a condition is
 *			met or a timeout occurs
 * @op: accessor function (takes @args as its arguments)
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @sleep_us: Maximum time to sleep between reads in us (0
 *            tight-loops).  Should be less than ~20ms since usleep_range
 *            is used (see Documentation/timers/timers-howto.rst).
 * @timeout_us: Timeout in us, 0 means never timeout
 * @sleep_before_read: if it is true, sleep @sleep_us before read.
 * @args: arguments for @op poll
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @args is stored in @val. Must not
 * be called from atomic context if sleep_us or timeout_us are used.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define read_poll_timeout(op, val, cond, sleep_us, timeout_us, \
				sleep_before_read, args...) \
({ \
	u64 __timeout_us = (timeout_us); \
	unsigned long __sleep_us = (sleep_us); \
	ktime_t __timeout = ktime_add_us(ktime_get(), __timeout_us); \
	might_sleep_if((__sleep_us) != 0); \
	if (sleep_before_read && __sleep_us) \
		usleep_range((__sleep_us >> 2) + 1, __sleep_us); \
	for (;;) { \
		(val) = op(args); \
		if (cond) \
			break; \
		if (__timeout_us && \
		    ktime_compare(ktime_get(), __timeout) > 0) { \
			(val) = op(args); \
			break; \
		} \
		if (__sleep_us) \
			usleep_range((__sleep_us >> 2) + 1, __sleep_us); \
		cpu_relax(); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

/**
 * read_poll_timeout_atomic - Periodically poll an address until a condition is
 * 				met or a timeout occurs
 * @op: accessor function (takes @args as its arguments)
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @delay_us: Time to udelay between reads in us (0 tight-loops).  Should
 *            be less than ~10us since udelay is used (see
 *            Documentation/timers/timers-howto.rst).
 * @timeout_us: Timeout in us, 0 means never timeout
 * @delay_before_read: if it is true, delay @delay_us before read.
 * @args: arguments for @op poll
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @args is stored in @val.
 *
 * This macro does not rely on timekeeping.  Hence it is safe to call even when
 * timekeeping is suspended, at the expense of an underestimation of wall clock
 * time, which is rather minimal with a non-zero delay_us.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define read_poll_timeout_atomic(op, val, cond, delay_us, timeout_us, \
					delay_before_read, args...) \
({ \
	u64 __timeout_us = (timeout_us); \
	s64 __left_ns = __timeout_us * NSEC_PER_USEC; \
	unsigned long __delay_us = (delay_us); \
	u64 __delay_ns = __delay_us * NSEC_PER_USEC; \
	if (delay_before_read && __delay_us) { \
		udelay(__delay_us); \
		if (__timeout_us) \
			__left_ns -= __delay_ns; \
	} \
	for (;;) { \
		(val) = op(args); \
		if (cond) \
			break; \
		if (__timeout_us && __left_ns < 0) { \
			(val) = op(args); \
			break; \
		} \
		if (__delay_us) { \
			udelay(__delay_us); \
			if (__timeout_us) \
				__left_ns -= __delay_ns; \
		} \
		cpu_relax(); \
		if (__timeout_us) \
			__left_ns--; \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

/**
 * readx_poll_timeout - Periodically poll an address until a condition is met or a timeout occurs
 * @op: accessor function (takes @addr as its only argument)
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @sleep_us: Maximum time to sleep between reads in us (0
 *            tight-loops).  Should be less than ~20ms since usleep_range
 *            is used (see Documentation/timers/timers-howto.rst).
 * @timeout_us: Timeout in us, 0 means never timeout
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @addr is stored in @val. Must not
 * be called from atomic context if sleep_us or timeout_us are used.
<<<<<<< HEAD
 */
#define readl_poll_timeout(addr, val, cond, sleep_us, timeout_us) \
({ \
	ktime_t timeout = ktime_add_us(ktime_get(), timeout_us); \
	might_sleep_if(timeout_us); \
	for (;;) { \
		(val) = readl(addr); \
		if (cond) \
			break; \
		if (timeout_us && ktime_compare(ktime_get(), timeout) > 0) { \
			(val) = readl(addr); \
			break; \
		} \
		if (sleep_us) \
			usleep_range(DIV_ROUND_UP(sleep_us, 4), sleep_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

/**
 * readl_poll_timeout_noirq - Periodically poll an address until a condition is met or a timeout occurs
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @max_reads: Maximum number of reads before giving up
 * @time_between_us: Time to udelay() between successive reads
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout.
 */
#define readl_poll_timeout_noirq(addr, val, cond, max_reads, time_between_us) \
({ \
	int count; \
	for (count = (max_reads); count > 0; count--) { \
		(val) = readl(addr); \
		if (cond) \
			break; \
		udelay(time_between_us); \
	} \
	(cond) ? 0 : -ETIMEDOUT; \
})

/**
 * readl_poll - Periodically poll an address until a condition is met
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @sleep_us: Maximum time to sleep between reads in uS (0 tight-loops)
 *
 * Must not be called from atomic context if sleep_us is used.
 */
#define readl_poll(addr, val, cond, sleep_us) \
	readl_poll_timeout(addr, val, cond, sleep_us, 0)

/**
 * readl_tight_poll_timeout - Tight-loop on an address until a condition is met or a timeout occurs
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @timeout_us: Timeout in uS, 0 means never timeout
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @addr is stored in @val. Must not
 * be called from atomic context if timeout_us is used.
 */
#define readl_tight_poll_timeout(addr, val, cond, timeout_us) \
	readl_poll_timeout(addr, val, cond, 0, timeout_us)

/**
 * readl_tight_poll - Tight-loop on an address until a condition is met
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 *
 * May be called from atomic context.
 */
#define readl_tight_poll(addr, val, cond) \
	readl_poll_timeout(addr, val, cond, 0, 0)
=======
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define readx_poll_timeout(op, addr, val, cond, sleep_us, timeout_us)	\
	read_poll_timeout(op, val, cond, sleep_us, timeout_us, false, addr)

/**
 * readx_poll_timeout_atomic - Periodically poll an address until a condition is met or a timeout occurs
 * @op: accessor function (takes @addr as its only argument)
 * @addr: Address to poll
 * @val: Variable to read the value into
 * @cond: Break condition (usually involving @val)
 * @delay_us: Time to udelay between reads in us (0 tight-loops).  Should
 *            be less than ~10us since udelay is used (see
 *            Documentation/timers/timers-howto.rst).
 * @timeout_us: Timeout in us, 0 means never timeout
 *
 * Returns 0 on success and -ETIMEDOUT upon a timeout. In either
 * case, the last read value at @addr is stored in @val.
 *
 * When available, you'll probably want to use one of the specialized
 * macros defined below rather than this macro directly.
 */
#define readx_poll_timeout_atomic(op, addr, val, cond, delay_us, timeout_us) \
	read_poll_timeout_atomic(op, val, cond, delay_us, timeout_us, false, addr)

#define readb_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readb, addr, val, cond, delay_us, timeout_us)

#define readb_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readb, addr, val, cond, delay_us, timeout_us)

#define readw_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readw, addr, val, cond, delay_us, timeout_us)

#define readw_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readw, addr, val, cond, delay_us, timeout_us)

#define readl_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readl, addr, val, cond, delay_us, timeout_us)

#define readl_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readl, addr, val, cond, delay_us, timeout_us)

#define readq_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readq, addr, val, cond, delay_us, timeout_us)

#define readq_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readq, addr, val, cond, delay_us, timeout_us)

#define readb_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readb_relaxed, addr, val, cond, delay_us, timeout_us)

#define readb_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readb_relaxed, addr, val, cond, delay_us, timeout_us)

#define readw_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readw_relaxed, addr, val, cond, delay_us, timeout_us)

#define readw_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readw_relaxed, addr, val, cond, delay_us, timeout_us)

#define readl_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readl_relaxed, addr, val, cond, delay_us, timeout_us)

#define readl_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readl_relaxed, addr, val, cond, delay_us, timeout_us)

#define readq_relaxed_poll_timeout(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout(readq_relaxed, addr, val, cond, delay_us, timeout_us)

#define readq_relaxed_poll_timeout_atomic(addr, val, cond, delay_us, timeout_us) \
	readx_poll_timeout_atomic(readq_relaxed, addr, val, cond, delay_us, timeout_us)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_IOPOLL_H */
