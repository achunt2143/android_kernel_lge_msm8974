<<<<<<< HEAD
#ifndef _ASM_ARM_SECCOMP_H
#define _ASM_ARM_SECCOMP_H

#include <linux/unistd.h>

#define __NR_seccomp_read __NR_read
#define __NR_seccomp_write __NR_write
#define __NR_seccomp_exit __NR_exit
#define __NR_seccomp_sigreturn __NR_rt_sigreturn

#endif /* _ASM_ARM_SECCOMP_H */
=======
/* SPDX-License-Identifier: GPL-2.0-only */
#ifndef _ASM_SECCOMP_H
#define _ASM_SECCOMP_H

#include <asm-generic/seccomp.h>

#define SECCOMP_ARCH_NATIVE		AUDIT_ARCH_ARM
#define SECCOMP_ARCH_NATIVE_NR		NR_syscalls
#define SECCOMP_ARCH_NATIVE_NAME	"arm"

#endif /* _ASM_SECCOMP_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
