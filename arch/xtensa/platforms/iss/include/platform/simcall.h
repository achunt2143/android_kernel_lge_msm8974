/*
 * include/asm-xtensa/platform-iss/simcall.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 Tensilica Inc.
<<<<<<< HEAD
=======
 * Copyright (C) 2017 - 2021 Cadence Design Systems Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _XTENSA_PLATFORM_ISS_SIMCALL_H
#define _XTENSA_PLATFORM_ISS_SIMCALL_H

<<<<<<< HEAD

/*
 *  System call like services offered by the simulator host.
 */

#define SYS_nop		0	/* unused */
#define SYS_exit	1	/*x*/
#define SYS_fork	2
#define SYS_read	3	/*x*/
#define SYS_write	4	/*x*/
#define SYS_open	5	/*x*/
#define SYS_close	6	/*x*/
#define SYS_rename	7	/*x 38 - waitpid */
#define SYS_creat	8	/*x*/
#define SYS_link	9	/*x (not implemented on WIN32) */
#define SYS_unlink	10	/*x*/
#define SYS_execv	11	/* n/a - execve */
#define SYS_execve	12	/* 11 - chdir */
#define SYS_pipe	13	/* 42 - time */
#define SYS_stat	14	/* 106 - mknod */
#define SYS_chmod	15
#define SYS_chown	16	/* 202 - lchown */
#define SYS_utime	17	/* 30 - break */
#define SYS_wait	18	/* n/a - oldstat */
#define SYS_lseek	19	/*x*/
#define SYS_getpid	20
#define SYS_isatty	21	/* n/a - mount */
#define SYS_fstat	22	/* 108 - oldumount */
#define SYS_time	23	/* 13 - setuid */
#define SYS_gettimeofday 24	/*x 78 - getuid (not implemented on WIN32) */
#define SYS_times	25	/*X 43 - stime (Xtensa-specific implementation) */
#define SYS_socket      26
#define SYS_sendto      27
#define SYS_recvfrom    28
#define SYS_select_one  29      /* not compitible select, one file descriptor at the time */
#define SYS_bind        30
#define SYS_ioctl	31

/*
 * SYS_select_one specifiers
 */

#define  XTISS_SELECT_ONE_READ    1
#define  XTISS_SELECT_ONE_WRITE   2
#define  XTISS_SELECT_ONE_EXCEPT  3


#endif /* _XTENSA_PLATFORM_ISS_SIMCALL_H */

=======
#include <linux/bug.h>

#ifdef CONFIG_XTENSA_SIMCALL_ISS
#include <platform/simcall-iss.h>
#endif
#ifdef CONFIG_XTENSA_SIMCALL_GDBIO
#include <platform/simcall-gdbio.h>
#endif

static inline int simc_exit(int exit_code)
{
#ifdef SYS_exit
	return __simc(SYS_exit, exit_code, 0, 0);
#else
	WARN_ONCE(1, "%s: not implemented\n", __func__);
	return -1;
#endif
}

static inline int simc_open(const char *file, int flags, int mode)
{
	return __simc(SYS_open, (int) file, flags, mode);
}

static inline int simc_close(int fd)
{
	return __simc(SYS_close, fd, 0, 0);
}

static inline int simc_ioctl(int fd, int request, void *arg)
{
#ifdef SYS_ioctl
	return __simc(SYS_ioctl, fd, request, (int) arg);
#else
	WARN_ONCE(1, "%s: not implemented\n", __func__);
	return -1;
#endif
}

static inline int simc_read(int fd, void *buf, size_t count)
{
	return __simc(SYS_read, fd, (int) buf, count);
}

static inline int simc_write(int fd, const void *buf, size_t count)
{
	return __simc(SYS_write, fd, (int) buf, count);
}

static inline int simc_poll(int fd)
{
#ifdef SYS_select_one
	long timeval[2] = { 0, 0 };

	return __simc(SYS_select_one, fd, XTISS_SELECT_ONE_READ, (int)&timeval);
#else
	WARN_ONCE(1, "%s: not implemented\n", __func__);
	return -1;
#endif
}

static inline int simc_lseek(int fd, uint32_t off, int whence)
{
	return __simc(SYS_lseek, fd, off, whence);
}

static inline int simc_argc(void)
{
#ifdef SYS_iss_argc
	return __simc(SYS_iss_argc, 0, 0, 0);
#else
	WARN_ONCE(1, "%s: not implemented\n", __func__);
	return 0;
#endif
}

static inline int simc_argv_size(void)
{
#ifdef SYS_iss_argv_size
	return __simc(SYS_iss_argv_size, 0, 0, 0);
#else
	WARN_ONCE(1, "%s: not implemented\n", __func__);
	return 0;
#endif
}

static inline void simc_argv(void *buf)
{
#ifdef SYS_iss_set_argv
	__simc(SYS_iss_set_argv, (int)buf, 0, 0);
#else
	WARN_ONCE(1, "%s: not implemented\n", __func__);
#endif
}

#endif /* _XTENSA_PLATFORM_ISS_SIMCALL_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
