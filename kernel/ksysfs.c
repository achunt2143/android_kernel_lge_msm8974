<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * kernel/ksysfs.c - sysfs attributes in /sys/kernel, which
 * 		     are not related to any other subsystem
 *
 * Copyright (C) 2004 Kay Sievers <kay.sievers@vrfy.org>
<<<<<<< HEAD
 * 
 * This file is release under the GPLv2
 *
 */

=======
 */

#include <asm/byteorder.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/export.h>
#include <linux/init.h>
#include <linux/kexec.h>
#include <linux/profile.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/capability.h>
<<<<<<< HEAD
=======
#include <linux/compiler.h>

#include <linux/rcupdate.h>	/* rcu_expedited and rcu_normal */

#if defined(__LITTLE_ENDIAN)
#define CPU_BYTEORDER_STRING	"little"
#elif defined(__BIG_ENDIAN)
#define CPU_BYTEORDER_STRING	"big"
#else
#error Unknown byteorder
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define KERNEL_ATTR_RO(_name) \
static struct kobj_attribute _name##_attr = __ATTR_RO(_name)

#define KERNEL_ATTR_RW(_name) \
<<<<<<< HEAD
static struct kobj_attribute _name##_attr = \
	__ATTR(_name, 0644, _name##_show, _name##_store)

#if defined(CONFIG_HOTPLUG)
=======
static struct kobj_attribute _name##_attr = __ATTR_RW(_name)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* current uevent sequence number */
static ssize_t uevent_seqnum_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%llu\n", (unsigned long long)uevent_seqnum);
}
KERNEL_ATTR_RO(uevent_seqnum);

=======
	return sysfs_emit(buf, "%llu\n", (u64)atomic64_read(&uevent_seqnum));
}
KERNEL_ATTR_RO(uevent_seqnum);

/* cpu byteorder */
static ssize_t cpu_byteorder_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%s\n", CPU_BYTEORDER_STRING);
}
KERNEL_ATTR_RO(cpu_byteorder);

/* address bits */
static ssize_t address_bits_show(struct kobject *kobj,
				 struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%zu\n", sizeof(void *) * 8 /* CHAR_BIT */);
}
KERNEL_ATTR_RO(address_bits);

#ifdef CONFIG_UEVENT_HELPER
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* uevent helper program, used during early boot */
static ssize_t uevent_helper_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%s\n", uevent_helper);
=======
	return sysfs_emit(buf, "%s\n", uevent_helper);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static ssize_t uevent_helper_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	if (count+1 > UEVENT_HELPER_PATH_LEN)
		return -ENOENT;
	memcpy(uevent_helper, buf, count);
	uevent_helper[count] = '\0';
	if (count && uevent_helper[count-1] == '\n')
		uevent_helper[count-1] = '\0';
	return count;
}
KERNEL_ATTR_RW(uevent_helper);
#endif

#ifdef CONFIG_PROFILING
static ssize_t profiling_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", prof_on);
=======
	return sysfs_emit(buf, "%d\n", prof_on);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static ssize_t profiling_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	int ret;

	if (prof_on)
		return -EEXIST;
	/*
	 * This eventually calls into get_option() which
	 * has a ton of callers and is not const.  It is
	 * easiest to cast it away here.
	 */
	profile_setup((char *)buf);
	ret = profile_init();
	if (ret)
		return ret;
	ret = create_proc_profile();
	if (ret)
		return ret;
	return count;
}
KERNEL_ATTR_RW(profiling);
#endif

<<<<<<< HEAD
#ifdef CONFIG_KEXEC
static ssize_t kexec_loaded_show(struct kobject *kobj,
				 struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", !!kexec_image);
}
KERNEL_ATTR_RO(kexec_loaded);

static ssize_t kexec_crash_loaded_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", !!kexec_crash_image);
=======
#ifdef CONFIG_KEXEC_CORE
static ssize_t kexec_loaded_show(struct kobject *kobj,
				 struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", !!kexec_image);
}
KERNEL_ATTR_RO(kexec_loaded);

#ifdef CONFIG_CRASH_DUMP
static ssize_t kexec_crash_loaded_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", kexec_crash_loaded());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
KERNEL_ATTR_RO(kexec_crash_loaded);

static ssize_t kexec_crash_size_show(struct kobject *kobj,
				       struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%zu\n", crash_get_memory_size());
=======
	ssize_t size = crash_get_memory_size();

	if (size < 0)
		return size;

	return sysfs_emit(buf, "%zd\n", size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static ssize_t kexec_crash_size_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	unsigned long cnt;
	int ret;

<<<<<<< HEAD
	if (strict_strtoul(buf, 0, &cnt))
=======
	if (kstrtoul(buf, 0, &cnt))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	ret = crash_shrink_memory(cnt);
	return ret < 0 ? ret : count;
}
KERNEL_ATTR_RW(kexec_crash_size);

<<<<<<< HEAD
static ssize_t vmcoreinfo_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%lx %x\n",
		       paddr_vmcoreinfo_note(),
		       (unsigned int)vmcoreinfo_max_size);
}
KERNEL_ATTR_RO(vmcoreinfo);

#endif /* CONFIG_KEXEC */
=======
#endif /* CONFIG_CRASH_DUMP*/
#endif /* CONFIG_KEXEC_CORE */

#ifdef CONFIG_VMCORE_INFO

static ssize_t vmcoreinfo_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	phys_addr_t vmcore_base = paddr_vmcoreinfo_note();
	return sysfs_emit(buf, "%pa %x\n", &vmcore_base,
			  (unsigned int)VMCOREINFO_NOTE_SIZE);
}
KERNEL_ATTR_RO(vmcoreinfo);

#ifdef CONFIG_CRASH_HOTPLUG
static ssize_t crash_elfcorehdr_size_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	unsigned int sz = crash_get_elfcorehdr_size();

	return sysfs_emit(buf, "%u\n", sz);
}
KERNEL_ATTR_RO(crash_elfcorehdr_size);

#endif

#endif /* CONFIG_VMCORE_INFO */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* whether file capabilities are enabled */
static ssize_t fscaps_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", file_caps_enabled);
}
KERNEL_ATTR_RO(fscaps);

=======
	return sysfs_emit(buf, "%d\n", file_caps_enabled);
}
KERNEL_ATTR_RO(fscaps);

#ifndef CONFIG_TINY_RCU
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int rcu_expedited;
static ssize_t rcu_expedited_show(struct kobject *kobj,
				  struct kobj_attribute *attr, char *buf)
{
<<<<<<< HEAD
	return sprintf(buf, "%d\n", rcu_expedited);
=======
	return sysfs_emit(buf, "%d\n", READ_ONCE(rcu_expedited));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static ssize_t rcu_expedited_store(struct kobject *kobj,
				   struct kobj_attribute *attr,
				   const char *buf, size_t count)
{
	if (kstrtoint(buf, 0, &rcu_expedited))
		return -EINVAL;

	return count;
}
KERNEL_ATTR_RW(rcu_expedited);

<<<<<<< HEAD
/*
 * Make /sys/kernel/notes give the raw contents of our kernel .notes section.
 */
extern const void __start_notes __attribute__((weak));
extern const void __stop_notes __attribute__((weak));
=======
int rcu_normal;
static ssize_t rcu_normal_show(struct kobject *kobj,
			       struct kobj_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "%d\n", READ_ONCE(rcu_normal));
}
static ssize_t rcu_normal_store(struct kobject *kobj,
				struct kobj_attribute *attr,
				const char *buf, size_t count)
{
	if (kstrtoint(buf, 0, &rcu_normal))
		return -EINVAL;

	return count;
}
KERNEL_ATTR_RW(rcu_normal);
#endif /* #ifndef CONFIG_TINY_RCU */

/*
 * Make /sys/kernel/notes give the raw contents of our kernel .notes section.
 */
extern const void __start_notes __weak;
extern const void __stop_notes __weak;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define	notes_size (&__stop_notes - &__start_notes)

static ssize_t notes_read(struct file *filp, struct kobject *kobj,
			  struct bin_attribute *bin_attr,
			  char *buf, loff_t off, size_t count)
{
	memcpy(buf, &__start_notes + off, count);
	return count;
}

<<<<<<< HEAD
static struct bin_attribute notes_attr = {
=======
static struct bin_attribute notes_attr __ro_after_init  = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.attr = {
		.name = "notes",
		.mode = S_IRUGO,
	},
	.read = &notes_read,
};

struct kobject *kernel_kobj;
EXPORT_SYMBOL_GPL(kernel_kobj);

static struct attribute * kernel_attrs[] = {
	&fscaps_attr.attr,
<<<<<<< HEAD
#if defined(CONFIG_HOTPLUG)
	&uevent_seqnum_attr.attr,
=======
	&uevent_seqnum_attr.attr,
	&cpu_byteorder_attr.attr,
	&address_bits_attr.attr,
#ifdef CONFIG_UEVENT_HELPER
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&uevent_helper_attr.attr,
#endif
#ifdef CONFIG_PROFILING
	&profiling_attr.attr,
#endif
<<<<<<< HEAD
#ifdef CONFIG_KEXEC
	&kexec_loaded_attr.attr,
	&kexec_crash_loaded_attr.attr,
	&kexec_crash_size_attr.attr,
	&vmcoreinfo_attr.attr,
#endif
	&rcu_expedited_attr.attr,
	NULL
};

static struct attribute_group kernel_attr_group = {
=======
#ifdef CONFIG_KEXEC_CORE
	&kexec_loaded_attr.attr,
#ifdef CONFIG_CRASH_DUMP
	&kexec_crash_loaded_attr.attr,
	&kexec_crash_size_attr.attr,
#endif
#endif
#ifdef CONFIG_VMCORE_INFO
	&vmcoreinfo_attr.attr,
#ifdef CONFIG_CRASH_HOTPLUG
	&crash_elfcorehdr_size_attr.attr,
#endif
#endif
#ifndef CONFIG_TINY_RCU
	&rcu_expedited_attr.attr,
	&rcu_normal_attr.attr,
#endif
	NULL
};

static const struct attribute_group kernel_attr_group = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.attrs = kernel_attrs,
};

static int __init ksysfs_init(void)
{
	int error;

	kernel_kobj = kobject_create_and_add("kernel", NULL);
	if (!kernel_kobj) {
		error = -ENOMEM;
		goto exit;
	}
	error = sysfs_create_group(kernel_kobj, &kernel_attr_group);
	if (error)
		goto kset_exit;

	if (notes_size > 0) {
		notes_attr.size = notes_size;
		error = sysfs_create_bin_file(kernel_kobj, &notes_attr);
		if (error)
			goto group_exit;
	}

	return 0;

group_exit:
	sysfs_remove_group(kernel_kobj, &kernel_attr_group);
kset_exit:
	kobject_put(kernel_kobj);
exit:
	return error;
}

core_initcall(ksysfs_init);
