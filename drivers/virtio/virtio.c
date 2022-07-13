<<<<<<< HEAD
#include <linux/virtio.h>
#include <linux/spinlock.h>
#include <linux/virtio_config.h>
#include <linux/module.h>

/* Unique numbering for virtio devices. */
static unsigned int dev_index;
=======
// SPDX-License-Identifier: GPL-2.0-only
#include <linux/virtio.h>
#include <linux/spinlock.h>
#include <linux/virtio_config.h>
#include <linux/virtio_anchor.h>
#include <linux/module.h>
#include <linux/idr.h>
#include <linux/of.h>
#include <uapi/linux/virtio_ids.h>

/* Unique numbering for virtio devices. */
static DEFINE_IDA(virtio_index_ida);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t device_show(struct device *_d,
			   struct device_attribute *attr, char *buf)
{
	struct virtio_device *dev = dev_to_virtio(_d);
<<<<<<< HEAD
	return sprintf(buf, "0x%04x\n", dev->id.device);
}
=======
	return sysfs_emit(buf, "0x%04x\n", dev->id.device);
}
static DEVICE_ATTR_RO(device);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t vendor_show(struct device *_d,
			   struct device_attribute *attr, char *buf)
{
	struct virtio_device *dev = dev_to_virtio(_d);
<<<<<<< HEAD
	return sprintf(buf, "0x%04x\n", dev->id.vendor);
}
=======
	return sysfs_emit(buf, "0x%04x\n", dev->id.vendor);
}
static DEVICE_ATTR_RO(vendor);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t status_show(struct device *_d,
			   struct device_attribute *attr, char *buf)
{
	struct virtio_device *dev = dev_to_virtio(_d);
<<<<<<< HEAD
	return sprintf(buf, "0x%08x\n", dev->config->get_status(dev));
}
=======
	return sysfs_emit(buf, "0x%08x\n", dev->config->get_status(dev));
}
static DEVICE_ATTR_RO(status);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t modalias_show(struct device *_d,
			     struct device_attribute *attr, char *buf)
{
	struct virtio_device *dev = dev_to_virtio(_d);
<<<<<<< HEAD
	return sprintf(buf, "virtio:d%08Xv%08X\n",
		       dev->id.device, dev->id.vendor);
}
=======
	return sysfs_emit(buf, "virtio:d%08Xv%08X\n",
		       dev->id.device, dev->id.vendor);
}
static DEVICE_ATTR_RO(modalias);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t features_show(struct device *_d,
			     struct device_attribute *attr, char *buf)
{
	struct virtio_device *dev = dev_to_virtio(_d);
	unsigned int i;
	ssize_t len = 0;

	/* We actually represent this as a bitstring, as it could be
	 * arbitrary length in future. */
<<<<<<< HEAD
	for (i = 0; i < ARRAY_SIZE(dev->features)*BITS_PER_LONG; i++)
		len += sprintf(buf+len, "%c",
			       test_bit(i, dev->features) ? '1' : '0');
	len += sprintf(buf+len, "\n");
	return len;
}
static struct device_attribute virtio_dev_attrs[] = {
	__ATTR_RO(device),
	__ATTR_RO(vendor),
	__ATTR_RO(status),
	__ATTR_RO(modalias),
	__ATTR_RO(features),
	__ATTR_NULL
};
=======
	for (i = 0; i < sizeof(dev->features)*8; i++)
		len += sysfs_emit_at(buf, len, "%c",
			       __virtio_test_bit(dev, i) ? '1' : '0');
	len += sysfs_emit_at(buf, len, "\n");
	return len;
}
static DEVICE_ATTR_RO(features);

static struct attribute *virtio_dev_attrs[] = {
	&dev_attr_device.attr,
	&dev_attr_vendor.attr,
	&dev_attr_status.attr,
	&dev_attr_modalias.attr,
	&dev_attr_features.attr,
	NULL,
};
ATTRIBUTE_GROUPS(virtio_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int virtio_id_match(const struct virtio_device *dev,
				  const struct virtio_device_id *id)
{
	if (id->device != dev->id.device && id->device != VIRTIO_DEV_ANY_ID)
		return 0;

	return id->vendor == VIRTIO_DEV_ANY_ID || id->vendor == dev->id.vendor;
}

/* This looks through all the IDs a driver claims to support.  If any of them
 * match, we return 1 and the kernel will call virtio_dev_probe(). */
static int virtio_dev_match(struct device *_dv, struct device_driver *_dr)
{
	unsigned int i;
	struct virtio_device *dev = dev_to_virtio(_dv);
	const struct virtio_device_id *ids;

<<<<<<< HEAD
	ids = container_of(_dr, struct virtio_driver, driver)->id_table;
=======
	ids = drv_to_virtio(_dr)->id_table;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; ids[i].device; i++)
		if (virtio_id_match(dev, &ids[i]))
			return 1;
	return 0;
}

<<<<<<< HEAD
static int virtio_uevent(struct device *_dv, struct kobj_uevent_env *env)
{
	struct virtio_device *dev = dev_to_virtio(_dv);
=======
static int virtio_uevent(const struct device *_dv, struct kobj_uevent_env *env)
{
	const struct virtio_device *dev = dev_to_virtio(_dv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return add_uevent_var(env, "MODALIAS=virtio:d%08Xv%08X",
			      dev->id.device, dev->id.vendor);
}

<<<<<<< HEAD
static void add_status(struct virtio_device *dev, unsigned status)
{
	dev->config->set_status(dev, dev->config->get_status(dev) | status);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void virtio_check_driver_offered_feature(const struct virtio_device *vdev,
					 unsigned int fbit)
{
	unsigned int i;
<<<<<<< HEAD
	struct virtio_driver *drv = container_of(vdev->dev.driver,
						 struct virtio_driver, driver);
=======
	struct virtio_driver *drv = drv_to_virtio(vdev->dev.driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < drv->feature_table_size; i++)
		if (drv->feature_table[i] == fbit)
			return;
<<<<<<< HEAD
=======

	if (drv->feature_table_legacy) {
		for (i = 0; i < drv->feature_table_size_legacy; i++)
			if (drv->feature_table_legacy[i] == fbit)
				return;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG();
}
EXPORT_SYMBOL_GPL(virtio_check_driver_offered_feature);

<<<<<<< HEAD
=======
static void __virtio_config_changed(struct virtio_device *dev)
{
	struct virtio_driver *drv = drv_to_virtio(dev->dev.driver);

	if (!dev->config_enabled)
		dev->config_change_pending = true;
	else if (drv && drv->config_changed)
		drv->config_changed(dev);
}

void virtio_config_changed(struct virtio_device *dev)
{
	unsigned long flags;

	spin_lock_irqsave(&dev->config_lock, flags);
	__virtio_config_changed(dev);
	spin_unlock_irqrestore(&dev->config_lock, flags);
}
EXPORT_SYMBOL_GPL(virtio_config_changed);

static void virtio_config_disable(struct virtio_device *dev)
{
	spin_lock_irq(&dev->config_lock);
	dev->config_enabled = false;
	spin_unlock_irq(&dev->config_lock);
}

static void virtio_config_enable(struct virtio_device *dev)
{
	spin_lock_irq(&dev->config_lock);
	dev->config_enabled = true;
	if (dev->config_change_pending)
		__virtio_config_changed(dev);
	dev->config_change_pending = false;
	spin_unlock_irq(&dev->config_lock);
}

void virtio_add_status(struct virtio_device *dev, unsigned int status)
{
	might_sleep();
	dev->config->set_status(dev, dev->config->get_status(dev) | status);
}
EXPORT_SYMBOL_GPL(virtio_add_status);

/* Do some validation, then set FEATURES_OK */
static int virtio_features_ok(struct virtio_device *dev)
{
	unsigned int status;

	might_sleep();

	if (virtio_check_mem_acc_cb(dev)) {
		if (!virtio_has_feature(dev, VIRTIO_F_VERSION_1)) {
			dev_warn(&dev->dev,
				 "device must provide VIRTIO_F_VERSION_1\n");
			return -ENODEV;
		}

		if (!virtio_has_feature(dev, VIRTIO_F_ACCESS_PLATFORM)) {
			dev_warn(&dev->dev,
				 "device must provide VIRTIO_F_ACCESS_PLATFORM\n");
			return -ENODEV;
		}
	}

	if (!virtio_has_feature(dev, VIRTIO_F_VERSION_1))
		return 0;

	virtio_add_status(dev, VIRTIO_CONFIG_S_FEATURES_OK);
	status = dev->config->get_status(dev);
	if (!(status & VIRTIO_CONFIG_S_FEATURES_OK)) {
		dev_err(&dev->dev, "virtio: device refuses features: %x\n",
			status);
		return -ENODEV;
	}
	return 0;
}

/**
 * virtio_reset_device - quiesce device for removal
 * @dev: the device to reset
 *
 * Prevents device from sending interrupts and accessing memory.
 *
 * Generally used for cleanup during driver / device removal.
 *
 * Once this has been invoked, caller must ensure that
 * virtqueue_notify / virtqueue_kick are not in progress.
 *
 * Note: this guarantees that vq callbacks are not in progress, however caller
 * is responsible for preventing access from other contexts, such as a system
 * call/workqueue/bh.  Invoking virtio_break_device then flushing any such
 * contexts is one way to handle that.
 * */
void virtio_reset_device(struct virtio_device *dev)
{
#ifdef CONFIG_VIRTIO_HARDEN_NOTIFICATION
	/*
	 * The below virtio_synchronize_cbs() guarantees that any
	 * interrupt for this line arriving after
	 * virtio_synchronize_vqs() has completed is guaranteed to see
	 * vq->broken as true.
	 */
	virtio_break_device(dev);
	virtio_synchronize_cbs(dev);
#endif

	dev->config->reset(dev);
}
EXPORT_SYMBOL_GPL(virtio_reset_device);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int virtio_dev_probe(struct device *_d)
{
	int err, i;
	struct virtio_device *dev = dev_to_virtio(_d);
<<<<<<< HEAD
	struct virtio_driver *drv = container_of(dev->dev.driver,
						 struct virtio_driver, driver);
	u32 device_features;

	/* We have a driver! */
	add_status(dev, VIRTIO_CONFIG_S_DRIVER);
=======
	struct virtio_driver *drv = drv_to_virtio(dev->dev.driver);
	u64 device_features;
	u64 driver_features;
	u64 driver_features_legacy;

	/* We have a driver! */
	virtio_add_status(dev, VIRTIO_CONFIG_S_DRIVER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Figure out what features the device supports. */
	device_features = dev->config->get_features(dev);

<<<<<<< HEAD
	/* Features supported by both device and driver into dev->features. */
	memset(dev->features, 0, sizeof(dev->features));
	for (i = 0; i < drv->feature_table_size; i++) {
		unsigned int f = drv->feature_table[i];
		BUG_ON(f >= 32);
		if (device_features & (1 << f))
			set_bit(f, dev->features);
	}

	/* Transport features always preserved to pass to finalize_features. */
	for (i = VIRTIO_TRANSPORT_F_START; i < VIRTIO_TRANSPORT_F_END; i++)
		if (device_features & (1 << i))
			set_bit(i, dev->features);

	dev->config->finalize_features(dev);

	err = drv->probe(dev);
	if (err)
		add_status(dev, VIRTIO_CONFIG_S_FAILED);
	else
		add_status(dev, VIRTIO_CONFIG_S_DRIVER_OK);

	return err;
}

static int virtio_dev_remove(struct device *_d)
{
	struct virtio_device *dev = dev_to_virtio(_d);
	struct virtio_driver *drv = container_of(dev->dev.driver,
						 struct virtio_driver, driver);

	drv->remove(dev);

	/* Driver should have reset device. */
	BUG_ON(dev->config->get_status(dev));

	/* Acknowledge the device's existence again. */
	add_status(dev, VIRTIO_CONFIG_S_ACKNOWLEDGE);
	return 0;
}

static struct bus_type virtio_bus = {
	.name  = "virtio",
	.match = virtio_dev_match,
	.dev_attrs = virtio_dev_attrs,
=======
	/* Figure out what features the driver supports. */
	driver_features = 0;
	for (i = 0; i < drv->feature_table_size; i++) {
		unsigned int f = drv->feature_table[i];
		BUG_ON(f >= 64);
		driver_features |= (1ULL << f);
	}

	/* Some drivers have a separate feature table for virtio v1.0 */
	if (drv->feature_table_legacy) {
		driver_features_legacy = 0;
		for (i = 0; i < drv->feature_table_size_legacy; i++) {
			unsigned int f = drv->feature_table_legacy[i];
			BUG_ON(f >= 64);
			driver_features_legacy |= (1ULL << f);
		}
	} else {
		driver_features_legacy = driver_features;
	}

	if (device_features & (1ULL << VIRTIO_F_VERSION_1))
		dev->features = driver_features & device_features;
	else
		dev->features = driver_features_legacy & device_features;

	/* Transport features always preserved to pass to finalize_features. */
	for (i = VIRTIO_TRANSPORT_F_START; i < VIRTIO_TRANSPORT_F_END; i++)
		if (device_features & (1ULL << i))
			__virtio_set_bit(dev, i);

	err = dev->config->finalize_features(dev);
	if (err)
		goto err;

	if (drv->validate) {
		u64 features = dev->features;

		err = drv->validate(dev);
		if (err)
			goto err;

		/* Did validation change any features? Then write them again. */
		if (features != dev->features) {
			err = dev->config->finalize_features(dev);
			if (err)
				goto err;
		}
	}

	err = virtio_features_ok(dev);
	if (err)
		goto err;

	if (dev->config->create_avq) {
		err = dev->config->create_avq(dev);
		if (err)
			goto err;
	}

	err = drv->probe(dev);
	if (err)
		goto err_probe;

	/* If probe didn't do it, mark device DRIVER_OK ourselves. */
	if (!(dev->config->get_status(dev) & VIRTIO_CONFIG_S_DRIVER_OK))
		virtio_device_ready(dev);

	if (drv->scan)
		drv->scan(dev);

	virtio_config_enable(dev);

	return 0;

err_probe:
	if (dev->config->destroy_avq)
		dev->config->destroy_avq(dev);
err:
	virtio_add_status(dev, VIRTIO_CONFIG_S_FAILED);
	return err;

}

static void virtio_dev_remove(struct device *_d)
{
	struct virtio_device *dev = dev_to_virtio(_d);
	struct virtio_driver *drv = drv_to_virtio(dev->dev.driver);

	virtio_config_disable(dev);

	drv->remove(dev);

	if (dev->config->destroy_avq)
		dev->config->destroy_avq(dev);

	/* Driver should have reset device. */
	WARN_ON_ONCE(dev->config->get_status(dev));

	/* Acknowledge the device's existence again. */
	virtio_add_status(dev, VIRTIO_CONFIG_S_ACKNOWLEDGE);

	of_node_put(dev->dev.of_node);
}

static const struct bus_type virtio_bus = {
	.name  = "virtio",
	.match = virtio_dev_match,
	.dev_groups = virtio_dev_groups,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.uevent = virtio_uevent,
	.probe = virtio_dev_probe,
	.remove = virtio_dev_remove,
};

<<<<<<< HEAD
int register_virtio_driver(struct virtio_driver *driver)
=======
int __register_virtio_driver(struct virtio_driver *driver, struct module *owner)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Catch this early. */
	BUG_ON(driver->feature_table_size && !driver->feature_table);
	driver->driver.bus = &virtio_bus;
<<<<<<< HEAD
	return driver_register(&driver->driver);
}
EXPORT_SYMBOL_GPL(register_virtio_driver);
=======
	driver->driver.owner = owner;

	return driver_register(&driver->driver);
}
EXPORT_SYMBOL_GPL(__register_virtio_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void unregister_virtio_driver(struct virtio_driver *driver)
{
	driver_unregister(&driver->driver);
}
EXPORT_SYMBOL_GPL(unregister_virtio_driver);

<<<<<<< HEAD
=======
static int virtio_device_of_init(struct virtio_device *dev)
{
	struct device_node *np, *pnode = dev_of_node(dev->dev.parent);
	char compat[] = "virtio,deviceXXXXXXXX";
	int ret, count;

	if (!pnode)
		return 0;

	count = of_get_available_child_count(pnode);
	if (!count)
		return 0;

	/* There can be only 1 child node */
	if (WARN_ON(count > 1))
		return -EINVAL;

	np = of_get_next_available_child(pnode, NULL);
	if (WARN_ON(!np))
		return -ENODEV;

	ret = snprintf(compat, sizeof(compat), "virtio,device%x", dev->id.device);
	BUG_ON(ret >= sizeof(compat));

	/*
	 * On powerpc/pseries virtio devices are PCI devices so PCI
	 * vendor/device ids play the role of the "compatible" property.
	 * Simply don't init of_node in this case.
	 */
	if (!of_device_is_compatible(np, compat)) {
		ret = 0;
		goto out;
	}

	dev->dev.of_node = np;
	return 0;

out:
	of_node_put(np);
	return ret;
}

/**
 * register_virtio_device - register virtio device
 * @dev        : virtio device to be registered
 *
 * On error, the caller must call put_device on &@dev->dev (and not kfree),
 * as another code path may have obtained a reference to @dev.
 *
 * Returns: 0 on suceess, -error on failure
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int register_virtio_device(struct virtio_device *dev)
{
	int err;

	dev->dev.bus = &virtio_bus;
<<<<<<< HEAD

	/* Assign a unique device index and hence name. */
	dev->index = dev_index++;
	dev_set_name(&dev->dev, "virtio%u", dev->index);

	/* We always start by resetting the device, in case a previous
	 * driver messed it up.  This also tests that code path a little. */
	dev->config->reset(dev);

	/* Acknowledge that we've seen the device. */
	add_status(dev, VIRTIO_CONFIG_S_ACKNOWLEDGE);

	INIT_LIST_HEAD(&dev->vqs);

	/* device_register() causes the bus infrastructure to look for a
	 * matching driver. */
	err = device_register(&dev->dev);
	if (err)
		add_status(dev, VIRTIO_CONFIG_S_FAILED);
=======
	device_initialize(&dev->dev);

	/* Assign a unique device index and hence name. */
	err = ida_alloc(&virtio_index_ida, GFP_KERNEL);
	if (err < 0)
		goto out;

	dev->index = err;
	err = dev_set_name(&dev->dev, "virtio%u", dev->index);
	if (err)
		goto out_ida_remove;

	err = virtio_device_of_init(dev);
	if (err)
		goto out_ida_remove;

	spin_lock_init(&dev->config_lock);
	dev->config_enabled = false;
	dev->config_change_pending = false;

	INIT_LIST_HEAD(&dev->vqs);
	spin_lock_init(&dev->vqs_list_lock);

	/* We always start by resetting the device, in case a previous
	 * driver messed it up.  This also tests that code path a little. */
	virtio_reset_device(dev);

	/* Acknowledge that we've seen the device. */
	virtio_add_status(dev, VIRTIO_CONFIG_S_ACKNOWLEDGE);

	/*
	 * device_add() causes the bus infrastructure to look for a matching
	 * driver.
	 */
	err = device_add(&dev->dev);
	if (err)
		goto out_of_node_put;

	return 0;

out_of_node_put:
	of_node_put(dev->dev.of_node);
out_ida_remove:
	ida_free(&virtio_index_ida, dev->index);
out:
	virtio_add_status(dev, VIRTIO_CONFIG_S_FAILED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(register_virtio_device);

<<<<<<< HEAD
void unregister_virtio_device(struct virtio_device *dev)
{
	device_unregister(&dev->dev);
}
EXPORT_SYMBOL_GPL(unregister_virtio_device);

=======
bool is_virtio_device(struct device *dev)
{
	return dev->bus == &virtio_bus;
}
EXPORT_SYMBOL_GPL(is_virtio_device);

void unregister_virtio_device(struct virtio_device *dev)
{
	int index = dev->index; /* save for after device release */

	device_unregister(&dev->dev);
	ida_free(&virtio_index_ida, index);
}
EXPORT_SYMBOL_GPL(unregister_virtio_device);

#ifdef CONFIG_PM_SLEEP
int virtio_device_freeze(struct virtio_device *dev)
{
	struct virtio_driver *drv = drv_to_virtio(dev->dev.driver);
	int ret;

	virtio_config_disable(dev);

	dev->failed = dev->config->get_status(dev) & VIRTIO_CONFIG_S_FAILED;

	if (drv && drv->freeze) {
		ret = drv->freeze(dev);
		if (ret) {
			virtio_config_enable(dev);
			return ret;
		}
	}

	if (dev->config->destroy_avq)
		dev->config->destroy_avq(dev);

	return 0;
}
EXPORT_SYMBOL_GPL(virtio_device_freeze);

int virtio_device_restore(struct virtio_device *dev)
{
	struct virtio_driver *drv = drv_to_virtio(dev->dev.driver);
	int ret;

	/* We always start by resetting the device, in case a previous
	 * driver messed it up. */
	virtio_reset_device(dev);

	/* Acknowledge that we've seen the device. */
	virtio_add_status(dev, VIRTIO_CONFIG_S_ACKNOWLEDGE);

	/* Maybe driver failed before freeze.
	 * Restore the failed status, for debugging. */
	if (dev->failed)
		virtio_add_status(dev, VIRTIO_CONFIG_S_FAILED);

	if (!drv)
		return 0;

	/* We have a driver! */
	virtio_add_status(dev, VIRTIO_CONFIG_S_DRIVER);

	ret = dev->config->finalize_features(dev);
	if (ret)
		goto err;

	ret = virtio_features_ok(dev);
	if (ret)
		goto err;

	if (dev->config->create_avq) {
		ret = dev->config->create_avq(dev);
		if (ret)
			goto err;
	}

	if (drv->restore) {
		ret = drv->restore(dev);
		if (ret)
			goto err_restore;
	}

	/* If restore didn't do it, mark device DRIVER_OK ourselves. */
	if (!(dev->config->get_status(dev) & VIRTIO_CONFIG_S_DRIVER_OK))
		virtio_device_ready(dev);

	virtio_config_enable(dev);

	return 0;

err_restore:
	if (dev->config->destroy_avq)
		dev->config->destroy_avq(dev);
err:
	virtio_add_status(dev, VIRTIO_CONFIG_S_FAILED);
	return ret;
}
EXPORT_SYMBOL_GPL(virtio_device_restore);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int virtio_init(void)
{
	if (bus_register(&virtio_bus) != 0)
		panic("virtio bus registration failed");
	return 0;
}

static void __exit virtio_exit(void)
{
	bus_unregister(&virtio_bus);
<<<<<<< HEAD
=======
	ida_destroy(&virtio_index_ida);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
core_initcall(virtio_init);
module_exit(virtio_exit);

MODULE_LICENSE("GPL");
