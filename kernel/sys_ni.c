<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/linkage.h>
#include <linux/errno.h>

#include <asm/unistd.h>

<<<<<<< HEAD
=======
#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
/* Architectures may override COND_SYSCALL and COND_SYSCALL_COMPAT */
#include <asm/syscall_wrapper.h>
#endif /* CONFIG_ARCH_HAS_SYSCALL_WRAPPER */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*  we can't #include <linux/syscalls.h> here,
    but tell gcc to not warn with -Wmissing-prototypes  */
asmlinkage long sys_ni_syscall(void);

/*
 * Non-implemented system calls get redirected here.
 */
asmlinkage long sys_ni_syscall(void)
{
	return -ENOSYS;
}

<<<<<<< HEAD
cond_syscall(sys_quotactl);
cond_syscall(sys32_quotactl);
cond_syscall(sys_acct);
cond_syscall(sys_lookup_dcookie);
cond_syscall(sys_swapon);
cond_syscall(sys_swapoff);
cond_syscall(sys_kexec_load);
cond_syscall(compat_sys_kexec_load);
cond_syscall(sys_init_module);
cond_syscall(sys_delete_module);
cond_syscall(sys_socketpair);
cond_syscall(sys_bind);
cond_syscall(sys_listen);
cond_syscall(sys_accept);
cond_syscall(sys_accept4);
cond_syscall(sys_connect);
cond_syscall(sys_getsockname);
cond_syscall(sys_getpeername);
cond_syscall(sys_sendto);
cond_syscall(sys_send);
cond_syscall(sys_recvfrom);
cond_syscall(sys_recv);
cond_syscall(sys_socket);
cond_syscall(sys_setsockopt);
cond_syscall(compat_sys_setsockopt);
cond_syscall(sys_getsockopt);
cond_syscall(compat_sys_getsockopt);
cond_syscall(sys_shutdown);
cond_syscall(sys_sendmsg);
cond_syscall(sys_sendmmsg);
cond_syscall(compat_sys_sendmsg);
cond_syscall(compat_sys_sendmmsg);
cond_syscall(sys_recvmsg);
cond_syscall(sys_recvmmsg);
cond_syscall(compat_sys_recvmsg);
cond_syscall(compat_sys_recv);
cond_syscall(compat_sys_recvfrom);
cond_syscall(compat_sys_recvmmsg);
cond_syscall(sys_socketcall);
cond_syscall(sys_futex);
cond_syscall(compat_sys_futex);
cond_syscall(sys_set_robust_list);
cond_syscall(compat_sys_set_robust_list);
cond_syscall(sys_get_robust_list);
cond_syscall(compat_sys_get_robust_list);
cond_syscall(sys_epoll_create);
cond_syscall(sys_epoll_create1);
cond_syscall(sys_epoll_ctl);
cond_syscall(sys_epoll_wait);
cond_syscall(sys_epoll_pwait);
cond_syscall(compat_sys_epoll_pwait);
cond_syscall(sys_semget);
cond_syscall(sys_semop);
cond_syscall(sys_semtimedop);
cond_syscall(compat_sys_semtimedop);
cond_syscall(sys_semctl);
cond_syscall(compat_sys_semctl);
cond_syscall(sys_msgget);
cond_syscall(sys_msgsnd);
cond_syscall(compat_sys_msgsnd);
cond_syscall(sys_msgrcv);
cond_syscall(compat_sys_msgrcv);
cond_syscall(sys_msgctl);
cond_syscall(compat_sys_msgctl);
cond_syscall(sys_shmget);
cond_syscall(sys_shmat);
cond_syscall(compat_sys_shmat);
cond_syscall(sys_shmdt);
cond_syscall(sys_shmctl);
cond_syscall(compat_sys_shmctl);
cond_syscall(sys_mq_open);
cond_syscall(sys_mq_unlink);
cond_syscall(sys_mq_timedsend);
cond_syscall(sys_mq_timedreceive);
cond_syscall(sys_mq_notify);
cond_syscall(sys_mq_getsetattr);
cond_syscall(compat_sys_mq_open);
cond_syscall(compat_sys_mq_timedsend);
cond_syscall(compat_sys_mq_timedreceive);
cond_syscall(compat_sys_mq_notify);
cond_syscall(compat_sys_mq_getsetattr);
cond_syscall(sys_mbind);
cond_syscall(sys_get_mempolicy);
cond_syscall(sys_set_mempolicy);
cond_syscall(compat_sys_mbind);
cond_syscall(compat_sys_get_mempolicy);
cond_syscall(compat_sys_set_mempolicy);
cond_syscall(sys_add_key);
cond_syscall(sys_request_key);
cond_syscall(sys_keyctl);
cond_syscall(compat_sys_keyctl);
cond_syscall(compat_sys_socketcall);
cond_syscall(sys_inotify_init);
cond_syscall(sys_inotify_init1);
cond_syscall(sys_inotify_add_watch);
cond_syscall(sys_inotify_rm_watch);
cond_syscall(sys_migrate_pages);
cond_syscall(sys_move_pages);
cond_syscall(sys_chown16);
cond_syscall(sys_fchown16);
cond_syscall(sys_getegid16);
cond_syscall(sys_geteuid16);
cond_syscall(sys_getgid16);
cond_syscall(sys_getgroups16);
cond_syscall(sys_getresgid16);
cond_syscall(sys_getresuid16);
cond_syscall(sys_getuid16);
cond_syscall(sys_lchown16);
cond_syscall(sys_setfsgid16);
cond_syscall(sys_setfsuid16);
cond_syscall(sys_setgid16);
cond_syscall(sys_setgroups16);
cond_syscall(sys_setregid16);
cond_syscall(sys_setresgid16);
cond_syscall(sys_setresuid16);
cond_syscall(sys_setreuid16);
cond_syscall(sys_setuid16);
cond_syscall(sys_vm86old);
cond_syscall(sys_vm86);
cond_syscall(sys_ipc);
cond_syscall(compat_sys_ipc);
cond_syscall(compat_sys_sysctl);
cond_syscall(sys_flock);
cond_syscall(sys_io_setup);
cond_syscall(sys_io_destroy);
cond_syscall(sys_io_submit);
cond_syscall(sys_io_cancel);
cond_syscall(sys_io_getevents);
cond_syscall(sys_syslog);
cond_syscall(sys_process_vm_readv);
cond_syscall(sys_process_vm_writev);
cond_syscall(compat_sys_process_vm_readv);
cond_syscall(compat_sys_process_vm_writev);

/* arch-specific weak syscall entries */
cond_syscall(sys_pciconfig_read);
cond_syscall(sys_pciconfig_write);
cond_syscall(sys_pciconfig_iobase);
cond_syscall(sys32_ipc);
cond_syscall(ppc_rtas);
cond_syscall(sys_spu_run);
cond_syscall(sys_spu_create);
cond_syscall(sys_subpage_prot);

/* mmu depending weak syscall entries */
cond_syscall(sys_mprotect);
cond_syscall(sys_msync);
cond_syscall(sys_mlock);
cond_syscall(sys_munlock);
cond_syscall(sys_mlockall);
cond_syscall(sys_munlockall);
cond_syscall(sys_mincore);
cond_syscall(sys_madvise);
cond_syscall(sys_mremap);
cond_syscall(sys_remap_file_pages);
cond_syscall(compat_sys_move_pages);
cond_syscall(compat_sys_migrate_pages);

/* block-layer dependent */
cond_syscall(sys_bdflush);
cond_syscall(sys_ioprio_set);
cond_syscall(sys_ioprio_get);

/* New file descriptors */
cond_syscall(sys_signalfd);
cond_syscall(sys_signalfd4);
cond_syscall(compat_sys_signalfd);
cond_syscall(compat_sys_signalfd4);
cond_syscall(sys_timerfd_create);
cond_syscall(sys_timerfd_settime);
cond_syscall(sys_timerfd_gettime);
cond_syscall(compat_sys_timerfd_settime);
cond_syscall(compat_sys_timerfd_gettime);
cond_syscall(sys_eventfd);
cond_syscall(sys_eventfd2);
cond_syscall(sys_memfd_create);

/* performance counters: */
cond_syscall(sys_perf_event_open);

/* fanotify! */
cond_syscall(sys_fanotify_init);
cond_syscall(sys_fanotify_mark);

/* open by handle */
cond_syscall(sys_name_to_handle_at);
cond_syscall(sys_open_by_handle_at);
cond_syscall(compat_sys_open_by_handle_at);

/* operate on Secure Computing state */
cond_syscall(sys_seccomp);
=======
#ifndef COND_SYSCALL
#define COND_SYSCALL(name) cond_syscall(sys_##name)
#endif /* COND_SYSCALL */

#ifndef COND_SYSCALL_COMPAT
#define COND_SYSCALL_COMPAT(name) cond_syscall(compat_sys_##name)
#endif /* COND_SYSCALL_COMPAT */

/*
 * This list is kept in the same order as include/uapi/asm-generic/unistd.h.
 * Architecture specific entries go below, followed by deprecated or obsolete
 * system calls.
 */

COND_SYSCALL(io_setup);
COND_SYSCALL_COMPAT(io_setup);
COND_SYSCALL(io_destroy);
COND_SYSCALL(io_submit);
COND_SYSCALL_COMPAT(io_submit);
COND_SYSCALL(io_cancel);
COND_SYSCALL(io_getevents_time32);
COND_SYSCALL(io_getevents);
COND_SYSCALL(io_pgetevents_time32);
COND_SYSCALL(io_pgetevents);
COND_SYSCALL_COMPAT(io_pgetevents_time32);
COND_SYSCALL_COMPAT(io_pgetevents);
COND_SYSCALL(io_uring_setup);
COND_SYSCALL(io_uring_enter);
COND_SYSCALL(io_uring_register);
COND_SYSCALL(eventfd2);
COND_SYSCALL(epoll_create1);
COND_SYSCALL(epoll_ctl);
COND_SYSCALL(epoll_pwait);
COND_SYSCALL_COMPAT(epoll_pwait);
COND_SYSCALL(epoll_pwait2);
COND_SYSCALL_COMPAT(epoll_pwait2);
COND_SYSCALL(inotify_init1);
COND_SYSCALL(inotify_add_watch);
COND_SYSCALL(inotify_rm_watch);
COND_SYSCALL(ioprio_set);
COND_SYSCALL(ioprio_get);
COND_SYSCALL(flock);
COND_SYSCALL(quotactl);
COND_SYSCALL(quotactl_fd);
COND_SYSCALL(signalfd4);
COND_SYSCALL_COMPAT(signalfd4);
COND_SYSCALL(timerfd_create);
COND_SYSCALL(timerfd_settime);
COND_SYSCALL(timerfd_settime32);
COND_SYSCALL(timerfd_gettime);
COND_SYSCALL(timerfd_gettime32);
COND_SYSCALL(acct);
COND_SYSCALL(capget);
COND_SYSCALL(capset);
/* __ARCH_WANT_SYS_CLONE3 */
COND_SYSCALL(clone3);
COND_SYSCALL(futex);
COND_SYSCALL(futex_time32);
COND_SYSCALL(set_robust_list);
COND_SYSCALL_COMPAT(set_robust_list);
COND_SYSCALL(get_robust_list);
COND_SYSCALL_COMPAT(get_robust_list);
COND_SYSCALL(futex_waitv);
COND_SYSCALL(futex_wake);
COND_SYSCALL(futex_wait);
COND_SYSCALL(futex_requeue);
COND_SYSCALL(kexec_load);
COND_SYSCALL_COMPAT(kexec_load);
COND_SYSCALL(init_module);
COND_SYSCALL(delete_module);
COND_SYSCALL(syslog);
COND_SYSCALL(setregid);
COND_SYSCALL(setgid);
COND_SYSCALL(setreuid);
COND_SYSCALL(setuid);
COND_SYSCALL(setresuid);
COND_SYSCALL(getresuid);
COND_SYSCALL(setresgid);
COND_SYSCALL(getresgid);
COND_SYSCALL(setfsuid);
COND_SYSCALL(setfsgid);
COND_SYSCALL(setgroups);
COND_SYSCALL(getgroups);
COND_SYSCALL(mq_open);
COND_SYSCALL_COMPAT(mq_open);
COND_SYSCALL(mq_unlink);
COND_SYSCALL(mq_timedsend);
COND_SYSCALL(mq_timedsend_time32);
COND_SYSCALL(mq_timedreceive);
COND_SYSCALL(mq_timedreceive_time32);
COND_SYSCALL(mq_notify);
COND_SYSCALL_COMPAT(mq_notify);
COND_SYSCALL(mq_getsetattr);
COND_SYSCALL_COMPAT(mq_getsetattr);
COND_SYSCALL(msgget);
COND_SYSCALL(old_msgctl);
COND_SYSCALL(msgctl);
COND_SYSCALL_COMPAT(msgctl);
COND_SYSCALL_COMPAT(old_msgctl);
COND_SYSCALL(msgrcv);
COND_SYSCALL_COMPAT(msgrcv);
COND_SYSCALL(msgsnd);
COND_SYSCALL_COMPAT(msgsnd);
COND_SYSCALL(semget);
COND_SYSCALL(old_semctl);
COND_SYSCALL(semctl);
COND_SYSCALL_COMPAT(semctl);
COND_SYSCALL_COMPAT(old_semctl);
COND_SYSCALL(semtimedop);
COND_SYSCALL(semtimedop_time32);
COND_SYSCALL(semop);
COND_SYSCALL(shmget);
COND_SYSCALL(old_shmctl);
COND_SYSCALL(shmctl);
COND_SYSCALL_COMPAT(shmctl);
COND_SYSCALL_COMPAT(old_shmctl);
COND_SYSCALL(shmat);
COND_SYSCALL_COMPAT(shmat);
COND_SYSCALL(shmdt);
COND_SYSCALL(socket);
COND_SYSCALL(socketpair);
COND_SYSCALL(bind);
COND_SYSCALL(listen);
COND_SYSCALL(accept);
COND_SYSCALL(connect);
COND_SYSCALL(getsockname);
COND_SYSCALL(getpeername);
COND_SYSCALL(setsockopt);
COND_SYSCALL_COMPAT(setsockopt);
COND_SYSCALL(getsockopt);
COND_SYSCALL_COMPAT(getsockopt);
COND_SYSCALL(sendto);
COND_SYSCALL(shutdown);
COND_SYSCALL(recvfrom);
COND_SYSCALL_COMPAT(recvfrom);
COND_SYSCALL(sendmsg);
COND_SYSCALL_COMPAT(sendmsg);
COND_SYSCALL(recvmsg);
COND_SYSCALL_COMPAT(recvmsg);
COND_SYSCALL(mremap);
COND_SYSCALL(add_key);
COND_SYSCALL(request_key);
COND_SYSCALL(keyctl);
COND_SYSCALL_COMPAT(keyctl);
COND_SYSCALL(landlock_create_ruleset);
COND_SYSCALL(landlock_add_rule);
COND_SYSCALL(landlock_restrict_self);
COND_SYSCALL(fadvise64_64);
COND_SYSCALL_COMPAT(fadvise64_64);
COND_SYSCALL(lsm_get_self_attr);
COND_SYSCALL(lsm_set_self_attr);
COND_SYSCALL(lsm_list_modules);

/* CONFIG_MMU only */
COND_SYSCALL(swapon);
COND_SYSCALL(swapoff);
COND_SYSCALL(mprotect);
COND_SYSCALL(msync);
COND_SYSCALL(mlock);
COND_SYSCALL(munlock);
COND_SYSCALL(mlockall);
COND_SYSCALL(munlockall);
COND_SYSCALL(mincore);
COND_SYSCALL(madvise);
COND_SYSCALL(process_madvise);
COND_SYSCALL(process_mrelease);
COND_SYSCALL(remap_file_pages);
COND_SYSCALL(mbind);
COND_SYSCALL(get_mempolicy);
COND_SYSCALL(set_mempolicy);
COND_SYSCALL(migrate_pages);
COND_SYSCALL(move_pages);
COND_SYSCALL(set_mempolicy_home_node);
COND_SYSCALL(cachestat);

COND_SYSCALL(perf_event_open);
COND_SYSCALL(accept4);
COND_SYSCALL(recvmmsg);
COND_SYSCALL(recvmmsg_time32);
COND_SYSCALL_COMPAT(recvmmsg_time32);
COND_SYSCALL_COMPAT(recvmmsg_time64);

/* Posix timer syscalls may be configured out */
COND_SYSCALL(timer_create);
COND_SYSCALL(timer_gettime);
COND_SYSCALL(timer_getoverrun);
COND_SYSCALL(timer_settime);
COND_SYSCALL(timer_delete);
COND_SYSCALL(clock_adjtime);
COND_SYSCALL(getitimer);
COND_SYSCALL(setitimer);
COND_SYSCALL(alarm);
COND_SYSCALL_COMPAT(timer_create);
COND_SYSCALL_COMPAT(getitimer);
COND_SYSCALL_COMPAT(setitimer);

/*
 * Architecture specific syscalls: see further below
 */

/* fanotify */
COND_SYSCALL(fanotify_init);
COND_SYSCALL(fanotify_mark);

/* open by handle */
COND_SYSCALL(name_to_handle_at);
COND_SYSCALL(open_by_handle_at);
COND_SYSCALL_COMPAT(open_by_handle_at);

COND_SYSCALL(sendmmsg);
COND_SYSCALL_COMPAT(sendmmsg);
COND_SYSCALL(process_vm_readv);
COND_SYSCALL_COMPAT(process_vm_readv);
COND_SYSCALL(process_vm_writev);
COND_SYSCALL_COMPAT(process_vm_writev);

/* compare kernel pointers */
COND_SYSCALL(kcmp);

COND_SYSCALL(finit_module);

/* operate on Secure Computing state */
COND_SYSCALL(seccomp);

COND_SYSCALL(memfd_create);

/* access BPF programs and maps */
COND_SYSCALL(bpf);

/* execveat */
COND_SYSCALL(execveat);

COND_SYSCALL(userfaultfd);

/* membarrier */
COND_SYSCALL(membarrier);

COND_SYSCALL(mlock2);

COND_SYSCALL(copy_file_range);

/* memory protection keys */
COND_SYSCALL(pkey_mprotect);
COND_SYSCALL(pkey_alloc);
COND_SYSCALL(pkey_free);

/* memfd_secret */
COND_SYSCALL(memfd_secret);

/*
 * Architecture specific weak syscall entries.
 */

/* pciconfig: alpha, arm, arm64, ia64, sparc */
COND_SYSCALL(pciconfig_read);
COND_SYSCALL(pciconfig_write);
COND_SYSCALL(pciconfig_iobase);

/* sys_socketcall: arm, mips, x86, ... */
COND_SYSCALL(socketcall);
COND_SYSCALL_COMPAT(socketcall);

/* compat syscalls for arm64, x86, ... */
COND_SYSCALL_COMPAT(fanotify_mark);

/* x86 */
COND_SYSCALL(vm86old);
COND_SYSCALL(modify_ldt);
COND_SYSCALL(vm86);
COND_SYSCALL(kexec_file_load);
COND_SYSCALL(map_shadow_stack);

/* s390 */
COND_SYSCALL(s390_pci_mmio_read);
COND_SYSCALL(s390_pci_mmio_write);
COND_SYSCALL(s390_ipc);
COND_SYSCALL_COMPAT(s390_ipc);

/* powerpc */
COND_SYSCALL(rtas);
COND_SYSCALL(spu_run);
COND_SYSCALL(spu_create);
COND_SYSCALL(subpage_prot);


/*
 * Deprecated system calls which are still defined in
 * include/uapi/asm-generic/unistd.h and wanted by >= 1 arch
 */

/* __ARCH_WANT_SYSCALL_NO_FLAGS */
COND_SYSCALL(epoll_create);
COND_SYSCALL(inotify_init);
COND_SYSCALL(eventfd);
COND_SYSCALL(signalfd);
COND_SYSCALL_COMPAT(signalfd);

/* __ARCH_WANT_SYSCALL_OFF_T */
COND_SYSCALL(fadvise64);

/* __ARCH_WANT_SYSCALL_DEPRECATED */
COND_SYSCALL(epoll_wait);
COND_SYSCALL(recv);
COND_SYSCALL_COMPAT(recv);
COND_SYSCALL(send);
COND_SYSCALL(uselib);

/* optional: time32 */
COND_SYSCALL(time32);
COND_SYSCALL(stime32);
COND_SYSCALL(utime32);
COND_SYSCALL(adjtimex_time32);
COND_SYSCALL(sched_rr_get_interval_time32);
COND_SYSCALL(nanosleep_time32);
COND_SYSCALL(rt_sigtimedwait_time32);
COND_SYSCALL_COMPAT(rt_sigtimedwait_time32);
COND_SYSCALL(timer_settime32);
COND_SYSCALL(timer_gettime32);
COND_SYSCALL(clock_settime32);
COND_SYSCALL(clock_gettime32);
COND_SYSCALL(clock_getres_time32);
COND_SYSCALL(clock_nanosleep_time32);
COND_SYSCALL(utimes_time32);
COND_SYSCALL(futimesat_time32);
COND_SYSCALL(pselect6_time32);
COND_SYSCALL_COMPAT(pselect6_time32);
COND_SYSCALL(ppoll_time32);
COND_SYSCALL_COMPAT(ppoll_time32);
COND_SYSCALL(utimensat_time32);
COND_SYSCALL(clock_adjtime32);

/*
 * The syscalls below are not found in include/uapi/asm-generic/unistd.h
 */

/* obsolete: SGETMASK_SYSCALL */
COND_SYSCALL(sgetmask);
COND_SYSCALL(ssetmask);

/* obsolete: SYSFS_SYSCALL */
COND_SYSCALL(sysfs);

/* obsolete: __ARCH_WANT_SYS_IPC */
COND_SYSCALL(ipc);
COND_SYSCALL_COMPAT(ipc);

/* obsolete: UID16 */
COND_SYSCALL(chown16);
COND_SYSCALL(fchown16);
COND_SYSCALL(getegid16);
COND_SYSCALL(geteuid16);
COND_SYSCALL(getgid16);
COND_SYSCALL(getgroups16);
COND_SYSCALL(getresgid16);
COND_SYSCALL(getresuid16);
COND_SYSCALL(getuid16);
COND_SYSCALL(lchown16);
COND_SYSCALL(setfsgid16);
COND_SYSCALL(setfsuid16);
COND_SYSCALL(setgid16);
COND_SYSCALL(setgroups16);
COND_SYSCALL(setregid16);
COND_SYSCALL(setresgid16);
COND_SYSCALL(setresuid16);
COND_SYSCALL(setreuid16);
COND_SYSCALL(setuid16);

/* restartable sequence */
COND_SYSCALL(rseq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
