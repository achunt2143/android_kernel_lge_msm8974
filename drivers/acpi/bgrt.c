<<<<<<< HEAD
/*
 * Copyright 2012 Red Hat, Inc <mjg@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <acpi/acpi.h>
#include <acpi/acpi_bus.h>

static struct acpi_table_bgrt *bgrt_tab;
static struct kobject *bgrt_kobj;

struct bmp_header {
	u16 id;
	u32 size;
} __attribute ((packed));

static struct bmp_header bmp_header;

static ssize_t show_version(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", bgrt_tab->version);
}
static DEVICE_ATTR(version, S_IRUGO, show_version, NULL);

static ssize_t show_status(struct device *dev,
			   struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", bgrt_tab->status);
}
static DEVICE_ATTR(status, S_IRUGO, show_status, NULL);

static ssize_t show_type(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", bgrt_tab->image_type);
}
static DEVICE_ATTR(type, S_IRUGO, show_type, NULL);

static ssize_t show_xoffset(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", bgrt_tab->image_offset_x);
}
static DEVICE_ATTR(xoffset, S_IRUGO, show_xoffset, NULL);

static ssize_t show_yoffset(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	return snprintf(buf, PAGE_SIZE, "%d\n", bgrt_tab->image_offset_y);
}
static DEVICE_ATTR(yoffset, S_IRUGO, show_yoffset, NULL);

static ssize_t show_image(struct file *file, struct kobject *kobj,
	       struct bin_attribute *attr, char *buf, loff_t off, size_t count)
{
	int size = attr->size;
	void __iomem *image = attr->private;

	if (off >= size) {
		count = 0;
	} else {
		if (off + count > size)
			count = size - off;

		memcpy_fromio(buf, image+off, count);
	}

	return count;
}

static struct bin_attribute image_attr = {
	.attr = {
		.name = "image",
		.mode = S_IRUGO,
	},
	.read = show_image,
};

static struct attribute *bgrt_attributes[] = {
	&dev_attr_version.attr,
	&dev_attr_status.attr,
	&dev_attr_type.attr,
	&dev_attr_xoffset.attr,
	&dev_attr_yoffset.attr,
	NULL,
};

static struct attribute_group bgrt_attribute_group = {
	.attrs = bgrt_attributes,
};

static int __init bgrt_init(void)
{
	acpi_status status;
	int ret;
	void __iomem *bgrt;

	if (acpi_disabled)
		return -ENODEV;

	status = acpi_get_table("BGRT", 0,
				(struct acpi_table_header **)&bgrt_tab);

	if (ACPI_FAILURE(status))
		return -ENODEV;

	sysfs_bin_attr_init(&image_attr);

	bgrt = ioremap(bgrt_tab->image_address, sizeof(struct bmp_header));

	if (!bgrt) {
		ret = -EINVAL;
		goto out_err;
	}

	memcpy_fromio(&bmp_header, bgrt, sizeof(bmp_header));
	image_attr.size = bmp_header.size;
	iounmap(bgrt);

	image_attr.private = ioremap(bgrt_tab->image_address, image_attr.size);

	if (!image_attr.private) {
		ret = -EINVAL;
		goto out_err;
	}

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * BGRT boot graphic support
 * Authors: Matthew Garrett, Josh Triplett <josh@joshtriplett.org>
 * Copyright 2012 Red Hat, Inc <mjg@redhat.com>
 * Copyright 2012 Intel Corporation
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/sysfs.h>
#include <linux/efi-bgrt.h>

static void *bgrt_image;
static struct kobject *bgrt_kobj;

#define BGRT_SHOW(_name, _member) \
	static ssize_t _name##_show(struct kobject *kobj,			\
				    struct kobj_attribute *attr, char *buf)	\
	{									\
		return sysfs_emit(buf, "%d\n", bgrt_tab._member);		\
	}									\
	static struct kobj_attribute bgrt_attr_##_name = __ATTR_RO(_name)

BGRT_SHOW(version, version);
BGRT_SHOW(status, status);
BGRT_SHOW(type, image_type);
BGRT_SHOW(xoffset, image_offset_x);
BGRT_SHOW(yoffset, image_offset_y);

static ssize_t image_read(struct file *file, struct kobject *kobj,
	       struct bin_attribute *attr, char *buf, loff_t off, size_t count)
{
	memcpy(buf, attr->private + off, count);
	return count;
}

static BIN_ATTR_RO(image, 0);	/* size gets filled in later */

static struct attribute *bgrt_attributes[] = {
	&bgrt_attr_version.attr,
	&bgrt_attr_status.attr,
	&bgrt_attr_type.attr,
	&bgrt_attr_xoffset.attr,
	&bgrt_attr_yoffset.attr,
	NULL,
};

static struct bin_attribute *bgrt_bin_attributes[] = {
	&bin_attr_image,
	NULL,
};

static const struct attribute_group bgrt_attribute_group = {
	.attrs = bgrt_attributes,
	.bin_attrs = bgrt_bin_attributes,
};

int __init acpi_parse_bgrt(struct acpi_table_header *table)
{
	efi_bgrt_init(table);
	return 0;
}

static int __init bgrt_init(void)
{
	int ret;

	if (!bgrt_tab.image_address)
		return -ENODEV;

	bgrt_image = memremap(bgrt_tab.image_address, bgrt_image_size,
			      MEMREMAP_WB);
	if (!bgrt_image) {
		pr_notice("Ignoring BGRT: failed to map image memory\n");
		return -ENOMEM;
	}

	bin_attr_image.private = bgrt_image;
	bin_attr_image.size = bgrt_image_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bgrt_kobj = kobject_create_and_add("bgrt", acpi_kobj);
	if (!bgrt_kobj) {
		ret = -EINVAL;
<<<<<<< HEAD
		goto out_iounmap;
=======
		goto out_memmap;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ret = sysfs_create_group(bgrt_kobj, &bgrt_attribute_group);
	if (ret)
		goto out_kobject;

<<<<<<< HEAD
	ret = sysfs_create_bin_file(bgrt_kobj, &image_attr);
	if (ret)
		goto out_group;

	return 0;

out_group:
	sysfs_remove_group(bgrt_kobj, &bgrt_attribute_group);
out_kobject:
	kobject_put(bgrt_kobj);
out_iounmap:
	iounmap(image_attr.private);
out_err:
	return ret;
}

static void __exit bgrt_exit(void)
{
	iounmap(image_attr.private);
	sysfs_remove_group(bgrt_kobj, &bgrt_attribute_group);
	sysfs_remove_bin_file(bgrt_kobj, &image_attr);
}

module_init(bgrt_init);
module_exit(bgrt_exit);

MODULE_AUTHOR("Matthew Garrett");
MODULE_DESCRIPTION("BGRT boot graphic support");
MODULE_LICENSE("GPL");
=======
	return 0;

out_kobject:
	kobject_put(bgrt_kobj);
out_memmap:
	memunmap(bgrt_image);
	return ret;
}
device_initcall(bgrt_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
