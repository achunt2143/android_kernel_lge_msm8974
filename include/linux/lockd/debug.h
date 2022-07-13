<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/include/linux/lockd/debug.h
 *
 * Debugging stuff.
 *
 * Copyright (C) 1996 Olaf Kirch <okir@monad.swb.de>
 */

#ifndef LINUX_LOCKD_DEBUG_H
#define LINUX_LOCKD_DEBUG_H

<<<<<<< HEAD
#ifdef __KERNEL__

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sunrpc/debug.h>

/*
 * Enable lockd debugging.
 * Requires RPC_DEBUG.
 */
<<<<<<< HEAD
#ifdef RPC_DEBUG
# define LOCKD_DEBUG		1
#endif

#undef ifdebug
#if defined(RPC_DEBUG) && defined(LOCKD_DEBUG)
=======
#undef ifdebug
#if IS_ENABLED(CONFIG_SUNRPC_DEBUG)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# define ifdebug(flag)		if (unlikely(nlm_debug & NLMDBG_##flag))
#else
# define ifdebug(flag)		if (0)
#endif

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Debug flags
 */
#define NLMDBG_SVC		0x0001
#define NLMDBG_CLIENT		0x0002
#define NLMDBG_CLNTLOCK		0x0004
#define NLMDBG_SVCLOCK		0x0008
#define NLMDBG_MONITOR		0x0010
#define NLMDBG_CLNTSUBS		0x0020
#define NLMDBG_SVCSUBS		0x0040
#define NLMDBG_HOSTCACHE	0x0080
#define NLMDBG_XDR		0x0100
#define NLMDBG_ALL		0x7fff

#endif /* LINUX_LOCKD_DEBUG_H */
