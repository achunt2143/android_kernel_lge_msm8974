<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/stat.h>
#include <asm/macio.h>


#define macio_config_of_attr(field, format_string)			\
static ssize_t								\
field##_show (struct device *dev, struct device_attribute *attr,	\
              char *buf)						\
{									\
	struct macio_dev *mdev = to_macio_device (dev);			\
	return sprintf (buf, format_string, mdev->ofdev.dev.of_node->field); \
}

=======
// SPDX-License-Identifier: GPL-2.0
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/stat.h>
#include <asm/macio.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
compatible_show (struct device *dev, struct device_attribute *attr, char *buf)
{
	struct platform_device *of;
	const char *compat;
	int cplen;
	int length = 0;

	of = &to_macio_device (dev)->ofdev;
	compat = of_get_property(of->dev.of_node, "compatible", &cplen);
	if (!compat) {
		*buf = '\0';
		return 0;
	}
	while (cplen > 0) {
		int l;
		length += sprintf (buf, "%s\n", compat);
		buf += length;
		l = strlen (compat) + 1;
		compat += l;
		cplen -= l;
	}

	return length;
}
<<<<<<< HEAD
=======
static DEVICE_ATTR_RO(compatible);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t modalias_show (struct device *dev, struct device_attribute *attr,
			      char *buf)
{
<<<<<<< HEAD
	int len = of_device_get_modalias(dev, buf, PAGE_SIZE - 2);

	buf[len] = '\n';
	buf[len+1] = 0;

	return len+1;
=======
	return of_device_modalias(dev, buf, PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t devspec_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct platform_device *ofdev;

	ofdev = to_platform_device(dev);
<<<<<<< HEAD
	return sprintf(buf, "%s\n", ofdev->dev.of_node->full_name);
}

macio_config_of_attr (name, "%s\n");
macio_config_of_attr (type, "%s\n");

struct device_attribute macio_dev_attrs[] = {
	__ATTR_RO(name),
	__ATTR_RO(type),
	__ATTR_RO(compatible),
	__ATTR_RO(modalias),
	__ATTR_RO(devspec),
	__ATTR_NULL
=======
	return sprintf(buf, "%pOF\n", ofdev->dev.of_node);
}
static DEVICE_ATTR_RO(modalias);
static DEVICE_ATTR_RO(devspec);

static ssize_t name_show(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%pOFn\n", dev->of_node);
}
static DEVICE_ATTR_RO(name);

static ssize_t type_show(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", of_node_get_device_type(dev->of_node));
}
static DEVICE_ATTR_RO(type);

static struct attribute *macio_dev_attrs[] = {
	&dev_attr_name.attr,
	&dev_attr_type.attr,
	&dev_attr_compatible.attr,
	&dev_attr_modalias.attr,
	&dev_attr_devspec.attr,
	NULL,
};

static const struct attribute_group macio_dev_group = {
	.attrs = macio_dev_attrs,
};

const struct attribute_group *macio_dev_groups[] = {
	&macio_dev_group,
	NULL,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
