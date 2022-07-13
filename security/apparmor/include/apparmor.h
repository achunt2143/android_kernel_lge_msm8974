<<<<<<< HEAD
/*
 * AppArmor security module
 *
 * This file contains AppArmor basic global and lib definitions
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2010 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation, version 2 of the
 * License.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * AppArmor security module
 *
 * This file contains AppArmor basic global
 *
 * Copyright (C) 1998-2008 Novell/SUSE
 * Copyright 2009-2017 Canonical Ltd.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __APPARMOR_H
#define __APPARMOR_H

<<<<<<< HEAD
#include <linux/fs.h>

#include "match.h"
=======
#include <linux/types.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Class of mediation types in the AppArmor policy db
 */
<<<<<<< HEAD
#define AA_CLASS_ENTRY		0
#define AA_CLASS_UNKNOWN	1
#define AA_CLASS_FILE		2
#define AA_CLASS_CAP		3
#define AA_CLASS_NET		4
#define AA_CLASS_RLIMITS	5
#define AA_CLASS_DOMAIN		6

#define AA_CLASS_LAST		AA_CLASS_DOMAIN
=======
#define AA_CLASS_NONE		0
#define AA_CLASS_UNKNOWN	1
#define AA_CLASS_FILE		2
#define AA_CLASS_CAP		3
#define AA_CLASS_DEPRECATED	4
#define AA_CLASS_RLIMITS	5
#define AA_CLASS_DOMAIN		6
#define AA_CLASS_MOUNT		7
#define AA_CLASS_PTRACE		9
#define AA_CLASS_SIGNAL		10
#define AA_CLASS_XMATCH		11
#define AA_CLASS_NET		14
#define AA_CLASS_LABEL		16
#define AA_CLASS_POSIX_MQUEUE	17
#define AA_CLASS_MODULE		19
#define AA_CLASS_DISPLAY_LSM	20
#define AA_CLASS_NS		21
#define AA_CLASS_IO_URING	22

#define AA_CLASS_X		31
#define AA_CLASS_DBUS		32

#define AA_CLASS_LAST		AA_CLASS_DBUS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Control parameters settable through module/boot flags */
extern enum audit_mode aa_g_audit;
extern bool aa_g_audit_header;
extern bool aa_g_debug;
<<<<<<< HEAD
=======
extern bool aa_g_hash_policy;
extern bool aa_g_export_binary;
extern int aa_g_rawdata_compression_level;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern bool aa_g_lock_policy;
extern bool aa_g_logsyscall;
extern bool aa_g_paranoid_load;
extern unsigned int aa_g_path_max;

<<<<<<< HEAD
/*
 * DEBUG remains global (no per profile flag) since it is mostly used in sysctl
 * which is not related to profile accesses.
 */

#define AA_DEBUG(fmt, args...)						\
	do {								\
		if (aa_g_debug && printk_ratelimit())			\
			printk(KERN_DEBUG "AppArmor: " fmt, ##args);	\
	} while (0)

#define AA_ERROR(fmt, args...)						\
	do {								\
		if (printk_ratelimit())					\
			printk(KERN_ERR "AppArmor: " fmt, ##args);	\
	} while (0)

/* Flag indicating whether initialization completed */
extern int apparmor_initialized __initdata;

/* fn's in lib */
char *aa_split_fqname(char *args, char **ns_name);
void aa_info_message(const char *str);
void *kvmalloc(size_t size);


/**
 * aa_strneq - compare null terminated @str to a non null terminated substring
 * @str: a null terminated string
 * @sub: a substring, not necessarily null terminated
 * @len: length of @sub to compare
 *
 * The @str string must be full consumed for this to be considered a match
 */
static inline bool aa_strneq(const char *str, const char *sub, int len)
{
	return !strncmp(str, sub, len) && !str[len];
}

/**
 * aa_dfa_null_transition - step to next state after null character
 * @dfa: the dfa to match against
 * @start: the state of the dfa to start matching in
 *
 * aa_dfa_null_transition transitions to the next state after a null
 * character which is not used in standard matching and is only
 * used to separate pairs.
 */
static inline unsigned int aa_dfa_null_transition(struct aa_dfa *dfa,
						  unsigned int start)
{
	/* the null transition only needs the string's null terminator byte */
	return aa_dfa_next(dfa, start, 0);
}

static inline bool mediated_filesystem(struct inode *inode)
{
	return !(inode->i_sb->s_flags & MS_NOUSER);
}
=======
#ifdef CONFIG_SECURITY_APPARMOR_EXPORT_BINARY
#define AA_MIN_CLEVEL zstd_min_clevel()
#define AA_MAX_CLEVEL zstd_max_clevel()
#define AA_DEFAULT_CLEVEL ZSTD_CLEVEL_DEFAULT
#else
#define AA_MIN_CLEVEL 0
#define AA_MAX_CLEVEL 0
#define AA_DEFAULT_CLEVEL 0
#endif /* CONFIG_SECURITY_APPARMOR_EXPORT_BINARY */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __APPARMOR_H */
