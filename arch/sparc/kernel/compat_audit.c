<<<<<<< HEAD
#define __32bit_syscall_numbers__
#include <asm/unistd.h>

unsigned sparc32_dir_class[] = {
=======
// SPDX-License-Identifier: GPL-2.0
#define __32bit_syscall_numbers__
#include <linux/audit_arch.h>
#include <asm/unistd.h>
#include "kernel.h"

unsigned int sparc32_dir_class[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/audit_dir_write.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_chattr_class[] = {
=======
unsigned int sparc32_chattr_class[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/audit_change_attr.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_write_class[] = {
=======
unsigned int sparc32_write_class[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/audit_write.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_read_class[] = {
=======
unsigned int sparc32_read_class[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/audit_read.h>
~0U
};

<<<<<<< HEAD
unsigned sparc32_signal_class[] = {
=======
unsigned int sparc32_signal_class[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/audit_signal.h>
~0U
};

<<<<<<< HEAD
int sparc32_classify_syscall(unsigned syscall)
{
	switch(syscall) {
	case __NR_open:
		return 2;
	case __NR_openat:
		return 3;
	case __NR_socketcall:
		return 4;
	case __NR_execve:
		return 5;
	default:
		return 1;
=======
int sparc32_classify_syscall(unsigned int syscall)
{
	switch(syscall) {
	case __NR_open:
		return AUDITSC_OPEN;
	case __NR_openat:
		return AUDITSC_OPENAT;
	case __NR_socketcall:
		return AUDITSC_SOCKETCALL;
	case __NR_execve:
		return AUDITSC_EXECVE;
	case __NR_openat2:
		return AUDITSC_OPENAT2;
	default:
		return AUDITSC_COMPAT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
