<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _NAMESPACE_H_
#define _NAMESPACE_H_
#ifdef __KERNEL__

struct mnt_namespace;
struct fs_struct;
struct user_namespace;
<<<<<<< HEAD
=======
struct ns_common;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern struct mnt_namespace *copy_mnt_ns(unsigned long, struct mnt_namespace *,
		struct user_namespace *, struct fs_struct *);
extern void put_mnt_ns(struct mnt_namespace *ns);
<<<<<<< HEAD
=======
extern struct ns_common *from_mnt_ns(struct mnt_namespace *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern const struct file_operations proc_mounts_operations;
extern const struct file_operations proc_mountinfo_operations;
extern const struct file_operations proc_mountstats_operations;

#endif
#endif
