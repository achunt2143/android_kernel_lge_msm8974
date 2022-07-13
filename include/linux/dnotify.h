<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_DNOTIFY_H
#define _LINUX_DNOTIFY_H
/*
 * Directory notification for Linux
 *
 * Copyright (C) 2000,2002 Stephen Rothwell
 */

#include <linux/fs.h>

struct dnotify_struct {
	struct dnotify_struct *	dn_next;
	__u32			dn_mask;
	int			dn_fd;
	struct file *		dn_filp;
	fl_owner_t		dn_owner;
};

#ifdef __KERNEL__


#ifdef CONFIG_DNOTIFY

#define DNOTIFY_ALL_EVENTS (FS_DELETE | FS_DELETE_CHILD |\
			    FS_MODIFY | FS_MODIFY_CHILD |\
			    FS_ACCESS | FS_ACCESS_CHILD |\
			    FS_ATTRIB | FS_ATTRIB_CHILD |\
<<<<<<< HEAD
			    FS_CREATE | FS_DN_RENAME |\
			    FS_MOVED_FROM | FS_MOVED_TO)

extern int dir_notify_enable;
extern void dnotify_flush(struct file *, fl_owner_t);
extern int fcntl_dirnotify(int, struct file *, unsigned long);
=======
			    FS_CREATE | FS_RENAME |\
			    FS_MOVED_FROM | FS_MOVED_TO)

extern void dnotify_flush(struct file *, fl_owner_t);
extern int fcntl_dirnotify(int, struct file *, unsigned int);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else

static inline void dnotify_flush(struct file *filp, fl_owner_t id)
{
}

<<<<<<< HEAD
static inline int fcntl_dirnotify(int fd, struct file *filp, unsigned long arg)
=======
static inline int fcntl_dirnotify(int fd, struct file *filp, unsigned int arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -EINVAL;
}

#endif /* CONFIG_DNOTIFY */

#endif /* __KERNEL __ */

#endif /* _LINUX_DNOTIFY_H */
