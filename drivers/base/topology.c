<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * driver/base/topology.c - Populate sysfs with cpu topology information
 *
 * Written by: Zhang Yanmin, Intel Corporation
 *
 * Copyright (C) 2006, Intel Corp.
 *
 * All rights reserved.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include <linux/init.h>
=======
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/hardirq.h>
#include <linux/topology.h>

<<<<<<< HEAD
#define define_one_ro_named(_name, _func)				\
	static DEVICE_ATTR(_name, 0444, _func, NULL)

#define define_one_ro(_name)				\
	static DEVICE_ATTR(_name, 0444, show_##_name, NULL)

#define define_id_show_func(name)				\
static ssize_t show_##name(struct device *dev,			\
		struct device_attribute *attr, char *buf)	\
{								\
	unsigned int cpu = dev->id;				\
	return sprintf(buf, "%d\n", topology_##name(cpu));	\
}

#if defined(topology_thread_cpumask) || defined(topology_core_cpumask) || \
    defined(topology_book_cpumask)
static ssize_t show_cpumap(int type, const struct cpumask *mask, char *buf)
{
	ptrdiff_t len = PTR_ALIGN(buf + PAGE_SIZE - 1, PAGE_SIZE) - buf;
	int n = 0;

	if (len > 1) {
		n = type?
			cpulist_scnprintf(buf, len-2, mask) :
			cpumask_scnprintf(buf, len-2, mask);
		buf[n++] = '\n';
		buf[n] = '\0';
	}
	return n;
}
#endif

#ifdef arch_provides_topology_pointers
#define define_siblings_show_map(name)					\
static ssize_t show_##name(struct device *dev,				\
			   struct device_attribute *attr, char *buf)	\
{									\
	unsigned int cpu = dev->id;					\
	return show_cpumap(0, topology_##name(cpu), buf);		\
}

#define define_siblings_show_list(name)					\
static ssize_t show_##name##_list(struct device *dev,			\
				  struct device_attribute *attr,	\
				  char *buf)				\
{									\
	unsigned int cpu = dev->id;					\
	return show_cpumap(1, topology_##name(cpu), buf);		\
}

#else
#define define_siblings_show_map(name)					\
static ssize_t show_##name(struct device *dev,				\
			   struct device_attribute *attr, char *buf)	\
{									\
	return show_cpumap(0, topology_##name(dev->id), buf);		\
}

#define define_siblings_show_list(name)					\
static ssize_t show_##name##_list(struct device *dev,			\
				  struct device_attribute *attr,	\
				  char *buf)				\
{									\
	return show_cpumap(1, topology_##name(dev->id), buf);		\
}
#endif

#define define_siblings_show_func(name)		\
	define_siblings_show_map(name); define_siblings_show_list(name)

define_id_show_func(physical_package_id);
define_one_ro(physical_package_id);

define_id_show_func(core_id);
define_one_ro(core_id);

define_siblings_show_func(thread_cpumask);
define_one_ro_named(thread_siblings, show_thread_cpumask);
define_one_ro_named(thread_siblings_list, show_thread_cpumask_list);

define_siblings_show_func(core_cpumask);
define_one_ro_named(core_siblings, show_core_cpumask);
define_one_ro_named(core_siblings_list, show_core_cpumask_list);

#ifdef CONFIG_SCHED_BOOK
define_id_show_func(book_id);
define_one_ro(book_id);
define_siblings_show_func(book_cpumask);
define_one_ro_named(book_siblings, show_book_cpumask);
define_one_ro_named(book_siblings_list, show_book_cpumask_list);
#endif

static struct attribute *default_attrs[] = {
	&dev_attr_physical_package_id.attr,
	&dev_attr_core_id.attr,
	&dev_attr_thread_siblings.attr,
	&dev_attr_thread_siblings_list.attr,
	&dev_attr_core_siblings.attr,
	&dev_attr_core_siblings_list.attr,
#ifdef CONFIG_SCHED_BOOK
	&dev_attr_book_id.attr,
	&dev_attr_book_siblings.attr,
	&dev_attr_book_siblings_list.attr,
=======
#define define_id_show_func(name, fmt)					\
static ssize_t name##_show(struct device *dev,				\
			   struct device_attribute *attr, char *buf)	\
{									\
	return sysfs_emit(buf, fmt "\n", topology_##name(dev->id));	\
}

#define define_siblings_read_func(name, mask)					\
static ssize_t name##_read(struct file *file, struct kobject *kobj,		\
			   struct bin_attribute *attr, char *buf,		\
			   loff_t off, size_t count)				\
{										\
	struct device *dev = kobj_to_dev(kobj);                                 \
										\
	return cpumap_print_bitmask_to_buf(buf, topology_##mask(dev->id),	\
					   off, count);                         \
}										\
										\
static ssize_t name##_list_read(struct file *file, struct kobject *kobj,	\
				struct bin_attribute *attr, char *buf,		\
				loff_t off, size_t count)			\
{										\
	struct device *dev = kobj_to_dev(kobj);					\
										\
	return cpumap_print_list_to_buf(buf, topology_##mask(dev->id),		\
					off, count);				\
}

define_id_show_func(physical_package_id, "%d");
static DEVICE_ATTR_RO(physical_package_id);

#ifdef TOPOLOGY_DIE_SYSFS
define_id_show_func(die_id, "%d");
static DEVICE_ATTR_RO(die_id);
#endif

#ifdef TOPOLOGY_CLUSTER_SYSFS
define_id_show_func(cluster_id, "%d");
static DEVICE_ATTR_RO(cluster_id);
#endif

define_id_show_func(core_id, "%d");
static DEVICE_ATTR_RO(core_id);

define_id_show_func(ppin, "0x%llx");
static DEVICE_ATTR_ADMIN_RO(ppin);

define_siblings_read_func(thread_siblings, sibling_cpumask);
static BIN_ATTR_RO(thread_siblings, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(thread_siblings_list, CPULIST_FILE_MAX_BYTES);

define_siblings_read_func(core_cpus, sibling_cpumask);
static BIN_ATTR_RO(core_cpus, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(core_cpus_list, CPULIST_FILE_MAX_BYTES);

define_siblings_read_func(core_siblings, core_cpumask);
static BIN_ATTR_RO(core_siblings, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(core_siblings_list, CPULIST_FILE_MAX_BYTES);

#ifdef TOPOLOGY_CLUSTER_SYSFS
define_siblings_read_func(cluster_cpus, cluster_cpumask);
static BIN_ATTR_RO(cluster_cpus, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(cluster_cpus_list, CPULIST_FILE_MAX_BYTES);
#endif

#ifdef TOPOLOGY_DIE_SYSFS
define_siblings_read_func(die_cpus, die_cpumask);
static BIN_ATTR_RO(die_cpus, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(die_cpus_list, CPULIST_FILE_MAX_BYTES);
#endif

define_siblings_read_func(package_cpus, core_cpumask);
static BIN_ATTR_RO(package_cpus, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(package_cpus_list, CPULIST_FILE_MAX_BYTES);

#ifdef TOPOLOGY_BOOK_SYSFS
define_id_show_func(book_id, "%d");
static DEVICE_ATTR_RO(book_id);
define_siblings_read_func(book_siblings, book_cpumask);
static BIN_ATTR_RO(book_siblings, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(book_siblings_list, CPULIST_FILE_MAX_BYTES);
#endif

#ifdef TOPOLOGY_DRAWER_SYSFS
define_id_show_func(drawer_id, "%d");
static DEVICE_ATTR_RO(drawer_id);
define_siblings_read_func(drawer_siblings, drawer_cpumask);
static BIN_ATTR_RO(drawer_siblings, CPUMAP_FILE_MAX_BYTES);
static BIN_ATTR_RO(drawer_siblings_list, CPULIST_FILE_MAX_BYTES);
#endif

static struct bin_attribute *bin_attrs[] = {
	&bin_attr_core_cpus,
	&bin_attr_core_cpus_list,
	&bin_attr_thread_siblings,
	&bin_attr_thread_siblings_list,
	&bin_attr_core_siblings,
	&bin_attr_core_siblings_list,
#ifdef TOPOLOGY_CLUSTER_SYSFS
	&bin_attr_cluster_cpus,
	&bin_attr_cluster_cpus_list,
#endif
#ifdef TOPOLOGY_DIE_SYSFS
	&bin_attr_die_cpus,
	&bin_attr_die_cpus_list,
#endif
	&bin_attr_package_cpus,
	&bin_attr_package_cpus_list,
#ifdef TOPOLOGY_BOOK_SYSFS
	&bin_attr_book_siblings,
	&bin_attr_book_siblings_list,
#endif
#ifdef TOPOLOGY_DRAWER_SYSFS
	&bin_attr_drawer_siblings,
	&bin_attr_drawer_siblings_list,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	NULL
};

<<<<<<< HEAD
static struct attribute_group topology_attr_group = {
	.attrs = default_attrs,
=======
static struct attribute *default_attrs[] = {
	&dev_attr_physical_package_id.attr,
#ifdef TOPOLOGY_DIE_SYSFS
	&dev_attr_die_id.attr,
#endif
#ifdef TOPOLOGY_CLUSTER_SYSFS
	&dev_attr_cluster_id.attr,
#endif
	&dev_attr_core_id.attr,
#ifdef TOPOLOGY_BOOK_SYSFS
	&dev_attr_book_id.attr,
#endif
#ifdef TOPOLOGY_DRAWER_SYSFS
	&dev_attr_drawer_id.attr,
#endif
	&dev_attr_ppin.attr,
	NULL
};

static umode_t topology_is_visible(struct kobject *kobj,
				   struct attribute *attr, int unused)
{
	if (attr == &dev_attr_ppin.attr && !topology_ppin(kobj_to_dev(kobj)->id))
		return 0;

	return attr->mode;
}

static const struct attribute_group topology_attr_group = {
	.attrs = default_attrs,
	.bin_attrs = bin_attrs,
	.is_visible = topology_is_visible,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name = "topology"
};

/* Add/Remove cpu_topology interface for CPU device */
<<<<<<< HEAD
static int __cpuinit topology_add_dev(unsigned int cpu)
=======
static int topology_add_dev(unsigned int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device *dev = get_cpu_device(cpu);

	return sysfs_create_group(&dev->kobj, &topology_attr_group);
}

<<<<<<< HEAD
static void __cpuinit topology_remove_dev(unsigned int cpu)
=======
static int topology_remove_dev(unsigned int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device *dev = get_cpu_device(cpu);

	sysfs_remove_group(&dev->kobj, &topology_attr_group);
<<<<<<< HEAD
}

static int __cpuinit topology_cpu_callback(struct notifier_block *nfb,
					   unsigned long action, void *hcpu)
{
	unsigned int cpu = (unsigned long)hcpu;
	int rc = 0;

	switch (action) {
	case CPU_UP_PREPARE:
	case CPU_UP_PREPARE_FROZEN:
		rc = topology_add_dev(cpu);
		break;
	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		topology_remove_dev(cpu);
		break;
	}
	return notifier_from_errno(rc);
}

static int __cpuinit topology_sysfs_init(void)
{
	int cpu;
	int rc;

	for_each_online_cpu(cpu) {
		rc = topology_add_dev(cpu);
		if (rc)
			return rc;
	}
	hotcpu_notifier(topology_cpu_callback, 0);

	return 0;
}

=======
	return 0;
}

static int __init topology_sysfs_init(void)
{
	return cpuhp_setup_state(CPUHP_TOPOLOGY_PREPARE,
				 "base/topology:prepare", topology_add_dev,
				 topology_remove_dev);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
device_initcall(topology_sysfs_init);
