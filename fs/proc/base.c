<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/fs/proc/base.c
 *
 *  Copyright (C) 1991, 1992 Linus Torvalds
 *
 *  proc base directory handling functions
 *
 *  1999, Al Viro. Rewritten. Now it covers the whole per-process part.
 *  Instead of using magical inumbers to determine the kind of object
 *  we allocate and fill in-core inodes upon lookup. They don't even
 *  go into icache. We cache the reference to task_struct upon lookup too.
 *  Eventually it should become a filesystem in its own. We don't use the
 *  rest of procfs anymore.
 *
 *
 *  Changelog:
 *  17-Jan-2005
 *  Allan Bezerra
 *  Bruna Moreira <bruna.moreira@indt.org.br>
 *  Edjard Mota <edjard.mota@indt.org.br>
 *  Ilias Biris <ilias.biris@indt.org.br>
 *  Mauricio Lin <mauricio.lin@indt.org.br>
 *
 *  Embedded Linux Lab - 10LE Instituto Nokia de Tecnologia - INdT
 *
 *  A new process specific entry (smaps) included in /proc. It shows the
 *  size of rss for each memory area. The maps entry lacks information
 *  about physical memory size (rss) for each mapped file, i.e.,
 *  rss information for executables and library files.
 *  This additional information is useful for any tools that need to know
 *  about physical memory consumption for a process specific library.
 *
 *  Changelog:
 *  21-Feb-2005
 *  Embedded Linux Lab - 10LE Instituto Nokia de Tecnologia - INdT
 *  Pud inclusion in the page table walking.
 *
 *  ChangeLog:
 *  10-Mar-2005
 *  10LE Instituto Nokia de Tecnologia - INdT:
 *  A better way to walks through the page table as suggested by Hugh Dickins.
 *
 *  Simo Piiroinen <simo.piiroinen@nokia.com>:
 *  Smaps information related to shared, private, clean and dirty pages.
 *
 *  Paul Mundt <paul.mundt@nokia.com>:
 *  Overall revision about smaps.
 */

<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/errno.h>
#include <linux/time.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/task_io_accounting_ops.h>
#include <linux/init.h>
#include <linux/capability.h>
#include <linux/file.h>
#include <linux/fdtable.h>
<<<<<<< HEAD
=======
#include <linux/generic-radix-tree.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/string.h>
#include <linux/seq_file.h>
#include <linux/namei.h>
#include <linux/mnt_namespace.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/rcupdate.h>
#include <linux/kallsyms.h>
#include <linux/stacktrace.h>
#include <linux/resource.h>
#include <linux/module.h>
#include <linux/mount.h>
#include <linux/security.h>
#include <linux/ptrace.h>
<<<<<<< HEAD
#include <linux/tracehook.h>
=======
#include <linux/printk.h>
#include <linux/cache.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/cgroup.h>
#include <linux/cpuset.h>
#include <linux/audit.h>
#include <linux/poll.h>
#include <linux/nsproxy.h>
#include <linux/oom.h>
#include <linux/elf.h>
#include <linux/pid_namespace.h>
<<<<<<< HEAD
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <linux/flex_array.h>
#ifdef CONFIG_HARDWALL
#include <asm/hardwall.h>
#endif
#include <trace/events/oom.h>
#include "internal.h"
=======
#include <linux/user_namespace.h>
#include <linux/fs_struct.h>
#include <linux/slab.h>
#include <linux/sched/autogroup.h>
#include <linux/sched/mm.h>
#include <linux/sched/coredump.h>
#include <linux/sched/debug.h>
#include <linux/sched/stat.h>
#include <linux/posix-timers.h>
#include <linux/time_namespace.h>
#include <linux/resctrl.h>
#include <linux/cn_proc.h>
#include <linux/ksm.h>
#include <uapi/linux/lsm.h>
#include <trace/events/oom.h>
#include "internal.h"
#include "fd.h"

#include "../../lib/kstrtox.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* NOTE:
 *	Implementing inode permission operations in /proc is almost
 *	certainly an error.  Permission checks need to happen during
 *	each system call not at open time.  The reason is that most of
 *	what we wish to check for permissions in /proc varies at runtime.
 *
 *	The classic example of a problem is opening file descriptors
 *	in /proc for a task before it execs a suid executable.
 */

<<<<<<< HEAD
struct pid_entry {
	char *name;
	int len;
=======
static u8 nlink_tid __ro_after_init;
static u8 nlink_tgid __ro_after_init;

struct pid_entry {
	const char *name;
	unsigned int len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	umode_t mode;
	const struct inode_operations *iop;
	const struct file_operations *fop;
	union proc_op op;
};

#define NOD(NAME, MODE, IOP, FOP, OP) {			\
	.name = (NAME),					\
	.len  = sizeof(NAME) - 1,			\
	.mode = MODE,					\
	.iop  = IOP,					\
	.fop  = FOP,					\
	.op   = OP,					\
}

#define DIR(NAME, MODE, iops, fops)	\
	NOD(NAME, (S_IFDIR|(MODE)), &iops, &fops, {} )
#define LNK(NAME, get_link)					\
	NOD(NAME, (S_IFLNK|S_IRWXUGO),				\
		&proc_pid_link_inode_operations, NULL,		\
		{ .proc_get_link = get_link } )
#define REG(NAME, MODE, fops)				\
	NOD(NAME, (S_IFREG|(MODE)), NULL, &fops, {})
<<<<<<< HEAD
#define INF(NAME, MODE, read)				\
	NOD(NAME, (S_IFREG|(MODE)), 			\
		NULL, &proc_info_file_operations,	\
		{ .proc_read = read } )
#define ONE(NAME, MODE, show)				\
	NOD(NAME, (S_IFREG|(MODE)), 			\
		NULL, &proc_single_file_operations,	\
		{ .proc_show = show } )

static int proc_fd_permission(struct inode *inode, int mask);

/* ANDROID is for special files in /proc. */
#define ANDROID(NAME, MODE, OTYPE)			\
	NOD(NAME, (S_IFREG|(MODE)),			\
		&proc_##OTYPE##_inode_operations,	\
		&proc_##OTYPE##_operations, {})
=======
#define ONE(NAME, MODE, show)				\
	NOD(NAME, (S_IFREG|(MODE)),			\
		NULL, &proc_single_file_operations,	\
		{ .proc_show = show } )
#define ATTR(LSMID, NAME, MODE)				\
	NOD(NAME, (S_IFREG|(MODE)),			\
		NULL, &proc_pid_attr_operations,	\
		{ .lsmid = LSMID })
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Count the number of hardlinks for the pid_entry table, excluding the .
 * and .. links.
 */
<<<<<<< HEAD
static unsigned int pid_entry_count_dirs(const struct pid_entry *entries,
=======
static unsigned int __init pid_entry_nlink(const struct pid_entry *entries,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int n)
{
	unsigned int i;
	unsigned int count;

<<<<<<< HEAD
	count = 0;
=======
	count = 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < n; ++i) {
		if (S_ISDIR(entries[i].mode))
			++count;
	}

	return count;
}

static int get_task_root(struct task_struct *task, struct path *root)
{
	int result = -ENOENT;

	task_lock(task);
	if (task->fs) {
		get_fs_root(task->fs, root);
		result = 0;
	}
	task_unlock(task);
	return result;
}

static int proc_cwd_link(struct dentry *dentry, struct path *path)
{
<<<<<<< HEAD
	struct task_struct *task = get_proc_task(dentry->d_inode);
=======
	struct task_struct *task = get_proc_task(d_inode(dentry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int result = -ENOENT;

	if (task) {
		task_lock(task);
		if (task->fs) {
			get_fs_pwd(task->fs, path);
			result = 0;
		}
		task_unlock(task);
		put_task_struct(task);
	}
	return result;
}

static int proc_root_link(struct dentry *dentry, struct path *path)
{
<<<<<<< HEAD
	struct task_struct *task = get_proc_task(dentry->d_inode);
=======
	struct task_struct *task = get_proc_task(d_inode(dentry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int result = -ENOENT;

	if (task) {
		result = get_task_root(task, path);
		put_task_struct(task);
	}
	return result;
}

<<<<<<< HEAD
struct mm_struct *mm_for_maps(struct task_struct *task)
{
	return mm_access(task, PTRACE_MODE_READ);
}

static int proc_pid_cmdline(struct task_struct *task, char * buffer)
{
	int res = 0;
	unsigned int len;
	struct mm_struct *mm = get_task_mm(task);
	if (!mm)
		goto out;
	if (!mm->arg_end)
		goto out_mm;	/* Shh! No looking before we're done */

 	len = mm->arg_end - mm->arg_start;
 
	if (len > PAGE_SIZE)
		len = PAGE_SIZE;
 
	res = access_process_vm(task, mm->arg_start, buffer, len, 0);

	// If the nul at the end of args has been overwritten, then
	// assume application is using setproctitle(3).
	if (res > 0 && buffer[res-1] != '\0' && len < PAGE_SIZE) {
		len = strnlen(buffer, res);
		if (len < res) {
		    res = len;
		} else {
			len = mm->env_end - mm->env_start;
			if (len > PAGE_SIZE - res)
				len = PAGE_SIZE - res;
			res += access_process_vm(task, mm->env_start, buffer+res, len, 0);
			res = strnlen(buffer, res);
		}
	}
out_mm:
	mmput(mm);
out:
	return res;
}

static int proc_pid_auxv(struct task_struct *task, char *buffer)
{
	struct mm_struct *mm = mm_for_maps(task);
	int res = PTR_ERR(mm);
	if (mm && !IS_ERR(mm)) {
		unsigned int nwords = 0;
		do {
			nwords += 2;
		} while (mm->saved_auxv[nwords - 2] != 0); /* AT_NULL */
		res = nwords * sizeof(mm->saved_auxv[0]);
		if (res > PAGE_SIZE)
			res = PAGE_SIZE;
		memcpy(buffer, mm->saved_auxv, res);
		mmput(mm);
	}
	return res;
}

=======
/*
 * If the user used setproctitle(), we just get the string from
 * user space at arg_start, and limit it to a maximum of one page.
 */
static ssize_t get_mm_proctitle(struct mm_struct *mm, char __user *buf,
				size_t count, unsigned long pos,
				unsigned long arg_start)
{
	char *page;
	int ret, got;

	if (pos >= PAGE_SIZE)
		return 0;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	ret = 0;
	got = access_remote_vm(mm, arg_start, page, PAGE_SIZE, FOLL_ANON);
	if (got > 0) {
		int len = strnlen(page, got);

		/* Include the NUL character if it was found */
		if (len < got)
			len++;

		if (len > pos) {
			len -= pos;
			if (len > count)
				len = count;
			len -= copy_to_user(buf, page+pos, len);
			if (!len)
				len = -EFAULT;
			ret = len;
		}
	}
	free_page((unsigned long)page);
	return ret;
}

static ssize_t get_mm_cmdline(struct mm_struct *mm, char __user *buf,
			      size_t count, loff_t *ppos)
{
	unsigned long arg_start, arg_end, env_start, env_end;
	unsigned long pos, len;
	char *page, c;

	/* Check if process spawned far enough to have cmdline. */
	if (!mm->env_end)
		return 0;

	spin_lock(&mm->arg_lock);
	arg_start = mm->arg_start;
	arg_end = mm->arg_end;
	env_start = mm->env_start;
	env_end = mm->env_end;
	spin_unlock(&mm->arg_lock);

	if (arg_start >= arg_end)
		return 0;

	/*
	 * We allow setproctitle() to overwrite the argument
	 * strings, and overflow past the original end. But
	 * only when it overflows into the environment area.
	 */
	if (env_start != arg_end || env_end < env_start)
		env_start = env_end = arg_end;
	len = env_end - arg_start;

	/* We're not going to care if "*ppos" has high bits set */
	pos = *ppos;
	if (pos >= len)
		return 0;
	if (count > len - pos)
		count = len - pos;
	if (!count)
		return 0;

	/*
	 * Magical special case: if the argv[] end byte is not
	 * zero, the user has overwritten it with setproctitle(3).
	 *
	 * Possible future enhancement: do this only once when
	 * pos is 0, and set a flag in the 'struct file'.
	 */
	if (access_remote_vm(mm, arg_end-1, &c, 1, FOLL_ANON) == 1 && c)
		return get_mm_proctitle(mm, buf, count, pos, arg_start);

	/*
	 * For the non-setproctitle() case we limit things strictly
	 * to the [arg_start, arg_end[ range.
	 */
	pos += arg_start;
	if (pos < arg_start || pos >= arg_end)
		return 0;
	if (count > arg_end - pos)
		count = arg_end - pos;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	len = 0;
	while (count) {
		int got;
		size_t size = min_t(size_t, PAGE_SIZE, count);

		got = access_remote_vm(mm, pos, page, size, FOLL_ANON);
		if (got <= 0)
			break;
		got -= copy_to_user(buf, page, got);
		if (unlikely(!got)) {
			if (!len)
				len = -EFAULT;
			break;
		}
		pos += got;
		buf += got;
		len += got;
		count -= got;
	}

	free_page((unsigned long)page);
	return len;
}

static ssize_t get_task_cmdline(struct task_struct *tsk, char __user *buf,
				size_t count, loff_t *pos)
{
	struct mm_struct *mm;
	ssize_t ret;

	mm = get_task_mm(tsk);
	if (!mm)
		return 0;

	ret = get_mm_cmdline(mm, buf, count, pos);
	mmput(mm);
	return ret;
}

static ssize_t proc_pid_cmdline_read(struct file *file, char __user *buf,
				     size_t count, loff_t *pos)
{
	struct task_struct *tsk;
	ssize_t ret;

	BUG_ON(*pos < 0);

	tsk = get_proc_task(file_inode(file));
	if (!tsk)
		return -ESRCH;
	ret = get_task_cmdline(tsk, buf, count, pos);
	put_task_struct(tsk);
	if (ret > 0)
		*pos += ret;
	return ret;
}

static const struct file_operations proc_pid_cmdline_ops = {
	.read	= proc_pid_cmdline_read,
	.llseek	= generic_file_llseek,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_KALLSYMS
/*
 * Provides a wchan file via kallsyms in a proper one-value-per-file format.
 * Returns the resolved symbol.  If that fails, simply return the address.
 */
<<<<<<< HEAD
static int proc_pid_wchan(struct task_struct *task, char *buffer)
=======
static int proc_pid_wchan(struct seq_file *m, struct pid_namespace *ns,
			  struct pid *pid, struct task_struct *task)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long wchan;
	char symname[KSYM_NAME_LEN];

<<<<<<< HEAD
	wchan = get_wchan(task);

	if (lookup_symbol_name(wchan, symname) < 0)
		if (!ptrace_may_access(task, PTRACE_MODE_READ))
			return 0;
		else
			return sprintf(buffer, "%lu", wchan);
	else
		return sprintf(buffer, "%s", symname);
=======
	if (!ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS))
		goto print0;

	wchan = get_wchan(task);
	if (wchan && !lookup_symbol_name(wchan, symname)) {
		seq_puts(m, symname);
		return 0;
	}

print0:
	seq_putc(m, '0');
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* CONFIG_KALLSYMS */

static int lock_trace(struct task_struct *task)
{
<<<<<<< HEAD
	int err = mutex_lock_killable(&task->signal->cred_guard_mutex);
	if (err)
		return err;
	if (!ptrace_may_access(task, PTRACE_MODE_ATTACH)) {
		mutex_unlock(&task->signal->cred_guard_mutex);
=======
	int err = down_read_killable(&task->signal->exec_update_lock);
	if (err)
		return err;
	if (!ptrace_may_access(task, PTRACE_MODE_ATTACH_FSCREDS)) {
		up_read(&task->signal->exec_update_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EPERM;
	}
	return 0;
}

static void unlock_trace(struct task_struct *task)
{
<<<<<<< HEAD
	mutex_unlock(&task->signal->cred_guard_mutex);
=======
	up_read(&task->signal->exec_update_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_STACKTRACE

#define MAX_STACK_TRACE_DEPTH	64

static int proc_pid_stack(struct seq_file *m, struct pid_namespace *ns,
			  struct pid *pid, struct task_struct *task)
{
<<<<<<< HEAD
	struct stack_trace trace;
	unsigned long *entries;
	int err;
	int i;

	entries = kmalloc(MAX_STACK_TRACE_DEPTH * sizeof(*entries), GFP_KERNEL);
	if (!entries)
		return -ENOMEM;

	trace.nr_entries	= 0;
	trace.max_entries	= MAX_STACK_TRACE_DEPTH;
	trace.entries		= entries;
	trace.skip		= 0;

	err = lock_trace(task);
	if (!err) {
		save_stack_trace_tsk(task, &trace);

		for (i = 0; i < trace.nr_entries; i++) {
			seq_printf(m, "[<%pK>] %pS\n",
				   (void *)entries[i], (void *)entries[i]);
		}
=======
	unsigned long *entries;
	int err;

	/*
	 * The ability to racily run the kernel stack unwinder on a running task
	 * and then observe the unwinder output is scary; while it is useful for
	 * debugging kernel issues, it can also allow an attacker to leak kernel
	 * stack contents.
	 * Doing this in a manner that is at least safe from races would require
	 * some work to ensure that the remote task can not be scheduled; and
	 * even then, this would still expose the unwinder as local attack
	 * surface.
	 * Therefore, this interface is restricted to root.
	 */
	if (!file_ns_capable(m->file, &init_user_ns, CAP_SYS_ADMIN))
		return -EACCES;

	entries = kmalloc_array(MAX_STACK_TRACE_DEPTH, sizeof(*entries),
				GFP_KERNEL);
	if (!entries)
		return -ENOMEM;

	err = lock_trace(task);
	if (!err) {
		unsigned int i, nr_entries;

		nr_entries = stack_trace_save_tsk(task, entries,
						  MAX_STACK_TRACE_DEPTH, 0);

		for (i = 0; i < nr_entries; i++) {
			seq_printf(m, "[<0>] %pB\n", (void *)entries[i]);
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unlock_trace(task);
	}
	kfree(entries);

	return err;
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_SCHEDSTATS
/*
 * Provides /proc/PID/schedstat
 */
static int proc_pid_schedstat(struct task_struct *task, char *buffer)
{
	return sprintf(buffer, "%llu %llu %lu\n",
			(unsigned long long)task->se.sum_exec_runtime,
			(unsigned long long)task->sched_info.run_delay,
			task->sched_info.pcount);
=======
#ifdef CONFIG_SCHED_INFO
/*
 * Provides /proc/PID/schedstat
 */
static int proc_pid_schedstat(struct seq_file *m, struct pid_namespace *ns,
			      struct pid *pid, struct task_struct *task)
{
	if (unlikely(!sched_info_on()))
		seq_puts(m, "0 0 0\n");
	else
		seq_printf(m, "%llu %llu %lu\n",
		   (unsigned long long)task->se.sum_exec_runtime,
		   (unsigned long long)task->sched_info.run_delay,
		   task->sched_info.pcount);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#ifdef CONFIG_LATENCYTOP
static int lstats_show_proc(struct seq_file *m, void *v)
{
	int i;
	struct inode *inode = m->private;
	struct task_struct *task = get_proc_task(inode);

	if (!task)
		return -ESRCH;
	seq_puts(m, "Latency Top version : v0.1\n");
<<<<<<< HEAD
	for (i = 0; i < 32; i++) {
=======
	for (i = 0; i < LT_SAVECOUNT; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct latency_record *lr = &task->latency_record[i];
		if (lr->backtrace[0]) {
			int q;
			seq_printf(m, "%i %li %li",
				   lr->count, lr->time, lr->max);
			for (q = 0; q < LT_BACKTRACEDEPTH; q++) {
				unsigned long bt = lr->backtrace[q];
<<<<<<< HEAD
				if (!bt)
					break;
				if (bt == ULONG_MAX)
					break;
=======

				if (!bt)
					break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				seq_printf(m, " %ps", (void *)bt);
			}
			seq_putc(m, '\n');
		}

	}
	put_task_struct(task);
	return 0;
}

static int lstats_open(struct inode *inode, struct file *file)
{
	return single_open(file, lstats_show_proc, inode);
}

static ssize_t lstats_write(struct file *file, const char __user *buf,
			    size_t count, loff_t *offs)
{
<<<<<<< HEAD
	struct task_struct *task = get_proc_task(file->f_dentry->d_inode);

	if (!task)
		return -ESRCH;
	clear_all_latency_tracing(task);
=======
	struct task_struct *task = get_proc_task(file_inode(file));

	if (!task)
		return -ESRCH;
	clear_tsk_latency_tracing(task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_task_struct(task);

	return count;
}

static const struct file_operations proc_lstats_operations = {
	.open		= lstats_open,
	.read		= seq_read,
	.write		= lstats_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif

<<<<<<< HEAD
static int proc_oom_score(struct task_struct *task, char *buffer)
{
	unsigned long points = 0;

	read_lock(&tasklist_lock);
	if (pid_alive(task))
		points = oom_badness(task, NULL, NULL,
					totalram_pages + total_swap_pages);
	read_unlock(&tasklist_lock);
	return sprintf(buffer, "%lu\n", points);
}

struct limit_names {
	char *name;
	char *unit;
=======
static int proc_oom_score(struct seq_file *m, struct pid_namespace *ns,
			  struct pid *pid, struct task_struct *task)
{
	unsigned long totalpages = totalram_pages() + total_swap_pages;
	unsigned long points = 0;
	long badness;

	badness = oom_badness(task, totalpages);
	/*
	 * Special case OOM_SCORE_ADJ_MIN for all others scale the
	 * badness value into [0, 2000] range which we have been
	 * exporting for a long time so userspace might depend on it.
	 */
	if (badness != LONG_MIN)
		points = (1000 + badness * 1000 / (long)totalpages) * 2 / 3;

	seq_printf(m, "%lu\n", points);

	return 0;
}

struct limit_names {
	const char *name;
	const char *unit;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct limit_names lnames[RLIM_NLIMITS] = {
	[RLIMIT_CPU] = {"Max cpu time", "seconds"},
	[RLIMIT_FSIZE] = {"Max file size", "bytes"},
	[RLIMIT_DATA] = {"Max data size", "bytes"},
	[RLIMIT_STACK] = {"Max stack size", "bytes"},
	[RLIMIT_CORE] = {"Max core file size", "bytes"},
	[RLIMIT_RSS] = {"Max resident set", "bytes"},
	[RLIMIT_NPROC] = {"Max processes", "processes"},
	[RLIMIT_NOFILE] = {"Max open files", "files"},
	[RLIMIT_MEMLOCK] = {"Max locked memory", "bytes"},
	[RLIMIT_AS] = {"Max address space", "bytes"},
	[RLIMIT_LOCKS] = {"Max file locks", "locks"},
	[RLIMIT_SIGPENDING] = {"Max pending signals", "signals"},
	[RLIMIT_MSGQUEUE] = {"Max msgqueue size", "bytes"},
	[RLIMIT_NICE] = {"Max nice priority", NULL},
	[RLIMIT_RTPRIO] = {"Max realtime priority", NULL},
	[RLIMIT_RTTIME] = {"Max realtime timeout", "us"},
};

/* Display limits for a process */
<<<<<<< HEAD
static int proc_pid_limits(struct task_struct *task, char *buffer)
{
	unsigned int i;
	int count = 0;
	unsigned long flags;
	char *bufptr = buffer;
=======
static int proc_pid_limits(struct seq_file *m, struct pid_namespace *ns,
			   struct pid *pid, struct task_struct *task)
{
	unsigned int i;
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct rlimit rlim[RLIM_NLIMITS];

	if (!lock_task_sighand(task, &flags))
		return 0;
	memcpy(rlim, task->signal->rlim, sizeof(struct rlimit) * RLIM_NLIMITS);
	unlock_task_sighand(task, &flags);

	/*
	 * print the file header
	 */
<<<<<<< HEAD
	count += sprintf(&bufptr[count], "%-25s %-20s %-20s %-10s\n",
			"Limit", "Soft Limit", "Hard Limit", "Units");

	for (i = 0; i < RLIM_NLIMITS; i++) {
		if (rlim[i].rlim_cur == RLIM_INFINITY)
			count += sprintf(&bufptr[count], "%-25s %-20s ",
					 lnames[i].name, "unlimited");
		else
			count += sprintf(&bufptr[count], "%-25s %-20lu ",
					 lnames[i].name, rlim[i].rlim_cur);

		if (rlim[i].rlim_max == RLIM_INFINITY)
			count += sprintf(&bufptr[count], "%-20s ", "unlimited");
		else
			count += sprintf(&bufptr[count], "%-20lu ",
					 rlim[i].rlim_max);

		if (lnames[i].unit)
			count += sprintf(&bufptr[count], "%-10s\n",
					 lnames[i].unit);
		else
			count += sprintf(&bufptr[count], "\n");
	}

	return count;
}

#ifdef CONFIG_HAVE_ARCH_TRACEHOOK
static int proc_pid_syscall(struct task_struct *task, char *buffer)
{
	long nr;
	unsigned long args[6], sp, pc;
	int res = lock_trace(task);
	if (res)
		return res;

	if (task_current_syscall(task, &nr, args, 6, &sp, &pc))
		res = sprintf(buffer, "running\n");
	else if (nr < 0)
		res = sprintf(buffer, "%ld 0x%lx 0x%lx\n", nr, sp, pc);
	else
		res = sprintf(buffer,
		       "%ld 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx 0x%lx\n",
		       nr,
		       args[0], args[1], args[2], args[3], args[4], args[5],
		       sp, pc);
	unlock_trace(task);
	return res;
=======
	seq_puts(m, "Limit                     "
		"Soft Limit           "
		"Hard Limit           "
		"Units     \n");

	for (i = 0; i < RLIM_NLIMITS; i++) {
		if (rlim[i].rlim_cur == RLIM_INFINITY)
			seq_printf(m, "%-25s %-20s ",
				   lnames[i].name, "unlimited");
		else
			seq_printf(m, "%-25s %-20lu ",
				   lnames[i].name, rlim[i].rlim_cur);

		if (rlim[i].rlim_max == RLIM_INFINITY)
			seq_printf(m, "%-20s ", "unlimited");
		else
			seq_printf(m, "%-20lu ", rlim[i].rlim_max);

		if (lnames[i].unit)
			seq_printf(m, "%-10s\n", lnames[i].unit);
		else
			seq_putc(m, '\n');
	}

	return 0;
}

#ifdef CONFIG_HAVE_ARCH_TRACEHOOK
static int proc_pid_syscall(struct seq_file *m, struct pid_namespace *ns,
			    struct pid *pid, struct task_struct *task)
{
	struct syscall_info info;
	u64 *args = &info.data.args[0];
	int res;

	res = lock_trace(task);
	if (res)
		return res;

	if (task_current_syscall(task, &info))
		seq_puts(m, "running\n");
	else if (info.data.nr < 0)
		seq_printf(m, "%d 0x%llx 0x%llx\n",
			   info.data.nr, info.sp, info.data.instruction_pointer);
	else
		seq_printf(m,
		       "%d 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx 0x%llx\n",
		       info.data.nr,
		       args[0], args[1], args[2], args[3], args[4], args[5],
		       info.sp, info.data.instruction_pointer);
	unlock_trace(task);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* CONFIG_HAVE_ARCH_TRACEHOOK */

/************************************************************************/
/*                       Here the fs part begins                        */
/************************************************************************/

/* permission checks */
<<<<<<< HEAD
static int proc_fd_access_allowed(struct inode *inode)
{
	struct task_struct *task;
	int allowed = 0;
=======
static bool proc_fd_access_allowed(struct inode *inode)
{
	struct task_struct *task;
	bool allowed = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Allow access to a task's file descriptors if it is us or we
	 * may use ptrace attach to the process and find out that
	 * information.
	 */
	task = get_proc_task(inode);
	if (task) {
<<<<<<< HEAD
		allowed = ptrace_may_access(task, PTRACE_MODE_READ);
=======
		allowed = ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		put_task_struct(task);
	}
	return allowed;
}

<<<<<<< HEAD
int proc_setattr(struct dentry *dentry, struct iattr *attr)
{
	int error;
	struct inode *inode = dentry->d_inode;
=======
int proc_setattr(struct mnt_idmap *idmap, struct dentry *dentry,
		 struct iattr *attr)
{
	int error;
	struct inode *inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (attr->ia_valid & ATTR_MODE)
		return -EPERM;

<<<<<<< HEAD
	error = inode_change_ok(inode, attr);
	if (error)
		return error;

	if ((attr->ia_valid & ATTR_SIZE) &&
	    attr->ia_size != i_size_read(inode)) {
		error = vmtruncate(inode, attr->ia_size);
		if (error)
			return error;
	}

	setattr_copy(inode, attr);
	mark_inode_dirty(inode);
=======
	error = setattr_prepare(&nop_mnt_idmap, dentry, attr);
	if (error)
		return error;

	setattr_copy(&nop_mnt_idmap, inode, attr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * May current process learn task's sched/cmdline info (for hide_pid_min=1)
 * or euid/egid (for hide_pid_min=2)?
 */
<<<<<<< HEAD
static bool has_pid_permissions(struct pid_namespace *pid,
				 struct task_struct *task,
				 int hide_pid_min)
{
	if (pid->hide_pid < hide_pid_min)
		return true;
	if (in_group_p(pid->pid_gid))
		return true;
	return ptrace_may_access(task, PTRACE_MODE_READ);
}


static int proc_pid_permission(struct inode *inode, int mask)
{
	struct pid_namespace *pid = inode->i_sb->s_fs_info;
=======
static bool has_pid_permissions(struct proc_fs_info *fs_info,
				 struct task_struct *task,
				 enum proc_hidepid hide_pid_min)
{
	/*
	 * If 'hidpid' mount option is set force a ptrace check,
	 * we indicate that we are using a filesystem syscall
	 * by passing PTRACE_MODE_READ_FSCREDS
	 */
	if (fs_info->hide_pid == HIDEPID_NOT_PTRACEABLE)
		return ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS);

	if (fs_info->hide_pid < hide_pid_min)
		return true;
	if (in_group_p(fs_info->pid_gid))
		return true;
	return ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS);
}


static int proc_pid_permission(struct mnt_idmap *idmap,
			       struct inode *inode, int mask)
{
	struct proc_fs_info *fs_info = proc_sb_info(inode->i_sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct task_struct *task;
	bool has_perms;

	task = get_proc_task(inode);
	if (!task)
		return -ESRCH;
<<<<<<< HEAD
	has_perms = has_pid_permissions(pid, task, 1);
	put_task_struct(task);

	if (!has_perms) {
		if (pid->hide_pid == 2) {
=======
	has_perms = has_pid_permissions(fs_info, task, HIDEPID_NO_ACCESS);
	put_task_struct(task);

	if (!has_perms) {
		if (fs_info->hide_pid == HIDEPID_INVISIBLE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Let's make getdents(), stat(), and open()
			 * consistent with each other.  If a process
			 * may not stat() a file, it shouldn't be seen
			 * in procfs at all.
			 */
			return -ENOENT;
		}

		return -EPERM;
	}
<<<<<<< HEAD
	return generic_permission(inode, mask);
=======
	return generic_permission(&nop_mnt_idmap, inode, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}



static const struct inode_operations proc_def_inode_operations = {
	.setattr	= proc_setattr,
};

<<<<<<< HEAD
#define PROC_BLOCK_SIZE	(3*1024)		/* 4K page size but our output routines use some slack for overruns */

static ssize_t proc_info_read(struct file * file, char __user * buf,
			  size_t count, loff_t *ppos)
{
	struct inode * inode = file->f_path.dentry->d_inode;
	unsigned long page;
	ssize_t length;
	struct task_struct *task = get_proc_task(inode);

	length = -ESRCH;
	if (!task)
		goto out_no_task;

	if (count > PROC_BLOCK_SIZE)
		count = PROC_BLOCK_SIZE;

	length = -ENOMEM;
	if (!(page = __get_free_page(GFP_TEMPORARY)))
		goto out;

	length = PROC_I(inode)->op.proc_read(task, (char*)page);

	if (length >= 0)
		length = simple_read_from_buffer(buf, count, ppos, (char *)page, length);
	free_page(page);
out:
	put_task_struct(task);
out_no_task:
	return length;
}

static const struct file_operations proc_info_file_operations = {
	.read		= proc_info_read,
	.llseek		= generic_file_llseek,
};

static int proc_single_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
	struct pid_namespace *ns;
	struct pid *pid;
	struct task_struct *task;
	int ret;

	ns = inode->i_sb->s_fs_info;
	pid = proc_pid(inode);
=======
static int proc_single_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
	struct pid_namespace *ns = proc_pid_ns(inode->i_sb);
	struct pid *pid = proc_pid(inode);
	struct task_struct *task;
	int ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	task = get_pid_task(pid, PIDTYPE_PID);
	if (!task)
		return -ESRCH;

	ret = PROC_I(inode)->op.proc_show(m, ns, pid, task);

	put_task_struct(task);
	return ret;
}

static int proc_single_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, proc_single_show, inode);
}

static const struct file_operations proc_single_file_operations = {
	.open		= proc_single_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

<<<<<<< HEAD
static int mem_open(struct inode* inode, struct file* file)
{
	struct task_struct *task = get_proc_task(file->f_path.dentry->d_inode);
	struct mm_struct *mm;

	if (!task)
		return -ESRCH;

	mm = mm_access(task, PTRACE_MODE_ATTACH);
	put_task_struct(task);
=======

struct mm_struct *proc_mem_open(struct inode *inode, unsigned int mode)
{
	struct task_struct *task = get_proc_task(inode);
	struct mm_struct *mm = ERR_PTR(-ESRCH);

	if (task) {
		mm = mm_access(task, mode | PTRACE_MODE_FSCREDS);
		put_task_struct(task);

		if (!IS_ERR_OR_NULL(mm)) {
			/* ensure this mm_struct can't be freed */
			mmgrab(mm);
			/* but do not pin its memory */
			mmput(mm);
		}
	}

	return mm;
}

static int __mem_open(struct inode *inode, struct file *file, unsigned int mode)
{
	struct mm_struct *mm = proc_mem_open(inode, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (IS_ERR(mm))
		return PTR_ERR(mm);

<<<<<<< HEAD
	if (mm) {
		/* ensure this mm_struct can't be freed */
		atomic_inc(&mm->mm_count);
		/* but do not pin its memory */
		mmput(mm);
	}

	/* OK to pass negative loff_t, we can catch out-of-range */
	file->f_mode |= FMODE_UNSIGNED_OFFSET;
	file->private_data = mm;

	return 0;
=======
	file->private_data = mm;
	return 0;
}

static int mem_open(struct inode *inode, struct file *file)
{
	int ret = __mem_open(inode, file, PTRACE_MODE_ATTACH);

	/* OK to pass negative loff_t, we can catch out-of-range */
	file->f_mode |= FMODE_UNSIGNED_OFFSET;

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t mem_rw(struct file *file, char __user *buf,
			size_t count, loff_t *ppos, int write)
{
	struct mm_struct *mm = file->private_data;
	unsigned long addr = *ppos;
	ssize_t copied;
	char *page;
<<<<<<< HEAD
=======
	unsigned int flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!mm)
		return 0;

<<<<<<< HEAD
	page = (char *)__get_free_page(GFP_TEMPORARY);
=======
	page = (char *)__get_free_page(GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!page)
		return -ENOMEM;

	copied = 0;
<<<<<<< HEAD
	if (!atomic_inc_not_zero(&mm->mm_users))
		goto free;

	while (count > 0) {
		int this_len = min_t(int, count, PAGE_SIZE);
=======
	if (!mmget_not_zero(mm))
		goto free;

	flags = FOLL_FORCE | (write ? FOLL_WRITE : 0);

	while (count > 0) {
		size_t this_len = min_t(size_t, count, PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (write && copy_from_user(page, buf, this_len)) {
			copied = -EFAULT;
			break;
		}

<<<<<<< HEAD
		this_len = access_remote_vm(mm, addr, page, this_len, write);
=======
		this_len = access_remote_vm(mm, addr, page, this_len, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!this_len) {
			if (!copied)
				copied = -EIO;
			break;
		}

		if (!write && copy_to_user(buf, page, this_len)) {
			copied = -EFAULT;
			break;
		}

		buf += this_len;
		addr += this_len;
		copied += this_len;
		count -= this_len;
	}
	*ppos = addr;

	mmput(mm);
free:
	free_page((unsigned long) page);
	return copied;
}

static ssize_t mem_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	return mem_rw(file, buf, count, ppos, 0);
}

static ssize_t mem_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	return mem_rw(file, (char __user*)buf, count, ppos, 1);
}

loff_t mem_lseek(struct file *file, loff_t offset, int orig)
{
	switch (orig) {
	case 0:
		file->f_pos = offset;
		break;
	case 1:
		file->f_pos += offset;
		break;
	default:
		return -EINVAL;
	}
	force_successful_syscall_return();
	return file->f_pos;
}

static int mem_release(struct inode *inode, struct file *file)
{
	struct mm_struct *mm = file->private_data;
	if (mm)
		mmdrop(mm);
	return 0;
}

static const struct file_operations proc_mem_operations = {
	.llseek		= mem_lseek,
	.read		= mem_read,
	.write		= mem_write,
	.open		= mem_open,
	.release	= mem_release,
};

<<<<<<< HEAD
static ssize_t environ_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	struct task_struct *task = get_proc_task(file->f_dentry->d_inode);
	char *page;
	unsigned long src = *ppos;
	int ret = -ESRCH;
	struct mm_struct *mm;

	if (!task)
		goto out_no_task;

	ret = -ENOMEM;
	page = (char *)__get_free_page(GFP_TEMPORARY);
	if (!page)
		goto out;


	mm = mm_for_maps(task);
	ret = PTR_ERR(mm);
	/* Ensure the process spawned far enough to have an environment. */
	if (!mm || IS_ERR(mm) || !mm->env_end)
		goto out_free;

	ret = 0;
	while (count > 0) {
		int this_len, retval, max_len;

		this_len = mm->env_end - (mm->env_start + src);

		if (this_len <= 0)
			break;

		max_len = (count > PAGE_SIZE) ? PAGE_SIZE : count;
		this_len = (this_len > max_len) ? max_len : this_len;

		retval = access_process_vm(task, (mm->env_start + src),
			page, this_len, 0);
=======
static int environ_open(struct inode *inode, struct file *file)
{
	return __mem_open(inode, file, PTRACE_MODE_READ);
}

static ssize_t environ_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	char *page;
	unsigned long src = *ppos;
	int ret = 0;
	struct mm_struct *mm = file->private_data;
	unsigned long env_start, env_end;

	/* Ensure the process spawned far enough to have an environment. */
	if (!mm || !mm->env_end)
		return 0;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	ret = 0;
	if (!mmget_not_zero(mm))
		goto free;

	spin_lock(&mm->arg_lock);
	env_start = mm->env_start;
	env_end = mm->env_end;
	spin_unlock(&mm->arg_lock);

	while (count > 0) {
		size_t this_len, max_len;
		int retval;

		if (src >= (env_end - env_start))
			break;

		this_len = env_end - (env_start + src);

		max_len = min_t(size_t, PAGE_SIZE, count);
		this_len = min(max_len, this_len);

		retval = access_remote_vm(mm, (env_start + src), page, this_len, FOLL_ANON);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (retval <= 0) {
			ret = retval;
			break;
		}

		if (copy_to_user(buf, page, retval)) {
			ret = -EFAULT;
			break;
		}

		ret += retval;
		src += retval;
		buf += retval;
		count -= retval;
	}
	*ppos = src;
<<<<<<< HEAD

	mmput(mm);
out_free:
	free_page((unsigned long) page);
out:
	put_task_struct(task);
out_no_task:
=======
	mmput(mm);

free:
	free_page((unsigned long) page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct file_operations proc_environ_operations = {
<<<<<<< HEAD
	.read		= environ_read,
	.llseek		= generic_file_llseek,
};

static ssize_t oom_adjust_read(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
	struct task_struct *task = get_proc_task(file->f_path.dentry->d_inode);
	char buffer[PROC_NUMBUF];
	size_t len;
	int oom_adjust = OOM_DISABLE;
	unsigned long flags;

	if (!task)
		return -ESRCH;

	if (lock_task_sighand(task, &flags)) {
		oom_adjust = task->signal->oom_adj;
		unlock_task_sighand(task, &flags);
	}

	put_task_struct(task);

	len = snprintf(buffer, sizeof(buffer), "%i\n", oom_adjust);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t oom_adjust_write(struct file *file, const char __user *buf,
				size_t count, loff_t *ppos)
{
	struct task_struct *task;
	char buffer[PROC_NUMBUF];
	int oom_adjust;
	unsigned long flags;
	int err;

	memset(buffer, 0, sizeof(buffer));
=======
	.open		= environ_open,
	.read		= environ_read,
	.llseek		= generic_file_llseek,
	.release	= mem_release,
};

static int auxv_open(struct inode *inode, struct file *file)
{
	return __mem_open(inode, file, PTRACE_MODE_READ_FSCREDS);
}

static ssize_t auxv_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	struct mm_struct *mm = file->private_data;
	unsigned int nwords = 0;

	if (!mm)
		return 0;
	do {
		nwords += 2;
	} while (mm->saved_auxv[nwords - 2] != 0); /* AT_NULL */
	return simple_read_from_buffer(buf, count, ppos, mm->saved_auxv,
				       nwords * sizeof(mm->saved_auxv[0]));
}

static const struct file_operations proc_auxv_operations = {
	.open		= auxv_open,
	.read		= auxv_read,
	.llseek		= generic_file_llseek,
	.release	= mem_release,
};

static ssize_t oom_adj_read(struct file *file, char __user *buf, size_t count,
			    loff_t *ppos)
{
	struct task_struct *task = get_proc_task(file_inode(file));
	char buffer[PROC_NUMBUF];
	int oom_adj = OOM_ADJUST_MIN;
	size_t len;

	if (!task)
		return -ESRCH;
	if (task->signal->oom_score_adj == OOM_SCORE_ADJ_MAX)
		oom_adj = OOM_ADJUST_MAX;
	else
		oom_adj = (task->signal->oom_score_adj * -OOM_DISABLE) /
			  OOM_SCORE_ADJ_MAX;
	put_task_struct(task);
	if (oom_adj > OOM_ADJUST_MAX)
		oom_adj = OOM_ADJUST_MAX;
	len = snprintf(buffer, sizeof(buffer), "%d\n", oom_adj);
	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static int __set_oom_adj(struct file *file, int oom_adj, bool legacy)
{
	struct mm_struct *mm = NULL;
	struct task_struct *task;
	int err = 0;

	task = get_proc_task(file_inode(file));
	if (!task)
		return -ESRCH;

	mutex_lock(&oom_adj_mutex);
	if (legacy) {
		if (oom_adj < task->signal->oom_score_adj &&
				!capable(CAP_SYS_RESOURCE)) {
			err = -EACCES;
			goto err_unlock;
		}
		/*
		 * /proc/pid/oom_adj is provided for legacy purposes, ask users to use
		 * /proc/pid/oom_score_adj instead.
		 */
		pr_warn_once("%s (%d): /proc/%d/oom_adj is deprecated, please use /proc/%d/oom_score_adj instead.\n",
			  current->comm, task_pid_nr(current), task_pid_nr(task),
			  task_pid_nr(task));
	} else {
		if ((short)oom_adj < task->signal->oom_score_adj_min &&
				!capable(CAP_SYS_RESOURCE)) {
			err = -EACCES;
			goto err_unlock;
		}
	}

	/*
	 * Make sure we will check other processes sharing the mm if this is
	 * not vfrok which wants its own oom_score_adj.
	 * pin the mm so it doesn't go away and get reused after task_unlock
	 */
	if (!task->vfork_done) {
		struct task_struct *p = find_lock_task_mm(task);

		if (p) {
			if (test_bit(MMF_MULTIPROCESS, &p->mm->flags)) {
				mm = p->mm;
				mmgrab(mm);
			}
			task_unlock(p);
		}
	}

	task->signal->oom_score_adj = oom_adj;
	if (!legacy && has_capability_noaudit(current, CAP_SYS_RESOURCE))
		task->signal->oom_score_adj_min = (short)oom_adj;
	trace_oom_score_adj_update(task);

	if (mm) {
		struct task_struct *p;

		rcu_read_lock();
		for_each_process(p) {
			if (same_thread_group(task, p))
				continue;

			/* do not touch kernel threads or the global init */
			if (p->flags & PF_KTHREAD || is_global_init(p))
				continue;

			task_lock(p);
			if (!p->vfork_done && process_shares_mm(p, mm)) {
				p->signal->oom_score_adj = oom_adj;
				if (!legacy && has_capability_noaudit(current, CAP_SYS_RESOURCE))
					p->signal->oom_score_adj_min = (short)oom_adj;
			}
			task_unlock(p);
		}
		rcu_read_unlock();
		mmdrop(mm);
	}
err_unlock:
	mutex_unlock(&oom_adj_mutex);
	put_task_struct(task);
	return err;
}

/*
 * /proc/pid/oom_adj exists solely for backwards compatibility with previous
 * kernels.  The effective policy is defined by oom_score_adj, which has a
 * different scale: oom_adj grew exponentially and oom_score_adj grows linearly.
 * Values written to oom_adj are simply mapped linearly to oom_score_adj.
 * Processes that become oom disabled via oom_adj will still be oom disabled
 * with this implementation.
 *
 * oom_adj cannot be removed since existing userspace binaries use it.
 */
static ssize_t oom_adj_write(struct file *file, const char __user *buf,
			     size_t count, loff_t *ppos)
{
	char buffer[PROC_NUMBUF] = {};
	int oom_adj;
	int err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count)) {
		err = -EFAULT;
		goto out;
	}

<<<<<<< HEAD
	err = kstrtoint(strstrip(buffer), 0, &oom_adjust);
	if (err)
		goto out;
	if ((oom_adjust < OOM_ADJUST_MIN || oom_adjust > OOM_ADJUST_MAX) &&
	     oom_adjust != OOM_DISABLE) {
=======
	err = kstrtoint(strstrip(buffer), 0, &oom_adj);
	if (err)
		goto out;
	if ((oom_adj < OOM_ADJUST_MIN || oom_adj > OOM_ADJUST_MAX) &&
	     oom_adj != OOM_DISABLE) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	task = get_proc_task(file->f_path.dentry->d_inode);
	if (!task) {
		err = -ESRCH;
		goto out;
	}

	task_lock(task);
	if (!task->mm) {
		err = -EINVAL;
		goto err_task_lock;
	}

	if (!lock_task_sighand(task, &flags)) {
		err = -ESRCH;
		goto err_task_lock;
	}

	if (oom_adjust < task->signal->oom_adj && !capable(CAP_SYS_RESOURCE)) {
		err = -EACCES;
		goto err_sighand;
	}

	/*
	 * Warn that /proc/pid/oom_adj is deprecated, see
	 * Documentation/feature-removal-schedule.txt.
	 */
	printk_once(KERN_WARNING "%s (%d): /proc/%d/oom_adj is deprecated, please use /proc/%d/oom_score_adj instead.\n",
		  current->comm, task_pid_nr(current), task_pid_nr(task),
		  task_pid_nr(task));
	task->signal->oom_adj = oom_adjust;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Scale /proc/pid/oom_score_adj appropriately ensuring that a maximum
	 * value is always attainable.
	 */
<<<<<<< HEAD
	if (task->signal->oom_adj == OOM_ADJUST_MAX)
		task->signal->oom_score_adj = OOM_SCORE_ADJ_MAX;
	else
		task->signal->oom_score_adj = (oom_adjust * OOM_SCORE_ADJ_MAX) /
								-OOM_DISABLE;
	trace_oom_score_adj_update(task);
err_sighand:
	unlock_task_sighand(task, &flags);
err_task_lock:
	task_unlock(task);
	put_task_struct(task);
=======
	if (oom_adj == OOM_ADJUST_MAX)
		oom_adj = OOM_SCORE_ADJ_MAX;
	else
		oom_adj = (oom_adj * OOM_SCORE_ADJ_MAX) / -OOM_DISABLE;

	err = __set_oom_adj(file, oom_adj, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err < 0 ? err : count;
}

<<<<<<< HEAD
static int oom_adjust_permission(struct inode *inode, int mask)
{
	uid_t uid;
	struct task_struct *p;

	p = get_proc_task(inode);
	if(p) {
		uid = task_uid(p);
		put_task_struct(p);
	}

	/*
	 * System Server (uid == 1000) is granted access to oom_adj of all 
	 * android applications (uid > 10000) as and services (uid >= 1000)
	 */
	if (p && (current_fsuid() == 1000) && (uid >= 1000)) {
		if (inode->i_mode >> 6 & mask) {
			return 0;
		}
	}

	/* Fall back to default. */
	return generic_permission(inode, mask);
}

static const struct inode_operations proc_oom_adjust_inode_operations = {
	.permission	= oom_adjust_permission,
};

static const struct file_operations proc_oom_adjust_operations = {
	.read		= oom_adjust_read,
	.write		= oom_adjust_write,
=======
static const struct file_operations proc_oom_adj_operations = {
	.read		= oom_adj_read,
	.write		= oom_adj_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.llseek		= generic_file_llseek,
};

static ssize_t oom_score_adj_read(struct file *file, char __user *buf,
					size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct task_struct *task = get_proc_task(file->f_path.dentry->d_inode);
	char buffer[PROC_NUMBUF];
	int oom_score_adj = OOM_SCORE_ADJ_MIN;
	unsigned long flags;
=======
	struct task_struct *task = get_proc_task(file_inode(file));
	char buffer[PROC_NUMBUF];
	short oom_score_adj = OOM_SCORE_ADJ_MIN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size_t len;

	if (!task)
		return -ESRCH;
<<<<<<< HEAD
	if (lock_task_sighand(task, &flags)) {
		oom_score_adj = task->signal->oom_score_adj;
		unlock_task_sighand(task, &flags);
	}
	put_task_struct(task);
	len = snprintf(buffer, sizeof(buffer), "%d\n", oom_score_adj);
=======
	oom_score_adj = task->signal->oom_score_adj;
	put_task_struct(task);
	len = snprintf(buffer, sizeof(buffer), "%hd\n", oom_score_adj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t oom_score_adj_write(struct file *file, const char __user *buf,
					size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct task_struct *task;
	char buffer[PROC_NUMBUF];
	unsigned long flags;
	int oom_score_adj;
	int err;

	memset(buffer, 0, sizeof(buffer));
=======
	char buffer[PROC_NUMBUF] = {};
	int oom_score_adj;
	int err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count)) {
		err = -EFAULT;
		goto out;
	}

	err = kstrtoint(strstrip(buffer), 0, &oom_score_adj);
	if (err)
		goto out;
	if (oom_score_adj < OOM_SCORE_ADJ_MIN ||
			oom_score_adj > OOM_SCORE_ADJ_MAX) {
		err = -EINVAL;
		goto out;
	}

<<<<<<< HEAD
	task = get_proc_task(file->f_path.dentry->d_inode);
	if (!task) {
		err = -ESRCH;
		goto out;
	}

	task_lock(task);
	if (!task->mm) {
		err = -EINVAL;
		goto err_task_lock;
	}

	if (!lock_task_sighand(task, &flags)) {
		err = -ESRCH;
		goto err_task_lock;
	}

	if (oom_score_adj < task->signal->oom_score_adj_min &&
			!capable(CAP_SYS_RESOURCE)) {
		err = -EACCES;
		goto err_sighand;
	}

	task->signal->oom_score_adj = oom_score_adj;
	if (has_capability_noaudit(current, CAP_SYS_RESOURCE))
		task->signal->oom_score_adj_min = oom_score_adj;
	trace_oom_score_adj_update(task);
	/*
	 * Scale /proc/pid/oom_adj appropriately ensuring that OOM_DISABLE is
	 * always attainable.
	 */
	if (task->signal->oom_score_adj == OOM_SCORE_ADJ_MIN)
		task->signal->oom_adj = OOM_DISABLE;
	else
		task->signal->oom_adj = (oom_score_adj * OOM_ADJUST_MAX) /
							OOM_SCORE_ADJ_MAX;
err_sighand:
	unlock_task_sighand(task, &flags);
err_task_lock:
	task_unlock(task);
	put_task_struct(task);
=======
	err = __set_oom_adj(file, oom_score_adj, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return err < 0 ? err : count;
}

static const struct file_operations proc_oom_score_adj_operations = {
	.read		= oom_score_adj_read,
	.write		= oom_score_adj_write,
	.llseek		= default_llseek,
};

<<<<<<< HEAD
#ifdef CONFIG_AUDITSYSCALL
#define TMPBUFLEN 21
static ssize_t proc_loginuid_read(struct file * file, char __user * buf,
				  size_t count, loff_t *ppos)
{
	struct inode * inode = file->f_path.dentry->d_inode;
=======
#ifdef CONFIG_AUDIT
#define TMPBUFLEN 11
static ssize_t proc_loginuid_read(struct file * file, char __user * buf,
				  size_t count, loff_t *ppos)
{
	struct inode * inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct task_struct *task = get_proc_task(inode);
	ssize_t length;
	char tmpbuf[TMPBUFLEN];

	if (!task)
		return -ESRCH;
	length = scnprintf(tmpbuf, TMPBUFLEN, "%u",
<<<<<<< HEAD
				audit_get_loginuid(task));
=======
			   from_kuid(file->f_cred->user_ns,
				     audit_get_loginuid(task)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_task_struct(task);
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static ssize_t proc_loginuid_write(struct file * file, const char __user * buf,
				   size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct inode * inode = file->f_path.dentry->d_inode;
	char *page, *tmp;
	ssize_t length;
	uid_t loginuid;
=======
	struct inode * inode = file_inode(file);
	uid_t loginuid;
	kuid_t kloginuid;
	int rv;

	/* Don't let kthreads write their own loginuid */
	if (current->flags & PF_KTHREAD)
		return -EPERM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rcu_read_lock();
	if (current != pid_task(proc_pid(inode), PIDTYPE_PID)) {
		rcu_read_unlock();
		return -EPERM;
	}
	rcu_read_unlock();

<<<<<<< HEAD
	if (count >= PAGE_SIZE)
		count = PAGE_SIZE - 1;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (*ppos != 0) {
		/* No partial writes. */
		return -EINVAL;
	}
<<<<<<< HEAD
	page = (char*)__get_free_page(GFP_TEMPORARY);
	if (!page)
		return -ENOMEM;
	length = -EFAULT;
	if (copy_from_user(page, buf, count))
		goto out_free_page;

	page[count] = '\0';
	loginuid = simple_strtoul(page, &tmp, 10);
	if (tmp == page) {
		length = -EINVAL;
		goto out_free_page;

	}
	length = audit_set_loginuid(loginuid);
	if (likely(length == 0))
		length = count;

out_free_page:
	free_page((unsigned long) page);
	return length;
=======

	rv = kstrtou32_from_user(buf, count, 10, &loginuid);
	if (rv < 0)
		return rv;

	/* is userspace tring to explicitly UNSET the loginuid? */
	if (loginuid == AUDIT_UID_UNSET) {
		kloginuid = INVALID_UID;
	} else {
		kloginuid = make_kuid(file->f_cred->user_ns, loginuid);
		if (!uid_valid(kloginuid))
			return -EINVAL;
	}

	rv = audit_set_loginuid(kloginuid);
	if (rv < 0)
		return rv;
	return count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations proc_loginuid_operations = {
	.read		= proc_loginuid_read,
	.write		= proc_loginuid_write,
	.llseek		= generic_file_llseek,
};

static ssize_t proc_sessionid_read(struct file * file, char __user * buf,
				  size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct inode * inode = file->f_path.dentry->d_inode;
=======
	struct inode * inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct task_struct *task = get_proc_task(inode);
	ssize_t length;
	char tmpbuf[TMPBUFLEN];

	if (!task)
		return -ESRCH;
	length = scnprintf(tmpbuf, TMPBUFLEN, "%u",
				audit_get_sessionid(task));
	put_task_struct(task);
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static const struct file_operations proc_sessionid_operations = {
	.read		= proc_sessionid_read,
	.llseek		= generic_file_llseek,
};
#endif

#ifdef CONFIG_FAULT_INJECTION
static ssize_t proc_fault_inject_read(struct file * file, char __user * buf,
				      size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct task_struct *task = get_proc_task(file->f_dentry->d_inode);
=======
	struct task_struct *task = get_proc_task(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char buffer[PROC_NUMBUF];
	size_t len;
	int make_it_fail;

	if (!task)
		return -ESRCH;
	make_it_fail = task->make_it_fail;
	put_task_struct(task);

	len = snprintf(buffer, sizeof(buffer), "%i\n", make_it_fail);

	return simple_read_from_buffer(buf, count, ppos, buffer, len);
}

static ssize_t proc_fault_inject_write(struct file * file,
			const char __user * buf, size_t count, loff_t *ppos)
{
	struct task_struct *task;
<<<<<<< HEAD
	char buffer[PROC_NUMBUF], *end;
	int make_it_fail;

	if (!capable(CAP_SYS_RESOURCE))
		return -EPERM;
	memset(buffer, 0, sizeof(buffer));
=======
	char buffer[PROC_NUMBUF] = {};
	int make_it_fail;
	int rv;

	if (!capable(CAP_SYS_RESOURCE))
		return -EPERM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;
<<<<<<< HEAD
	make_it_fail = simple_strtol(strstrip(buffer), &end, 0);
	if (*end)
		return -EINVAL;
	task = get_proc_task(file->f_dentry->d_inode);
=======
	rv = kstrtoint(strstrip(buffer), 0, &make_it_fail);
	if (rv < 0)
		return rv;
	if (make_it_fail < 0 || make_it_fail > 1)
		return -EINVAL;

	task = get_proc_task(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!task)
		return -ESRCH;
	task->make_it_fail = make_it_fail;
	put_task_struct(task);

	return count;
}

static const struct file_operations proc_fault_inject_operations = {
	.read		= proc_fault_inject_read,
	.write		= proc_fault_inject_write,
	.llseek		= generic_file_llseek,
};
<<<<<<< HEAD
=======

static ssize_t proc_fail_nth_write(struct file *file, const char __user *buf,
				   size_t count, loff_t *ppos)
{
	struct task_struct *task;
	int err;
	unsigned int n;

	err = kstrtouint_from_user(buf, count, 0, &n);
	if (err)
		return err;

	task = get_proc_task(file_inode(file));
	if (!task)
		return -ESRCH;
	task->fail_nth = n;
	put_task_struct(task);

	return count;
}

static ssize_t proc_fail_nth_read(struct file *file, char __user *buf,
				  size_t count, loff_t *ppos)
{
	struct task_struct *task;
	char numbuf[PROC_NUMBUF];
	ssize_t len;

	task = get_proc_task(file_inode(file));
	if (!task)
		return -ESRCH;
	len = snprintf(numbuf, sizeof(numbuf), "%u\n", task->fail_nth);
	put_task_struct(task);
	return simple_read_from_buffer(buf, count, ppos, numbuf, len);
}

static const struct file_operations proc_fail_nth_operations = {
	.read		= proc_fail_nth_read,
	.write		= proc_fail_nth_write,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif


#ifdef CONFIG_SCHED_DEBUG
/*
 * Print out various scheduling related per-task fields:
 */
static int sched_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
<<<<<<< HEAD
=======
	struct pid_namespace *ns = proc_pid_ns(inode->i_sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct task_struct *p;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;
<<<<<<< HEAD
	proc_sched_show_task(p, m);
=======
	proc_sched_show_task(p, ns, m);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	put_task_struct(p);

	return 0;
}

static ssize_t
sched_write(struct file *file, const char __user *buf,
	    size_t count, loff_t *offset)
{
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct task_struct *p;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;
	proc_sched_set_task(p);

	put_task_struct(p);

	return count;
}

static int sched_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, sched_show, inode);
}

static const struct file_operations proc_pid_sched_operations = {
	.open		= sched_open,
	.read		= seq_read,
	.write		= sched_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif

#ifdef CONFIG_SCHED_AUTOGROUP
/*
 * Print out autogroup related information:
 */
static int sched_autogroup_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
	struct task_struct *p;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;
	proc_sched_autogroup_show_task(p, m);

	put_task_struct(p);

	return 0;
}

static ssize_t
sched_autogroup_write(struct file *file, const char __user *buf,
	    size_t count, loff_t *offset)
{
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
	struct task_struct *p;
	char buffer[PROC_NUMBUF];
	int nice;
	int err;

	memset(buffer, 0, sizeof(buffer));
=======
	struct inode *inode = file_inode(file);
	struct task_struct *p;
	char buffer[PROC_NUMBUF] = {};
	int nice;
	int err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
		return -EFAULT;

	err = kstrtoint(strstrip(buffer), 0, &nice);
	if (err < 0)
		return err;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;

	err = proc_sched_autogroup_set_nice(p, nice);
	if (err)
		count = err;

	put_task_struct(p);

	return count;
}

static int sched_autogroup_open(struct inode *inode, struct file *filp)
{
	int ret;

	ret = single_open(filp, sched_autogroup_show, NULL);
	if (!ret) {
		struct seq_file *m = filp->private_data;

		m->private = inode;
	}
	return ret;
}

static const struct file_operations proc_pid_sched_autogroup_operations = {
	.open		= sched_autogroup_open,
	.read		= seq_read,
	.write		= sched_autogroup_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

#endif /* CONFIG_SCHED_AUTOGROUP */

<<<<<<< HEAD
static ssize_t comm_write(struct file *file, const char __user *buf,
				size_t count, loff_t *offset)
{
	struct inode *inode = file->f_path.dentry->d_inode;
	struct task_struct *p;
	char buffer[TASK_COMM_LEN];

	memset(buffer, 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
=======
#ifdef CONFIG_TIME_NS
static int timens_offsets_show(struct seq_file *m, void *v)
{
	struct task_struct *p;

	p = get_proc_task(file_inode(m->file));
	if (!p)
		return -ESRCH;
	proc_timens_show_offsets(p, m);

	put_task_struct(p);

	return 0;
}

static ssize_t timens_offsets_write(struct file *file, const char __user *buf,
				    size_t count, loff_t *ppos)
{
	struct inode *inode = file_inode(file);
	struct proc_timens_offset offsets[2];
	char *kbuf = NULL, *pos, *next_line;
	struct task_struct *p;
	int ret, noffsets;

	/* Only allow < page size writes at the beginning of the file */
	if ((*ppos != 0) || (count >= PAGE_SIZE))
		return -EINVAL;

	/* Slurp in the user data */
	kbuf = memdup_user_nul(buf, count);
	if (IS_ERR(kbuf))
		return PTR_ERR(kbuf);

	/* Parse the user data */
	ret = -EINVAL;
	noffsets = 0;
	for (pos = kbuf; pos; pos = next_line) {
		struct proc_timens_offset *off = &offsets[noffsets];
		char clock[10];
		int err;

		/* Find the end of line and ensure we don't look past it */
		next_line = strchr(pos, '\n');
		if (next_line) {
			*next_line = '\0';
			next_line++;
			if (*next_line == '\0')
				next_line = NULL;
		}

		err = sscanf(pos, "%9s %lld %lu", clock,
				&off->val.tv_sec, &off->val.tv_nsec);
		if (err != 3 || off->val.tv_nsec >= NSEC_PER_SEC)
			goto out;

		clock[sizeof(clock) - 1] = 0;
		if (strcmp(clock, "monotonic") == 0 ||
		    strcmp(clock, __stringify(CLOCK_MONOTONIC)) == 0)
			off->clockid = CLOCK_MONOTONIC;
		else if (strcmp(clock, "boottime") == 0 ||
			 strcmp(clock, __stringify(CLOCK_BOOTTIME)) == 0)
			off->clockid = CLOCK_BOOTTIME;
		else
			goto out;

		noffsets++;
		if (noffsets == ARRAY_SIZE(offsets)) {
			if (next_line)
				count = next_line - kbuf;
			break;
		}
	}

	ret = -ESRCH;
	p = get_proc_task(inode);
	if (!p)
		goto out;
	ret = proc_timens_set_offset(file, p, offsets, noffsets);
	put_task_struct(p);
	if (ret)
		goto out;

	ret = count;
out:
	kfree(kbuf);
	return ret;
}

static int timens_offsets_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, timens_offsets_show, inode);
}

static const struct file_operations proc_timens_offsets_operations = {
	.open		= timens_offsets_open,
	.read		= seq_read,
	.write		= timens_offsets_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};
#endif /* CONFIG_TIME_NS */

static ssize_t comm_write(struct file *file, const char __user *buf,
				size_t count, loff_t *offset)
{
	struct inode *inode = file_inode(file);
	struct task_struct *p;
	char buffer[TASK_COMM_LEN] = {};
	const size_t maxlen = sizeof(buffer) - 1;

	if (copy_from_user(buffer, buf, count > maxlen ? maxlen : count))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;

<<<<<<< HEAD
	if (same_thread_group(current, p))
		set_task_comm(p, buffer);
=======
	if (same_thread_group(current, p)) {
		set_task_comm(p, buffer);
		proc_comm_connector(p);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		count = -EINVAL;

	put_task_struct(p);

	return count;
}

static int comm_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
	struct task_struct *p;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;

<<<<<<< HEAD
	task_lock(p);
	seq_printf(m, "%s\n", p->comm);
	task_unlock(p);
=======
	proc_task_name(m, p, false);
	seq_putc(m, '\n');
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	put_task_struct(p);

	return 0;
}

static int comm_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, comm_show, inode);
}

static const struct file_operations proc_pid_set_comm_operations = {
	.open		= comm_open,
	.read		= seq_read,
	.write		= comm_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int proc_exe_link(struct dentry *dentry, struct path *exe_path)
{
	struct task_struct *task;
<<<<<<< HEAD
	struct mm_struct *mm;
	struct file *exe_file;

	task = get_proc_task(dentry->d_inode);
	if (!task)
		return -ENOENT;
	mm = get_task_mm(task);
	put_task_struct(task);
	if (!mm)
		return -ENOENT;
	exe_file = get_mm_exe_file(mm);
	mmput(mm);
=======
	struct file *exe_file;

	task = get_proc_task(d_inode(dentry));
	if (!task)
		return -ENOENT;
	exe_file = get_task_exe_file(task);
	put_task_struct(task);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (exe_file) {
		*exe_path = exe_file->f_path;
		path_get(&exe_file->f_path);
		fput(exe_file);
		return 0;
	} else
		return -ENOENT;
}

<<<<<<< HEAD
static void *proc_pid_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct inode *inode = dentry->d_inode;
	struct path path;
	int error = -EACCES;

=======
static const char *proc_pid_get_link(struct dentry *dentry,
				     struct inode *inode,
				     struct delayed_call *done)
{
	struct path path;
	int error = -EACCES;

	if (!dentry)
		return ERR_PTR(-ECHILD);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Are we allowed to snoop on the tasks file descriptors? */
	if (!proc_fd_access_allowed(inode))
		goto out;

	error = PROC_I(inode)->op.proc_get_link(dentry, &path);
	if (error)
		goto out;

<<<<<<< HEAD
	nd_jump_link(nd, &path);
	return NULL;
=======
	error = nd_jump_link(&path);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ERR_PTR(error);
}

<<<<<<< HEAD
static int do_proc_readlink(struct path *path, char __user *buffer, int buflen)
{
	char *tmp = (char*)__get_free_page(GFP_TEMPORARY);
=======
static int do_proc_readlink(const struct path *path, char __user *buffer, int buflen)
{
	char *tmp = kmalloc(PATH_MAX, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *pathname;
	int len;

	if (!tmp)
		return -ENOMEM;

<<<<<<< HEAD
	pathname = d_path(path, tmp, PAGE_SIZE);
	len = PTR_ERR(pathname);
	if (IS_ERR(pathname))
		goto out;
	len = tmp + PAGE_SIZE - 1 - pathname;
=======
	pathname = d_path(path, tmp, PATH_MAX);
	len = PTR_ERR(pathname);
	if (IS_ERR(pathname))
		goto out;
	len = tmp + PATH_MAX - 1 - pathname;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (len > buflen)
		len = buflen;
	if (copy_to_user(buffer, pathname, len))
		len = -EFAULT;
 out:
<<<<<<< HEAD
	free_page((unsigned long)tmp);
=======
	kfree(tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}

static int proc_pid_readlink(struct dentry * dentry, char __user * buffer, int buflen)
{
	int error = -EACCES;
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
=======
	struct inode *inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct path path;

	/* Are we allowed to snoop on the tasks file descriptors? */
	if (!proc_fd_access_allowed(inode))
		goto out;

	error = PROC_I(inode)->op.proc_get_link(dentry, &path);
	if (error)
		goto out;

	error = do_proc_readlink(&path, buffer, buflen);
	path_put(&path);
out:
	return error;
}

<<<<<<< HEAD
static const struct inode_operations proc_pid_link_inode_operations = {
	.readlink	= proc_pid_readlink,
	.follow_link	= proc_pid_follow_link,
=======
const struct inode_operations proc_pid_link_inode_operations = {
	.readlink	= proc_pid_readlink,
	.get_link	= proc_pid_get_link,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.setattr	= proc_setattr,
};


/* building an inode */

<<<<<<< HEAD
static int task_dumpable(struct task_struct *task)
{
	int dumpable = 0;
	struct mm_struct *mm;

	task_lock(task);
	mm = task->mm;
	if (mm)
		dumpable = get_dumpable(mm);
	task_unlock(task);
	if(dumpable == 1)
		return 1;
	return 0;
}

struct inode *proc_pid_make_inode(struct super_block * sb, struct task_struct *task)
{
	struct inode * inode;
	struct proc_inode *ei;
	const struct cred *cred;
=======
void task_dump_owner(struct task_struct *task, umode_t mode,
		     kuid_t *ruid, kgid_t *rgid)
{
	/* Depending on the state of dumpable compute who should own a
	 * proc file for a task.
	 */
	const struct cred *cred;
	kuid_t uid;
	kgid_t gid;

	if (unlikely(task->flags & PF_KTHREAD)) {
		*ruid = GLOBAL_ROOT_UID;
		*rgid = GLOBAL_ROOT_GID;
		return;
	}

	/* Default to the tasks effective ownership */
	rcu_read_lock();
	cred = __task_cred(task);
	uid = cred->euid;
	gid = cred->egid;
	rcu_read_unlock();

	/*
	 * Before the /proc/pid/status file was created the only way to read
	 * the effective uid of a /process was to stat /proc/pid.  Reading
	 * /proc/pid/status is slow enough that procps and other packages
	 * kept stating /proc/pid.  To keep the rules in /proc simple I have
	 * made this apply to all per process world readable and executable
	 * directories.
	 */
	if (mode != (S_IFDIR|S_IRUGO|S_IXUGO)) {
		struct mm_struct *mm;
		task_lock(task);
		mm = task->mm;
		/* Make non-dumpable tasks owned by some root */
		if (mm) {
			if (get_dumpable(mm) != SUID_DUMP_USER) {
				struct user_namespace *user_ns = mm->user_ns;

				uid = make_kuid(user_ns, 0);
				if (!uid_valid(uid))
					uid = GLOBAL_ROOT_UID;

				gid = make_kgid(user_ns, 0);
				if (!gid_valid(gid))
					gid = GLOBAL_ROOT_GID;
			}
		} else {
			uid = GLOBAL_ROOT_UID;
			gid = GLOBAL_ROOT_GID;
		}
		task_unlock(task);
	}
	*ruid = uid;
	*rgid = gid;
}

void proc_pid_evict_inode(struct proc_inode *ei)
{
	struct pid *pid = ei->pid;

	if (S_ISDIR(ei->vfs_inode.i_mode)) {
		spin_lock(&pid->lock);
		hlist_del_init_rcu(&ei->sibling_inodes);
		spin_unlock(&pid->lock);
	}
}

struct inode *proc_pid_make_inode(struct super_block *sb,
				  struct task_struct *task, umode_t mode)
{
	struct inode * inode;
	struct proc_inode *ei;
	struct pid *pid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We need a new inode */

	inode = new_inode(sb);
	if (!inode)
		goto out;

	/* Common stuff */
	ei = PROC_I(inode);
<<<<<<< HEAD
	inode->i_ino = get_next_ino();
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;
=======
	inode->i_mode = mode;
	inode->i_ino = get_next_ino();
	simple_inode_init_ts(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode->i_op = &proc_def_inode_operations;

	/*
	 * grab the reference to task.
	 */
<<<<<<< HEAD
	ei->pid = get_task_pid(task, PIDTYPE_PID);
	if (!ei->pid)
		goto out_unlock;

	if (task_dumpable(task)) {
		rcu_read_lock();
		cred = __task_cred(task);
		inode->i_uid = cred->euid;
		inode->i_gid = cred->egid;
		rcu_read_unlock();
	}
=======
	pid = get_task_pid(task, PIDTYPE_PID);
	if (!pid)
		goto out_unlock;

	/* Let the pid remember us for quick removal */
	ei->pid = pid;

	task_dump_owner(task, 0, &inode->i_uid, &inode->i_gid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	security_task_to_inode(task, inode);

out:
	return inode;

out_unlock:
	iput(inode);
	return NULL;
}

<<<<<<< HEAD
int pid_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat)
{
	struct inode *inode = dentry->d_inode;
	struct task_struct *task;
	const struct cred *cred;
	struct pid_namespace *pid = dentry->d_sb->s_fs_info;

	generic_fillattr(inode, stat);

	rcu_read_lock();
	stat->uid = 0;
	stat->gid = 0;
	task = pid_task(proc_pid(inode), PIDTYPE_PID);
	if (task) {
		if (!has_pid_permissions(pid, task, 2)) {
=======
/*
 * Generating an inode and adding it into @pid->inodes, so that task will
 * invalidate inode's dentry before being released.
 *
 * This helper is used for creating dir-type entries under '/proc' and
 * '/proc/<tgid>/task'. Other entries(eg. fd, stat) under '/proc/<tgid>'
 * can be released by invalidating '/proc/<tgid>' dentry.
 * In theory, dentries under '/proc/<tgid>/task' can also be released by
 * invalidating '/proc/<tgid>' dentry, we reserve it to handle single
 * thread exiting situation: Any one of threads should invalidate its
 * '/proc/<tgid>/task/<pid>' dentry before released.
 */
static struct inode *proc_pid_make_base_inode(struct super_block *sb,
				struct task_struct *task, umode_t mode)
{
	struct inode *inode;
	struct proc_inode *ei;
	struct pid *pid;

	inode = proc_pid_make_inode(sb, task, mode);
	if (!inode)
		return NULL;

	/* Let proc_flush_pid find this directory inode */
	ei = PROC_I(inode);
	pid = ei->pid;
	spin_lock(&pid->lock);
	hlist_add_head_rcu(&ei->sibling_inodes, &pid->inodes);
	spin_unlock(&pid->lock);

	return inode;
}

int pid_getattr(struct mnt_idmap *idmap, const struct path *path,
		struct kstat *stat, u32 request_mask, unsigned int query_flags)
{
	struct inode *inode = d_inode(path->dentry);
	struct proc_fs_info *fs_info = proc_sb_info(inode->i_sb);
	struct task_struct *task;

	generic_fillattr(&nop_mnt_idmap, request_mask, inode, stat);

	stat->uid = GLOBAL_ROOT_UID;
	stat->gid = GLOBAL_ROOT_GID;
	rcu_read_lock();
	task = pid_task(proc_pid(inode), PIDTYPE_PID);
	if (task) {
		if (!has_pid_permissions(fs_info, task, HIDEPID_INVISIBLE)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rcu_read_unlock();
			/*
			 * This doesn't prevent learning whether PID exists,
			 * it only makes getattr() consistent with readdir().
			 */
			return -ENOENT;
		}
<<<<<<< HEAD
		if ((inode->i_mode == (S_IFDIR|S_IRUGO|S_IXUGO)) ||
		    task_dumpable(task)) {
			cred = __task_cred(task);
			stat->uid = cred->euid;
			stat->gid = cred->egid;
		}
=======
		task_dump_owner(task, inode->i_mode, &stat->uid, &stat->gid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	rcu_read_unlock();
	return 0;
}

/* dentry stuff */

/*
<<<<<<< HEAD
 *	Exceptional case: normally we are not allowed to unhash a busy
 * directory. In this case, however, we can do it - no aliasing problems
 * due to the way we treat inodes.
 *
 * Rewrite the inode's ownerships here because the owning task may have
 * performed a setuid(), etc.
 *
 * Before the /proc/pid/status file was created the only way to read
 * the effective uid of a /process was to stat /proc/pid.  Reading
 * /proc/pid/status is slow enough that procps and other packages
 * kept stating /proc/pid.  To keep the rules in /proc simple I have
 * made this apply to all per process world readable and executable
 * directories.
 */
int pid_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct inode *inode;
	struct task_struct *task;
	const struct cred *cred;

	if (flags & LOOKUP_RCU)
		return -ECHILD;

	inode = dentry->d_inode;
	task = get_proc_task(inode);

	if (task) {
		if ((inode->i_mode == (S_IFDIR|S_IRUGO|S_IXUGO)) ||
		    task_dumpable(task)) {
			rcu_read_lock();
			cred = __task_cred(task);
			inode->i_uid = cred->euid;
			inode->i_gid = cred->egid;
			rcu_read_unlock();
		} else {
			inode->i_uid = 0;
			inode->i_gid = 0;
		}
		inode->i_mode &= ~(S_ISUID | S_ISGID);
		security_task_to_inode(task, inode);
		put_task_struct(task);
		return 1;
	}
	d_drop(dentry);
	return 0;
}

static int pid_delete_dentry(const struct dentry * dentry)
=======
 * Set <pid>/... inode ownership (can change due to setuid(), etc.)
 */
void pid_update_inode(struct task_struct *task, struct inode *inode)
{
	task_dump_owner(task, inode->i_mode, &inode->i_uid, &inode->i_gid);

	inode->i_mode &= ~(S_ISUID | S_ISGID);
	security_task_to_inode(task, inode);
}

/*
 * Rewrite the inode's ownerships here because the owning task may have
 * performed a setuid(), etc.
 *
 */
static int pid_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct inode *inode;
	struct task_struct *task;
	int ret = 0;

	rcu_read_lock();
	inode = d_inode_rcu(dentry);
	if (!inode)
		goto out;
	task = pid_task(proc_pid(inode), PIDTYPE_PID);

	if (task) {
		pid_update_inode(task, inode);
		ret = 1;
	}
out:
	rcu_read_unlock();
	return ret;
}

static inline bool proc_inode_is_dead(struct inode *inode)
{
	return !proc_pid(inode)->tasks[PIDTYPE_PID].first;
}

int pid_delete_dentry(const struct dentry *dentry)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Is the task we represent dead?
	 * If so, then don't put the dentry on the lru list,
	 * kill it immediately.
	 */
<<<<<<< HEAD
	return !proc_pid(dentry->d_inode)->tasks[PIDTYPE_PID].first;
=======
	return proc_inode_is_dead(d_inode(dentry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

const struct dentry_operations pid_dentry_operations =
{
	.d_revalidate	= pid_revalidate,
	.d_delete	= pid_delete_dentry,
};

/* Lookups */

/*
 * Fill a directory entry.
 *
 * If possible create the dcache entry and derive our inode number and
 * file type from dcache entry.
 *
 * Since all of the proc inode numbers are dynamically generated, the inode
<<<<<<< HEAD
 * numbers do not exist until the inode is cache.  This means creating the
=======
 * numbers do not exist until the inode is cache.  This means creating
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * the dcache entry in readdir is necessary to keep the inode numbers
 * reported by readdir in sync with the inode numbers reported
 * by stat.
 */
<<<<<<< HEAD
int proc_fill_cache(struct file *filp, void *dirent, filldir_t filldir,
	const char *name, int len,
	instantiate_t instantiate, struct task_struct *task, const void *ptr)
{
	struct dentry *child, *dir = filp->f_path.dentry;
	struct inode *inode;
	struct qstr qname;
	ino_t ino = 0;
	unsigned type = DT_UNKNOWN;

	qname.name = name;
	qname.len  = len;
	qname.hash = full_name_hash(name, len);

	child = d_lookup(dir, &qname);
	if (!child) {
		struct dentry *new;
		new = d_alloc(dir, &qname);
		if (new) {
			child = instantiate(dir->d_inode, new, task, ptr);
			if (child)
				dput(new);
			else
				child = new;
		}
	}
	if (!child || IS_ERR(child) || !child->d_inode)
		goto end_instantiate;
	inode = child->d_inode;
	if (inode) {
		ino = inode->i_ino;
		type = inode->i_mode >> 12;
	}
	dput(child);
end_instantiate:
	if (!ino)
		ino = find_inode_number(dir, &qname);
	if (!ino)
		ino = 1;
	return filldir(dirent, name, len, filp->f_pos, ino, type);
}

static unsigned name_to_int(struct dentry *dentry)
{
	const char *name = dentry->d_name.name;
	int len = dentry->d_name.len;
	unsigned n = 0;

	if (len > 1 && *name == '0')
		goto out;
	while (len-- > 0) {
		unsigned c = *name++ - '0';
		if (c > 9)
			goto out;
		if (n >= (~0U-9)/10)
			goto out;
		n *= 10;
		n += c;
	}
	return n;
out:
	return ~0U;
}

#define PROC_FDINFO_MAX 64

static int proc_fd_info(struct inode *inode, struct path *path, char *info)
{
	struct task_struct *task = get_proc_task(inode);
	struct files_struct *files = NULL;
	struct file *file;
	int fd = proc_fd(inode);

	if (task) {
		files = get_files_struct(task);
		put_task_struct(task);
	}
	if (files) {
		/*
		 * We are not taking a ref to the file structure, so we must
		 * hold ->file_lock.
		 */
		spin_lock(&files->file_lock);
		file = fcheck_files(files, fd);
		if (file) {
			unsigned int f_flags;
			struct fdtable *fdt;

			fdt = files_fdtable(files);
			f_flags = file->f_flags & ~O_CLOEXEC;
			if (close_on_exec(fd, fdt))
				f_flags |= O_CLOEXEC;

			if (path) {
				*path = file->f_path;
				path_get(&file->f_path);
			}
			if (info)
				snprintf(info, PROC_FDINFO_MAX,
					 "pos:\t%lli\n"
					 "flags:\t0%o\n",
					 (long long) file->f_pos,
					 f_flags);
			spin_unlock(&files->file_lock);
			put_files_struct(files);
			return 0;
		}
		spin_unlock(&files->file_lock);
		put_files_struct(files);
	}
	return -ENOENT;
}

static int proc_fd_link(struct dentry *dentry, struct path *path)
{
	return proc_fd_info(dentry->d_inode, path, NULL);
}

static int tid_fd_revalidate(struct dentry *dentry, unsigned int flags)
{
	struct inode *inode;
	struct task_struct *task;
	int fd;
	struct files_struct *files;
	const struct cred *cred;

	if (flags & LOOKUP_RCU)
		return -ECHILD;

	inode = dentry->d_inode;
	task = get_proc_task(inode);
	fd = proc_fd(inode);

	if (task) {
		files = get_files_struct(task);
		if (files) {
			struct file *file;
			rcu_read_lock();
			file = fcheck_files(files, fd);
			if (file) {
				unsigned f_mode = file->f_mode;

				rcu_read_unlock();
				put_files_struct(files);

				if (task_dumpable(task)) {
					rcu_read_lock();
					cred = __task_cred(task);
					inode->i_uid = cred->euid;
					inode->i_gid = cred->egid;
					rcu_read_unlock();
				} else {
					inode->i_uid = 0;
					inode->i_gid = 0;
				}

				if (S_ISLNK(inode->i_mode)) {
					unsigned i_mode = S_IFLNK;
					if (f_mode & FMODE_READ)
						i_mode |= S_IRUSR | S_IXUSR;
					if (f_mode & FMODE_WRITE)
						i_mode |= S_IWUSR | S_IXUSR;
					inode->i_mode = i_mode;
				}

				security_task_to_inode(task, inode);
				put_task_struct(task);
				return 1;
			}
			rcu_read_unlock();
			put_files_struct(files);
		}
		put_task_struct(task);
	}
	d_drop(dentry);
	return 0;
}

static const struct dentry_operations tid_fd_dentry_operations =
{
	.d_revalidate	= tid_fd_revalidate,
	.d_delete	= pid_delete_dentry,
};

static struct dentry *proc_fd_instantiate(struct inode *dir,
	struct dentry *dentry, struct task_struct *task, const void *ptr)
{
	unsigned fd = *(const unsigned *)ptr;
 	struct inode *inode;
 	struct proc_inode *ei;
	struct dentry *error = ERR_PTR(-ENOENT);

	inode = proc_pid_make_inode(dir->i_sb, task);
	if (!inode)
		goto out;
	ei = PROC_I(inode);
	ei->fd = fd;

	inode->i_mode = S_IFLNK;
	inode->i_op = &proc_pid_link_inode_operations;
	inode->i_size = 64;
	ei->op.proc_get_link = proc_fd_link;
	d_set_d_op(dentry, &tid_fd_dentry_operations);
	d_add(dentry, inode);
	/* Close the race of the process dying before we return the dentry */
	if (tid_fd_revalidate(dentry, 0))
		error = NULL;

 out:
	return error;
}

static struct dentry *proc_lookupfd_common(struct inode *dir,
					   struct dentry *dentry,
					   instantiate_t instantiate)
{
	struct task_struct *task = get_proc_task(dir);
	unsigned fd = name_to_int(dentry);
	struct dentry *result = ERR_PTR(-ENOENT);

	if (!task)
		goto out_no_task;
	if (fd == ~0U)
		goto out;

	result = instantiate(dir, dentry, task, &fd);
out:
	put_task_struct(task);
out_no_task:
	return result;
}

static int proc_readfd_common(struct file * filp, void * dirent,
			      filldir_t filldir, instantiate_t instantiate)
{
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	struct task_struct *p = get_proc_task(inode);
	unsigned int fd, ino;
	int retval;
	struct files_struct * files;

	retval = -ENOENT;
	if (!p)
		goto out_no_task;
	retval = 0;

	fd = filp->f_pos;
	switch (fd) {
		case 0:
			if (filldir(dirent, ".", 1, 0, inode->i_ino, DT_DIR) < 0)
				goto out;
			filp->f_pos++;
		case 1:
			ino = parent_ino(dentry);
			if (filldir(dirent, "..", 2, 1, ino, DT_DIR) < 0)
				goto out;
			filp->f_pos++;
		default:
			files = get_files_struct(p);
			if (!files)
				goto out;
			rcu_read_lock();
			for (fd = filp->f_pos-2;
			     fd < files_fdtable(files)->max_fds;
			     fd++, filp->f_pos++) {
				char name[PROC_NUMBUF];
				int len;

				if (!fcheck_files(files, fd))
					continue;
				rcu_read_unlock();

				len = snprintf(name, sizeof(name), "%d", fd);
				if (proc_fill_cache(filp, dirent, filldir,
						    name, len, instantiate,
						    p, &fd) < 0) {
					rcu_read_lock();
					break;
				}
				rcu_read_lock();
			}
			rcu_read_unlock();
			put_files_struct(files);
	}
out:
	put_task_struct(p);
out_no_task:
	return retval;
}

static struct dentry *proc_lookupfd(struct inode *dir, struct dentry *dentry,
				    unsigned int flags)
{
	return proc_lookupfd_common(dir, dentry, proc_fd_instantiate);
}

static int proc_readfd(struct file *filp, void *dirent, filldir_t filldir)
{
	return proc_readfd_common(filp, dirent, filldir, proc_fd_instantiate);
}

static ssize_t proc_fdinfo_read(struct file *file, char __user *buf,
				      size_t len, loff_t *ppos)
{
	char tmp[PROC_FDINFO_MAX];
	int err = proc_fd_info(file->f_path.dentry->d_inode, NULL, tmp);
	if (!err)
		err = simple_read_from_buffer(buf, len, ppos, tmp, strlen(tmp));
	return err;
}

static const struct file_operations proc_fdinfo_file_operations = {
	.open           = nonseekable_open,
	.read		= proc_fdinfo_read,
	.llseek		= no_llseek,
};

static const struct file_operations proc_fd_operations = {
	.read		= generic_read_dir,
	.readdir	= proc_readfd,
	.llseek		= default_llseek,
};

#ifdef CONFIG_CHECKPOINT_RESTORE

=======
bool proc_fill_cache(struct file *file, struct dir_context *ctx,
	const char *name, unsigned int len,
	instantiate_t instantiate, struct task_struct *task, const void *ptr)
{
	struct dentry *child, *dir = file->f_path.dentry;
	struct qstr qname = QSTR_INIT(name, len);
	struct inode *inode;
	unsigned type = DT_UNKNOWN;
	ino_t ino = 1;

	child = d_hash_and_lookup(dir, &qname);
	if (!child) {
		DECLARE_WAIT_QUEUE_HEAD_ONSTACK(wq);
		child = d_alloc_parallel(dir, &qname, &wq);
		if (IS_ERR(child))
			goto end_instantiate;
		if (d_in_lookup(child)) {
			struct dentry *res;
			res = instantiate(child, task, ptr);
			d_lookup_done(child);
			if (unlikely(res)) {
				dput(child);
				child = res;
				if (IS_ERR(child))
					goto end_instantiate;
			}
		}
	}
	inode = d_inode(child);
	ino = inode->i_ino;
	type = inode->i_mode >> 12;
	dput(child);
end_instantiate:
	return dir_emit(ctx, name, len, ino, type);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * dname_to_vma_addr - maps a dentry name into two unsigned longs
 * which represent vma start and end addresses.
 */
static int dname_to_vma_addr(struct dentry *dentry,
			     unsigned long *start, unsigned long *end)
{
<<<<<<< HEAD
	if (sscanf(dentry->d_name.name, "%lx-%lx", start, end) != 2)
		return -EINVAL;
=======
	const char *str = dentry->d_name.name;
	unsigned long long sval, eval;
	unsigned int len;

	if (str[0] == '0' && str[1] != '-')
		return -EINVAL;
	len = _parse_integer(str, 16, &sval);
	if (len & KSTRTOX_OVERFLOW)
		return -EINVAL;
	if (sval != (unsigned long)sval)
		return -EINVAL;
	str += len;

	if (*str != '-')
		return -EINVAL;
	str++;

	if (str[0] == '0' && str[1])
		return -EINVAL;
	len = _parse_integer(str, 16, &eval);
	if (len & KSTRTOX_OVERFLOW)
		return -EINVAL;
	if (eval != (unsigned long)eval)
		return -EINVAL;
	str += len;

	if (*str != '\0')
		return -EINVAL;

	*start = sval;
	*end = eval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int map_files_d_revalidate(struct dentry *dentry, unsigned int flags)
{
	unsigned long vm_start, vm_end;
	bool exact_vma_exists = false;
	struct mm_struct *mm = NULL;
	struct task_struct *task;
<<<<<<< HEAD
	const struct cred *cred;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode *inode;
	int status = 0;

	if (flags & LOOKUP_RCU)
		return -ECHILD;

<<<<<<< HEAD
	if (!capable(CAP_SYS_ADMIN)) {
		status = -EACCES;
		goto out_notask;
	}

	inode = dentry->d_inode;
=======
	inode = d_inode(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	task = get_proc_task(inode);
	if (!task)
		goto out_notask;

<<<<<<< HEAD
	if (!ptrace_may_access(task, PTRACE_MODE_READ))
		goto out;

	mm = get_task_mm(task);
	if (!mm)
		goto out;

	if (!dname_to_vma_addr(dentry, &vm_start, &vm_end)) {
		down_read(&mm->mmap_sem);
		exact_vma_exists = !!find_exact_vma(mm, vm_start, vm_end);
		up_read(&mm->mmap_sem);
=======
	mm = mm_access(task, PTRACE_MODE_READ_FSCREDS);
	if (IS_ERR_OR_NULL(mm))
		goto out;

	if (!dname_to_vma_addr(dentry, &vm_start, &vm_end)) {
		status = mmap_read_lock_killable(mm);
		if (!status) {
			exact_vma_exists = !!find_exact_vma(mm, vm_start,
							    vm_end);
			mmap_read_unlock(mm);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	mmput(mm);

	if (exact_vma_exists) {
<<<<<<< HEAD
		if (task_dumpable(task)) {
			rcu_read_lock();
			cred = __task_cred(task);
			inode->i_uid = cred->euid;
			inode->i_gid = cred->egid;
			rcu_read_unlock();
		} else {
			inode->i_uid = 0;
			inode->i_gid = 0;
		}
=======
		task_dump_owner(task, 0, &inode->i_uid, &inode->i_gid);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		security_task_to_inode(task, inode);
		status = 1;
	}

out:
	put_task_struct(task);

out_notask:
<<<<<<< HEAD
	if (status <= 0)
		d_drop(dentry);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return status;
}

static const struct dentry_operations tid_map_files_dentry_operations = {
	.d_revalidate	= map_files_d_revalidate,
	.d_delete	= pid_delete_dentry,
};

<<<<<<< HEAD
static int proc_map_files_get_link(struct dentry *dentry, struct path *path)
=======
static int map_files_get_link(struct dentry *dentry, struct path *path)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long vm_start, vm_end;
	struct vm_area_struct *vma;
	struct task_struct *task;
	struct mm_struct *mm;
	int rc;

	rc = -ENOENT;
<<<<<<< HEAD
	task = get_proc_task(dentry->d_inode);
=======
	task = get_proc_task(d_inode(dentry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!task)
		goto out;

	mm = get_task_mm(task);
	put_task_struct(task);
	if (!mm)
		goto out;

	rc = dname_to_vma_addr(dentry, &vm_start, &vm_end);
	if (rc)
		goto out_mmput;

<<<<<<< HEAD
	rc = -ENOENT;
	down_read(&mm->mmap_sem);
	vma = find_exact_vma(mm, vm_start, vm_end);
	if (vma && vma->vm_file) {
		*path = vma->vm_file->f_path;
		path_get(path);
		rc = 0;
	}
	up_read(&mm->mmap_sem);
=======
	rc = mmap_read_lock_killable(mm);
	if (rc)
		goto out_mmput;

	rc = -ENOENT;
	vma = find_exact_vma(mm, vm_start, vm_end);
	if (vma && vma->vm_file) {
		*path = *file_user_path(vma->vm_file);
		path_get(path);
		rc = 0;
	}
	mmap_read_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_mmput:
	mmput(mm);
out:
	return rc;
}

struct map_files_info {
<<<<<<< HEAD
	struct file	*file;
	unsigned long	len;
	unsigned char	name[4*sizeof(long)+2]; /* max: %lx-%lx\0 */
};

static struct dentry *
proc_map_files_instantiate(struct inode *dir, struct dentry *dentry,
			   struct task_struct *task, const void *ptr)
{
	const struct file *file = ptr;
	struct proc_inode *ei;
	struct inode *inode;

	if (!file)
		return ERR_PTR(-ENOENT);

	inode = proc_pid_make_inode(dir->i_sb, task);
=======
	unsigned long	start;
	unsigned long	end;
	fmode_t		mode;
};

/*
 * Only allow CAP_SYS_ADMIN and CAP_CHECKPOINT_RESTORE to follow the links, due
 * to concerns about how the symlinks may be used to bypass permissions on
 * ancestor directories in the path to the file in question.
 */
static const char *
proc_map_files_get_link(struct dentry *dentry,
			struct inode *inode,
		        struct delayed_call *done)
{
	if (!checkpoint_restore_ns_capable(&init_user_ns))
		return ERR_PTR(-EPERM);

	return proc_pid_get_link(dentry, inode, done);
}

/*
 * Identical to proc_pid_link_inode_operations except for get_link()
 */
static const struct inode_operations proc_map_files_link_inode_operations = {
	.readlink	= proc_pid_readlink,
	.get_link	= proc_map_files_get_link,
	.setattr	= proc_setattr,
};

static struct dentry *
proc_map_files_instantiate(struct dentry *dentry,
			   struct task_struct *task, const void *ptr)
{
	fmode_t mode = (fmode_t)(unsigned long)ptr;
	struct proc_inode *ei;
	struct inode *inode;

	inode = proc_pid_make_inode(dentry->d_sb, task, S_IFLNK |
				    ((mode & FMODE_READ ) ? S_IRUSR : 0) |
				    ((mode & FMODE_WRITE) ? S_IWUSR : 0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!inode)
		return ERR_PTR(-ENOENT);

	ei = PROC_I(inode);
<<<<<<< HEAD
	ei->op.proc_get_link = proc_map_files_get_link;

	inode->i_op = &proc_pid_link_inode_operations;
	inode->i_size = 64;
	inode->i_mode = S_IFLNK;

	if (file->f_mode & FMODE_READ)
		inode->i_mode |= S_IRUSR;
	if (file->f_mode & FMODE_WRITE)
		inode->i_mode |= S_IWUSR;

	d_set_d_op(dentry, &tid_map_files_dentry_operations);
	d_add(dentry, inode);

	return NULL;
=======
	ei->op.proc_get_link = map_files_get_link;

	inode->i_op = &proc_map_files_link_inode_operations;
	inode->i_size = 64;

	d_set_d_op(dentry, &tid_map_files_dentry_operations);
	return d_splice_alias(inode, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dentry *proc_map_files_lookup(struct inode *dir,
		struct dentry *dentry, unsigned int flags)
{
	unsigned long vm_start, vm_end;
	struct vm_area_struct *vma;
	struct task_struct *task;
	struct dentry *result;
	struct mm_struct *mm;

<<<<<<< HEAD
	result = ERR_PTR(-EACCES);
	if (!capable(CAP_SYS_ADMIN))
		goto out;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	result = ERR_PTR(-ENOENT);
	task = get_proc_task(dir);
	if (!task)
		goto out;

	result = ERR_PTR(-EACCES);
<<<<<<< HEAD
	if (!ptrace_may_access(task, PTRACE_MODE_READ))
=======
	if (!ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_put_task;

	result = ERR_PTR(-ENOENT);
	if (dname_to_vma_addr(dentry, &vm_start, &vm_end))
		goto out_put_task;

	mm = get_task_mm(task);
	if (!mm)
		goto out_put_task;

<<<<<<< HEAD
	down_read(&mm->mmap_sem);
=======
	result = ERR_PTR(-EINTR);
	if (mmap_read_lock_killable(mm))
		goto out_put_mm;

	result = ERR_PTR(-ENOENT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vma = find_exact_vma(mm, vm_start, vm_end);
	if (!vma)
		goto out_no_vma;

<<<<<<< HEAD
	result = proc_map_files_instantiate(dir, dentry, task, vma->vm_file);

out_no_vma:
	up_read(&mm->mmap_sem);
=======
	if (vma->vm_file)
		result = proc_map_files_instantiate(dentry, task,
				(void *)(unsigned long)vma->vm_file->f_mode);

out_no_vma:
	mmap_read_unlock(mm);
out_put_mm:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mmput(mm);
out_put_task:
	put_task_struct(task);
out:
	return result;
}

static const struct inode_operations proc_map_files_inode_operations = {
	.lookup		= proc_map_files_lookup,
	.permission	= proc_fd_permission,
	.setattr	= proc_setattr,
};

static int
<<<<<<< HEAD
proc_map_files_readdir(struct file *filp, void *dirent, filldir_t filldir)
{
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	struct vm_area_struct *vma;
	struct task_struct *task;
	struct mm_struct *mm;
	ino_t ino;
	int ret;

	ret = -EACCES;
	if (!capable(CAP_SYS_ADMIN))
		goto out;

	ret = -ENOENT;
	task = get_proc_task(inode);
=======
proc_map_files_readdir(struct file *file, struct dir_context *ctx)
{
	struct vm_area_struct *vma;
	struct task_struct *task;
	struct mm_struct *mm;
	unsigned long nr_files, pos, i;
	GENRADIX(struct map_files_info) fa;
	struct map_files_info *p;
	int ret;
	struct vma_iterator vmi;

	genradix_init(&fa);

	ret = -ENOENT;
	task = get_proc_task(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!task)
		goto out;

	ret = -EACCES;
<<<<<<< HEAD
	if (!ptrace_may_access(task, PTRACE_MODE_READ))
		goto out_put_task;

	ret = 0;
	switch (filp->f_pos) {
	case 0:
		ino = inode->i_ino;
		if (filldir(dirent, ".", 1, 0, ino, DT_DIR) < 0)
			goto out_put_task;
		filp->f_pos++;
	case 1:
		ino = parent_ino(dentry);
		if (filldir(dirent, "..", 2, 1, ino, DT_DIR) < 0)
			goto out_put_task;
		filp->f_pos++;
	default:
	{
		unsigned long nr_files, pos, i;
		struct flex_array *fa = NULL;
		struct map_files_info info;
		struct map_files_info *p;

		mm = get_task_mm(task);
		if (!mm)
			goto out_put_task;
		down_read(&mm->mmap_sem);

		nr_files = 0;

		/*
		 * We need two passes here:
		 *
		 *  1) Collect vmas of mapped files with mmap_sem taken
		 *  2) Release mmap_sem and instantiate entries
		 *
		 * otherwise we get lockdep complained, since filldir()
		 * routine might require mmap_sem taken in might_fault().
		 */

		for (vma = mm->mmap, pos = 2; vma; vma = vma->vm_next) {
			if (vma->vm_file && ++pos > filp->f_pos)
				nr_files++;
		}

		if (nr_files) {
			fa = flex_array_alloc(sizeof(info), nr_files,
						GFP_KERNEL);
			if (!fa || flex_array_prealloc(fa, 0, nr_files,
							GFP_KERNEL)) {
				ret = -ENOMEM;
				if (fa)
					flex_array_free(fa);
				up_read(&mm->mmap_sem);
				mmput(mm);
				goto out_put_task;
			}
			for (i = 0, vma = mm->mmap, pos = 2; vma;
					vma = vma->vm_next) {
				if (!vma->vm_file)
					continue;
				if (++pos <= filp->f_pos)
					continue;

				get_file(vma->vm_file);
				info.file = vma->vm_file;
				info.len = snprintf(info.name,
						sizeof(info.name), "%lx-%lx",
						vma->vm_start, vma->vm_end);
				if (flex_array_put(fa, i++, &info, GFP_KERNEL))
					BUG();
			}
		}
		up_read(&mm->mmap_sem);

		for (i = 0; i < nr_files; i++) {
			p = flex_array_get(fa, i);
			ret = proc_fill_cache(filp, dirent, filldir,
					      p->name, p->len,
					      proc_map_files_instantiate,
					      task, p->file);
			if (ret)
				break;
			filp->f_pos++;
			fput(p->file);
		}
		for (; i < nr_files; i++) {
			/*
			 * In case of error don't forget
			 * to put rest of file refs.
			 */
			p = flex_array_get(fa, i);
			fput(p->file);
		}
		if (fa)
			flex_array_free(fa);
		mmput(mm);
	}
=======
	if (!ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS))
		goto out_put_task;

	ret = 0;
	if (!dir_emit_dots(file, ctx))
		goto out_put_task;

	mm = get_task_mm(task);
	if (!mm)
		goto out_put_task;

	ret = mmap_read_lock_killable(mm);
	if (ret) {
		mmput(mm);
		goto out_put_task;
	}

	nr_files = 0;

	/*
	 * We need two passes here:
	 *
	 *  1) Collect vmas of mapped files with mmap_lock taken
	 *  2) Release mmap_lock and instantiate entries
	 *
	 * otherwise we get lockdep complained, since filldir()
	 * routine might require mmap_lock taken in might_fault().
	 */

	pos = 2;
	vma_iter_init(&vmi, mm, 0);
	for_each_vma(vmi, vma) {
		if (!vma->vm_file)
			continue;
		if (++pos <= ctx->pos)
			continue;

		p = genradix_ptr_alloc(&fa, nr_files++, GFP_KERNEL);
		if (!p) {
			ret = -ENOMEM;
			mmap_read_unlock(mm);
			mmput(mm);
			goto out_put_task;
		}

		p->start = vma->vm_start;
		p->end = vma->vm_end;
		p->mode = vma->vm_file->f_mode;
	}
	mmap_read_unlock(mm);
	mmput(mm);

	for (i = 0; i < nr_files; i++) {
		char buf[4 * sizeof(long) + 2];	/* max: %lx-%lx\0 */
		unsigned int len;

		p = genradix_ptr(&fa, i);
		len = snprintf(buf, sizeof(buf), "%lx-%lx", p->start, p->end);
		if (!proc_fill_cache(file, ctx,
				      buf, len,
				      proc_map_files_instantiate,
				      task,
				      (void *)(unsigned long)p->mode))
			break;
		ctx->pos++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

out_put_task:
	put_task_struct(task);
out:
<<<<<<< HEAD
=======
	genradix_free(&fa);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static const struct file_operations proc_map_files_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= proc_map_files_readdir,
	.llseek		= default_llseek,
};

#endif /* CONFIG_CHECKPOINT_RESTORE */

/*
 * /proc/pid/fd needs a special permission handler so that a process can still
 * access /proc/self/fd after it has executed a setuid().
 */
static int proc_fd_permission(struct inode *inode, int mask)
{
	struct task_struct *p;
	int rv;

	rv = generic_permission(inode, mask);
	if (rv == 0)
		return rv;

	rcu_read_lock();
	p = pid_task(proc_pid(inode), PIDTYPE_PID);
	if (p && same_thread_group(p, current))
		rv = 0;
	rcu_read_unlock();

	return rv;
}

/*
 * proc directories can do almost nothing..
 */
static const struct inode_operations proc_fd_inode_operations = {
	.lookup		= proc_lookupfd,
	.permission	= proc_fd_permission,
	.setattr	= proc_setattr,
};

static struct dentry *proc_fdinfo_instantiate(struct inode *dir,
	struct dentry *dentry, struct task_struct *task, const void *ptr)
{
	unsigned fd = *(unsigned *)ptr;
 	struct inode *inode;
 	struct proc_inode *ei;
	struct dentry *error = ERR_PTR(-ENOENT);

	inode = proc_pid_make_inode(dir->i_sb, task);
	if (!inode)
		goto out;
	ei = PROC_I(inode);
	ei->fd = fd;
	inode->i_mode = S_IFREG | S_IRUSR;
	inode->i_fop = &proc_fdinfo_file_operations;
	d_set_d_op(dentry, &tid_fd_dentry_operations);
	d_add(dentry, inode);
	/* Close the race of the process dying before we return the dentry */
	if (tid_fd_revalidate(dentry, 0))
		error = NULL;

 out:
	return error;
}

static struct dentry *proc_lookupfdinfo(struct inode *dir,
					struct dentry *dentry,
					unsigned int flags)
{
	return proc_lookupfd_common(dir, dentry, proc_fdinfo_instantiate);
}

static int proc_readfdinfo(struct file *filp, void *dirent, filldir_t filldir)
{
	return proc_readfd_common(filp, dirent, filldir,
				  proc_fdinfo_instantiate);
}

static const struct file_operations proc_fdinfo_operations = {
	.read		= generic_read_dir,
	.readdir	= proc_readfdinfo,
	.llseek		= default_llseek,
};

/*
 * proc directories can do almost nothing..
 */
static const struct inode_operations proc_fdinfo_inode_operations = {
	.lookup		= proc_lookupfdinfo,
	.setattr	= proc_setattr,
};


static struct dentry *proc_pident_instantiate(struct inode *dir,
	struct dentry *dentry, struct task_struct *task, const void *ptr)
=======
	.iterate_shared	= proc_map_files_readdir,
	.llseek		= generic_file_llseek,
};

#if defined(CONFIG_CHECKPOINT_RESTORE) && defined(CONFIG_POSIX_TIMERS)
struct timers_private {
	struct pid *pid;
	struct task_struct *task;
	struct sighand_struct *sighand;
	struct pid_namespace *ns;
	unsigned long flags;
};

static void *timers_start(struct seq_file *m, loff_t *pos)
{
	struct timers_private *tp = m->private;

	tp->task = get_pid_task(tp->pid, PIDTYPE_PID);
	if (!tp->task)
		return ERR_PTR(-ESRCH);

	tp->sighand = lock_task_sighand(tp->task, &tp->flags);
	if (!tp->sighand)
		return ERR_PTR(-ESRCH);

	return seq_list_start(&tp->task->signal->posix_timers, *pos);
}

static void *timers_next(struct seq_file *m, void *v, loff_t *pos)
{
	struct timers_private *tp = m->private;
	return seq_list_next(v, &tp->task->signal->posix_timers, pos);
}

static void timers_stop(struct seq_file *m, void *v)
{
	struct timers_private *tp = m->private;

	if (tp->sighand) {
		unlock_task_sighand(tp->task, &tp->flags);
		tp->sighand = NULL;
	}

	if (tp->task) {
		put_task_struct(tp->task);
		tp->task = NULL;
	}
}

static int show_timer(struct seq_file *m, void *v)
{
	struct k_itimer *timer;
	struct timers_private *tp = m->private;
	int notify;
	static const char * const nstr[] = {
		[SIGEV_SIGNAL] = "signal",
		[SIGEV_NONE] = "none",
		[SIGEV_THREAD] = "thread",
	};

	timer = list_entry((struct list_head *)v, struct k_itimer, list);
	notify = timer->it_sigev_notify;

	seq_printf(m, "ID: %d\n", timer->it_id);
	seq_printf(m, "signal: %d/%px\n",
		   timer->sigq->info.si_signo,
		   timer->sigq->info.si_value.sival_ptr);
	seq_printf(m, "notify: %s/%s.%d\n",
		   nstr[notify & ~SIGEV_THREAD_ID],
		   (notify & SIGEV_THREAD_ID) ? "tid" : "pid",
		   pid_nr_ns(timer->it_pid, tp->ns));
	seq_printf(m, "ClockID: %d\n", timer->it_clock);

	return 0;
}

static const struct seq_operations proc_timers_seq_ops = {
	.start	= timers_start,
	.next	= timers_next,
	.stop	= timers_stop,
	.show	= show_timer,
};

static int proc_timers_open(struct inode *inode, struct file *file)
{
	struct timers_private *tp;

	tp = __seq_open_private(file, &proc_timers_seq_ops,
			sizeof(struct timers_private));
	if (!tp)
		return -ENOMEM;

	tp->pid = proc_pid(inode);
	tp->ns = proc_pid_ns(inode->i_sb);
	return 0;
}

static const struct file_operations proc_timers_operations = {
	.open		= proc_timers_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release_private,
};
#endif

static ssize_t timerslack_ns_write(struct file *file, const char __user *buf,
					size_t count, loff_t *offset)
{
	struct inode *inode = file_inode(file);
	struct task_struct *p;
	u64 slack_ns;
	int err;

	err = kstrtoull_from_user(buf, count, 10, &slack_ns);
	if (err < 0)
		return err;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;

	if (p != current) {
		rcu_read_lock();
		if (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE)) {
			rcu_read_unlock();
			count = -EPERM;
			goto out;
		}
		rcu_read_unlock();

		err = security_task_setscheduler(p);
		if (err) {
			count = err;
			goto out;
		}
	}

	task_lock(p);
	if (slack_ns == 0)
		p->timer_slack_ns = p->default_timer_slack_ns;
	else
		p->timer_slack_ns = slack_ns;
	task_unlock(p);

out:
	put_task_struct(p);

	return count;
}

static int timerslack_ns_show(struct seq_file *m, void *v)
{
	struct inode *inode = m->private;
	struct task_struct *p;
	int err = 0;

	p = get_proc_task(inode);
	if (!p)
		return -ESRCH;

	if (p != current) {
		rcu_read_lock();
		if (!ns_capable(__task_cred(p)->user_ns, CAP_SYS_NICE)) {
			rcu_read_unlock();
			err = -EPERM;
			goto out;
		}
		rcu_read_unlock();

		err = security_task_getscheduler(p);
		if (err)
			goto out;
	}

	task_lock(p);
	seq_printf(m, "%llu\n", p->timer_slack_ns);
	task_unlock(p);

out:
	put_task_struct(p);

	return err;
}

static int timerslack_ns_open(struct inode *inode, struct file *filp)
{
	return single_open(filp, timerslack_ns_show, inode);
}

static const struct file_operations proc_pid_set_timerslack_ns_operations = {
	.open		= timerslack_ns_open,
	.read		= seq_read,
	.write		= timerslack_ns_write,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static struct dentry *proc_pident_instantiate(struct dentry *dentry,
	struct task_struct *task, const void *ptr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct pid_entry *p = ptr;
	struct inode *inode;
	struct proc_inode *ei;
<<<<<<< HEAD
	struct dentry *error = ERR_PTR(-ENOENT);

	inode = proc_pid_make_inode(dir->i_sb, task);
	if (!inode)
		goto out;

	ei = PROC_I(inode);
	inode->i_mode = p->mode;
=======

	inode = proc_pid_make_inode(dentry->d_sb, task, p->mode);
	if (!inode)
		return ERR_PTR(-ENOENT);

	ei = PROC_I(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (S_ISDIR(inode->i_mode))
		set_nlink(inode, 2);	/* Use getattr to fix if necessary */
	if (p->iop)
		inode->i_op = p->iop;
	if (p->fop)
		inode->i_fop = p->fop;
	ei->op = p->op;
<<<<<<< HEAD
	d_set_d_op(dentry, &pid_dentry_operations);
	d_add(dentry, inode);
	/* Close the race of the process dying before we return the dentry */
	if (pid_revalidate(dentry, 0))
		error = NULL;
out:
	return error;
=======
	pid_update_inode(task, inode);
	d_set_d_op(dentry, &pid_dentry_operations);
	return d_splice_alias(inode, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dentry *proc_pident_lookup(struct inode *dir, 
					 struct dentry *dentry,
<<<<<<< HEAD
					 const struct pid_entry *ents,
					 unsigned int nents)
{
	struct dentry *error;
	struct task_struct *task = get_proc_task(dir);
	const struct pid_entry *p, *last;

	error = ERR_PTR(-ENOENT);
=======
					 const struct pid_entry *p,
					 const struct pid_entry *end)
{
	struct task_struct *task = get_proc_task(dir);
	struct dentry *res = ERR_PTR(-ENOENT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!task)
		goto out_no_task;

	/*
	 * Yes, it does not scale. And it should not. Don't add
	 * new entries into /proc/<tgid>/ without very good reasons.
	 */
<<<<<<< HEAD
	last = &ents[nents - 1];
	for (p = ents; p <= last; p++) {
		if (p->len != dentry->d_name.len)
			continue;
		if (!memcmp(dentry->d_name.name, p->name, p->len))
			break;
	}
	if (p > last)
		goto out;

	error = proc_pident_instantiate(dir, dentry, task, p);
out:
	put_task_struct(task);
out_no_task:
	return error;
}

static int proc_pident_fill_cache(struct file *filp, void *dirent,
	filldir_t filldir, struct task_struct *task, const struct pid_entry *p)
{
	return proc_fill_cache(filp, dirent, filldir, p->name, p->len,
				proc_pident_instantiate, task, p);
}

static int proc_pident_readdir(struct file *filp,
		void *dirent, filldir_t filldir,
		const struct pid_entry *ents, unsigned int nents)
{
	int i;
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	struct task_struct *task = get_proc_task(inode);
	const struct pid_entry *p, *last;
	ino_t ino;
	int ret;

	ret = -ENOENT;
	if (!task)
		goto out_no_task;

	ret = 0;
	i = filp->f_pos;
	switch (i) {
	case 0:
		ino = inode->i_ino;
		if (filldir(dirent, ".", 1, i, ino, DT_DIR) < 0)
			goto out;
		i++;
		filp->f_pos++;
		/* fall through */
	case 1:
		ino = parent_ino(dentry);
		if (filldir(dirent, "..", 2, i, ino, DT_DIR) < 0)
			goto out;
		i++;
		filp->f_pos++;
		/* fall through */
	default:
		i -= 2;
		if (i >= nents) {
			ret = 1;
			goto out;
		}
		p = ents + i;
		last = &ents[nents - 1];
		while (p <= last) {
			if (proc_pident_fill_cache(filp, dirent, filldir, task, p) < 0)
				goto out;
			filp->f_pos++;
			p++;
		}
	}

	ret = 1;
out:
	put_task_struct(task);
out_no_task:
	return ret;
}

#ifdef CONFIG_SECURITY
static ssize_t proc_pid_attr_read(struct file * file, char __user * buf,
				  size_t count, loff_t *ppos)
{
	struct inode * inode = file->f_path.dentry->d_inode;
=======
	for (; p < end; p++) {
		if (p->len != dentry->d_name.len)
			continue;
		if (!memcmp(dentry->d_name.name, p->name, p->len)) {
			res = proc_pident_instantiate(dentry, task, p);
			break;
		}
	}
	put_task_struct(task);
out_no_task:
	return res;
}

static int proc_pident_readdir(struct file *file, struct dir_context *ctx,
		const struct pid_entry *ents, unsigned int nents)
{
	struct task_struct *task = get_proc_task(file_inode(file));
	const struct pid_entry *p;

	if (!task)
		return -ENOENT;

	if (!dir_emit_dots(file, ctx))
		goto out;

	if (ctx->pos >= nents + 2)
		goto out;

	for (p = ents + (ctx->pos - 2); p < ents + nents; p++) {
		if (!proc_fill_cache(file, ctx, p->name, p->len,
				proc_pident_instantiate, task, p))
			break;
		ctx->pos++;
	}
out:
	put_task_struct(task);
	return 0;
}

#ifdef CONFIG_SECURITY
static int proc_pid_attr_open(struct inode *inode, struct file *file)
{
	file->private_data = NULL;
	__mem_open(inode, file, PTRACE_MODE_READ_FSCREDS);
	return 0;
}

static ssize_t proc_pid_attr_read(struct file * file, char __user * buf,
				  size_t count, loff_t *ppos)
{
	struct inode * inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *p = NULL;
	ssize_t length;
	struct task_struct *task = get_proc_task(inode);

	if (!task)
		return -ESRCH;

<<<<<<< HEAD
	length = security_getprocattr(task,
				      (char*)file->f_path.dentry->d_name.name,
=======
	length = security_getprocattr(task, PROC_I(inode)->op.lsmid,
				      file->f_path.dentry->d_name.name,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				      &p);
	put_task_struct(task);
	if (length > 0)
		length = simple_read_from_buffer(buf, count, ppos, p, length);
	kfree(p);
	return length;
}

static ssize_t proc_pid_attr_write(struct file * file, const char __user * buf,
				   size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct inode * inode = file->f_path.dentry->d_inode;
	char *page;
	ssize_t length;
	struct task_struct *task = get_proc_task(inode);

	length = -ESRCH;
	if (!task)
		goto out_no_task;
=======
	struct inode * inode = file_inode(file);
	struct task_struct *task;
	void *page;
	int rv;

	/* A task may only write when it was the opener. */
	if (file->private_data != current->mm)
		return -EPERM;

	rcu_read_lock();
	task = pid_task(proc_pid(inode), PIDTYPE_PID);
	if (!task) {
		rcu_read_unlock();
		return -ESRCH;
	}
	/* A task may only write its own attributes. */
	if (current != task) {
		rcu_read_unlock();
		return -EACCES;
	}
	/* Prevent changes to overridden credentials. */
	if (current_cred() != current_real_cred()) {
		rcu_read_unlock();
		return -EBUSY;
	}
	rcu_read_unlock();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count > PAGE_SIZE)
		count = PAGE_SIZE;

	/* No partial writes. */
<<<<<<< HEAD
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	page = (char*)__get_free_page(GFP_TEMPORARY);
	if (!page)
		goto out;

	length = -EFAULT;
	if (copy_from_user(page, buf, count))
		goto out_free;

	/* Guard against adverse ptrace interaction */
	length = mutex_lock_interruptible(&task->signal->cred_guard_mutex);
	if (length < 0)
		goto out_free;

	length = security_setprocattr(task,
				      (char*)file->f_path.dentry->d_name.name,
				      (void*)page, count);
	mutex_unlock(&task->signal->cred_guard_mutex);
out_free:
	free_page((unsigned long) page);
out:
	put_task_struct(task);
out_no_task:
	return length;
}

static const struct file_operations proc_pid_attr_operations = {
	.read		= proc_pid_attr_read,
	.write		= proc_pid_attr_write,
	.llseek		= generic_file_llseek,
};

static const struct pid_entry attr_dir_stuff[] = {
	REG("current",    S_IRUGO|S_IWUGO, proc_pid_attr_operations),
	REG("prev",       S_IRUGO,	   proc_pid_attr_operations),
	REG("exec",       S_IRUGO|S_IWUGO, proc_pid_attr_operations),
	REG("fscreate",   S_IRUGO|S_IWUGO, proc_pid_attr_operations),
	REG("keycreate",  S_IRUGO|S_IWUGO, proc_pid_attr_operations),
	REG("sockcreate", S_IRUGO|S_IWUGO, proc_pid_attr_operations),
};

static int proc_attr_dir_readdir(struct file * filp,
			     void * dirent, filldir_t filldir)
{
	return proc_pident_readdir(filp,dirent,filldir,
				   attr_dir_stuff,ARRAY_SIZE(attr_dir_stuff));
=======
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user(buf, count);
	if (IS_ERR(page)) {
		rv = PTR_ERR(page);
		goto out;
	}

	/* Guard against adverse ptrace interaction */
	rv = mutex_lock_interruptible(&current->signal->cred_guard_mutex);
	if (rv < 0)
		goto out_free;

	rv = security_setprocattr(PROC_I(inode)->op.lsmid,
				  file->f_path.dentry->d_name.name, page,
				  count);
	mutex_unlock(&current->signal->cred_guard_mutex);
out_free:
	kfree(page);
out:
	return rv;
}

static const struct file_operations proc_pid_attr_operations = {
	.open		= proc_pid_attr_open,
	.read		= proc_pid_attr_read,
	.write		= proc_pid_attr_write,
	.llseek		= generic_file_llseek,
	.release	= mem_release,
};

#define LSM_DIR_OPS(LSM) \
static int proc_##LSM##_attr_dir_iterate(struct file *filp, \
			     struct dir_context *ctx) \
{ \
	return proc_pident_readdir(filp, ctx, \
				   LSM##_attr_dir_stuff, \
				   ARRAY_SIZE(LSM##_attr_dir_stuff)); \
} \
\
static const struct file_operations proc_##LSM##_attr_dir_ops = { \
	.read		= generic_read_dir, \
	.iterate_shared	= proc_##LSM##_attr_dir_iterate, \
	.llseek		= default_llseek, \
}; \
\
static struct dentry *proc_##LSM##_attr_dir_lookup(struct inode *dir, \
				struct dentry *dentry, unsigned int flags) \
{ \
	return proc_pident_lookup(dir, dentry, \
				  LSM##_attr_dir_stuff, \
				  LSM##_attr_dir_stuff + ARRAY_SIZE(LSM##_attr_dir_stuff)); \
} \
\
static const struct inode_operations proc_##LSM##_attr_dir_inode_ops = { \
	.lookup		= proc_##LSM##_attr_dir_lookup, \
	.getattr	= pid_getattr, \
	.setattr	= proc_setattr, \
}

#ifdef CONFIG_SECURITY_SMACK
static const struct pid_entry smack_attr_dir_stuff[] = {
	ATTR(LSM_ID_SMACK, "current",	0666),
};
LSM_DIR_OPS(smack);
#endif

#ifdef CONFIG_SECURITY_APPARMOR
static const struct pid_entry apparmor_attr_dir_stuff[] = {
	ATTR(LSM_ID_APPARMOR, "current",	0666),
	ATTR(LSM_ID_APPARMOR, "prev",		0444),
	ATTR(LSM_ID_APPARMOR, "exec",		0666),
};
LSM_DIR_OPS(apparmor);
#endif

static const struct pid_entry attr_dir_stuff[] = {
	ATTR(LSM_ID_UNDEF, "current",	0666),
	ATTR(LSM_ID_UNDEF, "prev",		0444),
	ATTR(LSM_ID_UNDEF, "exec",		0666),
	ATTR(LSM_ID_UNDEF, "fscreate",	0666),
	ATTR(LSM_ID_UNDEF, "keycreate",	0666),
	ATTR(LSM_ID_UNDEF, "sockcreate",	0666),
#ifdef CONFIG_SECURITY_SMACK
	DIR("smack",			0555,
	    proc_smack_attr_dir_inode_ops, proc_smack_attr_dir_ops),
#endif
#ifdef CONFIG_SECURITY_APPARMOR
	DIR("apparmor",			0555,
	    proc_apparmor_attr_dir_inode_ops, proc_apparmor_attr_dir_ops),
#endif
};

static int proc_attr_dir_readdir(struct file *file, struct dir_context *ctx)
{
	return proc_pident_readdir(file, ctx, 
				   attr_dir_stuff, ARRAY_SIZE(attr_dir_stuff));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations proc_attr_dir_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= proc_attr_dir_readdir,
	.llseek		= default_llseek,
=======
	.iterate_shared	= proc_attr_dir_readdir,
	.llseek		= generic_file_llseek,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct dentry *proc_attr_dir_lookup(struct inode *dir,
				struct dentry *dentry, unsigned int flags)
{
	return proc_pident_lookup(dir, dentry,
<<<<<<< HEAD
				  attr_dir_stuff, ARRAY_SIZE(attr_dir_stuff));
=======
				  attr_dir_stuff,
				  attr_dir_stuff + ARRAY_SIZE(attr_dir_stuff));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct inode_operations proc_attr_dir_inode_operations = {
	.lookup		= proc_attr_dir_lookup,
	.getattr	= pid_getattr,
	.setattr	= proc_setattr,
};

#endif

#ifdef CONFIG_ELF_CORE
static ssize_t proc_coredump_filter_read(struct file *file, char __user *buf,
					 size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct task_struct *task = get_proc_task(file->f_dentry->d_inode);
=======
	struct task_struct *task = get_proc_task(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mm_struct *mm;
	char buffer[PROC_NUMBUF];
	size_t len;
	int ret;

	if (!task)
		return -ESRCH;

	ret = 0;
	mm = get_task_mm(task);
	if (mm) {
		len = snprintf(buffer, sizeof(buffer), "%08lx\n",
			       ((mm->flags & MMF_DUMP_FILTER_MASK) >>
				MMF_DUMP_FILTER_SHIFT));
		mmput(mm);
		ret = simple_read_from_buffer(buf, count, ppos, buffer, len);
	}

	put_task_struct(task);

	return ret;
}

static ssize_t proc_coredump_filter_write(struct file *file,
					  const char __user *buf,
					  size_t count,
					  loff_t *ppos)
{
	struct task_struct *task;
	struct mm_struct *mm;
<<<<<<< HEAD
	char buffer[PROC_NUMBUF], *end;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int val;
	int ret;
	int i;
	unsigned long mask;

<<<<<<< HEAD
	ret = -EFAULT;
	memset(buffer, 0, sizeof(buffer));
	if (count > sizeof(buffer) - 1)
		count = sizeof(buffer) - 1;
	if (copy_from_user(buffer, buf, count))
		goto out_no_task;

	ret = -EINVAL;
	val = (unsigned int)simple_strtoul(buffer, &end, 0);
	if (*end == '\n')
		end++;
	if (end - buffer == 0)
		goto out_no_task;

	ret = -ESRCH;
	task = get_proc_task(file->f_dentry->d_inode);
	if (!task)
		goto out_no_task;

	ret = end - buffer;
	mm = get_task_mm(task);
	if (!mm)
		goto out_no_mm;
=======
	ret = kstrtouint_from_user(buf, count, 0, &val);
	if (ret < 0)
		return ret;

	ret = -ESRCH;
	task = get_proc_task(file_inode(file));
	if (!task)
		goto out_no_task;

	mm = get_task_mm(task);
	if (!mm)
		goto out_no_mm;
	ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0, mask = 1; i < MMF_DUMP_FILTER_BITS; i++, mask <<= 1) {
		if (val & mask)
			set_bit(i + MMF_DUMP_FILTER_SHIFT, &mm->flags);
		else
			clear_bit(i + MMF_DUMP_FILTER_SHIFT, &mm->flags);
	}

	mmput(mm);
 out_no_mm:
	put_task_struct(task);
 out_no_task:
<<<<<<< HEAD
	return ret;
=======
	if (ret < 0)
		return ret;
	return count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations proc_coredump_filter_operations = {
	.read		= proc_coredump_filter_read,
	.write		= proc_coredump_filter_write,
	.llseek		= generic_file_llseek,
};
#endif

<<<<<<< HEAD
/*
 * /proc/self:
 */
static int proc_self_readlink(struct dentry *dentry, char __user *buffer,
			      int buflen)
{
	struct pid_namespace *ns = dentry->d_sb->s_fs_info;
	pid_t tgid = task_tgid_nr_ns(current, ns);
	char tmp[PROC_NUMBUF];
	if (!tgid)
		return -ENOENT;
	sprintf(tmp, "%d", tgid);
	return vfs_readlink(dentry,buffer,buflen,tmp);
}

static void *proc_self_follow_link(struct dentry *dentry, struct nameidata *nd)
{
	struct pid_namespace *ns = dentry->d_sb->s_fs_info;
	pid_t tgid = task_tgid_nr_ns(current, ns);
	char *name = ERR_PTR(-ENOENT);
	if (tgid) {
		name = __getname();
		if (!name)
			name = ERR_PTR(-ENOMEM);
		else
			sprintf(name, "%d", tgid);
	}
	nd_set_link(nd, name);
	return NULL;
}

static void proc_self_put_link(struct dentry *dentry, struct nameidata *nd,
				void *cookie)
{
	char *s = nd_get_link(nd);
	if (!IS_ERR(s))
		__putname(s);
}

static const struct inode_operations proc_self_inode_operations = {
	.readlink	= proc_self_readlink,
	.follow_link	= proc_self_follow_link,
	.put_link	= proc_self_put_link,
};

/*
 * proc base
 *
 * These are the directory entries in the root directory of /proc
 * that properly belong to the /proc filesystem, as they describe
 * describe something that is process related.
 */
static const struct pid_entry proc_base_stuff[] = {
	NOD("self", S_IFLNK|S_IRWXUGO,
		&proc_self_inode_operations, NULL, {}),
};

static struct dentry *proc_base_instantiate(struct inode *dir,
	struct dentry *dentry, struct task_struct *task, const void *ptr)
{
	const struct pid_entry *p = ptr;
	struct inode *inode;
	struct proc_inode *ei;
	struct dentry *error;

	/* Allocate the inode */
	error = ERR_PTR(-ENOMEM);
	inode = new_inode(dir->i_sb);
	if (!inode)
		goto out;

	/* Initialize the inode */
	ei = PROC_I(inode);
	inode->i_ino = get_next_ino();
	inode->i_mtime = inode->i_atime = inode->i_ctime = CURRENT_TIME;

	/*
	 * grab the reference to the task.
	 */
	ei->pid = get_task_pid(task, PIDTYPE_PID);
	if (!ei->pid)
		goto out_iput;

	inode->i_mode = p->mode;
	if (S_ISDIR(inode->i_mode))
		set_nlink(inode, 2);
	if (S_ISLNK(inode->i_mode))
		inode->i_size = 64;
	if (p->iop)
		inode->i_op = p->iop;
	if (p->fop)
		inode->i_fop = p->fop;
	ei->op = p->op;
	d_add(dentry, inode);
	error = NULL;
out:
	return error;
out_iput:
	iput(inode);
	goto out;
}

static struct dentry *proc_base_lookup(struct inode *dir, struct dentry *dentry)
{
	struct dentry *error;
	struct task_struct *task = get_proc_task(dir);
	const struct pid_entry *p, *last;

	error = ERR_PTR(-ENOENT);

	if (!task)
		goto out_no_task;

	/* Lookup the directory entry */
	last = &proc_base_stuff[ARRAY_SIZE(proc_base_stuff) - 1];
	for (p = proc_base_stuff; p <= last; p++) {
		if (p->len != dentry->d_name.len)
			continue;
		if (!memcmp(dentry->d_name.name, p->name, p->len))
			break;
	}
	if (p > last)
		goto out;

	error = proc_base_instantiate(dir, dentry, task, p);

out:
	put_task_struct(task);
out_no_task:
	return error;
}

static int proc_base_fill_cache(struct file *filp, void *dirent,
	filldir_t filldir, struct task_struct *task, const struct pid_entry *p)
{
	return proc_fill_cache(filp, dirent, filldir, p->name, p->len,
				proc_base_instantiate, task, p);
}

#ifdef CONFIG_TASK_IO_ACCOUNTING
static int do_io_accounting(struct task_struct *task, char *buffer, int whole)
{
	struct task_io_accounting acct = task->ioac;
	unsigned long flags;
	int result;

	result = mutex_lock_killable(&task->signal->cred_guard_mutex);
	if (result)
		return result;

	if (!ptrace_may_access(task, PTRACE_MODE_READ)) {
=======
#ifdef CONFIG_TASK_IO_ACCOUNTING
static int do_io_accounting(struct task_struct *task, struct seq_file *m, int whole)
{
	struct task_io_accounting acct;
	int result;

	result = down_read_killable(&task->signal->exec_update_lock);
	if (result)
		return result;

	if (!ptrace_may_access(task, PTRACE_MODE_READ_FSCREDS)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = -EACCES;
		goto out_unlock;
	}

<<<<<<< HEAD
	if (whole && lock_task_sighand(task, &flags)) {
		struct task_struct *t = task;

		task_io_accounting_add(&acct, &task->signal->ioac);
		while_each_thread(task, t)
			task_io_accounting_add(&acct, &t->ioac);

		unlock_task_sighand(task, &flags);
	}
	result = sprintf(buffer,
			"rchar: %llu\n"
			"wchar: %llu\n"
			"syscr: %llu\n"
			"syscw: %llu\n"
			"read_bytes: %llu\n"
			"write_bytes: %llu\n"
			"cancelled_write_bytes: %llu\n",
			(unsigned long long)acct.rchar,
			(unsigned long long)acct.wchar,
			(unsigned long long)acct.syscr,
			(unsigned long long)acct.syscw,
			(unsigned long long)acct.read_bytes,
			(unsigned long long)acct.write_bytes,
			(unsigned long long)acct.cancelled_write_bytes);
out_unlock:
	mutex_unlock(&task->signal->cred_guard_mutex);
	return result;
}

static int proc_tid_io_accounting(struct task_struct *task, char *buffer)
{
	return do_io_accounting(task, buffer, 0);
}

static int proc_tgid_io_accounting(struct task_struct *task, char *buffer)
{
	return do_io_accounting(task, buffer, 1);
}
#endif /* CONFIG_TASK_IO_ACCOUNTING */

=======
	if (whole) {
		struct signal_struct *sig = task->signal;
		struct task_struct *t;
		unsigned int seq = 1;
		unsigned long flags;

		rcu_read_lock();
		do {
			seq++; /* 2 on the 1st/lockless path, otherwise odd */
			flags = read_seqbegin_or_lock_irqsave(&sig->stats_lock, &seq);

			acct = sig->ioac;
			__for_each_thread(sig, t)
				task_io_accounting_add(&acct, &t->ioac);

		} while (need_seqretry(&sig->stats_lock, seq));
		done_seqretry_irqrestore(&sig->stats_lock, seq, flags);
		rcu_read_unlock();
	} else {
		acct = task->ioac;
	}

	seq_printf(m,
		   "rchar: %llu\n"
		   "wchar: %llu\n"
		   "syscr: %llu\n"
		   "syscw: %llu\n"
		   "read_bytes: %llu\n"
		   "write_bytes: %llu\n"
		   "cancelled_write_bytes: %llu\n",
		   (unsigned long long)acct.rchar,
		   (unsigned long long)acct.wchar,
		   (unsigned long long)acct.syscr,
		   (unsigned long long)acct.syscw,
		   (unsigned long long)acct.read_bytes,
		   (unsigned long long)acct.write_bytes,
		   (unsigned long long)acct.cancelled_write_bytes);
	result = 0;

out_unlock:
	up_read(&task->signal->exec_update_lock);
	return result;
}

static int proc_tid_io_accounting(struct seq_file *m, struct pid_namespace *ns,
				  struct pid *pid, struct task_struct *task)
{
	return do_io_accounting(task, m, 0);
}

static int proc_tgid_io_accounting(struct seq_file *m, struct pid_namespace *ns,
				   struct pid *pid, struct task_struct *task)
{
	return do_io_accounting(task, m, 1);
}
#endif /* CONFIG_TASK_IO_ACCOUNTING */

#ifdef CONFIG_USER_NS
static int proc_id_map_open(struct inode *inode, struct file *file,
	const struct seq_operations *seq_ops)
{
	struct user_namespace *ns = NULL;
	struct task_struct *task;
	struct seq_file *seq;
	int ret = -EINVAL;

	task = get_proc_task(inode);
	if (task) {
		rcu_read_lock();
		ns = get_user_ns(task_cred_xxx(task, user_ns));
		rcu_read_unlock();
		put_task_struct(task);
	}
	if (!ns)
		goto err;

	ret = seq_open(file, seq_ops);
	if (ret)
		goto err_put_ns;

	seq = file->private_data;
	seq->private = ns;

	return 0;
err_put_ns:
	put_user_ns(ns);
err:
	return ret;
}

static int proc_id_map_release(struct inode *inode, struct file *file)
{
	struct seq_file *seq = file->private_data;
	struct user_namespace *ns = seq->private;
	put_user_ns(ns);
	return seq_release(inode, file);
}

static int proc_uid_map_open(struct inode *inode, struct file *file)
{
	return proc_id_map_open(inode, file, &proc_uid_seq_operations);
}

static int proc_gid_map_open(struct inode *inode, struct file *file)
{
	return proc_id_map_open(inode, file, &proc_gid_seq_operations);
}

static int proc_projid_map_open(struct inode *inode, struct file *file)
{
	return proc_id_map_open(inode, file, &proc_projid_seq_operations);
}

static const struct file_operations proc_uid_map_operations = {
	.open		= proc_uid_map_open,
	.write		= proc_uid_map_write,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= proc_id_map_release,
};

static const struct file_operations proc_gid_map_operations = {
	.open		= proc_gid_map_open,
	.write		= proc_gid_map_write,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= proc_id_map_release,
};

static const struct file_operations proc_projid_map_operations = {
	.open		= proc_projid_map_open,
	.write		= proc_projid_map_write,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= proc_id_map_release,
};

static int proc_setgroups_open(struct inode *inode, struct file *file)
{
	struct user_namespace *ns = NULL;
	struct task_struct *task;
	int ret;

	ret = -ESRCH;
	task = get_proc_task(inode);
	if (task) {
		rcu_read_lock();
		ns = get_user_ns(task_cred_xxx(task, user_ns));
		rcu_read_unlock();
		put_task_struct(task);
	}
	if (!ns)
		goto err;

	if (file->f_mode & FMODE_WRITE) {
		ret = -EACCES;
		if (!ns_capable(ns, CAP_SYS_ADMIN))
			goto err_put_ns;
	}

	ret = single_open(file, &proc_setgroups_show, ns);
	if (ret)
		goto err_put_ns;

	return 0;
err_put_ns:
	put_user_ns(ns);
err:
	return ret;
}

static int proc_setgroups_release(struct inode *inode, struct file *file)
{
	struct seq_file *seq = file->private_data;
	struct user_namespace *ns = seq->private;
	int ret = single_release(inode, file);
	put_user_ns(ns);
	return ret;
}

static const struct file_operations proc_setgroups_operations = {
	.open		= proc_setgroups_open,
	.write		= proc_setgroups_write,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= proc_setgroups_release,
};
#endif /* CONFIG_USER_NS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int proc_pid_personality(struct seq_file *m, struct pid_namespace *ns,
				struct pid *pid, struct task_struct *task)
{
	int err = lock_trace(task);
	if (!err) {
		seq_printf(m, "%08x\n", task->personality);
		unlock_trace(task);
	}
	return err;
}

<<<<<<< HEAD
=======
#ifdef CONFIG_LIVEPATCH
static int proc_pid_patch_state(struct seq_file *m, struct pid_namespace *ns,
				struct pid *pid, struct task_struct *task)
{
	seq_printf(m, "%d\n", task->patch_state);
	return 0;
}
#endif /* CONFIG_LIVEPATCH */

#ifdef CONFIG_KSM
static int proc_pid_ksm_merging_pages(struct seq_file *m, struct pid_namespace *ns,
				struct pid *pid, struct task_struct *task)
{
	struct mm_struct *mm;

	mm = get_task_mm(task);
	if (mm) {
		seq_printf(m, "%lu\n", mm->ksm_merging_pages);
		mmput(mm);
	}

	return 0;
}
static int proc_pid_ksm_stat(struct seq_file *m, struct pid_namespace *ns,
				struct pid *pid, struct task_struct *task)
{
	struct mm_struct *mm;

	mm = get_task_mm(task);
	if (mm) {
		seq_printf(m, "ksm_rmap_items %lu\n", mm->ksm_rmap_items);
		seq_printf(m, "ksm_zero_pages %lu\n", mm->ksm_zero_pages);
		seq_printf(m, "ksm_merging_pages %lu\n", mm->ksm_merging_pages);
		seq_printf(m, "ksm_process_profit %ld\n", ksm_process_profit(mm));
		mmput(mm);
	}

	return 0;
}
#endif /* CONFIG_KSM */

#ifdef CONFIG_STACKLEAK_METRICS
static int proc_stack_depth(struct seq_file *m, struct pid_namespace *ns,
				struct pid *pid, struct task_struct *task)
{
	unsigned long prev_depth = THREAD_SIZE -
				(task->prev_lowest_stack & (THREAD_SIZE - 1));
	unsigned long depth = THREAD_SIZE -
				(task->lowest_stack & (THREAD_SIZE - 1));

	seq_printf(m, "previous stack depth: %lu\nstack depth: %lu\n",
							prev_depth, depth);
	return 0;
}
#endif /* CONFIG_STACKLEAK_METRICS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Thread groups
 */
static const struct file_operations proc_task_operations;
static const struct inode_operations proc_task_inode_operations;

static const struct pid_entry tgid_base_stuff[] = {
	DIR("task",       S_IRUGO|S_IXUGO, proc_task_inode_operations, proc_task_operations),
	DIR("fd",         S_IRUSR|S_IXUSR, proc_fd_inode_operations, proc_fd_operations),
<<<<<<< HEAD
#ifdef CONFIG_CHECKPOINT_RESTORE
	DIR("map_files",  S_IRUSR|S_IXUSR, proc_map_files_inode_operations, proc_map_files_operations),
#endif
	DIR("fdinfo",     S_IRUSR|S_IXUSR, proc_fdinfo_inode_operations, proc_fdinfo_operations),
=======
	DIR("map_files",  S_IRUSR|S_IXUSR, proc_map_files_inode_operations, proc_map_files_operations),
	DIR("fdinfo",     S_IRUGO|S_IXUGO, proc_fdinfo_inode_operations, proc_fdinfo_operations),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	DIR("ns",	  S_IRUSR|S_IXUGO, proc_ns_dir_inode_operations, proc_ns_dir_operations),
#ifdef CONFIG_NET
	DIR("net",        S_IRUGO|S_IXUGO, proc_net_inode_operations, proc_net_operations),
#endif
	REG("environ",    S_IRUSR, proc_environ_operations),
<<<<<<< HEAD
	INF("auxv",       S_IRUSR, proc_pid_auxv),
	ONE("status",     S_IRUGO, proc_pid_status),
	ONE("personality", S_IRUGO, proc_pid_personality),
	INF("limits",	  S_IRUGO, proc_pid_limits),
=======
	REG("auxv",       S_IRUSR, proc_auxv_operations),
	ONE("status",     S_IRUGO, proc_pid_status),
	ONE("personality", S_IRUSR, proc_pid_personality),
	ONE("limits",	  S_IRUGO, proc_pid_limits),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SCHED_DEBUG
	REG("sched",      S_IRUGO|S_IWUSR, proc_pid_sched_operations),
#endif
#ifdef CONFIG_SCHED_AUTOGROUP
	REG("autogroup",  S_IRUGO|S_IWUSR, proc_pid_sched_autogroup_operations),
#endif
<<<<<<< HEAD
	REG("comm",      S_IRUGO|S_IWUSR, proc_pid_set_comm_operations),
#ifdef CONFIG_HAVE_ARCH_TRACEHOOK
	INF("syscall",    S_IRUGO, proc_pid_syscall),
#endif
	INF("cmdline",    S_IRUGO, proc_pid_cmdline),
=======
#ifdef CONFIG_TIME_NS
	REG("timens_offsets",  S_IRUGO|S_IWUSR, proc_timens_offsets_operations),
#endif
	REG("comm",      S_IRUGO|S_IWUSR, proc_pid_set_comm_operations),
#ifdef CONFIG_HAVE_ARCH_TRACEHOOK
	ONE("syscall",    S_IRUSR, proc_pid_syscall),
#endif
	REG("cmdline",    S_IRUGO, proc_pid_cmdline_ops),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ONE("stat",       S_IRUGO, proc_tgid_stat),
	ONE("statm",      S_IRUGO, proc_pid_statm),
	REG("maps",       S_IRUGO, proc_pid_maps_operations),
#ifdef CONFIG_NUMA
	REG("numa_maps",  S_IRUGO, proc_pid_numa_maps_operations),
#endif
	REG("mem",        S_IRUSR|S_IWUSR, proc_mem_operations),
	LNK("cwd",        proc_cwd_link),
	LNK("root",       proc_root_link),
	LNK("exe",        proc_exe_link),
	REG("mounts",     S_IRUGO, proc_mounts_operations),
	REG("mountinfo",  S_IRUGO, proc_mountinfo_operations),
	REG("mountstats", S_IRUSR, proc_mountstats_operations),
#ifdef CONFIG_PROC_PAGE_MONITOR
	REG("clear_refs", S_IWUSR, proc_clear_refs_operations),
	REG("smaps",      S_IRUGO, proc_pid_smaps_operations),
<<<<<<< HEAD
	REG("pagemap",    S_IRUGO, proc_pagemap_operations),
=======
	REG("smaps_rollup", S_IRUGO, proc_pid_smaps_rollup_operations),
	REG("pagemap",    S_IRUSR, proc_pagemap_operations),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifdef CONFIG_SECURITY
	DIR("attr",       S_IRUGO|S_IXUGO, proc_attr_dir_inode_operations, proc_attr_dir_operations),
#endif
#ifdef CONFIG_KALLSYMS
<<<<<<< HEAD
	INF("wchan",      S_IRUGO, proc_pid_wchan),
#endif
#ifdef CONFIG_STACKTRACE
	ONE("stack",      S_IRUGO, proc_pid_stack),
#endif
#ifdef CONFIG_SCHEDSTATS
	INF("schedstat",  S_IRUGO, proc_pid_schedstat),
=======
	ONE("wchan",      S_IRUGO, proc_pid_wchan),
#endif
#ifdef CONFIG_STACKTRACE
	ONE("stack",      S_IRUSR, proc_pid_stack),
#endif
#ifdef CONFIG_SCHED_INFO
	ONE("schedstat",  S_IRUGO, proc_pid_schedstat),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifdef CONFIG_LATENCYTOP
	REG("latency",  S_IRUGO, proc_lstats_operations),
#endif
#ifdef CONFIG_PROC_PID_CPUSET
<<<<<<< HEAD
	REG("cpuset",     S_IRUGO, proc_cpuset_operations),
#endif
#ifdef CONFIG_CGROUPS
	REG("cgroup",  S_IRUGO, proc_cgroup_operations),
#endif
	INF("oom_score",  S_IRUGO, proc_oom_score),
	ANDROID("oom_adj",S_IRUGO|S_IWUSR, oom_adjust),
	REG("oom_score_adj", S_IRUGO|S_IWUSR, proc_oom_score_adj_operations),
#ifdef CONFIG_AUDITSYSCALL
=======
	ONE("cpuset",     S_IRUGO, proc_cpuset_show),
#endif
#ifdef CONFIG_CGROUPS
	ONE("cgroup",  S_IRUGO, proc_cgroup_show),
#endif
#ifdef CONFIG_PROC_CPU_RESCTRL
	ONE("cpu_resctrl_groups", S_IRUGO, proc_resctrl_show),
#endif
	ONE("oom_score",  S_IRUGO, proc_oom_score),
	REG("oom_adj",    S_IRUGO|S_IWUSR, proc_oom_adj_operations),
	REG("oom_score_adj", S_IRUGO|S_IWUSR, proc_oom_score_adj_operations),
#ifdef CONFIG_AUDIT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	REG("loginuid",   S_IWUSR|S_IRUGO, proc_loginuid_operations),
	REG("sessionid",  S_IRUGO, proc_sessionid_operations),
#endif
#ifdef CONFIG_FAULT_INJECTION
	REG("make-it-fail", S_IRUGO|S_IWUSR, proc_fault_inject_operations),
<<<<<<< HEAD
=======
	REG("fail-nth", 0644, proc_fail_nth_operations),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifdef CONFIG_ELF_CORE
	REG("coredump_filter", S_IRUGO|S_IWUSR, proc_coredump_filter_operations),
#endif
#ifdef CONFIG_TASK_IO_ACCOUNTING
<<<<<<< HEAD
	INF("io",	S_IRUSR, proc_tgid_io_accounting),
#endif
#ifdef CONFIG_HARDWALL
	INF("hardwall",   S_IRUGO, proc_pid_hardwall),
#endif
};

static int proc_tgid_base_readdir(struct file * filp,
			     void * dirent, filldir_t filldir)
{
	return proc_pident_readdir(filp,dirent,filldir,
				   tgid_base_stuff,ARRAY_SIZE(tgid_base_stuff));
=======
	ONE("io",	S_IRUSR, proc_tgid_io_accounting),
#endif
#ifdef CONFIG_USER_NS
	REG("uid_map",    S_IRUGO|S_IWUSR, proc_uid_map_operations),
	REG("gid_map",    S_IRUGO|S_IWUSR, proc_gid_map_operations),
	REG("projid_map", S_IRUGO|S_IWUSR, proc_projid_map_operations),
	REG("setgroups",  S_IRUGO|S_IWUSR, proc_setgroups_operations),
#endif
#if defined(CONFIG_CHECKPOINT_RESTORE) && defined(CONFIG_POSIX_TIMERS)
	REG("timers",	  S_IRUGO, proc_timers_operations),
#endif
	REG("timerslack_ns", S_IRUGO|S_IWUGO, proc_pid_set_timerslack_ns_operations),
#ifdef CONFIG_LIVEPATCH
	ONE("patch_state",  S_IRUSR, proc_pid_patch_state),
#endif
#ifdef CONFIG_STACKLEAK_METRICS
	ONE("stack_depth", S_IRUGO, proc_stack_depth),
#endif
#ifdef CONFIG_PROC_PID_ARCH_STATUS
	ONE("arch_status", S_IRUGO, proc_pid_arch_status),
#endif
#ifdef CONFIG_SECCOMP_CACHE_DEBUG
	ONE("seccomp_cache", S_IRUSR, proc_pid_seccomp_cache),
#endif
#ifdef CONFIG_KSM
	ONE("ksm_merging_pages",  S_IRUSR, proc_pid_ksm_merging_pages),
	ONE("ksm_stat",  S_IRUSR, proc_pid_ksm_stat),
#endif
};

static int proc_tgid_base_readdir(struct file *file, struct dir_context *ctx)
{
	return proc_pident_readdir(file, ctx,
				   tgid_base_stuff, ARRAY_SIZE(tgid_base_stuff));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations proc_tgid_base_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= proc_tgid_base_readdir,
	.llseek		= default_llseek,
};

static struct dentry *proc_tgid_base_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	return proc_pident_lookup(dir, dentry,
				  tgid_base_stuff, ARRAY_SIZE(tgid_base_stuff));
=======
	.iterate_shared	= proc_tgid_base_readdir,
	.llseek		= generic_file_llseek,
};

struct pid *tgid_pidfd_to_pid(const struct file *file)
{
	if (file->f_op != &proc_tgid_base_operations)
		return ERR_PTR(-EBADF);

	return proc_pid(file_inode(file));
}

static struct dentry *proc_tgid_base_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	return proc_pident_lookup(dir, dentry,
				  tgid_base_stuff,
				  tgid_base_stuff + ARRAY_SIZE(tgid_base_stuff));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct inode_operations proc_tgid_base_inode_operations = {
	.lookup		= proc_tgid_base_lookup,
	.getattr	= pid_getattr,
	.setattr	= proc_setattr,
	.permission	= proc_pid_permission,
};

<<<<<<< HEAD
static void proc_flush_task_mnt(struct vfsmount *mnt, pid_t pid, pid_t tgid)
{
	struct dentry *dentry, *leader, *dir;
	char buf[PROC_NUMBUF];
	struct qstr name;

	name.name = buf;
	name.len = snprintf(buf, sizeof(buf), "%d", pid);
	dentry = d_hash_and_lookup(mnt->mnt_root, &name);
	if (dentry) {
		shrink_dcache_parent(dentry);
		d_drop(dentry);
		dput(dentry);
	}

	name.name = buf;
	name.len = snprintf(buf, sizeof(buf), "%d", tgid);
	leader = d_hash_and_lookup(mnt->mnt_root, &name);
	if (!leader)
		goto out;

	name.name = "task";
	name.len = strlen(name.name);
	dir = d_hash_and_lookup(leader, &name);
	if (!dir)
		goto out_put_leader;

	name.name = buf;
	name.len = snprintf(buf, sizeof(buf), "%d", pid);
	dentry = d_hash_and_lookup(dir, &name);
	if (dentry) {
		shrink_dcache_parent(dentry);
		d_drop(dentry);
		dput(dentry);
	}

	dput(dir);
out_put_leader:
	dput(leader);
out:
	return;
}

/**
 * proc_flush_task -  Remove dcache entries for @task from the /proc dcache.
 * @task: task that should be flushed.
 *
 * When flushing dentries from proc, one needs to flush them from global
 * proc (proc_mnt) and from all the namespaces' procs this task was seen
 * in. This call is supposed to do all of this job.
 *
 * Looks in the dcache for
 * /proc/@pid
 * /proc/@tgid/task/@pid
 * if either directory is present flushes it and all of it'ts children
 * from the dcache.
=======
/**
 * proc_flush_pid -  Remove dcache entries for @pid from the /proc dcache.
 * @pid: pid that should be flushed.
 *
 * This function walks a list of inodes (that belong to any proc
 * filesystem) that are attached to the pid and flushes them from
 * the dentry cache.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * It is safe and reasonable to cache /proc entries for a task until
 * that task exits.  After that they just clog up the dcache with
 * useless entries, possibly causing useful dcache entries to be
<<<<<<< HEAD
 * flushed instead.  This routine is proved to flush those useless
 * dcache entries at process exit time.
 *
 * NOTE: This routine is just an optimization so it does not guarantee
 *       that no dcache entries will exist at process exit time it
 *       just makes it very unlikely that any will persist.
 */

void proc_flush_task(struct task_struct *task)
{
	int i;
	struct pid *pid, *tgid;
	struct upid *upid;

	pid = task_pid(task);
	tgid = task_tgid(task);

	for (i = 0; i <= pid->level; i++) {
		upid = &pid->numbers[i];
		proc_flush_task_mnt(upid->ns->proc_mnt, upid->nr,
					tgid->numbers[i].nr);
	}

	upid = &pid->numbers[pid->level];
	if (upid->nr == 1)
		pid_ns_release_proc(upid->ns);
}

static struct dentry *proc_pid_instantiate(struct inode *dir,
					   struct dentry * dentry,
					   struct task_struct *task, const void *ptr)
{
	struct dentry *error = ERR_PTR(-ENOENT);
	struct inode *inode;

	inode = proc_pid_make_inode(dir->i_sb, task);
	if (!inode)
		goto out;

	inode->i_mode = S_IFDIR|S_IRUGO|S_IXUGO;
=======
 * flushed instead.  This routine is provided to flush those useless
 * dcache entries when a process is reaped.
 *
 * NOTE: This routine is just an optimization so it does not guarantee
 *       that no dcache entries will exist after a process is reaped
 *       it just makes it very unlikely that any will persist.
 */

void proc_flush_pid(struct pid *pid)
{
	proc_invalidate_siblings_dcache(&pid->inodes, &pid->lock);
}

static struct dentry *proc_pid_instantiate(struct dentry * dentry,
				   struct task_struct *task, const void *ptr)
{
	struct inode *inode;

	inode = proc_pid_make_base_inode(dentry->d_sb, task,
					 S_IFDIR | S_IRUGO | S_IXUGO);
	if (!inode)
		return ERR_PTR(-ENOENT);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	inode->i_op = &proc_tgid_base_inode_operations;
	inode->i_fop = &proc_tgid_base_operations;
	inode->i_flags|=S_IMMUTABLE;

<<<<<<< HEAD
	set_nlink(inode, 2 + pid_entry_count_dirs(tgid_base_stuff,
						  ARRAY_SIZE(tgid_base_stuff)));

	d_set_d_op(dentry, &pid_dentry_operations);

	d_add(dentry, inode);
	/* Close the race of the process dying before we return the dentry */
	if (pid_revalidate(dentry, 0))
		error = NULL;
out:
	return error;
}

struct dentry *proc_pid_lookup(struct inode *dir, struct dentry * dentry, unsigned int flags)
{
	struct dentry *result;
	struct task_struct *task;
	unsigned tgid;
	struct pid_namespace *ns;

	result = proc_base_lookup(dir, dentry);
	if (!IS_ERR(result) || PTR_ERR(result) != -ENOENT)
		goto out;

	tgid = name_to_int(dentry);
	if (tgid == ~0U)
		goto out;

	ns = dentry->d_sb->s_fs_info;
=======
	set_nlink(inode, nlink_tgid);
	pid_update_inode(task, inode);

	d_set_d_op(dentry, &pid_dentry_operations);
	return d_splice_alias(inode, dentry);
}

struct dentry *proc_pid_lookup(struct dentry *dentry, unsigned int flags)
{
	struct task_struct *task;
	unsigned tgid;
	struct proc_fs_info *fs_info;
	struct pid_namespace *ns;
	struct dentry *result = ERR_PTR(-ENOENT);

	tgid = name_to_int(&dentry->d_name);
	if (tgid == ~0U)
		goto out;

	fs_info = proc_sb_info(dentry->d_sb);
	ns = fs_info->pid_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_lock();
	task = find_task_by_pid_ns(tgid, ns);
	if (task)
		get_task_struct(task);
	rcu_read_unlock();
	if (!task)
		goto out;

<<<<<<< HEAD
	result = proc_pid_instantiate(dir, dentry, task, NULL);
=======
	/* Limit procfs to only ptraceable tasks */
	if (fs_info->hide_pid == HIDEPID_NOT_PTRACEABLE) {
		if (!has_pid_permissions(fs_info, task, HIDEPID_NO_ACCESS))
			goto out_put_task;
	}

	result = proc_pid_instantiate(dentry, task, NULL);
out_put_task:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	put_task_struct(task);
out:
	return result;
}

/*
 * Find the first task with tgid >= tgid
 *
 */
struct tgid_iter {
	unsigned int tgid;
	struct task_struct *task;
};
static struct tgid_iter next_tgid(struct pid_namespace *ns, struct tgid_iter iter)
{
	struct pid *pid;

	if (iter.task)
		put_task_struct(iter.task);
	rcu_read_lock();
retry:
	iter.task = NULL;
	pid = find_ge_pid(iter.tgid, ns);
	if (pid) {
		iter.tgid = pid_nr_ns(pid, ns);
<<<<<<< HEAD
		iter.task = pid_task(pid, PIDTYPE_PID);
		/* What we to know is if the pid we have find is the
		 * pid of a thread_group_leader.  Testing for task
		 * being a thread_group_leader is the obvious thing
		 * todo but there is a window when it fails, due to
		 * the pid transfer logic in de_thread.
		 *
		 * So we perform the straight forward test of seeing
		 * if the pid we have found is the pid of a thread
		 * group leader, and don't worry if the task we have
		 * found doesn't happen to be a thread group leader.
		 * As we don't care in the case of readdir.
		 */
		if (!iter.task || !has_group_leader_pid(iter.task)) {
=======
		iter.task = pid_task(pid, PIDTYPE_TGID);
		if (!iter.task) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			iter.tgid += 1;
			goto retry;
		}
		get_task_struct(iter.task);
	}
	rcu_read_unlock();
	return iter;
}

<<<<<<< HEAD
#define TGID_OFFSET (FIRST_PROCESS_ENTRY + ARRAY_SIZE(proc_base_stuff))

static int proc_pid_fill_cache(struct file *filp, void *dirent, filldir_t filldir,
	struct tgid_iter iter)
{
	char name[PROC_NUMBUF];
	int len = snprintf(name, sizeof(name), "%d", iter.tgid);
	return proc_fill_cache(filp, dirent, filldir, name, len,
				proc_pid_instantiate, iter.task, NULL);
}

static int fake_filldir(void *buf, const char *name, int namelen,
			loff_t offset, u64 ino, unsigned d_type)
{
	return 0;
}

/* for the /proc/ directory itself, after non-process stuff has been done */
int proc_pid_readdir(struct file * filp, void * dirent, filldir_t filldir)
{
	unsigned int nr;
	struct task_struct *reaper;
	struct tgid_iter iter;
	struct pid_namespace *ns;
	filldir_t __filldir;

	if (filp->f_pos >= PID_MAX_LIMIT + TGID_OFFSET)
		goto out_no_task;
	nr = filp->f_pos - FIRST_PROCESS_ENTRY;

	reaper = get_proc_task(filp->f_path.dentry->d_inode);
	if (!reaper)
		goto out_no_task;

	for (; nr < ARRAY_SIZE(proc_base_stuff); filp->f_pos++, nr++) {
		const struct pid_entry *p = &proc_base_stuff[nr];
		if (proc_base_fill_cache(filp, dirent, filldir, reaper, p) < 0)
			goto out;
	}

	ns = filp->f_dentry->d_sb->s_fs_info;
	iter.task = NULL;
	iter.tgid = filp->f_pos - TGID_OFFSET;
	for (iter = next_tgid(ns, iter);
	     iter.task;
	     iter.tgid += 1, iter = next_tgid(ns, iter)) {
		if (has_pid_permissions(ns, iter.task, 2))
			__filldir = filldir;
		else
			__filldir = fake_filldir;

		filp->f_pos = iter.tgid + TGID_OFFSET;
		if (proc_pid_fill_cache(filp, dirent, __filldir, iter) < 0) {
			put_task_struct(iter.task);
			goto out;
		}
	}
	filp->f_pos = PID_MAX_LIMIT + TGID_OFFSET;
out:
	put_task_struct(reaper);
out_no_task:
=======
#define TGID_OFFSET (FIRST_PROCESS_ENTRY + 2)

/* for the /proc/ directory itself, after non-process stuff has been done */
int proc_pid_readdir(struct file *file, struct dir_context *ctx)
{
	struct tgid_iter iter;
	struct proc_fs_info *fs_info = proc_sb_info(file_inode(file)->i_sb);
	struct pid_namespace *ns = proc_pid_ns(file_inode(file)->i_sb);
	loff_t pos = ctx->pos;

	if (pos >= PID_MAX_LIMIT + TGID_OFFSET)
		return 0;

	if (pos == TGID_OFFSET - 2) {
		struct inode *inode = d_inode(fs_info->proc_self);
		if (!dir_emit(ctx, "self", 4, inode->i_ino, DT_LNK))
			return 0;
		ctx->pos = pos = pos + 1;
	}
	if (pos == TGID_OFFSET - 1) {
		struct inode *inode = d_inode(fs_info->proc_thread_self);
		if (!dir_emit(ctx, "thread-self", 11, inode->i_ino, DT_LNK))
			return 0;
		ctx->pos = pos = pos + 1;
	}
	iter.tgid = pos - TGID_OFFSET;
	iter.task = NULL;
	for (iter = next_tgid(ns, iter);
	     iter.task;
	     iter.tgid += 1, iter = next_tgid(ns, iter)) {
		char name[10 + 1];
		unsigned int len;

		cond_resched();
		if (!has_pid_permissions(fs_info, iter.task, HIDEPID_INVISIBLE))
			continue;

		len = snprintf(name, sizeof(name), "%u", iter.tgid);
		ctx->pos = iter.tgid + TGID_OFFSET;
		if (!proc_fill_cache(file, ctx, name, len,
				     proc_pid_instantiate, iter.task, NULL)) {
			put_task_struct(iter.task);
			return 0;
		}
	}
	ctx->pos = PID_MAX_LIMIT + TGID_OFFSET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
<<<<<<< HEAD
=======
 * proc_tid_comm_permission is a special permission function exclusively
 * used for the node /proc/<pid>/task/<tid>/comm.
 * It bypasses generic permission checks in the case where a task of the same
 * task group attempts to access the node.
 * The rationale behind this is that glibc and bionic access this node for
 * cross thread naming (pthread_set/getname_np(!self)). However, if
 * PR_SET_DUMPABLE gets set to 0 this node among others becomes uid=0 gid=0,
 * which locks out the cross thread naming implementation.
 * This function makes sure that the node is always accessible for members of
 * same thread group.
 */
static int proc_tid_comm_permission(struct mnt_idmap *idmap,
				    struct inode *inode, int mask)
{
	bool is_same_tgroup;
	struct task_struct *task;

	task = get_proc_task(inode);
	if (!task)
		return -ESRCH;
	is_same_tgroup = same_thread_group(current, task);
	put_task_struct(task);

	if (likely(is_same_tgroup && !(mask & MAY_EXEC))) {
		/* This file (/proc/<pid>/task/<tid>/comm) can always be
		 * read or written by the members of the corresponding
		 * thread group.
		 */
		return 0;
	}

	return generic_permission(&nop_mnt_idmap, inode, mask);
}

static const struct inode_operations proc_tid_comm_inode_operations = {
		.setattr	= proc_setattr,
		.permission	= proc_tid_comm_permission,
};

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Tasks
 */
static const struct pid_entry tid_base_stuff[] = {
	DIR("fd",        S_IRUSR|S_IXUSR, proc_fd_inode_operations, proc_fd_operations),
<<<<<<< HEAD
	DIR("fdinfo",    S_IRUSR|S_IXUSR, proc_fdinfo_inode_operations, proc_fdinfo_operations),
	DIR("ns",	 S_IRUSR|S_IXUGO, proc_ns_dir_inode_operations, proc_ns_dir_operations),
	REG("environ",   S_IRUSR, proc_environ_operations),
	INF("auxv",      S_IRUSR, proc_pid_auxv),
	ONE("status",    S_IRUGO, proc_pid_status),
	ONE("personality", S_IRUGO, proc_pid_personality),
	INF("limits",	 S_IRUGO, proc_pid_limits),
#ifdef CONFIG_SCHED_DEBUG
	REG("sched",     S_IRUGO|S_IWUSR, proc_pid_sched_operations),
#endif
	REG("comm",      S_IRUGO|S_IWUSR, proc_pid_set_comm_operations),
#ifdef CONFIG_HAVE_ARCH_TRACEHOOK
	INF("syscall",   S_IRUGO, proc_pid_syscall),
#endif
	INF("cmdline",   S_IRUGO, proc_pid_cmdline),
	ONE("stat",      S_IRUGO, proc_tid_stat),
	ONE("statm",     S_IRUGO, proc_pid_statm),
	REG("maps",      S_IRUGO, proc_tid_maps_operations),
#ifdef CONFIG_NUMA
	REG("numa_maps", S_IRUGO, proc_tid_numa_maps_operations),
=======
	DIR("fdinfo",    S_IRUGO|S_IXUGO, proc_fdinfo_inode_operations, proc_fdinfo_operations),
	DIR("ns",	 S_IRUSR|S_IXUGO, proc_ns_dir_inode_operations, proc_ns_dir_operations),
#ifdef CONFIG_NET
	DIR("net",        S_IRUGO|S_IXUGO, proc_net_inode_operations, proc_net_operations),
#endif
	REG("environ",   S_IRUSR, proc_environ_operations),
	REG("auxv",      S_IRUSR, proc_auxv_operations),
	ONE("status",    S_IRUGO, proc_pid_status),
	ONE("personality", S_IRUSR, proc_pid_personality),
	ONE("limits",	 S_IRUGO, proc_pid_limits),
#ifdef CONFIG_SCHED_DEBUG
	REG("sched",     S_IRUGO|S_IWUSR, proc_pid_sched_operations),
#endif
	NOD("comm",      S_IFREG|S_IRUGO|S_IWUSR,
			 &proc_tid_comm_inode_operations,
			 &proc_pid_set_comm_operations, {}),
#ifdef CONFIG_HAVE_ARCH_TRACEHOOK
	ONE("syscall",   S_IRUSR, proc_pid_syscall),
#endif
	REG("cmdline",   S_IRUGO, proc_pid_cmdline_ops),
	ONE("stat",      S_IRUGO, proc_tid_stat),
	ONE("statm",     S_IRUGO, proc_pid_statm),
	REG("maps",      S_IRUGO, proc_pid_maps_operations),
#ifdef CONFIG_PROC_CHILDREN
	REG("children",  S_IRUGO, proc_tid_children_operations),
#endif
#ifdef CONFIG_NUMA
	REG("numa_maps", S_IRUGO, proc_pid_numa_maps_operations),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	REG("mem",       S_IRUSR|S_IWUSR, proc_mem_operations),
	LNK("cwd",       proc_cwd_link),
	LNK("root",      proc_root_link),
	LNK("exe",       proc_exe_link),
	REG("mounts",    S_IRUGO, proc_mounts_operations),
	REG("mountinfo",  S_IRUGO, proc_mountinfo_operations),
#ifdef CONFIG_PROC_PAGE_MONITOR
	REG("clear_refs", S_IWUSR, proc_clear_refs_operations),
<<<<<<< HEAD
	REG("smaps",     S_IRUGO, proc_tid_smaps_operations),
	REG("pagemap",    S_IRUGO, proc_pagemap_operations),
=======
	REG("smaps",     S_IRUGO, proc_pid_smaps_operations),
	REG("smaps_rollup", S_IRUGO, proc_pid_smaps_rollup_operations),
	REG("pagemap",    S_IRUSR, proc_pagemap_operations),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifdef CONFIG_SECURITY
	DIR("attr",      S_IRUGO|S_IXUGO, proc_attr_dir_inode_operations, proc_attr_dir_operations),
#endif
#ifdef CONFIG_KALLSYMS
<<<<<<< HEAD
	INF("wchan",     S_IRUGO, proc_pid_wchan),
#endif
#ifdef CONFIG_STACKTRACE
	ONE("stack",      S_IRUGO, proc_pid_stack),
#endif
#ifdef CONFIG_SCHEDSTATS
	INF("schedstat", S_IRUGO, proc_pid_schedstat),
=======
	ONE("wchan",     S_IRUGO, proc_pid_wchan),
#endif
#ifdef CONFIG_STACKTRACE
	ONE("stack",      S_IRUSR, proc_pid_stack),
#endif
#ifdef CONFIG_SCHED_INFO
	ONE("schedstat", S_IRUGO, proc_pid_schedstat),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
#ifdef CONFIG_LATENCYTOP
	REG("latency",  S_IRUGO, proc_lstats_operations),
#endif
#ifdef CONFIG_PROC_PID_CPUSET
<<<<<<< HEAD
	REG("cpuset",    S_IRUGO, proc_cpuset_operations),
#endif
#ifdef CONFIG_CGROUPS
	REG("cgroup",  S_IRUGO, proc_cgroup_operations),
#endif
	INF("oom_score", S_IRUGO, proc_oom_score),
	REG("oom_adj",   S_IRUGO|S_IWUSR, proc_oom_adjust_operations),
	REG("oom_score_adj", S_IRUGO|S_IWUSR, proc_oom_score_adj_operations),
#ifdef CONFIG_AUDITSYSCALL
=======
	ONE("cpuset",    S_IRUGO, proc_cpuset_show),
#endif
#ifdef CONFIG_CGROUPS
	ONE("cgroup",  S_IRUGO, proc_cgroup_show),
#endif
#ifdef CONFIG_PROC_CPU_RESCTRL
	ONE("cpu_resctrl_groups", S_IRUGO, proc_resctrl_show),
#endif
	ONE("oom_score", S_IRUGO, proc_oom_score),
	REG("oom_adj",   S_IRUGO|S_IWUSR, proc_oom_adj_operations),
	REG("oom_score_adj", S_IRUGO|S_IWUSR, proc_oom_score_adj_operations),
#ifdef CONFIG_AUDIT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	REG("loginuid",  S_IWUSR|S_IRUGO, proc_loginuid_operations),
	REG("sessionid",  S_IRUGO, proc_sessionid_operations),
#endif
#ifdef CONFIG_FAULT_INJECTION
	REG("make-it-fail", S_IRUGO|S_IWUSR, proc_fault_inject_operations),
<<<<<<< HEAD
#endif
#ifdef CONFIG_TASK_IO_ACCOUNTING
	INF("io",	S_IRUSR, proc_tid_io_accounting),
#endif
#ifdef CONFIG_HARDWALL
	INF("hardwall",   S_IRUGO, proc_pid_hardwall),
#endif
};

static int proc_tid_base_readdir(struct file * filp,
			     void * dirent, filldir_t filldir)
{
	return proc_pident_readdir(filp,dirent,filldir,
				   tid_base_stuff,ARRAY_SIZE(tid_base_stuff));
=======
	REG("fail-nth", 0644, proc_fail_nth_operations),
#endif
#ifdef CONFIG_TASK_IO_ACCOUNTING
	ONE("io",	S_IRUSR, proc_tid_io_accounting),
#endif
#ifdef CONFIG_USER_NS
	REG("uid_map",    S_IRUGO|S_IWUSR, proc_uid_map_operations),
	REG("gid_map",    S_IRUGO|S_IWUSR, proc_gid_map_operations),
	REG("projid_map", S_IRUGO|S_IWUSR, proc_projid_map_operations),
	REG("setgroups",  S_IRUGO|S_IWUSR, proc_setgroups_operations),
#endif
#ifdef CONFIG_LIVEPATCH
	ONE("patch_state",  S_IRUSR, proc_pid_patch_state),
#endif
#ifdef CONFIG_PROC_PID_ARCH_STATUS
	ONE("arch_status", S_IRUGO, proc_pid_arch_status),
#endif
#ifdef CONFIG_SECCOMP_CACHE_DEBUG
	ONE("seccomp_cache", S_IRUSR, proc_pid_seccomp_cache),
#endif
#ifdef CONFIG_KSM
	ONE("ksm_merging_pages",  S_IRUSR, proc_pid_ksm_merging_pages),
	ONE("ksm_stat",  S_IRUSR, proc_pid_ksm_stat),
#endif
};

static int proc_tid_base_readdir(struct file *file, struct dir_context *ctx)
{
	return proc_pident_readdir(file, ctx,
				   tid_base_stuff, ARRAY_SIZE(tid_base_stuff));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dentry *proc_tid_base_lookup(struct inode *dir, struct dentry *dentry, unsigned int flags)
{
	return proc_pident_lookup(dir, dentry,
<<<<<<< HEAD
				  tid_base_stuff, ARRAY_SIZE(tid_base_stuff));
=======
				  tid_base_stuff,
				  tid_base_stuff + ARRAY_SIZE(tid_base_stuff));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations proc_tid_base_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= proc_tid_base_readdir,
	.llseek		= default_llseek,
=======
	.iterate_shared	= proc_tid_base_readdir,
	.llseek		= generic_file_llseek,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct inode_operations proc_tid_base_inode_operations = {
	.lookup		= proc_tid_base_lookup,
	.getattr	= pid_getattr,
	.setattr	= proc_setattr,
};

<<<<<<< HEAD
static struct dentry *proc_task_instantiate(struct inode *dir,
	struct dentry *dentry, struct task_struct *task, const void *ptr)
{
	struct dentry *error = ERR_PTR(-ENOENT);
	struct inode *inode;
	inode = proc_pid_make_inode(dir->i_sb, task);

	if (!inode)
		goto out;
	inode->i_mode = S_IFDIR|S_IRUGO|S_IXUGO;
	inode->i_op = &proc_tid_base_inode_operations;
	inode->i_fop = &proc_tid_base_operations;
	inode->i_flags|=S_IMMUTABLE;

	set_nlink(inode, 2 + pid_entry_count_dirs(tid_base_stuff,
						  ARRAY_SIZE(tid_base_stuff)));

	d_set_d_op(dentry, &pid_dentry_operations);

	d_add(dentry, inode);
	/* Close the race of the process dying before we return the dentry */
	if (pid_revalidate(dentry, 0))
		error = NULL;
out:
	return error;
=======
static struct dentry *proc_task_instantiate(struct dentry *dentry,
	struct task_struct *task, const void *ptr)
{
	struct inode *inode;
	inode = proc_pid_make_base_inode(dentry->d_sb, task,
					 S_IFDIR | S_IRUGO | S_IXUGO);
	if (!inode)
		return ERR_PTR(-ENOENT);

	inode->i_op = &proc_tid_base_inode_operations;
	inode->i_fop = &proc_tid_base_operations;
	inode->i_flags |= S_IMMUTABLE;

	set_nlink(inode, nlink_tid);
	pid_update_inode(task, inode);

	d_set_d_op(dentry, &pid_dentry_operations);
	return d_splice_alias(inode, dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct dentry *proc_task_lookup(struct inode *dir, struct dentry * dentry, unsigned int flags)
{
<<<<<<< HEAD
	struct dentry *result = ERR_PTR(-ENOENT);
	struct task_struct *task;
	struct task_struct *leader = get_proc_task(dir);
	unsigned tid;
	struct pid_namespace *ns;
=======
	struct task_struct *task;
	struct task_struct *leader = get_proc_task(dir);
	unsigned tid;
	struct proc_fs_info *fs_info;
	struct pid_namespace *ns;
	struct dentry *result = ERR_PTR(-ENOENT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!leader)
		goto out_no_task;

<<<<<<< HEAD
	tid = name_to_int(dentry);
	if (tid == ~0U)
		goto out;

	ns = dentry->d_sb->s_fs_info;
=======
	tid = name_to_int(&dentry->d_name);
	if (tid == ~0U)
		goto out;

	fs_info = proc_sb_info(dentry->d_sb);
	ns = fs_info->pid_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_lock();
	task = find_task_by_pid_ns(tid, ns);
	if (task)
		get_task_struct(task);
	rcu_read_unlock();
	if (!task)
		goto out;
	if (!same_thread_group(leader, task))
		goto out_drop_task;

<<<<<<< HEAD
	result = proc_task_instantiate(dir, dentry, task, NULL);
=======
	result = proc_task_instantiate(dentry, task, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_drop_task:
	put_task_struct(task);
out:
	put_task_struct(leader);
out_no_task:
	return result;
}

/*
 * Find the first tid of a thread group to return to user space.
 *
 * Usually this is just the thread group leader, but if the users
 * buffer was too small or there was a seek into the middle of the
 * directory we have more work todo.
 *
 * In the case of a short read we start with find_task_by_pid.
 *
 * In the case of a seek we start with the leader and walk nr
 * threads past it.
 */
<<<<<<< HEAD
static struct task_struct *first_tid(struct task_struct *leader,
		int tid, int nr, struct pid_namespace *ns)
{
	struct task_struct *pos;

	rcu_read_lock();
	/* Attempt to start with the pid of a thread */
	if (tid && (nr > 0)) {
		pos = find_task_by_pid_ns(tid, ns);
		if (pos && (pos->group_leader == leader))
=======
static struct task_struct *first_tid(struct pid *pid, int tid, loff_t f_pos,
					struct pid_namespace *ns)
{
	struct task_struct *pos, *task;
	unsigned long nr = f_pos;

	if (nr != f_pos)	/* 32bit overflow? */
		return NULL;

	rcu_read_lock();
	task = pid_task(pid, PIDTYPE_PID);
	if (!task)
		goto fail;

	/* Attempt to start with the tid of a thread */
	if (tid && nr) {
		pos = find_task_by_pid_ns(tid, ns);
		if (pos && same_thread_group(pos, task))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto found;
	}

	/* If nr exceeds the number of threads there is nothing todo */
<<<<<<< HEAD
	pos = NULL;
	if (nr && nr >= get_nr_threads(leader))
		goto out;
=======
	if (nr >= get_nr_threads(task))
		goto fail;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If we haven't found our starting place yet start
	 * with the leader and walk nr threads forward.
	 */
<<<<<<< HEAD
	for (pos = leader; nr > 0; --nr) {
		pos = next_thread(pos);
		if (pos == leader) {
			pos = NULL;
			goto out;
		}
	}
=======
	for_each_thread(task, pos) {
		if (!nr--)
			goto found;
	}
fail:
	pos = NULL;
	goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
found:
	get_task_struct(pos);
out:
	rcu_read_unlock();
	return pos;
}

/*
 * Find the next thread in the thread list.
 * Return NULL if there is an error or no next thread.
 *
 * The reference to the input task_struct is released.
 */
static struct task_struct *next_tid(struct task_struct *start)
{
	struct task_struct *pos = NULL;
	rcu_read_lock();
	if (pid_alive(start)) {
<<<<<<< HEAD
		pos = next_thread(start);
		if (thread_group_leader(pos))
			pos = NULL;
		else
=======
		pos = __next_thread(start);
		if (pos)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			get_task_struct(pos);
	}
	rcu_read_unlock();
	put_task_struct(start);
	return pos;
}

<<<<<<< HEAD
static int proc_task_fill_cache(struct file *filp, void *dirent, filldir_t filldir,
	struct task_struct *task, int tid)
{
	char name[PROC_NUMBUF];
	int len = snprintf(name, sizeof(name), "%d", tid);
	return proc_fill_cache(filp, dirent, filldir, name, len,
				proc_task_instantiate, task, NULL);
}

/* for the /proc/TGID/task/ directories */
static int proc_task_readdir(struct file * filp, void * dirent, filldir_t filldir)
{
	struct dentry *dentry = filp->f_path.dentry;
	struct inode *inode = dentry->d_inode;
	struct task_struct *leader = NULL;
	struct task_struct *task;
	int retval = -ENOENT;
	ino_t ino;
	int tid;
	struct pid_namespace *ns;

	task = get_proc_task(inode);
	if (!task)
		goto out_no_task;
	rcu_read_lock();
	if (pid_alive(task)) {
		leader = task->group_leader;
		get_task_struct(leader);
	}
	rcu_read_unlock();
	put_task_struct(task);
	if (!leader)
		goto out_no_task;
	retval = 0;

	switch ((unsigned long)filp->f_pos) {
	case 0:
		ino = inode->i_ino;
		if (filldir(dirent, ".", 1, filp->f_pos, ino, DT_DIR) < 0)
			goto out;
		filp->f_pos++;
		/* fall through */
	case 1:
		ino = parent_ino(dentry);
		if (filldir(dirent, "..", 2, filp->f_pos, ino, DT_DIR) < 0)
			goto out;
		filp->f_pos++;
		/* fall through */
	}
=======
/* for the /proc/TGID/task/ directories */
static int proc_task_readdir(struct file *file, struct dir_context *ctx)
{
	struct inode *inode = file_inode(file);
	struct task_struct *task;
	struct pid_namespace *ns;
	int tid;

	if (proc_inode_is_dead(inode))
		return -ENOENT;

	if (!dir_emit_dots(file, ctx))
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* f_version caches the tgid value that the last readdir call couldn't
	 * return. lseek aka telldir automagically resets f_version to 0.
	 */
<<<<<<< HEAD
	ns = filp->f_dentry->d_sb->s_fs_info;
	tid = (int)filp->f_version;
	filp->f_version = 0;
	for (task = first_tid(leader, tid, filp->f_pos - 2, ns);
	     task;
	     task = next_tid(task), filp->f_pos++) {
		tid = task_pid_nr_ns(task, ns);
		if (proc_task_fill_cache(filp, dirent, filldir, task, tid) < 0) {
			/* returning this tgid failed, save it as the first
			 * pid for the next readir call */
			filp->f_version = (u64)tid;
=======
	ns = proc_pid_ns(inode->i_sb);
	tid = (int)file->f_version;
	file->f_version = 0;
	for (task = first_tid(proc_pid(inode), tid, ctx->pos - 2, ns);
	     task;
	     task = next_tid(task), ctx->pos++) {
		char name[10 + 1];
		unsigned int len;

		tid = task_pid_nr_ns(task, ns);
		if (!tid)
			continue;	/* The task has just exited. */
		len = snprintf(name, sizeof(name), "%u", tid);
		if (!proc_fill_cache(file, ctx, name, len,
				proc_task_instantiate, task, NULL)) {
			/* returning this tgid failed, save it as the first
			 * pid for the next readir call */
			file->f_version = (u64)tid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			put_task_struct(task);
			break;
		}
	}
<<<<<<< HEAD
out:
	put_task_struct(leader);
out_no_task:
	return retval;
}

static int proc_task_getattr(struct vfsmount *mnt, struct dentry *dentry, struct kstat *stat)
{
	struct inode *inode = dentry->d_inode;
	struct task_struct *p = get_proc_task(inode);
	generic_fillattr(inode, stat);
=======

	return 0;
}

static int proc_task_getattr(struct mnt_idmap *idmap,
			     const struct path *path, struct kstat *stat,
			     u32 request_mask, unsigned int query_flags)
{
	struct inode *inode = d_inode(path->dentry);
	struct task_struct *p = get_proc_task(inode);
	generic_fillattr(&nop_mnt_idmap, request_mask, inode, stat);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (p) {
		stat->nlink += get_nr_threads(p);
		put_task_struct(p);
	}

	return 0;
}

static const struct inode_operations proc_task_inode_operations = {
	.lookup		= proc_task_lookup,
	.getattr	= proc_task_getattr,
	.setattr	= proc_setattr,
	.permission	= proc_pid_permission,
};

static const struct file_operations proc_task_operations = {
	.read		= generic_read_dir,
<<<<<<< HEAD
	.readdir	= proc_task_readdir,
	.llseek		= default_llseek,
};
=======
	.iterate_shared	= proc_task_readdir,
	.llseek		= generic_file_llseek,
};

void __init set_proc_pid_nlink(void)
{
	nlink_tid = pid_entry_nlink(tid_base_stuff, ARRAY_SIZE(tid_base_stuff));
	nlink_tgid = pid_entry_nlink(tgid_base_stuff, ARRAY_SIZE(tgid_base_stuff));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
