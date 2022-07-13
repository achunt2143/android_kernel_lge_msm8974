<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * class.c - basic device class management
 *
 * Copyright (c) 2002-3 Patrick Mochel
 * Copyright (c) 2002-3 Open Source Development Labs
 * Copyright (c) 2003-2004 Greg Kroah-Hartman
 * Copyright (c) 2003-2004 IBM Corp.
<<<<<<< HEAD
 *
 * This file is released under the GPLv2
 *
 */

=======
 */

#include <linux/device/class.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/string.h>
#include <linux/kdev_t.h>
#include <linux/err.h>
#include <linux/slab.h>
<<<<<<< HEAD
#include <linux/genhd.h>
#include <linux/mutex.h>
#include "base.h"

#define to_class_attr(_attr) container_of(_attr, struct class_attribute, attr)

=======
#include <linux/blkdev.h>
#include <linux/mutex.h>
#include "base.h"

/* /sys/class */
static struct kset *class_kset;

#define to_class_attr(_attr) container_of(_attr, struct class_attribute, attr)

/**
 * class_to_subsys - Turn a struct class into a struct subsys_private
 *
 * @class: pointer to the struct bus_type to look up
 *
 * The driver core internals need to work on the subsys_private structure, not
 * the external struct class pointer.  This function walks the list of
 * registered classes in the system and finds the matching one and returns the
 * internal struct subsys_private that relates to that class.
 *
 * Note, the reference count of the return value is INCREMENTED if it is not
 * NULL.  A call to subsys_put() must be done when finished with the pointer in
 * order for it to be properly freed.
 */
struct subsys_private *class_to_subsys(const struct class *class)
{
	struct subsys_private *sp = NULL;
	struct kobject *kobj;

	if (!class || !class_kset)
		return NULL;

	spin_lock(&class_kset->list_lock);

	if (list_empty(&class_kset->list))
		goto done;

	list_for_each_entry(kobj, &class_kset->list, entry) {
		struct kset *kset = container_of(kobj, struct kset, kobj);

		sp = container_of_const(kset, struct subsys_private, subsys);
		if (sp->class == class)
			goto done;
	}
	sp = NULL;
done:
	sp = subsys_get(sp);
	spin_unlock(&class_kset->list_lock);
	return sp;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t class_attr_show(struct kobject *kobj, struct attribute *attr,
			       char *buf)
{
	struct class_attribute *class_attr = to_class_attr(attr);
	struct subsys_private *cp = to_subsys_private(kobj);
	ssize_t ret = -EIO;

	if (class_attr->show)
		ret = class_attr->show(cp->class, class_attr, buf);
	return ret;
}

static ssize_t class_attr_store(struct kobject *kobj, struct attribute *attr,
				const char *buf, size_t count)
{
	struct class_attribute *class_attr = to_class_attr(attr);
	struct subsys_private *cp = to_subsys_private(kobj);
	ssize_t ret = -EIO;

	if (class_attr->store)
		ret = class_attr->store(cp->class, class_attr, buf, count);
	return ret;
}

<<<<<<< HEAD
static const void *class_attr_namespace(struct kobject *kobj,
					const struct attribute *attr)
{
	struct class_attribute *class_attr = to_class_attr(attr);
	struct subsys_private *cp = to_subsys_private(kobj);
	const void *ns = NULL;

	if (class_attr->namespace)
		ns = class_attr->namespace(cp->class, class_attr);
	return ns;
}

static void class_release(struct kobject *kobj)
{
	struct subsys_private *cp = to_subsys_private(kobj);
	struct class *class = cp->class;
=======
static void class_release(struct kobject *kobj)
{
	struct subsys_private *cp = to_subsys_private(kobj);
	const struct class *class = cp->class;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("class '%s': release.\n", class->name);

	if (class->class_release)
		class->class_release(class);
	else
		pr_debug("class '%s' does not have a release() function, "
			 "be careful\n", class->name);

<<<<<<< HEAD
	kfree(cp);
}

static const struct kobj_ns_type_operations *class_child_ns_type(struct kobject *kobj)
{
	struct subsys_private *cp = to_subsys_private(kobj);
	struct class *class = cp->class;
=======
	lockdep_unregister_key(&cp->lock_key);
	kfree(cp);
}

static const struct kobj_ns_type_operations *class_child_ns_type(const struct kobject *kobj)
{
	const struct subsys_private *cp = to_subsys_private(kobj);
	const struct class *class = cp->class;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return class->ns_type;
}

static const struct sysfs_ops class_sysfs_ops = {
	.show	   = class_attr_show,
	.store	   = class_attr_store,
<<<<<<< HEAD
	.namespace = class_attr_namespace,
};

static struct kobj_type class_ktype = {
=======
};

static const struct kobj_type class_ktype = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.sysfs_ops	= &class_sysfs_ops,
	.release	= class_release,
	.child_ns_type	= class_child_ns_type,
};

<<<<<<< HEAD
/* Hotplug events for classes go to the class subsys */
static struct kset *class_kset;


int class_create_file(struct class *cls, const struct class_attribute *attr)
{
	int error;
	if (cls)
		error = sysfs_create_file(&cls->p->subsys.kobj,
					  &attr->attr);
	else
		error = -EINVAL;
	return error;
}

void class_remove_file(struct class *cls, const struct class_attribute *attr)
{
	if (cls)
		sysfs_remove_file(&cls->p->subsys.kobj, &attr->attr);
}

static struct class *class_get(struct class *cls)
{
	if (cls)
		kset_get(&cls->p->subsys);
	return cls;
}

static void class_put(struct class *cls)
{
	if (cls)
		kset_put(&cls->p->subsys);
}

static int add_class_attrs(struct class *cls)
{
	int i;
	int error = 0;

	if (cls->class_attrs) {
		for (i = 0; attr_name(cls->class_attrs[i]); i++) {
			error = class_create_file(cls, &cls->class_attrs[i]);
			if (error)
				goto error;
		}
	}
done:
	return error;
error:
	while (--i >= 0)
		class_remove_file(cls, &cls->class_attrs[i]);
	goto done;
}

static void remove_class_attrs(struct class *cls)
{
	int i;

	if (cls->class_attrs) {
		for (i = 0; attr_name(cls->class_attrs[i]); i++)
			class_remove_file(cls, &cls->class_attrs[i]);
	}
=======
int class_create_file_ns(const struct class *cls, const struct class_attribute *attr,
			 const void *ns)
{
	struct subsys_private *sp = class_to_subsys(cls);
	int error;

	if (!sp)
		return -EINVAL;

	error = sysfs_create_file_ns(&sp->subsys.kobj, &attr->attr, ns);
	subsys_put(sp);

	return error;
}
EXPORT_SYMBOL_GPL(class_create_file_ns);

void class_remove_file_ns(const struct class *cls, const struct class_attribute *attr,
			  const void *ns)
{
	struct subsys_private *sp = class_to_subsys(cls);

	if (!sp)
		return;

	sysfs_remove_file_ns(&sp->subsys.kobj, &attr->attr, ns);
	subsys_put(sp);
}
EXPORT_SYMBOL_GPL(class_remove_file_ns);

static struct device *klist_class_to_dev(struct klist_node *n)
{
	struct device_private *p = to_device_private_class(n);
	return p->device;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void klist_class_dev_get(struct klist_node *n)
{
<<<<<<< HEAD
	struct device *dev = container_of(n, struct device, knode_class);
=======
	struct device *dev = klist_class_to_dev(n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	get_device(dev);
}

static void klist_class_dev_put(struct klist_node *n)
{
<<<<<<< HEAD
	struct device *dev = container_of(n, struct device, knode_class);
=======
	struct device *dev = klist_class_to_dev(n);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	put_device(dev);
}

<<<<<<< HEAD
int __class_register(struct class *cls, struct lock_class_key *key)
{
	struct subsys_private *cp;
=======
int class_register(const struct class *cls)
{
	struct subsys_private *cp;
	struct lock_class_key *key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int error;

	pr_debug("device class '%s': registering\n", cls->name);

	cp = kzalloc(sizeof(*cp), GFP_KERNEL);
	if (!cp)
		return -ENOMEM;
	klist_init(&cp->klist_devices, klist_class_dev_get, klist_class_dev_put);
	INIT_LIST_HEAD(&cp->interfaces);
	kset_init(&cp->glue_dirs);
<<<<<<< HEAD
	__mutex_init(&cp->mutex, "subsys mutex", key);
	error = kobject_set_name(&cp->subsys.kobj, "%s", cls->name);
	if (error) {
		kfree(cp);
		return error;
	}

	/* set the default /sys/dev directory for devices of this class */
	if (!cls->dev_kobj)
		cls->dev_kobj = sysfs_dev_char_kobj;

#if defined(CONFIG_BLOCK)
	/* let the block class directory show up in the root of sysfs */
	if (!sysfs_deprecated || cls != &block_class)
		cp->subsys.kobj.kset = class_kset;
#else
	cp->subsys.kobj.kset = class_kset;
#endif
	cp->subsys.kobj.ktype = &class_ktype;
	cp->class = cls;
	cls->p = cp;

	error = kset_register(&cp->subsys);
	if (error) {
		kfree(cp);
		return error;
	}
	error = add_class_attrs(class_get(cls));
	class_put(cls);
	return error;
}
EXPORT_SYMBOL_GPL(__class_register);

void class_unregister(struct class *cls)
{
	pr_debug("device class '%s': unregistering\n", cls->name);
	remove_class_attrs(cls);
	kset_unregister(&cls->p->subsys);
}

static void class_create_release(struct class *cls)
=======
	key = &cp->lock_key;
	lockdep_register_key(key);
	__mutex_init(&cp->mutex, "subsys mutex", key);
	error = kobject_set_name(&cp->subsys.kobj, "%s", cls->name);
	if (error)
		goto err_out;

	cp->subsys.kobj.kset = class_kset;
	cp->subsys.kobj.ktype = &class_ktype;
	cp->class = cls;

	error = kset_register(&cp->subsys);
	if (error)
		goto err_out;

	error = sysfs_create_groups(&cp->subsys.kobj, cls->class_groups);
	if (error) {
		kobject_del(&cp->subsys.kobj);
		kfree_const(cp->subsys.kobj.name);
		goto err_out;
	}
	return 0;

err_out:
	lockdep_unregister_key(key);
	kfree(cp);
	return error;
}
EXPORT_SYMBOL_GPL(class_register);

void class_unregister(const struct class *cls)
{
	struct subsys_private *sp = class_to_subsys(cls);

	if (!sp)
		return;

	pr_debug("device class '%s': unregistering\n", cls->name);

	sysfs_remove_groups(&sp->subsys.kobj, cls->class_groups);
	kset_unregister(&sp->subsys);
	subsys_put(sp);
}
EXPORT_SYMBOL_GPL(class_unregister);

static void class_create_release(const struct class *cls)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pr_debug("%s called for %s\n", __func__, cls->name);
	kfree(cls);
}

/**
 * class_create - create a struct class structure
<<<<<<< HEAD
 * @owner: pointer to the module that is to "own" this struct class
 * @name: pointer to a string for the name of this class.
 * @key: the lock_class_key for this class; used by mutex lock debugging
=======
 * @name: pointer to a string for the name of this class.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This is used to create a struct class pointer that can then be used
 * in calls to device_create().
 *
 * Returns &struct class pointer on success, or ERR_PTR() on error.
 *
 * Note, the pointer created here is to be destroyed when finished by
 * making a call to class_destroy().
 */
<<<<<<< HEAD
struct class *__class_create(struct module *owner, const char *name,
			     struct lock_class_key *key)
=======
struct class *class_create(const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct class *cls;
	int retval;

	cls = kzalloc(sizeof(*cls), GFP_KERNEL);
	if (!cls) {
		retval = -ENOMEM;
		goto error;
	}

	cls->name = name;
<<<<<<< HEAD
	cls->owner = owner;
	cls->class_release = class_create_release;

	retval = __class_register(cls, key);
=======
	cls->class_release = class_create_release;

	retval = class_register(cls);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (retval)
		goto error;

	return cls;

error:
	kfree(cls);
	return ERR_PTR(retval);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(__class_create);
=======
EXPORT_SYMBOL_GPL(class_create);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * class_destroy - destroys a struct class structure
 * @cls: pointer to the struct class that is to be destroyed
 *
 * Note, the pointer to be destroyed must have been created with a call
 * to class_create().
 */
<<<<<<< HEAD
void class_destroy(struct class *cls)
{
	if ((cls == NULL) || (IS_ERR(cls)))
=======
void class_destroy(const struct class *cls)
{
	if (IS_ERR_OR_NULL(cls))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	class_unregister(cls);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(class_destroy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * class_dev_iter_init - initialize class device iterator
 * @iter: class iterator to initialize
 * @class: the class we wanna iterate over
 * @start: the device to start iterating from, if any
 * @type: device_type of the devices to iterate over, NULL for all
 *
 * Initialize class iterator @iter such that it iterates over devices
 * of @class.  If @start is set, the list iteration will start there,
 * otherwise if it is NULL, the iteration starts at the beginning of
 * the list.
 */
<<<<<<< HEAD
void class_dev_iter_init(struct class_dev_iter *iter, struct class *class,
			 struct device *start, const struct device_type *type)
{
	struct klist_node *start_knode = NULL;

	if (start)
		start_knode = &start->knode_class;
	klist_iter_init_node(&class->p->klist_devices, &iter->ki, start_knode);
	iter->type = type;
=======
void class_dev_iter_init(struct class_dev_iter *iter, const struct class *class,
			 const struct device *start, const struct device_type *type)
{
	struct subsys_private *sp = class_to_subsys(class);
	struct klist_node *start_knode = NULL;

	if (!sp)
		return;

	if (start)
		start_knode = &start->p->knode_class;
	klist_iter_init_node(&sp->klist_devices, &iter->ki, start_knode);
	iter->type = type;
	iter->sp = sp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(class_dev_iter_init);

/**
 * class_dev_iter_next - iterate to the next device
 * @iter: class iterator to proceed
 *
 * Proceed @iter to the next device and return it.  Returns NULL if
 * iteration is complete.
 *
 * The returned device is referenced and won't be released till
 * iterator is proceed to the next device or exited.  The caller is
 * free to do whatever it wants to do with the device including
 * calling back into class code.
 */
struct device *class_dev_iter_next(struct class_dev_iter *iter)
{
	struct klist_node *knode;
	struct device *dev;

	while (1) {
		knode = klist_next(&iter->ki);
		if (!knode)
			return NULL;
<<<<<<< HEAD
		dev = container_of(knode, struct device, knode_class);
=======
		dev = klist_class_to_dev(knode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!iter->type || iter->type == dev->type)
			return dev;
	}
}
EXPORT_SYMBOL_GPL(class_dev_iter_next);

/**
 * class_dev_iter_exit - finish iteration
 * @iter: class iterator to finish
 *
 * Finish an iteration.  Always call this function after iteration is
 * complete whether the iteration ran till the end or not.
 */
void class_dev_iter_exit(struct class_dev_iter *iter)
{
	klist_iter_exit(&iter->ki);
<<<<<<< HEAD
=======
	subsys_put(iter->sp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(class_dev_iter_exit);

/**
 * class_for_each_device - device iterator
 * @class: the class we're iterating
 * @start: the device to start with in the list, if any.
 * @data: data for the callback
 * @fn: function to be called for each device
 *
 * Iterate over @class's list of devices, and call @fn for each,
 * passing it @data.  If @start is set, the list iteration will start
 * there, otherwise if it is NULL, the iteration starts at the
 * beginning of the list.
 *
 * We check the return of @fn each time. If it returns anything
 * other than 0, we break out and return that value.
 *
 * @fn is allowed to do anything including calling back into class
 * code.  There's no locking restriction.
 */
<<<<<<< HEAD
int class_for_each_device(struct class *class, struct device *start,
			  void *data, int (*fn)(struct device *, void *))
{
=======
int class_for_each_device(const struct class *class, const struct device *start,
			  void *data, int (*fn)(struct device *, void *))
{
	struct subsys_private *sp = class_to_subsys(class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct class_dev_iter iter;
	struct device *dev;
	int error = 0;

	if (!class)
		return -EINVAL;
<<<<<<< HEAD
	if (!class->p) {
=======
	if (!sp) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		WARN(1, "%s called for class '%s' before it was initialized",
		     __func__, class->name);
		return -EINVAL;
	}

	class_dev_iter_init(&iter, class, start, NULL);
	while ((dev = class_dev_iter_next(&iter))) {
		error = fn(dev, data);
		if (error)
			break;
	}
	class_dev_iter_exit(&iter);
<<<<<<< HEAD
=======
	subsys_put(sp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return error;
}
EXPORT_SYMBOL_GPL(class_for_each_device);

/**
 * class_find_device - device iterator for locating a particular device
 * @class: the class we're iterating
 * @start: Device to begin with
 * @data: data for the match function
 * @match: function to check device
 *
 * This is similar to the class_for_each_dev() function above, but it
 * returns a reference to a device that is 'found' for later use, as
 * determined by the @match callback.
 *
 * The callback should return 0 if the device doesn't match and non-zero
 * if it does.  If the callback returns non-zero, this function will
 * return to the caller and not iterate over any more devices.
 *
 * Note, you will need to drop the reference with put_device() after use.
 *
<<<<<<< HEAD
 * @fn is allowed to do anything including calling back into class
 * code.  There's no locking restriction.
 */
struct device *class_find_device(struct class *class, struct device *start,
				 void *data,
				 int (*match)(struct device *, void *))
{
=======
 * @match is allowed to do anything including calling back into class
 * code.  There's no locking restriction.
 */
struct device *class_find_device(const struct class *class, const struct device *start,
				 const void *data,
				 int (*match)(struct device *, const void *))
{
	struct subsys_private *sp = class_to_subsys(class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct class_dev_iter iter;
	struct device *dev;

	if (!class)
		return NULL;
<<<<<<< HEAD
	if (!class->p) {
=======
	if (!sp) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		WARN(1, "%s called for class '%s' before it was initialized",
		     __func__, class->name);
		return NULL;
	}

	class_dev_iter_init(&iter, class, start, NULL);
	while ((dev = class_dev_iter_next(&iter))) {
		if (match(dev, data)) {
			get_device(dev);
			break;
		}
	}
	class_dev_iter_exit(&iter);
<<<<<<< HEAD
=======
	subsys_put(sp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return dev;
}
EXPORT_SYMBOL_GPL(class_find_device);

int class_interface_register(struct class_interface *class_intf)
{
<<<<<<< HEAD
	struct class *parent;
=======
	struct subsys_private *sp;
	const struct class *parent;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct class_dev_iter iter;
	struct device *dev;

	if (!class_intf || !class_intf->class)
		return -ENODEV;

<<<<<<< HEAD
	parent = class_get(class_intf->class);
	if (!parent)
		return -EINVAL;

	mutex_lock(&parent->p->mutex);
	list_add_tail(&class_intf->node, &parent->p->interfaces);
	if (class_intf->add_dev) {
		class_dev_iter_init(&iter, parent, NULL, NULL);
		while ((dev = class_dev_iter_next(&iter)))
			class_intf->add_dev(dev, class_intf);
		class_dev_iter_exit(&iter);
	}
	mutex_unlock(&parent->p->mutex);

	return 0;
}

void class_interface_unregister(struct class_interface *class_intf)
{
	struct class *parent = class_intf->class;
=======
	parent = class_intf->class;
	sp = class_to_subsys(parent);
	if (!sp)
		return -EINVAL;

	/*
	 * Reference in sp is now incremented and will be dropped when
	 * the interface is removed in the call to class_interface_unregister()
	 */

	mutex_lock(&sp->mutex);
	list_add_tail(&class_intf->node, &sp->interfaces);
	if (class_intf->add_dev) {
		class_dev_iter_init(&iter, parent, NULL, NULL);
		while ((dev = class_dev_iter_next(&iter)))
			class_intf->add_dev(dev);
		class_dev_iter_exit(&iter);
	}
	mutex_unlock(&sp->mutex);

	return 0;
}
EXPORT_SYMBOL_GPL(class_interface_register);

void class_interface_unregister(struct class_interface *class_intf)
{
	struct subsys_private *sp;
	const struct class *parent = class_intf->class;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct class_dev_iter iter;
	struct device *dev;

	if (!parent)
		return;

<<<<<<< HEAD
	mutex_lock(&parent->p->mutex);
=======
	sp = class_to_subsys(parent);
	if (!sp)
		return;

	mutex_lock(&sp->mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	list_del_init(&class_intf->node);
	if (class_intf->remove_dev) {
		class_dev_iter_init(&iter, parent, NULL, NULL);
		while ((dev = class_dev_iter_next(&iter)))
<<<<<<< HEAD
			class_intf->remove_dev(dev, class_intf);
		class_dev_iter_exit(&iter);
	}
	mutex_unlock(&parent->p->mutex);

	class_put(parent);
}

ssize_t show_class_attr_string(struct class *class,
			       struct class_attribute *attr, char *buf)
{
	struct class_attribute_string *cs;
	cs = container_of(attr, struct class_attribute_string, attr);
	return snprintf(buf, PAGE_SIZE, "%s\n", cs->str);
=======
			class_intf->remove_dev(dev);
		class_dev_iter_exit(&iter);
	}
	mutex_unlock(&sp->mutex);

	/*
	 * Decrement the reference count twice, once for the class_to_subsys()
	 * call in the start of this function, and the second one from the
	 * reference increment in class_interface_register()
	 */
	subsys_put(sp);
	subsys_put(sp);
}
EXPORT_SYMBOL_GPL(class_interface_unregister);

ssize_t show_class_attr_string(const struct class *class,
			       const struct class_attribute *attr, char *buf)
{
	struct class_attribute_string *cs;

	cs = container_of(attr, struct class_attribute_string, attr);
	return sysfs_emit(buf, "%s\n", cs->str);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL_GPL(show_class_attr_string);

struct class_compat {
	struct kobject *kobj;
};

/**
 * class_compat_register - register a compatibility class
 * @name: the name of the class
 *
 * Compatibility class are meant as a temporary user-space compatibility
 * workaround when converting a family of class devices to a bus devices.
 */
struct class_compat *class_compat_register(const char *name)
{
	struct class_compat *cls;

	cls = kmalloc(sizeof(struct class_compat), GFP_KERNEL);
	if (!cls)
		return NULL;
	cls->kobj = kobject_create_and_add(name, &class_kset->kobj);
	if (!cls->kobj) {
		kfree(cls);
		return NULL;
	}
	return cls;
}
EXPORT_SYMBOL_GPL(class_compat_register);

/**
 * class_compat_unregister - unregister a compatibility class
 * @cls: the class to unregister
 */
void class_compat_unregister(struct class_compat *cls)
{
	kobject_put(cls->kobj);
	kfree(cls);
}
EXPORT_SYMBOL_GPL(class_compat_unregister);

/**
 * class_compat_create_link - create a compatibility class device link to
 *			      a bus device
 * @cls: the compatibility class
 * @dev: the target bus device
 * @device_link: an optional device to which a "device" link should be created
 */
int class_compat_create_link(struct class_compat *cls, struct device *dev,
			     struct device *device_link)
{
	int error;

	error = sysfs_create_link(cls->kobj, &dev->kobj, dev_name(dev));
	if (error)
		return error;

	/*
	 * Optionally add a "device" link (typically to the parent), as a
	 * class device would have one and we want to provide as much
	 * backwards compatibility as possible.
	 */
	if (device_link) {
		error = sysfs_create_link(&dev->kobj, &device_link->kobj,
					  "device");
		if (error)
			sysfs_remove_link(cls->kobj, dev_name(dev));
	}

	return error;
}
EXPORT_SYMBOL_GPL(class_compat_create_link);

/**
 * class_compat_remove_link - remove a compatibility class device link to
 *			      a bus device
 * @cls: the compatibility class
 * @dev: the target bus device
 * @device_link: an optional device to which a "device" link was previously
 * 		 created
 */
void class_compat_remove_link(struct class_compat *cls, struct device *dev,
			      struct device *device_link)
{
	if (device_link)
		sysfs_remove_link(&dev->kobj, "device");
	sysfs_remove_link(cls->kobj, dev_name(dev));
}
EXPORT_SYMBOL_GPL(class_compat_remove_link);

<<<<<<< HEAD
=======
/**
 * class_is_registered - determine if at this moment in time, a class is
 *			 registered in the driver core or not.
 * @class: the class to check
 *
 * Returns a boolean to state if the class is registered in the driver core
 * or not.  Note that the value could switch right after this call is made,
 * so only use this in places where you "know" it is safe to do so (usually
 * to determine if the specific class has been registered yet or not).
 *
 * Be careful in using this.
 */
bool class_is_registered(const struct class *class)
{
	struct subsys_private *sp = class_to_subsys(class);
	bool is_initialized = false;

	if (sp) {
		is_initialized = true;
		subsys_put(sp);
	}
	return is_initialized;
}
EXPORT_SYMBOL_GPL(class_is_registered);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int __init classes_init(void)
{
	class_kset = kset_create_and_add("class", NULL, NULL);
	if (!class_kset)
		return -ENOMEM;
	return 0;
}
<<<<<<< HEAD

EXPORT_SYMBOL_GPL(class_create_file);
EXPORT_SYMBOL_GPL(class_remove_file);
EXPORT_SYMBOL_GPL(class_unregister);
EXPORT_SYMBOL_GPL(class_destroy);

EXPORT_SYMBOL_GPL(class_interface_register);
EXPORT_SYMBOL_GPL(class_interface_unregister);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
