<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* ----------------------------------------------------------------------- *
 *
 *   Copyright 2000-2008 H. Peter Anvin - All Rights Reserved
 *   Copyright 2009 Intel Corporation; author: H. Peter Anvin
 *
<<<<<<< HEAD
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 675 Mass Ave, Cambridge MA 02139,
 *   USA; either version 2 of the License, or (at your option) any later
 *   version; incorporated herein by reference.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ----------------------------------------------------------------------- */

/*
 * x86 MSR access device
 *
 * This device is accessed by lseek() to the appropriate register number
 * and then read/write in chunks of 8 bytes.  A larger size means multiple
 * reads or writes of the same register.
 *
 * This driver uses /dev/cpu/%d/msr where %d is the minor number, and on
 * an SMP box will direct the access to CPU %d.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>

#include <linux/types.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/poll.h>
#include <linux/smp.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/uaccess.h>
#include <linux/gfp.h>
<<<<<<< HEAD

#include <asm/processor.h>
#include <asm/msr.h>

static struct class *msr_class;

static loff_t msr_seek(struct file *file, loff_t offset, int orig)
{
	loff_t ret;
	struct inode *inode = file->f_mapping->host;

	mutex_lock(&inode->i_mutex);
	switch (orig) {
	case 0:
		file->f_pos = offset;
		ret = file->f_pos;
		break;
	case 1:
		file->f_pos += offset;
		ret = file->f_pos;
		break;
	default:
		ret = -EINVAL;
	}
	mutex_unlock(&inode->i_mutex);
	return ret;
}
=======
#include <linux/security.h>

#include <asm/cpufeature.h>
#include <asm/msr.h>

static enum cpuhp_state cpuhp_msr_state;

enum allow_write_msrs {
	MSR_WRITES_ON,
	MSR_WRITES_OFF,
	MSR_WRITES_DEFAULT,
};

static enum allow_write_msrs allow_writes = MSR_WRITES_DEFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t msr_read(struct file *file, char __user *buf,
			size_t count, loff_t *ppos)
{
	u32 __user *tmp = (u32 __user *) buf;
	u32 data[2];
	u32 reg = *ppos;
<<<<<<< HEAD
	int cpu = iminor(file->f_path.dentry->d_inode);
=======
	int cpu = iminor(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;
	ssize_t bytes = 0;

	if (count % 8)
		return -EINVAL;	/* Invalid chunk size */

	for (; count; count -= 8) {
		err = rdmsr_safe_on_cpu(cpu, reg, &data[0], &data[1]);
		if (err)
			break;
		if (copy_to_user(tmp, &data, 8)) {
			err = -EFAULT;
			break;
		}
		tmp += 2;
		bytes += 8;
	}

	return bytes ? bytes : err;
}

<<<<<<< HEAD
=======
static int filter_write(u32 reg)
{
	/*
	 * MSRs writes usually happen all at once, and can easily saturate kmsg.
	 * Only allow one message every 30 seconds.
	 *
	 * It's possible to be smarter here and do it (for example) per-MSR, but
	 * it would certainly be more complex, and this is enough at least to
	 * avoid saturating the ring buffer.
	 */
	static DEFINE_RATELIMIT_STATE(fw_rs, 30 * HZ, 1);

	switch (allow_writes) {
	case MSR_WRITES_ON:  return 0;
	case MSR_WRITES_OFF: return -EPERM;
	default: break;
	}

	if (!__ratelimit(&fw_rs))
		return 0;

	pr_warn("Write to unrecognized MSR 0x%x by %s (pid: %d).\n",
	        reg, current->comm, current->pid);
	pr_warn("See https://git.kernel.org/pub/scm/linux/kernel/git/tip/tip.git/about for details.\n");

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t msr_write(struct file *file, const char __user *buf,
			 size_t count, loff_t *ppos)
{
	const u32 __user *tmp = (const u32 __user *)buf;
	u32 data[2];
	u32 reg = *ppos;
<<<<<<< HEAD
	int cpu = iminor(file->f_path.dentry->d_inode);
	int err = 0;
	ssize_t bytes = 0;

=======
	int cpu = iminor(file_inode(file));
	int err = 0;
	ssize_t bytes = 0;

	err = security_locked_down(LOCKDOWN_MSR);
	if (err)
		return err;

	err = filter_write(reg);
	if (err)
		return err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (count % 8)
		return -EINVAL;	/* Invalid chunk size */

	for (; count; count -= 8) {
		if (copy_from_user(&data, tmp, 8)) {
			err = -EFAULT;
			break;
		}
<<<<<<< HEAD
		err = wrmsr_safe_on_cpu(cpu, reg, data[0], data[1]);
		if (err)
			break;
=======

		add_taint(TAINT_CPU_OUT_OF_SPEC, LOCKDEP_STILL_OK);

		err = wrmsr_safe_on_cpu(cpu, reg, data[0], data[1]);
		if (err)
			break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tmp += 2;
		bytes += 8;
	}

	return bytes ? bytes : err;
}

static long msr_ioctl(struct file *file, unsigned int ioc, unsigned long arg)
{
	u32 __user *uregs = (u32 __user *)arg;
	u32 regs[8];
<<<<<<< HEAD
	int cpu = iminor(file->f_path.dentry->d_inode);
=======
	int cpu = iminor(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	switch (ioc) {
	case X86_IOC_RDMSR_REGS:
		if (!(file->f_mode & FMODE_READ)) {
			err = -EBADF;
			break;
		}
<<<<<<< HEAD
		if (copy_from_user(&regs, uregs, sizeof regs)) {
=======
		if (copy_from_user(&regs, uregs, sizeof(regs))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EFAULT;
			break;
		}
		err = rdmsr_safe_regs_on_cpu(cpu, regs);
		if (err)
			break;
<<<<<<< HEAD
		if (copy_to_user(uregs, &regs, sizeof regs))
=======
		if (copy_to_user(uregs, &regs, sizeof(regs)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EFAULT;
		break;

	case X86_IOC_WRMSR_REGS:
		if (!(file->f_mode & FMODE_WRITE)) {
			err = -EBADF;
			break;
		}
<<<<<<< HEAD
		if (copy_from_user(&regs, uregs, sizeof regs)) {
			err = -EFAULT;
			break;
		}
		err = wrmsr_safe_regs_on_cpu(cpu, regs);
		if (err)
			break;
		if (copy_to_user(uregs, &regs, sizeof regs))
=======
		if (copy_from_user(&regs, uregs, sizeof(regs))) {
			err = -EFAULT;
			break;
		}
		err = security_locked_down(LOCKDOWN_MSR);
		if (err)
			break;

		err = filter_write(regs[1]);
		if (err)
			return err;

		add_taint(TAINT_CPU_OUT_OF_SPEC, LOCKDEP_STILL_OK);

		err = wrmsr_safe_regs_on_cpu(cpu, regs);
		if (err)
			break;
		if (copy_to_user(uregs, &regs, sizeof(regs)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = -EFAULT;
		break;

	default:
		err = -ENOTTY;
		break;
	}

	return err;
}

static int msr_open(struct inode *inode, struct file *file)
{
<<<<<<< HEAD
	unsigned int cpu;
=======
	unsigned int cpu = iminor(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct cpuinfo_x86 *c;

	if (!capable(CAP_SYS_RAWIO))
		return -EPERM;

<<<<<<< HEAD
	cpu = iminor(file->f_path.dentry->d_inode);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cpu >= nr_cpu_ids || !cpu_online(cpu))
		return -ENXIO;	/* No such CPU */

	c = &cpu_data(cpu);
	if (!cpu_has(c, X86_FEATURE_MSR))
		return -EIO;	/* MSR not supported */

	return 0;
}

/*
 * File operations we support
 */
static const struct file_operations msr_fops = {
	.owner = THIS_MODULE,
<<<<<<< HEAD
	.llseek = msr_seek,
=======
	.llseek = no_seek_end_llseek,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read = msr_read,
	.write = msr_write,
	.open = msr_open,
	.unlocked_ioctl = msr_ioctl,
	.compat_ioctl = msr_ioctl,
};

<<<<<<< HEAD
static int __cpuinit msr_device_create(int cpu)
{
	struct device *dev;

	dev = device_create(msr_class, NULL, MKDEV(MSR_MAJOR, cpu), NULL,
			    "msr%d", cpu);
	return IS_ERR(dev) ? PTR_ERR(dev) : 0;
}

static void msr_device_destroy(int cpu)
{
	device_destroy(msr_class, MKDEV(MSR_MAJOR, cpu));
}

static int __cpuinit msr_class_cpu_callback(struct notifier_block *nfb,
				unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;
	int err = 0;

	switch (action) {
	case CPU_UP_PREPARE:
		err = msr_device_create(cpu);
		break;
	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
	case CPU_DEAD:
		msr_device_destroy(cpu);
		break;
	}
	return notifier_from_errno(err);
}

static struct notifier_block __refdata msr_class_cpu_notifier = {
	.notifier_call = msr_class_cpu_callback,
};

static char *msr_devnode(struct device *dev, umode_t *mode)
=======
static char *msr_devnode(const struct device *dev, umode_t *mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return kasprintf(GFP_KERNEL, "cpu/%u/msr", MINOR(dev->devt));
}

<<<<<<< HEAD
static int __init msr_init(void)
{
	int i, err = 0;
	i = 0;

	if (__register_chrdev(MSR_MAJOR, 0, NR_CPUS, "cpu/msr", &msr_fops)) {
		printk(KERN_ERR "msr: unable to get major %d for msr\n",
		       MSR_MAJOR);
		err = -EBUSY;
		goto out;
	}
	msr_class = class_create(THIS_MODULE, "msr");
	if (IS_ERR(msr_class)) {
		err = PTR_ERR(msr_class);
		goto out_chrdev;
	}
	msr_class->devnode = msr_devnode;
	for_each_online_cpu(i) {
		err = msr_device_create(i);
		if (err != 0)
			goto out_class;
	}
	register_hotcpu_notifier(&msr_class_cpu_notifier);

	err = 0;
	goto out;

out_class:
	i = 0;
	for_each_online_cpu(i)
		msr_device_destroy(i);
	class_destroy(msr_class);
out_chrdev:
	__unregister_chrdev(MSR_MAJOR, 0, NR_CPUS, "cpu/msr");
out:
	return err;
}

static void __exit msr_exit(void)
{
	int cpu = 0;
	for_each_online_cpu(cpu)
		msr_device_destroy(cpu);
	class_destroy(msr_class);
	__unregister_chrdev(MSR_MAJOR, 0, NR_CPUS, "cpu/msr");
	unregister_hotcpu_notifier(&msr_class_cpu_notifier);
}

module_init(msr_init);
module_exit(msr_exit)
=======
static const struct class msr_class = {
	.name		= "msr",
	.devnode	= msr_devnode,
};

static int msr_device_create(unsigned int cpu)
{
	struct device *dev;

	dev = device_create(&msr_class, NULL, MKDEV(MSR_MAJOR, cpu), NULL,
			    "msr%d", cpu);
	return PTR_ERR_OR_ZERO(dev);
}

static int msr_device_destroy(unsigned int cpu)
{
	device_destroy(&msr_class, MKDEV(MSR_MAJOR, cpu));
	return 0;
}

static int __init msr_init(void)
{
	int err;

	if (__register_chrdev(MSR_MAJOR, 0, NR_CPUS, "cpu/msr", &msr_fops)) {
		pr_err("unable to get major %d for msr\n", MSR_MAJOR);
		return -EBUSY;
	}
	err = class_register(&msr_class);
	if (err)
		goto out_chrdev;

	err  = cpuhp_setup_state(CPUHP_AP_ONLINE_DYN, "x86/msr:online",
				 msr_device_create, msr_device_destroy);
	if (err < 0)
		goto out_class;
	cpuhp_msr_state = err;
	return 0;

out_class:
	class_unregister(&msr_class);
out_chrdev:
	__unregister_chrdev(MSR_MAJOR, 0, NR_CPUS, "cpu/msr");
	return err;
}
module_init(msr_init);

static void __exit msr_exit(void)
{
	cpuhp_remove_state(cpuhp_msr_state);
	class_unregister(&msr_class);
	__unregister_chrdev(MSR_MAJOR, 0, NR_CPUS, "cpu/msr");
}
module_exit(msr_exit)

static int set_allow_writes(const char *val, const struct kernel_param *cp)
{
	/* val is NUL-terminated, see kernfs_fop_write() */
	char *s = strstrip((char *)val);

	if (!strcmp(s, "on"))
		allow_writes = MSR_WRITES_ON;
	else if (!strcmp(s, "off"))
		allow_writes = MSR_WRITES_OFF;
	else
		allow_writes = MSR_WRITES_DEFAULT;

	return 0;
}

static int get_allow_writes(char *buf, const struct kernel_param *kp)
{
	const char *res;

	switch (allow_writes) {
	case MSR_WRITES_ON:  res = "on"; break;
	case MSR_WRITES_OFF: res = "off"; break;
	default: res = "default"; break;
	}

	return sprintf(buf, "%s\n", res);
}

static const struct kernel_param_ops allow_writes_ops = {
	.set = set_allow_writes,
	.get = get_allow_writes
};

module_param_cb(allow_writes, &allow_writes_ops, NULL, 0600);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("H. Peter Anvin <hpa@zytor.com>");
MODULE_DESCRIPTION("x86 generic MSR driver");
MODULE_LICENSE("GPL");
