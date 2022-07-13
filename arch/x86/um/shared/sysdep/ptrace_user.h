<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <generated/user_constants.h>

#define PT_OFFSET(r) ((r) * sizeof(long))

#define PT_SYSCALL_NR(regs) ((regs)[HOST_ORIG_AX])
#define PT_SYSCALL_NR_OFFSET PT_OFFSET(HOST_ORIG_AX)

#define PT_SYSCALL_RET_OFFSET PT_OFFSET(HOST_AX)

#define REGS_IP_INDEX HOST_IP
#define REGS_SP_INDEX HOST_SP

#ifdef __i386__
#define FP_SIZE ((HOST_FPX_SIZE > HOST_FP_SIZE) ? HOST_FPX_SIZE : HOST_FP_SIZE)
#else
#define FP_SIZE HOST_FP_SIZE
<<<<<<< HEAD

/*
 * x86_64 FC3 doesn't define this in /usr/include/linux/ptrace.h even though
 * it's defined in the kernel's include/linux/ptrace.h. Additionally, use the
 * 2.4 name and value for 2.4 host compatibility.
 */
#ifndef PTRACE_OLDSETOPTIONS
#define PTRACE_OLDSETOPTIONS 21
#endif

=======
#endif

/*
 * glibc before 2.27 does not include PTRACE_SYSEMU_SINGLESTEP in its enum,
 * ensure we have a definition by (re-)defining it here.
 */
#ifndef PTRACE_SYSEMU_SINGLESTEP
#define PTRACE_SYSEMU_SINGLESTEP 32
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
