<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/time.h>
<<<<<<< HEAD
#include <linux/kernel_stat.h>
#include <asm/cputime.h>

static int uptime_proc_show(struct seq_file *m, void *v)
{
	struct timespec uptime;
	struct timespec idle;
	u64 idletime;
	u64 nsec;
	u32 rem;
	int i;

	idletime = 0;
	for_each_possible_cpu(i)
		idletime += (__force u64) kcpustat_cpu(i).cpustat[CPUTIME_IDLE];

	do_posix_clock_monotonic_gettime(&uptime);
	monotonic_to_bootbased(&uptime);
	nsec = cputime64_to_jiffies64(idletime) * TICK_NSEC;
	idle.tv_sec = div_u64_rem(nsec, NSEC_PER_SEC, &rem);
=======
#include <linux/time_namespace.h>
#include <linux/kernel_stat.h>
#include "internal.h"

static int uptime_proc_show(struct seq_file *m, void *v)
{
	struct timespec64 uptime;
	struct timespec64 idle;
	u64 idle_nsec;
	u32 rem;
	int i;

	idle_nsec = 0;
	for_each_possible_cpu(i) {
		struct kernel_cpustat kcs;

		kcpustat_cpu_fetch(&kcs, i);
		idle_nsec += get_idle_time(&kcs, i);
	}

	ktime_get_boottime_ts64(&uptime);
	timens_add_boottime(&uptime);

	idle.tv_sec = div_u64_rem(idle_nsec, NSEC_PER_SEC, &rem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	idle.tv_nsec = rem;
	seq_printf(m, "%lu.%02lu %lu.%02lu\n",
			(unsigned long) uptime.tv_sec,
			(uptime.tv_nsec / (NSEC_PER_SEC / 100)),
			(unsigned long) idle.tv_sec,
			(idle.tv_nsec / (NSEC_PER_SEC / 100)));
	return 0;
}

<<<<<<< HEAD
static int uptime_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, uptime_proc_show, NULL);
}

static const struct file_operations uptime_proc_fops = {
	.open		= uptime_proc_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= single_release,
};

static int __init proc_uptime_init(void)
{
	proc_create("uptime", 0, NULL, &uptime_proc_fops);
	return 0;
}
module_init(proc_uptime_init);
=======
static int __init proc_uptime_init(void)
{
	struct proc_dir_entry *pde;

	pde = proc_create_single("uptime", 0, NULL, uptime_proc_show);
	pde_make_permanent(pde);
	return 0;
}
fs_initcall(proc_uptime_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
