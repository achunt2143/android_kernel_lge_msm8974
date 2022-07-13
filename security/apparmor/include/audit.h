<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AppArmor security module
 *
 * This file contains AppArmor auditing function definitions.
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __AA_AUDIT_H
#define __AA_AUDIT_H

#include <linux/audit.h>
#include <linux/fs.h>
#include <linux/lsm_audit.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "file.h"
<<<<<<< HEAD

struct aa_profile;

extern const char *const audit_mode_names[];
#define AUDIT_MAX_INDEX 5

=======
#include "label.h"

extern const char *const audit_mode_names[];
#define AUDIT_MAX_INDEX 5
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum audit_mode {
	AUDIT_NORMAL,		/* follow normal auditing of accesses */
	AUDIT_QUIET_DENIED,	/* quiet all denied access messages */
	AUDIT_QUIET,		/* quiet all messages */
	AUDIT_NOQUIET,		/* do not quiet audit messages */
	AUDIT_ALL		/* audit all accesses */
};

enum audit_type {
	AUDIT_APPARMOR_AUDIT,
	AUDIT_APPARMOR_ALLOWED,
	AUDIT_APPARMOR_DENIED,
	AUDIT_APPARMOR_HINT,
	AUDIT_APPARMOR_STATUS,
	AUDIT_APPARMOR_ERROR,
	AUDIT_APPARMOR_KILL,
	AUDIT_APPARMOR_AUTO
};

<<<<<<< HEAD
extern const char *const op_table[];
enum aa_ops {
	OP_NULL,

	OP_SYSCTL,
	OP_CAPABLE,

	OP_UNLINK,
	OP_MKDIR,
	OP_RMDIR,
	OP_MKNOD,
	OP_TRUNC,
	OP_LINK,
	OP_SYMLINK,
	OP_RENAME_SRC,
	OP_RENAME_DEST,
	OP_CHMOD,
	OP_CHOWN,
	OP_GETATTR,
	OP_OPEN,

	OP_FPERM,
	OP_FLOCK,
	OP_FMMAP,
	OP_FMPROT,

	OP_CREATE,
	OP_POST_CREATE,
	OP_BIND,
	OP_CONNECT,
	OP_LISTEN,
	OP_ACCEPT,
	OP_SENDMSG,
	OP_RECVMSG,
	OP_GETSOCKNAME,
	OP_GETPEERNAME,
	OP_GETSOCKOPT,
	OP_SETSOCKOPT,
	OP_SOCK_SHUTDOWN,

	OP_PTRACE,

	OP_EXEC,
	OP_CHANGE_HAT,
	OP_CHANGE_PROFILE,
	OP_CHANGE_ONEXEC,

	OP_SETPROCATTR,
	OP_SETRLIMIT,

	OP_PROF_REPL,
	OP_PROF_LOAD,
	OP_PROF_RM,
};


struct apparmor_audit_data {
	int error;
	int op;
	int type;
	void *profile;
	const char *name;
	const char *info;
	union {
		void *target;
		struct {
			long pos;
			void *target;
		} iface;
		struct {
			int rlim;
			unsigned long max;
		} rlim;
		struct {
			const char *target;
			u32 request;
			u32 denied;
			uid_t ouid;
		} fs;
	};
};

/* define a short hand for apparmor_audit_data structure */
#define aad apparmor_audit_data

void aa_audit_msg(int type, struct common_audit_data *sa,
		  void (*cb) (struct audit_buffer *, void *));
int aa_audit(int type, struct aa_profile *profile, gfp_t gfp,
	     struct common_audit_data *sa,
	     void (*cb) (struct audit_buffer *, void *));

=======
#define OP_NULL NULL

#define OP_SYSCTL "sysctl"
#define OP_CAPABLE "capable"

#define OP_UNLINK "unlink"
#define OP_MKDIR "mkdir"
#define OP_RMDIR "rmdir"
#define OP_MKNOD "mknod"
#define OP_TRUNC "truncate"
#define OP_LINK "link"
#define OP_SYMLINK "symlink"
#define OP_RENAME_SRC "rename_src"
#define OP_RENAME_DEST "rename_dest"
#define OP_CHMOD "chmod"
#define OP_CHOWN "chown"
#define OP_GETATTR "getattr"
#define OP_OPEN "open"

#define OP_FRECEIVE "file_receive"
#define OP_FPERM "file_perm"
#define OP_FLOCK "file_lock"
#define OP_FMMAP "file_mmap"
#define OP_FMPROT "file_mprotect"
#define OP_INHERIT "file_inherit"

#define OP_PIVOTROOT "pivotroot"
#define OP_MOUNT "mount"
#define OP_UMOUNT "umount"

#define OP_CREATE "create"
#define OP_POST_CREATE "post_create"
#define OP_BIND "bind"
#define OP_CONNECT "connect"
#define OP_LISTEN "listen"
#define OP_ACCEPT "accept"
#define OP_SENDMSG "sendmsg"
#define OP_RECVMSG "recvmsg"
#define OP_GETSOCKNAME "getsockname"
#define OP_GETPEERNAME "getpeername"
#define OP_GETSOCKOPT "getsockopt"
#define OP_SETSOCKOPT "setsockopt"
#define OP_SHUTDOWN "socket_shutdown"

#define OP_PTRACE "ptrace"
#define OP_SIGNAL "signal"

#define OP_EXEC "exec"

#define OP_CHANGE_HAT "change_hat"
#define OP_CHANGE_PROFILE "change_profile"
#define OP_CHANGE_ONEXEC "change_onexec"
#define OP_STACK "stack"
#define OP_STACK_ONEXEC "stack_onexec"

#define OP_SETPROCATTR "setprocattr"
#define OP_SETRLIMIT "setrlimit"

#define OP_PROF_REPL "profile_replace"
#define OP_PROF_LOAD "profile_load"
#define OP_PROF_RM "profile_remove"

#define OP_USERNS_CREATE "userns_create"

#define OP_URING_OVERRIDE "uring_override"
#define OP_URING_SQPOLL "uring_sqpoll"

struct apparmor_audit_data {
	int error;
	int type;
	u16 class;
	const char *op;
	const struct cred *subj_cred;
	struct aa_label *subj_label;
	const char *name;
	const char *info;
	u32 request;
	u32 denied;
	union {
		/* these entries require a custom callback fn */
		struct {
			struct aa_label *peer;
			union {
				struct {
					const char *target;
					kuid_t ouid;
				} fs;
				struct {
					int rlim;
					unsigned long max;
				} rlim;
				struct {
					int signal;
					int unmappedsig;
				};
				struct {
					int type, protocol;
					struct sock *peer_sk;
					void *addr;
					int addrlen;
				} net;
			};
		};
		struct {
			struct aa_profile *profile;
			const char *ns;
			long pos;
		} iface;
		struct {
			const char *src_name;
			const char *type;
			const char *trans;
			const char *data;
			unsigned long flags;
		} mnt;
		struct {
			struct aa_label *target;
		} uring;
	};

	struct common_audit_data common;
};

/* macros for dealing with  apparmor_audit_data structure */
#define aad(SA) (container_of(SA, struct apparmor_audit_data, common))
#define aad_of_va(VA) aad((struct common_audit_data *)(VA))

#define DEFINE_AUDIT_DATA(NAME, T, C, X)				\
	/* TODO: cleanup audit init so we don't need _aad = {0,} */	\
	struct apparmor_audit_data NAME = {				\
		.class = (C),						\
		.op = (X),                                              \
		.common.type = (T),					\
		.common.u.tsk = NULL,					\
		.common.apparmor_audit_data = &NAME,			\
	};

void aa_audit_msg(int type, struct apparmor_audit_data *ad,
		  void (*cb) (struct audit_buffer *, void *));
int aa_audit(int type, struct aa_profile *profile,
	     struct apparmor_audit_data *ad,
	     void (*cb) (struct audit_buffer *, void *));

#define aa_audit_error(ERROR, AD, CB)				\
({								\
	(AD)->error = (ERROR);					\
	aa_audit_msg(AUDIT_APPARMOR_ERROR, (AD), (CB));		\
	(AD)->error;					\
})


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int complain_error(int error)
{
	if (error == -EPERM || error == -EACCES)
		return 0;
	return error;
}

<<<<<<< HEAD
=======
void aa_audit_rule_free(void *vrule);
int aa_audit_rule_init(u32 field, u32 op, char *rulestr, void **vrule);
int aa_audit_rule_known(struct audit_krule *rule);
int aa_audit_rule_match(u32 sid, u32 field, u32 op, void *vrule);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __AA_AUDIT_H */
