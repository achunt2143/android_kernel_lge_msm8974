<<<<<<< HEAD
#ifndef _LINUX_FANOTIFY_H
#define _LINUX_FANOTIFY_H

#include <linux/types.h>

/* the following events that user-space can register for */
#define FAN_ACCESS		0x00000001	/* File was accessed */
#define FAN_MODIFY		0x00000002	/* File was modified */
#define FAN_CLOSE_WRITE		0x00000008	/* Writtable file closed */
#define FAN_CLOSE_NOWRITE	0x00000010	/* Unwrittable file closed */
#define FAN_OPEN		0x00000020	/* File was opened */

#define FAN_Q_OVERFLOW		0x00004000	/* Event queued overflowed */

#define FAN_OPEN_PERM		0x00010000	/* File open in perm check */
#define FAN_ACCESS_PERM		0x00020000	/* File accessed in perm check */

#define FAN_ONDIR		0x40000000	/* event occurred against dir */

#define FAN_EVENT_ON_CHILD	0x08000000	/* interested in child events */

/* helper events */
#define FAN_CLOSE		(FAN_CLOSE_WRITE | FAN_CLOSE_NOWRITE) /* close */

/* flags used for fanotify_init() */
#define FAN_CLOEXEC		0x00000001
#define FAN_NONBLOCK		0x00000002

/* These are NOT bitwise flags.  Both bits are used togther.  */
#define FAN_CLASS_NOTIF		0x00000000
#define FAN_CLASS_CONTENT	0x00000004
#define FAN_CLASS_PRE_CONTENT	0x00000008
#define FAN_ALL_CLASS_BITS	(FAN_CLASS_NOTIF | FAN_CLASS_CONTENT | \
				 FAN_CLASS_PRE_CONTENT)

#define FAN_UNLIMITED_QUEUE	0x00000010
#define FAN_UNLIMITED_MARKS	0x00000020

#define FAN_ALL_INIT_FLAGS	(FAN_CLOEXEC | FAN_NONBLOCK | \
				 FAN_ALL_CLASS_BITS | FAN_UNLIMITED_QUEUE |\
				 FAN_UNLIMITED_MARKS)

/* flags used for fanotify_modify_mark() */
#define FAN_MARK_ADD		0x00000001
#define FAN_MARK_REMOVE		0x00000002
#define FAN_MARK_DONT_FOLLOW	0x00000004
#define FAN_MARK_ONLYDIR	0x00000008
#define FAN_MARK_MOUNT		0x00000010
#define FAN_MARK_IGNORED_MASK	0x00000020
#define FAN_MARK_IGNORED_SURV_MODIFY	0x00000040
#define FAN_MARK_FLUSH		0x00000080
#ifdef __KERNEL__
/* not valid from userspace, only kernel internal */
#define FAN_MARK_ONDIR		0x00000100
#endif

#define FAN_ALL_MARK_FLAGS	(FAN_MARK_ADD |\
				 FAN_MARK_REMOVE |\
				 FAN_MARK_DONT_FOLLOW |\
				 FAN_MARK_ONLYDIR |\
				 FAN_MARK_MOUNT |\
				 FAN_MARK_IGNORED_MASK |\
				 FAN_MARK_IGNORED_SURV_MODIFY |\
				 FAN_MARK_FLUSH)

/*
 * All of the events - we build the list by hand so that we can add flags in
 * the future and not break backward compatibility.  Apps will get only the
 * events that they originally wanted.  Be sure to add new events here!
 */
#define FAN_ALL_EVENTS (FAN_ACCESS |\
			FAN_MODIFY |\
			FAN_CLOSE |\
			FAN_OPEN)

/*
 * All events which require a permission response from userspace
 */
#define FAN_ALL_PERM_EVENTS (FAN_OPEN_PERM |\
			     FAN_ACCESS_PERM)

#define FAN_ALL_OUTGOING_EVENTS	(FAN_ALL_EVENTS |\
				 FAN_ALL_PERM_EVENTS |\
				 FAN_Q_OVERFLOW)

#define FANOTIFY_METADATA_VERSION	3

struct fanotify_event_metadata {
	__u32 event_len;
	__u8 vers;
	__u8 reserved;
	__u16 metadata_len;
	__aligned_u64 mask;
	__s32 fd;
	__s32 pid;
};

struct fanotify_response {
	__s32 fd;
	__u32 response;
};

/* Legit userspace responses to a _PERM event */
#define FAN_ALLOW	0x01
#define FAN_DENY	0x02
/* No fd set in event */
#define FAN_NOFD	-1

/* Helper functions to deal with fanotify_event_metadata buffers */
#define FAN_EVENT_METADATA_LEN (sizeof(struct fanotify_event_metadata))

#define FAN_EVENT_NEXT(meta, len) ((len) -= (meta)->event_len, \
				   (struct fanotify_event_metadata*)(((char *)(meta)) + \
				   (meta)->event_len))

#define FAN_EVENT_OK(meta, len)	((long)(len) >= (long)FAN_EVENT_METADATA_LEN && \
				(long)(meta)->event_len >= (long)FAN_EVENT_METADATA_LEN && \
				(long)(meta)->event_len <= (long)(len))
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_FANOTIFY_H
#define _LINUX_FANOTIFY_H

#include <linux/sysctl.h>
#include <uapi/linux/fanotify.h>

#define FAN_GROUP_FLAG(group, flag) \
	((group)->fanotify_data.flags & (flag))

/*
 * Flags allowed to be passed from/to userspace.
 *
 * We intentionally do not add new bits to the old FAN_ALL_* constants, because
 * they are uapi exposed constants. If there are programs out there using
 * these constant, the programs may break if re-compiled with new uapi headers
 * and then run on an old kernel.
 */

/* Group classes where permission events are allowed */
#define FANOTIFY_PERM_CLASSES	(FAN_CLASS_CONTENT | \
				 FAN_CLASS_PRE_CONTENT)

#define FANOTIFY_CLASS_BITS	(FAN_CLASS_NOTIF | FANOTIFY_PERM_CLASSES)

#define FANOTIFY_FID_BITS	(FAN_REPORT_DFID_NAME_TARGET)

#define FANOTIFY_INFO_MODES	(FANOTIFY_FID_BITS | FAN_REPORT_PIDFD)

/*
 * fanotify_init() flags that require CAP_SYS_ADMIN.
 * We do not allow unprivileged groups to request permission events.
 * We do not allow unprivileged groups to get other process pid in events.
 * We do not allow unprivileged groups to use unlimited resources.
 */
#define FANOTIFY_ADMIN_INIT_FLAGS	(FANOTIFY_PERM_CLASSES | \
					 FAN_REPORT_TID | \
					 FAN_REPORT_PIDFD | \
					 FAN_UNLIMITED_QUEUE | \
					 FAN_UNLIMITED_MARKS)

/*
 * fanotify_init() flags that are allowed for user without CAP_SYS_ADMIN.
 * FAN_CLASS_NOTIF is the only class we allow for unprivileged group.
 * We do not allow unprivileged groups to get file descriptors in events,
 * so one of the flags for reporting file handles is required.
 */
#define FANOTIFY_USER_INIT_FLAGS	(FAN_CLASS_NOTIF | \
					 FANOTIFY_FID_BITS | \
					 FAN_CLOEXEC | FAN_NONBLOCK)

#define FANOTIFY_INIT_FLAGS	(FANOTIFY_ADMIN_INIT_FLAGS | \
				 FANOTIFY_USER_INIT_FLAGS)

/* Internal group flags */
#define FANOTIFY_UNPRIV		0x80000000
#define FANOTIFY_INTERNAL_GROUP_FLAGS	(FANOTIFY_UNPRIV)

#define FANOTIFY_MARK_TYPE_BITS	(FAN_MARK_INODE | FAN_MARK_MOUNT | \
				 FAN_MARK_FILESYSTEM)

#define FANOTIFY_MARK_CMD_BITS	(FAN_MARK_ADD | FAN_MARK_REMOVE | \
				 FAN_MARK_FLUSH)

#define FANOTIFY_MARK_IGNORE_BITS (FAN_MARK_IGNORED_MASK | \
				   FAN_MARK_IGNORE)

#define FANOTIFY_MARK_FLAGS	(FANOTIFY_MARK_TYPE_BITS | \
				 FANOTIFY_MARK_CMD_BITS | \
				 FANOTIFY_MARK_IGNORE_BITS | \
				 FAN_MARK_DONT_FOLLOW | \
				 FAN_MARK_ONLYDIR | \
				 FAN_MARK_IGNORED_SURV_MODIFY | \
				 FAN_MARK_EVICTABLE)

/*
 * Events that can be reported with data type FSNOTIFY_EVENT_PATH.
 * Note that FAN_MODIFY can also be reported with data type
 * FSNOTIFY_EVENT_INODE.
 */
#define FANOTIFY_PATH_EVENTS	(FAN_ACCESS | FAN_MODIFY | \
				 FAN_CLOSE | FAN_OPEN | FAN_OPEN_EXEC)

/*
 * Directory entry modification events - reported only to directory
 * where entry is modified and not to a watching parent.
 */
#define FANOTIFY_DIRENT_EVENTS	(FAN_MOVE | FAN_CREATE | FAN_DELETE | \
				 FAN_RENAME)

/* Events that can be reported with event->fd */
#define FANOTIFY_FD_EVENTS (FANOTIFY_PATH_EVENTS | FANOTIFY_PERM_EVENTS)

/* Events that can only be reported with data type FSNOTIFY_EVENT_INODE */
#define FANOTIFY_INODE_EVENTS	(FANOTIFY_DIRENT_EVENTS | \
				 FAN_ATTRIB | FAN_MOVE_SELF | FAN_DELETE_SELF)

/* Events that can only be reported with data type FSNOTIFY_EVENT_ERROR */
#define FANOTIFY_ERROR_EVENTS	(FAN_FS_ERROR)

/* Events that user can request to be notified on */
#define FANOTIFY_EVENTS		(FANOTIFY_PATH_EVENTS | \
				 FANOTIFY_INODE_EVENTS | \
				 FANOTIFY_ERROR_EVENTS)

/* Events that require a permission response from user */
#define FANOTIFY_PERM_EVENTS	(FAN_OPEN_PERM | FAN_ACCESS_PERM | \
				 FAN_OPEN_EXEC_PERM)

/* Extra flags that may be reported with event or control handling of events */
#define FANOTIFY_EVENT_FLAGS	(FAN_EVENT_ON_CHILD | FAN_ONDIR)

/* Events that may be reported to user */
#define FANOTIFY_OUTGOING_EVENTS	(FANOTIFY_EVENTS | \
					 FANOTIFY_PERM_EVENTS | \
					 FAN_Q_OVERFLOW | FAN_ONDIR)

/* Events and flags relevant only for directories */
#define FANOTIFY_DIRONLY_EVENT_BITS	(FANOTIFY_DIRENT_EVENTS | \
					 FAN_EVENT_ON_CHILD | FAN_ONDIR)

#define ALL_FANOTIFY_EVENT_BITS		(FANOTIFY_OUTGOING_EVENTS | \
					 FANOTIFY_EVENT_FLAGS)

/* These masks check for invalid bits in permission responses. */
#define FANOTIFY_RESPONSE_ACCESS (FAN_ALLOW | FAN_DENY)
#define FANOTIFY_RESPONSE_FLAGS (FAN_AUDIT | FAN_INFO)
#define FANOTIFY_RESPONSE_VALID_MASK (FANOTIFY_RESPONSE_ACCESS | FANOTIFY_RESPONSE_FLAGS)

/* Do not use these old uapi constants internally */
#undef FAN_ALL_CLASS_BITS
#undef FAN_ALL_INIT_FLAGS
#undef FAN_ALL_MARK_FLAGS
#undef FAN_ALL_EVENTS
#undef FAN_ALL_PERM_EVENTS
#undef FAN_ALL_OUTGOING_EVENTS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_FANOTIFY_H */
