<<<<<<< HEAD
#include <linux/fanotify.h>
#include <linux/fcntl.h>
=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/fanotify.h>
#include <linux/fcntl.h>
#include <linux/fdtable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/anon_inodes.h>
#include <linux/fsnotify_backend.h>
#include <linux/init.h>
#include <linux/mount.h>
#include <linux/namei.h>
#include <linux/poll.h>
#include <linux/security.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>
<<<<<<< HEAD

#include <asm/ioctls.h>

#include "../../mount.h"

#define FANOTIFY_DEFAULT_MAX_EVENTS	16384
#define FANOTIFY_DEFAULT_MAX_MARKS	8192
#define FANOTIFY_DEFAULT_MAX_LISTENERS	128
=======
#include <linux/compat.h>
#include <linux/sched/signal.h>
#include <linux/memcontrol.h>
#include <linux/statfs.h>
#include <linux/exportfs.h>

#include <asm/ioctls.h>

#include "../fsnotify.h"
#include "../fdinfo.h"
#include "fanotify.h"

#define FANOTIFY_DEFAULT_MAX_EVENTS	16384
#define FANOTIFY_OLD_DEFAULT_MAX_MARKS	8192
#define FANOTIFY_DEFAULT_MAX_GROUPS	128
#define FANOTIFY_DEFAULT_FEE_POOL_SIZE	32

/*
 * Legacy fanotify marks limits (8192) is per group and we introduced a tunable
 * limit of marks per user, similar to inotify.  Effectively, the legacy limit
 * of fanotify marks per user is <max marks per group> * <max groups per user>.
 * This default limit (1M) also happens to match the increased limit of inotify
 * max_user_watches since v5.10.
 */
#define FANOTIFY_DEFAULT_MAX_USER_MARKS	\
	(FANOTIFY_OLD_DEFAULT_MAX_MARKS * FANOTIFY_DEFAULT_MAX_GROUPS)

/*
 * Most of the memory cost of adding an inode mark is pinning the marked inode.
 * The size of the filesystem inode struct is not uniform across filesystems,
 * so double the size of a VFS inode is used as a conservative approximation.
 */
#define INODE_MARK_COST	(2 * sizeof(struct inode))

/* configurable via /proc/sys/fs/fanotify/ */
static int fanotify_max_queued_events __read_mostly;

#ifdef CONFIG_SYSCTL

#include <linux/sysctl.h>

static long ft_zero = 0;
static long ft_int_max = INT_MAX;

static struct ctl_table fanotify_table[] = {
	{
		.procname	= "max_user_groups",
		.data	= &init_user_ns.ucount_max[UCOUNT_FANOTIFY_GROUPS],
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= &ft_zero,
		.extra2		= &ft_int_max,
	},
	{
		.procname	= "max_user_marks",
		.data	= &init_user_ns.ucount_max[UCOUNT_FANOTIFY_MARKS],
		.maxlen		= sizeof(long),
		.mode		= 0644,
		.proc_handler	= proc_doulongvec_minmax,
		.extra1		= &ft_zero,
		.extra2		= &ft_int_max,
	},
	{
		.procname	= "max_queued_events",
		.data		= &fanotify_max_queued_events,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec_minmax,
		.extra1		= SYSCTL_ZERO
	},
};

static void __init fanotify_sysctls_init(void)
{
	register_sysctl("fs/fanotify", fanotify_table);
}
#else
#define fanotify_sysctls_init() do { } while (0)
#endif /* CONFIG_SYSCTL */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * All flags that may be specified in parameter event_f_flags of fanotify_init.
 *
 * Internal and external open flags are stored together in field f_flags of
 * struct file. Only external open flags shall be allowed in event_f_flags.
 * Internal flags like FMODE_NONOTIFY, FMODE_EXEC, FMODE_NOCMTIME shall be
 * excluded.
 */
#define	FANOTIFY_INIT_ALL_EVENT_F_BITS				( \
		O_ACCMODE	| O_APPEND	| O_NONBLOCK	| \
		__O_SYNC	| O_DSYNC	| O_CLOEXEC     | \
		O_LARGEFILE	| O_NOATIME	)

extern const struct fsnotify_ops fanotify_fsnotify_ops;

<<<<<<< HEAD
static struct kmem_cache *fanotify_mark_cache __read_mostly;
static struct kmem_cache *fanotify_response_event_cache __read_mostly;

struct fanotify_response_event {
	struct list_head list;
	__s32 fd;
	struct fsnotify_event *event;
};

/*
 * Get an fsnotify notification event if one exists and is small
 * enough to fit in "count". Return an error pointer if the count
 * is not large enough.
 *
 * Called with the group->notification_mutex held.
 */
static struct fsnotify_event *get_one_event(struct fsnotify_group *group,
					    size_t count)
{
	BUG_ON(!mutex_is_locked(&group->notification_mutex));

	pr_debug("%s: group=%p count=%zd\n", __func__, group, count);

	if (fsnotify_notify_queue_is_empty(group))
		return NULL;

	if (FAN_EVENT_METADATA_LEN > count)
		return ERR_PTR(-EINVAL);

	/* held the notification_mutex the whole time, so this is the
	 * same event we peeked above */
	return fsnotify_remove_notify_event(group);
}

static int create_fd(struct fsnotify_group *group, struct fsnotify_event *event)
{
	int client_fd;
	struct dentry *dentry;
	struct vfsmount *mnt;
	struct file *new_file;

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

=======
struct kmem_cache *fanotify_mark_cache __ro_after_init;
struct kmem_cache *fanotify_fid_event_cachep __ro_after_init;
struct kmem_cache *fanotify_path_event_cachep __ro_after_init;
struct kmem_cache *fanotify_perm_event_cachep __ro_after_init;

#define FANOTIFY_EVENT_ALIGN 4
#define FANOTIFY_FID_INFO_HDR_LEN \
	(sizeof(struct fanotify_event_info_fid) + sizeof(struct file_handle))
#define FANOTIFY_PIDFD_INFO_HDR_LEN \
	sizeof(struct fanotify_event_info_pidfd)
#define FANOTIFY_ERROR_INFO_LEN \
	(sizeof(struct fanotify_event_info_error))

static int fanotify_fid_info_len(int fh_len, int name_len)
{
	int info_len = fh_len;

	if (name_len)
		info_len += name_len + 1;

	return roundup(FANOTIFY_FID_INFO_HDR_LEN + info_len,
		       FANOTIFY_EVENT_ALIGN);
}

/* FAN_RENAME may have one or two dir+name info records */
static int fanotify_dir_name_info_len(struct fanotify_event *event)
{
	struct fanotify_info *info = fanotify_event_info(event);
	int dir_fh_len = fanotify_event_dir_fh_len(event);
	int dir2_fh_len = fanotify_event_dir2_fh_len(event);
	int info_len = 0;

	if (dir_fh_len)
		info_len += fanotify_fid_info_len(dir_fh_len,
						  info->name_len);
	if (dir2_fh_len)
		info_len += fanotify_fid_info_len(dir2_fh_len,
						  info->name2_len);

	return info_len;
}

static size_t fanotify_event_len(unsigned int info_mode,
				 struct fanotify_event *event)
{
	size_t event_len = FAN_EVENT_METADATA_LEN;
	int fh_len;
	int dot_len = 0;

	if (!info_mode)
		return event_len;

	if (fanotify_is_error_event(event->mask))
		event_len += FANOTIFY_ERROR_INFO_LEN;

	if (fanotify_event_has_any_dir_fh(event)) {
		event_len += fanotify_dir_name_info_len(event);
	} else if ((info_mode & FAN_REPORT_NAME) &&
		   (event->mask & FAN_ONDIR)) {
		/*
		 * With group flag FAN_REPORT_NAME, if name was not recorded in
		 * event on a directory, we will report the name ".".
		 */
		dot_len = 1;
	}

	if (info_mode & FAN_REPORT_PIDFD)
		event_len += FANOTIFY_PIDFD_INFO_HDR_LEN;

	if (fanotify_event_has_object_fh(event)) {
		fh_len = fanotify_event_object_fh_len(event);
		event_len += fanotify_fid_info_len(fh_len, dot_len);
	}

	return event_len;
}

/*
 * Remove an hashed event from merge hash table.
 */
static void fanotify_unhash_event(struct fsnotify_group *group,
				  struct fanotify_event *event)
{
	assert_spin_locked(&group->notification_lock);

	pr_debug("%s: group=%p event=%p bucket=%u\n", __func__,
		 group, event, fanotify_event_hash_bucket(group, event));

	if (WARN_ON_ONCE(hlist_unhashed(&event->merge_list)))
		return;

	hlist_del_init(&event->merge_list);
}

/*
 * Get an fanotify notification event if one exists and is small
 * enough to fit in "count". Return an error pointer if the count
 * is not large enough. When permission event is dequeued, its state is
 * updated accordingly.
 */
static struct fanotify_event *get_one_event(struct fsnotify_group *group,
					    size_t count)
{
	size_t event_size;
	struct fanotify_event *event = NULL;
	struct fsnotify_event *fsn_event;
	unsigned int info_mode = FAN_GROUP_FLAG(group, FANOTIFY_INFO_MODES);

	pr_debug("%s: group=%p count=%zd\n", __func__, group, count);

	spin_lock(&group->notification_lock);
	fsn_event = fsnotify_peek_first_event(group);
	if (!fsn_event)
		goto out;

	event = FANOTIFY_E(fsn_event);
	event_size = fanotify_event_len(info_mode, event);

	if (event_size > count) {
		event = ERR_PTR(-EINVAL);
		goto out;
	}

	/*
	 * Held the notification_lock the whole time, so this is the
	 * same event we peeked above.
	 */
	fsnotify_remove_first_event(group);
	if (fanotify_is_perm_event(event->mask))
		FANOTIFY_PERM(event)->state = FAN_EVENT_REPORTED;
	if (fanotify_is_hashed_event(event->mask))
		fanotify_unhash_event(group, event);
out:
	spin_unlock(&group->notification_lock);
	return event;
}

static int create_fd(struct fsnotify_group *group, const struct path *path,
		     struct file **file)
{
	int client_fd;
	struct file *new_file;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	client_fd = get_unused_fd_flags(group->fanotify_data.f_flags);
	if (client_fd < 0)
		return client_fd;

<<<<<<< HEAD
	if (event->data_type != FSNOTIFY_EVENT_PATH) {
		WARN_ON(1);
		put_unused_fd(client_fd);
		return -EINVAL;
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * we need a new file handle for the userspace program so it can read even if it was
	 * originally opened O_WRONLY.
	 */
<<<<<<< HEAD
	dentry = dget(event->path.dentry);
	mnt = mntget(event->path.mnt);
	/* it's possible this event was an overflow event.  in that case dentry and mnt
	 * are NULL;  That's fine, just don't call dentry open */
	if (dentry && mnt)
		new_file = dentry_open(dentry, mnt,
				       group->fanotify_data.f_flags | FMODE_NONOTIFY,
				       current_cred());
	else
		new_file = ERR_PTR(-EOVERFLOW);
=======
	new_file = dentry_open(path,
			       group->fanotify_data.f_flags | __FMODE_NONOTIFY,
			       current_cred());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(new_file)) {
		/*
		 * we still send an event even if we can't open the file.  this
		 * can happen when say tasks are gone and we try to open their
		 * /proc files or we try to open a WRONLY file like in sysfs
		 * we just send the errno to userspace since there isn't much
		 * else we can do.
		 */
		put_unused_fd(client_fd);
		client_fd = PTR_ERR(new_file);
	} else {
<<<<<<< HEAD
		fd_install(client_fd, new_file);
=======
		*file = new_file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return client_fd;
}

<<<<<<< HEAD
static int fill_event_metadata(struct fsnotify_group *group,
				   struct fanotify_event_metadata *metadata,
				   struct fsnotify_event *event)
{
	int ret = 0;

	pr_debug("%s: group=%p metadata=%p event=%p\n", __func__,
		 group, metadata, event);

	metadata->event_len = FAN_EVENT_METADATA_LEN;
	metadata->metadata_len = FAN_EVENT_METADATA_LEN;
	metadata->vers = FANOTIFY_METADATA_VERSION;
	metadata->reserved = 0;
	metadata->mask = event->mask & FAN_ALL_OUTGOING_EVENTS;
	metadata->pid = pid_vnr(event->tgid);
	if (unlikely(event->mask & FAN_Q_OVERFLOW))
		metadata->fd = FAN_NOFD;
	else {
		metadata->fd = create_fd(group, event);
		if (metadata->fd < 0)
			ret = metadata->fd;
	}

	return ret;
}

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
static struct fanotify_response_event *dequeue_re(struct fsnotify_group *group,
						  __s32 fd)
{
	struct fanotify_response_event *re, *return_re = NULL;

	mutex_lock(&group->fanotify_data.access_mutex);
	list_for_each_entry(re, &group->fanotify_data.access_list, list) {
		if (re->fd != fd)
			continue;

		list_del_init(&re->list);
		return_re = re;
		break;
	}
	mutex_unlock(&group->fanotify_data.access_mutex);

	pr_debug("%s: found return_re=%p\n", __func__, return_re);

	return return_re;
}

static int process_access_response(struct fsnotify_group *group,
				   struct fanotify_response *response_struct)
{
	struct fanotify_response_event *re;
	__s32 fd = response_struct->fd;
	__u32 response = response_struct->response;

	pr_debug("%s: group=%p fd=%d response=%d\n", __func__, group,
		 fd, response);
=======
static int process_access_response_info(const char __user *info,
					size_t info_len,
				struct fanotify_response_info_audit_rule *friar)
{
	if (info_len != sizeof(*friar))
		return -EINVAL;

	if (copy_from_user(friar, info, sizeof(*friar)))
		return -EFAULT;

	if (friar->hdr.type != FAN_RESPONSE_INFO_AUDIT_RULE)
		return -EINVAL;
	if (friar->hdr.pad != 0)
		return -EINVAL;
	if (friar->hdr.len != sizeof(*friar))
		return -EINVAL;

	return info_len;
}

/*
 * Finish processing of permission event by setting it to ANSWERED state and
 * drop group->notification_lock.
 */
static void finish_permission_event(struct fsnotify_group *group,
				    struct fanotify_perm_event *event, u32 response,
				    struct fanotify_response_info_audit_rule *friar)
				    __releases(&group->notification_lock)
{
	bool destroy = false;

	assert_spin_locked(&group->notification_lock);
	event->response = response & ~FAN_INFO;
	if (response & FAN_INFO)
		memcpy(&event->audit_rule, friar, sizeof(*friar));

	if (event->state == FAN_EVENT_CANCELED)
		destroy = true;
	else
		event->state = FAN_EVENT_ANSWERED;
	spin_unlock(&group->notification_lock);
	if (destroy)
		fsnotify_destroy_event(group, &event->fae.fse);
}

static int process_access_response(struct fsnotify_group *group,
				   struct fanotify_response *response_struct,
				   const char __user *info,
				   size_t info_len)
{
	struct fanotify_perm_event *event;
	int fd = response_struct->fd;
	u32 response = response_struct->response;
	int ret = info_len;
	struct fanotify_response_info_audit_rule friar;

	pr_debug("%s: group=%p fd=%d response=%u buf=%p size=%zu\n", __func__,
		 group, fd, response, info, info_len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * make sure the response is valid, if invalid we do nothing and either
	 * userspace can send a valid response or we will clean it up after the
	 * timeout
	 */
<<<<<<< HEAD
	switch (response) {
=======
	if (response & ~FANOTIFY_RESPONSE_VALID_MASK)
		return -EINVAL;

	switch (response & FANOTIFY_RESPONSE_ACCESS) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case FAN_ALLOW:
	case FAN_DENY:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	if (fd < 0)
		return -EINVAL;

	re = dequeue_re(group, fd);
	if (!re)
		return -ENOENT;

	re->event->response = response;

	wake_up(&group->fanotify_data.access_waitq);

	kmem_cache_free(fanotify_response_event_cache, re);

	return 0;
}

static int prepare_for_access_response(struct fsnotify_group *group,
				       struct fsnotify_event *event,
				       __s32 fd)
{
	struct fanotify_response_event *re;

	if (!(event->mask & FAN_ALL_PERM_EVENTS))
		return 0;

	re = kmem_cache_alloc(fanotify_response_event_cache, GFP_KERNEL);
	if (!re)
		return -ENOMEM;

	re->event = event;
	re->fd = fd;

	mutex_lock(&group->fanotify_data.access_mutex);

	if (atomic_read(&group->fanotify_data.bypass_perm)) {
		mutex_unlock(&group->fanotify_data.access_mutex);
		kmem_cache_free(fanotify_response_event_cache, re);
		event->response = FAN_ALLOW;
		return 0;
	}
		
	list_add_tail(&re->list, &group->fanotify_data.access_list);
	mutex_unlock(&group->fanotify_data.access_mutex);

	return 0;
}

static void remove_access_response(struct fsnotify_group *group,
				   struct fsnotify_event *event,
				   __s32 fd)
{
	struct fanotify_response_event *re;

	if (!(event->mask & FAN_ALL_PERM_EVENTS))
		return;

	re = dequeue_re(group, fd);
	if (!re)
		return;

	BUG_ON(re->event != event);

	kmem_cache_free(fanotify_response_event_cache, re);

	return;
}
#else
static int prepare_for_access_response(struct fsnotify_group *group,
				       struct fsnotify_event *event,
				       __s32 fd)
{
	return 0;
}

static void remove_access_response(struct fsnotify_group *group,
				   struct fsnotify_event *event,
				   __s32 fd)
{
	return;
}
#endif

static ssize_t copy_event_to_user(struct fsnotify_group *group,
				  struct fsnotify_event *event,
				  char __user *buf)
{
	struct fanotify_event_metadata fanotify_event_metadata;
	int fd, ret;

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	ret = fill_event_metadata(group, &fanotify_event_metadata, event);
	if (ret < 0)
		goto out;

	fd = fanotify_event_metadata.fd;
	ret = prepare_for_access_response(group, event, fd);
	if (ret)
		goto out_close_fd;

	ret = -EFAULT;
	if (copy_to_user(buf, &fanotify_event_metadata,
			 fanotify_event_metadata.event_len))
		goto out_kill_access_response;

	return fanotify_event_metadata.event_len;

out_kill_access_response:
	remove_access_response(group, event, fd);
out_close_fd:
	if (fd != FAN_NOFD)
		sys_close(fd);
out:
#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	if (event->mask & FAN_ALL_PERM_EVENTS) {
		event->response = FAN_DENY;
		wake_up(&group->fanotify_data.access_waitq);
	}
#endif
=======
	if ((response & FAN_AUDIT) && !FAN_GROUP_FLAG(group, FAN_ENABLE_AUDIT))
		return -EINVAL;

	if (response & FAN_INFO) {
		ret = process_access_response_info(info, info_len, &friar);
		if (ret < 0)
			return ret;
		if (fd == FAN_NOFD)
			return ret;
	} else {
		ret = 0;
	}

	if (fd < 0)
		return -EINVAL;

	spin_lock(&group->notification_lock);
	list_for_each_entry(event, &group->fanotify_data.access_list,
			    fae.fse.list) {
		if (event->fd != fd)
			continue;

		list_del_init(&event->fae.fse.list);
		finish_permission_event(group, event, response, &friar);
		wake_up(&group->fanotify_data.access_waitq);
		return ret;
	}
	spin_unlock(&group->notification_lock);

	return -ENOENT;
}

static size_t copy_error_info_to_user(struct fanotify_event *event,
				      char __user *buf, int count)
{
	struct fanotify_event_info_error info = { };
	struct fanotify_error_event *fee = FANOTIFY_EE(event);

	info.hdr.info_type = FAN_EVENT_INFO_TYPE_ERROR;
	info.hdr.len = FANOTIFY_ERROR_INFO_LEN;

	if (WARN_ON(count < info.hdr.len))
		return -EFAULT;

	info.error = fee->error;
	info.error_count = fee->err_count;

	if (copy_to_user(buf, &info, sizeof(info)))
		return -EFAULT;

	return info.hdr.len;
}

static int copy_fid_info_to_user(__kernel_fsid_t *fsid, struct fanotify_fh *fh,
				 int info_type, const char *name,
				 size_t name_len,
				 char __user *buf, size_t count)
{
	struct fanotify_event_info_fid info = { };
	struct file_handle handle = { };
	unsigned char bounce[FANOTIFY_INLINE_FH_LEN], *fh_buf;
	size_t fh_len = fh ? fh->len : 0;
	size_t info_len = fanotify_fid_info_len(fh_len, name_len);
	size_t len = info_len;

	pr_debug("%s: fh_len=%zu name_len=%zu, info_len=%zu, count=%zu\n",
		 __func__, fh_len, name_len, info_len, count);

	if (WARN_ON_ONCE(len < sizeof(info) || len > count))
		return -EFAULT;

	/*
	 * Copy event info fid header followed by variable sized file handle
	 * and optionally followed by variable sized filename.
	 */
	switch (info_type) {
	case FAN_EVENT_INFO_TYPE_FID:
	case FAN_EVENT_INFO_TYPE_DFID:
		if (WARN_ON_ONCE(name_len))
			return -EFAULT;
		break;
	case FAN_EVENT_INFO_TYPE_DFID_NAME:
	case FAN_EVENT_INFO_TYPE_OLD_DFID_NAME:
	case FAN_EVENT_INFO_TYPE_NEW_DFID_NAME:
		if (WARN_ON_ONCE(!name || !name_len))
			return -EFAULT;
		break;
	default:
		return -EFAULT;
	}

	info.hdr.info_type = info_type;
	info.hdr.len = len;
	info.fsid = *fsid;
	if (copy_to_user(buf, &info, sizeof(info)))
		return -EFAULT;

	buf += sizeof(info);
	len -= sizeof(info);
	if (WARN_ON_ONCE(len < sizeof(handle)))
		return -EFAULT;

	handle.handle_type = fh->type;
	handle.handle_bytes = fh_len;

	/* Mangle handle_type for bad file_handle */
	if (!fh_len)
		handle.handle_type = FILEID_INVALID;

	if (copy_to_user(buf, &handle, sizeof(handle)))
		return -EFAULT;

	buf += sizeof(handle);
	len -= sizeof(handle);
	if (WARN_ON_ONCE(len < fh_len))
		return -EFAULT;

	/*
	 * For an inline fh and inline file name, copy through stack to exclude
	 * the copy from usercopy hardening protections.
	 */
	fh_buf = fanotify_fh_buf(fh);
	if (fh_len <= FANOTIFY_INLINE_FH_LEN) {
		memcpy(bounce, fh_buf, fh_len);
		fh_buf = bounce;
	}
	if (copy_to_user(buf, fh_buf, fh_len))
		return -EFAULT;

	buf += fh_len;
	len -= fh_len;

	if (name_len) {
		/* Copy the filename with terminating null */
		name_len++;
		if (WARN_ON_ONCE(len < name_len))
			return -EFAULT;

		if (copy_to_user(buf, name, name_len))
			return -EFAULT;

		buf += name_len;
		len -= name_len;
	}

	/* Pad with 0's */
	WARN_ON_ONCE(len < 0 || len >= FANOTIFY_EVENT_ALIGN);
	if (len > 0 && clear_user(buf, len))
		return -EFAULT;

	return info_len;
}

static int copy_pidfd_info_to_user(int pidfd,
				   char __user *buf,
				   size_t count)
{
	struct fanotify_event_info_pidfd info = { };
	size_t info_len = FANOTIFY_PIDFD_INFO_HDR_LEN;

	if (WARN_ON_ONCE(info_len > count))
		return -EFAULT;

	info.hdr.info_type = FAN_EVENT_INFO_TYPE_PIDFD;
	info.hdr.len = info_len;
	info.pidfd = pidfd;

	if (copy_to_user(buf, &info, info_len))
		return -EFAULT;

	return info_len;
}

static int copy_info_records_to_user(struct fanotify_event *event,
				     struct fanotify_info *info,
				     unsigned int info_mode, int pidfd,
				     char __user *buf, size_t count)
{
	int ret, total_bytes = 0, info_type = 0;
	unsigned int fid_mode = info_mode & FANOTIFY_FID_BITS;
	unsigned int pidfd_mode = info_mode & FAN_REPORT_PIDFD;

	/*
	 * Event info records order is as follows:
	 * 1. dir fid + name
	 * 2. (optional) new dir fid + new name
	 * 3. (optional) child fid
	 */
	if (fanotify_event_has_dir_fh(event)) {
		info_type = info->name_len ? FAN_EVENT_INFO_TYPE_DFID_NAME :
					     FAN_EVENT_INFO_TYPE_DFID;

		/* FAN_RENAME uses special info types */
		if (event->mask & FAN_RENAME)
			info_type = FAN_EVENT_INFO_TYPE_OLD_DFID_NAME;

		ret = copy_fid_info_to_user(fanotify_event_fsid(event),
					    fanotify_info_dir_fh(info),
					    info_type,
					    fanotify_info_name(info),
					    info->name_len, buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	/* New dir fid+name may be reported in addition to old dir fid+name */
	if (fanotify_event_has_dir2_fh(event)) {
		info_type = FAN_EVENT_INFO_TYPE_NEW_DFID_NAME;
		ret = copy_fid_info_to_user(fanotify_event_fsid(event),
					    fanotify_info_dir2_fh(info),
					    info_type,
					    fanotify_info_name2(info),
					    info->name2_len, buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	if (fanotify_event_has_object_fh(event)) {
		const char *dot = NULL;
		int dot_len = 0;

		if (fid_mode == FAN_REPORT_FID || info_type) {
			/*
			 * With only group flag FAN_REPORT_FID only type FID is
			 * reported. Second info record type is always FID.
			 */
			info_type = FAN_EVENT_INFO_TYPE_FID;
		} else if ((fid_mode & FAN_REPORT_NAME) &&
			   (event->mask & FAN_ONDIR)) {
			/*
			 * With group flag FAN_REPORT_NAME, if name was not
			 * recorded in an event on a directory, report the name
			 * "." with info type DFID_NAME.
			 */
			info_type = FAN_EVENT_INFO_TYPE_DFID_NAME;
			dot = ".";
			dot_len = 1;
		} else if ((event->mask & ALL_FSNOTIFY_DIRENT_EVENTS) ||
			   (event->mask & FAN_ONDIR)) {
			/*
			 * With group flag FAN_REPORT_DIR_FID, a single info
			 * record has type DFID for directory entry modification
			 * event and for event on a directory.
			 */
			info_type = FAN_EVENT_INFO_TYPE_DFID;
		} else {
			/*
			 * With group flags FAN_REPORT_DIR_FID|FAN_REPORT_FID,
			 * a single info record has type FID for event on a
			 * non-directory, when there is no directory to report.
			 * For example, on FAN_DELETE_SELF event.
			 */
			info_type = FAN_EVENT_INFO_TYPE_FID;
		}

		ret = copy_fid_info_to_user(fanotify_event_fsid(event),
					    fanotify_event_object_fh(event),
					    info_type, dot, dot_len,
					    buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	if (pidfd_mode) {
		ret = copy_pidfd_info_to_user(pidfd, buf, count);
		if (ret < 0)
			return ret;

		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	if (fanotify_is_error_event(event->mask)) {
		ret = copy_error_info_to_user(event, buf, count);
		if (ret < 0)
			return ret;
		buf += ret;
		count -= ret;
		total_bytes += ret;
	}

	return total_bytes;
}

static ssize_t copy_event_to_user(struct fsnotify_group *group,
				  struct fanotify_event *event,
				  char __user *buf, size_t count)
{
	struct fanotify_event_metadata metadata;
	const struct path *path = fanotify_event_path(event);
	struct fanotify_info *info = fanotify_event_info(event);
	unsigned int info_mode = FAN_GROUP_FLAG(group, FANOTIFY_INFO_MODES);
	unsigned int pidfd_mode = info_mode & FAN_REPORT_PIDFD;
	struct file *f = NULL, *pidfd_file = NULL;
	int ret, pidfd = FAN_NOPIDFD, fd = FAN_NOFD;

	pr_debug("%s: group=%p event=%p\n", __func__, group, event);

	metadata.event_len = fanotify_event_len(info_mode, event);
	metadata.metadata_len = FAN_EVENT_METADATA_LEN;
	metadata.vers = FANOTIFY_METADATA_VERSION;
	metadata.reserved = 0;
	metadata.mask = event->mask & FANOTIFY_OUTGOING_EVENTS;
	metadata.pid = pid_vnr(event->pid);
	/*
	 * For an unprivileged listener, event->pid can be used to identify the
	 * events generated by the listener process itself, without disclosing
	 * the pids of other processes.
	 */
	if (FAN_GROUP_FLAG(group, FANOTIFY_UNPRIV) &&
	    task_tgid(current) != event->pid)
		metadata.pid = 0;

	/*
	 * For now, fid mode is required for an unprivileged listener and
	 * fid mode does not report fd in events.  Keep this check anyway
	 * for safety in case fid mode requirement is relaxed in the future
	 * to allow unprivileged listener to get events with no fd and no fid.
	 */
	if (!FAN_GROUP_FLAG(group, FANOTIFY_UNPRIV) &&
	    path && path->mnt && path->dentry) {
		fd = create_fd(group, path, &f);
		if (fd < 0)
			return fd;
	}
	metadata.fd = fd;

	if (pidfd_mode) {
		/*
		 * Complain if the FAN_REPORT_PIDFD and FAN_REPORT_TID mutual
		 * exclusion is ever lifted. At the time of incoporating pidfd
		 * support within fanotify, the pidfd API only supported the
		 * creation of pidfds for thread-group leaders.
		 */
		WARN_ON_ONCE(FAN_GROUP_FLAG(group, FAN_REPORT_TID));

		/*
		 * The PIDTYPE_TGID check for an event->pid is performed
		 * preemptively in an attempt to catch out cases where the event
		 * listener reads events after the event generating process has
		 * already terminated. Report FAN_NOPIDFD to the event listener
		 * in those cases, with all other pidfd creation errors being
		 * reported as FAN_EPIDFD.
		 */
		if (metadata.pid == 0 ||
		    !pid_has_task(event->pid, PIDTYPE_TGID)) {
			pidfd = FAN_NOPIDFD;
		} else {
			pidfd = pidfd_prepare(event->pid, 0, &pidfd_file);
			if (pidfd < 0)
				pidfd = FAN_EPIDFD;
		}
	}

	ret = -EFAULT;
	/*
	 * Sanity check copy size in case get_one_event() and
	 * event_len sizes ever get out of sync.
	 */
	if (WARN_ON_ONCE(metadata.event_len > count))
		goto out_close_fd;

	if (copy_to_user(buf, &metadata, FAN_EVENT_METADATA_LEN))
		goto out_close_fd;

	buf += FAN_EVENT_METADATA_LEN;
	count -= FAN_EVENT_METADATA_LEN;

	if (fanotify_is_perm_event(event->mask))
		FANOTIFY_PERM(event)->fd = fd;

	if (info_mode) {
		ret = copy_info_records_to_user(event, info, info_mode, pidfd,
						buf, count);
		if (ret < 0)
			goto out_close_fd;
	}

	if (f)
		fd_install(fd, f);

	if (pidfd_file)
		fd_install(pidfd, pidfd_file);

	return metadata.event_len;

out_close_fd:
	if (fd != FAN_NOFD) {
		put_unused_fd(fd);
		fput(f);
	}

	if (pidfd >= 0) {
		put_unused_fd(pidfd);
		fput(pidfd_file);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/* intofiy userspace file descriptor functions */
<<<<<<< HEAD
static unsigned int fanotify_poll(struct file *file, poll_table *wait)
{
	struct fsnotify_group *group = file->private_data;
	int ret = 0;

	poll_wait(file, &group->notification_waitq, wait);
	mutex_lock(&group->notification_mutex);
	if (!fsnotify_notify_queue_is_empty(group))
		ret = POLLIN | POLLRDNORM;
	mutex_unlock(&group->notification_mutex);
=======
static __poll_t fanotify_poll(struct file *file, poll_table *wait)
{
	struct fsnotify_group *group = file->private_data;
	__poll_t ret = 0;

	poll_wait(file, &group->notification_waitq, wait);
	spin_lock(&group->notification_lock);
	if (!fsnotify_notify_queue_is_empty(group))
		ret = EPOLLIN | EPOLLRDNORM;
	spin_unlock(&group->notification_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static ssize_t fanotify_read(struct file *file, char __user *buf,
			     size_t count, loff_t *pos)
{
	struct fsnotify_group *group;
<<<<<<< HEAD
	struct fsnotify_event *kevent;
	char __user *start;
	int ret;
	DEFINE_WAIT(wait);
=======
	struct fanotify_event *event;
	char __user *start;
	int ret;
	DEFINE_WAIT_FUNC(wait, woken_wake_function);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	start = buf;
	group = file->private_data;

	pr_debug("%s: group=%p\n", __func__, group);

<<<<<<< HEAD
	while (1) {
		prepare_to_wait(&group->notification_waitq, &wait, TASK_INTERRUPTIBLE);

		mutex_lock(&group->notification_mutex);
		kevent = get_one_event(group, count);
		mutex_unlock(&group->notification_mutex);

		if (kevent) {
			ret = PTR_ERR(kevent);
			if (IS_ERR(kevent))
				break;
			ret = copy_event_to_user(group, kevent, buf);
			fsnotify_put_event(kevent);
			if (ret < 0)
				break;
			buf += ret;
			count -= ret;
			continue;
		}

		ret = -EAGAIN;
		if (file->f_flags & O_NONBLOCK)
			break;
		ret = -ERESTARTSYS;
		if (signal_pending(current))
			break;

		if (start != buf)
			break;

		schedule();
	}

	finish_wait(&group->notification_waitq, &wait);
=======
	add_wait_queue(&group->notification_waitq, &wait);
	while (1) {
		/*
		 * User can supply arbitrarily large buffer. Avoid softlockups
		 * in case there are lots of available events.
		 */
		cond_resched();
		event = get_one_event(group, count);
		if (IS_ERR(event)) {
			ret = PTR_ERR(event);
			break;
		}

		if (!event) {
			ret = -EAGAIN;
			if (file->f_flags & O_NONBLOCK)
				break;

			ret = -ERESTARTSYS;
			if (signal_pending(current))
				break;

			if (start != buf)
				break;

			wait_woken(&wait, TASK_INTERRUPTIBLE, MAX_SCHEDULE_TIMEOUT);
			continue;
		}

		ret = copy_event_to_user(group, event, buf, count);
		if (unlikely(ret == -EOPENSTALE)) {
			/*
			 * We cannot report events with stale fd so drop it.
			 * Setting ret to 0 will continue the event loop and
			 * do the right thing if there are no more events to
			 * read (i.e. return bytes read, -EAGAIN or wait).
			 */
			ret = 0;
		}

		/*
		 * Permission events get queued to wait for response.  Other
		 * events can be destroyed now.
		 */
		if (!fanotify_is_perm_event(event->mask)) {
			fsnotify_destroy_event(group, &event->fse);
		} else {
			if (ret <= 0) {
				spin_lock(&group->notification_lock);
				finish_permission_event(group,
					FANOTIFY_PERM(event), FAN_DENY, NULL);
				wake_up(&group->fanotify_data.access_waitq);
			} else {
				spin_lock(&group->notification_lock);
				list_add_tail(&event->fse.list,
					&group->fanotify_data.access_list);
				spin_unlock(&group->notification_lock);
			}
		}
		if (ret < 0)
			break;
		buf += ret;
		count -= ret;
	}
	remove_wait_queue(&group->notification_waitq, &wait);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (start != buf && ret != -EFAULT)
		ret = buf - start;
	return ret;
}

static ssize_t fanotify_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
<<<<<<< HEAD
#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	struct fanotify_response response = { .fd = -1, .response = -1 };
	struct fsnotify_group *group;
	int ret;

	group = file->private_data;

	if (count > sizeof(response))
		count = sizeof(response);

	pr_debug("%s: group=%p count=%zu\n", __func__, group, count);

	if (copy_from_user(&response, buf, count))
		return -EFAULT;

	ret = process_access_response(group, &response);
	if (ret < 0)
		count = ret;

	return count;
#else
	return -EINVAL;
#endif
=======
	struct fanotify_response response;
	struct fsnotify_group *group;
	int ret;
	const char __user *info_buf = buf + sizeof(struct fanotify_response);
	size_t info_len;

	if (!IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS))
		return -EINVAL;

	group = file->private_data;

	pr_debug("%s: group=%p count=%zu\n", __func__, group, count);

	if (count < sizeof(response))
		return -EINVAL;

	if (copy_from_user(&response, buf, sizeof(response)))
		return -EFAULT;

	info_len = count - sizeof(response);

	ret = process_access_response(group, &response, info_buf, info_len);
	if (ret < 0)
		count = ret;
	else
		count = sizeof(response) + ret;

	return count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fanotify_release(struct inode *ignored, struct file *file)
{
	struct fsnotify_group *group = file->private_data;
<<<<<<< HEAD

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	struct fanotify_response_event *re, *lre;

	mutex_lock(&group->fanotify_data.access_mutex);

	atomic_inc(&group->fanotify_data.bypass_perm);

	list_for_each_entry_safe(re, lre, &group->fanotify_data.access_list, list) {
		pr_debug("%s: found group=%p re=%p event=%p\n", __func__, group,
			 re, re->event);

		list_del_init(&re->list);
		re->event->response = FAN_ALLOW;

		kmem_cache_free(fanotify_response_event_cache, re);
	}
	mutex_unlock(&group->fanotify_data.access_mutex);

	wake_up(&group->fanotify_data.access_waitq);
#endif
	/* matches the fanotify_init->fsnotify_alloc_group */
	fsnotify_put_group(group);
=======
	struct fsnotify_event *fsn_event;

	/*
	 * Stop new events from arriving in the notification queue. since
	 * userspace cannot use fanotify fd anymore, no event can enter or
	 * leave access_list by now either.
	 */
	fsnotify_group_stop_queueing(group);

	/*
	 * Process all permission events on access_list and notification queue
	 * and simulate reply from userspace.
	 */
	spin_lock(&group->notification_lock);
	while (!list_empty(&group->fanotify_data.access_list)) {
		struct fanotify_perm_event *event;

		event = list_first_entry(&group->fanotify_data.access_list,
				struct fanotify_perm_event, fae.fse.list);
		list_del_init(&event->fae.fse.list);
		finish_permission_event(group, event, FAN_ALLOW, NULL);
		spin_lock(&group->notification_lock);
	}

	/*
	 * Destroy all non-permission events. For permission events just
	 * dequeue them and set the response. They will be freed once the
	 * response is consumed and fanotify_get_response() returns.
	 */
	while ((fsn_event = fsnotify_remove_first_event(group))) {
		struct fanotify_event *event = FANOTIFY_E(fsn_event);

		if (!(event->mask & FANOTIFY_PERM_EVENTS)) {
			spin_unlock(&group->notification_lock);
			fsnotify_destroy_event(group, fsn_event);
		} else {
			finish_permission_event(group, FANOTIFY_PERM(event),
						FAN_ALLOW, NULL);
		}
		spin_lock(&group->notification_lock);
	}
	spin_unlock(&group->notification_lock);

	/* Response for all permission events it set, wakeup waiters */
	wake_up(&group->fanotify_data.access_waitq);

	/* matches the fanotify_init->fsnotify_alloc_group */
	fsnotify_destroy_group(group);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static long fanotify_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct fsnotify_group *group;
<<<<<<< HEAD
	struct fsnotify_event_holder *holder;
=======
	struct fsnotify_event *fsn_event;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __user *p;
	int ret = -ENOTTY;
	size_t send_len = 0;

	group = file->private_data;

	p = (void __user *) arg;

	switch (cmd) {
	case FIONREAD:
<<<<<<< HEAD
		mutex_lock(&group->notification_mutex);
		list_for_each_entry(holder, &group->notification_list, event_list)
			send_len += FAN_EVENT_METADATA_LEN;
		mutex_unlock(&group->notification_mutex);
=======
		spin_lock(&group->notification_lock);
		list_for_each_entry(fsn_event, &group->notification_list, list)
			send_len += FAN_EVENT_METADATA_LEN;
		spin_unlock(&group->notification_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = put_user(send_len, (int __user *) p);
		break;
	}

	return ret;
}

static const struct file_operations fanotify_fops = {
<<<<<<< HEAD
=======
	.show_fdinfo	= fanotify_show_fdinfo,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.poll		= fanotify_poll,
	.read		= fanotify_read,
	.write		= fanotify_write,
	.fasync		= NULL,
	.release	= fanotify_release,
	.unlocked_ioctl	= fanotify_ioctl,
<<<<<<< HEAD
	.compat_ioctl	= fanotify_ioctl,
	.llseek		= noop_llseek,
};

static void fanotify_free_mark(struct fsnotify_mark *fsn_mark)
{
	kmem_cache_free(fanotify_mark_cache, fsn_mark);
}

static int fanotify_find_path(int dfd, const char __user *filename,
			      struct path *path, unsigned int flags)
=======
	.compat_ioctl	= compat_ptr_ioctl,
	.llseek		= noop_llseek,
};

static int fanotify_find_path(int dfd, const char __user *filename,
			      struct path *path, unsigned int flags, __u64 mask,
			      unsigned int obj_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;

	pr_debug("%s: dfd=%d filename=%p flags=%x\n", __func__,
		 dfd, filename, flags);

	if (filename == NULL) {
<<<<<<< HEAD
		struct file *file;
		int fput_needed;

		ret = -EBADF;
		file = fget_light(dfd, &fput_needed);
		if (!file)
=======
		struct fd f = fdget(dfd);

		ret = -EBADF;
		if (!f.file)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;

		ret = -ENOTDIR;
		if ((flags & FAN_MARK_ONLYDIR) &&
<<<<<<< HEAD
		    !(S_ISDIR(file->f_path.dentry->d_inode->i_mode))) {
			fput_light(file, fput_needed);
			goto out;
		}

		*path = file->f_path;
		path_get(path);
		fput_light(file, fput_needed);
=======
		    !(S_ISDIR(file_inode(f.file)->i_mode))) {
			fdput(f);
			goto out;
		}

		*path = f.file->f_path;
		path_get(path);
		fdput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		unsigned int lookup_flags = 0;

		if (!(flags & FAN_MARK_DONT_FOLLOW))
			lookup_flags |= LOOKUP_FOLLOW;
		if (flags & FAN_MARK_ONLYDIR)
			lookup_flags |= LOOKUP_DIRECTORY;

		ret = user_path_at(dfd, filename, lookup_flags, path);
		if (ret)
			goto out;
	}

	/* you can only watch an inode if you have read permissions on it */
<<<<<<< HEAD
	ret = inode_permission2(path->mnt, path->dentry->d_inode, MAY_READ);
	if (ret)
		path_put(path);
=======
	ret = path_permission(path, MAY_READ);
	if (ret) {
		path_put(path);
		goto out;
	}

	ret = security_path_notify(path, mask, obj_type);
	if (ret)
		path_put(path);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}

static __u32 fanotify_mark_remove_from_mask(struct fsnotify_mark *fsn_mark,
<<<<<<< HEAD
					    __u32 mask,
					    unsigned int flags)
{
	__u32 oldmask;

	spin_lock(&fsn_mark->lock);
	if (!(flags & FAN_MARK_IGNORED_MASK)) {
		oldmask = fsn_mark->mask;
		fsnotify_set_mark_mask_locked(fsn_mark, (oldmask & ~mask));
	} else {
		oldmask = fsn_mark->ignored_mask;
		fsnotify_set_mark_ignored_mask_locked(fsn_mark, (oldmask & ~mask));
	}
	spin_unlock(&fsn_mark->lock);

	if (!(oldmask & ~mask))
		fsnotify_destroy_mark(fsn_mark);

	return mask & oldmask;
=======
					    __u32 mask, unsigned int flags,
					    __u32 umask, int *destroy)
{
	__u32 oldmask, newmask;

	/* umask bits cannot be removed by user */
	mask &= ~umask;
	spin_lock(&fsn_mark->lock);
	oldmask = fsnotify_calc_mask(fsn_mark);
	if (!(flags & FANOTIFY_MARK_IGNORE_BITS)) {
		fsn_mark->mask &= ~mask;
	} else {
		fsn_mark->ignore_mask &= ~mask;
	}
	newmask = fsnotify_calc_mask(fsn_mark);
	/*
	 * We need to keep the mark around even if remaining mask cannot
	 * result in any events (e.g. mask == FAN_ONDIR) to support incremenal
	 * changes to the mask.
	 * Destroy mark when only umask bits remain.
	 */
	*destroy = !((fsn_mark->mask | fsn_mark->ignore_mask) & ~umask);
	spin_unlock(&fsn_mark->lock);

	return oldmask & ~newmask;
}

static int fanotify_remove_mark(struct fsnotify_group *group,
				fsnotify_connp_t *connp, __u32 mask,
				unsigned int flags, __u32 umask)
{
	struct fsnotify_mark *fsn_mark = NULL;
	__u32 removed;
	int destroy_mark;

	fsnotify_group_lock(group);
	fsn_mark = fsnotify_find_mark(connp, group);
	if (!fsn_mark) {
		fsnotify_group_unlock(group);
		return -ENOENT;
	}

	removed = fanotify_mark_remove_from_mask(fsn_mark, mask, flags,
						 umask, &destroy_mark);
	if (removed & fsnotify_conn_mask(fsn_mark->connector))
		fsnotify_recalc_mask(fsn_mark->connector);
	if (destroy_mark)
		fsnotify_detach_mark(fsn_mark);
	fsnotify_group_unlock(group);
	if (destroy_mark)
		fsnotify_free_mark(fsn_mark);

	/* matches the fsnotify_find_mark() */
	fsnotify_put_mark(fsn_mark);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fanotify_remove_vfsmount_mark(struct fsnotify_group *group,
					 struct vfsmount *mnt, __u32 mask,
<<<<<<< HEAD
					 unsigned int flags)
{
	struct fsnotify_mark *fsn_mark = NULL;
	__u32 removed;

	fsn_mark = fsnotify_find_vfsmount_mark(group, mnt);
	if (!fsn_mark)
		return -ENOENT;

	removed = fanotify_mark_remove_from_mask(fsn_mark, mask, flags);
	fsnotify_put_mark(fsn_mark);
	if (removed & real_mount(mnt)->mnt_fsnotify_mask)
		fsnotify_recalc_vfsmount_mask(mnt);

	return 0;
=======
					 unsigned int flags, __u32 umask)
{
	return fanotify_remove_mark(group, &real_mount(mnt)->mnt_fsnotify_marks,
				    mask, flags, umask);
}

static int fanotify_remove_sb_mark(struct fsnotify_group *group,
				   struct super_block *sb, __u32 mask,
				   unsigned int flags, __u32 umask)
{
	return fanotify_remove_mark(group, &sb->s_fsnotify_marks, mask,
				    flags, umask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fanotify_remove_inode_mark(struct fsnotify_group *group,
				      struct inode *inode, __u32 mask,
<<<<<<< HEAD
				      unsigned int flags)
{
	struct fsnotify_mark *fsn_mark = NULL;
	__u32 removed;

	fsn_mark = fsnotify_find_inode_mark(group, inode);
	if (!fsn_mark)
		return -ENOENT;

	removed = fanotify_mark_remove_from_mask(fsn_mark, mask, flags);
	/* matches the fsnotify_find_inode_mark() */
	fsnotify_put_mark(fsn_mark);
	if (removed & inode->i_fsnotify_mask)
		fsnotify_recalc_inode_mask(inode);
=======
				      unsigned int flags, __u32 umask)
{
	return fanotify_remove_mark(group, &inode->i_fsnotify_marks, mask,
				    flags, umask);
}

static bool fanotify_mark_update_flags(struct fsnotify_mark *fsn_mark,
				       unsigned int fan_flags)
{
	bool want_iref = !(fan_flags & FAN_MARK_EVICTABLE);
	unsigned int ignore = fan_flags & FANOTIFY_MARK_IGNORE_BITS;
	bool recalc = false;

	/*
	 * When using FAN_MARK_IGNORE for the first time, mark starts using
	 * independent event flags in ignore mask.  After that, trying to
	 * update the ignore mask with the old FAN_MARK_IGNORED_MASK API
	 * will result in EEXIST error.
	 */
	if (ignore == FAN_MARK_IGNORE)
		fsn_mark->flags |= FSNOTIFY_MARK_FLAG_HAS_IGNORE_FLAGS;

	/*
	 * Setting FAN_MARK_IGNORED_SURV_MODIFY for the first time may lead to
	 * the removal of the FS_MODIFY bit in calculated mask if it was set
	 * because of an ignore mask that is now going to survive FS_MODIFY.
	 */
	if (ignore && (fan_flags & FAN_MARK_IGNORED_SURV_MODIFY) &&
	    !(fsn_mark->flags & FSNOTIFY_MARK_FLAG_IGNORED_SURV_MODIFY)) {
		fsn_mark->flags |= FSNOTIFY_MARK_FLAG_IGNORED_SURV_MODIFY;
		if (!(fsn_mark->mask & FS_MODIFY))
			recalc = true;
	}

	if (fsn_mark->connector->type != FSNOTIFY_OBJ_TYPE_INODE ||
	    want_iref == !(fsn_mark->flags & FSNOTIFY_MARK_FLAG_NO_IREF))
		return recalc;

	/*
	 * NO_IREF may be removed from a mark, but not added.
	 * When removed, fsnotify_recalc_mask() will take the inode ref.
	 */
	WARN_ON_ONCE(!want_iref);
	fsn_mark->flags &= ~FSNOTIFY_MARK_FLAG_NO_IREF;

	return true;
}

static bool fanotify_mark_add_to_mask(struct fsnotify_mark *fsn_mark,
				      __u32 mask, unsigned int fan_flags)
{
	bool recalc;

	spin_lock(&fsn_mark->lock);
	if (!(fan_flags & FANOTIFY_MARK_IGNORE_BITS))
		fsn_mark->mask |= mask;
	else
		fsn_mark->ignore_mask |= mask;

	recalc = fsnotify_calc_mask(fsn_mark) &
		~fsnotify_conn_mask(fsn_mark->connector);

	recalc |= fanotify_mark_update_flags(fsn_mark, fan_flags);
	spin_unlock(&fsn_mark->lock);

	return recalc;
}

struct fan_fsid {
	struct super_block *sb;
	__kernel_fsid_t id;
	bool weak;
};

static int fanotify_set_mark_fsid(struct fsnotify_group *group,
				  struct fsnotify_mark *mark,
				  struct fan_fsid *fsid)
{
	struct fsnotify_mark_connector *conn;
	struct fsnotify_mark *old;
	struct super_block *old_sb = NULL;

	FANOTIFY_MARK(mark)->fsid = fsid->id;
	mark->flags |= FSNOTIFY_MARK_FLAG_HAS_FSID;
	if (fsid->weak)
		mark->flags |= FSNOTIFY_MARK_FLAG_WEAK_FSID;

	/* First mark added will determine if group is single or multi fsid */
	if (list_empty(&group->marks_list))
		return 0;

	/* Find sb of an existing mark */
	list_for_each_entry(old, &group->marks_list, g_list) {
		conn = READ_ONCE(old->connector);
		if (!conn)
			continue;
		old_sb = fsnotify_connector_sb(conn);
		if (old_sb)
			break;
	}

	/* Only detached marks left? */
	if (!old_sb)
		return 0;

	/* Do not allow mixing of marks with weak and strong fsid */
	if ((mark->flags ^ old->flags) & FSNOTIFY_MARK_FLAG_WEAK_FSID)
		return -EXDEV;

	/* Allow mixing of marks with strong fsid from different fs */
	if (!fsid->weak)
		return 0;

	/* Do not allow mixing marks with weak fsid from different fs */
	if (old_sb != fsid->sb)
		return -EXDEV;

	/* Do not allow mixing marks from different btrfs sub-volumes */
	if (!fanotify_fsid_equal(&FANOTIFY_MARK(old)->fsid,
				 &FANOTIFY_MARK(mark)->fsid))
		return -EXDEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static __u32 fanotify_mark_add_to_mask(struct fsnotify_mark *fsn_mark,
				       __u32 mask,
				       unsigned int flags)
{
	__u32 oldmask = -1;

	spin_lock(&fsn_mark->lock);
	if (!(flags & FAN_MARK_IGNORED_MASK)) {
		oldmask = fsn_mark->mask;
		fsnotify_set_mark_mask_locked(fsn_mark, (oldmask | mask));
	} else {
		__u32 tmask = fsn_mark->ignored_mask | mask;
		fsnotify_set_mark_ignored_mask_locked(fsn_mark, tmask);
		if (flags & FAN_MARK_IGNORED_SURV_MODIFY)
			fsn_mark->flags |= FSNOTIFY_MARK_FLAG_IGNORED_SURV_MODIFY;
	}

	if (!(flags & FAN_MARK_ONDIR)) {
		__u32 tmask = fsn_mark->ignored_mask | FAN_ONDIR;
		fsnotify_set_mark_ignored_mask_locked(fsn_mark, tmask);
	}

	spin_unlock(&fsn_mark->lock);

	return mask & ~oldmask;
=======
static struct fsnotify_mark *fanotify_add_new_mark(struct fsnotify_group *group,
						   fsnotify_connp_t *connp,
						   unsigned int obj_type,
						   unsigned int fan_flags,
						   struct fan_fsid *fsid)
{
	struct ucounts *ucounts = group->fanotify_data.ucounts;
	struct fanotify_mark *fan_mark;
	struct fsnotify_mark *mark;
	int ret;

	/*
	 * Enforce per user marks limits per user in all containing user ns.
	 * A group with FAN_UNLIMITED_MARKS does not contribute to mark count
	 * in the limited groups account.
	 */
	if (!FAN_GROUP_FLAG(group, FAN_UNLIMITED_MARKS) &&
	    !inc_ucount(ucounts->ns, ucounts->uid, UCOUNT_FANOTIFY_MARKS))
		return ERR_PTR(-ENOSPC);

	fan_mark = kmem_cache_alloc(fanotify_mark_cache, GFP_KERNEL);
	if (!fan_mark) {
		ret = -ENOMEM;
		goto out_dec_ucounts;
	}

	mark = &fan_mark->fsn_mark;
	fsnotify_init_mark(mark, group);
	if (fan_flags & FAN_MARK_EVICTABLE)
		mark->flags |= FSNOTIFY_MARK_FLAG_NO_IREF;

	/* Cache fsid of filesystem containing the marked object */
	if (fsid) {
		ret = fanotify_set_mark_fsid(group, mark, fsid);
		if (ret)
			goto out_put_mark;
	} else {
		fan_mark->fsid.val[0] = fan_mark->fsid.val[1] = 0;
	}

	ret = fsnotify_add_mark_locked(mark, connp, obj_type, 0);
	if (ret)
		goto out_put_mark;

	return mark;

out_put_mark:
	fsnotify_put_mark(mark);
out_dec_ucounts:
	if (!FAN_GROUP_FLAG(group, FAN_UNLIMITED_MARKS))
		dec_ucount(ucounts, UCOUNT_FANOTIFY_MARKS);
	return ERR_PTR(ret);
}

static int fanotify_group_init_error_pool(struct fsnotify_group *group)
{
	if (mempool_initialized(&group->fanotify_data.error_events_pool))
		return 0;

	return mempool_init_kmalloc_pool(&group->fanotify_data.error_events_pool,
					 FANOTIFY_DEFAULT_FEE_POOL_SIZE,
					 sizeof(struct fanotify_error_event));
}

static int fanotify_may_update_existing_mark(struct fsnotify_mark *fsn_mark,
					      unsigned int fan_flags)
{
	/*
	 * Non evictable mark cannot be downgraded to evictable mark.
	 */
	if (fan_flags & FAN_MARK_EVICTABLE &&
	    !(fsn_mark->flags & FSNOTIFY_MARK_FLAG_NO_IREF))
		return -EEXIST;

	/*
	 * New ignore mask semantics cannot be downgraded to old semantics.
	 */
	if (fan_flags & FAN_MARK_IGNORED_MASK &&
	    fsn_mark->flags & FSNOTIFY_MARK_FLAG_HAS_IGNORE_FLAGS)
		return -EEXIST;

	/*
	 * An ignore mask that survives modify could never be downgraded to not
	 * survive modify.  With new FAN_MARK_IGNORE semantics we make that rule
	 * explicit and return an error when trying to update the ignore mask
	 * without the original FAN_MARK_IGNORED_SURV_MODIFY value.
	 */
	if (fan_flags & FAN_MARK_IGNORE &&
	    !(fan_flags & FAN_MARK_IGNORED_SURV_MODIFY) &&
	    fsn_mark->flags & FSNOTIFY_MARK_FLAG_IGNORED_SURV_MODIFY)
		return -EEXIST;

	return 0;
}

static int fanotify_add_mark(struct fsnotify_group *group,
			     fsnotify_connp_t *connp, unsigned int obj_type,
			     __u32 mask, unsigned int fan_flags,
			     struct fan_fsid *fsid)
{
	struct fsnotify_mark *fsn_mark;
	bool recalc;
	int ret = 0;

	fsnotify_group_lock(group);
	fsn_mark = fsnotify_find_mark(connp, group);
	if (!fsn_mark) {
		fsn_mark = fanotify_add_new_mark(group, connp, obj_type,
						 fan_flags, fsid);
		if (IS_ERR(fsn_mark)) {
			fsnotify_group_unlock(group);
			return PTR_ERR(fsn_mark);
		}
	}

	/*
	 * Check if requested mark flags conflict with an existing mark flags.
	 */
	ret = fanotify_may_update_existing_mark(fsn_mark, fan_flags);
	if (ret)
		goto out;

	/*
	 * Error events are pre-allocated per group, only if strictly
	 * needed (i.e. FAN_FS_ERROR was requested).
	 */
	if (!(fan_flags & FANOTIFY_MARK_IGNORE_BITS) &&
	    (mask & FAN_FS_ERROR)) {
		ret = fanotify_group_init_error_pool(group);
		if (ret)
			goto out;
	}

	recalc = fanotify_mark_add_to_mask(fsn_mark, mask, fan_flags);
	if (recalc)
		fsnotify_recalc_mask(fsn_mark->connector);

out:
	fsnotify_group_unlock(group);

	fsnotify_put_mark(fsn_mark);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fanotify_add_vfsmount_mark(struct fsnotify_group *group,
				      struct vfsmount *mnt, __u32 mask,
<<<<<<< HEAD
				      unsigned int flags)
{
	struct fsnotify_mark *fsn_mark;
	__u32 added;
	int ret = 0;

	fsn_mark = fsnotify_find_vfsmount_mark(group, mnt);
	if (!fsn_mark) {
		if (atomic_read(&group->num_marks) > group->fanotify_data.max_marks)
			return -ENOSPC;

		fsn_mark = kmem_cache_alloc(fanotify_mark_cache, GFP_KERNEL);
		if (!fsn_mark)
			return -ENOMEM;

		fsnotify_init_mark(fsn_mark, fanotify_free_mark);
		ret = fsnotify_add_mark(fsn_mark, group, NULL, mnt, 0);
		if (ret)
			goto err;
	}
	added = fanotify_mark_add_to_mask(fsn_mark, mask, flags);

	if (added & ~real_mount(mnt)->mnt_fsnotify_mask)
		fsnotify_recalc_vfsmount_mask(mnt);
err:
	fsnotify_put_mark(fsn_mark);
	return ret;
=======
				      unsigned int flags, struct fan_fsid *fsid)
{
	return fanotify_add_mark(group, &real_mount(mnt)->mnt_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_VFSMOUNT, mask, flags, fsid);
}

static int fanotify_add_sb_mark(struct fsnotify_group *group,
				struct super_block *sb, __u32 mask,
				unsigned int flags, struct fan_fsid *fsid)
{
	return fanotify_add_mark(group, &sb->s_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_SB, mask, flags, fsid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fanotify_add_inode_mark(struct fsnotify_group *group,
				   struct inode *inode, __u32 mask,
<<<<<<< HEAD
				   unsigned int flags)
{
	struct fsnotify_mark *fsn_mark;
	__u32 added;
	int ret = 0;

=======
				   unsigned int flags, struct fan_fsid *fsid)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pr_debug("%s: group=%p inode=%p\n", __func__, group, inode);

	/*
	 * If some other task has this inode open for write we should not add
<<<<<<< HEAD
	 * an ignored mark, unless that ignored mark is supposed to survive
	 * modification changes anyway.
	 */
	if ((flags & FAN_MARK_IGNORED_MASK) &&
	    !(flags & FAN_MARK_IGNORED_SURV_MODIFY) &&
	    (atomic_read(&inode->i_writecount) > 0))
		return 0;

	fsn_mark = fsnotify_find_inode_mark(group, inode);
	if (!fsn_mark) {
		if (atomic_read(&group->num_marks) > group->fanotify_data.max_marks)
			return -ENOSPC;

		fsn_mark = kmem_cache_alloc(fanotify_mark_cache, GFP_KERNEL);
		if (!fsn_mark)
			return -ENOMEM;

		fsnotify_init_mark(fsn_mark, fanotify_free_mark);
		ret = fsnotify_add_mark(fsn_mark, group, inode, NULL, 0);
		if (ret)
			goto err;
	}
	added = fanotify_mark_add_to_mask(fsn_mark, mask, flags);

	if (added & ~inode->i_fsnotify_mask)
		fsnotify_recalc_inode_mask(inode);
err:
	fsnotify_put_mark(fsn_mark);
	return ret;
=======
	 * an ignore mask, unless that ignore mask is supposed to survive
	 * modification changes anyway.
	 */
	if ((flags & FANOTIFY_MARK_IGNORE_BITS) &&
	    !(flags & FAN_MARK_IGNORED_SURV_MODIFY) &&
	    inode_is_open_for_write(inode))
		return 0;

	return fanotify_add_mark(group, &inode->i_fsnotify_marks,
				 FSNOTIFY_OBJ_TYPE_INODE, mask, flags, fsid);
}

static struct fsnotify_event *fanotify_alloc_overflow_event(void)
{
	struct fanotify_event *oevent;

	oevent = kmalloc(sizeof(*oevent), GFP_KERNEL_ACCOUNT);
	if (!oevent)
		return NULL;

	fanotify_init_event(oevent, 0, FS_Q_OVERFLOW);
	oevent->type = FANOTIFY_EVENT_TYPE_OVERFLOW;

	return &oevent->fse;
}

static struct hlist_head *fanotify_alloc_merge_hash(void)
{
	struct hlist_head *hash;

	hash = kmalloc(sizeof(struct hlist_head) << FANOTIFY_HTABLE_BITS,
		       GFP_KERNEL_ACCOUNT);
	if (!hash)
		return NULL;

	__hash_init(hash, FANOTIFY_HTABLE_SIZE);

	return hash;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* fanotify syscalls */
SYSCALL_DEFINE2(fanotify_init, unsigned int, flags, unsigned int, event_f_flags)
{
	struct fsnotify_group *group;
	int f_flags, fd;
<<<<<<< HEAD
	struct user_struct *user;

	pr_debug("%s: flags=%d event_f_flags=%d\n",
		__func__, flags, event_f_flags);

	if (!capable(CAP_SYS_ADMIN))
		return -EPERM;

	if (flags & ~FAN_ALL_INIT_FLAGS)
=======
	unsigned int fid_mode = flags & FANOTIFY_FID_BITS;
	unsigned int class = flags & FANOTIFY_CLASS_BITS;
	unsigned int internal_flags = 0;

	pr_debug("%s: flags=%x event_f_flags=%x\n",
		 __func__, flags, event_f_flags);

	if (!capable(CAP_SYS_ADMIN)) {
		/*
		 * An unprivileged user can setup an fanotify group with
		 * limited functionality - an unprivileged group is limited to
		 * notification events with file handles and it cannot use
		 * unlimited queue/marks.
		 */
		if ((flags & FANOTIFY_ADMIN_INIT_FLAGS) || !fid_mode)
			return -EPERM;

		/*
		 * Setting the internal flag FANOTIFY_UNPRIV on the group
		 * prevents setting mount/filesystem marks on this group and
		 * prevents reporting pid and open fd in events.
		 */
		internal_flags |= FANOTIFY_UNPRIV;
	}

#ifdef CONFIG_AUDITSYSCALL
	if (flags & ~(FANOTIFY_INIT_FLAGS | FAN_ENABLE_AUDIT))
#else
	if (flags & ~FANOTIFY_INIT_FLAGS)
#endif
		return -EINVAL;

	/*
	 * A pidfd can only be returned for a thread-group leader; thus
	 * FAN_REPORT_PIDFD and FAN_REPORT_TID need to remain mutually
	 * exclusive.
	 */
	if ((flags & FAN_REPORT_PIDFD) && (flags & FAN_REPORT_TID))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (event_f_flags & ~FANOTIFY_INIT_ALL_EVENT_F_BITS)
		return -EINVAL;

	switch (event_f_flags & O_ACCMODE) {
	case O_RDONLY:
	case O_RDWR:
	case O_WRONLY:
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	user = get_current_user();
	if (atomic_read(&user->fanotify_listeners) > FANOTIFY_DEFAULT_MAX_LISTENERS) {
		free_uid(user);
		return -EMFILE;
	}

	f_flags = O_RDWR | FMODE_NONOTIFY;
=======
	if (fid_mode && class != FAN_CLASS_NOTIF)
		return -EINVAL;

	/*
	 * Child name is reported with parent fid so requires dir fid.
	 * We can report both child fid and dir fid with or without name.
	 */
	if ((fid_mode & FAN_REPORT_NAME) && !(fid_mode & FAN_REPORT_DIR_FID))
		return -EINVAL;

	/*
	 * FAN_REPORT_TARGET_FID requires FAN_REPORT_NAME and FAN_REPORT_FID
	 * and is used as an indication to report both dir and child fid on all
	 * dirent events.
	 */
	if ((fid_mode & FAN_REPORT_TARGET_FID) &&
	    (!(fid_mode & FAN_REPORT_NAME) || !(fid_mode & FAN_REPORT_FID)))
		return -EINVAL;

	f_flags = O_RDWR | __FMODE_NONOTIFY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (flags & FAN_CLOEXEC)
		f_flags |= O_CLOEXEC;
	if (flags & FAN_NONBLOCK)
		f_flags |= O_NONBLOCK;

	/* fsnotify_alloc_group takes a ref.  Dropped in fanotify_release */
<<<<<<< HEAD
	group = fsnotify_alloc_group(&fanotify_fsnotify_ops);
	if (IS_ERR(group)) {
		free_uid(user);
		return PTR_ERR(group);
	}

	group->fanotify_data.user = user;
	atomic_inc(&user->fanotify_listeners);

	group->fanotify_data.f_flags = event_f_flags;
#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	mutex_init(&group->fanotify_data.access_mutex);
	init_waitqueue_head(&group->fanotify_data.access_waitq);
	INIT_LIST_HEAD(&group->fanotify_data.access_list);
	atomic_set(&group->fanotify_data.bypass_perm, 0);
#endif
	switch (flags & FAN_ALL_CLASS_BITS) {
=======
	group = fsnotify_alloc_group(&fanotify_fsnotify_ops,
				     FSNOTIFY_GROUP_USER | FSNOTIFY_GROUP_NOFS);
	if (IS_ERR(group)) {
		return PTR_ERR(group);
	}

	/* Enforce groups limits per user in all containing user ns */
	group->fanotify_data.ucounts = inc_ucount(current_user_ns(),
						  current_euid(),
						  UCOUNT_FANOTIFY_GROUPS);
	if (!group->fanotify_data.ucounts) {
		fd = -EMFILE;
		goto out_destroy_group;
	}

	group->fanotify_data.flags = flags | internal_flags;
	group->memcg = get_mem_cgroup_from_mm(current->mm);

	group->fanotify_data.merge_hash = fanotify_alloc_merge_hash();
	if (!group->fanotify_data.merge_hash) {
		fd = -ENOMEM;
		goto out_destroy_group;
	}

	group->overflow_event = fanotify_alloc_overflow_event();
	if (unlikely(!group->overflow_event)) {
		fd = -ENOMEM;
		goto out_destroy_group;
	}

	if (force_o_largefile())
		event_f_flags |= O_LARGEFILE;
	group->fanotify_data.f_flags = event_f_flags;
	init_waitqueue_head(&group->fanotify_data.access_waitq);
	INIT_LIST_HEAD(&group->fanotify_data.access_list);
	switch (class) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case FAN_CLASS_NOTIF:
		group->priority = FS_PRIO_0;
		break;
	case FAN_CLASS_CONTENT:
		group->priority = FS_PRIO_1;
		break;
	case FAN_CLASS_PRE_CONTENT:
		group->priority = FS_PRIO_2;
		break;
	default:
		fd = -EINVAL;
<<<<<<< HEAD
		goto out_put_group;
=======
		goto out_destroy_group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (flags & FAN_UNLIMITED_QUEUE) {
		fd = -EPERM;
		if (!capable(CAP_SYS_ADMIN))
<<<<<<< HEAD
			goto out_put_group;
		group->max_events = UINT_MAX;
	} else {
		group->max_events = FANOTIFY_DEFAULT_MAX_EVENTS;
=======
			goto out_destroy_group;
		group->max_events = UINT_MAX;
	} else {
		group->max_events = fanotify_max_queued_events;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (flags & FAN_UNLIMITED_MARKS) {
		fd = -EPERM;
		if (!capable(CAP_SYS_ADMIN))
<<<<<<< HEAD
			goto out_put_group;
		group->fanotify_data.max_marks = UINT_MAX;
	} else {
		group->fanotify_data.max_marks = FANOTIFY_DEFAULT_MAX_MARKS;
=======
			goto out_destroy_group;
	}

	if (flags & FAN_ENABLE_AUDIT) {
		fd = -EPERM;
		if (!capable(CAP_AUDIT_WRITE))
			goto out_destroy_group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	fd = anon_inode_getfd("[fanotify]", &fanotify_fops, group, f_flags);
	if (fd < 0)
<<<<<<< HEAD
		goto out_put_group;

	return fd;

out_put_group:
	fsnotify_put_group(group);
	return fd;
}

SYSCALL_DEFINE(fanotify_mark)(int fanotify_fd, unsigned int flags,
			      __u64 mask, int dfd,
			      const char  __user * pathname)
=======
		goto out_destroy_group;

	return fd;

out_destroy_group:
	fsnotify_destroy_group(group);
	return fd;
}

static int fanotify_test_fsid(struct dentry *dentry, unsigned int flags,
			      struct fan_fsid *fsid)
{
	unsigned int mark_type = flags & FANOTIFY_MARK_TYPE_BITS;
	__kernel_fsid_t root_fsid;
	int err;

	/*
	 * Make sure dentry is not of a filesystem with zero fsid (e.g. fuse).
	 */
	err = vfs_get_fsid(dentry, &fsid->id);
	if (err)
		return err;

	fsid->sb = dentry->d_sb;
	if (!fsid->id.val[0] && !fsid->id.val[1]) {
		err = -ENODEV;
		goto weak;
	}

	/*
	 * Make sure dentry is not of a filesystem subvolume (e.g. btrfs)
	 * which uses a different fsid than sb root.
	 */
	err = vfs_get_fsid(dentry->d_sb->s_root, &root_fsid);
	if (err)
		return err;

	if (!fanotify_fsid_equal(&root_fsid, &fsid->id)) {
		err = -EXDEV;
		goto weak;
	}

	fsid->weak = false;
	return 0;

weak:
	/* Allow weak fsid when marking inodes */
	fsid->weak = true;
	return (mark_type == FAN_MARK_INODE) ? 0 : err;
}

/* Check if filesystem can encode a unique fid */
static int fanotify_test_fid(struct dentry *dentry, unsigned int flags)
{
	unsigned int mark_type = flags & FANOTIFY_MARK_TYPE_BITS;
	const struct export_operations *nop = dentry->d_sb->s_export_op;

	/*
	 * We need to make sure that the filesystem supports encoding of
	 * file handles so user can use name_to_handle_at() to compare fids
	 * reported with events to the file handle of watched objects.
	 */
	if (!exportfs_can_encode_fid(nop))
		return -EOPNOTSUPP;

	/*
	 * For sb/mount mark, we also need to make sure that the filesystem
	 * supports decoding file handles, so user has a way to map back the
	 * reported fids to filesystem objects.
	 */
	if (mark_type != FAN_MARK_INODE && !exportfs_can_decode_fh(nop))
		return -EOPNOTSUPP;

	return 0;
}

static int fanotify_events_supported(struct fsnotify_group *group,
				     const struct path *path, __u64 mask,
				     unsigned int flags)
{
	unsigned int mark_type = flags & FANOTIFY_MARK_TYPE_BITS;
	/* Strict validation of events in non-dir inode mask with v5.17+ APIs */
	bool strict_dir_events = FAN_GROUP_FLAG(group, FAN_REPORT_TARGET_FID) ||
				 (mask & FAN_RENAME) ||
				 (flags & FAN_MARK_IGNORE);

	/*
	 * Some filesystems such as 'proc' acquire unusual locks when opening
	 * files. For them fanotify permission events have high chances of
	 * deadlocking the system - open done when reporting fanotify event
	 * blocks on this "unusual" lock while another process holding the lock
	 * waits for fanotify permission event to be answered. Just disallow
	 * permission events for such filesystems.
	 */
	if (mask & FANOTIFY_PERM_EVENTS &&
	    path->mnt->mnt_sb->s_type->fs_flags & FS_DISALLOW_NOTIFY_PERM)
		return -EINVAL;

	/*
	 * mount and sb marks are not allowed on kernel internal pseudo fs,
	 * like pipe_mnt, because that would subscribe to events on all the
	 * anonynous pipes in the system.
	 *
	 * SB_NOUSER covers all of the internal pseudo fs whose objects are not
	 * exposed to user's mount namespace, but there are other SB_KERNMOUNT
	 * fs, like nsfs, debugfs, for which the value of allowing sb and mount
	 * mark is questionable. For now we leave them alone.
	 */
	if (mark_type != FAN_MARK_INODE &&
	    path->mnt->mnt_sb->s_flags & SB_NOUSER)
		return -EINVAL;

	/*
	 * We shouldn't have allowed setting dirent events and the directory
	 * flags FAN_ONDIR and FAN_EVENT_ON_CHILD in mask of non-dir inode,
	 * but because we always allowed it, error only when using new APIs.
	 */
	if (strict_dir_events && mark_type == FAN_MARK_INODE &&
	    !d_is_dir(path->dentry) && (mask & FANOTIFY_DIRONLY_EVENT_BITS))
		return -ENOTDIR;

	return 0;
}

static int do_fanotify_mark(int fanotify_fd, unsigned int flags, __u64 mask,
			    int dfd, const char  __user *pathname)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *inode = NULL;
	struct vfsmount *mnt = NULL;
	struct fsnotify_group *group;
<<<<<<< HEAD
	struct file *filp;
	struct path path;
	int ret, fput_needed;
=======
	struct fd f;
	struct path path;
	struct fan_fsid __fsid, *fsid = NULL;
	u32 valid_mask = FANOTIFY_EVENTS | FANOTIFY_EVENT_FLAGS;
	unsigned int mark_type = flags & FANOTIFY_MARK_TYPE_BITS;
	unsigned int mark_cmd = flags & FANOTIFY_MARK_CMD_BITS;
	unsigned int ignore = flags & FANOTIFY_MARK_IGNORE_BITS;
	unsigned int obj_type, fid_mode;
	u32 umask = 0;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("%s: fanotify_fd=%d flags=%x dfd=%d pathname=%p mask=%llx\n",
		 __func__, fanotify_fd, flags, dfd, pathname, mask);

	/* we only use the lower 32 bits as of right now. */
<<<<<<< HEAD
	if (mask & ((__u64)0xffffffff << 32))
		return -EINVAL;

	if (flags & ~FAN_ALL_MARK_FLAGS)
		return -EINVAL;
	switch (flags & (FAN_MARK_ADD | FAN_MARK_REMOVE | FAN_MARK_FLUSH)) {
	case FAN_MARK_ADD:		/* fallthrough */
	case FAN_MARK_REMOVE:
		if (!mask)
			return -EINVAL;
	case FAN_MARK_FLUSH:
=======
	if (upper_32_bits(mask))
		return -EINVAL;

	if (flags & ~FANOTIFY_MARK_FLAGS)
		return -EINVAL;

	switch (mark_type) {
	case FAN_MARK_INODE:
		obj_type = FSNOTIFY_OBJ_TYPE_INODE;
		break;
	case FAN_MARK_MOUNT:
		obj_type = FSNOTIFY_OBJ_TYPE_VFSMOUNT;
		break;
	case FAN_MARK_FILESYSTEM:
		obj_type = FSNOTIFY_OBJ_TYPE_SB;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	if (mask & FAN_ONDIR) {
		flags |= FAN_MARK_ONDIR;
		mask &= ~FAN_ONDIR;
	}

#ifdef CONFIG_FANOTIFY_ACCESS_PERMISSIONS
	if (mask & ~(FAN_ALL_EVENTS | FAN_ALL_PERM_EVENTS | FAN_EVENT_ON_CHILD))
#else
	if (mask & ~(FAN_ALL_EVENTS | FAN_EVENT_ON_CHILD))
#endif
		return -EINVAL;

	filp = fget_light(fanotify_fd, &fput_needed);
	if (unlikely(!filp))
=======
	switch (mark_cmd) {
	case FAN_MARK_ADD:
	case FAN_MARK_REMOVE:
		if (!mask)
			return -EINVAL;
		break;
	case FAN_MARK_FLUSH:
		if (flags & ~(FANOTIFY_MARK_TYPE_BITS | FAN_MARK_FLUSH))
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}

	if (IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS))
		valid_mask |= FANOTIFY_PERM_EVENTS;

	if (mask & ~valid_mask)
		return -EINVAL;


	/* We don't allow FAN_MARK_IGNORE & FAN_MARK_IGNORED_MASK together */
	if (ignore == (FAN_MARK_IGNORE | FAN_MARK_IGNORED_MASK))
		return -EINVAL;

	/*
	 * Event flags (FAN_ONDIR, FAN_EVENT_ON_CHILD) have no effect with
	 * FAN_MARK_IGNORED_MASK.
	 */
	if (ignore == FAN_MARK_IGNORED_MASK) {
		mask &= ~FANOTIFY_EVENT_FLAGS;
		umask = FANOTIFY_EVENT_FLAGS;
	}

	f = fdget(fanotify_fd);
	if (unlikely(!f.file))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EBADF;

	/* verify that this is indeed an fanotify instance */
	ret = -EINVAL;
<<<<<<< HEAD
	if (unlikely(filp->f_op != &fanotify_fops))
		goto fput_and_out;
	group = filp->private_data;
=======
	if (unlikely(f.file->f_op != &fanotify_fops))
		goto fput_and_out;
	group = f.file->private_data;

	/*
	 * An unprivileged user is not allowed to setup mount nor filesystem
	 * marks.  This also includes setting up such marks by a group that
	 * was initialized by an unprivileged user.
	 */
	ret = -EPERM;
	if ((!capable(CAP_SYS_ADMIN) ||
	     FAN_GROUP_FLAG(group, FANOTIFY_UNPRIV)) &&
	    mark_type != FAN_MARK_INODE)
		goto fput_and_out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * group->priority == FS_PRIO_0 == FAN_CLASS_NOTIF.  These are not
	 * allowed to set permissions events.
	 */
	ret = -EINVAL;
<<<<<<< HEAD
	if (mask & FAN_ALL_PERM_EVENTS &&
	    group->priority == FS_PRIO_0)
		goto fput_and_out;

	ret = fanotify_find_path(dfd, pathname, &path, flags);
	if (ret)
		goto fput_and_out;

	/* inode held in place by reference to path; group by fget on fd */
	if (!(flags & FAN_MARK_MOUNT))
=======
	if (mask & FANOTIFY_PERM_EVENTS &&
	    group->priority == FS_PRIO_0)
		goto fput_and_out;

	if (mask & FAN_FS_ERROR &&
	    mark_type != FAN_MARK_FILESYSTEM)
		goto fput_and_out;

	/*
	 * Evictable is only relevant for inode marks, because only inode object
	 * can be evicted on memory pressure.
	 */
	if (flags & FAN_MARK_EVICTABLE &&
	     mark_type != FAN_MARK_INODE)
		goto fput_and_out;

	/*
	 * Events that do not carry enough information to report
	 * event->fd require a group that supports reporting fid.  Those
	 * events are not supported on a mount mark, because they do not
	 * carry enough information (i.e. path) to be filtered by mount
	 * point.
	 */
	fid_mode = FAN_GROUP_FLAG(group, FANOTIFY_FID_BITS);
	if (mask & ~(FANOTIFY_FD_EVENTS|FANOTIFY_EVENT_FLAGS) &&
	    (!fid_mode || mark_type == FAN_MARK_MOUNT))
		goto fput_and_out;

	/*
	 * FAN_RENAME uses special info type records to report the old and
	 * new parent+name.  Reporting only old and new parent id is less
	 * useful and was not implemented.
	 */
	if (mask & FAN_RENAME && !(fid_mode & FAN_REPORT_NAME))
		goto fput_and_out;

	if (mark_cmd == FAN_MARK_FLUSH) {
		ret = 0;
		if (mark_type == FAN_MARK_MOUNT)
			fsnotify_clear_vfsmount_marks_by_group(group);
		else if (mark_type == FAN_MARK_FILESYSTEM)
			fsnotify_clear_sb_marks_by_group(group);
		else
			fsnotify_clear_inode_marks_by_group(group);
		goto fput_and_out;
	}

	ret = fanotify_find_path(dfd, pathname, &path, flags,
			(mask & ALL_FSNOTIFY_EVENTS), obj_type);
	if (ret)
		goto fput_and_out;

	if (mark_cmd == FAN_MARK_ADD) {
		ret = fanotify_events_supported(group, &path, mask, flags);
		if (ret)
			goto path_put_and_out;
	}

	if (fid_mode) {
		ret = fanotify_test_fsid(path.dentry, flags, &__fsid);
		if (ret)
			goto path_put_and_out;

		ret = fanotify_test_fid(path.dentry, flags);
		if (ret)
			goto path_put_and_out;

		fsid = &__fsid;
	}

	/* inode held in place by reference to path; group by fget on fd */
	if (mark_type == FAN_MARK_INODE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inode = path.dentry->d_inode;
	else
		mnt = path.mnt;

<<<<<<< HEAD
	/* create/update an inode mark */
	switch (flags & (FAN_MARK_ADD | FAN_MARK_REMOVE | FAN_MARK_FLUSH)) {
	case FAN_MARK_ADD:
		if (flags & FAN_MARK_MOUNT)
			ret = fanotify_add_vfsmount_mark(group, mnt, mask, flags);
		else
			ret = fanotify_add_inode_mark(group, inode, mask, flags);
		break;
	case FAN_MARK_REMOVE:
		if (flags & FAN_MARK_MOUNT)
			ret = fanotify_remove_vfsmount_mark(group, mnt, mask, flags);
		else
			ret = fanotify_remove_inode_mark(group, inode, mask, flags);
		break;
	case FAN_MARK_FLUSH:
		if (flags & FAN_MARK_MOUNT)
			fsnotify_clear_vfsmount_marks_by_group(group);
		else
			fsnotify_clear_inode_marks_by_group(group);
=======
	ret = mnt ? -EINVAL : -EISDIR;
	/* FAN_MARK_IGNORE requires SURV_MODIFY for sb/mount/dir marks */
	if (mark_cmd == FAN_MARK_ADD && ignore == FAN_MARK_IGNORE &&
	    (mnt || S_ISDIR(inode->i_mode)) &&
	    !(flags & FAN_MARK_IGNORED_SURV_MODIFY))
		goto path_put_and_out;

	/* Mask out FAN_EVENT_ON_CHILD flag for sb/mount/non-dir marks */
	if (mnt || !S_ISDIR(inode->i_mode)) {
		mask &= ~FAN_EVENT_ON_CHILD;
		umask = FAN_EVENT_ON_CHILD;
		/*
		 * If group needs to report parent fid, register for getting
		 * events with parent/name info for non-directory.
		 */
		if ((fid_mode & FAN_REPORT_DIR_FID) &&
		    (flags & FAN_MARK_ADD) && !ignore)
			mask |= FAN_EVENT_ON_CHILD;
	}

	/* create/update an inode mark */
	switch (mark_cmd) {
	case FAN_MARK_ADD:
		if (mark_type == FAN_MARK_MOUNT)
			ret = fanotify_add_vfsmount_mark(group, mnt, mask,
							 flags, fsid);
		else if (mark_type == FAN_MARK_FILESYSTEM)
			ret = fanotify_add_sb_mark(group, mnt->mnt_sb, mask,
						   flags, fsid);
		else
			ret = fanotify_add_inode_mark(group, inode, mask,
						      flags, fsid);
		break;
	case FAN_MARK_REMOVE:
		if (mark_type == FAN_MARK_MOUNT)
			ret = fanotify_remove_vfsmount_mark(group, mnt, mask,
							    flags, umask);
		else if (mark_type == FAN_MARK_FILESYSTEM)
			ret = fanotify_remove_sb_mark(group, mnt->mnt_sb, mask,
						      flags, umask);
		else
			ret = fanotify_remove_inode_mark(group, inode, mask,
							 flags, umask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		ret = -EINVAL;
	}

<<<<<<< HEAD
	path_put(&path);
fput_and_out:
	fput_light(filp, fput_needed);
	return ret;
}

#ifdef CONFIG_HAVE_SYSCALL_WRAPPERS
asmlinkage long SyS_fanotify_mark(long fanotify_fd, long flags, __u64 mask,
				  long dfd, long pathname)
{
	return SYSC_fanotify_mark((int) fanotify_fd, (unsigned int) flags,
				  mask, (int) dfd,
				  (const char  __user *) pathname);
}
SYSCALL_ALIAS(sys_fanotify_mark, SyS_fanotify_mark);
=======
path_put_and_out:
	path_put(&path);
fput_and_out:
	fdput(f);
	return ret;
}

#ifndef CONFIG_ARCH_SPLIT_ARG64
SYSCALL_DEFINE5(fanotify_mark, int, fanotify_fd, unsigned int, flags,
			      __u64, mask, int, dfd,
			      const char  __user *, pathname)
{
	return do_fanotify_mark(fanotify_fd, flags, mask, dfd, pathname);
}
#endif

#if defined(CONFIG_ARCH_SPLIT_ARG64) || defined(CONFIG_COMPAT)
SYSCALL32_DEFINE6(fanotify_mark,
				int, fanotify_fd, unsigned int, flags,
				SC_ARG64(mask), int, dfd,
				const char  __user *, pathname)
{
	return do_fanotify_mark(fanotify_fd, flags, SC_VAL64(__u64, mask),
				dfd, pathname);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * fanotify_user_setup - Our initialization function.  Note that we cannot return
 * error because we have compiled-in VFS hooks.  So an (unlikely) failure here
 * must result in panic().
 */
static int __init fanotify_user_setup(void)
{
<<<<<<< HEAD
	fanotify_mark_cache = KMEM_CACHE(fsnotify_mark, SLAB_PANIC);
	fanotify_response_event_cache = KMEM_CACHE(fanotify_response_event,
						   SLAB_PANIC);
=======
	struct sysinfo si;
	int max_marks;

	si_meminfo(&si);
	/*
	 * Allow up to 1% of addressable memory to be accounted for per user
	 * marks limited to the range [8192, 1048576]. mount and sb marks are
	 * a lot cheaper than inode marks, but there is no reason for a user
	 * to have many of those, so calculate by the cost of inode marks.
	 */
	max_marks = (((si.totalram - si.totalhigh) / 100) << PAGE_SHIFT) /
		    INODE_MARK_COST;
	max_marks = clamp(max_marks, FANOTIFY_OLD_DEFAULT_MAX_MARKS,
				     FANOTIFY_DEFAULT_MAX_USER_MARKS);

	BUILD_BUG_ON(FANOTIFY_INIT_FLAGS & FANOTIFY_INTERNAL_GROUP_FLAGS);
	BUILD_BUG_ON(HWEIGHT32(FANOTIFY_INIT_FLAGS) != 12);
	BUILD_BUG_ON(HWEIGHT32(FANOTIFY_MARK_FLAGS) != 11);

	fanotify_mark_cache = KMEM_CACHE(fanotify_mark,
					 SLAB_PANIC|SLAB_ACCOUNT);
	fanotify_fid_event_cachep = KMEM_CACHE(fanotify_fid_event,
					       SLAB_PANIC);
	fanotify_path_event_cachep = KMEM_CACHE(fanotify_path_event,
						SLAB_PANIC);
	if (IS_ENABLED(CONFIG_FANOTIFY_ACCESS_PERMISSIONS)) {
		fanotify_perm_event_cachep =
			KMEM_CACHE(fanotify_perm_event, SLAB_PANIC);
	}

	fanotify_max_queued_events = FANOTIFY_DEFAULT_MAX_EVENTS;
	init_user_ns.ucount_max[UCOUNT_FANOTIFY_GROUPS] =
					FANOTIFY_DEFAULT_MAX_GROUPS;
	init_user_ns.ucount_max[UCOUNT_FANOTIFY_MARKS] = max_marks;
	fanotify_sysctls_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
device_initcall(fanotify_user_setup);
