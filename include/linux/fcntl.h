<<<<<<< HEAD
#ifndef _LINUX_FCNTL_H
#define _LINUX_FCNTL_H

#include <asm/fcntl.h>

#define F_SETLEASE	(F_LINUX_SPECIFIC_BASE + 0)
#define F_GETLEASE	(F_LINUX_SPECIFIC_BASE + 1)

/*
 * Cancel a blocking posix lock; internal use only until we expose an
 * asynchronous lock api to userspace:
 */
#define F_CANCELLK	(F_LINUX_SPECIFIC_BASE + 5)

/* Create a file descriptor with FD_CLOEXEC set. */
#define F_DUPFD_CLOEXEC	(F_LINUX_SPECIFIC_BASE + 6)

/*
 * Request nofications on a directory.
 * See below for events that may be notified.
 */
#define F_NOTIFY	(F_LINUX_SPECIFIC_BASE+2)

/*
 * Set and get of pipe page size array
 */
#define F_SETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 7)
#define F_GETPIPE_SZ	(F_LINUX_SPECIFIC_BASE + 8)

/*
 * Types of directory notifications that may be requested.
 */
#define DN_ACCESS	0x00000001	/* File accessed */
#define DN_MODIFY	0x00000002	/* File modified */
#define DN_CREATE	0x00000004	/* File created */
#define DN_DELETE	0x00000008	/* File removed */
#define DN_RENAME	0x00000010	/* File renamed */
#define DN_ATTRIB	0x00000020	/* File changed attibutes */
#define DN_MULTISHOT	0x80000000	/* Don't remove notifier */

#define AT_FDCWD		-100    /* Special value used to indicate
                                           openat should use the current
                                           working directory. */
#define AT_SYMLINK_NOFOLLOW	0x100   /* Do not follow symbolic links.  */
#define AT_REMOVEDIR		0x200   /* Remove directory instead of
                                           unlinking file.  */
#define AT_SYMLINK_FOLLOW	0x400   /* Follow symbolic links.  */
#define AT_NO_AUTOMOUNT		0x800	/* Suppress terminal automount traversal */
#define AT_EMPTY_PATH		0x1000	/* Allow empty relative pathname */

#ifdef __KERNEL__

#ifndef force_o_largefile
#define force_o_largefile() (BITS_PER_LONG != 32)
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FCNTL_H
#define _LINUX_FCNTL_H

#include <linux/stat.h>
#include <uapi/linux/fcntl.h>

/* List of all valid flags for the open/openat flags argument: */
#define VALID_OPEN_FLAGS \
	(O_RDONLY | O_WRONLY | O_RDWR | O_CREAT | O_EXCL | O_NOCTTY | O_TRUNC | \
	 O_APPEND | O_NDELAY | O_NONBLOCK | __O_SYNC | O_DSYNC | \
	 FASYNC	| O_DIRECT | O_LARGEFILE | O_DIRECTORY | O_NOFOLLOW | \
	 O_NOATIME | O_CLOEXEC | O_PATH | __O_TMPFILE)

/* List of all valid flags for the how->resolve argument: */
#define VALID_RESOLVE_FLAGS \
	(RESOLVE_NO_XDEV | RESOLVE_NO_MAGICLINKS | RESOLVE_NO_SYMLINKS | \
	 RESOLVE_BENEATH | RESOLVE_IN_ROOT | RESOLVE_CACHED)

/* List of all open_how "versions". */
#define OPEN_HOW_SIZE_VER0	24 /* sizeof first published struct */
#define OPEN_HOW_SIZE_LATEST	OPEN_HOW_SIZE_VER0

#ifndef force_o_largefile
#define force_o_largefile() (!IS_ENABLED(CONFIG_ARCH_32BIT_OFF_T))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#if BITS_PER_LONG == 32
#define IS_GETLK32(cmd)		((cmd) == F_GETLK)
#define IS_SETLK32(cmd)		((cmd) == F_SETLK)
#define IS_SETLKW32(cmd)	((cmd) == F_SETLKW)
#define IS_GETLK64(cmd)		((cmd) == F_GETLK64)
#define IS_SETLK64(cmd)		((cmd) == F_SETLK64)
#define IS_SETLKW64(cmd)	((cmd) == F_SETLKW64)
#else
#define IS_GETLK32(cmd)		(0)
#define IS_SETLK32(cmd)		(0)
#define IS_SETLKW32(cmd)	(0)
#define IS_GETLK64(cmd)		((cmd) == F_GETLK)
#define IS_SETLK64(cmd)		((cmd) == F_SETLK)
#define IS_SETLKW64(cmd)	((cmd) == F_SETLKW)
#endif /* BITS_PER_LONG == 32 */

#define IS_GETLK(cmd)	(IS_GETLK32(cmd)  || IS_GETLK64(cmd))
#define IS_SETLK(cmd)	(IS_SETLK32(cmd)  || IS_SETLK64(cmd))
#define IS_SETLKW(cmd)	(IS_SETLKW32(cmd) || IS_SETLKW64(cmd))

<<<<<<< HEAD
#endif /* __KERNEL__ */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
